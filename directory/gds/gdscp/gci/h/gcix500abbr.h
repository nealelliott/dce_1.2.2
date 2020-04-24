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
 * $Log: gcix500abbr.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:07  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:35  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:29  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:58  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  11:46:46  marrek]
 * 
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:57:51  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:31  keutel
 * 	creation
 * 	[1994/03/21  12:54:34  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gcix500abbr.h                                     */
/* Description : File contains function prototypes for the public  */
/*		 functions in the source file gcix500abbr.c 	   */
/* Date        : January, 1994                                     */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCIX500ABBR_H
#define _GCIX500ABBR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gcix500abbr_rcsid[] = "@(#)$RCSfile: gcix500abbr.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:04 $";
#endif

/*-- Function Prototypes ------------------------------------------*/           
int gc_i_x500abbr(
    ClientData    client_data,/* IN - Not used                                */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments                     */
    char        **argv);      /* IN - Arguments themselves                    */

#endif /* ifndef _GCIX500ABBR_H */
