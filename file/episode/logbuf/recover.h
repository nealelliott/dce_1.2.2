/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: recover.h,v $
 * Revision 1.1.60.1  1996/10/02  17:25:32  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:43  damon]
 *
 * Revision 1.1.55.2  1994/06/09  14:01:23  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:38  annie]
 * 
 * Revision 1.1.55.1  1994/02/04  20:14:03  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:45  devsrc]
 * 
 * Revision 1.1.53.1  1993/12/07  17:19:39  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:02:48  jaffe]
 * 
 * $EndLog$
 */
/* recover.h */

/*
 * Copyright (C) 1994, 1990 Transarc Corporation - All Rights Reserved
 *
 * $Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/recover.h,v 1.1.60.1 1996/10/02 17:25:32 damon Exp $
 */
/*
 * Based upon the Episode Buffer/Log/Recovery Specification and Design.
 */

#if !defined(TRANSARC_AFS_RECOVER_H)
#define TRANSARC_AFS_RECOVER_H

/* either "Epis" or "sipE" */
#define LOG_SALVAGE_MAGIC_NUMBER (0x73697045)

struct recvrRec {
    struct logPageHdr *page;         /* Page for this log record */
    struct logRec *logrec;           /* Log record */
    struct fifo_Thread  tranThread;  /* Thread linking records on the same
				      * hash chain keyed by transaction id */
    struct fifo *       tranFifoP;   /* Pointer to head of the transaction hash
				      * chain (fifo) containing this elem. */
    /*
     * This comment explains the following 3 fields.
     *
     * The disk hash is organized as a two dimensional data structure.
     * Each element in the hash table represents a different disk address.
     * These elements are linked (by the diskAddrThread fifo_Thread).
     * Hanging off that element (via that element's diskFifo fifo) is a 
     * list (linked by diskThread fifo_Thread) of other elements which also 
     * modify that disk block.
     *
     * XXX Placing fields diskFifo and diskAddrThread here is not elegant, as 
     * only a small percentage of the struct recvrRec elements will actually 
     * use them.
     * -- ra 9/24/93
     */
    struct fifo         diskFifo;       /* See above */
    struct fifo_Thread  diskThread;     /* See above */
    struct fifo_Thread  diskAddrThread; /* See above */

    struct recvrRec *nextLogRec;     /* linked list by time */
    unsigned        status;          /* true if complete */
};

/*
 * Here is the matrix:
 *
 *                  End seen  End not seen
 * Begin seen        REDO       UNDO
 * Begin not seen    REDO       <fail>
 *
 * Records (not whole transactions) are marked SKIP if they predate a kill
 * record on the same block.  The transaction contain such records must have
 * committed (REDO--->SKIP, but not UNDO-/->SKIP).
 */

#define RECVR_STATUS_UNDEF (0x0)
#define RECVR_STATUS_REDO (0x1)
#define RECVR_STATUS_UNDO (0x2)
#define RECVR_STATUS_SKIP (0x3)

/* prototypes */

/* 
 * EXPORT elbr_InitLogRecHdr _TAKES(( struct logRecHdr   *logRecHdrP ));
 */

/* 
 * Initialise a log record header. All fields except dataOffset are
 * set to zero. The dataOffset field is set to an invalid value 0xFFFF.
 */
#define elbr_InitLogRecHdr(logRecHdrP) \
{\
     bzero((char *)(logRecHdrP), sizeof(struct logRecHdr));  \
     (logRecHdrP)->dataOffset = (u_int16)(0xFFFF); /* invalid value */ \
}
/* SHARED */
void elbr_ChecksumLogPage _TAKES((char *logPage, long logPageSize));
/* SHARED */
 u_int16 elbr_FirstLogRecordOffset _TAKES((IN const struct logPageHdr *pgHdr));

EXPORT u_int32 elbr_Recover _TAKES((struct async_device *,
				    struct async_device *, daddr_t *,
				    unsigned));

EXPORT u_int32 elbr_WriteRestartBlock _TAKES((struct async_device *, daddr_t,
					      struct logRec *, unsigned,
					      unsigned char));
EXPORT u_int32 elbr_WriteCheckpointBlock _TAKES((struct async_device *, daddr_t,
						 struct logRec *, unsigned,
						 unsigned, unsigned char));
EXPORT u_int32 elbr_DumpLog _TAKES((struct async_device *, daddr_t *, unsigned,
			       struct recvrRec **, unsigned *));
#if !defined(KERNEL)
EXPORT int32 elbr_ShowRecord _TAKES((struct recvrRec *));
EXPORT int32 elbr_ScriptRecord _TAKES((struct recvrRec *));
#endif

/* SHARED */
void elbr_InitLogPageHdr _TAKES((struct logPageHdr *,unsigned char));

#endif /* TRANSARC_AFS_RECOVER_H */
