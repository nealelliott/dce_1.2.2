/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: auth_at_ki.c,v $
 * Revision 1.1.2.2  1996/02/18  18:35:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:55:11  marty]
 *
 * Revision 1.1.2.1  1995/12/08  21:39:31  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/DCE1_1_WP/jrr_1.2_mothra/1  1995/11/13  18:23 UTC  dat
 * 	Add missing "}" and assign req_uid, not uid to mapping .uid
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/06/26  20:25 UTC  maunsell_c
 * 	osf dce1.2 gway
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/22  20:14 UTC  maunsell_c
 * 	mothra C&S fixes
 * 
 * 	HP revision /main/brezak_transarc/1  1995/06/01  22:03 UTC  brezak
 * 	Cut at auth_at_ki.c for RIOS
 * 	[1995/12/08  18:11:26  root]
 * 
 * $EndLog$
 */
/*
 * auth_at_ki.c - Kernel interface to AT database.
 *
 * This module provides a clean, simple way for alien file systems to
 * authenticate remote users into DFS.  A single call is provided
 * that does all the necessary AT database checking and cred installing.
 *
 * Returns 1 on cred match, 0 otherwise.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <dcedfs/osi.h>
#include <dcedfs/auth_at.h>

int
at_translate_creds(
    unsigned long handle,
    fsid_t *fsid,
    unsigned long addr,
    uid_t req_uid,
    osi_cred_t *cred,
    void * exi)
{
    at_pag_mapping_t mapping;
    u_long newpag;
    int result = 0;

    if ((fsid->val[0] != AFS_VFSMAGIC) || (fsid->val[1] != AFS_VFSFSID)) {
        return 0;       /* Only do requests for the DFS cache manager */
    }

    bzero(&mapping.addr, sizeof(struct sockaddr));
    mapping.addr.sa_family = AF_INET;
    bcopy(&addr, &mapping.addr.sa_data[0], sizeof(struct in_addr));
    mapping.uid = req_uid;

    if (at_query(&mapping) == 0) {
	(void) osi_SetPagInCred(&cred, mapping.pag, &newpag, 1);
	result = 1;
    }
    return(result);
}

