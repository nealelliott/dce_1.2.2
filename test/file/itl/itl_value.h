/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl_value.h,v 1.1.414.1 1996/10/17 18:27:55 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
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
 * $Log: itl_value.h,v $
 * Revision 1.1.414.1  1996/10/17  18:27:55  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:02  damon]
 *
 * Revision 1.1.409.1  1994/02/04  20:45:51  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:46  devsrc]
 * 
 * Revision 1.1.407.1  1993/12/07  17:45:00  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:31:06  jaffe]
 * 
 * Revision 1.1.2.5  1993/01/11  18:31:43  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:11:38  htf]
 * 
 * Revision 1.1.2.4  1992/11/18  22:00:56  jaffe
 * 	Transarc delta: fred-itl-ofer-fixes-10-30 1.1
 * 	  Selected comments:
 * 	    Incorporates Ofer's changes that permit procedures to have parameters
 * 	    passed to them, and also includes the vector and valList data types.  The
 * 	    vector data type is needed for DFS testing.  It allows us to manipulate
 * 	    binary data.
 * 	    Added itlValue_ConvertValue().
 * 	[1992/11/18  15:38:27  jaffe]
 * 
 * Revision 1.1.2.3  1992/10/28  17:58:50  jaffe
 * 	Transarc delta: fred-itl-dir-stat-mem-cmds 1.2
 * 	  Selected comments:
 * 	    Adding support for directory functions, stat function, and memory mapped
 * 	    files.
 * 	    Not done yet, just upgrading.
 * 	    This complets this delta.
 * 	    Fixed bug found by Ofer in TP code.  Function interface change.
 * 	[1992/10/28  12:49:52  jaffe]
 * 
 * 	$TALog: itl_value.h,v $
 * 	Revision 1.12  1994/11/01  22:30:17  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.11 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.11  1993/02/02  15:43:54  fred
 * 	[merge of changes from 1.8 to 1.10 into 1.9]
 * 
 * Revision 1.10  1993/01/19  21:13:00  fred
 * 	To add the pack/unpack capabilities to ITL variables so that we can do
 * 	better distributed testing.  Also added some other ITL bug fixes from Ofer.
 * 
 * 	Not yet complete.  Need to fix the FX tests.
 * 
 * 	Added pack/unpack functions.
 * 	[from r1.8 by delta fred-ot6848-add-pack-unpack-capability, r1.1]
 * 
 * Revision 1.9  1993/01/14  17:38:51  jaffe
 * 	revdrop of latest OSF copyright changes to these files
 * 	[from r1.8 by delta osf-revdrop-01-12-92, r1.1]
 * 
 * 	Revision 1.7  1992/11/10  19:49:33  jaffe
 * 	Sync up with the OSF source as of Nov 9.  This delta fixes mostly test
 * 	code problems related to missing OSF copyrights.
 * 	[from revision 1.4 by delta osf-sync-with-nov-9-drop, r1.1]
 * 
 * Revision 1.5  1992/11/03  15:27:51  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 
 * 	Added itlValue_ConvertValue().
 * 	[from revision 1.4 by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.4  1992/10/05  16:32:23  jdp
 * 	[merge of changes from 1.1 to 1.3 into 1.2]
 * 
 * Revision 1.3  1992/10/02  20:32:16  jaffe
 * 	Sync with OSF sources.
 * 
 * 	Added correct OSF headers.
 * 	[from revision 1.1 by delta osf-cleanup-from-sept-25-92-drop-test, r1.1]
 * 
 * Revision 1.2  1992/09/10  11:51:06  fred
 * 	This complets this delta.
 * 
 * 	Fixed bug found by Ofer in TP code.  Function interface change.
 * 	[from revision 1.1 by delta fred-itl-dir-stat-mem-cmds, revision 1.2]
 * 
 * Revision 1.1  1992/08/10  20:11:37  fred
 * 	Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	Initial checkin.
 * 
 * 	Initial checkin.
 * 	[added by delta fred-checkin-itl, revision 1.1]
 * 
 * Revision 1.5  1992/04/27  21:02:07  oz
 * 	Add a display upcall for long form display of variable types.
 * 
 * 	itlValue_ToString is now called itlValue_PrintString
 * 	New functions:
 * 	itlValue_DisplayValue
 * 	itlValue_Display
 * 	[from revision 1.4 by delta oz-2495-ITL:Add-display-upcall-to-var-types, revision 1.1]
 * 
 * Revision 1.4  1992/04/20  23:55:49  oz
 * 	Fix prototype for itlValue_VarValConvert
 * 
 * 	Fix prototype for itlValue_VarValConvert
 * 	(it was called itlValue_VarValueConvert)
 * 	[from revision 1.3 by delta oz-2428-prototype-for-itlValue_VarValConvert, revision 1.1]
 * 
 * Revision 1.3  1992/04/10  19:40:31  oz
 * 	More corrections
 * 
 * 	Added itlValue_New
 * 	[from revision 1.2 by delta oz:2334-Allow-access-to-var-upcalls, revision 1.4]
 * 
 * Revision 1.2  1992/04/10  18:55:59  oz
 * 	Continued changes:
 * 
 * 	The upcal structure has an additional member: new
 * 	Added itlValue_VarValConvert
 * 	[from revision 1.1 by delta oz:2334-Allow-access-to-var-upcalls, revision 1.2]
 * 
 * Revision 1.1  1992/04/10  03:12:11  oz
 * 	The ITL user can define her own variable types.
 * 	This delta colocates all variable type information in one module
 * 	called itl_value.[ch]
 * 	  In addition, when the client defines and registers variable types
 * 	the client provides upcall ITL uses to manipulate the type.  The new
 * 	module allows the user access to these upcalls, providing greated
 * 	flexibility in defining new, and recursive, types.
 * 
 * 	[added by delta oz:2334-Allow-access-to-var-upcalls, revision 1.1]
 * 
 * $EndLog$
 */

#ifndef TRANSACRC_ITL_VALUES_H
#define TRANSACRC_ITL_VALUES_H 1

/*
 *  This module allows the user to register variable types and allows
 *  the user access to all itl known types.
 *
 *  When the user registers a type, the user specifies several upcalls
 *  that itl uses in order to manipulate these types.
 *  This module provides wrappers the user can call to access these
 *  upcalls, for the user's types or for ITL native types.
 *
 *  To get a more detailed explanation of each of the functions,
 *  see the description of the upcalls below.
 */

/*
 *  itlExp_varTypeInfo_t -
 *	Information necessary to define a new type.
 *
 *	name: The name of the new type (e.g., string) - used for printing.
 *	copy: A function used to copy values
 *		  var_val_t *copy(valP)
 *		  var_val_t *valP;
 *		It takes one parameter of type var_val_t * -
 *		It returns a dynamically allocated var_val_t which
 *		contains a copy of the value.
 *	new:  Given a pointer_t and a type, create a var_val_t of this
 *	      type whose value is the given pointer.
 *	free: A routine called to free a values of this type.
 *		It takes one parameter: pointer_t.
 *	read: A function that reads one such value (for the set command).
 *		It takes two parameters, a value (possibly NULL) and
 *		the type code, and returns a pointer to a var_val_t.
 *		The value, if not null, is the first value the read function
 *		should use.  It may be a value a prior read function read
 *		and is passing on to this one to use (for
 *		recursive structures).
 *	binaryOp: A routine that is called to perform binary operations
 *		on values of this type.
 *		  binaryOp(leftVal, rightVal, Op)
 *			INOUT var_val_t *leftVal
 *			IN var_val_t *rightVal
 *			IN char *Op
 *		  Op is a string representing the operation (.e.g, "==")
 *		  leftVal is a pointer to the value on the left side of
 *		  of the operator.  The function binaryOp may do what
 *		  it pleases with either value, nothing is assumed about
 *		  the rightVal upon return.
 *		  Upon return leftVal should contain the result.
 *	unaryOp: A routine called to perform unary operators on values of
 *		this type:
 *		Form:  unaryOp(valP, op)
 *			INOUT var_val_t *valP
 *			IN char *op
 *		Upon return, valP should contain the result of the operation.
 *
 *	convert: A routine called to convert one type to another.
 *		Form:  convert(valP, type)
 *		   INOUT var_val_t *valP
 *		   IN int type
 *	        Convert the value to the specified type
 *		Upon return, valP will point to the dynamically allocated
 *		new value (of the specified type).
 *		Even if the type is integer, you must dynamically allocate
 *		space for the integer and make valP point to the integer.
 *		The type boolean is meant to convert the type for boolean
 *		tests.
 *		For example, if you implement strings, you may decide to
 *		convert string to integers by calling strtol but when you check
 *		strings in boolean expressions you may want to know whether
 *		the string is the null string or not.
 *
 *		This function returns FALSE on success and TRUE otherwise.
 *	printString: A routine that is called in order to convert the type to
 *		a string for printing.  You may want to format it differently 
 *		here than you would when simply converting it to a string.
 *		BUT, consider the ramifications if you do it.
 *		form:   char *printString(valP, lenP)
 *				IN var_val_t *valP;
 *				OUT int *lenP;
 *	getField: A routine that is called on complex structures to return
 *		a specific structure member.  The structure may be stored
 *		internally in any way.  This function must return a var_val_t
 *		of the appropriate type.
 *		This function returns NULL if the given field is not specified
 *		for the value.
 *		form:	var_val_t *getField(valP, fieldName)
 *				IN var_val_t *valP;
 *				IN char *fieldName;
 *
 *	setField: Set on field of a structure.
 *		return TRUE if all is well and the field was set.
 *		return FALSE if the field is not defined for the value
 *	        or if the fieldVal is not valid.
 *		form:	boolean_t setField(valP, fieldName, fieldValP)
 *				IN var_val_t *valP;
 *				IN char *fieldName;
 *				IN var_val_t *fieldValP;
 *
 *	pack: Pack the value to contiguous bytes in a form that can be
 *		saved on disk or in a file and unpacked later.
 *		This can be used for marshalling and unmarshalling as well.
 *		form:	pointer_t *pack(valP, bufP)
 *				IN  var_val_t *valP;
 *				OUT int *lenP;
 *		The function dynamically allocates (using ITL_ALLOC)
 *		a buffer for the packed record and returns a pointer to the
 *		new buffer.  The length of the buffer is returned in the
 *		out parameter lenP.  The caller is responsible for freeing
 *		the buffer using ITL_FREE.
 *
 *		If the user does not specify a pack function, ITL assumes
 *		that the type is a simple contiguous type.  It also assumes
 *		that the size of the type is VALUE_SIZE(valP) and does
 *		the packing itself.
 *
 *		The function returns NULL on error (invalid type).
 *
 *	unpack: unpack a variable value that was packed using the pack upcall
 *		described above.
 *		It returns a dynamically allocated var_val_t.
 *		form:	var_val_t *unpack(bufP, len)
 *				IN pointer_t bufP;
 *				IN int len;
 *				IN int type;
 *
 *		If the user does not specify an unpack function, the default
 *		unpack function is used.  By default, ITL assumes that the
 *		type is flat.
 *
 *	shortHelp: A short help describing the type and providing the list
 *		of fields if any.
 *
 *	help:	A more extensive help message.
 */

typedef struct {
    char *name;
    var_val_t *(*copy) _TAKES((var_val_t *));
    var_val_t *(*new) _TAKES((pointer_t, int));
    boolean_t  (*free) _TAKES((var_val_t *));
    var_val_t *(*read) _TAKES((var_val_t *, int));
    boolean_t  (*binaryOp) _TAKES((var_val_t *, var_val_t *, char *));
    boolean_t  (*unaryOp) _TAKES((var_val_t *, char *));
    boolean_t  (*convert) _TAKES((var_val_t *, int));
    char      *(*printString) _TAKES((var_val_t *, int *));
    char      *(*displayValue) _TAKES((var_val_t *, int *));
    var_val_t *(*getField) _TAKES((var_val_t *, char *));
    boolean_t  (*setField) _TAKES((var_val_t *, char *, var_val_t *));
    pointer_t  (*pack)     _TAKES((IN var_val_t *, OUT  int *));
    var_val_t *(*unpack)   _TAKES((IN pointer_t, IN int len, IN int type));
    char *shortHelp;
    char *help;
} itlValue_varTypeInfo_t;



/*
 *  itlValue_RegisterVarType
 *	Register a new variable type with itl.
 *
 *  Parameters:
 *	varInfo: Information about the new type
 *  Return Code: the integer code for the new type.
 *	A return code of -1 means there was an error and the type was
 *	not registered.
 */
extern int itlValue_RegisterVarType _TAKES((
  IN itlValue_varTypeInfo_t *infoP
));

/*
 *  itlValue_Free
 *	Free an arbitrary type.
 *	This frees the value field, but not the var_val_t structure.
 */
extern void itlValue_Free _TAKES((
  IN var_val_t *varValP
));

/*
 *  Copy a var value.
 */
extern var_val_t *itlValue_Copy _TAKES((
  IN var_val_t *inVal
));

/*
 *  Return a new var val of the given type with the given value
 */
extern var_val_t *itlValue_New _TAKES((
  IN pointer_t ptr,
  IN int type
));

/*
 *  Convert a var value to a string (short form)
 */
extern char *itlValue_PrintString _TAKES((
  IN	var_val_t *valP,
  OUT	int *lenP
));
#define itlValue_ToString itlValue_PrintString

/*
 *  Convert a var value to a string (long form)
 */
extern char *itlValue_DisplayValue _TAKES((
  IN	var_val_t *valP,
  OUT	int *lenP
));

/*
 *  Convert a var value to a string:
 *  If the user defined a DisplayValue upcall (long form), call it
 *  Otherwise call the short form (printString).
 */
extern char *itlValue_Display _TAKES((
  IN	var_val_t *valP,
  OUT	int *lenP
));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlValue_VarValConvert
 *     Convert a variable value of one type to another.
 *     The value is changed in place modifying the input value.
 *  The function also returns a pointer to the value for convenience.
 *
 *  When this function is called with a value of type 'A' and asked
 *  to convert it to a value of type 'B', it first calls the
 *  conversion function of the type 'A' type.  If this fails, it
 *  calls the conversion function of the type 'B' type, if this also
 *  fails it prints an error message.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern var_val_t *itlValue_VarValConvert _TAKES((
   INOUT var_val_t *value,
   IN int type
));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlValue_ConvertValue
 *     Convert a variable value of one type to another.
 *     The value is changed in place modifying the input value.
 *  The function also returns a pointer to the value for convenience.
 *  This is similart to itlValue_ConvertValue except that it does not
 *  report any errors via itl_Error. 
 * 
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern var_val_t *itlValue_ConvertValue _TAKES((
   INOUT var_val_t *value,
   IN int type
));


/*
 *  itlValue_TypeIsValid
 *	Is the provided type a valid ITL type?
 *	TRUE if it is, FALSE otherwise.
 */
extern boolean_t itlValue_TypeIsValid _TAKES((
  IN int type
));

/*
 * itlValue_Type --  Convert a variable type to its ASCII string name.
 * 
 */
extern char *itlValue_Type _TAKES((
  IN int varType
));

/*
 *  Call the convert function of the type provided to convert the
 *  provided value to that type.
 *
 *  Parameters:
 *	valType: The type to which the value should be converted.
 *	calledType: The function to be called to make that converstion.
 *		    This can either be the function registered for the
 *		    the current value or for the desired value.
 */
extern boolean_t itlValue_Convert _TAKES((
  INOUT var_val_t *value,
  IN int valType,
  IN int calledType
));


/*
 *	Return the type code of a type given by name
 *	If the type is not a recognized type, return -1.
 */
extern int itlValue_TypeByName _TAKES((
  IN char *varType
));

/*
 *  itlValue_Read
 *	Given a type code (int), call the appropriate routine
 *	to read a value of that type.
 */
extern var_val_t *itlValue_Read _TAKES((
  IN var_val_t *valP,
  IN int type
));

/*
 *  Perform binary ops on values
 */
extern boolean_t itlValue_BinaryOp _TAKES((
  INOUT var_val_t *leftVal,
  IN var_val_t *rightVal,
  IN char *Op
));

/*
 *  Perform a Unary Operation on a value.
 */
extern boolean_t itlValue_UnaryOp _TAKES((
  IN var_val_t *inVal,
  IN char *unaryOp
));

/*
 *  Set a field of a value.
 */
extern boolean_t itlValue_SetField _TAKES((
  IN var_val_t *inVal,
  IN char *fieldName,
  IN var_val_t *fieldVal
));

/*
 *  Get a field of a value.
 */  
extern var_val_t *itlValue_GetField _TAKES((
  IN var_val_t *inVal,
  IN char *fieldName
));

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
extern pointer_t itlValue_Pack _TAKES((
  IN  var_val_t *inVal,
  OUT int *lenP
));

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
extern var_val_t *itlValue_Unpack _TAKES((
  IN  pointer_t bufP,
  IN  int len,
  IN  int type
));


/*
 *  itlValue_Help
 *	Return the help message for this type.
 *	This returns a pointer to the help message string.  It does not
 *	copy it.  Therefore, the caller should not free it.
 *	The function return NULL if the type is invalid.
 */
SHARED char *itlValue_Help _TAKES(( IN int type ));

/*
 *  itlValue_ShortHelp
 *	Return the short help message for this type.
 *	This returns a pointer to the message string.  It does not
 *	copy it.  Therefore, the caller should not free it.
 *	The function return NULL if the type is invalid.
 */
SHARED char *itlValue_ShortHelp _TAKES(( IN int type));


/*
 *  Initialize the itl-value module
 */
extern void itlValue_Init _TAKES((void));

#endif /*  TRANSACRC_ITL_VALUES_H  */
