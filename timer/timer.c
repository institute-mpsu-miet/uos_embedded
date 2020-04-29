/**\namespace timer \brief \~english
 * Module "timer" - system time management.
 *
 * Full description of file timer.c.
 */

/**\namespace timer \brief \~russian
 * Модуль "timer" - управление системным временем.
 *
 * Полное описание файла timer.c.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>
#include <timer/timer.h>
#include <timer/timeout.h>

#if defined (ARM_CORTEX_M1) || defined (ARM_CORTEX_M3) || defined (ARM_CORTEX_M4)
extern volatile uint32_t __timer_ticks_uos;
#endif

#if I386
#   include <runtime/i386/i8253.h>
#   define TIMER_IRQ        0   /* IRQ0 */
#endif

#if __AVR__             /* Timer 1 compare A */
#   if defined(__AVR_ATmega2561__) || defined(__AVR_ATmega2560__)
#      define TIMER_IRQ     16
#   endif
#   if defined (__AVR_ATmega103__) || defined (__AVR_ATmega128__)
#      define TIMER_IRQ     11
#   endif
#   ifdef __AVR_ATmega161__
#      define TIMER_IRQ     6
#   endif
#   ifdef __AVR_ATmega168__
#      define TIMER_IRQ     10
#   endif
#   ifdef __AVR_AT90USB647__
#      define TIMER_IRQ     16
#   endif
#endif  /*__AVR__*/

#if ARM_S3C4530
#   define TIMER_IRQ        10  /* Timer 0 interrupt */
#endif

#if ARM_AT91SAM
#   define TIMER_IRQ    AT91C_ID_SYS
#endif

#if ARM_OMAP44XX
#   define TIMER_IRQ        29  /* Private Timer interrupt */
#endif

#if ELVEES_MC24
#   define TIMER_IRQ        29  /* Interval Timer interrupt */
#endif

#if ELVEES_MC0226
#   define TIMER_IRQ        29  /* Interval Timer interrupt */
#endif

#if ELVEES_MC24R2
#   define TIMER_IRQ        22  /* Interval Timer interrupt */
#endif

#if ELVEES_NVCOM01
#   define TIMER_IRQ        22  /* Interval Timer interrupt */
#endif

#if ELVEES_NVCOM02
#   define TIMER_IRQ        22  /* TODO: Interval Timer interrupt */
#endif

#if ELVEES_MCT02
#   define TIMER_IRQ        22  /* Interval Timer interrupt */
#endif

#if ELVEES_MCT03P
#   define TIMER_IRQ        22  /* Interval Timer interrupt */
#endif

#if ELVEES_MC0428
#   define TIMER_IRQ        22  /* Interval Timer interrupt */
#endif

#if ELVEES_MC30SF6
#   define TIMER_IRQ        22  /* Interval Timer interrupt */
#endif

#if PIC32MX
#   define TIMER_IRQ    PIC32_VECT_CT   /* Core Timer Interrupt */
#endif

#if ARM_STM32F4
#   define TIMER_IRQ        82  /* Systick */
#endif

#if ARM_STM32L151RC || ARM_STM32L152RC
#   if defined(RTC_TIMER)
#       define TIMER_IRQ        IRQ_RTC_WKUP    /* RTC */
#   else
#       define TIMER_IRQ        ARCH_TIMER_IRQ  /* Systick */
#   endif
#endif

#if ARM_1986BE9
#   define TIMER_IRQ        32  /* Systick */
#endif

#if ARM_1986BE1
#   if (ARM_SYS_TIMER==1)
#       define TIMER_IRQ    TIMER1_IRQn
#       define SYS_TIMER    ARM_TIMER1
#       define PER_CLOCK_EN ARM_PER_CLOCK_TIMER1
#       define TIM_CLK_EN   ARM_TIM1_CLOCK_EN
#   elif (ARM_SYS_TIMER==2)
#       define TIMER_IRQ    TIMER2_IRQn
#       define SYS_TIMER    ARM_TIMER2
#       define PER_CLOCK_EN ARM_PER_CLOCK_TIMER2
#       define TIM_CLK_EN   ARM_TIM2_CLOCK_EN
#   elif (ARM_SYS_TIMER==3)
#       define TIMER_IRQ    TIMER3_IRQn
#       define SYS_TIMER    ARM_TIMER3
#       define PER_CLOCK_EN ARM_PER_CLOCK_TIMER3
#       define TIM_CLK_EN   ARM_TIM3_CLOCK_EN
#   elif (ARM_SYS_TIMER==4)
#       define TIMER_IRQ    TIMER4_IRQn
#       define SYS_TIMER    ARM_TIMER4
#       define PER_CLOCK_EN ARM_PER_CLOCK_TIMER4
#       define TIM_CLK_EN   ARM_UART_CLOCK_TIM4_EN
#   else
#       warning "ARM_SYS_TIMER is not defined in CFLAGS (target.cfg). Using TIMER1 for system timer."
#       define TIMER_IRQ    TIMER1_IRQn
#       define SYS_TIMER    ARM_TIMER1
#       define PER_CLOCK_EN ARM_PER_CLOCK_TIMER1
#       define TIM_CLK_EN   ARM_TIM1_CLOCK_EN
#   endif
#endif

#if MSP430
#   ifdef TIMERA0_VECTOR
#      define TIMER_IRQ     (TIMERA0_VECTOR / 2)
#   else
#      define TIMER_IRQ     (TIMER0_A0_VECTOR / 2)
#      define TACTL         TA0CTL
#      define TACCR0        TA0CCR0
#      define TAEX0         TA0EX0
#   endif
#endif

#if LINUX386
#   include <sys/time.h>
#   include <signal.h>
#   define TIMER_IRQ        SIGALRM
#endif

/**\~english
 * Check that `msec' milliseconds have passed.
 * `Interval' is the time interval, probably rolled over the day.
 *
 * \~russian
 * Проверка, прошло ли указанное количество миллисекунд `msec'.
 * Параметр `interval' содержит интервал времени, возможно, переходящий границу суток.
 */
small_int_t
interval_greater_or_equal (long interval, long msec)
{
    if (interval < 0)
        interval += TIMER_MSEC_PER_DAY;
    else if (interval >= TIMER_MSEC_PER_DAY)
        interval -= TIMER_MSEC_PER_DAY;
    return (interval >= msec);
}

/*
 * Timer update function.
 */
#ifndef SW_TIMER
static inline
#endif
void timer_update (timer_t *t)
{
#ifdef SW_TIMER
    // mutex_activate (&t->lock,
        // (void*) (size_t) t->milliseconds);
#endif

#if defined (ARM_CORTEX_M1) || defined (ARM_CORTEX_M3) || defined (ARM_CORTEX_M4)
    __timer_ticks_uos++;

    if (__timer_ticks_uos==0)
        __timer_ticks_uos++;

#endif

    /* Increment current time. */
#if( defined USEC_TIMER || defined DMA_TIMER)
    t->usec_in_msec += t->usec_per_tick;
    while (t->usec_in_msec > TIMER_USEC_PER_MSEC) {
        t->milliseconds++;
        t->usec_in_msec -= TIMER_USEC_PER_MSEC;
    }
#else
    t->milliseconds += t->msec_per_tick;
#endif

    if (t->milliseconds >= TIMER_MSEC_PER_DAY) {
        ++t->days;
        t->milliseconds -= TIMER_MSEC_PER_DAY;
        t->next_decisec -= TIMER_MSEC_PER_DAY;
    }

    /* Send signal every 100 msec. */ /* Потенциально не работает, если t->msec_per_tick > 100 */
#if( defined USEC_TIMER || defined DMA_TIMER)
    if (t->usec_per_tick / 1000 <= 100 &&
#else
    if (t->msec_per_tick <= 100 &&
#endif
        t->milliseconds >= t->next_decisec)
    {
        t->next_decisec += 100;
/* debug_printf ("<ms=%lu,nxt=%lu> ", t->milliseconds, t->next_decisec); */
        // if (! list_is_empty (&t->decisec.waiters) || ! list_is_empty (&t->decisec.groups))
        // {
        //     mutex_activate (&t->decisec, (void*) (size_t) t->milliseconds);
        // }
    }

// #ifdef TIMER_TIMEOUTS
//     if (! list_is_empty (&t->timeouts)) {
//         timeout_t *to;
//         list_iterate (to, &t->timeouts) {
// #ifdef USEC_TIMER
//             to->cur_time -= t->usec_per_tick;
// #else
//             to->cur_time -= t->msec_per_tick;
// #endif
//             if (to->cur_time <= 0) {
//                 if (to->handler)
//                     to->handler(to, to->handler_arg);
//                 if (! list_is_empty (&to->mutex->waiters) ||
//                         ! list_is_empty (&to->mutex->groups)) {
//                     mutex_activate (to->mutex, to->signal);
//                 }
//                 if (to->autoreload) {
//                     to->cur_time += to->interval;
//                 } else {
//                     timeout_t *prev_to = (timeout_t *) to->item.prev;
//                     list_unlink (&to->item);
//                     to = prev_to;
//                 }
//             }
//         }
//     }
// #endif
}

/*
 * Timer interrupt handler.
 */
bool_t
timer_handler (timer_t *t)
{
/*debug_printf ("<ms=%ld> ", t->milliseconds);*/

#if defined (ELVEES)
    /* Clear interrupt. */
    MC_ITCSR &= ~MC_ITCSR_INT;
#endif
#if ARM_AT91SAM
    /* Clear interrupt. */
    *AT91C_PITC_PIVR;
#endif
#if ARM_OMAP44XX
    /* Clear interrupt. */
    ARM_PRT_INT_STATUS = ARM_PRT_EVENT;
#endif
#if ARM_1986BE1
    SYS_TIMER->TIM_STATUS &= ~ARM_TIM_CNT_ARR_EVENT;
#endif
#if PIC32MX
    /* Increment COMPARE register. */
    unsigned compare = mips_read_c0_register (C0_COMPARE);
    do {
        compare += t->compare_step;
        mips_write_c0_register (C0_COMPARE, compare);
    } while ((int) (compare - mips_read_c0_register (C0_COUNT)) < 0);
#endif
#if defined (RTC_TIMER)
#   if defined (ARM_STM32L151RC) || defined (ARM_STM32L152RC)
        RTC->ISR &= ~RTC_WUTF;
        PWR->CR |= PWR_CWUF;
        EXTI->PR = EXTI_RTC_WKUP;
#   endif
#endif

    timer_update (t);
    
    #ifdef DMA_TIMER
    // if(t->flag){
    //     if(t->msec_signal == t->milliseconds){
    //         if(t->usec_in_msec - t->usec_signal > 200){
    //             t->flag = 0;
    //             arch_intr_allow (TIMER_IRQ);
    //             return 0;
    //         }
    //     }else{
    //         if(TIMER_MSEC_PER_DAY + t->usec_in_msec - t->usec_signal > 200){
    //             t->flag = 0;
    //             arch_intr_allow (TIMER_IRQ);
    //             return 0;
    //         }
    //     }
    // } else if(t->unset == 0 && t->flag == 0){
    //     arch_intr_allow (TIMER_IRQ);
    //     return 0;
    // }
    #endif
    arch_intr_allow (TIMER_IRQ);

    /* Must signal a lock, for timer_wait().
     * TODO: Optimize timer_delay, keeping a sorted
     * queue of delay values. */
    return 1;
}

/**\~english
 * Return the (real) time in milliseconds since uOS start.
 *
 * \~russian
 * Запрос времени в миллисекундах.
 */
unsigned long
timer_milliseconds (timer_t *t)
{
    unsigned long val;

    mutex_lock (&t->lock);
    val = t->milliseconds;
    mutex_unlock (&t->lock);
    return val;
}

uint64_t
timer_microseconds (timer_t *t)
{
    uint64_t val;

    mutex_lock (&t->lock);
    val = t->milliseconds * 1000;
#ifdef USEC_TIMER
    val += t->usec_in_msec;
#endif
    mutex_unlock (&t->lock);
    return val;
}

/**\~english
 * Return the (real) time in days and milliseconds since uOS start.
 *
 * \~russian
 * Запрос времени в сутках и миллисекундах.
 */
unsigned int
timer_days (timer_t *t, unsigned long *milliseconds)
{
    unsigned short val;

    mutex_lock (&t->lock);
    if (milliseconds)
        *milliseconds = t->milliseconds;
    val = t->days;
    mutex_unlock (&t->lock);
    return val;
}

/**\~english
 * Delay the current task by the given time in milliseconds.
 *
 * \~russian
 * Задержка выполнения текущей задачи.
 */
void
timer_delay (timer_t *t, unsigned long msec)
{
    unsigned long t0;

    mutex_lock (&t->lock);
    t0 = t->milliseconds;
    while (! interval_greater_or_equal (t->milliseconds - t0, msec)) {
        mutex_wait (&t->lock);
    }
    mutex_unlock (&t->lock);
}

/**\~english
 * Check that `msec' milliseconds passed since the `t0' moment.
 *
 * \~russian
 * Проверка временного события.
 */
bool_t
timer_passed (timer_t *t, unsigned long t0, unsigned int msec)
{
    unsigned long now;

    mutex_lock (&t->lock);
    now = t->milliseconds;
    mutex_unlock (&t->lock);

    return interval_greater_or_equal (now - t0, msec);
}

/**
 * @brief      Функция деинициализации таймера
 *
 */
void timers_deinit(void)
{
    ARM_RSTCLK->TIM_CLOCK = 0;
    
    ARM_TIMER1->TIM_CNTRL = 0;
    ARM_TIMER1->TIM_CNT = 0;
    ARM_TIMER1->TIM_PSG = 0;
    ARM_TIMER1->TIM_ARR = 0;
    ARM_TIMER1->TIM_IE = 0;
    ARM_TIMER2->TIM_CNTRL = 0;
    ARM_TIMER2->TIM_CNT = 0;
    ARM_TIMER2->TIM_PSG = 0;
    ARM_TIMER2->TIM_ARR = 0;
    ARM_TIMER2->TIM_IE = 0;
    ARM_TIMER3->TIM_CNTRL = 0;
    ARM_TIMER3->TIM_CNT = 0;
    ARM_TIMER3->TIM_PSG = 0;
    ARM_TIMER3->TIM_ARR = 0;
    ARM_TIMER3->TIM_IE = 0;
}

#if( defined USEC_TIMER || defined DMA_TIMER)
/**\~english
 * Nanosecond Timer initialization.
 *
 * \~russian
 * Инициализация наносекудного таймера.
 */
void
timer_init_us (timer_t *t, unsigned long khz, unsigned long usec_per_tick)
{
    t->usec_per_tick = usec_per_tick;
    t->khz = khz;
    #ifdef DMA_TIMER
    t->unset = 1;
    #endif
#ifndef SW_TIMER
    /* Attach fast handler to timer interrupt. */
    mutex_attach_irq (&t->lock, TIMER_IRQ, (handler_t) timer_handler, t);

    /* Initialize the hardware. */
#if ARM_1986BE9
    ARM_SYSTICK->CTRL = 0;
    ARM_SYSTICK->VAL = 0;
    ARM_SYSTICK->LOAD = t->khz / 1000 * t->usec_per_tick - 1;
    ARM_SYSTICK->CTRL = ARM_SYSTICK_CTRL_ENABLE |
                ARM_SYSTICK_CTRL_TICKINT |
                ARM_SYSTICK_CTRL_HCLK;
#endif // ARM_1986BE9

#if ARM_1986BE1
    ARM_RSTCLK->PER_CLOCK |= PER_CLOCK_EN;
#if (ARM_SYS_TIMER==4)
    ARM_RSTCLK->UART_CLOCK |= TIM_CLK_EN;
#else
    ARM_RSTCLK->TIM_CLOCK |= TIM_CLK_EN;
#endif
    SYS_TIMER->TIM_CNT = 0;
    SYS_TIMER->TIM_PSG = 0;
    SYS_TIMER->TIM_ARR = t->khz / 1000 * t->usec_per_tick - 1;
    SYS_TIMER->TIM_IE = ARM_TIM_CNT_ARR_EVENT_IE;
    SYS_TIMER->TIM_CNTRL = ARM_TIM_CNT_EN;
#endif


#endif // SW_TIMER

#ifdef TIMER_TIMEOUTS
    list_init (&t->timeouts);
#endif
}
#else



/**\~english
 * Create timer task.
 *
 * \~russian
 * Инициализация таймера.
 */
void
timer_init (timer_t *t, unsigned long khz, small_uint_t msec_per_tick)
{
    t->msec_per_tick = msec_per_tick;
    t->khz = khz;

#ifndef SW_TIMER
    /* Attach fast handler to timer interrupt. */
    mutex_attach_irq (&t->lock, TIMER_IRQ, (handler_t) timer_handler, t);

    /* Initialize the hardware. */
#if __AVR__
    TCCR1A = 0;
    TCCR1B = 0;
    OCR1A = (t->khz * t->msec_per_tick) / 8 - 2;
    TCNT1 = 0;
    TCCR1B = 0x0A;  /* clock source CK/8, clear on match A */
#endif
#if I386
    {
    /* For I386, t->khz contains actually Hz (=1193182). */
    unsigned short count = (t->khz * t->msec_per_tick + 500) / 1000;
    outb (I8253_MODE_SEL0 | I8253_MODE_RATEGEN | I8253_MODE_16BIT,
        I8253_MODE);
    outb (count & 0xff, I8253_CNTR0);
    outb (count >> 8, I8253_CNTR0);
    }
#endif
#if ARM_S3C4530
    /* Use timer 0 in toggle mode. */
    ARM_TMOD &= ~(ARM_TMOD_TE0 | ARM_TMOD_TCLR0);
    ARM_TDATA(0) = (t->khz * t->msec_per_tick) - 1;
    ARM_TCNT(0) = 0;
    ARM_TMOD |= ARM_TMOD_TE0 | ARM_TMOD_TMD0;
#endif
#if ARM_AT91SAM
    *AT91C_PITC_PIMR = (((t->khz * t->msec_per_tick + 8) >> 4) - 1) |
        AT91C_PITC_PITEN | AT91C_PITC_PITIEN;
#endif
#if ARM_OMAP44XX
    ARM_PRT_LOAD = khz * msec_per_tick - 1;
    ARM_PRT_CONTROL = ARM_PRT_TIMER_EN | ARM_PRT_AUTO_RELOAD |
        ARM_PRT_IRQ_EN;
#endif
#if PIC32MX
    /* Use core timer. */
    unsigned count = mips_read_c0_register (C0_COUNT);
    t->compare_step = (t->khz * t->msec_per_tick + 1) / 2;
    mips_write_c0_register (C0_COMPARE, count + t->compare_step);
#endif
#if defined (ELVEES)
    /* Use interval timer with prescale 1:1. */
    MC_ITCSR = 0;
    MC_ITSCALE = 0;
    MC_ITPERIOD = t->khz * t->msec_per_tick - 1;
    MC_ITCSR = MC_ITCSR_EN;
#endif
#if ARM_CORTEX_M3 || ARM_CORTEX_M4
#   if defined (RTC_TIMER)
#       if defined (ARM_STM32L151RC) || defined (ARM_STM32L152RC)
            /* Clear write protection for RTC registers */
            RCC->APB1ENR |= RCC_PWREN;
            PWR->CR |= PWR_DBP;
            RTC->WPR = 0xCA;
            RTC->WPR = 0x53;
            /* Enable LSE and set it as clock source for RTC */
            RCC->CSR |= RCC_LSEON;
            while (! (RCC->CSR & RCC_LSERDY));
            RCC->CSR |= RCC_RTCEN | RCC_RTCSEL_LSE;
            /* Enable RTC Wakeup interrupt in the EXTI */
            EXTI->PR = EXTI_RTC_WKUP;
            EXTI->RTSR |= EXTI_RTC_WKUP;
            EXTI->IMR |= EXTI_RTC_WKUP;
            /* HZ_CLKIN_RTC is divided by 2 by WUT Prescaler */
            RTC->CR = 0;
            while (! (RTC->ISR & RTC_WUTWF));
            RTC->WUTR = (HZ_CLKIN_RTC / 2) * t->msec_per_tick / 1000 - 1;
            RTC->CR = RTC_WUCKSEL_DIV2 | RTC_WUTE | RTC_WUTIE;
#       endif
#   else
        ARM_SYSTICK->CTRL = 0;
        ARM_SYSTICK->VAL = 0;
#       ifdef SETUP_HCLK_HSI
            /* Max 2130 msec/tick at 8 MHz. */
            ARM_SYSTICK->LOAD = 8000 * t->msec_per_tick - 1;
#       else
            /* Max 213 msec/tick at 80 MHz. */
            ARM_SYSTICK->LOAD = t->khz * t->msec_per_tick - 1;
#       endif
            ARM_SYSTICK->CTRL = ARM_SYSTICK_CTRL_ENABLE |
                        ARM_SYSTICK_CTRL_TICKINT |
                        ARM_SYSTICK_CTRL_HCLK;
#   endif
#endif
#if ARM_1986BE1
    ARM_RSTCLK->PER_CLOCK |= PER_CLOCK_EN;
#if (ARM_SYS_TIMER==4)
    ARM_RSTCLK->UART_CLOCK |= TIM_CLK_EN;
#else
    ARM_RSTCLK->TIM_CLOCK |= TIM_CLK_EN;
#endif
    SYS_TIMER->TIM_CNT = 0;
    SYS_TIMER->TIM_PSG = 0;
    SYS_TIMER->TIM_ARR = t->khz * t->msec_per_tick - 1;
    SYS_TIMER->TIM_IE = ARM_TIM_CNT_ARR_EVENT_IE;
    SYS_TIMER->TIM_CNTRL = ARM_TIM_CNT_EN;
#endif
#if MSP430
    {
    unsigned long divider = t->msec_per_tick * t->khz;
#ifdef TAEX0
    /* Setup divider expansion. */
    small_uint_t nx = (divider - 4) >> (16 + 3);
    assert (nx <= 7);
    TAEX0 = nx;
    if (nx)
        divider /= nx + 1;
#endif
    TACTL = TACLR;              /* Stop timer. */
    TACTL = TASSEL_SMCLK | ID_DIV8;     /* Source clock SMCLK divided by 8. */
    divider = (divider + 4) >> 3;
    assert (divider <= 65536);
    TACCR0 = divider - 1;           /* Set tick rate. */
    TACTL |= MC_1;              /* Start timer in up mode. */
    }
#endif
#if LINUX386
    {
    struct itimerval itv;

    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = t->msec_per_tick * 1000L;
    itv.it_value = itv.it_interval;
    setitimer (ITIMER_REAL, &itv, 0);
    }
#endif

#endif // SW_TIMER

#ifdef TIMER_TIMEOUTS
    list_init (&t->timeouts);
#endif
}
#endif // USEC_TIMER
