/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_uxcalls.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:30:25 $
 */
/*
 * HISTORY
 * $Log: cma_uxcalls.h,v $
 * Revision 1.1.8.1  1996/05/10  13:30:25  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:52 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:48 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:57  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  14:16:21  truitt]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:49  devsrc]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:41:46  root  1.1.8.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:10:34  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:41:36  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:31:30  hopkins]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:31:01  mgm  1.1.1.11]
 *
 * Revision 1.1.4.1  1994/06/09  13:34:57  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:49  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:21:53  alan
 * 	Insert copyright notices
 * 	[1992/12/07  16:00:11  alan]
 * 
 * Revision 1.1  1992/01/19  14:40:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef CMA_UXCALLS
#define CMA_UXCALLS

#ifdef __cplusplus
extern "C" {
#endif

#undef fork
#pragma _HP_SECONDARY_DEF _fork fork
#define cma_fork _fork
#define fork fork_sys

#pragma _HP_SECONDARY_DEF _atfork atfork
#define cma_atfork _atfork

#ifdef __cplusplus
}
#endif

#endif
