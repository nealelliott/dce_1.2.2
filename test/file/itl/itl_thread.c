/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_thread.c,v $
 * Revision 1.1.411.1  1996/10/17  18:27:48  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:57  damon]
 *
 * Revision 1.1.406.1  1994/02/04  20:45:45  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:44  devsrc]
 * 
 * Revision 1.1.404.1  1993/12/07  17:44:56  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:30:11  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:31:34  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:11:13  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:23:36  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	 Selected comments:
 * 	   Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	   Initial checkin.
 * 	[1992/09/23  21:30:38  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: itl_thread.c,v 1.1.411.1 1996/10/17 18:27:48 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    SHARED   itlThread_PerformInit, itlThread_PerformStart, 
 *             itlThread_PerformCleanup
 *    
 *    EXPORT   itl_AddThreadState, itl_StartIndThread
 *    
 *    PRIVATE  GenericItlIndThread
 *    
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $TALog: itl_thread.c,v $
 * Revision 1.6  1994/11/01  22:30:11  cfe
 * Bring over the changes that the OSF made in going from their DCE 1.0.3
 * release to their DCE 1.1 release.
 * [from r1.4 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 *
 * Revision 1.4  1993/01/14  17:38:43  jaffe
 * revdrop of latest OSF copyright changes to these files
 * [from r1.3 by delta osf-revdrop-01-12-92, r1.1]
 *
 * Revision 1.1  1992/08/10  20:11:29  fred
 * Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * Initial checkin.
 *
 * Initial checkin.
 * [added by delta fred-checkin-itl, revision 1.1]
 *
 * Revision 1.3  1992/03/27  21:15:58  oz
 * Changes which were made before 1.0.1 and were not supposed to be
 * incorporated in 1.0.1 and therefore were not readied and were
 * not moved to CML with the rest of the file.
 *
 * These changes add tranC support for ITL as well as handling
 * transaction aborts (exceptions) in the lower level io modules.
 *
 * See General Comment Above
 * Was previously part of itl_io.c
 * [added by delta oz-2172-itl-support-tranc, revision 1.1]
 *
 * Revision 1.2  92/03/04  15:14:38  oz
 * 	 - Moved from itl_io.c to here:
 * 		 GenericItlIndThread
 * 		itl_StartIndThread
 * 	[92/02/24  08:30:44  oz]
 * 
 * 	Initial Checkin
 * 	[92/02/23  16:01:43  oz]
 * 
 */

/*
 * it_thread.c -- 
 *    The module that deals with ITL threads including starting
 *    independent and joinable threads as well as keeping the context,
 *    including the values of variables and the IO state, when starting
 *    new threads.
 *
 *
 */
/*
 *  When running with DCE, we use the exception handler to make sure
 *  our state is always consistent - ITL can be used in conjunction with
 *  TRAN-C which uses the DCE exception handler.
 *  We ALWAYS use the following syntax:
 *	TRY {
 *	  .
 *	  .
 *     } FINALLY {
 *        .
 *        .
 *     } ENDTRY;
 */

#ifndef ITL_WITHOUT_DCE
#include <dce/exc_handling.h>
#else
#define TRY
#define ENDTRY 
#define FINALLY
#endif /* ITL_WITHOUT_DCE */

#include <utils/itl/itl_private.h>


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 *
 *    Hooks to allow the user to initialize a thread,
 *    set variables after the thread is created and to
 *    clean up after the thread completes.
 *
 *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 */
typedef struct {
    void (*InitProc) _TAKES((pointer_t *ptr));
    void (*StartProc) _TAKES((pointer_t *ptr));
    void (*CleanupProc) _TAKES((pointer_t *ptr));
} itl_threadBlockProcs_t;

SHARED LATCH_TYPE threadBlockMutex;
PRIVATE int numThreadBlocks = 0;
PRIVATE itl_threadBlockProcs_t *threadBlockProcsP = NULL;

#define THREAD_INIT(i)    (threadBlockProcsP[(i)].InitProc)
#define THREAD_START(i)   (threadBlockProcsP[(i)].StartProc)
#define THREAD_CLEANUP(i) (threadBlockProcsP[(i)].CleanupProc)

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 * Function: itl_AddThreadState
 * 
 * Description:
 *     Adds another block of possible thread state to iterator
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT boolean_t itl_AddThreadState(InitProc, StartProc, CleanupProc)
    void (*InitProc) _TAKES((pointer_t *));
    void (*StartProc) _TAKES((pointer_t *));
    void (*CleanupProc) _TAKES((pointer_t *));
{
    LATCH_LOCK(&threadBlockMutex);

    if (numThreadBlocks++){
	ITL_UTIL_REALLOC(threadBlockProcsP, itl_threadBlockProcs_t *,
			 (numThreadBlocks * sizeof(itl_threadBlockProcs_t)));
    } else {
	ITL_UTIL_ALLOC(threadBlockProcsP, itl_threadBlockProcs_t *,
		       sizeof(itl_threadBlockProcs_t));
    }

    THREAD_INIT(numThreadBlocks-1) = InitProc;
    THREAD_START(numThreadBlocks-1) = StartProc;
    THREAD_CLEANUP(numThreadBlocks-1) = CleanupProc;

    LATCH_UNLOCK(&threadBlockMutex);

    return(FALSE);
}

/*
 *   itlThread_PerformInit - 
 *   Peform the threadInit callbacks registered for starting new threads.
 *   This routine is called before a thread is started.  It allows
 *   the thread that is starting the new thread to initialize data
 *   for the new thread based on values of the starting thread.
 *   The caller may issue any number of such callbacks.  This
 *   routine calls all currently registered callbacks.
 */
SHARED void itlThread_PerformInit(threadBlockP, numThreadsP)
  OUT pointer_t **threadBlockP;
  OUT int *numThreadsP;
{
    int i;
    LATCH_LOCK(&threadBlockMutex);
    *numThreadsP = numThreadBlocks;
    ITL_ALLOC(*threadBlockP, pointer_t *,
	      numThreadBlocks * sizeof(pointer_t));
    for(i=0; i < numThreadBlocks; i++) {
	(*THREAD_INIT(i))(&(*threadBlockP)[i]);
    }
    LATCH_UNLOCK(&threadBlockMutex);
}

/*
 *  itlThread_PerformStart:
 *  Perform the thread start callbacks registered for this thread.
 *  These callbacks are made after the thread is created but before
 *  it starts executing its destined routine.
 *  The callbacks are made after the Thread IO is initialized and
 *  immediately before the call the itl_CommandLoop is made.
 */
SHARED void itlThread_PerformStart(threadBlocks, numThreads)
  IN pointer_t *threadBlocks;
  IN int numThreads;
{
    int i;

    LATCH_LOCK(&threadBlockMutex);
    for (i=0; i<numThreads; i++) {
	(*THREAD_START(i))(&(threadBlocks[i]));
    }
    LATCH_UNLOCK(&threadBlockMutex);
}

/*
 *  Perform Thread Cleanup:
 *  Perform the thread cleanup callbacks registered for this thread.
 *  These callbacks are made after the thread exits.
 */
SHARED void itlThread_PerformCleanup(threadBlocks, numThreads)
  IN pointer_t *threadBlocks;
  IN int numThreads;
{
    int i;
    LATCH_LOCK(&threadBlockMutex);
    for (i=0; i<numThreads; i++) {
	(*THREAD_CLEANUP(i))(&(threadBlocks[i]));
    }
    ITL_FREE(threadBlocks);
    LATCH_UNLOCK(&threadBlockMutex);
}


typedef struct {
    int	numThreadBlocks;	/*  Number of thread blocks in array below  */
    pointer_t *threadBlocks;	/*  Array of extended thread state	    */
    void (*func) _TAKES((pointer_t argP));	/* The function to execute  */
    pointer_t argP;		/*  The pointer to pass to the function	    */
} funcInfo_t;

/*
 *  GenericItlIndThread
 *	An independent thread which runs a function under ITL.
 *	The thread is not joinable.  Therefore, it assumes that the data
 *	passed to it is dynamically allocated and frees it at the end.
 *	It wraps the function and supplies the new thread the ITL context.
 *
 *  Logic:
 *	- Set the current IO to be the parent IO
 *	- After the thread terminates:
 *	  . the strTheadInfo_t structure was dynamically allocated by
 *	    the caller and not freed, free it.
 */
PRIVATE void GenericItlIndThread _TAKES((pointer_t));
PRIVATE void GenericItlIndThread(ptr)
  IN pointer_t ptr;
{
    funcInfo_t *funcInfoP = (funcInfo_t *)ptr;

    itl_Trace(ITL_IO_TRACE, "Starting the independent itl thread.\n");

    itlThread_PerformStart(funcInfoP->threadBlocks,
			   funcInfoP->numThreadBlocks);

    TRY {
	/*
	 *  Execute the client's function
	 */
	(*(funcInfoP->func))(funcInfoP->argP);
    } FINALLY {
	/*
	 *  Let each thread block free itself
	 */
	itlThread_PerformCleanup(funcInfoP->threadBlocks,
				 funcInfoP->numThreadBlocks);
	ITL_FREE(funcInfoP);
    } ENDTRY;
    itl_Trace(ITL_IO_TRACE, "The thread completed.\n");
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_StartIndThread
 *      Start an itl thread (perform all the inializations, supply
 *	the IO information as well as the parent's info such
 *	as the parent's variable values).
 *	The thread performs the provides function and provides the
 *	function with the given parameter.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT void itl_StartIndThread(fn, argP)
  IN void (*fn) _TAKES((pointer_t));
  IN pointer_t argP;
{
    funcInfo_t *funcInfoP;
    itl_Trace(ITL_SERVER_TRACE, "Starting an independent itl thread.\n");

    ITL_NEW(funcInfoP, funcInfo_t);
    funcInfoP->func = fn;
    funcInfoP->argP = argP;
    /* 
     *  Let each thread block do its initial actions in old thread
     */
    itlThread_PerformInit(&funcInfoP->threadBlocks,
			  &funcInfoP->numThreadBlocks);

    (*itlIO_CreateIndThreadP)(GenericItlIndThread,
			      (pointer_t) funcInfoP);
}

