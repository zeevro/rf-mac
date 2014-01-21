#ifndef QUEUE_H
#define	QUEUE_H

typedef struct {
    uint8_t size;
    uint8_t head;
    uint16_t * array;
} queue_t;

void queue_init(queue_t * q, uint16_t * array, uint8_t size);

void queue_push(queue_t * q, uint16_t value);

#endif	/* QUEUE_H */