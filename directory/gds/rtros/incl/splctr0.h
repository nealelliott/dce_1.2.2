/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: splctr0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:29  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:37:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:42  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:15  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:56  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:51  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:30  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:08  keutel
 * 	creation
 * 	[1994/03/21  13:28:48  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPLCTR0_H
#define _SPLCTR0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _splctr0_rcsid[] = { "@(#) $RCSfile: splctr0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:29 $" } ;
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
/* NAME 	: splctr0.h					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K.         			      */
/* DATE 	: 28-04-93					      */
/*                                                                    */
/* COMPONENT    : RTROS                                               */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V03.00                                        */
/*                                                                    */
/* DESCRIPTION	: Function Prototyping for the Step application       */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  " @(#)splctr0.h 1.1 93/11/29"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 03.00 K0 | 93-04-28|  original		       | kc |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* *******************	  FUNCTION IN SPLCTR0.C    ****************** */
/* *******************				   ****************** */

signed short int sp0500_traceLctr PROTO ((long	int                      ,
					  long	int                      ,
					  unsigned short int             ,
					  char                FAR * FAR *,
					  unsigned long int         FAR *,
					  unsigned short int             ));

#endif	/* _SPLCTR0_H */
