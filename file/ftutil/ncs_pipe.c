/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ncs_pipe.c,v $
 * Revision 1.1.90.1  1996/10/02  17:49:07  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:50  damon]
 *
 * $EndLog$
*/
/*
 * ftserver_pipe.c
 *
 * NCS realization of pipe abstraction for ftserver.
 *
 * (C) Copyright 1991 Transarc Corporation
 * All Rights Reserved
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
#include <ncs_pipe.h>

/*
 * ReadNCSPipe
 *
 * This is the read op inside the generic afsPipe structure.  It reads in the
 * specified number of bytes (if possible); a short read indicates an EOF
 * condition (the EOF flag is also set in this case.)
 *
 * -1 return indicates an error.
 */

static int ReadNCSPipe(obj, buf, size)
     afsPipe_t *obj;
     char *buf;
     unsigned size;
  {
      ncsPipeData_t *pdP;
      long len;
      long cnt;

      pdP = (ncsPipeData_t *)obj->private_data;

      if (obj->pipe_error) {
	  pdP->status = obj->pipe_error;
	  return -1;
      }

      if (!(obj->flags & PFL_READ)) {
	  pdP->status = EOPNOTSUPP;
	  return -1;
      }

      if (obj->flags & (PFL_CLOSED|PFL_EOF)) {
	  pdP->status = EBADF;
	  return -1;
      }

      /* Use any data we have in the pipe buffer */
      cnt = pdP->dataBufSize - pdP->dataBufOff;
      if (cnt > 0) {
          if (cnt > size) cnt = size;
	  bcopy(pdP->dataBuf + pdP->dataBufOff, buf, cnt);
	  pdP->dataBufOff += cnt;
	  if ( pdP->dataBufOff == pdP->dataBufSize) {
	      pdP->dataBufOff = 0;
	      pdP->dataBufSize = 0;
	  }
      }

      while (cnt < size && !(obj->flags & PFL_EOF)) {
          if (size - cnt >= NCS_BUFFER_SIZE) {
	      /* read the directly into the caller's buffer */
	      (*(pdP->pipeP->pull))(pdP->pipeP->state,
				    (unsigned char *)buf + cnt,
				    size - cnt, (unsigned long *)&len);
	      if (len == 0) obj->flags |= PFL_EOF;
	      else cnt += len;
          } else {
	      /* refill the buffer, copy what we need */
	      (*(pdP->pipeP->pull))(pdP->pipeP->state,
				    (unsigned char *)pdP->dataBuf,
				    NCS_BUFFER_SIZE,
				    (unsigned long *)&pdP->dataBufSize);
	      if (pdP->dataBufSize == 0) {
	          obj->flags |= PFL_EOF;
		  break;
	      }
              len = pdP->dataBufSize;
	      if (len > size - cnt) len = size - cnt;
	      bcopy(pdP->dataBuf, buf + cnt, len);
	      pdP->dataBufOff = len;
	      cnt += len;
          }
      }

      return cnt;
  }

/*
 * WriteNCSPipe
 *
 * This is the write op inside the generic afsPipe structure.  It writes in
 * the specified number of bytes (if possible).
 *
 * -1 indicates an error.
 */

static int WriteNCSPipe(obj, buf, size)
     afsPipe_t *obj;
     char *buf;
     unsigned size;
  {
      ncsPipeData_t *pdP;
      long len;
      long cnt;

      pdP = (ncsPipeData_t *)obj->private_data;

      if (obj->pipe_error) {
	  pdP->status = obj->pipe_error;
	  return -1;
      }

      if (!(obj->flags & PFL_WRITE)) {
	  pdP->status = EOPNOTSUPP;
	  return -1;
      }

      if (obj->flags & (PFL_CLOSED|PFL_EOF)) {
	  pdP->status = EBADF;
	  return -1;
      }

      /* if we already have a partially full buffer, use it */
      if (pdP->dataBufSize) {
	  cnt = NCS_BUFFER_SIZE - pdP->dataBufSize;
	  if (cnt > size) cnt = size;
	  bcopy(buf, pdP->dataBuf + pdP->dataBufOff, cnt);
	  pdP->dataBufOff += cnt;
	  pdP->dataBufSize += cnt;
	
	  /* transmit the buffer if it is full */
	  if (pdP->dataBufSize == NCS_BUFFER_SIZE) {
	      (*(pdP->pipeP->push))(pdP->pipeP->state,
				    (unsigned char *)pdP->dataBuf,
				    pdP->dataBufSize);
	      pdP->dataBufOff = 0;
	      pdP->dataBufSize = 0;
	  }
      } else cnt = 0;

      if (size - cnt >= NCS_BUFFER_SIZE) {
	  /* send the remainder directly */
	  (*(pdP->pipeP->push))(pdP->pipeP->state,
				(unsigned char *)buf + cnt,
				size - cnt);
	  return size;
      } else if (size) {
	  /* copy the rest into the pipe buffer */
	  bcopy(buf + cnt, pdP->dataBuf + pdP->dataBufOff, size - cnt);
	  pdP->dataBufOff += size - cnt;
	  pdP->dataBufSize += size - cnt;
      }

      return size;
  }

/*
 * HoldNCSPipe
 *
 * this is the hold op inside the generic afsPipe structure.  It simply
 * bumps the reference count.
 *
 */

static void HoldNCSPipe(obj)
     afsPipe_t *obj;
  {
      obj->refCount++;
  }

/*
 * ReleaseNCSPipe
 *
 * this is the release op inside the generic afsPipe structure.  It
 * decrements the reference count, and when the count goes to zero it
 * deletes the pipe resources.  Future references to the pipe are not
 * allowed
 */

static void ReleaseNCSPipe(obj)
     afsPipe_t *obj;
  {
      ncsPipeData_t *pdP;

      obj->refCount--;
      if (obj->refCount)
	  return;
      /* last reference, delete */
      pdP = (ncsPipeData_t *)obj->private_data;

      if (!(obj->flags & PFL_CLOSED))
	  (*(obj->pipe_close))(obj, (obj->flags & PFL_DUPLEX));

      osi_Free(pdP->dataBuf, NCS_BUFFER_SIZE);
      osi_Free(pdP, sizeof(ncsPipeData_t));
  }

/*
 * CloseNCSPipe
 *
 * This is the close op inside the generic afsPipe structure.  It marks the
 * pipe as closed.  In the read pipe case, it drains all the remaining
 * input from the pipe.  In the write pipe case it writes 0 bytes,
 * signalling the NCS stub code that the pipe is done.
 */

static int CloseNCSPipe(obj, flags)
     afsPipe_t *obj;
     unsigned flags;
  {
      ncsPipeData_t *pdP;

      if(obj->flags & PFL_NOCLOSE) /* do nothing */
	  return 0;

      pdP = (ncsPipeData_t *)obj->private_data;

      if (flags != (obj->flags & PFL_DUPLEX)) {
	  pdP->status = EINVAL;
	  return -1;
      }

      if (obj->pipe_error) {
	  pdP->status = obj->pipe_error;
	  return -1;
      }

      if ((flags & PFL_READ) && (obj->flags & PFL_EOF) == 0) {
	  /* read remaining data */
	  do {
	      (*(pdP->pipeP->pull))(pdP->pipeP->state,
				    (unsigned char *)pdP->dataBuf,
				    NCS_BUFFER_SIZE,
				    (unsigned long *)&pdP->dataBufSize);
	  } while(pdP->dataBufSize != 0);
      }

      if (flags & PFL_WRITE) {
	  /* flush remaining data */
	  if (pdP->dataBufSize > 0) {
	      (*(pdP->pipeP->push))(pdP->pipeP->state,
				    (unsigned char *)pdP->dataBuf,
				    pdP->dataBufSize);
	  }
	  /* now push zero bytes to signal end-of-pipe */
	  (*(pdP->pipeP->push))(pdP->pipeP->state,
				(unsigned char *)pdP->dataBuf, 0);
      }

      obj->flags |= (PFL_CLOSED|PFL_EOF);

      return 0;
 }

/*
 * StatNCSPipe
 *
 * this code returns the current error status value of the given afsPipe
 * object.
 *
 */
static int StatNCSPipe(obj, type, errVal)
     afsPipe_t *obj;
     unsigned type;
     long *errVal;
  {
      ncsPipeData_t *pdP;

      pdP = (ncsPipeData_t *)obj->private_data;

      *errVal = pdP->status;

      return 0;
  }

/*
 * CrashNCSPipe
 *
 * Abort use of this pipe.
 *
 */
static int CrashNCSPipe(obj, errorCode)
     afsPipe_t *obj;
     unsigned errorCode;
  {
      ncsPipeData_t *pdP;

      obj->pipe_error = errorCode;

      return 0;
  }

/* exported routine */

/*
 * CreateNCSPipe
 *
 * This routine takes a pointer to an NCS pipe structure, an afs pipe
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

int CreateNCSPipe(ncsPipeP, afsPipeP, flags)
     pipe_t *ncsPipeP;
     afsPipe_t *afsPipeP;
     unsigned flags;
  {
      ncsPipeData_t *pdP;
      int noCloseFlag;

      if (!ncsPipeP || !afsPipeP)
	  return -1;

      noCloseFlag = flags & PFL_NOCLOSE;
      flags &= ~PFL_NOCLOSE;

      if (flags != PFL_READ && flags != PFL_WRITE)
	  return -1; /* half-duplex pipes here. */

      pdP = (ncsPipeData_t *)osi_Alloc(sizeof(ncsPipeData_t));


      pdP->pipeP = ncsPipeP;
      pdP->dataBuf = osi_Alloc(NCS_BUFFER_SIZE);
      pdP->dataBufOff = 0;
      pdP->dataBufSize = 0;
      pdP->status = 0;

      afsPipeP->refCount = 1;
      afsPipeP->flags = flags | noCloseFlag;
      afsPipeP->pipe_error = 0;
      afsPipeP->pipe_read = ReadNCSPipe;
      afsPipeP->pipe_write = WriteNCSPipe;
      afsPipeP->pipe_hold = HoldNCSPipe;
      afsPipeP->pipe_rele = ReleaseNCSPipe;
      afsPipeP->pipe_close = CloseNCSPipe;
      afsPipeP->pipe_stat = StatNCSPipe;
      afsPipeP->pipe_crash = CrashNCSPipe;
      afsPipeP->private_data = (char *)pdP;

      return 0;
  }
