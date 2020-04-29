/*
 * Debug console input/output for Cortex-M3 architecture.
 *
 * Copyright (C) 2010 Serge Vakulenko, <serge@vak.ru>
 *               2013 Dmitry Podkhvatilin <vatilin@gmail.com>
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
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <kernel/internal.h>

bool_t debug_onlcr = 1;

static int debug_char = -1;

static void (*hook) (void *arg, short c);
static void *hook_arg;

/*
 * Redirect debug output.
 */
void
debug_redirect (void (*func) (void*, short), void *arg)
{
	hook = func;
	hook_arg = arg;
}

#if defined (ARM_1986BE1) || defined (ARM_1986BE9)
/*
 * Send a byte to the UART transmitter, with interrupts disabled.
 */
void
debug_putchar (void *arg, short c)
{
	arch_state_t x;

	arm_intr_disable (&x);

	if (hook) {
		hook (hook_arg, c);
		arm_intr_restore (x);
		return;
	}
#ifdef ARM_UART1_DEBUG
	/* Wait for transmitter holding register empty. */
	while (ARM_UART1->FR & ARM_UART_FR_TXFF)
		continue;
#else
	/* Wait for transmitter holding register empty. */
	while (ARM_UART2->FR & ARM_UART_FR_TXFF)
		continue;
#endif
again:
	/* Send byte. */
	/* TODO: unicode to utf8 conversion. */
#ifdef ARM_UART1_DEBUG
	ARM_UART1->DR = c;

/* Wait for transmitter holding register empty. */
	while (ARM_UART1->FR & ARM_UART_FR_TXFF)
		continue;
#else
	ARM_UART2->DR = c;

/* Wait for transmitter holding register empty. */
	while (ARM_UART2->FR & ARM_UART_FR_TXFF)
		continue;
#endif

	watchdog_alive ();
	if (debug_onlcr && c == '\n') {
		c = '\r';
		goto again;
	}
	arm_intr_restore (x);
}

/*
 * Wait for the byte to be received and return it.
 */
unsigned short
debug_getchar (void)
{
	unsigned c;
	arch_state_t x;

	if (debug_char >= 0) {
		c = debug_char;
		debug_char = -1;
/*debug_printf ("getchar -> 0x%02x\n", c);*/
		return c;
	}
	arm_intr_disable (&x);

	/* Enable receiver. */
#ifdef ARM_UART1_DEBUG
	ARM_UART1->CTL |= ARM_UART_CTL_RXE;

	/* Wait until receive data available. */
	while (ARM_UART1->FR & ARM_UART_FR_RXFE) {
		watchdog_alive ();
		arm_intr_restore (x);
		arm_intr_disable (&x);
	}
#else
	ARM_UART2->CTL |= ARM_UART_CTL_RXE;

		/* Wait until receive data available. */
	while (ARM_UART2->FR & ARM_UART_FR_RXFE) {
		watchdog_alive ();
		arm_intr_restore (x);
		arm_intr_disable (&x);
	}
#endif
	/* TODO: utf8 to unicode conversion. */
#ifdef ARM_UART1_DEBUG
	c = ARM_UART1->DR & ARM_UART_DR_DATA;
#else
	c = ARM_UART2->DR & ARM_UART_DR_DATA;
#endif

	arm_intr_restore (x);
	return c;
}

/*
 * Get the received byte without waiting.
 */
int
debug_peekchar (void)
{
	unsigned char c = 0;
	arch_state_t x;

	if (debug_char >= 0)
		return debug_char;

	arm_intr_disable (&x);
    
	if (hook) {
		hook (hook_arg, c);
		arm_intr_restore (x);
		return 0;
	}

#ifdef ARM_UART1_DEBUG
	/* Enable receiver. */
	ARM_UART1->CTL |= ARM_UART_CTL_RXE;

	/* Check if receive data available. */
	if (ARM_UART1->FR & ARM_UART_FR_RXFE) {
		arm_intr_restore (x);
		return -1;
	}
	/* TODO: utf8 to unicode conversion. */
	c = ARM_UART1->DR & ARM_UART_DR_DATA;
#else
	/* Enable receiver. */
	ARM_UART2->CTL |= ARM_UART_CTL_RXE;

	/* Wait until receive data available. */
	if (ARM_UART2->FR & ARM_UART_FR_RXFE) {
		arm_intr_restore (x);
		return -1;
	}
	/* TODO: utf8 to unicode conversion. */
	c = ARM_UART2->DR & ARM_UART_DR_DATA;
#endif
	arm_intr_restore (x);
	debug_char = c;
	return c;
}
#endif /* ARM_1986BE1 || ARM_1986BE9 */


#if defined(ARM_STM32F4) || defined(ARM_STM32L151RC) || defined(ARM_STM32L152RC)

#if defined(USE_USART1)
#	define DBG_USART	USART1
#else
#	define DBG_USART	USART3
#endif

void
debug_putchar (void *arg, short c)
{
    arch_state_t x;

	arm_intr_disable (&x);

    /* Wait for transmitter holding register empty. */ 
    while (! (DBG_USART->SR & USART_TC));

again:
	/* Send byte. */
	DBG_USART->DR = c;
    /* Wait for transmitter holding register empty. */
	while (! (DBG_USART->SR & USART_TC));

	watchdog_alive ();
	if (debug_onlcr && c == '\n') {
		c = '\r';
		goto again;
	}
	arm_intr_restore (x);
}

unsigned short
debug_getchar (void)
{
	unsigned c;
	arch_state_t x;

	if (debug_char >= 0) {
		c = debug_char;
		debug_char = -1;
		return c;
	}
    
	arm_intr_disable (&x);

	/* Wait until receive data available. */
	while (! (DBG_USART->SR & USART_RXNE)) {
		watchdog_alive ();
		arm_intr_restore (x);
		arm_intr_disable (&x);
	}

	c = DBG_USART->DR;

	arm_intr_restore (x);
	return c;

}

int
debug_peekchar (void)
{
	unsigned char c;
	arch_state_t x;

	if (debug_char >= 0)
		return debug_char;

	arm_intr_disable (&x);

	/* Check if receive data available. */
	if (! (DBG_USART->SR & USART_RXNE)) {
		arm_intr_restore (x);
		return -1;
	}

	c = DBG_USART->DR;

	arm_intr_restore (x);
	debug_char = c;
	return c;
}
#endif /* ARM_STM32F4 || ARM_STM32L151RC || ARM_STM32L152RC */


void
debug_puts (const char *p)
{
	arch_state_t x;

	arm_intr_disable (&x);
	for (; *p; ++p)
		debug_putchar (0, *p);
	arm_intr_restore (x);
}
