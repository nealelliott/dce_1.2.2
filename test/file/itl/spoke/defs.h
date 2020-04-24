/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: defs.h,v $
 * Revision 1.1.313.1  1996/10/17  18:28:18  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:12  damon]
 *
 * Revision 1.1.308.1  1994/02/04  20:46:12  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:55  devsrc]
 * 
 * Revision 1.1.306.1  1993/12/07  17:45:17  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:22:43  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: defs.h,v 1.1.313.1 1996/10/17 18:28:18 damon Exp $
 *
 * COMPONENT_NAME: Distributed ITL Development Infrastructure
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1992,1993
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */

#ifndef ITL_SPOKE_DEFS_H
#define ITL_SPOKE_DEFS_H

#include <pthread.h>
#include <dce/rpc.h>
#include <utils/cht_hash.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/spoke.h>
    
#define	ASSERT(x)	\
    MACRO_BEGIN	\
    if (!(x)) { \
	itl_Error("assertion blown at %s [%d]\n",  __FILE__, __LINE__); \
	exit(-1); \
    } \
    MACRO_END
		    
#define	DCE_ERROR(status, msg)  itlu_DCEError((unsigned long) (status), (msg))

#define	LOCK(m) \
    MACRO_BEGIN \
    int _err; \
    _err = pthread_mutex_lock(m); \
    ASSERT(!_err); \
    MACRO_END

#define	UNLOCK(m) \
    MACRO_BEGIN \
    int _err; \
    _err = pthread_mutex_unlock(m) ; \
    ASSERT(!_err); \
    MACRO_END	

#define	WAIT(c,m) \
    MACRO_BEGIN \
    int _err; \
    _err = pthread_cond_wait(c,m); \
    ASSERT(!_err); \
    MACRO_END		


#define	TIMED_WAIT(c,m,abstime,timed_outp) \
    MACRO_BEGIN \
    int _err; \
    _err = pthread_cond_timedwait(c,m,abstime); \
    ASSERT(!_err || errno == EAGAIN); \
    if (_err == -1) *(timed_outp) = (errno == EAGAIN); \
    MACRO_END
    
#define	SIGNAL(c) \
    MACRO_BEGIN \
    int _err; \
    _err = pthread_cond_signal(c); \
    ASSERT(!_err); \
    MACRO_END

#define	BROADCAST(c) \
    MACRO_BEGIN \
    int _err; \
    _err = pthread_cond_broadcast(c); \
    ASSERT(!_err); \
    MACRO_END
    
spoke_v1_0_epv_t epv;
pthread_t listen_thread;


#define ITL_TRACE_SPOKE		0x01000000
#define	DEBUG(x)		itl_Trace x
#define MAX_HOST_NAME_LEN 300
#define MAX_NUMBER_OF_SPOKES 300
#define STDERR_FILENO 2
#define STDOUT_FILENO 1
#define STDIN_FILENO 0

#endif  /* ITL_SPOKE_DEFS_H */
