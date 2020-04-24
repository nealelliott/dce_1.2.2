/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_dceActions.h,v $
 * Revision 1.1.8.1  1996/10/17  18:27:09  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:35  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:30:39  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:09:17  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:14:48  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	  Selected comments:
 * 	    Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	    Initial checkin.
 * 	[1992/09/23  21:09:09  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1991 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_dceActions.h -- 
 *    Contains descriptive strings of DCE function calls.  These are 
 *    intended to be used ithe the ITL_CHECK_DCE_ERROR macro.
 */

#ifndef ITL_DCE_ACTIONS_H
#define ITL_DCE_ACTIONS_H

/*
 *  pthread functions.
 */
#define ITL_PTHR_ATTR_CREATE     "Thread Attribute Creation"
#define ITL_PTHR_THREAD_CREATE   "Thread Creation"
#define ITL_PTHR_KEY_CREATE      "Thread Data Key Creation"
#define ITL_PTHR_SET_DATA        "Set Thread Data"


#endif  /*  ITL_DCE_ACTIONS_H */
