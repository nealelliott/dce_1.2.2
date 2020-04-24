/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropm5.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:23  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:13  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:26  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:25  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:34  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:28  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:01  keutel
 * 	creation
 * 	[1994/03/21  13:32:59  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROROPM5_H
#define _ROROPM5_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roropm5_rcsid[] = { "@(#) $RCSfile: roropm5.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:23 $" } ;
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
/* NAME         : roropm5.h                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-31                                            */
/*                                                                    */
/* COMPONENT    : Remote Operation  service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : ROS-V3.0                                            */
/*                                                                    */
/* DESCRIPTION  : Automata table structure definitions.               */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropm5.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-03-31|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ************************************************************************** */
/*                                                                            */
/*           R O P M     A U T O M A T A     D E F I N I T I O N S            */
/*                                                                            */
/* ************************************************************************** */


/****************************************************************************/
/*                     A U T O M A T A      T A B L E                       */
/*                                                                          */
/*   Event-Action structure entries ordered in the array in the following   */
/*   fashion :                                                              */
/*                                                                          */
/*   All the State Table (ISO 9072-2 Table A.6, A.7 & A.8 )                 */
/*                                                                          */
/*   Events of Table A.6                                                    */
/*                                                                          */
/*   No.-of-events-of-first-state (n1)        Dummy-function                */
/*   Event-entry-1                            Action-of-event-entry-1       */
/*      .                                             .                     */
/*      .                                             .                     */
/*   Event-entry-n1                           Action-of-event-entry-n1      */
/*                                                                          */
/*   No.-of-events-of-second-state (n2)                                     */
/*      .                                                                   */
/*      .                                                                   */
/*   Events of Table A.7                                                    */
/*      .                                                                   */
/*      .                                                                   */
/*      .                                                                   */
/*   Events of Table A.8                                                    */
/*      .                                                                   */
/*      .                                                                   */
/*                                                                          */
/****************************************************************************/

typedef struct rS0740_stRopmAutomata {
    signed long  int          lInEvent;
    signed short int         (PASCAL *pAction) PROTO ((
                                                     rT0730_stContext  NEAR *,
                                                     signed   long int FAR  *,
                                                     signed   long int FAR  *,
                                                     unsigned long int FAR  *,
                                                     unsigned long int FAR  *,
                                                     unsigned long int FAR  *,
                                                     unsigned long int FAR  *));
} rT0740_stRopmAutomata;

#define RC0740_MaxActions  100     
                        /*(EventCounts = 12) + (Events = 074) < 100*/

static  rT0740_stRopmAutomata  vRopmAutomata [ RC0740_MaxActions ] = { 
                     

/* ** Modified Table A.6. of ISO 9072-2   *********************************** */
/* ** (States: 03, Events: 09)            *********************************** */

/* -- STA00: idle; unassociated (STAnoPm)  ---------------------------------- */
      2                   ,   ro0720_localError ,
      RC073A_BindReq      ,   ro0721_STA00xBindReq ,
      RC073K_AA_StartESTAB,   ro0722_STA00xStartESTAB ,

/* -- STA01: awaiting BindRsp,ESTAB,            ----------------------------- */
      12                  ,   ro0720_localError ,
      RC073B_BindRsp      ,   ro0723_STA01xBindRsp ,
      RC073E_UnBindRsp    ,   ro0724_STA01xUNBINDrsp,
      RC073F_AbortReq     ,   ro0725_STAXXxABORTreq,
      RC073L_AA_ESTAB     ,   ro0726_STA10xAA_ESTAB,
      RC073N_AA_EndREL    ,   ro0727_STA01xEndREL,
      RC073O_AA_ABind     ,   ro0728_STAXXxAA_ABind,

/* added on 30-9-93 by kc */
      RC073Q_ROINV        ,   ro072f_STA02xROINV,
      RC073R_RORES        ,   ro072g_STA02xRORES,
      RC073S_ROERR        ,   ro072h_STA02xROERR,
      RC073T_RORJU        ,   ro072i_STA02xRORJu,
      RC073U_RORJP        ,   ro072j_STA02xRORJp,
      RC073V_APDUua       ,   ro072k_STA02xAPDUua,

/* -- STA02: Active state of the Pm           ------------------------------- */
      14                  ,   ro0720_localError,
      RC073C_UnBindReq    ,   ro0729_STA02_UNBINDreq,
      RC073F_AbortReq     ,   ro0725_STAXXxABORTreq,

      RC073G_ROINVreq     ,   ro072a_STA02xINVreq,
      RC073H_RORESreq     ,   ro072b_STA02xRESreq,
      RC073I_ROERRreq     ,   ro072c_STA02xERRreq,
      RC073J_RORJUreq     ,   ro072d_STA02xRJUreq,

      RC073M_AA_REL       ,   ro072e_STA02xAA_REL,
      RC073O_AA_ABind     ,   ro0728_STAXXxAA_ABind,

      RC073Q_ROINV        ,   ro072f_STA02xROINV,
      RC073R_RORES        ,   ro072g_STA02xRORES,
      RC073S_ROERR        ,   ro072h_STA02xROERR,
      RC073T_RORJU        ,   ro072i_STA02xRORJu,
      RC073U_RORJP        ,   ro072j_STA02xRORJp,
      RC073V_APDUua       ,   ro072k_STA02xAPDUua,

   /* ------- Table A.8.  Extended -------------------------------------- */
   /* ------ STA000 (NoPm ) --------------------------------------------- */
   /* ----- States = 03 ------------------------------------------------- */
      2                   ,   ro0720_localError,
      RC073K_AA_StartESTAB,   ro0730_STA000xstartESTAB,
      RC073W_AA_ASSOCind  ,   ro0731_STA000xASSOCind,

/* -- STA100: Expecting Assoc Cnf   ------------------------------------- */
      8                   ,   ro0720_localError,
      RC073L_AA_ESTAB     ,   ro0732_STA100xAA_ESTAB,
      RC073N_AA_EndREL    ,   ro0733_STA000xAA_EndREL,
      RC073v_AA_ABreq     ,   ro0734_STAXXXxAA_ABreq,
      RC073X_AA_ASSOCcnf  ,   ro0735_STA100xAA_ASSOCcnf,
      RC073Z_AA_RELEASEcnf,   ro0736_STA100xAA_RELEASEcnf,
      RC073z_P_ABORTind   ,   ro0737_STAXXXxABORTind,
      RC073y_ABORTind     ,   ro0737_STAXXXxABORTind,
/* by kc on 30-9-93 */
      RC073x_P_DATAind    ,   ro073a_STA200xP_DATAind,
/* -- STA200 Active State After succussful Association           ------ */
      7                   ,   ro0720_localError,
      RC073M_AA_REL       ,   ro0738_STA200xAA_REL,
      RC073w_TRANSreq     ,   ro0739_STA200xTRANSreq,
      RC073v_AA_ABreq     ,   ro0734_STAXXXxAA_ABreq,
      RC073x_P_DATAind    ,   ro073a_STA200xP_DATAind,
      RC073Y_AA_RELEASEind,   ro073b_STA200xAA_RELEASEind,
      RC073z_P_ABORTind   ,   ro0737_STAXXXxABORTind,
      RC073y_ABORTind     ,   ro0737_STAXXXxABORTind,
/* -- Table A.7. of Ropm RT-Tr  --------------------------------------- */ 
/* ------ states = 06 ------------------------------------------------- */
/* ----- STA00 -------------------------------------------------------- */

      2                   ,    ro0720_localError,
      RC073K_AA_StartESTAB,    ro0740_STA00xStartESTAB,
      RC0740_RT_OPENind   ,    ro0741_STA00xRT_OPENind,
/* ----- STA10 -------------------------------------------------------- */

      7                   ,    ro0720_localError,
      RC073L_AA_ESTAB     ,    ro0742_STA10xAA_ESTAB,
      RC073N_AA_EndREL    ,    ro0743_STA10xAA_EndREL,
      RC073v_AA_ABreq     ,    ro0744_STA20_22xAA_ABreq,
      RC0741_RT_OPENcnf   ,    ro0745_STA10xRT_OPENcnf,
      RC0743_RT_CLOSEcnf  ,    ro0746_STA10xRT_CLOSEcnf,
      RC0744_RT_P_ABORTind,    ro0747_STA20_22xABORTind,
      RC0745_RT_U_ABORTind,    ro0747_STA20_22xABORTind,
/* ----- STA20 -------------------------------------------------------- */

      7                   ,    ro0720_localError,
      RC073M_AA_REL       ,    ro0748_STA20xAA_REL,
      RC073w_TRANSreq     ,    ro0749_STA20xTRANSreq,
      RC073v_AA_ABreq     ,    ro0744_STA20_22xAA_ABreq,
      RC0742_RT_CLOSEind  ,    ro074a_STA20xRT_CLOSEind,
      RC0749_RT_TPind     ,    ro074b_STA20xRT_TURN_PLEASEind,
      RC0744_RT_P_ABORTind,    ro0747_STA20_22xABORTind,
      RC0745_RT_U_ABORTind,    ro0747_STA20_22xABORTind,
/* ----- STA21 -------------------------------------------------------- */

      9                   ,    ro0720_localError,
      RC073M_AA_REL       ,    ro074c_STA21xAA_REL,
      RC073w_TRANSreq     ,    ro074m_STA21_23xTRANSreq,       /*Buf push */
      RC073v_AA_ABreq     ,    ro074d_STA21_23xAA_ABreq,
      RC0742_RT_CLOSEind  ,    ro0720_localError,
      RC0747_RT_TRcnfPos  ,    ro074e_STA21xRT_TRcnfPos,
      RC0748_RT_TRcnfNeg  ,    ro074f_STA21xRT_TRcnfNeg,
      RC0749_RT_TPind     ,    ro074g_STA21xRT_TPind,
      RC0744_RT_P_ABORTind,    ro0747_STA20_22xABORTind,
      RC0745_RT_U_ABORTind,    ro0747_STA20_22xABORTind,
/* ----- STA22 -------------------------------------------------------- */

      8                   ,    ro0720_localError,
      RC073M_AA_REL       ,    ro0748_STA20xAA_REL,
      RC073w_TRANSreq     ,    ro074h_STA22xTRANSreq,
      RC073v_AA_ABreq     ,    ro0744_STA20_22xAA_ABreq,
      RC0742_RT_CLOSEind  ,    ro0720_localError,
      RC073P_TRANSind     ,    ro074i_STA22xRT_TRind,
      RC074A_RT_TGind     ,    ro074j_STA22xRT_TGind,
      RC0744_RT_P_ABORTind,    ro0747_STA20_22xABORTind,
      RC0745_RT_U_ABORTind,    ro0747_STA20_22xABORTind,
/* ----- STA23 -------------------------------------------------------- */

      8                   ,    ro0720_localError,
      RC073M_AA_REL       ,    ro074c_STA21xAA_REL,
      RC073w_TRANSreq     ,    ro074m_STA21_23xTRANSreq,
      RC073v_AA_ABreq     ,    ro074d_STA21_23xAA_ABreq,
      RC0742_RT_CLOSEind  ,    ro0720_localError,
      RC073P_TRANSind     ,    ro074k_STA23xRT_TRind,
      RC074A_RT_TGind     ,    ro074l_STA23xRT_TGind,
      RC0744_RT_P_ABORTind,    ro0747_STA20_22xABORTind,
      RC0745_RT_U_ABORTind,    ro0747_STA20_22xABORTind,
     /* !!! check foe CLOSEind */    
};

#endif	/* _ROROPM5_H */
