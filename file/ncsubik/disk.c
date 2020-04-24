/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: disk.c,v $
 * Revision 1.1.72.1  1996/10/02  17:55:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:27  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1990, 1996 Transarc Corporation - All rights reserved */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <sys/types.h>

#include <dce/rpc.h>
#include <pthread.h>
#include <ubik_data.h>
#include <dcedfs/icl.h>
#include <dcedfs/compat.h>
#include <dcedfs/bomb.h>
#define UBIK_INTERNALS
#include <ubik.h>
#include <utrace.h>

#define	PHSIZE	128
struct buffer  {
    struct ubik_dbase *dbase;	    /* dbase within which the buffer resides */
    long file;			    /* Unique cache key */
    long page;			    /* page number */
    long accesstime;		    /* when accessed */
    struct buffer *hashNext;	    /* next dude in hash table */
    char *data;			    /* ptr to the data */
    char lockers;		    /* usage ref count */
    char dirty;			    /* is buffer modified */
    char hashIndex;		    /* back ptr to hash table */
} *Buffers;

#define pHash(fid) ((fid) & (PHSIZE-1))

/* round up integer N to nearest multiple of M */
#define ROUND_UP(N,M) (((int)((N)+(M)-1)/(int)(M))*(int)(M))

long ubik_nBuffers = UBIK_NBUFFERS;
static struct buffer *phTable[PHSIZE];	/* page hash table */
static struct buffer *LastBuffer;
static int nbuffers;
static int timecounter;
static int calls=0, ios=0;
static char *BufferData;
static struct buffer *newslot();
static initd = 0;
#define	BADFID	    0xffffffff

int ubik_maxbulk = UBIK_MAXBULK;	/* max write vector size */

static struct ubik_trunc *freeTruncList=0;
static DTrunc();
extern struct icl_set *ubikdisk_iclSetp;

/* Remove a transaction from the database's active transaction list.  Don't
 * free it */
static unthread(atrans)
    struct ubik_trans *atrans;
{
    struct ubik_trans **lt, *tt;
    lt = &atrans->dbase->activeTrans;
    for(tt = *lt; tt; lt = &tt->next, tt = *lt) {
	if (tt == atrans) {
	    /* found it */
	    *lt = tt->next;
	    return 0;
	}
    }
    return 2;	/* no entry */
}

/* some debugging assistance */
udisk_Debug(aparm)
    ubik_debug *aparm;
{
    struct buffer *tb;
    int i;

    aparm->localVersion = *((struct net_version *) &ubik_dbase->version);
    aparm->lockedPages = 0;
    aparm->writeLockedPages = 0;
    tb = Buffers;
    for(i=0;i<nbuffers;i++, tb++) {
	if (tb->lockers) {
	    aparm->lockedPages++;
	    if (tb->dirty) aparm->writeLockedPages++;
	}
    }
}

/* allocate a truncation entry.  We allocate special entries representing
 * truncations, rather than performing them immediately, so that we can abort a
 * transaction easily by simply purging the in-core memory buffers and
 * discarding these truncation entries.
*/
static struct ubik_trunc *GetTrunc() {
    struct ubik_trunc *tt;
    if (!freeTruncList) {
	freeTruncList = (struct ubik_trunc *)osi_Alloc(sizeof(struct ubik_trunc));
	if ( !freeTruncList )
	    return((struct ubik_trunc *) 0);
	freeTruncList->next = (struct ubik_trunc *) 0;
    }
    tt = freeTruncList;
    freeTruncList = tt->next;
    return tt;
}

/* free a truncation entry */
static PutTrunc(at)
    struct ubik_trunc *at;
{
    at->next = freeTruncList;
    freeTruncList = at;
    return 0;
}

/* find a truncation entry for a file, if any */
static struct ubik_trunc *FindTrunc(atrans, afile)
    struct ubik_trans *atrans;
    long afile;
{
    struct ubik_trunc *tt;
    for(tt=atrans->activeTruncs; tt; tt=tt->next) {
	if (tt->file == afile) return tt;
    }
    return (struct ubik_trunc *) 0;
}

/* do truncates associated with trans, and free them */
static DoTruncs(atrans)
    struct ubik_trans *atrans;
{
    long code;
    struct ubik_trunc *tt;
    int (*tproc)();

    tproc = atrans->dbase->truncate;
    for(tt = atrans->activeTruncs; tt; tt=tt->next) {
	/* zap pages from buffer cache */
	DTrunc(atrans->dbase, tt->file,	tt->length);
	code = (*tproc)(atrans->dbase, tt->file, tt->length);
	if (code) return code;
    }
    FreeTruncs(atrans);
    return 0;
}

/* free truncates associated with trans */
static FreeTruncs(atrans)
    struct ubik_trans *atrans;
{
    struct ubik_trunc *tt, *nt;

    for(tt = atrans->activeTruncs; tt; tt=nt) {
	nt = tt->next;
	PutTrunc(tt);
    }
    atrans->activeTruncs = (struct ubik_trunc *) 0;
    return 0;
}

/* log format is defined here, and implicitly in recovery.c
 *
 * 4 byte opcode, followed by parameters, each 4 bytes long.  All integers
 * are in logged in network standard byte order, in case we want to move logs
 * from machine-to-machine someday.
 *
 * Begin transaction: opcode
 * Commit transaction: opcode, version (8 bytes)
 * Truncate file: opcode, file number, length
 * Abort transaction: opcode
 * Write data: opcode, file, position, length, <length> data bytes
 */

/* write an opcode to the log */
udisk_LogOpcode(adbase, aopcode, async)
    struct ubik_dbase *adbase;
    long aopcode;
    int async;
{
    struct ubik_stat ustat;
    long code;

    icl_Trace2(ubikdisk_iclSetp,UK_TRACE_DISK_LOGOPCODE,ICL_TYPE_LONG, aopcode,
	       ICL_TYPE_LONG, async);
    /* figure out where to write */
    code = (*adbase->stat)(adbase, LOGFILE, &ustat);
    if (code) return code;

    /* setup data and do write */
    aopcode = htonl(aopcode);
    code = (*adbase->write)(adbase, LOGFILE, &aopcode, ustat.size,
			    sizeof(long));
    if (code)
        return code;

    /* optionally sync data */
    if (async) code = (*adbase->sync)(adbase, LOGFILE);
    else code = 0;
    return code;
}

/* log a commit, never syncing */
udisk_LogEnd(adbase, aversion)
    struct ubik_dbase *adbase;
    struct ubik_version *aversion;
{
    long code;
    long data[3];
    struct ubik_stat ustat;
    
    if (aversion)
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_DISK_LOGEND,
		 ICL_TYPE_LONG, aversion->epoch, ICL_TYPE_LONG,
		 aversion->counter);
    /* figure out where to write */
    code = (*adbase->stat)(adbase, LOGFILE, &ustat);
    if (code) return code;

    /* setup data */
    data[0] = htonl(LOGEND);
    data[1] = htonl(aversion->epoch);
    data[2] = htonl(aversion->counter);
    
    /* do write */
    code = (*adbase->write)(adbase, LOGFILE, data, ustat.size, 3*sizeof(long));
    if (code) 
        return code;

    /* finally sync the log */
    code = (*adbase->sync)(adbase, LOGFILE);
    return code;
}
    
/* log a truncate operation, never syncing */
udisk_LogTruncate(adbase, afile, alength)
    struct ubik_dbase *adbase;
    long afile, alength;
{
    long code;
    long data[3];
    struct ubik_stat ustat;
    
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_DISK_LOGTRUNCATE, 
	       ICL_TYPE_LONG, afile, ICL_TYPE_LONG, alength);
    /* figure out where to write */
    code = (*adbase->stat)(adbase, LOGFILE, &ustat);
    if (code) return code;

    /* setup data */
    data[0] = htonl(LOGTRUNCATE);
    data[1] = htonl(afile);
    data[2] = htonl(alength);
    
    /* do write */
    code = (*adbase->write)(adbase, LOGFILE, data, ustat.size, 3*sizeof(long));
    return code;
}
    
/* write some data to the log, never syncing */
udisk_LogWriteData(adbase, afile, abuffer, apos, alen)
    struct ubik_dbase *adbase;
    char *abuffer;
    long afile;
    long apos;
    long alen;
{
    struct ubik_stat ustat;
    long code;
    long data[4];
    long lpos;
    
    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_DISK_LOGWRITE,
	       ICL_TYPE_LONG, afile, ICL_TYPE_POINTER, abuffer, 
	       ICL_TYPE_LONG, apos, ICL_TYPE_LONG, alen);
    /* find end of log */
    code = (*adbase->stat)(adbase, LOGFILE, &ustat);
    lpos = ustat.size;
    if (code) return code;

    /* setup header */
    data[0] = htonl(LOGDATA);
    data[1] = htonl(afile);
    data[2] = htonl(apos);
    data[3] = htonl(alen);

    /* write header */
    code = (*adbase->write)(adbase, LOGFILE, data, lpos, 4*sizeof(long));
    if (code)
        return code;

    lpos += 4*sizeof(long);
    
    /* write data */
    code = (*adbase->write)(adbase, LOGFILE, abuffer, lpos, alen);
    return code;
}

static int DInit (abuffers)
    int abuffers;
{
    /* Initialize the venus buffer system. */
    int i;
    struct buffer *tb;
    Buffers = (struct buffer *) osi_Alloc(abuffers * sizeof(struct buffer));
    if ( !Buffers )
	return UNOMEM;
    bzero((char *)Buffers, abuffers * sizeof(struct buffer));
    BufferData = (char *) osi_Alloc(abuffers * UBIK_PAGESIZE);
    if ( !BufferData ) {
	osi_Free(Buffers, abuffers * sizeof(struct buffer));
	return UNOMEM;
    }
    timecounter = 0;
    LastBuffer = Buffers;
    nbuffers = abuffers;
    for(i=0;i<PHSIZE;i++) phTable[i] = 0;
    for (i=0;i<abuffers;i++) {
        /* Fill in each buffer with an empty indication. */
	tb = &Buffers[i];
        tb->data = &BufferData[UBIK_PAGESIZE*i];
	tb->file = BADFID;
    }
    return 0;
}

/* get a pointer to a particular buffer */
static char *DRead(dbase, fid, page)
    struct ubik_dbase *dbase;
    long fid;
    int page;
{
    /* Read a page from the disk. */
    struct buffer *tb;
    calls++;
    if (LastBuffer->page == page && LastBuffer->file == fid &&
	LastBuffer->dbase == dbase) {
	tb = LastBuffer;
	tb->accesstime = ++timecounter;
	tb->lockers++;
	return tb->data;
    }
    for(tb=phTable[pHash(fid)]; tb; tb=tb->hashNext) {
	if (tb->page == page && tb->file == fid && tb->dbase == dbase) {
	    tb->lockers++;
	    tb->accesstime = ++timecounter;
	    LastBuffer = tb;
	    return tb->data;
	}
    }
    /* can't find it */
    tb = newslot(dbase, fid, page);
    if (!tb) return 0;
    bzero(tb->data, UBIK_PAGESIZE);
    tb->lockers++;
    if ((*dbase->read)(dbase, fid, tb->data, page*UBIK_PAGESIZE,
		       UBIK_PAGESIZE)) {
	tb->file = BADFID;
	tb->lockers--;
        return 0;
    }
    ios++;
    /* Note that findslot sets the page field in the buffer equal to what it is
     * searching for. */
    return tb->data;
}

/* zap truncated pages */
static DTrunc(dbase, fid, length)
    struct ubik_dbase *dbase;
    long fid;
    long length;
{
    long maxPage;
    struct buffer *tb;
    
    /* first invalid page now in file */
    maxPage = (length+UBIK_PAGESIZE-1)>>UBIK_LOGPAGESIZE;
    for(tb = phTable[pHash(fid)]; tb; tb=tb->hashNext) {
	if (tb->page >= maxPage && tb->file == fid && tb->dbase == dbase) {
	    tb->file = BADFID;
	}
    }
    return 0;
}

/* mark a fid as invalid */
udisk_Invalidate(adbase, afid)
  struct ubik_dbase *adbase;
  long afid;
{
    struct buffer *tb;
    for(tb = phTable[pHash(afid)]; tb; tb=tb->hashNext) {
	if (tb->file == afid) {
	    tb->file = BADFID;
	}
    }
    return 0;
}

/* move this page into the correct hash bucket */
static FixupBucket(ap)
    struct buffer *ap;
{
    struct buffer **lp, *tp;
    int i;
    /* first try to get it out of its current hash bucket, in which it might
     * not be */
    i = ap->hashIndex;
    lp = &phTable[i];
    for(tp = *lp; tp; tp=tp->hashNext) {
	if (tp == ap) {
	    *lp = tp->hashNext;
	    break;
	}
	lp = &tp->hashNext;
    }
    /* now figure the new hash bucket */
    i = pHash(ap->file);
    ap->hashIndex = i;		/* remember where we are for deletion */
    ap->hashNext = phTable[i];	/* add us to the list */
    phTable[i] = ap;
}

/* create a new slot for a particular dbase page */
static struct buffer *newslot (adbase, afid, apage)
    struct ubik_dbase *adbase;
    long afid, apage;
{
    /* Find a usable buffer slot */
    long i;
    long pt;
    struct buffer *pp, *tp;

    pp = 0;		/* last pure */
    pt = 999999999;
    tp = Buffers;
    for (i=0;i<nbuffers;i++,tp++) {
	if (tp->lockers == 0) {
	    if (!tp->dirty && tp->accesstime < pt) {
		pp = tp;
		pt = tp->accesstime;
	    }
	}
    }

    if (pp == 0) {
        /* There are no unlocked buffers that don't need to be written to the
         * disk. */
	return (struct buffer *) 0;
    }

    /* Now fill in the header. */
    pp->dbase = adbase;
    pp->file = afid;
    pp->page = apage;
    pp->accesstime = ++timecounter;

    FixupBucket(pp);		/* move to the right hash bucket */

    LastBuffer = pp;
    return pp;
}

/* Release a buffer, specifying whether or not the buffer has been modified by
 * the locker. */
static DRelease (ap,flag)
    char *ap;
    int flag;
{
    int index;
    struct buffer *bp;

    if (!ap) return;
    index = (ap - (char *)BufferData) >> UBIK_LOGPAGESIZE;
    bp = &(Buffers[index]);
    bp->lockers--;
    if (flag) bp->dirty=1;
    return 0;
}

/* flush all modified buffers, leaves dirty bits set (they're cleared by
 * DSync).  Note interaction with DSync: you call this thing first, writing the
 * buffers to the disk.  Then you call DSync to sync all the files that were
 * written, and to clear the dirty bits.  You should always call DFlush/DSync
 * as a pair. */
static DFlush (adbase)
    struct ubik_dbase *adbase;
{
    int i;
    long code;
    struct buffer *tb;

    tb = Buffers;
    for(i=0;i<nbuffers;i++,tb++) {
        if (tb->dirty) {
	    code = tb->page * UBIK_PAGESIZE;	/* offset within file */
	    code = (*adbase->write)(adbase, tb->file, tb->data, code,
				    UBIK_PAGESIZE);
	    if (code)
	        return code;
	}
    }
    return 0;
}

/* flush all modified buffers */
static DAbort (adbase)
    struct ubik_dbase *adbase;
{
    int i;
    struct buffer *tb;

    tb = Buffers;
    for(i=0;i<nbuffers;i++,tb++) {
        if (tb->dirty) {
	    tb->dirty = 0;
	    tb->file = BADFID;
	}
    }
    return 0;
}

/* must only be called after DFlush, due to its interpretation of dirty flag */
static DSync(adbase)
    struct ubik_dbase *adbase;
{
    int i;
    long code;
    struct buffer *tb;
    long file;
    long rCode;

    rCode = 0;
    while (1) {
	file = BADFID;
	for(i=0,tb = Buffers; i<nbuffers; i++,tb++) {
	    if (tb->dirty == 1) {
		if (file == BADFID) file = tb->file;
		if (file != BADFID && tb->file == file) tb->dirty = 0;
	    }
	}
	if (file == BADFID) break;
	/* otherwise we have a file to sync */
	code = (*adbase->sync)(adbase, file);
	if (code) rCode = code;
    }
    return rCode;
}

/* Same as read, only do not even try to read the page */
static char *DNew (dbase, fid, page)
    struct ubik_dbase *dbase;
    int page;
    long fid;
{
    struct buffer *tb;

    if ((tb = newslot(dbase, fid, page)) == 0) return (char *) 0;
    tb->lockers++;
    bzero(tb->data, UBIK_PAGESIZE);
    return tb->data;
}

/* read data from database */
udisk_read(atrans, afile, abuffer, apos, alen)
    long afile;
    char *abuffer;
    long apos, alen;
    struct ubik_trans *atrans;
{
    char *bp;
    long offset, len, totalLen;
    struct ubik_dbase *dbase;

    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_DISK_READ, ICL_TYPE_POINTER, atrans);
    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_DISK_READ_PARAMS, ICL_TYPE_LONG, 
	       afile, ICL_TYPE_POINTER, abuffer, ICL_TYPE_LONG, apos,
	       ICL_TYPE_LONG, alen);
    if (atrans->flags & TRDONE) return UDONE;
    totalLen = 0;
    dbase = atrans->dbase;
    while (alen > 0) {
	bp = DRead(dbase, afile, apos>>UBIK_LOGPAGESIZE);
	if (!bp) return UEOF;
	/* otherwise, min of remaining bytes and end of buffer to user mode */
	offset = apos & (UBIK_PAGESIZE-1);
	len = UBIK_PAGESIZE - offset;
	if (len > alen) len = alen;
	bcopy(bp+offset, abuffer, len);
	abuffer += len;
	apos += len;
	alen -= len;
	totalLen += len;
	DRelease(bp, 0);
    }
    return 0;
}

/* truncate file */
udisk_truncate(atrans, afile, alength)
    struct ubik_trans *atrans;
    long afile;
    long alength;
{
    long code;
    struct ubik_trunc *tt;
    struct ubik_size *ts;

    icl_Trace3(ubikdisk_iclSetp, UK_TRACE_DISK_READ, 
	       ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, afile,
	       ICL_TYPE_LONG, alength);
    if (atrans->type != UBIK_WRITETRANS) return UBADTYPE;
    if (atrans->flags & TRDONE) return UDONE;

    /* write a truncate log record */
    code = udisk_LogTruncate(atrans->dbase, afile, alength);

    /* don't truncate until commit time */
    tt = FindTrunc(atrans, afile);
    if (!tt) {
	/* this file not truncated yet */
	tt=GetTrunc();
	if ( !tt ) return(UNOMEM);
	tt->next = atrans->activeTruncs;
	atrans->activeTruncs = tt;
	tt->file = afile;
	tt->length = alength;
    }
    else {
	/* already truncated to a certain length */
	if (tt->length > alength) tt->length = alength;
    }

    return code;
}

/* write data to database, using logs */
udisk_write(atrans, afile, abuffer, apos, alen)
    long afile;
    char *abuffer;
    long apos, alen;
    struct ubik_trans *atrans;
{
    char *bp;
    long offset, len, totalLen;
    struct ubik_dbase *dbase;
    struct ubik_trunc *tt;
    long code;

    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_DISK_WRITE, 
	       ICL_TYPE_POINTER, atrans);
    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_DISK_WRITE_PARAMS,
	       ICL_TYPE_LONG, afile,
	       ICL_TYPE_POINTER, abuffer, ICL_TYPE_LONG, apos,
	       ICL_TYPE_LONG, alen);
    if (atrans->type != UBIK_WRITETRANS) return UBADTYPE;
    if (atrans->flags & TRDONE) return UDONE;

    dbase = atrans->dbase;
    /* first write the data to the log */
    code = udisk_LogWriteData(dbase, afile, abuffer, apos, alen);
    if (code) return code;

    /* expand any truncations of this file */
    tt = FindTrunc(atrans, afile);
    if (tt) {
	if (tt->length < apos + alen) {
	    tt->length = apos + alen;
	}
    }

    /* now update vm */
    totalLen = 0;
    while (alen > 0) {
	bp = DRead(dbase, afile, apos>>UBIK_LOGPAGESIZE);
	if (!bp) {
	    bp = DNew(dbase, afile, apos>>UBIK_LOGPAGESIZE);
	    if (!bp) return UIOERROR;
	    bzero(bp, UBIK_PAGESIZE);
	}
	/* otherwise, min of remaining bytes and end of buffer to user mode */
	offset = apos & (UBIK_PAGESIZE-1);
	len = UBIK_PAGESIZE-offset;
	if (len > alen) len = alen;
	bcopy(abuffer, bp+offset, len);
	abuffer += len;
	apos += len;
	alen -= len;
	totalLen += len;
	DRelease(bp, 1);    /* buffer modified */
    }
    return 0;
}

/* begin a new local transaction */
udisk_begin(adbase, atype, atrans)
    struct ubik_trans **atrans;
    int atype;
    struct ubik_dbase *adbase;
{
    long code;
    struct ubik_trans *tt;
    long allocsize;

    *atrans = (struct ubik_trans *)NULL;
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_DISK_BEGIN, ICL_TYPE_LONG, atype);
    /* Make sure system is initialized before doing anything */
    if (!initd) {
	initd = 1;
	DInit(ubik_nBuffers);
    }
    if (atype == UBIK_WRITETRANS) {
	if (adbase->flags & DBWRITING) return USYNC;
	code = udisk_LogOpcode(adbase, LOGNEW, 0);
	if (code) return code;

	/* allocate single chunk of memory for transaction and bulk updates */
	allocsize = ROUND_UP(sizeof(struct ubik_trans),sizeof(double)) +
		    2 * sizeof(ubik_bulk_update) * ubik_maxbulk;
        tt = (struct ubik_trans *) osi_Alloc(allocsize);
	if ( !tt ) return(UNOMEM);
        bzero((char *)tt, sizeof(struct ubik_trans));
	tt->allocsize = allocsize;
	tt->bulk = (ubik_bulk_update *)
	    ((char *)tt + ROUND_UP(sizeof(struct ubik_trans),sizeof(double)));
	tt->rpc_bulk = tt->bulk + ubik_maxbulk;
        tt->nbulk = 0;
        tt->rpc_nbulk = 0;
    } else {
        tt = (struct ubik_trans *) osi_Alloc(sizeof(struct ubik_trans));
	if ( !tt ) return(UNOMEM);
        bzero((char *)tt, sizeof(struct ubik_trans));
	tt->allocsize = sizeof(struct ubik_trans);
    }
    tt->RpcRunning = 0;
    tt->RpcFlag = 0;
    tt->dbase = adbase;
    tt->next = adbase->activeTrans;
    adbase->activeTrans = tt;
    tt->type = atype;
    if (atype == UBIK_READTRANS) adbase->readers++;
    else if (atype == UBIK_WRITETRANS) {
      adbase->flags |= DBWRITING;
      icl_Trace1(ubikdisk_iclSetp, UK_TRACE_DISK_WRITING,
		 ICL_TYPE_LONG, adbase->flags);
    }
    *atrans = tt;
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_DISK_BEGIN_EXIT, ICL_TYPE_POINTER,
	       *atrans);
    return 0;
}

/* commit transaction */
udisk_commit(atrans)
    struct ubik_trans *atrans;
{
    struct ubik_dbase *dbase;
    char panic_str[80];
    long code;

    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_DISK_COMMIT,
	       ICL_TYPE_POINTER, atrans);
    if (atrans->flags & TRDONE) return UTWOENDS;
    atrans->flags |= TRDONE;
    dbase = atrans->dbase;
    /* do this first, we don't get a second chance */
    if (atrans->type == UBIK_WRITETRANS) {
	dbase->version.counter++;	/* bump commit count */
	osi_Wakeup((opaque)&dbase->version);
	code = udisk_LogEnd(dbase, &dbase->version);
	if (code) {
	    return(code);
	}

	/*
	 * When we get here, we have written the log file and
	 * are about to begin making changes to the database file.
	 * The transaction is committed.  THERE IS NO GOING BACK.
	 * If we get an error after this point, then the main database
	 * file is probably corrupted, and the only repair is to
	 * complete the transaction by playing the log, or to get
	 * a new database file from one of the other servers.
	 * 
	 * The recovery code looks for a complete log file during
	 * initialization, and the server will not start until
	 * the database has been repaired. We panic if we cannot
	 * complete the commit, and let the recovery code do its
	 * thing. It would be a serious mistake to try to use a
	 * database file with a partially committed transaction.
	 * The server must stay down until the problem is corrected.
	 */
	code = DFlush(dbase);    /* clear all modified flags */
	if (code) {
	    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_COMMIT_PANIC,
		       ICL_TYPE_STRING, "DFlush", ICL_TYPE_LONG, code);
	    sprintf(panic_str,"DFlush failed during commit: code=%d",code);
	    afsos_panic (panic_str);
	}
	code = DSync(dbase);     /* now sync all modified pages */
	if (code) {
	    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_COMMIT_PANIC,
		       ICL_TYPE_STRING, "DSync", ICL_TYPE_LONG, code);
	    sprintf(panic_str,"DSync failed during commit: code=%d",code);
	    afsos_panic (panic_str);
	}
	code = DoTruncs(atrans); /* truncate the database files */
	if (code) {
	    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_COMMIT_PANIC,
		       ICL_TYPE_STRING, "DoTruncs", ICL_TYPE_LONG, code);
	    sprintf(panic_str,"DoTruncs failed during commit: code=%d",code);
	    afsos_panic (panic_str);
	}
	/* label the committed dbase */
	code = (*dbase->setlabel)(dbase, 0, &dbase->version);
	if (code) {
	    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_COMMIT_PANIC,
		       ICL_TYPE_STRING, "setlabel", ICL_TYPE_LONG, code);
	    sprintf(panic_str,"setlabel failed during commit: code=%d",code);
	    afsos_panic (panic_str);
	}
	code = (*dbase->truncate)(dbase, LOGFILE, 0); /* discard log (opt) */
	if (code) {
	    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_COMMIT_PANIC,
		       ICL_TYPE_STRING, "truncate", ICL_TYPE_LONG, code);
	    sprintf(panic_str,"log truncate failed during commit:code=%d",code);
	    afsos_panic (panic_str);
	}
    }
    else {
	code = 0;
    }
    return code;
}

/* abort transaction */
udisk_abort(atrans)
    struct ubik_trans *atrans;
{
    struct ubik_dbase *dbase;
    
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_DISK_ABORT, 
	       ICL_TYPE_POINTER, atrans);
    if (atrans->flags & TRDONE) return UTWOENDS;
    atrans->flags |= (TRABORT | TRDONE);
    dbase = atrans->dbase;
    /*
     * check if we are the write trans before logging abort, lest we abort
     * a good write trans in progress
     */
    if (atrans->type == UBIK_WRITETRANS && dbase->flags & DBWRITING) {
	udisk_LogOpcode(dbase, LOGABORT, 1);
	(*dbase->truncate)(dbase, LOGFILE, 0);
	DAbort(dbase);
    }
    return 0;
}

/* destroy a transaction after it has been committed or aborted.  if it hasn't
 * committed before you call this routine, we'll abort the trans for you. */
udisk_end(atrans)
    struct ubik_trans *atrans;
{
    struct ubik_dbase *dbase;

    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_DISK_END,
	       ICL_TYPE_POINTER, atrans);
    if (!(atrans->flags & TRDONE)) udisk_abort(atrans);
    dbase = atrans->dbase;
    ulock_ReleaseAllLocks(atrans);
    FreeTruncs(atrans);
    unthread(atrans);
    /*
     * check if we are the write trans before unsetting the DBWRITING bit, else
     * we could be unsetting someone else's bit.
     */
    if (atrans->type == UBIK_WRITETRANS && dbase->flags & DBWRITING) {
      dbase->flags &= ~DBWRITING;
      icl_Trace1(ubikdisk_iclSetp, UK_TRACE_DISK_NOT_WRITING,
		 ICL_TYPE_LONG, dbase->flags);
    }
    else dbase->readers--;
    osi_Free(atrans,atrans->allocsize);
    icl_Trace0(ubikdisk_iclSetp, UK_TRACE_WAKEUP_WRITER);
    osi_Wakeup((opaque)&dbase->flags); /* let others try transaction */
    return 0;
}
