/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_platform.h,v $
 * Revision 1.1.8.1  1996/10/17  18:27:33  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:49  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:31:14  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:10:28  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:16:52  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	 Selected comments:
 * 	   Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	   Initial checkin.
 * 	[1992/09/23  21:26:49  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_platform.h -- 
 *    Implements macros that isolate the ITL from the underlying platform.
 *    This should permit different threading/mutex/io models to be
 *    implemented.
 *
 *    Currently it implements two models, the Encina BDE, and the OSF DCE.
 */

/*
 *  Input/Output Macros
 */
#define ITL_IO_OPEN(
