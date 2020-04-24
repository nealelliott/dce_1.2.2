/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: auth_at_ki.c,v $
 * Revision 1.1.4.1  1996/10/02  17:50:29  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:40  damon]
 *
 * Revision /main/HPDCE02/1  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/maunsell_mothra1.45/2  1995/01/31  21:12 UTC  maunsell_c
 * 	fix to return 0 or 1 as spec'd
 * 
 * Revision /main/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
 * 	add @sys @host support for dfs_login/dfsgwd/dfsgw
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
 * authenticate remote users into HP-DFS.  A single call is provided
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

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/gateway/libgwauth/auth_at_ki.c,v 1.1.4.1 1996/10/02 17:50:29 damon Exp $")

int
at_translate_creds(addr, uid, cred) 
struct sockaddr *addr;
uid_t uid;
osi_cred_t *cred;
{
	at_pag_mapping_t mapping;
	struct sockaddr_in *insock;
	int result = 0;
	switch (addr->sa_family) {
	case AF_INET :
		/*
		 * We are only interested in the internet address
		 * which is the actual network id/network address.
		 * So we ignore the port that is encoded in the 
		 * internet-style socket.
	 	 */ 
		insock = (struct sockaddr_in *)addr;
		bzero((char *)&mapping.addr, sizeof(struct sockaddr));
		mapping.addr.sa_family = AF_INET;
		bcopy((char *)&insock->sin_addr,
		      (char *)&mapping.addr.sa_data[0],
		      sizeof(struct in_addr));
		break;
	default :
		return(result);
	}
	mapping.uid = uid;
	if (at_query(&mapping) == 0) {
		/* XXX Why don't we check the return value? */
		(void) osi_SetPagInCred(cred, mapping.pag);
		result = 1;
	}
	return(result);
}

