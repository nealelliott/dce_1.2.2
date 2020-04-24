/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: paths.c,v $
 * Revision 1.1.94.1  1996/10/02  17:27:22  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:33:56  damon]
 *
 * $EndLog$
 */
/* paths.c -- This file contains to top-level procedures for performing the
 *     directory scan phase of the salvager.  See also walk.c. */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <com_err.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/salvage/paths.c,v 1.1.94.1 1996/10/02 17:27:22 damon Exp $")

#include <dcedfs/episode/critical.h>
#include <dcedfs/episode/logbuf.h>
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/fid.h>
#include <dcedfs/episode/file.h>
#include <dcedfs/episode/volume.h>
#include <dcedfs/episode/aggregate.h>
#include <dcedfs/episode/salvage.h>

#include <salvager.h>
#include <name.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsslvmac.h>
#include <dfsslvsvc.h>
#include <dfsslvmsg.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */


/* DeleteIndex -- kills off a file that has been determined to be openable by
 *     epiv_OpenAnode. */

PRIVATE void DeleteIndex (ss, i)
  IN struct epiz_salvageState * ss;
  IN u_long i;
{
    epi_anode_t h;
    afs_hyper_t zero;
    long code;

    if (ss->flags & EPIZ_SS_SALVAGE) {
	code = epiv_OpenAnode( ss->vol, epiz_ToEPIVIndex(i), &h );
	afsl_MBZ (code);
	AFS_hzero (zero);
	code = epia_Truncate (buffer_nullTranId, h, 0, zero);
	afsl_MBZ (code);
	code = epiv_DeleteAnode (buffer_nullTranId, h);
	afsl_MBZ (code);
    }
}

/* SalvageLinkCount -- adjusts the link count of a file. */

PRIVATE void SalvageLinkCount (ss, i)
  IN struct epiz_salvageState * ss;
  IN u_long i;
{
    long code;
    epi_anode_t h;
    struct epid_volFileId fid;

    epis_MakeIndexOnly( &fid, i, ss );
    afsl_Assert ((ss->typeLinkCounts[i] == EPIZ_SS_FILECNT) &&
		 (ss->fileLinkCounts[i] != ss->linkCounts[i]) &&
		 epid_IsLegal(&fid));

    if (!(ss->flags & EPIZ_SS_SALVAGE)) return;

    code = epif_Open( ss->vol, &fid, &h );
    if (!code)
	code = epiz_SalvageLinkCount (h, ss->linkCounts[i]);
    if (!code) code = epif_Close(h);
    afsl_MBZ (code);
    ss->fileLinkCounts[i] = ss->linkCounts[i];
}

/*
 * epis_DirWalk -- Set up to perform the walk of the anodes.
 *
 * CAUTIONS -- Link count errors generated here can't get decorated with
 *     pathname because the pathname walk already happen, but these problems
 *     weren't noted yet.  If this happens the rescan variable is set and we
 *     restart the directory walk and do only the decoration phase.
 */
PRIVATE long epis_DirWalk( volC, ss )
  IN epi_anode_t volC;
  IN struct epiz_salvageState * ss;
{
    long code = 0;
    long why;				/* return code from epis_Walk */
    u_long nIndex;			/* number of indexes in this volume */
    u_long * linkCounts;
    u_long * fileLinkCounts;
    char * typeLinkCounts;
    epi_volFileId_t rootFid;		/* root for tree walk */
    struct epis_path * volumePrefix;	/* path name structures */
    struct epis_path * pathPrefix;
    int rescan = 0;			/* walk directory structure again */
    int useOldFidIndex;			/* backward support for old indexing */

    useOldFidIndex = (epid_LastIndex (99999, ss->device) == 99999);

    code = epiv_LastIndex( volC, &ss->lastIndex );
    assert( code == 0 );
    ss->lastIndex = epiz_ToEPIAIndex(ss->lastIndex);

    nIndex = ss->lastIndex+1;		/* size to allocate */

    /*
    ** Make an array to contain the link counts of all the anodes.
    ** Make an array to contain anode link counts (of anode reached)
    */
    linkCounts = (u_long *) osi_Alloc( nIndex*sizeof(*linkCounts) );
    if (linkCounts == 0) {
	return(-1);	/* XXX need real error numbers */
    }
    bzero( (char *)linkCounts, nIndex*sizeof(*linkCounts) );
    ss->linkCounts = linkCounts;

    fileLinkCounts = (u_long *) osi_Alloc( nIndex*sizeof(*fileLinkCounts) );
    if (fileLinkCounts == 0) {
	return(-1);	/* XXX need real error numbers */
    }
    bzero( (char *)fileLinkCounts, nIndex*sizeof(*fileLinkCounts) );
    ss->fileLinkCounts = fileLinkCounts;

    typeLinkCounts = (char *) osi_Alloc( nIndex*sizeof(*typeLinkCounts) );
    if (typeLinkCounts == 0) {
	return(-1);	/* XXX need real error numbers */
    }
    bzero( typeLinkCounts, nIndex*sizeof(*typeLinkCounts) );
    ss->typeLinkCounts = typeLinkCounts;

    /* Now set up the pathname variables so the walk knows where it is. */

    volumePrefix = epis_AllocNameElem( ssVolInfo(ss)->ident.name,
				      (struct epis_path *)0 );
    pathPrefix = epis_AllocNameElem( ":", volumePrefix );

    epid_MakeVolumeRoot(&rootFid);
    /*
     * decrement the rootFid reference by one, since the first reference
     * via the root fid doesn't count.
     */
    ss->linkCounts[epis_ToIndex(&rootFid,ss)]--;
    ss->parentFid = &rootFid;		/* root's ".." points to itself */

    why = epis_Walk (volC, &rootFid, ss, pathPrefix);
    if (why == EPI_E_NOENT)
	/* This means that epif_Open failed so the root was no good. */
	why = EPIZ_E_BADFID;

    switch (why) {
      case EPIZ_E_NOLINKS:
      case EPIZ_E_BADDIR:
      case EPIZ_E_BADFID:
      case EPIZ_E_DEADFILE:
	epiz_ReportError (ss, 0, EPIZ_E_BADROOTDIR);
	/* we return this code to our caller */
	break;
      case EPI_E_AGGREGATEFULL:
      case EIO:
	/* already reported at lower level, but punt this volume now */
	break;
      default:
	afsl_MBZ (why);	/* this assertion must fail */
    }

    if (why)
	/* Any serious problem here is going to mark the whole volume as
         * inconsistent, so don't bother with all these other checks.  Just
         * cleanup and get out. */
	goto punt;

    /*
     * Verify LinkCounts.
     * First, we have to try and collect any entries which we haven't yet
     * touched (NOCNT).
     */
    /* If the type was set to BADCNT then the anode was not a valid file.  It
     * may be converted to an ACL (but we don't do that now).  We should punt
     * them. */
    /* BEGIN */ {
	int i;
	for (i=EPIS_FIRSTINDEX; i<=ss->lastIndex; ++i) {
	    epi_anode_t h;
	    unsigned int len;
	    struct epiz_anodeItem *item;
	    u_int32 auxLinkCount;
	    struct epid_volFileId fid;
	    int statusType;		/* type indicated by status size */
	    char type;			/* type of this index */

	    type = ss->typeLinkCounts[i];
	    if ((type != EPIZ_SS_NOCNT) && (type != EPIZ_SS_BADCNT)) continue;

	    /* Punt this file if it was damaged on the first pass. */
	    item = epiz_FindAnodeItem (ss->deadFiles, i, ss->volIx);
	    if (item) {			/* this file is too far gone */
		ss->typeLinkCounts[i] = EPIZ_SS_DEADCNT;
		continue;
	    }

	    ss->anodeIx = i;
	    epis_MakeIndexOnly( &fid, i, ss );
	    if ((type == EPIZ_SS_NOCNT) &&
		epid_IsLegal(&fid) &&	/* index okay for a file */
		!(code = epif_Open( volC, &fid, &h ))) {

		/* Do file verification */

		long isDir;

		if (epia_GetCopies(h) > 0) {
		    if (!(ss->flags & EPIZ_SS_BACKINGVOLUME))
			epiz_ReportError (ss, 0,
					  EPIZ_E_NOTREADONLY_HASBACKING);
		    ss->flags |= EPIZ_SS_BACKINGVOLUME;
		}

		isDir = (((epif_GetMode(h) & S_IFMT) == S_IFDIR)
			 ? EPIF_CFLAGS_DIRECTORY : 0);
		code = epiz_VerifyFileAux (h, isDir, ss, epis_VerifyAcl,
					   epis_VerifyPlist, (opaque)ss);
		if (code) {
		    /* something seriously wrong was already reported */
		    afsl_Assert (code == -1);
		    ss->typeLinkCounts[i] = EPIZ_SS_DELETECNT;
		}
		else if (code == 0) {
		    ss->typeLinkCounts[i] = EPIZ_SS_FILECNT;
		    ss->fileLinkCounts[i] = epif_GetLinkCount(h);
		}
		code = epif_Close (h);
		afsl_MBZ (code);
		continue;
	    }

	    /* not a file, so maybe an auxilary anode of some type? */

	    code = epiv_OpenAnode( volC, epiz_ToEPIVIndex(i), &h );
	    if (code) {
		/* nope, really no good, probably unused or deleted, but should
                 * make sure. */
		ss->typeLinkCounts[i] = EPIZ_SS_FREECNT;
		continue;
	    }
	    ss->anode = h;
	    code = epia_GetStatus (h, sizeof(auxLinkCount),
				   (char *)&auxLinkCount, &len);
	    if (!code) statusType = epiz_LegalFileStatusLength(len);
	    if (code ||
		/* with new indexing acls must not use file indexes */
		(!useOldFidIndex && epid_IsLegal(&fid)) ||
		(statusType != EPIZ_SS_ACLCNT)) {
		/* Trouble.  epia_GetStatus should never really fail.  If the
                 * status size is invalid punt it.  Note that if the status
                 * size indicate a file, but epif_Open failed (just above) then
                 * we punt it as well. */
		epiz_ReportError (ss, 0, EPIZ_E_BADORPHANFILE);
		ss->typeLinkCounts[i] = EPIZ_SS_DELETECNT;
	    } else {
		ss->typeLinkCounts[i] = EPIZ_SS_ACLCNT;
		ss->fileLinkCounts[i] = auxLinkCount;
	    }
	    code = epiv_CloseAnode(h);
	    afsl_MBZ (code);
	    ss->anode = 0;
	}
    }

    /*
    ** Zero the static zeroLinks VerifyZeroLinks maintains,
    ** to use epiz_AllocZeroLink to add zero-link-count files...
    */
    epiz_InitZeroLinks();

    /*
    ** Now compare the values.
    ** Note: the tests here are longer than they need to
    ** be to report 'More/Fewer'.  Originally, it was belived
    ** the behaviors would be different for some of the
    ** state changes described below.
    ** Note: we treat linkCounts on files, acls, and plists
    ** EXACTLY the same way.  We may not be able to do that forever. XXX
    */

    /* BEGIN */ {
	int i;
	for (i=EPIS_FIRSTINDEX; i<=ss->lastIndex; ++i) {
	    int state;			/* state link count for index */
	    char type = ss->typeLinkCounts[i];
	    ss->anodeIx = i;
	    ss->anode = 0;

	    /* these were filtered out in previous loop */
	    afsl_Assert ((type != EPIZ_SS_NOCNT) && (type != EPIZ_SS_BADCNT));

	    /* these are handled on SCAN pass */
	    if (type == EPIZ_SS_DEADCNT)
		continue;

	    /* these are probably free, but make sure */
	    if (type == EPIZ_SS_FREECNT) {
		/* need to write epit_MakeFree function to do this.  Meanwhile
                 * just assume it is correctly free. */
		continue;
	    }

	    /* these should be deleted w/ extreme prejudice */
	    if (type == EPIZ_SS_DELETECNT) {
		if (ss->flags & EPIZ_SS_BACKINGVOLUME) {
		    epiz_ReportError (ss, 0, EPIZ_E_BADBACKINGORPHAN);
		    continue;
		}
		DeleteIndex (ss, i);
		continue;
	    }

	    /* for files, acls and plists check the link count against expected
             * value. */
	    afsl_Assert ((type == EPIZ_SS_FILECNT) ||
			 (type == EPIZ_SS_ACLCNT) ||
			 (type == EPIZ_SS_PLISTCNT));

#define S_FILE 1			/* index is an file, */
#define S_AUX 0				/*   else just a aux */
#define S_WRONG 2			/* link count no expected value */
#define S_UNREFD 4			/* should be no references */
	    state = ((type == EPIZ_SS_FILECNT) ? S_FILE : 0) +
		((ss->linkCounts[i] != ss->fileLinkCounts[i]) ? S_WRONG : 0) +
		    ((ss->linkCounts[i] == 0) ? S_UNREFD : 0);

	    if ((state == S_FILE) || (state == S_AUX) ||
		(state == S_UNREFD+S_FILE)) {
		/* These are okay states */
	    } else {
		/* These states require repair, which we can't do if this is a
                 * backing fileset. */
		if (ss->flags & EPIZ_SS_BACKINGVOLUME) {
		    epiz_ReportError (ss, 0, EPIZ_E_BADBACKINGLINKS);
		    continue;
		}
	    }

	    switch (state) {
	      case S_FILE:		/* okay file */
	      case S_AUX:		/* okay AUX */
		break;

	      case S_WRONG+S_FILE:
		epiz_ReportError (ss, 0, EPIZ_E_BADLINKFILE,
				  ss->fileLinkCounts[i], ss->linkCounts[i]);
		SalvageLinkCount (ss, i);
		(void) epiz_AllocAnodeItem (&ss->badLinkCnts, i, ss->volIx);
		rescan++;
		break;

	      case S_WRONG+S_AUX:
		epiz_ReportError (ss, 0, EPIZ_E_AUX_BADLINK,
				  ss->fileLinkCounts[i], ss->linkCounts[i]);
		if (ss->flags & EPIZ_SS_SALVAGE) {
		    code = epiz_SalvageAux (volC, epiz_ToEPIVIndex(i),
					    ss->linkCounts[i]);
		    afsl_MBZ (code);
		    ss->fileLinkCounts[i] = ss->linkCounts[i];
		}
		break;

	      case S_UNREFD+S_AUX:
	      case S_UNREFD+S_WRONG+S_AUX:
		epiz_ReportError (ss, 0, EPIZ_E_AUX_ORPHAN);
		DeleteIndex (ss, i);
		break;

	      case S_UNREFD+S_WRONG+S_FILE:
		epiz_ReportError (ss, 0, EPIZ_E_BADLINKFILE,
				  ss->fileLinkCounts[i], ss->linkCounts[i]);
		SalvageLinkCount (ss, i);
		/* fall through */

	      case S_UNREFD+S_FILE:
		/* We only report these, if zeroLink list changes, when we dump
                 * the contents of this list at the end of the salvage.  The
                 * code in epiz_SalvageZeroLink flushes this list if there are
                 * no errors. */
		/* epiz_ReportError (ss, 0, EPIZ_E_ZEROLINKFILE); */
		code = epiz_AllocZeroLink (epiz_ToEPIVIndex(i), ss);
		afsl_MBZ (code);
		break;

	      default:
		afsl_Assert (state == -1); /* can't happen */
	    }
	}
    }
    ss->anodeIx = 0;

    /* We are done repairing per-file problems so clear these bits so we allow
     * repairs even of backing filesets. */

    ss->flags &= ~EPIZ_SS_BACKINGVOLUME;

    why = epiz_SalvageZeroLink( volC, ss );
    if (why == EPIZ_E_BADBACKINGZLC)
	/* Unused: we *DO* repair ZLC of backing fileset. */
	goto punt;
    afsl_MBZ (why);

    if (rescan) {
	/* Invoke the directory walk again to decorate link count errors with
         * pathnames.  We set GETPATHNAMES so the epis_Walk knows to skip most
         * work.  But we still need to re-zero the typeLinkCounts array so we
         * can avoid getting hung up by directory loops. */
	ss->flags |= EPIZ_SS_GETPATHNAMES;
	bzero( ss->typeLinkCounts, nIndex*sizeof(*typeLinkCounts) );
	(void) epis_Walk (volC, &rootFid, ss, pathPrefix);
	ss->flags &= ~EPIZ_SS_GETPATHNAMES;
    }

punt:
    epis_FreeNameElem( pathPrefix );
    epis_FreeNameElem( volumePrefix );

    (void) osi_Free( (opaque)linkCounts, nIndex*sizeof(*linkCounts) );
    ss->linkCounts = 0;
    (void) osi_Free ( (opaque)fileLinkCounts, nIndex*sizeof(*fileLinkCounts) );
    ss->fileLinkCounts = 0;
    (void) osi_Free ( (opaque)typeLinkCounts, nIndex*sizeof(*typeLinkCounts) );
    ss->typeLinkCounts = 0;

    /* Caller can mark volume as inconsistent if there was trouble. */
    return(why);
}

/* epis_VerifyAggrPaths -- called from ss->dirScanPass to walk the directory
 *     structure. */

SHARED long epis_VerifyAggrPaths (ss)
  IN struct epiz_salvageState * ss;
{
    long code;
    long i;				/* common iterator */
    u_long lastIndex;

    if (ss->flags & EPIZ_SS_NODIRSCAN) {
	dce_svc_printf(SLV_S_DISABLING_DIRECTORY_SCAN_MSG);
	return 0;
    }

    /*
     * Find the max index in the AVL.  Then just iterate through each of the
     * entries.  We open the anode, then we test for volume.  If it is a
     * volume, we call a routine to walk the directory (starting at the root
     * fid).
     */
    code = epit_LastIndex( ss->aggrAnode, &lastIndex );
    assert( code == 0 );

    for ( i=1; i<=lastIndex; ++i ) {

	/* punt this volume if it was damaged on the first pass */
	if (epiz_FindAnodeItem (ss->deadVolumes, i,
				epia_GetIndex(ss->aggrAnode)))
	    continue;			/* this volume is too far gone */

	/* Open the entry.  See what it is */
	ss->volIx = i;
	BEGIN_LCRIT (epi_anode_t entry,
		     code = epia_Open (ss->aggrAnode, i, 0, 0, &entry),
		     code = epia_Close (entry)) {
	    struct epia_info info;

	    code = epia_GetInfo( entry, &info );
	    afsl_MBZ (code);
	    if ((info.flags & EPIA_CFLAGS_TYPE) != EPIA_CFLAGS_VOLUME)
		continue;		/* don't care about these here */

	    /*
	     * Open the volume, get some info about it, do some processing on
	     * the root.
             */
	    BEGIN_LCRIT (epi_volume_t volC,
			 code = epig_OpenVolume (ss->aggrAnode, i, &volC),
			 code = epig_CloseVolume(buffer_nullTranId,volC)) {
		struct epiv_status volInfo;
		ss->vol = volC;
		ss->qVolInfo = (opaque)&volInfo;
		ss->flags &= ~(EPIZ_SS_BACKINGVOLUME|EPIZ_SS_SETINCON);
		code = epiv_GetStatus(volC, &volInfo);
		afsl_MBZ (code);

		if (volInfo.flags & EPIV_STATUS_FLAGS_INCONSISTENT){
		    /*
		     * Just tell the user...
		     */
		    epiz_ReportError (ss, 0, EPIZ_E_VOLINCONSISTENT);
		    continue;
		}

		/* Set BACKINGVOLUME using READONLY header bit */
		if (volInfo.flags & EPIV_STATUS_FLAGS_READONLY)
		    ss->flags |= EPIZ_SS_BACKINGVOLUME;

		/* Find largest file VV */
		AFS_hzero(ss->highAnode);
		/* Suppress quota check during repairs. */
		if (!(ss->flags & EPIZ_SS_BACKINGVOLUME) &&
		    (ss->flags & EPIZ_SS_SALVAGE)) {
		    code = epiv_SuppressQuotaCheck (volC, 1);
		    afsl_MBZ (code);
		}

		code = epis_DirWalk( volC, ss );

		/* return quota checking to normal */
		epiv_SuppressQuotaCheck (volC, 0);
		/* stop diabling repairs due to backing. */
		ss->flags &= ~EPIZ_SS_BACKINGVOLUME;

		if (code || (ss->flags & EPIZ_SS_SETINCON)) {
		    if (!code && (volInfo.flags &
				  EPIV_STATUS_FLAGS_INCONSISTENT))
			/* already set incon */;
		    else {
			epiz_ReportError (ss, 0, EPIZ_E_BADVOLUME);
			if (ss->flags & EPIZ_SS_SALVAGE) {
			    volInfo.flags |= EPIV_STATUS_FLAGS_INCONSISTENT;
			    code = epiv_SetStatus (buffer_nullTranId, volC,
						   EPIV_STATUS_FLAGS,
						   &volInfo);
			    afsl_MBZ (code);
			}
		    }
		    code = 0;
		}
		if (!(volInfo.flags & EPIV_STATUS_FLAGS_INCONSISTENT) &&
		    (AFS_hcmp (volInfo.version, ss->highAnode) < 0)) {

		    /* The volume's VV is too small. */

		    epiz_ReportError
			(ss, 0, EPIZ_E_2SMALLVOLVERSION,
			 epiz_Printed (epiz_PrintedHyper, volInfo.version),
			 epiz_Printed (epiz_PrintedHyper, ss->highAnode));
		    if (ss->flags & EPIZ_SS_SALVAGE) {
			volInfo.version = ss->highAnode;
			code = epiv_SetStatus (buffer_nullTranId, volC,
					       EPIV_STATUS_VERSION, &volInfo);
			afsl_MBZ (code);
		    }
		}
	    } END_CRIT;
	    ss->qVolInfo = 0;		/* Remove out-of-scope structure ptr */

	    afsl_MBZ (code);
	} END_CRIT;
    }					/* lastIndex iterator */
    return code;
}

/*
** epis_VerifyDirectoryPaths
**
** This code is included to provide an external interface
** to walk the directory structure.  There is a danger
** in salvaging the directory when this routine is called:
** any COW anodes which reference COW directories are NOT
** updated... This interface ought to only be used when
** verifying, not salvaging.
**
 */

EXPORT long epis_VerifyDevPaths( device, ss )
  IN struct async_device *device;
  IN struct epiz_salvageState * ss;
{
    int code = 0;
    struct aggregate * aggr;

    if (ss->flags & EPIZ_SS_SALVAGE) {
	dce_svc_printf(SLV_S_COW_ANODES_MSG);
	dce_svc_printf(SLV_S_FURTHER_DIRECTORIES_MSG );
	dce_svc_printf(SLV_S_FURTHER_SALVAGE_MSG );
    }

    BEGIN_LCRIT(epi_anode_t aggrAnode,
		code = epig_InitAggregate (device->majdev, device->mindev,
				    0, 0, &aggrAnode),
		code = epig_CloseAggregate(aggrAnode)) {

	dce_svc_printf(SLV_S_AVL_ANODES_MSG, aggrAnode);
	aggr = (struct aggregate *) epia_GetFileRock(aggrAnode);
	if (aggr == 0) {
	    dce_svc_printf(SLV_S_NO_AGGREGATE_HANDLE_MSG, aggrAnode);
	    ABORT_CRIT (-1);
	}
	code = epis_VerifyAggrPaths(ss);
    } END_CRIT;

    return(code);
}
