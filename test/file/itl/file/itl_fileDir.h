/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fileDir.h,v $
 * Revision 1.1.8.1  1996/10/17  18:25:49  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:59  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:29:53  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:07:44  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  17:39:53  jaffe
 * 	New file
 * 	[1992/10/27  22:19:18  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fileDir.h -- 
 *    Includes declarations for functions and objects dealing with
 *    the opendir, readdir, etc. functions.
 * 
 */

#ifndef ITL_FILE_DIR_H
#define ITL_FILE_DIR_H

#include <dirent.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_value.h>

extern int itlFile_dirDataType;   /* for comparison only */

extern void itlFile_DirInit _TAKES((
				    void
				    ));

extern var_val_t *itlFile_DirCopy _TAKES((
					  IN var_val_t *copyDirP
					  ));

extern var_val_t *itlFile_DirNew _TAKES((
					 IN pointer_t newDirP,
					 IN int newType
					 ));

extern boolean_t itlFile_DirFree _TAKES((
					 IN var_val_t *freeDirP
					 ));

extern boolean_t itlFile_DirBinaryOps _TAKES((
					      INOUT var_val_t *leftValP,
					      IN var_val_t *rightValP,
					      IN char *opStr
					      ));

extern char *itlFile_DirDisplay _TAKES((
				      IN var_val_t *displayDirP,
				      OUT int *lengthP
				      ));

extern var_val_t *itlFile_DirGetField _TAKES((
					      IN var_val_t *dirP,
					      IN char *fieldName
					      ));

#endif  /* ITL_FILE_DIR_H */
