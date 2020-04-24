/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acf_lex_yy.c,v $
 * Revision 1.1.8.2  1996/02/18  18:50:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:04:27  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:12:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:09:33  root]
 * 
 * Revision 1.1.4.2  1993/07/07  19:54:08  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:29:02  ganni]
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
**      acf.y
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Main module for ACF lexical analyzer.  This module is necessary in
**      order to assure that acf.h is included into lex-generated code before
**      anything else (see acf.h for a description of why this is necessary).
**      lex puts some of its generated definitions BEFORE any user-included
**      code, and therefore the same thing can not be accom- plished by merely
**      putting "#include acf.h" in the lex source.
**
*/

#include <acf.h>                /* Renames globals */
#include <acf_lex_yy_i.c>       /* Generated by lex */