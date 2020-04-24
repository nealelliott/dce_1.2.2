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
 * itl_fxFidName.c -- 
 *    Code for dealing with the afsFidName data type.
 *
 */
/*
 * HISTORY
 * $Log: itl_fxFidName.c,v $
 * Revision 1.1.319.1  1996/10/17  18:26:28  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:18  damon]
 *
 * $EndLog$
 */

#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <dcedfs/common_data.h>
#include <dcedfs/afs4int.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_commonHyper.h>
#include <utils/itl/itl_commonTimeval.h>
#include <utils/itl/itl_commonFid.h>
#include <utils/itl/itl_fxCommon.h>
#include <utils/itl/itl_fxPrincipal.h>
#include <utils/itl/itl_fxFidName.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxFidName.c,v 1.1.319.1 1996/10/17 18:26:28 damon Exp $")


int itlFX_fidNameDataType;


PRIVATE var_val_t *FidNameCopy _TAKES((
				       IN var_val_t *copyVarP
				       ));

PRIVATE var_val_t *FidNameNew _TAKES((
				      IN pointer_t newValP,
				      IN int newType
				      ));

PRIVATE boolean_t FidNameFree _TAKES((
				      IN var_val_t *freeVarP
				      ));

PRIVATE var_val_t *FidNameRead _TAKES((
				       IN var_val_t *readVarP,
				       IN int varType
				       ));

PRIVATE char *FidNameDisplay _TAKES((
				     IN var_val_t *displayVarP,
				     OUT int *lengthP
				     ));

PRIVATE var_val_t *FidNameGetField _TAKES((
					   IN var_val_t *getVarP,
					   IN char *fieldName
					   ));

PRIVATE boolean_t FidNameSetField _TAKES((
					  IN var_val_t *setVarP,
					  IN char *fieldName,
					  IN var_val_t *fieldVarP
					  ));

PRIVATE itlValue_varTypeInfo_t fidNameVarType = {
    "fs_fxFidName",
    FidNameCopy,
    FidNameNew,
    FidNameFree,
    FidNameRead,
    NULL,
    NULL,
    NULL,
    NULL,
    FidNameDisplay,
    FidNameGetField,
    FidNameSetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *fidNameFieldNames[] = {
    "fid",
    "name"
};

/*
 *  FidNameInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void FidNameInit()
{
    /*
     *  We can safely set FidNameHashType because this function is called
     *  via pthread_once.
     */
    itlFX_fidNameDataType = itlValue_RegisterVarType(&fidNameVarType);
}

/*
 * itlFX_FidNameInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlFX_FidNameInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) FidNameInit))
	itl_Error("pthread_once failed in FidNameInit\n");
}


PRIVATE var_val_t *FidNameCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    afsFidTaggedName *fidNameValP = (afsFidTaggedName *) VALUE_VAL(copyVarP);
    afsFidTaggedName *newFidNameP;

    assert(VALUE_TYPE(copyVarP) == itlFX_fidNameDataType);
    ITL_NEW(newFidNameP, afsFidTaggedName);
    *newFidNameP = *fidNameValP;
    return(FidNameNew((pointer_t) newFidNameP, itlFX_fidNameDataType));
}

PRIVATE var_val_t *FidNameNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newFidNameVarP;

    assert(newType == itlFX_fidNameDataType);
    ITL_NEW(newFidNameVarP, var_val_t);
    VALUE_TYPE(newFidNameVarP) = newType;
    VALUE_SIZE(newFidNameVarP) = sizeof(afsFidTaggedName);
    VALUE_VAL(newFidNameVarP) = newValP;
    return(newFidNameVarP);
}

PRIVATE boolean_t FidNameFree(freeVarP)
  IN var_val_t *freeVarP;
{
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}


PRIVATE var_val_t *FidNameRead(readVarP, varType)
  IN var_val_t *readVarP;
  IN int varType;
{
    var_val_t *newVarP;
    int eof, wordSize;
    char *nameStr;

    assert(varType == itlFX_fidNameDataType);
    if (!readVarP)
    {

	itl_PromptUser("File Id: ");
	readVarP = itl_GetVarVal();

	if (VALUE_TYPE(readVarP) == itlFX_fidNameDataType)
	    newVarP = readVarP;
	else
	{
	    afsFidTaggedName *newFidNameP;

	    /*
	     *  The variable we previously read had better have been an
	     *  fid (itlCommon_fidDataType) or we got problems.
	     *
	     *  NOTE:
	     *    If this code is changed to actually read in an fid and
	     *    name (for interface testing) then be sure to use the
	     *    ITL_FX_NAME_CHECK macro to verify the name.
	     */
	    assert(VALUE_TYPE(readVarP) == itlCommon_fidDataType);
	    ITL_NEW(newFidNameP, afsFidTaggedName);
	    (void) memset((char *) newFidNameP, 0, sizeof(afsFidTaggedName));
	    newFidNameP->fid = *((afsFid *) VALUE_VAL(readVarP));
	    itl_PromptUser("Name: ");
	    nameStr = itl_GetString(&eof, &wordSize);
	    if (wordSize < AFS_NAMEMAX)
	    {
		strcpy((char *) newFidNameP->name.tn_chars, nameStr);
		newVarP = FidNameNew((pointer_t) newFidNameP,
				     itlFX_fidNameDataType);
	    }
	    else 
	    {
		ITL_FX_NAME_ERROR(nameStr);
		itl_FreeVarVal(readVarP);
		newVarP = (var_val_t *) 0;
	    }
	    ITL_FREE(nameStr);
	}
    }
    else
    {
	itl_FreeVarVal(readVarP);
	newVarP = (var_val_t *) 0;
	itl_AdviseUser("Code not expecting this to happen.\n");
    }
    return(newVarP);
}

PRIVATE char *FidNameDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    char *displayStr;
    char *fidStr;
    int strLength, fieldLength;
    afsFidTaggedName *fidNameValP;
    var_val_t *tempVarP;

    fidNameValP = (afsFidTaggedName *) VALUE_VAL(displayVarP);

    /*
     *  First obtain display strings for all sub-fields that are
     *  data types themselves.
     */
    tempVarP = itlValue_New((pointer_t) &(fidNameValP->fid),
			    itlCommon_fidDataType);
    assert(tempVarP);
    fidStr = itlValue_Display(tempVarP, &fieldLength);
    strLength = fieldLength;
    itl_FreeVarVal(tempVarP);

    /*
     *  Calculations for the length of the string assume worst case for
     *  the fields of the afs_hyper_t data type.  That is they are assumed to
     *  be 10 characters large.
     *  An additional byte is added for the end of string character, and
     *  the length of the labels used in the sprintf that follows are
     *  also included.
     */
    strLength += strlen((char *) fidNameValP->name.tn_chars) +  /* name field */
	         5 +  /* fid label */
		 6 +  /* spaces and newline */
		 10;   /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "\n  Fid: %s\n  Name: %s\n", fidStr, 
	    fidNameValP->name.tn_chars);
    assert((int) strlen(displayStr) < strLength);

    ITL_FREE(fidStr);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);
}

PRIVATE var_val_t *FidNameGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    afsFidTaggedName *fidNameValP;

    fidNameValP = (afsFidTaggedName *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, fidNameFieldNames,
			sizeof(fidNameFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* fid */
	    retValP = itlCommon_FidCreate(&(fidNameValP->fid));
	    break;

	  case 1:   /* name */
	    retValP = itlStds_StringVal((char *) fidNameValP->name.tn_chars);
	    break;
	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retValP);
}


PRIVATE boolean_t FidNameSetField(setValP, fieldName, fieldValP)
  IN var_val_t *setValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    int fieldNum;
    afsFidTaggedName *fidNameValP;
    boolean_t retVal = FALSE;

    fidNameValP = (afsFidTaggedName *) VALUE_VAL(setValP);

    if (itl_LookupTable(fieldName, fidNameFieldNames,
			sizeof(fidNameFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* fid */
	    if (VALUE_TYPE(fieldValP) == itlCommon_fidDataType)
	    {
		fidNameValP->fid = *((afsFid *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 1:   /* name */
	    if (VALUE_TYPE(fieldValP) == itl_stringType)
	    {
		if ((VALUE_SIZE(fieldValP) - 1) < AFS_NAMEMAX) {
		    strcpy((char *) fidNameValP->name.tn_chars, 
			   (char *) VALUE_VAL(fieldValP));
		    retVal = TRUE;
		}
		else
		    ITL_FX_NAME_ERROR(VALUE_VAL(fieldValP));
	    }
	    break;
	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retVal);
}

/*
 *  itlFX_FidNameGet() --
 *    Will prompt for and obtain the value of a fs_fxFidName variable.
 */
SHARED void itlFX_FidNameGet(fidNameP)
  OUT afsFidTaggedName *fidNameP;  /* pointer to caller allocated afsFidTaggedName */
{
    var_val_t *fidNameVarP;

    fidNameVarP = FidNameRead((var_val_t *) 0, itlFX_fidNameDataType);
    assert(fidNameVarP);
    *fidNameP = *((afsFidTaggedName *) VALUE_VAL(fidNameVarP));
    itl_FreeVarVal(fidNameVarP);
}
