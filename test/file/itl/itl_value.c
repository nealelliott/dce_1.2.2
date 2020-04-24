/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl_value.c,v 1.1.413.1 1996/10/17 18:27:53 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1991
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: itl_value.c,v $
 * Revision 1.1.413.1  1996/10/17  18:27:53  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:00  damon]
 *
 * Revision 1.1.408.1  1994/02/04  20:45:48  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:45  devsrc]
 * 
 * Revision 1.1.406.1  1993/12/07  17:44:59  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:30:45  jaffe]
 * 
 * Revision 1.1.2.5  1993/01/11  18:31:41  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:11:32  htf]
 * 
 * Revision 1.1.2.4  1992/11/18  22:00:51  jaffe
 * 	Transarc delta: fred-itl-ofer-fixes-10-30 1.1
 * 	  Selected comments:
 * 	    Incorporates Ofer's changes that permit procedures to have parameters
 * 	    passed to them, and also includes the vector and valList data types.  The
 * 	    vector data type is needed for DFS testing.  It allows us to manipulate
 * 	    binary data.
 * 	    Added itlValue_ConvertValue().
 * 	[1992/11/18  15:37:28  jaffe]
 * 
 * Revision 1.1.2.3  1992/10/28  17:58:40  jaffe
 * 	Transarc delta: fred-itl-dir-stat-mem-cmds 1.2
 * 	  Selected comments:
 * 	    Adding support for directory functions, stat function, and memory mapped
 * 	    files.
 * 	    Not done yet, just upgrading.
 * 	    This complets this delta.
 * 	    Fixed bug found by Ofer in TP code.  Function interface change.
 * 	Transarc delta: fred-itl-ofer-fixes-9-22-92 1.2
 * 	  Selected comments:
 * 	    Incorporate fixes made to the TP version of ITL.  Some of them are performance
 * 	    related.  Others concerned the printing of error messages.  In most cases
 * 	    messages are no longer printed by support functions.  The determination of
 * 	    whether a message should be printed is now left to functions that call these
 * 	    support functions.
 * 	    Removed error messages.
 * 	    Fix (picky) compiler warnings
 * 	[1992/10/28  12:39:31  jaffe]
 * 
 * 	$TALog: itl_value.c,v $
 * 	Revision 1.13  1994/11/01  22:30:14  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.12 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.12  1993/02/02  15:43:46  fred
 * 	[merge of changes from 1.9 to 1.11 into 1.10]
 * 
 * Revision 1.11  1993/01/19  21:12:58  fred
 * 	To add the pack/unpack capabilities to ITL variables so that we can do
 * 	better distributed testing.  Also added some other ITL bug fixes from Ofer.
 * 
 * 	Not yet complete.  Need to fix the FX tests.
 * 
 * 	Added pack/unpack functions.
 * 	[from r1.9 by delta fred-ot6848-add-pack-unpack-capability, r1.1]
 * 
 * Revision 1.10  1993/01/14  17:38:49  jaffe
 * 	revdrop of latest OSF copyright changes to these files
 * 	[from r1.9 by delta osf-revdrop-01-12-92, r1.1]
 * 
 * 	Revision 1.8  1992/11/10  19:49:31  jaffe
 * 	Sync up with the OSF source as of Nov 9.  This delta fixes mostly test
 * 	code problems related to missing OSF copyrights.
 * 	[from revision 1.5 by delta osf-sync-with-nov-9-drop, r1.1]
 * 
 * Revision 1.6  1992/11/03  15:27:48  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 
 * 	Added itlValue_ConvertValue().
 * 	[from revision 1.5 by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.5  1992/10/05  16:17:19  jdp
 * 	[merge of changes from 1.1 to 1.4 into 1.3]
 * 
 * Revision 1.4  1992/10/02  20:32:15  jaffe
 * 	Sync with OSF sources.
 * 
 * 	Added correct OSF headers.
 * 	[from revision 1.1 by delta osf-cleanup-from-sept-25-92-drop-test, r1.1]
 * 
 * Revision 1.3  1992/09/23  15:53:45  fred
 * 	Incorporate fixes made to the TP version of ITL.  Some of them are performance
 * 	related.  Others concerned the printing of error messages.  In most cases
 * 	messages are no longer printed by support functions.  The determination of
 * 	whether a message should be printed is now left to functions that call these
 * 	support functions.
 * 
 * 	Removed error messages.
 * 	[from revision 1.2 by delta fred-itl-ofer-fixes-9-22-92, revision 1.1]
 * 
 * Revision 1.2  1992/09/10  11:51:04  fred
 * 	This complets this delta.
 * 
 * 	Fixed bug found by Ofer in TP code.  Function interface change.
 * 	[from revision 1.1 by delta fred-itl-dir-stat-mem-cmds, revision 1.2]
 * 
 * Revision 1.1  1992/08/10  20:11:35  fred
 * 	Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	Initial checkin.
 * 
 * 	Initial checkin.
 * 	[added by delta fred-checkin-itl, revision 1.1]
 * 
 * Revision 1.5  1992/04/28  04:36:42  oz
 * 	Continued Work.
 * 
 * 	Corrected itlValue_Display (it did not return the result)
 * 	[from revision 1.4 by delta oz-2495-ITL:Add-display-upcall-to-var-types, revision 1.3]
 * 
 * Revision 1.4  1992/04/27  21:02:06  oz
 * 	Add a display upcall for long form display of variable types.
 * 
 * 	itlValue_ToString is now called itlValue_PrintString
 * 	New functions:
 * 	itlValue_DisplayValue
 * 	itlValue_Display
 * 	[from revision 1.3 by delta oz-2495-ITL:Add-display-upcall-to-var-types, revision 1.1]
 * 
 * Revision 1.3  1992/04/10  19:40:29  oz
 * 	More corrections
 * 
 * 	Added itlValue_New
 * 	[from revision 1.2 by delta oz:2334-Allow-access-to-var-upcalls, revision 1.4]
 * 
 * Revision 1.2  1992/04/10  18:55:59  oz
 * 	Continued changes:
 * 
 * 	Function upcalls are now optional
 * 	Added itlValue_VarValConvert
 * 	[from revision 1.1 by delta oz:2334-Allow-access-to-var-upcalls, revision 1.2]
 * 
 * Revision 1.1  1992/04/10  03:12:10  oz
 * 	The ITL user can define her own variable types.
 * 	This delta colocates all variable type information in one module
 * 	called itl_value.[ch]
 * 	  In addition, when the client defines and registers variable types
 * 	the client provides upcall ITL uses to manipulate the type.  The new
 * 	module allows the user access to these upcalls, providing greated
 * 	flexibility in defining new, and recursive, types.
 * 
 * 	Some functions were moved from itl_exp.c, some are new.
 * 	[added by delta oz:2334-Allow-access-to-var-upcalls, revision 1.1]
 * 
 * $EndLog$
 */

#include <assert.h>
#include <utils/itl/itl_private.h>
#include <utils/itl/itl_value.h>



/*
 *  Registerd Variable Types
 *
 */
PRIVATE itlValue_varTypeInfo_t *varTypeArray;
PRIVATE int numVarTypes;

/*
 * itlValue_RegisterVarType
 */
SHARED int itlValue_RegisterVarType(infoP)
  IN itlValue_varTypeInfo_t *infoP;
{
    numVarTypes++;
    ITL_UTIL_REALLOC(varTypeArray, itlValue_varTypeInfo_t *,
			 (numVarTypes * sizeof(itlValue_varTypeInfo_t)));
    varTypeArray[numVarTypes-1] = *infoP;
    return(numVarTypes-1);
}

/*
 *  itlValue_Free
 *	Free an arbitrary type.
 */
SHARED void itlValue_Free(varValP)
  IN var_val_t *varValP;
{
    if ((VALUE_TYPE(varValP) < numVarTypes) &&
	varTypeArray[VALUE_TYPE(varValP)].free) {
	(varTypeArray[VALUE_TYPE(varValP)].free)(varValP);
    }
    else {
	itl_Error("FreeArbitraryType: Invalid type: %d\n",
		  VALUE_TYPE(varValP));
    }
}

SHARED var_val_t *itlValue_Copy(inVal)
  IN var_val_t *inVal;
{
    var_val_t *copyVal;
    if ((VALUE_TYPE(inVal) < numVarTypes) &&
	varTypeArray[VALUE_TYPE(inVal)].copy) {
	copyVal = (varTypeArray[VALUE_TYPE(inVal)].copy)(inVal);
    } else {
	copyVal = (var_val_t *)NULL;
    }
    return(copyVal);
}

SHARED var_val_t *itlValue_New(ptr, type)
  IN pointer_t ptr;
  IN int type;
{
    var_val_t *copyVal;
    if ((type < numVarTypes) && varTypeArray[type].new) {
	copyVal = (varTypeArray[type].new)(ptr, type);
    } else {
	copyVal = (var_val_t *)NULL;
    }
    return(copyVal);
}


SHARED char *itlValue_PrintString(valP, lenP)
  IN	var_val_t *valP;
  OUT	int *lenP;
{
    char *res;
    if ((VALUE_TYPE(valP) < numVarTypes) &&
	varTypeArray[VALUE_TYPE(valP)].printString) {
	res = (varTypeArray[VALUE_TYPE(valP)].printString)(valP, lenP);
    }
    else {
	res = NULL;
	if (lenP)
	    *lenP = 0;
    }
    return(res);
}

SHARED char *itlValue_DisplayValue(valP, lenP)
  IN	var_val_t *valP;
  OUT	int *lenP;
{
    char *res;
    if ((VALUE_TYPE(valP) < numVarTypes) &&
	varTypeArray[VALUE_TYPE(valP)].displayValue) {
	    res = (varTypeArray[VALUE_TYPE(valP)].displayValue)(valP, lenP);
    } else {
	res = NULL;
	if (lenP)
	    *lenP = 0;
    }
    return(res);
}

SHARED char *itlValue_Display(valP, lenP)
  IN	var_val_t *valP;
  OUT	int *lenP;
{
    char *res = itlValue_DisplayValue(valP, lenP);

    if (!res)
	res = itlValue_PrintString(valP, lenP);
    return(res);
}


SHARED boolean_t itlValue_TypeIsValid(type)
  IN int type;
{
    return(type < numVarTypes);
}

/*
 * itlValue_Type --  Convert a variable type to an ASCII string name.
 * 
 */
SHARED char *itlValue_Type(varType)
  IN int varType;
{
    if (varType < numVarTypes) {
	return(varTypeArray[varType].name);
    }
    else {
	return("---  Invalid  ---");
    }
}

SHARED boolean_t itlValue_Convert(value, valType, calledType)
  INOUT var_val_t *value;
  IN int valType;	/*  The type to convert to  */
  IN int calledType;	/*  The type to be called   */
{
    boolean_t res;

    if (VALUE_TYPE(value) == valType) {
	return(FALSE);
    }
    
    if ((calledType >= numVarTypes) || (valType >= numVarTypes) ||
	!varTypeArray[calledType].convert) {
	return(TRUE);
    } else if ((calledType != valType) &&
	       (calledType != VALUE_TYPE(value))) {
	return(TRUE);
    } else {
	res = (*(varTypeArray[calledType].convert))(value, valType);
    }
    return(res);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlValue_ConvertValue
 *     Convert a variable value of one type to another.
 *     The value is changed in place modifying the input value.
 *  The function also returns a pointer to the value for convenience.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
SHARED var_val_t *itlValue_ConvertValue(value, type)
  INOUT var_val_t *value;
  IN int type;
{
    if (!itlValue_Convert(value, type, type)) {
	return(value);
    } else if (!itlValue_Convert(value, type, VALUE_TYPE(value))) {
	return(value);
    } else {
	return(NULL);
    }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlValue_VarValConvert
 *     Convert a variable value of one type to another.
 *     The value is changed in place modifying the input value.
 *  The function also returns a pointer to the value for convenience.
 *  If the conversion could not be performed, itl_Error is called.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
SHARED var_val_t *itlValue_VarValConvert(value, type)
  INOUT var_val_t *value;
  IN int type;
{
    var_val_t *retVal = itlValue_ConvertValue(value, type);
    if (!retVal)
	itl_Error("itlExp_VarValueConvert: Cannot convert from %s to %s\n",
		  itlValue_Type(VALUE_TYPE(value)),
		  itlValue_Type(type));
    return(retVal);
}



/*
 *  itlValue_TypeByName -
 *	Return the type code of a type given by name
 *	If the type is not a recognized type, return -1.
 */
SHARED int itlValue_TypeByName(varType)
  IN char *varType;
{
    int i;
    extern int strcasecmp();

    for (i=0; i < numVarTypes; i++) {
	if (strcasecmp(varType, varTypeArray[i].name) == 0)
	    return(i);
    }
    return(-1);
}


/*
 *  itlValue_Read
 *	Given a type code (int), call the appropriate routine
 *	to read a value of that type.
 *	This function may reuse all or parts of the input value.
 *	That value should be dynamically allocated by the caller
 *	and will be freed by this function if needed.
 */
SHARED var_val_t *itlValue_Read(valP, type)
  IN var_val_t *valP;
  IN int type;
{
    var_val_t *retVal;
    if ((type >= numVarTypes) || (!varTypeArray[type].read)) {
	return(NULL);
    }
    retVal = (varTypeArray[type].read)(valP, type);
    return(retVal);
}


SHARED boolean_t itlValue_BinaryOp(leftVal, rightVal, Op)
  INOUT var_val_t *leftVal;
  IN var_val_t *rightVal;
  IN char *Op;
{
    boolean_t res;
    if ((VALUE_TYPE(leftVal) >= numVarTypes) ||
	!varTypeArray[VALUE_TYPE(leftVal)].binaryOp) {
	return(TRUE);
    }
    res = (varTypeArray[VALUE_TYPE(leftVal)].binaryOp)(leftVal, rightVal, Op);
    return(res);
}

SHARED boolean_t itlValue_UnaryOp(inVal, unaryOp)
  IN var_val_t *inVal;
  IN char *unaryOp;
{
    boolean_t res;
    if ((VALUE_TYPE(inVal) >= numVarTypes) ||
	!varTypeArray[VALUE_TYPE(inVal)].unaryOp) {
	return(TRUE);
    }
    res = (varTypeArray[VALUE_TYPE(inVal)].unaryOp)(inVal, unaryOp);
    return(res);
}

/*
 *  itlValue_GetField
 *     Return one field of a variable which is defined as a complex
 *	structure.
 *
 *  Parameters:
 *	inVal - The input value (var_val_t * type)
 *	name - The (string) name of the field.
 *
 *  This function returns a dynamically allocated value that must
 *  be freed by the caller
 */
EXPORT var_val_t *itlValue_GetField(inVal, fieldName)
  IN var_val_t *inVal;
  IN char *fieldName;
{
    var_val_t *retVal;
    if (!inVal)
	return NULL;
    if ((VALUE_TYPE(inVal) >= numVarTypes) ||
	!varTypeArray[VALUE_TYPE(inVal)].getField) {
	return(NULL);
    }
    retVal = (varTypeArray[VALUE_TYPE(inVal)].getField)(inVal, fieldName);
    return(retVal);
}

/*
 *  itlValue_SetField
 *     Set one field of a variable which is defined as a complex structure.
 *
 *  Parameters:
 *	inVal - The input value (var_val_t * type)
 *	name - The (string) name of the field.
 *	fieldVal - The new value for the field.
 *
 *  This function returns TRUE on success and FALSE if either the field
 *  is not a valid field or if the value is not the correct value for the
 *  field.
 */
SHARED boolean_t itlValue_SetField(inVal, fieldName, fieldVal)
  IN var_val_t *inVal;
  IN char *fieldName;
  IN var_val_t *fieldVal;
{
    if (!(inVal && fieldVal && fieldName))
	return FALSE;
    if (VALUE_TYPE(inVal) >= numVarTypes) {
	return(FALSE);
    }
    if ((VALUE_TYPE(fieldVal) >= numVarTypes) ||
	!varTypeArray[VALUE_TYPE(inVal)].setField) {
	return(FALSE);
    }
    if (!(varTypeArray[VALUE_TYPE(inVal)].setField)(inVal,
						    fieldName,
						    fieldVal)) {
	return(FALSE);
    }
    return(TRUE);
}


/*
 *  itlValue_Pack
 *     Pack a var value into a contiguous buffer
 *
 *  Parameters:
 *	inVal - The input value (var_val_t * type)
 *	lenP - The output length.
 *
 *  This function returns a pointer to the dynamically allocated buffer
 *  that stores the packed value
 *  is not a valid field or if the value is not the correct value for the
 *  field.
 */
SHARED pointer_t itlValue_Pack(inVal, lenP)
  IN  var_val_t *inVal;
  OUT int *lenP;
{
    pointer_t retVal;

    if (!inVal ||
	(VALUE_TYPE(inVal) >= numVarTypes))
	return NULL;

    if (varTypeArray[VALUE_TYPE(inVal)].pack)
	retVal = (varTypeArray[VALUE_TYPE(inVal)].pack)(inVal, lenP);
    else {
	/*
	 *  The type is a simple type, we will do the packing ourselves
	 */
	ITL_ALLOC(retVal, pointer_t, VALUE_SIZE(inVal));
	ITL_MEMCOPY(VALUE_VAL(inVal), retVal, VALUE_SIZE(inVal));
	*lenP = VALUE_SIZE(inVal);
    }
    return(retVal);
}

/*
 *  itlValue_Unpack
 *     Unpack a var value from a contiguous buffer to a val_val_t 
 *
 *  Parameters:
 *	bufP - The input buffer (pointer_t)
 *	len  - The buffer length.
 *	type - The ITL variable type code.
 *
 *  This function returns a pointer to the dynamically allocated var_val_t
 *  structure that contains the new value.
 */
SHARED var_val_t *itlValue_Unpack(bufP, len, type)
  IN  pointer_t bufP;
  IN  int len;
  IN  int type;
{
    var_val_t*retVal;

    if (!bufP ||
	(type >= numVarTypes))
	return NULL;

    if (varTypeArray[type].unpack)
	retVal = (varTypeArray[type].unpack)(bufP, len, type);
    else {
	/*
	 *  The type is a simple type, we will do the packing ourselves
	 */
	pointer_t newBuf;
	ITL_ALLOC(newBuf, pointer_t, len);
	ITL_MEMCOPY(bufP, newBuf, len);
	retVal = itlValue_New(newBuf, type);
	assert(VALUE_SIZE(retVal) == len);
    }
    return(retVal);
}

SHARED char *itlValue_ShortHelp(type)
  IN	int type;
{
    if (type >= numVarTypes)
	return NULL;
    else
	return(varTypeArray[type].shortHelp);
}


SHARED char *itlValue_Help(type)
  IN	int type;
{
    if (type >= numVarTypes)
	return NULL;
    else
	return(varTypeArray[type].help);
}

SHARED void itlValue_Init()
{
    numVarTypes = 0;
    /*
     *  Initialize the variable-types  array.
     */
    ITL_NEW(varTypeArray, itlValue_varTypeInfo_t);
}
