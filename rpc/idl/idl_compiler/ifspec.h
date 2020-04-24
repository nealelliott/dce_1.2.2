/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ifspec.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:14  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:18:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:11:54  root]
 * 
 * Revision 1.1.4.2  1993/07/07  19:59:44  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:32:41  ganni]
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
**  NAME:
**
**      ifspec.h
**
**  FACILITY:
**
**      IDL Compiler Backend
**
**  ABSTRACT:
**
**  Header file for ifspec.c
**
**  VERSION: DCE 1.0
**
*/

#ifndef IFSPEC_H
#define IFSPEC_H

extern void CSPELL_interface_def(
#ifdef PROTO
    FILE *fid,
    AST_interface_n_t *ifp,
    BE_output_k_t kind,
    boolean generate_mepv
#endif
);

void CSPELL_manager_epv
(
#ifdef PROTO
    FILE *fid,
    AST_interface_n_t *ifp
#endif
);

#endif
