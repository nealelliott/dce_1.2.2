/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dhops.c,v $
 * Revision 1.1.62.1  1996/10/02  17:27:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:33:54  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/dir_buffer.h>		/* struct dh_ops */

#include <dcedfs/episode/logbuf.h>		/* logTranRec_t */
#include <dcedfs/episode/anode.h>		/* epi_anode_t */

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/salvage/dhops.c,v 1.1.62.1 1996/10/02 17:27:18 damon Exp $")

/* Forward declarations */

static int	sdh_Read(struct Dir_Handle *, u_long, char *, u_long);
static int	sdh_Write(
    struct Dir_Handle *, logTranRec_t, u_long, char *, u_long);
static u_int32	sdh_Length(struct Dir_Handle *);
static void	sdh_Zap(struct Dir_Handle *);
static int	sdh_Eq(struct Dir_Handle *, struct Dir_Handle *);
static void	sdh_Copy(struct Dir_Handle *, struct Dir_Handle *);
static int	sdh_FidHash(struct Dir_Handle *);

static struct dh_ops epis_dho = {
    sdh_Read, sdh_Write, sdh_Length, sdh_FidHash, sdh_Eq, sdh_Zap, sdh_Copy};

#define GetH(dh)	((epi_anode_t)((dh)->dh_data[0]))
#define SetH(dh, h)	((dh)->dh_data[0] = (int32)(h))

/*
 * SetDirHandle -- Initialize a directory handle
 */

/* SHARED */
void SetDirHandle(
  IN epi_anode_t h,			/* anode */
  OUT struct Dir_Handle * dh)		/* directory handle */
{
    dh->dh_ops = &epis_dho;
    SetH(dh, h);
}

/*
 * PutDirHandle -- All this currently does is dir_Annul the entry
 */
/* SHARED */
void PutDirHandle(IN struct Dir_Handle * dh)
{
    dirb_Annul( dh );
}

/*
 * sdh_Read -- read data from a directory, using directory handle.
 */

static int sdh_Read(
    struct Dir_Handle *dh,		/* directory handle */
    u_long addr,			/* addr to read from relative to
					 *   start of directory */
    char *data,				/* addr to read into */
    u_long len)				/* # bytes to read */
{
    struct uio uio;
    struct iovec iov;

    epia_InitUio (&uio, &iov, addr, len, data);
    return (epia_Read(GetH(dh), 0, &uio) == 0);
}
/*
 * sdh_Write -- write data to a directory, using directory handle.
 */

static int sdh_Write(
    struct Dir_Handle *dh,		/* directory handle */
    logTranRec_t tranId,		/* transaction ID */
    u_long addr,			/* address to write to relative to
					 *   start of directory */
    char *data,				/* address to write from */
    u_long len)				/* # bytes to write */
{
    int code;				/* error return code */
    struct uio uio;
    struct iovec iov;

    epia_InitUio (&uio, &iov, addr, len, data);
    code = epia_Write(tranId, GetH(dh), 0, &uio);
    if (code)
	switch (code) {
	    case EPI_E_VOLUMEFULL:
#ifdef	AFS_SUNOS5_ENV
		code = ENOSPC;
#else
		code = EDQUOT;
#endif	/* AFS_SUNOS5_ENV */
		break;
	    case EPI_E_AGGREGATEFULL:
		code = ENOSPC;
		break;
	    default:
		code = EIO;
		break;
	}
    return (code);
}
/*
 * sdh_Length -- length of a directory in bytes, using directory handle.
 *
 * RETURN CODES -- since there is no provision for returning errors we return a
 *     length of zero if anything goes wrong.
 */

static u_int32 sdh_Length(struct Dir_Handle *dh)
{
    struct epia_info infbuf;		/* place to write info about anode */
    long code;

    code = epia_GetInfo(GetH(dh), &infbuf);
    if (code) return 0;
    if (!AFS_hfitsinu32(infbuf.length)) return 0;
    return (AFS_hgetlo(infbuf.length));
}
/*
 * sdh_FidHash -- return a hashable number from a directory handle.
 */

static int sdh_FidHash (struct Dir_Handle *dh)
{
    return (int) GetH(dh);
}
/*
 * sdh_Eq -- compare directory handles for equality.
 */

static int sdh_Eq(struct Dir_Handle *dh1, struct Dir_Handle *dh2)
{
    return (GetH(dh1) == GetH(dh2));
}
/*
 * sdh_Zap -- render directory handle unrecognizable.
 */

static void sdh_Zap(struct Dir_Handle *dh)
{
    SetH(dh, 0);
}
/*
 * sdh_Copy -- copy directory handle.
 */

static void sdh_Copy(struct Dir_Handle *dhto, struct Dir_Handle *dhfrom)
{
    *dhto = *dhfrom;
}
