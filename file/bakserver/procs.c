/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: procs.c,v $
 * $EndLog$
 */
/*
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * procs.c -- Bakserver procedures
 *
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */

/* TBD
 *	ht_lookupEntry - tape ids
 *	Truncate Tape - tape id's
 *	usetape - tape id's
 */

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <sys/resource.h>
#include <dcedfs/param.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/lock.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <netinet/in.h>
#include <dcedfs/dfsauth.h>
#include <dcedfs/dacl.h>
#include <budb_errs.h>
#include <database.h>
#include <error_macros.h>
#include <globals.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/bakserver/RCS/procs.c,v 4.79 1996/08/02 19:48:29 hyer Exp $")

extern struct 		memoryDB db;	/* incore copies of db structures */
extern buServerConfP	globalConfPtr;	/* bakserver config information */

/* Text block management */
struct memTextBlock
{
    struct memTextBlock *mtb_next;	/* next in chain */
    long   		mtb_nbytes;	/* # of bytes in this block */
    struct blockHeader 	mtb_blkHeader;	/* in memory header */
    dbadr  		mtb_addr;	/* disk address of block */
};

typedef	struct memTextBlock	memTextBlockT;
typedef	memTextBlockT		*memTextBlockP;


/*
 * The startupLock mutex protects the procsInited global variable. The global
 * variable determines if the server has reached a state where the backup
 * database has been initialized and ready to receive RPCs for the bakserver
 * interface.
 */
static osi_dlock_t startupLock;
static int procsInited = 0;
/* This variable is protected by the procsInited flag. */
static int (*rebuildDatabase)();

/*
 * The following two routines enable and disable thread cancellability. These
 * routines are used by BUDB RPCs to protect themselves against malicious
 * client cancels. Such abrupt cancels may leave the bakserver in an unstable
 * state such as locks held and resources not freed.
 */
static osi_dlock_t globalLock;
static initDone = 0;

void bak_disableCancel(rtnName, cancelState, cancelCode)
char *rtnName;
int *cancelState;
int *cancelCode;
{
  errno=0;
  *cancelCode=0;
  DFS_DISABLE_CANCEL(cancelState, *cancelCode);
  if (*cancelCode)
    Log("%s: DFS_DISABLE_CANCEL failed with code %lu, errno %lu\n",
	rtnName, *cancelCode, errno);
  LogDebug(3, "disable thread cancellation, cancel code %d\n", *cancelCode);
}

void bak_enableCancel(rtnName, cancelState, cancelCode)
char *rtnName;
int cancelState;
int *cancelCode;
{
  errno=0;
  *cancelCode=0;
  DFS_ENABLE_CANCEL(cancelState, *cancelCode);
  if (*cancelCode)
    Log("%s: DFS_ENABLE_CANCEL failed with code %lu, errno %lu\n",
	rtnName, *cancelCode, errno);
  LogDebug(3, "reenable thread cancellation, cancel code %d\n", *cancelCode);
}

/* The following two procedures serialize requests to particular BUDB RPCs.
 * They also handle disabling and enabling cancels. So when the day comes where
 * such a coarse RPC lock is not required, take care to retain the cancel
 * handling because this is always required. Servers have to maintain integrity
 * of their locks upon a cancel of client thread.
 */
void bak_Lock(rtnName, cancelState, cancelCode, doCancels)
     char *rtnName;
     int *cancelState;
     int *cancelCode;
     int doCancels;
{
    if (doCancels)  {
        LogDebug(3, "bak_Lock: disabling thread cancellation\n");
	errno=0;
	*cancelCode=0;
	DFS_DISABLE_CANCEL(cancelState, *cancelCode);
	if (*cancelCode) {
	  Log("%s: DFS_DISABLE_CANCEL failed with code %lu, errno %lu\n",
	      rtnName, *cancelCode, errno);
	  LogDebug(3, "bak_Lock: failed to disable cancels, code %d\n",
		   *cancelCode);
	  return;/* we don't lock the global lock if cancel protection fails */
	}
    }
    if (!initDone) {
      initDone=1;
      lock_Init(&globalLock);
    }
    lock_ObtainWrite(&globalLock);
    LogDebug(3, "bak_Lock: succeeded\n");
}

void bak_Unlock(rtnName, cancelState, cancelCode, doCancels)
     char *rtnName;
     int cancelState;
     int *cancelCode;
     int doCancels;
{
    lock_ReleaseWrite(&globalLock);
    if (doCancels)  {
        LogDebug(3, "bak_Unlock: reenabling thread cancellation\n");
	errno=0;
	*cancelCode=0;
	DFS_ENABLE_CANCEL(cancelState, *cancelCode);
	if (*cancelCode) {
	    Log("%s: DFS_ENABLE_CANCEL failed with code %lu, errno %lu\n",
		rtnName, *cancelCode, errno);
	    LogDebug(3, "bak_Unlock: failed to enable cancels, code %d\n",
		     *cancelCode);
	  }
    }
    LogDebug(3, "bak_Unlock: succeeded\n");
}

/* AwaitInitialization
 */

long
AwaitInitialization()
{  
    long start = 0;
    long code = 0;
    struct timespec delayTime;

    while ( 1 ) {
      lock_ObtainRead(&startupLock);
      if (procsInited) {
	lock_ReleaseRead(&startupLock);
	break;
      }
      lock_ReleaseRead(&startupLock);
      if (!start) 
	  start = time(0);
      else
      { 
	  if (time(0)-start > 5) {
	      LogDebug(3, "AwaitInitialization: server not ready, code %d\n",
		       UNOQUORUM);
	      return UNOQUORUM;
	  }
      }
      /* sleep for 1 sec. */
      delayTime.tv_sec = 1;
      delayTime.tv_nsec = 0;
      (void) pthread_delay_np (&delayTime);
    }
    LogDebug(3, "AwaitInitialization: server ready\n");
    return 0;
}

/* tailCompPtr
 *      name is a pathname style name, determine trailing name and return
 *      pointer to it
 */

char *
tailCompPtr(pathNamePtr)
     char *pathNamePtr;
{
    char *ptr;
    ptr = rindex(pathNamePtr, '/');
    if ( ptr == 0 )
    {
        /* this should never happen */
        LogError(0, "tailCompPtr: could not find / in name(%s)\n",
               pathNamePtr);
        return(pathNamePtr);
    }
    else
        ptr++;                                  /* skip the / */
    return(ptr);
}

/* callPermitted
 * exit:
 *	0 - no
 *	1 - yes
 */

callPermitted(h)
handle_t h;
{
    error_status_t code;

    code = dfsauth_server_CheckAuthorization (&h,
					      (dacl_permset_t *)NULL,
					      (char *)NULL,
					      (uuid_t *)NULL);

    if (code) {
      LogDebug(3, "callPermitted: no authorization, code %ld\n", code);
      if (code != DAUT_ERROR_ACCESS_DENIED)
       LogError(code,"dfsauth_server_CheckAuthorization failed: code %lu %s\n",
		code, dfs_dceErrTxt(code));
      return 0;
    }
    else
      return 1;
}

/* InitRPC
 *	This is called by every RPC interface to create a Ubik transaction
 *	and read the database header into core
 * entry:
 *	ut
 *	lock
 *	this_op
 * notes:
 *	sets a lock on byte 1 of the database. Looks like it enforces
 *	single threading by use of the lock.
 */

long
InitRPC (ut, lock, this_op)
  struct ubik_trans **ut;
  int   lock;				/* indicate read/write transaction */
  int  *this_op;			/* opcode of RCP, for COUNT_ABO */
{   int   code=0;
    int   code2=0;
    float wait = 0.91;			/* start waiting for 1 second */
    struct timespec delayTime;

    /* wait for server initialization to finish if this is not InitProcs
     * calling */
    if (this_op) if (code = AwaitInitialization())
    	return code;

    for (code = UNOQUORUM; code == UNOQUORUM; )
    { /*f*/
	if (lock == LOCKREAD)
		code = ubik_BeginTrans (globalConfPtr->database,
					UBIK_READTRANS, ut);
	else
		code = ubik_BeginTrans (globalConfPtr->database,
					UBIK_WRITETRANS, ut);

	if (code == UNOQUORUM)
	{
	    /* no quorum elected */
	    /* printf ("Waiting for quorum election.\n"); */
	    if (wait < 15.0) wait *= 1.1;
	    /* sleep for wait secs. */
        delayTime.tv_sec = (int)wait;
        delayTime.tv_nsec = 0;
        code2 = pthread_delay_np (&delayTime);
        if (code2 < 0)
	    return code2;

	pthread_yield();
	}
    } /*f*/

    if (code)
    	return code;
    wait = 0.91;

    /* set lock at posiion 1, for 1 byte of type lock */
    if (code = ubik_SetLock (*ut, 1, 1, lock))
    {

	/* if (this_op) COUNT_ABO; */
	ubik_AbortTrans (*ut);
	return code;
    }

    /* check that dbase is initialized and setup cheader */
    if (lock == LOCKREAD)
    {
	/* init but don't fix because this is read only */
	while (code = CheckInit(*ut, 0))
	{
	    ubik_AbortTrans(*ut);
	    if (code = ubik_BeginTrans (globalConfPtr->database,
					UBIK_WRITETRANS, ut))
		continue;
	    if (code = ubik_SetLock (*ut, 1, 1, LOCKWRITE)) {
		ubik_AbortTrans (*ut);
		continue;
	    }
	    /* now fix up the database */
	    if (code = CheckInit(*ut, rebuildDatabase)) {
		ubik_AbortTrans (*ut);
		return code;
	    }
	    if (code = ubik_EndTrans(*ut)) return code;
	    /* now open the read transaction that was originally requested. */
	    return InitRPC (ut, lock, this_op);
	}
    }
    else {
	if (code = CheckInit(*ut, rebuildDatabase)) {
	    /* if (this_op) COUNT_ABO; */
	    ubik_AbortTrans(*ut);
	    return code;
	}
    }
    return 0;
}

/* This is called to initialize a newly created database */

static int initialize_database (ut)
  struct ubik_trans *ut;
{   long  code;

    code = 0;
    return code;
}

long
InitProcs()
{
    struct ubik_trans  *ut;
    long code = 0;

    /*
     * Grab the startupLock before modifying the procsInited variable.
     * The AwaitInitialization routine wants to look at it too, and we
     * may need concurrency control here, at least in the future.
     */
    lock_Init(&startupLock);
    lock_ObtainWrite(&startupLock);
    procsInited = 0;
    lock_ReleaseWrite(&startupLock);

    InitDB();

    rebuildDatabase = initialize_database;

    if (code = InitRPC (&ut, LOCKREAD, 0))
    {
	LogError(code, "InitProcs: InitRPC failed\n");
	return code;
    }
    code = ubik_EndTrans(ut);
    if (code)
    {
	LogError(code, "InitProcs: ubik_EndTrans failed\n");
	return code;
    }

    rebuildDatabase = 0;		/* only do this during init */

    /* Grab the startupLock before modifying the procsInited variable */
    lock_ObtainWrite(&startupLock);
    procsInited = 1;
    lock_ReleaseWrite(&startupLock);

error_exit:
    return(code);
}

struct returnListElement {
    dbadr a;
    long sortKey;
};

struct returnList {
    int nElements;			/* number in list */
    int maxElements;			/* upper bound */
    int allocSize;			/* number of elements allocated */
    struct returnListElement *elements;	/* array of elements */
};

static long InitReturnList (list, max)
  struct returnList *list;
  int max;
{
    list->nElements = 0;
    list->maxElements = max;
    list->allocSize = 0;
    list->elements = 0;
    return 0;
}

static long AddToReturnList (list, a, sort)
  struct returnList *list;
  dbadr a;
  long sort;
{
  /* There doesn't seem to be a osi_Realloc, so can't convert the memory
     allocation calls to osi
   */
    if (a == 0) return 0;
    if (list->nElements >= BUDB_MAX_RETURN_LIST) return BUDB_LIST2BIG;
    if (list->nElements >= list->allocSize) {
	if (list->elements == 0)
	    list->elements = (struct returnListElement *)
		malloc (sizeof(struct returnListElement) *
			(list->allocSize = 10));
	else list->elements = (struct returnListElement *)
	    realloc (list->elements, sizeof(struct returnListElement) *
		     (list->allocSize += 10));
    }
    list->elements[list->nElements].a = a;
    list->elements[list->nElements].sortKey = sort;
    list->nElements++;
    return 0;
}

long FillVolEntry(ut, va, vol)
  struct ubik_trans *ut;
  dbadr va;
  budb_volumeEntry *vol;
{
    struct volFragment v;
    struct volInfo vi;
    dbadr via;
    struct tape t;
    dbadr ta;
    struct dump d;
    dbadr da;

    if (dbread (ut, va, &v, sizeof(v))) return BUDB_IO;
    vol->position = ntohl(v.position);
    vol->clone = ntohl(v.clone);
    vol->incTime = ntohl(v.incTime);
    vol->nBytes = ntohl(v.nBytes);
    vol->startByte = ntohl(v.startByte);
    vol->flags = ntohs(v.flags);	/* low 16 bits here */
    vol->seq = ntohs(v.sequence);

    via = ntohl(v.vol);
    if (dbread (ut, via, &vi, sizeof(vi))) return BUDB_IO;
    strcpy ((char *)vol->name, vi.name);
    strcpy ((char *)vol->server, vi.server);
    DFSH_MemFromNetHyper(vol->id, vi.id);
    vol->partition = ntohl(vi.partition);
    vol->nFrags = ntohl(vi.nFrags);
    vol->flags |= ntohl(vi.flags) & VOLINFOFLAGS; /* high 16 bits here */

    ta = ntohl(v.tape);
    if (dbread (ut, ta, &t, sizeof(t))) return BUDB_IO;
    strcpy ((char *)vol->tape, t.name);

    da = ntohl(t.dump);
    if (dbread (ut, da, &d, sizeof(d))) return BUDB_IO;
    vol->dump = ntohl(d.id);
    return 0;
}

long FillDumpEntry (ut, da, dump)
  struct ubik_trans *ut;
  dbadr da;
  budb_dumpEntry *dump;
{
    struct dump d;

    if (dbread (ut, da, &d, sizeof(d))) return BUDB_IO;
    dump->id = ntohl(d.id);
    dump->flags = ntohl(d.flags);
    dump->created = ntohl(d.created);
    strncpy ((char *)dump->name, d.dumpName, sizeof(dump->name));
    strncpy ((char *)dump->dumpPath, d.dumpPath, sizeof(dump->dumpPath));
    strncpy ((char *)dump->volumeSetName, d.volumeSet, sizeof(dump->volumeSetName));

    dump->parent = ntohl(d.parent);
    dump->level = ntohl(d.level);
    dump->nVolumes = ntohl(d.nVolumes);

    /* Initialize strings */

    dump->dumper.name[0] = '\0';
    dump->dumper.instance[0] = '\0';
    dump->dumper.cell[0] = '\0';
    dump->dumper.spare[0] = '\0';

    tapeSet_ntoh(&d.tapes, &dump->tapes);
    /* printf("dump name %s, parent %d, level %d\n",
	d.dumpName, ntohl(d.parent), ntohl(d.level)); */

    return 0;
}

long FillTapeEntry (ut, ta, tape)
  struct ubik_trans *ut;
  dbadr ta;
  budb_tapeEntry *tape;
{
    struct tape t;
    struct dump d;

    if ( dbread (ut, ta, &t, sizeof(t)) )
	return BUDB_IO;

    strcpy ((char *)tape->name, t.name);
    tape->flags = ntohl(t.flags);
    tape->written = ntohl(t.written);
    tape->expires = ntohl(t.expires);
    tape->nMBytes = ntohl(t.nMBytes);
    tape->nBytes = ntohl(t.nBytes);
    tape->nFiles = ntohl(t.nFiles);
    tape->nVolumes = ntohl(t.nVolumes);
    tape->seq = ntohl(t.seq);

    /* next three fields for future use */
    tape->tapeid = ntohl(t.tapeid);
    tape->useCount = ntohl(t.useCount);
    tape->mediaType = ntohl(t.mediaType);

    if (dbread (ut, ntohl(t.dump), &d, sizeof(d))) return BUDB_IO;
    tape->dump = ntohl(d.id);
    return 0;
}

static long
SendReturnDumpList (ut, list, FillProc, e_size, index, nextIndexP,
		    dbTimeP, eList)
  struct ubik_trans *ut;
  struct returnList *list;		/* list of elements to return */
  long (*FillProc)();			/* proc to fill entry */
  int   e_size;				/* size of each element */
  long  index;				/* index from previous call */
  long *nextIndexP;			/* if more elements are available */
  long *dbTimeP;			/* time of last db update */
  budb_dumpList **eList;		/* dump list structure */
{
    long code;
    int  max,min,num;
    int  i;
    char *e;

    *dbTimeP = ntohl(db.h.lastUpdate);
    *nextIndexP = -1;

    if (list->nElements == 0)
      return BUDB_EMPTYDUMPLIST;

    if (index >= list->nElements)
      return BUDB_ENDOFLIST;

    /* if a value for max # of elements is supplied, use it, otherwise
     * set to BUDB_MAX_RETURN_LIST
     */
    num = list->nElements - index;	/* number we have to return */
    if (num < BUDB_MAX_RETURN_LIST) min = num;
    else min = BUDB_MAX_RETURN_LIST;
    /* We hold min. memory to return the _len field for the stub. If we can't
     * allocate any memory after freeing this minimally allocated one, we're
     * hosed. This condition may cause the client to generate an exception
     * depending on stub behavior, we can't do much about it
     */
    rpc_ss_free((idl_void_p_t)*eList); /* free the dummy first */
    *eList = (budb_dumpList *)rpc_ss_allocate((unsigned32)sizeof(budb_dumpList)+((min-1)*sizeof(budb_dumpEntry)));
    if (*eList == (budb_dumpList *)NULL) {
      *eList = (budb_dumpList *)rpc_ss_allocate(sizeof(budb_dumpList));
      if (*eList)
	(*eList)->budb_dumpList_len=0;
      return BUDB_NOMEM;
    }
    e = (char *)(*eList)->budb_dumpList_val;
    (*eList)->budb_dumpList_len = min;

    /* clear the return elements */
    bzero((char *)(*eList)->budb_dumpList_val,
	  e_size * (*eList)->budb_dumpList_len);

    for (i=0; i<min; i++, e += e_size) {
	code = (*FillProc) (ut, list->elements[index+i].a, e);
	if (code) return code;
    }

    if (index+i == list->nElements) *nextIndexP = -1;
    else *nextIndexP = index+i;
    return 0;
}

static long
SendReturnTapeList (ut, list, FillProc, e_size, index, nextIndexP,
		    dbTimeP, eList)
  struct ubik_trans *ut;
  struct returnList *list;		/* list of elements to return */
  long (*FillProc)();			/* proc to fill entry */
  int   e_size;				/* size of each element */
  long  index;				/* index from previous call */
  long *nextIndexP;			/* if more elements are available */
  long *dbTimeP;			/* time of last db update */
  budb_tapeList **eList;	    	/* tape list structure */
{
    long code;
    int  max,min,num;
    int  i;
    char *e;

    *dbTimeP = ntohl(db.h.lastUpdate);
    *nextIndexP = -1;
    if (index >= list->nElements)
      return BUDB_ENDOFLIST;

    /* if a value for max # of elements is supplied, use it, otherwise
     * set to BUDB_MAX_RETURN_LIST
     */
    num = list->nElements - index;	/* number we have to return */
    if (num < BUDB_MAX_RETURN_LIST) min = num;
    else min = BUDB_MAX_RETURN_LIST;
    /* We hold min. memory to return the _len field for the stub. If we can't
     * allocate any memory after freeing this minimally allocated one, we're
     * hosed. This condition may cause the client to generate an exception
     * depending on stub behavior, we can't do much about it
     */
    rpc_ss_free((idl_void_p_t)*eList);
    *eList = (budb_tapeList *)rpc_ss_allocate((unsigned32)sizeof(budb_tapeList)+((min-1)*sizeof(budb_tapeEntry)));
    if (*eList == (budb_tapeList *)NULL) {
      *eList = (budb_tapeList *)rpc_ss_allocate(sizeof(budb_tapeList));
      if (*eList)
	(*eList)->budb_tapeList_len=0;
      return BUDB_NOMEM;
    }
    e = (char *)(*eList)->budb_tapeList_val;
    (*eList)->budb_tapeList_len = min;

    /* clear the return elements */
    bzero((char *)(*eList)->budb_tapeList_val,
	  e_size * (*eList)->budb_tapeList_len);

    for (i=0; i<min; i++, e += e_size) {
	code = (*FillProc) (ut, list->elements[index+i].a, e);
	if (code) return code;
    }

    if (index+i == list->nElements) *nextIndexP = -1;
    else *nextIndexP = index+i;
    return 0;
}

static long
SendReturnVolumeList (ut, list, FillProc, e_size, index, nextIndexP,
		    dbTimeP, eList)
  struct ubik_trans *ut;
  struct returnList *list;		/* list of elements to return */
  long (*FillProc)();			/* proc to fill entry */
  int   e_size;				/* size of each element */
  long  index;				/* index from previous call */
  long *nextIndexP;			/* if more elements are available */
  long *dbTimeP;			/* time of last db update */
  budb_volumeList **eList;		/* volume list structure */
{
    long code;
    int  max,min,num;
    int  i;
    char *e;

    *dbTimeP = ntohl(db.h.lastUpdate);
    *nextIndexP = -1;
    if (index >= list->nElements)
      return BUDB_ENDOFLIST;

    /* if a value for max # of elements is supplied, use it, otherwise
     * set to BUDB_MAX_RETURN_LIST
     */
    num = list->nElements - index;	/* number we have to return */
    if (num < BUDB_MAX_RETURN_LIST) min = num;
    else min = BUDB_MAX_RETURN_LIST;
    /* We hold min. memory to return the _len field for the stub. If we can't
     * allocate any memory after freeing this minimally allocated one, we're
     * hosed. This condition may cause the client to generate an exception
     * depending on stub behavior, we can't do much about it
     */
    rpc_ss_free((idl_void_p_t)*eList);
    *eList = (budb_volumeList *)rpc_ss_allocate((unsigned32)sizeof(budb_volumeList)+((min-1)*sizeof(budb_volumeEntry)));
    if (*eList == (budb_volumeList *)NULL) {
      *eList = (budb_volumeList *)rpc_ss_allocate(sizeof(budb_volumeList));
      if (*eList)
	(*eList)->budb_volumeList_len=0;
      return BUDB_NOMEM;
    }
    e = (char *)(*eList)->budb_volumeList_val;
    (*eList)->budb_volumeList_len = min;

    /* clear the return elements */
    bzero((char *)(*eList)->budb_volumeList_val,
	  e_size*(*eList)->budb_volumeList_len);

    for (i=0; i<min; i++, e += e_size) {
	code = (*FillProc) (ut, list->elements[index+i].a, e);
	if (code) return code;
    }

    if (index+i == list->nElements) *nextIndexP = -1;
    else *nextIndexP = index+i;
    return 0;
}

/* Come here to delete a volInfo structure.
   notes: Called with db.lock held for write */

static long DeleteVolInfo (ut, via, vi)
  struct ubik_trans *ut;
  dbadr via;
  struct volInfo *vi;
{   long code;
    dbadr hvia;
    struct volInfo hvi;

    if (vi->firstFragment) return 0;	/* still some frags, don't free yet */
    if (vi->sameNameHead == 0) { /* this is the head */
	if (vi->sameNameChain) return 0; /* empty head, some non-heads left */

	code = ht_HashOut (ut, &db.volName, via, vi);
	if (code) return code;
	code = FreeStructure (ut, volInfo_BLOCK, via);
	return code;
    }
    hvia = ntohl(vi->sameNameHead);
    if (dbread (ut, hvia, &hvi, sizeof(hvi))) return BUDB_IO;
    code = RemoveFromList (ut, hvia, &hvi, &hvi.sameNameChain, via, vi, &vi->sameNameChain);
    if (code == -1) return BUDB_DATABASEINCONSISTENT;
    if (code == 0) code = FreeStructure (ut, volInfo_BLOCK, via);
    return code;
}

/* Detach a volume fragment from its volInfo structure.  Its tape chain is
   already freed.  This routine frees the structure and the caller must not
   write it out.
   notes: Called with db.lock under shared lock
*/
static long DeleteVolFragment (ut, va, v)
  struct ubik_trans *ut;
  dbadr va;
  struct volFragment *v;
{   long code;
    struct volInfo vi;
    dbadr via, tpa;
    struct tape tp;

    via = ntohl(v->vol);
    if (dbread (ut, via, &vi, sizeof(vi))) return BUDB_IO;

    /* remove from the volinfo chain */
    code = RemoveFromList (ut, via, &vi, &vi.firstFragment, va, v, &v->sameNameChain);
    if (code == -1) return BUDB_DATABASEINCONSISTENT;
    if (code) return code;
    if (vi.firstFragment == 0)
      if (code = DeleteVolInfo (ut, via, &vi)) {
	return code;
      }
    /* remove from the tape chain */
    tpa = ntohl(v->tape);
    if (dbread (ut, tpa, &tp, sizeof(tp))) return BUDB_IO;
    code = RemoveFromList(ut, tpa, &tp, &tp.firstVol, va, v,&v->sameTapeChain);
    if (code == -1) return BUDB_DATABASEINCONSISTENT;

    if (code = FreeStructure (ut, volFragment_BLOCK, va)) {
      return code;
    }
    /* decrement frag counter */
    code = set_word_addr (ut, via, &vi, &vi.nFrags, htonl(ntohl(vi.nFrags)-1));
    if (code) return code;
    return 0;
}

/* DeleteTape - by freeing all its volumes and removing it from its dump chain.
 * The caller will remove it from the hash table if necessary.  The caller is
 * also responsible for writing the tape out if necessary.
 *
 * notes: called with db.lock held for write.
 */
static long DeleteTape (ut, ta, t)
  struct ubik_trans *ut;
  dbadr ta;
  struct tape *t;
{   long code;
    struct dump d;
    dbadr da;

    da = ntohl(t->dump);
    if (da == 0) return BUDB_DATABASEINCONSISTENT;
    if (dbread (ut, da, &d, sizeof(d))) return BUDB_IO;
    if (d.firstTape == 0) return BUDB_DATABASEINCONSISTENT;

    /* Since the tape should have been truncated there should never be any
     * volumes in the tape. */
    if (t->firstVol || t->nVolumes) return BUDB_DATABASEINCONSISTENT;

    code = RemoveFromList (ut, da, &d, &d.firstTape, ta, t, &t->nextTape);
    if (code == -1) return BUDB_DATABASEINCONSISTENT;
    if (code) return code;


    return 0;
}

static long
DeleteDump (ut, da, d)
     struct ubik_trans *ut;
     dbadr da;
     struct dump *d;
{
    long  code;

    /* Since the tape should have been truncated this should never happen. */
    if (d->firstTape || d->nVolumes)
	ERROR(BUDB_DATABASEINCONSISTENT);

    code = ht_HashOut (ut, &db.dumpIden, da, d);
    if ( code )
	ERROR(code);

    code = ht_HashOut (ut, &db.dumpName, da, d);
    if ( code )
	ERROR(code);

    code = FreeStructure (ut, dump_BLOCK, da);
    if ( code )
	ERROR(code);

error_exit:
    return code;
}

/*
 * VolInfoMatch()
 *     This is called with a volumeEntry and a volInfo structure and compares
 * them.  It returns non-zero if they are equal.  It is used by GetVolInfo to
 * search volInfo structures once it has the head volInfo structure from the
 * volName hash table.
 *
 *     When called from GetVolInfo the name compare is redundant.
 */

static int VolInfoMatch (vol, vi)
  budb_volumeEntry *vol;
  struct volInfo *vi;
{
    afs_hyper_t temp;

    DFSH_MemFromNetHyper(temp, vi->id);

    return ((strcmp ((char *)vol->name, vi->name) == 0) &&
	    AFS_hsame(vol->id, temp) &&
	    (strcmp ((char *)vol->server, vi->server) == 0) &&
	    (vol->partition == ntohl(vi->partition)) &&
	    ((vol->flags & VOLINFOFLAGS) == ntohl(vi->flags)));
}

/*
 * GetVolInfo()
 *     This routine takes a volumeEntry structure from an RPC interface and
 * returns the corresponding volInfo structure, creating it if necessary.
 *
 *     The caller must write the entry out.
 */

static long GetVolInfo (ut, vol, viaP, vi)
  struct ubik_trans *ut;
  budb_volumeEntry *vol;
  struct volInfo *vi;
  dbadr *viaP;
{   long  code;
    struct volInfo hvi;
    dbadr hvia, via;

    code = ht_LookupEntry (ut, &db.volName, vol->name, &hvia, &hvi);
    if (code)
      return code;
    if (hvia == 0) {			/* no head entry so create one */
	code = AllocStructure (ut, volInfo_BLOCK, 0, &via, vi);
	if (code) {
	  return code;
	}
	/* initialze those parts we need for the hash table and those specific
	 * to the head */
	strcpy (vi->name, (char *)vol->name);
	code = ht_HashIn (ut, &db.volName, via, vi);
        LogDebug(3, "GetVolInfo: Fileset Info for %s placed at %d\n",
		 db.volName, via);
	if (code) return code;
	vi->sameNameHead = vi->sameNameChain = 0;
      new_volInfo:
	vi->firstFragment = 0;
	vi->nFrags = 0;
	DFSH_NetFromMemHyper(vi->id, vol->id);
	strcpy (vi->server, (char *)vol->server);
	vi->partition = htonl(vol->partition);
	vi->flags = htonl(vol->flags);
	*viaP = via;
	return 0;
    }
    /* Hash table has an entry with this name, now find the correct entry */
    via = hvia;
    *vi = hvi;
    while (!VolInfoMatch (vol, vi)) {
	if (vi->sameNameChain == 0) {
	    /* allocate new entry and thread it onto entry in hash table */
	    code = AllocStructure (ut, volInfo_BLOCK, 0, &via, vi);
	    if (code) {
	      return code;
	    }
	    strcpy (vi->name, (char *)vol->name);
	    vi->nameHashChain = 0;	/* not in hash table */
	    vi->sameNameHead = htonl(hvia);
	    vi->sameNameChain = hvi.sameNameChain;

	    /* write head entry link */

	    if (set_word_addr(ut, hvia, &hvi, &hvi.sameNameChain,htonl(via))) {
		return BUDB_IO;
	    }
	    /* finish initializing entry */

	    goto new_volInfo;
	}
	via = ntohl(vi->sameNameChain);
	if (dbread (ut, via, vi, sizeof(*vi))) return BUDB_IO;
    }
    /* return matching entry */
    *viaP = via;
    return 0;
}

/* deletesomevolumesfromtape
 *	Deletes a specified number of volumes from a tape. The tape
 *	and dump are modified to reflect the smaller number of volumes.
 *	The transaction is not terminated, it is up to the caller to
 *	finish the transaction and start a new one (if desired).
 * entry:
 *	maxvolumestodelete - don't delete more than this many volumes
 */

long
deleteSomeVolumesFromTape(ut, tapeAddr, tapePtr, maxVolumesToDelete)
     struct ubik_trans *ut;
     dbadr tapeAddr;
     struct tape *tapePtr;
     int *maxVolumesToDelete;
{
    struct volFragment volFrag;
    dbadr volFragAddr, nextVolFragAddr;

    dbadr dumpAddr;
    struct dump dump;

    int volumesDeleted = 0;
    long  code = 0;

    if ( tapePtr == 0 )
	return(0);

    volFragAddr = ntohl(tapePtr->firstVol);

    while ( (volFragAddr != 0) && (*maxVolumesToDelete > 0) )
    {
	code = dbread(ut, volFragAddr, &volFrag, sizeof(volFrag));
        if ( code )
            ERROR(code);

        nextVolFragAddr = ntohl(volFrag.sameTapeChain);
        code = DeleteVolFragment(ut, volFragAddr, &volFrag);
        if ( code )
            ERROR(code);

        volFragAddr = nextVolFragAddr;
	(*maxVolumesToDelete)--;
	volumesDeleted++;
    }

    /* reset the volume fragment pointer in the tape */
    tapePtr->firstVol = htonl(volFragAddr);

    /* diminish the tape's volume count */
    tapePtr->nVolumes = htonl(ntohl(tapePtr->nVolumes) - volumesDeleted);

    code = dbwrite(ut, tapeAddr, tapePtr, sizeof(*tapePtr));
    if ( code )
	ERROR(code);

    /* diminish the dump's volume count */
    dumpAddr = ntohl(tapePtr->dump);
    code = dbread(ut, dumpAddr, &dump, sizeof(dump));
    if ( code )
	ERROR(code);

    dump.nVolumes = htonl(ntohl(dump.nVolumes) - volumesDeleted);
    code = dbwrite(ut, dumpAddr, &dump, sizeof(dump));
    if ( code )
	ERROR(code);

error_exit:
    return(code);
}

/* deleteDump
 *	deletes a dump in stages, by repeatedly deleting a small number of
 *	volumes from the dump until none are left. The dump is then deleted.
 *
 *	In the case where multiple calls are made to delete the same
 *	dump, the operation will succeed but contention for structures
 *	will result in someone getting back an error.
 *
 * entry:
 *	id - id of dump to delete
 * notes:
 *	Database unlocked and current transaction ended before
 *	deleteDump returns. Dump is deleted in chunks, with a new
 *      transaction for each chunk.
 */

long
deleteDump(ut, id)
  struct ubik_trans *ut;
  dumpId id;
{
    struct dump dump;
    dbadr dumpAddr;
    struct tape tape;
    dbadr tapeAddr;
    long code = 0;
    int volCnt;

    /* iterate until the dump is truly deleted */
    lock_UpgradeSToW(&(db.lock));
    volCnt = BUDB_UBIK_MAX_VOLS;
    while ( 1 )
    {
	pthread_yield();

	code = ht_LookupEntry (ut, &db.dumpIden, &id, &dumpAddr, &dump);
	if (code || (dumpAddr == 0)) {
	  lock_ReleaseWrite(&(db.lock));
	  ubik_AbortTrans(ut);
	  return(code);
	}
	tapeAddr = ntohl(dump.firstTape);
	if ( tapeAddr != 0 )
	{
	    /* there is a tape to delete */
	    code = dbread(ut, tapeAddr, &tape, sizeof(tape));
	    if ( code ) {
	      lock_ReleaseWrite(&(db.lock));
	      ubik_AbortTrans(ut);
	      return(code);
	    }
	    if ( tape.nVolumes ) {
	      /* tape is not empty */
	      code = deleteSomeVolumesFromTape(ut, tapeAddr, &tape, &volCnt);
	      if ( code ) {
	        lock_ReleaseWrite(&(db.lock));
	        ubik_AbortTrans(ut);
		return(code);
	      }
	    }
	    if ( ntohl(tape.nVolumes) == 0 ) {
	      /* tape is now empty, delete it */
	      code = DeleteTape(ut, tapeAddr, &tape);
	      if ( code ) {
	        lock_ReleaseWrite(&(db.lock));
	        ubik_AbortTrans(ut);
		return(code);
	      }
	      code = ht_HashOut(ut, &db.tapeName, tapeAddr, &tape);
	      if ( code ) {
	        lock_ReleaseWrite(&(db.lock));
	        ubik_AbortTrans(ut);
		return(code);
	      }
	      code = FreeStructure(ut, tape_BLOCK, tapeAddr);
	      if ( code ) {
	        lock_ReleaseWrite(&(db.lock));
	        ubik_AbortTrans(ut);
		return(code);
	      }

	      /* count deleting the tape */
	      if ( volCnt > 0 ) volCnt--;
	    }

	    /*
	     * start a new transaction after each chunk
	     */

	    if ( volCnt <= 0 )
	    {
	        lock_ReleaseWrite(&(db.lock));
	        ubik_EndTrans(ut);
   		code = InitRPC (&ut, LOCKWRITE, (int *) 1 /*this_op*/ );
    		if ( code ) {
		    return(code);
		}
        	lock_ObtainWrite(&(db.lock));
		volCnt = BUDB_UBIK_MAX_VOLS;
	    }

	    continue;			/* next deletion portion */
	}

	/* finally done. No more tapes left in the dump. Delete the dump
	 * itself
	 */
	code = DeleteDump(ut, dumpAddr, &dump);
	lock_ReleaseWrite(&(db.lock));
	ubik_EndTrans(ut);
	return(code);
    }
}

/* --------------
 * dump selection routines - used by BUDB_GetDumps
 * --------------
 */

/* most recent dump selection */

struct chosenDump
{
    struct chosenDump *next;
    dbadr addr;
    u_long  date;
};

struct wantDumpRock
{
    int	maxDumps;			/* max wanted */
    int	ndumps;				/* actual in chain */
    struct chosenDump *chain;
};


wantDump(dumpAddrParam, dumpParam, dumpListPtrParam)
     char *dumpAddrParam;
     char *dumpParam;
     char *dumpListPtrParam;
{
    dbadr dumpAddr;
    struct dump *dumpPtr;
    struct wantDumpRock *rockPtr;

    dumpAddr = (dbadr) dumpAddrParam;
    dumpPtr = (struct dump *) dumpParam;
    rockPtr = (struct wantDumpRock *) dumpListPtrParam;

    /* if we don't have our full complement, just add another */
    if ( rockPtr->ndumps < rockPtr->maxDumps )
	return(1);

    /* got the number we need, select based on date */
    if (  (u_long) ntohl(dumpPtr->created) > rockPtr->chain->date )
	return(1);

    return(0);
}

rememberDump(dumpAddrParam, dumpParam,  dumpListPtrParam)
     char *dumpAddrParam;
     char *dumpParam;
     char *dumpListPtrParam;
{
    dbadr dumpAddr;
    struct dump *dumpPtr;
    struct wantDumpRock *rockPtr;
    struct chosenDump *ptr, *deletedPtr, **nextPtr;

    dumpAddr = (dbadr) dumpAddrParam;
    dumpPtr = (struct dump *) dumpParam;
    rockPtr = (struct wantDumpRock *) dumpListPtrParam;

    ptr = (struct chosenDump *) osi_Alloc(sizeof(struct chosenDump));
    if ( ptr == 0 )
	return(0);
    bzero((char *)ptr, sizeof(*ptr));
    ptr->addr = dumpAddr;
    ptr->date = (u_long) ntohl(dumpPtr->created);

    if ( rockPtr->ndumps == rockPtr->maxDumps )
    {
	/* got to drop one */
	deletedPtr = rockPtr->chain;
	rockPtr->chain = deletedPtr->next;

	(void)osi_Free(deletedPtr, sizeof(struct chosenDump));
    }
    else
	rockPtr->ndumps++;

    /* now insert in the right place */
    nextPtr = &rockPtr->chain;

    /* if there any dumps to compare with */
    if ( *nextPtr )
    {
	while ( (*nextPtr)->date <= (u_long) ntohl(dumpPtr->created) )
	{
	    nextPtr = &((*nextPtr)->next);
	    if ( *nextPtr == 0 )			/* end of chain */
		break;
	}
    }

    ptr->next = *nextPtr;
    *nextPtr = ptr;
    return(0);
}


/* ---------------------------------------------
 * general interface routines - alphabetic
 * ---------------------------------------------
 */


/*
 * This routine has been replaced by BUDB_AddVolumes. The
 * routine remains in the interface for compability with down
 * level clients.
 */
error_status_t
BUDB_AddVolume (h, vol)
  handle_t h;
  budb_volumeEntry *vol;
{
    long  code;

    code = BUDB_AddVolumes(h, 1, vol);
    return code;
}

error_status_t
BUDB_AddVolumes (h, cnt, vol)
  handle_t h;
  unsigned long cnt;
  budb_volumeEntry vol[];
{   int index;
    long code;
    struct ubik_trans *ut;
    struct dump d;
    struct tape t;
    dbadr da,ta;
    struct volInfo vi;
    dbadr via;
    struct volFragment v;
    dbadr va;
    char *rtn_Name="BUDB_AddVolumes";
    u_long bytes;
    int tapeFound;
    int cancelCode, cancelState;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }
    for ( index = 0 ; index < cnt ; index++ )
    {
        if ((strlen ((char *)vol[index].name) >= sizeof(vi.name)) ||
	    (strlen ((char *)vol[index].server) >= sizeof(vi.server)) ||
	    (strlen ((char *)vol[index].tape) >= sizeof(t.name))) {
          bak_enableCancel(rtn_Name, cancelState, &cancelCode);
          return BUDB_BADARGUMENT;
        }
    }
    if (code = InitRPC (&ut, LOCKWRITE, (int *)1/*this_op*/)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    LogDebug(2, "Entering BUDB_AddVolumes: \n");
    lock_ObtainWrite(&(db.lock));
    for ( index = 0 ; index < cnt ; index++ )
    {
        LogDebug(1, "Adding volume %s; ", vol[index].name);
        code = ht_LookupEntry (ut, &db.dumpIden, &vol[index].dump, &da, &d);
        lock_ConvertWToS(&(db.lock));
        if (code) {
          abort:
          LogDebug(1, "failed to add volume, code %ld\n", code);
          lock_ReleaseShared(&(db.lock));
          ubik_AbortTrans(ut);
          bak_enableCancel(rtn_Name, cancelState, &cancelCode);
          return code;
        }
    
        if (da == 0)
        {
	    code = BUDB_NODUMPID;
	    goto abort;
        }
    
        /* search for the right tape */
        tapeFound = 0;
        ta = ntohl(d.firstTape);
        while ( ta != 0 )
        {
	    code = dbread(ut, ta, &t, sizeof(t));
	    if ( strcmp(t.name, (char *)vol[index].tape) == 0 )
	    {
	        tapeFound = 1;
	        break;
	    }
	    ta = ntohl(t.nextTape);
        }
    
        /* can't locate tape */
        if ( tapeFound == 0 )
        {
	    code = BUDB_NOTAPENAME;
	    goto abort;
        }
        LogDebug(1, "in tape %s; ", vol[index].tape);
        if ((t.dump != htonl(da)) ||
	    ((ntohl(t.flags) & BUDB_TAPE_BEINGWRITTEN) == 0) ||
	    ((ntohl(d.flags) & BUDB_DUMP_INPROGRESS) == 0))
	    {code = BUDB_BADPROTOCOL; goto abort;}
    
        /* checkHash(ut, HT_volName_FUNCTION); */
        lock_UpgradeSToW(&(db.lock));
        if (code = GetVolInfo (ut, &(vol[index]), &via, &vi)) {
          lock_ConvertWToS(&(db.lock));
          goto abort;
        }
    
        /* check for other fragments from a possible earlier addvol */
    
        code = AllocStructure (ut, volFragment_BLOCK, 0, &va, &v);
        if (code) {
          lock_ConvertWToS(&(db.lock));
          goto abort;
        }
        v.vol = htonl(via);
        v.sameNameChain = vi.firstFragment;
        vi.firstFragment = htonl(va);
        vi.nFrags = htonl(ntohl(vi.nFrags)+1);
        if (code = dbwrite (ut, via, &vi, sizeof(vi))) {
          lock_ConvertWToS(&(db.lock));
          goto abort;
        }
        v.tape = htonl(ta);
        v.sameTapeChain = t.firstVol;
        t.firstVol = htonl(va);
        t.nVolumes = htonl(ntohl(t.nVolumes) + 1);
    
        bytes = ntohl(t.nBytes) + vol[index].nBytes;
        t.nMBytes = htonl(ntohl(t.nMBytes) + bytes/(1024*1024));
        t.nBytes =  htonl(bytes % (1024*1024));
        if (code = dbwrite (ut, ta, &t, sizeof(t))) {
          lock_ConvertWToS(&(db.lock));
          goto abort;
        }
        d.nVolumes = htonl(ntohl(d.nVolumes) + 1);
        if (code = dbwrite (ut, da, &d, sizeof(d))) {
          lock_ConvertWToS(&(db.lock));
          goto abort;
        }
        v.position = htonl(vol[index].position);
        v.clone = htonl(vol[index].clone);
        v.incTime = htonl(vol[index].incTime);
        v.startByte = htonl(vol[index].startByte);
        v.nBytes = htonl(vol[index].nBytes);
        v.flags = htons(vol[index].flags & VOLFRAGMENTFLAGS);
        v.sequence = htons(vol[index].seq);
        LogDebug(1, "at %d\n", va);
        if (code = dbwrite (ut, va, &v, sizeof(v))) {
          lock_ConvertWToS(&(db.lock));
          goto abort;
        }
        if (code = set_header_word (ut, lastUpdate, htonl(time(0)))) {
          lock_ConvertWToS(&(db.lock));
          goto abort;
        }
    }
    LogDebug(2, "Leaving BUDB_AddVolumes, code %ld\n", code);
    lock_ReleaseWrite(&(db.lock));
    code = ubik_EndTrans(ut);
    if (code)
      LogDebug(1, "failed to add fileset, code=%ld\n");
    else
      LogDebug(1, "successfully added\n");
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;
}

/* BUDB_CreateDump
 *	records the existence of a dump in the database. This creates only
 *	the dump record, to which one must attach tape and volume records.
 * TBD
 *	1) record the volume set
 */

error_status_t
BUDB_CreateDump(h, dump)
  handle_t h;
  budb_dumpEntry *dump;
{
    long code;
    struct ubik_trans *ut;
    struct dump d;
    dbadr a;
    struct timespec delayTime;
    /* for lookups */
    dbadr findDumpAddr;
    struct dump findDump;
    long	    kvno;
    Date	    expiration;		/* checked by Security Module */
    char *rtn_Name="BUDB_CreateDump";
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }

    if (strlen((char *)dump->name) >= sizeof(d.dumpName)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_BADARGUMENT;
    }
    if (code = InitRPC (&ut, LOCKWRITE, (int *) 1 /*this_op*/)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    LogDebug(2, "Entering BUDB_CreateDump: \n");
    LogDebug(1, "Creating dump %s, path %s; ", dump->name, dump->dumpPath);
    lock_ObtainShared(&(db.lock));
    bzero((char *)&d, sizeof(d));
    lock_UpgradeSToW(&(db.lock));
    if (code = AllocStructure (ut, dump_BLOCK, 0, &a, &d)) {
        lock_ConvertWToS(&(db.lock));
	goto abort;
    }
    lock_ConvertWToS(&(db.lock));
    /* dump id's are time stamps */
    if ( dump->id == 0 )
    {
        LogDebug(1, "dump id not supplied, allocate one; ");
	/* no dump id supplied */
	while ( dump->id == 0 )
	{ /*w*/
	    /* allocate an id and iterate until no conflicts */

	    dump->id = time(0);

	    /* ensure it is unique */
	    lock_UpgradeSToW(&(db.lock));
	    code = ht_LookupEntry(ut, &db.dumpIden, &dump->id, &findDumpAddr,
				  &findDump);
	    if ( code ) {
	        lock_ConvertWToS(&(db.lock));
		goto abort;
	    }
	    /* no conflicts, continue */
	    if ( findDumpAddr == 0 )
	    {
		/* update the last dump id allocated */
		code = set_header_word(ut, lastDumpId, htonl(dump->id));
		if ( code ) {
		    lock_ConvertWToS(&(db.lock));
		    goto abort;
		}
		lock_ConvertWToS(&(db.lock));
		break;
	    }

	    /* found a conflicting id - wait a while */
	    dump->id = 0;
	    delayTime.tv_sec = 1;
	    delayTime.tv_nsec = 0;
	    lock_ReleaseWrite(&(db.lock));
	    code = pthread_delay_np (&delayTime);
	    if (code < 0)
		fprintf(stderr, "BUDB_CreateDump: pthread_delay_np failed\n");
	    lock_ObtainShared(&(db.lock));
	} /*w*/
    }
    else
    { /*e*/
	/* dump id supplied e.g. for database restore. Disallow conflicts */
        LogDebug(1, "dump id %lu supplied, check to see if unique; ",
		 dump->id);
	lock_UpgradeSToW(&(db.lock));
	code = ht_LookupEntry(ut, &db.dumpIden, &dump->id, &findDumpAddr,
			      &findDump);
	lock_ConvertWToS(&(db.lock));
	if ( code )
	    goto abort;

	/* disallow conflicts */
	if ( findDumpAddr != 0 )
	{
	    LogDebug(1, "dump id exists already; ");
	    code = BUDB_DUMPIDEXISTS;
	    goto abort;
	}
    } /*e*/

    d.id = htonl(dump->id);
    strcpy( d.dumpName, (char *)dump->name);		/* volset.dumpname */
    strcpy( d.dumpPath, (char *)dump->dumpPath);	/* dump node path */
    strcpy( d.volumeSet, (char *)dump->volumeSetName);	/* volume set */

    d.parent = htonl(dump->parent);		/* parent id */
    d.level = htonl(dump->level);


    /* if creation time specified use that, else just the current time */
    if (dump->created == 0)
    	dump->created = time(0);
    d.created = htonl(dump->created);
    tapeSet_hton(&dump->tapes, &d.tapes);

    lock_UpgradeSToW(&(db.lock));
/*    
  Remove this error check, as old butc's do not check for this
  if (dump->flags & BUDB_DUMP_INPROGRESS) {
        lock_ConvertWToS(&(db.lock));
	code = BUDB_DUMPINUSE;
	goto abort;
    }
*/
    dump->flags |=  BUDB_DUMP_INPROGRESS;
    d.flags = htonl(dump->flags);

    code = ht_HashIn (ut, &db.dumpName, a, &d);
    if (code)  {
        lock_ConvertWToS(&(db.lock));
        goto abort;
    }
    code = ht_HashIn (ut, &db.dumpIden, a, &d);
    if (code) {
        lock_ConvertWToS(&(db.lock));
    	goto abort;
    }
    LogDebug(1, "\ncreation parameters: dump name %s(%lu), fileset family %s, parent %d, level %d, time %lu\n",
	     dump->name, dump->id, dump->volumeSetName, dump->parent,
	     dump->level, dump->created);
    if (code = dbwrite (ut, a, (char *)&d, sizeof(d))) {
        lock_ConvertWToS(&(db.lock));
    	goto abort;
    }
    if (code = set_header_word (ut, lastUpdate, htonl(time(0)))) {
        lock_ConvertWToS(&(db.lock));
    	goto abort;
    }
    lock_ReleaseWrite(&(db.lock));
    code = ubik_EndTrans(ut);
    LogDebug(2, "Leaving BUDB_CreateDump, code %lu\n", code);
    if (code)
      LogDebug(1, "failed to create dump, code=%ld\n", code);
    else
      LogDebug(1, "successfully created dump\n");
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;

abort:
    lock_ReleaseShared(&(db.lock));
    ubik_AbortTrans(ut);
    LogDebug(2, "Leaving BUDB_CreateDump, code %lu\n", code);
    LogDebug(1, "failed to create dump, code=%ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;
}

error_status_t
BUDB_DeleteDump (h, id)
  handle_t h;
  dumpId id;
{
  long code;
  char *rtn_Name="BUDB_DeleteDump";
  int cancelState, cancelCode;
  struct ubik_trans *ut;

  bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
  if (cancelCode)
    return(cancelCode);

  if ( callPermitted(h) == 0 ) {
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(BUDB_NOTPERMITTED);
  }

  if (code = InitRPC (&ut, LOCKWRITE, (int *)1/*this_op*/)) {
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;
  }

  LogDebug(2, "Entering BUDB_DeleteDump: \n");
  LogDebug(1, "Deleting dump %ld\n", id);
  lock_ObtainShared(&(db.lock));
  
  /*
   * deleteDump will delete volumes from the dump in chunks,
   * and ends the transaction after each chunk.
   */

  code = deleteDump(ut, id);
  
  LogDebug(2, "Leaving BUDB_DeleteDump, code %ld\n", code);

  bak_enableCancel(rtn_Name, cancelState, &cancelCode);
  return (code);
}

error_status_t
BUDB_DeleteTape (h, tape)
  handle_t h;
  budb_tapeEntry *tape;		/* tape info */
{   long  code;
    struct ubik_trans *ut;
    struct tape t;
    dbadr a;
    char *rtn_Name="BUDB_DeleteTape";
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }
    if (code = InitRPC (&ut, LOCKWRITE, (int *)1/*this_op*/)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    LogDebug(2, "Entering BUDB_DeleteTape: \n");
    LogDebug(1, "Deleting tape %s containing dump %lu; ",
	     tape->name, tape->dump);
    lock_ObtainWrite(&(db.lock));
    code = ht_LookupEntry (ut, &db.tapeName, tape->name, &a, &t);
    if (code) {
    abort:
      lock_ReleaseWrite(&(db.lock));
      ubik_AbortTrans(ut);
      LogDebug(2, "Leaving BUDB_DeleteTape, code %ld\n", code);
      LogDebug(1, "failed to delete tape, code=%ld\n", code);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    LogDebug(1, "at location %ld; ", a);
    if ((code = DeleteTape (ut, a, &t)) ||
	(code = FreeStructure (ut, tape_BLOCK, a)))
      goto abort;
    LogDebug(1, "at time %lu; ", time(0));
    if (code = set_header_word (ut, lastUpdate, htonl(time(0))))
      goto abort;

    code = ubik_EndTrans(ut);
    lock_ReleaseWrite(&(db.lock));
    LogDebug(2, "\nLeaving BUDB_DeleteTape, code %ld\n", code);
    if (code)
      LogDebug(1, "failed to delete tape, code=%ld\n", code);
    else
      LogDebug(1, "successfully deleted tape\n");
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;
}

/* BUDB_DeleteVDP
 *      Deletes old information from the database for a particular dump path
 *      and volumset. This supercedes the old policy implemented in
 *      UseTape, which simply matched on the volumeset.dump. Consequently
 *      it was unable to handle name re-use.
 * entry:
 *      dsname - dumpset name, i.e. volumeset.dumpname
 *      dumpPath - full path of dump node
 *      curDumpID - current dump in progress - so that is may be excluded
 * exit:
 *      0 - ok
 *      n - some error. May or may not have deleted information.
 */

error_status_t
BUDB_DeleteVDP(h, ndsname, ndumpPath, curDumpId)
  handle_t h;
  bakNameString_t *ndsname;
  bakNameString_t *ndumpPath;
  long curDumpId;
{
    struct dump dump;
    dbadr dumpAddr;
    char *rtn_Name="BUDB_DeleteVDP";
    struct ubik_trans *ut;
    long code = 0;
    int cancelState, cancelCode;
    char *dsname, *dumpPath;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }

    /* unmarshal the arguments */
    dsname = (char *)osi_Alloc(strlen((char *)ndsname)+1);
    if (dsname == (char *)NULL) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    strcpy(dsname, (char *)ndsname);
    dumpPath = (char *)osi_Alloc(strlen((char *)ndumpPath)+1);
    if (dumpPath == (char *)NULL) {
      osi_Free(dsname, strlen(dsname)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    strcpy(dumpPath, (char *)ndumpPath);
    code = InitRPC (&ut, LOCKWRITE, (int *) 1 /*this_op*/ );
    if ( code ) {
      osi_Free(dsname, strlen(dsname)+1);
      osi_Free(dumpPath, strlen(dumpPath)+1);
      LogDebug(2, "Leaving BUDB_DeleteVDP, code %ld\n", code);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(code);
    }

    LogDebug(2, "Entering BUDB_DeleteVDP: \n");
    LogDebug(1, "Deleting dump %lu, dump set %s, dump path %s; ",
	     curDumpId, dsname, dumpPath);
    lock_ObtainShared(&(db.lock));
    while ( 1 )
    {
	lock_UpgradeSToW(&(db.lock));
	code = ht_LookupEntry(ut, &db.dumpName, dsname, &dumpAddr, &dump);
	lock_ConvertWToS(&(db.lock));
	if ( code )
	    ABORT(code);

	while ( dumpAddr != 0 )
	{ /*wd*/
	    if ( (strcmp(dump.dumpName, (char *)dsname) == 0)
	    &&   (strcmp(dump.dumpPath, (char *)dumpPath) == 0 )
	    &&   (ntohl(dump.id) != curDumpId)
               )
	    {
		LogDebug(1, "found dump entry at location %lu; ", dumpAddr);
		LogDebug(1, "deleteing dump id %lu; ", dump.id);

		code = deleteDump(ut, ntohl(dump.id));

		if ( code ) {
		  osi_Free(dsname, strlen(dsname)+1);
		  osi_Free(dumpPath, strlen(dumpPath)+1);
		  LogDebug(2, "Leaving BUDB_DeleteVDP, code %ld\n", code);
		  bak_enableCancel(rtn_Name, cancelState, &cancelCode);
		  return(code);
		}

		/*
		 * start a new transaction and start the traversal over
		 * since the various chains may have changed
		 */

    		code = InitRPC (&ut, LOCKWRITE, (int *) 1 /*this_op*/ );
    		if ( code ) {
	          osi_Free(dsname, strlen(dsname)+1);
	          osi_Free(dumpPath, strlen(dumpPath)+1);
		  LogDebug(2, "Leaving BUDB_DeleteVDP, code %ld\n", code);
	          bak_enableCancel(rtn_Name, cancelState, &cancelCode);
	          return(code);
    		}
                lock_ObtainShared(&(db.lock));
		break;
	    }

	    dumpAddr = ntohl(dump.nameHashChain);
	    if ( dumpAddr )
	    {
		code = dbread(ut, dumpAddr, &dump, sizeof(dump));
		if ( code )
		    ABORT(code);
	    }
	} /*wd*/

	/* check if all the dumps have been examined */
	if ( dumpAddr == 0 )
	{
	    /* can terminate */
	  osi_Free(dsname, strlen(dsname)+1);
	  osi_Free(dumpPath, strlen(dumpPath)+1);
	  lock_ReleaseShared(&(db.lock));
	  code = ubik_EndTrans(ut);
	  LogDebug(1, "successfully deleted all dumps\n");
	  LogDebug(2, "Leaving BUDB_DeleteVDP, code %ld\n", code);
	  bak_enableCancel(rtn_Name, cancelState, &cancelCode);
	  return(code);
	}
    }
    /* not reached */


abort_exit:
    osi_Free(dsname, strlen(dsname)+1);
    osi_Free(dumpPath, strlen(dumpPath)+1);
    lock_ReleaseShared(&(db.lock));
    ubik_AbortTrans(ut);
    LogDebug(2, "Leaving BUDB_DeleteVDP, code %ld\n", code);
    LogDebug(1, "failed to delete all dumps, code=%ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}

/* BUDB_FindClone
 * notes:
 *	re-write to do lookups by volume name
 */

error_status_t
BUDB_FindClone(h, dumpID, nvolName, clonetimep, clonesparep)
  handle_t h;
  long dumpID;
  bakNameString_t *nvolName;
  unsigned long *clonetimep;
  unsigned long *clonesparep;
{
    dbadr diskAddr;
    dbadr tapeAddr, volFragmentAddr;
    struct dump dump;
    struct tape tape;
    struct volFragment volFragment;
    struct volInfo volInfo;
    struct ubik_trans *ut;
    long code;
    char *rtn_Name="BUDB_FindClone";
    int cancelState, cancelCode;
    char *volName;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }

    *clonetimep = 0;
    *clonesparep = 0;

    volName = (char *)osi_Alloc(strlen((char *)nvolName)+1);
    if (volName == (char *)NULL) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    strcpy(volName, (char *)nvolName);
    code = InitRPC (&ut, LOCKREAD, (int *) 1 /*this_op*/ );
    if ( code ) {
      osi_Free(volName, strlen(volName)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(code);
    }
    LogDebug(2, "Entering BUDB_FindClone: \n");
    LogDebug(1, "finding clone time of fileset %s in dump %lu; ",
	     volName, dumpID);
    lock_ObtainShared(&(db.lock));
    while ( dumpID != 0 )
    {
	lock_UpgradeSToW(&(db.lock));
	code = ht_LookupEntry (ut, &db.dumpIden, &dumpID, &diskAddr, &dump);
	lock_ConvertWToS(&(db.lock));
	if ( code )
	  goto exit;	/* shouldn't this be abort??? */

	/* just to be sure */
	if ( ntohl(dump.id) != dumpID )
	{
	    LogDebug(1, "dump id requested %d, found %d; ",
		   dumpID, ntohl(dump.id));
	    code = BUDB_INTERNALERROR;
	    goto abort;
	}
	LogDebug(1, "searching all tapes for fileset %s; ", volName);
	/* search all the tapes */
	tapeAddr = ntohl(dump.firstTape);
	while ( tapeAddr )
	{
	    code = dbread(ut, tapeAddr, &tape, sizeof(tape));
	    if (code)
	    	goto abort;

	    /* now check all the volume fragments */
	    volFragmentAddr = ntohl(tape.firstVol);
	    while ( volFragmentAddr )
	    {
		code = dbread(ut, volFragmentAddr, &volFragment,
			      sizeof(volFragment));
		if (code)
			goto abort;

		/* now check if this volume is the one we want */
		code = dbread(ut, ntohl(volFragment.vol), &volInfo,
			      sizeof(volInfo));
		if ( code )
			goto abort;

		if ( strcmp(volInfo.name, (char *)volName) == 0 )
		{
		    *clonetimep = ntohl(volFragment.clone);
		    LogDebug(1, "found fileset %s, clone time %lu; ",
			     volName, *clonetimep);
		    goto exit;
		}

		volFragmentAddr = ntohl(volFragment.sameTapeChain);
	    }
	    tapeAddr = ntohl(tape.nextTape);
	}
	dumpID = ntohl(dump.parent);
    }

exit:
    osi_Free(volName, strlen(volName)+1);
    lock_ReleaseShared(&(db.lock));
    code = ubik_EndTrans(ut);
    LogDebug(1, "found clone\n");
    LogDebug(2, "Leaving BUDB_FindClone, code %ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);

abort:
    osi_Free(volName, strlen(volName)+1);
    lock_ReleaseShared(&(db.lock));
    ubik_AbortTrans(ut);
    LogDebug(1, "failed to find clone\n");
    LogDebug(2, "Leaving BUDB_FindClone, code %ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}

/* BUDB_FindDump
 *      Find latest volume dump before adate.
 *      Used by restore code when restoring a user requested volume(s)
 * entry:
 *      volumeName - name of volume to match on
 *      beforeDate - look for dumps older than this date
 * exit:
 *      deptr - descriptor of most recent dump
 */

error_status_t
BUDB_FindDump(h, nvolumeName, beforeDate, dateSpare, deptr)
     handle_t h;
     bakNameString_t *nvolumeName;
     unsigned long beforeDate;
     unsigned long dateSpare;
     budb_dumpEntry *deptr;
{
    struct dump dump;
    struct tape tape;
    struct volInfo volInfo;
    struct volFragment volFragment;
    dbadr volInfoAddr, volFragmentAddr;
    char *rtn_Name="BUDB_FindDump";
    dbadr selectedDumpAddr = 0;
    long dumpCreated;
    long selectedDate = 0;
    struct ubik_trans *ut;
    long code;
    int cancelState, cancelCode;
    char *volumeName;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }
    bzero((char *)deptr, sizeof(budb_dumpEntry));
    volumeName = (char *)osi_Alloc(strlen((char *)nvolumeName)+1);
    if (volumeName == (char *)NULL) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    strcpy(volumeName, (char *)nvolumeName);
    code = InitRPC (&ut, LOCKREAD, (int *) 1 /*this_op*/ );
    if ( code ) {
      osi_Free(volumeName, strlen(volumeName)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(code);
    }
    LogDebug(2, "Entering BUDB_FindDump: \n");
    LogDebug(1, "Looking for dump for fileset %s before date %s; ",
	     volumeName, ctime((time_t *)&beforeDate));
    lock_ObtainWrite(&(db.lock));
    code = ht_LookupEntry (ut, &db.volName, volumeName, &volInfoAddr,
			   &volInfo);
    lock_ConvertWToS(&(db.lock));
    if ( code )
    	goto abort;

    /* now check against all volumes of this name */
    while ( volInfoAddr )
    { /*wvi*/
	volFragmentAddr = ntohl(volInfo.firstFragment);

	while ( volFragmentAddr )
	{ /*wvf*/
	    code = dbread(ut, volFragmentAddr, &volFragment,
			  sizeof(volFragment));
	    if (code)
	    	goto abort;

	    code = dbread(ut, ntohl(volFragment.tape), &tape, sizeof(tape));
	    if (code)
	    	goto abort;

	    code = dbread(ut, ntohl(tape.dump), &dump, sizeof(dump));
	    if (code)
	    	goto abort;

	    dumpCreated = ntohl(dump.created);

	    /* now we can examine the date */
	    if ( (selectedDate < dumpCreated) && (dumpCreated < beforeDate) )
	    {
		/* this is a more recent dump */
		selectedDate = dumpCreated;
		selectedDumpAddr = ntohl(tape.dump);
	    }

	    volFragmentAddr = ntohl(volFragment.sameNameChain);
	} /*wvf*/

	volInfoAddr = ntohl(volInfo.sameNameChain);
	if ( volInfoAddr )
	{
	    code = dbread(ut, volInfoAddr, &volInfo, sizeof(volInfo));
	    if (code)
	    	goto abort;
	}
    } /*wvi*/

exit:
    if ( selectedDumpAddr == 0 )
    {
	code = BUDB_NOENT;
	goto abort;
    }

    code = FillDumpEntry(ut, selectedDumpAddr, deptr);
    if ( code )
	goto abort;
    osi_Free(volumeName, strlen(volumeName)+1);
    lock_ReleaseShared(&(db.lock));
    code = ubik_EndTrans(ut);
    LogDebug(1, "found dump for fileset at %ld\n", selectedDumpAddr);
    LogDebug(2, "Leaving BUDB_FindDump, code %ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);

abort:
    lock_ReleaseShared(&(db.lock));
    ubik_AbortTrans(ut);
    LogDebug(1, "failed to find dump for fileset %s\n", volumeName);
    LogDebug(2, "Leaving BUDB_FindDump, code %ld\n", code);
    osi_Free(volumeName, strlen(volumeName)+1);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}

/* BUDB_FindLatestDump
 *	Find the latest dump of volumeset vsname with dump name dname.
 * entry:
 *	vsname - volumeset name
 *	dname - dumpname
 */

error_status_t
BUDB_FindLatestDump(h, nvsname, ndumpPath, dumpentry)
     handle_t h;
     bakNameString_t   *nvsname, *ndumpPath;
     budb_dumpEntry *dumpentry;
{
    struct ubik_trans *ut;
    struct dump d;
    dbadr curdbaddr, retdbaddr;
    Date latest;
    long dumpNameLength;
    char *dumpNamePtr = 0;
    long code;
    char *rtn_Name="BUDB_FindLatestDump";
    int cancelState, cancelCode;
    char *vsname, *dumpPath;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }
    vsname = (char *)osi_Alloc(strlen((char *)nvsname)+1);
    if (vsname == (char *)NULL) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    strcpy(vsname, (char *)nvsname);
    dumpPath = (char *)osi_Alloc(strlen((char *)ndumpPath)+1);
    if (dumpPath == (char *)NULL) {
      osi_Free(vsname, strlen(vsname)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    strcpy(dumpPath, (char *)ndumpPath);
    /* construct the name of the dump */
    dumpNameLength=strlen((char *)vsname) + strlen(tailCompPtr(dumpPath)) + 2;
    dumpNamePtr = (char *) osi_Alloc(dumpNameLength);
    if ( dumpNamePtr == (char *)NULL ) {
      osi_Free(vsname, strlen(vsname)+1);
      osi_Free(dumpPath, strlen(dumpPath)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    code = InitRPC(&ut, LOCKREAD, (int *) 1);
    if ( code ) {
      osi_Free(vsname, strlen(vsname)+1);
      osi_Free(dumpPath, strlen(dumpPath)+1);
      osi_Free(dumpNamePtr, dumpNameLength);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(code);
    }
    LogDebug(2, "Entering BUDB_FindLatestDump: \n");
    strcpy(dumpNamePtr, (char *)vsname);
    strcat(dumpNamePtr, ".");
    strcat(dumpNamePtr, tailCompPtr(dumpPath));
    LogDebug(1, "finding latest dump for fileset family %s, dump path %s, dump set %s; ", vsname, dumpPath, dumpNamePtr);

    /* Lookup on dumpname */
    lock_ObtainWrite(&(db.lock));
    code = ht_LookupEntry(ut, &db.dumpName, dumpNamePtr, &curdbaddr, &d);
    lock_ConvertWToS(&(db.lock));
    if ( code )
    {
    	goto abort;
    }

    if ( curdbaddr == 0 )
    {
	/* nothing found with the specified dump name */
        LogDebug(1, "failed to locate a dump for dump set %s; ", dumpNamePtr);
	code = BUDB_NODUMPNAME;
	goto abort;
    }
    LogDebug(1, "did find hash table entry, now read dump entry to verify; ");
    latest = 0;
    retdbaddr = 0;
    while ( curdbaddr != 0 )
    {
	if ( (strcmp(d.dumpPath, (char *)dumpPath) == 0)
	&&   (strcmp(d.dumpName, dumpNamePtr) == 0)
	&&   (ntohl(d.created) > latest)
	   )
	{
	    latest = ntohl(d.created);
	    retdbaddr = curdbaddr;
	}
	curdbaddr = ntohl(d.nameHashChain);
	if ( dbread(ut, curdbaddr, &d, sizeof(d)) )
	{
	    code = BUDB_IO;
	    goto abort;
	}
    }

    if ( retdbaddr == 0 )
    {
	/* no matches found */
        LogDebug(1, "the hash table seems to be out of date; ");
        LogDebug(1, "failed to locate a dump for dump set %s; ", dumpNamePtr);
	code = BUDB_NODUMPNAME;
	goto abort;
    }
    else
    {
	/* return the dump found */
        LogDebug(1, "found dump entry at location %ld; ", retdbaddr);
	FillDumpEntry(ut, retdbaddr, dumpentry);
    }

exit:
    lock_ReleaseShared(&(db.lock));
    code = ubik_EndTrans(ut);
    if (code)
      LogDebug(1, "search for latest dump failed, code=%ld\n", code);
    else
      LogDebug(1, "search for latest dump successful\n");
    LogDebug(2, "Leaving BUDB_FindLatestDump, code %ld\n", code);
    osi_Free(vsname, strlen(vsname)+1);
    osi_Free(dumpPath, strlen(dumpPath)+1);
    if ( dumpNamePtr )
      osi_Free(dumpNamePtr, dumpNameLength);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);

abort:
    lock_ReleaseShared(&(db.lock));
    LogDebug(1, "search for latest dump failed, code=%ld\n", code);
    LogDebug(2, "Leaving BUDB_FindLatestDump, code %ld\n", code);
    osi_Free(vsname, strlen(vsname)+1);
    osi_Free(dumpPath, strlen(dumpPath)+1);
    if ( dumpNamePtr )
    	osi_Free(dumpNamePtr, dumpNameLength);
    ubik_AbortTrans(ut);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}


error_status_t
BUDB_FinishDump (h, dump)
  handle_t h;
  budb_dumpEntry *dump;
{   long code;
    struct ubik_trans *ut;
    struct dump d;
    dbadr a;
    char *rtn_Name="BUDB_FinishDump";
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }
    if (code = InitRPC (&ut, LOCKWRITE, (int *)1/*this_op*/)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    lock_ObtainWrite(&(db.lock));
    code = ht_LookupEntry (ut, &db.dumpIden, &dump->id, &a, &d);
    lock_ConvertWToS(&(db.lock));
    if (code) {
    abort:
      lock_ReleaseShared(&(db.lock));
      ubik_AbortTrans(ut);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    if (a == 0) {
      code = BUDB_NODUMPID;
      goto abort;
    }

/*
  Remove this error check to allow interop with old butcs
    if (!(dump->flags & BUDB_DUMP_INPROGRESS)) {
      code = BUDB_DUMPNOTINUSE;
      goto abort;
    }
*/
    dump->flags &= ~BUDB_DUMP_INPROGRESS;
    d.flags = htonl(dump->flags);

    /* if creation time specified set it */
    if (dump->created) d.created = htonl(dump->created);
    dump->created = ntohl(d.created);

    /* fill in various fields on output */
    lock_UpgradeSToW(&(db.lock));
    if (code = dbwrite (ut, a, &d, sizeof(d))) {
      lock_ConvertWToS(&(db.lock));
      goto abort;
    }
    if (code = set_header_word (ut, lastUpdate, htonl(time(0)))) {
      lock_ConvertWToS(&(db.lock));
      goto abort;
    }
    lock_ReleaseWrite(&(db.lock));
    code = ubik_EndTrans(ut);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;
}

error_status_t
BUDB_FinishTape (h, tape)
  handle_t h;
  budb_tapeEntry *tape;
{   long  code;
    struct ubik_trans *ut;
    struct tape t;
    dbadr a;
    char *rtn_Name="BUDB_FinishTape";
    int cancelState, cancelCode;
    struct dump       d;


    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }
    if (code = InitRPC (&ut, LOCKWRITE, (int *)1/*this_op*/)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    lock_ObtainWrite(&(db.lock));
    code = ht_LookupEntry (ut, &db.tapeName, tape->name, &a, &t);
    lock_ConvertWToS(&(db.lock));
    if (code || !a) {
      if (!code) code = BUDB_NOTAPENAME;
      goto abort;
    }

    /* Read the dump structure */

	code = dbread(ut, ntohl(t.dump), &d, sizeof(d));
	if (code)
	  goto abort;

    /* search for the right tape on the rest of the chain */

    while (ntohl(d.id) != tape->dump) {
	a = ntohl(t.nameHashChain);
	if (!a) {
		 code = BUDB_NOTAPENAME;
		 goto abort;
	       }

	code = dbread(ut, a, &t, sizeof(t));
	if (code)
	  goto abort;
	
	code = dbread(ut, ntohl(t.dump), &d, sizeof(d));
	if (code)
	  goto abort;
	} 


    if ((ntohl(t.flags) & BUDB_TAPE_BEINGWRITTEN) == 0)
	{code = BUDB_TAPENOTINUSE; goto abort;}
    /* t.nBytes = htonl(tape->nBytes); */
    t.nFiles = htonl(tape->nFiles);
    tape->flags &= ~BUDB_TAPE_BEINGWRITTEN;
    t.flags = htonl(tape->flags);
    lock_UpgradeSToW(&(db.lock));
    if (dbwrite (ut, a, &t, sizeof(t))) {
      lock_ConvertWToS(&(db.lock));
      code = BUDB_IO; goto abort;
    }
    if (code = set_header_word (ut, lastUpdate, htonl(time(0)))) {
      lock_ConvertWToS(&(db.lock));
      goto abort;
    }
    lock_ReleaseWrite(&(db.lock));
    code = ubik_EndTrans(ut);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;

  abort: 
    lock_ReleaseShared(&(db.lock));
    ubik_AbortTrans(ut);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;

}

/* BUDB_GetDumps
 *	return a set of dumps that match the specified criteria
 * entry:
 *	call - rx call
 *	majorVersion - version of interface structures. Permits compatibility
 *		checks to be made
 *	flags - for search and select operations. Broken down into flags
 *		for name, start point, end point and time.
 *	name - name to search for. Interpretation based on flags
 *	end
 *	index
 *	nextIndexP
 *	dbTimeP
 * exit:
 *	nextIndexP
 *	dbTimeP - time at which the database was last modified. Up to
 *		caller (client) to take appropriate action if database
 *		modified between successive calls
 *	dumps - list of matching dumps
 * notes:
 *	currently supported are:
 *	BUDB_OP_DUMPNAME
 *	BUDB_OP_DUMPID

 *
 *
 */
error_status_t
BUDB_GetDumps (h, majorVersion, flags, nname, start, end,
		    index, nextIndexP, dbTimeP, dumps)
  handle_t h;
  long majorVersion;			/* version of interface structures */
  long  flags;				/* search & select controls */
  bakNameString_t *nname;		/* s&s parameters */
  long  start;
  long  end;
  long  index;				/* start index of returned entries */
  long *nextIndexP;			/* output index for next call */
  long *dbTimeP;
  budb_dumpList **dumps;		/* pointer to buffer */
{   long  code;
    struct ubik_trans *ut;
    struct dump d;
    dbadr da;
    long nameFlags, startFlags, endFlags, timeFlags;
    struct returnList list;
    char *rtn_Name="BUDB_GetDumps";
    int cancelState, cancelCode;
    char *name;

    *dumps = (budb_dumpList *)rpc_ss_allocate(sizeof(budb_dumpList));
    if (*dumps == (budb_dumpList *)NULL) {
      return(BUDB_NOMEM);
    }
    (*dumps)->budb_dumpList_len=0; /* to avoid copying on error cases */
    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }
    name = (char *)osi_Alloc(strlen((char *)nname)+1);
    if (name == (char *)NULL) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    strcpy(name, (char *)nname);
    if (majorVersion != CUR_BUDB_VERSION) {
      osi_Free(name, strlen(name)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_OLDINTERFACE;
    }
    if (index < 0) {
      osi_Free(name, strlen(name)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_ENDOFLIST;
    }
    if (code = InitRPC (&ut, LOCKREAD, (int *)1/*this_op*/)) {
      osi_Free(name, strlen(name)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    nameFlags = flags & BUDB_OP_NAMES;
    startFlags = flags & BUDB_OP_STARTS;
    endFlags = flags & BUDB_OP_ENDS;
    timeFlags = flags & BUDB_OP_TIMES;

    InitReturnList (&list, 0);

    lock_ObtainShared(&(db.lock));
    if (nameFlags == BUDB_OP_DUMPNAME) {
	if (startFlags == BUDB_OP_DUMPID) {
	  code = BUDB_BADFLAGS;
	  goto abort;
	}
	lock_UpgradeSToW(&(db.lock));
	code = ht_LookupEntry (ut, &db.dumpName, name, &da, &d);
	lock_ConvertWToS(&(db.lock));
	if (code) goto abort;
	if (da == 0) {code = BUDB_NODUMPNAME; goto abort;}

	/* not yet implemented */
	if (startFlags || endFlags || timeFlags)
	    { code = BUDB_BADFLAGS; goto abort; }

	while (1) {
	    if (strcmp (d.dumpName, (char *)name) == 0) {
		code = AddToReturnList (&list, da, 0);
		if (code) goto abort;
	    }
	    da = d.nameHashChain;	/* get next dump w/ name */
	    if (da == 0) break;
	    da = ntohl(da);
	    if (dbread (ut, da, &d, sizeof(d)))
		{ code = BUDB_IO; goto abort; }
	}
    }
    else
    if ( nameFlags == BUDB_OP_VOLUMENAME )
    {
#ifdef PA
	struct volInfo vi;

	LogError(0, "NYI, BUDB_OP_VOLUMENAME\n");
	goto abort;


	if ( startFlags != BUDB_OP_STARTTIME )
		goto abort;

	/* lookup a dump by volumename and time stamp. Find the most recent
	 * dump of the specified volumename, that occured before the supplied
	 * time
	 */

	/* get us a volInfo for name */
	lock_UpgradeSToW(&(db.lock));
	code = ht_LookupEntry(ut, &db.volName, name, &da, &vi);
	lock_ConvertWToS(&(db.lock));
	if ( code )
		goto abort;

	while (1)
	{
	    /* now iterate over all the entries of this name */
	    for ( va = vi.firstFragment; va != 0; va = v.sameNameChain )
	    {
		va = ntohl(va);
		if ( dbread(ut, va, &v, sizeof(v)) )
	    	goto abort;

		if date on fragment > date
	    	ignore it - too recent;

		if ( date on fragment < date && date on fragment > bestfound )
	    	bestfound = date on fragment;

	    } /* for va */

	    da = vi.sameNameChain;
	    if ( da == 0 )
	    	break;
	    da = ntohl(da);
	    if ( dbread(ut, da, &vi, sizeof(vi)) )
	    	goto abort;
	}

	if nothing found
		return error

	from saved volfragment address, compute dump.
	otherwise, return dump found
#endif /* PA */

    }  else if (startFlags == BUDB_OP_DUMPID) {
	if (endFlags || timeFlags)	/* undefined */
	    { code = BUDB_BADFLAGS; goto abort; }
	lock_UpgradeSToW(&(db.lock));
	code = ht_LookupEntry (ut, &db.dumpIden, &start, &da, &d);
	lock_ConvertWToS(&(db.lock));
	if (code) goto abort;
	if (da == 0) {code = BUDB_NODUMPID; goto abort;}
	/* if (nameFlags == BUDB_OP_DUMPNAME) ; */ /* handled above */

	/* NYI */
	if (nameFlags)
	    { code = BUDB_BADFLAGS; goto abort; }

	code = AddToReturnList (&list, da, 0);
	if (code) goto abort;
    }
    else
    if ( endFlags == BUDB_OP_NPREVIOUS )
    {
	struct wantDumpRock rock;
	struct chosenDump *ptr, *nextPtr;

	extern wantDump(), rememberDump();

	/* no other flags should be set */

	/* end specifies how many dumps */
	if ( end == 0 )
	{
	    code = BUDB_BADFLAGS;
	    goto abort;
	}

	bzero((char *)&rock, sizeof(rock));
	rock.maxDumps = end;

	lock_UpgradeSToW(&(db.lock));
	scanHashTable(ut, &db.dumpName, wantDump, rememberDump,
		      (char *) &rock);
	lock_ConvertWToS(&(db.lock));

	ptr = rock.chain;
	while ( ptr )
	{
	    nextPtr = ptr->next;
	    AddToReturnList (&list, ptr->addr, 0);
	    (void)osi_Free(ptr, sizeof(struct chosenDump));
	    ptr = nextPtr;
	}
    }
    else
    {
	code = BUDB_BADFLAGS;
	goto abort;
    }

    code = SendReturnDumpList (ut, &list, FillDumpEntry,
			   sizeof(budb_dumpEntry),
			   index, nextIndexP, dbTimeP, dumps);
    if (code) goto abort;
    if (*nextIndexP == -1) /* returned all elements */
      free(list.elements);
    lock_ReleaseShared(&(db.lock));
    code = ubik_EndTrans(ut);
    osi_Free(name, strlen(name)+1);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;

abort:
    free(list.elements);
    osi_Free(name, strlen(name)+1);
    lock_ReleaseShared(&(db.lock));
    ubik_AbortTrans(ut);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;
}

error_status_t
BUDB_GetServerInterfaces (h, serverInterfacesP)
     handle_t h;
     dfs_interfaceList *serverInterfacesP;
{
    error_status_t st;
    char *rtn_Name="BUDB_GetServerInterfaces";
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if (callPermitted(h) == 0) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }
    dfs_GetServerInterfaces (BUDB_v4_0_s_ifspec, serverInterfacesP, &st);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return st;
}

error_status_t
BUDB_GetTapes (h, majorVersion, flags, nname, start, end, index, nextIndexP,
	       dbTimeP, tapes)
  handle_t h;
     long   majorVersion;		/* version of interface structures */
     long  flags;			/* search & select controls */
     bakNameString_t *nname;			/* s&s parameters */
     long  start;
     long  end;				/* reserved: MBZ */
     long  index;			/* start index of returned entries */
     long *nextIndexP;			/* output index for next call */
     long *dbTimeP;
     budb_tapeList **tapes;		/* pointer to buffer */
{
    int tapeSelected;
    struct ubik_trans *ut;
    struct tape t;
    dbadr ta;
    long nameFlags, startFlags, endFlags, timeFlags;
    struct returnList list;
    long  code = 0;
    char *rtn_Name="BUDB_GetTapes";
    int cancelState, cancelCode;
    char *name;

    *tapes = (budb_tapeList *)rpc_ss_allocate(sizeof(budb_tapeList));
    if (*tapes == (budb_tapeList *)NULL) {
      return(BUDB_NOMEM);
    }
    (*tapes)->budb_tapeList_len=0;
    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }

    name = (char *)osi_Alloc(strlen((char *)nname)+1);
    if (name == (char *)NULL) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    strcpy(name, (char *)nname);
    if (majorVersion != CUR_BUDB_VERSION) {
      osi_Free(name, strlen(name)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_OLDINTERFACE;
    }
    if (index < 0) {
      osi_Free(name, strlen(name)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_ENDOFLIST;
    }
    code = InitRPC (&ut, LOCKREAD, (int *) 1 /*this_op*/ );
    if (code) {
      osi_Free(name, strlen(name)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    nameFlags = flags & BUDB_OP_NAMES;
    startFlags = flags & BUDB_OP_STARTS;
    endFlags = flags & BUDB_OP_ENDS;
    timeFlags = flags & BUDB_OP_TIMES;

    InitReturnList (&list, 0);

    lock_ObtainShared(&(db.lock));
    if (nameFlags == BUDB_OP_TAPENAME)
    { /*it*/
	lock_UpgradeSToW(&(db.lock));
	code = ht_LookupEntry (ut, &db.tapeName, name, &ta, &t);
	lock_ConvertWToS(&(db.lock));
	if ( code )
	    ABORT(code);

	if (ta == 0)
	    ABORT(BUDB_NOTAPENAME);

	/* NYI */
	if ( (startFlags & ~BUDB_OP_DUMPID)
	||    endFlags
	||    timeFlags
	   )
	{
	    ABORT(BUDB_BADFLAGS);
	}

	/* follow the hash chain to the end */
	while ( ta != 0 )
	{ /*w*/
	    tapeSelected = 0;

	    if ( startFlags & BUDB_OP_DUMPID )
	    {
		struct dump tempDump;

		/* read in the dump */
		code = dbread(ut, ntohl(t.dump), &tempDump, sizeof(tempDump));
		if ( code )
		    ABORT(code);

		/* check if both name and dump id match */
		if ( (strcmp((char *)name, t.name) == 0)
		&&   (ntohl(tempDump.id) == start)
		   )
		    tapeSelected = 1;
	    }
	    else
	    {
		if ( strcmp((char *)name, t.name) == 0 )
		    tapeSelected = 1;
	    }

	    /* if it matched the criteria */
	    if ( tapeSelected )
	    {
		code = AddToReturnList (&list, ta, 0);
		if ( code )
		    ABORT(code);
	    }

	    ta = ntohl(t.nameHashChain);
	    if ( ta )
		dbread(ut, ta, &t, sizeof(t));
	} /*w*/
    } /*it*/
    else
    {
	ABORT( BUDB_BADFLAGS );
    }

    code = SendReturnTapeList (ut, &list, FillTapeEntry,
			   sizeof(budb_tapeEntry),
			   index, nextIndexP, dbTimeP, tapes);
    if (code)
	ABORT(code);

error_exit:					/* and normal exit */
    if (*nextIndexP == -1)
      (void)free(list.elements);
    osi_Free(name, strlen(name)+1);
    lock_ReleaseShared(&(db.lock));
    code = ubik_EndTrans(ut);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;

abort_exit:
    (void)free(list.elements);
    osi_Free(name, strlen(name)+1);
    lock_ReleaseShared(&(db.lock));
    ubik_AbortTrans(ut);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}

/* BUDB_GetVolumes
 *	get a set of volumes according to the specified criteria.
 *	See BUDB_GetDumps for general information on parameters
 *	Currently supports:
 *	1) volume match - returns volumes based on volume name only.
 *	2) flags = BUDB_OP_DUMPID in which case name is a volume name
 *		and start is a dumpid. Returns all volumes of the specified
 *		name on the selected dumpid.
 */

error_status_t
BUDB_GetVolumes (h, majorVersion, flags, nname, start, end,
		      index, nextIndexP, dbTimeP, volumes)
  handle_t h;
  long  majorVersion;			/* version of interface structures */
  long  flags;				/* search & select controls */
  bakNameString_t *nname;		/*  - parameters for search */
  long  start;				/*  - usage depends which BUDP_OP_* */
  long  end;				/*  - bits are set */
  long  index;				/* start index of returned entries */
  long *nextIndexP;			/* output index for next call */
  long *dbTimeP;
  budb_volumeList **volumes;		/* pointer to buffer */
{
    long  code;
    struct ubik_trans *ut;
    struct volInfo vi;
    dbadr via;
    long nameFlags, startFlags, endFlags, timeFlags;
    struct returnList list;
    char *rtn_Name="BUDB_GetVolumes";
    int cancelState, cancelCode;
    char *name;

    *volumes = (budb_volumeList *)rpc_ss_allocate(sizeof(budb_volumeList));
    if (*volumes == (budb_volumeList *)NULL) {
      return(BUDB_NOMEM);
    }
    (*volumes)->budb_volumeList_len=0;
    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }
    name = (char *)osi_Alloc(strlen((char *)nname)+1);
    if (name == (char *)NULL) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    strcpy(name, (char *)nname);
    if (majorVersion != CUR_BUDB_VERSION) {
      osi_Free(name, strlen(name)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_OLDINTERFACE;
    }
    if (index < 0) {
      osi_Free(name, strlen(name)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_ENDOFLIST;
    }
    if (code = InitRPC (&ut, LOCKREAD, (int *)1/*this_op*/)) {
      osi_Free(name, strlen(name)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    nameFlags = flags & BUDB_OP_NAMES;
    startFlags = flags & BUDB_OP_STARTS;
    endFlags = flags & BUDB_OP_ENDS;
    timeFlags = flags & BUDB_OP_TIMES;

    InitReturnList (&list, 0);
    lock_ObtainShared(&(db.lock));
    if ( nameFlags == BUDB_OP_VOLUMENAME )
    {
	/* lookup the volume (specified by name) in the dump (specified
	 * by id
	 */

	/* dumpid permissible, all others off */
	if ( ((startFlags & ~BUDB_OP_DUMPID) != 0 ) || endFlags ||   timeFlags)
	    ABORT(BUDB_BADFLAGS);

	/* returns ptr to volinfo of requested name */
	lock_UpgradeSToW(&(db.lock));
	code = ht_LookupEntry (ut, &db.volName, name, &via, &vi);
	lock_ConvertWToS(&(db.lock));
	if (code)
	    ABORT(code);

	if (via == 0)
	   ABORT(BUDB_NOVOLUMENAME);

	/* Iterate over all volume fragments with this name and collect them in
         * the return list. */

	while (1) {
	    struct volFragment v;
	    long va;

	    /* traverse all the volume fragments for this volume info
	     * structure
	     */
	    for (va=vi.firstFragment; va; va=v.sameNameChain) {
		va = ntohl(va);
		if (dbread (ut, va, &v, sizeof(v)))
		    ABORT(BUDB_IO);

		if ( startFlags & BUDB_OP_DUMPID )
		{
		    struct tape atape;
		    struct dump adump;

		    /* get the dump id for this fragment */
		    if (dbread(ut, ntohl(v.tape), &atape, sizeof(atape)))
			ABORT(BUDB_IO);

		    if (dbread(ut, ntohl(atape.dump), &adump, sizeof(adump)))
			ABORT(BUDB_IO);

		    /* dump id does not match */
		    if ( ntohl(adump.id) != start )
			continue;
		}

		code = AddToReturnList (&list, va, 0);
		if (code)
		    ABORT(code);
	    }

	    via = vi.sameNameChain;
	    if (via == 0) break;
	    via = ntohl(via);
	    if (dbread (ut, via, &vi, sizeof(vi)))
		ABORT(BUDB_IO);
	}
    }
    else
    if ( ((nameFlags == 0) || (nameFlags == BUDB_OP_TAPENAME))
    &&   (startFlags == BUDB_OP_DUMPID)
       )
    {
	struct dump dump;
	dbadr dumpAddr;
	struct tape tape;
	dbadr tapeAddr;
	struct volFragment volFrag;
	dbadr volFragAddr;

	/* lookup all volumes for a specified dump id */

	/* no other flags should be set */
	if ( endFlags || timeFlags)
 	    ABORT(BUDB_BADFLAGS);

	/* find the dump */
	lock_UpgradeSToW(&(db.lock));
	code = ht_LookupEntry(ut, &db.dumpIden, &start, &dumpAddr, &dump);
	lock_ConvertWToS(&(db.lock));
	if ( code )
	    ABORT(code);

	/* traverse all the tapes */
	tapeAddr = ntohl(dump.firstTape);
	while ( tapeAddr != 0 )
	{ /*w*/
	    code = dbread(ut, tapeAddr, &tape, sizeof(tape));
	    if ( code )
		ABORT(code);

	    if ( ( nameFlags != BUDB_OP_TAPENAME ) ||
		 ((nameFlags == BUDB_OP_TAPENAME) && (strcmp(tape.name,name) == 0)) ) {
	      
	    /* now return all the volumes */
	    volFragAddr = ntohl(tape.firstVol);
	    while ( volFragAddr != 0 )
	    {
		code = dbread(ut, volFragAddr, &volFrag, sizeof(volFrag));
		if ( code )
		    ABORT(code);

		code = AddToReturnList(&list, volFragAddr, 0);
		if ( code )
		    ABORT(code);

		volFragAddr = ntohl(volFrag.sameTapeChain);
	      }
	    }
	    tapeAddr = ntohl(tape.nextTape);

	} /*w*/
    }
    else
    {
    	ABORT(BUDB_BADFLAGS);
    }

    code = SendReturnVolumeList (ut, &list, FillVolEntry,
			   sizeof(budb_volumeEntry),
			   index, nextIndexP, dbTimeP, volumes);
    if (code)
	ABORT(code);

error_exit:
    if (*nextIndexP == -1)
      (void)free(list.elements);
    osi_Free(name, strlen(name)+1);
    lock_ReleaseShared(&(db.lock));
    code = ubik_EndTrans(ut);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;

abort_exit:
    (void)free(list.elements);
    osi_Free(name, strlen(name)+1);
    lock_ReleaseShared(&(db.lock));
    ubik_AbortTrans(ut);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;
}

error_status_t
BUDB_UseTape (h, tape, new)
  handle_t h;
  budb_tapeEntry *tape;		/* tape info */
  long  *new;				/* set if tape is new */
{   long  code;
    struct ubik_trans *ut;
    struct tape t;
    dbadr a;
    struct dump d;
    dbadr da;
    char *rtn_Name="BUDB_UseTape";
    int cancelState, cancelCode;
    dbadr tapeAddr;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOTPERMITTED);
    }
    if (strlen ((char *)tape->name) >= sizeof(t.name)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_BADARGUMENT;
    }
    if (code = InitRPC (&ut, LOCKWRITE, (int *)1/*this_op*/)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    LogDebug(2, "Entering BUDB_UseTape: \n");
    LogDebug(1, "selecting tape %s containing dump %lu; ", tape->name,
	     tape->dump);
    *new = 0;
    lock_ObtainShared(&(db.lock));
    bzero ((char *)&t, sizeof(t));
    lock_UpgradeSToW(&(db.lock));
    if (code = AllocStructure (ut, tape_BLOCK, 0, &a, &t)) {
      lock_ConvertWToS(&(db.lock));
      goto abort;
    }
    LogDebug(1, "allocating tape block at %ld; ", a);
    strcpy (t.name, (char *)tape->name);
    if (code = ht_HashIn (ut, &db.tapeName, a, &t)) {
      lock_ConvertWToS(&(db.lock));
      goto abort;
    }
    *new = 1;
    LogDebug(1, "tape is new; ");
    /* Since deleting a tape may change the dump (if its the same one), read in
       the dump after the call to DeleteTape. */

    code = ht_LookupEntry (ut, &db.dumpIden, &tape->dump, &da, &d);
    lock_ConvertWToS(&(db.lock));
    if (code) goto abort;
    if (da == 0) {code = BUDB_NODUMPID; goto abort;}

     /* Check for duplicates */
 
     tapeAddr = ntohl (d.firstTape);
 
     while (tapeAddr != 0) {
       struct tape temptape;
 
         code = dbread(ut, tapeAddr, &temptape, sizeof(temptape));
           if (code)
             goto abort;
       if (strncmp((char *)tape->name, temptape.name, BU_MAXTAPELEN) == 0) {
       /* duplicate, throw it out */
       code = 0;
      LogDebug(1, "Duplicate tape found, name = %s. Ignored \n", temptape.name);
       goto abort;
       }
	tapeAddr = ntohl(temptape.nextTape);
      }

    /* Tape is not a duplicate */
 
    /* Use user supplied written time (if any) */
    t.written = (tape->written ? htonl(tape->written) : htonl(time(0)));
    t.expires = htonl(tape->expires);
    t.dump = htonl(da);
    t.nextTape = d.firstTape;
    t.seq = htonl(tape->seq);
    t.useCount = htonl(tape->useCount);

    t.tapeid = 0;			/* for future use */
    t.mediaType = 0;

    d.firstTape = htonl(a);

    /* increment number of tapes in the dump */
    if (tape->seq >= ntohl(d.tapes.maxTapes))
	d.tapes.maxTapes = htonl(tape->seq);
/*
  Remove this error check to allow interop with old butcs
    if (tape->flags & BUDB_TAPE_BEINGWRITTEN) {
      code = BUDB_TAPEINUSE;
      goto abort;
    }
*/
    tape->flags |= BUDB_TAPE_BEINGWRITTEN;
    t.flags = htonl(tape->flags);

    LogDebug(1, "tape parameters: written date %lu, expire date %lu, dump ID %lu, position in tape sequence %ld, use count %ld; ",
	     ntohl(t.written), ntohl(t.expires), da, tape->seq,
	     tape->useCount);
    lock_UpgradeSToW(&(db.lock));
    if (dbwrite (ut, a, &t, sizeof(t)) ||
	dbwrite (ut, da, &d, sizeof(d))) {
      lock_ConvertWToS(&(db.lock));
      code = BUDB_IO; goto abort;
    }
    if (code = set_header_word (ut, lastUpdate, htonl(time(0)))) {
      lock_ConvertWToS(&(db.lock));
      goto abort;
    }
    LogDebug(1, "successfully added tape\n");
    lock_ReleaseWrite(&(db.lock));
    code = ubik_EndTrans(ut);
    LogDebug(2, "Leaving BUDB_UseTape, code %ld\n", code);
    if (code)
      LogDebug(1, "failed to obtain tape, code=%ld\n", code);
    else
      LogDebug(1, "successfully obtained tape\n");
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;

abort:
    lock_ReleaseShared(&(db.lock));
    ubik_AbortTrans(ut);
    LogDebug(2, "Leaving BUDB_UseTape, code %ld\n", code);
    LogDebug(1, "failed to obtain tape, code=%ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;

}


/* ---------------------------------------------
 * debug interface routines
 * ---------------------------------------------
 */

error_status_t
BUDB_T_DumpHashTable (h, type, nfilename)
  handle_t h;
  long   type;
  bakNameString_t *nfilename;
{   long code;
    struct ubik_trans *ut;
    struct memoryHashTable *mht;
    int   ent;
    char *rtn_Name="BUDB_T_DumpHashTable";
    char  path[32];
    FILE *DUMP = (FILE *)NULL;
    char *filename;
    int   length;
    unsigned long hash;
    dbadr a, first_a;
    char  e[sizeof(struct block)];	/* unnecessarily conservative */
    int   e_size;
    int   old;
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    filename = (char *)osi_Alloc(strlen((char *)nfilename+1));
    if (filename == (char *)NULL) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    strcpy(filename, (char *)nfilename);
    if (strlen ((char *)filename) >= sizeof(path)-5) {
      osi_Free(filename, strlen(filename)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_BADARGUMENT;
    }
    if (code = AwaitInitialization()) {
      osi_Free(filename, strlen(filename)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }
    if (code = InitRPC (&ut, LOCKWRITE, (int *)1/*this_op*/)) {
      osi_Free(filename, strlen(filename)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return code;
    }

    lock_ObtainShared(&(db.lock));
    if ((mht = ht_GetType (type, &e_size)) == 0) {
      code = BUDB_BADARGUMENT;
      goto abort;
    }
    sprintf (path, "/tmp/%s", filename);
    DUMP = fopen (path, "w");
    if (DUMP == 0) {
	code = BUDB_BADARGUMENT;
      abort:
	lock_ReleaseShared(&(db.lock));
	ubik_AbortTrans(ut);
	if (DUMP) fclose (DUMP);
	osi_Free(filename, strlen(filename)+1);
	bak_enableCancel(rtn_Name, cancelState, &cancelCode);
	return code;
    }

    ent = 0;
    for (old=0; ; old++) {
	length = (old ? mht->oldLength : mht->length);
	if (length) fprintf (DUMP, "Dumping %sHash Table:\n", (old?"Old ":""));
	for (hash=0; hash<length; hash++) {
	    if ((hash&15) == 0) pthread_yield();
	    lock_UpgradeSToW(&(db.lock));
	    a = ht_LookupBucket (ut, mht, hash, old);
	    lock_ConvertWToS(&(db.lock));
	    first_a = a;
	    while (a) {
		if (code = dbread (ut, a, e, e_size)) goto abort;
		ent++;
		if (a == first_a) fprintf (DUMP, "  in bucket %d at %d is ", hash, a);
		else fprintf (DUMP, "    at %d is ", a);
		switch (type) {
		  case HT_dumpIden_FUNCTION:
		    fprintf (DUMP, "%d\n", ntohl(((struct dump *)e)->id));
		    break;
		  case HT_dumpName_FUNCTION:
		    fprintf (DUMP, "%s\n", ((struct dump *)e)->dumpName);
		    break;
		  case HT_tapeName_FUNCTION:
		    fprintf (DUMP, "%s\n", ((struct tape *)e)->name);
		    break;
		  case HT_volName_FUNCTION:
		    fprintf (DUMP, "%s\n", ((struct volInfo *)e)->name);
		    break;
		}
		lock_UpgradeSToW(&(db.lock));
		if ((ht_HashEntry (mht, e) % length) != hash) {
  		    lock_ConvertWToS(&(db.lock));
		    code = BUDB_DATABASEINCONSISTENT;
		    goto abort;		/* wrong hash bucket? */
		}
		lock_ConvertWToS(&(db.lock));
		a = ntohl(*(dbadr *)(e + mht->threadOffset));
	    }
	}
	if (old) break;
    }
    fprintf (DUMP, "%d entries found\n", ent);
    if (ntohl(mht->ht->entries) != ent) {
	code = BUDB_DATABASEINCONSISTENT;
	goto abort;
    }
    lock_ReleaseShared(&(db.lock));
    code = ubik_EndTrans(ut);

  done:
    osi_Free(filename, strlen(filename)+1);
    if (DUMP) fclose (DUMP);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return code;
}

error_status_t
BUDB_T_GetVersion(h, majorVersion)
  handle_t h;
  long *majorVersion;
{
    struct ubik_trans *ut;
    long code;
    char *rtn_Name="BUDB_T_GetVersion";
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    *majorVersion=0;
    code = InitRPC (&ut, LOCKREAD, 0);
    if ( code ) {
      LogError(code, "BUDB_T_GetVersion: InitRPC failed\n");
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return (code);
    }
    *majorVersion = CUR_BUDB_VERSION;

    code = ubik_EndTrans(ut);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}

/* BUDB_T_DumpDatabase
 *	dump as much of the database as possible int /tmp/<filename>
 */

error_status_t
BUDB_T_DumpDatabase(h, nfilename)
  handle_t h;
  bakNameString_t *nfilename;
{
    FILE *dumpfid;
    int	entrySize;
    struct ubik_trans *ut;
    char *rtn_Name="BUDB_T_DumpDatabase";
    char *pathPrefix = "/tmp/";
    char *path = (char *)NULL;
    dbadr dbAddr;
    int type, old, length, hash;
    long code;
    char *filename;

    struct memoryHashTable *mht;
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);
    filename = (char *)osi_Alloc(strlen((char *)nfilename)+1);
    if (filename == (char *)NULL) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    strcpy(filename, (char *)nfilename);
    path = (char *) osi_Alloc(strlen(pathPrefix)+strlen((char *)filename)+1);
    if ( path == (char *)NULL ) {
      osi_Free(filename, strlen(filename)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_NOMEM);
    }
    sprintf(path, "%s%s", pathPrefix, filename);
    dumpfid = fopen(path, "w");
    if ( dumpfid == 0 )
    {
      (void)osi_Free(filename, strlen(filename)+1);
      (void)osi_Free(path, strlen(path)+1);
      code = BUDB_BADARGUMENT;
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return (code);
    }

    code = AwaitInitialization();
    if ( code ) {
      (void)osi_Free(filename, strlen(filename)+1);
      (void)osi_Free(path, strlen(path)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(code);
    }
    code = InitRPC (&ut, LOCKWRITE, (int *) 1 /*this_op*/);
    if ( code ) {
      (void)osi_Free(filename, strlen(filename)+1);
      (void)osi_Free(path, strlen(path)+1);
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return (code);
    }
    lock_ObtainShared(&(db.lock));
    /* dump all items in the database */
    for ( type=1; type<=HT_MAX_FUNCTION; type++ )
    { /*ft*/
        lock_UpgradeSToW(&(db.lock));
	mht = ht_GetType (type, &entrySize);
	lock_ConvertWToS(&(db.lock));
	if ( mht == 0 )
	{
	    code = BUDB_BADARGUMENT;
	    goto error;
	}

	for ( old =0; old <= 1; old++ )
	{ /*fo*/
	    length = ( old ? mht->oldLength : mht->length);
	    if ( length == 0 )
	    	continue;

	    fprintf (dumpfid, "Dumping %s Hash Table:\n", (old ? "Old ":""));

	    for ( hash = 0; hash < length; hash++ )
	    { /*f*/
	        lock_UpgradeSToW(&(db.lock));
		dbAddr = ht_LookupBucket (ut, mht, hash, old);
		lock_ConvertWToS(&(db.lock));

		while ( dbAddr )
		{ /*w*/
		    switch (type)
		    { /*s*/
		      case HT_dumpIden_FUNCTION:
			{
			    struct dump hostDump, diskDump;

			    code = dbread (ut, dbAddr, &diskDump,
					   sizeof(diskDump));
			    if ( code )
			    	goto error;

			    fprintf(dumpfid, "\ndumpId hash %d, entry at %d\n",
				    hash, dbAddr);
			    fprintf(dumpfid, "----------------------------\n");
			    dump_ntoh(&diskDump, &hostDump);
			    printDump(dumpfid, &hostDump);
			    dbAddr = hostDump.idHashChain;
			}
			break;

		      case HT_dumpName_FUNCTION:
			{
			    struct dump hostDump, diskDump;

			    code = dbread (ut, dbAddr, &diskDump,
					   sizeof(diskDump));
			    if ( code )
			    	goto error;

			    fprintf(dumpfid, "\ndumpname hash %d, entry at %d\n",
				    hash, dbAddr);
			    fprintf(dumpfid, "----------------------------\n");
			    dump_ntoh(&diskDump, &hostDump);
			    printDump(dumpfid, &hostDump);
			    dbAddr = hostDump.nameHashChain;
			}
			break;

		      case HT_tapeName_FUNCTION:
			{
			    struct tape hostTape, diskTape;

			    code = dbread (ut, dbAddr, &diskTape,
					   sizeof(diskTape));
			    if ( code )
			    	goto error;

			    fprintf(dumpfid, "\ntapename hash %d, entry at %d\n",
				    hash, dbAddr);
			    fprintf(dumpfid, "----------------------------\n");
			    tape_ntoh(&diskTape, &hostTape);
			    printTape(dumpfid, &hostTape);
			    dbAddr = hostTape.nameHashChain;
			}
			break;

		      case HT_volName_FUNCTION:
			{
			    struct volInfo hostVolInfo, diskVolInfo;

			    code = dbread (ut, dbAddr, &diskVolInfo,
					   sizeof(diskVolInfo));
			    if ( code )
			    	goto error;

			    fprintf(dumpfid, "\nvolname hash %d, entry at %d\n",
				    hash, dbAddr);
			    fprintf(dumpfid, "----------------------------\n");
			    volInfo_ntoh(&diskVolInfo, &hostVolInfo);
			    printVolInfo(dumpfid, &hostVolInfo);
			    dbAddr = hostVolInfo.nameHashChain;

			    volFragsDump(ut, dumpfid,
					 hostVolInfo.firstFragment);
			}
			break;

		      default:
			fprintf(dumpfid, "unknown type %d\n", type);
			break;

		    } /*s*/
		} /*w*/
	    } /*f*/
	} /*fo*/
    } /*ft*/

error:
    osi_Free(filename, strlen(filename)+1);
    lock_ReleaseShared(&(db.lock));
    code = ubik_EndTrans(ut);		/* is this safe if no ut started ?*/
    if ( dumpfid )
    	fclose(dumpfid);
    if ( path )
    	osi_Free(path, strlen(path)+1);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}

volFragsDump(ut, dumpfid, dbAddr)
    struct ubik_trans *ut;
    FILE *dumpfid;
    dbadr dbAddr;
{
    struct volFragment hostVolFragment, diskVolFragment;
    long code;

    while ( dbAddr )
    {
	code = dbread (ut, dbAddr, &diskVolFragment, sizeof(diskVolFragment));

	/* don't be fussy about errors */
	if ( code )
	{
	    fprintf(dumpfid, "volFragsDump: Error reading database\n");
	    return(0);
	}

	fprintf(dumpfid, "\nvolfragment entry at %d\n", dbAddr);
	fprintf(dumpfid, "----------------------------\n");
	volFragment_ntoh(&diskVolFragment, &hostVolFragment);
	printVolFragment(dumpfid, &hostVolFragment);
	dbAddr = hostVolFragment.sameNameChain;
    }
    return(0);
}




/* utilities - structure printing routines
 *	currently used for debug only
 */

#ifdef notdef
/* utilities - network to host conversion
 *	currently used for debug only
 */

volFragmentDiskToHost(diskVfPtr, hostVfPtr)
     struct volFragment *diskVfPtr, *hostVfPtr;
{
    hostVfPtr->vol = ntohl(diskVfPtr->vol);
    hostVfPtr->sameNameChain = ntohl(diskVfPtr->sameNameChain);
    hostVfPtr->tape = ntohl(diskVfPtr->tape);
    hostVfPtr->sameTapeChain = ntohl(diskVfPtr->sameTapeChain);
    hostVfPtr->position = ntohl(diskVfPtr->position);
    hostVfPtr->clone = ntohl(diskVfPtr->clone);
    hostVfPtr->incTime = ntohl(diskVfPtr->incTime);
    hostVfPtr->startByte = ntohl(diskVfPtr->startByte);
    hostVfPtr->nBytes = ntohl(diskVfPtr->nBytes);
    hostVfPtr->flags = ntohs(diskVfPtr->flags);
    hostVfPtr->sequence = ntohs(diskVfPtr->sequence);
}

volInfoDiskToHost(diskViPtr, hostViPtr)
     struct volInfo *diskViPtr, *hostViPtr;
{
    strcpy(hostViPtr->name, diskViPtr->name);
    hostViPtr->nameHashChain = ntohl(diskViPtr->nameHashChain);
    hostViPtr->id = ntohl(diskViPtr->id);
    strcpy(hostViPtr->server, diskViPtr->server);
    hostViPtr->partition = ntohl(diskViPtr->partition);
    hostViPtr->flags = ntohl(diskViPtr->flags);
    hostViPtr->sameNameHead = ntohl(diskViPtr->sameNameHead);
    hostViPtr->sameNameChain = ntohl(diskViPtr->sameNameChain);
    hostViPtr->firstFragment = ntohl(diskViPtr->firstFragment);
    hostViPtr->nFrags = ntohl(diskViPtr->nFrags);
}

tapeDiskToHost(diskTapePtr, hostTapePtr)
     struct tape *diskTapePtr, *hostTapePtr;
{
    strcpy(hostTapePtr->name, diskTapePtr->name);
    hostTapePtr->nameHashChain = ntohl(diskTapePtr->nameHashChain);
    hostTapePtr->flags = ntohl(diskTapePtr->flags);

    /* tape id conversion here */
    hostTapePtr->written = ntohl(diskTapePtr->written);
    hostTapePtr->nBytes = ntohl(diskTapePtr->nBytes);
    hostTapePtr->nFiles = ntohl(diskTapePtr->nFiles);
    hostTapePtr->nVolumes = ntohl(diskTapePtr->nVolumes);
    hostTapePtr->seq = ntohl(diskTapePtr->seq);
    hostTapePtr->dump = ntohl(diskTapePtr->dump);
    hostTapePtr->nextTape = ntohl(diskTapePtr->nextTape);
    hostTapePtr->firstVol = ntohl(diskTapePtr->firstVol);
    hostTapePtr->useCount = ntohl(diskTapePtr->useCount);
}

dumpDiskToHost(diskDumpPtr, hostDumpPtr)
     struct dump *diskDumpPtr, *hostDumpPtr;
{
    hostDumpPtr->id = ntohl(diskDumpPtr->id);
    hostDumpPtr->idHashChain = ntohl(diskDumpPtr->idHashChain);
    strcpy(hostDumpPtr->dumpName, diskDumpPtr->dumpName);
    strcpy(hostDumpPtr->dumpPath, diskDumpPtr->dumpPath);
    strcpy(hostDumpPtr->volumeSet, diskDumpPtr->volumeSet);
    hostDumpPtr->nameHashChain = ntohl(diskDumpPtr->nameHashChain);
    hostDumpPtr->flags = ntohl(diskDumpPtr->flags);
    hostDumpPtr->parent = ntohl(diskDumpPtr->parent);
    hostDumpPtr->created = ntohl(diskDumpPtr->created);
/*  hostDumpPtr->incTime = ntohl(diskDumpPtr->incTime); */
    hostDumpPtr->nVolumes = ntohl(diskDumpPtr->nVolumes);

    /* tapeset conversion here */

    hostDumpPtr->firstTape = ntohl(diskDumpPtr->firstTape);

    /* principal conversion here */
}


/* NOTUSED */
checkHash(ut,hashType)
     struct ubik_trans *ut;
     int hashType;
{
    struct memoryHashTable *mhtPtr;
    int entrySize, hashTableLength;
    int bucket;
    int old;
    long code;

    lock_UpgradeSToW(&(db.lock));
    mhtPtr = ht_GetType(hashType, &entrySize);
    lock_ConvertWToS(&(db.lock));
    if ( mhtPtr == 0 )
	ERROR(-1);

    for ( old = 0; old < 1; old++)
    {
	LogDebug(5, "\nold = %d\n", old);
	printMemoryHashTable(stdout, mhtPtr);
	LogDebug(5, "\n");
	hashTableLength = ( old ? mhtPtr->oldLength : mhtPtr->length);

	for ( bucket = 0; bucket < hashTableLength; bucket++ )
	{
	    dbadr entryAddr;

	    lock_UpgradeSToW(&(db.lock));
	    entryAddr = ht_LookupBucket (ut, mhtPtr, bucket, old);
	    lock_ConvertWToS(&(db.lock));
	    while (entryAddr != 0)
	    {
	        LogDebug(6, "bucket %d has disk addr %d\n", bucket, entryAddr);
		switch (hashType)
		{
		  case HT_dumpIden_FUNCTION:
		  {
		      struct dump diskDump, hostDump;

		      code = dbread(ut, entryAddr, &diskDump, entrySize);
		      if ( code )
			  ERROR(-1);

		      dump_ntoh(&diskDump, &hostDump);
		      printDump(stdout, &hostDump);
		      entryAddr = hostDump.idHashChain;
		  }
		  break;

		  case HT_dumpName_FUNCTION:
		    break;

		  case HT_tapeName_FUNCTION:
		    break;

		  case HT_volName_FUNCTION:
     	          {
		      struct volInfo diskVolInfo, hostVolInfo;

		      code = dbread(ut, entryAddr, &diskVolInfo, entrySize);
		      if ( code )
			  ERROR(-1);
		      volInfo_ntoh(&diskVolInfo, &hostVolInfo);
		      printVolInfo(stdout, &hostVolInfo);
		      entryAddr = hostVolInfo.nameHashChain;
		    break;
		  }
	        }
	    }
	}
    }
error_exit:
    return(code);
}
#endif /* notdef */


/* declare and initialize the manager's entry point vector  */
BUDB_v4_0_epv_t   BUDB_v4_0_manager_epv = {
	 BUDB_AddVolume,
	 BUDB_CreateDump,
	 BUDB_DeleteDump,
	 BUDB_DeleteTape,
	 BUDB_DeleteVDP,
	 BUDB_FindClone,
	 BUDB_FindDump,
	 BUDB_FindLatestDump,
	 BUDB_FinishDump,
	 BUDB_FinishTape,
	 BUDB_GetDumps,
	 BUDB_GetTapes,
	 BUDB_GetVolumes,
	 BUDB_UseTape,
	 BUDB_GetText,
	 BUDB_GetTextVersion,
	 BUDB_SaveText,
	 BUDB_FreeAllLocks,
	 BUDB_FreeLock,
	 BUDB_GetInstanceId,
	 BUDB_GetLock,
	 BUDB_DbVerify,
	 BUDB_DumpDB,
	 BUDB_RestoreDbHeader,
	 BUDB_T_GetVersion,
	 BUDB_T_DumpHashTable,
	 BUDB_T_DumpDatabase,
	 BUDB_GetServerInterfaces,
	 BUDB_AddVolumes
};
