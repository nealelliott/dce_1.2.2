/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_iocalls_2.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:26:57 $
 */
/*
 * HISTORY
 * $Log: cma_iocalls_2.h,v $
 * Revision 1.1.8.1  1996/05/10  13:26:57  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:49 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:47 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:24  devsrc]
 * 	 *
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:50  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  14:29:22  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:41:39  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:10:06  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:34:16  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:31:02  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:41  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:30:52  mgm  1.1.1.11]
 *
 * Revision 1.1.4.1  1994/06/09  13:34:26  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:24  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:21:13  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:41  alan]
 * 
 * Revision 1.1  1992/01/19  14:40:06  devrcs
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

#undef close
#pragma _HP_SECONDARY_DEF _close close
#define cma_close _close
#define close _close_sys

#undef open
#pragma _HP_SECONDARY_DEF _open open
#define cma_open _open
#define open _open_sys

#undef read
#pragma _HP_SECONDARY_DEF _read read
#define cma_read _read
#define read _read_sys

#undef readv
#pragma _HP_SECONDARY_DEF _readv readv
#define cma_readv _readv
#define readv _readv_sys

#undef write
#pragma _HP_SECONDARY_DEF _write write
#define cma_write _write
#define write _write_sys

#undef writev
#pragma _HP_SECONDARY_DEF _writev writev
#define cma_writev _writev
#define writev _writev_sys

#ifdef __cplusplus
}
#endif

#endif
