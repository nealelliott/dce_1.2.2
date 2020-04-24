/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsgw_userint.h,v $
 * Revision 1.1.6.1  1996/10/02  17:49:55  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:20  damon]
 *
 * Revision /main/HPDCE02/1  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
 * 	add @sys @host support for dfs_login/dfsgwd/dfsgw
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:07  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:46  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:01:52  mckeen]
 * 
 * $EndLog$
 */
/*
 * dfsgw_userint.h
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgw/dfsgw_userint.h,v 1.1.6.1 1996/10/02 17:49:55 damon Exp $ */
#ifndef _DFSGW_USERINT_H_
#define _DFSGW_USERINT_H_

/*** param offsets ***/
#define NET_ID_OFFSET              0 
#define DCE_ID_OFFSET              1 
#define SYSNAME_OFFSET             2
#define REMOTEHOST_OFFSET          3
#define ADDR_FAMILY_OFFSET         4 

/* the operation that main needs to call to get things set up */
IMPORT void SetUpSyntax _TAKES((void));

/* the operations to which the setup routines need to refer */
IMPORT int dfsgw_AddCmd _TAKES((
                                 struct cmd_syndesc *as,
                                 char *arock
                               ));

IMPORT int dfsgw_DeleteCmd _TAKES((
                                 struct cmd_syndesc *as,
                                 char *arock
                               ));

IMPORT int dfsgw_QueryCmd _TAKES((
                                 struct cmd_syndesc *as,
                                 char *arock
                               ));

IMPORT int dfsgw_ListCmd _TAKES((
                                 struct cmd_syndesc *as,
                                 char *arock
                               ));


#endif /* _DFSGW_USERINT_H_ */
