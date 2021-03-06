#ifndef _TX_QUEUE_H
#define _TX_QUEUE_H

typedef struct {
    BOOL needs_ack;
    node_address_t dst;
    UINT8 size;
    UINT8 payload[MAX_PAYLOAD_SIZE];
} tx_message_t;

typedef struct {
    UINT8 size;
    UINT8 count;
    UINT8 head;
    UINT8 tail;
    tx_message_t * array;
} tx_queue_t;

void tx_queue_init(tx_queue_t * q, tx_message_t * array, UINT8 size);
void tx_queue_push(tx_queue_t * q, tx_message_t * msg);
tx_message_t * tx_queue_pop(tx_queue_t * q);

#endif /* _TX_QUEUE_H */