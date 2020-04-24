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
 * $Log: gctmoddsc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:47  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:02  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:01  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:27  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:58:10  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:18  keutel
 * 	creation
 * 	[1994/03/21  12:55:46  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program				   */
/* File Name   : gctmoddsc.h					   */
/* Description : This file contains all the prototypes for the	   */
/*		 public functions defined in gctmoddsc.c	   */
/* Date        : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCTMODDSC_H
#define _GCTMODDSC_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gctmoddsc_rcsid[] = "@(#)$RCSfile: gctmoddsc.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:11 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
OM_private_object gc_t_get_context(
    void);

void gc_t_free_context(
    void);

#endif  /* ifndef _GCTMODDSC_H */
