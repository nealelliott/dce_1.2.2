/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rortpm0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:06  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:54  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:01  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:55  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:37  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:32  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:17  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:35  keutel
 * 	creation
 * 	[1994/03/21  13:27:52  keutel]
 * 
 * $EndLog$
 */
#ifndef _RORTPM0_H
#define _RORTPM0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rortpm0_rcsid[] = { "@(#) $RCSfile: rortpm0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:06 $" } ;
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
/* NAME         : rortpm0.h                                           */
/*                                                                    */
/* AUTHOR       : Shivkumar Haran                                     */
/* DATE         : 92-04-15                                            */
/*                                                                    */
/* COMPONENT    : Reliable transfer service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Protocol machine service interface                  */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rortpm0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 2.00   K0 |92-04-15|  original                      |skh |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ****************** Protocol machine basic event number ******************* */
/* ******************                                     ******************* */

#define RC1000_RtpmEvent          (0x00000900L)

/* ****************** Reliable Transfer Service events ********************** */
/* ******************                                  ********************** */

#define RC1001_RT_OPENreq         (RC1000_RtpmEvent | 0x00000000L)
#define RC1002_RT_OPENind         (RC1000_RtpmEvent | 0x00000001L)
#define RC1003_RT_OPENrsp         (RC1000_RtpmEvent | 0x00000002L)
#define RC1004_RT_OPENcnf         (RC1000_RtpmEvent | 0x00000003L)

#define RC1005_RT_CLOSEreq        (RC1000_RtpmEvent | 0x00000004L)
#define RC1006_RT_CLOSEind        (RC1000_RtpmEvent | 0x00000005L)
#define RC1007_RT_CLOSErsp        (RC1000_RtpmEvent | 0x00000006L)
#define RC1008_RT_CLOSEcnf        (RC1000_RtpmEvent | 0x00000007L)

#define RC1009_RT_TRANSFERreq     (RC1000_RtpmEvent | 0x00000008L)
#define RC100a_RT_TRANSFERind     (RC1000_RtpmEvent | 0x00000009L)
#define RC100b_RT_TRANSFERcnf     (RC1000_RtpmEvent | 0x0000000aL)

#define RC100c_RT_TURN_PLEASEreq  (RC1000_RtpmEvent | 0x0000000bL)
#define RC100d_RT_TURN_PLEASEind  (RC1000_RtpmEvent | 0x0000000cL)

#define RC100e_RT_TURN_GIVEreq    (RC1000_RtpmEvent | 0x0000000dL)
#define RC100f_RT_TURN_GIVEind    (RC1000_RtpmEvent | 0x0000000eL)

#define RC100g_RT_P_ABORTind      (RC1000_RtpmEvent | 0x0000000fL)

#define RC100h_RT_U_ABORTreq      (RC1000_RtpmEvent | 0x00000010L)
#define RC100i_RT_U_ABORTind      (RC1000_RtpmEvent | 0x00000011L)

#define RC100j_RT_BUFFER          (RC1000_RtpmEvent | 0x00000012L)

#define RC100k_RT_RECOVER         (RC1000_RtpmEvent | 0x00000013L)

/* ************************ 'ulDialogueMode' values ************************* */
/* ************************                         ************************* */

#define RC1010_Monologue          (0x00000000L) /* Only one way interaction   */
#define RC1011_TwoWayAlternate    (0x00000001L) /* Half duplex interaction    */
#define RC1012_DialogueModeAbsent (0x00000002L) /* Default MONOLOGUE selected */

/* ************************ 'ulInitialTurn' values ************************** */
/* ************************                        ************************** */

#define RC1015_TurnInitiator   (0x00000000UL)
#define RC1016_TurnResponder   ((unsigned long int) (RCro57_DataToken      |\
                                                     RCro58_MinorToken     |\
                                                     RCro59_ActivityToken  |\
                                                     RCro5a_ReleaseToken ) )

/* ************************ 'ulWindowSize' values *************************** */
/* ************************                       *************************** */

#define RC1019_CheckpointUnit  (0x00000400L)  /* Checkpointsize is in units of*/
                                              /* one kilobyte.                */

#define RC101a_WindowAbsent    (0x00000000L)  /*Partner selects windowSize = 0*/
#define RC101b_WindowDefault   (0x00000003L)  /* Default WindowSize if Absent */
#define RC101c_MinWindowSize   (0x00000001L)  /* Minimum windowSize     = 1   */
#define RC101d_MaxWindowSize   (MAXLONG_POS / RC1019_CheckpointUnit)
                                              /* Maximum windowSize cannot be */
                                              /* greater the maximum no. of   */
                                              /* checkpoints. Maximum no. of  */
                                              /* checkpoints is the maximum   */
                                              /* size of data possible (i.e.  */
                                              /* size of LONG) divided by     */
                                              /* RC101n_MinCheckpointSize     */
                                              /* expressed in BYTES and not   */
                                              /* in RC1019_CheckpointUnit.    */

/*
--------------------------------------------------------------------------------
|         |    Requestor      ReqProtocol      AccProtocol        Acceptor     |
|----------------------------------------------------------------------------  |
|  Send ->|       0     ->         3       ->       3      ->        3         |
|  Recv1<-|      wA     <-        wA       <-      wA      <-  1 <= wA <= 3    |
|  Recv2<-|       3     <-         3       <-       3      <-        0         |
|         |                                                                    |
|  Send ->| 1 <= wR <= Max   ->   wR       ->      wR      ->       wR         |
|  Recv <-|      wA          <-   wA       <-      wA      <-  1 <= wA <= wR   |
|         |                                                                    |
--------------------------------------------------------------------------------

wR  : Window Requestor
wA  : Window Acceptor
Max : RC101d_MaxWindowSize
*/

/* ************************ 'ulCheckPointSize' values *********************** */
/* ************************                           *********************** */

#define RC101k_CheckpointAbsent   (0x00000000L)  /*Checkpoint parameter absent*/
#define RC101l_CheckpointPartner  (0x00000000L)  /*Partner selects chkptsize  */
#define RC101m_NoCheckpointing    (0x00000000L)  /*Dont use checkpointing     */
#define RC101n_MinCheckpointSize  (0x00000001L)  /*Minimum checkpointSize = 1 */
#define RC101o_MaxCheckpointSize  (MAXLONG_POS / RC1019_CheckpointUnit)
                                                 /* Maximum checkpointsize in */
                                                 /* CheckpointUnits (and not  */
                                                 /* in bytes) is the maximum  */
                                                 /* size of data possible (i.e*/
                                                 /* of a LONG) divided by     */
                                                 /* RC101n_MinCheckpointSize. */
                                                 /* expressed in BYTES and NOT*/
                                                 /* in RC1019_CheckpointUnit. */
/*
--------------------------------------------------------------------------------
|         |    Requestor      ReqProtocol      AccProtocol        Acceptor     |
|------------------------------------------------------------------------------|
|  Send ->|       0     ->         0       ->       0      ->        0         |
|  Recv1<-|       0     <-         0       <-       0      <-        0         |
|  Recv2<-|      cA     <-        cA       <-      cA      <-  1 <= cA <= Max  |
|         |                                                                    |
|  Send ->| 1 <= cR <= Max   ->   cR       ->      cR      ->       cR         |
|  Recv1<-|       0          <-    0       <-       0      <-        0         |
|  Recv2<-|      cA          <-   cA       <-      cA      <-  1 <= cA <= cR   |
|         |                                                                    |
--------------------------------------------------------------------------------

cR  : Checkpoint requestor
cA  : Checkpoint acceptor
Max : RC101o_MaxCheckpointSize
*/

/* ****************** 'openResult' (Also see 'result' values in ************* */
/* ******************  roacpm0.h for structure & meaning)       ************* */

/*
  Result values when generated by ACSE or Presentation :
  ------------------------------------------------------
  Byte 3                          = Not used
  Byte 2                          = Not used
  Byte 1                          = ResultSource (see roacpm0.h)
  Byte 0, High Nibble             = Result       (see roacpm0.h)
  Byte 0, Low Nibble, Normal mode = Diagnostics  (see roacpm0.h)
  Byte 0, Low Nibble, X410 mode   = RefuseReason (see roacpm0.h)

  Note: Byte 2 is however used by the underlying layers (SPM/PPM/ACPM).
        Value used is RCro81_UserReject (0x00ff0000) (see rouply0.h)


  Result values when generated by the layers underlying Presentation :
  --------------------------------------------------------------------
  See respective layer header files for descriptions. For e.g. "rospm00.h"
  for roloif result values.
*/

#define RC101v_ValidationFailure  (0x00000002L) /* Validation failure         */
#define RC101w_UnaccDialogueMode  (0x00000003L) /* Unacceptable Dialogue Mode */

/* ****************************************************************** */
/* ****************** Connect parameter substructures *************** */
/* ******************                                 *************** */

/* ****************** Open request, RTPM **************************** */
/* ******************                    **************************** */

typedef struct rS1000_stReqRtpm {
   unsigned long  int     ulCheckpointSize;
   unsigned long  int     ulWindowSize;
   unsigned long  int     ulDialogueMode;
   unsigned long  int     ulInitialTurn;
   signed   long  int     lApplicationProtocol;
   signed   short int     sAbstrSynId;
} rT1000_stReqRtpm;

/* This substructure is passed to the RTPM during a RT-OPEN request event:

   - ulCheckpointSize      : Checkpoint size. Is in units of 1024 bytes.
   - ulWindowSize          : Window size
   - ulDialogueMode        : Monologue or two-way-alternate
   - ulInitialTurn         : Original owner of turn
   - lApplicationProtocol  : Application protocol
   - sAbstrSynId           : RTSE abstract syntax identifier
*/

/* ****************** Open indication, RTPM ************************* */
/* ******************                       ************************* */

typedef struct rS1001_stIndRtpm {
   unsigned long  int     ulDialogueMode;
   unsigned long  int     ulInitialTurn;
   signed   long  int     lApplicationProtocol;
   signed   short int     sAbstrSynId;
} rT1001_stIndRtpm;

/* This substructure is passed from the RTPM during a RT-OPEN indication
   event:

   - ulDialogueMode        : Monologue or two-way-alternate
   - ulInitialTurn         : Original owner of turn
   - lApplicationProtocol  : Application protocol
   - sAbstrSynId           : RTSE abstract syntax identifier
*/

/* ****************** Open response, RTPM *************************** */
/* ******************                     *************************** */

typedef struct rS1002_stRspRtpm {
   unsigned long  int    ulCheckpointSize;
   unsigned long  int    ulWindowSize;
} rT1002_stRspRtpm;

/* This substructure is passed to the RTPM during a RT-OPEN response
   event:

   - ulCheckpointSize: Checkpoint size
   - ulWindowSize    : Window size
*/

/* ****************** Open confirm, RTPM **************************** */
/* ******************                    **************************** */

/* No parameters exist. */

/* ****************************************************************** */
/* ****************** Complete structures, RTPM ********************* */
/* ******************                           ********************* */

/* ****************** Open request, RTPM **************************** */
/* ******************                    **************************** */

typedef struct rS100a_stOpenReqRtpm {
   rTro4a_stReqAll        stAll;
   rTro6a_stReqPpm        stPpm;
   rTro7a_stAcpm          stAcpm;
   rT1000_stReqRtpm       stRtpm;
} rT100a_stOpenReqRtpm;

/* This structure is passed to the RTPM during a RT-OPEN request event:

   - stAll           : Substructure: common               (see rouply0.h).
   - stPpm           : Substructure: presentation         (see rouply0.h).
   - stAcpm          : Substructure: ACSE                 (see rouply0.h).
   - stRtpm          : Substructure: RTSE                 (see above).
*/

/* ****************** Open indication, RTPM ************************* */
/* ******************                       ************************* */

typedef struct rS100b_stOpenIndRtpm {
   rTro4b_stIndAll        stAll;
   rTro6b_stIndPpm        stPpm;
   rTro7a_stAcpm          stAcpm;
   rT1001_stIndRtpm       stRtpm;
} rT100b_stOpenIndRtpm;

/* This structure is passed from the RTPM during a RT-OPEN indication
   event:

   - stAll           : Substructure: common               (see rouply0.h).
   - stPpm           : Substructure: presentation         (see rouply0.h).
   - stAcpm          : Substructure: ACSE                 (see rouply0.h).
   - stRtpm          : Substructure: RTSE                 (see above).
*/

/* ****************** Open response, RTPM *************************** */
/* ******************                     *************************** */

typedef struct rS100c_stOpenRspRtpm {
   rTro4c_stRspCnfAll     stAll;
   rTro6c_stRspPpm        stPpm;
   rTro7a_stAcpm          stAcpm;
   rT1002_stRspRtpm       stRtpm;
} rT100c_stOpenRspRtpm;

/* This structure is passed to the RTPM during a RT-OPEN response
   event:

   - stAll           : Substructure: common               (see rouply0.h).
   - stPpm           : Substructure: presentation         (see rouply0.h).
   - stAcpm          : Substructure: ACSE                 (see rouply0.h).
   - stRtpm          : Substructure: RTSE                 (see above).
*/

/* ****************** Open confirm, RTPM **************************** */
/* ******************                    **************************** */

typedef struct rS100d_stOpenCnfRtpm {
   rTro4c_stRspCnfAll     stAll;
   rTro6d_stCnfPpm        stPpm;
   rTro7a_stAcpm          stAcpm;
} rT100d_stOpenCnfRtpm;

/* This structure is passed from the RTPM during a RT-OPEN confirm
   event:

   - stAll           : Substructure: common               (see rouply0.h).
   - stPpm           : Substructure: presentation         (see rouply0.h).
   - stAcpm          : Substructure: ACSE                 (see rouply0.h).
*/

/* ******************** Transfer related definitions ************************ */
/* ********************                              ************************ */

/* -------------------- Transfer result values       ------------------------ */
#define RC1020_MaxTpFileName   (0x00000200L)  /* Not more than 512 bytes      */

#define RC1021_UdataAbsent     (0x00000000UL)
#define RC1022_UdataIsPdv      (0x00000001UL)
#define RC1023_UdataIsTpdu     (0x00000002UL)

typedef struct rS100n_stTpInfo {
    signed   long  int    lLenFileName;
    char                  vFileName [RC1020_MaxTpFileName];
} rT100n_stTpInfo;

#define RC1025_MaxTransferTime (0x0009ffffL)

/* ***************** Transfer result values ********************************* */
/* *****************                        ********************************* */


/*----------------------------------------------------------------------------*/
/* Structure of the transfer result value.                                    */
/*----------------------------------------------------------------------------*/

/*
    ___________________________________________________
   |   Byte 3   |   Byte 2   |   Byte 1   |   Byte 0   |
   |____________|____________|____________|____________|


  Byte 0, High nibble = RT_PAind follows => ORed with RC102A_ProvAbortFollows.
                                            Otherwise value of byte is 0.
  Byte 0, Low nibble  = Diagnostic.
  Byte 1              = Protocol machine event number
                        (see individual PM service interfaces)
  Byte 2              = Reserved for future use.
  Byte 3              = Reserved for future use.
*/

/*----------------------------------------------------------------------------*/
/* Values used for transfer result when RT-TRANSFERcnf indicates successful.  */
/*----------------------------------------------------------------------------*/

#define RC1027_TransferSuccess (0x00000000UL)


/*----------------------------------------------------------------------------*/
/* Values used for transfer result when RT-TRANSFERcnf indicates unsuccessful.*/
/*----------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* Value to be ORed with when a provider abort follows a RT-TRANSFERcnf.  */
    /*------------------------------------------------------------------------*/
#define RC102A_ProvAbortFollows       (0x00000010UL)

    /*------------------------------------------------------------------------*/
    /* One of the values that have to be ORed with for a Transfer failure.    */
    /*------------------------------------------------------------------------*/
#define RC102F_TpduError                 (0x00000001UL | RC1000_RtpmEvent)
#define RC102G_TimeoutOrPeerNonSpecific  (0x00000002UL | RC1000_RtpmEvent)
#define RC102H_ReceiverOverflow          (0x00000003UL | RC1000_RtpmEvent)
#define RC102I_ReasonNotSpecified        (0x00000004UL | RC1000_RtpmEvent)
#define RC102J_PermanentError            (0x00000005UL | RC1000_RtpmEvent)
#define RC102K_UserError                 (0x00000006UL | RC1000_RtpmEvent)
#define RC102L_ProtocolError             (0x00000007UL | RC1000_RtpmEvent)

/* ***************** RTS provider abort reason values *********************** */
/* ***************** (not provided in the standards). *********************** */

/*----------------------------------------------------------------------------*/
/* Structure of the provider abort reason value.                              */
/*----------------------------------------------------------------------------*/
/*
    ___________________________________________________
   |   Byte 3   |   Byte 2   |   Byte 1   |   Byte 0   |
   |____________|____________|____________|____________|

  Byte 0 = Source of abort
  Byte 1 = Protocol machine event number (see individual PM service interfaces)
  Byte 2 = Protocol reason value         (i.e. abort initiated by peer-PM)
  Byte 3 = Local reason value            (i.e. abort initiated by local-PM)

*/

    /*------------------------------------------------------------------------*/
    /* Values for Byte 0 (Source of abort).                                   */
    /*------------------------------------------------------------------------*/
#define RC102a_PeerPM         (0x00000000UL)  /* Aborted by the peer  PM.     */
#define RC102b_LocalPM        (0x00000001UL)  /* Aborted by the local PM.     */
#define RC102c_PeerUser       (0x00000002UL)  /* Aborted by the peer  user.   */

    /*------------------------------------------------------------------------*/
    /* Value of abort used when the association is permanently aborted by the */
    /* PEER RTPM (also valid in X.410 mode).                                  */
    /*------------------------------------------------------------------------*/
#define RC102g_RT_ProvABORT   (0x00010000UL| RC1000_RtpmEvent | RC102a_PeerPM)

    /*------------------------------------------------------------------------*/
    /* Value of abort used when the association is permanently aborted by the */
    /* LOCAL RTPM (also valid in X.410 mode).                                 */
    /* Note: The value RC102h_RTORQapduError is possible only during an       */
    /*       association recovery. (The other case i.e. during an association */
    /*       establishment is not possible because the logical association    */
    /*       does not exist for the responding RTPM).                         */
    /*------------------------------------------------------------------------*/
#define RC102h_RTORQapduError  (0x01000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102i_RTOACapduError  (0x02000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102j_RTORJapduError  (0x03000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102k_RTTPapduError   (0x04000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102l_RTTRapduError   (0x05000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102m_RTABapduError   (0x06000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102n_PROTsequError   (0x07000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102o_ProtocolError   (0x08000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102p_LocalUserError  (0x09000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102q_RecoveryTimeout (0x0a000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102r_TransferTimeout (0x0b000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102s_PeerReject      (0x0c000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102t_NoBufferProvided (0x0d000000UL|RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102u_MultipleBuffers (0x0e000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102v_CannotRecover   (0x0f000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102w_ReflectedParam  (0x10000000UL| RC1000_RtpmEvent | RC102b_LocalPM)
#define RC102x_RtsSynMatchErr  (0x11000000UL| RC1000_RtpmEvent | RC102b_LocalPM)

/* ********************** Event-Parameter-Matrix ***************************** *
 * **********************                        ***************************** *

 ------------------------------------------------------------------------------
|event            |IN/OUT|p1            |p2          |p3         |p4           |
|------------------------------------------------------------------------------|
|RT_OPENreq       |IN    |pdv  |noPdv   |NULL        |rT100a     |NULL         |
|RT_OPENind       |rtse  |pdv  |noPdv   |NULL        |rT100b     |NULL         |
|RT_OPENrsp       |IN    |pdv  |noPdv   |NULL        |rT100c     |openResult   |
|RT_OPENcnf       |rtse  |pdv  |noPdv   |NULL        |rT100d     |openResult   |
|------------------------------------------------------------------------------|
|RT_CLOSEreq      |IN    |pdv  |noPdv 1 |NULL        |NULL       |closeReason 2|
|RT_CLOSEind      |rtse  |pdv  |noPdv 1 |NULL        |NULL       |closeReason 2|
|RT_CLOSErsp      |IN    |pdv  |noPdv 1 |NULL        |NULL       |closeReason 2|
|RT_CLOSEcnf      |rtse  |pdv  |noPdv 1 |NULL        |NULL       |closeReason 2|
|------------------------------------------------------------------------------|
|RT_TRANSFERreq   |IN    |pdv  |rT100n  |whatUdata   |transfId   |transfTime   |
|RT_TRANSFERind   |rtse  |pdv  |rT100n  |whatUdata   |transfId   |NULL         |
|RT_TRANSFERcnf   |rtse  |pdv  |rT100n  |whatUdata   |transfId   |transfResult |
|                 |      |noPdv|norT100n|            |           |             |
|------------------------------------------------------------------------------|
|RT_TURN_PLEASEreq|IN   3|NULL          |signPriority|NULL       |valPriority  |
|RT_TURN_PLEASEind|rtse 3|NULL          |signPriority|NULL       |valPriority  |
|------------------------------------------------------------------------------|
|RT_TURN_GIVEreq  |IN   4|NULL          |NULL        |NULL       |NULL         |
|RT_TURN_GIVEind  |rtse 4|NULL          |NULL        |NULL       |NULL         |
|------------------------------------------------------------------------------|
|RT_P_ABORTind    |rtse  |NULL          |addAbortInfo|NULL       |abortReason  |
|------------------------------------------------------------------------------|
|RT_U_ABORTreq    |IN   5|pdv  |noPdv   |NULL        |NULL       |NULL         |
|RT_U_ABORTind    |rtse 5|pdv  |noPdv   |NULL        |NULL       |NULL         |
|------------------------------------------------------------------------------|
|RT_BUFFER        |IN   6|NULL          |NULL        |rT100n     |transfId     |
|------------------------------------------------------------------------------|
|All other        |IN   7|NOTDEFINED    |NOTDEFINED  |NOTDEFINED |NOTDEFINED   |
|values           |OUT   |NOTDONE       |NOTDONE     |NOTDONE    |NOTDONE      |
 ------------------------------------------------------------------------------

Note: 
1)    p1 is always noPdv in X.410 mode for RT_CLOSEreq and RT_CLOSErsp
2)    closeReason is not defined in X.410 mode.
3)    RT_TURN_PLEASEreq is possible only when the requestor of the service is
      the RECEIVER and only when RC1011_TwoWayAlternate has been chosen
      for ulDialogueMode.
4)    RT_TURN_GIVEreq is possible only when the requestor of the service is
      the SENDER and only when RC1011_TwoWayAlternate has been chosen
      for ulDialogueMode.
5)    RT_U_ABORTreq/ind is possible only in NORMAL mode.
6)    RT_BUFFER is possible only if the requestor of service is the RECEIVER.
      It is not possible when the requestor has already issued a RT_BUFFER and
      has not yet received a RT_TRANSFERind.

Contradiction of conditions 1) to 6) or occurence of condition 7) would lead
to a state of a PERMANENT ERROR in the RTS system.



IN           : It is an incoming event (received by the RTPM).
OUT          : It is an outgoing event (sent by the RTPM).
rtse         : This event is posted to the user of the RTSE service events
NOTDEFINED   : This parameter contains a not defined value.
NOTDONE      : Such an event is not issued by the RTPM.
NULL         : Parameter not used.
pdv          : Global heap memory handle of PDV-tree root node.
               Either of type rTro04_stUserData or of type rTro03_stPdvHeader.
               Transparent to RTS.  Header is always of type rTro03_stPdvHeader
                                                                 (see rouply0.h)
noPdv        : No PDV description of PM user data for this event (see rouply0.h)
rT100a       : Local heap memory handle to RT-OPEN-request
               service primitive parameters                      (see above)
rT100b       : Local heap memory handle to RT-OPEN-indication
               service primitive parameters                      (see above)
rT100c       : Local heap memory handle to RT-OPEN-response
               service primitive parameters                      (see above)
rT100d       : Local heap memory handle to RT-OPEN-confirm
               service primitive parameters                      (see above)
openResult   : Open result
closeReason  : Release result reason                             (see rouply0.h)
rT100n       : Local heap memory handle of TPDU description      (see above)
norT100n     : No TPDU description of PM user data for this event(see above)
whatUdata    : RC1021_UdataAbsent => No user data in p1 (neither TPDU nor PDV).
               RC1022_UdataIsPdv  => p1 contains PDV  description (Used by ROS)
               RC1023_UdataIsTpdu => p1 contains TPDU description (Used by exif)
transfId     : Transfer Identifier associated with the transfer.
               Transparent to RTPM.
transfTime   : Time within which either the transfer is completed or the user is
               indicated of a RT-TRANSFERcnf with result indicating a transfer 
               failure.                                          (see above)
transfResult : Result of transfer.                               (see above)
signPriority : Sign of the value present in valPriority in p4. Values possible :
               - NULL (or FALSE) => +ve;
               - Any other value => -ve.
valPriority  : Absolute value of the priority parameter.
               Must not be greater than MAXLONG_POS              (see rogenrl.h)
addAbortInfo : Additional abort information. If abort generated by RTPM and
               abortReason = "Invalid Parameter", this field
               contains the reflected parameter description. The reflected 
               parameter is a bit string that identifies which parameters are
               regarded as invalid parameters in the service primitive that
               caused an abort. The order of the bits in the bit string is the
               same as the order of the parameters in the tables of service
               parameters in ACSE and Presentation ISO standards (i.e. bit 1
               represents the first parameter).
abortReason  : Provider abort reason             (see rouply0.h. Also see above)


Note that the generic parameters were used in ROS v2.x according to a certain
convention which has been slightly modified as described below :
 p1:  Traditional & Suggested use: PM user data.
 p2:  Traditionally: Used for conveying the receive buffer description.
      Suggested: Use as non-memory-handle parameter.
 p3:  Traditionally: Used for conveying local memory handle.
      Suggested: Use as local memory handle for constructed types where possible
 p4:  Traditionally: Used for passing values.
      Suggested: Retain traditional use.

* ********************** End of Event-Parameter-Matrix  ********************* *
* **********************                                ********************* */

#endif	/* _RORTPM0_H */
