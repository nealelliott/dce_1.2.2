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
 * $Log: propagat.h,v $
 * Revision 1.1.10.2  1996/02/18  23:45:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:28  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:21:55  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:05 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:13:03  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:40  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:44:28  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:58:47  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:25:12  bfc]
 * 
 * Revision 1.1.6.2  1993/07/07  20:03:42  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:34:42  ganni]
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
**      propagat.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Declaration of functions exported by propagat.c.
**
**  VERSION: DCE 1.0
**
*/

#ifndef PROPAGATH_INCL
#define PROPAGATH_INCL

#include <ast.h>                /* Abstract Syntax Tree defs */

/*
**  P R O P _ m a i n
**
**  Main routine for propagation component of IDL compiler.
**  Propagates attributes and other information throughout the AST.
**  Much of propagation is done by earlier compiler phases - this
**  routine handles any propagation that is easier to do if saved
**  until after the parsing/AST-building phases are complete.
*/

extern boolean PROP_main(       /* Returns true on success */
#ifdef PROTO
    boolean     *cmd_opt_arr,   /* [in] Array of command option flags */
    void        **cmd_val_arr,  /* [in] Array of command option values */
    AST_interface_n_t *int_p    /* [in] Ptr to AST interface node */
#endif
);

extern boolean PROP_accessor(   /* Returns TRUE on success */
#ifdef PROTO
    boolean     *cmd_opt_arr,   /* [in] Array of command option flags */
    void        **cmd_val_arr,  /* [in] Array of command option values */
    AST_interface_n_t *int_p    /* [in] Ptr to AST interface node */
#endif
);


#ifndef mips
void PROP_set_type_attr
(
#ifdef PROTO
    AST_type_n_t *type_node_ptr,
    AST_flags_t  type_attr
#endif
);
#endif


#endif
