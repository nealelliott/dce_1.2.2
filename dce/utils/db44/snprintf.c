/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: snprintf.c,v $
 * Revision 1.1.4.2  1996/02/18  19:21:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:36:42  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/11/09  20:42 UTC  psn
 * 	Include pthread.h for use wrappers.
 * 	[1995/12/08  18:08:31  root]
 * 
 * Revision 1.1.2.1  1993/12/01  22:30:39  bowe
 * 	Initial checkin.
 * 	[1993/12/01  16:55:15  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <sys/types.h>
#include <cdefs.h>

#include <compat.h>

#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <pthread.h>

int
#ifdef __STDC__
snprintf(char *str, size_t n, const char *fmt, ...)
#else
snprintf(str, n, fmt, va_alist)
	char *str;
	size_t n;
	const char *fmt;
	va_dcl
#endif
{
	va_list ap;
	char *rp;
	int rval;
#ifdef __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
#ifdef VSPRINTF_CHARSTAR
	rp = vsprintf(str, fmt, ap);
	va_end(ap);
	return (strlen(rp));
#else
	rval = vsprintf(str, fmt, ap);
	va_end(ap);
	return (rval);
#endif
}

int
vsnprintf(str, n, fmt, ap)
	char *str;
	size_t n;
	const char *fmt;
	va_list ap;
{
#ifdef VSPRINTF_CHARSTAR
	return (strlen(vsprintf(str, fmt, ap)));
#else
	return (vsprintf(str, fmt, ap));
#endif
}
