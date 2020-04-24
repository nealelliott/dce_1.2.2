/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dir_salvage.c,v $
 * Revision 1.1.92.1  1996/10/02  17:20:32  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:40  damon]
 *
 * $EndLog$
*/
/*
 * Copyright (C) 1994, 1990 Transarc Corporation
 * All rights reserved.
 */

/*                              DEcorum File System
                                 Directory Package
                           Directory Salvager Subpackage                  */

/* The directory salvager consists of two routines.  The first, dirs_OK, checks
   to see if the directory looks good.  If the directory does NOT look good,
   the approved procedure is to then call dirs_Salvage, which copies all the
   good entries from the damaged dir into a new directory. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/episode/logbuf.h>	/* elbt_TranRec_t */
#include <dcedfs/episode/fid.h>		/* epi_volFileId_t */
#include <dir_buffer.h>
#include <dir.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/dir/dir_salvage.c,v 1.1.92.1 1996/10/02 17:20:32 damon Exp $")

/* Define static check functions */

PRIVATE int CheckHeader(
    struct Dir_Header *,
    void (*Log)(),			/* function to pass err msgs to */
    u_int );

PRIVATE int CheckPageHeader(
    struct Dir_Page *,
    int,
    void (*Log)() );                     /* function to pass err msgs to */

PRIVATE int CheckFreeArea(
    u_int maxentpiece,			/* last valid piece in buffer */
    struct Dir_Entry * ep,		/* pointer to in-core free area */
    u_char free,			/* piece number of free area */
    int pgno,				/* page number */
    void (*Log)());			/* function to pass err msgs to */

PRIVATE int CheckFreePieces(
    u_char freep[],			/* map of free pieces on page */
    u_char nfree,			/* piece number */
    u_char len,				/* length of free area */
    u_char next,			/* number of next piece on free list */
    int pgno,				/* page number */
    void (*Log)());			/* function to pass err msgs to */

PRIVATE int CheckEntry(
    u_int maxentpiece,			/* last valid piece in buffer */
    struct Dir_Entry *ep,		/* pointer to in-core dir entry */
    u_char entno,			/* piece number of entry */
    int pgno,				/* page number */
    void (*Log)());			/* function to pass err msgs to */

PRIVATE int CheckEntryPieces(
    u_char inusep[],			/* map of in-use pieces on page */
    u_char entno,			/* piece number */
    u_char len,				/* length of free area */
    int pgno,				/* page number */
    void (*Log)());			/* function to pass err msgs to */

PRIVATE int CheckEntryHash(
    struct Dir_Entry *ep,		/* pointer to in-core dir entry */
    u_char entno,			/* piece number of entry */
    u_char hashval,			/* number of hash chain */
    int pgno,				/* page number */
    void (*Log)());			/* function to pass err msgs to */

/* String area for error messages */

static
char errM[EFS_MAX_FILENAME_LENGTH+100];

/*
 * dirs_OK -- Check for damage to a directory
 */

EXPORT int dirs_OK (dir, Log)
  IN struct Dir_Handle *dir;		/* directory */
  IN void (*Log)();			/* function to pass err msgs to */
{
    int pgcount;			/* total # pages in directory */
    u_int maxentpiece;			/* last valid piece in buffer */
    int pgno;				/* page number */
    u_int pieceno;			/* piece number */
    u_char hashval;			/* hash value */
    struct Dir_Header *dhp;		/* pointer to in-core dir header */
    struct Dir_Page *pp;		/* pointer to in-core dir page */
    struct Dir_Entry *ep;		/* pointer to in-core dir entry */
    u_char freep[EPP];			/* map of free pieces */
    u_char inusep[EPP];			/* map of pieces in entries */
    int foundDot, foundDotDot;		/* found these entries */

    /* Check validity of dir header and get size of dir */

    dhp = (struct Dir_Header *) dirb_Read (dir, 0);

    if (!CheckHeader(dhp, Log, DHOP_LENGTH (dir))) {
	dirb_Release((caddr_t)dhp);
	return 0;
    }

    pgcount = ntohs(dhp->pgcount);
    if (pgcount == 1)
	maxentpiece = dhp->pnhimark + 1;
    else
	maxentpiece = EPP;
    dirb_Release((caddr_t)dhp);

    foundDot = foundDotDot = 0;		/* make sure we have these correct */

    /* Check for various problems in each page */

    for (pgno=0; pgno<pgcount; pgno++) {
        pp = (struct Dir_Page *) dirb_Read(dir, pgno);

	/* Check page header */

	if (!CheckPageHeader(pp,pgno,Log)) {
	    dirb_Release((caddr_t)pp);
	    return 0;
	}

	/* Check free list */

	for (pieceno=0; pieceno < EPP; pieceno++) freep[pieceno] = 0;

	pieceno = pp->udh.pghd.flist;
        while (pieceno) {
	    ep = (struct Dir_Entry *) pp + pieceno;
	    if (!CheckFreeArea (maxentpiece,ep,pieceno,pgno,Log) ||
		!CheckFreePieces (freep, pieceno, ep->length, ep->next,
				  pgno, Log)) {
		dirb_Release((caddr_t)pp);
		return 0;
	    }
	    pieceno = ep->next;
	}

	/* Check all hash chains */

	for (pieceno=0; pieceno < EPP; pieceno++) inusep[pieceno] = 0;

	for (hashval=0; hashval<NHASHENT; hashval++) {
	    pieceno = pp->hashTable[hashval];
	    while (pieceno) {
		ep = (struct Dir_Entry *) pp + pieceno;
		if (!CheckEntry (maxentpiece,ep,pieceno,pgno,Log) ||
		    !CheckEntryPieces (inusep,pieceno,ep->length, pgno,Log) ||
		    !CheckEntryHash (ep,pieceno,hashval,pgno,Log)) {
		    dirb_Release((caddr_t)pp);
		    return 0;
		}
		if (ep->name[0] == '.') {
		    if (ep->name[1] == 0) foundDot++;
		    else if (ep->name[1] == '.')
			if (ep->name[2] == 0) foundDotDot++;
		}
		pieceno = ep->next;
	    }
	}

	/* Check consistency of freep and inusep */

	for (pieceno = HDRPIECES; pieceno < EPP; pieceno++)
	    if (freep[pieceno] == inusep[pieceno]) {
		if (Log) {
		    sprintf(errM,
"page %d, free map and entry map inconsistent for piece %d.\n",
			    pgno, pieceno);
		    (*Log)(errM);
		}
		dirb_Release((caddr_t)pp);
		return 0;
	    }

        dirb_Release((caddr_t)pp);
    }
    if ((foundDot != 1) || (foundDotDot != 1)) {
	if (Log) {
	    sprintf(errM, "directory had %d \".\" and %d \"..\" entries\n",
		    foundDot, foundDotDot);
	    (*Log)(errM);
	}
	return 0;
    }
    return 1;
}

/*
 * CheckHeader -- Check for problems in directory header
 *
 * If there is something wrong with directory header,
 *   log an appropriate message and return 0;
 * else
 *   return 1.
 *
 */

PRIVATE int CheckHeader(
	struct Dir_Header *dhp,
	void (*Log)(),
	u_int size)
{
    char *msg;				/* pointer to error message */

    msg = 0;
    if (!dhp)
	msg = "could not read first page in directory.\n";
    else if (dhp->dir_magic != htonl(DIR_MAGIC))
	msg = "bad magic number in directory header.\n";
    else if (dhp->pgcount == 0)
	msg = "zero page count.\n";
    else if (size & PIECEMASK)
	msg = "file size not a whole number of pieces.\n";
    else if (dhp->pgcount == htons(1)) {
	if (size < (dhp->pnhimark + 1) * PIECESIZE)
	    msg = "high-water mark past end of file.\n";
	else if (size > (dhp->pnhimark + 1) * PIECESIZE)
	    msg = "end of file past high-water mark.\n";
	else if (dhp->pnhimark < HDRPIECES)
	    msg = "high-water mark is less than minimum.\n";
	else if (size > DIRPAGESIZE)
	    msg = "file size implies page count greater than 1.\n";
    } else {
	if (size & (DIRPAGESIZE-1))
	    msg = "file size not a multiple of directory page size.\n";
	if (dhp->pgcount != htons(size >> LOGDIRPAGESIZE))
	    msg = "page count and file size are inconsistent.\n";
    }
    if (!msg) return 1;
    if (Log) (*Log)(msg);
    return 0;
}

/*
 * CheckPageHeader -- Check for problems in directory page header
 *
 * If there is something wrong with directory page header,
 *   log an appropriate message and return 0;
 * else
 *   return 1.
 *
 */

PRIVATE int CheckPageHeader(
	struct Dir_Page *pp,
	int pgno,
	void (*Log)())
{
    u_char hashval;			/* index to hash table */

    if (!pp) {
	if (Log) sprintf(errM, "could not read page %d of directory.\n", pgno);
    } else if (pgno > 0 &&
	       pp->udh.pghd.dir_page_magic != htonl(DIR_PAGE_MAGIC)) {
	if (Log)
	    sprintf(errM, "bad magic number in header of directory page %d.\n",
		    pgno);
    } else if (pp->udh.pghd.flist && pp->udh.pghd.flist < HDRPIECES) {
	if (Log) sprintf(errM, "invalid free list header on page %d.\n", pgno);
    } else {
	for (hashval=0; hashval < NHASHENT; hashval++)
	    if (pp->hashTable[hashval] && pp->hashTable[hashval] < HDRPIECES)
		break;
	if (hashval < NHASHENT) {
	    if (Log)
		sprintf(errM, "page %d, hash table entry %d is invalid.\n",
			pgno, hashval);
	} else
	    return 1;
    }
    if (Log) (*Log)(errM);
    return 0;
}

/*
 * CheckFreeArea -- Check validity of various fields in a free area
 *
 * We also check if the free area is beyond maxentpiece.
 *
 */

PRIVATE int CheckFreeArea(
	u_int maxentpiece,
	struct Dir_Entry * ep,
	u_char nfree,
	int pgno,
	void (*Log)() )
{
    char *msg;				/* error message template */

    if (nfree >= maxentpiece) {
	if (Log) {
	    sprintf(errM, "free area number is %d, maxentpiece is only %d.\n",
		    nfree, maxentpiece);
	    (*Log)(errM);
	}
	return 0;
    }

    if (ep->dir_entry_magic != htonl(NON_ENTRY_MAGIC))
	msg = "page %d, bad magic number in free area %d.\n";
    else if (ep->next && ep->next < HDRPIECES)
	msg = "page %d, free area %d has invalid next ptr.\n";
    else if ((ep->length == 0)
	     || ((ep->length > maxentpiece - nfree)
		 && (ep->length != EPP - nfree)))
	msg = "page %d, free area %d has invalid length field.\n";
    else if (ep->inuse)
	msg = "page %d, piece %d on free list but inuse field set.\n";
    else
	return 1;
    if (Log) {
	sprintf(errM, msg, pgno, nfree);
	(*Log)(errM);
    }
    return 0;
}

/*
 * CheckFreePieces -- Check validity of a free area w.r.t. other areas on
 *		      the free list.
 *
 */

PRIVATE int CheckFreePieces(
	u_char *freep,
	u_char nfree,
	u_char len,
	u_char next,
	int pgno,
	void(*Log)())
{
    u_int i;				/* handy for-loop control */

    /* Check for overlapping free areas */
    for (i=0; i<len; i++) {
	if (freep[i+nfree]) {
	    if (Log) {
		sprintf(errM, "page %d: piece %d in two free areas.\n",
			pgno, i+nfree);
		(*Log)(errM);
	    }
	    return 0;
	}
	freep[i+nfree] = 1;
    }

    /* Check for non-coalesced free areas */
    if (next && nfree+len == next) {
	if (Log) {
	    sprintf(errM, "page %d: free areas %d and %d not coalesced.\n",
		    pgno, nfree, next);
	    (*Log)(errM);
	}
	return 0;
    }

    /* Check for free areas out of order */
    if (next && nfree > next) {
	if (Log) {
	    sprintf(errM, "page %d: area %d before %d on free list.\n",
		    pgno, nfree, next);
	    (*Log)(errM);
	}
	return 0;
    }

    return 1;
}

/*
 * CheckEntry -- Check validity of various fields in a directory entry
 *
 */

PRIVATE int CheckEntry(
	u_int maxentpiece,
	struct Dir_Entry *ep,
	u_char entno,
	int pgno,
	void (*Log)())
{
    char *msg;				/* error message template */

    if (ep->dir_entry_magic != htonl(ENTRY_MAGIC))
	msg = "page %d, bad magic number in entry at piece %d.\n";
    else if (ep->next && ep->next < HDRPIECES)
	msg = "page %d, entry at piece %d has invalid next ptr.\n";
    else if (ep->length == 0 || ep->length > maxentpiece - entno)
	msg = "page %d, entry at piece %d has invalid length field.\n";
    else if (!ep->inuse)
	msg = "page %d, entry at piece %d on hash chain but inuse field clear.\n";
    else if (ep->name[0] == '\0')
	msg = "page %d, entry at piece %d has zero length name.\n";
    else if (strlen((char *)(ep->name)) > (size_t)EFS_MAX_FILENAME_LENGTH
		|| dir_NamePieces(strlen((char *)ep->name)) != ep->length)
	msg = "page %d, name in entry at piece %d is too long or too short.\n";
    else
	return 1;
    if (Log) {
	sprintf(errM, msg, pgno, entno);
	(*Log)(errM);
    }
    return 0;
}

/*
 * CheckEntryPieces -- Check validity of a directory entry w.r.t. other entries.
 *
 */

PRIVATE int CheckEntryPieces(
	u_char *inusep,
	u_char entno,
	u_char len,
	int pgno,
	void(*Log)())
{
    u_int i;				/* handy for-loop control */

    /* Check for overlapping entries */
    for (i=0; i<len; i++) {
	if (inusep[i+entno]) {
	    if (Log) {
		sprintf(errM, "page %d: piece %d in two directory entries.\n",
			pgno, i+entno);
		(*Log)(errM);
	    }
	    return 0;
	}
	inusep[i+entno] = 1;
    }

    return 1;
}

/*
 * CheckEntryHash -- Check whether an entry is on the correct hash chain
 *
 */

PRIVATE int CheckEntryHash(
	struct Dir_Entry *ep,
	u_char entno,
	u_char hashval,
	int pgno,
	void (*Log)())
{
    int h;				/* hash value of name */
    u_int len;				/* dummy parameter for dir_Hash */

    h = dir_Hash ((char *)ep->name, &len);
    if (h != hashval) {
	if (Log) {
	    sprintf(errM,
		    "page %d, entry %d has hash value %d, should be %d.\n",
		    pgno, entno, h, hashval);
	    (*Log)(errM);
	}
	return 0;
    } else
	return 1;
}

/*
 * Macro for use with DHOPS_WRITE
 * find the difference (in bytes) between to addresses.
 */

#define addrdif(a,b) ((char *) (a) - (char *) (b))

/*
 * dirs_Salvage -- Salvage a damaged directory
 *
 * We find as many valid-looking entries in the old (damaged) directory as we
 * can, and copy them into the new directory.
 *
 * In finding valid entries, we must in general assume that no pointers in the
 * directory are valid.  Thus we ignore the hash chains and the free list, and
 * search linearly through each page.
 *
 * Also, if there is anything wrong with an entry (e.g. inuse bit not
 * consistent with magic number), we assume it is totally worthless, including
 * the length field.  We ignore the length field and resume our search for
 * valid entries at the very next piece.
 *
 * Elsewhere (see note in dir_PieceScan), we assume that a failure return from
 * dirb_Read indicates the end of the directory.  Here, however, we do not make
 * this assumption, except that beyond the containers length the read will
 * surely fail.  If a failure occurs on one page, we keep on trying more pages
 * up to the page count given in the directory header or the directory
 * container's size, which ever is less.
 *
 * RETURN CODES -- There are several special codes returned to indicate how the
 *     salvage went.  A code of zero means everything was okay.  Other bits are
 *     ORed together to indicate the state of the new directory as follows:
 *         bit 1: missing "."
 *         bit 2: missing ".."
 *         bit 4: bad problems, should probably punt and remove entry entirely.
 *         bit 8: I/O error unknown state. */

EXPORT int dirs_Salvage (fromFile, toFile, Log)
  IN struct Dir_Handle *fromFile;	/* old (damaged) directory */
  IN struct Dir_Handle *toFile;		/* new directory */
  IN void (*Log)();			/* function to pass err msgs to */
{
    int code;				/* temp for results of calls to
					   write to new directory */
    u_long size;			/* size of fromFile */
    u_long toSize;			/* size of toFile */
    int nPieces;			/* number of valid pieces in buffer */
    int sizePgcount;			/* page count calc. from file size */
    int pgcount;			/* page count */
    int pgno;				/* page number */
    int foundDot, foundDotDot;		/* found these entries */
    struct Dir_Header *dhp;		/* pointer to from dir header */
    struct Dir_Header *tdhp;		/* pointer to to dir header */

    /* Initialize new directory */
    dir_MakeDir (toFile, 0, 0, (elbt_TranRec_t) 0); /* Returns no error code */

    /* Figure out how many pages are in the directory.  We assume the pgcount
     * field is correct, unless it is invalid, in which case we give up. */

    dhp = (struct Dir_Header *) dirb_Read(fromFile, 0);
    if (!dhp) {
        if (Log) (*Log)("could not read first page in directory.\n");
        return DIRS_EXIT_NODOT|DIRS_EXIT_NODOTDOT;
    }

    pgcount = ntohs(dhp->pgcount);

    /* Calculate the pgcount indicated by the container's size.  If this is
     * smaller than the directory header's pgcount then use it instead. */
    size = DHOP_LENGTH(fromFile);
    toSize = DHOP_LENGTH(toFile);
    sizePgcount = (MAX(size,toSize) + (DIRPAGESIZE-1)) >> LOGDIRPAGESIZE;
    if (sizePgcount >= (1<<16)) {
	if (Log) {
	    sprintf(errM,
"file size %lu too large for 16-bit pgcount field, using page count of %d.\n",
		    size, pgcount);
	    (*Log)(errM);
	}
	pgcount = pgcount;		/* just use existing value */
    }
    else if (pgcount > sizePgcount) {
	if (Log) {
	    sprintf(errM,
"page count %d too large for file size %lu, using pgcount of %d instead.\n",
		    pgcount, size, sizePgcount);
	    (*Log)(errM);
	}
	pgcount = sizePgcount;
    }
    else if (pgcount < sizePgcount) {
	if (Log) {
	    sprintf(errM,
"page count %d too small for file size %lu, using %d from file size instead.\n",
		    pgcount, size, sizePgcount);
	    (*Log)(errM);
	}
	pgcount = sizePgcount;
    }

    if (pgcount == 0) {
	if (Log) (*Log)("zero page count.\n");
	dirb_Release((caddr_t)dhp);
        return DIRS_EXIT_NODOT|DIRS_EXIT_NODOTDOT;
    }

    if (pgcount == 1) {

	/* pnhimark may be damaged so use the container's length.  We use
         * nPieces to control the loop over all pieces in the fromDir.  Since
         * we start that loop at HDRPIECES don't set it below that value. */

	nPieces = MIN(size,DIRPAGESIZE) / PIECESIZE;
	nPieces = MAX(nPieces, HDRPIECES);
    } else
	nPieces = EPP;

    dirb_Release((caddr_t)dhp);

    foundDot = foundDotDot = 0;		/* make sure we have these correct */

    if (pgcount != 1) {

	/* MakeDir sets pgcount to 1.  But we must set pgcount of toFile to
         * match pgcount we are using.  Note that below we always make sure to
         * write a full page at pgno == pgcount-1 so that length is set
         * correctly. */

	tdhp = (struct Dir_Header *) dirb_Read(toFile,0);
	if (!tdhp) {
	    if (Log) (*Log)("could not read first page toFile directory.\n");
	    return DIRS_EXIT_NODOT|DIRS_EXIT_NODOTDOT;
	}
	tdhp->pgcount = htons(pgcount);
       code = DHOP_WRITE (toFile, elbt_NullTranId,
                          addrdif(&tdhp->pgcount,tdhp),
                          &tdhp->pgcount, sizeof(tdhp->pgcount));
	dirb_Release((caddr_t)tdhp);
	if (code) goto bail;
    }

    /* Enumerate all pages */
    code = 0;
    for (pgno=0; pgno < pgcount; pgno++) {
	u_long pgOff = pgno * DIRPAGESIZE;
	u_char *freep;
	int endfree;
	struct Dir_Entry * lastfree;
	struct Dir_Page_Header *pp;	/* pointer to from dir page hdr */
	struct Dir_Page_Header *tpp;	/* pointer to to dir page hdr */
	u_int pieceno;			/* piece number */
	int pieces;			/* amount to bump pieceno by */
	int himark;			/* last directory data */

	pp = (struct Dir_Page_Header *) dirb_Read(fromFile,pgno);
	if (!pp) {
	    if (Log) {
		sprintf(errM, "I/O error reading page %d.\n", pgno);
		(*Log)(errM);
	    }
	    continue;
	}

	if (pgno != 0) {		/* for every new page... */

	    /* Fast check for sparse directories: empty and interior pages.
             * The first and last pages must really exist. */

	    if (pgno != pgcount-1) {
		int empty;

		empty = (pp->dir_page_magic == 0);
		if (empty) for (pieceno=HDRPIECES; pieceno<EPP; pieceno++) {
		    struct Dir_Entry *ep;
		    ep = (struct Dir_Entry *) pp + pieceno;
		    if (ep->dir_entry_magic) {
			empty = 0;
			break;
		    }
		}
		if (empty) {

		    /* This page is completely empty don't even create the
		     * corresponding page in the to dir. */

		    dirb_Release((caddr_t)pp);
		    continue;
		}
	    }

	    code = dir_AddPage( toFile, elbt_NullTranId, pgno );
	    afsl_MBZ (code);	/* do the code XXX */
	    /* Note: we bump pgcount at the beginning because we can't go back
             * to page 0; we must traverse the pages in order. */
	}
	/* else: first page is pre-created */

	tpp = (struct Dir_Page_Header *) dirb_Read(toFile,pgno);
	freep = &tpp->flist;		/* for building new free list */
	endfree = 0;			/* never set yet */

	/* Is the following still true? -ota 930311 */
	/*
	** It is very important that we walk the pages only
	** once, in increasing order.  That's because we are
	** REWRITING the contents of the toFile, and the
	** salvager (the primary user of this routine) is
	** going to point that target at the same anode
	** as the source.  This all will work when you lift
	** what you need out of the anode page-wise before
	** you re-write any data back...
	*
	* The whole directory buffer done in salvage/salvage_dhops.c is
	* important to make this work.
        */

	/* Enumerate all pieces within page.  Then go around the loop one more
         * time to insure that all remaining pieces past nPieces are in the
         * free list of the new directory. */

	for (pieceno=HDRPIECES; pieceno<nPieces+1; pieceno+=pieces) {
	    struct Dir_Entry *ep;	/* from dir entry */
	    struct Dir_Entry *tep;	/* to dir entry */
	    int allocated;		/* is current entry allocated */

	    if (pieceno < nPieces)
		ep = (struct Dir_Entry *) pp + pieceno;
	    else ep = 0;		/* last time around */
	    tep = (struct Dir_Entry *) tpp + pieceno;

	    if (ep) {
		allocated = CheckEntry(nPieces,ep,pieceno,pgno,/*noop*/0);
		if (allocated) {
		    epi_volFileId_t fid; /* dummy fid for dir_lookup */
		    long off;		/* dummy offset for dir_Lookup */

		    pieces = ep->length;

		    code = dir_Lookup (toFile, (char *)ep->name, &fid, &off);
		    if (code == 0) {
			if (Log) {
			    sprintf(errM, "duplicate entries with name %s.\n",
				    ep->name);
			    (*Log)(errM);
			}
			allocated = 0;	/* add this slot to the free list */
		    } else {
			afsl_PAssert (code == ENOENT,
				      ("Unexpected code (%d) from dir_Lookup",
				       code));
		    }

		} else if (CheckFreeArea(nPieces,ep,pieceno,pgno,/*noop*/0))
		    pieces = ep->length;
		else pieces = 1;
	    } else {
		allocated = 0;
		pieces = EPP - pieceno;
		if (pieces == 0)
		    break;
	    }

	    if (allocated) {
		int found;
		int entno;
		u_char next;
		u_char *prevP;

		/* copy the old entry */
		bcopy ((caddr_t) ep, (caddr_t) tep, ep->length * PIECESIZE);
		/*
		 * If we are on the first page, we need to update the max piece
		 * count.
                 */
		if ((pgno == 0) && (pgcount == 1)) {
		    struct Dir_Header * tdhP;
		    tdhP = (struct Dir_Header *) tpp;
		    himark = pieceno + ep->length - 1;
		    himark++;	/* last entry will be free entry */
		    tdhP->pnhimark = himark;
		    code = DHOP_WRITE (toFile, elbt_NullTranId,
				       addrdif(&tdhP->pnhimark,tdhP),
				       &tdhP->pnhimark,
				       sizeof(tdhP->pnhimark));
		    if (code) break;
		}
		/*
		 * Fix up the hash table list, including new entry next ptr.
		 */
		/* BEGIN */{
		    struct Dir_Page *tdpP;
		    int i;		/* hash value of name */
		    u_int len;		/* dummy parameter to dir_Hash */

		    tdpP = (struct Dir_Page *)tpp;
		    i = dir_Hash((char *)ep->name, &len);
		    tep->next = tdpP->hashTable[i];
		    tdpP->hashTable[i] = pieceno;
		    code = DHOP_WRITE (toFile, elbt_NullTranId,
				       (pgOff +
					addrdif(&tdpP->hashTable[i],tdpP)),
				       &tdpP->hashTable[i],
				       sizeof(tdpP->hashTable[i]));
		    if (code) break;
		}
		/*
		 ** Now write back the new entry
		 */
		code = DHOP_WRITE (toFile, elbt_NullTranId,
				   pgOff + addrdif(tep,tpp),
				   tep, ep->length * PIECESIZE);
		if (code) break;
		himark = pieceno + ep->length - 1;

		if (ep->name[0] == '.') {
		    if (ep->name[1] == 0) foundDot++;
		    else if (ep->name[1] == '.')
			if (ep->name[2] == 0) foundDotDot++;
		}

	    } else {

		/* This will be a free piece.  If old dir had a free piece use
                 * its length, otherwise just add the current piece to the free
                 * list. */

		if (endfree == pieceno) {
		    /* Join contiguous free areas */
		    lastfree->length += pieces;
		    code = DHOP_WRITE (toFile, elbt_NullTranId,
				       (pgOff +
					addrdif (&lastfree->length, tpp)),
				       &lastfree->length, 1);
		    if (code) break;
		} else {
		    /* Put at end of free list */
		    *freep = pieceno;
		    code = DHOP_WRITE (toFile, elbt_NullTranId,
				       pgOff + addrdif (freep, tpp),
				       freep, 1);
		    if (code) break;
		    freep = &tep->next;
		    tep->dir_entry_magic = htonl(NON_ENTRY_MAGIC);
		    tep->next = 0;
		    tep->inuse = 0;
		    tep->length = pieces;
		    /* Write free piece */
		    code = DHOP_WRITE (toFile, elbt_NullTranId,
				       pgOff + addrdif (tep, tpp),
				       tep, PIECESIZE);
		    if (code) break;
		    himark = pieceno;
		    lastfree = tep;
		}
		endfree = pieceno + pieces;
	    }
	}
	if ((code == 0) && (endfree == 0)) {

	    /* There are no free entries on this page, so make sure we set the
             * flist to zero (it defaults to HDRPIECES). */

	    *freep = 0;
	    code = DHOP_WRITE (toFile, elbt_NullTranId,
			       pgOff + addrdif (freep, tpp), freep, 1);
	}

	if ((code == 0) && (pgno == 0) && (pgcount == 1)) {

	    /* If we are on the first page, we need to update the max piece
             * count.  On exit from the above loop, pieceno would be the offset
             * of the first piece not in the new directory.  Use this to set
             * pnhimark. */

	    struct Dir_Header * tdhP;
	    tdhP = (struct Dir_Header *) tpp;
	    tdhP->pnhimark = himark;
	    code = DHOP_WRITE (toFile, elbt_NullTranId,
			       addrdif(&tdhP->pnhimark,tdhP),
			       &tdhP->pnhimark,
			       sizeof(tdhP->pnhimark));
	}

	dirb_Release((caddr_t)pp);
	dirb_Release((caddr_t)tpp);
    }

bail:
    if (code) {
	afsl_Trace (AFSL_TR_ERRMAP, ("dirs_Salvage: I/O error code %u", code));
	code = DIRS_EXIT_IO;
    }
    if ((foundDot != 1) || (foundDotDot != 1)) {
	if (Log) {
	    sprintf(errM, "Directory had %d \".\" and %d \"..\" entries\n",
		    foundDot, foundDotDot);
	    (*Log)(errM);
	}
	afsl_Assert ((foundDot <= 1) && (foundDotDot <= 1));
	if (foundDot == 0) code |= DIRS_EXIT_NODOT;
	if (foundDotDot == 0) code |= DIRS_EXIT_NODOTDOT;
    }
    return code;
}
