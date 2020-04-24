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
 * $Log: util_signal.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:05  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:25:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:58:30  root]
 * 
 * Revision 1.1.2.5  1994/06/09  16:02:43  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:19  devsrc]
 * 
 * Revision 1.1.2.4  1994/01/10  19:54:56  rousseau
 * 	Fix rios build problems.
 * 	[1994/01/10  19:53:38  rousseau]
 * 
 * Revision 1.1.2.3  1993/12/21  19:44:28  rousseau
 * 	Moved definition of tclXWrongArgs to util_signal.c .
 * 	[1993/12/21  19:44:11  rousseau]
 * 
 * Revision 1.1.2.2  1993/12/21  19:29:22  rousseau
 * 	Style guide changes.
 * 	[1993/12/21  19:27:07  rousseau]
 * 
 * Revision 1.1.2.1  1993/12/21  16:19:27  rousseau
 * 	Initial version.
 * 	[1993/12/21  16:19:13  rousseau]
 * 
 * $EndLog$
 */

#ifndef _UTIL_SIGNAL_H
#define _UTIL_SIGNAL_H

/*
 * MODULE: util_signal.h
 *
 * DESCRIPTION:
 *   This module contains prototypes and definitions for implementing
 * the TCL signal command.
 *
 */

/* INCLUDE FILES  */

#include <sys/signal.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <dce/tcl.h>
#include <dcecp.h>

/* DECLARATIONS */

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* INTERNAL VARIABLES */

/* PROTOTYPES */

/* void panic                      (void); */
int Tcl_ProcessSignals          (ClientData clientData,
                                 Tcl_Interp *interp,
                                 int cmdResultCode);
int Tcl_SignalCmd               (char *clientData,
                                 Tcl_Interp *interp, 
                                 int argc, 
                                 char **argv);

void Tcl_SetupSigInt            (void);

void Tcl_InitSignalHandling     (Tcl_Interp *interp);


/*
 * Flag user to indicate that a signal that was setup to return an error
 * occured (it may not have been processed yet).  This is used by interactive
 * command loops to flush input.  It should be explictly cleared by any routine
 * that cares about it.  Also an application-supplied function to call if a
 * error signal occurs.  This normally flushes command input.
 */
extern int tclGotErrorSignal;
extern void (*tclErrorSignalProc) _ANSI_ARGS_((int signalNum));

/*
 * Pointer to background error handler for signals handled while not in an
 * interpreter.
 */
extern void (*tclSignalBackgroundError) _ANSI_ARGS_((Tcl_Interp *interp));


/*
 * Exported utility functions.
 */
EXTERN char * 
Tcl_UpShift _ANSI_ARGS_((char       *targetStr,
                         CONST char *sourceStr));

EXTERN int
Tcl_StrToInt _ANSI_ARGS_((CONST char *string,
                          int         base,
                          int        *intPtr));


/*
 * Macros to do string compares.  They pre-check the first character before
 * checking of the strings are equal.
 */

#define STREQU(str1, str2) \
        (((str1) [0] == (str2) [0]) && (strcmp (str1, str2) == 0))
#define STRNEQU(str1, str2, cnt) \
        (((str1) [0] == (str2) [0]) && (strncmp (str1, str2, cnt) == 0))

/*
 * Macro that behaves like strdup, only uses ckalloc.
 */
#define ckstrdup(sourceStr) \
  (strcpy (ckalloc (strlen (sourceStr) + 1), sourceStr))

/*
 * Macro to do ctype functions with 8 bit character sets.
 */
#define ISSPACE(c) (isspace ((unsigned char) c))
#define ISDIGIT(c) (isdigit ((unsigned char) c))
#define ISLOWER(c) (islower ((unsigned char) c))

#endif
