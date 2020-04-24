/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdsext.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:36:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:22  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:20:31  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:03  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:08  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:38  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:58  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:13  marrek]
 * 
 * 	Change rcsid string format.
 * 	[1994/02/09  08:13:15  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:55:17  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XDS Convenience Functions                         */
/* File Name   : xdsext.h					   */
/* Description : This file contains all the defines and function   */
/*		 prototypes for the public convenience routines.   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XDSEXT_H
#define _XDSEXT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdsext_rcsid[] = "@(#)$RCSfile: xdsext.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:54 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
OM_return_code 
dsX_extract_attr_values(
    OM_private_object     object,	  /* IN  - The source object 	      */
    OM_object_identifier  attribute_type, /* IN  - Attribute to be extracted  */
    OM_boolean            local_strings,  /* IN  - Local strings required     */
    OM_public_object     *values,	  /* OUT - Extracted Attribute Values */
    OM_value_position    *total_number);  /* OUT - Number of extracted values */

#endif  /* ifndef _XDSEXT_H */
