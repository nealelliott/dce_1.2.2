/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_utils.h,v $
 * Revision 1.1.417.1  1996/10/17  18:28:53  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:31  damon]
 *
 * Revision 1.1.412.1  1994/02/04  20:46:37  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:07  devsrc]
 * 
 * Revision 1.1.410.1  1993/12/07  17:45:41  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:33:46  jaffe]
 * 
 * Revision 1.1.2.5  1993/01/11  18:32:28  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:13:36  htf]
 * 
 * Revision 1.1.2.4  1992/12/09  21:33:08  jaffe
 * 	Transarc delta: fred-ot6132-check-for-short-io 1.1
 * 	  Selected comments:
 * 	    To fix the read and write commands to check for short I/O.  Previously the
 * 	    user had to to this in the ITL script by looking at the CommandResult
 * 	    variable.  This option is still available if the user sets verifyStatus to
 * 	    false.
 * 	    Moved the CALLOC macro from here to itl.h and added declaration for
 * 	    itlu_IsVerifyTrue().
 * 	Transarc delta: fred-ot6158-make-assign-copy 1.1
 * 	  Selected comments:
 * 	    To fix failure of ITLU_ASSIGN_VARIABLE macro.  It did not copy the data
 * 	    sent to it and could lead to memory corruption.
 * 	    Added call to itlValue_Copy to the ITLU_ASSIGN_VARIABLE macro.
 * 	[1992/12/07  13:36:42  jaffe]
 * 
 * Revision 1.1.2.3  1992/11/18  22:02:37  jaffe
 * 	Transarc delta: jess-ot6043-itl-assign-var 1.2
 * 	  Selected comments:
 * 	    This delta move ASSIGN_ITL_VARIABLE macro into the itl/utils/itl_utils.h
 * 	    file so that it can be referenced from itl subcomponents.
 * 	    Change all invokation names of the macro to new name.
 * 	    Fix pmax compiling warnings.
 * 	[1992/11/18  16:36:33  jaffe]
 * 
 * Revision 1.1.2.2  1992/10/28  17:59:50  jaffe
 * 	Transarc delta: fred-itl-add-fcntl-flock 1.3
 * 	  Selected comments:
 * 	    To add fcntl and flock to the ITL file stuff.
 * 	    Also will fix some minor bugs, and convert to use Jeff Prem's utilities.
 * 	    This is a checkpoint.  I need to import a delta from Jeff.
 * 	    No changes yet.
 * 	    Complete the integration of Jeff's utility stuff and finish up work on
 * 	    fcntl and flock.
 * 	    Interface change to CheckResult.
 * 	    Forgot to add the files that implement the file locking stuff. What
 * 	    a dufus.
 * 	Transarc delta: jdp-add-itl-stuff 1.8
 * 	  Selected comments:
 * 	    Add code for three new itl libraries: libitlUtils.a, libitlAggr.a, and
 * 	    libitlFset.a
 * 	    Interface definition for libitlUtils.a
 * 	    Update error message in itlu_CheckReturnCode() so that the offending command
 * 	    is printed.
 * 	    Change signature of itlu_CheckReturnCode() to include command name.
 * 	    Added documentation for libitlUtils.a, fixed some bugs, and reorganized.
 * 	    Added documentation.
 * 	    Changed interface and implementation of result checking routines.
 * 	    Change name and usage of the variable used to control result checking.
 * 	    Provide routine for converting a value back to a name, assuming that
 * 	    the value--name mapping was constructed using itlu_ProcessValueMap().
 * 	    Provide itlu_ValueToName().
 * 	    Checkpointing in order to import another delta.
 * 	    Fix misc. compilation problems and close inorder to open another delta.
 * 	    Make this stuff compile on the pmax.
 * 	[1992/10/28  12:55:54  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1992,1993 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/utils/itl_utils.h,v 1.1.417.1 1996/10/17 18:28:53 damon Exp $
 *
 */

#ifndef	_TRANSARC_ITL_UTIL_H
#define	_TRANSARC_ITL_UTIL_H

#include <utils/itl/itl.h>
#include <utils/itl/itl_value.h>

/*
 * The ITL utilities library provide extensions and miscellaneous utilities 
 * for use with ITL.  Since ITL maintenance is not under our control, the
 * utilities are contained in a separate header file and library.
 *
 * The routines exported by this library depart from the standard ITL
 * practice of returning boolean return codes; instead, 0 is returned on
 * success, and a real error code (either an errno or dce error code) is
 * returned on failure.
 *
 * This package reserves the namespace defined by all symbols starting with
 * "itlu_", "ITLU_", "_itlu_", or "_ITLU_".
 */


/*
 *  ITLU_ASSIGN_VARIABLE() --
 *    Assigns a value to an ITL variable.  The code does the following.
 *      - The call to itlValue_New builds a new var_val_t.
 *      - The call to itl_SetVariable will do one of the 
 *        following.
 *          1)  Makes a copy of the variable, so caller allocated memory
 *              can be free'd when this macro returns.
 *          2)  If the variable does not exist, it will create
 *              it.
 *          3)  If the variable exists, it will free it's old
 *              value and replace it with the new one.
 *
 *  Agrugments:
 *    _newVarName - name of the variable.
 *    _newVarData - pointer to the data that is to be stored in the variable.
 *    _newVarType - ITL type for the variable.
 */
#define ITLU_ASSIGN_VARIABLE(_newVarName, _newVarData, _newVarType)          \
        MACRO_BEGIN                                                          \
	  var_val_t *iav_newVarP;                                            \
                                                                             \
	  iav_newVarP = itlValue_New((pointer_t) (_newVarData),              \
				     (_newVarType));                         \
	  if (!itl_SetVariable((_newVarName), iav_newVarP, var_local))       \
            itl_Error("Failed to set variable, %s\n", (_newVarName));        \
	  ITL_FREE(iav_newVarP);                                             \
        MACRO_END

/*
 *  The ITL_SET_INT_FIELD and ITL_SET_STRING_FIELD macros are intended
 *  to be used in the SetField functions of ITL data types.  This is why
 *  they accept a boolean variable and set it to TRUE but not to FALSE.  It's
 *  expected that the calling function defaults that value to FALSE.
 *
 *  ITLU_SET_INT_FIELD - Arguments
 *    _itlValP - the ITL variable which contains the integer value to assign.
 *    _setData - the field whose value is being set.
 *    _dataType - the type of the _setData field.  This is here because
 *                many types that are not integers use the integer type,
 *                e.g. pointers, unsigned32, etc.
 *    _boolVal - a boolean variable.  It will be set to TRUE if the 
 *               assignment is successful.  It will NOT be set to FALSE
 *               if the assignment fails.
 *
 *  ITLU_SET_STRING_FIELD - Arguments
 *    _itlValP - the ITL variable which contains the string value to assign.
 *    _copyToP - pointer to where the string in _itlValP is to be copied to
 *    _mSize   - maximum length of the string.  This includes the 
 *               end-of-string character, since it is included in the size
 *               of ITL strings given by the VALUE_SIZE macro.
 *    _boolVal - a boolean variable.  It will be set to TRUE if the 
 *               assignment is successful.  It will NOT be set to FALSE
 *               if the assignment fails.
 */
#define ITLU_SET_INT_FIELD(_itlValP, _setData, _dataType, _boolVal)       \
        if (VALUE_TYPE(_itlValP) == itl_intType)                          \
        {                                                                 \
          _setData = *((_dataType *) VALUE_VAL(_itlValP));                \
	  _boolVal = TRUE;                                                \
	}

#define ITLU_SET_STRING_FIELD(_itlValP, _copyToP, _mSize, _boolVal)       \
        if ((VALUE_TYPE(_itlValP) == itl_stringType) &&                   \
	    (VALUE_SIZE(_itlValP) <= (_mSize)))                           \
        {                                                                 \
          strcpy((char *) (_copyToP), (char *) VALUE_VAL(_itlValP));      \
	  _boolVal = TRUE;                                                \
	}


/*
 * Most ITL routines simply return a success/failure boolean value, but we
 * usually want to return a more meaningful error code from our routines.
 * In the case of a failed ITL call, we have to make something up, so we
 * return ITLU_UNKNOWN_ERROR.  The following function, itlu_ErrorMessage
 * treats this as a special value and returns an appropriate string
 * representation.
 */
#define ITLU_UNKNOWN_ERROR	(-1L)

/*
 *  itlu_ErrorMessage() --
 *    This routine returns an error message for the given code.  It handles
 *    both DCE and UNIX (errno) errors.
 *
 *  NOTE:
 *    This function allocates memory for the errorMesgPP parameter.  This
 *    must be free'd by the caller via the itlu_ErrorMessageFree() function.
 */
IMPORT void itlu_ErrorMessage _TAKES((
				      long errorCode,
				      char **errorMesgPP
				      ));

/*
 *  itlu_ErrorMessageFree() --
 *    Free the buffer allocated from a call to itlu_ErrorMessage()
 */
IMPORT void itlu_ErrorMessageFree _TAKES((
					  char *errorMesgP
					  ));


/*
 *  itlu_DCEError() --
 *    Reports a DCE error to the user.  Using this keeps the format of
 *    DCE error messages consistent across ITL libraries.
 */
IMPORT void itlu_DCEError _TAKES((
				   IN unsigned long error,
				   IN char *errorMessage
				   ));


/*
 * itlu_Init()
 *	This routine initializes the utility package and should
 *	be called once, after initializing ITL but before entering
 *	the ITL command loop.  See "src/test/file/fset/fset_test.c" for
 *	an example.
 *
 * PARAMETERS:
 *	none
 */
IMPORT long itlu_Init _TAKES((void));


/*
 * The following type and function are used to define batches of ITL
 * variables, in one, fell swoop.
 *
 * This facility is used to define ITL variables for errnos and other
 * error codes, but is generally useful for other purposes.
 */
typedef struct itlu_valueMap {
    char*	name;
    long	value;
} itlu_valueMap_t;

/*
 * itlu_ProcessValueMap()
 *	This routine takes an array of value map structures, and for each
 *	element, defines an ITL variable of name `name' with value `value'.
 *	It also defines an ITL array variable that can be used to invert
 *	the mapping.  Given an inverse variable named "X", $X[2] would
 *	give the name associated with value 2.
 *
 * PARAMETERS:
 *	IN valueMap
 *		An array of itl_valueMap structures that is terminated by
 *		an element whose `name' member is NULL.
 *	IN inverseVarName
 *		A character string whose value will be used as the name of
 *		an ITL array variable where each entry maps from a value
 *		to the associated name.  Due to the implementation of
 *		ITL arrays, the inverse map is sparsely allocated.
 *		If this variable is NULL, it is ignored, and no inverse
 *		map is defined.
 */
IMPORT long itlu_ProcessValueMap _TAKES((
    IN itlu_valueMap_t*	valueMap,
    IN char*		inverseVarName
));

/*
 * itlu_ValueToName()
 *	Given the name of an inverse variable used in a call to
 *	itlu_ProcessValueMap(), this routine converts an integer value to
 *	its corresponding name.
 *
 * PARAMETERS:
 *	IN value
 *		A value that will be mapped to a name.
 *	IN inverseVarName
 *		The name of the variable (previously passed as an argument to
 *		itlu_ProcessValueMap()) that holds the inverse mapping.
 *	OUT *nameP
 *		On return, *nameP points to a null-terminated string
 *		that holds the name associated with `value'.
 */
IMPORT long itlu_ValueToName _TAKES((
    IN long	value,
    IN char*	inverseVarName,
    OUT char**	nameP
));


/*
 * itlu_DefineSystemErrorCodes()
 *	This routine defines an ITL variable for each of the system error
 *	codes (errnos).  For example, after this routine is called, the ITL
 *	variable $ENOENT has the value 2.
 *
 *	It is generally not necessary to call this routine directly, since
 *	it is called by itlu_Init().
 *
 * PARAMETERS:
 *	none
 */
IMPORT long itlu_DefineSystemErrorCodes _TAKES((void));

/*
 * itlu_ExtendKnownErrorCodes()
 *	This routine allows other ITL packages to extend the set of known
 *	error codes with more interesting values (ie. DCE error codes defined
 *	by some package's error table).  The inverse map named by
 *	`ITLU_ERROR_CODE_TO_NAME_VAR' is extended to include the new values.
 *	See "src/test/file/itl/aggr/itl_aggr.c" for an example of this
 *	routine's intended usage.
 *
 * PARAMETERS:
 *	IN errorCodeMap
 *		An array of itl_valueMap structures that is terminated by
 *		an element whose `name' member is NULL.
 */
IMPORT long itlu_ExtendKnownErrorCodes _TAKES((
    IN itlu_valueMap_t*	errorCodeMap
));

/*
 * The name of the ITL array variable that maps from error code values
 * back to error names.  For example, after a call to
 * itlu_DefineSystemErrorCodes(), $itlu_error_code_to_name[2] will have
 * the value "ENOENT".  This is mostly used internally by the utility
 * package to convert error codes to more meaningfull names.
 */
#define ITLU_ERROR_CODE_TO_NAME_VAR	"itlu_error_code_to_name"


/*
 * The following section defines types and routines that are used to extend
 * the ITL command table.  In particular, by using the utilities below, the
 * programmer can define command arguments as well as the commands themselves
 * in a table-driven manner.  When observing the amount of duplicated code
 * that exists in previous ITL code simply to do argument processing, the
 * need for such a facility became evident.
 *
 * We accomplish this extension by defining a command table entry format,
 * `itlu_commandTable_t', that is similar to the corresponding ITL type,
 * `itl_commandTable_t', but with some important additions.  In
 * particular, one member is an array of "argument descriptors" whose
 * elements describe the arguments to the associated command.  After
 * defining the appropriate argument descriptors and an extended command
 * table, the programmer uses itlu_Cnvt2StdCmdTbl() to convert the extended
 * command table to one that itl_AddModuleCmds() will understand.  Enough
 * magic goes on behind the scenes so that by the time an ITL command's
 * internal function is called, all of its arguments have been parsed and
 * are passed to it as an array of `opaque' values.  This frees the programmer
 * from having to do argument parsing in each command function.  See
 * "src/test/file/itl/fset/itl_fset.c" for an example of this technique.
 *
 * Using this facility also provides the programmer with a consistent error
 * code verification scheme.  By default, if any command function returns a
 * non-zero exit status, an error message is printed and the script is
 * terminated.  The programmer can, however, use the verifyStatus command
 * to false, causing non-zero return codes to be
 * ignored.  The return code is still recored in the ITL variable
 * CHECK_RESULT_VAR_NAME, allowing assertions in the code to check for expected
 * results.  The default behavior can be restored by issuing the 
 * verifyStatus command with a value of true.
 */

/*
 * itlu_ParsingFunc_t
 * 	This type specifies the signature required of all argument parsing
 *	functions.  Function pointers of this type are used in the `parser'
 *	member of the argument descriptor structure defined below.  If
 *	an error occurs while parsing, a non-zero code should be returned.
 *
 *	The programmer is encouraged to write their own general purpose 
 *	parsing routines.  See the definition of itlFset_GetFsetId() in
 *	"src/test/file/itl/fset/itl_fset.c" for an example.
 *
 * PARAMETERS:
 *	OUT *resultP
 *		On success, this parameter contains the parsing routine's
 *		result.
 *	OUT *shouldFreeP
 *		The parsing function should set this parameter to TRUE or
 *		FALSE depending on whether or not the result needs to be
 *		freed (via ITL_UTIL_FREE()) after calling the command
 *		function.
 */
typedef long (*itlu_parsingFunc_t) _TAKES((
    OUT opaque*		resultP,
    OUT boolean_t*	shouldFreeP
));

/*
 * The following two functions are generic parsing routines that return
 * an integer or a string, respectively.  They are appropriate for use
 * as the `parser' member of argument descriptor structures.
 */
IMPORT long itlu_GetInteger _TAKES((
    OUT opaque*		resultP,
    OUT boolean_t*	shouldFreeP
));

IMPORT long itlu_GetString _TAKES((
    OUT opaque*		resultP,
    OUT boolean_t*	shouldFreeP
));

/*
 * itlu_argDesc_t
 *	This type describes an entry in an argument descriptor table.
 *	An array of this type (or NULL if command takes no arguments) is
 *	given for each entry in an extended command table, one argument
 *	descriptor table for each command.
 */
typedef struct itlu_argDesc {
    char*		name;		/* The argument's name */
    char*		prompt;		/* The interactive prompt for this */
					/* argument */
    itlu_parsingFunc_t	parser;		/* A parsing function for this */
					/* argument */
} itlu_argDesc_t;

/*
 * itlu_cmdFunc_t
 *	This type specifies the signature required of all command functions.
 *	Function pointers of this type are used in the `function' member
 *	of the extended command table structure defined below.
 *
 * PARAMETERS:
 *	IN rock
 *		This argument is passed in (uninterpreted) from the `rock'
 *		member of the extended command table structure defined
 *		below.
 *	IN argv
 *		This argument is an array of opaque data with one entry
 *		for each argument passed to this command.  It assumed
 *		that the command function knows both the number of arguments
 *		and their true types.  The arguments' ordering is the same as 
 *		is given in this command's argument descriptor array.
 */
typedef long (*itlu_cmdFunc_t) _TAKES((
    IN opaque	rock,
    IN opaque	argv[]
));

/*
 * itlu_commandTable_t
 *	This type describes an entry in an extended command table.  An array
 *	of this type is passed to itlu_Cnvt2StdCmdTbl() to construct a
 *	standard ITL command table that can be used as an argument to
 *	itl_AddModuleCmds().
 */
typedef struct itlu_commandTable {
    char*		command;	/* The command name */
    itlu_cmdFunc_t	function;	/* The command function */
    opaque		rock;		/* Arbitrary user data */
    itlu_argDesc_t*	argDesc;	/* Array of argument descriptrs, */
					/* terminated by an entry whose */
					/* `name' member is NULL */
    boolean_t		breakCom;	/* TRUE if this command causes */
					/* the script to terminate */
    char*		description;	/* A textual description of this */
					/* command */
} itlu_commandTable_t;

/*
 * itlu_Cnvt2StdCmdTbl()
 *	This routine converts an extended command table to one that
 *	itl_AddModuleCmds will understand.  The standard command table
 *	is dynamically allocated, but should not normally need to be freed
 *	since its members are referenced throughout the ITL session.
 *
 * PARAMETERS:
 *	IN utilCmdTable
 *		An array of itlu_commandTable structures, one for each
 *		command.
 *	IN mode
 *		The value of this variable is used as the `mode' member
 *		for each of the standard command table entries.  See the ITL
 *		documentation for more information on this member.
 *	IN class
 *		The value of this variable is used as the `mode' member
 *		for each of the standard command table entries.  See the ITL
 *		documentation for more information on this member.
 *	IN *stdCmdTableP
 *		On success, this argument holds an array of itl_commandTable
 *		structures that can be passed to itl_AddModuleCmds().
 *	OUT *numEntriesP
 *		On success, this argument holds the number of entries in
 *		`*stdCmdTableP'.
 */
IMPORT long itlu_Cnvt2StdCmdTbl _TAKES((
    IN itlu_commandTable_t*	utilCmdTable,
    IN unsigned long		mode,
    IN char*			class,
    OUT itl_commandTable_t**	stdCmdTableP,
    OUT unsigned*		numEntriesP
));


/*
 *  itlu_TraceDumpOnError()
 *    This routine defines whether an ICL trace dump will be generated 
 *    when an error occurs.
 *
 *  PARAMETERS:
 *    boolean_t doDump - TRUE, then do a dump, else do not.
 *    char *dumpFile   - full pathname of the dumpfile.  A process id
 *                       will be added to it.  It is ignored if doDump
 *                       is FALSE.
 *
 *  RETURN CODES:
 *    FALSE - success
 *    TRUE  - failure
 */
extern boolean_t itlu_TraceDumpOnError _TAKES((
					       IN boolean_t doDump,
					       IN char *dumpFile
					       ));


/*
 * itlu_CheckResult()
 *	This routine checks the given return code, and if the error code is
 *	non-zero and error code verification is turned on, an error message
 *	is printed and the test is terminated.  If either the error code is
 *	zero or error code verification is turned off, the variable
 *	CHECK_RESULT_VAR_NAME is set to the value of code.  See the 
 *      discussion above on turning error code verification on and off via 
 *      verifyStatus.
 *
 *	This routine is called automatically for commands that were setup using
 *	the extended command table interface.  It is provided here only for
 *	commands that are not setup as part of an extended command table.
 *
 * PARAMETERS:
 *	IN code
 *		The command's return value.
 */
IMPORT boolean_t itlu_CheckResult _TAKES((
    IN long	code
));


/*
 * itlu_VerifyStatus()
 *      Sets the CHECK_RESULT_VAR_NAME variable to be eithher true or false.
 *      It prompts the user for this info.
 */
extern boolean_t itlu_VerifyStatus _TAKES((
     void
));

/*
 *  itlu_IsVerifyTrue() --
 *    Returns the value of the variable used for verifying if status checking
 *    is to occur.
 */
extern boolean_t itlu_IsVerifyTrue _TAKES((
					   void
					   ));

/*
 * itlu_System()
 *	This routine provides a fork-exec interface.
 *
 * PARAMETERS:
 *	IN command
 *		The name of the command to be executed.
 *	IN argv
 *		Arguments (null-terminated) to command.
 */
IMPORT long itlu_System _TAKES((
    IN char*	command,
    IN char*	argv[]
));

#endif	/* _TRANSARC_ITL_UTIL_H */
