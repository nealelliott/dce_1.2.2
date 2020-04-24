/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_ufs.c,v $
 * Revision 1.1.668.1  1996/10/02  18:11:52  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:45:14  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1994, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_kvnode.h>

#ifdef	AFS_OSF_ENV
#include <sys/specdev.h>
#endif	/* AFS_OSF_ENV */

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/osi/osi_ufs.c,v 1.1.668.1 1996/10/02 18:11:52 damon Exp $")


#if !defined(KERNEL)
/*
 * Given a device name, return the device number and a vnode ptr.
 * If passed a non-null vpp, the vnode will be passed back in vpp
 *    and it will be held (VN_HOLD).  The caller assumes responsibility
 *    for VN_RELEasing it.
 * If, on the other hand, vpp is NULL, then the vnode will be
 *    VN_RELEased here, since the caller would have no handle on the
 *    vnode with which to release it.
 * This is going to remain the USER mode version.  Kernel mode versions
 * will be found in MACHINE/osi_ufs_mach.c.
 */
int osi_getvdev(fspec, devp, vpp)
    caddr_t fspec;			/* device pathname */
    dev_t *devp;			/* place to put device number */
    struct vnode **vpp;			/* device vnode */
{
    int code;				/* error return code */
    struct vnode *vp;

    if (code = osi_lookupname (fspec, OSI_UIOSYS, FOLLOW_LINK, 0, &vp))
	return (code);

    if (osi_vType (vp) != VBLK) {
	OSI_VN_RELE(vp);
	code = ENOTBLK;
    } else {
	if (devp) {
	    *devp = vp->v_rdev;
#ifdef	AFS_AIX31_ENV
	    *devp = brdev(*devp);	/* is this really necessary? */
#endif
	}
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
#endif	/* !KERNEL */
