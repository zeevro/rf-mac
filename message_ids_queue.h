#ifndef _MIDS_QUEUE_H
#define _MIDS_QUEUE_H

typedef struct {
    UINT8 size;
    UINT8 head;
    UINT16 * array;
} mids_queue_t;

void mids_queue_init(mids_queue_t * q, UINT16 * array, UINT8 size);
void mids_queue_push(mids_queue_t * q, UINT16 value);

#endif /* _MIDS_QUEUE_H */