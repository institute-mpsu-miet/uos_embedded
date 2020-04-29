#ifndef _TIMER_H_
#define _TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>

/**\~english
 * Number of milliseconds per day.
 *
 * \~russian
 * Количество миллисекунд в дне.
 */
#define TIMER_MSEC_PER_DAY  (24UL*60*60*1000)
#define TIMER_USEC_PER_MSEC (1000UL)

struct _timer_t {
    mutex_t lock;
    mutex_t decisec;        /* every 0.1 second is signalled here */
    unsigned long khz;      /* reference clock */
#ifdef DMA_TIMER
    unsigned long usec_per_tick;
    unsigned long usec_in_msec;
    unsigned long usec_signal;
    unsigned long msec_signal;
    unsigned int flag;
    unsigned int unset;
#endif    
#ifdef USEC_TIMER
    unsigned long usec_per_tick;
    unsigned long usec_in_msec;
#else
    small_uint_t msec_per_tick;
#endif

#if PIC32MX
    unsigned compare_step;
#endif
    unsigned long milliseconds; /* real time counter */
    unsigned long next_decisec; /* when next decisecond must be signalled */
    unsigned int days;      /* days counter */

#ifdef TIMER_TIMEOUTS
    list_t timeouts;
#endif
};

/**\~english
 * Data structure of a timer driver.
 *
 * \~russian
 * Структура данных для драйвера таймера.
 */
typedef struct _timer_t timer_t;

void timers_deinit(void);
#if( defined USEC_TIMER || defined DMA_TIMER)
// Микросекудный таймер. Поддерживается пока только для миландровских контроллеров
void timer_init_us (timer_t *t, unsigned long khz, unsigned long usec_per_tick);
#else
void timer_init (timer_t *t, unsigned long khz, small_uint_t msec_per_tick);
#endif

#ifdef SW_TIMER
void timer_update (timer_t *t);
#endif

/* Delay current task. */
void timer_delay (timer_t *t, unsigned long msec);

/* Query real time. */
unsigned long timer_milliseconds (timer_t *t);
uint64_t timer_microseconds (timer_t *t);
unsigned int timer_days (timer_t *t, unsigned long *milliseconds);
bool_t timer_passed (timer_t *t, unsigned long t0, unsigned int msec);
bool_t interval_greater_or_equal (long interval, long msec);

#ifdef __cplusplus
}
#endif

#endif /* _TIMER_H_ */
