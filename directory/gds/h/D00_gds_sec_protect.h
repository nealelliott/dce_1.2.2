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
 * $Log: D00_gds_sec_protect.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:59:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:10  root]
 * 
 * Revision 1.1.2.3  1994/06/21  14:47:00  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:19  marrek]
 * 
 * Revision 1.1.2.2  1994/06/09  18:46:08  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:10  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:14:49  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:09:48  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:49:52  marrek]
 * 
 * $EndLog$
 */

#ifndef _D00_GDS_SEC_PROTECT_H
#define _D00_GDS_SEC_PROTECT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char D00_gds_sec_protect_rcsid[] = "@(#)$RCSfile: D00_gds_sec_protect.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:17 $";
#endif

/* header file for dua mutexes */

#ifdef THREADSAFE 
  #include <pthread.h>

#endif 

#ifdef THREADSAFE 

	extern pthread_mutex_t gds_sec_cred_table_mutex;
	#define BEGIN_gds_sec_cred_table_LOCK \
		{int 	ijnmgds_sec_cred_tableslrvwuuu, gds_sec_cred_table_failed;\
		for (gds_sec_cred_table_failed=pthread_mutex_lock(&gds_sec_cred_table_mutex),\
			ijnmgds_sec_cred_tableslrvwuuu=1;	ijnmgds_sec_cred_tableslrvwuuu>0;\
			gds_sec_cred_table_failed=pthread_mutex_unlock(&gds_sec_cred_table_mutex), \
			ijnmgds_sec_cred_tableslrvwuuu--) {{{{{{\
			if (gds_sec_cred_table_failed) {\
				GDS_SEC_TRACE0(2,"mutex lock failed"); \
			} /* end if */
#else 
	#define BEGIN_gds_sec_cred_table_LOCK 
#endif 

#ifdef THREADSAFE 
	#define END_gds_sec_cred_table_LOCK }}}}}}\
		if (gds_sec_cred_table_failed) {\
			GDS_SEC_TRACE0(2,"mutex unlock failed"); \
		} /* end if */ }
#else 
	#define END_gds_sec_cred_table_LOCK 
#endif 

#ifdef THREADSAFE
int D00_gds_sec_protect_init(void);
#endif

#endif /* _D00_GDS_SEC_PROTECT_H */
