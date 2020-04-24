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
 * Revision 1.1.184.1  1996/10/02  17:58:44  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:09  damon]
 *
 * Revision 1.1.179.2  1994/06/09  14:15:57  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:18  annie]
 * 
 * Revision 1.1.179.1  1994/02/04  20:25:06  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:52  devsrc]
 * 
 * Revision 1.1.177.1  1993/12/07  17:30:09  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:31:33  jaffe]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1993, 1994 Transarc Corporation.  All rights reserved.
 */

#define osi_CantCVTVP(vp)	0

/* Macros for opening/closing files, used mostly for device files */

#define OSI_OP_OPEN(vp,flags,cred,code) VOP_OPEN(&vp, flags, cred, code)
#define OSI_OP_CLOSE(vp,flags,cred)			\
    {							\
	int ignore_error;				\
	VOP_CLOSE(vp, flags, cred, ignore_error);	\
    }
