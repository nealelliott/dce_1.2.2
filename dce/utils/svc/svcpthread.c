/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Interface to threads.  System-dependant.
 */

/*
 * HISTORY
 * $Log: svcpthread.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:28  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:38:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:10:12  root]
 * 
 * Revision 1.1.4.3  1994/08/28  00:51:08  rsalz
 * 	Add "SVC_BRIEF" env var support (OT CR 11512).
 * 	[1994/08/27  12:37:45  rsalz]
 * 
 * Revision 1.1.4.2  1994/06/09  16:06:32  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:02  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/29  18:11:22  rsalz
 * 	Check pthread_xxx return values where possible (OT CR 8883)
 * 	[1994/03/29  15:20:27  rsalz]
 * 
 * Revision 1.1.2.3  1993/12/01  21:18:27  rsalz
 * 	Fix pthread_once function to NOT set lock
 * 	[1993/12/01  21:18:04  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:10:17  rsalz
 * 	Initial release
 * 	[1993/08/16  18:04:53  rsalz]
 * 
 * $EndLog$
 */

#include <svcimpl.h>


/*
**  Initialization mutex.
*/
boolean			dce_svc_g_setup;
pthread_mutex_t		dce_svc_g_mutex;
pthread_once_t		dce_svc_g_once_block = pthread_once_init;
int			dce_svc_g_brief = FALSE;


/*
**  Initialize the mutex.
*/
void
dce_svc__init_mutex(
    void
)
{
    char		*p;

    if (pthread_mutex_init(&dce_svc_g_mutex, pthread_mutexattr_default) < 0)
	return;
    if ((p = getenv(SVC_BRIEF_ENVVAR)) != NULL)
	dce_svc_g_brief = atoi(p);
    dce_svc_g_setup = true;
}


void *
dce_svc__mapthreadid(
    pthread_t	*tid
)
{
#if	defined(MUTEX_FAST_NP)
    return tid->field1;
#else
    return *tid;
#endif	/* defined(MUTEX_FAST_NP) */
}
