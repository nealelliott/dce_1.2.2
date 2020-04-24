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
 * Revision 1.1.2.2  1996/02/18  18:35:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:55:09  marty]
 *
 * Revision 1.1.2.1  1995/12/08  21:39:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/06/26  20:25 UTC  maunsell_c
 * 	osf dce1.2 gway
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/23  14:15 UTC  maunsell_c
 * 	merge
 * 
 * 	HP revision /main/brezak_dfsgw/3  1995/06/07  21:37 UTC  brezak
 * 	Add assertion for not INET fam
 * 
 * 	HP revision /main/brezak_dfsgw/2  1995/06/01  21:58 UTC  brezak
 * 	return appropriate error code for default nettype.
 * 
 * 	HP revision /main/brezak_dfsgw/1  1995/05/18  16:22 UTC  brezak
 * 	a branch
 * 	[1995/12/08  18:11:21  root]
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:36  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:58:07  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:07:24  mckeen]
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
at_translate_creds(addr, uid, cred) 
    struct sockaddr *addr;
    uid_t uid;
    struct ucred *cred;
{
    at_pag_mapping_t mapping;
    u_long newpag;
    struct sockaddr_in *insock;
    int result = 0;
	
    if (addr->sa_family == AF_INET) {
	/*
	 * We are only interested in the internet address
	 * which is the actual network id/network address.
	 * So we ignore the port that is encoded in the 
	 * internet-style socket.
	 */ 
	insock = (struct sockaddr_in *)addr;
	bzero(&mapping.addr, sizeof(struct sockaddr));
	mapping.addr.sa_family = AF_INET;
	bcopy(&insock->sin_addr, &mapping.addr.sa_data[0], sizeof(struct in_addr));

	mapping.uid = uid;
	if (at_query(&mapping) == 0) {
	    (void) osi_SetPagInCred(&cred, mapping.pag, &newpag, 1);
	    result = 1;
	}
    }
#ifdef AFS_DEBUG
    else
	printf("at_translate_creds: unknown address family - %d\n", addr->sa_family);
#endif
    return result;
}

