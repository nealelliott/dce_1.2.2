/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: itl_listtype.c,v $
 * Revision 1.1.313.1  1996/10/17  18:27:28  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:46  damon]
 *
 * Revision 1.1.308.1  1994/02/04  20:45:29  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:34  devsrc]
 * 
 * Revision 1.1.306.1  1993/12/07  17:44:44  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:27:50  jaffe]
 * 
 * Revision 1.1.2.2  1992/11/18  22:00:10  jaffe
 * 	New File
 * 
 * 	$TALog: itl_listtype.c,v $
 * 	Revision 1.13  1994/11/01  22:29:44  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.12 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.12  1993/06/03  16:50:03  fred
 * 	[merge of changes from 1.6 to 1.9 into 1.11]
 * 
 * Revision 1.9  1993/04/21  19:12:48  srinivas
 * 	changes for solaris
 * 
 * 	changes for solaris
 * 	[from r1.6 by delta srinivas-itl-1, r1.1]
 * 
 * Revision 1.11  1993/06/03  15:33:09  fred
 * 	[merge of changes from 1.9 to 1.10 into 1.6]
 * 
 * Revision 1.10  1993/06/03  12:50:13  fred
 * 	To port all ITL based test programs to Solaris.  This includes the following
 * 	test programs,
 * 
 * 	fset_test, fx_test, bakserver_test file_test
 * 
 * 	as well as all the code under the src/test/file/itl directory.
 * 
 * 	The changes, to the .c files, if not explicitly listed, would normally be
 * 	one or more of the following.
 * 
 * 	include stdlib.h to define the free() function
 * 	include sys/types.h
 * 	work around the fact that the sys/mman.h file on Solaris defines
 * 	  the constants PRIVATE and SHARED.
 * 	remove extra ';', usually at the end of macro declarations
 * 
 * 	I believe all bsd'isms are now removed from ITL.
 * 
 * 	SUNOS5 porting changes.
 * 	[from r1.9 by delta fred-db3612-port-itl-to-solaris, r1.1]
 * 
 * Revision 1.6  1993/03/08  13:52:00  fred
 * 	To import fixes to ITL from Ofer.  Also, fixed some outstanding minor
 * 	bugs we found -
 * 	 - The spoke (distributed ITL) had a bug in it's sync point code.  It was
 * 	   not removing a variable from the hash table, which caused confusion when
 * 	   a sync point name was re-used in a tight loop.  Also, it was not
 * 	   setting the timeout field correctly during sync point creation.
 * 
 * 	 - Remove the dependency of the ITL utilities library on the com_err
 * 	   library.  I wrote our own error handling function that deals with both
 * 	   dce and errno correctly.  Besides, the error_message() function in the
 * 	   com_err library is not thread-safe.
 * 
 * 	Incorporation of Encina ITL fixes.
 * 	[from r1.5 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 * 
 * Revision 1.5  1993/02/02  15:43:23  fred
 * 	[merge of changes from 1.2 to 1.4 into 1.3]
 * 
 * Revision 1.4  1993/01/19  21:12:55  fred
 * 	To add the pack/unpack capabilities to ITL variables so that we can do
 * 	better distributed testing.  Also added some other ITL bug fixes from Ofer.
 * 
 * 	Not yet complete.  Need to fix the FX tests.
 * 
 * 	Added pack/unpack interfaces to datatype.
 * 	[from r1.2 by delta fred-ot6848-add-pack-unpack-capability, r1.1]
 * 
 * Revision 1.3  1993/01/15  14:50:32  jaffe
 * 	add correct history and copyright
 * 	[from r1.2 by delta osf-revdrop-01-12-92, r1.2]
 * 
 * Revision 1.2  1992/11/23  16:16:43  jaffe
 * 	Put in missing header after latest drop to the OSF.
 * 	[from revision 1.1 by delta osf-sync-with-nov-19-drop, r1.1]
 * 
 * 	Revision 1.1  1992/11/03  15:27:39  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 	[added by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.4  1992/10/20  19:17:01  oz
 * 	This delta implements ITL procedures with parameters and local variables.
 * 	The delta is backwards compatible: the set command for procedures
 * 	takes an optional integer for the number of parameters and another
 * 	optional number for the number of local variables.  The names of
 * 	the variables defines as either parameters to the procedures or as
 * 	local variables can be accessed in the procedure body by name.
 * 	[added by delta oz-4963-ITL-implement-procs-with-params-and-locals, r1.1]
 * 
 * Revision 1.3  1992/10/18  15:02:50  oz
 * 	Added a read function to itlVal types and corrected
 * 	the unary # operation.
 * 	[from revision 1.2 by delta oz-4963-ITL-create-procs-with-params, r1.6]
 * 
 * Revision 1.2  1992/10/18  02:34:15  oz
 * 	- Fixed typos and compilation error
 * 	[from revision 1.1 by delta oz-4963-ITL-create-procs-with-params, r1.4]
 * 
 * Revision 1.1  1992/10/17  19:02:18  oz
 * 	- Added files and enhances basic test script
 * 	[added by delta oz-4963-ITL-create-procs-with-params, r1.2]
 * 
 * $EndLog$
 */

/*
#ifdef	SUNOS5
#include <sys/types.h>
#endif
*/

#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_value.h>

typedef struct {
    int    numVals;
    var_val_t   **valList;
} valList_t;

SHARED int itl_valListType;

#define LIST_VALUE_VAL(valP)  ((valList_t *)VALUE_VAL(valP))


/*
 *  FreeValList
 */
PRIVATE boolean_t FreeValList(varValP)
  IN var_val_t *varValP;
{
    int i;
    if (VALUE_VAL(varValP)) {
	for (i=0; i< LIST_VALUE_VAL(varValP)->numVals; i++)
	    itl_FreeVarVal(LIST_VALUE_VAL(varValP)->valList[i]);
	ITL_FREE(LIST_VALUE_VAL(varValP)->valList);
	ITL_FREE(VALUE_VAL(varValP));
    }
    return(FALSE);
}

/*
 *  CopyValList.  
 */
PRIVATE var_val_t *CopyValList(inVal)
  IN var_val_t *inVal;
{
    var_val_t *outVal;

    valList_t *inValP = LIST_VALUE_VAL(inVal);
    valList_t *NewValP;
    int           i;

    ITL_NEW(NewValP, valList_t);
    NewValP->numVals = inValP->numVals;

    if (NewValP->numVals > 0)
	ITL_ALLOC(NewValP->valList, var_val_t **,
		  NewValP->numVals * sizeof(var_val_t *));

    for (i = 0; i < NewValP->numVals; i++){
	NewValP->valList[i] = itlValue_Copy(inValP->valList[i]);
    }
    outVal = itlValue_New((pointer_t)NewValP, itl_valListType);
    return(outVal);
}

PRIVATE char * ValListValueToString(valP, lenP)
  IN var_val_t *valP;
  OUT int *lenP;
{
    char          *result;
    valList_t *StrVal = LIST_VALUE_VAL(valP);

    itl_Trace(ITL_VAR_TRACE,
	      "Printing value of type %d.\n", VALUE_TYPE(valP));

    ITL_ALLOC(result, char *, 32);
    sprintf(result,"A list containing %d values.",StrVal->numVals); 
    if (lenP)
	*lenP = strlen(result);

    return(result);
}

PRIVATE char * ValListDisplay(valP, lenP)
  IN var_val_t *valP;
  OUT int *lenP;
{
    char          *result;
    valList_t *StrVal = (valList_t *)VALUE_VAL(valP);
    int            i, maxlen = 1024;
    char          *current;

    itl_Trace(ITL_VAR_TRACE,
	      "Printing value of type %d.\n", VALUE_TYPE(valP));

    ITL_ALLOC(result, char *, maxlen);
    current = result;
    sprintf(current,"This List containing %d values:",StrVal->numVals);
    current += strlen(result);

    for (i=0; i< StrVal->numVals; i++) {
	int newLen;
	char *tempStr = itlValue_Display(StrVal->valList[i], &newLen);
	int diff = current - result;
	if (diff + newLen + 12 > maxlen -1) {
	    maxlen += newLen + 1024;
	    ITL_REALLOC(result, char *, (maxlen * sizeof(char)));
	    current = (char *)((int) result + diff);
	}
	sprintf(current, "\t[%d]:\t%s", i, tempStr);
	ITL_FREE(tempStr);
	current += newLen;
	current += strlen(current);
    }

    if (lenP)
	*lenP = strlen(result);

    return(result);
}

PRIVATE boolean_t ValListConvert(value, type)
  IN var_val_t *value;
  IN int type;
{
    itl_Error("Cannot convert valList types\n");
    return TRUE;
}

PRIVATE boolean_t EvalBinaryValListOp(leftVal, rightVal, Op)
  INOUT var_val_t *leftVal;
  IN var_val_t *rightVal;
  IN char *Op;
{
    if (strcmp(Op, "==") == 0) {
	var_val_t *newValP;
	int *res;
	ITL_NEW(res, int);
	if (VALUE_TYPE(leftVal) != VALUE_TYPE(rightVal)) {
	    *res = FALSE;
	} else {
	    valList_t *leftP = LIST_VALUE_VAL(leftVal);
	    valList_t *rightP = LIST_VALUE_VAL(rightVal);
	    if (leftP->numVals != rightP->numVals)
		*res = FALSE;
	    else {
		int i;
		*res = TRUE;
		for (i=0; i<leftP->numVals; i++) {
		    if (itlValue_BinaryOp(leftP->valList[i],
					  rightP->valList[i],
					  "==") ||
			INT_VALUE_VAL(leftP->valList[i]) == 0) {
			*res = FALSE;
			break;
		    }
		}
	    }
	}
	newValP = itlValue_New(res, itl_intType);
	itlValue_Free(leftVal);
	*leftVal = *newValP;
	ITL_FREE(newValP);
	return(FALSE);
    } else {
	itl_Error("Binary op %s is not impelemented on valList types.\n",
		  Op);
	return TRUE;
    }
}

PRIVATE boolean_t EvalUnaryValListOp(inVal, Op)
  IN var_val_t *inVal;
  IN char *Op;
{
    if (*Op == '#') {
	var_val_t *newValP;
	int res = LIST_VALUE_VAL(inVal)->numVals;
	newValP = itlStds_IntVal(res);

	itlValue_Free(inVal);
	*inVal = *newValP;
	ITL_FREE(newValP);
	return(FALSE);
    }
    itl_Error("Unary ops are not impelementIded on valList types\n");
    return TRUE;
}

/* 
 */
PRIVATE var_val_t *ReadValList _TAKES((
  IN var_val_t *valP,
  IN int type
));
PRIVATE var_val_t *ReadValList(valP, type)
  IN var_val_t *valP;
  IN int type;
{
    valList_t *newListP;
    int i;
    if (!valP) {
	itl_PromptUser("# of entries: ");
	valP = itl_GetVarVal();
    }
    if (VALUE_TYPE(valP) == itl_valListType) {
	return(valP);
    } else if (!itlValue_ConvertValue(valP, itl_intType)) {
	return(NULL);
    }

    ITL_NEW(newListP, valList_t);
    newListP->numVals = INT_VALUE_VAL(valP);
    itl_FreeVarVal(valP);

    if (newListP->numVals > 0)
	ITL_ALLOC(newListP->valList, var_val_t **,
		  newListP->numVals * sizeof(var_val_t *));

    for (i=0; i<newListP->numVals; i++) {
	itl_PromptUser("Value %d: ", i);
	newListP->valList[i] = itl_GetVarVal();
    }
    
    return itlValue_New((pointer_t)newListP, itl_valListType);
}


/*
 *  New simple types
 */
PRIVATE var_val_t *ValListNew _TAKES((
  IN  pointer_t ptr,
  IN int type
));
PRIVATE var_val_t *ValListNew(ptr, type)
  IN  pointer_t ptr;
  IN int type;
{
    var_val_t *newValue;
    ITL_NEW(newValue, var_val_t);
    VALUE_VAL(newValue) = ptr;
    VALUE_TYPE(newValue) = type;
    VALUE_SIZE(newValue) = LIST_VALUE_VAL(newValue)->numVals;
    return(newValue);
}

PRIVATE var_val_t *ValListGetField(valP, name)
  IN	var_val_t *valP;
  IN	char *name;
{
    valList_t *valListP;
    char         *Str;
    var_val_t *retVal;
    int index;

    if (VALUE_TYPE(valP) != itl_valListType)
	return NULL;
    valListP = LIST_VALUE_VAL(valP);

    if (strcmp(name, "len") == 0) {
	return (itlStds_IntVal(valListP->numVals));
    } else {
	char *endStr;
	int index = strtol(name, &endStr, 0);

	if ((index >= valListP->numVals) || (index < 0)) {
	    itl_Error("Index %d is out of range(max = %d)!\n", index, 
		      valListP->numVals -1);
	    return (var_val_t *)NULL;
	} else if (*endStr != '\0') {
	    itl_Error("%s is an invalid field for valList\n", name);
	    return (var_val_t *)NULL;
	} else {
	    return (itlValue_Copy(valListP->valList[index]));
	}
    }
}


PRIVATE boolean_t ValListSetField(inVal, name, fieldVal)
  IN	var_val_t *inVal;
  IN	char *name;
  IN	var_val_t *fieldVal;
{
    valList_t *valListP;
    char         *Str;
    var_val_t *retVal;
    int index;

    if (VALUE_TYPE(inVal) != itl_valListType)
	return(FALSE);
    valListP = LIST_VALUE_VAL(inVal);

    if (strcmp(name, "len") == 0) {
	itl_Error("The length of the list cannot be modified (it is %d)\n",
		  LIST_VALUE_VAL(inVal)->numVals);
	return(FALSE);
    } else {
	char *endStr;
	int index = strtol(name, &endStr, 0);
	
	if (*endStr != '\0') {
	    itl_Error("%s is not a valid field for type valList\n", name);
	    return(FALSE);
	} else if ((index >= valListP->numVals) || (index < 0)) {
	    itl_Error("Index %d is out of range(max = %d)!\n", index, 
		      valListP->numVals -1);
	    return (FALSE);
	} else {
	    itl_FreeVarVal(valListP->valList[index]);
	    valListP->valList[index] = itlValue_Copy(fieldVal);
	    return(TRUE);
	}
    }
}


SHARED itlValue_varTypeInfo_t itl_valList[] = {
{ "valList",
      CopyValList,
      ValListNew,
      FreeValList,
      ReadValList,
      EvalBinaryValListOp,
      EvalUnaryValListOp,
      ValListConvert,
      ValListValueToString,
      ValListDisplay,
      ValListGetField,
      ValListSetField,
      NULL,
      NULL,
      "List of variable values",
      "The valList type contains a list of variable values.  \
It allows access to two fields: \n\
  len - which gives the number of values\n\
  %d - (an integer) returns a var_val_t containing the i'th value"
      }
};

SHARED void itl_ValListInit()
{
    itl_valListType = itlValue_RegisterVarType(&itl_valList[0]);
}

SHARED var_val_t *itl_SetValListVar(num, valArray)
  IN int     num;
  IN var_val_t  **valArray;
{
    var_val_t  *tempP;
    valList_t  myList;
    var_val_t  *tempValP;

    myList.numVals = num;
    myList.valList = valArray;
    tempP = itlValue_New((pointer_t)&myList, itl_valListType);
    tempValP = itlValue_Copy(tempP);
    ITL_FREE(tempP);
    return(tempValP);
}
