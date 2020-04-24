/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_libc_calls.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:27:28 $
 */
/*
 * HISTORY
 * $Log: cma_libc_calls.h,v $
 * Revision 1.1.8.1  1996/05/10  13:27:28  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:50 UTC  bissen
 * 	Merge Spec 1170 changes for Walnut Creek.
 * 	[1995/07/10  20:08 UTC  jss  /main/HPDCE02/4]
 *
 * 	Change symbol __hpux1010 to hpux1010p.
 * 	[1995/07/10  19:54 UTC  jss  /main/HPDCE02/jss_mothra_cose/3]
 *
 * 	Change macro __hpux1010p to __hpux1010.
 * 	[1995/06/28  17:24 UTC  jss  /main/HPDCE02/jss_mothra_cose/2]
 *
 * 	CHFts15474. Added COSE compliant socket routines.
 * 	[1995/06/09  20:00 UTC  jss  /main/HPDCE02/jss_mothra_cose/1]
 *
 * 	Merge in exec wrapper changes for CHFts15229 and purify changes.
 * 	[1995/06/05  21:56 UTC  jss  /main/HPDCE02/3]
 *
 * 	CHFts15229. exec wrapper changes to support adding a cma_execve entry to
 * 	tepv.
 * 	[1995/05/11  17:32 UTC  jss  /main/HPDCE02/jss_mothra_fix4/1]
 *
 * 	Fix single-threaded tepv problems with system().
 * 	[1995/04/27  21:06 UTC  jss  /main/HPDCE02/2]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:47 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:54  truitt  /main/HPDCE01/1]
 *
 * 	Support for 10.0 libc_r exec calls.  Added the
 * 	define of execve as _execve_sys for the 10.0 case.
 * 	[1993/10/22  15:05:22  truitt]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:27  devsrc]
 * 	 *
 *
 * 	Change __hpux100 to __hpux1000p
 * 	[1993/09/22  19:58:35  hopkins]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  14:27:00  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:41:44  root  1.1.9.3]
 *
 * 	Remove fattach/fdetach code ...
 * 	[1993/08/20  18:37:56  hopkins]
 *
 * 	Change HPUX_100 define to __hpux100
 * 	[1993/08/11  17:47:08  hopkins]
 *
 * 	Add entries for exec() and STREAMS
 * 	[1993/08/06  19:39:51  hopkins]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:10:23  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:38:58  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:31:19  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:53:35  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:24:26  osfrcs  1.1.1.8]
 *
 * 	DCE 1.0.1 Release
 * 	[1992/09/23  20:36:48  hopkins]
 *
 * Revision 1.1.4.1  1994/06/09  13:34:33  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:27  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:21:34  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:56  alan]
 * 
 * Revision 1.1  1992/01/19  14:40:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

#ifndef P_LIBC_CALLS
#define P_LIBC_CALLS

#ifdef __cplusplus
extern "C" {
#endif

#define accept		_accept_sys
#define close		_close_sys
#define connect 	_connect_sys
#define creat		_creat_sys
#define dup		_dup_sys
#define dup2		_dup2_sys
#define fcntl		_fcntl_sys
#define fork		_fork_sys
#define ioctl		_ioctl_sys
#define msgrcv		_msgrcv_sys
#define	msgsnd		_msgsnd_sys
#define open		_open_sys
#define pipe		_pipe_sys
#define recv		_recv_sys
#define recvmsg 	_recvmsg_sys
#define recvfrom	_recvfrom_sys
#define read		_read_sys
#define readv		_readv_sys
#define select		_select_sys
#define semop		_semop_sys
#define send		_send_sys
#define sendmsg 	_sendmsg_sys
#define sendto		_sendto_sys
#define setrlimit	_setrlimit_sys
#define sigaction	_sigaction_sys
#define socket		_socket_sys
#define socketpair	_socketpair_sys
#define wait		_wait_sys
#define wait3		_wait3_sys
#define waitpid		_waitpid_sys
#define write		_write_sys
#define writev		_writev_sys
#define system          _system_sys
#define execve		_execve_sys

#if __hpux1000p
#define getmsg		_getmsg_sys
#define getpmsg		_getpmsg_sys
#define poll		_poll_sys
#define putmsg		_putmsg_sys
#define putpmsg		_putpmsg_sys
#endif /* __hpux1000p */
#if __hpux1010p
#define recvmsg2	_recvmsg2_sys
#define sendmsg2	_sendmsg2_sys
#define socket2 	_socket2_sys
#define socketpair2	_socketpair2_sys
#endif /* __hpux1010p */

#ifdef __cplusplus
}
#endif

#endif
