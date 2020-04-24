/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: genpipes.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:10  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:17:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:11:34  root]
 * 
 * Revision 1.1.4.2  1993/07/07  19:59:07  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:32:15  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1990 by
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
**      genpipes.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Function prototypes for genpipes.c
**
**  VERSION: DCE 1.0
**
*/

#ifndef GENPIPES_H
#define GENPIPES_H

#define BE_FINISHED_WITH_PIPES -32767

void BE_spell_pipe_struct_name
(
#ifdef PROTO
    AST_parameter_n_t *p_parameter,
    char pipe_struct_name[]
#endif
);

void CSPELL_init_server_pipes
(
#ifdef PROTO
    FILE *fid,
    AST_operation_n_t *p_operation,
    long *p_first_pipe      /* ptr to index and direction of first pipe */
#endif
);

void CSPELL_pipe_support_header
(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *p_pipe_type,
    BE_pipe_routine_k_t push_or_pull,
    boolean in_header
#endif
);

void BE_gen_pipe_routines
(
#ifdef PROTO
    FILE *fid,
    AST_interface_n_t *p_interface
#endif
);

void BE_gen_pipe_routine_decls
(
#ifdef PROTO
    FILE *fid,
    AST_interface_n_t *p_interface
#endif
);

void CSPELL_pipe_base_cast_exp
(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *p_type
#endif
);

void CSPELL_pipe_base_type_exp
(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *p_type
#endif
);

void BE_undec_piped_arrays
(
#ifdef PROTO
    AST_parameter_n_t *flat
#endif
);

#endif
