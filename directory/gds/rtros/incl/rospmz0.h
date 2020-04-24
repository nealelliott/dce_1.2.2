/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospmz0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:11  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:57  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:09  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:59  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:41  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:35  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:29  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:42  keutel
 * 	creation
 * 	[1994/03/21  13:28:00  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROSPMZ0_H
#define _ROSPMZ0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rospmz0_rcsid[] = { "@(#) $RCSfile: rospmz0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:11 $" } ;
#endif


/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1993                     */
/*  * All rights reserved                                             */
/*  *                                                                 */
/*  * This software is furnished under licence and may be used only   */
/*  * in accordance with the terms of that licence and with the       */
/*  * inclusion of the above copyright notice.                        */
/*  * This software may not be provided or otherwise made available   */
/*  * to, or used by, any other person. No title to or ownership of   */
/*  * the software is hereby  transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NEW NAME     : rospmz0.h                                           */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-07					      */
/*                                                                    */
/* COMPONENT    : RTROS session protocol machine                      */
/*                                                                    */
/* PRD#/VERS.	: RTROS-V3.00					      */
/*                                                                    */
/* DESCRIPTION	: Information related to SPM inform function.         */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rospmz0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 k00 | 93-04-07|  original         	       |ma  |	      */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ------------------------------------------------------------------------- */
/*  sInformType              = Type of information requested.                */
/*                             Possible values are :                         */
/* ------------------------------------------------------------------------- */ 

#define RC08z0_SpmEvent                0x0500   
#define RC08z1_InformStoreOffset       ( RC08z0_SpmEvent | 0x0001 )

/* -------------------------------------------------------------------------- */
/*     pRequestedInfo        = Pointer to memory containing the data          */
/*                             structure associated with sInformType that is  */
/*                             TO BE FILLED with the requested info.          */
/* -------------------------------------------------------------------------- */

typedef unsigned long int rT08z0_ulStoreOffsetLen;  

#endif	/* _ROSPMZ0_H */
