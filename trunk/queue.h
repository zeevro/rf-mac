#ifndef _QUEUE_H
#define _QUEUE_H

typedef struct {
    UINT8 size;
    UINT8 head;
    UINT16 * array;
} queue_t;

void queue_init(queue_t * q, UINT16 * array, UINT8 size);
void queue_push(queue_t * q, UINT16 value);

#endif /* _QUEUE_H */