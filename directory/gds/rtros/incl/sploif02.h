/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sploif02.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:30  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:17  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:37:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:43  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:16  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:57  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:52  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:32  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:09  keutel
 * 	creation
 * 	[1994/03/21  13:28:50  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPLOIF02_H
#define _SPLOIF02_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _sploif02_rcsid[] = { "@(#) $RCSfile: sploif02.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:30 $" } ;
#endif

/**********************************************************************/
/*                                                                    */
/*    COPYRIGHT  (C)						      */
/*    Siemens Nixdorf Informationssysteme AG 1991		      */
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
/* NAME 	: sploif0.h					      */
/*                                                                    */
/* AUTHOR	: Natalia Serra 				      */
/* DATE 	: 13-03-92					      */
/*                                                                    */
/* COMPONENT    : RTS                                                 */
/*                                                                    */
/* PRD#/VERS.   : RTS-V02.00                                          */
/*                                                                    */
/* DESCRIPTION	: Function Prototyping for the Step application       */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  " @(#)sploif02.h 1.1 93/11/29"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 02.00 K0 | 13-03-92|  original		       | ns |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* *******************	  FUNCTION IN SPLOIF0.C    ****************** */
/* *******************				   ****************** */

signed short int sp0300_traceLoif PROTO ((long	int                      ,
					  long	int                      ,
					  unsigned short int             ,
					  char                FAR * FAR *,
					  unsigned long  int        FAR *,
					  unsigned short int             ));

#endif	/* _SPLOIF02_H */
