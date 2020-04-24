/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: cma_px.c,v $
 * Revision 1.1.4.2  1996/02/17  23:33:27  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:40  marty]
 *
 * Revision 1.1.4.1  1995/12/07  21:56:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:27  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:52:45  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:49:43  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/14  21:31:43  ohara
 * 	new file for OSF tree
 * 	[1994/04/14  20:36:49  ohara]
 * 
 * Revision 1.1.2.5  94/03/24  18:56:06  maher
 * 	insetp is incorrectly declared when __STDC__ is defined
 * 	[94/03/24  16:52:03  maher]
 * 
 * Revision 1.1.2.3  94/03/22  08:25:50  maher
 * 	sigprocmask now calls __nr_sigaction() rather than __r_sigaction()
 * 	[94/03/22  08:17:33  maher]
 * 
 * 	created file -- added wrapper for sigprocmask() (see CR# 690)
 * 	[94/03/10  12:15:15  maher]
 * 
 * Revision 1.2  1993/09/17  22:41:42  bhandaru
 * 	#000208 (usl_port)
 * 	Fix cma_spm.c sigprocmask wrapper to handle the case
 * 	where input and output sigset_t pointers are same !
 * 
 * Revision 1.1  1993/09/09  21:32:26  bhandaru
 * 	#000169 (usl_port)
 * 	Fixes for cma-7, and defining _FTX in dce.h if
 * 	necessary
 * 
 * $EndLog$
 */


#ifndef lint
static char	*sccsid = "%W%	(DEC OSF/1)	%G%";
#endif 


/*
 *  INCLUDE FILES
 */


#include <cma.h>
#include <cma_init.h>
#include <exc_handling.h>
#include <errno.h>

#if (_CMA_UNIX_TYPE == _CMA__SVR4)
# include <cma_px_redefs.h>
#endif

/*
 * Global data
 */


/* Following array has all the asynchronous signals ignored by 
 * default ( => non-terminating ).
 * viz: SIGCHLD SIGPWR SIGWINCH SIGURG SIGCONT
 */

static int cma__async_ignsigarr[] = { SIGCHLD, SIGPWR, SIGWINCH, SIGURG, SIGCONT  };





/* Enable this code by default - whether _CMA_NOWRAPPERS_
 * is defined or not. Redefinition of sigprocmask occurs
 * in a cma_px.h only if _CMA_NOWRAPPERS_ is not defined
 * and CMA_SIGPROCMASK is defined. This way, this 
 * sigprocmask wrapper would be dormant unless the
 * app explicitly requests it. It may be enabled by
 * default for apps by defining CMA_SIGPROCMASK in
 * dce.h conditional #ifndef _CMA_NOWRAPPERS_
 */


/*
 *  FUNCTIONAL DESCRIPTION: 	A dummy signal handler 
 *  FORMAL PARAMETERS: 		nono
 *  IMPLICIT INPUTS: 		none
 *  IMPLICIT OUTPUTS: 		none
 *  FUNCTION VALUE:
 *
 *  SIDE EFFECTS:
 */


#ifdef _CMA_PROTO_
static void cma___sigprocmask_shandler (int sig)
#else
static void cma___sigprocmask_shandler (sig)
	int sig;
#endif
{
  return;
}

/*
 *  FUNCTIONAL DESCRIPTION: 	
 *
 * 		Wrapper for sigprocmask, that effectively blocks
 * 		async signals. This is done to prevent a race
 * 		condition between an async signal and sigwait
 *
 * 
 *			
 *  FORMAL PARAMETERS:
 *				same as sigprocmask(2)
 *  IMPLICIT INPUTS: 		
 *  IMPLICIT OUTPUTS: 		
 *  FUNCTION VALUE:
 *
 *  SIDE EFFECTS:
 * 	Note/Caution:
 * 		Eventhough this wrapper and sigprocmask(2) could be called
 * 		to mask non-async signals, I suspect there will be undesirable effects if a
 * 		user sets the mask to block any signal other than async
 * 		signals to be sigwaited upon - Because in DCE threads model,
 * 		signals are mapped to exceptions and delivered to threads.
 */


int cma_sigprocmask ( int h, const sigset_t *insetp, sigset_t *osetp )
{
  int s, st, i;
  sigset_t oset; /* saved value of output and output sets */
  sigset_t iset;
  sigset_t *isetp = (sigset_t *)insetp; /* need a non-const pointer */

  cma__int_init ();

  /* Issue the op. Dont change the code below unless you know
   * what you are doing. This ensures that we provide the
   * same behavior as sigprocmask(2) for various combinations
   * of isetp and osetp, including the case isetp == osetp
   */

  st = __nr_sigprocmask(h,isetp,( osetp && isetp == osetp )?&oset:osetp);

  if ( st == -1 || isetp == NULL )
     return st;

  if ( osetp && isetp == osetp ) {
       extern int errno;
       /* Save input set before overwriting */
       iset = *isetp;
       isetp = &iset;
    TRY
      *osetp = oset;
    CATCH_ALL
      errno = EFAULT;
      return -1;
    ENDTRY
   }
  
  /* We have non-NULL input set here */
  i=0;
  while ( i < sizeof(cma__async_ignsigarr)/sizeof(int) ) {
    struct sigaction a;
    
    s = cma__async_ignsigarr[i++];
    
    /* We only care about signals that are 
     *   - present in cma__async_ignsigarr and
     *   - present in set pointed to by isetp and
     *   - The signal handler for this signal is SIG_DFL or
     *     the handler registered by this function using 
     *     sigaction. 
     */

    if ( ( h == SIG_SETMASK || h == SIG_BLOCK ) &&
	sigismember(isetp,s) ) {
      
      if ( __nr_sigaction(s,NULL,&a) == -1 ) 
	cma__bugcheck("cma_sigprocmask:1 sigaction returns -1");

	/* Note that we dont check for SIG_IGN below, as
	 * application MUST use sigwait to deal with an
	 * asynchronous signal and SHOULD NOT register
	 * singal handlers for these. In any case, an
	 * applications that asks these signals to be
	 * ignored and wants to mask them is necessarily
	 * confused. In that case, this wrapper has no
	 * effect
	 */

      if ( a.sa_handler == SIG_DFL ) {
	/* sigaction to install our dummy handler. */
	a.sa_handler = cma___sigprocmask_shandler;
	if (__nr_sigaction(s,&a,NULL) == -1) 
		cma__bugcheck("cma_sigprocmask:2 sigaction returns -1");
      }
    } else if ( ( h == SIG_UNBLOCK && sigismember(isetp,s)) ||
	       (h == SIG_SETMASK && !sigismember(isetp,s) ) ) {
      
      if ( __nr_sigaction(s,NULL,&a) == -1 ) 
	cma__bugcheck("cma_sigprocmask:3 sigaction returns -1");
      
      if (a.sa_handler == cma___sigprocmask_shandler ) {
	/* sigaction to replace our handler */
	a.sa_handler = SIG_DFL;
	if ( __nr_sigaction(s,&a,NULL) == -1 ) 
	  cma__bugcheck("cma_sigprocmask:4 sigaction returns -1");
      }
    }
  }
  
  return st;
}
