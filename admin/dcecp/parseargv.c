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
 * $Log: parseargv.c,v $
 * Revision 1.1.10.1  1996/08/09  11:48:22  arvind
 * 	Merge changes.
 * 	[1996/06/11  15:22 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1995/11/30  20:55 UTC  truitt  /main/HPDCE02/4]
 *
 * 	Merge parsing changes.
 * 	[1995/11/30  20:54 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts54/1]
 *
 * 	CHFts16888: Ensure parsing is done on the correct tables.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/11  14:46 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.8.3  1996/02/18  19:14:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:28  marty]
 * 
 * Revision 1.1.8.2  1995/12/08  21:21:51  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/15  17:37 UTC  truitt
 * 	Merge minor message work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts29/1  1995/05/15  17:36 UTC  truitt
 * 	CHFts15237: Minor message cleanup.
 * 
 * 	HP revision /main/HPDCE02/2  1995/02/10  19:30 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts11/1  1995/02/10  19:28 UTC  truitt
 * 	CHFts14285: Make sure the error code convenience
 * 	variable gets set when an error occurs.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/05  17:22 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/12/05  17:21 UTC  truitt
 * 	OT12544: Removed unused (and use of uninitialized variable)
 * 	from the parsing function.  Also fixed the message for the
 * 	login command.
 * 	[1995/12/08  17:56:19  root]
 * 
 * Revision 1.1.4.8  1994/08/03  15:21:13  rousseau
 * 	Added more parseargv messages (CR 10600).
 * 	[1994/08/03  15:20:25  rousseau]
 * 
 * Revision 1.1.4.7  1994/07/28  20:34:50  rousseau
 * 	Merged with changes from 1.1.4.6
 * 	[1994/07/28  20:34:36  rousseau]
 * 
 * 	64 bit changes (CR 11451).
 * 	[1994/07/28  20:33:25  rousseau]
 * 
 * Revision 1.1.4.6  1994/06/09  16:02:15  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:46  devsrc]
 * 
 * Revision 1.1.4.5  1994/05/02  19:37:21  pwang
 * 	Do not need to free string when calling Tcl_SetResult()
 * 	with TCL_DYNAMIC. Removed the extra free.
 * 	[1994/05/02  19:36:43  pwang]
 * 
 * Revision 1.1.4.4  1994/05/02  17:26:15  pwang
 * 	Added a new feature to dcp_ParseArgv() to detect the
 * 	Unrecognized options
 * 	[1994/05/02  17:23:05  pwang]
 * 
 * Revision 1.1.4.3  1994/04/08  18:49:35  rousseau
 * 	Added ability to handle ambiguous verbs (CR 10241).
 * 	[1994/04/08  18:48:28  rousseau]
 * 
 * Revision 1.1.4.2  1994/02/10  21:48:43  rousseau
 * 	Fixes for help facility problems. CR's 9917 9918 9919 9920
 * 	[1994/02/10  21:48:30  rousseau]
 * 
 * Revision 1.1.4.1  1994/02/02  19:43:57  rousseau
 * 	Removed '-' from help switch.
 * 	[1994/02/02  19:41:55  rousseau]
 * 
 * Revision 1.1.2.6  1993/12/05  23:43:38  rousseau
 * 	Fixed problem with argument error reporting.
 * 	[1993/12/05  23:43:26  rousseau]
 * 
 * Revision 1.1.2.5  1993/10/22  17:05:50  rousseau
 * 	Changes for Tcl7.0
 * 	[1993/10/22  17:05:45  rousseau]
 * 
 * Revision 1.1.2.4  1993/10/20  20:53:41  rousseau
 * 	Changed help messages from dcp_s_ to dcp_t_ .
 * 	[1993/10/20  20:53:30  rousseau]
 * 
 * Revision 1.1.2.3  1993/10/20  15:03:22  rousseau
 * 	Fix typos. Added includes for standard headers.
 * 	[1993/10/20  15:03:16  rousseau]
 * 
 * Revision 1.1.2.2  1993/10/19  13:52:53  rousseau
 * 	Renamed dcp_s_operations to dcp_s_help_operations and
 * 	dcp_s_help to dcp_s_help_help.
 * 	[1993/10/19  13:52:39  rousseau]
 * 
 * Revision 1.1.2.1  1993/10/18  19:20:25  rousseau
 * 	Conversion from dsh to dcecp. Includes initial i18n changes.
 * 	[1993/10/18  19:20:16  rousseau]
 * 
 * $EndLog$
 */

/*
 * PRE-HISTORY
 * Revision 1.1.2.6  1993/09/07  23:35:25  melman
 * 	Added operations command
 * 	[1993/09/07  23:35:09  melman]
 *
 * Revision 1.1.2.5  1993/08/06  21:06:39  melman
 * 	Abbreviations of arguments assumed that the args were at least
 * 	two characters long when doing a quick check avoiding strncmp().
 * 	Added if (length>1) check to the 2nd character test.
 * 	[1993/08/06  21:06:18  melman]
 * 
 * Revision 1.1.2.4  1993/07/26  17:57:18  melman
 * 	Stopped exporting dsh.h
 * 	[1993/07/26  17:56:44  melman]
 * 
 * Revision 1.1.2.3  1993/07/26  15:27:38  kevins
 * 	removed tk.h and replaced with dsh.h
 * 	[1993/07/26  15:16:53  kevins]
 * 
 * 	Fix Log header errors.
 * 	[1993/07/26  13:49:08  kevins]
 * 
 * Revision 1.1.2.2  1993/07/23  20:03:14  kevins
 * 	Initial submission of dsh
 * 	[1993/07/23  19:36:04  kevins]
 * 
 */

/*
 * parseargv.c    (tkArgv.c) --
 *
 *	This file contains a procedure that handles table-based
 *	argv-argc parsing.
 *
 * Copyright 1990 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>

#include <dcecp.h>
#include <util_var.h>

/*
 * Default table of argument descriptors.  These are normally available
 * in every application.
 */

static dcp_ArgvInfo defaultTable[] = {
    {"operations", DCP_ARGV_OPS, (char *) NULL,	(char *) NULL, 
         dcp_t_operations_help},
    {NULL, DCP_ARGV_END, (char *) NULL, (char *) NULL, 0}
};

/*
 * Forward declarations for procedures defined in this file:
 */

static void	ReturnOps _ANSI_ARGS_((Tcl_Interp *interp,
		    dcp_ArgvInfo *argTable, int flags));

#define DCP_SET_PARSE_ERROR(code, param) {                                     \
         Tcl_SetResult(interp, (char *)dce_sprintf(code, param), TCL_DYNAMIC); \
         DCP_GENERAL_SET_E_VAR(code);                                          \
         return TCL_ERROR;                                                     \
}

#define DCP_SET_PARSE_ERROR_TWO(code, par1, par2) {                                 \
         Tcl_SetResult(interp, (char *)dce_sprintf(code, par1, par2), TCL_DYNAMIC); \
         DCP_GENERAL_SET_E_VAR(code);                                               \
         return TCL_ERROR;                                                          \
}


/*
 *----------------------------------------------------------------------
 *
 * dcp_ParseArgv --
 *
 *	Process an argv array according to a table of expected
 *	command-line options.  See the manual page for more details.
 *
 * Results:
 *	The return value is a standard Tcl return value.  If an
 *	error occurs then an error message is left in interp->result.
 *	Under normal conditions, both *argcPtr and *argv are modified
 *	to return the arguments that couldn't be processed here (they
 *	didn't match the option table, or followed an DCP_ARGV_REST
 *	argument).
 *
 * Side effects:
 *	Variables may be modified, resources may be entered for tkwin,
 *	or procedures may be called.  It all depends on the arguments
 *	and their entries in argTable.  See the user documentation
 *	for details.
 *
 *----------------------------------------------------------------------
 */

int
dcp_ParseArgv(interp, argcPtr, argv, argTable, flags)
    Tcl_Interp *interp;		/* Place to store error message. */
    int *argcPtr;		/* Number of arguments in argv.  Modified
				 * to hold # args left in argv at end. */
    char **argv;		/* Array of arguments.  Modified to hold
				 * those that couldn't be processed here. */
    dcp_ArgvInfo *argTable;	/* Array of option descriptions */
    int flags;			/* Or'ed combination of various flag bits,
				 * such as DCP_ARGV_NO_DEFAULTS. */
{
    register dcp_ArgvInfo *infoPtr;
				/* Pointer to the current entry in the
				 * table of argument descriptions. */
    dcp_ArgvInfo *matchPtr;	/* Descriptor that matches current argument. */
    dcp_ArgvInfo *tmpPtr;       /* Used for reporting errors about non unique
                                 * commands. */
    char *curArg;		/* Current argument */
    register char c;		/* Second character of current arg (used for
				 * quick check for matching;  use 2nd char.
				 * because first char. will almost always
				 * be '-'). */
    int srcIndex;		/* Location from which to read next argument
				 * from argv. */
    int dstIndex;		/* Index into argv to which next unused
				 * argument should be copied (never greater
				 * than srcIndex). */
    int argc;			/* # arguments in argv still to process. */
    int length;			/* Number of characters in current argument. */
    int i;
    boolean32 first_match;
    int table_count = 0;        /* Number of tables to search for arguments */

    if (flags & DCP_ARGV_DONT_SKIP_FIRST_ARG) {
	srcIndex = dstIndex = 0;
	argc = *argcPtr;
    } else {
	srcIndex = dstIndex = 1;
	argc = *argcPtr-1;
    }

    while (argc > 0) {
	curArg = argv[srcIndex];
	srcIndex++;
	argc--;
	c = curArg[1];
	length = strlen(curArg);

	/*
	 * Loop throught the argument descriptors searching for one with
	 * the matching key string.  If found, leave a pointer to it in
	 * matchPtr.
	 */

	matchPtr = NULL;
        first_match = FALSE;
        if (flags & DCP_ARGV_OPERATIONS) {
            table_count = 2;
        }
        else {
            table_count = 1;
        }

        for (i = 0; i < table_count; i++) {
	    if (i == 0) {
		infoPtr = argTable;
	    } else {
		infoPtr = defaultTable;
	    }
            tmpPtr = infoPtr;
	    for (; infoPtr->type != DCP_ARGV_END; infoPtr++) {
		 if (infoPtr->key == NULL) {
		     continue;
		 }
		 if (((length > 1) && (infoPtr->key[1] != c))
			 || (strncmp(infoPtr->key, curArg, length) != 0)) {
		     continue;
		 }
                 if (strncmp(infoPtr->key, curArg, length) == 0) {
                     if (first_match) {
                         DCP_APPEND_RESULT_CODE_ONE_ARG(dcp_s_ambiguous_arg, curArg);
                         for (; tmpPtr->type != DCP_ARGV_END; tmpPtr++) {
                             if (strncmp(tmpPtr->key, curArg, length) == 0) {
                                 Tcl_AppendResult(interp, 
                                                  " ",
                                                  tmpPtr->key, 
                                                  (char *) NULL);
                             }
                         }
                         return TCL_ERROR;
                     }
                     first_match = TRUE;
                 }

		 if (infoPtr->key[length] == 0) {
		     matchPtr = infoPtr;
		     goto gotMatch;
		 }
		 if (flags & DCP_ARGV_NO_ABBREV) {
		     continue;
		 }
		 matchPtr = infoPtr;
	    }
	}
	if (matchPtr == NULL) {

	    /*
	     * Unrecognized argument/options.  Just copy it down, unless 
	     * the caller prefers an error to be registered.
	     */
	    if (flags & DCP_ARGV_NO_LEFTOVERS) {
                DCP_SET_PARSE_ERROR(dcp_s_bad_arg, curArg);
	    }

	    if ((flags & DCP_ARGV_NO_OPT_LEFTOVERS) &&
		(curArg[0] == '-')) {
                DCP_SET_PARSE_ERROR(dcp_s_bad_opt, curArg);
	    }
	    argv[dstIndex] = curArg;
	    dstIndex++;
	    continue;
	}

	/*
	 * Take the appropriate action based on the option type
	 */

	gotMatch:
	infoPtr = matchPtr;
	switch (infoPtr->type) {
	    case DCP_ARGV_CONSTANT:
		*((long *) infoPtr->dst) = (long) infoPtr->src;
		if( flags & DCP_ARGV_FOUND_ONE)
		   goto argsDone;
		break;
	    case DCP_ARGV_INT:
		if (argc == 0) {
                    DCP_SET_PARSE_ERROR(dcp_s_opt_requires_int, infoPtr->key);
		} else {
		    char *endPtr;

		    *((int *)infoPtr->dst) = 
                        strtol(argv[srcIndex], &endPtr, 0);
		    if ((endPtr == argv[srcIndex]) || (*endPtr != 0)) {
                        DCP_SET_PARSE_ERROR_TWO(dcp_s_expected_int_arg, infoPtr->key,
                                                argv[srcIndex]);
		    }
		    srcIndex++;
		    argc--;
		}
		break;
	    case DCP_ARGV_STRING:
		if (argc == 0) {
                    DCP_SET_PARSE_ERROR(dcp_s_opt_requires_string, infoPtr->key);
		} else {
		    *((char **)infoPtr->dst) = argv[srcIndex];
		    srcIndex++;
		    argc--;
		}
		break;
	    case DCP_ARGV_UID:
		if (argc == 0) {
		    goto missingArg;
		} else {
		    srcIndex++;
		    argc--;
		}
		break;
	    case DCP_ARGV_REST:
		*((int *) infoPtr->dst) = dstIndex;
		goto argsDone;
	    case DCP_ARGV_FLOAT:
		if (argc == 0) {
                    DCP_SET_PARSE_ERROR(dcp_s_opt_requires_float, infoPtr->key);
		} else {
		    char *endPtr;

		    *((double *) infoPtr->dst) =
			    strtod(argv[srcIndex], &endPtr);
		    if ((endPtr == argv[srcIndex]) || (*endPtr != 0)) {
                        DCP_SET_PARSE_ERROR_TWO(dcp_s_expected_float_arg, infoPtr->key,
                                                argv[srcIndex]);
		    }
		    srcIndex++;
		    argc--;
		}
		break;
	    case DCP_ARGV_OPS:
                if (flags & DCP_ARGV_OPERATIONS) {
                    ReturnOps (interp, argTable, flags);
                    return TCL_OK;
                }
                else {
                    DCP_SET_PARSE_ERROR(dcp_s_bad_arg, curArg);
                }
	    default:
                DCP_SET_PARSE_ERROR_TWO(dcp_s_bad_arg_type, infoPtr->type,
                                        "dcp_ArgvInfo");
	}

    }

    /*
     * If we broke out of the loop because of an OPT_REST argument,
     * copy the remaining arguments down.
     */

    argsDone:
    while (argc) {
	argv[dstIndex] = argv[srcIndex];
	srcIndex++;
	dstIndex++;
	argc--;
    }
    argv[dstIndex] = (char *) NULL;
    *argcPtr = dstIndex;
    return TCL_OK;

    missingArg:
        DCP_SET_PARSE_ERROR(dcp_s_opt_requires_more_args, curArg);
}

/*
 *----------------------------------------------------------------------
 *
 * ReturnOps --
 *
 *	Returns a list of all key's in the argTable
 *
 * Results:
 *	Interp->result will be modified to hold a list of
 *	all the keys in argTable, plus all those
 *	in the default table IF DCP_ARGV_NO_OPERATIONS is
 *	specified in flags.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */
static void
ReturnOps(interp, argTable, flags)
    Tcl_Interp *interp;		/* Place information in this interp's
				 * result area. */
    dcp_ArgvInfo *argTable;	/* Array of command-specific argument
				 * descriptions. */
    int flags;			/* If the DCP_ARGV_NO_DEFAULTS bit is set
				 * in this word, then don't generate
				 * information for default options. */
{
    register dcp_ArgvInfo *infoPtr;

    if (flags & DCP_ARGV_OPERATIONS) {
	for (infoPtr = argTable; infoPtr->type != DCP_ARGV_END; infoPtr++) {
	    Tcl_AppendElement(interp, infoPtr->key);
	}

	for (infoPtr = defaultTable; infoPtr->type != DCP_ARGV_END; infoPtr++) {
	    Tcl_AppendElement(interp, infoPtr->key);
	}
    }
}
