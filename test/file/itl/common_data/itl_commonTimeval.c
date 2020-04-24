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
 * itl_commonTimeval.c -- 
 *    Code for dealing with the timeval (afsTimeval) data type.
 */
/*
 * HISTORY
 * $Log: itl_commonTimeval.c,v $
 * Revision 1.1.321.1  1996/10/17  18:25:33  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:52  damon]
 *
 * Revision 1.1.316.1  1994/02/04  20:43:31  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:42  devsrc]
 * 
 * Revision 1.1.314.2  1994/01/20  18:44:02  annie
 * 	added copyright header
 * 	[1994/01/20  18:40:21  annie]
 * 
 * Revision 1.1.314.1  1993/12/07  17:43:30  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:09:37  jaffe]
 * 
 * $EndLog$
 */

#include <assert.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_commonTimeval.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/common_data/itl_commonTimeval.c,v 1.1.321.1 1996/10/17 18:25:33 damon Exp $")

#define ITL_HALF_MICRO_SECOND  500000

int itlCommon_timevalDataType;

PRIVATE pthread_mutex_t localTimeMutex;

PRIVATE var_val_t *TimevalCopy _TAKES((
				       IN var_val_t *copyTimevalP
				       ));

PRIVATE var_val_t *TimevalNew _TAKES((
				      IN pointer_t newTimevalP,
				      IN int newType
				      ));

PRIVATE boolean_t TimevalFree _TAKES((
				      IN var_val_t *freeTimevalP
				      ));

PRIVATE var_val_t *TimevalRead _TAKES((
				       IN var_val_t *readTimevalP,
				       IN int varType
				       ));

PRIVATE boolean_t TimevalBinaryOp _TAKES((
					  IN var_val_t *leftOpP,
					  IN var_val_t *rightOpP,
					  IN char *binaryOp
					  ));

PRIVATE boolean_t TimevalConvert _TAKES((
					 IN var_val_t *convertTimevalP,
					 IN int varType
					 ));

PRIVATE char *TimevalDisplay _TAKES((
				     IN var_val_t *displayTimevalP,
				     OUT int *lengthP
				     ));

PRIVATE var_val_t *TimevalGetField _TAKES((
					   IN var_val_t *getValP,
					   IN char *fieldName
					   ));

PRIVATE boolean_t TimevalSetField _TAKES((
					  IN var_val_t *setValP,
					  IN char *fieldName,
					  IN var_val_t *fieldValP
					  ));


PRIVATE itlValue_varTypeInfo_t timevalVarType = {
    "timeval",
    TimevalCopy,
    TimevalNew,
    TimevalFree,
    TimevalRead,
    TimevalBinaryOp,
    NULL,
    TimevalConvert,
    NULL,
    TimevalDisplay,
    TimevalGetField,
    TimevalSetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *timevalFieldNames[] = {
    "tv_sec",
    "tv_usec"
};

/*
 *  TimevalInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void TimevalInit()
{
    /*
     *  We can safely set timevalHashType because this function is called
     *  via pthread_once.
     */
    itlCommon_timevalDataType = itlValue_RegisterVarType(&timevalVarType);
    pthread_mutex_init(&localTimeMutex, pthread_mutexattr_default);
}

/*
 * itlCommon_TimevalInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlCommon_TimevalInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) TimevalInit))
	itl_Error("pthread_once failed in TimevalInit\n");
}


PRIVATE var_val_t *TimevalCopy(copyTimevalP)
  IN var_val_t *copyTimevalP;
{
    struct timeval *copyValP = (struct timeval *) VALUE_VAL(copyTimevalP);
    struct timeval *newTimevalP;

    assert(VALUE_TYPE(copyTimevalP) == itlCommon_timevalDataType);
    ITL_NEW(newTimevalP, struct timeval);
    newTimevalP->tv_sec = copyValP->tv_sec;
    newTimevalP->tv_usec = copyValP->tv_usec;
    return(TimevalNew((pointer_t) newTimevalP, itlCommon_timevalDataType));
}

PRIVATE var_val_t *TimevalNew(newTimevalP, newType)
  IN pointer_t newTimevalP;
  IN int newType;
{
    var_val_t *newValP;

    assert(newType == itlCommon_timevalDataType);
    ITL_NEW(newValP, var_val_t);
    VALUE_TYPE(newValP) = newType;
    VALUE_SIZE(newValP) = sizeof(struct timeval);
    VALUE_VAL(newValP) = newTimevalP;
    return(newValP);
}

PRIVATE boolean_t TimevalFree(freeTimevalP)
  IN var_val_t *freeTimevalP;
{
    ITL_FREE(VALUE_VAL(freeTimevalP));
    return(FALSE);
}

PRIVATE var_val_t *TimevalRead(readTimevalP, varType)
  IN var_val_t *readTimevalP;
  IN int varType;
{
    var_val_t *newVarP;

    assert(varType == itlCommon_timevalDataType);
    if (!readTimevalP)
    {
	/*
	 *  Ask for time in format easiest for the user, i.e.
	 *  date, time, and microseconds.
	 */
	itl_AdviseUser("Entering the empty string (\"\") for the date and\n");
	itl_AdviseUser("time values, and 0 for the micro-seconds value\n");
	itl_AdviseUser("will cause the current time to be used.\n");

	itl_PromptUser("Date (\"mm/dd/yy\"): ");
	readTimevalP = itl_GetVarVal();

	if (VALUE_TYPE(readTimevalP) == itlCommon_timevalDataType)
	    newVarP = readTimevalP;
	else
	{
	    struct tm *tmInfo;
	    char *dateP, *timeP, *fieldP, *separatorP;
	    int j, *tmFieldP, timeStrLength;
	    boolean_t dateIsNull, timeIsNull;
	    struct timeval *newTimevalP;
	    time_t now;

	    /*
	     *  A mutex is used here because the man page for localtime
	     *  says it returns a pointer to a static data area, i.e. it
	     *  is not thread safe.
	     */
	    pthread_mutex_lock(&localTimeMutex);
	    ITL_NEW(newTimevalP, struct timeval);
	    now = time((time_t *) 0);
	    tmInfo = (struct tm *) localtime(&now); /* init time info */

	    newVarP = itlValue_ConvertValue(readTimevalP, itl_stringType);
	    assert(newVarP);
	    dateP = (char *) VALUE_VAL(newVarP);

	    if (strcmp(dateP, ""))
	    {
		/*
		 *  Update tm structure with users date.
		 */
		for (fieldP = dateP, j = 0; j < 3; j++)
		{
		    switch (j) {
		      case 0:
			tmFieldP = &(tmInfo->tm_mon);
			break;
		      case 1:
			tmFieldP = &(tmInfo->tm_mday);
			break;
		      case 2:
			tmFieldP = &(tmInfo->tm_year);
			break;
		    }
		    *tmFieldP = (int) strtol(fieldP, &separatorP, 10);
		    if (j < 2)
		    {
			if (*separatorP == '/')
			    fieldP = separatorP + 1;
			else
			{
			    itl_Error("Invalid date specified, no /.\n");
			    break;
			}
		    }
		}
		/*
		 *  mktime treats the month based on 0-11 rather than
		 *  the more intuitive 1-12.  (This must be a UNIXism
		 *  from days of old.)  So subtract one from the month
		 *  entered by the user.
		 */ 
		tmInfo->tm_mon = tmInfo->tm_mon - 1; 
	    }
	    else
		dateIsNull = TRUE;

	    itl_FreeVarVal(newVarP);

	    ITL_GET_STRING("Time (\"hh:mm:ss\"): ", timeP, &timeStrLength);
	    assert(timeP);
	    
	    if (strcmp(timeP, ""))
	    {
		/*
		 *  Update tm structure with users time.
		 */
		for (fieldP = timeP, j = 0; j < 3; j++)
		{
		    switch (j) {
		      case 0:
			tmFieldP = &(tmInfo->tm_hour);
			break;
		      case 1:
			tmFieldP = &(tmInfo->tm_min);
			break;
		      case 2:
			tmFieldP = &(tmInfo->tm_sec);
			break;
		    }
		    *tmFieldP = (int) strtol(fieldP, &separatorP, 10);
		    if (j < 2)
		    {
			if (*separatorP == ':')
			    fieldP = separatorP + 1;
			else
			{
			    itl_Error("Invalid time specified, no :.\n");
			    break;
			}
		    }
		}
	    }
	    else
		timeIsNull = TRUE;

	    ITL_UTIL_FREE(timeP);

	    itl_PromptUser("Time (micro-seconds): ");
	    newTimevalP->tv_usec = itl_GetInteger((boolean_t *) 0);

	    /*
	     *  If the user wanted the present time, then get it again.
	     *  Just trying to be more accurate.
	     */
	    if (dateIsNull && timeIsNull && (newTimevalP->tv_usec == 0))
	    {
		now = time((time_t *) 0);
		tmInfo = (struct tm *) localtime(&now);
	    }
	    newTimevalP->tv_sec = mktime(tmInfo);
	    pthread_mutex_unlock(&localTimeMutex);
	    newVarP = TimevalNew((pointer_t) newTimevalP, 
				 itlCommon_timevalDataType);
	}
    }
    else
    {
	if (!TimevalConvert(readTimevalP, itlCommon_timevalDataType))
	    newVarP = readTimevalP;
	else
	{
	    itl_FreeVarVal(readTimevalP);
	    newVarP = (var_val_t *) 0;
	}
    }
    return(newVarP);
}

PRIVATE boolean_t TimevalBinaryOp(leftOpP, rightOpP, binaryOp)
  IN var_val_t *leftOpP;
  IN var_val_t *rightOpP;
  IN char *binaryOp;
{
    boolean_t validOp = FALSE, opSucceeded = FALSE;
    boolean_t isEqual, isGreater;
    var_val_t *opResultP;
    struct timeval *leftVal, *rightVal;

    leftVal = (struct timeval *) VALUE_VAL(leftOpP);
    rightVal = (struct timeval *) VALUE_VAL(rightOpP);
    
    isEqual = !timercmp(leftVal, rightVal, !=);
    isGreater = timercmp(leftVal, rightVal, >);

    switch (*binaryOp) {
      case '=' :
	  if (binaryOp[1] == '=')
	      opSucceeded = isEqual;
	  else
	      validOp = TRUE;
	  break;

      case '!' :
	  if (binaryOp[1] == '=')
	      opSucceeded = !isEqual;
	  else
	      validOp = TRUE;
	  break;

      case '>' :
	  if (binaryOp[1] == '=')
	      opSucceeded = (isGreater || isEqual);
	  else
	  {
	      if (binaryOp[1] == '\0')
		  opSucceeded = isGreater;
	      else
		  validOp = TRUE;
	  }
	  break;

      case '<' :
	  if (binaryOp[1] == '=')
	      opSucceeded = (!isGreater || isEqual);
	  else
	  {
	      if (binaryOp[1] == '\0')
		  opSucceeded = !(isGreater || isEqual);
	      else
		  validOp = TRUE;
	  }
	  break;

      default :
	  validOp = TRUE;
    }

    if (validOp == TRUE)
	itl_Error("Binary operation, %s, NOT supported/valid on timeval's.\n", 
		  binaryOp);

    /*
     *  Set the left value to 1 if the comparison was successful and
     *  0 if not.
     */
    opResultP = itlStds_IntVal(opSucceeded);
    itlValue_Free(leftOpP);
    *leftOpP = *opResultP;
    ITL_FREE(opResultP);

    return(validOp);
}


PRIVATE boolean_t TimevalConvert(convertTimevalP, varType)
  IN var_val_t *convertTimevalP;
  IN int varType;
{
    boolean_t retVal = FALSE;
    struct timeval *timevalVal;
    var_val_t *newValP;

    timevalVal = (struct timeval *) VALUE_VAL(convertTimevalP);
    /*
     *  If the type of the convertTimevalP variable is a timeval, then we
     *  are supposed to convert it to whatever varType is.  Otherwise
     *  we are to convert the convertTimevalP to be a timeval.
     *  Currently, we can convert int's to timeval's and vice versa.
     */
    if (VALUE_TYPE(convertTimevalP) == itlCommon_timevalDataType)
    {
	if (varType == itl_intType)
	{
	    int seconds = timevalVal->tv_sec;

	    /*
	     *  Round the seconds field based on the value of the 
	     *  usec field, and convert that to an integer.
	     */
	    if (timevalVal->tv_usec >= ITL_HALF_MICRO_SECOND)
		seconds++;

	    newValP = itlStds_IntVal(seconds);
	}
	else
	{
	    itl_Error("Can only convert timeval's to integers.\n");
	    retVal = TRUE;
	}
    }
    else
    {
	if (VALUE_TYPE(convertTimevalP) == itl_intType)
	{
	    struct timeval newTimeval;

	    newTimeval.tv_sec = *(int *) VALUE_VAL(convertTimevalP);
	    newTimeval.tv_usec = 0;
	    newValP = TimevalNew((pointer_t) &newTimeval, 
				 itlCommon_timevalDataType);
	}
	else
	{
	    itl_Error("Can only convert integer's to timeval's.\n");
	    retVal = TRUE;
	}
    }

    if (!retVal)
    {
	itlValue_Free(convertTimevalP);
	*convertTimevalP = *newValP;
	ITL_FREE(newValP);
    }

    return(retVal);
}

PRIVATE char *TimevalDisplay(displayTimevalP, lengthP)
  IN var_val_t *displayTimevalP;
  OUT int *lengthP;
{
    char *displayStr;
    int strLength;
    struct timeval *timevalVal;

    timevalVal = (struct timeval *) VALUE_VAL(displayTimevalP);

    /*
     *  Calculations for the length of the string assume worst case for
     *  the fields of the timeval data type.  That is they are assumed to
     *  be 10 characters large.
     *  An additional byte is added for the end of string character, and
     *  the length of the labels used in the sprintf that follows are
     *  also included.
     */
    strLength = 19 +  /* sec field */
	        25 +  /* usec field */
		6  +  /* spaces and newline */
		5;    /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "Seconds: %10d   Micro-seconds: %10d\n", 
	    timevalVal->tv_sec, timevalVal->tv_usec);
    assert((int) strlen(displayStr) < strLength);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);
}

PRIVATE var_val_t *TimevalGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum, convertVal;
    struct timeval *timevalValP;

    timevalValP = (struct timeval *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, timevalFieldNames,
			sizeof(timevalFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* tv_sec */
	    convertVal = (int) timevalValP->tv_sec;
	    break;

	  case 1:   /* tv_usec */
	    convertVal = (int) timevalValP->tv_usec;
	    break;
	}

	retValP = itlStds_IntVal(convertVal);
    }

    return(retValP);
}


PRIVATE boolean_t TimevalSetField(setValP, fieldName, fieldValP)
  IN var_val_t *setValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    int fieldNum;
    struct timeval *timevalValP;
    boolean_t retVal = TRUE;

    if (VALUE_TYPE(fieldValP) == itl_intType)
    {
	timevalValP = (struct timeval *) VALUE_VAL(setValP);

	if (itl_LookupTable(fieldName, timevalFieldNames,
			    sizeof(timevalFieldNames) / sizeof(char *),
			    &fieldNum) == 1)
	{
	    switch (fieldNum) {
	      case 0:   /* tv_sec */
		timevalValP->tv_sec = *((unsigned long *) VALUE_VAL(fieldValP));
		break;

	      case 1:   /* tv_usec */
		timevalValP->tv_usec = *((unsigned long *) VALUE_VAL(fieldValP));
		break;
	    }
	}
    }
    else
	retVal = FALSE;

    return(retVal);
}

/*
 *  itlCommon_TimevalCreate()
 *    Given a value for seconds and micro-seconds, create a var_val_t that
 *    contains that data.
 */
SHARED var_val_t *itlCommon_TimevalCreate(seconds, microSeconds)
  IN unsigned long seconds, microSeconds;
{
    var_val_t  *newVarP, *copyVarP;
    struct timeval timeData;

    timeData.tv_sec = seconds;
    timeData.tv_usec = microSeconds;
    newVarP = itlValue_New((pointer_t) &timeData, itlCommon_timevalDataType);
    copyVarP = itlValue_Copy(newVarP);
    ITL_FREE(newVarP);
    return(copyVarP);
}
