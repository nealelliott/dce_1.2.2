/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1994, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>			/* Should be always first */
#include <dcedfs/sysincludes.h>		/* Standard vendor system headers */
#include <cm.h>				/* Cm-based standard headers */

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/cm/RCS/cm_chunk.c,v 4.6 1994/11/02 16:26:14 cfe Exp $")

/*
 * Chunk module: INCOMPLETE...
 */

int cm_chunkoffset(offset)
    long offset;
{
    if (offset < cm_firstcsize) 
	return offset;
    else
	return ((offset - cm_firstcsize) & (cm_othercsize - 1));
}


int cm_chunk(offset)
    long offset;
{
    if (offset < cm_firstcsize)
	return 0;
    else
	return (((offset - cm_firstcsize) >> cm_logchunk) + 1);
}


int cm_chunkbase(offset)
    int offset;
{
    if (offset < cm_firstcsize)
	return 0;
    else
	return (((offset - cm_firstcsize) & ~(cm_othercsize - 1)) + cm_firstcsize);
}


int cm_chunksize(offset)
    long offset;
{
    if (offset < cm_firstcsize)
	return cm_firstcsize;
    else
	return cm_othercsize;
}


int cm_chunktobase(chunk)
    long chunk;
{
    if (chunk == 0)
	return 0;
    else 
	return (cm_firstcsize + ((chunk - 1) << cm_logchunk));
}


int cm_chunktosize(chunk)
    long chunk;
{
    if (chunk == 0)
	return cm_firstcsize;
    else
	return cm_othercsize;
}
