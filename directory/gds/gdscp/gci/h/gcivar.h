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
 * $Log: gcivar.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:06  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:34  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:28  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:57  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:57:48  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:35:43  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:30  keutel
 * 	creation
 * 	[1994/03/21  12:54:32  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gcivar.h                                          */
/* Description : File contains function prototypes for the routines*/
/*		 in the source file gcivar.c 			   */
/* Date        : January, 1994                                     */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCIVAR_H
#define _GCIVAR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gcivar_rcsid[] = "@(#)$RCSfile: gcivar.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:03 $";
#endif

/*-- Function Prototypes ------------------------------------------*/           
void gc_i_set_error_code(
    Tcl_Interp  *interp,    /* IN - Tcl-interpreter which processes the cmd */
    signed16  	 value);    /* IN - The value to be set 		    */

void gc_i_set_cwo_value(
    Tcl_Interp  *interp,  /* IN - Tcl-interpreter which processes the cmd */
    char        *value);  /* IN - The value to be set.                    */

void gc_i_set_cwo_trace(
    Tcl_Interp  *interp);   /* IN - Tcl-interpreter which processes the cmd */

boolean gc_i_is_local_string(
    Tcl_Interp  *interp);   /* IN - Tcl-interpreter which processes the cmd */

#endif /* ifndef _GCIVAR_H */
