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
 * $Log: gcix500obj.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:08  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:36  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:30  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:58  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  11:46:47  marrek]
 * 
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:57:53  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:32  keutel
 * 	creation
 * 	[1994/03/21  12:54:36  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gcix500obj.h                                      */
/* Description : File contains function prototypes for the public  */
/*		 functions in the source file gcix500obj.c 	   */
/* Date        : January, 1994                                     */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCIX500OBJ_H
#define _GCIX500OBJ_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gcix500obj_rcsid[] = "@(#)$RCSfile: gcix500obj.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:05 $";
#endif

/*-- Function Prototypes ------------------------------------------*/           
int gc_i_x500obj(
    ClientData    client_data,/* IN - Not used                                */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments                     */
    char        **argv);      /* IN - Arguments themselves                    */

#endif /* ifndef _GCIX500OBJ_H */
