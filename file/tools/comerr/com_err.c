/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: com_err.c,v $
 * Revision 1.1.750.1  1996/10/02  18:48:14  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:06  damon]
 *
 * Revision 1.1.745.2  1994/07/13  22:30:01  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:55:56  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:22  mbs]
 * 
 * Revision 1.1.745.1  1994/06/09  14:22:59  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:34:34  annie]
 * 
 * Revision 1.1.6.2  1993/01/21  15:55:39  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:39:02  cjd]
 * 
 * Revision 1.1.2.3  1992/04/21  16:53:50  mason
 * 	Gratutiously, bci stripped my log message.  Misc. changes.
 * 	[1992/04/20  23:21:52  mason]
 * 
 * Revision 1.1.2.2  1992/01/24  04:13:10  devsrc
 * 	Fixed logs
 * 	[1992/01/23  22:23:11  devsrc]
 * 
 * $EndLog$
*/
/*
 * Copyright 1987, 1988 by MIT Student Information Processing Board.
 *
 * For copyright info, see mit-sipb-cr.h.
 */

#include <stdio.h>
#include <mit-sipb-cr.h>

/*
 * Our environment only provides for ANSI's <stdarg.h> when using GNU
 * C.  Grump grump...
 */
#if ! __GNUC__
#define VARARGS 1
#endif

/* We don't have the v*printf routines... */
#define vfprintf(stream,fmt,args) _doprnt(fmt,args,stream)

#if __STDC__ && !VARARGS
#	include <stdarg.h>
#else /* varargs: not STDC or no <stdarg> */
	/* Non-ANSI, always take <varargs.h> path. */
#	undef VARARGS
#	define VARARGS 1
#	include <varargs.h>
#	undef vfprintf
#	define vfprintf(stream,fmt,args) _doprnt(fmt,args,stream)
#endif /* varargs */

#include <error_table.h>
#include <internal.h>

/*
 * Protect us from header version (externally visible) of com_err, so
 * we can survive in a <varargs.h> environment.  I think.
 */
#define com_err com_err_external
/* #include "com_err.h" */
#undef com_err

/* BSD. sigh. */
#undef vfprintf
#define vfprintf(stream,fmt,args) _doprnt(fmt,args,stream)

#if ! lint
static const char rcsid[] =
    "$Header: /u0/rcs_trees/dce/rcs/file/tools/comerr/com_err.c,v 1.1.750.1 1996/10/02 18:48:14 damon Exp $";
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
