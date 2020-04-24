/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spdpat0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:28  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:37:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:40  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:14  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:55  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:50  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:28  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:07  keutel
 * 	creation
 * 	[1994/03/21  13:28:47  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPDPAT0_H
#define _SPDPAT0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _spdpat0_rcsid[] = { "@(#) $RCSfile: spdpat0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:28 $" } ;
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
/* NAME 	: spdpat0.h					      */
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
/* HISTORY      : sccsid  =  " @(#)spdpat0.h 1.1 93/11/29"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 02.00 K0 | 13-03-92|  original		       | ns |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* *******************	  FUNCTION IN SPDPAT0.C    ****************** */
/* *******************				   ****************** */

signed short int sp0200_traceDpat PROTO ((long	int                      ,
					  long	int                      ,
					  unsigned short int             ,
					  char                FAR * FAR *,
					  unsigned long int         FAR *,
					  unsigned short int             ));

#endif	/* _SPDPAT0_H */
