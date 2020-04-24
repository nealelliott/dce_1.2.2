/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_iocalls_4.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:27:18 $
 */
/*
 * HISTORY
 * $Log: cma_iocalls_4.h,v $
 * Revision 1.1.8.1  1996/05/10  13:27:18  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:50 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:47 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:26  devsrc]
 * 	 *
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:52  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  14:33:45  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:41:40  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:10:17  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:36:36  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:31:13  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:52  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:30:55  mgm  1.1.1.11]
 *
 * Revision 1.1.4.1  1994/06/09  13:34:31  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:26  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:21:27  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:52  alan]
 * 
 * Revision 1.1  1992/01/19  14:40:09  devrcs
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

#undef accept
#pragma _HP_SECONDARY_DEF _accept accept
#define cma_accept _accept
#define accept _accept_sys

#undef connect
#pragma _HP_SECONDARY_DEF _connect connect
#define cma_connect _connect
#define connect _connect_sys

#undef recv
#pragma _HP_SECONDARY_DEF _recv recv
#define cma_recv _recv
#define recv _recv_sys

#undef recvmsg
#pragma _HP_SECONDARY_DEF _recvmsg recvmsg
#define cma_recvmsg _recvmsg
#define recvmsg _recvmsg_sys

#undef recvfrom
#pragma _HP_SECONDARY_DEF _recvfrom recvfrom
#define cma_recvfrom _recvfrom
#define recvfrom _recvfrom_sys

#undef select
#pragma _HP_SECONDARY_DEF _select select
#define cma_select _select
#define select _select_sys

#undef send
#pragma _HP_SECONDARY_DEF _send send
#define cma_send _send
#define send _send_sys

#undef sendmsg
#pragma _HP_SECONDARY_DEF _sendmsg sendmsg
#define cma_sendmsg _sendmsg
#define sendmsg _sendmsg_sys

#undef sendto
#pragma _HP_SECONDARY_DEF _sendto sendto
#define cma_sendto _sendto
#define sendto _sendto_sys

#undef socket
#pragma _HP_SECONDARY_DEF _socket socket
#define cma_socket _socket
#define socket _socket_sys

#undef socketpair
#pragma _HP_SECONDARY_DEF _socketpair socketpair
#define cma_socketpair _socketpair
#define socketpair _socketpair_sys

#ifdef __cplusplus
}
#endif

#endif
