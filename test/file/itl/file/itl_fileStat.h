/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fileStat.h,v $
 * Revision 1.1.8.1  1996/10/17  18:25:56  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:03  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:30:01  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:08:09  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  17:48:03  jaffe
 * 	New file
 * 	[1992/10/27  22:24:13  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fileStat.h -- 
 *    Includes declarations for functions and objects dealing with
 *    the stat() function and the data type it deals with.
 * 
 */

#ifndef ITL_FILE_STAT_H
#define ITL_FILE_STAT_H

#include <dirent.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_value.h>

extern int itlFile_statDataType;   /* for comparison only */

extern void itlFile_StatInit _TAKES((
				    void
				    ));

extern var_val_t *itlFile_StatCopy _TAKES((
					  IN var_val_t *copyStatP
					  ));

extern var_val_t *itlFile_StatNew _TAKES((
					 IN pointer_t newStatP,
					 IN int newType
					 ));

extern boolean_t itlFile_StatFree _TAKES((
					 IN var_val_t *freeStatP
					 ));

extern boolean_t itlFile_StatBinaryOps _TAKES((
					      INOUT var_val_t *leftValP,
					      IN var_val_t *rightValP,
					      IN char *opStr
					      ));

extern char *itlFile_StatDisplay _TAKES((
				      IN var_val_t *displayStatP,
				      OUT int *lengthP
				      ));

extern var_val_t *itlFile_StatGetField _TAKES((
					      IN var_val_t *statValP,
					      IN char *fieldName
					      ));

#endif  /* ITL_FILE_STAT_H */
