#include "my_lib.h"

route_t routes[0x100];
uint16_t old_msgs_fqids[NUM_OLD_MSGS];
queue_t old_msgs;
message_t tx_msg;

uint16_t fqid(const message_t * m) {
    return (m->id << 8) | m->u_src;
}

void init_protocol() {
    uint16_t i;
    for (i = 0; i < 0x100; i++)
    {
        routes[i].router = 0;
        routes[i].distance = 0xFF;
    }

    queue_init(&old_msgs, old_msgs_fqids, NUM_OLD_MSGS);

    tx_msg.signature = MESSAGE_SIGNATURE;
    tx_msg.id = 0;
    tx_msg.u_src = NODE_ADDRESS;
    tx_msg.src = NODE_ADDRESS;
    tx_msg.hops = 0;
}

void rx_handler(const message_t * message, const uint8_t length) {
    route_t * route;
    message_t forward_message;
    uint8_t u_src_id;
    uint16_t m_fqid;
    uint8_t i;

    if (message->signature != MESSAGE_SIGNATURE) return;

    if (message->u_src == NODE_ADDRESS) return;

    u_src_id = message->u_src - 1;

    route = &(routes[u_src_id]);

    if (route->distance > message->hops)
    {
        route->router = message->src;
        route->distance = message->hops;
    }

    if (message->dst != 0 && message->dst != NODE_ADDRESS) return;

    m_fqid = fqid(message);
    for (i = 0; i < NUM_OLD_MSGS; i++)
    {
        if (m_fqid == old_msgs_fqids[i]) return;
    }

    queue_push(&old_msgs, m_fqid);

    if (message->u_dst == NODE_ADDRESS)
    {
        // TODO: Handle payload
        // payload_length = length - HEADER_SIZE;
        LED = LED_ON;
        delay_ms(800);
        LED = LED_OFF;

        return;
    }

    memcpy((void *)(&forward_message), (void *)message, length);

    forward_message.src = NODE_ADDRESS;
    forward_message.dst = routes[message->u_dst - 1].router;
    forward_message.hops ++;

    LED = LED_ON;
    delay_ms(200);
    LED = LED_OFF;

    radio_tx(&forward_message, length);
}

void tx_message(const node_address_t dst, const char * payload, const uint8_t payload_length) {
    tx_msg.id ++;
    tx_msg.u_dst = dst;
    tx_msg.dst = routes[dst - 1].router;
    *(tx_msg.payload) = payload;

    radio_tx(&tx_msg, HEADER_SIZE + payload_length);
}