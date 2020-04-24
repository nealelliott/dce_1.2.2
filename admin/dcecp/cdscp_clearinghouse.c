/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdscp_clearinghouse.c,v $
 * Revision 1.1.14.1  1996/08/09  11:45:39  arvind
 * 	Merge changes.
 * 	[1996/06/10  17:49 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/18  17:36 UTC  truitt  /main/HPDCE02/8]
 *
 * 	Merge.
 * 	[1996/03/18  17:34 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/10  14:25 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.12.3  1996/02/18  19:12:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:36  marty]
 * 
 * Revision 1.1.12.2  1995/12/08  21:19:54  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/7  1995/08/22  20:43 UTC  truitt
 * 	Merge clearinghouse work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts45/1  1995/08/22  20:42 UTC  truitt
 * 	CHFts16073: Correctly report errors when a completely bogus clearinghouse
 * 	name is used.
 * 	CHFts16097: Ensure clearinghouse show options play well together.
 * 
 * 	HP revision /main/HPDCE02/6  1995/04/06  14:08 UTC  jrr
 * 	Fix unchecked strcpy() of user input into fixed size buffer (CHFts14888).
 * 	[1995/04/05  21:33 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14888/1]
 * 
 * 	HP revision /main/HPDCE02/5  1995/04/06  13:55 UTC  jrr
 * 	Fix various memory leaks (CHFts14834).
 * 	[1995/04/05  14:31 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14834/1]
 * 
 * 	HP revision /main/HPDCE02/4  1995/03/27  17:28 UTC  jrr
 * 	Check for NULL return from cdsGetHandle() (CHFts14682).
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_cr12550/1  1995/03/27  16:08 UTC  jrr
 * 	Check for NULL return from cdsGetHandle() (CHFts14682).
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/14  15:54 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  15:53 UTC  truitt
 * 	CHFts14285: Combine all cds check args macros into one,
 * 	since all six are the same anyway.  Also, make sure correct
 * 	macros are called when processing an error condition.
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/05  18:05 UTC  truitt
 * 	OT12710: Remove useless macro code.
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/03  19:55 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/11/03  19:53 UTC  truitt
 * 	Fix missing clearinghouse command switches.
 * 	[1995/12/08  17:54:36  root]
 * 
 * Revision 1.1.4.23  1994/09/16  14:50:13  kevins
 * 	OT 11270 Implemented clearinghouse catalog
 * 	[1994/09/16  14:47:27  kevins]
 * 
 * Revision 1.1.4.22  1994/09/12  19:54:02  kevins
 * 	OT 12095 Made the -timestamps a required option on the repair
 * 	command and the -checkpoint a required option on the
 * 	initiate command.
 * 	[1994/09/12  13:54:06  kevins]
 * 
 * Revision 1.1.4.21  1994/09/07  15:12:26  kevins
 * 	OT 12064 removed repair notices
 * 	[1994/09/07  14:27:32  kevins]
 * 
 * Revision 1.1.4.20  1994/09/06  13:15:50  kevins
 * 	OT 12044 Modified clearinghouse error messages
 * 	[1994/09/06  12:58:36  kevins]
 * 
 * Revision 1.1.4.19  1994/09/02  21:03:17  zee
 * 	Add uuid argument to get_clearinghouse_handle (for cdsalias usage).
 * 	[1994/09/02  18:40:36  zee]
 * 
 * Revision 1.1.4.18  1994/08/26  18:31:37  kevins
 * 	OT 11888 replace  obsolete constants
 * 	[1994/08/26  18:22:24  kevins]
 * 
 * Revision 1.1.4.17  1994/08/19  21:09:53  zee
 * 	Add clringhouse_repair command.
 * 	[1994/08/19  18:10:23  zee]
 * 
 * Revision 1.1.4.16  1994/08/02  20:12:47  kevins
 * 	OT 11361 fixed clearinghouse counter format
 * 	[1994/08/02  17:30:11  kevins]
 * 
 * Revision 1.1.4.15  1994/08/01  15:07:17  zee
 * 	Add clringhouse_initiate and clringhouse_verify commands.
 * 	[1994/08/01  15:05:46  zee]
 * 
 * Revision 1.1.4.14  1994/07/29  20:37:24  kevins
 * 	OT 11451 mad 64 bit changes requested by a_ferris
 * 	[1994/07/29  20:19:14  kevins]
 * 
 * Revision 1.1.4.13  1994/07/12  19:26:22  zee
 * 	Include cprprc_hcell.h and incorporate hcell interface in
 * 	get_clearinghouse_handle.
 * 	[1994/07/12  15:55:40  zee]
 * 
 * Revision 1.1.4.12  1994/06/09  16:01:41  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:03  devsrc]
 * 
 * Revision 1.1.4.11  1994/05/05  21:03:06  peckham
 * 	Eliminate dnstabes reference.
 * 	[1994/05/05  20:57:52  peckham]
 * 
 * Revision 1.1.4.10  1994/05/03  13:48:56  kevins
 * 	OT#  10407  10500  10408 10311
 * 	Remove unnecessary cds include files.
 * 	Display multivalued attributes correctly.
 * 	Set the attribute types appropriately.
 * 	Correct the display format to correctly display tcl lists.
 * 	[1994/05/03  12:55:01  kevins]
 * 
 * Revision 1.1.4.9  1994/04/07  10:22:09  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:17:52  kevins]
 * 
 * 	CR 10304 Modifications to help remove cross component dependency build issues
 * 	[1994/04/07  10:10:34  kevins]
 * 
 * Revision 1.1.4.8  1994/03/26  11:56:08  kevins
 * 	CR 10230 removed instances of extern Tcl_Interp *interp from sources
 * 	[1994/03/26  11:55:54  kevins]
 * 
 * Revision 1.1.4.7  1994/03/14  17:39:36  kevins
 * 	CR9904 Added support for help system
 * 	[1994/03/14  13:44:20  kevins]
 * 
 * Revision 1.1.4.6  1994/03/10  15:50:26  kevins
 * 	CR 10030 removed extra newlines
 * 	[1994/03/10  15:42:14  kevins]
 * 
 * Revision 1.1.4.5  1994/02/02  14:32:01  kevins
 * 	Changed NULL_Set to NULL CR 9766
 * 	[1994/02/02  14:31:00  kevins]
 * 
 * Revision 1.1.4.4  1994/01/12  13:02:54  kevins
 * 	replaced bzero() function with memset
 * 	[1994/01/12  13:01:45  kevins]
 * 
 * Revision 1.1.4.3  1993/12/27  15:05:47  kevins
 * 	Remove dce_error function call
 * 	[1993/12/27  15:03:25  kevins]
 * 
 * 	removed dce_error call
 * 	[1993/12/27  13:45:16  kevins]
 * 
 * Revision 1.1.4.2  1993/12/22  18:12:30  kevins
 * 	Message catalog cleanup
 * 	[1993/12/22  18:08:44  kevins]
 * 
 * 	Message catalog cleanup
 * 	[1993/12/22  12:33:30  kevins]
 * 
 * Revision 1.1.4.1  1993/12/20  20:05:48  kevins
 * 	updated messages for cdscp sub-component
 * 	[1993/12/20  18:56:12  kevins]
 * 
 * Revision 1.1.2.2  1993/10/22  11:16:33  kevins
 * 	updated to tcl7.0
 * 	[1993/10/22  11:15:27  kevins]
 * 
 * Revision 1.1.2.1  1993/10/21  19:39:30  kevins
 * 	completed the name change from dsh to dcecp
 * 	[1993/10/21  19:38:50  kevins]
 * 
 * Revision 1.1.2.5  1993/10/14  19:27:58  kevins
 * 	Defuncted the clearinghouse status command
 * 	[1993/10/14  19:27:45  kevins]
 * 
 * Revision 1.1.2.4  1993/09/15  13:48:35  kevins
 * 	corrected problems with show routine
 * 	[1993/09/15  13:48:08  kevins]
 * 
 * Revision 1.1.2.3  1993/08/25  20:23:26  kevins
 * 	Altered method for reading attributes
 * 	[1993/08/25  20:21:33  kevins]
 * 
 * Revision 1.1.2.2  1993/08/23  14:27:34  kevins
 * 	Initial submission of clearinghouse commands
 * 	[1993/08/23  14:17:51  kevins]
 * 
 * $EndLog$
 */

/* INCLUDES */
#include <stdio.h>

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
#include <cdscp.h>
#include <dce/tcl.h>
#include <dce/cdsclerk.h>
#include <dce/sec_login.h>
#include <cprpc_clerk.h>
#include <cprpc_server.h>
#include <cprpc_hcell.h>
#include <dce/dnsmessage.h>
#include <dce/attributes.h>
#include <dce/dnsclerk.h>
#include <dce/rpc_cvt.h>
#include <dce/cprpc.h>
#include <dce/cprpcp.h>
#include <dce/dnsmanpar.h>
#include <dce/server_ncl_defs.h>
#include <dcecp.h>
#include <util_general.h>
#include <util_var.h>
#include <util_help.h>

/* MACROS */
#define APPEND_CH_COUNTER(buffer) {                                        \
    char *tmp;                                                             \
    tmp = Tcl_GetVar(attr_interp, "clear_counters", 0);                    \
    if (tmp)                                                               \
        Tcl_SetVar(attr_interp, "clear_counters", "\n", TCL_APPEND_VALUE); \
    Tcl_SetVar(attr_interp, "clear_counters", buffer, TCL_APPEND_VALUE);   \
    free(buffer);                                                          \
}

/* PROTOTYPES */
void get_towerset(cds_FullName_t *, cds_Set_t **, error_status_t *);
void translate_counter64(cprpc_attr_t *, char *);
int show_clearinghouse_counters(cprpc_attr_list_t *);


/*
 * FUNCTION: clringhouse_catalog
 *
 * OVERVIEW: This function will list all of the clearinghouses that exist in 
 *    the provided pathname. The pathname defaults to '/.:' if not provided.
 *
 * INPUTS:
 *  ClientData clientData
 *  int argc - total number of arguments
 *  char **argv - total arguments
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - contains the error message if any also contains 
 *                 the result (clearinghouse names) on success
 *
 * RETURNS:
 *  TCL_OK - on success
 *
 */
int
clringhouse_catalog(ClientData clientData, 
                    Tcl_Interp *interp, 
                    int argc, 
                    char **argv)
{
    cds_full_name_t     dir_name;
    int                 cds_status = CDS_SUCCESS;
    cds_handle_t        cds_context_handle;
    cds_simple_name_t   wildcard;
    int                 cds_str_sname_len=CDS_STR_SNAME_MAX;
    cds_simple_name_t   *cds_str_sname;
    char                global_name[CDS_STR_SNAME_MAX];
    char                clr_name[CDS_STR_SNAME_MAX];
    char                **cargv;
    int                 cargc;
    int                 i;
    boolean32           simplename = FALSE;
    char                *temp;
    char                root_name[CDS_SNAME_MAX];
    struct cdsFlagStat  flags;
    int                 size;

    dcp_ArgvInfo arg_table[] = { 
        {"-simplename", DCP_ARGV_CONSTANT, (char *)1, NULL,
             dcp_t_cds_clear_list_smpl_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&simplename;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();
 
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    strcpy(global_name, "/.:");

    /*
     * Set up the wildcard variable.
     */
    size = sizeof(wildcard);
    cds_status = cdsCvtSimpleToOpq((unsigned char *)"*", 1, &wildcard, &size);
    DCP_CHECK_CDS_ERROR(cds_status);

    /*
     * If no arguments are provided establish a pseudo argument.
     */
    if (argc > 1) {
        DCP_CHECK_EXTRA_ARGS();
        DCP_GENERAL_SET_NAMELIST(argv[1]);
        if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK)
            return TCL_ERROR;
    }
    else {
        cargc = 1;
        cargv = (char **)malloc(2*sizeof(char *));
        if (cargv == NULL) {
            DCP_SET_RESULT_CODE(dcp_s_no_memory);
            return TCL_ERROR;
        }
        cargv[0] = (char *)global_name;
        cargv[1] = NULL;
    }

    /*
     * Set the login context appropriately 
     */
    do_sec_login();

    /*
     * Foreach entry in the argument list find the 
     * clearinghouse name.
     */
    for (i = 0; i < cargc; i++) {
        /*
         * Save a copy of the root pathname for full/simple translation.
         */
        temp = expand_cell_name(cargv[i], (error_status_t *)&cds_status);
        DCP_CHECK_RPC_ERROR(cds_status);
        strcpy(root_name, temp);
        free(temp);

        /*
         * Convert the pathname to opaque.
         */
        size = sizeof(dir_name);
        cds_status = cdsCvtFullToOpq((unsigned char *)root_name, &dir_name, &size);
        DCP_CHECK_CDS_ERROR(cds_status);

        /*
         * Initialize the namespace.
         */
        cds_context_handle = cdsGetHandle();
        if (cds_context_handle == NULL_CDS_HANDLE) {
            free((char *)cargv);
            DCP_SET_RESULT_CODE(dcp_s_cds_no_handle);
            return TCL_ERROR;
        }

        clear_flags(&flags,cdsOpqDNA);
        cds_status = CDS_SUCCESS;
        
        /*
         * Enumerate each object until all have been aquired.
         */
        while (cds_status != CDS_EMPTY) {
            cds_str_sname_len = CDS_STR_SNAME_MAX;
            cds_status = cdsEnumObj(&dir_name,
                                    cds_context_handle,
                                    (cds_simple_name_t *)cds_classlist[0].cname,
                                    (cds_simple_name_t *)&wildcard,
                                    NULL,
                                    &cds_str_sname,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    &flags);
            if (cds_status == CDS_ERROR) {
                free((char *)cargv);
                DCP_SET_MISC_ERROR(flags.fsLocStat);
            }
            else if (cds_status == CDS_SUCCESS) {
                /*
                 * Convert the clearinghouse name to a string.
                 */
                cds_status = cdsCvtSimpleToStr(cds_str_sname,
                                               (unsigned char *)clr_name,
                                               &cds_str_sname_len);
                DCP_CHECK_CDS_ERROR(cds_status);

                /*
                 * Allocate the output buffer and generate the name.
                 */
                temp = malloc(strlen(clr_name) + strlen(root_name) + 2);
                if (strlen(interp->result))
                    Tcl_AppendResult(interp, "\n", NULL);
                if (!simplename)
                    sprintf(temp, "%s/%s", root_name, clr_name);
                else
                    strcpy(temp, clr_name);
                Tcl_AppendResult(interp, temp, NULL); 
                free(temp);
            }
        }

        /*
         * Reset the namespace.
         */
        cdsFreeHandle(cds_context_handle);
    }

    free((char *)cargv);
    return TCL_OK;
}


/*
 * FUNCTION: clringhouse_create()
 *
 * OVERVIEW: This function creates a clearinghouse as named.
 *
 * INPUTS:
 *  int  argc - Number of input arguments (argv)
 *  char **argv - value of all input arguments.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp -  Interpreter structure for variables and return 
 *                       messages.
 *
 * OUTPUTS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *
 */
int
clringhouse_create(ClientData clientData,
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    cds_full_name_t  clr_name;
    int              cds_status = CDS_SUCCESS;
    char             **cargv;
    int              cargc;
    int              i;

    dcp_ArgvInfo arg_table[] = { 
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    DCP_CHECK_CDS_NO_ARG(clearinghouse);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Set the login context appropriately.
     */
    do_sec_login();

    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK)
        return TCL_ERROR;

    for (i = 0; i < cargc; i++) {
        DCP_CHECK_CDS_NAME_LEN(cargv[i]);
        strcpy((char *)clr_name.uc, cargv[i]);
        cds_status = clrnghouse_command(clr_name, CRECLECMD);
        DCP_CHECK_CDS_ERROR(cds_status);
    }

    free((char *)cargv);
    return TCL_OK;
}


/*
 * FUNCTION: clringhouse_delete()
 *
 * OVERVIEW: This function deletes the named clearinghouse.
 *
 * INPUTS:
 * 
 *  int  argc - Number of input arguments (argv)
 *  char **argv - value of all input arguments.
 *
 * INPUTS/OUTPUTS:
 * Tcl_Interp *interp - Interpreter structure for variables and return 
 *                      messages.
 * 
 * OUTPUTS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure.
 *
 */
int
clringhouse_delete(ClientData clientData,
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    cds_full_name_t  clr_name;
    int              cds_status = CDS_SUCCESS;
    char             **cargv;
    int              cargc;
    int              i;

    dcp_ArgvInfo arg_table[] = { 
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    DCP_CHECK_CDS_NO_ARG(clearinghouse);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    do_sec_login();

    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK)
        return TCL_ERROR;

    for(i = 0; i < cargc; i++) {
        DCP_CHECK_CDS_NAME_LEN(cargv[i]);
        strcpy((char *)clr_name.uc, cargv[i]);
        cds_status = clrnghouse_command(clr_name, DELCLECMD);
        DCP_CHECK_CDS_ERROR(cds_status);
    }
    free((char *)cargv);
    return TCL_OK;
}


/*
 * FUNCTION:  clringhouse_disable()
 * 
 * OVERVIEW: This function disables the named clearinghouse.
 *
 * INPUTS:
 *  int  argc - Number of input arguments (argv)
 *  char **argv - value of all input arguments.
 *
 * INPUT/OUTPUT:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *
 */
int
clringhouse_disable(ClientData clientData,
                    Tcl_Interp *interp,
                    int argc,
                    char **argv)
{
    cds_full_name_t  clr_name;
    int              cds_status = CDS_SUCCESS;
    char             **cargv;
    int              cargc;
    int              i;

    dcp_ArgvInfo arg_table[] = { 
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    DCP_CHECK_CDS_NO_ARG(clearinghouse);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    do_sec_login();

    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK)
        return TCL_ERROR;

    for (i = 0; i < cargc; i++) {
        DCP_CHECK_CDS_NAME_LEN(cargv[i]);
        strcpy((char *)clr_name.uc, cargv[i]);
        cds_status = clrnghouse_command(clr_name, CLEARCLE);
        DCP_CHECK_CDS_ERROR(cds_status);
    }
    free((char *)cargv);
    return TCL_OK;
}


/*
 *  FUNCTION: clringhouse_show()
 *
 *  OVERVIEW: This function shows the attributes in a named clearinghouse.
 *
 *  INPUTS:
 *
 *  int  argc - Number of input arguments (argv)
 *  char **argv - value of all input arguments.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                       will hold the error message (if any) generated from the
 *                       command. This is also the location of any convienience
 *                       variables.
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
clringhouse_show(ClientData clientData,
                 Tcl_Interp *interp,
                 int argc,
                 char **argv)
{
    cds_full_name_t     clr_name;
    int                 cds_status = CDS_SUCCESS;
    char                *temp;
    extern Tcl_Interp   *attr_interp;
    int                 all = 0;
    int                 attribute = 0;
    int                 counter = 0;
    char                **cargv;
    int                 cargc;
    int                 schema = 0;
    int                 action = ShowAttribute;
    int                 i;
    cds_attribute_list  *list;

    dcp_ArgvInfo arg_table[] = {
        {"-all", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_show_clearinghouse_counters_attr_help},
        {"-attributes", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_show_clearinghouse_attr_help},
        {"-counters", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_show_clearinghouse_counters_help},
        {"-schema", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_schema_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&all;
    arg_table[1].dst = (char *)&attribute;
    arg_table[2].dst = (char *)&counter;
    arg_table[3].dst = (char *)&schema;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
  
    if (schema)
        action = ShowAttrType;

    DCP_CHECK_CDS_NO_ARG(clearinghouse);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if ((all) && ((counter) || (attribute) || (schema))) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    if ((schema) && ((counter) || (attribute))) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    if (all)
        counter = attribute = 1;
    else {
        if (!counter && !attribute)
            attribute = 1;
    }

    do_sec_login();

    /* 
     * Reset the temp_attr_value since it may have been used previously
     * in a call to this function.  If this call returns a TCL_ERROR that 
     * indicates a first call.
     */
    Tcl_UnsetVar(attr_interp, "temp_attr_value", 0); 

    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK)
        return TCL_ERROR;

    for (i = 0; i < cargc; i++) {
        DCP_CHECK_CDS_NAME_LEN(cargv[i]);
        strcpy((char *)clr_name.uc, cargv[i]);
        if (attribute || all) {
            if (enum_all_attr(interp, &clr_name, NULL,
                              cdsCH, action, &list) != TCL_OK) {
                free((char *)cargv);
                return TCL_ERROR;
            }

            if (format_all_attr(interp, list) != TCL_OK) {
                free((char *)cargv);
                free_attr_value(&list);
                return TCL_ERROR;
            }
            free_attr_value(&list);
        }

        if (counter || all) {
            cds_status = clearinghouse_status(clr_name, SHOWSRVCLE);
            DCP_CHECK_CDS_ERROR(cds_status);
        }
    }

    if (counter || all) {
        temp = Tcl_GetVar(attr_interp, "clear_counters", 0);
        if (attribute || all)
            Tcl_AppendResult(interp,"\n",NULL);
        if (temp)
            Tcl_AppendResult(interp,temp,NULL);
    }
    free((char *)cargv);
    return TCL_OK;
}


/*
 * FUNCTION:  clringhouse_initiate()
 * 
 * OVERVIEW: This function initiates some action on the named clearinghouse.
 *
 * INPUTS:
 *  int  argc - Number of input arguments (argv)
 *  char **argv - value of all input arguments.
 *
 * INPUT/OUTPUT:
 *  Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                       will hold the error message (if any) generated from the
 *                       command. This is also the location of any convienience
 *                       variables.
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *
 */
int
clringhouse_initiate(ClientData clientData,
                     Tcl_Interp *interp,
                     int argc,
                     char **argv)
{
    cds_full_name_t  clr_name;
    int              cds_status = CDS_SUCCESS;
    char             **cargv;
    int              cargc;
    int              i;
    boolean32        checkpoint = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-checkpoint", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_initiate_clringhouse_ckpt_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&checkpoint;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
  
    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    DCP_CHECK_CDS_NO_ARG(clearinghouse);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);
 
    if (!checkpoint) {
        DCP_SET_MISC_ERROR(dcp_s_cds_no_checkpoint);
    }

    do_sec_login();

    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK)
        return TCL_ERROR;
    for (i = 0; i < cargc; i++) {
        DCP_CHECK_CDS_NAME_LEN(cargv[i]);
        strcpy((char *)clr_name.uc, cargv[i]);
        cds_status = clrnghouse_command(clr_name, FORCECHECKCMD);
        DCP_CHECK_CDS_ERROR(cds_status);
    }
    free((char *)cargv);
    return TCL_OK;
}


/*
 * FUNCTION: clringhouse_verify()
 * 
 * OVERVIEW: This function starts the background thread on the named
 *           clearinghouse to run.
 *
 * INPUTS:
 *  int  argc - Number of input arguments (argv)
 *  char **argv - value of all input arguments.
 *
 * INPUT/OUTPUT:
 *  Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                       will hold the error message (if any) generated from the
 *                       command. This is also the location of any convienience
 *                       variables.
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *
 */
int
clringhouse_verify(ClientData clientData,
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    cds_full_name_t  clr_name;
    int              cds_status = CDS_SUCCESS;
    char             **cargv;
    int              cargc;
    int              i;

    dcp_ArgvInfo arg_table[] = { 
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();    

    DCP_CHECK_CDS_NO_ARG(clearinghouse);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);
 
    do_sec_login();

    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK)
        return TCL_ERROR;
    for (i = 0; i < cargc; i++) {
        DCP_CHECK_CDS_NAME_LEN(cargv[i]);
        strcpy((char *)clr_name.uc, cargv[i]);
        cds_status = clrnghouse_command(clr_name, FORCEBACKCMD);
        DCP_CHECK_CDS_ERROR(cds_status);
    }
    free((char *)cargv);
    return TCL_OK;
}


/*
 * FUNCTION: clringhouse_repair()
 *
 * OVERVIEW: This function analyzes and repairs certain inconsistencies
 *           in the specified clearinghouse.  Currently, it repairs:
 *
 *           -timestamp: Bad timestamps (in the future) in the clearinghouse.
 *                       This command disables the clearinghouse, analyzes
 *                       and repairs the bad timestamps, checkpoints the
 *                       clearinghouse, and enables the clearinghouse.
 *                       This is a serious command and may take serious time.
 *
 * INPUTS:
 *  int  argc - Number of input arguments (argv)
 *  char **argv - value of all input arguments.
 *
 * INPUT/OUTPUT:
 *  Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                       will hold the error message (if any) generated from the
 *                       command. This is also the location of any convienience
 *                       variables.
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *
 */
int
clringhouse_repair(ClientData clientData,
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    cds_full_name_t  clr_name;
    int              cds_status = CDS_SUCCESS;
    char             **cargv;
    int              cargc;
    int              i = 0;
    boolean32        timestamps = FALSE;
 
    dcp_ArgvInfo arg_table[] = {
        {"-timestamps", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_repair_clringhouse_ts_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&timestamps;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
  
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    DCP_CHECK_CDS_NO_ARG(clearinghouse);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);
 
    if (!timestamps) {
        DCP_SET_MISC_ERROR(dcp_s_cds_no_timestamps);
    }

    do_sec_login();

    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK)
        return TCL_ERROR;

    for (i = 0; i < cargc; i++) {
        int repair_status;
        int error = 0;
        int loop = 1;

        DCP_CHECK_CDS_NAME_LEN(cargv[i]);
        strcpy((char *)clr_name.uc, cargv[i]);
        cds_status = clrnghouse_command(clr_name, REPAIRTIMECMD);
        DCP_CHECK_CDS_ERROR(cds_status);

        /*
         * Continually get repair status until complete or error.
         */
        while (loop) {
            repair_status = clrnghouse_command(clr_name,REPAIRSTATUSCMD);
            switch (repair_status) {
                case CDS_TSREP_OFF:
                case CDS_TSREP_DISABLE:
                case CDS_TSREP_REPAIR:
                case CDS_TSREP_CHECKPT:
                case CDS_TSREP_ENABLE:
                    break;
                case CDS_TSREP_DONE:
                    loop = 0;
                    break;
                default:
                    /*
                     * Some error occurred.
                     */
                    loop = 0;
                    error = 1;
                    break;
            }
        }
        if (error) {
            DCP_CHECK_CDS_ERROR(repair_status);
        }
    }
    free((char *)cargv);
    return TCL_OK;
}


/*
 * FUNCTION: get_default_handle()
 *
 * OVERVIEW: Aquire a default handle for the cell 
 * 
 * INPUTS:
 *    int clerk_server_id : 0 - DNScpClerk_v1_0_c_ifspec
 *                          1 - DNScpServer_v1_0_c_ifspec
 *
 * OUTPUTS:
 *
 * INPUT/OUTPUT:
 *
 * RETURNS:
 *
 */
handle_t
get_default_handle(int clerk_server_id,
                   unsigned32 *status)
{
    rpc_binding_handle_t  handle;
    unsigned_char_t       *string_binding;
    rpc_protseq_vector_t  *protseq_vector = NULL;
    int                   i;
    rpc_if_handle_t       sec_ifspec;

    /*
     * Get a vector of the supported protocols.
     */
    rpc_network_inq_protseqs(&protseq_vector, status);
    if ((protseq_vector == NULL) || (*status != rpc_s_ok))
        return NULL;

    /*
     * Cycle through all of the available protocol sequences until 
     * one successfully binds. 
     */
    for (i = 0; i < protseq_vector->count; ++i) {
        rpc_string_binding_compose(NULL,
                                   protseq_vector->protseq[i],
                                   NULL,
                                   NULL,
                                   NULL,  /* no network options */
                                   &string_binding,
                                   (unsigned32 *)status);
        if (*status == rpc_s_ok) {
            rpc_binding_from_string_binding(string_binding, &handle, status);
            if (*status == rpc_s_ok) {
                rpc_string_free(&string_binding, status);

                /*
                 * Endpoint must be fully bound before setting authentication.
                 */
                sec_ifspec = (clerk_server_id == 0) ?
                    DNScpClerk_v1_0_c_ifspec : DNScpServer_v1_0_c_ifspec;

                rpc_ep_resolve_binding(handle,sec_ifspec,status);
                rpc_binding_set_auth_info(handle,
                                          NULL,
                                          rpc_c_authn_level_default,
                                          rpc_c_authn_dce_secret,
                                          (rpc_auth_identity_handle_t)login_context_h,
                                          rpc_c_authz_dce,
                                          status);
                if (*status != rpc_s_ok)
                    continue;
                return((rpc_binding_handle_t )handle);
            }
        }
    }

    /*
     * All the protocols in our table have failed to result in a binding.
     */
    return (NULL);
}


/*
 * FUNCTION: clrnghouse_command()
 *
 * OVERVIEW: This function performs a variety of clearinghouse
 *  functions, depending on the 'command' passed in.
 *
 * INPUTS:
 *
 * OUTPUTS:
 *
 * INPUT/OUTPUT:
 *
 * RETURNS:
 */
int
clrnghouse_command(cds_full_name_t clr_name,
                   unsigned32 command)
{
    cds_FullName_t        replica_name;
    clh_info_t            clh_info; 
    clh_info_list_t       clh_info_list;
    clh_info_list_p_t     clh_info_list_p;
    cds_FullName_t        clh_name;
    rpc_binding_handle_t  rpc_binding = NULL;
    unsigned32            rpc_status = rpc_s_ok;
    unsigned32            euid;
    command_status_t      command_status = CDS_SUCCESS;
    extern Tcl_Interp     *attr_interp; 
    char                  *parent_name;
    char                  interp_cmd[1024];

    /* 
     * Zero out the relevant data structures. 
     */
    memset((char *)&clh_info, 0, sizeof(clh_info));
    memset((char *)&clh_name, 0, sizeof(clh_name));
    memset((char *)&replica_name, 0, sizeof(replica_name));

    /* 
     * Set appropiate pointers.
     */
    clh_info.replica_type = RT_readOnly;
    clh_info.clh_name_p = &clh_name;
    clh_info.replica_name_p = &replica_name;
    clh_info.attr_list_p = NULL;

    /*
     * Pick off the replica name, which will be used as the cell base. 
     */
    sprintf(interp_cmd, "file dirname %s", clr_name.uc);
    Tcl_Eval(attr_interp, interp_cmd);

    /*
     * Expand the parent name.
     */
    parent_name = expand_cell_name((char *)attr_interp->result, &rpc_status);
    if (rpc_status != rpc_s_ok) {
        return rpc_status;
    }
    if (!parent_name) {
        return dcp_s_no_memory;
    }
    strcpy((char *)clh_info.replica_name_p->fn_name, parent_name);
    clh_info.replica_name_p->fn_length = strlen((char *)clh_info.replica_name_p->fn_name);
    free(parent_name);

    /*
     * Expand the clearinghouse name.
     */
    parent_name = expand_cell_name((char *)clr_name.uc, &rpc_status);
    if (rpc_status != rpc_s_ok) {
        return rpc_status;
    }
    if (!parent_name) {
        return dcp_s_no_memory;
    }
    strcpy((char *)clh_info.clh_name_p->fn_name, parent_name);
    free(parent_name);
    clh_info.clh_name_p->fn_length = strlen((char *)clh_info.clh_name_p->fn_name);

    /*
     * Store the '/...' uuid as the root uuid for the clearinghouse 
     * information block passed to the server.
     */
    memcpy(&clh_info.clh_name_p->fn_root, cdsWildRoot,
	   sizeof(clh_info.clh_name_p->fn_root));
    memcpy(&clh_info.replica_name_p->fn_root, &clh_info.clh_name_p->fn_root,
	   sizeof(clh_info.clh_name_p->fn_root));

    /*  
     * Get authenticated binding handle if we don't already have one.
     */
    if (rpc_binding == NULL) {
	rpc_binding = get_default_handle (1, &rpc_status);
	if (rpc_status != rpc_s_ok)
	    return rpc_status;
    }

    /*
     * Set up the list of clearinghouses. 
     */
    clh_info_list.clh_count = 1;
    clh_info_list.clh_members[0] = &clh_info;
    clh_info_list_p = &clh_info_list;

    euid = (long)geteuid();

    /*
     * Make the clearinghouse call to the server.
     */
    rpc_status = dnscp_server(rpc_binding,
                              command,
                              euid,
                              NULL,
                              &clh_info_list_p,
                              &command_status);

    /* 
     * Check for command and rpc transmission success 
     */
    if (command_status != CDS_SUCCESS)
        return command_status;

    if (rpc_status != rpc_s_ok)
        return rpc_status;

    return CDS_SUCCESS;
}


/*
 * FUNCTION: clearinghouse_status()
 * 
 * INPUTS:
 *
 * OUTPUTS:
 *
 * INPUT/OUTPUT:
 *
 * RETURNS:
 */
int
clearinghouse_status(cds_full_name_t clr_name,
                     unsigned32 command)
{
    clh_info_t            clh_info; 
    clh_info_list_t       clh_info_list;
    clh_info_list_p_t     clh_info_list_p;
    cds_FullName_t        clh_name;
    rpc_binding_handle_t  rpc_binding = NULL;
    unsigned32            rpc_status = rpc_s_ok;
    unsigned32            euid;
    command_status_t      command_status = CDS_SUCCESS;
    char                  *parent_name;
    unsigned32            status = error_status_ok;
    cprpc_attr_list_t     *attr_list;

    /* 
     * Zero out the relevant data structures. 
     */
    memset((char *)&clh_info, 0, sizeof(clh_info));
    memset((char *)&clh_name, 0, sizeof(clh_name));

    /* 
     * Set appropiate pointers.
     */
    clh_info.replica_type = RT_readOnly;
    clh_info.clh_name_p = &clh_name;
    clh_info.attr_list_p = NULL;

    /*
     * Set up the list of clearinghouses. 
     */
    clh_info_list.clh_count = 1;
    clh_info_list.clh_members[0] = &clh_info;
    clh_info_list_p = &clh_info_list;

    /*
     * Expand the clearinghouse name.
     */
    parent_name = expand_cell_name((char *)clr_name.uc, &rpc_status);
    if (rpc_status != rpc_s_ok)
        return rpc_status;
    if (!parent_name)
        return dcp_s_no_memory;
    strcpy((char *)clh_info.clh_name_p->fn_name, parent_name);
    free(parent_name);

    clh_info.clh_name_p->fn_length = strlen((char *)clh_info.clh_name_p->fn_name);

    /*
     * Store the '/...' uuid as the root uuid for the clearinghouse 
     * information block passed to the server.
     */
    memcpy(&clh_info.clh_name_p->fn_root, cdsWildRoot,
           sizeof(clh_info.clh_name_p->fn_root));

    /*  
     * Get authenticated binding handle if we don't already have one
     */
    if (rpc_binding == NULL) {
        rpc_binding = get_clearinghouse_handle(1,
                                               clh_info.clh_name_p,
                                               NULL,
                                               &rpc_status);
        if (rpc_status != rpc_s_ok)
            return rpc_status;
    }
    euid = (long)geteuid();

    /*
     * Make the clearinghouse call to the server.
     */
    rpc_status = dnscp_server(rpc_binding,
                              command,		    
                              euid,
                              NULL,
                              &clh_info_list_p,
                              &command_status);

    /*
     * Check for command and rpc transmission success.
     */
    if (command_status != CDS_SUCCESS)
        return command_status;

    if (rpc_status != rpc_s_ok)
        return rpc_status;

    if (command == SHOWSRVCLE) {
        attr_list = clh_info_list_p->clh_members[0]->attr_list_p;
        status = show_clearinghouse_counters(attr_list);
        if (status != error_status_ok)
            return status;
    }

    return CDS_SUCCESS;
}


/*
 * FUNCTION: get_clearinghouse_handle()
 * 
 * INPUTS:
 *    int server_id : 0 - DNScpClerk_v1_0_c_ifspec
 *                    1 - DNScpServer_v1_0_c_ifspec
 *                    2 - cprpc_hcell_v1_0_c_ifspec
 *
 * OUTPUTS:
 *
 * INPUT/OUTPUT:
 *
 * RETURNS:
 *
 */
handle_t
get_clearinghouse_handle(int server_id,
                         cds_FullName_t *ch_fullname,
                         uuid_t *chid_p,
                         error_status_t *status)
{
    rpc_binding_handle_t  handle;
    int                   i;
    cds_Set_t             *towerset = NULL;
    unsigned_char_t       **string_bindings;
    rpc_if_handle_t       sec_ifspec; 

    /*
     * Get the towerset for this clearinghouse.
     */
    get_towerset (ch_fullname, &towerset, status);
    if (*status != CDS_SUCCESS)
        return (NULL);

    /*
     * Convert the towerset to an array of string bindings and
     * free the towerset.
     */
    *status = cdsCvtTowerToStr((char *)towerset, (char **)&string_bindings);
    free(towerset);
    if (*status != CDS_SUCCESS)
        return (NULL);

    /*
     * Loop through our table of string bindings until we get one that works.
     */
    for (i = 0; string_bindings[i] != NULL; ++i) {
        rpc_binding_from_string_binding(string_bindings[i], &handle, status);
        if (*status == rpc_s_ok) {
            /*
             * Endpoint must be fully bound before setting authentication.
             */
            switch (server_id) {
                case 0:
                    sec_ifspec = DNScpClerk_v1_0_c_ifspec;
                    break;

                case 1:
                    sec_ifspec = DNScpServer_v1_0_c_ifspec;
                    break;

                case 2:
                    sec_ifspec = cprpc_hcell_v1_0_c_ifspec;
                    if (chid_p) {
                        rpc_binding_set_object(handle, chid_p, status);
                        if (*status != rpc_s_ok) {
                            free (string_bindings);
                            return (NULL);
                        }
                    }
                    break;
            }
	    
            rpc_ep_resolve_binding(handle, sec_ifspec, status);
            rpc_binding_set_auth_info(handle,
                                      NULL,
                                      rpc_c_authn_level_default,
                                      rpc_c_authn_dce_secret,
                                      (rpc_auth_identity_handle_t)login_context_h,
                                      rpc_c_authz_dce,
                                      status);
            if (*status != rpc_s_ok)
                continue;
            free(string_bindings);
            return ((handle_t) handle);
        }
    }

    /*
     * We only get here if we have failed to get a binding handle.
     */
    free(string_bindings);
    return (NULL);
}


/*
 * FUNCTION: get_towerset()
 * 
 * INPUTS:
 * cds_FullName_t *ch_fullname_p; 
 *
 * OUTPUTS:
 * cds_Set_t **towerset_pp   - contains a structure which describes the 
 *                             clearinghouse tower. 
 * error_status_t  *status_p - describtes the error condition, if any.
 *
 */
void
get_towerset(cds_FullName_t *ch_fullname_p,
             cds_Set_t **towerset_pp, 
             error_status_t *status_p)
{
    cds_handle_t        handle;
    cds_full_name_t     full_name;
    int                 read_status, attr_index, attr_num, total_attr_bytes;
    struct cdsFlagStat  flags;
    cds_attrvalue_t     *attrval_p;
    
    /*
     * Arbitrarily large array of attribute values in the special
     * form of MemberHeader and AtomicValue.
     */
    MemberValue_t *MemberValues[16];
    
#define MAX_ATTR sizeof(MemberValues)/sizeof(MemberValue_t *)

    strcpy ((char *)full_name.uc, (char *)ch_fullname_p->fn_name);

    /*
     * Sanity check our towerset_pp input, we always return something.
     */
    if (!towerset_pp)
    {
        *status_p = CDS_ERROR;
        return;
    }

    /*
     * Our defaults.
     */
    *towerset_pp = NULL;
    *status_p = CDS_SUCCESS;
    clear_flags(&flags, cdsStrDCE);
    ZERO_bytes (MemberValues, sizeof(MemberValues));

    /*
     * Don't process a null fullname.
     */
    if (!ch_fullname_p)
        return;

    /*
     * Get a handle for reading attributes.
     */
    handle = cdsGetHandle();
    if (handle == NULL_CDS_HANDLE) {
        *status_p = CDS_ERROR;
        return;
    }

    /*
     * Read all the CDS_CHLastAddress attributes into an array of
     * dns attribute values.
     */
    for (attr_index = attr_num = total_attr_bytes = 0,
         read_status = CDS_SUCCESS;
         (read_status == CDS_SUCCESS) &&
         (*status_p == CDS_SUCCESS) &&
         (attr_index < MAX_ATTR);
         attr_index++) {
        /*
         * Read an attribute value.
         */
        read_status = cdsReadAttrValue(&full_name,
                                       handle,
                                       (cds_attr_name_t *)"CDS_CHLastAddress\0",
                                       cdsCH,
                                       NULL,
                                       &attrval_p,
                                       NULL,
                                       NULL,
                                       NULL,
                                       &flags);

        /*
         * Only process if successful read.
         */
        if (read_status == CDS_SUCCESS) {
            int need_bytes;
            int attr_bytes;

            /*
             * We only know about byte values.
             */
            if (attrval_p->val_syntax != cds_byte) {
                *status_p = CDS_ERROR;
                continue;
            }

            /*
             * Allocate some space.
             */
            attr_bytes = attrval_p->val_u.val_byte.length;
            need_bytes = sizeof(MemberValue_overhead) + attr_bytes;
            MemberValues[attr_index] = (MemberValue_t *)malloc(need_bytes);

            if (MemberValues[attr_index] == NULL) {
                *status_p = CDS_NONSRESOURCES;
                continue;
            }
            ZERO_bytes(MemberValues[attr_index], need_bytes);

            /*
             * Make a copy of this attribute and bump the total
             * number of attribute bytes that we have accumulated
             * and the number of attributes that we have read
             * successfully.
             */
            SET_FLAG(MemberValues[attr_index]->mv_flag, MV_present);
            INS16(MemberValues[attr_index]->mv_length, attr_bytes);
            INS8(MemberValues[attr_index]->mv_valuetype, cds_byte);
            COPY_bytes(attrval_p->val_u.val_byte.byte_p,
                       MemberValues[attr_index]->mv_value,
                       attr_bytes);
            total_attr_bytes += attr_bytes;
            attr_num++;
        }
    }

    /*
     * Check that we successfully read attributes, if we didn't,
     * then return a failure.
     */
    if (attr_num == 0) {
        if (*status_p == CDS_SUCCESS) {
            *status_p = read_status == CDS_SUCCESS ? CDS_ERROR : read_status;
        }
        return;
    }

    /*
     * Now that we have accumulated all the attribute values, build
     * a towerset of all the tower members.
     */
    if (*status_p == CDS_SUCCESS) {
        Set_t  *towerset_p;
        byte_t *towerset_datap;
        byte_t *towerset_offsetp;
        int     towerset_offset;
        int     need_bytes, copy_bytes, value_bytes;

        /*
         * Allocate enough space for the towerset
         */
        need_bytes = sizeof(Set_overhead) + (sizeof(word_u) * attr_num) +
                            (sizeof (MemberValue_overhead) * attr_num) +
                            total_attr_bytes;
        towerset_p = (Set_t *)malloc(need_bytes);
        if (towerset_p == NULL)
        {
            *status_p = CDS_NONSRESOURCES;
            goto cleanup;
        }

        /*
         * Initialize some towerset overhead.
         */
        SET_FLAG(towerset_p->st_flag, ST_present);
        INS16(towerset_p->st_offsetLength, attr_num * sizeof(word_u));
        value_bytes = attr_num * sizeof(MemberValue_overhead) + total_attr_bytes;
        INS16(towerset_p->st_valueLength, value_bytes);

        /*
         * Now add each of the individual attributes to the towerset.
         */
        towerset_datap = DATA_Set((Set_u *)towerset_p);
        towerset_offset = towerset_datap - (byte_t *)towerset_p;
        for (attr_index = 0; attr_index < attr_num; attr_index++) {
            towerset_offsetp = OFFSET_Set((Set_u *)towerset_p, attr_index);
            INS16(towerset_offsetp, towerset_offset);
            copy_bytes = sizeof(MemberValue_overhead) +
                EXT16(MemberValues[attr_index]->mv_length);
            COPY_bytes(MemberValues[attr_index],
                       towerset_datap,
                       copy_bytes);
            towerset_datap  += copy_bytes;
            towerset_offset += copy_bytes;
        }

        /*
         * Return a pointer to the towerset to our caller.
         */
        *towerset_pp = (cds_Set_t *)towerset_p;
    }

    /*
     * Cleanup by freeing the 'read' handle and release any memory
     * we allocated for our MemberValue array.
     */
cleanup:
    cdsFreeHandle(handle);
    for (attr_index = 0; attr_index < attr_num; attr_index++)
    {
        free(MemberValues[attr_index]);
    }
}


/*
 * FUNCTION: show_clearinghouse_counters()
 *
 * OVERVIEW:
 *  Prints to a buffer the statistics of a particular clearinghouse. 
 *
 * INPUTS:
 *  cprpc_attr_list_t *attr_list - contains a structure describe various aspects
 *                                 of the named clearinghouse. 
 *
 * OUTPUTS:
 *  Fills the interpreter with information from the attribute list. 
 *
 */
int 
show_clearinghouse_counters(cprpc_attr_list_t *attr_list)
{
    extern Tcl_Interp  *attr_interp;
    int                attr_num;
    int                i;
    cprpc_attr_t       *attr_p;
    int                attr_id;
    char               buffer[64];
    char               *reads;
    char               *reads_svc;
    char               *writes;
    char               *writes_svc;
    char               *partials;
    char               *partials_svc;
    char               *unreach;
    char               *unreach_svc;
    char               *corrupt;
    char               *corrupt_svc;
    char               *enable;
    char               *enable_svc;
    char               *disable;
    char               *disable_svc;
    char               *upfail;
    char               *upfail_svc;
    char               *skfail;
    char               *skfail_svc;
    char               *missing;
    char               *missing_svc;

    Tcl_UnsetVar(attr_interp, "clear_counters", 0);
    attr_num = attr_list->attr_len;

    for (i = 0; i < attr_num; i++) {
        attr_p  = attr_list->attr_members[i];
        attr_id = attr_list->attr_members[i]->attr_id;
        translate_counter64(attr_p, buffer);
        switch (attr_id) {
            case CH_READ_ACCESS_COUNT:
                reads_svc = (char *)dce_sprintf(dcp_t_cds_readaccess);
                reads = malloc(strlen(reads_svc) + strlen(buffer) +10);
                sprintf(reads, "{%s %s}", reads_svc, buffer);
                free(reads_svc);
                break;
            case CH_WRITE_ACCESS_COUNT:
                writes_svc = (char *)dce_sprintf(dcp_t_cds_writeaccess);
                writes = malloc(strlen(writes_svc) + strlen(buffer) +10);
                sprintf(writes, "{%s %s}", writes_svc, buffer);
                free(writes_svc);
                break;
            case CH_REFERENCES_RETURNED_COUNT:
                partials_svc = (char *)dce_sprintf(dcp_t_cds_clr_references);
                partials = malloc(strlen(partials_svc) + strlen(buffer) +10);
                sprintf(partials , "{%s %s}", partials_svc, buffer);
                free(partials_svc);
                break;
            case CH_ROOT_NOT_REACHABLE_COUNT:
                unreach_svc = (char *)dce_sprintf(dcp_t_cds_clr_unreachable);
                unreach = malloc(strlen(unreach_svc) + strlen(buffer) +10);
                sprintf(unreach, "{%s %s}", unreach_svc, buffer);
                free(unreach_svc);
                break;
            case CH_DATA_CORRUPTION_COUNT:
                corrupt_svc = (char *)dce_sprintf(dcp_t_cds_clr_data_corr);
                corrupt = malloc(strlen(corrupt_svc) + strlen(buffer) +10);
                sprintf(corrupt, "{%s %s}", corrupt_svc, buffer);
                free(corrupt_svc);
                break;
            case CH_SKULK_FAILURES_COUNT:
                skfail_svc = (char *)dce_sprintf(dcp_t_cds_clr_skulk_fail);
                skfail = malloc(strlen(skfail_svc) + strlen(buffer) +10);
                sprintf(skfail, "{%s %s}", skfail_svc, buffer);
                free(skfail_svc);
                break;
            case CH_ENTRY_MISSING_COUNT:
                missing_svc = (char *)dce_sprintf(dcp_t_cds_clr_entry_missing);
                missing = malloc(strlen(missing_svc) + strlen(buffer) +10);
                sprintf(missing , "{%s %s}", missing_svc, buffer);
                free(missing_svc);
                break;
            case CH_UPGRADES_FAILED_COUNT:
                upfail_svc = (char *)dce_sprintf(dcp_t_cds_clr_upgrade_fail);
                upfail = malloc(strlen(upfail_svc) + strlen(buffer) +10);
                sprintf(upfail, "{%s %s}", upfail_svc, buffer);
                free(upfail_svc);
                break;
            case CH_ENABLE_COUNT:
                enable_svc = (char *)dce_sprintf(dcp_t_cds_clr_enable);
                enable = malloc(strlen(enable_svc) + strlen(buffer) +10);
                sprintf(enable, "{%s %s}", enable_svc, buffer);
                free(enable_svc);
                break;
            case CH_DISABLE_COUNT:
                disable_svc = (char *)dce_sprintf(dcp_t_cds_clr_disable);
                disable = malloc(strlen(disable_svc) + strlen(buffer) +10);
                sprintf(disable, "{%s %s}", disable_svc, buffer);
                free(disable_svc);
                break;
            default:
                break;
        }
    }
    APPEND_CH_COUNTER(corrupt);
    APPEND_CH_COUNTER(disable);
    APPEND_CH_COUNTER(enable);
    APPEND_CH_COUNTER(upfail);
    APPEND_CH_COUNTER(missing);
    APPEND_CH_COUNTER(reads);
    APPEND_CH_COUNTER(partials);
    APPEND_CH_COUNTER(unreach);
    APPEND_CH_COUNTER(skfail);
    APPEND_CH_COUNTER(writes);
    return error_status_ok;
}


/*
 * FUNCTION: translate_counter64()
 * 
 * INPUTS:
 *  char *buffer - contains an ascii integer
 *
 * INPUT/OUTPUT:
 *  cprpc_attr_t *attr - contains both the result and the description of the 
 *                       integer.
 *
 */
void
translate_counter64(cprpc_attr_t *attr,
                    char *buffer)
{
#if LONG_BIT >= 64
     sprintf(buffer, "%d", attr->value.av_val.av_counter64);
#else
    if (attr->value.av_val.av_counter64.high == 0)
        sprintf(buffer, "%d", attr->value.av_val.av_counter64.low);
    else
        sprintf(buffer, "0x%0lx%0lx",
                attr->value.av_val.av_counter64.high,
                attr->value.av_val.av_counter64.low);
#endif
}
