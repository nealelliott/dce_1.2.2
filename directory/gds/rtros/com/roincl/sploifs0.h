/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sploifs0.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:28  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:43  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:50  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:53  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:50:11  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:14  keutel
 * 	creation
 * 	[1994/03/21  13:17:56  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPLOIFS0_H
#define _SPLOIFS0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _sploifs0_rcsid[] = { "@(#) $RCSfile: sploifs0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:09 $" } ;
#endif

/**********************************************************************/
/*                                                                    */
/*    COPYRIGHT  (C)						      */
/*    Siemens Nixdorf Informationssysteme AG 1993		      */
/*    All rights reserved					      */
/*								      */
/*    This software is furnished under licence and may be used only   */
/*    in accordance with the terms of that licence and with the       */
/*    inclusion of the above copyright notice.			      */
/*    This software may not be provided or otherwise made available   */
/*    to, or used by, any other person. No title to or ownership of   */
/*    the software is hereby transferred.			      */
/*                                                                    */
/**********************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME 	: sploifs0.h      				      */
/*                                                                    */
/* AUTHOR	: Shivkumar     				      */
/* DATE 	: 01-12-93					      */
/*                                                                    */
/* COMPONENT    : ROS                                                 */
/*                                                                    */
/* PRD#/VERS.   : ROS-V03.00                                          */
/*                                                                    */
/* DESCRIPTION	: Function Prototyping for the Step application       */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  " @(#)sploifs0.h 1.1 93/12/20"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 03.00 K0 | 01-12-93|  original		       | skh|	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* *******************	  FUNCTION IN SPLOIFS0.C   ****************** */
/* *******************				   ****************** */

signed short int sp1800_traceLoif PROTO ((
signed   long  int	         type,
signed   long  int	         time,
unsigned short int               argn,
char                 FAR * FAR * argv,
unsigned long  int         FAR * argl,
unsigned short int               nerr));

#endif	/* _SPLOIFS0_H */
