/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xoiinit.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:54  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:11  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:14  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:52  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:31  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:05  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:14:01  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:56:11  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOI - XOM Object Info Module                      */
/* File Name   : xoiinit.h					   */
/* Description : This file contains all the function prototypes    */
/*		 for the routines in xoiinit.c			   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOIINIT_H
#define _XOIINIT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xoiinit_rcsid[] = "@(#)$RCSfile: xoiinit.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:54 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
xoi_object_class *xoi_get_objclass_ptr(
    void);

xoi_attribute *xoi_get_attribute_ptr(
    void);

xoi_omclass_struct *xoi_get_omclass_ptr(
    void);

void xoi_free_blockinfo(
    void);

OM_integer xoi_init(
    OM_uint *line_no); /* OUT - Contains erroneous line No.in case of errors */

#endif  /* ifndef _XOIINIT_H */
