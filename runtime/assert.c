#include <runtime/lib.h>

/*
 * Called from assert() macro.
 * Print debugging information and halt the system.
 */
#ifdef NDEBUG
void
__assert_fail_ndebug ()
{
	debug_printf ("\nAssertion failed at address %p\n\n",
		__builtin_return_address (0));
	uos_halt (1);
}
#else
void
__assert_fail (const char *cond, const char *file, unsigned int line,
	const char *func)
{
	debug_printf ("\n `%S' %u: :\n%S, %S\n\n",
		func, line,file, cond);
	uos_halt (1);
}
#endif
