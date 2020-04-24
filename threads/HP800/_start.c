/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: _start.c,v $
 * Revision 1.1.2.2  1996/10/03  15:16:23  arvind
 * 	Add OSF copyright
 * 	[1996/05/24  23:18 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.2.1  1996/06/04  22:15:33  arvind
 * 	Merge in changes to CHFts14939 and CHFts14807 from branch jss_mothra_fix3.
 * 	[1995/04/26  20:04 UTC  jss  /main/HPDCE02/6]
 * 
 * 	Fix for CHFts14939.  Call cma__pre_init_libc() to support single threaded
 * 	client changes.
 * 	[1995/04/05  22:05 UTC  jss  /main/HPDCE02/jss_mothra_fix3/1]
 * 
 * 	stop VTALARM signal for the remainder of the exit process.
 * 	this is conditionalized just for vista builds.
 * 	[1995/02/16  13:59 UTC  root  /main/HPDCE02/5]
 * 
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:36 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  20:01 UTC  jss  /main/HPDCE02/3]
 * 
 * 	Changes for single threaded client.
 * 	[1994/12/15  15:42 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 * 
 * 	Merge singlestack code.
 * 	[1994/10/27  21:06 UTC  chris_p  /main/HPDCE02/2]
 * 
 * 	Change symbol names with "carevue" to "singlestack"
 * 	[1994/10/17  20:30 UTC  chris_p  /main/HPDCE02/proven_threads_940803/3]
 * 
 * 	Fix a few minor problems
 * 	[1994/10/13  20:13 UTC  chris_p  /main/HPDCE02/proven_threads_940803/2]
 * 
 * 	Moved CAREVUE init code here.
 * 	[1994/10/06  21:54 UTC  chris_p  /main/HPDCE02/proven_threads_940803/1]
 * 
 * 	fix broken conditional
 * 	[1994/06/23  13:44 UTC  rmm  /main/HPDCE01/3]
 * 
 * 	fix broken conditional
 * 	[1994/06/23  13:37 UTC  rmm  /main/HPDCE01/rmm_rodan_work/2]
 * 
 * 	change module name to libcma since cma is standalone now
 * 	[1994/06/22  20:49 UTC  rmm  /main/HPDCE01/rmm_rodan_work/1]
 * 
 * 	Removed extra characters after #endif compiler
 * 	warnings by putting comment marks around them.
 * 	[1993/08/16  18:31:21  truitt]
 * 
 * 	*
 * 
 * 	Initial King Kong branch
 * 	[1993/07/08  17:25:40  root]
 * 
 * 	Included domestic vs. export in libdce rev_ctl.
 * 	[1993/06/14  19:52:57  tatsu_s]
 * 
 * 	Add libdce versioning code
 * 	[1993/06/02  18:30:25  hopkins]
 * 
 * 	Initial GAMERA branch
 * 	[1993/03/26  13:01:24  mgm  1.1.5.2]
 * 
 * 	Change covdump type declaration from void to int
 * 	[1992/09/18  19:56:15  pare]
 * 
 * 	Enable ViSTA code coverage functions used for instrumenting code
 * 	[1992/09/18  17:11:01  pare]
 * 
 * 	Initial revision
 * 	[1992/06/18  22:18:05  rose_l]
 * 
 * $EndLog$
 */
/*
*/
/* $Source: /u0/rcs_trees/dce/rcs/threads/HP800/_start.c,v $
 * $Revision: 1.1.2.2 $	$Author: arvind $
 * $State: Exp $   	$Locker:  $
 * $Date: 1996/10/03 15:16:23 $
 *
 * Revision 1.2  92/05/07  15:31:47  15:31:47  dtonogai (Dale Tonogal)
 * Set _NAMESPACE_CLEAN if _HP_LIBC_R defined
 * 
 * Revision 1.1  92/03/03  11:14:12  11:14:12  dtonogai (Dale Tonogal)
 * Initial revision
 * 
 * Revision 1.1  91/12/03  15:31:34  15:31:34  dtonogai (Dale Tonogal)
 * initial
 * 
 * Revision 1.1  91/06/14  15:54:49  15:54:49  dtonogai (Dale Tonogai)
 * Initial revision
 * 
 * Revision 64.6  89/03/10  11:14:14  11:14:14  ssa
 * Moved definition of environ from _start.c to environ.c.
 * 
 * Revision 64.5  89/02/04  15:50:12  15:50:12  cary
 * Added __argc_value and __argv_value for s300 compatibility.
 * 
 * Revision 64.4  89/02/03  09:11:48  09:11:48  ddg (Dave Grindeland[Ft.Collins])
 * gave environ an initial value and pragma'd it
 * 
 * Revision 64.3  89/01/19  14:08:43  14:08:43  marklin (Mark Lin)
 * Namespace cleanup using _NAMESPACE_CLEAN flag.
 * 
 * Revision 62.1  88/09/13  16:14:34  16:14:34  gnuss (Greg Nuss)
 * Call sigsetreturn() at start of C code so kernel always knows address of
 * signal trampoline code.
 * 
 * Revision 1.10  86/05/15  11:06:15  11:06:15  yohn
 * <sys/vers.h> is non-file now...
 * 
 * Revision 1.9  86/02/05  13:09:38  13:09:38  yohn (Michael Yohn Thompson)
 * environ no longer statically initialized to zero (this is
 * the default value anyway).  This allows the Fortran and
 * Pascal startup code to declare environ, also -- the linker
 * will just fold these globals together.
 * 
 * Revision 1.8  85/10/30  13:26:31  ss (Steve Serocki)
 * plugged in buffer allocation and cleanup calls for the profilers.
 * -DMCRT0 turns it all on.
 * 
 * Revision 1.7  85/10/11  09:53:34  yohn (Michael Y. Thompson)
 * don't include <sys/vers.h> for comet.
 * 
 * Revision 1.6  85/08/13  15:10:50  cary (Cary Coutant)
 * Reverse last revision (return from main *should* be the exit status).
 * 
 * Revision 1.5  85/04/08  13:08:56  wallace (Kevin G. Wallace)
 * Set exit status to zero if main() returns.
 * 
 * Revision 1.4  84/10/18  10:08:28  jcm (J. C. Mercier)
 * Added include file vers.h to help identify synchronization
 * problems between commands and the kernel.
 * 
 * Revision 1.3  84/05/28  01:28:42  wallace (Kevin G. Wallace)
 * Changed name of start() to _start() so that it is not
 * as likely to conflict with a user symbol.  This problem
 * will be avoided if we write the whole thing in assembly
 * language or get a linker that can to incremental links.
 * 
 * Revision 1.2  84/04/02  10:18:48  mojo (Joe Moran)
 * Header added.  
 */

#ifdef _HP_LIBC_R
# ifndef _NAMESPACE_CLEAN
# define _NAMESPACE_CLEAN
# endif
#endif

#ifdef _NAMESPACE_CLEAN
#define exit ___exit
#define environ _environ
#endif

    /*
     * The next five lines are just to provide the right values for
     *  the _sigsetreturn() call.
     */
#include <signal.h>
#define MAGIC_COOKIE 0x06211988	/* magic cookie for future expansion of save
				   state.  This is put in for forward
				   compatibility */
extern void _sigreturn();

extern char **environ;

#ifdef __VISTA__
extern int covdump();
static int cma__cancel_timer_atexit();       
#endif

void * cma_singlestack_stackaddr;

#ifdef HP_CMA_SINGLESTACK_OPT
#ifdef HP_CMA_SINGLESTACK_TEST
int cma_singlestack_opt = 1;
#else
int cma_singlestack_opt;
#endif

#endif

int __argc_value = 0;
char **__argv_value = (char **)0;
 
_start(argc, argv, envp)
	int argc;
	char **argv;
	char **envp;
{
#ifdef HP_CMA_SINGLESTACK_OPT
    char addr;

    cma_singlestack_stackaddr = &(addr);
    cma_singlestack_stackaddr = (char *)((int)cma_singlestack_stackaddr & (~31));
#endif

	__argc_value = argc;
	__argv_value = argv;
	/*
	 * Only initialize environ if it's not already set.  Starting in 10.0,
	 * environ is initialized in crt0.o.  This for backward compatability.
	 */
#if __hpux1000p
	if (environ == (char **)0)
#endif
	    environ = envp;

	    /* Tell kernel where our signal return code is */
	_sigsetreturn(_sigreturn,MAGIC_COOKIE,sizeof(struct sigcontext));

#       ifdef MCRT0
	    monstartup( _start, &etext);
#       endif /* MCRT0 */

#ifdef _HP_LIBC_R
	hp__pre_init_libc();
#endif

#ifdef __VISTA__ 
        covinit((char *)0);
        atexit(covdump);
	atexit(cma__cancel_timer_atexit);
#endif
	exit(main(argc, argv, envp));
}

#ifdef MCRT0
/*ARGSUSED*/

#ifdef _NAMESPACE_CLEAN
#undef exit
#pragma _HP_SECONDARY_DEF ___exit exit
#define exit ___exit
#endif

exit( code)
     register code;
{
  monitor( 0);
  _cleanup();
  _exit( code);
}
#endif /* MCRT0 */

#ifdef __VISTA__ 
static int
cma__cancel_timer_atexit()        
{
  struct sigaction    act;

  act.sa_flags = 0;
  act.sa_handler = SIG_IGN;

  (void)_sigaction_sys (SIGVTALRM, &act, (struct sigaction *)0);
}
#endif


