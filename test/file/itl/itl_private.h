/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl_private.h,v 1.1.417.1 1996/10/17 18:27:38 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1991, 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: itl_private.h,v $
 * Revision 1.1.417.1  1996/10/17  18:27:38  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:51  damon]
 *
 * Revision 1.1.412.1  1994/02/04  20:45:36  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:39  devsrc]
 * 
 * Revision 1.1.410.1  1993/12/07  17:44:49  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:29:03  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:31:22  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:10:49  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:23:07  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	 Selected comments:
 * 	   Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	   Initial checkin.
 * 	[1992/09/23  21:28:14  jaffe]
 * 
 * 	$TALog: itl_private.h,v $
 * 	Revision 1.6  1994/11/01  22:29:56  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.5 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.5  1993/03/01  14:48:26  fred
 * 	To make ITL work on SUN DCE, i.e. for the Alpine group.  Most of the changes
 * 	relate to SunOS 4.1 since it is not ANSI.
 * 
 * 	Note:  I made the changes and validated that they did not break the RIOS
 * 	       or PMAX builds.  Ward Travis will validate that this compiles on the
 * 	       SUN, since to my knowledge our build environment does not support
 * 	       the SPARC.
 * 
 * 	Changes to support SUN DCE.
 * 	[from r1.4 by delta fred-db3201-make-itl-work-on-suns, r1.1]
 * 
 * Revision 1.4  1993/01/14  17:38:35  jaffe
 * 	revdrop of latest OSF copyright changes to these files
 * 	[from r1.3 by delta osf-revdrop-01-12-92, r1.1]
 * 
 * Revision 1.1  1992/08/10  20:11:25  fred
 * 	Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	Initial checkin.
 * 
 * 	Initial checkin.
 * 	[added by delta fred-checkin-itl, revision 1.1]
 * 
 * Revision 1.20  1992/03/27  21:15:54  oz
 * 	Changes which were made before 1.0.1 and were not supposed to be
 * 	incorporated in 1.0.1 and therefore were not readied and were
 * 	not moved to CML with the rest of the file.
 * 
 * 	These changes add tranC support for ITL as well as handling
 * 	transaction aborts (exceptions) in the lower level io modules.
 * 
 * 	See General Comment Above
 * 	[from revision 1.19 by delta oz-2172-itl-support-tranc, revision 1.1]
 * 
 * Revision 1.16  1992/02/07  11:23:43  oz
 * 	- removed dev.exception stuff from here
 * 	  Moved to individual modules where it should be (see itl_io.c)
 * 	- var_t  ==>  itl_var_t
 * 	[92/02/06  13:17:19  oz]
 * 
 * Revision 1.15  92/02/05  20:51:35  oz
 * 	- Stub TRY etc. only if they are not defined.
 * 
 * Revision 1.14  92/02/02  09:23:06  oz
 * 	- Added compile switches:
 * 	  ITL_WITHOUT_THREADS - stub thread_static macros
 * 	  ITL_WITHOUT_DCE - stub TRY - CATCH_ALL - ENDTRY
 * 	[92/01/31  10:51:42  oz]
 * 
 * Revision 1.13  92/01/17  10:02:17  oz
 * 	 - renamed doFetchAndOp  ==>  itlExp_FetchAndOp
 * 	   and moved it to itl_exp.h
 * 	[92/01/17  10:01:13  oz]
 * 
 * Revision 1.12  92/01/10  11:33:10  oz
 * 	 - Removed unnecessary extern declarations
 * 
 * Revision 1.11  92/01/02  09:58:40  oz
 * 	 - Moved help_t def to itl_command.c
 * 	 - Removed some unneeded extern decls
 * 	[91/12/26  12:24:09  oz]
 * 
 * Revision 1.10  91/12/12  13:55:54  oz
 * 	Moved some defs to itl_exp.h
 * 	[91/12/10  19:48:26  oz]
 * 
 * Revision 1.9  91/11/06  16:47:44  oz
 * 	Added #include <utils/util.h>
 * 
 * Revision 1.8  91/11/06  16:30:56  oz
 * 	 - Removed #include <utils/itl/itl_trace.h>
 * 
 * Revision 1.7  91/10/17  15:21:59  fred
 * 	Removed FUNC_SUCCESS and FUNC_FAILURE definitions.
 * 
 * Revision 1.6  91/10/17  14:23:00  fred
 * 	Removed extern definitionf for ProcessCmdLine().  It should be a private
 * 	function in itl_main.c.
 * 
 * Revision 1.5  91/10/17  14:07:43  oz
 * 	- moved some Tran related info to itl_tran.h
 * 
 * Revision 1.4  91/10/17  10:53:07  oz
 * 	- Moved var structure and macros to itl.h
 * 	- Moved some routines to itl.h and left #defines for them
 * 	  here that should eventually be ridden of.
 * 	- Move the tranData routines to itl_tran.h and left #defines
 * 	  for them here that should eventually be ridden of.
 * 
 * Revision 1.3  91/10/02  11:49:04  oz
 * 	Renamed OpenServerPort to be itlPort_OpenServerPort
 * 	Renamed serverRoutine to be itlServer_Start
 * 	Renamed initializeExp to be itlExp_Init
 * 
 * Revision 1.2  91/09/12  17:59:11  oz
 * 	This file should eventually be eliminated and is used
 * 	mostly for now until each ITL module has a corresponding .h
 * 	file and all the names are converted to consistent names
 * 	used by all modules as well as external users.
 * 
 * Revision 1.1  91/08/22  17:22:46  mss
 * 	Modified REC tester to make separate library
 * 
 * 	$EndTALog$
 * 
 * $EndLog$
 */
/*
 * itl_private.h -- 
 *    Common internal header file included by components of itl programs.
 */

#ifndef ITL_ITL_PRIVATE_H
#define ITL_ITL_PRIVATE_H

#include <pthread.h>
#include <sys/signal.h>
#include <string.h>
#include "itl_stds.h"
#include <utils/itl/itl.h>

#include <utils/cht_hash.h>            /* Defines CHT_GENERATE_ROUTINES */ 

extern int atoi();
extern long strtol();

#ifndef	MIN
#define	MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef	MAX
#define	MAX(a,b) (((a)>(b))?(a):(b))
#endif


/* Backward compatibility */
#define getIntVariable itl_GetIntVariable
#define setIntVariable itl_SetIntVariable

#define LATCH_TYPE             pthread_mutex_t
#define LATCH_INIT(_alatchP)   pthread_mutex_init((_alatchP), \
						  pthread_mutexattr_default)
#define LATCH_LOCK             pthread_mutex_lock
#define LATCH_UNLOCK           pthread_mutex_unlock
#define LATCH_TRYLOCK          pthread_mutex_trylock

#define NEXT_AVAILABLE_SLOT -1

#define MAX_COMMAND_SIZE    100
#define MAX_STRING_LENGTH   10240
#define MAX_DESCR_LENGTH    10240
#define MAX_RECORD_SIZE     4096

#define VAR_GLOBAL_BY_DEFAULT TRUE

#define EOLN '\n'
#define WHITE_SPACES " \t\n"
#define ALPHA_NUM_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"



/*
 * Data Types
 */

/*
 * status_map_t maps between status strings entered for the setStatus
 * command, and the actual REC status value.
 */
#define MAX_STATUS_LENGTH   64   /* maximum length of a status code string */
typedef struct {
    char statStrg[MAX_STATUS_LENGTH];
    long statusValue;
} status_map_t;
extern status_map_t statusCodes[];
extern long MapStatusString _TAKES(());
extern char *statusToName _TAKES((IN long status));
			  
/*
 *   Data structures for script variables.
 */

#define var_t itl_var_t

#define VAR_GLOBAL_REC(_varP) ((_varP)->val)

extern var_val_t *varGlobalVal _TAKES((
				       IN itl_var_t *varP,
				       IN boolean_t alloc));
extern var_val_t *varLocalVal _TAKES((
				       IN itl_var_t *varP,
				       IN boolean_t alloc));
/*
 *  Server routines:
 */
#define ITL_SERVER_PREFIX "/tmp/itl"
#define ITL_SERVER_PREFIX_SPRINTF "/tmp/itl%s"

extern int itlPort_OpenServerPort    _TAKES((IN char *portPrefix));
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *     itlServer_Start -
 *  Open a socket and listen on it for incoming requests.
 *  portPrefix: the port prefix for the unix socket port.
 *  The actual socket name is derived from this prefix.
 *  When a request arrives, this routine starts a new thread and redirects
 *  input and output of the new thread to the socket and continues to
 *  listen for new requests.
 *  A call to this routing never returns.
 *  The portPrefix is passed as a pointer_t so that this function
 *  can be passed as a parameter to thread functions.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itlServer_Start _TAKES((IN pointer_t portPrefix));

extern void itlServer_Listen _TAKES((IN char *portPrefix));
/*
 *   Misc Routines.
 */

#define ansiTimeToStr itl_AnsiTimeToStr

#define IntToString itl_IntToStaticStr
#define DumpBytes itl_DumpBytes

/*  Some Global Variables:  */


extern void TestInitialize  _TAKES((void));
extern boolean_t itlExp_Init     _TAKES((void));
extern void cleanupExp      _TAKES((void));
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlPrint_Init
 *	Initialize the itlPrint module.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itlPrint_Init _TAKES((void));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlPrint_ElapsedTime
 *	Return the number of miliseconds elapsed since the
 *	test started.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern unsigned long itlPrint_ElapsedTime _TAKES((void));

extern boolean_t startLogging _TAKES((IN char *fileName));
extern void stopLogging _TAKES(());


#define commandTable_t itl_commandTable_t

#define locateVariable itl_LocateVariable


/*
 *  extern itl_var_t *newVariable _TAKES(());
 * 
 *  extern void freeVariable _TAKES(());
 */


extern void printVarValue _TAKES((IN FILE *file, IN itl_var_t *var));


extern boolean_t printVarType _TAKES((IN itl_var_t var));

/*  
 * Some Routines to manipulate script variables
 */

/* setVariable: Set a variable if one exists or create one */
#define setVariable itl_SetVariable

extern void makeVarGlobal _TAKES((IN itl_var_t *varP));
extern void makeVarLocal  _TAKES((IN itl_var_t *varP));

#define GetPointer(_str) itl_GetBoolean((_str), "Good", "Bad", "Pointer")

extern int GetInteger _TAKES(());

extern var_val_t *GetExpression _TAKES(());

extern var_val_t *EvalBinaryOp _TAKES((
				       INOUT var_val_t *leftVal,
				       IN var_val_t *rightVal,
				       IN char *Op));

extern var_val_t *GetVarVal _TAKES(());

/*
 *   IO_GetWord()  
 *    return one word from the current location in the history buffer.
 *    This routine handles string expressions, including
 *    string concatenation, duplication, and variable expansion.
 *
 *    A word is defined to be any string of consecutive non-white
 *    characters, or any sequence of characters surrounded by double
 *    quotes, or any expression.
 */
extern char *IO_GetWord _TAKES((
				OUT boolean_t *eofP,
				OUT int *lenP
				));



extern boolean_t CommandLoop _TAKES((IN char *promptStr));

extern void Init_IO _TAKES(());

#define VAR_THREAD_REC(_varP, _varValueP)                                  \
        MACRO_BEGIN                                                        \
          if (pthread_getspecific((_varP)->threadVal,                      \
				  (pthread_addr_t *) &(_varValueP)) != 0)  \
          {                                                                \
	    itl_PrintError("Could not obtain variable value.\n\tFile: %s\n\tLine: %d\n",                                                                  \
			   __FILE__, __LINE__);                            \
	  exit(-1);                                                        \
          }                                                                \
        MACRO_END

extern itl_specialVar_t       threadID;       /* The thread ID          */

extern void initializeSpecialVars _TAKES(());

#include <utils/itl/itl_io.h>

#endif

