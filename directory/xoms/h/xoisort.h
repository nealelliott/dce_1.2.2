/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xoisort.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:55  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:15  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:54  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:32  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:07  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:14:09  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:56:21  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOI - XOM Object Info Module                      */
/* File Name   : xoisort.h					   */
/* Description : This file contains all the function prototypes    */
/*		 for the routines in xoisort.c			   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOISORT_H
#define _XOISORT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xoisort_rcsid[] = "@(#)$RCSfile: xoisort.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:55 $";
#endif

#include <xoistruct.h>

/*-- Function Prototypes ------------------------------------------*/
void xoi_free_sort_structures(
    void);

OM_integer xoi_sort_info(
    void);

OM_integer xoi_get_oinfo_from_abbrv(
    char       *abbrv,   /* IN  - Object Class Abbreviation	     */
    xoi_oinfo **obj);    /* OUT - Pointer to the xoi_oinfo structure */

OM_integer xoi_get_oinfo_from_objid(
    OM_string   *e_objid, /* IN  - Encoded Object Class Identifier    */
    xoi_oinfo  **obj);	  /* OUT - Pointer to the xoi_oinfo structure */

OM_uint xoi_get_oinfo_system_index(
    OM_uint  index);  /* IN - Index to the sorted array */

OM_integer xoi_get_ainfo_from_objid(
    OM_string   *e_objid, /* IN  - Encoded Attribute Object Identifier  */
    xoi_ainfo  **attr);   /* OUT - Pointer to the xoi_ainfo structure   */

OM_integer xoi_get_ainfo_from_abbrv(
    char       *abbrv,   /* IN  - Attribute Abbreviation	     */
    xoi_ainfo **attr);   /* OUT - Pointer to the xoi_ainfo structure */

OM_uint xoi_get_ainfo_system_index(
    OM_uint  index);  /* IN - Index to the sorted array */

OM_integer xoi_get_cinfo_from_abbrv(
    OM_string  *omclass,   /* IN  - OM Class Identifier		       */
    char       *abbrv,     /* IN  - Abbreviation of the component      */
    xoi_cinfo **comp);	   /* OUT - Pointer to the xoi_cinfo structure */

OM_integer xoi_get_cinfo_from_omtype(
    OM_string  *omclass,   /* IN  - OM Class Identifier		       */
    OM_type     omtype,    /* IN  - OM_type of the component           */
    xoi_cinfo **comp);     /* OUT - Pointer to the xoi_cinfo structure */

#endif  /* ifndef _XOISORT_H */
