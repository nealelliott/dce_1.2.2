/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mtspipes.h,v $
 * Revision 1.1.6.2  1996/02/18  23:45:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:20  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:20:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:12:32  root]
 * 
 * Revision 1.1.2.2  1993/07/07  20:46:06  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  20:44:00  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1990, 1991, 1992 by
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
**      mtspipes.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Function prototypes for mtspipes.c
**
**  VERSION: DCE 1.0
**
*/

#ifndef MTSPIPES_H
#define MTSPIPES_H

#define BE_FINISHED_WITH_PIPES -32767

void DDBE_init_server_pipes
(
#ifdef PROTO
    FILE *fid,
    AST_operation_n_t *p_operation,
    long *p_first_pipe      /* ptr to index and direction of first pipe */
#endif
);

#endif
