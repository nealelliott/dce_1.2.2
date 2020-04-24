/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * HISTORY
 * $Log: crvb_sig_003.c,v $
 * Revision 1.1.67.2  1996/02/17  23:30:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:07  marty]
 *
 * Revision 1.1.67.1  1995/12/11  22:59:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:26  root]
 * 
 * Revision 1.1.64.1  1993/10/12  19:35:34  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:06  rhw]
 * 
 * Revision 1.1.5.4  1993/01/31  20:46:57  jd
 * 	Modified this test to check that sigaction works with sync term
 * 	signals. It used SIGPIPE.
 * 	[1993/01/31  19:48:16  jd]
 * 
 * Revision 1.1.5.3  1992/12/30  16:31:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:08:13  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  20:38:21  devsrc
 * 	[OT 5373] SNI/SVR4 Merge UPDATE: Fixed function prototype headers to eliminate
 * 	          compiler errors on AIX3.2
 * 	[1992/09/24  20:03:44  sekhar]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:32:34  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/11  17:16:37  bolinger
 * 	Fix OT defect 2895:  add test for whether CMA was built with
 * 	_CMA_SYNC_KILL_ code enabled.  If so, this test is dummied
 * 	out.
 * 	[1992/05/11  13:49:43  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and copied
 *  only in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without notice
 *  and should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CRVB_SIG_003
 *
 *  TEST TOPICS:
 *
 *	SIG  -  U*IX signal handling
 *
 *  ABSTRACT:
 *
 *	This program tests the CMA implementation of the POSIX
 *	'sigaction' routine.
 *	
 *
 *  AUTHORS:
 *
 *      Paul Curtin
 *
 *  CREATION DATE:
 *
 *	3 December 1990
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Paul Curtin	22 February 1991
 *		Added an additional signal SIGFPE.
 *	002	Dave Butenhof	28 February 1991
 *		Fix argument name mismatch in declarations
 *	003	Dave Butenhof	28 February 1991
 *		Make the test more portable by using setjmp/longjmp instead
 *		of incrementing PC field in sigcontext struct from signal
 *		handler.
 *	004	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#if _CMA_UNIX_TYPE == _CMA__SVR4
#include <dce/cma_defs.h>
#endif
#include <dce/cts_test.h>
#include <sys/types.h>
#include <signal.h>
#include <setjmp.h>
#if _CMA_UNIX_TYPE == _CMA__SVR4
#	include <ucontext.h>
#endif

#define ITERATIONS 100 
int fd1[2];
int fd2[2];

cma_t_integer	pipe1_count = 0, pipe2_count = 0;

void
pipe1_handler
        (
	int			sig,
	int			siginfo,
	struct sigcontext	*scp)
    {
    cts_comment ("SIGPIPE1 received");
    pipe1_count++;

    }

void
pipe2_handler
        (
	int			sig,
	int			siginfo,
	struct sigcontext	*scp)
    {
    cts_comment ("SIGPIPE2 received");
    pipe2_count++;

    }

int *thread1_addr;
static cma_t_address
pipe1_thread (cma_t_address arg)
    {
    struct sigaction vec;
    struct sigaction ovec;
    sigset_t	empty_mask;
    int		flags, i;

    pipe(fd1);
    close(fd1[0]);
    vec.sa_handler = pipe1_handler;
    vec.sa_mask = empty_mask;
    vec.sa_flags = flags;
    sigaction (SIGPIPE, &vec, &ovec);

    for(i = 0; i < ITERATIONS; i++) {
        write(fd1[1],"a",1);
    }

    return cma_c_null_ptr;

    }

static cma_t_address
pipe2_thread (cma_t_address arg)
    {
    struct sigaction vec;
    struct sigaction ovec;
    sigset_t	empty_mask;
    int		flags, i;

    pipe(fd2);
    close(fd2[0]);
    vec.sa_handler = pipe2_handler;
    vec.sa_mask = empty_mask;
    vec.sa_flags = flags;
    sigaction (SIGPIPE, &vec, &ovec);

    for(i = 0; i < ITERATIONS; i++) {
        write(fd2[1],"b",1); 
    }


    return cma_c_null_ptr;
    }

main ()
    {
    cma_t_thread       thread1, thread2;
    cma_t_integer      argument;
    cma_t_exit_status  status;
    cma_t_address      result;
	sigset_t		set;
    
    cts_test ("CRVB_SIG_003", "Test sigaction");

    cma_init ();

    cma_thread_create (
	    &thread1,
	    &cma_c_null,
	    pipe1_thread,
	    (cma_t_address)&argument);

    cma_thread_create (
	    &thread2,
	    &cma_c_null,
	    pipe2_thread,
	    (cma_t_address)&argument);

    cma_thread_join (&thread2, &status, &result);

    cma_thread_join (&thread1, &status, &result); 

    if (pipe1_count != ITERATIONS)
		cts_failed (
		"SIGPIPE1 handler executed %d times, not %d",
		pipe1_count,
		ITERATIONS);

    if (pipe2_count != ITERATIONS)
    	cts_failed (
		"SIGPIPE2 handler executed %d times, not %d",
		pipe2_count,
		ITERATIONS);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_SIG_003.C */
/*  *6    29-MAY-1991 18:02:27 BUTENHOF "Fix compilation errors" */
/*  *5    28-FEB-1991 12:05:29 BUTENHOF "Improve portability" */
/*  *4    28-FEB-1991 07:54:23 BUTENHOF "Fix signal tests for RIOS" */
/*  *3    22-FEB-1991 11:39:31 CURTIN "Added an additional case; fixed inf. loop" */
/*  *2    24-JAN-1991 20:51:31 BUTENHOF "Fix names" */
/*  *1    17-DEC-1990 17:02:07 CURTIN "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_SIG_003.C */
