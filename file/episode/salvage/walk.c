/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* walk.c -- */

/* Copyright (C) 1995, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/salvage/RCS/walk.c,v 4.58 1995/06/11 23:44:20 dlc Exp $")

#include <dcedfs/episode/critical.h>
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/file.h>
#include <dcedfs/dir_buffer.h>
#include <dcedfs/dir.h>
#include <dcedfs/episode/salvage.h>

#include "name.h"
#include "salvager.h"

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsslvmac.h>
#include <dfsslvsvc.h>
#include <dfsslvmsg.h>

static char *dirs_ok_error = 0;
static int verbose = 0;

/* directory_error -- supply a new directory error message.  Complain and punt
 *     old message if any.  If no new msg supplied just punt old message. */

static void directory_error(char *msg)
{
    /* XXX what about allocated errors? */
    /* XXX for now */
    if (dirs_ok_error) {
	if (msg && verbose) {
	    dce_svc_printf(SLV_S_OVERWRITE_OLD_DIR_ERROR_MSG, dirs_ok_error);
	}
	(void) osi_Free( dirs_ok_error, strlen(dirs_ok_error) + 1 );
    }
    if (msg) {
	/* Copy msg away, but skip the trailing newline, if any */
	u_int len = strlen(msg);
	if (msg[len-1] == '\n') len--;
	dirs_ok_error = (char *)osi_Alloc( len + 1 );
	strncpy( dirs_ok_error, msg, len );
	dirs_ok_error[len] = 0;		/* add terminating null */
    } else {
	dirs_ok_error = 0;
    }
}

#define WITH_PATH(pathname,path) \
    BEGIN_LCRIT(char *pathname,\
		pathname = epis_MakeNamePath (path), \
		epis_ReleaseNamePath (path))
#define END_PATH END_CRIT

static long Enumerate(
  IN epi_anode_t volC,
  IN epi_anode_t file,
  IN struct epia_info *info,
  IN struct epiz_salvageState *ss,
  IN struct epis_path * path)
{
    int code = 0;
    int i;
    int dirIsOkay;
    int pathOnly = ss->flags & EPIZ_SS_GETPATHNAMES;
    struct Dir_Handle dh;
    int dirstate;			/* problems with directory */
    long hint;
    epi_volFileId_t *myParent;		/* fid of my parent directory */
    epi_volFileId_t *myFid;		/* fid of this directory */
    u_long myIndex;			/* anodeId of this directory */

    if (!pathOnly) ss->nDirectories++;

    /* set this static flag so directory_error can see it. */
    verbose = ss->flags & EPIZ_SS_VERBOSE;

    /*
    ** Turn file into a Dir_Handle,
    ** then find every entry in the directory.
    */
    SetDirHandle(file, &dh);

    myParent = ss->parentFid;		/* save these */
    myFid = ss->fid;
    myIndex = ss->anodeIx;
    /*
    ** See if the contents of the directory seem OK...
    */
    dirIsOkay = dirs_OK ( &dh, directory_error);
    if (!dirIsOkay && !pathOnly) {
	/* failure and not just getting pathnames */

	afsl_Assert(ss->pathname == 0);
	ss->pathname = epis_MakeNamePath (path);

	if (dirs_ok_error) {
	    epiz_ReportError (ss, 0, EPIZ_E_DIRERROR, dirs_ok_error);
	    directory_error (0);
	    ss->flags |= EPIZ_SS_MISC_PROBLEM;
	}

	if (ss->flags & EPIZ_SS_BACKINGVOLUME) {
	    /* If this is a backing anode, then we're going to bail out because
             * we cannot safely modify such a directory. */

	    epiz_ReportError (ss, 0, EPIZ_E_BADBACKINGDIR);
	}
	else if (ss->flags & EPIZ_SS_SALVAGE) {
	    /* Now dirs_salvage the directory... */

	    struct Dir_Handle salvage_dh;

	    /*
	    ** SetSalvageDirHandle uses osi_Alloc'ed buffers
	    ** to cache the entire contents of the directory.
	    */
	    SetSalvageDirHandle( file, &salvage_dh );

	    dirs_ok_error = 0;
	    dirstate = dirs_Salvage( &dh, &salvage_dh, directory_error );
	    afsl_Assert (!(dirstate & DIRS_EXIT_IO));

	    if (dirs_ok_error) {
		/* Recorded some error during salvage, so report last one. */
		epiz_ReportError (ss, 0, EPIZ_E_DIRSALERROR, dirs_ok_error);
		directory_error (0);
	    }

	    /* Now process various hard error cases */
	    if (dirstate & DIRS_EXIT_BAD) {
		/* punt this entry, caller will ReportError */
		code = EPIZ_E_BADDIR;
	    } else for (i=0; i<2; i++) if (dirstate &
					   ((i==0) ? DIRS_EXIT_NODOT
					    : DIRS_EXIT_NODOTDOT)) {

		/* Otherwise the directory problems are relatively minor, just
                 * fix them up by creating "." and/or ".." as appropriate and
                 * go on. */

		epi_volFileId_t dummy;	/* dummy fid for dir_lookup */
		long offset;		/* offset for dir_Lookup */
		long why = ((i==0) ? EPIZ_E_NODOT : EPIZ_E_NODOTDOT);
		char *name = ((i==0) ? "." : "..");
		epi_volFileId_t *fid = ((i==0) ? myFid : myParent);

		epiz_ReportError (ss, 0, why,
				  epiz_Printed(epid_PrintedFid, fid));
		code = dir_Lookup (&salvage_dh, name, &dummy, &offset);
		afsl_PAssert (code == ENOENT,
			      ("Got code (%d) from dir_Lookup on \"%s\"",
			       code, name));
		code = dir_Create (&salvage_dh, name, fid, offset, 0,
				   elbb_NullTranId);
		afsl_MBZ(code);
	    }
	    /*
             * Write back the new directory contents.  New disk blocks are
             * allocated if overwriting a COW anode.  No new anode is created.
             */
	    code = PutSalvageDirHandle( &salvage_dh );

	    /*
	     * Make sure everything is clean... like rewind...
	     */
	    PutDirHandle( &dh );
	    SetDirHandle(file, &dh);

	    if (code == 0) {
		dirIsOkay = dirs_OK ( &dh, directory_error);
		afsl_Assert(dirIsOkay != 0 );
		ss->flags |= EPIZ_SS_REPAIRED;
	    } else {
		switch (code) {
		  case EPI_E_AGGREGATEFULL:
		    epiz_ReportError (ss, 0, EPIZ_E_AGGREGATEFULL);
		    break;
		  case EPI_E_VOLUMEFULL:
		    /* these are suppressed during salvage */
		  default:
		    afsl_MBZ(code);
		}
	    }
	}

	if (ss->pathname) {
	    epis_ReleaseNamePath (path);
	    ss->pathname = 0;
	}
    }

    /* At this point either the dir is OK (and code is zero) or if the dir is
     * not OK then if we are salvaging the there should be a non-zero code
     * indicating why we couldn't do the repair.  If we are verifying only,
     * then code may be zero.  In anycase, only recurse if the directory is
     * okay. */

    hint = 0;
    while (dirIsOkay) {
	long pieceNo;
	struct Dir_Page *dbP;
	struct Dir_Entry *entryP;
	struct epis_path * newPath;	/* entry information */
	epi_volFileId_t fid;
	long why;
	int isdot, isdotdot;

	afsl_MBZ(code);
	code = 0;

	pieceNo = dir_PieceScan(&dh, hint);
	if (pieceNo == 0) {		/* all done? */
	    break;
	}
	entryP = dir_GetPiece(&dh, pieceNo, &dbP);
	afsl_Assert (entryP);		/* just called PieceScan to validate */

	/* Save info from entry on stack so we don't hold the dir buffer over
	 * the recursion step. */

	hint = pieceNo + entryP->length;
	newPath = epis_AllocNameElem((char *)entryP->name, path );
	fid = entryP->fid;

	dirb_Release((caddr_t)dbP);

	afsl_Assert(ss->pathname == 0);

	isdot = (strcmp (newPath->name, ".") == 0);
	isdotdot = (strcmp (newPath->name, "..") == 0);
	if (isdot || isdotdot) {

	    /* Check that "." and ".." point where they should.  If not set
	     * the fids of these entries to the correct value. */

	    epi_volFileId_t *dotFid = (isdot ? myFid : myParent);
	    long why = (isdot ? EPIZ_E_REPLACEDOT : EPIZ_E_REPLACEDOTDOT);

	    if (!epid_EqualIndexes(&fid, dotFid)) {
		epiz_ReportError (ss, 0, why,
				  epiz_Printed(epid_PrintedFid, dotFid));
		if (ss->flags & EPIZ_SS_BACKINGVOLUME) {
		    epiz_ReportError (ss, 0, EPIZ_E_BADBACKINGDIR);
		}
		else if (ss->flags & EPIZ_SS_SALVAGE) {
		    fid = *dotFid;	/* set local fid and repair dir */
		    code = dir_Replace (&dh, pieceNo, &fid, elbb_NullTranId);
		    afsl_MBZ(code);
		}
	    }
	}

	/* Do recursive descent */

	ss->parentFid = ss->fid;
	why = epis_Walk (volC, &fid, ss, newPath);
	ss->parentFid = myParent;
	ss->fid = myFid;
	ss->anodeIx = myIndex;

	if (why && (isdot || isdotdot)) {

	    /* We shouldn't get an error on "." or ".." because we just
	     * checked their fids and reported any errors above.  But if
	     * they couldn't be repaired, just skip them now. */

	    afsl_Assert ((ss->flags & EPIZ_SS_BACKINGVOLUME) ||
			 !(ss->flags & EPIZ_SS_SALVAGE));
	    why = 0;
	}

	if (why != 0) {
	    if (!ss->pathname)
		ss->pathname = epis_MakeNamePath (path);

	    /*
	     * The only errors we ought to see are ones we want to recover
	     * from... like zero-link-count in the file error.
	     */
	    if (why == EPI_E_NOENT)

		/* This means that epif_Open failed so the fid was no good,
		 * like unique was wrong, or index pointed to an acl, so
		 * flush the dir entry now, and maybe delete the file later
		 * as well.
		 */
		why = EPIZ_E_BADFID;

	    switch (why) {
	      case EPIZ_E_NOLINKS:
	      case EPIZ_E_BADDIR:
	      case EPIZ_E_BADFID:
	      case EPIZ_E_DEADFILE:
		epiz_ReportError (ss, 0, why, newPath->name,
				  epiz_Printed(epid_PrintedFid, &fid));
		break;
	      case EPI_E_AGGREGATEFULL:
	      case EIO:
		code = why;		/* a subdir was damaged */
		break;
	      default:
		afsl_MBZ(why);		/* this assertion must fail */
	    }

	    if (code == 0) {
		if (ss->flags & EPIZ_SS_BACKINGVOLUME) {
		    epiz_ReportError (ss, 0, EPIZ_E_BADBACKINGDIR);
		}
		else if (ss->flags & EPIZ_SS_SALVAGE) {
		    /*
		     * remove the entry from the directory.
		     */
		    code = dir_Delete (&dh, pieceNo, buffer_nullTranId);
		    afsl_MBZ(code);
		}
	    }
	}
	if (ss->pathname) {
	    epis_ReleaseNamePath (path);
	    ss->pathname = 0;
	}
	epis_FreeNameElem(newPath);
	if (code) break;
    }

    PutDirHandle( &dh );
    return code;
}

/* epis_Walk -- is the primitive operation to deal with a file in the directory
 *     hierarchy.  If the file is a directory it recurses by calling Enumerate.
 *
 * PARAMETERS -- It checks the EPIZ_SS_GETPATHNAMES bit in the ss->flags word.
 *     If set it skips most of the work and just recurses on directories and
 *     decorates pathnames.
 *
 * RETURN CODES -- There are two important codes returned by this routine, in
 *     addition to the codes that can be returned by lower level procedures
 *     (epif_GetStatus is the main one here).
 *         EPIZ_E_NOLINKS -- which indicates that a file isn't being references
 *             properly or has a zero linkCount and should be deleted.
 *	   EPIZ_E_BADDIR -- which indicates that the directory salvager was
 *	       unable to make a good directory and recomends that the directory
 *	       be removed.  This only comes from recursive call to Enumerate.
 *         EPIZ_E_DEADFILE -- file was damaged beyond repair in pass one so we
 *             just flush the directory entry that point to it.
 *	   EPI_E_AGGREGATEFULL -- ran out of space repairing a directory.  Mark
 *	       the whole volume as INCONSISTENT instead.
 *         EPI_E_NOENT -- This error is returned by epif_Open and basically
 *             means that it isn't a file.  Perhaps it is an acl or some worse
 *             problem.  We'll remove the directory entry immediately, and if
 *             it doesn't turn out to be an acl or something sensible later
 *             we'll delete it.
 */

/* SHARED */
long epis_Walk(
  epi_anode_t volC,
  epi_volFileId_t *fid,
  struct epiz_salvageState *ss,
  struct epis_path *path)
{
    int code = 0;
    struct epis_path * dirPath = 0;
    char oldType;			/* to avoid directory loops */

    afsl_Assert((ss->linkCounts != 0) &&
	   (ss->fileLinkCounts != 0) &&
	   (ss->typeLinkCounts != 0));

    if (!epid_IsLegal(fid)) return EPIZ_E_BADFID;
    /* uniquifiers in on-disk fids are now set to zero -- 12/16/92 BWL */
    if (epid_UniquePart(fid) == 0)
	epid_SetDCUnique(fid);
    ss->fid = fid;
    ss->anodeIx = epis_ToIndex(fid,ss);

    if (ss->anodeIx < EPIS_FIRSTINDEX)
	return EPIZ_E_BADFID;		/* some serious problem! */
    if (ss->anodeIx > ss->lastIndex)
	return EPIZ_E_BADFID;		/* fid past end of volume table */

    /* Errors past this point should go to decorate name to provide as much
     * pathname info as possible. */

    /* Save the old type so we can avoid getting stuck in directory loops. */
    oldType = ss->typeLinkCounts[ss->anodeIx];

    if (!(oldType == EPIZ_SS_NOCNT) && !(oldType == EPIZ_SS_FILECNT)) {
	/* This fid is known to be bogus so punt now, removing directory entry
         * that points here. */
	switch (oldType) {
	  case EPIZ_SS_DEADCNT:
	    code = EPIZ_E_DEADFILE; break;
	  case EPIZ_SS_BADCNT:
	  case EPIZ_SS_DELETECNT:
	    code = EPI_E_NOENT; break;
	  default:
	    /* Others are unused or only assigned after directory walk */
	    afsl_Assert (oldType == 0);
	}
	if (code) goto decorate_name;
    }

    {	/* Punt this file if it was damaged on the first pass. */
	struct epiz_anodeItem *item;
	item = epiz_FindAnodeItem (ss->deadFiles, ss->anodeIx, ss->volIx);
	if (item) {			/* this file is too far gone */
	    ss->typeLinkCounts[ss->anodeIx] = EPIZ_SS_DEADCNT;
	    code = EPIZ_E_DEADFILE;
	    goto decorate_name;
	}
    }

    BEGIN_LCRIT (epi_anode_t file,
		 code = epif_Open( volC, fid, &file ),
		 code = epif_Close(file)) {
	struct epia_info aInfo;
	u_short fLinkCount;		/* a few file status values */
	u_short fMode;
	u_long fUnique;
	long isDir;

	code = epia_GetInfo( file, &aInfo ); /* get the index */
	if (code) {			/* some unresolved low level problem */
	    epiz_ReportError (ss, code, EPIZ_E_FILEPROBLEM);
	    ABORT_CRIT (EPI_E_NOENT);
	}
	if (aInfo.copies > 0) {
	    if (!(ss->flags & EPIZ_SS_BACKINGVOLUME))
		epiz_ReportError (ss, 0, EPIZ_E_NOTREADONLY_HASBACKING);
	    ss->flags |= EPIZ_SS_BACKINGVOLUME;
	}

	/* Calling epif_GetStatus is too dangerous if repairs are not being
         * made so just get the individual fields we need. */

	fMode = epif_GetMode(file);
	isDir = (((fMode & S_IFMT) == S_IFDIR) ? EPIF_CFLAGS_DIRECTORY : 0);

        /* Before we try anything in the epif module make sure we've checked if
         * out.  Verify the auxiliary containers of file exactly once so we get
         * the aux link counts correct. */

	if (oldType == 0) {
	    code = epiz_VerifyFileAux (file, isDir, ss, epis_VerifyAcl,
				      epis_VerifyPlist, (opaque)ss );
	    if (code) {
		/* something seriously wrong was already reported */
		afsl_Assert (code == -1);
		code = 0;
		ss->typeLinkCounts[ss->anodeIx] = EPIZ_SS_DELETECNT;
		continue;
	    }
	}

	fUnique = epif_GetUnique(file);
	epid_SetUnique (ss->fid, fUnique);

	if (ss->flags & EPIZ_SS_GETPATHNAMES) {

	    /* punt everything else here and just recurse on dirs */

	    ss->typeLinkCounts[ss->anodeIx] = EPIZ_SS_FILECNT;
	    if (isDir && (oldType == 0)) {
		dirPath = epis_AllocNameElem( "/", path );
		(void) Enumerate (volC, file, &aInfo, ss, dirPath);
		/* don't care about the code */
	    }
	    continue;			/* punt the rest of the work */
	}

	fLinkCount = epif_GetLinkCount(file);
	ss->fileLinkCounts[ss->anodeIx] = fLinkCount;
	ss->typeLinkCounts[ss->anodeIx] = EPIZ_SS_FILECNT;
	if (fLinkCount == 0) {
	    /*
	     * In the case of a file with a zero linkCount we fix the
	     * problem by removing the directory entry.  We can't do
	     * that here, but up above, we can!  Just return NOLINKS
	     * and caller will flush entry.  On the other hand, if this
	     * is a directory something is definitely wrong since a
	     * directory must always have a linkCount of at least two.
	     * So for dirs, just push on and let the top-level reset
	     * the linkCount to the proper value.
	     */
	    if (!isDir)
		{ ABORT_CRIT (EPIZ_E_NOLINKS) }
	}

	/*
	 ** Do type dependent operations.
	 */
	switch (fMode & S_IFMT) {
	  case S_IFDIR:
	    if (oldType == 0) {
		/* since enumerate will change anodeIx we need to restore it
                 * upon return. */
		u_long localIndex = ss->anodeIx;

#if 0 /* this isn't necessary since we don't do any COWPASS stuff anymore, and
       * is screws up some stuff that only expects to be called from SCAN and
       * DUPFIX passes. */
		/*
		 ** Before we look at the directory, we have to
		 ** make sure any COW references are correct.
		 */
		if (aInfo.flags & EPIA_CFLAGS_COPYONWRITE) {
		    code = epia_VerifyAnode( ss, volC, ss->anodeIx );
		    afsl_Trace (AFSL_TR_ERRMAP,
 ("epis_Walk: discarding code %u from epia_VerifyAnode", code));
		}
#endif

		dirPath = epis_AllocNameElem( "/", path );
		code = Enumerate (volC, file, &aInfo, ss, dirPath);
		ss->anodeIx = localIndex;
		if (code) continue;
	    } else
		if (oldType != EPIZ_SS_FILECNT) {
		    dce_svc_printf(SLV_S_EPIS_WALK_MSG, oldType, ss->anodeIx );
		    dce_svc_printf(SLV_S_DIRECTORY_TYPE_EXPECTED_MSG );
		    ABORT_CRIT ( EPIZ_E_NOLINKS );
		}
	    break;

	  case S_IFCHR:
	  case S_IFBLK:
	  case S_IFREG:
	  case S_IFLNK:
	  case S_IFSOCK:
	  case S_IFIFO:
	    break;

	  default:
	    /* Unknown type, so act as if epif_Open failed. */
	    ss->typeLinkCounts[ss->anodeIx] = EPIZ_SS_BADCNT;
	    code = EPI_E_NOENT;
	}
	if (code) continue;
	ss->linkCounts[ss->anodeIx]++;	/* only inc if everything is okay */
	if (!isDir && (oldType == 0))
	    ss->nFiles++;		/* count non-dirs on first encounter */
    } END_CRIT;

    /* If there is some problem, remember that we noticed so we can delete the
     * file later. */
    if (code && (ss->typeLinkCounts[ss->anodeIx] == EPIZ_SS_NOCNT))
	ss->typeLinkCounts[ss->anodeIx] = EPIZ_SS_BADCNT;

decorate_name:
    /*
     * Since we are now done visiting this file, before we move on, lets check
     * to see if we can decorate any names.  note: just cause we can't open
     * this file, doesn't mean we can't decorate the name.  Use dirPath is
     * available because if the entry is directory it will have a trailing
     * slash.
     */

    /* BEGIN */ {
	struct epis_path *bestPath = dirPath;
	if (!bestPath) bestPath = path;
	if (code) {
	    /* If there's an error force memory of this anode */
	    (void) epiz_GetAnode (ss->anodeIx, ss->volIx);
	}
    	epis_DecorateNames (ss->anodeIx, ss->volIx, bestPath);
	if (dirPath) {
	    epis_FreeNameElem (dirPath);
	}
    }

    return code;
}
