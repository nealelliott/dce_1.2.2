/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_anode.h,v $
 * Revision 1.1.27.1  1996/10/02  17:24:04  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:31:49  damon]
 *
 * $EndLog$
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/eacl/dacl_anode.h,v 1.1.27.1 1996/10/02 17:24:04 damon Exp $ */
/*
 * Copyright (C) 1995, 1993 Transarc Corporation
 * All rights reserved.
 */

#ifndef TRANSARC_DACL_ANODE_H
#define TRANSARC_DACL_ANODE_H

#include <dce/id_base.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/dacl.h>
#include <dcedfs/epi_id.h>
#include <dcedfs/episode/anode.h>

/*
 * Functions exported by dacl_anode.c.
 */
extern void eacl_AllocateAcl(dacl_t **);
extern long eacl_ReadAclFromAnode(
    dacl_t *, epi_anode_t, u_int32, int, epi_uuid_t *);
extern void eacl_ReleaseAcl(dacl_t *);

/*
 * Function exported by dacl_xcred.c.
 */
extern void eacl_FindPacList(osi_cred_t *, pac_list_t ***, u_int32 *);
extern void eacl_FindPac(osi_cred_t *, sec_id_pac_t **);

extern void eacl_Init(void);

#endif /* !TRANSARC_DACL_ANODE_H */
