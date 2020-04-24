/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: tclMain.c,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.8.1  1995/02/16  22:00:33  levy
 * 	Submission of 1.1 unintegrated.
 * 	[1995/02/16  20:40:23  levy]
 *
 * Revision 1.1.6.1  1994/10/21  21:39:15  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:32:32  mori_m]
 * 
 * Revision 1.1.2.2  1994/08/31  22:39:34  mori_m
 * 	CR 11259:  Overdone messaging:  deleted tcl_s_main_exit.
 * 	[1994/08/31  22:38:15  mori_m]
 * 
 * Revision 1.1.2.1  1994/07/12  21:19:26  mori_m
 * 	CR 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:12:57  mori_m]
 * 
 * $EndLog$
 */
/* 
 * main.c --
 *
 *	Main program for Tcl shells and other Tcl-based applications.
 *
 * Copyright (c) 1988-1993 The Regents of the University of California.
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

#include <stdio.h>
#include <tcl.h>
#include <errno.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>
#include <locale.h>
#include "tclWInt.h"

/*
 * Declarations for various library procedures and variables (don't want
 * to include tclUnix.h here, because people might copy this file out of
 * the Tcl source directory to make their own modified versions).
 */

extern int		errno;
extern void		exit _ANSI_ARGS_((int status));
extern int		isatty _ANSI_ARGS_((int fd));
extern char *		strcpy _ANSI_ARGS_((char *dst, CONST char *src));

static Tcl_Interp *interp;	/* Interpreter for application. */
static Tcl_DString command;	/* Used to buffer incomplete commands being
				 * read from stdin. */
char *tcl_RcFileName = NULL;	/* Name of a user-specific startup script
				 * to source if the application is being run
				 * interactively (e.g. "~/.tclshrc").  Set
				 * by Tcl_AppInit.  NULL means don't source
				 * anything ever. */
#ifdef TCL_MEM_DEBUG
static char dumpFile[100];	/* Records where to dump memory allocation
				 * information. */
static int quitFlag = 0;	/* 1 means the "checkmem" command was
				 * invoked, so the application should quit
				 * and dump memory allocation information. */
#endif

/*
 * Forward references for procedures defined later in this file:
 */

static int		CheckmemCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char *argv[]));
/*
 * Global variable for I18N
 */
i18nSpecChars i18n_special_chars;
i18nParseChars i18n_parse_chars;

/*
 *----------------------------------------------------------------------
 *
 * main --
 *
 *	This is the main program for a Tcl-based shell that reads
 *	Tcl commands from standard input.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Can be almost arbitrary, depending on what the Tcl commands do.
 *
 *----------------------------------------------------------------------
 */

int
main(argc, argv)
    int argc;				/* Number of arguments. */
    char **argv;			/* Array of argument strings. */
{
    char buffer[1000], *cmd, *args, *fileName;
    int code, gotPartial, tty;
    int exitCode = 0;
    unsigned char *i18n_msg_format;
    error_status_t msg_st;

    dce_msg_define_msg_table (dce_tcl_g_table,
			sizeof dce_tcl_g_table / sizeof dce_tcl_g_table[0],
			&msg_st);

    if (msg_st != rpc_s_ok)
		fprintf(stderr, "Unable to open message table\n");

    (void)setlocale(LC_ALL, "");	/* setting a current locale */

    i18n_special_chars.init_flag = 0;	/* mark special character table
					   un-initialized */

    interp = Tcl_CreateInterp();
#ifdef TCL_MEM_DEBUG
    Tcl_InitMemory(interp);
    Tcl_CreateCommand(interp, "checkmem", CheckmemCmd, (ClientData) 0,
	    (Tcl_CmdDeleteProc *) NULL);
#endif

    if (MB_CUR_MAX > 1)		/* I18N processing */
    {
        return _i18n_main(interp, argc, argv);
    }

    /*
     * Make command-line arguments available in the Tcl variables "argc"
     * and "argv".  If the first argument doesn't start with a "-" then
     * strip it off and use it as the name of a script file to process.
     */

    fileName = NULL;
    if ((argc > 1) && (argv[1][0] != '-')) {
	fileName = argv[1];
	argc--;
	argv++;
    }

    args = Tcl_Merge(argc-1, argv+1);

    Tcl_SetVar(interp, "argv", args, TCL_GLOBAL_ONLY);
    ckfree(args);
    sprintf(buffer, "%d", argc-1);
    Tcl_SetVar(interp, "argc", buffer, TCL_GLOBAL_ONLY);
    Tcl_SetVar(interp, "argv0", (fileName != NULL) ? fileName : argv[0],
	    TCL_GLOBAL_ONLY);

    /*
     * Set the "tcl_interactive" variable.
     */

    tty = isatty(0);
    Tcl_SetVar(interp, "tcl_interactive",
	    ((fileName == NULL) && tty) ? "1" : "0", TCL_GLOBAL_ONLY);

    /*
     * Invoke application-specific initialization.
     */

    if (Tcl_AppInit(interp) != TCL_OK) {
	/* i18n
	fprintf(stderr, "Tcl_AppInit failed: %s\n", interp->result);
	 */
	i18n_msg_format = dce_msg_get(tcl_s_main_appl_init_failed);
	fprintf(stderr, (char *)i18n_msg_format, interp->result);
	free(i18n_msg_format);
    }

    /*
     * If a script file was specified then just source that file
     * and quit.
     */

    if (fileName != NULL) {
	code = Tcl_EvalFile(interp, fileName);
	if (code != TCL_OK) {
	    fprintf(stderr, "%s\n", interp->result);
	    exitCode = 1;
	}
	goto done;
    }

    /*
     * We're running interactively.  Source a user-specific startup
     * file if Tcl_AppInit specified one and if the file exists.
     */

    if (tcl_RcFileName != NULL) {
	Tcl_DString buffer;
	char *fullName;
	FILE *f;

	fullName = Tcl_TildeSubst(interp, tcl_RcFileName, &buffer);
	if (fullName == NULL) {
	    fprintf(stderr, "%s\n", interp->result);
	} else {
	    f = fopen(fullName, "r");
	    if (f != NULL) {
		code = Tcl_EvalFile(interp, fullName);
		if (code != TCL_OK) {
		    fprintf(stderr, "%s\n", interp->result);
		}
		fclose(f);
	    }
	}
	Tcl_DStringFree(&buffer);
    }

    /*
     * Process commands from stdin until there's an end-of-file.
     */

    gotPartial = 0;
    Tcl_DStringInit(&command);
    while (1) {
	clearerr(stdin);
	if (tty) {
	    char *promptCmd;

	    promptCmd = Tcl_GetVar(interp,
		gotPartial ? "tcl_prompt2" : "tcl_prompt1", TCL_GLOBAL_ONLY);
	    if (promptCmd == NULL) {
		defaultPrompt:
		if (!gotPartial) {
		    fputs("% ", stdout);
		}
	    } else {
		code = Tcl_Eval(interp, promptCmd);
		if (code != TCL_OK) {
		    fprintf(stderr, "%s\n", interp->result);
		    /* i18n
		    Tcl_AddErrorInfo(interp,
			    "\n    (script that generates prompt)");
		     */
		    i18n_msg_format = dce_msg_get(tcl_s_main_script_gen_prompt);
		    Tcl_AddErrorInfo(interp, (char *)i18n_msg_format);
		    free(i18n_msg_format);

		    goto defaultPrompt;
		}
	    }
	    fflush(stdout);
	}
	if (fgets(buffer, 1000, stdin) == NULL) {
	    if (ferror(stdin)) {
		if (errno == EINTR) {
		    if (tcl_AsyncReady) {
			(void) Tcl_AsyncInvoke((Tcl_Interp *) NULL, 0);
		    }
		    clearerr(stdin);
		} else {
		    goto done;
		}
	    } else {
		if (!gotPartial) {
		    goto done;
		}
	    }
	    buffer[0] = 0;
	}
	cmd = Tcl_DStringAppend(&command, buffer, -1);
	if ((buffer[0] != 0) && !Tcl_CommandComplete(cmd)) {
	    gotPartial = 1;
	    continue;
	}

	gotPartial = 0;
	code = Tcl_RecordAndEval(interp, cmd, 0);
	Tcl_DStringFree(&command);
	if (code != TCL_OK) {
	    fprintf(stderr, "%s\n", interp->result);
	} else if (tty && (*interp->result != 0)) {
	    printf("%s\n", interp->result);
	}
#ifdef TCL_MEM_DEBUG
	if (quitFlag) {
	    Tcl_DeleteInterp(interp);
	    Tcl_DumpActiveMemory(dumpFile);
	    exit(0);
	}
#endif
    }

    /*
     * Rather than calling exit, invoke the "exit" command so that
     * users can replace "exit" with some other command to do additional
     * cleanup on exit.  The Tcl_Eval call should never return.
     */

    done:
    sprintf(buffer, "exit %d", exitCode);
    Tcl_Eval(interp, buffer);
    return 1;
}

/*
 *----------------------------------------------------------------------
 *
 * CheckmemCmd --
 *
 *	This is the command procedure for the "checkmem" command, which
 *	causes the application to exit after printing information about
 *	memory usage to the file passed to this command as its first
 *	argument.
 *
 * Results:
 *	Returns a standard Tcl completion code.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */
#ifdef TCL_MEM_DEBUG

	/* ARGSUSED */
static int
CheckmemCmd(clientData, interp, argc, argv)
    ClientData clientData;		/* Not used. */
    Tcl_Interp *interp;			/* Interpreter for evaluation. */
    int argc;				/* Number of arguments. */
    char *argv[];			/* String values of arguments. */
{
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc != 2) {
	/* i18n
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" fileName\"", (char *) NULL);
	 */
	i18n_msg_format = dce_msg_get(tcl_s_main_unixaz_wnum_args_filename);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    strcpy(dumpFile, argv[1]);
    quitFlag = 1;
    return TCL_OK;
}
#endif

/*
 *----------------------------------------------------------------------
 *
 * _i18n_main --
 *
 *	This is the main program for a Tcl-based shell that reads
 *	Tcl commands from standard input.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Can be almost arbitrary, depending on what the Tcl commands do.
 *
 *----------------------------------------------------------------------
 */

int
_i18n_main(interp, argc, argv)
    Tcl_Interp *interp;
    int argc;				/* Number of arguments. */
    char **argv;			/* Array of argument strings. */
{
    wchar_t *wargs; 
    char buffer[1000], *fileName;
    int code, gotPartial, tty, cerr;
    int exitCode = 0;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    size_t wlen, st;
    wchar_t *wtemp;
    char  *ctemp, *cmd;

    /*
     * Create wide special characters
     */
    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	exitCode = 1;
	goto done;
    }

    /*
     * Make command-line arguments available in the Tcl variables "argc"
     * and "argv".  If the first argument doesn't start with a "-" then
     * strip it off and use it as the name of a script file to process.
     */

    if (argv[1] != NULL)
    {
	_Multibyte_to_WChar(interp, wlen, argv[1], wtemp, st) 
	if (st == (size_t)-1)
	{
	    exitCode = 1;
	    goto done;
	}
    }

    fileName = NULL;
    if ((argc > 1) && (*wtemp != i18n_special_chars.w_minus)) {
	fileName = argv[1];
	argc--;
	argv++;
    }
    ckfree(wtemp);

    wargs = _Tcl_WMerge(interp, argc-1, argv+1, &cerr);
    if (cerr != TCL_OK)
    {
	exitCode = 1;
	goto done;
    }

    _WChar_To_Multibyte(interp, wlen, wargs, ctemp, st)
    if (st == (size_t)-1)
    {
	ckfree(wargs);
	exitCode = 1;
	goto done;
    }
    Tcl_SetVar(interp, "argv", ctemp, TCL_GLOBAL_ONLY);
    ckfree(wargs);
    ckfree(ctemp);
    sprintf(buffer, "%d", argc-1);
    Tcl_SetVar(interp, "argc", buffer, TCL_GLOBAL_ONLY);
    Tcl_SetVar(interp, "argv0", (fileName != NULL) ? fileName : argv[0],
	    TCL_GLOBAL_ONLY);

    /*
     * Set the "tcl_interactive" variable.
     */

    tty = isatty(0);
    Tcl_SetVar(interp, "tcl_interactive",
	    ((fileName == NULL) && tty) ? "1" : "0", TCL_GLOBAL_ONLY);

    /*
     * Invoke application-specific initialization.
     */
    if (_Tcl_WAppInit(interp) != TCL_OK) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_main_appl_init_failed);
	fprintf(stderr, (char *)i18n_msg_format, interp->result);
	free(i18n_msg_format);
    }

    /*
     * If a script file was specified then just source that file
     * and quit.
     */

    if (fileName != NULL) 
    {
	code = _Tcl_WEvalFile(interp, fileName);
	if (code != TCL_OK) 
	{
	    fprintf(stderr, "%s\n", interp->result);
	    exitCode = 1;
	}
	goto done;
    }

    /*
     * We're running interactively.  Source a user-specific startup
     * file if Tcl_AppInit specified one and if the file exists.
     */

    if (tcl_RcFileName != NULL) {
	Tcl_DString buffer;
	char *fullName;
	FILE *f;

	fullName = _Tcl_WTildeSubst(interp, tcl_RcFileName, &buffer);
	if (fullName == NULL) 
	{
	    fprintf(stderr, "%s\n", interp->result);
	} else 
	{
	    f = fopen(fullName, "r");
	    if (f != NULL) 
	    {
		code = _Tcl_WEvalFile(interp, fullName);
		if (code != TCL_OK) 
		{
		    fprintf(stderr, "%s\n", interp->result);
		}
		fclose(f);
	    }
	}
	Tcl_DStringFree(&buffer);
    }

    /*
     * Process commands from stdin until there's an end-of-file.
     */

    gotPartial = 0;
    Tcl_DStringInit(&command);
    while (1) 
    {
	clearerr(stdin);
	if (tty) 
	{
	    char *promptCmd;

	    promptCmd = Tcl_GetVar(interp,
		gotPartial ? "tcl_prompt2" : "tcl_prompt1", TCL_GLOBAL_ONLY);
	    if (promptCmd == NULL) 
	    {
		defaultPrompt:
		if (!gotPartial) 
		{
		    fputs("% ", stdout);
		}
		/* What happens if gotPartial is true && prompt2 is not set? */
	    } 
	    else 
	    {
	        _Multibyte_to_WChar(interp, wlen, promptCmd, wtemp, st) 
	        if (st == (size_t)-1)
	        {
			exitCode = 1;
			goto done;
	    	}
		code = _Tcl_WEval(interp, wtemp);
		ckfree(wtemp);
		if (code != TCL_OK) 
		{
		    fprintf(stderr, "%s\n", interp->result);
		    i18n_msg_format = dce_msg_get(tcl_s_main_script_gen_prompt);
		    Tcl_AddErrorInfo(interp, (char *)i18n_msg_format);
		    free(i18n_msg_format);

		    goto defaultPrompt;
		}
	    }
	    fflush(stdout);
	}
	if (fgets(buffer, 1000, stdin) == NULL) 
	{
	    if (ferror(stdin)) 
	    {
		if (errno == EINTR) 
		{
		    if (tcl_AsyncReady) 
		    {
			(void) Tcl_AsyncInvoke((Tcl_Interp *) NULL, 0);
		    }
		    clearerr(stdin);
		} 
		else 
		{
		    goto done;
		}
	    } 
	    else 
	    {
		if (!gotPartial) 
		{
		    goto done;
		}
	    }
	    buffer[0] = 0;
	}
	cmd = Tcl_DStringAppend(&command, buffer, -1);
	cerr = _Tcl_WCommandComplete(interp, cmd);
	if (cerr == TCL_CONV_ERROR)
	{
	    exitCode = 1;
	    goto done;
	}
	if ((buffer[0] != 0) && !cerr) 
	{
	    gotPartial = 1;
	    continue;
	}

	gotPartial = 0;
	code = _Tcl_WRecordAndEval(interp, cmd, 0);
	Tcl_DStringFree(&command);
	if (code != TCL_OK) {
	    fprintf(stderr, "%s\n", interp->result);
	} else if (tty && (*interp->result != 0)) {
	    printf("%s\n", interp->result);
	}
#ifdef TCL_MEM_DEBUG
	if (quitFlag) {
	    Tcl_DeleteInterp(interp);
	    Tcl_DumpActiveMemory(dumpFile);
	    exit(0);
	}
#endif
    }

    /*
     * Rather than calling exit, invoke the "exit" command so that
     * users can replace "exit" with some other command to do additional
     * cleanup on exit.  The Tcl_Eval call should never return.
     */

    done:
    sprintf(buffer, "exit %d", exitCode);
    Tcl_Eval(interp, buffer);
    return 1;
}
