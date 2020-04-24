/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_ufs_mach.c,v $
 * Revision 1.1.119.1  1996/10/02  17:59:33  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:38  damon]
 *
 * Revision 1.1.114.2  1994/06/09  14:16:35  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:47  annie]
 * 
 * Revision 1.1.114.1  1994/02/04  20:25:43  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:16:06  devsrc]
 * 
 * Revision 1.1.112.1  1993/12/07  17:30:32  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:31:47  jaffe]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1993, 1994 Transarc Corporation.  All rights reserved.
 */

#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>

/*
 * Given a device name, return the device number and a vnode ptr.
 * If passed a non-null vpp, the vnode will be passed back in vpp
 *    and it will be held (VN_HOLD).  The caller assumes responsibility
 *    for VN_RELEasing it.
 * If, on the other hand, vpp is NULL, then the vnode will be
 *    VN_RELEased here, since the caller would have no handle on the
 *    vnode with which to release it.
 */
int osi_getvdev(fspec, devp, vpp)
    caddr_t fspec;			/* device pathname */
    dev_t *devp;			/* place to put device number */
    struct vnode **vpp;			/* device vnode */
{
    int code = 0;			/* error return code */
    struct vnode *vp;

    if (code = lookupvp (fspec, L_SEARCHLOC, &vp))
        return (code);

    if (osi_vType (vp) != VBLK) {
	OSI_VN_RELE(vp);
        code = ENOTBLK;
    } else {
	*devp = brdev(vp->v_rdev);

	if (vpp) {
	    /* caller intends to use the vnode,
	       therefore caller assumes responsibility for
	       VN_RELEasing it */
	    *vpp = vp;
	} else {
	    /* caller wasn't interested in the vnode */
	    OSI_VN_RELE(vp);
	}
    }
    return (code);
}
