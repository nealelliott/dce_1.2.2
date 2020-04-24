/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakDumpEntry.c --
 *    Code for the budb_dumpEntry data type.
 */
/*
 * HISTORY
 * $Log: itl_bakDumpEntry.c,v $
 * Revision 1.1.6.1  1996/10/17  18:24:43  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:29  damon]
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
#include <utils/itl/itl_bakTapeSet.h>
#include <utils/itl/itl_bakPrincipal.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakDumpEntry.c,v 1.1.6.1 1996/10/17 18:24:43 damon Exp $")

int itlBak_dumpEntryDataType;


PRIVATE var_val_t *DumpEntryCopy _TAKES((
					 IN var_val_t *copyVarP
					 ));

PRIVATE var_val_t *DumpEntryNew _TAKES((
					IN pointer_t newValP,
					IN int newType
					));

PRIVATE boolean_t DumpEntryFree _TAKES((
					IN var_val_t *freeVarP
					));

PRIVATE var_val_t *DumpEntryRead _TAKES((
					 IN var_val_t *readDumpEntryP,
					 IN int varType
					 ));

PRIVATE boolean_t DumpEntryBinaryOp _TAKES((
					    IN var_val_t *leftOpP,
					    IN var_val_t *rightOpP,
					    IN char *binaryOp
					    ));

PRIVATE char *DumpEntryDisplay _TAKES((
				       IN var_val_t *displayVarP,
				       OUT int *lengthP
				       ));

PRIVATE var_val_t *DumpEntryGetField _TAKES((
					     IN var_val_t *getVarP,
					     IN char *fieldName
					     ));

PRIVATE boolean_t DumpEntrySetField _TAKES((
					    IN var_val_t *setValP,
					    IN char *fieldName,
					    IN var_val_t *fieldValP
					    ));


PRIVATE itlValue_varTypeInfo_t dumpEntryVarType = {
    "fs_bakDumpEntry",
    DumpEntryCopy,
    DumpEntryNew,
    DumpEntryFree,
    DumpEntryRead,
    DumpEntryBinaryOp,
    NULL,
    NULL,
    NULL,
    DumpEntryDisplay,
    DumpEntryGetField,
    DumpEntrySetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *dumpEntryFieldNames[] = {
    "id",
    "parent",
    "level",
    "flags",
    "volumeSetName",
    "dumpPath",
    "name",
    "created",
    "incTime",
    "nVolumes",
    "tapes",
    "dumper",
    "spare1",
    "spare2",
    "spare3",
    "spare4"
};

/*
 *  DumpEntryInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void DumpEntryInit()
{
    /*
     *  We can safely set DumpEntryHashType because this function is called
     *  via pthread_once.
     */
    itlBak_dumpEntryDataType = itlValue_RegisterVarType(&dumpEntryVarType);
}

/*
 * itlBak_DumpEntryInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlBak_DumpEntryInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) DumpEntryInit))
	itl_Error("pthread_once failed in DumpEntryInit\n");
}


PRIVATE var_val_t *DumpEntryCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    budb_dumpEntry *DumpEntryValP = (budb_dumpEntry *) VALUE_VAL(copyVarP);
    budb_dumpEntry *newDumpEntryP;

    assert(VALUE_TYPE(copyVarP) == itlBak_dumpEntryDataType);
    ITL_NEW(newDumpEntryP, budb_dumpEntry);
    *newDumpEntryP = *DumpEntryValP;
    return(DumpEntryNew((pointer_t) newDumpEntryP, itlBak_dumpEntryDataType));
}

PRIVATE var_val_t *DumpEntryNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newDumpEntryVarP;

    assert(newType == itlBak_dumpEntryDataType);
    ITL_NEW(newDumpEntryVarP, var_val_t);
    VALUE_TYPE(newDumpEntryVarP) = newType;
    VALUE_SIZE(newDumpEntryVarP) = sizeof(budb_dumpEntry);
    VALUE_VAL(newDumpEntryVarP) = newValP;
    return(newDumpEntryVarP);
}

PRIVATE boolean_t DumpEntryFree(freeVarP)
  IN var_val_t *freeVarP;
{
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}

PRIVATE var_val_t *DumpEntryRead(readDumpEntryP, varType)
  IN var_val_t *readDumpEntryP;
  IN int varType;
{
    var_val_t *newVarP;

    assert(varType == itlBak_dumpEntryDataType);
    if (!readDumpEntryP)
    {
	itl_PromptUser("Id: ");
	readDumpEntryP = itl_GetVarVal();

	if (VALUE_TYPE(readDumpEntryP) == itlBak_dumpEntryDataType)
	    newVarP = readDumpEntryP;
	else
	{
	    budb_dumpEntry *newDumpEntryP;

	    /*
	     *  The memory allocated here for newDumpEntryP is free'd in the 
	     *  DumpEntryFree() routine.
	     */
	    ITL_NEW(newDumpEntryP, budb_dumpEntry);
	    newVarP = itlValue_ConvertValue(readDumpEntryP, itl_intType);
	    assert(newVarP);
	    newDumpEntryP->id = *((dumpId *) (VALUE_VAL(newVarP)));
	    itl_FreeVarVal(newVarP);

	    itl_PromptUser("Parent: ");
	    newDumpEntryP->parent = (dumpId) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Level: ");
	    newDumpEntryP->level = (signed32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Flags: ");
	    newDumpEntryP->flags = (signed32) itl_GetInteger((boolean_t *) 0);

	    itlBak_CommonGetLimitedStr("VolumeSetName", BU_MAXNAMELEN,
				       (char *) &(newDumpEntryP->volumeSetName));
	    itlBak_CommonGetLimitedStr("DumpPath", BU_MAXPATHLEN,
				       (char *) &(newDumpEntryP->dumpPath));
	    itlBak_CommonGetLimitedStr("Name", BU_MAXNAMELEN,
				       (char *) &(newDumpEntryP->name));

	    newDumpEntryP->created = (Date) itlBak_CommonGetTime("Created",
								FALSE);
	    newDumpEntryP->incTime = (Date) itlBak_CommonGetTime("IncTime",
								TRUE);

	    itl_PromptUser("nVolumes: ");
	    newDumpEntryP->nVolumes = (signed32) itl_GetInteger((boolean_t *) 0);
	    itlBak_TapeSetGet(&(newDumpEntryP->tapes));
	    itlBak_PrincipalGet(&(newDumpEntryP->dumper));

	    itl_PromptUser("Spare1: ");
	    newDumpEntryP->spare1 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare2: ");
	    newDumpEntryP->spare2 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare3: ");
	    newDumpEntryP->spare3 = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Spare4: ");
	    newDumpEntryP->spare4 = (unsigned32) itl_GetInteger((boolean_t *) 0);

	    newVarP = DumpEntryNew((pointer_t) newDumpEntryP, 
				   itlBak_dumpEntryDataType);
	}
    }
    else
	newVarP = (var_val_t *) 0;

    return(newVarP);
}

EXPORT boolean_t itlBak_DumpEntryCompare(leftP, rightP)
  IN budb_dumpEntry *leftP;
  IN budb_dumpEntry *rightP;
{
    boolean_t retVal = FALSE;

    /*
     *  Compare the two structures and save the result.
     */
    if ((leftP->id == rightP->id) &&
	(leftP->parent == rightP->parent) &&
	(leftP->level == rightP->level) &&
	(leftP->flags == rightP->flags) &&
	(!strcmp((char *) leftP->volumeSetName, 
		 (char *) rightP->volumeSetName)) &&
	(!strcmp((char *) leftP->dumpPath, (char *) rightP->dumpPath)) &&
	(!strcmp((char *) leftP->name, (char *) rightP->name)) &&
	(leftP->created == rightP->created) &&
	(leftP->incTime == rightP->incTime) &&
	(leftP->nVolumes == rightP->nVolumes) &&
	itlBak_TapeSetCompare(&(leftP->tapes), &(rightP->tapes)) &&
	itlBak_PrincipalCompare(&(leftP->dumper), &(rightP->dumper)) &&
	(leftP->spare1 == rightP->spare1) &&
	(leftP->spare2 == rightP->spare2) &&
	(leftP->spare3 == rightP->spare3) &&
	(leftP->spare4 == rightP->spare4))
	retVal = TRUE;

    return(retVal);
}

/*
 *  DumpEntryBinaryOp() --
 *    Just compares two DumpEntrys. 
 *
 *  WARNING:
 *    The return value (boolean) indicates if the operation requested is
 *    valid.  Oddly TRUE means it is not a valid operation and FALSE 
 *    implies it is a valid operation.  Its says nothing about the success 
 *    of the comparison.
 *    The value of leftOpP indicates if the operation succeeded.
 */
PRIVATE boolean_t DumpEntryBinaryOp(leftOpP, rightOpP, binaryOp)
  IN var_val_t *leftOpP;
  IN var_val_t *rightOpP;
  IN char *binaryOp;
{
    boolean_t validOp = FALSE, opSucceeded = FALSE, areEqual;
    budb_dumpEntry *leftValP, *rightValP;
    var_val_t *opResultP;

    leftValP = (budb_dumpEntry *) VALUE_VAL(leftOpP);
    rightValP = (budb_dumpEntry *) VALUE_VAL(rightOpP);

    areEqual = itlBak_DumpEntryCompare(leftValP, rightValP);
    
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
	  itl_Error("Operation, %s, NOT supported on fs_bakDumpEntry's.\n",
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

PRIVATE char *DumpEntryDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    char *displayStr, *index;
    char *tapesStr, *dumperStr;
    budb_dumpEntry *dumpEntryValP;
    int strLength;

    dumpEntryValP = (budb_dumpEntry *) VALUE_VAL(displayVarP);

    /*
     *  Obtain the strings for the sub-datatypes.  Use the lengthP argument
     *  as a temporary holding place for the length of the sub-datatype
     *  strings.
     */
    tapesStr = itlBak_TapeSetDisplay(&(dumpEntryValP->tapes), lengthP);
    strLength = *lengthP;
    dumperStr = itlBak_PrincipalDisplay(&(dumpEntryValP->dumper), lengthP);
    strLength += *lengthP;

    /*
     *  Calculations for the length of the string assume worst case for
     *  the fields of the unsigned32 data type.  That is they are assumed to
     *  be 10 characters large.
     *  An additional byte is added for the end of string character, and
     *  the length of the labels used in the sprintf that follows are
     *  also included.
     */
    strLength += 10 + 6 +                    /* id field */
	         10 + 10 +                   /* parent field */
	         10 + 9 +                    /* level field */
	         10 + 9 +                    /* flags field */
	         BU_MAXNAMELEN + 15 +        /* volumeSetName field */
	         BU_MAXPATHLEN + 10 +        /* dumpPath field */
	         BU_MAXNAMELEN + 6 +         /* name field */
		 CHARS_IN_DATE_STRING + 9 +  /* created field */
		 CHARS_IN_DATE_STRING + 9 +  /* incTime field */
		 10 + 9 +                    /* spare1 field */
		 10 + 9 +                    /* spare2 field */
		 10 + 9 +                    /* spare3 field */
		 10 + 9 +                    /* spare4 field */
		 40 +                        /* newlines and spaces */
		 10;                         /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "\n  Id: %10d\n  Parent: %10d\n",
	    dumpEntryValP->id, dumpEntryValP->parent);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Level: %10d   Flags: 0x%x\n",
	    dumpEntryValP->level, dumpEntryValP->flags);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  VolumeSetName: %s\n  DumpPath: %s\n  Name: %s\n",
	    dumpEntryValP->volumeSetName, dumpEntryValP->dumpPath,
	    dumpEntryValP->name);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Created: %s  IncTime: %s",
	    ctime((time_t *) &(dumpEntryValP->created)),
	    ctime((time_t *) &(dumpEntryValP->incTime)));
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare1: %10d   Spare2: %10d\n",
	    dumpEntryValP->spare1, dumpEntryValP->spare2);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare3: %10d   Spare4: %10d\n",
	    dumpEntryValP->spare3, dumpEntryValP->spare4);

    assert((int) strlen(displayStr) < strLength);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);
}

PRIVATE var_val_t *DumpEntryGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    budb_dumpEntry *dumpEntryValP;

    dumpEntryValP = (budb_dumpEntry *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, dumpEntryFieldNames,
			sizeof(dumpEntryFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* id */
	    retValP = itlStds_IntVal((int) dumpEntryValP->id);
	    break;

	  case 1:   /* parent */
	    retValP = itlStds_IntVal((int) dumpEntryValP->parent);
	    break;

	  case 2:   /* level */
	    retValP = itlStds_IntVal((int) dumpEntryValP->level);
	    break;

	  case 3:   /* flags */
	    retValP = itlStds_IntVal((int) dumpEntryValP->flags);
	    break;

	  case 4:   /* volumeSetName */
	    retValP = itlStds_StringVal((char *) dumpEntryValP->volumeSetName);
	    break;

	  case 5:   /* dumpPath */
	    retValP = itlStds_StringVal((char *) dumpEntryValP->dumpPath);
	    break;

	  case 6:   /* name */
	    retValP = itlStds_StringVal((char *) dumpEntryValP->name);
	    break;

	  case 7:   /* created */
	    retValP = itlStds_IntVal((int) dumpEntryValP->created);
	    break;

	  case 8:   /* incTime */
	    retValP = itlStds_IntVal((int) dumpEntryValP->incTime);
	    break;

	  case 9:   /* nVolumes */
	    retValP = itlStds_IntVal((int) dumpEntryValP->nVolumes);
	    break;

	  case 10:  /* tapes */
	    retValP = itlBak_TapeSetCreate(&(dumpEntryValP->tapes));
	    break;

	  case 11:  /* dumper */
	    retValP = itlBak_PrincipalCreate(&(dumpEntryValP->dumper));
	    break;

	  case 12:  /* spare1 */
	    retValP = itlStds_IntVal((int) dumpEntryValP->spare1);
	    break;

	  case 13:  /* spare2 */
	    retValP = itlStds_IntVal((int) dumpEntryValP->spare2);
	    break;

	  case 14:  /* spare3 */
	    retValP = itlStds_IntVal((int) dumpEntryValP->spare3);
	    break;

	  case 15:  /* spare4 */
	    retValP = itlStds_IntVal((int) dumpEntryValP->spare4);
	    break;
	}
    }
    else
	itl_Error("Field table lookup FAILED.\n");

    return(retValP);
}


PRIVATE boolean_t DumpEntrySetField(setValP, fieldName, fieldValP)
  IN var_val_t *setValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    int fieldNum;
    budb_dumpEntry *dumpEntryValP;
    boolean_t retVal = FALSE;

    if (itl_LookupTable(fieldName, dumpEntryFieldNames,
			sizeof(dumpEntryFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	dumpEntryValP = (budb_dumpEntry *) VALUE_VAL(setValP);

	switch (fieldNum) {
	  case 0:   /* id */
	    ITLU_SET_INT_FIELD(fieldValP, dumpEntryValP->id, dumpId, retVal);
	    break;

	  case 1:   /* parent */
	    ITLU_SET_INT_FIELD(fieldValP, dumpEntryValP->parent, dumpId, 
			       retVal);
	    break;
	
	  case 2:   /* level */
	    ITLU_SET_INT_FIELD(fieldValP, dumpEntryValP->level, signed32,
			       retVal);
	    break;

	  case 3:   /* flags */
	    ITLU_SET_INT_FIELD(fieldValP, dumpEntryValP->flags, signed32,
			       retVal);
	    break;

	  case 4:   /* volumeSetName */
	    ITLU_SET_STRING_FIELD(fieldValP, &(dumpEntryValP->volumeSetName), 
				  BU_MAXNAMELEN, retVal);
	    break;

	  case 5:   /* dumpPath */
	    ITLU_SET_STRING_FIELD(fieldValP, &(dumpEntryValP->dumpPath), 
				  BU_MAXPATHLEN, retVal);
	    break;

	  case 6:   /* name */
	    ITLU_SET_STRING_FIELD(fieldValP, &(dumpEntryValP->name), 
				  BU_MAXNAMELEN, retVal);
	    break;

	  case 7:   /* created */
	    ITLU_SET_INT_FIELD(fieldValP, dumpEntryValP->created, Date,
			       retVal);
	    break;

	  case 8:   /* incTime */
	    ITLU_SET_INT_FIELD(fieldValP, dumpEntryValP->incTime, Date,
			       retVal);
	    break;

	  case 9:   /* nVolumes */
	    ITLU_SET_INT_FIELD(fieldValP, dumpEntryValP->nVolumes, signed32,
			       retVal);
	    break;

	  case 10:  /* tapes */
	    if (VALUE_TYPE(fieldValP) == itlBak_tapeSetDataType)
	    {
		dumpEntryValP->tapes = *((budb_tapeSet *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 11:  /* dumper */
	    if (VALUE_TYPE(fieldValP) == itlBak_principalDataType)
	    {
		dumpEntryValP->dumper = *((budb_principal *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 12:  /* spare1 */
	    ITLU_SET_INT_FIELD(fieldValP, dumpEntryValP->spare1, unsigned32,
			       retVal);
	    break;

	  case 13:  /* spare2 */
	    ITLU_SET_INT_FIELD(fieldValP, dumpEntryValP->spare2, unsigned32,
			       retVal);
	    break;

	  case 14:  /* spare3 */
	    ITLU_SET_INT_FIELD(fieldValP, dumpEntryValP->spare3, unsigned32,
			       retVal);
	    break;

	  case 15:  /* spare4 */
	    ITLU_SET_INT_FIELD(fieldValP, dumpEntryValP->spare4, unsigned32,
			       retVal);
 	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retVal);
}

/*
 *  itlBak_DumpEntryGet() --
 *    Reads a budb_dumpEntry data type from the input stream.
 *
 *  NOTE:
 *    Caller is responsible for freeing the name string that is returned.
 *
 *  RETURN CODES:
 *    TRUE  - success
 *    FALSE - failure
 */
EXPORT boolean_t itlBak_DumpEntryGet(varNamePP, dumpEntryP)
  OUT char **varNamePP;
  OUT budb_dumpEntry *dumpEntryP; /* ptr to caller allocated structure */
{
    itl_var_t *varP;
    var_val_t *dumpEntryVarP;
    char *varName;
    boolean_t err, retVal = TRUE;

    itl_PromptUser("Dump Entry Variable Name: ");
    varName = itl_GetVariableName(&err, NULL);
    if ((varP = itl_LocateVariable(varName)) != (itl_var_t *) 0)
    {
	dumpEntryVarP = VAR_VALUE_REC(varP);
	*dumpEntryP = *((budb_dumpEntry *) VALUE_VAL(dumpEntryVarP));
	*varNamePP = varName;
    }
    else
    {
	ITL_FREE(varName);
	retVal = FALSE;
    }

    return(retVal);

}
