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
 * $Log: gctbind.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:51  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:43  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:59  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:58  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:25  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:58:00  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:13  keutel
 * 	creation
 * 	[1994/03/21  12:55:38  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program				   */
/* File Name   : gctbind.h					   */
/* Description : This file contains the prototypes for the public  */
/*		 functions in file gctbind.c 			   */
/* Date        : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCTBIND_H
#define _GCTBIND_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gctbind_rcsid[] = "@(#)$RCSfile: gctbind.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:08 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
OM_private_object gc_t_get_bound_session(
    void);

OM_workspace gc_t_get_workspace(
    void);

#endif  /* ifndef _GCTBIND_H */
