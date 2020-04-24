/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: local_pipe.c,v $
 * $EndLog$
*/
/*
 * local_pipe.c
 *
 * local realization of pipe abstraction for ftserver.
 *
 * (C) Copyright 1996, 1991 Transarc Corporation.
 * All Rights Reserved.
 */

#include <fcntl.h>
#include <dcedfs/param.h>
#include <dcedfs/common_data.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <sys/errno.h>
#include <dce/rpc.h>
#include <dcedfs/compat.h>
 
#include <pipe.h>
#include <local_pipe.h>

/*
 *
 * SERVER SIDE ROUTINES
 *
 */

/*
 * ReadLocalPipe
 *
 * This is the read op inside the generic afsPipe structure.  Local pipes
 * only support writes. It is an error to do a read on a local pipe.
 *
 * -1 return indicates an error.
 */

static int ReadLocalPipe(obj, buf, size)
     afsPipe_t *obj;
     char *buf;
     unsigned size;
  {
      return -1;
  }

/*
 * WriteLocalPipe
 *
 * This is the write op inside the generic afsPipe structure.  It writes in
 * the specified number of bytes (if possible).
 *
 * -1 indicates an error.
 */

static int WriteLocalPipe(obj, buf, size)
     afsPipe_t *obj;
     char *buf;
     unsigned size;
  {
      localPipe_t *pdP;
      long len;
      long cnt;

      pdP = (localPipe_t *)obj->private_data;
      lock_ObtainWrite(&pdP->lock);

      if (obj->pipe_error) {
	  pdP->status = obj->pipe_error;
	  osi_Wakeup(pdP);
	  lock_ReleaseWrite(&pdP->lock);
	  return -1;
      }

      if (!(obj->flags & PFL_WRITE)) {
	  pdP->status = EOPNOTSUPP;
	  osi_Wakeup(pdP);
	  lock_ReleaseWrite(&pdP->lock);
	  return -1;
      }

      if (obj->flags & (PFL_CLOSED|PFL_EOF)) {
	  pdP->status = EBADF;
	  osi_Wakeup(pdP);
	  lock_ReleaseWrite(&pdP->lock);
	  return -1;
      }

      /*
       * Put the buffer information into the pipe buffer and wait
       * for our counterpart to drain the data.
       */
      pdP->dataBuf = buf;
      pdP->dataBufSize = size;
      osi_Wakeup(pdP);
      while(pdP->dataBuf && !pdP->status) {
	  osi_SleepW(pdP, &pdP->lock);
          lock_ObtainWrite(&pdP->lock);

      }

      if (pdP->status) {
	  lock_ReleaseWrite(&pdP->lock);
	  return -1;
      }

      lock_ReleaseWrite(&pdP->lock);
      return size;
  }

/*
 * HoldLocalPipe
 *
 * this is the hold op inside the generic afsPipe structure.  It simply
 * bumps the reference count.
 */

static void HoldLocalPipe(obj)
     afsPipe_t *obj;
  {
      obj->refCount++;
  }

/*
 * ReleaseLocalPipe
 *
 * this is the release op inside the generic afsPipe structure.  It
 * decrements the reference count, and when the count goes to zero it
 * deletes the pipe resources.  Future references to the pipe are not
 * allowed
 */

static void ReleaseLocalPipe(obj)
     afsPipe_t *obj;
  {
      localPipe_t *pdP;

      obj->refCount--;
      if (obj->refCount)
	  return;
      /* last reference, delete */
      pdP = (localPipe_t *)obj->private_data;

      if (!(obj->flags & PFL_CLOSED))
	  (*(obj->pipe_close))(obj, (obj->flags & PFL_DUPLEX));

      osi_Free(pdP, sizeof(localPipe_t));
  }

/*
 * CloseLocalPipe
 *
 * This is the close op inside the generic afsPipe structure.
 * It marks the pipe as closed. 
 */

static int CloseLocalPipe(obj, flags)
     afsPipe_t *obj;
     unsigned flags;
  {
      localPipe_t *pdP;

      if(obj->flags & PFL_NOCLOSE) /* do nothing */
	  return 0;

      pdP = (localPipe_t *)obj->private_data;
      lock_ObtainWrite(&pdP->lock);

      if (flags != (obj->flags & PFL_DUPLEX)) {
	  pdP->status = EINVAL;
	  osi_Wakeup(pdP);
	  lock_ReleaseWrite(&pdP->lock);
	  return -1;
      }

      if (obj->pipe_error) {
	  pdP->status = obj->pipe_error;
	  osi_Wakeup(pdP);
	  lock_ReleaseWrite(&pdP->lock);
	  return -1;
      }

      /* mark the pipe closed */
      pdP->dataEndFlag = 1;
      osi_Wakeup(pdP);
      lock_ReleaseWrite(&pdP->lock);

      obj->flags |= (PFL_CLOSED|PFL_EOF);

      return 0;
  }

/*
 * StatLocalPipe
 *
 * this code returns the current error status value of the given afsPipe
 * object.
 *
 */
static int StatLocalPipe(obj, type, errVal)
     afsPipe_t *obj;
     unsigned type;
     long *errVal;
  {
      localPipe_t *pdP;

      pdP = (localPipe_t *)obj->private_data;

      lock_ObtainWrite(&pdP->lock);
      *errVal = pdP->status;
      lock_ReleaseWrite(&pdP->lock);

      return 0;
  }

/*
 * CrashLocalPipe
 *
 * Abort use of this pipe.
 *
 */
static int CrashLocalPipe(obj, errorCode)
     afsPipe_t *obj;
     unsigned errorCode;
  {
      localPipe_t *pdP;

      pdP = (localPipe_t *)obj->private_data;

      obj->pipe_error = errorCode;
      lock_ObtainWrite(&pdP->lock);
      pdP->status = errorCode;
      osi_Wakeup(pdP);
      lock_ReleaseWrite(&pdP->lock);

      return 0;
  }

/* exported routine */

/*
 * CreateLocalPipe
 *
 * This routine takes a pointer to an Local pipe structure, an afs pipe
 * structure, and a flags field (which tells which type of pipe this is)
 *
 * It initializes the afsPipeP structure to allow future use for manipulating
 * this pipe using the standard pipe operators.
 *
 * Side effects: this sets the reference count to afsPipeP.
 *
 * Locking: no locking is done within this package.  The caller must do any
 * locking necessary.
 *
 */

int CreateLocalPipe(afsPipeP, flags)
     afsPipe_t *afsPipeP;
     unsigned long flags;
  {
      localPipe_t *pdP;
      unsigned long noCloseFlag;

      if (!afsPipeP)
	  return -1;

      noCloseFlag = flags & PFL_NOCLOSE;
      flags &= ~PFL_NOCLOSE;

      /* output pipes only */
      if (flags != PFL_WRITE) {
          return -1;
      }

      pdP = (localPipe_t *)osi_Alloc(sizeof(localPipe_t));
      if (!pdP)
	  return -1;

      lock_Init(&pdP->lock);
      pdP->dataBuf = NULL;
      pdP->dataBufSize = 0;
      pdP->dataEndFlag = 0;
      pdP->sendBuf = NULL;
      pdP->sendBufSize = 0;
      pdP->sendEndFlag = 0;
      pdP->status = 0;

      afsPipeP->refCount = 1;
      afsPipeP->flags = flags | noCloseFlag;
      afsPipeP->pipe_error = 0;
      afsPipeP->pipe_read = ReadLocalPipe;
      afsPipeP->pipe_write = WriteLocalPipe;
      afsPipeP->pipe_hold = HoldLocalPipe;
      afsPipeP->pipe_rele = ReleaseLocalPipe;
      afsPipeP->pipe_close = CloseLocalPipe;
      afsPipeP->pipe_stat = StatLocalPipe;
      afsPipeP->pipe_crash = CrashLocalPipe;
      afsPipeP->private_data = (char *)pdP;

      return 0;
  }

/*
 *
 * CLIENT SIDE ROUTINES
 *
 *
 *
 */

void localPipeAlloc(state, bsize, buf, nbytes)
    void *state;
    idl_ulong_int bsize;
    idl_byte **buf;
    idl_ulong_int *nbytes;
{
      afsPipe_t *obj = (afsPipe_t *)state;
      localPipe_t *pdP = (localPipe_t *)obj->private_data;

      /*
       * We only synchronize with the sender at the beginning and the
       * end of each buffer. We keep a private buffer pointer to avoid
       * synchronization overhead.
       */

      if (!pdP->sendBufSize) {
	  lock_ObtainWrite(&pdP->lock);

	  /* notify the sender when a buffer has been sent */
	  if (pdP->sendBuf) {
	      pdP->dataBuf = NULL;
	      osi_Wakeup(pdP);
	  }

	  /* wait for the sender to post an output buffer */
	  while (!pdP->dataBuf && !pdP->dataEndFlag && !pdP->status) {
	      osi_SleepW(pdP, &pdP->lock);
              lock_ObtainWrite(&pdP->lock);
	  }

	  if (pdP->dataEndFlag || pdP->status) {
	      pdP->sendEndFlag = 1;
	  } else {
	      pdP->sendBuf = pdP->dataBuf;
	      pdP->sendBufSize = pdP->dataBufSize;
	  }

	  lock_ReleaseWrite(&pdP->lock);
      }

      /* allocate directly from the sender's buffer */
      if (!pdP->sendEndFlag) {
          *buf = (idl_byte *)pdP->sendBuf;
          *nbytes = pdP->sendBufSize;
      } else {
	  *buf = (idl_byte *)pdP;
	  *nbytes = bsize;
      }
}

void localPipePull(state, buf, bsize, nbytes)
    void *state;
    idl_byte *buf;
    idl_ulong_int bsize;
    idl_ulong_int *nbytes;
{
      afsPipe_t *obj = (afsPipe_t *)state;
      localPipe_t *pdP = (localPipe_t *)obj->private_data;

      if (!pdP->sendEndFlag) {
          *nbytes = bsize;
          pdP->sendBuf += bsize;
          pdP->sendBufSize -= bsize;
      } else {
	  *nbytes = 0;
      }
}

