/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_var.c,v $
 * Revision 1.2  1996/12/03  10:42:58  salamone
 * Fix headers
 *
 * Revision 1.1  96/10/07  16:40:43  16:40:43  wwang (Weidong Wang)
 * Initial revision
 * 
 * Revision 1.1  1996/10/01  17:07:12  wwang
 * Initial revision
 *
 * Revision 1.1.4.13  1994/08/31  23:03:59  salamone
 * 	CR11916 - Add _b(sec) support.
 * 	[1994/08/31  23:02:23  salamone]
 *
 * Revision 1.1.4.12  1994/08/08  18:52:00  melman
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:31  melman]
 * 
 * Revision 1.1.4.11  1994/08/01  20:41:28  rousseau
 * 	Add _h support (CR 11285).
 * 	[1994/08/01  20:41:05  rousseau]
 * 
 * Revision 1.1.4.10  1994/05/10  17:26:05  salamone
 * 	CR10293 - Free memory from dce_sprintf()
 * 	[1994/05/10  16:17:20  salamone]
 * 
 * Revision 1.1.4.9  1994/05/09  20:36:47  rousseau
 * 	Fix _s(comp) (CR 10583).
 * 	[1994/05/09  20:36:39  rousseau]
 * 
 * Revision 1.1.4.8  1994/05/05  14:09:39  salamone
 * 	Free argv properly.
 * 	[1994/05/05  14:08:40  salamone]
 * 
 * Revision 1.1.4.7  1994/05/04  21:11:20  salamone
 * 	Have another TCL global variable follow each read-only
 * 	convenience variable. This solution fulfills the need
 * 	for multiple interpreters used within dcecp.
 * 	[1994/05/04  21:07:55  salamone]
 * 
 * Revision 1.1.4.6  1994/03/31  22:07:08  salamone
 * 	CR10259 - Remove global C variables for dcecp convenience
 * 	  variable support.
 * 	[1994/03/31  21:14:52  salamone]
 * 
 * Revision 1.1.4.5  1994/03/29  17:48:45  salamone
 * 	CR9854  - Remove malloc of fixed length convenience variables.
 * 	CR10087 - Provide list support of $_n
 * 	[1994/03/29  17:47:41  salamone]
 * 
 * Revision 1.1.4.4  1994/01/05  16:57:58  salamone
 * 	Remove setting interp->result with convenience variable
 * 	[1994/01/05  16:57:40  salamone]
 * 
 * Revision 1.1.4.3  1993/12/30  19:58:50  rousseau
 * 	Changed include of dcemsgmsg.h to dcesvcmsg.h
 * 	[1993/12/30  19:58:07  rousseau]
 * 
 * Revision 1.1.4.2  1993/12/21  19:06:41  rousseau
 * 	Changed include of dcecp_login.h to util_login.h
 * 	[1993/12/21  19:06:34  rousseau]
 * 
 * Revision 1.1.4.1  1993/12/20  15:53:06  salamone
 * 	Remove length parameter from dcp_general_set_namelist()
 * 	[1993/12/17  20:04:15  salamone]
 * 
 * Revision 1.1.2.1  1993/12/14  17:14:01  salamone
 * 	Initial version of util_var.c
 * 	[1993/12/14  17:12:35  salamone]
 * 
 * $EndLog$
 */

/*  
 * MODULE:  
 * util_var.c
 *
 * DESCRIPTION:
 * This module contains routines that support the dcecp convenience 
 * variables.
 */

/* 
 * INCLUDE FILES 
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dcemcpmsg.h>
#include <dce/rpc.h>   
#include <dce/rpcbase.h> 
#include <tcl.h>
#include <dce/dce_cf.h>
#include <moscp.h>
#include <util_login.h>  
#include <util_var.h>  

/* 
 * INTERNAL VARIABLES 
 */
static char *server_name_tbl[] = {"cds", "aud", "sec", "dts", NULL};

/* 
 * PROTOTYPES 
 */
char *dcp__general_tracevar_writeproc(ClientData, Tcl_Interp *, char *, char *, int);
char *dcp__general_tracevar_parentreadproc(ClientData, Tcl_Interp *, char *, int);
void  dcp__general_tracevar_unsetproc(ClientData, Tcl_Interp *, char *, char *, int); 
char *dcp__general_get_parentname(char *);


/* 
 * FUNCTION: 
 * dcp__general_tracevar_parentreadproc()
 *
 * OVERVIEW: 
 * This routine is called when a user or application attempts
 * to read the DCP_C_GENERAL_PARENT_NAME dcecp convenience variable.  
 *
 * INPUTS:
 * client_data:         Value of dcecp convenience variable passed.
 * interp:              Interpreter containing variable.
 * varname:             Name of dcecp convenience variable
 * flags:               Options to Tcl_GetVar()
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * Error message for user.
 */
char * 
dcp__general_tracevar_parentreadproc(ClientData  client_data,
                                     Tcl_Interp  *interp,
                                     char        *varname,
                                     int         flags)
{
    char *result;
    char *name_list;
    char *parentname;
    char *parentname_list = (char *)NULL;
    int i;
    int name_argc;
    char **name_argv = (char **)NULL;
    Tcl_DString dstring;

    flags = TCL_GLOBAL_ONLY; 

    /* 
     * Obtain current value of DCP_C_GENERAL_NAME_LIST
     */
    name_list = Tcl_GetVar(interp, DCP_C_GENERAL_NAME_LIST, flags);

    if (name_list == NULL){
        parentname_list = dce_strdup((char *)DCP_C_GENERAL_NULL);
    } /* End of DCP_C_GENERAL_NAME_LIST was "unset" */
    else {
        if (Tcl_SplitList(interp, name_list, &name_argc, &name_argv) != TCL_OK){
            return((char *)1);
        }
        Tcl_DStringInit(&dstring);
        if (name_argc == 0) {
            parentname_list = dce_strdup((char *)DCP_C_GENERAL_NULL);
	} else {
            for (i = 0; i < name_argc; i++){
               parentname = dcp__general_get_parentname(name_argv[i]);
               parentname_list = Tcl_DStringAppendElement(&dstring, parentname);
            } /* End of loop thru all names in list */
        } /* End of process name_argv */
    } /* End of determine DCP_C_GENERAL_PARENT_NAME value */

    if ((strcmp(parentname_list, "{}")) == 0){
	*parentname_list = '\0';
    } 

    result = DCP_GENERAL_SET_P_VAR(parentname_list);

    /*
     * Free allocated memory
     */
    if (name_argv != NULL) {
        free((char *) name_argv);
        Tcl_DStringFree(&dstring);
    }

    if (result != NULL){
        return((char *)1);
    }

    return((char *)NULL);

} /* End of dcp__general_tracevar_parentreadproc() routine */


/* 
 * FUNCTION: 
 * dcp__general_tracevar_writeproc()
 *
 * OVERVIEW: 
 * This routine is called when a user or application attempts
 * to set/write a dcecp convenience variable.  
 *
 * INPUTS:
 * client_data:         Value of dcecp convenience variable passed.
 * interp:              Interpreter containing variable.
 * varname:             Name of dcecp convenience variable
 * flags:               Options to Tcl_SetVar()
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * Error message for user.
 */
char * 
dcp__general_tracevar_writeproc(ClientData  client_data,
                                Tcl_Interp  *interp,
                                char        *varname,
                                char        *varname_index,
                                int         flags)
{
    char *result;
    unsigned char *tmp_msg_p;
    char *msg_p;

    flags = TCL_GLOBAL_ONLY; 
    if (varname_index == NULL) {
        result = Tcl_SetVar(interp, varname, (char *)client_data, flags); 
    } else {
        result = Tcl_SetVar2(interp, varname, varname_index, 
			     (char *)client_data, flags); 
    }
    /*
     * Return non-NULL value if error occurred 
     * in Tcl_SetVar() call.
     */
    if (result == NULL) {
        return((char *)1);
    }
    tmp_msg_p = dce_sprintf(mcp_s_var_set_readonly, varname);
    msg_p = dce_strdup((char *)tmp_msg_p);
    free((char *) tmp_msg_p);
    return((char *)msg_p); 

} /* End of dcp__general_tracevar_writeproc() routine */


/* 
 * FUNCTION: 
 * dcp__general_tracevar_unsetproc()
 *
 * OVERVIEW:
 * This routine is called when a user or application attempts
 * to unset a dcecp convenience variable.  
 *
 * INPUTS:
 * client_data:         Value of dcecp convenience variable passed.
 * interp:              Interpreter containing variable.
 * varname:             Name of dcecp convenience variable
 * flags:               Options to Tcl_SetVar()
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:  
 * None
 */
void
dcp__general_tracevar_unsetproc(ClientData  client_data,
                                Tcl_Interp  *interp,
                                char        *varname,
                                char        *varname_index,
                                int         flags)
{
    char *result;
    unsigned char *msg_p;
    char output_string[BUFSIZ] = "";

    /*
     * "varname" has been destroyed and the traces have been removed.
     */
    flags = TCL_GLOBAL_ONLY;
    if (varname_index == NULL) {
        result = Tcl_SetVar(interp, varname, (char *)client_data, flags); 
    } else {
        result = Tcl_SetVar2(interp, varname, varname_index,
			     (char *)client_data, flags); 
    }
    /*
     * If Tcl_SetVar() call successful, reset tracing.
     */
    if (result != NULL) {
	if (varname_index == NULL) {
	    sprintf(output_string, "%s", varname);
            Tcl_TraceVar(interp, varname, 
                         TCL_TRACE_WRITES|TCL_GLOBAL_ONLY,
                         (Tcl_VarTraceProc *)dcp__general_tracevar_writeproc, 
                         client_data); 
            Tcl_TraceVar(interp, varname, 
                         TCL_TRACE_UNSETS|TCL_GLOBAL_ONLY,
                         (Tcl_VarTraceProc *)dcp__general_tracevar_unsetproc, 
                         client_data); 
            if ((strcmp(varname, DCP_C_GENERAL_PARENT_NAME)) == 0){
                Tcl_TraceVar(interp, varname, 
                      TCL_TRACE_READS|TCL_GLOBAL_ONLY,
                      (Tcl_VarTraceProc *)dcp__general_tracevar_parentreadproc, 
                      client_data); 
	    }
        } else {
	    sprintf(output_string, "%s(%s)", varname, varname_index);
            Tcl_TraceVar2(interp, varname, varname_index,
                          TCL_TRACE_WRITES|TCL_GLOBAL_ONLY,
                          (Tcl_VarTraceProc *)dcp__general_tracevar_writeproc, 
                          client_data); 
            Tcl_TraceVar2(interp, varname, varname_index,
                          TCL_TRACE_UNSETS|TCL_GLOBAL_ONLY,
                          (Tcl_VarTraceProc *)dcp__general_tracevar_unsetproc, 
                          client_data); 
        }
        msg_p = dce_sprintf(mcp_s_var_unset_readonly, output_string);
        Tcl_SetResult(interp, (char *)msg_p, TCL_DYNAMIC);
	free((char *) msg_p);
    } /* End of result not equal NULL */

} /* End of dcp__general_tracevar_unsetproc() routine */


/* 
 * FUNCTION: 
 * dcp_general_set_var()
 *
 * OVERVIEW:
 * Sets a dcecp convenience variable to a new value assuming 
 * that the variable is currently being traced. Then, must
 * reset all traces.
 *
 * INPUTS:
 * interp:              Interpreter containing variable.
 * varname:             Name of dcecp convenience variable
 * new_element:         New element for dcecp convenience variable.
 *
 * INPUT/OUTPUTS:
 * setvar_flags:        Options to Tcl_SetVar()
 *
 * OUTPUTS:
 * dcp_g_general_(x)_var:   Where (x) refers to dcecp convenience variables.
 *
 * RETURNS:
 * A non-NULL value is returned upon error.
 */
char *
dcp_general_set_var(Tcl_Interp  *interp,
                    char        *varname,
                    char        *varname_index,
                    char        *new_element,
                    int         setvar_flags)
{
    char *result;
    char *old_varname_p;
    char old_varname[10] = "_old";
    char *current_var_list = (char *)NULL;
    char *new_var_list = (char *)NULL;
    int getvar_flags = TCL_GLOBAL_ONLY;

    setvar_flags |= TCL_GLOBAL_ONLY; 
    old_varname_p = strcat(old_varname, varname);

    /*
     * Obtain current value of convenience variable
     */
    if (varname_index == NULL) {
        current_var_list = Tcl_GetVar(interp, old_varname_p, getvar_flags);
    } else {
        current_var_list = Tcl_GetVar2(interp, old_varname_p, 
				       varname_index, getvar_flags);
    }

    /*
     * Remove all traces on convenience variable
     */
    if (current_var_list != NULL){
	if (varname_index == NULL) {
            Tcl_UntraceVar(interp, varname,
                      TCL_TRACE_WRITES|TCL_GLOBAL_ONLY,
                      (Tcl_VarTraceProc *)dcp__general_tracevar_writeproc, 
                      (ClientData)current_var_list);
            Tcl_UntraceVar(interp, varname,
                      TCL_TRACE_UNSETS|TCL_GLOBAL_ONLY,
                      (Tcl_VarTraceProc *)dcp__general_tracevar_unsetproc, 
                      (ClientData)current_var_list);
    	    Tcl_UntraceVar(interp, varname,
                      TCL_TRACE_READS|TCL_GLOBAL_ONLY,
                      (Tcl_VarTraceProc *)dcp__general_tracevar_parentreadproc, 
                      (ClientData)current_var_list);
       } else {
            Tcl_UntraceVar2(interp, varname, varname_index,
                           TCL_TRACE_WRITES|TCL_GLOBAL_ONLY,
                           (Tcl_VarTraceProc *)dcp__general_tracevar_writeproc, 
                           (ClientData)current_var_list);
            Tcl_UntraceVar2(interp, varname, varname_index,
                           TCL_TRACE_UNSETS|TCL_GLOBAL_ONLY,
                           (Tcl_VarTraceProc *)dcp__general_tracevar_unsetproc, 
                           (ClientData)current_var_list);
       }
    }
	      
    /*
     * Set convenience variable with new value
     */
    if (varname_index == NULL) {
        result = Tcl_SetVar(interp, varname, new_element, setvar_flags);
    } else {
        result = Tcl_SetVar2(interp, varname, varname_index,
			     new_element, setvar_flags);
    }
    /*
     * Return non-NULL value if error occurred
     * in Tcl_SetVar() call.
     */
    if (result == NULL) {
        return((char *)1);
    }

    /*
     * Set convenience variable counterpart with new value
     */
    if (varname_index == NULL) {
        result = Tcl_SetVar(interp, old_varname_p, new_element, setvar_flags);
    } else {
        result = Tcl_SetVar2(interp, old_varname_p, varname_index,
			     new_element, setvar_flags);
    }

    /*
     * Obtain new value of convenience variable
     * This is done in case this routine is invoked with
     * the TCL_APPEND_VALUE and TCL_LIST_ELEMENT bits set
     * in setvar_flags.
     */
    if (varname_index == NULL) {
        new_var_list = Tcl_GetVar(interp, old_varname_p, getvar_flags);
    } else {
        new_var_list = Tcl_GetVar2(interp, old_varname_p, varname_index,
				   getvar_flags);
    }


    /*
     * Re-set all traces on convenience variable
     */
    if (new_var_list != NULL){
	if (varname_index == NULL) {
            Tcl_TraceVar(interp, varname,
                         TCL_TRACE_WRITES|TCL_GLOBAL_ONLY,
                         (Tcl_VarTraceProc *)dcp__general_tracevar_writeproc, 
                         (ClientData) new_var_list);
            Tcl_TraceVar(interp, varname,
                         TCL_TRACE_UNSETS|TCL_GLOBAL_ONLY,
                         (Tcl_VarTraceProc *)dcp__general_tracevar_unsetproc, 
                         (ClientData) new_var_list);
            if ((strcmp(varname, DCP_C_GENERAL_PARENT_NAME)) == 0){
                 Tcl_TraceVar(interp, varname,
                      TCL_TRACE_READS|TCL_GLOBAL_ONLY,
                      (Tcl_VarTraceProc *)dcp__general_tracevar_parentreadproc, 
                      (ClientData) new_var_list);
            } /* End of varname = DCP_C_GENERAL_PARENT_NAME */
	} else {
            Tcl_TraceVar2(interp, varname, varname_index,
                         TCL_TRACE_WRITES|TCL_GLOBAL_ONLY,
                         (Tcl_VarTraceProc *)dcp__general_tracevar_writeproc, 
                         (ClientData) new_var_list);
            Tcl_TraceVar2(interp, varname, varname_index,
                         TCL_TRACE_UNSETS|TCL_GLOBAL_ONLY,
                         (Tcl_VarTraceProc *)dcp__general_tracevar_unsetproc, 
                         (ClientData) new_var_list);
	}
    }

    return((char *)NULL);

} /* End of dcp_general_set_var() routine */


/* 
 * FUNCTION: 
 * dcp_general_unset_var()
 *
 * OVERVIEW:
 * Removes a dcecp convenience variable assuming that the
 * variable is currently being traced.
 *
 * INPUTS:
 * interp:              Interpreter containing variable.
 * varname:             Name of dcecp convenience variable
 * var_list:            Whole array of dcecp convenience variable.
 *
 * INPUT/OUTPUTS:
 * unsetvar_flags:        Options to Tcl_UnsetVar()
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:  
 * None
 */
void 
dcp_general_unset_var(Tcl_Interp    *interp,
                      char          *varname,
                      char          *varname_index,
                      int           unsetvar_flags)
{
    char *var_list = (char *)NULL;
    char *old_varname_p;
    char old_varname[10] = "_old";
    int getvar_flags = TCL_GLOBAL_ONLY;

    unsetvar_flags |= TCL_GLOBAL_ONLY; 
    old_varname_p = strcat(old_varname, varname);

    /*
     * Obtain current value of convenience variable
     */
    if (varname_index == NULL) {
        var_list = Tcl_GetVar(interp, old_varname_p, getvar_flags);
    } else {
        var_list = Tcl_GetVar2(interp, old_varname_p, varname_index,
			       getvar_flags);
    }

    /*
     * Remove all traces on convenience variable
     */
    if (var_list != NULL){
	if (varname_index == NULL) {
            Tcl_UntraceVar(interp, varname,
                           TCL_TRACE_WRITES|TCL_GLOBAL_ONLY,
                           (Tcl_VarTraceProc *)dcp__general_tracevar_writeproc, 
                           (ClientData)var_list);
            Tcl_UntraceVar(interp, varname,
                           TCL_TRACE_UNSETS|TCL_GLOBAL_ONLY,
                           (Tcl_VarTraceProc *)dcp__general_tracevar_unsetproc, 
                           (ClientData)var_list);
            Tcl_UntraceVar(interp, varname,
                      TCL_TRACE_READS|TCL_GLOBAL_ONLY,
                      (Tcl_VarTraceProc *)dcp__general_tracevar_parentreadproc, 
                      (ClientData) var_list);
	} else {
            Tcl_UntraceVar2(interp, varname, varname_index,
                           TCL_TRACE_WRITES|TCL_GLOBAL_ONLY,
                           (Tcl_VarTraceProc *)dcp__general_tracevar_writeproc, 
                           (ClientData)var_list);
            Tcl_UntraceVar2(interp, varname, varname_index,
                           TCL_TRACE_UNSETS|TCL_GLOBAL_ONLY,
                           (Tcl_VarTraceProc *)dcp__general_tracevar_unsetproc, 
                           (ClientData)var_list);
	}
    }

    /*
     * Just destroy the convenience variable and not it's "old"
     * TCL global variable counterpart. The "old" TCL global
     * variable must exist for Tcl_GetVar() to succeed.
     */
    if (varname_index == NULL) {
        Tcl_UnsetVar(interp, varname, unsetvar_flags);
    } else {
        Tcl_UnsetVar2(interp, varname, varname_index, unsetvar_flags);
    }

} /* End of dcp_general_unset_var() */


/* 
 * FUNCTION: 
 * dcp__general_get_parentname()
 *
 * OVERVIEW:
 * Convenience Variable DCP_C_GENERAL_PARENT_NAME holds the parent 
 * of Convenience Variable DCP_C_GENERAL_NAME_LIST.  If 
 * DCP_C_GENERAL_NAME_LIST is a list, then this
 * is a list of the same length, where each element is the
 * parent of the corresponding element in DCP_C_GENERAL_NAME_LIST.
 *
 * INPUTS:
 * in_string:           String to parse.
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * Parent of DCP_C_GENERAL_NAME_LIST.
 */
char *
dcp__general_get_parentname(char *in_string)
{
    int childname_len, parentname_len, in_string_len;
    char *childname;
    char *parentname;

    /* 
     * If no slashes; parentname is empty string 
     */
    parentname = dce_strdup((char *)DCP_C_GENERAL_NULL);

    if ((in_string != NULL) && (*in_string != '\0')){
        in_string_len = strlen(in_string);
        if ((childname = strrchr(in_string, '/')) != NULL){
            childname_len = strlen(childname);
            if((parentname_len = (in_string_len - childname_len)) != 0){
                parentname = dce_strdup(in_string);
                parentname[parentname_len] = '\0';
            }
        }  /* End of childname != NULL */
    } /* End of valid in_string */

    return((char *)parentname);

} /* End of dcp__general_get_parentname() routine */


/* 
 * FUNCTION: 
 * dcp_general_init_var()
 *
 * OVERVIEW:
 * Establish initial traces for all dcecp Convenience Variables
 *
 * INPUTS:
 * interp:              Interpreter containing variable.
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * dcp_g_general_(x)_var:   Where (x) refers to dcecp convenience variables.
 *
 * RETURNS:
 * NULL if error occurs.
 */
void * 
dcp_general_init_var(Tcl_Interp *interp)
{
    char *result;
    int i;
    int flags = TCL_GLOBAL_ONLY;
    char *old_varname_p;
    char old_varname[10];
    char *hostname = NULL;
    error_status_t st;

    /*
     * Set all writeable convenience varibles.
     */
    result = DCP_GENERAL_SET_NAMELIST("");
    if (result == NULL){
        return((void *)NULL);   /* Error occurred */
    }

    for (i = 0; server_name_tbl[i] != NULL; i++) { 
        result = DCP_GENERAL_SET_S_VAR(server_name_tbl[i], "");
        if (result == NULL){
            return((void *)NULL);   /* Error occurred */
        }
    } /* End of loop thru DCP_C_GENERAL_SERVER_NAME array */


    /*
     * Set traces on all read-only convenience varibles and
     * set all TCL global variables and their counterparts.
     */
    strcpy(old_varname, DCP_C_GENERAL_VARNAME_PREFIX);
    old_varname_p = strcat(old_varname, 
			   DCP_C_GENERAL_BOUND_SERVER);
    Tcl_SetVar2(interp, old_varname_p, "sec", "", flags);
    result = DCP_GENERAL_SET_B_VAR("sec", "");
    if (result != NULL){
        return((void *)NULL);   /* Error occurred */
    }

    strcpy(old_varname, DCP_C_GENERAL_VARNAME_PREFIX);
    old_varname_p = strcat(old_varname, 
			   DCP_C_GENERAL_PARENT_NAME);
    Tcl_SetVar(interp, old_varname_p, "", flags);
    result = DCP_GENERAL_SET_P_VAR("");
    if (result != NULL){
        return((void *)NULL);   /* Error occurred */
    }

    strcpy(old_varname, DCP_C_GENERAL_VARNAME_PREFIX);
    old_varname_p = strcat(old_varname, 
                           DCP_C_GENERAL_PRINCIPAL_CELL);
    Tcl_SetVar(interp, old_varname_p, "", flags);
    result = DCP_GENERAL_SET_C_VAR("");
    if (result != NULL){
        return((void *)NULL);   /* Error occurred */
    }

    strcpy(old_varname, DCP_C_GENERAL_VARNAME_PREFIX);
    old_varname_p = strcat(old_varname, 
                           DCP_C_GENERAL_OBJECT);
    Tcl_SetVar(interp, old_varname_p, "", flags);
    result = DCP_GENERAL_SET_O_VAR("");
    if (result != NULL){
        return((void *)NULL);   /* Error occurred */
    }

    strcpy(old_varname, DCP_C_GENERAL_VARNAME_PREFIX);
    old_varname_p = strcat(old_varname, 
                           DCP_C_GENERAL_RETURN_VALUE);
    Tcl_SetVar(interp, old_varname_p, "", flags);
    result = DCP_GENERAL_SET_R_VAR("");
    if (result != NULL){
        return((void *)NULL);   /* Error occurred */
    }

    strcpy(old_varname, DCP_C_GENERAL_VARNAME_PREFIX);
    old_varname_p = strcat(old_varname, 
                           DCP_C_GENERAL_PRINCIPAL);
    Tcl_SetVar(interp, old_varname_p, "", flags);
    result = DCP_GENERAL_SET_U_VAR("");
    if (result != NULL){
        return((void *)NULL);   /* Error occurred */
    }

    strcpy(old_varname, DCP_C_GENERAL_VARNAME_PREFIX);
    old_varname_p = strcat(old_varname, 
                           DCP_C_GENERAL_HOSTNAME);
    Tcl_SetVar(interp, old_varname_p, "", flags);
    dce_cf_get_host_name(&hostname, &st);
    if (st != error_status_ok) {
        result = DCP_GENERAL_SET_H_VAR("");
    } 
    else {
        result = DCP_GENERAL_SET_H_VAR(hostname);
    }
    if (result != NULL){
        return((void *)NULL);   /* Error occurred */
    }

    return((void *)1);    /* OK */

} /* End of dcp_general_init_var() routine */


/* 
 * FUNCTION:    
 * dcp_general_cleanup_var()
 *
 * OVERVIEW:
 * Remove all traces on dcecp Convenience Variables
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * dcp_g_general_(x)_var:   Where (x) refers to dcecp convenience variables.
 *
 * RETURNS: 
 * None
 */
void 
dcp_general_cleanup_var(Tcl_Interp *interp)
{
    int i;

    /*
     * Destroy all writeable convenience varibles.
     */
    DCP_GENERAL_UNSET_NAMELIST();
    for (i = 0; server_name_tbl[i] != NULL; i++) { 
        DCP_GENERAL_UNSET_S_VAR(server_name_tbl[i]);
    }

    /*
     * Remove traces on all read-only convenience varibles and
     * destroy them.
     */
    DCP_GENERAL_UNSET_B_VAR("sec");
    DCP_GENERAL_UNSET_C_VAR();
    DCP_GENERAL_UNSET_H_VAR();
    DCP_GENERAL_UNSET_O_VAR();
    DCP_GENERAL_UNSET_P_VAR();
    DCP_GENERAL_UNSET_R_VAR();
    DCP_GENERAL_UNSET_U_VAR();

} /* End of dcp_general_cleanup_var() routine */
