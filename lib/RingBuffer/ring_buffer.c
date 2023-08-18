#include "ring_buffer.h"

uint8_t RingBuffer_isFull(RING_BUFFER *rb)
{
    return ((rb->_writeCount - rb->_readCount) & ~(rb->_mask)) != 0;
}

uint8_t RingBuffer_isEmpty(RING_BUFFER *rb)
{
    return rb->_writeCount == rb->_readCount;
}

uint8_t RingBuffer_write(RING_BUFFER *rb, uint8_t value)
{
    if (RingBuffer_isFull(rb))
        return 0;
    rb->_data[rb->_writeCount++ & rb->_mask] = value;
    return 1;
}

uint8_t RingBuffer_forceWrite(RING_BUFFER *rb, uint8_t value)
{
    uint8_t isEmpty = !RingBuffer_isFull(rb);
    if (!isEmpty)
        rb->_readCount++;
    rb->_data[rb->_writeCount++ & rb->_mask] = value;
    return isEmpty;
}

uint8_t RingBuffer_read(RING_BUFFER *rb, uint8_t *value)
{
    if (RingBuffer_isEmpty(rb))
        return 0;
    *value = rb->_data[rb->_readCount++ & rb->_mask];
    return 1;
}

void RingBuffer_init(RING_BUFFER *rb, uint8_t *data, uint8_t size)
{
    rb->_data = data;
    rb->_readCount = 0;
    rb->_writeCount = 0;
    rb->_mask = size - 1;
    while (size & rb->_mask)
        ;
}
