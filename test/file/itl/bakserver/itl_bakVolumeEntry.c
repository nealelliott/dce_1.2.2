/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1996, 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakVolumeEntry.c --
 *    Code for the budb_volumeEntry data type.
 */
/*
 * HISTORY
 * $Log: itl_bakVolumeEntry.c,v $
 * Revision 1.1.6.1  1996/10/17  18:25:01  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:37  damon]
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
#include <utils/itl/itl_commonHyper.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakVolumeEntry.c,v 1.1.6.1 1996/10/17 18:25:01 damon Exp $")

int itlBak_volumeEntryDataType;


PRIVATE var_val_t *VolumeEntryCopy _TAKES((
					 IN var_val_t *copyVarP
					 ));

PRIVATE var_val_t *VolumeEntryNew _TAKES((
					IN pointer_t newValP,
					IN int newType
					));

PRIVATE boolean_t VolumeEntryFree _TAKES((
					IN var_val_t *freeVarP
					));

PRIVATE var_val_t *VolumeEntryRead _TAKES((
					 IN var_val_t *readVolumeEntryP,
					 IN int varType
					 ));

PRIVATE boolean_t VolumeEntryBinaryOp _TAKES((
					    IN var_val_t *leftOpP,
					    IN var_val_t *rightOpP,
					    IN char *binaryOp
					    ));

PRIVATE char *VolumeEntryDisplay _TAKES((
				       IN var_val_t *displayVarP,
				       OUT int *lengthP
				       ));

PRIVATE var_val_t *VolumeEntryGetField _TAKES((
					     IN var_val_t *getVarP,
					     IN char *fieldName
					     ));

PRIVATE boolean_t VolumeEntrySetField _TAKES((
					    IN var_val_t *setValP,
					    IN char *fieldName,
					    IN var_val_t *fieldValP
					    ));


PRIVATE itlValue_varTypeInfo_t volumeEntryVarType = {
    "fs_bakVolumeEntry",
    VolumeEntryCopy,
    VolumeEntryNew,
    VolumeEntryFree,
    VolumeEntryRead,
    VolumeEntryBinaryOp,
    NULL,
    NULL,
    NULL,
    VolumeEntryDisplay,
    VolumeEntryGetField,
    VolumeEntrySetField,
    NULL,
    NULL,
    NULL,
    NULL
};

PRIVATE char *volumeEntryFieldNames[] = {
    "name",
    "flags",
    "id",
    "server",
    "partition",
    "nFrags",
    "position",
    "clone",
    "incTime",
    "startByte",
    "nBytes",
    "seq",
    "dump",
    "tape",
    "spare1",
    "spare2",
    "spare3",
    "spare4"
};

/*
 *  VolumeEntryInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void VolumeEntryInit()
{
    /*
     *  We can safely set volumeEntryHashType because this function is called
     *  via pthread_once.
     */
    itlBak_volumeEntryDataType = itlValue_RegisterVarType(&volumeEntryVarType);
}

/*
 * itlBak_VolumeEntryInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlBak_VolumeEntryInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) VolumeEntryInit))
	itl_Error("pthread_once failed in VolumeEntryInit\n");
}


PRIVATE var_val_t *VolumeEntryCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    budb_volumeEntry *volumeEntryValP = (budb_volumeEntry *) 
	                                VALUE_VAL(copyVarP);
    budb_volumeEntry *newVolumeEntryP;

    assert(VALUE_TYPE(copyVarP) == itlBak_volumeEntryDataType);
    ITL_NEW(newVolumeEntryP, budb_volumeEntry);
    *newVolumeEntryP = *volumeEntryValP;
    return(VolumeEntryNew((pointer_t) newVolumeEntryP, 
			  itlBak_volumeEntryDataType));
}

PRIVATE var_val_t *VolumeEntryNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newVolumeEntryVarP;

    assert(newType == itlBak_volumeEntryDataType);
    ITL_NEW(newVolumeEntryVarP, var_val_t);
    VALUE_TYPE(newVolumeEntryVarP) = newType;
    VALUE_SIZE(newVolumeEntryVarP) = sizeof(budb_volumeEntry);
    VALUE_VAL(newVolumeEntryVarP) = newValP;
    return(newVolumeEntryVarP);
}

PRIVATE boolean_t VolumeEntryFree(freeVarP)
  IN var_val_t *freeVarP;
{
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}

PRIVATE var_val_t *VolumeEntryRead(readVolumeEntryP, varType)
  IN var_val_t *readVolumeEntryP;
  IN int varType;
{
    var_val_t *newVarP;

    assert(varType == itlBak_volumeEntryDataType);
    if (!readVolumeEntryP)
    {
	itl_PromptUser("Name: ");
	readVolumeEntryP = itl_GetVarVal();

	if (VALUE_TYPE(readVolumeEntryP) == itlBak_volumeEntryDataType)
	    newVarP = readVolumeEntryP;
	else
	{
	    budb_volumeEntry *newVolumeEntryP;

	    /*
	     *  The memory allocated here for newVolumeEntryP is free'd in the 
	     *  VolumeEntryFree() routine.
	     */
	    ITL_NEW(newVolumeEntryP, budb_volumeEntry);
	    newVarP = itlValue_ConvertValue(readVolumeEntryP, itl_stringType);
	    assert(newVarP);
	    if ((int) strlen(VALUE_VAL(newVarP)) < BU_MAXNAMELEN)
	    {
		strcpy((char *) newVolumeEntryP->name, 
		       (char *) (VALUE_VAL(newVarP)));
		itl_FreeVarVal(newVarP);

		itl_PromptUser("Flags: ");
		newVolumeEntryP->flags = (unsigned32) itl_GetInteger((boolean_t *) 0);

		itlCommon_HyperGet((afs_hyper_t*) &(newVolumeEntryP->id));
		itlBak_CommonGetLimitedStr("Server", BU_MAXHOSTLEN,
					   (char *) &(newVolumeEntryP->server));
		itl_PromptUser("Partition: ");
		newVolumeEntryP->partition = (signed32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("nFrags: ");
		newVolumeEntryP->nFrags = (signed32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("Position: ");
		newVolumeEntryP->position = (signed32) itl_GetInteger((boolean_t *) 0);
		newVolumeEntryP->clone = (Date) itlBak_CommonGetTime("Clone",
								     FALSE);
		newVolumeEntryP->incTime = (Date) itlBak_CommonGetTime("IncTime",
								      TRUE);
		itl_PromptUser("StartByte: ");
		newVolumeEntryP->startByte = (signed32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("nBytes: ");
		newVolumeEntryP->nBytes = (signed32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("Seq: ");
		newVolumeEntryP->seq = (signed32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("Dump: ");
		newVolumeEntryP->dump = (dumpId) itl_GetInteger((boolean_t *) 0);
		itlBak_CommonGetLimitedStr("Tape", BU_MAXNAMELEN,
					   (char *) &(newVolumeEntryP->tape));
		itl_PromptUser("Spare1: ");
		newVolumeEntryP->spare1 = (unsigned32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("Spare2: ");
		newVolumeEntryP->spare2 = (unsigned32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("Spare3: ");
		newVolumeEntryP->spare3 = (unsigned32) itl_GetInteger((boolean_t *) 0);
		itl_PromptUser("Spare4: ");
		newVolumeEntryP->spare4 = (unsigned32) itl_GetInteger((boolean_t *) 0);

		newVarP = VolumeEntryNew((pointer_t) newVolumeEntryP, 
					 itlBak_volumeEntryDataType);
	    }
	    else
	    {
		itl_Error("Name field length exceeds maximum of %d\n", 
			  BU_MAXNAMELEN);
		newVarP = (var_val_t *) 0;
	    }
	}
    }
    else
	newVarP = (var_val_t *) 0;

    return(newVarP);
}

EXPORT boolean_t itlBak_VolumeEntryCompare(leftP, rightP)
  IN budb_volumeEntry *leftP, *rightP;
{
    boolean_t retVal = FALSE;

    /*
     *  Compare the two structures and save the result.
     */
    if ((!strcmp((char *) leftP->name, (char *) rightP->name)) &&
	(leftP->flags == rightP->flags) &&
	(itlCommon_HyperCompare((afs_hyper_t *) &(leftP->id), 
				(afs_hyper_t *) &(rightP->id)) == 0) &&
	(!strcmp((char *) leftP->server, (char *) rightP->server)) &&
	(leftP->partition == rightP->partition) &&
	(leftP->nFrags == rightP->nFrags) &&
	(leftP->position == rightP->position) &&
	(leftP->clone == rightP->clone) &&
	(leftP->incTime == rightP->incTime) &&
	(leftP->startByte == rightP->startByte) &&
	(leftP->nBytes == rightP->nBytes) &&
	(leftP->seq == rightP->seq) &&
	(leftP->dump == rightP->dump) &&
	(!strcmp((char *) leftP->tape, (char *) rightP->tape)) &&
	(leftP->spare1 == rightP->spare1) &&
	(leftP->spare2 == rightP->spare2) &&
	(leftP->spare3 == rightP->spare3) &&
	(leftP->spare4 == rightP->spare4))
	retVal = TRUE;

    return(retVal);
}

/*
 *  VolumeEntryBinaryOp() --
 *    Just compares two volume entry types.
 *
 *  WARNING:
 *    The return value (boolean) indicates if the operation requested is
 *    valid.  Oddly TRUE means it is not a valid operation and FALSE 
 *    implies it is a valid operation.  Its says nothing about the success 
 *    of the comparison.
 *    The value of leftOpP indicates if the operation succeeded.
 */
PRIVATE boolean_t VolumeEntryBinaryOp(leftOpP, rightOpP, binaryOp)
  IN var_val_t *leftOpP;
  IN var_val_t *rightOpP;
  IN char *binaryOp;
{
    boolean_t validOp = FALSE, opSucceeded = FALSE, areEqual;
    budb_volumeEntry *leftValP, *rightValP;
    var_val_t *opResultP;

    leftValP = (budb_volumeEntry *) VALUE_VAL(leftOpP);
    rightValP = (budb_volumeEntry *) VALUE_VAL(rightOpP);

    areEqual = itlBak_VolumeEntryCompare(leftValP, rightValP);
    
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
	  itl_Error("Operation, %s, NOT supported on fs_bakVolumeEntry's.\n",
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

PRIVATE char *VolumeEntryDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    char *displayStr, *index;
    char *idStr;
    budb_volumeEntry *volumeEntryValP;
    int strLength;

    volumeEntryValP = (budb_volumeEntry *) VALUE_VAL(displayVarP);

    /*
     *  Obtain the strings for the sub-datatypes.  Use the lengthP argument
     *  as a temporary holding place for the length of the sub-datatype
     *  strings.
     */
    idStr = itlCommon_HyperDisplay((afs_hyper_t *) &(volumeEntryValP->id), lengthP);
    strLength = *lengthP;

    /*
     *  Calculations for the length of the string assume worst case for
     *  the fields of the unsigned32 data type.  That is they are assumed to
     *  be 10 characters large.
     *  An additional byte is added for the end of string character, and
     *  the length of the labels used in the sprintf that follows are
     *  also included.
     */
    strLength += BU_MAXNAMELEN + 6 +         /* name field */
	         10 + 9 +                    /* flags field */
	         BU_MAXHOSTLEN + 15 +        /* server field */
	         10 + 11 +                   /* partition field */
	         10 + 8 +                    /* nFrags field */
	         10 + 10 +                   /* position field */
		 CHARS_IN_DATE_STRING + 7 +  /* clone field */
		 CHARS_IN_DATE_STRING + 9 +  /* incTime field */
	         10 + 11 +                   /* startByte field */
	         10 + 8 +                    /* nBytes field */
	         10 + 5 +                    /* seq field */
	         10 + 6 +                    /* dump field */
	         BU_MAXTAPELEN + 6 +         /* tape field */
		 10 + 9 +                    /* spare1 field */
		 10 + 9 +                    /* spare2 field */
		 10 + 9 +                    /* spare3 field */
		 10 + 9 +                    /* spare4 field */
		 55 +                        /* newlines and spaces */
		 10;                         /* paddding */

    ITL_ALLOC(displayStr, char *, strLength);
    sprintf(displayStr, "\n  Name: %s\n  Server: %s\n  Tape: %s\n",
	    volumeEntryValP->name, volumeEntryValP->server,
	    volumeEntryValP->tape);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Id: %s\n  Clone: %s  IncTime: %s",
	    idStr, ctime((time_t *) &(volumeEntryValP->clone)),
	    ctime((time_t *) &(volumeEntryValP->incTime)));
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Flags: %10d   Dump: %10d\n",
	    volumeEntryValP->flags, volumeEntryValP->dump);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Partition: %10d   Postion: %10d\n",
	    volumeEntryValP->partition, volumeEntryValP->position);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  nFrags: %10d   Seq: %10d\n",
	    volumeEntryValP->nFrags, volumeEntryValP->seq);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  StartByte: %10d   nBytes: %10d\n",
	    volumeEntryValP->startByte, volumeEntryValP->nBytes);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare1: %10d   Spare2: %10d\n",
	    volumeEntryValP->spare1, volumeEntryValP->spare2);
    index = &(displayStr[strlen(displayStr)]);
    sprintf(index, "  Spare3: %10d   Spare4: %10d\n",
	    volumeEntryValP->spare3, volumeEntryValP->spare4);

    assert((int) strlen(displayStr) < strLength);

    if (lengthP)
	*lengthP = strLength;

    return(displayStr);
}

PRIVATE var_val_t *VolumeEntryGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum;
    budb_volumeEntry *volumeEntryValP;

    volumeEntryValP = (budb_volumeEntry *) VALUE_VAL(getValP);

    if (itl_LookupTable(fieldName, volumeEntryFieldNames,
			sizeof(volumeEntryFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	switch (fieldNum) {
	  case 0:   /* name */
	    retValP = itlStds_StringVal((char *) volumeEntryValP->name);
	    break;

	  case 1:   /* flags */
	    retValP = itlStds_IntVal((int) volumeEntryValP->flags);
	    break;

	  case 2:   /* id */
	    retValP = itlCommon_HyperCreate(AFS_HGETBOTH(volumeEntryValP->id));
	    break;

	  case 3:   /* server */
	    retValP = itlStds_StringVal((char *) volumeEntryValP->server);
	    break;

	  case 4:   /* partition */
	    retValP = itlStds_IntVal((int) volumeEntryValP->partition);
	    break;

	  case 5:   /* nFrags */
	    retValP = itlStds_IntVal((int) volumeEntryValP->nFrags);
	    break;

	  case 6:   /* position */
	    retValP = itlStds_IntVal((int) volumeEntryValP->position);
	    break;

	  case 7:   /* clone */
	    retValP = itlStds_IntVal((int) volumeEntryValP->clone);
	    break;

	  case 8:   /* incTime */
	    retValP = itlStds_IntVal((int) volumeEntryValP->incTime);
	    break;

	  case 9:   /* startByte */
	    retValP = itlStds_IntVal((int) volumeEntryValP->startByte);
	    break;

	  case 10:  /* nBytes */
	    retValP = itlStds_IntVal((int) volumeEntryValP->nBytes);
	    break;

	  case 11:  /* seq */
	    retValP = itlStds_IntVal((int) volumeEntryValP->seq);
	    break;

	  case 12:  /* dump */
	    retValP = itlStds_IntVal((int) volumeEntryValP->dump);
	    break;

	  case 13:  /* tape */
	    retValP = itlStds_StringVal((char *) volumeEntryValP->tape);
	    break;

	  case 14:  /* spare1 */
	    retValP = itlStds_IntVal((int) volumeEntryValP->spare1);
	    break;

	  case 15:  /* spare2 */
	    retValP = itlStds_IntVal((int) volumeEntryValP->spare2);
	    break;

	  case 16:  /* spare3 */
	    retValP = itlStds_IntVal((int) volumeEntryValP->spare3);
	    break;

	  case 17:  /* spare4 */
	    retValP = itlStds_IntVal((int) volumeEntryValP->spare4);
	    break;
	}
    }
    else
	itl_Error("Field table lookup FAILED.\n");

    return(retValP);
}


PRIVATE boolean_t VolumeEntrySetField(setValP, fieldName, fieldValP)
  IN var_val_t *setValP;
  IN char *fieldName;
  IN var_val_t *fieldValP;
{
    int fieldNum;
    budb_volumeEntry *volumeEntryValP;
    boolean_t retVal = FALSE;

    if (itl_LookupTable(fieldName, volumeEntryFieldNames,
			sizeof(volumeEntryFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
	volumeEntryValP = (budb_volumeEntry *) VALUE_VAL(setValP);

	switch (fieldNum) {
	  case 0:   /* name */
	    ITLU_SET_STRING_FIELD(fieldValP, &(volumeEntryValP->name), 
				  BU_MAXNAMELEN, retVal);
	    break;

	  case 1:   /* flags */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->flags, unsigned32,
			       retVal);
	    break;

	  case 2:   /* id */
	    if (VALUE_TYPE(fieldValP) == itlCommon_hyperDataType)
	    {
		volumeEntryValP->id = *((afs_hyper_t *) VALUE_VAL(fieldValP));
		retVal = TRUE;
	    }
	    break;

	  case 3:   /* server */
	    ITLU_SET_STRING_FIELD(fieldValP, &(volumeEntryValP->server), 
				  BU_MAXHOSTLEN, retVal);
	    break;

	  case 4:   /* partition */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->partition, 
			       signed32, retVal);
	    break;
	
	  case 5:   /* nFrags */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->nFrags, signed32,
			       retVal);
	    break;

	  case 6:   /* position */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->position, signed32,
			       retVal);
	    break;

	  case 7:   /* clone */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->clone, Date,
			       retVal);
	    break;

	  case 8:   /* incTime */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->incTime, Date,
			       retVal);
	    break;

	  case 9:   /* startByte */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->startByte, signed32,
			       retVal);
	    break;

	  case 10:   /* nBytes */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->nBytes, signed32,
			       retVal);
	    break;

	  case 11:  /* seq */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->seq, signed32,
			       retVal);
	    break;

	  case 12:  /* dump */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->dump, dumpId,
			       retVal);
	    break;

	  case 13:   /* tape */
	    ITLU_SET_STRING_FIELD(fieldValP, &(volumeEntryValP->tape), 
				  BU_MAXTAPELEN, retVal);
	    break;

	  case 14:  /* spare1 */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->spare1, unsigned32,
			       retVal);
	    break;

	  case 15:  /* spare2 */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->spare2, unsigned32,
			       retVal);
	    break;

	  case 16:  /* spare3 */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->spare3, unsigned32,
			       retVal);
	    break;

	  case 17:  /* spare4 */
	    ITLU_SET_INT_FIELD(fieldValP, volumeEntryValP->spare4, unsigned32,
			       retVal);
 	}
    }
    else
	itl_Error("Field table lookup field.\n");

    return(retVal);
}

/*
 *  itlBak_VolumeEntryGet() --
 *    Reads a budb_volumeEntry data type from the input stream.
 */
EXPORT void itlBak_VolumeEntryGet(volumeEntryP)
  OUT budb_volumeEntry *volumeEntryP;  /* ptr to caller allocated structure */
{
    var_val_t *varP;

    itl_AdviseUser("Volume Entry -");
    varP = VolumeEntryRead((var_val_t *) 0, itlBak_volumeEntryDataType);
    assert(varP);
    *volumeEntryP = *((budb_volumeEntry *) VALUE_VAL(varP));
    itl_FreeVarVal(varP);
}
