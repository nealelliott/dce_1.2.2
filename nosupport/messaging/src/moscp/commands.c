/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * MODULE: commands.c
 *
 * DESCRIPTION:
 *   Code for all the object commands.
 * ______________________________________________________________________
 *
 * $Log: commands.c,v $
 * Revision 1.7  1996/12/03  10:42:23  salamone
 * Fix headers
 *
 * Revision 1.6  96/12/02  16:19:37  16:19:37  salamone (Julie Salamone)
 * Remove queue rename
 * 
 * Revision 1.5  96/11/29  13:09:02  13:09:02  salamone (Julie Salamone)
 * Fix queue enqueue
 * 
 * Revision 1.4  96/11/23  12:10:31  12:10:31  salamone (Julie Salamone)
 * Alphabetize que commands
 * 
 * Revision 1.3  96/11/22  18:06:46  18:06:46  salamone (Julie Salamone)
 * Remove queue save & cancel
 * 
 * $EndLog$
 * ______________________________________________________________________
 */

/* INCLUDE FILES  */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h> 
#include <fcntl.h>
#include <unistd.h>             /* For execl() */
#include <sys/wait.h>           /* For waitpid() */
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcemcpmsg.h>
#include <dce/dcesvcmsg.h>
#include <dce/dce_error.h>
#include <tcl.h>
#include <moscp.h>
#include <util_help.h>
#include <util_login.h>
#include <util_signal.h>
#include <util_var.h>
#include <util_binding.h>
#include <queue.h>

/* DECLARATIONS */
typedef struct dcecp_cmd_table_s_t {
    char *cmdName;
    Tcl_CmdProc *proc;
} dcecp_cmd_table_t;
 
/* PROTOTYPES */
int mcp_init            (ClientData, Tcl_Interp *, int, char **);
int mcp_errtext         (ClientData, Tcl_Interp *, int, char **);
int mcp_quit            (ClientData, Tcl_Interp *, int, char **);
int mcp_cleanup         (ClientData, Tcl_Interp *, int, char **);
int mcp_resolve         (ClientData, Tcl_Interp *, int, char **);
int mcp_shell           (ClientData, Tcl_Interp *, int, char **);
int mcp_echo            (ClientData, Tcl_Interp *, int, char **);
int mcp_queue           (ClientData, Tcl_Interp *, int, char **);

/* INTERNAL VARIABLES */
dcecp_cmd_table_t table[] = {
    { "dcecpinit",      mcp_init },
    { "errtext",        mcp_errtext },
    { "quit",           mcp_quit },
    { "dcecp_cleanup",  mcp_cleanup },
    { "resolve",        mcp_resolve },
    { "shell",          mcp_shell },
    { "echo",           mcp_echo },
    { "queue",          mcp_queue }
};


int
mcp_initInterp(Tcl_Interp *interp)
{
    int                result;
    int                i;
    dcecp_cmd_table_t *tp;
    void              *init_var_p;
    char               cmd[128];
    
    static char inittclCmd[] = "if [file exists [info library]/init.tcl] {source [info library]/init.tcl}";
    static char initdcpCmd[] = "if [file exists [set dcecp_library]/init.dcecp] {source [set dcecp_library]/init.dcecp}";
    static char setautopath[] = "if {[info globals auto_path] == \"auto_path\"} {set auto_path [linsert $auto_path 0 $dcecp_library]} else {set auto_path $dcecp_library}";
    static char mcp_exit[] = "rename exit exit.old; proc exit {{status 0}} {dcecp_cleanup; exit.old $status}";
    
    /* Allocate space for dcecp Convenience Variables */
    init_var_p = dcp_general_init_var(interp);
    if (init_var_p == NULL){
        DCP_SET_MISC_ERROR(mcp_s_no_memory);
    }
    
    /* Tell Tcl about all the commands */
    i = sizeof table / sizeof table[0];
    for (tp=table; --i >= 0; tp++) {
	Tcl_CreateCommand(interp, tp->cmdName, tp->proc, NULL, NULL);
    }
	
#ifdef _DCECP_TEST

    /* Initialize signal handling support */
    Tcl_InitSignalHandling(interp);
    
#endif /* _DCECP_TEST */

    /* Overload exit with our own exit */
    result = Tcl_Eval(interp, mcp_exit);
    if (result != TCL_OK) {
        printf("%s\n", interp->result);
        exit(1);
    }
    
    /* Run the init.tcl file in the Tcl library */
    result = Tcl_Eval(interp, inittclCmd);
    if (result != TCL_OK) {
        printf("%s\n", interp->result);
        exit(1);
    }
    
    /* Set $dcecp_library */
    sprintf(cmd, "set dcecp_library %s/%s", dcelocal_path, DCECP_SCRIPT_DIR);
    result = Tcl_Eval(interp, cmd);
    if (result != TCL_OK) {
        printf("%s\n", interp->result);
        exit(1);
    }
    
    /*
     * Run the init.dcecp file in the dcecp library, 
     * dcecp_library must be set 
     */
    result = Tcl_Eval(interp, initdcpCmd);
    if (result != TCL_OK) {
        printf("%s\n", interp->result);
        exit(1);
    }
    
    /* 
     * Set $auto_path to use dcecp library 
     * Autoloading allows dcecp to start up without 
     * loading any script files.  The files will be loaded when 
     * their procedures are invoked.
     */
    result = Tcl_Eval(interp, setautopath);
    if (result != TCL_OK) {
        printf("%s\n", interp->result);
        exit(1);
    }
    
    return TCL_OK;
}

int
mcp_init(ClientData clientData, Tcl_Interp *interp,int argc, char *argv[])
{
    mcp_initInterp(interp);
}

/*
 * FUNCTION: mcp_resolve
 *
 * OVERVIEW: This takes a server partial binding handle and an
 *    interface identifier and resolves the partial binding handle.
 *
 * INPUTS:
 *  ClientData clientData
 *  int argc 
 *  char *argv[]
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp   
 *
 * RETURNS:
 *  TCL_OK on success. 
 */


int
mcp_resolve(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{


    rpc_binding_handle_t     binding_h, out_binding;
    char                     *if_h_string;
    rpc_ep_inq_handle_t      inquiry_context;
    rpc_if_id_t              if_id;
    char                   *object_uuid_string = NULL;
    uuid_t                  object_uuid;
    uuid_t                   obj_uuid;
    unsigned_char_p_t        elt;
    char                     *vers_minor;
    char                     *version;
    error_status_t           st;
    char                     *end;
    unsigned_char_t          *full_binding;
    boolean32                found = FALSE;
    int                      aargc;
    char                     **aargv;
    char                     *binding_string;
    char                     *resolve_string;
    int                      bargc;
    char                     **bargv;
    boolean32                have_obj_uuid = FALSE;
    boolean32                help_flag = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-interface", DCP_ARGV_STRING,NULL,NULL, mcp_t_resolve_interface_help},
        {"-object",    DCP_ARGV_STRING,NULL,NULL, mcp_t_object_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, mcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    if_h_string = NULL;

    arg_table[0].dst = (char *)&if_h_string;
    arg_table[1].dst = (char *)&object_uuid_string;
    arg_table[2].dst = (char *)&help_flag;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Handle requests for help */
    if (help_flag) {
        DCP_CHECK_MISC_HELP(2);
    }

    DCP_CHECK_EXTRA_ARGS();  
    if (argc <= 1) {
        DCP_SET_RESULT_CODE(mcp_s_resolve_need_binding);
        return TCL_ERROR;
    }
    
    if (!if_h_string) {
        DCP_SET_RESULT_CODE(mcp_s_resolve_need_if);
        return TCL_ERROR;
    }
    
    /* 
     * Parse the interface id string into a uuid, major number and 
     * minor number.
     */
    if(Tcl_SplitList(interp,if_h_string,&aargc,&aargv) == TCL_ERROR)
        return TCL_ERROR;

    if(aargc > 1)
        version = aargv[1];
    else {
        version =  strchr(aargv[0], (int )',');
        if(version)
            version++;
        else{
            DCP_SET_RESULT_CODE(mcp_s_resolve_bad_version);
            return TCL_ERROR;
        }
    }
    
    vers_minor = strchr(version, (int )'.');

    if(vers_minor)
        vers_minor++;
    else {
        DCP_SET_RESULT_CODE(mcp_s_resolve_bad_version);
        return TCL_ERROR;
    }
        

    if_id.vers_minor = strtol(vers_minor,&end,16);
    vers_minor--;
    *vers_minor = '\0';

    if_id.vers_major = strtol(version,&end,16);
    version--;
    *version = '\0';
    uuid_from_string((unsigned char *)aargv[0], &if_id.uuid, &st);
    DCP_CHECK_UUID_ERROR(st);
    
    if (object_uuid_string != NULL) {
        uuid_from_string((unsigned char *)object_uuid_string,
                         &object_uuid,
                         &st);
        if (st != rpc_s_ok) {
            DCP_SET_RESULT_CODE(st);
            return TCL_ERROR;
        }
        have_obj_uuid = TRUE;
    }
    
    if(Tcl_SplitList(interp,argv[1],&bargc,&bargv) == TCL_ERROR)
        return TCL_ERROR;
    
    if(bargc > 1) {
        binding_string = dcp_binding_tcl_to_string(bargc,bargv,&st);
    } 
    else {
        binding_string = dce_strdup(argv[1]);
    }

    rpc_binding_from_string_binding((unsigned char *)binding_string, 
                                    &binding_h, 
                                    &st);
    DCP_CHECK_RPC_ERROR(st);

    if (have_obj_uuid) {
        rpc_mgmt_ep_elt_inq_begin(binding_h,
                                  rpc_c_ep_match_by_both,
                                  &if_id,
                                  rpc_c_vers_exact,
                                  &object_uuid,
                                  &inquiry_context,
                                  &st);
    } else {
        rpc_mgmt_ep_elt_inq_begin(binding_h,
                                  rpc_c_ep_match_by_if,
                                  &if_id,
                                  rpc_c_vers_exact,
                                  NULL,
                                  &inquiry_context,
                                  &st);
    }
    DCP_CHECK_RPC_ERROR(st);
    Tcl_ResetResult(interp);
    for (;;){
        rpc_mgmt_ep_elt_inq_next(inquiry_context,
                                 &if_id,
                                 &out_binding,
                                 &obj_uuid,
                                 &elt,
                                 &st);
        if(st == rpc_s_no_more_elements){
            rpc_mgmt_ep_elt_inq_done(&inquiry_context,&st);
            free(binding_string);
            DCP_CHECK_RPC_ERROR(st);
            if(!found){
                DCP_SET_RESULT_CODE(mcp_s_resolve_not_found);
                return TCL_ERROR;
            }
            else 
                return TCL_OK;
        }
        DCP_CHECK_RPC_ERROR(st);
        rpc_binding_to_string_binding(out_binding, &full_binding, &st);
        DCP_CHECK_RPC_ERROR(st);
        if(!strncmp((char *)full_binding,binding_string,strlen(binding_string))){
            found = TRUE;
            resolve_string = dcp_binding_string_to_tcl((char *)full_binding,&st);
            if(strlen(interp->result))
                Tcl_AppendResult(interp, "\n", (char *)resolve_string,NULL);
            else
                Tcl_AppendResult(interp, (char *)resolve_string,NULL);
            free(resolve_string);
        }
        rpc_binding_free(&out_binding,&st);
        DCP_CHECK_RPC_ERROR(st);
        rpc_string_free(&full_binding,&st);
        DCP_CHECK_RPC_ERROR(st);
    } /* end while TRUE */
} /* end mcp_resolve */


/*
 * FUNCTION:    mcp_errtext
 *
 * OVERVIEW:    Display a message from an installed message catalog. Use
 *              a supplied argument message code to find the message.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
mcp_errtext(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    error_status_t              err_code;
    char                        *endptr;
    
    /* Check for extraneous arguments */
    if (argc > 2) {
        if (argv[2][0] == '-') {
            Tcl_SetResult(interp, (char *)dce_sprintf(mcp_s_bad_opt,
                                                      argv[2]), TCL_DYNAMIC);
        }
        else {
            Tcl_SetResult(interp, (char *)dce_sprintf(mcp_s_bad_arg,
                                                      argv[2]), TCL_DYNAMIC);
        }
        return TCL_ERROR;
    }
    
    if (argc < 2) {
        DCP_SET_RESULT_CODE(mcp_s_need_arg);
        return TCL_ERROR;
    }
    
    err_code = strtoul(argv[1], &endptr, 0);
    
    if ((err_code == 0) || (endptr[0] != '\0')) {
        Tcl_SetResult(interp, 
                      (char *)dce_sprintf(mcp_s_errtext_invalid_code, argv[1]),
                      TCL_DYNAMIC);
        return TCL_ERROR;
    }

    DCP_SET_RESULT_CODE(err_code);
    return TCL_OK;
}


/*
 * FUNCTION:    mcp_quit
 *
 * OVERVIEW:    Exit dcecp. Cleanup as necessary.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
mcp_quit(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    /* Just call the Tcl command exit */
    Tcl_Eval(interp, "exit");
    return TCL_OK;
}


/*
 * FUNCTION:    mcp_cleanup
 *
 * OVERVIEW:    Do exit cleanup work.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
mcp_cleanup(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    dcp_general_cleanup_var(interp);
    delete_all_login_contexts(interp);
}


/*
 * FUNCTION:    mcp_shell
 *
 * OVERVIEW:    With no arguments, spawns a shell. With arguments, runs
 *              the specified command and returns.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
mcp_shell(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    char *command, *shell;
    pid_t pid;
    int w, status;
    
    /* Get the correct shell */    
    shell = getenv("SHELL");
    if (shell == NULL) {
        shell = DEFAULT_SHELL;
    }

    /* If no args, just spawn $SHELL */
    if (argc == 1) {
        if ((pid = fork()) == 0) {
            /* In Child */
            execl(DEFAULT_SHELL, "sh", "-c", shell, (char *)NULL);
            exit(0);
            /* NOTREACHED */
        }
        
        if (pid == -1) {
            DCP_SET_RESULT_CODE(mcp_s_shell_fork_fail);
            return TCL_ERROR;
        }
        
        do {
            w = waitpid(pid, &status, 0);
        } while (w == -1 && errno == EINTR);
    }
    else {
        /* Execute a single command */
        command = Tcl_Merge(argc - 1, argv + 1);  
        
        status = Tcl_VarEval(interp, 
                             "exec ", 
                             command, 
                             (char *)NULL);
        free(command);
        return status;
    }
    return TCL_OK;
}

/*
 * FUNCTION:    mcp_echo
 *
 * OVERVIEW:    Evaluate and display the supplied arguments.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
mcp_echo(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    char *cmd;
    
    if (argc == 1) {
        return TCL_OK;
    }
    
    cmd = Tcl_Merge(argc-1, argv+1);
    puts(cmd);
    free((char *)cmd);
    
    return TCL_OK;
}


int
mcp_queue(ClientData clientData, Tcl_Interp *interp, 
              int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    dcp_ArgvInfo arg_table[] = {
	{"catalog",DCP_ARGV_CONSTANT,(char *)dcp_que_catalog,(char *)&func,
             mcp_t_queue_catalog_help},
        {"create",DCP_ARGV_CONSTANT,(char *)dcp_que_create,(char *)&func,
             mcp_t_queue_create_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)dcp_que_delete,(char *)&func,
             mcp_t_queue_delete_help},
        {"dequeue",DCP_ARGV_CONSTANT,(char *)dcp_que_dequeue,(char *)&func,
             mcp_t_queue_dequeue_help},
	{"enqueue",DCP_ARGV_CONSTANT,(char *)dcp_que_enqueue,(char *)&func,
             mcp_t_queue_enqueue_help},
        {"list",DCP_ARGV_CONSTANT,(char *)dcp_que_list,(char *)&func,
             mcp_t_queue_list_help},
        {"modify",DCP_ARGV_CONSTANT,(char *)dcp_que_modify,(char *)&func,
             mcp_t_queue_modify_help},
        {"move",DCP_ARGV_CONSTANT,(char *)dcp_que_move,(char *)&func,
             mcp_t_queue_move_help},
	{"remove",DCP_ARGV_CONSTANT,(char *)dcp_que_remove,(char *)&func,
             mcp_t_queue_remove_help},
        {"show",DCP_ARGV_CONSTANT,(char *)dcp_que_show,(char *)&func,
             mcp_t_queue_show_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
             mcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };
    
    func = NULL;
    arg_table[10].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(mcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("queue");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                      DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                      DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(mcp_t_queue_vhelp);
    
    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}

