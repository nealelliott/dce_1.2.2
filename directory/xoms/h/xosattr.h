/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosattr.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:59  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:20  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:22:31  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:35  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:24  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:12  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:14:39  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:56:56  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosattr.h					   */
/* Description : This file contains all the function prototypes    */
/*		 for the routines in xosattr.c 		   	   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOSATTR_H
#define _XOSATTR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xosattr_rcsid[] = "@(#)$RCSfile: xosattr.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:59 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
OM_return_code xos_to_public_attr_object(
    OM_string  *attr_string,   /* IN  - The string to be converted            */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings    	      */
    OM_object  *attr_obj,      /* OUT - The converted public object           */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type);   /* OUT - Type of error in case of errors       */

OM_return_code xos_create_normal_attr_obj(
    OM_string  *objid,        /* IN  - Attribute Object Identifier            */
    OM_syntax   omsyntax,     /* IN  - Syntax of the Attribute	              */
    char       *value,	      /* IN  - Attribute Value string	              */
    OM_uint32   value_len,    /* IN  - Length of Value string	              */
    char       *val_mem_top,  /* IN  - Memory heap to store attribute values  */
    OM_boolean  is_filter,    /* IN  - Boolean flag to indicate filter item   */
    OM_boolean  local_strings,/* IN  - To specify local strings    	      */
    OM_object  *attr_obj,     /* OUT - The created object	              */
    OM_integer *error_pos,    /* OUT - Error position in case of errors       */
    OM_integer *error_type);  /* OUT - Type of error in case of errors        */

OM_return_code xos_convert_public_attr_to_str(
    OM_object   object,        /* IN  - The Object to be converted 	    */
    OM_boolean  local_strings, /* IN  - To indicate local string conversion */
    OM_string  *string);       /* OUT - The converted string       	    */

void xos_free_attr_object(
    OM_object attr_object);  /* IN - The object to be freed */

#endif  /* ifndef _XOSATTR_H */
