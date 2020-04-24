/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sendrcv.h,v $
 * Revision 1.1.4.1  1996/10/02  17:49:41  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:08  damon]
 *
 * Revision /main/HPDCE02/1  1995/06/22  20:14 UTC  maunsell_c
 * 	mothra C&S fixes
 * 
 * Revision /main/maunsell_mothra1.48/2  1995/06/16  16:36 UTC  maunsell_c
 * 	audit cleanup
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:28  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:58:01  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:05:42  mckeen]
 * 
 * $EndLog$
 */
/*
 * sendrcv.h - Header file for send/rcv function.
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfs_login/sendrcv.h,v 1.1.4.1 1996/10/02 17:49:41 damon Exp $ */

#ifndef SENDRCV_H
#define SENDRCV_H

/*
 * timeout large enough to avoid routine re-transmits
 */
#define SR_DMXTIMO	(120)		/* Default maximum timeout */
#define SR_DITIMO	(20)		/* Default initial timeout */
#define SR_DINCR	(10)		/* Default increment */

int sendrcv
#ifdef __STDC__
	(int sock, char *sdata, int slen, unsigned long itimeout, int tincr, int mxtimeout, int *dlen, char *ddata);
#else 
	();
#endif /* __STDC__ */
#endif /* SENDRCV_H */ 
