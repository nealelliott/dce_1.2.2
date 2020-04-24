/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: D00_dua_protect.c,v $
 * Revision 1.1.8.2  1996/02/18  18:16:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:42  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:52:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:17  root]
 * 
 * Revision 1.1.6.2  1994/06/21  14:46:32  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:38:26  marrek]
 * 
 * Revision 1.1.6.1  1994/02/22  17:38:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:28:14  marrek]
 * 
 * Revision 1.1.4.2  1993/08/11  05:48:54  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:48:27  marrek]
 * 
 * Revision 1.1.2.2  1993/02/02  10:11:10  marrek
 * 	Creation
 * 	[1993/02/01  17:56:02  marrek]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: D00_dua_protect.c,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 18:16:09 $";
#endif


/* source file for dua mutexes */

#include <pthread.h>
#include <D00_dua_protect.h>

/* protect_init function returns 0 if OK */

pthread_mutex_t attached_to_ipc_mutex;
pthread_mutex_t d21_bind_tab_mutex;
pthread_mutex_t d21_ipc_tab_mutex;
pthread_mutex_t d21_sig_count_mutex;



int D00_dua_protect_init(void)
{
	int status = 0;
	pthread_mutexattr_t attr;

	status = pthread_mutexattr_create(&attr);
	if (status == 0) {
		status = pthread_mutexattr_setkind_np(&attr,MUTEX_RECURSIVE_NP);
	} /* end if */

	if (status == 0) {
		status = pthread_mutex_init(&attached_to_ipc_mutex, attr);
	} /* end if */

	if (status == 0) {
		status = pthread_mutex_init(&d21_bind_tab_mutex, attr);
	} /* end if */

	if (status == 0) {
		status = pthread_mutex_init(&d21_ipc_tab_mutex, attr);
	} /* end if */

	if (status == 0) {
		status = pthread_mutex_init(&d21_sig_count_mutex, attr);
	} /* end if */

	if (status == 0) {
		status = pthread_mutexattr_delete(&attr);
	} /* end if */

	return (status);
} /* end D00_dua_protect_init */
