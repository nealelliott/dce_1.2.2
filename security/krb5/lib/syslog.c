/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: syslog.c,v $
 * Revision 1.1.12.1  1996/10/03  15:07:12  arvind
 * 	Silence some type mismatch warnings.
 * 	[1996/09/16  22:20 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.10.2  1996/02/18  00:16:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:30  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  17:45:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:04  root]
 * 
 * Revision 1.1.7.3  1994/06/30  19:37:13  ohara
 * 	always use stdarg.h
 * 	[1994/06/07  22:46:45  ohara]
 * 
 * Revision 1.1.7.2  1994/06/02  21:29:08  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:17:05  mdf]
 * 
 * Revision 1.1.7.1  1994/04/01  20:08:21  jd
 * 	syslog.c depended on _CMA_PROTO_ being set. It no longer exists so set
 * 	it here as a temporary work around untill security cleanup.
 * 	[1994/03/28  19:41:14  jd]
 * 
 * Revision 1.1.5.3  1992/12/29  16:33:11  zeliff
 * 	     Embedding copyright notice
 * 	     [1992/12/28  21:21:04  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  21:20:00  devsrc
 * 	     [OT 5373]    SNI/SVR4 merge.
 * 	     [1992/09/11  21:35:50  sekhar]
 * 
 * Revision 1.1.2.2  1992/06/30  21:32:33  burati
 * 	     CR4516 HP/UX porting changes
 * 	     [1992/06/30  21:25:23  burati]
 * 
 * Revision 1.1  1992/01/19  14:53:45  devrcs
 * 	     Initial revision
 * 
 * $EndLog$
 */

/*  syslog.c V=5 11/25/91 //littl/prgy/krb5/lib
**
** Copyright (c) Hewlett-Packard Company 1991,1994
** Unpublished work. All Rights Reserved.
**
*/
/* system logger (post-BSD4.x); includes vsyslog
 *
 * Copyright Hewlett-Packard Company 1991
 *  Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 *
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)syslog.c    5.20 (Berkeley) 1/19/89";
#endif /* LIBC_SCCS and not lint */

/*
 * VSYSLOG - a varargs equivalent of syslog.  This function takes a 
 *           va_list instead of a variable number of arguments.  
 *           vsyslog() is intended for use in building varargs-based 
 *           logging/error reporting routines which route their output 
 *           through syslogd.
 */

#include <stdarg.h>

#ifdef __hpux
#include <syslog.h>
#else
#include <sys/syslog.h>
#endif

#include <string.h>
#include <stdio.h>

#ifdef OSF_DCE
#   include <krb5/osf_dce.h>
#endif

#define FMTSTRLEN 1023   
#define LOGSTRLEN 2047
extern int errno;

int vsyslog
(
    int pri, 
    register char const *fmt, 
    va_list ap
)

{
    int saved_errno;
    char fmt_cpy[FMTSTRLEN+1], 
         log_str[LOGSTRLEN+1], 
         last = '\0';
    register char const *p = fmt; 
    register char *q = fmt_cpy;
    register char *t;

    saved_errno = errno;

    /* Walk through format string */
    while (*p)
    {
        /* if %m in format string */ 
        if (*p == 'm' && last == '%')
        {
            /* substitute error message for %m */
            t = strerror(saved_errno);
            strcpy(q-1, t);
            q += strlen(t) + 1;
            last = *p;
            ++p;
        }
        /* allow for %%m escapes... */
        if( last == '%' && *p == '%' )
        {
            last = '\0';
        }
	else last = *p;
        *q++ = *p++;
    } 
    *q = '\0';
            
    vsprintf(log_str, fmt_cpy, ap); 

    syslog(pri, "%s", log_str);

    return(0);
}
