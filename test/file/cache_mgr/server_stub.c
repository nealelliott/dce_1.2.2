/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * server_stub.c -- 
 *    Stub routines for server (hub) side of cache consistency tests.
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
 * $Log: server_stub.c,v $
 * Revision 1.1.8.1  1996/10/17  18:18:06  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:56  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:25:29  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:57:30  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  16:10:53  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:55:05  jaffe]
 * 
 * $EndLog$
 */

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>

#include "hub.h"

wlr_StubChmod(callp)
  struct wlr_call *callp;
{
    char tbuffer[WLR_STRINGSIZE];
    long mode;
    long code;

    code = wlr_XDRChmodIP(callp, tbuffer, &mode, 0);
    if (code) return code;
    code = wlr_ServerChmod(callp, tbuffer, mode);
    wlr_AfterServerCall(callp, code);
    return 0;
}

wlr_StubStat(callp)
  struct wlr_call *callp;
{
    char tbuffer[WLR_STRINGSIZE];
    struct stat tstat;
    long code;

    code = wlr_XDRString(callp, tbuffer, 0);
    if (code) return code;
    code = wlr_ServerStat(callp, tbuffer, &tstat);
    wlr_AfterServerCall(callp, code);
    code = wlr_XDRStat(callp, &tstat, 1);
    return code;
}

wlr_StubOpen(callp)
  struct wlr_call *callp;
{
    long code;
    char tbuffer[WLR_STRINGSIZE];
    long flags;
    long mode;
    long fdp;

    code = wlr_XDROpenIP(callp, tbuffer, &flags, &mode, 0);
    if (code) return code;
    code = wlr_ServerOpen(callp, tbuffer, flags, mode, &fdp);
    wlr_AfterServerCall(callp, code);
    code = wlr_XDRLong(callp, &fdp, 1);
    return code;
}

wlr_StubRead(callp)
  struct wlr_call *callp;
{
    long fd, offset, len, len2;
    char block[WLR_BLOCKSIZE];
    long code;

    code = wlr_XDRReadIP(callp, &fd, &offset, &len, 0);
    if (code) return code;
    code = wlr_ServerRead(callp, fd, offset, len, block, &len2);
    wlr_AfterServerCall(callp, code);
    code = wlr_XDRReadOP(callp, block, &len2, 1);
    return code;
}

wlr_StubWrite(callp)
  struct wlr_call *callp;
{
    long fd, offset, len, len2;
    char block[WLR_BLOCKSIZE];
    long code;

    code = wlr_XDRWriteIP(callp, &fd, &offset, &len, block, 0);
    if (code) return code;
    code = wlr_ServerWrite(callp, fd, offset, len, block, &len2);
    wlr_AfterServerCall(callp, code);
    code = wlr_XDRLong(callp, &len2, 1);
    return code;
}

wlr_StubFtrunc(callp)
  struct wlr_call *callp;
{
    long fd, len;
    long code;

    code = wlr_XDRFtruncIP(callp, &fd, &len, 0);
    if (code) return code;
    code = wlr_ServerFtrunc(callp, fd, len);
    wlr_AfterServerCall(callp, code);
    return 0;
}

wlr_StubFstat(callp)
  struct wlr_call *callp;
{
    long fd;
    struct stat tstat;
    long code;

    code = wlr_XDRLong(callp, &fd, 1);
    if (code) return code;
    code = wlr_ServerFstat(callp, fd, &tstat);
    wlr_AfterServerCall(callp, code);
    code = wlr_XDRStat(callp, &tstat, 0);
  done:
    return code;
}

wlr_StubClose(callp)
  struct wlr_call *callp;
{
    long fd;
    long code;

    code = wlr_XDRLong(callp, &fd, 0);
    if (code) return code;
    code = wlr_ServerClose(callp, fd);
    wlr_AfterServerCall(callp, code);
    return 0;
}

wlr_StubTrunc(callp)
  struct wlr_call *callp;
{
    long len;
    char path[WLR_STRINGSIZE];
    long code;

    code = wlr_XDRTruncIP(callp, path, &len, 0);
    if (code) return code;
    code = wlr_ServerTrunc(callp, path, len);
    wlr_AfterServerCall(callp, code);
    return 0;
}

wlr_StubRename(callp)
  struct wlr_call *callp;
{
    char srcPath[WLR_STRINGSIZE], targetPath[WLR_STRINGSIZE];
    long code;

    code = wlr_XDRRenameIP(callp, srcPath, targetPath, 0);
    if (code) return code;
    code = wlr_ServerRename(callp, srcPath, targetPath);
    wlr_AfterServerCall(callp, code);
    return 0;
}

wlr_StubUnlink(callp)
  struct wlr_call *callp;
{
    char path[WLR_STRINGSIZE];
    long code;

    code = wlr_XDRString(callp, path, 0);
    if (code) return code;
    code = wlr_ServerUnlink(callp, path);
    wlr_AfterServerCall(callp, code);
    return 0;
}

wlr_StubLink(callp)
  struct wlr_call *callp;
{
    char srcPath[WLR_STRINGSIZE], targetPath[WLR_STRINGSIZE];
    long code;

    code = wlr_XDRLinkIP(callp, srcPath, targetPath, 0);
    if (code) return code;
    code = wlr_ServerLink(callp, srcPath, targetPath);
    wlr_AfterServerCall(callp, code);
    return 0;
}

wlr_StubSymlink(callp)
  struct wlr_call *callp;
{
    char srcPath[WLR_STRINGSIZE], targetPath[WLR_STRINGSIZE];
    long code;

    code = wlr_XDRSymlinkIP(callp, srcPath, targetPath, 0);
    if (code) return code;
    code = wlr_ServerSymlink(callp, srcPath, targetPath);
    wlr_AfterServerCall(callp, code);
    return 0;
}

wlr_StubMkdir(callp)
  struct wlr_call *callp;
{
    long code;
    char tbuffer[WLR_STRINGSIZE];
    long mode;

    code = wlr_XDRMakedirIP(callp, tbuffer, &mode, 0);
    if (code) return code;
    code = wlr_ServerMakedir(callp, tbuffer, mode);
    wlr_AfterServerCall(callp, code);
    return 0;
}

wlr_StubRmdir(callp)
  struct wlr_call *callp;
{
    char path[WLR_STRINGSIZE];
    long code;

    code = wlr_XDRString(callp, path, 0);
    if (code) return code;
    code = wlr_ServerRmdir(callp, path);
    wlr_AfterServerCall(callp, code);
    return 0;
}

wlr_StubUtimes(callp)
  struct wlr_call *callp;
{
    long code;
    char tbuffer[WLR_STRINGSIZE];
    long mtimeSec, mtimeUsec;
    long atimeSec, atimeUsec;

    code = wlr_XDRUtimesIP(callp, tbuffer, &mtimeSec, &mtimeUsec,
			 &atimeSec, &atimeUsec, 0);
    if (code) return code;
    code = wlr_ServerUtimes(callp, tbuffer, mtimeSec, mtimeUsec,
			    atimeSec, atimeUsec);
    wlr_AfterServerCall(callp, code);
    return 0;
}

wlr_StubReaddir(callp)
  struct wlr_call *callp;
{
    long fd, offset, len, len2;
    char block[WLR_BLOCKSIZE];
    long code;

    code = wlr_XDRReaddirIP(callp, &fd, &offset, &len, 0);
    if (code) return code;
    code = wlr_ServerReaddir(callp, fd, offset, len, block, &len2);
    wlr_AfterServerCall(callp, code);
    code = wlr_XDRReaddirOP(callp, block, &len2, 1);
    return code;
}

wlr_StubReadlink(callp)
  struct wlr_call *callp;
{
    char result[WLR_STRINGSIZE];
    char path[WLR_STRINGSIZE];
    long len;
    long code;

    code = wlr_XDRString(callp, path, 0);
    if (code) return code;
    code = wlr_ServerReadlink(callp, path, result, &len);
    wlr_AfterServerCall(callp, code);
    code = wlr_XDRReadlinkOP(callp, result, &len, 1);
    return code;
}

/* main dispatch function */
wlr_Dispatch(callp)
  struct wlr_call *callp;
{
    struct wlr_packet *pktp;
    long code;

    pktp = callp->s;
    if (!pktp) return -3;

    switch(pktp->d.opcode) {
      case WLR_OP_CHMOD:
	code = wlr_StubChmod(callp);
	break;

      case WLR_OP_STAT:
	code = wlr_StubStat(callp);
	break;

      case WLR_OP_OPEN:
	code = wlr_StubOpen(callp);
	break;

      case WLR_OP_READ:
	code = wlr_StubRead(callp);
	break;
	
      case WLR_OP_WRITE:
	code = wlr_StubWrite(callp);
	break;
	
      case WLR_OP_FTRUNC:
	code = wlr_StubFtrunc(callp);
	break;
	
      case WLR_OP_FSTAT:
	code = wlr_StubFstat(callp);
	break;

      case WLR_OP_CLOSE:
	code = wlr_StubClose(callp);
	break;

      case WLR_OP_TRUNC:
	code = wlr_StubTrunc(callp);
	break;

      case WLR_OP_RENAME:
	code = wlr_StubRename(callp);
	break;

      case WLR_OP_UNLINK:
	code = wlr_StubUnlink(callp);
	break;

      case WLR_OP_LINK:
	code = wlr_StubLink(callp);
	break;

      case WLR_OP_SYMLINK:
	code = wlr_StubSymlink(callp);
	break;

      case WLR_OP_MKDIR:
	code = wlr_StubMkdir(callp);
	break;

      case WLR_OP_RMDIR:
	code = wlr_StubRmdir(callp);
	break;

      case WLR_OP_UTIMES:
	code = wlr_StubUtimes(callp);
	break;

      case WLR_OP_READDIR:
	code = wlr_StubReaddir(callp);
	break;

      case WLR_OP_READLINK:
	code = wlr_StubReadlink(callp);
	break;

    }	/* switch statement */

    return code;
}
