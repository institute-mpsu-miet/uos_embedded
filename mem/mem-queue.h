#ifndef __MEM_QUEUE_H__
#define __MEM_QUEUE_H__

#include <mem/mem.h>

typedef struct _mem_queue_t {
	int msg_num;
	int count;	
	void **tail;
	void **queue;
} mem_queue_t;



void mem_queue_init (mem_queue_t *q, mem_pool_t *pool, int msg_num);


/*
 * Clear queue.
 * The queue is usable after this operation.
 */
void mem_queue_clear (mem_queue_t *q);


/*
 * Free (dispose) queue.
 * The function clears all the memory that is taken for the queue.
 * The queue is unusable after this operation.
 */
void mem_queue_free (mem_queue_t *q);


/*
 * Add a packet to queue.
 * Before call, a user should check that the queue is not full.
 */
void mem_queue_put (mem_queue_t *q, void *block);


/*
 * Get a packet from queue.
 * It must be checked that the queue is not empty before calling this.
 */

void mem_queue_get (mem_queue_t *q, void **block);

/*
 * Returns the first element from queue
 * It must be checked that the queue is not empty before calling this.
 */
void *mem_queue_current (mem_queue_t *q);



/*
 * Check that queue is full.
 */
bool_t mem_queue_is_full (mem_queue_t *q);


/*
 * Check that queue is empty.
 */
bool_t mem_queue_is_empty (mem_queue_t *q);

#endif