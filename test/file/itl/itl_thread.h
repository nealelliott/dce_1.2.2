/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_thread.h,v $
 * Revision 1.1.412.1  1996/10/17  18:27:50  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:57  damon]
 *
 * Revision 1.1.407.1  1994/02/04  20:45:47  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:44  devsrc]
 * 
 * Revision 1.1.405.1  1993/12/07  17:44:57  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:30:27  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:31:37  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:11:22  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:23:42  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	 Selected comments:
 * 	   Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	   Initial checkin.
 * 	[1992/09/24  12:41:52  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: itl_thread.h,v 1.1.412.1 1996/10/17 18:27:50 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
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
 * $TALog: itl_thread.h,v $
 * Revision 1.5  1994/11/01  22:30:12  cfe
 * Bring over the changes that the OSF made in going from their DCE 1.0.3
 * release to their DCE 1.1 release.
 * [from r1.4 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 *
 * Revision 1.4  1993/01/14  17:38:45  jaffe
 * revdrop of latest OSF copyright changes to these files
 * [from r1.3 by delta osf-revdrop-01-12-92, r1.1]
 *
 * Revision 1.1  1992/08/10  20:11:31  fred
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
 * Revision 1.2  92/03/04  15:14:40  oz
 * 	 - added a detailed description
 * 	[92/02/24  08:31:02  oz]
 * 
 * 	Initial Checkin
 * 	[92/02/23  16:01:51  oz]
 * 
 */

/*
 * itl_thread.h -- 
 *    itl thread related matters (was part of itl_io)
 *
 *
 */
/*
 *  Description:
 *
 *	Itl maintains information about the state of the threads in
 *  thread slots.  It stores the IO state of the thread (file or string,
 *  file handle, etc.) as well as the history information (which history
 *  buffer the index and offset in the file, last prompt, etc.) and the
 *  variable information (the values of all local variables.
 *    When a new thread is created, the state of the new thread is a
 *  function of the state of its parent.  E.g., the child thread inherits
 *  from its parent the values of all the parent's local variables.
 *  In addition, the client of itl, the user, may also have data associated
 *  with threads that the client may wish to initialize when a thread is
 *  created.  For this purpose, this module was created.
 *
 *    This module allows the client of ITL to register as thread-client
 *  in otherwords, the client notifies ITL that it has thread state it would
 *  like to pass from parent thread to child thread by calling
 *  itl_AddThreadState (defined in itl.h).  In that call the client supplies
 *  thread upcalls.  The first (init) is called in the parent thread before
 *  the thread is created and allows the parent to initialize some 
 *  information to be sent to the child.
 *    The second upcall (Start) is called in the child thread before it
 *  starts executing.  The final (Cleanup) is called in the child thread
 *  before it terminates and allows to free any memory allocated by the
 *  first two upcalls.
 *
 *    ITL itself uses this same mechanism.  The itlIO module registers
 *  as a client of the itlThread modules and provides three upcalls
 *  to pass the itl information from paren to child thread.  (see
 *  itl_io.c for itl's use of this module).
 *
 *    If you want to use ITL but you want the flexibility to start your
 *  own threads (that will access ITL data such as ITL variables) you
 *  must follow the same procedure:  You must call itlThread_PerformInit
 *  in the parent thread.  You must provide a pointer to a pointer_t variable
 *  and a pointer to an int.  You must then call itlThread_PerformStart
 *  in the child thread providing it the pointer and int returned by the
 *  first call.  You must then call itlThread_PerformCleanup after the
 *  thread finishes before it terminates.
 *
 *    This module also exports a function: itl_StartIndThread
 *  to start a thread under ITL.  It takes as a parameter the function
 *  to run in the thread and the argument to the function.  It starts
 *  a new thread performing the initialization described above.  The
 *  thread is a detached thread and cannot be joined.
 *
 *    For example, the threadExec command uses the function described
 *  above.  The regular exec command calls a function, itl_ExecuteVar
 *  which takes a pointer to a script variable and executes the string
 *  value of that variable.  The threadExec command is implemented by
 *  calling itl_StartIndThread and providing itl_ExecuteVar and the
 *  pointer to the variable as parameters.
 */

#ifndef TRANSARC_ITL_THREAD_H
#define TRANSARC_ITL_THREAD_H 1
/*
 *   itlThread_PerformInit - 
 *   Peform the threadInit callbacks registered for starting new threads.
 *   This routine is called before a thread is started.  It allows
 *   the thread that is starting the new thread to initialize data
 *   for the new thread based on values of the starting thread.
 *   The caller may issue any number of such callbacks.  This
 *   routine calls all currently registered callbacks.
 */
extern void itlThread_PerformInit _TAKES((OUT pointer_t **threadBlockP,
					  OUT int *numThreadsP));


/*
 *  itlThread_PerformStart:
 *  Perform the thread start callbacks registered for this thread.
 *  These callbacks are made after the thread is created but before
 *  it starts executing its destined routine.
 *  The callbacks are made after the Thread IO is initialized and
 *  immediately before the call the itl_CommandLoop is made.
 */
extern void itlThread_PerformStart _TAKES((IN pointer_t *threadBlocks,
					   IN int numThreads));

/*
 *  Perform Thread Cleanup:
 *  Perform the thread cleanup callbacks registered for this thread.
 *  These callbacks are made after the thread exits.
 */
extern void itlThread_PerformCleanup _TAKES((IN pointer_t *threadBlocks,
					     IN int numThreads));

#endif /*  TRANSARC_ITL_THREAD_H  */
