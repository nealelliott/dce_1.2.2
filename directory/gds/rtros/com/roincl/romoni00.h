/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: romoni00.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:37  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:53  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:55  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:24  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:32  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:58  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:41  keutel
 * 	creation
 * 	[1994/03/21  13:17:43  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROMONI00_H
#define _ROMONI00_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _romoni00_rcsid[] = { "@(#) $RCSfile: romoni00.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:46 $" } ;
#endif

/***************************************************************************\ 
*                                                                           * 
*  * Copyright (c)                                                          * 
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1993                            * 
*  * All rights reserved                                                    * 
*  *                                                                        * 
*  * This software is furnished under licence and may be used only in       * 
*  * accordance with the terms of that licence and with the inclusion of    * 
*  * the above copyright notice. This software may not be provided or       * 
*  * otherwise made available to, or used by, any other person. No title    * 
*  * to or ownership of the software is hereby transferred.                 * 
*                                                                           * 
\***************************************************************************/ 
/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : romoni00.h                                          */
/*                                                                    */
/* AUTHOR       : Milind m. Aphale                                    */
/* DATE         : 93-05-14                                            */
/*                                                                    */
/* COMPONENT    : RTROS PDU MONITOR                                   */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V03.00                                        */
/*                                                                    */
/* DESCRIPTION  : This file provides the function prototypes for      */
/*                PDU monitor.                                        */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :   sccsid  =  @(#) romoni00.h 1.1 93/11/29 RTROS-V3.00            */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/*          |         |                                |    |         */ 
/* 03.00 K0 | 93-05-14|  original                      | ma |         */
/*          |         |                                |    |         */ 
/*          |         |                                |    |         */ 
/*          |         |                                |    |         */ 
/*datoff **************************************************************/

unsigned short int  PASCAL ro00m1_moniON PROTO(( void ));  

void PASCAL ro00m0_moniFct  PROTO(( 
		unsigned long  int ,  /*  hEcb        */
		unsigned long  int ,  /*  hAcb        */
		signed   short int ,  /*  sPmNo       */
		unsigned long  int ,  /*  ulEvent     */
		unsigned long  int *, /*  pP1         */
		unsigned long  int *, /*  pP2         */
		unsigned long  int *, /*  pP3         */
		unsigned long  int *  /*  pP4         */
                ));

#endif	/* _ROMONI00_H */
