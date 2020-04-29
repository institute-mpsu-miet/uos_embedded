#include <runtime/lib.h>
#include <stream/stream.h>

void debug_dump (const char *caption, void* data, unsigned len)
{
	uint8_t *t, *h, i;
	unsigned offset;

	debug_printf ("%S: %u bytes at %p\n", caption, len, data);
	for (offset = 0; offset < len; ) {
		unsigned lot = len - offset;
		if (lot > 16)
			lot = 16;

		debug_printf ("[+%04X]:", offset);
		h = data; h += offset; t = h; offset += lot;

		i = 17 * 3;
		do {
			if (uos_valid_memory_address (h))
				debug_printf (" %02X", *h);
			else
				debug_puts (" __");
			i -= 3;
			if (i == 9 * 3) {
				debug_putc (' ');
				--i;
			}
			++h;
		} while (--lot);

		do
			debug_putc (' ');
		while (--i);

		do {
			unsigned char c = ' ';
			if (uos_valid_memory_address (t)) {
				c = *t;
				if (c < ' ' || c > 127)
					c = '.';
			}
			debug_putc (c);
		} while (++t != h);
		debug_putc ('\n');
	}
}
