#include "app_modules_config.h"
#if CSP_ENABLE_LOG

#include <kservice.h>
#include <ringblk_buf.h>

/**
 * ring block buffer object initialization
 *
 * @param rbb ring block buffer object
 * @param buf buffer
 * @param buf_size buffer size
 * @param block_set block set
 * @param blk_max_num max block number
 *
 * @note When your application need align access, please make the buffer address is aligned.
 */
void rbb_init(rbb_t rbb, uint8_t *buf, size_t buf_size, rbb_blk_t block_set, size_t blk_max_num)
{
    size_t i;

    TSC_ASSERT(rbb);
    TSC_ASSERT(buf);
    TSC_ASSERT(block_set);

    rbb->buf = buf;
    rbb->buf_size = buf_size;
    rbb->blk_set = block_set;
    rbb->blk_max_num = blk_max_num;
    rbb->tail = &rbb->blk_list;
    tsc_slist_init(&rbb->blk_list);
    tsc_slist_init(&rbb->free_list);
    /* initialize block status */
    for (i = 0; i < blk_max_num; i++)
    {
        block_set[i].status = RBB_BLK_UNUSED;
        tsc_slist_init(&block_set[i].list);
        tsc_slist_insert(&rbb->free_list, &block_set[i].list);
    }
}

#ifdef TSC_USING_HEAP

/**
 * ring block buffer object create
 *
 * @param buf_size buffer size
 * @param blk_max_num max block number
 *
 * @return != NULL: ring block buffer object
 *            NULL: create failed
 */
rbb_t rbb_create(size_t buf_size, size_t blk_max_num)
{
    rbb_t rbb = NULL;
    uint8_t *buf;
    rbb_blk_t blk_set;

    rbb = (rbb_t)tsc_malloc(sizeof(struct rbb));
    if (!rbb)
    {
        return NULL;
    }

    buf = (uint8_t *)tsc_malloc(buf_size);
    if (!buf)
    {
        tsc_free(rbb);
        return NULL;
    }

    blk_set = (rbb_blk_t)tsc_malloc(sizeof(struct rbb_blk) * blk_max_num);
    if (!blk_set)
    {
        tsc_free(buf);
        tsc_free(rbb);
        return NULL;
    }

    rbb_init(rbb, buf, buf_size, blk_set, blk_max_num);

    return rbb;
}

/**
 * ring block buffer object destroy
 *
 * @param rbb ring block buffer object
 */
void rbb_destroy(rbb_t rbb)
{
    TSC_ASSERT(rbb);

    tsc_free(rbb->buf);
    tsc_free(rbb->blk_set);
    tsc_free(rbb);

}

#endif

static rbb_blk_t find_empty_blk_in_set(rbb_t rbb)
{
    struct rbb_blk *blk;

    TSC_ASSERT(rbb);

    if (tsc_slist_isempty(&rbb->free_list))
    {
        return NULL;
    }
    blk = tsc_slist_first_entry(&rbb->free_list, struct rbb_blk, list);
    tsc_slist_remove(&rbb->free_list, &blk->list);
    TSC_ASSERT(blk->status == RBB_BLK_UNUSED);
    return blk;
}

static inline void list_append(rbb_t rbb, tsc_slist_t* n)
{
    /* append the node to the tail */
    rbb->tail->next = n;
    n->next = NULL;
    /* save tail node */
    rbb->tail = n;
}

static inline tsc_slist_t *list_remove(rbb_t rbb, tsc_slist_t* n)
{
    tsc_slist_t* l = &rbb->blk_list;
    struct tsc_slist_node* node = l;

    /* remove slist head */
    while (node->next && node->next != n) node = node->next;
    /* remove node */
    if (node->next != (tsc_slist_t*)0)
    {
        node->next = node->next->next;
        n->next = NULL;
        /* update tail node */
        if (rbb->tail == n)
            rbb->tail = node;
    }
    return l;
}

/**
 * Allocate a block by given size. The block will add to blk_list when allocate success.
 *
 * @param rbb ring block buffer object
 * @param blk_size block size
 *
 * @note When your application need align access, please make the blk_szie is aligned.
 *
 * @return != NULL: allocated block
 *            NULL: allocate failed
 */
rbb_blk_t rbb_blk_alloc(rbb_t rbb, size_t blk_size)
{
    long level;
    size_t empty1 = 0, empty2 = 0;
    rbb_blk_t head, tail, new_rbb = NULL;

    TSC_ASSERT(rbb);
    TSC_ASSERT(blk_size < (1L << 24));

    level = hw_interrupt_disable();

    new_rbb = find_empty_blk_in_set(rbb);

    if (new_rbb)
    {
        if (tsc_slist_isempty(&rbb->blk_list) == 0)
        {
            head = tsc_slist_first_entry(&rbb->blk_list, struct rbb_blk, list);
            /* get tail rbb blk object */
            tail = tsc_slist_entry(rbb->tail, struct rbb_blk, list);
            if (head->buf <= tail->buf)
            {
                /**
                 *                      head                     tail
                 * +--------------------------------------+-----------------+------------------+
                 * |      empty2     | block1 |   block2  |      block3     |       empty1     |
                 * +--------------------------------------+-----------------+------------------+
                 *                            rbb->buf
                 */
                empty1 = (rbb->buf + rbb->buf_size) - (tail->buf + tail->size);
                empty2 = head->buf - rbb->buf;

                if (empty1 >= blk_size)
                {
                    list_append(rbb, &new_rbb->list);
                    new_rbb->status = RBB_BLK_INITED;
                    new_rbb->buf = tail->buf + tail->size;
                    new_rbb->size = blk_size;
                }
                else if (empty2 >= blk_size)
                {
                    list_append(rbb, &new_rbb->list);
                    new_rbb->status = RBB_BLK_INITED;
                    new_rbb->buf = rbb->buf;
                    new_rbb->size = blk_size;
                }
                else
                {
                    /* no space */
                    new_rbb = NULL;
                }
            }
            else
            {
                /**
                 *        tail                                              head
                 * +----------------+-------------------------------------+--------+-----------+
                 * |     block3     |                empty1               | block1 |  block2   |
                 * +----------------+-------------------------------------+--------+-----------+
                 *                            rbb->buf
                 */
                empty1 = head->buf - (tail->buf + tail->size);

                if (empty1 >= blk_size)
                {
                    list_append(rbb, &new_rbb->list);
                    new_rbb->status = RBB_BLK_INITED;
                    new_rbb->buf = tail->buf + tail->size;
                    new_rbb->size = blk_size;
                }
                else
                {
                    /* no space */
                    new_rbb = NULL;
                }
            }
        }
        else
        {
            /* the list is empty */
            list_append(rbb, &new_rbb->list);
            new_rbb->status = RBB_BLK_INITED;
            new_rbb->buf = rbb->buf;
            new_rbb->size = blk_size;
        }
    }
    else
    {
        new_rbb = NULL;
    }

    hw_interrupt_enable(level);

    return new_rbb;
}

/**
 * put a block to ring block buffer object
 *
 * @param block the block
 */
void rbb_blk_put(rbb_blk_t block)
{
    TSC_ASSERT(block);
    TSC_ASSERT(block->status == RBB_BLK_INITED);

    block->status = RBB_BLK_PUT;
}

/**
 * get a block from the ring block buffer object
 *
 * @param rbb ring block buffer object
 *
 * @return != NULL: block
 *            NULL: get failed
 */
rbb_blk_t rbb_blk_get(rbb_t rbb)
{
    long level;
    rbb_blk_t block = NULL;
    tsc_slist_t *node;

    TSC_ASSERT(rbb);

    if (tsc_slist_isempty(&rbb->blk_list))
        return 0;

    level = hw_interrupt_disable();

    for (node = tsc_slist_first(&rbb->blk_list); node; node = tsc_slist_next(node))
    {
        block = tsc_slist_entry(node, struct rbb_blk, list);
        if (block->status == RBB_BLK_PUT)
        {
            block->status = RBB_BLK_GET;
            goto __exit;
        }
    }
    /* not found */
    block = NULL;

__exit:

    hw_interrupt_enable(level);

    return block;
}

/**
 * return the block size
 *
 * @param block the block
 *
 * @return block size
 */
size_t rbb_blk_size(rbb_blk_t block)
{
    TSC_ASSERT(block);

    return block->size;
}

/**
 * return the block buffer
 *
 * @param block the block
 *
 * @return block buffer
 */
uint8_t *rbb_blk_buf(rbb_blk_t block)
{
    TSC_ASSERT(block);

    return block->buf;
}

/**
 * free the block
 *
 * @param rbb ring block buffer object
 * @param block the block
 */
void rbb_blk_free(rbb_t rbb, rbb_blk_t block)
{
    long level;

    TSC_ASSERT(rbb);
    TSC_ASSERT(block);
    TSC_ASSERT(block->status != RBB_BLK_UNUSED);

    level = hw_interrupt_disable();
    /* remove it on rbb block list */
    list_remove(rbb, &block->list);
    block->status = RBB_BLK_UNUSED;
    tsc_slist_insert(&rbb->free_list, &block->list);
    hw_interrupt_enable(level);
}

/**
 * get a continuous block to queue by given size
 *
 *          tail                         head
 * +------------------+---------------+--------+----------+--------+
 * |      block3      |  empty1       | block1 |  block2  |fragment|
 * +------------------+------------------------+----------+--------+
 *                                    |<-- return_size -->|    |
 *                                    |<--- queue_data_len --->|
 *
 *         tail                          head
 * +------------------+---------------+--------+----------+--------+
 * |      block3      |  empty1       | block1 |  block2  |fragment|
 * +------------------+------------------------+----------+--------+
 *                                    |<-- return_size -->|              out of len(b1+b2+b3)    |
 *                                    |<-------------------- queue_data_len -------------------->|
 *
 * @param rbb ring block buffer object
 * @param queue_data_len The max queue data size, and the return size must less then it.
 * @param queue continuous block queue
 *
 * @return the block queue data total size
 */
size_t rbb_blk_queue_get(rbb_t rbb, size_t queue_data_len, rbb_blk_queue_t blk_queue)
{
    long level;
    size_t data_total_size = 0;
    tsc_slist_t *node, *tmp = NULL;
    rbb_blk_t last_block = NULL, block;

    TSC_ASSERT(rbb);
    TSC_ASSERT(blk_queue);

    if (tsc_slist_isempty(&rbb->blk_list))
        return 0;

    level = hw_interrupt_disable();

    node = tsc_slist_first(&rbb->blk_list);
    if (node != NULL)
    {
        tmp = tsc_slist_next(node);
    }
    for (; node; node = tmp, tmp = tsc_slist_next(node))
    {
        if (!last_block)
        {
            last_block = tsc_slist_entry(node, struct rbb_blk, list);
            if (last_block->status == RBB_BLK_PUT)
            {
                /* save the first put status block to queue */
                blk_queue->blocks = last_block;
                blk_queue->blk_num = 0;
            }
            else
            {
                /* the first block must be put status */
                last_block = NULL;
                continue;
            }
        }
        else
        {
            block = tsc_slist_entry(node, struct rbb_blk, list);
            /*
             * these following conditions will break the loop:
             * 1. the current block is not put status
             * 2. the last block and current block is not continuous
             * 3. the data_total_size will out of range
             */
            if (block->status != RBB_BLK_PUT ||
                last_block->buf > block->buf ||
                data_total_size + block->size > queue_data_len)
            {
                break;
            }
            /* backup last block */
            last_block = block;
        }
        /* remove current block */
        data_total_size += last_block->size;
        last_block->status = RBB_BLK_GET;
        blk_queue->blk_num++;
    }

    hw_interrupt_enable(level);

    return data_total_size;
}

/**
 * get all block length on block queue
 *
 * @param blk_queue the block queue
 *
 * @return total length
 */
size_t rbb_blk_queue_len(rbb_blk_queue_t blk_queue)
{
    size_t i = 0, data_total_size = 0;
    rbb_blk_t blk;

    TSC_ASSERT(blk_queue);

    for (blk = blk_queue->blocks; i < blk_queue->blk_num; i++)
    {
        data_total_size += blk->size;
        blk = tsc_slist_entry(blk->list.next, struct rbb_blk, list);
    }
    return data_total_size;
}

/**
 * return the block queue buffer
 *
 * @param blk_queue the block queue
 *
 * @return block queue buffer
 */
uint8_t *rbb_blk_queue_buf(rbb_blk_queue_t blk_queue)
{
    TSC_ASSERT(blk_queue);

    return blk_queue->blocks[0].buf;
}

/**
 * free the block queue
 *
 * @param rbb ring block buffer object
 * @param blk_queue the block queue
 */
void rbb_blk_queue_free(rbb_t rbb, rbb_blk_queue_t blk_queue)
{
    size_t i = 0;
    rbb_blk_t blk, next_blk;

    TSC_ASSERT(rbb);
    TSC_ASSERT(blk_queue);

    for (blk = blk_queue->blocks; i < blk_queue->blk_num; i++)
    {
        next_blk = tsc_slist_entry(blk->list.next, struct rbb_blk, list);
        rbb_blk_free(rbb, blk);
        blk = next_blk;
    }
}

/**
 * The put status and buffer continuous blocks can be make a block queue.
 * This function will return the length which from next can be make block queue.
 *
 * @param rbb ring block buffer object
 *
 * @return the next can be make block queue's length
 */
size_t rbb_next_blk_queue_len(rbb_t rbb)
{
    long level;
    size_t data_len = 0;
    tsc_slist_t *node;
    rbb_blk_t last_block = NULL, block;

    TSC_ASSERT(rbb);

    if (tsc_slist_isempty(&rbb->blk_list))
        return 0;

    level = hw_interrupt_disable();

    for (node = tsc_slist_first(&rbb->blk_list); node; node = tsc_slist_next(node))
    {
        if (!last_block)
        {
            last_block = tsc_slist_entry(node, struct rbb_blk, list);
            if (last_block->status != RBB_BLK_PUT)
            {
                /* the first block must be put status */
                last_block = NULL;
                continue;
            }
        }
        else
        {
            block = tsc_slist_entry(node, struct rbb_blk, list);
            /*
             * these following conditions will break the loop:
             * 1. the current block is not put status
             * 2. the last block and current block is not continuous
             */
            if (block->status != RBB_BLK_PUT || last_block->buf > block->buf)
            {
                break;
            }
            /* backup last block */
            last_block = block;
        }
        data_len += last_block->size;
    }

    hw_interrupt_enable(level);

    return data_len;
}

/**
 * get the ring block buffer object buffer size
 *
 * @param rbb ring block buffer object
 *
 * @return buffer size
 */
size_t rbb_get_buf_size(rbb_t rbb)
{
    TSC_ASSERT(rbb);

    return rbb->buf_size;
}


#endif /* CSP_ENABLE_LOG */
