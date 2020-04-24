/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakTapeEntry.c --
 *    Code for the budb_TapeEntry data type.
 */
/*
 * HISTORY
 * $Log: itl_bakTapeEntry.c,v $
 * Revision 1.1.6.1  1996/10/17  18:24:55  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:33  damon]
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

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakTapeEntry.c,v 1.1.6.1 1996/10/17 18:24:55 damon Exp $")

int itlBak_tapeEntryDataType;


PRIVATE var_val_t *TapeEntryCopy _TAKES((
					 IN var_val_t *copyVarP
					 ));

PRIVATE var_val_t *TapeEntryNew _TAKES((
					IN pointer_t newValP,
					IN int newType
					));

PRIVATE boolean_t TapeEntryFree _TAKES((
					IN var_val_t *freeVarP
					));

PRIVATE var_val_t *TapeEntryRead _TAKES((
					 IN var_val_t *readTapeEntryP,
					 IN int varType
					 ));

PRIVATE boolean_t TapeEntryBinaryOp _TAKES((
					    IN var_val_t *leftOpP,
					    IN var_val_t *rightOpP,
					    IN char *binaryOp
					    ));

PRIVATE char *TapeEntryDisplay _TAKES((
				       IN var_val_t *displayVarP,
				       OUT int *lengthP
				       ));

PRIVATE var_val_t *TapeEntryGetField _TAKES((
					     IN var_val_t *getVarP,
					     IN char *fieldName
					     ));

PRIVATE boolean_t TapeEntrySetField _TAKES((
					    IN var_val_t *setValP,
					    IN char *fieldName,
					    IN var_val_t *fieldValP
					    ));


PRIVATE itlValue_varTypeInfo_t tapeEntryVarType = {
    "fs_bakTapeEntry",
    TapeEntryCopy,
    TapeEntryNew,
    TapeEntryFree,
    TapeEntryRead,
    TapeEntryBinaryOp,
    NULL,
    NULL,
    NULL,
    TapeEntryDisplay,
    TapeEntryGetField,
    TapeEntrySetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *TapeEntryFieldNames[] = {
    "name",
    "flags",
    "written",
    "expires",
    "nMBytes",
    "nBytes",
    "nFiles",
    "nVolumes",
    "seq",
    "tapeid",
    "useCount",
    "mediaType",
    "dump",
    "spare1",
    "spare2",
    "spare3",
    "spare4"
};

/*
 *  TapeEntryInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void TapeEntryInit()
{
    /*
     *  We can safely set TapeEntryHashType because this function is called
     *  via pthread_once.
     */
    itlBak_tapeEntryDataType = itlValue_RegisterVarType(&tapeEntryVarType);
}

/*
 * itlBak_TapeEntryInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlBak_TapeEntryInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) TapeEntryInit))
	itl_Error("pthread_once failed in TapeEntryInit\n");
}


PRIVATE var_val_t *TapeEntryCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    budb_tapeEntry *tapeEntryValP = (budb_tapeEntry *) VALUE_VAL(copyVarP);
    budb_tapeEntry *newTapeEntryP;

    assert(VALUE_TYPE(copyVarP) == itlBak_tapeEntryDataType);
    ITL_NEW(newTapeEntryP, budb_tapeEntry);
    *newTapeEntryP = *tapeEntryValP;
    return(TapeEntryNew((pointer_t) newTapeEntryP, itlBak_tapeEntryDataType));
}

PRIVATE var_val_t *TapeEntryNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newTapeEntryVarP;

    assert(newType == itlBak_tapeEntryDataType);
    ITL_NEW(newTapeEntryVarP, var_val_t);
    VALUE_TYPE(newTapeEntryVarP) = newType;
    VALUE_SIZE(newTapeEntryVarP) = sizeof(budb_tapeEntry);
    VALUE_VAL(newTapeEntryVarP) = newValP;
    return(newTapeEntryVarP);
}

PRIVATE boolean_t TapeEntryFree(freeVarP)
  IN var_val_t *freeVarP;
{
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}

PRIVATE var_val_t *TapeEntryRead(readTapeEntryP, varType)
  IN var_val_t *readTapeEntryP;
  IN int varType;
{
    var_val_t *newVarP;

    assert(varType == itlBak_tapeEntryDataType);
    if (!readTapeEntryP)
    {
	itl_PromptUser("Name: ");
	readTapeEntryP = itl_GetVarVal();

	if (VALUE_TYPE(readTapeEntryP) == itlBak_tapeEntryDataType)
	    newVarP = readTapeEntryP;
	else
	{
	    budb_tapeEntry *newTapeEntryP;

	    /*
	     *  The memory allocated here for newTapeEntryP is free'd in the 
	     *  TapeEntryFree() routine.
	     */
	    ITL_NEW(newTapeEntryP, budb_tapeEntry);
	    newVarP = itlValue_ConvertValue(readTapeEntryP, itl_stringType);
	    assert(newVarP);
	    if ((int) strlen(VALUE_VAL(newVarP)) < BU_MAXTAPELEN)
	    {
		strcpy((char *) newTapeEntryP->name, 
		       (char *) (VALUE_VAL(newVarP)));
		itl_FreeVarVal(newVarP);

		itl_PromptUser("Flags: ");
		newTapeEntryP->flags = (unsigned32) itl_GetInteger((boolean_t *) 0);
		newTapeEntryP->written = (Date) itlBak_CommonGetTime("Written",
								    FALSE);
		newTapeEntryP->expires = (Date) itlBak_CommonGetTime("Expires",
								    FALSE);
		itl_PromptUser("nMBytes: ");
		newTapeEntryP->nMBytes = (unsigned32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("nBytes: ");
		newTapeEntryP->nBytes = (unsigned32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("nFiles: ");
		newTapeEntryP->nFiles = (signed32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("nVolumes: ");
		newTapeEntryP->nVolumes = (signed32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("seq: ");
		newTapeEntryP->seq = (signed32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("tapeid: ");
		newTapeEntryP->tapeid = (signed32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("useCount: ");
		newTapeEntryP->useCount = (signed32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("mediaType: ");
		newTapeEntryP->mediaType = (signed32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("Dump: ");
		newTapeEntryP->dump = (dumpId) itl_GetInteger((boolean_t *) 0);

		itl_PromptUser("Spare1: ");
		newTapeEntryP->spare1 = (unsigned32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("Spare2: ");
		newTapeEntryP->spare2 = (unsigned32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("Spare3: ");
		newTapeEntryP->spare3 = (unsigned32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("Spare4: ");
		newTapeEntryP->spare4 = (unsigned32) itl_GetInteger((boolean_t *) 0);

		newVarP = TapeEntryNew((pointer_t) newTapeEntryP, 
				       itlBak_tapeEntryDataType);
	    }
	    else
	    {
		itl_Error("Name field length exceeds maximum of %d\n", 
			  BU_MAXTAPELEN);
		newVarP = (var_val_t *) 0;
	    }
	}
    }
    else
	newVarP = (var_val_t *) 0;

    return(newVarP);
}

EXPORT boolean_t itlBak_TapeEntryCompare(leftP, rightP)
  IN budb_tapeEntry *leftP;
  IN budb_tapeEntry *rightP;
{
    boolean_t retVal = FALSE;

    /*
     *  Compare the two structures and save the result.
     */
    if ((!strcmp((char *) leftP->name, (char *) rightP->name)) &&
	(leftP->flags == rightP->flags) &&
	(leftP->written == rightP->written) &&
	(leftP->expires == rightP->expires) &&
	(leftP->nMBytes == rightP->nMBytes) &&
	(leftP->nBytes == rightP->nBytes) &&
	(leftP->nFiles == rightP->nFiles) &&
	(leftP->nVolumes == rightP->nVolumes) &&
	(leftP->seq == rightP->seq) &&
	(leftP->tapeid == rightP->tapeid) &&
	(leftP->useCount == rightP->useCount) &&
	(leftP->mediaType == rightP->mediaType) &&
	(leftP->dump == rightP->dump) &&
	(leftP->spare1 == rightP->spare1) &&
	(leftP->spare2 == rightP->spare2) &&
	(leftP->spare3 == rightP->spare3) &&
	(leftP->spare4 == rightP->spare4))
	retVal = TRUE;

    return(retVal);
}

/*
 *  TapeEntryBinaryOp() --
 *    Just compares two tapes entry types.. 
 *
 *  WARNING:
 *    The return value (boolean) indicates if the operation requested is
 *    valid.  Oddly TRUE means it is not a valid operation and FALSE 
 *    implies it is a valid operation.  Its says nothing about the success 
 *    of the comparison.
 *    The value of leftOpP indicates if the operation succeeded.
 */
PRIVATE boolean_t TapeEntryBinaryOp(leftOpP, rightOpP, binaryOp)
  IN var_val_t *leftOpP;
  IN var_val_t *rightOpP;
  IN char *binaryOp;
{
    boolean_t validOp = FALSE, opSucceeded = FALSE, areEqual;
    budb_tapeEntry *leftValP, *rightValP;
    var_val_t *opResultP;

    leftValP = (budb_tapeEntry *) VALUE_VAL(leftOpP);
    rightValP = (budb_tapeEntry *) VALUE_VAL(rightOpP);

    areEqual = itlBak_TapeEntryCompare(leftValP, rightValP);
    
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
	  itl_Error("Operation, %s, NOT supported on fs_bakTapeEntry's.\n",
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

PRIVATE char *TapeEntryDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    char *displayStr, *index;
    budb_tapeEntry *tapeEntryValP;
    int strLength;

    tapeEntryValP = (budb_tapeEntry *) VALUE_VAL(displayVarP);

    /*
     *  Calculations for the length of the string assume worst case for
     *  the fields of the unsigned32 data type.  That is they are assumed to
     *  be 10 characters large.
     *  An additional byte is added for the end of string character, and
     *  the length of the labels used in the sprintf that follows are
     *  also included.
     */
    strLength = BU_MAXTAPELEN + 6 +          /* name field */
	        10 + 10 +                    /* flags field */
		CHARS_IN_DATE_STRING + 9 +   /* written field */
		CHARS_IN_DATE_STRING + 9 +   /* expires field */
		10 + 12 +                    /* nMBytes field */
		10 + 12 +                    /* nBytes field */
		10 + 12 +                    /* nFiles field */
		10 + 13 +                    /* nVolumes field */
		10 + 8 +                     /* seq field */
		10 + 11 +                    /* tapeid field */
		10 + 12 +                    /* useCount field */
		10 + 13 +                    /* mediaType field */
		10 + 9 +                     /* dump field */
		10 + 9 +                     /* spare1 field */
		10 + 9 +                     /* spare2 field */
		10 + 9 +                     /* spare3 field */
		10 + 9 +                     /* spare4 field */
		55 +                         /* newlines and spaces */
		10;                          /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "\n  Name: %s\n  Flags: %10d   Dump: %10d\n",
	    tapeEntryValP->name, tapeEntryValP->flags, tapeEntryValP->dump);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Written: %s   Expires: %s",
	    ctime((time_t *) &(tapeEntryValP->written)),
	    ctime((time_t *) &(tapeEntryValP->expires)));
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  nMBytes: %10d   nBytes: %10d\n",
	    tapeEntryValP->nMBytes, tapeEntryValP->nBytes);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  nFiles: %10d   nVolumes: %10d\n",
	    tapeEntryValP->nFiles, tapeEntryValP->nVolumes);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Seq: %10d   TapeId: %10d\n",
	    tapeEntryValP->seq, tapeEntryValP->tapeid);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  UseCount: %10d   MediaType: %10d\n",
	    tapeEntryValP->useCount, tapeEntryValP->mediaType);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare1: %10d   Spare2: %10d\n",
	    tapeEntryValP->spare1, tapeEntryValP->spare2);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare3: %10d   Spare4: %10d\n",
	    tapeEntryValP->spare3, tapeEntryValP->spare4);

    assert((int) strlen(displayStr) < strLength);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);
}

PRIVATE var_val_t *TapeEntryGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    budb_tapeEntry *tapeEntryValP;

    tapeEntryValP = (budb_tapeEntry *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, TapeEntryFieldNames,
			sizeof(TapeEntryFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* name */
	    retValP = itlStds_StringVal((char *) tapeEntryValP->name);
	    break;

	  case 1:   /* flags */
	    retValP = itlStds_IntVal((int) tapeEntryValP->flags);
	    break;

	  case 2:   /* written */
	    retValP = itlStds_IntVal((int) tapeEntryValP->written);
	    break;

	  case 3:   /* expires */
	    retValP = itlStds_IntVal((int) tapeEntryValP->expires);
	    break;

	  case 4:   /* nMBytes */
	    retValP = itlStds_IntVal((int) tapeEntryValP->nMBytes);
	    break;

	  case 5:   /* nBytes */
	    retValP = itlStds_IntVal((int) tapeEntryValP->nBytes);
	    break;

	  case 6:   /* nFiles */
	    retValP = itlStds_IntVal((int) tapeEntryValP->nFiles);
	    break;

	  case 7:   /* nVolumes */
	    retValP = itlStds_IntVal((int) tapeEntryValP->nVolumes);
	    break;

	  case 8:   /* seq */
	    retValP = itlStds_IntVal((int) tapeEntryValP->seq);
	    break;

	  case 9:   /* tapeid */
	    retValP = itlStds_IntVal((int) tapeEntryValP->tapeid);
	    break;

	  case 10:  /* useCount */
	    retValP = itlStds_IntVal((int) tapeEntryValP->useCount);
	    break;

	  case 11:  /* mediaType */
	    retValP = itlStds_IntVal((int) tapeEntryValP->mediaType);
	    break;

	  case 12:  /* dump */
	    retValP = itlStds_IntVal((int) tapeEntryValP->dump);
	    break;

	  case 13:  /* spare1 */
	    retValP = itlStds_IntVal((int) tapeEntryValP->spare1);
	    break;

	  case 14:  /* spare2 */
	    retValP = itlStds_IntVal((int) tapeEntryValP->spare2);
	    break;

	  case 15:  /* spare3 */
	    retValP = itlStds_IntVal((int) tapeEntryValP->spare3);
	    break;

	  case 16:  /* spare4 */
	    retValP = itlStds_IntVal((int) tapeEntryValP->spare4);
	    break;
	}
    }
    else
	itl_Error("Field table lookup FAILED.\n");

    return(retValP);
}


PRIVATE boolean_t TapeEntrySetField(setValP, fieldName, fieldValP)
  IN var_val_t *setValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    int fieldNum;
    budb_tapeEntry *tapeEntryValP;
    boolean_t retVal = FALSE;

    if (itl_LookupTable(fieldName, TapeEntryFieldNames,
			sizeof(TapeEntryFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	tapeEntryValP = (budb_tapeEntry *) VALUE_VAL(setValP);

	switch (fieldNum) {
	  case 0:   /* name */
	    ITLU_SET_STRING_FIELD(fieldValP, &(tapeEntryValP->name), 
				  BU_MAXTAPELEN, retVal);
	    break;

	  case 1:   /* flags */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->flags, unsigned32,
			       retVal);
	    break;

	  case 2:   /* written */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->written, Date, 
			       retVal);
	    break;
	
	  case 3:   /* expires */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->expires, Date,
			       retVal);
	    break;

	  case 4:   /* nMBytes */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->nMBytes, unsigned32,
			       retVal);
	    break;

	  case 5:   /* nBytes */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->nBytes, unsigned32,
			       retVal);
	    break;

	  case 6:   /* nFiles */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->nFiles, signed32,
			       retVal);
	    break;

	  case 7:   /* nVolumes */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->nVolumes, signed32,
			       retVal);
	    break;

	  case 8:   /* seq */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->seq, signed32,
			       retVal);
	    break;

	  case 9:   /* tapeid */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->tapeid, signed32,
			       retVal);
	    break;

	  case 10:  /* useCount */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->useCount, signed32,
			       retVal);
	    break;

	  case 11:  /* mediaType */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->mediaType, signed32,
			       retVal);
	    break;

	  case 12:  /* dump */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->dump, dumpId,
			       retVal);
	    break;

	  case 13:  /* spare1 */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->spare1, unsigned32,
			       retVal);
	    break;

	  case 14:  /* spare2 */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->spare2, unsigned32,
			       retVal);
	    break;

	  case 15:  /* spare3 */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->spare3, unsigned32,
			       retVal);
	    break;

	  case 16:  /* spare4 */
	    ITLU_SET_INT_FIELD(fieldValP, tapeEntryValP->spare4, unsigned32,
			       retVal);
 	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retVal);
}

/*
 *  itlBak_TapeEntryGet() --
 *    Reads a budb_tapeEntry data type from the input stream.
 */
EXPORT void itlBak_TapeEntryGet(TapeEntryP)
  OUT budb_tapeEntry *TapeEntryP;  /* ptr to caller allocated structure */
{
    var_val_t *varP;

    itl_AdviseUser("Tape Entry -");
    varP = TapeEntryRead((var_val_t *) 0, itlBak_tapeEntryDataType);
    assert(varP);
    *TapeEntryP = *((budb_tapeEntry *) VALUE_VAL(varP));
    itl_FreeVarVal(varP);
}
