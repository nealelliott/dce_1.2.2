/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_uio_mach.h,v $
 * Revision 1.1.321.1  1996/10/02  17:59:38  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:40  damon]
 *
 * $EndLog$
 */
/*
 *      Copyright (C) 1996, 1990 Transarc Corporation
 *      All rights reserved.
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/osi/RIOS/osi_uio_mach.h,v 1.1.321.1 1996/10/02 17:59:38 damon Exp $ */

#ifndef TRANSARC_OSI_UIO_MACH_H
#define	TRANSARC_OSI_UIO_MACH_H

#include <sys/uio.h>

/* 
 * This file contains standard macros that are used to isolate OS
 * system dependencies for uio servides
 */

/*
 * uio vector related definitions
 */
#define	osi_uio_iov		uio_iov
#define	osi_uio_iovcnt		uio_iovcnt
#define	osi_uio_offset		uio_offset
#define	osi_uio_fmode		uio_fmode
#define	osi_uio_resid		uio_resid
#define	osi_uio_seg		uio_segflg
#define	OSI_UIOSYS		UIO_SYSSPACE
#define	OSI_UIOUSER		UIO_USERSPACE
#define osi_InitUIO(up)		((up)->uio_fmode = 0)

typedef short osi_uio_seg_t;

#define osi_uio_set_offset(u, o) \
  (AFS_hfitsinu32(o) ? ((u).osi_uio_offset = AFS_hgetlo(o)) , 0 : !0)

#define osi_uio_get_offset(u, o) (AFS_hset64((o), 0, (u).osi_uio_offset))

#ifdef KERNEL
#define osi_uiomove(cp, n, rw, uio) uiomove (cp, n, rw, uio)
#else /*  ! KERNEL */
#define osi_uiomove(cp, n, rw, uio) osi_user_uiomove (cp, n, rw, uio)
#endif /* KERNEL */


#endif /* TRANSARC_OSI_UIO_MACH_H */
