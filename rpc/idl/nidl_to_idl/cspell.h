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
 * Revision 1.1.4.2  1996/02/18  23:45:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:43  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:34:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:17:40  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:14:29  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:47:10  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:14:31  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:10:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  02:59:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * ==========================================================================
 * Confidential and Proprietary.  Copyright 1987 by Apollo Computer Inc.,
 * Chelmsford, Massachusetts.  Unpublished -- All Rights Reserved Under
 * Copyright Laws Of The United States.
 *
 * Apollo Computer Inc. reserves all rights, title and interest with respect
 * to copying, modification or the distribution of such software programs and
 * associated documentation, except those rights specifically granted by Apollo
 * in a Product Software Program License, Source Code License or Commercial
 * License Agreement (APOLLO NETWORK COMPUTING SYSTEM) between Apollo and
 * Licensee.  Without such license agreements, such software programs may not
 * be used, copied, modified or distributed in source or object code form.
 * Further, the copyright notice must appear on the media, the supporting
 * documentation and packaging as set forth in such agreements.  Such License
 * Agreements do not grant any rights to use Apollo Computer's name or trademarks
 * in advertising or publicity, with respect to the distribution of the software
 * programs without the specific prior written permission of Apollo.  Trademark
 * agreements may be obtained in a separate Trademark License Agreement.
 * ==========================================================================
 */

    /*******************************************************************/
    /* Routines shared between the insert file speller and the backend */
    /*******************************************************************/

long cardinality(
#ifdef __STDC__
    subrange_t  s
#endif
    ) ;

type_t *pointerize_type(
#ifdef __STDC__
    type_t *tp
#endif
    ) ;

void pointerize_routine(
#ifdef __STDC__
    routine_t *rp
#endif
    ) ;

type_t *gen_type_node(
#ifdef __STDC__
    type_kind kind
#endif
    ) ;


boolean record_type(
#ifdef __STDC__
    type_t *tp
#endif
    ) ;

void CSPELL_constant_def(
#ifdef __STDC__
    FILE *fid,
    binding_t *bp
#endif
    ) ;

void CSPELL_constant_val(
#ifdef __STDC__
    FILE *fid,
    constant_t *cp
#endif
    ) ;

void CSPELL_name_declarator(
#ifdef __STDC__
    FILE *fid,
    NAMETABLE_id_t name,
    boolean pointer,
    array_dimension_t *array,
    boolean function,
    parameter_t *paramlist,
    boolean pointer_elements
#endif
    ) ;

void CSPELL_type_exp(
#ifdef __STDC__
    FILE *fid,
    type_t *tp,
    boolean *pointer,
    array_dimension_t **array_dimensions,
    boolean *function,
    parameter_t **paramlist,
    boolean *pointer_elements
#endif
    ) ;

void CSPELL_type_exp_simple(
#ifdef __STDC__
    FILE *fid,
    type_t *tp
#endif
    ) ;


void CSPELL_type_def(
#ifdef __STDC__
    FILE *fid,
    binding_t *bp
#endif
    ) ;

void CSPELL_typed_name(
#ifdef __STDC__
    FILE *fid,
    type_t *type,
    NAMETABLE_id_t name
#endif
    ) ;

void CSPELL_var_decl(
#ifdef __STDC__
    FILE *fid,
    type_t *type,
    NAMETABLE_id_t name
#endif
    ) ;

    /****************************/
    /* Generate a c header file */
    /****************************/

void CSPELL_gen_c_ins_file(
#ifdef __STDC__
    binding_t *astp,
    FILE  *fid,
    boolean emit_env,
    boolean gen_ansi
#endif
    ) ;
