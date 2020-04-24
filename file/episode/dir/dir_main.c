/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $EndLog$
 */
/*
 * Copyright (C) 1990 Transarc Corporation
 * All rights reserved.
 */

/*                         DEcorum file system
                            Directory package                    */

# include <dcedfs/stds.h>
# include <dcedfs/osi.h>
# include <dcedfs/osi_net.h>
# include <dcedfs/episode/logbuf.h>	/* logTranRec_t */
# include <dcedfs/episode/fid.h>	/* epi_volFileId_t */
#include <dir.h>
#include <dir_buffer.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/dir/RCS/dir_main.c,v 4.49 1994/11/02 20:13:06 cfe Exp $")

/*
 * Macro for use with dh_Write;
 * finds the difference in bytes between two addresses
 */

#define addrdif(a,b) ((char *) (a) - (char *) (b))

/* Macros to check for corruption */

#define BadFree(entP) ((entP)->dir_entry_magic != htonl(NON_ENTRY_MAGIC))
#define BadEntry(entP) ((entP)->dir_entry_magic != htonl(ENTRY_MAGIC))
#define BadArea(entP) ((entP)->inuse ? BadEntry(entP) : BadFree(entP))

/* Forward declarations */

static void ClearPage(struct Dir_Page *);
static int FreePieces(
    struct Dir_Handle *, logTranRec_t, struct Dir_Page_Header *,
    long, struct Dir_Entry *);
static int FindEntry(
    struct Dir_Handle *, char *, struct Dir_Page **,
    long *, struct Dir_Entry **);

/*
 * Error handling
 *
 * Calls to DHOP_WRITE will generally not return EIO (because the output is
 * asynchronous), but they may return EDQUOT or ENOSPC.
 *
 * This can happen even for calls that do not extend the directory.  For
 * example an attempt to delete a link can return EDQUOT or ENOSPC.  This
 * could occur if the directory were copy-on-write, and the modification to
 * it required a new copy to be made, forcing allocation of one or more new
 * fragments.
 *
 * Thus most calls to DHOP_WRITE are checked for errors, and when there is
 * an error, previous calls to DHOP_WRITE in the same directory operation may
 * have to be "undone" (i.e. by writing the old data).
 */

/*
 * dir_NamePieces -- number of pieces required for a name.
 *
 */

/* EXPORT */
u_int dir_NamePieces (
	u_int len		/* length of the name */
	)
{
    return (len + (2*PIECESIZE-BASENAMESIZE-1)) >> LOGPIECESIZE;
}

/*
 * dir_Create -- Create an entry in a directory
 *
 * If called on behalf of mkdir, we do not call DHOP_WRITE.  Instead our
 * caller (dir_MakeDir) performs all the writing with one call to DHOP_WRITE.
 * This is an optimization.
 */

/* EXPORT */
int dir_Create (
	struct Dir_Handle *dir,		/* directory */
	char *name,			/* name of entry */
	epi_volFileId_t *fid,		/* file id of entry */
	long offset,			/* piece # at which to create entry */
	int mkdir,			/* called from mkdir */
	logTranRec_t tranId)		 /* transaction id */
{
    u_int pieces;			/* size of entry in pieces */
    register int h;			/* hash value of entry name */
    u_int len;				/* length of entry name */
    struct Dir_Entry *entP;		/* pointer to in-core entry */
    struct Dir_Entry *fragP;		/* remainder after entP has been cut
					 * from free area */
    struct Dir_Entry savedFrag;		/* in case of emergency ... */
    u_int entno;			/* piece # within page */
    struct Dir_Page *dpP;		/* pointer to in-core dir page */
    struct Dir_Header *dhP;		/* pointer to dir header */
    int err;				/* error code */
    int pgno;				/* page no. of entry */
    u_int next;				/* piece # for iteration on free list */
    int himark;				/* high-water mark on 1-page dir */
    struct Dir_Entry *freeP;		/* pointer to in-core free area */
    u_char *prevP;			/* pointer to free list chain to area */
    u_char *prevLenP;
    u_char oldHash;

    /* hash the name and find size of entry */
    h = dir_Hash(name, &len);
    if (len == 0) {
	return ENOENT;
    } else if (len > EFS_MAX_FILENAME_LENGTH) {
	return ENAMETOOLONG;
    }

    pieces = dir_NamePieces (len);

    /* grow directory if necessary */
    if (offset == 0) {

	/* Add a page */
	u_int len = DHOP_LENGTH (dir);
	if (len == 0) panic ("zero length directory");
	if (len < DIRPAGESIZE)
	    pgno = 1;
	else
	    pgno = len >> LOGDIRPAGESIZE;
	err = dir_AddPage (dir, tranId, pgno);
	if (err) {
	    dirb_AnnulPage (dir, pgno);
	    return err;
	}
	offset = (pgno << LEPP) + HDRPIECES;

	(void)dir_SetPageCount(dir, pgno + 1, tranId);
    } else
	pgno = offset >> LEPP;

    /* bring entry into memory */
    entP = dir_GetPiece (dir, offset, &dpP);
    entno = offset & (EPP-1);

    /* save the entry for possible later failure recovery */
    savedFrag = *entP;	/* structure assignment */

    /* find the entry on the free list */
    prevLenP = 0;
    for (next = dpP->udh.pghd.flist, prevP = &dpP->udh.pghd.flist;
	 next != entno;
	 next = freeP->next, prevP = &freeP->next) {
	freeP = (struct Dir_Entry *) dpP + next;
	if (entno > next && entno < next + freeP->length) {
	    /*
	     * We have found a free area that contains the pieces designated
	     * for the entry, but the entry will not be at the start of the
	     * free area.  This will not happen during normal directory
	     * operations, but will happen frequently during fileset restores.
	     */
	    if (freeP->length - (entno - next) < pieces) {
		err = EINVAL;
		goto out;
	    }
	    entP->next = freeP->next;
	    entP->length = freeP->length - (entno - next);
	    freeP->next = entno;
	    freeP->length = entno - next;
	    prevLenP = &freeP->length;
	    prevP = &freeP->next;
	    break;
	}
	afsl_Assert (freeP->next > next); /* no looping! */
    }

    /* Update piece count if necessary */
    himark = 0;
    if (pgno == 0 && DHOP_LENGTH (dir) <= DIRPAGESIZE) {
	dhP = (struct Dir_Header *) dpP;
	himark = entno + pieces - 1;
	if (entP->length > pieces) himark++;
	if (himark > (int) dhP->pnhimark) {
	    dhP->pnhimark = himark;
	    himark = -1;		/* remember we did it */
	}
    }

    /* modify free list */
    fragP = 0;
    if (entP->length == pieces)
	*prevP = entP->next;
    else {
	*prevP = entno + pieces;
	fragP = entP + pieces;
	fragP->dir_entry_magic = htonl(NON_ENTRY_MAGIC);
	fragP->length = entP->length - pieces;
	fragP->next = entP->next;
	fragP->inuse = 0;
	entP->length = pieces;
	pieces++;
    }

    /* fill in the entry */
    entP->fid = *fid;
    epid_ClearUnique (&entP->fid);
    strcpy((char *)entP->name, name);
    entP->dir_entry_magic = htonl(ENTRY_MAGIC);
    entP->inuse = 1;

    /* thread it on the hash table list. */
    oldHash = entP->next = dpP->hashTable[h];
    dpP->hashTable[h] = entno;

    /* write modifications to disk */
    if (!mkdir) {

	/* write entry and trailing fragment if any */
	err = DHOP_WRITE(dir,tranId,offset*PIECESIZE,entP,pieces*PIECESIZE);
	if (err) {
undoEntry:
	    dirb_AnnulPage (dir, pgno);
	    /*
	     * If the write was more than one piece, it may have crossed a
	     * fragment boundary.  If so, some bytes got written and some
	     * didn't.  In particular, the header (first piece) may have been
	     * written.  To fix this we "undo" the write by rewriting the old
	     * free area header info.
	     */
	    DHOP_WRITE(dir,tranId,offset*PIECESIZE,&savedFrag,sizeof(savedFrag));
	    goto out;
	}

	/* write hash table entry */
	err = DHOP_WRITE(dir,tranId,
			 pgno * DIRPAGESIZE + addrdif(&dpP->hashTable[h],dpP),
			 &dpP->hashTable[h],
			 sizeof(dpP->hashTable[h]));
	if (err) goto undoEntry;

	/* write free list mod */
	err = DHOP_WRITE(dir,tranId,
			 pgno * DIRPAGESIZE + addrdif(prevP,dpP),
			 prevP,sizeof(*prevP));
	if (err) {
	    /* undo hash table mod */
	    (void) DHOP_WRITE (dir,tranId,
			       pgno * DIRPAGESIZE +
				   addrdif(&dpP->hashTable[h],dpP),
			       &oldHash,
			       sizeof(oldHash));
	    goto undoEntry;
	}

	if (prevLenP)
	    (void) DHOP_WRITE(dir,tranId,
			      pgno * DIRPAGESIZE + addrdif(prevLenP,dpP),
			      prevLenP,sizeof(*prevLenP));

	/* write change to high-water mark */
	if (himark == -1)
	    (void) DHOP_WRITE(dir,tranId,
			      addrdif(&dhP->pnhimark,dhP),
			      &dhP->pnhimark,sizeof(dhP->pnhimark));
    }

out:
    dirb_Release((caddr_t)dpP);
    return err;
}

/*
 * dir_Delete -- delete an entry from a directory
 *
 */

/* EXPORT */
int dir_Delete (
	struct Dir_Handle *dir,		/* directory */
	long offset,			/* piece number of entry */
	logTranRec_t  tranId)		/* transaction id */
{
    struct Dir_Entry *entP;		/* pointer to in-core entry */
    struct Dir_Entry *prevP;		/* temp entry pointer */
    struct Dir_Page *dpP;		/* pointer to in-core dir page */
    u_char *chnP;			/* ptr to hash chain to entry */
    u_char oldChn;			/* old chain val for recovery */
    u_int len;				/* dummy parameter to dir_Hash */
    int err;
    int unhashed = 0;

    entP = dir_GetPiece (dir, offset, &dpP);
    if (!entP) return EIO;
    afsl_Assert (!BadEntry(entP));	/* specified offset better allocated */

    /* find hash chain to entry */
    chnP = &dpP->hashTable[dir_Hash((char *)entP->name, &len)];
    while (*chnP != (offset & (EPP-1))) {
	prevP = (struct Dir_Entry *) dpP + *chnP;
	if ((!*chnP) || BadEntry (prevP))
	    /* Could not find hash pointer to entry */
	    /* Recovery:  delete entry anyway */
	    goto nohash;
	chnP = &prevP->next;
    }

    oldChn = *chnP;
    unhashed = 1;
    *chnP = entP->next;
    err = DHOP_WRITE(dir,tranId,
		addrdif(chnP,dpP) + ((offset & ~(EPP-1)) << LOGPIECESIZE),
		chnP,
		sizeof(*chnP));
    if (err) goto out;
nohash:
    err = FreePieces(
	dir, tranId, (struct Dir_Page_Header *)dpP, offset >> LEPP, entP);
    if (err && unhashed)
	/* undo hash change */
	(void) DHOP_WRITE(dir,tranId,
		  addrdif(chnP,dpP) + ((offset & ~(EPP-1)) << LOGPIECESIZE),
		  &oldChn,
		  sizeof(oldChn));
out:
    if (err)
	dirb_AnnulPage (dir, offset >> LEPP);
    dirb_Release ((caddr_t)dpP);
    return err;
}

/*
 * dir_Replace -- change an entry
 */

/* EXPORT */
int dir_Replace (
	struct Dir_Handle *dir,		/* directory */
	long offset,			/* piece number of entry */
	epi_volFileId_t *fid,		/* file id to put in entry */
	logTranRec_t tranId)		/* transaction id */
{
    struct Dir_Entry *entP;		/* pointer to in-core entry */
    struct Dir_Page *dpP;		/* pointer to in-core dir page */
    int err;

    entP = dir_GetPiece (dir, offset, &dpP);
    if (!entP) return EIO;
    afsl_Assert (!BadEntry(entP));	/* specified offset better allocated */
    entP->fid = *fid;
    epid_ClearUnique (&entP->fid);
    err = DHOP_WRITE(dir,tranId,
		addrdif(&entP->fid,entP) + (offset << LOGPIECESIZE),
		&entP->fid,
		sizeof(entP->fid));
    if (err)
	dirb_AnnulPage (dir, offset >> LEPP);
    dirb_Release((caddr_t)dpP);
    return err;
}

/*
 * dir_FixDotDot -- change ``..'' entry
 */

/* EXPORT */
int dir_FixDotDot(
	struct Dir_Handle *dir,		/* directory */
	epi_volFileId_t *fid,		/* new parent */
	logTranRec_t tranId)		/* transaction id */
{
    struct Dir_Entry *entP;		/* pointer to in-core entry */
    long offset;			/* piece no. of entry */
    struct Dir_Page *dpP;		/* pointer to in-core dir page */
    int err;				/* error code from FindEntry */

    err = FindEntry(dir, "..", &dpP, &offset, &entP);
    if (err != 0)
	return err;

    entP->fid = *fid;
    epid_ClearUnique(&entP->fid);
    err = DHOP_WRITE(dir, tranId,
		offsetof(struct Dir_Entry, fid) + (offset << LOGPIECESIZE),
		&entP->fid, sizeof (entP->fid));
    if (err != 0)
	dirb_AnnulPage(dir, offset >> LEPP);
    dirb_Release((caddr_t)dpP);
    return err;
}

/*
 * dir_AddPage -- Add a page to a directory
 *
 */
/* SHARED */
dir_AddPage(
    struct Dir_Handle *dir,		/* directory */
    logTranRec_t tranId,		/* transaction ID */
    int pageno)				/* page number */
{
    register struct Dir_Page *dpP;	/* pointer to in-core dir page */
    int err;				/* error return code */

    if (pageno==0) panic("bogus call to addpage");
    dpP = (struct Dir_Page *) dirb_New(dir,pageno);
    dpP->udh.pghd.dir_page_magic = htonl(DIR_PAGE_MAGIC);
    dpP->udh.hd.pnhimark = 0;		/* unused on non-first pages */
    dpP->udh.hd.pgcount = 0;		/*  ditto  */
    ClearPage(dpP);
    err = DHOP_WRITE(dir,tranId,pageno*DIRPAGESIZE,dpP,DIRPAGESIZE);
    dirb_Release((caddr_t)dpP);
    return (err);
}

/*
 * ClearPage -- sets up a page to indicate all pieces are available.
 *
 */

static void ClearPage(struct Dir_Page *dpP)
{
    int firstno;			/* piece number of first free piece */
    struct Dir_Entry *entP;		/* pointer to first free piece */

    dpP->udh.pghd.flist = firstno = HDRPIECES;
    entP = (struct Dir_Entry *) dpP + firstno;
    entP->dir_entry_magic = htonl(NON_ENTRY_MAGIC);
    entP->next = 0;
    entP->length = EPP - firstno;
    entP->inuse = 0;
    bzero (dpP->DP_pad, sizeof(dpP->DP_pad));
    bzero ((caddr_t) dpP->hashTable, sizeof(dpP->hashTable));
}

/*
 * dir_SetPageCount -- Fix up page count in dir header
 */

/* SHARED */
dir_SetPageCount(
    struct Dir_Handle *dir,
    int newCount,
    logTranRec_t tranId)
{
    struct Dir_Header *dhP;		/* pointer to dir header */
    int code;

    dhP = (struct Dir_Header *) dirb_Read (dir, 0);
    if (!dhP) return EIO;
    dhP->pgcount = htons (newCount);
    code = DHOP_WRITE (dir, tranId, addrdif (&dhP->pgcount, dhP),
		       &dhP->pgcount, sizeof (&dhP->pgcount));
    dirb_Release((caddr_t)dhP);
    return (code);
}

/*
 * dir_FitsAtEnd -- Return true if the entry with the given piece number and
 *	name will fit in the last free block on the last page.
 */

/* SHARED */
int dir_FitsAtEnd(
    struct Dir_Handle *dir,
    int *piecenoP,
    char *name,
    int *fitsP)
{
    u_int nameLen = strlen(name);
    u_int pieces = dir_NamePieces (nameLen);
    struct Dir_Page *dpP;
    int code = 0;
    u_int next;
    struct Dir_Entry *freeP;		/* pointer to in-core free area */
    u_int entno;
    int lastPage = dir_LastPage(dir);

    *fitsP = 0;

    if (DIR_OFF_TO_PAGE(DIR_PIECE_TO_OFF(*piecenoP)) != lastPage) {
	*piecenoP = 0;
	return 0;
    }

    if (dir_GetPiece(dir, lastPage << LEPP, &dpP) == 0)
	return EIO;

    next = dpP->udh.pghd.flist;
    if (next == 0) {
	*piecenoP = 0;
	goto release_page;
    }

    freeP = (struct Dir_Entry *)dpP + next;

    while (freeP->next != 0) {
	next = freeP->next;
	freeP = (struct Dir_Entry *)dpP + next;
    }

    entno = *piecenoP & (EPP-1);
    *fitsP = (entno >= next &&
	      entno < next + freeP->length &&
	      freeP->length - (entno - next) >= pieces);

    if (*fitsP == 0)
	if (pieces <= freeP->length)
	    *piecenoP = (lastPage << LEPP) + next;
        else
	    *piecenoP = 0;

release_page:
    dirb_Release((caddr_t)dpP);
    return code;
}

/*
 * FreePieces -- Free the space used by a directory entry
 *
 * We keep the free list in order of increasing address, and we try to
 * coalesce the entry with the one before it and/or the one after it,
 * if they are contiguous.
 */
static int FreePieces(
    struct Dir_Handle *dir,		/* directory */
    logTranRec_t tranId,		/* transaction id */
    struct Dir_Page_Header *dpP,	/* pointer to in-core dir page */
    long pgno,				/* page number */
    struct Dir_Entry * entP)		/* pointer to in-core dir entry */
{
    u_int firstpiece;			/* number of first piece */
    int npieces;			/* number of pieces */
    u_char prev;			/* previous ent on free list */
    u_char succ;			/* next ent (successor) on free list */
    struct Dir_Entry * preventP;	/* pointer to in-core previous ent */
    struct Dir_Entry * succentP;	/* pointer to in-core successor ent */
    struct Dir_Entry * joinentP;	/* handy spare ent pointer */
    int joinprev;			/* will coalesce ent with previous */
    int joinsucc;			/* will coalesce ent with successor */
    int bf = 0;				/* bad (corrupted) free area found */
    struct Dir_Entry oldEnt;		/* for failure recovery */
    int err;				/* error return code */

    bcopy ((char *)entP, (char *)&oldEnt, NONNAMESIZE);

    firstpiece = addrdif(entP,dpP) >> LOGPIECESIZE;
    npieces = entP->length;
    entP->dir_entry_magic = htonl(NON_ENTRY_MAGIC);
    entP->inuse = 0;

    /* figure out where we will go on free list */
    prev = 0;
    succ = dpP->flist;
    while (succ && succ < firstpiece) {
	succentP = (struct Dir_Entry *) dpP + succ;
	if (BadFree (succentP)) {
	    /* Corrupted free list entry */
	    /* Recovery: ignore rest of free list */
	    bf = 1;
	    break;
	}
	prev = succ;
	preventP = succentP;
	succ = preventP->next;
    }
    /*
     * Possible cases:
     *	free list is empty:  loop goes through no iterations;
     *	    prev and succ set to 0.
     *  all areas on free list are < firstitem:
     *	    loop traverses entire list;
     *	    prev set to last area on list, succ set to 0.
     *  all areas on free list are > firstitem;
     *	    loop goes through no iterations;
     *	    prev set to 0, succ set to first area.
     *  firstitem will go in middle of free list:
     *	    prev set to predecessor, succ set to successor.
     */
    joinprev = (prev != 0) && (prev + preventP->length) == firstpiece;
    if (!bf) {
	joinsucc = (succ != 0) && (firstpiece + npieces) == succ;
	if (joinsucc) {
	    succentP = (struct Dir_Entry *) dpP + succ;
	    if (BadFree (succentP)) joinsucc = 0;
	}
    }

    /* fix up pointers */
    if (joinprev) {
	if (joinsucc)
	    preventP->next = succentP->next;
    } else {
	if (joinsucc)
	    entP->next = succentP->next;
	else
	    entP->next = succ;
	if (prev == 0)
	    dpP->flist = firstpiece;
	else
	    preventP->next = firstpiece;
    }

    /* fix up length */
    if (joinprev) {
	joinentP = preventP;
	joinentP->length += npieces;
    } else
	joinentP = entP;
    if (joinsucc) joinentP->length += succentP->length;

    err = DHOP_WRITE(dir,tranId,
		pgno * DIRPAGESIZE + firstpiece * PIECESIZE,
		entP, NONNAMESIZE);
    if (err) return err;
    if (prev == 0)
	err = DHOP_WRITE(dir,tranId,
		   pgno * DIRPAGESIZE + addrdif(&dpP->flist,dpP),
		   &dpP->flist,sizeof(dpP->flist));
    else
	err = DHOP_WRITE(dir,tranId,
		   pgno * DIRPAGESIZE + prev * PIECESIZE,
		   preventP, NONNAMESIZE);
    if (err)
	/* undo entry modification */
	(void) DHOP_WRITE(dir,tranId,
		    pgno * DIRPAGESIZE + firstpiece * PIECESIZE,
		    &oldEnt, NONNAMESIZE);
    return err;
}

/*
 * dir_MakeDir -- Initialize a directory
 *
 * We save all our disk writing for the end.  This avoids a lot of little
 * calls to DHOP_WRITE when adding the ``.'' and ``..'' entries.  We pass
 * a flag to dir_Create telling it not to do any calls to DHOP_WRITE.
 */

/* EXPORT */
int dir_MakeDir (
	struct Dir_Handle *dir,		/* directory */
	epi_volFileId_t *me,		/* use this for . */
	epi_volFileId_t *parent,	/* use this for .. */
	logTranRec_t tranId)		/* transaction ID */
{
    struct Dir_Page *dhP;		/* pointer to in-core dir page */
    int err;				/* error return code */

    dhP = (struct Dir_Page *) dirb_New(dir,0);
    dhP->udh.hd.dir_magic = htonl(DIR_MAGIC);
    dhP->udh.hd.pnhimark = HDRPIECES;	/* see comment in FindPieces */
    dhP->udh.hd.pgcount = htons(1);
    ClearPage(dhP);
    if (me) (void) dir_Create(dir,".",me,HDRPIECES,1,tranId);
    if (parent) (void) dir_Create(dir,"..",parent,HDRPIECES+1,1,tranId);
    err = DHOP_WRITE(dir,tranId,0,dhP,(HDRPIECES + 3) * PIECESIZE);
    dirb_Release((caddr_t)dhP);
    return (err);
}

/*
 * dir_Lookup -- look up an entry in a directory
 *
 * PARAMETERS:
 *   offset -- if lookup is successful, this is set to piece # of entry;
 *	       if lookup is unsuccessful (ENOENT), this is set to piece #
 *		 of free area where entry can be added; if there is no
 *		 room for entry, set to zero.
 *
 */

/* EXPORT */
int dir_Lookup (
	struct Dir_Handle *dir,		/* directory */
	char *name,			/* name of entry */
	epi_volFileId_t *fid,		/* file ID of result */
	long *offset)			/* piece number of result */
{
    struct Dir_Entry *entP;		/* pointer to in-core dir entry */
    struct Dir_Page *dpP;		/* pointer to in-core dir page */
    int err;				/* error code from FindEntry */

    err = FindEntry(dir,name,&dpP,offset,&entP);
    if (err) return err;
    *fid = entP->fid;
    dirb_Release((caddr_t)dpP);
    return 0;
}

/*
 * dir_IsEmpty -- Emptiness of directory.
 *
 * Returns 1 iff directory is empty except for "." and ".." entries,
 *   otherwise 0.  I/O errors cause return of 1.
 *
 */

/* EXPORT */
int dir_IsEmpty (struct Dir_Handle *dir)
{
    int pgcount;			/* total # of pages */
    int pgno;				/* page number */
    int eno;				/* piece number */
    struct Dir_Header *dhP;		/* pointer to in-core dir header */
    struct Dir_Page_Header *dpP;	/* ptr to in-core dir page header */
    struct Dir_Entry *entP;		/* pointer to in-core dir entry */

    dhP = (struct Dir_Header *) dirb_Read(dir,0);
    if (!dhP) return 1;
    pgcount = ntohs(dhP->pgcount);

    /* for each page, ... */
    for (pgno=0; pgno < pgcount; pgno++) {
	if (pgno == 0)
	    dpP = (struct Dir_Page_Header *) dhP;
	else {
	    dpP = (struct Dir_Page_Header *) dirb_Read(dir,pgno);
	    if (!dpP) return 1;
	}

	/* for each entry on the page (in address order), ... */
	for (eno = HDRPIECES, entP = (struct Dir_Entry *) dpP + eno;
	     eno < EPP;
	     eno += entP->length, entP += entP->length) {
	    if (BadArea (entP) || entP->length == 0) {
		dirb_Release((caddr_t)dpP);
		return 1;
	    }
	    if (!entP->inuse) continue;
	    if (!strcmp((char *)entP->name,".") || !strcmp((char *)entP->name,"..")) continue;
	    dirb_Release((caddr_t)dpP);
	    return 0;
	}
	dirb_Release((caddr_t)dpP);
    }
    return 1;
}

/*
 * dir_GetPiece -- Return a pointer to an entry, given its piece number
 *
 * A pointer to the buffer page is put in the place indicated by dpPP.  The
 * caller must eventually free this page by calling dirb_Release.
 *
 */

/* EXPORT */
struct Dir_Entry *dir_GetPiece (
	struct Dir_Handle *dir,		/* directory */
	long pieceno,			/* piece number */
	struct Dir_Page **dpPP		/* place to put ptr to dir buffer */
	)
{
    *dpPP = (struct Dir_Page *) dirb_Read(dir, (int)pieceno >> LEPP);
    if (!*dpPP) return 0;
    return (struct Dir_Entry *) *dpPP + (pieceno & (EPP - 1));
}

/*
 * dir_Hash -- Hash value of a name
 *
 */

/* EXPORT */
int dir_Hash(
	char *string,			/* name to be hashed */
	u_int *lenP)			/* place to put length */
{
    register u_char tc;			/* current character */
    register int hval;			/* accumulated value */
    register int tval;			/* handy temp */
    register u_int len;			/* length */

    len = 0;
    hval = 0;
    while (tc=(*string++)) {
        hval *= 173;			/* prime; = 0xad */
        hval  += tc;
	len++;
    }
    *lenP = len;
    tval = hval & (NHASHENT-1);

    /* if hval has gone negative, negate tval.  *
     * not sure if this is needed or even good. */
    if (tval == 0)
	return tval;
    else if (hval < 0)
	tval = NHASHENT-tval;
    return tval;
}

/*
 * FindEntry -- Find a directory entry, given its name
 *
 * A pointer to the buffer page is put in the place indicated by dpPP.  The
 * caller must eventually free this page by calling dirb_Release.
 *
 * If no entry is found, no buffers are locked, and a null pointer is
 * returned.
 *
 * The parameter offP corresponds to the parameter offset in dir_Lookup.
 *
 */

static int FindEntry(
    struct Dir_Handle *dir,		/* directory */
    char *ename,			/* name of entry */
    struct Dir_Page **dpPP,		/* ptr to ptr to in-core dir page */
    long *offP,				/* ptr to piece number */
    struct Dir_Entry **entPP)		/* ptr to ptr to found entry */
{
    int pgcount;			/* total # pages */
    register int pgno;			/* page number */
    register int hashval;		/* hash value of name */
    u_int len;				/* length of name */
    u_int pieces;			/* # pieces required for entry */
    long offset;			/* piece # of in-core dir entry */
    register struct Dir_Entry *entP;	/* pointer to in-core dir entry */
    struct Dir_Header *dhP;		/* pointer to in-core dir header page */
    register struct Dir_Page *dpP;	/* pointer to in-core dir page */
    int space = 0;			/* piece # of first good free area */

    hashval = dir_Hash (ename, &len);
    pieces = dir_NamePieces (len);
    dhP = (struct Dir_Header *) dirb_Read(dir,0);
    if (!dhP) return EIO;
    pgcount = ntohs(dhP->pgcount);

    for (pgno=0; pgno < pgcount; pgno++) {
	if (pgno == 0)
	    dpP = (struct Dir_Page *) dhP;
	else {
	    dpP = (struct Dir_Page *) dirb_Read(dir,pgno);
	    if (!dpP) return EIO;
	}

	/* Search for entry on this page */
	offset = dpP->hashTable[hashval];
	while (offset) {
	    entP = (struct Dir_Entry *) dpP + offset;
	    if (BadEntry (entP))
		/* Corrupted directory entry */
		/* Recovery: ignore entry */
		break;
	    if (!strcmp((char *)entP->name,ename)) {
		*dpPP = dpP;
		*offP = offset + (pgno << LEPP);
		*entPP = entP;
		return 0;
	    }
	    offset = entP->next;
	}

	/* Search for free space on this page */
	if (space == 0)
	    dir_FindFreeSpace(dpP, pgno, pieces, &space);

	/* Ready for next page */
	dirb_Release((caddr_t)dpP);
    }

    *offP = space;
    return ENOENT;
}

/*
 * dir_PieceScan -- Iterate entry within directory
 *
 * Parameter: directory; and piece number, not necessarily valid.
 *
 * Return value:  piece number of a valid directory entry, or 0 if there
 *                are none left or if an error is discovered (we only check for
 *                a few errors.
 *
 */

/* EXPORT */
int dir_PieceScan (
	struct Dir_Handle *dir,		/* directory */
	u_long pieceno)			/* piece number */
{
    u_int pgno;				/* page number */
    struct Dir_Page * dpP;		/* pointer to in-core dir page */
    u_long newpieceno;			/* new piece number */
    struct Dir_Entry * newpieceP;	/* piece indicated by new piece no. */
    u_int len;				/* length of dir in bytes */
    u_int pgmax;			/* page number of last page */

    len = DHOP_LENGTH (dir);
    pgmax = (len-1) >> LOGDIRPAGESIZE;
    pgno = pieceno >> LEPP;
    if (pgno > pgmax) return 0;

    pieceno &= (EPP-1);

    dpP = (struct Dir_Page *) dirb_Read(dir, (int)pgno);
    if (!dpP) return 0;

    /*
     * if pieceno looks like a legitimate entry or free area,
     * use it as the starting point of our search.  Otherwise,
     * scan forward from the beginning of the page to find a
     * legitimate entry or free area starting at or beyond pieceno.
     */
    newpieceno = pieceno;
    newpieceP = (struct Dir_Entry *) dpP + newpieceno;
    if (BadArea (newpieceP)) {
	newpieceno = HDRPIECES;
	while (newpieceno < pieceno) {
	    newpieceP = (struct Dir_Entry *) dpP + newpieceno;
	    newpieceno += newpieceP->length;
	    if (BadArea (newpieceP)
		|| newpieceP->length == 0
		|| newpieceno > EPP) {
		dirb_Release((caddr_t)dpP);
		return 0;
	    }
	}
    }

    /* find an entry, on this page or a subsequent one */
    do {
	while (newpieceno < EPP) {
	    newpieceP = (struct Dir_Entry *) dpP + newpieceno;
	    if (newpieceP->inuse) {
		dirb_Release((caddr_t)dpP);
		if (BadEntry(newpieceP)) return 0;
		return newpieceno + pgno * EPP;
	    }
	    newpieceno += newpieceP->length;
	    if (BadFree (newpieceP) || newpieceP->length == 0
		  || newpieceno > EPP) {
		dirb_Release((caddr_t)dpP);
		return 0;
	    }
	}
	dirb_Release((caddr_t)dpP);
	if (++pgno > pgmax) return 0;
	dpP = (struct Dir_Page *) dirb_Read(dir, (int)pgno);
	newpieceno = HDRPIECES;
    } while (dpP);

    return 0;
}

/* EXPORT */
void dir_FindFreeSpace(struct Dir_Page *dpP, int pgno,
			      u_int pieces, int *freenoP)
{
    int freeno;				/* piece # of current free area */
    struct Dir_Entry *freeP;		/* pointer to in-core free area */

    *freenoP = 0;

    for (freeno = dpP->udh.pghd.flist; freeno; freeno = freeP->next) {
	freeP = (struct Dir_Entry *) dpP + freeno;
	if (BadFree (freeP))
	    break;
	if (freeP->length >= pieces) {
	    *freenoP = freeno + (pgno << LEPP);
	    break;
	}
    }
}
