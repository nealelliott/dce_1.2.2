/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spcmx01.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:39  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:22  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:41:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:37  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:59  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:41  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:49  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:04:14  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:25  keutel
 * 	creation
 * 	[1994/03/21  13:32:20  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPCMX01_H
#define _SPCMX01_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _spcmx01_rcsid[] = { "@(#) $RCSfile: spcmx01.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:39 $" } ;
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
/* NAME 	: spcmx01.h        				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K          			      */
/* DATE 	: 93-04-16-02-94     */
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

/* *******************				   ****************** */

signed short int sp0300_traceCmx  PROTO ((
long int	               type,
long int	               time,
unsigned short int             argn,
char                FAR * FAR *argv,
unsigned long int         FAR *argl,
unsigned short int             nerr));


#endif	/* _SPCMX01_H */
