/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spspm0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:33  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:37:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:48  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:18  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:00:00  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:54  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:38  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:11  keutel
 * 	creation
 * 	[1994/03/21  13:28:55  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPSPM0_H
#define _SPSPM0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _spspm0_rcsid[] = { "@(#) $RCSfile: spspm0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:33 $" } ;
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
/* NAME 	: spspm0.h         				      */
/*                                                                    */
/* AUTHOR	: Shivkumar               			      */
/* DATE 	: 93-04-93					      */
/*                                                                    */
/* COMPONENT    : RTROS                                               */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V03.00                                        */
/*                                                                    */
/* DESCRIPTION	: Function Prototyping for the Step application       */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  " @(#)spspm0.h 1.2 93/12/20"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 03.00 K0 | 93-04-20|  original		       | kc |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* *******************	  FUNCTION IN SPROIF0.C    ****************** */
/* *******************				   ****************** */

signed short int sp0800_traceSpm  PROTO ((
long int	               type,
long int	               time,
unsigned short int             argn,
char                FAR * FAR *argv,
unsigned long int         FAR *argl,
unsigned short int             nerr));

void sp0820_writeSpmPdu PROTO ((
unsigned long  int       hStore,                                
unsigned       int FAR * puInEvent,                             
unsigned long  int FAR * pulOffset,                             
signed   short int       bPduDump));                              

#endif	/* _SPSPM0_H */
