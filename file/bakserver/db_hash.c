/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_hash.c,v $
 * Revision 1.1.98.1  1996/10/02  16:59:52  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:16  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * db_hash.c -- A general purpose hash table for the bak database
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bakserver/db_hash.c,v 1.1.98.1 1996/10/02 16:59:52 damon Exp $")

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <dcedfs/param.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <budb_errs.h>
#include <database.h>
#include <error_macros.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

/* The bakserver database defined in database.c */
extern struct memoryDB db;
int sizeFunctions[HT_MAX_FUNCTION+1];
int nHTBuckets = NhtBucketS;		/* testing: we need small HT blocks */

/* ht_TableSize - return the size of table necessary to represent a hashtable
 * of given length in memory.  It basically rounds the length up by the number
 * of buckets per block. */

int ht_TableSize (length)
  int length;
{   int n;
    if (length == 0) return 0;
    n = (length + nHTBuckets-1) / nHTBuckets;
    return n*sizeof(struct memoryHTBlock *);
}

/* ht_ResetT - resets the in-memory representation of a hashtable block array.
 * It also resets the global variable nHTBuckets. */

static void ht_ResetT (blocksP, sizeP, length)
  struct memoryHTBlock ***blocksP;
  int  *sizeP;
  int   length;
{   struct memoryHTBlock **b = *blocksP;
    int newsize;
    int n;
    int i;

    nHTBuckets = ntohl(db.h.nHTBuckets);
    if (b) {
	n = *sizeP / sizeof(b[0]);
	newsize = ht_TableSize (length);
	if (*sizeP != newsize) { /* The value of nHTBuckets changed */
	    /* free all blocks in the old array */
	    for (i=0; i<n; i++)
		if (b[i]) osi_Free (b[i], sizeof(struct memoryHTBlock));
	    (void)osi_Free(b, *sizeP);
	    *sizeP = 0;
	    *blocksP = 0;
	} else {
	    /* invalidate the blocks of the array  */
	    for (i=0; i<n; i++)
		if (b[i]) b[i]->valid = 0;
	}
    }
}

/* ht_Reset
 *	reinitialize a memory hash table. 
 * 	Calls ht_ResetT to invalidate the two block arrays.
 */
    
void ht_Reset (mht)
  struct memoryHashTable *mht;
{   struct hashTable *ht;

    if (!(mht && (ht = mht->ht)))
	db_panic ("some ht called with bad mht"); 
    mht->threadOffset = ntohl(ht->threadOffset);
    mht->length = ntohl(ht->length);
    mht->oldLength = ntohl(ht->oldLength);
    mht->progress = ntohl(ht->progress);
    ht_ResetT (&mht->blocks, &mht->size, mht->length);
    ht_ResetT (&mht->oldBlocks, &mht->oldSize, mht->oldLength);
}

/* 
 * InitDBhash - When server starts, do hash table initialization.
 * db.lock held for write by caller.
 */
long InitDBhash ()
{

#ifdef notdef
    if (test) {
	/* Use very small HT blocks in test.  When rebuilding db use this. */
	nHTBuckets = 10;
    }
#endif

    sizeFunctions[0] = 0;

    sizeFunctions[HT_dumpIden_FUNCTION] = sizeof(struct dump);
    sizeFunctions[HT_dumpName_FUNCTION] = sizeof(struct dump);
    sizeFunctions[HT_volName_FUNCTION] = sizeof(struct volInfo);
    sizeFunctions[HT_tapeName_FUNCTION] = sizeof(struct tape);

    db.volName.ht = &db.h.volName;
    db.tapeName.ht = &db.h.tapeName;
    db.dumpName.ht = &db.h.dumpName;
    db.dumpIden.ht = &db.h.dumpIden;
    return 0;
}

/* ht_DBInit - When rebuilding database, this sets up the hash tables. */

void ht_DBInit ()
{
    db.h.nHTBuckets = htonl(nHTBuckets);

    {   struct volInfo *s = 0;
	db.h.volName.threadOffset = htonl((char *)&s->nameHashChain - (char *)s);
	db.h.volName.functionType = htonl(HT_volName_FUNCTION);
    }
    {   struct tape *s = 0;
	db.h.tapeName.threadOffset = htonl((char *)&s->nameHashChain - (char *)s);
	db.h.tapeName.functionType = htonl(HT_tapeName_FUNCTION);
    }
    {   struct dump *s = 0;
	db.h.dumpName.threadOffset = htonl((char *)&s->nameHashChain - (char *)s);
	db.h.dumpName.functionType = htonl(HT_dumpName_FUNCTION);

	db.h.dumpIden.threadOffset = htonl((char *)&s->idHashChain - (char *)s);
	db.h.dumpIden.functionType = htonl(HT_dumpIden_FUNCTION);
    }
    ht_Reset (&db.volName);
    ht_Reset (&db.tapeName);
    ht_Reset (&db.dumpName);
    ht_Reset (&db.dumpIden);
}

long ht_AllocTable (ut, mht)
  struct ubik_trans *ut;
  struct memoryHashTable *mht;
{   struct hashTable *ht;
    long  code;
    int   len;
    int	  nb;				/* number of blocks for hashTable */
    int	  i;
    struct memoryHTBlock **b;

    if (!(mht && (ht = mht->ht))) db_panic ("some ht called with bad mht"); 
    if (ht->length || mht->blocks) db_panic ("previous table still allocated");

    len = ntohl(ht->entries) * 2;	/* allow room to grow */
    /* should actually use ht_TableSize(len) here */
    nb = (len + nHTBuckets-1) / nHTBuckets;
    if (nb == 0) nb = 1;		/* at least one block */
    len = nb * nHTBuckets;		/* new hash table length */

    mht->size = nb*sizeof(struct memoryHTBlock *);
    b = mht->blocks = (struct memoryHTBlock **) osi_Alloc (mht->size);
    bzero ((char *)b, mht->size);

    for (i=0; i<nb; i++) {
	b[i]=(struct memoryHTBlock *)osi_Alloc(sizeof (struct memoryHTBlock));
	code = AllocBlock (ut, (struct block *)&b[i]->b, &b[i]->a);
	if (code) return code;
	b[i]->valid = 0;

	b[i]->b.h.type = hashTable_BLOCK;

	/* thread the blocks */
	if (i) b[i-1]->b.h.next = htonl(b[i]->a);
    }
    for (i=0; i<nb; i++)
    {
	code = dbwrite (ut, b[i]->a, (char *)&b[i]->b,
			sizeof(struct htBlock) + (nHTBuckets -
						  NhtBucketS)*sizeof(dbadr));
	if (code)
	    return code;
    }
    if (code = set_word_addr (ut, 0, &db.h, &ht->table, htonl(b[0]->a)))
	return code;

    if (code = set_word_addr (ut, 0, &db.h, &ht->length, htonl(len)))
	return code;
    mht->length = len;
    return 0;
}

long ht_FreeTable (ut, mht)
  struct ubik_trans *ut;
  struct memoryHashTable *mht;
{   struct hashTable *ht;
    long  code;
    struct blockHeader bh;
    dbadr a,na;

    if (!(mht && (ht = mht->ht))) db_panic ("some ht called with bad mht"); 
    if (ht->oldLength == 0) db_panic ("no table to free");

    ht_ResetT (&mht->oldBlocks, &mht->oldSize, 0);

    for (a=ntohl(ht->oldTable); a; a=na) {
	if (dbread (ut, a, (char *)&bh, sizeof(bh))) 
	{
	    Log("ht_FreeTable: dbread failed\n");
	    return BUDB_IO;
	}
	na = ntohl(bh.next);
	if (code = FreeBlock (ut, &bh, a)) return code;
    }
    if (set_word_addr (ut, 0, &db.h, &ht->oldTable, 0) ||
	set_word_addr (ut, 0, &db.h, &ht->oldLength, 0) ||
	set_word_addr (ut, 0, &db.h, &ht->progress, 0))
	return BUDB_IO;
    mht->oldLength = mht->progress = 0;
    return 0;
}

long 
ht_GetTableBlock (ut, mht, hash, old, blockP, boP)
  struct ubik_trans *ut;
  struct memoryHashTable *mht;
  unsigned long hash;
  int   old;
  struct memoryHTBlock **blockP;
  int  *boP;
{   
    struct hashTable *ht;
    struct memoryHTBlock **b;
    int	  hi,bi;
    struct memoryHTBlock ***blocksP;
    int  *sizeP;
    int   n;
    int	  i;
    int   length;
    dbadr ta;

    if ( (mht == 0)
    ||   ((ht = mht->ht) == 0)
       )
    {
    	db_panic ("some ht called with bad mht"); 
    }

    *blockP = 0;

    if (old) {
	if ((length = mht->oldLength) == 0) return 0; /* no entries */
	hi = hash % length;
	if (hi < mht->progress) return 0; /* no such entry */
	blocksP = &mht->oldBlocks;
	sizeP = &mht->oldSize;
    } else {
	if ((length = mht->length) == 0) return 0; /* no entries */
	hi = hash % length;
	blocksP = &mht->blocks;
	sizeP = &mht->size;
    }

    bi = hi / nHTBuckets;		/* block index */
    *boP = hi - bi*nHTBuckets;		/* block offset ptr */

    if (*blocksP == 0) {
	*sizeP = ht_TableSize (length);
	*blocksP = (struct memoryHTBlock **) osi_Alloc (*sizeP);
	bzero ((char *)*blocksP, *sizeP);
    }
    n = *sizeP / sizeof (struct memoryHTBlock *);
    if (bi >= n) db_panic ("table size inconsistent");
    b = *blocksP;

    /* find an allocated block or the beginning of the block array */
    for (i=bi; (i>0) && (b[i] == 0); i--);

    while (1) {
	if (b[i] == 0) {
	    if (i == 0) { /* the first block is found from the hashTable */
		ta = ntohl(old ? ht->oldTable : ht->table);
		if (ta == 0) db_panic ("non-zero length, but no table");
	    }
	    /* else ta is set from last time around loop */
	    b[i] = (struct memoryHTBlock *) osi_Alloc (sizeof (struct memoryHTBlock));
	    b[i]->a = ta;
	    b[i]->valid = 0;
	}

	if (!b[i]->valid) {
	    if (dbread (ut, b[i]->a, (char *)&b[i]->b, sizeof(struct htBlock)))
		return BUDB_IO;
	    b[i]->valid = 1;
	}

	if (i == bi)
	{
	    *blockP = b[bi];
	    /* printf("ht_GetTableBlock: hash %d block %d offset %d\n",
		hash, *blockP, *boP); */
	    return 0;
        }

	ta = ntohl(b[i++]->b.h.next);	/* get next ptr from current block */
    }
}

/* ht_MaybeAdjust
 *	Decide when to push the current hash table to the old hash table.
 *	The entries in the old hash table are VALID, and are slowly hashed
 *	into the current table.
 */

static long 
ht_MaybeAdjust (ut, mht)
     struct ubik_trans *ut;
     struct memoryHashTable *mht;
{   
    struct hashTable *ht = mht->ht;
    int numberEntries = ntohl(ht->entries);

    /* old hash table must be empty */
    if ( mht->oldLength != 0)
	return(0);

    /* if the table is more than 50% full, we need to grow
     * or, if more than one block and less than one eighth full, we need to
     * shrink it. Either way, we push the entries to the old hash table
     */

    if ( (numberEntries > mht->length*2)
    ||   ((mht->length > nHTBuckets) && (numberEntries*8 < mht->length))
       )
    {
	/* push current hash table to old hash table */
	ht->oldLength = ht->length;
	ht->oldTable = ht->table;
	ht->progress = 0;
	ht->length = 0;
	ht->table = 0;
	if (dbwrite (ut, ((char *)ht - (char *)&db.h), (char *)ht, 
		     sizeof (*ht))) 
	    return BUDB_IO;
	ht_Reset (mht);
	LogDebug(4, "ht_MaybeAdjust: push ht to old\n");
    }
    return 0;
}

dbadr ht_LookupBucket (ut, mht, hash, old)
  struct ubik_trans *ut;
  struct memoryHashTable *mht;
  unsigned long hash;
  int old;
{   struct memoryHTBlock *block;
    int   bo;
    long  code;

    if ((old ? mht->oldLength : mht->length) == 0) return 0;
    code = ht_GetTableBlock (ut, mht, hash, old, &block, &bo);
    if (code || (block == 0)) return 0;
    return ntohl(block->b.bucket[bo]);
}

/* This function is not too bad, for small hash tables, but suffers, I think,
 * from insufficient mixing of the hash information. */

unsigned long Old2StringHashFunction (str)
  unsigned char *str;
{   unsigned long hash = 1000003;	/* big prime to make "" hash nicely */
    while (*str) hash = (hash<<1) + (hash>>31) + *str++;
    return hash;
}

/* This was actually a coding error, and produces dreadful results.  The
 * problem is that the hash needs to be mixed up not the incoming character. */

unsigned long Old3StringHashFunction (str)
  unsigned char *str;
{   unsigned long hash = 1000003;	/* big prime to make "" hash nicely */
    while (*str) hash += (*str++) * 0x072a51a4;
    return hash;
}

/* This function is pretty good.  Its main problem is that the low two bits of
 * the hash multiplier are zero which tends to shift information too far left.
 * It behaves especially badly for hash tables whose size is a power of two. */

unsigned long Old4StringHashFunction (str)
  unsigned char *str;
{   unsigned long hash = 1000003;	/* big prime to make "" hash nicely */
    while (*str) hash = (*str++) + hash * 0x072a51a4;
    return hash;
}

/* While this is good for a hash table with 500 buckets it is nearly as bad as
 * #3 with a hash table as big as 8200. */

unsigned long Old5StringHashFunction (str)
  unsigned char *str;
{   unsigned long hash = 1000003;	/* big prime to make "" hash nicely */
    while (*str) hash += (*str++);
    return hash;
}

/* This was an attempt to produce a hash function with the smallest and
 * simplest mixing multiplier.  This is only a little worse than the real one,
 * and the difference seems to be smaller with larger hash tables.  It behaves
 * better than the random hash function. */

unsigned long Old6StringHashFunction (str)
  unsigned char *str;
{   unsigned long hash = 1000003;	/* big prime to make "" hash nicely */
    while (*str) hash = hash*0x81 + (*str++);
    return hash;
}

/* This actually seems to be little better then the real one.  Having the same
 * number of bits but only 5 bits apart seems to produce worse results but
 * having the bits spanning the same range farther apart also doesn't do as
 * well.  All these differences are fairly small, however. */

unsigned long Old7StringHashFunction (str)
  unsigned char *str;
{   unsigned long hash = 1000003;	/* big prime to make "" hash nicely */
    while (*str) hash = hash*0x42108421 + (*str++);
    return hash;
}

/* This function tries to provide some non-linearity by providing some feedback
 * from higher-order bits in the word.  It also uses shifts instead of
 * multiplies, which may be faster on some architectures. */

unsigned long Old8StringHashFunction (str)
  unsigned char *str;
{   unsigned long hash = 1000003;	/* big prime to make "" hash nicely */
    while (*str)
	hash = hash + (hash<<7) + (hash<<14) + (hash<<21) + (hash<<28) +
	    (hash>>17) + *str++;
    return hash;
}

/* This is the result of the above search for good hash functions.  It seems
 * that the choice of multipliers is somewhat arbitrary but has several
 * constraints.  It shouldn't have too many or too few one bits and should be
 * odd.  It behaves beeter than the random hash function. */

unsigned long StringHashFunction (str)
  unsigned char *str;
{   unsigned long hash = 1000003;	/* big prime to make "" hash nicely */
    /* The multiplicative constant should be odd and have a goodly number of
     * one bits. */
    while (*str) hash = (*str++) + hash * 0x10204081;
    return hash;
}

unsigned long IdHashFunction (id)
  unsigned long id;
{   unsigned long l,r;
    id *= 81847;
    l = id | 0xaaaaaaaa;
    r = id | 0x55555555;
    return (l*r);
}

unsigned long ht_HashEntry (mht, e)
  struct memoryHashTable *mht;
  char *e;				/* entry's address (in b) */
{   
    int   type = ntohl(mht->ht->functionType);
    unsigned long retval;

    
    switch (type)
    {
      case HT_dumpIden_FUNCTION:
	retval = IdHashFunction(ntohl(((struct dump *)e)->id));
	LogDebug(4, "HashEntry: dumpid returns %d\n", retval);
	break;

      case HT_dumpName_FUNCTION:
	retval = StringHashFunction(((struct dump *)e)->dumpName);
	LogDebug(4, "HashEntry: dumpname returns %d\n", retval);
	break;

      case HT_tapeName_FUNCTION:
	retval = StringHashFunction(((struct tape *)e)->name);
	LogDebug(4, "HashEntry: tapename returns %d\n", retval);
	break;

      case HT_volName_FUNCTION:
	retval =  StringHashFunction (((struct volInfo *)e)->name);
	LogDebug(4, "HashEntry: volname returns %d\n", retval);
	break;

      default: db_panic ("illegal hash function");
    }

    return(retval);
}


/* ht_GetType
 *	returns a ptr to the memory hash table for the specified hash
 *	list.
 */

struct memoryHashTable *
ht_GetType(type, e_sizeP)
     int   type;
     int  *e_sizeP;
{  
    struct memoryHashTable *mht;

    if ((type <= 0) || (type > HT_MAX_FUNCTION)) 
	return 0;

    if (e_sizeP) *e_sizeP = sizeFunctions[type];
    switch (type) 
    {
      case HT_dumpIden_FUNCTION:
	mht = &db.dumpIden;
	break;

      case HT_dumpName_FUNCTION:
	mht = &db.dumpName;
	break;

      case HT_tapeName_FUNCTION:
	mht = &db.tapeName;
	break;

      case HT_volName_FUNCTION:
	mht = &db.volName;
	break;

      default: return 0;
    }
    if (ntohl(mht->ht->functionType) != type)
	db_panic ("ht types don't match");
    return mht;
}

static int ht_KeyMatch (type, key, e)
  int   type;
  char *key;
  char *e;
{
    switch (type) {
      case HT_dumpIden_FUNCTION:
	return *(dumpId *)key == ntohl(((struct dump *)e)->id);
      case HT_dumpName_FUNCTION:
	return strcmp (key, ((struct dump *)e)->dumpName) == 0;
      case HT_tapeName_FUNCTION:
	return strcmp (key, ((struct tape *)e)->name) == 0;
      case HT_volName_FUNCTION:
	return strcmp (key, ((struct volInfo *)e)->name) == 0;

      default: db_panic ("illegal hash function");
    }
}

/* ht_LookupEntry
 * entry:
 *	ut - ubik transaction
 *	mht - memory hash table ptr
 *	key - hash and lookup key
 * exit:
 *	eaP - dbaddr of entry found or zero if failed
  *	e - contents of located entry
 */

long
ht_LookupEntry (ut, mht, key, eaP, e)
  struct ubik_trans *ut;
  struct memoryHashTable *mht;
  char *key;				/* pointer to lookup key to match */
  dbadr *eaP;				/* db addr of entry found or zero */
  char *e;				/* contents of located entry */
{   struct hashTable *ht;
    int  type;
    int  e_size;
    int  old;
    unsigned long hash;
    dbadr a;

    if (!key || !eaP || !e) db_panic ("null ptrs passed to LookupEntry");
    if (!(mht && (ht = mht->ht))) db_panic ("some ht called with bad mht"); 

    *eaP = 0;				/* initialize not-found indicator */

    type = ntohl(ht->functionType);
    e_size = sizeFunctions[type];
    if (type == HT_dumpIden_FUNCTION)
    	hash = IdHashFunction (*(dumpId *)key);
    else
    	hash = StringHashFunction (key);

    for (old=0; ; old++)
    {
	a = ht_LookupBucket (ut, mht, hash, old);
	while (a) {
	    if (dbread (ut, a, e, e_size))
	    	return BUDB_IO;
	    if (ht_KeyMatch (type, key, e))
	    {
		*eaP = a;
		return 0;
	    }
	    a = ntohl(*(dbadr *)(e + mht->threadOffset));
	}
	if (old) return 0;
    }
}

/* ht_HashInList
 * entry:
 *	opQuota - max # of items to move
 * exit:
 *	opQuota - adjusted to reflect # of moves
 */

static long ht_HashInList (ut, mht, opQuota, block, blockOffset)
     struct ubik_trans *ut;
     struct memoryHashTable *mht;
     int *opQuota;
     struct memoryHTBlock *block;
     int blockOffset;
{
    struct hashTable *ht = mht->ht;
    long  code;
    dbadr ea, next_ea;
    dbadr listA;
    char  e[sizeof(struct block)];	/* unnecessarily conservative */
    int   e_size = sizeFunctions[ntohl(ht->functionType)];

    if (mht->length == 0) 
    {
	if (code = ht_AllocTable (ut, mht)) 
	{
	    Log("ht_HashInList: ht_AllocTable failed\n");
	    return code;
	}
    }

    listA = ntohl(block->b.bucket[blockOffset]);

    if (listA == 0) 
    {
	Log("ht_HashInList: expecting non-zero bucket\n");
	return 0;
    }

    for (ea=listA; ea; ea=next_ea) 
    { /*f*/

	LogDebug(4, "ht_HashInList: move entry at %d, type %d\n",
	    ea, ntohl(mht->ht->functionType));

	if (dbread (ut, ea, e, e_size)) 
	    return BUDB_IO;

        /* LogNetDump((struct dump *) e); */

	/* get the address of the next item on the list */
	next_ea = ntohl(*(dbadr *)(e + mht->threadOffset));

	/* write the link into the bucket */
	code = set_word_addr(ut, block->a, &block->b, 
			     &block->b.bucket[blockOffset], 
			     htonl(next_ea));
	if ( code )
	{
	    Log("ht_HashInList: bucket update failed\n");
	    return(code);
	}

	{   struct memoryHTBlock *block;
	    int   bo;
	    unsigned long hash;

	    /* get the hash value */
	    hash = ht_HashEntry (mht, e) % mht->length;
	    LogDebug(4, "ht_HashInList: moved to %d\n", hash);

	    /* get the new hash table block */
	    code = ht_GetTableBlock (ut, mht, hash, 0/*old*/, &block, &bo);
	    if (code)
            {
		Log("ht_HashInList: ht_GetTableBlock failed\n");
		return code;
	    }
	    if (block == 0)
	    {
		Log("ht_HashInList: ht_GetTableBlock returned 0\n");
		return BUDB_INTERNALERROR;
	    }

	    /* Chain entry at front of bucket;
	     * first threadOffset of entry = bucket
	     * then bucket = addr of entry
	     */
	    if (set_word_offset (ut, ea, e, mht->threadOffset, block->b.bucket[bo]) ||
		set_word_addr (ut, block->a, &block->b, &block->b.bucket[bo], htonl(ea)))
		return BUDB_IO;
	}

	if ( --(*opQuota) == 0 )
	    break;
    } /*f*/
    return 0;
}


/* ht_MoveEntries
 *	The hash table is needs to be re-sized. Move entries from the old
 *	to the new.
 */

static long 
ht_MoveEntries(ut, mht)
     struct ubik_trans *ut;
     struct memoryHashTable *mht;
{   
    struct memoryHTBlock *block;
    unsigned long hash;
    int count;
    int   bo;
    long  code;

    if (mht->oldLength == 0)
	return 0;

    LogDebug(4, "Entering ht_MoveEntries:\n");
    /* we assume here that the hash function will map numbers smaller than the
     * size of the hash table straight through to hash table indexes.
     */
    hash = mht->progress;

    /* get hash table block ? */
    code = ht_GetTableBlock (ut, mht, hash, 1 /*old*/, &block, &bo);
    if (code) 
	return code;

    if (block == 0) 
	return BUDB_INTERNALERROR;

    count = 10;				/* max. # entries to move */

    do {
	if (block->b.bucket[bo])
	{
	    code = ht_HashInList(ut, mht, &count, block, bo);
	    if ( code )
	    {
		Log("ht_MoveEntries: ht_HashInList failed\n");
		return(BUDB_IO);
	    }
	}

	if (block->b.bucket[bo] == 0)
	{
	    /* this bucket is now empty */
	    mht->progress++;
	}

	/* don't exceed the quota of items to be moved */
	if ( count == 0 )
	    break;

    } while (++bo < nHTBuckets);

    if (mht->progress >= mht->oldLength)
	return( ht_FreeTable (ut, mht) );

    if (set_word_addr (ut, 0, &db.h, &mht->ht->progress, htonl(mht->progress)))
    {
	Log("ht_MoveEntries: progress set failed\n");
	return BUDB_IO;
    }
    return 0;
}


#ifdef notdef
static long 
ht_MoveEntries(ut, mht)
     struct ubik_trans *ut;
     struct memoryHashTable *mht;
{   
    unsigned long hash;
    int   bo;
    struct memoryHTBlock *block;
    long  code;

    if (mht->oldLength == 0)
	return 0;

    LogDebug(4, "Entering: ht_MoveEntries:\n");
    /* we assume here that the hash function will map numbers smaller than the
     * size of the hash table straight through to hash table indexes.
     */
    hash = mht->progress;

    /* get hash table block ? */
    code = ht_GetTableBlock (ut, mht, hash, 1 /*old*/, &block, &bo);
    if (code) 
	return code;

    if (block == 0) 
	return BUDB_INTERNALERROR;

    do {
	mht->progress++;
	if (block->b.bucket[bo])
	{
	    code = ht_HashInList(ut, mht, ntohl(block->b.bucket[bo]));
	    if ( code )
	    {
		Log("ht_MoveEntries: ht_HashInList failed\n");
		return(BUDB_IO);
	    }
	    code = set_word_addr(ut, block->a, &block->b, &block->b.bucket[bo],
				 0);
	    if ( code )
	    {
		Log("ht_MoveEntries: clear old entry failed\n");
		return BUDB_IO;
	    }
	    break;
	}
    } while (++bo < nHTBuckets);

    if (mht->progress >= mht->oldLength)
	return( ht_FreeTable (ut, mht) );

    if (set_word_addr (ut, 0, &db.h, &mht->ht->progress, htonl(mht->progress)))
    {
	Log("ht_MoveEntries: progress set failed\n");
	return BUDB_IO;
    }
    return 0;
}
#endif /* notdef */

long 
ht_HashIn(ut, mht, ea, e)
     struct ubik_trans *ut;
     struct memoryHashTable *mht;
     dbadr ea;				/* block db address */
     char *e;				/* entry's address (in b) */
{   
    struct hashTable *ht;
    unsigned long hash;
    struct memoryHTBlock *block;
    int   bo;
    long  code;

    if (!(mht && (ht = mht->ht))) 
	db_panic("some ht called with bad mht"); 

    if (code = ht_MaybeAdjust (ut, mht))
	return code;
    if (mht->length == 0) 
	if (code = ht_AllocTable (ut, mht)) 
	    return code;

    hash = ht_HashEntry (mht, e);
    code = ht_GetTableBlock (ut, mht, hash, 0/*old*/, &block, &bo);
    if (code)
	return code;
    if (block == 0) 
	return BUDB_INTERNALERROR;

    code = set_word_offset(ut, ea, e, mht->threadOffset, block->b.bucket[bo]);
    if ( code )
	return BUDB_IO;
    LogDebug(4, "Hashin: set %d to %d\n", mht->threadOffset, 
	     block->b.bucket[bo]);

    code = set_word_addr(ut, block->a, &block->b, &block->b.bucket[bo],
			 htonl(ea));
    if ( code )
	return BUDB_IO;
    LogDebug(4, "Hashin: set %d to %d\n", &block->b.bucket[bo], htonl(ea));

    code = set_word_addr(ut, 0, &db.h, &ht->entries, htonl(ntohl(ht->entries)
							   + 1));
    if ( code )
	return BUDB_IO;



    
    return ht_MoveEntries (ut, mht);
}

/* RemoveFromList - generic procedure to delete an entry from a list given its
 * head and thread offset.  Only a single long is modified by this routine.
 * The head pointer is modified, in place, using set_word_addr if the entry is
 * at the head of the list, otherwise only the thread of the previous entry is
 * modified.  The entry pointer is only used to calculate the thread offset,
 * but is not otherwise used. */

long RemoveFromList (ut, ea, e, head, ta, t, thread)
  struct ubik_trans *ut;
  dbadr ea;				/* db addr of head structure */
  char *e;				/* head structure */
  dbadr *head;				/* address of head pointer */
  dbadr ta;				/* db addr of strucure to be removed */
  char *t;				/* structure being removed */
  dbadr *thread;			/* pointer to thread pointer */
{   long  code;
    int   threadOffset = ((char *)thread - t);
    dbadr next_a;			/* db addr of next element in list */
    dbadr loop_a;			/* db addr of current list element */

    if (*head == 0) return -1;		/* empty list: not found */
    next_a = ntohl(*head);		/* start at head of list */
    if (next_a == ta) {			/* remove from head of list */
	code = set_word_addr (ut, ea, e, head, *thread);
	return code;
    }
    do {
	loop_a = next_a;
	code = dbread (ut, loop_a+threadOffset, (char *)&next_a, sizeof(dbadr));
	if (code) return code;
	if (next_a == 0) return -1;	/* end of list: not found */
    } while (ta != (next_a = ntohl(next_a)));
    code = dbwrite (ut, loop_a+threadOffset, (char *)thread, sizeof(dbadr));
    return code;
}

long ht_HashOutT (ut, mht, hash, ea, e, old)
  struct ubik_trans *ut;
  struct memoryHashTable *mht;
  unsigned long hash;
  dbadr ea;
  char *e;
  int   old;
{
    struct memoryHTBlock *block;
    int   bo;
    long  code;

    if ((old ? mht->oldLength : mht->length) == 0)
	return -1;
    code = ht_GetTableBlock (ut, mht, hash, old, &block, &bo);
    if (code)
	return code;
    if ((block == 0) || (block->b.bucket[bo] == 0))
	return -1;

    code = RemoveFromList(ut, block->a, (char *)&block->b,
			  &block->b.bucket[bo], ea, e,
			  (dbadr *)(e+mht->threadOffset));
    if (code)
	return code;
#if 0
    net_ea = htonl(ea);
    unthread_ea = *(long *)((char *)e + mht->threadOffset);
    if (block->b.bucket[bo] == net_ea) {
	if (set_word_addr (ut, block->a, &block->b, &block->b.bucket[bo],
			   unthread_ea)) return BUDB_IO;
	goto done;
    }
    loop_a = ntohl(block->b.bucket[bo]);
    while (1) {
	if (dbread (ut, loop_a + mht->threadOffset,
		    (char *)&next_loop_a, sizeof(dbadr)))
	    return BUDB_IO;
	if (next_loop_a == 0) return -1; /* not found */
	if (net_ea == next_loop_a) {
	    if (dbwrite (ut, loop_a + mht->threadOffset,
			 (char *)&unthread_ea, sizeof(dbadr))) return BUDB_IO;
	    goto done;
	}
	loop_a = ntohl (next_loop_a);
    }	
  done:
#endif
    if (set_word_addr (ut, 0, &db.h, &mht->ht->entries,
		       htonl(ntohl(mht->ht->entries) - 1))) return BUDB_IO;
    return 0;
}

long
ht_HashOut (ut, mht, ea, e)
     struct ubik_trans *ut;
     struct memoryHashTable *mht;
     dbadr ea;
     char *e;
{
    unsigned long hash;
    long  code;
    
    if (!mht) 
	db_panic ("some ht called with bad mht"); 
    hash = ht_HashEntry (mht, e);
    if (mht->oldLength) 
    {
	code = ht_HashOutT (ut, mht, hash, ea, e, 1/*old*/);
	if (code == 0) 
	    return 0;
	else
        if (code != -1)
	    ERROR(code);
    }
    if (mht->length == 0)		 /* not found */
	ERROR(BUDB_INTERNALERROR);
    code = ht_HashOutT (ut, mht, hash, ea, e, 0/*old*/);
    if (code == -1)
	ERROR(BUDB_NOENT);
    if (code)
	ERROR(code);

    code = ht_MoveEntries (ut, mht);
    if ( code )
	ERROR(code);
    code = ht_MaybeAdjust (ut, mht);
    if ( code )
	ERROR(code);

error_exit:
    return(code);
}

/* generic hash table traversal routines */


long
scanHashTableBlock(ut, mhtPtr, htBlockPtr, old, length, index,
		   selectFn, operationFn, rockPtr)
     struct ubik_trans *ut;
     struct memoryHashTable *mhtPtr;
     struct htBlock *htBlockPtr;
     int old;
     long length;                       /* size of whole hash table */
     int index;                         /* base index of this block */
     int (*selectFn)();
     int (*operationFn)();
     char *rockPtr;
{
    int type;                           /* hash table type */
    int entrySize;			/* hashed entry size */

    u_long *mapEntryPtr = 0;            /* for status checks */
    
    char entry[sizeof(struct block)];
    dbadr entryAddr, nextEntryAddr;

    int i;
    long code = 0;

    type = ntohl(mhtPtr->ht->functionType);
    entrySize = sizeFunctions[type];

    /* step through this hash table block, being careful to stop
     * before the end of the overall hash table
     */

    for ( i = 0; (i < nHTBuckets) && (index < length); i++, index++ )
    { /*f*/
	entryAddr = 0;
	nextEntryAddr = ntohl( htBlockPtr->bucket[i] );

	/* if this is the old hash table, all entries below the progress mark
	 * should have been moved to the new hash table
	 */
	if (old && (index < mhtPtr->progress) && nextEntryAddr)
                return BUDB_INTERNALERROR;

	/* now walk down the chain of each bucket */
	while ( nextEntryAddr )
	{ /*w*/

	    entryAddr = nextEntryAddr;
	    if ( dbread(ut, entryAddr, &entry[0], entrySize) )
		return(BUDB_INTERNALERROR);

	    if ( (*selectFn)(entryAddr, &entry[0], rockPtr) )
	    {
		(*operationFn)(entryAddr, &entry[0], rockPtr);
	    }

	    nextEntryAddr = ntohl( *((dbadr *)(entry + mhtPtr->threadOffset)));
	} /*w*/

    } /*f*/

    return(0);
}

long
scanHashTable(ut, mhtPtr, selectFn, operationFn, rockPtr)
    struct ubik_trans *ut;
    struct memoryHashTable *mhtPtr;
    int (*selectFn)();
    int (*operationFn)();
    char *rockPtr;
{
    struct htBlock hashTableBlock;
    dbadr tableAddr;                            /* disk addr of hash block */
    int tableLength;                            /* # entries */
    int blockLength;                            /* # blocks */
    int hashIndex;
    int blockIndex, entryIndex;
    int old;
    int i;
    long code = 0;

    extern int nHTBuckets;                      /* # buckets in a hash table */

    for ( old = 0; old <= 1; old++ )
    { /*fo*/
        if (old)
        {
            /* check the old hash table */
            tableLength = mhtPtr->oldLength;
            if ( tableLength == 0 )
                continue;                       /* nothing to do */
	    
            tableAddr = ntohl(mhtPtr->ht->oldTable);
        }
        else
        {
            /* check current hash table */
            tableLength = mhtPtr->length;
            if ( tableLength == 0 )
                continue;                       /* nothing to do */
	    
            tableAddr = ntohl(mhtPtr->ht->table);
        }

	blockLength = (tableLength-1)/nHTBuckets;
        hashIndex = 0;

        /* follow the hash chain */
        for ( i = 0; i <= blockLength; i++ )
        { /*fi*/
            /* chain too short */
            if ( tableAddr == 0 )
                ERROR(BUDB_DATABASEINCONSISTENT);

            code = dbread(ut, tableAddr, &hashTableBlock,
                          sizeof(hashTableBlock));
            if ( code )
                goto error_exit;

            code = scanHashTableBlock(ut, mhtPtr, &hashTableBlock, old,
				      tableLength, hashIndex,
				      selectFn, operationFn,
				      rockPtr);
            if ( code )
                goto error_exit;

            hashIndex += nHTBuckets;
            tableAddr = ntohl(hashTableBlock.h.next);
        } /*fi*/
    } /*fo*/

error_exit:
    return(code);
}



