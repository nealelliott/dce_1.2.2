/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comtest.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:22  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:58  root]
 * 
 * Revision 1.1.2.2  1993/01/11  22:12:30  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:04:33  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
**
**  NAME
**
**    COMTEST.H
**
**  FACILITY:
**
**     Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**     definitions common to both the client and server
**     sides of the RPC Communications Services Test 
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**   7-may-91   woodbury    remove extraneous print statements
**  12-apr-91   janicki     dce.h =>dce_error.h
**  21-mar-91   janicki     Update dce_error_inq_text()
**  10-feb-91   burton      move parsing tokens here
**  07-jan-91   burton      add communication of pid to client
**  01-nov-90   woodbury    add check for use_stderr flag
**  17-oct-90   woodbury    original 
**
**  %a%private_end
**
*/

/*
 * Include file
 */

#include <dce/dce_error.h>

/*
 * flag set by the -f command line option to redirect
 * printed output to stderr
 */
extern int use_stderr;

/*
 * macros used for setting up various test
 * conditions and reporting the results
 */

/*
 *  PRINT_STATUS - if status is rpc_s_ok, print the success completion
 *  message, otherwise print the failure completion message.
 */

#define PRINT_STATUS(ret, testno, routine, test, test_status){   \
int st;                                         \
dce_error_string_t error_text;                                \
int             tn;                                         \
    tn = testno;                                            \
    dce_error_inq_text (test_status, error_text, &st);     \
    fprintf (stdout,"\n");                                  \
    fprintf (stdout,"Test Number:      %d\n", tn);      \
    fprintf (stdout,"Routine Tested:   %s\n", routine);     \
    if (use_stderr) {                                       \
    fprintf (stderr,"\n");                                  \
    fprintf (stderr,"Test Number:      %d\n", tn);      \
    fprintf (stderr,"Routine Tested:   %s\n", routine);     \
    }\
    switch (ret) { \
    case COMTEST_OK:\
        fprintf (stdout,    \
            "Test Status:      %%tst-success: Test SUCCEEDED\n");   \
        if (use_stderr) fprintf (stderr,    \
            "Test Status:      %%tst-success: Test SUCCEEDED\n");   \
        if (error_text != NULL && isascii((int) *error_text))       \
        {   fprintf (stdout,"Status Returned:  %s\n\n", error_text);  \
            if (use_stderr) { \
                fprintf (stderr, "Status Returned:  %s\n\n", error_text); \
        }}\
        break; \
    case COMTEST_FAILED: \
        fprintf (stdout,"Test Status:      %%tst-fail: Test FAILED\n"); \
        if (use_stderr) fprintf (stderr, \
            "Test Status:      %%tst-fail: Test FAILED\n"); \
        if (error_text != NULL && isascii((int) *error_text))       \
        {   fprintf (stdout,"Status Returned:  %s\n\n", error_text);  \
            if (use_stderr) { \
                fprintf (stderr, "Status Returned:  %s\n\n", error_text); \
        }}\
        break;\
    case RPC_CALL_FAILED: \
        fprintf (stdout,"Test Status:      %%tst-fail: RPC Call FAILED\n"); \
        if (use_stderr) fprintf (stderr, \
            "Test Status:      %%tst-fail: RPC library routine FAILED\n"); \
        if (error_text != NULL && isascii((int) *error_text))       \
        {   fprintf (stdout,"Status Returned:  %s\n\n", error_text);  \
            if (use_stderr) { \
                fprintf (stderr, "Status Returned:  %s\n\n", error_text); \
        }}\
        break;\
    case INSF_ARGS:\
        fprintf (stdout, \
            "Test Status:      %%tst-fail: Insufficient arguments\n"); \
        if (use_stderr) fprintf (stderr, \
            "Test Status:      %%tst-fail: Insufficient arguments\n"); \
        break; \
    default: \
        break;\
    }\
     fprintf (stdout,"\n");                                         \
     if (use_stderr) fprintf (stderr,"\n");                                         \
}

/*
 * comtest return codes
 */

#define     COMTEST_OK          (0)     /* com test succeeded */
#define     RPC_CALL_FAILED     (-1)    /* status returned != rpc_s_ok */
#define     INSF_ARGS           (-2)    /* insufficient arguments */
#define     COMTEST_FAILED      (-3)    /* com test failed, rpc call ok */
#define     INVALID_ARG         (-4)    /* invalid arg on command line */

#define     CHECK_ARGS(num_args)    {\
if (argc < (num_args)) {\
    *status = rpc_s_unknown_status_code;\
    return( INSF_ARGS );\
}}
#define     PRINT_ARGS {\
int i;\
for (i=0; i<argc; i++) fprintf (stdout, "%s ", argv[i]);\
fprintf (stdout, "\n");\
}
#define     CHECK_STATUS {\
if ((*status) != rpc_s_ok){\
    return(RPC_CALL_FAILED);\
}}
#define     FREE_STRING( str ) {\
unsigned32  temp_status;        \
    rpc_string_free ((str), &temp_status);\
    if (temp_status != rpc_s_ok){\
         fprintf (stdout, "rpc_string_free failed\n");}\
}

/*
 * For comtest_client_xfr, the default wait time before
 * trying the read in the COMTST_protseq_file, (i.e., the
 * time to give the parent process to return and the server
 * process to have the output files initialized and be
 * waiting for an RPC.
 */
#define COMTST_default_wait 2

/*
 * COMTST_protseqs_tmpfile is the name
 * of a temporary file which is created by
 * a server process to store the current protseq and
 * string bindings after calling rpc_network_inq_protseqs.
 * Client processes can read this file in order
 * to get binding string and endpoint information to
 * send RPCs to the server.
 */
 static char *COMTST_protseqs_tmpfile = "COMTST_protseqs_tmpfile";

/*
 * COMTST_pid_tmpfile used to store the process
 * id of the server process so the client process
 * may send a SIGINT upon completion of a test
 */
 static char *COMTST_pid_tmpfile = "COMTST_pid_tmpfile";

/*
 * COMTST_obj_tmpfile where the server stores object
 * UUID strings.  The client reads each of these and
 * sets this as the object in the binding in order to
 * send RPCs which will be directed to the specified
 * object
 */
 static char *COMTST_obj_tmpfile = "COMTST_obj_tmpfile";

/*
 * COMTST_client_lockfile, provides a lock to insure
 * that only a single client is trying to run and
 * send RPCs at a time
 */
 static char *COMTST_client_lockfile = "COMTST_client_lockfile";

/*
 * COMTST_client_more_tsts and COMTEST_client_nolmore_tsts
 * are sent by the client to signal the server test currently
 * in operation whether there are more RPC calls to follow,
 * (i.e., other binding strings to be tested). If not the
 * server is free to break out of the 'rpc_server_listen'
 * that it is currently in.
 */
#define COMTST_client_more_tsts 0
#define COMTST_client_nomore_tsts 1
#define COMTEST_TESTNO  "COMTEST_testno"
#define COMTEST_SYNC_FILE "COMTEST_sync_file"

/*
 * Token definitions for parsing commands
 */

#define LPARN                               '('
#define RPARN                               ')'
#define SP                                  ' '
#define HT                                  '\t'
#define     REGISTER_DEFAULT_EPV            "default"
#define     REGISTER_GENERATED_EPV          "generated"
#define     SERVER_RPC_WAIT                 "server_wait"
#define     BOGUS_OBJ_UUID                  "bogus_obj"
#define     NIL_UUID                        "nil_uuid"
