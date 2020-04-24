/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * (C) Copyright 1993, 1996 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * ol_verify.h -- standalone on-line bak database verification program
 * 
 * $Header: /u0/rcs_trees/dce/rcs/file/bakserver/ol_verify.h,v 1.1.45.1 1996/10/02 17:02:53 damon Exp $
 */
/*
 * HISTORY
 * $Log: ol_verify.h,v $
 * Revision 1.1.45.1  1996/10/02  17:02:53  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:27  damon]
 *
 * $EndLog$
 */

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

/* flags associated with each structure. These are set and checked in 
 * the blockMap entries
 */

#define MAP_DUMPHASH            1	/* dump name hash checked */
#define MAP_TAPEHASH            2	/* tape name hash checked */
#define MAP_VOLHASH             4	/* volume name hash checked */
#define MAP_IDHASH              8	/* dump id hash checked */

#define MAP_HASHES (MAP_DUMPHASH | MAP_TAPEHASH | MAP_VOLHASH | MAP_IDHASH)

#define MAP_FREE                0x10	/* item is free */
#define MAP_RECREATE            0x20
#define MAP_HTBLOCK             0x40	/* hash table block */
#define MAP_TAPEONDUMP          0x100
#define MAP_VOLFRAGONTAPE       0x200
#define MAP_VOLFRAGONVOL        0x400
#define MAP_VOLINFOONNAME       0x800
#define MAP_VOLINFONAMEHEAD     0x1000
#define MAP_TEXTBLOCK		0x2000	/* block of text */

#define MAP_DOUBLEREF           0x10000         /* doubly referenced entry */
#define MAP_UNREF               0x20000         /* unreferenced entry */
#define MAP_HASHERROR           0x40000         /* entry hash locn. error */

#define MAP_ENTRYERROR          0x80000         /* bad link in entry tree */

#define MAP_ENTRY_ERROR (MAP_DOUBLEREF|MAP_UNREF|MAP_HASHERROR|MAP_ENTRYERROR)

/* max errors tolerated by verifydb before bailing out */
#define BUDB_VERIFY_MAXERRORS	10

/* one blockMap for every block in the database. Each element of the entries
 *	array describes the status of a data structure/entry in that block
 */

struct blockMap 
{
    struct blockHeader header;		/* copy of the block header */
    char  isfree;                         /* on free list */
    int   nEntries;                     /* size of the entries arrays */
    u_long entries[1];			/* describes each entry */
};

/* status for verify call */
struct dbStatus
{
    char hostname[64];		/* host on which checked */
    long status;		/* ok, not ok */
};


struct misc_hash_stats {                /* stats for hashing */
    int max;                            /* longest chain length */
    double avg;                         /* avg length */
    double std_dev;                     /* standard deviation of length */
};

struct misc_data {
    int errors;                         /* errors encountered */
    int maxErrors;                      /* abort after this many errors */
    int nBlocks;                        /* number of database blocks */
    int  nDump, nTape, nVolInfo, nVolFrag; /* counts of each type */
    int  nVolName;                      /* volInfos w/ head==0 */
    int  maxVolsPerVolInfo;             /* maximum list lengths */
    int  maxVolsPerTape;
    int  maxVolsPerDump;
    int  maxVolInfosPerName;
    int  maxTapesPerDump;
    int  freeLength[NBLOCKTYPES];       /* length of free lists */
    int  fullyFree[NBLOCKTYPES];        /* free blocks full of free entries */
    int  veryLongChain;                 /* length of chain to report */
    int  checkFragCount;                /* report fragment count errors */
    struct misc_hash_stats dumpName, dumpIden, tapeName, volName;
    FILE *recreate;                     /* stream for recreate instructions */
};
