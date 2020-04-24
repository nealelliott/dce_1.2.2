/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm03.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:53  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:39  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:56  keutel
 * 	OT 11620
 * 	[1994/08/19  13:49:02  keutel]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:47  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:23  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:24  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:49  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:19  keutel
 * 	creation
 * 	[1994/03/21  13:27:05  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPM03_H
#define _ROPPM03_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppm03_rcsid[] = { "@(#) $RCSfile: roppm03.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:53 $" } ;
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
/* NAME         : roppm03.h                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-06-29                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Protocol machine logging interface                  */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppm03.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-03-24|  adaptation of ROS V2.5A00 K05 |hm  |         */
/* 2.0A00 K02|92-06-29|  comments PPM context improved |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/***************************************************************************/
/*********            PPM CONSTANTS                              ***********/
/***************************************************************************/

/***************************************************************************/
/* Constants derived from ISO 8822/8823                                    */
/***************************************************************************/

/***************************************************************************/

/* States of the extended finite state machine */

#define  RC0430_StaI0           0
#define  RC0431_StaI1           1
#define  RC0432_StaI2           2
#define  RC0433_Stat0           3

#define  RC0434_MaxStates       4


/***************************************************************************/

/* Incoming events of the finite state machine */

/* N O T E :  Be  V E R Y   C A R E F U L  if you want to change      */
/*            numeric values of event numbers !                       */
/*            (The numbers are used as indices in State-Event-Matrices*/
/*            or their right sequence is necessary for defined ranges */
/*            of event numbers.)                                      */

#define  RC0435_MaxInEvent      19

#define  RC0436_IEvPCONreq      RC0401_PCONNECTreq
#define  RC0437_IEvCP           RC0802_SCONNECTind
#define  RC0438_IEvPCONrspPos   RC0403_PCONNECTrsp
#define  RC0439_IEvCPA          RC0804_SCONNECTcnf
#define  RC043a_IEvPCONrspNeg   0x0604
#define  RC043b_IEvCPR          0x0505
#define  RC043c_IEvSCONcnfNeg   0x0506

#define  RC043d_IEvPRELreq      RC0405_PRELEASEreq
#define  RC043e_IEvSRELind      RC0806_SRELEASEind
#define  RC043f_IEvPRELrspPos   RC0407_PRELEASErsp
#define  RC043g_IEvSRELcnfPos   RC0808_SRELEASEcnf
#define  RC043h_IEvPRELrspNeg   0x060b
#define  RC043i_IEvSRELcnfNeg   0x050c

#define  RC043j_IEvPUABreq      RC0409_PUABORTreq
#define  RC043k_IEvARU          RC080a_SUABORTind  /*not mapped (good for ARP decode error) */
#define  RC043l_IEvARP          0x050f
#define  RC043m_IEvSPABind      RC080b_SPABORTind

#define  RC043n_IEvPDTreq       RC040c_PDATAreq
#define  RC043o_IEvTD           RC080d_SDATAind


/***************************************************************************/

/* Outgoing events of the finite state machine */

#define  RC043p_MaxOutEvent     19

#define  RC043q_OEvCP           RC0801_SCONNECTreq
#define  RC043r_OEvCPA          RC0803_SCONNECTrsp
#define  RC043s_OEvCPR          RC0803_SCONNECTrsp
#define  RC043t_OEvPCONind      RC0402_PCONNECTind
#define  RC043u_OEvPCONcnfPos   RC0404_PCONNECTcnf
#define  RC043v_OEvPCONcnfNeg   RC0404_PCONNECTcnf

#define  RC043w_OEvSRELreq      RC0805_SRELEASEreq
#define  RC043x_OEvSRELrspPos   RC0807_SRELEASErsp
#define  RC043y_OEvSRELrspNeg   RC0807_SRELEASErsp
#define  RC043z_OEvPRELind      RC0406_PRELEASEind
#define  RC043A_OEvPRELcnfPos   RC0408_PRELEASEcnf
#define  RC043B_OEvPRELcnfNeg   RC0408_PRELEASEcnf

#define  RC043C_OEvARU          RC0809_SUABORTreq
#define  RC043D_OEvPUABind      RC040a_PUABORTind
#define  RC043E_OEvPPABind      RC040b_PPABORTind

#define  RC043F_OEvTD           RC080c_SDATAreq
#define  RC043G_OEvPDTind       RC040d_PDATAind


/***************************************************************************/

/* Event ranges which have to be passed transparently by the PPM (to or from SPM) */

/*incoming events */
#define  RC043H_MinEventDirectFromUser  RC040e_PTOKEN_PLEASEreq
#define  RC043I_MaxEventDirectFromUser  RC040D_PACT_DISCARDrsp

#define  RC043J_MinEventDirectFromSpm   RC080f_STOKEN_PLEASEind
#define  RC043K_MaxEventDirectFromSpm   RC080G_SSYNCPOINT


/*outgoing event --> max-value not necessary */
#define  RC043L_MinEventDirectToUser    RC040f_PTOKEN_PLEASEind


/***************************************************************************/
/* Constants which are implementation specific                             */
/***************************************************************************/

/***************************************************************************/

/* Initial values of syntax array elements in the PPM context */

#define RC043M_NoAbsId      -1      /*for offline-troubleshooting           */
#define RC043N_NoSynResult  RCro6e_ReasonNotSpecified
                                    /*for offline-troubleshooting           */
                                    /*this value to ensure right encoding   */
                                    /*even if undefined local problem       */
#define RC043O_NoTransId    -1      /*not only for troubleshooting !        */
                                    /*indicates that a pres. context id. is */
                                    /*not valid                             */
                                    /*used as return value of the function  */
                                    /*ro0425_getTransId() too               */
#define RC043P_NoContId     -2L     /*for offline-troubleshooting           */
                                    /*this value because even integers may  */
                                    /*occur only if context management is   */
                                    /*in use (otherwise odd integers)       */


/***************************************************************************/

/* Initial value of CPR provider reason        */
/* (in the PPM context and for local variable) */

#define RC043Q_NoReject     -1      /*means: issue CPA, not CPR */

/***************************************************************************/
/*********            PPM DATA TYPES                             ***********/
/***************************************************************************/


/* Note: divide the names P context and PPM context!                    */
/* P context  : presentation context (negotiated abstract and transfer  */
/*              syntaxes, see ISO 8822/8823)                            */
/* PPM context: presentation protocol machine context (data block which */
/*              describes the status of the PPM, implementation speci-  */
/*              ficly defined)                                          */


/***************************************************************************/

/* PPM context */

typedef struct rS0430_stContext {
   signed   short int      sState;
   signed   short int      bRelease;
   signed   short int      bReleaseCollision;
   signed   short int      bNormalMode;
   signed   short int      sSessProtVers;

   rTro61_vAbstrSynResult  vAbstrSynResult;          /* stDcs */
   rTro60_vAbstrSynId      vAbstrSynId;
   signed   short int      vTransSynId [RCro01_MaxAbstrSyn];
   signed   long  int      vPresContId [RCro01_MaxAbstrSyn];
   signed   short int      sDefContResult;
   signed   short int      sDefAbstrSynId;
   signed   short int      sDefTransSynId;
   signed   short int      sDcsNumb;

   rTro76_stDefinedBy FAR *pDefinedBy;               /* stGlobal (for ANY) */
   unsigned long  int FAR *pTransSynName;
   signed   short int      sAbstrSynId;

   rT0441_stEntContext    *pEntContext;              /* stGlobal (other) */
   signed   long  int      lProviderReason;
   signed   long  int      lType;
   signed   short int      vTransSynNumbInReceivedCp [RCro01_MaxAbstrSyn];
   signed   short int      sTransSynIdAbo;

/*   unsigned long  int      ulCp;                     */ /* Redirection  */
   signed   long  int      bUserData;          /* Added for having a varaible
                                                  which holds always TRUE for
                                                  for the PDUs which has User
                                                  data as mandatory.
                                                    (ex. TD ) */
  signed long int         lIndefCount;
          
} rT0430_stContext;

/*
   part: see ISO 8823
   - sState         : Current state of the PPM.
   - bRelease       : Release flag.
   - bReleaseCollision : Release collision flag.
   - bNormalMode    : TRUE: normal-mode connection.
   - sSessProtVers  : Session protocol version of the used session connection.

   part: syntax handling
   - vAbstrSynResult: Result or provider reason,
                      first element: number of AbsId's.
   - vAbstrSynId    : Abstract syntax identifiers (AbsId's),
                      agreed during connection establishment,
                      first element: number of AbsId's (not evaluated,
                      redundant to vAbstrSynResult P!?).
   - vTransSynId    : Defined context set: transfer syntax identifiers (TransId's),
                      agreed during connection establishment,
                      the value RC043O_NoTransId indicates that a pres. context
                      identifier is not valid,
                      vector index: AbsId.
   - vPresContId    : Defined context set: presentation context identifiers,
                      agreed during connection establishment,
                      vector index: AbsId.
   - sDefContResult : Default context: result.
   - sDefAbstrSynId : Default context: abstract syntax.
   - sDefTransSynId : Default context: transfer syntax.
   - sDcsNumb       : Defined context set: number of presentation contexts.

   part: E/D Control, stored for ANY/ANY DEFINED BY
   - pDefinedBy     : ANY DEFINED BY encoding information (pointer to).
   - pTransSynName  : Transfer syntax object identifier (pointer to).
   - sAbstrSynId    : Abstract syntax identifier (AbsId).


   part: quasi-global variables
   - pEntContext    : Pointer to presentation entity invocation context.
                      (better "void *" to avoid reference to rT0441_stEntContext
                      which is defined in roppm04.h P!?).
   - lProviderReason: CPR provider reason.
   - lType          : PPDU type (used in E/D Control).
   - vTransSynNumbInReceivedCp : Array of numbers of proposed transfer 
                      syntaxes in the received CP PPDU.
                      Used to decide if a transfer syntax object identifier
                      is expected in a PDV-list of the received CP PPDU.
                      (Because of MAVROS conventions it is not possible to
                      give the pointer to PDV node to ro0116_decodePdvList().)
                      Not used in the current version !!
   - sTransSynIdAbo : Transfer syntax identifier of the last PDV-list or 
                      EXTERNAL in ARU PPDU (i.e. transfer syntax of abort
                      products user data if there are products user data,
                      otherwise the identifier has no relevance).
                      Given as P4 parameter of PUABORTind to the PS-user
                      (really necessary only if PUABORTind before PCONNECTcnf
                      or PCONNECTrsp).
*/

#endif	/* _ROPPM03_H */
