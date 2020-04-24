/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdscp_link.c,v $
 * Revision 1.1.12.1  1996/08/09  11:45:53  arvind
 * 	Merge changes.
 * 	[1996/06/10  17:58 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/18  17:37 UTC  truitt  /main/HPDCE02/5]
 *
 * 	Merge.
 * 	[1996/03/18  17:36 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/10  14:27 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.10.3  1996/02/18  19:12:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:40  marty]
 * 
 * Revision 1.1.10.2  1995/12/08  21:19:57  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/04/06  14:08 UTC  jrr
 * 	Check CDS names for valid size on input (CHFts14888).
 * 	[1995/04/05  21:33 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14888/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/14  15:56 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  15:55 UTC  truitt
 * 	CHFts14285: Combine all cds check args macros into one,
 * 	since all six are the same anyway.  Also, make sure correct
 * 	macros are called when processing an error condition.
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/05  17:52 UTC  truitt
 * 	OT12710: Remove useless macro code.
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/03  20:12 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/11/03  20:11 UTC  truitt
 * 	Fix link command switches.
 * 	[1995/12/08  17:54:41  root]
 * 
 * Revision 1.1.4.13  1994/08/26  14:24:54  kevins
 * 	OT 11904 11869 -linkout tp -to and initialize list structure
 * 	[1994/08/26  14:08:00  kevins]
 * 
 * Revision 1.1.4.12  1994/06/09  16:01:42  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:05  devsrc]
 * 
 * Revision 1.1.4.11  1994/05/20  22:13:33  kevins
 * 	OT 10714 Modified attribute parsing return values.
 * 	[1994/05/20  22:11:21  kevins]
 * 
 * Revision 1.1.4.10  1994/05/11  19:27:06  kevins
 * 	OT 10097 Modified argument checknig routines
 * 	[1994/05/11  19:25:25  kevins]
 * 
 * 	OT 10097 modified arg checking
 * 	[1994/05/11  16:12:18  kevins]
 * 
 * Revision 1.1.4.9  1994/05/03  13:48:57  kevins
 * 	OT# 10311  10407 10500 10499 10408
 * 	Remove unnecessary cds include files.
 * 	Update modify switches to work correctly according to the RFC.
 * 	Display multivalued attributes correctly.
 * 	Set the attribute types appropriately.
 * 	Correct the display format to correctly display tcl lists.
 * 	[1994/05/03  12:56:16  kevins]
 * 
 * Revision 1.1.4.8  1994/04/07  10:22:11  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:17:54  kevins]
 * 
 * 	CR 10304 Modifications to help remove cross component dependency build issues
 * 	[1994/04/07  10:10:36  kevins]
 * 
 * 	Removed unnecessary options.h include
 * 
 * Revision 1.1.4.7  1994/03/24  10:21:26  kevins
 * 	CR 10081 modified -timeout on link create
 * 	[1994/03/24  10:21:12  kevins]
 * 
 * Revision 1.1.4.6  1994/03/14  17:39:38  kevins
 * 	CR9904 Added support for help system
 * 	[1994/03/14  13:44:22  kevins]
 * 
 * Revision 1.1.4.5  1994/03/10  15:50:21  kevins
 * 	CR 10030 removed extra newlines
 * 	[1994/03/10  15:42:15  kevins]
 * 
 * Revision 1.1.4.4  1994/02/03  18:42:16  kevins
 * 	updated to conform to func spec
 * 	[1994/02/03  18:29:40  kevins]
 * 
 * Revision 1.1.4.3  1993/12/27  15:05:48  kevins
 * 	Remove dce_error function call
 * 	[1993/12/27  15:03:31  kevins]
 * 
 * Revision 1.1.4.2  1993/12/22  18:12:32  kevins
 * 	Message catalog cleanup
 * 	[1993/12/22  18:08:23  kevins]
 * 
 * 	Message catalog cleanup
 * 
 * Revision 1.1.4.1  1993/12/20  20:05:58  kevins
 * 	Fixed to conform to style guide.
 * 	[1993/12/20  18:51:39  kevins]
 * 
 * 	 updated message catalogs
 * 	[1993/12/13  18:41:13  kevins]
 * 
 * 	Alter command return values
 * 	[1993/08/03  16:42:08  kevins]
 * 
 * Revision 1.1.2.1  1993/10/21  19:39:32  kevins
 * 	completed the name change from dsh to dcecp
 * 	[1993/10/21  19:38:51  kevins]
 * 
 * Revision 1.1.2.8  1993/10/13  19:01:37  kevins
 * 	Added modify commands.
 * 	[1993/10/13  18:57:55  kevins]
 * 
 * Revision 1.1.2.7  1993/09/15  13:48:32  kevins
 * 	corrected problems with show routine
 * 	[1993/09/15  13:48:05  kevins]
 * 
 * Revision 1.1.2.6  1993/08/25  20:23:35  kevins
 * 	Altered method for reading attributes
 * 	[1993/08/25  20:21:45  kevins]
 * 
 * Revision 1.1.2.5  1993/08/23  14:27:37  kevins
 * 	Added attribute encoding/decoding information
 * 	[1993/08/23  14:16:19  kevins]
 * 
 * Revision 1.1.2.4  1993/08/03  17:08:01  kevins
 * 	Alter command return values
 * 	[1993/08/03  17:00:53  kevins]
 * 
 * Revision 1.1.2.3  1993/07/26  15:27:24  kevins
 * 	removed tk.h and replaced with dsh.h
 * 	[1993/07/26  15:19:01  kevins]
 * 
 * Revision 1.1.2.2  1993/07/23  20:02:11  kevins
 * 	Initial submission of dsh
 * 	[1993/07/23  19:35:00  kevins]
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
 * Don't inherit stubbase.h's use of the exception based pthreads API.
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
#include <util_var.h>
#include <util_general.h>
#include <util_help.h>


/***************************************************************
 *
 *  link_create()
 *   
 * Creates a new soft link. There is one required switch 
 * -to which takes as its argument the name that the 
 * link is to point to. There is also a -timeout switch which
 * can be used to specify the CDS_LinkTimeout attribute. 
 * Without this switch the soft link is permanent.
 *
 *  Inputs
 * 
 *  int  argc - Number of input arguments (argv)
 *  char **argv - value of all input arguments.
 *
 *
 * Input/Output
 *
 * Tcl_Interp *interp -  Interpreter structure for variables and return 
 *                       messages.
 * 
 * Output 
 * returns an error status code. 
 * 
 */

int
    link_create(ClientData clientData, 
                Tcl_Interp *interp, 
                int argc, 
                char **argv)
{
    struct cdsFlagStat  flags;
    extern Tcl_Interp   *attr_interp;
    cds_full_name_t     link_name;
    cds_full_name_t     target;
    int                 cds_status = CDS_SUCCESS;
    char                *linkto_value;
    boolean32           linkto = FALSE;
    static char         *timeout;
    char                *attribute = NULL;
    cdsAttrType_t       attr_type = cdsSet;
    cdsEntries_t        type = cdsSLink;
    unsigned32          result = error_status_ok;
    Attribute_Action_t  action = AddAttribute;
    char                *linktarget_svc;
    char                *command_format = "lindex [set [attr_name %s]] %d";
    char                *command;
    cds_attr_name_t     attribute_name;
    cds_attribute_list  *list;
    int                 j, k, i;
    int                 cargc;
    char                **cargv;
    char                *temp;

  
    dcp_ArgvInfo arg_table[] = {
	{"-attribute",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_cds_create_link_attr_help},
	{"-timeout",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_cds_create_link_timeout_help},
	{"-to",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_cds_create_link_to_help},
	{NULL,DCP_ARGV_END,NULL,NULL,0} 
    };
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    
    linkto_value = NULL;
    timeout = NULL;

    arg_table[0].dst = (char *)&attribute;
    arg_table[1].dst = (char *)&timeout;
    arg_table[2].dst = (char *)&linkto_value;

    if(dcp_ParseArgv(interp,&argc,argv,arg_table,0) != TCL_OK)
	return TCL_ERROR;
    
    DCP_CHECK_CDS_NO_ARG(link);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if(linkto_value) {
	linkto = TRUE;
    }    
    if(attribute && (linkto_value || timeout)){
        DCP_SET_MISC_ERROR(dcp_s_rgy_list_and_switches);
    }
    target.uc[0] = '\0';

    do_sec_login();
    
    if(attribute || timeout) {
        list = (cds_attribute_list *)malloc(sizeof(cds_attribute_list));
        memset((char *)list,0,sizeof(cds_attribute_list));
    }
    if (attribute){
        if(cds_parse_attributes( interp, attribute, action, type,
                                      attr_type, list) != TCL_OK)
            return TCL_ERROR;
        for(i = 0; i < list->cur_value; i++){
            if(list->value[i]->attribute[0]->val_syntax == cds_FullName){
                linktarget_svc = (char *)dce_sprintf(dcp_t_cds_linktarget);
                command = malloc(strlen(command_format) + 
                                 strlen(linktarget_svc) + 20);
                sprintf(command,command_format,linktarget_svc,OidName);
                Tcl_Eval(attr_interp,command_format);
                if(strcmp(list->value[i]->attr_name,attr_interp->result)){
                    F_fullname(list->value[i]->attribute[0],&temp);
                    strcpy((char *)target.uc,temp);
                    free(temp);
                    list->value[i]->action = IgnoreAttribute;
                }
                free(linktarget_svc);
                free(command);
            } /* end if attribute == Fullname */
        }
    } 
    if (timeout){
        result = parse_timeout(timeout,list);
        if (result != error_status_ok)
            DCP_SET_MISC_ERROR(result);
    }
    /*  
     *   check to see if a linktarget was found in the attribute list or on the command
     *   line. 
     */
    if(!strlen((char *)target.uc) && !linkto){
        DCP_SET_MISC_ERROR(dcp_s_cds_no_linkto_value);
    }
        
     /* cycle through all links that are specified to be created. */
    if(Tcl_SplitList(interp,argv[1],&cargc,&cargv) != TCL_OK)
        return TCL_ERROR;
    for(i = 0 ; i < cargc; i++){
        DCP_CHECK_CDS_NAME_LEN(cargv[i]);
        strcpy((char *)link_name.uc,(char *)cargv[i]);
        if(!strlen((char *)target.uc)) {
            DCP_CHECK_CDS_NAME_LEN(linkto_value);
            strcpy((char *)target.uc,(char *)linkto_value);
        }
        clear_flags(&flags,cdsStrDCE);   /* clear/initialize the cds flags. */ 
        cds_status = cdsCreateSLink(&link_name,&target,NULL,
                                    NULL,&flags);
        if (cds_status != CDS_SUCCESS){
            DCP_SET_MISC_ERROR(flags.fsLocStat);
        }
        for(k = 0 ;attribute && k < list->cur_value ; k++){
            if(list->value[k]->action != IgnoreAttribute){
                for(j = 0; j < list->value[k]->cur_attr ; j++){
                    strcpy((char *)attribute_name.uc,list->value[k]->attr_name);
                    cds_status = cdsAddAttrValue(&link_name,&attribute_name,
                                                 list->value[k]->attr_type,
                                                 list->value[k]->obj_type,
                                                 list->value[k]->attribute[j],
                                                 NULL, &flags);
                    if(cds_status != CDS_SUCCESS){
                        DCP_SET_MISC_ERROR(flags.fsLocStat);
                    }
                } /* end for all attribute entries */
            }
        } /* end for all attributes */
    } /* end for all links */
    return TCL_OK;
}



/**********************************************************
 *
 * link_delete()
 * This function deletes the named link 
 *
 *  Inputs
 * 
 *  int  argc - Number of input arguments (argv)
 *  char **argv - value of all input arguments.
 *
 *
 * Input/Output
 *
 * Tcl_Interp *interp -  Interpreter structure for variables and return 
 *                       messages.
 * 
 * Output 
 * returns an error status code. 
 * 
 */


int
    link_delete(ClientData clientData, 
                Tcl_Interp *interp, 
                int argc, 
                char **argv)

{
    struct cdsFlagStat  flags;
    cds_full_name_t     link_name;
    int                 link_number = 0;
    int                 cds_status = CDS_SUCCESS;
    char                **dargv;
    int                 dargc;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();
    DCP_CHECK_CDS_NO_ARG(link);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    do_sec_login();
    /* cycle through all objects that are specified to be deleted. */
    if(Tcl_SplitList(interp,argv[1],&dargc,&dargv) != TCL_OK )
        return TCL_ERROR;
    for(;link_number < dargc; link_number++){
        DCP_CHECK_CDS_NAME_LEN(dargv[link_number]);
        strcpy((char *)link_name.uc,(char *)dargv[link_number]);
        clear_flags(&flags,cdsStrDCE);   /* clear/initialize the cds flags. */ 
        cds_status = CDS_SUCCESS;
        cds_status = cdsDelSLink(&link_name,&flags);
        if (cds_status != CDS_SUCCESS){
            DCP_SET_MISC_ERROR(flags.fsLocStat);
        } /* end if cds_status*/
    } /* end for all dargv */
    free((char *)dargv);
    return TCL_OK;
}


/**********************************************************
 *
 * link_show - This function shows the attributes in a named 
 *              link.
 *
 *  Inputs
 * 
 *  int  argc - Number of input arguments (argv)
 *  char **argv - value of all input arguments.
 *
 *
 * Input/Output
 *
 * Tcl_Interp *interp -  Interpreter structure for variables and return 
 *                       messages.
 * 
 * Output 
 * returns an error status code. 
 * 
 */


int
    link_show(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)

{
    cds_full_name_t     link_name;
    int                 link_number = 0; 
    int                 largc = 0;
    int                 schema;
    int                 action = ShowAttribute;
    char                **largv;
    cds_attribute_list  *list;

    dcp_ArgvInfo  arg_table[] = {
        {"-schema",DCP_ARGV_CONSTANT,(char *)1,NULL,
             dcp_t_cds_schema_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0} 
    };
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    schema = 0;
    arg_table[0].dst = (char *)&schema;
    if(dcp_ParseArgv(interp,&argc,argv,arg_table,0) != TCL_OK)
        return TCL_ERROR;
    if(schema)
        action = ShowAttrType;   
 
    DCP_CHECK_CDS_NO_ARG(link);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    do_sec_login();
    
    if(Tcl_SplitList(interp,argv[1],&largc,&largv) != TCL_OK)
        return TCL_ERROR;
    for(; link_number < largc; link_number++){
        DCP_CHECK_CDS_NAME_LEN(largv[link_number]);
	strcpy((char *)link_name.uc,largv[link_number]);
        if(enum_all_attr(interp, &link_name, 
                         NULL, cdsSLink, action, &list) != TCL_OK)
            return TCL_ERROR;

        if(format_all_attr(interp, list) != TCL_OK){
            free_attr_value(&list);
            return TCL_ERROR;
        }
    }/* for loop */
    free((char *)largv);
    free_attr_value(&list);
    return TCL_OK;
}


/**********************************************************
 *
 * link_modify - This function modifies an attribute in 
 *                the named link. 
 *
 *  Inputs
 * 
 *  int  argc - Number of input arguments (argv)
 *  char **argv - value of all input arguments.
 *
 *
 * Input/Output
 *
 * Tcl_Interp *interp -  Interpreter structure for variables and return 
 *                       messages.
 * 
 * Output 
 * returns an error status code. 
 * 
 */


int
    link_modify(ClientData clientData, 
                Tcl_Interp *interp, 
                int argc, 
                char **argv)
{
    char                *add;
    char                *remove;
    char                *change;
    cds_full_name_t     link_name;
    cds_attr_name_t     attr_name;
    int                 result = 0;
    int                 link_number;
    int                 largc;
    char                **largv;
    int                 order[ModifyTypes];
    cdsEntries_t        type = cdsSLink;
    cds_attribute_list  *list;
    
    dcp_ArgvInfo arg_table[] = {
	{"-add",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_cds_mod_link_add_help},
	{"-change",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_cds_mod_link_change_help}, 
	{"-remove",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_cds_mod_link_rem_help},    
	{NULL,DCP_ARGV_END,NULL,NULL,0} 
    };
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    
    remove = NULL;
    add = NULL;
    change = NULL;
    arg_table[0].dst = (char *)&add;
    arg_table[1].dst = (char *)&change;
    arg_table[2].dst = (char *)&remove;
    determine_modify_order(order, argc, argv);
    if(dcp_ParseArgv(interp,&argc,argv,arg_table,0) != TCL_OK)
	return TCL_ERROR;
    
    /* check the input parameters for correct options and switches. */

    DCP_CHECK_CDS_NO_ARG(link);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if((!add && !remove && !change)){
        DCP_GENERAL_UNSET_NAMELIST();
	DCP_SET_MISC_ERROR(dcp_s_cds_no_link_mod_operation);
    }

    /* determine what type of modify operation is in store. */
    list = (cds_attribute_list *)malloc(sizeof(cds_attribute_list));
    memset((char *)list,0,sizeof(cds_attribute_list));
   
    result = generate_attribute_list(interp,
                                     add,
                                     remove,
                                     change,
                                     type,
                                     list,
                                     0,
                                     0,
                                     order);
    if(result != TCL_OK)
        return TCL_ERROR;

    do_sec_login();
    if(Tcl_SplitList(interp,argv[1],&largc,&largv) != TCL_OK)
        return TCL_ERROR;
    for( link_number = 0; link_number < largc; link_number++){
        DCP_CHECK_CDS_NAME_LEN(largv[link_number]);
	strcpy((char *)link_name.uc,largv[link_number]);
        result = modify_cds_attribute(interp, &link_name, &attr_name, list);
        if(result != TCL_OK)
            return result;        
    } /* end for all links */
    return TCL_OK;
}
