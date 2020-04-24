/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xoiutil.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:57  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:13  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:17  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:56  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:33  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:11  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:14:24  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:56:38  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOI - XOM Object Info Module                      */
/* File Name   : xoiutil.h					   */
/* Description : This file contains all the prototypes for the	   */
/*		 functions in file xoiutil.c			   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOIUTIL_H
#define _XOIUTIL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xoiutil_rcsid[] = "@(#)$RCSfile: xoiutil.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:57 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
void xoi_free_omstring(
    OM_string *omstring); /* IN - String to be freed  */

void xoi_strcpy_to_lower(
    char *dst,    /* OUT - Converted String */
    char *src);   /* IN  - Input String     */

void xoi_strcpy_to_upper(
    char *dst,    /* OUT - Converted String */
    char *src);   /* IN  - Input String     */

OM_uint xoi_get_next_line(
    FILE *fileptr,      /* IN  - File Pointer		  */
    char *data_buffer); /* OUT - Line read from the file  */

OM_integer xoi_validate_abbrv(
    char *abbrv);  /* IN - Abbreviation */

OM_integer xoi_to_omclass(
    char       *str,      /* IN  - String to be converted */
    OM_string **omclass); /* OUT - Converted OM_string    */

OM_integer xoi_get_omclass_define(
    OM_string  *omclass,  /* IN  - OM Class string      */
    char       *str);     /* OUT - OM Class Name string */

OM_integer xoi_to_omsyntax(
    char      *str,       /* IN  - String to be converted */
    OM_syntax *omsyntax); /* OUT - Converted OM_syntax    */

char *xoi_strdup(
    char *str);  /* IN - Input String to be duplicated */

OM_string *xoi_create_omstring(
    char       *str,   /* IN - Input String		  */
    OM_uint32   len);  /* IN - Length of the input string */

#endif  /* ifndef _XOIUTIL_H */
