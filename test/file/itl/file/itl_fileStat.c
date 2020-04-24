/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fileStat.c,v $
 * Revision 1.1.117.1  1996/10/17  18:25:54  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:02  damon]
 *
 * Revision 1.1.112.1  1994/02/04  20:43:53  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:51  devsrc]
 * 
 * Revision 1.1.110.1  1993/12/07  17:43:43  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:12:35  jaffe]
 * 
 * Revision 1.1.9.2  1993/07/19  19:41:58  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:31:53  zeliff]
 * 
 * Revision 1.1.7.3  1993/07/16  21:54:10  kissel
 * 	Put HP RP work on the 1.0.3i branch.
 * 	[1993/06/21  15:05:19  kissel]
 * 
 * Revision 1.1.5.2  1993/06/04  19:03:00  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:43:57  kissel]
 * 
 * Revision 1.1.3.2  1993/02/26  18:02:00  kissel
 * 	Porting modifications for HPUX: eliminate st_flags and st_gen, just like Sun.
 * 	[1993/02/26  17:57:46  kissel]
 * 
 * Revision 1.1.2.4  1993/01/11  18:29:59  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:08:05  htf]
 * 
 * Revision 1.1.2.3  1992/11/18  21:45:21  jaffe
 * 	Transarc delta: itl-sanzi-initial-spoke-library 1.1
 * 	  Selected comments:
 * 	    Addition of Spoke Library to the suite of ITL libraries.
 * 	    Port of file_test and its libs to the Sun.
 * 	    Port this file to run on the Sun.
 * 	[1992/11/18  14:50:14  jaffe]
 * 
 * Revision 1.1.2.2  1992/10/28  17:47:56  jaffe
 * 	New file
 * 	[1992/10/27  22:22:49  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fileStat.c -- 
 *    Functions to implement the variable type for the structure
 *    returned by the stat() system call.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_fileStat.h>

int itlFile_statDataType;

PRIVATE itlValue_varTypeInfo_t statVarType = {
    "fs_stat",
    itlFile_StatCopy,
    itlFile_StatNew,
    itlFile_StatFree,
    NULL,
    itlFile_StatBinaryOps,
    NULL,
    NULL,
    NULL,
    itlFile_StatDisplay,
    itlFile_StatGetField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
    };

/*
 *  XXX -
 *    Don't support the following fields, yet.
 *    SUNOS4:
 *      st_spare4[2]
 *    SUNOS5:
 *      st_pad1[3]
 *      st_pad2[3]
 *      st_pad3
 *      st_pad4[8]
 */
PRIVATE char *statFieldNames[] = {
    "st_dev",
    "st_ino",
    "st_mode",
    "st_nlink",
    "st_uid",
    "st_gid",
    "st_rdev",
    "st_size",
    "st_atime",
    "st_mtime",
    "st_ctime",
    "st_blksize",
    "st_blocks",
#if defined(_AIX) || defined(_OSF1_) || defined(SUNOS4) || defined(__hpux)
    "st_spare1",
    "st_spare2",
    "st_spare3",
#endif  /* _AIX || _OSF1_ || SUNOS4 */
#if defined(_AIX) || defined(_OSF1_)
    "st_flags",     /* AIX uses st_flag and OSF/1 uses st_flags so both are */
    "st_flag",      /* defined so either will work in a script. */
    "st_gen"
#endif  /* _AIX || _OSF1_ */
#if defined(SUNOS5)
    "st_fstype"
#endif  /* SUNOS5 */
};

/*
 *  StatInit() --
 *    To be called via pthread_once to do the real initialization.
 */
PRIVATE void StatInit()
{
    /*
     *  We can safely set itlFile_statDataType because this function is called
     *  via pthread_once.
     */
    itlFile_statDataType = itlValue_RegisterVarType(&statVarType);
}

/*
 * itlFile_StatInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
SHARED void itlFile_StatInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) StatInit))
	itl_PrintError("pthread_once failed in StatInit\n");
}

SHARED var_val_t *itlFile_StatNew(newStatP, newType)
  IN pointer_t newStatP;
  IN int newType;
{
    var_val_t *newVarP;
    struct stat *statEntryP = (struct stat *) newStatP;

    ITL_UTIL_NEW(newVarP, var_val_t);
    VALUE_TYPE(newVarP) = newType;
    VALUE_VAL(newVarP) = newStatP;
    VALUE_SIZE(newVarP) = (int) sizeof(struct stat);
    return(newVarP);
}

SHARED boolean_t itlFile_StatFree(freeStatP)
  IN var_val_t *freeStatP;
{
    if (VALUE_VAL(freeStatP))
	ITL_UTIL_FREE(VALUE_VAL(freeStatP));
    return(FALSE);
}

SHARED var_val_t *itlFile_StatCopy(copyStatP)
  IN var_val_t *copyStatP;
{
    var_val_t *newCopyP;
    struct stat *statP;

    newCopyP = itl_NewVarVal();
    ITL_UTIL_ALLOC(statP, struct stat *, VALUE_SIZE(copyStatP));
    VALUE_SIZE(newCopyP) = VALUE_SIZE(copyStatP);
    VALUE_TYPE(newCopyP) = VALUE_TYPE(copyStatP);
    VALUE_VAL(newCopyP) = statP;
    ITL_UTIL_MEMCOPY(VALUE_VAL(copyStatP), VALUE_VAL(newCopyP), 
		     VALUE_SIZE(copyStatP));
    return(newCopyP);
}

SHARED char *itlFile_StatDisplay(displayStatP, lengthP)
  IN var_val_t *displayStatP;
  OUT int *lengthP;
{
    char *returnStr;
    struct stat *statP;
    unsigned int strLength;

    statP = (struct stat *) VALUE_VAL(displayStatP);

    /*
     *  The calculations below are just estimates.  The values for each
     *  field include the estimate of size, plus the space to hold the
     *  field name, a space, an equals sign, another space, and a comma in 
     *  the output string.
     */
    strLength = 14  +   /* st_dev */
	        20  +   /* st_ino */
	        17  +   /* st_mode */
	        16  +   /* st_nlink */
	        17  +   /* st_uid */
	        17  +   /* st_gid */
	        15  +   /* st_rdev */
	        21  +   /* st_size */
	        38  +   /* st_atime */
	        17  +   /* st_spare1 */
	        38  +   /* st_mtime */
	        17  +   /* st_spare2 */
	        38  +   /* st_ctime */
	        17  +   /* st_spare3 */
	        20  +   /* st_blksize */
	        20  +   /* st_blocks */
#ifndef SUNOS5
	        22  +   /* st_flags */
	        14  +   /* st_gen */
#else
                _ST_FSTYPSZ + /* fstype */
#endif
                10;     /* slack for safety */

    ITL_UTIL_ALLOC(returnStr, char *, strLength);

    /*
     *  We should be able to use the result of sprintf, given
     *  sprintf returns the length of the string.  But CMA appears to
     *  returning the string pointer, so we'll do the safe thing and just
     *  do strlen().
     */
    sprintf(returnStr, "{dev = %d, ino = %d, mode = 0x%x, nlink = %d\n",
	    statP->st_dev, statP->st_ino, statP->st_mode, statP->st_nlink);
    sprintf(&(returnStr[strlen(returnStr)]), 
	    " uid = %d, gid = %d, rdev = %d, size = %ld\n",
	    statP->st_uid, statP->st_gid, statP->st_rdev, statP->st_size);
    /*
     *  I separated the sprinf's for the time stuff because I couldn't
     *  get %23s to work for the time string returned by ctime().  So
     *  I just subtract one from the length.
     */
    sprintf(&(returnStr[strlen(returnStr)]), " atime = %s",
	    ctime(&(statP->st_atime)));
#ifndef SUNOS5
    sprintf(&(returnStr[strlen(returnStr) - 1]), ", spare1 = %d\n", 
	    statP->st_spare1);
#endif
    sprintf(&(returnStr[strlen(returnStr)]), " mtime = %s",
	    ctime(&(statP->st_mtime)));
#ifndef SUNOS5
    sprintf(&(returnStr[strlen(returnStr) - 1]), ", spare2 = %d\n", 
	    statP->st_spare2);
#endif
    sprintf(&(returnStr[strlen(returnStr)]), " ctime = %s",
	    ctime(&(statP->st_ctime)));
#ifndef SUNOS5
    sprintf(&(returnStr[strlen(returnStr) - 1]), ", spare3 = %d\n", 
	    statP->st_spare3);
#endif

#ifdef _AIX   /* AIX uses st_flag not st_flags */

    sprintf(&(returnStr[strlen(returnStr)]), 
	    " blksize = %ld, blocks = %ld, flags = %ld, gen = %ld}\n",
	    statP->st_blksize, statP->st_blocks, statP->st_flag, 
	    statP->st_gen);

#elif defined(__OSF1__)   /* _AIX */

    sprintf(&(returnStr[strlen(returnStr)]), 
	    " blksize = %ld, blocks = %ld, flags = %ld, gen = %ld}\n",
	    statP->st_blksize, statP->st_blocks, statP->st_flags, 
	    statP->st_gen);

#elif defined(SUNOS5)  /* _AIX */

    statP->st_fstype[_ST_FSTYPSZ-1] = '\0';
    sprintf(&(returnStr[strlen(returnStr)]), 
	    " blksize = %d, blocks = %d, fstype = %s}\n",
	    statP->st_blksize, statP->st_blocks, statP->st_fstype);

#elif defined(SUNOS4)   /* _AIX */

    sprintf(&(returnStr[strlen(returnStr)]), 
	    " blksize = %ld, blocks = %ld, flags = %ld, gen = %ld}\n",
	    statP->st_blksize, statP->st_blocks, 0, 0);

#endif  /* _AIX */

    if (lengthP)
	*lengthP = strLength;

    return(returnStr);
}

SHARED var_val_t *itlFile_StatGetField(statValP, fieldName)
  IN var_val_t *statValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum, convertVal;
    struct stat *statP;

    statP = (struct stat *) VALUE_VAL(statValP);

    if (itl_LookupTable(fieldName, statFieldNames,
			sizeof(statFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* st_dev */
	    convertVal = (int) statP->st_dev;
	    break;
	  case 1:   /* st_ino */
	    convertVal = (int) statP->st_ino;
	    break;
	  case 2:   /* st_mode */
	    convertVal = (int) statP->st_mode;
	    break;
	  case 3:   /* st_nlink */
	    convertVal = (int) statP->st_nlink;
	    break;
	  case 4:   /* st_uid */
	    convertVal = (int) statP->st_uid;
	    break;
	  case 5:   /* st_gid */
	    convertVal = (int) statP->st_gid;
	    break;
	  case 6:   /* st_rdev */
	    convertVal = (int) statP->st_rdev;
	    break;
	  case 7:   /* st_size */
	    convertVal = (int) statP->st_size;
	    break;
	  case 8:   /* st_atime */
	    convertVal = (int) statP->st_atime;
	    break;
	  case 9:   /* st_mtime */
	    convertVal = (int) statP->st_mtime;
	    break;
	  case 10:   /* st_ctime */
	    convertVal = (int) statP->st_ctime;
	    break;
	  case 11:   /* st_blksize */
	    convertVal = (int) statP->st_blksize;
	    break;
	  case 12:   /* st_blocks */
	    convertVal = (int) statP->st_blocks;
	    break;

#if defined(_AIX) || defined(_OSF1_) || defined(SUNOS4) || defined(__hpux)

	  case 13:   /* st_spare1 */
	    convertVal = (int) statP->st_spare1;
	    break;
	  case 14:   /* st_spare2 */
	    convertVal = (int) statP->st_spare2;
	    break;
	  case 15:   /* st_spare3 */
	    convertVal = (int) statP->st_spare3;
	    break;

#endif  /* _AIX || _OSF1_ || SUNOS4 || __hpux*/

#if defined(_AIX) || defined(_OSF1_)

	  case 16:   /* st_flags or */
	  case 17:   /* st_flags */
#ifdef _AIX
	    convertVal = (int) statP->st_flag;
#else   /* _AIX */
	    convertVal = (int) statP->st_flags;
#endif  /* _AIX */
	    break;
	  case 18:   /* st_gen */
	    convertVal = (int) statP->st_gen;
	    break;

#endif  /* _AIX || _OSF1_ */

#if defined(SUNOS5)

	  case 13:   /* st_fstype */
    	    statP->st_fstype[_ST_FSTYPSZ-1] = '\0';
	    retValP = itlStds_StringVal(statP->st_fstype);
	    break;

#endif  /* SUNOS5 */

	}
        if (!retValP)
		retValP = itlStds_IntVal(convertVal);
    }
    else
    {
	itl_Error("Field, %s, is invalid. Field table lookup FAILED.\n",
		  fieldName);
    }

    return(retValP);
}

SHARED boolean_t itlFile_StatBinaryOps(leftValP, rightValP, opStr)
  INOUT var_val_t *leftValP;
  IN var_val_t *rightValP;
  IN char *opStr;
{
    itl_Error("Binary operatons on struct stat not currently implemented.\n");
}
