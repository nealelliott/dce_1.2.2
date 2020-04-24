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
 * itl_fxStoreStatus.c -- 
 *    Code for dealing with the afsStoreStatus data type.
 *
 */
/*
 * HISTORY
 * $Log: itl_fxStoreStatus.c,v $
 * Revision 1.1.322.1  1996/10/17  18:26:44  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:26  damon]
 *
 * $EndLog$
 */

#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <pthread.h>
#include <dce/sec_login.h>
#include <dcedfs/common_data.h>
#include <dcedfs/afs4int.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_fxDCEError.h>
#include <utils/itl/itl_commonHyper.h>
#include <utils/itl/itl_commonTimeval.h>
#include <utils/itl/itl_fxPrincipal.h>
#include <utils/itl/itl_fxStoreStatus.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxStoreStatus.c,v 1.1.322.1 1996/10/17 18:26:44 damon Exp $")


int itlFX_storeStatusDataType;


PRIVATE var_val_t *StoreStatusCopy _TAKES((
					   IN var_val_t *copyVarP
					   ));

PRIVATE var_val_t *StoreStatusNew _TAKES((
					  IN pointer_t newValP,
					  IN int newType
					  ));

PRIVATE boolean_t StoreStatusFree _TAKES((
					  IN var_val_t *freeVarP
					  ));

PRIVATE var_val_t *StoreStatusRead _TAKES((
					   IN var_val_t *readVarP,
					   IN int varType
					   ));

PRIVATE char *StoreStatusDisplay _TAKES((
					 IN var_val_t *displayVarP,
					 OUT int *lengthP
					 ));

PRIVATE var_val_t *StoreStatusGetField _TAKES((
					       IN var_val_t *getVarP,
					       IN char *fieldName
					       ));

PRIVATE boolean_t StoreStatusSetField _TAKES((
					      IN var_val_t *setVarP,
					      IN char *fieldName,
					      IN var_val_t *fieldVarP
					      ));


PRIVATE itlValue_varTypeInfo_t storeStatusVarType = {
    "fs_fxStoreStatus",
    StoreStatusCopy,
    StoreStatusNew,
    StoreStatusFree,
    StoreStatusRead,
    NULL,
    NULL,
    NULL,
    NULL,
    StoreStatusDisplay,
    StoreStatusGetField,
    StoreStatusSetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *storeStatusFieldNames[] = {
    "mask",
    "modTime",
    "accessTime",
    "changeTime",
    "owner",
    "group",
    "mode",
    "truncLength",
    "length",
    "typeUUID",
    "deviceType",
    "deviceNumber",
    "cmask"
};


PRIVATE itlu_valueMap_t statusMaskFlags[] = {
    {"AFS_SETMODTIME",      AFS_SETMODTIME},
    {"AFS_SETOWNER",        AFS_SETOWNER},
    {"AFS_SETGROUP",        AFS_SETGROUP},
    {"AFS_SETMODE",         AFS_SETMODE},
    {"AFS_SETACCESSTIME",   AFS_SETACCESSTIME},
    {"AFS_SETCHANGETIME",   AFS_SETCHANGETIME},
    {"AFS_SETLENGTH",       AFS_SETLENGTH},
    {"AFS_SETTYPEUUID",     AFS_SETTYPEUUID},
    {"AFS_SETDEVNUM",       AFS_SETDEVNUM},
    {"AFS_SETMODEXACT",     AFS_SETMODEXACT},
    {"AFS_SETTRUNCLENGTH",  AFS_SETTRUNCLENGTH},
    {NULL,                  0}
};

/*
 *  StoreStatusInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void StoreStatusInit()
{
    long returnCode;

    /*
     *  Ensure utilities package is initialized.
     */
    if (itlu_Init())
    {
	itl_Error("ITL utilities did not initialize, exiting.\n");
	exit(-1);
    }

    /*
     *  We can safely set storeStatusHashType because this function is called
     *  via pthread_once.
     */
    itlFX_storeStatusDataType = itlValue_RegisterVarType(&storeStatusVarType);

    if (returnCode = itlu_ProcessValueMap(statusMaskFlags, NULL))
    {
	itl_Error("itlu_ProcessValueMap (statusMaskFlags) FAILED - %ld\n", 
		  returnCode);
	exit(-1);
    }
    itlCommon_HyperInit();
    itlCommon_TimevalInit();
}

/*
 * itlFX_StoreStatusInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlFX_StoreStatusInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) StoreStatusInit))
	itl_Error("pthread_once failed in StoreStatusInit\n");
}


PRIVATE var_val_t *StoreStatusCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    afsStoreStatus *statusValP = (afsStoreStatus *) VALUE_VAL(copyVarP);
    afsStoreStatus *newStatusP;

    assert(VALUE_TYPE(copyVarP) == itlFX_storeStatusDataType);
    ITL_NEW(newStatusP, afsStoreStatus);
    *newStatusP = *statusValP;
    return(StoreStatusNew((pointer_t) newStatusP, itlFX_storeStatusDataType));
}

PRIVATE var_val_t *StoreStatusNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newStatusVarP;

    assert(newType == itlFX_storeStatusDataType);
    ITL_NEW(newStatusVarP, var_val_t);
    VALUE_TYPE(newStatusVarP) = newType;
    VALUE_SIZE(newStatusVarP) = sizeof(afsStoreStatus);
    VALUE_VAL(newStatusVarP) = newValP;
    return(newStatusVarP);
}

PRIVATE boolean_t StoreStatusFree(freeVarP)
  IN var_val_t *freeVarP;
{
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}

/*
 *  GetOwnerId() --
 *    Converts a name to an actual user identifier.  The name can be
 *    a real name, which will be looked up, or a numeric string.
 *
 *  RETURN CODES:
 *    Success - The numeric value of the user id requested.
 *    Failure - -1
 */
PRIVATE int GetIdentifier(nameP, getOwner)
  IN char *nameP;
  IN boolean_t getOwner;   /* TRUE get owner id, else get group id */
{
    struct passwd *pwDataP;
    error_status_t errorCode;
    itlFX_principal_t *princInfo;
    int retVal;

    if (isdigit(*nameP))
	retVal = (int) strtol(nameP, (char **) NULL, 0);
    else
    {
	retVal = -1;

	if ((princInfo = itlFX_PrincipalLookup(nameP)) != 
	    (itlFX_principal_t *) 0)
	{
	    sec_login_get_pwent(ITL_FX_PRINCIPAL_GET_LOGIN(princInfo),
				(sec_login_passwd_t *) &pwDataP, &errorCode);
	    if (errorCode == error_status_ok)
		retVal = (getOwner) ? pwDataP->pw_uid : pwDataP->pw_gid;
	    else
		ITL_FX_DCE_ERROR(errorCode, "sec_login_get_pwent failed");
	}
	else
	    itl_Error("User, %s, is not defined.  See dceLogin command.\n",
		      nameP);
    }
 
    return(retVal);
}


PRIVATE var_val_t *StoreStatusRead(readVarP, varType)
  IN var_val_t *readVarP;
  IN int varType;
{
    var_val_t *newVarP;
    int eof, wordSize;
    char *nameStr;

    assert(varType == itlFX_storeStatusDataType);
    if (!readVarP)
    {
	itl_PromptUser("Mask ($AFS_SET flags): ");
	readVarP = itl_GetVarVal();

	if (VALUE_TYPE(readVarP) == itlFX_storeStatusDataType)
	    newVarP = readVarP;
	else
	{
	    afsStoreStatus *newStatusP;

	    /*
	     *  This memory is de-allocated in the StoreStatusFree()
	     *  routine.
	     */
	    ITL_NEW(newStatusP, afsStoreStatus);
	    (void) memset((char *) newStatusP, 0, sizeof(afsStoreStatus));
	    newVarP = itlValue_ConvertValue(readVarP, itl_intType);
	    assert(newVarP);
	    newStatusP->mask = *((unsigned32 *) VALUE_VAL(newVarP));
	    itl_FreeVarVal(newVarP);

	    /*
	     *  Based on the values of the mask field, we prompt for the 
	     *  fields the user wishes to set.
	     */
	    if (newStatusP->mask & AFS_SETMODTIME)
	    {
		itl_AdviseUser("ModTime -\n");
		newVarP = itlValue_Read((var_val_t *) 0, 
					itlCommon_timevalDataType);
		newStatusP->modTime = *((afsTimeval *) VALUE_VAL(newVarP));
		itl_FreeVarVal(newVarP);
	    }
	    if (newStatusP->mask & AFS_SETACCESSTIME)
	    {
		itl_AdviseUser("AccessTime -\n");
		newVarP = itlValue_Read((var_val_t *) 0, 
					itlCommon_timevalDataType);
		newStatusP->accessTime = *((afsTimeval *) VALUE_VAL(newVarP));
		itl_FreeVarVal(newVarP);
	    }
	    if (newStatusP->mask & AFS_SETCHANGETIME)
	    {
		itl_AdviseUser("ChangeTime -\n");
		newVarP = itlValue_Read((var_val_t *) 0, 
					itlCommon_timevalDataType);
		newStatusP->changeTime = *((afsTimeval *) VALUE_VAL(newVarP));
		itl_FreeVarVal(newVarP);
	    }
	    if (newStatusP->mask & AFS_SETOWNER)
	    {
		itl_PromptUser("Owner: ");
		nameStr = itl_GetString(&eof, &wordSize);
		newStatusP->owner = GetIdentifier(nameStr, TRUE);
		ITL_FREE(nameStr);
	    }
	    if (newStatusP->mask & AFS_SETGROUP)
	    {
		itl_PromptUser("Group: ");
		nameStr = itl_GetString(&eof, &wordSize);
		newStatusP->group = GetIdentifier(nameStr, FALSE);
		ITL_FREE(nameStr);
	     }
	    if (newStatusP->mask & AFS_SETMODE)
	    {
		itl_PromptUser("Mode: ");
		newStatusP->mode = itl_GetInteger((boolean_t *) 0);
	    }
	    if (newStatusP->mask & AFS_SETTRUNCLENGTH)
	    {
		itl_AdviseUser("TruncLength -\n");
		newVarP = itlValue_Read((var_val_t *) 0, 
					itlCommon_hyperDataType);
		newStatusP->truncLength = *((afs_hyper_t *) VALUE_VAL(newVarP));
		itl_FreeVarVal(newVarP);
	    }
	    if (newStatusP->mask & AFS_SETLENGTH)
	    {
		itl_AdviseUser("Length -\n");
		newVarP = itlValue_Read((var_val_t *) 0, 
					itlCommon_hyperDataType);
		newStatusP->length = *((afs_hyper_t *) VALUE_VAL(newVarP));
		itl_FreeVarVal(newVarP);
	    }
	    if (newStatusP->mask & AFS_SETDEVNUM)
	    {
		itl_PromptUser("Device Type: ");
		newStatusP->deviceType = itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("Device Number: ");
		newStatusP->deviceNumber = itl_GetInteger((boolean_t *) 0);
	    }
	    if (newStatusP->mask & AFS_SETTYPEUUID)
	    {
		itl_Error("Don't know how to get a uuid.\n");
	    }
	    newVarP = StoreStatusNew((pointer_t) newStatusP, 
				     itlFX_storeStatusDataType);
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

PRIVATE char *StoreStatusDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    char *displayStr, *index;
    char *mTimeStr, *aTimeStr, *cTimeStr, *truncLengthStr, *lengthStr;
    int strLength, fieldLength;
    afsStoreStatus *statusValP;
    var_val_t *tempVarP;

    statusValP = (afsStoreStatus *) VALUE_VAL(displayVarP);

    /*
     *  First obtain display strings for all sub-fields that are
     *  data types themselves.
     */
    tempVarP = itlValue_New((pointer_t) &(statusValP->modTime),
			    itlCommon_timevalDataType);
    assert(tempVarP);
    mTimeStr = itlValue_Display(tempVarP, &fieldLength);
    strLength = fieldLength;
    itl_FreeVarVal(tempVarP);
    tempVarP = itlValue_New((pointer_t) &(statusValP->accessTime),
			    itlCommon_timevalDataType);
    assert(tempVarP);
    aTimeStr = itlValue_Display(tempVarP, &fieldLength);
    strLength += fieldLength;
    itl_FreeVarVal(tempVarP);
    tempVarP = itlValue_New((pointer_t) &(statusValP->changeTime),
			    itlCommon_timevalDataType);
    assert(tempVarP);
    cTimeStr = itlValue_Display(tempVarP, &fieldLength);
    strLength += fieldLength;
    itl_FreeVarVal(tempVarP);
    tempVarP = itlValue_New((pointer_t) &(statusValP->truncLength),
			    itlCommon_hyperDataType);
    assert(tempVarP);
    truncLengthStr = itlValue_Display(tempVarP, &fieldLength);
    strLength += fieldLength;
    itl_FreeVarVal(tempVarP);
    tempVarP = itlValue_New((pointer_t) &(statusValP->length),
			    itlCommon_hyperDataType);
    assert(tempVarP);
    lengthStr = itlValue_Display(tempVarP, &fieldLength);
    strLength += fieldLength;
    itl_FreeVarVal(tempVarP);

    /*
     *  Calculations for the length of the string assume worst case for
     *  the fields of the afs_hyper_t data type.  That is they are assumed to
     *  be 10 characters large.
     *  An additional byte is added for the end of string character, and
     *  the length of the labels used in the sprintf that follows are
     *  also included.
     */
    strLength += 17 +  /* mask field */
	         10 +  /* modTime label */
	         12 +  /* accessTime label */
	         12 +  /* changeTime label */
	         18 +  /* owner field */
	         18 +  /* group field */
	         17 +  /* mode field */
                 13 +  /* truncLength label */
                  8 +  /* length label */
	         19 +  /* typeUUID field */
	         23 +  /* deviceType field */
	         25 +  /* deviceNumber field */
		 13 +  /* cmask field */
		 43 +  /* spaces and newline */
		 10;   /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "\n  Mask: 0x%x\n  ModTime> %s\n  AccessTime> %s\n  ChangeTime> %s\n",
	    statusValP->mask, mTimeStr, aTimeStr, cTimeStr);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Owner: %10d   Group: %10d   Mode: 0x%x\n",
	    statusValP->owner, statusValP->group, statusValP->mode);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  TruncLength> %s\n  Length> %s\n  typeUUID: NOT_USED\n",
	    truncLengthStr, lengthStr);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  deviceType: %10d   deviceNumber: %10d   cmask: 0x%x\n",
	    statusValP->deviceType, statusValP->deviceNumber, 
	    statusValP->cmask);
    assert((int) strlen(displayStr) < strLength);

    ITL_FREE(mTimeStr);
    ITL_FREE(aTimeStr);
    ITL_FREE(cTimeStr);
    ITL_FREE(truncLengthStr);
    ITL_FREE(lengthStr);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);
}

PRIVATE var_val_t *StoreStatusGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    afsStoreStatus *statusValP;

    statusValP = (afsStoreStatus *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, storeStatusFieldNames,
			sizeof(storeStatusFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* mask */
	    retValP = itlStds_IntVal((int) statusValP->mask);
	    break;

	  case 1:   /* modTime */
	    retValP = itlCommon_TimevalCreate((statusValP->modTime).sec,
					      (statusValP->modTime).usec);
	    break;

	  case 2:   /* accessTime */
	    retValP = itlCommon_TimevalCreate((statusValP->accessTime).sec,
					      (statusValP->accessTime).usec);
	    break;

	  case 3:   /* changeTime */
	    retValP = itlCommon_TimevalCreate((statusValP->changeTime).sec,
					      (statusValP->changeTime).usec);
	    break;

	  case 4:   /* owner */
	    retValP = itlStds_IntVal((int) statusValP->owner);
	    break;

	  case 5:   /* group */
	    retValP = itlStds_IntVal((int) statusValP->group);
	    break;

	  case 6:   /* mode */
	    retValP = itlStds_IntVal((int) statusValP->mode);
	    break;

	  case 7:   /* truncLength */
	    retValP =
		itlCommon_HyperCreate(AFS_HGETBOTH(statusValP->truncLength));
	    break;

	  case 8:   /* length */
	    retValP = itlCommon_HyperCreate(AFS_HGETBOTH(statusValP->length));
	    break;

	  case 9:   /* typeUUID - NOT IMPLEMENTED */
	    retValP = itlStds_IntVal(0);
	    break;

	  case 10:   /* deviceType */
	    retValP = itlStds_IntVal((int) statusValP->deviceType);
	    break;

	  case 11:   /* deviceNumber */
	    retValP = itlStds_IntVal((int) statusValP->deviceNumber);
	    break;

	  case 12:   /* cmask */
	    retValP = itlStds_IntVal((int) statusValP->cmask);
	    break;
	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retValP);
}


PRIVATE boolean_t StoreStatusSetField(setValP, fieldName, fieldValP)
  IN var_val_t *setValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    int fieldNum;
    afsStoreStatus *statusValP;
    boolean_t retVal = FALSE;

    statusValP = (afsStoreStatus *) VALUE_VAL(setValP);

    if (itl_LookupTable(fieldName, storeStatusFieldNames,
			sizeof(storeStatusFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* mask */
	    if (VALUE_TYPE(fieldValP) == itl_intType)
	    {
		statusValP->mask = *((unsigned32 *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 1:   /* modTime */
	    if (VALUE_TYPE(fieldValP) == itlCommon_timevalDataType)
	    {
		statusValP->modTime = *((afsTimeval *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 2:   /* accessTime */
	    if (VALUE_TYPE(fieldValP) == itlCommon_timevalDataType)
	    {
		statusValP->accessTime = *((afsTimeval *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 3:   /* changeTime */
	    if (VALUE_TYPE(fieldValP) == itlCommon_timevalDataType)
	    {
		statusValP->changeTime = *((afsTimeval *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 4:   /* owner */
	    if (VALUE_TYPE(fieldValP) == itl_intType)
	    {
		statusValP->owner = *((unsigned32 *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 5:   /* group */
	    if (VALUE_TYPE(fieldValP) == itl_intType)
	    {
		statusValP->group = *((unsigned32 *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 6:   /* mode */
	    if (VALUE_TYPE(fieldValP) == itl_intType)
	    {
		statusValP->mode = *((unsigned32 *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 7:   /* truncLength */
	    if (VALUE_TYPE(fieldValP) == itlCommon_hyperDataType)
	    {
		statusValP->truncLength = *((afs_hyper_t *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 8:   /* length */
	    if (VALUE_TYPE(fieldValP) == itlCommon_hyperDataType)
	    {
		statusValP->length = *((afs_hyper_t *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 9:   /* typeUUID - NOT IMPLEMENTED */
	    itl_Error("Can't deal with UUID's yet.\n");
	    break;

	  case 10:   /* deviceType */
	    if (VALUE_TYPE(fieldValP) == itl_intType)
	    {
		statusValP->deviceType = *((unsigned32 *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 11:   /* deviceNumber */
	    if (VALUE_TYPE(fieldValP) == itl_intType)
	    {
		statusValP->deviceNumber = *((unsigned32 *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 12:   /* cmask */
	    if (VALUE_TYPE(fieldValP) == itl_intType)
	    {
		statusValP->cmask = *((unsigned32 *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;
	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retVal);
}

/*
 *  itlFX_StoreStatusGet() --
 *    Will prompt for and obtain the value of a fs_fxStoreStatus variable.
 */
SHARED void itlFX_StoreStatusGet(statusP)
  OUT afsStoreStatus *statusP;  /* pointer to caller allocated afsStoreStatus */
{
    var_val_t *statusVarP;

    statusVarP = StoreStatusRead((var_val_t *) 0, itlFX_storeStatusDataType);
    assert(statusVarP);
    *statusP = *((afsStoreStatus *) VALUE_VAL(statusVarP));
    itl_FreeVarVal(statusVarP);
}
