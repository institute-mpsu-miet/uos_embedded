#ifndef __MEM_QUEUE_C__
#define __MEM_QUEUE_C__

#include <runtime/lib.h>
#include <kernel/uos.h>
#include <mem/mem-queue.h>

inline __attribute__((always_inline))
void mem_queue_init (mem_queue_t *q, mem_pool_t *pool, int msg_num)
{
	q->queue = (void **) mem_alloc (pool, msg_num * 4);
	assert (q->queue);
	q->tail = q->queue;
	q->msg_num = msg_num;
	q->count = 0;
}

/*
 * Clear queue.
 * The queue is usable after this operation.
 */
inline __attribute__((always_inline))
void mem_queue_clear (mem_queue_t *q)
{
	while (q->count) {
		mem_free (*q->tail);
		if (--q->tail < q->queue)
			q->tail += q->msg_num;
		--q->count;
	}
}

/*
 * Free (dispose) queue.
 * The function clears all the memory that is taken for the queue.
 * The queue is unusable after this operation.
 */

inline __attribute__((always_inline))
void mem_queue_free (mem_queue_t *q)
{
    mem_queue_clear (q);
	if (q->queue) {
		mem_free (*q->queue);
		q->queue = 0;
	}
}

/*
 * Add a packet to queue.
 * Before call, a user should check that the queue is not full.
 */
inline __attribute__((always_inline))
void mem_queue_put (mem_queue_t *q, void *block)
{
	void **head;

	/* Must be called ONLY when queue is not full. */
	assert (q->count < q->msg_num);

	/* Compute the last place in the queue. */
	head = q->tail - q->count;
	if (head < q->queue)
		head += q->msg_num;

	/* Put the packet in. */
	*head = block;
	++q->count;
}

/*
 * Get a packet from queue.
 * It must be checked that the queue is not empty before calling this.
 */
inline __attribute__((always_inline))
void mem_queue_get (mem_queue_t *q, void **block)
{
	assert (q->tail >= q->queue);
	assert (q->tail < q->queue + q->msg_num);
	if (q->count > 0) {
		/* Get the first packet from queue. */
		if (block != 0)
    		*block = *q->tail;

		/* Advance head pointer. */
		if (--q->tail < q->queue)
			q->tail += q->msg_num;
		--q->count;
	}
}

/*
 * Returns the first element from queue
 * It must be checked that the queue is not empty before calling this.
 */
inline __attribute__((always_inline))
void *mem_queue_current (mem_queue_t *q)
{
	assert (q->tail >= q->queue);
	assert (q->tail < q->queue + q->msg_num);
	return *q->tail;
}


/*
 * Check that queue is full.
 */
inline __attribute__((always_inline))
bool_t mem_queue_is_full (mem_queue_t *q)
{
	return (q->count == q->msg_num);
}

/*
 * Check that queue is empty.
 */
inline __attribute__((always_inline))
bool_t mem_queue_is_empty (mem_queue_t *q)
{
	return (q->count == 0);
}

#endif
