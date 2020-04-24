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
 * Revision 1.1.385.1  1996/10/02  17:58:45  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:10  damon]
 *
 * $EndLog$
 */
/*
 *      Copyright (C) 1996, 1990 Transarc Corporation
 *      All rights reserved.
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/osi/OSF1/osi_uio_mach.h,v 1.1.385.1 1996/10/02 17:58:45 damon Exp $ */

#ifndef TRANSARC_OSI_UIO_MACH_H
#define	TRANSARC_OSI_UIO_MACH_H
/* 
 * This file contains standard macros that are used to isolate OS
 * system dependencies for uio servides
 */

#include <sys/uio.h>

#ifndef KERNEL
enum	uio_rw { UIO_READ, UIO_WRITE };

/*
 * Segment flag values.
 */
enum	uio_seg {
	UIO_USERSPACE,		/* from user data space */
	UIO_SYSSPACE,		/* from system space */
	UIO_USERISPACE		/* from user I space */
};

struct uio {
	struct	iovec *uio_iov;
	int	uio_iovcnt;
	off_t	uio_offset;
	int	uio_resid;
	enum	uio_seg uio_segflg;
};
#endif /* !KERNEL */

/*
 * uio vector related definitions
 */
#define	osi_uio_iov		uio_iov
#define	osi_uio_iovcnt		uio_iovcnt
#define	osi_uio_offset		uio_offset
#define	osi_uio_resid		uio_resid
#define	osi_uio_seg		uio_segflg
#define	OSI_UIOSYS		UIO_SYSSPACE
#define	OSI_UIOUSER		UIO_USERSPACE
#define osi_InitUIO(up)		/* nothing special */

typedef enum uio_seg osi_uio_seg_t;

#define osi_uio_set_offset(u, o) \
  (AFS_hfitsinu32(o) ? ((u).osi_uio_offset = AFS_hgetlo(o)) , 0 : !0)

#define osi_uio_get_offset(u, o) AFS_hset64((o), 0, (u).osi_uio_offset)

#ifdef KERNEL
#define osi_uiomove(cp, n, rw, uio) \
	((uio)->uio_rw = (rw), uiomove(cp, n, uio))
#else /*  ! KERNEL */
#define osi_uiomove(cp, n, rw, uio) osi_user_uiomove (cp, n, rw, uio)
#endif /* KERNEL */
#endif /* TRANSARC_OSI_UIO_MACH_H */

