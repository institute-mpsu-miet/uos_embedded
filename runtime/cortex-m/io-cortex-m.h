/*
 * Register definitions for ARM Cortex cores.
 *
 * Copyright (C) 2010 Serge Vakulenko, <serge@vak.ru>
 *               2013 Dmitry Podkhvatilin, <vatilin@gmail.com>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "COPYING.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "COPY-UOS.txt" for details.
 */

/*------------------------------------------------------
 * SysTick timer
 */
typedef struct
{
    arm_reg_t CTRL;     /* Управление и статус */
    arm_reg_t LOAD;     /* Загружаемое значение */
    arm_reg_t VAL;      /* Текущее состояние */
    arm_reg_t CALIB;    /* Подстройка */
} SYSTICK_t;

#define ARM_SYSTICK     ((SYSTICK_t*) (ARM_SYSTEM_BASE + 0xE010))

/*
 * Регистр SYSTICK CTRL: управление и статус системного таймера.
 */
#define ARM_SYSTICK_CTRL_COUNTFLAG  (1 << 16) /* досчитал ли таймер до 0 */
#define ARM_SYSTICK_CTRL_HCLK       (1 << 2)  /* источник 0-Ext clock, 1-HCLK */
#define ARM_SYSTICK_CTRL_TICKINT    (1 << 1)  /* разрешение прерывания */
#define ARM_SYSTICK_CTRL_ENABLE     (1 << 0)  /* пуск таймера */

/*------------------------------------------------------
 * NVIC interrupt controller
 */
/* Разрешение прерываний */
#define ARM_NVIC_ISER(n)  (*(arm_reg_t*) (ARM_SYSTEM_BASE + 0xE100 + (n) * 4))
/* Запрет прерывания */
#define ARM_NVIC_ICER(n)  (*(arm_reg_t*) (ARM_SYSTEM_BASE + 0xE180 + (n) * 4))
/* Перевод прерывания в состояние ожидания обслуживания */
#define ARM_NVIC_ISPR(n)  (*(arm_reg_t*) (ARM_SYSTEM_BASE + 0xE200 + (n) * 4))
/* Сброс состояния ожидания обслуживания */
#define ARM_NVIC_ICPR(n)  (*(arm_reg_t*) (ARM_SYSTEM_BASE + 0xE280 + (n) * 4))
/* Активные прерывания */
#define ARM_NVIC_IABR(n)  (*(arm_reg_t*) (ARM_SYSTEM_BASE + 0xE300 + (n) * 4))
/* Приоритет прерываний */
#define ARM_NVIC_IPR(n)   (*(arm_reg_t*) (ARM_SYSTEM_BASE + 0xE400 + (n) * 4))
/* Программное формирование прерывания */
#define ARM_NVIC_STIR     (*(arm_reg_t*) (ARM_SYSTEM_BASE + 0xEF00))

/*------------------------------------------------------
 * SCB system control block
 */
typedef struct
{
    arm_reg_t CPUID;      /* Идентификация процессора */
    arm_reg_t ICSR;       /* Управление прерываниями */
    arm_reg_t VTOR;       /* Смещение таблицы векторов прерываний */
    arm_reg_t AIRCR;      /* Управление прерываниями и программным сбросом */
    arm_reg_t SCR;        /* Управление системой */
    arm_reg_t CCR;        /* Конфигурация и управление */
    arm_reg_t SHPR1;      /* Приоритет обработчиков memory/bus/usage fault */
    arm_reg_t SHPR2;      /* Приоритет обработчика SVCall */
    arm_reg_t SHPR3;      /* Приоритет обработчиков SysTick, PendSV */
    arm_reg_t SHCSR;      /* Управление и состояние системных обработчиков */
    arm_reg_t CFSR;       /* Состояние отказов с конфигурируемым приоритетом */
    arm_reg_t HFSR;       /* Состояние тяжелого отказа */
    arm_reg_t UNUSED;     /* DFSR */
    arm_reg_t MMAR;       /* Адрес отказа доступа к памяти */
    arm_reg_t BFAR;       /* Адрес отказа доступа к шине */
    arm_reg_t AFSR;       /* Состояние внешнего отказа */
} SCB_t;

#define ARM_SCB     ((SCB_t*) (ARM_SYSTEM_BASE + 0xED00))


/*
 * Регистр SCB ICSR: управление прерываниями.
 */
/* (RW) установка NMI */
#define ARM_ICSR_NMIPENDSET     (1 << 31)
/* (RW) установка ожидания обслуживания PendSV */
#define ARM_ICSR_PENDSVSET      (1 << 28)
/* (WO) сброс ожидания обслуживания PendSV */
#define ARM_ICSR_PENDSVCLR      (1 << 27)
/* (RW) установка ожидания обслуживания SysTick */
#define ARM_ICSR_PENDSTSET      (1 << 26)
/* (WO) сброс ожидания обслуживания SysTick */
#define ARM_ICSR_PENDSTCLR      (1 << 25)
/* (RO) было ли вытеснение обработки исключения */
#define ARM_ICSR_ISRPREEMPT     (1 << 23)
/* (RO) есть прерывания, ожидающие обслуживания */
#define ARM_ICSR_ISRPENDING     (1 << 22)
/* (RO) номер ожидающего исключения */
#define ARM_ICSR_VECTPENDING(s) ((s) >> 12 & 0x1ff)
/* (RO) есть активные прерванные исключения */
#define ARM_ICSR_RETTOBASE      (1 << 11)
/* (RO) номер активного исключения (0 - режим приложения) */
#define ARM_ICSR_VECTACTIVE(s)  ((s) & 0x1ff)

/*
 * Регистр SCB AIRCR: управление прерываниями и программный сброс.
 */
#define ARM_AIRCR_VECTKEY       (0x05FA << 16)  /* ключ доступа к регистру */
#define ARM_AIRCR_ENDIANESS     (1 << 15)       /* старший байт идет первым */
#define ARM_AIRCR_PRIGROUP(n)   ((n) << 8)      /* группировка приоритетов исключений */
#define ARM_AIRCR_SYSRESETREQ   (1 << 2)        /* запрос сброса системы */

/*
 * Регистр SCB SCR: управление системой.
 */
/* Указание на то, что переход из неактивного состояния был вызван 
 * событием просыпания */
#define ARM_SCR_SEVONPEND       (1 << 4)
/* Признак "глубокого сна" */
#define ARM_SCR_SLEEPDEEP       (1 << 2)
/* Признак засыпания при возвращении на базовый уровень приоритета */
#define ARM_SCR_SLEEPONEXIT     (1 << 1)

/*
 * Регистр SCB CCR: конфигурация и управление.
 */
/* Выравнивание стека на 8 байт при входе в исключение */
#define ARM_CCR_STKALIGN        (1 << 9)
/* Игнорирование доступа к "точным данным" на приоритета исполнения -1 и -2 */
#define ARM_CCR_BFHFNMIGN       (1 << 8)
/* Включение ловушки деления на 0 */
#define ARM_CCR_DIV_0_TRP       (1 << 4)
/* Включение ловушки невыровненного доступа */
#define ARM_CCR_UNALIGN_TRP     (1 << 3)
/* Разрешение доступа к STIR из непривилегированного кода */
#define ARM_CCR_USERSETMPEND    (1 << 1)
/* Разрешение перехода в Thread-mode на уровне приоритета, отличном от базового */
#define ARM_CCR_NONBASETHRDENA  (1 << 0)
 
/*
 * Регистр SCB SHCSR: управление и состояние системных обработчиков.
 */
#define ARM_SHCSR_USGFAULTENA    (1 << 18)  /* разрешение обработки отказов,
                                             * вызванных ошибками программирования */
#define ARM_SHCSR_BUSFAULTENA    (1 << 17)  /* разрешение обработки отказа доступа к шине */
#define ARM_SHCSR_MEMFAULTENA    (1 << 16)  /* разрешение обработки отказа доступа к памяти */
#define ARM_SHCSR_SVCALLPENDED   (1 << 15)  /* признак ожидания обработки вызова SVC */
#define ARM_SHCSR_BUSFAULTPENDED (1 << 14)  /* признак ожидания обработки отказа
                                             * доступа к шине */
#define ARM_SHCSR_MEMFAULTPENDED (1 << 13)  /* признак ожидания обработки отказа
                                             * доступа к памяти */
#define ARM_SHCSR_USGFAULTPENDED (1 << 12)  /* признак ожидания обработки отказа,
                                             * вызванного ошибками программирования */
#define ARM_SHCSR_SYSTICKACT     (1 << 11)  /* признак активности обработчика
                                             * исключения SysTick */
#define ARM_SHCSR_PENDSVACT      (1 << 10)  /* признак активности обработчика
                                             * исключения PendSV */
#define ARM_SHCSR_MONITORACT     (1 << 8)   /* признак активности монитора отладчика */
#define ARM_SHCSR_SVCALLACT      (1 << 7)   /* признак активности обработчика вызова SVC */
#define ARM_SHCSR_USGFAULTACT    (1 << 3)   /* признак активности обработчика отказа,
                                             * вызванного ошибкой программирования */
#define ARM_SHCSR_BUSFAULTACT    (1 << 1)   /* признак активности обработчика отказа
                                             * доступа к шине */
#define ARM_SHCSR_MEMFAULTACT    (1 << 0)   /* признак активности обработчика отказа
                                             * доступа к памяти */

/*
 * Регистр SCB CFSR: состояние конфигурируемых сбоев.
 */
#define ARM_CFSR_DIVBYZERO      (1 << 25)
#define ARM_CFSR_UNALIGNED      (1 << 24)
#define ARM_CFSR_NOCP           (1 << 19)
#define ARM_CFSR_INVPC          (1 << 18)
#define ARM_CFSR_INVSTATE       (1 << 17)
#define ARM_CFSR_UNDEFINSTR     (1 << 16)
#define ARM_CFSR_BFARVALID      (1 << 15)
#define ARM_CFSR_LSPERR         (1 << 13)
#define ARM_CFSR_STKERR         (1 << 12)
#define ARM_CFSR_UNSTKERR       (1 << 11)
#define ARM_CFSR_IMPRECISERR    (1 << 10)
#define ARM_CFSR_PRECISERR      (1 << 9)
#define ARM_CFSR_IBUSERR        (1 << 8)
#define ARM_CFSR_MMARVALID      (1 << 7)
#define ARM_CFSR_MLSPERR        (1 << 5)
#define ARM_CFSR_MSTKERR        (1 << 4)
#define ARM_CFSR_MUNSTKERR      (1 << 3)
#define ARM_CFSR_DACCVIOL       (1 << 1)
#define ARM_CFSR_IACCVIOL       (1 << 0)

/*
 * Регистр SCB HFSR: состояние HardFault.
 */
/* Признак события Debug */
#define ARM_HFSR_DEBUGEVT       (1 << 31)
/* Признак эскалации исключения до HardFault */
#define ARM_HFSR_FORCED         (1 << 30)
/* Признак ошибки чтения таблицы векторов исключений */
#define ARM_HFSR_VECTTBL        (1 << 1)

/*
 * Регистры SCB SHPRi: приоритет системных обработчиков.
 */
#define ARM_SHPR1_UFAULT(n)     ((n) << 16) /* usage fault */
#define ARM_SHPR1_BFAULT(n)     ((n) << 8)  /* bus fault */
#define ARM_SHPR1_MMFAULT(n)    ((n) << 0)  /* memory management fault */
#define ARM_SHPR2_SVCALL(n)     ((n) << 24) /* SVCall */
#define ARM_SHPR3_SYSTICK(n)    ((n) << 24) /* SysTick */
#define ARM_SHPR3_PENDSV(n)     ((n) << 16) /* PendSV */

