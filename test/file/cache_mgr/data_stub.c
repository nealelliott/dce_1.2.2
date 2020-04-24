/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * data_stub.c -- 
 *    Routines for moving data between hub and spoke in cache
 *    consistency tests.
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: data_stub.c,v $
 * Revision 1.1.421.1  1996/10/17  18:17:47  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:47  damon]
 *
 * Revision 1.1.416.1  1994/02/04  20:40:02  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:22:19  devsrc]
 * 
 * Revision 1.1.414.1  1993/12/07  17:41:30  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:40:39  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/11  18:25:12  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:56:39  htf]
 * 
 * Revision 1.1.2.3  1992/11/04  19:21:42  jaffe
 * 	Transarc delta: fred-cc-fix-socket-port 1.3
 * 	  Selected comments:
 * 	    Additional fixes were necessary.  A couple calls to htons were added and
 * 	    also normalized the optn() flags.  There values are different on AIX and
 * 	    OSF/1.
 * 	    Included openFlags.h.
 * 	    Forgot to include new .h file.
 * 	[1992/11/04  17:34:09  jaffe]
 * 
 * Revision 1.1.2.2  1992/10/28  16:01:41  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:06:18  jaffe]
 * 
 * $EndLog$
 */

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>
#include <dcedfs/osi.h>

#include "hub.h"

/* basic xdr stuff */

wlr_DirToHostOrder(bufferp, inLen)
  struct wlr_netdirent *bufferp;
  long inLen;
{
    long irl;

    while (inLen > 0) {
	irl = ntohs(bufferp->recLength);
	bufferp->recLength = irl;
	bufferp->inode = ntohl(bufferp->inode);
	bufferp->offset = ntohl(bufferp->offset);
	bufferp->nameLength = ntohs(bufferp->nameLength);
	/* add one for the null at the end of the string */
	inLen -= irl;
	/* now move pointers by the right amount */
	bufferp = (struct wlr_netdirent *) (((long)bufferp) + irl);
    }
    return 0;
}

wlr_XDRStat(callp, statp, marshall)
  struct wlr_call *callp;
  struct stat *statp;
  int marshall;
{
    long code;
    short tshort;

    /* handle case where st_mode isn't a short, too */
    if (marshall) tshort = statp->st_mode;
    code = wlr_XDRShort(callp, &tshort, marshall);
    if (code) return code;
    if (!marshall) statp->st_mode = tshort;

    code = wlr_XDRLong(callp, &statp->st_size, marshall);
    if (code) return code;
    code = wlr_XDRShort(callp, (short *) &statp->st_nlink, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, &statp->st_mtime, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, &statp->st_atime, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, &statp->st_ctime, marshall);
    return code;
}

wlr_XDRBlock(callp, blockp, marshall)
  struct wlr_call *callp;
  char *blockp;
  int marshall;
{
    if (marshall)
	bcopy(blockp, callp->mp, WLR_BLOCKSIZE);
    else
	bcopy(callp->mp, blockp, WLR_BLOCKSIZE);
    callp->mp += WLR_BLOCKSIZE;
    return 0;
}

wlr_XDRString(callp, stringp, marshall)
  struct wlr_call *callp;
  char *stringp;
  int marshall;
{
    int tlen;
    if (marshall) {
	tlen = strlen(stringp)+1;
	if (tlen > WLR_STRINGSIZE) return -2;
	bcopy(stringp, callp->mp, tlen);
    }
    else {
	/* unmarshall */
	tlen = strlen(callp->mp)+1;
	if (tlen > WLR_STRINGSIZE) return -2;
	bcopy(callp->mp, stringp, tlen);
    }
    callp->mp += tlen;
    return 0;
}

wlr_XDRLong(callp, longp, marshall)
  struct wlr_call *callp;
  long *longp;
  int marshall;
{
    /* no doubt this is architecture dependent */
    long value;
    value = 0;

    if (marshall) {
	value = htonl(*longp);
	bcopy(&value, callp->mp, sizeof(value));
    }
    else {
	/* unmarshall */
	bcopy(callp->mp, &value, sizeof(value));
	*longp = ntohl(value);
    }
    callp->mp += 4;
    return 0;
}

wlr_XDRShort(callp, intp, marshall)
  struct wlr_call *callp;
  short *intp;
  int marshall;
{
    /* no doubt this is architecture dependent */
    short value;

    value = 0;
    if (marshall) {
	value = htons(*intp);
	bcopy(&value, callp->mp, sizeof(value));
    }
    else {
	/* unmarshall */
	bcopy(callp->mp, &value, sizeof(value));
	*intp = ntohs(value);
    }
    callp->mp += 2;
    return 0;
}

/* chmod call */

wlr_XDRChmodIP(callp, pathp, modep, marshall)
  struct wlr_call *callp;
  char *pathp;
  long *modep;
  int marshall;
{
    long code;
    code = wlr_XDRString(callp, pathp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, modep, marshall);
    return code;
}

/* open stuff */
wlr_XDROpenIP(callp, pathp, flagsp, modep, marshall)
  struct wlr_call *callp;
  char *pathp;
  long *flagsp;
  long *modep;
  int marshall;
{
    long code;
    code = wlr_XDRString(callp, pathp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, flagsp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, modep, marshall);
    return code;
}

/* read stuff */

wlr_XDRReadIP(callp, ifdp, offsetp, lenp, marshall)
  struct wlr_call *callp;
  long *ifdp;
  long *offsetp;
  long *lenp;
  int marshall;
{
    long code;

    code = wlr_XDRLong(callp, ifdp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, offsetp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, lenp, marshall);
    return code;
}

wlr_XDRReadOP(callp, blockp, lenp, marshall)
  struct wlr_call *callp;
  long *lenp;
  char *blockp;
  int marshall;
{
    long code;

    code = wlr_XDRBlock(callp, blockp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, lenp, marshall);
    return code;
}

/* write stuff */

wlr_XDRWriteIP(callp, ifdp, offsetp, lenp, blockp, marshall)
  struct wlr_call *callp;
  long *ifdp;
  long *offsetp;
  long *lenp;
  char *blockp;
  int marshall;
{
    long code;

    code = wlr_XDRLong(callp, ifdp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, offsetp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, lenp, marshall);
    if (code) return code;
    code = wlr_XDRBlock(callp, blockp, marshall);
    return code;
}

/* ftrunc stuff */

wlr_XDRFtruncIP(callp, fdp, lenp, marshall)
  struct wlr_call *callp;
  long *fdp;
  long *lenp;
  int marshall;
{
    long code;

    code = wlr_XDRLong(callp, fdp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, lenp, marshall);
    return code;
}

/* trunc stuff */

wlr_XDRTruncIP(callp, namep, lenp, marshall)
  struct wlr_call *callp;
  char *namep;
  long *lenp;
  int marshall;
{
    long code;
    char path[WLR_STRINGSIZE];
    long size;

    code = wlr_XDRString(callp, path, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, &size, marshall);
    return code;
}

/* rename stuff */

wlr_XDRRenameIP(callp, srcPath, targetPath, marshall)
  struct wlr_call *callp;
  char *srcPath, *targetPath;
  int marshall;
{
    long code;

    code = wlr_XDRString(callp, srcPath, marshall);
    if (code) return code;
    code = wlr_XDRString(callp, targetPath, marshall);
    return code;
}

/* link stuff */

wlr_XDRLinkIP(callp, srcPath, targetPath, marshall)
  struct wlr_call *callp;
  char *srcPath, *targetPath;
  int marshall;
{
    long code;

    code = wlr_XDRString(callp, srcPath, marshall);
    if (code) return code;
    code = wlr_XDRString(callp, targetPath, marshall);
    return code;
}

/* symlink stuff */

wlr_XDRSymlinkIP(callp, srcPath, targetPath, marshall)
  struct wlr_call *callp;
  char *srcPath, *targetPath;
  int marshall;
{
    long code;

    code = wlr_XDRString(callp, srcPath, marshall);
    if (code) return code;
    code = wlr_XDRString(callp, targetPath, marshall);
    return code;
}

/* makedir stuff */

wlr_XDRMakedirIP(callp, pathp, modep, marshall)
  struct wlr_call *callp;
  char *pathp;
  long *modep;
  int marshall;
{
    long code;
    code = wlr_XDRString(callp, pathp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, modep, marshall);
    return code;
}

/* utimes stuff */

wlr_XDRUtimesIP(callp, pathp, mtimeSec, mtimeUsec, atimeSec, atimeUsec, marshall)
  struct wlr_call *callp;
  char *pathp;
  long *mtimeSec, *mtimeUsec, *atimeSec, *atimeUsec;
  int marshall;
{
    long code;
    code = wlr_XDRString(callp, pathp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, mtimeSec, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, mtimeUsec, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, atimeSec, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, atimeUsec, marshall);
    return code;
}

/* readdir stuff */

wlr_XDRReaddirIP(callp, ifdp, offsetp, lenp, marshall)
  struct wlr_call *callp;
  long *ifdp;
  long *offsetp;
  long *lenp;
  int marshall;
{
    long code;

    code = wlr_XDRLong(callp, ifdp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, offsetp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, lenp, marshall);
    return code;
}

wlr_XDRReaddirOP(callp, blockp, lenp, marshall)
  struct wlr_call *callp;
  long *lenp;
  char *blockp;
  int marshall;
{
    long code;

    code = wlr_XDRBlock(callp, blockp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, lenp, marshall);
    if (code) return code;
    /* convert dir listing, too, when unmarshalling; marshalling cvt is done
     * by data sender.
     */
    if (!marshall)
	code = wlr_DirToHostOrder((struct wlr_netdirent *) blockp, *lenp);
    else code = 0;
    return code;
}

/* readlink stuff */

wlr_XDRReadlinkOP(callp, stringp, lenp, marshall)
  struct wlr_call *callp;
  char *stringp;
  long *lenp;
  int marshall;
{
    long code;

    code = wlr_XDRString(callp, stringp, marshall);
    if (code) return code;
    code = wlr_XDRLong(callp, lenp, marshall);
    return code;
}
