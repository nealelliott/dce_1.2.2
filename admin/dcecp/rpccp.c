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
 * $Log: rpccp.c,v $
 * Revision 1.1.8.1  1996/08/09  11:50:56  arvind
 * 	Merge changes.
 * 	[1996/06/12  14:21 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/20  21:42 UTC  truitt  /main/HPDCE02/9]
 *
 * 	Merge changes.
 * 	[1996/03/20  21:41 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf70/1]
 *
 * 	CHFts16688: Bring on-line help for rpc commands into synch with
 * 	the way the code actually works.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/12  13:21 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.6.2  1996/02/18  19:14:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:04  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  21:22:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/8  1995/10/12  14:23 UTC  kevins
 * 	Merge CHFts16462 into DCEHP02
 * 
 * 	HP revision /main/HPDCE02/kevins_chfts16462/1  1995/10/09  19:50 UTC  kevins
 * 	CHFts16462 Incorrect binding interface parsing characteristics
 * 
 * 	HP revision /main/HPDCE02/7  1995/06/30  14:17 UTC  truitt
 * 	Merge 'import' changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts37/1  1995/06/30  14:14 UTC  truitt
 * 	CHFts15604: Remove -version option from 'import' commands.
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/26  18:09 UTC  jrr
 * 	Fix poor choice of function names in rpccp.c
 * 	[1995/05/24  18:31 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts15321/1]
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/23  19:13 UTC  truitt
 * 	Merge show header work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts30/1  1995/05/23  19:13 UTC  truitt
 * 	CHFts14679: Clean up show headers.
 * 
 * 	HP revision /main/HPDCE02/4  1995/04/06  13:56 UTC  jrr
 * 	Fix various memory leaks (CHFts14834).
 * 	[1995/04/05  14:32 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14834/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/03/27  19:35 UTC  truitt
 * 	Merge endpoint work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts22/1  1995/03/27  19:33 UTC  truitt
 * 	CHFts14656: Have 'endpoint create' check for an interface
 * 	id correctly.
 * 	CHFts14657: Have 'endpoint delete' check for an interface
 * 	id correctly.
 * 	CHFts14658: Allow the legal version strings to be abbreviated.
 * 	CHFts14659: Fix the 'endpoint show' message when an
 * 	illegal attribute is specified.
 * 	CHFts14660: The rpcentry 'show' output is slightly wrong.
 * 	CHFts14661: The rpcentry 'max' option can be specified with
 * 	bogus values.
 * 
 * 	HP revision /main/HPDCE02/2  1995/02/14  16:31 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  16:30 UTC  truitt
 * 	CHFts14285: When setting error codes, use the right
 * 	macros, to ensure the _e var sets set correctly.
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/03  20:21 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/11/03  20:19 UTC  truitt
 * 	Fix endpoint command switches.
 * 	[1995/12/08  17:57:14  root]
 * 
 * Revision 1.1.4.26  1994/09/29  22:44:22  melman
 * 	fix while (true) warning
 * 	[1994/09/29  22:42:49  melman]
 * 
 * Revision 1.1.4.25  1994/09/29  22:25:46  melman
 * 	removed resolve_binding()
 * 	[1994/09/29  22:20:00  melman]
 * 
 * Revision 1.1.4.24  1994/09/09  19:10:46  coren
 * 	Corrected misuse of "boolean" type in change for -uuid option.
 * 	[1994/09/09  18:20:43  coren]
 * 
 * 	Added -uuid option to endpoint_show. (CR 12109).
 * 	Made endpoint_show recognize host binding in tcl syntax (CR 11415).
 * 	Corrected calls to Tcl_AppendResult to pass  NULL rather than
 * 	TCL_STATIC for their last argument. (CR 12116)
 * 	[1994/09/09  16:48:44  coren]
 * 
 * Revision 1.1.4.23  1994/09/01  22:43:38  coren
 * 	Corrected test for mutually exclusive options in rpcprofile_add to
 * 	check for -annotation (CR 12010).
 * 	[1994/09/01  21:51:29  coren]
 * 
 * Revision 1.1.4.22  1994/08/22  21:49:36  coren
 * 	Inserted newlines into the output of show, list, and import
 * 	commands when operating on a list of objects (CR 11015).
 * 
 * 	Made import only report "no matching binding" error if there was no
 * 	match for any of the objects in a list (CR 11816).
 * 	[1994/08/22  20:57:59  coren]
 * 
 * Revision 1.1.4.21  1994/08/17  15:54:34  coren
 * 	Made "show" commands reject uuid without version (CR 10814).
 * 
 * 	Fixed dcp_rpc_make_binding_vector to correctly interpret lists of
 * 	bindings in either or both of string and TCL syntax (CR 10328).
 * 
 * 	Make "rpcentry show" accept interfaces in either or both syntaxes,
 * 	either singly or in lists (CR 10328).
 * 	[1994/08/17  14:03:00  coren]
 * 
 * Revision 1.1.4.20  1994/08/03  18:51:19  coren
 * 	Fixed output format of endpoint show (CR 11146).
 * 	[1994/08/03  17:19:12  coren]
 * 
 * Revision 1.1.4.19  1994/07/26  14:57:11  coren
 * 	Removed string-binding argument from "endpoint delete".
 * 	[1994/07/26  14:56:46  coren]
 * 
 * 	Added optional string-binding argument to "endpoint show" and "endpoint
 * 	delete" to allow access to non-local endpoint map (CR 11415).
 * 	[1994/07/26  14:15:07  coren]
 * 
 * Revision 1.1.4.18  1994/06/20  19:44:56  coren
 * 	Corrected endpoitn delete for null object case.
 * 	[1994/06/13  19:25:17  coren]
 * 
 * 	Removed tabs.
 * 	[1994/06/10  21:54:37  coren]
 * 
 * 	Added implementation of endpoint operations.
 * 	[1994/06/10  21:49:13  coren]
 * 
 * Revision 1.1.4.17  1994/06/09  16:02:27  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:02  devsrc]
 * 
 * Revision 1.1.4.16  1994/05/27  19:24:14  coren
 * 	Made rpcprofile add check for combination of -default with -interface
 * 	and/or -priority (CR 10758).
 * 	[1994/05/27  15:32:48  coren]
 * 
 * Revision 1.1.4.15  1994/05/27  15:36:05  kevins
 * 	added uuid compare and name compare commands.
 * 	[1994/05/27  15:35:10  kevins]
 * 
 * Revision 1.1.4.14  1994/05/25  19:34:18  rousseau
 * 	Merged with changes from 1.1.4.13
 * 	[1994/05/25  19:34:10  rousseau]
 * 
 * 	Make rpcprofile show annotations as a list (CR 10771).
 * 	[1994/05/25  19:33:29  rousseau]
 * 
 * Revision 1.1.4.13  1994/05/25  18:06:00  coren
 * 	Fixed "export" to use correct variables for stirng bindings (CR
 * 	10734).
 * 	Made output of "import" be a list of lists (CR 10733).
 * 	Corrected "rpcentry_show" to use the correct variable for object
 * 	UUIDs (CR 10756).
 * 	[1994/05/25  16:53:07  coren]
 * 
 * Revision 1.1.4.12  1994/05/11  23:32:23  melman
 * 	fixed missing arg to dce_sprintf
 * 	[1994/05/11  23:27:06  melman]
 * 
 * Revision 1.1.4.11  1994/05/11  19:27:13  kevins
 * 	OT 10097 transf arg checking macro to dcecp.h
 * 	[1994/05/11  15:28:05  kevins]
 * 
 * Revision 1.1.4.10  1994/05/09  21:21:22  melman
 * 	Check for -version without -interface
 * 	[1994/05/09  21:16:11  melman]
 * 
 * Revision 1.1.4.9  1994/04/29  18:45:56  melman
 * 	Fixed some stupid free mistakes
 * 	[1994/04/26  20:17:47  melman]
 * 
 * Revision 1.1.4.8  1994/04/20  20:26:06  melman
 * 	Removed antiquated name expand usage message
 * 	Removed redundant free's in error macros
 * 	Moved DCP_GENERAL_SET_NAMELIST calls after arg checking
 * 	Changed scoping of str in rpcprofile_show
 * 	[1994/04/20  20:20:42  melman]
 * 
 * Revision 1.1.4.7  1994/04/14  16:38:28  rousseau
 * 	Remove extra macro for CR 10350.
 * 	[1994/04/14  16:38:19  rousseau]
 * 
 * Revision 1.1.4.6  1994/04/11  21:28:26  melman
 * 	Merged with changes from 1.1.4.5
 * 	[1994/04/11  21:28:07  melman]
 * 
 * 	implemented rpc_profile_show
 * 	added free's for dce_sprintf calls
 * 	general free cleanup for groups and profiles
 * 	support tcl syntax of interfaces IDs
 * 	[1994/04/11  19:28:30  melman]
 * 
 * Revision 1.1.4.5  1994/03/31  22:07:13  salamone
 * 	CR10259 - Remove global C variables for dcecp convenience variable support.
 * 	[1994/03/31  21:15:12  salamone]
 * 
 * Revision 1.1.4.4  1994/03/29  21:42:05  melman
 * 	added convenience variable support
 * 	[1994/03/29  21:40:09  melman]
 * 
 * Revision 1.1.4.3  1994/03/28  23:00:25  melman
 * 	Added rpcentry show and binding utilities
 * 	[1994/03/28  22:53:50  melman]
 * 
 * Revision 1.1.4.2  1994/03/22  20:01:37  melman
 * 	rpccp error message cleanup
 * 	[1994/03/22  19:45:50  melman]
 * 
 * Revision 1.1.4.1  1994/03/17  21:52:39  melman
 * 	rpc command cleanup
 * 	[1994/03/17  20:46:27  melman]
 * 
 * Revision 1.1.2.4  1993/11/08  20:10:02  melman
 * 	don't include options.h
 * 	[1993/11/08  20:09:50  melman]
 * 
 * Revision 1.1.2.3  1993/10/22  17:06:34  rousseau
 * 	Changes for Tcl7.0
 * 	[1993/10/22  17:06:18  rousseau]
 * 
 * Revision 1.1.2.2  1993/10/20  20:48:51  rousseau
 * 	Changed help messages from dcp_s_ to dcp_t_
 * 	[1993/10/20  20:48:44  rousseau]
 * 
 * Revision 1.1.2.1  1993/10/20  15:00:08  rousseau
 * 	Conversion from dsh to dcecp. Move strings into message catalog.
 * 	[1993/10/20  14:59:54  rousseau]
 * 
 * $EndLog$
 */

/*
 * PRE-HISTORY
 * Revision 1.1.2.11  1993/09/14  23:57:10  melman
 * 	Added rpc-profile remove
 * 	[1993/09/14  23:56:53  melman]
 *
 * Revision 1.1.2.10  1993/09/09  23:12:15  melman
 * 	Added unexport
 * 	[1993/09/09  23:11:51  melman]
 * 
 * Revision 1.1.2.9  1993/09/08  22:43:45  melman
 * 	Added export command
 * 	[1993/09/08  22:43:32  melman]
 * 
 * Revision 1.1.2.8  1993/09/03  20:59:01  melman
 * 	Added rpc-profile list
 * 	[1993/09/03  20:58:49  melman]
 * 
 * Revision 1.1.2.7  1993/09/03  17:22:04  melman
 * 	Removed show_server and some cleanup
 * 	[1993/09/03  17:15:51  melman]
 * 
 * 	profile add and made expand_name_internal public
 * 	[1993/08/11  18:52:35  melman]
 * 
 * Revision 1.1.2.6  1993/08/06  21:51:52  melman
 * 	Added rpc-group list
 * 	[1993/08/06  21:51:26  melman]
 * 
 * Revision 1.1.2.5  1993/08/05  20:58:50  melman
 * 	addto and removefrom to add and remove
 * 	[1993/08/05  20:46:05  melman]
 * 
 * Revision 1.1.2.4  1993/08/04  20:49:11  melman
 * 	*** empty log message ***
 * 	[1993/08/04  20:47:34  melman]
 * 
 * Revision 1.1.2.3  1993/08/02  19:14:36  melman
 * 	Initial version of updated rpccp functionality
 * 	[1993/08/02  19:12:37  melman]
 * 
 * Revision 1.1.2.2  1993/07/23  20:03:26  kevins
 * 	Initial submission of dsh
 * 	[1993/07/23  19:36:10  kevins]
 * 
 */

#include <stdio.h>
#include <locale.h>
#include <setjmp.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>                     /* for INT_MAX */

#include <dce/dce.h>
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
#include <rpccp.h>
#include <dcecp.h>
#include <util_var.h>
#include <util_help.h>
#include <util_binding.h>
#include <dce/tcl.h>
#include <dce/ep.h>
#include <util_var.h>

#define DCP_CHECK_RPC_NO_ARG(type)                                      \
  if(argc < 2){                                                         \
     char *tmp;                                                         \
     tmp = (char *)dce_sprintf(dcp_s_rpc_no_ ## type ## _name);         \
     Tcl_SetResult(interp, tmp, TCL_DYNAMIC);                           \
     DCP_GENERAL_SET_E_VAR(dcp_s_rpc_no_ ## type ## _name);             \
     return TCL_ERROR;                                                  \
  }                                                                     \

#define DCP_CHECK_RPC_ERROR_LARGV(code)                                 \
  if (code != rpc_s_ok) {                                               \
      free((char *) largv);                                             \
      DCP_SET_RESULT_CODE(code);                                        \
      return TCL_ERROR;                                                 \
  }

#define DCP_CHECK_RPC_ERROR_LARGV_IF(code)                              \
    if (code != rpc_s_ok) {                                             \
        free((char *) largv);                                           \
        if (if_id_vector) {                                             \
            rpc_if_id_vector_free(&if_id_vector, &temp_status);         \
        }                                                               \
        DCP_SET_RESULT_CODE(code);                                      \
        return TCL_ERROR;                                               \
    }

#define DCP_CHECK_RPC_ERROR_LARGV_OBJ(code)                             \
    if (code != rpc_s_ok) {                                             \
        free((char *) largv);                                           \
        if (inquiry_context) {                                          \
            rpc_ns_entry_object_inq_done(&inquiry_context,              \
                                         &temp_status);                 \
        }                                                               \
        DCP_SET_RESULT_CODE(code);                                      \
        return TCL_ERROR;                                               \
    }

#define MAX_IF_ID_STRING_LENGTH         36+9+9+1
#define MAX_ANNOTATION_LENGTH           256
#define MAX_INTERFACES                  32
#define MAX_OBJECTS                     32
#define MAX_BINDINGS                    32
#define MAX_VERSION_VALUES              5

/* The NDR transfer syntax id */
static rpc_syntax_id_t NIDL_ndr_id = {
{0x8a885d04UL, 0x1ceb, 0x11c9, 0x9f, 0xe8, {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}},2};

struct interface_vec {
    unsigned32              count;
    rpc_if_id_t             if_id[MAX_INTERFACES];
};

struct uuid_vec {
    unsigned32              count;
    uuid_p_t                uuid[MAX_OBJECTS];
};

static char *version_name[] = {"all",
                               "compatible",
                               "exact",
                               "majoronly",
                               "upto"};

static unsigned32 version_val[] = {rpc_c_vers_all,
                                   rpc_c_vers_compatible,
                                   rpc_c_vers_exact,
                                   rpc_c_vers_major_only,
                                   rpc_c_vers_upto};

/*
 * Forward References
 */
static boolean32 if_id_in_vector(rpc_if_id_t *,
    struct interface_vec *, unsigned32 *);
static boolean32 uuid_in_vector(uuid_t *, struct uuid_vec *, unsigned32 *);
void dcp_rpc_parse_if_id(char *, rpc_if_id_t *, unsigned32 *);
void dcp_rpc_make_binding_vector(Tcl_Interp *, char *, rpc_binding_vector_t *,
    unsigned32 *);
void dcp_rpc_make_object_vector(Tcl_Interp *, char *, uuid_t *,
    uuid_vector_t *, unsigned32 *);

/***************************************************************************
 *
 * Operations
 *
 **************************************************************************/

int
dcp_rpc_uuid_create(ClientData clientData, Tcl_Interp *interp,
                    int argc, char **argv)
{
    unsigned32          rpc_status;
    unsigned32          temp_status;
    unsigned char      *uuid_str;
    uuid_t              uuid;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* This command accepts no options. If there are any arguments, fail. */
    if (argc != 1) {
        if (argv[1][0] == '-') {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_opt, argv[1]);
        }
        else {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_arg, argv[1]);
        }
        return TCL_ERROR;
    }

    uuid_create(&uuid, &rpc_status);
    DCP_CHECK_RPC_ERROR(rpc_status);

    uuid_to_string(&uuid, &uuid_str, &rpc_status);
    DCP_CHECK_RPC_ERROR(rpc_status);

    DCP_SET_RESULT((char *)uuid_str);
    rpc_string_free(&uuid_str, &temp_status);
    return TCL_OK;
}

int
dcp_rpc_uuid_compare(ClientData clientData, Tcl_Interp *interp,
                     int argc, char **argv)
{
    uuid_t     uuid1;
    uuid_t     uuid2;
    unsigned32 status;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    if (argc != 3) {
        if (argc > 3){
            if(argv[3][0] == '-') {
                DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_opt, argv[3]);
            }
            else {
                DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_arg, argv[3]);
            }
        }
        else {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_wrong_num_args, 2);
        }
        return TCL_ERROR;
    }
    uuid_from_string((unsigned_char_t *)argv[1],&uuid1,&status);
    DCP_CHECK_RPC_ERROR(status);
    uuid_from_string((unsigned_char_t *)argv[2],&uuid2,&status);
    DCP_CHECK_RPC_ERROR(status);
    if(uuid_equal(&uuid1,&uuid2,&status))
        Tcl_SetResult(interp,"1",TCL_VOLATILE);
    else
        Tcl_SetResult(interp,"0",TCL_VOLATILE);

    return TCL_OK;
}

int 
dcp_name_compare(ClientData clientData, Tcl_Interp *interp,
                 int argc, char **argv)
{
    unsigned32          status;
    unsigned char      *expanded1;
    unsigned char      *expanded2;
    unsigned32          name_syntax = rpc_c_ns_syntax_default;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    if (argc != 3) {
        if (argc > 3){
            if(argv[3][0] == '-') {
                DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_opt, argv[3]);
            }
            else {
                DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_arg, argv[3]);
            }
        }
        else  {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_wrong_num_args, 2);
        }
        return TCL_ERROR;
    }
    
    rpc_ns_entry_expand_name (name_syntax,
                              (unsigned char *)argv[1],
                              &expanded1,
                              &status);
    if (status != rpc_s_ok){   
        DCP_SET_RESULT_CODE(status);
        rpc_string_free(&expanded1, &status);
        return TCL_ERROR;
    }
    
    rpc_ns_entry_expand_name (name_syntax,
                              (unsigned char *)argv[2],
                              &expanded2,
                              &status);
    if (status != rpc_s_ok){   
        DCP_SET_RESULT_CODE(status);
        rpc_string_free(&expanded2, &status);
        return TCL_ERROR;
    }    
    if(!strcmp((char *)expanded2,(char *)expanded1))
        Tcl_SetResult(interp,"1",TCL_VOLATILE);
    else
        Tcl_SetResult(interp,"0",TCL_VOLATILE);

    rpc_string_free(&expanded1, &status);
    rpc_string_free(&expanded2, &status);

    return TCL_OK;
        
}

int 
dcp_name_expand(ClientData clientData, Tcl_Interp *interp,
                int argc, char **argv)
{
    unsigned32          rpc_status;
    unsigned32          temp_status;
    unsigned char      *expanded_p;
    unsigned32          name_syntax = rpc_c_ns_syntax_default;
    
    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(entry);

    rpc_ns_entry_expand_name (name_syntax,
                              (unsigned char *)argv[1],
                              &expanded_p,
                              &rpc_status);
    
    if (rpc_status == rpc_s_ok) {
        Tcl_SetResult(interp, (char *)expanded_p, TCL_VOLATILE);
        rpc_string_free(&expanded_p, &temp_status);
    }
    else {
        DCP_SET_RESULT_CODE(rpc_status);
        rpc_string_free(&expanded_p, &temp_status);
        return TCL_ERROR;
    }
    return TCL_OK;
}

int
dcp_rpcentry_create(ClientData clientData, Tcl_Interp *interp,
                    int argc, char **argv) 
{
    unsigned32      name_syntax = rpc_c_ns_syntax_default;
    unsigned32      rpc_status;
    int             j;              /* index for list argv */
    int             largc;          /* argc for the list */
    char          **largv;          /* argv for the list */
    
    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(entry);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    /* Treat argv[1] as a list and process each element */
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        /* do the real work */
        rpc_ns_mgmt_entry_create(name_syntax, 
                                 (unsigned char *)largv[j],
                                 &rpc_status);
        DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
    }
    free((char *) largv);
    return TCL_OK;
}

int
dcp_rpcentry_delete(ClientData clientData, Tcl_Interp *interp,
                    int argc, char **argv)
{
    unsigned32      name_syntax = rpc_c_ns_syntax_default;
    unsigned32      rpc_status;
    int             j;              /* index for list argv */
    int             largc;          /* argc for the list */
    char          **largv;          /* argv for the list */
    
    
    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(entry);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    /* Treat argv[1] as a list and process each element */
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        /* do the real work */
        rpc_ns_mgmt_entry_delete(name_syntax,
                                 (unsigned char *)largv[j],
                                 &rpc_status);
        DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
    }
    free((char *) largv);
    return TCL_OK;
}

/*
 * FUNCTION: if_id_in_vector
 *
 * OVERVIEW: return TRUE/FALSE if if_id_t is in interface_vec
 *           called by rpcentry_show()
 *
 * INPUTS:
 *   if_id              interface id to look for 
 *   if_vector          interface vector to look in
 *
 * OUTPUTS:
 *   rpc_status         status as usual (returned from uuid_equal)
 *
 * RETURNS:
 *   TRUE/FALSE         TRUE if interface id found in vector
 */
static boolean32 
if_id_in_vector(rpc_if_id_t *if_id,
                struct interface_vec *if_vector,
                unsigned32 *rpc_status)
{
    int i;
    int found;
    
    for (i=0; i<if_vector->count; i++) {
        found = uuid_equal(&(if_vector->if_id[i].uuid),
                           &(if_id->uuid),
                           rpc_status);
        if (*rpc_status != rpc_s_ok) {
            return FALSE;
        }
        
        if (found &&
            (if_vector->if_id[i].vers_major == if_id->vers_major) &&
            (if_vector->if_id[i].vers_minor == if_id->vers_minor)) {
            return TRUE;
        }
    }
    return FALSE;
    
}

static boolean32 
uuid_in_vector(uuid_t *uuid,
               struct uuid_vec *uuid_vector,
               unsigned32 *rpc_status)
{
    int i;
    
    for (i=0; i<uuid_vector->count; i++) {
        if (uuid_equal(uuid, uuid_vector->uuid[i], rpc_status)) {
            return TRUE;
        }
    }
    return FALSE;
}

int
dcp_rpcentry_show(ClientData clientData, Tcl_Interp *interp,
                  int argc, char **argv)
{
    boolean32               all_interfaces = FALSE;
    boolean32               all_objects = FALSE;
    boolean32               objects_found = FALSE;
    boolean32               update_data = TRUE;
    boolean32               single_tcl_interface = FALSE;
    boolean32               wrote_something = FALSE;
    char                    str[256];
    char                   *tcl_bind;
    char                   *interface_str = NULL;
    char                   *object_uuid_str = NULL;
    char                  **largv;          /* argv for the list */
    int                     j,k;
    int                     largc;          /* argc for the list */
    int                     uuid_count;
    char                    *noobj_svc;
    rpc_if_id_t             interface_id;
    rpc_if_id_vector_p_t    if_id_vector = NULL;
    rpc_ns_inq_handle_t     inquiry_context = NULL;
    unsigned32              name_syntax = rpc_c_ns_syntax_default;
    unsigned32              rpc_status;
    unsigned32              temp_status;
    unsigned_char_p_t       uuid_string = NULL;
    uuid_t                  object_uuid;
    uuid_t                  objects[MAX_OBJECTS];
    struct interface_vec    interface_vector;
    struct uuid_vec         object_vector;
    rpc_ns_handle_t         import_context;
    rpc_binding_handle_t    binding;
    unsigned_char_p_t       string_binding;
    
    rpc_if_rep_t    if_spec = {
        0,                  /* ifspec rep version                           */
        0,                  /* operation count (not needed)                 */
        0,                  /* interface version (to be filled in later)    */
        { 0 },              /* interface uuid (to be filled in later)       */
        2,                  /* stub/runtime interface version number        */
        { 0, NULL },        /* endpoint vector (not needed)                 */
        { 1, &NIDL_ndr_id}, /* NDR transfer syntax (the only one for now)   */
        NULL,               /* server stub epv (not needed)                 */
        NULL                /* manager stub epv (not needed)                */
        };

    dcp_ArgvInfo arg_table[] = {
        {"-interface", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rpc_interface_list_help},
        {"-noupdate", DCP_ARGV_CONSTANT, (char *)0, NULL, 
             dcp_t_rpc_noupdate_help},
        {"-object",    DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rpc_object_list_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&interface_str;
    arg_table[1].dst = (char *)&update_data;
    arg_table[2].dst = (char *)&object_uuid_str;
    
    /* initialized the interface_id */
    memset(&interface_id, 0, sizeof(rpc_if_id_t));
    object_vector.count = 0;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(entry);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    if (interface_str) {
        /* Treat the interface_str as a list of interfaces */
        Tcl_SplitList(interp, interface_str, &largc, &largv);
	if (largc == 2) {
	    /* this might be a single tcl syntax interface */
	    dcp_rpc_parse_if_id(interface_str,
                                &(interface_vector.if_id[0]),
                                &rpc_status);
	    if (rpc_status == rpc_s_ok) {
		interface_vector.count = 1;
		single_tcl_interface = TRUE;
	    }
	}

	if (!single_tcl_interface) {
	    for (j=0; j<largc; j++) {
		/* get the interface uuid string and the version numbers */
		dcp_rpc_parse_if_id(largv[j],
				    &(interface_vector.if_id[j]),
				    &rpc_status);
		DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
	    }                           /* for j in each largv*/
	    interface_vector.count = largc;
	}
        free((char *) largv);
    }
    else {
        all_interfaces = TRUE;
    }
    
    if (object_uuid_str) {
        dcp_rpc_make_object_vector(interp, object_uuid_str, &objects[0],
                                   (uuid_vector_t *)&object_vector,
                                   &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    }
    else {
        all_objects = TRUE;
    }
    
    /* Treat argv[1] as a list and process each element */
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        /* interfaces first */
        rpc_ns_mgmt_entry_inq_if_ids(name_syntax,
                                     (unsigned char *)largv[j],
                                     &if_id_vector,
                                     &rpc_status);
        
        if (rpc_status != rpc_s_no_interfaces_exported) {
            DCP_CHECK_RPC_ERROR_LARGV_IF(rpc_status);
            
	    if (wrote_something) {
		Tcl_AppendResult(interp, "\n", NULL);
	    }
	    wrote_something = FALSE;

            /* walk the vector looking for compatible interfaces */
            for (k=0; k<if_id_vector->count; k++) {
                if (all_interfaces || if_id_in_vector(if_id_vector->if_id[k],
                                                      &interface_vector,
                                                      &rpc_status)) {
                    /* got one, put in result */
                    uuid_to_string(&(if_id_vector->if_id[k]->uuid),
                                   &uuid_string,
                                   &rpc_status);
                    DCP_CHECK_RPC_ERROR_LARGV_IF(rpc_status);

                    sprintf(str, "{%s %d.%d",
                            (char *)uuid_string,
                            if_id_vector->if_id[k]->vers_major,
                            if_id_vector->if_id[k]->vers_minor);

                    if (interp->result[0] != '\0') {
                        Tcl_AppendResult(interp, "\n", NULL);
                    }
                    Tcl_AppendResult(interp, str, NULL);
		    wrote_something = TRUE;
                    rpc_string_free (&uuid_string, &temp_status);
                    
                    /* add all the bindings for this interface to result */
                    /* fill in the if spec */
                    if_spec.id = if_id_vector->if_id[k]->uuid;
                    if_spec.vers = (if_id_vector->if_id[k]->vers_minor << 16) |
                        if_id_vector->if_id[k]->vers_major;
                    
                    rpc_ns_binding_import_begin(name_syntax,
                                                (unsigned char *)largv[j],
                                                (rpc_if_handle_t) &if_spec,
                                                (object_vector.count) ? &(objects[0]) : NULL,
                                                &import_context,
                                                &rpc_status);
                    DCP_CHECK_RPC_ERROR_LARGV_IF(rpc_status);
                    
                    /*
                     * Check to determine if  the caller wants to update 
                     * the local name-service data. If so, set the 
                     * handle expiration age to bypass the local cache.
                     */
                    if (update_data) {
                        rpc_ns_mgmt_handle_set_exp_age(import_context,
                                                       0,
                                                       &rpc_status);
                        DCP_CHECK_RPC_ERROR_LARGV_IF(rpc_status);
                    }
                    
                    /*
                     * import until there are no more bindings available
                     * (or the number specified is reached)
                     */
                    do {
                        rpc_ns_binding_import_next(import_context,
                                                   &binding,
                                                   &rpc_status);
                        
                        if (rpc_status == rpc_s_no_more_bindings) {
                            break;
                        }
                        DCP_CHECK_RPC_ERROR_LARGV_IF(rpc_status);
                        
                        /*
                         * Remove the object UUID, if any, that's 
                         * automatically included by import. We only 
                         * want to display the string equivalent of 
                         * a tower - and it doesn't contain object 
                         * UUIDs. 
                         */
                        rpc_binding_set_object(binding, NULL, &rpc_status);
                        DCP_CHECK_RPC_ERROR_LARGV_IF(rpc_status);
                        
                        /* convert the binding to a string binding */
                        rpc_binding_to_string_binding(binding,
                                                      &string_binding,
                                                      &rpc_status);
                        DCP_CHECK_RPC_ERROR_LARGV_IF(rpc_status);
                        
                        tcl_bind=dcp_binding_string_to_tcl((char *)string_binding,
                                                           &rpc_status);

                        Tcl_AppendResult(interp, "\n {", tcl_bind, "}", NULL);
                        
                        /* free the string binding memory held by 
                         * the runtime */ 
                        /* free the binding memory held by the runtime */
                        free(tcl_bind);
                        rpc_string_free(&string_binding, &temp_status);
                        rpc_binding_free(&binding, &rpc_status);
                    } while (rpc_status != rpc_s_no_more_bindings);
                    rpc_ns_binding_import_done (&import_context, &rpc_status);
                    
                    Tcl_AppendResult(interp, "}", NULL);
                }                       /* interface okay to return */
            }                           /* for loop checking interfaces*/
        }                               /* if interfaces exported */
        
        if (if_id_vector) {
            rpc_if_id_vector_free(&if_id_vector, &temp_status);
        }
        
        /* objects second */
        /* set up an inquiry context */
        rpc_ns_entry_object_inq_begin(name_syntax,
                                      (unsigned char *)largv[j],
                                      &inquiry_context,
                                      &rpc_status);
        
        DCP_CHECK_RPC_ERROR_LARGV_OBJ(rpc_status);
        
        /*
         * Check to determine whether to update the 
         * local name-service data. If so, set the 
         * handle expiration age so to bypass the local 
         * cache. 
         */
        if (update_data) {
            rpc_ns_mgmt_handle_set_exp_age(inquiry_context, 0, &rpc_status);
            DCP_CHECK_RPC_ERROR_LARGV_OBJ(rpc_status);
        }
        
        /* inquire all of the objects */
        uuid_count = 0;
        objects_found=FALSE;
        do {
            rpc_ns_entry_object_inq_next(inquiry_context,
                                         &object_uuid,
                                         &rpc_status);
            
            if (rpc_status == rpc_s_no_more_members) {
                break;
            }
            DCP_CHECK_RPC_ERROR_LARGV_OBJ(rpc_status);
            
            /* check for match against the ones specified */
            if (!all_objects &&
                !uuid_in_vector(&object_uuid, &object_vector, &rpc_status)) {
                continue;
            }
            
            uuid_to_string(&object_uuid, &uuid_string, &rpc_status);
            DCP_CHECK_RPC_ERROR_LARGV_OBJ(rpc_status);
            
            if (interp->result[0] != '\0') {
                Tcl_AppendResult(interp, "\n", NULL);
            }
            if (!objects_found) {
                Tcl_AppendResult(interp, "{", NULL);
                objects_found = TRUE;
		wrote_something = TRUE;
            }
            if (uuid_count > 0) {
               Tcl_AppendResult(interp, "  ", NULL);
            }
            Tcl_AppendResult(interp, (char *)uuid_string, NULL);

            uuid_count++;
            rpc_string_free(&uuid_string, &temp_status);
        } while (rpc_status != rpc_s_no_more_members);

        if (objects_found) {
            Tcl_AppendResult(interp, "}", NULL);
        } else {
            /* if there is nothing then output "noobjects" */
            if (wrote_something) {
                noobj_svc = (char *)dce_sprintf(dcp_t_rpc_no_objects);
                if (noobj_svc == NULL) {
                    DCP_SET_RESULT_CODE(dcp_s_no_memory);
                    return TCL_ERROR;
                }
                Tcl_AppendResult(interp, "\n", noobj_svc, NULL);
                free(noobj_svc);
            }
        }
        
        if (inquiry_context) {
            rpc_ns_entry_object_inq_done(&inquiry_context, &temp_status);
        }
    }                                   /* for each j in argv[1] */
    free((char *) largv);
    return TCL_OK;
}

int
dcp_rpcgroup_add(ClientData clientData, Tcl_Interp *interp,
                 int argc, char **argv)
{
    unsigned32      name_syntax = rpc_c_ns_syntax_default;
    unsigned32      rpc_status;
    int             i,j;
    int             gargc;          /* argc for list of groups */
    char          **gargv;          /* argv for list of groups */
    int             margc;          /* argc for list of members */
    char          **margv;          /* argv for list of members */
    char           *member_name = NULL;
    dcp_ArgvInfo arg_table[] = {
        {"-member", DCP_ARGV_STRING, NULL, NULL, dcp_t_rpc_member_list_help},
        {NULL,      DCP_ARGV_END,    NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&member_name;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(group);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    if (member_name == NULL) {
        DCP_SET_RESULT_CODE(dcp_s_rpc_no_member_name);
        return TCL_ERROR; 
    }
    
    /* 
     * The group name is in argv[1], the value of member_name is 
     * treated as a list.  Both are split and each margv is added 
     * to each gargv 
     */        
    Tcl_SplitList(interp, argv[1], &gargc, &gargv);
    Tcl_SplitList(interp, member_name, &margc, &margv);
    for (i=0; i<gargc; i++) {
        for (j=0; j<margc; j++) {
            /* do the real work */
            rpc_ns_group_mbr_add(name_syntax,
                                 (unsigned char *)gargv[i],
                                 name_syntax,  
                                 (unsigned char *)margv[j],
                                 &rpc_status);
            if (rpc_status != rpc_s_ok) {
                free((char *) gargv);
                free((char *) margv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
        }
    }
    free((char *) gargv);
    free((char *) margv);
    return TCL_OK;
}

int
dcp_rpcgroup_delete(ClientData clientData, Tcl_Interp *interp,
                    int argc, char **argv)
{
    unsigned32      name_syntax = rpc_c_ns_syntax_default;
    unsigned32      rpc_status;
    int             j;              /* index for list argv */
    int             largc;          /* argc for the list */
    char          **largv;          /* argv for the list */
    
    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(group);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    /* Treat argv[1] as a list and process each element */
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        /* do the real work */
        rpc_ns_group_delete(name_syntax,
                            (unsigned char *)largv[j],
                            &rpc_status);
        DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
    }
    free((char *) largv);
    return TCL_OK;
}

int
dcp_rpcgroup_list(ClientData clientData, Tcl_Interp *interp,
                  int argc, char **argv)
{
    unsigned32            name_syntax = rpc_c_ns_syntax_default;
    char                 *member_name = NULL;
    boolean32             update_data = TRUE;
    unsigned32            rpc_status;
    unsigned32            temp_status;
    int                   result = TCL_OK;
    int                   j;              /* index for list argv */
    int                   largc;          /* argc for the list */
    char                **largv;          /* argv for the list */
    int                   m;              /* index for member list argv */
    int                   margc;          /* argc for the member list */
    char                **margv;          /* argv for the member list */
    char                **emargv;         /* argv for the expanded margv */
    rpc_ns_inq_handle_t   inquiry_context;
    char *                group_member_name;
    boolean32             members_found = FALSE;
    boolean32             wrote_something = FALSE;
    
    dcp_ArgvInfo arg_table[] = {
        {"-member",   DCP_ARGV_STRING, NULL, NULL, dcp_t_rpc_member_list_help},
        {"-noupdate", DCP_ARGV_CONSTANT, (char *)0, NULL,
             dcp_t_rpc_noupdate_help},
        {NULL,        DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&member_name;
    arg_table[1].dst = (char *)&update_data;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(group);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    /* 
     * if there were members specifed, get a list of canonical names 
     * to compare any results we get with.
     */
    if (member_name) {
        Tcl_SplitList(interp, member_name, &margc, &margv);
        emargv = (char **)malloc(margc*sizeof(char *));
        for (m=0; m<margc; m++) { 
            rpc_ns_entry_expand_name(name_syntax,
                                     (unsigned char *)margv[m],
                                     (unsigned char **)&emargv[m],
                                     &rpc_status);
            
            if (rpc_status != rpc_s_ok) {
                free((char *) margv);
                free((char *) emargv);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
        }
        free((char *) margv);
    }        

    /* Treat argv[1] as a list of groups and process each element */
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        /* do the real work */
        /* set up a context to get the members */
        rpc_ns_group_mbr_inq_begin(name_syntax,
                                   (unsigned char *)largv[j],
                                   name_syntax,
                                   &inquiry_context,
                                   &rpc_status);
        if (rpc_status != rpc_s_ok) {
            if (member_name) free((char *) emargv);
            free((char *) largv);
            DCP_SET_RESULT_CODE(rpc_status);
            return TCL_ERROR;
        }
        
        /* 
         * Check if the caller wants us to update the local 
         * name-service data. If so, set the handle expiration 
         * age so that we bypass the cache.
         */
        if (update_data) {
            rpc_ns_mgmt_handle_set_exp_age(inquiry_context, 0, &rpc_status);
            if (rpc_status != rpc_s_ok) {
                rpc_ns_group_mbr_inq_done(&inquiry_context, &temp_status);
                if (member_name) free((char *) emargv);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
        }
        
	if (wrote_something) {
	    Tcl_AppendResult(interp, "\n", NULL);
	}
	wrote_something = FALSE;
        do {
            rpc_ns_group_mbr_inq_next(inquiry_context,
                                      (unsigned char **)&group_member_name,
                                      &rpc_status);
            
            if (rpc_status == rpc_s_no_more_members) {
                break;
            }
            
            if (rpc_status != rpc_s_ok) {
                rpc_ns_group_mbr_inq_done(&inquiry_context, &temp_status);
                if (member_name) free((char *) emargv);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
            
            /* 
             * Comparison with group_member_name and emargv 
             * to see if we shouldn't return this name
             */
            if (member_name) {
                for (m=0; m<margc; m++) { 
                    if (!strcmp(emargv[m], group_member_name)) {
                        if (members_found == TRUE) {
                            Tcl_AppendResult(interp, "\n", NULL);
                        }
                        Tcl_AppendResult(interp, group_member_name, NULL);
                        members_found = TRUE;
			wrote_something = TRUE;
                        break;
                    }
                }
            } else {            /* all members are to be returned */
                if (members_found == TRUE) {
                    Tcl_AppendResult(interp, "\n", NULL);
                }
                Tcl_AppendResult(interp, group_member_name, NULL);
                members_found = TRUE;
		wrote_something = TRUE;
            }
            
            rpc_string_free((unsigned char **)&group_member_name, &temp_status);
            if (rpc_status != rpc_s_ok) {
                rpc_ns_group_mbr_inq_done(&inquiry_context, &temp_status);
                if (member_name) free((char *) emargv);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
        } while (rpc_status != rpc_s_no_more_members);
        
        /* clean up the context */
        rpc_ns_group_mbr_inq_done(&inquiry_context, &rpc_status);
        if (rpc_status != rpc_s_ok) {
            if (member_name) free((char *) emargv);
            free((char *) largv);
            DCP_SET_RESULT_CODE(rpc_status);
            return TCL_ERROR;
        }
    }                           /* for j in each largv*/
    if ((strlen((char *)interp->result) > 0) &&
	(interp->result[strlen((char *)interp->result) - 1] == '\n')) {
	interp->result[strlen((char *)interp->result) - 1] = '\0';
    }
    if (member_name) free((char *) emargv);
    free((char *) largv);
    return result;
}


int
dcp_rpcgroup_remove(ClientData clientData, Tcl_Interp *interp,
                    int argc, char **argv)
{
    unsigned32      name_syntax = rpc_c_ns_syntax_default;
    unsigned32      rpc_status;
    int             i,j;
    int             gargc;          /* argc for list of groups */
    char          **gargv;          /* argv for list of groups */
    int             margc;          /* argc for list of members */
    char          **margv;          /* argv for list of members */
    char           *member_name = NULL;
    dcp_ArgvInfo arg_table[] = {
        {"-member", DCP_ARGV_STRING, NULL, NULL, dcp_t_rpc_member_list_help},
        {NULL,      DCP_ARGV_END,    NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&member_name;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(group);

    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    if (member_name == NULL) {
        DCP_SET_RESULT_CODE(dcp_s_rpc_no_member_name);
        return TCL_ERROR; 
    }

    /* 
     * The group name is in argv[1], the value of member_name is 
     * treated as a list.  Both are split and each margv is added 
     * to each gargv 
     */        
    Tcl_SplitList(interp, argv[1], &gargc, &gargv);
    Tcl_SplitList(interp, member_name, &margc, &margv);
    for (i=0; i<gargc; i++) {
        for (j=0; j<margc; j++) {
            /* do the real work */
            rpc_ns_group_mbr_remove(name_syntax,
                                    (unsigned char *)gargv[i],
                                    name_syntax,  
                                    (unsigned char *)margv[j],
                                    &rpc_status);
            if (rpc_status != rpc_s_ok) {
                free((char *) gargv);
                free((char *) margv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
        }
    }
    free((char *) gargv);
    free((char *) margv);
    return TCL_OK;
}
/****************************************************************/


int
dcp_rpcprofile_add(ClientData clientData, Tcl_Interp *interp,
                   int argc, char **argv)
{
    unsigned32      name_syntax = rpc_c_ns_syntax_default;
    int             i,j;
    int             gargc;          /* argc for list of groups */
    char          **gargv;          /* argv for list of groups */
    int             margc;          /* argc for list of members */
    char          **margv;          /* argv for list of members */
    char           *member_name = NULL;
    boolean32       default_profile = FALSE;
    int             priority_int = 0;
    unsigned32      priority = 0;
    char           *annotation = NULL;
    char           *null_annotation = "";
    char           *interface_str = NULL;
    rpc_if_id_t     interface_id;
    unsigned32      rpc_status;
    dcp_ArgvInfo arg_table[] = {
        {"-annotation", DCP_ARGV_STRING,   NULL, NULL,
             dcp_t_rpc_annotation_help},
        {"-default",    DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_rpc_default_profile_help},
        {"-interface",  DCP_ARGV_STRING,   NULL, NULL,
             dcp_t_rpc_interface_help},
        {"-member",     DCP_ARGV_STRING,   NULL, NULL,
             dcp_t_rpc_member_list_help},
        {"-priority",   DCP_ARGV_INT,      NULL, NULL,
             dcp_t_rpc_priority_help},
        {NULL,          DCP_ARGV_END,      NULL,NULL,0} 
    };
    
    arg_table[0].dst = (char *)&annotation;
    arg_table[1].dst = (char *)&default_profile;
    arg_table[2].dst = (char *)&interface_str;
    arg_table[3].dst = (char *)&member_name;
    arg_table[4].dst = (char *)&priority_int;
    
    /* initialized the interface_id */
    memset(&interface_id, 0, sizeof(rpc_if_id_t));
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(profile);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    /* Cannot use -default with other options */
    if (default_profile) {
        if ((interface_str != NULL) || (priority_int != 0) ||
	    (annotation != NULL)) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_default_with_others);
            return TCL_ERROR;
        }
    }        

    if (annotation == NULL) {
        annotation = null_annotation;
    }
    
    if ((priority_int < 0) || (priority_int > 7)) {
        DCP_SET_RESULT_CODE(dcp_s_rpc_bad_priority);
        return TCL_ERROR;                                   
    }
    else {
        priority = (unsigned32)priority_int;
    }
    
    if (member_name == NULL) {
        DCP_SET_RESULT_CODE(dcp_s_rpc_no_member_name);
        return TCL_ERROR; 
    }
    
    if (interface_str == NULL) {
        /* Interface must be given unless default profile is used */
        if (default_profile == FALSE) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_no_interface_id_def);
            return TCL_ERROR; 
        }            
    }
    else {
        /* get the interface uuid string and the version numbers */
        dcp_rpc_parse_if_id(interface_str, &interface_id, &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    }
    
    /* 
     * The profile name is in argv[1], the value of member_name is 
     * treated as a list.  Both are split and each margv is added 
     * to each gargv 
     */        
    Tcl_SplitList(interp, argv[1], &gargc, &gargv);
    Tcl_SplitList(interp, member_name, &margc, &margv);
    for (i=0; i<gargc; i++) {
        for (j=0; j<margc; j++) {
            if (default_profile) {
                /* add the default profile element */
                rpc_ns_profile_elt_add(name_syntax,
                                       (unsigned char *)gargv[i],
                                       NULL,
                                       name_syntax,
                                       (unsigned char *)margv[j],
                                       priority,
                                       (unsigned char *)annotation,
                                       &rpc_status);
                
                if (rpc_status != rpc_s_ok) {
                    free((char *) gargv);
                    free((char *) margv);
                    DCP_SET_RESULT_CODE(rpc_status);
                    return TCL_ERROR;
                }
            }
            else {
                /* add the specified element from the profile */
                rpc_ns_profile_elt_add(name_syntax,
                                       (unsigned char *)gargv[i],
                                       &interface_id,
                                       name_syntax,
                                       (unsigned char *)margv[j],
                                       priority,
                                       (unsigned char *)annotation,
                                       &rpc_status);
                
                if (rpc_status != rpc_s_profile_element_replaced) {
                    if (rpc_status != rpc_s_ok) {
                        free((char *) gargv);
                        free((char *) margv);
                        DCP_SET_RESULT_CODE(rpc_status);
                        return TCL_ERROR;
                        
                    }
                }
            }
        }
    }
    free((char *) gargv);
    free((char *) margv);
    return TCL_OK;
}

int
dcp_rpcprofile_delete(ClientData clientData, Tcl_Interp *interp,
                      int argc, char **argv)
{
    unsigned32      name_syntax = rpc_c_ns_syntax_default;
    unsigned32      rpc_status;
    int             j;              /* index for list argv */
    int             largc;          /* argc for the list */
    char          **largv;          /* argv for the list */
    
    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(profile);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    /* Treat argv[1] as a list and process each element */
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        rpc_ns_profile_delete (name_syntax,
                               (unsigned char *)largv[j],
                               &rpc_status);
        DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
    }
    free((char *) largv);
    return TCL_OK;
    
}

int
dcp_rpcprofile_list(ClientData clientData, Tcl_Interp *interp,
                    int argc, char **argv)
{
    unsigned32          name_syntax = rpc_c_ns_syntax_default;
    char               *member_name = NULL;
    boolean32           update_data = TRUE;
    unsigned32            rpc_status;
    int                   result = TCL_OK;
    int                   j;              /* index for list argv */
    int                   largc;          /* argc for the list */
    char                **largv;          /* argv for the list */
    int                   m;              /* index for member list argv */
    int                   margc;          /* argc for the member list */
    char                **margv;          /* argv for the member list */
    char                **emargv;         /* argv for the expanded margv */
    rpc_ns_inq_handle_t   inquiry_context;
    unsigned_char_p_t     profile_member_name;
    rpc_if_id_t           element_if_id;
    unsigned32            element_priority;
    unsigned_char_p_t     element_annotation;
    boolean32             members_found = FALSE;
    boolean32             wrote_something = FALSE;
    unsigned32            temp_status;
    
    dcp_ArgvInfo arg_table[] = {
        {"-member",   DCP_ARGV_STRING, NULL, NULL, dcp_t_rpc_member_list_help},
        {"-noupdate", DCP_ARGV_CONSTANT, (char *)0, NULL,
           dcp_t_rpc_noupdate_help},
        {NULL,        DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&member_name;
    arg_table[1].dst = (char *)&update_data;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(profile);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    /* 
     * if there were members specifed, get a list of canonical names 
     * to compare any results we get with.
     */
    if (member_name) {
        Tcl_SplitList(interp, member_name, &margc, &margv);
        emargv = (char **)malloc(margc*sizeof(char *));
        for (m=0; m<margc; m++) { 
            rpc_ns_entry_expand_name(name_syntax,
                                     (unsigned char *)margv[m],
                                     (unsigned char **)&emargv[m],
                                     &rpc_status);
            
            if (rpc_status != rpc_s_ok) {
                free((char *) margv);
                free((char *) emargv);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
        }
        free((char *) margv);
    }        

    /* Treat argv[1] as a list of profiles and process each element */
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        /* do the real work */
        /* set up a context to get the members */
        rpc_ns_profile_elt_inq_begin (name_syntax,
                                      (unsigned char *)largv[j], 
                                      rpc_c_profile_all_elts,
                                      NULL,
                                      0,
                                      name_syntax,
                                      NULL,
                                      &inquiry_context,
                                      &rpc_status);
        if (rpc_status != rpc_s_ok) {
            if (member_name) free((char *) emargv);
            free((char *) largv);
            DCP_SET_RESULT_CODE(rpc_status);
            return TCL_ERROR;
        }
        
        /* 
         * Check if the caller wants us to update the local 
         * name-service data. If so, set the handle expiration 
         * age so that we bypass the cache.
         */
        if (update_data) {
            rpc_ns_mgmt_handle_set_exp_age(inquiry_context, 0, &rpc_status);
            if (rpc_status != rpc_s_ok) {
                rpc_ns_profile_elt_inq_done(&inquiry_context, &temp_status);
                if (member_name) free((char *) emargv);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
        }
        
	if (wrote_something) {
	    Tcl_AppendResult(interp, "\n", NULL);
	}
	wrote_something = FALSE;
        do {
            rpc_ns_profile_elt_inq_next (inquiry_context,
                                         &element_if_id,
                                         &profile_member_name,
                                         &element_priority,
                                         &element_annotation,
                                         &rpc_status);
            
            if (rpc_status == rpc_s_no_more_elements) {
                break;
            }
            
            if (rpc_status != rpc_s_ok) {
                rpc_ns_profile_elt_inq_done(&inquiry_context, &temp_status);
                if (member_name) free((char *) emargv);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
            
            /* 
             * Comparison with profile_member_name and emargv 
             * to see if we shouldn't return this name
             */
            if (member_name) {
                for (m=0; m<margc; m++) { 
                    if (!strcmp(emargv[m], (char *)profile_member_name)) {
                        if (members_found == TRUE) {
                            Tcl_AppendResult(interp, "\n", NULL);
                        }
                        Tcl_AppendResult(interp, profile_member_name, NULL);
                        members_found = TRUE;
			wrote_something = TRUE;
                        break;
                    }
                }
            } else {            /* all members are to be returned */
                if (members_found == TRUE) {
                    Tcl_AppendResult(interp, "\n", NULL);
                }
                Tcl_AppendResult(interp, profile_member_name, NULL);
                members_found = TRUE;
		wrote_something = TRUE;
            }
            
            rpc_string_free((unsigned char **)&profile_member_name,
                            &temp_status);
        } while (rpc_status != rpc_s_no_more_members);
        
        /* clean up the context */
        rpc_ns_profile_elt_inq_done (&inquiry_context, &rpc_status);
        if (rpc_status != rpc_s_ok) {
            if (member_name) free((char *) emargv);
            free((char *) largv);
            DCP_SET_RESULT_CODE(rpc_status);
            return TCL_ERROR;
        }
    }                           /* for j in each largv*/
    if ((strlen((char *)interp->result) > 0) &&
	(interp->result[strlen((char *)interp->result) - 1] == '\n')) {
	interp->result[strlen((char *)interp->result) - 1] = '\0';
    }
    if (member_name) free((char *) emargv);
    free((char *) largv);
    return result;
}

int
dcp_rpcprofile_remove(ClientData clientData, Tcl_Interp *interp,
                      int argc, char **argv)
{
    unsigned32          name_syntax = rpc_c_ns_syntax_default;
    char               *member_name = NULL;
    unsigned32          rpc_status;
    int                 j;              /* index for list largv */
    int                 largc;          /* argc for the list */
    char              **largv;          /* argv for the list */
    boolean32           default_profile = FALSE;
    int                 priority_int = -1;
    unsigned32          priority = 0;
    char               *annotation = NULL;
    char               *null_annotation = "";
    char               *interface_str = NULL;
    rpc_if_id_t         interface_id;
    rpc_ns_inq_handle_t inquiry_context;
    rpc_if_id_t         element_if_id;
    unsigned_char_p_t   element_member_name;
    unsigned32          element_priority;
    unsigned_char_p_t   element_annotation;
    unsigned32          temp_status;
    
    typedef enum {
        none,
        default_only,
        annotation_match,
        priority_match,
        interface
        } match_type_t;
    match_type_t        match_type = none;
    
    dcp_ArgvInfo arg_table[] = {
        {"-annotation", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rpc_annotation_help},
        {"-default",    DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_rpc_default_profile_help},
        {"-interface",  DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rpc_interface_help},
        {"-member",     DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rpc_member_help},
        {"-priority",   DCP_ARGV_INT,    NULL, NULL,
             dcp_t_rpc_priority_help},
        {NULL,          DCP_ARGV_END,      NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&annotation;
    arg_table[1].dst = (char *)&default_profile;
    arg_table[2].dst = (char *)&interface_str;
    arg_table[3].dst = (char *)&member_name;
    arg_table[4].dst = (char *)&priority_int;
    
    /* initialized the interface_id */
    memset(&interface_id, 0, sizeof(rpc_if_id_t));
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(profile);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    /* 
     * User must specify the elements (members) to be removed, either 
     * they use the -default switch to say the default profile is to be 
     * removed, or they must give one of (-interface && -member), 
     * -annotation, -priority.
     */
    if (default_profile == TRUE) {
        match_type = default_only;
        if (annotation || member_name || interface_str ||
            (priority_int != -1)) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_default_with_others);
            return TCL_ERROR;
        }
    }
    
    if (annotation != NULL) {
        /* we know it can't be default_only since it checks for an annotation */
        match_type = annotation_match;
    }
    else {
        annotation = null_annotation;
    }
    
    if (priority_int != -1) {
        if (match_type != none) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_priority_with_others);
            return TCL_ERROR;
        }
        else {
            match_type = priority_match;
            if ((priority_int < 0) || (priority_int > 7)) {
                DCP_SET_RESULT_CODE(dcp_s_rpc_bad_priority);
                return TCL_ERROR;                                   
            }
            else {
                priority = (unsigned32)priority_int;
            }
        }
    }
    
    /* 
     * if there have been switches so far then if either of these appear
     * it's an error 
     * else no switches have been found so far, then there must be an 
     * interface and member switch,
     */
    if (match_type != none) {
        if ((member_name != NULL) || (interface_str != NULL)) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_member_or_annot);
            return TCL_ERROR;
        }
    }
    else {
        if (member_name == NULL) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_no_member_name);
            return TCL_ERROR; 
        }
        
        if (interface_str == NULL) {
            /* Interface must be given unless default profile is used */
            DCP_SET_RESULT_CODE(dcp_s_rpc_no_interface_id_def);
            return TCL_ERROR; 
        }
        else {
            match_type = interface;
            /* get the interface uuid string and the version numbers */
            dcp_rpc_parse_if_id(interface_str, &interface_id, &rpc_status);
            DCP_CHECK_RPC_ERROR(rpc_status);
        }
    }
    
    /* The profile name is in argv[1] which can be a list */        
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        /* do the real work */
        if (match_type == default_only) {
            /* remove the default profile element */
            rpc_ns_profile_elt_remove(name_syntax,
                                      (unsigned char *)largv[j],
                                      NULL,
                                      name_syntax,
                                      NULL,
                                      &rpc_status);
            DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
        }
        else if (match_type == interface) {
            /* remove the specified element from the profile */
            rpc_ns_profile_elt_remove(name_syntax,
                                      (unsigned char *)largv[j],
                                      &interface_id,
                                      name_syntax,
                                      (unsigned char *)member_name,
                                      &rpc_status);
            DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
        }
        else {
            /* match_type is annotation or priority */
            rpc_ns_profile_elt_inq_begin(name_syntax,
                                         (unsigned char *)largv[j], 
                                         rpc_c_profile_all_elts,
                                         &interface_id,
                                         rpc_c_vers_compatible,
                                         name_syntax,
                                         (unsigned char *)member_name,
                                         &inquiry_context,
                                         &rpc_status);
            DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
            
            /* loop through each element to see if it matches */
            do {
                rpc_ns_profile_elt_inq_next(inquiry_context,
                                            &element_if_id,
                                            &element_member_name,
                                            &element_priority,
                                            &element_annotation,
                                            &rpc_status);
                
                if (rpc_status != rpc_s_no_more_elements) {
                    if (rpc_status != rpc_s_ok) {
                        free((char *) largv);
                        rpc_ns_profile_elt_inq_done(&inquiry_context,
                                                    &temp_status);
                        DCP_SET_RESULT_CODE(rpc_status);
                        return TCL_ERROR;
                    }
                    else {
                        /*
                         * does the specified annotation or priority
                         * match the one in the profile element?
                         */
                        if (((match_type == annotation_match) &&
                             (!strcmp((char *)annotation,
                                      (char *)element_annotation))) ||
                            ((match_type == priority_match) &&
                             (priority == element_priority))) {
                            
                            /* remove the element */
                            rpc_ns_profile_elt_remove(name_syntax,
                                                      (unsigned char *)largv[j],
                                                      &element_if_id,
                                                      name_syntax,
                                                      element_member_name,
                                                      &rpc_status);
                            if (rpc_status != rpc_s_ok) {
                                free((char *) largv);
                                rpc_string_free(&element_member_name,
                                                &temp_status);
                                rpc_string_free(&element_annotation,
                                                &temp_status);
                                rpc_ns_profile_elt_inq_done(&inquiry_context,
                                                            &temp_status);
                                DCP_SET_RESULT_CODE(rpc_status);
                                return TCL_ERROR;
                            }  
                        }
                    }
                }
                /* Free member name if memory was allocated */
                if (element_member_name != NULL) {
                    rpc_string_free(&element_member_name, &temp_status);
                }
                
                /* Free annotation string if memory was allocated */
                if (element_annotation != NULL) {
                    rpc_string_free(&element_annotation, &temp_status);
                }
            } while (rpc_status != rpc_s_no_more_elements);
            
            rpc_ns_profile_elt_inq_done(&inquiry_context, &rpc_status);
            DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
        }                       /* if on match_type */
    }                           /* for j in each largv*/
    free((char *) largv);
    return TCL_OK;
}

int
dcp_rpcprofile_show(ClientData clientData, Tcl_Interp *interp,
                    int argc, char **argv)
{
    boolean32           default_profile = FALSE;
    boolean32           update_data = TRUE;
    char               *annotation = NULL;
    char               *element_uuid;
    char               *interface_str = NULL;
    char               *member_name = NULL;
    char               *version_string = NULL;
    int                 i, j;
    int                 largc;          /* argc for the list */
    char                **largv;        /* argv for the list */
    int                 priority_int = -1;
    rpc_if_id_t         element_if_id;
    rpc_if_id_t         interface_id;
    rpc_ns_inq_handle_t inquiry_context;
    unsigned32          element_priority;
    unsigned32          inquiry_type;
    unsigned32          name_syntax = rpc_c_ns_syntax_default;
    unsigned32          priority = 0;
    unsigned32          rpc_status;
    unsigned32          temp_status;
    unsigned32          version_option = rpc_c_vers_exact;
    unsigned_char_p_t   element_annotation;
    unsigned_char_p_t   element_member_name;
    boolean32           wrote_something = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-annotation", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rpc_annotation_help},
        {"-default",    DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_rpc_default_profile_help},
        {"-interface",  DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rpc_interface_help},
        {"-member",     DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rpc_member_help},
        {"-noupdate",   DCP_ARGV_CONSTANT, (char *)0, NULL,
             dcp_t_rpc_noupdate_help},
        {"-priority",   DCP_ARGV_INT,    NULL, NULL,
             dcp_t_rpc_priority_help},
        {"-version",    DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rpc_version_help},
        {NULL,          DCP_ARGV_END,      NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&annotation;
    arg_table[1].dst = (char *)&default_profile;
    arg_table[2].dst = (char *)&interface_str;
    arg_table[3].dst = (char *)&member_name;
    arg_table[4].dst = (char *)&update_data;
    arg_table[5].dst = (char *)&priority_int;
    arg_table[6].dst = (char *)&version_string;
    
    /* initialized the interface_id */
    memset(&interface_id, 0, sizeof(rpc_if_id_t));
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(profile);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    if (default_profile == TRUE) {
        inquiry_type = rpc_c_profile_default_elt;
        if (annotation || member_name || interface_str ||
            (priority_int != -1)) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_default_with_others);
            return TCL_ERROR;
        }
    }
    
    if (interface_str) {
        if ((member_name) && (*member_name != '\0')) {
            inquiry_type = rpc_c_profile_match_by_both;
        } else {
            inquiry_type = rpc_c_profile_match_by_if;
        }

        /* get the interface uuid string and the version numbers */
        dcp_rpc_parse_if_id(interface_str, &interface_id, &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    } else {
        if ((member_name) && (*member_name != '\0')) {
            inquiry_type = rpc_c_profile_match_by_mbr;
        } else {
            inquiry_type = rpc_c_profile_all_elts;
        }
    }

    if (priority_int != -1) {
        if ((priority_int < 0) || (priority_int > 7)) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_bad_priority);
            return TCL_ERROR;
        }
        else {
            priority = (unsigned32)priority_int;
        }
    }
    
    if (version_string != NULL) {
        if (interface_str == NULL) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_version_no_interface);
            return TCL_ERROR;
        }
        for (i = 0; i < MAX_VERSION_VALUES; i++) {
            if (strncmp(version_name[i], version_string, strlen(version_string)) == 0) {
                version_option = version_val[i];
                break;
            }
        }
        if (i == MAX_VERSION_VALUES) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rpc_bad_version_opt, version_string);
            return TCL_ERROR;
        }
    }

    /* The profile name is in argv[1] which can be a list */        
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        /* do the real work */
        rpc_ns_profile_elt_inq_begin(name_syntax,
                                     (unsigned char *)largv[j], 
                                     inquiry_type,
                                     &interface_id,
                                     version_option,
                                     name_syntax,
                                     (unsigned char *)member_name,
                                     &inquiry_context,
                                     &rpc_status);
        DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
        
        /* 
         * Check if the caller wants us to update the local 
         * name-service data. If so, set the handle expiration 
         * age so that we bypass the cache.
         */
        if (update_data) {
            rpc_ns_mgmt_handle_set_exp_age(inquiry_context, 0, &rpc_status);
            if (rpc_status != rpc_s_ok) {
                rpc_ns_profile_elt_inq_done(&inquiry_context, &temp_status);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
        }

	if (wrote_something) {
	    Tcl_AppendResult(interp, "\n", NULL);
	}
	wrote_something = FALSE;

        /* loop through each element to see if it matches */
        do {
            rpc_ns_profile_elt_inq_next(inquiry_context,
                                        &element_if_id,
                                        &element_member_name,
                                        &element_priority,
                                        &element_annotation,
                                        &rpc_status);
            
            if (rpc_status == rpc_s_no_more_elements) {
                break;
            }
            if (rpc_status != rpc_s_ok) {
                free((char *) largv);
                rpc_ns_profile_elt_inq_done(&inquiry_context, &temp_status);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
            
            uuid_to_string(&element_if_id.uuid,
                           (unsigned char **)&element_uuid, 
                           &rpc_status);
            
            if (rpc_status != rpc_s_ok) {
                free((char *) largv);
                rpc_string_free(&element_member_name, &temp_status);
                rpc_string_free(&element_annotation, &temp_status);
                rpc_ns_profile_elt_inq_done(&inquiry_context, &temp_status);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
            
            /* Add check against annotation and priority here */
            if (((priority_int == -1) || (element_priority == priority)) &&
                ((!annotation) || (!strcmp(annotation,
                                           (char *)element_annotation)))) {
                char str[512];
                
                /* show the element */
                if (interp->result[0] != '\0') {
                    Tcl_AppendResult(interp, "\n", NULL);
                }

                /* Output the annotation as a list if it contains a space */
                if (strchr((char *)element_annotation, ' ')) {
                    sprintf(str, "{{%s %d.%d} %s %d {%s}}",
                            element_uuid,
                            element_if_id.vers_major,
                            element_if_id.vers_minor,
                            element_member_name,
                            element_priority,
                            element_annotation);
                } 
                else {
                    sprintf(str, "{{%s %d.%d} %s %d %s}",
                            element_uuid,
                            element_if_id.vers_major,
                            element_if_id.vers_minor,
                            element_member_name,
                            element_priority,
                            element_annotation);
                }

                Tcl_AppendResult(interp, (char *)str, NULL);
		wrote_something = TRUE;
            }
            /* Free member name if memory was allocated */
            /* Free annotation string if memory was allocated */
            if (element_member_name) {
                rpc_string_free(&element_member_name, &temp_status);
            }
            if (element_annotation) {
                rpc_string_free(&element_annotation, &temp_status);
            }
        } while (rpc_status != rpc_s_no_more_elements);
        rpc_ns_profile_elt_inq_done(&inquiry_context, &temp_status);
    }                           /* for j in each largv*/
    if ((strlen((char *)interp->result) > 0) &&
	(interp->result[strlen((char *)interp->result) - 1] == '\n')) {
	interp->result[strlen((char *)interp->result) - 1] = '\0';
    }
    free((char *) largv);
    return TCL_OK;
}

int
dcp_rpc_import(ClientData clientData, Tcl_Interp *interp, 
               int argc, char **argv)
{
    unsigned32              rpc_status;
    unsigned32              name_syntax = rpc_c_ns_syntax_default;
    char                   *interface_str = NULL;
    rpc_if_id_t             interface_id;
    boolean32               update_data = TRUE;
    int                     max_bindings = INT_MAX;
    boolean32               all_objects = TRUE;
    char                   *object_uuid_string = NULL;
    uuid_t                  object_uuid;
    int                     j;
    int                     largc;          /* argc for the list */
    char                  **largv;          /* argv for the list */
    rpc_ns_import_handle_t  import_context;
    rpc_binding_handle_t    binding;
    unsigned char          *string_binding;
    char                   *tcl_binding;
    unsigned32              temp_status;
    boolean32              wrote_something = FALSE;

    rpc_if_rep_t    interface_spec = {
        0,                  /* ifspec rep version                           */
        0,                  /* operation count (not needed)                 */
        0,                  /* interface version (to be filled in later)    */
        { 0 },              /* interface uuid (to be filled in later)       */
        2,                  /* stub/runtime interface version number        */
        { 0, NULL },        /* endpoint vector (not needed)                 */
        { 1, &NIDL_ndr_id}, /* NDR transfer syntax (the only one for now)   */
        NULL,               /* server stub epv (not needed)                 */
        NULL                /* manager stub epv (not needed)                */
        };
    
    dcp_ArgvInfo arg_table[] = {
        {"-interface", DCP_ARGV_STRING,   NULL, NULL, dcp_t_rpc_interface_help},
        {"-max",       DCP_ARGV_INT,      NULL, NULL, dcp_t_rpc_max_help},
        {"-noupdate",  DCP_ARGV_CONSTANT, (char *)0, NULL, 
           dcp_t_rpc_noupdate_help},
        {"-object",    DCP_ARGV_STRING,   NULL, NULL, dcp_t_rpc_object_help},
        {NULL,         DCP_ARGV_END,      NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&interface_str;
    arg_table[1].dst = (char *)&max_bindings;
    arg_table[2].dst = (char *)&update_data;
    arg_table[3].dst = (char *)&object_uuid_string;

    /* initialized the interface_id */
    memset(&interface_id, 0, sizeof(rpc_if_id_t));
    uuid_create_nil(&object_uuid, &rpc_status);
    if (rpc_status != rpc_s_ok) {
        DCP_SET_RESULT_CODE(rpc_status);
        return TCL_ERROR;
    }
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(entry);

    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    if (interface_str == NULL) {
        /* Interface must be given  */
        DCP_SET_RESULT_CODE(dcp_s_rpc_no_interface_id);
        return TCL_ERROR; 
    }
    else {
        /* get the interface uuid string and the version numbers */
        dcp_rpc_parse_if_id(interface_str, &interface_id, &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
        
        /* fill in the if spec */
        interface_spec.id = interface_id.uuid;
        interface_spec.vers =
            (interface_id.vers_minor << 16) | interface_id.vers_major;
    }

    if (object_uuid_string != NULL) {
        all_objects = FALSE;
        uuid_from_string((unsigned char *)object_uuid_string,
                         &object_uuid,
                         &rpc_status);
        if (rpc_status != rpc_s_ok) {
            DCP_SET_RESULT_CODE(rpc_status);
            return TCL_ERROR;
        }
    }

    /*
     * Check that the max value entered is legal (greater than 0)
     */
    if (max_bindings < 1) {
        DCP_SET_RESULT_CODE(dcp_s_rpc_bad_max_value);
        return TCL_ERROR;
    }

    /* Treat argv[1] as a list and process each element */
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        /* do the real work */
        /* open import context with object uuid if specified */
        rpc_ns_binding_import_begin(name_syntax,
                                    (unsigned char *)largv[j],
                                    (rpc_if_handle_t) &interface_spec,
                                    (all_objects) ? NULL : &object_uuid,
                                    &import_context,
                                    &rpc_status);
        DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
        
        /* 
         * Check if the caller wants us to update the local 
         * name-service data. If so, set the handle expiration 
         * age so that we bypass the cache.
         */
        if (update_data) {
            rpc_ns_mgmt_handle_set_exp_age(import_context, 0, &rpc_status);
            if (rpc_status != rpc_s_ok) {
                rpc_ns_binding_import_done(&import_context, &temp_status);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
        }
        
	if (wrote_something) {
	    Tcl_AppendResult(interp, "\n", NULL);
	}
	wrote_something = FALSE;
        do {
            rpc_ns_binding_import_next(import_context, &binding, &rpc_status);
            
            if (rpc_status == rpc_s_no_more_bindings) {
                break;
            }
            
            if (rpc_status != rpc_s_ok) {
                rpc_ns_binding_import_done(&import_context, &temp_status);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }
            
            /*
             * Remove the object UUID, if any, that's automatically
             * included by import. We only want to display the
             * string equivalent of a tower - and it doesn't contain
             * object UUIDs.
             */
            rpc_binding_set_object(binding, NULL, &rpc_status);
            if (rpc_status != rpc_s_ok) {
                rpc_binding_free(&binding, &temp_status);
                rpc_ns_binding_import_done (&import_context, &temp_status);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }

            /* convert the binding to a string binding */
            rpc_binding_to_string_binding(binding,
                                          &string_binding,
                                          &rpc_status);
            if (rpc_status != rpc_s_ok) {
                rpc_binding_free(&binding, &temp_status);
                rpc_ns_binding_import_done (&import_context, &temp_status);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }

            tcl_binding=dcp_binding_string_to_tcl((char *)string_binding,
                                                  &rpc_status);
            if (rpc_status != rpc_s_ok) {
                rpc_binding_free(&binding, &temp_status);
                rpc_ns_binding_import_done (&import_context, &temp_status);
                free((char *) largv);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }

            /* add to the result */
            if (interp->result[0] != '\0') {
                Tcl_AppendResult(interp, "\n", NULL);
            }
            Tcl_AppendResult(interp, "{", (char *)tcl_binding, "}", NULL);
	    wrote_something = TRUE;
            free(tcl_binding);
            
            /* free the string binding memory held by the runtime */
            rpc_string_free(&string_binding, &temp_status);
        } while ((rpc_status != rpc_s_no_more_bindings) && --max_bindings > 0);
        
        /* clean up the context */
        rpc_ns_binding_import_done (&import_context, &rpc_status);
        DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
    }                           /* for j in each largv*/
    
    /* if binding not found return error */
    if (interp->result[0] == '\0') {
	free((char *) largv);
	DCP_SET_RESULT_CODE(dcp_s_rpc_no_binding_found);
	return TCL_ERROR;
    }

    else if (interp->result[strlen((char *)interp->result) - 1] == '\n') {
	interp->result[strlen((char *)interp->result) - 1] = '\0';
    }
    free((char *) largv);
    return TCL_OK;
}

int
dcp_rpc_export(ClientData clientData, Tcl_Interp *interp, 
               int argc, char **argv)
{
    unsigned32              rpc_status;
    unsigned32              name_syntax = rpc_c_ns_syntax_default;
    char                   *interface_str = NULL;
    rpc_if_id_t             interface_id;
    char                   *string_binding = NULL;
    char                   *object_uuid_str = NULL;
    int                     j;              /* index for list argv */
    int                     largc;          /* argc for the list */
    char                  **largv;          /* argv for the list */
    
    struct binding_vec {
        unsigned32              count;
        rpc_binding_handle_t    binding_h[MAX_BINDINGS];
    } binding_vector = {0};
    
    struct uuid_vec {
        unsigned32              count;
        uuid_p_t                uuid[MAX_OBJECTS];
    } object_vector;
    uuid_t                      objects[MAX_OBJECTS];

    rpc_if_rep_t    interface_spec = {
        0,                  /* ifspec rep version                           */
        0,                  /* operation count (not needed)                 */
        0,                  /* interface version (to be filled in later)    */
        { 0 },              /* interface uuid (to be filled in later)       */
        2,                  /* stub/runtime interface version number        */
        { 0, NULL },        /* endpoint vector (not needed)                 */
        { 1, &NIDL_ndr_id}, /* NDR transfer syntax (the only one for now)   */
        NULL,               /* server stub epv (not needed)                 */
        NULL                /* manager stub epv (not needed)                */
        };
    
    dcp_ArgvInfo arg_table[] = {
        {"-binding",   DCP_ARGV_STRING,   NULL, NULL, dcp_t_rpc_binding_list_help},
        {"-interface", DCP_ARGV_STRING,   NULL, NULL, dcp_t_rpc_interface_help},
        {"-object",    DCP_ARGV_STRING,   NULL, NULL,
             dcp_t_rpc_object_list_help},
        {NULL,         DCP_ARGV_END,      NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&string_binding;
    arg_table[1].dst = (char *)&interface_str;
    arg_table[2].dst = (char *)&object_uuid_str;
    
    /* initialized the interface_id and vectors*/
    memset(&interface_id, 0, sizeof(rpc_if_id_t));
    binding_vector.count = 0;
    object_vector.count = 0;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(profile);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    if (interface_str != NULL) {
        /* get the interface uuid string and the version numbers */
        dcp_rpc_parse_if_id(interface_str, &interface_id, &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
        
        /* fill in the if spec */
        interface_spec.id = interface_id.uuid;
        interface_spec.vers =
            (interface_id.vers_minor << 16) | interface_id.vers_major;
    }
    
    if (string_binding == NULL) {
        if (interface_str != NULL) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_no_binding);
            return TCL_ERROR;
        }
    }
    else {
        if (interface_str == NULL) {
            /* Interface must be given  */
            DCP_SET_RESULT_CODE(dcp_s_rpc_no_interface_id);
            return TCL_ERROR; 
        }
        dcp_rpc_make_binding_vector(interp, string_binding,
                                    (rpc_binding_vector_t *)&binding_vector,
                                    &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    }
        
    if (object_uuid_str == NULL) {
        if ((interface_str == NULL) || (string_binding == NULL)) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_no_objects);
            return TCL_ERROR;
        }
    }
    else {
        dcp_rpc_make_object_vector(interp, object_uuid_str, &objects[0],
                                   (uuid_vector_t *)&object_vector,
                                   &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    }
    
    /* Treat argv[i] as a list and process each element */
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        /* if no interface was specified, just export objects */
        if (interface_str) {
            rpc_ns_binding_export(name_syntax,
                                  (unsigned_char_p_t)largv[j],
                                  (rpc_if_handle_t) &interface_spec,
                                  (rpc_binding_vector_p_t) &binding_vector,
                                  (uuid_vector_p_t) ((object_vector.count) ?
                                                     &object_vector : NULL),
                                  &rpc_status);
        }
        else {
            rpc_ns_binding_export(name_syntax,
                                  (unsigned_char_p_t)largv[j],
                                  NULL,
                                  NULL,
                                  (uuid_vector_p_t) &object_vector,
                                  &rpc_status);
        }
        DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
    }                           /* for j in each largv*/
    free((char *) largv);
    return TCL_OK;
}

int
dcp_rpc_unexport(ClientData clientData, Tcl_Interp *interp, 
                 int argc, char **argv)
{
    unsigned32              rpc_status;
    unsigned32              name_syntax = rpc_c_ns_syntax_default;
    char                   *interface_str = NULL;
    rpc_if_id_t             interface_id;
    unsigned32              version_option = rpc_c_vers_compatible;
    char                   *version_string = NULL;
    char                   *object_uuid_str = NULL;
    int                     i, j;
    int                     largc;          /* argc for the list */
    char                  **largv;          /* argv for the list */

    struct uuid_vec {
        unsigned32              count;
        uuid_p_t                uuid[MAX_OBJECTS];
    } obj_vector;
    uuid_t                      objects[MAX_OBJECTS];
    
    dcp_ArgvInfo arg_table[] = {
        {"-interface", DCP_ARGV_STRING,   NULL, NULL,
             dcp_t_rpc_interface_help},
        {"-object",    DCP_ARGV_STRING,   NULL, NULL,
             dcp_t_rpc_object_list_help},
        {"-version",   DCP_ARGV_STRING,   NULL, NULL,
             dcp_t_rpc_version_help},
        {NULL,         DCP_ARGV_END,      NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&interface_str;
    arg_table[1].dst = (char *)&object_uuid_str;
    arg_table[2].dst = (char *)&version_string;
    
    /* initialized the interface_id and vectors*/
    memset(&interface_id, 0, sizeof(rpc_if_id_t));
    obj_vector.count = 0;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();
    DCP_CHECK_RPC_NO_ARG(profile);
    
    /* Set convenience variables */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    if (interface_str != NULL) {
        /* get the interface uuid string and the version numbers */
        dcp_rpc_parse_if_id(interface_str, &interface_id, &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    }

    if (version_string != NULL) {
        if (interface_str == NULL) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_version_no_interface);
            return TCL_ERROR;
        }
        for (i = 0; i < MAX_VERSION_VALUES; i++) {
            if (strncmp(version_name[i], version_string, strlen(version_string)) == 0) {
                version_option = version_val[i];
                break;
            }
        }
        if (i == MAX_VERSION_VALUES) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rpc_bad_version_opt, version_string);
            return TCL_ERROR;
        }
    }

    if (object_uuid_str == NULL) {
        if (interface_str == NULL) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_no_objects);
            return TCL_ERROR;
        }
    }
    else {
        dcp_rpc_make_object_vector(interp, object_uuid_str, &objects[0],
                                   (uuid_vector_t *)&obj_vector, &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    }
    
    /* Treat argv[1] as a list and process each element */
    Tcl_SplitList(interp, argv[1], &largc, &largv);
    for (j=0; j<largc; j++) {
        /* if no interface was specified, just export objects */
        if (interface_str) {
            rpc_ns_mgmt_binding_unexport(name_syntax,
                                         (unsigned_char_p_t)largv[j],
                                         &interface_id,
                                         version_option,
                                         (uuid_vector_p_t)((obj_vector.count) ?
                                                           &obj_vector : NULL),
                                         &rpc_status);
        }
        else {
            /*
             * The version argument is meaningless here but still must
             * be specified correctly since it gets checked in the
             * called routine.
             */
            rpc_ns_mgmt_binding_unexport(name_syntax,
                                         (unsigned_char_p_t)largv[j],
                                         NULL,
                                         rpc_c_vers_all,
                                         (uuid_vector_p_t)&obj_vector,
                                         &rpc_status);
            
        }
        DCP_CHECK_RPC_ERROR_LARGV(rpc_status);
    }                           /* for j in each largv*/
    free((char *) largv);
    return TCL_OK;
}

int
dcp_endpoint_create(ClientData clientData, Tcl_Interp *interp,
                    int argc, char **argv)
{
    unsigned32       rpc_status;
    boolean32        noreplace = FALSE;
    char             *interface_str = NULL;
    rpc_if_id_t      interface_id;
    char             *string_binding = NULL;
    char             *object_uuid_str = NULL;
    unsigned_char_t  *annotation_str = NULL;
    uuid_t           objects[MAX_OBJECTS];

    struct binding_vec {
        unsigned32            count;
        rpc_binding_handle_t  binding_h[MAX_BINDINGS];
    } binding_vector = {0};

    struct uuid_vec {
        unsigned32  count;
        uuid_p_t    uuid[MAX_OBJECTS];
    } object_vector;

    rpc_if_rep_t  interface_spec = {
        0,                  /* ifspec rep version                           */
        0,                  /* operation count (not needed)                 */
        0,                  /* interface version (to be filled in later)    */
        { 0 },              /* interface uuid (to be filled in later)       */
        2,                  /* stub/runtime interface version number        */
        { 0, NULL },        /* endpoint vector (not needed)                 */
        { 1, &NIDL_ndr_id}, /* NDR transfer syntax (the only one for now)   */
        NULL,               /* server stub epv (not needed)                 */
        NULL                /* manager stub epv (not needed)                */
        };

    dcp_ArgvInfo arg_table[] = {
        {"-annotation", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_annotation_help},
        {"-binding", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_binding_list_help},
        {"-interface", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_interface_help},
        {"-noreplace", DCP_ARGV_CONSTANT, (char *)1, NULL, 
            dcp_t_rpc_noreplace_help},
        {"-object", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_object_list_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&annotation_str;
    arg_table[1].dst = (char *)&string_binding;
    arg_table[2].dst = (char *)&interface_str;
    arg_table[3].dst = (char *)&noreplace;
    arg_table[4].dst = (char *)&object_uuid_str;

    /*
     * Initialize the interface_id and vectors
     */
    memset(&interface_id, 0, sizeof(rpc_if_id_t));
    binding_vector.count = 0;
    object_vector.count = 0;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for extraneous arguments (this command takes no non-options)
     */
    DCP_CHECK_EXTRA_ARGS_NO_ARG();

    /*
     * The interface_id is required.
     */
    if (interface_str == NULL) {
        DCP_SET_RESULT_CODE(dcp_s_rpc_no_interface_id);
        return TCL_ERROR;
    }

    /*
     * A binding string is required.
     */
    if (string_binding == NULL) {
        DCP_SET_RESULT_CODE(dcp_s_rpc_no_binding);
        return TCL_ERROR;
    }

    /*
     * Get the interface uuid string and the version numbers
     */
    dcp_rpc_parse_if_id(interface_str, &interface_id, &rpc_status);
    DCP_CHECK_RPC_ERROR(rpc_status);

    /*
     * Fill in the if spec
     */
    interface_spec.id = interface_id.uuid;
    interface_spec.vers = (interface_id.vers_minor << 16) | interface_id.vers_major;

    /*
     * Set up the binding vector
     */
    dcp_rpc_make_binding_vector(interp,
                                string_binding,
                                (rpc_binding_vector_t *)&binding_vector,
                                &rpc_status);
    DCP_CHECK_RPC_ERROR(rpc_status);

    if (object_uuid_str != NULL) {
        dcp_rpc_make_object_vector(interp,
                                   object_uuid_str,
                                   &objects[0],
                                   (uuid_vector_t *)&object_vector,
                                   &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    }

    if (noreplace) {
        rpc_ep_register_no_replace((rpc_if_handle_t) &interface_spec,
                                   (rpc_binding_vector_p_t) &binding_vector,
                                   (uuid_vector_p_t) ((object_vector.count) ?
                                                      &object_vector : NULL),
                                   annotation_str,
                                   &rpc_status);
    }
    else {
        rpc_ep_register((rpc_if_handle_t) &interface_spec,
                        (rpc_binding_vector_p_t) &binding_vector,
                        (uuid_vector_p_t) ((object_vector.count) ?
                                           &object_vector : NULL),
                        annotation_str,
                        &rpc_status);
    }
    DCP_CHECK_RPC_ERROR(rpc_status);
    return TCL_OK;
}


int
dcp_endpoint_delete(ClientData clientData, Tcl_Interp *interp,
                    int argc, char **argv)
{
    unsigned32   rpc_status;
    char         *interface_str = NULL;
    rpc_if_id_t  interface_id;
    char         *string_binding = NULL;
    char         *object_uuid_str = NULL;
    int          i, j;
    uuid_t       objects[MAX_OBJECTS];

    struct binding_vec {
        unsigned32            count;
        rpc_binding_handle_t  binding_h[MAX_BINDINGS];
    } binding_vector = {0};

    struct uuid_vec {
        unsigned32  count;
        uuid_p_t    uuid[MAX_OBJECTS];
    } object_vector;

    rpc_if_rep_t  interface_spec = {
        0,                  /* ifspec rep version                           */
        0,                  /* operation count (not needed)                 */
        0,                  /* interface version (to be filled in later)    */
        { 0 },              /* interface uuid (to be filled in later)       */
        2,                  /* stub/runtime interface version number        */
        { 0, NULL },        /* endpoint vector (not needed)                 */
        { 1, &NIDL_ndr_id}, /* NDR transfer syntax (the only one for now)   */
        NULL,               /* server stub epv (not needed)                 */
        NULL                /* manager stub epv (not needed)                */
        };

    dcp_ArgvInfo arg_table[] = {
        {"-binding", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_binding_list_help},
        {"-interface", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_interface_help},
        {"-object", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_object_list_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&string_binding;
    arg_table[1].dst = (char *)&interface_str;
    arg_table[2].dst = (char *)&object_uuid_str;
    
    /*
     * Initialize the interface_id and vectors
     */
    memset(&interface_id, 0, sizeof(rpc_if_id_t));
    binding_vector.count = 0;
    object_vector.count = 0;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for extraneous arguments (this command takes no non-options)
     */
    DCP_CHECK_EXTRA_ARGS_NO_ARG();

    /*
     * The interface_id is required.
     */
    if (interface_str == NULL) {
        DCP_SET_RESULT_CODE(dcp_s_rpc_no_interface_id);
        return TCL_ERROR;
    }

    /*
     * A binding string is required.
     */
    if (string_binding == NULL) {
        DCP_SET_RESULT_CODE(dcp_s_rpc_no_binding);
        return TCL_ERROR;
    }

    /*
     * Get the interface uuid string and the version numbers
     */
    dcp_rpc_parse_if_id(interface_str, &interface_id, &rpc_status);
    DCP_CHECK_RPC_ERROR(rpc_status);

    /*
     * Fill in the if spec
     */
    interface_spec.id = interface_id.uuid;
    interface_spec.vers = (interface_id.vers_minor << 16) | interface_id.vers_major;

    /*
     * Set up the binding vector
     */
    dcp_rpc_make_binding_vector(interp,
                                string_binding,
                                (rpc_binding_vector_t *)&binding_vector,
                                &rpc_status);
    DCP_CHECK_RPC_ERROR(rpc_status);

    if (object_uuid_str != NULL) {
        dcp_rpc_make_object_vector(interp,
                                   object_uuid_str,
                                   &objects[0],
                                   (uuid_vector_t *) &object_vector,
                                   &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    }

    /* 
     * Go through binding vector and delete each one individually; can't
     * use rpc_ep_unregister to delete cross-product because it doesn't 
     * take NULL object vector to mean "all matching bindings".
     */
    for (i = 0; i < binding_vector.count; i++) {
        if (object_vector.count == 0) {
            rpc_mgmt_ep_unregister(NULL,
                                   &interface_id,
                                   binding_vector.binding_h[i],
                                   NULL,
                                   &rpc_status);
            DCP_CHECK_RPC_ERROR(rpc_status);
        }
        else {
            for (j = 0; j < object_vector.count; j++) {
                rpc_mgmt_ep_unregister(NULL,
                                       &interface_id,
                                       binding_vector.binding_h[i],
                                       object_vector.uuid[j],
                                       &rpc_status);
                DCP_CHECK_RPC_ERROR(rpc_status);
            }
        }
    }
    return TCL_OK;
}


int
dcp_endpoint_show(ClientData clientData, Tcl_Interp *interp,
                  int argc, char **argv)
{
    unsigned32            rpc_status, temp_status;
    char                  *interface_str = NULL;
    rpc_if_id_t           interface_id;
    unsigned32            version_option = rpc_c_vers_compatible;
    char                  *version_string = NULL;
    char                  *object_uuid_str = NULL;
    char                  *object_svc;
    char                  *interface_svc;
    char                  *binding_svc;
    char                  *annotation_svc;
    boolean32             uuid_opt = FALSE;
    rpc_binding_handle_t  ep_binding;
    uuid_t		  ep_uuid;
    unsigned32            inq_type;
    rpc_ep_inq_handle_t   inq_context;
    int                   inquiries;
    boolean               some_returned = FALSE;
    int                   i, j;
    rpc_if_id_t           if_id;          /* returned by ...ep_inq_next */
    rpc_binding_handle_t  binding;        /* returned by ...ep_inq_next */
    uuid_t                object_uuid;    /* returned by ...ep_inq_next */
    unsigned_char_t       *annotation;    /* returned by ...ep_inq_next */
    unsigned_char_t       *output_uuid_str;
    unsigned_char_t       *string_binding;
    char                  *tcl_binding;
    int                   largc;
    char                  **largv;
    char                  output[512];
    uuid_t                objects[MAX_OBJECTS];

    struct uuid_vec {
        unsigned32  count;
        uuid_p_t    uuid[MAX_OBJECTS];
    } obj_vector;

    dcp_ArgvInfo arg_table[] = {
        {"-interface", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_interface_help},
        {"-object", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_object_list_help},
	{"-uuid", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rpc_uuid_help},
        {"-version", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_version_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&interface_str;
    arg_table[1].dst = (char *)&object_uuid_str;
    arg_table[2].dst = (char *)&uuid_opt;
    arg_table[3].dst = (char *)&version_string;

    /*
     * Initialized the interface_id and vectors
     */
    memset(&interface_id, 0, sizeof(rpc_if_id_t));
    obj_vector.count = 0;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for extraneous arguments
     */
    DCP_CHECK_EXTRA_ARGS();

    if (uuid_opt) {
        if (interface_str != NULL || object_uuid_str != NULL || version_string != NULL) {
            DCP_SET_RESULT_CODE(dcp_s_rpc_uuid_with_others);
            return TCL_ERROR;
        }
    } else {
        if (interface_str != NULL) {
            /*
             * Get the interface uuid string and the version numbers
             */
	    dcp_rpc_parse_if_id(interface_str, &interface_id, &rpc_status);
	    DCP_CHECK_RPC_ERROR(rpc_status);
        }

        if (version_string != NULL) {
            if (interface_str == NULL) {
                DCP_SET_RESULT_CODE(dcp_s_rpc_version_no_interface);
                return TCL_ERROR;
            }
            for (i = 0; i < MAX_VERSION_VALUES; i++) {
                if (strncmp(version_name[i], version_string, strlen(version_string)) == 0) {
                    version_option = version_val[i];
                    break;
                }
            }
            if (i == MAX_VERSION_VALUES) {
                DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rpc_bad_version_opt, version_string);
                return TCL_ERROR;
            }
        }
    }

    if (argv[1] == NULL) {
	/*
         * Attempt to get binding handle for local host
         */
        rpc_binding_to_epmap((rpc_binding_handle_t) NULL,
                             &ep_binding,
                             &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    }
    else {
        /*
         * check to see if supplied binding is string or tcl syntax
         */
        if (!strchr(argv[1], ' ')) {
	    rpc_binding_from_string_binding((unsigned_char_p_t) argv[1],
                                            &ep_binding,
                                            &rpc_status);
	}
	else {
            Tcl_SplitList(interp, (char *) argv[1], &largc, &largv);
            string_binding = (unsigned_char_t *)dcp_binding_tcl_to_string(largc,
                                                    largv,
                                                    &rpc_status);
            free((char *) largv);
            DCP_CHECK_RPC_ERROR(rpc_status);
            rpc_binding_from_string_binding((unsigned_char_p_t) string_binding,
                                            &ep_binding,
                                            &rpc_status);
            rpc_string_free((unsigned char **)&string_binding, &temp_status);
        }
        if (rpc_status == rpc_s_invalid_rpc_protseq) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rpc_bad_endpoint_binding, argv[1]);
            return TCL_ERROR;
        }
        DCP_CHECK_RPC_ERROR(rpc_status);
    }

    /*
     * if -uuid specified, just get uuid of endpoint map and return
     */
    if (uuid_opt) {
        (*ept_v3_0_c_epv.ept_inq_object) (ep_binding, &ep_uuid, &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
        uuid_to_string(&ep_uuid, &output_uuid_str, &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
        Tcl_AppendResult(interp, output_uuid_str, NULL);
        rpc_string_free(&output_uuid_str, &temp_status);
        return TCL_OK;
    }

    if (object_uuid_str != NULL) {
        dcp_rpc_make_object_vector(interp,
                                   object_uuid_str,
                                   &objects[0],
                                   (uuid_vector_t *)&obj_vector,
                                   &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    }

    /*
     * Figure out what to select on
     */
    if (obj_vector.count == 0) {
        inq_type = (interface_str == NULL) ? rpc_c_ep_all_elts : 
                                             rpc_c_ep_match_by_if;
        inquiries = 1;
    }
    else {
        inq_type = (interface_str == NULL) ? rpc_c_ep_match_by_obj : 
                                             rpc_c_ep_match_by_both;
        inquiries = obj_vector.count;
    }

    /*
     * Do an inquiry cycle for each object specified
     */
    for (j = 0; j < inquiries; j++) {
        rpc_mgmt_ep_elt_inq_begin(ep_binding,
                                  inq_type,
                                  &interface_id,
                                  version_option,
                                  obj_vector.uuid[j],
                                  &inq_context,
                                  &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);

        for (;;) {
            rpc_mgmt_ep_elt_inq_next(inq_context,
                                     &if_id,
                                     &binding,
                                     &object_uuid,
                                     &annotation,
                                     &rpc_status);
            if (rpc_status == rpc_s_no_more_elements) {
                break;
            }

            if (rpc_status != rpc_s_ok) {
                rpc_mgmt_ep_elt_inq_done(&inq_context, &temp_status);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }

            if (some_returned) {
                Tcl_AppendResult(interp, "\n\n", NULL);
            }
            some_returned = TRUE;

            Tcl_AppendResult(interp, "{", NULL);

            /*
             * Check for nil object UUID
             */
            if (!uuid_is_nil(&object_uuid, &temp_status)) {
                uuid_to_string(&object_uuid, &output_uuid_str, &rpc_status);
                if (rpc_status != rpc_s_ok) {
                    rpc_binding_free(&binding, &temp_status);
                    rpc_string_free(&annotation, &temp_status);
                    rpc_mgmt_ep_elt_inq_done(&inq_context, &temp_status);
                    DCP_SET_RESULT_CODE(rpc_status);
                    return TCL_ERROR;
                }

                object_svc = (char *)dce_sprintf(dcp_t_rpc_object_header);
                if (object_svc == NULL) {
                    DCP_SET_RESULT_CODE(dcp_s_no_memory);
                    return TCL_ERROR;
                }
                sprintf(output, "{%s %s}\n ", object_svc, output_uuid_str);
                rpc_string_free(&output_uuid_str, &temp_status);
                Tcl_AppendResult(interp, output, NULL);
                free(object_svc);
            }

            uuid_to_string(&(if_id.uuid), &output_uuid_str, &rpc_status);
            if (rpc_status != rpc_s_ok) {
                rpc_binding_free(&binding, &temp_status);
                rpc_string_free(&annotation, &temp_status);
                rpc_mgmt_ep_elt_inq_done(&inq_context, &temp_status);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }

            interface_svc = (char *)dce_sprintf(dcp_t_rpc_interface_header);
            if (interface_svc == NULL) {
                DCP_SET_RESULT_CODE(dcp_s_no_memory);
                return TCL_ERROR;
            }
            sprintf(output, "{%s {%s %d.%d}}\n ", interface_svc, output_uuid_str,
                             if_id.vers_major, if_id.vers_minor);
            rpc_string_free(&output_uuid_str, &temp_status);
            Tcl_AppendResult(interp, output, NULL);
            free(interface_svc);

            rpc_binding_to_string_binding (binding, &string_binding, &rpc_status);
            rpc_binding_free(&binding, &temp_status);
            if (rpc_status != rpc_s_ok) {
                rpc_string_free(&annotation, &temp_status);
                rpc_mgmt_ep_elt_inq_done(&inq_context, &temp_status);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }

            tcl_binding = dcp_binding_string_to_tcl((char *)string_binding, &rpc_status);
            rpc_string_free(&string_binding, &temp_status);
            if (rpc_status != rpc_s_ok) {
                rpc_string_free(&annotation, &temp_status);
                rpc_mgmt_ep_elt_inq_done(&inq_context, &temp_status);
                DCP_SET_RESULT_CODE(rpc_status);
                return TCL_ERROR;
            }

            binding_svc = (char *)dce_sprintf(dcp_t_rpc_binding_header);
            if (binding_svc == NULL) {
                DCP_SET_RESULT_CODE(dcp_s_no_memory);
                return TCL_ERROR;
            }
            sprintf(output, "{%s {%s}}", binding_svc, tcl_binding);
            free(tcl_binding);
            Tcl_AppendResult(interp, output, NULL);
            free(binding_svc);

            if (strlen((char *)annotation) != 0) {
                annotation_svc = (char *)dce_sprintf(dcp_t_rpc_annotation_header);
                if (annotation_svc == NULL) {
                    DCP_SET_RESULT_CODE(dcp_s_no_memory);
                    return TCL_ERROR;
                }
                if (strchr((char *)annotation, ' ')) {
                    Tcl_AppendResult(interp, "\n {", annotation_svc, " {", annotation, "}}", NULL);
                } else {
                    Tcl_AppendResult(interp, "\n {", annotation_svc, " ", annotation, "}", NULL);
                }
                free(annotation_svc);
            }
            rpc_string_free(&annotation, &temp_status);

            Tcl_AppendResult(interp, "}", NULL);
        }

        rpc_mgmt_ep_elt_inq_done(&inq_context, &rpc_status);
        DCP_CHECK_RPC_ERROR(rpc_status);
    }
    return TCL_OK;
}


/*
 * FUNCTION: dcp_rpc_parse_if_id
 *
 * OVERVIEW: converts an interface id in either string or tcl syntax
 *           into an rpc_if_id_t
 */
void 
dcp_rpc_parse_if_id(char *str, rpc_if_id_t *if_id_p, unsigned32 *st)
{
    char uuid_str[MAX_IF_ID_STRING_LENGTH];
    char space_format[14] = "%[^ ] %hd.%hd";
    char comma_format[14] = "%[^,],%hd.%hd";
    char *format, *end, *rest, *minor;

    /* Remove any leading blank spaces on the interface spec provided */

    while ( *str == ' ' ) {
        str++;
    }

    /* get the interface uuid string and the version numbers */
    if ((rest = strchr(str, ' ')) != NULL) {     /* if space is in string */
	format = space_format;
    } else if ((rest = strchr(str, ',')) != NULL) {  /* if comma is in string */
	format = comma_format;
    } else {
	*st = dcp_s_rpc_interface_no_version;
	return;
    }

    /*
     * Make sure there's room for the version number, and that the
     * separating period is present
     */

    end = str + strlen(str);
    if ((end - rest < 4) || ((minor = strchr(rest, '.')) == NULL) ||
	(end - minor < 2)) {
	*st = dcp_s_rpc_interface_no_version;
	return;
    }

    sscanf(str, format,
	   uuid_str,
	   &(if_id_p->vers_major),
	   &(if_id_p->vers_minor));

    uuid_from_string((unsigned_char_p_t)uuid_str, &(if_id_p->uuid), st);
    return;
}

/*
 * FUNCTION: dcp_rpc_make_binding_vector
 *
 * OVERVIEW: Takes a list of string bindings and outputs a vector suitable
 *           for passing to appropriate rpc_* routines.
 *
 *           (Note: currently only handles a single binding rather than a
 *            list.)
 */
void
dcp_rpc_make_binding_vector(Tcl_Interp *interp,
                            char *string_binding,
                            rpc_binding_vector_t *binding_vector, 
                            unsigned32 *rpc_status)
{
    unsigned32              temp_status;
    char                   *sbind;
    int                     largc, targc;   /* argc for the list */
    char                  **largv, **targv; /* argv for the list */
    boolean                 single_tcl_binding = FALSE;

    /* 
     * Treat the string binding as a list of bindings.  Convert each 
     * one to a binding handle and store in a vector.
     */
    if (!strchr(string_binding, ' ')) {
        /* we have a single string syntax binding */
        rpc_binding_from_string_binding((unsigned_char_p_t)string_binding,
                                        &(*binding_vector).binding_h[0],
                                        rpc_status);
        if (*rpc_status != rpc_s_ok) {
            return;
        }
        (*binding_vector).count = 1;
    }
    else {
        Tcl_SplitList(interp, string_binding, &largc, &largv);
        if ((sbind=dcp_binding_tcl_to_string(largc, largv, rpc_status)) != NULL) {
	    /* we might have a single tcl syntax binding */
	    rpc_binding_from_string_binding((unsigned_char_p_t)sbind,
					    &(*binding_vector).binding_h[0],
					    rpc_status);
	    if (*rpc_status == rpc_s_ok) {
		single_tcl_binding = TRUE;
		(*binding_vector).count = 1;
	    }
	    rpc_string_free((unsigned char **)&sbind, &temp_status);
	}

	if (!single_tcl_binding) {
	    /*
	     * list didn't parse as a single tcl syntax binding, assume
	     * that it's a list of bindings
	     */
	    int i;
	    for (i = 0; i < largc; i++) {
		if (i >= MAX_BINDINGS) {
		    *rpc_status = dcp_s_too_many_list_elements;
		    break;
		}
		if (!strchr(largv[i], ' ')) {
		    /* assume string syntax or error */
		    rpc_binding_from_string_binding((unsigned_char_p_t)largv[i],
						    &(*binding_vector).binding_h[i],
						    rpc_status);
		    if (*rpc_status != rpc_s_ok) {
			break;
		    }
		    (*binding_vector).count++;
		}
		else {
		    /* must be tcl syntax (or error) */
		    Tcl_SplitList(interp, (char *)largv[i],
				  &targc, &targv);
		    sbind=dcp_binding_tcl_to_string(targc, targv, rpc_status);
		    if (*rpc_status != rpc_s_ok) {
			break;
		    }
		    rpc_binding_from_string_binding((unsigned_char_p_t)sbind,
						    &(*binding_vector).binding_h[i],
						    rpc_status);
		    (*binding_vector).count++;
		    rpc_string_free((unsigned char **)&sbind, &temp_status);
		    free ((char *) targv);
		    if (*rpc_status != rpc_s_ok) {
			break;
		    }
		}
	    }
	}
	free ((char *) largv);
    }
}

/*
 * FUNCTION: dcp_rpc_make_object_vector
 *
 * OVERVIEW: Takes a list of object UUIDs and outputs a vector suitable
 *           for passing to appropriate rpc_* routines.
 *
 */
void
dcp_rpc_make_object_vector(Tcl_Interp *interp,
                           char *object_uuid_str,
                           uuid_t *objects,
                           uuid_vector_t *object_vector, 
                           unsigned32 *rpc_status)
{
    int                     j;              /* index for list argv */
    int                     largc;          /* argc for the list */
    char                  **largv;          /* argv for the list */

    /* 
     * Treat the object string as a list of uuid's.  Convert each 
     * one to a uuid_t and store in a vector.
     */
    Tcl_SplitList(interp, object_uuid_str, &largc, &largv);
    if (largc > MAX_OBJECTS) {
        *rpc_status = dcp_s_too_many_list_elements;
        free((char *) largv);
        return;
    }

    for (j=0; j<largc; j++) {
        uuid_from_string((unsigned char *)largv[j],
                         &objects[j],
                         rpc_status);
        object_vector->uuid[j] = &(objects[j]);
        if (*rpc_status != rpc_s_ok) {
            break;
        }
    }
    (*object_vector).count = largc;
    free((char *) largv);
}
