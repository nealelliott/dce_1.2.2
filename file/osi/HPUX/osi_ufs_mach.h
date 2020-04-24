/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_ufs_mach.h,v $
 * Revision 1.1.127.1  1996/10/02  17:57:55  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:38  damon]
 *
 * Revision 1.1.122.2  1994/06/09  14:15:22  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:53  annie]
 * 
 * Revision 1.1.122.1  1994/02/04  20:24:25  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:35  devsrc]
 * 
 * Revision 1.1.120.1  1993/12/07  17:29:41  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:31:03  jaffe]
 * 
 * $EndLog$
 */
/*
 * Copyright (C) Transarc Corp. 1993, 1994  All rights reserved
 */
/*
 * $Header: /u0/rcs_trees/dce/rcs/file/osi/HPUX/osi_ufs_mach.h,v 1.1.127.1 1996/10/02 17:57:55 damon Exp $
 */

#define osi_CantCVTVP(vp)	0

/* Macros for opening/closing files, used mostly for device files */

#define OSI_OP_OPEN(vp,flags,cred,code) code = VOP_OPEN (&vp, flags, cred)
#define OSI_OP_CLOSE(vp,flags,cred) VOP_CLOSE (vp, flags, cred)
