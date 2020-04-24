/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: file_pipe.c,v $
 * Revision 1.1.27.1  1996/10/02  17:48:23  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:20  damon]
 *
 * Revision 1.1.22.3  1994/07/13  22:28:08  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:35:20  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:12  mbs]
 * 
 * Revision 1.1.22.2  1994/06/09  14:09:04  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:44  annie]
 * 
 * Revision 1.1.22.1  1994/02/04  20:19:29  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:30  devsrc]
 * 
 * Revision 1.1.20.2  1994/01/20  18:43:05  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:20  annie]
 * 
 * Revision 1.1.20.1  1993/12/07  17:25:26  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:04:39  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1993 Transarc Corporation
 * All Rights Reserved
 */

#include <errno.h>

#include <dcedfs/stds.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/file_pipe.c,v 1.1.27.1 1996/10/02 17:48:23 damon Exp $")

#include <file_pipe.h>

typedef struct filePipeData {
    FILE*	fileStream;
    long	status;
} filePipeData_t;


/*
 * ReadFilePipe()
 */
PRIVATE int ReadFilePipe(obj, buf, size)
    IN afsPipe_t*	obj;
    OUT char*		buf;
    IN unsigned		size;
{
    filePipeData_t*	pdP;
    int			numBytes;

    pdP = (filePipeData_t*)obj->private_data;

    if (!(obj->flags & PFL_READ)) {
	pdP->status = EOPNOTSUPP;
	return -1;
    }

    if (obj->flags & (PFL_CLOSED|PFL_EOF)) {
	pdP->status = EBADF;
	return -1;
    }

    errno = 0;
    if ((numBytes = fread(buf, 1, size, pdP->fileStream)) != size)
	if (errno != 0) {
	    pdP->status = errno;
	    return -1;
	} else if (numBytes == 0) {
	    obj->flags |= PFL_EOF;
	}

    return numBytes;
}	/* ReadFilePipe() */

/*
 * WriteFilePipe()
 */
PRIVATE int WriteFilePipe(obj, buf, size)
    IN afsPipe_t*	obj;
    IN char*		buf;
    IN unsigned		size;
{
    filePipeData_t*	pdP;

    pdP = (filePipeData_t*)obj->private_data;

    if (!(obj->flags & PFL_WRITE)) {
	pdP->status = EOPNOTSUPP;
	return -1;
    }

    if (obj->flags & (PFL_CLOSED|PFL_EOF)) {
	pdP->status = EBADF;
	return -1;
    }

    if (fwrite(buf, 1, size, pdP->fileStream) != size) {
	pdP->status = errno;
	return -1;
    }

    return size;
}	/* WriteFilePipe() */

/*
 * HoldFilePipe()
 */
PRIVATE void HoldFilePipe(obj)
    INOUT afsPipe_t*	obj;
{
    obj->refCount++;
}	/* HoldFilePipe() */

/*
 * ReleaseFilePipe()
 */
PRIVATE void ReleaseFilePipe(obj)
    INOUT afsPipe_t*	obj;
{
    filePipeData_t*	pdP;
      
    if (--obj->refCount)
	return;

    /* last reference, delete */
    pdP = (filePipeData_t*)obj->private_data;
      
    if (!(obj->flags & PFL_CLOSED))
	(*(obj->pipe_close))(obj, (obj->flags & PFL_DUPLEX));

    osi_Free(pdP, sizeof *pdP);
}	/* ReleaseFilePipe() */

/*
 * CloseFilePipe()
 */
PRIVATE int CloseFilePipe(obj, flags)
    IN afsPipe_t*	obj;
    IN unsigned		flags;
{
    filePipeData_t*	pdP;
      
    if(obj->flags & PFL_NOCLOSE)	/* do nothing */
	return 0;

    pdP = (filePipeData_t*)obj->private_data;

    if (flags != (obj->flags & PFL_DUPLEX)) {
	pdP->status = EINVAL;
	return -1;
    }
      
    /* flush remaining data */
    if ((flags & PFL_WRITE) && fflush(pdP->fileStream) == EOF) {
	pdP->status = errno;
	return -1;
    }

    obj->flags |= (PFL_CLOSED|PFL_EOF);

    return 0;
}	/* CloseFilePipe() */

/*
 * StatFilePipe()
 */
PRIVATE int StatFilePipe(obj, type, errVal)
    IN afsPipe_t*	obj;
    IN unsigned		type;
    OUT long*	errVal;
{
    filePipeData_t*	pdP;
      
    pdP = (filePipeData_t*)obj->private_data;

    *errVal = pdP->status;
      
    return 0;
}	/* StatFilePipe() */

/*
 * CreateFilePipe()
 */
EXPORT int CreateFilePipe(fileStream, pipeP, flags)
    IN FILE*		fileStream;
    OUT afsPipe_t*	pipeP;
    IN unsigned		flags;
{
    filePipeData_t	*pdP;
    unsigned		noCloseFlag;

    if (!fileStream || !pipeP)
	return -1;

    noCloseFlag = flags & PFL_NOCLOSE;
    flags &= ~PFL_NOCLOSE;

    if ((flags & PFL_DUPLEX) == PFL_DUPLEX)
	return -1;			/* only half-duplex pipes here. */

    pdP = (filePipeData_t *)osi_Alloc(sizeof *pdP);

    pdP->fileStream = fileStream;

    pipeP->refCount = 1;
    pipeP->flags = flags | noCloseFlag;
    pipeP->pipe_read = ReadFilePipe;
    pipeP->pipe_write = WriteFilePipe;
    pipeP->pipe_hold = HoldFilePipe;
    pipeP->pipe_rele = ReleaseFilePipe;
    pipeP->pipe_close = CloseFilePipe;
    pipeP->pipe_stat = StatFilePipe;
    pipeP->private_data = (char *)pdP;

    return 0;
}


