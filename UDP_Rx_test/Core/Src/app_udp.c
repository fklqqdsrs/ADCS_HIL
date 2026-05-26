/*
 * app_udp.c
 *
 *  Created on: Jan 9, 2026
 *      Author: Chinnapop
 */


#include "app_udp.h"

#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include "lwip/ip_addr.h"

#include <string.h>

#define UDP_MAX_PKT  256U   // ของคุณ 120/124 bytes สบายมาก

static struct udp_pcb *g_pcb = NULL;

static uint8_t  g_buf[UDP_MAX_PKT];
static uint16_t g_len = 0;
static volatile bool g_has = false;

static void udp_rx_cb(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                      const ip_addr_t *addr, u16_t port)
{
    (void)arg; (void)pcb; (void)addr; (void)port;

    if (p == NULL) return;

    uint16_t copy_len = (p->tot_len > UDP_MAX_PKT) ? UDP_MAX_PKT : (uint16_t)p->tot_len;
    (void)pbuf_copy_partial(p, g_buf, copy_len, 0);

    g_len = copy_len;
    g_has = true;

    pbuf_free(p);
}

void UDP_Server_Init(uint16_t listen_port)
{
    if (g_pcb != NULL) {
        udp_remove(g_pcb);
        g_pcb = NULL;
    }

    g_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
    if (g_pcb == NULL) return;

    if (udp_bind(g_pcb, IP_ANY_TYPE, listen_port) != ERR_OK) {
        udp_remove(g_pcb);
        g_pcb = NULL;
        return;
    }

    udp_recv(g_pcb, udp_rx_cb, NULL);
}

bool UDP_Server_Pop(uint8_t *dst, uint16_t dst_max, uint16_t *out_len)
{
    if (!g_has) return false;

    uint16_t n = (g_len > dst_max) ? dst_max : g_len;
    memcpy(dst, g_buf, n);

    g_has = false;
    g_len = 0;

    if (out_len) *out_len = n;
    return true;
}
