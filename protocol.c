#include "main.h"

route_t routes[0xFF];

UINT16 old_msgs_fqids[NUM_OLD_MSGS];
mids_queue_t old_msgs;

message_t tx_msgs[TX_QUEUE_SIZE];
tx_queue_t tx_queue;

message_t temp_tx_msg;

node_address_t node_address;

UINT16 fqid(message_t * m) {
    return (m->id << 8) | m->u_src;
}

void set_null_route(route_t * route) {
    route->router = 0;
    route->distance = 0xFF;
}

void init_protocol(node_address_t address) {
    UINT8 i = 0;

    node_address = address;

    do
    {
        set_null_route(&(routes[i++]));
    }
    while (i != 0);

    memset(old_msgs_fqids, 0, sizeof (old_msgs_fqids));

    mids_queue_init(&old_msgs, old_msgs_fqids, NUM_OLD_MSGS);

    temp_tx_msg.signature = MESSAGE_SIGNATURE;
    temp_tx_msg.id = 0;
    temp_tx_msg.u_src = node_address;
    temp_tx_msg.src = node_address;
    temp_tx_msg.hops = 0;
}

void rx_handler(message_t * message, UINT8 length) {
    route_t * src_route;
    route_t * dst_route;
    message_t forward_message;
    UINT8 u_src_id;
    UINT16 m_fqid;
    UINT8 i;

    if (message->signature != MESSAGE_SIGNATURE) return;

    if (message->u_src == node_address) return;

    u_src_id = message->u_src - 1;

    src_route = &(routes[u_src_id]);

    if (src_route->distance > message->hops)
    {
        src_route->router = message->src;
        src_route->distance = message->hops;
    }

    if (message->dst != 0 && message->dst != node_address) return;

    m_fqid = fqid(message);
    for (i = 0; i < NUM_OLD_MSGS; i++)
    {
        if (m_fqid == old_msgs_fqids[i]) return;
    }

    mids_queue_push(&old_msgs, m_fqid);

    if (message->u_dst == node_address)
    {
        if (message->u_src == 1)
        {
            delay_ms(50);
            tx_message(message->u_src, (UINT8 *)"Hi", 2, FALSE);
        }

        //*
        // TODO: Handle payload
        // payload_length = length - HEADER_SIZE;
        LED = 1;
        delay_ms(800);
        LED = 0;
        // */

        return;
    }

    memcpy((void *)(&forward_message), (void *)message, length);

    dst_route = &(routes[message->u_dst - 1]);

    if (message->new_route)
    {
        set_null_route(dst_route);
    }

    forward_message.src = node_address;
    forward_message.dst = dst_route->router;
    forward_message.hops++;

    LED = 1;
    delay_ms(200);
    LED = 0;

    radio_tx((UINT8 *)(&forward_message), length);
}

void tx_message(node_address_t dst, UINT8 * payload, UINT8 payload_length, BOOL new_route) {
    temp_tx_msg.new_route = new_route;
    temp_tx_msg.id++;
    temp_tx_msg.u_dst = dst;
    temp_tx_msg.dst = routes[dst - 1].router;
    memcpy(&(temp_tx_msg.payload), payload, payload_length);

    radio_tx((UINT8 *)(&temp_tx_msg), HEADER_SIZE + payload_length);

    // TODD: Init timer for retransmit
}