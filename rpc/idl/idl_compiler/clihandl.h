/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: clihandl.h,v $
 * Revision 1.1.8.2  1996/02/18  23:44:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:43:58  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:14:44  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:02 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:10:24  root]
 * 
 * Revision 1.1.2.1  1995/10/31  21:15:23  bfc
 * 	may 5 version from nightly
 * 	[1995/10/31  18:34:45  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE R1.1
 * 	[1995/10/31  18:32:54  bfc]
 * 
 * Revision 1.1.4.2  1993/07/07  19:56:38  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:30:26  ganni]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      clihandl.h
**
**  FACILITY:
**
**      IDL Compiler Backend
**
**  ABSTRACT:
**
**  Header file for clihandl.c
**
**  VERSION: DCE 1.0
**
*/
#ifndef CLIHANDL_H
#define CLIHANDL_H

#include <commstat.h>

typedef enum {
    BE_parm_handle_t_k,    /* Operation has first parameter of type handle_t */
    BE_parm_user_handle_k, /* Operation has first parm with [handle] attrib */
    BE_context_handle_k,   /* Operation has an [in] context handle parameter */
    BE_impl_handle_t_k,    /* No handle parm. Implicit handle_t handle */
    BE_impl_user_handle_k, /* No handle parm. Implicit [handle] handle */
    BE_auto_handle_k,      /* No handle parm. [auto_handle] interface */
    BE_rep_as_handle_t_k,  /* First parm handle_t with [rep_as] passed by val */
    BE_rep_as_handle_t_p_k /* First parm handle_t with [rep_as] passed by ref */
} BE_handle_type_k_t;

typedef struct {
    BE_handle_type_k_t handle_type;  /* Type of handle for operation */
    char *assoc_name;             /* Ptr to name to be used for assoc handle */
    char *type_name;                /* Ptr to name of [handle] type */
    char *user_handle_name;         /* Ptr to name of [handle] object */
    char deref_assoc;         /* '*' if handle must be dereferenced, else ' ' */
    char deref_generic;      /* '*' if handle must be dereferenced, else ' ' */
    boolean auto_handle_idempotent_op;  /* Only used if op is [auto_handle]
                                           TRUE if op is [idempotent] */
    NAMETABLE_id_t rep_as_name; /* Name of handle_t param to which [rep_as]
                                        is attached */
    NAMETABLE_id_t rep_as_type; /* Type of handle param which has [rep_as] */
} BE_handle_info_t;

extern char assoc_handle_ptr[];

void BE_setup_client_handle(
#ifdef PROTO
    FILE *fid,
    AST_interface_n_t *p_interface,
    AST_operation_n_t *p_operation,
    BE_handle_info_t *p_handle_info
#endif
);

void CSPELL_call_start(
#ifdef PROTO
    FILE *fid,
    BE_handle_info_t *p_handle_info,
    AST_interface_n_t *p_interface,
    AST_operation_n_t *p_operation,
    unsigned long op_num,            /* Number of current operation */
    BE_stat_info_t *p_comm_stat_info,
    BE_stat_info_t *p_fault_stat_info
#endif
);

void CSPELL_auto_handle_statics
(
#ifdef PROTO
    FILE * fid
#endif
);

void CSPELL_restart_logic
(
#ifdef PROTO
    FILE * fid,
    AST_operation_n_t *p_operation,
    boolean uses_packet
#endif
);

void CSPELL_binding_free_if_needed
(
#ifdef PROTO
    FILE *fid,
    BE_handle_info_t *p_handle_info
#endif
);

#endif

