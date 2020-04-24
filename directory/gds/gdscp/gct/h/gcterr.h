/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gcterr.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:46  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:01  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:00  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:27  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:58:08  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:37:02  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:17  keutel
 * 	creation
 * 	[1994/03/21  12:55:44  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program				   */
/* File Name   : gcterr.h					   */
/* Description : This file contains all the prototypes for the 	   */
/*		 functions defined in file gcterr.c		   */
/* Date        : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCTERR_H
#define _GCTERR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gcterr_rcsid[] = "@(#)$RCSfile: gcterr.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:10 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
signed16 gc_t_map_xds_error(
    DS_status     error_xds,     /* IN  - The XDS error to be converted     */
    boolean       local_strings, /* IN  - To specify local strings          */
    gc_t_vector **error_vect);   /* OUT - The converted error string vector */

signed16 gc_t_map_xom_error(
    OM_return_code   om_ret_code,  /* IN  - The XOM error to be converted     */
    gc_t_vector    **error_vect);  /* OUT - The converted error string vector */

signed16 gc_t_map_xos_error(
    OM_integer    error,        /* IN  - The error to be converted         */
    char         *err_string,   /* IN  - Additional string needed in output*/
    OM_integer    error_pos,    /* OUT - 0 or error position               */
    gc_t_vector **error_vect);  /* OUT - The converted error string vector */

signed16 gc_t_map_xoi_error(
    OM_integer    error,        /* IN  - The error to be converted            */
    char         *append_str,   /* IN  - String to be appended to error string*/
    gc_t_vector **error_vect);  /* OUT - The converted error string vector    */

signed16 gc_t_map_xoi_schema_file_error(
    OM_integer    error,        /* IN  - The error to be converted            */
    OM_uint       line_no,      /* IN  - The error line number in schema file */
    gc_t_vector **error_vect);  /* OUT - The converted error string vector    */

signed16 gc_t_create_gct_error_vector(
    signed16      error,        /* IN  - The error to be converted            */
    char         *append_str,   /* IN  - String to be appended to error string*/
    gc_t_vector **error_vect);  /* OUT - The converted error string vector    */

signed16 gc_t_create_gct_arg_error_vector(
    signed16      error,        /* IN  - The error to be converted            */
    char         *append_str,   /* IN  - String to be appended to error string*/
    gc_t_vector **error_vect);  /* OUT - The converted error string vector    */

signed16 gc_t_create_partial_qual_error(
    OM_object      partial_obj,  /* IN  - The partial outcome qualifier object*/
    boolean        local_strings,/* IN  - To specify local strings            */
    gc_t_vector  **error_vect);  /* OUT - Error message in case of errors     */

#endif  /* ifndef _GCTERR_H */
