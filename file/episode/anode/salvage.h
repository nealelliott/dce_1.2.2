/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: salvage.h,v $
 * Revision 1.1.89.1  1996/10/02  17:18:58  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:28:48  damon]
 *
 * $EndLog$
 */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */
/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/salvage.h,v 1.1.89.1 1996/10/02 17:18:58 damon Exp $ */

#ifndef TRANSARC_EPI_SALVAGE_H
#define TRANSARC_EPI_SALVAGE_H

#include "fid.h"
#include "volume.h"
#include "sal_errors.h"			/* Pickup error messages */

/*
** Many of the structures which follow are in their infancy.
** They have not been optimized, neither for efficiency of
** access, nor for space.  There is much more interest to first
** provice the information.
*/

/* struct epiz_copiesVerify -- This structure is populated during an early pass
 *     to describe all the anodes with nonzero copies.  These anodes are ones
 *     referenced by COW anodes.  It is increased by the number which the
 *     backing anode claim point to it, and reduced by one for each COW anode
 *     that points to it.  This should result in a zero if everything is okay.
 *     A negative number means too many referencers.  A positive number means
 *     too few. */

struct epiz_copiesVerify {
    struct epiz_copiesVerify * next;
    u_long	volIx;
    u_long	maxAnodeIx;		/* length of array */
    struct {
	long	copies;		/* missing COW anode referencers */
	daddr_t block;			/* anode location */
	u_long offset;			/* since backing ref is not by index */
    } anode[1];				/* indexed by AnodeIx */
					/* note: since first index is 1, */
					/* copies[0] would always be zero */
};

/*
** anodeItem.
** This is a generic anode identifier.  It has enough
** delcaration to identify some specific anode.
** It appears on attribute lists.
**
** Attribute in salvageState: unsafeAnode.
** this is a list of anodes which will be marked as
** unsafe at the end of a salvage.  These may have
** been promoted from other kinds of errors.
** Each entry may or may not be completely decorated.
**
** Attribute in salvageState: dupAnodes.
** a list of the anodes which have duplicate block addresses.
*/
struct epiz_anode { /* these are all on a static list in salvage.c */
    struct epiz_anode *next;
    long        flags;
    long	refCnt;
    char *	name;			/* path name of file within volume */
    u_long	anodeIx;		/* anode number within volume */
    u_long	volIx;			/* volume number */
};

struct epiz_anodeItem {
    struct epiz_anode *anode;		/* identity info */
    struct epiz_anodeItem * next;
    u_long	flags;
    u_long	refCnt;			/* number of times allocated */
    union	aiPrivate {		/* Private Usage section */
	struct	ResFrag {		/* Fragment section */
	    u_long	FragLen;	    /* how many frags */
	    daddr_t	NewFrag;	    /* new frag location */
	    u_long	NewFirst;	    /* new frag offset */
	} ResFrag;			/* Block section */
	struct	ResBlock {
	    daddr_t	OldBlock;	    /* old (dup) block */
	    daddr_t	NewBlock;	    /* new block daddr */
	} ResBlock;
    } aiPrivate;

};
/* epiz_anodeItem.flags and epiz_anode.flags */
#define	EPIZ_AI_NAME	0x1		/* entry is decorated with name */
#define	EPIZ_AI_AFULL	0x2		/* aggregate was full */
#define	EPIZ_AI_VFULL	0x4		/* volume was full */
#define EPIZ_AI_ALLOC	0x8		/* space allocated allocated */

/* epiz_anodeItem abbreviations */
#define	aiFragLen	aiPrivate.ResFrag.FragLen
#define	aiNewFrag	aiPrivate.ResFrag.NewFrag
#define	aiNewFirst	aiPrivate.ResFrag.NewFirst
#define	aiOldBlock	aiPrivate.ResBlock.OldBlock
#define	aiNewBlock	aiPrivate.ResBlock.NewBlock

/* List of bad blocks multiply used */
/* For now, use simple mechanism */

struct epiz_dupBlockList {
    struct epiz_dupBlockList *next;
    daddr_t dupBlock;			/* daddr of dupBlock */
    short fragBitsLen;			/* length of fragBits */
    char * fragBits;			/* multiply used frags */
};

/* Salvage State Information */

struct epiz_salvageState {
    /* Current Status Information */
    u_long flags;			/* what's been done, to do, do next */
    u_long pass;			/* pass number */
    struct async_device *device;	/* device all this is running on */
    epi_anode_t aggrAnode;		/* aggregate anode (convenience) */
    u_long volIx;			/* index of volume we are doing */
    u_long anodeIx;			/* index of anode */
    epi_anode_t vol;			/* volume anode */
    epi_anode_t anode;			/* file anode */

    /* for path walk */
    epi_volFileId_t *fid;		/* fid of current file */
    epi_volFileId_t *parentFid;		/* fid of parent directory */
    char *pathname;			/* pathname of current file */
    u_long auxIx;			/* index of auxiliary anode */
    epi_anode_t aux;			/* auxiliary anode */
    int auxWhich;			/* type of auxiliary anode */

    /* for block data errors */
    struct /*  epix_blockLocation */ {
	u_long index;			/* block number in container, or, for
                                         * frags, see ssFragFirst/Length. */
	daddr_t block;			/* block we are working on */
	int tree;			/* which indirect tree or -1 => DIRECT
                                         * or -2 => fragmented. */
	u_int depth;			/* number of indirect blks used */
	daddr_t blocks[5];		/* disk addr of indirect blk */
	int indexes[5];			/* index in each indirect blk */
    } loc;				/* our 'current' block info */

    /* for bitmap errors */
    epi_anode_t bitmap;			/* bitmap container if available */
    u_long bitmapPage;			/* page of bitmap */
    daddr_t bitmapBlock;		/* block of bitmap */
    u_long bitmapIndex;			/* current byte in map */

    opaque qVolInfo;			/* info from 'current' volume */
    afs_hyper_t highAnode;			/* highest Anode verion number */
    u_long blkNo;			/* block that is duplicately used */

    /* other anode traversal info */
    long expectedType;			/* expected type of anodes in volume */
    /* these arrays indexed by EPIA_CFLAGS_TYPE */
    long specialIx[6];			/* index where special anode found */
    long expectedIx[6];			/* index SB says special anode has */

    /* callouts... */
    long (*dirScanPass)();		/* directory traversal */

    /* unsafe Housekeeping Area */
    struct epiz_anodeItem * unsafeList;	/* list of unsafe anodes */

    /* Anode reference (COW) copies verification */
    struct epiz_copiesVerify * copiesList;

    /* Restoration Lists */
    struct epiz_anodeItem * oughtRestore;/* these anodes did lose data */
    struct epiz_anodeItem * mayRestore;	/* these anodes could be trouble */
    /* Block reference counting */
    u_long (*blockRef)();		/* What to do for each block */
					/* ex: MarkUsed or SaveAnodeRef */

    struct epiz_SimBits *simBits;	/* reconstructed bitmap */
    struct epiz_dupBlockList *dupList;	/* list of dup blocks */
    struct epiz_anodeItem *dupAnodeList;/* " " dup anodes (from dupList) */
    struct epiz_anodeItem *badAcls;	/* " " anodes w/ bad acl's */
    struct epiz_anodeItem *badPlists;	/* " " anodes w/ bad plists's */
    struct epiz_anodeItem *zeroLinkCnt;	/* " " anodes refering to zero link
                                         * count anodes */
    struct epiz_anodeItem *badLinkCnts;	/* " " anodes w/ wrong link counts */
    struct epiz_anodeItem *reserveFrags;/* list of anodes and space needs */
    struct epiz_anodeItem *reserveBlocks;/* list of anodes and space needs */

    /* list of anode containers of trashed anodes */
    struct epiz_anodeItem * deadVolumes;
    struct epiz_anodeItem * deadFiles;

    /* quota counting */
    epia_quota_t quotaVal;		/* quota calculated value */
    epia_quota_t quotaAnode;		/* quota from an anode */

    /* File (epif style) management */
    u_long *linkCounts;			/* linkCounts list (computing) */
    u_long *fileLinkCounts;		/* linkCounts of the file */
    char *typeLinkCounts;		/* acl? plist? file? what are we
                                         * counting? */
    u_long lastIndex;			/* highest linkCount index */

    /* aggregate info */
    daddr_t maxBlock;			/* largest legal block addr from
                                         * superblock, or size of bitmap,
                                         * whichever is less*/
    daddr_t minBlock;			/* smallest legal block addr */

    /* statistics */
    u_long nVolumes;
    u_long nAnodes;
    u_long nAnodePages;
    u_long nBitmapPages;
    u_long nIndirectBlocks;
    u_long nDirectories;
    u_long nFiles;
    u_long nAuxs;
    u_long nSpares[6];
};
/* Easy of use #defines */
#define ssVolInfo(ss)	((struct epiv_status *)((ss)->qVolInfo))
#define ssFragFirst(ss) (*(u_int16 *)(&ss->loc.index))
#define ssFragLength(ss) (*(u_int16 *)((char *)(&ss->loc.index) + \
				       sizeof(u_int16)))

/* See volume.h and ../salvage/name.h for more on these macros. */
#define epiz_ToEPIAIndex(i) ((i)+1)
#define epiz_ToEPIVIndex(i) ((i)-1)

/* Values for salvageState.flags */

    /* parameter info */
#define	EPIZ_SS_SALVAGE		    0x1	/* write on the aggregate to fix it */
#define EPIZ_SS_VERBOSE		    0x2	/* report extra information */
#define EPIZ_SS_NODIRSCAN	    0x4	/* directory structure present */
#define EPIZ_SS_RECOVER		    0x8	/* run recover first */

#define EPIZ_SS_REPAIRCOWREFS	   0x10	/* patch COW blk refs to direct ones */
#define EPIZ_SS_NOCOWREFS	   0x20	/* don't allow COW references */
#define EPIZ_SS_MAYBECOWUSES	0x40000	/* blockmap repairs => DOCOWFIX */
#define	EPIZ_SS_DOCOWFIX	   0x40	/* need a repair scan for COW refs */
#define EPIZ_SS_MAYBERESALVAGE	   0x80	/* set resalvage on blockmap errors */

    /* error reporting */
#define EPIZ_SS_REPLACE_BITMAP    0x100	/* bitmap was seriously damaged and
                                         * should be replaced */
#define EPIZ_SS_FIX_SUPERBLOCK	  0x200	/* superblock info inconsistent */
#define EPIZ_SS_FIX_BADBLOCK   0x100000 /* badblock file damaged */
#define EPIZ_SS_MISC_PROBLEM	  0x400	/* some non-anode specific problem
                                         * occurred (otherwise see paths) */
#define EPIZ_SS_MISC_TROUBLE	  0x800	/* some (unfixable) problems occured */
#define EPIZ_SS_REPAIRED	 0x1000	/* aggregate was repaired */
#define EPIZ_SS_RESALVAGE	 0x2000	/* incomplete repair requires another
                                         * salvage pass */
#define EPIZ_SS_GROW_LOG	 0x4000	/* log too small */

    /* misc */
#define EPIZ_SS_GETPATHNAMES	0x10000	/* dir walk only for pathnames */
#define EPIZ_SS_BACKINGVOLUME	0x20000	/* current volume is backing */
			     /* 0x40000	see above */
#define EPIZ_SS_SETINCON	0x80000 /* errors in RO volume: set incon */
			    /* 0x100000	see above */
#define EPIZ_SS_LOGCLEAN       0x200000	/* log is ok and needs no recovery */
			    /* 0x400000	unused */
			    /* 0x800000	unused */

/* Values for salvageState.pass */

#define	EPIZ_SS_SCANPASS	1	/* Scan anodes on disk for problems */
#define	EPIZ_SS_DUPFIXPASS	2	/* fix duplicate block references */
#define	EPIZ_SS_COWFIXPASS	3	/* continue bad COW block ref fixes */

/* Values for salvageState.fileLinkCounts[i] */
#define	EPIZ_SS_NOCNT		0	/* bzero sets to this */
#define	EPIZ_SS_FILECNT		1	/* epif_Open works, a good file */
#define	EPIZ_SS_ACLCNT		2	/* an acl */
#define	EPIZ_SS_PLISTCNT	3	/* a plist */
#define EPIZ_SS_DEADCNT		4	/* damaged by first pass */
#define EPIZ_SS_BADCNT		5	/* not a file, so delete dir entry */
#define EPIZ_SS_FREECNT		6	/* epiv_OpenAnode fails: deleted? */
#define EPIZ_SS_DELETECNT	7	/* epiv_OpenAnode works, but bogus */
#define	EPIZ_SS_ERROR		127	/* when we tried and couldn't get it */

/* There are five bits indicating various problem with the aggregate that were
 * detected and reported.  These bits are ORed together a returned as an exit
 * code.  See also epiz_GetExitCode and epiz_GetExitMessage.
 *     Bit 0x1 -- verification found one or more problems.
 *     Bit 0x2 -- salvage made some repair.
 *     Bit 0x4 -- a repair was made that necessitates further salvaging.
 *     Bit 0x8 -- a problem was detected that could not be repaired.
 *     Bit 0x10 -- there was some problem that prevents verification.
 * The larger the number the worse the problem.  In particular if the 0x10 bit
 * is set there is some serious problem preventing the program from running at
 * all.  If bit 0x8 is set the aggregate is, and remains, seriously damaged.
 * If bit 0x4 is set then a serious repair was started but major cleanup action
 * is indicated, so the salvager should be re-run.  This happens, for instance,
 * if a volume is damaged beyond repair, rather than attemps to cleaup
 * gracefully, it would be safer to delete the volume table anode and let a
 * subsequent salvage cleanup all the unallocated blocks, and other problems.
 * If bit 0x4 is set, bit 0x2 and 0x1 should also be set.  If salvage is
 * specified good exit codes are 0x0 or 0x3 (bits 0x1 and 0x2 should not appear
 * alone).  In verify-only only 0x0 and 0x1 are expected. */

#define EXIT_PROBLEMS	0x1
#define EXIT_REPAIRS	0x2
#define EXIT_RESALVAGE  0x4
#define EXIT_UNFIXABLE	0x8
#define EXIT_CRASH	0x10

#define EPIZ_PRINTBUFLEN 32

/* Shared macros */

/* epiz_Printed -- calls a procedure (or a macro) to generate a printed
 *     representation of an object.  The procedure is given the object, a
 *     buffer length and the buffer ptr.  This macro obtains the buffers from a
 *     circular pool used by the salvager. */

#define epiz_Printed(proc, obj) proc(obj, EPIZ_PRINTBUFLEN, epiz_NextPrintBuf())

/* Shared procedures */

#ifndef KERNEL
/* from volume.c */

/* SHARED */
extern long epiv_VerifyVolume _TAKES((
  struct epiz_salvageState *ss,
  epi_anode_t volC,
  long ix));

/* from aggregate.c */

extern long epig_VerifyExistingAggregate _TAKES((
  IN struct async_device *device,
  IN struct epiz_salvageState * ss));

/* from salvage.c */

/* SHARED */
extern struct epiz_anode * epiz_GetAnode _TAKES((
  IN u_long anodeIx,
  IN u_long volIx));

/* SHARED */
extern struct epiz_anode * epiz_FindAnode _TAKES((
  IN u_long anodeIx,
  IN u_long volIx));

/* SHARED */
extern struct epiz_anodeItem * epiz_FindAnodeItem _TAKES((
  IN struct epiz_anodeItem * list,
  IN u_long anodeIx,
  IN u_long volIx));

/* SHARED */
extern struct epiz_anodeItem * epiz_AllocAnodeItem _TAKES((
  IN struct epiz_anodeItem ** headP,
  IN u_long anodeIx,
  IN u_long volIx));

/* SHARED */
extern int epiz_DecorateAnode _TAKES((
  IN struct epiz_anode * anode,
  IN char * name));

/* SHARED */
extern long epiz_SalvageCopiesVerify _TAKES((
  IN epi_anode_t vol,
  IN u_long anodeIx,
  IN struct epiz_copiesVerify *cv,
  IN struct epiz_salvageState * ss ));

/* SHARED */
extern char *epiz_PrintedHyper _TAKES((	/* this should be a macro in stds.h */
  IN afs_hyper_t h,
  IN int len,				/* length of buffer */
  OUT char *buf));			/* printed result returned here */

/* SHARED */
extern char *epiz_NextPrintBuf _TAKES(( void ));

/* SHARED */
extern void epiz_SuppressError _TAKES((
  IN int why));

/* SHARED */
extern void epiz_ReportError
#if 0
_TAKES((
  IN struct epiz_salvageState * ss,
  IN long errorCode,			/* caller's code, if any */
  IN int why,
  IN opaque a,
  IN opaque b,
  IN opaque c));
#else
();
#endif

/* SHARED */
extern u_long epiz_AddCopiesVerify _TAKES((
  IN long copies,
  IN epi_anode_t Back,
  IN u_long bIndex,
  IN struct epiz_salvageState * ss));

/* SHARED */
extern long epiz_VerifyCopiesVerify _TAKES((
  IN struct epiz_salvageState * ss,
  IN long * cnt ));

/* SHARED */
extern struct epiz_anodeItem * epiz_FindResBlock _TAKES((
  IN struct epiz_anodeItem * headP,
  IN u_long anodeIx,
  IN u_long volIx,
  IN daddr_t block));

/* SHARED */
extern long epiz_EnumerateAnodeItems _TAKES((
  IN struct epiz_anodeItem * list,
  IN epi_anode_t aggrAnode,		 /* aggregate */
  IN long (*proc)(),                    /* call for each entry */
  IN char *rock));                        /* pass through parameter */

/* SHARED */
extern void epiz_AddDupBlock _TAKES((
  IN daddr_t b,
  IN u_long fragno,
  IN struct epiz_salvageState * ss));

/* SHARED */
extern long epiz_IsDupBlock _TAKES((
  IN daddr_t b,
  IN u_long fragno,
  IN struct epiz_salvageState * ss));

/* SHARED */
extern char * epiz_ssVolName _TAKES((
  IN struct epiz_salvageState *ss ));

/* SHARED */
extern u_long epiz_ssQuotaLimit _TAKES((
  IN struct epiz_salvageState * ss ));

/* SHARED */
extern long epiz_DisplayNames _TAKES((
  IN char * typeName,           /* header for each line */
  IN struct epiz_anodeItem * list));

/* SHARED */
extern void epiz_DisplayAllNames _TAKES((
  IN struct epiz_salvageState * ss));

/* SHARED */
extern long epiz_AddReserveFrags _TAKES((
  IN epi_anode_t file,
  IN u_long len,
  IN struct epiz_salvageState * ss));

/* SHARED */
extern long epiz_AddReserveBlock _TAKES((
  IN epi_anode_t file,
  IN u_long blkno,
  IN struct epiz_salvageState * ss));

/* from file.c */

/* SHARED */
extern int epiz_LegalFileStatusLength _TAKES((
  IN int len));

/* SHARED */
extern long epiz_SalvageLinkCount _TAKES((
  IN epi_anode_t hP,
  IN long cnt));

/* SHARED */
extern long epiz_VerifyFile _TAKES((
  IN epi_anode_t vol,
  IN epi_volFileId_t *fid,
  IN struct epiz_salvageState * ss));

typedef long (*epiz_AuxVerify_t)
  (char *, u_long, struct epiz_salvageState *, opaque);

/* SHARED */
extern long epiz_VerifyFileAux _TAKES((
  IN epi_anode_t aP,
  IN long flags,
  IN struct epiz_salvageState * ss,
  IN epiz_AuxVerify_t aclProc,
  IN epiz_AuxVerify_t plistProc,
  IN opaque rock ));

/* SHARED */
extern long epiz_InitZeroLinks _TAKES(( void ));

/* SHARED */
extern long epiz_AllocZeroLink _TAKES((
  IN u_int32 index,
  IN struct epiz_salvageState * ss));

/* SHARED */
extern long epiz_SalvageZeroLink _TAKES((
  IN epi_anode_t volC,
  IN struct epiz_salvageState * ss ));

/* from volume_table.c */

/* SHARED */
extern long epit_VerifyVolumeTable _TAKES((
  IN struct epiz_salvageState *ss,
  IN epi_anode_t volC,
  IN long (*proc)(),			/* call for each entry */
  IN char *rock));			/* pass through parameter */

/* from fixed_anode.c */

#endif	/* KERNEL */

#endif /* TRANSARC_EPI_SALVAGE_H */
