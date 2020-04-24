/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_threadStatic.h,v $
 * Revision 1.1.8.1  1996/10/17  18:27:51  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:58  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:31:38  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:11:26  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:23:48  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	 Selected comments:
 * 	   Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	   Initial checkin.
 * 	[1992/09/24  12:42:10  jaffe]
 * 
 * $EndLog$
 */
/*
 *  itl_threadStatic.h --
 *    This package implements thread static variables.
 *    For this ITL they are used to implement static variables that
 *    contain thread specific information.
 * 
 */

#ifndef ITL_THREAD_STATIC_H
#define ITL_THREAD_STATIC_H

#include <errno.h>
#include <pthread.h>

#include "itl_stds.h"

/*
 *  Arguments:
 *    _dkey      - a key for a thread data area (via pthread_keycreate)
 *    _dptr      - a pointer variable
 *    _dptr_type - the type of the _sptr variable.
 *    _init_func - function to call to initialze data.  It is passed the
 *                 _dptr argument. 
 */

#define ITL_THREAD_STATIC_INIT(_dkey, _dptr, _dptr_type, _init_func)         \
        MACRO_BEGIN                                                          \
          if (pthread_getspecific(_dkey, (pthread_addr_t *) &(_dptr)) != 0)  \
          {                                                                  \
            itl_Error("pthread_getspecific failed, errno = %d\n", errno);    \
            exit(-1);                                                        \
          }                                                                  \
          if (!(_dptr))                                                      \
          {                                                                  \
            ITL_UTIL_NEW(_dptr, _dptr_type);                                 \
            if (pthread_setspecific((_dkey), (pthread_addr_t) (_dptr)) != 0) \
            {                                                                \
              itl_Error("pthread_setspecific failed, errno = %d\n", errno);  \
              exit(-1);                                                      \
            }                                                                \
	    _init_func((_dptr));                                             \
          }                                                                  \
        MACRO_END


#endif  /* ITL_THREAD_STATIC_H */
