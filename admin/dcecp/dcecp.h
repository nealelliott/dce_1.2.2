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
 * $Log: dcecp.h,v $
 * Revision 1.1.9.2  1996/02/18  23:31:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:37  marty]
 *
 * Revision 1.1.9.1  1995/12/08  21:20:20  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/10/20  21:21 UTC  jrr
 * 	Add _conf for CDS confidence.
 * 	[1995/10/20  21:10 UTC  jrr  /main/HPDCE02/jrr_dcecp_01/1]
 * 
 * 	HP revision /main/HPDCE02/4  1995/03/24  21:51 UTC  jrr
 * 	Change DCP_SET_RESULT to use TCL_VOLATILE instead of TCL_DYNAMIC.
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_volitile/1  1995/03/24  21:49 UTC  jrr
 * 	Fix DCP_SET_RESULT() to usue TCL_VOLATILE instead of TCL_DYNAMIC.
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/21  16:18 UTC  truitt
 * 	Merge convenience variable work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts14/1  1995/02/21  16:17 UTC  truitt
 * 	CHFts14390: Add two variables which will indicate whether
 * 	the current command sets the _o or _e variables.
 * 
 * 	HP revision /main/HPDCE02/2  1995/02/14  16:03 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  16:02 UTC  truitt
 * 	CHFts14285: Condense a couple of the macros to make
 * 	them a bit smaller.
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/10  19:36 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/truitt_dcecp_chfts11/1  1995/02/10  19:33 UTC  truitt
 * 	CHFts14285 partial: Add support to pass the error
 * 	code back to main, instead of just the error message.
 * 	[1995/12/08  17:55:05  root]
 * 
 * Revision 1.1.7.24  1994/08/31  23:03:54  salamone
 * 	CR11916 - Add _b(sec) support.
 * 	[1994/08/31  23:02:19  salamone]
 * 
 * Revision 1.1.7.23  1994/08/25  15:51:13  rousseau
 * 	Change _POSIX_PATH_MAX to DCECP_PATH_MAX (CR 11866).
 * 	[1994/08/25  15:50:51  rousseau]
 * 
 * Revision 1.1.7.22  1994/08/08  21:04:24  melman
 * 	put include dce_utils.h in dcecp.h
 * 	[1994/08/08  21:01:47  melman]
 * 
 * Revision 1.1.7.21  1994/08/01  20:41:24  rousseau
 * 	Add _h support (CR 11285).
 * 	[1994/08/01  20:40:58  rousseau]
 * 
 * Revision 1.1.7.20  1994/06/24  17:29:25  rousseau
 * 	Added DCP_SET_RESULT_CODE_ONE_ARG.
 * 	[1994/06/24  17:29:10  rousseau]
 * 
 * Revision 1.1.7.19  1994/06/20  19:44:47  coren
 * 	Added DCP_CHECK_EXTRA_ARGS_NO_ARG macro for checking for extra arguments
 * 	in a command that takes no non-option arguments.
 * 	[1994/06/10  21:48:59  coren]
 * 
 * Revision 1.1.7.18  1994/06/09  16:01:52  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:17  devsrc]
 * 
 * Revision 1.1.7.17  1994/06/06  15:46:02  rousseau
 * 	Added new APPEND macro.
 * 	[1994/06/06  15:41:47  rousseau]
 * 
 * Revision 1.1.7.16  1994/05/26  17:56:29  salamone
 * 	Remove flag to leave error message in the interp result
 * 	when obtaining and setting _s. This affects macros:
 * 	DCP_GENERAL_SET_S_VAR() and DCP_GENERAL_GET_S_VAR()
 * 	This avoids using the Tcl_ResetResult() function.
 * 	[1994/05/26  16:37:18  salamone]
 * 
 * Revision 1.1.7.15  1994/05/11  19:27:11  kevins
 * 	OT 10097 added arg checking macro
 * 	[1994/05/11  15:27:07  kevins]
 * 
 * Revision 1.1.7.14  1994/05/10  17:26:07  salamone
 * 	Add macro to read _s variable
 * 	[1994/05/10  16:18:14  salamone]
 * 
 * Revision 1.1.7.13  1994/05/09  19:26:04  rousseau
 * 	Added DCP_CHECK_RPC_ERROR_FREE_*() macros.
 * 	[1994/05/09  19:24:33  rousseau]
 * 
 * 	Merge up.
 * 	[1994/05/05  13:49:39  rousseau]
 * 
 * Revision 1.1.7.12  1994/05/04  21:11:24  salamone
 * 	Have another TCL global variable follow each read-only
 * 	convenience variable. This solution fulfills the need
 * 	for multiple interpreters used within dcecp.
 * 	[1994/05/04  21:08:16  salamone]
 * 
 * Revision 1.1.7.11  1994/05/02  17:27:35  pwang
 * 	Added a new flag DCP_ARGV_NO_OPT_LEFTOVERS to dcp_ParseArgv
 * 	[1994/05/02  17:20:59  pwang]
 * 
 * Revision 1.1.7.10  1994/04/21  12:55:25  rousseau
 * 	Changed macros to free memory correctly (CR 10293).
 * 	[1994/04/21  12:55:05  rousseau]
 * 
 * Revision 1.1.7.9  1994/03/31  22:07:10  salamone
 * 	CR10259 - Remove global C variables for dcecp convenience variable support.
 * 	[1994/03/31  21:15:31  salamone]
 * 
 * Revision 1.1.7.8  1994/03/29  17:48:46  salamone
 * 	CR9854  - Remove malloc of fixed length convenience variables.
 * 	CR10087 - Provide list support of $_n
 * 	[1994/03/29  17:47:00  salamone]
 * 
 * Revision 1.1.7.7  1994/03/08  15:49:41  melman
 * 	init.dcecp and verb commands
 * 	[1994/03/08  14:46:03  melman]
 * 
 * Revision 1.1.7.6  1994/02/10  21:46:57  rousseau
 * 	Fixes for help facility problems. CR's 9917 9918 9919 9920
 * 	[1994/02/10  21:43:39  rousseau]
 * 
 * Revision 1.1.7.5  1994/02/02  20:00:42  rousseau
 * 	Added help macros.
 * 	[1994/02/02  19:59:53  rousseau]
 * 
 * Revision 1.1.7.4  1994/01/06  16:54:01  rousseau
 * 	Added #include of dce/audit.h .
 * 	[1994/01/06  16:53:55  rousseau]
 * 
 * Revision 1.1.7.3  1994/01/06  16:02:45  rousseau
 * 	Added DCP_CHECK_AUD_ERROR macro.
 * 	[1994/01/06  16:02:40  rousseau]
 * 
 * Revision 1.1.7.2  1993/12/20  20:36:48  kevins
 * 	Merged with changes from 1.1.7.1
 * 	[1993/12/20  20:36:38  kevins]
 * 
 * 	Added error setting macro
 * 	[1993/12/16  20:35:33  kevins]
 * 
 * Revision 1.1.7.1  1993/12/20  15:52:46  salamone
 * 	Remove length parameter from dcp_general_set_namelist()
 * 	[1993/12/17  20:03:55  salamone]
 * 
 * Revision 1.1.2.9  1993/12/14  17:18:16  salamone
 * 	Added macros for dcecp convenience variable support
 * 	[1993/12/14  17:17:54  salamone]
 * 
 * Revision 1.1.2.8  1993/11/11  01:56:10  rousseau
 * 	Moved global_context_list and associated mutexes to dcecp_login.h
 * 	[1993/11/11  01:56:04  rousseau]
 * 
 * Revision 1.1.2.7  1993/11/09  18:18:45  rousseau
 * 	Changed prototype for dcp_initInterp().
 * 	[1993/11/09  18:18:37  rousseau]
 * 
 * Revision 1.1.2.6  1993/11/05  15:37:31  rousseau
 * 	Changed DCP_APPEND_RESULT_CODE to append "\n" correctly.
 * 	Added dcp_login_context_s_t.
 * 	[1993/11/05  15:37:01  rousseau]
 * 
 * Revision 1.1.2.5  1993/10/28  14:16:39  rousseau
 * 	Added DCP_CHECK_UUID_ERROR.
 * 	[1993/10/28  14:16:30  rousseau]
 * 
 * Revision 1.1.2.4  1993/10/26  14:21:19  rousseau
 * 	Included rpcbase.h for sec_rgy_status_ok .
 * 	[1993/10/26  14:21:14  rousseau]
 * 
 * Revision 1.1.2.3  1993/10/26  14:01:59  rousseau
 * 	Added some macros to handle DCE error codes.
 * 	[1993/10/26  14:01:48  rousseau]
 * 
 * Revision 1.1.2.2  1993/10/20  14:58:48  rousseau
 * 	Changed dcp_ArgvInfo help element to unsigned32.
 * 	[1993/10/20  14:58:33  rousseau]
 * 
 * Revision 1.1.2.1  1993/10/18  19:27:02  rousseau
 * 	Conversion from dsh to dcecp.
 * 	[1993/10/18  19:26:52  rousseau]
 * 
 * $EndLog$
 */

/*
 * PRE-HISTORY
 * Revision 1.1.2.8  1993/10/18  00:00:58  rousseau
 *      Added DEFAULT_SHELL for dcp_shell
 *      [1993/10/18  00:00:16  rousseau]
 *
 * Revision 1.1.2.7  1993/09/07  23:35:24  melman
 *      Added operations command
 *      [1993/09/07  23:35:08  melman]
 * 
 * Revision 1.1.2.6  1993/08/03  13:57:01  kevins
 *      *** empty log message ***
 * 
 * Revision 1.1.2.5  1993/08/02  19:14:33  melman
 *      Initial version of updated rpccp functionality
 *      [1993/08/02  19:12:33  melman]
 * 
 * Revision 1.1.2.4  1993/07/26  17:02:45  melman
 *      Removed alot of unused TK stuff
 *      [1993/07/26  17:01:48  melman]
 * 
 * Revision 1.1.2.3  1993/07/26  15:27:34  kevins
 *      added information for Tk_ParseArgv
 *      [1993/07/26  15:22:17  kevins]
 * 
 * Revision 1.1.2.2  1993/07/23  20:02:35  kevins
 *      Initial submission of dsh
 *      [1993/07/23  19:35:30  kevins]
 * 
 */

#ifndef _DCECP_H
#define _DCECP_H

#include <stddef.h>
#include <pthread.h>
#include <dce/rpc.h>
#include <dce/tcl.h>
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
#define DCP_CDS_NUM_CONF_LEVEL 3 

/*
 * Exported procedures and variables.
 */

int dcp_ParseArgv (Tcl_Interp *interp, int *argcPtr, char **argv,
                   dcp_ArgvInfo *argTable, int flags);
int dcp_initInterp(Tcl_Interp *interp);

extern boolean32 was_e_var_set;
extern boolean32 was_o_var_set;

/* 
 * Some handy macros for setting and returning result codes.
 */
#define DCP_SET_MISC_ERROR(code) {                                      \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    DCP_GENERAL_SET_E_VAR(code);                                        \
    return TCL_ERROR;                                                   \
}

#define DCP_SET_RESULT_CODE(code) {                                     \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    DCP_GENERAL_SET_E_VAR(code);                                        \
}

#define DCP_SET_RESULT_CODE_ONE_ARG(code, arg) {                        \
    Tcl_SetResult(interp, (char *)dce_sprintf(code, arg), TCL_DYNAMIC); \
    DCP_GENERAL_SET_E_VAR(code);                                        \
}

#define DCP_SET_RESULT(string)                                          \
    Tcl_SetResult(interp, string, TCL_VOLATILE); 

#define DCP_APPEND_RESULT_CODE(code) {                                  \
    char *tmp;                                                          \
    tmp = (char *)dce_sprintf(code);                                    \
    if (interp->result[0] == '\0')                                      \
        Tcl_AppendResult(interp, tmp, (char *)NULL);                    \
    else                                                                \
        Tcl_AppendResult(interp, "\n", tmp, (char *)NULL);              \
    free(tmp);                                                          \
    DCP_GENERAL_SET_E_VAR(code);                                        \
}

#define DCP_APPEND_RESULT_CODE_ONE_ARG(code, arg1) {                    \
    char *tmp;                                                          \
    tmp = (char *)dce_sprintf(code, arg1);                              \
    if (interp->result[0] == '\0')                                      \
        Tcl_AppendResult(interp, tmp, (char *)NULL);                    \
    else                                                                \
        Tcl_AppendResult(interp, "\n", tmp, (char *)NULL);              \
    free(tmp);                                                          \
    DCP_GENERAL_SET_E_VAR(code);                                        \
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
    DCP_GENERAL_SET_E_VAR(code);                                        \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_RPC_ERROR_FREE_ONE(code, arg1) {                      \
  if (code != rpc_s_ok) {                                               \
    free((char *)arg1);                                                 \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    DCP_GENERAL_SET_E_VAR(code);                                        \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_SEC_ERROR(code) {                                     \
  if (code != sec_rgy_status_ok) {                                      \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    DCP_GENERAL_SET_E_VAR(code);                                        \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_CDS_ERROR(code) {                                     \
  if (code != CDS_SUCCESS) {                                            \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    DCP_GENERAL_SET_E_VAR(code);                                        \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_UUID_ERROR(code) {                                    \
  if (code != uuid_s_ok) {                                              \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    DCP_GENERAL_SET_E_VAR(code);                                        \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_EXTRA_ARGS()                                                               \
    /* Check for extraneous arguments */                                                     \
    if (argc > 2) {                                                                          \
        if (argv[2][0] == '-') {                                                             \
            Tcl_SetResult(interp, (char *)dce_sprintf(dcp_s_bad_opt, argv[2]), TCL_DYNAMIC); \
            DCP_GENERAL_SET_E_VAR(dcp_s_bad_opt);                                            \
        }                                                                                    \
        else {                                                                               \
            Tcl_SetResult(interp, (char *)dce_sprintf(dcp_s_bad_arg, argv[2]), TCL_DYNAMIC); \
            DCP_GENERAL_SET_E_VAR(dcp_s_bad_arg);                                            \
        }                                                                                    \
        return TCL_ERROR;                                                                    \
    }

#define DCP_CHECK_EXTRA_ARGS_NO_ARG()                                                        \
    /* Check for extraneous arguments on command with no non-options */                      \
    if (argc > 1) {                                                                          \
        if (argv[1][0] == '-') {                                                             \
            Tcl_SetResult(interp, (char *)dce_sprintf(dcp_s_bad_opt, argv[1]), TCL_DYNAMIC); \
            DCP_GENERAL_SET_E_VAR(dcp_s_bad_opt);                                            \
        }                                                                                    \
        else {                                                                               \
            Tcl_SetResult(interp, (char *)dce_sprintf(dcp_s_bad_arg, argv[1]), TCL_DYNAMIC); \
            DCP_GENERAL_SET_E_VAR(dcp_s_bad_arg);                                            \
        }                                                                                    \
        return TCL_ERROR;                                                                    \
    }

#define DCP_CHECK_EXTRA_ARGS_TWO_ARG()                                                       \
    /* Check for extraneous arguments on command with extra arg */                           \
    if (argc > 3) {                                                                          \
        if (argv[3][0] == '-') {                                                             \
            Tcl_SetResult(interp, (char *)dce_sprintf(dcp_s_bad_opt, argv[3]), TCL_DYNAMIC); \
            DCP_GENERAL_SET_E_VAR(dcp_s_bad_opt);                                            \
        }                                                                                    \
        else {                                                                               \
            Tcl_SetResult(interp, (char *)dce_sprintf(dcp_s_bad_arg, argv[3]), TCL_DYNAMIC); \
            DCP_GENERAL_SET_E_VAR(dcp_s_bad_arg);                                            \
        }                                                                                    \
        return TCL_ERROR;                                                                    \
    }

/* 
 * Macros for obtaining dcecp convenience variables.
 */
#define DCP_GENERAL_GET_S_VAR(index)                                 \
    Tcl_GetVar2(interp, DCP_C_GENERAL_SERVER_NAME, (char *)(index), TCL_GLOBAL_ONLY);

#define DCP_GENERAL_GET_B_VAR(index)                                 \
    Tcl_GetVar2(interp, DCP_C_GENERAL_BOUND_SERVER, (char *)(index), TCL_GLOBAL_ONLY);

#define DCP_GENERAL_GET_E_VAR()                                      \
    Tcl_GetVar(interp, DCP_C_GENERAL_ERROR_CODE, TCL_GLOBAL_ONLY);

/* 
 * Macros for setting dcecp convenience variables.
 */
#define DCP_GENERAL_SET_NAMELIST(element)                       \
    Tcl_SetVar(interp, DCP_C_GENERAL_NAME_LIST, (char *)(element), TCL_GLOBAL_ONLY);

#define DCP_GENERAL_SET_S_VAR(index, element)                   \
    Tcl_SetVar2(interp, DCP_C_GENERAL_SERVER_NAME, (char *)(index), (char *)(element), TCL_GLOBAL_ONLY);

#define DCP_GENERAL_SET_B_VAR(index, element)                   \
    dcp_general_set_var(interp, DCP_C_GENERAL_BOUND_SERVER, (char *)index, (char *)(element), 0);

#define DCP_GENERAL_SET_E_VAR(element) {                                                       \
    char *errcode;                                                                             \
    was_e_var_set = TRUE;                                                                      \
    errcode = malloc(sizeof(char) * 32);                                                       \
    sprintf(errcode, "%d", element);                                                           \
    dcp_general_set_var(interp, DCP_C_GENERAL_ERROR_CODE, (char *)NULL, (char *)(errcode), 0); \
    free(errcode);                                                                             \
}

#define DCP_GENERAL_SET_P_VAR(element)                          \
    dcp_general_set_var(interp, DCP_C_GENERAL_PARENT_NAME, (char *)NULL, (char *)(element), 0);

#define DCP_GENERAL_SET_O_VAR(element) {                                                   \
    was_o_var_set = TRUE;                                                                  \
    dcp_general_set_var(interp, DCP_C_GENERAL_OBJECT, (char *)NULL, (char *)(element), 0); \
}

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

#define DCP_GENERAL_UNSET_E_VAR()                               \
    dcp_general_unset_var(interp, DCP_C_GENERAL_ERROR_CODE, (char *)NULL, 0);

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

#endif /* _DCECP_H */
