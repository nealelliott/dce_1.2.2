/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sproif0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:41  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:41  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:03  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:43  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:52  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:04:18  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:28  keutel
 * 	creation
 * 	[1994/03/21  13:32:22  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPROIF0_H
#define _SPROIF0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _sproif0_rcsid[] = { "@(#) $RCSfile: sproif0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:41 $" } ;
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
/* NAME 	: sproif0.h					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K.         			      */
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
/* HISTORY      : sccsid  =  " @(#)sproif0.h 1.1 93/11/29"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 03.00 K0 | 93-04-20|  original		       | kc |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* *******************	  FUNCTION IN SPROIF0.C    ****************** */
/* *******************				   ****************** */

signed short int sp1300_traceExif PROTO ((
long int	              lType,
long int	              lTime,
unsigned short int            argn,
char               FAR * FAR *argv,
unsigned long int        FAR *argl,
unsigned short int            nerr));

#endif	/* _SPROIF0_H */
