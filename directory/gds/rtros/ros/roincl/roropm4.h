/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropm4.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:22  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:02  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:11  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:23  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:24  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:33  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:24  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:59  keutel
 * 	creation
 * 	[1994/03/21  13:32:55  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROROPM4_H
#define _ROROPM4_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roropm4_rcsid[] = { "@(#) $RCSfile: roropm4.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:22 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1992                     */
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
/* NAME         : roropm4.h                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-05                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Protocol machine service interface                  */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropm4.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-02-05|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/



signed short int PASCAL ro0720_localError PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0721_STA00xBindReq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0722_STA00xStartESTAB PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0723_STA01xBindRsp PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro0724_STA01xUNBINDrsp PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0725_STAXXxABORTreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0726_STA10xAA_ESTAB PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro0727_STA01xEndREL PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0728_STAXXxAA_ABind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0729_STA02_UNBINDreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro072a_STA02xINVreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro072b_STA02xRESreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro072c_STA02xERRreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro072d_STA02xRJUreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro072e_STA02xAA_REL PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro072f_STA02xROINV PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro072g_STA02xRORES PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro072h_STA02xROERR PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro072i_STA02xRORJu PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro072j_STA02xRORJp PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro072k_STA02xAPDUua PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));



/* -------- Automata Function prototypes for Tr-Prestation Table ----- */

signed short int PASCAL ro0730_STA000xstartESTAB PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));
signed short int PASCAL ro0731_STA000xASSOCind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0732_STA100xAA_ESTAB PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0733_STA000xAA_EndREL PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0734_STAXXXxAA_ABreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0735_STA100xAA_ASSOCcnf PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0736_STA100xAA_RELEASEcnf PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0737_STAXXXxABORTind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0738_STA200xAA_REL PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0739_STA200xTRANSreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro073a_STA200xP_DATAind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro073b_STA200xAA_RELEASEind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));




/* **************************************************************** */
/*    Automata Table A.7. Functions (With RTSE )                    */      
/* **************************************************************** */

signed short int PASCAL ro0740_STA00xStartESTAB PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *, 
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0741_STA00xRT_OPENind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0742_STA10xAA_ESTAB PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0743_STA10xAA_EndREL PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0744_STA20_22xAA_ABreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0745_STA10xRT_OPENcnf PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0746_STA10xRT_CLOSEcnf PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0747_STA20_22xABORTind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0748_STA20xAA_REL PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro0749_STA20xTRANSreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074a_STA20xRT_CLOSEind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074b_STA20xRT_TURN_PLEASEind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074c_STA21xAA_REL PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074d_STA21_23xAA_ABreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074e_STA21xRT_TRcnfPos PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));


signed short int PASCAL ro074f_STA21xRT_TRcnfNeg PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074g_STA21xRT_TPind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074h_STA22xTRANSreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074i_STA22xRT_TRind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074j_STA22xRT_TGind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074k_STA23xRT_TRind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074l_STA23xRT_TGind PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));

signed short int PASCAL ro074m_STA21_23xTRANSreq PROTO ((
        rT0730_stContext     NEAR *,
        signed   long  int   FAR  *,
        signed   long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *,
        unsigned long  int   FAR  *));


#endif	/* _ROROPM4_H */
