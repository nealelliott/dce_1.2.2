/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_kvol.h,v $
 * Revision 1.1.11.1  1996/10/02  17:44:04  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:51  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:05:30  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:21:04  annie]
 * 
 * Revision 1.1.4.3  1993/01/21  18:26:58  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  16:49:57  zeliff]
 * 
 * Revision 1.1.4.2  1992/08/31  21:10:38  jaffe
 * 	Fixed Header
 * 	[1992/08/30  14:50:22  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/05  16:15:43  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  23:53:24  jdp]
 * 
 * Revision 1.1  1992/01/19  02:39:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* $Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_kvol.h,v 1.1.11.1 1996/10/02 17:44:04 damon Exp $ */

/*
 *	    Definitions and declarations for testing outside the kernel.
 *	These are required because some definitions in volume.h are not
 *	compiled when KERNEL is off.
 */

#ifndef TRANSARC_EFS_KVOL_H
#define TRANSARC_EFS_KVOL_H 1

#ifndef KERNEL

/*
 * Internal volume handle for all VOP_* calls.
 * In-kernel structure only; a volume descriptor (long) is passed to the user
 * land to associate it with this structure.
 */
struct vol_handle {		
    struct volume *volp;		    /* pointer to incore volume */
    long opentype;			    /* Open volume type */
    struct afsFid fid;			    /* Fid for "active" volume anode */
    long spare1;
    long spare2;
};

#endif /* KERNEL */

#endif /* TRANSARC_EFS_KVOL_H */
