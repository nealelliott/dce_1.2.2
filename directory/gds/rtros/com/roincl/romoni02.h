/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: romoni02.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:56  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:58  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:27  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:34  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:04  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:43  keutel
 * 	creation
 * 	[1994/03/21  13:19:36  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROMONI02_H
#define _ROMONI02_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _romoni02_rcsid[] = { "@(#) $RCSfile: romoni02.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:48 $" } ;
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
/* NAME         : romoni02.h                                          */
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
/* HISTORY      :   sccsid  =  @(#) romoni02.h 1.1 93/11/29 RTROS-V3.00            */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/*          |         |                                |    |         */ 
/* 03.00 K0 | 93-05-14|  original                      | ma |         */
/*          |         |                                |    |         */ 
/*          |         |                                |    |         */ 
/*          |         |                                |    |         */ 
/*datoff **************************************************************/

int  PASCAL ro00mb_yesOrNo PROTO((
          FILE * ,        /* stream       */
          char * ,        /* string       */                       
          int             /* defvalue     */
          ));
                     

                                                                   
unsigned short int  PASCAL ro00m2_SpmPpmMoniFct PROTO(( 
		unsigned long  int ,  /* ulEvent      */                      
		unsigned long int *   /* hStore       */
 		));                                  

#endif	/* _ROMONI02_H */
