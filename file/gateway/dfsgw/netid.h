/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: netid.h,v $
 * Revision 1.1.6.1  1996/10/02  17:50:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:23  damon]
 *
 * Revision /main/HPDCE02/1  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
 * 	add @sys @host support for dfs_login/dfsgwd/dfsgw
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:14  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:51  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:01:55  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgw/netid.h,v 1.1.6.1 1996/10/02 17:50:01 damon Exp $ */
/* 
 * netid.h - Network ID.
 *
 * In unix there is a "standard" way of representing a network address, using a
 * struct sockaddr.  However the sockaddr structure just holds space
 * to hold the address, and no structure or form is implied.  In
 * practice, network subsystems mold their own representation of 
 * addresses over the sockaddr structure; for example, internet
 * addresses use a struct sockaddr_in.  
 *
 * To handle this problem, we use this basic abstraction with the
 * properties that it can be initialized from some string, retrieved
 * by a struct sockaddr, and printed back out again.  Individual network
 * implementations of the object straighten out the differences.
 */

#ifndef _NETID_H_
#define _NETID_H_

#include <sys/types.h>
#include <sys/socket.h>


typedef struct netid_t {
       int (*assign_id_str)();          /* initialize with string */
       int (*assign_id_addr)();         /* initialize with addr */
       int (*get_sockaddr)();           /* return addr in sockaddr form */
       int (*get_str)();                /* return addr in string form */
       int valid;
       struct sockaddr addr;        
} netid_t;

typedef netid_t *netid_p;

#endif /* _NETID_H_ */
