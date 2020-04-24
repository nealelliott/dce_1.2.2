/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: logwtmp.c,v $
 * Revision 1.1.2.1  1996/06/05  21:26:30  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:15:28  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * pty_logwtmp: Implement the logwtmp function if not present.
 *
 * Copyright 1995 by the Massachusetts Institute of Technology.
 *
 * 
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby
 * granted, provided that the above copyright notice appear in all
 * copies and that both that copyright notice and this permission
 * notice appear in supporting documentation, and that the name of
 * M.I.T. not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability
 * of this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 * 
 */

#include <com_err.h>
#include "libpty.h"
#include "pty-int.h"

long pty_logwtmp (tty, user, host )
    char *user, *tty, *host;
{
#ifdef HAVE_LOGWTMP
    logwtmp(tty,user,host);
    return 0;
#else
        struct utmp ut;
    char *tmpx;
    char utmp_id[5];

    /* Will be empty for logout */
    int loggingin = user[0];


#ifndef NO_UT_HOST
    strncpy(ut.ut_host, host, sizeof(ut.ut_host));
#endif

    strncpy(ut.ut_line, tty, sizeof(ut.ut_line));
    ut.ut_time = time(0);
    
#ifndef NO_UT_PID
    ut.ut_pid = getpid();
    strncpy(ut.ut_user, user, sizeof(ut.ut_user));

    tmpx = tty + strlen(tty) - 2;
    sprintf(utmp_id, "kr%s", tmpx);
    strncpy(ut.ut_id, utmp_id, sizeof(ut.ut_id));
    ut.ut_pid = (loggingin ? getpid() : 0);
    ut.ut_type = (loggingin ? USER_PROCESS : DEAD_PROCESS);
#else
    strncpy(ut.ut_name, user, sizeof(ut.ut_name));
#endif

    return ptyint_update_wtmp(&ut, host, user);
#endif /*HAVE_LOGWTMP*/
}

