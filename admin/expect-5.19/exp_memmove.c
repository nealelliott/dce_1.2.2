/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: exp_memmove.c,v $
 * Revision 1.1.2.1  1996/08/09  19:11:11  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:55:52  arvind]
 *
 * $EndLog$
 */

/* memmove - some systems lack this */

#include <expect_cf.h>
#include <tcl.h>

/* like memcpy but can handle overlap */
#ifndef HAVE_MEMMOVE
char *
memmove(dest,src,n)
VOID *dest;
CONST VOID *src;
int n;
{
	char *d;
	CONST char *s;

	d = dest;
	s = src;
	if (s<d && (d < s+n)) {
		for (d+=n, s+=n; 0<n; --n)
			*--d = *--s;
	} else for (;0<n;--n) *d++ = *s++;
	return dest;
}
#endif /* HAVE_MEMMOVE */
