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
 * $Log: cspeldcl.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:01  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:15:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:02 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:10:39  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:47:38  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:43:10  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:56:03  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:23:52  bfc]
 * 
 * Revision 1.1.4.2  1993/07/07  19:57:08  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:30:49  ganni]
 * 
 * $EndLog$
 */
/*
**  @OSF_COPYRIGHT@
**
**  Copyright (c) 1989, 1990, 1991, 1992, 1993 by
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
**      cspeldcl.h
**
**  FACILITY:
**
**      IDL Compiler Backend
**
**  ABSTRACT:
**
**  Header file for cspeldcl.c
**
**  VERSION: DCE 1.0
**
*/

#ifndef CSPELDCL_H
#define CSPELDCL_H

extern void CSPELL_constant_val (
#ifdef PROTO
    FILE *fid, AST_constant_n_t *cp
#endif
);

extern void CSPELL_labels (
#ifdef PROTO
    FILE *fid, AST_case_label_n_t *tgp
#endif
);

extern void CSPELL_parameter_list (
#ifdef PROTO
    FILE        *fid,
    AST_parameter_n_t *pp,
    boolean encoding_services
#endif
);

extern void CSPELL_finish_synopsis (
#ifdef PROTO
    FILE *fid, AST_parameter_n_t *paramlist
#endif
);


typedef enum {base_class, stub_class, mgr_class} class_t;

#endif
