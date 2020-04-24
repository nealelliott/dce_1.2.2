/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: D00_switch_protect.c,v $
 * Revision 1.1.8.2  1996/02/18  18:17:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:54  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:55:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:55  root]
 * 
 * Revision 1.1.6.1  1994/02/22  18:45:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:37:27  marrek]
 * 
 * Revision 1.1.4.2  1993/08/11  06:45:30  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:01:42  marrek]
 * 
 * Revision 1.1.2.2  1993/02/02  10:17:19  marrek
 * 	Creation.
 * 	[1993/02/02  09:28:28  marrek]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: D00_switch_protect.c,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 18:17:54 $";
#endif


/* source file for switch mutexes */

#include <pthread.h>
#include <D00_switch_protect.h>

/* protect_init function returns 0 if OK */

pthread_mutex_t unauth_mutex;



int D00_switch_protect_init(void)
{
	int status = 0;
	pthread_mutexattr_t attr;

	status = pthread_mutexattr_create(&attr);
	if (status == 0) {
		status = pthread_mutexattr_setkind_np(&attr,MUTEX_RECURSIVE_NP);
	} /* end if */

	if (status == 0) {
		status = pthread_mutex_init(&unauth_mutex, attr);
	} /* end if */

	if (status == 0) {
		status = pthread_mutexattr_delete(&attr);
	} /* end if */

	return (status);
} /* end D00_switch_protect_init */
