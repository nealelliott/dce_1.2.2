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
 * $Log: main.c,v $
 * Revision 1.1.6.2  1996/02/18  19:14:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:26  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:21:47  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/17  1995/10/20  14:33 UTC  kevins
 * 	Need to test signal handling stuff for dcecp<->acctmgr pipes
 * 	[1995/10/20  14:29 UTC  kevins  /main/HPDCE02/kevins_chfts16379/1]
 * 
 * 	HP revision /main/HPDCE02/16  1995/10/19  16:56 UTC  truitt
 * 	Merge minor local work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts50/1  1995/10/19  16:54 UTC  truitt
 * 	CHFts16529: Local work.
 * 
 * 	HP revision /main/HPDCE02/15  1995/09/29  20:46 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/3  1995/09/23  17:47 UTC  tatsu_s
 * 	Failure to open dced databases is non-fatal.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/23  00:54 UTC  tatsu_s
 * 	Added dcp_general_bind_local().
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/21  15:27 UTC  tatsu_s
 * 	Added -local option.
 * 
 * 	HP revision /main/HPDCE02/14  1995/07/14  18:28 UTC  truitt
 * 	Merge optimization changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts39/1  1995/07/14  18:26 UTC  truitt
 * 	CHFts15725: Change variable definition for optimization purposes.
 * 
 * 	HP revision /main/HPDCE02/13  1995/06/05  18:47 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/05  18:45 UTC  truitt
 * 	CHFts15398: Clear the results field before reporting
 * 	an error message when unable to open a script file for
 * 	processing.
 * 
 * 	HP revision /main/HPDCE02/12  1995/05/30  18:18 UTC  jrr
 * 	Fix handling of stdout and stderr (CHFts14678).
 * 	[1995/05/24  17:01 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14678/1]
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_chfts14678/2  1995/05/30  18:16 UTC  jrr
 * 	Back out part of the command line fix.
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_chfts14678/1  1995/05/24  17:01 UTC  jrr
 * 	Fix handling of stdout and stderr (CHFts14678).
 * 
 * 	HP revision /main/HPDCE02/10  1995/05/03  19:50 UTC  jrr
 * 	Fix init of global login list mutex (CHFts15101).
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_chfts15101/1  1995/04/26  23:12 UTC  jrr
 * 	Move init of login_list mutex to Tcl_AppInit from check_initial_auth().
 * 
 * 	HP revision /main/HPDCE02/9  1995/04/06  14:13 UTC  jrr
 * 	Fix startup warning about login context (CHFts14940).
 * 	[1995/04/06  14:12 UTC  jrr  /main/HPDCE02/jrr_dcecp_icontext/1]
 * 
 * 	HP revision /main/HPDCE02/8  1995/04/06  13:55 UTC  jrr
 * 	Fix various memory leaks (CHFts14834).
 * 	[1995/04/05  14:32 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14834/1]
 * 
 * 	HP revision /main/HPDCE02/7  1995/03/24  20:05 UTC  truitt
 * 	Merge simple pipe change.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts21/1  1995/03/24  20:03 UTC  truitt
 * 	CHFts14641: When in pipe mode, output error information
 * 	to stdout instead of stderr.
 * 
 * 	HP revision /main/HPDCE02/6  1995/03/21  16:34 UTC  jrr
 * 	Improve output speed by replacing putchar() loop with printf() (CR12218).
 * 
 * 	HP revision /main/HPDCE02/jrr_CR12218/1  1995/03/21  16:32 UTC  jrr
 * 	Replace putchar() loop with printf() (CR12218).
 * 
 * 	HP revision /main/HPDCE02/5  1995/02/21  16:15 UTC  truitt
 * 	Merge convenience variable work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts14/1  1995/02/21  16:13 UTC  truitt
 * 	CHFts14390: When in -tty mode, do not display the
 * 	success/fail line if the command was not a dcecp
 * 	built-in command.
 * 	CHFts14391: Add a function call that determines whether
 * 	the command was a built-in or an external.
 * 
 * 	HP revision /main/HPDCE02/4  1995/02/10  19:33 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts11/1  1995/02/10  19:31 UTC  truitt
 * 	CHFts14285 partial: Some of the work to have dcecp, when called
 * 	with the new '-tty' flag, put out a success/fail line before the
 * 	normal output.
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/24  19:33 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts04/1  1995/01/24  19:31 UTC  truitt
 * 	CHFts14176: Remove the 'home' variable warning for
 * 	the dcecp rc file when not in interactive mode.  Along
 * 	for the ride is the ability, through the new -tty switch,
 * 	to have a background process of dcecp accept commands
 * 	through a pipe.  This involves setting the tty variable
 * 	to '1' when the -tty switch is used.
 * 
 * 	HP reviss
 * 
 * Revision 1.1.4.30  1994/10/04  21:11:50  rousseau
 * 	Fix use of '-s' (CR 10720).
 * 	[1994/10/04  21:11:33  rousseau]
 * 
 * Revision 1.1.4.29  1994/09/30  14:45:57  rousseau
 * 	Add \n to message.
 * 	[1994/09/30  14:45:39  rousseau]
 * 
 * Revision 1.1.4.28  1994/09/28  13:02:03  melman
 * 	fixed dcecp_verbose_errors
 * 	[1994/09/28  13:01:49  melman]
 * 
 * Revision 1.1.4.27  1994/09/20  19:48:46  rousseau
 * 	Merged with changes from 1.1.4.26
 * 	[1994/09/20  19:48:39  rousseau]
 * 
 * 	Print message if initial auth check fails.
 * 	[1994/09/20  19:48:15  rousseau]
 * 
 * Revision 1.1.4.26  1994/09/20  18:00:21  kevins
 * 	OT 12262 commands that have passwords in them now fail when called in -c mode
 * 	[1994/09/20  17:58:59  kevins]
 * 
 * Revision 1.1.4.25  1994/09/15  22:46:39  rousseau
 * 	Fix exit codes and piped stdin handling (CR 12174, 12192).
 * 	[1994/09/15  22:46:08  rousseau]
 * 
 * Revision 1.1.4.24  1994/09/13  22:38:49  rousseau
 * 	Change printf to putchar loop for long outputs (CR 12094).
 * 	[1994/09/13  22:38:15  rousseau]
 * 
 * Revision 1.1.4.23  1994/09/01  20:22:22  coren
 * 	Merged with changes from 1.1.4.22
 * 	[1994/09/01  20:22:16  coren]
 * 
 * 	Put TRY/CATCH_ALL wrapper around input loop so as to retain control after
 * 	SIGINT (ctrl-c). (CR 10278)
 * 	[1994/09/01  18:40:31  coren]
 * 
 * Revision 1.1.4.22  1994/09/01  19:04:42  truitt
 * 	Put schema commands in separate table.
 * 	[1994/09/01  18:05:07  truitt]
 * 
 * Revision 1.1.4.21  1994/09/01  13:35:20  rousseau
 * 	Fix bad free (CR 11986)
 * 	[1994/09/01  13:34:25  rousseau]
 * 
 * Revision 1.1.4.20  1994/07/18  20:16:17  mori_m
 * 	CR: 11271  Initialize Tcl 'in core' message table when dcecp starts.
 * 	[1994/07/18  20:13:08  mori_m]
 * 
 * Revision 1.1.4.19  1994/07/12  17:43:45  melman
 * 	write errorInfo to stderr not stdout
 * 	[1994/07/12  17:41:35  melman]
 * 
 * Revision 1.1.4.18  1994/07/12  15:47:30  melman
 * 	Added dcecp_verbose_errors
 * 	[1994/07/12  15:41:33  melman]
 * 
 * Revision 1.1.4.17  1994/07/08  19:35:45  rousseau
 * 	Add return TCL_OK to Tcl_AppInit().
 * 	[1994/07/08  19:35:20  rousseau]
 * 
 * Revision 1.1.4.16  1994/06/28  20:01:51  rousseau
 * 	Added initial support for -s (CR 10720).
 * 	[1994/06/28  19:58:50  rousseau]
 * 
 * Revision 1.1.4.15  1994/06/09  16:02:14  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:45  devsrc]
 * 
 * Revision 1.1.4.14  1994/05/18  21:15:39  melman
 * 	fixed EOF at begining of line in partial command core dump
 * 	[1994/05/16  21:09:48  melman]
 * 
 * Revision 1.1.4.13  1994/05/10  10:00:12  kevins
 * 	OT 9691 Add registry show/modify commands
 * 	[1994/05/09  19:17:15  kevins]
 * 
 * Revision 1.1.4.12  1994/05/03  16:50:16  kevins
 * 	OT 10230  remove external declaration of Tcl_Interp *interp
 * 	[1994/05/03  15:16:30  kevins]
 * 
 * Revision 1.1.4.11  1994/04/20  18:18:53  kevins
 * 	CR 10406 encode/decode cds attributes by syntax
 * 	[1994/04/20  15:22:01  kevins]
 * 
 * Revision 1.1.4.10  1994/04/07  19:14:48  rousseau
 * 	Turn _r back on (CR 9854)
 * 	[1994/04/07  19:14:30  rousseau]
 * 
 * 	Added creation and initialization of "dir_interp" for using scripts
 * 	for recursive "directory" operations.
 * 	[1994/04/01  20:50:36  coren]
 * 
 * 	Merge to next version
 * 	[1994/03/31  23:49:29  coren]
 * 
 * 	Merging yet again.
 * 	[1994/03/22  20:04:58  coren]
 * 
 * Revision 1.1.4.9  1994/04/05  20:48:29  coren
 * 	Added creation and initialization of "dir_interp" for using scripts
 * 	for recursive "directory" operations.
 * 	Removed use of canned pathnames.
 * 	[1994/04/05  19:10:01  coren]
 * 
 * Revision 1.1.4.8  1994/03/31  22:07:11  salamone
 * 	CR10259 - Remove global C variables for dcecp convenience variable support.
 * 	[1994/03/31  21:15:57  salamone]
 * 
 * Revision 1.1.4.7  1994/03/17  20:51:26  rousseau
 * 	Added newline onto end of user's input string (CR 10126).
 * 	[1994/03/17  20:51:11  rousseau]
 * 
 * Revision 1.1.4.6  1994/02/02  19:45:53  rousseau
 * 	Fix secondary prompt for CR 9862.
 * 	[1994/02/02  19:45:29  rousseau]
 * 
 * Revision 1.1.4.5  1994/01/05  16:30:29  kevins
 * 	modified installation path of cds attribute script file
 * 	[1994/01/05  16:29:15  kevins]
 * 
 * 	modified location for tcl script read during initialization
 * 	[1994/01/05  16:02:44  kevins]
 * 
 * Revision 1.1.4.4  1993/12/30  19:58:49  rousseau
 * 	Changed include of dcemsgmsg.h to dcesvcmsg.h
 * 	[1993/12/30  19:58:05  rousseau]
 * 
 * Revision 1.1.4.3  1993/12/29  15:58:12  kevins
 * 	added function to intialize attribute information from cds_attributes file
 * 	[1993/12/29  15:54:49  kevins]
 * 
 * Revision 1.1.4.2  1993/12/21  18:42:21  rousseau
 * 	Changed include of dcecp_login.h to util_login.h
 * 	[1993/12/21  18:42:14  rousseau]
 * 
 * Revision 1.1.4.1  1993/12/21  16:22:02  rousseau
 * 	Initial style guide changes.
 * 	Changed Tcl_appInit to Tcl_AppInit.
 * 	[1993/12/21  16:21:49  rousseau]
 * 
 * Revision 1.1.2.14  1993/12/14  18:19:12  salamone
 * 	Support for dcecp convenience variables
 * 	[1993/12/14  18:18:46  salamone]
 * 
 * Revision 1.1.2.12  1993/12/10  00:14:07  melman
 * 	Added include of editline.h
 * 	[1993/12/10  00:13:58  melman]
 * 
 * Revision 1.1.2.11  1993/12/08  19:25:00  rousseau
 * 	Pulled application init specific code out of dcp_initInterp in
 * 	commands.c and moved it in tcl_appInit() here.
 * 	[1993/12/08  19:24:51  rousseau]
 * 
 * Revision 1.1.2.10  1993/11/23  13:22:33  kevins
 * 	Merged with changes from 1.1.2.9
 * 	[1993/11/23  13:22:25  kevins]
 * 
 * 	Added rgy initialization
 * 	[1993/11/23  13:21:35  kevins]
 * 
 * Revision 1.1.2.9  1993/11/19  22:30:37  rousseau
 * 	Moved local modifications from main() to dcp_InitInterp().
 * 	[1993/11/19  22:20:43  rousseau]
 * 
 * Revision 1.1.2.8  1993/11/15  00:26:27  rousseau
 * 	Fixed usage of "-c" for command line execution.
 * 	Removed unneeded "\n" from commands.
 * 	[1993/11/15  00:26:22  rousseau]
 * 
 * Revision 1.1.2.7  1993/11/11  01:54:32  rousseau
 * 	Moved check_initial_auth() to dcecp_login.c and changed call
 * 	paramters.
 * 	[1993/11/11  01:54:21  rousseau]
 * 
 * Revision 1.1.2.6  1993/11/10  21:12:31  rousseau
 * 	Upgraded main() to Tcl7.1 style.
 * 	Disabled $r.
 * 	[1993/11/10  21:12:25  rousseau]
 * 
 * Revision 1.1.2.5  1993/11/05  15:47:07  rousseau
 * 	Added check_initial_auth() and stubbed check_visible_pw() calls.
 * 	[1993/11/05  15:46:54  rousseau]
 * 
 * Revision 1.1.2.4  1993/10/22  17:03:45  rousseau
 * 	Upgrade to Tcl7.0
 * 	[1993/10/22  17:03:35  rousseau]
 * 
 * Revision 1.1.2.3  1993/10/20  20:55:40  rousseau
 * 	Changed help messages from dcp_s_ to dcp_t_ .
 * 	[1993/10/20  20:55:34  rousseau]
 * 
 * Revision 1.1.2.2  1993/10/20  14:52:17  rousseau
 * 	Changed message catalog include and other i18n changes.
 * 	[1993/10/20  14:52:08  rousseau]
 * 
 * Revision 1.1.2.1  1993/10/18  19:26:11  rousseau
 * 	Conversion from dsh to dcecp. Includes initial i18n changes.
 * 	[1993/10/18  19:21:27  rousseau]
 * 
 * $EndLog$
 */

/*
 * PRE-HISTORY
 * Revision 1.1.2.12  1993/10/13  19:01:45  kevins
 * 	Added an else condition to Tcl_Eval when loading attribute evaluation
 * 	script.
 * 	[1993/10/13  18:52:08  kevins]
 *
 * Revision 1.1.2.11  1993/09/15  15:01:58  kevins
 * 	add -c argument capability
 * 	[1993/09/15  15:01:51  kevins]
 * 
 * Revision 1.1.2.10  1993/09/04  00:20:11  melman
 * 	Added first arg as script name
 * 	[1993/09/04  00:20:00  melman]
 * 
 * Revision 1.1.2.9  1993/08/23  19:01:07  kevins
 * 	Added fix for command assembly
 * 	[1993/08/23  19:00:28  kevins]
 * 
 * Revision 1.1.2.8  1993/08/05  19:45:55  kevins
 * 	Added a new interpretor for attribute functions
 * 	[1993/08/05  19:45:22  kevins]
 * 
 * Revision 1.1.2.7  1993/08/04  19:32:45  melman
 * 	Added loading of init.tcl
 * 	[1993/08/04  19:28:40  melman]
 * 
 * Revision 1.1.2.6  1993/07/28  22:39:15  melman
 * 	Added $r and fixed blown pointer
 * 	[1993/07/28  22:38:55  melman]
 * 
 * Revision 1.1.2.5  1993/07/27  15:25:00  melman
 * 	Changed ClientData for dsh_initInterp to NULL
 * 	[1993/07/27  15:24:37  melman]
 * 
 * Revision 1.1.2.4  1993/07/26  19:09:59  melman
 * 	Moved dsh_svc.c stuff to main.c
 * 	[1993/07/26  19:09:36  melman]
 * 
 * Revision 1.1.2.3  1993/07/26  17:57:16  melman
 * 	Stopped exporting dsh.h
 * 	[1993/07/26  17:56:40  melman]
 * 
 * Revision 1.1.2.2  1993/07/23  20:02:50  kevins
 * 	Initial submission of dsh
 * 	[1993/07/23  19:35:38  kevins]
 * 
 */

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <dce/dce.h>
#include <dce/editline.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dce/dcedcpmsg.h>
#include <dce/tcl.h>
#include <dcecp.h>
#include <util_login.h>

extern Tcl_Interp *attr_interp;
extern Tcl_Interp *dir_interp;
extern boolean32 check_visible_pw;

/* shouldn't these be generated by sams? */
dce_svc_subcomp_t dcp_svc_table[5];
dce_svc_handle_t dcp_svc_handle;

/* Prototypes */
#define DCP_MAX_CMD_SIZE                2048

/* Prototypes */
int main(int, char **);
int Tcl_AppInit(Tcl_Interp *);
void init_dcp_svc(void);
void init_cds_attribute_table(Tcl_Interp *);
void init_rgy_attribute_table(Tcl_Interp *);
void init_schema_attribute_table(Tcl_Interp *);
void setup_signal_handling(void);


int
main(int argc, char *argv[])
{
    Tcl_Interp                    *interp;
    Tcl_DString                   command;
    char                          buffer[DCP_MAX_CMD_SIZE];
    char                          *cmd, *args, *line, *fileName;
    int                           code, gotPartial, tty;
    volatile int                  got_signal;
    int                           exitCode = 0;
    boolean32                     echo_commands = FALSE;
    char                          *p;
    char                          *tcl_var;
    char                          *errcode = NULL;
    error_status_t		  msg_st;
    boolean32                     pipe_flag = FALSE;
    int                           argv_index = 0;
    /* 
     * User-specific startup script. Only loaded when we are running
     * interactively. 
     */
    char                          *tcl_RcFileName = "~/.dcecprc";	

    /* Initialize Tcl incore messages */
    dce_msg_define_msg_table (dce_tcl_g_table,
                        sizeof dce_tcl_g_table / sizeof dce_tcl_g_table[0],
			&msg_st);

    if (msg_st != rpc_s_ok)
        fprintf(stderr, "Unable to open Tcl message table\n");
    
    /* Create the interpreters */
    interp = Tcl_CreateInterp();
    
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

    /* Set the "tcl_interactive" variable. */
    while ((argv_index < argc)  && (pipe_flag == FALSE)) {
        if (strcmp (argv[argv_index], "-tty") == 0) {
            pipe_flag = TRUE;
            break;
        }
        argv_index++;
    }
    if (pipe_flag == FALSE)
        tty = isatty(0);
    else
        tty = 1;
    Tcl_SetVar(interp, "tcl_interactive",
               ((fileName == NULL) && tty) ? "1" : "0", TCL_GLOBAL_ONLY);

    /* Application initialization */
    if (Tcl_AppInit(interp) != TCL_OK) {
        fprintf(stderr, "Application initialization failed: %s\n", interp->result);
    }

    /* 
     * If a script file was specified then just source that file and quit.
     * We really only want to print out errors. If we print out normal output,
     * we just get a '0' (successful completion of the script) on stdout. Not
     * quite the behavior we want.
     */
    if (fileName != NULL) {
        Tcl_ResetResult(interp);
        code = Tcl_EvalFile(interp, fileName);
        if (code != TCL_OK) {
            fprintf(stderr, "%s\n", interp->result);
            exitCode = 1;
        }
        goto done;
    }

    /*
     * Source a user-specific startup file if Tcl_AppInit specified one 
     * and if the file exists.
     */
    if (tcl_RcFileName != NULL) {
        Tcl_DString DSbuffer;
        char *fullName;
        
        fullName = Tcl_TildeSubst(interp, tcl_RcFileName, &DSbuffer);
        if (fullName == NULL) {
            if (tty) {
                fprintf(stderr, "Warning: %s\n", interp->result);
            }
        } else {
            if (access(fullName, R_OK) == 0) {
                code = Tcl_EvalFile(interp, fullName);
                if (code != TCL_OK) {
                    if (tty) {
                        fprintf(stderr, "Warning: %s\n", interp->result);
                    }
                }
            }
        }
        Tcl_DStringFree(&DSbuffer);
    }
    

    /* Squash any result that previous startup commands may have left around */
    Tcl_ResetResult(interp);
    setup_signal_handling();
    gotPartial = 0;
    Tcl_DStringInit(&command);

    if (!tty) {
        tcl_var = Tcl_GetVar(interp, "dcecp_echo_commands", TCL_GLOBAL_ONLY);
        if (tcl_var && !strcmp(tcl_var, "1")) {
            echo_commands = TRUE;
        }
        if (tcl_var) {
            free(tcl_var);
        }
    }

    got_signal = 0;

    /* Process commands from stdin until there's an end-of-file */
  top:
    TRY {
	for (;;) {
	    clearerr(stdin);
	    if (tty) {
		char *promptCmd;
		
		promptCmd = Tcl_GetVar(interp,
				       gotPartial ? "tcl_prompt2" : "tcl_prompt1", 
				       TCL_GLOBAL_ONLY);
		if (promptCmd == NULL) {
		  defaultPrompt:
		    if (!gotPartial) {
			Tcl_SetResult(interp, "dcecp> ", TCL_STATIC);
		    }
		    else {
			Tcl_SetResult(interp, "> ", TCL_STATIC);
		    }
		} else {
		    code = Tcl_Eval(interp, promptCmd);
		    if (code != TCL_OK) {
			goto defaultPrompt;
		    }
		}
		fflush(stdout);
	    }


            if (tty) {
                line = readline(interp->result);
            }
            else {
                if (fgets(buffer, DCP_MAX_CMD_SIZE, stdin) == NULL) {
                    line = NULL;
                }
                else {
                    line = dce_strdup(buffer);
                    
                    if (echo_commands) {
                        fwrite(line, strlen(line), 1, stdout);
                    } 
                } 
            }

	    if (line == NULL) {
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
		    /* Handle EOF at begining of line */
		    if (gotPartial) {
			/* 
			 * if in middle of multi-line command, clean up and 
			 * goto top level prompt. 
			 */
			gotPartial=0;
			Tcl_DStringFree(&command);
			continue;
		    } else {
			/* At top level command, exit dcecp */
			goto done;
		    }
		}
                buffer[0] = 0;
            }
            add_history(line);
	    strcpy(buffer, line);
	    free(line);
	    
	    /* 
	     * This is needed because readline strips off the trailing
	     * newline. This makes writing interactive multi-line commands
	     * fail.
	     */
	    strcat(buffer, "\n");

	    cmd = Tcl_DStringAppend(&command, buffer, -1);
            if ((buffer[0] != 0) && !Tcl_CommandComplete(cmd)) {
		gotPartial = 1;
                continue;
	    }

            /*
             * Before we call the next command, reset the
             * tracking variables.
             */
            was_e_var_set = FALSE;
            was_o_var_set = FALSE;

            gotPartial = 0;
            code = Tcl_RecordAndEval(interp, cmd, 0);
            Tcl_DStringFree(&command);

            if (code != TCL_OK) {
                /*
                 * There was an error with the command executed.  If
                 * dcecp internally set the error code, great.  If not,
                 * set to an undefined error code.  This way an old
                 * code won't be identified with an external error.
                 */
                if (was_e_var_set == FALSE) {
                    DCP_GENERAL_SET_E_VAR(-1);
                }

                /*
                 * If we are working through a pipe, output a code,
                 * before the regular error message, that indicates
                 * the command failed, but only if an internal command
                 * was used.
                 */
                if ((pipe_flag == TRUE) && (was_o_var_set == TRUE)) {
                    errcode = DCP_GENERAL_GET_E_VAR();
                    fprintf(stdout, "ERROR %s (%s)\n", errcode, interp->result);
                }
                else {
                    char *info_cmd="puts stderr [format {Error: %s} [set errorInfo]]";
                    tcl_var = Tcl_GetVar(interp, 
                                         "dcecp_verbose_errors", 
                                         TCL_GLOBAL_ONLY);
                    if (tcl_var && !strcmp(tcl_var, "1")) {
                        Tcl_Eval(interp, info_cmd);
                    } else {
                        fprintf(stderr, "Error: %s\n", interp->result);
                    }
                }
                exitCode = 1;
            }
            else {
                /*
                 * If we are working through a pipe, output a success tag
                 * before the first line of command results, if any, but
                 * only if the command was an internal dcecp command.  If
                 * the command is an external command, but there is stuff
                 * in the interpreter structure, we can also display a
                 * success tag.
                 */
                if ((pipe_flag == TRUE) && (was_o_var_set == TRUE)) {
                    fprintf(stdout, "OK\n");
                }
                if (*interp->result != 0) {
                    printf("%s\n", interp->result);
                    exitCode = 0;
                }
            }

	    /* No matter what, save the result in $_r */
	    DCP_GENERAL_SET_R_VAR(interp->result);
            /* reset sig handler info. */
	    got_signal = 0;
	} /* End of while(1) loop */
    }
    CATCH_ALL {
        if ( (pipe_flag == TRUE) && (got_signal > 10) ) {
            exit(1);
        }
	got_signal++;
    }

    ENDTRY
    if (got_signal) {
	gotPartial=0;
	Tcl_DStringFree(&command);
	goto top;
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
 * setup_signal_handling
 *
 * Arrange to catch SIGINT thread-safely, so we can return to dcecp
 * main prompt rather than killing dcecp session.
 */

void
setup_signal_handling(void)
{
    sigset_t set;
    pthread_t me;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    me = pthread_self();
    pthread_signal_to_cancel_np(&set, &me);
}


/* 
 * Tcl_AppInit
 * 
 * Do application specific startup stuff here
 *
 */
int
Tcl_AppInit(Tcl_Interp *interp)
{
    int                           argc, result, argv_index, new_index;
    char                          **argv;
    char                          *p;
    boolean32                     standalone_flag = FALSE;
    boolean32                     command_flag = FALSE;
    boolean32                     pipe_flag = FALSE;
    boolean32                     local_flag = FALSE;
    char                          *errcode = NULL;
    char                          *cds_attr_eval_file = "attr_eval.tcl";
    char                          *cds_dir_ops_file = "dir_ops.dcecp";
    char                          temp_attr_eval_file[256];
    char                          command[1024], *one_command = NULL;

    /* Set the locale and initialize the msg table */
    (void)setlocale(LC_ALL, "");
    init_dcp_svc();
    
    Tcl_SplitList(interp, 
                  Tcl_GetVar(interp, "argv", 0), 
                  &argc,
                  &argv);

    argv_index = 0;
    new_index = 0;

    while (argv[argv_index] != NULL) {
        if (strcmp (argv[argv_index], "-c") == 0) {
            /* We've got a '-c', we're done */
            command_flag = TRUE;
            argv_index++;  /* Bump past the '-c' */
            /* Copy down the rest */
            for (; argv_index <= argc; argv_index++) {
                if (strcmp(argv[argv_index], "-tty") == 0) {
                    pipe_flag = TRUE;
                }
                else {
                    argv[new_index] = argv[argv_index];
                    new_index++;
                }
            }
            argc--;
            break;
        }
        else if (strcmp (argv[argv_index], "-s") == 0) {
            /* We've got a '-s', set the standalone flag */
            standalone_flag = TRUE;
            argc--;
        }
        else if (strcmp(argv[argv_index], "-tty") == 0) {
            /*
             * We've got a '-tty', set the pipe flag.
             */
            pipe_flag = TRUE;
        }
        else if (strcmp(argv[argv_index], "-local") == 0) {
            /*
             * We've got a '-local', set the local flag.
             */
            local_flag = TRUE;
            argc--;
        }
        else {
            /* Copy down the argument */
            argv[new_index] = argv[argv_index];
            new_index++;
        }

        argv_index++;
    }
    if (pipe_flag == TRUE) {
        argc--;
    }

    /* Initialize dcecp commands */
    dcp_initInterp(interp);

#ifndef	HPDCE_DCED_LOCAL
    {
        char  old_varname[12];
        char  *cp;

        /*
         * Set traces on "_local" read-only convenience varibles and set it.
         */
        strcpy(old_varname, DCP_C_GENERAL_VARNAME_PREFIX);
        strcat(old_varname, "_local");
        Tcl_SetVar(interp, old_varname, "", TCL_GLOBAL_ONLY);
        if (dcp_general_set_var(interp, "_local", (char *)NULL, "false", 0) != NULL)
            return TCL_ERROR;

        if (local_flag) {
            if (dcp_general_bind_local(interp) != TCL_OK) {
                fprintf(stderr, "Warning: %s, Local operation disabled.\n", interp->result);
            }
            else {
                if (dcp_general_set_var(interp, "_local", (char *)NULL, "true", 0) != NULL)
                    return TCL_ERROR;
            }
        }
    }
#endif	/* HPDCE_DCED_LOCAL */

    /* Create the CDS interpreter */
    attr_interp = Tcl_CreateInterp();
    
    init_cds_attribute_table(interp);
    init_rgy_attribute_table(interp);
    init_schema_attribute_table(interp);
    /* initialize the attr_interp interpreter */
    sprintf(temp_attr_eval_file, "%s/%s/%s", dcelocal_path, DCECP_SCRIPT_DIR,
	    cds_attr_eval_file);
    sprintf(command,"if [file exists %s] {source %s} else {error \"Attribute evaluation script attr_eval.tcl does not exist in appropriate directory. \"}",temp_attr_eval_file,temp_attr_eval_file);
    result = Tcl_Eval(attr_interp, command);
    if ((result != TCL_OK) && (pipe_flag == FALSE)) {
        printf("%s\n", attr_interp->result);
    }

    /* set up scripts for directory -tree operations */
    dir_interp = Tcl_CreateInterp();
    dcp_initInterp(dir_interp);
    sprintf(command,"if [file exists $dcecp_library/%s] {source $dcecp_library/%s} else {error \"Directory operations script dir_ops.dcecp does not exist in appropriate directory. \"}",
	    cds_dir_ops_file,cds_dir_ops_file);
    result = Tcl_Eval(dir_interp, command);
    if ((result != TCL_OK) && (pipe_flag == FALSE)) {
        printf("%s\n", dir_interp->result);
    }

    /* Initialize the mutex for the global_context_list. */
    if (pthread_mutex_init(&login_list, pthread_mutexattr_default) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_mutex_init_fail);
        return TCL_ERROR;
    }
    
    /* If -s was specified, skip contacting the security server */
    if (!standalone_flag) {
        /* check to see if an initial login context exists */    
        if (check_initial_auth(interp) == TCL_ERROR) {
            if (pipe_flag == TRUE) {
                printf("STARTUP Unable to acquire initial login context: %s\n",
                       interp->result);
            } 
            else {
                printf("Warning: Unable to acquire initial login context: %s\n",
                       interp->result);
            }
        }
    }

    /* If -c exists process accordingly if a #! process as a shell 
     * script, otherwise enter the command line interpreter. */
    if (command_flag) {
        check_visible_pw = TRUE;
        was_e_var_set = FALSE;
        was_o_var_set = FALSE;

        /* A command line dcecp command was issued. */
        one_command = Tcl_Concat(argc, argv);
        result = Tcl_Eval(interp, one_command);
        free(one_command);

        if (result != TCL_OK) {
            /*
             * There was an error during the command execution.  If
             * it is an external error, set the error code to undefined.
             */
            if (was_e_var_set == FALSE) {
                DCP_GENERAL_SET_E_VAR(-1);
            }

            if ((pipe_flag == TRUE) && (was_o_var_set == TRUE)) {
                errcode = DCP_GENERAL_GET_E_VAR();
                printf("ERROR %s (%s)\n", errcode, interp->result);
            }
            if (result == TCL_ERROR) {
                fprintf(stdout, "Error");
            } else {
                fprintf(stdout, "Error %d", result);
            }
            if (interp->result != NULL) {
                printf(": %s\n", interp->result);
            } else {
                printf("\n");
            }
            free((char *)argv);
            exit(1);
        }
        else {
            if ((pipe_flag == TRUE) && (was_o_var_set == TRUE)) {
                printf("OK\n");
            }
            if (interp->result != NULL) {
                printf("%s\n", interp->result);
            }
        }
        free((char *)argv);
        exit(0);
    }

    free((char *)argv);
    return TCL_OK;
}


void
init_dcp_svc(void)
{
    error_status_t	        st;
    
    /* Load message table; ignore errors. */
    dce_msg_define_msg_table(dcp_msg_table,
                             sizeof dcp_msg_table / sizeof dcp_msg_table[0],
                             &st);
    if (st != msg_s_ok) {
        (void)fprintf(stderr,
                      "Cannot register dcp msg table, error 0x%lx\n",
                      st);
        exit(1);
        /* NOTREACHED */
    }    
    
    dcp_svc_handle = dce_svc_register(dcp_svc_table, (idl_char *)"dcp", &st);
    if (dcp_svc_handle == NULL) {
        (void)fprintf(stderr, "Cannot register svc table, error 0x%lx\n", st);
        exit(1);
        /* NOTREACHED */
    }
}
