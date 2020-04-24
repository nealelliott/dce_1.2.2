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
 * $Log: gciprint.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:00  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:32  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:25  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:54  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:57:41  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:24  keutel
 * 	creation
 * 	[1994/03/21  12:54:26  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gciprint.h                                        */
/* Description : This file contains the prototypes for all the     */
/*		 global functions in file gciprint.c		   */
/* Date        : Oct 04, 1993                                      */
/* Author      : M. Kronenburg, SNI BU BA KP13                     */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCIPRINT_H
#define _GCIPRINT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gciprint_rcsid[] = "@(#)$RCSfile: gciprint.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:00 $";
#endif

/*-- Function Prototypes ------------------------------------------*/           
signed16 gc_i_prepare_print_result(
    gc_t_vector  *in_vect,  /* IN - Vector to convert                      */
    Tcl_Interp   *interp);  /* OUT - Tcl-interpreter set in case of errors */

signed16 gc_i_prepare_showabbr_print_result(
    gc_t_vector  *in_vect,  /* IN  - Vector to convert                      */
    Tcl_Interp   *interp);  /* OUT - Tcl-interpreter set in case of errors  */

signed16 gc_i_prepare_showdsc_print_result(
    gc_t_vector  *in_vect,  /* IN  - Vector to convert                     */
    Tcl_Interp   *interp);  /* OUT - Tcl-interpreter set in case of errors */

void gc_i_print_result(
    void);

#endif /* ifndef _GCIPRINT_H */
