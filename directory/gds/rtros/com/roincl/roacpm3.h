/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpm3.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:35  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:23  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:11  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:23  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:23  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:45  keutel
 * 	creation
 * 	[1994/03/21  13:18:41  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROACPM3_H
#define _ROACPM3_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roacpm3_rcsid[] = { "@(#) $RCSfile: roacpm3.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:35 $" } ;
#endif

/* *************************************************************************/
/*                                                                         */
/*  * COPYRIGHT  (C)                                                       */
/*  * Siemens Nixdorf Informationssysteme AG 1992                          */
/*  * All rights reserved                                                  */
/*  *                                                                      */
/*  * This software is furnished under licence and may be used only        */
/*  * in accordance with the terms of that licence and with the            */
/*  * inclusion of the above copyright notice.                             */
/*  * This software may not be provided or otherwise made available        */
/*  * to, or used by, any other person. No title to or ownership of        */
/*  * the software is hereby  transferred.                                 */
/*                                                                         */
/* *************************************************************************/

/*daton ********************************************************************/
/*                                                                         */
/* TYPE         : INCLUDE                                                  */
/*                                                                         */
/* NAME         : roacpm3.h                                                */
/*                                                                         */
/* AUTHOR       : Johann Mueller, Michael Rippstain                        */
/* DATE         : 92-01-09                                                 */
/*                                                                         */
/* COMPONENT    : Association Control Service Element (ACSE)               */
/*                Protocol Machine                                         */
/*                                                                         */
/* PRD#/VERS.   : RTS-V2.00                                                */
/*                                                                         */
/* DESCRIPTION  : Internal definitions                                     */
/*                                                                         */
/* SYSTEM DEPENDENCE :  none                                               */
/*                                                                         */
/* HISTORY      : sccsid  =  @(#)roacpm3.h	1.1 94/01/26                                       */
/*                                                                         */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM#      */
/* 2.00 K0  | 92-01-09|  original                      |mr  |              */
/*          |         |                                |    |              */
/*datoff *******************************************************************/

/* mr?: das prototypiing der matrix fct ist nachzuziehen
*/


/* ****************** States of the extened finite state ***************** */
/* ****************** machine as defined in ISO 8650.    ***************** */

#define RC0650_MaxStates         8

#define RC0651_STA0              0
#define RC0652_STA1              1
#define RC0653_STA2              2
#define RC0654_STA3              3
#define RC0655_STA4              4
#define RC0656_STA5              5
#define RC0657_STA6              6
#define RC0658_STA7              7

/* ****************** Incomming events of the finite state *************** */
/* ****************** machine as defined in ISO 8650.      *************** */

#define  RC0660_MaxInEvents     18

#define  RC0661_A_ASCreq        RC0601_AA_ASSOCIATEreq
#define  RC0662_A_ASCrsp_pos    RC0603_AA_ASSOCIATErsp
#define  RC0663_A_ASCrsp_neg    (RC0600_AcpmEvent + 2)
#define  RC0664_AARQ            RC0402_PCONNECTind
#define  RC0665_AARE_pos        RC0404_PCONNECTcnf
#define  RC0666_AARE_neg        (RC0400_PpmEvent  + 5)
#define  RC0667_P_CONcnf_neg    (RC0400_PpmEvent  + 6)

#define  RC0668_A_RLSreq        RC0605_AA_RELEASEreq
#define  RC0669_A_RLSrsp_pos    RC0607_AA_RELEASErsp
#define  RC0670_A_RLSrsp_neg    (RC0600_AcpmEvent + 9)
#define  RC0671_RLRQ            RC0406_PRELEASEind
#define  RC0672_RLRE_pos        RC0408_PRELEASEcnf
#define  RC0673_RLRE_neg        (RC0400_PpmEvent  +12)

#define  RC0674_A_ABRreq        RC0609_AA_ABORTreq
#define  RC0675_ABRT            RC040a_PUABORTind
#define  RC0676_P_PABind        RC040b_PPABORTind

#define  RC0677_EXTRN_1         RC060c_AA_EXTERN_1
#define  RC0678_EXTRN_2         RC060d_AA_EXTERN_2

/* ****************** Outgoing events of the finite state machine ******** */
/* ****************** as defined in ISO 8650                       ******* */

#define  RC0680_maxOUTEVENT     15

#define  RC0681_A_ASCind        RC0602_AA_ASSOCIATEind
#define  RC0682_A_ASCcnf_pos    RC0604_AA_ASSOCIATEcnf
#define  RC0683_A_ASCcnf_neg    RC0604_AA_ASSOCIATEcnf
#define  RC0684_AARQ            RC0401_PCONNECTreq
#define  RC0685_AARE_pos        RC0403_PCONNECTrsp
#define  RC0686_AARE_neg        RC0403_PCONNECTrsp

#define  RC0687_A_RLSind        RC0606_AA_RELEASEind
#define  RC0688_A_RLScnf_pos    RC0608_AA_RELEASEcnf
#define  RC0689_A_RLScnf_neg    RC0608_AA_RELEASEcnf
#define  RC0690_RLRQ            RC0405_PRELEASEreq
#define  RC0691_RLRE_pos        RC0407_PRELEASErsp
#define  RC0692_RLRE_neg        RC0407_PRELEASErsp

#define  RC0693_A_ABRind        RC060a_AA_ABORTind
#define  RC0694_ABRT            RC0409_PUABORTreq
#define  RC0695_A_PABind        RC060b_AA_PABORTind

/* ****************** Structure of acse protocol machine ***************** */
/* ****************** invocation                         ***************** */

typedef struct rS0650_acse               {
	signed short int    sState        ;
	signed short int    sAssocInitFlag;
	signed short int    sSpmProtoVers ;
	signed short int    sAcseAbsId    ;
	signed long  int    lMode        ;
      /*  unsigned long int   hAARQapdu    ; *//* changes  .. vinita - for     */
} rT0650_acse;                        /* storing AARQ apdu for redirection */  

/*  This structure notes the actual values for the acse-PM.
    He lives along one invocation.

    -sState         : state of the PM (see roacpm3.h)
    -sAssocInitFlag : association initiator flag, this flag notes who has
		      originated the PM;
			 0 : means the ACPM is initiator
			 1 : means the ACPM is responder
    -sSpmProtoVers  : session protocol version;    RCro50_ProtVers1
						or RCro51_ProtVers2
    -sAcseAbsId     : acse abstract syntax identifier
    -lMode         : mode selector;     RCro70_X400Mode1984
				     or RCro71_NormalMode
				     or RCro72_SNIprivate
*/

/* ****************** Function prototyping ******************************* */
/* ******************                      ******************************* */

signed short int NEAR PASCAL
	      ro0608_providerAbort PROTO ((signed   long  int       ,
					   unsigned long  int       ,
					   rT0650_acse        NEAR *,
					   signed   long  int       ,
					   unsigned long  int       ,
					   unsigned long  int       ,
					   unsigned long  int       ,
					   unsigned long  int        ));



            signed  short int NEAR PASCAL ro060d_CheckLegal PROTO ((
                     unsigned long  int            hContext,              
                     void                     FAR *pInfo ));     

#endif	/* _ROACPM3_H */
