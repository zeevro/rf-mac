#ifndef QUEUE_H
#define	QUEUE_H

typedef struct {
    uint8_t size;
    void * ptr;
    void * array;
} queue_t;

void queue_init(queue_t * q, void * array) {
    q->array = array;
    q->ptr = array;
}

void queue_push(queue_t * q, void * value) {
    *(q->ptr) = value;
    (q->ptr)++;
    if (q->ptr > q->array + q->size)
    {
        q->ptr = q->array;
    }
}

#endif	/* QUEUE_H */

