/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fileStatfs.h,v $
 * Revision 1.1.8.1  1996/10/17  18:25:59  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:05  damon]
 *
 * Revision 1.1.2.5  1993/01/13  21:08:18  shl
 * 	Transarc delta: fred-ot6414-use-dcedfs-not-afs 1.2
 * 	  Selected comments:
 * 	    Change <afs/stds.h> to <dcedfs/stds.h>.
 * 	    Ditto for the file below.
 * 	    See above.
 * 	[1993/01/13  14:04:32  shl]
 * 
 * Revision 1.1.2.4  1993/01/11  18:30:07  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:08:19  htf]
 * 
 * Revision 1.1.2.3  1992/11/24  21:45:02  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:44:42  bolinger]
 * 
 * Revision 1.1.2.2  1992/10/28  17:48:25  jaffe
 * 	Transarc delta: jess-itl-add-statfs 1.1
 * 	  Selected comments:
 * 	    Add statfs function call to the itl file interface.
 * 	    See above.
 * 	[1992/10/27  22:26:16  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fileStatfs.h -- 
 *    Includes declarations for functions and objects dealing with
 *    the statfs() function and the data type it deals with.
 * 
 */

#include <dcedfs/param.h> /* for ENV defs */

#ifndef ITL_FILE_STATFS_H
#define ITL_FILE_STATFS_H

#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>

#ifdef AFS_OSF_ENV
#include <sys/mount.h> 	/* osf1 */
#endif /* AFS_OSF_ENV */

#ifdef AFS_SUN_ENV 
#include <sys/vfs.h>    /* sun */
#endif /* AFS_SUN_ENV */

#ifdef AFS_AIX_ENV
#include <sys/statfs.h> /* aix */
#endif /* AFS_AIX_ENV */


#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_value.h>

extern int itlFile_statfsDataType;   /* for comparison only */

extern void itlFile_StatfsInit _TAKES((
				    void
				    ));

extern var_val_t *itlFile_StatfsCopy _TAKES((
					  IN var_val_t *copyStatfsP
					  ));

extern var_val_t *itlFile_StatfsNew _TAKES((
					 IN pointer_t newStatfsP,
					 IN int newType
					 ));

extern boolean_t itlFile_StatfsFree _TAKES((
					 IN var_val_t *freeStatfsP
					 ));

extern boolean_t itlFile_StatfsBinaryOps _TAKES((
					      INOUT var_val_t *leftValP,
					      IN var_val_t *rightValP,
					      IN char *opStr
					      ));

extern char *itlFile_StatfsDisplay _TAKES((
				      IN var_val_t *displayStatfsP,
				      OUT int *lengthP
				      ));

extern var_val_t *itlFile_StatfsGetField _TAKES((
					      IN var_val_t *statfsValP,
					      IN char *fieldName
					      ));

#endif  /* ITL_FILE_STATFS_H */
