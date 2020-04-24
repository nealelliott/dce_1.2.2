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
 * itl_commonSockaddr.c -- 
 *    This file provides functions for implementing the struct sockaddr 
 *    data type which may be used through out the ITL API.
 * 
 */
/*
 * HISTORY
 * $Log: itl_commonSockaddr.c,v $
 * Revision 1.1.319.1  1996/10/17  18:25:30  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:50  damon]
 *
 * Revision 1.1.314.1  1994/02/04  20:43:26  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:39  devsrc]
 * 
 * Revision 1.1.312.2  1994/01/20  18:44:01  annie
 * 	added copyright header
 * 	[1994/01/20  18:40:19  annie]
 * 
 * Revision 1.1.312.1  1993/12/07  17:43:28  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:08:53  jaffe]
 * 
 * $EndLog$
 */


#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <dcedfs/common_data.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_vectype.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_commonSockaddr.h>

#ifdef	SUNOS5   /* defined in mman.h, can you believe it */
#undef	PRIVATE
#define PRIVATE static
#endif

int itlCommon_sockaddrDataType;

PRIVATE var_val_t *itlCommon_SockaddrNew _TAKES((
					IN pointer_t newSockaddrP,
					IN int newType
					));

PRIVATE boolean_t itlCommon_SockaddrFree _TAKES((
					IN var_val_t *freeSockaddrP
					));

PRIVATE var_val_t *itlCommon_SockaddrCopy _TAKES((
					 IN var_val_t *copySockaddrP
					 ));

PRIVATE char *itlCommon_SockaddrDisplay _TAKES((
				       IN var_val_t *displaySockaddrP,
				       OUT int *lengthP
				       ));

PRIVATE boolean_t itlCommon_SockaddrBinaryOp _TAKES((
				       IN var_val_t *leftOpP,
				       IN var_val_t *rightOpP,
				       IN char *binaryOp
                                       ));

PRIVATE var_val_t *itlCommon_SockaddrGetField _TAKES((
                                         IN var_val_t *getValP,
                                         IN char *fieldName
                                         ));

PRIVATE boolean_t itlCommon_SockaddrSetField _TAKES((
                                        IN var_val_t *setValP,
                                        IN char *fieldName,
                                        IN var_val_t *fieldValP
                                        ));


IMPORT char * hostutil_GetNameByINet _TAKES((
                                             struct in_addr       addr
                                           ));

PRIVATE itlValue_varTypeInfo_t commonSockaddrVarType = {
    "common_Sockaddr",
    itlCommon_SockaddrCopy,
    itlCommon_SockaddrNew,
    itlCommon_SockaddrFree,
    NULL,
    itlCommon_SockaddrBinaryOp,
    NULL,
    NULL,
    NULL,
    itlCommon_SockaddrDisplay,
    itlCommon_SockaddrGetField,
    itlCommon_SockaddrSetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *sockaddrFieldNames[] = {
    "sa_family",
    "sa_data"
};


/*
 *  SockaddrInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void SockaddrInit()
{
    /*
     *  We can safely set sockaddrHashType because this function is called
     *  via pthread_once.
     */
    itlCommon_sockaddrDataType = 
          itlValue_RegisterVarType(&commonSockaddrVarType); 
}

/*
 * itlCommon_SockaddrInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlCommon_SockaddrInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) SockaddrInit))
	itl_Error("pthread_once failed in SockaddrInit\n");
}

/**
 **  Comm_sockAddr variable rouines.
 **/

PRIVATE var_val_t *itlCommon_SockaddrNew(newSockaddrP, newType)
  IN pointer_t newSockaddrP;
  IN int newType;
{
    var_val_t *newVarP;
    struct sockaddr *sockaddrP = (struct sockaddr *) newSockaddrP;

    ITL_UTIL_NEW(newVarP, var_val_t);
    VALUE_TYPE(newVarP) = newType;
    VALUE_VAL(newVarP) = newSockaddrP;
    VALUE_SIZE(newVarP) = (int) sizeof(struct sockaddr);
    return(newVarP);
}

PRIVATE boolean_t itlCommon_SockaddrFree(freeSockaddrP)
  IN var_val_t *freeSockaddrP;
{
    if (VALUE_VAL(freeSockaddrP))
	ITL_UTIL_FREE(VALUE_VAL(freeSockaddrP));
    return(FALSE);
}

PRIVATE var_val_t *itlCommon_SockaddrCopy(copySockaddrP)
  IN var_val_t *copySockaddrP;
{
    struct sockaddr *sockaddrP;

    assert(VALUE_TYPE(copySockaddrP) == itlCommon_sockaddrDataType);
    ITL_NEW(sockaddrP, struct sockaddr);
    ITL_UTIL_MEMCOPY(VALUE_VAL(copySockaddrP), sockaddrP, 
		     VALUE_SIZE(copySockaddrP));
    return(itlCommon_SockaddrNew((pointer_t) sockaddrP, 
				 itlCommon_sockaddrDataType));
}

PRIVATE char *itlCommon_SockaddrDisplay(displaySockaddrP, lengthP)
  IN var_val_t *displaySockaddrP;
  OUT int *lengthP;
{
    struct sockaddr *sockaddrP;
    int fieldLength, strLength;
    char *returnStr;
    char * hostNameP;
    char *indexP;
    int i;

    sockaddrP = (struct sockaddr *) VALUE_VAL(displaySockaddrP);

    /*
     *  First obtain display strings for all sub-fields that are
     *  data types themselves.
     */
    assert(sockaddrP);

    /*
     *  Lengths below are calculated assuming worst case, 
     *  that is that a short will take up 10 bytes in a string; 
     *  10 for the field name and 1 for end of string character.
     *  It is rough figure but the assertion following the 
     *  code will let us know if there is enough memory or not.
     */
    strLength =  10 +  /* family address */
	         36 +  /*  sockaddr */
                 60 +  /*  host name   */
	         10;   /* padding */

    /*
     *  Length of the strings are explicitly calculated, because sprintf
     *  did not do it correctly on every platform, i.e. this is guaranteed
     *  to be portable.
     */
    ITL_UTIL_ALLOC(returnStr, char *, strLength);

    indexP=returnStr;
    sprintf(returnStr, "\n  Family: %d\n  Socket Data: ", 
	    sockaddrP->sa_family);
    indexP += strlen(returnStr);

    for ( i = 0; i < (sizeof(struct sockaddr) - 2); i++ ) {
      sprintf(indexP, "%02x", (unsigned char) sockaddrP->sa_data[i]);
      indexP +=2;
    }
    
    indexP = returnStr + strlen(returnStr); 
    hostNameP = hostutil_GetNameByINet(
			 ((struct sockaddr_in *) sockaddrP)->sin_addr);
    sprintf(indexP, "\n  Host Name: %s\n", hostNameP);

    assert((int) strlen(returnStr) < strLength);

    if (lengthP)
	*lengthP = strLength;

    return(returnStr);
}


/*
 *  itlCommon_SockaddrCreate()
 *    Given an struct sockaddr return a pointer to a var_val_t that 
 *    contains that data.
 */
SHARED var_val_t *itlCommon_SockadrrCreate(sockaddrP)
  IN struct sockaddr *sockaddrP;
{
    var_val_t  *newVarP, *copyVarP;
    struct sockaddr sockaddrData;

    sockaddrData = *sockaddrP;
    newVarP = itlValue_New((pointer_t) &sockaddrData, itlCommon_sockaddrDataType);
    copyVarP = itlValue_Copy(newVarP);
    ITL_FREE(newVarP);
    return(copyVarP);
}

PRIVATE boolean_t itlCommon_SockaddrBinaryOp(leftOpP, rightOpP, binaryOp)
  IN var_val_t *leftOpP;
  IN var_val_t *rightOpP;
  IN char *binaryOp;
{
    boolean_t validOp = FALSE, opSucceeded = FALSE, areEqual = FALSE;
    var_val_t *opResultP;
    struct sockaddr *leftVal, *rightVal;

    leftVal = (struct sockaddr *) VALUE_VAL(leftOpP);
    rightVal = (struct sockaddr *) VALUE_VAL(rightOpP);

#ifdef _AIX
    if ((leftVal->sa_len == rightVal->sa_len) &&
	(leftVal->sa_family == rightVal->sa_family) &&
#endif /* _AIX */
#if defined(SUNOS5) || defined(__hpux)
    if ((leftVal->sa_family == rightVal->sa_family) &&
#endif /* SUNOS5 */
	(!strcmp(leftVal->sa_data, rightVal->sa_data)))
	areEqual = TRUE;
	
    switch (*binaryOp) {
      case '=' :
	  if (binaryOp[1] == '=')
	      opSucceeded = areEqual;
	  else
	      validOp = TRUE;
	  break;

      case '!' :
	  if (binaryOp[1] == '=')
	      opSucceeded = !areEqual;
	  else
	      validOp = TRUE;
	  break;

      default :
	  validOp = TRUE;
    }

    if (validOp == TRUE)
	itl_Error("Binary operation, %s, is NOT supported/valid on sockaddr's.\n", 
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


PRIVATE var_val_t *itlCommon_SockaddrGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    struct sockaddr *sockaddrValP;
    char *vecVal;

    sockaddrValP = (struct sockaddr *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, sockaddrFieldNames,
			sizeof(sockaddrFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* sa_family */
	    retValP = itlStds_IntVal((int) sockaddrValP->sa_family);
	    break;

	  case 1:   /*  14 bytes sa_data */
            retValP = itl_SetVectorType(sockaddrValP->sa_data,
					sizeof(struct sockaddr) - 
					sizeof(unsigned short));
	    break;
	}

    }
    return(retValP);
}


PRIVATE boolean_t itlCommon_SockaddrSetField(setValP, fieldName, fieldValP)
  IN var_val_t *setValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    int fieldNum;
    struct sockaddr *sockaddrValP;
    boolean_t retVal = TRUE;

    if (VALUE_TYPE(fieldValP) == itl_intType)
    {
	sockaddrValP = (struct sockaddr *) VALUE_VAL(setValP);

	if (itl_LookupTable(fieldName, sockaddrFieldNames,
			    sizeof(sockaddrFieldNames) / sizeof(char *),
			    &fieldNum) == 1)
	{
	    switch (fieldNum) {
	      case 0:   /* sa_family */
#ifdef _AIX
		sockaddrValP->sa_family = *((unsigned char *) 
					    VALUE_VAL(fieldValP));
#endif /* _AIX */
#ifdef SUNOS5
		sockaddrValP->sa_family = *((unsigned short *) 
					    VALUE_VAL(fieldValP));
#endif /* SUNOS5 */
		break;

	      case 1:   /* sa_data */
		memcpy(sockaddrValP->sa_data, VALUE_VAL(fieldValP),                                    sizeof(struct sockaddr) - sizeof(unsigned short));
		break;
	    }
	}
    }
    else
	retVal = FALSE;

    return(retVal);
}

