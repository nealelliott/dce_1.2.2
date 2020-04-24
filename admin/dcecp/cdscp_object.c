/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdscp_object.c,v $
 * Revision 1.1.8.3  1996/02/18  19:12:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:42  marty]
 *
 * Revision 1.1.8.2  1995/12/08  21:20:02  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/01  18:52 UTC  truitt
 * 	Merge changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  18:51 UTC  truitt
 * 	CHFts15085: Fix specific memory leaks in code when
 * 	monitor is run using dcecp.
 * 
 * 	HP revision /main/HPDCE02/5  1995/04/06  14:08 UTC  jrr
 * 	Fix unchecked strcpy() of user input into fixed size buffer (CHFts14888).
 * 	[1995/04/05  21:33 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14888/1]
 * 
 * 	HP revision /main/HPDCE02/4  1995/04/06  13:55 UTC  jrr
 * 	Fix various memory leaks (CHFts14834).
 * 	[1995/04/05  14:31 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14834/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/24  15:44 UTC  truitt
 * 	Merge message work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts15/1  1995/02/24  15:43 UTC  truitt
 * 	CHFts14284: Fix bad error message in object create.
 * 
 * 	HP revision /main/HPDCE02/2  1995/02/14  15:57 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  15:57 UTC  truitt
 * 	CHFts14285: Combine all cds check args macros into one,
 * 	since all six are the same anyway.  Also, make sure correct
 * 	macros are called when processing an error condition.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/05  17:54 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/12/05  17:53 UTC  truitt
 * 	OT12710: Remove useless macro code.
 * 	[1995/12/08  17:54:45  root]
 * 
 * Revision 1.1.4.11  1994/08/21  19:45:22  melman
 * 	fix various compiler warnings
 * 	[1994/08/21  19:29:35  melman]
 * 
 * Revision 1.1.4.10  1994/06/09  16:01:43  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:07  devsrc]
 * 
 * Revision 1.1.4.9  1994/05/20  22:13:35  kevins
 * 	OT 10714 Modified attribute parsing return values.
 * 	[1994/05/20  22:11:23  kevins]
 * 
 * Revision 1.1.4.8  1994/05/11  19:27:04  kevins
 * 	OT 10097 Modified argument checknig routines
 * 	[1994/05/11  19:25:23  kevins]
 * 
 * 	OT 10097 modified arg checking
 * 	[1994/05/11  16:12:19  kevins]
 * 
 * Revision 1.1.4.7  1994/05/03  13:48:58  kevins
 * 	OT# 10311 10360 10407 10414 10500 10499 10408
 * 	Remove unnecessary cds include files.
 * 	Update modify switches to work correctly according to the RFC.
 * 	Fix inconsistent error messages.
 * 	Display multivalued attributes correctly.
 * 	Set the attribute types appropriately.
 * 	Correct the display format to correctly display tcl lists.
 * 	Object create now accepts multiple attributes.
 * 	[1994/05/03  12:57:22  kevins]
 * 
 * Revision 1.1.4.6  1994/03/14  17:39:40  kevins
 * 	CR9904 Added support for help system
 * 	[1994/03/14  13:44:23  kevins]
 * 
 * Revision 1.1.4.5  1994/03/10  15:50:23  kevins
 * 	CR 10030 removed extra newlines
 * 	[1994/03/10  15:42:55  kevins]
 * 
 * Revision 1.1.4.4  1994/02/03  18:42:17  kevins
 * 	updated to conform to func spec
 * 	[1994/02/03  18:29:41  kevins]
 * 
 * Revision 1.1.4.3  1993/12/27  15:05:49  kevins
 * 	Remove dce_error function call
 * 	[1993/12/27  15:03:30  kevins]
 * 
 * Revision 1.1.4.2  1993/12/22  18:12:33  kevins
 * 	Message catalog cleanup
 * 	[1993/12/22  18:08:24  kevins]
 * 
 * 	Message catalog cleanup
 * 
 * Revision 1.1.4.1  1993/12/20  20:06:08  kevins
 * 	Fixed to conform to style guide.
 * 	[1993/12/20  18:51:43  kevins]
 * 
 * Revision 1.1.2.1  1993/10/21  19:39:34  kevins
 * 	completed the name change from dsh to dcecp
 * 	[1993/10/21  19:38:52  kevins]
 * 
 * Revision 1.1.2.8  1993/10/13  19:01:39  kevins
 * 	Added modify commands
 * 	[1993/10/13  19:01:09  kevins]
 * 
 * Revision 1.1.2.7  1993/09/15  13:48:34  kevins
 * 	corrected problems with show routine
 * 	[1993/09/15  13:48:07  kevins]
 * 
 * 	Alter command return values
 * 	[1993/08/03  16:42:17  kevins]
 * 
 * Revision 1.1.2.6  1993/08/25  20:23:43  kevins
 * 	Altered method for reading attributes
 * 	[1993/08/25  20:21:57  kevins]
 * 
 * Revision 1.1.2.5  1993/08/23  14:27:40  kevins
 * 	Added attribute encoding/decoding information
 * 	[1993/08/23  14:16:22  kevins]
 * 
 * Revision 1.1.2.4  1993/08/03  17:08:05  kevins
 * 	Alter command return values
 * 	[1993/08/03  17:01:03  kevins]
 * 
 * Revision 1.1.2.3  1993/07/26  15:27:27  kevins
 * 	removed tk.h and replaced with dsh.h
 * 	[1993/07/26  15:19:07  kevins]
 * 
 * Revision 1.1.2.2  1993/07/23  20:02:19  kevins
 * 	Initial submission of dsh
 * 	[1993/07/23  19:35:04  kevins]
 * 
 * $EndLog$
 */
/* INCLUDES */
#include <stdio.h>
#include <locale.h>
#include <setjmp.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>                     /* for UINT_MAX */

/*
 * Don't inherit stubbase.h's use of the exception based pthreas API.
 */
#ifndef NCK
#  define NCK
#endif
#include <dce/rpc.h>
#include <dce/stubbase.h>

#include <stdlib.h>
#include <string.h>
#include <dce/dce_error.h>

#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dcecp.h>
#include <dce/tcl.h>
#include <dce/cdsclerk.h>
#include <dce/sec_login.h>
#include <cdscp.h>
#include <dce/dce.h>
#include <dce/dce_svc.h>
#include <dce/dnsmessage.h>
#include <util_var.h>
#include <util_general.h>
#include <util_help.h>


/*
 * FUNCTION: object_create 
 *
 * OVERVIEW: This function creates an object in the cds namespace
 * 
 *
 * INPUTS:
 *
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
 */
int object_create(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    int                 cargc;
    cds_full_name_t     obj_name;
    char                **cargv;
    struct cdsFlagStat  flags;
    int                 obj_number;
    cds_id_t            *obj_uuid;
    int                 cds_status = CDS_SUCCESS;
    cds_attribute_list  *list = NULL;
    int                 j,i;
    int                 single = 0;
    char                *attribute = NULL;
    char                *objuuid_svc;
    char                *command_format = "lindex [set [attr_name %s]] %d";
    char                *command;
    cds_attr_name_t     attr_name;
    extern Tcl_Interp   *attr_interp;
    cdsEntries_t        type = cdsObj;
    cdsAttrType_t       attr_type = cdsSet;
    Attribute_Action_t  action = AddAttribute;

    /*
     *  Set up the parse table, initialize the parser variables and 
     *  cll the parser. 
     */
    dcp_ArgvInfo arg_table[] = {
        {"-attribute", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_cds_obj_create_wattr_help},
        {"-single", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_obj_single_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&attribute;
    arg_table[1].dst = (char *)&single;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    DCP_CHECK_CDS_NO_ARG(obj);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (single && !attribute) {
        DCP_SET_MISC_ERROR(dcp_s_cds_single_no_attr);
    }
    else if (single)
        attr_type = cdsSingle;

    do_sec_login();

    /* 
     * If any attributes were named parse them. Make a determination if the 
     * attribute specified was an objuuid. If so this particular attribute 
     * is passed to the cdsCreateObj call. All others are added after the 
     * object has been created. 
     */
    obj_uuid = NULL;
    if (attribute) {
        list = (cds_attribute_list *)malloc(sizeof(cds_attribute_list));
        if (!list) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        memset((char *)list, 0, sizeof(cds_attribute_list));
        if (cds_parse_attributes(interp, attribute, action, type, attr_type, list) != TCL_OK) {
            free((cds_attribute_list *)list);
            return TCL_ERROR;
        }

        /*
         * Find out if one of the attributes added is an objuuid. If so 
         * add it to the list of entries passed into the cds on the 
         * cdspi. This is ugly. 
         */
        for (i = 0; i < list->cur_value; i++) {
            if (list->value[i]->attribute[0]->val_syntax == cds_uuid) {
                objuuid_svc = (char *)dce_sprintf(dcp_t_cds_objectuuid);
                command = malloc(strlen(command_format) + strlen(objuuid_svc) + 20);
                sprintf(command, command_format, objuuid_svc, OidName);
                Tcl_Eval(attr_interp,command_format);
                if (strcmp(list->value[i]->attr_name, attr_interp->result)) {
                    obj_uuid = &(list->value[i]->attribute[0]->val_u.val_uuid);
                    list->value[i]->action = IgnoreAttribute;
                }
                free(objuuid_svc);
                free(command);
            }
        }
    }

    /*
     * Cycle through all objects that are specified to be created.
     */
    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK) {
        if (list != NULL)
            free_attr_value(&list);
        return TCL_ERROR;
    }
    for (obj_number = 0; obj_number < cargc; obj_number++) {
        DCP_CHECK_CDS_NAME_LEN(cargv[obj_number]);
        strcpy((char *)obj_name.uc, (char *)cargv[obj_number]);
        clear_flags(&flags,cdsStrDCE);
        cds_status = CDS_SUCCESS;
        cds_status = cdsCreateObj(&obj_name,
                                  NULL,
                                  NULL,
                                  obj_uuid,
                                  NULL,
                                  &flags);
        if (cds_status != CDS_SUCCESS) {
            free((char *)cargv);
            if (list != NULL)
                free_attr_value(&list);
            DCP_SET_MISC_ERROR(flags.fsLocStat);
        }
        for (i = 0; attribute && i < list->cur_value ; i++) {
            for (j = 0; 
                 j <  list->value[i]->cur_attr && 
                 list->value[i]->action != IgnoreAttribute;
                 j++) {
                strcpy((char *)attr_name.uc, list->value[i]->attr_name);
                cds_status = cdsAddAttrValue(&obj_name,
                                             &attr_name,
                                             list->value[i]->attr_type,
                                             list->value[i]->obj_type,
                                             list->value[i]->attribute[j],
                                             NULL,
                                             &flags);
                if (cds_status != CDS_SUCCESS) {
                    free((char *)cargv);
                    if (list != NULL)
                        free_attr_value(&list);
                    DCP_SET_MISC_ERROR(flags.fsLocStat);
                }
            }
        }
    }
    free((char *)cargv);
    if (list != NULL)
        free_attr_value(&list);
    return TCL_OK;
}


/*
 * FUNCTION: object_delete
 *
 * OVERVIEW: This function deletes the named object 
 * 
 *
 * INPUTS:
 *
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
 */
int object_delete(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    struct cdsFlagStat  flags;
    cds_full_name_t     obj_name;
    int                 obj_number = 1;
    int                 cds_status = CDS_SUCCESS;
    char                **dargv;
    int                 dargc;

    /*
     * Check to see if the user just wants help.
     */
    DCP_NO_COMMAND_HELP();

    DCP_CHECK_CDS_NO_ARG(obj);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    do_sec_login();

    /*
     * Cycle through all objects that are specified to be deleted.
     */
    if (Tcl_SplitList(interp, argv[1], &dargc, &dargv) != TCL_OK)
        return TCL_ERROR;
    for (obj_number = 0; obj_number < dargc; obj_number++) {
        DCP_CHECK_CDS_NAME_LEN(dargv[obj_number]);
        strcpy((char *)obj_name.uc, (char *)dargv[obj_number]);
        clear_flags(&flags, cdsStrDCE);
        cds_status = CDS_SUCCESS;
        cds_status = cdsDelObj(&obj_name, &flags);
        if (cds_status != CDS_SUCCESS) {
            free((char *)dargv);
            DCP_SET_MISC_ERROR(flags.fsLocStat);
        }
    }
    free((char *)dargv);
    return TCL_OK;
}


/*
 * FUNCTION: object_modify
 *
 * OVERVIEW: This function deletes the named object 
 * 
 *
 * INPUTS:
 *
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
 */
int object_modify(ClientData clientData, 
                  Tcl_Interp *interp, 
                  int argc, 
                  char **argv)
{
    unsigned32          result = error_status_ok;
    char                *add = NULL;
    char                *remove = NULL;
    char                *change = NULL;
    cds_full_name_t     obj_name;
    cds_attr_name_t     attr_name;
    int                 margc;
    char                **margv;
    int                 order[ModifyTypes];
    int                 obj_number;
    int                 single = 0;
    int                 types = 0;
    cdsEntries_t        type = cdsObj;
    cds_attribute_list  *list = NULL;

    dcp_ArgvInfo arg_table[] = {
        {"-add", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_cds_obj_mod_add_help},
        {"-change", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_cds_obj_mod_change_help}, 
        {"-remove", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_cds_obj_mod_rem_help},
        {"-single", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_obj_single_help},    
        {"-types", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_mod_types_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /* 
     * Set up parser variables and initialize parser table. 
     * Terminate the command if the appropriate parameters 
     * haven't been made. 
     */
    arg_table[0].dst = (char *)&add;
    arg_table[1].dst = (char *)&change;
    arg_table[2].dst = (char *)&remove;
    arg_table[3].dst = (char *)&single;
    arg_table[4].dst = (char *)&types;

    determine_modify_order(order, argc, argv);  
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    DCP_CHECK_CDS_NO_ARG(obj);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (!add && !remove && !change) {
        DCP_SET_MISC_ERROR(dcp_s_cds_no_obj_mod_operation);
    }
    if (single && !add) {
        DCP_SET_MISC_ERROR(dcp_s_cds_single_no_add);
    }
    if (types && !remove) {
        DCP_SET_MISC_ERROR(dcp_s_cds_types_no_remove);
    }

    /* 
     * Setup the modification parameters. If the user wants a single 
     * valued attribute to be added the single switch should have be 
     * set. If the user wants an entry in a multi-valued attribute to   
     * be removed the value switch should have be set. 
     */
    list = (cds_attribute_list *)malloc(sizeof(cds_attribute_list));
    if (!list) {
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    memset((char *)list,0,sizeof(cds_attribute_list));
    result = generate_attribute_list(interp,
                                     add,
                                     remove,
                                     change,
                                     type,
                                     list,
                                     single,
                                     types,
                                     order);
    if (result != TCL_OK) {
        free((cds_attribute_list *)list);
        return TCL_ERROR;
    }

    do_sec_login();

    /* 
     * Split the entry list and cycle through each entry in 
     * the list. 
     */
    if (Tcl_SplitList(interp, argv[1], &margc, &margv) != TCL_OK) {
        free_attr_value(&list);
        return TCL_ERROR;
    }

    for (obj_number = 0; obj_number < margc; obj_number++) {
        DCP_CHECK_CDS_NAME_LEN(margv[obj_number]);
        strcpy((char *)obj_name.uc, margv[obj_number]);
        result = modify_cds_attribute(interp, &obj_name, &attr_name, list);
        if (result != TCL_OK) {
            free_attr_value(&list);
            free((char *)margv);
            return result;
        }
    }
    free_attr_value(&list);
    free((char *)margv);
    return TCL_OK;
}


/*
 * FUNCTION: object_show
 *
 * OVERVIEW: This function deletes the named object
 * 
 *
 * INPUTS:
 *
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
 */
int object_show(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    cds_full_name_t     obj_name;
    int                 sargc = 0;
    char                **sargv;
    int                 schema = 0;
    int                 action = ShowAttribute;
    int                 obj_number = 0; 
    cds_attribute_list  *list = NULL;

    dcp_ArgvInfo  arg_table[] = {
        {"-schema", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_schema_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&schema;
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    if (schema)
        action = ShowAttrType;    

    DCP_CHECK_CDS_NO_ARG(obj);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    do_sec_login();

    if (Tcl_SplitList(interp, argv[1], &sargc, &sargv) == TCL_ERROR)
        return TCL_ERROR;
    for (; obj_number < sargc; obj_number++) {
        DCP_CHECK_CDS_NAME_LEN(sargv[obj_number]);
        strcpy((char *)obj_name.uc, sargv[obj_number]);
        if (enum_all_attr(interp,
                          &obj_name, 
                          NULL,
                          cdsObj,
                          action,
                          &list) != TCL_OK) {
            free((char *)sargv);
            return TCL_ERROR;
        }

        if (format_all_attr(interp, list) != TCL_OK) {
            free((char *)sargv);
            free_attr_value(&list);
            return TCL_ERROR;
        }
        free_attr_value(&list);
    }
    free((char *)sargv);
    return TCL_OK;
}
