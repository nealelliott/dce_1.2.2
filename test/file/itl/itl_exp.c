/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl_exp.c,v 1.1.418.1 1996/10/17 18:27:10 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    SHARED   itlExp_SetParentVarList, itlExp_FreeArbitraryType, 
 *             itlExp_TestAndSet, itlExp_TypeByName
 *    
 *    EXPORT   itlExp_RegisterVarType, itl_FreeVarVal, 
 *             itl_VarValueToBoolean, itl_ShowVarVal, itl_ShowAllVariables, 
 *             itlExp_CollectAllVars, itl_GetInteger, itlExp_Cleanup
 *    
 *    PRIVATE  VarHashFunction, showVariableRecord, freeVariable, 
 *             collectVarIterator
 *    
 *    extern   itl_ShowVariable, getIntVariable, GetInteger, 
 *             cleanupExpIterator
 *    
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1991,1992,1993
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: itl_exp.c,v $
 * Revision 1.1.418.1  1996/10/17  18:27:10  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:37  damon]
 *
 * Revision 1.1.413.1  1994/02/04  20:45:12  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:27  devsrc]
 * 
 * Revision 1.1.411.1  1993/12/07  17:44:29  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:10:03  jaffe]
 * 
 * Revision 1.1.2.5  1993/01/11  18:30:42  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:09:23  htf]
 * 
 * Revision 1.1.2.4  1992/11/18  21:59:38  jaffe
 * 	Transarc delta: fred-itl-ofer-fixes-10-30 1.1
 * 	  Selected comments:
 * 	    Incorporates Ofer's changes that permit procedures to have parameters
 * 	    passed to them, and also includes the vector and valList data types.  The
 * 	    vector data type is needed for DFS testing.  It allows us to manipulate
 * 	    binary data.
 * 	    Changes for procedure parameter passing.
 * 	[1992/11/18  15:00:33  jaffe]
 * 
 * Revision 1.1.2.3  1992/10/28  17:49:41  jaffe
 * 	Transarc delta: fred-itl-add-unary-var-exists 1.2
 * 	  Selected comments:
 * 	    To import Ofer's fix for undefined variables.
 * 	    Add the ? unary operator to check for the existence of ITL variables.
 * 	    Imported Ofer's addition of the ? unary operator.
 * 	    Added itl_Error call when an undefined variable is encountered.  It is
 * 	    placed inside ITL_FS_TEST compile time constant.
 * 	    Change the message output when a variable is missing.
 * 	    Removed a newline from the front of the message.
 * 	Transarc delta: fred-itl-dir-stat-mem-cmds 1.2
 * 	  Selected comments:
 * 	    Adding support for directory functions, stat function, and memory mapped
 * 	    files.
 * 	    Not done yet, just upgrading.
 * 	    This complets this delta.
 * 	    Fixed a defect that Ofer had fixed in TP version.
 * 	Transarc delta: fred-itl-ofer-fixes-9-22-92 1.2
 * 	  Selected comments:
 * 	    Incorporate fixes made to the TP version of ITL.  Some of them are performance
 * 	    related.  Others concerned the printing of error messages.  In most cases
 * 	    messages are no longer printed by support functions.  The determination of
 * 	    whether a message should be printed is now left to functions that call these
 * 	    support functions.
 * 	    Changed showAll function to sort.  Split it into two functions.
 * 	    Fix (picky) compiler warnings
 * 	Transarc delta: fred-itl-ofer-fixes-9-24-92 1.1
 * 	  Selected comments:
 * 	    Include some minor fixes made to the TP ITL.
 * 	    Changed showAll command to not be case sensitive.
 * 	[1992/10/28  12:27:37  jaffe]
 * 
 * 	$TALog: itl_exp.c,v $
 * 	Revision 1.31  1994/11/01  22:29:21  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.30 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.30  1993/12/02  22:14:13  jaffe
 * 	Cleanup copyrights and headers
 * 	[from r1.29 by delta osf-fix-copyrights-for-1.0.3a, r1.1]
 * 
 * Revision 1.29  1993/06/03  12:50:06  fred
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
 * 	[from r1.25 by delta fred-db3612-port-itl-to-solaris, r1.1]
 * 
 * Revision 1.25  1993/03/08  13:51:51  fred
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
 * 	[from r1.24 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 * 
 * Revision 1.24  1993/03/01  14:48:20  fred
 * 	To make ITL work on SUN DCE, i.e. for the Alpine group.  Most of the changes
 * 	relate to SunOS 4.1 since it is not ANSI.
 * 
 * 	Note:  I made the changes and validated that they did not break the RIOS
 * 	       or PMAX builds.  Ward Travis will validate that this compiles on the
 * 	       SUN, since to my knowledge our build environment does not support
 * 	       the SPARC.
 * 
 * 	Changes to support SUN DCE.
 * 	[from r1.23 by delta fred-db3201-make-itl-work-on-suns, r1.1]
 * 
 * Revision 6.2  1993/01/28  22:16:18  travis
 * 	Kick off a series of changes required to get this directory to
 * 	build under SunOS.
 * 
 * 	Cast key destructor argument properly.
 * 	[from r6.1 by delta travis-x-libitl-needs-a-memmove-macro, r1.1]
 * 
 * Revision 1.23  1993/02/02  15:43:13  fred
 * 	[merge of changes from 1.20 to 1.22 into 1.21]
 * 
 * Revision 1.22  1993/01/19  21:12:51  fred
 * 	To add the pack/unpack capabilities to ITL variables so that we can do
 * 	better distributed testing.  Also added some other ITL bug fixes from Ofer.
 * 
 * 	Not yet complete.  Need to fix the FX tests.
 * 
 * 	Changes for pack/unpack.
 * 	[from r1.20 by delta fred-ot6848-add-pack-unpack-capability, r1.1]
 * 
 * Revision 1.21  1993/01/14  17:38:07  jaffe
 * 	revdrop of latest OSF copyright changes to these files
 * 	[from r1.20 by delta osf-revdrop-01-12-92, r1.1]
 * 
 * 	Revision 1.19  1992/11/10  19:49:03  jaffe
 * 	Sync up with the OSF source as of Nov 9.  This delta fixes mostly test
 * 	code problems related to missing OSF copyrights.
 * 	[from revision 1.14 by delta osf-sync-with-nov-9-drop, r1.1]
 * 
 * Revision 1.17  1992/11/03  15:27:26  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 
 * 	Changes for procedure parameter passing.
 * 	[from revision 1.14 by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.14  1992/10/05  16:28:17  mcinerny
 * 	[merge of changes from 1.2 to 1.12 into 1.10]
 * 
 * Revision 1.12  1992/10/02  20:31:51  jaffe
 * 	Sync with OSF sources.
 * 
 * 	Added correct OSF headers.
 * 	[from revision 1.2 by delta osf-cleanup-from-sept-25-92-drop-test, r1.1]
 * 
 * Revision 1.10  1992/09/24  15:23:18  fred
 * 	Include some minor fixes made to the TP ITL.
 * 
 * 	Changed showAll command to not be case sensitive.
 * 	[from revision 1.7 by delta fred-itl-ofer-fixes-9-24-92, revision 1.1]
 * 
 * Revision 1.7  1992/09/23  15:53:35  fred
 * 	Incorporate fixes made to the TP version of ITL.  Some of them are performance
 * 	related.  Others concerned the printing of error messages.  In most cases
 * 	messages are no longer printed by support functions.  The determination of
 * 	whether a message should be printed is now left to functions that call these
 * 	support functions.
 * 
 * 	Changed showAll function to sort.  Split it into two functions.
 * 	[from revision 1.5 by delta fred-itl-ofer-fixes-9-22-92, revision 1.1]
 * 
 * Revision 1.5  1992/09/17  11:59:21  fred
 * 	Change the message output when a variable is missing.
 * 
 * 	Removed a newline from the front of the message.
 * 	[from revision 1.4 by delta fred-itl-add-unary-var-exists, revision 1.2]
 * 
 * Revision 1.4  1992/09/16  21:25:11  fred
 * 	To import Ofer's fix for undefined variables.
 * 	Add the ? unary operator to check for the existence of ITL variables.
 * 
 * 	Imported Ofer's addition of the ? unary operator.
 * 	Added itl_Error call when an undefined variable is encountered.  It is
 * 	placed inside ITL_FS_TEST compile time constant.
 * 	[from revision 1.3 by delta fred-itl-add-unary-var-exists, revision 1.1]
 * 
 * Revision 1.3  1992/09/10  11:50:36  fred
 * 	This complets this delta.
 * 
 * 	Fixed a defect that Ofer had fixed in TP version.
 * 	[from revision 1.2 by delta fred-itl-dir-stat-mem-cmds, revision 1.2]
 * 
 * Revision 1.2  1992/08/20  19:37:28  fred
 * 	Manually imported bug fixes from the Encina ITL.  Got them from Ofer.
 * 	Also, I changed the ITL_FILE_CHECK_ERROR macro to fail when something
 * 	succeeds that was expected to fail.
 * 	Added the verifyFileError command to toggle file system function verification.
 * 	(The code was already there I just was not making it available.)
 * 
 * 	Ofer bug fixes.
 * 	[from revision 1.1 by delta fred-fix-itl-bugs-8-20-92, revision 1.1]
 * 
 * Revision 1.1  1992/08/10  20:10:42  fred
 * 	Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	Initial checkin.
 * 
 * 	Initial checkin.
 * 	[added by delta fred-checkin-itl, revision 1.1]
 * 
 * 	@TRANSARC_COPYRIGHT@
 * 
 * Revision 6.2  1993/01/28  22:16:18  travis
 * 	Kick off a series of changes required to get this directory to
 * 	build under SunOS.
 * 
 * 	Cast key destructor argument properly.
 * 	[from r6.1 by delta travis-x-libitl-needs-a-memmove-macro, r1.1]
 * 
 * 	$EndTALog$
 * 
 * Revision 1.36  1992/04/28  04:36:40  oz
 * 	Continued Work.
 * 
 * 	itl_DisplayVarVal: different format for long strings and short.
 * 	renamed showVariableRecord to be ShowVariableRecord and added prototype.
 * 	[from revision 1.35 by delta oz-2495-ITL:Add-display-upcall-to-var-types, revision 1.3]
 * 
 * Revision 1.35  1992/04/27  21:46:09  oz
 * 	Continued modifications
 * 
 * 	Added itl_DisplayVarVal
 * 	[from revision 1.34 by delta oz-2495-ITL:Add-display-upcall-to-var-types, revision 1.2]
 * 
 * Revision 1.34  1992/04/24  03:46:08  oz
 * 	ITL now supports nested fields in variable types and allows the user
 * 	to access these fields using the . notation.  E.g.,
 * 
 * 	set foo.size 45
 * 	set foo.name.len 34
 * 
 * 	Until now, fethc&OP operations on nested fields was implemented
 * 	allowing the follwoing:
 * 
 * 	fetch&Op foo.size + 4
 * 	fetch&Op foo.name.len * 2
 * 
 * 	But test&Set was not implemented.
 * 	This Delta implements test and set allowing:
 * 
 * 	Test&Set foo.size > 7 7
 * 	Test&Set foo.name.len < 10 25
 * 
 * 	[from revision 1.33 by delta oz-2475:ITL:Implement-test-and-set-on-nested-fields, revision 1.1]
 * 
 * Revision 1.33  1992/04/10  18:55:54  oz
 * 	Continued changes:
 * 
 * 	Moved to itl_value.c:
 * 	itlExp_VarValueConvert
 * 	itlExp_ReadSimpleTypes
 * 	[from revision 1.32 by delta oz:2334-Allow-access-to-var-upcalls, revision 1.2]
 * 
 * Revision 1.32  1992/04/10  03:12:05  oz
 * 	The ITL user can define her own variable types.
 * 	This delta colocates all variable type information in one module
 * 	called itl_value.[ch]
 * 	  In addition, when the client defines and registers variable types
 * 	the client provides upcall ITL uses to manipulate the type.  The new
 * 	module allows the user access to these upcalls, providing greated
 * 	flexibility in defining new, and recursive, types.
 * 
 * 	Moved varTypeArray and numVarTypes to itl_value.c
 * 	Moved itlExp_RegisterVarType to itl_value.c (and renamed)
 * 	Moved routines that had to do with variable types to the itlValue module.
 * 	The routines here call the appropriate routine there.
 * 	[from revision 1.31 by delta oz:2334-Allow-access-to-var-upcalls, revision 1.1]
 * 
 * Revision 1.31  1992/04/03  22:13:39  oz
 * 	  Allow type conversions both ways:
 * 
 * 	  When the itlExp_VarValueConvert function is called with a value of
 * 	  type 'A' and asked to convert it to a value of type 'B', it first
 * 	  calls the conversion function of the type 'A' type.  If this fails, it
 * 	  calls the conversion function of the type 'B' type, if this also
 * 	  fails it prints an error message.
 * 	[from revision 1.30 by delta oz-2228:Add-access-to-recursive-structures, revision 1.3]
 * 
 * Revision 1.30  1992/04/02  14:48:59  oz
 * 	  Continued developlment on the access to recursive access.
 * 	  fetch&Op works properly now.
 * 	  test&Set still doesn't
 * 
 * 	  New functions:
 * 	itlExp_GetNestedField
 * 	itlExp_SetNestedField
 * 	ModifyNestedField
 * 	[from revision 1.29 by delta oz-2228:Add-access-to-recursive-structures, revision 1.2]
 * 
 * Revision 1.29  1992/04/01  20:53:05  oz
 * 	ITL allows the user to define her own variable types.  These may be nested
 * 	structures.  This delta allows the user to access data imbedded in such
 * 	structures and to modify single fields in an existing structure.
 * 
 * 	  New function: LookupVarTable - Used for looking up variables in the table.
 * 	It is similar to itl_LocateVariable but takes a chtLatch var as an
 * 	optional param.
 * 	  Added support for field specifications.
 * 	  LocateVariable: The variable name is up to the first '.'
 * 	  New functions:
 * 	ObtainRecordVal
 * 	itlExp_GetValueField
 * 	itlExp_SetValueField
 * 
 * 	[from revision 1.28 by delta oz-2228:Add-access-to-recursive-structures, revision 1.1]
 * 
 * Revision 1.28  1992/03/30  20:37:23  oz
 * 	ITL variables are thread safe and can easily be used to maintain a test
 * 	program's state.  However, there may be some variables used by the test
 * 	program that one may not want to be accessed by the tester (or the script).
 * 	This change adds this facility by making all variables whose name starts
 * 	with a space invisible to the tester.
 * 
 * 	Modified the function that displays all the variables to ignore variables
 * 	whose name begins with a space.
 * 	[from revision 1.27 by delta oz-2201-Add_invisible_vars, revision 1.1]
 * 
 * Revision 1.27  1992/03/30  15:43:26  oz
 * 	Add a new function to itl: itl_PeekVarVal
 * 	It is a necessary function in order to allow defining arbitrary
 * 	complex user types for itl variables (which the sfs test intends to do)
 * 
 * 	Define the new function
 * 	[from revision 1.26 by delta oz-2193-Add:itl_PeekVarVal-function, revision 1.1]
 * 
 * Revision 1.26  1992/03/27  21:15:42  oz
 * 	Changes which were made before 1.0.1 and were not supposed to be
 * 	incorporated in 1.0.1 and therefore were not readied and were
 * 	not moved to CML with the rest of the file.
 * 
 * 	These changes add tranC support for ITL as well as handling
 * 	transaction aborts (exceptions) in the lower level io modules.
 * 
 * 	See General Comment Above
 * 	[from revision 1.25 by delta oz-2172-itl-support-tranc, revision 1.1]
 * 
 * Revision 1.22  1992/02/11  14:30:16  fred
 * 	Changed UTIL_ASSERT to ASSERT.
 * 	[92/02/11  14:28:07  fred]
 * 
 * Revision 1.21  92/02/02  09:22:13  oz
 * 	- setParent vars now takes a pointer to a var list.
 * 	- Renamed locateParentVar  => LocateParentVar
 * 	[92/01/31  10:45:16  oz]
 * 
 * Revision 1.20  92/01/20  08:30:24  oz
 * 	 - Replaced an occurence of itl_Prompt by itl_PromotUser
 * 	[92/01/18  18:08:02  oz]
 * 
 * 	 - Using:
 * 		itl_Error	instead of itl_PrintErrMsg
 * 		itl_Message	instead of itl_PrintMsg
 * 	[92/01/18  13:01:44  oz]
 * 
 * 	 - use the new itl_Trace instead of itl_TracePrintf
 * 	[92/01/15  14:53:42  oz]
 * 
 * Revision 1.19  92/01/17  10:02:08  oz
 * 	- New Functions:
 * 		CopyValue
 * 		itlExp_TestAndSet
 * 	- renamed: doFetchAndOp   =>  itlExp_FetchAndOp
 * 	[92/01/17  09:59:41  oz]
 * 
 * Revision 1.18  92/01/16  16:41:11  oz
 * 	 - Trace used threadSlots
 * 	[92/01/15  10:17:24  oz]
 * 
 * Revision 1.17  92/01/08  15:16:28  oz
 * 	 - Use itlExp_VarCurVal instead of VarLocalVal to print
 * 	   the local value of a variable (it obtains the parent
 * 	   value if there is no current local value)
 * 	[92/01/06  09:17:48  oz]
 * 
 * Revision 1.16  92/01/02  09:58:21  oz
 * 	 - Added the function name to the message which prints Invalid Type
 * 	[91/12/27  18:33:11  oz]
 * 
 * 	 - Fixed the hash function to use the first letter and
 * 	   the last 5 letters of a var name.
 * 	[91/12/26  09:52:48  oz]
 * 
 * Revision 1.15  91/12/24  14:16:09  oz
 * 	 - Removed include of stds/transarc_stds.h & bde/bde.h
 * 	 - Changed the hash functions for variables:
 * 	   It depends on the last 5 chars
 * 	[91/12/24  14:12:29  oz]
 * 
 * Revision 1.14  91/12/17  15:21:13  fred
 * 	Fixed called to set data for ANSI.
 * 
 * Revision 1.13  91/12/12  13:55:17  oz
 * 	 - New Functions:
 * 	       itlExp_TypeByName
 * 	       itlExp_GetValueByType
 * 	[91/12/10  19:46:39  oz]
 * 
 * Revision 1.12  91/11/21  15:32:48  oz
 * 	 - Allow for additional types:
 * 	 - new type: itlExp_varTypeInfo_t
 * 	 - Array of types: varTypeArray
 * 	 - New routine: itlExp_RegisterVarType
 * 	 - Moved type specific routines to itl_stdtypes.c
 * 	[91/11/11  16:39:13  oz]
 * 
 * Revision 1.11  91/11/06  16:47:03  oz
 * 	Removed unused varaibles
 * 
 * Revision 1.10  91/10/30  16:52:19  oz
 * 	 - Bug: The bitwise & and bitwise | operators were not handled
 * 	        correctly - They failed to check if the next character is
 * 	        a & (resp. |).
 * 	   FIXED.
 * 
 * Revision 1.9  91/10/30  15:02:49  oz
 * 	 - Enhanced allowable expressions on strings:
 * 	     Added: <, <=, ==, >=, >, <<, >>
 * 	 - New Functions:
 * 		StringShift
 * 		StringCmp
 * 		EvalBinaryStrOp
 * 
 * Revision 1.8  91/10/23  16:44:34  oz
 * 	Fixed bug in evaluating random numbers, when rightVal == leftVal-1
 * 
 * Revision 1.7  91/10/21  16:58:16  oz
 * 	Corrected the implementation of RANDOM:
 * 	Works with any range up, down, or equal.
 * 
 * Revision 1.6  91/10/21  14:41:09  oz
 * 	- Added the random binary operator: ..
 * 	- Moved the history to the end.
 * 
 * Revision 1.5  91/10/16  19:10:51  oz
 * 	 - the type var_t no longer exists and was replaced by itl_var_t.
 * 	 - Renamed:
 * 	      locateVariable    ->    itl_LocateVariable
 * 	      varLocalDefault   ->    itlExp_VarCurVal
 * 	      showVariableValue ->    itl_ShowVarVal
 * 
 * Revision 1.4  91/10/02  11:52:13  oz
 * 	Rename initializeExp to be itlExp_Init.
 * 	Write the RCSid in init during verbose tracing.
 * 
 * Revision 1.3  91/09/16  11:53:09  mss
 * 	Make call match declaration
 * 	 itlExp_cleanup => itlExp_Cleanup
 * 
 * Revision 1.2  91/09/12  17:40:20  oz
 * 	Converting the REC tester into a general ITL testing tool.
 * 	Too many changes to list them all for now.
 * 	Many name changes to conform with the transarc standards.
 * 	A more detailed list will be kept after the official first release.
 * 
 * Revision 1.1  91/08/22  17:20:20  mss
 * 	Modified REC tester to make separate library
 * 
 * Revision 1.10  91/08/08  18:15:15  oz
 * 	VarValueToString dynamically allocates the result.
 * 	The CHT is now EXPORT instead of PRIVATE
 * 	Started using the new IO module.
 * 	[91/08/08  17:45:59  oz]
 * 
 * Revision 1.9  91/07/03  18:05:48  oz
 * 	- Added an explanation to the Variable definitions an usage,
 * 	- Replace currentOutput() by the macr Cur_Out
 * 	- Added a scope to insertVariable commands.
 * 	- Added checks before dividing (or modding) by zero.
 * 	  For now it just prints a message.
 * 	[91/07/03  18:03:12  oz]
 * 
 * Revision 1.8  91/06/21  18:14:10  oz
 * 	All output now uses fprintf using thread safe file handles.
 * 	Fixed synchronization glitch when variables were created
 * 	and for fetch&Op: Variables are now locked when created until
 * 	they have a value.
 * 	Replaced debugPrintf by tracePrintf and added a VAR debug level.
 * 	[91/06/21  17:57:57  oz]
 * 
 * Revision 1.7  91/06/06  10:36:54  oz
 * 	Added lock tracing (REC_TEST_LOCK_TRACE)
 * 	Free the variable list after writing out all the variables.
 * 	[91/05/30  17:52:37  oz]
 * 
 * Revision 1.6  91/05/23  14:11:11  oz
 * 	Added a variable for each thread which holds a list
 * 	of variables containing pointers to the values of its parent.
 * 	Added routines for searching for the value of the parent.
 * 	Added a routine which makes a variable global.
 * 	When a variable is global, its local values are always
 * 	ignored.
 * 	Added a routing which collects all the values of all the
 * 	variables into a list which can be printed or sent to
 * 	a child thread.
 * 	[91/05/23  12:36:57  oz]
 * 
 * Revision 1.5  91/05/20  15:02:57  oz
 * 	Small changes to local variables.
 * 	[91/05/20  15:00:31  oz]
 * 
 * Revision 1.4  91/05/17  10:34:59  oz
 * 	Added facility (which is not used yet) for having a thread specific
 * 	(local) values for variables.
 * 	[91/05/17  09:48:09  oz]
 * 
 * 	Corrected a bug in the forLoop definition of the hash function.
 * 	[91/05/10  09:44:51  oz]
 * 
 * 	All functions that handle the history buffer and
 * 	the script variables were moved to this file.
 * 
 * 	The script variables are now stored in a CHT and
 * 	are thread safe.
 * 	[91/05/09  12:59:26  oz]
 * 
 * 	Moved all the routines and data structures dealing
 * 	with script variables and expressions from rec_io.c
 * 	to this new file.
 * 
 * 	Initial Checkin.
 * 	[91/05/07  10:48:23  oz]
 * 
 * $EndLog$
 */
/*
 * itl_exp.c -- 
 *    The routines for handling expressions and variables.
 */

#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include <utils/itl/itl_private.h>
#include <utils/itl/itlio_internal.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_threadStatic.h>
#include <utils/itl/itl_dceActions.h>

/*******************************************************************
 ** 
 **   Some functions that handle script variables.
 **   The variables are stored in a (global) concurrent Hash Table.
 **
 ******************************************************************
 **/


/*
 *   Script Variables.
 *
 *     The test program allows the user to define script variables.
 *     All the variables are kept in a global Concurrent Hash Table (CHT)
 *     indexed by their name.
 *     Each variable has a global value and a local value.  The local
 *     value is local to a thread and is kept on a thread slot.
 *     It is possible to define a variable to be global, in which case
 *     it still has a local value, but the local value is not used.  
 *     Rather, the global value is used.
 *     
 *     The value of a variable is a structure of type var_val_t
 *     which contains the type of the variable, the size of the
 *     variable and a pointer to the value of the variable.
 *
 *     The values of a variable can be obtained by using the following
 *     routines and macros:
 *     VarLocalVal - gives a pointer to the local value of the variable
 *     varGlobalVal - gives a pointer to the global value of the variable
 *     varLocalDefault - gives a pointer to the current value
 *          If the variable is defined to be global, the current value is
 *          the global value, otherwise, it is the local value.
 *
 *     Given a pointer to the value field of a variable, it can be
 *     further analyzed by:
 *     VALUE_TYPE - the type of the variable
 *     VALUE_SIZE - the size
 *     VALUE_VAL  - a pointer to the actual value
 *
 *     Given a pointer to a variable structure, the following macros
 *     are shorthand for accessing the current fields of the variable:
 *     VAR_TYPE - the type of the current value of the variable
 *     VAR_SIZE - the size of the value
 *     VAR_VALUE - a pointer to the value
 *     VAR_NAME - the name of the variable
 *     VAR_STR_VALUE - the value casted to a string (char *)
 *     VAR_INT_VALUE - the value as an integer
 *     VAR_PROC_VALUE - the value as a procedure (char *)
 *     (e.g. VAR_TYPE is a shorthand for VALUE_TYPE(varLocalDefault(varP))
 *      where varLocalDefault gives the current (either local or global)
 *      value of a variable.)
 *
 *     One can easily add more types by adding more types in the 
 *     enumerated structure of types and by modifiying all the routines
 *     That process the variable values, such as varValToStr which converts
 *     a variable value to a string to `know' the new types.
 *
 *     Value Inheritance:
 *     When a thread is created, it inherits all the variable values from
 *     its parent.  This is accomplished as follows:
 *     Before creating a thread the parent composes a list of all its 
 *     variables and their current value (actually, currently a pointer
 *     to the value is kept).  The parent then passes this list (actually
 *     a pointer to that list) to the new thread.
 *     The thread keeps this list in a thread safe variable called
 *     parentVarList.
 *     When a thread looks up a variable, finds that it is a local variable
 *     and the thread has no local value for that variable the thread
 *     looks the variable up in the list parentVarList.  If the variable
 *     is found there its value is taken, and inserted as the new local 
 *     value for the variable.  (Note, each variable will be looked for at
 *     most once in this list.)
 *     Not further that if the parent changes the value of a variable after
 *     creating a thread but before the thread read the variable the thread
 *     will get the new value of the variable.  Currently, when a thread
 *     spawns other thread by a coLoop call the sapwning thread
 *     does not work but simply waits for all the threads it created
 *     to terminate.  Therefore, this is not a problem.  It will be rather
 *     expensive to create a list of the actual values rather than pointers
 *     to the values.
 *
 *     Variable Scope:
 *     As mentioned earlier a variable can be either local or global.
 *     When the user creates a variable he/she can specify the scope
 *     explicitly, however, if it is not defined explicitly, the default
 *     scope is used.  Currently we follow the following scheme:
 *     All variables created by the main thread are by default global.
 *     All variables created by other threads are by default local.
 *     Exceptions: When you perform a fetch&Op operation on a variable
 *     regardles of the previous scope of the variable, the variable
 *     becomes local.
 *     The index variable of a coLoop is always local.
 *
 *     Other routines in this module:
 *     
 *     locateVar: takes a name of a variable and returns a pointer
 *        to the structure associated with the variable in the CHT.
 *     itlExp_NewVarVal: Create a new variable value -
 *	  Allocates the appropriate structure to hold the variable.
 *     itlExp_SetVarVal: Set a value.
 *     itlExp_FreeVarVal: Free a variable value structure.
 *     assignValue:  Assign a value.  It allocates space for the new value
 *        and if the variable had an old value the space is reallocated.
 */

static char RCSid[] = "$";

#if !defined(_NOIDENT) && !defined(lint)
static char *TARCSId = "$Id: itl_exp.c,v 1.1.418.1 1996/10/17 18:27:10 damon Exp $";
#endif


/******************************************************************
 *
 *  The hash function.  It takes the characters of the string, 
 *  copies them to an integer array of length MAX_HASH_LEN
 *  and adds all the numbers together.
 *  
 *  The copy is necessary in case the string is not word alligned
 *  and integers should be.
 */
#define VAR_HASHTABLE_SIZE 512
#define MAX_HASH_LEN 5
PRIVATE int VarHashFunction(str)
  IN char *str;
{
    int result, i;
    int len = strlen(str);
    int rep = (len > MAX_HASH_LEN) ? MAX_HASH_LEN : len;

    for (result = (int)str[0], i=1; i <= rep; i++) {
	result = result * 5 + (int)str[len-i];
    }
    return( result % VAR_HASHTABLE_SIZE );
}

/* 
 *   Set up the CHT functions for the variable table.
 *   All the functions will be prefixed by varTable_ 
 */
#define GET_VAR_KEY(_varP) (VAR_NAME(_varP))
#define VAR_HASH_FUNCTION(_varKey)  VarHashFunction(_varKey)
#define VAR_KEYS_EQUAL(A,B) (strcmp((A), (B)) == 0) 
#define NULL_LATCH (cht_mutex_t *)NULL
CHT_GENERATE_ROUTINES(EXPORT,varTable_,VAR_HASHTABLE_SIZE,
		      itl_var_t,
		      chtLink,
		      char *,
		      GET_VAR_KEY,
		      VAR_HASH_FUNCTION,
		      VAR_KEYS_EQUAL)

/*
 *  Declare keys for thread data area(s) used by this module.
 */
PRIVATE pthread_key_t varListKey;
PRIVATE pthread_key_t localVarListKey;


PRIVATE void InitVarList(listP)
  IN varList_t *listP;
{
    listP->nameList  = NULL;
    listP->valList  = NULL;
    listP->listLen  = 0;
    listP->listSize  = 0;
    listP->allocInc  = 128;
    listP->local = FALSE;
    listP->copied = FALSE;
}

/*
 *  itlExp_ParentVarList:  Returns a pointer to a thread safe list containing
 *  the names and values of al the variables of the parent thread.
 *  The list is created by the parent when the thread is created - 
 *  However, the list contains pointers to the thread values so the values
 *  may change if the parent thread changes the values of those variables
 *  before the thread uses them.  As soon as the thread looks up a 
 *  value in this list it copies the value to a local value.
 */
SHARED varList_t *itlExp_ParentVarList()
{
    varList_t *varListP;

    ITL_THREAD_STATIC_INIT(varListKey, varListP, varList_t, InitVarList);

    if (DebugLevel & ITL_THREAD_SLOT_TRACE) {
	itl_Message("varList on slot %d, data %x\n",
		    (int) varListKey,
		    varListP);
    }

    return(varListP);
}

/*
 *  itlExp_SetParentVarList:  Set the list of the variables associated with
 *  the parent.  Before a thread is created, the creator compiles
 *  a list of all its variables and passes a pointer to this list
 *  to the new thread.  The new thread passes this pointer to this routine
 *  (setParentVarList) which puts it in a thread slot for later
 *  lookup.  When a thread looks up a variable and cannot find a local
 *  value for it it scans this list.  If it finds a value for the 
 *  variable in the list it assigns this value to be the new
 *  local value of the variable.
 */
SHARED void itlExp_SetParentVarList(list)
  IN varList_t *list;
{
    *(itlExp_ParentVarList()) = *list;
}


/* 
 *    LocateParentVar()
 *       locate a variable value in the list of values
 *       associated with the parent thread.
 */
PRIVATE var_val_t *LocateParentVar(varName)
  IN char *varName;
{
    varList_t *parentList = itlExp_ParentVarList();
    int i;
    if (!parentList) return((var_val_t *) NULL);
    for (i=0; i<parentList->listLen; i++) {
	if (strcmp(parentList->nameList[i], varName) == 0)
	    return(parentList->valList[i]);
    }
    return((var_val_t *)NULL);
}


/*
 *  Routines for manipulating proc-local variables:
 */
typedef struct localVarInfo {
    struct localVarInfo *next;
    int		 numVars;
    itl_var_t	*varList;
} localVarInfo_t;

PRIVATE void InitLocalVarList(varListPP)
  IN localVarInfo_t **varListPP;
{
    *varListPP = (localVarInfo_t *) 0;
}

PRIVATE localVarInfo_t **LocalVarList()
{
    localVarInfo_t **varList;

    ITL_THREAD_STATIC_INIT(localVarListKey, varList, localVarInfo_t *, 
			   InitLocalVarList);
    return (varList);
}

PRIVATE itl_var_t *AssignVarVal _TAKES((
  OUT itl_var_t *variable,
  IN var_val_t *value,
  IN scope_t scope
));

/*
 *  Routines to initialize the local variables when a new
 *  thread is created inside a procedure with local vairables
 */
PRIVATE void ThreadInitVarLocals _TAKES (( IN pointer_t *ptrP));
PRIVATE void ThreadStartVarLocals _TAKES (( IN pointer_t *ptrP));
PRIVATE void ThreadCleanVarLocals _TAKES (( IN pointer_t *ptrP));

typedef struct {
  int	      numVars;
  char      **names;
  var_val_t **values;
} varLocalsInfo_t;

PRIVATE void ThreadInitVarLocals(ptrP)
  IN pointer_t *ptrP;
{
  localVarInfo_t *listP = *LocalVarList();
  varLocalsInfo_t *retVal;

  if (listP && listP->numVars > 0) {
    int i;
    itl_var_t *varP;
    ITL_NEW(retVal, varLocalsInfo_t);
    retVal->numVars = listP->numVars;

    ITL_ALLOC(retVal->names, char **, listP->numVars * sizeof(char *));
    ITL_ALLOC(retVal->values, var_val_t **,
	      listP->numVars * sizeof(var_val_t *));
    for (i=0, varP=listP->varList; i<listP->numVars; i++, varP++) {
      ITL_ALLOC(retVal->names[i], char *, strlen(VAR_NAME(varP)) + 1);
      ITL_MEMCOPY(VAR_NAME(varP), retVal->names[i], strlen(VAR_NAME(varP)) + 1);
      retVal->values[i] = itlValue_Copy(itlExp_VarCurVal(varP));
    }
  } else {
    retVal = NULL;
  }
  *ptrP = (pointer_t)retVal;
}

PRIVATE void ThreadStartVarLocals(ptrP)
  IN pointer_t *ptrP;
{
  varLocalsInfo_t *newLocalsP = (varLocalsInfo_t *)*ptrP;
  if (newLocalsP) {
    itlExp_PushLocalVariables(newLocalsP->numVars,
			      newLocalsP->values,
			      newLocalsP->names);
  }
}

PRIVATE void ThreadCleanVarLocals(ptrP)
  IN pointer_t *ptrP;
{
  varLocalsInfo_t *newLocalsP = (varLocalsInfo_t *)*ptrP;
  int i;
  if (newLocalsP) {
    for (i=0; i<newLocalsP->numVars; i++) {
      ITL_FREE(newLocalsP->names[i]);
      itlValue_Free(newLocalsP->values[i]);
    }
    ITL_FREE(newLocalsP->names);
    ITL_FREE(newLocalsP->values);
    ITL_FREE(newLocalsP);
  }
}


SHARED void itlExp_PushLocalVariables(numVars, vallistPP, varNames)
  IN	int numVars;
  IN	var_val_t **vallistPP;
  IN	char **varNames;
{
    int i;
    localVarInfo_t **listPP = LocalVarList();
    localVarInfo_t *newEntryP;
    itl_var_t *newVarP;

    ITL_NEW(newEntryP, localVarInfo_t);
    newEntryP->next = *listPP;
    newEntryP->numVars = numVars;
    newEntryP->varList = (itl_var_t *) 0;

    if (numVars > 0)
    {
	ITL_ALLOC(newEntryP->varList, itl_var_t *, numVars * sizeof(itl_var_t));

	for (i=0, newVarP = newEntryP->varList; i<numVars; i++, newVarP++) {
	    newVarP->val = NULL;	/* It has no initial value yet */
	    ITL_ALLOC(newVarP->name, char *, strlen(varNames[i])+1);
	    ITL_MEMCOPY((pointer_t)varNames[i], (pointer_t) VAR_NAME(newVarP),
			strlen(varNames[i])+1);
	    newVarP->state = itl_var_procLocal;
	    AssignVarVal(newVarP, vallistPP[i], var_global);
	}
    }
    *listPP = newEntryP;
}

SHARED void itlExp_PopLocalVariables()
{
    int i;
    localVarInfo_t **topEntryPP = LocalVarList();
    localVarInfo_t *topEntryP = *topEntryPP;
    localVarInfo_t *newTopP;

    assert(topEntryP);
    newTopP = topEntryP->next;

    for (i=0; i<topEntryP->numVars; i++) {
	ITL_FREE(VAR_NAME(&topEntryP->varList[i]));
	itl_FreeVarVal(VAR_VALUE_REC(&topEntryP->varList[i]));
    }

    if (topEntryP->varList)
	ITL_FREE(topEntryP->varList);

    ITL_FREE(topEntryP);
    *topEntryPP = newTopP;
}

/*
 *  LookupLocalVariable
 *	Lookup a variable in the procedure local variable list.
 */
PRIVATE itl_var_t *LookupLocalVariable _TAKES((IN char *name));
PRIVATE itl_var_t *LookupLocalVariable(name)
  IN	char *name;
{
    localVarInfo_t *topEntryP = *LocalVarList();
    int i;

    if (!topEntryP)
	return(NULL);
    for (i=0; i<topEntryP->numVars; i++) {
	if (strcmp(VAR_NAME(&(topEntryP->varList[i])), name) == 0) {
	    return(&(topEntryP->varList[i]));
	}
    }
    return(NULL);
}

/*
 *  LookupVarTable
 *	-  Look for a variable structure in the variable tables.
 *
 *	This first searches the list of local variables.  If the
 *	variable is not found there, then the global hash tables
 *	is searched.
 *
 *  Parameters:
 *	varName - The name of the variable
 *	chtLatchP - an out parameter for the CHT latch on the variable.
 *		    If this is NULL, the CHT lock is dropped before returning.
 */
PRIVATE itl_var_t *LookupVarTable _TAKES((
  IN  char *varName,
  OUT cht_mutex_t *chtLatchP
));

PRIVATE itl_var_t *LookupVarTable(varName, chtLatchP)
  IN  char *varName;
  OUT cht_mutex_t *chtLatchP;
{
    char *dotLocation;
    char *useName = varName;
    int dotOffset;
    itl_var_t *retVal;

    dotLocation = strchr(varName, '.');
    if (dotLocation) {
	dotOffset = dotLocation - varName;
	ITL_ALLOC(useName, char *, dotOffset + 1);
	ITL_MEMCOPY(varName, useName, dotOffset);
	useName[dotOffset] = '\0';
    }
    retVal = LookupLocalVariable(useName);
    if (retVal) {
	if (chtLatchP)
	    *chtLatchP = (cht_mutex_t)NULL;
    } else {
	if (chtLatchP)
	    retVal = varTable_Lookup(useName, CHT_LATCH_KEEP, chtLatchP);
	else
	    retVal = varTable_Lookup(useName, CHT_LATCH_BRACKET);
    }

    if (dotLocation)
	ITL_FREE(useName);
    return(retVal);
}


/*
 *  itl_LocateVariable:
 *    Given a string representing a variable name it returns a pointer to
 *    a record containing that variable or NULL if there is no such record.
 *
 *    If the name contains a '.', the variable name is the string up to
 *    and not including the '.', the rest is a field spec.
 */
EXPORT itl_var_t *itl_LocateVariable(vName)
  IN char *vName;
{
  itl_var_t *tempVar = LookupVarTable(vName, NULL);
  itl_Trace(ITL_VAR_TRACE,
	    "locateVariable looked for %s, returning variable %s\n",
	    vName, (tempVar ? VAR_NAME(tempVar) : "NULL"));
  return(tempVar);
}


/*
 *  Allocate and initialize a new variable value structure
 */
var_val_t *itl_NewVarVal()
{
    var_val_t *newValue;
    ITL_UTIL_ALLOC(newValue, var_val_t *, sizeof(var_val_t));
    VALUE_SIZE(newValue) = sizeof(int);
    ITL_UTIL_ALLOC(VALUE_VAL(newValue), pointer_t, sizeof(int));
    VALUE_TYPE(newValue) = itl_intType;;
    *(int *)VALUE_VAL(newValue) = 0x0deff00d;
    return(newValue);
}

/*
 *  itlExp_FreeArbitraryType
 *	Free an arbitrary type.
 */
SHARED void itlExp_FreeArbitraryType(varValP)
  IN var_val_t *varValP;
{
    itlValue_Free(varValP);
}

/*
 *  itl_FreeVarVal():
 *       Free a dynamically created variable
 */
EXPORT void itl_FreeVarVal(varValP)
IN var_val_t *varValP;
{
    if (!varValP)
	return;
    /*
     *  If the value has allocated space, free it.
     */
    itlExp_FreeArbitraryType(varValP);
    /*
     *  Free the value structure itself
     */
    ITL_UTIL_FREE(varValP);
}

/*
 *  AssignValue: Copy a value structure.
 *  If the new value already has a value structure, make sure it is
 *  big enough by using realloc, otherwise, allocate the necessary space.
 */
PRIVATE var_val_t *AssignValue _TAKES((OUT var_val_t *, IN var_val_t *));
PRIVATE var_val_t *AssignValue(outVal, inVal)
OUT var_val_t *outVal;
IN var_val_t *inVal;
{
    var_val_t *copyVal = itlValue_Copy(inVal);
    if (!copyVal) {
	itl_Error("AssignValue: Invalid type: %d\n",
		  VALUE_TYPE(inVal));
    } else {
	itlExp_FreeArbitraryType(outVal);
	*outVal = *copyVal;
	ITL_FREE(copyVal);
    }
    return(outVal);
}

SHARED boolean_t itlExp_SetPermitted(varP, scope)
  IN	itl_var_t *varP;
  IN	scope_t	   scope;
{
  if ((varP->state == itl_var_global) &&
      (scope == var_same) &&
      (itlIO_DefaultScope() != var_global)) {
      itl_Message("WARNING: The global variable %s is being set in a thread other than the main thread\n",
		  VAR_NAME(varP));
  }
  return(TRUE);
}


SHARED var_val_t *itlExp_GetNestedField(valP, fieldSpec)
  IN  var_val_t *valP;
  IN  char *fieldSpec;
{
    char *nextDot = strchr(fieldSpec, '.');
    char *localSpec = fieldSpec;
    char *curField;
    var_val_t *retVal = valP;
    boolean_t valAllocated = FALSE;	/*  Should retVal be freed  */

    itl_Trace(ITL_VAR_TRACE,
	      "Getting field %s for type %d (%s)\n",
	      fieldSpec,
	      VALUE_TYPE(valP),
	      itlValue_Type(VALUE_TYPE(valP)) ?
	      itlValue_Type(VALUE_TYPE(valP)) : "Invalid");

    if (nextDot) {
	/*
	 *  There are at least two fields.
	 *  We need to modify the field spec (to insert nulls).
	 *  We make a local copy of it and free it later.
	 */
	ITL_ALLOC(localSpec, char *, strlen(fieldSpec)+1);
	ITL_MEMCOPY(fieldSpec, localSpec, strlen(fieldSpec)+1);
    }
    curField = localSpec;
    while (TRUE) {
	var_val_t *newVal;

	nextDot = strchr(curField, '.');
	if (nextDot)
	    *nextDot = '\0';
	newVal = itlValue_GetField(retVal, curField);
	if (valAllocated) {
	    itlValue_Free(retVal);
	    ITL_FREE(retVal);
	}
	retVal = newVal;;
	valAllocated = TRUE;
	if (!retVal || !nextDot) {
	    if (localSpec != fieldSpec)
		ITL_FREE(localSpec);
	    return retVal;
	}
	curField = nextDot + 1;
    }
}

/*
 *  ObtainFieldVal
 *	Given a var_val_t and a field spec.
 *	Return a pointer to the dynamically allocated field value.
 *	  The field spec is a variable name optionally followed by
 *	a dot sperated string of field names.
 *	The value, valP is a variable vaule.  The field spec is the full
 *	field spec including the variable name.
 */
PRIVATE var_val_t *ObtainFieldVal _TAKES((
  IN	var_val_t *valP,
  IN	char *fieldSpec
));
PRIVATE var_val_t *ObtainFieldVal(valP, fieldSpec)
  IN	var_val_t *valP;
  IN	char *fieldSpec;
{
    char *nextDot = strchr(fieldSpec, '.');
    var_val_t *retVal;

    if (nextDot) {
	retVal = itlExp_GetNestedField(valP, nextDot+1);
    } else {
	retVal = AssignValue(itl_NewVarVal(), valP);
    }
    return(retVal);
}

/*
 *    VarLocalVal()
 *       return a pointer to the local value structure of a
 *       variable.  If alloc is TRUE and the variable has
 *       no local structure, a local structure is created.
 *
 *    Assumptions:  The variable is allocated, including the thread slot
 */
var_val_t *VarLocalVal(varP, alloc)
IN itl_var_t *varP;
IN boolean_t alloc;
{
 
    var_val_t *localValue;

    VAR_THREAD_REC(varP, localValue);

    if (!localValue && alloc) {
	int retCode;

	localValue = itl_NewVarVal();
	retCode = pthread_setspecific(varP->threadVal, 
				      (pthread_addr_t) localValue);
	ITL_CHECK_DCE_CODE(retCode, ITL_PTHR_SET_DATA);
    }
    return(localValue);
}

/*
 *    VarGlobalVal()
 *       return a pointer to the global value structure of a
 *       variable.  If alloc is TRUE and the variable has
 *       no global structure, a global structure is created.
 *
 */
var_val_t *VarGlobalVal(varP, alloc)
IN itl_var_t *varP;
IN boolean_t alloc;
{
    var_val_t *globalValue = VAR_GLOBAL_REC(varP);

    if (!globalValue && alloc) {
	globalValue = itl_NewVarVal();
	VAR_GLOBAL_REC(varP) = globalValue;
    }
    return(globalValue);
}

 
/*
 *  itlExp_VarCurVal():
 *     If the variable has a local value, the local value is returned.
 *     Otherwise, the list of the variable values of the parent is 
 *     searched and finally the global value is used.
 *     If the variable does not have a local value, the local value is
 *     assigned to be the new value found in the search and that value
 *     is returned.
 */
EXPORT var_val_t *itlExp_VarCurVal(varP)
IN itl_var_t *varP;
{
    var_val_t *curValue;

    if ((varP->state == itl_var_global) ||
	(varP->state == itl_var_procLocal)) {
	itl_Trace(ITL_VAR_TRACE,
		  "itlExp_VarCurVal: Var %s is global - returning the global val\n",
		  VAR_NAME(varP));
	return(VarGlobalVal(varP, FALSE));
    }
    /*
     *  It is not a global variable - Look for a local value
     */
    if (!(curValue = VarLocalVal(varP, FALSE))) {
	/*
	 *  It has no local value, check the list of parent values
	 */
	if (!(curValue = LocateParentVar(VAR_NAME(varP)))) {
	    /*
	     *  No value there either, use a default value
	     */
	    itl_Trace(ITL_VAR_TRACE,
		      "itlExp_VarCurVal: Var %s currently has no local value\n        Assigning it a local value from the global value\n",
		      VAR_NAME(varP));
	    curValue = VarGlobalVal(varP, FALSE);
	} else {
	    /*
	     *  Found a value in the parent's list...
	     */
	    itl_Trace(ITL_VAR_TRACE, 
		      "itlExp_VarCurVal: Var %s currently has no local value\n        Assigning it a default value from its parent.\n",
		      VAR_NAME(varP));
	}
	assert(curValue);
	(void) AssignValue(VarLocalVal(varP, TRUE), curValue);
    } else {
	itl_Trace(ITL_VAR_TRACE, 
		  "itlExp_VarCurVal: Using the local value for %s\n",
		  VAR_NAME(varP));
    }

    return(curValue);
}

/*
 *  itl_VarValueToString(varValue, strLen):
 *     Return the value of a script variable as a string
 *     and return the length of the string (not including terminating NULL)
 *
 *  THIS ROUTINE ALLOCATES SPACE FOR THE STRING.
 *  It is up to the caller to free the space.
 */
EXPORT char *itl_VarValueToString(valP, outLen)
IN    var_val_t *valP;
INOUT int *outLen;
{
    char *res = itlValue_ToString(valP, outLen);
    if (!res) {
	itl_Error("VarValueToString: Invalid type: %d\n",
		  VALUE_TYPE(valP));
	ITL_UTIL_ALLOC(res, char *, 15);
	sprintf(res, "## Invalid ##");
	if (outLen)
	    *outLen = strlen(res);
    }
    return(res);
}

/*
 *  itl_VarValueToBoolean - Convert a varValue to boolean.
 */
EXPORT boolean_t itl_VarValueToBoolean(varVal)
  IN var_val_t *varVal;
{
    return(varVal &&
	   (((VALUE_TYPE(varVal) == itl_intType) && INT_VALUE_VAL(varVal)) ||
	    *STR_VALUE_VAL(varVal)));
}


/*
 * itl_ShowVarVal() --  Display a variable record (short form).
 *
 * RETURN CODES: none
 */
EXPORT boolean_t itl_ShowVarVal(name, value, scope)
IN char *name;
IN var_val_t *value;
IN char *scope;
{
    if (!value) {
	return(TRUE);
    } else {
	char *varValue = itl_VarValueToString(value, NULL);
	itl_Message("%s = %s (%s) %s %s\n",
		    name,
		    varValue,
		    itlValue_Type(value->type),
		    (int) strlen(scope) > 0 ? "-" : "",
		    scope);
	ITL_UTIL_FREE(varValue);
	return(FALSE);
    }
}

/*
 * itl_DisplayVarVal() --  Display a variable record (long form).
 *
 * RETURN CODES: none
 */
EXPORT boolean_t itl_DisplayVarVal(name, value, scope)
IN char *name;
IN var_val_t *value;
IN char *scope;
{
    if (!value) {
	return(TRUE);
    } else {
	char *varValue = itlValue_Display(value, NULL);
	if (!varValue) {
	    itl_Error("Invalid type passed to display.\n");
	    return(TRUE);
	} else {
	    itl_Message((int) strlen(varValue) > 65 ?
			"%s = \n%s\n\t\t(%s) %s %s\n" :
			"%s = %s (%s) %s %s\n",
			name,
			varValue,
			itlValue_Type(VALUE_TYPE(value)),
			(int) strlen(scope) > 0 ? "-" : "",
			scope);
	    ITL_FREE(varValue);
	}
	return(FALSE);
    }
}

/*
 *  ShowVariableRecord()
 *
 *    Display the value of a variable record
 *	We use the function itlExp_VarCurVal to get the local
 *	value if any so that if it is a local variable which has no
 *	local value the value is fetched from the parent.
 */
PRIVATE void ShowVariableRecord _TAKES(( IN var_t *record ));
PRIVATE void ShowVariableRecord(record)
  IN var_t *record;
{
    if (!record) {
	itl_Message("No Such Variable.\n");
    } else {
	var_val_t *curVal = itlExp_VarCurVal(record);
	if (record->state == itl_var_threadLocal) {
	    if (curVal) {
		(void) itl_DisplayVarVal(VAR_NAME(record),
					 curVal,
					 "thread-local");
	    } else {
		itl_Message("Variable %s is not assigned a thread local value\n",
			    VAR_NAME(record));
	    }
	}
	curVal = VarGlobalVal(record, FALSE);
	if (curVal) {
	    (void) itl_DisplayVarVal(VAR_NAME(record),
				     curVal,
				     record->state == itl_var_global ?
				     "proc-local" : "global");
	} 
    }
}

/*  itl_ShowVariable:
 *    given a string representing a variable name, display the contents
 *    of the variable if one exists.
 */
void itl_ShowVariable(name)
IN char *name;
{
    itl_var_t *varRecord = locateVariable(name);
    if (varRecord)
	ShowVariableRecord(varRecord);
    else
	itl_Message("No such variable: %s\n", name);
}


/*
 *  CmpVarStruct
 *	Compare two var structures for sorting.
 */
PRIVATE int CmpVarNames _TAKES((
  IN	char *var1,
  IN	char *var2
));
PRIVATE int CmpVarNames(var1, var2)
  IN	char *var1;
  IN	char *var2;
{
    char *arrInd1 = strchr(var1, '[');
    char *arrInd2 = strchr(var2, '[');

    if (arrInd1 && arrInd2 && isdigit(arrInd1[1]) && isdigit(arrInd2[1])) {
	/*  The variables may be arrays, sort them by array index.
	 *  For now we do not deal with multi dimensional arrays
	 */
	int bracketOffset = arrInd1 - var1;
	int res = strncasecmp(var1, var2, bracketOffset);
	if (res != 0)
	    return res;
	else if (var2[bracketOffset] != '[')
	    return(-1);
	else {
	    int val1 = strtol(arrInd1 + 1, NULL, 0);
	    int val2 = strtol(arrInd2 + 1, NULL, 0);
	    if (val1 < val2)
		return -1;
	    else if (val1 > val2)
		return 1;
	    else
		return 0;
	}
    } else
	return strcasecmp(var1, var2);
}

SHARED void itlExp_ShowVariables(matchStr)
  IN	char *matchStr;
{
    int j;
    varList_t varList;
    int i;

    for (j=0; j<2; j++) {
	boolean_t headerPrinted = FALSE;
	varList = itlExp_CollectAllVars(j != 0, FALSE);
	/*
	 *  Count the number of visible variables.
	 */
	if (!matchStr) {
	    int numVars = 0;
	    for (i=0; i<varList.listLen; i++) {
		if (varList.nameList[i][0] != ' ')
		    numVars ++;
	    }
	    itl_Message("  There are %d %s variables:\n", numVars,
			j==0 ? "global" : "local");
	    headerPrinted = TRUE;
	}
	/*
	 *  Sort the variable list
	 */
	for (i=0; i<varList.listLen; i++) {
	    int l;
	    for (l=i+1; l<varList.listLen; l++) {
		if (CmpVarNames(varList.nameList[i], varList.nameList[l]) > 0){
		    char *t = varList.nameList[i];
		    var_val_t *x = varList.valList[i];
		    varList.nameList[i] = varList.nameList[l];
		    varList.nameList[l] = t;
		    varList.valList[i] = varList.valList[l];
		    varList.valList[l] = x;
		}
	    }
	}

	for (i=0; i<varList.listLen; i++) {
	    if ((varList.nameList[i][0] != ' ') &&
		(!matchStr || strcasestr(varList.nameList[i], matchStr))) {
		if (!headerPrinted) {
		    itl_Message("  The following %s variables match '%s':\n", 
				j==0 ? "global" : "local",
				matchStr);
		    headerPrinted = TRUE;
		}
		itl_ShowVarVal(varList.nameList[i], varList.valList[i], "");
	    }
	}
	itlExp_FreeVarList(&varList);
    }

}

/*
 *  Display the values of all the script variables.
 */
EXPORT boolean_t itl_ShowAllVariables(param)
  IN int param;
{
    itlExp_ShowVariables(NULL);
    return(FALSE);
}

/*
 *  NewVariable():
 *     Allocate a new variable:  Allocate the variable structure,
 *     Space for the variable value, and for the variable name
 *     and copy the name of the variable.
 *
 *   Return:  A pointer to the new variable.
 */
PRIVATE itl_var_t *NewVariable _TAKES((
				       IN char *name,
				       IN scope_t scope));

PRIVATE itl_var_t *NewVariable(name, scope)
IN char *name;
IN scope_t scope;
{
    itl_var_t *tempVar;
    int retCode;

    ITL_UTIL_ALLOC(tempVar, itl_var_t *, sizeof(var_t));
    tempVar->val = NULL;

    if ((scope == var_global) ||
	((itlIO_DefaultScope() == var_global) &&
	 ((scope == var_same) || (scope == var_default)))) {
	tempVar->state = itl_var_global;
    } else {
	tempVar->state = itl_var_threadLocal;
    }

    ITL_UTIL_ALLOC(VAR_NAME(tempVar), char *, strlen(name)+1);
    ITL_UTIL_MEMCOPY( (pointer_t)name, (pointer_t) VAR_NAME(tempVar),
		     strlen(name)+1);
    /* 
     * Allocate the thread data area - its value is assigned later.
     */
    retCode = pthread_keycreate(&(tempVar->threadVal), 
				(pthread_destructor_t) itl_FreeVarVal);
    ITL_CHECK_DCE_CODE(retCode, ITL_PTHR_KEY_CREATE);

    itl_Trace(ITL_THREAD_SLOT_TRACE,
	      "Allocated slot %d for variable %s\n",
	      tempVar->threadVal,
	      name);

    return(tempVar);
}

/*
 *   Free a variable structure
 *
 *  Free the global structure if any, free the space allocated for the
 *  name and then free the variable itself.
 */
PRIVATE void freeVariable(varP)
IN itl_var_t *varP;
{
    int retCode;

    itl_Trace(ITL_VAR_TRACE,
	      "Freeing variable %s.\n",
	      VAR_NAME(varP));
    /*  Free the local value for this thread.
     *  We cannot free the local value of other threads.  We assume
     *  that when this is called there are no other threads.
     *  The value is free by assigning NULL to the slot.
     *  pthreads will free the old value with the free function we
     *  provided when we created the value.
     */
    retCode = pthread_setspecific(varP->threadVal, (pthread_addr_t) 0);
    ITL_CHECK_DCE_CODE(retCode, ITL_PTHR_SET_DATA);
    /*  
     * Free the global value for the variable
     */
    itl_FreeVarVal(VarGlobalVal(varP, FALSE));
    VAR_GLOBAL_REC(varP) = NULL;
    /*  
     *  Free the space allocated for the name and for the
     *  variable itself.
     */
    ITL_UTIL_FREE(VAR_NAME(varP));
    ITL_UTIL_FREE(varP);
}

/*
 *  AssignVarVal:
 *      Assign a value to a given varValue record.
 *      Make sure to dealocate any memory used by the previous
 *      variable if any (e.g., in the case of a string variable
 */
PRIVATE itl_var_t *AssignVarVal(variable, value, scope)
  OUT itl_var_t *variable;
  IN var_val_t *value;
  IN scope_t scope;
{
    /*
     *   Use the local value if:
     *	   The variable is not a proc-local variable and:
     *	    the user either wants to force it to be local or the user
     *	    wants it to stay the same and it is local
     */
    if ((variable->state != itl_var_procLocal) &&
	((scope == var_local)  ||
	 ((scope == var_default) && (itlIO_DefaultScope() == var_local)) ||
	 ((variable->state == itl_var_threadLocal) && (scope == var_same)))) {
	variable->state = itl_var_threadLocal;
	(void) AssignValue(VarLocalVal(variable, TRUE), value);
    }
    /*
     *  We assign a global value if we do not assign a local value
     *  or if there is no global value
     */
    if ((variable->state == itl_var_procLocal) ||
	(scope == var_global) ||
	((scope == var_default) && (itlIO_DefaultScope() == var_global)) ||
	((scope == var_same) && (variable->state == itl_var_global)) ||
	!VarGlobalVal(variable, FALSE)) {
	if ((variable->state == itl_var_threadLocal) &&
	    ((scope == var_global) ||
	     (scope == var_default) && (itlIO_DefaultScope() == var_global))) {
	    variable->state = itl_var_global;
	}
	(void) AssignValue(VarGlobalVal(variable, TRUE), value);
    }
    return(variable);
}

/*
 *  insertVariable:
 *    given a record corresponding to a variable, insert the record
 *    into the table.
 *
 *    Locks:  The caller must hold a latch on the bucket containing
 *    (or will contain) this variable.  This routine takes no latches.
 */
PRIVATE itl_var_t *insertVariable(name, value, scope)
  IN char *name;
  IN var_val_t *value;
  IN scope_t scope;
{
    itl_var_t *temp_var = NewVariable(name, scope);
    
    itl_Trace(ITL_VERBOSE_TRACE, 
	      "Inserting variable %s\n",
	      VAR_NAME(temp_var));

    (void) AssignVarVal(temp_var, value, scope);
    varTable_Insert(temp_var, CHT_LATCH_NONE);
    return(temp_var);
}

/*
 *  itl_SetVariable:
 *    set a variable to a specified val.value.
 *    If the variable has an entry, modify the values, otherwise
 *    insert the record using insertVariable.
 */
EXPORT var_t *itl_SetVariable(name, value, scope)
  IN char *name;
  IN var_val_t *value;
  IN scope_t scope;
{
    itl_var_t *var_p, *result;
    cht_mutex_t chtLatch;
    char *dotLocation = strchr(name, '.');

    assert(value);
    itl_Trace(ITL_LOCK_TRACE, 
	      "Locking entry for var %s (%d)\n",
	      name, VAR_HASH_FUNCTION(name));

    var_p = LookupVarTable(name, &chtLatch);

    itl_Trace(ITL_VAR_TRACE, "Setting Variable %s\n", name);
    if (var_p == (var_t *)NULL) {
	/*
	 *  There is no entry for this variable
	 */
	if (dotLocation) {
	    /*
	     *  If we are setting one field of a variable, the variable
	     *  must exist
	     */
	    result = NULL;
	} else {
	    itl_Trace(ITL_VAR_TRACE, "No variable %s, Creating\n", name);
	    result = insertVariable(name, value, scope);
	}
    } else {
	if (!itlExp_SetPermitted(var_p, scope)) {
	    result = NULL;
	} else {
	    if (dotLocation) {
		/*
		 *  We are assigning one field of a variable
		 */
		if (!itlExp_SetNestedField(VAR_VALUE_REC(var_p),
					   dotLocation + 1, value)) {
		    result = NULL;
		} else {
		    result = var_p;
		}
	    } else {
		itl_Trace(ITL_VAR_TRACE,
			  "Assigning a new value to %s:\n",
			  VAR_NAME(var_p));
	
		result = AssignVarVal(var_p, value, scope);
	    }
	}
    }

    itl_Trace(ITL_LOCK_TRACE, 
	      "unLocking entry for var %s (%d)\n",
	      name, VAR_HASH_FUNCTION(name));
    cht_unlock(chtLatch);
    return(result);
}

SHARED itl_var_t *itlExp_SetVarValue(name, type, size, value, scope)
  IN char *name;
  IN int type;
  IN int size;
  IN pointer_t value;
  IN scope_t scope;
{
    var_val_t tempVal;
    tempVal.type = type;
    tempVal.size = size;
    tempVal.value = value;
    return(itl_SetVariable(name, &tempVal, scope));
}

/*
 *    getIntVariable:
 *      Given a string representing a name of an integer variable
 *      return the value of the variable.
 *      If the variable does not exist or it is not an integer variable
 *      the function returns 0.
 */ 
int getIntVariable(name)
  IN char *name;
{
  itl_var_t *var_p = locateVariable(name);

  if ((var_p == (var_t *)NULL) || (VAR_TYPE(var_p) != itl_intType)) {
    return(0);
  }
  else {
    return(VAR_INT_VALUE(var_p));
  }
}

/*
 *  setIntVariable:
 *    Given a name of a variable and an integer it sets the variable
 *    to that value by calling itlExp_SetVarValue.
 */
var_t *itl_SetIntVariable(name, value, scope)
  IN char *name;
  IN int value;
  IN scope_t scope;
{
    return(itlExp_SetVarValue(name, itl_intType, sizeof(int), &value, scope));
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlExp_TestAndSet
 *	Perform a test-and-set operation on a script variable.
 *
 *	The current global value of the variable is examined:
 *	The described operation is performed on it.  If the result
 *	is TRUE, the value newValue is set to the global value of the
 *	variable and the script variable CommandResult is set to TRUE.
 *	Otherwise, the global value of the variable is unchanged
 *	and the script variable CommandResult is set to FALSE.
 */
SHARED  boolean_t itlExp_TestAndSet(name, value, op, newValue)
  IN char *name;
  IN var_val_t *value;
  IN char *op;
  IN var_val_t *newValue;
{
    cht_mutex_t chtLatch;
    char *dotLoc;
    boolean_t result;
    pthread_t pthreadId = pthread_self();

    itl_var_t *temp_var = LookupVarTable(name, &chtLatch);

    itl_Trace(ITL_LOCK_TRACE, 
	      "[%d] =+= Holding a lock on the bucket of variable %s (%d)\n",
	      pthread_getunique_np(&pthreadId),
	      name,
	      VAR_HASH_FUNCTION(name));

    itl_Trace(ITL_VAR_TRACE,
	      "[%d] =+= Test&Set - var %s, op %s.\n",
	       pthread_getunique_np(&pthreadId),
	       name, op);
    if (!temp_var) {
	/*  The variable does not exist - The result is FALSE  */
	if (chtLatch)
	    cht_unlock(chtLatch);
	itl_Trace(ITL_VAR_TRACE,
		  "[%d] =+= Test&Set -  Var %s not defined - returnin FALSE.\n",
		  pthread_getunique_np(&pthreadId), name);
	return(FALSE);
    } 

    result = FALSE;
    if (dotLoc = strchr(name, '.')) {
	/*
	 *  The user specified a nested field.
	 */
	var_val_t *globalVal = VarGlobalVal(temp_var, FALSE);
	var_val_t *nestedValP = itlExp_GetNestedField(globalVal, dotLoc + 1);
	if (nestedValP) {
	    if (itl_VarValueToBoolean(EvalBinaryOp(nestedValP, value, op))) {
		result = itlExp_SetNestedField(globalVal,
					       dotLoc + 1, newValue);
	    }
	    itl_FreeVarVal(nestedValP);
	}
    } else {
	var_val_t *globalVal = VarGlobalVal(temp_var, FALSE);
	var_val_t *tempVal = itlValue_Copy(globalVal);

	assert(globalVal);
	assert(tempVal);

	(void) EvalBinaryOp(tempVal, value, op);
	if (result = itl_VarValueToBoolean(tempVal)) {
	    (void) AssignValue(globalVal, newValue);
	}
	itl_FreeVarVal(tempVal);
    }
    if (chtLatch)
	cht_unlock(chtLatch);
    return(result);
}


/*
 * SwapItlValues
 *	Convert the rhs value to the type of the lhs value (if needed)
 *	and then swap the two values.
 *	Return TRUE on success, FALSE on failure (the conversion may fail).
 */
PRIVATE boolean_t SwapItlValues _TAKES((
  IN	var_val_t *lhsP,
  IN	var_val_t *rhsP
));
PRIVATE boolean_t SwapItlValues(lhsP, rhsP)
  IN	var_val_t *lhsP;
  IN	var_val_t *rhsP;
{
    var_val_t tempVal;

    /*
     *  Swap the two values, the right hand side will be freed
     *  by the caller.  This is the simplest way since we are
     *  allowed to modify the right hand side, but whatever we
     *  return must be free-able
     */
    tempVal = *lhsP;
    *lhsP = *rhsP;
    *rhsP = tempVal;
    return(TRUE);
}


/*
 *  itlExp_FetchAndOp
 *    Given a name of a variable,  a var-value and an operation, it 
 *    takes the current value of the variable, performs the operation
 *    and writes the value back, all the time holding a lock on
 *    CHT row containing the variable.
 */
SHARED var_t *itlExp_FetchAndOp(name, value, op)
  IN char *name;
  IN var_val_t *value;
  IN char *op;
{
    cht_mutex_t chtLatch;
    itl_var_t *temp_var = LookupVarTable(name, &chtLatch);
    var_val_t *globalVal;
    char *dotLoc;
    boolean_t success;

    itl_Trace(ITL_LOCK_TRACE, 
	      "Holding a lock on the bucket of variable %s (%d)\n",
	      name,
	      VAR_HASH_FUNCTION(name));

    itl_Trace(ITL_VAR_TRACE, "Fetch&Op: var: %s, op: %s\n", name, op);
    if (!temp_var) {
	/*  The variable does not exist - Create  */
	assert(chtLatch);
	cht_unlock(chtLatch);
	itl_Trace(ITL_VAR_TRACE,
		  "fetch&Op: Var %s not defined - defining\n",
		  name);
	temp_var = setIntVariable(name, 0, var_local);
	/*  Look for it again to lock it.  */
	temp_var = LookupVarTable(name, &chtLatch);
	assert(temp_var);
    }
    globalVal = VarGlobalVal(temp_var, FALSE);
    if (!temp_var || !globalVal) {
	itl_Trace(ITL_LOCK_TRACE, 
		  "Unlocking the bucket of variable %s (%d)\n",
		  name,
		  VAR_HASH_FUNCTION(name));
	if (chtLatch)
	    cht_unlock(chtLatch);
	return((var_t *)NULL);
    }
    /*
     *  Set the local value of the variable to be the current
     *  global value and make sure that the variable is not GLOBAL.
     */
    if (temp_var->state == itl_var_global) {
	temp_var->state = itl_var_threadLocal;
    }
    AssignVarVal(temp_var, globalVal, var_local);
    /*
     *  Compute and write the new value into the global value of temp_var
     *  We first call the user's binary operator upcall to perform the
     *  binary operation.  If this fails, and the operation requested is
     *  the assignment operator, we do it ourselves.
     */
    if (dotLoc = strchr(name, '.')) {
	var_val_t *newValP = itlExp_GetNestedField(globalVal, dotLoc + 1);
	if (newValP &&
	    (!itlValue_BinaryOp(newValP, value, op) ||
	     ((strcmp(op, "=") == 0) && SwapItlValues(newValP, value)))) {
	    success = itlExp_SetNestedField(globalVal, dotLoc + 1, newValP);
	} else {
	    success = FALSE;
	}
	itl_FreeVarVal(newValP);
    } else {
	success = !itlValue_BinaryOp(globalVal, value, op) ||
	    ((strcmp(op, "=") == 0) && SwapItlValues(globalVal, value));
    }

    itl_Trace(ITL_LOCK_TRACE, 
	      "Unlocking the bucket of variable %s (%d)\n",
	      name,
	      VAR_HASH_FUNCTION(name));
    if (chtLatch)
	cht_unlock(chtLatch);
    return(success ? temp_var : NULL);
}

/*
 *  itl_SetStrVariable:
 *    Given a name of a variable and a string it sets the variable
 *    to that value by calling itlExp_SetVarValue.
 */
var_t *itl_SetStrVariable(name, string, scope)
  IN char *name;
  IN char *string;
  IN scope_t scope;
{
    return(itlExp_SetVarValue(name, itl_stringType, strlen(string)+1, string, scope));
}

/*
 *    itl_GetStringVariable:
 *      Given a string representing a name of an integer variable
 *      return the value of the variable.
 *      If the variable does not exist or it is not an string variable
 *      the function returns NULL.
 */ 
char * itl_GetStringVariable(name)
IN char *name;
{
/* do we need to lock table to get value? */
  itl_var_t *var_p = locateVariable(name);
  char * retValue;

  if ((var_p == (var_t *)NULL) || (VAR_TYPE(var_p) != itl_stringType)) {
    return((char *)0);
  }
  else {
    ITL_UTIL_ALLOC(retValue, char *, VAR_SIZE(var_p));
    ITL_UTIL_MEMCOPY( (pointer_t)VAR_STR_VALUE(var_p), (pointer_t) retValue,
		VAR_SIZE(var_p));
    return retValue;
  }
}

EXPORT itl_var_t *itl_SetProcVariable(name, string, scope)
  IN char *name;
  IN char *string;
  IN scope_t scope;
{
    itl_var_t *retVal;
    var_val_t *valP = itlStds_ProcVal(string, 0, NULL, NULL);

    retVal = itlExp_SetVarValue(name, itl_procType,
				VALUE_SIZE(valP), VALUE_VAL(valP), scope);
    itl_FreeVarVal(valP);
    return(retVal);
}



/*
 *  itlExp_CollectAllVars()
 *    return an array of pointers to the current values
 *    of all the variables.
 *    The function is divided into two: collectAllVars which 
 *    initialized the return structure and calls the CHT iterator function.
 *    collectVarIterator which is called by the CHT forAll iterator
 *    on each variable.
 */
PRIVATE boolean_t collectVarIterator(varP, bufferP)
IN itl_var_t *varP;
IN char *bufferP;
{
    varList_t *result = (varList_t *)bufferP;

    if (((varP->state == itl_var_global) && !result->local) ||
	((varP->state == itl_var_threadLocal) && result->local)) {
	if (result->listSize == result->listLen) {
	    result->listSize+= result->allocInc;
	    ITL_REALLOC(result->valList, var_val_t **,
			(result->listSize * sizeof(var_val_t *)));
	    ITL_REALLOC(result->nameList, char **,
			(result->listSize * sizeof(char *)));
	}
	(result->nameList)[result->listLen] = VAR_NAME(varP);
	(result->valList)[result->listLen] = 
	    (result->copied ?  itlValue_Copy(itlExp_VarCurVal(varP)) :
	     itlExp_VarCurVal(varP));
	(result->listLen)++;
    }
    return(TRUE);
}

EXPORT varList_t itlExp_CollectAllVars(local, copy)
  IN boolean_t local;
  IN boolean_t copy;
{
    static varList_t resultInit = {NULL, NULL, 0, 0, 128, FALSE, FALSE};
    varList_t result;

    result = resultInit;
    result.local = local;
    result.copied = copy;
    
    /*  Initialize an array for the variable list  */
    /*  The loop then safely uses realloc	   */
    ITL_ALLOC(result.valList, var_val_t **,
	      result.allocInc * sizeof(var_val_t *));

    ITL_ALLOC(result.nameList, char **, result.allocInc * sizeof(char *));
    result.listSize = result.allocInc;

    (void) varTable_Forall(collectVarIterator,
			   (char *)&result,
			   CHT_LATCH_BRACKET);
    return(result);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlExp_FreeVarList
 *	Free the list of variables collected by a
 *	call to itlExp_CollectAllVars().
 */
EXPORT void itlExp_FreeVarList(listP)
  IN varList_t *listP;
{
    if (listP->copied) {
	int i;
	/*
	 *  The list has a private copy of all the values.
	 *  First, free all the values, and then the list itself.
	 */
	for (i=0; i<listP->listLen; i++) {
	    itl_FreeVarVal(listP->valList[i]);
	}
    }
    ITL_FREE(listP->valList);
    ITL_FREE(listP->nameList);
}

/*******************************************************************
 *
 *   Routines dealing with Expressions.
 * 
 *******************************************************************
 */


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  EvalBinaryOp
 *     Evaluate a binary operator on variable values.
 *
 *  Parameters:
 *	leftVal  - The left input value (var_val_t type)
 *	rightVal - The right input value (var_val_t type)
 *	Op       - the operator (string)
 *
 *  This function performs the operation on the given values.
 *  The result is written over the first (left) value.
 *  The second value (the right value) may also be modified -
 *  its type may be converted depending on the first type and on the
 *  operation.
 *  
 *  This function als returns a pointer to the result for convenience.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT var_val_t *EvalBinaryOp(leftVal, rightVal, Op)
  INOUT var_val_t *leftVal;
  IN var_val_t *rightVal;
  IN char *Op;
{
    if (itlValue_BinaryOp(leftVal, rightVal, Op)) {
	itl_Error("%s is an invalid op between types %s and %s\n",
		  Op,
		  itlValue_Type(VALUE_TYPE(leftVal)),
		  itlValue_Type(VALUE_TYPE(rightVal)));
    }
    return(leftVal);
}

/*
 *  EvalUnaryOp
 *     Evaluate a unary operator on a variable value.
 *
 *  Parameters:
 *	inVal - The input value (var_val_t type)
 *	unaryOp - the operator (string)
 *
 *  This functions performs the operation on the value, in place.
 *  The result overwrites the current value of inVal.
 */
EXPORT var_val_t *EvalUnaryOp(inVal, unaryOp)
  IN var_val_t *inVal;
  IN char *unaryOp;
{
    if (itlValue_UnaryOp(inVal, unaryOp)) {
	itl_Error("%s is an invalid unary op on type %s\n",
		  unaryOp,
		  itlValue_Type(VALUE_TYPE(inVal)));
    }
    return(inVal);
}


PRIVATE boolean_t ModifyNestedField _TAKES((
  IN var_val_t *inValP,
  IN char *fieldName,
  IN var_val_t *fieldVal
));
PRIVATE boolean_t ModifyNestedField(inValP, fieldName, fieldVal)
  IN var_val_t *inValP;
  IN char *fieldName;
  IN var_val_t *fieldVal;
{
    char *dotLocation = strchr(fieldName, '.');
    boolean_t retVal;

    itl_Trace(ITL_VAR_TRACE,
	      "Setting field %s for type %d (%s) to type %d (%s)\n",
	      fieldName,
	      VALUE_TYPE(inValP),
	      itlValue_Type(VALUE_TYPE(inValP)),
	      VALUE_TYPE(fieldVal),
	      itlValue_Type(VALUE_TYPE(fieldVal)));

    if (!dotLocation) {
	retVal = itlValue_SetField(inValP, fieldName, fieldVal);
	return(retVal);
    } else {
	var_val_t *valP;

	*dotLocation = '\0';
	valP = itlValue_GetField(inValP, fieldName);
	if (!valP)
	    return(FALSE);
	retVal = ModifyNestedField(valP, dotLocation + 1, fieldVal);
	if (!retVal)
	    return(FALSE);
	retVal = itlValue_SetField(inValP, fieldName, valP);
	itlValue_Free(valP);
	ITL_FREE(valP);
	return(TRUE);
    }
}

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
 */
SHARED boolean_t itlExp_SetNestedField(inValP, fieldSpec, fieldVal)
  IN var_val_t *inValP;
  IN char *fieldSpec;
  IN var_val_t *fieldVal;
{
    char *nextDot = strchr(fieldSpec, '.');
    char *localSpec = fieldSpec;
    boolean_t retVal;

    if (nextDot) {
	/*
	 *  There are at least two fields.
	 *  We need to modify the field spec (to insert nulls).
	 *  We make a local copy of it and free it later.
	 */
	ITL_ALLOC(localSpec, char *, strlen(fieldSpec)+1);
	ITL_MEMCOPY(fieldSpec, localSpec, strlen(fieldSpec)+1);
    }
    retVal = ModifyNestedField(inValP, localSpec, fieldVal);
    if (nextDot)
	ITL_FREE(localSpec);
    return(retVal);
}


/*******************************************************************
 *
 *   Routines dealing with Arbitrary Expressions.
 * 
 *******************************************************************
 */

/*
 *  SetDefaultVal
 *     Set the default value of a var value.
 */
EXPORT var_val_t *SetDefaultVal(inVal)
  OUT var_val_t *inVal;
{
    inVal->size = sizeof(int);
    inVal->type = itl_intType;
    ITL_UTIL_REALLOC(inVal->value, pointer_t, inVal->size);
    *((int *)inVal->value) = 0;
    return(inVal);
}

typedef struct {
    char	*opStr;		/*  The string representation	*/
    int		precedence;	/*  0 is the lowest precedence	*/
    int		associate;	/*  LEFT_ASSOCIATE or RIGHT_ASSOCIATE */
} opDef_t;

PRIVATE int highestPrecedence;
#define LEFT_ASSOCIATE	11
#define RIGHT_ASSOCIATE	12

/*
 *  opDefTable
 *	Converting Binary operations to codes.
 *	If one operation string is a prefix of another, the longer
 *	one must appear first in the table below.
 */
PRIVATE opDef_t opDefTable[] = {
  {"||", 0,	LEFT_ASSOCIATE},
  {"|",  2,	LEFT_ASSOCIATE},
  {"^",  3,	LEFT_ASSOCIATE},
  {"&&", 1,	LEFT_ASSOCIATE},
  {"&",  4,	LEFT_ASSOCIATE},
  {"==", 5,	LEFT_ASSOCIATE},
  {"!=", 5,	LEFT_ASSOCIATE},
  {"<<", 7,	LEFT_ASSOCIATE},
  {"<=", 6,	LEFT_ASSOCIATE},
  {"<",  6,	LEFT_ASSOCIATE},
  {">>", 7,	LEFT_ASSOCIATE},
  {">=", 6,	LEFT_ASSOCIATE},
  {">",  6,	LEFT_ASSOCIATE},
  {"+",  8,	LEFT_ASSOCIATE},
  {"-",  8,	LEFT_ASSOCIATE},
  {"*",  9,	LEFT_ASSOCIATE},
  {"/",  9,	LEFT_ASSOCIATE},
  {"%",  9,	LEFT_ASSOCIATE},
  {"..", 10,	LEFT_ASSOCIATE}
};

#define OP_TABLE_SIZE (sizeof(opDefTable)/ sizeof(opDef_t))
/*  firstOpTab: For each character, it defines the index of the first
 *  operation the opDefable that starts with the given char
 */
PRIVATE int firstOpTab[256];
/*
 *  NextOperator
 *	Return the next binary operator in the input line, if any.
 */
PRIVATE int NextOperator _TAKES(( void ));
PRIVATE int NextOperator()
{
    char *curLine;
    int i;
    opDef_t *curDef;
    int firstInd;

    itlIO_AdvanceToNonWhite();
    curLine = itlIO_HistoryLoc();
    firstInd = firstOpTab[*curLine];
    for (i = firstInd, curDef = &opDefTable[firstInd];
	 i < OP_TABLE_SIZE;i++, curDef++) {
	if (strncmp(curLine, curDef->opStr, strlen(curDef->opStr)) == 0) {
	    itlIO_AdvanceIndex(strlen(curDef->opStr));
	    return(i);
	}
    }
    return(-1);
}


/*
 *  find an expression factor:
 *  Part of a routine for evaluating expressions correctly
 *  See also: getIntTerm
 */
var_val_t *GetFactor()
{
    char *unaryOp;
    var_val_t *retVal;
    
    
    if (itl_MatchCharacter('-')) {
	unaryOp = "-";
    } else if (itl_MatchCharacter('+')) {
	unaryOp = "+";
    } else if (itl_MatchCharacter('!')) {
	unaryOp = "!";
    } else if (itl_MatchCharacter('#')) {
	unaryOp = "#";
    } else if (itl_MatchCharacter('~')) {
	unaryOp = "~";
    } else if (itl_MatchCharacter('(')) {
	retVal = GetExpression();
	if (!itl_MatchCharacter(')')) {
	    itl_Error("Expecting a ) to close the expression\n");
	    retVal = SetDefaultVal(retVal);
	    return(retVal);
	}
	return(retVal);
    } else if (itl_MatchCharacter('$')) {
	boolean_t err;
	var_t *varP;
	char *varName = itl_GetVariableName(&err, NULL);
	if (!varName || !(varP = locateVariable(varName))) {

#ifdef ITL_FS_TEST
	    /*
	     *  If a variable is un-defined we let the user know it.
	     *  Not doing this provides for very difficult debugging of
	     *  ITL scripts.
	     */
	    itl_Error("Invalid or missing variable, %s\n\n", 
		      (varName) ? varName : "_NO_NAME_VAR_");
#endif  /* ITL_FS_TEST */

	    retVal = SetDefaultVal(itl_NewVarVal());
	    if (varName)
		ITL_UTIL_FREE(varName);
	    return(retVal);
	}
	retVal = ObtainFieldVal(VAR_VALUE_REC(varP), varName);
	if (!retVal) {
	    itl_Error("Invalid variable spec: %s\n", varName);
	    retVal = SetDefaultVal(itl_NewVarVal());
	}
	ITL_UTIL_FREE(varName);
	return(retVal);
    } else if (itl_MatchCharacter('?')) {
	boolean_t err;
	char *varName = itl_GetVariableName(&err, NULL);
	if (!varName || !itl_LocateVariable(varName)) {
	    retVal = itlStds_IntVal(FALSE);
	} else {
	    retVal = itlStds_IntVal(TRUE);
	}
	ITL_UTIL_FREE(varName);
	return(retVal);
    } else {
	char nextC = itl_NextCharacter();
	if ((nextC >= '0') && (nextC <= '9')) {
	    /*
	     * It is a digit
	     */
	    int len;
	    boolean_t eof;
	    char *word = itl_GetAlphaWord(&eof, &len);

	    retVal = itl_NewVarVal();
	    retVal->size = sizeof(int);
	    retVal->type = itl_intType;
	    ITL_UTIL_REALLOC(retVal->value, pointer_t, sizeof(int));
	    *((int *)retVal->value) = (int)strtol(word, NULL, 0);
	    ITL_FREE(word);
	    return(retVal);
	} else {
	    boolean_t err;
	    int len;
	    char *outStr;
	    if (itl_MatchCharacter('"')) {
		outStr = itl_GetQuotedString(&err, &len);
	    } else {
		outStr = itl_GetWord(&err, &len);
	    }
	    if (!outStr) {
		retVal = SetDefaultVal(itl_NewVarVal());
		return(retVal);
	    } else {
		retVal = itl_NewVarVal();
		retVal->size = len+1;
		ITL_UTIL_FREE(retVal->value);
		retVal->value = (pointer_t)outStr;
		retVal->type = itl_stringType;
		return(retVal);
	    }
	}
    }
    /*
     *  If we got here, the first character was a unary operator.
     *  Deal with it.
     */
    retVal = GetFactor();
    (void) EvalUnaryOp(retVal, unaryOp);
    return(retVal);
}

/*
 * GetTerm
 *	This function implements the recursive decent expression parsing.
 *	At any level of the parsing:
 *	1- If this is the highest level, read a factor, otherwise,
 *	   recursively obtain a term from the higher level.
 *	2- See if the next operation is of this precedence (it cannot
 *	   be of a higher precedence).
 *	   a- If it is of the current precedence, perform the operation
 *     	      obtaining a new lhs and goto step 1.
 *	   b- Otherwise, return
 */
PRIVATE var_val_t *GetTerm _TAKES((
  IN	int  level,
  INOUT	int *nextOpP
));
PRIVATE var_val_t *GetTerm(level, nextOpP)
  IN	int level;
  INOUT	int *nextOpP;
{
    var_val_t *retVal;
    
    if (level > highestPrecedence) {
	retVal = GetFactor();
	*nextOpP = NextOperator();
	return(retVal);
    } else {
	retVal = GetTerm(level+1, nextOpP);
    }

    if (opDefTable[*nextOpP].associate == LEFT_ASSOCIATE) {
	while ((*nextOpP != -1) &&
	       (opDefTable[*nextOpP].precedence == level)) {
	    int perfOp = *nextOpP;
	    var_val_t *rightVal = GetTerm(level+1, nextOpP);
	    (void) EvalBinaryOp(retVal, rightVal,
				opDefTable[perfOp].opStr);
	    itl_FreeVarVal(rightVal);
	}
    } else {
	/*
	 *  Right Associative Operations are handled recursively
	 */
	if ((*nextOpP != -1) && (opDefTable[*nextOpP].precedence == level)) {
	    int perfOp = *nextOpP;
	    var_val_t *rightVal = GetTerm(level, nextOpP);
	
	    (void) EvalBinaryOp(retVal, rightVal, opDefTable[perfOp].opStr);
	    itl_FreeVarVal(rightVal);
	}
    }
    assert((*nextOpP == -1) || (opDefTable[*nextOpP].precedence < level));
    return(retVal);
}

EXPORT var_val_t *GetExpression()
{
    int nextOp;
    var_val_t *retVal = GetTerm(0, &nextOp);

     if (itl_MatchCharacter('?')) {
	 boolean_t condPart = itl_VarValueToBoolean(retVal);
	 itl_FreeVarVal(retVal);
	 retVal = GetTerm(0, &nextOp);

	  if (!itl_MatchCharacter(':')) {
	       itl_Error("Illegal conditional expression, expected a ':'");
	       SetDefaultVal(retVal);
	       return(retVal);
	  } else {
	       var_val_t *elsePart = GetTerm(0, &nextOp);
	       if (!condPart) {
		   (void) AssignValue(retVal, elsePart);
	       }
	       itl_FreeVarVal(elsePart);
	       return(retVal);
	  }
     } else {
	 return(retVal);
     }
}


/*
 *  Get an integer:
 */
int GetInteger()
{
    int retVal;
    var_val_t *tempVal = GetFactor();
    if (tempVal->type != itl_intType) {
	(void) itlValue_VarValConvert(tempVal, itl_intType);
	retVal = *((int *)tempVal->value);
    } else {
	retVal = *((int *)tempVal->value);
    }
    itl_FreeVarVal(tempVal);
  return(retVal);
}

/*
 * Exported version of getinteger routine for use by
 * command procedures 
 *
 */
EXPORT int itl_GetInteger(errP)
OUT boolean_t *errP;
{
    int retVal;
    var_val_t *tempVal = GetFactor();
    if (tempVal == NULL) {
	retVal = 0;
        if(errP) *errP = TRUE;
    }
    else {
        if(errP) *errP = FALSE;
	if (tempVal->type != itl_intType) {
	    (void) itlValue_VarValConvert(tempVal, itl_intType);
	    retVal = *((int *)tempVal->value);
	} else {
	    retVal = *((int *)tempVal->value);
	}
	itl_FreeVarVal(tempVal);
    }
  return(retVal);
}


/*
 *  itl_GetString:
 *	Get a string from the input interpreting variables if necessary.
 */
char *itl_GetString(eof, len)
  INOUT boolean_t *eof;
  INOUT int *len;
{
    char *retVal;
    int size;
    var_val_t *tempVal = itlValue_Read(NULL, itl_stringType);
    if (!tempVal) {
	if (eof)
	    *eof = TRUE;
	retVal = NULL;
	size = 0;
    } else {
	if (eof)
	    *eof = FALSE;
	retVal = STR_VALUE_VAL(tempVal);
	size = VALUE_SIZE(tempVal)-1;
	ITL_FREE(tempVal);
    }
    if (len)
	*len = size;
    return(retVal);
}

/*
 *   itl_GetVarVal - Get a variable value.
 */
EXPORT var_val_t *itl_GetVarVal()
{
    var_val_t *retVal = GetFactor();
    return(retVal);
}

/*
 *   itl_PeekVarVal - Peek at the next variable value.
 */
EXPORT var_val_t *itl_PeekVarVal()
{
    var_val_t *retVal;
    int startOffset, startIndex;

    itlIO_CurrentHistLoc(&startOffset, &startIndex);
    retVal = GetFactor();
    itlIO_GotoHistLoc(startOffset, startIndex);
    return(retVal);
}

/*
 *  That routine that is called by the CHT forAll routine to
 *  clean up the variable table.
 */
boolean_t cleanupExpIterator(varP, bufferP)
IN itl_var_t *varP;
IN char *bufferP;
{
   varTable_Del(varP, CHT_LATCH_NONE);
   freeVariable(varP);
   return(TRUE);
}
/*
 *  itlExp_Cleanup():
 *    Clean up this module:  Free all the memory that was allocated
 *    for variables.
 */
EXPORT void itlExp_Cleanup()
{
    (void) varTable_Forall(cleanupExpIterator,
			   NULL,
			   CHT_LATCH_BRACKET);
}

/*
 * itlExp_Init() -- 
 *    Initializes the expressions module.  Needed for pthread 
 *    implementation so that thread data areas could be safely
 *    generated.
 *
 * ASSUMPTIONS:
 *    This routine is only called once, and non-concurrently via the
 *    itl_InitItl() function.
 *
 * RETURN CODES:
 *    FALSE - success
 *    TRUE  - error
 */
SHARED boolean_t itlExp_Init()
{
    int i;
    boolean_t returnVal = FALSE;

    itl_Trace(ITL_VERBOSE_TRACE,
	      "Initializing the variable table.\n\t%s\n",
	      RCSid);
    varTable_Init();
    itlValue_Init();
    (void) itlExp_UseStandardTypeDef();

    /*
     *  Compute the highest precedence defined.
     */
    highestPrecedence = -1;
    for (i=0; i<256; i++)
	firstOpTab[i] = -1;
    for (i=0; i<OP_TABLE_SIZE; i++) {
	if (highestPrecedence < opDefTable[i].precedence)
	    highestPrecedence = opDefTable[i].precedence;
	if (firstOpTab[opDefTable[i].opStr[0]] == -1)
	    firstOpTab[opDefTable[i].opStr[0]] = i;
    }
    for (i=0; i<256; i++)
	if (firstOpTab[i] == -1)
	    firstOpTab[i] = 0;

    if (pthread_keycreate(&varListKey, (pthread_destructor_t) free) != 0) 
    {
	itl_Error("varListKey creation failed in itl_InitExp, errno = %d\n", 
		  errno);
	returnVal = TRUE;
    }
    if (pthread_keycreate(&localVarListKey, (pthread_destructor_t) free) != 0) 
    {
	itl_Error("localVarListKey creation failed in itl_InitExp, errno = %d\n",
		  errno);
	returnVal = TRUE;
    }
    /*
     *  Register thread callbacks for initializing procedure local vars.
     */
    itl_AddThreadState(ThreadInitVarLocals,
		       ThreadStartVarLocals,
		       ThreadCleanVarLocals);

    return(returnVal);
}
