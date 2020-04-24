/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm04.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:13:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:55  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:46  keutel
 * 	OT 11620
 * 	[1994/08/19  13:48:06  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:57  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:09  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:07  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:51:37  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:39  keutel
 * 	creation
 * 	[1994/03/21  13:20:50  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roppm04.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:48 $" } ;
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
/* TYPE         : C-MODULE                                            */
/*                                                                    */
/* NAME         : roppm04.c                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-06-29                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Some functions called from the PM function          */
/*                                                                    */
/*   This C module provides functions which                           */
/*                                                                    */
/*     - process an incoming event from PS-user                       */
/*     - process an incoming event from SPM                           */
/*     - process PS-user data given to PS-user                        */
/*     - process PS-user data got from PS-user                        */
/*     - process an event which has to be mapped                      */
/*       directly from/to SPM (pseudo PA function)                    */
/*     - process a presentation provider abort.                       */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-03-24|  original                      |hm  |         */
/* 2.0A00 K02|92-06-29|  SCCS-Id changed;              |hm  |         */
/*           |        |  providerAbort(): X410-mode,   |    |         */
/*           |        |  Sta_I0 && TriggerAbort,       |    |         */
/*           |        |  free resources bound to P3    |    |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/**********************************************************************/
/*                                                                    */
/* Note:                                                              */
/* The use of the Hungarian Notation to mark the C data types         */
/* is as follows:                                                     */
/*                                                                    */
/*   b   boolean (i.e. in the most cases signed int)                  */
/*   bs  bit string (i.e. unsigned long)                              */
/*   c   signed char                                                  */
/*   h   memory handle (i.e. unsigned long)                           */
/*   hs  Store handle (i.e. unsigned long)                            */
/*   i   signed int                                                   */
/*   l   signed long                                                  */
/*   o   object identifier                                            */
/*   p   pointer to any data type                                     */
/*   s   signed short                                                 */
/*   st  structure                                                    */
/*   u   unsigned int                                                 */
/*   uc  unsigned char                                                */
/*   ul  unsigned long                                                */
/*   un  union                                                        */
/*   us  unsigned short                                               */
/*   v   vector of any data type                                      */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/* Note:                                                              */
/* In order to mark code parts which are not clear or which are still */
/* missing, the use of some strings is as follows:                    */
/*                                                                    */
/*   S!?  open question (regarding ISO standard)                      */
/*   P!?  open question (regarding programming style)                 */
/*   !!   has still to be done                                        */
/*   !!!! has still to be done (very important)                       */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) roppm04.c 1.2 93/12/20   RTS-V2.00";


/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>      /* Machine dependent generation             */
#include <roerval.h>      /* Unrecoverable error codes                */
#include <rolgval.h>      /* Log value definitions                    */
#include <rouply0.h>      /* Common type and value definitions        */
#include <rouply1.h>      /* PM service interface structures          */
#include <routil0.h>      /* Utility interface                        */
#include <rostor0.h>      /* Store interface                          */
#include <rodpat0.h>      /* Dispatcher interface                     */
#include <rospm00.h>      /* SPM service interface                    */
#include <roppmz0.h>      /* PPM Inform funs. & defs                  */
#include <roppm00.h>      /* PPM service interface                    */
#include <roppm01.h>      /* PPM protocol interface                   */
#include <roppm02.h>      /* PPM calling interface                    */
#include <roppm04.h>      /* PPM entity invoc. context (before 03.h!) */
#include <roppm03.h>      /* PPM logging interface                    */
#include <roppm05.h>      /* PPM function defs            K02         */
#include <roppm10.h>      /* PPM intern: macros                       */
#include <roppm11.h>      /* PPM intern: types and constants          */
#include <roppm12.h>      /* PPM intern: function prototyping         */
#include <roppm14.h>      /* PPM intern: State event Matrix funct Proto */
#include <rortpm0.h>      /* RTPM Service Interface                   */
#include <rortpz0.h>      /* RTPM Inform  Interface                   */


/**********************************************************************/
/*                                                                    */
/*              E X T E R N A L   D A T A                             */
/*                                                                    */
/**********************************************************************/

extern  signed short int
   (PASCAL * rv0460_vPpaMatrix [RC0434_MaxStates] [RC0435_MaxInEvent]) PROTO ((
rT0430_stContext       *pContext,      /* <-> PM context (pointer to )       */
rT0441_stEntContext  *pEntContext,   /* --> Pres. entity invocation context   */
                                    /*     (pointer to)                */
rT04a0_unConn      FAR *pConn,      /* <-> Parameter P3 structure (pointer to)
*/
rT041G_unPpdu      FAR *pNode,               /* <-> Pres. PDV node (pointer to)           */
unsigned long  int FAR * FAR *pX410RtpmNode, /* <-- Pointer to PS-user data handle        */
                                             /*     (X410-mode) (pointer to)              */         
signed   long  int FAR * FAR *pbUserData,    /* <-- Pointer to PS-user data OPTIONAL flag */
                                             /*     (normal-mode) (pointer to)            */
rT0417_stUserData  FAR * FAR *pUserData,     /* <-- Pointer to PS-user data struct        */
                                             /*     (normal-mode) (pointer to)            */
unsigned       int FAR *pOutEvent,           /* <-- Outgoing event (pointer to)           */
unsigned long  int FAR *phNode,              /* <-> Pres. PDV node (pointer to)           */
unsigned long  int FAR *pP1,                 /* <-> Event parameter P1 (pointer to)       */
unsigned long  int FAR *pP2,                 /* <-> Event parameter P2 (pointer to)       */
unsigned long  int FAR *pP3,                 /* <-> Event parameter P2 (pointer to)       */
unsigned long  int FAR *pP4                  /* <-> Event parameter P4 (pointer to)       */
)) ;

/****
extern signed short int PASCAL ro0462_dummyUnexpectedSessEvent ();
extern signed short int PASCAL ro0461_dummyUnexpectedPpdu ();
****/

/**********************************************************************/
/*                                                                    */
/*              S T A T I C   D A T A                                 */
/*                                                                    */
/**********************************************************************/

/*EventId table 1 (events handled by the State-Event-Matrix)          */
/*Note: - RC043m_IEvSPABind (State I0) causes no event to the PS-user */
/*        and no event to the SPM.                                    */ 

static signed char rv0440_vEventId1 [RC0435_MaxInEvent] =
{
  /* event: RC0436_IEvPCONreq    */   RC040U_SendEvent,
  /*        RC0437_IEvCP         */   0,
  /*        RC0438_IEvPCONrspPos */   RC040U_SendEvent,
  /*        RC0439_IEvCPA        */   1,
  /*        RC043a_IEvPCONrspNeg */   RC040U_SendEvent,
  /*        RC043b_IEvCPR        */   2,
  /*        RC043c_IEvSCONcnfNeg */   RC040S_SConCnfNeg,
                                      /*local value, never transferred */

  /*        RC043d_IEvPRELreq    */   RC040U_SendEvent,
  /*        RC043e_IEvSRELind    */   14,
  /*        RC043f_IEvPRELrspPos */   RC040U_SendEvent,
  /*        RC043g_IEvSRELcnfPos */   15,
  /*        RC043h_IEvPRELrspNeg */   RC040U_SendEvent,
  /*        RC043i_IEvSRELcnfNeg */   15,

  /*        RC043j_IEvPUABreq    */   RC040U_SendEvent,
  /*        RC043k_IEvARU        */   3,
  /*        RC043l_IEvARP        */   4,        /*mapping if DecodeError S!? */
  /*        RC043m_IEvSPABind    */   NULL,
                                      /*RC043m_IEvSPABind: not relevant (no abort event issued) */

  /*        RC043n_IEvPDTreq     */   RC040U_SendEvent,
  /*        RC043o_IEvTD         */   7
};


/*EventId table 2 (events which has to be mapped directly from SPM to */
/*PS-user, including RC080F_SCONTINUE and RC080G_SSYNCPOINT)          */

static signed char rv0441_vEventId2 [] =
{
  /* S-TOKEN-PLEASE indication       */   0x11,
  /* S-CONTROL-GIVE indication       */   0x12,
  /* S-SYNC-MINOR indication         */   0x13,
  /* S-SYNC-MINOR confirm            */   0x14,
  /* S-U-EXCEPTION-REPORT indication */   0x18,
  /* S-ACTIVITY-START indication     */   0x19,
  /* S-ACTIVITY-RESUME indication    */   0x1a,
  /* S-ACTIVITY-END indication       */   0x1f, /*here not synchronous indices */
  /* S-ACTIVITY-END confirm          */   0x20,
  /* S-ACTIVITY-INTERRUPT indication */   0x1b,
  /* S-ACTIVITY-INTERRUPT confirm    */   0x1c,
  /* S-ACTIVITY-DISCARD indication   */   0x1d,
  /* S-ACTIVITY-DISCARD confirm      */   0x1e,
  /* S-P-EXCEPTION-REPORT indication */   0x17, /*here not synchronous indices */

  /* local events                    */
  /* S-CONTINUE                      */   NULL,
                                          /*S_CONTINUE: not relevant (no abort event issued) */
  /* S-SYNCPOINT                     */   RC040T_SSyncPoint
                                          /*local value, abort reason mapped for transfer */
};


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0440_eventFromUser                                */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0440_eventFromUser (
	rT0430_stContext       *pContext,
	unsigned       int FAR *pInEvent,
	unsigned long  int      ulP1,
	unsigned long  int      ulP4,
	signed         int FAR *pLockP3, 
	signed         int FAR *pDirectSpm, 
	unsigned long  int FAR *phNode,  
	rT041G_unPpdu FAR *FAR *ppNode)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0440_eventFromUser")
unsigned       int       uPdvNodeSize;  /*PDV node size (0: no PDV node alloc) */
unsigned       int       uPdvNodeType;  /*PDV node type */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /* ** initialize local variables */

  uPdvNodeSize  = 0;                    /*0: no PDV node alloc */

  /*-----------------------------------------------------------*/
  /* Event which has to be mapped directly from PS-user to SPM */
  /*-----------------------------------------------------------*/

  if (*pInEvent >= RC043H_MinEventDirectFromUser
  &&  *pInEvent <= RC043I_MaxEventDirectFromUser
  )
  {
    *pDirectSpm = TRUE;

    /*prepare the PDV node allocation if necessary                          */
    /*Note: Never necessary in X410-mode (ulP1 is not checked for this mode */
    /*      because no local validation is wished).                         */
    /*      Only relevant if RC0433_Stat0, then is pContext->bNormalMode    */
    /*      valid.                                                          */
    if (pContext->bNormalMode       /*(if normal-mode and     */
    &&  ulP1                        /* if PS-user data exist) */          
    )
    {
      uPdvNodeSize = sizeof (rT041F_stOther);
      uPdvNodeType = RC041F_Other;
    }
  }


  /*-----------------------------------------------------------*/
  /* Other event                                               */
  /*-----------------------------------------------------------*/

  else
  {
    switch (*pInEvent)
    {
      case RC0407_PRELEASErsp:
        if (ulP4 != RCro90_Affirmative)
          *pInEvent = RC043h_IEvPRELrspNeg;  /*no break */
      case RC0405_PRELEASEreq:
        if (!ulP1)                      /*if no PS-user data: */
          break;                        /*no PDV node alloc */
      case RC040c_PDATAreq:
        if (pContext->bNormalMode)      /*X410-mode: no CLOSE user data, */
        {                               /*no pres. PDV node for PDATAreq */
          uPdvNodeSize = sizeof (rT041F_stOther);
          uPdvNodeType = RC041F_Other;
        }
        break;
      case RC0409_PUABORTreq:
        uPdvNodeSize = sizeof (rT041D_stAru);
        uPdvNodeType = RC041D_Aru;
        break;
      case RC0403_PCONNECTrsp:
        *pLockP3     = TRUE;
        if (ulP4 == RCro80_Accept)
        {
          uPdvNodeSize = sizeof (rT041B_stCpa);
          uPdvNodeType = RC041B_Cpa;
        } 
	    /* vinita */

        else if ((ulP4 & RCro85_Redirection) == RCro85_Redirection) 
        {
          *pInEvent = RC043a_IEvPCONrspNeg; 
          uPdvNodeSize = sizeof (rT041A_stCp);   
          uPdvNodeType = RC041A_Cp;              
          *pLockP3     = FALSE;
	}

        else
        {
          *pInEvent = RC043a_IEvPCONrspNeg;
          uPdvNodeSize = sizeof (rT041C_stCpr);
          uPdvNodeType = RC041C_Cpr;
        }
        break;
      case RC0401_PCONNECTreq:
        *pLockP3     = TRUE;
        uPdvNodeSize = sizeof (rT041A_stCp);
        uPdvNodeType = RC041A_Cp;
        break;
      default:
        RM0003_RETURN (fnctName, signed short int, -R_PUNKEV)
    }
  }

  /*----------------------------------------------------*/
  /* Alloc, lock and initialize a PDV node if necessary */
  /*----------------------------------------------------*/

  if (uPdvNodeSize)
  {
    RM0420_GlobAlloc (*phNode, uPdvNodeSize)
    RM0421_GlobLock  (*phNode, *ppNode, rT041G_unPpdu)

    (*ppNode)->stPdvHeader.sPmNumb = RC0215_Presentation;
    (*ppNode)->stPdvHeader.lType   = (signed long int) uPdvNodeType;
    /* Note: sAbstrSynId, lEncodingChoice are not relevant */

    /*for use in E/D Control functions */
    pContext->lType = (signed long int) uPdvNodeType;
  }

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0441_eventFromSpm                                 */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0441_eventFromSpm (
	rT0430_stContext       *pContext,
	unsigned       int FAR *pInEvent,
	unsigned long  int FAR *pP1,
	unsigned long  int      ulP2,
	unsigned long  int      ulP4,
	signed         int FAR *pLockP3, 
	signed         int FAR *pDirectSpm, 
	unsigned long  int FAR *pStore,  
	unsigned long  int FAR *phNode,  
	rT041G_unPpdu FAR *FAR *ppNode)

/*                                                                    */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0441_eventFromSpm")
signed   short int       rc;            /*return code of a called function */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*--------------------------------------------------------*/
  /* Copy *pP1 to *pStore because otherwise providerAbort() */
  /* cannot decide clearly if *pP1 is a Store handle or a   */
  /* PDV node memory handle.                                */
  /*--------------------------------------------------------*/

  *pStore = *pP1;
  *pP1    = 0;

  /*-----------------------------------------------------------*/
  /* Event which has to be mapped directly from SPM to PS-user */
  /* (including RC080F_SCONTINUE, RC080G_SSYNCPOINT)           */
  /*-----------------------------------------------------------*/

  if (*pInEvent >= RC043J_MinEventDirectFromSpm
  &&  *pInEvent <= RC043K_MaxEventDirectFromSpm
  )
  {
    /*---------------------------------------------------------*/
    /* Check before decoding if valid state/event intersection */
    /*---------------------------------------------------------*/

    if (pContext->sState != RC0433_Stat0
    &&  *pInEvent        != RC080F_SCONTINUE /*for this event state not relevant */
    )
      RM0003_RETURN (fnctName, signed short int, RC041n_UnexpectedSessEvent)

    /*set flag */
    *pDirectSpm = TRUE;
  }


  /*-----------------------------------------------------------*/
  /* Other event                                               */
  /*-----------------------------------------------------------*/

  else
  {
    switch (*pInEvent)
    {
      case RC080d_SDATAind:
      case RC0806_SRELEASEind:
      case RC080a_SUABORTind:
      case RC080b_SPABORTind:
        break;
      case RC0808_SRELEASEcnf:
        if (ulP4 == RCro91_Negative)    /*"!= RCro90_Affirmative" P!? */
          *pInEvent = RC043i_IEvSRELcnfNeg;
        break;
      case RC0804_SCONNECTcnf:
        switch (ulP4)
        {
          case RCro80_Accept:
            break;
          case RCro81_UserReject:
            *pInEvent = RC043b_IEvCPR;
            break;
          default:
            *pInEvent = RC043c_IEvSCONcnfNeg; 
            break;
        }                               /*no break */
      case RC0802_SCONNECTind:
        *pLockP3 = TRUE;
        break;
      case RC080H_STRIGGERABORT:        /*compute provider abort reason */
        RM0003_RETURN (fnctName, signed short int,
                       (signed short int) (ulP4 >> 24) + 0x50)
      default:
        RM0003_RETURN (fnctName, signed short int, -R_PUNKEV)
    }


    /*---------------------------------------------------------*/
    /* Check before decoding if valid state/event intersection */
    /* (compare function pointer in State-Event-Matrix with    */
    /* addresses of two dummy functions)                       */
    /*---------------------------------------------------------*/

    if (rv0460_vPpaMatrix [pContext->sState] [*pInEvent & 0xff] ==
        ro0461_dummyUnexpectedPpdu)
      RM0003_RETURN (fnctName, signed short int, RC041m_UnexpectedPpdu)

    if (rv0460_vPpaMatrix [pContext->sState] [*pInEvent & 0xff] ==
        ro0462_dummyUnexpectedSessEvent)
      RM0003_RETURN (fnctName, signed short int, RC041n_UnexpectedSessEvent)
  }


  /*------------------------------------------------------------------*/
  /* Decode the PPDU.                                                 */
  /* If no Store (i. e. encoded PPDU) exists ro0431_decodPpm returns  */
  /* immediately (if a Store is mandatory the return value will cause */
  /* an error handling).                                              */
  /*------------------------------------------------------------------*/
  /* for the pupose of step add a logging here  by kc on 26-11-93 */

          ro0043_pack10Log ( RC0022_ACTIVITY,
                             RL0405_EncodeInfo,
                             3,
                             (signed short int) sizeof(unsigned int),
                             (char FAR *) pInEvent,
                             (signed short int)sizeof(unsigned long int),
                             (char FAR *)&ulP2,
                             (signed short int)sizeof(rT0430_stContext),
                             (char FAR *)pContext);
            ro0043_pack10Log ( RC0022_ACTIVITY,
              RL0404_PcxtInfo,
              3,
              (pContext->pDefinedBy)  ?
              (signed short int) sizeof(rTro76_stDefinedBy):
              (signed short int) NULL                      ,
              (char FAR *)       pContext->pDefinedBy      ,
              (pContext->pTransSynName)                    ?
              (signed short int) ((pContext->pTransSynName[0]+1)*
                        sizeof(unsigned long int)): 
              (signed short int) NULL                      ,
              (char FAR *) pContext->pTransSynName         ,
              (pContext->pEntContext)                      ?
              (signed short int) sizeof(rT0441_stEntContext):
              (signed short int) NULL                       ,
              (char FAR *) pContext->pEntContext            );

          ro0043_pack10Log ( RC0022_ACTIVITY,
                             RL0406_DecodeInfo,
                             3,
                             (signed short int)0, 
                             (char FAR *) pInEvent,
                             (signed short int)0,
                             (char FAR *)&ulP2,
                             (signed short int)0,
                             (char FAR *)pContext);
  ro042a_indefDecset(pContext); /* Added by Baiju / Amit                     
                                      Tue Apr  5 14:39:56 MDT 1994 */      
  rc = ro0431_decodPpm (
    *pInEvent, /* --> Incoming session event (defines the PPDU type). */
    pContext,  /* --> PPM context (pointer to).                       */
    pStore,    /* <-> Store handle (encoded byte string) (pointer to).*/
    phNode,   /* <-- Memory handle to PDV node (pointer to).         */
    ulP2);     /* --> Store offset received from session              */

  if (rc)
    RM0003_RETURN (fnctName, signed short int, rc)
  /*--------------------------------------------*/
  /* Note: If rc > 0 (i.e. soft error):         */
  /* later no PPA execution but provider abort. */
  /*--------------------------------------------*/


  /*----------------------------------------*/
  /* If successful and a PDV tree exists:   */
  /* - process RTPM PDV node,               */
  /* - lock the pres. PDV node,             */
  /* - map S-UABORT indication event.       */
  /*----------------------------------------*/

  if (*phNode)
  {
    /*process RTPM PDV node                        */
    /*Note: Only relevant if RC0433_Stat0, then is */
    /*      pContext->bNormalMode valid.           */
    if (!pContext->bNormalMode                  /*(if X410-mode and        */
    &&  (  *pInEvent == RC080d_SDATAind         /* (if S-DATAind or        */
        || *pInEvent == RC080f_STOKEN_PLEASEind /*  if S-TOKEN-PLEASEind)) */
        )
    )
    {
      *pP1    = *phNode;
      *phNode = 0;
      /*Note: *phNode is reserved for pres. PDV node. */
    }
    else
    {
      RM0421_GlobLock (*phNode, *ppNode, rT041G_unPpdu)

      if ((*ppNode)->stPdvHeader.lType == RC041E_Arp)
        *pInEvent = RC043l_IEvARP;
    }
  }

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0444_dataToUser                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0444_dataToUser (
	rT0430_stContext       *pContext,
	unsigned       int      uOutEvent,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *phNode,  
	unsigned long  int FAR *pX410RtpmNode,
	signed   long  int FAR *pbUserData,
	rT0417_stUserData  FAR *pUserData)
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0444_dataToUser")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*----------------*/
  /* If normal-mode */
  /*----------------*/

  if (pContext->bNormalMode)
  {
    /*test OPTIONAL indicators if PS-user PDV node exists */
    if (*phNode
/* **skh: Corrected for port to DOS: 8/10/93
    &&  (  !pbUserData
        || *pbUserData
        )
*/
    &&  (  pbUserData
        && *pbUserData
        )
    )
      /*Note: Simply/fully encoding is valid (has been checked */
      /*      in E/D Control).                                 */

      if (pUserData->lSimplyFullyChoice == RC0418_FullyEncoded) {
    /* here a check should be added to check whether ulPdvList
       is more than 1 if  so rc =1. this should be done to initiate
       provider abort :: vinita*/

       if (pUserData->unUserData.stFully.ulPdvList > 1)
       RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
        *pP1 = pUserData->unUserData.stFully.vPdvList [0];
        /*and further vPdvList elements !! P!? */
      }
      else
        *pP1 = pUserData->unUserData.hSimply;
  }

  /*--------------*/
  /* If X410-mode */
  /*--------------*/

  else
  {
    if (pX410RtpmNode)
      *pP1 = *pX410RtpmNode;

    /*Note: If X410-mode and RTTR or RTTP APDU: no pres. PDV node, */
    /*      only RTPM PDV node. *pP1 has been already set in       */
    /*      ro0441_eventFromSpm(), and pX410RtpmNode is zero.      */
  }

  /*----------------*/
  /* Common actions */
  /*----------------*/

  /*unlock and free the pres. PDV node if necessary */
  if (*phNode) 
  {
/* Not needed now bu kc on 7/7/93 
    if (uOutEvent ==  RC043t_OEvPCONind)
    {
         RM0422_GlobUnLock (*phNode)
    } 
    else
    {     */
    RM0422_GlobUnLock (*phNode)
    RM0424_GlobFree   (*phNode)
  /*  } */
    /*sets *phNode to 0 (avoid providerAbort() actions regarding *phNode) */
  }

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0445_dataFromUser                                 */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0445_dataFromUser (
	rT0430_stContext       *pContext,
	unsigned       int      uOutEvent,
	unsigned long  int FAR *pP1,
	unsigned long  int     ulP4,
	unsigned long  int FAR *phNode,  
	unsigned long  int FAR *pX410RtpmNode,
	signed   long  int FAR *pbUserData,
	rT0417_stUserData  FAR *pUserData)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0445_dataFromUser")
signed         int       bRtpmNode;     /*TRUE: no pres. PDV node (X410-mode, RTTR/RTTP) */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /* ** initialize local variables */

  bRtpmNode = FALSE;


  if (*pP1)                             /*if PS-user PDV node exists */
  {
    /*----------------*/
    /* If normal-mode */
    /*----------------*/

    if (pContext->bNormalMode)
    {
      if (pbUserData)
      /*if OPTIONAL flag exists in the pres. PDV node     */
      /*(otherwise: no pres. PDV node if no PS-user data) */ 
        *pbUserData = TRUE;
/* ***** ATTENTION ***** by kc on 12-11-93 ********************* */
/* previous state : pContext->sDcsNumb > 1                       */
/* change         : pContext->sDcsNumb >= 1                      */
/* reason : If negeotiated AbsSyns. are only 1 (ACSE) or one     */
/*          PPM User and this is rejected by the ACPM or PPM-User*/
/*          then we do have UserData with sDcsNumb == 1 & in our */
/*          implementation SimplyEncoded is not supported &      */
/*          here what encoding has to come Q to JM ????????      */
/*          So the correction was made to the above              */
/*          If any problem then look this area                   */

      if (  (uOutEvent          == RC0801_SCONNECTreq )
          ||(pContext->sDcsNumb >=  1 )
          ||( (  ulP4 & RCro85_Redirection) == RCro85_Redirection )
      )
      {
        pUserData->lSimplyFullyChoice              = RC0418_FullyEncoded;
        pUserData->unUserData.stFully.ulPdvList    = 1;   /*number */
        pUserData->unUserData.stFully.vPdvList [0] = *pP1;
        /*and further vPdvList elements !! P!? */
      }
      else
      {
        pUserData->lSimplyFullyChoice = RC0417_SimplyEncoded;
        pUserData->unUserData.hSimply = *pP1;
      }
    }

    /*--------------*/
    /* If X410-mode */
    /*--------------*/

    else
    {
      if (pX410RtpmNode)
        *pX410RtpmNode = *pP1;
      else
      {
        /*Note: if X410-mode and RTTR or RTTP APDU: no pres. PDV node,    */
        /*      only RTPM PDV node, but hNode is necessary for encodPpm() */
        *phNode   = *pP1;
        bRtpmNode = TRUE;               /*avoid unlock-try (see below) */
      }
    }

  } /*to "if (*pP1)" */
  /*Note: No "else" because *pbUserData is 0-initialized. */

  /*----------------*/
  /* Common actions */
  /*----------------*/

  /*unlock the pres. PDV root node if necessary */
  if (*phNode 
  &&  !bRtpmNode
  )
    RM0422_GlobUnLock (*phNode)

  /*avoid providerAbort() actions regarding *pP1 */
  *pP1 = 0;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0446_eventDirect                                  */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0446_eventDirect (
	rT0430_stContext       *pContext,
	unsigned       int      uInEvent,
	unsigned       int FAR *pOutEvent,
	rT041G_unPpdu      FAR *pNode,
	rT0417_stUserData  FAR * FAR *pUserData)

/*                                                                    */
/* DESCRIPTION  : Process an incoming event from PS-user              */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0446_eventDirect")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------------------------------------------------------------*/
  /* If correct PM state                                                    */
  /* (for incoming events from SPM already checked before decoding,         */
  /* therefore redundant for such events)                                   */
  /*------------------------------------------------------------------------*/

  if (pContext->sState == RC0433_Stat0
  ||  uInEvent         == RC080F_SCONTINUE /*for this event state not relevant */
  )
  {
    /* set a new PM basic event number */
    *pOutEvent = (uInEvent & 0xff00) == RC0400_PpmEvent
               ?  uInEvent - RC0400_PpmEvent + RC0800_SpmEvent
               :  uInEvent - RC0800_SpmEvent + RC0400_PpmEvent;


    /* if pres. PDV node allocated (PS-user data exist, */
    /* only possible in normal-mode for these events):  */
    /* set input parameter for the userdata() function  */
    if (pNode)
      *pUserData = (rT0417_stUserData  FAR *)&pNode->stOther.stUserData;
  }


  /*------------------------------------------------------------------------*/
  /* If invalid PM state: error handling                                    */
  /* (for incoming events from SPM already checked before decoding,         */
  /* therefore only possible for events from PS-user)                       */
  /*------------------------------------------------------------------------*/

  else
    RM0003_RETURN (fnctName, signed short int, -R_PLSQFL)

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0447_providerAbort                                */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-06-29                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0447_providerAbort (
	rT0430_stContext   NEAR * pContext,
	signed         int        iAbortReason,
	unsigned       int        uInEvent,
	unsigned long  int        ulP1,
	unsigned long  int        ulP2,
	unsigned long  int        ulP3,
	unsigned long  int        ulP4,
	unsigned long  int        hNode,
	unsigned long  int        hsStore)
 
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0447_providerAbort")
rT04a0_unConn           *pConn;         /*pointer to parameter P3 structure */
unsigned long  int       hAddress;      /*memory handle to presentation address */

unsigned long  int       hCldAddr=NULL;   
signed         int       iEventId;      /*provider abort event identifier */
rT041G_unPpdu       FAR *pNode;         /*pointer to pres. PDV node */
signed   short int       rc;            /*return code of a called function */
unsigned long  int      ulOffset= NULL; /* by kc for store Offset          */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------------------------------------------------------------*/
  /* Free the resources bound to the actual PPM call.                       */
  /* Some formal parameters are used later for function                     */
  /* intern purposes (hNode, hsStore).                                      */
  /* Notes: - The PDV nodes ulP1 and hNode are unlocked now                 */
  /*          (no beginning unlock-try in clearPdvTree()).                  */
  /*        - One aspect for "no call of ro0401_cleanUp()": there is no     */
  /*          immediate return if an error occurs in this function. (P!?)   */
  /*------------------------------------------------------------------------*/

  /* return if an error occurs P!? (clearPdvNode does not, */
  /* but this f. is called also if detach)                 */

  rc = ro0246_clearPdvTree (&ulP1);
  if (rc)
    RM0003_RETURN (fnctName, signed short int, rc)

  rc = ro0246_clearPdvTree (&hNode);
  if (rc)
    RM0003_RETURN (fnctName, signed short int, rc)

  if (hsStore)
  {
    rc = ro0075_deletestore (hsStore);
    if (rc)
      RM0003_RETURN (fnctName, signed short int, -R_PSTERR)
  }

  if (ulP3)
  {
    /* free resources bound to P3 */
    RM0431_LocLock (ulP3, pConn, rT04a0_unConn)  /*is always unlocked (P!?) */
    switch (uInEvent)
    {
      case RC0436_IEvPCONreq:
        hAddress = pConn->stReq.stAll.hCalledAddress;
        break;
      case RC0437_IEvCP:
        hAddress = pConn->stInd.stAll.hCallingAddress;
        hCldAddr = pConn->stInd.stAll.hCalledAddress;
        break;
      case RC0438_IEvPCONrspPos:
      case RC043a_IEvPCONrspNeg:
        hAddress = pConn->stRsp.stAll.hRespondAddress;
        break;
      case RC0439_IEvCPA:
      case RC043b_IEvCPR:
      case RC043c_IEvSCONcnfNeg:
        hAddress = pConn->stCnf.stAll.hRespondAddress;
        break;
      default:
        hAddress = NULL;
        break;
    }
    RM0432_LocUnLock (ulP3)
    if (hAddress)
      RM0434_LocFree (hAddress)

    if (hCldAddr)
      RM0434_LocFree (hCldAddr)

    /* free P3 */
    RM0434_LocFree (ulP3)
  }


  /*------------------------------------------------------------------------*/
  /* Compute AboEventId (table with index uInEvent)                         */
  /* Note: Invalid events already led to synchronous return value R_PUNKEV. */
  /*------------------------------------------------------------------------*/

  /*event processed by the State-Event-Matrix */
  if ((uInEvent & 0xff) < RC0435_MaxInEvent)
    iEventId = (signed int) rv0440_vEventId1 [uInEvent & 0xff];
  else

  /*event which has to be mapped directly from SPM to PS-user */
  /*(including RC080F_SCONTINUE, RC080G_SSYNCPOINT)           */
  if (uInEvent            >= RC043J_MinEventDirectFromSpm
  &&  uInEvent            <= RC043K_MaxEventDirectFromSpm
  )  
    iEventId = (signed int) rv0441_vEventId2 
               [(uInEvent & 0xff) - (RC043J_MinEventDirectFromSpm & 0xff)];
  else

  /*event RC080H_STRIGGERABORT */
  if (uInEvent == RC080H_STRIGGERABORT)
  {
    if (ulP4 & 0x00010000)
      iEventId = RC040V_NegAddAboInfo;  /*addAbortInfo neg. signed */
    else
      iEventId = RC040U_SendEvent;      /*addAbortInfo pos. signed */
  }
  else

  /*other event (has to be mapped directly from PS-user to SPM) */
    iEventId = RC040U_SendEvent;


  /*------------------------------------------------------------------------*/
  /* Issue a presentation provider abort indication event                   */
  /*------------------------------------------------------------------------*/

  /*Not if the incoming event indicates that the P-connection does not */
  /*exist for the PS-user.                                             */
  /*Notes: - In difference to S-UABORTreq, for                         */
  /*         . invalid state/event intersections                       */
  /*         . S-TriggerAbort (e.g. after P-CONNECTrspNeg)             */
  /*         (they are caused by the underlying PM!)                   */
  /*         the event has to be issued not in I0 state. (The state/   */
  /*         event intersection is checked with the old PM state,      */
  /*         of course.)                                               */
  /*         For other abort reasons it is sufficient to consider      */
  /*         the mapped incoming event, and it is not necessary to     */
  /*         consider the PM state. This has the advantage that the    */
  /*         moment of changing the PM state has no influence.         */
  /*       - For RC0437_IEvCP the event has to be issued only if       */
  /*         invalid state/event intersection.                         */

  if ((   (   iAbortReason != RC041m_UnexpectedPpdu
          &&  iAbortReason != RC041n_UnexpectedSessEvent
          &&  uInEvent     != RC080H_STRIGGERABORT
          )
      ||  pContext->sState != RC0430_StaI0
      )
  &&  (   uInEvent         != RC0437_IEvCP
      ||  iAbortReason     == RC041m_UnexpectedPpdu
      )
  &&  uInEvent != RC043a_IEvPCONrspNeg
  &&  uInEvent != RC043f_IEvPRELrspPos
  &&  uInEvent != RC043j_IEvPUABreq
  )
  {
    /*compute ulP2 */
    if (uInEvent != RC080H_STRIGGERABORT)
      ulP2 = 0;                         /*other events: no addAbortInfo */


    /*compute ulP4 */
    ulP4 =
      1                                                  /*aborted by local ppm */
    + RC0400_PpmEvent                                    /*PM basic event number */
    + (((unsigned long int) iAbortReason & 0xff) << 16)  /*abort reason */
    + (((unsigned long int) iEventId     & 0xff) << 24); /*event identifier */

    /* define values in roppm00.h (service oriented, like other PMs) P!? */


    rc = ro0211_pushOutEvent (
      RC0216_Acse,
      RC043E_OEvPPABind,
      NULL,                    /*P1              */
      ulP2,                    /*P2 (addAboInfo) */
      NULL,                    /*P3              */
      ulP4);                   /*P4 (aboReason)  */

    if (rc)
      RM0003_RETURN (fnctName, signed short int, rc)
  } /*to "if issue P-PABORTind" */


  /*------------------------------------------------------------------------*/
  /* Issue a session user abort request event                               */
  /*------------------------------------------------------------------------*/

  /*Not if the incoming event indicates that the P-connection does not */
  /*exist for the underlying PMs and the partner.                      */
  /*Not if X410-mode.                                                  */
  /*Notes: - In difference to P-PABORTind, for invalid state/event     */
  /*         intersections (they are caused by the underlying PM!)     */
  /*         the PM state has not to be considered.                    */
  /*       - RC0435_IEvARU (== RC080a_SUABORTind) is the event before        */
  /*         event mapping --> used if ARP decode error too (because   */
  /*         in this case no event mapping is done).                   */

  if (uInEvent != RC0436_IEvPCONreq
  &&  uInEvent != RC043b_IEvCPR
  &&  uInEvent != RC043c_IEvSCONcnfNeg
  &&  uInEvent != RC043g_IEvSRELcnfPos
  &&  uInEvent != RC043k_IEvARU
  &&  uInEvent != RC043m_IEvSPABind
  )
  {
    /*---------------------------------*/
    /* Alloc, lock and fill a PDV node */
    /*---------------------------------*/

    RM0420_GlobAlloc (hNode, sizeof (rT041G_unPpdu))
    RM0421_GlobLock  (hNode, pNode,  rT041G_unPpdu)

    /*process PDV header */
    pNode->stPdvHeader.sPmNumb = RC0215_Presentation;
    /* Note: sAbstrSynId, lEncodingChoice are not relevant */


    if (pContext->bNormalMode)
    {
      /*--------------------------------------*/
      /* Normal-mode: fill a PDV node for ARP */
      /*--------------------------------------*/

      /*process PDV header */
      pNode->stPdvHeader.lType = RC041E_Arp;

      /*process abort reason */
      if (iAbortReason > RCro6n_InvalidPpduParam
      ||  iEventId     > 32
      )
        /*translate a reason or event identifier of a local relevance (not ISO 8823) */
        iAbortReason = RC041k_ReasonNotSpecified; /*or OPTIONAL S!? */
      pNode->stArp.bAbortReason = TRUE;
      pNode->stArp.lAbortReason = (signed long int) iAbortReason;

      /*process event identifier */
      if (iAbortReason != RC041k_ReasonNotSpecified
      &&  iAbortReason != RC041l_UnrecognizedPpdu
      )
      {
        pNode->stArp.bEventId = TRUE;
        pNode->stArp.lEventId = (signed long int) iEventId;
      }
      /*Note: No "else" because pNode->bEventId is 0-initialized. */
    }

    else /*to "if normal-mode" */
    {
      /*----------------------------------------------------------*/
      /* X410-mode: fill a PDV node for ARU,                      */
      /*            get a PDV node describing RTAB APDU from RTPM */
      /*----------------------------------------------------------*/

      /*process pres. PDV node */
      pNode->stPdvHeader.lType         = RC041D_Aru;
      pNode->stAru.lX410orNormalChoice = RC041a_X410Mode;
      pNode->stAru.hX410RtpmNode       = (unsigned long int)iEventId;

      rc = ro0231_getInform ((unsigned long int)ro0214_getAcbEcb (NULL),
                             (signed short int)RC0218_Rtse,
                             RC10z2_GenRtabPdvNode,
                             (void FAR *)&pNode->stAru.hX410RtpmNode);
                                /* <-- Memory handle to PDV node (pointer to) */

      if (rc)                                  /*rc never > 0 */
        RM0003_RETURN (fnctName, signed short int, rc)
    } /*to "if normal-mode else" */

    RM0422_GlobUnLock (hNode)


    /*-----------------*/
    /* Encode the PPDU */
    /*-----------------*/

    rc = ro0430_encodPpm (
      RC043C_OEvARU, /* --> Outgoing session event (defines the PPDU type). */
      NULL,          /* --> Connect result (P4 of S-CONNECT response).      */
      (unsigned long int  FAR *)&ulOffset,  /* --> Offset of store for      */
                                            /* session encoding by kc 19/05 */
      pContext,      /* --> PPM context (pointer to).                       */
      (unsigned long int  FAR *)&hsStore,   /* <-- Store handle (encoded    */
                                            /*     byte string) (pointer to)*/
      (unsigned long int  FAR *)&hNode);    /* <-> Memory handle to PDV node*/
                                            /*     (pointer to).            */

    if (rc < 0)
      RM0003_RETURN (fnctName, signed short int, rc)

    /*-------------------------------*/
    /* If successful: push out event */
    /*-------------------------------*/

    if (rc == 0)
    {
      rc = ro0211_pushOutEvent (
        RC0214_Session,
        RC043C_OEvARU,
        hsStore,                 /*P1 */
        ulOffset,                /*P2 */
        NULL,                    /*P3 */
        NULL);                   /*P4 */

      if (rc)                           /*never > 0 */
        RM0003_RETURN (fnctName, signed short int, rc)
    }

    /*----------------------------------------------------------*/
    /* If rc > 0 (i.e. soft error):                             */
    /* no further provider abort. Try to free resources.        */
    /*----------------------------------------------------------*/

    else
    {
      RM0424_GlobFree (hNode)

      if (hsStore)
      {
        rc = ro0075_deletestore (hsStore);
        if (rc)
          RM0003_RETURN (fnctName, signed short int, -R_PSTERR)
      }
    }

  } /*to "if issue S-UABORTreq" */

  /*---------------------------------------------------------*/
  /* Set Idle state (leads to loss of PM invocation context) */
  /*---------------------------------------------------------*/

  pContext->sState = RC0430_StaI0;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/


/* EOF */

