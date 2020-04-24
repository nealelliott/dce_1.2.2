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
 * $Log: util_general.c,v $
 * Revision 1.1.6.1  1996/08/09  11:53:05  arvind
 * 	Merge changes.
 * 	[1996/06/12  15:17 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/02/06  17:50 UTC  truitt  /main/HPDCE02/3]
 *
 * 	Merge changes.
 * 	[1996/02/06  17:48 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf60/1]
 *
 * 	CHFts17281: Make sure binding stuff is completely understood.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/12  14:41 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.4.2  1996/02/18  19:15:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:24  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:25:03  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  20:47 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/23  00:50 UTC  tatsu_s
 * 	Added dcp_general_is_bound_locally() and dcp_general_un/bind_local().
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/10  19:06 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/truitt_dcecp_chfts11/1  1995/02/10  19:03 UTC  truitt
 * 	Fix minor control-L problem.
 * 	[1995/12/08  17:58:14  root]
 * 
 * Revision 1.1.2.11  1994/09/29  21:28:38  kevins
 * 	OT 12415 fixed suspect malloc lengths
 * 	[1994/09/29  21:27:59  kevins]
 * 
 * Revision 1.1.2.10  1994/09/22  16:29:54  salamone
 * 	dcp_rgy_sa_list_replicas() returns pointer to local storage.
 * 	[1994/09/22  16:28:44  salamone]
 * 
 * Revision 1.1.2.9  1994/08/21  19:45:32  melman
 * 	fix various compiler warnings
 * 	[1994/08/21  19:29:49  melman]
 * 
 * Revision 1.1.2.8  1994/08/19  22:15:50  rousseau
 * 	Made aud and dts attributes and values abbreviateable (CR 11025).
 * 	[1994/08/19  22:15:02  rousseau]
 * 
 * Revision 1.1.2.7  1994/08/19  18:04:12  salamone
 * 	CR10683 - Provide generic routines to handle 1 sbin in TCL syntax.
 * 	[1994/08/19  17:50:19  salamone]
 * 
 * Revision 1.1.2.6  1994/08/08  18:51:57  melman
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:30  melman]
 * 
 * Revision 1.1.2.5  1994/07/28  20:34:52  rousseau
 * 	64 bit changes (CR 11451).
 * 	[1994/07/28  20:33:28  rousseau]
 * 
 * Revision 1.1.2.4  1994/06/09  16:02:35  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:11  devsrc]
 * 
 * Revision 1.1.2.3  1994/02/15  10:20:42  kevins
 * 	Added util_var.h for prototypes.
 * 	[1994/02/14  15:39:42  kevins]
 * 
 * Revision 1.1.2.2  1994/02/03  17:24:26  kevins
 * 	added a couple of interesting routines
 * 	[1994/02/03  17:18:48  kevins]
 * 
 * Revision 1.1.2.1  1993/12/20  20:08:03  kevins
 * 	New file for general utilities
 * 	[1993/12/20  18:49:18  kevins]
 * 
 * $EndLog$
 */
/* 
 * INCLUDE FILES
 */
#include <stdio.h>
#include <locale.h>
#include <errno.h>
#include <dce/macros.h>
/*
 * Don't inherit stubbase.h's use of the exception based pthrads API.
 */
#ifndef NCK
#  define NCK
#endif
#include <dce/rpc.h>
#include <dce/stubbase.h>

#ifndef SNI_SVR4_POSIX
#include <strings.h>
#endif
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dcedhdmsg.h>
#include <dce/dcedpvt.h>
#include <dce/dce_svc.h>
#include <dcecp.h> 
#include <dce/tcl.h>
#include <util_general.h>
#include <util_binding.h>
#include <util_var.h>


/*
 * FUNCTION: expand_cell_name
 *
 * OVERVIEW:
 * 
 * expand the cell name typically the "/.:" abbrev into a full cell name.
 * This function requires that enough space be allocated in the to_name buffer.
 *
 * INPUTS:
 * char *from_name - Name of cell entry to be expanded.          
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 * char *to_name - expanded cell entry name returned.
 *
 * RETURNS:
 * char * the expanded name of from_name
 * 
 */
char *
expand_cell_name(char *from_name, error_status_t *status)
{
    unsigned_char_t  *buffer;
    char             *to_name;

    *status = rpc_s_ok;
    rpc_ns_entry_expand_name(rpc_c_ns_syntax_default,
                             (unsigned_char_t *)from_name,
                             &buffer,
                             status);

    if (*status != rpc_s_ok)
        return NULL;

    to_name = dce_strdup((char *)buffer);
    rpc_string_free(&buffer,status);

    return (char *)to_name;
}
 

/*
 * FUNCTION: isllist
 *
 * OVERVIEW:
 * 
 * This will return TRUE if the provided argument is a tcl list of 
 * tcl lists. {{a b} {c d} {e f}}
 *
 * INPUTS:
 * char *list - the pointer to a possible list of lists 
 *
 * RETURNS:
 *  int
 *      TRUE - if the argument is a list of lists
 *      FALSE - if not.
 */
int
isllist(char *list)
{
    extern Tcl_Interp  *attr_interp;
    char               *cmd_string = "llength [ lindex {%s} 0 ]";
    char               *command;
    int                size = 0;

    command = (char *)malloc(strlen(cmd_string) + strlen(list) + 1);
    if (command == NULL)
        return dcp_s_no_memory;
    sprintf(command, cmd_string, list);
    Tcl_Eval(attr_interp, command);
    free(command);
    size = atoi(attr_interp->result);
    if (size > 1)
        return TRUE;
    else
        return FALSE;
}


/*
 * FUNCTION: decode_uuid 
 *
 * OVERVIEW:
 * 
 * This function converts a uuid_t structure into a uuid string. 
 * 
 * INPUTS:
 *  uuid_t *uuid - a structure containing a uuid.
 *
 * OUTPUTS:
 * char *uuid_ptr - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  uuid_s_ok - if the uuid was translatable. 
 */
int
decode_uuid(uuid_t *uuid, char *uuid_ptr)
{
    unsigned32       status = -1;
    unsigned_char_t  *string_p; 

    uuid_to_string((uuid_t *)uuid, &string_p, &status);
    if (status == uuid_s_ok) {
        strcpy((char *)uuid_ptr, (char *)string_p);
        rpc_string_free(&string_p, &status);
    } 
    else
        *uuid_ptr = '0';
    return status;
}


/*
 * FUNCTION:
 * dcp_general_split_list()
 *
 * OVERVIEW:
 * This routine will ensure that an argument which can be a list
 * of fully bound string bindings or the name of the object to
 * operate on are split properly. Basically, this routine ensures
 * that 1 string binding in TCL syntax is handled as one string
 * binding and not a list of things.
 *
 * INPUTS:
 * name_tcllist:
 *           Pointer to string with list structure.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * name_argc:
 *           Pointer to location to fill in with the number of
 *           elements in the list.
 *
 * name_argv:
 *           Pointer to store pointer to array of pointers
 *           to list elements.
 *
 * RETURNS:
 * TCL_OK    if no error occurs.
 * TCL_ERROR if error occurs.
 */
int
dcp_general_split_list(Tcl_Interp *interp,
                       char *name_tcllist,
                       int *name_argc,
                       char ***name_argv)
{
    int                   rc = TCL_OK;
    char                  *sbind; 
    char                  *temp_sbind; 
    rpc_binding_handle_t  server_handle;
    error_status_t        status;

    CLEAR_STATUS(&status);

    /*
     * Determine if there is one string binding in TCL syntax.  
     */
    if (Tcl_SplitList(interp, name_tcllist, name_argc, name_argv) != TCL_OK) {
        return(TCL_ERROR); 
    }

    sbind = dcp_binding_tcl_to_string(*name_argc, *name_argv, &status);
    if (sbind != NULL) {
        rpc_binding_from_string_binding((unsigned_char_p_t)sbind,
                                        &server_handle,
                                        &status);
        if (GOOD_STATUS(&status)) {
            free((char *)*name_argv);
            temp_sbind = dce_strdup(sbind);
            rc = dcp_general_allocate_argv_for_one(interp,
                                                   temp_sbind,
                                                   name_argc,
                                                   name_argv);
        }
        rpc_string_free((unsigned char **)&sbind, &status);
    }

    return(rc);
}


/*
 * FUNCTION:
 * dcp_general_allocate_argv_for_one()
 *
 * OVERVIEW:
 * This routine will allocate only 1 element in the argv[] array.
 *
 * INPUTS:
 * name:     Pointer to 1 object to allocate space for.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * name_argc:
 *           Pointer to location to fill in with the number of
 *           elements in the list.
 *
 * name_argv:
 *           Pointer to store pointer to array of pointers
 *           to list elements.
 *
 * RETURNS:
 * TCL_OK    if no error occurs.
 * TCL_ERROR if error occurs.
 */
int
dcp_general_allocate_argv_for_one(Tcl_Interp *interp,
                                  char *name,
                                  int *name_argc,
                                  char ***name_argv)
{
    char  **temp_argv;

    temp_argv = (char **)malloc(2 * (sizeof(char *)) + strlen(name) + 1);
    if (temp_argv == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    temp_argv[0] = (char *)&temp_argv[2];
    temp_argv[1] = NULL;
    *name_argc = 1;
    strcpy(temp_argv[0], name);
    *name_argv = temp_argv;

    return(TCL_OK);
}


/*
 * FUNCTION:    dcp_general_find_attr_index
 *
 * OVERVIEW:    Given a string as a key, and an array of strings that 
 *              represent a list of valid attributes, return TCL_OK if
 *              the key is a unique substring of one of the attributes.
 *              Return an index that points to the correct attribute upon
 *              success. If the key is not unique, or does not match any
 *              attribute, return TCL_ERROR, and place the reason for
 *              failure in interp->result.
 *
 * INPUTS:
 *  key
 *    A character string that we are trying to find an attribute match for.
 *
 *  count
 *    A count of the attributes in attrs[].
 *
 *  attrs
 *    A pointer to an array of character strings that represent the valid 
 *  attribute names.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  index 
 *    The index of the matched attribute.
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_general_find_attr_index(Tcl_Interp *interp, char *key, int count,
                            char *attrs[], int *index)
{
    int  i;
    int  matches = 0;

    for (i = 0; i < count; i++) {
        if (strncmp(key, attrs[i], strlen(key)) == 0) {
            *index = i;
            matches++;
        }
    }

    switch(matches) {
        case 0:
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_attribute, key);
            *index = -1;
            return TCL_ERROR;
        case 1:
            return TCL_OK;
        default:
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_attr_not_unique, key);
            *index = -1;
            return TCL_ERROR;
    }
}

int
dcp_general_is_bound_locally(Tcl_Interp *interp)
{
    int	  local_flag;
    char  *local;

    local = Tcl_GetVar(interp, "_local", TCL_GLOBAL_ONLY);
    if (local != NULL) {
        if (Tcl_GetBoolean(interp, local, &local_flag) == TCL_OK)
            return local_flag;
    }

    return FALSE;
}

int
dcp_general_bind_local(Tcl_Interp *interp)
{
    rpc_ss_thread_handle_t  thread_handle;
    error_status_t          st;

    dced_init_global_data(FALSE, FALSE, FALSE, TRUE, &st);
    if (st != error_status_ok) {
        DCP_SET_MISC_ERROR(st);
    }

    dced_setup_global_uuids(&st);
    if (st != error_status_ok) {
        DCP_SET_MISC_ERROR(st);
    }

    dced_open_databases(FALSE, &st);
    if (st != error_status_ok) {
        DCP_SET_MISC_ERROR(st);
    }

    dced_setup_acls(&st);
    if (st != error_status_ok) {
        DCP_SET_MISC_ERROR(st);
    }

    dced_init_name_caches(&st);
    if (st != error_status_ok) {
        DCP_SET_MISC_ERROR(st);
    }

    dced_hd_read_post_processors(&st);
    if (st != error_status_ok) {
        DCP_SET_MISC_ERROR(st);
    }

    /*
     * Enable the stub memory managment environment.
     */
    TRY
        thread_handle = rpc_ss_get_thread_handle();
    CATCH(pthread_badparam_e);
        thread_handle = NULL;
    ENDTRY;
    if (thread_handle == NULL) {
        rpc_sm_enable_allocate(&st);
        if (st != error_status_ok) {
            DCP_SET_MISC_ERROR(st);
        }
    }
    else {
        /*
         * Install rpc_ss_allocate() and rpc_ss_free().
         */
        rpc_sm_set_client_alloc_free(rpc_ss_allocate, rpc_ss_free, &st);
        if (st != error_status_ok) {
            DCP_SET_MISC_ERROR(st);
        }
    }

    return TCL_OK;
}

int
dcp_general_unbind_local(Tcl_Interp *interp)
{
    error_status_t  st;

    if (dcp_general_is_bound_locally(interp))
        dced_close_databases(&st);

    return TCL_OK;
}
