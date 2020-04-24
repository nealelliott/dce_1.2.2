/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ol_verify.c,v $
 * Revision 1.1.74.1  1996/10/02  17:02:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:26  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1995, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * ol_verify.c -- standalone on-line bak database verification program
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bakserver/ol_verify.c,v 1.1.74.1 1996/10/02 17:02:50 damon Exp $")

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/lock.h>
#include <pthread.h>
#include <database.h>
#include <error_macros.h>
#include <globals.h>
#include <budb_errs.h>
#include <ol_verify.h>

/* notes
 * 1) volInfo structures refering to a volume of the same name are
 *	chained together, i.e. the volumes described differ in volid, partition
 *	etc. The structure at the head of this list (the sameNameChain) is
 *	treated specially. When a delete volInfo request is processed, heads
 *	are not deleted unless all other items on the sameNameChain are gone.
 *
 *	The result is that volInfo (head) structures may be present
 *	even if no tape structures refer to them. These structures are
 *	unreachable in a top-down tree walk.
 * TO DO
 * 1)	make the verify tolerant of errors. Want to get a summary statistic
 *	indicating how may dumps are lost and how many text blocks lost
 * 2)	Make the recreation instructions write out whatever is good. This
 *	is only for the off-line case.
 */

extern int sizeFunctions[];
extern int nHTBuckets;
/* The bakserver database defined in database.c */
extern struct memoryDB db;

int nBlocks;			/* total number of blocks in db */
struct misc_data *misc;
struct blockMap **blockMap = 0; /* initial block map */
struct misc_data miscData;
long orphanCount = 0; 		/* count of orphan blocks detected */

/* describes number of entries for each type of block */
int blockEntries[NBLOCKTYPES] =
{
    0			 /* free_BLOCK */,
    NvolFragmentS,
    NvolInfoS,
    NtapeS,
    NdumpS,
    1, 			/* hashTable_BLOCK */
    1,			/* text block */
    0,			/* spares */
    0,
    0,
    0,
    0
};

int blockEntrySize[NBLOCKTYPES] =
{
    0			 /* free */,
    sizeof(((struct vfBlock *)0)->a[0]),
    sizeof(((struct viBlock *)0)->a[0]),
    sizeof(((struct tBlock *)0)->a[0]),
    sizeof(((struct dBlock *)0)->a[0]),
    0,
    0,
    0,			/* spares */
    0,
    0,
    0,
    0
};

char *typeName[NBLOCKTYPES] =
{
    "free",
    "volFragment",
    "volInfo",
    "tape",
    "dump",
    "hashTable",
    "text",
    "spare1",
    "spare2",
    "spare3",
    "spare4",
    "spare5"
};

int hashBlockType[HT_MAX_FUNCTION+1] =
 {
    0,
    dump_BLOCK,
    dump_BLOCK,
    tape_BLOCK,
    volInfo_BLOCK
};

/* Compatibility table for the bits in the blockMap. */

struct mapCompatability
{
    short trigger;                      /* these bits trigger this element */
    short Set; /* unused */             /* these additional bits must be 1 */
    short Reset;                        /* excluding above, these must be 0 */
} mapC[] =
#if 0
{	/* XXX this code needs to be revisited since
	   MAP_ENTRY_ERROR is not making it into Reset
	   since Reset is a short
	   */
    MAP_FREE, 				0, MAP_ENTRY_ERROR,
    MAP_HTBLOCK, 			0, MAP_ENTRY_ERROR,
    MAP_DUMPHASH | MAP_IDHASH, 		0, MAP_ENTRY_ERROR,
    MAP_TAPEHASH | MAP_TAPEONDUMP, 	0, MAP_ENTRY_ERROR,
    MAP_VOLINFOONNAME, 			0, MAP_ENTRY_ERROR,
    MAP_VOLINFONAMEHEAD | MAP_VOLHASH, 	0, MAP_ENTRY_ERROR,
    MAP_VOLFRAGONTAPE | MAP_VOLFRAGONVOL, 0, MAP_ENTRY_ERROR,
    MAP_TEXTBLOCK,			0, MAP_ENTRY_ERROR
};
#else
{
    MAP_FREE, 				0, 0,
    MAP_HTBLOCK, 			0, 0,
    MAP_DUMPHASH | MAP_IDHASH, 		0, 0,
    MAP_TAPEHASH | MAP_TAPEONDUMP, 	0, 0,
    MAP_VOLINFOONNAME, 			0, 0,
    MAP_VOLINFONAMEHEAD | MAP_VOLHASH, 	0, 0,
    MAP_VOLFRAGONTAPE | MAP_VOLFRAGONVOL, 0, 0,
    MAP_TEXTBLOCK,			0, 0,
};
#endif /* 0 */
/* no. of entries in the mapC array */
int NMAPCs = (sizeof(mapC)/sizeof(mapC[0]));

/* for identifying stored textual information */

char *textName[TB_NUM] =
{
    "Dump Schedule\n",
    "Volume Sets\n",
    "Tape Hosts\n"
};

/* ------------------------------------
 * supporting routines
 * ------------------------------------
 */

/* BumpErrors
 *	increment the error count
 * exit:
 *	0 - continue
 *	1 - maximum errors exceeded
 */

long
BumpErrors()
{
    if ( ++miscData.errors > miscData.maxErrors )
    /* exceeded max errors; quit verification */
	return(1);
    else
	return(0);
}

/* ConvertDiskAddress
 *	given a disk address, break it down into a block and entry index. These
 *	permit access to the block map information. The conversion process
 *	compares the supplied address with the alignment/type information
 *	stored in the block map.
 * exit:
 *	0 - ok
 *	BUDB_ADDR - address alignment checks failed
 */

long
ConvertDiskAddress(address, blockIndexPtr, entryIndexPtr)
     unsigned long address;
     int *blockIndexPtr;
     int *entryIndexPtr;
{
    int index, offset;
    int type;

    *blockIndexPtr = -1;
    *entryIndexPtr = -1;

    /* This is safest way to handle totally bogus addresses (eg 0x80001234). */

    if ( (address < sizeof(db.h))
    ||   (address >= ntohl(db.h.eofPtr))
       )
    {
	return BUDB_ADDR;
    }

    address -= sizeof(db.h);
    index = address / BLOCKSIZE;
    type = blockMap[index]->header.type;
    offset = address - index*BLOCKSIZE;
    if (offset % sizeof(long))			/* alignment check */
	return BUDB_ADDR;
    if (offset && (type > 0) && (type <= MAX_STRUCTURE_BLOCK_TYPE))
    {
        offset -= sizeof(struct blockHeader);
        if ((offset < 0) || (offset % blockEntrySize[type]))
	    return BUDB_ADDR;
        offset /= blockEntrySize[type];
        if (offset >= blockEntries[type])
	    return BUDB_ADDR;
    }
    *blockIndexPtr = index;
    *entryIndexPtr = offset;
    return 0;
}

/* LogHeader
 * 	In order to cut down on verbose traces as the database verification
 *	proceeds, one needs to record some state information. This state
 *	information is printed only if an error occurs, providing additional
 *	context to the error message(s)
 */

LogHeader(st)
     char *st;
{
    static char *save = 0;
    char *ptr;

    if ( st )
    {
	/* caller provided a string to save */
	save = st;
	return;
    }

    /* caller asking for previously saved string to be printed */
    if ( save != 0 )
    {
	ptr = save;
	save = 0;			/* prevent infinite recursion */
	Log(ptr);
    }
}

char *
TypeName(index)
     int index;
{
    static char error[12];

    if ( (index < 0) || (index >= NBLOCKTYPES))
    {
        sprintf(error, "block_type=%d", index);
        return(error);
    }
    return(typeName[index]);
}

/* ------------------------------------
 * verification routines - structure specific
 * ------------------------------------
 */

/* assign max of v,i to v */

#define MAXINTO(v,i) v = ((i)>(v) ? (i) : (v))

/* verifyDumpEntry
 * entry
 *	dumpPtr - dump structure to be checked
 * exit:
 *      0 - ok
 *      n - failed
 */

long
verifyDumpEntry( ut, dumpAddr, ai, ao, dumpPtr)
     long dumpAddr;
     int ai, ao;
     struct dump *dumpPtr;
{
    struct tape tape;
    long tapeAddr, tapeCount, volCount;
    int tapeIndex, tapeOffset;

    long code = 0;

    tapeCount = 0;
    volCount = 0;

    /* check the list of tapes */
    tapeAddr = ntohl(dumpPtr->firstTape);
    while ( tapeAddr != 0 )
    {
	code = ConvertDiskAddress(tapeAddr, &tapeIndex, &tapeOffset);
	if ( code )
	{
	    LogDebug(1, "verifyDumpEntry: bad disk address %ld\n", tapeAddr);
	    goto dump_error;
	}

	code = dbread(ut, tapeAddr, &tape, sizeof(tape));
	if ( code )
	    ERROR(BUDB_IO);

	if ( ntohl(tape.dump) != dumpAddr )
	{
	  LogDebug(1,"verifyDumpEntry: tape (%ld) doesn't refer to dump %ld\n",
		   tape.dump, dumpAddr);
	  Log( "tape doesn't point to dump\n");
	  if (BumpErrors()) {
	    code = BUDB_DATABASEINCONSISTENT;
	    goto tape_error;
	  }
	}

	if ( blockMap[tapeIndex]->entries[tapeOffset] & MAP_TAPEONDUMP )
	{
	    LogDebug(1, "verifyDumpEntry: tape %ld on multiple dumps\n",
		     tapeAddr);
            Log( "tape on multiple dumps\n");
	    if (BumpErrors()) {
	      code = BUDB_DATABASEINCONSISTENT;
	      goto tape_error;
	    }
	}

	blockMap[tapeIndex]->entries[tapeOffset] |= MAP_TAPEONDUMP;
	tapeCount++;
	volCount += ntohl(tape.nVolumes);
	tapeAddr = ntohl(tape.nextTape);
    }

    if ( ntohl(dumpPtr->nVolumes) != volCount )
    {
        LogDebug(1, "verifyDumpEntry: dump's fileset count wrong: was %ld, should be %d\n", ntohl(dumpPtr->nVolumes), volCount);
	Log( "Dump's fileset count wrong: was %d, should be %d\n",
                 ntohl(dumpPtr->nVolumes), volCount);
	if (BumpErrors()) {
	  code = BUDB_DATABASEINCONSISTENT;
	  goto dump_error;
	}
    }

    MAXINTO (misc->maxVolsPerDump, volCount);
    MAXINTO (misc->maxTapesPerDump, tapeCount);
    misc->nDump++;

    /* normal and error exit */
error_exit:
    /* if we found errors, try to mark this dump as bad */
    if ( code )
    {
	int dumpIndex, dumpOffset;

	Log("verifyDumpEntry: returning with %d\n", code);

	/* mark this dump as bad */
	(void)ConvertDiskAddress(dumpAddr, &dumpIndex, &dumpOffset);
	blockMap[dumpIndex]->entries[dumpOffset] |= MAP_ENTRYERROR;
    }
    LogDebug(3, "verifyDumpEntry returning code %ld\n", code);
    return(code);

tape_error:
    /* print both dump and tape */
    Log("Inconsistent dump (addr %d) & tape %d\n", dumpAddr, tapeAddr);
    goto error_exit;

dump_error:
    /* print just the dump */
    Log("Inconsistent dump (addr %d)\n", dumpAddr);
    goto error_exit;
}

long
verifyTapeEntry (ut, tapeAddr, ai, ao, tapePtr)
  struct ubik_trans *ut;
  long tapeAddr;
  int ai, ao;
  struct tape *tapePtr;
{

    int volCount;
    long volFragAddr;
    int blockIndex, entryIndex;
    u_long *flagsPtr;
    struct volFragment volFragment;
    long code = 0;

    volCount = 0;
    volFragAddr = ntohl(tapePtr->firstVol);
    while (volFragAddr)
    {
	code = ConvertDiskAddress(volFragAddr, &blockIndex, &entryIndex);
	if (code)
	{
	    LogDebug(1, "verifyTapeEntry: invalid fileset fragment address %ld in tape at address %ld\n",
		   volFragAddr, tapeAddr);
	    Log("Invalid volFragAddr %d in tape at addr %d\n",
		     volFragAddr, tapeAddr);
	    ERROR(code);
	}
	flagsPtr = &blockMap[blockIndex]->entries[entryIndex];

	code = dbread (ut, volFragAddr, &volFragment, sizeof(volFragment));
	if ( code )
	    ERROR(code);

	if ( ntohl(volFragment.tape) != tapeAddr )
	{
	    LogDebug(1, "verifyTapeEntry: fileset fragment(addr %d) doesn't point back to tape (addr %d)\n",
		     volFragAddr, tapeAddr);
	    Log("Volfrag (addr %d) doesn't point back to tape (addr %d)\n",
		     volFragAddr, tapeAddr);
	    if (BumpErrors())
	      ERROR(BUDB_DATABASEINCONSISTENT);
	}

	if ( *flagsPtr & MAP_VOLFRAGONTAPE)
	{
	    /* error - this fileset fragment has already been examined */
	    LogDebug(1, "verifyTapeEntry: fileset fragment(addr %d) on multiple tapes\n",
		     volFragAddr);
	    Log("Volfrag (addr %d) on multiple tapes\n",
		     volFragAddr);
	    if (BumpErrors())
	      ERROR(BUDB_DATABASEINCONSISTENT);
	}

	*flagsPtr |= MAP_VOLFRAGONTAPE;
	volCount++;
	volFragAddr = ntohl(volFragment.sameTapeChain);
    }

    /* now check computed vs. recorded volume counts */
    if ( ntohl(tapePtr->nVolumes) != volCount )
    {
	LogDebug(1, "verifyTapeEntry: Tape(addr %d) fileset count recorded %d computed %d\n",
		 tapeAddr, ntohl(tapePtr->nVolumes), volCount);
	Log("Tape (addr %d) fileset count recorded %d computed %d\n",
		 tapeAddr, ntohl(tapePtr->nVolumes), volCount);
	if (BumpErrors())
	  ERROR(BUDB_DATABASEINCONSISTENT);
    }

    MAXINTO (misc->maxVolsPerTape, volCount);
    misc->nTape++;

error_exit:
    LogDebug(3, "verifyTapeEntry returning code %ld\n", code);
    return(code);
}

long
verifyVolFragEntry (ut, va, ai, ao, v)
     struct ubik_trans *ut;
     long va;
     int ai, ao;
     struct volFragment *v;
{
    misc->nVolFrag++;
    return 0;
}

long
verifyVolInfoEntry (ut, volInfoAddr, ai, ao, volInfo)
     struct ubik_trans *ut;
     long volInfoAddr;
     int ai, ao;
     struct volInfo *volInfo;
{
    int volCount;
    int nFrags;
    long volFragAddr;
    int blockIndex, entryIndex;
    u_long *flagsPtr;
    struct volFragment volFragment;
    long code = 0;

    volCount = 0;
    volFragAddr = ntohl(volInfo->firstFragment);

    /* check each fragment attached to this volinfo structure */
    while (volFragAddr)
    { /*w*/
	code = ConvertDiskAddress (volFragAddr, &blockIndex, &entryIndex);
	if (code)
	{
	    LogDebug(1, "verifyVolInfoEntry: Invalid fileset fragment(addr %d) on fileset Info(addr %d)\n",
		     volFragAddr, volInfoAddr);
	    Log("Invalid volFragAddr %d on volInfo (addr %d)\n",
		     volFragAddr, volInfoAddr);
	    ERROR(code);
	}
	flagsPtr = &blockMap[blockIndex]->entries[entryIndex];

	code = dbread(ut, volFragAddr, &volFragment, sizeof(volFragment));
	if ( code )
	    ERROR(code);

	if ( ntohl(volFragment.vol) != volInfoAddr)
	{
	    LogDebug(1, "verifyVolInfoEntry: fileset fragment(addr %d) doesn't point to fileset Info(addr %d)\n",
		     volFragAddr, volInfoAddr);
	    Log("volFragment (addr %d) doesn't point to volInfo (addr %d)\n",
		     volFragAddr, volInfoAddr);
	    if (BumpErrors())
	      ERROR(BUDB_DATABASEINCONSISTENT);
	}

	if ( *flagsPtr & MAP_VOLFRAGONVOL)
	{
	    /* volume fragment already on a volinfo chain */
	    LogDebug(1, "verifyVolInfoEntry: fileset fragment(addr %d) on multiple fileset info chains\n",
		     volFragAddr);
	    Log("Volfragment (addr %d) on multiple volInfo chains\n",
		     volFragAddr);
	    if (BumpErrors())
	      ERROR(BUDB_DATABASEINCONSISTENT);
	}

	*flagsPtr |= MAP_VOLFRAGONVOL;
	volCount++;
	volFragAddr = ntohl(volFragment.sameNameChain);
    } /*w*/

    /* check computed vs. recorded number of fragments */
    nFrags = ntohl(volInfo->nFrags);
    if ( misc->checkFragCount && (nFrags != volCount) )
    {
	LogDebug(1, "verifyVolInfoEntry: fileset Info(addr %d) fragment count actual %d recorded %d\n",
		volInfoAddr, volCount, nFrags);
	Log("VolInfo (addr %d) frag count actual %d recorded %d\n",
		volInfoAddr, volCount, nFrags);
	if (BumpErrors())
	  ERROR(BUDB_DATABASEINCONSISTENT);
    }

    /* Check that all volInfo structures with same name point to the same
     * head. If sameNameHead == 0, this is the head structure so we check,
     * otherwise ignore
     */
    if (volInfo->sameNameHead == 0)
    { /*i*/
	int viCount = 1;		/* count this one */
	struct volInfo tvi;
	long tviAddr;

	tviAddr = ntohl(volInfo->sameNameChain);
	while ( tviAddr != 0 )
	{ /*w*/
	    viCount++;
	    code = ConvertDiskAddress (tviAddr, &blockIndex, &entryIndex);
	    if (code)
	    {
		LogDebug(1, "verifyVolInfoEntry: Bad fileset Info(addr %d) on same name chain\n",
		    tviAddr);
		Log("Bad volInfo (addr %d) on same name chain\n",
		    tviAddr);
		ERROR(code);
	    }
	    flagsPtr = &blockMap[blockIndex]->entries[entryIndex];

	    code = dbread(ut, tviAddr, &tvi, sizeof(tvi));
	    if ( code )
	        ERROR(code);

	    if ( ntohl(tvi.sameNameHead) != volInfoAddr)
	    {
		LogDebug(1, "verifyVolInfoEntry: fileset Info(addr %d) sameNameHead is %d should be %d\n",
		    tviAddr, ntohl(tvi.sameNameHead), volInfoAddr);
		Log("VolInfo (addr %d) sameNameHead is %d should be %d\n",
		    tviAddr, ntohl(tvi.sameNameHead), volInfoAddr);
		if (BumpErrors())
		  ERROR(BUDB_DATABASEINCONSISTENT);
	    }

	    if (*flagsPtr & MAP_VOLINFOONNAME)
	    {
		LogDebug(1, "verifyVolInfoEntry: fileset Info(addr %d) on multiple same name chains\n",
		    tviAddr);
		Log("volInfo (addr %d) on multiple same name chains\n",
		    tviAddr);
		if (BumpErrors())
		  ERROR(BUDB_DATABASEINCONSISTENT);
	    }
	    *flagsPtr |= MAP_VOLINFOONNAME;

	    tviAddr = ntohl(tvi.sameNameChain);
	} /*w*/

	/* select the passed in structure flags */
	flagsPtr = &blockMap[ai]->entries[ao];
	if ( *flagsPtr & MAP_VOLINFONAMEHEAD)
	{
	    LogDebug(1, "verifyVolInfoEntry: fileset Info(addr %d) is name head multiple times\n",
		volInfoAddr);
	    Log("VolInfo (addr %d) is name head multiple times\n",
		volInfoAddr);
	    if (BumpErrors())
	      ERROR(BUDB_DATABASEINCONSISTENT);
	}

	*flagsPtr |= MAP_VOLINFONAMEHEAD;
	MAXINTO (misc->maxVolInfosPerName, viCount);
	misc->nVolName++;
    } /*i*/

    MAXINTO (misc->maxVolsPerVolInfo, volCount);
    misc->nVolInfo++;

error_exit:
    LogDebug(3, "verifyVolInfoEntry returning code %ld\n", code);
    return(code);
}


/* ------------------------------------
 * verification routines - general
 * ------------------------------------
 */

/* verifyBlocks
 *
 * globals
 *	blockMap
 */

long
verifyBlocks(ut)
     struct ubik_trans *ut;
{
    struct block block;
    int blocktype;
    long blockAddr;
    struct blockMap *ablockMap = 0;
    int bmsize;
    int i;
    long code = 0;

    /* globals
     * int nBlocks;
     */

    /* look at every block in the database */
    for ( i = 0; i < nBlocks; i++ )
    {
	/* read the block header */
	blockAddr = sizeof(db.h) + i*BLOCKSIZE;
	code = dbread(ut, blockAddr, (char *) &block.h, sizeof(block.h));
	if ( code )
	    ERROR(code);

	/* check the block type */
	blocktype = block.h.type;			/* char */
	if ( (blocktype < 0) || ( blocktype >= NBLOCKTYPES) )
	{
	    LogDebug(1, "verifyBlocks: block (index %d addr %d) has invalid type (%d)\n",
		     i, blockAddr, blocktype);
	    Log("block (index %d addr %d) has invalid type (%d)\n",
		     i, blockAddr, blocktype);
	    ERROR(BUDB_BLOCKTYPE);
	}

	/* allocate the block map memory */
	/*
	 * Free blocks have 0 block entries associated with them. We will have
	 * to be careful not to allocate less than the size of blockMap for
	 * such cases.
	 */
	if (blockEntries[blocktype])
	  bmsize=sizeof(*ablockMap)+(blockEntries[blocktype]-1)*sizeof(ablockMap->entries[0]);
	else
	  bmsize=sizeof(*ablockMap);

	LogDebug(1, "verifyBlocks: blockMap struct %d, blockType %d nentries %d each %d\n",
	       sizeof(*ablockMap),
	       blocktype,
	       blockEntries[blocktype],
	       sizeof(ablockMap->entries[0]));

	ablockMap = (struct blockMap *) osi_Alloc(bmsize);
	if ( ablockMap == 0 )
	    ERROR(BUDB_NOMEM);
	bzero((char *)ablockMap, bmsize);

	ablockMap->nEntries = blockEntries[blocktype];

	/* save the block header in the block map */
	ablockMap->header = block.h;
	blockMap[i] = ablockMap;
	LogDebug(3, "verifyBlocks: block %d, nEntries %d bmsize %d addr %d\n",
	    i, ablockMap->nEntries, bmsize, ablockMap);
	/* Log("block %d, nEntries %d bmsize %d addr %d\n",
	    i, ablockMap->nEntries, bmsize, ablockMap); */
    }

error_exit:
    pthread_yield();
    LogDebug(3, "verifyBlocks returning code %ld\n", code);
    return(code);
}

long
verifyHashTableBlock(ut, mhtPtr, htBlockPtr, old, length, index, mapBit)
     struct ubik_trans *ut;
     struct memoryHashTable *mhtPtr;
     struct htBlock *htBlockPtr;
     int old;
     long length;                       /* size of whole hash table */
     int index;                         /* base index of this block */
     int mapBit;
{
    int type;                           /* hash table type */
    int entrySize;			/* hashed entry size */
    int blockType;			/* block type for this hash table */

    u_long *mapEntryPtr = 0;            /* for status checks */
    int blockIndex, entryIndex;

    char entry[sizeof(struct block)];
    dbadr entryAddr, nextEntryAddr;

    int hash;				/* calculated hash value for entry */

    int i;
    long code = 0;

    type = ntohl(mhtPtr->ht->functionType);
    entrySize = sizeFunctions[type];
    blockType = hashBlockType[type];

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
	{
	    LogDebug(2, "verifyHashTableBlock: Old hash table not empty (entry %d) below progress (%d)\n",
		     i,  mhtPtr->progress);
	    Log("Old hash table not empty (entry %d) below progress (%d)\n",
		     i,  mhtPtr->progress);
            if (BumpErrors())
                return BUDB_DATABASEINCONSISTENT;
        }

	/* now walk down the chain of each bucket */
	while ( nextEntryAddr )
	{ /*w*/
	    code = ConvertDiskAddress(nextEntryAddr, &blockIndex, &entryIndex);
	    if ( code )
	    {
		LogDebug(1, "verifyHashTableBlock: Invalid chain addr %d\n",
		    nextEntryAddr);
		Log("verifyHashTableBlock: Invalid chain addr %d\n",
		    nextEntryAddr);
		goto skip_bucket;
	    }

	    mapEntryPtr = &blockMap[blockIndex]->entries[entryIndex];
	    if ( blockMap[blockIndex]->header.type != blockType )
	    {
		LogDebug(1, "verifyHashTableBlock: Hash table chain (block index %d) incorrect\n",
			 blockIndex);
		LogDebug(1, "verifyHashTableBlock: Table type %d traverses block type %d\n",
			 blockType, blockMap[blockIndex]->header.type);
		Log("Hash table chain (block index %d) incorrect\n",
			 blockIndex);
		Log("Table type %d traverses block type %d\n",
			 blockType, blockMap[blockIndex]->header.type);
		goto skip_bucket;
	    }

	    entryAddr = nextEntryAddr;
	    if ( code = dbread(ut, entryAddr, &entry[0], entrySize) )
		return(code);

	    lock_UpgradeSToW(&(db.lock));
	    hash = ht_HashEntry(mhtPtr, &entry[0]) % length;
	    lock_ConvertWToS(&(db.lock));
	    if ( hash != index )
	    {
		/* if it hashed to some other place */
		*mapEntryPtr |= MAP_HASHERROR;

		LogDebug(1, "verifyHashTableBlock: Entry (addr %d) bucket %d, should be in %d\n",
			 entryAddr, index, hash);
		Log("Entry (addr %d) bucket %d, should be in %d\n",
			 entryAddr, index, hash);

		if ( BumpErrors() )
		    return(BUDB_DATABASEINCONSISTENT);
	    }

	    if ( blockMap[blockIndex]->entries[entryIndex] & mapBit )
	    {
		/* doubly reference entry */
		*mapEntryPtr |= MAP_DOUBLEREF;
		LogDebug(1, "verifyHashTableBlock: Entry (addr %d) multiply referenced\n",
			 entryAddr);
		Log("Entry (addr %d) multiply referenced\n",
			 entryAddr);

		if ( BumpErrors() )
		    return(BUDB_DATABASEINCONSISTENT);
	    }

	    /* set bit appropriate to the entry type */
	    *mapEntryPtr |= mapBit;

	    nextEntryAddr = ntohl( *((dbadr *)(entry + mhtPtr->threadOffset)));
	    /* proceed with next item in chain */
	    continue;

skip_bucket:
	    /* error case */
	    /* should print out information about last entry traversed */
	    if ( BumpErrors() )
		return(BUDB_DATABASEINCONSISTENT);

	    LogDebug(3, "verifyHashTableBlock: Skipping remainder of bucket %d\n", index);
	    Log("Skipping remainder of bucket %d\n", index);

	    /* proceed to next bucket */
	    break;

	} /*w*/

	/* finished following the entries in this bucket */
    } /*f*/
    LogDebug(3, "verifyHashTableBlock returning code 0\n");
    return(0);
}

long
verifyHashTable(ut, mhtPtr, mapBit)
     struct ubik_trans *ut;
     struct memoryHashTable *mhtPtr;
     int mapBit;
{
    struct hashTable *htPtr = mhtPtr->ht;

    struct htBlock hashTableBlock;
    int tableLength;				/* # entries */
    int blockLength;				/* # blocks */
    dbadr tableAddr;				/* disk addr of hash block */
    int blockIndex, entryIndex;
    int hashIndex;
    int old;
    int i;
    long code = 0;

    extern int nHTBuckets;			/* # buckets in a hash table */

    LogDebug(4, "Htable: length %d oldlength %d progress %d\n",
	mhtPtr->length, mhtPtr->oldLength, mhtPtr->progress);

    /* check both old and current tables */
    for ( old = 0; old <= 1; old++ )
    { /*fo*/
	if (old)
	{
            /* check the old hash table */
            tableLength = mhtPtr->oldLength;
	    if ( tableLength == 0 )
		continue;			/* nothing to do */

            tableAddr = ntohl(htPtr->oldTable);
        }
        else
        {
            /* check current hash table */
            tableLength = mhtPtr->length;
	    if ( tableLength == 0 )
		continue;			/* nothing to do */

            tableAddr = ntohl(htPtr->table);
        }

	blockLength = (tableLength-1)/nHTBuckets;
	hashIndex = 0;

	/* follow the hash chain */
	for ( i = 0; i <= blockLength; i++ )
	{ /*fi*/
	    /* chain too short */
	    if ( tableAddr == 0 )
	    {
		Log("Short hash chain ( %d < expected %d), hashtype %d\n",
			 i+1, blockLength, ntohl(htPtr->functionType));
		if (BumpErrors())
		  ERROR(BUDB_DATABASEINCONSISTENT);
	    }

	    code = ConvertDiskAddress(tableAddr, &blockIndex, &entryIndex);
	    if ( code )
	        ERROR(code);

            if ( blockMap[blockIndex]->header.type != hashTable_BLOCK )
	    {
		LogDebug(1, "verifyHashTable: Hashtable block (index %d addr %d) hashtype %d\n",
			 i+1, tableAddr, ntohl(htPtr->functionType));
		LogDebug(1, "verifyHashTable: - type %d, expected %d\n",
			 blockMap[blockIndex]->header.type, hashTable_BLOCK);
		Log("Hashtable block (index %d addr %d) hashtype %d\n",
			 i+1, tableAddr, ntohl(htPtr->functionType));
		Log("- type %d, expected %d\n",
			 blockMap[blockIndex]->header.type, hashTable_BLOCK);
		if (BumpErrors())
		  return BUDB_DATABASEINCONSISTENT;
	    }

	    /* check if we've traversed this block before */
            if (blockMap[blockIndex]->entries[entryIndex] & MAP_HTBLOCK)
            {
		LogDebug(1, "verifyHashTable: Hash table block (index %d addr %d) multiple ref\n",
			 i+1, tableAddr);
		Log("Hash table block (index %d addr %d) multiple ref\n",
			 i+1, tableAddr);
		if (BumpErrors())
		  ERROR(BUDB_DATABASEINCONSISTENT);
            }

	    /* mark this (hash table) block as examined  */
            blockMap[blockIndex]->entries[entryIndex] |= MAP_HTBLOCK;

	    code = dbread(ut, tableAddr, &hashTableBlock,
			  sizeof(hashTableBlock));
	    if ( code )
	        ERROR(code);

	    code = verifyHashTableBlock(ut, mhtPtr, &hashTableBlock, old,
					tableLength, hashIndex, mapBit);
	    if ( code )
	        ERROR(code);

	    hashIndex += nHTBuckets;
	    tableAddr = ntohl(hashTableBlock.h.next);
	} /*fi*/
    } /*fo*/

error_exit:
    pthread_yield();
    LogDebug(3, "verifyHashTable returning code %ld\n", code);
    return(code);
}

/* verifyEntryChains
 *	do a linear walk of all the blocks. Check each block of structures
 *	to see if the actual free matches the recorded free. Also check
 *	the integrity of each allocated structure.
 */

long
verifyEntryChains(ut)
     struct ubik_trans *ut;
{
    long code;
    long offset;
    int blockIndex, entryIndex;
    char entry[sizeof(struct block)];
    u_long *flagsPtr;
    int entrySize;
    int type;

    int nFree;

    /* globals
     * int nBlocks;
     */

    static long (*checkEntry[NBLOCKTYPES])()
        = { 	0, 				/* free block */
		verifyVolFragEntry,
		verifyVolInfoEntry,
                verifyTapeEntry,
		verifyDumpEntry,
		0,				/* text block */
		0,				/* spares     */
		0,
		0,
		0,
		0
	  };

    pthread_yield();
    for ( blockIndex = 0; blockIndex < nBlocks; blockIndex++)
    { /*f*/
        type = blockMap[blockIndex]->header.type;

	/* ignore non-structure or blocks with invalid type */
	/* should we flag this as an error ?? */
        if ((type <= 0) || (type > MAX_STRUCTURE_BLOCK_TYPE))
	    continue;

        entrySize = blockEntrySize[type];

        offset = sizeof(db.h) + blockIndex*BLOCKSIZE +
	    		sizeof(struct blockHeader);
        nFree = 0;
        for ( entryIndex=0; entryIndex<blockMap[blockIndex]->nEntries;
	     			entryIndex++, offset += entrySize )
	{ /*f*/
            if ( dbread (ut, offset, &entry[0], entrySize))
		return BUDB_IO;

	    flagsPtr = &blockMap[blockIndex]->entries[entryIndex];

	    /* check the allocator's brain-damaged notion of allocated/freed
	     * by looking at the first "long" of the structure
	     */

            if ( *((long *) &entry[0]) == 0 )
	    {
		/* supposedly free structure. is it marked as non-free ? */
		if ( *flagsPtr & MAP_HASHES )
		{
		    LogDebug(1, "verifyEntryChains: blockindex %d, entryindex %d, hashtype %d\n",
			     blockIndex, entryIndex, (*flagsPtr & MAP_HASHES));
		    LogDebug(1, "       free by allocater convention\n");
		    Log("Entry: blockindex %d, entryindex %d, hashtype %d\n",
			     blockIndex, entryIndex, (*flagsPtr & MAP_HASHES));
		    Log("       free by allocater convention\n");
		    if (BumpErrors())
		      return BUDB_DATABASEINCONSISTENT;
		}

                if ( *flagsPtr & MAP_FREE )
		{
		    LogDebug(1, "verifyEntryChains: blockindex %d, entryindex %d, hashtype %d\n",
			     blockIndex, entryIndex, (*flagsPtr & MAP_HASHES));
		    LogDebug(1, "       entry already marked free\n");
		    Log("Entry: blockindex %d, entryindex %d, hashtype %d\n",
			     blockIndex, entryIndex, (*flagsPtr & MAP_HASHES));
		    Log("       entry already marked free\n");
		    if (BumpErrors())
		      return BUDB_DATABASEINCONSISTENT;
                }
		else
		{
		    nFree++;
		    *flagsPtr |= MAP_FREE;
		}
                continue;
            }

	    /* check the entry itself for consistency */
            code = (*(checkEntry[type]))(ut, offset, blockIndex, entryIndex,
					 &entry[0]);
            if (code)
		return code;
        } /*f*/

	/* check computed free with recorded free entries */
        if ( nFree != ntohs(blockMap[blockIndex]->header.nFree) )
	{
	    LogDebug(1, "verifyEntryChains: Block (index %d) free count %d has %d free structs\n",
		     blockIndex, ntohs(blockMap[blockIndex]->header.nFree),
		     nFree);
	    Log("Block (index %d) free count %d has %d free structs\n",
		     blockIndex, ntohs(blockMap[blockIndex]->header.nFree),
		     nFree);
            if (BumpErrors())
		return BUDB_DATABASEINCONSISTENT;
            continue;
        }
    } /*f*/
    LogDebug(3, "verifyEntryChains returning 0\n");
    return 0;
}


long
verifyFreeLists()
{
    int i;
    long addr;
    int blockIndex, entryIndex;
    int nFree;
    long code;

    pthread_yield();
    /* for each free list */
    for (i=0; i<NBLOCKTYPES; i++)
    {
	misc->fullyFree[i] = misc->freeLength[i] = 0;

	for (addr=db.h.freePtrs[i];
	     addr;
	     addr=blockMap[blockIndex]->header.next
	    )
	{
	    misc->freeLength[i]++;
	    addr = ntohl(addr);
	    code = ConvertDiskAddress (addr, &blockIndex, &entryIndex);

	    if (code || (entryIndex != 0))
	    {
		LogDebug(1, "verifyFreeLists: Bad address in %s free chain %d\n",
			 TypeName(i), addr);
		Log("Bad address in %s free chain %d\n",
			 TypeName(i), addr);
		if (BumpErrors())
		    return(BUDB_DATABASEINCONSISTENT);
		break;
	    }

	    /* check block type */
	    if (blockMap[blockIndex]->header.type != i)
	    {
		/* item type does not match free chain type */
		LogDebug(1, "verifyFreeLists: Found %s type in %s free chain\n",
		    TypeName(blockMap[blockIndex]->header.type),
		    TypeName(i), addr);
		/* item type does not match free chain type */
		Log("Found %s type in %s free chain\n",
		    TypeName(blockMap[blockIndex]->header.type),
		    TypeName(i), addr);
		if (BumpErrors())
		    return(BUDB_DATABASEINCONSISTENT);
		break;
	    }

	    nFree = ntohs(blockMap[blockIndex]->header.nFree);
	    if (i != free_BLOCK)
	    {
		if ((nFree <= 0) || (nFree > blockEntries[i])) {
		    LogDebug(1, "verifyFreeLists: Illegal free count %d on %s free chain\n",
			     nFree, TypeName(i));
		    Log ("Illegal free count %d on %s free chain\n",
			     nFree, TypeName(i));
		    if (BumpErrors())
			return(BUDB_DATABASEINCONSISTENT);
		    continue;
		}
		if (nFree == blockEntries[i])
		    misc->fullyFree[i]++;
	    }
	    if (blockMap[blockIndex]->isfree)
	    {
		LogDebug(1, "verifyFreeLists: %s free chain block at %d multiply threaded\n",
		    TypeName(i), addr);
		Log("%s free chain block at %d multiply threaded\n",
		    TypeName(i), addr);
		if (BumpErrors())
		    return BUDB_DATABASEINCONSISTENT;
		break;
	    }
	    blockMap[blockIndex]->isfree++;
	}
    }
    LogDebug(3, "verifyFreeLists returning code 0\n");
    return 0;
}

/* verifyMapBits
 *	Examines each entry to make sure it was traversed appropriately by
 *	checking the bits for compatibility.
 */

long
verifyMapBits()
{
    int blockIndex, entryIndex;
    int i;
    int trigs;

    pthread_yield();
    for ( blockIndex=0; blockIndex<nBlocks; blockIndex++)
    {
	if (blockMap[blockIndex]->nEntries == 0)
	{
	    /* no entries in this block. Ensure the block is free */

	    if (blockMap[blockIndex]->isfree != 1)
	    {
	        orphanCount++;
		LogDebug(1, "verifyMapBits: Orphan free block (index %d)\n", blockIndex);
		Log("Orphan free block (index %d)\n", blockIndex);
		if (BumpErrors())
		    return BUDB_DATABASEINCONSISTENT;
	    }
	    continue;
	}

	/* check each entry */
	for ( entryIndex=0;
	      entryIndex < blockMap[blockIndex]->nEntries;
	      entryIndex++
	    )
	{ /*f*/
	    trigs = 0;
	    for (i=0; i < NMAPCs; i++)
	    {
		int bits = blockMap[blockIndex]->entries[entryIndex];
		int trigger = mapC[i].trigger;

		if ((bits & trigger) == trigger)
		{
		    int set = mapC[i].Set;
		    int reset = mapC[i].Reset & (~set) & (~trigger);
		    trigs++;
		    if ( ((bits & set) != set)
		    ||   ((~bits & reset) != reset)
		       )
		    {
			LogDebug(1, "verifyMapBits: Compatibility bits error block index %d struct index %d, bits 0x%x\n",
			    blockIndex, entryIndex, bits);
			Log("Compatibility bits error block index %d struct index %d, bits 0x%x\n",
			    blockIndex, entryIndex, bits);
			if (BumpErrors())
			    return BUDB_DATABASEINCONSISTENT;
		    }
		}
	    }
	    if (!trigs)
	    {
		LogDebug(1, "verifyMapBits: %s block %d, struct %d bits had no triggers\n",
		    TypeName(blockMap[blockIndex]->header.type),
		    blockIndex,
		    entryIndex);
		Log("%s block %d, struct %d bits had no triggers\n",
		    TypeName(blockMap[blockIndex]->header.type),
		    blockIndex,
		    entryIndex);
		if (BumpErrors())
		    return BUDB_DATABASEINCONSISTENT;
	    }
	} /*f*/
    }
    LogDebug(3, "verifyMapBits returning code 0\n");
    return 0;
}

long
verifyText(ut)
     struct ubik_trans *ut;
{
    int i;
    long code = 0;

    extern long verifyTextChain();

    /* check each of the text types in use */

    for ( i = 0; i < TB_NUM; i++ )
    {
	/* set prefix string - for identifying which text block is being
	 * checked
	 */
	LogHeader(textName[i]);
	code = verifyTextChain(ut, &db.h.textBlock[i]);
	if ( code )
	    return(code);
    }
    return(0);
}

/* verifyTextChain
 *	check the integrity of a text chain. Also checks the new chain.
 */

long
verifyTextChain(ut, tbPtr)
     struct ubik_trans *ut;
     struct textBlock *tbPtr;
{
    dbadr blockAddr;
    int blockIndex, entryIndex;
    u_long *flagsPtr;
    struct block block;
    long size;
    long code = 0;

    pthread_yield();
    size = 0;
    blockAddr = ntohl(tbPtr->textAddr);
    while ( ntohl(tbPtr->size) >= 0 && blockAddr != 0 )
    {
	code = dbread(ut, blockAddr, &block, sizeof(block));
	if ( code )
	    ERROR(code);

	code = ConvertDiskAddress(blockAddr, &blockIndex, &entryIndex);
	if ( code )
	{
	    LogDebug(1, "verifyTextChain: Invalid text block addr (%d)\n", blockAddr);
	    Log("Invalid text block addr (%d)\n", blockAddr);
	    ERROR(code);
	}

	flagsPtr = &blockMap[blockIndex]->entries[entryIndex];

	if ( *flagsPtr & MAP_TEXTBLOCK )
	{
	    LogDebug(1, "verifyTextChain: Text block (addr %d) multiply chained\n",
		blockAddr);
	    Log("Text block (addr %d) multiply chained\n",
		blockAddr);
	    if (BumpErrors())
	      ERROR(BUDB_DATABASEINCONSISTENT);
	}

	*flagsPtr |= MAP_TEXTBLOCK;
	size += BLOCK_DATA_SIZE;

	blockAddr = ntohl(block.h.next);
    }

    if ( ntohl(tbPtr->size) > size )
    {
	LogDebug(1, "verifyTextChain: Text block size %d > computed capacity %d\n",
	    ntohl(tbPtr->size), size);
	Log("Text block size %d > computed capacity %d\n",
	    ntohl(tbPtr->size), size);
	if (BumpErrors())
	  ERROR(BUDB_DATABASEINCONSISTENT);
    }

    /* repeat for new chain */
    size = 0;
    blockAddr = ntohl(tbPtr->newTextAddr);
    while ( ntohl(tbPtr->newsize)>=0 && blockAddr != 0 )
    {
	code = dbread(ut, blockAddr, &block, sizeof(block));
	if ( code )
	    ERROR(code);

	code = ConvertDiskAddress(blockAddr, &blockIndex, &entryIndex);
	if ( code )
	{
	    LogDebug(1, "verifyTextChain: Invalid new text block addr (%d)\n", blockAddr);
	    Log("Invalid new text block addr (%d)\n", blockAddr);
	    ERROR(code);
	}

	flagsPtr = &blockMap[blockIndex]->entries[entryIndex];

	if ( *flagsPtr & MAP_TEXTBLOCK )
	{
	    LogDebug(1,"verifyTextChain: New text block (addr %d) multiply chained\n",
		blockAddr);
	    Log("New text block (addr %d) multiply chained\n",
		blockAddr);
	    if (BumpErrors())
	      ERROR(BUDB_DATABASEINCONSISTENT);
	}

	*flagsPtr |= MAP_TEXTBLOCK;
	size += BLOCK_DATA_SIZE;

	blockAddr = ntohl(block.h.next);
    }

    if ( ntohl(tbPtr->newsize) > size )
    {
	LogDebug(1, "verifyTextChain: Text block new size %d > computed capacity %d\n",
	    ntohl(tbPtr->newsize), size);
	Log("Text block new size %d > computed capacity %d\n",
	    ntohl(tbPtr->newsize), size);
	if (BumpErrors())
	  ERROR(BUDB_DATABASEINCONSISTENT);
    }

error_exit:
    LogDebug(3, "verifyTextChain returning code %ld\n", code);
    return(code);
}

/* -----------------------------------------
 * verification driver routines
 * -----------------------------------------
 */

/* verifyDatabase
 *	Check the integrity of the database
 */

long
verifyDatabase(ut, recreateFile)
     struct ubik_trans *ut;
     FILE *recreateFile;
{
    long eof;
    int bmsize;
    long code = 0;

    extern int nBlocks;			/* no. blocks in database */

    /* clear verification statistics */
    misc = &miscData;
    bzero ((char *)&miscData, sizeof(miscData));

    miscData.maxErrors = BUDB_VERIFY_MAXERRORS;
    miscData.veryLongChain = 0;
    miscData.checkFragCount = 1;		/* check frags */

    /* check eofPtr */
    eof = ntohl(db.h.eofPtr);
    eof -= sizeof(db.h);			/* subtract header */
    nBlocks = eof / BLOCKSIZE;

    if ((eof < 0) || (nBlocks*BLOCKSIZE != eof))
    {
        LogDebug(1,"Database eof ptr %ld bad, block count %ld blocksize %ld\n",
		 eof, nBlocks, BLOCKSIZE);
	Log("Database eofPtr (%d) bad, blocksize %d\n", eof, BLOCKSIZE);
	/* too serious to ignore, bail out */
	ERROR(BUDB_DATABASEINCONSISTENT);
    }

    /* set size of database */
    miscData.nBlocks = nBlocks;

    /* construct block map - first level is the array of pointers */
    if (nBlocks) {
      bmsize = nBlocks*sizeof(struct blockMap *);
      blockMap = (struct blockMap **) osi_Alloc(bmsize);
      if ( blockMap == 0 )
	ERROR(BUDB_NOMEM);
      bzero((char *)blockMap, bmsize);
    }
    else {
      bmsize = 0;
      blockMap = (struct blockMap **)NULL;
    }

     /* check blocks and construct the block map */
    LogHeader("Block map construction:\n");
    LogDebug(1, "verify each block in database; ");
    code = verifyBlocks(ut);
    if ( code )
        ERROR(code);

    /* check the various hash tables */

    LogHeader("Hash table check: fileset name hash\n");
    LogDebug(1, "verify each fileset entry in hash table; ");
    code = verifyHashTable(ut, &db.volName, MAP_VOLHASH);
    if ( code )
	ERROR(code);

    LogHeader("Hash table check: tape name hash\n");
    LogDebug(1, "verify each tape entry in hash table; ");
    verifyHashTable(ut, &db.tapeName, MAP_TAPEHASH);
    if ( code )
	ERROR(code);

    LogHeader("Hash table check: dump name hash\n");
    LogDebug(1, "verify each dump entry in hash table; ");
    verifyHashTable(ut, &db.dumpName, MAP_DUMPHASH);
    if ( code )
	ERROR(code);

    LogHeader("Hash table check: dump id hash\n");
    LogDebug(1, "verify each dump id in hash table; ");
    verifyHashTable(ut, &db.dumpIden, MAP_IDHASH);
    if ( code )
	ERROR(code);

    /* check the entry chains */
    LogHeader("Structure chains check:\n");
    LogDebug(1, "verify each entry chain in database; ");
    code = verifyEntryChains(ut);
    if ( code )
	ERROR(code);

    /* check text blocks */
    LogDebug(1, "verify each text block in database; ");
    code = verifyText(ut);
    if ( code )
	ERROR(code);

    /* check free list */
    LogHeader("Structure Free lists check:\n");
    LogDebug(1, "verify free list integrity; ");
    code = verifyFreeLists();
    if ( code )
	ERROR(code);

    /* check entry map bit compatibility */
    LogDebug(1, "verify entry map bit compatibility; ");
    code = verifyMapBits();
    if ( code )
	ERROR(code);

    /* check for unreferenced entries */

    /* write recreation instructions */

    /* print summary - if any */

error_exit:
    /* clean up all allocated structures - don't touch the return code */
    LogDebug(1, "done database verification, cleaning up\n");
    /* free the block map */
    if ( blockMap != 0 )
    {
	int i;

	/* free all the individual maps */
	for ( i = 0; i < nBlocks; i++ ) {

	/*
	 * Free blocks have 0 block entries associated with them. We have
	 * to be careful to free the whole size of the blockMap
	 */

	  if ( blockMap[i] )
	    if (blockMap[i]->nEntries == 0)
	    (void)osi_Free(blockMap[i],
			   sizeof(*(blockMap[i])));
	  else
	    (void)osi_Free(blockMap[i],
			   sizeof(*(blockMap[i])) + (blockMap[i]->nEntries-1) *
			   sizeof(blockMap[i]->entries[0]));
	}
	/* free the pointer array */
	(void)osi_Free(blockMap, bmsize);
	blockMap = (struct blockMap **)0;
    }

    LogHeader("");				/* clear the header */
    LogDebug(2, "returning code %ld from verifyDatabase\n", code);
    return(code);
}


/* -----------------------------
 * interface routines
 * -----------------------------
 */

/* BUDB_DbVerify
 *	check the integrity of the database
 * exit:
 *	status - integrity: 0, ok; n, not ok (error value)
 *	orphans - no. of orphan blocks
 *	host - address of host that did verification
 */

error_status_t
BUDB_DbVerify(h, status, orphans, host)
  handle_t h;
     long *status;
     long *orphans;
     long *host;
{
    struct ubik_trans *ut;
    long code = 0, st = 0;
    char *rtn_Name="BUDB_DbVerify";
    int cancelState, cancelCode;
    struct sockaddr_in sockadd;
    struct sockaddr_in *sockaddrp;

    *status = *orphans = *host = 0;
    bak_Lock(rtn_Name, &cancelState, &cancelCode, BAK_CANCELS);
    if (cancelCode)
      return(cancelCode);

    if ( callPermitted(h) == 0 ) {
      bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
      return(BUDB_NOTPERMITTED);
    }
    code = InitRPC (&ut, LOCKREAD, (int *) 1);
    if ( code ) {
      bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
      return(code);
    }
    LogDebug(2, "Entering BUDB_DbVerify: \n");
    LogDebug(1, "verifying backup database\n");
    /* check the database */
    lock_ObtainShared(&(db.lock));
    code = verifyDatabase(ut, 0);
    if ( code )
      ubik_AbortTrans(ut);
    else
      code = ubik_EndTrans(ut);
    lock_ReleaseShared(&(db.lock));

    *status = miscData.errors; /* count of errors, at most 10 */
    *orphans = orphanCount;

    sockaddrp = &sockadd;
    st = rpcx_binding_to_sockaddr(h, sockaddrp);
    if (st) {
	*host = 0;
    } else {
	(void)rpcx_ipaddr_from_sockaddr(sockaddrp, (char *)host);
    }
    LogDebug(2, "Leaving BUDB_DbVerify, code %ld\n", code);
    LogDebug(1, "completed database verification\n");
    bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
    return(0);
}


/* ----------------------
 * debug support
 * ----------------------
 */

/* check_header
 *	do a simple sanity check on the database header
 */

check_header(callerst)
     char *callerst;
{
    static int iteration_count = 0;
    long eof;

    eof = ntohl(db.h.eofPtr);
    if ( (eof == 0) || (eof < 0) )
    {
	Log("Eof check failed, caller %s, eof %d\n",
	    callerst, eof);
    }

    eof -= sizeof(db.h);
    if ( eof < 0 )
    {
	Log("Adjusted Eof check failed, caller %s, eof %d\n",
	    callerst, eof);
    }

    iteration_count++;
    if ( iteration_count >= 10 )
    {
	Log("Eof ptr is %d\n", eof);
	iteration_count = 0;
    }
}
