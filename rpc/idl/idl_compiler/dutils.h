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
 * $Log: dutils.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:06  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:16:55  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:03 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:11:15  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:47:55  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:43:31  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:56:43  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:10  bfc]
 * 
 * Revision 1.1.4.2  1993/07/07  19:58:19  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:31:38  ganni]
 * 
 * $EndLog$
 */
/*
**  @OSF_COPYRIGHT@
**
**  Copyright (c) 1989 by
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
**      dutils.h
**
**  FACILITY:
**
**      IDL Compiler Backend
**
**  ABSTRACT:
**
**  Header file for dutils.c
**
**  VERSION: DCE 1.0
*/

#ifndef DUTILS_H
#define DUTILS_H


#include <bedeck.h>


NAMETABLE_id_t BE_new_local_var_name
(
#ifdef PROTO
    char *root
#endif
);

char *BE_get_name
(
#ifdef PROTO
    NAMETABLE_id_t id
#endif
);

int BE_required_alignment
(
#ifdef PROTO
    AST_parameter_n_t *param
#endif
);

int BE_resulting_alignment
(
#ifdef PROTO
    AST_parameter_n_t *param
#endif
);

struct BE_ptr_init_t *BE_new_ptr_init
(
#ifdef PROTO
    NAMETABLE_id_t pointer_name,
    AST_type_n_t *pointer_type,
    NAMETABLE_id_t pointee_name,
    AST_type_n_t *pointee_type,
    boolean heap
#endif
);

AST_type_n_t *BE_get_type_node
(
#ifdef PROTO
    AST_type_k_t kind
#endif
);

AST_type_n_t *BE_pointer_type_node
(
#ifdef PROTO
    AST_type_n_t *type
#endif
);

AST_type_n_t *BE_slice_type_node
(
#ifdef PROTO
    AST_type_n_t *type
#endif
);

char *BE_first_element_expression
(
#ifdef PROTO
    AST_parameter_n_t *param
#endif
);

char *BE_count_expression
(
#ifdef PROTO
    AST_parameter_n_t *param
#endif
);

char *BE_size_expression
(
#ifdef PROTO
    AST_parameter_n_t *param
#endif
);

void BE_declare_surrogates
(
#ifdef PROTO
    AST_operation_n_t *oper,
    AST_parameter_n_t *param
#endif
);

void BE_declare_server_surrogates
(
#ifdef PROTO
    AST_operation_n_t *oper
#endif
);

int BE_num_elts
(
#ifdef PROTO
    AST_parameter_n_t *param
#endif
);

char *BE_A_expression
(
#ifdef PROTO
    AST_parameter_n_t *param,
    int dim
#endif
);

char *BE_B_expression
(
#ifdef PROTO
    AST_parameter_n_t *param,
    int dim
#endif
);

char *BE_Z_expression
(
#ifdef PROTO
    AST_parameter_n_t *param,
    int dim
#endif
);

AST_parameter_n_t *BE_create_recs
(
#ifdef PROTO
    AST_parameter_n_t *params,
    BE_side_t side
#endif
);

#ifdef DEBUG_VERBOSE

void traverse(
#ifdef PROTO
    AST_parameter_n_t *list,
    int indent
#endif
);

void traverse_blocks(
#ifdef PROTO
    BE_param_blk_t *block
#endif
);

#endif


#endif
