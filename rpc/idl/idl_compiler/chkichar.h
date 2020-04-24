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
 * $Log: chkichar.h,v $
 * Revision 1.1.6.2  1996/02/18  23:44:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:43:56  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:14:24  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:02 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:10:15  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:47:30  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:43:00  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:55:38  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:23:37  bfc]
 * 
 * Revision 1.1.2.2  1993/07/07  20:45:13  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  20:43:27  ganni]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) Digital Equipment Corporation, 1993
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
**  NAME:
**
**      chkichar.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  IDL Compiler Semantic Checking for international character support.
**
*/

#ifndef CHKICHAR_H
#define CHKICHAR_H


extern void CHK_param_cs(
#ifdef PROTO
    AST_parameter_n_t   *param_p,       /* [in] Ptr to AST parameter node */
    AST_type_n_t        *type_p         /* [in] Parameter type */
#endif
);

extern void CHK_op_cs(
#ifdef PROTO
    AST_operation_n_t   *op_p           /* [in] Ptr to AST operation node */
#endif
);

extern void CHK_field_cs(
#ifdef PROTO
    AST_field_n_t       *field_p        /* [in] Ptr to AST field node */
#endif
);

extern void CHK_pipe_base_type_cs(
#ifdef PROTO
    AST_pipe_n_t        *pipe_p,        /* [in] Ptr to AST pipe node */
    AST_interface_n_t   *int_p          /* [in] Ptr to interface node */
#endif
);

extern void CHK_type_cs(
#ifdef PROTO
    AST_type_n_t        *top_type_p,    /* [in] Top-level presented type */
    AST_type_n_t        *type_p,        /* [in] Ptr to AST type node */
    AST_interface_n_t   *int_p          /* [in] Ptr to interface node */
#endif
);


#endif /* CHKICHAR_H */

