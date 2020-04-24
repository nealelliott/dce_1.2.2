/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sproif10.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:42  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:43  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:05  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:44  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:54  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:04:22  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:29  keutel
 * 	creation
 * 	[1994/03/21  13:32:41  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPROIF10_H
#define _SPROIF10_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _sproif10_rcsid[] = { "@(#) $RCSfile: sproif10.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:42 $" } ;
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
/* NAME 	: spexif10.h					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K.         			      */
/* DATE 	: 93-04-23					      */
/*                                                                    */
/* COMPONENT    : Step Functions                                      */
/*                                                                    */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION	: Function Prototyping for the EXIF - STEP module     */
/*		  (local functions) and local definitions.	      */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  " @(#)sproif10.h 1.1 93/11/29"                        */
/*                                                                    */
/* Vers.Nr. | Date    | Modification		       |mark| CR# FM# */
/* 03.00 K0 | 93-04-23| original		       | kc |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ***************    LOCAL FUNCTIONS IN SPEOIF0.C    *************** */
/* ***************				      *************** */

void sp1301_enEvent	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1302_reEvent	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1303_enAttach	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1304_reAttach	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1305_enDetach	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1307_enBindReq	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1308_reBindReq	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1309_enBindRsp	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1311_reBindInd	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1312_reBindCnf	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1313_enInvReq	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1314_reInvInd	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1315_enResReq	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1316_reResInd	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1317_enErrReq	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1318_reErrInd	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1319_enURjReq	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1320_reURjInd      PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1321_rePRjInd      PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1322_enUnBindReq   PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1323_reUnBindReq   PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1324_enUnBindRsp   PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1325_reUnBindReq	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1326_reUnBindInd   PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1327_reUnBindCnf PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1328_enUAbortReq	  PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp1329_reUAbortReq   PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp132a_reUAbortInd   PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp132b_rePAbortInd   PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp132c_enProvBuf   PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp132d_enCwait   PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp132e_enInform   PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp132f_rnProvBuf   PROTO ((char FAR * FAR *, unsigned long int FAR *));
void sp132g_rnrtrosCl   PROTO ((char FAR * FAR *, unsigned long int FAR *));

/*--------------------------------------------------------------------*/
void sp1330_enHeader      PROTO (( RO_fncthd FAR * ));
void sp1331_reEvHead      PROTO (( RO_evnthd FAR * ));
void sp1332_WrObjId      PROTO (( int , char FAR *, char  FAR *, unsigned long int)); 
void sp1333_writeAclass      PROTO (( int , char FAR *, int)); 
void sp1334_writeOpclass      PROTO (( int , char FAR *, int)); 
void sp1335_writeMtype      PROTO (( int , char FAR *, int)); 
void sp1336_writeResult      PROTO (( int , char FAR *, unsigned long int)); 
void sp1337_writeUreasn      PROTO (( int , char FAR *, unsigned long int)); 
void sp1338_writeReason      PROTO (( int , char FAR *, unsigned long int)); 
                            
#endif	/* _SPROIF10_H */
