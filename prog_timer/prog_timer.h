/**
 * @file    prog_timer.h
 * @author  Ilia Zubkov (mindwork), Dmitrii Kaleev (aod)
 * @date    18 June 2018
 * @version 0.0.2
 * @brief   WatchDog Driver for Milandr MCU
 *
 * @note
 *
 */
/*******************************************************************************
 * Copyright (c) 2016 Ilia Zubkov (), Dmitrii Kaleev (kaleev@org.miet.ru)      *
 *                                                                             *
 * The MIT License (MIT):                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a     *
 * copy of this software and associated documentation files (the "Software"),  *
 * to deal in the Software without restriction, including without limitation   *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell   *
 * copies of the Software, and to permit persons to whom the Software is       *
 * furnished to do so, subject to the following conditions:                    *
 * The above copyright notice and this permission notice shall be included     *
 * in all copies or substantial portions of the Software.                      *
 *                                                                             *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,             *
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR       *
 * OTHER DEALINGS IN THE SOFTWARE.                                             *
 ******************************************************************************/

#ifndef _PROG_TIMER_H_
#define _PROG_TIMER_H_

#include <runtime/lib.h>
#include <kernel/uos.h>
#include <timer/timer.h>


//настройки для M3
#define TIMER_HANDLE 	int16_t
#define TIMEVAL 	 	uint16_t
#define TIMEVAL_MAX 	~(TIMEVAL)0

/* --------- types and constants definitions --------- */
#define TIMER_FREE 0
#define TIMER_ARMED 1
#define TIMER_TRIG 2
#define TIMER_TRIG_PERIOD 3

#define TIMER_NONE -1

#ifndef USEC_TIMER
#define MS_TO_TIMEVAL(ms) (ms)
#define US_TO_TIMEVAL(us) (1)
#else
#define MS_TO_TIMEVAL(ms) (ms*1000)
#define US_TO_TIMEVAL(us) (us)
#endif


/*	для эпохи задействуем три аппаратных таймера с разными предделителями и соответственно разными погрешностями
	верхний предел для каждого таймера будет свой, более HPT таймер используется для времен по умолчанию меньших 100 мс
	MPT для времен меньших 1000мс и LPT для времен больших 1000мс или для задержек, для которых не особо важна точность времени отправки,
	например таймауты передачи 	
*/
//HIGH PRECISION TIMER
#define TIMER_1 0
//MEDIUM PRECISION TIMER
#define TIMER_2 1
//LOW PRECISION TIMER
#define TIMER_3 2

#ifndef TIMER_1_PERIOD
#define TIMER_1_PERIOD      50      // ms
#endif

#ifndef TIMER_2_PERIOD
#define TIMER_2_PERIOD      5       // ms
#endif

#ifndef TIMER_3_PERIOD
#define TIMER_3_PERIOD      125     // ms
#endif

#define DEFINE_TIMER_NUM(interval)   (interval < TIMER_1_PERIOD ?  TIMER_2 : TIMER_1)

/*
 * Data structure for timer
 */
typedef struct timers_t{
    mutex_t lock8us;       /* сюда будут приходить прерывания  */
    unsigned port;      /* timer number */
    TIMER_t *reg;
    unsigned last_time_set;
}timers_t;

typedef struct{
    TIMER_t *reg;
    mutex_t lock;
    unsigned long milliseconds;
}timer_n_t;

typedef struct{
    TIMER_t         *reg;
    mutex_t         lock;
    unsigned int    khz;
    unsigned short  psg;    //предделитель таймера
    int             port;
    unsigned        irq_flag;
} timer_arr_t;


typedef struct{
    TIMER_t         *reg;
    mutex_t         lock;
    unsigned long   khz;
    int             port;
    unsigned long   microseconds;
    unsigned long   milliseconds;
    unsigned long   usec_per_tick;
} clock_t;



void timers_init ( timers_t *t, int port);


/*
*   Установка ARR таймера
*/
void set_arr_timer(timer_arr_t *t, unsigned long ms_value, uint32_t usec_value);


/*
*   Остановка ARR таймера
*/
void stop_arr_timer(timer_arr_t *t);


void timer_arr_init(timer_arr_t *t, int port, unsigned int khz, unsigned int basis);



/*
*   Текущее время в микросекундах с момента запуска
*/
unsigned long get_current_time(clock_t *t);


/*
*   Таймер, который просто тикает и выдает текущее время с момента запуска 
*/
void clock_init (clock_t *t, int port, unsigned long khz, unsigned long usec_per_tick);



typedef void (*TimerCallback_t)(void* d, uint32_t id);

struct struct_s_timer_entry {
	uint8_t state;
	void* d;
	TimerCallback_t callback; 	/* The callback func. */
	uint32_t id; 				/* The callback func. */
	TIMEVAL val;
	TIMEVAL interval; 			/* Periodicity */
};

typedef struct struct_s_timer_entry s_timer_entry;

/*
*	Установка таймера с коллбэком
*/
TIMER_HANDLE set_alarm_num(void* d, uint32_t id, TimerCallback_t callback, TIMEVAL value, TIMEVAL period, uint8_t num);

/*
*	Уничтожение таймера
*/
TIMER_HANDLE del_alarm_num(TIMER_HANDLE handle, uint8_t num);


/*
*	Обработка таймера по срабатыванию
*/
void time_dispatch_num(uint8_t num);


/*
*	Установка таймера, переопределяется в взаивисмости от платформы
*/
void set_timer_num(TIMEVAL value, uint8_t num);

/*
*	Для получения времени, прошедшего с полследнего срабатывания таймера, переопределяется, в зависимости от платформы
*/
TIMEVAL get_elapsed_time_num(uint8_t num);

/*
*	Функция инициализации таймеров
*/
void init_alarms_num();

void task_program_timer(void *arg);

/*
*	Функцяи для инициалиации программного таймера, првязанного к апапртному arr таймеру
*/
void programm_timer_init(uint8_t number, uint32_t basis, timer_t *systemTimer);


#endif//_PROG_TIMER_H_