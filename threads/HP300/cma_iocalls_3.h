/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_iocalls_3.h,v $
 * Revision 1.1.6.2  1996/02/18  23:08:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:16  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:45:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:59:01  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:34:06  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:14  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:20:24  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:58:56  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef CMA_IOCALLS
#define CMA_IOCALLS

#undef creat
#pragma _HP_SECONDARY_DEF _creat creat
#define cma_creat _creat
#define creat _creat_sys

#undef dup
#pragma _HP_SECONDARY_DEF _dup dup
#define cma_dup _dup
#define dup _dup_sys

#undef dup2
#pragma _HP_SECONDARY_DEF _dup2 dup2
#define cma_dup2 _dup2
#define dup2 _dup2_sys

#undef fcntl
#pragma _HP_SECONDARY_DEF _fcntl fcntl
#define cma_fcntl _fcntl
#define fcntl _fcntl_sys

#undef pipe
#pragma _HP_SECONDARY_DEF _pipe pipe
#define cma_pipe _pipe
#define pipe _pipe_sys

#endif
