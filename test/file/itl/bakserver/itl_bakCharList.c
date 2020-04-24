/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakCharList.c --
 *    Code for the implementing the charListT type.  Yes, it is
 *    very similar to the other list types, but it is also significantly
 *    different to warrant its own type.  It requires a read function,
 *    and since it is so similar to a C string type, it has peculiar
 *    semantics.
 */
/*
 * HISTORY
 * $Log: itl_bakCharList.c,v $
 * Revision 1.1.6.1  1996/10/17  18:24:26  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:23  damon]
 *
 * $EndLog$
 */

#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <dce/rpc.h>
#include <dcedfs/common_data.h>
#include <dcedfs/budb.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_bakCommon.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakCharList.c,v 1.1.6.1 1996/10/17 18:24:26 damon Exp $")

int itlBak_charListDataType;


PRIVATE var_val_t *CharListCopy _TAKES((
					IN var_val_t *copyVarP
					));

PRIVATE var_val_t *CharListNew _TAKES((
				       IN pointer_t newValP,
				       IN int newType
				       ));

PRIVATE boolean_t CharListFree _TAKES((
				       IN var_val_t *freeVarP
				       ));

PRIVATE var_val_t *CharListRead _TAKES((
					IN var_val_t *readCharListP,
					IN int varType
					));

PRIVATE boolean_t CharListBinaryOp _TAKES((
					   IN var_val_t *leftOpP,
					   IN var_val_t *rightOpP,
					   IN char *binaryOp
					   ));

PRIVATE char *CharListDisplay _TAKES((
				      IN var_val_t *displayVarP,
				      OUT int *lengthP
				      ));

PRIVATE var_val_t *CharListGetField _TAKES((
					    IN var_val_t *getVarP,
					    IN char *fieldName
					    ));

PRIVATE itlValue_varTypeInfo_t charListVarType = {
    "fs_bakCharList",
    CharListCopy,
    CharListNew,
    CharListFree,
    CharListRead,
    CharListBinaryOp,
    NULL,
    NULL,
    NULL,
    CharListDisplay,
    CharListGetField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

/*
 *  CharListInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void CharListInit()
{
    /*
     *  We can safely set tapeSetHashType because this function is called
     *  via pthread_once.
     */
    itlBak_charListDataType = itlValue_RegisterVarType(&charListVarType);
}

/*
 * itlBak_CharListInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlBak_CharListInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) CharListInit))
	itl_Error("pthread_once failed in CharListInit\n");
}


PRIVATE var_val_t *CharListCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    charListT *listValP = (charListT *) VALUE_VAL(copyVarP);
    charListT *newListP;

    assert(VALUE_TYPE(copyVarP) == itlBak_charListDataType);
    assert(listValP->charListT_len < BUDB_MAX_CHAR_LIST);

    ITL_NEW(newListP, charListT);
    newListP->charListT_len = listValP->charListT_len;
    (void) memcpy((char *) newListP->charListT_val,
		  (char *) listValP->charListT_val,
		  (int) listValP->charListT_len);
    return(CharListNew((pointer_t) newListP, itlBak_charListDataType));
}

PRIVATE var_val_t *CharListNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newHeaderVarP;
    charListT *newListP = (charListT *) newValP;

    assert(newType == itlBak_charListDataType);
    ITL_NEW(newHeaderVarP, var_val_t);
    VALUE_TYPE(newHeaderVarP) = itlBak_charListDataType;
    VALUE_SIZE(newHeaderVarP) = sizeof(charListT);
    VALUE_VAL(newHeaderVarP) = newValP;
    return(newHeaderVarP);
}

PRIVATE boolean_t CharListFree(freeVarP)
  IN var_val_t *freeVarP;
{
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}

PRIVATE var_val_t *CharListRead(readCharListP, varType)
  IN var_val_t *readCharListP;
  IN int varType;
{
    var_val_t *newVarP;

    assert(varType == itlBak_charListDataType);
    if (!readCharListP)
    {
	itl_PromptUser("Char List Length: ");
	readCharListP = itl_GetVarVal();

	if (VALUE_TYPE(readCharListP) == itlBak_charListDataType)
	    newVarP = readCharListP;
	else
	{
	    charListT *newCharListP;
	    int strLength, dataLength;
	    char *dataP;

	    /*
	     *  The variable we just read needs to have been an integer.
	     */
	    newVarP = itlValue_ConvertValue(readCharListP, itl_intType);
	    assert(newVarP);
	    strLength = INT_VALUE_VAL(newVarP);
	    itlValue_Free(newVarP);
	    ITL_GET_STRING("Char List Data: ", dataP, &dataLength);
	    dataLength++;   /* account for '\0' */
	    /*
	     *  If the user entered 0 for the length, we just use the 
	     *  length of the string for the len field.  Otherwise we
	     *  use what the user specifies.
	     */
	    if (strLength > 0)
	    {
		/*
		 *  Still must check length against the actual length
		 *  of the string.  If the string is longer we allocate
		 *  enough space for the string, but set the length as
		 *  requested.  This allows for test that check to see
		 *  that the bakserver correctly deals with charListT's
		 *  where the data and length are not correctly matched.
		 */
		dataLength = (strLength > dataLength) ? strLength 
		                                        : dataLength;
	    }
	    else
	    {
		strLength = dataLength;
	    }
	    /*
	     *  If the length is larger than, BUDB_MAX_CHAR_LIST,
	     *  we issue a message and only allocate BUDB_MAX_CHAR_LIST.
	     */
	    if (dataLength > BUDB_MAX_CHAR_LIST)
	    {
		itl_Message("WARNING: You are asking that more than %d bytes be allocated.\n",
			    BUDB_MAX_CHAR_LIST);
		itl_Message("         Will only use the first %d bytes.\n",
			    BUDB_MAX_CHAR_LIST);
		dataLength = BUDB_MAX_CHAR_LIST;
	    }

	    /*
	     *  The memory allocated here for newCharListP is free'd in the 
	     *  CharListFree() routine.
	     */
	    ITL_NEW(newCharListP, charListT);
	    (void) memcpy((char *) newCharListP->charListT_val, dataP, 
			  dataLength);
	    ITL_FREE(dataP);
	    newCharListP->charListT_len = strLength;

	    newVarP = CharListNew((pointer_t) newCharListP, 
				  itlBak_charListDataType);
	}
    }
    else
	newVarP = (var_val_t *) 0;

    return(newVarP);
}

/*
 *  CharListBinaryOp() --
 *    Just compares two charListT's.
 *
 *  WARNING:
 *    The return value (boolean) indicates if the operation requested is
 *    valid.  Oddly TRUE means it is not a valid operation and FALSE 
 *    implies it is a valid operation.  Its says nothing about the success 
 *    of the comparison.
 *    The value of leftOpP indicates if the operation succeeded.
 */
PRIVATE boolean_t CharListBinaryOp(leftOpP, rightOpP, binaryOp)
  IN var_val_t *leftOpP;
  IN var_val_t *rightOpP;
  IN char *binaryOp;
{
    boolean_t validOp, opSucceeded, areEqual;
    charListT *leftValP, *rightValP;
    var_val_t *opResultP;

    leftValP = (charListT *) VALUE_VAL(leftOpP);
    rightValP = (charListT *) VALUE_VAL(rightOpP);

    validOp = opSucceeded = areEqual = FALSE;

    if ((leftValP->charListT_len == rightValP->charListT_len) &&
	(!memcmp((char *) leftValP->charListT_val,
		 (char *) rightValP->charListT_val,
		 (int) leftValP->charListT_len)))
    {
	areEqual = TRUE;
    }

    switch (*binaryOp) {

      case '=' :
	  if (binaryOp[1] == '=')
	      opSucceeded = areEqual;
	  break;
	
      case '!' :
	  if (binaryOp[1] == '=')
	      opSucceeded = !areEqual;
	  break;

      case '/' :   /* look for rightVal as a "sub-string" of leftVal */
	  if (areEqual)
	      opSucceeded = TRUE;
	  else
	  {
	      if (leftValP->charListT_len >= rightValP->charListT_len)
	      {
		  int i;

		  /*
		   *  The following algorithm is very slow, but I currently
		   *  do not have the time to investigate a faster one.
		   *  Please replace if you know of a faster means of doing
		   *  the comparison.
		   */
		  for (i = 0; i <= (leftValP->charListT_len - 
				    rightValP->charListT_len); i++)
		  {
		      if (memcmp(&(leftValP->charListT_val[i]),
				 rightValP->charListT_val,
				 (int) rightValP->charListT_len) == 0)
		      {
			  opSucceeded = TRUE;
			  break;
		      }
		  }
	      }
	  }
	  break;

       default :
	   itl_Error("Binary operation, %s, NOT supported on fs_bakCharList.\n",
		     binaryOp);
	  validOp = TRUE;
      }

    /*
     *  Set the left value to be 1 if the comparison was successful, and
     *  0 if it was not.
     */
    opResultP = itlStds_IntVal(opSucceeded);
    itlValue_Free(leftOpP);
    *leftOpP = *opResultP;
    ITL_FREE(opResultP);

    return(validOp);
}

PRIVATE char *CharListDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    char *displayStrP, *valueStrP;
    int strLength;
    charListT *listP;

    assert(VALUE_TYPE(displayVarP) == itlBak_charListDataType);
    listP = (charListT *) VALUE_VAL(displayVarP);

    valueStrP = itl_DisplayBytes((char *) listP->charListT_val,
				 (unsigned long) listP->charListT_len,
				 TRUE, (unsigned long *) &strLength);
    strLength += 10 + 8 + /* length field */
                 7 +      /* value field label */
		 10;      /* newlines and padding */

    ITL_ALLOC(displayStrP, char *, strLength);
    sprintf(displayStrP, "  Length: %d\n  Value:\n%s\n", 
	    listP->charListT_len, valueStrP);

    ITL_FREE(valueStrP);

    return(displayStrP);
}

PRIVATE var_val_t *CharListGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    charListT *listP;
    int index;
    var_val_t *newVarP;


    listP = (charListT *) VALUE_VAL(getValP);

    /*
     *  Determine if we need to get the length field or one of the
     *  entries from the val list array.
     */
    if (strstr(fieldName, "len"))
    {
	retValP = itlStds_IntVal((int) (listP->charListT_len));
    }
    else 
    {
	/*
	 *  Accessing one of the items in the val list array.
	 *  The field name is scanned looking for a '[' and an 
	 *  accompanying array index.
	 *  NOTE:
	 *    For now, if the user requests a field within one of the
	 *    array elements we gripe.
	 */

	if (itlBak_CommonGetArrayIndex(fieldName, &index))
	{
	    if ((index < (listP->charListT_len)) && (index >= 0))
	    {
		int oneChar;

		oneChar = (int) listP->charListT_val[index];
		newVarP = itlValue_New((pointer_t) &oneChar, itl_intType);
		retValP = itlValue_Copy(newVarP);
		ITL_FREE(newVarP);
	    }
	    else
	    {
		itl_Error("Index, %d, for %s, is invalid.\n", 
			  index, "charListT_val");
	    }
	}
	else
	{
	    char *stringData;

	    /*
	     *  We treat the val field as a string if no array index is
	     *  given.
	     */

	    /*
	     *  Use calloc to zero out the memory, and implicitly place
	     *  a null at the end of the string.
	     */
	    ITL_CALLOC(stringData, char *, (listP->charListT_len + 1));
	    ITL_UTIL_MEMCOPY((char *) listP->charListT_val,
			     stringData, listP->charListT_len);
	    newVarP = itlValue_New((pointer_t) stringData, itl_stringType);
	    retValP = itlValue_Copy(newVarP);
	    ITL_FREE(newVarP);
	    ITL_FREE(stringData);
	}
    }

    return(retValP);
}


/*
 *  itlBak_CharListVariableGet() --
 *    Will prompt for and obtain the value of a charListT variable.
 *    It returns both a name and charListT because its usage in the backup
 *    interface is for INOUT type parameters.  And to accomplish this
 *    in ITL you need to keep track of the name so you can reset its
 *    value.
 *
 *  NOTE:
 *    Caller is responsible for freeing the name and charList that
 *    are returned.
 *
 *  RETURN CODES:
 *    TRUE  - success
 *    FALSE - failure
 */
EXPORT boolean_t itlBak_CharListVariableGet(varNamePP, charListPP)
  OUT char **varNamePP;
  OUT charListT **charListPP;
{
    itl_var_t *varP;
    var_val_t *listVarP;
    charListT *charListP;
    char *varName;
    boolean_t err, retVal = FALSE;

    itl_PromptUser("Char List Variable Name: ");
    varName = itl_GetVariableName(&err, NULL);
    if ((varP = itl_LocateVariable(varName)) != (itl_var_t *) 0)
    {
	listVarP = VAR_VALUE_REC(varP);
	charListP = (charListT *) VALUE_VAL(listVarP);

	/*
	 *  Make a copy, because we cannot allow operations on the 
	 *  original value.
	 */
	ITL_NEW(*charListPP, charListT); 
	(void) memcpy((char *) *charListPP, (char *) charListP,
		      charListP->charListT_len);
	*varNamePP = varName;
	retVal = TRUE;
    }

    if (retVal == FALSE)
	ITL_FREE(varName);

    return(retVal);
}

/*
 *  itlBak_CharListDataGet() --
 *    Will prompt for and obtain a charListT.
 *
 */
EXPORT void itlBak_CharListDataGet(charListP)
  OUT charListT *charListP;  /* ptr to caller allocated structure */
{
    var_val_t *listVarP;

    itl_AdviseUser("Char List -");
    listVarP = CharListRead((var_val_t *) 0, itlBak_charListDataType);
    assert(listVarP);
    *charListP = *((charListT *) VALUE_VAL(listVarP));
    itl_FreeVarVal(listVarP);
}
