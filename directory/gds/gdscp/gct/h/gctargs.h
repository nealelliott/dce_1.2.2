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
 * $Log: gctargs.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:42  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:58  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:57  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:24  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:57:58  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:11  keutel
 * 	creation
 * 	[1994/03/21  12:55:37  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program				   */
/* File Name   : gctargs.h					   */
/* Description : This file contains all the prototypes for the 	   */
/*		 functions defined in file gctargs.c		   */
/* Date        : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCTARGS_H
#define _GCTARGS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gctargs_rcsid[] = "@(#)$RCSfile: gctargs.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:07 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
signed16 gc_t_check_args(
    gc_t_op_type   op_type,         /* IN  - Type of operation		     */
    char         **in_vect,         /* IN  - Array of input vectors          */
    signed16       vect_count,      /* IN  - Count of input vectors          */
    signed16      *attr_info_count, /* OUT - Count of attr.info switches     */
    gc_t_vector  **error_vect);     /* OUT - Error message in case of errors */

signed16 gc_t_check_switch(
    gc_t_vect_type  operation,     /* IN  - The type of gdscp operation       */
    char           *user_switch,   /* IN  - The user supplied switch          */
    char           *defined_switch,/* IN  - The Translator defined switch     */
    boolean   	   *is_present,    /* OUT - Indicates user switch is OK or not*/
    gc_t_vector   **error_vect);   /* OUT - Error vector in case of errors    */

#endif  /* ifndef _GCTARGS_H */
