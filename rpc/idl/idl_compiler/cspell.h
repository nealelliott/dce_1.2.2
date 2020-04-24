/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cspell.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:02  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:15:39  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/01  23:52 UTC  lmm
 * 	Merging idl++ mods into Mothra
 * 	[1995/12/07  21:10:48  root]
 * 
 * Revision 1.1.4.2  1993/07/07  19:57:20  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:30:57  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1990, 1993 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME:
**
**      cspell.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Definitions of routines declared in cspell.c
**
**  VERSION: DCE 1.0
**
*/

#ifndef CSPELL_H
#define CSPELL_H

void CSPELL_std_include(
#ifdef PROTO
    FILE *fid,
    char header_name[],
    BE_output_k_t filetype,
    int op_count
#endif
);

void spell_name(
#ifdef PROTO
    FILE *fid,
    NAMETABLE_id_t name
#endif
);

void CSPELL_var_decl(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *type,
    NAMETABLE_id_t name
#endif
);

void CSPELL_typed_name(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *type,
    NAMETABLE_id_t name,
    AST_type_n_t *in_typedef,
    boolean in_struct,
    boolean spell_tag,
    boolean encoding_services
#endif
);

void CSPELL_function_def_header(
#ifdef PROTO
    FILE *fid,
    AST_operation_n_t *oper,
    NAMETABLE_id_t name
#endif
);

void CSPELL_cast_exp(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *tp
#endif
);

void CSPELL_ptr_cast_exp(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *tp
#endif
);

void CSPELL_type_exp_simple(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *tp
#endif
);

boolean CSPELL_scalar_type_suffix(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *tp
#endif
);

void CSPELL_pipe_struct_routine_decl
(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *p_pipe_type,
    BE_pipe_routine_k_t routine_kind,
    boolean cast
#endif
);

void DDBE_spell_manager_param_cast
(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *tp
#endif
);

#endif

#ifdef _HP_OODCE_

#define MAX_TAIL_LEN 15

typedef enum {p_k, a_k, f_k} type_kind_t;
typedef struct {
    int len;
    struct {
        type_kind_t kind;
        union {
            struct {
                AST_array_n_t *array;
                boolean in_typedef_or_struct;
            } array_info;
            struct {
                AST_parameter_n_t *param_list;
                boolean function_def;
            } function_info;
        } content;
    }  vec[MAX_TAIL_LEN];
} type_tail_t;

void CSPELL_type_exp (
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *tp,
    type_tail_t *tail,
    AST_type_n_t *in_typedef,
    boolean in_struct,
    boolean func_def,
    boolean spell_tag
#endif
);

void CSPELL_array_bounds(
#ifdef PROTO
    FILE *fid,
    AST_array_n_t *array,
    boolean in_typedef_or_struct
#endif
);

#endif /*_HP_OODCE_*/
