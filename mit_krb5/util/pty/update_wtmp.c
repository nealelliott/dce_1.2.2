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
 * $Log: update_wtmp.c,v $
 * Revision 1.1.2.1  1996/06/05  21:27:37  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:16:16  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * ptyint_update_utmp: Update or create a utmp entry
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

#if !defined(WTMP_FILE) && defined(_PATH_WTMP)
#define WTMP_FILE _PATH_WTMP
#endif


/* if it is *still* missing, assume SunOS */
#ifndef WTMP_FILE
#define	WTMP_FILE	"/usr/adm/wtmp"
#endif


long ptyint_update_wtmp (ent , host, user)
    struct utmp *ent;
     char *host;
char *user;
    {
    struct utmp ut;
    struct stat statb;
    int fd;
#ifdef HAVE_SETUTXENT
    struct utmpx utx;

    getutmpx(ent, &utx);
if (host)
    strncpy(utx.ut_host, host, sizeof(utx.ut_host) );
else utx.ut_host[0] = 0;
if (user)
    strncpy(utx.ut_user, user, sizeof(utx.ut_user));
    updwtmpx(WTMPX_FILE, &utx);
#endif

#ifdef HAVE_UPDWTMP
#ifndef HAVE_SETUTXENT
/* This is already performed byupdwtmpx if present.*/
    updwtmp(WTMP_FILE, ent);
#endif /* HAVE_SETUTXENT*/
#else /* HAVE_UPDWTMP */

    if ((fd = open(WTMP_FILE, O_WRONLY|O_APPEND, 0)) >= 0) {
	if (!fstat(fd, &statb)) {
	  (void)memset((char *)&ut, 0, sizeof(ut));
	  (void)strncpy(ut.ut_line, ent->ut_line, sizeof(ut.ut_line));
	  (void)strncpy(ut.ut_name, ent->ut_name, sizeof(ut.ut_name));
#ifndef NO_UT_HOST
	  (void)strncpy(ut.ut_host, ent->ut_host, sizeof(ut.ut_host));
#endif
	  (void)time(&ut.ut_time);
#if defined(HAVE_GETUTENT) && defined(USER_PROCESS)
	  if (ent->ut_name) {
	    if (!ut.ut_pid)
	      ut.ut_pid = getpid();
	    ut.ut_type = USER_PROCESS;
	  } else {
#ifdef EMPTY
	    ut.ut_type = EMPTY;
#else
	    ut.ut_type = DEAD_PROCESS; /* For Linux brokenness*/
#endif	    

	  }
#endif
	    if (write(fd, (char *)&ut, sizeof(struct utmp)) !=
		sizeof(struct utmp))
	      (void)ftruncate(fd, statb.st_size);
	}
	(void)close(fd);
    }
#endif /* HAVE_UPDWTMP */
    return 0; /* no current failure cases; file not found is not failure!*/
    
}

