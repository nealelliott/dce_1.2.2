/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_iocalls_4.h,v $
 * Revision 1.1.6.2  1996/02/18  23:08:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:18  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:45:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:59:03  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:34:08  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:15  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:20:31  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:01  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef CMA_IOCALLS
#define CMA_IOCALLS

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

#endif
