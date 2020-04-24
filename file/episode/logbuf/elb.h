/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: elb.h,v $
 * Revision 1.1.61.1  1996/10/02  17:24:56  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:20  damon]
 *
 * $EndLog$
 */
/* elb.h */

/* (C) Copyright 1994, 1992 Transarc Corporation - All Rights Reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/elb.h,v 1.1.61.1 1996/10/02 17:24:56 damon Exp $ */

#if !defined (TRANSARC_ELB__H)
#define TRANSARC_ELB_H

#include <logbuf.h>

/* Get ICL tracing stuff. */

#include <dcedfs/icl.h>		/* icl_CreateLog */
#include <elb_trace.h>

extern struct icl_set *elb_iclSet;

/* bufFlags */
#define BUFFER_NEW    (0x0)		/* used when initializing buffer */
#define BUFFER_WRITE  (0x00000001)	/* write if set/read if not */
#define BUFFER_INUSE  (0x00000002)	/* not on buffer pool's free list */
#define BUFFER_DIRTY  (0x00000004)	/* modified */
#define BUFFER_LOGGED (0x00000008)	/* this buffer logged */
#define BUFFER_DELETE (0x00000010)	/* <unused> */
#define BUFFER_WRITEP (0x00000020)	/* A write is pending on this buffer */
#define BUFFER_READP  (0x00000040)	/* A read is pending on this buffer */
#define BUFFER_ERROR  (0x00000080)	/* Error while reading this buffer */
#define BUFFER_DISC   (0x00000100)      /* discard when done */
#define BUFFER_RECLM  (0x00000200)      /* attempting to reclaim this buffer */
#define BUFFER_UPDATE (0x00000400)      /* update inprogress */
#define BUFFER_NOTIFY (0x00000800)      /* signal buffer condvar */
/* macros to manipulate buffer flags */

#define BUFFER_CLEARFLAG(bp, flag)  ((bp)->bufFlags &= ~flag)
#define BUFFER_SETFLAG(bp, flag)    ((bp)->bufFlags |= flag)
#define BUFFER_CKFLAG(bp, flag)    ((bp)->bufFlags & flag)

#define bufferSetWrite(bp)   BUFFER_SETFLAG(bp, BUFFER_WRITE)
#define bufferSetRead(bp)    BUFFER_CLEARFLAG(bp, BUFFER_WRITE)
#define bufferSetInuse(bp)   BUFFER_SETFLAG(bp, BUFFER_INUSE)
#define bufferSetDirty(bp)   BUFFER_SETFLAG(bp, BUFFER_DIRTY)
#define bufferSetLogged(bp)  BUFFER_SETFLAG(bp, BUFFER_LOGGED)
#define bufferSetDelete(bp)  BUFFER_SETFLAG(bp, BUFFER_DELETE)
#define bufferSetWritep(bp)  BUFFER_SETFLAG(bp, BUFFER_WRITEP)
#define bufferSetReadp(bp)   BUFFER_SETFLAG(bp, BUFFER_READP)
#define bufferSetError(bp)   BUFFER_SETFLAG(bp, BUFFER_ERROR)
#define bufferSetDiscard(bp) BUFFER_SETFLAG(bp, BUFFER_DISC)
#define bufferSetReclaim(bp) BUFFER_SETFLAG(bp, BUFFER_RECLM)
#define bufferSetUpdate(bp)  BUFFER_SETFLAG(bp, BUFFER_UPDATE)
#define bufferSetNotify(bp)  BUFFER_SETFLAG(bp, BUFFER_NOTIFY)

#define bufferClearWrite(bp)  bufferSetRead(bp)
#define bufferClearInuse(bp)  BUFFER_CLEARFLAG(bp, BUFFER_INUSE)
#define bufferClearDirty(bp)  BUFFER_CLEARFLAG(bp, BUFFER_DIRTY)
#define bufferClearLogged(bp) BUFFER_CLEARFLAG(bp, BUFFER_LOGGED)
#define bufferClearDelete(bp) BUFFER_CLEARFLAG(bp, BUFFER_DELETE)
#define bufferClearWritep(bp) BUFFER_CLEARFLAG(bp, BUFFER_WRITEP)
#define bufferClearReadp(bp)  BUFFER_CLEARFLAG(bp, BUFFER_READP)
#define bufferClearError(bp)  BUFFER_CLEARFLAG(bp, BUFFER_ERROR)
#define bufferClearDiscard(bp) BUFFER_CLEARFLAG(bp, BUFFER_DISC)
#define bufferClearReclaim(bp) BUFFER_CLEARFLAG(bp, BUFFER_RECLM)
#define bufferClearUpdate(bp) BUFFER_CLEARFLAG(bp, BUFFER_UPDATE)
#define bufferClearNotify(bp) BUFFER_CLEARFLAG(bp, BUFFER_NOTIFY)

#define bufferIsWrite(bp)     BUFFER_CKFLAG(bp, BUFFER_WRITE)
#define bufferIsInuse(bp)     BUFFER_CKFLAG(bp, BUFFER_INUSE)
#define bufferIsDirty(bp)     BUFFER_CKFLAG(bp, BUFFER_DIRTY)
#define bufferIsLogged(bp)    BUFFER_CKFLAG(bp, BUFFER_LOGGED)
#define bufferIsDelete(bp)    BUFFER_CKFLAG(bp, BUFFER_DELETE)
#define bufferIsWritep(bp)    BUFFER_CKFLAG(bp, BUFFER_WRITEP)
#define bufferIsReadp(bp)     BUFFER_CKFLAG(bp, BUFFER_READP)
#define bufferIsError(bp)     BUFFER_CKFLAG(bp, BUFFER_ERROR)
#define bufferIsDiscard(bp)   BUFFER_CKFLAG(bp, BUFFER_DISC)
#define bufferIsReclaim(bp)   BUFFER_CKFLAG(bp, BUFFER_RECLM)
#define bufferIsUpdate(bp)    BUFFER_CKFLAG(bp, BUFFER_UPDATE)
#define bufferIsNotify(bp)    BUFFER_CKFLAG(bp, BUFFER_NOTIFY)

#define bufferData(bp) elbb_BufferData(bp)

#define BUFFER_HOLD(bP) ((bP)->refCount++)

/* BUFFER_RELEASE -- decrement a buffer's refCount.  If it reaches zero it
 *     performs a wakeup if one has been requested.  For code density reasons
 *     this doesn't bother to clear the reclaim bit since this feature will be
 *     used rarely, and typically the buffer will be recycled soon after. */

#define BUFFER_RELEASE(bP) \
    (((--(bP)->refCount == 0) && bufferIsReclaim(bP)) \
     ? (epi_cond_broadcast(&bP->cond),0) : 0)


#define LOGREC_FIXEDPART_SIZE (sizeof(struct logRecHdr) + sizeof(u_int32))

/* Debug mask bits for logbuf modules */

/* NOTE -- These are one origin bits (1-32) so converting them to a mask
 *     requires subtracting one.  The usage in this module is inconsistent in
 *     this regard.  See logbuf_SleepPrint macro below for correct usage.  This
 *     is OT 5258. */

#define LOGBUF_DEBUG_BUFFER	1
#define LOGBUF_DEBUG_LOG	2
#define LOGBUF_DEBUG_VLOG       3	/* verbose log info */
#define LOGBUF_DEBUG_LOGREC     4	/* write debug info into log */
#define LOGBUF_DEBUG_RECOVER    5
#define LOGBUF_DEBUG_VRECOVER   6	/* verbose recovery info */
#define LOGBUF_DEBUG_STARTRAN   7	/* write debug msgs into start-tran */
#define LOGBUF_DEBUG_IOFINISH   8	/* track i/o completions */
#define LOGBUF_DEBUG_BUFREUSE   9	/* track buffer reuse */
#define LOGBUF_DEBUG_TRAN      10	/* tran.c tracing */
#define LOGBUF_DEBUG_VTRAN     11	/* verbose tran.c tracing */
#define LOGBUF_DEBUG_VBUFFER   12	/* verbose buffer.c tracing */
#define LOGBUF_DEBUG_DEFERLOGFULL 13	/* skip noncritical logfull handling */

#define LOGBUF_DEBUG_SLEEP	32	/* all calls to sleep */
/* Randomize buffers whose contents are NOT initialized from disk. */
#define LOGBUF_DEBUG_RANDOMIZE_BUFFERS 31
/* insert preemtions, same bit as in anode.p.h */
#define LOGBUF_DEBUG_ADDPREEMPTS     29

#ifdef AFS_DEBUG
EXPORT char *logbuf_debugFilename[11+1]
#if (LOGBUF_DEBUG_BUFFER == DEBUG_THIS_FILE)
 =
  {    "",
       "buffer.c",
       "log.c",
       "log.c",
       "log.c",
       "recover.c",
       "recover.c",
       "buffer.c",
       "buffer.c",
       "buffer.c",
       "tran.c",
       "tran.c",
   }
#endif
;
#endif

#if 0
#define logbuf_DbMask() \
    ((strcmp(__FILE__, "buffer.c") == 0) ? LOGBUF_DEBUG_BUFFER : \
     ((strcmp(__FILE__, "log.c") == 0) ? LOGBUF_DEBUG_LOG : \
      0))
#endif

#define logbuf_DBprint(p) dmprintf (afsdb_episode_logbuf, DEBUG_THIS_FILE, p)
#define logbuf_SleepPrint(p) \
    dprintf (afsdb_episode_logbuf & \
	     (((unsigned)1 << (DEBUG_THIS_FILE - 1)) | \
		((unsigned)1 << (LOGBUF_DEBUG_SLEEP - 1))), p)

#endif /* TRANSARC_ELB_H */
