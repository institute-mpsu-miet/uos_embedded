#ifndef _RING_H_
#define _RING_H_

typedef struct _ring_t
{
    unsigned char   *data;
    unsigned        size;
    unsigned        wr;
    unsigned        rd;
} ring_t;

void ring_init(ring_t *r, void *buffer, unsigned size);
unsigned ring_avail_write(ring_t *r);
unsigned ring_avail_read(ring_t *r);
void *ring_write_ptr(ring_t *r);
void *ring_read_ptr(ring_t *r);
unsigned ring_advance_write_ptr(ring_t *r, unsigned len);
unsigned ring_advance_read_ptr(ring_t *r, unsigned len);
unsigned ring_write(ring_t *r, void *data, unsigned len);
unsigned ring_read(ring_t *r, void *data, unsigned len);

#endif
