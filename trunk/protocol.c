#include "my_lib.h"

route_t routes[0x100];
uint8_t message_counters[0x100];

uint16_t fqid(message_t * m) {
    return (m->id << 8) | m->u_src;
}

void init_route_table() {
    int i;
    for (i = 0; i < 0x100; i++)
    {
        routes[i].router = 0;
        routes[i].distance = 0xFF;
    }
}

void rx_handler(message_t * message, uint8_t length) {
    route_t * route;
    message_t forward_message;
    uint8_t u_src_id;

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

    if (message->id <= message_counters[u_src_id]) return;

    message_counters[u_src_id] = message->id;

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