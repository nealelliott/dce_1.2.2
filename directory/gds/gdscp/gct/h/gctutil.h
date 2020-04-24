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
 * $Log: gctutil.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:01  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:52  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:04  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:05  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:31  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:58:18  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:37:04  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:24  keutel
 * 	creation
 * 	[1994/03/21  12:55:54  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program				   */
/* File Name   : gctutil.h					   */
/* Description : This file contains all the prototypes for the 	   */
/*		 functions defined in file gctutil.c and utility   */
/*		 macros.					   */
/* Date        : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCTUTIL_H
#define _GCTUTIL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gctutil_rcsid[] = "@(#)$RCSfile: gctutil.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:14 $";
#endif

/*-- Utility macros -----------------------------------------------*/
/* To initialize a NULL descriptor */
#define GC_T_FILL_OMD_NULL(desc) \
	omX_fill(OM_NO_MORE_TYPES, OM_S_NO_MORE_SYNTAXES, 0,		\
		 OM_ELEMENTS_UNSPECIFIED, desc)

/*-- Function Prototypes ------------------------------------------*/
signed16 gc_t_create_full_dn_string(
    char      *rdn_string,  /* IN  - The RDN string           */
    OM_string *full_dn);    /* OUT - The created full DN      */

signed16 gc_t_string_to_name(
    char	       *dn_string,    /* IN  - The string to be converted     */
    boolean             local_strings,/* IN  - To specify local strings       */
    OM_private_object  *name,         /* OUT - The converted object           */
    gc_t_vector       **error_vect);  /* OUT - Error message in case of errors*/

signed16 gc_t_string_to_object(
    char                 *string,        /* IN  - The string to be converted */
    OM_object_identifier  class,         /* IN  - The OM Class to be created */
    boolean               local_strings, /* IN  - To specify local strings   */
    OM_private_object    *object,        /* OUT - The converted object       */
    gc_t_vector         **error_vect);   /* OUT - Error messages, if errors  */

signed16 gc_t_string_to_filter(
    char               *string,       /* IN  - The string to be converted     */
    boolean             local_strings,/* IN  - To specify local strings       */
    OM_private_object  *filter,       /* OUT - The converted filter object    */
    gc_t_vector       **error_vect);  /* OUT - Error message in case of errors*/

signed16 gc_t_convert_to_normalized_string(
    char       *in_str,      /* IN  - The input string                 */
    signed32    in_len,      /* IN  - Length of the input string       */
    OM_string  *out_str,     /* OUT - The converted string             */
    signed32   *error_pos);  /* OUT - Error position in case of errors */

signed16 gc_t_fill_with_objid(
    char           *abbrv,     /* IN  - The attribute abbreviation       */
    OM_type         omtype,    /* IN  - OM type to be filled in          */
    OM_descriptor  *desc,      /* I/O - The descriptor to be filled      */
    gc_t_vector   **out_vect); /* OUT - Error messages in case of errors */

signed16 gc_t_create_result_vector_data(
    gc_t_vector    *result_vector, /* IN - The result vector            */
    unsigned32      vector_index,  /* IN - Index into the result vector */
    gc_t_vect_type  vector_type,   /* IN - Type of vector               */
    OM_string	    data,	   /* IN - The input data		*/
    OM_syntax       syntax);       /* IN - OM_syntax of the data        */

void gc_t_get_entry_info_vect_count(
    OM_public_object   entry_info,   /* IN  - The entry information object   */
    boolean            abbrv_result, /* IN  - To indicate abbreviated result */
    unsigned32        *vect_count);  /* OUT - Count of required vectors      */

signed16 gc_t_process_entry_info(
    OM_public_object   entry_info,    /* IN  - The entry information object   */
    boolean            abbrv_result,  /* IN  - To indicate abbreviated result */
    boolean            local_strings, /* IN  - To specify local strings       */
    unsigned32	      *vectors_used,  /* OUT - No. of vectors used for result */
    gc_t_vector      **result_vector, /* OUT - Result vector                  */
    gc_t_vector      **err_vector);   /* OUT - Error vector in case of errors */

void gc_t_get_partial_vect_count(
    OM_object    partial_obj,  /* IN  - The partial outcome qualifier object*/
    unsigned16  *vect_count);  /* OUT - Count of the number of vectors      */

signed16 gc_t_process_partial_qual_object(
    OM_object      partial_obj,  /* IN  - The partial outcome qualifier object*/
    boolean        local_strings,/* IN  - To specify local strings            */
    gc_t_vector   *result_vect,  /* I/O - The result vector                   */
    gc_t_vector  **error_vect);  /* OUT - Error message in case of errors     */

signed16 gc_t_process_access_points(
    OM_object     access_object, /* IN  - The access point object        */    
    boolean       local_strings, /* IN  - To specify local strings       */
    gc_t_vector  *result_vect,   /* I/O - The result vector              */    
    gc_t_vector **error_vect);   /* OUT - Error vector in case of errors */    

void gc_t_free_result(
    gc_t_vector  *res_vector); /* IN - The result vector to be freed */

#endif  /* ifndef _GCTUTIL_H */
