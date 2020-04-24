/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cstubgen.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:02  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:15:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:10:50  root]
 * 
 * Revision 1.1.4.2  1993/07/07  19:57:25  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:31:01  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1990 by
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
**      cstubgen.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Function prototypes for cstubgen.c
**
**  VERSION: DCE 1.0
**
*/

#ifndef CSTUBGEN_H
#define CSTUBGEN_H

#include <clihandl.h>

extern BE_handle_info_t BE_handle_info;

void CSPELL_test_status (
#ifdef PROTO
    FILE *fid
#endif
);

void CSPELL_test_transceive_status
(
#ifdef PROTO
    FILE *fid
#endif
);

void BE_gen_cstub(
#ifdef PROTO
    FILE *fid,                      /* Handle for emitted C text */
    AST_interface_n_t *p_interface, /* Ptr to AST interface node */
    language_k_t language,          /* Language stub is to interface to */
    char header_name[],             /* Name of header file to be included in stub */
    boolean bugs[],                 /* List of backward compatibility "bugs" */
    boolean generate_cepv           /* generate cepv if true */
#endif
);

#endif
