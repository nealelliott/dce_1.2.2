/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: astp_gbl.c,v $
 * Revision 1.1.8.2  1996/02/18  18:50:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:04:36  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:13:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:09:51  root]
 * 
 * Revision 1.1.4.2  1993/07/07  19:55:26  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:29:35  ganni]
 * 
 * $EndLog$
 */
/*
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
**  NAME
**
**      ASTP_BLD_GLOBALS.C
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Defines global variables used by the parser and abstract
**      sytax tree (AST) builder modules.
**
**  VERSION: DCE 1.0
**
*/

#include <nidl.h>
#include <astp.h>

/*
 * External variables defined here, exported in ASTP.H
 * Theses externals are shared between the AST builder modules
 */

/*
 *  Interface Attributes
 */

int             interface_pointer_class = 0;    /* Default class for          */
                                                /* undecorated pointers       */
                                                /* Values are 0, ASTP_REF,    */
                                                /* ASTP_UNIQUE, or ASTP_PTR   */

/*
 *  Operation, Parameter, Type Attributes
 */

AST_type_n_t        *ASTP_transmit_as_type = NULL;
AST_type_n_t        *ASTP_switch_type = NULL;
AST_case_label_n_t  *ASTP_case = NULL;


/*
 *  Interface just parsed
 */
AST_interface_n_t *the_interface = NULL;

/*
 * List head for saved context for field
 * attributes forward referenced parameters.
 */
ASTP_field_ref_ctx_t *ASTP_field_ref_ctx_list = NULL;

/*
 * List head for referenced struct/union tags.
 */
ASTP_tag_ref_n_t *ASTP_tag_ref_list = NULL;


/*
 *  Control for parser
 */
boolean ASTP_parsing_main_idl = TRUE;

/*
 *  Builtin in constants
 */

AST_constant_n_t    *zero_constant_p = NULL;

/*
 * Builtin base types
 */
AST_type_n_t    *ASTP_char_ptr = NULL,
                *ASTP_boolean_ptr = NULL,
                *ASTP_byte_ptr = NULL,
                *ASTP_void_ptr = NULL,
                *ASTP_handle_ptr = NULL,
                *ASTP_short_float_ptr = NULL,
                *ASTP_long_float_ptr = NULL,
                *ASTP_small_int_ptr = NULL,
                *ASTP_short_int_ptr = NULL,
                *ASTP_long_int_ptr = NULL,
                *ASTP_hyper_int_ptr = NULL,
                *ASTP_small_unsigned_ptr = NULL,
                *ASTP_short_unsigned_ptr = NULL,
                *ASTP_long_unsigned_ptr = NULL,
                *ASTP_hyper_unsigned_ptr = NULL;

/* Default tag for union */
NAMETABLE_id_t  ASTP_tagged_union_id;


