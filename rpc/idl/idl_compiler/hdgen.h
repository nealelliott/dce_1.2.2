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
 * $Log: hdgen.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:12  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:18:20  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/20  20:34 UTC  jrr
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/11/17  17:03 UTC  dat  /main/dat_xidl2/1]
 * 
 * 	oct 95 idl drop
 * 	[1995/10/23  00:43:50  bfc]
 * 	 *
 * 	may 95 idl drop
 * 	[1995/10/21  22:57:26  bfc]
 * 	 *
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:33  bfc]
 * 	 *
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/05  20:11 UTC  lmm
 * 	Merge idl++ fixes into Mothra
 * 
 * 	HP revision /main/lmm_idlxx_fixes/1  1994/12/07  17:03 UTC  lmm
 * 	Don't spell prototypes when both -no_mepv and -cepv are spec'd in idl++
 * 	[1995/12/07  21:11:44  root]
 * 
 * Revision 1.1.4.2  1993/07/07  19:59:29  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:32:30  ganni]
 * 
 * $EndLog$
 */
/*
**  @OSF_COPYRIGHT@
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
**      hdgen.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Function prototypes for hdgen.c
**
**  VERSION: DCE 1.0
**
*/

#ifndef HDGEN_H
#define HDGEN_H


void BE_gen_c_header(
#ifdef PROTO
    FILE *fid,              /* Handle for emitted C text */
    AST_interface_n_t *ifp, /* Ptr to AST interface node */
    boolean bugs[],         /* List of backward compatibility "bugs" */
    boolean cepv_opt        /* -cepv option present */
#endif
);

int BE_is_handle_param(
#ifdef PROTO
    AST_parameter_n_t *p    /* [in] ptr to parameter node */
#endif
);

void BE_gen_cxx_class(
#ifdef PROTO
    FILE *fid,
    AST_interface_n_t   *ifp,
    class_t c    
#endif
);


void CSPELL_type_def
(
#ifdef PROTO
    FILE *fid,
    AST_type_n_t *tp,
    boolean spell_tag
#endif
);

char *mapchar
(
#ifdef PROTO
    AST_constant_n_t *cp,   /* Constant node with kind == AST_char_const_k */
    boolean warning_flag    /* TRUE => log warning on nonportable escape char */
#endif
);

#endif /* HDGEN_H */

