/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rolctr2.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:43  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:34:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:21  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:39  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:15  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:16  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:20  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:23  keutel
 * 	creation
 * 	[1994/03/21  13:26:41  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROLCTR2_H
#define _ROLCTR2_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rolctr2_rcsid[] = { "@(#) $RCSfile: rolctr2.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:43 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONS AG	      */
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
/* NAME 	: rolcrt2.h					      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-26					      */
/*                                                                    */
/* COMPONENT	: RTS dispatcher				      */
/*								      */
/* PRD#/VERS.	: RTS-V03.00					      */
/*                                                                    */
/* DESCRIPTION	: Rts association control function calling	      */
/*		  interface					      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rolctr2.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 02.00 K0 | 91-10-30|  original		       |jm  |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ***************** ASO-lower control prototype function *********** */
/* ***************** definitions 			************* */


signed short    int PASCAL  ro0500_lowerCtrl PROTO ((unsigned long int));

/* Return value == 0 : successfully       */                            
/*              != 0 : error              */                            
/* Parameter 1       : pm context handle  */                            

signed short    int PASCAL  ro0501_cleanUp   PROTO ((
       		signed   long  int,   unsigned long  int, 	 
                unsigned long  int,   unsigned long  int,     
                unsigned long  int  ));                          

/* Return value == 0 : successfully            */ 
/*              != 0 : error                   */ 
/* Parameter 1       : event                   */ 
/* Parameter 2       : handle P1 or PM context */
/* Parameter 3       : p2                      */
/* Parameter 4       : handle P3               */
/* Parameter 5       : p4                      */


signed short int PASCAL ro0502_informLowerCtrl PROTO ((
		unsigned long  int ,               
		signed   short int ,                
		void     FAR   *   ))  ; 


/* Return value == 0 : successfully                               */ 
/*              != 0 : error                                      */ 
/* Parameter 1       : PM Context Handle                          */ 
/* Parameter 2       : Information Type                           */
/* Parameter 3       : void pointter to an address where info is  */
/*		       to be copied                               */

#endif	/* _ROLCTR2_H */
