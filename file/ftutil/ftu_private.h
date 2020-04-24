/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftu_private.h,v $
 * Revision 1.1.27.1  1996/10/02  17:48:45  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:36  damon]
 *
 * $EndLog$
 */
/*
 * (C) Copyright 1996, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * $Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftu_private.h,v 1.1.27.1 1996/10/02 17:48:45 damon Exp $
 */

#ifndef	_TRANSARC_DFS_FTU_PRIVATE_H
#define	_TRANSARC_DFS_FTU_PRIVATE_H

#include <dcedfs/icl.h>

/*
 * External declarations
 */

SHARED long vols_DumpVolume _TAKES((
    IN ftu_fsetDesc_t		fsetDesc,
    IN ftu_dumpCriteria_t*	dumpCriteriaP,
    IN afsPipe_t*		pipeP,
    IN afs_hyper_t*		aggrType,
    IN ftu_kaProc_t		kaProc,
    IN void*			kaArg
));

SHARED long vols_RestoreVolume _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN afs_hyper_t*	baseTypeP,
    IN afsPipe_t*	afsPipeP,
    IN unsigned long	restoreFlags,
    IN ftu_kaProc_t	kaProc,
    IN void*		kaArg
));

SHARED struct icl_set*	_ftu_iclSetP;

/*
 * Utilities
 */
#define CALL_KA_PROC(kaProc, kaArg)	\
MACRO_BEGIN				\
    if (kaProc)				\
	(*kaProc)(kaArg);		\
MACRO_END

#define HIGH_ID(fsetId)	AFS_hgethi(fsetId)
#define LOW_ID(fsetId)	AFS_hgetlo(fsetId)

#endif	/* _TRANSARC_DFS_FTU_PRIVATE_H */

