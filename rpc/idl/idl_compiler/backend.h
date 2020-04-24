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
 * $Log: backend.h,v $
 * Revision 1.1.8.2  1996/02/18  23:44:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:43:53  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:13:36  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/20  19:02 UTC  jrr
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/11/17  17:01 UTC  dat  /main/dat_xidl2/1]
 * 
 * 	oct 95 idl drop
 * 	[1995/10/23  00:42:54  bfc]
 * 	 *
 * 	may 95 idl drop
 * 	[1995/10/21  22:55:26  bfc]
 * 	 *
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:23:31  bfc]
 * 	 *
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/01  23:51 UTC  lmm
 * 	Merging idl++ mods into Mothra
 * 
 * 	HP revision /main/OODCE01/1  1994/05/24  19:59 UTC  griffith_j
 * 	create branch for OODCE idl++ compiler
 * 
 * 	HP revision /main/lmm_idl_cxx/1  1994/05/23  20:05 UTC  lmm
 * 	OODCE IDL extensions
 * 	[1995/12/07  21:09:57  root]
 * 
 * Revision 1.1.4.2  1993/07/07  19:55:41  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:29:44  ganni]
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
**      backend.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Header for backend.c
**
**  VERSION: DCE 1.0
**
*/


#ifndef BACKEND_H
#define BACKEND_H


boolean BE_main
(
#ifdef PROTO
    boolean             *cmd_opt,   /* [in] array of cmd option flags */
    void                **cmd_val,  /* [in] array of cmd option values */
    FILE                *h_fid,     /* [in] header file handle, or NULL */
    FILE                *caux_fid,  /* [in] client aux file handle, or NULL */
    FILE                *saux_fid,  /* [in] server aux file handle, or NULL */
    FILE                *cstub_fid, /* [in] cstub file handle, or NULL */
    FILE                *sstub_fid, /* [in] sstub file handle, or NULL */
    FILE                *cxxmgr_fid,/* [in] C++ manager class handle, or NULL */
    AST_interface_n_t   *int_p      /* [in] ptr to interface node */
#endif
);

void BE_push_malloc_ctx
(
#ifdef PROTO
      void
#endif
);

void BE_push_perm_malloc_ctx
(
#ifdef PROTO
      void
#endif
);

void BE_pop_malloc_ctx
(
#ifdef PROTO
      void
#endif
);

heap_mem *BE_ctx_malloc
(
#ifdef PROTO
      int size
#endif
);


#endif /* BACKEND_H */
