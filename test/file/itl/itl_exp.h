/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl_exp.h,v 1.1.419.1 1996/10/17 18:27:15 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
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
 * $Log: itl_exp.h,v $
 * Revision 1.1.419.1  1996/10/17  18:27:15  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:40  damon]
 *
 * Revision 1.1.414.1  1994/02/04  20:45:16  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:28  devsrc]
 * 
 * Revision 1.1.412.1  1993/12/07  17:44:33  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:26:11  jaffe]
 * 
 * Revision 1.1.2.5  1993/01/11  18:30:50  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:09:38  htf]
 * 
 * Revision 1.1.2.4  1992/11/18  21:59:48  jaffe
 * 	Transarc delta: fred-itl-ofer-fixes-10-30 1.1
 * 	  Selected comments:
 * 	    Incorporates Ofer's changes that permit procedures to have parameters
 * 	    passed to them, and also includes the vector and valList data types.  The
 * 	    vector data type is needed for DFS testing.  It allows us to manipulate
 * 	    binary data.
 * 	    Changes for procedure parameter passing.
 * 	[1992/11/18  15:03:02  jaffe]
 * 
 * Revision 1.1.2.3  1992/10/28  17:49:50  jaffe
 * 	Transarc delta: fred-itl-ofer-fixes-9-22-92 1.2
 * 	  Selected comments:
 * 	    Incorporate fixes made to the TP version of ITL.  Some of them are performance
 * 	    related.  Others concerned the printing of error messages.  In most cases
 * 	    messages are no longer printed by support functions.  The determination of
 * 	    whether a message should be printed is now left to functions that call these
 * 	    support functions.
 * 	    Defined new function itlExp_ShowVariables().
 * 	    Fix (picky) compiler warnings
 * 	[1992/10/28  12:29:49  jaffe]
 * 
 * 	$TALog: itl_exp.h,v $
 * 	Revision 1.11  1994/11/01  22:29:31  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.10 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.10  1993/03/08  13:51:53  fred
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
 * 	[from r1.9 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 * 
 * Revision 1.9  1993/01/14  17:38:12  jaffe
 * 	revdrop of latest OSF copyright changes to these files
 * 	[from r1.8 by delta osf-revdrop-01-12-92, r1.1]
 * 
 * 	Revision 1.7  1992/11/10  19:49:08  jaffe
 * 	Sync up with the OSF source as of Nov 9.  This delta fixes mostly test
 * 	code problems related to missing OSF copyrights.
 * 	[from revision 1.4 by delta osf-sync-with-nov-9-drop, r1.1]
 * 
 * Revision 1.5  1992/11/03  15:27:32  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 
 * 	Changes for procedure parameter passing.
 * 	[from revision 1.4 by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.4  1992/10/05  16:28:50  jdp
 * 	[merge of changes from 1.1 to 1.3 into 1.2]
 * 
 * Revision 1.3  1992/10/02  20:31:53  jaffe
 * 	Sync with OSF sources.
 * 
 * 	Added correct OSF headers.
 * 	[from revision 1.1 by delta osf-cleanup-from-sept-25-92-drop-test, r1.1]
 * 
 * Revision 1.2  1992/09/23  15:53:40  fred
 * 	Incorporate fixes made to the TP version of ITL.  Some of them are performance
 * 	related.  Others concerned the printing of error messages.  In most cases
 * 	messages are no longer printed by support functions.  The determination of
 * 	whether a message should be printed is now left to functions that call these
 * 	support functions.
 * 
 * 	Defined new function itlExp_ShowVariables().
 * 	[from revision 1.1 by delta fred-itl-ofer-fixes-9-22-92, revision 1.1]
 * 
 * Revision 1.1  1992/08/10  20:10:46  fred
 * 	Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	Initial checkin.
 * 
 * 	Initial checkin.
 * 	[added by delta fred-checkin-itl, revision 1.1]
 * 
 * Revision 1.15  1992/04/10  18:55:56  oz
 * 	Continued changes:
 * 
 * 	Removed:
 * 	itlExp_VarValueConvert
 * 	itlExp_ReadSimpleTypes
 * 	[from revision 1.14 by delta oz:2334-Allow-access-to-var-upcalls, revision 1.2]
 * 
 * Revision 1.14  1992/04/10  03:12:07  oz
 * 	The ITL user can define her own variable types.
 * 	This delta colocates all variable type information in one module
 * 	called itl_value.[ch]
 * 	  In addition, when the client defines and registers variable types
 * 	the client provides upcall ITL uses to manipulate the type.  The new
 * 	module allows the user access to these upcalls, providing greated
 * 	flexibility in defining new, and recursive, types.
 * 
 * 	Moved itlExp_varTypeInfo_t to itl_value.h and renamed
 * 	itlValue_varTypeInfo_t
 * 	Removed some declarations which are no longer needed.
 * 	[from revision 1.13 by delta oz:2334-Allow-access-to-var-upcalls, revision 1.1]
 * 
 * Revision 1.13  1992/04/03  22:13:45  oz
 * 	  Allow type conversions both ways:
 * 
 * 	  Added comments to itlExp_VarValueConvert
 * 	[from revision 1.12 by delta oz-2228:Add-access-to-recursive-structures, revision 1.3]
 * 
 * Revision 1.12  1992/04/02  14:49:05  oz
 * 	  Continued developlment on the access to recursive access.
 * 	  fetch&Op works properly now.
 * 	  test&Set still doesn't
 * 
 * 	  New functions:
 * 	itlExp_SetNestedField
 * 	itlExp_GetNestedField
 * 	[from revision 1.11 by delta oz-2228:Add-access-to-recursive-structures, revision 1.2]
 * 
 * Revision 1.11  1992/04/01  20:53:09  oz
 * 	ITL allows the user to define her own variable types.  These may be nested
 * 	structures.  This delta allows the user to access data imbedded in such
 * 	structures and to modify single fields in an existing structure.
 * 
 * 	  Added two functions to varType specification: GetField and SetField
 * 	  New functions:
 * 	itlExp_GetValueField
 * 	itlExp_SetValueField
 * 	[from revision 1.10 by delta oz-2228:Add-access-to-recursive-structures, revision 1.1]
 * 
 * Revision 1.10  1992/03/27  21:15:45  oz
 * 	Changes which were made before 1.0.1 and were not supposed to be
 * 	incorporated in 1.0.1 and therefore were not readied and were
 * 	not moved to CML with the rest of the file.
 * 
 * 	These changes add tranC support for ITL as well as handling
 * 	transaction aborts (exceptions) in the lower level io modules.
 * 
 * 	See General Comment Above
 * 	[from revision 1.9 by delta oz-2172-itl-support-tranc, revision 1.1]
 * 
 * Revision 1.7  1992/02/07  11:23:20  oz
 * 	 - var_t is now itl_var_t
 * 	[92/02/06  13:12:45  oz]
 * 
 * Revision 1.6  92/02/02  09:22:21  oz
 * 	 - itlExp_SetParentVarList now takes a pointer to the var list
 * 	[92/01/31  10:45:53  oz]
 * 
 * Revision 1.5  92/01/17  10:02:14  oz
 * 	- renamed doFetchAndOp   ==>  itlExp_FetchAndOp
 * 	  and moved to here from itl.h
 * 	- Defined itlExp_TestAndSet
 * 	[92/01/17  10:00:40  oz]
 * 
 * Revision 1.4  91/12/20  10:43:09  oz
 * 	 - changed var_t to itl_var_t in varList_t
 * 
 * Revision 1.3  91/12/12  13:55:29  oz
 * 	 - New functions:
 * 		itlExp_TypeByName
 * 		itlExp_GetValueByType
 * 		itlExp_SetVarValue
 * 	[91/12/10  19:47:55  oz]
 * 
 * Revision 1.2  91/11/21  15:32:59  oz
 * 	Initial Checkin
 * 	[91/11/11  16:37:02  oz]
 * 
 * $EndLog$
 */
/*
 * itl_exp.h -- 
 *    The routines for handling expressions and variables.
 *
 */
#ifndef TRANSARC_ITL_EXP_H
#define TRANSARC_ITL_EXP_H 1

extern boolean_t itl_InitExp _TAKES((
				     void
				     ));

/*
 *  itlStds_IntVal
 *	Given an int value, return an int var-val
 */
extern var_val_t *itlStds_IntVal _TAKES((IN  int value));


/*
 *  itlStds_StringVal
 *	Given a string, create a string var-val (copying the
 *	input string to a locally allocated space).
 */
extern var_val_t *itlStds_StringVal _TAKES((IN  char *str));


/*
 *  itlStds_ProcVal
 *	Create a proc var-val (copying the input data to locally
 *	allocated space).
 */
extern var_val_t *itlStds_ProcVal _TAKES((IN  char  *str,
					  IN  int    numParams,
					  IN  int   *types,
					  IN  char **names));

/*
 *  itlExp_PushLocalVariables
 *	Declare local variables for a procedure.
 *	numVars: the number of variables.
 *	valListPP - an array of pointers to the values for the variables.
 *	varNames - an array of names for the variables.
 *
 *	ITL maintains a stack of context for procedures.  When a procedure
 *	begins it initializes its context by pushing new local variables
 *	onto this stack.  When the procedure terminates, it pops the
 *	values off the stack by calling itlExp_PopLocalVariables
 */
extern void itlExp_PushLocalVariables _TAKES((
  IN	int numVars,
  IN	var_val_t **valListPP,
  IN	char **varNames
));

/*
 *  itlExp_PopLocalVariables
 *	see explanation for itlExp_PushLocalVariables
 */
extern void itlExp_PopLocalVariables _TAKES(( void ));

/*
 *	Return the executable string of the procedure value.
 */
extern char *itlStds_ProcString _TAKES((IN var_val_t *valP));


/*
 *  itlExp_FreeArbitraryType
 *	Free the value part of a var_val_t
 */
extern void itlExp_FreeArbitraryType _TAKES(( IN var_val_t *varValP));

/*
 *   itlExp_UseStandardTypeDef()
 *	Use standard definitions for the standard types
 */
extern boolean_t itlExp_UseStandardTypeDef _TAKES(( void ));

/*
 *  itlExp_SetPermitted
 *	Is a set permitted to this variable.
 *	If the variable is a global variable we may not allow
 *	modifications to the variable not in the main thread.
 *	This is a function of itl_EnforceScopeForSet.
 */
extern boolean_t itlExp_SetPermitted _TAKES((
   IN	itl_var_t *varP,
   IN	scope_t scope
));

/*
 *  itlExp_GetValueField
 *     Return one field of a variable value which is defined
 *     as a complex structure.
 *
 *  Parameters:
 *	inVal - The input value (var_val_t * type)
 *	name - The (string) name of the field.
 *
 *  The string name (field definition) is a name of a field defined
 *  for the given value.  It is not a nested definition.  To obtain
 *  a deeper field (a nested field), use itlExp_GetNestedField.
 *
 *  This function returns a dynamically allocated value that must
 *  be freed by the caller
 */
extern var_val_t *itlExp_GetValueField _TAKES((
  IN var_val_t *inVal,
  IN char *fieldName
));

/*
 *  itlExp_GetNestedField
 *     Return one field of a variable value which is defined
 *     as a complex structure.
 *
 *  Parameters:
 *	inVal - The input value (var_val_t * type)
 *	name - The (string) name of the field.
 *
 *  The string name (field definition) is a dot separated list of
 *  fields to allow access to deeply nested fields.
 *
 *  This function returns a dynamically allocated value that must
 *  be freed by the caller
 */
extern var_val_t *itlExp_GetNestedField _TAKES((
  IN var_val_t *inVal,
  IN char *fieldName
));

/*
 *  itlExp_SetValueField
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
 *
 *  The field specified must be valid for the given value.
 *  In order to modify a deeply nested field, use the function
 *  itlExp_SetNestedField.
 */
extern boolean_t itlExp_SetValueField _TAKES((
  IN var_val_t *inVal,
  IN char *fieldName,
  IN var_val_t *fieldVal
));

/*
 *  itlExp_SetNestedField
 *     Set a nested field of a variable which is defined as a
 *     complex structure.
 *
 *  Parameters:
 *	inVal - The input value (var_val_t * type)
 *	name - The (string) name of the field.
 *	fieldVal - The new value for the field.
 *
 *  This function returns TRUE on success and FALSE if either the field
 *  is not a valid field or if the value is not the correct value for the
 *  field.
 *
 *  The field specified is a dot separated list of fields.
 */
extern boolean_t itlExp_SetNestedField _TAKES((
  IN var_val_t *inVal,
  IN char *fieldName,
  IN var_val_t *fieldVal
));

/*
 *  itlExp_SetVarValue -
 *	Set a variable to an arbitrary value.
 */
extern itl_var_t *itlExp_SetVarValue _TAKES((
					     IN char *name,
					     IN int type,
					     IN int size,
					     IN pointer_t value,
					     IN scope_t scope
					     ));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlExp_FetchAndOp-
 *	Perform a fetch and op operation on a script variable.
 *
 *	If the variable is currently global, it is made local.
 *	The current global value of the variable is copied into its local
 *	value and the operation is performed on the global value
 *	all in one operation
 */
extern itl_var_t *itlExp_FetchAndOp _TAKES((IN char *name,
					IN var_val_t *value,
					IN char *op));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlExp_TestAndSet
 *	Perform a test-and-set operation on a script variable.
 *
 *	The current global value of the variable is examined:
 *	The described operation is performed on it.  If the result
 *	is TRUE, the value newValue is set to the global value of the
 *	variable and the function call returns TRUE.
 *	Otherwise, the global value of the variable is unchanged
 *	and the function call returns FALSE.
 */
extern boolean_t itlExp_TestAndSet _TAKES((IN char *name,
					   IN var_val_t *value,
					   IN char *op,
					   IN var_val_t *newValue));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  varList_t:
 *	A list of variables
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
typedef struct {
    char **nameList;	/*  The list of var Names  	 */
    var_val_t **valList; /*  The list of variable Values  */
    int listLen;	/*  The number of variables in the list  */
    int listSize;	/*  The size allocated for the list  */
    int allocInc;  /*  The size to increase the list allocation when needed */
    boolean_t local;    /* Are these the local values or the global  */
    boolean_t copied;   /* vars copied: Are the variable values copied
			 * or pointed to.  This effects the free function
			 */
} varList_t;



/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlExp_CollectAllVars
 *	Collect all the current vairables into a dynamically
 *	allocated list.  The list contains a pointer to the variables
 *	name and pointer to the variables value.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern varList_t itlExp_CollectAllVars _TAKES((IN boolean_t local,
					       IN boolean_t copy));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlExp_FreeVarList
 *	Free the list of variables collected by a
 *	call to itlExp_CollectAllVars(...)
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itlExp_FreeVarList _TAKES((IN varList_t *listP));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlExp_SetParentVarList
 *	Set the thread's parent var list to the given list.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itlExp_SetParentVarList _TAKES((IN varList_t *list));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlExp_ParentVarList
 *	Return the parent variable list for the current thread.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern varList_t *itlExp_ParentVarList _TAKES(());

/*
 *  itlExp_ShowVariables
 *	Display all the defined variables.
 *	If matchStr is not NULL, display only those variables which
 *	contain the string matchStr in them
 */
SHARED void itlExp_ShowVariables _TAKES((IN char *matchStr));


#endif  /*  TRANSARC_ITL_EXP_H  */
