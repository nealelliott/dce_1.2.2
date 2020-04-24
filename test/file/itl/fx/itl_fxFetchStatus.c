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
 * itl_fxFetchStatus.c -- 
 *    Code for dealing with the afsFetchStatus data type.
 */
/*
 * HISTORY
 * $Log: itl_fxFetchStatus.c,v $
 * Revision 1.1.318.1  1996/10/17  18:26:24  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:16  damon]
 *
 * $EndLog$
 */

#include <assert.h>
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
#include <utils/itl/itl_fxFetchStatus.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxFetchStatus.c,v 1.1.318.1 1996/10/17 18:26:24 damon Exp $")


int itlFX_fetchStatusDataType;


PRIVATE var_val_t *FetchStatusCopy _TAKES((
					   IN var_val_t *copyVarP
					   ));

PRIVATE var_val_t *FetchStatusNew _TAKES((
					  IN pointer_t newValP,
					  IN int newType
					  ));

PRIVATE boolean_t FetchStatusFree _TAKES((
					  IN var_val_t *freeVarP
					  ));

PRIVATE char *FetchStatusDisplay _TAKES((
					 IN var_val_t *displayVarP,
					 OUT int *lengthP
					 ));

PRIVATE var_val_t *FetchStatusGetField _TAKES((
					       IN var_val_t *getVarP,
					       IN char *fieldName
					       ));


PRIVATE itlValue_varTypeInfo_t fetchStatusVarType = {
    "fs_fxFetchStatus",
    FetchStatusCopy,
    FetchStatusNew,
    FetchStatusFree,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    FetchStatusDisplay,
    FetchStatusGetField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *fetchStatusFieldNames[] = {
    "interfaceVersion",
    "fileType",
    "linkCount",
    "length",
    "dataVersion",
    "author",
    "owner",
    "group",
    "callerAccess",
    "anonymousAccess",
    "aclExpirationTime",
    "mode",
    "parentVnode",
    "parentUnique",
    "modTime",
    "changeTime",
    "accessTime",
    "serverModTime",
    "typeUUID",
    "objectUUID",
    "deviceNumber"
};

/*
 *  FetchStatusInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void FetchStatusInit()
{
    /*
     *  We can safely set fetchStatusHashType because this function is called
     *  via pthread_once.
     */
    itlFX_fetchStatusDataType = itlValue_RegisterVarType(&fetchStatusVarType);

    itlCommon_HyperInit();
    itlCommon_TimevalInit();
}

/*
 * itlFX_FetchStatusInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlFX_FetchStatusInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) FetchStatusInit))
	itl_Error("pthread_once failed in FetchStatusInit\n");
}


PRIVATE var_val_t *FetchStatusCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    afsFetchStatus *statusValP = (afsFetchStatus *) VALUE_VAL(copyVarP);
    afsFetchStatus *newStatusP;

    assert(VALUE_TYPE(copyVarP) == itlFX_fetchStatusDataType);
    ITL_NEW(newStatusP, afsFetchStatus);
    *newStatusP = *statusValP;
    return(FetchStatusNew((pointer_t) newStatusP, itlFX_fetchStatusDataType));
}

PRIVATE var_val_t *FetchStatusNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newStatusVarP;

    assert(newType == itlFX_fetchStatusDataType);
    ITL_NEW(newStatusVarP, var_val_t);
    VALUE_TYPE(newStatusVarP) = newType;
    VALUE_SIZE(newStatusVarP) = sizeof(afsFetchStatus);
    VALUE_VAL(newStatusVarP) = newValP;
    return(newStatusVarP);
}

PRIVATE boolean_t FetchStatusFree(freeVarP)
  IN var_val_t *freeVarP;
{
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}

PRIVATE char *FetchStatusDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    char *displayStr, *index;
    char *mTimeStr, *aTimeStr, *cTimeStr, *serverModStr;
    char *dataVersionStr, *lengthStr;
    int strLength, fieldLength;
    afsFetchStatus *statusValP;
    var_val_t *tempVarP;

    statusValP = (afsFetchStatus *) VALUE_VAL(displayVarP);

    /*
     *  First obtain display strings for all sub-fields that are
     *  data types themselves.
     */
    tempVarP = itlValue_New((pointer_t) &(statusValP->length),
			    itlCommon_hyperDataType);
    assert(tempVarP);
    lengthStr = itlValue_Display(tempVarP, &fieldLength);
    strLength = fieldLength;
    itl_FreeVarVal(tempVarP);
    tempVarP = itlValue_New((pointer_t) &(statusValP->dataVersion),
			    itlCommon_hyperDataType);
    assert(tempVarP);
    dataVersionStr = itlValue_Display(tempVarP, &fieldLength);
    strLength += fieldLength;
    itl_FreeVarVal(tempVarP);
    tempVarP = itlValue_New((pointer_t) &(statusValP->modTime),
			    itlCommon_timevalDataType);
    assert(tempVarP);
    mTimeStr = itlValue_Display(tempVarP, &fieldLength);
    strLength += fieldLength;
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
    tempVarP = itlValue_New((pointer_t) &(statusValP->serverModTime),
			    itlCommon_timevalDataType);
    assert(tempVarP);
    serverModStr = itlValue_Display(tempVarP, &fieldLength);
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
    strLength += 29 +  /* interfaceVersion field */
	         21 +  /* fileType field */
	         21 +  /* linkCount field */
		  8 +  /* length label */
		 13 +  /* dataVersion label */
	         19 +  /* author field */
	         18 +  /* owner field */
	         18 +  /* group field */
	         25 +  /* callerAccess field */
	         27 +  /* aclExpirationTime field */
	         17 +  /* mode field */
	         24 +  /* parentVnode field */
	         25 +  /* parentUnique field */
		  9 +  /* modTime label */
		 12 +  /* changeTime label */
		 12 +  /* accessTime label */
		 15 +  /* serverModTime label */
	         21 +  /* typeUUID field */
	         22 +  /* objectUUID field */
	         25 +  /* deviceNumber field */
		 63 +  /* spaces and newline */
		 10;   /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "\n  InterfaceVersion: %10d\n", 
	    statusValP->interfaceVersion);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  FileType: %10d   LinkCount: %10d\n  Length> %s\n  DataVersion> %s\n",
	    statusValP->fileType, statusValP->linkCount, lengthStr, 
	    dataVersionStr);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Author: %10d   Owner: %10d   Group: %10d\n",
	    statusValP->author, statusValP->owner, statusValP->group);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  CallerAccess: %10d   AnonymousAccess: %10d\n",
	    statusValP->callerAccess, statusValP->anonymousAccess);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  AclExpirationTime: %10d   Mode: %10d\n",
	    statusValP->aclExpirationTime, statusValP->mode);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  ParentVnode: %10d   ParentUnique: %10d\n",
	    statusValP->parentVnode, statusValP->parentUnique);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  ModTime> %s\n  ChangeTime> %s\n", mTimeStr, cTimeStr);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  AccessTime> %s\n  ServerModTime> %s\n", 
	    aTimeStr, serverModStr);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  TypeUUID: NOT_USED   ObjectUUID: NOT_USED   DeviceNumber: %10d\n",
	    statusValP->deviceNumber);
    assert((int) strlen(displayStr) < strLength);

    ITL_FREE(mTimeStr);
    ITL_FREE(aTimeStr);
    ITL_FREE(cTimeStr);
    ITL_FREE(serverModStr);
    ITL_FREE(dataVersionStr);
    ITL_FREE(lengthStr);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);

}

PRIVATE var_val_t *FetchStatusGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    afsFetchStatus *statusValP;

    statusValP = (afsFetchStatus *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, fetchStatusFieldNames,
			sizeof(fetchStatusFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* interfaceVersion */
	    retValP = itlStds_IntVal((int) statusValP->interfaceVersion);
	    break;

	  case 1:   /* fileType */
	    retValP = itlStds_IntVal((int) statusValP->fileType);
	    break;

	  case 2:   /* linkCount */
	    retValP = itlStds_IntVal((int) statusValP->linkCount);
	    break;

	  case 3:   /* length */
	    retValP = itlCommon_HyperCreate(AFS_HGETBOTH(statusValP->length));
	    break;

	  case 4:   /* dataVersion */
	    retValP =
		itlCommon_HyperCreate(AFS_HGETBOTH(statusValP->dataVersion));
	    break;

	  case 5:   /* author */
	    retValP = itlStds_IntVal((int) statusValP->author);
	    break;

	  case 6:   /* owner */
	    retValP = itlStds_IntVal((int) statusValP->owner);
	    break;

	  case 7:   /* group */
	    retValP = itlStds_IntVal((int) statusValP->group);
	    break;

	  case 8:   /* callerAccess */
	    retValP = itlStds_IntVal((int) statusValP->callerAccess);
	    break;

	  case 9:   /* anonymousAccess */
	    retValP = itlStds_IntVal((int) statusValP->anonymousAccess);
	    break;

	  case 10:   /* aclExpirationTime */
	    retValP = itlStds_IntVal((int) statusValP->aclExpirationTime);
	    break;

	  case 11:   /* mode */
	    retValP = itlStds_IntVal((int) statusValP->mode);
	    break;

	  case 12:   /* parentVnode */
	    retValP = itlStds_IntVal((int) statusValP->parentVnode);
	    break;

	  case 13:   /* parentUnique */
	    retValP = itlStds_IntVal((int) statusValP->parentUnique);
	    break;

	  case 14: /* modTime */
	    retValP = itlCommon_TimevalCreate((statusValP->modTime).sec,
					      (statusValP->modTime).usec);

	    break;

	  case 15:   /* changeTime */
	    retValP = itlCommon_TimevalCreate((statusValP->changeTime).sec,
					      (statusValP->changeTime).usec);
	    break;

	  case 16:   /* accessTime */
	    retValP = itlCommon_TimevalCreate((statusValP->accessTime).sec,
					      (statusValP->accessTime).usec);
	    break;
 
	  case 17:   /* typeUUID - NOT IMPLEMENTED */
	    retValP = itlStds_IntVal(0);
	    break;
 
	  case 18:   /* objectUUID - NOT IMPLEMENTED */
	    retValP = itlStds_IntVal(0);
	    break;

	  case 19:   /* deviceNumber */
	    retValP = itlStds_IntVal((int) statusValP->deviceNumber);
	    break;
	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retValP);
}
