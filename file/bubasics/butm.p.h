/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: butm.p.h,v $
 * Revision 1.1.10.1  1996/10/02  17:05:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:18  damon]
 *
 * $EndLog$
 */
/*
 * (C) Copyright 1991 Transarc Corporation.
 * All Rights Reserved.
 */
#include <dcedfs/param.h>
#include <dcedfs/bubasics.h>

/* mt does not work in AIX, hence the stuff below */
#ifdef AFS_AIX_ENV
#include <sys/tape.h>

struct stop st_com;
#ifdef AFS_AIX31_ENV
/*
 * AIX31 device driver neglected to provide this functionality.
 * sigh.
 */
struct always_wonderful {
	long	utterly_fantastic;
} st_status;
#else
struct stget st_status;
#endif

#define GENCOM st_com
#define GENGET st_status
#define GENOP st_op
#define GENCOUNT st_count
#define GENWEOF STWEOF
#define GENREW STREW
#define GENFSF STFSF
#define GENRETEN STRETEN
#define GENRESET STRESET
#define GENBSF STREW /* should be a no-op */ 
#define GENCALL(fid,com) ioctl(fid,STIOCTOP,com)

#else
#include <sys/mtio.h>

struct mtop mt_com;
struct mtget mt_status;

#define GENCOM mt_com
#define GENGET mt_status
#define GENOP  mt_op
#define GENCOUNT mt_count
#define GENWEOF MTWEOF
#define GENREW MTREW
#define GENFSF MTFSF
#define GENRETEN MTRETEN
#define GENBSF MTBSF
#define GENCALL(fid,com) ioctl(fid,MTIOCTOP,com)

#endif

typedef unsigned long ButmDate;

/* A generic structure that represent all types of file and block marks 
 * on tape
 */
typedef struct markStruct {
    long count;	    /* actual number of bytes valid in the block */
    long magic;	    
    long spares[4];
} markStruct;

#define BUTM_BLOCKSIZE	16384			/* tape block size */
#define BUTM_HDRSIZE	(6*sizeof(long))	/* sizeof(markStruct) */
#define	BUTM_DATASIZE  (BUTM_BLOCKSIZE - BUTM_HDRSIZE)

/* A structure containing all the components of a physical tape block.
 * Each layer adds a header to the file data. This structure is used to
 * avoid copying the data when a header is added.
 */

struct butm_tapeInfo {
    long  structVersion;
    struct {
	long (*mount)();
	long (*dismount)();
	long (*create)();
	long (*readLabel)();
	long (*seek)();
	long (*readFileBegin)();
	long (*readFileData)();
	long (*readFileDataCopy)();
	long (*readFileEnd)();
	long (*writeFileBegin)();
	long (*writeFileData)();
	long (*writeFileDataCopy)();
	long (*writeFileEnd)();
	long (*writeEOT)();
	long (*setSize)();
	long (*getSize)();
	long *spares[10];
    } ops;
    char  name[BU_MAXTAPELEN];
    long  position;			/* current position of tape */
    /* the next three fields are used for modeling tape usage */
    long  nBytes;			/* number of bytes written so far */
    long  kBytes;			/* NEW. no of Kbytes written */
    long  nRecords;			/* number of records written */
    long  nFiles;			/* number of files written */
    /* These fields provide the coefficients for the above variables */
    long  recordSize;			/* bytes per record */
    long  tapeSize;			/* size of tape */
    long  coefBytes;			/* length multiplier for bytes */
    long  coefRecords;			/*   ditto  records */
    long  coefFiles;			/*   ditto  files */
    int   simultaneousTapes;		/* # of tapes mounted simultaneously */
    int   status;			/* status of tape */
    int   flags;			/* e.g. read-only, sequential */
    char  *tcRock;			/* for random butc storage */
    char  *tmRock;			/* for random tape module storage */
    long  sizeflags;			/* can be added with impunity since
					   it is incore. Indicates what the 
					   units of tape size are. */
    long  spares[10];
    char  spareChars[64];
};


struct butm_tapeLabel {
    long magic;				/* Magic number to recognize a label */
    long structVersion;			/* structure version number */
    ButmDate creationTime;		/* when tape was first labeled */
    ButmDate expirationDate;		/* when tape can be relabelled */
    char name[BU_MAXTAPELEN];		/* tape identifier */
    char cell[BU_MAXCELLLEN];		/* cell which owns tape. */
    u_long dumpid;			/* which dump on this tape  */
    long useCount;			/* # times written */
    char comment[128];
    long size;
    char dumpPath[BU_MAX_DUMP_PATH];	/* dump schedule path name */
    long spares[8];			
    char spareChars[64];
};

struct progress {
    int   fid;				/* file id of simulated tape */
    char  reading;			/* read file operation in progress */
    char  writing;			/* write file operation in progress */
    char spares[2];			
};

typedef struct tapeConfig{
    char device[256];			/* tape drive device name */
    long capacity;			/* capacity of tape in this drive */
    long fileMarkSize;			/* size of file marks, in bytes */
} tapeConfig;

/* returns answer in bytes */
#define butm_remainingSpace(i) (1024*((i)->tapeSize - ((i)->kBytes*(i)->coefBytes + \
 (i)->nFiles*(i)->coefFiles + (i)->nRecords*(i)->coefRecords )))

/* returns answer in kbytes */
#define butm_remainingKSpace(i) ((i)->tapeSize - ((i)->kBytes*(i)->coefBytes + \
 (i)->nFiles*(i)->coefFiles + (i)->nRecords*(i)->coefRecords ))

#define BUTM_STATUS_OFFLINE    	(1<<0)	/* tape not mounted */
#define BUTM_STATUS_TAPEERROR  	(1<<1)	/* tape error encountered */
#define BUTM_STATUS_WRITEERROR 	(1<<2)	/* tape error during write */
#define BUTM_STATUS_READERROR  	(1<<3)	/* tape error during read */
#define BUTM_STATUS_SEEKERROR  	(1<<4)	/* tape error during positioning */
#define BUTM_STATUS_EOF        	(1<<5)	/* tape at EOF */
#define BUTM_STATUS_EOT      	(1<<6)	/* end of tape reached */

#define BUTM_FLAGS_READONLY 	(1<<0)	/* tape mounted read-only */
#define BUTM_FLAGS_SEQUENTIAL 	(1<<1)	/* tape is sequential access: sort positions */

/* now the procedure macros */
#define butm_Mount(i,t) (*((i)->ops.mount))(i,t)
#define butm_Dismount(i) (*((i)->ops.dismount))(i)
#define butm_Create(i,l) (*((i)->ops.create))(i,l)
#define butm_ReadLabel(i,l) (*((i)->ops.readLabel))(i,l)
#define butm_Seek(i,p) (*((i)->ops.seek))(i,p)
#define butm_ReadFileBegin(i) (*((i)->ops.readFileBegin))(i)
#define butm_ReadFileData(i,d,l,n) (*((i)->ops.readFileData))(i,d,l,n)
#define butm_ReadFileDataCopy(i,d,l,n) (*((i)->ops.readFileDataCopy))(i,d,l,n)
#define butm_ReadFileEnd(i) (*((i)->ops.readFileEnd))(i)
#define butm_WriteFileBegin(i) (*((i)->ops.writeFileBegin))(i)
#define butm_WriteFileData(i,d,l) (*((i)->ops.writeFileData))(i,d,l)
#define butm_WriteFileDataCopy(i,d,l) (*((i)->ops.writeFileDataCopy))(i,d,l)
#define butm_WriteFileEnd(i) (*((i)->ops.writeFileEnd))(i)
#define butm_WriteEOT(i) (*((i)->ops.writeEOT))(i)
#define butm_SetSize(i,s) (*((i)->ops.setSize))(i,s)
#define butm_GetSize(i,s) (*((i)->ops.getSize))(i,s)

/* Magic numbers on tape - For DFS backup */
#define FILE_MAGIC  1000000017			/* s/w file mark */
#define TAPE_MAGIC  1100000019			/* tape label block */
#define BLOCK_MAGIC 1100000015			/* file data block */
#define FILE_BEGIN  0				/* byte field in file mark */
#define FILE_END    1				/* byte field in file mark */
#define TM_EOT     -1				/* byte field in file mark */

/* Magic numbers on tape - For AFS backup */
#define AFS_FILE_MAGIC  1000000007		/* s/w file mark */
#define AFS_TAPE_MAGIC  1100000009		/* tape label block */
#define AFS_BLOCK_MAGIC 1100000005		/* file data block */

/* Types of tape operations - write and read */
#define WRITE_OP 1
#define READ_OP  0

