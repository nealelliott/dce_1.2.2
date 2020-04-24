/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * Copyright (c) 1995 Transarc Corp.  All rights reserved.
 */
/*
 * HISTORY
 * $Log: auth_at.h,v $
 * Revision 1.1.6.1  1996/10/02  17:50:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:39  damon]
 *
 * Revision /main/HPDCE02/2  1995/06/22  20:14 UTC  maunsell_c
 * 	mothra C&S fixes
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/4  1995/06/14  22:15 UTC  brezak
 * 	Change prototype for at_atname().
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/3  1995/06/12  22:40 UTC  brezak
 * 	Cleanup.
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/2  1995/06/09  03:08 UTC  brezak
 * 	Fixes from Transarc
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/1  1995/05/26  17:26 UTC  brezak
 * 	Add prototypes for external func.
 * 	Remove unused struct.
 * 
 * Revision /main/HPDCE02/1  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/maunsell_mothra1.45/2  1995/01/31  20:47 UTC  maunsell_c
 * 	fixed include path for cm.h
 * 
 * Revision /main/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
 * 	add @sys @host support for dfs_login/dfsgwd/dfsgw
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:35  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:58:06  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:07:23  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/libgwauth/auth_at.h,v 1.1.6.1 1996/10/02 17:50:27 damon Exp $ */
/*
 * auth_at.h - Authentication table interface.
 *
 * The authentication table (AT) holds a list of translations:
 * (nodeID, userID) -> PAG that are used for alien access of HP-DFS.
 */

#ifndef AUTH_AT_H
#define AUTH_AT_H

#include <dcedfs/cm.h>

/*
 * Mapping table entry, used in most AT syscalls.
 */
typedef struct {
	struct sockaddr addr;	 
	uid_t uid;
	unsigned long pag;
	time_t expiration_time;
        char sysname[CM_MAXSYSNAME];
        char hostname[OSI_MAXHOSTNAMELEN];
} at_pag_mapping_t;

typedef at_pag_mapping_t *at_pag_mapping_p;

/*
 * Syscall options for AFSCALL_AT.
 */
#define AT_OBSOLETE	(0x0)		/* Obsolete */
#define AT_DELETE	(0x1)		/* Delete entry */
#define AT_QUERY	(0x2)		/* Query entry */
#define AT_LIST		(0x3)		/* Return contents of AT */
#define AT_GETSIZE	(0x4)		/* Return size of AT */
#define AT_ADD          (0x5)		/* Add entry. See subcmds below */
#define AT_NSYSCALLS    (0x6)

/* 
 * Sub options for the AT_ADD cmd
 */

#define AT_ADD_NEW_ENTRY 	(0x0)
#define AT_ADD_UPDATE_EXPTIME 	(0x1)
#define AT_ADD_NSUBCMDS		(0x2)


/*
 * AT_ATNAME operation 
 * for @sys @host  
 */
#define ATSYS           (0x01)         /* get @sys sysname */
#define ATHOST          (0x02)         /* get @host hostname */

#ifdef _KERNEL
extern void at_configure _TAKES((void));
#endif

extern int at_query(at_pag_mapping_p mapping);
extern int at_add(at_pag_mapping_p mapping, int subcmd);
extern int at_delete(at_pag_mapping_p mapping);
extern int at_size(void);
extern int at_list(int *nmappings, at_pag_mapping_p mappings);
extern int at_atname (int type, int pag, char *np, int nsz);
extern void at_gc(void);

extern struct icl_set *dfsgw_iclSetp;

#endif /* AUTH_AT_H */
