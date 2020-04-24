/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosname.h,v $
 * Revision 1.1.4.2  1996/03/11  13:25:03  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:18  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:27  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:22:37  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:39  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:32  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:16  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:16:28  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:57:29  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                     	   */
/* File Name   : xosname.h					   */
/* Description : This file contains all the function prototypes    */
/*		 for the routines in xosname.c 			   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOSNAME_H
#define _XOSNAME_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xosname_rcsid[] = "@(#)$RCSfile: xosname.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:03 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
OM_return_code xos_convert_str_to_public_name(
    OM_string  *dn_string,     /* IN  - The string to be converted            */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  is_rdn,        /* IN  - Indicates that an RDN is to be created*/
    OM_boolean  local_strings, /* IN  - To specify local strings    	      */
    OM_object  *name,          /* OUT - The converted DN                      */
    OM_integer *error_pos,     /* OUT - Error Position in case of errors      */
    OM_integer *error_type);   /* OUT - Type of error in case of errors       */

OM_return_code xos_convert_public_name_to_str(
    OM_object   dn_object,     /* IN  - The Dist.Name to be converted       */
    OM_boolean  local_strings, /* IN  - To indicate local string conversion */
    OM_string  *string);       /* OUT - The converted DN string             */

OM_return_code xos_convert_public_rdn_to_str(
    OM_object   rdn_object,    /* IN  - The Rel.Dist.Name to be converted   */
    OM_boolean  local_strings, /* IN  - To indicate local string conversion */
    OM_string  *string);       /* OUT - The converted RDN string            */

void xos_free_dn_object(
    OM_object dn_object);   /* IN - The DN Object to be freed */

void xos_free_rdn_object(
    OM_object rdn_object);  /* IN - The RDN Object to be freed */

#endif  /* ifndef _XOSNAME_H */
