/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_libc_calls.h,v $
 * Revision 1.1.6.2  1996/02/18  23:08:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:20  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:45:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:59:05  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:34:10  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:16  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:20:37  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:05  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef P_LIBC_CALLS
#define P_LIBC_CALLS

#define accept	    _accept_sys
#define close	    _close_sys
#define connect     _connect_sys
#define creat	    _creat_sys
#define dup	    _dup_sys
#define dup2	    _dup2_sys
#define fcntl	    _fcntl_sys
#define fork	    _fork_sys
#define open	    _open_sys
#define pipe	    _pipe_sys
#define recv	    _recv_sys
#define recvmsg     _recvmsg_sys
#define recvfrom    _recvfrom_sys
#define read	    _read_sys
#define readv	    _readv_sys
#define select	    _select_sys
#define send	    _send_sys
#define sendmsg     _sendmsg_sys
#define sendto	    _sendto_sys
#define sigaction   _sigaction_sys
#define socket	    _socket_sys
#define socketpair  _socketpair_sys
#define write	    _write_sys
#define writev	    _writev_sys

#endif
