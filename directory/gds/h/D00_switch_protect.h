/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: D00_switch_protect.h,v $
 * Revision 1.1.8.2  1996/02/18  23:35:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:10  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:59:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:11  root]
 * 
 * Revision 1.1.6.2  1994/06/21  14:47:01  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:21  marrek]
 * 
 * Revision 1.1.6.1  1994/02/22  18:07:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:41:55  marrek]
 * 
 * Revision 1.1.4.2  1993/08/10  12:08:56  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:36:18  marrek]
 * 
 * Revision 1.1.2.2  1993/02/02  10:18:47  marrek
 * 	Creation.
 * 	[1993/02/02  09:38:02  marrek]
 * 
 * $EndLog$
 */

#ifndef _D00_SWITCH_PROTECT_H
#define _D00_SWITCH_PROTECT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char D00_switch_protect_rcsid[] = "@(#)$RCSfile: D00_switch_protect.h,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 23:35:18 $";
#endif

/* header file for switch mutexes */

#ifdef THREADSAFE 
  #include <pthread.h>

#endif 

#ifdef THREADSAFE 

	extern pthread_mutex_t unauth_mutex;
	#define BEGIN_unauth_LOCK \
		{int 	ijnmunauthslrvwuuu, unauth_failed;\
		for (unauth_failed=pthread_mutex_lock(&unauth_mutex),\
			ijnmunauthslrvwuuu=1;	ijnmunauthslrvwuuu>0;\
			unauth_failed=pthread_mutex_unlock(&unauth_mutex), \
			ijnmunauthslrvwuuu--) {{{{{{\
			if (unauth_failed) {\
				DCE_SVC_LOG((GDS_S_MUTEX_LOCK_FAILED_MSG, \
				"unauth",function_name)); \
			} /* end if */
#else 
	#define BEGIN_unauth_LOCK 
#endif 

#ifdef THREADSAFE 
	#define END_unauth_LOCK }}}}}}\
		if (unauth_failed) {\
			DCE_SVC_LOG((GDS_S_MUTEX_LOCK_FAILED_MSG, \
			"unauth",function_name)); \
		} /* end if */ }
#else 
	#define END_unauth_LOCK 
#endif 

#ifdef THREADSAFE 
int D00_switch_protect_init(void);
#endif

#endif /* _D00_SWITCH_PROTECT_H */
