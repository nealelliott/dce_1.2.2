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
 * $Log: tclUnixAZ.c,v $
 * Revision 1.1.8.3  1996/02/18  19:19:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:04:45  marty]
 *
 * Revision 1.1.8.2  1995/12/08  21:34:39  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/28  20:25 UTC  dnguyen
 * 	Merge cleaning up of I18N-commented code
 * 
 * 	HP revision /main/dnguyen_mothra/1  1995/04/11  16:45 UTC  dnguyen
 * 	Clean up I18N comments
 * 	[1995/12/08  18:06:14  root]
 * 
 * Revision 1.1.4.1  1994/10/21  21:39:24  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:33:30  mori_m]
 * 
 * Revision 1.1.2.1  1994/07/12  21:19:34  mori_m
 * 	CR 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:14:34  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclUnixAZ.c --
 *
 *	This file contains the top-level command procedures for
 *	commands in the Tcl core that require UNIX facilities
 *	such as files and process execution.  Much of the code
 *	in this file is based on earlier versions contributed
 *	by Karl Lehenbauer, Mark Diekhans and Peter da Silva.
 *
 * Copyright (c) 1991-1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#include "tclInt.h"
#include "tclWInt.h"
#include "tclUnix.h"
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>

/*
 * The variable below caches the name of the current working directory
 * in order to avoid repeated calls to getcwd.  The string is malloc-ed.
 * NULL means the cache needs to be refreshed.
 */

static char *currentDir =  NULL;

/*
 * If the system doesn't define the EWOULDBLOCK errno, just #define it
 * to a bogus value that will never occur.
 */

#ifndef EWOULDBLOCK
#define EWOULDBLOCK -1901
#endif

/*
 * Prototypes for local procedures defined in this file:
 */

static int		CleanupChildren _ANSI_ARGS_((Tcl_Interp *interp,
			    int numPids, int *pidPtr, int errorId,
			    int keepNewline));
char *			GetFileType _ANSI_ARGS_((int mode));
static char *		GetOpenMode _ANSI_ARGS_((Tcl_Interp *interp,
			    char *string, int *modePtr));
static int		StoreStatData _ANSI_ARGS_((Tcl_Interp *interp,
			    char *varName, struct stat *statPtr));

/*
 *----------------------------------------------------------------------
 *
 * Tcl_CdCmd --
 *
 *	This procedure is invoked to process the "cd" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_CdCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    char *dirName;
    Tcl_DString buffer;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *i18n_temp;
    /* int result; */

    if (argc > 2) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_dirname);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    if (argc == 2) {
	dirName = argv[1];
    } else {
	dirName = "~";
    }
    dirName = Tcl_TildeSubst(interp, dirName, &buffer);
    if (dirName == NULL) {
	return TCL_ERROR;
    }
    if (currentDir != NULL) {
	ckfree(currentDir);
	currentDir = NULL;
    }
    /* result = TCL_OK; */
    if (chdir(dirName) != 0) {
	i18n_temp = Tcl_PosixError(interp);
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_cannot_change_work_dir);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(dirName)
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, dirName, i18n_temp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	/* result = TCL_ERROR; */
        Tcl_DStringFree(&buffer);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&buffer);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_CloseCmd --
 *
 *	This procedure is invoked to process the "close" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_CloseCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    OpenFile *oFilePtr;
    int result = TCL_OK;
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *i18n_temp;

    if (argc != 2) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (Tcl_GetOpenFile(interp, argv[1], 0, 0, &f) != TCL_OK) {
	return TCL_ERROR;
    }
    oFilePtr = tclOpenFiles[fileno(f)];
    tclOpenFiles[fileno(f)] = NULL;

    /*
     * First close the file (in the case of a process pipeline, there may
     * be two files, one for the pipe at each end of the pipeline).
     */

    if (oFilePtr->f2 != NULL) {
	clearerr(oFilePtr->f2);
	if (fclose(oFilePtr->f2) == EOF) {
	    i18n_temp = Tcl_PosixError(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_closing);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1])
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1], i18n_temp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	}
    }
    clearerr(oFilePtr->f);
    if (fclose(oFilePtr->f) == EOF) {
	i18n_temp = Tcl_PosixError(interp);
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_closing);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1])
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1], i18n_temp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	result = TCL_ERROR;
    }

    /*
     * If the file was a connection to a pipeline, clean up everything
     * associated with the child processes.
     */

    if (oFilePtr->numPids > 0) {
	if (CleanupChildren(interp, oFilePtr->numPids, oFilePtr->pidPtr,
		oFilePtr->errorId, 0) != TCL_OK) {
	    result = TCL_ERROR;
	}
    }

    ckfree((char *) oFilePtr);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_EofCmd --
 *
 *	This procedure is invoked to process the "eof" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_EofCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc != 2) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (Tcl_GetOpenFile(interp, argv[1], 0, 0, &f) != TCL_OK) {
	return TCL_ERROR;
    }
    if (feof(f)) {
	interp->result = "1";
    } else {
	interp->result = "0";
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ExecCmd --
 *
 *	This procedure is invoked to process the "exec" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_ExecCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int outputId;			/* File id for output pipe.  -1
					 * means command overrode. */
    int errorId;			/* File id for temporary file
					 * containing error output. */
    int *pidPtr;
    int numPids, result, keepNewline;
    int firstWord;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    /*
     * Check for a leading "-keepnewline" argument.
     */

    keepNewline = 0;
    for (firstWord = 1; (firstWord < argc) && (argv[firstWord][0] == '-');
	    firstWord++) {
	if (strcmp(argv[firstWord], "-keepnewline") == 0) {
	    keepNewline = 1;
	} else if (strcmp(argv[firstWord], "--") == 0) {
	    firstWord++;
	    break;
	} else {
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_bad_switch_keepnewline);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[firstWord])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[firstWord]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
    }

    if (argc <= firstWord) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_switch_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * See if the command is to be run in background;  if so, create
     * the command, detach it, and return a list of pids.
     */

    if ((argv[argc-1][0] == '&') && (argv[argc-1][1] == 0)) {
	int i;
	char id[50];

	argc--;
	argv[argc] = NULL;
	numPids = Tcl_CreatePipeline(interp, argc-firstWord, argv+firstWord,
		&pidPtr, (int *) NULL, (int *) NULL, (int *) NULL);
	if (numPids < 0) {
	    return TCL_ERROR;
	}
	Tcl_DetachPids(numPids, pidPtr);
	for (i = 0; i < numPids; i++) {
	    sprintf(id, "%d", pidPtr[i]);
	    Tcl_AppendElement(interp, id);
	}
	ckfree((char *) pidPtr);
	return TCL_OK;
    }

    /*
     * Create the command's pipeline.
     */

    numPids = Tcl_CreatePipeline(interp, argc-firstWord, argv+firstWord,
	    &pidPtr, (int *) NULL, &outputId, &errorId);
    if (numPids < 0) {
	return TCL_ERROR;
    }

    /*
     * Read the child's output (if any) and put it into the result.
     */

    result = TCL_OK;
    if (outputId != -1) {
	while (1) {
#	    define BUFFER_SIZE 1000
	    char buffer[BUFFER_SIZE+1];
	    int count;
    
	    count = read(outputId, buffer, (size_t) BUFFER_SIZE);
    
	    if (count == 0) {
		break;
	    }
	    if (count < 0) {
		char *i18n_temp;

		Tcl_ResetResult(interp);
		i18n_temp = Tcl_PosixError(interp);
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_read_from_out_pipe);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(i18n_temp)
					   + strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, i18n_temp);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);

		result = TCL_ERROR;
		break;
	    }
	    buffer[count] = 0;
	    Tcl_AppendResult(interp, buffer, (char *) NULL);
	}
	close(outputId);
    }

    if (CleanupChildren(interp, numPids, pidPtr, errorId, keepNewline)
	    != TCL_OK) {
	result = TCL_ERROR;
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ExitCmd --
 *
 *	This procedure is invoked to process the "exit" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_ExitCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int value;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if ((argc != 1) && (argc != 2)) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_returncode);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (argc == 1) {
	exit(0);
    }
    if (Tcl_GetInt(interp, argv[1], &value) != TCL_OK) {
	return TCL_ERROR;
    }
    exit(value);
    /*NOTREACHED*/
    return TCL_OK;			/* Better not ever reach this! */
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_FileCmd --
 *
 *	This procedure is invoked to process the "file" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_FileCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    char *p;
    int length, statOp, result;
    int mode = 0;			/* Initialized only to prevent
					 * compiler warning message. */
    struct stat statBuf;
    char *fileName, c;
    Tcl_DString buffer;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 3) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_opt_name_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    c = argv[1][0];
    length = strlen(argv[1]);
    result = TCL_OK;

    /*
     * First handle operations on the file name.
     */

    fileName = Tcl_TildeSubst(interp, argv[2], &buffer);
    if (fileName == NULL) {
	result = TCL_ERROR;
	goto done;
    }
    if ((c == 'd') && (strncmp(argv[1], "dirname", length) == 0)) {
	if (argc != 3) {
	    argv[1] = "dirname";
	    not3Args:
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_name);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen(argv[1])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0], argv[1]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto done;
	}
	p = strrchr(fileName, '/');
	if (p == NULL) {
	    interp->result = ".";
	} else if (p == fileName) {
	    interp->result = "/";
	} else {
	    *p = 0;
	    Tcl_SetResult(interp, fileName, TCL_VOLATILE);
	    *p = '/';
	}
	goto done;
    } else if ((c == 'r') && (strncmp(argv[1], "rootname", length) == 0)
	    && (length >= 2)) {
	char *lastSlash;

	if (argc != 3) {
	    argv[1] = "rootname";
	    goto not3Args;
	}
	p = strrchr(fileName, '.');
	lastSlash = strrchr(fileName, '/');
	if ((p == NULL) || ((lastSlash != NULL) && (lastSlash > p))) {
	    Tcl_SetResult(interp, fileName, TCL_VOLATILE);
	} else {
	    *p = 0;
	    Tcl_SetResult(interp, fileName, TCL_VOLATILE);
	    *p = '.';
	}
	goto done;
    } else if ((c == 'e') && (strncmp(argv[1], "extension", length) == 0)
	    && (length >= 3)) {
	char *lastSlash;

	if (argc != 3) {
	    argv[1] = "extension";
	    goto not3Args;
	}
	p = strrchr(fileName, '.');
	lastSlash = strrchr(fileName, '/');
	if ((p != NULL) && ((lastSlash == NULL) || (lastSlash < p))) {
	    Tcl_SetResult(interp, p, TCL_VOLATILE);
	}
	goto done;
    } else if ((c == 't') && (strncmp(argv[1], "tail", length) == 0)
	    && (length >= 2)) {
	if (argc != 3) {
	    argv[1] = "tail";
	    goto not3Args;
	}
	p = strrchr(fileName, '/');
	if (p != NULL) {
	    Tcl_SetResult(interp, p+1, TCL_VOLATILE);
	} else {
	    Tcl_SetResult(interp, fileName, TCL_VOLATILE);
	}
	goto done;
    }

    /*
     * Next, handle operations that can be satisfied with the "access"
     * kernel call.
     */

    if (fileName == NULL) {
	result = TCL_ERROR;
	goto done;
    }
    if ((c == 'r') && (strncmp(argv[1], "readable", length) == 0)
	    && (length >= 5)) {
	if (argc != 3) {
	    argv[1] = "readable";
	    goto not3Args;
	}
	mode = R_OK;
	checkAccess:
	if (access(fileName, mode) == -1) {
	    interp->result = "0";
	} else {
	    interp->result = "1";
	}
	goto done;
    } else if ((c == 'w') && (strncmp(argv[1], "writable", length) == 0)) {
	if (argc != 3) {
	    argv[1] = "writable";
	    goto not3Args;
	}
	mode = W_OK;
	goto checkAccess;
    } else if ((c == 'e') && (strncmp(argv[1], "executable", length) == 0)
	    && (length >= 3)) {
	if (argc != 3) {
	    argv[1] = "executable";
	    goto not3Args;
	}
	mode = X_OK;
	goto checkAccess;
    } else if ((c == 'e') && (strncmp(argv[1], "exists", length) == 0)
	    && (length >= 3)) {
	if (argc != 3) {
	    argv[1] = "exists";
	    goto not3Args;
	}
	mode = F_OK;
	goto checkAccess;
    }

    /*
     * Lastly, check stuff that requires the file to be stat-ed.
     */

    if ((c == 'a') && (strncmp(argv[1], "atime", length) == 0)) {
	if (argc != 3) {
	    argv[1] = "atime";
	    goto not3Args;
	}
	if (stat(fileName, &statBuf) == -1) {
	    goto badStat;
	}
	sprintf(interp->result, "%ld", statBuf.st_atime);
	goto done;
    } else if ((c == 'i') && (strncmp(argv[1], "isdirectory", length) == 0)
	    && (length >= 3)) {
	if (argc != 3) {
	    argv[1] = "isdirectory";
	    goto not3Args;
	}
	statOp = 2;
    } else if ((c == 'i') && (strncmp(argv[1], "isfile", length) == 0)
	    && (length >= 3)) {
	if (argc != 3) {
	    argv[1] = "isfile";
	    goto not3Args;
	}
	statOp = 1;
    } else if ((c == 'l') && (strncmp(argv[1], "lstat", length) == 0)) {
	if (argc != 4) {
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_lstat_name_var);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto done;
	}

	if (lstat(fileName, &statBuf) == -1) {
	    char *i18n_temp;

	    i18n_temp = Tcl_PosixError(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_no_lstat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[2])
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[2], i18n_temp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto done;
	}
	result = StoreStatData(interp, argv[3], &statBuf);
	goto done;
    } else if ((c == 'm') && (strncmp(argv[1], "mtime", length) == 0)) {
	if (argc != 3) {
	    argv[1] = "mtime";
	    goto not3Args;
	}
	if (stat(fileName, &statBuf) == -1) {
	    goto badStat;
	}
	sprintf(interp->result, "%ld", statBuf.st_mtime);
	goto done;
    } else if ((c == 'o') && (strncmp(argv[1], "owned", length) == 0)) {
	if (argc != 3) {
	    argv[1] = "owned";
	    goto not3Args;
	}
	statOp = 0;
    } else if ((c == 'r') && (strncmp(argv[1], "readlink", length) == 0)
	    && (length >= 5)) {
	char linkValue[MAXPATHLEN+1];
	int linkLength;

	if (argc != 3) {
	    argv[1] = "readlink";
	    goto not3Args;
	}

	/*
	 * If S_IFLNK isn't defined it means that the machine doesn't
	 * support symbolic links, so the file can't possibly be a
	 * symbolic link.  Generate an EINVAL error, which is what
	 * happens on machines that do support symbolic links when
	 * you invoke readlink on a file that isn't a symbolic link.
	 */

#ifndef S_IFLNK
	linkLength = -1;
	errno = EINVAL;
#else
	linkLength = readlink(fileName, linkValue, sizeof(linkValue) - 1);
#endif /* S_IFLNK */
	if (linkLength == -1) {
	    char *i18n_temp;

	    i18n_temp = Tcl_PosixError(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_no_readlink);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[2])
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[2], i18n_temp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto done;
	}
	linkValue[linkLength] = 0;
	Tcl_SetResult(interp, linkValue, TCL_VOLATILE);
	goto done;
    } else if ((c == 's') && (strncmp(argv[1], "size", length) == 0)
	    && (length >= 2)) {
	if (argc != 3) {
	    argv[1] = "size";
	    goto not3Args;
	}
	if (stat(fileName, &statBuf) == -1) {
	    goto badStat;
	}
	sprintf(interp->result, "%ld", statBuf.st_size);
	goto done;
    } else if ((c == 's') && (strncmp(argv[1], "stat", length) == 0)
	    && (length >= 2)) {
	if (argc != 4) {
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_stat_name_var);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto done;
	}

	if (stat(fileName, &statBuf) == -1) {
	    char *i18n_temp;

	    badStat:

	    i18n_temp = Tcl_PosixError(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_no_stat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[2])
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[2], i18n_temp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto done;
	}
	result = StoreStatData(interp, argv[3], &statBuf);
	goto done;
    } else if ((c == 't') && (strncmp(argv[1], "type", length) == 0)
	    && (length >= 2)) {
	if (argc != 3) {
	    argv[1] = "type";
	    goto not3Args;
	}
	if (lstat(fileName, &statBuf) == -1) {
	    goto badStat;
	}
	interp->result = GetFileType((int) statBuf.st_mode);
	interp->freeProc = (Tcl_FreeProc *) free;
	goto done;
    } else {
	i18n_msg_format = dce_msg_get(tcl_s_unixza_bad_opt_atime_dirname_exec);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1])
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	result = TCL_ERROR;
	goto done;
    }
    if (stat(fileName, &statBuf) == -1) {
	interp->result = "0";
	goto done;
    }
    switch (statOp) {
	case 0:
	    mode = (geteuid() == statBuf.st_uid);
	    break;
	case 1:
	    mode = S_ISREG(statBuf.st_mode);
	    break;
	case 2:
	    mode = S_ISDIR(statBuf.st_mode);
	    break;
    }
    if (mode) {
	interp->result = "1";
    } else {
	interp->result = "0";
    }

    done:
    Tcl_DStringFree(&buffer);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * StoreStatData --
 *
 *	This is a utility procedure that breaks out the fields of a
 *	"stat" structure and stores them in textual form into the
 *	elements of an associative array.
 *
 * Results:
 *	Returns a standard Tcl return value.  If an error occurs then
 *	a message is left in interp->result.
 *
 * Side effects:
 *	Elements of the associative array given by "varName" are modified.
 *
 *----------------------------------------------------------------------
 */

static int
StoreStatData(interp, varName, statPtr)
    Tcl_Interp *interp;			/* Interpreter for error reports. */
    char *varName;			/* Name of associative array variable
					 * in which to store stat results. */
    struct stat *statPtr;		/* Pointer to buffer containing
					 * stat data to store in varName. */
{
    char string[30];
    char *i18n_file_type;

    sprintf(string, "%d", statPtr->st_dev);
    if (Tcl_SetVar2(interp, varName, "dev", string, TCL_LEAVE_ERR_MSG)
	    == NULL) {
	return TCL_ERROR;
    }
    sprintf(string, "%d", statPtr->st_ino);
    if (Tcl_SetVar2(interp, varName, "ino", string, TCL_LEAVE_ERR_MSG)
	    == NULL) {
	return TCL_ERROR;
    }
    sprintf(string, "%d", statPtr->st_mode);
    if (Tcl_SetVar2(interp, varName, "mode", string, TCL_LEAVE_ERR_MSG)
	    == NULL) {
	return TCL_ERROR;
    }
    sprintf(string, "%d", statPtr->st_nlink);
    if (Tcl_SetVar2(interp, varName, "nlink", string, TCL_LEAVE_ERR_MSG)
	    == NULL) {
	return TCL_ERROR;
    }
    sprintf(string, "%d", statPtr->st_uid);
    if (Tcl_SetVar2(interp, varName, "uid", string, TCL_LEAVE_ERR_MSG)
	    == NULL) {
	return TCL_ERROR;
    }
    sprintf(string, "%d", statPtr->st_gid);
    if (Tcl_SetVar2(interp, varName, "gid", string, TCL_LEAVE_ERR_MSG)
	    == NULL) {
	return TCL_ERROR;
    }
    sprintf(string, "%ld", statPtr->st_size);
    if (Tcl_SetVar2(interp, varName, "size", string, TCL_LEAVE_ERR_MSG)
	    == NULL) {
	return TCL_ERROR;
    }
    sprintf(string, "%ld", statPtr->st_atime);
    if (Tcl_SetVar2(interp, varName, "atime", string, TCL_LEAVE_ERR_MSG)
	    == NULL) {
	return TCL_ERROR;
    }
    sprintf(string, "%ld", statPtr->st_mtime);
    if (Tcl_SetVar2(interp, varName, "mtime", string, TCL_LEAVE_ERR_MSG)
	    == NULL) {
	return TCL_ERROR;
    }
    sprintf(string, "%ld", statPtr->st_ctime);
    if (Tcl_SetVar2(interp, varName, "ctime", string, TCL_LEAVE_ERR_MSG)
	    == NULL) {
	return TCL_ERROR;
    }

    i18n_file_type = GetFileType((int) statPtr->st_mode);
    if (Tcl_SetVar2(interp, varName, "type",
	    i18n_file_type, TCL_LEAVE_ERR_MSG) == NULL) {
	free(i18n_file_type);
	return TCL_ERROR;
    }
    free(i18n_file_type);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * GetFileType --
 *
 *	Given a mode word, returns a string identifying the type of a
 *	file.
 *
 * Results:
 *	A static text string giving the file type from mode.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
GetFileType(mode)
    int mode;
{
    if (S_ISREG(mode)) {
	return ((char *)dce_msg_get(tcl_s_unixaz_file_type_file));
    } else if (S_ISDIR(mode)) {
	return ((char *)dce_msg_get(tcl_s_unixaz_file_type_dir));
    } else if (S_ISCHR(mode)) {
	return ((char *)dce_msg_get(tcl_s_unixaz_file_type_char));
    } else if (S_ISBLK(mode)) {
	return ((char *)dce_msg_get(tcl_s_unixaz_file_type_block));
    } else if (S_ISFIFO(mode)) {
	return ((char *)dce_msg_get(tcl_s_unixaz_file_type_fifo));
    } else if (S_ISLNK(mode)) {
	return ((char *)dce_msg_get(tcl_s_unixaz_file_type_link));
    } else if (S_ISSOCK(mode)) {
	return ((char *)dce_msg_get(tcl_s_unixaz_file_type_socket));
    }
    return ((char *)dce_msg_get(tcl_s_unixaz_file_type_unknown));
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_FlushCmd --
 *
 *	This procedure is invoked to process the "flush" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_FlushCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc != 2) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (Tcl_GetOpenFile(interp, argv[1], 1, 1, &f) != TCL_OK) {
	return TCL_ERROR;
    }
    clearerr(f);
    if (fflush(f) == EOF) {
	char *i18n_temp;

	i18n_temp = Tcl_PosixError(interp);
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_flushing);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1])
				+ strlen(i18n_temp)
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1], i18n_temp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GetsCmd --
 *
 *	This procedure is invoked to process the "gets" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_GetsCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#   define BUF_SIZE 200
    char buffer[BUF_SIZE+1];
    int totalCount, done, flags;
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *i18n_temp;

    if ((argc != 2) && (argc != 3)) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid_varname);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (Tcl_GetOpenFile(interp, argv[1], 0, 1, &f) != TCL_OK) {
	return TCL_ERROR;
    }

    /*
     * We can't predict how large a line will be, so read it in
     * pieces, appending to the current result or to a variable.
     */

    totalCount = 0;
    done = 0;
    flags = 0;
    clearerr(f);
    while (!done) {
	register int c, count;
	register char *p;

	for (p = buffer, count = 0; count < BUF_SIZE-1; count++, p++) {
	    c = getc(f);
	    if (c == EOF) {
		if (ferror(f)) {
		    /*
		     * If the file is in non-blocking mode, return any
		     * bytes that were read before a block would occur.
		     */

		    if ((errno == EWOULDBLOCK)
			    && ((count > 0 || totalCount > 0))) {
			done = 1;
			break;
		    }
		    Tcl_ResetResult(interp);
		    i18n_temp = Tcl_PosixError(interp);
		    i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_reading);
		    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1])
					    +strlen(i18n_temp)
					    + strlen((char *)i18n_msg_format) + 1);
		    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1], i18n_temp);
		    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		    free(i18n_msg_format);
		    ckfree(i18n_msg_buf);

		    return TCL_ERROR;
		} else if (feof(f)) {
		    if ((totalCount == 0) && (count == 0)) {
			totalCount = -1;
		    }
		    done = 1;
		    break;
		}
	    }
	    if (c == '\n') {
		done = 1;
		break;
	    }
	    *p = c;
	}
	*p = 0;
	if (argc == 2) {
	    Tcl_AppendResult(interp, buffer, (char *) NULL);
	} else {
	    if (Tcl_SetVar(interp, argv[2], buffer, flags|TCL_LEAVE_ERR_MSG)
		    == NULL) {
		return TCL_ERROR;
	    }
	    flags = TCL_APPEND_VALUE;
	}
	totalCount += count;
    }

    if (argc == 3) {
	sprintf(interp->result, "%d", totalCount);
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_OpenCmd --
 *
 *	This procedure is invoked to process the "open" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_OpenCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int pipeline, fd, mode, prot, readWrite, permissions;
    char *access;
    FILE *f, *f2;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if ((argc < 2) || (argc > 4)) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fname_acc_per);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    prot = 0666;
    if (argc == 2) {
	mode = O_RDONLY;
	access = "r";
    } else {
	access = GetOpenMode(interp, argv[2], &mode);
	if (access == NULL) {
	    return TCL_ERROR;
	}
	if (argc == 4) {
	    if (Tcl_GetInt(interp, argv[3], &prot) != TCL_OK) {
		return TCL_ERROR;
	    }
	}
    }

    f = f2 = NULL;
    readWrite = mode & (O_RDWR|O_RDONLY|O_WRONLY);
    if (readWrite == O_RDONLY) {
	permissions = TCL_FILE_READABLE;
    } else if (readWrite == O_WRONLY) {
	permissions = TCL_FILE_WRITABLE;
    } else {
	permissions = TCL_FILE_READABLE|TCL_FILE_WRITABLE;
    }

    pipeline = 0;
    if (argv[1][0] == '|') {
	pipeline = 1;
    }

    /*
     * Open the file or create a process pipeline.
     */

    if (!pipeline) {
	char *fileName;
	Tcl_DString buffer;

	fileName = Tcl_TildeSubst(interp, argv[1], &buffer);
	if (fileName == NULL) {
	    return TCL_ERROR;
	}
	fd = open(fileName, mode, prot);
	Tcl_DStringFree(&buffer);
	if (fd < 0) {
	    char *i18n_temp;

	    i18n_temp = Tcl_PosixError(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_cannot_open);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1])
				    +strlen(i18n_temp)
				    + strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1], i18n_temp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	f = fdopen(fd, access);
	if (f == NULL) {
	    close(fd);
	    return TCL_ERROR;
	}
	Tcl_EnterFile(interp, f, permissions);
    } else {
	int *inPipePtr, *outPipePtr;
	int cmdArgc, inPipe, outPipe, numPids, *pidPtr, errorId;
	char **cmdArgv;
	OpenFile *oFilePtr;

	if (Tcl_SplitList(interp, argv[1]+1, &cmdArgc, &cmdArgv) != TCL_OK) {
	    return TCL_ERROR;
	}
	inPipePtr = (permissions & TCL_FILE_WRITABLE) ? &inPipe : NULL;
	outPipePtr = (permissions & TCL_FILE_READABLE) ? &outPipe : NULL;
	inPipe = outPipe = errorId = -1;
	numPids = Tcl_CreatePipeline(interp, cmdArgc, cmdArgv,
		&pidPtr, inPipePtr, outPipePtr, &errorId);
	ckfree((char *) cmdArgv);
	if (numPids < 0) {
	    pipelineError:
	    if (f != NULL) {
		fclose(f);
	    }
	    if (f2 != NULL) {
		fclose(f2);
	    }
	    if (numPids > 0) {
		Tcl_DetachPids(numPids, pidPtr);
		ckfree((char *) pidPtr);
	    }
	    if (errorId != -1) {
		close(errorId);
	    }
	    return TCL_ERROR;
	}
	if (permissions & TCL_FILE_READABLE) {
	    if (outPipe == -1) {
		if (inPipe != -1) {
		    close(inPipe);
		}
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_cannot_read_output);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
		free(i18n_msg_format);

		goto pipelineError;
	    }
	    f = fdopen(outPipe, "r");
	}
	if (permissions & TCL_FILE_WRITABLE) {
	    if (inPipe == -1) {
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_cannot_write_input);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
		free(i18n_msg_format);

		goto pipelineError;
	    }
	    if (f != NULL) {
		f2 = fdopen(inPipe, "w");
	    } else {
		f = fdopen(inPipe, "w");
	    }
	}
	Tcl_EnterFile(interp, f, permissions);
	oFilePtr = tclOpenFiles[fileno(f)];
	oFilePtr->f2 = f2;
	oFilePtr->numPids = numPids;
	oFilePtr->pidPtr = pidPtr;
	oFilePtr->errorId = errorId;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * GetOpenMode --
 *
 *	description.
 *
 * Results:
 *	Normally, sets *modePtr to an access mode for passing to "open",
 *	and returns a string that can be used as the access mode in a
 *	subsequent call to "fdopen".  If an error occurs, then returns
 *	NULL and sets interp->result to an error message.
 *
 * Side effects:
 *	None.
 *
 * Special note:
 *	This code is based on a prototype implementation contributed
 *	by Mark Diekhans.
 *
 *----------------------------------------------------------------------
 */

static char *
GetOpenMode(interp, string, modePtr)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting. */
    char *string;			/* Mode string, e.g. "r+" or
					 * "RDONLY CREAT". */
    int *modePtr;			/* Where to store mode corresponding
					 * to string. */
{
    int mode, modeArgc, c, i, gotRW;
    char **modeArgv, *flag;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
#define RW_MODES (O_RDONLY|O_WRONLY|O_RDWR)

    /*
     * Check for the simpler fopen-like access modes (e.g. "r").  They
     * are distinguished from the POSIX access modes by the presence
     * of a lower-case first letter.
     */

    mode = 0;
    if (islower(UCHAR(string[0]))) {
	switch (string[0]) {
	    case 'r':
		mode = O_RDONLY;
		break;
	    case 'w':
		mode = O_WRONLY|O_CREAT|O_TRUNC;
		break;
	    case 'a':
		mode = O_WRONLY|O_CREAT|O_APPEND;
		break;
	    default:
		error:
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_illegal_access_mode);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(string)
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, string);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);

		return NULL;
	}
	if (string[1] == '+') {
	    mode &= ~(O_RDONLY|O_WRONLY);
	    mode |= O_RDWR;
	    if (string[2] != 0) {
		goto error;
	    }
	} else if (string[1] != 0) {
	    goto error;
	}
	*modePtr = mode;
	return string;
    }

    /*
     * The access modes are specified using a list of POSIX modes
     * such as O_CREAT.
     */

    if (Tcl_SplitList(interp, string, &modeArgc, &modeArgv) != TCL_OK) {
	i18n_msg_format = dce_msg_get(tcl_s_unixza_proc_open_access_mode);
	Tcl_AddErrorInfo(interp, (char *)i18n_msg_format);
	free(i18n_msg_format);

	Tcl_AddErrorInfo(interp, string);
	Tcl_AddErrorInfo(interp, "\"");
	return NULL;
    }
    gotRW = 0;
    for (i = 0; i < modeArgc; i++) {
	flag = modeArgv[i];
	c = flag[0];
	if ((c == 'R') && (strcmp(flag, "RDONLY") == 0)) {
	    mode = (mode & ~RW_MODES) | O_RDONLY;
	    gotRW = 1;
	} else if ((c == 'W') && (strcmp(flag, "WRONLY") == 0)) {
	    mode = (mode & ~RW_MODES) | O_WRONLY;
	    gotRW = 1;
	} else if ((c == 'R') && (strcmp(flag, "RDWR") == 0)) {
	    mode = (mode & ~RW_MODES) | O_RDWR;
	    gotRW = 1;
	} else if ((c == 'A') && (strcmp(flag, "APPEND") == 0)) {
	    mode |= O_APPEND;
	} else if ((c == 'C') && (strcmp(flag, "CREAT") == 0)) {
	    mode |= O_CREAT;
	} else if ((c == 'E') && (strcmp(flag, "EXCL") == 0)) {
	    mode |= O_EXCL;
	} else if ((c == 'N') && (strcmp(flag, "NOCTTY") == 0)) {
#ifdef O_NOCTTY
	    mode |= O_NOCTTY;
#else
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_access_mode_not_supported);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(flag)
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, flag);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    ckfree((char *) modeArgv);
	    return NULL;
#endif
	} else if ((c == 'N') && (strcmp(flag, "NONBLOCK") == 0)) {
#ifdef O_NONBLOCK
	    mode |= O_NONBLOCK;
#else
	    mode |= O_NDELAY;
#endif
	} else if ((c == 'T') && (strcmp(flag, "TRUNC") == 0)) {
	    mode |= O_TRUNC;
	} else {
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_invalid_access_mode);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(flag)
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, flag);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    ckfree((char *) modeArgv);
	    return NULL;
	}
    }
    ckfree((char *) modeArgv);
    if (!gotRW) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_access_mode_must_inc);
	Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
	free(i18n_msg_format);

	return NULL;
    }
    *modePtr = mode;

    /*
     * The calculation of fdopen access mode below isn't really correct,
     * but it doesn't have to be.  All it has to do is to disinguish
     * read and write permissions, plus indicate append mode.
     */

    i = mode & RW_MODES;
    if (i == O_RDONLY) {
	return "r";
    }
    if (mode & O_APPEND) {
	if (i == O_WRONLY) {
	    return "a";
	} else {
	    return "a+";
	}
    }
    if (i == O_WRONLY) {
	return "w";
    }
    return "r+";
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_PidCmd --
 *
 *	This procedure is invoked to process the "pid" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_PidCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    FILE *f;
    OpenFile *oFilePtr;
    int i;
    char string[50];
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc > 2) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_opt_fileId);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (argc == 1) {
	sprintf(interp->result, "%d", getpid());
    } else {
	if (Tcl_GetOpenFile(interp, argv[1], 0, 0, &f) != TCL_OK) {
	    return TCL_ERROR;
	}
	oFilePtr = tclOpenFiles[fileno(f)];
	for (i = 0; i < oFilePtr->numPids; i++) {
	    sprintf(string, "%d", oFilePtr->pidPtr[i]);
	    Tcl_AppendElement(interp, string);
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_PutsCmd --
 *
 *	This procedure is invoked to process the "puts" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_PutsCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    FILE *f;
    int i, newline;
    char *fileId;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    i = 1;
    newline = 1;
    if ((argc >= 2) && (strcmp(argv[1], "-nonewline") == 0)) {
	newline = 0;
	i++;
    }
    if ((i < (argc-3)) || (i >= argc)) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_nonew_fileid_str);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * The code below provides backwards compatibility with an old
     * form of the command that is no longer recommended or documented.
     */

    if (i == (argc-3)) {
	if (strncmp(argv[i+2], "nonewline", strlen(argv[i+2])) != 0) {
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_bad_arg_nonewline);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[i+2])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[i+2]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	newline = 0;
    }
    if (i == (argc-1)) {
	fileId = "stdout";
    } else {
	fileId = argv[i];
	i++;
    }

    if (Tcl_GetOpenFile(interp, fileId, 1, 1, &f) != TCL_OK) {
	return TCL_ERROR;
    }

    clearerr(f);
    fputs(argv[i], f);
    if (newline) {
	fputc('\n', f);
    }
    if (ferror(f)) {
	char *i18n_temp;

	i18n_temp = Tcl_PosixError(interp);
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_writing);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(fileId)
				+ strlen(i18n_temp)
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, fileId, i18n_temp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_PwdCmd --
 *
 *	This procedure is invoked to process the "pwd" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_PwdCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    char buffer[MAXPATHLEN+1];
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc != 1) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_pwd);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (currentDir == NULL) {
	if (getcwd(buffer, MAXPATHLEN+1) == NULL) {
	    if (errno == ERANGE) {
		interp->result = (char *)dce_msg_get(tcl_s_unixaz_cwd_name_too_long);
		interp->freeProc = (Tcl_FreeProc *) free;
	    } else {
		char *i18n_temp;

		i18n_temp = Tcl_PosixError(interp);
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_with_cwd_name);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, i18n_temp);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);

	    }
	    return TCL_ERROR;
	}
	currentDir = (char *) ckalloc((unsigned) (strlen(buffer) + 1));
	strcpy(currentDir, buffer);
    }
    interp->result = currentDir;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ReadCmd --
 *
 *	This procedure is invoked to process the "read" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_ReadCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int bytesLeft, bytesRead, count;
#define READ_BUF_SIZE 4096
    char buffer[READ_BUF_SIZE+1];
    int newline, i;
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if ((argc != 2) && (argc != 3)) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid_numbytes_read);
	i18n_msg_buf = (unsigned char *)ckalloc((strlen(argv[0]) * 2)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0], argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    i = 1;
    newline = 1;
    if ((argc == 3) && (strcmp(argv[1], "-nonewline") == 0)) {
	newline = 0;
	i++;
    }
    if (Tcl_GetOpenFile(interp, argv[i], 0, 1, &f) != TCL_OK) {
	return TCL_ERROR;
    }

    /*
     * Compute how many bytes to read, and see whether the final
     * newline should be dropped.
     */

    if ((argc >= (i + 2)) && isdigit(UCHAR(argv[i+1][0]))) {
	if (Tcl_GetInt(interp, argv[i+1], &bytesLeft) != TCL_OK) {
	    return TCL_ERROR;
	}
    } else {
	bytesLeft = 1<<30;

	/*
	 * The code below provides backward compatibility for an
	 * archaic earlier version of this command.
	 */

	if (argc >= (i + 2)) {
	    if (strncmp(argv[i+1], "nonewline", strlen(argv[i+1])) == 0) {
		newline = 0;
	    } else {
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_bad_arg_nonewline);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[i+1])
						+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[i+1]);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);

		return TCL_ERROR;
	    }
	}
    }

    /*
     * Read the file in one or more chunks.
     */

    bytesRead = 0;
    clearerr(f);
    while (bytesLeft > 0) {
	count = READ_BUF_SIZE;
	if (bytesLeft < READ_BUF_SIZE) {
	    count = bytesLeft;
	}
	count = fread(buffer, 1, count, f);
	if (ferror(f)) {
	    /*
	     * If the file is in non-blocking mode, break out of the
	     * loop and return any bytes that were read.
	     */

	    if ((errno == EWOULDBLOCK) && ((count > 0) || (bytesRead > 0))) {
		clearerr(f);
		bytesLeft = count;
	    } else {
		char *i18n_temp;

		Tcl_ResetResult(interp);
		i18n_temp = Tcl_PosixError(interp);
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_reading);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[i])
				+ strlen((char *)i18n_temp)
				+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[i], i18n_temp);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);

		return TCL_ERROR;
	    }
	}
	if (count == 0) {
	    break;
	}
	buffer[count] = 0;
	Tcl_AppendResult(interp, buffer, (char *) NULL);
	bytesLeft -= count;
	bytesRead += count;
    }
    if ((newline == 0) && (bytesRead > 0)
	    && (interp->result[bytesRead-1] == '\n')) {
	interp->result[bytesRead-1] = 0;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SeekCmd --
 *
 *	This procedure is invoked to process the "seek" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_SeekCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    FILE *f;
    int offset, mode;
    unsigned char *i18n_msg_buf, *i18n_msg_format;

    if ((argc != 3) && (argc != 4)) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid_offset_org);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (Tcl_GetOpenFile(interp, argv[1], 0, 0, &f) != TCL_OK) {
	return TCL_ERROR;
    }
    if (Tcl_GetInt(interp, argv[2], &offset) != TCL_OK) {
	return TCL_ERROR;
    }
    mode = SEEK_SET;
    if (argc == 4) {
	int length;
	char c;

	length = strlen(argv[3]);
	c = argv[3][0];
	if ((c == 's') && (strncmp(argv[3], "start", length) == 0)) {
	    mode = SEEK_SET;
	} else if ((c == 'c') && (strncmp(argv[3], "current", length) == 0)) {
	    mode = SEEK_CUR;
	} else if ((c == 'e') && (strncmp(argv[3], "end", length) == 0)) {
	    mode = SEEK_END;
	} else {
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_bad_origin);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[3])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[3]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
    }
    clearerr(f);
    if (fseek(f, (long) offset, mode) == -1) {
	char *i18n_temp;

	i18n_temp = Tcl_PosixError(interp);
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_during_seek);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(i18n_temp)
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, i18n_temp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SourceCmd --
 *
 *	This procedure is invoked to process the "source" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_SourceCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc != 2) {
	i18n_msg_format = dce_msg_get(tcl_s_main_unixaz_wnum_args_filename);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    return Tcl_EvalFile(interp, argv[1]);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_TellCmd --
 *
 *	This procedure is invoked to process the "tell" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_TellCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc != 2) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (Tcl_GetOpenFile(interp, argv[1], 0, 0, &f) != TCL_OK) {
	return TCL_ERROR;
    }
    sprintf(interp->result, "%d", ftell(f));
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_TimeCmd --
 *
 *	This procedure is invoked to process the "time" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_TimeCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int count, i, result;
    double timePer;
#if NO_GETTOD
    struct tms dummy2;
    long start, stop;
#else
    struct timeval start, stop;
    struct timezone tz;
    int micros;
#endif
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc == 2) {
	count = 1;
    } else if (argc == 3) {
	if (Tcl_GetInt(interp, argv[2], &count) != TCL_OK) {
	    return TCL_ERROR;
	}
    } else {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_cmd_count);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
#if NO_GETTOD
    start = times(&dummy2);
#else
    gettimeofday(&start, &tz);
#endif
    for (i = count ; i > 0; i--) {
	result = Tcl_Eval(interp, argv[1]);
	if (result != TCL_OK) {
	    if (result == TCL_ERROR) {
		char msg[120];
		i18n_msg_format = dce_msg_get(tcl_s_unixza_time_body_line);
		sprintf(msg, (char *)i18n_msg_format, interp->errorLine);
		Tcl_AddErrorInfo(interp, msg);
		free(i18n_msg_format);
	    }
	    return result;
	}
    }
#if NO_GETTOD
    stop = times(&dummy2);
    timePer = (((double) (stop - start))*1000000.0)/CLK_TCK;
#else
    gettimeofday(&stop, &tz);
    micros = (stop.tv_sec - start.tv_sec)*1000000
	    + (stop.tv_usec - start.tv_usec);
    timePer = micros;
#endif
    Tcl_ResetResult(interp);
    i18n_msg_format = dce_msg_get(tcl_s_unixaz_microsec_interation);
    sprintf(interp->result, (char *)i18n_msg_format, timePer/count);
    free(i18n_msg_format);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * CleanupChildren --
 *
 *	This is a utility procedure used to wait for child processes
 *	to exit, record information about abnormal exits, and then
 *	collect any stderr output generated by them.
 *
 * Results:
 *	The return value is a standard Tcl result.  If anything at
 *	weird happened with the child processes, TCL_ERROR is returned
 *	and a message is left in interp->result.
 *
 * Side effects:
 *	If the last character of interp->result is a newline, then it
 *	is removed unless keepNewline is non-zero.  File errorId gets
 *	closed, and pidPtr is freed back to the storage allocator.
 *
 *----------------------------------------------------------------------
 */

static int
CleanupChildren(interp, numPids, pidPtr, errorId, keepNewline)
    Tcl_Interp *interp;		/* Used for error messages. */
    int numPids;		/* Number of entries in pidPtr array. */
    int *pidPtr;		/* Array of process ids of children. */
    int errorId;		/* File descriptor index for file containing
				 * stderr output from pipeline.  -1 means
				 * there isn't any stderr output. */
    int keepNewline;		/* Non-zero means don't discard trailing
				 * newline. */
{
    int result = TCL_OK;
    int i, pid, length, abnormalExit;
    WAIT_STATUS_TYPE waitStatus;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *i18n_temp;

    abnormalExit = 0;
    for (i = 0; i < numPids; i++) {
	pid = waitpid(pidPtr[i], (int *) &waitStatus, 0);
	if (pid == -1) {
	    i18n_temp = Tcl_PosixError(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_wait_exit);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, i18n_temp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    continue;
	}

	/*
	 * Create error messages for unusual process exits.  An
	 * extra newline gets appended to each error message, but
	 * it gets removed below (in the same fashion that an
	 * extra newline in the command's output is removed).
	 */

	if (!WIFEXITED(waitStatus) || (WEXITSTATUS(waitStatus) != 0)) {
	    char msg1[20], msg2[20];

	    result = TCL_ERROR;
	    sprintf(msg1, "%d", pid);
	    if (WIFEXITED(waitStatus)) {
		sprintf(msg2, "%d", WEXITSTATUS(waitStatus));
		Tcl_SetErrorCode(interp, "CHILDSTATUS", msg1, msg2,
			(char *) NULL);
		abnormalExit = 1;
	    } else if (WIFSIGNALED(waitStatus)) {
		char *p;
		char *i18n_check;
		int  i18n_result = 0;
	
		p = Tcl_SignalMsg((int) (WTERMSIG(waitStatus)));
		i18n_check = Tcl_SignalId((int) (WTERMSIG(waitStatus)), &i18n_result);
		Tcl_SetErrorCode(interp, "CHILDKILLED", msg1,
			i18n_check, p, (char *) NULL);
		if (i18n_result == 1)
			free(i18n_check);
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_child_killed);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(p)
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, p);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		free(p);
		ckfree(i18n_msg_buf);

	    } else if (WIFSTOPPED(waitStatus)) {
		char *p;
		char *i18n_check;
		int  i18n_result = 0;

		p = Tcl_SignalMsg((int) (WSTOPSIG(waitStatus)));
		i18n_check = Tcl_SignalId((int) (WSTOPSIG(waitStatus)), &i18n_result);
		Tcl_SetErrorCode(interp, "CHILDSUSP", msg1,
			i18n_check, p, (char *) NULL);
		if (i18n_result == 1)
			free(i18n_check);
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_child_suspended);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(p)
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, p);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		free(p);
		ckfree(i18n_msg_buf);

	    } else {
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_child_st_no_sense);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
		free(i18n_msg_format);

	    }
	}
    }
    ckfree((char *) pidPtr);

    /*
     * Read the standard error file.  If there's anything there,
     * then return an error and add the file's contents to the result
     * string.
     */

    if (errorId >= 0) {
	while (1) {
#	    define BUFFER_SIZE 1000
	    char buffer[BUFFER_SIZE+1];
	    int count;
    
	    count = read(errorId, buffer, (size_t) BUFFER_SIZE);
    
	    if (count == 0) {
		break;
	    }
	    result = TCL_ERROR;
	    if (count < 0) {
		i18n_temp = Tcl_PosixError(interp);
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_read_stderr);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, i18n_temp);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);

		break;
	    }
	    buffer[count] = 0;
	    Tcl_AppendResult(interp, buffer, (char *) NULL);
	}
	close(errorId);
    }

    /*
     * If a child exited abnormally but didn't output any error information
     * at all, generate an error message here.
     */

    if (abnormalExit && (*interp->result == 0)) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_child_abend_exit);
	Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
	free(i18n_msg_format);
    }

    /*
     * If the last character of interp->result is a newline, then remove
     * the newline character (the newline would just confuse things).
     * Special hack: must replace the old terminating null character
     * as a signal to Tcl_AppendResult et al. that we've mucked with
     * the string.
     */

    length = strlen(interp->result);
    if (!keepNewline && (length > 0) && (interp->result[length-1] == '\n')) {
	interp->result[length-1] = '\0';
	interp->result[length] = 'x';
    }

    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_WCdCmd --
 *
 *	This procedure is invoked to process the "cd" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WCdCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    char *dirName;
    Tcl_DString buffer;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *i18n_temp, *ctemp;
    size_t len, st;

    if (argc > 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_dirname);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("cd")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "cd");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    if (argc == 2) 
    {
	_WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;
	dirName = _Tcl_WTildeSubst(interp, ctemp, &buffer);
	ckfree(ctemp);
    } 
    else 
    {
	dirName = "~";
	dirName = _Tcl_WTildeSubst(interp, dirName, &buffer);
    }
    if (dirName == NULL) 
    {
	return TCL_ERROR;
    }
    if (currentDir != NULL) 
    {
	ckfree(currentDir);
	currentDir = NULL;
    }
    if (chdir(dirName) != 0) 
    {
	i18n_temp = Tcl_PosixError(interp);
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_cannot_change_work_dir);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(dirName)
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, dirName, i18n_temp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

        Tcl_DStringFree(&buffer);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&buffer);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WCloseCmd --
 *
 *	This procedure is invoked to process the "close" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WCloseCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    OpenFile *oFilePtr;
    int result = TCL_OK;
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *i18n_temp, *ctemp;
    size_t len, st;

    if (argc != 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("close")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "close");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if (Tcl_GetOpenFile(interp, ctemp, 0, 0, &f) != TCL_OK) 
    {
	ckfree(ctemp);
	return TCL_ERROR;
    }

    oFilePtr = tclOpenFiles[fileno(f)];
    tclOpenFiles[fileno(f)] = NULL;

    /*
     * First close the file (in the case of a process pipeline, there may
     * be two files, one for the pipe at each end of the pipeline).
     */

    if (oFilePtr->f2 != NULL) 
    {
	clearerr(oFilePtr->f2);
	if (fclose(oFilePtr->f2) == EOF) 
    {
	    i18n_temp = Tcl_PosixError(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_closing);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp, i18n_temp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    result = TCL_ERROR;
	}
    }
    clearerr(oFilePtr->f);
    if (fclose(oFilePtr->f) == EOF) 
    {
	i18n_temp = Tcl_PosixError(interp);
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_closing);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp, i18n_temp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(ctemp);

	result = TCL_ERROR;
    }
    ckfree(ctemp);

    /*
     * If the file was a connection to a pipeline, clean up everything
     * associated with the child processes.
     */

    if (oFilePtr->numPids > 0) 
    {
	if (CleanupChildren(interp, oFilePtr->numPids, oFilePtr->pidPtr,
		oFilePtr->errorId, 0) != TCL_OK) 
	{
	    result = TCL_ERROR;
	}
    }

    ckfree((char *) oFilePtr);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WEofCmd --
 *
 *	This procedure is invoked to process the "eof" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WEofCmd(notUsed, interp, argc, wargv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp;
    size_t len, st;

    if (argc != 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("eof")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "eof");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if (Tcl_GetOpenFile(interp, ctemp, 0, 0, &f) != TCL_OK) 
    {
	ckfree(ctemp);
	return TCL_ERROR;
    }
    ckfree(ctemp);
    if (feof(f)) 
    {
	interp->result = "1";
    } 
    else 
    {
	interp->result = "0";
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WExecCmd --
 *
 *	This procedure is invoked to process the "exec" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WExecCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int outputId;			/* File id for output pipe.  -1
					 * means command overrode. */
    int errorId;			/* File id for temporary file
					 * containing error output. */
    int *pidPtr;
    int numPids, result, keepNewline;
    int firstWord, i;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp;
    size_t len, st;
    char **nargv;		/* new argv for char */
    int  nargc = 0;		/* argc for new argv */
    wchar_t **wtempargv;

    /*
     * Check for a leading "-keepnewline" argument.
     */

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    keepNewline = 0;
    for (firstWord = 1; (firstWord < argc) 
	    && (wargv[firstWord][0] == i18n_special_chars.w_minus); firstWord++)
    {
	_WChar_To_Multibyte(interp, len, wargv[firstWord], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	if (strcmp(ctemp, "-keepnewline") == 0) 
	{
	    keepNewline = 1;
	} 
	else if (strcmp(ctemp, "--") == 0) 
	{
	    firstWord++;
	    break;
	} 
	else 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_bad_switch_keepnewline);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	ckfree(ctemp);
    }

    if (argc <= firstWord) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_switch_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("exec")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "exec");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * See if the command is to be run in background;  if so, create
     * the command, detach it, and return a list of pids.
     */

    if ((wargv[argc-1][0] == i18n_special_chars.w_and) 
	    && (wargv[argc-1][1] == i18n_special_chars.w_esc_zero)) 
    {
	char id[50];

	argc--;
	wargv[argc] = NULL;
	/*
	 * Create new argv array
	 */
	nargv = (char **)malloc(argc-firstWord * sizeof(char *));
	for (i = 0; i < argc-firstWord; i++)
	{
	    wtempargv = wargv+firstWord+i;
	    _WChar_To_Multibyte(interp, len, *wtempargv, nargv[i], st)
	    if (st == -1)
		nargv[i] = (char *)NULL;
	    nargc++;
	}

	numPids = Tcl_CreatePipeline(interp, nargc, nargv,
		&pidPtr, (int *) NULL, (int *) NULL, (int *) NULL);
	/* 
	 * Free working area
	 */
	i = nargc-1;
	while (i >= 0)
	{
	    free(nargv[i--]);
	}
	free(nargv);

	if (numPids < 0) 
	{
	    return TCL_ERROR;
	}
	Tcl_DetachPids(numPids, pidPtr);
	for (i = 0; i < numPids; i++) 
	{
	    sprintf(id, "%d", pidPtr[i]);
	    Tcl_AppendElement(interp, id);
	}
	ckfree((char *) pidPtr);
	return TCL_OK;
    }

    /*
     * Create new argv array
     */
    nargv = (char **)malloc(argc-firstWord * sizeof(char *));
    for (i = 0; i < argc-firstWord; i++)
    {
	wtempargv = wargv+firstWord+i;
	_WChar_To_Multibyte(interp, len, *wtempargv, nargv[i], st)
	if (st == -1)
	    nargv[i] = (char *)NULL;
	nargc++;
    }

    /*
     * Create the command's pipeline.
     */

    numPids = Tcl_CreatePipeline(interp, nargc, nargv,
	    &pidPtr, (int *) NULL, &outputId, &errorId);

    /* 
     * Free working area
     */
    i = nargc-1;
    while (i >= 0)
    {
	free(nargv[i--]);
    }
    free(nargv);

    if (numPids < 0) 
    {
	return TCL_ERROR;
    }

    /*
     * Read the child's output (if any) and put it into the result.
     */

    result = TCL_OK;
    if (outputId != -1) 
    {
	while (1) 
	{
#	    define BUFFER_SIZE 1000
	    char buffer[BUFFER_SIZE+1];
	    int count;
    
	    count = read(outputId, buffer, (size_t) BUFFER_SIZE);
    
	    if (count == 0) 
	    {
		break;
	    }
	    if (count < 0) 
	    {
		char *i18n_temp;

		Tcl_ResetResult(interp);
		i18n_temp = Tcl_PosixError(interp);
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_read_from_out_pipe);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(i18n_temp)
					   + strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, i18n_temp);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);

		result = TCL_ERROR;
		break;
	    }
	    buffer[count] = 0;
	    Tcl_AppendResult(interp, buffer, (char *) NULL);
	}
	close(outputId);
    }

    if (CleanupChildren(interp, numPids, pidPtr, errorId, keepNewline)
	    != TCL_OK) 
    {
	result = TCL_ERROR;
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WExitCmd --
 *
 *	This procedure is invoked to process the "exit" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WExitCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int value;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if ((argc != 1) && (argc != 2)) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_returncode);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("exit")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "exit");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (argc == 1) 
    {
	exit(0);
    }
    if (_Tcl_WGetInt(interp, wargv[1], &value) != TCL_OK) 
    {
	return TCL_ERROR;
    }
    exit(value);
    /*NOTREACHED*/
    return TCL_OK;			/* Better not ever reach this! */
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WFileCmd --
 *
 *	This procedure is invoked to process the "file" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WFileCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    char *p;
    int length, statOp, result;
    int mode = 0;			/* Initialized only to prevent
					 * compiler warning message. */
    struct stat statBuf;
    char *fileName, c;
    Tcl_DString buffer;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *cargv1, *cargv2, *cargv3;
    size_t len, st;

    if (argc < 3) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_opt_name_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("file")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "file");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    _WChar_To_Multibyte(interp, len, wargv[1], cargv1, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    c = *cargv1;
    length = strlen(cargv1);
    result = TCL_OK;

    /*
     * First handle operations on the file name.
     */

    _WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
    if (st == (size_t)-1)
    {
	ckfree(cargv1);
	return TCL_ERROR;
    }

    fileName = _Tcl_WTildeSubst(interp, cargv2, &buffer);
    if (fileName == NULL) 
    {
	ckfree(cargv1);
	ckfree(cargv2);
	result = TCL_ERROR;
	goto done;
    }
    if ((c == 'd') && (strncmp(cargv1, "dirname", length) == 0)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "dirname"; */
	    not3Args:
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_name);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("file")
					+ strlen(cargv1)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "file", cargv1);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(cargv1);
	    ckfree(cargv2);

	    result = TCL_ERROR;
	    goto done;
	}
	p = strrchr(fileName, '/');
	if (p == NULL) 
	{
	    interp->result = ".";
	} 
	else if (p == fileName) 
	{
	    interp->result = "/";
	} 
	else 
	{
	    *p = 0;
	    Tcl_SetResult(interp, fileName, TCL_VOLATILE);
	    *p = '/';
	}
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    } 
    else if ((c == 'r') && (strncmp(cargv1, "rootname", length) == 0)
	    && (length >= 2)) 
    {
	char *lastSlash;

	if (argc != 3) 
	{
	    /* argv[1] = "rootname"; */
	    goto not3Args;
	}
	p = strrchr(fileName, '.');
	lastSlash = strrchr(fileName, '/');
	if ((p == NULL) || ((lastSlash != NULL) && (lastSlash > p))) 
	{
	    Tcl_SetResult(interp, fileName, TCL_VOLATILE);
	} 
	else 
	{
	    *p = 0;
	    Tcl_SetResult(interp, fileName, TCL_VOLATILE);
	    *p = '.';
	}
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    } 
    else if ((c == 'e') && (strncmp(cargv1, "extension", length) == 0)
	    && (length >= 3)) 
    {
	char *lastSlash;

	if (argc != 3) 
	{
	    /* argv[1] = "extension" */;
	    goto not3Args;
	}
	p = strrchr(fileName, '.');
	lastSlash = strrchr(fileName, '/');
	if ((p != NULL) && ((lastSlash == NULL) || (lastSlash < p))) 
	{
	    Tcl_SetResult(interp, p, TCL_VOLATILE);
	}
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    } 
    else if ((c == 't') && (strncmp(cargv1, "tail", length) == 0)
	    && (length >= 2)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "tail"; */
	    goto not3Args;
	}
	p = strrchr(fileName, '/');
	if (p != NULL) 
	{
	    Tcl_SetResult(interp, p+1, TCL_VOLATILE);
	} 
	else 
	{
	    Tcl_SetResult(interp, fileName, TCL_VOLATILE);
	}
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    }

    /*
     * Next, handle operations that can be satisfied with the "access"
     * kernel call.
     */

    if (fileName == NULL) 
    {
	result = TCL_ERROR;
	goto done;
    }
    if ((c == 'r') && (strncmp(cargv1, "readable", length) == 0)
	    && (length >= 5)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "readable"; */
	    goto not3Args;
	}
	mode = R_OK;
	checkAccess:
	if (access(fileName, mode) == -1) 
	{
	    interp->result = "0";
	} 
	else 
	{
	    interp->result = "1";
	}
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    } 
    else if ((c == 'w') && (strncmp(cargv1, "writable", length) == 0)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "writable"; */
	    goto not3Args;
	}
	mode = W_OK;
	goto checkAccess;
    } 
    else if ((c == 'e') && (strncmp(cargv1, "executable", length) == 0)
	    && (length >= 3)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "executable"; */
	    goto not3Args;
	}
	mode = X_OK;
	goto checkAccess;
    } 
    else if ((c == 'e') && (strncmp(cargv1, "exists", length) == 0)
	    && (length >= 3)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "exists"; */
	    goto not3Args;
	}
	mode = F_OK;
	goto checkAccess;
    }

    /*
     * Lastly, check stuff that requires the file to be stat-ed.
     */

    if ((c == 'a') && (strncmp(cargv1, "atime", length) == 0)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "atime"; */
	    goto not3Args;
	}
	if (stat(fileName, &statBuf) == -1) 
	{
	    goto badStat;
	}
	sprintf(interp->result, "%ld", statBuf.st_atime);
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    } 
    else if ((c == 'i') && (strncmp(cargv1, "isdirectory", length) == 0)
	    && (length >= 3)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "isdirectory"; */
	    goto not3Args;
	}
	statOp = 2;
	ckfree(cargv1);
	ckfree(cargv2);
    } 
    else if ((c == 'i') && (strncmp(cargv1, "isfile", length) == 0)
	    && (length >= 3)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "isfile"; */
	    goto not3Args;
	}
	statOp = 1;
	ckfree(cargv1);
	ckfree(cargv2);
    } 
    else if ((c == 'l') && (strncmp(cargv1, "lstat", length) == 0)) 
    {
	if (argc != 4) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_lstat_name_var);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("file")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "file");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(cargv1);
	    ckfree(cargv2);

	    result = TCL_ERROR;
	    goto done;
	}

	if (lstat(fileName, &statBuf) == -1) 
	{
	    char *i18n_temp;

	    i18n_temp = Tcl_PosixError(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_no_lstat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv2)
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv2, i18n_temp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(cargv1);
	    ckfree(cargv2);

	    result = TCL_ERROR;
	    goto done;
	}
	_WChar_To_Multibyte(interp, len, wargv[3], cargv3, st)
	if (st == (size_t)-1)
	{
	    ckfree(cargv1);
	    ckfree(cargv2);
	    goto done;
	}
	result = StoreStatData(interp, cargv3, &statBuf);
	ckfree(cargv3);
	goto done;
    } 
    else if ((c == 'm') && (strncmp(cargv1, "mtime", length) == 0)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "mtime"; */
	    goto not3Args;
	}
	if (stat(fileName, &statBuf) == -1) 
	{
	    goto badStat;
	}
	sprintf(interp->result, "%ld", statBuf.st_mtime);
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    } 
    else if ((c == 'o') && (strncmp(cargv1, "owned", length) == 0)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "owned"; */
	    goto not3Args;
	}
	ckfree(cargv1);
	ckfree(cargv2);
	statOp = 0;
    } 
    else if ((c == 'r') && (strncmp(cargv1, "readlink", length) == 0)
	    && (length >= 5)) 
    {
	char linkValue[MAXPATHLEN+1];
	int linkLength;

	if (argc != 3) 
	{
	    /* argv[1] = "readlink"; */
	    goto not3Args;
	}

	/*
	 * If S_IFLNK isn't defined it means that the machine doesn't
	 * support symbolic links, so the file can't possibly be a
	 * symbolic link.  Generate an EINVAL error, which is what
	 * happens on machines that do support symbolic links when
	 * you invoke readlink on a file that isn't a symbolic link.
	 */

#ifndef S_IFLNK
	linkLength = -1;
	errno = EINVAL;
#else
	linkLength = readlink(fileName, linkValue, sizeof(linkValue) - 1);
#endif /* S_IFLNK */
	if (linkLength == -1) 
	{
	    char *i18n_temp;

	    i18n_temp = Tcl_PosixError(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_no_readlink);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv2)
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv2, i18n_temp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(cargv1);
	    ckfree(cargv2);

	    result = TCL_ERROR;
	    goto done;
	}
	linkValue[linkLength] = 0;
	Tcl_SetResult(interp, linkValue, TCL_VOLATILE);
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    } 
    else if ((c == 's') && (strncmp(cargv1, "size", length) == 0)
	    && (length >= 2)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "size"; */
	    goto not3Args;
	}
	if (stat(fileName, &statBuf) == -1) 
	{
	    goto badStat;
	}
	sprintf(interp->result, "%ld", statBuf.st_size);
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    } 
    else if ((c == 's') && (strncmp(cargv1, "stat", length) == 0)
	    && (length >= 2)) 
    {
	if (argc != 4) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_stat_name_var);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("file")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "file");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(cargv1);
	    ckfree(cargv2);

	    result = TCL_ERROR;
	    goto done;
	}

	if (stat(fileName, &statBuf) == -1) 
	{
	    char *i18n_temp;

	    badStat:
	    i18n_temp = Tcl_PosixError(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_no_stat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv2)
					+ strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv2, i18n_temp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(cargv1);
	    ckfree(cargv2);

	    result = TCL_ERROR;
	    goto done;
	}
	_WChar_To_Multibyte(interp, len, wargv[3], cargv3, st)
	if (st == (size_t)-1)
	{
	    ckfree(cargv1);
	    ckfree(cargv2);
	    goto done;
	}
	result = StoreStatData(interp, cargv3, &statBuf);
	ckfree(cargv3);
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    } 
    else if ((c == 't') && (strncmp(cargv1, "type", length) == 0)
	    && (length >= 2)) 
    {
	if (argc != 3) 
	{
	    /* argv[1] = "type"; */
	    ckfree(cargv1);
	    ckfree(cargv2);
	    goto not3Args;
	}
	if (lstat(fileName, &statBuf) == -1) 
	{
	    goto badStat;
	}
	interp->result = GetFileType((int) statBuf.st_mode);
	interp->freeProc = (Tcl_FreeProc *) free;
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    } 
    else 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixza_bad_opt_atime_dirname_exec);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv1)
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv1);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(cargv1);
	ckfree(cargv2);

	result = TCL_ERROR;
	goto done;
    }
    if (stat(fileName, &statBuf) == -1) 
    {
	interp->result = "0";
	ckfree(cargv1);
	ckfree(cargv2);
	goto done;
    }
    switch (statOp) 
    {
	case 0:
	    mode = (geteuid() == statBuf.st_uid);
	    break;
	case 1:
	    mode = S_ISREG(statBuf.st_mode);
	    break;
	case 2:
	    mode = S_ISDIR(statBuf.st_mode);
	    break;
    }
    if (mode) 
    {
	interp->result = "1";
    } 
    else 
    {
	interp->result = "0";
    }
    ckfree(cargv1);
    ckfree(cargv2);

    done:
    Tcl_DStringFree(&buffer);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WFlushCmd --
 *
 *	This procedure is invoked to process the "flush" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WFlushCmd(notUsed, interp, argc, wargv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp;
    size_t len, st;

    if (argc != 2) {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("flush")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "flush");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if (Tcl_GetOpenFile(interp, ctemp, 1, 1, &f) != TCL_OK) 
    {
	ckfree(ctemp);
	return TCL_ERROR;
    }
    clearerr(f);
    if (fflush(f) == EOF) 
    {
	char *i18n_temp;

	i18n_temp = Tcl_PosixError(interp);
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_flushing);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
				+ strlen(i18n_temp)
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp, i18n_temp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(ctemp);

	return TCL_ERROR;
    }
    ckfree(ctemp);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WGetsCmd --
 *
 *	This procedure is invoked to process the "gets" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WGetsCmd(notUsed, interp, argc, wargv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
#   define BUF_SIZE 200
    char buffer[BUF_SIZE+1];
    int totalCount, done, flags;
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *i18n_temp, *ctemp;
    wchar_t *wtemp;
    size_t len, st;

    if ((argc != 2) && (argc != 3)) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid_varname);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("gets")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "gets");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if (Tcl_GetOpenFile(interp, ctemp, 0, 1, &f) != TCL_OK) 
    {
	ckfree(ctemp);
	return TCL_ERROR;
    }

    /*
     * We can't predict how large a line will be, so read it in
     * pieces, appending to the current result or to a variable.
     */

    totalCount = 0;
    done = 0;
    flags = 0;
    clearerr(f);
    while (!done) 
    {
	register int c, count;
	register char *p;

	for (p = buffer, count = 0; count < BUF_SIZE-1; count++, p++) 
	{
	    c = getc(f);
	    if (c == EOF) 
	    {
		if (ferror(f)) 
		{
		    /*
		     * If the file is in non-blocking mode, return any
		     * bytes that were read before a block would occur.
		     */

		    if ((errno == EWOULDBLOCK)
			    && ((count > 0 || totalCount > 0))) 
		    {
			done = 1;
			break;
		    }
		    Tcl_ResetResult(interp);
		    i18n_temp = Tcl_PosixError(interp);
		    i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_reading);
		    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					    +strlen(i18n_temp)
					    + strlen((char *)i18n_msg_format) + 1);
		    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp, i18n_temp);
		    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		    free(i18n_msg_format);
		    ckfree(i18n_msg_buf);
		    ckfree(ctemp);

		    return TCL_ERROR;
		} 
		else if (feof(f)) 
		{
		    if ((totalCount == 0) && (count == 0)) 
		    {
			totalCount = -1;
		    }
		    done = 1;
		    break;
		}
	    }
	    if (c == '\n') 
	    {
		done = 1;
		break;
	    }
	    *p = c;
	}
	*p = 0;
	if (argc == 2) 
	{
	    Tcl_AppendResult(interp, buffer, (char *) NULL);
	} 
	else 
	{
	    _Multibyte_to_WChar(interp, len, buffer, wtemp, st)
	    if (st == (size_t)-1)
	    {
		ckfree(ctemp);
		return TCL_ERROR;
	    }

	    if (_Tcl_WSetVar(interp, wargv[2], wtemp, flags|TCL_LEAVE_ERR_MSG)
		    == NULL) 
	    {
		ckfree(ctemp);
		ckfree(wtemp);
		return TCL_ERROR;
	    }
	    ckfree(wtemp);
	    flags = TCL_APPEND_VALUE;
	}
	totalCount += count;
    }
    ckfree(ctemp);

    if (argc == 3) 
    {
	sprintf(interp->result, "%d", totalCount);
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WOpenCmd --
 *
 *	This procedure is invoked to process the "open" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WOpenCmd(notUsed, interp, argc, wargv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int pipeline, fd, mode, prot, readWrite, permissions;
    char *access;
    FILE *f, *f2;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp;
    size_t len, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if ((argc < 2) || (argc > 4)) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fname_acc_per);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("open")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "open");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    prot = 0666;
    if (argc == 2) 
    {
	mode = O_RDONLY;
	access = "r";
    } 
    else 
    {
	_WChar_To_Multibyte(interp, len, wargv[2], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	access = GetOpenMode(interp, ctemp, &mode);
	if (access == NULL) 
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	if (argc == 4) 
	{
	    if (_Tcl_WGetInt(interp, wargv[3], &prot) != TCL_OK) 
	    {
		return TCL_ERROR;
	    }
	}
	ckfree(ctemp);
    }

    f = f2 = NULL;
    readWrite = mode & (O_RDWR|O_RDONLY|O_WRONLY);
    if (readWrite == O_RDONLY) 
    {
	permissions = TCL_FILE_READABLE;
    } 
    else if (readWrite == O_WRONLY) 
    {
	permissions = TCL_FILE_WRITABLE;
    } 
    else 
    {
	permissions = TCL_FILE_READABLE|TCL_FILE_WRITABLE;
    }

    pipeline = 0;
    if (wargv[1][0] == i18n_special_chars.w_wall) 
    {
	pipeline = 1;
    }

    /*
     * Open the file or create a process pipeline.
     */

    if (!pipeline) 
    {
	char *fileName;
	Tcl_DString buffer;

	_WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	fileName = _Tcl_WTildeSubst(interp, ctemp, &buffer);
	if (fileName == NULL) 
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	fd = open(fileName, mode, prot);
	Tcl_DStringFree(&buffer);
	if (fd < 0) 
	{
	    char *i18n_temp;

	    i18n_temp = Tcl_PosixError(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_cannot_open);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
				    +strlen(i18n_temp)
				    + strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp, i18n_temp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	f = fdopen(fd, access);
	if (f == NULL) 
	{
	    close(fd);
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	Tcl_EnterFile(interp, f, permissions);
    } 
    else 
    {
	int *inPipePtr, *outPipePtr;
	int cmdArgc, inPipe, outPipe, numPids, *pidPtr, errorId;
	wchar_t **wcmdArgv;
	OpenFile *oFilePtr;
	char **nargv;
	int i, nargc = 0;
	wchar_t **wtempargv;

	if (_Tcl_WSplitList(interp, wargv[1]+1, &cmdArgc, &wcmdArgv) != TCL_OK) 
	{
	    return TCL_ERROR;
	}
	inPipePtr = (permissions & TCL_FILE_WRITABLE) ? &inPipe : NULL;
	outPipePtr = (permissions & TCL_FILE_READABLE) ? &outPipe : NULL;
	inPipe = outPipe = errorId = -1;

	/*
	 * Create new argv array
	 */
	nargv = (char **)malloc(cmdArgc * sizeof(char *));
	for (i = 0; i < cmdArgc; i++)
	{
	    wtempargv = wcmdArgv+i;
	    _WChar_To_Multibyte(interp, len, *wtempargv, nargv[i], st)
	    if (st == -1)
		nargv[i] = (char *)NULL;
	    nargc++;
	}

	numPids = Tcl_CreatePipeline(interp, nargc, nargv,
		&pidPtr, inPipePtr, outPipePtr, &errorId);
	/*
	 * Free working area
	 */
	ckfree((wchar_t *) wcmdArgv);
	i = nargc-1;
	while (i >= 0)
	{
	    free(nargv[i--]);
	}
	free(nargv);

	if (numPids < 0) 
	{
	    pipelineError:
	    if (f != NULL) 
	    {
		fclose(f);
	    }
	    if (f2 != NULL) 
	    {
		fclose(f2);
	    }
	    if (numPids > 0) 
	    {
		Tcl_DetachPids(numPids, pidPtr);
		ckfree((char *) pidPtr);
	    }
	    if (errorId != -1) 
	    {
		close(errorId);
	    }
	    return TCL_ERROR;
	}
	if (permissions & TCL_FILE_READABLE) 
	{
	    if (outPipe == -1) 
	    {
		if (inPipe != -1) 
		{
		    close(inPipe);
		}
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_cannot_read_output);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
		free(i18n_msg_format);

		goto pipelineError;
	    }
	    f = fdopen(outPipe, "r");
	}
	if (permissions & TCL_FILE_WRITABLE) 
	{
	    if (inPipe == -1) 
	    {
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_cannot_write_input);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
		free(i18n_msg_format);

		goto pipelineError;
	    }
	    if (f != NULL) 
	    {
		f2 = fdopen(inPipe, "w");
	    } 
	    else 
	    {
		f = fdopen(inPipe, "w");
	    }
	}
	Tcl_EnterFile(interp, f, permissions);
	oFilePtr = tclOpenFiles[fileno(f)];
	oFilePtr->f2 = f2;
	oFilePtr->numPids = numPids;
	oFilePtr->pidPtr = pidPtr;
	oFilePtr->errorId = errorId;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WPidCmd --
 *
 *	This procedure is invoked to process the "pid" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WPidCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    FILE *f;
    OpenFile *oFilePtr;
    int i;
    char string[50];
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp;
    size_t len, st;

    if (argc > 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_opt_fileId);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("pid")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "pid");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (argc == 1) 
    {
	sprintf(interp->result, "%d", getpid());
    } 
    else 
    {
	_WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	if (Tcl_GetOpenFile(interp, ctemp, 0, 0, &f) != TCL_OK) 
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	ckfree(ctemp);
	oFilePtr = tclOpenFiles[fileno(f)];
	for (i = 0; i < oFilePtr->numPids; i++) 
	{
	    sprintf(string, "%d", oFilePtr->pidPtr[i]);
	    Tcl_AppendElement(interp, string);
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WPutsCmd --
 *
 *	This procedure is invoked to process the "puts" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WPutsCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    FILE *f;
    int i, newline;
    char *fileId;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp, *ctemp2;
    int fileId_alloc = 0;
    size_t len, st;

    i = 1;
    newline = 1;
    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if ((argc >= 2) && (strcmp(ctemp, "-nonewline") == 0)) 
    {
	newline = 0;
	i++;
    }
    ckfree(ctemp);

    if ((i < (argc-3)) || (i >= argc)) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_nonew_fileid_str);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("puts")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "puts");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * The code below provides backwards compatibility with an old
     * form of the command that is no longer recommended or documented.
     */

    if (i == (argc-3)) 
    {
	_WChar_To_Multibyte(interp, len, wargv[i+2], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	if (strncmp(ctemp, "nonewline", strlen(ctemp)) != 0) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_bad_arg_nonewline);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	newline = 0;
    }
    if (i == (argc-1)) 
    {
	fileId = "stdout";
    } 
    else 
    {
	_WChar_To_Multibyte(interp, len, wargv[i], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	fileId = ctemp;
	fileId_alloc = 1;
	i++;
    }

    if (Tcl_GetOpenFile(interp, fileId, 1, 1, &f) != TCL_OK) 
    {
	if (fileId_alloc)
	    ckfree(ctemp);
	return TCL_ERROR;
    }

    clearerr(f);
    _WChar_To_Multibyte(interp, len, wargv[i], ctemp2, st)
    if (st == (size_t)-1)
    {
	if (fileId_alloc)
	    ckfree(ctemp);
	return TCL_ERROR;
    }

    fputs(ctemp2, f);
    if (newline) 
    {
	fputc('\n', f);
    }
    if (ferror(f)) 
    {
	char *i18n_temp;

	i18n_temp = Tcl_PosixError(interp);
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_writing);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(fileId)
				+ strlen(i18n_temp)
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, fileId, i18n_temp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	if (fileId_alloc)
	    ckfree(ctemp);
	ckfree(ctemp2);

	return TCL_ERROR;
    }
    if (fileId_alloc)
	ckfree(ctemp);
    ckfree(ctemp2);

    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WPwdCmd --
 *
 *	This procedure is invoked to process the "pwd" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WPwdCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    char buffer[MAXPATHLEN+1];
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc != 1) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_pwd);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("pwd")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "pwd");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (currentDir == NULL) 
    {
	if (getcwd(buffer, MAXPATHLEN+1) == NULL) 
	{
	    if (errno == ERANGE) 
	    {
		interp->result = (char *)dce_msg_get(tcl_s_unixaz_cwd_name_too_long);
		interp->freeProc = (Tcl_FreeProc *) free;
	    } 
	    else 
	    {
		char *i18n_temp;

		i18n_temp = Tcl_PosixError(interp);
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_with_cwd_name);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(i18n_temp)
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, i18n_temp);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);

	    }
	    return TCL_ERROR;
	}
	currentDir = (char *) ckalloc((unsigned) (strlen(buffer) + 1));
	strcpy(currentDir, buffer);
    }
    interp->result = currentDir;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WReadCmd --
 *
 *	This procedure is invoked to process the "read" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WReadCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int bytesLeft, bytesRead, count;
#define READ_BUF_SIZE 4096
    char buffer[READ_BUF_SIZE+1];
    int newline, i;
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp;
    size_t len, st;

    if ((argc != 2) && (argc != 3)) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid_numbytes_read);
	i18n_msg_buf = (unsigned char *)ckalloc((strlen("read") * 2)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "read", "read");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    i = 1;
    newline = 1;

    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if ((argc == 3) && (strcmp(ctemp, "-nonewline") == 0)) 
    {
	newline = 0;
	i++;
    }
    ckfree(ctemp);

    _WChar_To_Multibyte(interp, len, wargv[i], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if (Tcl_GetOpenFile(interp, ctemp, 0, 1, &f) != TCL_OK) 
    {
	ckfree(ctemp);
	return TCL_ERROR;
    }
    ckfree(ctemp);

    /*
     * Compute how many bytes to read, and see whether the final
     * newline should be dropped.
     */

    if ((argc >= (i + 2)) && iswdigit(wargv[i+1][0])) 
    {
	if (_Tcl_WGetInt(interp, wargv[i+1], &bytesLeft) != TCL_OK) 
	{
	    return TCL_ERROR;
	}
    } 
    else 
    {
	bytesLeft = 1<<30;

	/*
	 * The code below provides backward compatibility for an
	 * archaic earlier version of this command.
	 */

	if (argc >= (i + 2)) 
	{
	    _WChar_To_Multibyte(interp, len, wargv[i+1], ctemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    if (strncmp(ctemp, "nonewline", strlen(ctemp)) == 0) 
	    {
		newline = 0;
	    } 
	    else 
	    {
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_bad_arg_nonewline);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
						+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);
		ckfree(ctemp);

		return TCL_ERROR;
	    }
	    ckfree(ctemp);
	}
    }

    /*
     * Read the file in one or more chunks.
     */

    bytesRead = 0;
    clearerr(f);
    while (bytesLeft > 0) 
    {
	count = READ_BUF_SIZE;
	if (bytesLeft < READ_BUF_SIZE) 
	{
	    count = bytesLeft;
	}
	count = fread(buffer, 1, count, f);
	if (ferror(f)) 
	{
	    /*
	     * If the file is in non-blocking mode, break out of the
	     * loop and return any bytes that were read.
	     */

	    if ((errno == EWOULDBLOCK) && ((count > 0) || (bytesRead > 0))) 
	    {
		clearerr(f);
		bytesLeft = count;
	    } 
	    else 
	    {
		char *i18n_temp;

		_WChar_To_Multibyte(interp, len, wargv[i], ctemp, st)
		if (st == (size_t)-1)
		    return TCL_ERROR;

		Tcl_ResetResult(interp);
		i18n_temp = Tcl_PosixError(interp);
		i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_reading);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
				+ strlen((char *)i18n_temp)
				+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp, i18n_temp);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);
		ckfree(ctemp);

		return TCL_ERROR;
	    }
	}
	if (count == 0) 
	{
	    break;
	}
	buffer[count] = 0;
	Tcl_AppendResult(interp, buffer, (char *) NULL);
	bytesLeft -= count;
	bytesRead += count;
    }
    if ((newline == 0) && (bytesRead > 0)
	    && (interp->result[bytesRead-1] == '\n')) 
    {
	interp->result[bytesRead-1] = 0;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WSeekCmd --
 *
 *	This procedure is invoked to process the "seek" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WSeekCmd(notUsed, interp, argc, wargv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    FILE *f;
    int offset, mode;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    size_t len, st;

    if ((argc != 3) && (argc != 4)) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid_offset_org);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("seek")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "seek");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if (Tcl_GetOpenFile(interp, ctemp, 0, 0, &f) != TCL_OK) 
    {
	ckfree(ctemp);
	return TCL_ERROR;
    }
    ckfree(ctemp);

    if (_Tcl_WGetInt(interp, wargv[2], &offset) != TCL_OK) 
    {
	return TCL_ERROR;
    }
    mode = SEEK_SET;
    if (argc == 4) 
    {
	int length;
	char c;

	_WChar_To_Multibyte(interp, len, wargv[3], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	length = strlen(ctemp);
	c = *ctemp;
	if ((c == 's') && (strncmp(ctemp, "start", length) == 0)) 
	{
	    mode = SEEK_SET;
	} 
	else if ((c == 'c') && (strncmp(ctemp, "current", length) == 0)) 
	{
	    mode = SEEK_CUR;
	} 
	else if ((c == 'e') && (strncmp(ctemp, "end", length) == 0)) 
	{
	    mode = SEEK_END;
	} 
	else 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_unixaz_bad_origin);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	ckfree(ctemp);
    }
    clearerr(f);
    if (fseek(f, (long) offset, mode) == -1) 
    {
	char *i18n_temp;

	i18n_temp = Tcl_PosixError(interp);
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_error_during_seek);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(i18n_temp)
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, i18n_temp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WSourceCmd --
 *
 *	This procedure is invoked to process the "source" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WSourceCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp;
    size_t clen, st;
    int ret_val;

    if (argc != 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_main_unixaz_wnum_args_filename);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("source")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "source");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    _WChar_To_Multibyte(interp, clen, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    ret_val = _Tcl_WEvalFile(interp, ctemp);
    ckfree(ctemp);
    return ret_val;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WTellCmd --
 *
 *	This procedure is invoked to process the "tell" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WTellCmd(notUsed, interp, argc, wargv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    FILE *f;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp;
    size_t len, st;

    if (argc != 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_fileid);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("tell")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "tell");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if (Tcl_GetOpenFile(interp, ctemp, 0, 0, &f) != TCL_OK) 
    {
	ckfree(ctemp);
	return TCL_ERROR;
    }
    sprintf(interp->result, "%d", ftell(f));
    ckfree(ctemp);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WTimeCmd --
 *
 *	This procedure is invoked to process the "time" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WTimeCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int count, i, result;
    double timePer;
#if NO_GETTOD
    struct tms dummy2;
    long start, stop;
#else
    struct timeval start, stop;
    struct timezone tz;
    int micros;
#endif
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc == 2) 
    {
	count = 1;
    } 
    else if (argc == 3) 
    {
	if (_Tcl_WGetInt(interp, wargv[2], &count) != TCL_OK) 
	{
	    return TCL_ERROR;
	}
    } 
    else 
    {
	i18n_msg_format = dce_msg_get(tcl_s_unixaz_wnum_args_cmd_count);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("time")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "time");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
#if NO_GETTOD
    start = times(&dummy2);
#else
    gettimeofday(&start, &tz);
#endif
    for (i = count ; i > 0; i--) 
    {
	result = _Tcl_WEval(interp, wargv[1]);
	if (result != TCL_OK) 
	{
	    if (result == TCL_ERROR) 
	    {
		char msg[120];
		i18n_msg_format = dce_msg_get(tcl_s_unixza_time_body_line);
		sprintf(msg, (char *)i18n_msg_format, interp->errorLine);
		Tcl_AddErrorInfo(interp, msg);
		free(i18n_msg_format);
	    }
	    return result;
	}
    }
#if NO_GETTOD
    stop = times(&dummy2);
    timePer = (((double) (stop - start))*1000000.0)/CLK_TCK;
#else
    gettimeofday(&stop, &tz);
    micros = (stop.tv_sec - start.tv_sec)*1000000
	    + (stop.tv_usec - start.tv_usec);
    timePer = micros;
#endif
    Tcl_ResetResult(interp);
    i18n_msg_format = dce_msg_get(tcl_s_unixaz_microsec_interation);
    sprintf(interp->result, (char *)i18n_msg_format, timePer/count);
    free(i18n_msg_format);
    return TCL_OK;
}
