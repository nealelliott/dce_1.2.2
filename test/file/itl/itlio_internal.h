/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itlio_internal.h,v $
 * Revision 1.1.418.1  1996/10/17  18:28:07  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:08  damon]
 *
 * Revision 1.1.413.1  1994/02/04  20:46:04  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:51  devsrc]
 * 
 * Revision 1.1.411.1  1993/12/07  17:45:07  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:32:30  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:31:56  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:12:09  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:24:24  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	 Selected comments:
 * 	   Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	   Initial checkin.
 * 	[1992/09/24  12:44:09  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: itlio_internal.h,v 1.1.418.1 1996/10/17 18:28:07 damon Exp $
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
 * $TALog: itlio_internal.h,v $
 * Revision 1.6  1994/11/01  22:30:31  cfe
 * Bring over the changes that the OSF made in going from their DCE 1.0.3
 * release to their DCE 1.1 release.
 * [from r1.5 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 *
 * Revision 1.5  1993/03/08  13:52:09  fred
 * To import fixes to ITL from Ofer.  Also, fixed some outstanding minor
 * bugs we found -
 *  - The spoke (distributed ITL) had a bug in it's sync point code.  It was
 *    not removing a variable from the hash table, which caused confusion when
 *    a sync point name was re-used in a tight loop.  Also, it was not
 *    setting the timeout field correctly during sync point creation.
 *
 *  - Remove the dependency of the ITL utilities library on the com_err
 *    library.  I wrote our own error handling function that deals with both
 *    dce and errno correctly.  Besides, the error_message() function in the
 *    com_err library is not thread-safe.
 *
 * Incorporation of Encina ITL fixes.
 * [from r1.4 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 *
 * Revision 1.4  1993/01/14  17:38:58  jaffe
 * revdrop of latest OSF copyright changes to these files
 * [from r1.3 by delta osf-revdrop-01-12-92, r1.1]
 *
 * Revision 1.1  1992/08/10  20:11:45  fred
 * Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * Initial checkin.
 *
 * Initial checkin.
 * [added by delta fred-checkin-itl, revision 1.1]
 *
 * Revision 1.3  1992/03/27  21:16:06  oz
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
 * Revision 1.2  92/03/04  15:14:51  oz
 * 	Initial Checkin
 * 	[92/02/23  16:01:33  oz]
 * 
 */

/*
 * itlio_internal.h -- 
 *    Declarations internal to the itl_io module.
 * 
 *
 *
 */

#ifndef TRANSARC_ITL_IO_INTERNAL_H
#define TRANSARC_ITL_IO_INTERNAL_H 1

/*=*=*=*==*=*=*==*=*=*==*=*=*==*=*=*==*=*=*==*=*=*==*=*=*==*=*=*==*=*=*=
 *
 *	Thread State:
 *		IO State
 *		History Buffer State
 *
 *=*=*=*==*=*=*==*=*=*==*=*=*==*=*=*==*=*=*==*=*=*==*=*=*==*=*=*==*=*=*=
 */


/*
 *  itlIO_info_t:
 *	Structure to hold thread specific IO information.
 *
 *	Each thread has its onw copy of this structure which determines
 *	how the thread should perform IO.
 *	Points of interest:  Many threads may be reading from the same
 *	file into the same buffer.  Each history buffer has a latch
 *	in order to make sure that the right thing happens.  In addition,
 *	each history buffer maintains the highest numbered line for which
 *	the user was prompted for to make sure the user is not prompted
 *	twice for the same line.  When a thread prompts for a line the
 *	thread updates that value.  The thread also sets its
 *	io_thread to TRUE indicating that it is permitted advise the
 *	user.  When the thread next goes to read a line and the line is
 *	already there, or it has already been prompted for it sets
 *	io_thread to FALSE.
 */
typedef struct {
    FILE *io_out;		/* File to use for output	   */
    FILE *io_error;		/* File to use as stderr	   */
    char *fileName;		/* Name of the input file if any */
    char *parentFile;		/* Name of parent file if any	 */
    boolean_t fileActive;	/* Are we running from a file?   */
    char **inputString;		/* When we run from a string    */
    boolean_t interactive;	/* Interactive mode?            */
    unsigned int nestingLevel;	/* The nesting level.		*/
    unsigned int braceLevel;	/* The brace nesting level (for trans) */
    scope_t   defaultScope;	/* Default Scope for Variables  */
    boolean_t error;		/* Was there an error?		*/
    boolean_t interface;        /* Interface Mode?  		*/
    int	      includeNesting;	/* The include nesting level	*/
    boolean_t earlyQuit;	/* Was an early quit detectred  */
    boolean_t openPermitted;	/* Allowed to open the file if needed
				 * This is TRUE by default and set to FALSE
				 * as soon as we read from the file
				 */
    FILE *defaultInput;		/*  The default input file.
				 *  This is usually stdin unless this is
				 *  a thread talking to an itl client
				 */
} itlIO_info_t;

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_Data
 *	The IO information of this thread.
 */
extern itlIO_info_t *itlIO_Data _TAKES((void));

#define Current_IO itlIO_Data()


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_HistoryLoc()
 *    returns a character pointer pointing to the current location in
 *    the history list.
 */
extern char *itlIO_HistoryLoc _TAKES((void));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlIO_GetUninterpretedLine(outStr, doneStr, outLen):
 *    Get a line of input without interpreting variables.
 *    If doneStr is not NULL and the line contains the string doneStr
 *    then the function returns TRUE.  Otherwise it returns FALSE.
 *    The line does not include the string doneStr or a newLine.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t itlIO_GetUninterpretedLine _TAKES((
						    OUT char *outStr,
						    IN char *doneStr,
						    OUT int *outLen));

/*
 *  itlIO_InternalInit
 *	Initialize the internal module of itlIO
 */
extern boolean_t itlIO_InternalInit _TAKES((void));

#endif  /*  TRANSARC_ITL_IO_INTERNAL_H  */

