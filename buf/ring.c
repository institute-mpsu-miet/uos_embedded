#include <runtime/lib.h>
#include "ring.h"

void ring_init(ring_t *r, void *buffer, unsigned size)
{
    assert(size == 0x00000002 || size == 0x00000004 || size == 0x00000008 ||
        size == 0x00000010 || size == 0x00000020 || size == 0x00000040 ||
        size == 0x00000080 || size == 0x00000100 || size == 0x00000200 ||
        size == 0x00000400 || size == 0x00000800 || size == 0x00001000 ||
        size == 0x00002000 || size == 0x00004000 || size == 0x00008000 ||
        size == 0x00010000 || size == 0x00020000 || size == 0x00040000 ||
        size == 0x00080000 || size == 0x00100000 || size == 0x00200000 ||
        size == 0x00400000 || size == 0x00800000 || size == 0x01000000 ||
        size == 0x02000000 || size == 0x04000000 || size == 0x08000000 ||
        size == 0x10000000 || size == 0x20000000 || size == 0x40000000 ||
        size == 0x80000000);
    r->data = buffer;
    r->size = size;
    r->rd = 0;
    r->wr = 0;
}

unsigned ring_avail_write(ring_t *r)
{
    return (r->size - (r->wr - r->rd) - 1) & (r->size - 1);
}

unsigned ring_avail_read(ring_t *r)
{
    return (r->wr - r->rd + r->size) & (r->size - 1);
}

void *ring_write_ptr(ring_t *r)
{
    return r->data + r->wr;
}

void *ring_read_ptr(ring_t *r)
{
    return r->data + r->rd;
}

unsigned ring_advance_write_ptr(ring_t *r, unsigned len)
{
    assert (len <= r->size);
    if (ring_avail_write(r) < len)
        return 0;
    r->wr = (r->wr + len) & (r->size - 1);
    return len;
}

unsigned ring_advance_read_ptr(ring_t *r, unsigned len)
{
    assert (len <= r->size);
    if (ring_avail_read(r) < len)
        return 0;
    r->rd = (r->rd + len) & (r->size - 1);
    return len;
}

unsigned ring_write(ring_t *r, void *data, unsigned len)
{
    assert (len <= r->size);
    if (ring_avail_write(r) < len)
        return 0;

    unsigned avail_write_to_end_of_buffer = r->size - r->wr;
    if (avail_write_to_end_of_buffer >= len) {
        memcpy(r->data + r->wr, data, len);
        r->wr = (r->wr + len) & (r->size - 1);
    } else {
        memcpy(r->data + r->wr, data, avail_write_to_end_of_buffer);
        memcpy(r->data, data + avail_write_to_end_of_buffer, len - avail_write_to_end_of_buffer);
        r->wr = len - avail_write_to_end_of_buffer;
    }
    return len;
}

unsigned ring_read(ring_t *r, void *data, unsigned len)
{
    assert (len <= r->size);
    if (ring_avail_read(r) < len)
        return 0;

    unsigned avail_read_to_end_of_buffer = r->size - r->rd;
    if (avail_read_to_end_of_buffer >= len) {
        memcpy(data, r->data + r->rd, len);
        r->rd = (r->rd + len) & (r->size - 1);
    } else {
        memcpy(data, r->data + r->rd, avail_read_to_end_of_buffer);
        memcpy(data + avail_read_to_end_of_buffer, r->data, len - avail_read_to_end_of_buffer);
        r->rd = len - avail_read_to_end_of_buffer;
    }
    return len;
}



