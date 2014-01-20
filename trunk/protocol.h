#ifndef PROTOCOL_H
#define	PROTOCOL_H

#define ROUTE_TABLE_SIZE 32

#define NODE_ADDRESS 2

typedef uint8_t node_address_t;

typedef struct {
    node_address_t router;
    uint8_t distance;
} route_t;

typedef struct {
    uint8_t signature;
    uint8_t id;
    node_address_t u_src;
    node_address_t u_dst;
    node_address_t src;
    node_address_t dst;
    uint8_t hops;
    char payload[];
} message_t;

#define MESSAGE_SIGNATURE 0x90

#define HEADER_SIZE (sizeof(uint8_t) * 3 + sizeof(node_address_t) * 4)

uint16_t fqid(message_t * m);

void init_route_table();

void rx_handler(message_t * message, uint8_t length);

#endif	/* PROTOCOL_H */

