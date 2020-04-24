/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: test_common.h,v $
 * Revision 1.1.9.2  1996/02/18  23:06:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:04  marty]
 *
 * Revision 1.1.9.1  1995/12/11  19:30:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:17  root]
 * 
 * Revision 1.1.6.2  1993/06/15  21:08:51  sommerfeld
 * 	Define srandom(x) as srand(x) on HP-UX.
 * 	Define random() as rand() on HP-UX.
 * 	[1993/05/11  14:29:21  tatsu_s]
 * 
 * Revision 1.1.2.2  1993/01/07  23:39:39  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:46:02  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <dce/cma.h>
#include <dce/rpcexc.h>
#include <dce/cmalib_crtlx.h>

#if     defined(__hpux)
#define random()        rand()
#define srandom(x)      srand(x)
#else
extern long random();
#endif  /* defined(__hpux) */
