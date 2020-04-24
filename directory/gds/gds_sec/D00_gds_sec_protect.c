/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: D00_gds_sec_protect.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:55:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:22  root]
 * 
 * Revision 1.1.2.3  1994/06/21  14:46:48  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:39:32  marrek]
 * 
 * Revision 1.1.2.2  1994/06/09  18:45:07  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:54  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:14:48  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:10:08  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:49:50  marrek]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: D00_gds_sec_protect.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:14 $";
#endif


/* source file for gds_sec mutexes */

#include <pthread.h>
#include <D00_gds_sec_protect.h>

/* protect_init function returns 0 if OK */

pthread_mutex_t gds_sec_cred_table_mutex;



int D00_gds_sec_protect_init(void)
{
	int status = 0;
	pthread_mutexattr_t attr;

	status = pthread_mutexattr_create(&attr);
	if (status == 0) {
		status = pthread_mutexattr_setkind_np(&attr,MUTEX_RECURSIVE_NP);
	} /* end if */

	if (status == 0) {
		status = pthread_mutex_init(&gds_sec_cred_table_mutex, attr);
	} /* end if */

	if (status == 0) {
		status = pthread_mutexattr_delete(&attr);
	} /* end if */

	return (status);
} /* end D00_gds_sec_protect_init */
