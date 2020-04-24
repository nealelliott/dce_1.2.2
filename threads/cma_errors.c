/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_errors.c,v $
 * Revision 1.1.19.1  1996/05/10  13:36:49  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:41 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:55  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  12:40:13  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:29  root  1.1.9.3]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:13:46  root]
 *
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:43:37  hinman]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:54:46  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:26:55  osfrcs  1.1.1.8]
 *
 * 	DCE 1.0.1 Release
 * 	[1992/09/23  20:37:51  hopkins]
 *
 * Revision 1.1.10.2  1994/06/09  13:37:59  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:56  devsrc]
 * 
 * Revision 1.1.10.1  1994/04/01  20:16:20  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:30  jd]
 * 
 * Revision 1.1.7.2  1993/06/24  21:00:25  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:43:37  hinman]
 * 
 * Revision 1.1.4.4  1993/02/01  22:17:59  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:26:07  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:19:50  hinman]
 * 
 * Revision 9.5.1.3  92/12/17  13:37:49  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.2.2  92/11/06  11:16:55  raj
 * 	[raj] Dont lock kernel if doing bugcheck. See note in cma_bugcheck().
 * 
 * Revision 9.5.1.2  92/09/10  16:32:28  hinman
 * 	[hinman] - Fold forward from raj_om
 * 
 * Revision 9.5.2.2  92/09/10  16:01:48  raj
 * 	[raj] merging SVR4 changes into 1.0.2
 * 
 * Revision 9.9  92/09/02  14:03:02  hinman
 * 	Update checkin
 * 
 * Revision 1.1.2.3  1992/08/14  20:21:41  bolinger
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  20:55:52  bolinger]
 * 
 * 	Change "DECThreads" to "DCE Threads" wherever visible to programmer
 * 	or application.
 * 	[1992/08/10  15:22:35  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:21:18  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:21:00  bolinger]
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
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	This module is the implementation of
 * 	the platform-specific error reporting mechanism.
 *
 *  AUTHORS:
 *
 *	Robert A. Conti
 *
 *  CREATION DATE:
 *
 *	6 October 1989
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Webb Scales	20 October 1989
 *		ifdef globalvalue & delete call to cma__error(cma$_bugcheck)
 *	002	Webb Scales	20 October 1989
 *		ifdef the refernences to lib$stop, too!
 *	003	Bob Conti	10 December 1989
 *		Move cma__unimplemented here from cma_exception.c
 *	004	Bob Conti	10 December 1989
 *		Hook up to portable exceptions package for the non-VMS case.
 *	005	Dave Butenhof	14 December 1989
 *		Add cma__report_error to generate error message for an
 *		exception.
 *	006	Dave Butenhof	15 December 1989
 *		On UNIX, cma__report_error should report exception status
 *		value, not address, if it's status type (even though the
 *		value doesn't really MEAN anything on UNIX).
 *	007	Webb Scales	11 January 1990
 *		Added assertion that cma_error is called _outside_ the kernel.
 *		If we call cma_error while we are inside the kernel, the entire
 *		process will hang when we try to raise the exception, because
 *		we will attempt to reenter the kernel to get the thread's tcb.
 *	008	Dave Butenhof	29 May 1990
 *		Clean up by removing #include for starlet.h on VMS (it's not
 *		needed anymore, and complicates SCA searching).
 *	009	Webb Scales	15 August 1990
 *		Replaced an #ifdef unix with an #if _CMA_OS_ == _CMA__UNIX
 *	010	Paul Curtin	15 October 1990
 *		Made cma$_bugcheck cma$s_bugcheck
 *	011	Dave Butenhof	25 March 1991
 *		Change from cma_exception to exc_handling
 *	012	Dave Butenhof	09 April 1991
 *		Don't reference kernel_critical flag directly
 *	013	Dave Butenhof	01 May 1991
 *		Add string argument to cma__bugcheck() (written out before
 *		raising exception).
 *	014	Dave Butenhof	08 May 1991
 *		Use new macro to test for kernel already locked, which
 *		evaporates under multiprocessor implementation.
 *	015	Dave Butenhof	11 June 1991
 *		Change cma__bugcheck() to call formatting functions for
 *		kernel and semaphore tracing if the appropriate conditionals
 *		are set.
 *	016	Paul Curtin	18 November 1991
 *		Added the use of an Alpha switch.
 *	017	Dave Butenhof	27 November 1991
 *		Remove globalvalue of cma$s_bugcheck.
 *	018	Dave Butenhof	16 December 1991
 *		Add null argument count to cma__error's lib$stop.
 *	019	Dave Butenhof	27 January 1992
 *		Add more nulls to cma__error's lib$stop, to match
 *		exc_handling's exc_raise_status.
 *	020	Dave Butenhof	28 January 1992
 *		Modify lib$stop local prototype to avoid comp. errors in 019.
 *	021	Dave Butenhof	17 March 1992
 *		Major revision to bugcheck: abort execution immediately
 *		rather than raising an exception (which obscures the state
 *		information and also won't work if the bugcheck occurred
 *		while the DECthreads kernel is locked!) This is only the
 *		beginning: there should be a full state report, optionally to
 *		an output file, as well as the core dump for more detailed
 *		poking about.
 *	022	Dave Butenhof	23 March 1992
 *		Change bugcheck to support argument list.
 *	023	Dave Butenhof	01 April 1992
 *		Change text of bugcheck message -- instead of calling it an
 *		"internal error", call it an "internal problem". The DTM
 *		test set thinks any output with "error" is a failure, making
 *		it difficult to pass a bugcheck test.
 *	024	Dave Butenhof	17 April 1992
 *		Add DECthreads version to bugcheck message.
 *	025	Dave Butenhof	08 June 1992
 *		Add current thread to bugcheck trace file.
 *	026	Dave Butenhof	10 June 1992
 *		Add #include for cma_kernel.h to get try_enter_kernel().
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_errors.h>
#include <cma_kernel.h>
#include <cma_dispatch.h>
#include <cma_stack.h>
#include <cma_init.h>
#include <cma_debugger.h>
#if _CMA_OS_ == _CMA__UNIX
# include <stdio.h>
#include <cma_signal.h>
#endif
#include <cma_util.h>
# include <stdarg.h>


/*
 * GLOBALS
 */

/*
 * LOCAL DATA
 */

static cma__t_file	cma___g_dumpfile;

/*
 * LOCAL FUNCTIONS
 */


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine is called to report that a major internal inconsistency
 *	has been detected in CMA during execution of the production version.
 *
 *      The user action associated with a call to this routine is to submit
 *  	an SPR, so this routine should only be used when an internal bug
 *	is suspected, rather than a client-caused error.
 *
 *	Note that this routine cannot be "macro-ized" as it is called from
 *	assembly code.
 *
 *  FORMAL PARAMETERS:
 *
 *	text	Text describing error circumstances
 *	args	Variable printf-style arglist for text (format string).
 *
 *  IMPLICIT INPUTS:
 *
 *	NONE
 *
 *  IMPLICIT OUTPUTS:
 *
 *	NONE
 *
 *  FUNCTION VALUE:
 *
 *	NONE
 *
 *  SIDE EFFECTS:
 *
 *	The bugcheck is reported as appropriate to the current host.
 *	Typically, an exception is raised in the current thread.
 */
static void cma___dump_eol(char	*buffer)
    {
# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma__int_output (cma___g_dumpfile, "%s\n", buffer);
#else
    cma__int_fprintf (cma___g_dumpfile, "%s\n", buffer);
# endif /* _CMA__SVR4 */
    }

extern void
cma__bugcheck(char *text,
	...)
    {
    char		buffer[512];
    cma__t_file		errfile;
    cma__t_int_tcb	*self;
    cma__t_eol_routine	save_eol;
    va_list		ap;
    static char		*log_file = "cma_dump.log";
    static char		*thdcmd[4] = {"thread","-i","-a","-f"};
    static char		*mutcmd[2] = {"mutex","-q"};
    static char		*concmd[2] = {"condition","-q"};
    static char		*shocmd[2] = {"show","-m"};
#ifdef _CMA_TRACE_KERNEL_
    static char		*shkcmd[2] = {"show","-k"};
#endif
#ifdef _CMA_TRACE_SEM_
    static char		*shscmd[2] = {"show","-s"};
#endif
#if _CMA_KTHREADS_ != _CMA__NONE
    static char		*shvcmd[2] = {"show","-v"};
#endif   
    va_start (ap, text);


    if (text != (char *)cma_c_null_ptr)
	cma__int_vsprintf (buffer, text, ap);
    else
	cma__int_sprintf (buffer, "unknown.");

    va_end (ap);
# if _CMA_UNIX_TYPE == _CMA__SVR4
    errfile = cma__int_output_open ("stderr", "w");
    cma__int_output (
#else
    errfile = cma__int_fopen ("stderr", "w");
    cma__int_fprintf (
# endif /* _CMA__SVR4 */
	    errfile,
	    "%%Internal DCE Threads problem (version %s), terminating execution.\n",
	    cma__g_version);
# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma__int_output (
#else
    cma__int_fprintf (
# endif /* _CMA__SVR4 */
	    errfile,
	    "%% Reason: %s\n", buffer);
# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma__int_output (
# else
    cma__int_fprintf (
# endif /* _CMA__SVR4 */
	    errfile,
	    "%% See '%s' for state information.\n", log_file);

# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma___g_dumpfile = cma__int_output_open (log_file, "w");
    cma__int_output (
# else
    cma___g_dumpfile = cma__int_fopen (log_file, "w");
    cma__int_fprintf (
# endif /* _CMA__SVR4 */
	    cma___g_dumpfile,
	    "%%Internal DCE Threads problem (version %s), terminating execution.\n",
	    cma__g_version);
# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma__int_output (
# else
    cma__int_fprintf (
# endif /* _CMA__SVR4 */
	    cma___g_dumpfile,
	    "%% Reason: %s\n", buffer);
    self = cma__get_self_tcb ();
# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma__int_output (
# else
    cma__int_fprintf (
# endif /* _CMA__SVR4 */
	    cma___g_dumpfile,
	    "The current thread is %d (address 0x%08x)\n",
	    self->header.sequence, (long) self);

#  if _CMA_UNIX_TYPE != _CMA__SVR4
    /*
     * Try to lock the kernel -- but continue anyway. Since we're
     * bugchecking, even if this is a multiprocessor we can't count on
     * another thread every RELEASING the kernel (plus, we can't tell whether
     * the current thread might hold the lock). Ignoring an existing MP lock
     * is potentially dangerous -- but we're bugchecking anyway, so what are
     * we supposed to do but our best?
     */
    if (! cma__tryenter_kernel ())
	cma__int_fprintf (
		cma___g_dumpfile,
		"DECthreads scheduling database is locked.\n");

#else

/* With the SVR4 reentrant library implementation We dont locking the kernel 
 * in SVR4 because of the following scenario
 * lock kernel and cma__int_fprintf -> non-reentrant printf -> write 
 * -> cma_write -> cma__io_wait which tries to lock the kernel and bugchecks.
 * The long term solution of removing library dependencies from cma.
 */

# endif /* _CMA_UNIX_TYPE = _CMA__SVR4 */

    cma__set_eol_routine (cma___dump_eol, &save_eol);
# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma__int_output (cma___g_dumpfile, "Current threads:\n");
# else
    cma__int_fprintf (cma___g_dumpfile, "Current threads:\n");
# endif /* _CMA__SVR4 */
    cma__debug_list_threads (sizeof(thdcmd)/sizeof (char *), thdcmd);
# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma__int_output (cma___g_dumpfile, "Mutexes:\n");
# else
    cma__int_fprintf (cma___g_dumpfile, "Mutexes:\n");
# endif /* _CMA__SVR4 */
    cma__debug_list_mutexes (sizeof(mutcmd)/sizeof (char *), mutcmd);
# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma__int_output (cma___g_dumpfile, "Condition variables:\n");
# else
    cma__int_fprintf (cma___g_dumpfile, "Condition variables:\n");
# endif /* _CMA__SVR4 */
    cma__debug_list_cvs (sizeof (concmd)/sizeof (char *), concmd);
    cma__debug_show (sizeof (shocmd)/sizeof (char *), shocmd);
#ifdef _CMA_TRACE_KERNEL_
# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma__int_output (cma___g_dumpfile, "Kernel trace info:\n");
# else
    cma__int_fprintf (cma___g_dumpfile, "Kernel trace info:\n");
# endif /* _CMA__SVR4 */
    cma__debug_show (sizeof (shkcmd)/sizeof (char *), shkcmd);
#endif
#ifdef _CMA_TRACE_SEM_
# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma__int_output (cma___g_dumpfile, "Semaphore trace info:\n");
# else
    cma__int_fprintf (cma___g_dumpfile, "Semaphore trace info:\n");
# endif /* _CMA__SVR4 */
    cma__debug_show (sizeof (shscmd)/sizeof (char *), shscmd);
#endif
#if _CMA_KTHREADS_ != _CMA__NONE
# if _CMA_UNIX_TYPE == _CMA__SVR4
    cma__int_output (cma___g_dumpfile, "Kernel threads:\n");
# else
    cma__int_fprintf (cma___g_dumpfile, "Kernel threads:\n");
# endif /* _CMA__SVR4 */
    cma__debug_show (sizeof (shvcmd)/sizeof (char *), shvcmd);
#endif
    cma__set_eol_routine (save_eol, (cma__t_eol_routine *)0);
#if _CMA_OS_ == _CMA__VMS
    cma__abort ();
#else
# ifndef _HP_LIBC_R
    fflush (cma___g_dumpfile);
# endif
    cma__abort_process (SIGIOT);	/* Abort with core dump */
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine is called to report that a client-caused error
 *	has been detected in CMA during execution of the production version.
 * 	In other words, to report errors that are defined by the CMA
 *	architecture specification.
 *
 *      The user action associated with a call to this routine depends on
 *  	the particular status code passed in.  Be sure to use the
 *	appropriate status code and/or modify the manual text in the file
 *	CMA_MESSAGES*.GNM so that when the client gets the error it is
 *	clear what action is to be taken.
 *
 *  FORMAL PARAMETERS:
 *
 *	code  	An integer "status code" in a format appropriate to the
 *		current host.
 *
 *  IMPLICIT INPUTS:
 *
 *	NONE
 *
 *  IMPLICIT OUTPUTS:
 *
 *	NONE
 *
 *  FUNCTION VALUE:
 *
 *	NONE
 *
 *  SIDE EFFECTS:
 *
 *	The error is reported as appropriate to the current host.
 *	Typically, an exception is raised in the current thread.
 */
extern void
cma__error(
	cma_t_status	code)
    {
#if _CMA_OS_ == _CMA__VMS
    extern void
    lib$stop (cma_t_status,cma_t_integer,cma_t_integer,cma_t_integer);

    /*
     * Signal the error, adding several 0s. The first specifies that the
     * condition code has no arguments. The second specifies a secondary
     * condition code of "0" (with sys$putmsg will ignore); otherwise,
     * sys$putmsg might find non-zero codes in whatever happens to follow the
     * argument list on the stack. The third should be unnecessary, but
     * provides a safety net (either an addition null status code, or an
     * argument count of 0 to the first null status code, depending on
     * implementation).
     */
    lib$stop (code, 0, 0, 0);
#else
    cma__assert_not_kernel ();
    exc_raise_status (code);
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This function is used by the transfer vector to report unimplemented
 *	entry points.
 *
 *  FORMAL PARAMETERS:
 *
 * 	none
 *
 *  IMPLICIT INPUTS:
 *
 * 	none
 *
 *  IMPLICIT OUTPUTS:
 *
 * 	none
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
cma__unimplemented(void)
    {
    cma__error (cma_s_unimp);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ERRORS.C */
/*   23    4-AUG-1992 11:04:52 BUTENHOF "64 bit address printout" */
/*   22   17-JUL-1992 17:31:23 KEANE "Fix report attr. object display" */
/*   21   10-JUL-1992 07:59:41 BUTENHOF "Report attr. objs on bucheck" */
/*   20   16-JUN-1992 06:49:00 BUTENHOF "Fix bugcheck code" */
/*  *19   10-JUN-1992 14:03:04 BUTENHOF "Fix enter kernel code" */
/*  *18    9-JUN-1992 12:10:46 BUTENHOF "Add current thread on bugcheck" */
/*  *17   17-APR-1992 11:11:02 BUTENHOF "Add version number string" */
/*  *16    1-APR-1992 07:53:52 BUTENHOF "Remove ""error"" from bugcheck -- for DTM" */
/*  *15   24-MAR-1992 13:46:19 BUTENHOF "Put bugcheck output in file" */
/*  *14   17-MAR-1992 09:55:59 BUTENHOF "Abort process on bugcheck" */
/*  *13   28-JAN-1992 06:40:17 BUTENHOF "Fix compilation error" */
/*  *12   27-JAN-1992 05:50:00 BUTENHOF "Pad lib$stop in cma__error with nulls" */
/*  *11   16-DEC-1991 12:55:19 BUTENHOF "Specify 0 argument count on cma__error()" */
/*  *10   27-NOV-1991 11:03:35 BUTENHOF "Remove globalvalue on cma$s_bugcheck" */
/*  *9    18-NOV-1991 11:07:55 CURTIN "Added the use of an Alpha switch." */
/*  *8    11-JUN-1991 17:16:55 BUTENHOF "Add & use functions to dump kernel/sem trace arrays" */
/*  *7    10-JUN-1991 18:21:44 SCALES "Add sccs headers for Ultrix" */
/*  *6    29-MAY-1991 17:14:38 BUTENHOF "Put some meat on the bugcheck printout" */
/*  *5    10-MAY-1991 17:52:01 BUTENHOF "Use new macro to test for kernel locked" */
/*  *4     2-MAY-1991 13:58:12 BUTENHOF "Add string argument to cma__bugcheck()" */
/*  *3    12-APR-1991 23:35:49 BUTENHOF "Change references to kernel_critical" */
/*  *2     1-APR-1991 18:08:29 BUTENHOF "QAR 93, exception text" */
/*  *1    12-DEC-1990 21:45:26 BUTENHOF "Error handling" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ERRORS.C */
