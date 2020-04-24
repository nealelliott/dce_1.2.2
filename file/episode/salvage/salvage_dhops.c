/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/debug.h>
#include <dcedfs/osi.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/dir_buffer.h>		/* struct dh_ops */
#include <dcedfs/dir.h>

#include <dcedfs/episode/logbuf.h>	/* logTranRec_t */
#include <dcedfs/episode/anode.h>	/* epi_anode_t */

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/salvage/RCS/salvage_dhops.c,v 4.24 1996/03/04 19:02:36 ota Exp $")

/*
** These ops look much like the *normal* dhops.
** They are used in a radically different way.
** They are the target of the directory salvage routine
** dirs_Salvage.
** Instead of trying to find some empty anode to stuff
** the directory contents with, we use the write primitive
** to RECORD the contents of the new directory.  After
** the salvage is completed, we rewrite the contents of
** the *written* directory into some anode which we
** can later open...
** For the sake of the explanation, the anode target after
** the dirs_Salvage is done, is the same source anode.
** The objective is to salvage in place...
**
*/

struct NoDisk {
    struct NoDisk	*next;
    u_long		pageNo;
    char		*pageData;	/* DISK_PAGE */
};
#define	DISK_PAGE	DIRPAGESIZE

#define	lnoDiskHead(dh)	(dh->dh_data[1])
#define	noDiskHead(dh)	((struct NoDisk *)(dh->dh_data[1]))

#define lnoDiskLength(dh) (dh->dh_data[2])
#define noDiskLength(dh) (dh->dh_data[2])

/* Forward declarations */

static int	rdh_Read(struct Dir_Handle *, u_long, char *, u_long);
static int	rdh_Write(
    struct Dir_Handle *, logTranRec_t, u_long, char *, u_long);
static u_int32	rdh_Length(struct Dir_Handle *);
static void	rdh_Zap(struct Dir_Handle *);
static int	rdh_Eq(struct Dir_Handle *, struct Dir_Handle *);
static void	rdh_Copy(struct Dir_Handle *, struct Dir_Handle *);
static int	rdh_FidHash(struct Dir_Handle *);

static struct dh_ops epis_dho = {rdh_Read, rdh_Write, rdh_Length,
		     rdh_FidHash, rdh_Eq, rdh_Zap, rdh_Copy};

#define	ldh_data_to_h(dh)	(dh->dh_data[0])
#define	dh_data_to_h(dh)	(epi_anode_t)(dh->dh_data[0])
#define	h_to_dh_data(dh)	(int32)(h)

/*
 * findBlock -- Finds a block on the list.  Simple return arguments.
 */

static struct NoDisk *
findBlock(struct NoDisk *list, u_long pageNo)
{
    struct NoDisk *ndP;
    for (ndP = list; ndP; ndP = ndP->next) {
	if (ndP->pageNo == pageNo)
		return(ndP);
    }
    return(0);
}

/*
 * SetSalvageDirHandle -- Initialize a directory handle
 */
/* SHARED */
void SetSalvageDirHandle(
    epi_anode_t *h,			/* anode */
    struct Dir_Handle * dh)		/* directory handle */
{
    dh->dh_ops = &epis_dho;
    ldh_data_to_h(dh) = h_to_dh_data(h);
    lnoDiskHead(dh) = 0;
    lnoDiskLength(dh) = 0;
}

/*
 * PutSalvageDirHandle -- Put a directory handle.  In this case, we flush the
 *     contents of the 'No Disk' blocks back to the anode we mentioned.
 *
 * RETURN CODES -- we return errors from epia_Write directly.
 */
/* SHARED */
long PutSalvageDirHandle(struct Dir_Handle *dh)
{
    int code = 0;			/* error return code */
    struct NoDisk * ndP;
    u_long length;
    u_long pgno;
    u_long pgcount;
    int didTruncate = 0;
    afs_hyper_t l;

    dirb_Annul( dh );

    length = noDiskLength (dh);
    pgcount = (length + (DISK_PAGE-1)) >> LOGDIRPAGESIZE;
    for (pgno=0; pgno<pgcount; pgno++) {
	struct uio uio;
	struct iovec iov;
	struct NoDisk * ndP = findBlock (noDiskHead(dh), pgno);
	long writelen = MIN(length,DISK_PAGE);
	long offset =  pgno * DISK_PAGE;

	if (!ndP) {

	    /* Skip over null pages but make sure we call truncate at least
             * once to blast existing blocks */

	    if (!didTruncate) {
		AFS_hset64 (l, 0, offset);
		code = epia_Truncate (buffer_nullTranId,
				      dh_data_to_h(dh), 0, l);
		if (code) return (code);
		didTruncate = 1;
	    }
	    continue;
	}

	epia_InitUio (&uio, &iov, offset, writelen, ndP->pageData);
	code = epia_Write(buffer_nullTranId, dh_data_to_h(dh), 0, &uio);
	if (code)
	    return code;
    }

    if (!didTruncate) {

	/* now set length down if necessary */

	AFS_hset64 (l, 0, length);
	code = epia_Truncate (buffer_nullTranId, dh_data_to_h(dh), 0, l);
	if (code) return (code);
    }
    return 0;
}

/*
 * rdh_Read -- read data from a directory, using directory handle.
 *
 * RETURN CODES -- boolean indicating success. */

static int rdh_Read(
    struct Dir_Handle *dh,		/* directory handle */
    u_long addr,			/* addr to read from relative to
					 *   start of directory */
    char *data,				/* addr to read into */
    u_long len)				/* # bytes to read */
{
    struct NoDisk *ndP;

    if (addr+len > rdh_Length(dh))
	return 0;			/* beyond EOF */

    bzero( data, len );	/* in case we don't read any data */

    while ( len ) {
	u_long copyLen;
	u_long copyOffset;

	ndP = findBlock( noDiskHead(dh), addr / DISK_PAGE );
	if (ndP == 0) {
	    /* skip this possible disk reference. */
	    copyOffset = (addr % DISK_PAGE);
	    copyLen = len;
	    if ((addr + len) > DISK_PAGE)
		copyLen = DISK_PAGE - copyOffset;
	} else {
	    copyOffset = (addr % DISK_PAGE);
	    copyLen = len;
	    if ((addr + len) > DISK_PAGE)
		copyLen = DISK_PAGE - copyOffset;

	    (void) bcopy( &(ndP->pageData[copyOffset]), data, copyLen );
	}
	len -= copyLen;
	addr += copyLen;
	data += copyLen;
    }
    return(1);				/* success */
}

/*
 * rdh_Write -- write data to a directory, using directory handle.
 */

static int rdh_Write(
    struct Dir_Handle *dh,		/* directory handle */
    logTranRec_t tranId,		/* transaction ID */
    u_long addr,			/* address to write to relative to
					 *   start of directory */
    char *data,				/* address to write from */
    u_long len)				/* # bytes to write */
{
    int code = 0;
    struct NoDisk *ndP;

    if ((addr+len) > noDiskLength(dh))
	lnoDiskLength(dh) = addr+len;

    while ( len ) {
	ndP = findBlock( noDiskHead(dh), addr / DISK_PAGE );
	if (ndP == 0) {
	    /* make a new one, link it in. */
	    char * newData;
	    ndP = (struct NoDisk *)osi_Alloc( sizeof(*ndP) );
	    newData = (char *) osi_Alloc( DISK_PAGE );
	    if ((newData == 0) || (ndP == 0)) {
		return EIO; /* XXX need more trouble here. */
	    }
	    bzero( newData, DISK_PAGE );
	    ndP->pageData = newData;
	    ndP->pageNo = addr / DISK_PAGE;
	    ndP->next = noDiskHead(dh);
	    lnoDiskHead(dh) = (int32)ndP;
	}

	/* BEGIN */ {
	    u_long copyLen;
	    u_long copyOffset;

	    copyOffset = (addr % DISK_PAGE);
	    copyLen = MIN (len, DISK_PAGE - copyOffset);

	    (void) bcopy( data, &ndP->pageData[copyOffset], copyLen );
	    len -= copyLen;
	    addr += copyLen;
	    data += copyLen;
	}
    }
    return(code);
}

/*
 * rdh_Length -- length of a directory in bytes, using directory handle.
 *
 * RETURN CODES -- since there is no provision for returning errors we return a
 *     length of zero if anything goes wrong.
 */

static u_int32 rdh_Length(struct Dir_Handle *dh)
{
    struct epia_info infbuf;		/* place to write info about anode */
    u_int32 dirLen;
    long code;

    code = epia_GetInfo(dh_data_to_h(dh), &infbuf);
    if (code) return 0;
    if (!AFS_hfitsinu32(infbuf.length)) return 0;
    dirLen = AFS_hgetlo(infbuf.length);
    if (dirLen < noDiskLength(dh))
	dirLen = noDiskLength(dh);
    return (dirLen);
}
/*
 * rdh_FidHash -- return a hashable number from a directory handle.
 */

static int rdh_FidHash(struct Dir_Handle *dh)
{
    return (int) dh_data_to_h(dh);
}
/*
 * rdh_Eq -- compare directory handles for equality.
 */

static int rdh_Eq(struct Dir_Handle *dh1, struct Dir_Handle *dh2)
{
    return (dh_data_to_h(dh1) == dh_data_to_h(dh2));
}
/*
 * rdh_Zap -- render directory handle unrecognizable.
 */

static void rdh_Zap(struct Dir_Handle *dh)
{
    ldh_data_to_h(dh) = (int32) 0;
}
/*
 * rdh_Copy -- copy directory handle.
 */

static void rdh_Copy(struct Dir_Handle *dhto, struct Dir_Handle *dhfrom)
{
    *dhto = *dhfrom;
}
