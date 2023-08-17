#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

typedef struct
{
	uint8_t *_data;
	volatile uint8_t _readCount;
	volatile uint8_t _writeCount;
	uint8_t _mask;
} RingBuffer;

uint8_t RingBuffer_isFull(RingBuffer *rb)
{
	return ((rb->_writeCount - rb->_readCount) & ~(rb->_mask)) != 0;
}

uint8_t RingBuffer_isEmpty(RingBuffer *rb)
{
	return rb->_writeCount == rb->_readCount;
}

uint8_t RingBuffer_write(RingBuffer *rb, uint8_t value)
{
	if (RingBuffer_isFull(rb))
		return 0;
	rb->_data[rb->_writeCount++ & rb->_mask] = value;
	return 1;
}

uint8_t RingBuffer_forceWrite(RingBuffer *rb, uint8_t value)
{
	uint8_t isEmpty = !RingBuffer_isFull(rb);
	if (!isEmpty)
		rb->_readCount++;
	rb->_data[rb->_writeCount++ & rb->_mask] = value;
	return isEmpty;
}

uint8_t RingBuffer_read(RingBuffer *rb, uint8_t *value)
{
	if (RingBuffer_isEmpty(rb))
		return 0;
	*value = rb->_data[rb->_readCount++ & rb->_mask];
	return 1;
}

void RingBuffer_init(RingBuffer *rb, uint8_t *data, uint8_t size)
{
	rb->_data = data;
	rb->_readCount = 0;
	rb->_writeCount = 0;
	rb->_mask = size - 1;
	while (size & rb->_mask)
		;
}

#endif /* RING_BUFFER_H */
