/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spcmx10.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:40  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:23  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:39  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:01  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:42  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:50  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:04:16  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:27  keutel
 * 	creation
 * 	[1994/03/21  13:32:47  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPCMX10_H
#define _SPCMX10_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _spcmx10_rcsid[] = { "@(#) $RCSfile: spcmx10.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:40 $" } ;
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
/* NAME 	: spcmx10.h       				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K.         			      */
/* DATE 	: 28-10-93					      */
/*                                                                    */
/* COMPONENT    : Step Functions                                      */
/*                                                                    */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION	: Function Prototyping for the CMX  - STEP module     */
/*		  (local functions) and local definitions.	      */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  " @(#)spcmx10.h 1.2 94/01/19"                        */
/*                                                                    */
/* Vers.Nr. | Date    | Modification		       |mark| CR# FM# */
/* 03.00 K0 | 93-04-23| original		       | kc |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ***************    LOCAL FUNCTIONS IN SPEOIF0.C    *************** */
/* ***************				      *************** */

void sp0301_enTatach	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0302_reTattach	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0303_enTdetach	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0304_reTdetach	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0306_enTevent	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0307_reTevent	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0308_enTdisrq	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0309_reTdisrq	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp030a_enTconrq	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp030b_reTconrq	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp030c_enTredrq	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp030d_reTredrq	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp030e_enTconrs	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp030f_reTconrs	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0310_enTinfo	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0311_reTinfo	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0312_enTdatarq	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0313_reTdatarq      PROTO ((char FAR * FAR *, unsigned long int *));
void sp0314_enTconin      PROTO ((char FAR * FAR *, unsigned long int *));
void sp0315_reTconin   PROTO ((char FAR * FAR *, unsigned long int *));
void sp0316_enTredin   PROTO ((char FAR * FAR *, unsigned long int *));
void sp0317_reTredin   PROTO ((char FAR * FAR *, unsigned long int *));
void sp0318_enTconcf	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp0319_reTconcf   PROTO ((char FAR * FAR *, unsigned long int *));
void sp031a_enTdisin PROTO ((char FAR * FAR *, unsigned long int *));
void sp031b_reTdisin	  PROTO ((char FAR * FAR *, unsigned long int *));
void sp031c_enTdatain   PROTO ((char FAR * FAR *, unsigned long int *));
void sp031d_reTdatain   PROTO ((char FAR * FAR *, unsigned long int *));

/*--------------------------------------------------------------------*/
void sp03a0_writeAOptionNr PROTO (( int , char FAR * , int ));
void sp03a1_writeCOptionNr PROTO (( int , char FAR * , int ));
void sp03a2_writeFnRetVal PROTO (( int , char FAR * , int ));
void sp03a3_writeErrorVal PROTO (( int , char FAR * , int ));
void sp03a4_writeWaitMode PROTO (( int , char FAR * , int ));
void sp03a5_writeCmxOptions PROTO (( int , char FAR * , int ));
void sp03a6_writeChainVal PROTO (( int , char FAR * , int ));
void sp03a7_writeTeventVal PROTO (( int , char FAR * , int ));
void sp03a8_writeAbortReason PROTO (( int , char FAR * , int ));
void sp03a9_writeAddress PROTO (( union t_address FAR *   pAddr,      
int                     mode));       

#endif	/* _SPCMX10_H */
