/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakTapeSet.c --
 *    Code for the budb_tapeSet data type.
 */
/*
 * HISTORY
 * $Log: itl_bakTapeSet.c,v $
 * Revision 1.1.6.1  1996/10/17  18:24:58  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:35  damon]
 *
 * $EndLog$
 */

#include <assert.h>
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

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakTapeSet.c,v 1.1.6.1 1996/10/17 18:24:58 damon Exp $")

int itlBak_tapeSetDataType;


PRIVATE var_val_t *TapeSetCopy _TAKES((
				       IN var_val_t *copyVarP
				       ));

PRIVATE var_val_t *TapeSetNew _TAKES((
				      IN pointer_t newValP,
				      IN int newType
				      ));

PRIVATE boolean_t TapeSetFree _TAKES((
				      IN var_val_t *freeVarP
				      ));

PRIVATE var_val_t *TapeSetRead _TAKES((
				       IN var_val_t *readTapeSetP,
				       IN int varType
				       ));

PRIVATE boolean_t TapeSetBinaryOp _TAKES((
					  IN var_val_t *leftOpP,
					  IN var_val_t *rightOpP,
					  IN char *binaryOp
					  ));

PRIVATE char *TapeSetDisplay _TAKES((
				     IN var_val_t *displayVarP,
				     OUT int *lengthP
				     ));

PRIVATE var_val_t *TapeSetGetField _TAKES((
					   IN var_val_t *getVarP,
					   IN char *fieldName
					   ));

PRIVATE boolean_t TapeSetSetField _TAKES((
					  IN var_val_t *setValP,
					  IN char *fieldName,
					  IN var_val_t *fieldValP
					  ));


PRIVATE itlValue_varTypeInfo_t tapeSetVarType = {
    "fs_bakTapeSet",
    TapeSetCopy,
    TapeSetNew,
    TapeSetFree,
    TapeSetRead,
    TapeSetBinaryOp,
    NULL,
    NULL,
    NULL,
    TapeSetDisplay,
    TapeSetGetField,
    TapeSetSetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *tapeSetFieldNames[] = {
    "id",
    "tapeServer",
    "format",
    "maxTapes",
    "a",
    "b",
    "spare1",
    "spare2",
    "spare3",
    "spare4"
};

/*
 *  TapeSetInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void TapeSetInit()
{
    /*
     *  We can safely set tapeSetHashType because this function is called
     *  via pthread_once.
     */
    itlBak_tapeSetDataType = itlValue_RegisterVarType(&tapeSetVarType);
}

/*
 * itlBak_TapeSetInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlBak_TapeSetInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) TapeSetInit))
	itl_Error("pthread_once failed in TapeSetInit\n");
}


PRIVATE var_val_t *TapeSetCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    budb_tapeSet *tapeSetValP = (budb_tapeSet *) VALUE_VAL(copyVarP);
    budb_tapeSet *newTapeSetP;

    assert(VALUE_TYPE(copyVarP) == itlBak_tapeSetDataType);
    ITL_NEW(newTapeSetP, budb_tapeSet);
    *newTapeSetP = *tapeSetValP;
    return(TapeSetNew((pointer_t) newTapeSetP, itlBak_tapeSetDataType));
}

PRIVATE var_val_t *TapeSetNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newTapeSetVarP;

    assert(newType == itlBak_tapeSetDataType);
    ITL_NEW(newTapeSetVarP, var_val_t);
    VALUE_TYPE(newTapeSetVarP) = newType;
    VALUE_SIZE(newTapeSetVarP) = sizeof(budb_tapeSet);
    VALUE_VAL(newTapeSetVarP) = newValP;
    return(newTapeSetVarP);
}

PRIVATE boolean_t TapeSetFree(freeVarP)
  IN var_val_t *freeVarP;
{
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}

PRIVATE var_val_t *TapeSetRead(readTapeSetP, varType)
  IN var_val_t *readTapeSetP;
  IN int varType;
{
    var_val_t *newVarP;

    assert(varType == itlBak_tapeSetDataType);
    if (!readTapeSetP)
    {
	itl_PromptUser("Id: ");
	readTapeSetP = itl_GetVarVal();

	if (VALUE_TYPE(readTapeSetP) == itlBak_tapeSetDataType)
	    newVarP = readTapeSetP;
	else
	{
	    budb_tapeSet *newTapeSetP;
	    int wordSize;
	    boolean_t eof;
	    char *strP;

	    /*
	     *  The memory allocated here for newTapeSetP is free'd in the 
	     *  TapeSetFree() routine.
	     */
	    ITL_NEW(newTapeSetP, budb_tapeSet);
	    newVarP = itlValue_ConvertValue(readTapeSetP, itl_intType);
	    assert(newVarP);
	    newTapeSetP->id = *((signed32 *) (VALUE_VAL(newVarP)));
	    itl_FreeVarVal(newVarP);

	    itlBak_CommonGetLimitedStr("TapeServer", BU_MAXHOSTLEN,
				       (char *) &(newTapeSetP->tapeServer));
	    itlBak_CommonGetLimitedStr("Format", BU_MAXTAPELEN,
				       (char *) &(newTapeSetP->format));

	    itl_PromptUser("maxTapes: ");
	    newTapeSetP->maxTapes = (signed32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("A: ");
	    newTapeSetP->a = (signed32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("B: ");
	    newTapeSetP->b = (signed32) itl_GetInteger((boolean_t *) 0);

	    itl_PromptUser("Spare1: ");
	    newTapeSetP->spare1 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare2: ");
	    newTapeSetP->spare2 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare3: ");
	    newTapeSetP->spare3 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare4: ");
	    newTapeSetP->spare4 = (unsigned32) itl_GetInteger((boolean_t *) 0);

	    newVarP = TapeSetNew((pointer_t) newTapeSetP, 
				   itlBak_tapeSetDataType);
	}
    }
    else
	newVarP = (var_val_t *) 0;

    return(newVarP);
}

/*
 *  itlBak_TapeSetCompare() --
 *    Compares to budb_tapeSet structures and returns TRUE if they
 *    are equal and FALSE if not.  This function is needed so higher
 *    level data types (containing a budb_tapeSet) can determine if their
 *    structures are equal.
 *
 * RETURN CODES:
 *    TRUE  - equal
 *    FALSE - not equal
 */
EXPORT boolean_t itlBak_TapeSetCompare(tapeSet1, tapeSet2)
  IN budb_tapeSet *tapeSet1;
  IN budb_tapeSet *tapeSet2;
{
    boolean_t retVal = FALSE;

    if ((tapeSet1->id == tapeSet2->id) &&
	(!strcmp((char *) tapeSet1->tapeServer, 
		 (char *) tapeSet2->tapeServer)) &&
	(!strcmp((char *) tapeSet1->format, (char *) tapeSet2->format)) &&
	(tapeSet1->maxTapes == tapeSet2->maxTapes) &&
	(tapeSet1->a == tapeSet2->a) &&
	(tapeSet1->b == tapeSet2->b) &&
	(tapeSet1->spare1 == tapeSet2->spare1) &&
	(tapeSet1->spare2 == tapeSet2->spare2) &&
	(tapeSet1->spare3 == tapeSet2->spare3) &&
	(tapeSet1->spare4 == tapeSet2->spare4))
	retVal = TRUE;

    return(retVal);
}

/*
 *  TapeSetBinaryOp() --
 *    Just compares two tapesets. 
 *
 *  WARNING:
 *    The return value (boolean) indicates if the operation requested is
 *    valid.  Oddly TRUE means it is not a valid operation and FALSE 
 *    implies it is a valid operation.  Its says nothing about the success 
 *    of the comparison.
 *    The value of leftOpP indicates if the operation succeeded.
 */
PRIVATE boolean_t TapeSetBinaryOp(leftOpP, rightOpP, binaryOp)
  IN var_val_t *leftOpP;
  IN var_val_t *rightOpP;
  IN char *binaryOp;
{
    boolean_t validOp = FALSE, opSucceeded = FALSE, areEqual;
    budb_tapeSet *leftValP, *rightValP;
    var_val_t *opResultP;

    leftValP = (budb_tapeSet *) VALUE_VAL(leftOpP);
    rightValP = (budb_tapeSet *) VALUE_VAL(rightOpP);

    /*
     *  Compare the two structures and save the result.
     */
    areEqual = itlBak_TapeSetCompare(leftValP, rightValP);
    
    switch (*binaryOp) {
      case '=' :
	  if (binaryOp[1] == '=')
	      opSucceeded = areEqual;
	  break;

      case '!' :
	  if (binaryOp[1] == '=')
	      opSucceeded = !areEqual;
	  break;

      default :
	  itl_Error("Operation, %s, NOT supported on fs_bakTapeSet's.\n",
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

/*
 *  itlBak_TapesetDisplay() --
 *    Allows data types that contain tapesets to get a display string
 *    without having to create an ITL variable and call itlValue_Display().
 */
EXPORT char *itlBak_TapeSetDisplay(tapeSetP, lengthP)
  IN budb_tapeSet *tapeSetP;
  OUT int *lengthP;               /* may be NULL */
{
    char *displayStr, *index;
    unsigned int strLength;

    /*
     *  Calculations for the length of the string assume worst case for
     *  the fields of the unsigned32 data type.  That is they are assumed to
     *  be 10 characters large.
     *  An additional byte is added for the end of string character, and
     *  the length of the labels used in the sprintf that follows are
     *  also included.
     */
    strLength = 10 + 4 +               /* id field */
	        BU_MAXHOSTLEN + 12 +   /* tapeServer field */
	        BU_MAXTAPELEN + 8 +    /* format field */
		10 + 11 +              /* maxTapes field */
		10 + 4 +               /* a field */
		10 + 4 +               /* b field */
		10 + 9 +               /* spare1 field */
		10 + 9 +               /* spare2 field */
		10 + 12 +              /* spare3 field */
		10 + 9 +               /* spare4 field */
		24 +                   /* newlines and spaces */
		10;                    /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "\n  Id: %d\n  TapeServer: %s\n  Format: %s\n", 
	    tapeSetP->id,
	    (char *) tapeSetP->tapeServer, (char *) tapeSetP->format);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  MaxTapes: %10d   A: %10d   B: %10d\n",
	    tapeSetP->maxTapes, tapeSetP->a, tapeSetP->b);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare1: %10d   Spare2: %10d\n",
	    tapeSetP->spare1, tapeSetP->spare2);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare3: %10d   Spare4: %10d\n",
	    tapeSetP->spare3, tapeSetP->spare4);

    assert(strlen(displayStr) < strLength);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);
}

PRIVATE char *TapeSetDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    budb_tapeSet *tapeSetValP;

    tapeSetValP = (budb_tapeSet *) VALUE_VAL(displayVarP);

    return(itlBak_TapeSetDisplay(tapeSetValP, lengthP));
}

PRIVATE var_val_t *TapeSetGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    budb_tapeSet *tapeSetValP;

    tapeSetValP = (budb_tapeSet *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, tapeSetFieldNames,
			sizeof(tapeSetFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* id */
	    retValP = itlStds_IntVal((int) tapeSetValP->id);
	    break;

	  case 1:   /* tapeServer */
	    retValP = itlStds_StringVal((char *) tapeSetValP->tapeServer);
	    break;

	  case 2:   /* format */
	    retValP = itlStds_StringVal((char *) tapeSetValP->format);
	    break;

	  case 3:   /* maxTapes */
	    retValP = itlStds_IntVal((int) tapeSetValP->maxTapes);
	    break;

	  case 4:   /* a */
	    retValP = itlStds_IntVal((int) tapeSetValP->a);
	    break;

	  case 5:   /* b */
	    retValP = itlStds_IntVal((int) tapeSetValP->b);
	    break;

	  case 6:   /* spare1 */
	    retValP = itlStds_IntVal((int) tapeSetValP->spare1);
	    break;

	  case 7:   /* spare2 */
	    retValP = itlStds_IntVal((int) tapeSetValP->spare2);
	    break;

	  case 8:   /* spare3 */
	    retValP = itlStds_IntVal((int) tapeSetValP->spare3);
	    break;

	  case 9:   /* spare4 */
	    retValP = itlStds_IntVal((int) tapeSetValP->spare4);
	    break;
	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retValP);
}


PRIVATE boolean_t TapeSetSetField(setValP, fieldName, fieldValP)
  IN var_val_t *setValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    int fieldNum;
    budb_tapeSet *tapeSetValP;
    boolean_t retVal = FALSE;

    if (itl_LookupTable(fieldName, tapeSetFieldNames,
			sizeof(tapeSetFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	tapeSetValP = (budb_tapeSet *) VALUE_VAL(setValP);

	switch (fieldNum) {
	  case 0:   /* id */
	    ITLU_SET_INT_FIELD(fieldValP, tapeSetValP->id, signed32, retVal);
	    break;

	  case 1:   /* tapeServer */
	    ITLU_SET_STRING_FIELD(fieldValP, &(tapeSetValP->tapeServer), 
				  BU_MAXHOSTLEN, retVal);
	    break;
	
	  case 2:   /* format */
	    ITLU_SET_STRING_FIELD(fieldValP, &(tapeSetValP->format), 
				  BU_MAXTAPELEN, retVal);
	    break;

	  case 3:   /* maxTapes */
	    ITLU_SET_INT_FIELD(fieldValP, tapeSetValP->maxTapes, signed32,
			       retVal);
	    break;

	  case 4:   /* a */
	    ITLU_SET_INT_FIELD(fieldValP, tapeSetValP->a, signed32, retVal);
	    break;

	  case 5:   /* b */
	    ITLU_SET_INT_FIELD(fieldValP, tapeSetValP->b, signed32, retVal);
	    break;

	  case 6:   /* spare1 */
	    ITLU_SET_INT_FIELD(fieldValP, tapeSetValP->spare1, unsigned32, 
			       retVal);
	    break;

	  case 7:   /* spare2 */
	    ITLU_SET_INT_FIELD(fieldValP, tapeSetValP->spare2, unsigned32, 
			       retVal);
	    break;

	  case 8:   /* spare3 */
	    ITLU_SET_INT_FIELD(fieldValP, tapeSetValP->spare3, unsigned32, 
			       retVal);
	    break;

	  case 9:   /* spare4 */
	    ITLU_SET_INT_FIELD(fieldValP, tapeSetValP->spare4, unsigned32, 
			       retVal);
	    break;
 	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retVal);
}

/*
 * itlBak_TapeSetCreate() --
 *    Create an ITL variable from a budb_tapeSet data type.
 *
 * RETURN CODES;
 *    A pointer to an ITL variable.
 */
EXPORT var_val_t *itlBak_TapeSetCreate(tapeSetP)
  IN budb_tapeSet *tapeSetP;
{
    budb_tapeSet *newTapeSetP;

    ITL_NEW(newTapeSetP, budb_tapeSet);
    *newTapeSetP = *tapeSetP;
    return(TapeSetNew(newTapeSetP, itlBak_tapeSetDataType));
}

/*
 *  itlBak_TapeSetGet() --
 *    Reads a budb_tapeSet data type from the input stream.
 */
EXPORT void itlBak_TapeSetGet(tapeSetP)
  OUT budb_tapeSet *tapeSetP;  /* ptr to caller allocated structure */
{
    var_val_t *varP;

    itl_AdviseUser("Tape Set -");
    varP = TapeSetRead((var_val_t *) 0, itlBak_tapeSetDataType);
    assert(varP);
    *tapeSetP = *((budb_tapeSet *) VALUE_VAL(varP));
    itl_FreeVarVal(varP);
}
