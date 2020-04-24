/*
 * ID: $Id: itl_bakCommon.c,v 1.2 1993/06/21 21:06:19 fred Exp $
 *
 * COMPONENT_NAME: itl_bakCommon.c
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1993
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: $
 *
 * $TALog: itl_bakCommon.c,v $
 * Revision 1.2  1993/06/21  21:06:19  fred
 * The charListT type was still not working correctly.  Since there were
 * so many exceptions for it in the common list type code I made it its
 * own data type.
 *
 * Move GetArrayIndex to here from itl_bakListType.c, since it is used also
 * by the char list code.  Renamed to itlBak_CommonGetArrayIndex().
 * [from r1.1 by delta fred-db3729-fix-bakserver_test-save-text, r1.2]
 *
 * Revision 1.1  1993/05/18  20:16:39  fred
 * To add an ITL based test program for the backup server.
 * [added by delta fred-db3540-add-itl-bak-tests, r1.1]
 *
 *
 * $EndLog$
 */

#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>

PRIVATE pthread_mutex_t localTimeMutex;

PRIVATE void CommonInit()
{
    pthread_mutex_init(&localTimeMutex, pthread_mutexattr_default);
}

SHARED void itlBak_CommonInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) CommonInit))
	itl_Error("pthread_once failed in CommonInit\n");
}

/*
 * itlBak_CommonGetLimitedStr() -
 *    Function to get a string that has a maximum length limit.
 *
 * Arguments:
 *    _fname - name of the string we are prompting for
 *    _maxLen - maximum length of the string, including end-of-string char.
 *    _dptr - pointer where the obtained string will be copied to.
 * RETURN CODES:
 *    TRUE  - success
 *    FALSE - failure
 */
SHARED boolean_t itlBak_CommonGetLimitedStr(fieldName, maxLength, dataP)
  IN char *fieldName;         /* name of the string we are prompting for */
  IN unsigned int maxLength;  /* max. length of string, including '\0' */
  OUT char *dataP;            /* ptr where the string will be copied to */
{
    char *strP;
    int strLength;
    boolean_t eof, retVal = TRUE;

    itl_PromptUser("%s: ", fieldName);
    strP = itl_GetString(&eof, &strLength);
    assert(strP);
    if (strLength < maxLength)
	strcpy(dataP, strP);
    else
    {
	itl_Error("%s length exceeds maximum of %d\n", fieldName, maxLength);
	retVal = FALSE;
    }

    ITL_FREE(strP);
    return(retVal);
}

/*
 * itlBak_CommonGetTime() -- 
 *    Reads in a date and time string and converts them to a time in 
 *    seconds value.
 * 
 * RETURN CODES:
 *    The time in seconds, or 0.
 */
SHARED time_t itlBak_CommonGetTime(fieldName, acceptFull)
  IN char *fieldName;        /* name of field that date is for */
  IN boolean_t acceptFull;   /* TRUE - then "full" for date returns 0 */
                             /* this handles the incTime field in the */
                             /* budb_dumpEntry data type */
{
    boolean_t eof;
    struct tm *tmPtr, tmData;
    char *separatorP, *fieldP, *strP;
    int j, *tmFieldP, strLength;
    time_t now;

    /*
     *  Ask for time in format easiest for the user, i.e.
     *  date and time.
     */
    itl_AdviseUser("Entering the empty string (\"\") for the date\n");
    itl_AdviseUser("will cause the current time to be used.\n");

    if (acceptFull) 
    {
	itl_AdviseUser("\nEntering the string (\"full\") for the date\n");
	itl_AdviseUser("will set the field, %s, to 0\n\n", fieldName);
    }

    itl_PromptUser("%s Date (\"mm/dd/yy\"): ", fieldName);
    strP = itl_GetString(&eof, &strLength);
    assert(strP);

    if (acceptFull)
    {
	if (!strcmp("full", strP))
	{
	    ITL_FREE(strP);
	    return((time_t) 0);   /* for incTime field in the */
	                          /* budb_dumpEntry data type */
	}
    }

    /*
     *  A mutex is used here because the man page for localtime
     *  says it returns a pointer to a static data area, i.e. it
     *  is not thread safe.
     */
    pthread_mutex_lock(&localTimeMutex);
    now = time((time_t *) 0);
    tmPtr = (struct tm *) localtime(&now); /* init time info */
    /*
     *  Copy the data so we can drop the mutex.
     */
    tmData = *tmPtr;
    pthread_mutex_unlock(&localTimeMutex);

    if (!strcmp(strP, ""))
    {
	ITL_FREE(strP);
	return(now);
    }

    /*
     *  Update tm structure with users date.
     */
    for (fieldP = strP, j = 0; j < 3; j++)
    {
	switch (j) {
	  case 0:
	    tmFieldP = &(tmData.tm_mon);
	    break;
	  case 1:
	    tmFieldP = &(tmData.tm_mday);
	    break;
	  case 2:
	    tmFieldP = &(tmData.tm_year);
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

	/*
	 *  mktime treats the month based on 0-11 rather than
	 *  the more intuitive 1-12.  (This must be a UNIXism
	 *  from days of old.)  So subtract one from the month
	 *  entered by the user.
	 */ 
	tmData.tm_mon = tmData.tm_mon - 1; 
    }

    ITL_FREE(strP);

    itl_PromptUser("%s Time (\"hh:mm:ss\"): ", fieldName);
    strP = itl_GetString(&eof, &strLength);
    assert(strP);

    /*
     *  Update tm structure with users time.
     */
    for (fieldP = strP, j = 0; j < 3; j++)
    {
	switch (j) {
	  case 0:
	    tmFieldP = &(tmData.tm_hour);
	    break;
	  case 1:
	    tmFieldP = &(tmData.tm_min);
	    break;
	  case 2:
	    tmFieldP = &(tmData.tm_sec);
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

    ITL_UTIL_FREE(strP);

    return(mktime(&tmData));
}

/*
 *  itlBak_CommonGetArrayIndex() --
 *    Returns the index into one of the list arrays from a user supplied
 *    fieldName.  So if the user had an ITL variable called volList, then 
 *    they would access one of the volumeEntry entries in the list as
 *    volList.budb_volumeList_val[n], where n is a number.  This function
 *    returns that number.  Also it checks to see if anything comes after
 *    the array index in square brackets, e.g. 
 *    volList.budb_volumeList_val[n].junk.  In this case it returns FALSE,
 *    otherwise it returns TRUE.
 */
EXPORT boolean_t itlBak_CommonGetArrayIndex(fieldName, indexP)
  IN char *fieldName;
  OUT int *indexP;
{
    int indexValue;
    char *leftBracket;
    boolean_t retVal = FALSE;

    /*
     *  Check for garbage after the right bracket.
     */
    if (fieldName[strlen(fieldName) - 1] == ']')
    {
	leftBracket = strchr(fieldName, '[');
	if (sscanf((leftBracket + 1), "%d", &indexValue) == 1)
	{
	    *indexP = indexValue;
	    retVal = TRUE;
	}
    }

    return(retVal);
}

