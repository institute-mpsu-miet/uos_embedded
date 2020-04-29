/**
 * @file    prog_timer.c
 * @author  Ilia Zubkov (mindwork), Dmitrii Kaleev (aod)
 * @date    18 June 2018
 * @version 0.0.2
 * @brief   Program timers for Milandr MCU
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

#ifndef _PROG_TIMER_C_
#define _PROG_TIMER_C_

#include <prog_timer/prog_timer.h>
#include <timer/timer.h>

//количество реальных таймеров(аппаратных или программных, которые будут обслуживать таймер CANOpen)
#ifndef MAX_TIMERS
	#define MAX_TIMERS 3
#endif	

#ifndef MAX_NB_TIMER
	#define MAX_NB_TIMER 32
#endif

/*  ---------  The timer table --------- */
s_timer_entry timers[MAX_TIMERS][MAX_NB_TIMER]; //надо проинициализировать таймеры перед работой с ними


TIMEVAL total_sleep_time[MAX_TIMERS];
TIMER_HANDLE last_timer_raw[MAX_TIMERS];

#define min_val(a,b) ((a<b)?a:b)

timer_arr_t timer_arr1, timer_arr2, timer_arr3;


volatile TIMEVAL last_time_set[MAX_TIMERS];

timer_t *timer; //указатель на системный таймер

//локальный мутекс, для одновременного выполнения коллбэков с нескольких таймеров
mutex_t timer_lock;


/*
*   Установка ARR таймера
*/
void set_arr_timer(timer_arr_t *t, unsigned long ms_value, unsigned long usec_value)
{
    mutex_lock(&t->lock);
    t->reg->TIM_CNTRL=0;
    t->reg->TIM_CNT=0;
    t->reg->TIM_IE=0;
    t->reg->TIM_ARR=(uint16_t)(t->khz/t->psg*ms_value + t->khz/t->psg/1000*usec_value - 1);
    t->reg->TIM_IE = ARM_TIM_CNT_ARR_EVENT_IE;
    t->reg->TIM_CNTRL = ARM_TIM_CNT_EN;
    t->irq_flag = 0;
    mutex_unlock(&t->lock);
}


/*
*   Остановка ARR таймера
*/
void stop_arr_timer(timer_arr_t *t)
{
    t->reg->TIM_CNTRL = 0;
    t->reg->TIM_CNT = 0;
    t->reg->TIM_IE = 0;
    t->irq_flag = 0;
}


/*
*   Быстрый обработчик, для arr таймера
*/
bool_t timer_arr_handler(timer_arr_t *t)
{
    t->reg->TIM_STATUS &=~ARM_TIM_CNT_ARR_EVENT;
    
    t->reg->TIM_CNTRL = 0;
    t->reg->TIM_CNT = 0;
    t->reg->TIM_IE = 0;
    t->irq_flag = 1; //признак того, что таймер сработал

    switch(t->port){
        case 0: arch_intr_allow (TIMER1_IRQn);
        break;
        case 1: arch_intr_allow (TIMER2_IRQn);
        break;
        case 2: arch_intr_allow (TIMER3_IRQn);
        break;
        #ifdef ARM_1986BE1 
        case 3: arch_intr_allow (TIMER4_IRQn);
        #endif
        break;
    }

    return 0;
}


/*
*   Функция установки arr таймера
*/
void timer_arr_init(timer_arr_t *t, int port, unsigned int khz, unsigned int basis)
{
    t->khz=khz;
    t->port=port;   
    t->psg=(khz*basis/1000) - 1;     //делитель частоты счета таймера
     // t->psg=(basis);     //делитель частоты счета таймера

    debug_printf("Инициализирован ARR таймер %d khz %d \n",t->port+1, t->khz );

    switch(t->port){
        case TIMER_1: 
            t->reg = ARM_TIMER1;
            ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_TIMER1;
            ARM_RSTCLK->TIM_CLOCK |= ARM_TIM1_CLOCK_EN;
            mutex_attach_irq(&t->lock, TIMER1_IRQn, (handler_t)timer_arr_handler, t);
        break;
        case TIMER_2: 
            t->reg = ARM_TIMER2;  
            ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_TIMER2; 
            ARM_RSTCLK->TIM_CLOCK |= ARM_TIM2_CLOCK_EN; 
            mutex_attach_irq(&t->lock, TIMER2_IRQn, (handler_t)timer_arr_handler, t);
        break;
        case TIMER_3: 
            t->reg = ARM_TIMER3;  
            ARM_RSTCLK->PER_CLOCK |= ARM_PER_CLOCK_TIMER3;
            ARM_RSTCLK->TIM_CLOCK |= ARM_TIM3_CLOCK_EN;
            mutex_attach_irq(&t->lock, TIMER3_IRQn, (handler_t)timer_arr_handler, t);
        break;
    }

    t->reg->TIM_CNT = 0;
    t->reg->TIM_PSG = t->psg;
    t->reg->TIM_ARR = 0;
    t->irq_flag = 0;
}




/*
*	Функция инициализации таймеров
*/
void init_alarms()
{
	int i=0, j=0;
	for(i=0;i<MAX_TIMERS;i++){

		for(j=0;j<MAX_NB_TIMER;j++){

			timers[i][j].state=TIMER_FREE;
			timers[i][j].d=NULL;
			timers[i][j].callback=NULL;
			timers[i][j].id=0;
			timers[i][j].val=0;
			timers[i][j].interval=0;
		}	

		total_sleep_time[i]=TIMEVAL_MAX;
		last_timer_raw[i]=-1;
	}
}

/*
* 	Установка таймеров с привязкой к номеру апапартного таймера
*/
TIMER_HANDLE set_alarm_num(void* d, uint32_t id, TimerCallback_t callback, TIMEVAL value, TIMEVAL period, uint8_t num)
{
	TIMER_HANDLE row_number;
	s_timer_entry *row;

	/* in order to decide new timer setting we have to run over all timer rows */
	for(row_number=0, row=timers[num]; row_number <= last_timer_raw[num] + 1 && row_number < MAX_NB_TIMER; row_number++, row++)
	{
		if (callback && 	/* if something to store */
		   row->state == TIMER_FREE) /* and empty row */
		{	/* just store */
			TIMEVAL real_timer_value;
			TIMEVAL elapsed_time;

			if (row_number == last_timer_raw[num] + 1) last_timer_raw[num]++;

			elapsed_time = get_elapsed_time_num(num);
			/* set next wakeup alarm if new entry is sooner than others, or if it is alone */
			real_timer_value = value;
			real_timer_value = min_val(real_timer_value, TIMEVAL_MAX);
			
			//MSG_WAR(0x3A05, "real_timer_value", real_timer_value);

			if (total_sleep_time[num] > elapsed_time && total_sleep_time[num] - elapsed_time > real_timer_value)
			{
				total_sleep_time[num] = elapsed_time + real_timer_value;
				//MSG_WAR(0x3A05, "real_timer_value", real_timer_value);
				//printf("real_timer_value %d\n",real_timer_value);
				set_timer_num(real_timer_value, num);
			}
			row->callback = callback;
			row->d = d;
			row->id = id;
			row->val = value + elapsed_time;
			
			//printf("row->val %x \n",value);

			row->interval = period;
			row->state = TIMER_ARMED;
			return row_number;
		}
	}

	return TIMER_NONE;
}


/*
*	Для удаления таймера по номеру
*/
TIMER_HANDLE del_alarm_num(TIMER_HANDLE handle, uint8_t num)
{
	/* Quick and dirty. system timer will continue to be trigged, but no action will be preformed. */
	if(handle != TIMER_NONE){

		if(handle == last_timer_raw[num])
			last_timer_raw[num]--;
		timers[num][handle].state = TIMER_FREE;
	}

	return TIMER_NONE;
}


/*
*	Удаление таймера по номеру
*/
void time_dispatch_num(uint8_t num)
{
	//printf("Time Dispatch");
	TIMER_HANDLE i;
	TIMEVAL next_wakeup = TIMEVAL_MAX; /* used to compute when should normaly occur next wakeup */
	/* First run : change timer state depending on time */
	/* Get time since timer signal */
	uint32_t overrun = (uint32_t)get_elapsed_time_num(num);
	TIMEVAL real_total_sleep_time = total_sleep_time[num] + overrun;

	s_timer_entry *row;

	for(i=0, row = timers[num]; i <= last_timer_raw[num]; i++, row++)
	{
		if (row->state & TIMER_ARMED) /* if row is active */
		{
			if (row->val <= real_total_sleep_time) /* to be trigged */
			{
				if (!row->interval) /* if simply outdated */
				{
					row->state = TIMER_TRIG; /* ask for trig */
				}
				else /* or period have expired */
				{
					/* set val as interval, with 32 bit overrun correction, */
					/* modulo for 64 bit not available on all platforms     */
					row->val = row->interval - (overrun % (uint32_t)row->interval);
					row->state = TIMER_TRIG_PERIOD; /* ask for trig, periodic */
					/* Check if this new timer value is the soonest */
					if(row->val < next_wakeup)
						next_wakeup = row->val;
				}
			}
			else
			{
				/* Each armed timer value in decremented. */
				row->val -= real_total_sleep_time;

				/* Check if this new timer value is the soonest */
				if(row->val < next_wakeup)
					next_wakeup = row->val;
			}
		}
	}

	/* Remember how much time we should sleep. */
	total_sleep_time[num] = next_wakeup;

	/* Set timer to soonest occurence */

	set_timer_num(next_wakeup, num);

	/* Then trig them or not. */
	for(i=0, row = timers[num]; i<=last_timer_raw[num]; i++, row++)
	{
		if (row->state & TIMER_TRIG)
		{
			row->state &= ~TIMER_TRIG; /* reset trig state (will be free if not periodic) */
			if(row->callback)
				(*row->callback)(row->d, row->id); /* trig ! */
		}
	}
}



#ifndef USEC_TIMER
/*
*   Функция подсчета прошедшего времени для разных таймеров
*/
TIMEVAL get_elapsed_time_num(uint8_t num)
{   
    TIMEVAL time;
    //время в милисекундах
    time=timer_milliseconds(timer); 
    // debug_printf("time %d \n", time);
    time=time > last_time_set[num] ? (time - last_time_set[num]) : (~(TIMEVAL)0-last_time_set[num] + time+1);
    
    return time;
}

#else //USEC_TIMER

/*
*   Функция подсчета прошедшего времени для разных таймеров - переопределена для canopen
*/
TIMEVAL get_elapsed_time_num(uint8_t num)
{   
    TIMEVAL time;
    //время в микросекундах
    time=timer_microseconds(timer); 
    // debug_printf("time %d \n", time);
    time=time > last_time_set[num] ? (time - last_time_set[num]) : (~(TIMEVAL)0-last_time_set[num] + time+1);
    return time;
}

#endif


#ifndef USEC_TIMER
/*
*   Установка таймера по номеру -переопределена для canopen
*/
void set_timer_num(TIMEVAL value, uint8_t num)
{
    switch(num)
    {
        case 0:
            set_arr_timer(&timer_arr1, value,0);
            break;
        case 1:
            set_arr_timer(&timer_arr2, value,0);
            break;
        case 2: 
            // debug_printf("Установка времени %d \n", value);
            set_arr_timer(&timer_arr3, value, 0);
            break;
        default:
            debug_printf("ERROR: Неверный номер таймера \n");
            break;                   
    }
} 

#else //USEC_TIMER

/*
*   Установка таймера по номеру -переопределена для canopen
*/
void set_timer_num(TIMEVAL value, uint8_t num)
{
    switch(num)
    {
        case 0:
            set_arr_timer(&timer_arr1, 0, value);
            break;
        case 1:
            set_arr_timer(&timer_arr2, 0, value);
            break;
        case 2:
            set_arr_timer(&timer_arr3, 0, value);
            break;
        default:
            debug_printf("ERROR: Неверный номер таймера \n");
            break;                   
    }
} 

#endif //USEC_TIMER


/*
*	Таск таймера
*/
void task_program_timer(void *arg)
{
    uint8_t num=*(uint8_t*)(arg);

    debug_printf("Запуск таска обработки таймера %d \n", num);

    timer_arr_t  *timer_arr;

    switch(num){
        case 0:
            timer_arr=&timer_arr1;
            break;
        case 1:
            timer_arr=&timer_arr2;
            break;    
        case 2:
            timer_arr=&timer_arr3;
            break; 
        default:
            timer_arr=NULL;
            // debug_printf("ERROR: неверный номер таймера!\n");                             
            break;
    }

    #ifndef USEC_TIMER
    last_time_set[num]=timer_milliseconds(timer);
    #else
    last_time_set[num]=timer_microseconds(timer);
    #endif

    mutex_lock(&timer_lock);
    // if(num==1){
    //     setState(&_Data, Initialisation);
    // }
    time_dispatch_num(num);
    mutex_unlock(&timer_lock);

    while(1){
        mutex_lock(&timer_arr->lock);
        if(!timer_arr->irq_flag) //сработал таймер или нет
            mutex_wait(&timer_arr->lock);
        mutex_unlock(&timer_arr->lock); 

        #ifndef USEC_TIMER
        last_time_set[num]=timer_milliseconds(timer);
        #else
        last_time_set[num]=timer_microseconds(timer);
        #endif
    
        mutex_lock(&timer_lock);
        time_dispatch_num(num);
        mutex_unlock(&timer_lock);
    }
}  

/**
 * @brief Функцяи для инициалиации программного таймера, првязанного к апапртному arr таймеру
 * 
 * @param number - номер аппаратного таймера
 * @param basis - период срабатывания таймера
 * @param systemTimer - указатель на драйвер таймера
 * @return
 */
void programm_timer_init(uint8_t number, uint32_t basis, timer_t *systemTimer)
{
	init_alarms();

	if(number>2){
		debug_printf("error timer number %d  > 2 \n", number);
		return;
	}

	timer=systemTimer;
	debug_printf("timer  num  %d\n", number);

	switch(number){
		case TIMER_1:
			//инициалиазция аппартаного таймера
			timer_arr_init(&timer_arr1, number, KHZ, basis);			
            break;
		case TIMER_2:
			timer_arr_init(&timer_arr2, number, KHZ, basis);
			break;	
		case TIMER_3:
			timer_arr_init(&timer_arr3, number, KHZ, basis);   			
            break;		
	}
}


#endif //_PROG_TIMER_C_