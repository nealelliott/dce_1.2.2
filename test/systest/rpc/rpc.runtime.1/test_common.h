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
 * Revision 1.1.9.2  1996/02/18  23:07:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:40  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:43:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:20:25  root]
 * 
 * Revision 1.1.7.1  1993/09/09  19:03:01  cmckeen
 * 	HP's TET'ized version
 * 	[1993/09/09  18:58:46  cmckeen]
 * 
 * Revision 1.1.5.2  1993/08/24  13:33:48  giza
 * 	.
 * 	[1993/08/23  20:33:52  giza]
 * 
 * Revision 1.1.1.5  1993/05/04  22:21:57  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.2.3  1993/02/05  15:21:58  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:42:35  cjd]
 * 
 * Revision 1.1.3.2  1993/02/04  22:11:04  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.2.2  1992/09/25  19:28:57  eperkins
 * 	Initial version
 * 	[1992/09/25  19:28:38  eperkins]
 * 
 * $EndLog$
 */
/*
*/
#include <dce/cma.h>
#include <dce/rpcexc.h>
#include <dce/cmalib_crtlx.h>
#include <pthread.h>

pthread_key_t thread_gdata_key;

