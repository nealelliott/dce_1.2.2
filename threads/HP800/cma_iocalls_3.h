/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_iocalls_3.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:27:06 $
 */
/*
 * HISTORY
 * $Log: cma_iocalls_3.h,v $
 * Revision 1.1.8.1  1996/05/10  13:27:06  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:50 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:47 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:25  devsrc]
 * 	 *
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:51  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  14:31:31  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:41:39  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:10:12  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:35:29  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:31:07  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:47  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:30:54  mgm  1.1.1.11]
 *
 * Revision 1.1.4.1  1994/06/09  13:34:28  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:25  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:21:19  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:47  alan]
 * 
 * Revision 1.1  1992/01/19  14:40:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef CMA_IOCALLS
#define CMA_IOCALLS

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif
