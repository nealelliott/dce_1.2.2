/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosutil.h,v $
 * Revision 1.1.4.2  1996/03/11  13:25:05  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:19  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:30  root]
 * 
 * Revision 1.1.2.4  1994/06/21  14:49:04  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:49:27  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:22:39  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:41  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:28  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:34  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:18  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:18:12  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:57:47  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosutil.h					   */
/* Description : This file contains all the function prototypes    */
/*		 for the routines in xosutil.c 			   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOSUTIL_H
#define _XOSUTIL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xosutil_rcsid[] = "@(#)$RCSfile: xosutil.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:05 $";
#endif

#include <nl_types.h>

/*-- Function Prototypes ------------------------------------------*/
OM_integer xos_map_xoi_error(
    OM_integer xoi_error);  /* IN - The XOI error to be mapped */

OM_return_code xos_convert_to_normalized_string(
    char  	*in_str,      /* IN  - The input string		              */
    OM_uint32    in_len,      /* IN  - Length of the input string	      */
    char   	 quote_char,  /* IN  - Quote character used to delimit values */
    char        *out_str,     /* OUT - Output converted string		      */
    OM_uint32   *out_len,     /* OUT - Length of the output string	      */
    OM_integer  *error_pos,   /* OUT - Error position in case of errors       */
    OM_integer  *error_type); /* OUT - Type of error in case of errors	      */

OM_return_code xos_convert_to_private(
    OM_object		  public_obj,   /* IN  - Object to be converted */
    OM_object_identifier  class,        /* IN  - Class of the object    */
    OM_workspace          workspace,    /* IN  - The workspace          */
    OM_private_object    *private_obj); /* OUT - The converted object   */

OM_return_code xos_fill_descriptor(
    OM_type     type,          /* IN  - Type of Object           */
    OM_syntax   syntax,	       /* IN  - Syntax of the object     */
    OM_uint32   length,	       /* IN  - Data length              */
    void       *elements,      /* IN  - Data Value               */
    OM_boolean  local_strings, /* IN  - To specify local strings */
    OM_object   destination);  /* OUT - The filled up descriptor */

OM_return_code xos_map_xds_error_to_string(
    nl_catd         nls_desc,      /* IN  - Pointer to the message catalog */
    OM_enumeration  enum_error,    /* IN  - The enumerated error value     */
    OM_object	    object,        /* IN  - DS_C_ATTRIBUTE_ERROR or NULL   */
    OM_string	   *string);       /* OUT - The converted string	   */

#endif  /* ifndef _XOSUTIL_H */
