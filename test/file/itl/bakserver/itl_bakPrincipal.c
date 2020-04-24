/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakPrincipal.c --
 *    Code for the budb_principal data type.
 */
/*
 * HISTORY
 * $Log: itl_bakPrincipal.c,v $
 * Revision 1.1.6.1  1996/10/17  18:24:52  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:32  damon]
 *
 * $EndLog$
 */

#include <string.h>
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

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakPrincipal.c,v 1.1.6.1 1996/10/17 18:24:52 damon Exp $")

#define SPARE_FIELD_LEN  64   /* lenfth of the 'spare' field */

int itlBak_principalDataType;


PRIVATE var_val_t *PrincipalCopy _TAKES((
					 IN var_val_t *copyVarP
					 ));

PRIVATE var_val_t *PrincipalNew _TAKES((
					IN pointer_t newValP,
					IN int newType
					));

PRIVATE boolean_t PrincipalFree _TAKES((
					IN var_val_t *freeVarP
					));

PRIVATE var_val_t *PrincipalRead _TAKES((
					 IN var_val_t *readPrincipalP,
					 IN int varType
					 ));

PRIVATE boolean_t PrincipalBinaryOp _TAKES((
					    IN var_val_t *leftOpP,
					    IN var_val_t *rightOpP,
					    IN char *binaryOp
					    ));

PRIVATE char *PrincipalDisplay _TAKES((
				       IN var_val_t *displayVarP,
				       OUT int *lengthP
				       ));

PRIVATE var_val_t *PrincipalGetField _TAKES((
					     IN var_val_t *getVarP,
					     IN char *fieldName
					     ));

PRIVATE boolean_t PrincipalSetField _TAKES((
					    IN var_val_t *setValP,
					    IN char *fieldName,
					    IN var_val_t *fieldValP
					    ));


PRIVATE itlValue_varTypeInfo_t principalVarType = {
    "fs_bakPrincipal",
    PrincipalCopy,
    PrincipalNew,
    PrincipalFree,
    PrincipalRead,
    PrincipalBinaryOp,
    NULL,
    NULL,
    NULL,
    PrincipalDisplay,
    PrincipalGetField,
    PrincipalSetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *principalFieldNames[] = {
    "name",
    "instance",
    "cell",
    "spare",
    "spare1",
    "spare2",
    "spare3",
    "spare4"
};

/*
 *  PrincipalInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void PrincipalInit()
{
    /*
     *  We can safely set principalHashType because this function is called
     *  via pthread_once.
     */
    itlBak_principalDataType = itlValue_RegisterVarType(&principalVarType);
}

/*
 * itlBak_PrincipalInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlBak_PrincipalInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) PrincipalInit))
	itl_Error("pthread_once failed in PrincipalInit\n");
}


PRIVATE var_val_t *PrincipalCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    budb_principal *principalValP = (budb_principal *) VALUE_VAL(copyVarP);
    budb_principal *newPrincipalP;

    assert(VALUE_TYPE(copyVarP) == itlBak_principalDataType);
    ITL_NEW(newPrincipalP, budb_principal);
    (void) memcpy((char *) newPrincipalP, (char *) principalValP, 
		  sizeof(budb_principal));
    return(PrincipalNew((pointer_t) newPrincipalP, itlBak_principalDataType));
}

PRIVATE var_val_t *PrincipalNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newPrincipalVarP;

    assert(newType == itlBak_principalDataType);
    ITL_NEW(newPrincipalVarP, var_val_t);
    VALUE_TYPE(newPrincipalVarP) = newType;
    VALUE_SIZE(newPrincipalVarP) = sizeof(budb_principal);
    VALUE_VAL(newPrincipalVarP) = newValP;
    return(newPrincipalVarP);
}

PRIVATE boolean_t PrincipalFree(freeVarP)
  IN var_val_t *freeVarP;
{
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}

PRIVATE var_val_t *PrincipalRead(readPrincipalP, varType)
  IN var_val_t *readPrincipalP;
  IN int varType;
{
    var_val_t *newVarP;

    assert(varType == itlBak_principalDataType);
    if (!readPrincipalP)
    {
	itl_PromptUser("Name: ");
	readPrincipalP = itl_GetVarVal();

	if (VALUE_TYPE(readPrincipalP) == itlBak_principalDataType)
	    newVarP = readPrincipalP;
	else
	{
	    budb_principal *newPrincipalP;
	    int wordSize;
	    boolean_t eof;
	    char *strP;

	    /*
	     *  The memory allocated here for newPrincipalP is free'd in the 
	     *  PrincipalFree() routine.
	     */
	    ITL_NEW(newPrincipalP, budb_principal);
	    newVarP = itlValue_ConvertValue(readPrincipalP, itl_stringType);
	    assert(newVarP);
	    /*
	     *  Sizes of string variables include the end-of-string character.
	     *  Thus the <= instead of just <.
	     */
	    if (VALUE_SIZE(newVarP) <= BU_MAXUNAMELEN)
	    {
		strcpy((char *) &(newPrincipalP->name), 
		       (char *) VALUE_VAL(newVarP));
	    }
	    else
	    {
		itl_Error("Name length exceeds maximum of %d\n",
			  BU_MAXUNAMELEN);
	    }
	    itl_FreeVarVal(newVarP);

	    itlBak_CommonGetLimitedStr("Instance", BU_MAXINAMELEN,
				       (char *) &(newPrincipalP->instance));
	    itlBak_CommonGetLimitedStr("Cell", BU_MAXCELLLEN,
				       (char *) &(newPrincipalP->cell));
	    itlBak_CommonGetLimitedStr("Spare", SPARE_FIELD_LEN,
				       (char *) &(newPrincipalP->spare));

	    itl_PromptUser("Spare1: ");
	    newPrincipalP->spare1 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare2: ");
	    newPrincipalP->spare2 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare3: ");
	    newPrincipalP->spare3 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare4: ");
	    newPrincipalP->spare4 = (unsigned32) itl_GetInteger((boolean_t *) 0);

	    newVarP = PrincipalNew((pointer_t) newPrincipalP, 
				   itlBak_principalDataType);
	}
    }
    else
    {
	newVarP = (var_val_t *) 0;
	itl_FreeVarVal(readPrincipalP);
    }

    return(newVarP);
}

/*
 *  itlBak_PrincipalCompare() --
 *    Compares to budb_principal structures and returns TRUE if they
 *    are equal and FALSE if not.  This function is needed so higher
 *    level data types (containing a budb_principal) can determine if their
 *    structures are equal.
 *
 * RETURN CODES:
 *    TRUE  - equal
 *    FALSE - not equal
 */
EXPORT boolean_t itlBak_PrincipalCompare(principal1, principal2)
  IN budb_principal *principal1;
  IN budb_principal *principal2;
{
    boolean_t retVal = FALSE;

    if ((!strcmp((char *) principal1->name, (char *) principal2->name)) &&
	(!strcmp((char *) principal1->instance, 
		 (char *) principal2->instance)) &&
	(!strcmp((char *) principal1->cell, (char *) principal2->cell)) &&
	(!strcmp((char *) principal1->spare, (char *) principal2->spare)) &&
	(principal1->spare1 == principal2->spare1) &&
	(principal1->spare2 == principal2->spare2) &&
	(principal1->spare3 == principal2->spare3) &&
	(principal1->spare4 == principal2->spare4))
	retVal = TRUE;

    return(retVal);
}

/*
 *  PrincipalBinaryOp() --
 *    Just compares two principals.
 *
 *  WARNING:
 *    The return value (boolean) indicates if the operation requested is
 *    valid.  Oddly TRUE means it is not a valid operation and FALSE 
 *    implies it is a valid operation.  Its says nothing about the success 
 *    of the comparison.
 *    The value of leftOpP indicates if the operation succeeded.
 */
PRIVATE boolean_t PrincipalBinaryOp(leftOpP, rightOpP, binaryOp)
  IN var_val_t *leftOpP;
  IN var_val_t *rightOpP;
  IN char *binaryOp;
{
    boolean_t validOp = FALSE, opSucceeded = FALSE, areEqual;
    budb_principal *leftValP, *rightValP;
    var_val_t *opResultP;

    leftValP = (budb_principal *) VALUE_VAL(leftOpP);
    rightValP = (budb_principal *) VALUE_VAL(rightOpP);

    /*
     *  Compare the two structures and save the result.
     */
    areEqual = itlBak_PrincipalCompare(leftValP, rightValP);
    
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
	  itl_Error("Operation, %s, NOT supported on fs_bakPrincipal's.\n",
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
 *  itlBak_PrincipalDisplay() --
 *    Allows data types that contain principals to get a display string
 *    without having to create an ITL variable and call itlValue_Display().
 */
EXPORT char *itlBak_PrincipalDisplay(principalP, lengthP)
  IN budb_principal *principalP;
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
    strLength = BU_MAXUNAMELEN + 7 +   /* name field */
	        BU_MAXINAMELEN + 11 +  /* instance field */
	        BU_MAXCELLLEN + 7 +    /* cell field */
		SPARE_FIELD_LEN + 8 +  /* spare field */
		10 + 12 +              /* spare1 field */
		10 + 9 +               /* spare2 field */
		10 + 12 +              /* spare3 field */
		10 + 9 +               /* spare4 field */
		20 +                   /* newlines and spaces */
		10;                    /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "\n  Name: %s\n  Instance: %s\n", 
	    principalP->name, principalP->instance);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Cell: %s\n  Spare: %s\n",
	    principalP->cell, principalP->spare);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare1: %10d   Spare2: %10d\n",
	    principalP->spare1, principalP->spare2);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare3: %10d   Spare4: %10d\n",
	    principalP->spare3, principalP->spare4);

    assert(strlen(displayStr) < strLength);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);
}

PRIVATE char *PrincipalDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    budb_principal *principalValP;

    principalValP = (budb_principal *) VALUE_VAL(displayVarP);

    return(itlBak_PrincipalDisplay(principalValP, lengthP));
}

PRIVATE var_val_t *PrincipalGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    budb_principal *principalValP;

    principalValP = (budb_principal *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, principalFieldNames,
			sizeof(principalFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* name */
	    retValP = itlStds_StringVal((char *) principalValP->name);
	    break;

	  case 1:   /* instance */
	    retValP = itlStds_StringVal((char *) principalValP->instance);
	    break;

	  case 2:   /* cell */
	    retValP = itlStds_StringVal((char *) principalValP->cell);
	    break;

	  case 3:   /* spare */
	    retValP = itlStds_StringVal((char *) principalValP->spare);
	    break;

	  case 4:   /* spare1 */
	    retValP = itlStds_IntVal((int) principalValP->spare1);
	    break;

	  case 5:   /* spare2 */
	    retValP = itlStds_IntVal((int) principalValP->spare2);
	    break;

	  case 6:   /* spare3 */
	    retValP = itlStds_IntVal((int) principalValP->spare3);
	    break;

	  case 7:   /* spare4 */
	    retValP = itlStds_IntVal((int) principalValP->spare4);
	    break;
	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retValP);
}


PRIVATE boolean_t PrincipalSetField(setValP, fieldName, fieldValP)
  IN var_val_t *setValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    int fieldNum;
    budb_principal *principalValP;
    boolean_t retVal = FALSE;

    if (itl_LookupTable(fieldName, principalFieldNames,
			sizeof(principalFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	principalValP = (budb_principal *) VALUE_VAL(setValP);

	switch (fieldNum) {
	  case 0:   /* name */
	    ITLU_SET_STRING_FIELD(fieldValP, &(principalValP->name),
				  BU_MAXUNAMELEN, retVal);
	    break;

	  case 1:   /* instance */
	    ITLU_SET_STRING_FIELD(fieldValP, &(principalValP->instance),
				  BU_MAXINAMELEN, retVal);
	    break;
	
	  case 2:   /* cell */
	    ITLU_SET_STRING_FIELD(fieldValP, &(principalValP->cell),
				  BU_MAXCELLLEN, retVal);
	    break;

	  case 3:   /* spare */
	    ITLU_SET_STRING_FIELD(fieldValP, &(principalValP->spare),
				  SPARE_FIELD_LEN, retVal);
	    break;

	  case 4:   /* spare1 */
	    ITLU_SET_INT_FIELD(fieldValP, principalValP->spare1, unsigned32,
			       retVal);
	    break;

	  case 5:   /* spare2 */
	    ITLU_SET_INT_FIELD(fieldValP, principalValP->spare2, unsigned32,
			       retVal);
	    break;

	  case 6:   /* spare3 */
	    ITLU_SET_INT_FIELD(fieldValP, principalValP->spare3, unsigned32,
			       retVal);
	    break;

	  case 7:   /* spare4 */
	    ITLU_SET_INT_FIELD(fieldValP, principalValP->spare4, unsigned32,
			       retVal);
	    break;
 	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retVal);
}

/*
 * itlBak_PrincipalCreate() --
 *    Create an ITL variable from a budb_principal data type.
 *
 * RETURN CODES;
 *    A pointer to an ITL variable.
 */
EXPORT var_val_t *itlBak_PrincipalCreate(principalP)
  IN budb_principal *principalP;
{
    budb_principal *newPrincipalP;

    ITL_NEW(newPrincipalP, budb_principal);
    *newPrincipalP = *principalP;
    return(PrincipalNew(newPrincipalP, itlBak_principalDataType));
}

/*
 *  itlBak_PrincipalGet() --
 *    Reads a budb_principal data type from the input stream.
 */
EXPORT void itlBak_PrincipalGet(principalP)
  OUT budb_principal *principalP;  /* ptr to caller allocated structure */
{
    var_val_t *varP;

    itl_AdviseUser("Principal -");
    varP = PrincipalRead((var_val_t *) 0, itlBak_principalDataType);
    assert(varP);
    *principalP = *((budb_principal *) VALUE_VAL(varP));
    itl_FreeVarVal(varP);
}
