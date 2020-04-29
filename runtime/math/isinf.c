/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Changed to return -1 for -Inf by Ulrich Drepper <drepper@cygnus.com>.
 * Public domain.
 */
#include <runtime/lib.h>
#include <runtime/math.h>
#include <runtime/math/private.h>

/*
 * isinf(x) returns 1 is x is inf, -1 if x is -inf, else 0;
 * no branching!
 */
int isinf (double x)
{
	long hx, lx;

	EXTRACT_WORDS (hx, lx, x);
	lx |= (hx & 0x7fffffff) ^ 0x7ff00000;
	lx |= -lx;
	return ~(lx >> 31) & (hx >> 30);
}
