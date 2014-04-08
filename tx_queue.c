#include "main.h"

void tx_queue_init(tx_queue_t * q, tx_message_t * array, UINT8 size) {
    q->array = array;
    q->head = 0;
    q->tail = size - 1;
    q->size = size;
    q->count = 0;
}

void tx_queue_push(tx_queue_t * q, tx_message_t * msg) {
    if (q->count >= q->size - 1)
    {
        return;
    }
    memcpy(msg, &(q->array[q->head]), sizeof(tx_message_t));
    q->head = (q->head + 1) % q->size;
    q->count ++;
}

tx_message_t * tx_queue_pop(tx_queue_t * q) {
    if (q->count <= 0)
    {
        return NULL;
    }
    q->count --;
    q->tail = (q->tail + 1) % q->size;
    return &(q->array[q->tail]);
}