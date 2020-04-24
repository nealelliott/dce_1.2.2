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
 * Revision 1.1.126.1  1996/10/02  17:57:53  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:37  damon]
 *
 * Revision 1.1.121.2  1994/06/09  14:15:20  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:52  annie]
 * 
 * Revision 1.1.121.1  1994/02/04  20:24:23  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:34  devsrc]
 * 
 * Revision 1.1.119.1  1993/12/07  17:29:40  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:30:57  jaffe]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1993, 1994 Transarc Corporation.  All rights reserved.
 */

#include <dcedfs/osi.h>
#include <dcedfs/osi_ufs.h>

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
    int 	code = 0;	/* error return code */
    dev_t	dev;
    struct vnode *vp, *realvp;

    if (code = osi_lookupname (fspec, OSI_UIOSYS, FOLLOW_LINK, 0, &vp))
	return (code);

    if (osi_vType(vp) != VBLK) {
	OSI_VN_RELE(vp);
	code = ENOTBLK;
    } else {
	/* Get device number */
	dev = vp->v_rdev;

	/* Get the real thing. */
	realvp = devtovp(dev);
	*devp = realvp->v_rdev;

	/*
	 * Throw away the device vnode! Why?
	 * Have to do this because it isn't the ``real''
	 * spec vnode needed for ops like mount.
	 */
	OSI_VN_RELE(vp);

	if (vpp) {
	    /* caller intends to use the vnode,
	       therefore caller assumes responsibility for
	       VN_RELEasing it */
	    *vpp = realvp;
	} else {
	    /* caller wasn't interested in the vnode */
	    OSI_VN_RELE(realvp);
	}
    }
    return (code);
}
