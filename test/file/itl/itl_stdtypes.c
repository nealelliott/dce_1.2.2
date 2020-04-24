/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl_stdtypes.c,v 1.1.110.1 1996/10/17 18:27:42 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    EXPORT   itlExp_UseStandardTypeDef
 *    
 *    PRIVATE  FreeSimpleType, SimpleTypeConvert, StringShift, StringCmp, 
 *             EvalBinaryStrOp, EvalBinaryIntOp, EvalUnaryIntOp, 
 *             EvalUnaryStringOp
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
 * $Log: itl_stdtypes.c,v $
 * Revision 1.1.110.1  1996/10/17  18:27:42  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:54  damon]
 *
 * Revision 1.1.105.1  1994/02/04  20:45:41  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:42  devsrc]
 * 
 * Revision 1.1.103.1  1993/12/07  17:44:53  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:29:52  jaffe]
 * 
 * Revision 1.1.2.5  1993/01/11  18:31:29  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:11:02  htf]
 * 
 * Revision 1.1.2.4  1992/11/18  22:00:35  jaffe
 * 	Transarc delta: fred-itl-ofer-fixes-10-30 1.1
 * 	  Selected comments:
 * 	    Incorporates Ofer's changes that permit procedures to have parameters
 * 	    passed to them, and also includes the vector and valList data types.  The
 * 	    vector data type is needed for DFS testing.  It allows us to manipulate
 * 	    binary data.
 * 	    Changes for procedure parameter passing.
 * 	[1992/11/18  15:34:43  jaffe]
 * 
 * Revision 1.1.2.3  1992/10/28  17:58:34  jaffe
 * 	Transarc delta: fix-itl-string-compare 1.1
 * 	  Selected comments:
 * 	    This delta fixes the string comparison problem.
 * 	    Thanks Fred and Ofer.
 * 	    See above.
 * 	Transarc delta: fred-itl-ofer-fixes-9-22-92 1.2
 * 	  Selected comments:
 * 	    Incorporate fixes made to the TP version of ITL.  Some of them are performance
 * 	    related.  Others concerned the printing of error messages.  In most cases
 * 	    messages are no longer printed by support functions.  The determination of
 * 	    whether a message should be printed is now left to functions that call these
 * 	    support functions.
 * 	    Removed error messages.
 * 	    Added the mod operator (%) for strings.  It truncates the string.
 * 	    Fix (picky) compiler warnings
 * 	[1992/10/28  12:36:16  jaffe]
 * 
 * 	$TALog: itl_stdtypes.c,v $
 * 	Revision 1.27  1994/11/01  22:30:02  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.25 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.25  1993/06/03  16:51:08  fred
 * 	[merge of changes from 1.19 to 1.22 into 1.24]
 * 
 * Revision 1.22  1993/04/21  19:12:49  srinivas
 * 	changes for solaris
 * 
 * 	changes for solaris
 * 	[from r1.19 by delta srinivas-itl-1, r1.1]
 * 
 * Revision 1.24  1993/06/03  15:33:58  fred
 * 	[merge of changes from 1.22 to 1.23 into 1.19]
 * 
 * Revision 1.23  1993/06/03  12:50:14  fred
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
 * 	[from r1.22 by delta fred-db3612-port-itl-to-solaris, r1.1]
 * 
 * Revision 1.19  1993/03/08  13:52:02  fred
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
 * 	[from r1.18 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 * 
 * Revision 1.18  1993/02/02  15:43:37  fred
 * 	[merge of changes from 1.14 to 1.17 into 1.16]
 * 
 * Revision 1.17  1993/01/19  21:12:56  fred
 * 	To add the pack/unpack capabilities to ITL variables so that we can do
 * 	better distributed testing.  Also added some other ITL bug fixes from Ofer.
 * 
 * 	Not yet complete.  Need to fix the FX tests.
 * 
 * 	Added pack/unpack interfaces to datatype.
 * 	[from r1.14 by delta fred-ot6848-add-pack-unpack-capability, r1.1]
 * 
 * Revision 1.16  1993/01/14  20:06:17  jaffe
 * 	[merge of changes from 1.12 to 1.15 into 1.14]
 * 
 * Revision 1.15  1993/01/14  17:38:41  jaffe
 * 	revdrop of latest OSF copyright changes to these files
 * 	[from r1.12 by delta osf-revdrop-01-12-92, r1.1]
 * 
 * 	Revision 1.10  1992/11/10  19:49:26  jaffe
 * 	Sync up with the OSF source as of Nov 9.  This delta fixes mostly test
 * 	code problems related to missing OSF copyrights.
 * 	[from revision 1.7 by delta osf-sync-with-nov-9-drop, r1.1]
 * 
 * Revision 1.8  1992/11/03  15:27:42  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 
 * 	Changes for procedure parameter passing.
 * 	[from revision 1.7 by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.7  1992/10/12  18:59:24  mcinerny
 * 	[merge of changes from 1.3 to 1.6 into 1.5]
 * 
 * Revision 1.6  1992/10/06  18:47:09  jess
 * 	This delta fixes the string comparison problem.
 * 	Thanks Fred and Ofer.
 * 
 * 	See above.
 * 	[from revision 1.3 by delta fix-itl-string-compare, r1.1]
 * 
 * Revision 1.5  1992/10/05  16:29:53  jdp
 * 	[merge of changes from 1.2 to 1.4 into 1.3]
 * 
 * Revision 1.4  1992/10/02  20:32:12  jaffe
 * 	Sync with OSF sources.
 * 
 * 	Added correct OSF headers.
 * 	[from revision 1.2 by delta osf-cleanup-from-sept-25-92-drop-test, r1.1]
 * 
 * Revision 1.3  1992/09/23  15:53:42  fred
 * 	Incorporate fixes made to the TP version of ITL.  Some of them are performance
 * 	related.  Others concerned the printing of error messages.  In most cases
 * 	messages are no longer printed by support functions.  The determination of
 * 	whether a message should be printed is now left to functions that call these
 * 	support functions.
 * 
 * 	Removed error messages.
 * 	Added the mod operator (%) for strings.  It truncates the string.
 * 	[from revision 1.2 by delta fred-itl-ofer-fixes-9-22-92, revision 1.1]
 * 
 * Revision 1.2  1992/08/20  19:37:48  fred
 * 	Manually imported bug fixes from the Encina ITL.  Got them from Ofer.
 * 	Also, I changed the ITL_FILE_CHECK_ERROR macro to fail when something
 * 	succeeds that was expected to fail.
 * 	Added the verifyFileError command to toggle file system function verification.
 * 	(The code was already there I just was not making it available.)
 * 
 * 	Ofer bug fixes.
 * 	[from revision 1.1 by delta fred-fix-itl-bugs-8-20-92, revision 1.1]
 * 
 * Revision 1.1  1992/08/10  20:11:28  fred
 * 	Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	Initial checkin.
 * 
 * 	Initial checkin.
 * 	[added by delta fred-checkin-itl, revision 1.1]
 * 
 * Revision 1.13  1992/06/29  16:04:16  oz
 * 	New Function: ReplicateString, used to replicate
 * 	strings in the string multiplication operator
 * 	[from revision 1.12 by delta oz-3342-itl-fix-string-multiplication-by-0, revision 1.3]
 * 
 * Revision 1.14  1992/12/10  19:22:36  fred
 * 	[merge of changes from 1.11 to 1.13 into 1.12]
 * 
 * Revision 1.13  1992/11/29  23:17:09  fred
 * 	To convert ITL to use the random() function instead of rand().
 * 	Also added the "error" command.
 * 
 * 	Changed calls to rand() to call random().
 * 	[from revision 1.11 by delta fred-ot6219-use-random-not-rand, r1.1]
 * 
 * Revision 1.12  1992/11/23  16:16:46  jaffe
 * 	Put in missing header after latest drop to the OSF.
 * 	[from revision 1.11 by delta osf-sync-with-nov-19-drop, r1.1]
 * 
 * 	Revision 1.10  1992/11/10  19:49:26  jaffe
 * 	Sync up with the OSF source as of Nov 9.  This delta fixes mostly test
 * 	code problems related to missing OSF copyrights.
 * 	[from revision 1.7 by delta osf-sync-with-nov-9-drop, r1.1]
 * 
 * Revision 1.8  1992/11/03  15:27:42  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 
 * 	Changes for procedure parameter passing.
 * 	[from revision 1.7 by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.7  1992/10/12  18:59:24  mcinerny
 * 	[merge of changes from 1.3 to 1.6 into 1.5]
 * 
 * Revision 1.6  1992/10/06  18:47:09  jess
 * 	This delta fixes the string comparison problem.
 * 	Thanks Fred and Ofer.
 * 
 * 	See above.
 * 	[from revision 1.3 by delta fix-itl-string-compare, r1.1]
 * 
 * Revision 1.5  1992/10/05  16:29:53  jdp
 * 	[merge of changes from 1.2 to 1.4 into 1.3]
 * 
 * Revision 1.4  1992/10/02  20:32:12  jaffe
 * 	Sync with OSF sources.
 * 
 * 	Added correct OSF headers.
 * 	[from revision 1.2 by delta osf-cleanup-from-sept-25-92-drop-test, r1.1]
 * 
 * Revision 1.3  1992/09/23  15:53:42  fred
 * 	Incorporate fixes made to the TP version of ITL.  Some of them are performance
 * 	related.  Others concerned the printing of error messages.  In most cases
 * 	messages are no longer printed by support functions.  The determination of
 * 	whether a message should be printed is now left to functions that call these
 * 	support functions.
 * 
 * 	Removed error messages.
 * 	Added the mod operator (%) for strings.  It truncates the string.
 * 	[from revision 1.2 by delta fred-itl-ofer-fixes-9-22-92, revision 1.1]
 * 
 * Revision 1.2  1992/08/20  19:37:48  fred
 * 	Manually imported bug fixes from the Encina ITL.  Got them from Ofer.
 * 	Also, I changed the ITL_FILE_CHECK_ERROR macro to fail when something
 * 	succeeds that was expected to fail.
 * 	Added the verifyFileError command to toggle file system function verification.
 * 	(The code was already there I just was not making it available.)
 * 
 * 	Ofer bug fixes.
 * 	[from revision 1.1 by delta fred-fix-itl-bugs-8-20-92, revision 1.1]
 * 
 * Revision 1.1  1992/08/10  20:11:28  fred
 * 	Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	Initial checkin.
 * 
 * 	Initial checkin.
 * 	[added by delta fred-checkin-itl, revision 1.1]
 * 
 * Revision 1.13  1992/06/29  16:04:16  oz
 * 	New Function: ReplicateString, used to replicate
 * 	strings in the string multiplication operator
 * 	[from revision 1.12 by delta oz-3342-itl-fix-string-multiplication-by-0, revision 1.3]
 * 
 * Revision 1.12  1992/06/26  21:40:49  oz
 * 	Do not copy string onto itself
 * 
 * 	[from revision 1.11 by delta oz-3342-itl-fix-string-multiplication-by-0, revision 1.2]
 * 
 * Revision 1.11  1992/06/25  16:04:57  oz
 * 	When a string was multiplied by 0, it was not
 * 	producing the correct result.
 * 
 * 	- Fixed EvalBinaryStrOp: It used to assume that the right value was
 * 	  bigger than 0 in a subtle way: It saved the first copy.
 * 	- FIXED.
 * 	[from revision 1.10 by delta oz-3342-itl-fix-string-multiplication-by-0, revision 1.1]
 * 
 * Revision 1.10  1992/04/27  21:02:00  oz
 * 	Add a display upcall for long form display of variable types.
 * 
 * 	Add NULL entry for the new upcall
 * 	[from revision 1.9 by delta oz-2495-ITL:Add-display-upcall-to-var-types, revision 1.1]
 * 
 * Revision 1.9  1992/04/10  18:55:58  oz
 * 	Continued changes:
 * 
 * 	No longer include itl_exp.h !!
 * 	New function: SimpleTypeNew
 * 	Read now takes two parameters.
 * 	ReadSimpleTypes was moved to here from itlExp
 * 	[from revision 1.8 by delta oz:2334-Allow-access-to-var-upcalls, revision 1.2]
 * 
 * Revision 1.8  1992/04/10  03:12:08  oz
 * 	The ITL user can define her own variable types.
 * 	This delta colocates all variable type information in one module
 * 	called itl_value.[ch]
 * 	  In addition, when the client defines and registers variable types
 * 	the client provides upcall ITL uses to manipulate the type.  The new
 * 	module allows the user access to these upcalls, providing greated
 * 	flexibility in defining new, and recursive, types.
 * 
 * 	Include itl_value.h
 * 	itlExp_RegisterVarType is now itlValue_RegisterVarType
 * 	itlExp_varTypeInfo_t is now itlValue_varTypeInfo_t
 * 	[from revision 1.7 by delta oz:2334-Allow-access-to-var-upcalls, revision 1.1]
 * 
 * Revision 1.7  1992/04/01  20:53:15  oz
 * 	ITL allows the user to define her own variable types.  These may be nested
 * 	structures.  This delta allows the user to access data imbedded in such
 * 	structures and to modify single fields in an existing structure.
 * 
 * 	  New functions:
 * 	GetFieldFromSimpleType
 * 	SetFieldOfSimpleType
 * 	  Modified the structures (two new function pointers)
 * 	[from revision 1.6 by delta oz-2228:Add-access-to-recursive-structures, revision 1.1]
 * 
 * Revision 1.6  1992/03/26  12:53:47  jaffe
 * 	Prologue Changes for 1.0.1
 * 
 * 	[from revision 1.5 by delta jaffe-ttools-prologue, revision 1.1]
 * 
 * Revision 1.5  1992/01/20  08:30:58  oz
 * 	 - using:
 * 		itl_Trace	instead of itl_TracePrintf
 * 		itl_Error	instead of itl_PrintErrMsg
 * 		itl_PromptUser	instead of itl_Prompt
 * 	[92/01/18  13:14:18  oz]
 * 
 * Revision 1.4  92/01/02  09:58:48  oz
 * 	 - Changed the unary string operator # to return the length
 * 	   of the string ant not the size of the value (off by one)
 * 	[91/12/26  09:51:39  oz]
 * 
 * Revision 1.3  91/12/12  13:55:58  oz
 * 	 - new function: ReadProcType
 * 	[91/12/10  19:48:56  oz]
 * 
 * Revision 1.2  91/11/21  15:33:47  oz
 * 	Initial Checkin
 * 	[91/11/11  16:33:46  oz]
 * 
 * $EndLog$
 */
/*
 * itl_stdtypes.c -- 
 *    The routines for defining ITL standard variable value types:
 *	itl_intType
 *	itl_stringType
 *	itl_procType
 *	itl_funcType
 *	itl_booleanType
 */

#ifdef	SUNOS5
#include <sys/types.h>
#endif
#include <assert.h>
#include <stdlib.h>
#include "itl_stds.h"
#include <utils/itl/itl_private.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_listtype.h>
#include <utils/itl/itl_vectype.h>

static char RCSid[] = "$Id: itl_stdtypes.c,v 1.1.110.1 1996/10/17 18:27:42 damon Exp $";

#define  OUT_PARAM_TYPE		-1
#define  PROC_LOCAL_TYPE	-2
#define  UNTYPED_PARAM		-3

PRIVATE int funcType;

/*
 *  Structure to store information about procedures:
 *	The procedure string
 *	The number of parameters.
 *	The type of parameters: -1 is untyped
 */
typedef struct {
    char  *proc;
    int    len;
    int    numParams;
    int   *types;
    int	   numLocals;
    char **names;
} itlProcType_t;

#define PROC_VALUE(valP) ((itlProcType_t *)VALUE_VAL(valP))

/*
 *  FreeSimpleType
 *	Free a value of type int, string, or proc
 */
PRIVATE boolean_t FreeSimpleType(varValP)
  IN var_val_t *varValP;
{
    if (VALUE_VAL(varValP))
	ITL_UTIL_FREE(VALUE_VAL(varValP));

    return(FALSE);
}

PRIVATE void FreeProcStructure _TAKES(( IN	itlProcType_t *procP ));
PRIVATE void FreeProcStructure(procP)
  IN	itlProcType_t *procP;
{
    int numVars = procP->numParams + procP->numLocals;
    if (numVars > 0) {
	int i;
	ITL_FREE(procP->types);
	for (i=0; i < numVars; i++) {
	    ITL_FREE(procP->names[i]);
	}
	ITL_FREE(procP->names);
    }
    ITL_FREE(procP);
}

PRIVATE boolean_t FreeProcType(varValP)
  IN var_val_t *varValP;
{
    if (!VALUE_VAL(varValP))
	return FALSE;
    if (VALUE_TYPE(varValP) == itl_procType) {
	itlProcType_t *procP = (itlProcType_t *)VALUE_VAL(varValP);
	ITL_FREE(procP->proc);
	FreeProcStructure(procP);
    }
    return(FALSE);
}

/*
 *  New simple types
 */
PRIVATE var_val_t *SimpleTypeNew _TAKES((
  IN  pointer_t ptr,
  IN int type
));
PRIVATE var_val_t *SimpleTypeNew(ptr, type)
  IN  pointer_t ptr;
  IN int type;
{
    var_val_t *newValue;
    ITL_NEW(newValue, var_val_t);
    VALUE_VAL(newValue) = ptr;
    VALUE_TYPE(newValue) = type;
    if ((type == itl_intType) || (type == itl_booleanType)) {
	VALUE_SIZE(newValue) = sizeof(int);
    } else if (type == itl_stringType) {
	VALUE_SIZE(newValue) = strlen((char *)ptr) + 1;
    } else if ((type == itl_procType) || (type == funcType)) {
	VALUE_SIZE(newValue) = PROC_VALUE(newValue)->len;
    } else {
	ITL_FREE(newValue);
	return(NULL);
    }
    return(newValue);
}

/*
 *  CopySimpleTypes
 *	Copy values of simple types
 */
PRIVATE var_val_t *CopySimpleTypes(inVal)
  IN var_val_t *inVal;
{
    var_val_t *outVal = itl_NewVarVal();

    VALUE_SIZE(outVal) = VALUE_SIZE(inVal);
    VALUE_TYPE(outVal) = VALUE_TYPE(inVal);
    ITL_UTIL_REALLOC(VALUE_VAL(outVal), 
		     pointer_t,
		     VALUE_SIZE(outVal));
    ITL_UTIL_MEMCOPY(VALUE_VAL(inVal),
		     VALUE_VAL(outVal),
		     VALUE_SIZE(outVal));
    return(outVal);
}


/*
 *  CopyProcTypes
 *	Copy values of simple types
 */
PRIVATE var_val_t *CopyProcTypes(inVal)
  IN var_val_t *inVal;
{
    itlProcType_t *procP = PROC_VALUE(inVal);
    itlProcType_t *newProc;
    int numVars;

    ITL_NEW(newProc, itlProcType_t);
    ITL_ALLOC(newProc->proc, char *, procP->len);

    ITL_MEMCOPY(procP->proc, newProc->proc, procP->len);
    newProc->len = procP->len;
    newProc->numParams = procP->numParams;
    newProc->numLocals = procP->numLocals;
    numVars = newProc->numLocals + newProc->numParams;
    if (numVars != 0) {
	int i;
	ITL_ALLOC(newProc->types, int *, procP->numParams * sizeof(int));
	ITL_ALLOC(newProc->names, char **, numVars * sizeof(char *));
	ITL_MEMCOPY(procP->types, newProc->types,
		    procP->numParams * sizeof(int));
	for (i=0; i<numVars; i++) {
	    int nameLen = strlen(procP->names[i]) + 1;
	    ITL_ALLOC(newProc->names[i], char *, nameLen);
	    ITL_MEMCOPY(procP->names[i], newProc->names[i], nameLen);
	}
    }
    return SimpleTypeNew((pointer_t)newProc, itl_procType);
}


/*
 *  ProcTypeToString
 *	Convert a procedure to a printable string
 */
PRIVATE char *ProcTypeToString(inVal, lenP)
  IN var_val_t *inVal;
  OUT	int    *lenP;
{
    itlProcType_t *procP = PROC_VALUE(inVal);
    char *retVal;
    int curLen;

    ITL_ALLOC(retVal, char *, 40);
    sprintf(retVal, "proc(%d params)", procP->numParams);
    curLen = strlen(retVal);
    if (procP->numParams == 0) {
	sprintf(&retVal[curLen], ".");
    } else {
	int i;
	sprintf(retVal + curLen, ": [");
	curLen += 3;

	for (i=0; i < procP->numParams; i++) {
	    char tempStr[1024];
	    int newLen;
	    if (procP->types[i] == OUT_PARAM_TYPE) {
		sprintf(tempStr, "%s ('out')", procP->names[i]);
	    } else if (procP->types[i] == UNTYPED_PARAM) {
		sprintf(tempStr, "%s ('untyped')", procP->names[i]);
	    } else {
		assert(procP->types[i] >= 0);
		sprintf(tempStr, "%s (%s)",
			procP->names[i], itlValue_Type(procP->types[i]));
	    }
	    newLen = strlen(tempStr);
	    ITL_REALLOC(retVal, char *, curLen + newLen + 3);
	    ITL_MEMCOPY(tempStr, retVal + curLen, newLen + 1);
	    curLen += newLen;
	    if (i == procP->numParams - 1) {
		sprintf(retVal + curLen, "].");
	    } else {
		sprintf(retVal + curLen, ", ");
	    }
	    curLen += 2;
	}
    }
    if (lenP)
	*lenP = strlen(retVal);
    return retVal;
}

/*
 *  ProcTypeDisplay
 *	Convert a procedure to a display string
 */
PRIVATE char *ProcTypeDisplay(inVal, lenP)
  IN var_val_t *inVal;
  IN int       *lenP;
{
    itlProcType_t *procP = PROC_VALUE(inVal);
    int curLen;
    char *retVal = ProcTypeToString(inVal, &curLen);

    ITL_REALLOC(retVal, char *, curLen + procP->len + 2);
    retVal[curLen] = '\n';
    curLen++;
    ITL_MEMCOPY((pointer_t)procP->proc, (pointer_t)(retVal + curLen),
		procP->len + 1);
    if (lenP)
	*lenP = curLen + procP->len;
    return retVal;
}

/*
 *  Convert an int type to a string.
 */
PRIVATE char * IntValueToString(valP, lenP)
  IN var_val_t *valP;
  OUT int *lenP;
{
    char *result;
    itl_Trace(ITL_VAR_TRACE,
	      "Printing value of type %d.\n", VALUE_TYPE(valP));

    ITL_UTIL_ALLOC(result, char *, VALUE_SIZE(valP) * 3 +1);
    sprintf(result, "%d", *(int *)VALUE_VAL(valP));
    if (lenP)
	*lenP = strlen(result);
    return(result);
}

/*
 *  Convert a string type to a printable string
 */
PRIVATE char * StrValueToString(valP, lenP)
  IN var_val_t *valP;
  OUT int *lenP;
{
    char *result;
    int len = VALUE_SIZE(valP)-1;
    ITL_UTIL_ALLOC(result, char *, len+1);
    ITL_UTIL_MEMCOPY(VALUE_VAL(valP), result, len+1);
    if (lenP)
	*lenP = len;
    return(result);
}

PRIVATE boolean_t SimpleTypeConvert(value, type)
  IN var_val_t *value;
  IN int type;
{
    if (VALUE_TYPE(value) != type) {
	if (type == itl_intType) {
	    if (VALUE_TYPE(value) == itl_booleanType) {
	    }
	    else if (VALUE_TYPE(value) == itl_stringType) {
		int newVal = strtol(value->value, NULL, 0);
		VALUE_SIZE(value) = sizeof(int);
		ITL_UTIL_REALLOC(VALUE_VAL(value), 
				 pointer_t,
				 VALUE_SIZE(value));
		INT_VALUE_VAL(value) = newVal;
	    }
	    else {
		return(TRUE);
	    }
	} else if (type == itl_booleanType) {
	    if (VALUE_TYPE(value) == itl_intType) {
	    }
	    else if (VALUE_TYPE(value) == itl_stringType) {
		int newVal = VALUE_VAL(value) && !*STR_VALUE_VAL(value);
		
		VALUE_SIZE(value) = sizeof(int);
		ITL_UTIL_REALLOC(VALUE_VAL(value), 
				 pointer_t,
				 VALUE_SIZE(value));
		INT_VALUE_VAL(value) = newVal;
	    }
	    else {
		return(TRUE);
	    }
	} else if (type == itl_stringType) {
	    int size;
	    char *newVal;
	    /*
	     *  The previous call returns the size NOT including 
	     *  the terminating NULL, add one to get the REAL size.
	     */
	    if (VALUE_TYPE(value) == itl_intType) {
		ITL_ALLOC(newVal, char *, 16);
		sprintf(newVal, "%d", INT_VALUE_VAL(value));
		size = strlen(newVal) + 1;
	    } else if (VALUE_TYPE(value) == itl_booleanType) {
		ITL_ALLOC(newVal, char *, 7);
		sprintf(newVal, "%s", INT_VALUE_VAL(value) ? "TRUE" : "FALSE");
		size = strlen(newVal) + 1;
	    } else if ((VALUE_TYPE(value) == itl_stringType) ||
		       (VALUE_TYPE(value) == itl_procType)) {
		size = VALUE_SIZE(value) + 1;
		ITL_ALLOC(newVal, char *, size);
		if (VALUE_TYPE(value) == itl_stringType) {
		    ITL_MEMCOPY(STR_VALUE_VAL(value), newVal, size);
		} else {
		    ITL_MEMCOPY(PROC_VALUE_VAL(value), newVal, size);
		}
	    } else {
		return(TRUE);
	    }

	    itlValue_Free(value);
	    VALUE_VAL(value) = (pointer_t)newVal;
	    VALUE_SIZE(value) = size;
	} else if (type == itl_procType) {
	    if (VALUE_TYPE(value) == itl_stringType) {
		itlProcType_t *procP;
		var_val_t *valP;
		ITL_NEW(procP, itlProcType_t);
		procP->proc = STR_VALUE_VAL(value);
		procP->len = strlen(procP->proc) + 1;
		procP->numParams = 0;
		procP->numLocals = 0;
		procP->types = NULL;
		procP->names = NULL;
		valP = SimpleTypeNew((pointer_t)procP, itl_procType);
		*value = *valP;
		ITL_FREE(valP);
	    } else {
		return(TRUE);
	    }
	} else  {
	    return(TRUE);
	}
    }
    VALUE_TYPE(value) = type;
    return(FALSE);
}


PRIVATE boolean_t StringShift _TAKES((
  IN var_val_t *leftVal,
  IN var_val_t *rightVal,
  IN boolean_t leftShift
));

PRIVATE boolean_t StringShift(leftVal, rightVal, leftShift)
  IN var_val_t *leftVal;
  IN var_val_t *rightVal;
  IN boolean_t leftShift;
{
    int shiftVal;
    char *prefix;
    char *str = STR_VALUE_VAL(leftVal);
    int len = strlen(str);

    if (!itlValue_ConvertValue(rightVal, itl_intType))
	return TRUE;
    shiftVal = INT_VALUE_VAL(rightVal) % len;

    if ((shiftVal == 0) || (len == 0))
	return FALSE;

    if ( !leftShift ) {
	shiftVal = -shiftVal;
    }
    if (shiftVal < 0)
	shiftVal += len;

    ITL_ALLOC(prefix, char *, shiftVal);
    ITL_MEMCOPY(str, prefix, shiftVal);
    ITL_MEMCOPY(&str[shiftVal], str, (len - shiftVal));
    ITL_MEMCOPY(prefix, &str[len - shiftVal], shiftVal);
    ITL_FREE(prefix);
    return FALSE;
}

PRIVATE boolean_t StringCmp _TAKES((
  INOUT var_val_t *leftVal,
  IN var_val_t *rightVal,
  IN char *Op
));

PRIVATE boolean_t StringCmp(leftVal, rightVal, Op)
  INOUT var_val_t *leftVal;
  IN var_val_t *rightVal;
  IN char *Op;
{
    int cmpVal;

    if (!itlValue_ConvertValue(rightVal, itl_stringType))
         return TRUE;
    cmpVal = strcmp(STR_VALUE_VAL(leftVal),
		    STR_VALUE_VAL(rightVal));
    if (!itlValue_ConvertValue(leftVal, itl_intType))
         return TRUE;
    switch (*Op) {
      case '<':
	INT_VALUE_VAL(leftVal) =  (Op[1] == '=') ?
	    cmpVal <= 0 : cmpVal < 0;
	break;
      case '=':
	INT_VALUE_VAL(leftVal) =  cmpVal == 0 ;
	break;
      case '>':
	INT_VALUE_VAL(leftVal) =  (Op[1] == '=') ?
	    cmpVal >= 0 : cmpVal > 0;
	break;
      case '!':
        INT_VALUE_VAL(leftVal) = (cmpVal != 0);
        break;
    }
    return FALSE;
}

/*
 *  ReplicateString
 *	Replicate the input string the desired number of times
 *	and write it into the output string.
 *	The number, num, must be >= 0
 *	The output string is preallocated and has enough space
 *	(for the terminating null as well)
 *  Logic:
 *	1- Create a string of half the desired size by a recursive call
 *	2- Duplicate the half by using memcopy
 *	3- Add an additional copy of the input string if the number is odd.
 *
 *  Using this algorithm, the total number of copies is logarithmic
 *  in the replication number.
 *	
 */
PRIVATE void ReplicateString _TAKES((
  IN	char *inStr,
  IN	int   len,	/*  The length of inStr  */
  IN	int   num,
  OUT	char *outStr
));
PRIVATE void ReplicateString(inStr, len, num, outStr)
  IN	char *inStr;
  IN	int   len;	/*  The length of inStr  */
  IN	int   num;
  OUT	char *outStr;
{
    int halfSize = len * ((num+1) / 2);
    int size = len * num;

    assert(num >= 0);
    if (num <= 1){
	ITL_MEMCOPY(inStr, outStr, size);
    } else {
	ReplicateString(inStr, len, (num+1)/2, outStr);
	ITL_MEMCOPY(outStr, outStr + halfSize, size - halfSize);
    }
    outStr[size] = '\0';
}
PRIVATE boolean_t EvalBinaryStrOp(leftVal, rightVal, Op)
  INOUT var_val_t *leftVal;
  IN var_val_t *rightVal;
  IN char *Op;
{
    boolean_t retVal = FALSE;
    int newSize;
    int rightValue;
    /*
     *  It is a string type, perform string operations:
     */
    switch (*Op) {
      case '*':  {
	  /*  Pattern creation by duplication:
	   *  The right side is interpreted as an integer:
	   */
	  if (!itlValue_ConvertValue(rightVal, itl_intType))
	      return(TRUE);
	  rightValue = INT_VALUE_VAL(rightVal);
	  
	  /*  The size includes the terminating NULL.
	   *  We operate on the strings without the terminating
	   *  NULLs and always add the terminating NULL
	   */
	  if (rightValue < 0) {
	      rightValue = 0;
	  } 
	  newSize = (VALUE_SIZE(leftVal) - 1) * rightValue + 1;
	  ITL_UTIL_REALLOC(VALUE_VAL(leftVal), pointer_t, newSize);
	  ReplicateString(STR_VALUE_VAL(leftVal),
			  VALUE_SIZE(leftVal) - 1,
			  rightValue,
			  STR_VALUE_VAL(leftVal));
	  VALUE_SIZE(leftVal) = newSize;
	  break;
      }
      case '%': {	/*  Truncate string to the desired size  */
	  if (!itlValue_ConvertValue(rightVal, itl_intType)) {
	      return TRUE;
	  }
	  rightValue = INT_VALUE_VAL(rightVal);
	  if ((VALUE_SIZE(leftVal) > rightValue) &&
	      (rightValue >= 0)) {
	      STR_VALUE_VAL(leftVal)[rightValue] = '\0';
	      VALUE_SIZE(leftVal) = rightValue + 1;
	      ITL_REALLOC(VALUE_VAL(leftVal), pointer_t, VALUE_SIZE(leftVal));
	  }
	  break;
      }
      case '+': {
	  /*
	   *  String concatenation
	   */
	  if (!itlValue_VarValConvert(rightVal, itl_stringType)) {
	      return TRUE;
	  }
	  
	  /*  The size includes the terminating NULL.
	   *  We operate on the strings without the terminating
	   *  NULLs and always add the terminating NULL
	   */
	  newSize = (VALUE_SIZE(leftVal)-1) + (VALUE_SIZE(rightVal)-1) + 1;
	  ITL_UTIL_REALLOC(VALUE_VAL(leftVal), pointer_t, newSize);
	  
	  ITL_UTIL_MEMCOPY(VALUE_VAL(rightVal),
			   STR_VALUE_VAL(leftVal) + VALUE_SIZE(leftVal) - 1,
			   VALUE_SIZE(rightVal));
	  leftVal->size = newSize;
	  break;
      }
      case '<':
	if (Op[1] == '<') {
	    retVal = StringShift(leftVal, rightVal, TRUE);
	}
	else if ((Op[1] != '\0') && (Op[1] != '=')) {
	    leftVal = NULL;
	    retVal = TRUE;
	}
	else {
	    retVal = StringCmp(leftVal, rightVal, Op);
	}
	break;

      case '=':
	if (Op[1] != '=') {
	    leftVal = NULL;
	    retVal = TRUE;
	}
	else {
	    retVal = StringCmp(leftVal, rightVal, Op);
	}
	break;
      case '!':
	if (Op[1] != '=') {
	    retVal = TRUE;
	    leftVal = NULL;
	}
	else {
	    retVal = StringCmp(leftVal, rightVal, Op);
	}
	break;
      case '>':
	if (Op[1] == '>') {
	    retVal = StringShift(leftVal, rightVal, FALSE);
	}
	else if ((Op[1] != '\0') && (Op[1] != '=')) {
	    return(TRUE);
	}
	else {
	    retVal = StringCmp(leftVal, rightVal, Op);
	}
	break;

      default: 
	return(TRUE);
    }
    return(retVal);
}

PRIVATE boolean_t EvalBinaryIntOp(leftVal, rightVal, Op)
  INOUT var_val_t *leftVal;
  IN var_val_t *rightVal;
  IN char *Op;
{
    int leftValue, rightValue;
    boolean_t retVal = FALSE;

    if (strcmp(Op, "=") == 0) {
	/*  We do not support assignment,  but if the operation
	 *  is assignment (as it may for fetch&OP, we do not want
	 *  to convert the RHS
	 */
	return(TRUE);
    }

    if (!itlValue_ConvertValue(rightVal, itl_intType)) {
	return(TRUE);
    }

    leftValue  = *((int *)leftVal->value);
    rightValue = *((int *)rightVal->value);
    switch(*Op) {
      case '+': leftValue += rightValue; break;
      case '-': leftValue -= rightValue; break;
      case '*': leftValue *= rightValue; break;
      case '^': leftValue ^= rightValue; break;
      case '/': if (rightValue == 0) {
	  itl_Error("About to divide by ZERO - \n I am not sure what you want me to do here.\nI'll let it go and let someone else deal with it. ;-)\n  File: %s, line: %d\n", 
		    __FILE__, __LINE__);
	  return(TRUE);
      } else {
	  leftValue = leftValue / rightValue;
      }
	break;
      case '%': if (rightValue == 0) {
	  itl_Error("About to MOD with ZERO - \n I am not sure what you want me to do here.\nI'll let it go and let someone else deal with it. ;-)\n  File: %s, line: %d\n", 
		    __FILE__, __LINE__);
	  return(TRUE);
      } else {
	  leftValue = leftValue % rightValue;
      }
	break;
      case '.':
	if (Op[1] == '.') {
	    /*
	     *  The random number binary operator:
	     */
	    if (rightValue > leftValue) {
		leftValue += (int) lrand48() % (rightValue - leftValue + 1);
	    }
	    else if (rightValue < leftValue) {
		leftValue -= (int) lrand48() % (leftValue - rightValue + 1);
	    }
	}
	break;
      case '=':
	if (Op[1] == '=') {
	    leftValue = leftValue == rightValue;
	} else {
	    /*  Assignment is not a valid binary operator  */
	    retVal = TRUE;
	}
	break;
      case '!':
	if (Op[1] == '=') {
	    leftValue = leftValue != rightValue;
	}
	break;
      case '|':
	if (Op[1] == '|') {
	    leftValue = leftValue || rightValue;
	} else {
	    leftValue = leftValue | rightValue;
	}
	break;   
      case '&':
	if (Op[1] == '&') {
	    leftValue = leftValue && rightValue;
	} else {
	    leftValue = leftValue & rightValue;
	}
	break;   
      case '<':
	if (Op[1] == '=') {
	    leftValue = leftValue <= rightValue;
	} else if (Op[1] == '<') {
	    leftValue = leftValue << rightValue;
	} else {
	    leftValue = leftValue < rightValue;
	}
	break;
      case '>':
	if (Op[1] == '=') {
	    leftValue = leftValue >= rightValue;
	} else if (Op[1] == '>') {
	    leftValue = leftValue >> rightValue;
	} else {
	    leftValue = leftValue > rightValue;
	}
	break;
    }
    INT_VALUE_VAL(leftVal) = leftValue;
    return(retVal);
}

PRIVATE boolean_t EvalUnaryIntOp(inVal, Op)
  IN var_val_t *inVal;
  IN char *Op;
{
    if (*Op == '#') {
	/*  Return the length of the given value.
	 *  If the value is an int type, the size is the size
	 *  specified in the value, otherwise, the size in the record
	 *  includes the terminating NULL.  We return the NET size,
	 *  which is the recorded size - 1.
	 */
	INT_VALUE_VAL(inVal) = VALUE_SIZE(inVal);
    }
    else {
	int *valP = (int *)inVal->value;
	switch(*Op) {
	  case '-':
	    *valP = -(*valP);
	    break;
	  case '+':
	    break;
	  case '!':
	    *valP = !(*valP);
	    break;
	  case '~':
	    *valP = ~(*valP);
	    break;
	  default:
	    itl_Error("Invalid unary operator on int: %s\n", Op);
	    return(TRUE);
	    break;
	}
    }
    return(FALSE);
}

PRIVATE boolean_t EvalUnaryStringOp(inVal, Op)
  IN var_val_t *inVal;
  IN char *Op;
{
    if (*Op == '#') {
	/*  Return the length of the given value.
	 *  If the value is an int type, the size is the size
	 *  specified in the value, otherwise, the size in the record
	 *  includes the terminating NULL.  We return the NET size,
	 *  which is the recorded size - 1.
	 */
	var_val_t *valP = itlStds_IntVal(VALUE_SIZE(inVal) - 1);
	itlValue_Free(inVal);
	*inVal = *valP;
	ITL_FREE(valP);
    } else {
	return(TRUE);
    }

    return(FALSE);
}

/*
 *  ReadParams
 *	Read the parameter definition for a procedure.
 *	Return TRUE on success, FALSE on failure.
 */
PRIVATE boolean_t ReadParams _TAKES((
  IN	itlProcType_t *procP,
  IN	int numParams
));
PRIVATE boolean_t ReadParams(procP, numParams)
  IN	itlProcType_t *procP;
  IN	int numParams;
{
    int i;
    if ((procP->numParams = numParams) == 0) {
      procP->types == NULL;
      procP->names == NULL;
	return (TRUE);
    } else if (procP->numParams > 12) {
	itl_Error("Too many params: %d, MAX is 12.\n", procP->numParams);
	return(FALSE);
    }
    ITL_ALLOC(procP->types, int *, procP->numParams * sizeof(int));
    ITL_ALLOC(procP->names, char **, procP->numParams * sizeof(char *));
    for (i=0; i<procP->numParams; i++) {
	char *paramT;
	int err;

	itl_PromptUser("Name of param %d: ", i+1);
	procP->names[i] = itl_GetVariableName(&err, NULL);

	itl_PromptUser("Type of param %d: ", i+1);
	paramT = itl_GetString(NULL, NULL);
	procP->types[i] = itlValue_TypeByName(paramT);
	if (procP->types[i] == -1) {
	    if (strcasecmp(paramT, "OUT") == 0) {
		procP->types[i] = OUT_PARAM_TYPE;
	    } else if (*paramT == '\0') {
		procP->types[i] = UNTYPED_PARAM;
	    } else {
		itl_Error("%s is an invalid type\n", paramT);
		ITL_FREE(procP->types);
		return(FALSE);
	    }
	}
	ITL_FREE(paramT);
    }
    return(TRUE);
}

/*
 *  ReadLocals
 *	Read the local variable names for a procedure.
 *	Return TRUE on success, FALSE on failure.
 */
PRIVATE boolean_t ReadLocals _TAKES((
  IN	itlProcType_t *procP,
  IN	int numLocals
));
PRIVATE boolean_t ReadLocals(procP, numLocals)
  IN	itlProcType_t *procP;
  IN	int numLocals;
{
    int i;

    if ((procP->numLocals = numLocals) == 0)
	return TRUE;
    if (procP->numLocals > 12) {
	itl_Error("Too many local variables: %d, MAX is 12.\n",
		  procP->numParams);
	return(FALSE);
    }
    if (procP->names) {
	ITL_REALLOC(procP->names, char **,
		    ((procP->numParams + procP->numLocals) * sizeof(char *)));
    }
    else {
	assert(procP->numParams == 0);
	ITL_ALLOC(procP->names, char **, procP->numLocals * sizeof(char *));
    }
	
    for (i=0; i<procP->numLocals; i++) {
	int err;
	itl_PromptUser("Name of local var %d: ", i);
	procP->names[procP->numParams + i] = itl_GetVariableName(&err, NULL);
    }
    return(TRUE);
}

/* 
 *  ReadProcType -
 *	Prompt and read a procedure definition.
 */
PRIVATE var_val_t *ReadProcType _TAKES((
  IN var_val_t *valP,
  IN int type
));
PRIVATE var_val_t *ReadProcType(valP, type)
  IN var_val_t *valP;
  IN int type;
{
    char *terminatingWord = NULL;
    int length;
    var_val_t *retVal;
    itlProcType_t *procP;

    if (!valP) {
	itl_PromptUser("# of params or Terminating word: ");
	valP = itl_GetVarVal();
    }
    if (VALUE_TYPE(valP) == itl_procType) {
	return(valP);
    }

    ITL_NEW(procP, itlProcType_t);
    procP->numParams = 0;
    procP->numLocals = 0;
    
    if (VALUE_TYPE(valP) == itl_intType) {
	/*
	 *  The user is defining parameters
	 */
	if (!ReadParams(procP, INT_VALUE_VAL(valP))) {
	    itl_FreeVarVal(valP);
	    FreeProcStructure(procP);
	    return(NULL);
	} else {
	    itl_FreeVarVal(valP);
	}

	itl_PromptUser("# local vars or Terminating word: ");
	valP = itl_GetVarVal();
	if (VALUE_TYPE(valP) == itl_intType) {
	    if (!ReadLocals(procP, INT_VALUE_VAL(valP))) {
		itl_FreeVarVal(valP);
		FreeProcStructure(procP);
		return(NULL);
	    } else {
		itl_FreeVarVal(valP);
	    }
	    itl_PromptUser("Terminating string: ");
	    terminatingWord = itl_GetString(NULL, NULL);
	} else if (VALUE_TYPE(valP) == itl_stringType) {
	    (void) ReadLocals(procP, 0);
	    terminatingWord = STR_VALUE_VAL(valP);
	    ITL_FREE(valP);
	} else {
	    itl_Error("A %s value is invalid as a terminating word\n",
		      itlValue_Type(VALUE_TYPE(valP)));
	    itl_FreeVarVal(valP);
	    FreeProcStructure(procP);
	    return(NULL);
	}  
    } else if (VALUE_TYPE(valP) == itl_stringType) {
	(void) ReadParams(procP, 0);
	(void) ReadLocals(procP, 0);
	terminatingWord = STR_VALUE_VAL(valP);
	ITL_FREE(valP);
    } else {
	itl_Error("A %s value is invalid as a terminating word\n",
		  itlValue_Type(VALUE_TYPE(valP)));
	itl_FreeVarVal(valP);
	FreeProcStructure(procP);
	return(NULL);
    }
    procP->proc = itl_GetSegment(terminatingWord, "proc=> ", 
				 type == itl_procType, &length);
    procP->len = length + 1;
    ITL_FREE(terminatingWord);
    retVal = SimpleTypeNew((pointer_t) procP, itl_procType);
    return(retVal);
}

PRIVATE var_val_t *GetFieldFromSimpleType _TAKES((
  IN	var_val_t *valP,
  IN	char *name
));
PRIVATE var_val_t *GetFieldFromSimpleType(valP, name)
  IN	var_val_t *valP;
  IN	char *name;
{
    return NULL;
}

SHARED var_val_t *itlStds_StringVal(str)
  IN  char *str;
{
    var_val_t *retVal = itl_NewVarVal();
    VALUE_TYPE(retVal) = itl_stringType;
    VALUE_SIZE(retVal) = strlen(str) + 1;
    ITL_REALLOC(VALUE_VAL(retVal), pointer_t, VALUE_SIZE(retVal));
    ITL_MEMCOPY(str, VALUE_VAL(retVal), VALUE_SIZE(retVal));
    return(retVal);
}

SHARED var_val_t *itlStds_IntVal(value)
  IN  int value;
{
    var_val_t *retVal = itl_NewVarVal();
    VALUE_TYPE(retVal) = itl_intType;
    VALUE_SIZE(retVal) = sizeof(int);
    ITL_REALLOC(VALUE_VAL(retVal), pointer_t, VALUE_SIZE(retVal));
    INT_VALUE_VAL(retVal) = value;
    return(retVal);
}

SHARED var_val_t *itlStds_ProcVal(str, numParams, types, names)
  IN	char *str;
  IN	int numParams;
  IN	int *types;
  IN	char **names;
{
    var_val_t *tempVal, *retVal;
    itlProcType_t *procP;
    ITL_NEW(procP, itlProcType_t);
    procP->proc = str;
    procP->len = strlen(str) + 1;
    procP->numParams = numParams;
    procP->types = types;
    procP->names = names;
    /*
     *  Create a var_val_t structure using the provided arrays.
     */
    tempVal = SimpleTypeNew((pointer_t) procP, itl_procType);
    /*
     *  Make a copy of this variable.
     */
    retVal = itlValue_Copy(tempVal);
    /*
     *  Free the variable shell
     */
    ITL_FREE(tempVal);
    return(retVal);
}

SHARED char *itlStds_ProcString(valP)
  IN	var_val_t *valP;
{
    return(PROC_VALUE(valP)->proc);
}

PRIVATE boolean_t SetFieldOfSimpleType _TAKES((
  IN	var_val_t *valP,
  IN	char *name,
  IN	var_val_t *fieldValP
));
PRIVATE boolean_t SetFieldOfSimpleType(valP, name, fieldValP)
  IN	var_val_t *valP;
  IN	char *name;
  IN	var_val_t *fieldValP;
{
    return FALSE;
}

/*
 *  ReadSimpleType
 *	Prompt and read a simple type
 */
PRIVATE var_val_t *ReadSimpleType _TAKES((
  IN var_val_t *valP,
  IN int type
));
PRIVATE var_val_t *ReadSimpleType(valP, type)
  IN var_val_t *valP;
  IN int type;
{
    var_val_t *retVal;
    if (!itlValue_TypeIsValid(type)) {
	itl_Error("ReadSimpleType: Invalid type: %d\n", type);
    }
    if (!valP) {
	itl_PromptUser("%s value: ", itlValue_Type(type));
	retVal = itl_GetVarVal();
    } else {
	retVal = valP;
    }
    if (retVal) {
	if (!itlValue_ConvertValue(retVal, type)) {
	    itl_FreeVarVal(retVal);
	    return(NULL);
	}
    }
    return(retVal);
}



SHARED itlValue_varTypeInfo_t standardTypes[] = {
{ "int",
      CopySimpleTypes,
      SimpleTypeNew,
      FreeSimpleType,
      ReadSimpleType,
      EvalBinaryIntOp,
      EvalUnaryIntOp,
      SimpleTypeConvert,
      IntValueToString,
      NULL,
      GetFieldFromSimpleType,
      SetFieldOfSimpleType,
      NULL,
      NULL,
      "ITL signed integers (4 bytes)",
      ""
      },
{ "boolean",
      CopySimpleTypes,
      SimpleTypeNew,
      FreeSimpleType,
      ReadSimpleType,
      EvalBinaryIntOp,
      EvalUnaryIntOp,
      SimpleTypeConvert,
      IntValueToString,
      NULL,
      GetFieldFromSimpleType,
      SetFieldOfSimpleType,
      NULL,
      NULL,
      NULL,
      NULL
      },
{ "string",
      CopySimpleTypes,
      SimpleTypeNew,
      FreeSimpleType,
      ReadSimpleType,
      EvalBinaryStrOp,
      EvalUnaryStringOp,
      SimpleTypeConvert,
      StrValueToString,
      NULL,
      GetFieldFromSimpleType,
      SetFieldOfSimpleType,
      NULL,
      NULL,
      "Null terminated strings.",
      "Null terminated strings.  The length of the string can be obtained \
using the # unary operator.  Binary operations valid for string include: \
+ for concetanation, * for replication, % for truncation, <<, >> for shifting."
  },
{ "proc",
      CopyProcTypes,
      SimpleTypeNew,
      FreeProcType,
      ReadProcType,
      NULL,
      NULL,
      NULL,
      ProcTypeToString,
      ProcTypeDisplay,
      GetFieldFromSimpleType,
      SetFieldOfSimpleType,
      NULL,
      NULL,
      "ITL Procedures (can be run using the exec command)",
      ""
      },
{ "func",
      CopyProcTypes,
      SimpleTypeNew,
      FreeProcType,
      ReadProcType,
      NULL,
      NULL,
      NULL,
      ProcTypeToString,
      ProcTypeDisplay,
      GetFieldFromSimpleType,
      SetFieldOfSimpleType,
      NULL,
      NULL,
      "ITL Functions (can be run using the exec command)",
      ""
      }
};
      
/*
 *  itlExp_UseStandardTypeDef()
 *	Set ITL to use the standard variable types.
 */
EXPORT boolean_t itlExp_UseStandardTypeDef()
{
    itl_Trace(ITL_VERBOSE_TRACE,
	      "Registering standard types\n\t%s\n", RCSid);
    itl_intType	    = itlValue_RegisterVarType(&standardTypes[0]);
    itl_booleanType = itlValue_RegisterVarType(&standardTypes[1]);
    itl_stringType  = itlValue_RegisterVarType(&standardTypes[2]);
    itl_procType    = itlValue_RegisterVarType(&standardTypes[3]);
    funcType        = itlValue_RegisterVarType(&standardTypes[4]);
    itl_ValListInit();
    itl_VecTypeInit();
    return(FALSE);
}

typedef struct {
    var_val_t *procOutParams;
} procContext_t;
/*
 *  itlStds_PreProc
 *	Perform the initialization before a procedure starts
 *	This reads the parameters for the procedure.
 *	The types of the arguments are in the procedure structure
 *	stored in the variable value.
 */
SHARED void itlStds_PreProc(varP, contextP)
  IN	itl_var_t	*varP;
  OUT	pointer_t	*contextP;
{
    int i;
    itl_var_t *tVarP;
    procContext_t *conP;
    var_val_t **vallistPP = NULL;
    var_val_t **outlistPP = NULL;
    itlProcType_t *procP = PROC_VALUE(VAR_VALUE_REC(varP));
    int numVars;

    *contextP = NULL;
    if (VAR_TYPE(varP) != itl_procType)
	return;

    ITL_NEW(conP, procContext_t);

    if ((tVarP = itl_LocateVariable(" outParams")) != NULL) {
	conP->procOutParams = itlValue_Copy(VAR_VALUE_REC(tVarP));
    } else {
	conP->procOutParams = NULL;
    }
    *contextP = (pointer_t)conP;

    if ((numVars = (procP->numParams + procP->numLocals)) > 0) {
	int *typeP;
	var_val_t *valP;
	/*
	 *  We have to initialize both the list of in parameter
	 *  values and the list of out parameters name.
	 *  Since only one of them applies to each, we initialize
	 *  the entry in the other list to some arbitrary int value
	 */
	ITL_ALLOC(vallistPP, var_val_t **, numVars * sizeof(var_val_t *));
	ITL_ALLOC(outlistPP, var_val_t **,
		  procP->numParams * sizeof(var_val_t *));
	for(i=0, typeP = procP->types; i<procP->numParams; i++, typeP++) {
	    if (*typeP == OUT_PARAM_TYPE) {
		boolean_t eof;
		itl_PromptUser("%d: Name of variable for out param %s: ",
			       i, procP->names[i]);
		outlistPP[i] = itlValue_New(itl_GetVariableName(&eof, NULL),
					   itl_stringType);
		vallistPP[i] = itlStds_IntVal(i);
		if (!outlistPP[i]) {
		    itl_Error("Error initializing OUT param %d for proc %s.\n",
			      i, VAR_NAME(varP));
		}
	    } else if (*typeP == PROC_LOCAL_TYPE) {
		/*
		 *  It is a local variable, we have to initialize it
		 *  for the varList type.
		 */
		vallistPP[i] = itlStds_IntVal(i);
		outlistPP[i] = itlStds_IntVal(i);
	    } else {
		if (*typeP == UNTYPED_PARAM) {
		    itl_PromptUser("%d: Value for untyped var %s: ",
				   i, procP->names[i]);
		    vallistPP[i] = itl_GetVarVal();
		} else {
		    assert(*typeP >= 0);
		    itl_PromptUser("%d: Value for var %s (%s): ",
				   i, procP->names[i],
				   itlValue_Type(*typeP));
		    vallistPP[i] = itlValue_Read(NULL, *typeP);
		}
		outlistPP[i] = itlStds_IntVal(i);
		if (!vallistPP[i]) {
		    itl_Error("Error initializing param %d (type %s) for proc %s.\n",
			      i, itlValue_Type(*typeP), VAR_NAME(varP));
		}
	    }
	}
	/*
	 *  Initialize the values for the local variables
	 */
	for (i=0; i<procP->numLocals; i++) {
	    vallistPP[procP->numParams + i] = itlStds_IntVal(i);
	}
	itlExp_PushLocalVariables(numVars, vallistPP, procP->names);

	valP = itl_SetValListVar(procP->numParams, outlistPP);
	itl_SetVariable(" outParams", valP, var_local);
	itl_FreeVarVal(valP);
	for (i=0; i<procP->numParams + procP->numLocals; i++) {
	    itl_FreeVarVal(vallistPP[i]);
	    if (i < procP->numParams)
		itl_FreeVarVal(outlistPP[i]);
	}
	ITL_FREE(vallistPP);
	ITL_FREE(outlistPP);
    } else {
	itlExp_PushLocalVariables(0, NULL, NULL);
    }
}

SHARED void itlStds_PostProc(varP, contextP)
  IN	itl_var_t	*varP;
  IN	pointer_t	contextP;
{
    int i;
    itlProcType_t *procP = PROC_VALUE(VAR_VALUE_REC(varP));
    itl_var_t *outsP = NULL;
    procContext_t *conP = (procContext_t *)contextP;
    var_val_t **outValuesPP, **outVarsPP;

    if (VAR_TYPE(varP) != itl_procType)
	return;

    /*
     *  We have to restore the caller's context and THEN write out
     *  the out parameters.  They must be restored in the caller's
     *  context in case one of the out parameters the calller
     *  specified is an out parameter of the caller.
     *    We do this in three steps:
     *	     save the out values locally
     *	     restore the caller's context
     *	     write the out values.
     */
    if (procP->numParams > 0)
    {
	ITL_ALLOC(outValuesPP, var_val_t **,
		  procP->numParams * sizeof(var_val_t *));
	ITL_ALLOC(outVarsPP, var_val_t **,
		  procP->numParams * sizeof(var_val_t *));

	for (i=0; i<procP->numParams; i++) {
	    if (procP->types[i] == -1) {
		char fieldName[5];
		itl_var_t *localVarP;

		sprintf(fieldName, "%d", i);
		/*
		 *  This is an out parameter
		 */
		if (!outsP) {
		    outsP = itl_LocateVariable(" outParams");
		    assert(outsP);
		}
		outVarsPP[i] = itlValue_GetField(VAR_VALUE_REC(outsP), 
						 fieldName);
		localVarP = itl_LocateVariable(procP->names[i]);
		assert(localVarP);
		outValuesPP[i] = itlValue_Copy(VAR_VALUE_REC(localVarP));
	    }
	}
    }
    /*
     *  Restore caller's context
     */
    itlExp_PopLocalVariables();

    if (conP->procOutParams) {
	itl_SetVariable(" outParams", conP->procOutParams, var_local);
	itl_FreeVarVal(conP->procOutParams);
    }
    ITL_FREE(conP);
    for (i=0; i<procP->numParams; i++) {
	if (procP->types[i] == -1) {
	    char fieldName[5];
	    sprintf(fieldName, "%d", i);
	    itl_SetVariable(STR_VALUE_VAL(outVarsPP[i]),
			    outValuesPP[i], var_local);
	    itl_FreeVarVal(outVarsPP[i]);
	    itl_FreeVarVal(outValuesPP[i]);
	}
    }
    if (procP->numParams > 0)
    {
	ITL_FREE(outValuesPP);
	ITL_FREE(outVarsPP);
    }
}
