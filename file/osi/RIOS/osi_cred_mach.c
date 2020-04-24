/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994 Transarc Corporation.  All rights reserved.
 */

#include <dcedfs/osi.h>
#include <dcedfs/osi_cred.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/osi/RIOS/RCS/osi_cred_mach.c,v 1.3 1995/07/18 22:58:13 blake Exp $")

int
osi_crequal(struct ucred *cred1p, struct ucred *cred2p)
{
    int notEqual, i;

    /*
     * Be conservative and avoid a bcmp of the cred structures, since
     * I don't have access to AIX sources currently to check whether that
     * would be correct.
     *
     * An occasional mismatch due to the groups being in different
     * order is not a critical factor (due to the current usage of this
     * routine in Episode access caching).
     */
    notEqual = ((cred1p->cr_uid != cred2p->cr_uid) ||
		(cred1p->cr_gid != cred2p->cr_gid) ||
		(cred1p->cr_ngrps != cred2p->cr_ngrps));
    /* Rely on check in the for loop */
    for (i = 0; !notEqual && i < cred1p->cr_ngrps; i++) {
	notEqual = (cred1p->cr_groups[i] != cred2p->cr_groups[i]);
    }
    return (notEqual == 0);
}
