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
 * $Log: user_exc.h,v $
 * Revision 1.1.6.2  1996/02/18  23:45:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:31  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:22:39  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:05 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:13:19  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:47  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:44:36  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:59:05  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:25:23  bfc]
 * 
 * Revision 1.1.2.2  1993/07/07  20:46:28  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  20:44:15  ganni]
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
**      user_exc.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Function prototypes for user_exc.c
**
**
*/

#ifndef USER_EXC_H
#define USER_EXC_H


void DDBE_user_exceptions
(
#ifdef PROTO
    FILE *fid,
    AST_interface_n_t *p_interface,
    int *p_num_declared_exceptions,
    int *p_num_extern_exceptions
#endif
);

#endif /* USER_EXC_H */

