/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_dhops.c,v $
 * Revision 1.1.75.1  1996/10/02  17:43:46  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:46  damon]
 *
 * $EndLog$
*/
/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */

/*			    Episode File System
			Directory Handle Operations
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/dir_buffer.h>		/* struct dh_ops */

#include <dcedfs/episode/async.h>
#include <dcedfs/episode/fid.h>		/* epi_volFileId_t */
#include <dcedfs/episode/logbuf.h>		/* logTranRec_t */
#include <dcedfs/episode/anode.h>		/* epi_anode_t */
#include <dcedfs/episode/volume.h>		/* epi_volume_t */

#include <efs_dirhandle.h>		/* struct epi_Dir_Handle */
#include <efs_evnode.h>			/* struct evnode */
#include <efs_misc.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_dhops.c,v 1.1.75.1 1996/10/02 17:43:46 damon Exp $")

/* Forward declarations */

static int edh_Read(struct epi_Dir_Handle *, u_long, char *, u_long);
static int edh_Write(
    struct epi_Dir_Handle *, logTranRec_t, u_long, char *, u_long
);
static u_int32 edh_Length(struct epi_Dir_Handle *);
static void edh_Zap(struct epi_Dir_Handle *);
static int edh_Eq(struct epi_Dir_Handle *, struct epi_Dir_Handle *);
static void edh_Copy(struct epi_Dir_Handle *, struct epi_Dir_Handle *);
static int edh_FidHash(struct epi_Dir_Handle *, long);

#define DEBUG_THIS_FILE EFS_DEBUG_DHOPS

static struct dh_ops dho = {edh_Read, edh_Write, edh_Length,
		     edh_FidHash, edh_Eq, edh_Zap, edh_Copy};

/* edh_SetDirHandle -- Initialize an episode-style directory handle */

void edh_SetDirHandle(
  struct epi_Dir_Handle *edh,		/* directory handle */
  epi_anode_t ap,
  epi_volume_t volh,
  epi_volFileId_t *fid)
{
    edh->edh_ops = &dho;
    edh->edh_ap = ap;
    edh->edh_volp = volh;
    edh->edh_fid = *fid;
}

/*
 * edh_Read -- read data from a directory, using directory handle.
 */

static int edh_Read(
    struct epi_Dir_Handle *edh,		/* directory handle */
    u_long addr,			/* start addr to read from relative to
					 *   start of directory */
    char *data,				/* addr to read into */
    u_long len)				/* # bytes to read */
{
    struct uio uio;
    struct iovec iov;

    epia_InitUio (&uio, &iov, addr, len, data);
    return (epia_Read(edh->edh_ap, 0, &uio) == 0);
}

/*
 * edh_Write -- write data to a directory, using directory handle.
 */

static int edh_Write(
    struct epi_Dir_Handle *edh,		/* directory handle */
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
    code = epia_Write(tranId, edh->edh_ap, 0, &uio);
    if (code)
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
    return (code);
}

/*
 * edh_Length -- length of a directory in bytes, using directory handle.
 */

static u_int32 edh_Length(struct epi_Dir_Handle *edh)
{
    return AFS_hgetlo(epia_GetLength (edh->edh_ap));
}

/*
 * edh_FidHash -- return a hashable number from a directory handle.
 */

static int edh_FidHash(
    struct epi_Dir_Handle *edh,		/* directory handle */
    long page)				/* page number */
{
    return epia_GetIndex (edh->edh_ap) + page;
}

/*
 * edh_Eq -- compare directory handles for equality.
 */

static int edh_Eq(
    struct epi_Dir_Handle *edh1,	/* directory handle */
    struct epi_Dir_Handle *edh2)	/* directory handle */
{
    return (edh1->edh_volp == edh2->edh_volp
	    &&
	    epid_Equal (&edh1->edh_fid, &edh2->edh_fid));
}

/*
 * edh_Zap -- render directory handle unrecognizable.
 */

static void edh_Zap(struct epi_Dir_Handle *edh)
{
    edh->edh_volp = (epi_volume_t) 0;
}

/*
 * edh_Copy -- copy directory handle.
 */

static void edh_Copy(
    struct epi_Dir_Handle *edhto,	/* directory handle to copy to */
    struct epi_Dir_Handle *edhfrom)	/* directory handle to copy from */
{
    *edhto = *edhfrom;
}
