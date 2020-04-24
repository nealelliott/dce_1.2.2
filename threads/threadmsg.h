/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: threadmsg.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:46:18 $
 */
/*
 * HISTORY
 * $Log: threadmsg.h,v $
 * Revision 1.1.8.1  1996/05/10  13:46:18  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:48 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:46 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:29:10  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:20:01  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  18:12  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  15:22:59  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:45:16  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:15:50  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:28:06  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:36:56  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:45:14  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:36:48  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:39:23  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:29:10  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:18:14  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:20:01  jd]
 * 
 * Revision 1.1.2.2  1992/12/15  22:33:42  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:45:14  alan]
 * 
 * Revision 1.1  1992/01/19  14:38:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

#ifdef __cplusplus
extern "C" {
#endif

#define MESSAGE_VERSION_USED 1
#define MESSAGE_VERSION 1
#define MAX_FMT_TEXT 256
#define MAX_FMT_SIZE ((size_t) 256)

#define CAT_SET 1
#define CAT_FILE "dcethreads.cat"

#ifdef __cplusplus
}
#endif

