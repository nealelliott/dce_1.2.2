/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rortpm1.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:08  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:05  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:56  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:38  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:33  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:21  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:38  keutel
 * 	creation
 * 	[1994/03/21  13:27:55  keutel]
 * 
 * $EndLog$
 */
#ifndef _RORTPM1_H
#define _RORTPM1_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rortpm1_rcsid[] = { "@(#) $RCSfile: rortpm1.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:08 $" } ;
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
/* NAME         : rortpm1.h                                           */
/*                                                                    */
/* AUTHOR       : Shivkumar, Haran                                    */
/* DATE         : 92-02-11                                            */
/*                                                                    */
/* COMPONENT    : Reliable transfer service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Protocol machine apdu definition                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rortpm1.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 2.00   K0 |92-02-11|  original                      |skh |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* *****************************            ********************************* */
/* ***************************** RTORQ apdu ********************************* */
/* *****************************            ********************************* */

/* ************** Refer rortpm0.h for the value definitions of : ************ */
/*                - checkpointSize                                            */
/*                - windowSize                                                */
/*                - dialogueMode                                              */
/* ************************************************************************** */

/* ********** RCro5m_MaxRef, RCro5n_MaxAddRef: (see rouply0.h) ************** */

/* ********** lCallingSSuserRef: Member identifiers of CHOICE *************** */
#define RC1040_T61String         0x00000000L     /* Solely in X.410-1984 mode */
#define RC1041_OctetString       0x00000001L     /* Solely in normal mode     */

typedef struct rS104b_stSessConnId {
    signed   long  int        lCallingSSuserRef;    /* CHOICE MemberId        */
    unsigned long  int        ulCallingSSuserRef;   /* Length of OCTET/T61 str*/
    char                      vCallingSSuserRef [RCro5m_MaxRef];
    unsigned long  int        ulCommonReference;    /* Length of OCTET STRING */
    char                      vCommonReference [RCro5m_MaxRef];
    signed   long  int        bAdditionalReference;
    unsigned long  int        ulAdditionalReference;/* Length of T61String    */
    char                      vAdditionalReference [RCro5n_MaxAddRef];
} rT104b_stSessConnId;

typedef union rU104a_unConnData {
    unsigned long  int        hOpen;    /* Handle to rTro04_stUserData struct */
                                        /* passed in p1. User PDV description */
    rT104b_stSessConnId       stRecover;
} rT104a_unConnData;

/* ************ lConnectionData: Member identifiers of CHOICE *************** */
#define RC1045_Null              0x00000000L /* No RTSE user data present.    */
#define RC1046_Open              0x00000001L /* RTSE user data only for       */
                                             /* normal open.                  */
#define RC1047_Recover           0x00000002L /* SessionConnectionIdentifier   */
                                             /* only during assoc. recovery.  */

typedef struct rS1041_stRtorqApdu {
    rTro03_stPdvHeader        stPdvHeader;
    signed   long  int        lCheckpointSize;
    signed   long  int        lWindowSize;
    signed   long  int        lDialogueMode;
    signed   long  int        lConnectionData;      /* CHOICE MemberId        */
    rT104a_unConnData         unConnectionData;
    signed   long  int        bApplicationProtocol; /* INTEGER OPTIONAL       */
    signed   long  int        lApplicationProtocol; /* see rouply0.h          */
} rT1041_stRtorqApdu;

/* ***************************** RTOAC apdu ********************************* */
/* *****************************            ********************************* */

typedef struct rS1042_stRtoacApdu {
    rTro03_stPdvHeader        stPdvHeader;
    signed   long  int        lCheckpointSize;
    signed   long  int        lWindowSize;
    signed   long  int        lConnectionDataAC;  /* CHOICE MemberId          */
    rT104a_unConnData         unConnectionDataAC;
} rT1042_stRtoacApdu;


/* ***************************** RTORJ apdu ********************************* */
/* *****************************            ********************************* */

/*----------------------------------------------------------------------------*/
/* Refuse reason values :                                                     */
/* RtsBusy can be used during normal association establishment and recovery.  */
/* CannotRecover can be used ONLY during recovery.                            */
/* ValidationFailure and UnaccDialogueMode can be used ONLY during normal     */
/* association establishment.                                                 */
/*----------------------------------------------------------------------------*/

#define RC1050_RtsBusy            (0x00000000L) /* RTS too busy to accept ass.*/
#define RC1051_CannotRecover      (0x00000001L) /* Cannot recover association */
#define RC1052_ValidationFailure  (0x00000002L) /* Validation failure         */
#define RC1053_UnaccDialogueMode  (0x00000003L) /* Unacceptable Dialogue Mode */

typedef struct rS1043_stRtorjApdu {
    rTro03_stPdvHeader        stPdvHeader;
    signed   long  int        bRefuseReason; /* INTEGER OPTIONAL              */
    signed   long  int        lRefuseReason;
    signed   long  int        bUserDataRJ;   /* ANY OPTIONAL                  */
    unsigned long  int        hUserDataRJ;   /* Handle to rTro04_stUserData   */
                                             /* passed in p1 (User PDV descr) */
} rT1043_stRtorjApdu;


/* ***************************** RTTP  apdu ********************************* */
/* *****************************            ********************************* */

typedef struct rS1044_stRttpApdu {
    rTro03_stPdvHeader        stPdvHeader;
    signed long  int          lPriority;
} rT1044_stRttpApdu;


/* ***************************** RTTR  apdu ********************************* */
/* *****************************            ********************************* */

typedef struct rS1045_stRttrApdu {
    rTro03_stPdvHeader        stPdvHeader;
    rTro02_stStore            stRttrApdu;  /* Store description of user TPDU  */
} rT1045_stRttrApdu;


/* ***************************** RTAB  apdu ********************************* */
/* *****************************            ********************************* */

/*----------------------------------------------------------------------------*/
/* 'lAbortReason' values.                                                     */
/*----------------------------------------------------------------------------*/
#define RC104a_LocalSysProblem   (0x00000000L)
#define RC104b_InvalidParameter  (0x00000001L) /* Invalid parameters specified*/
                                               /* in reflected parameter field*/
#define RC104c_UnrecogActivity   (0x00000002L) /* Sending RTPM did a trf_abort*/
                                               /* followed by provider abort  */
#define RC104d_TemporaryProblem  (0x00000003L) /* Used when the RTSE cannot   */
                                               /*accept a session for a period*/
                                               /* of time. No attempt at assoc*/
                                               /* recovery should be performed*/
                                               /* for a period of time        */
                                               /* determined by a local rule  */
#define RC104e_ProtocolError     (0x00000004L) /* RTSE level protocol error   */
#define RC104f_PermanentError    (0x00000005L) /* Also used in X.400 mode     */
#define RC104g_UserError         (0x00000006L)
#define RC104h_TransferCompleted (0x00000007L) /* Receiving RTPM cant discard */
                                               /*an already completed transfer*/
                                               /* Also used in X.400 mode.    */

typedef struct rS1046_stRtabApdu {
    rTro03_stPdvHeader        stPdvHeader;
    signed   long  int        bAbortReason;        /* INTEGER OPTIONAL        */
    signed   long  int        lAbortReason;
    signed   long  int        bReflectedParameter; /* BIT STRING OPTIONAL     */
    unsigned long  int        ulReflectedParamLen; /* Length in bits          */
    unsigned long  int        ulReflectedParameter;/* Bit pattern             */
    signed   long  int        bUserDataAB;         /* ANY OPTIONAL            */
    unsigned long  int        hUserDataAB;         /* Handle to struct        */
                                                   /* rTro04_stUserData:      */
                                                   /* Passed in p1; PDV descr.*/
} rT1046_stRtabApdu;

/* ***************************** RTPM apdus ********************************* */
/* *****************************            ********************************* */

#define RC104u_RTORQapdu      (0x00000000L)        /* RT-OPEN request APDU    */
#define RC104v_RTOACapdu      (0x00000001L)        /* RT-OPEN accept  APDU    */
#define RC104w_RTORJapdu      (0x00000002L)        /* RT-OPEN reject  APDU    */
#define RC104x_RTTPapdu       (0x00000003L)        /* RT-TURN-PLEASE  APDU    */
#define RC104y_RTTRapdu       (0x00000004L)        /* RT-TRANSFER     APDU    */
#define RC104z_RTABapdu       (0x00000005L)        /* RT-ABORT        APDU    */

typedef union rU1040_unRtseApdus {
    rTro03_stPdvHeader        stPdvHeader;
    rT1041_stRtorqApdu        stRtorq;
    rT1042_stRtoacApdu        stRtoac;
    rT1043_stRtorjApdu        stRtorj;
    rT1044_stRttpApdu         stRttp;
    rT1045_stRttrApdu         stRttr;
    rT1046_stRtabApdu         stRtab;
} rT1040_unRtseApdus;

#endif	/* _RORTPM1_H */
