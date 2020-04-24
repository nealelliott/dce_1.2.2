/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpm1.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:33  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:22  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:09  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:22  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:20  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:44  keutel
 * 	creation
 * 	[1994/03/21  13:18:37  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROACPM1_H
#define _ROACPM1_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roacpm1_rcsid[] = { "@(#) $RCSfile: roacpm1.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:34 $" } ;
#endif

/* *********************************************************************** */
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
/* *********************************************************************** */

/*daton ********************************************************************/
/*                                                                         */
/* TYPE         : INCLUDE                                                  */
/*                                                                         */
/* NAME         : roacpm1.h                                                */
/*                                                                         */
/* AUTHOR       : Johann Mueller, Michael Rippstain                        */
/* DATE         : 92-01-08                                                 */
/*                                                                         */
/* COMPONENT    : RTS Association Control Service Element                  */
/*                Protocol Machine                                         */
/*                                                                         */
/* PRD#/VERS.   : RTS-V2.00                                                */
/*                                                                         */
/* DESCRIPTION  : Protocol Machine Apdu Definition                         */
/*                                                                         */
/* SYSTEM DEPENDENCE :  none                                               */
/*                                                                         */
/* HISTORY      : sccsid  =  @(#)roacpm1.h	1.1 94/01/26                                       */
/*                                                                         */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM#      */
/* 2.00 K0  | 92-01-08|  original                      |mr  |              */
/*          |         |                                |    |              */
/*datoff *******************************************************************/



/* ------------------ Attribute Value Assertion -------------------------- */

typedef struct rS0630_stAttriValueAssertion {    /* SEQUENCE               */
   rTro00_vObjectId         vAttributeType;      /* OBJECT IDENTIFIER      */
   unsigned long int        lAttributeType;      /* T61String              */
   char                     vAttributeValue
			     [RCro7h_MaxAttriValue];
					       /* This ASN.1 type is defined
						 as ANY; here there is, like
						 Directory, an T61String
						 used                      */
} rT0630_stAttriValueAssertion;

/* ------------------ Relative Distinguished Name ------------------------ */

typedef struct rS0631_stRDN {                    /* SET OF                 */
   unsigned long int             lRDN;
   rT0630_stAttriValueAssertion stRDN [RCro7g_MaxRDN];
} rT0631_stRDN;

/* ------------------ RDNSequence ---------------------------------------- */

typedef struct rS0632_stRDNSequence {            /* SEQUENCE OF            */
   unsigned long int        lRDNSequence;
   rT0631_stRDN            stRDNSequence [RCro7f_MaxRDNSequence ];
} rT0632_stRDNSequence;

/* ------------------ Name ----------------------------------------------- */

typedef union rU0633_uName {
   rT0632_stRDNSequence    stRDNSequence;        /* only one possibility   */
						 /*  for now               */
} rT0633_uName;

typedef struct rS0634_stName {                   /* CHOICE                 */
   signed long int          lName;
   rT0633_uName             uName;
} rT0634_stName;

/* ------------------ AP-Title ------------------------------------------- */

typedef union rU0635_uApTitle {
   rT0634_stName           stApTitleForm1;       /* NAME                   */
   rTro00_vObjectId         vApTitleForm2;       /* OBJECT IDENTIFIER      */
} rT0635_uApTitle;

/* ------------------ AE-Qualifier --------------------------------------- */

typedef union  rU0637_uAeQualifier {
   rT0631_stRDN            stAeQualifierForm1;   /* Relative Disting. Name */
   signed long int          lAeQualifierForm2;   /* INTEGER                */
} rT0637_uAeQualifier;


/* ****************** AARQ-Apdu ****************************************** */
/* ******************           ****************************************** */

#define RC0639_MaxImplInfo            16
#define RC0640_ProtocolVers1          0x80000000 /* is imlemented as BIT   */
						 /* STRING with DEFAULT    */
						 /* version1               */

typedef struct rS0641_aarqApdu {
   rTro03_stPdvHeader      stPdvHeader;          /* apdu  */

   unsigned long int       ulProtoVersionLength; /* BIT STRING             */
   unsigned long int       ulProtoVersion;       /* protoVersion           */

   rTro00_vObjectId         vApliCxtName;        /* application context name */
						 /* appliContext           */
   signed long int          bCalledApTitle;      /* OPTIONAL               */
   signed long int          lCalledApTitle;      /* CHOICE (see rouply0.h) */
   rT0635_uApTitle          uCalledApTitle;

   signed long int          bCalledAeQualifier;  /* OPTIONAL               */
   signed long int          lCalledAeQualifier;  /* CHOICE (see rouply0.h) */
   rT0637_uAeQualifier      uCalledAeQualifier;

   signed long int          bCalledApInvocId;    /* OPTIONAL               */
   signed long int          lCalledApInvocId;
   signed long int          bCalledAeInvocId;    /* OPTIONAL               */
   signed long int          lCalledAeInvocId;

   signed long int          bCallingApTitle;     /* OPTIONAL               */
   signed long int          lCallingApTitle;     /* CHOICE (see rouply0.h) */
   rT0635_uApTitle          uCallingApTitle;

   signed long int          bCallingAeQualifier; /* OPTIONAL               */
   signed long int          lCallingAeQualifier; /* CHOICE (see rouply0.h) */
   rT0637_uAeQualifier      uCallingAeQualifier;

   signed long int          bCallingApInvocId;   /* OPTIONAL               */
   signed long int          lCallingApInvocId;
   signed long int          bCallingAeInvocId;   /* OPTIONAL               */
   signed long int          lCallingAeInvocId;

   signed long int          bImplInfo;           /* OPTIONAL               */
   unsigned long int       ulImplInfo;           /* GraphicString          */
   char                     vImplInfo            /*  implementation-data   */
			     [RC0639_MaxImplInfo];

   signed   long int        bUserInfo;           /* OPTIONAL               */
   unsigned long int       ulUserInfo;           /* SEQUENCE OF EXTERNAL   */
   unsigned long int        vUserInfo            /* Association-information  */
			     [RCro0b_MaxExternal];
						 /* for [MAX](see rouply0.h)      */
} rT0641_aarqApdu;


/* ****************** AARS-Apdu ****************************************** */
/* ******************           ****************************************** */

					/* Result parameter                */
#define RC0641_AA_Accept               0
#define RC0642_AA_PermanentReject      1
#define RC0643_AA_TransientReject      2

					/* lResultSourceDiag parameter     */
#define RC0644_ResultSourceUser        1
#define RC0645_ResultSourceProvid      2

/* ------------------ Associate Source Diagnostic ------------------------ */

/* see roacpm0.h under item "Result in AA_ASSOCIATE rsp/cnf" for the       */
/*  'Named-Values': lAcseServiceUser and lAcseServiceProvider              */

typedef union rU063b_uAssociSourceDiag {
   signed long int          lAcseServiceUser;     /* (see roacpm0.h)       */
   signed long int          lAcseServiceProvider; /* (see roacpm0.h)       */
} rT063b_uAssociSourceDiag;


typedef struct rS0642_aareApdu {
   rTro03_stPdvHeader      stPdvHeader;

   unsigned long int       ulProtoVersionLength; /* BIT STRING             */
   unsigned long int       ulProtoVersion;       /* protoVersion           */

   rTro00_vObjectId         vApliCxtName;        /* application context name */

   signed   long int        lResult;             /*  associResult */

   unsigned long int        lResultSourceDiag;   /* CHOICE                 */
   rT063b_uAssociSourceDiag uResultSourceDiag;

   signed long int          bRespondApTitle;     /* OPTIONAL               */
   signed long int          lRespondApTitle;     /* CHOICE (see rouply0.h) */
   rT0635_uApTitle          uRespondApTitle;

   signed long int          bRespondAeQualifier; /* OPTIONAL               */
   signed long int          lRespondAeQualifier; /* CHOICE (see rouply0.h) */
   rT0637_uAeQualifier      uRespondAeQualifier;

   signed long int          bRespondApInvocId;   /* OPTIONAL               */
   signed long int          lRespondApInvocId;

   signed long int          bRespondAeInvocId;   /* OPTIONAL               */
   signed long int          lRespondAeInvocId;

   signed long int          bImplInfo;           /* OPTIONAL               */
   unsigned long int       ulImplInfo;           /* GraphicString          */
   char                     vImplInfo            /*  implementation-data   */
			     [RC0639_MaxImplInfo];

   signed   long int        bUserInfo;           /* OPTIONAL               */
   unsigned long int       ulUserInfo;           /* SEQUENCE OF EXTERNAL   */
   unsigned long int        vUserInfo            /* Association-information  */
			     [RCro0b_MaxExternal];
						 /* for [MAX](see rouply0.h)      */
} rT0642_aareApdu;

/* ****************** RLRQ-Apdu ****************************************** */
/* ******************           ****************************************** */

/* ------------------ Release Request Reason ----------------------------- */

#define RC0652_RLRQreasonNormal        0
#define RC0653_RLRQreasonUrgent        1
#define RC0654_RLRQreasonUsrDef       30

typedef struct rS0643_rlrqApdu {
   rTro03_stPdvHeader      stPdvHeader;

   signed long int          bReason;             /* INTEGER OPTIONAL       */
   signed long int          lReason;             /* Release Request Reason */
						  /* rlrqReason */

   signed   long int        bUserInfo;           /* OPTIONAL               */
   unsigned long int       ulUserInfo;           /* SEQUENCE OF EXTERNAL   */
   unsigned long int        vUserInfo            /* Association-information  */
			     [RCro0b_MaxExternal];
						 /* for [MAX](see rouply0.h)      */
} rT0643_rlrqApdu;

/* ****************** RERE-Apdu ****************************************** */
/* ******************           ****************************************** */

/* ------------------ Release Response Reason ---------------------------- */

#define RC0657_RLREreasonNormal        0
#define RC0658_RLREreasonNotFin        1
#define RC0659_RLREreasonUsrDef       30

typedef struct rS0644_rlreApdu {
   rTro03_stPdvHeader      stPdvHeader;

   signed long int          bReason;             /* INTEGER OPTIONAL       */
   signed long int          lReason;             /* Release Response Reason  */
						 /* rlreReason */
   signed   long int        bUserInfo;           /* OPTIONAL               */
   unsigned long int       ulUserInfo;           /* SEQUENCE OF EXTERNAL   */
   unsigned long int        vUserInfo            /* Association-information  */
			     [RCro0b_MaxExternal];
						 /* for [MAX](see rouply0.h)      */
} rT0644_rlreApdu;

/* ****************** ABRT-Apdu ****************************************** */
/* ******************           ****************************************** */

/* ------------------ ABORT Source --------------------------------------- */

#define RC0660_AboSourceUser           0
#define RC0661_AboSourceProvider       1

typedef struct rS0645_abrtApdu {
   rTro03_stPdvHeader      stPdvHeader;

   signed long int          lAbortSource;        /* abort source           */
						 /*   aboSource  */
   signed   long int        bUserInfo;           /* OPTIONAL               */
   unsigned long int       ulUserInfo;           /* SEQUENCE OF EXTERNAL   */
   unsigned long int        vUserInfo            /* Association-information  */
			     [RCro0b_MaxExternal];
						 /* for [MAX](see rouply0.h)      */
} rT0645_abrtApdu;

/* ****************** AC-PM apdu's *************************************** */
/* ******************              *************************************** */

/* PDV header: Parameter 'lType' in 'rTro03_stPdvHeader' represents        */
/*  the 'signed long int' for the CHOICE                                   */

#define RC0665_AARQapdu                0
#define RC0666_AAREapdu                1
#define RC0667_RLRQapdu                2
#define RC0668_RLREapdu                3
#define RC0669_ABRTapdu                4

typedef union  rU0646_acseApdus {
   rTro03_stPdvHeader      stPdvHeader;          /* CHOICE  */

   rT0641_aarqApdu          aarqApdu;
   rT0642_aareApdu          aareApdu;
   rT0643_rlrqApdu          rlrqApdu;
   rT0644_rlreApdu          rlreApdu;
   rT0645_abrtApdu          abrtApdu;

} rT0646_acseApdus;

#endif	/* _ROACPM1_H */
