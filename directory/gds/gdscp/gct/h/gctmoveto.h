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
 * $Log: gctmoveto.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:49  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:03  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:02  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:28  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:58:12  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:21  keutel
 * 	creation
 * 	[1994/03/21  12:55:48  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program				   */
/* File Name   : gctmoveto.h					   */
/* Description : This file contains the prototypes for the public  */
/*		 functions in file gctmoveto.c 		   	   */
/* Date        : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCTMOVETO_H
#define _GCTMOVETO_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gctmoveto_rcsid[] = "@(#)$RCSfile: gctmoveto.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:12 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
OM_string *gc_t_get_current_dit_position(
    void);

signed16 gc_t_update_current_dit(
    char  *full_dn_string); /* IN  - The input DN string */

void gc_t_free_current_dit_position(
    void);

#endif  /* ifndef _GCTMOVETO_H */
