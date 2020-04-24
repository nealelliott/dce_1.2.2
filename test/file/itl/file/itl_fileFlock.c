/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fileFlock.c,v $
 * Revision 1.1.116.1  1996/10/17  18:25:50  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:00  damon]
 *
 * Revision 1.1.111.1  1994/02/04  20:43:51  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:50  devsrc]
 * 
 * Revision 1.1.109.1  1993/12/07  17:43:41  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:11:57  jaffe]
 * 
 * Revision 1.1.9.2  1993/07/19  19:41:47  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:31:46  zeliff]
 * 
 * Revision 1.1.7.3  1993/07/16  21:53:55  kissel
 * 	Put HP RP work on the 1.0.3i branch.
 * 	[1993/06/21  15:04:46  kissel]
 * 
 * Revision 1.1.5.2  1993/06/04  19:04:23  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:43:33  kissel]
 * 
 * Revision 1.1.3.2  1993/02/26  18:01:47  kissel
 * 	Make HPUX be like OSF for flock header files.
 * 	[1993/02/26  17:57:20  kissel]
 * 
 * Revision 1.1.2.4  1993/01/13  21:08:12  shl
 * 	Transarc delta: fred-ot6406-require-lock-var-for-fcntl 1.3
 * 	  Selected comments:
 * 	    To fix the fcntl function to require a lock variable be supplied for
 * 	    locking operations.
 * 	    This is just a checkpoint.  Got a more important defect to fix.
 * 	    Put finishing touches on this delta.
 * 	    Cleanup the read function so it will work correctly when called to set
 * 	    one lock variable from another.
 * 	    Fixed the compile problem with flock.h on pmax.
 * 	    Include fcntl.h on pmax to get definition of struct flock.
 * 	[1993/01/13  14:02:58  shl]
 * 
 * Revision 1.1.2.3  1993/01/11  18:29:55  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:07:56  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  17:47:42  jaffe
 * 	New file
 * 	[1992/10/27  22:21:28  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fileFlock.c -- 
 *    Functions to implement the variable type for the struct flock
 *    type used by fcntl().
 */

#include <sys/types.h>
#include <sys/file.h>

#if defined(__OSF1__) || defined(SUNOS5) || defined(__hpux)
#include <fcntl.h>   /* struct flock defined here */
#else
#include <sys/flock.h>
#endif /* defined(__OSF1__) || defined(SUNOS5) || defined(__hpux) */

#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_file.h>
#include <utils/itl/itl_fileUtils.h>
#include <utils/itl/itl_fileFlock.h>
    
int itlFile_flockDataType;

PRIVATE var_val_t *FlockCopy _TAKES((
				     IN var_val_t *copyFlockP
				     ));

PRIVATE var_val_t *FlockNew _TAKES((
				    IN pointer_t newFlockP,
				    IN int newType
				    ));

PRIVATE boolean_t FlockFree _TAKES((
				    IN var_val_t *freeFlockP
				    ));

PRIVATE var_val_t *FlockRead _TAKES((
				     IN var_val_t *flockValP,
				     IN int readType
				     ));

PRIVATE char *FlockDisplay _TAKES((
				   IN var_val_t *displayFlockP,
				   OUT int *lengthP
				   ));

PRIVATE var_val_t *FlockGetField _TAKES((
					 IN var_val_t *flockP,
					 IN char *fieldName
					 ));

PRIVATE boolean_t FlockSetField _TAKES((
					IN var_val_t *flockValP,
					IN char *fieldName,
					IN var_val_t *fieldValP
					));


PRIVATE itlValue_varTypeInfo_t flockVarType = {
    "fs_flock",
    FlockCopy,
    FlockNew,
    FlockFree,
    FlockRead,
    NULL,
    NULL,
    NULL,
    NULL,
    FlockDisplay,
    FlockGetField,
    FlockSetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *flockFieldNames[] = {
    "l_type",
    "l_whence",
    "l_start",
    "l_len",
    "l_pid",

#ifdef _AIX
    "l_sysid",
    "l_vfs",
#endif  /* _AIX */

    ""
};

PRIVATE itlu_valueMap_t flockTypeFlags[] = {
    {"F_RDLCK",  F_RDLCK},
    {"F_WRLCK",  F_WRLCK},
    {"F_UNLCK",  F_UNLCK},
    {NULL,       0}
};

/*
 *  FlockInit() --
 *    To be called via pthread_once to do the real initialization.
 */
PRIVATE void FlockInit()
{
    long returnCode;

    if (returnCode = itlu_ProcessValueMap(flockTypeFlags, NULL))
    {
	itl_Error("itlu_ProcessValueMap FAILED - %ld\n", returnCode);
	exit(-1);
    }

    /*
     *  We can safely set itlFile_flockDataType because this function is 
     *  called via pthread_once.
     */
    itlFile_flockDataType = itlValue_RegisterVarType(&flockVarType);
}

/*
 * itlFile_FlockInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
SHARED void itlFile_FlockInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) FlockInit))
	itl_PrintError("pthread_once failed in FlockInit\n");
}

PRIVATE var_val_t *FlockCopy(copyFlockP)
  IN var_val_t *copyFlockP;
{
    struct flock *flockP = (struct flock *) VALUE_VAL(copyFlockP);
    struct flock *newFlockP;

    assert(VALUE_TYPE(copyFlockP) == itlFile_flockDataType);
    ITL_NEW(newFlockP, struct flock);
    *newFlockP = *flockP;
    return(FlockNew((pointer_t) newFlockP, itlFile_flockDataType));
}

PRIVATE var_val_t *FlockNew(newFlockP, newType)
  IN pointer_t newFlockP;
  IN int newType;
{
    var_val_t *newVarP;
    struct flock *flockEntryP = (struct flock *) newFlockP;

    ITL_UTIL_NEW(newVarP, var_val_t);
    VALUE_TYPE(newVarP) = newType;
    VALUE_VAL(newVarP) = newFlockP;
    VALUE_SIZE(newVarP) = (int) sizeof(struct flock);
    return(newVarP);
}

PRIVATE boolean_t FlockFree(freeFlockP)
  IN var_val_t *freeFlockP;
{
    if (VALUE_VAL(freeFlockP))
	ITL_UTIL_FREE(VALUE_VAL(freeFlockP));
    return(FALSE);
}


PRIVATE var_val_t *FlockRead(flockValP, readType)
  IN var_val_t *flockValP;
  IN int readType;
{
    var_val_t *newValP;
    struct flock *newFlockP;

    assert(readType == itlFile_flockDataType);

    if (!flockValP)
    {
	/*
	 *  Prompt for first field.  If value returned is a flock variable
	 *  we are done.  Otherwise, we need to get each of the fields.
	 */
	itl_PromptUser("Lock Type ($F_{RD, WR, UN}LCK flags): ");
	flockValP = itl_GetVarVal();

	if (VALUE_TYPE(flockValP) == itlFile_flockDataType)
	    newValP = flockValP;
	else
	{
	    /*
	     *  Allocate an flock strucutre and fill in the fields.
	     */
	    ITL_UTIL_NEW(newFlockP, struct flock);
	    (void) memset((char *) newFlockP, 0, sizeof(struct flock));
	    newValP = itlValue_ConvertValue(flockValP, itl_intType);
	    assert(newValP);
	    newFlockP->l_type = (short) (*((int *) VALUE_VAL(newValP)));
	    itl_FreeVarVal(newValP);
	    itl_PromptUser("Lock Whence (choose $SEEK_SET, $SEEK_CUR, or $SEEK_END): ");
	    newFlockP->l_whence = (short) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Lock Start: ");
	    newFlockP->l_start = (off_t) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Lock Length: ");
	    newFlockP->l_len = (off_t) itl_GetInteger((boolean_t *) 0);

	    /*
	     *  Force the rest of the fields to be 0.
	     */
	    newFlockP->l_pid = 0;

#ifdef _AIX
	    newFlockP->l_sysid = (long) 0;
	    newFlockP->l_vfs = 0;
#endif  /* _AIX */

	    newValP = FlockNew((pointer_t) newFlockP, itlFile_flockDataType);
	}
    }
    else
    {
	itl_FreeVarVal(flockValP);
	newValP = (var_val_t *) 0;
	itl_AdviseUser("Code not expecting this to happen.\n");
    }

    return(newValP);
}

PRIVATE char *FlockDisplay(displayFlockP, lengthP)
  IN var_val_t *displayFlockP;
  OUT int *lengthP;
{
    char *returnStr, *whenceStr, *typeStr;
    struct flock *flockP;
    unsigned int strLength;

    flockP = (struct flock *) VALUE_VAL(displayFlockP);

    /*
     *  The calculations below are just estimates.  The values for each
     *  field include the estimate of size, plus the space to hold the
     *  field name, a space, an equals sign, another space, and a comma in 
     *  the output string.
     */
    strLength = 16  +   /* l_type */
	        20  +   /* l_whence */
	        18  +   /* l_start */
	        16  +   /* l_len */
	        16  +   /* l_pid */
                10;     /* slack for safety */

#ifdef _AIX

    strLength += 23 +   /* l_sysid */
	         19;    /* l_vfs */

#endif  /* _AIX */

    ITL_UTIL_ALLOC(returnStr, char *, strLength);

    /*
     *  Convert the type and whence fields to their string values.
     */
    typeStr = itlFile_FlagsToStr(flockTypeFlags, (long) flockP->l_type, FALSE);
    whenceStr = itlFile_FlagsToStr(seekFlags, (long) flockP->l_whence, FALSE);

    /*
     *  We should be able to use the result of sprintf, given
     *  sprintf returns the length of the string.  But CMA appears to
     *  returning the string pointer, so we'll do the safe thing and just
     *  do strlen().
     */
    sprintf(returnStr, "{l_type = %s, l_whence = %s\n", typeStr, whenceStr);
    sprintf(&(returnStr[strlen(returnStr)]), 
	    " l_start = %ld, l_len = %ld, l_pid = %d\n",
	    flockP->l_start, flockP->l_len, flockP->l_pid);

#ifdef _AIX

    sprintf(&(returnStr[strlen(returnStr)]), 
	    " l_sysid = %ld, l_vfs = %d\n", flockP->l_sysid, flockP->l_vfs);

#endif  /* _AIX */

    ITL_UTIL_FREE(typeStr);
    ITL_UTIL_FREE(whenceStr);

    if (lengthP)
	*lengthP = strLength;

    return(returnStr);
}

PRIVATE var_val_t *FlockGetField(flockValP, fieldName)
  IN var_val_t *flockValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum, convertVal;
    struct flock *flockP;

    flockP = (struct flock *) VALUE_VAL(flockValP);

    if (itl_LookupTable(fieldName, flockFieldNames,
			sizeof(flockFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* l_type */
	    convertVal = (int) flockP->l_type;
	    break;
	  case 1:   /* l_whence */
	    convertVal = (int) flockP->l_whence;
	    break;
	  case 2:   /* l_start */
	    convertVal = (int) flockP->l_start;
	    break;
	  case 3:   /* l_len */
	    convertVal = (int) flockP->l_len;
	    break;
	  case 4:   /* l_pid */
	    convertVal = (int) flockP->l_pid;
	    break;

#ifdef _AIX

	  case 5:   /* l_sysid */
	    convertVal = (int) flockP->l_sysid;
	    break;
	  case 6:   /* l_vfs */
	    convertVal = (int) flockP->l_vfs;
	    break;

#endif  /* _AIX */

	}
	retValP = itlStds_IntVal(convertVal);
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retValP);
}

PRIVATE boolean_t FlockSetField(flockValP, fieldName, fieldValP)
  IN var_val_t *flockValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    struct flock *flockP;
    boolean_t retValue = TRUE;

    /*
     *  Currently, the only type of value that any of these fields can
     *  be assigned from is an ITL integer.  So if the type of the
     *  fieldValP, is not integer, we return FALSE.
     */
    if (VALUE_TYPE(fieldValP) == itl_intType)
    {
	flockP = (struct flock *) VALUE_VAL(flockValP);

	if (itl_LookupTable(fieldName, flockFieldNames,
			    sizeof(flockFieldNames) / sizeof(char *),
			    &fieldNum) == 1)
	{
	    switch (fieldNum) {
	      case 0:   /* l_type */
		flockP->l_type = (short) (*((int *) VALUE_VAL(fieldValP)));
		break;
	      case 1:   /* l_whence */
		flockP->l_whence = (short) (*((int *) VALUE_VAL(fieldValP)));
		break;
	      case 2:   /* l_start */
		flockP->l_start = (off_t) (*((int *) VALUE_VAL(fieldValP)));
		break;
	      case 3:   /* l_len */
		flockP->l_len = (off_t) (*((int *) VALUE_VAL(fieldValP)));
		break;
	      default:  /* no other fields are setable by the user */
		itl_Error("Field, %s, is not setable.\n", fieldName);
		retValue = FALSE;
	    }
	}
    }
    else
	retValue = FALSE;

    return(retValue);
}

/*
 *  itlFile_FlockGet() --
 *    Will prompt for and obtain the value of a fs_flock variable.
 *
 *  Note:
 *    A pointer is returned to the flock structure within the ITL
 *    variable, because the semantics of these locks are that fields in
 *    this structure will be updated by calls to fcntl().  So passing
 *    back a copy of the structure would not allow for updating of the
 *    variable from which the data was gotten.
 */
SHARED struct flock *itlFile_FlockGet()
{
    itl_var_t *varP;
    var_val_t *flockVarP;
    struct flock *retValP;
    char *varName;
    boolean_t err;

    itl_PromptUser("Variable Name (fs_flock): ");
    varName = itl_GetVariableName(&err, NULL);
    if ((varP = itl_LocateVariable(varName)) != (itl_var_t *) 0)
    {
	flockVarP = VAR_VALUE_REC(varP);
	retValP = (struct flock *) VALUE_VAL(flockVarP);
    }
    else
	retValP = (struct flock *) 0;

    ITL_FREE(varName);
    return(retValP);
}
