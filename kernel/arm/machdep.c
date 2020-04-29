/*
 * Machine-dependent part of uOS for: ARM7, GCC.
 *
 * Copyright (C) 2000-2005 Serge Vakulenko, <vak@cronyx.ru>
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
#include "runtime/lib.h"
#include "kernel/uos.h"
#include "kernel/internal.h"

/*
 * Perform the task switch.
 */
void arch_task_switch (task_t *target)
{
	/* Save all registers in stack. */ \
	asm volatile (
#if __thumb__
	".balignw 4, 0x46c0 \n"
"	bx	pc \n"			/* switch to ARM mode */
"	nop \n"
"	.code	32 \n\t"
#endif
	"push	{lr} \n"		/* pc */
"	mrs	lr, cpsr \n"
#if __thumb__
"	orr	lr, lr, #0x20 \n"	/* thumb mode */
#endif
"	push	{lr} \n"		/* cpsr (from lr) */
"	push	{r0-r12,lr}"		/* save r0-r12,lr */
#if __thumb__
"\n	ldr	lr, [pc] \n"		/* switch back to Thumb mode */
"	bx	lr \n"
"	.word	.+5 \n"
"	.code	16"
#endif
	);

	/* Save current task stack. */
	task_current->stack_context = arm_get_stack_pointer ();

	task_current = target;

	/* Switch to the new task. */
	arm_set_stack_pointer (task_current->stack_context);

	/* Restore registers. */
	asm volatile (
#if __thumb__
	".balignw 4, 0x46c0 \n"
"	bx	pc \n"			/* switch to ARM mode */
"	nop \n"
"	.code	32 \n"
#endif
"	.globl	restore_regs \n"
"restore_regs: \n"
"	pop	{r0-r2} \n"	/* load r0-r2 */
"	ldr	lr, =_irq_stack_+12 \n"
"	stmdb	lr, {r0-r2} \n"		/* save r0-r2 to irq stack */
"	pop	{r3-r12, lr} \n"	/* load r3-r12, lr */
"	pop	{r0,r1} \n"		/* load saved cpsr,pc to r0,r1 */
"	mrs	r2, cpsr \n"		/* move cpsr to r0 */
"	bic	r2, r2, #1 \n"
"	msr	cpsr, r2 \n"		/* switch to irq mode */
"	msr	spsr, r0 \n"		/* restore spsr */
"	mov	lr, r1 \n"		/* restore lr */
"	ldmdb	sp, {r0-r2} \n"		/* load saved r0,r1 */
"	movs	pc, lr"			/* changes mode and branches */
	);
}

/*
 * The common part of the interrupt handler,
 * to minimize the code size.
 * Attribute "naked" skips function prologue.
 */
void __attribute__ ((naked))
_irq_handler_ (void)
{
	mutex_irq_t *h;
	int irq;
	
#ifdef ARM_OMAP44XX
	irq = 1023;
#endif

	for (;;) {
#ifdef ARM_OMAP44XX
		/* End of interrupt for previously acknowledged interrupt, 
		   if it is not spurious */
		if (irq != 1023)
			ARM_ICCEOIR = irq;
#endif
		/* Get the current irq number */
#ifdef ARM_S3C4530
		irq = ARM_INTOFFSET_IRQ >> 2;
#endif
#ifdef ARM_AT91SAM
		irq = *AT91C_AIC_IVR;		/* get most priority irq */
		*AT91C_AIC_EOICR = 0;		/* clear it */
#endif
#ifdef ARM_OMAP44XX
		irq = ARM_ICCIAR;		/* get irq and acknowledge it */
#endif
		if (irq >= ARCH_INTERRUPTS)
			break;

		/* Disable the irq, to avoid loops */
#ifdef ARM_S3C4530
		ARM_INTPND = 1 << irq;		/* clear pending irq */
		ARM_INTMSK |= 1 << irq;		/* disable */
#endif
#ifdef ARM_AT91SAM
		*AT91C_AIC_IDCR = 1 << irq;	/* disable */
#endif
#ifdef ARM_OMAP44XX
		ARM_ICDICER(irq >> 5) = (1 << (irq & 0x1F));	/* disable */
#endif
/*debug_printf ("<%d> ", irq);*/
		h = &mutex_irq [irq];
		if (! h->lock)
			continue;

		if (h->handler) {
			/* If the lock is free -- call fast handler. */
			if (h->lock->master) {
				/* Lock is busy -- remember pending irq.
				 * Call fast handler later, in mutex_unlock(). */
				h->pending = 1;
				continue;
			}
			if ((h->handler) (h->arg) != 0) {
				/* The fast handler returns 1 when it fully
				 * serviced an interrupt. In this case
				 * there is no need to wake up the interrupt
				 * servicing task, stopped on mutex_wait.
				 * Task switching is not performed. */
#ifdef ARM_S3C4530
				if (irq == 4 || irq == 6) {
					/* Enable UART transmit irq. */
					ARM_INTMSK &= ~(1 << irq);
				}
#endif
				continue;
			}
		}

		/* Signal the interrupt handler, if any. */
		mutex_activate (h->lock, 0);
	}

	/* LY: copy few lines of code from task_schedule() here. */
	if (task_need_schedule)	{
		task_t *new;

		task_need_schedule = 0;
		new = task_policy ();
		if (new != task_current) {
			task_current->stack_context = arm_get_stack_pointer ();
			task_current = new;
			new->ticks++;
			arm_set_stack_pointer (task_current->stack_context);
		}
	}

	/* Restore registers. */
#if __thumb__
	asm volatile (
	".globl restore_regs \n"
"	ldr	r0, =restore_regs \n"
"	bx	r0"
	);
#else
	asm volatile (
	".globl	restore_regs \n"
"	b	restore_regs"
	);
#endif
}

/*
 * Allow the given hardware interrupt,
 * unmasking it in the interrupt controller.
 */
void arch_intr_allow (int irq)
{
#ifdef ARM_S3C4530
	if (irq == 4 || irq == 6) {
		/* Do not enable UART transmit interrupt here. */
		return;
	}
	ARM_INTMSK &= ~(1 << irq);
#endif
#ifdef ARM_AT91SAM
	*AT91C_AIC_IECR = 1 << irq;
/*debug_printf ("<IECR:=%x> ", 1 << irq);*/
#endif
#ifdef ARM_OMAP44XX
	ARM_ICDISER(irq >> 5) = (1 << (irq & 0x1F));
#endif
}

/*
 * Build the initial task's stack frame.
 * Arguments:
 * t	- the task object
 * func	- the task function to call
 * arg	- the function argument
 * sp	- the pointer to (end of) stack space
 */
void
arch_build_stack_frame (task_t *t, void (*func) (void*), void *arg,
	unsigned stacksz)
{
	unsigned *sp = (unsigned*) ((char*) t + stacksz);

	*--sp = (unsigned) func;	/* pc - callee address */
#if __thumb__
	*--sp = 0x73;			/* cpsr - thumb, enable interrupts, svc mode */
#else
	*--sp = 0x53;			/* cpsr - enable interrupts, svc mode */
#endif
	*--sp = 0;			/* lr */
	*--sp = 0;			/* r12 */
	*--sp = 0;			/* r11 */
	*--sp = 0;			/* r10 */
	*--sp = 0;			/* r9 */
	*--sp = 0;			/* r8 */
	*--sp = 0;			/* r7 */
	*--sp = 0;			/* r6 */
	*--sp = 0;			/* r5 */
	*--sp = 0;			/* r4 */
	*--sp = 0;			/* r3 */
	*--sp = 0;			/* r2 */
	*--sp = 0;			/* r1 */
	*--sp = (unsigned) arg;		/* r0 - task argument */

	t->stack_context = (void*) sp;
}
