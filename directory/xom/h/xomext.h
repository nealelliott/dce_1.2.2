/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xomext.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:39:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:55  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:21:44  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:25  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:17  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:10  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:34  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:13:30  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:55:35  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xomext.h					   */
/* Description : This file contains all the defines and function   */
/*		 prototypes for the public convenience routines	   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOMEXT_H
#define _XOMEXT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xomext_rcsid[] = "@(#)$RCSfile: xomext.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:31 $";
#endif

/*-- Defines for error returns related to XOI ------------------------------  */
#define	OMX_SUCCESS	           ((OM_integer)  0)
#define OMX_CANNOT_READ_SCHEMA     ((OM_integer) -1)
#define OMX_SCHEMA_NOT_READ	   ((OM_integer) -2)
#define OMX_NO_START_OBJ_BLOCK	   ((OM_integer) -3)
#define OMX_NO_END_OBJ_BLOCK	   ((OM_integer) -4)
#define OMX_EMPTY_OBJ_BLOCK	   ((OM_integer) -5)
#define OMX_OBJ_FORMAT_ERROR	   ((OM_integer) -6)
#define OMX_DUPLICATE_OBJ_ABBRV    ((OM_integer) -7)
#define OMX_DUPLICATE_OBJ_OBJ_ID   ((OM_integer) -8)
#define OMX_NO_START_ATTR_BLOCK	   ((OM_integer) -9)
#define OMX_NO_END_ATTR_BLOCK	   ((OM_integer) -10)
#define OMX_EMPTY_ATTR_BLOCK	   ((OM_integer) -11)
#define OMX_ATTR_FORMAT_ERROR	   ((OM_integer) -12)
#define OMX_DUPLICATE_ATTR_ABBRV   ((OM_integer) -13)
#define OMX_DUPLICATE_ATTR_OBJ_ID  ((OM_integer) -14)
#define OMX_NO_START_CLASS_BLOCK   ((OM_integer) -15)
#define OMX_NO_END_CLASS_BLOCK	   ((OM_integer) -16)
#define OMX_EMPTY_CLASS_BLOCK	   ((OM_integer) -17)
#define OMX_CLASS_FORMAT_ERROR     ((OM_integer) -18)
#define OMX_NO_CLASS_NAME          ((OM_integer) -19)
#define OMX_DUPLICATE_CLASS_BLOCK  ((OM_integer) -20)
#define OMX_CLASS_BLOCK_UNDEFINED  ((OM_integer) -21)
#define OMX_INVALID_ABBRV          ((OM_integer) -22)
#define OMX_INVALID_OBJ_ID	   ((OM_integer) -23)
#define OMX_INVALID_CLASS_NAME     ((OM_integer) -24)
#define OMX_INVALID_SYNTAX     	   ((OM_integer) -25)
#define OMX_MEMORY_INSUFFICIENT    ((OM_integer) -26)
#define OMX_INVALID_PARAMETER	   ((OM_integer) -27)
#define OMX_UNKNOWN_ABBRV	   ((OM_integer) -28)
#define OMX_UNKNOWN_OBJ_ID	   ((OM_integer) -29)
#define OMX_UNKNOWN_OMTYPE	   ((OM_integer) -30)

/*-- Defines for error returns related to convenience library ----------------*/
#define OMX_MISSING_AVA  	   ((OM_integer) -101)
#define OMX_MISSING_ABBRV	   ((OM_integer) -102)
#define OMX_FORMAT_ERROR 	   ((OM_integer) -103)
#define OMX_UNKNOWN_ERROR	   ((OM_integer) -104)
#define OMX_MISSING_RDN_DELIMITER  ((OM_integer) -105)
#define OMX_MISMATCHED_QUOTES      ((OM_integer) -106)
#define OMX_MISSING_EQUAL_OPERATOR ((OM_integer) -107)
#define OMX_MISSING_ATTR_VALUE     ((OM_integer) -108)
#define OMX_MISSING_ATTR_INFO      ((OM_integer) -109)
#define OMX_MISSING_CLASS_START_OP ((OM_integer) -110)
#define OMX_MISSING_CLASS_END_OP   ((OM_integer) -111)
#define OMX_MISSING_CLASS_VALUE    ((OM_integer) -112)
#define OMX_MISSING_COMP_VALUE     ((OM_integer) -113)
#define OMX_MISMATCHED_BRACKETS    ((OM_integer) -114)
#define OMX_UNEXPECTED_OPERATOR    ((OM_integer) -115)
#define OMX_WRONG_VALUE		   ((OM_integer) -116)
#define OMX_UNKNOWN_KEYWORD	   ((OM_integer) -117)
#define OMX_MISSING_OPERATOR	   ((OM_integer) -118)
#define OMX_MISSING_COMPOUND_OP    ((OM_integer) -119)

/*-- Additional Errors returned by the omX_object_to_string function ---*/
#define OMX_CLASS_NOT_FOUND_IN_SCHEMA_FILE  ((OM_return_code) 31)

/*-- Function Prototypes ------------------------------------------*/
OM_return_code omX_fill(
    OM_type	      type,         /* IN  - Type of Object	      */
    OM_syntax	      syntax,       /* IN  - Syntax of the object     */
    OM_uint32	      length,       /* IN  - Data length	      */
    void	     *elements,     /* IN  - Data Value 	      */
    OM_descriptor    *destination); /* OUT - The filled up descriptor */

OM_return_code omX_fill_oid(
    OM_type	          type,         /* IN  - Type of Object	          */
    OM_object_identifier  object_id,    /* IN  - Value of the object      */
    OM_descriptor        *destination); /* OUT - The filled up descriptor */

OM_return_code omX_extract(
    OM_private_object  object,          /* IN  - Extract from this object    */
    OM_type_list       navigation_path, /* IN  - Leads to the target object  */
    OM_exclusions      exclusions,      /* IN  - Scope of extraction         */
    OM_type_list       included_types,  /* IN  - Objects to be extracted     */
    OM_boolean         local_strings,   /* IN  - Local strings required      */
    OM_value_position  initial_value,   /* IN  - First value to be extracted */
    OM_value_position  limiting_value,  /* IN  - Last value to be extracted  */
    OM_public_object  *values,	 	/* OUT - Array of extracted objects  */
    OM_value_position *total_number);	/* OUT - Count of extracted objects  */

OM_return_code omX_string_to_object(
    OM_workspace          workspace,     /* IN  - The workspace		      */
    OM_string            *string,        /* IN  - The string to be converted  */
    OM_object_identifier  class,         /* IN  - The OM Class to be created  */
    OM_boolean            local_strings, /* IN  - Local strings specified     */
    OM_private_object    *object,        /* OUT - The converted Object	      */
    OM_integer	         *error_position,/* OUT - Error Position in I/P string*/
    OM_integer	         *error_type);   /* OUT - Type of error 	      */

OM_return_code omX_object_to_string(
    OM_object    object,        /* The Object to be converted          */
    OM_boolean	 local_strings, /* To indicate local string conversion */
    OM_string	*string);       /* The converted DN string 	       */

#endif  /* ifndef _XOMEXT_H */
