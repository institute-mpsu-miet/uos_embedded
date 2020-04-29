#ifdef ARM_S3C4530
#	include <runtime/arm/io-s3c4530.h>
#endif
#ifdef ARM_NET50
#	include <runtime/arm/io-net50.h>
#endif
#ifdef ARM_AT91SAM
#	include <runtime/arm/io-at91sam.h>
#endif
#ifdef ARM_OMAP44XX
#	include <runtime/arm/io-omap44xx.h>
#endif

/*
 * Handle stack pointer register.
 */
static inline __attribute__ ((always_inline))
void arm_set_stack_pointer (void *x)
{
	asm volatile (
	"mov	sp, %0"
	: : "r" (x));
}

static inline __attribute__ ((always_inline))
void *arm_get_stack_pointer ()
{
	void *x;

	asm volatile (
	"mov	%0, sp"
	: "=r" (x));
	return x;
}

#define set_stack_pointer(x) arm_set_stack_pointer(x)
#define get_stack_pointer(x) arm_get_stack_pointer(x)

/*
 * Handle any register R0-R14.
 */
static inline __attribute__ ((always_inline))
void arm_set_register (int reg, unsigned val)
{
	asm volatile (
	"mov	r%c1, %0"
	: : "r" (val), "i" (reg));
}

static inline __attribute__ ((always_inline))
unsigned arm_get_register (int reg)
{
	unsigned val;

	asm volatile (
	"mov	%0, r%c1"
	: "=r" (val) : "i" (reg));
	return val;
}

/*
 * Disable and restore the hardware interrupts,
 * saving the interrupt enable flag into the supplied variable.
 */
static void inline __attribute__ ((always_inline))
arm_intr_disable (int *x)
{
	int temp;

#if __thumb2__
	asm volatile (
	"mrs	%1, basepri \n"		/* Cortex-M3 mode */
"	mov	%0, #16 \n"		/* basepri := 16 */
"	msr	basepri, %0"
	: "=r" (temp), "=r" (*(x)) : : "memory", "cc");
#else
	asm volatile (
#if __thumb__
	".balignw 4, 0x46c0 \n"
"	bx	pc \n"			/* switch to ARM mode */
"	nop \n"
"	.code	32 \n"
#endif

"	mrs	%1, cpsr \n"		/* ARM mode  */
"	orr	%0, %1, #0x80 \n"
"	msr	cpsr, %0 \n"

#if defined (ARM_CLI_PARANOIA)
"	msr	cpsr, %0 \n"
#endif

#if __thumb__
"	ldr	%0, [pc] \n"		/* switch back to Thumb mode */
"	bx	%0 \n"
"	.word	.+5 \n"
"	.code	16"
#endif
	: "=r" (temp), "=r" (*(x)) : : "memory", "cc");
#endif /* __thumb2__ */
}

static void inline __attribute__ ((always_inline))
arm_intr_restore (int x)
{
#if __thumb2__
	asm volatile (
	"msr	basepri, %0"		/* Cortex-M3 mode */
	: : "r" (x) : "memory", "cc");
#else
	int temp;

	asm volatile (
#if __thumb__
	".balignw 4, 0x46c0 \n"
"	bx	pc \n"			/* switch to ARM mode */
"	nop \n"
"	.code	32 \n\t"
#endif
	"msr	cpsr, %1"
#if __thumb__
"\n	ldr	%0, [pc] \n"		/* switch back to Thumb mode */
"	bx	%0 \n"
"	.word	.+5 \n"
"	.code	16"
#endif
	: "=r" (temp) : "r" (x) : "memory", "cc");
#endif /* __thumb2__ */
}

static void inline __attribute__ ((always_inline))
arm_intr_enable ()
{
#if __thumb2__
	asm volatile (
	"msr	basepri, %0"		/* Cortex-M3 mode */
	: : "r" (0) : "memory", "cc");
#else
	int temp;

	asm volatile (
#if __thumb__
	".balignw 4, 0x46c0 \n"
"	bx	pc \n"			/* switch to ARM mode */
"	nop \n"
"	.code	32 \n\t"
#endif
	"mrs	%0, cpsr \n"
"	bic	%0, %0, #0x80 \n"
"	msr	cpsr, %0"
#if __thumb__
"\n	ldr	%0, [pc] \n"		/* switch back to Thumb mode */
"	bx	%0 \n"
"	.word	.+5 \n"
"	.code	16"
#endif
	: "=r" (temp) : : "memory", "cc");
#endif /* __thumb2__ */
}

static void inline __attribute__ ((always_inline))
arm_bus_yield ()
{
	unsigned tmp1, tmp2;

	asm volatile (
#if __thumb2__
	"wfi"
#elif __thumb__
	"mul %1, %0 \n\t"
"	mul %1, %0 \n\t"
"	mul %1, %0"
#else
	"mul %1, %0, %0 \n\t"
"	mul %1, %0, %0 \n\t"
"	mul %1, %0, %0"
#endif
	: "=r" (tmp1), "=r" (tmp2) : : "cc");
}

/*
 * Read IPSR register.
 */
static inline __attribute__ ((always_inline))
unsigned arm_get_ipsr ()
{
	unsigned x;

	asm volatile (
	"mrs	%0, ipsr"
	: "=r" (x));
	return x;
}

/*
 * Count a number of leading (most significant) zero bits in a word.
 */
static int inline __attribute__ ((always_inline))
arm_count_leading_zeroes (unsigned x)
{
	int n;

	asm volatile (
	"clz	%0, %1"
	: "=r" (n) : "r" (x));
	return n;
}
