/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftu_aggrfset.c,v $
 * Revision 1.1.20.1  1996/10/02  17:48:29  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:29  damon]
 *
 * Revision 1.1.15.3  1994/07/13  22:28:10  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:35:22  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:14  mbs]
 * 
 * Revision 1.1.15.2  1994/06/09  14:09:08  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:48  annie]
 * 
 * Revision 1.1.15.1  1994/02/04  20:19:35  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:32  devsrc]
 * 
 * Revision 1.1.13.2  1994/01/20  18:43:08  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:23  annie]
 * 
 * Revision 1.1.13.1  1993/12/07  17:25:32  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:05:12  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1994, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <dcedfs/stds.h>
#include <dcedfs/vol_errs.h>

#include <ftutil.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftu_aggrfset.c,v 1.1.20.1 1996/10/02 17:48:29 damon Exp $")


/*
 * The following routines are wrappers that surround the related fileset
 * routines with an open and close.  Note that they are called with fileset
 * IDs instead of fileset descriptors.
 *
 * Since these routines exclusively use other ftutil routines, they do not
 * do any tracing or logging.
 *
 */

/*
 * ftu_AggrGetFsetStatus()
 */
EXPORT long
ftu_AggrGetFsetStatus(aggrId, fsetIdP, fsetStatusP)
    IN ftu_aggrId_t		aggrId;
    IN ftu_fsetId_t*		fsetIdP;
    OUT struct vol_status*	fsetStatusP;
{
    long		code;
    volatile ftu_fsetDesc_t	fsetDesc;

    code = ftu_AggrOpenFset(aggrId, fsetIdP,
			    FTU_SYS_GET_STATUS,  VOLERR_TRANS_GETSTATUS,
			    (ftu_fsetDesc_t *)&fsetDesc);
    if (code)
	return code;

    TRY {
	if (code = ftu_FsetGetStatus(fsetDesc, fsetStatusP)) {
	    ftu_AggrAbortFsetOpen(fsetDesc);
	}
    } CATCH_ALL {
	ftu_AggrAbortFsetOpen(fsetDesc);
	RERAISE;
    } ENDTRY
    if (code)
	return code;

    if (code = ftu_AggrCloseFset(fsetDesc))
	return code;

    return 0;
}	/* ftu_AggrGetFsetStatus() */


/*
 * ftu_AggrSetFsetStatus()
 */
EXPORT long
ftu_AggrSetFsetStatus(aggrId, fsetIdP, fsetMask, fsetStatusP)
    IN ftu_aggrId_t		aggrId;
    IN ftu_fsetId_t*		fsetIdP;
    IN u_long			fsetMask;
    OUT struct vol_status*	fsetStatusP;
{
    long		code;
    volatile ftu_fsetDesc_t	fsetDesc;

    code = ftu_AggrOpenFset(aggrId, fsetIdP,
			    FTU_SYS_SET_STATUS,  VOLERR_TRANS_SETSTATUS,
			    (ftu_fsetDesc_t *)&fsetDesc);
    if (code)
	return code;

    TRY {
	if (code = ftu_FsetSetStatus(fsetDesc, fsetMask, fsetStatusP)) {
	    ftu_AggrAbortFsetOpen(fsetDesc);
	}
    } CATCH_ALL {
	ftu_AggrAbortFsetOpen(fsetDesc);
	RERAISE;
    } ENDTRY
    if (code)
	return code;

    if (code = ftu_AggrCloseFset(fsetDesc))
	return code;

    return 0;
}	/* ftu_AggrSetFsetStatus() */


/*
 * ftu_AggrCloneFset()
 */
EXPORT long
ftu_AggrCloneFset(aggrId, srcIdP, destIdP, destName, destStates)
    IN ftu_aggrId_t	aggrId;
    IN ftu_fsetId_t*	srcIdP;
    IN ftu_fsetId_t*	destIdP;
    IN char*		destName;
    IN u_long		destStates;
{
    long		code;
    volatile ftu_fsetDesc_t	srcDesc;

    code = ftu_AggrOpenFset(aggrId, srcIdP,
			    FTU_SYS_CLONE,  VOLERR_TRANS_CLONE,
			    (ftu_fsetDesc_t *)&srcDesc);
    if (code)
	return code;

    TRY {
	if (code = ftu_FsetClone(srcDesc, destIdP, destName, destStates)) {
	    ftu_AggrAbortFsetOpen(srcDesc);
	}
    } CATCH_ALL {
	ftu_AggrAbortFsetOpen(srcDesc);
	RERAISE;
    } ENDTRY
    if (code)
	return code;

    if (code = ftu_AggrCloseFset(srcDesc))
	return code;

    return 0;
}	/* ftu_AggrCloneFset() */


/*
 * ftu_AggrRecloneFset()
 */
EXPORT long
ftu_AggrRecloneFset(aggrId, fsetIdP)
    IN ftu_aggrId_t	aggrId;
    IN ftu_fsetId_t*	fsetIdP;
{
    long		code;
    volatile ftu_fsetDesc_t	fsetDesc;

    code = ftu_AggrOpenFset(aggrId, fsetIdP,
			    FTU_SYS_RECLONE,  VOLERR_TRANS_RECLONE,
			    (ftu_fsetDesc_t *)&fsetDesc);
    if (code)
	return code;

    TRY {
	if (code = ftu_FsetReclone(fsetDesc)) {
	    ftu_AggrAbortFsetOpen(fsetDesc);
	}
    } CATCH_ALL {
	ftu_AggrAbortFsetOpen(fsetDesc);
	RERAISE;
    } ENDTRY
    if (code)
	return code;

    if (code = ftu_AggrCloseFset(fsetDesc))
	return code;

    return 0;
}	/* ftu_AggrRecloneFset() */


/*
 * ftu_AggrRecloneFsetInto()
 */
EXPORT long
ftu_AggrRecloneFsetInto(aggrId, fsetIdP, backingIdP)
    IN ftu_aggrId_t	aggrId;
    IN ftu_fsetId_t*	fsetIdP;
    IN ftu_fsetId_t*	backingIdP;
{
    long		code;
    volatile ftu_fsetDesc_t	fsetDesc;

    code = ftu_AggrOpenFset(aggrId, fsetIdP,
			    FTU_SYS_RECLONE, VOLERR_TRANS_RECLONE,
			    (ftu_fsetDesc_t *)&fsetDesc);
    if (code)
	return code;

    TRY {
	if (code = ftu_FsetRecloneInto(fsetDesc, backingIdP)) {
	    ftu_AggrAbortFsetOpen(fsetDesc);
	}
    } CATCH_ALL {
	ftu_AggrAbortFsetOpen(fsetDesc);
	RERAISE;
    } ENDTRY
    if (code)
	return code;

    if (code = ftu_AggrCloseFset(fsetDesc))
	return code;

    return 0;
}	/* ftu_AggrRecloneFsetInto() */


/*
 * ftu_AggrDestroyFset()
 */
EXPORT long
ftu_AggrDestroyFset(aggrId, fsetIdP)
    IN ftu_aggrId_t	aggrId;
    IN ftu_fsetId_t*	fsetIdP;
{
    long		code;
    volatile ftu_fsetDesc_t	fsetDesc;

    code = ftu_AggrOpenFset(aggrId, fsetIdP,
			    FTU_SYS_DESTROY, VOLERR_TRANS_DELETE,
			    (ftu_fsetDesc_t *)&fsetDesc);
    if (code)
	return code;

    TRY {
	if (code = ftu_FsetDestroy(fsetDesc)) {
	    ftu_AggrAbortFsetOpen(fsetDesc);
	}
    } CATCH_ALL {
	ftu_AggrAbortFsetOpen(fsetDesc);
	RERAISE;
    } ENDTRY
    if (code)
	return code;

    if (code = ftu_AggrCloseFset(fsetDesc))
	return code;

    return 0;
}	/* ftu_AggrDestroyFset() */


/*
 * ftu_AggrDumpFset()
 */
EXPORT long
ftu_AggrDumpFset(aggrId, fsetIdP, dumpCriteriaP, fileStream)
    IN ftu_aggrId_t		aggrId;
    IN ftu_fsetId_t*		fsetIdP;
    IN ftu_dumpCriteria_t*	dumpCriteriaP;
    IN FILE*			fileStream;
{
    long		code;
    volatile ftu_fsetDesc_t	fsetDesc;

    code = ftu_AggrOpenFset(aggrId, fsetIdP, FTU_SYS_DUMP, VOLERR_TRANS_DUMP,
			    (ftu_fsetDesc_t *)&fsetDesc);
    if (code)
	return code;

    TRY {
	if (code = ftu_FsetDump(fsetDesc, dumpCriteriaP, fileStream)) {
	    ftu_AggrAbortFsetOpen(fsetDesc);
	}
    } CATCH_ALL {
	ftu_AggrAbortFsetOpen(fsetDesc);
	RERAISE;
    } ENDTRY
    if (code)
	return code;

    if (code = ftu_AggrCloseFset(fsetDesc))
	return code;

    return 0;
}	/* ftu_AggrDumpFset() */


/*
 * ftu_AggrDumpFsetToPipe()
 */
EXPORT long
ftu_AggrDumpFsetToPipe(aggrId, fsetIdP, dumpCriteriaP, pipeP)
    IN ftu_aggrId_t		aggrId;
    IN ftu_fsetId_t*		fsetIdP;
    IN ftu_dumpCriteria_t*	dumpCriteriaP;
    IN afsPipe_t*		pipeP;
{
    long		code;
    volatile ftu_fsetDesc_t	fsetDesc;

    code = ftu_AggrOpenFset(aggrId, fsetIdP, FTU_SYS_DUMP, VOLERR_TRANS_DUMP,
			    (ftu_fsetDesc_t *)&fsetDesc);
    if (code)
	return code;

    TRY {
	if (code = ftu_FsetDumpToPipe(fsetDesc, dumpCriteriaP, pipeP)) {
	    ftu_AggrAbortFsetOpen(fsetDesc);
	}
    } CATCH_ALL {
	ftu_AggrAbortFsetOpen(fsetDesc);
	RERAISE;
    } ENDTRY
    if (code)
	return code;

    if (code = ftu_AggrCloseFset(fsetDesc))
	return code;

    return 0;
}	/* ftu_AggrDumpFsetToPipe() */


/*
 * ftu_AggrRestoreFset()
 */
EXPORT long
ftu_AggrRestoreFset(aggrId, fsetIdP, fileStream, flags)
    IN ftu_aggrId_t	aggrId;
    IN ftu_fsetId_t*	fsetIdP;
    IN FILE*		fileStream;
    IN u_long		flags;
{
    long		code;
    volatile ftu_fsetDesc_t	fsetDesc;

    code = ftu_AggrOpenFset(aggrId, fsetIdP,
			    FTU_SYS_RESTORE, VOLERR_TRANS_RESTORE,
			    (ftu_fsetDesc_t *)&fsetDesc);
    if (code)
	return code;

    TRY {
	if (code = ftu_FsetRestore(fsetDesc, fileStream, flags)) {
	    ftu_AggrAbortFsetOpen(fsetDesc);
	}
    } CATCH_ALL {
	ftu_AggrAbortFsetOpen(fsetDesc);
	RERAISE;
    } ENDTRY
    if (code)
	return code;

    if (code = ftu_AggrCloseFset(fsetDesc))
	return code;

    return 0;
}	/* ftu_AggrRestoreFset() */


/*
 * ftu_AggrRestoreFsetFromPipe()
 */
EXPORT long
ftu_AggrRestoreFsetFromPipe(aggrId, fsetIdP, pipeP, flags)
    IN ftu_aggrId_t	aggrId;
    IN ftu_fsetId_t*	fsetIdP;
    IN afsPipe_t*	pipeP;
    IN u_long		flags;
{
    long		code;
    volatile ftu_fsetDesc_t	fsetDesc;

    code = ftu_AggrOpenFset(aggrId, fsetIdP,
			    FTU_SYS_RESTORE, VOLERR_TRANS_RESTORE,
			    (ftu_fsetDesc_t *)&fsetDesc);
    if (code)
	return code;

    TRY {
	if (code = ftu_FsetRestoreFromPipe(fsetDesc, pipeP, flags)) {
	    ftu_AggrAbortFsetOpen(fsetDesc);
	}
    } CATCH_ALL {
	ftu_AggrAbortFsetOpen(fsetDesc);
	RERAISE;
    } ENDTRY
    if (code)
	return code;

    if (code = ftu_AggrCloseFset(fsetDesc))
	return code;

    return 0;
}	/* ftu_AggrRestoreFsetFromPipe() */
