/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: recover.c,v $
 * Revision 1.1.69.1  1996/10/02  17:25:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:41  damon]
 *
 * Revision 1.1.64.3  1994/07/13  22:20:18  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:47  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  20:12:48  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:57:07  mbs]
 * 
 * Revision 1.1.64.2  1994/06/09  14:01:21  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:35  annie]
 * 
 * Revision 1.1.64.2  1994/06/09  14:01:21  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:35  annie]
 * 
 * Revision 1.1.64.1  1994/02/04  20:13:59  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:43  devsrc]
 * 
 * Revision 1.1.62.1  1993/12/07  17:19:36  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:02:31  jaffe]
 * 
 * $EndLog$
 */
/* recover.c */

/*
 * Copyright (C) 1994, 1990 Transarc Corporation - All Rights Reserved
 */

/*
 * Based upon the Episode Buffer/Log/Recovery Specification and Design.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/lock.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/episode/async.h>
#include <elb.h> /* includes fifo.h */
#include <logrec.h>
#include <recover.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/recover.c,v 1.1.69.1 1996/10/02 17:25:27 damon Exp $")

int defaultLogChecksumVersion = LOGREC_CKSUMVER_DEFAULT;

int afsdb_episode_recover = 0;

#define DEBUG_THIS_FILE LOGBUF_DEBUG_RECOVER
#define VDBprint(p) \
    dprintf(afsdb_episode_recover, p)
#define DBprint(p) \
    dprintf(afsdb_episode_recover, p)

/* For sanity checks below. */
#if defined(KERNEL)
#define MAX_ALLOC_SIZE (BUF_MAXBUFSIZE + 1)
#else
#define MAX_ALLOC_SIZE ((64 * 1024) + 1)
#endif

/*
 * this package is one from which unwinding and failing gracefully is
 * difficult (but necessary) because this is the *wrong* layer from which
 * to call panic.  To this end, I'm adopting the following (ugly) convention:
 *
 * Internal functions which allocate memory take an allocList parameter;
 * this is a linked list of memory which must be deallocated upon cleanup.
 *
 * Internal functions do not return error codes; rather they raise an
 * "exception".
 *
 * I don't like it, but I like crashes during recovery much less!
 * -wam 5/2/91
 *
 * Locking is no longer done in this package; all code should be thread-safe
 * (multiple recoveries may occur simultaneously).
 */
#ifdef KERNEL

typedef label_t *recvr_jmp_buf;
#define JMP_BUF_DECL label_t recvr_env_storage; \
                     recvr_jmp_buf rcvr_env = (recvr_jmp_buf)&recvr_env_storage

#ifdef AFS_AIX31_ENV

/*
 * The AIX kernel has a somewhat different setjmp/longjmp model.
 */
#define RECVR_CATCH() setjmpx(rcvr_env)
#define RECVR_CLEAR() clrjmpx(rcvr_env)
#define RECVR_RAISE(code) longjmpx(code)

#else /* AFS_AIX31_ENV */
/* SunOS? */

#include <dce/ker/exc_handling.h>

/*
 * This assumes traditional semantics for an in-kernel setjmp/longjmp.
 */
#define RECVR_CATCH() dce_exc_setjmp(rcvr_env)
#define RECVR_CLEAR()
#define RECVR_RAISE(code) dce_exc_longjmp(rcvr_env, code)

#endif /* AFS_AIX31_ENV */

#else /* KERNEL */

/*
 * User Space.
 */
#include <setjmp.h>

typedef jmp_buf recvr_jmp_buf;
#define JMP_BUF_DECL recvr_jmp_buf rcvr_env

#define RECVR_CATCH() setjmp(rcvr_env)
#define RECVR_CLEAR()
#define RECVR_RAISE(code) do { DBprint(("# longjmp %d", code)); longjmp(rcvr_env, code); } while (0)

#endif  /* KERNEL */

/*
 * Here are some macros for verifying consistency; in case of failure they
 * perform a RECVR_RAISE.
 */

#define PTR_ALIGNED(ptr) \
    if ((unsigned)(ptr) & (unsigned)(3)) RECVR_RAISE(RCVR_ALIGN)
#define PTR_OK(ptr) if (!(ptr)) RECVR_RAISE(RCVR_NULLPTR)
#define PTR_AOK(ptr) do { PTR_OK(ptr); PTR_ALIGNED(ptr);} while(0)
#define RECVR_ASSERT(x) if (!(x)) RECVR_RAISE(RCVR_ASSERT)

/* VALID_SIZE -- panics on bogus size.  Uses assert instead of RAISE since is
 *     called where the latter is hard.  The error is pretty gross anyway. */

#define VALID_SIZE(s) afsl_Assert((s) < MAX_ALLOC_SIZE)

#ifndef KERNEL
/* Code for user space only */

#define RECOVER_HASHMAPSIZE (128)
#define recvrHash(tranNum) ((tranNum) % RECOVER_HASHMAPSIZE)

struct recvrAllocInfo {
    caddr_t buf;
    unsigned size;
    struct recvrAllocInfo * next;
};

#define ADD_TO_ALLOC_LIST(newMem, memSize, list) \
do { \
     struct recvrAllocInfo * volatile aiP; \
     VALID_SIZE(memSize); \
     aiP = (struct recvrAllocInfo * volatile )osi_Alloc_r(sizeof(struct recvrAllocInfo)); \
     aiP->buf = (caddr_t)newMem; aiP->size = memSize; aiP->next = list; \
     list = aiP; \
} while(0)

#define RECVR_ALLOC(ptr, type, size, list) \
do { \
    VALID_SIZE(size); \
    ptr = (type)osi_Alloc_r(size); \
    logbuf_pin(ptr, size); \
    if (list) ADD_TO_ALLOC_LIST((ptr), (size), (list));\
} while(0)

#define RECVR_CLEANUP(allocList) Cleanup(allocList)
#define readBlock(buffer, retval) blockOp(buffer, &retval, B_READ)
#define writeBlock(buffer, retval) blockOp(buffer, &retval, B_WRITE)

#if !defined(min)
#define min(a, b)                 (a>b ? b : a)
#endif /* min */

#if defined(panic)
#undef panic
#endif /* defined(panic) */
#define panic(str) do { fprintf(stderr, "%s\n", str); abort(); } while (0)

#define DATASIZE_OK(size, devP)  do { \
    PTR_AOK(devP);  VALID_SIZE(devP->blkSize); \
    if (size > devP->blkSize) RECVR_RAISE(RCVR_BADREF); } while (0)

#define OFFSET_OK(locn, offset, devP) DATASIZE_OK(locn+offset, devP)
#define LOGREC_TYPE_OK(logrec) \
do { \
    PTR_AOK(logrec); \
    if (!LOGREC_ISVALIDTYPE(logrec0>hdr.recType)) \
	RECVR_RAISE(RCVR_BADREC) \
} while (0)

#define LOGPAGE_OK(pageHdr) \
do { \
    PTR_AOK(pageHdr); \
    if (pageHdr->salvageID != LOG_SALVAGE_MAGIC_NUMBER) \
	RECVR_RAISE(RCVR_BADLOG); \
} while (0)

#define RECVR_ASSERT(x) if (!(x)) RECVR_RAISE(RCVR_ASSERT)

#define DISKTHREAD offsetof(struct recvrRec, diskThread)
#define DISKADDRTHREAD offsetof(struct recvrRec, diskAddrThread)

/*
 * these are numbers which are written by the recovery process each time
 * recovery succeeds.  They can be read to obtain information about how
 * much work recovery had to do.  Note they are not locked; the values
 * are really only useful if a single recovery is done
 */
/* EXPORT */
unsigned elbr_lastTime;  /* amount of elapsed time required */
/* EXPORT */
unsigned elbr_lastNrec;  /* # of records processed */
/* EXPORT */
unsigned elbr_lastNpage; /* # of log pages used */
/* EXPORT */
unsigned elbr_lastNdata; /* # of different data blocks modified */
/* EXPORT */
unsigned elbr_lastRedoData;  /* # of redo data operations */
/* EXPORT */
unsigned elbr_lastRedoFill;  /* # of redo fill operations */
/* EXPORT */
unsigned elbr_lastUndoData;  /* # of undo data operations */
/* EXPORT */
unsigned elbr_lastUndoFill;  /* # of undo fill operations */
/* EXPORT */
unsigned elbr_lastNECs;      /* # of ECs seen */
/* EXPORT */
unsigned elbr_lastLargestEC; /* # of transactions in largest EC */

#endif /* !KERNEL */

/* Checksum related declarations */
#define UNKNOWN_CKSUM_TYPE  (1)
#define BAD_CKSUM_LEN (2)

/* Check sum lengths in bytes. Array indexed by checksum version type */
static int cksumLen[3] =
{ /*LOGREC_CKSUMVER_NONE*/              LOGREC_CKSUMVER_NONE_LEN,
  /*LOGREC_CKSUMVER_CRC32_STD_ALL*/     LOGREC_CKSUMVER_CRC32_STD_ALL_LEN,
  /*LOGREC_CKSUMVER_CRC32_STD_SPARSE */ LOGREC_CKSUMVER_CRC32_STD_SPARSE_LEN
};

#define KnownChecksumType(/* unsigned char */ cksumVer) \
    (((unsigned char)(cksumVer)) < (sizeof(cksumLen)/sizeof(cksumLen[0])))

#define StoreLogPageCksum(/* char* */logPage, /* char* */ cksum, \
			  /* int */ cksumLength)                    \
    bcopy((cksum), (char *)((logPage)+sizeof(struct logPageHdr)), (cksumLength))

#define ZeroLogPageChecksum(/* char* */ logPage, /* int */ cksumLength) \
    bzero((char *)((logPage)+sizeof(struct logPageHdr)), (cksumLength))

/* forward references */
#ifndef KERNEL
static void blockOp _TAKES((struct buffer *, unsigned *, unsigned));
static void readData _TAKES((struct async_device *, daddr_t, caddr_t));
static void writeData _TAKES((struct async_device *, daddr_t, caddr_t));
static void readPage _TAKES((struct async_device *, daddr_t,
				   struct logPageHdr *));
static void AddToTranHash _TAKES((struct fifo *, struct recvrRec *,
				  recvr_jmp_buf));
static void AddToDiskHash _TAKES((struct fifo *,
					struct recvrRec *, recvr_jmp_buf));
static int32 HandleBulkEnd _TAKES((struct recvrRec *,
					 struct fifo *, recvr_jmp_buf));
static void ReInitList _TAKES((struct recvrRec *, recvr_jmp_buf));
static void MarkCommitted _TAKES((struct fifo *, int, recvr_jmp_buf));
static void AdjustDisk _TAKES((struct fifo *, struct async_device *,
				struct recvrAllocInfo * volatile *, recvr_jmp_buf));
static long parsePage _TAKES((char *, struct recvrRec **,
				    struct recvrAllocInfo * volatile *, recvr_jmp_buf));
static u_int32 SafeDumpLog _TAKES((struct async_device *, daddr_t *,
					 unsigned, recvr_jmp_buf,
					 struct recvrRec **, unsigned *,
					 struct logPageHdr **,
					 struct recvrAllocInfo * volatile *));
static u_int32 SafeDumpEntireLog _TAKES((struct async_device *, daddr_t *,
				         unsigned, recvr_jmp_buf,
				         struct recvrRec **, unsigned *,
				         struct logPageHdr **,
					 struct recvrAllocInfo * volatile *));

static u_int32 SafeDumpLogRecords _TAKES((caddr_t *, int, recvr_jmp_buf,
				          struct recvrRec **, unsigned *,
				          struct logPageHdr **,
				          struct recvrAllocInfo * volatile *));

static void Cleanup _TAKES((volatile struct recvrAllocInfo *));
static void VerifyRecordList _TAKES((struct recvrRec *, recvr_jmp_buf));
static void VerifyRecord _TAKES((struct recvrRec *, recvr_jmp_buf));
static void Redo _TAKES((struct recvrRec *, caddr_t, recvr_jmp_buf));
static void Undo _TAKES((struct recvrRec *, caddr_t, recvr_jmp_buf));

#endif /* !KERNEL */

static int GetChecksumLength _TAKES((int));
static void CalcChecksum _TAKES((char *, long, unsigned int, char *, int *));
static int RetrieveLogPageChecksum _TAKES((char *, char *, unsigned char *,
					   int *));
static int CalcLogPageChecksum _TAKES((char *, long, char *, int *));
static int SameChecksum _TAKES((char *, char *, int));
static int ValidLogPageChecksum _TAKES((char *, long, unsigned char *,
					unsigned char *));

/* Functions */
#ifndef KERNEL

/*
 * Recovery is a straight-forward process here:
 *
 * 1) we first obtain a list of all log records.
 *
 * 2) we organize the records into lists of related transactions and
 *    related disk modifications.
 *
 * 3) we sort the transaction sequences into two types: complete or
 *    incomplete.
 *
 * 4) we begin traversing the list of transactions, by data block number
 *    modified.  If there are any modifications for a particular data
 *    block, we first read it and then compute the modifications by
 *    cycling through all transactions modifying that block in time-sorted
 *    order.  Once modifications are computed we write the block back to
 *    disk.
 *
 *  N.B: in this incarnation (a kernel-based recovery system) the
 *  WriteBlocks() interface is not used.  In a mixed kernel/user
 *  space implementation it would be used.
 *
 * This code must run single-threaded in user space;  it runs multi-threaded
 * in kernel space (we must attach to the thread save context to allow
 * unwinding in case of errors.)
 */

static int recoverInitialized = 0;

static void Init()
{

      /* reset statistics variables  - every time! */
      elbr_lastTime = elbr_lastNrec = elbr_lastNpage = elbr_lastNdata =
	  elbr_lastRedoData = elbr_lastRedoFill = elbr_lastUndoData =
	      elbr_lastUndoFill = elbr_lastNECs = elbr_lastLargestEC = 0;


      /* one-time recovery initializaion - if any! */
      if (recoverInitialized)
	  return;

      recoverInitialized = 1;

}

/*
 * Recover - perform recovery calculations on specified device.
 *
 * This routine must be *very* paranoid about failure.  It is better
 * to return an error than to panic.  Indeed, in the end other than
 * the basic firewalls at the beginning there should be no panic paths
 * out of this code.
 */
/* EXPORT */
u_int32 elbr_Recover(
      IN struct async_device *ldP,
      IN struct async_device *dP,
      IN daddr_t *logLocnArray,
      IN unsigned logLocnArraySize)
{

      struct recvrRec *recvrRecList;
      unsigned recvrRecCount;
      struct fifo *tranHash, *diskHash;
      struct logRec ckpt, restart;
      unsigned passNum, pgNum;
      struct logPageHdr *lastPageHdr = 0;
      u_int32 code;
      struct recvrAllocInfo * volatile allocList = (struct recvrAllocInfo *)0;
      JMP_BUF_DECL;
      struct timeval startTime, endTime;

      /* firewalls */
      if ( !ldP || !dP || !logLocnArray || (logLocnArraySize == 0))
	  return RCVR_INVALID;

      Init();

      osi_GetTime(&startTime);


      if ((code = RECVR_CATCH())) {
	  /*
	   * this code path is our unwind case.  It would be nice if this
	   * path included the memory allocation failure problem so we
	   * could branch to a non-all-in-memory code path.  In either case,
	   * however we must clean up memory.
	   */

	  RECVR_CLEANUP(allocList);

	  /* code == 1 is the default longjmp case, which is
	   * meaningless in this context.  I'm not sure if longjmp in the
	   * kernel works right (ergo all this mumbo-jumbo).
	   */
	  return (code == 1) ?  RCVR_FAILED : code;
      }

      DBprint(("# elbr_Recover: beginning recover"));

      /* Step 0: allocate & initialize data areas */
      RECVR_ALLOC(tranHash, struct fifo *, RECOVER_HASHMAPSIZE *
		  sizeof(struct fifo), allocList);
      RECVR_ALLOC(diskHash, struct fifo * volatile, RECOVER_HASHMAPSIZE *
		  sizeof(struct fifo), allocList);


      /* Step 1: obtain list of log records (transactions) */
      code = SafeDumpLog(ldP, logLocnArray, logLocnArraySize,
			       rcvr_env, &recvrRecList,
			       &recvrRecCount, &lastPageHdr, &allocList);
      if (code != BUFFER_OK)
	  panic("elbr_Recover: SafeDumpLog failed");

      RECVR_ASSERT((recvrRecCount != 0) && lastPageHdr);
      RECVR_ASSERT(recvrRecList);
      RECVR_ASSERT(AssignOK(recvrRecList));
      if (afsdb_episode_recover > 0)
	  VerifyRecordList(recvrRecList, rcvr_env);

      /* Step 2: organize log records into list of associated transactions. */

      AddToTranHash(tranHash, recvrRecList, rcvr_env);

      /*
       * This routine walks the entire list and figures out if a particular
       * transaction is committed.  Each record is marked as either REDO, UNDO
       * or UNDEF.  We resolve UNDEFs in a second call to MarkCommitted after
       * handling bend records.
       */
      MarkCommitted(tranHash, 1/*undef okay*/, rcvr_env);
      if (afsdb_episode_recover > 0)
	  VerifyRecordList(recvrRecList, rcvr_env);

      /*
       * This handles the ugly case - the bulk-end transactions we use
       * to defer the commits of EC transactions.
       */
      HandleBulkEnd(recvrRecList, tranHash, rcvr_env);
      if (afsdb_episode_recover > 0)
	  VerifyRecordList(recvrRecList, rcvr_env);

      /*
       * Once the bulk end is done, we must remove all the
       * entries from the tran and disk hash tables and re-hash
       * because the correct locations have really been changed.
       */
      ReInitList(recvrRecList, rcvr_env);
      if (afsdb_episode_recover > 0)
	  VerifyRecordList(recvrRecList, rcvr_env);

      AddToTranHash(tranHash, recvrRecList, rcvr_env);
      AddToDiskHash(diskHash, recvrRecList, rcvr_env);

      /*
       * We must traverse the transaction list again and mark all
       * committed transactions.
       */
      MarkCommitted(tranHash, 0/*no undefs*/, rcvr_env);
      if (afsdb_episode_recover > 0)
	  VerifyRecordList(recvrRecList, rcvr_env);

      /*
       * Step 4: compute changes to disk
       *
       * This routine traverses the disk hash to find all entries which
       * must be applied/unapplied.
       */
      AdjustDisk(diskHash, dP, &allocList, rcvr_env);

      /* Step 5: recovery is complete.  Write out a clean restart record. */
      DBprint(("# elbr_Recover: Step 5"));

      /*
       * TODO: incorporate more information from the rest of the log in
       *       here...
       *
       */

      /* build the checkpoint record */
      elbr_InitLogRecHdr( &(ckpt.hdr) );
      ckpt.hdr.recType = LOGREC_CKPT;         /* = LOGREC_DEBUG ? */
      ckpt.oldData = (caddr_t)0;
      ckpt.newData = (caddr_t)0;
      ckpt.lsn = 1;

      passNum = lastPageHdr->passNumber;
      pgNum = lastPageHdr->pageNumber + 1;
      if (pgNum >= logLocnArraySize) {
	  pgNum %= logLocnArraySize;
	  passNum = IncPassno(passNum);
      }

      code = elbr_WriteCheckpointBlock(ldP, logLocnArray[pgNum], &ckpt,
				       passNum, pgNum,
				       lastPageHdr->cksum_ver);
      RECVR_ASSERT(code == 0);

      /* build the restart record */
      elbr_InitLogRecHdr( &(restart.hdr) );
      restart.hdr.recType = LOGREC_RSTRT;
      restart.oldData = (caddr_t)0;
      restart.newData = (caddr_t)0;
      restart.lsn = 1;

      /*
       * we increase the pass # to allow for an Episode crash immediately
       * after restarting the log; incrementing by 2 is just an extra bit
       * of paranoia...
       */
      code = elbr_WriteRestartBlock(ldP, *logLocnArray, &restart,
				    lastPageHdr->passNumber + 2,
				    lastPageHdr->cksum_ver);
      RECVR_ASSERT(code == 0);

      /* Step 6: clean up */

      RECVR_CLEANUP(allocList);
      RECVR_CLEAR();

      osi_GetTime(&endTime);

      elbr_lastTime = (endTime.tv_sec - startTime.tv_sec) * 1000 +
	  (endTime.tv_usec - startTime.tv_usec) / 1000;

      printf("recovery statistics:\n");
	  printf("\tElapsed time was %d ms\n", elbr_lastTime);

      printf("\t%d log page%s recovered consisting of %d record%s\n",
	     elbr_lastNpage, (elbr_lastNpage == 1 ? "" : "s"),
	     elbr_lastNrec, (elbr_lastNrec == 1 ? "" : "s"));

      printf("\tModified %d data block%s\n", elbr_lastNdata,
	     (elbr_lastNdata == 1 ? "" : "s"));

      printf("\t%d redo-data record%s, %d redo-fill record%s\n",
	     elbr_lastRedoData, (elbr_lastRedoData == 1 ? "" : "s"),
	     elbr_lastRedoFill, (elbr_lastRedoFill == 1 ? "" : "s"));

      printf("\t%d undo-data record%s, %d undo-fill record%s\n",
	     elbr_lastUndoData, (elbr_lastUndoData == 1 ? "" : "s"),
	     elbr_lastUndoFill, (elbr_lastRedoFill == 1 ? "" : "s"));
#if 0
      /* XXX: implement this in HandleBulkEnd */
      printf("\t%d equivalence classes seen, largest EC had %d elements\n",
	     elbr_lastNECs, elbr_lastLargestEC);
#endif /* 0 */


      return BUFFER_OK;

}

/*
 * elbr_DumpLog - this routine is responsible for reading in the contents
 * of the active log.  It can be called by an external program to retrieve the
 * actual contents of the active portion of the log.
 *
 */

/* EXPORT */
u_int32 elbr_DumpLog(
     IN struct async_device *ldP,
     IN daddr_t *logLocnArray,
     IN unsigned logLocnArraySize,
     OUT struct recvrRec **recvrRecordArray,
     OUT unsigned *logRecordCount)
{
      JMP_BUF_DECL;
      struct recvrAllocInfo * volatile allocList = (struct recvrAllocInfo *)0;
      struct logPageHdr *lastPageHdrPP;
      u_int32 code;

      if ((code = RECVR_CATCH())) {
	  /*
	   * this code path is our unwind case.  It would be nice if this
	   * path included the memory allocation failure problem so we
	   * could branch to a non-all-in-memory code path.  In either case,
	   * however we must clean up memory.
	   */

	  RECVR_CLEANUP(allocList);

	  return (code == 1) ?  RCVR_FAILED : code;
      }

      code = SafeDumpLog(ldP, logLocnArray, logLocnArraySize,
			 rcvr_env, recvrRecordArray,
			 logRecordCount, &lastPageHdrPP, &allocList);

      RECVR_CLEAR();
      return code;
}

/*
 * elbr_DumpEntireLog - this routine is responsible for reading in the contents
 * of the ENTIRE log.  It can be called by an external program to retrieve the
 * actual contents of the entire log.
 *
 */

/* EXPORT */
u_int32 elbr_DumpEntireLog(
     IN struct async_device *ldP,
     IN daddr_t *logLocnArray,
     IN unsigned logLocnArraySize,
     OUT struct recvrRec **logRecordArray,
     OUT unsigned *logRecordCount)
{
      JMP_BUF_DECL;
      struct recvrAllocInfo * volatile allocList = (struct recvrAllocInfo *)0;
      u_int32 code;

      if ((code = RECVR_CATCH())) {
	  /*
	   * this code path is our unwind case.  It would be nice if this
	   * path included the memory allocation failure problem so we
	   * could branch to a non-all-in-memory code path.  In either case,
	   * however we must clean up memory.
	   */

	  RECVR_CLEANUP(allocList);

	  return (code == 1) ?  RCVR_FAILED : code;
      }

      code = SafeDumpEntireLog(ldP, logLocnArray, logLocnArraySize,
			 rcvr_env, logRecordArray,
			 logRecordCount, 0, &allocList);
      RECVR_CLEAR();
      return code;
}

/*
 * ValidActiveLogPagePassNumber -
 *
 * The page pass # is OK if:
 * (1) the pass # is identical AND the logical # of page is <= head.
 * (2) the pass # is < head pass # AND the logical # of page is > head
 *     (wrapped log)
 *
 * The comparisons are ordered thus because it seems the first case is
 * most likely,
*/
static int ValidActiveLogPagePassNumber(
  IN struct logPageHdr *headPageP,
  IN struct logPageHdr *pageP)
{
    u_int16 pagePassNo, headPagePassNo;
    int result;

    pagePassNo = (u_int16)pageP->passNumber;
    headPagePassNo = (u_int16)headPageP->passNumber;

    if (pageP->pageNumber <= headPageP->pageNumber) {
	result = (pagePassNo == headPagePassNo);
    } else {
	result = (pagePassNo == DecPassno(headPagePassNo));
    }
    return result;
}

/*
 * SafeDumpLog - this routine is responsible for reading in the contents
 * of the active portion of the log.
 *
 * NB: the caller of this routine must be prepared to handle a longjmp
 * in case of an error!
 */

static u_int32 SafeDumpLog(
   IN struct async_device *ldP,
   IN daddr_t *logLocnArray,
   IN unsigned logLocnArraySize,
   IN recvr_jmp_buf rcvr_env,
   OUT struct recvrRec **recvrRecordArray,
   OUT unsigned *logRecordCount,
   OUT struct logPageHdr **lastPageHdrPP,
   OUT struct recvrAllocInfo * volatile *allocListP)
{
    unsigned low, high, current;
    struct logPageHdr *lowPageP, *highPageP, *currentPageP;
    struct logPageHdr page1, page2, page3;
    struct logPageHdr *pageP;
    unsigned tail, head;
    int index;
    struct recvrRec *recvrRecords, *newRecvrRecords;
    int recordCount;
    caddr_t *logBlocks;
    struct recvrAllocInfo * volatile allocList = allocListP ? *allocListP : 0;
    unsigned distance;
    unsigned pageNum;
    char *page;
    long code;
    unsigned char headPageCksumVer, headPageCksumLen;

    DBprint(("# SafeDumpLog: entered"));

    if (!recvrRecordArray || !logRecordCount)
	return RCVR_INVALID;

    /*
     * Take the listed array of buffers and attempt to
     * locate the head and tail of the log by performing
     * a binary search (on a reverse sorted list, however...)
     */

    /* The log page header ( including checksum ) is in the first
     * sector for the log page and we can rely on
     * correctness/atomicity of each sector.  So if the log page did
     * not get written out, the log page header would contain
     * information for an earlier pass, which would cause the binary
     * search to ignore this page. If only the first sector for the
     * log page had been written out and the binary search phase
     * determined this to be the head, the error would be detected by
     * a checksum error in the in-transit log pages check phase and
     * the head would be pushed back appropriately. Hence we can
     * afford not to check checksums in the binary search phase. All
     * active log pages outside the in-transit window are really
     * expected to be on disk correctly. If checksum errors are
     * detected for these pages, there is really nothing we can do
     * except give up.
     */

    low = 0;
    high = logLocnArraySize - 1;
    lowPageP = &page1;
    highPageP = &page2;
    currentPageP = &page3;
    readPage(ldP, logLocnArray[low], lowPageP);
    readPage(ldP, logLocnArray[high], highPageP);

    /*
     * A newly initialized log has pass # 0 written into
     * the log page.  This is an otherwise unused pass #.
     */
    if (lowPageP->passNumber == 0)
	RECVR_RAISE(RCVR_IOERR);	/* no legal pages at all */

    for (;;) {
	struct logPageHdr *lphP;

	/* first, compute halfway point */
	current = (low + high)/ 2;
	/* check for boundary condition */
	if (current == low || current == high) break;

	readPage(ldP, logLocnArray[current], currentPageP);

	if (currentPageP->salvageID != LOG_SALVAGE_MAGIC_NUMBER) {
	    /*
	     * log page is damaged; run salvager.
	     */
	    RECVR_RAISE(RCVR_IOERR);
	}

	/*
	 * arithmetic comparison.  distance of 0 or 1 means the
	 * current Page is equal to or newer - use the else clause.
	 * Otherwise the current page is older - use the if clause.
	 *
	 * I find that this algorithm is non-trivial to verify that it
	 * is correct, so here is the basic reasoning.  We have a log with
	 * three pointers and must adjust one of the pointers to narrow
	 * down where the most recently written page is located.
	 *
	 *   low              current                 high
	 * ||||||||||||||||||||||||||||||||||||||||||||||||||
	 *
	 * Now:
	 * if (low < current) we move the low pointer up
	 * if (low > current) we move the high pointer down
	 * if (low == current) we move the low pointer up
	 *
	 * or more graphically:
	 *
	 *   low              current                 high
	 * ||||||||||||||||||||||||||||||||||||||||||||||||||
	 * 33333333333333333444444444444444444444444444444444  (case 1)
	 * 44444444444444444443333333333333333333333333333333  (case 2)
	 * 33333333333333333333333333333333333334444444444444  (case 3)
	 *
	 * But we need to deal with the special case where we
	 * jump over 0, going from -1 to 1.  Thus, rather than
	 * using a straight comparison we use an arithmetic
	 * comparison:
	 *
	 * low < current when (low - current) > 1
	 * low > current when (low - current) = 1
	 * low == current when (low - current) = 0
	 *
	 * For example, when low is -1 and current is 1 (the wrap-around
	 * case) we get -2 which, through the wonders of unsigned
	 * arithmetic is > 1.
	 *
	 * The normal case is when low is n and current is n-1, we get
	 * -1, which, again because of unsigned arithmetic, is > 1.
	 *
	 * The other two cases (low == current and low > current) should
	 * be obvious.
	 *
	 * We combine cases (1) & (3) which had the same action, by
	 * noting that they are true if (low - current) is in [maxint/2,0]
	 * and that case (2) is true if (low - current is in [1,maxint/2-1]
	 *
	 * XXXX: note that we mask for the high bit - if we change the size
	 * of the on-disk passNumber field, we must change this
	 * comparison!
	 *
	 * - wam 920717
	 */
	distance = ((currentPageP->passNumber == 0)
		    ? 1			/* zero lower than anything */
		    : lowPageP->passNumber - currentPageP->passNumber);

	if ((distance == 0) || (distance & 0x8000)) {
	    /*
	     * move low up to current.
	     */
	    lphP = lowPageP;
	    lowPageP = currentPageP;
	    currentPageP = lphP;
	    low = current;
	    continue; /* read again*/
	} else {
	    /*
	     * move high down to current.
	     */
	    lphP = highPageP;
	    highPageP = currentPageP;
	    currentPageP = lphP;
	    high = current;
	    continue; /* read again */
	}
    } /* for */

    distance = lowPageP->passNumber - highPageP->passNumber;
    if (distance == 0) {
	/* lowPageP->passNumber == highPageP->passNumber */
	currentPageP = highPageP;
	current = high;
    } else {
	/* logical(lowPageP->passNumber) > logical(highPageP->passNumber) */
	currentPageP = lowPageP;
	current = low;
    }

    /*
     * When we write out each page we record how much pending I/O was
     * present at that time.  It is possible that we crashed after this
     * page was written out, but before the other pages were written
     * out to disk. Because we don't assume writes are in order, we
     * must check to assure ourselves that all the pages in the I/O
     * window did get written to the disk and are valid.
     *
     * We trust the log to make sure it doesn't assume a page with
     * uncomplete I/O is really complete (which means we trust the
     * underlying system to really guarantee that a page is on disk
     * when it says it is...)  So, if we find unwritten pages at this
     * point in the log, it is safe to move further back than that - there
     * cannot be any changes on disk we must undo that aren't in the
     * part of the log which IS on disk. - wam 920624
     */

    RECVR_ASSERT(currentPageP->io_window <= logLocnArraySize);


    /*
     * Find the oldest log page in the in-transit window of the head
     * that is invalid, if any. If we find such a log page, pull head
     * back to the page preceding the invalid log page
     */
    head = current;
    headPageCksumVer = currentPageP->cksum_ver;
    headPageCksumLen = currentPageP->cksum_len;

    /* Loop from oldest to latest in-transit pages. */
    for (index = currentPageP->io_window; index >= 0; index--) {

	RECVR_ALLOC(page, char *, ldP->blkSize, allocList);
	if (!page) {
	    panic("SafeDumpLog: malloc failed");
	}

	pageNum =
	    (head - index + logLocnArraySize) % logLocnArraySize;

	readData(ldP,logLocnArray[pageNum], page);
	pageP = (struct logPageHdr *)page;

	/*
	 * Check if log page is valid. The pass number also needs
	 * to be checked as the log page could be a much older logpage
	 * with a correct checksum but an old passnumber.
	 */

	if (!ValidLogPageChecksum(page, ldP->blkSize,
				  &headPageCksumVer, &headPageCksumLen) ||
	    !ValidActiveLogPagePassNumber(currentPageP, pageP)) {
	    head = (pageNum - 1 + logLocnArraySize) % logLocnArraySize;
	    break;
	}

    }

    if (head != current) {
	/*
	 * we must read the "head" block now, since it isn't the
	 * current block.
	 */
	DBprint(("partial I/O completion at head of log"));
	readPage(ldP, logLocnArray[head], currentPageP);
	current = head;
    }

    /* ASSERT(currentPageP is the pointer to the head page) */

    DBprint(("# SafeDumpLog: found head at %d", head));

    /*
     * Now that we have found the "head" of the log (the last log
     * block written) we can compute the "tail" of the log (the
     * oldest log record we will need for recovery)
     */

    /* need to handle boundary conditions */
    if (((currentPageP->oldestDataPage <= current) &&
	 (currentPageP->oldestTranPage <= current)) ||
	((currentPageP->oldestDataPage > current) &&
	 (currentPageP->oldestTranPage > current))) {
	/* both physically in front of head */
	/* both physically behind head */
	tail = min(currentPageP->oldestDataPage,
		   currentPageP->oldestTranPage);
    } else {
	/*
	 * one is ahead, one behind.  Find the one behind and
	 * then choose the one ahead
	 */
	tail = (currentPageP->oldestDataPage <= current) ?
	    currentPageP->oldestTranPage : currentPageP->oldestDataPage;

	/* sanity check - we picked the one physically "ahead" */
	RECVR_ASSERT(current < tail);
    }

    /* guarantee the caller described the log properly */
    RECVR_ASSERT(current == currentPageP->pageNumber);

    DBprint(("# SafeDumpLog: newest page at %d (block %d), points to oldest data %d, oldest tran %d\n",
	     current, logLocnArray[current], currentPageP->oldestDataPage,
	     currentPageP->oldestTranPage));

    /*
     * if tail is invalid, return an error
     */
    RECVR_ASSERT(tail < logLocnArraySize); /* zero indexed */

    DBprint(("# SafeDumpLog: found tail at %d", tail));

    /*
     * Now that we have the "head" and "tail" of the log we can actually
     * cycle from the tail to the head and build our array of recovery
     * records.
     */

    if (head == tail) {
	recordCount = 1;
    } else if (head > tail) {
	recordCount = head - tail + 1;
    } else {
	/* zero based array */
	recordCount = (logLocnArraySize - (tail - head) + 1);
    }

    /* now, let's be paranoid */
    recordCount = (logLocnArraySize < recordCount) ?
	logLocnArraySize : recordCount;

    RECVR_ASSERT (recordCount > 0);
#if 0 /* maybe this used to be possible... -ota 930715 */
    if (!recordCount) {
	/*  No log to recover anyway! */
	recvrRecordArray = (struct recvrRec **)0;
	*logRecordCount = 0;
	return BUFFER_OK;
    }
#endif

    RECVR_ALLOC(logBlocks, caddr_t *, sizeof(caddr_t) * (recordCount + 1),
		allocList);
    for (index = 0, current = tail; index < logLocnArraySize;
	 current = (current+1) % logLocnArraySize, index++) {

	/* TODO: is this a safe assumption ---
	   that we can read this much stuff into memory? */

	RECVR_ALLOC(logBlocks[index], char *, ldP->blkSize, allocList);
	if (logBlocks[index] == (caddr_t)0)
	    panic("SafeDumpLog: malloc failure during log read");

	readData(ldP, logLocnArray[current], logBlocks[index]);
	if (!ValidLogPageChecksum(logBlocks[index], ldP->blkSize,
				  &headPageCksumVer, &headPageCksumLen)) {
	    /*
	     * Invalid log page. Cannot proceed. Bail out.
	     * Only resort is to run salvager
	     */
	    RECVR_RAISE(RCVR_IOERR);
	}
	if (current == head)
	    break;

	/* Check passNumbers in the log pages. */
	if (!ValidActiveLogPagePassNumber(currentPageP,
					  (struct logPageHdr *)logBlocks[index]
					  )) {
	    RECVR_RAISE(RCVR_BADPASSNO);
	}
    }

    DBprint(("# SafeDumpLog: read %d log blocks", recordCount));
    /* now, walk through the records and build a linked
     * list of log records. We keep the records time-ordered.
     */

    elbr_lastNpage = recordCount;

    /* we special-case the "newest" page */
    code = parsePage(logBlocks[recordCount - 1], &recvrRecords,
		     allocListP, rcvr_env);
    if (code != 1) {
	RECVR_RAISE(RCVR_BADPAGE);
    }

    /*
     * set up a pointer to last log page header
     */
    if (lastPageHdrPP)
	*lastPageHdrPP = GetPageHdr(logBlocks[recordCount-1]);

    if (recordCount > 1) {
	for (index = recordCount - 2; ; index--) {
	    struct recvrRec *tmpP;

	    code = parsePage(logBlocks[index], &newRecvrRecords,
			     allocListP, rcvr_env);
	    if (code != 1) {
		RECVR_RAISE(RCVR_BADPAGE);
	    }

	    if (!newRecvrRecords) { /* empty page case */
		if (index == 0) {
		    break;
		}
		else {
		    continue;
		}
	    }

	    /* sanity check */
	    RECVR_ASSERT(newRecvrRecords != recvrRecords);

	    /* we must find the last element in the list, so we can
	     * join them. */
	    for (tmpP = newRecvrRecords; tmpP->nextLogRec;
		 tmpP = tmpP->nextLogRec) {
		PTR_OK(tmpP->page);
	    }

	    /* splice */
	    tmpP->nextLogRec = recvrRecords;

	    /* and reset pointer */
	    recvrRecords = newRecvrRecords;

	    /* we don't do a conditional above because we want to go through
	     * the code one last time even if index is zero.
	     */
	    if (index == 0)
		break;
	}
    }
    /*
     * we now have a linked list of recovery records.  Count them and
     * return them to the caller.
     */

    *recvrRecordArray = recvrRecords;

    for (index = 0; recvrRecords; index++,
	 recvrRecords = recvrRecords->nextLogRec) /* empty */;

    *logRecordCount = elbr_lastNrec = index;
    DBprint(("# SafeDumpLog: will process %d log records", elbr_lastNrec));
    return 0;
}

static u_int32 SafeDumpLogRecords(
   IN caddr_t *logPages,
   IN int logPageCount,
   IN recvr_jmp_buf rcvr_env,
   OUT struct recvrRec **logRecordsArray,
   OUT unsigned *logRecordCount,
   OUT struct logPageHdr **lastPageHdrPP,
   OUT struct recvrAllocInfo * volatile *allocListP)
{
    struct recvrRec *logRecords, *logPageRecords;
    int index;
    long code;

    code = parsePage(logPages[logPageCount - 1], &logPageRecords,
	      allocListP, rcvr_env);
    if (code != 1) {
	RECVR_RAISE(RCVR_BADPAGE);
    }
    logRecords = logPageRecords;

    /*
     * set up a pointer to last log page header
     */
    if (lastPageHdrPP) {
	*lastPageHdrPP =
	    GetPageHdr(logPages[logPageCount-1]);
    }

    if (logPageCount > 1) {
	for (index = logPageCount - 2; ; index--) {
	    struct recvrRec *tmpP;

	    code = parsePage(logPages[index], &logPageRecords,
			     allocListP, rcvr_env);
	    if (code != 1) {
		RECVR_RAISE(RCVR_BADPAGE);
	    }

	    if (!logPageRecords) { /* empty page case */
		if (index == 0) {
		    break;
		}
		else {
		    continue;
		}
	    }

	    /* sanity check */
	    RECVR_ASSERT(logPageRecords != logRecords);

	    /* we must find the last element in the list, so we can
	     * join them. */
	    for (tmpP = logPageRecords; tmpP->nextLogRec;
		 tmpP = tmpP->nextLogRec) {
		PTR_OK(tmpP->page);
	    }

	    /* splice */
	    tmpP->nextLogRec = logRecords;

	    /* and reset pointer */
	    logRecords = logPageRecords;

	    /* we don't do a conditional above because we want to go through
	     * the code one last time even if index is zero.
	     */
	    if (index == 0)
		break;
	}
    }
    /*
     * we now have a linked list of recovery records.  Count them and
     * return them to the caller.
     */

    *logRecordsArray = logRecords;

    for (index = 0; logRecords; index++,
	 logRecords = logRecords->nextLogRec) /* empty */;

    *logRecordCount = index;
    return 0;
}

/*
 * SafeDumpEntireLog - this routine is responsible for reading in the contents
 * of the entire log, not only the active section.
 *
 * NB: the caller of this routine must be prepared to handle a longjmp
 * in case of an error!
 */

static u_int32 SafeDumpEntireLog(
   IN struct async_device *ldP,
   IN daddr_t *logLocnArray,
   IN unsigned logLocnArraySize,
   IN recvr_jmp_buf rcvr_env,
   OUT struct recvrRec **logRecordArray,
   OUT unsigned *logRecordCount,
   OUT struct logPageHdr **lastPageHdrPP,
   OUT struct recvrAllocInfo * volatile *allocListP)
{
    caddr_t *logPages;
    int logPageCount;
    struct recvrAllocInfo * volatile allocList = allocListP ? *allocListP : 0;
    int index;

    if (!logRecordArray || !logRecordCount)
	return RCVR_INVALID;

    logPageCount = logLocnArraySize;
    RECVR_ALLOC(logPages, caddr_t *, sizeof(caddr_t) * (logPageCount + 1),
		allocList);
    for (index = 0; index < logLocnArraySize; index++) {
	RECVR_ALLOC(logPages[index], char *, ldP->blkSize, allocList);
	if (logPages[index] == (caddr_t)0) {
	    panic("SafeDumpEntireLog: malloc failure during log read");
	}
	readData(ldP, logLocnArray[index], logPages[index]);
	if (!ValidLogPageChecksum(logPages[index], ldP->blkSize, 0, 0)) {
	    /*
	     * Invalid log page. Cannot proceed. Bail out.
	     * Only resort is to run salvager
	     */
	    printf("Log page #%d at block %d has invalid checksum\n",
		   index, logLocnArray[index]);
	    RECVR_RAISE(RCVR_IOERR);
	}
    }
    SafeDumpLogRecords(logPages, logPageCount, rcvr_env,
		       logRecordArray, logRecordCount,
		       lastPageHdrPP, allocListP);
    return 0;
}

/*
 * code local to this module
 */

/*
 * blockOp - primitive used to read or write a disk block
 *
 */
static void blockOp(
     struct buffer *buffer,
     unsigned *retval,
     unsigned op)
{
     long returnVal;
     asevent_event_t *ae;
     struct buf *bp;

     ae = asevent_Create(ASEVENT_NOPIN);
     bp = asevent_MakeBuf(
	op, buffer->data, buffer->adev->blkSize,
	buffer->adev, buffer->blockNum, 0);
     returnVal = asevent_QueueIO(buffer->adev, bp, ae);
     if (returnVal == 0)
	returnVal = asevent_WaitEvent(ae);
     asevent_Release(ae);
     *retval  = (unsigned)returnVal;
}

/*
 * readData - read a data block
 *
 */
static void readData(
     async_device_t *dev,
     daddr_t blockNumber,
     caddr_t dataBuf)
{
      struct buffer buf;
      unsigned returnVal;

      DBprint(("# readData block %d", blockNumber));
      bzero((char *)&buf, sizeof buf);
      buf.adev = dev;
      buf.bufFlags = BUFFER_INUSE;
      buf.bufType = elbb_bufRecvr;
      buf.refCount = 1;
      buf.data = dataBuf;
      buf.blockNum = blockNumber;
      readBlock(&buf, returnVal);
      if (returnVal != 0) panic("ReadData: bad read");
      return;
}

/*
 * writeData - write a data block
 *
 */
static void writeData(
     async_device_t *dev,
     daddr_t blockNumber,
     caddr_t dataBuf)
{
      struct buffer buf;
      unsigned returnVal;

      DBprint(("# writeData block %d", blockNumber));
      bzero((char *)&buf, sizeof buf);
      buf.adev = dev;
      buf.bufFlags = BUFFER_INUSE;
      buf.bufType = elbb_bufRecvr;
      buf.refCount = 1;
      buf.data = dataBuf;
      buf.blockNum = blockNumber;
      writeBlock(&buf, returnVal);
      if (returnVal != 0) panic("ReadData: bad read");
      return;
}

/*
 * readPage - read a log page in.
 *
 * This routine reads the requested page and copies the page header
 * into the buffer supplied by the caller.
 */
static void readPage(
     async_device_t *dev,
     daddr_t blockNum,
     struct logPageHdr *page)
{
      char *data;

      VALID_SIZE(dev->blkSize);
      data = osi_Alloc_r(dev->blkSize);
      if (!data) panic("readPage: malloc failed");
      logbuf_pin(data, dev->blkSize);
      readData(dev, blockNum, data);
      bcopy(data, (char *)(page), sizeof(struct logPageHdr));
      logbuf_unpin(data, dev->blkSize);
      osi_Free_r((opaque)data, (dev)->blkSize);
}

/*
 * parsePage - take a log block and turn it into a list of recovery
 * records.  It returns 0 if the page is invalid or cannot be parsed
 * correctly;  some state may be returned however.
 * Space is allocated for the Records only; all other space is recycled
 * from the buffer(s).
 */
static long parsePage(
     IN char *pageBuf,
     OUT struct recvrRec **Records,
     struct recvrAllocInfo * volatile *allocListP,
     recvr_jmp_buf rcvr_env)
{
      struct logPageHdr *pageHdr;
      struct recvrRec *firstRec, *newRec;
      unsigned offset;
      struct recvrAllocInfo * volatile allocList = allocListP ? *allocListP : 0;

      DBprint(("# *** parsePage ***"));
      /* first, find the page header itself */
      pageHdr = GetPageHdr(pageBuf);

      DBprint(("# parsing page, header looks like:"));
      DBprint(("# passNumber %d, last offset %d", pageHdr->passNumber,
		     pageHdr->lastLogRecOff));
      DBprint(("# page # %d, timeWritten %lu",
	       pageHdr->pageNumber, pageHdr->timeWritten));
      DBprint(("# oldest data page %d, oldest tran page %d",
		     pageHdr->oldestDataPage,
		     pageHdr->oldestTranPage));

      RECVR_ASSERT(pageHdr->salvageID == LOG_SALVAGE_MAGIC_NUMBER);
      DBprint(("# page header is OK"));

      if (pageHdr->passNumber == 0) {
	  /* nothing on this page. */
	  *Records = (struct recvrRec *)0;
	  return 1;
      }

      /* allocate space for the first record
       * XXX: osi_Alloc panics if the space is not available; perhaps this
       * isn't the right result.
       */
      RECVR_ALLOC(firstRec, struct recvrRec *, sizeof(struct recvrRec),
		  allocList);
      newRec = firstRec;

      /*
       * We cycle through, builing a linked list (being careful to
       * maintain correct order here!) which terminates when we've
       * reached the last log record (as recorded in the page header.
       */
      for(offset = elbr_FirstLogRecordOffset(pageHdr); ; ) {

	  /* remember which page we got this from */
	  newRec->page = pageHdr;

	  /* "read" the log record */
	  RECVR_ASSERT(offset);
	  RECVR_ASSERT(offset <= pageHdr->lastLogRecOff);
	  newRec->logrec = GetLogrec(pageBuf, offset);

	  /* initialize the data structure references */
	  FIFO_INITTHREAD(&newRec->tranThread);
	  FIFO_INITTHREAD(&newRec->diskThread);
	  newRec->tranFifoP = (struct fifo *)0;
	  newRec->status = RECVR_STATUS_UNDEF; /* not complete */

	  offset += NextRecOffset(newRec->logrec);

	  /*
	   * Do not remove this verify; we need to check
	   * each record at least once, so we do it here.
	   * - wam 10/18/91
	   */
	  VerifyRecord(newRec, rcvr_env);

	  if (offset > pageHdr->lastLogRecOff) {
	      newRec->nextLogRec = (struct recvrRec *)0;
	      break; /* done */
	  }

	  /* now create a new record */
	  RECVR_ALLOC(newRec->nextLogRec, struct recvrRec *,
		      sizeof(struct recvrRec), allocList);

	  /*
	   * we are appending to the *end* of the linked list
	   * to preserve time-ordering.  The first record is
	   * saved in firstRec (remember?)
	   */
	  newRec = newRec->nextLogRec;
      }
      *Records = firstRec;
      /* done */
      DBprint(("# *** exiting parsePage"));
      return 1;
}

/*
 * AddToTranHash - add a list of records to a transaction hash table.
 * this routine does not return failure.
 */
static void AddToTranHash(
     struct fifo *hashTable,
     struct recvrRec *records, /* linked list returned by ParsePage */
     recvr_jmp_buf rcvr_env)
{
      unsigned index;
      struct recvrRec *recP;
      struct fifo *bucketP;

      PTR_AOK(hashTable);
      PTR_AOK(records);

      /*
       * initialize table before use
       */
      for (index = 0; index < RECOVER_HASHMAPSIZE; index++) {
	  bucketP = (struct fifo *)(hashTable + index);
	  FIFO_INIT (bucketP,
		     (unsigned)((char *)(&((struct recvrRec *)0)->tranThread)-
				(char *)(0)));
      }

      for (recP = records; recP; recP = recP->nextLogRec) {
	  index = recvrHash(recP->logrec->hdr.tranId);
	  bucketP = (struct fifo *)(hashTable + index);

	  /* we must maintain the time-based ordering of transactions
	   * so we insert the newest transaction at the end of the hash
	   * bucket.
	   */
	  PTR_AOK(bucketP);

	  FIFO_ADD(bucketP, &recP->tranThread);
	  recP->tranFifoP = bucketP;
      }

      return;
}

/*
 * AddToDiskHash - add a list of records to a disk block hash table.
 * this routine does not return failure.
 *
 * The only difference between TranHash and DiskHash is the actual
 * hash function used; this could be consolidated by some extra intelligence.
 * I don't do it b/c it doesn't seem to be worth the extra complexity or
 * work - these routines are simple. - wam 4/30/91.
 */
static void AddToDiskHash(
     struct fifo *hashTable,
     struct recvrRec *records, /* linked list returned by ParsePage */
     recvr_jmp_buf rcvr_env)
{
      unsigned index;
      struct recvrRec *recP;
      struct fifo *bucketP;

      PTR_AOK(hashTable);
      PTR_AOK(records);

      /*
       * initialize table before use
       */
      for (index = 0; index < RECOVER_HASHMAPSIZE; index++) {
	  bucketP = (struct fifo *)(hashTable + index);
	  FIFO_INIT (bucketP, DISKADDRTHREAD);
      }

      /*
       * The disk hash is organized as a two dimensional data structure.
       * Each element in the hash table represents a different disk address.
       * Hanging off that element is a list of other elements which also
       * modify that disk block.
       */

      for (recP = records; recP; recP = recP->nextLogRec ) {
	  struct recvrRec *rec2P;

	  if (!LOGREC_ISRECOVERABLE(recP->logrec->hdr.recType)) {
	      /*
	       * This log record isn't going to modify the disk.
	       * We skip it.
	       */
	      continue;
	  }

	  index = recvrHash(recP->logrec->hdr.dataAddr);
	  bucketP = (struct fifo *)(hashTable + index);

	  for (rec2P = FIFO_LookAtOldest(bucketP, DISKADDRTHREAD);
	       rec2P;
	       rec2P = FIFO_NextOldest(bucketP, rec2P,
				       &rec2P->diskAddrThread)) {
	      if (rec2P->logrec->hdr.dataAddr == recP->logrec->hdr.dataAddr)
		  break;
	  }

	  if (rec2P) {
	      /* add to existing chain */
	      FIFO_ADD(&rec2P->diskFifo, &recP->diskThread);
	  } else {
	      /* insert new element in hash list and make it the first record
               * in its own fifo */
	      FIFO_ADD(bucketP, &recP->diskAddrThread);
	      FIFO_INIT(&recP->diskFifo, DISKTHREAD);
	      FIFO_ADD(&recP->diskFifo, &recP->diskThread);
	  }
      }

      return;
}

#if 0
static void VerifyRecIsInList(
     struct recvrRec *recList,
     struct recvrRec *recP,
     recvr_jmp_buf rcvr_env)
{
      struct recvrRec *rP;

      VerifyRecord(recP, rcvr_env);

      /*
       * we really are only trying to verify this record is in the
       * list.  If it isn't we are going to fail.
       */
      for (rP = recList; rP && (rP != recP); rP = rP->nextLogRec)
	  ;

      RECVR_ASSERT(rP);
}
#endif

static void VerifyRecordList(
     struct recvrRec *recList,
     recvr_jmp_buf rcvr_env)
{
      struct recvrRec *rP;
      u_int32 lastLSN, thisLSN;

      thisLSN = 0;

      for (lastLSN = 0, rP = recList;
	   rP;
	   lastLSN = thisLSN, rP = rP->nextLogRec) {
	  PTR_ALIGNED(rP);

	  PTR_OK(rP->page);
	  PTR_ALIGNED(rP->page);

	  PTR_OK(rP->logrec);
	  PTR_ALIGNED(rP->logrec);
	  PTR_ALIGNED(rP->nextLogRec);
	  /*
	   * detect obvious cycles
	   */
	  RECVR_ASSERT(rP->nextLogRec != recList);
	  /*
	   * we must guarantee our transactions remain time-ordered.
	   */
	  thisLSN = GetLogrecLSN(rP->logrec, 0);
	  if (lastLSN)
	      RECVR_ASSERT(lastLSN < thisLSN);
      }
}

/*
 * VerifyRecord - check vailidity of a recovery record structure.
 *
 * This routine assumes it is threaded on both hash tables.
 */
static void VerifyRecord(
     IN struct recvrRec *recP,
     IN recvr_jmp_buf rcvr_env)
{
      RECVR_ASSERT(recP->page);
      RECVR_ASSERT(recP->logrec);
      PTR_OK(recP);
      PTR_ALIGNED(recP);
      PTR_OK(recP->logrec);
      PTR_ALIGNED(recP->logrec);
      /* we don't use the zero transaction id - but we allow it for a restart
       * record
       */
      RECVR_ASSERT((recP->logrec->hdr.tranId != 0) ||
		   (recP->logrec->hdr.recType == LOGREC_RSTRT) ||
		   (recP->logrec->hdr.recType == LOGREC_CKPT));
      if (!LOGREC_ISRECOVERABLE(recP->logrec->hdr.recType)) {
	  /* verification of end records & batch end records */
	  if (LOGREC_ISBEND(recP->logrec->hdr.recType)) {
	      /*
	       * A batch end record includes an end record.
	       * This code just makes sure the end record is
	       * of the correct length.
	       */
	      RECVR_ASSERT(recP->logrec->hdr.dataLength ==
			   NewLogrecSize(LOGREC_END, 0));
	  }
	  /*
	   * These are invariants & server as good sanity checks.
	   */
	  RECVR_ASSERT(recP->logrec->hdr.dataAddr == 0);
	  RECVR_ASSERT((recP->logrec->hdr.dataOffset == 0) ||
		       (recP->logrec->hdr.recType == LOGREC_CKPT) ||
		       (recP->logrec->hdr.recType == LOGREC_RSTRT));
      }
}

/*
 * HandleBulkEnd -- this routine traverses the list to find any bulk end
 *     transactions and processes them when found.
 *
 *     Bulk ends are the recovery end of ECs.  Because the end records must
 *     written atomically we use a new record type (BEND) and a new transaction
 *     to write all the associated transactions in.  When we see a transaction
 *     with BEND records we search to see if the transaction committed.  If it
 *     did, we take the data section of each BEND record (which is really an
 *     end record for the specific transaction) and add the appropriate end
 *     record into the various hash tables.  The caller should then go back and
 *     recompute the commitment state of all the transactions to incorporate
 *     these new end records.
 */
static int32 HandleBulkEnd(
     struct recvrRec *recvrList,
     struct fifo *hashTable,
     recvr_jmp_buf rcvr_env)
{
      struct recvrRec *recP;

      /* we traverse the list of all possible records */
      for (recP = recvrList; recP; recP = recP->nextLogRec) {
	  /* we are looking for a batch end */
	  if (LOGREC_ISBEND (recP->logrec->hdr.recType) &&
	      (recP->status == RECVR_STATUS_REDO)) {
	      /*
               * we now allow transactions to mix data records with batch end
               * records.  We only need process this record if it is a batch
               * end record.  We do this by changing the type of the record and
               * adjusting this record pointer to point into the data area.
               * This maintains the time ordering of our records yet eliminates
               * the now-spurious batch end record.
               */
	      recP->logrec = (struct logRec *)
		  GetLogrecOldData((char *)recP->logrec, 0);
	  } /* BEND record */
      } /* for */
      return BUFFER_OK;
}

/*
 * ReInitList - takes a list of recovery records and forcibly removes them
 * all from the hash tables.
 */
static void ReInitList(
     struct recvrRec *recvrRecList,
     recvr_jmp_buf rcvr_env)
{
      struct recvrRec *rP;

      for (rP = recvrRecList; rP; rP = rP->nextLogRec) {
	  FIFO_INITTHREAD(&rP->tranThread);
	  FIFO_INITTHREAD(&rP->diskThread);
	  rP->tranFifoP = (struct fifo *)0;
      }
}

/*
 * MarkCommitted -- mark all committed transactions within the system.
 *
 *     This routine traverses the transaction list (one more time!) to mark all
 *     transaction records (not just the start records.)  It presumes that the
 *     start records are marked correctly.
 */
static void MarkCommitted(
  IN struct fifo *hashTable,
  IN int undefOkay,
  IN recvr_jmp_buf rcvr_env)
{
    struct fifo *bucketP;
    unsigned index;
    struct recvrRec *recP, *rec2P;

    PTR_OK(hashTable);

#define SameTran(r1,r2) ((r1)->logrec->hdr.tranId == (r2)->logrec->hdr.tranId)
#define FirstTran(f,r) (struct recvrRec *) \
	FIFO_LookAtOldest(f, offsetof(struct recvrRec, tranThread))
#define NextTran(f,r) (struct recvrRec *)FIFO_NextOldest(f,r,&(r)->tranThread)
#define PrevTran(f,r) (struct recvrRec *)FIFO_NextNewest(f,r,&(r)->tranThread)
#define LogrecIsStart(r) ((r)->logrec->hdr.refCount == 1)
#define LogrecIsRedo(r) ((r)->status == RECVR_STATUS_REDO)
#define LogrecIsUndef(r) ((r)->status == RECVR_STATUS_UNDEF)
#define LogrecHasStatus(r) LOGREC_HASSTATUS((r)->logrec->hdr.recType)
#define LogrecIsEnd(r) LOGREC_ISEND((r)->logrec->hdr.recType)
    /*
     * we seek down each bucket in the transaction hash table; when
     * we find a begin trans we mark it and all associated transactions
     * as either REDO or UNDO.
     *
     * There are four states a transaction could be in at this point,
     * three of which are valid.  Here are the four states:
     * (has start, has end)
     * (has start, no end)
     * (no start, has end)
     * (no start, no end)
     *
     * The last state (no start, no end) is invalid - we should NEVER
     * see such a log.  The other three are valid.  Their corresponding
     * operations upon recovery are:
     *
     * (has start, has end) - REDO
     * (has start, no end) - UNDO
     * (no start, has end) - REDO
     *
     * the fourth case should cause recovery to fail.
     *
     */
    for (index = 0; index < RECOVER_HASHMAPSIZE; index++) {
	bucketP = (struct fifo *)hashTable + index;

	if (IsFifoEmpty(bucketP)) continue; /* skip empty buckets */

	/*
	 * We now go looking for end records.  When we find one, we begin
	 * seeking backwards within the fifo for records containing the same
	 * transaction ID.
	 */
	for (recP = FirstTran(bucketP, recP);
	     recP;
	     recP = NextTran(bucketP, recP)) {

	    /* internal sanity check */
	    PTR_AOK(recP);
	    PTR_AOK(recP->logrec);

	    if (!LogrecIsEnd(recP))
		continue; /* skip this record */

	    /*
	     * We've now found an end record.  Mark all transaction records
             * with this transaction ID as REDO.  We begin seeking BACKWARDS
             * from the end record.
             */
	    for (rec2P = recP; rec2P; rec2P = PrevTran(bucketP, rec2P)) {
		if (SameTran(recP, rec2P)) {
		    rec2P->status = RECVR_STATUS_REDO;
		}
	    } /* for - looking for REDO transactions */
	} /* for - looking for END records */

	for (recP = FirstTran(bucketP, recP);
	     recP;
	     recP = NextTran(bucketP, recP)) {

	    /* we are looking for start records not in REDO state */
	    if (! LogrecIsStart(recP) || LogrecIsRedo(recP))
		continue; /* skip this record */

	    /*
             * We've now found a start record not in the REDO state (implying
             * there is no END record.  So, we mark all other transaction
             * records with this tran id as UNDO type operations.
             */
	    for (rec2P = recP; rec2P; rec2P = NextTran(bucketP, rec2P)) {
		if (SameTran(recP, rec2P)) {
		    rec2P->status = RECVR_STATUS_UNDO;
		}
	    } /* for - looking for UNDO transactions */

	} /* for - looking for START records */

	/*
	 * XXX: This is a SANITY check.  It is not required for normal
	 * operations, but until we are certain the logging and recovery code
	 * are working 100%, it is relatively cheap and useful.
	 *
	 * Check for any records remaining in the UNDEF state.
	 */
	if (!undefOkay) {
	    for (recP = FirstTran(bucketP, recP);
		 recP;
		 recP = NextTran(bucketP, recP))
		if (LogrecHasStatus(recP)) {
		    RECVR_ASSERT(! LogrecIsUndef(recP));
		}
	}

    } /* for - walking the hash buckets */

#undef SameTran
#undef FirstTran
#undef NextTran
#undef PrevTran
#undef LogrecIsStart
#undef LogrecIsRedo
#undef LogrecIsUndef
}

/* HandleKillRecords -- looks through the records for a block, from newest to
 *     oldest, for undo-zero records used by the block allocator to implement
 *     NBS.  These must be interpreted as kill records for earlier updates to
 *     the same block.  Set the status of all such updates to SKIP. */

static void HandleKillRecords(
  struct recvrRec *head,
  recvr_jmp_buf rcvr_env)
{
    struct recvrRec *rec;
    int someRecs;
    int state;
#define HKR_FIND_NEWEST_KILL 1
#define HKR_SKIP_KILLS 2
#define HKR_FLUSH_RECORDS 3

    state = HKR_FIND_NEWEST_KILL;
    someRecs = 0;			/* true if reg. rec in 1st state */
    for (rec = FIFO_LookAtNewest(&(head)->diskFifo, DISKTHREAD);
	 rec;
	 rec = FIFO_NextNewest(&(head)->diskFifo, rec, &(rec)->diskThread)) {

	switch (state) {
	  case HKR_FIND_NEWEST_KILL:
	    if (!LOGREC_ISKILL(rec->logrec)) {
		someRecs = 1;
		break;
	    }
	    state = HKR_SKIP_KILLS;
	    /* FALL THROUGH */

	  case HKR_SKIP_KILLS:
	    if (LOGREC_ISKILL(rec->logrec)) {
		if ((rec->status == RECVR_STATUS_UNDO) && someRecs)
		    RECVR_RAISE (RCVR_USERDATA);
		break;
	    }
	    state = HKR_FLUSH_RECORDS;
	    /* FALL THROUGH */

	  case HKR_FLUSH_RECORDS:
	    if (rec->status == RECVR_STATUS_UNDO)
		RECVR_RAISE (RCVR_METADATA);
	    rec->status = RECVR_STATUS_SKIP;
	    break;

	  default:
	    RECVR_RAISE(RCVR_ASSERT);
	}
    }
}
#define ReadBlock(dP, bn, db) \
do { \
    if (!iodone) { \
	readData((dP), (bn), (db)); \
	iodone=1; \
        blockCount++; \
    } \
} while (0)
/*
 * AdjustDisk -- traverse disk hash table to compute the change set.
 *
 */
static void AdjustDisk(
     IN struct fifo *hashTable,
     IN async_device_t *devP,
     INOUT struct recvrAllocInfo * volatile *allocListP,
     IN recvr_jmp_buf rcvr_env)
{
      struct fifo *bucketP;
      unsigned index;
      struct recvrRec *recP, *rec2P;
      caddr_t diskBlock;
      daddr_t blockNum;
      struct recvrAllocInfo * volatile allocList = allocListP ? *allocListP : 0;
      unsigned blockCount = 0;
      int iodone = 0; /* defer i/o until necessary */

      PTR_OK(hashTable);
      PTR_OK(devP);

      VALID_SIZE(devP->blkSize);
      RECVR_ALLOC(diskBlock, caddr_t, devP->blkSize, allocList);

      /*
       * we seek through all the buckets in the disk hash table.  For
       * each bucket we compute the change set on all pertinent disk
       * blocks.
       */

      for (index = 0; index < RECOVER_HASHMAPSIZE; index++) {
	  bucketP = (struct fifo *)(hashTable + index);


	  if (IsFifoEmpty(bucketP)) continue; /* skip empty buckets */

	  /*
	   * To actually perform the recovery we must make two passes.
	   * Pass # 1 is where all undo operations are done; pass # 2
	   * is where all redo operations are done.  When all operations
	   * have completed, we write out the disk block and go to the
	   * next one.
	   */

	  for (recP = FIFO_LookAtOldest(bucketP, DISKADDRTHREAD);
	       recP;
	       recP = FIFO_NextOldest(bucketP, recP, &recP->diskAddrThread)) {

	      iodone = 0; /* no I/O yet */

	      blockNum = recP->logrec->hdr.dataAddr;

	      /* Pass #0 -- find kill records and flush earlier records. */
	      HandleKillRecords(recP, rcvr_env);

	      /*
	       *  pass # 1 - perform all the undo operations.
	       */

	      for (rec2P = FIFO_LookAtNewest(&(recP)->diskFifo, DISKTHREAD);
		   rec2P;
		   rec2P = FIFO_NextNewest(&(recP)->diskFifo, rec2P,
					   &(rec2P)->diskThread)) {

		  RECVR_ASSERT(rec2P->logrec->hdr.dataAddr == blockNum);

		  /* We only perform the operation if really necessary:
		   * (1) the record is a recoverable type
		   * (2) the transaction is in the undo status
		   * (3) there is old data/fill */

		  if (LOGREC_ISRECOVERABLE(rec2P->logrec->hdr.recType) &&
		      (rec2P->status == RECVR_STATUS_UNDO) &&
		      LOGREC_HASOLDDATA(rec2P->logrec->hdr.recType)) {
		      ReadBlock(devP, blockNum, diskBlock);
		      Undo(rec2P, diskBlock, rcvr_env);
		  }
	      }

	      /*
	       * pass #2 - perform all the redo operations; note the
	       * sanity check here to assure ourselves that the log records
	       * are still OK.
	       */

	      for (rec2P = recP;
		   rec2P;
		   rec2P = FIFO_NextOldest(&(recP)->diskFifo, rec2P,
					   &(rec2P)->diskThread)) {

		  /* We only perform the operation if really necessary:
		   * (1) the record is a recoverable type
		   * (2) the transaction is in the redo status
		   * (3) there is new data/fill */

		  if (LOGREC_ISRECOVERABLE(rec2P->logrec->hdr.recType) &&
		      (rec2P->status == RECVR_STATUS_REDO) &&
		      LOGREC_HASNEWDATA(rec2P->logrec->hdr.recType)) {
		      ReadBlock(devP, blockNum, diskBlock);
		      Redo(rec2P, diskBlock, rcvr_env);
		  }
	      } /* for */

	      /*
	       * write changes back (synch. I/O) - do nothing if
	       * I/O was deferred (ie for NBS).
	       */
	      if (iodone)
		  writeData(devP, blockNum, diskBlock);

	  } /* for */
      } /* for */

      elbr_lastNdata = blockCount;
      return;
}

/*
 * Redo - perform a forward operation
 *
 * With the addition of asymmetric log records + fill records the redo and
 * undo operations became sufficiently complex they deserved to be broken
 * out into separate routines.
 */

static void Redo(
     struct recvrRec *recP,
     caddr_t block,
     recvr_jmp_buf rcvr_env)
{
      /* must be a real redo */
      RECVR_ASSERT((recP->status == RECVR_STATUS_REDO) ||
		   (recP->status == RECVR_STATUS_UNDEF));
      RECVR_ASSERT(LOGREC_ISRECOVERABLE(recP->logrec->hdr.recType));

      /* if there is no new data, redo is a no-op */
      if (LOGREC_HASNEWDATA(recP->logrec->hdr.recType)) {
	  /* if this is a fill type */
	  if (LOGREC_ISDATA(recP->logrec->hdr.recType)) {

	      elbr_lastRedoData++;

	      DBprint(("# redo-copy: blk %d, off %d, len %d, tran %d, ref %d",
		       recP->logrec->hdr.dataAddr,
		       recP->logrec->hdr.dataOffset,
		       recP->logrec->hdr.dataLength,
		       recP->logrec->hdr.tranId,
		       recP->logrec->hdr.refCount));

	      LOGCOPY((char *)recP->logrec + NewDataOffset(recP->logrec),
		      (block + recP->logrec->hdr.dataOffset),
		      (recP->logrec->hdr.dataLength));
	  } else if (LOGREC_ISFILL(recP->logrec->hdr.recType)) {
	      unsigned char ch = * (unsigned char *)
		  ((char *)recP->logrec + NewDataOffset(recP->logrec));

	      elbr_lastRedoFill++;


	      DBprint(("# redo-fill: blk %d, off %d, len %d, tran %d, ref %d",
		       recP->logrec->hdr.dataAddr,
		       recP->logrec->hdr.dataOffset,
		       recP->logrec->hdr.dataLength,
		       recP->logrec->hdr.tranId,
		       recP->logrec->hdr.refCount));

	      LOGFILL(block + recP->logrec->hdr.dataOffset, ch,
		      recP->logrec->hdr.dataLength);

	  } else RECVR_RAISE(RCVR_REDOTYPE);
      }
}

static void Undo(
     struct recvrRec *recP,
     caddr_t block,
     recvr_jmp_buf rcvr_env)
{
      /* must be a real undo */
      RECVR_ASSERT(recP->status == RECVR_STATUS_UNDO);
      RECVR_ASSERT(LOGREC_ISRECOVERABLE(recP->logrec->hdr.recType));

      /* if there is no old data, undo is a no-op */
      if (LOGREC_HASOLDDATA(recP->logrec->hdr.recType)) {
	  /* if this is a fill type */
	  if (LOGREC_ISDATA(recP->logrec->hdr.recType)) {

	      elbr_lastUndoData++;

	      DBprint(("# undo-copy: blk %d, off %d, len %d, tran %d, ref %d",
		       recP->logrec->hdr.dataAddr,
		       recP->logrec->hdr.dataOffset,
		       recP->logrec->hdr.dataLength,
		       recP->logrec->hdr.tranId,
		       recP->logrec->hdr.refCount));

	      LOGCOPY((char *)recP->logrec + OldDataOffset(recP->logrec),
		      (block + recP->logrec->hdr.dataOffset),
		      (recP->logrec->hdr.dataLength));
	  } else if (LOGREC_ISFILL(recP->logrec->hdr.recType)) {
	      unsigned char ch = * (unsigned char *)
		  ((char *)recP->logrec + OldDataOffset(recP->logrec));

	      elbr_lastUndoFill++;

	      DBprint(("# undo-fill: blk %d, off %d, len %d, tran %d, ref %d",
		       recP->logrec->hdr.dataAddr,
		       recP->logrec->hdr.dataOffset,
		       recP->logrec->hdr.dataLength,
		       recP->logrec->hdr.tranId,
		       recP->logrec->hdr.refCount));

	      LOGFILL(block + recP->logrec->hdr.dataOffset, ch,
		      recP->logrec->hdr.dataLength);
	  } else RECVR_RAISE(RCVR_UNDOTYPE);
      }
}

/*
 * Cleanup - cleanup allocated memory
 *
 * This routine takes a list of allocated memory blocks and frees them.
 */

static void Cleanup(volatile struct recvrAllocInfo *allocList)
{
      struct recvrAllocInfo * volatile alP;

      DBprint(("# cleanup start"));
      while (allocList) {
	  alP = allocList->next;
	  logbuf_unpin(allocList->buf, allocList->size);
	  osi_Free_r(allocList->buf, allocList->size);
	  logbuf_unpin(allocList, sizeof(struct recvrAllocInfo));
	  osi_Free_r((char *)allocList, sizeof(struct recvrAllocInfo));
	  allocList = alP;
      }
      DBprint(("# cleanup done"));
}

/*
 * elbr_ShowRecord - provide a printable format for a recovery record.
 * this routine is only used in user-space.
 */
/* EXPORT */
int32 elbr_ShowRecord(struct recvrRec *recP)
{
      printf("----------------------------\n");
      printf("page # %d\n", recP->page->pageNumber);
      printf("lsn = %d\n", GetLogrecLSN((char *)recP->logrec, 0));
      switch (recP->logrec->hdr.recType) {
	case LOGREC_DEBUG:
	  printf("debug record, message is %s\n",
		 (char *)recP->logrec + OldDataOffset(recP->logrec));
	  break;
	case LOGREC_DATA:
	  printf("data record\n");
	  break;
	case LOGREC_BEND:
	  printf("batch end record\n");
	  break;
	case LOGREC_END:
	  printf("end record\n");
	  break;
	case LOGREC_RSTRT:
	  printf("restart record\n");
	  break;
	case LOGREC_CKPT:
	  printf("checkpoint record\n");
	  break;
	case LOGREC_FLUSH:
	  printf("flush record\n");
	  break;
	default:
	  printf("**** unknown log record type ****\n");
	  break;
      }
      printf("recType: 0x%x\n", recP->logrec->hdr.recType);
      printf("refCount: %d\n", recP->logrec->hdr.refCount);
      printf("dataLength: %d\n", recP->logrec->hdr.dataLength);
      printf("tranId: 0x%x\n", recP->logrec->hdr.tranId);
      printf("dataOffset: %d\n", recP->logrec->hdr.dataOffset);
      printf("prevLogRecOff: %d\n", recP->logrec->hdr.prevLogRecOff);
      return BUFFER_OK;
}

/*
 * CvtData -- prints the data in a representable format.
 */
static void CvtData(char *data, u_long length)
{
    char printRep[1024 + 1];
    char *cp;
    unsigned datum;

    afsl_Assert(length <= 64 * 1024);

    /* the current printable scheme is to turn it into HEX */
    while (length) {
	cp = printRep;
	while (length) {
	    datum = ((*data)>>4 &0xf);
	    *cp++ = (datum < 10) ? '0' + datum : 'a' + (datum - 10);
	    datum = ((*data)&0xf);
	    *cp++ = (datum < 10) ? '0' + datum : 'a' + (datum - 10);
	    data++;
	    length--;
	    if ((cp - printRep) >= sizeof(printRep)-1)
		break;
	}
	if (cp != printRep) {
	    *cp = '\0';
	    printf ("%s", printRep);
	}
    }
}

/*
 * elbr_ScriptRecord is the interface for the script language.  It takes
 * a recovery record structure and generates an approximate script entry.
 * It isn't the intent of this routine to generate 100% script, rather it is
 * an attempt to generate something close enough it can be edited into shape.
 */

/* EXPORT */
int32 elbr_ScriptRecord(struct recvrRec *recP)
{
      printf("# page # %d, ", recP->page->pageNumber);
      printf("passNumber = %d, ", recP->page->passNumber);
      printf("timeWritten = %lu, ", recP->page->timeWritten);
      printf("io_window = %d, ", recP->page->io_window);
      printf("lsn = %d\n", GetLogrecLSN((char *)recP->logrec, 0));
      printf("# this is reference %d for transaction %d\n",
	     recP->logrec->hdr.refCount, recP->logrec->hdr.tranId);
      switch (recP->logrec->hdr.recType) {
	case LOGREC_DEBUG:
	  printf("# debug TRAN_ID DEBUG_MESSAGE\n");
	  printf("debug %d %s\n", recP->logrec->hdr.tranId,
		 (char *)recP->logrec + OldDataOffset(recP->logrec));
	  break;
	case LOGREC_DATA:
	  printf("# data2 TRAN_ID DATA_LENGTH DISK_BLOCK DATA_OFFSET OLD_DATA NEW_DATA\n");
	  printf("data2 %d %d %d %d ", recP->logrec->hdr.tranId,
		 recP->logrec->hdr.dataLength, recP->logrec->hdr.dataAddr,
		 recP->logrec->hdr.dataOffset);
	  CvtData((char *)recP->logrec + OldDataOffset(recP->logrec),
		  recP->logrec->hdr.dataLength);
	  printf(" ");
	  CvtData((char *)recP->logrec + NewDataOffset(recP->logrec),
		  recP->logrec->hdr.dataLength);
	  printf("\n");
	  break;
	case LOGREC_BEND:
      {
	  struct logRecHdr *hdrP;
	  printf("# batch end record\n");
	  printf("# bend TRAN_ID\n");
	  hdrP = (struct logRecHdr *)GetLogrecOldData((char *)recP->logrec, 0);
	  printf("bend %d\n", hdrP->tranId);
	  break;
      }
	case LOGREC_END:
	  printf("# end record\n");
	  printf("# end TRAN_ID\n");
	  if (recP->logrec->hdr.dataLength) {
	      printf("end %d %s\n", recP->logrec->hdr.tranId,
		     GetLogrecOldData((char *)recP->logrec, 0));
	  } else {
	      printf("end %d\n", recP->logrec->hdr.tranId);
	  }
	  break;

	case LOGREC_RSTRT:
	  printf("# restart record\n");
	  printf("rstrt\n");
	  break;
	case LOGREC_CKPT:
	  printf("# checkpoint record\n");
	  printf("ckpt\n");
	  break;
	case LOGREC_FLUSH:
	  printf("# flush record\n");
	  printf("# flush TRAN_ID DISK_BLOCK\n");
	  printf("flush %d %d\n", recP->logrec->hdr.tranId,
		 recP->logrec->hdr.dataAddr);
	  break;
	case LOGREC_NFILL:
	  printf("# nfill TRAN_ID disk_block data_offset pattern length\n");
	  printf("nfill %d %d %d ", recP->logrec->hdr.tranId,
		 recP->logrec->hdr.dataAddr, recP->logrec->hdr.dataOffset);
	  CvtData((char *)recP->logrec+OldDataOffset(recP->logrec), 1);
	  printf (" %d\n", recP->logrec->hdr.dataLength);
	  break;
	case LOGREC_OFILL:
	  printf("# ofill TRAN_ID disk_block data_offset pattern length\n");
	  printf("ofill %d %d %d ", recP->logrec->hdr.tranId,
		 recP->logrec->hdr.dataAddr, recP->logrec->hdr.dataOffset);
	  CvtData((char *)recP->logrec+OldDataOffset(recP->logrec), 1);
	  printf (" %d\n", recP->logrec->hdr.dataLength);
	  break;
	case LOGREC_ODATA:
	  printf("# odata TRAN_ID DATA_LENGTH DISK_BLOCK DATA_OFFSET NEW_DATA\n");
	  printf("odata %d %d %d %d ", recP->logrec->hdr.tranId,
		 recP->logrec->hdr.dataLength, recP->logrec->hdr.dataAddr,
		 recP->logrec->hdr.dataOffset);
	  CvtData((char *)recP->logrec + OldDataOffset(recP->logrec),
			 recP->logrec->hdr.dataLength);
	  printf ("\n");
	  break;
	case LOGREC_NDATA:
	  printf("# ndata TRAN_ID DATA_LENGTH DISK_BLOCK DATA_OFFSET NEW_DATA\n");
	  printf("ndata %d %d %d %d ", recP->logrec->hdr.tranId,
		 recP->logrec->hdr.dataLength, recP->logrec->hdr.dataAddr,
		 recP->logrec->hdr.dataOffset);
	  CvtData((char *)recP->logrec + NewDataOffset(recP->logrec),
		  recP->logrec->hdr.dataLength);
	  printf ("\n");
	  break;
	case LOGREC_START:
	  printf("# start TRAN_ID SERVICE\n");
	  printf("start %d %s\n", recP->logrec->hdr.tranId,
		 (char *)recP->logrec + OldDataOffset(recP->logrec));
	  break;
	default:
	  printf("# **** unknown log record type %d ****\n",
		 recP->logrec->hdr.recType);
	  break;
      }
      printf("#########################################\n");
      return 0;
}
#else /* !KERNEL */
/* Code for kernel space only */

/* EXPORT */
u_int32 elbr_Recover (
      IN struct async_device *ldP,
      IN struct async_device *dP,
      IN daddr_t *logLocnArray,
      IN unsigned logLocnArraySize)
{
      return RCVR_MUSTRCVR; /* no kernel recovery anymore */
}
#endif /* !KERNEL */

/*
 * code common to both kernel and user space.
 */

/*
 * GetChecksumLength
 *
 * NOTE: Suggested protocol is to call GetChecksumLength only if
 * the checksum version in question statisfies the KnownChecksumType check.
 */
static int GetChecksumLength(int cksumVer)
{
    int len;

    afsl_Assert(KnownChecksumType(cksumVer));
    len = cksumLen[cksumVer];
    return len;
}

static void CalcChecksum(
  char *data,
  long dataLen,
  unsigned int cksumVer,
  char *cksumP,
  int *cksumLenP)
{
    unsigned long crc;
    int cksumLength;

    switch (cksumVer) {
      case LOGREC_CKSUMVER_CRC32_STD_ALL:
	CalcCRC32Std(data, dataLen, &crc);
	cksumLength=GetChecksumLength(cksumVer);
	bcopy((char *)&crc, (char *)cksumP, cksumLength);
	*cksumLenP=cksumLength;
	break;

      case LOGREC_CKSUMVER_NONE:
	/* Fall through intentionally */

      default:
	*cksumP=0;
	*cksumLenP=0;
    }
}

static int RetrieveLogPageChecksumAttributes(
  const struct logPageHdr *pgHdrP,
  unsigned char *cksumVerP,
  int *cksumLenP)
{
    int cksumLength;
    int code = 0;

    if (pgHdrP->cksum_len > LOGREC_CKSUM_MAX_LEN ) {
	code |= BAD_CKSUM_LEN;
    }
    if (KnownChecksumType(pgHdrP->cksum_ver)) {
	cksumLength = GetChecksumLength(pgHdrP->cksum_ver);
	/*
	 * Account for (pre solaris release) aggregates that record checksums
	 * but no checksum lengths in the log pages, when determining
	 * checksum length validity
	 */
	if ((pgHdrP->cksum_len != cksumLength) &&
	    (pgHdrP->cksum_len != LOGREC_BACK_COMPAT_CKSUM_LEN)) {
	    code |= BAD_CKSUM_LEN;
	    cksumLength = pgHdrP->cksum_len;
	}
    } else {
	code |= UNKNOWN_CKSUM_TYPE;
	cksumLength = pgHdrP->cksum_len;
    }
    if (cksumVerP) {
	*cksumVerP = pgHdrP->cksum_ver;
    }
    if (cksumLenP) {
	*cksumLenP = cksumLength;
    }

    return code;
}

/* RetrieveLogPageChecksum
 *
 * RETURNS a bitset of the following bits.
 *    0                   - no error
 *    UNKNOWN_CKSUM_TYPE  - unknown checksum type found
 *    BAD_CKSUM_LEN       - bad checksum length
 *
 * In all cases it returns the checksum, the checkum version and the
 * checksum length as found in the log page (disk) passed in, if the
 * space has been allocated by caller.
 */
static int RetrieveLogPageChecksum(
  char *logPage,
  char *cksumP,
  unsigned char *cksumVerP,
  int *cksumLenP)
{
    int code;

    code = RetrieveLogPageChecksumAttributes((struct logPageHdr *)logPage,
					     cksumVerP, cksumLenP);
    if (cksumP) {
	bcopy((logPage + sizeof(struct logPageHdr)), cksumP, *cksumLenP);
    }
    return code;
}

static int CalcLogPageChecksum(
  char *logPage,
  long logPageSize,
  char *cksumP,
  int *cksumLenP)
{
    int cksumLength;
    char oldcksum[LOGREC_CKSUM_MAX_LEN];
    struct logPageHdr *pgHdrP;
    int code = 0;

    pgHdrP = (struct logPageHdr *)logPage;

    code = RetrieveLogPageChecksum(logPage, oldcksum, 0, &cksumLength);
    if (!code) {
	code = 0;
	ZeroLogPageChecksum(logPage, cksumLength);
	CalcChecksum(logPage, logPageSize, pgHdrP->cksum_ver,
		     cksumP, cksumLenP);
	/* Restore old checksum */
	StoreLogPageCksum(logPage, oldcksum, cksumLength);
    }
    return code;
}

static int SameChecksum(char *cksum1, char *cksum2, int cksumLength)
{
    int result;

    result = bcmp((char *)cksum1, (char *)cksum2, cksumLength);
    if (!result) {
	result = 1;
    } else {
	result = 0;
    }
    return result;
}

/* Check log page checksum consistency. If headCksumVerP and headCksumLenP
 * is filled, the function will return failure if the checksum version
 * and length in the logPage differ.
 *
 * RETURNS
 *   1 - log page consistent
 *   0 - log page data inconsisteny
 */
static int ValidLogPageChecksum(char *logPage,
				long logPageSize,
				unsigned char *headCksumVerP,
				unsigned char *headCksumLenP)
{
    int result=0;
    int code = 0;

    unsigned char storedCksumVer;
    char storedCksum[LOGREC_CKSUM_MAX_LEN];
    char calcCksum[LOGREC_CKSUM_MAX_LEN];
    int storedCksumLen, calcCksumLen;
    static int unknownCksumTypeMsg = 0;

    code = RetrieveLogPageChecksum(logPage, storedCksum, &storedCksumVer,
				   &storedCksumLen);
#if 0
    /* Ensure checksum version consistency across all log pages.
     *
     * Currently turned off.
     */
    if ((headCksumVerP && (*headCksumVerP != storedCksumVer)) ||
	(headCksumLenP && (*headCksumLenP != storedCksumLen))) {
	return result;            /* failure */
    }
#endif
    if (!code) {
	code = CalcLogPageChecksum(logPage, logPageSize,
				   calcCksum, &calcCksumLen);

	/* The following check should always succeed. If it does not
	 * we return an invalid checksum error */
	if (!code) {
	    if ((storedCksumLen == calcCksumLen) &&
		SameChecksum(storedCksum, calcCksum, storedCksumLen)) {
		result = 1;
	    }
	}
    } else {
	if (code & BAD_CKSUM_LEN) {
	    /* The checksum length recorded on disk does not match the checksum
	     * version recorded on disk */
	    printf("Log has bad checksum length\n");
	} else if (code & UNKNOWN_CKSUM_TYPE) {
	    /* Unknown checksum type recorded on disk. Force success of
	     * checksum validity check */
	    result = 1;
	    if (!unknownCksumTypeMsg) {
		printf("Log has unknown checksum type on disk. Ignoring checksum check\n");
		unknownCksumTypeMsg++;
	    }
	} else {
	    printf("Unable to retrieve checksum in log page\n");
	}
    }
    return result;
}

/*
 * In-kernel recovery is no longer present, although I've left the code used
 * on the RS/6000 around.  In kernel mode, attempts to recover always fail.
 */

/*
 * elbr_WriteCheckpointBlock -- write a checkpoint block.  Called from both
 * user and kernel code.
 *
 * This function was added to close a window of vulnerability that was open
 * while writing the restart block.  If the restart block overwrites a block
 * of the active log, and the machine crashes in the middle of writing the
 * restart block, the log becomes unusable.  So before writing the restart
 * block, we append a checkpoint block to the log.  This cannot overwrite a
 * block of the active log, so if the machine crashes while we are writing
 * the checkpoint block, there's no problem.  If we successfully write a
 * checkpoint block, the checkpoint block itself becomes the entire active
 * region of the log.  Then, if the machine subsequently crashes while writing
 * a restart block, there are two possibilities:  (1) the restart block
 * doesn't clobber the checkpoint block.  Recovery will use the checkpoint
 * block; (2) the restart block clobbers the checkpoint block.  Recovery will
 * use the region that used to be the active region, i.e. the region prior to
 * the checkpoint block.  In either case, the log is usable (recovery can
 * proceed).
 */
u_int32 elbr_WriteCheckpointBlock (
     IN async_device_t *dP,
     IN daddr_t blockNum,
     IN struct logRec *ckptRecord,
     IN unsigned passNumber,
     IN unsigned pageNumber,
     IN unsigned char cksumVer)
{
      struct logPageHdr pgHdr;
      caddr_t dataP, dataBufP;
      asevent_event_t *eventP;
      struct buf *bp;
      long retval;

      /* firewall */
      afsl_Assert(((sizeof(struct logPageHdr) + sizeof(struct logRec)) <
		   dP->blkSize) &&
		  (dP->blkSize < MAX_ALLOC_SIZE));

      /* initialize */
      elbr_InitLogPageHdr(&pgHdr, cksumVer);
      pgHdr.passNumber = (u_int16)passNumber;
      pgHdr.pageNumber = pgHdr.oldestDataPage = pgHdr.oldestTranPage = pageNumber;
      pgHdr.lastLogRecOff = elbr_FirstLogRecordOffset(&pgHdr);
      pgHdr.timeWritten = osi_Time();

      /* now, pack the buffer */
      dataBufP = osi_Alloc_r(dP->blkSize);
      logbuf_pin(dataBufP, dP->blkSize);
      dataP = dataBufP;
      bcopy((caddr_t)&pgHdr, dataP, sizeof pgHdr);
      dataP += sizeof pgHdr;

      /* CksumLength() can return 0 if no checksums are being used. */
      dataP += GetChecksumLength( (int)(pgHdr.cksum_ver));

      /* Positioned at first log record offset now */
      bcopy((caddr_t)&ckptRecord->hdr, dataP,
	    sizeof (ckptRecord->hdr));
      dataP += sizeof ckptRecord->hdr;
      LOGCOPY((caddr_t)&ckptRecord->lsn, dataP,
	    sizeof (ckptRecord->lsn));

      /*
       * Calculate checksum.
       */
      elbr_ChecksumLogPage (dataBufP, dP->blkSize);

      /*
       * Write out the buffer
       */
      eventP = asevent_Create(ASEVENT_NOPIN);
      bp = asevent_MakeBuf(
		B_WRITE, dataBufP, dP->blkSize, dP, blockNum, 0);
      retval = asevent_QueueIO(dP, bp, eventP);
      if (retval == 0)
	 retval = asevent_WaitEvent(eventP);
      asevent_Release(eventP);
      if (retval)
	  return RCVR_FAILED;

      logbuf_unpin(dataBufP, dP->blkSize);
      osi_Free_r((opaque)dataBufP, (dP)->blkSize);

      return BUFFER_OK;
}

/* elbr_WriteRestartBlock -- write a restart block.  Called from both user and
 *     kernel code. */
/* EXPORT */
u_int32 elbr_WriteRestartBlock (
     IN async_device_t *dP,
     IN daddr_t blockNum,
     IN struct logRec *restartRecord,
     IN unsigned passNumber,
     IN unsigned char cksumVer)
{
      struct logPageHdr pgHdr;
      caddr_t dataP, dataBufP;
      asevent_event_t *eventP;
      struct buf *bp;
      long retval;

      /* firewall */
      afsl_Assert(((sizeof(struct logPageHdr) + sizeof(struct logRec)) <
		   dP->blkSize) &&
		  (dP->blkSize < MAX_ALLOC_SIZE));

      /* Validate checksum version and switch to default version type
       * if specified type is unknown */
      if (!KnownChecksumType(cksumVer)) {
	 printf("Log is using unknown checksum version %u, Setting to use version %u\n",
		cksumVer, defaultLogChecksumVersion);
	 cksumVer = (unsigned char) defaultLogChecksumVersion;
      }

      /* initialize */
      elbr_InitLogPageHdr(&pgHdr, cksumVer);
      pgHdr.passNumber = (u_int16)passNumber;
      pgHdr.lastLogRecOff = elbr_FirstLogRecordOffset(&pgHdr);
      pgHdr.timeWritten = osi_Time();

      /* now, pack the buffer */
      dataBufP = osi_Alloc_r(dP->blkSize);
      logbuf_pin(dataBufP, dP->blkSize);
      dataP = dataBufP;
      bcopy((caddr_t)&pgHdr, dataP, sizeof pgHdr);
      dataP += sizeof pgHdr;

      /* Zero checksum in log page header */
      bzero(dataP, (int)(pgHdr.cksum_len));
      dataP += (int)(pgHdr.cksum_len);

      /* Positioned at first log record offset now */
      bcopy((caddr_t)&restartRecord->hdr, dataP,
	    sizeof (restartRecord->hdr));
      dataP += sizeof restartRecord->hdr;
      LOGCOPY((caddr_t)&restartRecord->lsn, dataP,
	    sizeof (restartRecord->lsn));

      /*
       * We choose not to checksum the log page containing a restart record
       * only. Write the log page without checksum to disk.
       */

      eventP = asevent_Create(ASEVENT_NOPIN);
      bp = asevent_MakeBuf(B_WRITE, dataBufP, dP->blkSize, dP, blockNum, 0);
      retval = asevent_QueueIO(dP, bp, eventP);
      if (retval == 0)
	 retval = asevent_WaitEvent(eventP);
      asevent_Release(eventP);
      if (retval)
	  return RCVR_FAILED;

      logbuf_unpin(dataBufP, dP->blkSize);
      osi_Free_r((opaque)dataBufP, (dP)->blkSize);

      return BUFFER_OK;
}

/*
 * elbr_GetCleanLogInfo - pull the header off a clean log.
 *
 * returns 0 if not clean (needs recovery), 1 if clean.
 * The log page hdr structure ptr (if non-zero) is
 * filled in by this routine.
 */
/* EXPORT */
int elbr_GetCleanLogInfo(
  IN async_device_t *dP,
  IN daddr_t rstrtBlock,
  INOUT opaque xlphP) /* caller provides space */
{
      caddr_t dbP;
      asevent_event_t *ae;
      long code;
      struct logPageHdr *lphP;
      struct logRec *recP;
      unsigned char result;
      struct buf *bp;
      int cksumLength;

      /* allocate space for the restart block */
      dbP = osi_Alloc_r(dP->blkSize);
      logbuf_pin(dbP, dP->blkSize);
      lphP = GetPageHdr(dbP);

      /* create an event structure */
      ae = asevent_Create(ASEVENT_NOPIN);
      bp = asevent_MakeBuf(B_READ, dbP, dP->blkSize, dP, rstrtBlock, 0);
      code = asevent_QueueIO(dP, bp, ae);
      if (code == 0)
	 code = asevent_WaitEvent(ae);
      asevent_Release(ae);

      /*
       * we dump a complete listing of the page header when
       * recovery debugging is enabled.
       */
      if (code) {
	  DBprint(("I/O error %d reading restart block %u", code, rstrtBlock));
	  result = 0;
	  goto done;
      }

      DBprint(("Restart Record page header:"));
      DBprint(("                   pass #: %u", lphP->passNumber));
      DBprint(("                   page #: %u", lphP->pageNumber));
      DBprint(("last log record at offset: %u", lphP->lastLogRecOff));
      DBprint(("               salvage ID: %u", lphP->salvageID));
      DBprint(("         oldest data page: %u", lphP->oldestDataPage));
      DBprint(("         oldest tran page: %u", lphP->oldestTranPage));
      DBprint(("             time written: %u", lphP->timeWritten));
      DBprint(("         checksum version: %u", lphP->cksum_ver));
      DBprint(("          checksum length: %u", lphP->cksum_len));

      if (lphP->salvageID != LOG_SALVAGE_MAGIC_NUMBER) {
	  result = 0;
	  goto done;
      }

      /*
       * Always return the log page header if the caller has
       * provided the space for it
       */
      if (xlphP != NULL) {
	  *(struct logPageHdr *)xlphP = *lphP;
      }

      /* Check validity of checksum version. We do not check validity of the
       * checksum itself as the log page with only the restart record is not
       * "checksummed" */
      code = RetrieveLogPageChecksum(dbP, 0, 0, &cksumLength);
      if (code & ~UNKNOWN_CKSUM_TYPE) {
	  result = 0;
	  goto done;
      }

      /*
       * now we peek at the first log record and see if it is
       * a restart record.  If not, this isn't a clean log.
       */
      recP = (struct logRec *)(dbP + elbr_FirstLogRecordOffset(lphP));
      if (recP->hdr.recType != LOGREC_RSTRT) {
	  result = 0;
      } else {
	  result = 1;

	  /* If caller has provided space for log page header
	   * convert unknown checksum version to a known one,
	   * to be used for the aggregate after attach */
	  if ((code & UNKNOWN_CKSUM_TYPE) && xlphP) {
	      printf("Log is set to use an unknown checksum version %u, Changing to use version %u\n",
		     lphP->cksum_ver, defaultLogChecksumVersion);
	      ((struct logPageHdr *)xlphP)->cksum_ver =
		  (unsigned char) defaultLogChecksumVersion;
	  }

      }

done:
      /* Cleanup */
      logbuf_unpin(dbP, dP->blkSize);
      osi_Free_r((opaque)dbP, dP->blkSize);

      return result;
}

/*
 * elbr_InitLogPageHdr - initialize a log page header. All fields except
 * salvage_ID, cksum_ver and cksum_len is set to zero.
 */
/* SHARED */
void elbr_InitLogPageHdr(
     IN struct logPageHdr *pgHdrP,
     IN unsigned char cksumVer)
{
    pgHdrP->passNumber = (u_int16)0;
    pgHdrP->lastLogRecOff = (u_int16)0;
    pgHdrP->pageNumber = 0;
    pgHdrP->salvageID = LOG_SALVAGE_MAGIC_NUMBER;
    pgHdrP->oldestDataPage = (u_int16)0;
    pgHdrP->oldestTranPage = (u_int16)0;
    pgHdrP->timeWritten = 0;
    pgHdrP->reserved[0] = 0;
    pgHdrP->reserved[1] = 0;
    pgHdrP->reserved[2] = 0;
    pgHdrP->io_window = (u_int16)0;
    pgHdrP->cksum_ver = cksumVer;
    pgHdrP->cksum_len = GetChecksumLength(cksumVer);
}

/* SHARED */
void elbr_ChecksumLogPage(char *logPage, long logPageSize)
{
    char cksum[LOGREC_CKSUM_MAX_LEN];
    int cksumLength;
    int code = 0;

    /* Calculate checksum */
    code = CalcLogPageChecksum(logPage, logPageSize, cksum, &cksumLength);
    afsl_MBZ(code);

    /* Store Checksum */
    StoreLogPageCksum(logPage,cksum,cksumLength);
}

/* SHARED */
u_int16 elbr_FirstLogRecordOffset(IN const struct logPageHdr *pgHdrP)
{
    u_int16 offset = 0;
    int cksumLength = 0;
    int code = 0;


    code = RetrieveLogPageChecksumAttributes(pgHdrP,
					     0 /* no checksum version */,
					     &cksumLength);
    afsl_Assert(!(code & BAD_CKSUM_LEN));
    offset = (u_int16)sizeof(struct logPageHdr) + (u_int16)cksumLength;
    return offset;
}


