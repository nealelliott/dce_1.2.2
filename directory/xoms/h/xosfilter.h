/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosfilter.h,v $
 * Revision 1.1.4.2  1996/03/11  13:25:02  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:17  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:26  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:22:35  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:38  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:29  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:15  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:15:51  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:57:21  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosfilter.h					   */
/* Description : This file contains all the function prototypes    */
/*		 for the routines in xosfilter.c		   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOSFILTER_H
#define _XOSFILTER_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xosfilter_rcsid[] = "@(#)$RCSfile: xosfilter.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:02 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
OM_return_code xos_to_public_filter_object(
    OM_string  *string,        /* IN  - The string to be converted 	      */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings    	      */
    OM_object  *filter,        /* OUT - The converted filter object 	      */
    OM_integer *error_pos,     /* OUT - Error Position in case of errors      */
    OM_integer *error_type);   /* OUT - Type of error in case of errors	      */

void xos_free_filter_object(
    OM_object filter);  /* IN - The filter object to be freed */

#endif  /* ifndef _XOSFILTER_H */
