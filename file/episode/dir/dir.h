/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dir.h,v $
 * Revision 1.1.78.1  1996/10/02  17:20:22  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:38  damon]
 *
 * $EndLog$
*/
/*
 * Copyright (C) 1994, 1990 Transarc Corporation
 * All rights reserved.
 */

/*                             DEcorum File System
                                Directory Package                        */

#ifndef DIR_H
#define DIR_H

#include <sys/types.h>
#include <dcedfs/episode/fid.h>		/* epi_volFileId_t */
#include <dcedfs/episode/logbuf.h>
#include <dcedfs/dir_buffer.h>

/* Max object name length allowed i.e. maximum # chars in dir entry name */
#define EFS_MAX_FILENAME_LENGTH 256

/* constants */

#define DIRPAGESIZE 8192		/* bytes per page */
#define LOGDIRPAGESIZE 13		/* log2 of DIRPAGESIZE */
#define NHASHENT 128			/* entries in the hash tbl */
#define MAXPAGES 65536			/* max pages in a dir */
#define PIECESIZE 32			/* bytes per piece */
#define PIECEMASK (PIECESIZE - 1)	/* dir size must not have these bits */
#define LOGPIECESIZE 5			/* log2 of PIECESIZE */
#define BASENAMESIZE 16			/* # name chars that can fit in
					 * entry of size 1 piece
					 * (not necessarily a power of 2) */
#define NONNAMESIZE (PIECESIZE - BASENAMESIZE - 1)
					/* entry size excluding name */
#define EPP 256				/* pieces (entries) per page;
					 * equal to DIRPAGESIZE / PIECESIZE */
#define LEPP 8				/* log2 of EPP */

#define DIR_OFF_TO_PAGE(off)	((off) >> LOGDIRPAGESIZE)
#define DIR_OFF_TO_PIECE(off)	(((off) + PIECESIZE - 1) >> LOGPIECESIZE)
#define DIR_PIECE_TO_OFF(piece)	((piece) << LOGPIECESIZE)

/* Magic numbers */

#define ENTRY_MAGIC     0x76e694c1
#define NON_ENTRY_MAGIC 0xb937c268
#define DIR_MAGIC       0x2c70bf7f
#define DIR_PAGE_MAGIC  0xb63185f6

/*
 * Structures used in the on-disk representation of directories.
 *
 */

/* directory header */

struct Dir_Header {
    long dir_magic;			/* magic number (NET-ORDER) */
    u_char flist;			/* head of free list */
    u_char pnhimark;			/* piece number of high-water mark
					 * for 1-page dir */
    u_short pgcount;			/* page count (NET-ORDER) */
};

/* page header */

struct Dir_Page_Header {
    long dir_page_magic; 		/* magic number (NET-ORDER) */
    u_char flist;			/* head of free list */
};

/* page header with hash table */

struct Dir_Page {
    union {
	struct Dir_Header hd;
	struct Dir_Page_Header pghd;
    } udh;
    char DP_pad[PIECESIZE-sizeof(struct Dir_Header)];
					/* padded to size of piece;
					 * in using sizeof Dir_Header,
					 * we are assuming Dir_Header is
					 * larger than Dir_Page_Header. */
    u_char hashTable[NHASHENT];		/* hash table */
};

#define HDRPIECES (sizeof(struct Dir_Page) >> LOGPIECESIZE)
					/* # pieces in a dir page header */

/* Directory entry, or free area;            */
/*   sizeof had better be equal to PIECESIZE */

struct Dir_Entry {
    long dir_entry_magic;		/* magic number (NET-ORDER) */
    epi_volFileId_t fid;		/* file identifier */
    u_char next;			/* hash chain ptr */
    u_char length;			/* length in pieces */
    u_char inuse;			/* 1 if entry, 0 if free area */
    char name[BASENAMESIZE + 1];	/* name */
};

/* Exported functions */

IMPORT int dir_Hash (register char *, u_int *);
IMPORT u_int dir_NamePieces (u_int );
IMPORT int dir_Create(struct Dir_Handle *,
		char *, 
		epi_volFileId_t *,
		long ,
		int ,
		logTranRec_t );
IMPORT int dir_Delete(struct Dir_Handle *,
		long ,
		logTranRec_t );
IMPORT int dir_Replace(struct Dir_Handle *,
		long ,
		epi_volFileId_t *,
 		logTranRec_t );
IMPORT int dir_FixDotDot(struct Dir_Handle *,
		epi_volFileId_t *,
		logTranRec_t );
IMPORT int dir_MakeDir(struct Dir_Handle *,
		epi_volFileId_t *,
		epi_volFileId_t *,
		logTranRec_t );
IMPORT int dir_Lookup(struct Dir_Handle *,
		char *,
		epi_volFileId_t *,
		long * );
IMPORT int dir_PieceScan(struct Dir_Handle *,
		u_long );
IMPORT int dir_IsEmpty(struct Dir_Handle *);
IMPORT int dir_AddPage(struct Dir_Handle *,
		logTranRec_t,
		int );
IMPORT int dir_SetPageCount(struct Dir_Handle *, int, logTranRec_t);
IMPORT int dir_FitsAtEnd(struct Dir_Handle *, int *, char *, int *);
IMPORT struct Dir_Entry * dir_GetPiece(struct Dir_Handle *,
                long pieceno,
                struct Dir_Page **dpPP );		
IMPORT void dir_FindFreeSpace(struct Dir_Page *, int, u_int, int *);

#define dir_LastPage(dir) \
    (((DHOP_LENGTH((dir)) + DIRPAGESIZE - 1) >> LOGDIRPAGESIZE) - 1)

/* Exported functions from directory salvager */

EXPORT int dirs_OK _TAKES((
  IN struct Dir_Handle *dir,		/* directory */
  IN void (*Log)()));			/* function to pass err msgs to */

/* See explanations for these return codes in dirs_Salvage function comment. */
#define DIRS_EXIT_NODOT    1
#define DIRS_EXIT_NODOTDOT 2
#define DIRS_EXIT_BAD	   4
#define DIRS_EXIT_IO	   8

EXPORT int dirs_Salvage _TAKES((
  IN struct Dir_Handle *fromFile,	/* old (damaged) directory */
  IN struct Dir_Handle *toFile,		/* new directory */
  IN void (*Log)()));			/* function to pass err msgs to */

#endif /* DIR_H */
