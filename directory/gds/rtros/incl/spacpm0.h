/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spacpm0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:27  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:37:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:38  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:13  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:55  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:49  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:25  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:06  keutel
 * 	creation
 * 	[1994/03/21  13:28:45  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPACPM0_H
#define _SPACPM0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _spacpm0_rcsid[] = { "@(#) $RCSfile: spacpm0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:27 $" } ;
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
/* NAME 	: spacpm0.h					      */
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
/* HISTORY      : sccsid  =  " @(#)spacpm0.h 1.2 93/12/20"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 02.00 K0 | 13-03-92|  original		       | ns |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* *******************	  FUNCTION IN SPACPM0.C    ****************** */
/* *******************				   ****************** */

signed short int sp0600_traceAcpm PROTO ((long	int                     ,
					  long	int                     ,
					  unsigned short int            ,
					  char               FAR * FAR *,
					  unsigned long int        FAR *,
					  unsigned short int            ));


/*--------------------------------------------------------------------*/

/* P1 : event number */
void sp0620_writeAcpmPdu PROTO (( 
unsigned long int hNode));          

#endif	/* _SPACPM0_H */
