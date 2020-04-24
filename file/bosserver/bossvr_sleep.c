/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_sleep.c,v $
 * Revision 1.1.55.1  1996/10/02  17:04:45  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:05  damon]
 *
 * Revision 1.1.49.3  1994/07/13  22:19:58  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:40  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  19:58:38  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:54:07  mbs]
 * 
 * Revision 1.1.49.2  1994/06/09  13:52:34  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:22  annie]
 * 
 * Revision 1.1.49.2  1994/06/09  13:52:34  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:22  annie]
 * 
 * Revision 1.1.49.1  1994/02/04  20:06:32  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:47  devsrc]
 * 
 * Revision 1.1.47.1  1993/12/07  17:13:10  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:58:30  jaffe]
 * 
 * Revision 1.1.3.5  1993/01/18  19:57:00  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:33:25  cjd]
 * 
 * Revision 1.1.3.4  1992/11/24  15:41:42  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:09:43  bolinger]
 * 
 * Revision 1.1.3.3  1992/10/28  21:38:44  jaffe
 * 	Fixed RCSID
 * 	[1992/10/28  21:03:38  jaffe]
 * 
 * Revision 1.1.3.2  1992/10/27  20:09:28  jaffe
 * 	Transarc delta: bab-ot5549-bossvr-make-mutex-recursive-on-osf 1.2
 * 	  Selected comments:
 * 	    When the OSF first ported the bosserver to OSF/1, they ifdef'd out the call
 * 	    that made the bosserver's global mutex recursive.  It turns out that
 * 	    this prevented the bos restart command from working on the OSF/1 platform.
 * 	    ot 5549
 * 	    Went ahead and removed the need for the bosserver mutex to
 * 	    be recursive altogether.
 * 	    Removed acquisition of bosserver mutex when calling wakeup.
 * 	[1992/10/27  13:57:11  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:49:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bossvr_sleep.c -- routines that implement a utility simulating kernel-type
 * sleeping in a CMA environment
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>


RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/bosserver/bossvr_sleep.c,v 1.1.55.1 1996/10/02 17:04:45 damon Exp $")

#include <errno.h>
#include <pthread.h>
#include <bossvr_sleep.h>

static pthread_cond_t sleep_cond[SLEEP_HASH_CONDS];

IMPORT pthread_mutex_t bosserverMutex;

static int sleep_HashAddr(a)
char *a; {
    register long tval;
    tval = (long) a;
    tval = tval>> 8;	/* get rid of low order address bits */
    tval += (tval>>16);
    tval = tval & (SLEEP_HASH_CONDS-1);
    return tval;
}

EXPORT int bossvr_InitSleepers()
{
  int	rtnVal = 0;
  int	i;
  
  for (i = 0; (i < SLEEP_HASH_CONDS) && (rtnVal == 0); i++) {
    if (pthread_cond_init(&(sleep_cond[i]), pthread_condattr_default) != 0) {
      rtnVal = errno;
    }
  }
  
  return rtnVal;
}

/* this function is allowed to return spuriously */
EXPORT void bossvr_Sleep(addr)
     register long addr;
{
    register int hashVal;

    hashVal = sleep_HashAddr(addr);
    pthread_cond_wait(&sleep_cond[hashVal], &bosserverMutex);
}

EXPORT void bossvr_Wakeup(addr)
     register long addr; 
{
    register int hashVal;

    hashVal = sleep_HashAddr(addr);
    pthread_cond_broadcast(&sleep_cond[hashVal]);
}

