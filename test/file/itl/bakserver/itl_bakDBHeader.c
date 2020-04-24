/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakDBHeader.c --
 *    Code for the DbHeader data type.
 */
/*
 * HISTORY
 * $Log: itl_bakDBHeader.c,v $
 * Revision 1.1.6.1  1996/10/17  18:24:38  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:26  damon]
 *
 * $EndLog$
 */

#include <time.h>
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

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakDBHeader.c,v 1.1.6.1 1996/10/17 18:24:38 damon Exp $")

int itlBak_dbHeaderDataType;


PRIVATE var_val_t *DBHeaderCopy _TAKES((
				       IN var_val_t *copyVarP
				       ));

PRIVATE var_val_t *DBHeaderNew _TAKES((
				      IN pointer_t newValP,
				      IN int newType
				      ));

PRIVATE boolean_t DBHeaderFree _TAKES((
				      IN var_val_t *freeVarP
				      ));

PRIVATE var_val_t *DBHeaderRead _TAKES((
				       IN var_val_t *readHeaderP,
				       IN int varType
				       ));

PRIVATE boolean_t DBHeaderBinaryOp _TAKES((
					  IN var_val_t *leftOpP,
					  IN var_val_t *rightOpP,
					  IN char *binaryOp
					  ));

PRIVATE char *DBHeaderDisplay _TAKES((
				     IN var_val_t *displayVarP,
				     OUT int *lengthP
				     ));

PRIVATE var_val_t *DBHeaderGetField _TAKES((
					   IN var_val_t *getVarP,
					   IN char *fieldName
					   ));

PRIVATE boolean_t DBHeaderSetField _TAKES((
					  IN var_val_t *setValP,
					  IN char *fieldName,
					  IN var_val_t *fieldValP
					  ));


PRIVATE itlValue_varTypeInfo_t dbHeaderVarType = {
    "fs_bakDBHeader",
    DBHeaderCopy,
    DBHeaderNew,
    DBHeaderFree,
    DBHeaderRead,
    DBHeaderBinaryOp,
    NULL,
    NULL,
    NULL,
    DBHeaderDisplay,
    DBHeaderGetField,
    DBHeaderSetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *dbHeaderFieldNames[] = {
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
 *  DBHeaderInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void DBHeaderInit()
{
    /*
     *  We can safely set tapeSetHashType because this function is called
     *  via pthread_once.
     */
    itlBak_dbHeaderDataType = itlValue_RegisterVarType(&dbHeaderVarType);
}

/*
 * itlBak_DBHeaderInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlBak_DBHeaderInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) DBHeaderInit))
	itl_Error("pthread_once failed in DBHeaderInit\n");
}


PRIVATE var_val_t *DBHeaderCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    DbHeader *tapeSetValP = (DbHeader *) VALUE_VAL(copyVarP);
    DbHeader *newHeaderP;

    assert(VALUE_TYPE(copyVarP) == itlBak_dbHeaderDataType);
    ITL_NEW(newHeaderP, DbHeader);
    *newHeaderP = *tapeSetValP;
    return(DBHeaderNew((pointer_t) newHeaderP, itlBak_dbHeaderDataType));
}

PRIVATE var_val_t *DBHeaderNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newHeaderVarP;

    assert(newType == itlBak_dbHeaderDataType);
    ITL_NEW(newHeaderVarP, var_val_t);
    VALUE_TYPE(newHeaderVarP) = newType;
    VALUE_SIZE(newHeaderVarP) = sizeof(DbHeader);
    VALUE_VAL(newHeaderVarP) = newValP;
    return(newHeaderVarP);
}

PRIVATE boolean_t DBHeaderFree(freeVarP)
  IN var_val_t *freeVarP;
{
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}

PRIVATE var_val_t *DBHeaderRead(readHeaderP, varType)
  IN var_val_t *readHeaderP;
  IN int varType;
{
    var_val_t *newVarP;

    assert(varType == itlBak_dbHeaderDataType);
    if (!readHeaderP)
    {
	itl_PromptUser("dbVersion: ");
	readHeaderP = itl_GetVarVal();

	if (VALUE_TYPE(readHeaderP) == itlBak_dbHeaderDataType)
	    newVarP = readHeaderP;
	else
	{
	    DbHeader *newHeaderP;
	    int wordSize;
	    boolean_t eof;
	    char *strP;

	    /*
	     *  The memory allocated here for newHeaderP is free'd in the 
	     *  TapeSetFree() routine.
	     */
	    ITL_NEW(newHeaderP, DbHeader);
	    newVarP = itlValue_ConvertValue(readHeaderP, itl_intType);
	    assert(newVarP);
	    newHeaderP->dbversion = *((signed32 *) (VALUE_VAL(newVarP)));
	    itl_FreeVarVal(newVarP);

	    newHeaderP->created = (signed32) itlBak_CommonGetTime("Created",
								  FALSE);
	    itlBak_CommonGetLimitedStr("cell", BU_MAXCELLLEN,
				       (char *) &(newHeaderP->cell));
	    itl_PromptUser("LastDumpId: ");
	    newHeaderP->lastDumpId = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("LastInstanceId: ");
	    newHeaderP->lastInstanceId = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("LastTapeId: ");
	    newHeaderP->lastTapeId = (unsigned32) itl_GetInteger((boolean_t *) 0);

	    itl_PromptUser("Spare1: ");
	    newHeaderP->spare1 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare2: ");
	    newHeaderP->spare2 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare3: ");
	    newHeaderP->spare3 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare4: ");
	    newHeaderP->spare4 = (unsigned32) itl_GetInteger((boolean_t *) 0);

	    newVarP = DBHeaderNew((pointer_t) newHeaderP, 
				   itlBak_dbHeaderDataType);
	}
    }
    else
	newVarP = (var_val_t *) 0;

    return(newVarP);
}

/*
 *  DBHeaderBinaryOp() --
 *    Just compares two DBHeaders. 
 *
 *  WARNING:
 *    The return value (boolean) indicates if the operation requested is
 *    valid.  Oddly TRUE means it is not a valid operation and FALSE 
 *    implies it is a valid operation.  Its says nothing about the success 
 *    of the comparison.
 *    The value of leftOpP indicates if the operation succeeded.
 */
PRIVATE boolean_t DBHeaderBinaryOp(leftOpP, rightOpP, binaryOp)
  IN var_val_t *leftOpP;
  IN var_val_t *rightOpP;
  IN char *binaryOp;
{
    boolean_t validOp = FALSE, opSucceeded = FALSE, areEqual;
    var_val_t *opResultP;
    DbHeader *leftValP, *rightValP;

    leftValP = (DbHeader *) VALUE_VAL(leftOpP);
    rightValP = (DbHeader *) VALUE_VAL(rightOpP);

    /*
     *  Compare the two structures and save the result.
     */

    if ((leftValP->dbversion == rightValP->dbversion) &&
	(leftValP->created == rightValP->created) &&
	(!strcmp((char *) leftValP->cell, (char *) rightValP->cell)) &&
	(leftValP->lastDumpId == rightValP->lastDumpId) &&
	(leftValP->lastInstanceId == rightValP->lastInstanceId) &&
	(leftValP->lastTapeId == rightValP->lastTapeId) &&
	(leftValP->spare1 == rightValP->spare1) &&
	(leftValP->spare2 == rightValP->spare2) &&
	(leftValP->spare3 == rightValP->spare3) &&
	(leftValP->spare4 == rightValP->spare4))
	areEqual = TRUE;

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
	  itl_Error("Binary operation, %s, NOT supported on fs_dbHeader's.\n",
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

PRIVATE char *DBHeaderDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    DbHeader *headerValP;
    char *displayStr, *index;
    unsigned int strLength;

    headerValP = (DbHeader *) VALUE_VAL(displayVarP);

    /*
     *  Calculations for the length of the string assume worst case for
     *  the fields of the unsigned32 data type.  That is they are assumed to
     *  be 10 characters large.
     *  An additional byte is added for the end of string character, and
     *  the length of the labels used in the sprintf that follows are
     *  also included.
     */
    strLength = 10 + 11 +             /* dbversion field */
		10 + 9 +              /* created field */
	        BU_MAXCELLLEN + 6 +   /* cell field */
		10 + 12 +             /* lastDumpId field */
		10 + 16 +             /* lastInstanceId field */
		10 + 12 +             /* lastTapeId field */
		10 + 9 +              /* spare1 field */
		10 + 9 +              /* spare2 field */
		10 + 12 +             /* spare3 field */
		10 + 9 +              /* spare4 field */
		30 +                  /* newlines and spaces */
		10;                   /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "\n  Cell: %s\n  Created: %s",
	    headerValP->cell, ctime((time_t *) &(headerValP->created)));
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  dbVersion: %10d   LastDumpId: %10d\n",
	    headerValP->dbversion, headerValP->lastDumpId);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  LastInstanceId: %10d   LastTapeId: %10d\n",
	    headerValP->lastInstanceId, headerValP->lastTapeId);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare1: %10d   Spare2: %10d\n",
	    headerValP->spare1, headerValP->spare2);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare3: %10d   Spare4: %10d\n",
	    headerValP->spare3, headerValP->spare4);

    assert(strlen(displayStr) < strLength);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);
}

PRIVATE var_val_t *DBHeaderGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    DbHeader *headerValP;

    headerValP = (DbHeader *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, dbHeaderFieldNames,
			sizeof(dbHeaderFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* dbversion */
	    retValP = itlStds_IntVal((int) headerValP->dbversion);
	    break;

	  case 1:   /* created */
	    retValP = itlStds_IntVal((int) headerValP->created);
	    break;

	  case 2:   /* cell */
	    retValP = itlStds_StringVal((char *) headerValP->cell);
	    break;

	  case 3:   /* lastDumpId */
	    retValP = itlStds_IntVal((int) headerValP->lastDumpId);
	    break;

	  case 4:   /* lastInstanceId */
	    retValP = itlStds_IntVal((int) headerValP->lastInstanceId);
	    break;

	  case 5:   /* lastTapeId */
	    retValP = itlStds_IntVal((int) headerValP->lastTapeId);
	    break;

	  case 6:   /* spare1 */
	    retValP = itlStds_IntVal((int) headerValP->spare1);
	    break;

	  case 7:   /* spare2 */
	    retValP = itlStds_IntVal((int) headerValP->spare2);
	    break;

	  case 8:   /* spare3 */
	    retValP = itlStds_IntVal((int) headerValP->spare3);
	    break;

	  case 9:   /* spare4 */
	    retValP = itlStds_IntVal((int) headerValP->spare4);
	    break;
	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retValP);
}


PRIVATE boolean_t DBHeaderSetField(setValP, fieldName, fieldValP)
  IN var_val_t *setValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    int fieldNum;
    DbHeader *headerValP;
    boolean_t retVal = FALSE;

    if (itl_LookupTable(fieldName, dbHeaderFieldNames,
			sizeof(dbHeaderFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	headerValP = (DbHeader *) VALUE_VAL(setValP);

	switch (fieldNum) {
	  case 0:   /* dbversion */
	    ITLU_SET_INT_FIELD(fieldValP, headerValP->dbversion, signed32, 
			       retVal);
	    break;

	  case 1:   /* created */
	    ITLU_SET_INT_FIELD(fieldValP, headerValP->created, signed32, 
			       retVal);
	    break;
	
	  case 2:   /* cell */
	    ITLU_SET_STRING_FIELD(fieldValP, &(headerValP->cell), 
				  BU_MAXCELLLEN, retVal);
	    break;

	  case 3:   /* lastDumpId */
	    ITLU_SET_INT_FIELD(fieldValP, headerValP->lastDumpId, signed32,
			       retVal);
	    break;

	  case 4:   /* lastInstanceId */
	    ITLU_SET_INT_FIELD(fieldValP, headerValP->lastInstanceId, 
			       signed32, retVal);
	    break;

	  case 5:   /* lastTapeId */
	    ITLU_SET_INT_FIELD(fieldValP, headerValP->lastTapeId, signed32,
			       retVal);
	    break;

	  case 6:   /* spare1 */
	    ITLU_SET_INT_FIELD(fieldValP, headerValP->spare1, unsigned32, 
			       retVal);
	    break;

	  case 7:   /* spare2 */
	    ITLU_SET_INT_FIELD(fieldValP, headerValP->spare2, unsigned32, 
			       retVal);
	    break;

	  case 8:   /* spare3 */
	    ITLU_SET_INT_FIELD(fieldValP, headerValP->spare3, unsigned32, 
			       retVal);
	    break;

	  case 9:   /* spare4 */
	    ITLU_SET_INT_FIELD(fieldValP, headerValP->spare4, unsigned32, 
			       retVal);
	    break;
 	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retVal);
}

/*
 *  itlBak_DBHeaderGet() --
 *    Reads a DbHeader data type from the input stream.
 */
EXPORT void itlBak_DBHeaderGet(headerP)
  OUT DbHeader *headerP;  /* ptr to caller allocated structure */
{
    var_val_t *varP;

    itl_AdviseUser("dbHeader -");
    varP = DBHeaderRead((var_val_t *) 0, itlBak_dbHeaderDataType);
    assert(varP);
    *headerP = *((DbHeader *) VALUE_VAL(varP));
    itl_FreeVarVal(varP);
}
