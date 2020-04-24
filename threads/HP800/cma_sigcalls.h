/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_sigcalls.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:27:38 $
 */
/*
 * HISTORY
 * $Log: cma_sigcalls.h,v $
 * Revision 1.1.8.1  1996/05/10  13:27:38  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:50 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:47 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:56  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  14:18:57  truitt]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:29  devsrc]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:41:45  root  1.1.8.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:10:29  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:40:25  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:31:24  hopkins]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:30:57  mgm  1.1.1.11]
 *
 * Revision 1.1.4.1  1994/06/09  13:34:35  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:29  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:21:40  alan
 * 	Insert copyright notices
 * 	[1992/12/07  16:00:03  alan]
 * 
 * Revision 1.1  1992/01/19  14:40:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef CMA_SIGCALLS
#define CMA_SIGCALLS

#ifdef __cplusplus
extern "C" {
#endif

#undef sigaction
#pragma _HP_SECONDARY_DEF _sigaction sigaction
#define cma_sigaction _sigaction
#define sigaction _sigaction_sys

#pragma _HP_SECONDARY_DEF _sigwait sigwait
#define cma_sigwait _sigwait

#ifdef __cplusplus
}
#endif

#endif
