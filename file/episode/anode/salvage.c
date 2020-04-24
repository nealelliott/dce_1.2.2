/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: salvage.c,v $
 * Revision 1.1.98.1  1996/10/02  17:18:55  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:28:47  damon]
 *
 * $EndLog$
 */
/*
 * salvage.c -- procedures to manage information the salvager needs.
 */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/debug.h>
#include <dcedfs/osi.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#undef File				/* oops.  defined in common_data.h;
					 * redefined in this file. */

#ifdef KERNEL
#error This program isn't used in the kernel.
#endif /* KERNEL */

#define EPIZ_E_INITIALIZER 1		/* get epiz_errorData */

#include "anode.h"
#include "fixed_anode.h"
#include "salvage.h"
#include "verify.h"

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/salvage.c,v 1.1.98.1 1996/10/02 17:18:55 damon Exp $")

/* List of all problem anodes providing associated decoration. */
static struct epiz_anode *anodes;

/* epiz_GetAnode -- returns an anode descriptor for the specified index pair,
 *     allocating it if necessary.  It never returns zero (NULL). */

/* SHARED */
struct epiz_anode * epiz_GetAnode(
  IN u_long anodeIx,
  IN u_long volIx)
{
    struct epiz_anode *l;
    afsl_Assert (anodeIx && volIx);

    for (l=anodes; l; l=l->next)
	if ((l->anodeIx == anodeIx) && (l->volIx == volIx)) return l;
    l = osi_Alloc_r (sizeof(*l));
    bzero ((char *)l, sizeof(*l));
    l->next = anodes;
    anodes = l;
    l->anodeIx = anodeIx;
    l->volIx = volIx;
    return l;
}

/* epiz_FindAnode -- returns an anode descriptor for the specified index pair,
 *     if it exists.  It returns zero (NULL) otherwise. */

/* SHARED */
struct epiz_anode * epiz_FindAnode(
  IN u_long anodeIx,
  IN u_long volIx)
{
    struct epiz_anode *l;
    afsl_Assert (anodeIx && volIx);

    for (l=anodes; l; l=l->next)
	if ((l->anodeIx == anodeIx) && (l->volIx == volIx)) return l;
    return 0;
}

/*
** epiz_FindAnodeItem
** This find routine looks for matches based on
** <volIx,anodeIx> in the provided list header.
*/
/* SHARED */
struct epiz_anodeItem * epiz_FindAnodeItem(
  IN struct epiz_anodeItem * list,
  IN u_long anodeIx,
  IN u_long volIx)
{
    struct epiz_anodeItem * dP;
    struct epiz_anode * a;

    a = epiz_FindAnode (anodeIx, volIx);
    if (!a) return 0;
    for (dP = list; dP; dP = dP->next)
	if (dP->anode == a) return dP;
    return 0;
}

/*
 * NewAnodeItem -- always allocates a new anode item and threads it on the list
 *     (optionally) provided.
 */
static struct epiz_anodeItem * NewAnodeItem(
  IN struct epiz_anodeItem **headP,
  IN struct epiz_anode *a)		/* anode being added */
{
    int code = 0;
    struct epiz_anodeItem * dP = 0;

    /* make new entry */
    dP = osi_Alloc_r( sizeof(*dP) );
    bzero( (char *)dP, sizeof(*dP) );

    if (headP) { /* if a list was passed in, link it up */
	dP->next = *headP;
	*headP = dP;
    }

    /* Fill all fields */
    dP->refCnt = 1;
    dP->anode = a;
    a->refCnt++;
    return dP;
}

/*
 * epiz_AllocAnodeItem -- return an item for the specified index pair,
 *     allocating a new one if necessary.  If the headP is provided thread it
 *     on that list as a side-effect.
 *
 * CAUTIONS -- these items are sometimes freed in epiz_SalvageZeroLink (file.c)
 *     when it removes all entries from the zeroLinkCnt list.
 */
/* SHARED */
struct epiz_anodeItem *epiz_AllocAnodeItem(
  IN struct epiz_anodeItem ** headP,
  IN u_long anodeIx,
  IN u_long volIx)
{
    int code = 0;
    struct epiz_anodeItem * dP = 0;
    struct epiz_anode * a = epiz_GetAnode (anodeIx, volIx);

    /*
    ** First see if we already have this one on the list...
    */
    if (headP) {
	dP = epiz_FindAnodeItem (*headP, anodeIx, volIx);
	if (dP != 0) {
	    /* we've already seen this anode */
	    dP->refCnt++;
	    return dP;
	}
    }
    dP = NewAnodeItem (headP, a);
    return dP;
}

static int epiz_lostPathnames;		/* count of discarded pathnames */

/* epiz_DecorateAnode -- attaches a pathname to an anode.  It chooses the
 *     shortest pathname.  The return value is zero if the new name is used
 *     otherwise the a one is returned.  Old names that are replaced are
 *     discared. */

/* SHARED */
int epiz_DecorateAnode(
  IN struct epiz_anode *anode,
  IN char * name)
{
    if (!name) return 1;		/* bogus */
    if (!anode->name) {
	anode->name = name;
	anode->flags |= EPIZ_AI_NAME;
	return 0;
    }
    if ((u_int)strlen(name) < (u_int)strlen(anode->name)) {
	/* new name is shorter so use it instead. */
	anode->name = name;
	epiz_lostPathnames++;
	return 0;
    }
    return 1;				/* new name not used. */
}

/*
** epiz_DisplayNames.
**
*/

/* SHARED */
long epiz_DisplayNames(
  IN char *typeName,           /* header for each line */
  IN struct epiz_anodeItem *list)
{
    struct epiz_anodeItem *iP;

    for (iP = list; iP; iP = iP->next) {
	struct epiz_anode *a = iP->anode;
	if (a->name) {
	    printf( "%s %s volume index: %d anode index %d\n",
		    typeName, a->name, a->volIx, a->anodeIx );
	} else {
	    /* no name was available */
	    printf( "%s <no name was available>: volume index: %d anode index %d\n",
		typeName, a->volIx, a->anodeIx );
	}
    }
    return(0);
}

/*
 * epiz_DisplayAllNames -- prints out a list of all remarkable anodes.
 */

/* SHARED */
void epiz_DisplayAllNames(struct epiz_salvageState *ss)
{
    epiz_DisplayNames( "unsafe", ss->unsafeList );
    epiz_DisplayNames( "oughtRestore", ss->oughtRestore );
    epiz_DisplayNames( "mayRestore", ss->mayRestore );
    epiz_DisplayNames( "dupAnode", ss->dupAnodeList );
    epiz_DisplayNames( "badAcls", ss->badAcls );
    epiz_DisplayNames( "badPlists", ss->badPlists );
    epiz_DisplayNames( "badLinkCnts", ss->badLinkCnts );
    epiz_DisplayNames( "zeroLinkCnt", ss->zeroLinkCnt );
    epiz_DisplayNames( "reserveFrags", ss->reserveFrags );
    epiz_DisplayNames( "reserveBlocks", ss->reserveBlocks );
    epiz_DisplayNames( "deadVolumes", ss->deadVolumes );
    epiz_DisplayNames( "deadFiles", ss->deadFiles );

    return;
}

/*
** epiz_AddReserveFrags.
** Record some information about some required space.  The space
** is allocated between SCANPASS and DUPFIXPASS.  This is one
** of the two places where we keep information about necessary
** space to clean up DUP references.
*/

/* SHARED */
long epiz_AddReserveFrags(
  IN epi_anode_t File,
  IN u_long len,
  IN struct epiz_salvageState *ss)
{
    struct epiz_anodeItem *item;

    item = epiz_AllocAnodeItem( &ss->reserveFrags, epia_GetIndex(File),
			       epia_GetVolIndex(File));
    afsl_Assert (item && (item->refCnt == 1));
    item->aiFragLen = len;

    return(0);
}

/*
** epiz_AddReserveBlock.
** Record some information about a block which we want
** to later (between passes) reallocate.
*/
/* SHARED */
long epiz_AddReserveBlock( File, blkno, ss )
  IN epi_anode_t File;
  IN u_long blkno;
  IN struct epiz_salvageState * ss;
{
    struct epiz_anodeItem * item;
    struct epiz_anode * a;

    a = epiz_GetAnode (epia_GetIndex(File), epia_GetVolIndex(File));
    item = NewAnodeItem( &ss->reserveBlocks, a);
    afsl_Assert (item && (item->refCnt == 1));

    /* Fill in block */
    item->aiOldBlock = blkno;
    return(0);
}

/*
** epiz_FindResBlock
** This find routine looks for matches based on
** <volIx,anodeIx,aiOldBlock> in the provided list header.
*/
/* SHARED */
struct epiz_anodeItem * epiz_FindResBlock (headP, anodeIx, volIx, block)
  IN struct epiz_anodeItem * headP;
  IN u_long anodeIx;
  IN u_long volIx;
  IN daddr_t block;
{
    struct epiz_anodeItem * dP;
    struct epiz_anode *a;

    afsl_Assert(headP);

    a = epiz_GetAnode (anodeIx, volIx);
    for (dP = headP; dP; dP = dP->next) {
	if ((dP->anode == a) &&  (dP->aiOldBlock == block)) {
	    return dP;
	}
    }
    return(0);
}

/*
** epiz_EnumerateAnodeItems
** Iterate through the list passed in...
*/
/* SHARED */
long epiz_EnumerateAnodeItems( list, aggrAnode, proc, rock )
  IN struct epiz_anodeItem * list;
  IN epi_anode_t aggrAnode;		 /* aggregate */
  IN long (*proc)();                    /* call for each entry */
  IN char *rock;                        /* pass through parameter */
{
    int code;
    struct epiz_anodeItem * i;

    afsl_Assert((aggrAnode != 0) && (proc != 0) );

    for ( i = list; i != 0; i = i->next) {
	struct epiz_anode *a = i->anode;
	BEGIN_LCRIT (epi_anode_t vol,
		     code = epia_Open(aggrAnode, a->volIx, 0, 0, &vol),
		     code = epia_Close(vol)) {
	    BEGIN_LCRIT (epi_anode_t file,
			 code = epia_Open(vol, a->anodeIx, 0, 0, &file),
			 code = epia_Close(file)) {
		epia_SetAllocationHandle(file,
					 epia_GetAllocationHandle (aggrAnode));
		code = (*proc)( rock, vol, file, i ); 
	    } END_CRIT;
	} END_CRIT;
    }
    return(0);
}


static struct epiz_dupBlockList *AllocDupBlock(
  IN daddr_t b,
  IN u_int fragsPerBlock)
{
    struct epiz_dupBlockList * nb;

    nb = osi_Alloc_r( sizeof(*nb) );
    nb->dupBlock = b;

    /* Compute Number of char (bits) needed */
    nb->fragBitsLen = (fragsPerBlock + 7)>>3;
    nb->fragBits = osi_Alloc_r( nb->fragBitsLen );
    afsl_Assert (nb->fragBits);

    return nb;
}

/*
** FindDupBlock.
** This only finds the duplicate reference block,
*/

static struct epiz_dupBlockList *FindDupBlock(
  IN struct epiz_dupBlockList * bhead,
  IN daddr_t b)
{
    struct epiz_dupBlockList * i;

    for ( i = bhead; i ; i = i->next ) {
	if (i->dupBlock == b) {
	    return i;
	}
    }
    return 0;
}

static u_char Bits[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/* epiz_AddDupBlock -- marks a fragment within a block as having a duplicate
 *     reference to it.  These are checked on the second pass.  It is harmless
 *     to call this routine multiple times on the same fragment. */

/* SHARED */
void epiz_AddDupBlock(
  IN daddr_t b,
  IN u_long fragno,
  IN struct epiz_salvageState *ss)
{
    int code = 0;
    struct epiz_dupBlockList *nb = 0;

    afsl_Assert(ss != 0);

    nb = FindDupBlock( ss->dupList, b );
    if (nb == 0) {
	/* allocate and hook up new DupBlock */
	nb = AllocDupBlock( b, ss->simBits->fragsPerBlock );
	nb->next = ss->dupList;
	ss->dupList = nb;
    }

    /* Mark the bit in the frag. */ {
	int bit8 = fragno >> 3;
	char * byteP = &nb->fragBits[ bit8 ];
	int bitNo = fragno & 7;
	afsl_Assert (nb->fragBits && (bit8 < nb->fragBitsLen));
	*byteP |= Bits[bitNo];
    }
    return;

}


/* epiz_IsDupBlock -- Used to identify fragments in ss which are marked as
 *     multiply referenced.  It always returns either zero (0) or
 *     EPI_E_DUPBLKUSE. */

/* SHARED */
long epiz_IsDupBlock(
  IN daddr_t b,
  IN u_long fragno,
  IN struct epiz_salvageState *ss)
{
    int code = 0; 
    struct epiz_dupBlockList *nb;

    afsl_Assert(ss != NULL);
    if (ss->dupList == 0)
	return(0);

    nb = FindDupBlock( ss->dupList, b );
    if (nb != 0) {
	int bit8 = fragno >> 3;
	char * byteP = &nb->fragBits[ bit8 ];
	int bitNo = fragno & 7;
	if (nb->fragBits && (bit8 < nb->fragBitsLen)) {
	    if (*byteP & Bits[bitNo]) { /* found reference. */
		ss->blkNo = b;
		return(EPI_E_DUPBLKUSE);
	    }
	} else
	    return(0);
    }
    return(0);
}

/*
** Support routines for CopiesVerify
*/
static long AllocCopiesVerify(
  IN u_long Vx,				/* volume index in AVL */
  IN u_long maxAnodeIx,			/* anode index in Vx volume */
  INOUT struct epiz_copiesVerify **nPP)	/* value returned */
{
    long len;

    afsl_Assert(nPP != 0);	/* required parameter */
    /*
     * note: the use of '* (maxAnodeIx)' instead of '* (maxAnodeIx-1)'.
     * AnodeIx values range from 1...maxAnodeIx.
     */
    len = sizeof(**nPP) + (sizeof((*nPP)->anode) * (maxAnodeIx));
    *nPP = osi_Alloc_r( len );
    if (*nPP == 0) { /* allocation failure */
	return( EPI_E_SALVAGEMEM );
    }	
    bzero( (caddr_t)*nPP, len );

    (*nPP)->next = 0;
    (*nPP)->volIx = Vx;
    (*nPP)->maxAnodeIx = maxAnodeIx;

    return(0);
}

static long FreeCopiesVerify( nP )
  IN struct epiz_copiesVerify *nP;
{
    afsl_Assert(nP != 0 );
    osi_Free_r(nP, sizeof(*nP)+ (sizeof(nP->anode) * (nP->maxAnodeIx)));
    return(0);
}

/*
** FindCopiesVerify
** Find <Vx> on 'List'.  Return nP.
*/
static long FindCopiesVerify( Vx, List, nPP )
  IN u_long Vx;
  IN struct epiz_copiesVerify *List;
  INOUT struct epiz_copiesVerify **nPP;
{
    afsl_Assert(nPP != 0 );
    *nPP = 0;

    /* BEGIN */ {
    struct epiz_copiesVerify * c;
    for ( c = List; c; c = c->next )
	if (c->volIx == Vx) { /* found the entry */
	    *nPP = c;
	    return(0);
	}
    }
    /* no matching entry found */
    return(1);
}

/* SHARED */
char * epiz_ssVolName(struct epiz_salvageState * ss)
{
    afsl_Assert(ss != 0 );
    return( ssVolInfo(ss)->ident.name );
}

/* SHARED */
u_long epiz_ssQuotaLimit(struct epiz_salvageState * ss)
{
/* unused */
    afsl_Assert(0);
    /* NOTREACHED */
/* return( ssVolInfo(ss)->quota.limit ); */
}

/* SHARED */
char *epiz_PrintedHyper (h, len, buf)
  IN afs_hyper_t h;
  IN int len;				/* length of buffer */
  OUT char *buf;			/* printed result returned here */
{
    buf[len-1] = 0;
    dfsh_HyperToStr(&h, buf);
    afsl_Assert(buf[len - 1] == 0);	/* check for overwrite */
    return buf;
}

/* SHARED */
char *epiz_NextPrintBuf(void)
{
    static int which;
    static char bufs[4][EPIZ_PRINTBUFLEN];
    which = which & 3;
    return bufs[which++];
}

/* epiz_SuppressError -- disables the printing of the specified error message.
 *     Other error recording functions (e.g. setting the salvage exit code) are
 *     unaffected. */

/* SHARED */
void epiz_SuppressError(int why)
{
    epiz_errorData[why].flags |= EPIZ_E_SUPPRESS;
}

#define Indent(indent) printf ("%*s", indent, "")

/* epiz_ReportError -- is a fairly complicated procedure that prints error
 *     messages and also does some bookkeeping to track anodes affected by
 *     these errors.
 *
 * PARAMETERS -- The why argument is an index into a table defined in
 *     sal_errors.h.  This table gives the error messages as well as various
 *     flag bit that control other functions of this procedure.
 *
 *     Much of the information that ReportError uses in kept in the ss
 *     structure.  The procedure actually takes advantage of a good deal of
 *     subtle actions on the parts of its callers by customizing its behavior
 *     based on the contents of this structure.  Notable items:
 *      1 -- The distiction between when ss->anodeIx is non-zero but ss->anode
 *          is zero (and the corresponding vol case).
 *      2 -- The ss->pathname field is zero'd if the ptr is saved.  This allows
 *          the caller to know whether it is safe to free the string.  Except
 *          for this the caller always allocates (via epis_MakePathName) and
 *          frees (via epis_ReleasePathName) the string.
 *
 *     The errorCode is printed using com_err if it is non-zero and not -1.
 */

/* SHARED */
void epiz_ReportError (ss, errorCode, why, p1,p2,p3,p4)
  IN struct epiz_salvageState * ss;
  IN long errorCode;			/* caller's code, if any */
  IN int why;
  IN opaque p1;
  IN opaque p2;
  IN opaque p3;
  IN opaque p4;
{
    static u_long lastV, lastA;
    static u_long lastX;		/* last aux index */
    static int lastXt;			/* last aux type */
    static int lastF;
    static int lastP;			/* true if last included path info */
    static u_long lastB = (u_long)-1;	/* last bitmap page */

    struct epiz_errorData *d;
    long code;
    int indent;				/* current indentation */
    struct epiz_anodeItem **listHead;
    struct epiz_anodeItem **listHead2;	/* secondary list */
    epi_anode_t bada;
    u_long badai, badvi;
    char *pathname = 0;

    if (why == EPIZ_E_NOOP) return;	/* already printed */
    d = &epiz_errorData[why];
    if (d->flags & EPIZ_E_SUPPRESS)
	goto error_recording;		/* just skip the printing */
    if ((d->flags & EPIZ_E_ADVISE) && !(ss->flags & EPIZ_SS_VERBOSE))
	return;

    if ((d->flags & EPIZ_E_BLOCKDATA) && !(lastF & EPIZ_E_BLOCKDATA))
	/* print if showing blockdata to get EOF info */
        lastV = lastA = 0;

    indent = 0;
    pathname = ss->pathname;
    if (!pathname && ss->volIx && ss->anodeIx) {
	struct epiz_anode * a = epiz_FindAnode (ss->anodeIx, ss->volIx);
	if (a) pathname = a->name;
    }

    if (pathname) {
	if (!lastP || (lastV != ss->volIx) || (lastA != ss->anodeIx)) {
	    lastF = lastX = 0;		/* print per file stuff */
	    printf ("In %d,%d at %s\n", ss->volIx, ss->anodeIx, pathname);
	}
	indent = 2;
    } else if (ss->volIx) {
	if (lastV != ss->volIx) {
	    lastA = 0;			/* print anode if vol changes */
	    if (ss->qVolInfo)
		printf ("In volume %s %s (avl #%d)",
			ssVolInfo(ss)->ident.name,
			epiz_Printed(epiz_PrintedHyper,
				     ssVolInfo(ss)->ident.id),
			ss->volIx);
	    else
		printf ("In volume at avl #%d", ss->volIx);
	    if ((d->flags & EPIZ_E_BLOCKDATA) && ss->vol && !ss->anode) {
		printf (" EOF @ (%lu,,%lu)",
			AFS_HGETBOTH(epia_GetLength(ss->vol)));
	    }
	    printf ("\n");
	}
	indent = 2;
	if (ss->anodeIx) {
	    if (lastA != ss->anodeIx) {
		lastF = lastX = 0;	/* print per file stuff */
		Indent(indent);
		printf ("in anode (#%d)", ss->anodeIx);
		if ((d->flags & EPIZ_E_BLOCKDATA) && ss->anode) {
		    printf (" EOF @ (%lu,,%lu)",
			    AFS_HGETBOTH(epia_GetLength(ss->anode)));
		}
		printf ("\n");
	    }
	    indent += 2;
	}
    }

    if (d->flags & EPIZ_E_BLOCKDATA) {
	Indent(indent);
	if (ss->loc.tree == -2) {
	    printf ("fragment %d:%d in block %#x\n",
		    ssFragFirst(ss), ssFragLength(ss),
		    ss->loc.block);
	} else if (ss->loc.tree == -1) {
	    printf ("index %d is direct block %#x\n",
		    ss->loc.index, ss->loc.block);
	} else {
	    int i;
	    printf ("index %d using indirect tree %d at depth %d:",
		    ss->loc.index, ss->loc.tree, ss->loc.depth);
	    for (i=0; i<=ss->loc.depth; i++) {
		if (ss->loc.blocks[i] != (u_long)-1)
		    printf (" %#x", ss->loc.blocks[i]);
		if (ss->loc.indexes[i] >= 0)
		    printf ("[#%d]", ss->loc.indexes[i]);
	    }
	    printf ("\n");
	}
	indent += 2;
    }

    if (ss->auxWhich) {
	if (lastX != ss->auxWhich) {
	    static char * auxType[7] = {
		"reserved1",
		"acl",
		"initial dir acl",
		"initial file acl",
		"reserved2",
		"plist",
		"user plist"};

	    afsl_Assert (ss->auxWhich < 7);
	    Indent(indent);
	    printf ("in auxiliary anode (#%d) of type %s\n",
		    ss->auxIx, auxType[ss->auxWhich]);
	}
	indent += 2;
    }

    if (d->flags & EPIZ_E_BITMAP) {
	if (ss->bitmapPage != lastB) {
	    Indent(indent);
	    printf ("Bitmap page %lu (%#x)\n",
		    ss->bitmapPage, ss->bitmapBlock);
	}
	indent += 2;
	lastB = ss->bitmapPage;
    }

    lastV = ss->volIx;
    lastA = ss->anodeIx;
    lastP = (pathname != 0);
    lastF = d->flags;
    lastX = ss->auxWhich;

    if (d->msg) Indent(indent);

    /* Use special compact format for reporting bitmap errors. */
    if (d->flags & EPIZ_E_BITMAP) {
	printf ("ix %d ", ss->bitmapIndex);
	printf (d->msg, p1,p2,p3,p4);
	if (ss->flags & EPIZ_SS_SALVAGE) 
	    printf (".  Updated\n");
	else printf ("\n");
    } else {
	if (d->msg) {
	    printf (d->msg, p1,p2,p3,p4);
	    if (errorCode && (errorCode != -1))
		printf (" because %s", dfs_dceErrTxt (errorCode));
	    printf ("\n");
	} else {
	    /* no msg to go with this error code? */
	    afsl_Assert (errorCode == 0);
	}
	if ((ss->flags & EPIZ_SS_SALVAGE) &&
	    !(d->flags & (EPIZ_E_WARN|EPIZ_E_ADVISE)) &&
	    !(ss->flags & EPIZ_SS_BACKINGVOLUME)) {
	    if (d->fix) {
		Indent(indent);
		printf ("-> %s\n", d->fix);
	    }
	}
    }

error_recording:

    /* We've done the printing work.  Bailout now if only generating a warning
     * or advisory.  Otherwise we update various state information to remember
     * this error actually happen. */

    if (d->flags & (EPIZ_E_WARN|EPIZ_E_ADVISE)) {
	afsl_Assert (!d->fix);
	return;
    }

    if ((ss->flags & EPIZ_SS_SALVAGE) && !(d->flags & EPIZ_E_TROUBLE)) {
	ss->flags |= EPIZ_SS_REPAIRED;
	if (d->flags & EPIZ_E_RESALVAGE) ss->flags |= EPIZ_SS_RESALVAGE;
    }
    if ((ss->flags & EPIZ_SS_SALVAGE) && (d->flags & EPIZ_E_BLOCKDATA) &&
	(ss->flags & EPIZ_SS_MAYBERESALVAGE)) {

	/* We've already traversed some blocks in blockmap that we must
         * subsequently discard. */

	ss->flags |= EPIZ_SS_RESALVAGE;
    }
    if ((ss->flags & EPIZ_SS_SALVAGE) &&
	(d->flags & EPIZ_E_BLOCKDATA) && (ss->flags & EPIZ_SS_MAYBECOWUSES)) {

	/* If we've modified the block map of an anode with a non-zero copies
         * count then we may need another pass to check for further COW block
         * reference errors. */

	ss->flags |= EPIZ_SS_DOCOWFIX;
    }
    if (d->flags & EPIZ_E_TROUBLE) ss->flags |= EPIZ_SS_MISC_TROUBLE;
    if (ss->flags & EPIZ_SS_BACKINGVOLUME) {
	ss->flags |= EPIZ_SS_SETINCON;	/* make sure vol is marked INCON */
    }

    listHead = 0;
    listHead2 = 0;			/* some errors add to two lists */
    if ((d->flags & EPIZ_E_REGULARLIST) && ss->anodeIx) {
	int list = d->flags & EPIZ_E_REGULARLIST;
	/* Only mark regular anodes as unsafe.  If ss->anodeIx is zero then the
         * error relates to a volume or the AVL. */
	switch (list) {
	  case EPIZ_E_OUGHTRESTORE: listHead = &ss->oughtRestore; break;
	  case EPIZ_E_MAYRESTORE: listHead = &ss->mayRestore; break;
	  case EPIZ_E_BADLINKCNTS: listHead = &ss->badLinkCnts; break;
	  case EPIZ_E_BADAUX:
	    listHead = &ss->mayRestore;
	    listHead2 = ((ss->auxWhich & 4/*EPIF_AUX_TYPE_PLIST*/)
			 ? &ss->badPlists : &ss->badAcls); break;
	}

    } else {
	if (d->flags & EPIZ_E_DEADANODES) {

	    /* Also if it is open then also set resalvage bit since we've
             * already(?) traversed it. */

	    if (ss->anodeIx && (ss->volIx > epia_GetIndex(ss->aggrAnode))) {

		/* If anodeIx is set and the volIx is not the AVL then this is
                 * a regular file. */

		listHead = &ss->deadFiles;
		if (ss->anode) ss->flags |= EPIZ_SS_RESALVAGE;
	    } else {

		/* Otherwise, if anodeIx isn't set or if the anode is in the
                 * AVL it is a special anode or a volume. */

		/* assert (ss->volIx) */
		listHead = &ss->deadVolumes;
		if (ss->vol) ss->flags |= EPIZ_SS_RESALVAGE;
	    }
	}
    }

    /* If there's an anode mark that otherwise mark volume.  If there is an
     * open handle for either then mark it as unsafe.  If we couldn't even open
     * the anode/vol then just add it to the list by index. */

    bada = ss->anode;
    badai = ss->anodeIx;
    badvi = ss->volIx;
    if (!ss->anodeIx  && ss->volIx) {
	bada = ss->vol;
	badai = ss->volIx;
	badvi = epia_GetIndex(ss->aggrAnode);
    }
    code = 0;
    if (listHead && bada && epia_GetIndex(bada) && epia_GetVolIndex(bada)) {
	code = epiz_MarkUnsafeAnode (bada, listHead, ss);
	afsl_MBZ (code);

    } else if (listHead && badai && badvi) {
	(void) epiz_AllocAnodeItem (listHead, badai, badvi);
	if (listHead2)
	    (void) epiz_AllocAnodeItem (listHead2, badai, badvi);

    } else
	ss->flags |= EPIZ_SS_MISC_PROBLEM; /* !list so report nebulous prob */

    if (badai && badvi && ss->pathname) {
	if (!epiz_DecorateAnode ( epiz_GetAnode(badai, badvi), ss->pathname ))
	    ss->pathname = 0;		/* we squirreled it away */
    }
}

/* epiz_GetExitCode -- maps various error conditions encounted during salvage
 *     and verify into an canonical exit code. */

/* SHARED */
int epiz_GetExitCode(struct epiz_salvageState *ss)
{
    int somePaths;
    int exitCode = 0;

    /* if there were any remarkable anodes */
    somePaths = (ss->unsafeList || ss->mayRestore || ss->oughtRestore ||
		 ss->dupAnodeList || ss->badAcls || ss->badPlists ||
		 ss->badLinkCnts || ss->zeroLinkCnt || ss->reserveFrags ||
		 ss->reserveBlocks || ss->deadVolumes || ss->deadFiles);

    if (somePaths || (ss->flags & EPIZ_SS_MISC_PROBLEM))
	exitCode |= EXIT_PROBLEMS;
    if (ss->flags & EPIZ_SS_REPAIRED) exitCode |= EXIT_REPAIRS;
    if (ss->flags & EPIZ_SS_RESALVAGE) exitCode |= EXIT_RESALVAGE;
    if ((ss->flags & EPIZ_SS_REPLACE_BITMAP) ||
	(ss->flags & EPIZ_SS_GROW_LOG) ||
	(ss->flags & EPIZ_SS_FIX_SUPERBLOCK) ||
	(ss->flags & EPIZ_SS_MISC_TROUBLE))
	exitCode |= EXIT_UNFIXABLE;

    return exitCode;
}

/* epiz_GetExitMessage -- returns are printable representation of a salvage
 *     exit code. */

/* SHARED */
char *epiz_GetExitMessage(int exitCode)
{
    static char *reasons[5] = {
	"problems", "repairs", "resalvage", "unfixable", "crash"};
    static char reason[50];
    int i;

    reason[0] = 0;
    for (i=0; i<5; i++) {
	if (exitCode & (1<<i)) {
	    if (reason[0] != 0) strcat (reason, " ");
	    strcat (reason, reasons[i]);
	}
    }
    if (exitCode && (reasons[0] == 0))
	return "unknown";
    return reason;
}

/* These shouldn't really be here, it is called from fixed_anode.c, but uses
 * various epiv primitives.  I don't understand how to disentagle it now. -ota
 * 910827 */

#include "volume.h"

/* epiz_AddCopiesVerify -- called when anode has a backing anode 'Back' to
 *     record (decrement) a reference to Back.  Called when copies is non-zero
 *     to record (increment) number of expected COW references.
 *
 * RETURN CODES -- returns BADANODEINDEX if bIndex is out of range.  Our caller
 *     can not always validate the input index very conveniently, so we check
 *     here agains copies structure.  If volIndex is bogus we will also return
 *     some error code from epiv_Open or whatever.  I believe these routines
 *     safely do not panic. */

/* SHARED */
u_long epiz_AddCopiesVerify(
  IN long copies,
  IN epi_anode_t Back,
  IN u_long bIndex,
  IN struct epiz_salvageState *ss)
{
    int code;
    struct epiz_copiesVerify *nP;

    if (ss->pass != EPIZ_SS_SCANPASS)
	return 0;			/* only do this on the first pass */

    /* no anode in the AVL should have a copies field or COW bits set. */
    afsl_Assert (Back->volIndex != epia_GetIndex(ss->aggrAnode));
    if (bIndex == 0) return EPI_E_BADANODEINDEX;

    if (FindCopiesVerify( Back->volIndex, ss->copiesList, &nP )) {

	/* One doesn't already exist so allocate a new one.  We need to find
         * out how large of an array we need so open volume and get its max
         * index. */

	u_long lastIndex;
	afsl_Assert(ss->aggrAnode != 0 );

	BEGIN_LCRIT( epi_volume_t vol,
	    code = epiv_Open( ss->aggrAnode, Back->volIndex, &vol ),
	    code = epiv_Close( elbb_zeroTrans, vol ) ) {
		code = epiv_LastIndex( vol, &lastIndex );
	} END_CRIT;
	if (code) return code;

	lastIndex = epiz_ToEPIAIndex (lastIndex);
	if (bIndex > lastIndex) return EPI_E_BADANODEINDEX;
	code = AllocCopiesVerify( Back->volIndex, lastIndex, &nP );
	afsl_MBZ(code);
	nP->next = ss->copiesList;
	ss->copiesList = nP;
    }
    if (bIndex > nP->maxAnodeIx) return EPI_E_BADANODEINDEX;
    nP->anode[bIndex].copies += copies;
    nP->anode[bIndex].block = Back->block;
    nP->anode[bIndex].offset = Back->offset;
    return(0);
}

/*
** epiz_VerifyCopiesVerify.
** Determine how many bad entries are on the list.
** Salvage 
*/
/* SHARED */
long epiz_VerifyCopiesVerify(
  IN struct epiz_salvageState *ss,
  INOUT long * countP)
{
    long code;
    struct epiz_copiesVerify *cv;
    struct epiz_copiesVerify *lastCv;

    if (countP) *countP = 0;

    for ( lastCv = 0, cv = ss->copiesList; cv; lastCv = cv, cv = cv->next ) {
	u_long anodeIx;
	epi_anode_t vol;
	vol = 0;

	if (lastCv != 0) {
	    code = FreeCopiesVerify( lastCv );
	    afsl_MBZ(code);
	    lastCv = 0;
	}

	for ( anodeIx = 0; anodeIx <= cv->maxAnodeIx; ++anodeIx ) {
	    if (cv->anode[anodeIx].copies == 0)
		continue;
	    if (countP) *countP++;
	    ss->anodeIx = anodeIx;
	    ss->volIx = cv->volIx;
	    epiz_ReportError(ss, 0, EPIZ_E_BADCOPIES,
			      cv->anode[anodeIx].copies);
	    if (ss->flags & EPIZ_SS_SALVAGE) {
		if (vol == 0) {
		    afsl_Assert(ss->aggrAnode != 0 );
		    code = epiv_Open( ss->aggrAnode, cv->volIx, &vol );
		    afsl_MBZ(code);
		}
		code = epiz_SalvageCopiesVerify (vol, anodeIx, cv, ss);
		afsl_MBZ(code);
	    }
        }
	if (vol != 0) {
	    code = epiv_Close( elbb_zeroTrans, vol );
	    afsl_MBZ(code);
	}
    }

    if (lastCv != 0) {
	code = FreeCopiesVerify( lastCv );
	afsl_MBZ(code);
	lastCv = 0;
    }
    ss->copiesList = 0;			/* all freed */
    return(0);
}

