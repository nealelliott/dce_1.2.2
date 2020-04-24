/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdscp_cache.c,v $
 * Revision 1.1.10.3  1996/02/18  19:12:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:32  marty]
 *
 * Revision 1.1.10.2  1995/12/08  21:19:48  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/8  1995/08/22  20:45 UTC  truitt
 * 	Merge cache work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts45/1  1995/08/22  20:44 UTC  truitt
 * 	CHFts16018: Fix error checking in cdscache show command.
 * 
 * 	HP revision /main/HPDCE02/7  1995/07/26  19:29 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts43/1  1995/07/26  19:27 UTC  truitt
 * 	CHFts14274: Change 'destroy' to 'discard'.
 * 
 * 	HP revision /main/HPDCE02/6  1995/07/21  13:10 UTC  truitt
 * 	Merge cdscache additions.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts40/1  1995/07/21  13:09 UTC  truitt
 * 	CHFts14274: Add new cdscache command.
 * 
 * 	HP revision /main/HPDCE02/5  1995/03/23  18:41 UTC  truitt
 * 	Merge cache work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts21/1  1995/03/23  18:40 UTC  truitt
 * 	CHFts13968: fix cdscache create command instability.
 * 
 * 	HP revision /main/HPDCE02/4  1995/02/14  15:49 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  15:48 UTC  truitt
 * 	CHFts14285: Combine all cds check args macros into one,
 * 	since all six are the same anyway.  Also, make sure correct
 * 	macros are called when processing an error condition.
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/31  19:44 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts06/1  1995/01/31  19:42 UTC  truitt
 * 	The cache show command, when given with a bogus
 * 	clearinghouse name (but in the format of a
 * 	clearinghouse, i.e. /.:/blech_ch) returns with
 * 	an empty error message.
 * 	Also, The cdscache create help command core dumps.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts06/1  1995/01/31  19:37 UTC  truitt
 * 	CHFts14242: changed the error messaging for the cdscache
 * 	create and cdscache show commands.
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/05  17:50 UTC  truitt
 * 	OT12710: Remove useless macro code.
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/03  19:52 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/11/03  19:50 UTC  truitt
 * 	Fix order of cdscache show switches.
 * 	[1995/12/08  17:54:28  root]
 * 
 * Revision 1.1.4.17  1994/09/29  21:28:25  kevins
 * 	OT 12415 fixed suspect malloc lengths
 * 	[1994/09/29  21:27:43  kevins]
 * 
 * Revision 1.1.4.16  1994/08/10  13:31:44  kevins
 * 	OT 11575 untranslatable towers now displayed in binary format
 * 	[1994/08/10  10:36:40  kevins]
 * 
 * Revision 1.1.4.15  1994/08/02  20:09:43  kevins
 * 	OT 10187 Fix cdscache show command
 * 	[1994/08/02  17:57:31  kevins]
 * 
 * 	OT 10187 fixed cdscache show commands
 * 	[1994/08/02  17:20:49  kevins]
 * 
 * Revision 1.1.4.14  1994/06/09  16:01:39  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:02  devsrc]
 * 
 * Revision 1.1.4.13  1994/05/11  19:27:09  kevins
 * 	OT 10097 Modified argument checknig routines
 * 	[1994/05/11  19:25:27  kevins]
 * 
 * 	OT 10097 modified arg checking
 * 	[1994/05/11  16:12:21  kevins]
 * 
 * Revision 1.1.4.12  1994/05/05  21:03:04  peckham
 * 	Eliminate dnstabes reference.
 * 	[1994/05/05  20:57:16  peckham]
 * 
 * Revision 1.1.4.11  1994/05/03  13:48:55  kevins
 * 	Merged with changes from 1.1.4.10
 * 	[1994/05/03  13:48:43  kevins]
 * 
 * 	OT# 10500
 * 	Remove unnecessary cds include files.
 * 	[1994/05/03  12:52:48  kevins]
 * 
 * Revision 1.1.4.10  1994/04/29  15:49:21  peckham
 * 	Eliminate cdsExport_SimpleName() reference.
 * 	[1994/04/29  13:55:12  peckham]
 * 
 * Revision 1.1.4.10  1994/04/29  15:49:21  peckham
 * 	Eliminate cdsExport_SimpleName() reference.
 * 	[1994/04/29  13:55:12  peckham]
 * 
 * Revision 1.1.4.9  1994/04/07  10:22:08  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:17:50  kevins]
 * 
 * 	CR 10304 Modifications to help remove cross component dependency build issues
 * 	[1994/04/07  10:10:33  kevins]
 * 
 * Revision 1.1.4.8  1994/03/23  16:44:46  kevins
 * 	OT 10188 Added cadump/cds failure message
 * 	[1994/03/23  16:20:47  kevins]
 * 
 * Revision 1.1.4.7  1994/03/23  15:49:03  kevins
 * 	CR 10188 removed return of bogus error code in cdscache dump.
 * 	Also removed stale code.
 * 	[1994/03/23  15:48:39  kevins]
 * 
 * Revision 1.1.4.6  1994/03/14  17:39:35  kevins
 * 	CR9904 Added support for help system
 * 	[1994/03/14  13:44:19  kevins]
 * 
 * Revision 1.1.4.5  1994/01/12  13:02:52  kevins
 * 	replaced bzero() function with memset
 * 	[1994/01/12  13:01:43  kevins]
 * 
 * Revision 1.1.4.4  1993/12/29  15:58:08  kevins
 * 	removed cdscp_attr.h from includes
 * 	[1993/12/29  15:39:26  kevins]
 * 
 * Revision 1.1.4.3  1993/12/27  15:05:46  kevins
 * 	Remove dce_error function call
 * 	[1993/12/27  15:03:29  kevins]
 * 
 * Revision 1.1.4.2  1993/12/22  18:12:29  kevins
 * 	Message catalog cleanup
 * 	[1993/12/22  18:08:43  kevins]
 * 
 * 	Message catalog cleanup
 * 
 * Revision 1.1.4.1  1993/12/20  20:05:38  kevins
 * 	Fixed to conform to style guide.
 * 	[1993/12/20  18:51:38  kevins]
 * 
 * Revision 1.1.2.1  1993/10/21  19:39:28  kevins
 * 	completed the name change from dsh to dcecp
 * 	[1993/10/21  19:38:48  kevins]
 * 
 * Revision 1.1.2.3  1993/09/08  19:58:18  kevins
 * 	Merged with changes from 1.1.2.2
 * 	[1993/09/08  19:58:04  kevins]
 * 
 * 	Added cache commands
 * 	[1993/09/08  19:49:21  kevins]
 * 
 * Revision 1.1.2.2  1993/08/25  14:45:58  kevins
 * 	Initial sub for cds-cache commands
 * 	[1993/08/25  14:44:37  kevins]
 * 
 * $EndLog$
 */

/* INCLUDES */
#include <stdio.h>
#include <locale.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>
#include <fcntl.h>

/*
 * Don't inherit stubbase.h's use of the exception based pthreads API.
 */
#ifndef NCK
#  define NCK
#endif
#include <dce/rpc.h>
#include <dce/stubbase.h>

#ifndef SNI_SVR4_POSIX
#include <strings.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <dce/dce_error.h>
#include <sys/types.h>
#include <unistd.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dce_svc.h>
#include <dce/tcl.h>
#include <dce/cdsclerk.h>
#include <dce/sec_login.h>
#include <cprpc_clerk.h>
#include <cprpc_server.h>
#include <cdscp.h>
#include <dce/dnsmessage.h>
#include <dce/attributes.h>
#include <dce/dnsclerk.h>
#include <dce/cprpc.h>
#include <dce/cprpcp.h>
#include <dce/dnsmanpar.h>
#include <dce/cds_types.h>
#include <dce/rpc_cvt.h>
#include <dcecp.h>
#include <util_var.h>
#include <util_general.h>
#include <util_help.h>
#include <util_binding.h>

/* PROTOTYPES */
int dcp_show_cached_clearinghouse(Tcl_Interp *, rpc_binding_handle_t, char *);
int dcp_show_cached_server(Tcl_Interp *, rpc_binding_handle_t, char *);
void format_utc(utc_t *, char *);
int decode_replicapointer_towers(Set_t *, unsigned char *);
void clean_tower(char *);

/* DEFINES */
#define CDS_CLERK_CREATE_TIME 0           /* Creation time */
#define CDS_CLERK_PROTOCOL_ERROR_COUNT 1  /* Protocol error counter */
#define CDS_CLERK_AUTHENTICATION_COUNT 2  /* Authentication failure counter */
#define CDS_CLERK_READ_OPS_COUNT 3        /* Cache read ops counter */
#define CDS_CLERK_CACHE_HITS_COUNT 4      /* Cache hits counter */
#define CDS_CLERK_CACHE_BYPASS_COUNT 5    /* Cache bypassed counter */
#define CDS_CLERK_WRITE_OPS_COUNT 6       /* Cache writes counter */
#define CDS_CLERK_MISC_OPS_COUNT 7        /* Cache misc. operations counter */

#define REMOTE_CH_CREATE_TIME 2           /* Creation Time */
#define REMOTE_CH_READ_OPS_COUNT 10       /* C'house read operations counter */
#define REMOTE_CH_WRITE_OPS_COUNT 13      /* C'house write operations counter */
#define REMOTE_CH_MISC_OPS_COUNT 14       /* C'house misc operations counter */


/*
 * FUNCTION: cache_create
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *
 */
int
cache_create(ClientData clientData,
             Tcl_Interp *interp,
             int argc,
             char **argv)
{
    int         cds_status = CDS_SUCCESS;
    char        *binding = NULL;
    char        *server_name;
    char        **aargv;
    int         aargc;
    char        *temp_binding;
    unsigned32  rpc_status;

    dcp_ArgvInfo arg_table[] = {
        {"-binding", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_cds_cache_create_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&binding;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /*
     * If the parser comes back with less than two arguments the 
     * user typed something wrong.  Terminate command. 
     */
    DCP_CHECK_CDS_NO_ARG(cache);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);
 
    /*
     * If no binding value is specified, then this is an error.
     */
    if (!binding) {
        DCP_SET_MISC_ERROR(dcp_s_cds_no_binding_specified);
    }

    /* 
     * Retrieve the currently established network context.
     */
    do_sec_login();

    /*
     * Allocate and store the server/cache name for which the cache is 
     * being create. 
     */
    server_name = malloc(strlen(argv[1]) + 1);
    if (!server_name) {
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    strcpy(server_name, argv[1]);

    if (Tcl_SplitList(interp, binding, &aargc, &aargv) != TCL_OK) {
        free(server_name);
        return TCL_ERROR;
    }

    if (aargc > 1) {
        temp_binding = dcp_binding_tcl_to_string(aargc, aargv, &rpc_status);
        if (rpc_status != rpc_s_ok) {
            free(server_name);
            DCP_SET_MISC_ERROR(rpc_status);
        }
        strcpy(binding, temp_binding);
        rpc_string_free((unsigned_char_t **)&temp_binding, &rpc_status);
    }
    cds_status = cds_cache_command(server_name, binding, DEFCLKSRV);

    free(server_name);
    DCP_CHECK_CDS_ERROR(cds_status);

    return TCL_OK;
}


/*
 * FUNCTION: cache_delete
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *
 */
int
cache_delete(ClientData clientData,
             Tcl_Interp *interp,
             int argc,
             char **argv)
{
    int   cds_status = CDS_SUCCESS;
    char  *server_name;

    /* 
     * Check to see if the user just wants help.
     */
    DCP_NO_COMMAND_HELP();

    DCP_CHECK_CDS_NO_ARG(cache);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);
 
    /* 
     * Retrieve the currently established network context 
     */
    do_sec_login();

    /*
     * Allocate and store the server/cache name for which the cache is 
     * being create. 
     */
    server_name = malloc(strlen(argv[1]) + 1);
    if (!server_name) {
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    strcpy(server_name, argv[1]);

    cds_status = cds_cache_command(server_name, NULL, CLEARCLKSRV);
    DCP_CHECK_CDS_ERROR(cds_status);
    free(server_name);

    return TCL_OK;
}


/*
 * FUNCTION: cache_dump
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *
 */
int
cache_dump(ClientData clientData,
           Tcl_Interp *interp,
           int argc,
           char **argv)
{
    int          result = TCL_OK; 
    static char  *argva[] = {"cadump"};
    static int   argca = (sizeof(argva)/sizeof(argva[0]));
 
    /*
     * Check to see if the user just wants help.
     */
    DCP_NO_COMMAND_HELP();

    if (geteuid()) {
        DCP_SET_MISC_ERROR(dcp_s_cadump_not_root);
    }                       
    else
        result = cadump(argca, argva, 0); 

    if (result != CDS_SUCCESS) {   
        DCP_SET_MISC_ERROR(dcp_s_cadump_fail);
    }

    return TCL_OK;
}


/*
 * FUNCTION: cache_discard
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *
 */
int
cache_discard(ClientData clientData,
              Tcl_Interp *interp,
              int argc,
              char **argv)
{
    char               command[1024];
    extern Tcl_Interp  *dir_interp;
    unsigned32         result = error_status_ok;

    /*
     * Check to see if the user just wants help
     */
    DCP_NO_COMMAND_HELP();

    /*
     * Check for extra arguments, beyond the optional hostname.
     */
    DCP_CHECK_EXTRA_ARGS();

    /*
     * If a hostname is present, set the namelist variable.
     */
    if (argv[1] != NULL) {
        DCP_GENERAL_SET_NAMELIST(argv[1]);
    }
 
    if (argv[1] == NULL) {
        sprintf(command, "_dcp_cache_discard NULL");
    }
    else {
        sprintf(command, "_dcp_cache_discard %s", argv[1]);
    }

    if ((result = Tcl_Eval(dir_interp, command)) != TCL_OK) {
        DCP_SET_RESULT(dir_interp->result);
        return TCL_ERROR;
    }

    return TCL_OK;
}


/*
 * FUNCTION: cache_show
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *
 */
int
cache_show(ClientData clientData, 
           Tcl_Interp *interp, 
           int argc, 
           char **argv)
{
    int                   clearinghouse;
    int                   server;
    rpc_binding_handle_t  rpc_binding;
    unsigned32            rpc_status;

    dcp_ArgvInfo arg_table[] = {
        {"-clearinghouse", DCP_ARGV_CONSTANT, (char *)1, NULL,
             dcp_t_cds_cache_server_clearinghouse_help},
        {"-server", DCP_ARGV_CONSTANT, (char *)1, NULL,
             dcp_t_cds_cache_server_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    server = 0;
    clearinghouse = 0;

    arg_table[0].dst = (char *)&clearinghouse;
    arg_table[1].dst = (char *)&server;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /*
     * If the parser comes back with less than two arguments the 
     * user typed something wrong.  Terminate command. 
     */
    DCP_CHECK_CDS_NO_ARG(cache);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (!clearinghouse && !server) {
        DCP_SET_MISC_ERROR(dcp_s_cds_cache_show);
    }

    if (clearinghouse && server) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /*
     * Retrieve the currently established network context 
     */
    do_sec_login();

    /*
     * Get binding to CDS server.
     */
    rpc_binding = get_default_handle(0, &rpc_status);
    DCP_CHECK_RPC_ERROR(rpc_status);

    if (clearinghouse) {
        if (dcp_show_cached_clearinghouse(interp, rpc_binding, argv[1]) != TCL_OK)
            return TCL_ERROR;
    }
    else {
        if (dcp_show_cached_server(interp, rpc_binding, argv[1]) != TCL_OK)
            return TCL_ERROR;
    }
    return TCL_OK;
}


/*
 * FUNCTION: dcp_show_cached_clearinghouse
 *
 * OVERVIEW:
 *  This aquires and formats the information on cached clearinghouses.
 *
 * INPUTS:
 *  rpc_binding_handle_t rpc_binding - the binding to a cdsclerk
 *  char *clearinghouse_list - the clearinghouse to aquire the information for.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - contains the formated output. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success.
 */
int
dcp_show_cached_clearinghouse(Tcl_Interp *interp,
                              rpc_binding_handle_t rpc_binding,
                              char *clearinghouse_list)
{
    command_status_t   cds_status = CDS_SUCCESS;
    clh_info_list_t    clh_info_list;
    clh_info_list_p_t  info_ptr;
    int                i = 0, j = 0;
    unsigned32         euid;
    int                id;
    cds_FullName_t     *rep_nm;
    char               buffer[CDS_STR_FNAME_MAX];
    char               clr_buffer[CDS_STR_FNAME_MAX];
    char               clr_name[CDS_FNAME_MAX];
    Set_t              *tower_set;
    char               *clr_svc; 
    char               time[UTC_MAX_STR_LEN];
    char               *time_svc;
    unsigned char      tower[CDS_STR_FNAME_MAX];
    char               *read_svc;
    char               read[CDS_STR_FNAME_MAX];
    char               *write_svc; 
    char               write[CDS_STR_FNAME_MAX];
    char               *other_svc;
    char               other[CDS_STR_FNAME_MAX];
    cprpc_attr_p_t     attr_p;
    char               *conical_list;
    unsigned32         rpc_status;
    char               *conical_command = "set result {};\
                           foreach el {%s} {lappend result [name expand $el]};\
                           set result";
    char               *lsearch_command = "lsearch {%s} %s";
    char               *lreplace_command = "lreplace {%s} %d %d";
    int                lindex;
    char               *command;
    extern Tcl_Interp  *attr_interp;

    memset(tower, 0, CDS_STR_FNAME_MAX);
    memset(read, 0, CDS_STR_FNAME_MAX);
    memset(write, 0, CDS_STR_FNAME_MAX);
    memset(other, 0, CDS_STR_FNAME_MAX);

    euid = (long)geteuid();
    clh_info_list.clh_count = 0;
    clh_info_list.clh_members[0] = NULL;
    info_ptr = &clh_info_list;

    rpc_status = dnscp_clerk(rpc_binding,
                             SHOWCLKCLE,
                             euid,
                             NULL,
                             &info_ptr,
                             NULL,
                             &cds_status);
    DCP_CHECK_RPC_ERROR(rpc_status);
    DCP_CHECK_CDS_ERROR(cds_status);

    time_svc = (char *)dce_sprintf(dcp_t_cds_create_time);
    read_svc = (char *)dce_sprintf(dcp_t_cds_cache_read_access);
    write_svc = (char *)dce_sprintf(dcp_t_cds_cache_write_access);
    other_svc = (char *)dce_sprintf(dcp_t_cds_others);
    clr_svc = (char *)dce_sprintf(dcp_t_cds_clrname);

    command = malloc(strlen(conical_command) + strlen(clearinghouse_list) + 1);
    sprintf(command, conical_command, clearinghouse_list);
    if (Tcl_Eval(interp, command) == TCL_OK) {
        conical_list = malloc(strlen(interp->result) + 50);
        strcpy(conical_list, interp->result);
        Tcl_ResetResult(interp);
    }
    else {
        free(time_svc);
        free(read_svc);
        free(write_svc);
        free(other_svc);
        free(clr_svc);
        free(command);
        DCP_SET_MISC_ERROR(dcp_s_cds_not_valid_clearinghouse);
    }
    free(command);

    /*
     * Go through the clearinghouse list to find the cached information.
     */
    for (i = 0 ; i < (int)info_ptr->clh_count && strlen(conical_list); i++) {
        rep_nm = &(info_ptr->clh_members[i]->clh_replica_p->rp_chname);
        strncpy(clr_name, (char *)rep_nm->fn_name, rep_nm->fn_length);
        clr_name[rep_nm->fn_length] = '\0';
        command = malloc(strlen(clr_name) + strlen(conical_list) +
                         strlen(lsearch_command) + 1);
        sprintf(command, lsearch_command, conical_list, clr_name);
        Tcl_Eval(attr_interp, command);
        lindex = atoi(attr_interp->result);
        free(command);
        if (lindex != -1) {
            command = malloc(strlen(conical_list) + strlen(lreplace_command) + 7);
            sprintf(command,
                    lreplace_command,
                    conical_list,
                    lindex,
                    lindex);
            Tcl_Eval(attr_interp, command);
            strcpy(conical_list, attr_interp->result);
            free(command);
            sprintf(clr_buffer, "{%s %s}", clr_svc, clr_name);
            tower_set = (Set_t *)malloc(MAXATTRIBUTE);
            RPTowerUnpack(info_ptr->clh_members[i]->clh_replica_p->rp_towers, tower_set);
            decode_replicapointer_towers(tower_set, tower);
            for (j = 0; 
                 j < (int)info_ptr->clh_members[i]->attr_list_p->attr_len; 
                 j++) {
                attr_p = info_ptr->clh_members[i]->attr_list_p->attr_members[j];
                id = attr_p->attr_id;
                switch (id) {
                    case REMOTE_CH_READ_OPS_COUNT:
                        sprintf(read, "{%s %d}", read_svc,
                                attr_p->value.av_val.av_counter);
                        break;
                    case REMOTE_CH_WRITE_OPS_COUNT:
                        sprintf(write, "{%s %d}", write_svc,
                                attr_p->value.av_val.av_counter);
                        break;
                    case REMOTE_CH_MISC_OPS_COUNT: 
                        sprintf(other, "{%s %d}", other_svc,
                                attr_p->value.av_val.av_counter);
                        break;
                    case REMOTE_CH_CREATE_TIME:
                        format_utc((utc_t *)attr_p->value.av_val.av_utctime, time);
                        sprintf(buffer, "{%s %s}", time_svc, time);
                        break;
                    default:
                        break;
                }
            }
            DCP_APPEND_RESULT(clr_buffer);
            DCP_APPEND_RESULT(buffer);
            DCP_APPEND_RESULT(other);
            DCP_APPEND_RESULT(read);
            clean_tower((char *)tower);
            DCP_APPEND_RESULT(tower);
            DCP_APPEND_RESULT(write);
        }
    }
    free(time_svc);
    free(read_svc);
    free(write_svc);
    free(other_svc);
    free(clr_svc);
    if (strlen(conical_list)) {
        free(conical_list);
        DCP_SET_MISC_ERROR(dcp_s_cds_not_valid_clearinghouse);
    }
    free(conical_list);
    return TCL_OK;
}


/*
 * FUNCTION: dcp_show_cached_server
 *
 * OVERVIEW:
 *  This retrieves and formats the name and towers of a cached server.
 *
 * INPUTS:
 *  rpc_binding_handle_t rpc_binding - Binding to the cdsclerk
 *  char *server_list - list of servers to view.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - contains the formated output.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK  - on success
 */
int
dcp_show_cached_server(Tcl_Interp *interp,
                       rpc_binding_handle_t rpc_binding,
                       char *server_list)
{
    command_status_t     cds_status = CDS_SUCCESS;
    int                  i, j, k, l;
    int                  attr_id;
    int                  argc;
    char                 **argv;
    unsigned32           euid;
    ns_info_list_t       cds_ns;
    ns_info_list_p_t     cds_ns_p;
    unsigned32           rpc_status;
    SimpleName_t         cached_server;
    SimpleName_t         *ptr_server;
    cprpc_attr_list_p_t  attr_list;
    FullName_t           Fname;
    attr_AtomicValue_t   *attr_val;
    char                 clr_buffer[CDS_STR_FNAME_MAX];
    char                 *clr_svc;
    char                 buf[CDS_STR_SNAME_MAX];
    int                  buflen;
    unsigned char        tower[CDS_STR_FNAME_MAX];
    unsigned char        server_name[CDS_STR_SNAME_MAX];
    int                  length = CDS_STR_SNAME_MAX;
    Set_t                *towerset_ptr;
    unsigned32           found;

    euid = (long)geteuid();
    cds_ns.ns_count = 0;
    cds_ns.ns_members[0] = NULL;

    cds_ns_p = &cds_ns;

    rpc_status = dnscp_clerk(rpc_binding,
                             SHOWCLKSRV,
                             euid,
                             NULL,
                             NULL, 
                             &cds_ns_p,
                             &cds_status);	
    DCP_CHECK_RPC_ERROR(rpc_status);
    DCP_CHECK_CDS_ERROR(cds_status); 

    clr_svc = (char *)dce_sprintf(dcp_t_cds_clrname);

    if (Tcl_SplitList(interp, server_list, &argc, &argv) != TCL_OK) {
        free(clr_svc);
        return TCL_ERROR;
    }

    for (l = 0; l < argc; l++) {
        for (k = 0, found = FALSE; k < (int)cds_ns_p->ns_count ; k++) {
            cdsImport_SimpleName((cds_visa_t *)NULL,
                                 cds_ns_p->ns_members[k]->ns_tag_p,
                                 &cached_server,
                                 FALSE);  
            ptr_server = &cached_server;
            cdsCvtSimpleToStr((cds_simple_name_t *)ptr_server,
                              server_name,
                              &length);
            if (!strcmp((char *)server_name, argv[l])) {
                attr_list = cds_ns_p->ns_members[k]->attr_list_p;
                j = attr_list->attr_len;
                for (i = 0; i < j ; i++) {
                    attr_val = &attr_list->attr_members[i]->value;
                    attr_id = attr_list->attr_members[i]->attr_id;
                    switch (attr_id) {
                        case CP_FullName:
                            /*
                             * Format the cached nameserver fullname.
                             */
                            cdsImport_FullName((cds_visa_t *)NULL,
                                               attr_val->av_val.av_fullname_p,
                                               &Fname,
                                               0);
                            buflen = sizeof(buf);
                            cds_status = dnsCvtCDSFullToStr((dns_full_name_t *)&Fname,
                                                            (unsigned char *)buf,
                                                            &buflen);
                            DCP_CHECK_CDS_ERROR(cds_status);
                            sprintf(clr_buffer,"{%s %s}",clr_svc,buf);
                            break;

                        case CP_ReplicaPointer:
                            /*
                             * Format the cached nameserver tower.
                             */
                            towerset_ptr = (Set_t *) malloc(MAXATTRIBUTE);
                            RPTowerUnpack(attr_val->av_val.av_rp_p->rp_towers,
                                          towerset_ptr);

                            decode_replicapointer_towers(towerset_ptr, tower);
                            clean_tower((char *)tower);
                            break;
                    }
                }
                DCP_APPEND_RESULT(clr_buffer);
                DCP_APPEND_RESULT(tower);
                found = TRUE;
            }
        }
        if (!found) {
            free(clr_svc);
            DCP_SET_CDS_ERROR_ARG(dcp_s_cds_no_cache_server, argv[l]);
        }
    }
    free(clr_svc);
    free((char *)argv);
    return TCL_OK;
}


/*
 * FUNCTION: cds_cache_command
 *
 * OVERVIEW: This function performs a variety of cache
 *           functions (create and delete).
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int
 */
int
cds_cache_command(char *server_name,
                  char *binding,
                  unsigned32 command)
{
    rpc_binding_handle_t  rpc_binding;
    unsigned32            rpc_status = rpc_s_ok;
    unsigned32            euid;
    command_status_t      command_status = CDS_SUCCESS;
    ns_info_t             ns_info;
    ns_info_list_p_t      ns_info_list_p;
    cprpc_attr_t          ns_attr;
    cprpc_attr_list_p_t   ns_attr_list_p;
    cds_Name_t            ns_tag;
    cds_OpenChar_t        *tower_string_valp;
    int                   tower_string_len;
    int                   server_name_len = sizeof(ns_tag.nm_name);

    tower_string_valp = NULL;
    rpc_binding = NULL;

    /*
     * Convert the string server cache name into a opaque simple name.
     */
    cdsCvtSimpleToOpq((unsigned char *)server_name,
                      0,
                      (cds_simple_name_t *)ns_tag.nm_name,
                      &server_name_len);
    ns_tag.nm_length = server_name_len;

    /*
     * Set appropiate pointers.
     */
    if (binding) {
        tower_string_len = strlen(binding);
        tower_string_valp = (cds_OpenChar_t *)malloc(sizeof(cds_OpenChar_t) +
                                                     tower_string_len + 1);
        if (!tower_string_valp)
            return dcp_s_no_memory;

        tower_string_valp->op_length = tower_string_len + 1;
        strcpy ((char *)(tower_string_valp->op_array), binding);
        tower_string_valp->op_array[tower_string_len] = '\0';
    }

    /*  
     * Get authenticated binding handle if we don't already have one.
     */
    if (rpc_binding == NULL) {
        rpc_binding = get_default_handle(0, &rpc_status);
        if (rpc_status != rpc_s_ok) {
            free(tower_string_valp);
            return rpc_status;
        }
    }

    /*
     * Convert the tower string to an attribute.
     */
    ns_attr.attr_id = CP_char;
    ns_attr.value.av_valuetype = CP_char;
    if (binding)
        ns_attr.value.av_val.av_char_p = tower_string_valp;

    /*
     * Allocate and initialize an attribute list.
     */
    ns_attr_list_p = (cprpc_attr_list_t *)malloc(sizeof(cprpc_attr_list_t) +
                                                 sizeof(cprpc_attr_list_p_t));
    if (!ns_attr_list_p) {
        free(tower_string_valp);
        return dcp_s_no_memory;
    }
    ns_attr_list_p->attr_members[0] = &ns_attr;
    ns_attr_list_p->attr_len = 1;

    /*
     * Init our ns 'info' structure.
     */
    ns_info.ns_tag_p = &ns_tag;
    if (binding)
        ns_info.attr_list_p = ns_attr_list_p;
    else
        ns_info.attr_list_p = NULL;

    /*
     * Init our in/out list parameter.
     */
    ns_info_list_p = (ns_info_list_p_t) malloc(sizeof(ns_info_list_t) +
                                               sizeof(ns_info_list_p_t));
    if (!ns_info_list_p) {
        free(tower_string_valp);
        free(ns_attr_list_p);
        return dcp_s_no_memory;
    }
    ns_info_list_p->ns_members[0] = &ns_info;
    ns_info_list_p->ns_count = 1;

    euid = (long)geteuid();

    /*
     * Make the cache call to the server.
     */
    rpc_status = dnscp_clerk(rpc_binding,
                             command,
                             euid,
                             NULL,
                             NULL,
                             &ns_info_list_p,
                             &command_status);

    /*
     * Free associated memory.
     */
    free(tower_string_valp);
    free(ns_attr_list_p);
    free(ns_info_list_p);

    /* 
     * Check for command and rpc transmission success.
     */
    if (command_status != CDS_SUCCESS)
        return command_status;

    if (rpc_status != rpc_s_ok)
        return rpc_status;

    return CDS_SUCCESS;
}


/*
 * FUNCTION: clean_tower
 *
 * OVERVIEW: cleans up a tcl tower attribute string. The tower 
 *           attribute strings are generated from Replica Pointer
 *           structures and need to be cleaned for use in cache displays.
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  char *tower - the tcl attribute string to be cleaned.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void
clean_tower(char *tower)
{
    int                argc;
    char               **argv;
    char               *tmp_argv;
    char               *tmp_tower;
    char               c;
    extern Tcl_Interp  *attr_interp;
    int                i, j, k;
    unsigned           cr = FALSE;

    if (Tcl_SplitList(attr_interp, tower, &argc, &argv) != TCL_OK)
        return;

    if (argc > 0)
        tmp_tower = malloc((strlen(tower) * argc) + argc);
    else
        tmp_tower = malloc(strlen(tower) + 1);
    *tmp_tower = '\0';

    for (i = 0 ; i < argc; i++) {
        if (strchr(argv[i],'\n')) {
            k = 0;
            j = 0;
            tmp_argv = malloc(strlen(argv[i]) + 1);
            memset(tmp_argv, 0, (strlen(argv[i]) + 1));
            while (*(argv[i] + j) != '\0') {
                c = *(argv[i] + j + 1);
                if (*(argv[i] + j) == '\n') {
                    *(tmp_argv + k) = *(argv[i] + j);
                    k++;
                    cr = TRUE;
                }
                else if (cr && c == ' ' && c != '{') {
                    ;
                }
                else if (cr && c != ' ' && c != '{') {
                    *(tmp_argv + k) = ' ';
                    k++;
                    *(tmp_argv + k) = *(argv[i] + j);
                    k++;
                    cr = FALSE;
                }
                else {
                    cr = FALSE;
                    *(tmp_argv + k) = *(argv[i] + j);
                    k++;
                }
                j++;
            }
            *(tmp_argv + strlen(tmp_argv)+1) = '\0';
        }
        else {
            tmp_argv = dce_strdup(argv[i]);
        }
        if (strlen(tmp_tower))
            sprintf(tmp_tower, "%s\n{%s}", tmp_tower, tmp_argv);
        else
            sprintf(tmp_tower, "{%s}", tmp_argv);
        free(tmp_argv);
    }
    strcpy(tower, tmp_tower);
    free(tmp_tower);
    free((char *)argv);
}
