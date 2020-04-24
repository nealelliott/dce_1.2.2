/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fileDir.c,v $
 * Revision 1.1.415.1  1996/10/17  18:25:47  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:58  damon]
 *
 * Revision 1.1.410.1  1994/02/04  20:43:48  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:49  devsrc]
 * 
 * Revision 1.1.408.1  1993/12/07  17:43:39  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:11:40  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:29:50  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:07:39  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  17:39:47  jaffe
 * 	New file
 * 	[1992/10/27  20:00:59  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fileDir.c -- 
 *    Functions to implement the variable type for the directory entry
 *    structure used by the opendir, readdir, etc. calls.
 */

#include <sys/types.h>
#include <dirent.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_fileDir.h>

int itlFile_dirDataType;

PRIVATE itlValue_varTypeInfo_t dirVarType = {
    "fs_dirent",
    itlFile_DirCopy,
    itlFile_DirNew,
    itlFile_DirFree,
    NULL,
    itlFile_DirBinaryOps,
    NULL,
    NULL,
    NULL,
    itlFile_DirDisplay,
    itlFile_DirGetField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *direntFieldNames[] = {
#ifndef SUNOS5
    "d_ino",
    "d_reclen",
    "d_namlen",
    "d_name"
#else
    "d_ino",
    "d_off",
    "d_reclen",
    "d_name"
#endif
};

/*
 *  DirInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void DirInit()
{
    itlFile_dirDataType = itlValue_RegisterVarType(&dirVarType);
}

/*
 * itlFile_DirInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
SHARED void itlFile_DirInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    /*
     *  First ensure the hash table module is initialized.
     */
    itlu_NameHashInit();

    if (pthread_once(&hashOnce, (pthread_initroutine_t) DirInit))
	itl_PrintError("pthread_once failed in DirInit\n");
}

SHARED var_val_t *itlFile_DirNew(newDirP, newType)
  IN pointer_t newDirP;
  IN int newType;
{
    var_val_t *newVarP;
    struct dirent *dirEntryP = (struct dirent *) newDirP;

    ITL_UTIL_NEW(newVarP, var_val_t);
    VALUE_TYPE(newVarP) = newType;
    VALUE_VAL(newVarP) = newDirP;
    VALUE_SIZE(newVarP) = (int) dirEntryP->d_reclen;
    return(newVarP);
}

SHARED boolean_t itlFile_DirFree(freeDirP)
  IN var_val_t *freeDirP;
{
    if (VALUE_VAL(freeDirP))
	ITL_UTIL_FREE(VALUE_VAL(freeDirP));
    return(FALSE);
}

SHARED var_val_t *itlFile_DirCopy(copyDirP)
  IN var_val_t *copyDirP;
{
    var_val_t *newCopyP;
    struct dirent *dirP;

    newCopyP = itl_NewVarVal();
    ITL_UTIL_ALLOC(dirP, struct dirent *, VALUE_SIZE(copyDirP));
    VALUE_SIZE(newCopyP) = VALUE_SIZE(copyDirP);
    VALUE_TYPE(newCopyP) = VALUE_TYPE(copyDirP);
    VALUE_VAL(newCopyP) = dirP;
    ITL_UTIL_MEMCOPY(VALUE_VAL(copyDirP), VALUE_VAL(newCopyP), 
		     VALUE_SIZE(copyDirP));
    return(newCopyP);
}

SHARED char *itlFile_DirDisplay(displayDirP, lengthP)
  IN var_val_t *displayDirP;
  OUT int *lengthP;
{
    char *returnStr;
    struct dirent *dirP;
    unsigned int strLength;

    dirP = (struct dirent *) VALUE_VAL(displayDirP);
    /*
     *  In the calculation for how much space we need, we just assume
     *  10 bytes for the d_ino, d_reclen, and d_namlen fields.
     */
#ifndef	SUNOS5
    strLength = dirP->d_namlen  +   /* length of the directory name */
#else
    strLength = strlen(dirP->d_name)  +   /* length of the directory name */
#endif
	        10 + 10 + 10    +   /* d_ino, d_reclen, and d_namlen values */
	        10              +   /* "d_ino = " title and ", " */
	        13              +   /* "d_reclen = " title and ", " */
 	        12              +   /* "d_namlen = " title and newline */
	        10              +   /* "d_name = " title and newline */
	        2;                  /* for the '{' and '}' */
#ifndef SUNOS5
    ITL_UTIL_ALLOC(returnStr, char *, strLength);
    sprintf(returnStr, "{d_ino = %d, d_reclen = %d, d_namlen = %d\nd_name =  %s}\n",
	    dirP->d_ino, dirP->d_reclen, dirP->d_namlen, dirP->d_name);

#else
    ITL_UTIL_ALLOC(returnStr, char *, strLength);
    sprintf(returnStr, "{d_ino = %d, d_reclen = %d\nd_name =  %s}\n",
	    dirP->d_ino, dirP->d_reclen, dirP->d_name);
#endif

    if (lengthP)
	*lengthP = strLength;

    return(returnStr);
}

SHARED var_val_t *itlFile_DirGetField(dirVarP, fieldName)
  IN var_val_t *dirVarP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum, convertVal;
    struct dirent *dirP;

    dirP = (struct dirent *) VALUE_VAL(dirVarP);

    if (itl_LookupTable(fieldName, direntFieldNames,
			sizeof(direntFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* d_ino */
	    retValP = itlStds_IntVal(dirP->d_ino);
	    break;
#ifndef	SUNOS5
	  case 1:   /* d_reclen */
	    retValP = itlStds_IntVal(dirP->d_reclen);
	    break;
	  case 2:   /* d_namlen */
	    retValP = itlStds_IntVal(dirP->d_namlen);
	    break;
	  case 3:   /* d_name */
	    retValP = itlStds_StringVal(dirP->d_name);
	    break;
#else
	  case 1:   /* d_off */
	    retValP = itlStds_IntVal(dirP->d_off);
	    break;
	  case 2:   /* d_reclen */
	    retValP = itlStds_IntVal(dirP->d_reclen);
	    break;
	  case 3:   /* d_name */
	    retValP = itlStds_StringVal(dirP->d_name);
	    break;
#endif
	}
    }
    return(retValP);
}

SHARED boolean_t itlFile_DirBinaryOps(leftValP, rightValP, opStr)
  INOUT var_val_t *leftValP;
  IN var_val_t *rightValP;
  IN char *opStr;
{
    itl_Error("Binary operatons on dirent's not currently implemented.\n");
}
