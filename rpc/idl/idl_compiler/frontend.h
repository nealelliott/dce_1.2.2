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
 * $Log: frontend.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:10  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:17:45  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:03 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:11:32  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:05  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:43:43  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:57:11  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:26  bfc]
 * 
 * Revision 1.1.4.2  1993/07/07  19:59:03  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:32:11  ganni]
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
**      frontend.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Functions exported by frontend.c.
**
**  VERSION: DCE 1.0
**
*/

#ifndef FRONTEND_H
#define FRONTEND_H


#include <nidl.h>               /* IDL common defs */
#include <ast.h>                /* Abstract Syntax Tree defs */
#include <nametbl.h>            /* Nametable defs */

boolean FE_main(
#ifdef PROTO
    boolean             *cmd_opt,
    void                **cmd_val,
    STRTAB_str_t        idl_sid,
    AST_interface_n_t   **int_p
#endif
);


AST_interface_n_t   *FE_parse_import(
#ifdef PROTO
    STRTAB_str_t new_input
#endif
);

#endif /* FRONTEND_H */
