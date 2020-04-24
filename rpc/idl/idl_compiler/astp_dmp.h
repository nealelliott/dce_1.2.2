/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: astp_dmp.h,v $
 * Revision 1.1.8.2  1996/02/18  23:44:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:43:51  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:13:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:09:48  root]
 * 
 * Revision 1.1.4.2  1993/07/07  19:55:18  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:29:32  ganni]
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
**      ASTP_DMP.H
**
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**      Header file for the AST Builder Dumper module, ASTP_DMP.C
**
**  VERSION: DCE 1.0
**
*/

#ifndef ASTP_DMP_H
#define ASTP_DMP_H
#ifdef DUMPERS
#include <nidl.h>
#include <ast.h>

/*
 * Exported dump routines
 */

void AST_dump_interface
(
#ifdef PROTO
    AST_interface_n_t *if_n_p
#endif
);

void AST_dump_operation
(
#ifdef PROTO
    AST_operation_n_t *operation_node_ptr,
    int indentation
#endif
);

void AST_dump_parameter
(
#ifdef PROTO
    AST_parameter_n_t *parameter_node_ptr,
    int indentation
#endif
);

void AST_dump_nametable_id
(
#ifdef PROTO
    char   *format_string,
    NAMETABLE_id_t id
#endif
);

void AST_dump_parameter
(
#ifdef PROTO
    AST_parameter_n_t *param_node_ptr,
    int     indentation
#endif
);

void AST_dump_type(
#ifdef PROTO
    AST_type_n_t *type_n_p,
    char *format,
    int indentation
#endif
);


void AST_dump_constant
(
#ifdef PROTO
    AST_constant_n_t *constant_node_ptr,
    int indentation
#endif
);

void AST_enable_hex_dump();


#endif /* Dumpers */
#endif /* ASTP_DMP_H */
