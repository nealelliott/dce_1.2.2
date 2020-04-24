/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: local_pipe.h,v $
 * $EndLog$
 */
/*
*/
/*
 * local_pipe.h
 *
 * (C) Copyright 1996 Transarc Corporation
 * All Rights Reserved.
 *
 */

#ifndef _TRANSARC_LOCAL_PIPE_H
#define _TRANSARC_LOCAL_PIPE_H

#include <dcedfs/pipe.h>
#include <dcedfs/lock.h>

#define LOCAL_PIPE_READER_WAITING	0x01
#define LOCAL_PIPE_WRITER_WAITING	0x02
#define LOCAL_PIPE_SHUTDOWN		0x03

/*
 * interface between Local pipe code
 * and generic pipe code
 */

typedef struct localPipe {
    osi_dlock_t lock;
    char *dataBuf;
    long dataBufSize;
    long dataEndFlag;
    char *sendBuf;
    long sendBufSize;
    long sendEndFlag;
    long status;
} localPipe_t;

/* declaration(s) */

int CreateLocalPipe _TAKES((afsPipe_t *, unsigned long));

#endif /* _TRANSARC_LOCAL_PIPE_H */
