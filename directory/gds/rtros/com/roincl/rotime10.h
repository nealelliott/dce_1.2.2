/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rotime10.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:18  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:12  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:43  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:48  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:53  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:07  keutel
 * 	creation
 * 	[1994/03/21  13:19:29  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROTIME10_H
#define _ROTIME10_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rotime10_rcsid[] = { "@(#) $RCSfile: rotime10.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:03 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1990                     */
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
/* NAME 	: rotime10.h					      */
/*                                                                    */
/* AUTHOR       : N. Serra                                            */
/* DATE 	: 06-12-91					      */
/*                                                                    */
/* COMPONENT    : RTS                                                 */
/*                                                                    */
/* PRD#/VERS.   : RTS-V02.00                                          */
/*                                                                    */
/* DESCRIPTION	: Function prototyping for the Timer (local functions)*/
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 02.00 K0 | 06-12-91|  original		       | ns |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* *********   Move timer control list's pointers one position   ********* */
/* *********							 ********* */

signed short int ro1110_movePtrsOnePos (unsigned long int       FAR *,
					unsigned long int       FAR *,
					rT1100_ctrlTimer  FAR * FAR *,
					rT1100_ctrlTimer  FAR * FAR *,
					signed int);

/* Return value == 0 : success                                             */
/*               < 0 : error                                               */

/* P1:	 memory handle of the previous element in the list		   */
/* P2:	 memory handle of the current element in the list		   */
/* P3:	 pointer to the previous element				   */
/* P4:	 pointer to the current element 				   */
/* P5:	 boolean: the current element must be deleted or not		   */

#endif	/* _ROTIME10_H */
