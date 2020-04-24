/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosclass.h,v $
 * Revision 1.1.4.2  1996/03/11  13:25:00  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:22  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:22:33  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:36  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:26  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:27  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:13  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:14:46  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:57:05  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosclass.h					   */
/* Description : This file contains all the function prototypes    */
/*		 for the routines in xosclass.c 		   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOSCLASS_H
#define _XOSCLASS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xosclass_rcsid[] = "@(#)$RCSfile: xosclass.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:00 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
OM_return_code xos_create_struct_attr_obj(
    OM_string  *objid,        /* IN  - Attribute Object Identifier            */
    OM_string  *omclass,      /* IN  - Class Identifier                       */
    char       *value,	      /* IN  - Attribute Value string	              */
    OM_uint32   value_len,    /* IN  - Length of Value string	              */
    char       *val_mem_top,  /* IN  - Memory heap to store attribute values  */
    OM_boolean  is_filter,    /* IN  - Boolean flag to indicate filter item   */
    OM_boolean  local_strings,/* IN  - To specify local strings    	      */
    OM_object  *object,       /* OUT - The created object	              */
    OM_integer *error_pos,    /* OUT - Error position in case of errors       */
    OM_integer *error_type);  /* OUT - Type of error in case of errors        */

OM_return_code xos_get_class_value_data(
    char       *value,       /* IN  - The input component value string 	      */
    OM_uint32   value_len,   /* IN  - The length of the input string   	      */
    OM_boolean  delim_reqd,  /* IN  - Whether class delimiters ("{","}") reqd.*/
    OM_uint32  *class_len,   /* OUT - Length of the class value        	      */
    OM_uint    *comp_count,  /* OUT - Total number of components in the class */
    OM_integer *error_pos,   /* OUT - Error position in case of errors 	      */
    OM_integer *error_type); /* OUT - Type of error in case of errors  	      */

OM_return_code xos_str_to_obj_create_class(
    OM_string  *omclass,       /* IN  - Class Identifier                      */
    OM_string  *string,        /* IN  - The string to be converted 	      */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings    	      */
    OM_object  *object,        /* OUT - The created object	              */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type);   /* OUT - Type of error in case of errors       */

void xos_free_class_object(
    OM_object object);  /* IN - The object to be freed */

OM_return_code xos_convert_class_to_string(
    OM_object    object,       /* IN  - The object to be converted            */
    OM_boolean   local_strings,/* IN  - To indicate local string conversion   */
    OM_uint32    max_len,      /* IN  - Maximum length of the converted string*/
    OM_boolean   delim_reqd,   /* IN  - Whether Class Delimiters "{","}" reqd.*/
    char        *string,       /* OUT - The converted string                  */
    OM_uint32   *len_used);    /* OUT - Number of chars. written to O/P string*/

#endif  /* ifndef _XOSCLASS_H */
