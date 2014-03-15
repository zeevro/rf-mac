#include "main.h"

void mids_queue_init(mids_queue_t * q, UINT16 * array, UINT8 size) {
    q->array = array;
    q->head = 0;
    q->size = size;
}

void mids_queue_push(mids_queue_t * q, UINT16 value) {
    q->array[q->head] = value;
    q->head = (q->head + 1) % q->size;
}