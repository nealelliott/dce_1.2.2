/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: D00_dua_protect.h,v $
 * Revision 1.1.8.2  1996/02/18  23:35:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:07  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:59:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:08  root]
 * 
 * Revision 1.1.6.2  1994/06/21  14:46:59  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:17  marrek]
 * 
 * Revision 1.1.6.1  1994/02/22  18:07:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:41:47  marrek]
 * 
 * Revision 1.1.4.2  1993/08/10  12:08:05  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:36:10  marrek]
 * 
 * Revision 1.1.2.2  1993/02/02  10:18:39  marrek
 * 	Creation.
 * 	[1993/02/02  09:37:47  marrek]
 * 
 * $EndLog$
 */

#ifndef _D00_DUA_PROTECT_H
#define _D00_DUA_PROTECT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char D00_dua_protect_rcsid[] = "@(#)$RCSfile: D00_dua_protect.h,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 23:35:16 $";
#endif

/* header file for dua mutexes */

#ifdef THREADSAFE 
  #include <pthread.h>

#endif 

#ifdef THREADSAFE 

	extern pthread_mutex_t attached_to_ipc_mutex;
	#define BEGIN_attached_to_ipc_LOCK \
		{int 	ijnmattached_to_ipcslrvwuuu, attached_to_ipc_failed;\
		for (attached_to_ipc_failed=pthread_mutex_lock(&attached_to_ipc_mutex),\
			ijnmattached_to_ipcslrvwuuu=1;	ijnmattached_to_ipcslrvwuuu>0;\
			attached_to_ipc_failed=pthread_mutex_unlock(&attached_to_ipc_mutex), \
			ijnmattached_to_ipcslrvwuuu--) {{{{{{\
			if (attached_to_ipc_failed) { \
				DCE_SVC_LOG((GDS_S_MUTEX_LOCK_FAILED_MSG, \
				"attached_to_ipc",function_name)); \
			} /* end if */
#else 
	#define BEGIN_attached_to_ipc_LOCK 
#endif 

#ifdef THREADSAFE 
	#define END_attached_to_ipc_LOCK }}}}}}\
		if (attached_to_ipc_failed) {\
			DCE_SVC_LOG((GDS_S_MUTEX_LOCK_FAILED_MSG, \
			"attached_to_ipc",function_name)); \
		} /* end if */ }
#else 
	#define END_attached_to_ipc_LOCK 
#endif 

#ifdef THREADSAFE 

	extern pthread_mutex_t d21_bind_tab_mutex;
	#define BEGIN_d21_bind_tab_LOCK \
		{int 	ijnmd21_bind_tabslrvwuuu, d21_bind_tab_failed;\
		for (d21_bind_tab_failed=pthread_mutex_lock(&d21_bind_tab_mutex),\
			ijnmd21_bind_tabslrvwuuu=1;	ijnmd21_bind_tabslrvwuuu>0;\
			d21_bind_tab_failed=pthread_mutex_unlock(&d21_bind_tab_mutex), \
			ijnmd21_bind_tabslrvwuuu--) {{{{{{\
			if (d21_bind_tab_failed) {\
				DCE_SVC_LOG((GDS_S_MUTEX_LOCK_FAILED_MSG, \
				"d21_bind_tab",function_name)); \
			} /* end if */
#else 
	#define BEGIN_d21_bind_tab_LOCK 
#endif 

#ifdef THREADSAFE 
	#define END_d21_bind_tab_LOCK }}}}}}\
		if (d21_bind_tab_failed) {\
			DCE_SVC_LOG((GDS_S_MUTEX_LOCK_FAILED_MSG, \
			"d21_bind_tab",function_name)); \
		} /* end if */ }
#else 
	#define END_d21_bind_tab_LOCK 
#endif 

#ifdef THREADSAFE 

	extern pthread_mutex_t d21_ipc_tab_mutex;
	#define BEGIN_d21_ipc_tab_LOCK \
		{int 	ijnmd21_ipc_tabslrvwuuu, d21_ipc_tab_failed;\
		for (d21_ipc_tab_failed=pthread_mutex_lock(&d21_ipc_tab_mutex),\
			ijnmd21_ipc_tabslrvwuuu=1;	ijnmd21_ipc_tabslrvwuuu>0;\
			d21_ipc_tab_failed=pthread_mutex_unlock(&d21_ipc_tab_mutex), \
			ijnmd21_ipc_tabslrvwuuu--) {{{{{{\
			if (d21_ipc_tab_failed) {\
				DCE_SVC_LOG((GDS_S_MUTEX_LOCK_FAILED_MSG, \
				"d21_ipc_tab",function_name)); \
			} /* end if */
#else 
	#define BEGIN_d21_ipc_tab_LOCK 
#endif 

#ifdef THREADSAFE 
	#define END_d21_ipc_tab_LOCK }}}}}}\
		if (d21_ipc_tab_failed) {\
			DCE_SVC_LOG((GDS_S_MUTEX_LOCK_FAILED_MSG, \
			"d21_ipc_tab",function_name)); \
		} /* end if */ }
#else 
	#define END_d21_ipc_tab_LOCK 
#endif 

#ifdef THREADSAFE 

	extern pthread_mutex_t d21_sig_count_mutex;
	#define BEGIN_d21_sig_count_LOCK \
		{int 	ijnmd21_sig_countslrvwuuu, d21_sig_count_failed;\
		for (d21_sig_count_failed=pthread_mutex_lock(&d21_sig_count_mutex),\
			ijnmd21_sig_countslrvwuuu=1;	ijnmd21_sig_countslrvwuuu>0;\
			d21_sig_count_failed=pthread_mutex_unlock(&d21_sig_count_mutex), \
			ijnmd21_sig_countslrvwuuu--) {{{{{{\
			if (d21_sig_count_failed) {\
				DCE_SVC_LOG((GDS_S_MUTEX_LOCK_FAILED_MSG, \
				"d21_sig_count",function_name)); \
			} /* end if */
#else 
	#define BEGIN_d21_sig_count_LOCK 
#endif 

#ifdef THREADSAFE 
	#define END_d21_sig_count_LOCK }}}}}}\
		if (d21_sig_count_failed) {\
			DCE_SVC_LOG((GDS_S_MUTEX_LOCK_FAILED_MSG, \
			"d21_sig_count",function_name)); \
		} /* end if */ }
#else 
	#define END_d21_sig_count_LOCK 
#endif 

#ifdef THREADSAFE 
int D00_dua_protect_init(void);
#endif

#endif /* _D00_DUA_PROTECT_H */
