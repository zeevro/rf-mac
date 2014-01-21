#ifndef PROTOCOL_H
#define	PROTOCOL_H

#define NODE_ADDRESS 2

#define NUM_OLD_MSGS 16

#define MESSAGE_SIGNATURE 0x90

#define HEADER_SIZE (sizeof(uint8_t) * 3 + sizeof(node_address_t) * 4)

#define MAX_PAYLOAD_SIZE (32 - HEADER_SIZE)

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
    char payload[MAX_PAYLOAD_SIZE];
} message_t;

uint16_t fqid(const message_t * m);

void init_protocol();

void rx_handler(message_t * message, uint8_t length);

void tx_message(node_address_t dst, char * payload, uint8_t payload_length);

#endif	/* PROTOCOL_H */

