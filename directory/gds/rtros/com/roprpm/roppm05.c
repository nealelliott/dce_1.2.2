/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm05.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:45  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:13:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:59  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:02  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:12  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:08  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:51:54  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:42  keutel
 * 	creation
 * 	[1994/03/21  13:21:08  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roppm05.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:50 $" } ;
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
/* NAME         : roppm05.c                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-06-29                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Presentation Protocol Automata (PPA)                */
/*                Connection establishment transaction functions      */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-03-24|  adaptation of ROS V2.5A00 K05 |hm  |         */
/* 2.0A00 K01|92-04-24|  change of objcpy-RC-evaluation|hm  |         */
/*           |        |  (to get correct abort reasons)|    |         */
/* 2.0A00 K02|92-06-29|  SCCS-Id changed;              |hm  |         */
/*           |        |  P-selector evaluat. corrected;|    |         */
/*           |        |  if issue CPR after CP receipt:|    |         */
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

static char *sccsid =    "@(#) roppm05.c 1.2 93/12/20   RTS-V2.00";


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
#include <rouply0.h>      /* Common type and value definitions        */
#include <rouply1.h>      /* PM service interface structures          */
#include <routil0.h>      /* Utility interface (common functions)     */
#include <routil1.h>      /* Utility interface (object id. functions) */
#include <rodpat0.h>      /* Dispatcher interface                     */
#include <rouctr4.h>      /* Ae invocation                            */
#include <rospm00.h>      /* SPM service interface                    */
#include <roppm00.h>      /* PPM service interface                    */
#include <roppm01.h>      /* PPM protocol interface                   */
#include <roppm04.h>      /* PPM entity invoc. context (before 03.h!) */
#include <roppm03.h>      /* PPM logging interface                    */
#include <roppm10.h>      /* PPM intern: macros                       */
#include <roppm11.h>      /* PPM intern: types and constants          */
#include <roppmz0.h>      /* PPM intern: function prototyping         */
#include <roppm12.h>      /* PPM intern: function prototyping         */
#include <roppm14.h>      /* PPM intern: function prototyping         */


/**********************************************************************/
/*                                                                    */
/*              Static Function Declaration                           */
/*                                                                    */
/**********************************************************************/

/* ****************** Service parameter checking sub-function, ****** */
/* ****************** called from the PPA functions for:       ****** */
/* ******************              - P-CONNECT response pos.   ****** */
/* ******************              - P-CONNECT response neg.   ****** */

static signed short int PASCAL ro0457_checkConnRsp PROTO ((
   rT0430_stContext            *pContext,
   rT04a0_unConn           FAR *pConn));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value).

   - pContext          : <-> PPM context (pointer to).
   - pConn             : <-> Parameter P3 structure (pointer to).
*/


/* ****************** Presentation context handling sub-function, *** */
/* ****************** called from the PPA functions for:          *** */
/* ******************              - P-CONNECT response pos.      *** */
/* ******************              - P-CONNECT response neg.      *** */

static signed short int PASCAL ro0458_syntaxCpaCprSend PROTO ((
   rT0430_stContext            *pContext,
   rT0441_stEntContext         *pEntContext,
   rT04a0_unConn           FAR *pConn,
   signed   long  int      FAR *pbPresContDefRes,
   unsigned long  int      FAR *pulPresContDefRes,
   rT0413_stPresContDefRes FAR *pPresContDefRes));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value).

   - pContext          : <-> PPM context (pointer to).
   - pEntContext       : --> Pres. entity invocation context (pointer to).
   - pConn             : --> Parameter P3 structure (pointer to).
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
/* NAME         : ro0450_PCONreq                                      */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-04-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

/* RM0400_ParList (ro0450_PCONreq) P!? */
signed short int PASCAL ro0450_PCONreq (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/


/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0450_PCONreq")
rT0412_stPresContDef FAR *pAux;      /*aux. pointer to a context list element */
signed         int        iNumb;     /*number of abstract syntaxes */
signed         int        iAbsId;    /*abstract syntax identifier */
signed   long  int        lContId;   /*presentation context identifier */
signed         int        iInx;      /*index in "for"-loop */
signed         int        iTrsInx;   /*index in "for"-loop (transfer syntaxes) */
signed   short int        rc;        /*returncode of a called function */

rTro00_vObjectId          vBerTrfSyn = {3,2,1,1} ;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*----------------*/
  /* X410-mode only */
  /*----------------*/

  if (pConn->stReq.stAll.lModeSelector == RCro70_X400Mode1984)
    *pX410RtpmNode = (unsigned long int FAR *)&pNode->stCp.hX410RtpmNode;


  /*------------------*/
  /* Normal-mode only */
  /*------------------*/

  else
  {
    pContext->bNormalMode = TRUE;

    /*--------------------------*/
    /* Check service parameters */
    /*--------------------------*/

    if (pConn->stReq.stPpm.ulRequirements != RCro60_Kernel)
      RM0003_RETURN (fnctName, signed short int, -R_PFUNCU)
      /*here better abort P!? */


    /*------------------------------------*/
    /* Process simple protocol parameters */
    /*------------------------------------*/

    pNode->stCp.bNormalSEQUENCE   = TRUE;

    pNode->stCp.ulProtVersion     = 1;              /*#define P!? */
    pNode->stCp.bsProtVersion     = RC0419_ProtVersion1;

    pNode->stCp.bPresRequirements = FALSE;          /*comment S!? */

    pNode->stCp.bSessRequirements = FALSE;          /*comment S!? */


    /*------------------------------------------------------*/
    /* Read calling P-selector from local address structure */
    /*------------------------------------------------------*/
    
    rc = ro0217_getAddress (
      NULL,                         /* --> Memory handle to the whole PSAP. */
      RCro11_Pselector,          /* --> PM number.                       */
      RCro1a_MaxLenPSelector,       /* --> Max. length of P-selector.       */
      pNode->stCp.vCallingSelector  /* <-- P-selector (pointer to).         */
    );

    if (rc < 0)
      RM0003_RETURN (fnctName, signed short int, rc)
      /*no AddressLengthError possible P!? (has been checked in EXIF P!?) */
    if (rc > 0)
    {
      pNode->stCp.bCallingSelector  = TRUE;
      pNode->stCp.ulCallingSelector = (unsigned long int) rc;
    }


    /*------------------------------------------------------*/
    /* Read called P-selector from remote address structure */
    /*------------------------------------------------------*/
    
    rc = ro0217_getAddress (
      pConn->stReq.stAll.hCalledAddress, /* --> Memory handle to the whole PSAP. */
      RCro11_Pselector,               /* --> PM number.                       */
      RCro1a_MaxLenPSelector,            /* --> Max. length of P-selector.       */
      pNode->stCp.vCalledSelector        /* <-- P-selector (pointer to).         */
    );

    if (rc < 0)
      RM0003_RETURN (fnctName, signed short int, rc)
      /*no AddressLengthError possible P!? (has been checked in EXIF P!?) */
    if (rc > 0)
    {
      pNode->stCp.bCalledSelector  = TRUE;
      pNode->stCp.ulCalledSelector = (unsigned long int) rc;
    }
                     
    /*--------------------------------------------------*/
    /* Process the presentation-context-definition-list */
    /*--------------------------------------------------*/

    /* ***** process the number of elements */

    iNumb                         = pConn->stReq.stPpm.vAbstrSynId [0];
    pContext->sDcsNumb            = 
    pContext->vAbstrSynId     [0] = /*redundant P!? */
    pContext->vAbstrSynResult [0] = (signed short int) iNumb;

    if (iNumb)
    {
      pNode->stCp.bPresContDef  = TRUE;
      pNode->stCp.ulPresContDef = (unsigned long int) iNumb;
    }


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


    /* ***** compute PPM context and PDV node */

    for (
    iInx     = 1,
    pAux     = pNode->stCp.vPresContDef,
    lContId  = 1;
    iInx    <= iNumb;
    iInx++,
    pAux++,
    lContId += 2)
    {
      /* ** presentation context identifier and PPM context lists */

      iAbsId = (signed int) pConn->stReq.stPpm.vAbstrSynId [iInx];
      pContext->vAbstrSynId [iInx] = (signed short int) iAbsId;

      /*Temporary till P-CONNECTcnf (used in E/D Control during */
      /*the encoding of the CP or ARU PPDU and for the presen-  */
      /*tation-context-identifier-list of the ARU PPDU).        */
      /*Advantage: Centralized setting of favourized transfer   */
      /*syntax, not in a number of functions.                   */
      /*Would it be better to look if BER are proposed S!?      */
      /*But: then less user influence by sequence of attach()-  */
      /*TransSyn's.                                             */

      pContext->vTransSynId [iAbsId] = ro0425_getTransId (                
                  pEntContext,   
                  iAbsId,        
                   vBerTrfSyn);
      
      if ( pContext->vTransSynId [iAbsId] ==  RC043O_NoTransId)
                         
      pContext->vTransSynId [iAbsId] = pEntContext->vAbstrSyn [iAbsId].
                                       vTransSynId [1];

      pContext->vPresContId [iAbsId] =
      pAux->lPresContId              = lContId;


      /* ** abstract syntax name */

      rc = ro0002_objcpy (
        pAux->oAbstrSynName,                            /* <-- Target. */
        pEntContext->vAbstrSyn [iAbsId].oAbstrSynName); /* --> Source. */

      if (rc == R_LENULL)
        RM0003_RETURN (fnctName, signed short int, RC040I_WrongAbstrSynId)
        /*attach: EXIF has ensured correct object id. in pres. ent. context */
        /*--> error caused by connect parameters --> local abort reason     */
      if (rc)                                  /*rc never > 0 */
        RM0003_RETURN (fnctName, signed short int, rc)


      /* ** transfer syntax number and name(s) */

      pAux->ulTransSynNumber = (unsigned long int) pEntContext->
                               vAbstrSyn [iAbsId].vTransSynId [0];
      for (
      iTrsInx  = 1;
      iTrsInx <= (signed int) pAux->ulTransSynNumber;
      iTrsInx++)
      {
        rc = ro0002_objcpy (
          pAux->vTransSynName [iTrsInx - 1],     /* <-- Target. */
          pEntContext->vTransSynName             /* --> Source. */
           [pEntContext->vAbstrSyn [iAbsId].vTransSynId [iTrsInx]]);

        if (rc > 0)                              /* because R_LENULL */
          rc = -rc;
        if (rc)                                  /* rc now never > 0 */
          RM0003_RETURN (fnctName, signed short int, rc)
          /*attach: EXIF has ensured correct object identifiers and correct */
          /*transfer-syntax-per-abstract-syntax-list in pres. ent. context  */

      } /*to "for (iTrsInx...)" */
    }   /*to "for (iInx...)" */


    /*--------------------------------------*/
    /* Process default presentation context */
    /*--------------------------------------*/

    iAbsId = (signed int) pConn->stReq.stPpm.sDefAbstrSynId;
                                        /*iAbsId because better readability P!? */
    if (iAbsId != RCro6i_NoDefContext)
    {
      pNode->stCp.bDefaultContext = TRUE;


      /* ** default abstract syntax name */

      rc = ro0002_objcpy (
        pNode->stCp.oDefaultAbstrSyn,                   /* <-- Target. */
        pEntContext->vAbstrSyn [iAbsId].oAbstrSynName); /* --> Source. */

      if (rc == R_LENULL)
        RM0003_RETURN (fnctName, signed short int, RC040J_WrongDefAbstrSynId)
        /*attach: EXIF has ensured correct object id. in pres. ent. context */
        /*--> error caused by connect parameters --> local abort reason     */
      if (rc)                                  /*rc never > 0 */
        RM0003_RETURN (fnctName, signed short int, rc)


      /* ** default transfer syntax name */

      rc = ro0002_objcpy (
        pNode->stCp.oDefaultTransSyn,          /* <-- Target. */
        pEntContext->vTransSynName             /* --> Source. */
         [pConn->stReq.stPpm.sDefTransSynId]);
         /*better iTransId because better readability P!? */

      if (rc == R_LENULL)
        RM0003_RETURN (fnctName, signed short int, RC040K_WrongDefTransSynId)
        /*attach: EXIF has ensured correct object id. in pres. ent. context */
        /*--> error caused by connect parameters --> local abort reason     */
      if (rc)                                  /*rc never > 0 */
        RM0003_RETURN (fnctName, signed short int, rc)


      /* ** initialize PPM context */

      pContext->sDefContResult = RC043N_NoSynResult; /*P!? */
      pContext->sDefAbstrSynId = iAbsId;
      pContext->sDefTransSynId = pConn->stReq.stPpm.sDefTransSynId;
    }
    else
    {
      pContext->sDefContResult = /*one of them redundant P!? */
      pContext->sDefAbstrSynId = RCro6i_NoDefContext;

      /*Note: pNode->stCp.bDefaultContext is 0-initialized. */
    }


    /*----------------------*/
    /* Process PS-user data */
    /*----------------------*/

    *pbUserData = (signed long int   FAR *)&pNode->stCp.bUserData;
    *pUserData  = (rT0417_stUserData FAR *)&pNode->stCp.stUserData;

  } /*to "if (!X410-mode)" */


  /*---------------------------*/
  /* X410-mode and normal-mode */
  /*---------------------------*/

  pContext->sState            = RC0431_StaI1;
  pContext->bRelease          = FALSE;
  pContext->bReleaseCollision = FALSE;
  pContext->sSessProtVers     = pConn->stReq.stAll.sSessProtVers;

  pNode->stCp.lModeSelector   = pConn->stReq.stAll.lModeSelector;

  *pOutEvent = RC043q_OEvCP;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0451_CP                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-06-29                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0451_CP (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/


/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0451_CP")
signed         int        iProviderReason; /*provider reject reason (CPR) */
unsigned long  int        hConnNew;  /*memory handle to allocated P3 struct (CPR) */
rTro6g_stConnRspPpm  FAR *pConnNew;  /*pointer to allocated P3 struct (CPR) */
signed         int        iAbsId;    /*abstract syntax identifier */
signed         int        iInx;      /*index in "for"-loop */
char                      vOwnSelector [RCro1a_MaxLenPSelector]; /*own P-selector */
signed   short int        rc,sLen;    /*returncode of a called function */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /* ** initialize local variables */

  iProviderReason = RC043Q_NoReject;


  /*----------------*/
  /* X410-mode only */
  /*----------------*/

  if (pNode->stCp.lModeSelector == RCro70_X400Mode1984)
    *pX410RtpmNode = (unsigned long int FAR *)&pNode->stCp.hX410RtpmNode;


  /*------------------*/
  /* Normal-mode only */
  /*------------------*/

  else
  {
    /*--------------------------------------*/
    /* Check protocol parameters for abort  */
    /* (allowed after CP (or issue CPR S!?) */
    /*--------------------------------------*/

    if (!pNode->stCp.bNormalSEQUENCE)
      RM0003_RETURN (fnctName, signed short int, RCro6l_UnrecognizedPpduParam)
      /*right value S!? further checks S!? */


    /*------------------------------------------*/
    /* Check simple protocol parameters for CPR */
    /*------------------------------------------*/

    /*sequence: protocol version -> P-selector -> other S!? */
    /*no check of: - pNode->stCpa.bPresRequirements         */
    /*             - pNode->stCpa.bSessRequirements         */
    /* because of "Rules of extensibility" (8823, 8.5) S!?  */

    if ( !(pNode->stCp.bsProtVersion & RC0419_ProtVersion1) )
      iProviderReason = RC041g_ProtVersionNotSuppo;


    /*----------------------------------------------------------*/
    /* Read the own P-selector from local address structure and */
    /* compare with the called P-selector                       */
    /*----------------------------------------------------------*/
    
    if (!pNode->stCp.bCalledSelector)
      pNode->stCp.ulCalledSelector = NULL;

    sLen = ro0217_getAddress (
      NULL,                         /* --> Memory handle to the whole PSAP. */
      RCro11_Pselector,          /* --> PM number.                       */
      sizeof (vOwnSelector),        /* --> Max. length of P-selector.       */
      vOwnSelector                  /* <-- P-selector (pointer to).         */
    );

    if (sLen < 0)
      RM0003_RETURN (fnctName, signed short int, rc)
      /*no AddressLengthError possible P!? (has been checked in EXIF P!?) */

    /*Note: if (sLen >= 0): rc = length of own P-selector. */
    /* checking is done only if the AE Title NOCHK is not set */
    /* by kc for ROS2.6 on 8-12-93 */
    {
     unsigned long int          hAeInv=LNULL;
     rT0501_AEInvocation NEAR * pAeInv=LNULL;  
     rTro72_unAeTitle    NEAR * pAeTitle1=LNULL;

     rc = ro0215_getEiCxt (RC0233_ApplicationEiCxt,  
                      &hAeInv            );
     if (! rc) {
    /* --------- lock the AEInvocation --------------------------------- */  
    pAeInv = (rT0501_AEInvocation NEAR *) ro0031_locallock ( hAeInv);        
    if (! pAeInv) RM0003_RETURN (rs0006, signed short int, -R_LLOCKE)
    pAeTitle1    = (rTro72_unAeTitle NEAR *) ro0031_locallock (
            pAeInv->stOwnAeTitle.hHanOfSub[0]);     
    if (! pAeTitle1)     RM0003_RETURN (rs0006, signed short int, -R_LLOCKE)   
    if (!(pAeTitle1->lAeTitleForm & 0xff00 )) { /* No checking */ 
      if (ro0045_memAndLenCmp (
          vOwnSelector,                  /* --> Byte string 1.           */
          pNode->stCp.vCalledSelector,   /* --> Byte string 2.           */
          (unsigned long int) sLen,        /* --> Length of byte string 1. */
          pNode->stCp.ulCalledSelector)  /* --> Length of byte string 2. */
       )
        iProviderReason = RC041f_CalledPaddressUnknown;
     }
     if (ro0032_localunlock (pAeInv->stOwnAeTitle.hHanOfSub[0])) 
        RM0003_RETURN (rs0006, signed short int, -R_LULOKE)
     if (ro0032_localunlock (hAeInv)) 
        RM0003_RETURN (rs0006, signed short int, -R_LULOKE)
    }
   } /* end of 08-12-93 change for Called address */
    /*--------------------------------------------------------*/
    /* Write calling P-selector into remote address structure */
    /*--------------------------------------------------------*/
    
    if (pNode->stCp.bCallingSelector)
    {
      rc = ro0218_conAddress (
        &pConn->stInd.stAll.hCallingAddress, /* <-> Memory handle to the whole PSAP. */
        RCro11_Pselector,                 /* --> PM number.                       */
        (unsigned short int) pNode->stCp.ulCallingSelector,
                                             /* --> Length of P-selector.            */
        pNode->stCp.vCallingSelector         /* --> P-selector (pointer to).         */
      );
      /*Note: memory handle is changed if it was NULL */

      if (rc < 0)
        RM0003_RETURN (fnctName, signed short int, rc)
        /*Note: length of calling P-selector already checked in parse() P!? */
    }

    /*--------------------------------------------------------*/
    /* Write called P-selector into local address structure */
    /*--------------------------------------------------------*/
    
    if (pNode->stCp.bCalledSelector)
    {
      if (pConn->stInd.stAll.hCalledAddress == NULL)
       if ((pConn->stInd.stAll.hCalledAddress 
                  = ro0030_localalloc (RCro1e_MaxLenPSAP)) == 0)
           RM0003_RETURN(functName, signed short int, -R_LALLCE)            

      rc = ro0218_conAddress (
        &pConn->stInd.stAll.hCalledAddress, /* <-> Memory handle to the whole PSAP. */
        RCro11_Pselector,                 /* --> PM number.                       */
        (unsigned short int) pNode->stCp.ulCalledSelector,
                                             /* --> Length of P-selector.            */
        pNode->stCp.vCalledSelector         /* --> P-selector (pointer to).         */
      );
      /*Note: memory handle is changed if it was NULL */

      if (rc < 0)
        RM0003_RETURN (fnctName, signed short int, rc)
    }


    /*--------------------------------------------------------*/
    /* Force CPR if necessary (issue an event for the own PM) */
    /*--------------------------------------------------------*/
    
    if (iProviderReason == RC043Q_NoReject)   /*comment P!? */
      iProviderReason = (signed int) pContext->lProviderReason;
    if (iProviderReason != RC043Q_NoReject)      
    {
      /*-----------------------------------*/
      /* Alloc and fill a new P3 structure */
      /*-----------------------------------*/

      RM0430_LocAlloc (hConnNew, sizeof (rTro6g_stConnRspPpm))
      RM0431_LocLock  (hConnNew, pConnNew, rTro6g_stConnRspPpm)

      /* ***** stAll */

      pConnNew->stAll.hRespondAddress = NULL;  /*0-initialzing P!? only comment !? */
      pConnNew->stAll.sSessProtVers   = pConn->stInd.stAll.sSessProtVers;


      /* ***** stSpm */

      ro0041_memcpy (
        (char FAR *) &pConnNew->stSpm,                 /* <-- Target. */
        (char FAR *) &pConn->stInd.stSpm,              /* --> Source. */
        (unsigned long int)sizeof (pConnNew->stSpm));  /* --> Length. */

      /*change of session connection identifier (CONNECTrsp requires */
      /*the called SS-user reference)                                */
      pConnNew->stSpm.stConnId.ulCalledSSuserRef  =
      pConnNew->stSpm.stConnId.ulCallingSSuserRef;
      pConnNew->stSpm.stConnId.ulCallingSSuserRef = 0;
      ro0041_memcpy (
        (char FAR *) pConnNew->stSpm.stConnId.vCalledSSuserRef, /* <-- Target */
        (char FAR *) pConnNew->stSpm.stConnId.vCallingSSuserRef,/*  --> Source*/
        (unsigned long int)sizeof (pConnNew->stSpm.stConnId.vCalledSSuserRef));
                                                                /* --> Length */


      /* ***** stPpm */
      
      for (iInx = 0; iInx <= RCro04_MaxPresCont; iInx++)
        pConnNew->stPpm.vAbstrSynResult [iInx] = pContext->vAbstrSynResult [iInx];

      pConnNew->stPpm.sDefContResult = pContext->sDefContResult;

      pConnNew->stPpm.ulRequirements = RCro60_Kernel;


      RM0432_LocUnLock (hConnNew)


      /*----------------------------------------------------------------*/
      /* Process PPM context (pConn -> before release of old resources) */
      /*----------------------------------------------------------------*/

      pContext->sState            = RC0432_StaI2;
      pContext->sSessProtVers     = pConn->stInd.stAll.sSessProtVers;


      /*-----------------------*/
      /* Release old resources */
      /*-----------------------*/

      RM0422_GlobUnLock (*phNode)        /*must be unlocked in clearNodePpm() */
      rc = ro0246_clearPdvTree (phNode); /*sets *phNode to 0 */
      if (rc)
        RM0003_RETURN (fnctName, signed short int, rc)

      if (pConn->stInd.stAll.hCallingAddress)
        RM0434_LocFree (pConn->stInd.stAll.hCallingAddress)
      if (pConn->stInd.stAll.hCalledAddress)
        RM0434_LocFree (pConn->stInd.stAll.hCalledAddress)
      RM0432_LocUnLock (*pP3)
      RM0434_LocFree   (*pP3)            /*sets *pP3 to 0 */


      /*----------------------------*/
      /* Issue event for the own PM */
      /*----------------------------*/

      /*----------------------------*/
      /* Issue event for the own PM */
      /*----------------------------*/

      rc = ro0211_pushOutEvent (
        RC0215_Presentation,    /* --> PM number.                           */
        RC0403_PCONNECTrsp,     /* --> Event number.                        */
        NULL,                   /* --> Event parameter P1 (PS-user data).   */
        NULL,                   /* --> Event parameter P2.                  */
        hConnNew,               /* --> Event parameter P3.                  */
        RC0400_PpmEvent + iProviderReason);
                                /* --> Event parameter P4 (connect result): */
                                /*     protocol machine basic event number  */
                                /*     + provider reason (ISO 8823).        */
                                /*     PPM expects this value.              */
                                /*     LOIF expects RCro81_UserReject       */
                                /*     (will be set in ro0456_PCONrspNeg()).*/

      if (rc)                   /*rc never > 0 */
        RM0003_RETURN (fnctName, signed short int, rc)

      *pOutEvent = RC0403_PCONNECTrsp;  /*used as flag in ro0400_ppm() */

      RM0003_RETURN (fnctName, signed short int, OK)

    } /*to "if (iProviderReason)" */
    else


    /*-----------------------------------------*/
    /* Process successful P-CONNECT indication */
    /*-----------------------------------------*/

    {
      /* ***** Process DCS related P3 information */

      pConn->stInd.stPpm.vPresContId     [0] = (signed long int)
                                               pContext->vAbstrSynResult [0];
      pConn->stInd.stPpm.vAbstrSynResult [0] =
      pConn->stInd.stPpm.vAbstrSynId     [0] =
      pConn->stInd.stPpm.vTransSynId     [0] = pContext->vAbstrSynResult [0];

      for (
      iInx  = 1;
      iInx <= pContext->vAbstrSynResult  [0];
      iInx++)
      {
        pConn->stInd.stPpm.vAbstrSynResult [iInx] =
        pContext->vAbstrSynResult [iInx];

        if (pContext->vAbstrSynResult [iInx] == RCro6c_Acceptance)
        {
          iAbsId = (signed int)
          pContext->vAbstrSynId [iInx];

          pConn->stInd.stPpm.vAbstrSynId [iInx] =
          pContext->vAbstrSynId [iInx];

          pConn->stInd.stPpm.vTransSynId [iInx] =
          pContext->vTransSynId [iAbsId];

          pConn->stInd.stPpm.vPresContId [iInx] =
          pContext->vPresContId [iAbsId];
        }
      }

      /* ***** Process other P3 information */

      /*default context -> valid if no CPR issued */
      pConn->stInd.stPpm.sDefAbstrSynId = pContext->sDefAbstrSynId;
      pConn->stInd.stPpm.sDefTransSynId = pContext->sDefTransSynId;

      pConn->stInd.stPpm.ulRequirements = RCro60_Kernel;


      /*----------------------*/
      /* Process PS-user data */
      /*----------------------*/

      *pbUserData = (signed long int    FAR *)&pNode->stCp.bUserData;
      *pUserData  = (rT0417_stUserData  FAR *)&pNode->stCp.stUserData;

    } /*to "if (iProviderReason) else " */

  } /*to "if (!X410-mode)" */


  /*---------------------------*/
  /* X410-mode and normal-mode */
  /*---------------------------*/

  pContext->sState            = RC0432_StaI2;
  pContext->bRelease          = FALSE;
  pContext->bReleaseCollision = FALSE;
  pContext->sSessProtVers     = pConn->stInd.stAll.sSessProtVers;
/* mem. has to be copied. by kc on 10/6  */

/*  pContext->ulCp              = *phNode;  */

/* Not required. Taken care of in dataToUser that CP not freed.
  {
    rT041G_unPpdu  FAR * p  = NULL;
    RM0420_GlobAlloc (pContext->ulCp , sizeof(rT041G_unPpdu))
    RM0421_GlobLock (pContext->ulCp, p ,rT041G_unPpdu )
    ro0041_memcpy   ( (char FAR *)p ,  (char FAR *) pNode , 
                      (unsigned long int ) sizeof(rT041G_unPpdu) );
    RM0422_GlobUnLock(pContext->ulCp)
  } 
*/

  pConn->stInd.stAll.lModeSelector = pNode->stCp.lModeSelector;

  *pOutEvent = RC043t_OEvPCONind;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0452_CPA                                          */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0452_CPA (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/


/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0452_CPA")
signed         int        iAbsId;    /*abstract syntax identifier */
signed         int        iInx;      /*index in "for"-loop */
signed   short int        rc;        /*returncode of a called function */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*----------------*/
  /* X410-mode only */
  /*----------------*/

  if (!pContext->bNormalMode)
  {
    if (pNode->stCpa.lModeSelector != RCro70_X400Mode1984)
      RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)
      /*right value S!? */

    *pX410RtpmNode = (unsigned long int FAR *)&pNode->stCpa.hX410RtpmNode;
  }


  /*------------------*/
  /* Normal-mode only */
  /*------------------*/

  else
  {
    /*-------------------------------------*/
    /* Check protocol parameters for abort */
    /*-------------------------------------*/

    if (pNode->stCpa.lModeSelector != RCro71_NormalMode)
      RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)
      /*right value S!? */

    if (!pNode->stCpa.bNormalSEQUENCE)
      RM0003_RETURN (fnctName, signed short int, RCro6l_UnrecognizedPpduParam)
      /*right value S!? */

    if ( !(pNode->stCpa.bsProtVersion & RC0419_ProtVersion1) )
      RM0003_RETURN (fnctName, signed short int, RCro6l_UnrecognizedPpduParam)
      /*right value S!? */
      /*CP: issue CPR */

    if (pNode->stCpa.bPresRequirements
    ||  pNode->stCpa.bSessRequirements
    )
      RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
      /*CP: not checked because ISO 8823 8.5.1 (rules of extensibily) */


    /*-----------------------------------------------------------*/
    /* Write responding P-selector into remote address structure */
    /*-----------------------------------------------------------*/
    
    if (pNode->stCpa.bRespondingSelector)
    {
      rc = ro0218_conAddress (
        &pConn->stCnf.stAll.hRespondAddress, /* <-> Memory handle to the whole PSAP. */
        RCro11_Pselector,                 /* --> PM number.                       */
        (unsigned short int) pNode->stCpa.ulRespondingSelector,
                                             /* --> Length of P-selector.            */
        pNode->stCpa.vRespondingSelector     /* --> P-selector (pointer to).         */
      );
      /*Note: memory handle is changed if it was NULL */

      if (rc < 0)
        RM0003_RETURN (fnctName, signed short int, rc)
        /*Note: length of calling P-selector already checked in parse() P!? */
    }


    /*-----------------------------------------*/
    /* Process successful P-CONNECT confirm+   */
    /*-----------------------------------------*/

    /* ***** Process DCS related P3 information */

    pConn->stCnf.stPpm.vPresContId     [0] = (signed long int)
                                             pContext->vAbstrSynResult [0];
    pConn->stCnf.stPpm.vAbstrSynResult [0] =
    pConn->stCnf.stPpm.vTransSynId     [0] = pContext->vAbstrSynResult [0];

    for (
    iInx  = 1;
    iInx <= pContext->vAbstrSynResult  [0];
    iInx++)
    {
      pConn->stCnf.stPpm.vAbstrSynResult [iInx] =
      pContext->vAbstrSynResult [iInx];

      if (pContext->vAbstrSynResult [iInx] == RCro6c_Acceptance)
      {
        iAbsId = (signed int)
        pContext->vAbstrSynId [iInx];

        pConn->stCnf.stPpm.vTransSynId [iInx] =
        pContext->vTransSynId [iAbsId];

        pConn->stCnf.stPpm.vPresContId [iInx] =
        pContext->vPresContId [iAbsId];
      }
    }

    /* ***** Process other P3 information */

    pConn->stCnf.stPpm.sDefContResult = pContext->sDefContResult;

    pConn->stCnf.stPpm.ulRequirements = RCro60_Kernel;


    /*----------------------*/
    /* Process PS-user data */
    /*----------------------*/

    *pbUserData = (signed long int    FAR *)&pNode->stCpa.bUserData;
    *pUserData  = (rT0417_stUserData  FAR *)&pNode->stCpa.stUserData;

  } /*to "if (!X410-mode)" */


  /*---------------------------*/
  /* X410-mode and normal-mode */
  /*---------------------------*/

  pContext->sState        = RC0433_Stat0;
  pContext->sSessProtVers = pConn->stCnf.stAll.sSessProtVers;

  *pOutEvent = RC043u_OEvPCONcnfPos;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0453_CPR                                          */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0453_CPR (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/


/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0453_CPR")
signed         int        iAbsId;    /*abstract syntax identifier */
signed         int        iInx;      /*index in "for"-loop */
signed   short int        rc;        /*returncode of a called function */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*----------------*/
  /* X410-mode only */
  /*----------------*/

  if (!pContext->bNormalMode)
  {
    if (pNode->stCpr.lX410orNormalChoice != RC041a_X410Mode) 
      RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)
      /*right value S!? */

    *pX410RtpmNode = (unsigned long int FAR *)&pNode->stCpr.hX410RtpmNode;
  }


  /*------------------*/
  /* Normal-mode only */
  /*------------------*/

  else
  {
    /*-------------------------------------*/
    /* Check protocol parameters for abort */
    /*-------------------------------------*/

    if (pNode->stCpr.lX410orNormalChoice != RC041b_NormalMode) 
      RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)
      /*right value S!? */

    if ( !(pNode->stCpr.bsProtVersion & RC0419_ProtVersion1) )
      RM0003_RETURN (fnctName, signed short int, RCro6l_UnrecognizedPpduParam)
      /*right value S!? */
      /*V2.5: not S!?   allowed by 8823 S!? */

    /*check the received provider reason if rejected by PPM */
    if (pNode->stCpr.bProviderReason)
      if (pNode->stCpr.lProviderReason < RC041c_ReasonNotSpecified
      ||  pNode->stCpr.lProviderReason > RC041j_NoPSAPavailable
      )
        RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)
        /*right value S!? */


    /*-----------------------------------------------------------*/
    /* Write responding P-selector into remote address structure */
    /*-----------------------------------------------------------*/
    
    if (pNode->stCpr.bRespondingSelector)
    {
      rc = ro0218_conAddress (
        &pConn->stCnf.stAll.hRespondAddress, /* <-> Memory handle to the whole PSAP. */
        RCro11_Pselector,                 /* --> PM number.                       */
        (unsigned short int) pNode->stCpr.ulRespondingSelector,
                                             /* --> Length of P-selector.            */
        pNode->stCpr.vRespondingSelector     /* --> P-selector (pointer to).         */
      );
      /*Note: memory handle is changed if it was NULL */

      if (rc < 0)
        RM0003_RETURN (fnctName, signed short int, rc)
        /*Note: length of calling P-selector already checked in parse() P!? */
    }


    /*----------------------------------------*/
    /* Process a correct P-CONNECT confirm-   */
    /*----------------------------------------*/

    /* ***** Process DCS related P3 information */

    pConn->stCnf.stPpm.vPresContId     [0] = (signed long int)
                                             pContext->vAbstrSynResult [0];
    pConn->stCnf.stPpm.vAbstrSynResult [0] =
    pConn->stCnf.stPpm.vTransSynId     [0] = pContext->vAbstrSynResult [0];
    
    for (
    iInx  = 1;
    iInx <= pContext->vAbstrSynResult  [0];
    iInx++)
    {
      pConn->stCnf.stPpm.vAbstrSynResult [iInx] =
      pContext->vAbstrSynResult [iInx];

      if (pContext->vAbstrSynResult [iInx] == RCro6c_Acceptance)
      {
        iAbsId = (signed int)
        pContext->vAbstrSynId [iInx];

        pConn->stCnf.stPpm.vTransSynId [iInx] =
        pContext->vTransSynId [iAbsId];

        pConn->stCnf.stPpm.vPresContId [iInx] =
        pContext->vPresContId [iAbsId];
      }
    }

    /* ***** Process other P3 information */

    pConn->stCnf.stPpm.sDefContResult = pContext->sDefContResult;

    pConn->stCnf.stPpm.ulRequirements = RCro60_Kernel;


    /* ***** Process P4 information (ConnectResult) */

    /*change ConnectResult from UserReject if rejected by PPM */
    if (pNode->stCpr.bProviderReason)
      *pP4 =   RC0400_PpmEvent               /*protocol machine event number */
             + pNode->stCpr.lProviderReason; /*provider reason (ISO 8823) */


    /*----------------------*/
    /* Process PS-user data */
    /*----------------------*/

    *pbUserData = (signed long int    FAR *)&pNode->stCpr.bUserData;
    *pUserData  = (rT0417_stUserData  FAR *)&pNode->stCpr.stUserData;

  } /*to "if (!X410-mode)" */


  /*---------------------------*/
  /* X410-mode and normal-mode */
  /*---------------------------*/

  pContext->sState        = RC0430_StaI0;

  *pOutEvent = RC043v_OEvPCONcnfNeg;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0454_SCONcnfNeg                                   */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0454_SCONcnfNeg (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/


/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0454_SCONcnfNeg")
signed         int        iAbsId;    /*abstract syntax identifier */
signed         int        iInx;      /*index in "for"-loop */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------*/
  /* Normal-mode only */
  /*------------------*/

  if (pContext->bNormalMode)
  {
    /*---------------------------------------------------------------*/
    /* Process a P-CONNECT confirm- caused by the session service.   */
    /* There are no PS-user data -> Is a correct P3 necessary S!?    */
    /* There is no PPDU -> P3 information from P-CONNECT request S!? */
    /*---------------------------------------------------------------*/

    /* ***** Process DCS related P3 information */

    pConn->stCnf.stPpm.vPresContId     [0] = (signed long int)
                                             pContext->vAbstrSynResult [0];
    pConn->stCnf.stPpm.vAbstrSynResult [0] =
    pConn->stCnf.stPpm.vTransSynId     [0] = pContext->vAbstrSynResult [0];
    
    for (
    iInx  = 1;
    iInx <= pContext->vAbstrSynResult  [0];
    iInx++)
    {
      pConn->stCnf.stPpm.vAbstrSynResult [iInx] =
      pContext->vAbstrSynResult [iInx];

      if (pContext->vAbstrSynResult [iInx] == RCro6c_Acceptance)
      {
        iAbsId = (signed int)
        pContext->vAbstrSynId [iInx];

        pConn->stCnf.stPpm.vTransSynId [iInx] =
        pContext->vTransSynId [iAbsId];

        pConn->stCnf.stPpm.vPresContId [iInx] =
        pContext->vPresContId [iAbsId];
      }
    }

    /* ***** Process other P3 information */

    pConn->stCnf.stPpm.sDefContResult = pContext->sDefContResult;

    pConn->stCnf.stPpm.ulRequirements = RCro60_Kernel;

  } /*to "if (!X410-mode)" */


  /*---------------------------*/
  /* X410-mode and normal-mode */
  /*---------------------------*/

  pContext->sState        = RC0430_StaI0;

  *pOutEvent = RC043v_OEvPCONcnfNeg;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0455_PCONrspPos                                   */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0455_PCONrspPos (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/


/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0455_PCONrspPos")
signed   short int        rc;        /*returncode of a called function */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*----------------*/
  /* X410-mode only */
  /*----------------*/

  if (!pContext->bNormalMode)
  {
    pNode->stCpa.lModeSelector = RCro70_X400Mode1984;

    *pX410RtpmNode = (unsigned long int FAR *)&pNode->stCpa.hX410RtpmNode;
  }


  /*------------------*/
  /* Normal-mode only */
  /*------------------*/

  else
  {
    /*--------------------------*/
    /* Check service parameters */
    /*--------------------------*/

    rc = ro0457_checkConnRsp (
      pContext,                       /* <-> PPM context (pointer to). */
      pConn);                         /* <-> Parameter P3 structure (pointer to). */

    if (rc)
      RM0003_RETURN (fnctName, signed short int, rc)


    /*default context rejection is not allowed */
    if (pConn->stRsp.stPpm.sDefContResult != RCro6i_NoDefContext
    &&  pConn->stRsp.stPpm.sDefContResult != RCro6c_Acceptance
    )
      RM0003_RETURN (fnctName, signed short int, -R_PDFARS)
      /*already checked in EXIF! */
      /*here better abort because otherwise partner doesn't know what is P!S */


    /*------------------------------------*/
    /* Process simple protocol parameters */
    /*------------------------------------*/

    pNode->stCpa.lModeSelector     = RCro71_NormalMode;
    pNode->stCpa.bNormalSEQUENCE   = TRUE;

    pNode->stCpa.ulProtVersion     = 1;              /*#define P!? */
    pNode->stCpa.bsProtVersion     = RC0419_ProtVersion1;

    pNode->stCpa.bPresRequirements = FALSE;          /*comment S!? */

    pNode->stCpa.bSessRequirements = FALSE;          /*comment S!? */


    /*---------------------------------------------------------*/
    /* Read responding P-selector from local address structure */
    /*---------------------------------------------------------*/
    
    rc = ro0217_getAddress (
      NULL,                            /* --> Memory handle to the whole PSAP. */
      RCro11_Pselector,             /* --> PM number.                       */
      RCro1a_MaxLenPSelector,          /* --> Max. length of P-selector.       */
      pNode->stCpa.vRespondingSelector /* <-- P-selector (pointer to).         */
    );

    if (rc < 0)
      RM0003_RETURN (fnctName, signed short int, rc)
      /*no AddressLengthError possible P!? (has been checked in EXIF P!?) */
    if (rc > 0)
    {
      pNode->stCpa.bRespondingSelector  = TRUE;
      pNode->stCpa.ulRespondingSelector = (unsigned long int) rc;
    }


    /*---------------------------------------------------------*/
    /* Process the presentation-context-definition-result-list */
    /*---------------------------------------------------------*/

    rc = ro0458_syntaxCpaCprSend (
      pContext,                       /* <-> PPM context (pointer to). */
      pEntContext,                    /* --> Pres. entity invoc. context (pointer to). */
      pConn,                          /* --> Parameter P3 structure (pointer to). */
      &pNode->stCpa.bPresContDefRes,  /* <-- Pres. PDV node element (pointer to) */
      &pNode->stCpa.ulPresContDefRes, /* <-- Pres. PDV node element (pointer to) */
      pNode->stCpa.vPresContDefRes);  /* <-- Pres. PDV node element (pointer to) */

    if (rc)
      RM0003_RETURN (fnctName, signed short int, rc)


    /*----------------------*/
    /* Process PS-user data */
    /*----------------------*/

    *pbUserData = (signed long int    FAR *)&pNode->stCpa.bUserData;
    *pUserData  = (rT0417_stUserData  FAR *)&pNode->stCpa.stUserData;

  } /*to "if (!X410-mode)" */


  /*---------------------------*/
  /* X410-mode and normal-mode */
  /*---------------------------*/

  pContext->sState        = RC0433_Stat0;
  pContext->sSessProtVers = pConn->stRsp.stAll.sSessProtVers;

  *pOutEvent = RC043r_OEvCPA;

/*   RM0424_GlobFree   (pContext->ulCp)  *//* since no redirection used */

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0456_PCONrspNeg                                   */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0456_PCONrspNeg (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/


/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0456_PCONrspNeg")
signed   short int        rc;        /*returncode of a called function */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*-------------------*/
  /* Redirect  vinita  */
  /*-------------------*/

  if ((*pP4 & RCro85_Redirection) == RCro85_Redirection)
  {
  /*  rT041G_unPpdu      FAR *p;      
    RM0421_GlobLock (pContext->ulCp, p ,rT041G_unPpdu ) 
*   *phNode     = pContext->ulCp; *
                                             *further processing of user data *
                                             *is done in ro0445_dataFromUser  *
    *pbUserData = (signed long int    FAR *)&pNode->stCp.bUserData; 
    *pUserData  = (rT0417_stUserData  FAR *)&pNode->stCp.stUserData;

    ro0041_memcpy   ( (char FAR *)pNode ,  (char FAR *) p , 
                      (unsigned long int ) sizeof(rT041G_unPpdu) );
    RM0422_GlobUnLock(pContext->ulCp)
 */
    pContext->sState = RC0430_StaI0;
    *pOutEvent       = RC043s_OEvCPR;
    RM0003_RETURN (fnctName, signed short int, OK)
  }
/*  user data??????? */

  /*----------------*/
  /* X410-mode only */
  /*----------------*/

  if (!pContext->bNormalMode)
  {
    pNode->stCpr.lX410orNormalChoice = RC041a_X410Mode;

    *pX410RtpmNode = (unsigned long int FAR *)&pNode->stCpr.hX410RtpmNode;
  }


  /*------------------*/
  /* Normal-mode only */
  /*------------------*/

  else
  {
    /*--------------------------*/
    /* Check service parameters */
    /*--------------------------*/

    rc = ro0457_checkConnRsp (
      pContext,                       /* <-> PPM context (pointer to). */
      pConn);                         /* <-> Parameter P3 structure (pointer to). */

    if (rc)
      RM0003_RETURN (fnctName, signed short int, rc)


    /*check default context result (ignore user error)                   */
    /*(better local abort P!?)                                           */
    /*Notes: - The PS-user has finished his work --> a P-PABORTind is    */
    /*         not possible.                                             */
    /*       - It seems to be better to transfer a CPR than an ARP PPDU. */
    /*       - If provider rejected default context, the PCONNECTrsp_neg */
    /*         has been issued by the PPM itself (--> only three values  */
    /*         may be set by PS-user).                                   */
    /*       - See "process default-context-result" below (only three    */
    /*         values may occur in CPR PPDU; in difference to presenta-  */
    /*         tion-context-definition-result-list there is no parameter */
    /*         "provider-reason"                                         */
    /*         --> better set RCro6d_UserRejection than                  */
    /*             RCro6e_ReasonNotSpecified P!?)                        */
    /*       - The done check is not possible in EXIF because the PPM    */
    /*         context is necessary.                                     */

    if (pContext->sDefContResult          != RCro6i_NoDefContext
    &&  pConn->stRsp.stPpm.sDefContResult == RCro6i_NoDefContext
    )
      pConn->stRsp.stPpm.sDefContResult = RCro6e_ReasonNotSpecified;


    /*------------------------------------*/
    /* Process simple protocol parameters */
    /*------------------------------------*/

    pNode->stCpr.lX410orNormalChoice = RC041b_NormalMode; /*rouply0-value P!? */

    pNode->stCpr.ulProtVersion       = 1;           /*#define P!? */
    pNode->stCpr.bsProtVersion       = RC0419_ProtVersion1;


    /*-------------------------*/
    /* Process provider-reason */
    /*-------------------------*/

    if ((*pP4 & 0xff00) == RC0400_PpmEvent)
    {
      pNode->stCpr.bProviderReason = TRUE;
      pNode->stCpr.lProviderReason = (signed long int) (*pP4 & 0x00ff);
    }
    /*Note: No "else" because pNode->stCpr.bProviderReason is 0-initialized. */

    /*---------------------------------------------*/
    /* Set P4 (LOIF expects RCro81_UserReject P!?) */
    /*---------------------------------------------*/

    *pP4 = RCro81_UserReject;


    /*--------------------------------*/
    /* Process default-context-result */
    /*--------------------------------*/

    if (pContext->sDefContResult != RCro6i_NoDefContext)
    {
      pNode->stCpr.bDefaultContextResult = TRUE;
      pNode->stCpr.lDefaultContextResult = pConn->stRsp.stPpm.sDefContResult
                                           < 0
                                         ? RC0412_ProviderRejection
                                         : pConn->stRsp.stPpm.sDefContResult;
                                           /*value range checked in EXIF P!? */
    }
    /*Note: No "else" because pNode->stCpr.bDefaultContextResult is 0-initialized. */


    /*---------------------------------------------------------*/
    /* Read responding P-selector from local address structure */
    /*---------------------------------------------------------*/
    
    rc = ro0217_getAddress (
      NULL,                            /* --> Memory handle to the whole PSAP. */
      RCro11_Pselector,             /* --> PM number.                       */
      RCro1a_MaxLenPSelector,          /* --> Max. length of P-selector.       */
      pNode->stCpr.vRespondingSelector /* <-- P-selector (pointer to).         */
    );

    if (rc < 0)
      RM0003_RETURN (fnctName, signed short int, rc)
      /*no AddressLengthError possible P!? (has been checked in EXIF P!?) */
    if (rc > 0)
    {
      pNode->stCpr.bRespondingSelector  = TRUE;
      pNode->stCpr.ulRespondingSelector = (unsigned long int) rc;
    }


    /*---------------------------------------------------------*/
    /* Process the presentation-context-definition-result-list */
    /*---------------------------------------------------------*/

    rc = ro0458_syntaxCpaCprSend (
      pContext,                       /* <-> PPM context (pointer to). */
      pEntContext,                    /* --> Pres. entity invoc. context (pointer to). */
      pConn,                          /* --> Parameter P3 structure (pointer to). */
      &pNode->stCpr.bPresContDefRes,  /* <-- Pres. PDV node element (pointer to) */
      &pNode->stCpr.ulPresContDefRes, /* <-- Pres. PDV node element (pointer to) */
      pNode->stCpr.vPresContDefRes);  /* <-- Pres. PDV node element (pointer to) */

    if (rc)
      RM0003_RETURN (fnctName, signed short int, rc)


    /*----------------------*/
    /* Process PS-user data */
    /*----------------------*/

    *pbUserData = (signed long int   FAR *)&pNode->stCpr.bUserData;
    *pUserData  = (rT0417_stUserData FAR *)&pNode->stCpr.stUserData;

  } /*to "if (!X410-mode)" */


  /*---------------------------*/
  /* X410-mode and normal-mode */
  /*---------------------------*/

  pContext->sState = RC0430_StaI0;

  *pOutEvent = RC043s_OEvCPR;

/*   RM0424_GlobFree   (pContext->ulCp) */ /* since no redirection used */   

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0457_checkConnRsp                                 */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed short int PASCAL ro0457_checkConnRsp (
	rT0430_stContext        *pContext,     /* <-> PM context (pointer to) */
	rT04a0_unConn       FAR *pConn)        /* <-> Parameter P3 structure (pointer to) */

/*                                                                    */
/*inoff ***************************************************************/


/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0457_checkConnRsp")
signed         int        iInx;      /*index in "for"-loop */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  if (pConn->stRsp.stPpm.ulRequirements != RCro60_Kernel)
    RM0003_RETURN (fnctName, signed short int, -R_PFUNCU)
    /*here better abort because otherwise partner don't know what is P!? */


  /*--------------------------------------------------------------*/
  /* Process abstract syntax result list                          */
  /* Note: "vAbstrSynResult [0] == 0" means "no user rejection of */
  /*       proposed presentation contexts".                       */
  /*--------------------------------------------------------------*/

  if (pConn->stRsp.stPpm.vAbstrSynResult [0])
  {
    /*check it if "user rejection of proposed pres. contexts" may occur */
    /*(local abort reason if failure)                                   */
    /*(better ignore if CPR, like default context result P!?)           */

    /*wrong number of list elements */
    if (pConn->stRsp.stPpm.vAbstrSynResult [0] != 
        pContext->vAbstrSynResult [0])
      RM0003_RETURN (fnctName, signed short int, RC040L_WrongSynResult) 

    for (
    iInx  = 1;
    iInx <= pContext->vAbstrSynResult [0];
    iInx++)
      if (pConn->stRsp.stPpm.vAbstrSynResult [iInx]     != 
          pContext->vAbstrSynResult [iInx]
      &&  (   /*change only allowed to "user-rejection" */
              pConn->stRsp.stPpm.vAbstrSynResult [iInx] !=
              RCro6d_UserRejection
              /*change only allowed from "acceptance" */
          ||  pContext->vAbstrSynResult [iInx]          !=
              RCro6c_Acceptance
          )
      )
        RM0003_RETURN (fnctName, signed short int, RC040L_WrongSynResult) 
  }

  else
    /*fill it if "user rejection of proposed pres. contexts" may not occur */
    for (
    iInx  = 0;
    iInx <= pContext->vAbstrSynResult [0];
    iInx++)
      pConn->stRsp.stPpm.vAbstrSynResult [iInx] = 
      pContext->vAbstrSynResult [iInx];

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0458_syntaxCpaCprSend                             */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-04-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed short int PASCAL ro0458_syntaxCpaCprSend (
	rT0430_stContext        *pContext,     /* <-> PM context (pointer to) */
	rT0441_stEntContext     *pEntContext,  /* --> Pres. entity inv. context (pointer to) */
	rT04a0_unConn       FAR *pConn,        /* --> Parameter P3 structure (pointer to) */
	signed   long  int      FAR *pbPresContDefRes,
	unsigned long  int      FAR *pulPresContDefRes,
	rT0413_stPresContDefRes FAR *pvPresContDefRes)

/*                                                                    */
/*inoff ***************************************************************/


/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0458_syntaxCpaCprSend")
rT0413_stPresContDefRes FAR *pAux;   /*aux. pointer to a context list element */
signed         int        iNumb;     /*number of proposed abstract syntaxes */
signed         int        iAbsId;    /*abstract syntax identifier */
signed         int        iTransId;  /*transfer syntax identifier */
signed         int        iInx;      /*index in "for"-loop */
signed   short int        rc;        /*return code of a called function */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*---------------------------------------------------------*/
  /* Process the presentation-context-definition-result-list */
  /*---------------------------------------------------------*/

  /* ***** process the number of elements */

  iNumb = (signed int) pConn->stRsp.stPpm.vAbstrSynResult [0];

  if (iNumb)
  {
    *pbPresContDefRes  = TRUE;    /*FALSE: 0-init. P!? */
    *pulPresContDefRes = (unsigned long int) iNumb;
  }


  /* ***** fill PDV node */

  for (
  iInx  = 1,
  pAux  = pvPresContDefRes;             /*pAux because better readability */
  iInx <= iNumb;
  iInx++,
  pAux++)
  {
    iAbsId   = (signed int) pContext->vAbstrSynId [iInx];

    switch (pConn->stRsp.stPpm.vAbstrSynResult [iInx])
    {
      case RCro6c_Acceptance:
        pAux->lResult = RC0410_Acceptance;

        /* ** transfer syntax name */

        iTransId = (signed int) pContext->vTransSynId [iAbsId];

        rc = ro0002_objcpy (
          pAux->oTransSynName,                    /* <-- Target. */
          pEntContext->vTransSynName [iTransId]); /* --> Source. */
        if (rc > 0)                               /* because R_LENULL */
          rc = -rc;
        if (rc)                                   /* rc now never > 0 */
          RM0003_RETURN (fnctName, signed short int, rc)

        break;                                                 
                                                               
      case RCro6d_UserRejection:                               
        pAux->lResult = RC0411_UserRejection;

        /* ** actualize the context set (DCS) predefined by PCONNECTind */

        pContext->vAbstrSynResult [iInx  ] = RCro6d_UserRejection;
        pContext->vAbstrSynId     [iInx  ] = RC043M_NoAbsId;
        pContext->vTransSynId     [iAbsId] = RC043O_NoTransId;
        pContext->vPresContId     [iAbsId] = RC043P_NoContId;
        pContext->sDcsNumb--;
        break;

      default:
        pAux->lResult = RC0412_ProviderRejection;

        pAux->bProviderReason = TRUE;
        pAux->lProviderReason = (- pConn->stRsp.stPpm.vAbstrSynResult [iInx])
                                - 1;
                              /*see comment in ro0424_syntaxCpaCprReceive() */
        break;
    } /*to "switch" */
  }   /*to "for" */

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/


/* EOF */	

