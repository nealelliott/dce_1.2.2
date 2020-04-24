/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spuctr02.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:40  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:25  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:38:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:00  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:23  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:00:04  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:59  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:51  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:19  keutel
 * 	creation
 * 	[1994/03/21  13:29:03  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPUCTR02_H
#define _SPUCTR02_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _spuctr02_rcsid[] = { "@(#) $RCSfile: spuctr02.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:40 $" } ;
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
/* NAME 	: spuctr0.h					      */
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
/* HISTORY      : sccsid  =  " @(#)spuctr02.h 1.2 93/12/20"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 03.00 K0 | 93-04-28|  original		       | kc |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* *******************	  FUNCTION IN SPROPM0.C    ****************** */
/* *******************				   ****************** */

signed short int sp1500_traceUctr PROTO ((long	int                      ,
					  long	int                      ,
					  unsigned short int             ,
					  char                FAR * FAR *,
					  unsigned long  int        FAR *,
					  unsigned short int             ));

#endif	/* _SPUCTR02_H */
