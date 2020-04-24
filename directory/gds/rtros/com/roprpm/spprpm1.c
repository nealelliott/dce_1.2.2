/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spprpm1.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:43  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:08:53  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:31:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:17:47  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:47  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:31  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:53:17  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:33:51  keutel
 * 	creation
 * 	[1994/03/21  13:22:25  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spprpm1.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:43 $" } ;
#endif

/**********************************************************************/
/*                                                                    */
/*    COPYRIGHT  (C)						      */
/*    Siemens Nixdorf Informationssysteme AG 1993		      */
/*    All rights reserved					      */
/*								      */
/*    This software is furnished under licence and may be used only   */
/*    in accordance with the terms of that licence and with the       */
/*    inclusion of the above copyright notice.			      */
/*    This software may not be provided or otherwise made available   */
/*    to, or used by, any other person. No title to or ownership of   */
/*    the software is hereby transferred.			      */
/*                                                                    */
/**********************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME 	: spprpm1.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K                                      */
/* DATE 	: 24-11-93					      */
/*                                                                    */
/* COMPONENT    : RTROS Evaluation Program                            */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : In this module the the PPM functions for decoding   */
/*                are copied                                          */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.00   K00| 24-11-93| original		       | kc |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spprpm1.c 1.1 93/12/20   RTS-V2.00";

/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>	  /* General RTS definitons		      */
#include <rolgval.h>	  /* General RTS definitons		      */
#include <roerval.h>	  /* General RTS definitons		      */
#include <rouply0.h>
#include <rouply1.h>      /* PM service interface structures          */
#include <routil0.h>
#include <routil1.h>
#include <rostor0.h>      /* Store interface                          */ 
#include <rodpat0.h>      /* Dispatcher interface                     */
#include <rodpat1.h>      /* Dispatcher en/de-coding interface        */
#include <rospm00.h>
#include <rospmz0.h>      /* SPM Inform function defs & structs       */
#include <roppm00.h>
#include <roppm01.h>
#include <roppm02.h>
#include <roppm04.h>
#include <roppm05.h>      /* PPM                                      */ 
#include <roppm03.h>
#include <roppmz0.h>      /* PPM intern: MAVROS     definitions       */  
#include <roppm10.h>      /* PPM intern: macros                       */
#include <roppm11.h>      /* PPM intern: types and constants          */  
#include <roppm12.h>      /* PPM intern: Finction defs                */  
#include <roppmzx.h>      /* PPM intern: MAVROS     definitions       */  
#include <roppmzy.h>      /* PPM intern: SniPrivate definitions       */  
#include <roppmzw.h>      /* PPM intern: SniPrivate prototyping       */  

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spprpm0.h>	  /* Function prototyping		      */

/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/
static unsigned long int rv0410_vBerObjId [] = {3, 2, 1, 1};

/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0431_decodPpm                                     */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-25                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0431_decodPpm (
	unsigned       int        uInEvent,
	rT0430_stContext   NEAR * pContext,
	unsigned long  int FAR  * pStore,
	unsigned long  int FAR  * phNode,
	unsigned long  int        ulOffset)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0431_decodPpm")
signed   long  int  lRtseApduType;   /*RTSE APDU type (X410-mode) */
signed   long  int  lStoreLength;    /*Store length (max. length of the byte string) */
signed   long  int  lStoreWhere;     /*actual Store position */
rT041G_unPpdu  FAR *pNode;           /*pointer to PDV node */
signed   short int  rc;              /*return code of a called function */
signed   short int  rc1;             /*return code of a called function */
/*
char                    FAR *pStoreAddress;
rT08z0_ulStoreOffsetLen      info;
*/
unsigned long  int  FAR *PdvNode = LNULL;
signed   short int  route=0;  
signed   short int  sAbsId=0;  
signed   short int  sPresConId=0;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------------------------------------------------------*/
  /* Return if no Store exists.                                       */
  /* If no Store (i. e. encoded PPDU) exists ro0431_decodPpm returns  */
  /* immediately. If PS-user data (S-DATA indication) or PPCI (CP,    */
  /* CPA, CPR, ARU, ARP) are mandatory the return code will cause a   */
  /* provider abort. S-TOKEN-PLEASE indication: SS-user data are      */
  /* mandatory only in X410-mode (RTSE APCI).                         */
  /* (S-DATA indication: better hard error because SS-user data are   */
  /* mandatory P!? )                                                  */
  /*------------------------------------------------------------------*/

  if (!*pStore)
  {
    switch (uInEvent)
    {
      case RC080f_STOKEN_PLEASEind:
        /*SS-user data are only mandatory in X410-mode (RTSE APCI) */
        if (pContext->bNormalMode)
          break;
      case RC080d_SDATAind:
      case RC0802_SCONNECTind:
      case RC0439_IEvCPA:
      case RC043b_IEvCPR:
      case RC080a_SUABORTind:
        RM0003_RETURN (fnctName, signed short int, RC041l_UnrecognizedPpdu)
      default:
        break;
    } /*to "switch (uInEvent)" */

    RM0003_RETURN (fnctName, signed short int, OK)
  }


  /*------------------------------------------------------------------*/
  /* Activate the Store.                                              */
  /*------------------------------------------------------------------*/

  /* "hsStore = *pStore;"  because of efficiency P!? */
/*  info = uInEvent; */
/*
  rc = ro0231_getInform (ro0214_getAcbEcb (NULL), 
                         RC0214_Session, 
                         RC08z1_InformStoreOffset, &info);
  ulOffset = info; */

  rc = ro0074_storeactive (
    *pStore,              /* --> Store handle.                                   */
    ulOffset);            /* --> Store position from which the buffer is filled. */

  if (rc)
    RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

  /*------------------------------------------------------------------*/
  /* Compute the Store length.                                        */
  /* Used as input parameter of MAVROS functions and for looking if   */
  /* the whole Store was decoded.                                     */
  /*------------------------------------------------------------------*/
  rc =  ro0066_storepos (*pStore , ulOffset);

  lStoreLength = ro0062_storelength (*pStore);
  if (lStoreLength < 0)
    RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

/* kc : check for  the Sni PTS and set the bNormalMode on 26.5  */
  if (uInEvent == RC0802_SCONNECTind)
      rc  = ro04y0_CheckSniPTS( *pStore , pContext);

  /*------------------------------------------------------------------*/
  /* Process special events for X410-mode.                            */
  /* Call the RTPM X410-mode decoding function.                       */
  /*------------------------------------------------------------------*/

  if (!pContext->bNormalMode                 /*(if X410-mode and */
  &&  (  uInEvent == RC080d_SDATAind         /* (if S-DATA request or */
      || uInEvent == RC080f_STOKEN_PLEASEind /*  if S-TOKEN-PLEASE request)) */
      )
  )
  {
    rc = ro0245_decodX410 (
      RC0218_Rtse,           /* --> Rtse Protocol machine No.               */
      uInEvent,              /* --> Used to  determine RT APDU type in Cf Fn*/
      lRtseApduType,         /* --> Not used                                */
     *pStore,                /* --> Store handle (encoded byte string).     */
      (void              NEAR *)pContext,/* --> PPM context (pointer to).               */
      (unsigned long  int FAR *)phNode);/* <-> Memory handle to PDV node (pointer to). */
  }


  /*------------------------------------------------------------------*/
  /* Process a pres. PDV node (normal-mode or X410-mode).             */
  /*------------------------------------------------------------------*/

  else
  {
    /*----------------------------------------------------------------*/
    /* Alloc, lock and initialize a PDV node                          */
    /*----------------------------------------------------------------*/

    RM0420_GlobAlloc (*phNode, sizeof (rT041G_unPpdu))
      /*later: maybe PDU specific size !! (P!?) */
    RM0421_GlobLock (*phNode, pNode, rT041G_unPpdu)

    pNode->stPdvHeader.sPmNumb = RC0215_Presentation;
    /* Note: sAbstrSynId, lEncodingChoice are not relevant */

    /*----------------------------------------------------------------*/
    /* Call a MAVROS created function.                                */
    /* Before: Check the beginning tag of the PPDU (will be done once */
    /* more in the MAVROS created function except of CP PPDU and CPA  */
    /* PPDU; but checking here has the advantage that the appropriate */
    /* abort reason "Unrecognized PPDU" is set).                      */
    /*----------------------------------------------------------------*/
    if (pContext->bNormalMode == -1) /* for Sni Private mode  */
    {
     /* get store address to copy encoded string to store */            
    /*                                                                    
      if ((pStoreAddress = ro0069_stputs (*pStore, 
                                           sizeof (rT041G_unPpdu)))  == NULL) 
        RM0003_RETURN (fnctName, signed short int, -R_STOERR)           
       by kc on 16/5 */                                                                   
     switch (uInEvent)
     {
      case RC0802_SCONNECTind:
          rc = CPSniP_type_dec (
             *pStore,                  /*-->Store Address encoded byte string)*/
            (rT041A_stCp FAR *) pNode, /*-->PDV node (pointer to).            */
            pContext);                 /*-->PPM context (pointer to).         */
        if ( pNode->stCp.bUserData == TRUE) {
            PdvNode = (unsigned long int FAR *)
                         &pNode->stCp.stUserData.unUserData.stFully.vPdvList[0];
        }
        route = RC0216_Acse;                                            

        break;

      case RC0439_IEvCPA:
          rc = CPASniP_type_dec (
            *pStore,            /*-->Store Address encoded byte string)*/
            (rT041B_stCpa FAR *) pNode,/*-->PDV node (pointer to).            */
            pContext);                 /*-->PPM context (pointer to).         */

        if ( pNode->stCpa.bUserData == TRUE) {
            PdvNode = (unsigned long int FAR *)
                      &pNode->stCpa.stUserData.unUserData.stFully.vPdvList[0];  
        }
        route = RC0216_Acse;

        break;

      case RC043b_IEvCPR:
          rc = CPRSniP_type_dec (
            *pStore,            /*-->Store Address encoded byte string)*/
            (rT041C_stCpr FAR *) pNode,/*-->PDV node (pointer to).            */
            pContext);                 /*-->PPM context (pointer to).         */

        if ( pNode->stCpr.bUserData == TRUE) {
           PdvNode = (unsigned long int FAR *)
                     &pNode->stCpr.stUserData.unUserData.stFully.vPdvList[0]; 
        }
        route = RC0216_Acse;

        break;

      case RC080a_SUABORTind:
          rc = ABUSniP_type_dec (
            *pStore,            /*-->Store Address encoded byte string)*/
            (rT041D_stAru FAR *) pNode,/*-->PDV node (pointer to).            */
            pContext);                 /*-->PPM context (pointer to).         */

        if ( pNode->stAru.bUserData == TRUE) {
	   PdvNode = (unsigned long int FAR *)
	             &pNode->stAru.stUserData.unUserData.stFully.vPdvList[0];
        }
	route = RC0216_Acse;

        break;

      case RC043l_IEvARP:
          rc = ABPSniP_type_dec (
            *pStore,            /*-->Store Address encoded byte string)*/
            (rT041E_stArp FAR *) pNode,/*-->PDV node (pointer to).            */
            pContext);                 /*-->PPM context (pointer to).         */
        break;

      default:
          rc = UserSniP_type_dec (
            *pStore,            /*-->Store Address encoded byte string)*/
            (rT041F_stOther FAR *) pNode,/*-->PDV node (pointer to).          */
            pContext);                 /*-->PPM context (pointer to).         */
            route = ro0070_stgetc(*pStore);
            
            sPresConId = ro0070_stgetc(*pStore); /* to get the BER or Octet mask*/
            {
             rTro00_vObjectId  TransSynName;
             signed short int  sPmNumb;
             rT0410_stPdvList  stEncodingInfo;
           
             stEncodingInfo.lPresContId=(signed long int)sPresConId; 
             stEncodingInfo.oTransSynName [0] = 0;
             
             rc = ro0416_decodePdvList ( 
                   (rTro01_pfvObjectId       FAR *) &TransSynName,
                   (signed short int         FAR *) &sAbsId,
                   (signed short int         FAR *) &sPmNumb,
                   (rT0410_stPdvList         FAR *) &stEncodingInfo,
                   (rT0430_stContext             *) pContext );       

            } 

          PdvNode = (unsigned long int FAR *)
                    &pNode->stOther.stUserData.unUserData.stFully.vPdvList[0];
          if (!rc) { /* ** SKH on 19/6 */
              /* to get the route get the byte from store        */
              route = ro0077_stgetcp(*pStore);
              rc = ro04y1_routeSnixAutoNo ((signed short int FAR *)&route);
          }
        break;
     } /* end switch uInEvent */  
     if (!rc) { /* **SKH on 19/6 */
       if ( (route)  && ( PdvNode)  )
          rc = ro0243_decodSniP (route, *pStore, (void NEAR *)pContext,
                                 (unsigned long  int FAR *)PdvNode,
                                 (signed short int FAR *)&sAbsId);
     }
    } /* Sni Private Mode decoding */
    else {
    switch (uInEvent)
    {
      case RC0802_SCONNECTind:
        pContext->lType          =            /*for use in E/D Control functions         */
        pNode->stPdvHeader.lType = RC041A_Cp;
        rc = CP_type_tag (*pStore);           /*check the beginning tag (macro)          */
        if (rc == 0)
          rc = CP_type_dec (
            *pStore,                          /* --> Store handle (encoded byte string). */
            (unsigned long int) lStoreLength, /* --> Max. length of encoded byte string. */
            (rT041A_stCp FAR *) pNode,        /* --> PDV node (pointer to).              */
            (void NEAR *)pContext);           /* --> PPM context (pointer to).           */
        break;

      case RC0439_IEvCPA:
        pContext->lType          =            /*for use in E/D Control functions         */
        pNode->stPdvHeader.lType = RC041B_Cpa;
        rc = CPA_PPDU_tag (*pStore);          /*check the beginning tag (macro)          */
        if (rc == 0)
          rc = CPA_PPDU_dec (
            *pStore,                          /* --> Store handle (encoded byte string). */
            (unsigned long int) lStoreLength, /* --> Max. length of encoded byte string. */
            (rT041B_stCpa FAR *) pNode,       /* --> PDV node (pointer to).              */
            (void NEAR *)pContext);           /* --> PPM context (pointer to).           */
        break;

      case RC043b_IEvCPR:
        pContext->lType          =            /*for use in E/D Control functions         */
        pNode->stPdvHeader.lType = RC041C_Cpr;
        rc = CPR_PPDU_tag (*pStore);          /*check the beginning tag (macro)          */
        if (rc == 0)
          rc = CPR_PPDU_dec (
            *pStore,                          /* --> Store handle (encoded byte string). */
            (unsigned long int) lStoreLength, /* --> Max. length of encoded byte string. */
            (rT041C_stCpr FAR *) pNode,       /* --> PDV node (pointer to).              */
            (void NEAR *)pContext);           /* --> PPM context (pointer to).           */
        break;

      case RC080a_SUABORTind:
        pContext->lType = RC041D_Aru;         /*for use in E/D Control functions         */
                                              /*(important: set a value != RC041A_Cp)    */
        rc = Abort_type_tag (*pStore);        /*check the beginning tag (macro)          */
        if (rc == 0)
          rc = Abort_type_dec (
            *pStore,                          /* --> Store handle (encoded byte string). */
            (unsigned long int) lStoreLength, /* --> Max. length of encoded byte string. */
            (rT041G_unPpdu FAR *) pNode,      /* --> PDV node (pointer to).              */
            (void NEAR *)pContext);           /* --> PPM context (pointer to).           */
        break;

      default:
        if (!pContext->bNormalMode)           /*SS-user data illegal if X410-mode */
          rc = RC041m_UnexpectedPpdu;         /*note: only given to PS-user (no ARP) */

        pContext->lType          =            /*for use in E/D Control functions         */
        pNode->stPdvHeader.lType = RC041F_Other;
        rc = User_data_tag (*pStore);         /*check the beginning tag (macro)          */
        if (rc == 0)
          rc = User_data_dec (
            *pStore,                          /* --> Store handle (encoded byte string). */
            (unsigned long int) lStoreLength, /* --> Max. length of encoded byte string. */
            &pNode->stOther.stUserData,       /* --> PDV node (pointer to).              */
            (void NEAR *)pContext);           /* --> PPM context (pointer to).           */
        break;
    } /*to "switch (uInEvent)" */
   } /* normal or X410 mode  */ 

    if (rc >= 0)                            /*if OK or provider abort */
      RM0422_GlobUnLock (*phNode)

  } /*to "if (!bNormalMode ...) else" */


  /*------------------------------------------------------------------*/
  /* Process return values (for X410-mode too).                       */
  /*------------------------------------------------------------------*/

  if (rc < 0)
  {
    /*map return value if Store error */
    if (rc >= ST_MINERRVAL)
      rc = -R_PSTERR;
    RM0003_RETURN (fnctName, signed short int, rc)
  }

  if (rc == 0)
  {
    /*----------------------------------------------------------------*/
    /* Look if the whole Store is decoded. Force abort if not.        */
    /* Exceptions:                                                    */
    /* - Ignore CPC-type values.                                      */
    /* - Ignore length difference if X410-mode S-DATA indication      */
    /*   (because their are no tag and length fields and the deco-    */
    /*   ding function does not really "decode" the SS-user data).    */
    /*----------------------------------------------------------------*/

    lStoreWhere = ro0076_stwhere (*pStore);  /*get the current position */
    if (lStoreWhere < 0)
      RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

    if (lStoreWhere != lStoreLength       /*lStoreLength was computed above */
    &&  uInEvent    != RC0802_SCONNECTind /*ignore CPC-type values          */
    &&  (  uInEvent != RC080d_SDATAind    /*ignore length difference if     */
        || pContext->bNormalMode          /*X410-mode S-DATA indication     */
        )
    )
      RM0003_RETURN (fnctName, signed short int, RCro6l_UnrecognizedPpduParam)
      /*right value S!? */


    /*----------------------------------------------------------------*/
    /* Delete the Store.                                              */
    /*----------------------------------------------------------------*/

    rc = ro0075_deletestore (*pStore);
    if (rc)                             /*rc never > 0 */
      RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

    /*avoid providerAbort() actions regarding *pStore */
    *pStore = 0;                      
  }

  /*---------------------------------------------------------------------*/
  /* If rc > 0 (i.e. soft error): No mapping of return values necessary. */
  /* (Note: deletestore() and clearPdvTree() will be called when         */
  /*  P-PABORT is issued.                                                */
  /*  This has the advantage that there is no danger of lost of          */
  /*  the original return value.)                                        */
  /*---------------------------------------------------------------------*/

  else

    /*----------------------------------------------------------------*/
    /* The MAVROS created functions put their error code into the     */
    /* Store Control Block --> Necessary:                             */
    /* Reset the Store error code to avoid error at Store deletion    */
    /* (deletion will be done when P-PABORT will be issued).          */
    /* The return value (old MAVROS error code) is ignored. P!?       */
    /*----------------------------------------------------------------*/

    rc1 = ro0080_seterror (*pStore, OK);

  RM0003_RETURN (fnctName, signed short int, rc)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0415_decodeSimply                                 */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0415_decodeSimply (
	rTro01_pfvObjectId	 FAR *pTransSynName,
	signed   short int       FAR *pAbstrSynId,
	signed   short int       FAR *pPmNumb,
	void                         *pCxt)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (simply-enc. pres. user-data)  */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0415_decodeSimply")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*!! */

  RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
  /*right value S!? */
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0416_decodePdvList                                */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0416_decodePdvList (
	rTro01_pfvObjectId	 FAR *pTransSynName,
	signed   short int       FAR *pAbstrSynId,
	signed   short int       FAR *pPmNumb,
	rT0410_stPdvList         FAR *pEncodingInfo,
	void                         *pCxt)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (PDV-list)                     */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0416_decodePdvList")
rT0441_stEntContext    *pEntContext;    /*pointer to the pres. entity inv. context */
signed         int      iAbsId;         /*abstract syntax identifier */
signed         int      iTransId;       /*transfer syntax identifier */
rT0430_stContext  * pContext = (rT0430_stContext *)pCxt;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pEntContext = pContext->pEntContext;    /*local variable necessary P!? */

  /*------------------------------------------------------------------------*/
  /* Check the incoming PDU                                                 */
  /*------------------------------------------------------------------------*/

  if ((   pContext->sDcsNumb         <= 1                /*(if "fully" not o.k. or */
      &&  pContext->lType            != RC041A_Cp        /* but interworking S!?   */
      )
 /* ||  pEncodingInfo->lEncodingChoice != RCro0m_ChoiceAny */
   /* commented by kc on 28/6 to allow Octet sring & Bitstring    */
 /* if "enc. choice" not o.k.) */
  )
    RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
    /*right value S!? more than one S!? */


  /*------------------------------------------------------------------------*/
  /* Search abstract syntax identifier                                      */
  /*------------------------------------------------------------------------*/

  for (iAbsId = 0; iAbsId < RCro01_MaxAbstrSyn; iAbsId++)
    if (pContext->vPresContId [iAbsId] == pEncodingInfo->lPresContId)
      break;

  /*------------------------------------------------------------------------*/
  /* Check if the found AbsId belongs to the proposed or negotiated DCS.    */
  /* (Check of pContext->vTransSynId because the protocol allows arbitrary  */
  /* values for the elements of pContext->vPresContId, including the value  */
  /* RC043D_NoContId.)                                                      */
  /*------------------------------------------------------------------------*/

  if (iAbsId                         == RCro01_MaxAbstrSyn /*means "Not found" */
  ||  pContext->vTransSynId [iAbsId] == RC043O_NoTransId
  )
    RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
    /*right value S!? */


  /* Compute input parameter of the next decoding function. */

  pContext->sAbstrSynId =               /*for ANY */
  *pAbstrSynId          = (signed short int) iAbsId;
  *pPmNumb              = pEntContext->vAbstrSyn [iAbsId].vPmNumb [0];


  /*------------------------------------------------------------------------*/
  /* Check the transfer syntax object identifier (OPTIONAL).                */
  /* - ISO 8823 8.4.2.7: Shall be present if CP PPDU and if more than one   */
  /*   transfer syntaxes were proposed.                                     */
  /* - Implementation rule: Interworking is more important than conformance.*/
  /*   --> If CP PPDU there is no check of the presence dependent on the    */
  /*       number of proposed transfer syntaxes.                            */
  /*       Notes: - Mr. Temme says: Conformance is a prerequisite of        */
  /*                interworking tests S!?                                  */
  /*              - The number of proposed transfer syntaxes is stored      */
  /*                in pContext->vTransSynNumbInReceivedCp [] (not used     */
  /*                in the current version).                                */
  /* - Check of the object identifier value is done here and will be done   */
  /*   in the next decoding function (advantages of checking here:          */
  /*   1. native matter of the PPM                                          */
  /*   2. avoid error in the decoding function                              */
  /*   3. give pointer to the pres. entity invoc. context and not pointer   */
  /*      to pEncodingInfo to the decoding function)                        */
  /*                                                                        */
  /* Compute transfer syntax object identifier (input parameter of the      */
  /* next decoding function).                                               */
  /*------------------------------------------------------------------------*/

  if (pContext->lType                  == RC041A_Cp
  &&  pEncodingInfo->oTransSynName [0] != 0          /*OPTIONAL */
  )
  {
    /* Check if the transfer syntax is supported or not.  */

    iTransId = (signed int) ro0425_getTransId (
      pEntContext,                   /* --> Pres. entity invocation context (pointer to).   */
      iAbsId,                        /* --> Abstract syntax identifier.                     */
      pEncodingInfo->oTransSynName); /* --> Transfer syntax object identifier (pointer to). */

    if (iTransId == RC043O_NoTransId)
      RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
      /*right value S!? JM: CPR ! */
  }
  else
  {
    /* Check the presence of the transfer syntax parameter                   */
    /* (the check fails never if CP PPDU without transfer syntax parameter). */

    if (pEncodingInfo->oTransSynName [0] != 0)          /*OPTIONAL */
      RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
      /*right value S!? */


    /* Get the negotiated transfer syntax. */

    iTransId = pContext->vTransSynId [iAbsId];
    /*check if defined value P!? */
  }

  /* Compute input parameter of the next decoding function. */

  pContext->pTransSynName =               /*for ANY */
  *pTransSynName          =
	      (rTro01_pfvObjectId     )pEntContext->vTransSynName [iTransId];

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0417_decodeExternal                               */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0417_decodeExternal (
	rTro01_pfvObjectId	 FAR *pTransSynName,
	signed   short int       FAR *pAbstrSynId,
	signed   short int       FAR *pPmNumb,
	rT0411_stExternal        FAR *pEncodingInfo,
	void                         *pCxt)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (EXTERNAL)                     */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0417_decodeExternal")
rT0441_stEntContext    *pEntContext;    /*pointer to the pres. entity inv. context */
signed         int      iAbsId;         /*abstract syntax identifier */
signed         int      iTransId;       /*transfer syntax identifier */
rT0430_stContext  * pContext = (rT0430_stContext *)pCxt;


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pEntContext = pContext->pEntContext;    /*local variable necessary P!? */

  /*------------------------------------------------------------------------*/
  /* Check the incoming PDU                                                 */
  /*------------------------------------------------------------------------*/

  if (pEncodingInfo->bIndirectRef    == FALSE            /*ISO 8824 34.5 not supported */
 /* ||  pEncodingInfo->lEncodingChoice != RCro0m_ChoiceAny */
   /* commented by kc on 24/5 to allow Octet sring & Bitstring    */
                       /*if "enc. choice" not o.k.   */
  )                                                      /* not checked in ROS V2.5 S!? */
    RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
    /*right value S!? more than one S!? */


  /*------------------------------------------------------------------------*/
  /* Search abstract syntax identifier                                      */
  /*------------------------------------------------------------------------*/

  for (iAbsId = 0; iAbsId < RCro01_MaxAbstrSyn; iAbsId++)
    if (pContext->vPresContId [iAbsId] == pEncodingInfo->lIndirectRef)
      break;

  /*------------------------------------------------------------------------*/
  /* Check if the found AbsId belongs to the proposed or negotiated DCS.    */
  /* (Check of pContext->vTransSynId because the protocol allows arbitrary  */
  /* values for the elements of pContext->vPresContId, including the value  */
  /* RC043D_NoContId.)                                                      */
  /*------------------------------------------------------------------------*/

  if (iAbsId                         == RCro01_MaxAbstrSyn /*means "Not found" */
  ||  pContext->vTransSynId [iAbsId] == RC043O_NoTransId
  )
    RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
    /*right value S!? */


  /* Compute input parameter of the next decoding function. */

  pContext->sAbstrSynId =               /*for ANY */
  *pAbstrSynId          = (signed short int) iAbsId;
  *pPmNumb              = pEntContext->vAbstrSyn [iAbsId].vPmNumb [0];


  /*------------------------------------------------------------------------*/
  /* Check the transfer syntax object identifier (OPTIONAL).                */
  /* - ISO 8824 34.6: Shall be present if "pres. layer negotiation is in    */
  /*   use but incomplete" --> our interpretation: in CP PPDU (!$).         */
  /*   (There are further inprecise things too, e.g. last sentence: "shall  */
  /*   be identified by comment"; nothing about the number of proposed      */
  /*   transfer syntaxes).                                                  */
  /* - Implementation rule: Interworking is more important than conformance.*/
  /*   --> If CP PPDU there is no check of the presence (!$).               */
  /* - Check of the object identifier value is done here and will be done   */
  /*   in the next decoding function (advantages of checking here:          */
  /*   1. native matter of the PPM                                          */
  /*   2. avoid error in the decoding function                              */
  /*   3. give pointer to the pres. entity invoc. context and not pointer   */
  /*      to pEncodingInfo to the decoding function)                        */
  /*                                                                        */
  /* Compute transfer syntax object identifier (input parameter of the      */
  /* next decoding function).                                               */
  /*------------------------------------------------------------------------*/

  if (pContext->lType               == RC041A_Cp
  &&  pEncodingInfo->oDirectRef [0] != 0              /*OPTIONAL */
  )
  {
    /* Check if the transfer syntax is supported or not.  */

    iTransId = (signed int) ro0425_getTransId (
      pEntContext,                /* --> Pres. entity invocation context (pointer to).   */
      iAbsId,                     /* --> Abstract syntax identifier.                     */
      pEncodingInfo->oDirectRef); /* --> Transfer syntax object identifier (pointer to). */

    if (iTransId == RC043O_NoTransId)
      RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
      /*right value S!? JM: CPR ! */
  }
  else
  {
    /* Check the presence of the transfer syntax parameter                   */
    /* (the check fails never if CP PPDU without transfer syntax parameter). */

    if (pEncodingInfo->oDirectRef [0] != 0)          /*OPTIONAL */
      RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
      /*right value S!? */


    /* Get the negotiated transfer syntax. */

    iTransId = pContext->vTransSynId [iAbsId];
    /*check if defined value S!? */
  }

  /* Compute input parameter of the next decoding function. */

  pContext->pTransSynName =               /*for ANY */
  *pTransSynName          =
	      (rTro01_pfvObjectId      )pEntContext->vTransSynName [iTransId];

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0418_decodeAny                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0418_decodeAny (
	rTro76_stDefinedBy FAR * FAR *pDefinedBy,
	rTro01_pfvObjectId	 FAR *pTransSynName,
	signed   short int       FAR *pAbstrSynId,
	signed   short int       FAR *pPmNumb,
	signed   short             sAnyNumb,
	void                         *pCxt)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (ANY, ANY DEFINED BY)          */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0418_decodeAny")
rT0441_stEntContext    *pEntContext;     /*pointer to the pres. entity inv. context */

rT0430_stContext  * pContext = (rT0430_stContext *)pCxt;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pEntContext = pContext->pEntContext;  /*local variable necessary P!? */

  /*------------------------------------------------------------------------*/
  /* Store the DEFINED BY info if it is input parameter                     */
  /*------------------------------------------------------------------------*/

  if (pDefinedBy)
    pContext->pDefinedBy = (rTro76_stDefinedBy FAR *)*pDefinedBy;

  /*------------------------------------------------------------------------*/
  /* Set output parameters                                                  */
  /*------------------------------------------------------------------------*/

  if (pContext->bNormalMode)            /*if normal-mode */
  {
    *pDefinedBy    = (rTro76_stDefinedBy FAR *)pContext->pDefinedBy;
    *pTransSynName = (unsigned  long int FAR *)pContext->pTransSynName;
    *pAbstrSynId   = pContext->sAbstrSynId;


    /* Check the range of the "ANY number". */

    if (sAnyNumb < 0
    ||  sAnyNumb > RC0440_MaxDecodeControl
    )
      RM0003_RETURN (fnctName, signed short int, -R_PANYNU)


    *pPmNumb = pEntContext->vAbstrSyn [*pAbstrSynId].
               vPmNumb [sAnyNumb];

    /* Check the computed PM number. */

    if (*pPmNumb == RC0441_NoPmNumb)
      RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
      /*right value S!? */
  }
  else
  {
    *pDefinedBy    = (rTro76_stDefinedBy FAR *)0;
    *pTransSynName = (rTro01_pfvObjectId      )rv0410_vBerObjId;
                                                     /*redundant for X410 S!? */
    *pAbstrSynId   = -1;               /*#define P!? */
    *pPmNumb       = RC0220_RtseUser;
  }

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0432_clearUserData                                */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0432_clearUserData (
	rT0417_stUserData  FAR *pUserData)     

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0432_clearUserData")
signed         int  iInx;            /*index in "for"-loop */
signed         int  iErrCount;       /*error counter */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------------------------------------------------------*/
  /* Init error counter.                                              */
  /* Note: The function continues in the case of an error.            */
  /*       (Is this a right behaviour P!?)                            */
  /*------------------------------------------------------------------*/

  iErrCount = 0;


  /*------------------------------------------------------------------*/
  /* Simple encoding.                                                 */
  /*------------------------------------------------------------------*/

  if (pUserData->lSimplyFullyChoice == RC0417_SimplyEncoded)
  {
    if (ro0246_clearPdvTree (&pUserData->unUserData.hSimply))
      iErrCount++;
  }


  /*------------------------------------------------------------------*/
  /* Full encoding.                                                   */
  /*------------------------------------------------------------------*/

  else
    for (
    iInx = 0;
    iInx < RCro0a_MaxPdvList;     /*better pUserData->stFully.ulPdvList P!? */
    iInx++)
      if (ro0246_clearPdvTree (&pUserData->unUserData.stFully.
                               vPdvList [iInx]))
        iErrCount++;


  /*------------------------------------------------------------------*/
  /* Evaluate the error counter.                                      */
  /*------------------------------------------------------------------*/

  if (iErrCount)
    RM0003_RETURN (fnctName, signed short int, -R_PCLPDV)
  else
    RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0404_clearNodePpm                                 */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0404_clearNodePpm (
	unsigned long  int FAR *phNode)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0404_clearNodePpm")
rT041G_unPpdu  FAR *pNode;           /*pointer to PDV node */
signed         int  iErrCount;       /*error counter */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------------------------------------------------------*/
  /* Notes: 1. The function is called only if a PDV node exists       */
  /*           (checked by ro0246_clearPdvTree).                      */
  /*        2. No beginning unlock-try (PDV node is unlocked          */
  /*           when the function is called).                          */
  /*------------------------------------------------------------------*/

  /*------------------------------------------------------------------*/
  /* Init error counter.                                              */
  /* Note: The function continues in the case of an error.            */
  /*       (Is this a right behaviour P!?)                            */
  /*------------------------------------------------------------------*/

  iErrCount = 0;


  /*------------------------------------------------------------------*/
  /* Free resources bound to the PDV node.                            */
  /*------------------------------------------------------------------*/

  if ( (pNode = (rT041G_unPpdu FAR *) ro0021_globallock (*phNode)) == NULL)
    iErrCount++;
    /*furthermore only try to unlock and free the own PDV node */

  else
  {
    switch (pNode->stPdvHeader.lType)
    {
      case RC041A_Cp:
        if (ro0246_clearPdvTree  (&pNode->stCp.hX410RtpmNode))
          iErrCount++;
        if (ro0432_clearUserData (&pNode->stCp.stUserData))
          iErrCount++;
        break;
      case RC041B_Cpa:
        if (ro0246_clearPdvTree  (&pNode->stCpa.hX410RtpmNode))
          iErrCount++;
        if (ro0432_clearUserData (&pNode->stCpa.stUserData))
          iErrCount++;
        break;
      case RC041C_Cpr: 
        if (ro0246_clearPdvTree  (&pNode->stCpr.hX410RtpmNode))
          iErrCount++;
        if (ro0432_clearUserData (&pNode->stCpr.stUserData))
          iErrCount++;
        break;
      case RC041D_Aru: 
        if (ro0246_clearPdvTree  (&pNode->stAru.hX410RtpmNode))
          iErrCount++;
        if (ro0432_clearUserData (&pNode->stAru.stUserData))
          iErrCount++;
        break;
      case RC041E_Arp:
        break;
      case RC041F_Other:
        if (ro0432_clearUserData (&pNode->stOther.stUserData))
          iErrCount++;
        break;
    }
  }


  /*------------------------------------------------------------------*/
  /* Free the PDV node.                                               */
  /*------------------------------------------------------------------*/

  if (ro0022_globalunlock (*phNode))
    iErrCount++;

  if (ro0024_globalfree   (*phNode))
    iErrCount++;
  else
    *phNode = 0;


  /*------------------------------------------------------------------*/
  /* Evaluate the error counter.                                      */
  /*------------------------------------------------------------------*/

  if (iErrCount)
    RM0003_RETURN (fnctName, signed short int, -R_PCLPDV)
  else
    RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0405_getPmNumb                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0405_getPmNumb (
	unsigned long  int      hNode)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0405_getPmNumb")
rTro03_stPdvHeader  FAR *pNode;       /*pointer to PDV node */
signed   short int       sPmNumb;     /*PM number (= return code) */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  RM0421_GlobLock   (hNode, pNode, rTro03_stPdvHeader)
  sPmNumb = pNode->sPmNumb;
  RM0422_GlobUnLock (hNode)

  RM0003_RETURN (fnctName, signed short int, sPmNumb)
}/*end*/
/*EOF */

