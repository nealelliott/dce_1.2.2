/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_alloc.c,v $
 * Revision 1.1.76.1  1996/10/02  16:59:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:14  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * db_alloc.c -- Block and Structure Allocation
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bakserver/db_alloc.c,v 1.1.76.1 1996/10/02 16:59:47 damon Exp $")

#include <sys/types.h>
#include <netinet/in.h>
#include <dcedfs/param.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <budb_errs.h>
#include <database.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

static int nEntries[NBLOCKTYPES];
static int sizeEntries[NBLOCKTYPES];
/* The bakserver database defined in database.c */
extern struct memoryDB db;

long InitDBalloc ()
{
    int i;

    nEntries[0] = 0;
    sizeEntries[0] = 0;

    nEntries[volFragment_BLOCK] = NvolFragmentS;
    nEntries[volInfo_BLOCK] = NvolInfoS;
    nEntries[tape_BLOCK] = NtapeS;
    nEntries[dump_BLOCK] = NdumpS;

    sizeEntries[volFragment_BLOCK] = (int)sizeof(((struct vfBlock *)0)->a[0]);
    sizeEntries[volInfo_BLOCK] = sizeof(((struct viBlock *)0)->a[0]);
    sizeEntries[tape_BLOCK] = sizeof(((struct tBlock *)0)->a[0]);
    sizeEntries[dump_BLOCK] = sizeof(((struct dBlock *)0)->a[0]);

    /* initialize the spares */
    for(i=spare1_BLOCK; i<=spare5_BLOCK; i++) {
	nEntries[i] = 0;
	sizeEntries[i] = 0;
    }
    LogDebug(2, "InitDBalloc: initial allocation(count, size); fileset fragments (%ld, %ld), fileset info (%ld, %ld), tape (%ld, %ld), dump (%ld, %ld)\n",
	     NvolFragmentS, sizeEntries[volFragment_BLOCK],
	     NvolInfoS, sizeEntries[volInfo_BLOCK],
	     NtapeS, sizeEntries[tape_BLOCK],
	     NdumpS, sizeEntries[dump_BLOCK]);
    return 0;
}

/* AllocBlock
 *	allocate a (basic) database block. Extends the database if
 *	no free blocks are available.
 * exit:
 *	0 - aP points to a cleared block
 *	n - error
 */

long
AllocBlock (ut, block, aP)
     struct ubik_trans *ut;
     struct block *block;			/* copy of data */
     dbadr *aP;					/* db addr of block */
{  
    dbadr a;
    
    if (db.h.freePtrs[0] == 0) {
	/* if there are no free blocks, extend the database */
	a = ntohl(db.h.eofPtr);
	LogDebug(2, "AllocBlock: no free blocks, extending db, eof %ld\n",
		 a+BLOCKSIZE);
	if (set_header_word (ut, eofPtr, htonl(a+BLOCKSIZE)))
	  return BUDB_IO;
    }
    else
    {
	a = ntohl(db.h.freePtrs[0]);
	LogDebug(2, "AllocBlock: there are free blocks, chosen %ld\n", a);
	if ( dbread (ut, a, (char *)block, sizeof(block->h))	/* read hdr */
	||   set_header_word (ut, freePtrs[0], block->h.next)	/* set next */
	   )
	  return BUDB_IO;
    }

    /* clear, write to disk and return the block */
    bzero ((char *)block, sizeof(struct block));
    if(dbwrite(ut, a, (char *)block, sizeof(*block)))
      return BUDB_IO;
    *aP = a;
    LogDebug(2, "AllocBlock: returning block %ld\n", *aP);
    return 0;
}

/* FreeBlock
 *	Free a basic block
 * entry:
 *	bh - block header ptr. Memory copy of the block header.
 *	a - disk address of the block
 */

long 
FreeBlock (ut, bh, a)
  struct ubik_trans *ut;
  struct blockHeader *bh;		/* copy of data */
  dbadr a;				/* db address of block */
{
    if (a != BlockBase(a)) db_panic ("Block addr no good");

    bzero ((char *)bh, sizeof(struct blockHeader));
    bh->next = db.h.freePtrs[0];
    if (set_header_word (ut, freePtrs[0], htonl(a)) ||
	dbwrite (ut, a, (char *)bh, sizeof(*bh))) {
      bh->next = (dbadr)NULL;
      return BUDB_IO;
    }
    LogDebug(2, "FreeBlock: freeing block %ld\n", a);
    return 0;
}


/* AllocStructure
 * entry:
 *	type - type of structure to allocate
 *	related - address of related block
 *	saP - db addr of structure
 *	s -  structure data
 */

long
AllocStructure(ut, type, related, saP, s)
     struct ubik_trans *ut;
     char  type;	
     dbadr related;
     dbadr *saP;
     char *s;
{
    dbadr a;				/* block addr */
    struct block b;			/* copy of data */
    int   i;				/* block structure array index */
    long *bs;				/* ptr to first word of structure */
    int   nFree;
    
    if ( (type == 0)
    ||   (type > MAX_STRUCTURE_BLOCK_TYPE)
       )
    {
        LogDebug(2, "AllocStructure: bad structure type %d\n", (long)type);
	db_panic ("bad structure type");
    }
    bs = (long *)b.a;			/* ptr to first structure of block */

    if (db.h.freePtrs[type] == 0)
    {
	/* no free items of specified type */

	if ( AllocBlock(ut, &b, &a)
	||   set_header_word (ut, freePtrs[type], htonl(a))
	   )
	{
	    return BUDB_IO;
	}

	b.h.next = 0;
	b.h.type = type;
	b.h.flags = 0;
	b.h.nFree = ntohs(nEntries[type]-1);
	*bs = 1;			/* not free anymore */

	if ( dbwrite (ut, a, (char *)&b, sizeof(b)) )
	    return BUDB_IO;
	LogDebug(2, "AllocStructure: no free blocks, allocated new one %ld\n", 
		 a);
    } 
    else
    {
	int count = 10;

	/* Only do 10 (or so) at a time, to avoid transactions which modify
         * many buffer.
	 */
	LogDebug(2, "AllocStructure: there are free blocks, use them\n");
	while (1)
	{
	    a = ntohl(db.h.freePtrs[type]);
	    if ( dbread (ut, a, (char *)&b, sizeof(b)) )
		return BUDB_IO;

	    nFree = ntohs(b.h.nFree);
	    if (nFree == 0)
		db_panic ("nFree is zero");
	    
	    /* Completely empty blocks go to generic free list if there are
	     * more blocks on this free list 
	     */
	    if (b.h.next && (nFree == nEntries[type]) && (count-- > 0))
	    {
		if ( set_header_word (ut, freePtrs[type], b.h.next)
		||   FreeBlock (ut, &b.h, a)
		   )
		{
		    return BUDB_IO;
		}
		LogDebug(2, "AllocStructure: add to free block list %ld\n", a);
	    } 
	    else
	    { 
		/* we found a free structure */
		if (nFree == 1)
		{
		    /* if last free one: unthread block */
		    if (set_header_word (ut, freePtrs[type], b.h.next))
			return BUDB_IO;
		}
		break;
	    }
	}
	
	/* find the free structure - arbitrarily uses first word as
	 * allocated/free status. PA.
	 */
	i = 0;
	while ( *bs )
	{
	    i++;
	    bs = (long *) ((char *) bs + sizeEntries[type]);
	}
	
	if (i >= nEntries[type])
	    db_panic ("free count inconsistent with block");

	b.h.nFree = htons(nFree - 1);
	*bs = 1L; /* mark this entry as used, and write this to db */
	if (dbwrite (ut, a, (char *)&b, sizeof(b))) return BUDB_IO;
    }
    *(long *)s = 1;			/* make sure structure is not free */
    *saP = a + ((char *)bs - (char *)&b);
    
    LogDebug(2, "AllocStructure: allocated at %d, block at %d, offset %d\n",
	   *saP, a, ((char *)bs - (char *)&b) );
    /* caller must write back at least first word of structure */
    return 0;
}


/*
 * db.lock is held for write when called 
 */
long FreeStructure (ut, type, sa)
  struct ubik_trans *ut;
  char  type;				/* type of structure to allocate */
  dbadr sa;				/* db addr of structure */
{
    struct block b;			/* block containing free struct */
    dbadr a;				/* db address of block */
    int   nFree;			/* new free structures count */
    long freeWord;

    if ((type == 0) || (type > MAX_STRUCTURE_BLOCK_TYPE)) {
      LogDebug(2, "FreeStructure: bad structure type %d\n", (long)type);
      db_panic ("bad structure type");
    }
    a = BlockBase(sa);
    if (dbread(ut, a, (char *)&b, sizeof(b))) return BUDB_IO;
    if (type != b.h.type) db_panic ("block and structure of different types");

    b.h.nFree = htons (nFree = ntohs(b.h.nFree) + 1);
    if (nFree > nEntries[type]) db_panic ("free count too large");
    if (nFree == 1) {			/* add to free list for type */
	b.h.next = db.h.freePtrs[type];
	if (set_header_word (ut, freePtrs[type], htonl(a))) return BUDB_IO;
    }

    /* zero out the structure in the block, and write it to disk */
    bzero((char *)&b+sa-a, sizeEntries[type]);
    /* mark the structure as free, and write out block header */
    if (set_word_offset (ut, sa, &freeWord, 0, 0)  ||
	dbwrite (ut, a, (char *)&b, sizeof(b))) return BUDB_IO;
    return 0;
}
