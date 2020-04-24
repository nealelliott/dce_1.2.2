/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * client_stub.c -- 
 *    Stub code for client side of cache conisistency tests.
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
 * $Log: client_stub.c,v $
 * Revision 1.1.8.1  1996/10/17  18:17:35  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:45  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:25:04  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:56:25  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  16:01:12  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:00:09  jaffe]
 * 
 * $EndLog$
 */

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>

#include "hub.h"

/* chmod call */

wlr_DoChmod(clp, pathp, mode)
  struct wlr_client *clp;
  char *pathp;
  long mode;
{
    struct wlr_call *cp;
    long code;

    code = wlr_ABegin(clp, &cp);
    if (code) return code;
    code = wlr_XDRChmodIP(cp, pathp, &mode, 1);
    if (code) goto done;
    code = wlr_ACall(cp, WLR_OP_CHMOD);
    if (code) goto done;
    /* no output parameters to unmarshall */
    code = wlr_AWaitCall(cp);

  done:
    return code;
}

/* stat call */

wlr_DoStat(clp, pathp, statp)
  struct wlr_client *clp;
  char *pathp;
  struct stat *statp;
{
    long code;
    struct wlr_call *callp;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRString(callp, pathp, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_STAT);
    if (code) goto done;
    code = wlr_AWaitCall(callp);
    if (code) goto done;
    code = wlr_XDRStat(callp, statp, 0);

  done:
    return code;
}

/* open stuff */

wlr_DoOpen(clp, pathp, flags, mode, fdp)
  struct wlr_client *clp;
  char *pathp;
  long flags;
  long mode;
  long *fdp;
{
    long code;
    struct wlr_call *callp;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDROpenIP(callp, pathp, &flags, &mode, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_OPEN);
    if (code) goto done;
    code = wlr_AWaitCall(callp);
    if (code) goto done;
    code = wlr_XDRLong(callp, fdp, 0);

  done:
    return code;
}

/* read stuff */

wlr_DoRead(clp, fd, offset, len, blockp, lenp)
  struct wlr_client *clp;
  long fd, offset, len;
  char *blockp;
  long *lenp;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRReadIP(callp, &fd, &offset, &len, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_READ);
    if (code) goto done;
    code = wlr_AWaitCall(callp);
    if (code) goto done;
    code = wlr_XDRReadOP(callp, blockp, lenp, 0);

  done:
    return code;
}

/* write stuff */

wlr_DoWrite(clp, fd, offset, len, blockp, lenp)
  struct wlr_client *clp;
  long fd, offset, len;
  char *blockp;
  long *lenp;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRWriteIP(callp, &fd, &offset, &len, blockp, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_WRITE);
    if (code) goto done;
    code = wlr_AWaitCall(callp);
    if (code) goto done;
    code = wlr_XDRLong(callp, lenp, 0);

  done:
    return code;
}

/* ftrunc stuff */

wlr_DoFtrunc(clp, fd, len)
  struct wlr_client *clp;
  long fd, len;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRFtruncIP(callp, &fd, &len, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_FTRUNC);
    if (code) goto done;
    code = wlr_AWaitCall(callp);

  done:
    return code;
}

/* fstat stuff */

wlr_DoFstat(clp, fd, statp)
  struct wlr_client *clp;
  long fd;
  struct stat *statp;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRLong(callp, &fd, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_FSTAT);
    if (code) goto done;
    code = wlr_AWaitCall(callp);
    if (code) goto done;
    code = wlr_XDRStat(callp, statp, 0);

  done:
    return code;
}

/* close stuff */

wlr_DoClose(clp, fd)
  struct wlr_client *clp;
  long fd;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRLong(callp, &fd, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_CLOSE);
    if (code) goto done;
    code = wlr_AWaitCall(callp);

  done:
    return code;
}

/* trunc stuff */

wlr_DoTrunc(clp, path, len)
  struct wlr_client *clp;
  char *path;
  long len;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRTruncIP(callp, path, &len, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_TRUNC);
    if (code) goto done;
    code = wlr_AWaitCall(callp);

  done:
    return code;
}

/* rename stuff */

wlr_DoRename(clp, srcPath, targetPath)
  struct wlr_client *clp;
  char *srcPath, *targetPath;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRRenameIP(callp, srcPath, targetPath, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_RENAME);
    if (code) goto done;
    code = wlr_AWaitCall(callp);

  done:
    return code;
}

/* unlink stuff */

wlr_DoUnlink(clp, path)
  struct wlr_client *clp;
  char *path;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRString(callp, path, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_UNLINK);
    if (code) goto done;
    code = wlr_AWaitCall(callp);

  done:
    return code;
}

/* link stuff */

wlr_DoLink(clp, srcPath, targetPath)
  struct wlr_client *clp;
  char *srcPath, *targetPath;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRLinkIP(callp, srcPath, targetPath, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_LINK);
    if (code) goto done;
    code = wlr_AWaitCall(callp);

  done:
    return code;
}

/* symlink stuff */

wlr_DoSymlink(clp, srcPath, targetPath)
  struct wlr_client *clp;
  char *srcPath, *targetPath;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRSymlinkIP(callp, srcPath, targetPath, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_SYMLINK);
    if (code) goto done;
    code = wlr_AWaitCall(callp);

  done:
    return code;
}

/* makedir stuff */

wlr_DoMakedir(clp, pathp, mode)
  struct wlr_client *clp;
  char *pathp;
  long mode;
{
    long code;
    struct wlr_call *callp;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRMakedirIP(callp, pathp, &mode, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_MKDIR);
    if (code) goto done;
    code = wlr_AWaitCall(callp);

  done:
    return code;
}

/* rmdir stuff */

wlr_DoRmdir(clp, path)
  struct wlr_client *clp;
  char *path;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRString(callp, path, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_RMDIR);
    if (code) goto done;
    code = wlr_AWaitCall(callp);

  done:
    return code;
}

/* utimes stuff */

wlr_DoUtimes(clp, pathp, mtimeSec, mtimeUsec, atimeSec, atimeUsec)
  struct wlr_client *clp;
  char *pathp;
  long mtimeSec, mtimeUsec;
  long atimeSec, atimeUsec;
{
    long code;
    struct wlr_call *callp;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRUtimesIP(callp, pathp, &mtimeSec, &mtimeUsec,
			 &atimeSec, &atimeUsec, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_UTIMES);
    if (code) goto done;
    code = wlr_AWaitCall(callp);

  done:
    return code;
}

/* readdir stuff */

wlr_DoReaddir(clp, fd, offset, len, blockp, lenp)
  struct wlr_client *clp;
  long fd, offset, len;
  char *blockp;
  long *lenp;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRReaddirIP(callp, &fd, &offset, &len, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_READDIR);
    if (code) goto done;
    code = wlr_AWaitCall(callp);
    if (code) goto done;
    code = wlr_XDRReaddirOP(callp, blockp, lenp, 0);

  done:
    return code;
}

/* readlink stuff */

wlr_DoReadlink(clp, pathp, resultp, lenp)
  struct wlr_client *clp;
  char *pathp;
  char *resultp;
  long *lenp;
{
    struct wlr_call *callp;
    long code;

    code = wlr_ABegin(clp, &callp);
    if (code) return code;
    code = wlr_XDRString(callp, pathp, 1);
    if (code) goto done;
    code = wlr_ACall(callp, WLR_OP_READLINK);
    if (code) goto done;
    code = wlr_AWaitCall(callp);
    if (code) goto done;
    code = wlr_XDRReadlinkOP(callp, resultp, lenp, 0);

  done:
    return code;
}
