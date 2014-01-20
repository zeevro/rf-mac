#include "my_lib.h"

void queue_init(queue_t * q, uint16_t * array, uint8_t size) {
    q->array = array;
    q->head = 0;
    q->size = size;
}

void queue_push(queue_t * q, uint16_t value) {
    q->array[q->head] = value;
    q->head = (q->head + 1) % q->size;
}