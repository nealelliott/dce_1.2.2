/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropm3.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:21  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:01  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:09  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:20  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:22  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:31  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:18  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:57  keutel
 * 	creation
 * 	[1994/03/21  13:32:50  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROROPM3_H
#define _ROROPM3_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roropm3_rcsid[] = { "@(#) $RCSfile: roropm3.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:21 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1993                     */
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
/* NAME         : roropm3.h                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-06                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions Service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Protocol machine service interface                  */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropm3.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-02-06|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ********************************************************************/
/* *********************** ROPM CONSTANTS *****************************/


/* ****************** States of the extened finite state machine **** */
/* ****************** as defined in ISO 9072-2                   **** */


#define  RC0730_maxStates1	3	   /*  ROPM automata states   */

#define  RC0731_STAnoPm 	0	   /*  RO-PM aktiv state      */
#define  RC0732_STA01		1
#define  RC0733_STA02		2

#define  RC0734_maxStates2	3	   /*  ROPM-Tr (presentation  */

#define  RC0735_STAnoTr1        0          /*  mapping automata)      */
#define  RC0736_STA100		1	   /*  states		      */
#define  RC0737_STA200		2

#define RC0738_maxStates3       6          /* ROPM-Tr (Rtse )         */

#define RC0739_STAnoTr2         0
#define RC073a_STA10            1
#define RC073b_STA20            2
#define RC073c_STA21            3
#define RC073d_STA22            4
#define RC073e_STA23            5


/* ****************** Definition of APDUua limit ******************** */
/* ******************                            ******************** */

#define  RC073f_MaxAPDUua      2

/* ****************** Definition of subAutoNum   ******************** */
/* ******************                            ******************** */
#define RC073g_RopmPm          1
#define RC073h_RopmPrTr        2
#define RC073i_RopmRtTr        3
#define RC073j_End             0

/* **************** Return Values from the Automata Functions ******* */

#define RC073k_PushOutEvent    1
#define RC073l_ProviderReject  2
#define RC073m_ProviderAbort   3
#define RC073n_UnBindNotIniti  4 
#define RC073o_RejectCountAbo  5 
#define RC073p_ReleaseNotAcpt  6 
/* ***************** Events arranged for the state-Event automata *** */
/* **************** for table A-6 of 9072-2 RO-PM ******************* */

#define RC073A_BindReq           RC0701_RO_BINDreq
#define RC073B_BindRsp           RC0703_RO_BINDrsp 
#define RC073C_UnBindReq         RC0705_RO_UNBINDreq 
#define RC073E_UnBindRsp         RC0707_RO_UNBINDrsp 
#define RC073F_AbortReq          RC0709_RO_UABORTreq 

#define RC073G_ROINVreq          RC070c_RO_INVreq 
#define RC073H_RORESreq          RC070e_RO_RESreq 
#define RC073I_ROERRreq          RC070g_RO_ERRreq
#define RC073J_RORJUreq          RC070i_RO_RJUreq 

#define RC073K_AA_StartESTAB     (RC0700_RoseEvent |0x14) 
#define RC073L_AA_ESTAB          (RC0700_RoseEvent |0x15) 
#define RC073M_AA_REL            (RC0700_RoseEvent |0x16) 
#define RC073N_AA_EndREL         (RC0700_RoseEvent |0x17) 
#define RC073O_AA_ABind          (RC0700_RoseEvent |0x18) 

#define RC073P_TRANSind          (RC0700_RoseEvent |0x19)

#define RC073Q_ROINV             (RC0700_RoseEvent |0x1a) 
#define RC073R_RORES             (RC0700_RoseEvent |0x1b) 
#define RC073S_ROERR             (RC0700_RoseEvent |0x1c) 
#define RC073T_RORJU             (RC0700_RoseEvent |0x1d) 
#define RC073U_RORJP             (RC0700_RoseEvent |0x1e) 
#define RC073V_APDUua            (RC0700_RoseEvent |0x1f) 

/* **** Events for the state-Event table for RO-Tr pres  *********** */
#define RC073W_AA_ASSOCind       RC0602_AA_ASSOCIATEind
#define RC073X_AA_ASSOCcnf       RC0604_AA_ASSOCIATEcnf  
#define RC073Y_AA_RELEASEind     RC0606_AA_RELEASEind 
#define RC073Z_AA_RELEASEcnf     RC0608_AA_RELEASEcnf  
#define RC073z_P_ABORTind        RC060b_AA_PABORTind  
#define RC073y_ABORTind          RC060a_AA_ABORTind  

#define RC073x_P_DATAind         RC040d_PDATAind
  

/* *******for StartESTAB
              ESTAB
              REL
              EndREL use the same as above ************************* */

#define RC073w_TRANSreq        (RC0700_RoseEvent |0x20) 
#define RC073v_AA_ABreq        (RC0700_RoseEvent |0x21)

/* ****************** Events for State-Event Matrix for RO-Tr Rtse * */
#define RC0740_RT_OPENind      RC1002_RT_OPENind
#define RC0741_RT_OPENcnf      RC1004_RT_OPENcnf
#define RC0742_RT_CLOSEind     RC1006_RT_CLOSEind
#define RC0743_RT_CLOSEcnf     RC1008_RT_CLOSEcnf 
#define RC0744_RT_P_ABORTind   RC100g_RT_P_ABORTind  
#define RC0745_RT_U_ABORTind   RC100i_RT_U_ABORTind  

#define RC0746_RT_TRind        RC100a_RT_TRANSFERind
#define RC0747_RT_TRcnfPos     (RC0700_RoseEvent |0x22) 
#define RC0748_RT_TRcnfNeg     (RC0700_RoseEvent |0x23)
#define RC0749_RT_TPind        RC100d_RT_TURN_PLEASEind
#define RC074A_RT_TGind        RC100f_RT_TURN_GIVEind

/* *******for StartESTAB
              ESTAB
              REL
              EndREL 
              TRANSreq
              ABreq     use the same as above ************************* */

/* ****************** Session definitions *************************** */
/* ******************			  *************************** */

#define RC074a_AnyROIVapdu   161
#define RC074b_AnyRORSapdu   162
#define RC074c_AnyROERapdu   163
#define RC074d_AnyRORJapdu   164


/* ****************** Logging record type *************************** */
/* ******************			  *************************** */

#define  RC073x_ACTraROPM  -300L

/* ****************** Structure of ROSE protocol machine ************ */
/* ****************** invocation                         ************ */

typedef struct	 rS0730_stContext  {
	signed	 short int    sStateRopm;
	signed	 short int    sStateTrPm;
	signed	 short int    sReleaseFg;
	signed	 short int    sSubAutoNum;
	signed	 short int    bAssocInit;
	unsigned long  int    ulAssocClas;
        signed   short int    bToken;
        signed   short int    sAbsIdOp;
	signed	 short int    bSyncFlag;
	signed	 long  int    lInvokeId;
	signed	 short int    sRejCount;
        signed   short int    bmappingInfo;
        unsigned long  int    hFirst;
        unsigned long  int    hLast;
} rT0730_stContext;


/* ****************** Structure of ROSE Transfer Req ************ */
/* ****************** Queue                          ************ */

typedef struct rS0731_stTransQue{
     signed   long  int lEvent    ;       /* Events   */
     unsigned long  int vParams[4];       /* parameters P1,P2,P3,& P4 */
     unsigned long  int hNext    ;       /* Handle for the Next element*/
}rT0731_stTransQue;

 
/* ************************************************************** */

#endif	/* _ROROPM3_H */
