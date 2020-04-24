/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: database.h,v $
 * Revision 1.1.75.1  1996/10/02  16:59:45  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:13  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */
/* $Header: /u0/rcs_trees/dce/rcs/file/bakserver/database.h,v 1.1.75.1 1996/10/02 16:59:45 damon Exp $ */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1989
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include "budb.h"
#include <dcedfs/bubasics.h>
#include <dcedfs/lock.h>
#include <dcedfs/hyper.h>

typedef unsigned long dbadr;

struct hashTable {
    long functionType;			/* type of hash function */
    long threadOffset;			/* Byte offset of hash thread */
    long entries;			/* number of entries in hash table */
    long length;			/* number of hash table slots */
    dbadr table;			/* pointer to actual table */
    long progress;			/* number empty buckets in oldTable */
    long oldLength;			/* slots in old table */
    dbadr oldTable;			/* old table */
    long spare1;			/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
};

/* text blocks and related structures */

struct	textBlock
{
    u_long  version;			/* for access consistency checks */
    long  size;				/* size in bytes */
    dbadr textAddr;			/* list of blocks */
    long  newsize;			/* replacement size */
    dbadr newTextAddr;			/* list of new blocks */
    long spare1;			/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
};

struct db_lockS
{
    long type;                  /* user defined - for consistency checks */
    long lockState;             /* locked/free */
    long lockTime;              /* when locked */
    long expires;               /* when timeout expires */
    long instanceId;            /* user instance id */
    long lockHost;		/* locking host, if possible, NOT USED */
    afsNetAddr spareHost;	/* if we decide to record host identity */
    long spare1;		/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
};

typedef struct db_lockS    db_lockT;
typedef db_lockT           *db_lockP;

/* hash table function types */
#define HT_dumpIden_FUNCTION	1
#define HT_dumpName_FUNCTION	2
#define HT_tapeName_FUNCTION	3
#define HT_volName_FUNCTION	4

#define HT_MAX_FUNCTION		4

/* block types */

#define MAX_STRUCTURE_BLOCK_TYPE 4
#define free_BLOCK		0
#define volFragment_BLOCK	1
#define volInfo_BLOCK		2
#define tape_BLOCK		3
#define dump_BLOCK		4
#define hashTable_BLOCK		5
#define	text_BLOCK		6
#define spare1_BLOCK		7
#define spare2_BLOCK		8
#define spare3_BLOCK		9
#define spare4_BLOCK		10
#define spare5_BLOCK		11
#define NBLOCKTYPES		12 /* increased from 7, more types */

/* This defines root pointers to all parts of the database.  It is allocated
   starting at location zero. */

struct dbHeader {
    long  version;			/* initial version number. Must be the
					   first field in this structure */
    dumpId lastDumpId;			/* last dump id value */
    u_long  lastTapeId;			/* last tape id allocated */
    u_long lastInstanceId;		/* last instance Id handed out */
    dbadr freePtrs[NBLOCKTYPES];	/* pointers to free blocks */
    dbadr eofPtr;			/* first free byte in db */
    long  nHTBuckets;			/* size of hashtable blocks */
    Date  lastUpdate;			/* time of last change to database */
    long  ptrs[10];			/* spare pointers */
    struct hashTable volName;		/* hash tables */
    struct hashTable tapeName;
    struct hashTable dumpName;
    struct hashTable dumpIden;
    db_lockT textLocks[TB_MAX];		/* locks for text blocks */
    struct textBlock textBlock[TB_MAX];	/* raw text information */
    long  words[10];			/* spare words */
    long spare1;			/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
    long  checkVersion;			/* final version number. Must be the
					   last field in this structure */
};

/* The database is composed of several types of structures of different sizes.
   To simplify recovery from crashes, allocation is done in fixed size blocks.
   All structures in a block are of the same type.  Each block has a header
   which identifies its type, a count of the number of free structures, and a
   pointer to the next block of the same type. */

#define BLOCKSIZE 2048
#define BlockBase(a)							\
	((((a) - sizeof(struct dbHeader)) & ~(BLOCKSIZE-1)) +		\
	 sizeof(struct dbHeader))

struct blockHeader {
    char  type;				/* type: implies structure size */
    char  flags;			/* miscellaneous bits */
    short nFree;			/* number of free structures */
    dbadr next;				/* next free block same type */
    long spare1;			/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
};

#define	BLOCK_DATA_SIZE	(BLOCKSIZE-sizeof(struct blockHeader))

struct block {
    struct blockHeader h;
    char  a[BLOCKSIZE-sizeof(struct blockHeader)];
};

/* This macro defines the block containing each type of structure */
/*   to turn off padding define blockentrypad(words) to be a noop; to turn
 *   padding back on define it as below. */

#ifndef AFS_OSF_ENV
#define padentry(words) long pad[words];
#define blockentrypad(words) padentry(words)
#define blockstructure(abbrev,name) struct abbrev##Block {             \
    struct blockHeader h;						 \
    struct {								 \
	struct name s;							 \
	blockentrypad(((BLOCKSIZE-sizeof(struct blockHeader)) /		 \
		  (N##name##S)					 \
		  - sizeof (struct name)) / sizeof(long))		 \
    } a [N##name##S];						 \
};
#else
#define blockstructure(abbrev,name) struct abbrev##Block {             \
    struct blockHeader h;						 \
    struct {								 \
	struct name s;							 \
	char  pad[(BLOCKSIZE-sizeof(struct blockHeader)) /		 \
		  (N##name##S)						 \
		  - sizeof (struct name)];				 \
    } a [N##name##S];						 \
};
#endif	/* AFS_OSF_ENV */

#define NhtBucketS ((BLOCKSIZE-sizeof(struct blockHeader))/sizeof(dbadr))
struct htBlock {
    struct blockHeader h;
    dbadr bucket[NhtBucketS];
};

/* The database contains one volFragment for every occurence of volume data on
 * any tape. */

#define VOLFRAGMENTFLAGS 0xffff
struct volFragment {
    dbadr vol;				/* full volume info */
    dbadr sameNameChain;		/* next with same vol info */
    dbadr tape;				/* tape containing this fragment */
    dbadr sameTapeChain;		/* next fragment on tape */

    long  position;			/* on tape */
    Date  clone;			/* clone date of volume */
    Date  incTime;			/* time for incremental; 0 => full */
    long  startByte;			/* first byte of volume in this frag */
    long  nBytes;			/* bytes in fragment */
    short flags;			/* miscellaneous bits */
    short sequence;			/* seq of frag in dumped volume */
    long spare1;			/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
};
#define NvolFragmentS 30
blockstructure(vf,volFragment)

/* This represents additional information about a file system volume that
 * changes relatively infrequently.  Its purpose is to minimize the size of the
 * folFragment structure. */

#define VOLINFOFLAGS 0xffff0000

struct volInfo {
    char  name[BU_MAXNAMELEN];		/* name of volume: the hash key */
    long  flags;			/* miscellaneous bits */
    dfsh_diskHyper_t id;		/* read-write volume's id */
    char  server[BU_MAXHOSTLEN];	/* file server */
    long  partition;			/* disk partition on server */
    long  nFrags;			/* number of fragments on list */

    dbadr nameHashChain;		/* for volume name hash table */
    dbadr sameNameHead;			/* volInfo of first struct this name */
    dbadr sameNameChain;		/* next volume with this name */
    dbadr firstFragment;		/* all volFragment with this volInfo */
    long spare1;			/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
};
#define NvolInfoS 5
blockstructure(vi,volInfo)

/* The tape structure represents information of every physical tape in the
 * backup system.  Some of the data may persist event though the tape's
 * contents are repeatedly overwritten. */

struct tape
{
    char  name[BU_MAXTAPELEN];		/* name of physical tape */
    long  flags;			/* miscellaneous bits */
    Date  written;			/* tape writing started */
    Date  expires;			/* expiration date */
    u_long  nMBytes;			/* length of tape in Mbytes */
    u_long  nBytes;			/* remainder of Mbytes  */
    long  nFiles;			/*  ditto for  EOFs */
    long  nVolumes;			/*  ditto for  volume fragments */
    long  seq;				/* sequence in tapeSet */

    u_long  tapeid;			/* unique id */
    long  useCount;			/* # of times used */
    long  mediaType;			/* what kind of tape */

    dbadr nameHashChain;		/* for tape name hash table */
    dbadr dump;				/* dump (tapeSet) this is part of */
    dbadr nextTape;			/* next tape in dump (tapeSet) */
    dbadr firstVol;			/* first volume fragment on tape */
    long spare1;			/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
};

#define NtapeS /*20*/6
blockstructure(t,tape)

/* The structure describes every dump operation whose contents are still availe
 * on at least one tape.
 */

struct dump
{
    /* similar to budb_dumpEntry */
    dumpId id;			/* unique identifier for dump */
    dumpId parent;			/* parent dump */
    long  level;			/* dump level */
    long  flags;			/* miscellaneous bits */
    char  volumeSet[BU_MAXNAMELEN];	/* name of volume that was dumped */
    char  dumpPath[BU_MAX_DUMP_PATH];	/* "path" name of dump level */
    char  dumpName[BU_MAXNAMELEN];	/* dump name */
    Date  created;			/* time dump initiated */
/*  Date  incTime;			target time for incremental dumps */
    long  nVolumes;			/* number of volumes in dump */
    struct budb_tapeSet tapes;		/* tapes for this dump */
    u_long spare[2];			/* nominal spares */

    dbadr idHashChain;			/* for dump id hash table */
    dbadr nameHashChain;		/* for dump name hash table */
    dbadr firstTape;			/* first tape in dump (tapeSet) */
    long spare1;			/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
};

typedef	struct dump  dbDumpT;
typedef	dbDumpT	    *dbDumpP;

#define NdumpS 2
blockstructure(d,dump)

/* This structure is used to cache the hash table blocks for a database hash
   table.  The htBlock structure is modified slightly to accomodate this
   mechanism.  On disk the next ptr links the consecutive blocks of the hash
   table together.  In memory this information is inferred */

struct memoryHTBlock {
    int   valid;			/* block needs to be read in */
    dbadr a;				/* where stored in db */
    struct htBlock b;
    long spare1;			/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
};

struct memoryHashTable {
    struct hashTable *ht;		/* ptr to appropriate db.h hashtable */
    /* these are host byte order version of the corresponding HT fields */
    int threadOffset;			/* Byte offset of hash thread */
    int length;				/* number of hash table slots */
    int oldLength;			/* slots in old table */
    int progress;			/* number empty buckets in oldTable */
    /* these are the memory copies of the hash table buckets */
    int size;				/* allocated size of array */
    struct memoryHTBlock *(*blocks);	/* ptr to array of ht block pointers */
    int oldSize;			/*  ditto for old HT */
    struct memoryHTBlock *(*oldBlocks);
    long spare1;			/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
};

struct memoryDB { /* in core copies of database structures */
    osi_dlock_t lock;
    Date  readTime;
    struct dbHeader h;
    struct memoryHashTable volName;
    struct memoryHashTable tapeName;
    struct memoryHashTable dumpName;
    struct memoryHashTable dumpIden;
    struct textBlock textBlock[TB_NUM];
    long spare1;			/* nominal spares */
    long spare2;
    long spare3;
    long spare4;
};

/* The following macros should be called with db.lock locked for write */
#define set_header_word(ut,field,value) \
    dbwrite ((ut), ((char *)&(db.h.field) - (char *)&db.h), \
	     ((db.h.field = (value)), (char *)&(db.h.field)), \
	     sizeof(long))

#define set_word_offset(ut,a,b,offset,value)			      \
    dbwrite ((ut), (a)+(offset),				      \
	     (*(long *)((char *)(b) + (offset)) = (value),	      \
	      (char *)((char *)(b) + (offset))),		      \
	     sizeof(long))

#define set_word_addr(ut,a,b,addr,value)			      \
    dbwrite ((ut), (a)+((char *)(addr) - (char *)(b)),		      \
	     (*(long *)(addr) = (value),			      \
	      (char *)(addr)),					      \
	     sizeof(long))

struct memoryHashTable *ht_GetType ();
unsigned long ht_HashEntry();
dbadr ht_LookupBucket();
