/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xoidebug.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:53  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:12  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:51  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:30  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:03  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:13:54  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:56:02  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOI - XOM Object Info Module                      */
/* File Name   : xoidebug.h					   */
/* Description : This file contains all the defines and function   */
/* 		 prototypes for the debug routines in xoidebug.c   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOIDEBUG_H
#define _XOIDEBUG_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xoidebug_rcsid[] = "@(#)$RCSfile: xoidebug.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:53 $";
#endif

#include <xoisort.h>

/*-- Function Prototypes ------------------------------------------*/
void xoi_debug_string(
    char *message);	/* IN - The string to be printed   */

void xoi_debug_objclass_block(
    void);

void xoi_debug_attribute_block(
    void);

void xoi_debug_obj_sortinfo(
    xoi_obj_abbrv_ord *abbrv_array,  /* IN - Pointer to sorted Abbreviations */ 
    xoi_obj_id_ord    *objid_array); /* IN - Pointer to sorted Object Ids.   */

void xoi_debug_attr_sortinfo(
    xoi_attr_abbrv_ord *abbrv_array,  /* IN - Pointer to sorted Abbreviations */
    xoi_attr_id_ord    *objid_array); /* IN - Pointer to sorted Object Ids.   */

void xoi_debug_omclass_sortinfo(
    xoi_omclass_ord  *omclass_array); /* IN - Pointer to sorted OM Classes */

#endif  /* ifndef _XOIDEBUG_H */
