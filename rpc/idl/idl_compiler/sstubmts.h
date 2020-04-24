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
 * $Log: sstubmts.h,v $
 * Revision 1.1.6.2  1996/02/18  23:45:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:29  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:22:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:05 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:13:08  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:44  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:44:32  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:58:56  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:25:19  bfc]
 * 
 * Revision 1.1.2.2  1993/07/07  20:46:23  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  20:44:11  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1991, 1992 by
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
**  NAME:
**
**      sstubmts.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Header file for sstubmts.c
**
*/

#ifndef SSTUBMTS_H
#define SSTUBMTS_H


void BE_gen_sstub
(
#ifdef PROTO
    FILE *fid,              /* Handle for emitted C text */
    AST_interface_n_t *p_interface,     /* Ptr to AST interface node */
    language_k_t language,  /* Language stub is to interface to */
    char header_name[],     /* Name of header file to be included in stub */
    boolean *cmd_opt,
    void **cmd_val,
    DDBE_vectors_t *dd_vip    /* Data driven BE vector information ptr */
#endif
);


#endif /* SSTUBMTS_H */
