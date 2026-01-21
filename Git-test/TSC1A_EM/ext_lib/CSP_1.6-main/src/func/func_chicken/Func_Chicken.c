#include "Func_Chicken.h"

int send_chicken(uint8_t node, uint8_t prio, uint16_t dport, uint32_t timeout, uint8_t opts, uint8_t *data, uint16_t data_length)
{
    csp_conn_t *conn = csp_connect(prio, node, dport, timeout, opts);
    if (conn == NULL)
    {
        printf("Failed to connect: conn == NULL\n");
        return -1;
    }

    csp_packet_t *packet = csp_buffer_get(data_length + 1);
    if (packet == NULL)
    {
        printf("Failed to create packet: packet == NULL\n");
        csp_close(conn);
        return -2;
    }

    memcpy(&packet->data[0], data, data_length + 1);

    packet->length = data_length + 1;

    if (!csp_send(conn, packet, timeout))
    {
        csp_buffer_free(packet);
        printf("Failed to send data\n");
        csp_close(conn);
        return -3;
    }

    csp_close(conn);
    return 0;
}

int request_chicken(uint8_t node, uint8_t prio, uint16_t dport, uint32_t timeout, uint8_t opts,
                    uint8_t *send_data, uint16_t send_data_length,
                    uint8_t *recv_data, uint16_t *recv_data_length)
{

    csp_conn_t *conn = csp_connect(prio, node, dport, timeout, opts);
    if (conn == NULL)
    {
        printf("Failed to connect: conn == NULL\n");
        return -1;
    }

    csp_packet_t *packet = csp_buffer_get(send_data_length + 1);
    if (packet == NULL)
    {
        printf("Failed to create packet: packet == NULL\n");
        csp_close(conn);
        return -2;
    }

    memcpy(&packet->data[0], send_data, send_data_length + 1);
    packet->length = send_data_length + 1;

    if (!csp_send(conn, packet, timeout))
    {
        csp_buffer_free(packet);
        printf("Failed to send data\n");
        csp_close(conn);
        return -3;
    }

    if (recv_data != NULL && recv_data_length != NULL)
    {
        while ((packet = csp_read(conn, timeout)) != NULL)
        {
            uint16_t received_data_length = packet->length;

            if (*recv_data_length < received_data_length)
            {
                printf("Error: Provided buffer is too small for received data\n");
                csp_buffer_free(packet);
                csp_close(conn);
                return -4;
            }

            memcpy(recv_data, &packet->data[0], received_data_length);
            *recv_data_length = received_data_length;

            csp_buffer_free(packet);
            csp_close(conn);
            return 0;
        }
    }

    csp_close(conn);
    return -5;
}
