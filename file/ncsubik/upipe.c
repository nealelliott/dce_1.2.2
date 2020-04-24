/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1990, 1996 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: upipe.c,v $
 * Revision 1.1.40.1  1996/10/02  17:56:28  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:02  damon]
 *
 * $EndLog$
 */
#include <sys/types.h>
#include <pthread.h>
#include <dce/rpc.h>

#include <dcedfs/icl.h>
#include <dcedfs/compat.h>
#define UBIK_INTERNALS	1
#include <ubik.h>
#include <utrace.h>

extern void ubik_hold(), ubik_rele();
extern struct icl_set *ubikdisk_iclSetp;

/* The functions in this file are called from the RPC with no low-level
 * locks held.  They should obtain them whenever they need to access shared
 * ubik data.  RPC-managed data is not, however, such shared data.
 */

/*
 * This file contains push and pull routines for counted arrays of bytes.
 * The idea is that there is a 4 byte count, followed by exactly that number
 * of bytes.
 */
void upipe_Alloc(astatep, absize, abufpp, abcountp)
struct ubik_pipeState *astatep;	/* pipe's state field pointer */
unsigned long absize;		/* requested size of buffer */
char **abufpp;			/* return pointer to newly allocated buffer */
unsigned long *abcountp; {
  static unsigned char localBuf[1024];
  int cancelState, cancelErr;
  
  DFS_DISABLE_CANCEL(&cancelState, cancelErr);
  *abufpp = (char *)&localBuf[0];
  *abcountp = (absize < 1024)?absize:1024;
  astatep->allocBytes = *abcountp;
  DFS_ENABLE_CANCEL(cancelState, cancelErr);
}

void upipe_FilePull(astatep, abufp, amaxCount, arealCount)
struct ubik_pipeState *astatep;
char *abufp;
unsigned long amaxCount;
unsigned long * volatile arealCount; {
    struct ubik_pipeState * volatile tsp;
    register struct ubik_dbase *tdb;
    volatile long code = 0;
    volatile long tlen;
    volatile int cancelState, cancelErr;
    volatile long excCode=0;
    char *excNameP, excString[20];
    extern char *ExceptionToString();

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    icl_Trace0(ubikdisk_iclSetp, UK_TRACE_PIPE_PULL_ENTRY);
    tsp = (struct ubik_pipeState *) astatep;
    tdb = tsp->dbase;

    /* transfer the bytes, if any available */
    tlen = tsp->xfrBytes;	/* bytes remaining */
    if (tlen > amaxCount) 
       tlen = amaxCount;	/* don't send more than fits in buffer */

    /* do the transfer */
    if (tlen > 0) {
	ubik_hold();
	TRY {
	  code = (*tdb->read)(tdb, /* file # */ 0, abufp, tsp->offset, tlen);
	} CATCH_ALL {
	  excNameP = ExceptionToString(THIS_CATCH);
	  if (excNameP == NULL) {
	    sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excString;
	  }
	  icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		     excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		     __LINE__);
	  excCode=UEXCEPTION;
	} ENDTRY
	ubik_rele();
	if (excCode || code) {
	    tsp->error = (excCode ? UPIPE: code);
	    icl_Trace3(ubikdisk_iclSetp, UK_TRACE_PIPE_PULL,
		       ICL_TYPE_LONG, tlen, ICL_TYPE_LONG, code,
		       ICL_TYPE_LONG, tsp->error);
	    tsp->xfrBytes = 0;	/* should terminate the pipe sooner */
	    *arealCount = 0;	/* could not continue anymore */
	    goto out;
	} 
	tsp->offset += tlen;		/* where we are in the file */
	tsp->xfrBytes -= tlen;	/* fewer bytes remaining */
    }
    *arealCount = tlen;
out:
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_PIPE_PULL_EXIT, ICL_TYPE_LONG,
	       *arealCount);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
}

void upipe_FilePush(astatep, abufp, acount)
struct ubik_pipeState * volatile astatep;
char *abufp;
volatile unsigned long acount; {
    struct ubik_pipeState * volatile tsp;
    register struct ubik_dbase *tdb;
    volatile long code = 0;
    volatile int cancelState, cancelErr;
    volatile long excCode=0;
    char *excNameP, excString[20];
    extern char *ExceptionToString();

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_PIPE_PUSH_ENTRY, ICL_TYPE_LONG, acount);
    tsp = (struct ubik_pipeState *) astatep;
    tdb = tsp->dbase;

    /* transfer the bytes now */
    ubik_hold();
    TRY {
      code = (*tdb->write)(tdb, /* file # */ 0, abufp, astatep->offset, 
			   acount);
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		 __LINE__);
      excCode=UEXCEPTION;
    } ENDTRY
    ubik_rele();
    if (excCode || code) {
        tsp->error = (excCode ? UPIPE : code);
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_PIPE_PUSH,
		   ICL_TYPE_LONG, acount, ICL_TYPE_LONG, code,
		   ICL_TYPE_LONG, tsp->error);
    }
    astatep->offset += acount;	/* adjust file writing offset */
    icl_Trace0(ubikdisk_iclSetp, UK_TRACE_PIPE_PUSH_EXIT);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
}
