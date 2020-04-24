/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_kvnode_mach.h,v $
 * Revision 1.1.379.1  1996/10/02  17:58:25  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:56  damon]
 *
 * Revision 1.1.374.2  1994/06/09  14:15:39  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:08  annie]
 * 
 * Revision 1.1.374.1  1994/02/04  20:24:52  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:46  devsrc]
 * 
 * Revision 1.1.372.1  1993/12/07  17:30:00  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:02:04  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/21  14:50:35  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:52:42  cjd]
 * 
 * Revision 1.1.2.3  1992/09/25  18:32:30  jaffe
 * 	Remove duplicate HEADER and LOG entries
 * 	[1992/09/25  12:27:57  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  20:25:01  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    new file for machine specific additions for the osi_kvnode.h file.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  03:17:09  jaffe]
 * 
 * Revision 1.1.1.2  1992/08/30  03:17:09  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    new file for machine specific additions for the osi_kvnode.h file.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 
 * $EndLog$
 */
/*
 *      Copyright (C) 1992 Transarc Corporation
 *      All rights reserved.
 */

#ifndef TRANSARC_OSI_KVNODE_MACH_H
#define	TRANSARC_OSI_KVNODE_MACH_H

#ifndef KERNEL

/*
 * Misc flags
 *
 * If we have a definition of IO_UNIT, then assume that the other three are
 * also defined.
 */
#ifndef IO_UNIT
#define IO_UNIT		0x01		/* do io as atomic unit for VOP_RDWR */
#define IO_APPEND	0x02		/* append write for VOP_RDWR */
#define IO_SYNC		0x04		/* sync io for VOP_RDWR */
#define LOOKUP_DIR	0x08		/* want parent dir. vnode, VOP_LOOKUP */
#endif /* IO_UNIT */

/*
 * public vnode manipulation functions
 */
extern int vn_open(), vn_create(), vn_rdwr(), vn_close(), vn_link();
extern vn_rename(), vn_remove(), getvnodefp();
extern void vn_rele(), vattr_null();

#define VN_HOLD(VP)	{ (VP)->v_usecount++; }
#define VN_RELE(VP)	{ vn_rele(VP); }


#define	VOP_OPEN(vpp,f,c,r) (r) = (*((*vpp)->v_op->vn_open))((vpp),(f),(c))
#define	VOP_CLOSE(v,f,c,r)  (r) = (*((v)->v_op->vn_close))((v),(f),(c))
#endif /* ! KERNEL */

#endif /* TRANSARC_OSI_KVNODE_MACH_H */
