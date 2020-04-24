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
 * itl_fxVolsync.c -- 
 *    Code for dealing with the afsVolsync data type.
 *
 */
/*
 * HISTORY
 * $Log: itl_fxVolSync.c,v $
 * Revision 1.1.324.1  1996/10/17  18:26:53  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:31  damon]
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
#include <utils/itl/itl_fxVolSync.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxVolSync.c,v 1.1.324.1 1996/10/17 18:26:53 damon Exp $")


int itlFX_volSyncDataType;


PRIVATE var_val_t *VolSyncCopy _TAKES((
				       IN var_val_t *copyVarP
				       ));

PRIVATE var_val_t *VolSyncNew _TAKES((
				      IN pointer_t newValP,
				      IN int newType
				      ));

PRIVATE boolean_t VolSyncFree _TAKES((
				      IN var_val_t *freeVarP
				      ));

PRIVATE char *VolSyncDisplay _TAKES((
				     IN var_val_t *displayVarP,
				     OUT int *lengthP
				     ));

PRIVATE var_val_t *VolSyncGetField _TAKES((
					   IN var_val_t *getVarP,
					   IN char *fieldName
					   ));


PRIVATE itlValue_varTypeInfo_t volSyncVarType = {
    "fs_fxVolSync",
    VolSyncCopy,
    VolSyncNew,
    VolSyncFree,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    VolSyncDisplay,
    VolSyncGetField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *volSyncFieldNames[] = {
    "VolID"
    "VV",
    "VVAge",
    "VVPingAge",
};

/*
 *  VolSyncInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void VolSyncInit()
{
    /*
     *  We can safely set VolSyncHashType because this function is called
     *  via pthread_once.
     */
    itlFX_volSyncDataType = itlValue_RegisterVarType(&volSyncVarType);

    itlCommon_HyperInit();
}

/*
 * itlFX_VolSyncInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlFX_VolSyncInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) VolSyncInit))
	itl_Error("pthread_once failed in VolSyncInit\n");
}


PRIVATE var_val_t *VolSyncCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    afsVolSync *statusValP = (afsVolSync *) VALUE_VAL(copyVarP);
    afsVolSync *newStatusP;

    assert(VALUE_TYPE(copyVarP) == itlFX_volSyncDataType);
    ITL_NEW(newStatusP, afsVolSync);
    *newStatusP = *statusValP;
    return(VolSyncNew((pointer_t) newStatusP, itlFX_volSyncDataType));
}

PRIVATE var_val_t *VolSyncNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newStatusVarP;

    assert(newType == itlFX_volSyncDataType);
    ITL_NEW(newStatusVarP, var_val_t);
    VALUE_TYPE(newStatusVarP) = newType;
    VALUE_SIZE(newStatusVarP) = sizeof(afsVolSync);
    VALUE_VAL(newStatusVarP) = newValP;
    return(newStatusVarP);
}

PRIVATE boolean_t VolSyncFree(freeVarP)
  IN var_val_t *freeVarP;
{
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}

PRIVATE char *VolSyncDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    char *displayStr, *index;
    char *volIdStr, *vvStr;
    int strLength, fieldLength;
    afsVolSync *syncVarP;
    var_val_t *tempVarP;

    syncVarP = (afsVolSync *) VALUE_VAL(displayVarP);

    /*
     *  First obtain display strings for all sub-fields that are
     *  data types themselves.
     */
    tempVarP = itlValue_New((pointer_t) &(syncVarP->VolID),
			    itlCommon_hyperDataType);
    assert(tempVarP);
    volIdStr = itlValue_Display(tempVarP, &fieldLength);
    strLength = fieldLength;
    itl_FreeVarVal(tempVarP);
    tempVarP = itlValue_New((pointer_t) &(syncVarP->VV),
			    itlCommon_hyperDataType);
    assert(tempVarP);
    vvStr = itlValue_Display(tempVarP, &fieldLength);
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
    strLength +=  7 +  /* VolID label */
	          4 +  /* VV label */
	         18 +  /* VVAge field */
	         22 +  /* VVPingAge field */
		 12 +  /* spaces and newline */
		 10;   /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "\n  VolID: %s\n  VV: %s\n", volIdStr, vvStr);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  VVAge: %10d   VVPingAge: %10d\n",
	    syncVarP->VVAge, syncVarP->VVPingAge);
    assert((int) strlen(displayStr) < strLength);

    ITL_FREE(volIdStr);
    ITL_FREE(vvStr);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);

}

PRIVATE var_val_t *VolSyncGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    afsVolSync *syncVarP;

    syncVarP = (afsVolSync *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, volSyncFieldNames,
			sizeof(volSyncFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* VolId */
	    retValP = itlCommon_HyperCreate(AFS_HGETBOTH(syncVarP->VolID));
	    break;

	  case 1:   /* VV */
	    retValP = itlCommon_HyperCreate(AFS_HGETBOTH(syncVarP->VV));
	    break;

	  case 2:   /* VVAge */
	    retValP = itlStds_IntVal((int) syncVarP->VVAge);
	    break;

	  case 3:   /* VVPingAge */
	    retValP = itlStds_IntVal((int) syncVarP->VVPingAge);
	    break;
	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retValP);
}
