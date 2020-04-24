/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */

#ifndef _MOSCP_H
#define _MOSCP_H

#include <stddef.h>
#include <pthread.h>
#include <dce/rpc.h>
#include <tcl.h>
#include <dce/rgybase.h>        /* For sec_rgy_status_ok */
#include <dce/dce_error.h>
#include <dce/sec_login.h>
#include <dce/dce_utils.h>
#include <dce/audit.h>

/*
 * Structure used to specify how to handle argv options.
 */

typedef struct {
    char *key;          /* The key string that flags the option in the
                         * argv array. */
    int type;           /* Indicates option type;  see below. */
    char *src;          /* Value to be used in setting dst;  usage
                         * depends on type. */
    char *dst;          /* Address of value to be modified;  usage
                         * depends on type. */
    unsigned32 helpcode;/* Status code representing documentation */
                        /* message describing this option. */
} dcp_ArgvInfo;


/*
 * Legal values for the type field of a dcp_ArgvInfo: see the user
 * documentation for details.
 */

#define DCP_ARGV_CONSTANT               15
#define DCP_ARGV_INT                    16
#define DCP_ARGV_STRING                 17
#define DCP_ARGV_UID                    18
#define DCP_ARGV_REST                   19
#define DCP_ARGV_FLOAT                  20
#define DCP_ARGV_FUNC                   21
#define DCP_ARGV_GENFUNC                22
#define DCP_ARGV_HELP                   23
#define DCP_ARGV_CONST_OPTION           24
#define DCP_ARGV_OPTION_VALUE           25
#define DCP_ARGV_OPTION_NAME_VALUE      26
#define DCP_ARGV_END                    27
#define DCP_ARGV_OPS                    28

/*
 * Flag bits for passing to dcp_ParseArgv:
 */

#define DCP_ARGV_NO_DEFAULTS            0x1
#define DCP_ARGV_NO_LEFTOVERS           0x2
#define DCP_ARGV_NO_ABBREV              0x4
#define DCP_ARGV_DONT_SKIP_FIRST_ARG    0x8
#define DCP_ARGV_FOUND_ONE              0x10
#define DCP_ARGV_OPERATIONS             0x20
#define DCP_ARGV_NO_OPT_LEFTOVERS       0x40


/*
 *  Constants 
 */

#define UUID_STRING_LEN    37
#define DEFAULT_SHELL      "/bin/sh"
#define DCECP_SCRIPT_DIR   "dcecp"      /* relative to dcelocal_path  */
#define DCECP_PATH_MAX     1024         /* To replace _POSIX_PATH_MAX */

/*
 * Exported procedures and variables.
 */

int dcp_ParseArgv (Tcl_Interp *interp, int *argcPtr, char **argv,
                   dcp_ArgvInfo *argTable, int flags);
int dcp_initInterp(Tcl_Interp *interp);


/* 
 * Some handy macros for setting and returning result codes.
 */
#define DCP_SET_MISC_ERROR(code) {                                      \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    return TCL_ERROR;                                                   \
}

#define DCP_SET_RESULT_CODE(code)                                       \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC); 

#define DCP_SET_RESULT_CODE_ONE_ARG(code, arg1)                         \
    Tcl_SetResult(interp, (char *)dce_sprintf(code, arg1), TCL_DYNAMIC);

#define DCP_SET_RESULT(string)                                          \
    Tcl_SetResult(interp, string, TCL_DYNAMIC); 

#define DCP_APPEND_RESULT_CODE(code) {                                  \
    char *tmp;                                                          \
    tmp = (char *)dce_sprintf(code);                                    \
    if (interp->result[0] == '\0')                                      \
        Tcl_AppendResult(interp, tmp, (char *)NULL);                    \
    else                                                                \
        Tcl_AppendResult(interp, "\n", tmp, (char *)NULL);              \
    free(tmp);                                                          \
}

#define DCP_APPEND_RESULT_CODE_ONE_ARG(code, arg1) {                    \
    char *tmp;                                                          \
    tmp = (char *)dce_sprintf(code, arg1);                              \
    if (interp->result[0] == '\0')                                      \
        Tcl_AppendResult(interp, tmp, (char *)NULL);                    \
    else                                                                \
        Tcl_AppendResult(interp, "\n", tmp, (char *)NULL);              \
    free(tmp);                                                          \
}

#define DCP_APPEND_RESULT(string) {                                     \
  if (interp->result[0] == '\0')                                        \
    Tcl_AppendResult(interp, string, (char *)NULL);                     \
  else                                                                  \
    Tcl_AppendResult(interp, "\n", string, (char *)NULL);               \
}

#define DCP_CHECK_RPC_ERROR(code) {                                     \
  if (code != rpc_s_ok) {                                               \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_RPC_ERROR_FREE_ONE(code, arg1) {                      \
  if (code != rpc_s_ok) {                                               \
    free((char *)arg1);                                                 \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_RPC_ERROR_FREE_TWO(code, arg1, arg2) {                \
  if (code != rpc_s_ok) {                                               \
    free((char *)arg1);                                                 \
    free((char *)arg2);                                                 \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_RPC_ERROR_FREE_THREE(code, arg1) {                    \
  if (code != rpc_s_ok) {                                               \
    free((char *)arg1);                                                 \
    free((char *)arg2);                                                 \
    free((char *)arg3);                                                 \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_SEC_ERROR(code) {                                     \
  if (code != sec_rgy_status_ok) {                                      \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_CDS_ERROR(code) {                                     \
  if (code != CDS_SUCCESS) {                                            \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_UUID_ERROR(code) {                                    \
  if (code != uuid_s_ok) {                                              \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    return TCL_ERROR;                                                   \
  }                                                                     \
}


#define DCP_CHECK_EXTRA_ARGS()                                          \
    /* Check for extraneous arguments */                                \
    if (argc > 2) {                                                     \
        char *tmp;                                                      \
        if (argv[2][0] == '-') {                                        \
            tmp = (char *)dce_sprintf(mcp_s_bad_opt, argv[2]);          \
        }                                                               \
        else {                                                          \
            tmp = (char *)dce_sprintf(mcp_s_bad_arg,argv[2]);           \
        }                                                               \
        Tcl_SetResult(interp, tmp, TCL_DYNAMIC);                        \
        return TCL_ERROR;                                               \
    }

#define DCP_CHECK_EXTRA_ARGS_NO_ARG()                                   \
    /* Check for extraneous arguments on command that takes no non-options */ \
    if (argc > 1) {                                                     \
        char *tmp;                                                      \
        if (argv[1][0] == '-') {                                        \
            tmp = (char *)dce_sprintf(mcp_s_bad_opt, argv[1]);          \
        }                                                               \
        else {                                                          \
            tmp = (char *)dce_sprintf(mcp_s_bad_arg,argv[1]);           \
        }                                                               \
        Tcl_SetResult(interp, tmp, TCL_DYNAMIC);                        \
        return TCL_ERROR;                                               \
    }
/* 
 * Macros for obtaining dcecp convenience variables.
 */
#define DCP_GENERAL_GET_S_VAR(index)                                 \
    Tcl_GetVar2(interp, DCP_C_GENERAL_SERVER_NAME, (char *)(index), TCL_GLOBAL_ONLY);

#define DCP_GENERAL_GET_B_VAR(index)                                 \
    Tcl_GetVar2(interp, DCP_C_GENERAL_BOUND_SERVER, (char *)(index), TCL_GLOBAL_ONLY);

/* 
 * Macros for setting dcecp convenience variables.
 */
#define DCP_GENERAL_SET_NAMELIST(element)                       \
    Tcl_SetVar(interp, DCP_C_GENERAL_NAME_LIST, (char *)(element), TCL_GLOBAL_ONLY);

#define DCP_GENERAL_SET_S_VAR(index, element)                   \
    Tcl_SetVar2(interp, DCP_C_GENERAL_SERVER_NAME, (char *)(index), (char *)(element), TCL_GLOBAL_ONLY);

#define DCP_GENERAL_SET_B_VAR(index, element)                   \
    dcp_general_set_var(interp, DCP_C_GENERAL_BOUND_SERVER, (char *)index, (char *)(element), 0);

#define DCP_GENERAL_SET_P_VAR(element)                          \
    dcp_general_set_var(interp, DCP_C_GENERAL_PARENT_NAME, (char *)NULL, (char *)(element), 0);

#define DCP_GENERAL_SET_O_VAR(element)                          \
    dcp_general_set_var(interp, DCP_C_GENERAL_OBJECT, (char *)NULL, (char *)(element), 0);

#define DCP_GENERAL_SET_R_VAR(element)                          \
    dcp_general_set_var(interp, DCP_C_GENERAL_RETURN_VALUE, (char *)NULL, (char *)(element), 0);

#define DCP_GENERAL_SET_U_VAR(element)                          \
    dcp_general_set_var(interp, DCP_C_GENERAL_PRINCIPAL, (char *)NULL, (char *)(element), 0);

#define DCP_GENERAL_SET_C_VAR(element)                          \
    dcp_general_set_var(interp, DCP_C_GENERAL_PRINCIPAL_CELL, (char *)NULL, (char *)(element), 0);

#define DCP_GENERAL_SET_H_VAR(element)                          \
    dcp_general_set_var(interp, DCP_C_GENERAL_HOSTNAME, (char *)NULL, (char *)(element), 0);

/* 
 * Macros for unsetting dcecp convenience variables.
 */
#define DCP_GENERAL_UNSET_NAMELIST()				\
    Tcl_UnsetVar(interp, DCP_C_GENERAL_NAME_LIST, TCL_GLOBAL_ONLY);

#define DCP_GENERAL_UNSET_S_VAR(index)				\
    Tcl_UnsetVar2(interp, DCP_C_GENERAL_SERVER_NAME, (char *)(index), TCL_GLOBAL_ONLY);

#define DCP_GENERAL_UNSET_B_VAR(index)				\
    dcp_general_unset_var(interp, DCP_C_GENERAL_BOUND_SERVER, (char *)index, 0);

#define DCP_GENERAL_UNSET_P_VAR()                               \
    dcp_general_unset_var(interp, DCP_C_GENERAL_PARENT_NAME, (char *)NULL, 0);

#define DCP_GENERAL_UNSET_O_VAR()                               \
    dcp_general_unset_var(interp, DCP_C_GENERAL_OBJECT, (char *)NULL, 0);

#define DCP_GENERAL_UNSET_R_VAR()                               \
    dcp_general_unset_var(interp, DCP_C_GENERAL_RETURN_VALUE, (char *)NULL, 0);

#define DCP_GENERAL_UNSET_U_VAR()                               \
    dcp_general_unset_var(interp, DCP_C_GENERAL_PRINCIPAL, (char *)NULL, 0);

#define DCP_GENERAL_UNSET_C_VAR()                               \
    dcp_general_unset_var(interp, DCP_C_GENERAL_PRINCIPAL_CELL, (char *)NULL, 0);

#define DCP_GENERAL_UNSET_H_VAR()                               \
    dcp_general_unset_var(interp, DCP_C_GENERAL_HOSTNAME, (char *)NULL, 0);


#endif /* _MOSCP_H */

