/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * ID: $Id: itl_vectype.h,v 1.1.316.1 1996/10/17 18:28:00 damon Exp $ 
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
 * $Log: itl_vectype.h,v $
 * Revision 1.1.316.1  1996/10/17  18:28:00  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:04  damon]
 *
 * Revision 1.1.311.1  1994/02/04  20:45:57  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:48  devsrc]
 * 
 * Revision 1.1.309.1  1993/12/07  17:45:03  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:31:43  jaffe]
 * 
 * Revision 1.1.2.2  1992/11/18  22:01:13  jaffe
 * 	New File
 * 
 * 	$TALog: itl_vectype.h,v $
 * 	Revision 1.9  1994/11/08  16:12:13  cfe
 * 	[merge of changes from 1.5 to 1.8 into 1.7]
 * 
 * 	[*** log entries omitted ***]
 * 
 * Revision 1.7  1994/09/15  18:24:16  jess
 * 	Use inlValue_Free instead of OTL_FREE.
 * 
 * 	See above.
 * 	[from r1.6 by delta jess-db6005_fix_itl_mem_leaking, r1.2]
 * 
 * Revision 1.6  1994/09/15  15:20:50  jess
 * 	This delta fixes the ITL memory leak problem from vector component.
 * 
 * 	See above.
 * 	[from r1.5 by delta jess-db6005_fix_itl_mem_leaking, r1.1]
 * 
 * Revision 1.5  1993/03/08  13:52:06  fred
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
 * 	[from r1.4 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 * 
 * Revision 1.4  1993/01/15  14:50:41  jaffe
 * 	add correct history and copyright
 * 	[from r1.3 by delta osf-revdrop-01-12-92, r1.2]
 * 
 * Revision 1.3  1992/11/23  16:16:54  jaffe
 * 	Put in missing header after latest drop to the OSF.
 * 	[from revision 1.2 by delta osf-sync-with-nov-19-drop, r1.1]
 * 
 * 	Revision 1.2  1992/11/04  13:04:34  fred
 * 	Add support for binary data (the ITL vector type) into the read and write
 * 	routines for the file interface library.
 * 
 * 	Exported itl_vectorType definition.
 * 	[from revision 1.1 by delta fred-itl-support-binary-data, r1.1]
 * 
 * Revision 1.1  1992/11/03  15:28:02  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 	[added by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.2  1992/10/29  15:41:51  oz
 * 	Fixed small typos and added comments.
 * 	[from revision 1.1 by delta oz-5124-ITL-add-vector-type, r1.3]
 * 
 * Revision 1.1  1992/10/29  15:25:42  oz
 * 	[added by delta oz-5124-ITL-add-vector-type, r1.2]
 * 
 * $EndLog$
 */
#ifndef TRANSACRC_ITL_VECTYPE_H
#define TRANSACRC_ITL_VECTYPE_H 1

/*
 *  This module defines and exports a byte-array (or vector) type to ITL
 *
 *	The type name is 'vector'.  This module also defines how to convert
 *	integers and strings to vectors:
 *	an integer is converted to a 4 byte vector containing the same data.
 *	For strings: if the string starts with the characters 0x (or 0X)
 *	the string is interpreted as hex data where spaces, tabs and dashes
 *	are ignored.  Otherwised, the string is used as the data (without
 *	the terminating null
 *
 *  Examples
 *	set foo vector 40
 *	  	will result in a vector of 4 bytes: 
 *		> foo = (4) 0000-0028- (vector) 
 *	set foo vector "Hello World" 
 *		will result in an 11 byte vector:
 *		> foo = (11) 4865-6c6c-6f20-576f-726c-64 (vector) - 
 *	set foo vector "0x 11 22 33 44 a1 b1 b2"
 *		will result in a vector of 7 bytes:
 *		> foo = (7) 1122-3344-a1b1-b2 (vector)
 *
 *  Binary operations are similar to those defined for strings.
 *
 *
 *  Accessing the data
 *	The vector data is stored in a structure of type itl_vectorType_t.
 *	In order to access the data the user should call the macro
 *	ITL_VEC_VALUE_DATA.  This gives a pointer to the start of the data
 *	and the pointer is of type char *.  In order to get the size of
 *	a vector value, use the macro VALUE_SIZE(valP).
 */

/*
 *  ITL_MAGIC_NULL_STR
 *	The NULL token: if provided as the data to the itl
 *	vector type will result in a null vector (which is 
 *	different from the  EMPTY vector ("").
 */
#define ITL_MAGIC_NULL_STR   "NULL"

extern int itl_vectorType;

extern void itl_VecTypeInit _TAKES(( void ));

extern var_val_t *itl_SetVectorType _TAKES((
  IN	char	*value,
  IN	int	 size
));

/*
 *  itlVector_Value
 *	Return the data portion of a vector type.
 */
extern pointer_t itlVector_Value _TAKES(( IN var_val_t *valP ));

#endif  /*  TRANSACRC_ITL_VECTYPE_H  */
