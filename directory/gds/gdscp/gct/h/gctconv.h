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
 * $Log: gctconv.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:53  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:44  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:00  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:59  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:26  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:58:05  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:36:59  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:15  keutel
 * 	creation
 * 	[1994/03/21  12:55:41  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program				   */
/* File Name   : gctconv.h					   */
/* Description : This file contains all the prototypes for the     */
/*		 convenience functions provided by the GDSCP	   */
/*		 Translator. It also contains the data type 	   */
/*		 definitions.					   */
/* Date        : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCTCONV_H
#define _GCTCONV_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gctconv_rcsid[] = "@(#)$RCSfile: gctconv.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:09 $";
#endif

/*-- Data Types ---------------------------------------------------*/
/* To store information about the type of vector string */
typedef enum _gc_t_vect_type
{
    GC_T_DN            =  0, /* To specify Distinguished Name 	             */
    GC_T_ATTRNAME      =  1, /* To specify Attribute Full Name 	             */
    GC_T_ATTRVAL       =  2, /* To specify Attribute Value 		     */
    GC_T_SATTRNAME     =  3, /* To specify Structured Attribute Full Name    */
    GC_T_COMPNAME      =  4, /* To specify Component Full Name 	             */
    GC_T_COMPVAL       =  5, /* To specify Component Value 		     */
    GC_T_SCOMPNAME     =  6, /* To specify Structured Component Full Name    */
    GC_T_SCOMPEND      =  7, /* To specify Structured Component End          */
    GC_T_BOOLEAN       =  8, /* To specify Boolean string 		     */
    GC_T_OCLABBRV      =  9, /* To specify Object Class Abbreviation         */
    GC_T_OCLNAME       = 10, /* To specify Object Class Name      	     */
    GC_T_OBJID         = 11, /* To specify Object Identifiers      	     */
    GC_T_ATTRABBRV     = 12, /* To specify Attribute Abbreviation 	     */
    GC_T_COMPABBRV     = 13, /* To specify Component Abbreviation 	     */
    GC_T_ATTRINFO      = 14, /* To specify Abbreviated Attribute Information */
    GC_T_EOL           = 15, /* To specify End of Vector List 	             */
    GC_T_ERR           = 16, /* To specify Error Message 		     */
    GC_T_RESOLVED_DN   = 17, /* To specify Resolved Distinguished Name	     */
    GC_T_REF_DSA       = 18, /* To specify Referral DSA Name          	     */
    GC_T_REF_ADDR      = 19, /* To specify Referral DSA Address              */
    GC_T_LIMIT_PROBLEM = 20, /* To specify limit problem                     */
    GC_T_NO_CRIT_EXT   = 21  /* To specify critical extension problem 	     */
} gc_t_vect_type;

/* To store results */
typedef struct _gc_t_vector
{
    gc_t_vect_type   type;   /* Type of result    */
    char            *value;  /* The result itself */
} gc_t_vector;


/*-- Function Prototypes ------------------------------------------*/
signed16 gc_t_bind(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean	  local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect);     /* OUT - NULL (success) or error message */

signed16 gc_t_compare(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean	  local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect);     /* OUT - The result or the error message */

signed16 gc_t_create(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean	  local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect);     /* OUT - NULL (success) or error message */

signed16 gc_t_list(
    char         **in_vect,       /* IN  - Array of input vectors          */
    signed16       vect_count,    /* IN  - Count of input vectors          */
    boolean	   local_strings, /* IN  - To specify local strings        */
    gc_t_vector  **out_vect);     /* OUT - The result or the error message */

signed16 gc_t_moddsc(
    char        **in_vect,    /* IN  - Array of input vectors          */
    signed16      vect_count, /* IN  - Count of input vectors          */
    gc_t_vector **out_vect);  /* OUT - NULL (success) or error message */

signed16 gc_t_modify(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean	  local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect);     /* OUT - NULL (success) or error message */

signed16 gc_t_modrdn(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean	  local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect);     /* OUT - NULL (success) or error message */

signed16 gc_t_moveto(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean	  local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect);     /* OUT - NULL (success) or error message */

signed16 gc_t_pwo(
    gc_t_vector **out_vect);  /* OUT - The result or the error message */

signed16 gc_t_read(
    char         **in_vect,       /* IN  - Array of input vectors          */
    signed16       vect_count,    /* IN  - Count of input vectors          */
    boolean        abbrv_result,  /* IN  - To specify abbreviated result   */
    boolean	   local_strings, /* IN  - To specify local strings        */
    gc_t_vector  **out_vect);     /* OUT - The result or the error message */

signed16 gc_t_remove(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean	  local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect);     /* OUT - NULL (success) or error message */

signed16 gc_t_search(
    char         **in_vect,       /* IN  - Array of input vectors          */
    signed16       vect_count,    /* IN  - Count of input vectors          */
    boolean        abbrv_result,  /* IN  - To specify abbreviated result   */
    boolean	   local_strings, /* IN  - To specify local strings        */
    gc_t_vector  **out_vect);     /* OUT - The result or the error message */

signed16 gc_t_showabbrv(
    gc_t_vector **out_vect);  /* OUT - The result or the error message */

signed16 gc_t_showdsc(
    boolean       abbrv_result, /* IN  - To specify abbreviated result   */
    gc_t_vector **out_vect);    /* OUT - The result or the error message */

signed16 gc_t_unbind(
    gc_t_vector **out_vect);  /* OUT - NULL (success) or error message */

void gc_t_free_result(
    gc_t_vector *res_vector); /* IN  - The result vector to be freed   */

#endif  /* ifndef _GCTCONV_H */
