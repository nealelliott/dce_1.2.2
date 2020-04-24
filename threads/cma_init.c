/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: cma_init.c,v $
 * Revision 1.1.17.1  1996/05/10  13:37:38  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/4  1996/04/22  18:36 UTC  bissen
 * 	Fix .pthread_is_multithreaded_np
 *
 * 	HP revision /main/DCE_1.2/3  1996/03/20  16:44 UTC  psn
 * 	Fix build error -arvind.
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/20  15:50 UTC  bissen
 * 	Need to fix / * #ifdef _HP_LIBC_R * /
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:42 UTC  bissen
 * 	Merge fixes for CHFts16291 and CHFts16294.
 * 	[1995/09/14  20:09 UTC  jss  /main/HPDCE02/7]
 *
 * 	Fix for CHFts16291. Do not allow cma__g_mx_file to be larger than FD_SETSIZE.
 * 	[1995/09/13  20:34 UTC  jss  /main/HPDCE02/jss_wc_thd_tests/1]
 *
 * 	Merge fix for CHFts15539.
 * 	[1995/06/20  20:15 UTC  jss  /main/HPDCE02/6]
 *
 * 	CHFts15539. Added lock checking routines to be used by async signal handlers.
 * 	[1995/06/16  20:17 UTC  jss  /main/HPDCE02/jss_mothra_fix5/1]
 *
 * 	Merge from branch jss_single_thread
 * 	[1995/01/03  22:36 UTC  jss  /main/HPDCE02/5]
 *
 * 	Changes for single threaded client.  Added function pthread_is_multithreaded_np()
 * 	and added call to cma__hp_preinit_signal().
 * 	[1995/01/03  19:15 UTC  jss  /main/HPDCE02/jss_single_thread/2]
 *
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  19:58 UTC  jss  /main/HPDCE02/4]
 *
 * 	Changes for single threaded client.
 * 	[1994/12/15  15:41 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 *
 * 	Merge singlestack code.
 * 	[1994/10/27  21:05 UTC  chris_p  /main/HPDCE02/3]
 *
 * 	Grab the latest DCE bits and make sure they work with the sinlestack hack
 * 	[1994/10/27  17:32 UTC  chris_p  /main/HPDCE02/proven_threads_940803/5]
 *
 * 	Merge from branch jss_close_fix.
 * 	[1994/09/08  19:04 UTC  jss  /main/HPDCE02/2]
 *
 * 	Fix for CHFts12442. Close while I/O pending bug.
 * 	[1994/08/30  14:25 UTC  jss  /main/HPDCE02/jss_fix_close/1]
 *
 * 	Change symbol names with "carevue" to "singlestack"
 * 	[1994/10/17  20:29 UTC  chris_p  /main/HPDCE02/proven_threads_940803/4]
 *
 * 	Put the stackaddr on a 32 byte boundary.
 * 	[1994/10/06  21:51 UTC  chris_p  /main/HPDCE02/proven_threads_940803/3]
 *
 * 	Added HP_CMA_CAREVUE_TEST to set cma_carevue_opt for all the tests
 * 	[1994/09/08  19:29 UTC  chris_p  /main/HPDCE02/proven_threads_940803/2]
 *
 * 	Project carevue.
 * 	Threads now use only one stack.
 * 	Stacks are copied at context switch time.
 * 	[1994/09/06  22:52 UTC  chris_p  /main/HPDCE02/proven_threads_940803/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:04  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  12:50:25  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:36  root  1.1.9.3]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:18:50  root]
 *
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:43:53  hinman]
 *
 * 	Change from static initialization of exceptions
 * 	to runtime initialization, to be consistent with
 * 	the way it was done at DCE1.0.1, so that we get
 * 	backward compatibility with things linked against
 * 	previous versions of the shared libary.  Sigh.
 * 	Same for cma__g_global_mutex.  (Required because
 * 	9.0 linker copies BSS and DATA at link time, but
 * 	recopies DATA only at runtime.)
 * 	[1993/06/18  18:29:18  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:54:52  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:27:12  osfrcs  1.1.1.8]
 *
 * 	DCE 1.0.1 Release
 * 	[1992/09/23  20:38:02  hopkins]
 *
 * Revision 1.1.10.2  1994/06/09  13:38:06  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:01  devsrc]
 * 
 * Revision 1.1.10.1  1994/04/01  20:16:29  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:42  jd]
 * 
 * Revision 1.1.7.2  1993/06/24  21:01:04  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:43:53  hinman]
 * 
 * Revision 1.1.3.6  1993/02/01  22:18:09  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:26:20  hinman]
 * 
 * Revision 1.1.3.5  1993/01/11  22:56:04  jd
 * 	5821 - On OSF/1 platform don't allow the select mask width to exceed
 * 	256. I'd like to make this more dynamic by setting it to the maximum
 * 	number of open file descriptors (= 1024 for 1.1.1). But fd_set in
 * 	types.h still allocates an fds_bits array of FD_SETSIZE=256 and
 * 	applications passing in a select mask are likely to use this.
 * 	[1993/01/11  22:12:09  jd]
 * 	Revision 1.1.4.2  1993/01/11  16:20:05  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.3.4  1992/12/15  22:28:24  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:01  alan]
 * 
 * Revision 1.1.3.3  1992/09/29  20:15:01  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:49:05  sekhar]
 * 
 * Revision 1.1.3.2  1992/09/03  14:37:51  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:04:20  bolinger]
 * 
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:27:11  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1989, 1992 by
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
 *	Concert Multithread Architecture (CMA) services
 *
 *  ABSTRACT:
 *
 *	Initialize CMA
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	19 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	11 September 1989
 *		Implement cma_init external routine
 *	002	Hans Oser	26 September 1989
 *		Additions for thread and dispatch initialisation
 *	003	Dave Butenhof	30 October 1989
 *		Initialize timeslicing (unconditional) and nonblocking I/O
 *		(on unix systems).
 *	004	Dave Butenhof	1 November 1989
 *		Initialize known CV & Mutex queues in init_static to avoid
 *		problems with init dependencies.
 *	005	Webb Scales	7 November 1989
 *		Added cma__init_assem for VMS and enabled timeslicing for MIPS
 *	006	Webb Scales	10 November 1989
 *		Moved cma__init_assem to earlier in the initialization sequence 
 *		as it must be called before any (including null) threads are
 *		created.
 *	007	Dave Butenhof	17 November 1989
 *		Include cma_condition.h explicitly, since cma_tcb.h no longer
 *		does so.
 *	008	Dave Butenhof	8 December 1989
 *		Change cma_init to cma_shr_init (primary cma_init will now
 *		be linked into client image to do exception initialization as
 *		well as old "cma_init" even when CMA is embedded in a
 *		shareable image.
 *	009	Webb Scales	10 December 1989
 *		Remove references to (defunct) low-level thread I/O stuff.
 *	010	Bob Conti	10 December 1989
 *		Replace call to cma__init_exceptions with call to
 *		cma__init_impl_exc, which just initializes those
 *		exceptions that the CMA implementation can RAISE.  
 *		The CMA implementation must use the CMA__ERROR calls rather 
 *		than raising exceptions directly.  (Keeping the unused
 *		exception objects uninitialized will catch accidental use.)
 *		Added one-time flag to prevent accidental re-initialization 
 *		in the middle of a running program; this makes cma_init
 *		modular.
 *	011	Dave Butenhof	11 December 1989
 *		Change name of cma_client_init.h to cma_client.h, and change
 *		back to using cma__init_exceptions so CMA can CATCH
 *		exceptions.
 *	012	Dave Butenhof	26 January 1990
 *		Change initialization of stack chain (it's now a list, not a
 *		queue).
 *	013	Dave Butenhof	2 March 1990
 *		Integrate Kevin Ackley's changes for Apollo M68000 port.
 *	014	Webb Scales	22 March 1990
 *		Add signal module initialization.
 *	015	Dave Butenhof	9 April 1990
 *		New encapsulation for known object structures.
 *	016	Dave Butenhof	31 May 1990
 *		Translate environment variables at initialization.
 *	017	Dave Butenhof	7 June 1990
 *		Change environment variables to upper case.
 *	018	Dave Butenhof	27 June 1990
 *		Remove call to cma__init_vm (no longer needed).
 *	019	Bob Conti	5 July 1990
 *		Adapt cma_shr_init to changes made in CMA_CLIENT.C
 *	020	Paul Curtin     1 August 1990
 *		Added memory initialization routine: cma__init_memory
 *		replaced atoi w/ cma__atoi
 *	021	Webb Scales	16 August 1990
 *		Changed #ifdef's to #if's
 *	022	Dave Butenhof	28 August 1990
 *		Change cma_shr_init to cma__shr_init, since it's really an
 *		internal interface (even though it's called from the client
 *		image and must be in the transfer vector, no client should
 *		call it directly).
 *	023	Webb Scales	29 August 1990
 *		Replace instance of RAISE with call to cma__error.
 *	024	Bob Conti	1 October 1990
 *		Move insertion of main tcb on known threads queue to cma_tcb
 *	025	Dave Butenhof	9 October 1990
 *		Add (null) argument to cma__init_exceptions call, since it's
 *		also used as a cma_once init routine (from cma_init()), and
 *		therefore expects to be passed an argument (which it doesn't
 *		use).
 *	026	Dave Butenhof	22 January 1991
 *		Merge cma_init back into this module (should have been done a
 *		while ago when we removed the requirement that client link
 *		against cma_client.obj).
 *	027	Paul Curtin	24 January 1991
 *		Remove include of cma_client.h
 *	028	Dave Butenhof	24 January 1991
 *		Fix up initialization of exceptions (some status names use
 *		"cma" where they should use "exc").
 *	029	Dave Butenhof	18 March 1991
 *		Define cma_g_debug for UNIX systems.
 *	030	Dave Butenhof	27 March 1991
 *		Change initialization of exceptions to use new names.
 *	031	Dave Butenhof	12 April 1991
 *		Support OSF/1 init, including providing synchronization
 *		functions and per-thread errno to reentrant libraries.
 *	032	Dave Butenhof	24 April 1991
 *		Add function prefix to cma__trace messages.
 *	033	Paul Curtin	 7 May 1991
 *		Reworked cma_init to operate thru a macro: cma__int_init
 *	034	Paul Curtin	06 June 1991
 *		Added to init process, cma__init_atfork.
 *	035	Webb Scales and Dave Butenhof	    10 June 1991
 *		Conditionalize inclusion of I/O stuff.
 *	036	Dave Butenhof	16 September 1991
 *		To make the UNIX file I/O wrappers more efficient, the
 *		(possibly large) mask arrays are allocated dynamically at TCB
 *		creation. For this to work, cma_init_tcb() needs to know the
 *		correct size; since cma_init_thread_io() is usually run
 *		later, move the size calculation (getdtablesize()) here.
 *	037	Dave Butenhof	04 October 1991
 *		Clean up use of _CMA_UNIPROCESSOR_
 *	038	Dave Butenhof	16 October 1991
 *		Add a typecast in cma___get_self_tcb().
 *	039	Dave Butenhof	05 November 1991
 *		Integrate Dave Weisman's changes; build DECthreads on OSF/1
 *		without Mach threads or libc_r.a
 *	040	Dave Butenhof	25 November 1991
 *		Add cma_g_debug_cmd pointer.
 *	041	Dave Butenhof	26 November 1991
 *		Change PLATFORM conditional to OS, to support Alpha VMS.
 *	042	Dave Butenhof	18 December 1991
 *		MIPS C just decided to complain about cma_g_debug_cmd typing;
 *		it's right, so I'll fix it.
 *	043	Dave Butenhof	18 December 1991
 *		Change interface to cma__getenv to make it cleanly reentrant.
 *	044	Dave Butenhof	24 January 1992
 *		Disable declaration of extern exceptions in exc_handling.h so
 *		that all exceptions may be statically initialized as
 *		exc_kind_status_t structures without type conflicts.
 *	045	Dave Butenhof	12 March 1992
 *		Fix trace sequencing around atfork init.
 *	046	Dave Butenhof	19 March 1992
 *		Add debugging code to check libc_r.a mutex access on OSF/1.
 *	047	Dave Butenhof	02 April 1992
 *		Use _pthread_init_routine hook for automatic crt0.o init on
 *		OSF/1.
 *	048	Dave Butenhof	17 April 1992
 *		Include a version string from cma_version.h
 *	049	Webb Scales	24 April 1992
 *		Unconditionalize signal module init call.
 */


/*
 *  INCLUDE FILES
 */


#define _EXC_NO_EXCEPTIONS_		/* tell exc_handling.h to skip exports */
#include <cma.h>
#undef _EXC_NO_EXCEPTIONS_
#if defined(SNI_DCOSX)
#include <sys/resource.h>
#endif	/* SNI_DCOSX */
#include <cma_defs.h>

#if _CMA_UNIX_TYPE ==  _CMA__SVR4
#define CMA_INIT_NEEDED 1
#endif

#include <cma_init.h>
#include <cma_condition.h>
#include <cma_once.h>
#include <cma_tcb.h>
#include <cma_context.h>
#include <cma_dispatch.h>
#include <cma_timer.h>
#include <cma_stack.h>
#include <cma_queue.h>
#include <cma_list.h>
#include <cma_vm.h>
#include <cma_deb_core.h>
#include <cma_util.h>
#include <cma_int_errno.h>
#include <cma_version.h>

#if (_CMA_OS_ == _CMA__UNIX) && !_CMA_THREAD_SYNC_IO_
# include <cma_thread_io.h>
#endif

#if _CMA_PLATFORM_ != _CMA__VAX_VMS
# include <pthread.h>
#endif

#if (_CMA_OS_ == _CMA__UNIX)
#include <cma_signal.h>
#endif

/*
 *  LOCAL MACROS
 */

/*
 *  GLOBAL DATA
 */
int getdtablesize(void);

cma__t_atomic_bit	cma__g_init_started = cma__c_tac_static_clear;
cma__t_atomic_bit	cma__g_init_done = cma__c_tac_static_clear;

cma__t_atomic_bit	cma__g_init_started2 = cma__c_tac_static_clear;
cma__t_atomic_bit	cma__g_init_done2 = cma__c_tac_static_clear;


#if _CMA_PLATFORM_ != _CMA__VAX_VMS
/*
 * Define the CMA and pthread symbols which can be referenced by the client
 * program.  On VAX/VMS, these symbols are defined in the shareable image
 * transfer vector, not here. (Note, we want this to compile on Alpha VMS,
 * because of the difference in transfer vectors).
 */

/*
 * Define pointer to cma_debug; this ensures that cma_debugger.o is includes
 * in client a.out files, and that the user can call it manually from the
 * debugger.
 */
_CMA_EXPORT_ void (*cma_g_debug) (void) = cma_debug;
_CMA_EXPORT_ void (*cma_g_debug_cmd) (char *, ...) = cma_debug_cmd;

/*
 * Predefined handles used by CMA and clients: the CMA "null handle", and the
 * POSIX 1003.4a default attributes object handles.
 */

_CMA_EXPORT_ cma_t_handle		cma_c_null 		  = {0, 0};
_CMA_EXPORT_ pthread_attr_t		pthread_attr_default 	  = {0, 0};
_CMA_EXPORT_ pthread_mutexattr_t	pthread_mutexattr_default = {0, 0};
_CMA_EXPORT_ pthread_condattr_t		pthread_condattr_default  = {0, 0};

/* 
 * CMA exceptions
 */

#ifdef __hpux

/*
 * Routine to do run-time initialization
 * of exceptions.  See comments at actual
 * routine.
 */

static void
__hp_init_staticx (void);

/*
 * Declare these but don't initialize ...
 * we have to make sure they end up in BSS.
 * Ugh.
 */

_CMA_EXPORT_ exc_kind_status_t
    exc_e_uninitexc,
    exc_e_illaddr,
    exc_e_exquota,
    exc_e_insfmem,
    exc_e_nopriv,
    exc_e_illinstr,
    exc_e_resaddr,
    exc_e_privinst,
    exc_e_resoper,
    exc_e_SIGTRAP,
    exc_e_SIGIOT,
    exc_e_SIGEMT,
    exc_e_aritherr,
    exc_e_SIGSYS,
    exc_e_SIGPIPE,
    exc_e_excpu,
    exc_e_exfilsiz,
    exc_e_intovf,
    exc_e_intdiv,
    exc_e_fltovf,
    exc_e_fltdiv,
    exc_e_fltund,
    exc_e_decovf,
    exc_e_subrng,
#if _CMA_UNIX_TYPE ==  _CMA__SVR4
    exc_e_illopc,
    exc_e_illopn,
    exc_e_illadr,
    exc_e_illtrp,
    exc_e_prvopc,
    exc_e_prvreg,
    exc_e_coproc,
    exc_e_badstk,
    exc_e_brkpt,
    exc_e_trace,
    exc_e_fltinv,
    exc_e_maperr,
    exc_e_accerr,
    exc_e_adraln,
    exc_e_adrerr,
    exc_e_objerr,
    exc_e_fltres,
#endif	/* _CMA_UNIX_TYPE ==  _CMA__SVR4 */
    cma_e_alerted,
    cma_e_assertion,
    cma_e_badparam,
    cma_e_bugcheck,
    cma_e_exit_thread,
    cma_e_existence,
    cma_e_in_use,
    cma_e_use_error,
    cma_e_wrongmutex,
    cma_e_stackovf,
    cma_e_nostackmem,
    cma_e_notcmastack,
    cma_e_unimp,
    cma_e_inialrpro,
    cma_e_defer_q_full,
    cma_e_signal_q_full,
    cma_e_alert_nesting;

#else

_CMA_EXPORT_ exc_kind_status_t
    exc_e_uninitexc = {exc_kind_status_c, exc_s_uninitexc, {exc_newexc_c, exc_v2exc_c}},

    exc_e_illaddr = {exc_kind_status_c, exc_s_illaddr, {exc_newexc_c, exc_v2exc_c}},
    exc_e_exquota = {exc_kind_status_c, exc_s_exquota, {exc_newexc_c, exc_v2exc_c}},
    exc_e_insfmem = {exc_kind_status_c, exc_s_insfmem, {exc_newexc_c, exc_v2exc_c}},
    exc_e_nopriv = {exc_kind_status_c, exc_s_nopriv, {exc_newexc_c, exc_v2exc_c}},
    exc_e_illinstr = {exc_kind_status_c, exc_s_illinstr, {exc_newexc_c, exc_v2exc_c}},
    exc_e_resaddr = {exc_kind_status_c, exc_s_resaddr, {exc_newexc_c, exc_v2exc_c}},
    exc_e_privinst = {exc_kind_status_c, exc_s_privinst, {exc_newexc_c, exc_v2exc_c}},
    exc_e_resoper = {exc_kind_status_c, exc_s_resoper, {exc_newexc_c, exc_v2exc_c}},
    exc_e_SIGTRAP = {exc_kind_status_c, exc_s_SIGTRAP, {exc_newexc_c, exc_v2exc_c}},
    exc_e_SIGIOT = {exc_kind_status_c, exc_s_SIGIOT, {exc_newexc_c, exc_v2exc_c}},
    exc_e_SIGEMT = {exc_kind_status_c, exc_s_SIGEMT, {exc_newexc_c, exc_v2exc_c}},
    exc_e_aritherr = {exc_kind_status_c, exc_s_aritherr, {exc_newexc_c, exc_v2exc_c}},
    exc_e_SIGSYS = {exc_kind_status_c, exc_s_SIGSYS, {exc_newexc_c, exc_v2exc_c}},
    exc_e_SIGPIPE = {exc_kind_status_c, exc_s_SIGPIPE, {exc_newexc_c, exc_v2exc_c}},
    exc_e_excpu = {exc_kind_status_c, exc_s_excpu, {exc_newexc_c, exc_v2exc_c}},
    exc_e_exfilsiz = {exc_kind_status_c, exc_s_exfilsiz, {exc_newexc_c, exc_v2exc_c}},
    exc_e_intovf = {exc_kind_status_c, exc_s_intovf, {exc_newexc_c, exc_v2exc_c}},
    exc_e_intdiv = {exc_kind_status_c, exc_s_intdiv, {exc_newexc_c, exc_v2exc_c}},
    exc_e_fltovf = {exc_kind_status_c, exc_s_fltovf, {exc_newexc_c, exc_v2exc_c}},
    exc_e_fltdiv = {exc_kind_status_c, exc_s_fltdiv, {exc_newexc_c, exc_v2exc_c}},
    exc_e_fltund = {exc_kind_status_c, exc_s_fltund, {exc_newexc_c, exc_v2exc_c}},
    exc_e_decovf = {exc_kind_status_c, exc_s_decovf, {exc_newexc_c, exc_v2exc_c}},
    exc_e_subrng = {exc_kind_status_c, exc_s_subrng, {exc_newexc_c, exc_v2exc_c}},
#if _CMA_UNIX_TYPE ==  _CMA__SVR4
    exc_e_illopc = {exc_kind_status_c, exc_s_illopc, {exc_newexc_c, exc_v2exc_c}},
    exc_e_illopn = {exc_kind_status_c, exc_s_illopn, {exc_newexc_c, exc_v2exc_c}},
    exc_e_illadr = {exc_kind_status_c, exc_s_illadr, {exc_newexc_c, exc_v2exc_c}},
    exc_e_illtrp = {exc_kind_status_c, exc_s_illtrp, {exc_newexc_c, exc_v2exc_c}},
    exc_e_prvopc = {exc_kind_status_c, exc_s_prvopc, {exc_newexc_c, exc_v2exc_c}},
    exc_e_prvreg = {exc_kind_status_c, exc_s_prvreg, {exc_newexc_c, exc_v2exc_c}},
    exc_e_coproc = {exc_kind_status_c, exc_s_coproc, {exc_newexc_c, exc_v2exc_c}},
    exc_e_badstk = {exc_kind_status_c, exc_s_badstk, {exc_newexc_c, exc_v2exc_c}},
    exc_e_brkpt = {exc_kind_status_c, exc_s_brkpt, {exc_newexc_c, exc_v2exc_c}},
    exc_e_trace = {exc_kind_status_c, exc_s_trace, {exc_newexc_c, exc_v2exc_c}},
    exc_e_fltinv = {exc_kind_status_c, exc_s_fltinv, {exc_newexc_c, exc_v2exc_c}},
    exc_e_maperr = {exc_kind_status_c, exc_s_maperr, {exc_newexc_c, exc_v2exc_c}},
    exc_e_accerr = {exc_kind_status_c, exc_s_accerr, {exc_newexc_c, exc_v2exc_c}},
    exc_e_adraln = {exc_kind_status_c, exc_s_adraln, {exc_newexc_c, exc_v2exc_c}},
    exc_e_adrerr = {exc_kind_status_c, exc_s_adrerr, {exc_newexc_c, exc_v2exc_c}},
    exc_e_objerr = {exc_kind_status_c, exc_s_objerr, {exc_newexc_c, exc_v2exc_c}},
    exc_e_fltres = {exc_kind_status_c, exc_s_fltres, {exc_newexc_c, exc_v2exc_c}},
#endif	/* _CMA_UNIX_TYPE ==  _CMA__SVR4 */

    cma_e_alerted = {exc_kind_status_c, cma_s_alerted, {exc_newexc_c, exc_v2exc_c}},
    cma_e_assertion = {exc_kind_status_c, cma_s_assertion, {exc_newexc_c, exc_v2exc_c}},
    cma_e_badparam = {exc_kind_status_c, cma_s_badparam, {exc_newexc_c, exc_v2exc_c}},
    cma_e_bugcheck = {exc_kind_status_c, cma_s_bugcheck, {exc_newexc_c, exc_v2exc_c}},
    cma_e_exit_thread = {exc_kind_status_c, cma_s_exit_thread, {exc_newexc_c, exc_v2exc_c}},
    cma_e_existence = {exc_kind_status_c, cma_s_existence, {exc_newexc_c, exc_v2exc_c}},
    cma_e_in_use = {exc_kind_status_c, cma_s_in_use, {exc_newexc_c, exc_v2exc_c}},
    cma_e_use_error = {exc_kind_status_c, cma_s_use_error, {exc_newexc_c, exc_v2exc_c}},
    cma_e_wrongmutex = {exc_kind_status_c, cma_s_wrongmutex, {exc_newexc_c, exc_v2exc_c}},
    cma_e_stackovf = {exc_kind_status_c, cma_s_stackovf, {exc_newexc_c, exc_v2exc_c}},
    cma_e_nostackmem = {exc_kind_status_c, cma_s_nostackmem, {exc_newexc_c, exc_v2exc_c}},
    cma_e_notcmastack = {exc_kind_status_c, cma_s_notcmastack, {exc_newexc_c, exc_v2exc_c}},
    cma_e_unimp = {exc_kind_status_c, cma_s_unimp, {exc_newexc_c, exc_v2exc_c}},
    cma_e_inialrpro = {exc_kind_status_c, cma_s_inialrpro, {exc_newexc_c, exc_v2exc_c}},
    cma_e_defer_q_full = {exc_kind_status_c, cma_s_defer_q_full, {exc_newexc_c, exc_v2exc_c}},
    cma_e_signal_q_full = {exc_kind_status_c, cma_s_signal_q_full, {exc_newexc_c, exc_v2exc_c}},
    cma_e_alert_nesting = {exc_kind_status_c, cma_s_alert_nesting, {exc_newexc_c, exc_v2exc_c}};
#endif	/* __hpux */
#endif					/* if not VAX/VMS */

cma__t_env	cma__g_env[cma__c_env_count] = {
    /*
     * The following highwater marks determine the maximum number of objects
     * which can be cached (of each type) on a particular attributes object;
     * if the number of cached objects exceeds the "max" value, objects will
     * be destroyed until the number is reduced to the "min" value.
     */
    {"CMA_MAXATTR", cma__c_env_type_int, 20},	/* If more attributes than this, */
    {"CMA_MINATTR", cma__c_env_type_int, 5},	/* . purge back to this */
    {"CMA_MAXCOND", cma__c_env_type_int, 20},	/* If more conditions than this */
    {"CMA_MINCOND", cma__c_env_type_int, 5},	/* . purge back to this */
    {"CMA_MAXMUTEX", cma__c_env_type_int, 20},	/* If more mutexes than this */
    {"CMA_MINMUTEX", cma__c_env_type_int, 5},	/* . purge back to this */
    {"CMA_MAXTHREAD", cma__c_env_type_int, 20},	/* If more threads than this */
    {"CMA_MINTHREAD", cma__c_env_type_int, 5},	/* . purge back to this */
    /*
     * End of per-attributes object watermarks
     */
    {"CMA_MAXCLUSTER", cma__c_env_type_int, 2},	/* If more stack clusters than this */
    {"CMA_MINCLUSTER", cma__c_env_type_int, 1},	/* . purge back to this */
    {"CMA_MAXVP", cma__c_env_type_int, 0},	/* Maximum VPs allowed */
    {"CMA_MULTIPLEX", cma__c_env_type_int, 1},	/* Allow multiplexing on VPs */
    {"CMA_TRACE", cma__c_env_type_file, 0}	/* Enable tracing in debug build */
    };

char	*cma__g_version = cma__c_version;

/*
 *  LOCAL DATA
 */


#if (_CMA_OSIMPL_ == _CMA__OS_OSF) && _CMA_REENTRANT_CLIB_
typedef	void	*lib_mutex_t;
typedef	void	*lib_spinlock_t;
typedef void	*lib_threadid_t;

typedef	int		(*lib_mutex_func_t)(lib_mutex_t *);
typedef	int		(*lib_spinlock_func_t)(lib_spinlock_t *);
typedef	lib_threadid_t	(*lib_threadid_func_t)(void);

typedef struct lib_lock_functions {
	lib_mutex_func_t	mutex_create;
	lib_mutex_func_t	mutex_delete;
	lib_mutex_func_t	mutex_lock;
	lib_mutex_func_t	mutex_unlock;
	lib_mutex_func_t	mutex_trylock;
	lib_spinlock_func_t	spinlock_create;
	lib_spinlock_func_t	spinlock_delete;
	lib_spinlock_func_t	spinlock_lock;
	lib_spinlock_func_t	spinlock_unlock;
	lib_spinlock_func_t	spinlock_trylock;
	lib_threadid_func_t	thread_id;
} lib_lock_functions_t;

static lib_lock_functions_t	cma___g_lib_funcs;

# ifndef NDEBUG
static cma__t_int_mutex		*cma___g_dbg_libcr_m;
static lib_mutex_t		cma___g_dbg_libcr_v[1024];
static int			cma___g_dbg_libcr_c;
# endif

#endif

/*
 * LOCAL FUNCTIONS
 */

static void
cma___init_env (void);	/* Get environment variables */

#if (_CMA_OSIMPL_ == _CMA__OS_OSF) && _CMA_REENTRANT_CLIB_
static void
cma___init_lib_hooks (void);		/* Init OSF/1 library hooks */
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initialize the CMA shared library.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
cma_init(void)
    {

    /*
     * Call internal interface (macro)
     */
    cma__int_init();
    cma__int_init2 ();

    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initialize CMA static data at startup time.  This routine only
 *      initializes the portion of CMA that is needed to execute in single
 *      threaded mode.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
cma__init_static1(void)
    {
#ifdef __hpux
    /*
     * Hack to make sure cma__g_global_lock shows
     * up in BSS not DATA ... see comments near
     * __hp_init_staticx ... but global lock must
     * be initialized to 0 before calling cma___get_area()
     * (which is called by cma__alloc_mem()).
     */
    cma__g_global_lock = (cma__t_int_mutex *)cma_c_null_ptr;
    /*
     * Initialize the environment first, specifically to get the value of the
     * trace file (if any). This assume that cma___init_env isn't dependent
     * on ANY other DECthreads features!
     */
    cma___init_env ();

    /*
     * Temp hack to make sure exceptions show up in BSS ...
     * required for shared lib backwards compatability since
     * exceptions used to be in BSS and only moved to DATA
     * at DCE 1.0.2
     */
     __hp_init_staticx();

/* # ifdef _HP_LIBC_R */
    /*
     * Ok to put here since this just initializes a queue
     */
    cma__init_hp();
/*# endif */
#endif

    /*
     * WARNING:
     *
     * Do not EVER change the order of these calls unless you are ABSOLUTELY
     * certain that you know exactly what every one does!  There are many
     * interdependencies, and some (particularly the first few) use special
     * "bootstrap" versions of some calls to get their job done.  Changing
     * the order could be MUCH more complicated than it might seem!
     *
     * In general, new initializations should be added at the END; if this
     * won't do, be VERY, VERY, VERY CAREFUL!  Exception: cma__init_timer
     * should always remain at the END, to ensure that timer events can't
     * occur while other modules are still initializing!
     */
    cma__trace ((cma__c_trc_init, "(init_static) known object queues"));
    cma__queue_init (&cma__g_known_atts.queue);
    cma__queue_init (&cma__g_known_cvs.queue);
    cma__queue_init (&cma__g_known_mutexes.queue);
    cma__queue_init (&cma__g_known_threads.queue);
    cma__list_init (&cma__g_stack_clusters);
    cma__g_mutex_seq.seq = 1;
#if (_CMA_OS_ == _CMA__UNIX) && !_CMA_THREAD_SYNC_IO_
    cma__trace ((cma__c_trc_init, "(init_static) Get file table size"));
#if _CMA_UNIX_TYPE ==  _CMA__SVR4
        {
        struct  rlimit  rip;
        if ( getrlimit(RLIMIT_NOFILE, &rip) == -1 )
                {
                /*
                *       We should never get an error here, but...
                *
                *       Since everything is not initialized at this point, I'm
                *       not sure that it would be wise to call cma__bugcheck(),
                *       so we'll just exit the old fashioned way.
                */

                cma__bugcheck("cma__init_static: Inconsistent return from getrlimit");
                }

        cma__g_mx_file = rip.rlim_max;
        }
#elif _CMA_OSIMPL_ == _CMA__OS_OSF
/*
 * On OSF/1 1.1.1 the number of file descriptors is dynamic with
 * the default at 1024. The select mask width is still fixed at
 * 256.
 */
    cma__g_mx_file = getdtablesize();
    if(cma__g_mx_file > FD_SETSIZE)
        cma__g_mx_file = FD_SETSIZE;
#else
    cma__g_mx_file = getdtablesize();
    if(cma__g_mx_file > FD_SETSIZE)
        cma__g_mx_file = FD_SETSIZE;
#endif
#ifdef _HP_CMA_ASYNC_CLOSE_
/*
 * The number of file objects is determined by cma__g_mx_fo = 
 * (cma_c_mx_file + cma_c_mx_file / 4) to make sure that there are enough fo's
 * available if some of them are still busy with "zombie waiters" from a deferred
 * close.
 */
    cma__g_mx_fo = cma__c_mx_file + cma__c_mx_file / 4;
#endif
# if (_CMA_UNIX_TYPE !=  _CMA__SVR4)
    cma__g_nspm = ((cma__g_mx_file + cma__c_nbpm - 1)/cma__c_nbpm);
    cma__trace ((
	    cma__c_trc_init,
	    "(init_static)  %d file descriptors; mask is %d ints",
	    cma__g_mx_file,
	    cma__g_nspm));
# else
    cma__trace ((
            cma__c_trc_init,
            "(init_static)  %d file descriptors",
            cma__g_mx_file));
# endif /* (_CMA_UNIX_TYPE !=  _CMA__SVR4) */
#endif
#if _CMA_PLATFORM_ == _CMA__VAX_VMS
    cma__trace ((cma__c_trc_init, "(init_static) VAX/VMS assembler code"));
    cma__init_assem ();
#endif
    cma__trace ((cma__c_trc_init, "(init_static) memory management"));
    cma__init_memory ();
    cma__trace ((cma__c_trc_init, "(init_static) attributes"));
    cma__init_attr ();
    cma__trace ((cma__c_trc_init, "(init_static) mutexes"));
    cma__init_mutex ();
    cma__trace ((cma__c_trc_init, "(init_static) memory management locks"));
    cma__init_mem_locks ();
    cma__trace ((cma__c_trc_init, "(init_static) per-thread context"));
    cma__init_context ();
    cma__trace ((cma__c_trc_init, "(init_static) condition variables"));
    cma__init_cv ();
#if _CMA_OS_ != _CMA__VMS
    cma__trace ((cma__c_trc_init, "(init_static) atfork init"));
    cma__init_atfork ();
#endif
    cma__trace ((cma__c_trc_init, "(init_static) atfork init"));
    cma__init_atfork ();
    cma__trace ((cma__c_trc_init, "(init_static) one-time init"));
    cma__init_once ();
    cma__trace ((cma__c_trc_init, "(init_static) stacks"));
    cma__init_stack ();
#if _CMA_KTHREADS_ != _CMA__NONE
    cma__trace ((cma__c_trc_init, "(init_static) VP layer"));
    cma__init_vp ();
#endif
    cma__trace ((cma__c_trc_init, "(init_static) tcb management"));
    cma__init_tcb ();
    cma__trace ((cma__c_trc_init, "(init_static) pre-init signal handlers"));
    cma__preinit_signal ();
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initialize all CMA static data at startup time. This routine is
 *      called by pthread_create(), and is only needed to initialize the
 *      portion of CMA used during multi-threaded execution.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
cma__init_static2(void)
    {
    cma__trace ((cma__c_trc_init, "(init_static) deferral"));
    cma__init_defer ();
    cma__trace ((cma__c_trc_init, "(init_static) dispatcher"));
    cma__init_dispatch ();
#if _CMA_OS_ == _CMA__UNIX
# if !_CMA_THREAD_SYNC_IO_
    cma__trace ((cma__c_trc_init, "(init_static) thread synchronous I/O"));
    cma__init_thread_io ();
# endif
#endif
    cma__trace ((cma__c_trc_init, "(init_static) signal handlers"));
    cma__init_signal ();
    cma__trace ((cma__c_trc_init, "(init_static) debug code"));
    cma__init_debug ();			/* Must be after mutexes */
#if (_CMA_OSIMPL_ == _CMA__OS_OSF) && _CMA_REENTRANT_CLIB_
    cma__trace ((cma__c_trc_init, "(init_static) library reentrancy hooks"));
    cma___init_lib_hooks ();
#endif
#ifdef _HP_LIBC_R 
    cma__trace ((cma__c_trc_init, "(init_static) HP libc_r"));
    hp__init_libc();
#endif 
    cma__trace ((cma__c_trc_init, "(init_static) timers"));
    cma__init_timer ();
    cma__trace ((cma__c_trc_init, "(init_static) initialization complete"));
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get the value of all CMA environment variables to customize caching
 *	characteristics (and other things).
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	cma__g_env array (names of variables)
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma__g_env array (value of variables)
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
static void
cma___init_env(void)
    {
    cma_t_integer	i;
    char		*stringvalue, buffer[256];

    for (i = 0; i < cma__c_env_count; i++) {
	stringvalue = cma__getenv (cma__g_env[i].name, buffer, 256);

	if (stringvalue != cma_c_null_ptr) {

	    switch (cma__g_env[i].type) {
		case cma__c_env_type_int : {
		    cma__g_env[i].value = cma__atoi (stringvalue);
		    break;
		    }
		case cma__c_env_type_file : {
#ifndef NDEBUG
		    if (i == cma__c_env_trace)
			cma__init_trace (stringvalue);
#endif
		    break;
		    }

		}

	    }

	}

    }

#if (_CMA_OSIMPL_ == _CMA__OS_OSF) && _CMA_REENTRANT_CLIB_
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Define a set of functions that the OSF/1 reentrant libraries use for
 *	synchronization, and provide them to the system. Note that these
 *	functions do not use the _CMA_PROTO_ declaration conditionals, since
 *	the code is only compiled on OSF/1 derived systems, which will always
 *	have ANSI C compilers.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	a set of functions declared here.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	the structure cma___g_lib_funcs is initialized
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
void (*_pthread_init_routine)(void) = cma_init;

static int
cma___mutex_create (lib_mutex_t *mutex)
    {
    *mutex = (lib_mutex_t)cma__get_mutex (&cma__g_def_attr);

    if (*mutex == (lib_mutex_t)0) {
	cma__set_errno (ENOMEM);
	return -1;
	}
    else
	cma__obj_set_name (*mutex, "libc_r mutex");

#ifndef NDEBUG
    cma__int_lock (cma___g_dbg_libcr_m);
    cma__assert_fail (
	    cma___g_dbg_libcr_c < 1024,
	    "too many libc_r mutexes!");
    cma___g_dbg_libcr_v[cma___g_dbg_libcr_c++] = *mutex;
    cma__int_unlock (cma___g_dbg_libcr_m);
#endif
    return 0;
    }

static int
cma___mutex_delete (lib_mutex_t *mutex)
    {

    if (mutex == (lib_mutex_t *)cma_c_null_ptr) {
	cma__set_errno (EINVAL);
	return -1;
	}

    if (*mutex == (lib_mutex_t)cma_c_null_ptr) {
	cma__set_errno (EINVAL);
	return -1;
	}

#ifndef NDEBUG
    {
    int	i, done = cma_c_false;

    cma__int_lock (cma___g_dbg_libcr_m);

    for (i = 0; i < cma___g_dbg_libcr_c; i++) {

	if (cma___g_dbg_libcr_v[i] == *mutex) {
	    cma___g_dbg_libcr_v[i] = (lib_mutex_t)0;
	    done = cma_c_true;
	    break;
	    }

	}

    cma__assert_fail (done, "libc_r deleting mutex that wasn't created!");
    cma__int_unlock (cma___g_dbg_libcr_m);
    }
#endif
    cma__free_mutex ((cma__t_int_mutex *)*mutex);
    *mutex = (lib_mutex_t)cma_c_null_ptr;
    return 0;
    }

static int
cma___mutex_lock (lib_mutex_t *mutex)
    {
#ifndef NDEBUG
    {
    int	i, done = cma_c_false;

    cma__int_lock (cma___g_dbg_libcr_m);

    for (i = 0; i < cma___g_dbg_libcr_c; i++) {

	if (cma___g_dbg_libcr_v[i] == *mutex) {
	    done = cma_c_true;
	    break;
	    }

	}

    cma__assert_fail (done, "libc_r locking mutex that wasn't created!");
    cma__int_unlock (cma___g_dbg_libcr_m);
    }
#endif
    cma__int_lock ((cma__t_int_mutex *)*mutex);
    return 0;
    }

static int
cma___mutex_unlock (lib_mutex_t *mutex)
    {
#ifndef NDEBUG
    {
    int	i, done = cma_c_false;

    cma__int_lock (cma___g_dbg_libcr_m);

    for (i = 0; i < cma___g_dbg_libcr_c; i++) {

	if (cma___g_dbg_libcr_v[i] == *mutex) {
	    done = cma_c_true;
	    break;
	    }

	}

    cma__assert_fail (done, "libc_r unlocking mutex that wasn't created!");
    cma__int_unlock (cma___g_dbg_libcr_m);
    }
#endif
    cma__int_unlock ((cma__t_int_mutex *)*mutex);
    return 0;
    }

static int
cma___mutex_trylock (lib_mutex_t *mutex)
    {
#ifndef NDEBUG
    {
    int	i, done = cma_c_false;

    cma__int_lock (cma___g_dbg_libcr_m);

    for (i = 0; i < cma___g_dbg_libcr_c; i++) {

	if (cma___g_dbg_libcr_v[i] == *mutex) {
	    done = cma_c_true;
	    break;
	    }

	}

    cma__assert_fail (done, "libc_r unlocking mutex that wasn't created!");
    cma__int_unlock (cma___g_dbg_libcr_m);
    }
#endif
    return (cma__int_try_lock ((cma__t_int_mutex *)*mutex));
    }

static int
cma___spin_create (cma__t_atomic_bit **spinner)
    {
    int	status;

    *spinner = cma__alloc_object (cma__t_atomic_bit);

    if ((cma_t_address)*spinner != cma_c_null_ptr) {
	cma__tac_clear (*spinner);
	status = 0;
	}
    else {
	status = -1;
	cma__set_errno (ENOMEM);
	}

    return status;
    }

static int
cma___spin_delete (cma__t_atomic_bit **spinner)
    {

    if (spinner == (cma__t_atomic_bit **)cma_c_null_ptr) {
	cma__set_errno (EINVAL);
	return -1;
	}

    if (*spinner == (cma__t_atomic_bit *)cma_c_null_ptr) {
	cma__set_errno (EINVAL);
	return -1;
	}

    cma__free_mem ((cma_t_address)*spinner);
    *spinner = (cma__t_atomic_bit *)cma_c_null_ptr;
    return 0;
    }

static int
cma___spin_lock (cma__t_atomic_bit **spinner)
    {
    cma__spinlock (*spinner);
    return 0;
    }

static int
cma___spin_unlock (cma__t_atomic_bit **spinner)
    {
    cma__spinunlock (*spinner);
    return 0;
    }

static int
cma___spin_trylock (cma__t_atomic_bit **spinner)
    {
    return cma__test_and_set (*spinner);
    }

static int
cma___get_self_tcb ()
    {
    /*
     * NOTE: this will work even on a uniprocessor build where
     * cma__get_self_tcb() is a macro.
     */
    return (int)cma__get_self_tcb ();
    }

static void
cma___init_lib_hooks (void)
    {
#ifndef NDEBUG
    cma___g_dbg_libcr_c = 0;
    cma___g_dbg_libcr_m = cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma___g_dbg_libcr_m, "libcr debug mutex");
#endif

    cma___g_lib_funcs.mutex_create = (lib_mutex_func_t)cma___mutex_create;
    cma___g_lib_funcs.mutex_delete = (lib_mutex_func_t)cma___mutex_delete;
    cma___g_lib_funcs.mutex_lock = (lib_mutex_func_t)cma___mutex_lock;
    cma___g_lib_funcs.mutex_unlock = (lib_mutex_func_t)cma___mutex_unlock;
    cma___g_lib_funcs.mutex_trylock = (lib_mutex_func_t)cma___mutex_trylock;
    cma___g_lib_funcs.spinlock_create = (lib_spinlock_func_t)cma___spin_create;
    cma___g_lib_funcs.spinlock_delete = (lib_spinlock_func_t)cma___spin_delete;
    cma___g_lib_funcs.spinlock_lock = (lib_spinlock_func_t)cma___spin_lock;
    cma___g_lib_funcs.spinlock_unlock = (lib_spinlock_func_t)cma___spin_unlock;
    cma___g_lib_funcs.spinlock_trylock = (lib_spinlock_func_t)cma___spin_trylock;
    cma___g_lib_funcs.thread_id = (lib_threadid_func_t)cma___get_self_tcb;

    libc_declare_lock_functions (&cma___g_lib_funcs);
    ldr_declare_lock_functions (&cma___g_lib_funcs);
    }
#endif					/* _CMA__OS_OSF */

/*
 *  Test whether CMA has completed its entire multithreaded 
 *  initialization.  cma__g_init_done2 is set to true only after
 *  cma__init_static2() has completed.
 */
extern int pthread_is_multithreaded_np (void)
{
    return (cma__tac_isset (&cma__g_init_done2));
}

#ifdef __hpux

 /*
 *	Provide run-time initialization of exceptions,
 *	so that they end up in BSS.  This makes Gamera
 *	shared libdce.sl backwards compatible with the
 *	DE version, which had run-time initialization.
 *	Sigh.
 */

#define	__hp_statxinit(e,s)				\
	{						\
		(e).kind = exc_kind_status_c;		\
		(e).status = (s);			\
		(e).ext.sentinel = exc_newexc_c;	\
		(e).ext.version = exc_v2exc_c;		\
		(e).ext.extend = (exc_address_t)0;	\
		(e).ext.args = (unsigned int *)0;	\
	}

static void
__hp_init_staticx(void)
{
    __hp_statxinit( exc_e_uninitexc, exc_s_uninitexc );

    __hp_statxinit( exc_e_illaddr, exc_s_illaddr );
    __hp_statxinit( exc_e_exquota, exc_s_exquota );
    __hp_statxinit( exc_e_insfmem, exc_s_insfmem );
    __hp_statxinit( exc_e_nopriv, exc_s_nopriv );
    __hp_statxinit( exc_e_illinstr, exc_s_illinstr );
    __hp_statxinit( exc_e_resaddr, exc_s_resaddr );
    __hp_statxinit( exc_e_privinst, exc_s_privinst );
    __hp_statxinit( exc_e_resoper, exc_s_resoper );
    __hp_statxinit( exc_e_SIGTRAP, exc_s_SIGTRAP );
    __hp_statxinit( exc_e_SIGIOT, exc_s_SIGIOT );
    __hp_statxinit( exc_e_SIGEMT, exc_s_SIGEMT );
    __hp_statxinit( exc_e_aritherr, exc_s_aritherr );
    __hp_statxinit( exc_e_SIGSYS, exc_s_SIGSYS );
    __hp_statxinit( exc_e_SIGPIPE, exc_s_SIGPIPE );
    __hp_statxinit( exc_e_excpu, exc_s_excpu );
    __hp_statxinit( exc_e_exfilsiz, exc_s_exfilsiz );
    __hp_statxinit( exc_e_intovf, exc_s_intovf );
    __hp_statxinit( exc_e_intdiv, exc_s_intdiv );
    __hp_statxinit( exc_e_fltovf, exc_s_fltovf );
    __hp_statxinit( exc_e_fltdiv, exc_s_fltdiv );
    __hp_statxinit( exc_e_fltund, exc_s_fltund );
    __hp_statxinit( exc_e_decovf, exc_s_decovf );
    __hp_statxinit( exc_e_subrng, exc_s_subrng );

#if _CMA_UNIX_TYPE ==  _CMA__SVR4

    __hp_statxinit( exc_e_illopc, exc_s_illopc );
    __hp_statxinit( exc_e_illopn, exc_s_illopn );
    __hp_statxinit( exc_e_illadr, exc_s_illadr );
    __hp_statxinit( exc_e_illtrp, exc_s_illtrp );
    __hp_statxinit( exc_e_prvopc, exc_s_prvopc );
    __hp_statxinit( exc_e_prvreg, exc_s_prvreg );
    __hp_statxinit( exc_e_coproc, exc_s_coproc );
    __hp_statxinit( exc_e_badstk, exc_s_badstk );
    __hp_statxinit( exc_e_brkpt, exc_s_brkpt );
    __hp_statxinit( exc_e_trace, exc_s_trace );
    __hp_statxinit( exc_e_fltinv, exc_s_fltinv );
    __hp_statxinit( exc_e_maperr, exc_s_maperr );
    __hp_statxinit( exc_e_accerr, exc_s_accerr );
    __hp_statxinit( exc_e_adraln, exc_s_adraln );
    __hp_statxinit( exc_e_adrerr, exc_s_adrerr );
    __hp_statxinit( exc_e_objerr, exc_s_objerr );
    __hp_statxinit( exc_e_fltres, exc_s_fltres );

#endif /* _CMA_UNIX_TYPE ==  _CMA__SVR4 */

    __hp_statxinit( cma_e_alerted, cma_s_alerted );
    __hp_statxinit( cma_e_assertion, cma_s_assertion );
    __hp_statxinit( cma_e_badparam, cma_s_badparam );
    __hp_statxinit( cma_e_bugcheck, cma_s_bugcheck );
    __hp_statxinit( cma_e_exit_thread, cma_s_exit_thread );
    __hp_statxinit( cma_e_existence, cma_s_existence );
    __hp_statxinit( cma_e_in_use, cma_s_in_use );
    __hp_statxinit( cma_e_use_error, cma_s_use_error );
    __hp_statxinit( cma_e_wrongmutex, cma_s_wrongmutex );
    __hp_statxinit( cma_e_stackovf, cma_s_stackovf );
    __hp_statxinit( cma_e_nostackmem, cma_s_nostackmem );
    __hp_statxinit( cma_e_notcmastack, cma_s_notcmastack );
    __hp_statxinit( cma_e_unimp, cma_s_unimp );
    __hp_statxinit( cma_e_inialrpro, cma_s_inialrpro );
    __hp_statxinit( cma_e_defer_q_full, cma_s_defer_q_full );
    __hp_statxinit( cma_e_signal_q_full, cma_s_signal_q_full );
    __hp_statxinit( cma_e_alert_nesting, cma_s_alert_nesting );

    return;
}

/*
 *  Test whether the current threads TCB is locked. This is used
 *  by signal handlers to test for recursive locking of the TCB.
 */
extern int cma__hp_self_tcb_locked()
{
    cma__t_int_tcb      *tcb;

    cma__int_init ();

    tcb = cma__get_self_tcb ();
    return cma__int_mutex_locked(tcb->mutex);
}

/*
 *  Test whether the CMA kernel is locked. This is used
 *  by signal handlers to test for recursive locking of the kernel.
 */
extern int cma__hp_kernel_locked()
{
    return (cma__tac_isset (&cma__g_kernel_critical));
}

#endif	/* __hpux */

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_INIT.C */
/*  *30   24-APR-1992 17:04:11 SCALES "Unconditionalize signal module init" */
/*  *29   17-APR-1992 11:11:19 BUTENHOF "Add version number string" */
/*  *28   16-APR-1992 06:02:02 BUTENHOF "Add string template for version" */
/*  *27    2-APR-1992 21:30:18 BUTENHOF "Use crt0.o hooks on OSF/1" */
/*  *26   19-MAR-1992 13:17:25 BUTENHOF "Add debugging hooks for libc_r.a mutex code" */
/*  *25   13-MAR-1992 14:08:46 BUTENHOF "Fix trace sequence" */
/*  *24   18-FEB-1992 15:29:11 BUTENHOF "Adapt to new alloc_mem protocol" */
/*  *23   24-JAN-1992 10:02:16 BUTENHOF "Since I made it possible for Alpha open rtl, do static exception init" */
/*  *22   19-DEC-1991 13:08:28 BUTENHOF "Fix trace file init" */
/*  *21   18-DEC-1991 08:55:01 BUTENHOF "Fix a type problem" */
/*  *20   18-DEC-1991 06:45:16 BUTENHOF "Fix exception initialization" */
/*  *19   26-NOV-1991 16:15:27 BUTENHOF "Fix some conditionals" */
/*  *18   25-NOV-1991 14:00:06 BUTENHOF "Add cma_g_debug_cmd" */
/*  *17    5-NOV-1991 14:58:50 BUTENHOF "Integrate Dave Weisman's changes" */
/*  *16   16-OCT-1991 10:00:42 BUTENHOF "Add a typecast" */
/*  *15   14-OCT-1991 13:38:49 BUTENHOF "Refine/fix use of config symbols" */
/*  *14   24-SEP-1991 16:27:18 BUTENHOF "Get count of file descriptors early" */
/*  *13   17-SEP-1991 13:23:36 BUTENHOF "Get count of file descriptors early" */
/*  *12   10-JUN-1991 18:22:01 SCALES "Add sccs headers for Ultrix" */
/*  *11   10-JUN-1991 17:54:36 SCALES "Conditionalize inclusion of I/O stuff" */
/*  *10    6-JUN-1991 11:20:38 CURTIN "Added to init process, cma__init_atfork" */
/*  *9     7-MAY-1991 10:06:13 CURTIN "Reworked cma_init to use new cma__int_init macro" */
/*  *8     2-MAY-1991 13:58:22 BUTENHOF "Standardize trace messages" */
/*  *7    12-APR-1991 23:35:52 BUTENHOF "Mach support" */
/*  *6     1-APR-1991 18:08:53 BUTENHOF "Exception changes" */
/*  *5    21-MAR-1991 09:26:19 BUTENHOF "Fix up cma_debug access" */
/*  *4    24-JAN-1991 20:50:50 BUTENHOF "Fix status names in exception initializers" */
/*  *3    24-JAN-1991 11:40:27 CURTIN "removed include of cma_client.h" */
/*  *2    24-JAN-1991 00:34:54 BUTENHOF "Get rid of cma_client module" */
/*  *1    12-DEC-1990 21:46:23 BUTENHOF "CMA initializer" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_INIT.C */
