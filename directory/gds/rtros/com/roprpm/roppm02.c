/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm02.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:13:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:48  root]
 * 
 * Revision 1.1.2.5  1994/08/24  08:35:22  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:34:16  marrek]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:41  keutel
 * 	OT 11620
 * 	[1994/08/19  13:47:56  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:54  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:04  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:04  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:51:13  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:33  keutel
 * 	creation
 * 	[1994/03/21  13:20:28  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roppm02.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:42 $" } ;
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
/* NAME         : roppm02.c                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-06-29                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Presentation context handling functions,            */
/*                called from the MAVROS created functions which      */
/*                decode PPCI                                         */
/*                                                                    */
/*   This C module provides functions which                           */
/*                                                                    */
/*     - handle pres. context for a received CP PPDU                  */
/*     - handle pres. context for a received CPA PPDU                 */
/*     - handle pres. context for a received CPR PPDU                 */
/*     - handle pres. context for a received ARU PPDU.                */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-03-25|  adaptation of ROS V2.5A00 K05 |hm  |         */
/* 2.0A00 K02|92-06-29|  SCCS-Id changed;              |hm  |         */
/*           |        |  corrections in syntaxAru()    |    |         */
/*           |        |                                |    |         */
/* 3.0   K02 |08-08-94| roppm05.h is included          | kc |         */
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

static char *sccsid =    "@(#) roppm02.c 1.1 93/11/29   RTS-V2.00";


/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>      /* Machine dependent generation             */
#include <roerval.h>
#include <rouply0.h>      /* Common type and value definitions        */
#include <rouply1.h>      /* PM service interface structures          */
#include <routil0.h>      /* Utility interface (common functions)     */
#include <routil1.h>      /* Utility interface (object id. functions) */
#include <rodpat0.h> 
#include <roppm00.h>
#include <roppm01.h>      /* PPM protocol interface                   */
#include <roppm04.h>      /* PPM entity invoc. context (before 03.h!) */
#include <roppm05.h>      /* PPM function prototypes       K02        */
#include <roppmz0.h>      /* PPM inform  function header              */
#include <roppm03.h>      /* PPM logging interface                    */
#include <roppm10.h>      /* PPM intern: macros                       */
#include <roppm11.h>      /* PPM intern: types and constants          */
#include <roppm12.h>      /* PPM intern: function prototyping         */
#include <roppm13.h>      /* PPM intern: function prototyping (MAVROS)*/

/**********************************************************************/
/*                                                                    */
/*              Static Function Declaration                           */
/*                                                                    */
/**********************************************************************/

/* ****************** Presentation context handling sub-function, *** */
/* ****************** called from: - ro0421_syntaxCpa             *** */
/* ******************              - ro0422_syntaxCpr             *** */

static signed short int PASCAL ro0424_syntaxCpaCprReceive PROTO ((
   rT0430_stContext            *pContext,
   signed   long  int      FAR *pbPresContDefRes,
   unsigned long  int      FAR *pulPresContDefRes,
   rT0413_stPresContDefRes FAR *pvPresContDefRes));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value,
                       mapping of the value within the PPM encoding function).

   - pContext          : <-> PPM context (pointer to).
   - pbPresContDefRes  : --> Pres. PDV node element (pointer to):
                             presentation-context-definition-result-list OPTIONAL.
   - pulPresContDefRes : --> Pres. PDV node element (pointer to):
                             presentation-context-definition-result-list number of elements.
   - pvPresContDefRes  : --> Pres. PDV node element (pointer to):
                             presentation-context-definition-result-list element.
*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0420_syntaxCp                                     */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0420_syntaxCp (
void                     *pCxt,     /* <-> PPM context (pointer to).    */
rT041G_unPpdu        FAR *pNode)        /* --> Pres. PDV node (pointer to). */

/*                                                                    */
/* DESCRIPTION  : Handle PPCI Parameters for SCONNECTind              */
/*                (especially abstract and transfer syntaxes).        */
/*                                                                    */
/*   Called from CP_dec() after the decoding of the PPCI.             */
/*                                                                    */
/*   Negotiates the Defined Context Set (DCS) and stores it           */
/*   in the PPM context.                                              */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0420_syntaxCp")
rT0441_stEntContext      *pEntContext;/*pointer to the pres. entity inv. context */
rT0412_stPresContDef FAR *pAux;      /*aux. pointer to a context list element */
signed         int        iNumb;     /*number of proposed abstract syntaxes */
signed         int        iAbsId;    /*abstract syntax identifier */
signed         int        iInx;      /*index in "for"-loop */
signed         int        iTrsInx;   /*index in "for"-loop (proposed transfer syntaxes) */

rTro00_vObjectId          vBerTrfSyn = {3,2,1,1} ;

rT0430_stContext         *pContext = ( rT0430_stContext *)pCxt;     
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pEntContext = pContext->pEntContext;

  /*-----------------------------------------------------------*/
  /* Set in PPM context "bNormalMode" (necessary in decodeAny) */
  /*-----------------------------------------------------------*/

  pContext->bNormalMode = TRUE;


  /*-----------------------------------------------------------*/
  /* Process the received presentation-context-definition-list */
  /*-----------------------------------------------------------*/

  /* ***** process the number of elements */

  if (pNode->stCp.bPresContDef == FALSE)
    iNumb = 0;
  else
    iNumb = (signed int) pNode->stCp.ulPresContDef;

  pContext->vAbstrSynId     [0] = /*redundant P!? */
  pContext->vAbstrSynResult [0] = (signed short int) iNumb;


  /* ***** initialize PPM context arrays */

  for (iInx = 1; iInx <= RCro04_MaxPresCont; iInx++)
  {
    pContext->vAbstrSynId     [iInx] = RC043M_NoAbsId;
    pContext->vAbstrSynResult [iInx] = RC043N_NoSynResult;
  }

  for (iInx = 0; iInx <  RCro01_MaxAbstrSyn; iInx++)
  {
    pContext->vTransSynId     [iInx] = RC043O_NoTransId;
    pContext->vPresContId     [iInx] = RC043P_NoContId;
  }


  /* ***** compute PPM context */

  for (
  iInx  = 1,
  pAux  = pNode->stCp.vPresContDef;
  iInx <= iNumb;
  iInx++,
  pAux++)
  {
    /* ***** initialize result/reason for this abstract syntax */

    pContext->vAbstrSynResult [iInx] = RCro6f_AbstrSynNotSupported;
   

    /* ***** decide whether abstract syntax is supported or not */
   
    for (
    iAbsId = 0;
    iAbsId < RCro01_MaxAbstrSyn;
    iAbsId++)
    {
      if (ro0003_objcmp (
            pAux->oAbstrSynName,
            pEntContext->vAbstrSyn [iAbsId].oAbstrSynName) == OK
      )
      {
        /* ***** decide whether a transfer syntax for this */
        /*       abstract syntax is supported or not       */
     
        /*new initial value */
        pContext->vAbstrSynResult [iInx] = RCro6g_TransSynNotSupported; 
        /*iAbsId here into PPM context P!? (max. info to user) */

        /*loop: check whether BER is  proposed by the initiator */
        for (
        iTrsInx = 0;
        iTrsInx < (signed int) pAux->ulTransSynNumber;
        iTrsInx ++)
        {
          if (ro0003_objcmp (
              pAux->vTransSynName [iTrsInx],
              vBerTrfSyn) == OK)
           { 
            pContext->vTransSynId [iAbsId] = ro0425_getTransId (
            pEntContext,   /* --> Pres. entity invocation context (pointer to).   */
            iAbsId,        /* --> Abstract syntax identifier.                     */
            pAux->vTransSynName [iTrsInx]);
                           /* --> Transfer syntax object identifier (pointer to). */

            pContext->vAbstrSynResult [iInx  ] = RCro6c_Acceptance;
            pContext->vAbstrSynId     [iInx  ] = (signed short int) iAbsId;
            pContext->vPresContId     [iAbsId] = pAux->lPresContId;
            pContext->vTransSynNumbInReceivedCp  [iAbsId] = (signed short int)
                                                 pAux->ulTransSynNumber;
                                                 /*used for check of PDV-list */
            pContext->sDcsNumb++;                /*number of agreed syntaxes */

            break;  /*success (transfer syntax found), out of iTrsInx-loop */
           }  /*loop: to find whether BER is proposed by the initiator */
        else
        {
          /*look for a proposed transfer syntax in the own pres. entity invoc. context */
            pContext->vTransSynId [iAbsId] = ro0425_getTransId (
            pEntContext,   /* --> Pres. entity invocation context (pointer to).   */
            iAbsId,        /* --> Abstract syntax identifier.                     */
            pAux->vTransSynName [iTrsInx]);
                           /* --> Transfer syntax object identifier (pointer to). */

          if (pContext->vTransSynId [iAbsId] != RC043O_NoTransId)
          {
            /* ***** accepted -> insert into DCS */

            pContext->vAbstrSynResult [iInx  ] = RCro6c_Acceptance;
            pContext->vAbstrSynId     [iInx  ] = (signed short int) iAbsId;
            pContext->vPresContId     [iAbsId] = pAux->lPresContId;
            pContext->vTransSynNumbInReceivedCp  [iAbsId] = (signed short int)
                                                 pAux->ulTransSynNumber;
                                                 /*used for check of PDV-list */
            pContext->sDcsNumb++;                /*number of agreed syntaxes */

            break;  /*success (transfer syntax found), out of iTrsInx-loop */
          }
        } /* else part of BER */    
     } /*to "for (iTrsInx)" */

        break;      /*success (abstract syntax found), out of iAbsId-loop   */
      } /*to "if  (objcmp AbstrSynName)" */
    }   /*to "for (iAbsId)" */
  }     /*to "for (iInx)" */


  /*------------------------------*/
  /* Default presentation context */
  /*------------------------------*/

  if (pNode->stCp.bDefaultContext)
  {
    /* ***** initialize CPR parameter provider-reason */

    pContext->lProviderReason = RC041h_DefaultContNotSuppo;


    /* ***** initialize result for default context negotiation */

    /*Note: The value is stored in the PPM context in order to send */
    /*      a correct CPR PPDU if the reject reason is another than */
    /*      "default context not supported".                        */
    /*      (mapping to CPR parameter default-context-result with   */
    /*      value "provider-rejection")                             */

    pContext->sDefContResult = RCro6f_AbstrSynNotSupported;
   

    /* ***** decide whether default abstract syntax is supported or not */
   
    for (
    iAbsId = 0;
    iAbsId < RCro01_MaxAbstrSyn;
    iAbsId++)
    {
      if (ro0003_objcmp (
            pNode->stCp.oDefaultAbstrSyn,
            pEntContext->vAbstrSyn [iAbsId].oAbstrSynName)   == OK
      )
      {
        /* ***** decide whether the default transfer syntax for the */
        /*       default abstract syntax is supported or not        */
     
        /*new initial value (for offline-troubleshooting) */
        pContext->sDefContResult = RCro6g_TransSynNotSupported;

        /*look for the default transfer syntax in the own pres. entity invoc. context */
        pContext->sDefTransSynId = ro0425_getTransId (
          pEntContext,   /* --> Pres. entity invocation context (pointer to).   */
          iAbsId,        /* --> Abstract syntax identifier.                     */
          pNode->stCp.oDefaultTransSyn);
                         /* --> Transfer syntax object identifier (pointer to). */

        if (pContext->sDefTransSynId != RC043O_NoTransId)
        {
          /* ***** accepted -> insert into PPM context */

          pContext->lProviderReason = RC043Q_NoReject;
          pContext->sDefContResult  = RCro6c_Acceptance;
          pContext->sDefAbstrSynId  = (signed short int) iAbsId;
        }
      } /*to "if  (objcmp AbstrSynName)" */
    }   /*to "for (iAbsId)" */
  }     /*to "if  (bDefaultContext)" */
  else
  {
    pContext->sDefContResult = /*one of them redundant P!? */
    pContext->sDefAbstrSynId = RCro6i_NoDefContext;
  }

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0421_syntaxCpa                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0421_syntaxCpa (
void                     *pCxt,     /* <-> PPM context (pointer to).    */
rT041G_unPpdu        FAR *pNode)        /* --> Pres. PDV node (pointer to). */

/*                                                                    */
/* DESCRIPTION  : Handle PPCI Parameters for SCONNECTcnf              */
/*                (especially abstract and transfer syntaxes).        */
/*                                                                    */
/*   Called from CPA_dec() after the decoding of the PPCI.            */
/*                                                                    */
/*   Negotiates the Defined Context Set (DCS) and stores it           */
/*   in the PPM context.                                              */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0421_syntaxCpa")
signed   short int        rc;        /*returncode of a called function */


rT0430_stContext         *pContext = ( rT0430_stContext *)pCxt;     
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------------------------------------------------------*/
  /* Process the received presentation-context-definition-result-list */
  /*------------------------------------------------------------------*/

  rc = ro0424_syntaxCpaCprReceive (
    pContext,                       /* <-> PPM context (pointer to). */
    &pNode->stCpa.bPresContDefRes,  /* --> Pres. PDV node element (pointer to) */
    &pNode->stCpa.ulPresContDefRes, /* --> Pres. PDV node element (pointer to) */
    pNode->stCpa.vPresContDefRes);  /* --> Pres. PDV node element (pointer to) */

  if (rc)
    RM0003_RETURN (fnctName, signed short int, rc)


  /*--------------------------------------*/
  /* Process Default presentation context */
  /*--------------------------------------*/

  if (pContext->sDefContResult != RCro6i_NoDefContext)
    pContext->sDefContResult = RCro6c_Acceptance;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0422_syntaxCpr                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-25                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0422_syntaxCpr (
void                     *pCxt,     /* <-> PPM context (pointer to).    */
rT041G_unPpdu        FAR *pNode)        /* --> Pres. PDV node (pointer to). */

/*                                                                    */
/* DESCRIPTION  : Handle PPCI Parameters for SCONNECTcnf              */
/*                (especially abstract and transfer syntaxes).        */
/*                                                                    */
/*   Called from CPR_dec() after the decoding of the PPCI.            */
/*                                                                    */
/*   Negotiates the Defined Context Set (DCS) and stores it           */
/*   in the PPM context.                                              */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0422_syntaxCpr")
signed   short int        rc;        /*returncode of a called function */


rT0430_stContext         *pContext = ( rT0430_stContext *)pCxt;     
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------------------------------------------------------*/
  /* Process the received presentation-context-definition-result-list */
  /*------------------------------------------------------------------*/

  rc = ro0424_syntaxCpaCprReceive (
    pContext,                       /* <-> PPM context (pointer to). */
    &pNode->stCpr.bPresContDefRes,  /* --> Pres. PDV node element (pointer to) */
    &pNode->stCpr.ulPresContDefRes, /* --> Pres. PDV node element (pointer to) */
    pNode->stCpr.vPresContDefRes);  /* --> Pres. PDV node element (pointer to) */

  if (rc)
    RM0003_RETURN (fnctName, signed short int, rc)


  /*------------------------------------------------------------------------*/
  /* Process Default presentation context                                   */
  /*                                                                        */
  /* Implementation rule: Interworking is more important than conformance.  */
  /* --> Default context proposed: no abort if result is not received       */
  /*     (RC043B_NoSynReason has been set in P-CONNECTreq routine).         */ 
  /* --> Default context not proposed: no abort if result is received       */
  /*     (RC043B_NoSynReason has been set in P-CONNECTreq routine).         */ 
  /* --> Check if valid value not necessary because association is closed   */
  /*     and check in E/D Control only if "acceptance".                     */
  /*------------------------------------------------------------------------*/

  if (pNode->stCpr.bDefaultContextResult)
    if (pContext->sDefContResult != RCro6i_NoDefContext)
      switch (pNode->stCpr.lDefaultContextResult)
      {
        case RC0410_Acceptance:
          pContext->sDefContResult = RCro6c_Acceptance;
          break;                                                 
        case RC0411_UserRejection:                               
          pContext->sDefContResult = RCro6d_UserRejection;
          break;
        case RC0412_ProviderRejection:
        default:
          /*default: value not defined by ISO; force provider abort S!? */
          pContext->sDefContResult = RCro6e_ReasonNotSpecified;
          /*right value P!? */
          break;
      } /*to "switch (result)" */

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0423_syntaxAru                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-06-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0423_syntaxAru (
void                     *pCxt,     /* <-> PPM context (pointer to).    */
rT041G_unPpdu        FAR *pNode)        /* --> Pres. PDV node (pointer to). */

/*                                                                    */
/* DESCRIPTION  : Handle PPCI Parameters of the ARU PPDU              */
/*                (presentation-context-identifier-list)              */
/*                                                                    */
/*   Called from ARU_dec() after the decoding of the PPCI.            */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0423_syntaxAru")
rT0441_stEntContext    *pEntContext;    /*pointer to the pres. entity inv. context */
rT0414_stPresContId FAR *pAux;          /*aux. pointer to a context list element */
signed         int      iAbsId;         /*abstract syntax identifier */
signed         int      iTransId;       /*transfer syntax identifier */
signed         int      iInx;           /*index in "for"-loop (local pres. contexts) */
signed         int      iRecInx;        /*index in "for"-loop (received pres. contexts) */
signed         int      iRecNumb;       /*number of received pres. contexts */
signed         int      iRecNumbFound;  /*number of received pres. contexts found locally */


rT0430_stContext         *pContext = ( rT0430_stContext *)pCxt;     
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pEntContext = pContext->pEntContext;

  /*------------------------------------------------------------------------*/
  /* Process the presentation-context-identifier-list                       */
  /*                                                                        */
  /* - Implementation rule: Interworking is more important than conformance.*/
  /*   Differences from ISO 8823 6.4.2.1 (by the way not very precise)      */
  /*   are tolerated if the DCS is well-known and if there are no contra-   */
  /*   dictions between the DCS and the received list:                      */
  /*   --> The presence of the list is tolerated e.g. if there are no       */
  /*       PS-user data (note: in the current version just now there is no  */
  /*       knowledge if PS-user data exist).                                */
  /*   --> The absence of the whole list or of some pres. contexts is tole- */
  /*       rated if the PM state is "t0", i.e. the DCS is negotiated.       */
  /*   --> The list may contain all pres. contexts, not only those used     */
  /*       in the PS-user data (ISO 8823 6.4.2.1 is not precise especially  */
  /*       in this question).                                               */
  /* - The presence of the list is only required if PS-user data exist,     */
  /*   the DCS is not empty and the pres. context negotiation is not fi-    */
  /*   nished yet (PM states "I1" and "I2"). The first condition (PS-user   */
  /*   data exist) is given if an E/D Control function is called. The check */
  /*   is done in this function via pContext->vTransSynId []. If the list   */
  /*   was absent or if it did not include the right pres. contexts         */
  /*   the value RC043O_NoTransId causes an abort. This value               */
  /*   RC043O_NoTransId is set in the current function.                     */
  /*   (In the current implementation it is not checked if more than one    */
  /*   transfer syntaxes were proposed; note: this check would be a prob-   */
  /*   lem in I2. S!? P!?)                                                  */
  /* - The processing is done outgoing from the local list of pres.         */
  /*   contexts. This is better than a processing outgoing from the recei-  */
  /*   ved list because there may be deletions in the local list.           */
  /*   (The procedure is not time critical and so it is not bad that the    */
  /*   increment of pAux which may require a multiplication is done in the  */
  /*   inner loop.)                                                         */
  /*   Note: The members of the two lists don't correspond                  */
  /*         (like they correspond e.g. in the CONNECTcnf primitive).       */
  /*         The local list may be greater than the received list but not   */
  /*         vice versa.                                                    */
  /*         S-UABORTind in state I0 is an invalid event, and the current   */
  /*         function is not called in this case.                           */
  /*------------------------------------------------------------------------*/

  if (pNode->stAru.bPresContId)
  {
    iRecNumb      = (signed int) pNode->stAru.ulPresContId;
    iRecNumbFound = 0;

    /*------------------------*/
    /* Outer loop: local list */
    /*------------------------*/

    for (
    iInx  = 1;
    iInx <= pContext->vAbstrSynResult [0];
    iInx++)
    {
      /*ignore gaps in the local list (caused by syntax negotiation) */
      if (pContext->vAbstrSynResult [iInx] != RCro6c_Acceptance
      &&  pContext->sState                 != RC0431_StaI1
      )
        continue;

      iAbsId = (signed int) pContext->vAbstrSynId [iInx];


      /*-----------------------------------------------*/
      /* Inner loop: received list                     */
      /* (look for the local pres. context identifier) */
      /*-----------------------------------------------*/

      for (
      iRecInx  = 1,
      pAux     = pNode->stAru.vPresContId;
      iRecInx <= iRecNumb;
      iRecInx++,
      pAux++)
        if (pAux->lPresContId == pContext->vPresContId [iAbsId])
        {
          iRecNumbFound++;
          break;
        }

      /*---------------------------------------------------------------------*/
      /* If not RC0433_Stat0 and if not found: delete the local list element */
      /*---------------------------------------------------------------------*/

      if (iRecInx > iRecNumb)
      {
        if (pContext->sState != RC0433_Stat0)
          pContext->vTransSynId [iAbsId] = RC043O_NoTransId;
        /*Notes:                                                        */
        /* - Don't decrement pContext->sDcsNumb (must be greater than 1 */
        /*   for fully-encoded PS-user data. (S!?)                      */
        /* - It is not necessary to reset the other PPM context arrays  */
        /*   (would be only for offline-troubleshooting, but the        */
        /*   PM context will be freed soon.                             */
      }

      else
      {
        if (pContext->sState != RC0433_Stat0)
        {
          /*----------------------------------------------------------------*/
          /* Update the TransId list in PPM context which was created       */
          /* during P-CONNECTreq or P-CONNECTind                            */
          /* Notes: - In I2 there is no knowledge whether the received      */
          /*          transfer syntax was one of the proposed or not.       */
          /*        - I1 and I2: product-intern PS-user has not to be       */
          /*          informed about the transfer syntax at the PM module's */
          /*          service interface; the information is given via       */
          /*          E/D Control.                                          */
          /*        - I1 and I2: product-extern RTSE-user (and PS-user) has */
          /*          not to be informed about the transfer syntax at IRTS  */
          /*          because the RTSE service and protocol standards don't */
          /*          allow RT-USER-ABORT before association establishment  */
          /*          has been finished.                                    */
          /*          For later applications (e.g. ROSE applications) the   */
          /*          External Interface module may pass the PPM module's   */
          /*          service parameter P4).                                */
          /*----------------------------------------------------------------*/

  /*   In the current implementation in I2 only the transfer syntax given   */
  /*   at P_CONNECTind is accepted. May be better to update this transfer   */
  /*   syntax like in I1, but problem: inform the PS-user about the new     */
  /*   transfer syntax - product intern: parameter P4, but extern P!?)      */

          pContext->vTransSynId [iAbsId] = ro0425_getTransId (
            pEntContext,          /* --> Pres. entity invocation context (pointer to).   */
            iAbsId,               /* --> Abstract syntax identifier.                     */
            pAux->oTransSynName); /* --> Transfer syntax object identifier (pointer to). */

          if (pContext->vTransSynId [iAbsId] == RC043O_NoTransId)
            RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)
        }

        else
        {
          /*----------------------------------------------------------*/
          /* Decide whether the received transfer syntax is the       */
          /* negotiated transfer syntax for this abstract syntax (t0) */
          /*----------------------------------------------------------*/

          iTransId = (signed int) pContext->vTransSynId [iAbsId];

          if (ro0003_objcmp (
                pAux->oTransSynName,
                pEntContext->vTransSynName [iTransId]) != OK
          )
            RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)

        } /*to "if (!RC0433_Stat0)" */
      }   /*to "if (PresContId found)" */
    }     /*to "for (iInx)" (local list) */


    /*-----------------------------------------------------*/
    /* Abort if received list too long (better ignore S!?) */
    /*-----------------------------------------------------*/

    if (iRecNumb > iRecNumbFound)
      RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)

  } /*to "if (bPresContId)" */

  else

    /*-------------------------------------------------------------------------*/
    /* If not RC0433_Stat0 and no list received: E/D Control shall force abort */
    /*-------------------------------------------------------------------------*/

    if (pContext->sState != RC0433_Stat0)
      for (iAbsId = 0; iAbsId < RCro01_MaxAbstrSyn; iAbsId++)
        pContext->vTransSynId [iAbsId] = RC043O_NoTransId;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0424_syntaxCpaCprReceive                          */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed short int PASCAL ro0424_syntaxCpaCprReceive (
	rT0430_stContext        *pContext,     /* <-> PM context (pointer to) */
	signed   long  int      FAR *pbPresContDefRes,
	unsigned long  int      FAR *pulPresContDefRes,
	rT0413_stPresContDefRes FAR *pvPresContDefRes)

/*                                                                    */
/* DESCRIPTION  : Handle PPCI Parameters for SCONNECTcnf              */
/*                (especially abstract and transfer syntaxes).        */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0424_syntaxCpaCprReceive")
rT0441_stEntContext      *pEntContext;/*pointer to the pres. entity inv. context */
rT0413_stPresContDefRes FAR *pAux;   /*aux. pointer to a context list element */
signed         int        iNumb;     /*number of proposed abstract syntaxes */
signed         int        iInx;      /*index in "for"-loop */
signed         int        iAbsId;    /*abstract syntax identifier */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pEntContext = pContext->pEntContext;

  /*---------------------------------------------------------*/
  /* Process the presentation-context-definition-result-list */
  /*---------------------------------------------------------*/

  /* ***** process the number of elements */

  if (!*pbPresContDefRes
  ||  *pulPresContDefRes < 1
  )
      /*Isn't the 2nd check redundant to the 1st one S!?           */
      /*But note: heterogen (!) implementations are communicating. */
    iNumb = 0;
  else
    iNumb = (signed int) *pulPresContDefRes;


  if (iNumb != pContext->vAbstrSynResult [0])
    RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)


  /* ***** compute PPM context */

  for (
  iInx  = 1,
  pAux  = pvPresContDefRes;             /*pAux because better readability */
  iInx <= iNumb;
  iInx++,
  pAux++)
  {
    iAbsId = pContext->vAbstrSynId [iInx];

    switch (pAux->lResult)
    {
      case RC0410_Acceptance:
        pContext->vAbstrSynResult [iInx] = RCro6c_Acceptance;

        /* ***** Decide whether the confirmed transfer syntax for this */
        /*       abstract syntax was one of the proposed or not.       */

        if (pAux->oTransSynName [0] == 0)             /*check OPTIONAL */
          RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)

        pContext->vTransSynId [iAbsId] = ro0425_getTransId (
          pEntContext,          /* --> Pres. entity invocation context (pointer to).   */
          iAbsId,               /* --> Abstract syntax identifier.                     */
          pAux->oTransSynName); /* --> Transfer syntax object identifier (pointer to). */

        if (pContext->vTransSynId [iAbsId] == RC043O_NoTransId)
          RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)

        break;                                                 
                                                               
      case RC0411_UserRejection:                               
        pContext->vAbstrSynResult [iInx] = RCro6d_UserRejection;

        break;

      case RC0412_ProviderRejection:
      default:
        /*default: value not defined by ISO; force provider abort S!? */
        /*check pAux->bProviderReason S!? (interworking?)*/
        if (!pAux->bProviderReason)             /*check OPTIONAL */
          RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)

        pContext->vAbstrSynResult [iInx] = (signed short int)
                                           (- pAux->lProviderReason) - 1;

        /*compute a local used value of ProviderReason               */
        /*(necessary because Result and ProviderReason are contained */
        /* in one variable in ROS; the 2 ISO defined value areas are */
        /* overlapped.)                                              */
        /*Other situation than at CPR-lProviderReason ! (P!?)        */
        /*check if the provider reason value is valid S!?            */

        break;
    } /*to "switch (result)" */


    /* ***** actualize the context set (DCS) predefined by PCONNECTreq */

    if (pContext->vAbstrSynResult [iInx] != RCro6c_Acceptance)
    {
      pContext->vAbstrSynId [iInx  ] = RC043M_NoAbsId;
      pContext->vTransSynId [iAbsId] = RC043O_NoTransId;
      pContext->vPresContId [iAbsId] = RC043P_NoContId;
      pContext->sDcsNumb--;
    }

  } /*to "for (iInx)" */


  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0425_getTransId                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0425_getTransId (
rT0441_stEntContext      *pEntContext,  /* --> pres. entity inv. context (pointer to) */
signed         int        iAbsId,       /* --> abstract syntax identifier */
unsigned long  int  FAR  *oTransSynName)/* --> transfer syntax object id. (pointer to) */

/*                                                                    */
/* DESCRIPTION  : Look for a transfer syntax in the pres. entity      */
/*                invocation context and return the TransId.          */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0425_getTransId")
signed   short int        sTransId;  /*transfer syntax identifier */
signed         int        iTrsInx;   /*index in "for"-loop (local transfer syntaxes) */
signed         int        iTrsNumb;  /*number of local transfer syntaxes */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  iTrsNumb = (signed int) pEntContext->vAbstrSyn [iAbsId].
             vTransSynId [0];

  for (
  iTrsInx  = 1;
  iTrsInx <= iTrsNumb;
  iTrsInx++)
  {
    sTransId = pEntContext->vAbstrSyn [iAbsId].vTransSynId [iTrsInx];

    if (ro0003_objcmp (
          oTransSynName,
          pEntContext->vTransSynName [sTransId]) == OK
    )
      break;  /*success (transfer syntax found), out of iTrsInx-loop */
  } /*to "for (iTrsInx)" */


  if (iTrsInx > iTrsNumb)
    RM0003_RETURN (fnctName, signed short int, RC043O_NoTransId)
  else
    RM0003_RETURN (fnctName, signed short int, sTransId)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0426_getAbsId                                   */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE         : 93-03-01                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0426_getAbsId (
	signed   short int      sInformType,
	rT04z1_stAbsNameId FAR *pstAbsNameId,
        unsigned long  int      hContext) /* K02 */
/*                                                                    */
/* DESCRIPTION  : Look for a transfer syntax in the pres. entity      */
/*                invocation context and return the TransId.          */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0426_getAbsId")
unsigned long  int      hEntContext;
rT0441_stEntContext    *pEntContext;
signed         int      iAbsId;
signed         int      rc=0;
/* K02 rTro00_vObjectId        vBerTrfSyn = {3,2,1,1} ; */
rT0430_stContext        *pContext; 

  RM0002_ENTRY (fnctName)

  if ((pstAbsNameId->oAbsName[0]< 3)
  ||  (pstAbsNameId->oAbsName[0] > RCro00_MaxObjIdComp)
  )
    RM0003_RETURN (fnctName, signed short int, -R_POBJID)

  rc = ro0215_getEiCxt (
         RC0232_PresentationEiCxt,
         &hEntContext);
  if (rc < 0)
    RM0003_RETURN (fnctName, signed short int, rc)
  if (rc > 0)
    RM0003_RETURN (fnctName, signed short int, -R_PENTCX)

  RM0431_LocLock (hEntContext, pEntContext, rT0441_stEntContext);

switch (sInformType) {

case RC04zP_ACSEAbId:  
  for (
  iAbsId = 0; /* kc 1 to 0 on 4/6/ */
  iAbsId < RCro01_MaxAbstrSyn;
  iAbsId++) {
    /*search the given object identifier */
    if (ro0003_objcmp (
          pEntContext->vAbstrSyn [iAbsId].oAbstrSynName,
          pstAbsNameId->oAbsName) == OK) break;
  }   
       break; 
 /* case end */

case RC04zQ_EXIFAbId: 
  if ( hContext) {                                     
   RM0431_LocLock(hContext,pContext,rT0430_stContext)
  for (
  iAbsId = 0;   /* kc 1 to 0 on 4/6/ */
  iAbsId < RCro01_MaxAbstrSyn;
  iAbsId++) {
    /*search first AbsId not having the given object identifier
      (i.e. acse obj identifier) */

    if (ro0003_objcmp (
          pEntContext->vAbstrSyn [iAbsId].oAbstrSynName,
          pstAbsNameId->oAbsName) != OK) {
            if ( ( pEntContext->vAbstrSyn [iAbsId].oAbstrSynName[0] >= 3)
               &&(pEntContext->vAbstrSyn [iAbsId].oAbstrSynName[0] <=
                     RCro00_MaxObjIdComp                                ))
              if ( pContext->vAbstrSynResult[iAbsId] == RCro6c_Acceptance )
                 break; 
  /* K02              if ( ro0003_objcmp (
                              pEntContext->vTransSynName
              [pEntContext->vAbstrSyn [iAbsId].vTransSynId [1]],            
                              vBerTrfSyn) == OK ) break; */
             }
         }
       RM0432_LocUnLock(hContext) ;
      }
      else                                                                 
    pstAbsNameId->sAbsId = RCro01_MaxAbstrSyn; /*  means NotFound. */ 

       break; 
 /* case end */

} /* switch end */


  /* ***** unlock the entity invocation context */

  RM0432_LocUnLock (hEntContext)
  pstAbsNameId->sAbsId = iAbsId;

  RM0003_RETURN (fnctName, signed short int, rc)
  /*Note: "iAbsId == RCro01_MaxAbstrSyn" means NotFound. */
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0427_getDcs                                       */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE         : 93-02-23                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0427_getDcs (
	rT04z0_stDCS           FAR *pstDCS,
	unsigned long int           hContext, 
	rT04z0_stDCS           FAR *pRequestedInfo)
/*                                                                    */
/* DESCRIPTION  : Get a Defined Context Set .                         */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/* OUT-PARAMETERS :                                                   */
/*                                                                    */
/* RETURN VALUE   :                                                   */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

{/*entr*/

RM0001_STRG (fnctName, "ro0427_getDcs")
signed   short int          rc ;
rT0430_stContext            *pContext ;
/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (fnctName)

rc = 0;

if (!hContext )

  ro0041_memcpy((char FAR *)pRequestedInfo,
                (char FAR *)pstDCS,
                (unsigned long int)sizeof(rT04z0_stDCS));

else

{
  RM0431_LocLock (hContext, pContext, rT0430_stContext);
   /* get DCS from PPM Context */
   
    ro0041_memcpy((char FAR *)pRequestedInfo->vAbstrSynId, 
                  (char FAR *)pContext->vAbstrSynId, 
                  (unsigned long int)sizeof(pContext->vAbstrSynId));
    ro0041_memcpy((char FAR *)pRequestedInfo->vTransSynId, 
                  (char FAR *)pContext->vTransSynId, 
                  (unsigned long int)sizeof(pContext->vTransSynId));
    ro0041_memcpy((char FAR *)pRequestedInfo->vPresContId, 
                  (char FAR *)pContext->vPresContId, 
                  (unsigned long int)sizeof(pContext->vPresContId));
    ro0041_memcpy((char FAR *)pRequestedInfo->vAbstrSynResult, 
                  (char FAR *)pContext->vAbstrSynResult,
                  (unsigned long int)sizeof(pContext->vAbstrSynResult));              

    pRequestedInfo->pcnum = pContext->sDcsNumb;
    

    RM0432_LocUnLock (hContext)

}
 
 RM0003_RETURN (fnctName, signed short int, rc)


}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0428_findAbsId                                      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-06-04                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int PASCAL ro0428_findAbsId ( 
	rT04z2_stAbsIdFind FAR *pstAbsFnd)



/*                                                                    */
/* DESCRIPTION  :   Searches the Presentation Context for AbsId       */
/*                  match.                                            */
/*                                                                    */
/* IN-PARAMETERS              : AbsId                                 */
/*                                                                    */
/* OUT-PARAMETERS             : Boolean Found or Not                  */
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

RM0001_STRG (fnctName, "ro0428_findAbsId")
signed   short int      rc = OK;
unsigned long  int      hEntContext;
rT0441_stEntContext    *pEntContext;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (fnctName)
  pstAbsFnd->bFound = FALSE;
  rc = ro0215_getEiCxt ( RC0232_PresentationEiCxt,
                            &hEntContext);
  if (rc < 0)
    RM0003_RETURN (fnctName, signed short int, rc)
  if (rc > 0)
    RM0003_RETURN (fnctName, signed short int, -R_PENTCX)

  RM0431_LocLock (hEntContext, pEntContext, rT0441_stEntContext);
  if (   (pEntContext->vAbstrSyn [pstAbsFnd->sAbsId].oAbstrSynName[0] >= 3 )
      && (pEntContext->vAbstrSyn [pstAbsFnd->sAbsId].oAbstrSynName[0] <=
            RCro00_MaxObjIdComp                                ) )
       pstAbsFnd->bFound = TRUE;
             
  RM0432_LocUnLock (hEntContext)
RM0003_RETURN (fnctName, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         :                                                       */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-06-28                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0429_getMode ( 
	unsigned long int        hContext,
	rT04z3_stMode     FAR *  pMode)



/*                                                                    */
/* DESCRIPTION  : gets the mode from the ppm Context                  */
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
RM0001_STRG (fnctName, "ro0429_getMode")
signed   short int      rc = OK;
rT0430_stContext            *pContext ;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (fnctName)
  if ( hContext) {
      RM0431_LocLock (hContext, pContext, rT0430_stContext);
      pMode->sMode =  (signed short int )pContext->bNormalMode;
      RM0432_LocUnLock (hContext)
  } 
RM0003_RETURN (fnctName, signed short int, rc)                                    
}/*end*/   
/*exon ****************************************************************/  
/*                                                                    */  
/* TYPE         : C-FUNCTION                                          */  
/*                                                                    */  
/* NAME         : ro042a_indefDecset                                  */  
/*                                                                    */  
/* AUTHOR       : Baiju / Amit                                        */  
/* DATE         : 94-04-05                                            */  
/*                                                                    */  
/* SYNTAX       :                                                     */  
                                                                          
signed    short  int      ro042a_indefDecset( void       *pContext)       
                                                                          
/*                                                                    */  
/* DESCRIPTION  :                                                     */  
/*                                                                    */  
/*inoff ***************************************************************/  
/* ****************** l o c a l    v a r i a b l e s **************** */     
/* ******************                                **************** */     
{/*entr*/                                                                    
rT0430_stContext *pContext_loc;                                              
                                                                             
RM0001_STRG (fnctName, "ro042a_indefDecset")                                 
/* ****************** S T A R T ************************************* */     
/* ******************           ************************************* */     
                                                                             
  RM0002_ENTRY (fnctName)                                                    
                                                                             
        if (pContext != NULL) {                                              
                pContext_loc = (rT0430_stContext *)pContext;                 
                pContext_loc->lIndefCount = 0;                               
                RM0003_RETURN (fnctName, signed short int, OK)               
        }                                                                    
        else                                                                 
                RM0003_RETURN (fnctName, signed short int, ERROR)            
}
                                                                     
/*exon ****************************************************************/     
/*                                                                    */ 
/* TYPE         : C-FUNCTION                                          */ 
/*                                                                    */ 
/* NAME         : ro042b_indefDecget                                  */ 
/*                                                                    */ 
/* AUTHOR       : Baiju / Amit                                        */ 
/* DATE         : 94-04-05                                            */ 
/*                                                                    */ 
/* SYNTAX       :                                                     */ 
signed long int ro042b_indefDecget( void       *pContext)       
/*                                                                    */ 
/* DESCRIPTION  :                                                     */ 
/*                                                                    */ 
/*                                                                    */ 
/*inoff ***************************************************************/ 
                                                                         
/* ****************** l o c a l    v a r i a b l e s **************** */ 
/* ******************                                **************** */ 
{/*entr*/                                                                
rT0430_stContext *pContext_loc;                                          
                                                                         
RM0001_STRG (fnctName, "ro042b_indefDecget")                             
                                                                         
/* ****************** S T A R T ************************************* */ 
/* ******************           ************************************* */ 
                                                                         
  RM0002_ENTRY (fnctName)                                                
                                                                         
                                                                         
        pContext_loc = (rT0430_stContext *)pContext;                     
        if (pContext != NULL)                                            
                RM0003_RETURN (fnctName, signed long int,                
                                         pContext_loc->lIndefCount)      
        else                                                             
                RM0003_RETURN (fnctName, signed long int, -1)            

}
/*exon ****************************************************************/ 
/*                                                                    */ 
/* TYPE         : C-FUNCTION                                          */ 
/*                                                                    */   
/* NAME         : ro042c_indefDecinc                                  */   
/*                                                                    */   
/* AUTHOR       : Baiju / Amit                                        */   
/* DATE         : 94-04-05                                            */   
/*                                                                    */   
/* SYNTAX       :                                                     */   
                                                                           
signed    short  int      ro042c_indefDecinc( void      *pContext)         
                                                                           
/*                                                                    */   
/* DESCRIPTION  :                                                     */   
/*                                                                    */   
/*                                                                    */   
/*inoff ***************************************************************/   
                                                                           
/* ****************** l o c a l    v a r i a b l e s **************** */   
/* ******************                                **************** */   
{/*entr*/                                                                  
rT0430_stContext *pContext_loc;                                            
RM0001_STRG (fnctName, "ro042c_indefDecinc")                               
                                                                           
/* ****************** S T A R T ************************************* */   
/* ******************           ************************************* */   
                                                                           
  RM0002_ENTRY (fnctName)                                                  
                                                                           
                                                                           
        pContext_loc = (rT0430_stContext *)pContext;                       
        if (pContext != NULL) {                                            
                pContext_loc->lIndefCount++;                               
                RM0003_RETURN (fnctName, signed short int, OK)             
        }                                                                  
        else                                                               
                RM0003_RETURN (fnctName, signed short int, ERROR)          
}/*end */                                                                  
                                                                        
                                                                          
/* EOF */
