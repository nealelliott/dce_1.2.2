/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sal_errors.h,v $
 * Revision 1.1.87.1  1996/10/02  17:18:52  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:28:41  damon]
 *
 * $EndLog$
 */

/* sal_errors.h -- provides a table of error messages and parameters. */

/* Copyright (C) 1996, 1991 Transarc Corporation - All rights reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/sal_errors.h,v 1.1.87.1 1996/10/02 17:18:52 damon Exp $ */

#ifndef TRANSARC_EPI_SAL_ERRORS_H
#define TRANSARC_EPI_SAL_ERRORS_H

#ifdef EPIZ_E_INITIALIZER		/* only in salvage.c */

#define EPIZ_E_NREASONS 120
#define EPIZ_E_OUGHTRESTORE	   0x1
#define EPIZ_E_MAYRESTORE	   0x2
#define EPIZ_E_BADLINKCNTS	   0x3
#define EPIZ_E_BADAUX		   0x4	/* add to bad{Acl,Plist}s & mayRest. */
#define EPIZ_E_REGULARLIST	   0xf
#define EPIZ_E_DEADANODES	  0x10	/* add to dead volumes or anodes list.
                                         * implies RESALVAGE */
#define EPIZ_E_RESALVAGE	 0x100	/* rerun salvage to clean up mess */
#define EPIZ_E_TROUBLE		 0x200	/* can't fix these */
#define EPIZ_E_BLOCKDATA	 0x400	/* traversing blocks */
#define EPIZ_E_BITMAP		 0x800	/* verifying bitmaps */
#define EPIZ_E_WARN		0x1000	/* just report these */
#define EPIZ_E_ADVISE		0x4000	/* report these if -verbose */

#define EPIZ_E_SUPPRESS		0x2000	/* suppress printing of this msg */

			     /* 0x4000 see above */
			     /* 0x8000 unused */
			    /* 0x10000 unused */
			    /* 0x20000 unused */
			    /* 0x40000 unused */
			    /* 0x80000 unused */

struct epiz_errorData {
    char *msg;				/* text to report problem */
    char *fix;				/* add't'l text to report fix */
    int flags;
} epiz_errorData [EPIZ_E_NREASONS] = {
    {0,0,0},
    {/*2SMALLVOLVERSION*/
	"Volume version %s smaller than largest anode version number seen %s",
	 "setting VV to highest", 0 },
    {/*AGGREGATEFULL*/"Failed to repair directory because aggregate is full",
	 0, 0},
    {/*AUX_ACLNOTLOGGED*/"ACL was not logged",
	 "deleting ACL", EPIZ_E_BADAUX},
    {/*AUX_BADBACKING*/"Auxiliary container in backing fileset is irreparable",
	 0, 0},
    {/*AUX_BADLINK*/"Auxiliary container's link count is %d but should be %d",
	 "adjusting link count", EPIZ_E_BADLINKCNTS},
    {/*AUX_BADTYPE*/"Auxiliary anode previous referenced with different type",
	 "removing auxiliary container reference", EPIZ_E_BADAUX},
    {/*AUX_BOGUSINDEX*/"Auxiliary anode has fid-style index",
	 "removing auxiliary container reference", EPIZ_E_BADAUX },
    {/*AUX_INVALID*/"Auxiliary container's contents are invalid",
	 "deleting auxiliary container", EPIZ_E_BADAUX},
    {/*AUX_INVALIDINDEX*/"Auxiliary anode has invalid index",
	 "removing auxiliary container reference", EPIZ_E_BADAUX},
    {/*AUX_NOT*/"Auxiliary container is invalid",
	 "deleting auxiliary container", EPIZ_E_BADAUX},
    {/*AUX_ORPHAN*/"Auxiliary container is unreferenced",
	 "deleting auxiliary container", 0 },
    {/*AUX_PLISTBADLOGGED*/"Auxiliary plist was not logged or user plist was",
	 "removing auxiliary container reference", EPIZ_E_BADAUX},
    {/*AUX_ZEROLINK*/"Auxiliary container's link count is zero",
	 "removing auxiliary container reference", EPIZ_E_BADAUX},
    {/*AVL_BADINDEX*/"AVL claims index is %d but SB says %d",
	 0, EPIZ_E_TROUBLE},
    {/*AVL_BADSPECIALINDEX*/"SB claims %s index is %d, but found at index %d",
	 0, EPIZ_E_TROUBLE},
    {/*AVL_DUPLICATESPECIAL*/
	"Duplicate %s anode at index %d was also found earlier at index %d",
	0, EPIZ_E_TROUBLE},
    {/*AVL_ILLEGALTYPE*/"AVL anode type %d is unknown", 0, EPIZ_E_TROUBLE},
    {/*AVL_MISSINGSPECIAL*/"No anode of type %s found in AVL",
	 0, EPIZ_E_TROUBLE},
    {/*AVL_NOBLOCKZERO*/"AVL has no block zero", 0, EPIZ_E_TROUBLE},
    {/*AVL_SCANERROR*/"Failed scanning pages of AVL", 0, EPIZ_E_TROUBLE},
    {/*AVL_WRONGBLOCKZERO*/"AVL maps block zero to %#x", 0, EPIZ_E_TROUBLE},
    {/*BADALLOC*/"Anode allocated %d, calculated %d",
	"repairing allocated mismatch", 0},
    {/*BADANODEINDEX*/"Anode contains wrong index (%lu)", 0, 0},
    {/*BADAVL*/"AVL volume table is damaged", 0, EPIZ_E_TROUBLE},
    {/*BADBACKINGDIR*/"Directory in backing fileset is irreparable", 0, 0},
    {/*BADBACKINGLINKS*/"Backing fileset has irreparable link count errors",
	 0, 0},
    {/*BADBACKINGORPHAN*/
	"Backing fileset contains a peculiar anode that can not be deleted",
	0, 0},
    {/*BADBACKINGZLC*/"Backing fileset has irreparable zero link count list",
	 0, 0},
    {/*BADBITMAP*/"Bitmap is damaged", 0, EPIZ_E_TROUBLE},
    {/*BADBITS*/"(bits) block/frag %#x/%#x (computed %#x != disk %#x)",
	 0, EPIZ_E_BITMAP},
    {/*BADCOPIES*/"Anode copies field too high (low) by %d",
	 "adjusting copies count", 0},
    {/*BADDIR*/
	"Directory entry %s (fid %s) references directory damaged beyond repair",
	"removing subdirectory", EPIZ_E_OUGHTRESTORE },
    {/*BADFID*/"Directory entry %s contains invalid fid: %s",
	 "removing directory entry", EPIZ_E_OUGHTRESTORE },
    {/*BADIB*/"Indirect block header is damaged",
	 "removing indirect block", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA},
    {/*BADLINKFILE*/"File's link count is %d but should be %d",
	 "adjusting link count", EPIZ_E_BADLINKCNTS},
    {/*BADLOGGED*/"(logged) block %#x (computed %#x != disk %#x)",
	 0, EPIZ_E_BITMAP},
    {/*BADORPHANFILE*/"Volume index contains a peculiar anode",
	 "deleting anode", 0 },
    {/*BADROOTDIR*/"Volume root directory is damaged",
	 "moving all files to zero link list", EPIZ_E_OUGHTRESTORE },
    {/*BADSB*/"Aggregate has bad superblock because %s", 0, EPIZ_E_TROUBLE},
    {/*BADVISIBLE*/"Anode visible %d, calculated %d",
	 "repairing visible mismatch", 0},
    {/*BADVOLUME*/"Volume is damaged", "marked as inconsistent", 0 },
    {/*BADZLC*/"Zero link count list is wrong", "replacing list", 0},
    {/*BB_BADBLOCK*/
	"Bad blocks file contains block address (%#x) outside the aggregate",
	0, EPIZ_E_ADVISE},
    {/*BB_BADENTRY*/"Bad blocks file contains entry with bad length",
	 "making file empty", EPIZ_E_TROUBLE},
    {/*BB_COW*/"Bad blocks file is COW",
	 "making file empty", EPIZ_E_TROUBLE|EPIZ_E_BLOCKDATA},
    {/*BB_DUPBLK*/"Bad blocks file contains duplicate block address: %#x",
	 0, EPIZ_E_TROUBLE},
    {/*BB_IOERROR*/"Bad blocks file can not be read",
	 "making file empty", EPIZ_E_TROUBLE},
    {/*BB_MAGIC*/"Bad blocks file contained bad magic number %d",
	 "making file empty", EPIZ_E_TROUBLE},
    {/*BB_TOOLONG*/"Bad blocks file is longer than 2^32-1 bytes",
	 "making file empty", EPIZ_E_TROUBLE|EPIZ_E_BLOCKDATA},
    {/*BLOCK_BEYONDDEV*/"Block reference beyond end of device",
	 "removing block reference", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA },
    {/*BLOCK_BEYONDLEN*/"Block reference beyond end of file",
	 "removing block reference", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA },
    {/*BLOCK_ILLEGALCOWREF*/
	"Backing block reference not allowed in non-COW anode",
	"removing block reference", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA},
    {/*COWFIXPASS*/"Starting copy-on-write repair pass", 0, EPIZ_E_WARN},
    {/*COW_BADBLOCK*/"Backing block reference is to incorrect block: %#x",
	 "removing block reference", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA},
    {/*COW_BADFRAG*/"Backing anode has different fragment description",
	 "making file empty", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA},
    {/*COW_BADIB*/"Backing reference to invalid indirect block",
	 "removing block reference", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA},
    {/*COW_BADPTR*/"Couldn't open backing anode (blk/off %#x/%u)",
	 "disconnecting backing pointer (unclone)", EPIZ_E_MAYRESTORE},
    {/*COW_BADVOL*/"Could not open backing anode's volume (avl #%d)",
	 "disconnecting backing pointer (unclone)", EPIZ_E_MAYRESTORE},
    {/*COW_NOTBLOCKED*/"Backing block reference is to unblocked anode",
	 "removing block reference", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA},
    {/*COW_NOTFRAGMENTED*/"Backing anode is not fragmented",
	 "making file empty", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA},
    {/*DEADANODE*/"Anode damaged beyond repair",
	 "clearing anode", EPIZ_E_DEADANODES },
    {/*DEADFILE*/
	"Directory entry %s (fid %s) references file damaged beyond repair",
	"removing directory entry and deleting file", EPIZ_E_OUGHTRESTORE },
    {/*DIRERROR*/"Directory not okay because %s", "rebuilding directory", 0},
    {/*DIRNOTLOGGED*/"Directory is not logged",
	 "deleting directory", EPIZ_E_OUGHTRESTORE},
    {/*DIRSALERROR*/"Directory salvage error because %s", 0, 0},
    {/*DUPFIXPASS*/"Starting duplicate repair pass", 0, EPIZ_E_WARN},
    {/*FILELOGGED*/"Regular file is logged",
	 "deleting file", EPIZ_E_OUGHTRESTORE},
    {/*FILEPROBLEM*/"File got unexpected error", 0, 0},
    {/*FRAG_BEYONDDEV*/"fragment's block reference beyond end of device",
	 "making file empty", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA},
    {/*FRAG_BEYONDLEN*/"Fragment reference beyond end of file",
	 "making file empty", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA },
    {/*FRAG_BLOCKEMPTY*/"Fragmented file has empty fragment block reference",
	 "making file empty", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA},
    {/*FRAG_ILLEGALCOWREF*/
	"Backing fragment reference not allowed in non-COW anode",
	"making file empty", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA},
    {/*IBLOCK_BEYONDLEN*/"Indirect block beyond end of file",
	 "removing indirect block", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA },
    {/*MAXINLINE*/"Length (%lu,%lu) of inline anode is greater than maximum inline data size (%lu)",
	"setting length to maximum inline value", EPIZ_E_MAYRESTORE},
    {/*NEGATIVECOPIES*/"Anode copies field (%d) is negative", 0, 0},
    {/*NOAVLCOPIES*/"Anode in AVL has non-zero copies count of %u",
	 "clearing copies count", 0},
    {/*NOAVLCOW*/"Anodes in AVL can't be copy-on-write",
	 "disconnecting backing pointer (unclone)", 0},
    {/*NODOT*/"Directory missing \".\" entry, needs one with fid %s",
	 "creating entry for \".\"", 0},
    {/*NODOTDOT*/"Directory missing \"..\" entry, needs one with fid %s",
	 "creating entry for \"..\"", 0},
    {/*NOLINKS*/
	"Directory entry %s (fid %s) references file with improper link count",
	"removing directory entry", 0 },
    {/*NOTREADONLY_HASBACKING*/
	"Fileset not marked as READONLY but contains backing anodes",
	0, EPIZ_E_WARN},
    {/*NOVISIBLEQUOTAMSGS*/
	"Suppressing visible quota errors for individual anodes",
	0, EPIZ_E_WARN},
    {/*OLDMINOR_VISIBLEQUOTA*/
	"Disk format minor version shows no visible quota support",
	"updating aggregate header", 0},
    {/*PATCHBACKING*/0,
	 "clearing COW bit in block reference of anode being uncloned",
	 EPIZ_E_BLOCKDATA},
    {/*REPLACEDOT*/"Directory \".\" entry in error, needs one with fid %s",
	 "replacing entry for \".\"", 0},
    {/*REPLACEDOTDOT*/"Directory \"..\" entry in error, needs one with fid %s",
	 "replacing entry for \"..\"", 0},
    {/*TOOFARFRAGS*/
	"Fragmented file has fragments that cross a block boundary",
	 "removing all fragments", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA },
    {/*TOOFEWFRAGS*/
	"Fragmented file's length is too long, can't be larger than %s",
	 "reducing length", EPIZ_E_MAYRESTORE|EPIZ_E_BLOCKDATA },
    {/*TOOMANYFRAGS*/
	"Fragmented file claims more fragments that a block can hold",
	 "removing all fragments", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA },
    {/*UNEXPECTEDTYPE*/"Anode type field (%d) is unexpected", 0, 0},
    {/*VOLINCONSISTENT*/"Volume is marked as inconsistent, not walked",
	 0, EPIZ_E_WARN},
    {/*VOLQUOTA*/"Volume allocated/visible quota mismatched: usage %d/%d, computed %d/%d",
	 "resetting quota(s) to calculated value", 0 },
    {/*VOLQUOTAFAILED*/"Failed to repair volume quota", 0, EPIZ_E_TROUBLE },
    {/*ZEROFRAGS*/"Fragmented file claims to have zero fragments",
	 "removing all fragments", EPIZ_E_OUGHTRESTORE|EPIZ_E_BLOCKDATA },
    {/* not used *//*ZEROLINKFILE*/"File has zero references",
	 "being added to zero link count list", 0},

    /* new, unsorted */

    {/*2LARGEVOLVERSION*/"File volume version %s larger than volume's VV",
	 0, EPIZ_E_MAYRESTORE},
    {/*IRREPARABLECOPIES*/"Copies count cannot be repaired", 0,
	 EPIZ_E_RESALVAGE},
    {/*BITMAPTOOLARGE*/"Bitmap too large for aggregate's size",
	 0, EPIZ_E_TROUBLE},
    {/*BB_TOOSHORT*/"Bad blocks frame is shorter than 16 bytes",
	 "making file empty", EPIZ_E_TROUBLE|EPIZ_E_BLOCKDATA},
    {/*AVL_WRONGBLOCKSIZE*/"Block size (%ld) is too small for VM pagesize",
	 0, EPIZ_E_TROUBLE},
    {/*BITMAP_DAMAGED*/
	"Bitmap container was damaged, skipping bitmap checks and all repairs",
	0, 0},
    {/*BADANODEBLOCK*/"Volume table page %d is damaged",
	 "Clearing anode block", 0},
    {/*LOG_CANNOTCHECKRECOVERY*/
	"Log is too badly damaged to consider running recovery", 0, 0},
    {/*LOG_NEEDRECOVERY*/"Log has not been recovered", 0, EPIZ_E_WARN},
    {/*LOG_RECOVERYUNNEEDED*/"Log is clean; no recovery is necessary",
	 0, EPIZ_E_WARN},
    {/*LOG_TOOSMALL*/"A log of %d blocks is too small to use", 0, 0},
    {/*LOG_RECOVERYFAILED*/"Failed initializing log and running recovery",
	 0, EPIZ_E_TROUBLE},
    {/*AUX_UNAUTHMASK*/
	"ACL contained an unauth_mask entry that is being ignored",
	0, EPIZ_E_WARN},
    {/*OLDMINOR_TIMESTAMPS*/
	"Disk format minor version shows no AVL/Log timestamps",
	"updating aggregate header", 0},
    {/*OLDMINOR*/
	"Disk format minor version (%d) is old; current is %d",
	0, EPIZ_E_WARN},
    {/*DIRTOOLONG*/"Directory length (%s) is longer than 2^32",
	 "deleting directory", EPIZ_E_OUGHTRESTORE},

};
#endif /* only in salvage.c */

#define EPIZ_E_NOOP -1			/* must not be zero */
#define EPIZ_E_2SMALLVOLVERSION 1
#define EPIZ_E_AGGREGATEFULL 2
#define EPIZ_E_AUX_ACLNOTLOGGED 3
#define EPIZ_E_AUX_BADBACKING 4
#define EPIZ_E_AUX_BADLINK 5
#define EPIZ_E_AUX_BADTYPE 6
#define EPIZ_E_AUX_BOGUSINDEX 7
#define EPIZ_E_AUX_INVALID 8
#define EPIZ_E_AUX_INVALIDINDEX 9
#define EPIZ_E_AUX_NOT 10
#define EPIZ_E_AUX_ORPHAN 11
#define EPIZ_E_AUX_PLISTBADLOGGED 12
#define EPIZ_E_AUX_ZEROLINK 13
#define EPIZ_E_AVL_BADINDEX 14
#define EPIZ_E_AVL_BADSPECIALINDEX 15
#define EPIZ_E_AVL_DUPLICATESPECIAL 16
#define EPIZ_E_AVL_ILLEGALTYPE 17
#define EPIZ_E_AVL_MISSINGSPECIAL 18
#define EPIZ_E_AVL_NOBLOCKZERO 19
#define EPIZ_E_AVL_SCANERROR 20
#define EPIZ_E_AVL_WRONGBLOCKZERO 21
#define EPIZ_E_BADALLOC 22
#define EPIZ_E_BADANODEINDEX 23
#define EPIZ_E_BADAVL 24
#define EPIZ_E_BADBACKINGDIR 25
#define EPIZ_E_BADBACKINGLINKS 26
#define EPIZ_E_BADBACKINGORPHAN 27
#define EPIZ_E_BADBACKINGZLC 28
#define EPIZ_E_BADBITMAP 29
#define EPIZ_E_BADBITS 30
#define EPIZ_E_BADCOPIES 31
#define EPIZ_E_BADDIR 32
#define EPIZ_E_BADFID 33
#define EPIZ_E_BADIB 34
#define EPIZ_E_BADLINKFILE 35
#define EPIZ_E_BADLOGGED 36
#define EPIZ_E_BADORPHANFILE 37
#define EPIZ_E_BADROOTDIR 38
#define EPIZ_E_BADSB 39
#define EPIZ_E_BADVISIBLE 40
#define EPIZ_E_BADVOLUME 41
#define EPIZ_E_BADZLC 42
#define EPIZ_E_BB_BADBLOCK 43
#define EPIZ_E_BB_BADENTRY 44
#define EPIZ_E_BB_COW 45
#define EPIZ_E_BB_DUPBLK 46
#define EPIZ_E_BB_IOERROR 47
#define EPIZ_E_BB_MAGIC 48
#define EPIZ_E_BB_TOOLONG 49
#define EPIZ_E_BLOCK_BEYONDDEV 50
#define EPIZ_E_BLOCK_BEYONDLEN 51
#define EPIZ_E_BLOCK_ILLEGALCOWREF 52
#define EPIZ_E_COWFIXPASS 53
#define EPIZ_E_COW_BADBLOCK 54
#define EPIZ_E_COW_BADFRAG 55
#define EPIZ_E_COW_BADIB 56
#define EPIZ_E_COW_BADPTR 57
#define EPIZ_E_COW_BADVOL 58
#define EPIZ_E_COW_NOTBLOCKED 59
#define EPIZ_E_COW_NOTFRAGMENTED 60
#define EPIZ_E_DEADANODE 61
#define EPIZ_E_DEADFILE 62
#define EPIZ_E_DIRERROR 63
#define EPIZ_E_DIRNOTLOGGED 64
#define EPIZ_E_DIRSALERROR 65
#define EPIZ_E_DUPFIXPASS 66
#define EPIZ_E_FILELOGGED 67
#define EPIZ_E_FILEPROBLEM 68
#define EPIZ_E_FRAG_BEYONDDEV 69
#define EPIZ_E_FRAG_BEYONDLEN 70
#define EPIZ_E_FRAG_BLOCKEMPTY 71
#define EPIZ_E_FRAG_ILLEGALCOWREF 72
#define EPIZ_E_IBLOCK_BEYONDLEN 73
#define EPIZ_E_MAXINLINE 74
#define EPIZ_E_NEGATIVECOPIES 75
#define EPIZ_E_NOAVLCOPIES 76
#define EPIZ_E_NOAVLCOW 77
#define EPIZ_E_NODOT 78
#define EPIZ_E_NODOTDOT 79
#define EPIZ_E_NOLINKS 80
#define EPIZ_E_NOTREADONLY_HASBACKING 81
#define EPIZ_E_NOVISIBLEQUOTAMSGS 82
#define EPIZ_E_OLDMINOR_VISIBLEQUOTA 83
#define EPIZ_E_PATCHBACKING 84
#define EPIZ_E_REPLACEDOT 85
#define EPIZ_E_REPLACEDOTDOT 86
#define EPIZ_E_TOOFARFRAGS 87
#define EPIZ_E_TOOFEWFRAGS 88
#define EPIZ_E_TOOMANYFRAGS 89
#define EPIZ_E_UNEXPECTEDTYPE 90
#define EPIZ_E_VOLINCONSISTENT 91
#define EPIZ_E_VOLQUOTA 92
#define EPIZ_E_VOLQUOTAFAILED 93
#define EPIZ_E_ZEROFRAGS 94
#define EPIZ_E_ZEROLINKFILE 95
#define EPIZ_E_2LARGEVOLVERSION 96
#define EPIZ_E_IRREPARABLECOPIES 97
#define EPIZ_E_BITMAPTOOLARGE 98
#define EPIZ_E_BB_TOOSHORT 99
#define EPIZ_E_AVL_WRONGBLOCKSIZE 100
#define EPIZ_E_BITMAP_DAMAGED 101
#define EPIZ_E_BADANODEBLOCK 102
#define EPIZ_E_LOG_CANNOTCHECKRECOVERY 103
#define EPIZ_E_LOG_NEEDRECOVERY 104
#define EPIZ_E_LOG_RECOVERYUNNEEDED 105
#define EPIZ_E_LOG_TOOSMALL 106
#define EPIZ_E_LOG_RECOVERYFAILED 107
#define EPIZ_E_AUX_UNAUTHMASK 108
#define EPIZ_E_OLDMINOR_TIMESTAMPS 109
#define EPIZ_E_OLDMINOR 110
#define EPIZ_E_DIRTOOLONG 111

/* SHARED */
char *epiz_specialAnodeName[6]
#ifdef EPIZ_E_INITIALIZER		/* only in salvage.c */
    = {0, "AVL", "<volume>", "Bitmap", "Log", "Badblocks"}
#endif
;

#endif /* TRANSARC_EPI_SAL_ERRORS_H */
