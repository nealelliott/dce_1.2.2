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
 * HISTORY
 * $Log: cma_ux.h,v $
 * Revision 1.1.4.2  1996/02/18  23:08:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:20  marty]
 *
 * Revision 1.1.4.1  1995/12/07  21:52:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:01:46  root]
 * 
 * Revision 1.1.2.3  1994/06/13  17:58:33  jd
 * 	Added wrappers for cma_execv and cma_execve
 * 	[1994/06/13  14:38:11  jd]
 * 
 * Revision 1.1.2.2  1994/06/09  13:35:55  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:27  devsrc]
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
 * Revision 1.1.2.1  1994/04/01  20:14:38  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:51:22  jd]
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
#include <sys/uio.h>
/*
 *  INCLUDE FILES
 */

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
#  define execve        cma_execve
#  define execv         cma_execv
# endif /* _CMA_NOWRAPPERS_ && _CMA_UNIPROCESSOR */
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

#endif

/*
 * TYPEDEFS
 */


typedef void (*cma_t_fork_rtn) (cma_t_address);

/*
 *  GLOBAL DATA
 */

/*
 * INTERFACES
 */

extern void cma_atfork ( cma_t_address,cma_t_fork_rtn,cma_t_fork_rtn,cma_t_fork_rtn);

# if _CMA_UNIPROCESSOR_
extern void cma_import_fd (int);

extern void cma_unimport_fd (int);
# endif
extern int  cma_accept (int ,void *,int *);
extern int  cma_select(unsigned long,void *,void *,void *,struct timeval *);
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
extern ssize_t  cma_read (int, const void *,size_t);
extern int  cma_readv (int, struct iovec *, int);
extern int  cma_send (int , const void *, int, int );
extern int  cma_sendmsg (int, const struct msghdr msg[] , int);
extern int  cma_sendto (int,const void *,int, int, const void *,int);
extern int  cma_socket (int, int, int);
extern int  cma_socketpair (int, int, int, int[2]);
extern ssize_t  cma_write (int ,const void *,size_t);
extern int  cma_writev (int, struct iovec *,int);
extern int cma_execve(const char *, char * const *, char *const *); 
extern int cma_execv(const char *, char * const *); 
extern pid_t  cma_fork (void);

#endif

