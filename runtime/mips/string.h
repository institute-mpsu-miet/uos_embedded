/*
 * String inline routines for MIPS architecture.
 * Do not include this file directly, use runtime/lib.h instead.
 *
 * Copyright (C) 2008-2010 Serge Vakulenko, <serge@vak.ru>
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

/* Copy N bytes of SRC to DEST.  */
extern void *
memcpy (void *to, const void *from, size_t n);

/* Copy no more than N bytes of SRC to DEST, stopping when C is found.
   Return the position in DEST one byte past where C was copied,
   or NULL if C was not found in the first N bytes of SRC.  */

/* Set N bytes of S to C.  */

extern void *
memset (void *s, unsigned char c, size_t n);

/* Compare N bytes of S1 and S2.  */
extern small_int_t
memcmp (const void *s1, const void *s2, size_t n);

/* Copy N bytes of SRC to DEST, guaranteeing
   correct behavior for overlapping strings.  */
extern void *
memmove (void *dest, const void *src, size_t n);

/* Search N bytes of S for C.  */
extern void *
memchr (const void *s, unsigned char c, size_t n);

/* Return the length of S.  */
extern size_t
strlen (const unsigned char *s);

/* Copy SRC to DEST.  */
extern unsigned char *
strcpy (unsigned char *dest, const unsigned char *src);

/* Copy no more than N characters of SRC to DEST.  */
extern unsigned char *
strncpy (unsigned char *dest, const unsigned char *src, size_t n);

/* Append SRC onto DEST.  */
extern unsigned char *
strcat (unsigned char *dest, const unsigned char *src);

/* Append no more than N characters from SRC onto DEST.  */
extern unsigned char *
strncat (unsigned char *dest, const unsigned char *src, size_t n);

/* Compare S1 and S2.  */
extern small_int_t
strcmp (const unsigned char *s1, const unsigned char *s2);

/* Compare N characters of S1 and S2.  */
extern small_int_t
strncmp (const unsigned char *s1, const unsigned char *s2, size_t n);

/* Find the first occurrence of C in S.  */
extern unsigned char *
strchr (const unsigned char *s, unsigned char c);

/* Find the last occurrence of C in S.  */
extern unsigned char *
strrchr (const unsigned char *src, unsigned char c);

/* Find the length of STRING, but scan at most MAXLEN characters.
   If no '\0' terminator is found in that many characters, return MAXLEN.  */
extern size_t
strnlen (const unsigned char *string, size_t maxlen);
