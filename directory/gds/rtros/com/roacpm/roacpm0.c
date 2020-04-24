/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpm0.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:07:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:23  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:47:22  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:35  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:33  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:44:27  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:18  keutel
 * 	creation
 * 	[1994/03/21  13:14:13  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roacpm0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:09 $" } ;
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
/* TYPE         : MODULE                                                   */
/*                                                                         */
/* NAME         : roacpm0.c                                                */
/*                                                                         */
/* AUTHOR       : Johann Mueller, Michael Rippstain                        */
/* DATE         : 92-01-09                                                 */
/*                                                                         */
/* COMPONENT    : ACSE protocol machine                                    */
/*                                                                         */
/* PRD#/VERS.   : RTS-V2.00                                                */
/*                                                                         */
/* DESCRIPTION  : This modul contains all functions which implements       */
/*                the association control service element protocol machine */
/*                as defined in the standards ISO 8649 and ISO 8650.       */
/*                                                                         */
/* SYSTEM DEPENDENCE :  none                                               */
/*                                                                         */
/* HISTORY      :                                                          */
/*                                                                         */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM#      */
/* 2.00 K0  | 92-01-09|  original                      |mr  |              */
/*                                                                         */
/*datoff *******************************************************************/

/***************************************************************************/
/*                                                                         */
/*                     Module Identification                               */
/*                                                                         */
/***************************************************************************/

/** static char *sccsid =    "@(#) roacpm0.c 1.2 93/12/20   RTS-V2.00";  */
#ifndef RP0000_MicV0600
/*
static char *sccsid =    "@(#)roacpm0.c" SNI_VERSION " RTROS-V03.00";
*/
#endif

/***************************************************************************/
/*                                                                         */
/*              I N C L U D E S                                            */
/*                                                                         */
/***************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>     /* general definitions                            */
#include <roerval.h>     /* error values                                   */
#include <rouply0.h>     /* upper layer common eventing def.               */
#include <rouply1.h>     /* upper layer common eventing def.               */
#include <routil0.h>     /* utilities interface                            */
#include <routil1.h>     /* utilities interface                            */
#include <rolgval.h>     /* logging values                                 */
#include <roropz0.h>     /* Inform function for the Acpm inform            */
#include <asn1st.h>      /* ASN 1 MAVRO defnitions                         */

#include <roacpm2.h>     /* ac-pm function call interface                  */
#include <rodpat0.h>     /* dispatcher eventing function interf.           */
#include <rodpat1.h>     /* dispatcher En/decode function protos           */

#include <roendeco.h>
#include <roppm00.h>     /* presentation eventing interface                */
#include <roppm01.h>     /* presentation ANS1  structures                  */
#include <roppm02.h>     /* presentation function call interf.             */
#include <roppm05.h>     /* presentation En/decode function protos         */
#include <roppmz0.h>     /* presentation eventing interface                */
#include <roppmzy.h>     /* presentation eventing interface                */

#include <rostor0.h>     /* store commen utilities                         */
#include <roacpm0.h>     /* ac-pm eventing interface                       */
#include <roacpm1.h>     /* ac-pm protocol interface                       */
#include <roacpm3.h>     /* ac-pm internals                                */
#include <roacpm4.h>     /* ac-pm local function declarations              */


#include <roacpmzx.h>    /* ac-pm prototypes for Mavros En/Decode fct      */
#include <roacpz0.h>     /* acpm inform function structures                */
#include <roacpmzy.h>    /* acpm inform function structures                */

#include <roacpm01.h>    /* ac-pm prototypes for help functions            */

/***************************************************************************/
/*                                                                         */
/*              L O C A L  V A R I A B L E S                               */
/*                                                                         */
/***************************************************************************/

/* ***************** Object identifier values for ************************ */
/* ***************** abstract syntaxes.           ************************ */

static unsigned long int acseAbstractSyntax[] = { 5, 2, 2, 1, 0, 1 };

/* ****************** Logging function name strings ********************** */
/* ******************                               ********************** */

RM0001_STRG  (rs0600, "ro0600_acpm             ")
RM0001_STRG  (rs0601, "ro0601_cleanUp          ")
RM0001_STRG  (rs0602, "ro0602_encodAcpm        ")
RM0001_STRG  (rs0603, "ro0603_decodAcpm        ")
RM0001_STRG  (rs0604, "ro0604_clearNodeAcpm    ")

RM0001_STRG  (rs0607, "ro0607_LocalError       ")
RM0001_STRG  (rs0608, "ro0608_providerAbort    ")
RM0001_STRG  (rs0609, "ro0609_initAbort        ")
RM0001_STRG  (rs060a, "ro060a_informAcpm       ")
RM0001_STRG  (rs060b, "ro060b_encodSniPAcpm    ")
RM0001_STRG  (rs060c, "ro060c_decodSniPAcpm    ")
RM0001_STRG  (rs060d, "ro060d_CheckLegal       ")


RM0001_STRG  (rs0610, "ro0610_STA0xA_ASCreq    ")
RM0001_STRG  (rs0611, "ro0611_STA2xA_ASCrsp    ")
RM0001_STRG  (rs0613, "ro0613_STA0xAARQ        ")
RM0001_STRG  (rs0614, "ro0614_STA1xAARE        ")
RM0001_STRG  (rs0616, "ro0616_STA1xP_CONcnf_neg")

RM0001_STRG  (rs0617, "ro0617_STA5xA_RLSreq    ")
RM0001_STRG  (rs0618, "ro0618_RLSrsp           ")
RM0001_STRG  (rs0621, "ro0621_RLRQ             ")
RM0001_STRG  (rs0623, "ro0623_RLRE             ")

RM0001_STRG  (rs0626, "ro0626_A_ABRreq         ")
RM0001_STRG  (rs0627, "ro0627_ABRTorP_PABind   ")

RM0001_STRG  (rs0628, "ro0628_EXTERN           ")

/***************************************************************************/
/*                                                                         */
/*              E V E N T  -  S T A T E  -  M A T R I X                    */
/*         and                                                             */
/*              T R A N S A C T I O N  - F U N C T I O N                   */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/*        (1)   Local Sequence Error and Protocol Error                    */
/*                                                                         */
/***************************************************************************/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0607_LocalError                                        */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0607_LocalError ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      This function performs error reaction if a local service           */
/*      sequence flow error occurs.                                        */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/*         R_ALSQFL                                                        */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY  (rs0607)
RM0003_RETURN (rs0607, signed short int, -R_ALSQFL)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0608_providerAbort                                     */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int NEAR PASCAL ro0608_providerAbort (
       signed   long  int       lMode,
       unsigned long  int       ulAbortReason,
       rT0650_acse        NEAR *cxtPtr,
       signed   long  int       pmevnt,
       unsigned long  int       p1,
       unsigned long  int       p2,
       unsigned long  int       p3,
       unsigned long  int       p4)

/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      This function performs ac-pm provider abort procedure.             */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/*               lMode=1      ABRT-apdu is created                         */
/*               lMode=2      A_ABRTind is created                         */
/*               lMode=others ABRT-apdu and A_ABRTind are created          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */

{/*entr*/

signed   short int          rc;
unsigned long  int         pci;
rT0646_acseApdus   FAR   *ppci;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs0608)
rc = 0;

cxtPtr->sState = RC0651_STA0;
rc             = ro0601_cleanUp (pmevnt, p1, 0, p3, 0);

if ( (! rc) && (lMode != 2) && (cxtPtr->sSpmProtoVers != RCro50_ProtVers1)) {

    /* -------- ABORT-apdu is created ------------------------------------ */
    pci  = ro0020_globalalloc (sizeof (rT0646_acseApdus));
    ppci = (pci) ? (rT0646_acseApdus FAR *) ro0021_globallock (pci)
                 : 0                                               ;
    if (ppci) {

        ppci->stPdvHeader.sPmNumb         = RC0216_Acse;
        ppci->stPdvHeader.sAbstrSynId     = cxtPtr->sAcseAbsId;
        ppci->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
        ppci->stPdvHeader.lType           = RC0669_ABRTapdu;

        ppci->abrtApdu.lAbortSource       = RC0661_AboSourceProvider;

        if (ro0022_globalunlock (pci)) rc = -R_GULOKE;
    } else {
        rc = -R_GMEMRY;
    }
} else {
    pci  = 0;
}
if (rc) RM0003_RETURN (rs0608, signed short int, rc)

if ( lMode != 1) {
    rc = ro0211_pushOutEvent ( RC0217_LcrtFct,
                               RC0693_A_ABRind, 0, 0, 0 ,ulAbortReason);
}

if ( !rc && (lMode != 2) ) {
    rc = ro0211_pushOutEvent ( RC0215_Presentation,
                               RC0694_ABRT, pci, 0, 0, 0);
}

RM0003_RETURN (rs0608, signed short int, rc)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0609_initAbort                                         */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0609_initAbort ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      This function performs error reaction if a protocol                */
/*      sequence flow error occurs.                                        */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/
/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY  (rs0609)
RM0003_RETURN (rs0609, signed short int, -R_ALSQFL)
}/*end*/


/***************************************************************************/
/*                                                                         */
/*        (2)   Connection Establishment Transaction functions             */
/*                                                                         */
/***************************************************************************/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0610_STA0xA_ASCreq                                     */
/*                                                                         */
/* AUTHOR       : Johann Mueller, Michael Rippstain                        */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0610_STA0xA_ASCreq ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      Transaction function STA0 - A_ASCreq.                              */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int          rc;

unsigned long  int          acb;

unsigned long  int           pci;

rT04z1_stAbsNameId          stAbsId;

rT0646_acseApdus      FAR   *ppci;

rTro7e_stAssocReqAcpm NEAR  *pp3 ;

rTro7a_stAcpm         NEAR  *pAcpm;
rTro4a_stReqAll       NEAR  *pAll;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0610)
rc   = 0;
pci  = 0;
ppci = 0;

pp3  = (rTro7e_stAssocReqAcpm NEAR *) ro0031_locallock  (p3);
if (! pp3 )              RM0003_RETURN (rs0610, signed short int, -R_LLOCKE)

pAcpm = &(pp3->stAcpm);
pAll  = &(pp3->stAll);

/* ------------------- copy into cxt ------------------------------------- */

cxtPtr->sSpmProtoVers = pAll->sSessProtVers;
cxtPtr->lMode         = pAll->lModeSelector;

switch (cxtPtr->lMode) {

 /* ------------------ X400Mode1984 -------------------------------------- */

 case RCro70_X400Mode1984:

   pci = p1;
   break;

 /* ------------------ NormalMode ---------------------------------------- */

 case RCro71_NormalMode  :
 case RCro72_SNIprivate  :

   pci = ro0020_globalalloc (sizeof (rT0646_acseApdus));
   if (pci) ppci = (rT0646_acseApdus FAR  *) ro0021_globallock (pci);

   if (! ppci) {
         rc = -R_GMEMRY;
         break;
   }

   /* ---- copy into cxt ------------------------------------------------- */

   rc = ro0002_objcpy(stAbsId.oAbsName,acseAbstractSyntax);
   if (rc < 0) break;

   acb = ro0214_getAcbEcb(0);
 
   rc = ro0231_getInform(acb,RC0215_Presentation,RC04zP_ACSEAbId,
                (void *)&stAbsId);
   if ( rc) break;

   cxtPtr->sAcseAbsId = stAbsId.sAbsId;

   if ((cxtPtr->sAcseAbsId < RCro6a_MinAbstrSynId) ||
       (cxtPtr->sAcseAbsId > RCro6b_MaxAbstrSynId)   ) {
       rc = -R_AABSID; break;
   }

   /* ---- generate the acse-pci ----------------------------------------- */

   ppci->stPdvHeader.sPmNumb         = RC0216_Acse;
   ppci->stPdvHeader.sAbstrSynId     = cxtPtr->sAcseAbsId;
   ppci->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
   ppci->stPdvHeader.lType           = RC0665_AARQapdu;

   ppci->aarqApdu.ulProtoVersionLength  = 1;
   ppci->aarqApdu.ulProtoVersion        = RC0640_ProtocolVers1;

   rc = ro0650_copyAplCxtName (
                 pAcpm->sApplContextId                           ,/* IN */
                (rTro01_pfvObjectId)ppci->aarqApdu.vApliCxtName );/* OUT*/
   if ( rc) break;

   rc = ro0652_prpCpyTtlQul (RC0690_Called                       ,   /* IN */
                             &(pAcpm->hAeTitle)                  ,
                             &(ppci->aarqApdu.bCalledApTitle)    ,   /* OUT*/
                             &(ppci->aarqApdu.lCalledApTitle)    ,
                             &(ppci->aarqApdu.uCalledApTitle)    ,
                             &(ppci->aarqApdu.bCalledAeQualifier),
                             &(ppci->aarqApdu.lCalledAeQualifier),
                             &(ppci->aarqApdu.uCalledAeQualifier) );
   if ( rc) break;

   ppci->aarqApdu.bCalledApInvocId = pAcpm->stInvoc.bApInvocId;
   ppci->aarqApdu.lCalledApInvocId = pAcpm->stInvoc.lApInvocId;
   ppci->aarqApdu.bCalledAeInvocId = pAcpm->stInvoc.bAeInvocId;
   ppci->aarqApdu.lCalledAeInvocId = pAcpm->stInvoc.lAeInvocId;

   rc = ro0652_prpCpyTtlQul (RC0691_Calling                       ,  /* IN */
                             NULL                                 ,
                             &(ppci->aarqApdu.bCallingApTitle)    ,  /* OUT*/
                             &(ppci->aarqApdu.lCallingApTitle)    ,
                             &(ppci->aarqApdu.uCallingApTitle)    ,
                             &(ppci->aarqApdu.bCallingAeQualifier),
                             &(ppci->aarqApdu.lCallingAeQualifier),
                             &(ppci->aarqApdu.uCallingAeQualifier) );
   if ( rc) break;

   rc = ro0654_copyInvocId ( &(ppci->aarqApdu.bCallingApInvocId),
                             &(ppci->aarqApdu.lCallingApInvocId),
                             &(ppci->aarqApdu.bCallingAeInvocId),
                             &(ppci->aarqApdu.lCallingAeInvocId) );
   if ( rc) break;

   ppci->aarqApdu.bImplInfo     = FALSE;    /* not supported in RTS-V02.00 */
   /* ppci->aarqApdu.ulImplInfo    =
      ppci->aarqApdu.vImplInfo[RC0639_MaxImplInfo] =                       */

   if (! p1) {
         ppci->aarqApdu.bUserInfo    = FALSE;
   } else {
         ppci->aarqApdu.bUserInfo    = TRUE;
         ppci->aarqApdu.vUserInfo[0] = p1;
         /* RTS-V03.00 supports Sequence of "External" of association info */
         /* call new MoveSend utility to extract SOE information from P1   */
         /* changes start vp 93/04/06                                      */
         /* correction by kc on 10/5  buserInfo to ulUserInfo              */
         rc = ro000d_MoveSend (
                 (unsigned long int FAR *)   ppci->aarqApdu.vUserInfo, 
                 (unsigned long int FAR *) &(ppci->aarqApdu.ulUserInfo));
         if (rc) break;
   }

   break;

 default:
   rc = -R_AMODER;
   break;
} /* end of switch mode */

/* ------------------- create a new event and copy into cxt--------------- */

if (ppci) if (ro0022_globalunlock (pci)) rc = -R_GULOKE;
if (ro0032_localunlock (p3 )) rc = -R_LULOKE;

switch (rc) {
  case OK:
       cxtPtr->sState         = RC0652_STA1;
       cxtPtr->sAssocInitFlag = 1;
       rc                     = ro0211_pushOutEvent (RC0215_Presentation,
                                                     RC0684_AARQ        ,
                                                     pci, 0, p3, 0     );
       break;

  case 1:   /* pdu error */
       rc =(signed short int)  ro0024_globalfree (pci);
       if (! rc)
          rc = ro0608_providerAbort ( 2, 0,
                                      cxtPtr, pmevnt, p1, p2, p3, p4);
       break;

  default:
       break;

}
RM0003_RETURN (rs0610, signed short int, rc)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0611_STA2xA_ASCrsp                                     */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0611_STA2xA_ASCrsp (
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION   :                                                         */
/*                                                                         */
/*      Transaction function STA2 - A_ASCrsp_pos                           */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int          rc;

signed   long  int          lAbortMode;
unsigned long  int          ulDiag;
unsigned long  int          ulEvent;
unsigned long  int          pci;
rT0646_acseApdus     FAR   *ppci;

rTro7g_stAssocRspAcpm NEAR *pp3;

rTro7a_stAcpm         NEAR *pAcpm;
rTro4c_stRspCnfAll    NEAR *pAll;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0611)

rc   = 0;
pci  = 0;
ppci = 0;

if ((p4 & RCro85_Redirection) == RCro85_Redirection)                 
{                                               
/* ****** commented by kc on 7/7/93 

    pci          = cxtPtr->hAARQapdu;            
* After assigning  mkae the context handle to NULL by kc on 6/7 *
    cxtPtr->hAARQapdu = LNULL;;            

    ppci = (rT0646_acseApdus FAR  *) ro0021_globallock (pci);
    if (!ppci) RM0003_RETURN (rs0611, signed short int, -R_GMEMRY)
    if (! p1)
      ppci->aarqApdu.bUserInfo    = FALSE;
    else {
      ppci->aarqApdu.bUserInfo    = TRUE;
      ppci->aarqApdu.vUserInfo[0] = p1;
      * RTS-V03.00 supports Sequence of "External" of association info *
      * call new MoveSend utility to extract SOE information from P1   *
      * changes start vp 93/04/06                                      *
      rc = ro000d_MoveSend (
                       (unsigned long  int FAR *)  ppci->aarqApdu.vUserInfo, 
                       (unsigned long  int FAR *)&(ppci->aarqApdu.ulUserInfo));
      
    } * if user data present *
    if (ppci) if (ro0022_globalunlock (pci)) rc = -R_GULOKE;
     ******** end of comment */

}/* end if Redirection -- changes vinita */                                               
else
{

pp3  = (rTro7g_stAssocRspAcpm NEAR *) ro0031_locallock  (p3);
if (! pp3 ) RM0003_RETURN (rs0611, signed short int, -R_LLOCKE)

pAcpm = &(pp3->stAcpm);
pAll  = &(pp3->stAll);

/* ------------------- copy into cxt ------------------------------------- */

cxtPtr->sSpmProtoVers = pAll->sSessProtVers;

switch (cxtPtr->lMode) {

 /* ------------------ X400Mode1984 -------------------------------------- */

 case RCro70_X400Mode1984:

   pci = p1;
   break;

 /* ------------------ NormalMode ---------------------------------------- */

 case RCro71_NormalMode  :
 case RCro72_SNIprivate  :

   pci = ro0020_globalalloc (sizeof (rT0646_acseApdus));
   if (pci) ppci = (rT0646_acseApdus FAR  *) ro0021_globallock (pci);

   if (! ppci) {
         rc = -R_GMEMRY;
         break;
   }
   /* ---- generate the acse-pci ----------------------------------------- */

   ppci->stPdvHeader.sPmNumb         = RC0216_Acse;
   ppci->stPdvHeader.sAbstrSynId     = cxtPtr->sAcseAbsId;
   ppci->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
   ppci->stPdvHeader.lType           = RC0666_AAREapdu;

   ppci->aareApdu.ulProtoVersionLength = 1;
   ppci->aareApdu.ulProtoVersion       = RC0640_ProtocolVers1;

   rc = ro0650_copyAplCxtName (
                 pAcpm->sApplContextId                           ,/* IN */
                (rTro01_pfvObjectId)ppci->aareApdu.vApliCxtName );/* OUT*/
   if ( rc) break;

   switch ( p4 & 0x000000f0) {

    case RC0620_Accepted  :
         ppci->aareApdu.lResult = RC0641_AA_Accept;
         break;

    case RC0622_TempReject:                              /* see roacpm0.h */
         ppci->aareApdu.lResult = RC0643_AA_TransientReject;
         break;

    case RC0621_PermReject:                              /* see roacpm0.h */
    default               :
         ppci->aareApdu.lResult = RC0642_AA_PermanentReject;
         break;
   } /* end of switch */

   ppci->aareApdu.lResultSourceDiag = RC0644_ResultSourceUser;

   ulDiag = p4 & 0x0000000f;
   if ( ulDiag > 0x0000000a )
      ppci->aareApdu.uResultSourceDiag.lAcseServiceUser
                                       = (signed long int) RC0626_NoReason;
   else
      ppci->aareApdu.uResultSourceDiag.lAcseServiceUser
                                       = (signed long int) ulDiag;

   rc = ro0652_prpCpyTtlQul( RC0692_Respond                       ,  /* IN */
                             &(pAcpm->hAeTitle)                  ,
                             &(ppci->aareApdu.bRespondApTitle)    ,  /* OUT*/
                             &(ppci->aareApdu.lRespondApTitle)    ,
                             &(ppci->aareApdu.uRespondApTitle)    ,
                             &(ppci->aareApdu.bRespondAeQualifier),
                             &(ppci->aareApdu.lRespondAeQualifier),
                             &(ppci->aareApdu.uRespondAeQualifier) );
   if ( rc) break;

   ppci->aareApdu.bRespondApInvocId = pAcpm->stInvoc.bApInvocId;
   ppci->aareApdu.lRespondApInvocId = pAcpm->stInvoc.lApInvocId;
   ppci->aareApdu.bRespondAeInvocId = pAcpm->stInvoc.bAeInvocId;
   ppci->aareApdu.lRespondAeInvocId = pAcpm->stInvoc.lAeInvocId;

   ppci->aareApdu.bImplInfo     = FALSE;    /* not supported in RTS-V02.00 */
   /* ppci->aareApdu.ulImplInfo    =
      ppci->aareApdu.vImplInfo[RC0639_MaxImplInfo] =                       */

   if (! p1)
      ppci->aareApdu.bUserInfo    = FALSE;
   else {
      ppci->aareApdu.bUserInfo    = TRUE;
         ppci->aareApdu.vUserInfo[0] = p1;
         /* RTS-V03.00 supports Sequence of "External" of association info */
         /* call new MoveSend utility to extract SOE information from P1   */
         /* changes start vp 93/04/06                                      */
         rc = ro000d_MoveSend (
                         (unsigned long int FAR *)  ppci->aareApdu.vUserInfo, 
                         (unsigned long int FAR *)&(ppci->aareApdu.ulUserInfo));
         if (rc) break;
   }
   break;

 default:
   rc = -R_AMODER;
   break;
} /* end of switch */

/* ------------------- create a new event and copy into cxt--------------- */

if (ppci) if (ro0022_globalunlock (pci)) rc = -R_GULOKE;
if (ro0032_localunlock (p3 )) rc = -R_LULOKE;

}  /* no redirection requested :: change vinita */

switch (rc) {
  case OK:
       if ( (p4 & 0x000000f0) == RC0620_Accepted) {
          cxtPtr->sState = RC0656_STA5;
          p4             = RCro80_Accept;
          ulEvent        = RC0685_AARE_pos;
       } else {
          cxtPtr->sState = RC0651_STA0;
          ulEvent        = RC0686_AARE_neg;
          if ( (p4 & RCro85_Redirection) == RCro85_Redirection)  ;
          else p4             = RCro81_UserReject;
       }
       rc                = ro0211_pushOutEvent (RC0215_Presentation,
                                                ulEvent,
                                                pci, p2,  p3, p4     );
       break;

  case 1:   /* pdu error */
       rc =(signed short int) ro0024_globalfree (pci);
       if ( (p4 & 0x000000f0) == RC0620_Accepted)
            lAbortMode = 3;
       else lAbortMode = 1;
       if (! rc)
          rc = ro0608_providerAbort ( lAbortMode, 0,
                                      cxtPtr, pmevnt, p1, p2, p3, p4);
       break;

  default:
       break;
}

RM0003_RETURN (rs0611, signed short int, rc)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0613_STA0xAARQ                                         */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0613_STA0xAARQ ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      Transaction function STA0 - A_AARQ                                 */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* mr? bei aarq mit ! RC0640_ProtocolVers1  */

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int          rc,i;
signed   short int          op;

rT0646_acseApdus      FAR  *ppci;
rTro7f_stAssocIndAcpm NEAR *pp3;
unsigned long  int          hSdu;

rTro7b_stIndAcpm      NEAR *pAcpm;
rTro4b_stIndAll       NEAR *pAll;
rTro5a_stSpm          NEAR *pSpm;      /* **skh: 28/7 */

unsigned long  int          hCalledAeTitle; 
unsigned long  int          ulAssocDiag;


unsigned long int           hPci;
unsigned long int           hPp3;
rT0646_acseApdus      FAR  *pPci;
rTro6g_stConnRspPpm   NEAR *pPp3;

rTro4c_stRspCnfAll    NEAR *pAll_New;
rTro5a_stSpm          NEAR *pSpm_New;  /* **skh: 28/7 */

unsigned long  int          ulConResult;

unsigned long  int          acb;
rT04z1_stAbsNameId          stAbsId;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0613)
rc   = 0;
hSdu = 0;
ppci = 0;
op   = 0;
hCalledAeTitle = 0; 
ulAssocDiag    = 0;

pp3  = (rTro7f_stAssocIndAcpm NEAR *) ro0031_locallock  (p3);
if (! pp3 ) RM0003_RETURN (rs0613, signed short int, -R_LLOCKE)

pAcpm = &(pp3->stAcpm);
pAll  = &(pp3->stAll);
pSpm  = &(pp3->stSpm);

/* ---------------- check service parameter ------------------------------ */

/* ---------------- copy service parameter into cxt ---------------------- */

/* mr? ist ea auch bei der Prsp apdu richtig den context zu fuellen */

cxtPtr->sSpmProtoVers = pAll->sSessProtVers;
cxtPtr->lMode         = pAll->lModeSelector;

switch (cxtPtr->lMode) {

 /* ------------------ X400Mode1984 -------------------------------------- */

 case RCro70_X400Mode1984:

   hSdu = p1;

   break;

 /* ------------------ NormalMode ---------------------------------------- */

 case RCro71_NormalMode  :
 case RCro72_SNIprivate  :

   rc = ro0002_objcpy(stAbsId.oAbsName,acseAbstractSyntax);
   if (rc < 0)  break;

   acb = ro0214_getAcbEcb(0);
 
   rc = ro0231_getInform(acb,RC0215_Presentation,RC04zP_ACSEAbId,
                (void *)&stAbsId);

   cxtPtr->sAcseAbsId = stAbsId.sAbsId;

   if ( (cxtPtr->sAcseAbsId < RCro6a_MinAbstrSynId) ||
        (cxtPtr->sAcseAbsId > RCro6b_MaxAbstrSynId)   ) {
       rc = -R_AABSID;
       break;
   }
   if (p1) {

       /* store handle of  p1 i.e. AARQ APDU in  ACPM context block
          for future use in case of redirection in A_ASCrsp (-ve) */
       /* We have to copy the mem. area . so this is shifted down by kc on 
          10/6 */

/*       cxtPtr->hAARQapdu = p1; * by kc 7/7/93 */

       ppci = (rT0646_acseApdus FAR  *) ro0021_globallock (p1);
       if (! ppci) {
            rc = -R_GLOCKE;
            break;
       }
/* *** start *** */
/* Commented ++
       cxtPtr->hAARQapdu =ro0020_globalalloc (sizeof(rT0646_acseApdus));
       if (cxtPtr->hAARQapdu){
         rT0646_acseApdus      FAR  *p=NULL;
         
         p = (rT0646_acseApdus FAR  *) ro0021_globallock (cxtPtr->hAARQapdu);
         if (p) {
             ro0041_memcpy   ( (char FAR *)p ,  (char FAR *) ppci ,
                        (unsigned long int ) sizeof(rT0646_acseApdus) );
                       ** p = ppci; **
             ro0022_globalunlock (cxtPtr->hAARQapdu);
         }
         else rc = -R_GLOCKE;
       }else rc = -R_GALLCE;
       if  (rc ) break;  
Commented -- */
/* **** end of kc on 10/6 *** */
   }

   /* ---------------- check pci against cxt ----------------------------- */
   if ((! p1) ||
       (ppci->stPdvHeader.sPmNumb      != RC0216_Acse       ) ||
       (ppci->stPdvHeader.lType        != RC0665_AARQapdu   )   ) {
      rc = 1;
      break;
   }

   /* ---------------- check a protocol spezific parameter --------------- */

        /* the default value is ProtoVersion1, and the decoding fct forms it
            if there is no ProtoVersion1 transmitted                       */

   if (ppci->aarqApdu.ulProtoVersion == RC0640_ProtocolVers1)      op = 1;
   else                                                            op = 2;

   if (op==1) {

       rc = ro0651_cmpAndRetrieveAplCxtName (
                        (signed short int FAR *) &(pAcpm->sApplContextId) ,
                        (rTro01_pfvObjectId    )   ppci->aarqApdu.vApliCxtName);
       /* if the AplCxtName isn't founded in p4 is this marked */
       if (rc == 1) { ulAssocDiag = RC0627_UnsupApplCtxt;
                      rc = 0;
       }
       if (rc) break;

       /* -- chenck whether the CalledApTitle and the OwnApTitle are equal -*/
       rc = ro0653_retrieveApTitle ( &(pAcpm->hCldAeTitle)              ,
                                     ppci->aarqApdu.bCalledApTitle       ,
                                     ppci->aarqApdu.lCalledApTitle       ,
                                     &(ppci->aarqApdu.uCalledApTitle)    ,
                                     ppci->aarqApdu.bCalledAeQualifier   ,
                                     ppci->aarqApdu.lCalledAeQualifier   ,
                                     &(ppci->aarqApdu.uCalledAeQualifier) );
       if (rc) break;
       /* **** check only if any of bCalledApTitle or bCalledAeQualifier  */
       /* is TRUE . by kc on 14/05/93 *********************************** */
       /* start */
       if ( ppci->aarqApdu.bCalledApTitle || ppci->aarqApdu.bCalledAeQualifier)
       /* end */
            rc = ro0657_cmpAeTitle ( pAcpm->hCldAeTitle,   /* IN  */
                                       &ulAssocDiag   ); /* OUT */
       if (rc) break;
/* kc on 21-10-93 for Called aetitle in IROS3.00 *******
       rc = ro0009_deleteAETitle ( &hCalledAeTitle);
       if (rc) break;
 ********************************************************* */
       rc = ro0655_cmpInvocId ( ppci->aarqApdu.bCalledApInvocId,
                                ppci->aarqApdu.lCalledApInvocId,
                                ppci->aarqApdu.bCalledAeInvocId,
                                ppci->aarqApdu.lCalledAeInvocId,
                                (unsigned long int FAR *)&ulAssocDiag );
       if (rc) break;

       rc = ro0653_retrieveApTitle ( &(pAcpm->hAeTitle)                   ,
                                     ppci->aarqApdu.bCallingApTitle       ,
                                     ppci->aarqApdu.lCallingApTitle       ,
                                     &(ppci->aarqApdu.uCallingApTitle)    ,
                                     ppci->aarqApdu.bCallingAeQualifier   ,
                                     ppci->aarqApdu.lCallingAeQualifier   ,
                                     &(ppci->aarqApdu.uCallingAeQualifier) );
       if (rc) break;

       pAcpm->stInvoc.bApInvocId = ppci->aarqApdu.bCallingApInvocId;
       pAcpm->stInvoc.lApInvocId = ppci->aarqApdu.lCallingApInvocId;
       pAcpm->stInvoc.bAeInvocId = ppci->aarqApdu.bCallingAeInvocId;
       pAcpm->stInvoc.lAeInvocId = ppci->aarqApdu.lCallingAeInvocId;
       
       /* ***** changes for Called Invocation Ids in IROS3.00  *** */
       /* by kc on 21-10-93 */
       pAcpm->stCldInvoc.bApInvocId = ppci->aarqApdu.bCalledApInvocId;
       pAcpm->stCldInvoc.lApInvocId = ppci->aarqApdu.lCalledApInvocId;
       pAcpm->stCldInvoc.bAeInvocId = ppci->aarqApdu.bCalledAeInvocId;
       pAcpm->stCldInvoc.lAeInvocId = ppci->aarqApdu.lCalledAeInvocId;
       /* end 21-10-93 */

       /*                          ppci->aarqApdu.bImplInfo;
       isn't used in this version  ppci->aarqApdu.ulImplInfo ;
                                   ppci->aarqApdu.vImplInfo[RC0639_MaxImplInfo] =
       */
       if ( ppci->aarqApdu.bUserInfo != 0) {
               rc = ro000e_MoveReceive( 
                       (unsigned long int FAR *)   ppci->aarqApdu.vUserInfo,
                       (unsigned long int FAR *) &(ppci->aarqApdu.ulUserInfo));

       /* this function is called to copy in P1,the sequence of  external */
       /* before pushing out the event :: changes vp 93/04/06             */
           
               if (rc) break; 
               hSdu = ppci->aarqApdu.vUserInfo[0];
          }
       break;

   } else {
       /* ----- bei aarq mit != RC0640_ProtocolVers1  -------------------- */

       hPci = ro0020_globalalloc (sizeof (rT0646_acseApdus   ));
       hPp3 = ro0030_localalloc  (sizeof (rTro6g_stConnRspPpm));

       if (hPci && hPp3)  {
               pPci = (rT0646_acseApdus    FAR  *) ro0021_globallock (hPci);
               pPp3 = (rTro6g_stConnRspPpm NEAR *) ro0031_locallock  (hPp3);

              if ((! pPci) || (! pPp3)) rc = -R_LLOCKE;
       } else {
              rc = -R_LALLCE;
       }
       if (rc) break;

       /* -------- generate the Service parameter ------------------------ */

       pAll_New  = &(pPp3->stAll);
       pSpm_New  = &(pPp3->stSpm);

       pAll_New->hRespondAddress = 0;
       pAll_New->sSessProtVers   = pAll->sSessProtVers;

       ro0041_memcpy ( (char FAR *) pSpm_New,
                       (char FAR *) pSpm    ,
                       sizeof (*pSpm_New)    );

       for ( i=0; i<=pPp3->stPpm.vAbstrSynResult[0]; i++)
          pPp3->stPpm.vAbstrSynResult[i] = pp3->stPpm.vAbstrSynResult[i];

       pPp3->stPpm.sDefContResult  = (pp3->stPpm.sDefAbstrSynId == RCro6i_NoDefContext)
                                      ? RCro6i_NoDefContext
                                      : RCro6c_Acceptance  ;
       pPp3->stPpm.ulRequirements  = pp3->stPpm.ulRequirements;

       ulConResult  = RCro81_UserReject;

       /* ---- generate the acse-pci ------------------------------------- */

       pPci->stPdvHeader.sPmNumb         = RC0216_Acse;
       pPci->stPdvHeader.sAbstrSynId     = cxtPtr->sAcseAbsId;
       pPci->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
       pPci->stPdvHeader.lType           = RC0666_AAREapdu;

       pPci->aareApdu.ulProtoVersionLength = 1;
       pPci->aareApdu.ulProtoVersion  = RC0640_ProtocolVers1;

       rc = ro0002_objcpy( pPci->aareApdu.vApliCxtName,     /* target */
                           ppci->aarqApdu.vApliCxtName );   /* source */
       if ( rc<0) break;

       pPci->aareApdu.lResult         = RC0642_AA_PermanentReject;

       pPci->aareApdu.lResultSourceDiag = RC0645_ResultSourceProvid;
       pPci->aareApdu.uResultSourceDiag.lAcseServiceProvider
                       = (signed long int) 2 ; /* no commen acse version */

       /* the incomming CalledAeTitle is mapped to RespondingAeTitle */
       /* the hCalledAeTitle is only to handle, it is deleted again */

       rc = ro0653_retrieveApTitle ( &hCalledAeTitle                     ,
                                     ppci->aarqApdu.bCalledApTitle       ,
                                     ppci->aarqApdu.lCalledApTitle       ,
                                     &(ppci->aarqApdu.uCalledApTitle)    ,
                                     ppci->aarqApdu.bCalledAeQualifier   ,
                                     ppci->aarqApdu.lCalledAeQualifier   ,
                                     &(ppci->aarqApdu.uCalledAeQualifier) );
       if (rc) break;

       rc = ro0652_prpCpyTtlQul( RC0692_Respond                       ,  /* IN */
                                 &hCalledAeTitle                      ,
                                 &(pPci->aareApdu.bRespondApTitle)    ,  /* OUT*/
                                 &(pPci->aareApdu.lRespondApTitle)    ,
                                 &(pPci->aareApdu.uRespondApTitle)    ,
                                 &(pPci->aareApdu.bRespondAeQualifier),
                                 &(pPci->aareApdu.lRespondAeQualifier),
                                 &(pPci->aareApdu.uRespondAeQualifier) );
       if ( rc) break;

       rc = ro0009_deleteAETitle ( &hCalledAeTitle);
       if (rc) break;

       pPci->aareApdu.bRespondApInvocId = ppci->aarqApdu.bCalledApInvocId;
       pPci->aareApdu.lRespondApInvocId = ppci->aarqApdu.lCalledApInvocId;
       pPci->aareApdu.bRespondAeInvocId = ppci->aarqApdu.bCalledAeInvocId;
       pPci->aareApdu.lRespondAeInvocId = ppci->aarqApdu.lCalledAeInvocId;

       ppci->aareApdu.bImplInfo     = FALSE;    /* not supported in RTS-V02.00 */
       /* ppci->aareApdu.ulImplInfo    =
          ppci->aareApdu.vImplInfo[RC0639_MaxImplInfo] =                       */

       ppci->aareApdu.bUserInfo    = FALSE;

       if (ro0022_globalunlock (hPci))  rc = -R_GULOKE;
       if (ro0032_localunlock  (hPp3))  rc = -R_LULOKE;
       if (rc) break;


   } /* end of else op */
   break;

default:  rc = 1;           /* only if mode is wrong */
   break;

} /* end of switch-mode */

if (ppci) { if (ro0022_globalunlock (p1)) rc = -R_GULOKE;
}
if (ro0032_localunlock (p3 )) rc = -R_LULOKE;

switch (rc) {
  case OK:
       /* ------- create a new event and copy into cxt ------------------- */
       if ( op==2) {

           /* --- deletes the incomming pci and the service stack -- */
           rc = ro0601_cleanUp  (pmevnt, p1, 0, p3, 0);
           if (! rc) ppci = 0;
           else      break;

           cxtPtr->sState         = RC0651_STA0;

           rc = ro0211_pushOutEvent (RC0215_Presentation,
                                     RC0686_AARE_neg    ,
                                     hPci, 0, hPp3, ulConResult);

       } else {
           cxtPtr->sState         = RC0653_STA2;
           cxtPtr->sAssocInitFlag = 0;

           rc = ro0211_pushOutEvent (RC0217_LcrtFct          ,
                                     RC0681_A_ASCind         ,
                                     hSdu, p4, p3, ulAssocDiag );
       }
       if (ppci) if (ro0024_globalfree   (p1)) rc = -R_GFREEE; 
         /* by kc for
        Redirect on 6/7 i removed  by kc on 7/7/93 */
       break;

  case 1:   /* pdu error */
       rc = ro0608_providerAbort (1, 0,
                                  cxtPtr, pmevnt, p1, p2, p3, p4);
       break;

  default:
       break;
}

RM0003_RETURN (rs0613, signed short int, rc)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0614_STA1xAARE                                         */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0614_STA1xAARE ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      Transaction function STA1 - AARE                                   */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int          rc;
signed   long  int          lAbortMode;

rT0646_acseApdus      FAR  *ppci;
unsigned long  int          hSdu;
unsigned long  int          ulEvent;
unsigned long  int          ulAssocResult;

rTro7h_stAssocCnfAcpm NEAR *pp3;

rTro7a_stAcpm         NEAR *pAcpm;
rTro4c_stRspCnfAll    NEAR *pAll;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0614)
rc   = 0;
hSdu = 0;
ppci = 0;

pp3  = (rTro7h_stAssocCnfAcpm NEAR *) ro0031_locallock  (p3);
if (! pp3 )              RM0003_RETURN (rs0614, signed short int, -R_LLOCKE)

pAcpm = &(pp3->stAcpm);
pAll  = &(pp3->stAll);

/* ---------------- check service parameter ------------------------------ */

/* ---------------- copy service parameter into cxt ---------------------- */

cxtPtr->sSpmProtoVers = pAll->sSessProtVers;

switch (cxtPtr->lMode) {

 /* ------------------ X400Mode1984 -------------------------------------- */

 case RCro70_X400Mode1984:

   /* ------- create a new event and copy into cxt ----------------------- */

   hSdu    = p1;
   if ( p4)   /* there is a reject in underlying layers */
        ulAssocResult = RC0621_PermReject;  /* for upper layer */
   else ulAssocResult = RC0620_Accepted;
   break;

 /* ------------------ NormalMode ---------------------------------------- */
 case RCro71_NormalMode  :
 case RCro72_SNIprivate  :

   if (p1) {
       ppci = (rT0646_acseApdus FAR  *) ro0021_globallock (p1);
       if (! ppci) {
            rc = -R_GLOCKE;
            break;
       }
   }

   /* ---------------- check pci against cxt ----------------------------- */

   if ((! p1) ||
   /* ---------------- check an protocol spezific parameter -------------- */
       (ppci->stPdvHeader.sPmNumb     != RC0216_Acse      ) ||
       (ppci->stPdvHeader.lType       != RC0666_AAREapdu  ) ||

       (ppci->aareApdu.ulProtoVersion != RC0640_ProtocolVers1))      rc = 1;

   if ( p4 == RCro80_Accept) {
       if ( ppci->aareApdu.lResult != RC0641_AA_Accept)              rc = 1;
   } else {
       if ((ppci->aareApdu.lResult != RC0642_AA_PermanentReject) &&
           (ppci->aareApdu.lResult != RC0643_AA_TransientReject)   ) rc = 1;
   }

   if ( rc) break;

   /* ------- read the acse-pci --------------------------------- */

   rc = ro0651_cmpAndRetrieveAplCxtName ( 
                         (signed short int FAR *)&(pAcpm->sApplContextId)   ,
                         (rTro01_pfvObjectId    )  ppci->aareApdu.vApliCxtName);
   if (rc) break;  /* if rc=1 no AplCxtName founded */

   switch ( ppci->aareApdu.lResult) {
   case RC0641_AA_Accept         : ulAssocResult = RC0620_Accepted  ; break;
   case RC0642_AA_PermanentReject: ulAssocResult = RC0621_PermReject; break;
   case RC0643_AA_TransientReject: ulAssocResult = RC0622_TempReject; break;
   default                       : break;
   }

   switch ( ppci->aareApdu.lResultSourceDiag) {
   /* if Accept the value of ResultSourceDiag is also RC0644_ResultSourceUser */
    case RC0644_ResultSourceUser:
      if ( (ppci->aareApdu.uResultSourceDiag.lAcseServiceUser >= 0) &&
           (ppci->aareApdu.uResultSourceDiag.lAcseServiceUser <=10)   )
            ulAssocResult |= (unsigned long int)
                       ppci->aareApdu.uResultSourceDiag.lAcseServiceUser;
      else  rc = 1;
      break;

    case RC0645_ResultSourceProvid:
      if ( (ppci->aareApdu.uResultSourceDiag.lAcseServiceUser >= 0) &&
           (ppci->aareApdu.uResultSourceDiag.lAcseServiceUser <= 2)   )
            ulAssocResult = (unsigned long int)
                      ppci->aareApdu.uResultSourceDiag.lAcseServiceUser |
                      RC0600_AcpmEvent;
      else  rc = 1;
      break;

    default:
      rc = 1;
      break;
   } /* end of switch resultSource */

   if ( rc) break;

   rc = ro0653_retrieveApTitle ( &(pAcpm->hAeTitle)                   ,
                                 ppci->aareApdu.bRespondApTitle       ,
                                 ppci->aareApdu.lRespondApTitle       ,
                                 &(ppci->aareApdu.uRespondApTitle)    ,
                                 ppci->aareApdu.bRespondAeQualifier   ,
                                 ppci->aareApdu.lRespondAeQualifier   ,
                                 &(ppci->aareApdu.uRespondAeQualifier) );
   if (rc) break;

   pAcpm->stInvoc.bApInvocId = ppci->aareApdu.bRespondApInvocId;
   pAcpm->stInvoc.lApInvocId = ppci->aareApdu.lRespondApInvocId;
   pAcpm->stInvoc.bAeInvocId = ppci->aareApdu.bRespondAeInvocId;
   pAcpm->stInvoc.lAeInvocId = ppci->aareApdu.lRespondAeInvocId;

   /*                          ppci->aareApdu.bImplInfo;
   mr? isn't used in this version  ppci->aareApdu.ulImplInfo ;
                               ppci->aareApdu.vImplInfo[RC0639_MaxImplInfo] =
   */

   if ( ppci->aareApdu.bUserInfo != 0) {
               rc = ro000e_MoveReceive( 
                        (unsigned long int FAR *)  ppci->aareApdu.vUserInfo,
                        (unsigned long int FAR *)&(ppci->aareApdu.ulUserInfo));

       /* this function is called to copy in P1,the sequence of  external */
       /* before pushing out the event :: changes vp 93/04/06             */
           
               if (rc) break; 
           hSdu = ppci->aareApdu.vUserInfo[0];
      }
   break;
default:
   rc = -R_AMODER;
   break;

} /* end of switch-mode */

if (ppci) { if (ro0022_globalunlock (p1)) rc = -R_GULOKE;
}
if (ro0032_localunlock (p3 )) rc = -R_LULOKE;

switch (rc) {
  case OK:
       /* ------- create a new event and copy into cxt ----------------------- */
       if (p4 == RCro80_Accept) {
          cxtPtr->sState = RC0656_STA5;
          ulEvent        = RC0682_A_ASCcnf_pos;
       } else {
          cxtPtr->sState = RC0651_STA0;
          ulEvent        = RC0683_A_ASCcnf_neg;
       }

       rc  = ro0211_pushOutEvent ( RC0217_LcrtFct      ,
                                   ulEvent             ,
                                   hSdu, 0, p3, ulAssocResult );

       if (ppci) if (ro0024_globalfree   (p1)) rc = -R_GFREEE;
       break;

  case 1:   /* pdu error */
       if (p4 == RCro80_Accept) lAbortMode = 3;
       else                     lAbortMode = 2;

       rc = ro0608_providerAbort (lAbortMode, RC0612_AAREapduError,
                                  cxtPtr, pmevnt, p1, p2, p3, p4);
       break;
  default:
       break;
}
RM0003_RETURN (rs0614, signed short int, rc)
}/*end*/


/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0616_STA1xP_CONcnf_neg                                 */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0616_STA1xP_CONcnf_neg ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      Transaction function STA1 - P_CONcnf_neg                           */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int            rc;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY  (rs0616)

cxtPtr->sState = RC0651_STA0;
rc             = ro0211_pushOutEvent (RC0217_LcrtFct     ,
                                      RC0683_A_ASCcnf_neg,
                                      0, 0, p3, p4       );


RM0003_RETURN (rs0616,signed short int, rc)
}/*end*/

/***************************************************************************/
/*                                                                         */
/*        (3)   Connection Release (normal) Transaction functions          */
/*                                                                         */
/***************************************************************************/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0617_STA5xA_RLSreq                                     */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0617_STA5xA_RLSreq ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      Transaction function STA5 - A_RLSreq                               */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int         rc;

unsigned long  int         pci;
rT0646_acseApdus   FAR   *ppci;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0617)
rc   = 0;
pci  = 0;
ppci = 0;

/* ------------------- copy into cxt ------------------------------------- */

switch (cxtPtr->lMode) {

 /* ------------------ X400Mode1984 -------------------------------------- */

 case RCro70_X400Mode1984:

   pci = p1;

   break;

 /* ------------------ NormalMode ---------------------------------------- */

 case RCro71_NormalMode  :
 case RCro72_SNIprivate  :

   pci = ro0020_globalalloc (sizeof (rT0646_acseApdus));
   if (pci) ppci = (rT0646_acseApdus FAR  *) ro0021_globallock (pci);

   if (! ppci) {
         rc = -R_GMEMRY;
         break;
   }

   /* ---- generate the acse-pci ----------------------------------------- */

   ppci->stPdvHeader.sPmNumb         = RC0216_Acse;
   ppci->stPdvHeader.sAbstrSynId     = cxtPtr->sAcseAbsId;
   ppci->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
   ppci->stPdvHeader.lType           = RC0667_RLRQapdu;

   switch (p2) {
   case RC061b_Normal     :
               ppci->rlrqApdu.bReason = TRUE;
               ppci->rlrqApdu.lReason = RC0652_RLRQreasonNormal;
               break;
   case RC061c_Urgent     :
               ppci->rlrqApdu.bReason = TRUE;
               ppci->rlrqApdu.lReason = RC0653_RLRQreasonUrgent;
               break;
   case RC061d_UserDefined:
               ppci->rlrqApdu.bReason = TRUE;
               ppci->rlrqApdu.lReason = RC0654_RLRQreasonUsrDef;
               break;
   case RC061a_NotDefined :
   default                :
               ppci->rlrqApdu.bReason = FALSE;
               ppci->rlrqApdu.lReason = 0;
               break;
   }

   if (! p1) {
         ppci->rlrqApdu.bUserInfo    = FALSE;
   } else {
         ppci->rlrqApdu.bUserInfo    = TRUE;
         ppci->rlrqApdu.vUserInfo[0] = p1;
         /* RTS-V03.00 supports Sequence of "External" of association info */
         /* call new MoveSend utility to extract SOE information from P1   */
         /* changes start vp 93/04/06                                      */
         rc = ro000d_MoveSend (
                  (unsigned long int FAR *)  ppci->rlrqApdu.vUserInfo, 
                  (unsigned long int FAR *)&(ppci->rlrqApdu.ulUserInfo));
         if (rc) break;
   }

   break;

 default:
   rc = -R_AMODER;
   break;
} /* end of switch */

/* ------------------- create a new event and copy into cxt--------------- */

if (ppci) if (ro0022_globalunlock (pci)) rc = -R_GULOKE;

switch (rc) {
  case OK:
       cxtPtr->sState = RC0654_STA3;
       rc             = ro0211_pushOutEvent (RC0215_Presentation,
                                             RC0690_RLRQ,
                                             pci, 0, 0, 0);
       break;

  default:
       break;
}

RM0003_RETURN (rs0617, signed short int, rc)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0618_RLSrsp                                            */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0618_RLSrsp ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      Transaction function xxxx - A_RLSres                               */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int      rc;

unsigned long  int      ulEvent;
unsigned long  int      pci;
rT0646_acseApdus   FAR *ppci;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0618)
rc   = 0;
pci  = 0;
ppci = 0;

/* ------------------- copy into cxt ------------------------------------- */

switch (cxtPtr->lMode) {

 /* ------------------ X400Mode1984 -------------------------------------- */

 case RCro70_X400Mode1984:

   pci = p1;

   break;

 /* ------------------ NormalMode ---------------------------------------- */

 case RCro71_NormalMode  :
 case RCro72_SNIprivate  :

   pci = ro0020_globalalloc (sizeof (rT0646_acseApdus));
   if (pci) ppci = (rT0646_acseApdus FAR  *) ro0021_globallock (pci);

   if (! ppci) {
         rc = -R_GMEMRY;
         break;
   }

   /* ---- generate the acse-pci ----------------------------------------- */

   ppci->stPdvHeader.sPmNumb         = RC0216_Acse;
   ppci->stPdvHeader.sAbstrSynId     = cxtPtr->sAcseAbsId;
   ppci->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
   ppci->stPdvHeader.lType           = RC0668_RLREapdu;

   switch (p2) {
   case RC061j_Normal     :
               ppci->rlreApdu.bReason = TRUE;
               ppci->rlreApdu.lReason = RC0657_RLREreasonNormal;
               break;
   case RC061k_NotFinished:
               ppci->rlreApdu.bReason = TRUE;
               ppci->rlreApdu.lReason = RC0658_RLREreasonNotFin;
               break;
   case RC061l_UserDefined:
               ppci->rlreApdu.bReason = TRUE;
               ppci->rlreApdu.lReason = RC0659_RLREreasonUsrDef;
               break;
   case RC061i_NotDefined :
   default                :
               ppci->rlreApdu.bReason = FALSE;
               ppci->rlreApdu.lReason = 0;
               break;
   }

   if (! p1) {
         ppci->rlreApdu.bUserInfo    = FALSE;
   } else {
         ppci->rlreApdu.bUserInfo    = TRUE;
         ppci->rlreApdu.vUserInfo[0] = p1;
         /* RTS-V03.00 supports Sequence of "External" of association info */
         /* call new MoveSend utility to extract SOE information from P1   */
         /* changes start vp 93/04/06                                      */
         rc = ro000d_MoveSend (
                     (unsigned long int FAR *)  ppci->rlreApdu.vUserInfo, 
                     (unsigned long int FAR *)&(ppci->rlreApdu.ulUserInfo));
         if (rc) break;
   }

   break;

 default:
   rc = -R_AMODER;
   break;
} /* end of switch */

/* ------------------- create a new event and copy into cxt--------------- */

if (ppci) if (ro0022_globalunlock (pci)) rc = -R_GULOKE;

switch (rc) {
  case OK:
       if ( p4 == RCro90_Affirmative) {              /* RCro90_Affirmative */
            cxtPtr->sState = (cxtPtr->sState == RC0655_STA4)
                             ? RC0651_STA0
                             : RC0654_STA3;
            ulEvent        = RC0691_RLRE_pos;

       } else {                                         /* RCro91_Negative */
            cxtPtr->sState = RC0656_STA5;
            ulEvent        = RC0692_RLRE_neg;
       }

       rc = ro0211_pushOutEvent (RC0215_Presentation,
                                 ulEvent,
                                 pci, 0, 0, p4);
       break;

  default:
       break;
}

RM0003_RETURN (rs0618, signed short int, rc)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0621_RLRQ                                              */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0621_RLRQ ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      Transaction function xxxx - RLRQ                                   */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS              :                                           */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int            rc;

unsigned long  int          ulNewp2;
unsigned long  int          hSdu;
rT0646_acseApdus     FAR   *ppci;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0621)
rc   = 0;
hSdu = 0;
ppci = 0;

/* ---------------- check service parameter ------------------------------ */
/* ---------------- copy service parameter into cxt ---------------------- */

switch (cxtPtr->lMode) {

 /* ------------------ X400Mode1984 -------------------------------------- */
 case RCro70_X400Mode1984:

   hSdu    = p1;
   ulNewp2 = RC061a_NotDefined;
   break;

 /* ------------------ NormalMode ---------------------------------------- */
 case RCro71_NormalMode  :
 case RCro72_SNIprivate  :

   if (p1) {
       ppci = (rT0646_acseApdus FAR  *) ro0021_globallock (p1);
       if (! ppci) {
            rc = -R_GLOCKE;
            break;
       }
   }
   /* ---------------- check pci against cxt ----------------------------- */
   if ((! p1) ||
       (ppci->stPdvHeader.sPmNumb      != RC0216_Acse       ) ||
       (ppci->stPdvHeader.lType        != RC0667_RLRQapdu   )   ) {
       rc = 1;
       break;
   }
   /* ---------------- check an protocol spezific parameter -------------- */

   if (! ppci->rlrqApdu.bReason)
      ulNewp2 = RC061a_NotDefined;
   else
      switch (ppci->rlrqApdu.lReason) {
        case RC0652_RLRQreasonNormal: ulNewp2 = RC061b_Normal     ; break;
        case RC0653_RLRQreasonUrgent: ulNewp2 = RC061c_Urgent     ; break;
        case RC0654_RLRQreasonUsrDef: ulNewp2 = RC061d_UserDefined; break;
        default                     : ulNewp2 = RC061a_NotDefined ; break;
      }

   if ( ppci->rlrqApdu.bUserInfo != 0) {
               rc = ro000e_MoveReceive( 
                       (unsigned long int FAR *)  ppci->rlrqApdu.vUserInfo,
                        (unsigned long int FAR *)&(ppci->rlrqApdu.ulUserInfo));

       /* this function is called to copy in P1,the sequence of  external */
       /* before pushing out the event :: changes vp 93/04/06             */
           
               if (rc) break; 
           hSdu = ppci->rlrqApdu.vUserInfo[0];
      }

   break;

default:
   rc = -R_AMODER ;
   break;

} /* end of switch-mode */

/* ------------------- create a new event and copy into cxt--------------- */

if (ppci) if (ro0022_globalunlock (p1)) rc = -R_GULOKE;

switch (rc) {
  case OK:
            if ( cxtPtr->sState == RC0656_STA5)  cxtPtr->sState = RC0655_STA4;
       else if ((cxtPtr->sState == RC0654_STA3) &&
                (cxtPtr->sAssocInitFlag       )) cxtPtr->sState = RC0657_STA6;
       else                                      cxtPtr->sState = RC0658_STA7;

       rc = ro0211_pushOutEvent (RC0217_LcrtFct ,
                                 RC0687_A_RLSind,
                                 hSdu, ulNewp2, 0, 0 );

       if (ppci) if (ro0024_globalfree   (p1)) rc = -R_GFREEE;
       break;

  case 1:   /* pdu error */
       rc = ro0608_providerAbort (3, RC0613_RLRQapduError,
                                  cxtPtr, pmevnt, p1, p2, p3, p4);
       break;

  default:
       break;
}

RM0003_RETURN (rs0621, signed short int, rc)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0623_RLRE                                              */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0623_RLRE ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      Transaction function xxxx - RLRE-pos/neg                           */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int      rc;

signed   long  int      lAbortMode;
unsigned long  int      ulEvent;

unsigned long  int      ulNewp2;
unsigned long  int      hSdu;
rT0646_acseApdus   FAR *ppci;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0623)
rc   = 0;
hSdu = 0;
ppci = 0;

switch (cxtPtr->lMode) {

 /* ------------------ X400Mode1984 -------------------------------------- */
 case RCro70_X400Mode1984:

   hSdu    = p1;
   ulNewp2 = RC061i_NotDefined;

   break;

 /* ------------------ NormalMode ---------------------------------------- */
 case RCro71_NormalMode  :
 case RCro72_SNIprivate  :

   if (p1) {
       ppci = (rT0646_acseApdus FAR  *) ro0021_globallock (p1);
       if (! ppci) {
            rc = -R_GLOCKE;
            break;
       }
   }
   /* ---------------- check pci against cxt ----------------------------- */

   if ((! p1) ||
       (ppci->stPdvHeader.sPmNumb      != RC0216_Acse       ) ||
       (ppci->stPdvHeader.lType        != RC0668_RLREapdu   )   ) {
       rc = 1;
       break;
   }
   /* ---------------- check an protocol spezific parameter -------------- */

   if (! ppci->rlreApdu.bReason)
      ulNewp2 = RC061i_NotDefined;
   else
      switch (ppci->rlrqApdu.lReason) {
        case RC0657_RLREreasonNormal: ulNewp2 = RC061j_Normal     ; break;
        case RC0658_RLREreasonNotFin: ulNewp2 = RC061k_NotFinished; break;
        case RC0659_RLREreasonUsrDef: ulNewp2 = RC061l_UserDefined; break;
        default                     : ulNewp2 = RC061i_NotDefined ; break;
      }

   if ( ppci->rlreApdu.bUserInfo != 0) {
               rc = ro000e_MoveReceive( 
                         (unsigned long int FAR *)  ppci->rlreApdu.vUserInfo,
                         (unsigned long int FAR *)&(ppci->rlreApdu.ulUserInfo));

       /* this function is called to copy in P1,the sequence of  external */
       /* before pushing out the event :: changes vp 93/04/06             */
           
               if (rc) break; 
           hSdu = ppci->rlreApdu.vUserInfo[0];
      }

   break;

default:
   rc = -R_AMODER ;
   break;

} /* end of switch-mode */

/* ------------------- create a new event and copy into cxt--------------- */

if (ppci) if (ro0022_globalunlock (p1)) rc = -R_GULOKE;

switch (rc) {
  case OK:
       if ( p4 == RCro90_Affirmative) {              /* RCro90_Affirmative */
            cxtPtr->sState = (cxtPtr->sState==RC0654_STA3)
                             ? RC0651_STA0
                             : RC0655_STA4;
            ulEvent        = RC0688_A_RLScnf_pos;
       } else {                                         /* RCro91_Negative */
            cxtPtr->sState = RC0656_STA5;
            ulEvent        = RC0689_A_RLScnf_neg;
       }

       rc = ro0211_pushOutEvent (RC0217_LcrtFct,
                                 ulEvent,
                                 hSdu, ulNewp2, 0, p4);

       if (ppci) if (ro0024_globalfree   (p1)) rc = -R_GFREEE;
       break;

  case 1:   /* pdu error */
       if ( p4 == RCro90_Affirmative) lAbortMode = 2;
       else                           lAbortMode = 3;

       rc = ro0608_providerAbort ( lAbortMode, RC0614_RLREapduError,
                                   cxtPtr, pmevnt, p1, p2, p3, p4);
       break;

  default:
       break;
}

RM0003_RETURN (rs0623, signed short int, rc)
}/*end*/

/***************************************************************************/
/*                                                                         */
/*        (4)   Connection Release (abort) Transaction functions           */
/*                                                                         */
/***************************************************************************/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0626_A_ABRreq                                          */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0626_A_ABRreq ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      Transaction function xxxx - A_ABRreq                               */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS              :                                           */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/
signed   short int        op;
signed   short int        rc;

unsigned long  int       pci;
rT0646_acseApdus  FAR  *ppci;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0626)
rc   = 0;
pci  = 0;
ppci = 0;
op   = 0;

switch (cxtPtr->lMode) {

 /* ------------------ X400Mode1984 -------------------------------------- */

 case RCro70_X400Mode1984:

   pci = p1;

   break;

 /* ------------------ NormalMode ---------------------------------------- */

 case RCro71_NormalMode  :
 case RCro72_SNIprivate  :

   /* ---------------- check a protocol spezific parameter --------------- */

   if   (cxtPtr->sSpmProtoVers == RCro50_ProtVers1) op = 1; /* sess vers1  */
   else                                             op = 2; /* other sess  */

   if ( op == 1) {
        pci = p1;
   } else {
        pci = ro0020_globalalloc (sizeof (rT0646_acseApdus));
        if (pci) ppci = (rT0646_acseApdus FAR  *) ro0021_globallock (pci);

        if (! ppci) {
              rc = -R_GMEMRY;
              break;
        }

        /* ---- generate the acse-pci ------------------------------------ */

        ppci->stPdvHeader.sPmNumb         = RC0216_Acse;
        ppci->stPdvHeader.sAbstrSynId     = cxtPtr->sAcseAbsId;
        ppci->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
        ppci->stPdvHeader.lType           = RC0669_ABRTapdu;

        ppci->abrtApdu.lAbortSource = RC0660_AboSourceUser;

        if (! p1) {
              ppci->abrtApdu.bUserInfo    = FALSE;
        } else {
              ppci->abrtApdu.bUserInfo    = TRUE;
              ppci->abrtApdu.vUserInfo[0] = p1;
         /* RTS-V03.00 supports Sequence of "External" of association info */
         /* call new MoveSend utility to extract SOE information from P1   */
         /* changes start vp 93/04/06                                      */
         rc = ro000d_MoveSend (
                    (unsigned long int FAR *)  ppci->abrtApdu.vUserInfo, 
                    (unsigned long int FAR *)&(ppci->abrtApdu.ulUserInfo));
         if (rc) break;
        }
   }
   break;

 default:
   rc = -R_AMODER;
   break;
} /* end of switch */

/* ------------------- create a new event and copy into cxt--------------- */

if (ppci) if (ro0022_globalunlock (pci)) rc = -R_GULOKE;

switch (rc) {
  case OK:
       cxtPtr->sState = RC0651_STA0;
       rc             = ro0211_pushOutEvent (RC0215_Presentation,
                                             RC0694_ABRT,
                                             pci, 0, 0, 0);
       break;

  default:
       break;
}

RM0003_RETURN (rs0626, signed short int, rc)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0627_ABRTorP_PABind                                    */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0627_ABRTorP_PABind ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      Transaction function xxxx - ABRT/P_PABind                          */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int      rc;

rT0646_acseApdus  FAR  *ppci;
unsigned long  int      hSdu;

unsigned long  int      ulNewp2;
unsigned long  int      ulNewp4;

unsigned long  int      ulEvent;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0627)
rc   = 0;
hSdu = 0;
ppci = 0;

switch (cxtPtr->lMode) {

 /* ------------------ X400Mode1984 -------------------------------------- */
 case RCro70_X400Mode1984:

      if (pmevnt == RC0676_P_PABind) {

      /* ************* P-Provider Abort indication *********************** */
      /* ***************************************************************** */
          ulNewp2 = p2;  /* additional Abort info */
          p1      = 0;   /* means a pdv-node for upper layers */
          ulNewp4 = p4;  /* abort reason */
          ulEvent = RC0695_A_PABind;

      } else {
      /* ************* P-User Abort indication *************************** */
      /* ***************************************************************** */
          hSdu    = p1;
          p1      = 0;   /* means a pdv-node for upper layers */
          ulNewp2 = 0;
          ulNewp4 = 0;
          ulEvent = RC0693_A_ABRind;
      }

   break;

 /* ------------------ NormalMode ---------------------------------------- */
 case RCro71_NormalMode  :
 case RCro72_SNIprivate  :

      if (pmevnt == RC0676_P_PABind) {              /* old was autevt */

         /* ************* P-Provider Abort indication ******************** */
         /* ************************************************************** */
         ulNewp2 = p2;  /* additional Abort info */
         p1      = 0;   /* means a pdv-node for upper layers */
         ulNewp4 = p4;  /* abort reason */
         ulEvent = RC0695_A_PABind;

      } else {
         /* ************* P-User Abort indication ************************ */
         /* ************************************************************** */
         if (p1) {
             ppci = (rT0646_acseApdus FAR  *) ro0021_globallock (p1);
             if (! ppci) {
                  rc = -R_GLOCKE;
                  break;
             }
         }
         if ( cxtPtr->sSpmProtoVers == RCro50_ProtVers1) {

         /* ******** P-User Abort ind, no ABRT-apdu ********************** */
            /* ---------------- check pci against cxt -------------------- */

            if ( ppci &&
                (ppci->stPdvHeader.sPmNumb == RC0216_Acse )) {
               /* at Session Version1 there is no Acse-Pdu, follows Pdu-error */
                rc = 1;
                break;
            }

            hSdu    = p1;
            p1      = 0;   /* means a pdv-node for upper layers */
            ulNewp4 = RC0610_AA_UserABORT;
            ulEvent = RC0693_A_ABRind;

         } else {
         /* ******** P-User Abort ind, with ABRT-apdu ******************** */
            /* ---------------- check pci against cxt -------------------- */

            if (( ! p1)                                                 ||
                ( ppci->stPdvHeader.sPmNumb     != RC0216_Acse       )  ||
                ( ppci->stPdvHeader.lType       != RC0669_ABRTapdu   )  ||
                ((ppci->abrtApdu.lAbortSource   != RC0660_AboSourceUser) &&
                 (ppci->abrtApdu.lAbortSource   != RC0661_AboSourceProvider))) {
                rc = 1;
                break;
            }

            if (ppci->abrtApdu.lAbortSource  == RC0660_AboSourceUser) {
            /* -*-*-*- abort is initiated from user -*-*-*-*-*-*-*-*-*-*-* */
                if ( ppci->abrtApdu.bUserInfo != 0)  {
               rc = ro000e_MoveReceive( 
                         (unsigned long int FAR *) ppci->abrtApdu.vUserInfo,
                         (unsigned long int FAR *)&(ppci->abrtApdu.ulUserInfo));

       /* this function is called to copy in P1,the sequence of  external */
       /* before pushing out the event :: changes vp 93/04/06             */
           
               if (rc) break; 
                        hSdu = ppci->abrtApdu.vUserInfo[0];
                   }
                ulNewp2 = 0;
                ulNewp4 = RC0610_AA_UserABORT;
                ulEvent = RC0693_A_ABRind;

            } else {
            /* -*-*-*- abort is initiated from provider (acse)-*-*-*-*-*-* */
                if ( ppci->abrtApdu.bUserInfo != 0) {
                     rc = 1; /* is it right to do this? :: vinita */
                     break;
                }
                ulNewp2 = 0;
                ulNewp4 = RC0611_AA_ProvABORT;
                ulEvent = RC0693_A_ABRind;
            }

         } /* end of else != RCro50_ProtVers1 */
      } /* end of else "P-User Abort indication" */

   break;

default:
   rc = -R_AMODER ;
   break;

} /* end of switch-mode */

/* ------------------- create a new event and copy into cxt--------------- */

if (ppci) if (ro0022_globalunlock (p1)) rc = -R_GULOKE;

cxtPtr->sState = RC0651_STA0;

switch (rc) {
  case OK:
       rc = ro0211_pushOutEvent (RC0217_LcrtFct,
                                 ulEvent,
                                 hSdu, ulNewp2, 0, ulNewp4);

       if ( p1) if (ro0024_globalfree   (p1)) rc = -R_GFREEE;
       break;
  case 1:     /* pdu error */
       rc = ro0601_cleanUp  (pmevnt, p1, 0, 0, 0);
       if (! rc)
           rc = ro0211_pushOutEvent (RC0217_LcrtFct,
                                     RC0693_A_ABRind,
                                     0, 0, 0, RC0615_ABRTapduError);
       break;
  default:
       break;
}
RM0003_RETURN (rs0627, signed short int, rc)
}/*end*/


/***************************************************************************/
/*                                                                         */
/*        (5)   RESET - Function                                           */
/*                                                                         */
/***************************************************************************/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0628_EXTERN                                            */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-01-10                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int NEAR PASCAL ro0628_EXTERN ( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4)
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      Transaction function xxxx - EXTERN_1/EXTERN_2                      */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0628)

cxtPtr->sState = RC0656_STA5;

RM0003_RETURN (rs0628, signed short int, 0)
}/*end*/



/***************************************************************************/
/*                                                                         */
/*        (6)   Event - State - Matrix                                     */
/*                                                                         */
/***************************************************************************/

static signed short int

(NEAR PASCAL *automata [RC0650_MaxStates] [RC0660_MaxInEvents]) (
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4) =

{ /* state:    RC0651_STA0          */   {
  /*    event: RC0661_A_ASCreq      */   ro0610_STA0xA_ASCreq,
  /*           RC0662_A_ASCrsp_pos  */   ro0607_LocalError,
  /*           RC0663_A_ASCrsp_neg  */   ro0607_LocalError,
  /*           RC0664_AARQ          */   ro0613_STA0xAARQ,
  /*           RC0665_AARE_pos      */   ro0609_initAbort,
  /*           RC0666_AARE_neg      */   ro0609_initAbort,
  /*           RC0667_P_CONcnf_neg  */   ro0609_initAbort,

  /*           RC0668_A_RLSreq      */   ro0607_LocalError,
  /*           RC0669_A_RLSres_pos  */   ro0607_LocalError,
  /*           RC0670_A_RLSrsp_neg  */   ro0607_LocalError,
  /*           RC0671_RLRQ          */   ro0609_initAbort,
  /*           RC0672_RLRE_pos      */   ro0609_initAbort,
  /*           RC0673_RLRE_neg      */   ro0609_initAbort,

  /*           RC0674_A_ABRreq      */   ro0607_LocalError,
  /*           RC0675_ABRT          */   ro0609_initAbort,
  /*           RC0676_P_PABind      */   ro0609_initAbort,

  /*           RC0677_EXTRN_1       */   ro0609_initAbort,
  /*           RC0678_EXTRN_2       */   ro0607_LocalError,
  /* End of state                   */   },

  /* state:    RC0652_STA1          */   {
  /*    event: RC0661_A_ASCreq      */   ro0607_LocalError,
  /*           RC0662_A_ASCrsp_pos  */   ro0607_LocalError,
  /*           RC0663_A_ASCrsp_neg  */   ro0607_LocalError,
  /*           RC0664_AARQ          */   ro0609_initAbort,
  /*           RC0665_AARE_pos      */   ro0614_STA1xAARE,
  /*           RC0666_AARE_neg      */   ro0614_STA1xAARE,
  /*           RC0667_P_CONcnf_neg  */   ro0616_STA1xP_CONcnf_neg,

  /*           RC0668_A_RLSreq      */   ro0607_LocalError,
  /*           RC0669_A_RLSres_pos  */   ro0607_LocalError,
  /*           RC0670_A_RLSrsp_neg  */   ro0607_LocalError,
  /*           RC0671_RLRQ          */   ro0609_initAbort,
  /*           RC0672_RLRE_pos      */   ro0609_initAbort,
  /*           RC0673_RLRE_neg      */   ro0609_initAbort,

  /*           RC0674_A_ABRreq      */   ro0626_A_ABRreq,
  /*           RC0675_ABRT          */   ro0627_ABRTorP_PABind,
  /*           RC0676_P_PABind      */   ro0627_ABRTorP_PABind,

  /*           RC0677_EXTRN_1       */   ro0609_initAbort,
  /*           RC0678_EXTRN_2       */   ro0607_LocalError,
  /* End of state                   */   },


  /* state:    RC0653_STA2          */   {
  /*    event: RC0661_A_ASCreq      */   ro0607_LocalError,
  /*           RC0662_A_ASCrsp_pos  */   ro0611_STA2xA_ASCrsp,
  /*           RC0663_A_ASCrsp_neg  */   ro0611_STA2xA_ASCrsp,
  /*           RC0664_AARQ          */   ro0609_initAbort,
  /*           RC0665_AARE_pos      */   ro0609_initAbort,
  /*           RC0666_AARE_neg      */   ro0609_initAbort,
  /*           RC0667_P_CONcnf_neg  */   ro0609_initAbort,

  /*           RC0668_A_RLSreq      */   ro0607_LocalError,
  /*           RC0669_A_RLSres_pos  */   ro0607_LocalError,
  /*           RC0670_A_RLSrsp_neg  */   ro0607_LocalError,
  /*           RC0671_RLRQ          */   ro0609_initAbort,
  /*           RC0672_RLRE_pos      */   ro0609_initAbort,
  /*           RC0673_RLRE_neg      */   ro0609_initAbort,

  /*           RC0674_A_ABRreq      */   ro0626_A_ABRreq,
  /*           RC0675_ABRT          */   ro0627_ABRTorP_PABind,
  /*           RC0676_P_PABind      */   ro0627_ABRTorP_PABind,

  /*           RC0677_EXTRN_1       */   ro0609_initAbort,
  /*           RC0678_EXTRN_2       */   ro0607_LocalError,
  /* End of state                   */   },

  /* state:    RC0654_STA3          */   {
  /*    event: RC0661_A_ASCreq      */   ro0607_LocalError,
  /*           RC0662_A_ASCrsp_pos  */   ro0607_LocalError,
  /*           RC0663_A_ASCrsp_neg  */   ro0607_LocalError,
  /*           RC0664_AARQ          */   ro0609_initAbort,
  /*           RC0665_AARE_pos      */   ro0609_initAbort,
  /*           RC0666_AARE_neg      */   ro0609_initAbort,
  /*           RC0667_P_CONcnf_neg  */   ro0609_initAbort,

  /*           RC0668_A_RLSreq      */   ro0607_LocalError,
  /*           RC0669_A_RLSres_pos  */   ro0607_LocalError,
  /*           RC0670_A_RLSrsp_neg  */   ro0607_LocalError,
  /*           RC0671_RLRQ          */   ro0621_RLRQ,
  /*           RC0672_RLRE_pos      */   ro0623_RLRE,
  /*           RC0673_RLRE_neg      */   ro0623_RLRE,

  /*           RC0674_A_ABRreq      */   ro0626_A_ABRreq,
  /*           RC0675_ABRT          */   ro0627_ABRTorP_PABind,
  /*           RC0676_P_PABind      */   ro0627_ABRTorP_PABind,

  /*           RC0677_EXTRN_1       */   ro0628_EXTERN,
  /*           RC0678_EXTRN_2       */   ro0607_LocalError,
  /* End of state                    */   },


  /* state:    RC0655_STA4          */   {
  /*    event: RC0661_A_ASCreq      */   ro0607_LocalError,
  /*           RC0662_A_ASCrsp_pos  */   ro0607_LocalError,
  /*           RC0663_A_ASCrsp_neg  */   ro0607_LocalError,
  /*           RC0664_AARQ          */   ro0609_initAbort,
  /*           RC0665_AARE_pos      */   ro0609_initAbort,
  /*           RC0666_AARE_neg      */   ro0609_initAbort,
  /*           RC0667_P_CONcnf_neg  */   ro0609_initAbort,

  /*           RC0668_A_RLSreq      */   ro0607_LocalError,
  /*           RC0669_A_RLSres_pos  */   ro0618_RLSrsp,
  /*           RC0670_A_RLSrsp_neg  */   ro0618_RLSrsp,
  /*           RC0671_RLRQ          */   ro0609_initAbort,
  /*           RC0672_RLRE_pos      */   ro0609_initAbort,
  /*           RC0673_RLRE_neg      */   ro0609_initAbort,

  /*           RC0674_A_ABRreq      */   ro0626_A_ABRreq,
  /*           RC0675_ABRT          */   ro0627_ABRTorP_PABind,
  /*           RC0676_P_PABind      */   ro0627_ABRTorP_PABind,

  /*           RC0677_EXTRN_1       */   ro0609_initAbort,
  /*           RC0678_EXTRN_2       */   ro0628_EXTERN,
  /* End of state                   */   },

  /* state:    RC0656_STA5          */   {
  /*    event: RC0661_A_ASCreq      */   ro0607_LocalError,
  /*           RC0662_A_ASCrsp_pos  */   ro0607_LocalError,
  /*           RC0663_A_ASCrsp_neg  */   ro0607_LocalError,
  /*           RC0664_AARQ          */   ro0609_initAbort,
  /*           RC0665_AARE_pos      */   ro0609_initAbort,
  /*           RC0666_AARE_neg      */   ro0609_initAbort,
  /*           RC0667_P_CONcnf_neg  */   ro0609_initAbort,

  /*           RC0668_A_RLSreq      */   ro0617_STA5xA_RLSreq,
  /*           RC0669_A_RLSres_pos  */   ro0607_LocalError,
  /*           RC0670_A_RLSrsp_neg  */   ro0607_LocalError,
  /*           RC0671_RLRQ          */   ro0621_RLRQ,
  /*           RC0672_RLRE_pos      */   ro0609_initAbort,
  /*           RC0673_RLRE_neg      */   ro0609_initAbort,

  /*           RC0674_A_ABRreq      */   ro0626_A_ABRreq,
  /*           RC0675_ABRT          */   ro0627_ABRTorP_PABind,
  /*           RC0676_P_PABind      */   ro0627_ABRTorP_PABind,

  /*           RC0677_EXTRN_1       */   ro0628_EXTERN,
  /*           RC0678_EXTRN_2       */   ro0628_EXTERN,
  /* End of state                   */   },


  /* state:    R_0657_STA6          */   {
  /*    event: RC0661_A_ASCreq      */   ro0607_LocalError,
  /*           RC0662_A_ASCrsp_pos  */   ro0607_LocalError,
  /*           RC0663_A_ASCrsp_neg  */   ro0607_LocalError,
  /*           RC0664_AARQ          */   ro0609_initAbort,
  /*           RC0665_AARE_pos      */   ro0609_initAbort,
  /*           RC0666_AARE_neg      */   ro0609_initAbort,
  /*           RC0667_P_CONcnf_neg  */   ro0609_initAbort,

  /*           RC0668_A_RLSreq      */   ro0607_LocalError,
  /*           RC0669_A_RLSres_pos  */   ro0618_RLSrsp,
  /*           RC0670_A_RLSrsp_neg  */   ro0607_LocalError,
  /*           RC0671_RLRQ          */   ro0609_initAbort,
  /*           RC0672_RLRE_pos      */   ro0609_initAbort,
  /*           RC0673_RLRE_neg      */   ro0609_initAbort,

  /*           RC0674_A_ABRreq      */   ro0626_A_ABRreq,
  /*           RC0675_ABRT          */   ro0627_ABRTorP_PABind,
  /*           RC0676_P_PABind      */   ro0627_ABRTorP_PABind,

  /*           RC0677_EXTRN_1       */   ro0609_initAbort,
  /*           RC0678_EXTRN_2       */   ro0607_LocalError,
  /* End of state                   */   },

  /* state:    RC0658_STA7          */   {
  /*    event: RC0661_A_ASCreq      */   ro0607_LocalError,
  /*           RC0662_A_ASCrsp_pos  */   ro0607_LocalError,
  /*           RC0663_A_ASCrsp_neg  */   ro0607_LocalError,
  /*           RC0664_AARQ          */   ro0609_initAbort,
  /*           RC0665_AARE_pos      */   ro0609_initAbort,
  /*           RC0666_AARE_neg      */   ro0609_initAbort,
  /*           RC0667_P_CONcnf_neg  */   ro0609_initAbort,

  /*           RC0668_A_RLSreq      */   ro0607_LocalError,
  /*           RC0669_A_RLSres_pos  */   ro0607_LocalError,
  /*           RC0670_A_RLSrsp_neg  */   ro0607_LocalError,
  /*           RC0671_RLRQ          */   ro0609_initAbort,
  /*           RC0672_RLRE_pos      */   ro0623_RLRE   ,
  /*           RC0673_RLRE_neg      */   ro0609_initAbort,

  /*           RC0674_A_ABRreq      */   ro0626_A_ABRreq,
  /*           RC0675_ABRT          */   ro0627_ABRTorP_PABind,
  /*           RC0676_P_PABind      */   ro0627_ABRTorP_PABind,

  /*           RC0677_EXTRN_1       */   ro0609_initAbort,
  /*           RC0678_EXTRN_2       */   ro0607_LocalError,
  /* End of state                   */   }
};

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNCTION                                               */
/*                                                                         */
/* NAME         : ro0600_acpm                                              */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 91-01-03                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed   short int PASCAL ro0600_acpm (
        unsigned long  int        cxt)

/*                                                                         */
/* DESCRIPTION  :  This function runs the association control              */
/*                 service element protocol machine as defined in ISO      */
/*                 8650.                                                   */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/*    cxt       :  Pointer to the context buffer (protocol machine         */
/*                 invocation)                                             */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

rT0650_acse          NEAR *cxtPtr;
signed   long  int         pmevnt;
signed   long  int         autevt;

unsigned long  int             p1;
unsigned long  int             p2;
unsigned long  int             p3;
unsigned long  int             p4;

signed   short int    rc;
signed   short int state;
signed   long  int inevt;
unsigned long  int   acb;
unsigned long  int   ulEcb;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs0600)
rc = 0;
state = 0;
/* ****************** Ac-pm invocation *********************************** */
/* ******************                  *********************************** */

if (cxt) { /* there is a context */
    cxtPtr = (rT0650_acse NEAR *) ro0031_locallock (cxt);
    if(! cxtPtr)                                             rc = -R_LLOCKE;
} else {   /* create the first context */
    cxt = ro0030_localalloc (sizeof (rT0650_acse));
    if (! cxt)                                               rc = -R_LALLCE;
      else {
        cxtPtr = (rT0650_acse NEAR *) ro0031_locallock (cxt);
        if (! cxtPtr)                                        rc = -R_LLOCKE;
          else {
            rc = ro0212_regPmInvoc (cxt);
        }
    }
}

if (rc) RM0003_RETURN (rs0600, signed short int, rc);

/* ****************** Pop incomming event and write Logging ************** */
/* *******************                                      ************** */

inevt = pmevnt = ro0210_popInEvent (&p1, &p2, &p3, &p4);

acb   = ro0214_getAcbEcb ((unsigned long int FAR *)&ulEcb);

ro0043_pack10Log (RC0022_ACTIVITY                              ,
                  RL0601_ActivityAcpm                          ,
                  8                                            ,
                  (signed short int) sizeof (unsigned long int),
                  (char FAR *)       &acb                      ,
                  (signed short int) sizeof (*cxtPtr)          ,
                  (char FAR *)       cxtPtr                    ,
                  (signed short int) sizeof (signed long int)  ,
                  (char FAR *)       &pmevnt                   ,
                  (signed short int) sizeof (unsigned long int),
                  (char FAR *) &ulEcb                          ,
                  (signed short int) sizeof (unsigned long int),
                  (char FAR *) &p1                             ,
                  (signed short int) sizeof (unsigned long int),
                  (char FAR *) &p2                             ,
                  (signed short int) sizeof (unsigned long int),
                  (char FAR *) &p3                             ,
                  (signed short int) sizeof (unsigned long int),
                  (char FAR *) &p4                             );
                  

/* ****************** Map service events to automata events ************** */
/* ******************                                       ************** */

switch (inevt)     {
 case RC0601_AA_ASSOCIATEreq: break;
 case RC0603_AA_ASSOCIATErsp: if ( (p4 & 0x000000f0) != RC0620_Accepted)
                                              inevt = RC0663_A_ASCrsp_neg;
                              break;
 case RC0607_AA_RELEASErsp  : if ( p4 != RCro90_Affirmative)
                                              inevt = RC0670_A_RLSrsp_neg;
                                       /* guilty values RCro91_Negative
                                                        RCro90_Affirmative */
                              break;
 case RC0605_AA_RELEASEreq  : break;
 case RC0609_AA_ABORTreq    : break;
 case RC0402_PCONNECTind    : break;
 case RC0404_PCONNECTcnf    : switch ( p4) {
                              case RCro80_Accept:     break;
                              case RCro81_UserReject: inevt = RC0666_AARE_neg;
                                                      break;
                              default:              inevt = RC0667_P_CONcnf_neg;
                                                      break;
                              }
                              break;
 case RC0408_PRELEASEcnf    : if ( p4) inevt = RC0673_RLRE_neg;
                              break;
 case RC0406_PRELEASEind    : break;
 case RC040a_PUABORTind     : break;
 case RC040b_PPABORTind     : break;
 case RC060c_AA_EXTERN_1    : break;
 case RC060d_AA_EXTERN_2    : break;
 default                    : rc = -R_AUNKEV; break;
}

/* ****************** Call state-event-matix function ******************** */
/* ******************                                 ******************** */

if (! rc) {
    autevt = inevt;
    inevt &= 0x00ff;
    rc = (*automata [cxtPtr->sState] [inevt]) (cxtPtr, pmevnt, p1, p2, p3, p4);
}

/* ****************** Ac-pm destruction ********************************** */
/* ******************                   ********************************** */

state = cxtPtr->sState;

if (ro0032_localunlock (cxt)) rc = -R_LULOKE;

if (! state) {
        rc = ro0601_cleanUp  (LNULL, cxt, LNULL, LNULL, LNULL);
        if  (!rc) ro0213_regPmDestr ();
}

RM0003_RETURN (rs0600, signed short int, rc);
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0601_cleanUp                                           */
/*                                                                         */
/* AUTHOR       : Johann Mueller                                           */
/* DATE         : 89-09-30                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short    int PASCAL  ro0601_cleanUp (
       signed   long  int     pmEvent,
       unsigned long  int     p1c,
       unsigned long  int     p2c,
       unsigned long  int     p3c,
       unsigned long  int     p4c)

/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* RETURN VALUES              :                                            */
/*                                                                         */
/*         R_ALSQFL                                                        */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed short int    rc;

unsigned long int hAeTitle;
unsigned long int hAddress;
unsigned long int hCldAdd=NULL;
rT0650_acse NEAR * pp1c;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY  (rs0601)
rc = 0;

if (! pmEvent) {
    /* free also the AARQ handle in the context block before    */
    /* making the context block free                            */
   
    pp1c = (rT0650_acse NEAR *) ro0031_locallock(p1c);
      if (! pp1c) RM0003_RETURN (rs0601, signed short int, -R_LLOCKE)
     /*if (pp1c->hAARQapdu)
    if (ro0024_globalfree (pp1c->hAARQapdu)) 
      RM0003_RETURN (rs0601, signed short int, -R_LFREEE) * by kc on 7/7/ */
    if (ro0032_localunlock (p1c)) rc = -R_LULOKE;
    if (!rc)
        if (ro0034_localfree   (p1c)) rc = -R_LFREEE;

} else {
    switch (pmEvent)    {
    case RC0601_AA_ASSOCIATEreq: {
          rTro7e_stAssocReqAcpm NEAR *pp3_req;

          pp3_req = (rTro7e_stAssocReqAcpm NEAR *) ro0031_locallock(p3c);
          if (! pp3_req) RM0003_RETURN (rs0601, signed short int, -R_LLOCKE)
          hAeTitle = pp3_req->stAcpm.hAeTitle;
          hAddress = pp3_req->stAll.hCalledAddress;
          break;
         }
    case RC0603_AA_ASSOCIATErsp: 
          if ((p3c & RCro85_Redirection) == RCro85_Redirection)
               break;
          else{
          rTro7g_stAssocRspAcpm NEAR *pp3_rsp;

          pp3_rsp = (rTro7g_stAssocRspAcpm NEAR *) ro0031_locallock(p3c);
          if (! pp3_rsp) RM0003_RETURN (rs0601, signed short int, -R_LLOCKE)
          hAeTitle = pp3_rsp->stAcpm.hAeTitle;
          hAddress = pp3_rsp->stAll.hRespondAddress;
          break;
         }
    case RC0402_PCONNECTind    : {
          rTro7f_stAssocIndAcpm NEAR *pp3_ind;

          pp3_ind = (rTro7f_stAssocIndAcpm NEAR *) ro0031_locallock(p3c);
          if (! pp3_ind) RM0003_RETURN (rs0601, signed short int, -R_LLOCKE)
          hAeTitle = pp3_ind->stAcpm.hAeTitle;
          hAddress = pp3_ind->stAll.hCallingAddress;
          hCldAdd  = pp3_ind->stAll.hCalledAddress;
          break;
         }
    case RC0404_PCONNECTcnf    : {
          rTro7h_stAssocCnfAcpm NEAR *pp3_cnf;

          pp3_cnf = (rTro7h_stAssocCnfAcpm NEAR *) ro0031_locallock(p3c);
          if (! pp3_cnf) RM0003_RETURN (rs0601, signed short int, -R_LLOCKE)
          hAeTitle = pp3_cnf->stAcpm.hAeTitle;
          hAddress = pp3_cnf->stAll.hRespondAddress;
          break;
         }
    }

    if ( p3c) {
        /* --------- free the PSAP address ------------------------------- */
        if ( hAddress && ro0034_localfree (hAddress))
                    RM0003_RETURN (rs0601, signed short int, -R_LFREEE)
        if ( hCldAdd && ro0034_localfree (hCldAdd))
                    RM0003_RETURN (rs0601, signed short int, -R_LFREEE)


        /* --------- free the AeTitle ------------------------------------ */
        if ( hAeTitle) {
            rc = ro0009_deleteAETitle ( &(hAeTitle));
            if (! rc) RM0003_RETURN (rs0601, signed short int, rc)
        }

        if (ro0032_localunlock (p3c))
                    RM0003_RETURN (rs0601, signed short int, -R_LULOKE)
        if ( ro0034_localfree (p3c))
                    RM0003_RETURN (rs0601, signed short int, -R_LFREEE)
    }

    switch (pmEvent)    {
    case RC0601_AA_ASSOCIATEreq:
    case RC0603_AA_ASSOCIATErsp:
    case RC0607_AA_RELEASErsp  :
    case RC0605_AA_RELEASEreq  :
    case RC0609_AA_ABORTreq    :
    case RC0402_PCONNECTind    :
    case RC0404_PCONNECTcnf    :
    case RC0406_PRELEASEind    :
    case RC0408_PRELEASEcnf    :
    case RC040a_PUABORTind     :

         if (p1c) rc = ro0246_clearPdvTree (&p1c); break;
    }
}

RM0003_RETURN (rs0601, signed short int, rc)
}/*end*/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro0602_encodAcpm                                            */
/*                                                                            */
/* AUTHOR       : Michael Rippstain                                           */
/* DATE         : 92-01-27                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro0602_encodAcpm ( 
	rTro76_stDefinedBy   FAR  *pDefinedBy,
	rTro01_pfvObjectId         pTrfSynObjId,
	unsigned long int          hsStore,
	void                 NEAR *pPpmContext,
	unsigned long int    FAR  *phAcseApdus,
	unsigned long int    FAR  *pLength,
	signed        int          iAnyNumb)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     pDefinedBy     :  for ACSE  Not used for RTPM (Null pointer).
                         Supposed to be     */
/*                       ANY DEFINED BY encoding information (pointer to).    */
/*     pTrfSynObjId   :  Transfer syntax object identifier (pointer to).      */
/*     hsStore        :  Store handle (encoded byte string).                  */
/*     pPpmContext    :  PPM context (pointer to).                            */
/*     phAcseApdus    :  Memory handle to PDV node (pointer to).              */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/*     pLength        : Total length of all embedded PDV encodings            */
/*                      pointer to).                                          */
/*                      - ANY, OCTET STRING : scale "byte".                   */
/*                      - BIT STRING        : scale "bit".                    */
/*                        (lEncodingChoice from PDV header, see rouply0.h)    */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*       0          : OK.                                                     */
/*    != 0          : Error                                                   */
/*                    (caller returns immediately with the same value,        */
/*                     mapping of the value within the PPM encoding function).*/
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro0602_encodAcpm*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed   short int     sRetCode;
rT0646_acseApdus  FAR *pAcseApdus;
rTro00_vObjectId       vBerTrfSyn = {3, 2, 1, 1};

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (rs0602)

sRetCode = OK;

/*----------------------------------------------------------------------------*/
/* Currently only BER transfer syntax supported. Future additions possible.   */
/* Unsupported transfer syntax is a RTS user error - a non serious error.    */
/* **SKH: pTrfSynObjId is a pointer to vector. Change in interface ?         */
/*----------------------------------------------------------------------------*/
if (ro0003_objcmp (pTrfSynObjId,
                  (rTro01_pfvObjectId)vBerTrfSyn) != OK)
    RM0003_RETURN (rs0602, signed short int, RCro6m_UnexpectedPpduParam)

/*----------------------------------------------------------------------------*/
/* Process the RTPM PDV node.                                                 */
/*----------------------------------------------------------------------------*/
pAcseApdus = (rT0646_acseApdus FAR *)(void FAR *) 
                            ro0021_globallock(*phAcseApdus);
if (pAcseApdus == (rT0646_acseApdus FAR *)NULL)
    RM0003_RETURN (rs0602, signed short int, -R_GLOCKE)

/*----------------------------------------------------------------------------*/
/* Note: The store is created before the first call to encoding by PPM.       */
/*       After the entire decoding process, when it finally returns back      */
/*       to the PPM, it does a storeidle. Finally control of the store is     */
/*       passed to LOIF.                                                      */
/*                                                                            */
/* Note: Positive encoder return codes are non-serious errors for ACPM.       */
/*----------------------------------------------------------------------------*/
sRetCode = ACSE_apdu_cod (hsStore, -1, 0, pAcseApdus, pPpmContext);
if (sRetCode < 0)
    RM0003_RETURN (rs0602, signed short int, sRetCode)

if (ro0022_globalunlock (*phAcseApdus) != OK)
    RM0003_RETURN (rs0602, signed short int, -R_GULOKE)

/*----------------------------------------------------------------------------*/
/* Note: clearPdvTree() is called from Presentation at the end of encoding !  */
/* **SKH: Hubert ! Provide pLength back as output parameter. How ?            */
/*----------------------------------------------------------------------------*/

*pLength = 0;

RM0003_RETURN (rs0602, signed short int, sRetCode)

}/*end-ro0602_encodAcpm*/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro0603_decodAcpm                                            */
/*                                                                            */
/* AUTHOR       : Michael Rippstain                                           */
/* DATE         : 92-01-27                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */


signed short int PASCAL ro0603_decodAcpm ( 
	signed    short int       sAbstrSynId,
	signed   long  int        lEncodingChoice,
	rTro76_stDefinedBy FAR  * pDefinedBy,
	rTro01_pfvObjectId          pTrfSynObjId,
	unsigned long  int        hsStore,
	void               NEAR * pPpmContext,
	unsigned long  int FAR  * phAcseApdus,
	unsigned long  int        ulLength,
	signed         int        iAnyNumb)

/* INPUT  PARAMETERS                                                          */
/*                                                                            */
/*     sAbstrSynId    :  Abstract syntax identifier (describes the named      */
/*                       abstract syntax the node belongs to).                */
/*                       Has to be written into the PDV header                */
/*                       (is not necessary for the decoding function itself). */
/*                                                                            */
/*     lEncodingChoice:  Encoding choice value of the PDV node:               */
/*                        - ANY                                               */
/*                        - OCTET STRING                                      */
/*                        - BIT STRING.                                       */
/*                       Has to be written into the PDV header                */
/*                       (is not necessary for the decoding function itself). */
/*                       For values and further comments see rouply0.h.       */
/*                                                                            */
/*     pDefinedBy     :  with ACSE Not used for RTPM (NULL pointer). Supposed to contain*/
/*                       ANY DEFINED BY decoding information (pointer to).    */
/*                                                                            */
/*     pTrfSynObjId   :  Transfer syntax object identifier (pointer to).      */
/*                                                                            */
/*     hsStore        :  Store handle (decoded byte string).                  */
/*                                                                            */
/*     pPpmContext    :  PPM context (pointer to).                            */
/*                                                                            */
/*     ulLength       : Total length of all embedded PDV decodings.           */
/*                       - ANY, OCTET STRING : scale "byte".                  */
/*                       - BIT STRING        : scale "bit".                   */
/*                      Note: ANY length only if within PDV-list or EXTERNAL  */
/*                            and if definite length decoding (otherwise 0).  */
/*                                                                            */
/* OUTPUT PARAMETERS                                                          */
/*                                                                            */
/*     phAcseApdus    : Memory handle to PDV node (pointer to), allocated     */
/*                      within the function.                                  */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*       0          : OK.                                                     */
/*    != 0          : Error                                                   */
/*                    (caller returns immediately with the same value,        */
/*                     mapping of the value within the PPM decoding           */
/*                     function).                                             */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro0603_decodAcpm*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed   short int     sRetCode;
rT0646_acseApdus  FAR *pAcseApdus;
rTro00_vObjectId       vBerTrfSyn = {3, 2, 1, 1};


/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (rs0603)

sRetCode = OK;

/*----------------------------------------------------------------------------*/
/* Currently only BER transfer syntax supported. Future additions possible.   */
/* Unsupported transfer syntax is a peer RTPM error i.e. a non-serious error. */
/*                                                                           */
/* Note: Before the whole decoding process, the PPM does a storeactive (on   */
/*       the store that was storeidled by LOIF). The whole decoding process  */
/*       then proceeds and finally the store is deleted.                     */
/*       Note: The store associated with the RTTR apdu must not be deleted.  */
/*             Therefore, the MAVROS decoder does a stlink on the store.     */
/*                                                                           */
/* **SKH: pTrfSynObjId is a pointer to vector. Change in interface ?          */
/*----------------------------------------------------------------------------*/
if (ro0003_objcmp (pTrfSynObjId,
                  (rTro01_pfvObjectId)vBerTrfSyn) != OK)
    RM0003_RETURN (rs0603, signed short int, RCro6m_UnexpectedPpduParam)

/*----------------------------------------------------------------------------*/
/* Allocate and lock the PDV node.                                            */
/*----------------------------------------------------------------------------*/
*phAcseApdus = ro0020_globalalloc (sizeof (rT0646_acseApdus));
if (*phAcseApdus == NULL)
    RM0003_RETURN (rs0603, signed short int, -R_GALLCE)

pAcseApdus = (rT0646_acseApdus FAR *) (void FAR *) 
                                   ro0021_globallock (*phAcseApdus);
if (pAcseApdus == (rT0646_acseApdus FAR *)NULL)
    RM0003_RETURN (rs0603, signed short int, -R_GLOCKE)

/*----------------------------------------------------------------------------*/
/* Initialize the parameters for the decoding function.                       */
/* Note: pAcseApdus->stPdvHeader.lType would be initialized in the decoding   */
/*       function of MAVROS; so need not be done here !                       */
/*----------------------------------------------------------------------------*/
pAcseApdus->stPdvHeader.sPmNumb         = RC0216_Acse;
pAcseApdus->stPdvHeader.sAbstrSynId     = sAbstrSynId;
pAcseApdus->stPdvHeader.lEncodingChoice = lEncodingChoice;

/*----------------------------------------------------------------------------*/
/* The actual decoding in this function here.                                 */
/* **SKH: store error detecteds in RTSE_apdus_dec.                            */
/*----------------------------------------------------------------------------*/
if (ulLength == 0)
    ulLength = ro0062_storelength (hsStore);
else
    ulLength = ulLength + ro0076_stwhere(hsStore);

sRetCode = ACSE_apdu_dec (hsStore, ulLength, pAcseApdus, pPpmContext);
if (sRetCode < 0)
    RM0003_RETURN (rs0603, signed short int, sRetCode)

/*----------------------------------------------------------------------------*/
/* Positive decoder return codes are non-serious errors. They should be       */
/* properly handled and not immediately returned.                             */
/*----------------------------------------------------------------------------*/
if (ro0022_globalunlock (*phAcseApdus) != OK)
    RM0003_RETURN (rs0603, signed short int, -R_GULOKE)

RM0003_RETURN (rs0603, signed short int, sRetCode)
}/*end-ro0603_decodAcpm*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNCTION                                               */
/*                                                                         */
/* NAME         : ro0604_clearNodeAcpm                                     */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-01-27                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */
/*                                                                         */
signed short int PASCAL ro0604_clearNodeAcpm (
	unsigned long  int FAR  *phAcseApdus)

/*                                                                         */
/* INPUT  PARAMETERS                                                       */
/*                                                                         */
/*     phAcseApdus    : Memory handle to PDV node (pointer to).            */
/*                                                                         */
/* OUTPUT PARAMETERS                                                       */
/*                                                                         */
/*     phAcseApdus    : Memory handle to PDV node (pointer to).            */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/*       0            : OK.                                                */
/*    != 0            : Unrecoverable error (see roerval.h).               */
/*                                                                         */
/* GLOBAL DATA (read only) : None                                          */
/*                                                                         */
/* GLOBAL DATA (modified)  :                                               */
/*                                                                         */
/* REMARKS :  none                                                         */
/*                                                                         */
/*inoff ********************************************************************/

{/*entr-ro0604_clearNodeAcpm*/

/* *********************** l o c a l    v a r i a b l e s **************** */
/* ***********************                                **************** */

signed   short int     sRetCode;
unsigned short int     i;
rT0646_acseApdus  FAR *pAcseApdus;
signed   short int     sErrCount;    /* Error counter. Dont return inspite */
                                     /* of errors, but increment sErrCount.*/
                                     /* iErrCount > 0 on return means      */
                                     /* errors on execution.               */

/* **************************** S T A R T ******************************** */
/* ****************************           ******************************** */

RM0002_ENTRY (rs0604)

/*-------------------------------------------------------------------------*/
/* Note1: The function is called only if a PDV node exists (checked by     */
/*        ro0246_clearPdvTree).                                            */
/*                                                                         */
/* Note2: No attempt at localunlock (PDVnode). It must be already unlocked */
/*        prior to a call to this function.                                */
/*                                                                         */
/* Note3: The function continues in the case of an error.                  */
/*        Is this behaviour OK                                             */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* Initialization.                                                         */
/*-------------------------------------------------------------------------*/

sRetCode = OK;
sErrCount = 0;

if (phAcseApdus) {
    pAcseApdus = (rT0646_acseApdus FAR *) (void FAR *) 
                                 ro0021_globallock (*phAcseApdus);
}

if ((!phAcseApdus) || (pAcseApdus == (rT0646_acseApdus FAR *)NULL)) {
    sErrCount++;
}
else {
    /*---------------------------------------------------------------------*/
    /* Select PDU type and clear user data, if any.                        */
    /*---------------------------------------------------------------------*/

    switch (pAcseApdus->stPdvHeader.lType) {

     case RC0665_AARQapdu :
        if (pAcseApdus->aarqApdu.bUserInfo != 0 ) {
          for ( i=0; i<pAcseApdus->aarqApdu.ulUserInfo; i++)
              if (ro0246_clearPdvTree (
                       &(pAcseApdus->aarqApdu.vUserInfo[i]) ) != OK)
                  sErrCount++;
        }
        break;

     case RC0666_AAREapdu :
        if (pAcseApdus->aareApdu.bUserInfo != 0 ) {
          for ( i=0; i<pAcseApdus->aareApdu.ulUserInfo; i++)
              if (ro0246_clearPdvTree (
                       &(pAcseApdus->aareApdu.vUserInfo[i]) ) != OK)
                  sErrCount++;
        }
        break;


     case RC0667_RLRQapdu :
        if (pAcseApdus->rlrqApdu.bUserInfo != 0 ) {
          for ( i=0; i<pAcseApdus->rlrqApdu.ulUserInfo; i++)
              if (ro0246_clearPdvTree (
                       &(pAcseApdus->rlrqApdu.vUserInfo[i]) ) != OK)
                  sErrCount++;
        }
        break;

     case RC0668_RLREapdu :
        if (pAcseApdus->rlreApdu.bUserInfo != 0 ) {
          for ( i=0; i<pAcseApdus->rlreApdu.ulUserInfo; i++)
              if (ro0246_clearPdvTree (
                       &(pAcseApdus->rlreApdu.vUserInfo[i]) ) != OK)
                  sErrCount++;
        }
        break;

     case RC0669_ABRTapdu :
        if (pAcseApdus->abrtApdu.bUserInfo != 0 ) {
          for ( i=0; i<pAcseApdus->abrtApdu.ulUserInfo; i++)
              if (ro0246_clearPdvTree (
                       &(pAcseApdus->abrtApdu.vUserInfo[i]) ) != OK)
                  sErrCount++;
        }
        break;

    default : sErrCount++; break;
    }/*switch-end*/
}/*else-end*/

/*-------------------------------------------------------------------------*/
/* Return back formalities here.                                           */
/*-------------------------------------------------------------------------*/

if (ro0022_globalunlock (*phAcseApdus) != OK)
    sErrCount++;

if (ro0024_globalfree (*phAcseApdus) != OK)
    sErrCount++;
else
    *phAcseApdus = 0;

if (sErrCount != 0) sRetCode = -R_FCLEAR;
else                sRetCode = OK;

RM0003_RETURN (rs0604, signed short int, sRetCode)

}/*end-ro0604_clearNodeAcpm*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro060a_informAcpm                                   */ 
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed  short int FAR PASCAL ro060a_informAcpm (
	unsigned long  int      hContext,              
	signed   short int      sInformType,          
	void               FAR *pRequestedInfo)
                   
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
signed   short int  rc = OK;

rT06z0_stInfrmApplCxtName FAR  *pstInfo;


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs060a)
 pstInfo = (rT06z0_stInfrmApplCxtName FAR * )   pRequestedInfo;
 switch (sInformType){
     case   RC06z1_ApplCxtName:
         /* --Search for the OBJID that matches the ID -- */
         rc = ro0650_copyAplCxtName(
                                    pstInfo->sApplCxtId  , 
                                    (rTro01_pfvObjectId)pstInfo->vApplCxtName);
 
         break;
     case   RC06z2_ApplCxtID:
         /* -- Search for the ID that matches the OBJID -- */
         rc = ro0651_cmpAndRetrieveAplCxtName (
                           (signed short  int FAR *)&pstInfo->sApplCxtId  ,
                           (rTro01_pfvObjectId     ) pstInfo->vApplCxtName);
          break;
     case RC06z4_CheckLegal:
          rc = ro060d_CheckLegal(hContext, pRequestedInfo );
         break;

 }/*end of switch*/

RM0003_RETURN (rs060a, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro060b_encodSniPAcpm                                        */
/*                                                                            */
/* AUTHOR       : Vinita                                                      */
/* DATE         : 93-04-08                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro060b_encodSniPAcpm ( 
	unsigned long int          hStore,
	void                NEAR  *pPpmContext,
	unsigned long int   FAR   *phAcseApdus)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     hStore         :  Buffer holding encoded data string (pointer to).     */
/*     pPpmContext    :  PPM context (pointer to).                            */
/*     phAcseApdus    :  Memory handle to PDV node (pointer to).              */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*       0          : OK.                                                     */
/*    != 0          : Error                                                   */
/*                    (caller returns immediately with the same value,        */
/*                     mapping of the value within the PPM encoding function).*/
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro060b_encodSniPAcpm*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed   short int     sRetCode=0;
rT0646_acseApdus  FAR  *pAcseApdus;
signed   long  int     lType;
unsigned long  int  PdvNode=0;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (rs060b)

pAcseApdus = (rT0646_acseApdus FAR *) ro0021_globallock (*phAcseApdus);

lType = pAcseApdus->stPdvHeader.lType;  

 /* pOrigBuffer=pSendBuffer;*/ /* preserve original position of the buffer */

switch (lType) {

   case RC0665_AARQapdu:
     sRetCode = AARQSniP_type_cod(hStore,&(pAcseApdus->aarqApdu),pPpmContext); 
     PdvNode = pAcseApdus->aarqApdu.vUserInfo[0];
     break;
   case RC0666_AAREapdu:
     sRetCode = AARESniP_type_cod(hStore,&(pAcseApdus->aareApdu),pPpmContext); 
     PdvNode = pAcseApdus->aareApdu.vUserInfo[0];
     break;
   case RC0667_RLRQapdu:
     sRetCode = RLRQSniP_type_cod(hStore,&(pAcseApdus->rlrqApdu),pPpmContext); 
     PdvNode = pAcseApdus->rlrqApdu.vUserInfo[0];
     break;
   case RC0668_RLREapdu:
     sRetCode = RLRESniP_type_cod(hStore,&(pAcseApdus->rlreApdu),pPpmContext); 
     PdvNode = pAcseApdus->rlreApdu.vUserInfo[0];
     break;
   case RC0669_ABRTapdu:
     sRetCode = ABRTSniP_type_cod(hStore,&(pAcseApdus->abrtApdu),pPpmContext); 
     PdvNode = pAcseApdus->abrtApdu.vUserInfo[0];
     break; 

  }  /* end switch lType  */
    
     /* find length of the data encoded  so far */
/*       ulLength = (pSendBuffer - pOrigBuffer);  */

     /* get store address to copy encoded string to store */
 /*     if ((pStoreAddress = ro0069_stputs (*pStore, ulLength))  == NULL)      
        RM0003_RETURN (fnctName, signed short int, -R_STOERR)
 */
     /* copy encoded data string  to  store */
  /*    ro0041_memcpy (pStoreAddress, pOrigBuffer, ulLength);  */

     /* position store pointer to new position */
   /*  sRetCode = ro0066_storepos(*pStore,ulLength);   */

     /* call despatcher to switch encoding */ 
     if (PdvNode){
        rT0411_stExternal       stLocalExtern; 
        signed short  int      sPmNumb;        
        unsigned long int FAR *pTransSynName;  

 /*    sRetCode = ro0067_stputc (hStore , 0x01);  *//* boolen true for Udata */ 
       /* get the Presentation Context Id */
      sRetCode = ro0412_encodeExternal(
                        (rT0411_stExternal FAR *)&stLocalExtern,
                        (unsigned long int FAR * FAR *)&pTransSynName,
                        (signed short  int FAR *) &sPmNumb , 
                        pPpmContext,
                        PdvNode); 
      if(sRetCode == OK) 
         sRetCode = ro0067_stputc (hStore, (unsigned char)stLocalExtern.lIndirectRef);
      if(sRetCode == OK) 
          sRetCode = ro0242_encodSniP (RC0221_RoseUser, hStore, 
                      pPpmContext,&PdvNode); 
     }

    if (ro0022_globalunlock (*phAcseApdus) != OK)
         sRetCode  = -R_GLOCKE; 
    RM0003_RETURN (rs0602, signed short int, sRetCode)

}/*end-ro060b_encodSniPAcpm*/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro060c_decodSniPAcpm                                        */
/*                                                                            */
/* AUTHOR       : Vinita                                                      */
/* DATE         : 93-04-07                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */


signed short int PASCAL ro060c_decodSniPAcpm ( 
signed   short int   FAR  * psAbsId,
unsigned long int           pStore,
void                 NEAR * pPpmContext,
unsigned long int    FAR  * hAcseApdus)

{/*entr-ro060c_decodSniPAcpm*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed   short int     sRetCode = OK;
rT0646_acseApdus   FAR *pAcseApdus;
unsigned long  int FAR *PdvNode = LNULL;
signed   long  int  i=0;
/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (rs060c)

/* **** get store address to copy encoded string to store **** */
*hAcseApdus = ro0020_globalalloc (sizeof (rT0646_acseApdus));
if (*hAcseApdus)  {
    pAcseApdus = (rT0646_acseApdus FAR *) ro0021_globallock (*hAcseApdus);
    if (pAcseApdus == (rT0646_acseApdus FAR *)NULL)
        sRetCode  = -R_GLOCKE;
    else {
    /* ************* decode ACSE Mask Information **************** */ 
                                                                
        i = ro0070_stgetc( pStore);

        /* Whether check is required for 0x 0a  as Acse Msk !!!*/
        pAcseApdus->stPdvHeader.sPmNumb = RC0216_Acse;  

    /* ************* decode Mask Information ********************* */    
        pAcseApdus->stPdvHeader.lType = (signed long int)
                                            ro0070_stgetc (pStore);
                         

        switch (pAcseApdus->stPdvHeader.lType) {

           case RC0665_AARQapdu:
                sRetCode = AARQSniP_type_dec(pStore,
                      &(pAcseApdus->aarqApdu),pPpmContext); 
                if (pAcseApdus->aarqApdu.bUserInfo)  {
                  PdvNode =
                    (unsigned long int FAR *)&pAcseApdus->aarqApdu.vUserInfo[0];
                }
           break;

           case RC0666_AAREapdu:
                sRetCode = AARESniP_type_dec(pStore,
                           &(pAcseApdus->aareApdu),pPpmContext); 
                if (pAcseApdus->aareApdu.bUserInfo) {
                  PdvNode =
                    (unsigned long int FAR *)&pAcseApdus->aareApdu.vUserInfo[0];
                }
           break;

           case RC0667_RLRQapdu:
                sRetCode = RLRQSniP_type_dec(pStore,
                           &(pAcseApdus->rlrqApdu),pPpmContext); 
                if (pAcseApdus->rlrqApdu.bUserInfo) {
                  PdvNode =
                    (unsigned long int FAR *)&pAcseApdus->rlrqApdu.vUserInfo[0];
                }
           break;

           case RC0668_RLREapdu:
                sRetCode = RLRESniP_type_dec(pStore,
                           &(pAcseApdus->rlreApdu),pPpmContext); 
                if (pAcseApdus->rlreApdu.bUserInfo) {
                  PdvNode =
                    (unsigned long int FAR *)&pAcseApdus->rlreApdu.vUserInfo[0];
                }
           break;

           case RC0669_ABRTapdu:
                sRetCode = ABRTSniP_type_dec(pStore,
                           &(pAcseApdus->abrtApdu),pPpmContext); 
                if (pAcseApdus->abrtApdu.bUserInfo) {
                  PdvNode =
                    (unsigned long int FAR *)&pAcseApdus->abrtApdu.vUserInfo[0];
                }
           break; 
           default:
              sRetCode = RCro6l_UnrecognizedPpduParam;

        }  /* end switch lType  */
    

        /* call despatcher to switch encoding */ 
        if (PdvNode) {
             rTro00_vObjectId  TransSynName;  
             signed short int  sPmNumb;       
             rT0410_stPdvList  stEncodingInfo;
 
            i = ro0070_stgetc (pStore); /* to get the ConId */  
            stEncodingInfo.lPresContId=(signed long int)i;   
            stEncodingInfo.oTransSynName [0] = 0;                     
            sRetCode = ro0416_decodePdvList (                               
      (rTro01_pfvObjectId       FAR *) &TransSynName,     
      (signed short int         FAR *) psAbsId,           
      (signed short int         FAR *) &sPmNumb,          
      (rT0410_stPdvList         FAR *) &stEncodingInfo,   
      (void                         *) pPpmContext );        
           if(!sRetCode)                                                
            sRetCode = ro0243_decodSniP (RC0221_RoseUser, pStore, 
                                         pPpmContext,     PdvNode,psAbsId); 
        }
  
        if (ro0022_globalunlock (*hAcseApdus) != OK)
            sRetCode  = -R_GLOCKE; 

    }/*else-end: pAcseApdus == NULL */

} else sRetCode = -R_GALLCE;

RM0003_RETURN (rs060c, signed short int, sRetCode)

}/*end-ro060c_decodSniPAcpm*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro060a_informAcpm                                   */ 
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed  short int NEAR PASCAL ro060d_CheckLegal (
	unsigned long  int            hContext,              
	void                      FAR *pInfo)
                   
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
signed   short int  rc            = OK;
rT0650_acse        NEAR * cxtPtr  = NULL;
rT07z0_stPossibilityInfo FAR * pstChkLegal = NULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs060d)
    if (hContext) {
       if ( (cxtPtr = (rT0650_acse NEAR *) ro0031_locallock (hContext) ) 
                                                                     != NULL){
         pstChkLegal = (rT07z0_stPossibilityInfo FAR *)pInfo;

/* ** skh: 19/7/93
         if ( (pstChkLegal->lEvent == RC0605_AA_RELEASEreq) &&      
              (cxtPtr->sAssocInitFlag == 0                )   )     
               pstChkLegal->bPossible = FALSE;
*/
         pstChkLegal->lEvent &= 0xff;
         if ( (pstChkLegal->lEvent >  RC0660_MaxInEvents)             ||
              (automata [cxtPtr->sState] [pstChkLegal->lEvent] ==
                                                  ro0607_LocalError ) ||
              (automata [cxtPtr->sState] [pstChkLegal->lEvent] ==
                                                  ro0609_initAbort  ) )
               pstChkLegal->bPossible = FALSE;
       if (ro0032_localunlock (hContext)) rc = -R_LULOKE;
       }else rc = -R_LLOCKE;

    }

RM0003_RETURN (rs060d, signed short int, rc)
}/*end*/
