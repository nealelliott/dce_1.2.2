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
 * $Log: msgpthread.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:13  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:37:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:41  root]
 * 
 * Revision 1.1.4.4  1994/09/07  21:05:04  bowe
 * 	Add check SVC_NOXPGCAT, set dce_msg_g_noxpgcat. [CR 12079]
 * 	[1994/09/07  21:03:39  bowe]
 * 
 * Revision 1.1.4.3  1994/07/27  12:27:58  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:34:16  rsalz]
 * 
 * Revision 1.1.4.2  1994/06/09  16:06:04  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:39  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/29  18:11:15  rsalz
 * 	Check pthread_xxx return values where possible (OT CR 8883)
 * 	[1994/03/29  15:20:24  rsalz]
 * 
 * Revision 1.1.2.3  1993/12/01  21:18:26  rsalz
 * 	Fix pthread_once function to NOT set lock
 * 	[1993/12/01  21:18:02  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:08:49  rsalz
 * 	Initial release
 * 	[1993/08/16  18:03:42  rsalz]
 * 
 * $EndLog$
 */

#include <msgimpl.h>


/*
**  Initialization mutex.
*/
boolean			dce_msg_g_setup;
pthread_mutex_t		dce_msg_g_mutex;
pthread_once_t		dce_msg_g_once_block = pthread_once_init;
int			dce_msg_g_noxpgcat = FALSE;


/*
**  Initialize the mutex.
*/
void
dce_msg__init_mutex(
    void
)
{
#ifdef DCE_DEBUG
    char	*p;
#endif

    if (pthread_mutex_init(&dce_msg_g_mutex, pthread_mutexattr_default) < 0)
	return;
#ifdef DCE_DEBUG
    if ((p = getenv(SVC_NOXPGCAT_ENVVAR)) != NULL)
	dce_msg_g_noxpgcat = atoi(p);
#endif
    dce_msg_g_setup = true;
}
