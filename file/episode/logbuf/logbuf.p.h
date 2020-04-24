/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: logbuf.p.h,v $
 * Revision 1.1.74.1  1996/10/02  17:25:07  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:24  damon]
 *
 * Revision 1.1.69.2  1994/06/09  14:01:04  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:19  annie]
 * 
 * Revision 1.1.69.1  1994/02/04  20:13:46  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:38  devsrc]
 * 
 * Revision 1.1.67.1  1993/12/07  17:19:26  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:01:20  jaffe]
 * 
 * $EndLog$
*/
/* logbuf.p.h -- the prefix for the exported logbuf.h file. */

/*
 * (C) Copyright 1994, 1990 Transarc Corporation
 * All Rights Reserved
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/logbuf.p.h,v 1.1.74.1 1996/10/02 17:25:07 damon Exp $ */

#if !defined(TRANSARC_ELB_LOGBUF_H)
#define TRANSARC_ELB_LOGBUF_H

#include <dcedfs/osi_uio.h>
#include <dcedfs/episode/async.h>	/* gets epi_thread.h too. */
#include <dcedfs/episode/fifo.h>
#include <dcedfs/episode/instrument.h>

/*
 * users of this package are restricted to the first two cases
 */

enum elbb_bufType {
    elbb_bufData,	/* data buffers */
    elbb_bufLogged,	/* logged buffers */
    elbb_bufLog,	/* buffers used by log package - internal use only */
    elbb_bufRecvr,	/* buffer used by recovery package - internal only */
    elbb_bufUnused	/* empty/not in use buffer - internal use only */
};

/*
 * write back modes
 */

enum elbb_WBMode {
    elbb_async,   /* schedule write now, but don't wait for completion */
    elbb_syncLog, /* force write log, schedule buffer write, don't wait
                   * for data buffer write completion. */
    elbb_sync,    /* force write log, force write data buffer, upon return
                   * both log and data buffer have been written */
    elbb_lazy,    /* defer write until buffer package schedules write */
    elbb_adel,    /* schedule write now, reuse upon completion of write. */
    elbb_sdel     /* schedule write now, wait, reuse upon completion. */
};

#define IsValidWBMode(mode) \
    ((mode == elbb_async) || \
     (mode == elbb_syncLog) || \
     (mode == elbb_sync) || \
     (mode == elbb_lazy) || \
     (mode == elbb_adel) || \
     (mode == elbb_sdel))

typedef enum elbb_WBMode elbb_WBMode_t;
typedef u_int32 elbt_TranRec_t;
typedef elbt_TranRec_t buffer_tranRec_t;
typedef elbt_TranRec_t elbb_tranRec_t;
#define elbt_NullTranId ((elbt_TranRec_t)0)
#define elbt_IsNullTranId(trans) (trans == 0)
#define elbb_zeroTrans elbt_NullTranId
#define elbb_NullTranId elbt_NullTranId
#define elbb_IsZeroTrans(t) elbt_IsNullTranId(t)

/*
 * the actual buffer header.  Note that the order and placement of the adev &
 * blockNum fields is significant XXX but we won't say why.
 *
 * XXX Name space pollution XXX
 */
struct buffer {
    u_int32             bufFlags;	/* flags for buffer */
    enum elbb_bufType   bufType;	/* type of buffer */
    u_int32             refCount;	/* # of total references */
    struct buffer      *addrHashNext;   /* next element in address hash */
    struct fifo_Thread  freeList;       /* fifo thread for free list */
    struct fifo_Thread  ageQ;           /* fifo thread for age queue */
    struct fifo        *currentAgeQ;    /* associated age queue */
    caddr_t             data;		/* data address of buffer */
    async_device_t     *adev;		/* aggregate specification */
    daddr_t             blockNum;	/* block # on device */
    u_int16             bufLogFirstPass;/* pass # */
    u_int16             bufLogFirstOff;	/* log page offset */
    u_int16             bufLogLastPass; /* last pass */
    u_int16             bufLogLastOff;  /* last offset */
    elbt_TranRec_t      delTran;	/* <unused> */
    asevent_event_t	*aevent;	/* for event handler */
    int                 whenFreed;      /* for cache heuristics */
    u_int32             bufFH;          /* magic # from higher layer */
    struct buffer      *bufFHNext;      /* next buffer in hash bucket */
    struct fifo         fhList;         /* FH list */
    osi_dlock_t		bLock;		/* lock on this buffer */
    epi_cond_t		cond;		/* condition variable */
    opaque              poolP;          /* pointer to appropriate pool */
    struct fifo_Thread  blockThread;    /* buffers threaded on the block */
    elbt_TranRec_t      uzTran;         /* undo zero transaction (if any) */
};

#define elbb_buffer buffer

typedef struct buffer elbb_buffer_t;

#define elbb_BufferData(bp)	((bp)->data)
#define elbb_BufferDevice(bp)	((bp)->adev)
#define elbb_BufferBlock(bp)	((bp)->blockNum)
#define elbb_Zero(bp, off, len, tran, flag, bpp) \
    buffer_Fill(bp, off, 0, len, 1 /* save old */, tran, flag, bpp)


/* flags for elbb_ReadGeneral */
#define BUFFER_RG_READDISK  (0x1)
#define BUFFER_RG_DONTWAIT  (0x2)

#define BUFFER_RG_VALIDBITS (0x3)

/*
 * struct elbt_TranRec -- describes a transaction identified by a tranId.  The
 *     stucture is primarily managed by the tran module, but with significant
 *     help and cooperation from the log module.  The first group of fields are
 *     protected by tran module locks (see logbuf-locking for details).  The
 *     last group of fields are actually managed by the log module.
 *
 *     tranSize -- is protected by the tran lock but actually incremented by
 *         the log module in the WriteLogRecord macro.
 */

struct elbt_TranRec {
    elbt_TranRec_t	      tranId;      /* transaction identifier */
    u_int16                   refCount;    /* # of references */
    struct fifo_Thread        statusThread;/* free, active, complete */
    struct elbt_TranRec      *nextHash;    /* singly linked hash chain */
    unsigned                  sleepers;    /* true if someone sleeping */
    struct fifo               ecList;      /* ec information */
    struct elbt_TranRec      *ecHead;      /* head of this EC */
    unsigned                  ecSize;      /* # of elements in this EC */
    unsigned                  tranStatus;  /* status of this transaction */
    async_device_t	     *dev;	   /* data device */
    u_int32                   tranSize;    /* # of bytes used by tran */
    osi_dlock_t		      tranLock;	   /* tran record lock */
    /* bit fields */
    unsigned		      nested:1;	   /* transaction is nested */

    /* debugging information */
    const char *              opString;    /* character ptr */
    u_int32                   opHash;      /* a hash value from opString */

    /* log managed data structures */
    struct fifo_Thread        logMapThread;/* attached to log map */
    struct fifo_Thread        logAgeThread;/* used for aging */
    u_int16                   firstPass;   /* pass # when started */
    u_int16                   lastPass;    /* pass # @ last operation */
    u_int16                   firstOffset; /* beginning offset into log */
    u_int16                   lastOffset;  /* ending offset into log */
    async_device_t	     *ldev;	   /* log device */
    opaque                    mapP;        /* points to log map entry */
    u_int32		      reserved;	   /* bytes reserved for this tran */
    /* flags: bit fields */
    unsigned		      atran:1;	   /* on aTranList */
    unsigned		      ptran:1;	   /* on pTranList */
    unsigned		      ctran:1;	   /* on cTranList */
    unsigned		      ageq:1;	   /* on tranAgeQ */
};

struct elbb_params {
    caddr_t bufferPool;
    u_int32 byteCount;
    long spare[6];
};

/* function prototypes for use with log buffer package */

int32 elbb_Init _TAKES((struct elbb_params *));

int32 elbb_Tune _TAKES((u_int32, u_int32, opaque *));
/*
 * operations values for elbb_Tune.
 */

#define BUFFER_GETVAL 0x0      /* read value */
#define BUFFER_SETVAL 0x1      /* set value */

#define BUFFER_DEBUG   0x100   /* control debugging variable */
#define BUFFER_DECAY1  0x120   /* control logbuf_DecayRate1 */
#define BUFFER_DECAY2  0x121   /* control logbuf_DecayRate2 */

#define BUFFER_VER_STR 0x130   /* version string */
#define BUFFER_VER_DATE 0x131  /* version date string */
#define BUFFER_VER_NUM 0x132   /* verison number (from sum) */

#define BUFFER_MAXTRAN 0x200   /* control log_MaxTran */
#define BUFFER_TRANSOFT 0x201  /* control log_TranSoftLimit */
#define BUFFER_TRANHARD 0x202  /* control log_TranHardLimit */
#define BUFFER_MAXLOG  0x210   /* control log_MaxLogs */
#define BUFFER_LOGLIMIT 0x220  /* control % log size in 0.78125% increments */

/*
 * Function prototypes - external
 */

int32 elbb_ReadGeneral _TAKES((
  IN async_device_t *,
  IN daddr_t,
  IN long options,
  IN enum elbb_bufType,
  OUT struct buffer **));

int32 elbb_Read(
  IN async_device_t *dP,
  IN daddr_t blkno,
  IN enum elbb_bufType bt,
  OUT struct buffer **bPP);

int32 elbb_TranIsCommitted(IN elbt_TranRec_t bufTranId);

int32 elbb_SetFH(struct buffer *, u_int32);

int32 elbb_ReadAhead(async_device_t *, daddr_t, enum elbb_bufType);

int32 elbb_Release(struct buffer *, elbb_WBMode_t);

int32 elbb_ModifySource _TAKES((
  struct buffer *, u_int32, const caddr_t, u_int32,
  osi_uio_seg_t, elbt_TranRec_t, int, struct buffer **));

int32 elbb_FastModify _TAKES((async_device_t *, daddr_t, u_int32,
				caddr_t, u_int32, elbt_TranRec_t));

int32 elbb_Fill _TAKES((IN struct buffer *,
			  IN u_int32, /* offset */
			  IN int, /* pattern */
			  IN u_int32, /* # *bytes* to fill */
			  IN unsigned, /* save old data */
			  IN elbt_TranRec_t, /* transaction */
			  IN int, /* logged */
			  OUT struct buffer **));

int32 elbb_CopyBlock _TAKES((IN struct buffer *,
			     IN async_device_t *,
			     IN daddr_t));

int32 elbb_Initialize _TAKES((IN struct buffer *,
				IN u_int32,		/* offset */
				IN u_int32,		/* length */
				IN elbt_TranRec_t,	/* transaction */
				IN int,			/* logged */
				OUT struct buffer **));

int32 elbt_ECMerge _TAKES((elbt_TranRec_t, elbt_TranRec_t, unsigned long));

int32 elbb_StartTran _TAKES((caddr_t, elbt_TranRec_t,
			       async_device_t *, elbt_TranRec_t *));

extern int32 elbt_GetTranStatus _TAKES((elbt_TranRec_t));
extern void elbt_EndTran _TAKES((elbt_TranRec_t));

int32 elbb_LogInit _TAKES((async_device_t *, async_device_t *,
			     daddr_t *, unsigned));

/* elbb_LogForce -- forces a specified trans into a specified state, maybe.
 *     See also comment in tran.c for elbt_TranForce.
 *
 * PARAMETERS -- waitOK is specifies a spectrum of behaviors for this routine.
 *     0 -- caller can't wait for other transactions to end.  Commit only if
 *         transaction is complete, otherwise return BUF_WOULDBLOCK.
 *     1 -- don't return until transaction has committed.
 *     2 -- don't return until transaction has completed. *
 */
#define elbb_LogForce(t, w) elbt_TranForce(t, w)
extern long elbt_TranForce _TAKES((elbt_TranRec_t, unsigned waitOK));

extern int32 elbb_LogCreate _TAKES((
  IN async_device_t *ldP,
  IN async_device_t *dP,
  IN daddr_t *logLocnArray,
  IN unsigned logLocnArraySize,
  IN unsigned char cksumVer ));

extern int32 elbb_FsyncForce _TAKES((async_device_t *, u_int32, daddr_t *,
                                u_int32));
extern void elbb_SyncForce(void);

extern int32 elbb_Shutdown _TAKES((async_device_t *));

extern int32 elbb_Kill _TAKES((
  IN async_device_t *dP,
  IN daddr_t blkno,
  IN int metadataOK));

extern void elbb_Reserve _TAKES((int, unsigned));
/* EXPORT int elbr_GetCleanLogInfo(IN async_device_t *, IN daddr_t,
   INOUT struct logPageHdr)); */
extern int elbr_GetCleanLogInfo _TAKES((IN async_device_t *,
					IN daddr_t,
					INOUT opaque));
/* EXPORT int elbr_IsLogClean _TAKES((async_device_t *, daddr_t)); */
#define elbr_IsLogClean(dP,r) elbr_GetCleanLogInfo(dP,r,0)

/* elbb_QuiesceSystem -- brings a device to a consistent state.  The level on
 *     consistency is specified by the second parameter.
 *
 * PARAMETERS --
 *     dP -- specified the device to be quiesced.
 *     consistency -- is some combination of the following bits.  The tran bits
 *         are independent of the data bits.  No more than one bit in each
 *         group need be specified.
 *             SERIALIZE -- ensure all currently active trans have ended.
 *             COMMIT -- as above but wait until all trans have committed.
 *
 *             WRITE -- schedules all dirty buffers containing user data.
 *             FSYNC -- also waits for writes to complete.
 *             FLUSH -- also invalidates user buffers (vol_close).
 *             FLUSHALL -- also invalidates all buffers (elbb_Shutdown).
 */
#define ELBB_QUIESCE_SERIALIZE	(1<<0)
#define ELBB_QUIESCE_COMMIT	(1<<1)
#define ELBB_QUIESCE_WRITE	(1<<2)
#define ELBB_QUIESCE_FSYNC	(1<<3)
#define ELBB_QUIESCE_FLUSH	(1<<4)
#define ELBB_QUIESCE_FLUSHALL	(1<<5)

extern void elbb_QuiesceSystem _TAKES((async_device_t *dP,
                                       unsigned int consistency));

extern unsigned elbl_MinLogSize _TAKES((async_device_t *, unsigned));
extern void elbb_UndoZero _TAKES((IN async_device_t *, IN daddr_t,
                                  IN u_int32, IN u_int32, IN elbt_TranRec_t));
extern void elbb_UndoZeroBuffer _TAKES((IN struct buffer *, IN u_int32,
                                        IN u_int32, IN elbt_TranRec_t,
                                        OUT struct buffer **));
extern long elbl_Tune(IN u_long op, IN u_long type, INOUT opaque *value);

extern int ins_elbb(
  struct ins_opDesc *opcode,
  struct ins_varDesc *vars,
  struct ins_basicObjectId *argObjIds,
  struct ins_basicObjectId *resultObjIds,
  struct ins_basicObject *resultValues
);

extern int ins_elbb_tune(
  IN struct ins_opDesc *opcode,
  IN struct ins_varDesc *vars,
  IN struct ins_basicObjectId *argObjIds,
  OUT struct ins_basicObjectId *resultObjIds,
  OUT struct ins_basicObject *resultValues
);

#define elbb_MinLogSize elbl_MinLogSize
#define elbb_MinTranReservation elbl_MinTranReservation

/* backwards compatibility macros */

typedef elbt_TranRec_t buffer_TranRec_t;
typedef elbt_TranRec_t logTranRec_t;
#define buffer_nullTranId elbt_NullTranId
#define buffer_IsNullTranId(trans) elbt_IsNullTranId(trans)

#define buffer_Release elbb_Release
#define buffer_Fill elbb_Fill
#define buffer_StartTran elbb_StartTran
#define buffer_EndTran elbt_EndTran

#define elbb_ComputeMinLogSize elbl_MinLogSize
#define elbb_EndTran elbt_EndTran
#define elbb_LogClose elbb_Shutdown

/* short-hand macros */

#define buffer_Read(d,b,t,o) elbb_ReadGeneral (d,b,BUFFER_RG_READDISK,t,o)
#define elbb_CreateBlock(d,b,t,o) elbb_ReadGeneral (d,b,0/*don't read*/,t,o)
#define buffer_CreateBlock(d,b,t,o) elbb_CreateBlock(d,b,t,o)
#define elbb_TranStat(tranId) elbt_GetTranStatus(tranId)
#define buffer_TranStat(tranId) elbb_TranStat(tranId)
#define buffer_IsTranCommitted(tranId) (elbb_TranStat(tranId)==BUF_TRANCOMMIT)
#define buffer_Modify(b,o,nd,ndl,t,l,nb) elbb_Modify(b,o,nd,ndl,t,l,nb)
#define elbb_Modify(b, o, nd, ndl, t, l, nb) \
    elbb_ModifySource(b, o, nd, ndl, OSI_UIOSYS, t, l, nb)

#define buffer_EquivTran(t1,t2,dv) elbb_EquivTran(t1,t2,dv)
#define elbb_EquivTran(tran1, tran2, dv) \
do { \
    if ((tran1 != tran2) && tran1 && tran2) \
	elbt_ECMerge(tran1, tran2, dv); \
} while(0)


/* Implementation constants */

#define BUF_MINBLKSIZE (1024)  /* 1 KByte is smallest allowed size */
#define BUF_MAXBUFSIZE  (64 * BUF_MINBLKSIZE) /* largest buffer size */

#define BUFFER_OK (0)   /* specially defined error code (?) */
#define BUF_OK BUFFER_OK

#define BUF_INVALID_BLOCK (0xFFFFFFFF) /* the "invalid" disk block address */

#define LOG_EQUIVTRAN_SOFTLIMIT (30)
#define LOG_EQUIVTRAN_HARDLIMIT (100)

/* debugging symbol */
extern int afsdb_episode_logbuf;

#endif /* TRANSARC_ELB_LOGBUF_H */
