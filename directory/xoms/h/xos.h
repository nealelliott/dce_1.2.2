/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xos.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:58  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:19  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:21:57  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:34  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:24  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:21  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:11  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:14:31  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:56:47  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xos.h					   	   */
/* Description : This file contains all the defines and function   */
/*		 prototypes for the functions in file xos.c	   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOS_H
#define _XOS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xos_rcsid[] = "@(#)$RCSfile: xos.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:58 $";
#endif

/*-- Function Prototypes ------------------------------------------*/

OM_return_code xos_string_to_object(
    OM_workspace          workspace,     /* IN  - The workspace		      */
    OM_string            *string,        /* IN  - The string to be converted  */
    OM_object_identifier  class,         /* IN  - The OM Class to be created  */
    OM_boolean            local_strings, /* IN  - To specify local strings    */
    OM_private_object    *object,        /* OUT - The converted Object	      */
    OM_integer	         *error_position,/* OUT - Error Position in I/P string*/
    OM_integer	         *error_type);   /* OUT - Type of error 	      */

OM_return_code xos_object_to_string(
    OM_object    object,        /* The Object to be converted          */
    OM_boolean	 local_strings, /* To indicate local string conversion */
    OM_string	*string);       /* The converted DN string 	       */

#endif  /* ifndef _XOS_H */
