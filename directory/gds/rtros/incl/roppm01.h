/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm01.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:50  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:45  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:34  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:45  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:21  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:22  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:42  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:33  keutel
 * 	creation
 * 	[1994/03/21  13:26:59  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPM01_H
#define _ROPPM01_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppm01_rcsid[] = { "@(#) $RCSfile: roppm01.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:50 $" } ;
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
/* NAME         : roppm01.h                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-02-12                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : PPM protocol interface                              */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppm01.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-02-12|  original                      |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* ****************** Local syntax of               ***************** */
/* ****************** explicit encoding information ***************** */

/* ------------------ PDV-list -------------------------------------- */

/* This structure is held as local C variable (within the stack) in   */
/* E/D Control functions.                                             */
/*                                                                    */
/* see rouply0.h for the values of:                                   */
/*  - 'lEncodingChoice'                                               */


typedef struct rS0410_stPdvList {
   rTro00_vObjectId        oTransSynName;             /* OPTIONAL: 1st*/
                                                      /* ObjId elem.=0*/
   signed   long  int      lPresContId;
   signed   long  int      lEncodingChoice;
} rT0410_stPdvList;


/* ------------------ EXTERNAL -------------------------------------- */

/* This structure is held as local C variable (within the stack) in   */
/* E/D Control functions.                                             */
/*                                                                    */
/* see rouply0.h for the values of:                                   */
/*  - 'lEncodingChoice'                                               */


typedef struct rS0411_stExternal {
   rTro00_vObjectId        oDirectRef;                /* OPTIONAL: 1st*/
                                                      /* ObjId elem.=0*/

   signed   long  int      bIndirectRef;              /* OPTIONAL     */
   signed   long  int      lIndirectRef;

   signed   long  int      bObjectDescrip;            /* OPTIONAL     */
   unsigned long  int      ulObjectDescrip;           /* length       */
   char                    vObjectDescrip
                            [RCro0c_MaxLenObjDescExt];

   signed   long  int      lEncodingChoice;
} rT0411_stExternal;


/* ********** Local syntax of PPCI (sub-definitions) **************** */
/* **********                                        **************** */

/* ---------- Presentation-context-definition-list element ---------- */

typedef struct rS0412_stPresContDef {
   signed   long  int      lPresContId;
   rTro00_vObjectId        oAbstrSynName;
   unsigned long  int      ulTransSynNumber;
   rTro00_vObjectId        vTransSynName
                            [RCro03_MaxTransPerAbstr];
} rT0412_stPresContDef;


/* ---------- Presentation-context-definition-result-list element --- */

/* 'lResult' values */
#define RC0410_Acceptance             0L
#define RC0411_UserRejection          1L
#define RC0412_ProviderRejection      2L

/* 'lProviderReason' values */
/* Note the relation between the corresponding protocol and service   */
/* definitions (see rouply0.h):                                       */
/*      ProtocolValue = - (ServiceValue  + 1)                         */  
/*      ServiceValue  = - (ProtocolValue + 1)                         */  
#define RC0413_ReasonNotSpecified     0L
#define RC0414_AbstrSynNotSupported   1L
#define RC0415_TransSynNotSupported   2L
#define RC0416_LocalDcsLimitExceeded  3L


typedef struct rS0413_stPresContDefRes {
   signed   long  int      lResult;
   rTro00_vObjectId        oTransSynName;             /* OPTIONAL: 1st*/
                                                      /* ObjId elem.=0*/
   signed   long  int      bProviderReason;           /* OPTIONAL     */
   signed   long  int      lProviderReason;
} rT0413_stPresContDefRes;


/* ---------- Presentation-context-identifier-list element ---------- */

typedef struct rS0414_stPresContId {
   signed   long  int      lPresContId;
   rTro00_vObjectId        oTransSynName;
} rT0414_stPresContId;


/* ----------- User-data (fully encoded) ---------------------------- */

typedef struct rS0415_stFully {
   unsigned long  int      ulPdvList;                 /* number       */
   unsigned long  int      vPdvList
                            [RCro0a_MaxPdvList];
} rT0415_stFully;

/*
   - ulPdvList      : Number of PDV-lists (SEQUENCE OF PDV-list).
   - vPdvList       : Memory handles to PDV nodes (each node describes
                      one PDV-list)        (max. number see rouply0.h).
*/


/* ------------ User-data union ------------------------------------- */

typedef union rU0416_unUserData {
   unsigned long  int      hSimply;
   rT0415_stFully          stFully;
} rT0416_unUserData;

/*
   - hSimply        : If simply encoded: Memory handle to PDV node.
   - stFully        : If fully  encoded: See type definition above.
*/


/* ------------ User-data struct ------------------------------------ */

/* 'lSimplyFullyChoice' values */
#define RC0417_SimplyEncoded          0L
#define RC0418_FullyEncoded           1L


typedef struct rS0417_stUserData {
   unsigned long  int      lSimplyFullyChoice;
   rT0416_unUserData       unUserData;
} rT0417_stUserData;

/*
   - lSimplyFullyChoice: CHOICE member id.
   - unUserData     : Simply or fully  encoded: See type definition above.
*/


/* ****************** Local syntax of PPCI ************************** */
/* ******************                      ************************** */

/* ------------------ CP PPDU --------------------------------------- */

/* see rouply0.h for the values of:                                   */
/*  - 'lModeSelector'                                                 */
/* no values necessary in the current implementation version:         */
/*  - 'bsPresRequirements' (only kernel func. unit used)              */
/*  - 'bsSessRequirements'                                            */

/* 'bsProtVersion' value */
#define RC0419_ProtVersion1     0x80000000            /* bit (0)      */


typedef struct rS041A_stCp {
   rTro03_stPdvHeader      stPdvHeader;
   signed   long  int      lModeSelector;

                                          /* only used in X410-mode   */
   unsigned long  int      hX410RtpmNode;             /* OPTIONAL: 0  */


                                          /* only used in normal-mode */
   signed   long  int      bNormalSEQUENCE;           /* OPTIONAL     */

   unsigned long  int      ulProtVersion;             /* length (bits)*/
   unsigned long  int      bsProtVersion;

   signed   long  int      bCallingSelector;          /* OPTIONAL     */
   unsigned long  int      ulCallingSelector;         /* length       */
   char                    vCallingSelector
                            [RCro1a_MaxLenPSelector];

   signed   long  int      bCalledSelector;           /* OPTIONAL     */
   unsigned long  int      ulCalledSelector;          /* length       */
   char                    vCalledSelector
                            [RCro1a_MaxLenPSelector];

   signed   long  int      bPresContDef;              /* OPTIONAL     */
   unsigned long  int      ulPresContDef;             /* number       */
   rT0412_stPresContDef    vPresContDef
                            [RCro04_MaxPresCont];

   signed   long  int      bDefaultContext;           /* OPTIONAL     */
   rTro00_vObjectId        oDefaultAbstrSyn;
   rTro00_vObjectId        oDefaultTransSyn;

   signed   long  int      bPresRequirements;         /* OPTIONAL     */
   unsigned long  int      ulPresRequirements;        /* length (bits)*/
   unsigned long  int      bsPresRequirements;

   signed   long  int      bSessRequirements;         /* OPTIONAL     */
   unsigned long  int      ulSessRequirements;        /* length (bits)*/
   unsigned long  int      bsSessRequirements;

   signed   long  int      bUserData;                 /* OPTIONAL     */
   rT0417_stUserData       stUserData;
} rT041A_stCp;


/* ------------------ CPA PPDU -------------------------------------- */

/* see under "CP PPDU" for the value of:                              */
/*  - 'bsProtVersion'                                                 */
/* see rouply0.h for the values of:                                   */
/*  - 'lModeSelector'                                                 */
/* no values necessary in the current implementation version:         */
/*  - 'bsPresRequirements' (only kernel func. unit used)              */
/*  - 'bsSessRequirements'                                            */


typedef struct rS041B_stCpa {
   rTro03_stPdvHeader      stPdvHeader;
   signed   long  int      lModeSelector;

                                          /* only used in X410-mode   */
   unsigned long  int      hX410RtpmNode;             /* OPTIONAL: 0  */


                                          /* only used in normal-mode */
   signed   long  int      bNormalSEQUENCE;           /* OPTIONAL     */

   unsigned long  int      ulProtVersion;             /* length (bits)*/
   unsigned long  int      bsProtVersion;

   signed   long  int      bRespondingSelector;       /* OPTIONAL     */
   unsigned long  int      ulRespondingSelector;      /* length       */
   char                    vRespondingSelector
                            [RCro1a_MaxLenPSelector];

   signed   long  int      bPresContDefRes;           /* OPTIONAL     */
   unsigned long  int      ulPresContDefRes;          /* number       */
   rT0413_stPresContDefRes vPresContDefRes
                            [RCro04_MaxPresCont];

   signed   long  int      bPresRequirements;         /* OPTIONAL     */
   unsigned long  int      ulPresRequirements;        /* length (bits)*/
   unsigned long  int      bsPresRequirements;

   signed   long  int      bSessRequirements;         /* OPTIONAL     */
   unsigned long  int      ulSessRequirements;        /* length (bits)*/
   unsigned long  int      bsSessRequirements;

   signed   long  int      bUserData;                 /* OPTIONAL     */
   rT0417_stUserData       stUserData;
} rT041B_stCpa;

 
/* ------------------ CPR PPDU -------------------------------------- */

/* see under "CP PPDU" for the value of:                              */
/*  - 'bsProtVersion'                                                 */

/* 'lX410orNormalChoice' values */
#define RC041a_X410Mode                0L
#define RC041b_NormalMode              1L

/* 'lProviderReason' values */
#define RC041c_ReasonNotSpecified      0L
#define RC041d_TemporaryCongestion     1L
#define RC041e_LocalLimitExceeded      2L
#define RC041f_CalledPaddressUnknown   3L
#define RC041g_ProtVersionNotSuppo     4L
#define RC041h_DefaultContNotSuppo     5L
#define RC041i_UserDataNotReadable     6L
#define RC041j_NoPSAPavailable         7L


typedef struct rS041C_stCpr {
   rTro03_stPdvHeader      stPdvHeader;
   signed   long  int      lX410orNormalChoice;       /* CHOICE member*/

                                          /* only used in X410-mode   */
   unsigned long  int      hX410RtpmNode;             /* next PDV node*/


                                          /* only used in normal-mode */
   unsigned long  int      ulProtVersion;             /* length (bits)*/
   unsigned long  int      bsProtVersion;

   signed   long  int      bRespondingSelector;       /* OPTIONAL     */
   unsigned long  int      ulRespondingSelector;      /* length       */
   char                    vRespondingSelector
                            [RCro1a_MaxLenPSelector];

   signed   long  int      bPresContDefRes;           /* OPTIONAL     */
   unsigned long  int      ulPresContDefRes;          /* number       */
   rT0413_stPresContDefRes vPresContDefRes
                            [RCro04_MaxPresCont];

   signed   long  int      bDefaultContextResult;     /* OPTIONAL     */
   signed   long  int      lDefaultContextResult;

   signed   long  int      bProviderReason;           /* OPTIONAL     */
   signed   long  int      lProviderReason;

   signed   long  int      bUserData;                 /* OPTIONAL     */
   rT0417_stUserData       stUserData;
} rT041C_stCpr;


/* ------------------ ARU PPDU -------------------------------------- */

/* see under "CPR PPDU" for the values of:                            */
/*  - 'lX410orNormalChoice'                                           */


typedef struct rS041D_stAru {
   rTro03_stPdvHeader      stPdvHeader;
   signed   long  int      lX410orNormalChoice;       /* CHOICE member*/

                                          /* only used in X410-mode   */
   unsigned long  int      hX410RtpmNode;             /* next PDV node*/


                                          /* only used in normal-mode */
   signed   long  int      bPresContId;               /* OPTIONAL     */
   unsigned long  int      ulPresContId;              /* number       */
   rT0414_stPresContId     vPresContId
                            [RCro04_MaxPresCont];

   signed   long  int      bUserData;                 /* OPTIONAL     */
   rT0417_stUserData       stUserData;
} rT041D_stAru;


/* ------------------ ARP PPDU -------------------------------------- */

/* no values necessary in the current implementation version:         */
/*  - 'lEventId' (set via mapping table)                              */

/* 'lAbortReason' values */
/* Note: Three values are defined in rouply0.h because they are used  */
/*       by decoding functions of APMs too.                           */
#define RC041k_ReasonNotSpecified      0L
#define RC041l_UnrecognizedPpdu        1L
#define RC041m_UnexpectedPpdu          2L
#define RC041n_UnexpectedSessEvent     3L


typedef struct rS041E_stArp {
   rTro03_stPdvHeader      stPdvHeader;

   signed   long  int      bAbortReason;              /* OPTIONAL     */
   signed   long  int      lAbortReason;

   signed   long  int      bEventId;                  /* OPTIONAL     */
   signed   long  int      lEventId;
} rT041E_stArp;


/* ------------------ Other PPDU types ------------------------------ */

typedef struct rS041F_stOther {
   rTro03_stPdvHeader      stPdvHeader;

   rT0417_stUserData       stUserData;
} rT041F_stOther;

 
/* ------------------------------------------------------------------ */
/* ------------------ Union of all PPDU types ----------------------- */

/* PDV header: 'lType' values */
/* The PDV header struct member 'lType' (see rouply0.h) is only used by the */
/* clearPdvNode function. Otherwise the S-event describes the PDU type.     */
/* Exception: S-UABORT request/indication ('lType' contains the Abort-type  */
/* CHOICE value).                                                           */
#define RC041A_Cp                      0L
#define RC041B_Cpa                     1L
#define RC041C_Cpr                     2L
#define RC041D_Aru                     3L
#define RC041E_Arp                     4L
#define RC041F_Other                   5L


typedef union rU041G_unPpdu {
   rTro03_stPdvHeader      stPdvHeader;
   rT041A_stCp             stCp;
   rT041B_stCpa            stCpa;
   rT041C_stCpr            stCpr;
   rT041D_stAru            stAru;
   rT041E_stArp            stArp;
   rT041F_stOther          stOther;
} rT041G_unPpdu;

#endif	/* _ROPPM01_H */
