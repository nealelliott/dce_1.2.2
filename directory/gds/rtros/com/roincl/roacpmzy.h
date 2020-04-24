/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpmzy.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:23  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:40  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:28  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:15  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:26  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:36  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:33  keutel
 * 	creation
 * 	[1994/03/21  13:18:54  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROACPMZY_H
#define _ROACPMZY_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roacpmzy_rcsid[] = { "@(#) $RCSfile: roacpmzy.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:39 $" } ;
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
/* NAME         : roacpmzy.h                                          */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE         : 93-04-09                                            */
/*                                                                    */
/* COMPONENT    : ROS association control protocol machine            */
/*                                                                    */
/* PRD#/VERS.   : ROS-V3.00                                           */
/*                                                                    */
/* DESCRIPTION  : Encode/Decode    Private Transfer Syntax:           */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/*datoff **************************************************************/

#define RC06y0_Acse_apduType                unsigned long  int

#define RC06y1_Aarq_protoVersion            unsigned long  int
#define RC06y2_Aarq_appliContext            unsigned short int
#define RC06y3_Aarq_calledApTitl            unsigned short int
#define RC06y4_Aarq_calingApTitl            unsigned short int

#define RC06y5_Aare_protoVersion            unsigned long  int
#define RC06y6_Aare_appliContext            unsigned short int
#define RC06y7_Aare_associResult            signed   long  int
#define RC06y8_Aare_resultSource            unsigned long  int
#define RC06y8_Aare_diagnostic              signed   long  int
#define RC06ya_Aare_responApTitl            unsigned short int

#define RC06yb_Rlrq_rlrqReason              signed   long  int

#define RC06yc_Rlre_rlreReason              signed   long  int

#define RC06yd_Abrt_aboSource               signed   long  int

/* *****************************************************************  */
/* ************ Function protype for Sni PTS EN/Decoding ***********  */


signed short int PASCAL AARQSniP_type_cod PROTO ((
unsigned long int       ,
rT0641_aarqApdu   FAR * ,
rT0650_acse       NEAR* ));


signed short int PASCAL AARESniP_type_cod PROTO ((
unsigned long int       ,
rT0642_aareApdu   FAR * ,
rT0650_acse       NEAR* ));

signed short int PASCAL RLRQSniP_type_cod PROTO ((
unsigned long int       ,
rT0643_rlrqApdu   FAR * ,
rT0650_acse       NEAR* ));

signed short int PASCAL RLRESniP_type_cod PROTO ((
unsigned long int       ,
rT0644_rlreApdu   FAR * ,
rT0650_acse       NEAR* ));

signed short int PASCAL ABRTSniP_type_cod PROTO ((
unsigned long int       ,
rT0645_abrtApdu   FAR * ,
rT0650_acse       NEAR* ));

signed short int PASCAL AARQSniP_type_dec PROTO ((
unsigned long int       ,
rT0641_aarqApdu   FAR * ,
rT0650_acse       NEAR* ));

signed short int PASCAL AARESniP_type_dec PROTO ((
unsigned long int       ,
rT0642_aareApdu   FAR * ,
rT0650_acse       NEAR* ));

signed short int PASCAL RLRQSniP_type_dec PROTO ((
unsigned long int       ,
rT0643_rlrqApdu   FAR * ,
rT0650_acse       NEAR* ));

signed short int PASCAL RLRESniP_type_dec PROTO ((
unsigned long int       ,
rT0644_rlreApdu   FAR * ,
rT0650_acse       NEAR* ));

signed short int PASCAL ABRTSniP_type_dec PROTO ((
unsigned long int       ,
rT0645_abrtApdu   FAR * ,
rT0650_acse       NEAR* ));

#endif	/* _ROACPMZY_H */
