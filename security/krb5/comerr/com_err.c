/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: com_err.c,v $
 * Revision 1.1.7.2  1996/02/18  00:08:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:41  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:39:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:30  root]
 * 
 * Revision 1.1.5.1  1993/10/14  17:45:10  mccann
 * 	CR8651 64bit port
 * 	[1993/10/14  17:42:18  mccann]
 * 
 * Revision 1.1.3.3  1992/12/29  13:55:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:50:24  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:18:10  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:21:58  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:53:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  com_err.c V=2 10/23/91 //littl/prgy/krb5/comerr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * Copyright 1987, 1988 by MIT Student Information Processing Board.
 *
 * For copyright info, see mit-sipb-cr.h.
 */

#include <stdio.h>

#include "mit-sipb-copyright.h"

#if 0
/*
 * Our environment only provides for ANSI's <stdarg.h> when using GNU
 * C.  Grump grump...
 */
#if ! __GNUC__
#define VARARGS 1
#endif

/* We don't have the v*printf routines... */
#define vfprintf(stream,fmt,args) _doprnt(fmt,args,stream)
#endif
#if (__STDC__ || defined(__STDC_ZERO_OK__)) && !VARARGS
#	include <stdarg.h>
#else /* varargs: not STDC or no <stdarg> */
	/* Non-ANSI, always take <varargs.h> path. */
#	undef VARARGS
#	define VARARGS 1
#	include <varargs.h>
#	undef vfprintf
#	define vfprintf(stream,fmt,args) _doprnt(fmt,args,stream)
#endif /* varargs */

#include "error_table.h"
#include "internal.h"

/*
 * Protect us from header version (externally visible) of com_err, so
 * we can survive in a <varargs.h> environment.  I think.
 */
#define com_err com_err_external
/* #include "com_err.h" */
#undef com_err

#if 0
/* BSD. sigh. */
#undef vfprintf
#define vfprintf(stream,fmt,args) _doprnt(fmt,args,stream)
#endif

#if ! lint
static const char rcsid[] =
    "$Header: /u0/rcs_trees/dce/rcs/security/krb5/comerr/com_err.c,v 1.1.7.2 1996/02/18 00:08:26 marty Exp $";
#endif	/* ! lint */

static void
#ifdef __STDC__
    default_com_err_proc (const char *whoami, long code, const char *fmt, va_list args)
#else
    default_com_err_proc (whoami, code, fmt, args)
    const char *whoami;
    long code;
    const char *fmt;
    va_list args;
#endif
{
    if (whoami) {
	fputs(whoami, stderr);
	fputs(": ", stderr);
    }
    if (code) {
	fputs((char *)error_message(code), stderr);
	fputs(" ", stderr);
    }
    if (fmt) {
        vfprintf (stderr, fmt, args);
    }
    putc('\n', stderr);
    /* should do this only on a tty in raw mode */
    putc('\r', stderr);
    fflush(stderr);
}

#ifdef __STDC__
typedef void (*errf) (const char *, long, const char *, va_list);
#else
typedef void (*errf) ();
#endif

errf com_err_hook = default_com_err_proc;

void com_err_va (whoami, code, fmt, args)
    const char *whoami;
    long code;
    const char *fmt;
    va_list args;
{
    (*com_err_hook) (whoami, code, fmt, args);
}

#if ! VARARGS
void com_err (const char *whoami,
	      long code,
	      const char *fmt, ...)
{
#else
void com_err (va_alist)
    va_dcl
{
    const char *whoami, *fmt;
    long code;
#endif
    va_list pvar;

    if (!com_err_hook)
	com_err_hook = default_com_err_proc;
#if VARARGS
    va_start (pvar);
    whoami = va_arg (pvar, const char *);
    code = va_arg (pvar, long);
    fmt = va_arg (pvar, const char *);
#else
    va_start(pvar, fmt);
#endif
    com_err_va (whoami, code, fmt, pvar);
    va_end(pvar);
}

errf set_com_err_hook (new_proc)
    errf new_proc;
{
    errf x = com_err_hook;
    if (new_proc) com_err_hook = new_proc;
    else com_err_hook = default_com_err_proc;
    return x;
}

errf reset_com_err_hook () {
    errf x = com_err_hook;
    com_err_hook = default_com_err_proc;
    return x;
}
