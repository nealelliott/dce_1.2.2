/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: kinit.h,v $
 * Revision 1.1.4.1  1996/10/02  17:49:37  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:07  damon]
 *
 * Revision 1.1.2.1  1994/07/26  19:12:54  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:38  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:08:07  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfs_login/kinit.h,v 1.1.4.1 1996/10/02 17:49:37 damon Exp $ */

/*
 * kinit.h - Prototypes for kinit module
 */

#ifndef KINIT_H
#define KINIT_H

#define KRB5_DEFAULT_OPTIONS 0
#define KRB5_DEFAULT_HOURS 8
#define KRB5_DEFAULT_LIFE (60*60*(KRB5_DEFAULT_HOURS))

/*
 * Functions.
 */
int kinit
#ifdef __STDC__
	(char *pname, char *opassword, time_t lifetime, ...);
#else
	();
#endif 

#endif /* KINIT_H */
