/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm00.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:13:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:42  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:38  keutel
 * 	OT 11620
 * 	[1994/08/19  13:47:35  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:51  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:00  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:00  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:50:47  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:28  keutel
 * 	creation
 * 	[1994/03/21  13:20:07  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roppm00.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:37 $" } ;
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
/* NAME         : roppm00.c                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-06-29                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Functions called from outside the PM                */
/*                                                                    */
/*   This C module provides functions which                           */
/*                                                                    */
/*     - perform the PM function (main function of the PM)            */
/*     - clean event parameters or PM context                         */
/*     - clear a pres. PDV node                                       */
/*     - get the defined context set                                  */
/*     - get a searched abstract syntax identifier.                   */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-03-24|  adaptation of ROS V2.5A00 K05 |hm  |         */
/* 2.0A00 K02|92-06-29|  SCCS-Id changed;              |hm  |         */
/*           |        |  cleanUp(): free P3 resources  |    |         */
/* 3.0A00 K00|93-05-26|  cleanUp from running fn if    |skh |         */
/*           |        |  state idle (instead of free). |    |         */
/* 3.0A00 K00|93-05-26|  Remove ErrCnt from cleanUp.   |skh |         */
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
/*              Module Identification                                 */
/**********************************************************************/

/** static char *sccsid =    "@(#) roppm00.c 1.2 93/12/20   RTS-V2.00";   **/
#ifndef RP0000_MicV0600
/*
static char *sccsid =    "@(#)roppm00.c" SNI_VERSION " RTROS-V03.00";
*/
#endif
/**********************************************************************/
/*              I N C L U D E S                                       */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>      /* Machine dependent generation             */
#include <roerval.h>      /* Unrecoverable error codes                */
#include <rolgval.h>      /* Logging constants                        */
#include <rouply0.h>      /* Common type and value definitions        */
#include <rouply1.h>      /* PM service interface structures          */
#include <routil0.h>      /* Utility interface (common functions)     */
#include <routil1.h>      /* Utility interface (object id. functions) */
#include <rostor0.h>      /* Store interface                          */
#include <rodpat0.h>      /* Dispatcher interface                     */
#include <rospm00.h>      /* SPM service interface                    */
#include <roppm00.h>      /* PPM service interface                    */
#include <roppm01.h>      /* PPM protocol interface                   */
#include <roppm02.h>      /* PPM calling interface                    */
#include <roppm04.h>      /* PPM entity invoc. context (before 03.h!) */
#include <roppmz0.h>      /* PPM Inform header                        */
#include <roppm03.h>      /* PPM logging interface                    */
#include <roppm10.h>      /* PPM intern: macros                       */
#include <roppm11.h>      /* PPM intern: types and constants          */
#include <roppm12.h>      /* PPM intern: function prototyping         */
#include <roppm14.h>      /* PPM intern: function prototyping         */


/**********************************************************************/
/*                                                                    */
/*              E X T E R N A L   D A T A                             */
/*                                                                    */
/**********************************************************************/

/*State-Event-Matrix (is read-only) */
/* CES compiler: error (const!) P!?
extern  signed short int (*const rv0460_vPpaMatrix [RC0434_MaxStates] [RC0435_MaxInEvent])();
extern  signed short int (* rv0460_vPpaMatrix [RC0434_MaxStates] [RC0435_MaxInEvent])();
*/


/*********************************************************************/
/*                                                                   */
/*        (5)   Event - State - Matrix                               */
/*              (table of function pointers)                         */
/*                                                                   */
/*********************************************************************/

/*const P!? */
signed short int (PASCAL * rv0460_vPpaMatrix [RC0434_MaxStates] [RC0435_MaxInEvent]) PROTO ((
rT0430_stContext       *pContext,            /* <-> PM context (pointer to)               */
rT0441_stEntContext    *pEntContext,         /* --> Pres. entity invocation context       */
                                             /*     (pointer to)                          */
rT04a0_unConn      FAR *pConn,               /* <-> Parameter P3 structure (pointer to)   */
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
)) =

{ /* state: RC0430_StaI0         */   {
  /* event: RC0436_IEvPCONreq    */   ro0450_PCONreq,
  /*        RC0437_IEvCP         */   ro0451_CP,
  /*        RC0438_IEvPCONrspPos */   ro0460_localError,
  /*        RC0439_IEvCPA        */   ro0461_dummyUnexpectedPpdu,
  /*        RC043a_IEvPCONrspNeg */   ro0460_localError,
  /*        RC043b_IEvCPR        */   ro0461_dummyUnexpectedPpdu,
  /*        RC043c_IEvSCONcnfNeg */   ro0462_dummyUnexpectedSessEvent,
                                      /*RC043c_IEvSCONcnfNeg: local reason, never transferred */

  /*        RC043d_IEvPRELreq    */   ro0460_localError,
  /*        RC043e_IEvSRELind    */   ro0462_dummyUnexpectedSessEvent,
  /*        RC043f_IEvPRELrspPos */   ro0460_localError,
  /*        RC043g_IEvSRELcnfPos */   ro0462_dummyUnexpectedSessEvent,
  /*        RC043h_IEvPRELrspNeg */   ro0460_localError,
  /*        RC043i_IEvSRELcnfNeg */   ro0462_dummyUnexpectedSessEvent,

  /*        RC043j_IEvPUABreq    */   ro0460_localError,
  /*        RC043k_IEvARU        */   ro0461_dummyUnexpectedPpdu,
  /*        RC043l_IEvARP        */   ro0461_dummyUnexpectedPpdu,
  /*        RC043m_IEvSPABind    */   ro0462_dummyUnexpectedSessEvent,
                                      /*RC043m_IEvSPABind: not relevant (no abort event issued) */

  /*        RC043n_IEvPDTreq     */   ro0460_localError,
  /*        RC043o_IEvTD         */   ro0461_dummyUnexpectedPpdu,
  /* End of state                */   },

  /* state: RC0431_StaI1         */   {
  /* event: RC0436_IEvPCONreq    */   ro0460_localError,
  /*        RC0437_IEvCP         */   ro0461_dummyUnexpectedPpdu,
  /*        RC0438_IEvPCONrspPos */   ro0460_localError,
  /*        RC0439_IEvCPA        */   ro0452_CPA,
  /*        RC043a_IEvPCONrspNeg */   ro0460_localError,
  /*        RC043b_IEvCPR        */   ro0453_CPR,
  /*        RC043c_IEvSCONcnfNeg */   ro0454_SCONcnfNeg,
                                   
  /*        RC043d_IEvPRELreq    */   ro0460_localError,
  /*        RC043e_IEvSRELind    */   ro0462_dummyUnexpectedSessEvent,
  /*        RC043f_IEvPRELrspPos */   ro0460_localError,
  /*        RC043g_IEvSRELcnfPos */   ro0462_dummyUnexpectedSessEvent,
  /*        RC043h_IEvPRELrspNeg */   ro0460_localError,
  /*        RC043i_IEvSRELcnfNeg */   ro0462_dummyUnexpectedSessEvent,

  /*        RC043j_IEvPUABreq    */   ro0469_PUABreq,
  /*        RC043k_IEvARU        */   ro046a_ARU,
  /*        RC043l_IEvARP        */   ro046b_ARP,
  /*        RC043m_IEvSPABind    */   ro046c_SPABind,
                                   
  /*        RC043n_IEvPDTreq     */   ro0460_localError,
  /*        RC043o_IEvTD         */   ro0461_dummyUnexpectedPpdu,
  /* End of state                */   },

  /* state: RC0432_StaI2         */   {
  /* event: RC0436_IEvPCONreq    */   ro0460_localError,
  /*        RC0437_IEvCP         */   ro0461_dummyUnexpectedPpdu,
  /*        RC0438_IEvPCONrspPos */   ro0455_PCONrspPos,
  /*        RC0439_IEvCPA        */   ro0461_dummyUnexpectedPpdu,
  /*        RC043a_IEvPCONrspNeg */   ro0456_PCONrspNeg,
  /*        RC043b_IEvCPR        */   ro0461_dummyUnexpectedPpdu,
  /*        RC043c_IEvSCONcnfNeg */   ro0462_dummyUnexpectedSessEvent,
                                      /*RC043c_IEvSCONcnfNeg: local reason, never transferred */

  /*        RC043d_IEvPRELreq    */   ro0460_localError,
  /*        RC043e_IEvSRELind    */   ro0462_dummyUnexpectedSessEvent,
  /*        RC043f_IEvPRELrspPos */   ro0460_localError,
  /*        RC043g_IEvSRELcnfPos */   ro0462_dummyUnexpectedSessEvent,
  /*        RC043h_IEvPRELrspNeg */   ro0460_localError,
  /*        RC043i_IEvSRELcnfNeg */   ro0462_dummyUnexpectedSessEvent,

  /*        RC043j_IEvPUABreq    */   ro0469_PUABreq,
  /*        RC043k_IEvARU        */   ro046a_ARU,
  /*        RC043l_IEvARP        */   ro046b_ARP,
  /*        RC043m_IEvSPABind    */   ro046c_SPABind,
                                   
  /*        RC043n_IEvPDTreq     */   ro0460_localError,
  /*        RC043o_IEvTD         */   ro0461_dummyUnexpectedPpdu,
  /* End of state                */   },

  /* state: RC0433_Stat0         */   {
  /* event: RC0436_IEvPCONreq    */   ro0460_localError,
  /*        RC0437_IEvCP         */   ro0461_dummyUnexpectedPpdu,
  /*        RC0438_IEvPCONrspPos */   ro0460_localError,
  /*        RC0439_IEvCPA        */   ro0461_dummyUnexpectedPpdu,
  /*        RC043a_IEvPCONrspNeg */   ro0460_localError,
  /*        RC043b_IEvCPR        */   ro0461_dummyUnexpectedPpdu,
  /*        RC043c_IEvSCONcnfNeg */   ro0462_dummyUnexpectedSessEvent,
                                      /*RC043c_IEvSCONcnfNeg: local reason, never transferred */

  /*        RC043d_IEvPRELreq    */   ro0463_PRELreq,
  /*        RC043e_IEvSRELind    */   ro0464_SRELind,
  /*        RC043f_IEvPRELrspPos */   ro0467_PRELrspPos,
  /*        RC043g_IEvSRELcnfPos */   ro0465_SRELcnfPos,
  /*        RC043h_IEvPRELrspNeg */   ro0468_PRELrspNeg,
  /*        RC043i_IEvSRELcnfNeg */   ro0466_SRELcnfNeg,

  /*        RC043j_IEvPUABreq    */   ro0469_PUABreq,
  /*        RC043k_IEvARU        */   ro046a_ARU,
  /*        RC043l_IEvARP        */   ro046b_ARP,
  /*        RC043m_IEvSPABind    */   ro046c_SPABind,
                                   
  /*        RC043n_IEvPDTreq     */   ro046d_PDTreq,
  /*        RC043o_IEvTD         */   ro046e_TD,
  /* End of state                */   },
};


/**********************************************************************/
/*                                                                    */
/*              S T A T I C - -   D A T A                             */
/*                                                                    */
/**********************************************************************/

static rT04z0_stDCS  stDCS ;    /* vinita  */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0400_ppm                                          */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0400_ppm (
	unsigned long  int  hContext)

/*                                                                    */
/* DESCRIPTION  : PM Function (Main Function of the PM).              */
/*                                                                    */
/*   The function is structured into  5 parts (steps)  according      */
/*   to the protocol machine generic paper.                           */
/*   In the  case of an incoming P-event the PPA is called after      */
/*   some mapping actions.                                            */
/*   In the case of an incoming S-event (expect of SContinue and      */
/*   SPABORTind) the  decode() function is called to decode PPCI      */
/*   and APCI of a  PPDU  into local syntax and to move ROS-user      */
/*   data into  a ROS-user  data buffer.  If  a  PPDU  has  been      */
/*   completely decoded,  the S-event  is mapped  and the PPA is      */
/*   called.                                                          */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*   hContext     - memory handle to the PM context                   */
/*                  (if zero, a context will be created)              */
/*                                                                    */
/* OUT-PARAMETERS : none                                              */
/*                                                                    */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0400_ppm")
rT0430_stContext        *pContext;      /*pointer to PM context */
unsigned       int       uInEvent;      /*incoming event (maybe mapped) */
unsigned       int       uOutEvent;     /*outgoing event */
unsigned long  int       ulP1;          /*event parameter P1 */
unsigned long  int       ulP2;          /*event parameter P2 */
unsigned long  int       ulP3;          /*event parameter P3 */
unsigned long  int       ulP4;          /*event parameter P4 */

unsigned long  int       hEcb;          /*memory handle to ECB (for the activity logging) */
unsigned long  int       hAcb;          /*memory handle to ACB (for the activity logging) */

signed         int       bLockP3;       /*TRUE: locking of handle P3 */
signed         int       bDirectSpm;    /*TRUE: event which has to be mapped directly to/from SPM */

signed         int       iAbortReason;  /*provider abort reason */
signed   short int       sPmNumber;     /*aux. to compute the dest. machine no. */
/* signed      int       bEventId; */   /*TRUE: provider abort with event identifier */

unsigned long  int       hEntContext;   /*memory handle to pres. entity invocation context */
rT04a0_unConn           *pConn;         /*pointer to parameter P3 structure */
unsigned long  int       hNode;         /*memory handle to pres. PDV node */
rT041G_unPpdu       FAR *pNode;         /*pointer to pres. PDV node */
unsigned long  int       hsStore;       /*Store handle to the PPDU encoding */

                                        /*pointers to PDV node elements (PS-user data) */
unsigned long  int  FAR *pX410RtpmNode; /*pointer to PS-user data handle (X410-mode) */
signed   long  int  FAR *pbUserData;    /*pointer to PS-user data OPTIONAL flag (normal-mode) */
rT0417_stUserData   FAR *pUserData;     /*pointer to PS-user data struct (normal-mode) */


signed   short int       rc;            /*return code of a called function */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*-----------------------------------------*/
  /* Step 1 from PM-generic: lock PM context */
  /*-----------------------------------------*/

  /* Note: If there occurs an error in this step, try to free the memory */
  /*       which was allocated for the PM context. P!? (ask JM)          */

  /*if no context is installed, create and initialize one */
  if (hContext == 0)
  {
    /*allocate and lock a new context */
    if ( (hContext = ro0030_localalloc (
                     (unsigned long int) sizeof(rT0430_stContext))) 
                     == NULL )
      RM0003_RETURN (fnctName, signed short int, - R_LALLCE)
    if ( (pContext = (rT0430_stContext *) ro0031_locallock (hContext)) == NULL)
        RM0003_RETURN (fnctName, signed short int, - R_LLOCKE)

    /*register the new context */
    rc = ro0212_regPmInvoc (hContext);
    if (rc)
        RM0003_RETURN (fnctName, signed short int, rc)

    /*initialize the new context */
    pContext->lProviderReason = RC043Q_NoReject; /*CPR reason */
    /*No assignment because 0-initializing alloc: */
    /*pContext->sState == RC0430_StaI0            */
  }
  else
    /*lock the context */
    if ( (pContext = (rT0430_stContext *) ro0031_locallock (hContext)) == NULL)
      RM0003_RETURN (fnctName, signed short int, - R_LLOCKE) /*no memory lockable */
                                                             /*above: free  PM context P!? */


  /*--------------------------------------------*/
  /* Step 2 from PM-generic: pop incoming event */
  /*--------------------------------------------*/

  uInEvent = (unsigned int) ro0210_popInEvent (
    &ulP1,                 /* <-- Event parameter P1 (pointer to). */
    &ulP2,                 /* <-- Event parameter P2 (pointer to). */
    &ulP3,                 /* <-- Event parameter P3 (pointer to). */
    &ulP4);                /* <-- Event parameter P4 (pointer to). */
    /*unrecoverable error possible P!? */


  /* ** PM activity logging */

  /*compute the application id (ECB handle) and the association id (ACB handle) */
  /*(used for the PDU logging too P!?)                                          */
  hAcb = ro0214_getAcbEcb (&hEcb);

  ro0043_pack10Log (
    RC0022_ACTIVITY,
    RL0401_ActivityPpm,
    8,
    (signed short int) sizeof(hEcb),
    (char FAR *)       &hEcb,
    (signed short int) sizeof(hAcb),
    (char FAR *)       &hAcb,
    (signed short int) sizeof(ulP1),
    (char FAR *)       &ulP1,
    (signed short int) sizeof(ulP2),
    (char FAR *)       &ulP2,
    (signed short int) sizeof(ulP3),
    (char FAR *)       &ulP3,
    (signed short int) sizeof(ulP4),
    (char FAR *)       &ulP4,
    (signed short int) sizeof(uInEvent),
    (char FAR *)       &uInEvent,
    (signed short int) sizeof(rT0430_stContext),
    (char FAR *)       pContext);

  ro0043_pack10Log (
    RC0022_ACTIVITY,
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

  /* ** process presentation entity invocation context */

  rc = ro0215_getEiCxt (
         RC0232_PresentationEiCxt,           /*PM number */
         &hEntContext);                 /*memory handle to P-application invocation context (pointer to) */
  if (rc < 0)
    RM0003_RETURN (fnctName, signed short int, rc)
  if (rc > 0)                           /*no pres. entity invoc. context */
    RM0003_RETURN (fnctName, signed short int, -R_PENTCX);

  RM0431_LocLock (hEntContext, pContext->pEntContext, rT0441_stEntContext);
    
  
  /*------------------------------------------------*/

  /* Step 3 from PM-generic: process incoming event */
  /*------------------------------------------------*/
  
  /* ** initialize local variables */

  bDirectSpm    = FALSE;
  bLockP3       = FALSE;
/*bEventId      = TRUE; */
  hNode         = 0;
  pNode         = 0;
  hsStore       = 0;
  pX410RtpmNode = 0;
  pbUserData    = 0;
  pUserData     = 0;
  iAbortReason  = 0;
                                        /*necessary and enough P!? */
  
  /*----------------------------*/
  /* 1st event specific actions */
  /*----------------------------*/

  switch (uInEvent & 0xff00)
  {
    /*-----------------------------------------------------------*/
    /* 1st event specific actions:   incoming event from PS-user */
    /*-----------------------------------------------------------*/

    case RC0400_PpmEvent:

      rc = ro0440_eventFromUser (
        pContext,    /* --> PPM context (pointer to).                            */    
        &uInEvent,   /* --> Incoming event from PS-user, original (pointer to).  */
                     /* <-- Incoming event from PS-user, mapped (pointer to).    */
        ulP1,        /* --> Event parameter P1 (PDV node handle).                */
        ulP4,        /* --> Event parameter P4 (ConnectResult or ReleaseResult). */
        &bLockP3,    /* <-- TRUE: locking of handle P3 necessary (pointer to).   */
        &bDirectSpm, /* <-- TRUE: event which has to be mapped directly to SPM   */
                     /*     (pointer to).                                        */
        &hNode,      /* <-- Memory handle to pres. PDV node (pointer to).        */
        &pNode);     /* <-- Pointer to pres. PDV node (pointer to).              */

      break;

    /*-------------------------------------------------------*/
    /* 1st event specific actions:   incoming event from SPM */
    /*-------------------------------------------------------*/

    case RC0800_SpmEvent:

      rc = ro0441_eventFromSpm (
        pContext,    /* --> PPM context (pointer to).                         */    
        (unsigned int FAR *)&uInEvent,
                     /* --> Incoming event from SPM, original (pointer to).   */
                     /* <-- Incoming event from SPM, mapped (pointer to).     */
        (unsigned long int FAR *)&ulP1,
                     /* --> Event parameter P1 (Store handle) (pointer to).   */
                     /* <-- Event parameter P1 (PDV node handle) if X410-mode */
                     /*     and S-DATA indication or S-TOKEN-PLEASE indication*/
                     /*     (pointer to).                                     */
        ulP2,        /* --> Event parameter P4 (Store Offset rcvd from Spm).  */
        ulP4,        /* --> Event parameter P4(ConnectResult or ReleaseResult)*/
        (signed int FAR *)&bLockP3,
                     /* <-- TRUE: locking of handle P3 necessary (pointer to).*/
        (signed int FAR *)&bDirectSpm,
                     /* <-- TRUE: event which has to be mapped directly to SPM*/
                     /*     (pointer to).                                     */
        (unsigned long int FAR *)&hsStore,
                     /* <-- Store handle (encoded byte string) (pointer to).  */
                     /*     Handle may be != 0 if a decode error occured.     */
        (unsigned long int FAR *)&hNode,
                     /* <-- Memory handle to pres. PDV node (pointer to).     */
        (rT041G_unPpdu FAR * FAR *)&pNode);
                     /* <-- Pointer to pres. PDV node (pointer to).           */
      break;

    default:                            /*if no valid PM basic event number */
      RM0003_RETURN (fnctName, signed short int, -R_PUNKEV)

  } /*to "switch (uInEvent & 0xff00)" */

  if (rc < 0)
    RM0003_RETURN (fnctName, signed short int, rc)

  if (rc > 0)
    iAbortReason = rc;


  /*------------------------------------------------------------------*/
  /* PPA (presentation protocol automata)                             */
  /* Implements the state tables of ISO 8823.                         */
  /*------------------------------------------------------------------*/

  else
  {
    /*lock the handle P3 if necessary (connection establishment) */
    if (bLockP3 == TRUE         )        /*better "if (ulP3)" P!? */
                                      /*then already done in PPA */
       
      RM0431_LocLock (ulP3, pConn, rT04a0_unConn)

    /*----------------------------------------------------*/
    /* If the event has to be mapped directly from/to SPM */
    /*----------------------------------------------------*/

    if (bDirectSpm)
      rc = ro0446_eventDirect (
        pContext,    /* --> PPM context (pointer to).                        */
        uInEvent,    /* --> Incoming event from PS-user or SPM (pointer to). */
        &uOutEvent,  /* <-- Outgoing event to PS-user or SPM (pointer to).   */
        pNode,       /* --> Pointer to pres. PDV node (pointer to).          */
        &pUserData); /* <-- PS-user data struct within PDV node              */
                     /*     (normal-mode) (pointer to).                      */

    /*----------------------------------------------------*/
    /* Otherwise: State-Event-Matrix call                 */
    /*----------------------------------------------------*/

    else
    {
      rc = (*rv0460_vPpaMatrix [pContext->sState] [uInEvent & 0xff]) (
        pContext,              /* <-> PM context (pointer to)               */
        pContext->pEntContext, /* --> Pres. entity invocation context       */
                               /*     (pointer to)                          */
        pConn,                 /* <-> Parameter P3 structure (pointer to)   */
        pNode,                 /* <-> Pres. PDV node (pointer to)           */
        &pX410RtpmNode,        /* <-- Pointer to PS-user data handle        */
                               /*     (X410-mode) (pointer to)              */ 
        &pbUserData,           /* <-- Pointer to PS-user data OPTIONAL flag */
                               /*     (normal-mode) (pointer to)            */
        &pUserData,            /* <-- Pointer to PS-user data struct        */
                               /*     (normal-mode) (pointer to)            */
        &uOutEvent,            /* <-- Outgoing event (pointer to)           */
        &hNode,                /* <-> Pres. PDV node (pointer to)           */
        &ulP1,                 /* <-> Event parameter P1 (pointer to)       */
        &ulP2,                 /* <-> Event parameter P2 (pointer to)       */
        &ulP3,                 /* <-> Event parameter P2 (pointer to)       */
        &ulP4);                /* <-> Event parameter P4 (pointer to)       */
    }

    /*check if unrecoverable error (before unlocking the handle P3) */
    if (rc < 0)
      RM0003_RETURN (fnctName, signed short int, rc)

    /*unlock the handle P3 if necessary */
    if (bLockP3   == TRUE                /*better "if (ulP3)" P!? but: directSpm then too */
    &&  uOutEvent != RC0403_PCONNECTrsp  /*then already done in PPA */
    ) 
      RM0432_LocUnLock (ulP3)

    if (rc > 0)
    {
      iAbortReason = rc;

      /*unlock the pres. PDV node if necessary */
      /*(no unlock-try in clearPdvTree)        */
      if (hNode)
        RM0422_GlobUnLock (hNode)
    }

    /*----------------------------------------------------------------------*/
    /* If PPA successful:                                                   */
    /* - Centralized handling of PS-user PDV node                           */
    /* - Centralized issueing of outgoing event                             */
    /* (One and only one event has to be issued.)                           */
    /*                                                                      */
    /* Exception: If a received CP PPDU causes a CPR PPDU (generated not by */
    /* PS-user but by PPM) an internal event to the PPM has been issued al- */
    /* ready within the PPA and there is no further outgoing event.         */
    /*----------------------------------------------------------------------*/

    else
    {
      if (uOutEvent != RC0403_PCONNECTrsp) /*if PPM not generates a CPR PPDU */
      {
        if ((uOutEvent & 0xff00) == RC0400_PpmEvent)
        {
          /*------------------------------------------------------*/
          /* Handle PS-user PDV node if outgoing event to PS-user */
          /*------------------------------------------------------*/

          rc = ro0444_dataToUser (
            pContext,      /* --> PPM context (pointer to).                   */
            uOutEvent,     /* --> Outgoing event.                             */
            (unsigned long int FAR *)&ulP1,
                           /* <-- Event param P1 (PDV node handle) (ptr to).  */
            (unsigned long int FAR *)&hNode,
                           /* <-- Memory handle to pres. PDV node (pointer to)*/
            pX410RtpmNode, /* --> PS-user data handle (X410-mode) (pointer to)*/
            pbUserData,    /* --> PS-user data OPTIONAL flag                  */
                           /*     (normal-mode) (pointer to).                 */
            pUserData);    /* --> PS-user data struct within PDV node         */
                           /*     (normal-mode) (pointer to).                 */

    /* here if rc is found to be greater  than 1 then provider abort
       should be initiated */
          if (rc < 0)
            RM0003_RETURN (fnctName, signed short int, rc)

          if (rc > 0)
          {
            iAbortReason = rc;
          }

          else { 

          /*------------------------------------------------------*/
          /* Compute the PM number of the PS-user                 */
          /*------------------------------------------------------*/

          if (uOutEvent == RC043G_OEvPDTind
          ||  uOutEvent >= RC043L_MinEventDirectToUser
          )  
            sPmNumber = RC0217_LcrtFct;
          else
            sPmNumber = RC0216_Acse;

          /*------------------------------------------------------*/
          /* Issue outgoing event to PS-user                      */
          /*------------------------------------------------------*/

          rc = ro0211_pushOutEvent (
            sPmNumber,           /* --> PM number.          */
            uOutEvent,           /* --> Event number.       */
            ulP1,                /* --> Event parameter P1. */
            ulP2,                /* --> Event parameter P2. */
            ulP3,                /* --> Event parameter P3. */
            ulP4);               /* --> Event parameter P4. */

          if (rc)                           /*rc never > 0 */
            RM0003_RETURN (fnctName, signed short int, rc)
         }
        } /*to "if (uOutEvent & PpmEvent)" */
        else

        {
          /*------------------------------------------------------*/
          /* Handle PS-user PDV node if outgoing event to SPM     */
          /*------------------------------------------------------*/

          rc = ro0445_dataFromUser (
            pContext,      /* --> PPM context (pointer to).                   */
            uOutEvent,     /* --> Outgoing event.                             */
            (unsigned long int FAR *)&ulP1,
                           /* <-- Event param P1 (PDV node handle) (ptr to).  */
             ulP4,         /* -->Event parm P4(ConnectResult or ReleaseResult)*/
            (unsigned long int FAR *)&hNode,
                           /* <-- Memory handle to pres. PDV node (pointer to)*/
            pX410RtpmNode, /* --> PS-user data handle (X410-mode) (pointer to)*/
            pbUserData,    /* --> PS-user data OPTIONAL flag                  */
                           /*     (normal-mode) (pointer to).                 */
            pUserData);    /* --> PS-user data struct within PDV node         */
                           /*     (normal-mode) (pointer to).                 */
 
          if (rc)                           /*rc never > 0 */
            RM0003_RETURN (fnctName, signed short int, rc)

          /*------------------------------------------------------*/
          /* Encode the PPDU                                      */
          /*------------------------------------------------------*/
          
          ro0043_pack10Log ( RC0022_ACTIVITY,
                             RL0405_EncodeInfo,
                             3,
                             (signed short int) sizeof(unsigned int),
                             (char FAR *) &uOutEvent,
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

          rc = ro0430_encodPpm (
            uOutEvent, /* --> Outgoing session event (defines the PPDU type). */
            ulP4,      /* --> Connect result (p4 of S-CONNECT response).      */
            (unsigned long int FAR *)&ulP2,
                       /* <-- offset  of store for session encoding           */
            (rT0430_stContext      *)pContext,
                       /* --> PPM context (pointer to).                       */
            (unsigned long int FAR *)&hsStore,
                       /* <-- Store handle (encoded byte string) (pointer to).*/
            (unsigned long int FAR *)&hNode);
                       /* <-> Memory handle to PDV node (pointer to).         */

          if (rc < 0)
            RM0003_RETURN (fnctName, signed short int, rc)
          ro0042_writeLog (RC0022_ACTIVITY  ,          
                 RL0407_OffsetInfo,            
                 sizeof (unsigned long int) ,         
                 (char FAR *) &ulP2  );  
          if (rc > 0)
          {
            iAbortReason = rc;

        /*  bEventId = FALSE; */
            /* Because encoding error: no abort event identifier. */
            /* But: Wouldn't it be good locally P!?               */
          }

          /*------------------------------------------------------*/
          /* Issue outgoing event to SPM                          */
          /*------------------------------------------------------*/

          else
          {
            rc = ro0211_pushOutEvent (
              RC0214_Session,      /* --> PM number.          */
              uOutEvent,           /* --> Event number.       */
              hsStore,             /* --> Event parameter P1. */
              ulP2,                /* --> Event parameter P2. */
              ulP3,                /* --> Event parameter P3. */
              ulP4);               /* --> Event parameter P4. */

            if (rc)                       /*rc never > 0 */
              RM0003_RETURN (fnctName, signed short int, rc)
          }

        } /*to "if (uOutEvent & PpmEvent) else ..." */

      } /*to "if PPM not generates a CPR PPDU" */

    } /*to "if no PPA error" */

  } /*to "if no incoming event error" */


  /*---------------------*/
  /* Do a provider abort */
  /*---------------------*/

  if (iAbortReason)
  {
    rc = ro0447_providerAbort (
      pContext,     /* --> PPM context (pointer to).                       */
      iAbortReason, /* --> Provider abort reason.                          */
      uInEvent,     /* --> Incoming event, mapped.                         */
      ulP1,         /* --> Resource to be cleared:                         */
                    /*     Event parameter P1 (PDV node handle).           */
      ulP2,         /* --> If RC080H_STRIGGERABORT:                        */
                    /*     Additional abort info.                          */
      ulP3,         /* --> Resource to be cleared:                         */
                    /*     Event parameter P3 (Memory handle to structure) */
                    /*     (conn. establishment or ACTIVITY-START/RESUME). */
      ulP4,         /* --> If RC080H_STRIGGERABORT:                        */
                    /*     Whether additional abort info pos./neg. signed. */
      hNode,        /* --> Resource to be cleared:                         */
                    /*     Memory handle to pres. PDV node.                */
      hsStore);     /* --> Resource to be cleared:                         */
                    /*     Store handle (encoded byte string).             */

    if (rc)
      RM0003_RETURN (fnctName, signed short int, rc)
  }


  /*----------------------------------------------------------------------*/
  /* Step 4 from PM-generic: unlock PM context                            */
  /*----------------------------------------------------------------------*/

  /*unlock the entity invocation context */
  RM0432_LocUnLock (hEntContext)

  /*if state is Idle, the context is no longer needed */
  if (pContext->sState == RC0430_StaI0)
  
  {
   /* save DCS in static structure before deregistration of Context */
   /* vinita */
 
    ro0041_memcpy((char FAR *)stDCS.vAbstrSynId,
                  (char FAR *)pContext->vAbstrSynId, 
                  (unsigned long int)sizeof(pContext->vAbstrSynId));
    ro0041_memcpy((char FAR *)stDCS.vTransSynId,
                  (char FAR *)pContext->vTransSynId, 
                  (unsigned long int)sizeof(pContext->vTransSynId));
    ro0041_memcpy((char FAR *)stDCS.vPresContId,
                  (char FAR *)pContext->vPresContId, 
                  (unsigned long int)sizeof(pContext->vPresContId));
    ro0041_memcpy((char FAR *)stDCS.vAbstrSynResult,
                  (char FAR *)pContext->vAbstrSynResult, 
                  (unsigned long int)sizeof(pContext->vAbstrSynResult));

    stDCS.pcnum = pContext->sDcsNumb;
    

   /*deregister and free the context */
    RM0432_LocUnLock (hContext)
    rc = ro0401_cleanUp (LNULL, hContext, LNULL, LNULL, LNULL);
    if (rc != OK)
        RM0003_RETURN (fnctName, signed short int, rc)
    ro0213_regPmDestr ();
  }
  else
    /*unlock the context */
    RM0432_LocUnLock (hContext)

  
  /*----------------------------------------------------------------------*/
  /* Step 5 from PM-generic: synchronous return code                      */
  /*----------------------------------------------------------------------*/

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0401_cleanUp                                      */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-06-29                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0401_cleanUp (
	signed   long  int  lEvent,
	unsigned long  int  ulP1orhContext,
	unsigned long  int  ulP2,
	unsigned long  int  ulP3,
	unsigned long  int  ulP4)

/*                                                                    */
/* DESCRIPTION  : Clean Event Parameters or PM Context.               */
/*                                                                    */
/*   Called from outside the PM  if the application  has  to be       */
/*   detached.                                                        */
/*                                                                    */
/*   Cleans the event parameters if the parameter lEvent is  not      */
/*   equal to NULL.                                                   */
/*   Cleans the PM context if the parameter lEvent is NULL.           */
/*                                                                    */
/*   If an error is detected  an error counter is increased  and      */
/*   the clearing is continued.  At the end of the function  the      */
/*   error counter is checked and the returncode -R_PCLEAN    is      */
/*   set if the counter was increased.                                */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*   lEvent         - event to clear or NULL                          */
/*                                                                    */
/*   ulP1orhContext - parameter 1 or memory handle to PM context      */ 
/*                    (better union type P!?)                         */
/*                                                                    */
/*   ulP2           - parameter 2 or NULL                             */
/*                                                                    */
/*   ulP3           - parameter 3 or NULL                             */
/*                                                                    */
/*   ulP4           - parameter 4 or NULL                             */
/*                                                                    */
/*                                                                    */
/* OUT-PARAMETERS : none                                              */
/*                                                                    */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0401_cleanUp")
rT04a0_unConn      *pConn           = LNULL;/* pointer to param P3 structure  */
unsigned long  int  hAddress        = LNULL;/* memory handle to pres. address */
unsigned long  int  hCldAddr        = LNULL;/* memory handle to pres. address */
rT0430_stContext   *pulP1orhContext = LNULL;
signed short int    rc              = OK;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)


  /*---------*/
  /* Free P1 */
  /*---------*/

  /*if incoming event from PS-user */
  if (((unsigned int) lEvent & 0xff00) == RC0400_PpmEvent) {
    if (ro0246_clearPdvTree (&ulP1orhContext))
        RM0003_RETURN (fnctName, signed int, -R_PCLPDV)
  }

  /*if incoming event from SPM */
  if (((unsigned int) lEvent & 0xff00) == RC0800_SpmEvent) {
    if (ulP1orhContext) {
      if (ro0075_deletestore (ulP1orhContext))
         RM0003_RETURN (fnctName, signed int, -R_PSTERR)
    }
  }


  /*-----------------------------*/
  /* Free P3 and bound resources */
  /*-----------------------------*/
  if ( (lEvent == RC0403_PCONNECTrsp ) 
    && (( ulP3 & RCro85_Redirection) == RCro85_Redirection) )
     ulP3 = NULL;
  if (ulP3)
  {
    /* free resources bound to P3 */
    if ( (pConn = (rT04a0_unConn *) ro0031_locallock (ulP3)) == LNULL)
        RM0003_RETURN (fnctName, signed int, -R_LLOCKE)

      /*furthermore only try to unlock and free the own PDV node */

    switch (lEvent)
    {
        case RC0401_PCONNECTreq:
          hAddress = pConn->stReq.stAll.hCalledAddress;
          break;
        case RC0802_SCONNECTind:
          hAddress = pConn->stInd.stAll.hCallingAddress;
          hCldAddr = pConn->stInd.stAll.hCalledAddress;
          break;
        case RC0403_PCONNECTrsp:
          hAddress = pConn->stRsp.stAll.hRespondAddress;
          break;
        case RC0804_SCONNECTcnf:
          hAddress = pConn->stCnf.stAll.hRespondAddress;
          break;
        default:
          hAddress = NULL;
          break;
    }

    if (hAddress) {
      if      (ro0034_localfree (hAddress))
          RM0003_RETURN (fnctName, signed int, -R_LFREEE)
    }
    if (hCldAddr) {
      if      (ro0034_localfree (hCldAddr))
          RM0003_RETURN (fnctName, signed int, -R_LFREEE)
    }

    if      (ro0032_localunlock (ulP3))     rc = -R_LULOKE;
    else if (ro0034_localfree   (ulP3))     rc = -R_LFREEE; /* free P3 */
    if (rc)
        RM0003_RETURN (fnctName, signed int, rc)
  }


  /*----------------------------------------------*/
  /* Clear the PM context and the bound resources */
  /*----------------------------------------------*/

  if (!lEvent) {
    /* free handle of CP PPDU kept for redirection in context block */
    /* before making the context block free  */

    pulP1orhContext  = (rT0430_stContext *) ro0031_locallock (ulP1orhContext);
    if (pulP1orhContext) {
/*
        if (pulP1orhContext->ulCp) {
           if (ro0024_globalfree (pulP1orhContext->ulCp))
               RM0003_RETURN (fnctName, signed int, -R_GFREEE)
        } * by kc 7/7/93 */

        if      (ro0032_localunlock (ulP1orhContext))  rc = -R_LULOKE;
        else if (ro0034_localfree   (ulP1orhContext))  rc = -R_LFREEE;
        if (rc)
            RM0003_RETURN (fnctName, signed int, rc)
    }
  } /* end if call for clear pContext */

  /*-------------------------*/
  /* Check the error counter */
  /*-------------------------*/

  RM0003_RETURN (fnctName, signed int, rc)
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

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro0406_informPpm                                            */
/*                                                                            */
/* AUTHOR       : Vinita 						      */
/* DATE         : 93-02-23                                                    *//*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro0406_informPpm (
unsigned long  int      hContext,              /*  -> : see roppm03.h */
signed   short int      sInformType,           
void               FAR *pRequestedInfo)        

/* DESCRIPTION :  vinita                                                      */
/*                                                                            */
/*     Provides information on the status of PPM registered in the provided   */
/*     context block.                                                         */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     hContext              = PPM context block (handle of).                 */
/*                                                                            */
/*     sInformType           = Type of information requested.                 */
/*                             Possible values are :                          */
/*                             - RC04zP_ACSEAbId.                             */
/*                             - RC04zQ_EXIFAbId.                             */
/*			       - RC04zR_getDCS.                               */
/*								              */
/*     pRequestedInfo        = Pointer to memory containing the data          */
/*                             structure associated with sInformType that is  */
/*                             TO BE FILLED with the requested info.          */
/*                                                                            */
/*			     = pObjectId in case of (RC043R_ACSEAbId).        */
/*                                                                            */
/*                                                                            */
/* FORMAL OUT-PARAMETERS :                                                    */
/*                                                                            */
/*     pRequestedInfo        = Pointer to memory containing the data          */
/*                             structure associated with sInformType that is  */
/*                             FILLED with the requested info.                */
/*                                                                            */
/*                                                                            */
/* PUSHED OUT-PARAMETERS : None.                                              */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/* GLOBAL DATA (read only) : None.                                            */
/*                                                                            */
/* GLOBAL DATA (modified)  : None.                                            */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro0406_informPpm*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro0406_informPpm")
signed   short int          rc=0  ;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (pFnctName)

switch (sInformType) {

/*----------------------------------------------------------------------------*/
/* Information on ACSEAbsId requested.                                     */
/*----------------------------------------------------------------------------*/
case RC04zP_ACSEAbId : {
         	rc  = ro0426_getAbsId(sInformType,
/* K02                          (rT04z1_stAbsNameId FAR *)pRequestedInfo); */
                       (rT04z1_stAbsNameId FAR *)pRequestedInfo,hContext);
        break;

}/* end case getACSEAbId */


case RC04zQ_EXIFAbId : { 

        unsigned long int acseAbSyn[] = { 5, 2, 2, 1, 0, 1 };
        ro0002_objcpy(((rT04z1_stAbsNameId FAR *)pRequestedInfo)->oAbsName,
                       acseAbSyn);
        if(hContext)
	rc = ro0426_getAbsId(sInformType,
/* K02                         (rT04z1_stAbsNameId FAR *)pRequestedInfo); */
                  (rT04z1_stAbsNameId FAR *)pRequestedInfo,hContext);
       else {
         rT04z1_stAbsNameId FAR * pstAbsId = 
                                 (rT04z1_stAbsNameId FAR *)pRequestedInfo;
         pstAbsId->sAbsId = -1;
       }
        break;
}

case RC04zR_getDCS : {

	rc = ro0427_getDcs ((rT04z0_stDCS FAR *)&stDCS,
                             hContext,
                            (rT04z0_stDCS FAR *)pRequestedInfo);
        break;

}

case RC04zS_findAbsId:
        rc = ro0428_findAbsId ((rT04z2_stAbsIdFind FAR *)pRequestedInfo);
        break;

case RC04zT_getMode:
        rc = ro0429_getMode ( hContext , (rT04z3_stMode FAR *)pRequestedInfo);
        break;

}/*switch-end*/

RM0003_RETURN (pFnctName, signed short int, rc)

}/*end*/


/*EOF */
