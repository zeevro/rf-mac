#include "main.h"

void queue_init(queue_t * q, UINT16 * array, UINT8 size) {
    q->array = array;
    q->head = 0;
    q->size = size;
}

void queue_push(queue_t * q, UINT16 value) {
    q->array[q->head] = value;
    q->head = (q->head + 1) % q->size;
}