/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: aux_verify.c,v $
 * Revision 1.1.71.1  1996/10/02  17:27:16  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:33:53  damon]
 *
 * $EndLog$
*/
/* Copyright (C) 1995, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/salvage/aux_verify.c,v 1.1.71.1 1996/10/02 17:27:16 damon Exp $")

#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/file.h>
#include <dcedfs/dir_buffer.h>
#include <dcedfs/dir.h>
#include <dcedfs/episode/salvage.h>
#include <dcedfs/dacl.h>

#include <salvager.h>

SHARED long epis_VerifyAcl(
  char *buffer,
  u_long length,
  struct epiz_salvageState *ss,
  opaque rock)
{
    long code = 0;
    long a_numEntriesP;
    u_int len = length;

    /* Make no repairs but detect unauth_mask */
    code = dacl_epi_ValidateBuffer (buffer, &len, DACL_VB_FIND_UNAUTH_MASK, 0);
    if (code == DACL_FOUND_UNAUTH_MASK) {
	epiz_ReportError (ss, 0, EPIZ_E_AUX_UNAUTHMASK);
	code = 0;
    }
    return(code);
}

SHARED long epis_VerifyPlist(
  char *buffer,
  u_long length,
  struct epiz_salvageState *ss,
  opaque rock)
{
#if 0
    printf( "epis_VerifyPlist: \n" );
#endif
    return(0);
}
