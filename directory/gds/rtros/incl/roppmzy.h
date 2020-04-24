/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppmzy.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:57  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:49  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:47  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:50  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:26  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:56  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:25  keutel
 * 	creation
 * 	[1994/03/21  13:27:34  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPMZY_H
#define _ROPPMZY_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppmzy_rcsid[] = { "@(#) $RCSfile: roppmzy.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:57 $" } ;
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
/* NAME         : roppmzy.h                                           */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE         : 93-04-09                                            */
/*                                                                    */
/* COMPONENT    : ROS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : ROS-V3.00                                           */
/*                                                                    */
/* DESCRIPTION  : Encode/Decode    Private Transfer Syntax:           */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/*datoff **************************************************************/

#define RC04y0_PriLengthModeSelector      7 
#define RC04y1_PriVersion1                1

#define RC04y2_MskPCp                     1 
#define RC04y3_MskPCpa                    2 
#define RC04y4_MskPCpr                    3 
#define RC04y5_MskPAbo                    4 
#define RC04y6_MskPOther                  5 
#define RC04y7_MskAcse                    10
#define RC04y8_MskRoseOctet               20
#define RC04y9_MskRoseBER                 21

/* common transferred variables */

#define RC04ya_sPrivVersion                 signed   short int
#define RC04yb_sPrivPciId                   signed   short int

/* PPCI Cp*/

#define RC04yc_lCallingSelectorLength      unsigned long  int
#define RC04yd_ulCalledSelectorLength      unsigned long  int
#define RC04ye_lPresContDef                signed   long  int
#define RC04yf_vPresContId                 signed   long  int
#define RC04yg_AbstrSynName                unsigned short int
#define RC04yh_vTransSynNumber             signed   long  int
#define RC04yi_vTransSynName               unsigned short int


#define RC04yj_lRespondingSelectorLength   unsigned long  int
#define RC04yk_lPresContDefRes             signed   long  int   
#define RC04yl_vResult                     signed   long  int
#define RC04ym_vProviderReason             signed   long  int

#define RC04n_bArp                         signed   short int
#define RC04o_lAbortReason                 signed   long  int
#define RC04p_lEventId                     signed   long  int

#endif	/* _ROPPMZY_H */
