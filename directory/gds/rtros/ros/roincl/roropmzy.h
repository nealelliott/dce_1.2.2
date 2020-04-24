/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropmzy.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:26  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:05  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:16  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:31  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:27  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:33  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:06  keutel
 * 	creation
 * 	[1994/03/21  13:33:04  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROROPMZY_H
#define _ROROPMZY_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roropmzy_rcsid[] = { "@(#) $RCSfile: roropmzy.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:26 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1991                     */
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
/* NAME         : roropmzy.h                                          */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE         : 93-04-09                                            */
/*                                                                    */
/* COMPONENT    : RTROS Remote Operations Services Protocol Machine   */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : Encode/Decode    Private Transfer Syntax:           */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/*datoff **************************************************************/

#define RC07yA_RoivApdu          1
#define RC07yB_RorsApdu          2
#define RC07yC_RoerApdu          3
#define RC07yD_RorjApdu          4

#define RC07y0_Rose_apduType                unsigned long  int

#define RC07y1_Roiv_invokeId                signed   long  int
#define RC07y2_Roiv_linkedVa                signed   short int
#define RC07y3_Roiv_linkedId                signed   long  int
#define RC07y4_Roiv_operType                unsigned long  int
#define RC07y5_Roiv_localValue              signed   long  int
#define RC07y6_Roiv_globlValue              unsigned short int

#define RC07y7_Rors_invokeId                signed   long  int
#define RC07y8_Rors_operType                unsigned long  int
#define RC07y9_Rors_localValue              signed   long  int
#define RC07ya_Rors_globlValue              unsigned short int

#define RC07yb_Roer_invokeId                signed   long  int
#define RC07yc_Roer_operType                unsigned long  int
#define RC07yd_Roer_localValue              signed   long  int
#define RC07ye_Roer_globlValue              unsigned short int

#define RC07yf_Rorj_invokeVa                unsigned long  int
#define RC07yg_Rorj_invokeId                signed   long  int
#define RC07yh_Rorj_probType                unsigned long  int
#define RC07yi_Rorj_problem                 signed   long  int

signed short int PASCAL ROIVSniP_type_cod PROTO ((
                                          unsigned long int        hStore   ,
                                          rT0710_stROIVapdu FAR  * x        ,
                                          rT0730_stContext  NEAR * pContext));

signed short int PASCAL RORSSniP_type_cod PROTO ((
                                          unsigned long int        hStore   ,
                                          rT0712_stRORSapdu FAR  * x        ,
                                          rT0730_stContext  NEAR * pContext));

signed short int PASCAL ROERSniP_type_cod PROTO ((
                                          unsigned long int        hStore   ,
                                          rT0713_stROERapdu FAR  * x        ,
                                          rT0730_stContext  NEAR * pContext));

signed short int PASCAL RORJSniP_type_cod PROTO ((
                                          unsigned long int        hStore   ,
                                          rT0714_stRORJapdu FAR  * x        ,
                                          rT0730_stContext  NEAR * pContext));

signed short int PASCAL ROIV_type_dec PROTO ((
                                          unsigned long int        hStore   ,
                                          rT0710_stROIVapdu FAR  * x        ,
                                          rT0730_stContext  NEAR * pContext));

signed short int PASCAL RORS_type_dec PROTO ((
                                          unsigned long int        hStore   ,
                                          rT0712_stRORSapdu FAR  * x        ,
                                          rT0730_stContext  NEAR * pContext));

signed short int PASCAL ROER_type_dec PROTO ((
                                          unsigned long int        hStore   ,
                                          rT0713_stROERapdu FAR  * x        ,
                                          rT0730_stContext  NEAR * pContext));

signed short int PASCAL RORJ_type_dec PROTO ((
                                          unsigned long int        hStore   ,
                                          rT0714_stRORJapdu FAR  * x        ,
                                          rT0730_stContext  NEAR * pContext));

#endif	/* _ROROPMZY_H */
