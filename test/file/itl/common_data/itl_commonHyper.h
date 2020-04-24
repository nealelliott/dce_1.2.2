/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1996, 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_commonHyper.h -- 
 *    The definitions and declarations for the afs_hyper_t data type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/common_data/itl_commonHyper.h,v 1.1.316.1 1996/10/17 18:25:25 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_commonHyper.h,v $
 * Revision 1.1.316.1  1996/10/17  18:25:25  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:48  damon]
 *
 * $EndLog$
 */

#ifndef ITL_COMMON_HYPER_H
#define ITL_COMMON_HYPER_H

#include <utils/itl/itl_stds.h>

extern int itlCommon_hyperDataType;

/*
 *  External function declarations.
 */

extern void itlCommon_HyperInit _TAKES((
					void
					));

extern void itlCommon_HyperGet _TAKES((
				       OUT afs_hyper_t *hyperDataP
				       ));

extern var_val_t *itlCommon_HyperCreate _TAKES((
						IN unsigned long high,
						IN unsigned long low
						));

extern boolean_t itlCommon_HyperCompare _TAKES((
						IN afs_hyper_t *leftP,
						IN afs_hyper_t *rightP
						));

extern char *itlCommon_HyperDisplay _TAKES((
					    IN afs_hyper_t *displayHyperP,
					    OUT int *lengthP
					    ));

#endif  /* ITL_COMMON_HYPER_H */
