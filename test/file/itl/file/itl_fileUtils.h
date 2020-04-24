/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fileUtils.h,v $
 * Revision 1.1.8.1  1996/10/17  18:26:02  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:07  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:30:13  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:08:35  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  17:48:38  jaffe
 * 	New file
 * 	[1992/10/27  22:27:37  jaffe]
 * 
 * $EndLog$
 */
/*
 * itl_fileUtils.h -- 
 *    Utility functions to support the functions in itl_file.c.
 */

#ifndef ITL_FILE_UTILS_H
#define ITL_FILE_UTILS_H

#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_io.h>

#define ITL_FILE_INVALID_ID   -2   /* -1 is valid, see chown(2) */

/*
 *  The itlFile_idType_t types are forced to start at 1 so that 0 can
 *  be used for special cases.  For example the code to implement the
 *  chown(2) command uses a 0 parameter to indicate it is to change
 *  both the user and group id on a file.
 */
typedef enum {
    UserId = 1,
    GroupId = 2
} itlFile_idType_t;


extern void itlFile_utilInit _TAKES((
				     void
				     ));

extern char *itlFile_FlagsToStr _TAKES((
					IN itlu_valueMap_t *flags,
					IN long flagData,
					IN boolean_t bitMask
					));

extern int itlFile_GetIdentifier _TAKES((
					 itlFile_idType_t idType
					 ));

extern void itlFile_SetWorkingDir _TAKES((
					  IN char *newDir
					  ));

extern void itlFile_GetWorkingDir _TAKES((
					  OUT char **dirP,
					  OUT unsigned int *lengthP
					  ));

extern void itlFile_BuildFullPathName _TAKES((
					      IN char *nameP,
					      OUT char **uniqueNameP
					      ));


#endif /* ITL_FILE_UTILS_H */

