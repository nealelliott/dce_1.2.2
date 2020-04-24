/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_anode.c,v $
 * Revision 1.1.66.1  1996/10/02  17:24:02  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:31:49  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1995, 1991 Transarc Corporation
 * All rights reserved.
 */

/*
 *			Episode File System
 *			      ACL Pool
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/episode/dacl_anode.h>
#include <dcedfs/episode/file.h>		/* EPIF_AUX_IS_ACL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/eacl/dacl_anode.c,v 1.1.66.1 1996/10/02 17:24:02 damon Exp $")

/*
 * eacl_AllocateAcl -- allocate memory for an ACL
 *
 * This only allocates a header block.  The entries are allocated during
 * eacl_ReadFromAnode.
 *
 * TO DO:
 *	We are using a brutal method, with lots of calls to osi_Alloc.  Would
 *	like to improve this in the future.
 */

void
eacl_AllocateAcl(dacl_t **aclPP)
{
    *aclPP = (dacl_t *) osi_Alloc_r(sizeof (dacl_t));
    bzero((char *) *aclPP, sizeof (dacl_t));
}

/*
 * eacl_ReadAclFromAnode -- read an anode's ACL into memory
 */

long
eacl_ReadAclFromAnode(
    dacl_t *aclP,			/* where to put acl data */
    epi_anode_t ap,			/* Episode anode handle */
    u_int32 aclsize,			/* bytes in disk acl */
    int flag,				/* which acl to get */
    epi_uuid_t *mgrUuidP)		/* ACL manager uuid */
{
    char *bufferP;			/* where to put image of disk acl */
    long rtnVal = 0;			/* error return code */

    bufferP = osi_Alloc_r(aclsize);

    rtnVal = epif_ReadAux(ap, flag, (u_long)0, aclsize, bufferP);
    if (rtnVal == 0) {
	osi_UnmakePreemptionRight();
	rtnVal = dacl_ParseAcl (bufferP, aclsize, aclP, mgrUuidP);
	osi_MakePreemptionRight();
    }
    osi_Free_r(bufferP, aclsize);

    return rtnVal;
}

/*
 * eacl_ReleaseAcl -- release storage associated with an ACL
 */

void
eacl_ReleaseAcl(dacl_t *aclP)
{
    osi_UnmakePreemptionRight();
    dacl_FreeAclEntries(aclP);
    osi_MakePreemptionRight();
    osi_Free_r((opaque)aclP, sizeof (dacl_t));
}
