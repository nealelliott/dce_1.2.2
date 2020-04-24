/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: util_help.h,v $
 * Revision 1.2  1996/12/03  10:41:20  salamone
 * Fix headers
 *
 * Revision 1.1  96/10/07  16:40:43  16:40:43  wwang (Weidong Wang)
 * Initial revision
 * 
 * Revision 1.1  1996/10/01  17:07:12  wwang
 * Initial revision
 *
 * Revision 1.1.2.5  1994/10/01  00:42:15  rousseau
 * 	Add help support to login and resolve (CR 12441).
 * 	[1994/10/01  00:42:03  rousseau]
 *
 * Revision 1.1.2.4  1994/08/18  17:43:54  rousseau
 * 	Implemented help -verbose (CR 11559).
 * 	[1994/08/18  17:42:41  rousseau]
 * 
 * Revision 1.1.2.3  1994/06/09  16:02:37  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:14  devsrc]
 * 
 * Revision 1.1.2.2  1994/02/10  21:46:59  rousseau
 * 	Fixes for help facility problems. CR's 9917 9918 9919 9920
 * 	[1994/02/10  21:43:42  rousseau]
 * 
 * Revision 1.1.2.1  1994/02/02  19:43:59  rousseau
 * 	Initial version.
 * 	[1994/02/02  19:33:06  rousseau]
 * 
 * $EndLog$
 */

/*
 * MODULE: util_help.h
 *
 * DESCRIPTION:
 *   This module contains prototypes and definitions used in the
 * dcecp help facilities.
 *
 */

#ifndef _UTIL_HELP_H
#define _UTIL_HELP_H

/* INCLUDE FILES  */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dcemcpmsg.h>
#include <tcl.h>
#include <moscp.h>

/* DECLARATIONS */

#define MAX_HELP_MESSAGE_SIZE   256
#define HELP_TOKEN              "help"

#define DCP_CHECK_OBJECT_HELP(verbose_tag) {                            \
    if (help_flag) {                                                    \
        return (dcp_object_help(clientData, interp,                     \
                                argc, argv, arg_table, verbose_tag));   \
    }                                                                   \
}

#define DCP_CHECK_COMMAND_HELP() {                                      \
    if (clientData != NULL) {                                           \
        int _i = 0;                                                     \
        char _tmp[MAX_HELP_MESSAGE_SIZE];                               \
        while (arg_table[_i].type != DCP_ARGV_END) {                    \
            sprintf(_tmp, "%-20s%s", arg_table[_i].key,                 \
                    (char *)dce_sprintf(arg_table[_i].helpcode));       \
            DCP_APPEND_RESULT(_tmp);                                    \
            _i++;                                                       \
        }                                                               \
        return TCL_OK;                                                  \
    }                                                                   \
}

#define DCP_CHECK_MISC_HELP(count) {                                    \
    char _tmp[MAX_HELP_MESSAGE_SIZE];                                   \
    int _i = 0;                                                         \
    for (_i = 0; _i < count; _i++) {                                    \
        sprintf(_tmp, "%-20s%s", arg_table[_i].key,                     \
                (char *)dce_sprintf(arg_table[_i].helpcode));           \
        DCP_APPEND_RESULT(_tmp);                                        \
    }                                                                   \
    return TCL_OK;                                                      \
}

#define DCP_NO_COMMAND_HELP(){                                          \
    if (clientData != NULL) {                                           \
        return TCL_OK;                                                  \
    }                                                                   \
}
    

/* INTERNAL VARIABLES */

/* PROTOTYPES */

int
dcp_object_help (ClientData, 
                 Tcl_Interp *, 
                 int, char **, 
                 dcp_ArgvInfo [],
                 unsigned32);

#endif
