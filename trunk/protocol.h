#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#define NUM_OLD_MSGS 32

#define MESSAGE_SIGNATURE 0x90

#define HEADER_SIZE (sizeof(UINT8) * 3 + sizeof(node_address_t) * 4)

#define MAX_PAYLOAD_SIZE (32 - HEADER_SIZE)

#define NULL_ROUTE ((route_t){0, 0xFF})

typedef UINT8 node_address_t;

typedef struct {
    node_address_t router;
    UINT8 distance;
} route_t;

typedef struct {
    UINT8 signature;
    UINT8 new_route:1;
    UINT8 id:7;
    node_address_t u_src;
    node_address_t u_dst;
    node_address_t src;
    node_address_t dst;
    UINT8 hops;
    char payload[MAX_PAYLOAD_SIZE];
} message_t;

UINT16 fqid(message_t * m);
void init_protocol(node_address_t address);
void rx_handler(message_t * message, UINT8 length);
void tx_message(node_address_t dst, UINT8 * payload, UINT8 payload_length, BOOL new_route);

#endif /* _PROTOCOL_H */