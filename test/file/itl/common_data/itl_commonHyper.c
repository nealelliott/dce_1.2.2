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
 * itl_commonHyper.c -- 
 *    Code for dealing with the afs_hyper_t (afs_hyper_t) data type.
 */
/*
 * HISTORY
 * $Log: itl_commonHyper.c,v $
 * Revision 1.1.315.1  1996/10/17  18:25:23  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:47  damon]
 *
 * $EndLog$
 */

#include <assert.h>
#include <pthread.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_commonHyper.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/common_data/itl_commonHyper.c,v 1.1.315.1 1996/10/17 18:25:23 damon Exp $")


int itlCommon_hyperDataType;


PRIVATE var_val_t *HyperCopy _TAKES((
				     IN var_val_t *copyHyperP
				     ));

PRIVATE var_val_t *HyperNew _TAKES((
				    IN pointer_t newHyperP,
				    IN int newType
				    ));

PRIVATE boolean_t HyperFree _TAKES((
				    IN var_val_t *freeHyperP
				    ));

PRIVATE var_val_t *HyperRead _TAKES((
				     IN var_val_t *readHyperP,
				     IN int varType
				     ));

PRIVATE boolean_t HyperBinaryOp _TAKES((
					IN var_val_t *leftOpP,
					IN var_val_t *rightOpP,
					IN char *binaryOp
					));

PRIVATE boolean_t HyperConvert _TAKES((
				       IN var_val_t *convertHyperP,
				       IN int varType
				       ));

PRIVATE char *HyperDisplay _TAKES((
				   IN var_val_t *displayHyperP,
				   OUT int *lengthP
				   ));

PRIVATE var_val_t *HyperGetField _TAKES((
					 IN var_val_t *getValP,
					 IN char *fieldName
					 ));

PRIVATE boolean_t HyperSetField _TAKES((
					IN var_val_t *setValP,
					IN char *fieldName,
					IN var_val_t *fieldValP
					));


PRIVATE itlValue_varTypeInfo_t hyperVarType = {
    "afs_hyper_t",
    HyperCopy,
    HyperNew,
    HyperFree,
    HyperRead,
    HyperBinaryOp,
    NULL,
    HyperConvert,
    NULL,
    HyperDisplay,
    HyperGetField,
    HyperSetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *hyperFieldNames[] = {
    "high",
    "low"
};

/*
 *  HyperInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void HyperInit()
{
    /*
     *  We can safely set hyperHashType because this function is called
     *  via pthread_once.
     */
    itlCommon_hyperDataType = itlValue_RegisterVarType(&hyperVarType);
}

/*
 * itlCommon_HyperInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlCommon_HyperInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) HyperInit))
	itl_Error("pthread_once failed in HyperInit\n");
}


PRIVATE var_val_t *HyperCopy(copyHyperP)
  IN var_val_t *copyHyperP;
{
    afs_hyper_t *copyValP = (afs_hyper_t *) VALUE_VAL(copyHyperP);
    afs_hyper_t *newHyperP;

    assert(VALUE_TYPE(copyHyperP) == itlCommon_hyperDataType);
    ITL_NEW(newHyperP, afs_hyper_t);
    *newHyperP = *copyValP;
    return(HyperNew((pointer_t) newHyperP, itlCommon_hyperDataType));
}

PRIVATE var_val_t *HyperNew(newHyperP, newType)
  IN pointer_t newHyperP;
  IN int newType;
{
    var_val_t *newValP;

    assert(newType == itlCommon_hyperDataType);
    ITL_NEW(newValP, var_val_t);
    VALUE_TYPE(newValP) = newType;
    VALUE_SIZE(newValP) = sizeof(afs_hyper_t);
    VALUE_VAL(newValP) = newHyperP;
    return(newValP);
}

PRIVATE boolean_t HyperFree(freeHyperP)
  IN var_val_t *freeHyperP;
{
    ITL_FREE(VALUE_VAL(freeHyperP));
    return(FALSE);
}

PRIVATE var_val_t *HyperRead(readHyperP, varType)
  IN var_val_t *readHyperP;
  IN int varType;
{
    var_val_t *newVarP;

    assert(varType == itlCommon_hyperDataType);
    if (!readHyperP)
    {
	itl_PromptUser("High Value: ");
	readHyperP = itl_GetVarVal();

	if (VALUE_TYPE(readHyperP) == itlCommon_hyperDataType)
	    newVarP = readHyperP;
	else
	{
	    afs_hyper_t *newHyperP;
	    long high;

	    /*
	     *  The memory allocated here for newHyperP is free'd in the 
	     *  HyperFree() routine.
	     */
	    ITL_NEW(newHyperP, afs_hyper_t);
	    newVarP = itlValue_ConvertValue(readHyperP, itl_intType);
	    assert(newVarP);
	    high = *((unsigned long *) VALUE_VAL(newVarP));
	    itl_FreeVarVal(newVarP);

	    itl_PromptUser("Low Value: ");
	    AFS_hset64(*newHyperP, high, itl_GetInteger((boolean_t *) 0));
	    newVarP = HyperNew((pointer_t) newHyperP, itlCommon_hyperDataType);
	}
    }
    else
    {
	if (!HyperConvert(readHyperP, itlCommon_hyperDataType))
	    newVarP = readHyperP;
	else
	{
	    itl_FreeVarVal(readHyperP);
	    newVarP = (var_val_t *) 0;
	}
    }
    return(newVarP);
}

PRIVATE boolean_t HyperBinaryOp(leftOpP, rightOpP, binaryOp)
  IN var_val_t *leftOpP;
  IN var_val_t *rightOpP;
  IN char *binaryOp;
{
    boolean_t validOp = FALSE, opSucceeded = FALSE;
    var_val_t *opResultP;
    int compareVal;

    compareVal = itlCommon_HyperCompare((afs_hyper_t *) VALUE_VAL(leftOpP),
					(afs_hyper_t *) VALUE_VAL(rightOpP));

    switch (*binaryOp) {
      case '=' :
	  if (binaryOp[1] == '=')
	      opSucceeded = !compareVal;
	  else
	      validOp = TRUE;
	  break;

      case '!' :
	  if (binaryOp[1] == '=')
	      opSucceeded = compareVal;
	  else
	      validOp = TRUE;
	  break;

      case '>' :
	  if (binaryOp[1] == '=')
	  {
	      if (compareVal >= 0)
		  opSucceeded = TRUE;
	  }
	  else
	  {
	      if (binaryOp[1] == '\0')
	      {
		  if (compareVal == 1)
		  opSucceeded = TRUE;
	      }
	      else
		  validOp = TRUE;
	  }
	  break;

      case '<' :
	  if (binaryOp[1] == '=')
	  {
	      if (compareVal <= 0)
		  opSucceeded = TRUE;
	  }
	  else
	  {
	      if (binaryOp[1] == '\0')
	      {
		  if (compareVal == -1)
		      opSucceeded = TRUE;
	      }
	      else
		  validOp = TRUE;
	  }
	  break;

      default :
	  validOp = TRUE;
    }

    if (validOp == TRUE)
	itl_Error("Binary operation, %s, is NOT supported/valid on afs_hyper_t's.\n", 
		  binaryOp);

    /*
     *  Set the left value to be 1 if the comparison was successful and
     *  0 otherwise.
     */
    opResultP = itlStds_IntVal(opSucceeded);
    itlValue_Free(leftOpP);
    *leftOpP = *opResultP;
    ITL_FREE(opResultP);

    return(validOp);
}


PRIVATE boolean_t HyperConvert(convertHyperP, varType)
  IN var_val_t *convertHyperP;
  IN int varType;
{
    boolean_t retVal = TRUE;
    afs_hyper_t newHyper;
    var_val_t *newValP;

    /*
     *  If the type of the convertHyperP variable is a afs_hyper_t, then we
     *  are supposed to convert it to whatever varType is.  Otherwise
     *  we are to convert the convertHyperP to be a hyper.
     *  Currently, we only convert things to afs_hyper_t's.
     */
    if ((VALUE_TYPE(convertHyperP) != itlCommon_hyperDataType) &&
	(varType == itlCommon_hyperDataType))
    {
	if (VALUE_TYPE(convertHyperP) == itl_intType)
	{
	    AFS_hset64(newHyper, 0, *((int *) VALUE_VAL(convertHyperP)));
	    newValP = HyperNew((pointer_t) &newHyper, itlCommon_hyperDataType);
	    itlValue_Free(convertHyperP);
	    *convertHyperP = *newValP;
	    ITL_FREE(newValP);
	    retVal = FALSE;
	}
	else
	    itl_Error("Can only convert integers to afs_hyper_t's.\n");
    }
    else
	itl_Error("Can't convert afs_hyper_t's to anything else.\n");

    return(retVal);
}

/*
 *  itlCommon_HyperDisplay() --
 *    Allows data types containing hypers to get a display string to 
 *    embed in their display output.
 */
EXPORT char *itlCommon_HyperDisplay(displayHyperP, lengthP)
  IN afs_hyper_t *displayHyperP;
  OUT int *lengthP;
{
    char *displayStr;
    int strLength;

    /*
     *  Calculations for the length of the string assume worst case for
     *  the fields of the afs_hyper_t data type.  That is they are assumed to
     *  be 10 characters large.
     *  An additional byte is added for the end of string character, and
     *  the length of the labels used in the sprintf that follows are
     *  also included.
     */
    strLength = 17 +  /* high field */
	        17 +  /* low field */
		6  +  /* spaces and newline */
		5;    /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "  High: %10d     Low: %10d\n",
	    AFS_HGETBOTH(*displayHyperP));
    assert((int) strlen(displayStr) < strLength);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);
}

PRIVATE char *HyperDisplay(displayHyperP, lengthP)
  IN var_val_t *displayHyperP;
  OUT int *lengthP;
{
    afs_hyper_t *hyperVal;

    hyperVal = (afs_hyper_t *) VALUE_VAL(displayHyperP);

    return(itlCommon_HyperDisplay(hyperVal, lengthP));
}

PRIVATE var_val_t *HyperGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum, convertVal;
    afs_hyper_t *hyperValP;

    hyperValP = (afs_hyper_t *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, hyperFieldNames,
			sizeof(hyperFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* high */
	    convertVal = (int) AFS_hgethi(*hyperValP);
	    break;

	  case 1:   /* low */
	    convertVal = (int) AFS_hgetlo(*hyperValP);
	    break;
	}

	retValP = itlStds_IntVal(convertVal);
    }

    return(retValP);
}


PRIVATE boolean_t HyperSetField(setValP, fieldName, fieldValP)
  IN var_val_t *setValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    int fieldNum;
    afs_hyper_t *hyperValP;
    boolean_t retVal = TRUE;

    if (VALUE_TYPE(fieldValP) == itl_intType)
    {
	hyperValP = (afs_hyper_t *) VALUE_VAL(setValP);

	if (itl_LookupTable(fieldName, hyperFieldNames,
			    sizeof(hyperFieldNames) / sizeof(char *),
			    &fieldNum) == 1)
	{
	    switch (fieldNum) {
	      case 0:   /* high */
		AFS_hset64(*hyperValP,
			   *((unsigned long *) VALUE_VAL(fieldValP)),
			   AFS_hgetlo(*hyperValP));
		break;

	      case 1:   /* low */
		AFS_hset64(*hyperValP, AFS_hgethi(*hyperValP),
			   *((unsigned long *) VALUE_VAL(fieldValP)));
		break;
	    }
	}
    }
    else
	retVal = FALSE;

    return(retVal);
}


/*
 *  itlCommon_HyperGet() --
 *    Will prompt for and obtain the value of a fs_fxHyper variable.
 */
SHARED void itlCommon_HyperGet(hyperDataP)
  OUT afs_hyper_t *hyperDataP;   /* pointer to caller allocated afs_hyper_t */
{
    var_val_t *hyperVarP;

    hyperVarP = HyperRead((var_val_t *) 0, itlCommon_hyperDataType);
    assert(hyperVarP);
    *hyperDataP = *((afs_hyper_t *) VALUE_VAL(hyperVarP));
    itl_FreeVarVal(hyperVarP);
}

/*
 *  itlCommon_HyperCreate()
 *    Given two integer values it returns a pointer to a var_val_t that
 *    contains that data.
 */
SHARED var_val_t *itlCommon_HyperCreate(high, low)
  IN unsigned long high, low;
{
    var_val_t  *newVarP, *copyVarP;
    afs_hyper_t hyperData;

    AFS_hset64(hyperData, high, low);
    newVarP = itlValue_New((pointer_t) &hyperData, itlCommon_hyperDataType);
    copyVarP = itlValue_Copy(newVarP);
    ITL_FREE(newVarP);
    return(copyVarP);
}

/*
 *  itlCommon_HyperCompare() --
 *    Compare to hypers.  All comparisons are done left on right.
 *
 *  WARNINGS:
 *    Currently only supports, ==, >, <, >=, <=, !=.
 *
 *  RETURN CODES:
 *    -1 : left is less than right
 *    0  : left equals right
 *    1  : left is greater than right
 */
EXPORT int itlCommon_HyperCompare(leftP, rightP)
  IN afs_hyper_t *leftP;
  IN afs_hyper_t *rightP;
{
    return(AFS_hcmp(*leftP, *rightP));
}
