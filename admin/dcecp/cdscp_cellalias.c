/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdscp_cellalias.c,v $
 * Revision 1.1.8.1  1996/08/09  11:45:26  arvind
 * 	Merge changes.
 * 	[1996/06/10  14:46 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge changes for 'set' code removal.
 * 	[1996/03/18  15:37 UTC  truitt  /main/HPDCE02/6]
 *
 * 	Merge.
 * 	[1996/03/18  15:36 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	Small code cleanup.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/10  14:24 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.6.3  1996/02/18  19:12:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:34  marty]
 * 
 * Revision 1.1.6.2  1995/12/08  21:19:51  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/15  17:39 UTC  truitt
 * 	Merge message cleanup work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts29/1  1995/05/15  17:38 UTC  truitt
 * 	CHFts15237: No parameter should be passed when
 * 	the too_many_args error condition is set.
 * 
 * 	HP revision /main/HPDCE02/4  1995/05/01  18:51 UTC  truitt
 * 	Merge changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  18:50 UTC  truitt
 * 	CHFts15144: Code cleanup.  Also, make sure the correct
 * 	cds error status is returned.
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/14  15:53 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  15:52 UTC  truitt
 * 	CHFts14285: Combine all cds check args macros into one,
 * 	since all six are the same anyway.  Also, make sure correct
 * 	macros are called when processing an error condition.
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/05  17:51 UTC  truitt
 * 	OT12710: Remove useless macro code.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_01/1  1994/12/05  17:50 UTC  truitt
 * 	OT12710: Remove useless macro code.
 * 
 * 	HP revision /main/HPDCE02/1  1994/10/20  17:42 UTC  truitt
 * 
 * 	HP revision /main/truitt_era_dcecp/1  1994/10/20  17:30 UTC  truitt
 * 	Fix include line for cdscp_hcell include file.
 * 	[1995/12/08  17:54:33  root]
 * 
 * Revision 1.1.2.9  1994/09/30  17:26:17  kevins
 * 	OT 12377 Give correct error message when a list is entered.
 * 	[1994/09/30  17:26:04  kevins]
 * 
 * Revision 1.1.2.8  1994/09/30  13:57:47  zee
 * 	OT 12345: In cdsalias_connect, do not accept any argument and supply
 * 	dummy name onward to rpc call.
 * 	[1994/09/29  23:32:20  zee]
 * 
 * Revision 1.1.2.7  1994/09/28  11:54:38  kevins
 * 	OT 12377 modified list processing.
 * 	[1994/09/28  11:54:30  kevins]
 * 
 * Revision 1.1.2.6  1994/09/26  20:23:21  kevins
 * 	OT 12334 added cdsalias command
 * 	[1994/09/26  20:18:00  kevins]
 * 
 * Revision 1.1.2.5  1994/09/16  14:50:11  kevins
 * 	OT 12220 moddified cdsalias commands.
 * 	[1994/09/16  14:47:59  kevins]
 * 
 * Revision 1.1.2.4  1994/09/13  19:13:58  kevins
 * 	OT 12160 modified cdsalias catalog argument checking
 * 	[1994/09/13  18:50:37  kevins]
 * 
 * Revision 1.1.2.3  1994/09/02  21:03:16  zee
 * 	In cdsalias_command, add uuid argument to get_clearinghouse_handle call
 * 	since setting the uuid in the rpc_binding belongs there.
 * 	[1994/09/02  18:40:25  zee]
 * 
 * Revision 1.1.2.2  1994/08/01  15:07:16  zee
 * 	     Rename cellalias to cdsalias.
 * 	     In cdsalias catalog, add /.: if no name is specified.
 * 	[1994/07/29  20:58:48  zee]
 * 
 * Revision 1.1.2.1  1994/07/12  19:34:43  zee
 * 	     Initial version.
 * 	[1994/07/12  19:34:31  zee]
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
#include <dce/dnsmessage.h>
#include <dce/attributes.h>
#include <dce/dnsclerk.h>
#include <dce/rpc_cvt.h>
#include <dce/cprpc.h>
#include <dce/cprpcp.h>
#include <cprpc_hcell.h>
#include <dce/dnsmanpar.h>
#include <dce/server_ncl_defs.h>
#include <dcecp.h>
#include <util_general.h>
#include <util_var.h>
#include <util_help.h>

/* PROTOTYPES */
int cdsalias_command (cds_full_name_t, int);


/*
 *  FUNCTION: cdsalias_create
 *
 *   The function creates a cdsalias as named. 
 *
 * INPUTS:
 *   int  argc - Number of input arguments (argv)
 *   char **argv - value of all input arguments.
 *
 * INPUT/OUTPUT:
 *   Tcl_Interp *interp -  Interpreter structure for variables and return 
 *                       messages.
 * 
 * RETURNS: 
 *   int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 * 
 */
int cdsalias_create(ClientData clientData,
                    Tcl_Interp *interp,
                    int argc,
                    char **argv)
{
    cds_full_name_t  alias_name;
    int              cds_status = CDS_SUCCESS;
    char             *temp_cdsalias;
    int              cargc; 
    char             **cargv;

    /*
     * Check to see if the user just wants help.
     */
    DCP_NO_COMMAND_HELP();

    DCP_CHECK_CDS_NO_ARG(cdsalias) 
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);
 
    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    free((char *)cargv);

    if (cargc > 1) {
        DCP_SET_MISC_ERROR(dcp_s_bad_arg_list);
    }

    /* 
     * Set the login context appropriately 
     */
    do_sec_login();

    /*
     * Expand the pathname in case it contains '/.:'.
     */
    temp_cdsalias = expand_cell_name(argv[1], (error_status_t *)&cds_status);
    DCP_CHECK_RPC_ERROR(cds_status);

    strcpy((char *)alias_name.uc, temp_cdsalias);
    free(temp_cdsalias);

    cds_status = cdsalias_command(alias_name, ADDCELLALIASCMD);
    DCP_CHECK_CDS_ERROR(cds_status);

    return TCL_OK;
}


/*
 * FUNCTION: cdsalias_delete
 *
 *   This function deletes the named cdsalias.  You may only remove
 *   one cdsalias at a time.  Then you'll need to skulk the root
 *   directory.
 *
 * INPUTS:
 *   int  argc - Number of input arguments (argv)
 *   char **argv - value of all input arguments.
 *
 * INPUT/OUTPUT:
 *   Tcl_Interp *interp -  Interpreter structure for variables and return 
 *                       messages.
 * 
 * RETURNS: 
 *   int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 */
int cdsalias_delete(ClientData clientData,
                    Tcl_Interp *interp,
                    int argc,
                    char **argv)
{
    cds_full_name_t  alias_name;
    int              cds_status = CDS_SUCCESS;
    char             *temp_cdsalias;
    int              cargc; 
    char             **cargv;

    /*
     * Check to see if the user just wants help.
     */
    DCP_NO_COMMAND_HELP();
 
    DCP_CHECK_CDS_NO_ARG(cdsalias) 
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    free((char *)cargv);

    if (cargc > 1) {
        DCP_SET_MISC_ERROR(dcp_s_bad_arg_list);
    }

    /* 
     * Set the login context appropriately 
     */
    do_sec_login();

    /*
     * Expand the pathname in case it contains '/.:'.
     */
    temp_cdsalias = expand_cell_name(argv[1], (error_status_t *)&cds_status);
    DCP_CHECK_RPC_ERROR(cds_status);

    strcpy((char *)alias_name.uc, temp_cdsalias);
    free(temp_cdsalias); 

    cds_status = cdsalias_command(alias_name, REMCELLCMD);
    DCP_CHECK_CDS_ERROR(cds_status);

    return TCL_OK;
}


/*
 * FUNCTION: cdsalias_connect
 *
 * OVERVIEW: Establish a CDS hierarchical relationship between two cells.
 * 
 *
 * INPUTS:
 *  int argc - number of arguments in argv;
 *  char **argv - arguments
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - -  Interpreter structure for variables and return 
 *                       messages.
 * RETURNS:
 *   int, either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 */
int cdsalias_connect(ClientData clientData,
                     Tcl_Interp *interp,
                     int argc,
                     char **argv)
{
    cds_full_name_t  alias_name;
    int              cds_status = CDS_SUCCESS;
    char             *temp_cdsalias;
    cds_full_name_t  root_name = {"/.:"};

    dcp_ArgvInfo arg_table[] = {
        {NULL,DCP_ARGV_END,NULL,NULL,0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    if (argc > 1) {
        DCP_SET_RESULT_CODE(dcp_s_too_many_args);
        return TCL_ERROR;
    }
    argv[1] = (char *)&root_name;

    /* 
     * Set the login context appropriately 
     */
    do_sec_login();

    /*
     * Expand the pathname in case it contains '/.:'.
     */
    temp_cdsalias = expand_cell_name(argv[1], (error_status_t *)&cds_status);
    DCP_CHECK_RPC_ERROR(cds_status);

    strcpy((char *)alias_name.uc, temp_cdsalias);
    free(temp_cdsalias);    

    cds_status = cdsalias_command(alias_name, ADDCELLCHILDCMD);
    DCP_CHECK_CDS_ERROR(cds_status);

    return TCL_OK;
}


/*
 *  FUNCTION: cdsalias_catalog
 *
 *   This function shows a list of all of the defined cell aliases.
 *
 * INPUTS:
 *   int  argc - Number of input arguments (argv)
 *   char **argv - value of all input arguments.
 *
 * INPUT/OUTPUT:
 *   Tcl_Interp *interp -  Interpreter structure for variables and return 
 *                       messages.
 *
 * OUTPUTS:
 *   None
 *
 * RETURNS:
 *   int, either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 * 
 */
int cdsalias_catalog(ClientData clientData,
                     Tcl_Interp *interp,
                     int argc,
                     char **argv)
{
    cds_full_name_t     alias_name;
    extern Tcl_Interp   *attr_interp;
    cds_attribute_list  *list = NULL;
    char                *temp_cdsalias;
    int                 cds_status = CDS_SUCCESS;
    int                 cargc; 
    char                **cargv;

    /*
     * Check to see if the user just wants help.
     */
    DCP_NO_COMMAND_HELP();

    if (argc > 1) {
        DCP_CHECK_EXTRA_ARGS();
        DCP_GENERAL_SET_NAMELIST(argv[1]);
        if (Tcl_SplitList(interp,argv[1], &cargc, &cargv) == TCL_ERROR) {
            return TCL_ERROR;
        }
        free((char *)cargv);

        if (cargc > 1) {
            DCP_SET_MISC_ERROR(dcp_s_bad_arg_list);
        }

        temp_cdsalias = expand_cell_name(argv[1], (error_status_t *)&cds_status);
        DCP_CHECK_RPC_ERROR(cds_status);

        strcpy((char *)alias_name.uc, temp_cdsalias);
        free(temp_cdsalias);    
    }
    else {
        temp_cdsalias = expand_cell_name("/.:", (error_status_t *)&cds_status);
        DCP_CHECK_RPC_ERROR(cds_status);

        strcpy((char *)alias_name.uc, temp_cdsalias);
        free(temp_cdsalias);    
    }

    /* 
     * Set the login context appropriately 
     */
    do_sec_login();

    /* 
     * Reset the temp_attr_value since it may have been used previously
     * in a call to this function. If this call returns a TCL_ERROR that 
     * indicates a first call. 
     */
    Tcl_UnsetVar(attr_interp, "temp_attr_value", 0); 

    if (enum_set_attr(interp, &alias_name, NULL, "CDS_CellAliases",
                      cdsDir, &list) != TCL_OK)
        return TCL_ERROR;

    if (format_all_attr(interp, list) != TCL_OK) {
        free_attr_value(&list);
        return TCL_ERROR;
    }
    free_attr_value(&list);

    return TCL_OK;
}


/*
 *  FUNCTION: cdsalias_command
 *
 *   This function makes the cdsalias RPC call to the clearinghouse.
 *
 * INPUTS:
 *   cds_full_name_t    ca_name - clearinghouse name
 *   int                type - which cdsalias command
 *
 * INPUT/OUTPUT:
 *
 * OUTPUTS:
 *   None
 *
 * RETURNS:
 *
 */
int cdsalias_command (cds_full_name_t ca_name,
                      int type)
{
    cds_handle_t        hp;
    struct cdsFlagStat  flags;
    cds_attrvalue_t     *bufptr;
    ReplicaPointer_t    *rp_p;
    FullName_t          *chname_p;
    error_status_t      rpc_status;
    command_status_t    command_status;
    cds_FullName_t      alias_name;
    cds_FullName_t      master_chname;
    uuid_t              master_chid;
    unsigned char       opq_cellname[CDS_OPQ_FNAME_MAX];
    int                 fullnamesize = CDS_OPQ_FNAME_MAX;
    dns_status_t        status = CDS_SUCCESS;
    handle_t            rpc_binding = NULL;

    memset((char *)&alias_name,0,sizeof(alias_name));
    memset((char *)&master_chname,0,sizeof(master_chname));

    /*
     * Get internal representation of local root.
     */
    status = cdsCvtFullToOpq((unsigned char *)"/.:",
                             (dns_opq_fname *) opq_cellname,
                             &fullnamesize);
    if (status != CDS_SUCCESS) {
	return status;
    }

    hp = cdsGetHandle();
    if (hp == NULL_CDS_HANDLE)
        return dcp_s_cds_no_handle;

    /*
     * Initialize flags - use high confidence.
     */
    clear_flags(&flags, cdsOpqDNA);
    flags.fsConf = cdsConfHi;

    /*
     * Now loop to read all replica pointers and get master replica.
     */
    while (status == CDS_SUCCESS) {
        /*
         * Make the call to retrieve the attribute values.
         */
        status = cdsReadAttrValue((dns_opq_fname *)opq_cellname,
                                  hp,
                                  (cds_attr_name_t *)dns_AttributeName(DNS_REPLICAS),
                                  cdsDir,
                                  (dns_opq_fname *)NULL,
                                  &bufptr,
                                  (dns_cts **)NULL,
                                  (unsigned char *)NULL,
                                  (int *)NULL,
                                  &flags);
        if ((status != CDS_SUCCESS) || (bufptr->val_syntax != cds_ReplicaPointer)) {
            cdsFreeHandle(hp);
            return flags.fsLocStat;
        }

        rp_p = (ReplicaPointer_t *)bufptr->val_u.val_byte.byte_p;
        if (EXT8(FLD_ReplicaPointer_replicaType(rp_p)) != RT_master) {
            continue;
        }

        /*
         * Align and convert clearinghouse uuid.
         */
        memcpy (&master_chid, &rp_p->rp_CHID, sizeof(master_chid));

        /*
         * Convert clearinghouse name for exporting via RPC.
         */
        chname_p = FLD_ReplicaPointer_CHName(rp_p);
        COPY_ObjUID(chname_p->fn_root, &master_chname.fn_root);
        master_chname.fn_length = CDS_STR_FNAME_MAX;
        status = cdsCvtFullToStr((cds_full_name_t *)chname_p,
                                 (unsigned char *)&master_chname.fn_name,
                                 (int *)&master_chname.fn_length);
        if (status != CDS_SUCCESS) {
            return status;
        }
        break;
    }
    cdsFreeHandle(hp);

    if ((status == CDS_EMPTY) || (status == CDS_SUCCESS_STALE)) {
        return status;
    }

    /*
     * Convert alias name for exporting via RPC.
     */
    COPY_ObjUID(&chname_p->fn_root, &alias_name.fn_root);
    strcpy((char *)alias_name.fn_name, (char *)ca_name.uc);
    alias_name.fn_length = strlen((char *)ca_name.uc);

    /*
     * Get authenticated binding handle
     */
    if (rpc_binding == NULL) {
        rpc_binding = get_clearinghouse_handle(2, 
                                               &master_chname,
                                               &master_chid,
                                               &rpc_status);
        if (rpc_status != rpc_s_ok) {
            return rpc_status;
        }
    }

    switch (type) {
        case ADDCELLALIASCMD:
        case ADDCELLCHILDCMD:
            rpc_status = cp_AddCellname(rpc_binding,
                                        &alias_name,
                                        type,
                                        &master_chid,
                                        &command_status);
            break;

        case REMCELLCMD:
            rpc_status = cp_RemoveCellname(rpc_binding,
                                           &alias_name,
                                           &master_chid,
                                           &command_status);
            break;
        default:
            break;
    }
    if (rpc_status != rpc_s_ok) {
        return rpc_status;
    }

    if (command_status != CDS_SUCCESS) {
        return command_status;
    }

    return status;
}
