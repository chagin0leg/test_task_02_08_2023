#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

typedef struct RING_BUFFER
{
	uint8_t *_data;
	volatile uint8_t _readCount;
	volatile uint8_t _writeCount;
	uint8_t _mask;
} RING_BUFFER;

uint8_t RingBuffer_isFull(RING_BUFFER *rb);

uint8_t RingBuffer_isEmpty(RING_BUFFER *rb);

uint8_t RingBuffer_write(RING_BUFFER *rb, uint8_t value);

uint8_t RingBuffer_forceWrite(RING_BUFFER *rb, uint8_t value);

uint8_t RingBuffer_read(RING_BUFFER *rb, uint8_t *value);

void RingBuffer_init(RING_BUFFER *rb, uint8_t *data, uint8_t size);

#endif /* RING_BUFFER_H */
