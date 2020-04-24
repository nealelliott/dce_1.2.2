/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bos_pipeFile.c,v $
 * Revision 1.1.14.1  1996/10/02  21:06:14  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:17  damon]
 *
 * Revision 1.1.9.2  1994/07/13  22:23:05  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:53:48  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:58  mbs]
 * 
 * Revision 1.1.9.1  1994/06/09  14:23:57  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:35:54  annie]
 * 
 * Revision 1.1.5.3  1993/01/21  16:28:37  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:21:51  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  19:50:24  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:35:22  bolinger]
 * 
 * Revision 1.1.3.3  1992/01/24  04:13:40  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:21:23  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:27:19  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:45:22  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:23:58  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
 */
/*
 *  bos_pipeFile.c -- implementations of routines that handle the client end of NCS pipes
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>


#include <dcedfs/osi.h>

#include <dce/rpc.h>
#include <dce/stubbase.h>

#include <bos_pipeFile.h>

/*
 * this would probably be the right place to bind the state to the stream ptr,
 * but it's not clear how to get a name of any type of description of the
 * desired file into the routine
 */
/*
 * Despite appearances, this is thread-safe since it is only called by the
 * single-threaded (in this case) client code.
 */
EXPORT void FilePipeAlloc(state, bsize, buf, bcount)
     void *		state;	/* in: pipe's state pointer */
     ndr_ulong_int	bsize;	/* in: requested size (# of *bytes*) */
     ndr_byte **	buf;	/* out: pointer to allocated buffer */
     ndr_ulong_int *	bcount;	/* out: size of alloc'd buffer (# of *bytes*) */
{
  static ndr_byte	theBuffer[NIDL_PIPE_BUFF_SIZE];
  
  *buf = theBuffer;
  *bcount = NIDL_PIPE_BUFF_SIZE;
}

/* this routine must read data from a file and put it into the buffer */
EXPORT void FilePipePull(state, buf, esize, ecount)
     void *		state;	/* in: pipe's state pointer */
     ndr_byte *		buf;	/* in: buffer in which to place a chunk */
     ndr_ulong_int	esize;	/* in: buffer size (# of elements) */
     ndr_ulong_int *	ecount;	/* out: size of chunk (# of elements) */
{
  *ecount = (ndr_ulong_int)fread((char *)buf, sizeof(ndr_byte), esize, (FILE *)state);
}

/* this routine must take data from the buffer and write it into a file */
EXPORT void FilePipePush(state, buf, ecount)
     void *		state;	/* in: pipe's state pointer */
     ndr_byte *		buf;	/* in: buffer from which to copy chunk */
     ndr_ulong_int	ecount;	/* in: size of chunk (# of elements) */
{
  fwrite((char *)buf, sizeof(ndr_byte), ecount, (FILE *)state);
}

