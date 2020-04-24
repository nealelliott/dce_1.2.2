/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ir_pvt.h,v $
 * Revision 1.1.6.2  1996/02/18  23:45:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:15  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:19:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:12:00  root]
 * 
 * Revision 1.1.2.2  1993/07/07  20:45:50  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  20:43:49  ganni]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1991 by
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
**  NAME:
**
**      ir_pvt.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Header file containing defining intermediate-rep private data structures
**  for data that is kept in the IR_info_t field of some AST nodes.
**
**  %a%private_begin
**
**  MODIFICATION HISTORY: 
**  
**  09-Oct-91 D.Annicchiarico   Original version.
**
**  %a%private_end  
*/

#ifndef IR_PVTH_INCL
#define IR_PVTH_INCL

typedef struct IR_info_n_t {
    /*
     * Pointer to last created tuple in a parameter or type node's tuple list.
     */
    struct IR_tup_n_t   *cur_tup_p;
    /*
     * For a field, field number.  For other nodes, available for any use.
     */
    long                id_num;
    /*
     * On a param, T => requires server side preallocation of [ref] pointee(s).
     * On a type, T => same if reference is not under a full or unique pointer.
     */
    boolean             allocate_ref;
} IR_info_n_t;

typedef IR_info_n_t *IR_info_t;

/*
 * Data structure used to help sort the case labels of a union.
 */
typedef struct IR_case_info_n_t {
    struct AST_arm_n_t          *arm_p;     /* Ptr to union arm node */
    struct AST_case_label_n_t   *label_p;   /* Ptr to case label node */
    unsigned long               value;      /* Normallized case label value */
} IR_case_info_n_t;

#endif
