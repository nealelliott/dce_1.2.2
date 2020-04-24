/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_commonFid.c -- 
 *    C functions for implementing the DFS file id data type.
 */
/*
 * HISTORY
 * $Log: itl_commonFid.c,v $
 * Revision 1.1.314.1  1996/10/17  18:25:19  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:45  damon]
 *
 * Revision 1.1.309.1  1994/02/04  20:43:10  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:33  devsrc]
 * 
 * Revision 1.1.307.2  1994/01/20  18:43:56  annie
 * 	added copyright header
 * 	[1994/01/20  18:40:15  annie]
 * 
 * Revision 1.1.307.1  1993/12/07  17:43:18  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:08:00  jaffe]
 * 
 * $EndLog$
 */

#include <assert.h>
#include <dcedfs/common_data.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_commonHyper.h>
#include <utils/itl/itl_commonFid.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/common_data/itl_commonFid.c,v 1.1.314.1 1996/10/17 18:25:19 damon Exp $")

int itlCommon_fidDataType;


PRIVATE var_val_t *itlCommon_FidNew _TAKES((
					IN pointer_t newFidP,
					IN int newType
					));

PRIVATE boolean_t itlCommon_FidFree _TAKES((
					IN var_val_t *freeFidP
					));

PRIVATE var_val_t *itlCommon_FidCopy _TAKES((
					 IN var_val_t *copyFidP
					 ));

PRIVATE char *itlCommon_FidDisplay _TAKES((
				       IN var_val_t *displayFidP,
				       OUT int *lengthP
				       ));


PRIVATE itlValue_varTypeInfo_t fxFidVarType = {
    "fs_fxFid",
    itlCommon_FidCopy,
    itlCommon_FidNew,
    itlCommon_FidFree,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    itlCommon_FidDisplay,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

/*
 *  FidInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void FidInit()
{
    /*
     *  We can safely set fidHashType because this function is called
     *  via pthread_once.
     */
    itlCommon_fidDataType = itlValue_RegisterVarType(&fxFidVarType);
}

/*
 * itlCommon_FidInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlCommon_FidInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) FidInit))
	itl_Error("pthread_once failed in FidInit\n");
}

/*
 *  itlCommon_FidGet() --
 *    Will prompt for and obtain the value of a fs_fxFid variable.
 */
SHARED afsFid *itlCommon_FidGet()
{
    afsFid *fidInfoP;
    var_val_t *fidVarP;

    itl_PromptUser("File Id Variable (fs_fxFid): ");
    fidVarP = itl_GetVarVal();
    assert(fidVarP);
    if (VALUE_TYPE(fidVarP) == itlCommon_fidDataType)
	fidInfoP = (afsFid *) VALUE_VAL(fidVarP);
    else 
	fidInfoP = (afsFid *) 0;

    return(fidInfoP);
}

/**
 **  fs_fxFid variable rouines.
 **/

PRIVATE var_val_t *itlCommon_FidNew(newFidP, newType)
  IN pointer_t newFidP;
  IN int newType;
{
    var_val_t *newVarP;
    afsFid *fidP = (afsFid *) newFidP;

    ITL_UTIL_NEW(newVarP, var_val_t);
    VALUE_TYPE(newVarP) = newType;
    VALUE_VAL(newVarP) = newFidP;
    VALUE_SIZE(newVarP) = (int) sizeof(afsFid);
    return(newVarP);
}

PRIVATE boolean_t itlCommon_FidFree(freeFidP)
  IN var_val_t *freeFidP;
{
    if (VALUE_VAL(freeFidP))
	ITL_UTIL_FREE(VALUE_VAL(freeFidP));
    return(FALSE);
}

PRIVATE var_val_t *itlCommon_FidCopy(copyFidP)
  IN var_val_t *copyFidP;
{
    afsFid *fidP;

    assert(VALUE_TYPE(copyFidP) == itlCommon_fidDataType);
    ITL_NEW(fidP, afsFid);
    ITL_UTIL_MEMCOPY(VALUE_VAL(copyFidP), fidP, VALUE_SIZE(copyFidP));
    return(itlCommon_FidNew((pointer_t) fidP, itlCommon_fidDataType));
}

PRIVATE char *itlCommon_FidDisplay(displayFidP, lengthP)
  IN var_val_t *displayFidP;
  OUT int *lengthP;
{
    afsFid *fidP;
    char *returnStr, *cellStr, *volumeStr;
    unsigned int strLength, index;
    int fieldLength;
    var_val_t *tempVarP;

    fidP = (afsFid *) VALUE_VAL(displayFidP);

    /*
     *  First obtain display strings for all sub-fields that are
     *  data types themselves.
     */
    tempVarP = itlValue_New((pointer_t) &(fidP->Cell), 
			    itlCommon_hyperDataType);
    assert(tempVarP);
    cellStr = itlValue_Display(tempVarP, &fieldLength);
    strLength = fieldLength;
    itl_FreeVarVal(tempVarP);
    tempVarP = itlValue_New((pointer_t) &(fidP->Volume), 
					  itlCommon_hyperDataType);
    assert(tempVarP);
    volumeStr = itlValue_Display(tempVarP, &fieldLength);
    strLength += fieldLength;
    itl_FreeVarVal(tempVarP);

    /*
     *  Lengths below are calculated assuming worst case, that is that
     *  a long (unsigned32) will take up 11 bytes in a string; 10
     *  for data and 1 for end of string character.
     *  The lengths of the labels included in the sprintf below are also
     *  included in the calculations.  Also, any space, comma, etc. characters.
     */
    strLength +=  6 +  /* cell label */
	          8 +  /* cell label */
	         43 +  /* Vnode and Unique info */
		 10;   /* padding */

    /*
     *  Length of the strings are explicitly calculated, because sprintf
     *  did not do it correctly on every platform, i.e. this is guaranteed
     *  to be portable.
     */
    ITL_UTIL_ALLOC(returnStr, char *, strLength);
    sprintf(returnStr, "\n  Cell: %s\n  Volume: %s\n", cellStr, volumeStr);
    index = strlen(returnStr);
    sprintf(&returnStr[index], "  Vnode: %10ld   Unique: %10ld\n\n", 
	    fidP->Vnode, fidP->Unique);
    assert(strlen(returnStr) < strLength);

    ITL_FREE(cellStr);
    ITL_FREE(volumeStr);

    if (lengthP)
	*lengthP = strLength;

    return(returnStr);
}


/*
 *  itlCommon_FidCreate()
 *    Given an afsFid return a pointer to a var_val_t that 
 *    contains that data.
 */
SHARED var_val_t *itlCommon_FidCreate(fidP)
  IN afsFid *fidP;
{
    var_val_t  *newVarP, *copyVarP;
    afsFid fidData;

    fidData = *fidP;
    newVarP = itlValue_New((pointer_t) &fidData, itlCommon_fidDataType);
    copyVarP = itlValue_Copy(newVarP);
    ITL_FREE(newVarP);
    return(copyVarP);
}
