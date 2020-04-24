/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: logrec.h,v $
 * Revision 1.1.76.1  1996/10/02  17:25:11  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:26  damon]
 *
 * Revision 1.1.71.2  1994/06/09  14:01:06  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:24  annie]
 * 
 * Revision 1.1.71.1  1994/02/04  20:13:51  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:40  devsrc]
 * 
 * Revision 1.1.69.1  1993/12/07  17:19:30  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:01:51  jaffe]
 * 
 * $EndLog$
 */
/* logrec.h */

/*
 * (C) Copyright 1994, 1991 Transarc Corporation
 * All Rights Reserved
 *
 * $Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/logrec.h,v 1.1.76.1 1996/10/02 17:25:11 damon Exp $
 */

/*
 * This file contains information detailing the log record format, plus
 * macros to isolate the location of offsets within the log record itself.
 * I really don't expect anyone outside the logbuf layer to need this file.
 *
 * These may need to be changed when going to an architecture which requires
 * more strict alignment than is assumed here (i.e. all data
 * references must be 32-bit aligned.)
 * -wam 4/5/91
 */

#if !defined(TRANSARC_EPI_LOGREC_H)
#define TRANSARC_EPI_LOGREC_H

/*
 * this defines the number of pages the recovery package will verify have
 * all been correctly written to disk; the number passed by the buffer
 * package should be no smaller than this number.
 */
#define LOGREC_MAX_BUFFERS (5)

#include <dcedfs/episode/logbuf.h>

#if defined(AFS_AIX31_ENV) && defined(KERNEL)
#define AFS_PIN 1
#endif /* AFS_AIX31_ENV  && KERNEL */

/* log page header layout */
/* log record header layout */

struct logPageHdr {
    u_int16     passNumber;		/* pass # through the log */
    u_int16     lastLogRecOff;		/* offset to last log record on pg */
    u_int32     pageNumber;		/* log page # */
    u_int32     salvageID;		/* magic identifier for the salvager */
    u_int16     oldestDataPage;		/* offset in log (unwritten bufs) */
    u_int16     oldestTranPage;		/* offset in log (uncommitted trans) */
    u_int32     timeWritten;		/* when page scheduled to disk */
    u_int32     reserved[3];		/* reserved fields */
    u_int16     io_window;		/* # log pages in transit before this
					 * page was written */
    unsigned char cksum_ver;		/* checksum version */
    unsigned char cksum_len;		/* checksum length in words */
					/* variable length checksum */
};

#define LOGREC_INITIAL_LOGPAGE_PASSNO (0xfffffffc)
/* Macros to generate legal succeeding or preceding log page Pass Numbers */
#define IncPassno(passno) \
    ((u_int16)((((u_int16)(((u_int16)(passno))+1))==0) ? \
	       (((u_int16)(passno))+2) : (((u_int16)(passno))+1)))

#define DecPassno(passno) \
    ((u_int16)((((u_int16)(((u_int16)(passno))-1))==0) ? \
	       (((u_int16)(passno))-2) : (((u_int16)(passno))-1)))

/*
 * Checksum Version Flags
 */
/* For backwards compatibility with code that never recorded checksum
 * length on disk */
#define LOGREC_BACK_COMPAT_CKSUM_LEN (0x0)

#define LOGREC_CKSUM_MAX_LEN (0x100)

#define LOGREC_CKSUMVER_NONE (0x0)
#define LOGREC_CKSUMVER_NONE_LEN (0x0)

/* CRC 32 types */
#define LOGREC_CKSUMVER_CRC32_STD_ALL (0x01)
#define LOGREC_CKSUMVER_CRC32_STD_SPARSE (0x2) /* for future use. */

#define LOGREC_CKSUMVER_CRC32_LEN (0x4) /* bytes */
#define LOGREC_CKSUMVER_CRC32_STD_ALL_LEN      LOGREC_CKSUMVER_CRC32_LEN
#define LOGREC_CKSUMVER_CRC32_STD_SPARSE_LEN   LOGREC_CKSUMVER_CRC32_LEN

#define LOGREC_CKSUMVER_DEFAULT LOGREC_CKSUMVER_CRC32_STD_ALL

/*
 * log record type field declarations;  the upper two bits are used to
 * signal if the old data or new data areas are valid.  If both are
 * valid, they must be of equal size.
 */
#define LOGREC_OLDDATA (0x8000) /* the old data section is valid */
#define LOGREC_NEWDATA (0x4000) /* the new data section is valid */

#define LOGREC_DEBUG   (0x0000)                /* informational record */
#define LOGREC_START   (LOGREC_DEBUG|LOGREC_OLDDATA) /* has a string */
#define LOGREC_INTERNAL (LOGREC_DEBUG|LOGREC_NEWDATA) /* has binary data */
#define LOGREC_BASE_DATA (0x0037)              /* base data type record */
#define LOGREC_FLUSH   (0x019F)                /* buffer write record */
#define LOGREC_BEND    (0x0D63|LOGREC_OLDDATA) /* batch end transactions */
#define LOGREC_END     (0x24D0)                /* end a transaction */
#define LOGREC_CKPT    (0x2781)		       /* checkpoint record */
#define LOGREC_RSTRT   (0x3F0E)                /* restart record */
#define LOGREC_BASE_FILL (0x1A7D)              /* base fill type */

/* compound types */

/* data record types */
#define LOGREC_DATA    (LOGREC_BASE_DATA|LOGREC_OLDDATA|LOGREC_NEWDATA)
#define LOGREC_ODATA   (LOGREC_BASE_DATA|LOGREC_OLDDATA)
#define LOGREC_NDATA   (LOGREC_BASE_DATA|LOGREC_NEWDATA)

/* fill record types */
#define LOGREC_NFILL   (LOGREC_BASE_FILL|LOGREC_NEWDATA)
#define LOGREC_OFILL   (LOGREC_BASE_FILL|LOGREC_OLDDATA)
#define LOGREC_FILL    (LOGREC_BASE_FILL|LOGREC_OLDDATA|LOGREC_NEWDATA)

/* macros for checking record types */
#define LOGREC_HASOLDDATA(type) ((type)&LOGREC_OLDDATA)
#define LOGREC_HASNEWDATA(type) ((type)&LOGREC_NEWDATA)
#define LOGREC_HASDATA(type) ((type)&(LOGREC_OLDDATA|LOGREC_NEWDATA))

#define LOGREC_TYPE_MASK (~(LOGREC_OLDDATA|LOGREC_NEWDATA))
#define LOGREC_BASE_TYPE(type) ((type)&LOGREC_TYPE_MASK)

#define LOGREC_ISDATA(type) (LOGREC_BASE_TYPE(type) == LOGREC_BASE_DATA)
#define LOGREC_ISFILL(type) (LOGREC_BASE_TYPE(type) == LOGREC_BASE_FILL)
#define LOGREC_ISBEND(type) \
    (LOGREC_BASE_TYPE(type) == LOGREC_BASE_TYPE(LOGREC_BEND))
#define LOGREC_ISDEBUG(type) (LOGREC_BASE_TYPE(type) == LOGREC_DEBUG)
#define LOGREC_ISEND(type) (LOGREC_BASE_TYPE(type) == LOGREC_END)
#define LOGREC_HASSTATUS(type) (LOGREC_BASE_TYPE(type) != LOGREC_CKPT)

#define LOGREC_ISRECOVERABLE(type) (((type) == LOGREC_DATA) || \
				    ((type) == LOGREC_ODATA) || \
				    ((type) == LOGREC_NDATA) || \
				    ((type) == LOGREC_OFILL) || \
				    ((type) == LOGREC_NFILL))

#define LOGREC_ISVALIDTYPE(type) (((type) == LOGREC_DEBUG) || \
				  ((type) == LOGREC_INTERNAL) || \
				  LOGREC_ISDATA(type) || \
				  ((type) == LOGREC_FLUSH) || \
				  ((type) == LOGREC_BEND) || \
				  ((type) == LOGREC_END) || \
				  ((type) == LOGREC_CKPT) || \
				  ((type) == LOGREC_RSTRT) || \
				  ((type) == LOGREC_NFILL) || \
				  ((type) == LOGREC_OFILL) || \
				  ((type) == LOGREC_FLUSH))

#define LOGREC_ISKILL(logRec) \
    ((LOGREC_OFILL == (logRec)->hdr.recType) && \
     (*((unsigned char *)logRec + OldDataOffset(logRec)) == '\0'))

/*
 * this macro is used to determine if this is a "splittable" type of
 * record.  The only type of record which can be split currently
 * is a data record.
 */
#define LOGREC_SPLIT(type) (LOGREC_ISDATA(type))
#define LOGREC_NOSPLIT(type) !LOGREC_SPLIT(type)

/*****************************************************************
 * finished with log record types
 ****************************************************************/

/*
 * a log record consists of four parts: part (1) is this header; part (2)
 * is the old data; part (3) is the new data; part (4) is the log sequence
 * number.
 *
 */
struct logRecHdr {
    u_int16     recType;        /* record type */
    u_int16     refCount;       /* reference count */
    u_int32     dataLength;     /* data length */
    u_int32     tranId;         /* transaction ID */
    u_int32     deviceHandle;   /* device descriptor */
    daddr_t     dataAddr;       /* disk address of modified data */
    u_int16     dataOffset;     /* offset in disk block to mod. data */
    u_int16     prevLogRecOff;  /* offset to previous log record */
    u_int32     spare[2];       /* reserve spare fields */
};

/* in core version */
struct logRec {
    struct logRecHdr  hdr;      /* header portion */
    caddr_t           oldData;
    caddr_t           newData;
    u_int32           lsn;
};

/*
 * These macros are used to describe to both the log and recovery
 * packages how to find the data within a log page.  This is necessary
 * because we wish to align data structures so we can simply cast
 * pointers rather than bcopy (remember we want things to be fast.)
 */

/*
 * note that this alignment mechanism presumes the alignment is
 * a power-of-two. The use of (unsigned) may not be strictly correct
 * here!
 */
#ifndef ALIGN_SIZE
#define ALIGN_SIZE 4
#endif /* ALIGN_SIZE */
#define ALIGN(t,x) (t)((unsigned)((x) + (ALIGN_SIZE-1)) & (unsigned)(~(ALIGN_SIZE-1)))


/*
 * This first set is used to compute the offset to the beginning of
 * the data structure (using full-alignment rules)
 *
 * XXX: optimize this for the ALIGN_SIZE 4 case by simply doing the
 * operations directly.  Check this code *first* however.
 */


#define OldDataSize(type, dataLength) \
    (LOGREC_HASOLDDATA(type) ? (dataLength) : 0)

#define NewDataSize(type, dataLength) \
    (LOGREC_HASNEWDATA(type) ? (dataLength) : 0)

#define OldFillSize(type, dataLength) \
    (LOGREC_HASOLDDATA(type) ? ALIGN_SIZE : 0)

#define NewFillSize(type, dataLength) \
    (LOGREC_HASNEWDATA(type) ? ALIGN_SIZE : 0)


#define PageHdrLength() ALIGN(unsigned, sizeof(struct logPageHdr))

/* note the special case at the end to handle BEND code */
#define OldDataLength(logRec) \
(LOGREC_ISFILL((logRec)->hdr.recType) ? \
 OldFillSize((logRec)->hdr.recType, (logRec)->hdr.dataLength) : \
 OldDataSize((logRec)->hdr.recType, (logRec)->hdr.dataLength))

#define NewDataLength(logRec) \
(LOGREC_ISFILL((logRec)->hdr.recType) ? \
 NewFillSize((logRec)->hdr.recType, (logRec)->hdr.dataLength) : \
 NewDataSize((logRec)->hdr.recType, (logRec)->hdr.dataLength))

#define DataLength(logRec) (ALIGN(unsigned, (OldDataLength(logRec) + \
					     NewDataLength(logRec))))
#define OldDataOffset(logRec) ALIGN(unsigned,sizeof(struct logRecHdr))
#define NewDataOffset(logRec) (OldDataOffset(logRec) + OldDataLength(logRec))
#define LSNOffset(logRec) (sizeof(struct logRecHdr) + DataLength(logRec))
#define NextRecOffset(logRec) (LSNOffset(logRec) + sizeof(u_int32))

#define NewLogrecSize(t, l) \
(sizeof(struct logRecHdr) + sizeof(u_int32) + \
 (LOGREC_ISFILL(t) ? (OldFillSize(t, l) + NewFillSize(t,l)) : \
 (LOGREC_HASDATA(t) ? (OldDataSize(t, l) + NewDataSize(t,l)) : 0)))

/*
 * These are now access macros. Offset always refers to the beginning of
 * the log record.
 */
#define GetLogrec(buf, offset) (struct logRec *)((buf) + (offset))
#define GetLogrecHdr(buf, offset) (struct logRecHdr *)((buf) + (offset))
#define GetLogrecOldData(buf, offset) ((char *)GetLogrecHdr(buf, offset) + OldDataOffset(GetLogrecHdr(buf, offset)))
#define GetLogrecNewData(buf, offset) (char *)(GetLogrecHdr(buf, offset) + NewDataOffset(((struct logRec *)GetLogrecHdr(buf, offset))))
#define GetLogrecLSN(buf, offset) (*(u_int32 *)((char *)(buf) + (unsigned)(offset) + LSNOffset((GetLogrec(buf,offset)))))
#define GetNextLogrec(buf, offset) GetLogrecHdr(buf, offset + NextRecOffset(GetLogrecHdr(buf, offset)))
#define GetPageHdr(buf) (struct logPageHdr *)(buf)

/*
 * These are the copy macros.  We do a tremendous number of small
 * modifications and it is much cheaper to simply do direct assignments
 * for that class of modifications.  Unfortunately, in the case where
 * the data might be unaligned, alignment must be checked first.
 */

typedef struct {
    char hbytes[8];
} hbytes;

#define AssignOK(addr) (!((unsigned)(addr) & (ALIGN_SIZE-1)))

#define COPY(src,dest,type) (*(type)(dest) = *(type)(src))
#define COPY1(src,dest) COPY(src,dest,char *)
#define COPY2(src,dest) COPY(src,dest,u_int16 *)
#define COPY4(src,dest) COPY(src,dest,u_int32 *)
#define COPY8(src,dest) COPY(src,dest,hbytes *)

#define LOGCOPY_ALIGNED(src,dest,len) \
do {  \
    switch(len) { \
      case 2: COPY2(src,dest); break; \
      case 4: COPY4(src,dest); break; \
      case 8: COPY8(src,dest); break; \
      default: (void)bcopy(src,dest,len); break; \
    } \
} while(0)

#define LOGCOPY_ALIGNED2(src,dest,len) \
do {  \
    switch(len) { \
      case 1: COPY1(src,dest); break; \
      case 2: COPY2(src,dest); break; \
      case 4: COPY4(src,dest); break; \
      case 8: COPY8(src,dest); break; \
      default: (void)bcopy(src,dest,len); break; \
    } \
} while(0)

#define LOGCOPY(src,dest,len) \
    do { if (len == 1) { COPY1(src,dest); break; } \
	 if(AssignOK(dest) && AssignOK(src)) \
	     LOGCOPY_ALIGNED(src,dest,len); \
	 else bcopy(src,dest,len); \
     } while(0)

#ifdef AFS_HAS_MEMSET
#define LOGFILL(s, c, n) memset(s, c, n)
#else
#define LOGFILL(s, c, n) \
do {  \
    unsigned i; \
    unsigned char *cp = (unsigned char *)(s); \
    for(i = 0; i < (n); i++) \
	*cp++ = (unsigned char)(c); \
} while(0)
#endif /* AFS_HAS_MEMSET */

#ifdef AFS_PIN

#define logbuf_pin(x, s) do { \
int code; \
code = pin((uint)x, (int)s); \
afsl_MBZ(code); \
} while(0)

#define logbuf_unpin(x, s) (unpin((uint)x, (int)s))

#else

#define logbuf_pin(x, s) /* nothing */
#define logbuf_unpin(x, s) /* nothing */

#endif /* AFS_PIN */


/*
 * Log page checksumming information, etc.
 */

/* first, the symbolic constants - these are positions in the jump table */
#define LOGREC_NOCKSUM (0)

#define LOGREC_MAXNCKSUMS 1

/* now, the symbolic lengths - these should be defined even if the checksum
 * algorithm is not implemented for some reason; this allows us to skip over
 * checksums, if we so desire.
 */
#define LOGREC_NOCKSUM_LEN (0)

/*
 * The checksum algorithms are just an entry in a jump table.
 */
/* extern (*void[LOGREC_MAXNCKSUMS])(); */

/*
 * The default checksum
 */
#ifndef LOGREC_DEFAULT_CKSUM
#define LOGREC_DEFAULT_CKSUM LOGREC_NOCKSUM_LEN
#endif /* LOGREC_DEFAULT_CKSUM */

/* ComparePosition -- compares to log positions and returns a positive number
 *     if the first position is ahead of (newer) than the second.  It returns
 *     zero if they refer to the same log page, and negative otherwise.  The
 *     magnitude of the number is insignificant. */

#define ComparePosition(pass1,off1, pass2,off2) \
    ((int16)((pass1) - (pass2)) * 0x10000 + (/*signed*/int16)((off1) - (off2)))

/*
 * Function prototypes - shared between buffer, log & tran
 */

extern void elbl_Init (void);
extern long elbl_InitLog _TAKES((struct async_device *,
  struct async_device *, daddr_t *,
  u_int32, struct buffer **, u_int32,
  long (*) _TAKES((struct async_device *))));
extern long elbl_Create _TAKES((struct async_device *, const daddr_t *,
				 unsigned, unsigned char));
extern long elbl_Write _TAKES((struct elbt_TranRec *, unsigned, u_int32,
				const caddr_t));
extern long elbl_WriteExtended _TAKES((IN struct elbt_TranRec *, IN unsigned,
				       IN u_int32, IN daddr_t, IN u_int32,
				       IN const caddr_t oldData,
				       IN const caddr_t newData,
				       OUT u_long *));
extern long elbl_WriteWithBuffer _TAKES((struct elbt_TranRec *, unsigned,
					  u_int32, u_int32, struct buffer *,
					  const caddr_t, u_long *));
extern void elbl_Shutdown _TAKES((struct async_device *));
extern void elbl_Close _TAKES((struct async_device *, struct fifo *,
			       void (*const)(struct buffer *, int)));
extern void elbl_FullInfo _TAKES((struct async_device *, unsigned,
			          OUT u_long *ageQSize,
				  OUT struct buffer **));
extern void elbl_GetLogPageOff _TAKES((struct async_device *, u_int16 *,
				       u_int16 *));
extern void elbl_UnageBuffer _TAKES((IN struct buffer *bP, IN int force));
#define elbl_FirstAgedBuffer(dP,bPP) elbl_NextAgedBuffer(dP, 0, bPP)
extern void elbl_NextAgedBuffer _TAKES((struct async_device *,
					struct buffer *,
					OUT struct buffer **));
extern void elbl_PendTran(IN struct elbt_TranRec *tP);
extern int elbl_IsTranCommitted(IN struct elbt_TranRec *tP);
extern void elbl_ForceTran(IN struct elbt_TranRec *tP);
extern void elbl_ForceBuffer(IN struct buffer *bP);
extern int elbl_BufferCommitCost (IN struct buffer *bP);

/* used between tran & log */

/* Bits used in GetSomeTran calls.  The first four bits are multually exclusive
 * classes of transactions.  The nested and unnested states are only
 * appropriate for active or pending transactions.  These two bits are mutually
 * exclusive and setting neither means "don't care". */
#define ELBL_TRAN_ACTIVE	0x01
#define ELBL_TRAN_PENDING	0x02
#define ELBL_TRAN_COMPLETED	0x04
#define ELBL_TRAN_COMMITTED	0x08
#define ELBL_TRAN_NESTED	0x10	/* i.e. an undo zero tran */
#define ELBL_TRAN_UNNESTED	0x20

/* age values for GetSomeTran.  Age refers to the start time. */
#define ELBL_TRAN_OLDEST 0
#define ELBL_TRAN_NEWEST 1
#define ELBL_TRAN_ANY 2

extern int elbl_StartTran _TAKES((struct async_device *,
				   struct elbt_TranRec *, unsigned));
extern void elbl_DeleteTran _TAKES((struct elbt_TranRec *));
extern void elbl_CompleteTran _TAKES((struct elbt_TranRec *, unsigned,
				      struct elbt_TranRec *));
extern void elbl_GCTran _TAKES((struct async_device *, int, struct fifo *));
extern void elbl_WriteBend _TAKES((struct elbt_TranRec *,
				   struct elbt_TranRec *));
extern struct elbt_TranRec *elbl_GetSomeTran(
  IN struct async_device *dP,
  IN int age,
  IN unsigned which,
  OUT elbt_TranRec_t *tidP);
#define elbl_TranActivity(dP,which) \
      (elbl_GetSomeTran (dP, ELBL_TRAN_ANY, which, NULL) != 0)
extern void elbl_WaitForAnyTran _TAKES((struct async_device *));

extern int elbl_LogIsLocked(void);

#endif /* TRANSARC_EPI_LOGREC_H */
