/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_ux.h $Revision: 1.1.6.2 $ $Date: 1996/08/09 12:16:46 $
 */
/*
 * HISTORY
 * $Log: cma_ux.h,v $
 * Revision 1.1.6.2  1996/08/09  12:16:46  arvind
 * 	Put ALL exec calls back
 * 	[1996/08/01  19:53 UTC  bissen  /main/DCE_1.2.2/bissen_fix_19701/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:58  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:00:07  root]
 *
 * Revision 1.1.6.1  1996/05/10  13:30:13  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:52 UTC  bissen
 * 	Merge fix for CHFts13125 and CHFts15594.
 * 	[1995/07/31  23:07 UTC  jss  /main/HPDCE02/5]
 * 
 * 	Fix for CHFts13125. Conditionalize the inclusion of STREAMS prototypes.
 * 	[1995/07/31  22:58 UTC  jss  /main/HPDCE02/jss_mothra_fix6/2]
 * 
 * 	Fix for CHFts13125. Add function prototypes for all wrapper functions.
 * 	[1995/07/29  17:58 UTC  jss  /main/HPDCE02/jss_mothra_fix6/1]
 * 
 * 	Merge Spec 1170 changes for Walnut Creek.
 * 	[1995/07/10  20:08 UTC  jss  /main/HPDCE02/4]
 * 
 * 	Change symbol __hpux1010 to hpux1010p.
 * 	[1995/07/10  19:54 UTC  jss  /main/HPDCE02/jss_mothra_cose/3]
 * 
 * 	Change macro __hpux1010p to __hpux1010.
 * 	[1995/06/28  17:24 UTC  jss  /main/HPDCE02/jss_mothra_cose/2]
 * 
 * 	CHFts15474. COSE compliant socket changes.
 * 	[1995/06/09  18:15 UTC  jss  /main/HPDCE02/jss_mothra_cose/1]
 * 
 * 	Merge in exec wrapper changes for CHFts15229 and purify changes.
 * 	[1995/06/05  21:56 UTC  jss  /main/HPDCE02/3]
 * 
 * 	Remove cma_execle define.
 * 	[1995/05/31  21:57 UTC  jss  /main/HPDCE02/jss_mothra_fix4/2]
 * 
 * 	CHFts15229. exec wrapper changes to support adding a cma_execve entry to
 * 	tepv.
 * 	[1995/05/11  17:32 UTC  jss  /main/HPDCE02/jss_mothra_fix4/1]
 * 
 * 	Merge from branch jss_mothra_fix2.
 * 	[1995/02/16  15:44 UTC  jss  /main/HPDCE02/2]
 * 
 * 	Added routine _cma_atexec_hp() to improve DG RPC performance.
 * 	[1995/02/08  21:20 UTC  jss  /main/HPDCE02/jss_mothra_fix2/1]
 * 
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:48 UTC  jss  /main/HPDCE02/1]
 * 
 * 	merge kk and hpdce01
 * 	[1994/01/25  15:14  truitt  /main/HPDCE01/1]
 * 
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  15:55:39  truitt]
 * 	 *
 * 
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:53  root  1.1.9.3]
 * 
 * 	Remove fattach/fdetach defines ...
 * 	[1993/08/20  18:38:07  hopkins]
 * 
 * 	Get rid of HPUX_100 ifdef and add comments
 * 	for STREAMS defines:  these can't be called
 * 	prior to 10.0, but the ifdef is useless unless
 * 	we make every 10.0 application specify it.
 * 	[1993/08/10  20:43:05  hopkins]
 * 
 * 	Add STREAMS defines ...
 * 	[1993/08/06  19:39:56  hopkins]
 * 
 * 	Minor change in SVR4 wrapper (OT7335)
 * 	[1993/08/19  17:15:51  truitt]
 * 
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:14:56  hopkins]
 * 
 * 	Initial King Kong branch
 * 	[1993/07/08  19:14:25  root]
 * 
 * 	Added revision control strings
 * 	[1993/06/04  18:36:01  hopkins]
 * 
 * 	Gamera merge
 * 	[1993/03/26  23:57:22  hopkins]
 * 
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:29:28  osfrcs  1.1.1.8]
 * 
 * 	Add define for sleep wrapper.
 * 	[1993/01/04  16:10:34  hopkins]
 * 
 * 	Add defines for exec wrappers
 * 	[1992/12/21  23:28:37  hopkins]
 * 
 * 	Add define of system to cma_system.  See cma_hpux.c
 * 	for the new system call, which doesn't block SIGCHLD.
 * 	[1992/12/07  21:59:50  hopkins]
 * 
 * Revision 1.1.2.3  1994/06/13  17:57:17  jd
 * 	Added wrapper for cma_execv and cma_execve
 * 	[1994/06/13  14:23:09  jd]
 * 
 * Revision 1.1.2.2  1994/06/09  13:34:56  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:47  devsrc]
 * 
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:34:10  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:23:57  hinman]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:57:25  sekhar]
 * 
 * Revision 1.1.2.1  1994/04/01  20:14:05  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:04:39  jd]
 * 
 * Revision 1.1.6.3  1993/08/12  19:31:03  maher
 * 	CR# 7335 remove redefinition of sleep on SVR4
 * 	[1993/08/12  14:42:17  maher]
 * 
 * 	CR# 7335 remove redefinition of sleep on SVR4
 * 	[1993/08/12  14:42:17  maher]
 * 
 * Revision 1.1.6.2  1993/08/12  19:27:28  maher
 * 	*** empty log message ***
 * 
 * Revision 9.5.1.8  92/12/17  14:13:52  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.2.3  92/11/16  15:48:27  raj
 * 	[raj] Changes for new naming scheme with reentrant libs
 * 
 * Revision 9.5.2.2  92/11/06  11:27:02  raj
 * 	[raj] Changes for new naming scheme with reentrant libraries.
 * 
 * Revision 9.5.1.7  92/09/30  12:43:02  root
 * 	Acceptance of OSF rev 1.1.3.2
 * 
 * Revision 9.12.1.2  92/09/30  10:38:42  hinman
 * 	Auto checkin of OSF rev 1.1.3.2
 * 
 * Revision 1.1  1992/01/19  14:41:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and  copied
 *  only  in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any  other
 *  copies  thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is  hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without  notice
 *  and  should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	Header file for unix system call wrapper routines
 *
 *  AUTHORS:
 *
 *	Hans Oser
 *
 *  CREATION DATE:
 *
 *	19 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	6 December 1989
 *		Made inclusion conditional, added macros for memory allocation
 *		routines, added CMA routine prototypes.
 *	002	Dave Butenhof	18 December 1989
 *		Change ifndef name for module to correspond to file name.
 *	003	Dave Butenhof	13 February 1990
 *		Add a macro for errno reference (this version still uses the
 *		global per-process errno, but the macro access mechanism
 *		should help locate code which might be incompatible with the
 *		final multiprocessor-compatible solution, which will require
 *		such a macro).
 *	004	Webb Scales	22 March 1990
 *		Add definintions for 'sigwait' function.
 *	005	Webb Scales	27 March 1990
 *		Deleted cma_shutdown, added cma_socketpair.
 *		Split the C RTL wrappers, et al, off into cma_crtlx.h
 *	006	Webb Scales	24 July 1990
 *		Added 'creat', 'dup', and 'dup2'.
 *	007	Webb Scales	16 August 1990
 *		Added Apollo changes for new platforms
 *	008	Webb Scales	23 October 1990
 *		Added "import fd" routine.
 *	009	Paul Curtin	11 December 1990
 *		Added msghdr data structure.
 *	010	Webb Scales	11 December 1990
 *		Added wrapper for fcntl().
 *	011	Paul Curtin	31 January 1991
 *		Added _CMA_NOWRAPPERS_ conditional statement
 *	012	Dave Butenhof	5 February 1991
 *		To avoid breaking client code (now that this is pulled in
 *		transparently by cma.h, pthread.h, pthread_exc.h), drop all
 *		the header file includes; which means also dropping the
 *		prototypes (oh well).
 *	013	Dave Butenhof	12 February 1991
 *		Add fork wrapper
 *	014	Paul Curtin	24 April 1991
 *		Removed all wrapper routine prototypes.  Routine
 *		prototypes should now be picked up from the original
 *		headers and #defined to be protos for our routines.
 *		***NOTE: this makes the assumption that the argument
 *		types defined in our wrappers match all protos on all systems.
 *	015	Webb Scales	2 May 1991
 *		Moved the cma_import_fd prototype here, as this is the best
 *		of several poor choices.
 *	016	Dave Butenhof	23 May 1991
 *		Add conditionals for _CMA_UNIPROCESSOR_ so kernel-thread
 *		versions of DECthreads can still use some wrappers (e.g.,
 *		cma_fork()).
 *	017	Webb Scales	30 May 1991
 *		Created an un-import_fd function, and put the prototype here
 *		so it is with the import_fd function.
 *	018	Paul Curtin	06 June 1991
 *		Added Al Simon's atfork work.
 *	019	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: remove wrappers when
 *		_POSIX_REENTRANT_FUNCTIONS is defined.
 *	020	Dave Butenhof	03 October 1991
 *		Exclude wrappers if _CMA_THREAD_SYNC_IO_ is defined.
 *	021	Dave Butenhof	19 November 1991
 *		Remove direct check for _POSIX_REENTRANT_FUNCTIONS; rely on
 *		_CMA_REENTRANT_CLIB_, since we control that. OSF/1 defines
 *		_POSIX_REENTRANT_FUNCTIONS, but OSF DCE reference port isn't
 *		using libc_r.a.
 */


#ifndef CMA_UX
#define CMA_UX

#include <sys/socket.h>
#include <sys/sem.h>
#include <sys/resource.h>
#if __hpux1000p
#  include <stropts.h>
#  include <poll.h>
#endif

/*
 *  INCLUDE FILES
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#if !_CMA_THREAD_SYNC_IO_
# if !defined(_CMA_NOWRAPPERS_) && _CMA_UNIPROCESSOR_ 

/*
 * U*ix I/O System Call Wrappers
 */
#  define accept	cma_accept
#  define close		cma_close
#  define connect	cma_connect
#  define creat		cma_creat
#  define dup		cma_dup
#  define dup2		cma_dup2
#  define fcntl		cma_fcntl
#  define open		cma_open
#  define pipe		cma_pipe
#  define recv		cma_recv
#  define recvmsg	cma_recvmsg
#  define recvfrom	cma_recvfrom
#  define read		cma_read
#  define readv		cma_readv
#  define select	cma_select
#  define send		cma_send
#  define sendmsg	cma_sendmsg
#  define sendto	cma_sendto
#  define socket	cma_socket
#  define socketpair	cma_socketpair
#  define write		cma_write
#  define writev	cma_writev
# endif
/*
 * U*ix process control wrappers
 */
# if !defined(_CMA_NOWRAPPERS_)
#  define fork		cma_fork
#  define atfork	cma_atfork
#  if !_CMA_UNIPROCESSOR_
#   define cma_import_fd(fd)
#  endif
# endif

# if ((_CMA_VENDOR_ == _CMA__HP) && defined(_CMA_REENTRANT_CLIB_))
#  if !defined(_CMA_NOWRAPPERS_) && _CMA_UNIPROCESSOR_ 
#   define ioctl	cma_ioctl
#   define msgrcv	cma_msgrcv
#   define msgsnd	cma_msgsnd
#   define semop	cma_semop
#   define setrlimit	cma_setrlimit
#   define vfork	cma_fork
#   define wait		cma_wait
#   define wait3	cma_wait3
#   define waitpid	cma_waitpid
#   define system	cma_system
#  ifndef HP_CMA_FIX_CHFts19701
#   define execl        cma_execl
#   define execle       cma_execle
#   define execlp       cma_execlp
#   define execv        cma_execv
#   define execvp       cma_execvp
#  endif   /* HP_CMA_FIX_CHFts19701 */
#   define execve	cma_execve
#   define sleep	cma_sleep

/*
 * These defines are here for use with HPUX 10.0.
 * Calls to these routines will fail prior to
 * 10.0 regardless of the state of the defines.
 */
#   define getmsg	cma_getmsg
#   define getpmsg	cma_getpmsg
#   define poll		cma_poll
#   define putmsg	cma_putmsg
#   define putpmsg	cma_putpmsg

#  if defined(_XOPEN_SOURCE_EXTENDED)
#   undef  socket
#   undef  sockpair
#   undef  sendmsg
#   undef  recvmsg
#   define socket       cma_socket2
#   define sockpair     cma_socketpair2
#   define sendmsg      cma_sendmsg2
#   define recvmsg      cma_recvmsg2
#  endif  /* _XOPEN_SOURCE_EXTENDED */

#  endif
# endif
#endif

/*
 * TYPEDEFS
 */

typedef void (*cma_t_fork_rtn) (cma_t_address);

extern int  cma_accept (int ,void *,int *);
extern int  cma_select(int,int *,int *,int *,struct timeval *);
extern int  cma_close (int);
extern int  cma_connect (int,const void *,int);
extern int  cma_creat (const char *,mode_t);
extern int  cma_dup (int);
extern int  cma_dup2 (int ,int);
extern int  cma_fcntl (int,int,...);
extern int  cma_open (const char *,int,...);
extern int  cma_pipe (int []);
extern int  cma_recv (int,void *,int, int);
extern int  cma_recvmsg (int, struct msghdr msg[], int);
extern int  cma_recvfrom (int, 	void *, int, int, void *, int *);
extern ssize_t  cma_read (int, void *,size_t);
extern ssize_t  cma_readv (int, const struct iovec *, size_t);
extern int  cma_send (int , void *, int, int );
extern int  cma_sendmsg (int, const struct msghdr msg[] , int);
extern int  cma_sendto (int,const void *,int, int, const void *,int);
extern int  cma_socket (int, int, int);
extern int  cma_socketpair (int, int, int, int[2]);
extern ssize_t  cma_write (int ,const void *,size_t);
extern ssize_t  cma_writev (int, const struct iovec *,size_t);
extern pid_t  cma_fork (void);

extern void cma_atfork (
            cma_t_address, cma_t_fork_rtn, cma_t_fork_rtn, cma_t_fork_rtn);

# if _CMA_UNIPROCESSOR_
extern void cma_import_fd (int);
# endif

extern void cma_unimport_fd (int);
extern void _cma_atexec_hp (void (*pre_exec)(), void (*post_exec)());

extern int  cma_ioctl (int, int,...);
extern int  cma_msgrcv (int, void *, size_t, long, int);
extern int  cma_msgsnd (int, const void *, size_t, int);
extern int  cma_semop (int, struct sembuf *, unsigned int);
extern int  cma_setrlimit (int, const struct rlimit *);
extern pid_t  cma_wait (int *);
extern pid_t  cma_waitpid (pid_t, int *, int);
extern pid_t  cma_wait3 (int *, int, int *);
extern int  cma_system (const char *);
extern int  cma_execve (const char *, char *const [], char *const []);
extern unsigned int  cma_sleep(unsigned int);

#if __hpux1000p
extern int  cma_getmsg (int, struct strbuf *, struct strbuf *, int *);
extern int  cma_getpmsg (int, struct strbuf *, struct strbuf *, int *, int *);
extern int  cma_poll (struct pollfd *, int, int);
extern int  cma_putmsg (int, struct strbuf *, struct strbuf *, int);
extern int  cma_putpmsg (int, struct strbuf *, struct strbuf *, int, int);
#endif

# if defined(_XOPEN_SOURCE_EXTENDED)
extern int  cma_socket2 (int, int, int);
extern int  cma_socketpair2 (int, int, int, int[2]);
extern ssize_t  cma_sendmsg2 (int, const struct msghdr *, int);
extern ssize_t  cma_recvmsg2 (int, struct msghdr *, int);
# endif  /* _XOPEN_SOURCE_EXTENDED */

#endif
#ifdef __cplusplus
}
#endif

