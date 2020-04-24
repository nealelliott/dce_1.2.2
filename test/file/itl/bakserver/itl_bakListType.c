/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakListType.c --
 *    Code for the bak list data types, volume, dump, and tape.
 */
/*
 * HISTORY
 * $Log: itl_bakListType.c,v $
 * Revision 1.1.6.1  1996/10/17  18:24:47  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:30  damon]
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
#include <utils/itl/itl_bakListType.h>
#include <utils/itl/itl_bakVolumeEntry.h>
#include <utils/itl/itl_bakDumpEntry.h>
#include <utils/itl/itl_bakTapeEntry.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakListType.c,v 1.1.6.1 1996/10/17 18:24:47 damon Exp $")

/*
 *  The bakList_t is a generic data type for holding any of the bak server
 *  list types.  It allows for this code to support multiple (currently 4)
 *  list types.
 */
typedef struct {
    itlBak_listTypes_t listType;   /* type of list pointed to by listP */
    pointer_t listP;               /* list data */
} bakList_t;

/*
 *  COPY_LIST() --
 *    Macro for copying the various list types.  It allocates memory
 *    for the new list type, and it's accompanying list.  It then
 *    sets the appropriate fields, and copies the list.
 *    In the calculation of the new list, one (1) is subtracted from
 *    the current list length since it is included in the size of the
 *    list type.
 *
 *  Arguments:
 *
 *    _lp - the listP field of the bakList_t which is being created
 *    _copylp - the listP field of the bakList_t we are copying from
 *    _listType - the type of list, e.g. budb_volumeList
 *    _lenField - name of the length field for the list, 
 *                e.g. budb_volumeList_len
 *    _listField - name of the list field for the list, 
 *                 e.g. budb_volumeList_val
 *    _entryType - the list entry type, e.g. budb_volumeEntry
 */
#define COPY_LIST(_lp, _copylp, _listType, _lenField, _listField, _entryType) \
        MACRO_BEGIN                                                           \
          _listType *cl_destListP, *cl_srcListP;                              \
          int cl_typeSize = sizeof(_entryType);                               \
                                                                              \
          cl_srcListP = (_listType *) _copylp;                                \
          /*                                                                  \
           *  A list of length 0 is valid and is a special case.              \
           */                                                                 \
          if (cl_srcListP->_lenField == 0)                                    \
          {                                                                   \
              ITL_ALLOC(cl_destListP, _listType *, sizeof(_listType));        \
          }                                                                   \
          else                                                                \
          {                                                                   \
              ITL_ALLOC(cl_destListP, _listType *,                            \
			(sizeof(_listType) + ((cl_srcListP->_lenField - 1) *  \
					      cl_typeSize)));                 \
	      (void) memcpy((char *) cl_destListP->_listField,                \
			    (char *) cl_srcListP->_listField,                 \
			    (cl_srcListP->_lenField * cl_typeSize));          \
          }                                                                   \
          cl_destListP->_lenField = cl_srcListP->_lenField;                   \
          _lp = (pointer_t) cl_destListP;                                     \
        MACRO_END

/*
 *  COMPARE_LIST() --
 *    Macro for comparing two list to see if they are equal
 *
 *  Arguments:
 *    _leftlp - pointer to listP field of a bakList_t
 *    _rightlp - pointer to listP field of a bakList_t
 *    _listType - type of list we are comparing, e.g. budb_volumeList
 *    _lenField - name of the length field for the list, 
 *                e.g. budb_volumeList_len
 *    _listField - name of the list field for the list, 
 *                 e.g. budb_volumeList_val
 *    _cmpFunc - function to call to compare each entry in the list.
 *    _equal - will be set to TRUE if the list are equal, and FALSE if 
 *             they are not equal.
 */
#define COMPARE_LIST(_leftlp, _rightlp, _listType, _lenField, _listField, _cmpFunc, _equal)                                                                \
        MACRO_BEGIN                                                         \
          _listType *cl_cmpList1P, *cl_cmpList2P;                           \
          int cl_i;                                                         \
                                                                            \
          cl_cmpList1P = (_listType *) _leftlp;                             \
          cl_cmpList2P = (_listType *) _rightlp;                            \
          if (cl_cmpList1P->_lenField == cl_cmpList2P->_lenField)           \
          {                                                                 \
	      _equal = TRUE;                                                \
              for (cl_i = 0; cl_i < cl_cmpList1P->_lenField; cl_i++)        \
	      {                                                             \
                  if (!_cmpFunc(&(cl_cmpList1P->_listField[cl_i]),          \
				&(cl_cmpList2P->_listField[cl_i])))         \
		  {                                                         \
		      _equal = FALSE;                                       \
                      break;                                                \
		  }                                                         \
	      }                                                             \
          }                                                                 \
          else                                                              \
              _equal = FALSE;                                               \
        MACRO_END

/*
 *  MAKE_TYPE_VARIABLE() --
 *    This macro creates an ITL variable from an entry in one of the list
 *    types.  It is called by the GetField function.
 *
 *  Arguments:
 *    _lp - the listP field of the bakList_t from which the variable
 *          is to be created
 *    _listType - the type of list, e.g. budb_volumeList
 *    _dataType - the ITL data type for this entry, e.g. 
 *                itlBak_volumeEntryDataType
 *    _lenField - name of the length field for the list, 
 *                e.g. budb_volumeList_len
 *    _listField - name of the list field for the list, 
 *                 e.g. budb_volumeList_val
 *    _subscript - the index into the list array
 *    _newVar - a pointer to a var_val_t, the new variable is returned here
 */
#define MAKE_TYPE_VARIABLE(_lp, _listType, _dataType, _lenField, _listField, _subscript, _newVar)                                                           \
        MACRO_BEGIN                                                          \
          var_val_t  *mtv_newVarP;                                           \
          _listType *mtv_listP;                                              \
                                                                             \
          mtv_listP = (_listType *) _lp;                                     \
          if ((_subscript >= 0) && (_subscript < mtv_listP->_lenField))      \
          {                                                                  \
              mtv_newVarP = itlValue_New((pointer_t)                         \
				     &(mtv_listP->_listField[_subscript]),   \
				     _dataType);                             \
              _newVar = itlValue_Copy(mtv_newVarP);                          \
	      ITL_FREE(mtv_newVarP);                                         \
          }                                                                  \
          else                                                               \
              itl_Error("Index, %d, for %s, is invalid.",                    \
			_subscript, UTIL_STRING(_listField));                \
        MACRO_END


int itlBak_listDataType;


PRIVATE var_val_t *ListCopy _TAKES((
				    IN var_val_t *copyVarP
				    ));

PRIVATE var_val_t *ListNew _TAKES((
				   IN pointer_t newValP,
				   IN int newType
				   ));

PRIVATE boolean_t ListFree _TAKES((
				   IN var_val_t *freeVarP
				   ));

PRIVATE boolean_t ListBinaryOp _TAKES((
				       IN var_val_t *leftOpP,
				       IN var_val_t *rightOpP,
				       IN char *binaryOp
				       ));

PRIVATE char *ListDisplay _TAKES((
				  IN var_val_t *displayVarP,
				  OUT int *lengthP
				  ));

PRIVATE var_val_t *ListGetField _TAKES((
					IN var_val_t *getVarP,
					IN char *fieldName
					));

PRIVATE itlValue_varTypeInfo_t listVarType = {
    "fs_bakList",
    ListCopy,
    ListNew,
    ListFree,
    NULL,
    ListBinaryOp,
    NULL,
    NULL,
    NULL,
    ListDisplay,
    ListGetField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

/*
 *  ListInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void ListInit()
{
    /*
     *  We can safely set tapeSetHashType because this function is called
     *  via pthread_once.
     */
    itlBak_listDataType = itlValue_RegisterVarType(&listVarType);
}

/*
 * itlBak_ListInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlBak_ListInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) ListInit))
	itl_Error("pthread_once failed in ListInit\n");
}


PRIVATE var_val_t *ListCopy(copyVarP)
  IN var_val_t *copyVarP;
{
    bakList_t *listValP = (bakList_t *) VALUE_VAL(copyVarP);
    bakList_t *newListP;

    assert(VALUE_TYPE(copyVarP) == itlBak_listDataType);

    ITL_NEW(newListP, bakList_t);
    newListP->listType = listValP->listType;
    switch (newListP->listType) {

      case bakVolumeList:
	COPY_LIST(newListP->listP, listValP->listP, budb_volumeList,
		  budb_volumeList_len, budb_volumeList_val, budb_volumeEntry);
	break;
      case bakDumpList:
	COPY_LIST(newListP->listP, listValP->listP, budb_dumpList,
		  budb_dumpList_len, budb_dumpList_val, budb_dumpEntry);
	break;
      case bakTapeList:
	COPY_LIST(newListP->listP, listValP->listP, budb_tapeList,
		  budb_tapeList_len, budb_tapeList_val, budb_tapeEntry);
	break;
      default:
	itl_Error("Invalid bak list type in copy, %d\n", newListP->listType);
	break;
    }
    
    return(ListNew((pointer_t) newListP, VALUE_TYPE(copyVarP)));
}

PRIVATE var_val_t *ListNew(newValP, newType)
  IN pointer_t newValP;
  IN int newType;
{
    var_val_t *newHeaderVarP;

    assert(newType == itlBak_listDataType);

    ITL_NEW(newHeaderVarP, var_val_t);
    VALUE_TYPE(newHeaderVarP) = newType;
    VALUE_SIZE(newHeaderVarP) = sizeof(bakList_t);
    VALUE_VAL(newHeaderVarP) = newValP;
    return(newHeaderVarP);
}

PRIVATE boolean_t ListFree(freeVarP)
  IN var_val_t *freeVarP;
{
    bakList_t *listP = (bakList_t *) VALUE_VAL(freeVarP);

    ITL_FREE(listP->listP);
    ITL_FREE(VALUE_VAL(freeVarP));
    return(FALSE);
}

/*
 *  ListBinaryOp() --
 *    Just compares two bak lists.
 *
 *  WARNING:
 *    The return value (boolean) indicates if the operation requested is
 *    valid.  Oddly TRUE means it is not a valid operation and FALSE 
 *    implies it is a valid operation.  Its says nothing about the success 
 *    of the comparison.
 *    The value of leftOpP indicates if the operation succeeded.
 */
PRIVATE boolean_t ListBinaryOp(leftOpP, rightOpP, binaryOp)
  IN var_val_t *leftOpP;
  IN var_val_t *rightOpP;
  IN char *binaryOp;
{
    boolean_t validOp = FALSE, opSucceeded = FALSE, areEqual;
    bakList_t *leftValP, *rightValP;
    var_val_t *opResultP;

    leftValP = (bakList_t *) VALUE_VAL(leftOpP);
    rightValP = (bakList_t *) VALUE_VAL(rightOpP);

    if (leftValP->listType == rightValP->listType)
    {
	switch (leftValP->listType) {
	  case bakVolumeList:
	    COMPARE_LIST(leftValP->listP, rightValP->listP, budb_volumeList,
			 budb_volumeList_len, budb_volumeList_val, 
			 itlBak_VolumeEntryCompare, areEqual);
	    break;
	  case bakDumpList:
	    COMPARE_LIST(leftValP->listP, rightValP->listP, budb_dumpList,
			 budb_dumpList_len, budb_dumpList_val, 
			 itlBak_DumpEntryCompare, areEqual);
	    break;
	  case bakTapeList:
	    COMPARE_LIST(leftValP->listP, rightValP->listP, budb_tapeList,
			 budb_tapeList_len, budb_tapeList_val, 
			 itlBak_TapeEntryCompare, areEqual);
	    break;
	  default:
	    itl_Error("Invalid bak list type in binary op, %d\n", 
		      leftValP->listType);
	    validOp = TRUE;
	    break;
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

	      default :
		  itl_Error("Binary operation, %s, NOT supported on bak lists.\n",
			    binaryOp);
	      validOp = TRUE;
	  }
    }
    else
    {
	opSucceeded = FALSE;
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

PRIVATE char *ListDisplay(displayVarP, lengthP)
  IN var_val_t *displayVarP;
  OUT int *lengthP;
{
    itl_Error("Sorry, I don't know how to display bak list.\nYou can display each entry separately\n");

    return(NULL);
}

PRIVATE var_val_t *ListGetField(getValP, fieldName)
  IN var_val_t *getValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    bakList_t *listValP;

    listValP = (bakList_t *) VALUE_VAL(getValP);

    /*
     *  Determine if we need to get the length field or one of the
     *  entries from the val list array.
     */
    if (strstr(fieldName, "len"))
    {
	/*
	 *  Accessing a len field.
	 */
	switch (listValP->listType) {
	  case bakVolumeList:
	    retValP = itlStds_IntVal((int) ((budb_volumeList *) listValP->listP)->budb_volumeList_len);
	    break;
	  case bakDumpList:
	    retValP = itlStds_IntVal((int) ((budb_dumpList *) listValP->listP)->budb_dumpList_len);
	    break;
	  case bakTapeList:
	    retValP = itlStds_IntVal((int) ((budb_tapeList *) listValP->listP)->budb_tapeList_len);
	    break;
	  default:
	    itl_Error("Invalid bak list type in get field, %d\n", 
		      listValP->listType);
	    break;
	}
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

	int index;
	var_val_t *newVarP;

	if (itlBak_CommonGetArrayIndex(fieldName, &index))
	{
	    switch (listValP->listType) {

	      case bakVolumeList:
		MAKE_TYPE_VARIABLE(listValP->listP, budb_volumeList, 
				   itlBak_volumeEntryDataType, 
				   budb_volumeList_len, budb_volumeList_val,
				   index, retValP);
		break;

	      case bakDumpList:
		MAKE_TYPE_VARIABLE(listValP->listP, budb_dumpList, 
				   itlBak_dumpEntryDataType, 
				   budb_dumpList_len, budb_dumpList_val,
				   index, retValP);
		break;
	      case bakTapeList:
		MAKE_TYPE_VARIABLE(listValP->listP, budb_tapeList, 
				   itlBak_tapeEntryDataType, 
				   budb_tapeList_len, budb_tapeList_val,
				   index, retValP);
		break;

	      default:
		itl_Error("Invalid bak list type in get field, %d\n", 
			  listValP->listType);
		break;
	    }
	}
	else
	    itl_Error("Invalid bak list access.  Can only access list entries, not the entries subfields\n");
    }

    return(retValP);
}

/*
 *  itlBak_ListCreate() --
 *    Create a variable to contain one of the bak server lists.  Note
 *    that this routine copies the data sent to it, so that the caller,
 *    can free the memory if it was dynamically allocated.
 */
EXPORT void itlBak_ListCreate(listType, varName, listP)
  IN itlBak_listTypes_t listType;
  IN char *varName;
  IN pointer_t listP;
{
    bakList_t *newVarP;

    ITL_NEW(newVarP, bakList_t);
    newVarP->listType = listType;
    newVarP->listP = listP;
    /*
     *  The ITLU_ASSIGN_VARIABLE macro will make a copy of the data.  It
     *  ends up calling our ListCopy() function.
     */
    ITLU_ASSIGN_VARIABLE(varName, newVarP, itlBak_listDataType);
    ITL_FREE(newVarP);
}

