/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropm0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:56  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:06  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:27  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:59  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:14  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:05:46  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:46  keutel
 * 	creation
 * 	[1994/03/21  13:34:39  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roropm0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:56 $" } ;
#endif


/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1993                     */
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
/* NAME         : roropm0.c                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-31                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : ROS ROSE Protocol Machine                           */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropm0.c	1.2 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-03-31|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) roropm0.c 1.2 93/12/20   RTS-V2.00";



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
#include <rolgval.h>      /* Logging constants                        */

#include <rouply0.h>      /* Common type and value definitions        */
#include <rouply1.h>      /* PM service interface structures          */

#include <routil0.h>      /* Utility interface (common functions)     */
#include <routil1.h>      /* Utility interface (object id. functions) */
#include <rodpat0.h>      /* Dispatcher interface                     */
#include <rodpat1.h>      /* included during Dos compilation          */


#include <rostor0.h>      /* Store related defs & fun.  declarations  */
#include <roendeco.h>     /* Macros defs for PTS en/dec functions     */

#include <roacpm0.h>

#include <roppm00.h>
#include <roppmzy.h>      /* PTS constant  definitions                */

#include <rortpm0.h>

#include <roropm0.h>      /* Interface definitions                    */
#include <roropm1.h>      /* PDU data structures                      */
#include <roropm2.h>      /* Interface function definitions           */
#include <roropm3.h>      /* Local Events & Data structures           */
#include <roropm4.h>      /* Automata Function proto TYpe             */
#include <roropm5.h>      /* Static Automata Table                    */
#include <roropm6.h>      /* Internally used  function prototypes.    */
#include <roacpz0.h>      /* Acpm Inform fun & structures             */
#include <roropz0.h>      /* Inform function data structures          */
#include <roropmzy.h>     /* PTS relates defs.                        */
#include <roropzx.h>      /* Included while compiling on dos.-skh     */



/* ****************** Logging function name strings ***************** */
/* ******************                               ***************** */

RM0001_STRG (rs0700, "ro0700_ropm        ")
RM0001_STRG (rs0701, "ro0701_cleanUp     ")
RM0001_STRG (rs0702, "ro0702_encodRopm   ")
RM0001_STRG (rs0703, "ro0703_decodRopm   ")
RM0001_STRG (rs0705, "ro0705_informRopm  ")
RM0001_STRG (rs0704, "ro0704_clearNodeRop")
RM0001_STRG (rs0707, "ro0707_GiveProvider")     
RM0001_STRG (rs0708, "ro0708_ProviderAbor")
RM0001_STRG (rs0709, "ro0709_ProviderReject")
RM0001_STRG (rs070a, "ro070a_encodSniPRopm")
RM0001_STRG (rs070b, "ro070b_decodSniPRopm")

RM0001_STRG (rs070s, "ro070s_GetRtseAbsId")
RM0001_STRG (rs070t, "ro070t_ClearContext")
RM0001_STRG (rs070u, "ro070u_PutInQueue  ")
RM0001_STRG (rs070v, "ro070v_GetFromQueue")
RM0001_STRG (rs070w, "ro070w_GetPossibili")
RM0001_STRG (rs070x, "ro070x_ResolveRc   ")
/*====
RM0001_STRG (rs070y, "ro070y_GetApduType ")
====*/
RM0001_STRG (rs070z, "ro070z_SearchAndGet")
/* ****************************************************************** */
/*   Function Prototypes for the Local Functions                      */
/* ****************************************************************** */

static signed   short int NEAR PASCAL ro070z_SearchAndGetIndex PROTO ((
        rT0730_stContext      NEAR *   ,
        signed     long  int           ,
        signed     short int           ,
        signed     short int  FAR *   )); 
      
static signed  short int NEAR PASCAL ro070x_ResolveRc PROTO (( 
        rT0730_stContext      NEAR *       ,
        signed    long   int               ,
        signed    long   int               ,
        unsigned  long   int               ,
        unsigned  long   int               ,
        unsigned  long   int               ,
        unsigned  long   int               ,
        signed    short  int               ));

static signed  short int NEAR PASCAL ro070w_GetPossibilityInfo PROTO ((
        rT0730_stContext         NEAR * ,
        rT07z0_stPossibilityInfo FAR  * ));

static signed short int PASCAL ro070t_ClearContext PROTO ((
        rT0730_stContext  NEAR * ));




/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro070x_ResolveRc                                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed  short int NEAR PASCAL ro070x_ResolveRc ( 

rT0730_stContext      NEAR *       pContext     ,
signed  long   int               lInEvent    ,
signed  long   int               lOutEvent    ,
unsigned  long   int               hP1          ,
unsigned  long   int               hP2          ,
unsigned  long   int               hP3          ,
unsigned  long   int               hP4          ,
signed    short  int               sRetCode     )


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
signed   short int     rc =OK;
signed   short int     sPmNum;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
RM0002_ENTRY (rs070x)
 if (lOutEvent) {
      switch( lOutEvent & 0xff00) {
      case RC0700_RoseEvent :
           sPmNum = RC0221_RoseUser;
           break;
      case RC0400_PpmEvent:
      case RC0600_AcpmEvent:
           sPmNum = RC0217_LcrtFct;
           break;
      case RC1000_RtpmEvent:
           sPmNum = RC0218_Rtse;
           break;
      default:
           /*!!! give errro */
           break;
     }
 } 
    
 if (sRetCode)
    switch(sRetCode){
        case OK:
              break;
        case 1:        /* ---- Push out the event -------- */
           rc =  (lOutEvent)
                ? ro0211_pushOutEvent (sPmNum , lOutEvent ,hP1,hP2,hP3,hP4)
                : 0 ;
           break;
        default: 
           if (sRetCode > 1 ) {
             switch (lInEvent & 0xff00){
                case RC0700_RoseEvent:            
                     rc = -R_OROPME; 
                     break;                        
                case RC0400_PpmEvent:              
                case RC0600_AcpmEvent:             
                case RC1000_RtpmEvent:             
                           rc = ro0708_ProviderAbort(pContext,
                                                     lInEvent,
                                                     sRetCode,
                                                     hP1,hP2,hP3,hP4 );
                           break;
                default:
                       break;
             }
           } else rc = sRetCode; 
           break;
      }
RM0003_RETURN (rs070x, signed short int, rc)                                    
}/*end*/                                              

                          

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro070z_SearchAndGetIndex                            */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed   short int NEAR PASCAL ro070z_SearchAndGetIndex (
rT0730_stContext      NEAR *   pContext   ,
signed     long  int           lEvent     ,
signed     short int           sSubAutoNum,
signed     short int  FAR  *   psIndex          )

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
signed   short int         rc          = OK ;
signed   short int         sStateIndex ; 
signed   short int         sAutoIndex  = 0; 
signed   short int         sEndIdx  ; 
signed   short int         sState     ;
signed   short int  NEAR * pIdxAry;
/* ****************** s t a t i c   v a r i a b l e s *************** */
/* ******************                                **************** */

static signed  short int  vIdxRopmAutomata[RC0730_maxStates1];
static signed  short int  vIdxPrTrAutomata[RC0734_maxStates2];
static signed  short int  vIdxRtTrAutomata[RC0738_maxStates3];

static signed  short int  bFresh = TRUE;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs070z)
   if (bFresh ){
      for ( sStateIndex = 0 ;
            sStateIndex < RC0730_maxStates1;
            sStateIndex++ ) {
      
          vIdxRopmAutomata[sStateIndex] = sAutoIndex ;
          sAutoIndex   += (short)( vRopmAutomata[sAutoIndex].lInEvent + 1);
      /* --------- +1 is for the first element int Array which ----- */
      /* --------- holds the number of the events              ----- */
      } /*end of first array */
      for (  sStateIndex = 0 ;
             sStateIndex < RC0734_maxStates2 ;
             sStateIndex++ ) {
          
          vIdxPrTrAutomata[sStateIndex] = sAutoIndex;
          sAutoIndex   += (short)(vRopmAutomata[sAutoIndex].lInEvent + 1);
      } /*end of second array */
      for (  sStateIndex = 0 ;
             sStateIndex < RC0738_maxStates3 ;
             sStateIndex++ ) {
          
          vIdxRtTrAutomata[sStateIndex] = sAutoIndex;
          sAutoIndex   +=(short)(vRopmAutomata[sAutoIndex].lInEvent + 1);
      } /*end of third array */
      if   ((sAutoIndex > RC0740_MaxActions)){ 
          RM0003_RETURN (rs070z, signed short int, -R_FINTER);
      }
      else  
          bFresh = FALSE;
   }      /* end of init */

   /* --------------during normal operation search -------------- */
      switch(sSubAutoNum){
          case  RC073g_RopmPm:
                 sState = pContext->sStateRopm;
                 if (sState >= RC0730_maxStates1 ) rc = -R_OWSQFL;
                 pIdxAry = vIdxRopmAutomata;
                 break;
          case RC073h_RopmPrTr:
                 sState = pContext->sStateTrPm;
                 if (sState >= RC0734_maxStates2 ) rc = -R_OWSQFL;
                 pIdxAry = vIdxPrTrAutomata;
                 break;
          case RC073i_RopmRtTr:
                 sState = pContext->sStateTrPm;
                 if (sState >= RC0738_maxStates3 ) rc = -R_OWSQFL;
                 pIdxAry = vIdxRtTrAutomata;
                 break;
          default: 
                rc = -R_OWSQFL;
                break;
      }
     if ( rc != OK )                     
                RM0003_RETURN (rs070z, signed short int, rc)                                    
     *psIndex = pIdxAry[sState];
     sEndIdx  =(short)( *psIndex + vRopmAutomata[*psIndex].lInEvent );
     while ( (*psIndex <= sEndIdx) && 
             (lEvent != vRopmAutomata[*psIndex].lInEvent) ) 
           (*psIndex)++;
     if (*psIndex > sEndIdx )  {   /* Not found */
         *psIndex = -1;
         rc = -R_WREVNT;
     }
 

RM0003_RETURN (rs070z, signed short int, rc)                                    
}/*end*/                                                                 

       
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0700_ropm                                         */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

             signed short int FAR PASCAL ro0700_ropm(

                        unsigned long  int  hContext)

/*                                                                    */
/* DESCRIPTION  : PM Function (Main Function of the PM).              */
/*                                                                    */
/*   The function is structured into  5 parts (steps)  according      */
/*   to the protocol machine generic paper.                           */
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

{/*entr*/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

rT0730_stContext        *pContext;      /*pointer to ROPM context */
signed   long  int       lInEvent;      /*incoming event (maybe mapped) */
signed   long  int       lOutEvent;     /*outgoing event */
unsigned long  int       ulP1     = LNULL;          /*event parameter P1 */
unsigned long  int       ulP2     = LNULL;          /*event parameter P2 */
unsigned long  int       ulP3     = LNULL;          /*event parameter P3 */
unsigned long  int       ulP4     = LNULL;          /*event parameter P4 */

unsigned long  int       hEcb;          /*memory handle to ECB (for the activity logging) */
unsigned long  int       hAcb;          /*memory handle to ACB (for the activity logging) */

signed   short int       sSubAutoNum;
signed   short int       sIndex;


signed   short int       rc = OK;       /*return code of a called function */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */



  RM0002_ENTRY (rs0700)

  /*-----------------------------------------*/
  /* Step 1 from PM-generic: lock PM context */
  /*-----------------------------------------*/

  /*if no context is installed, create and initialize one */

  if (hContext == 0) {
    hContext = ro0030_localalloc((unsigned long int) sizeof(rT0730_stContext));
    if (hContext == NULL) rc = -R_LALLCE;
    else {
         pContext = (rT0730_stContext *) ro0031_locallock (hContext) ;
         if (pContext == NULL) rc = -R_LLOCKE;
    }
    if (rc != OK ) RM0003_RETURN (rs0700, signed short int, rc) 
    /* ------ Memory allocation is successfully Over --------------- */


    /* ---------- Now Register this new Context with Dispatcher ---- */

     rc = ro0212_regPmInvoc (hContext); 
     if (rc)
         RM0003_RETURN (rs0700, signed short int, rc);

    /* --------------initialize the new context  ------------------- */
    pContext->sStateRopm   = 
    pContext->sStateTrPm   =RC0731_STAnoPm;

    pContext->sReleaseFg   =0;
    pContext->sSubAutoNum  =0;
    pContext->bAssocInit   =FALSE;
    pContext->ulAssocClas  =0;
    pContext->bSyncFlag    =FALSE;
    pContext->lInvokeId    =0;
    pContext->sRejCount    =0;
    pContext->bmappingInfo =FALSE; /* default with out RTSE */
    pContext->bToken       =FALSE;
    pContext->hFirst       =LNULL;
    pContext->hLast        =LNULL;
   
    
   } /*end of the creation of the NEW Context */
  else   {           /* --- OLD Context ----- */
    pContext = (rT0730_stContext *) ro0031_locallock (hContext);
    if (pContext == NULL)
           RM0003_RETURN (rs0700, signed short int, - R_LLOCKE) ;
                                                            
  } 
  /* ------- Locking the Context is Over ------------------------ */



  /*--------------------------------------------*/
  /* Step 2 from PM-generic: pop incoming event */
  /*--------------------------------------------*/

  lInEvent = (unsigned int) ro0210_popInEvent (
    &ulP1,                
    &ulP2,               
    &ulP3,              
    &ulP4);            


  /* ** PM activity logging */

  /*get the application id (ECB handle) and the association id (ACB handle) */
   hAcb = ro0214_getAcbEcb (&hEcb); 

  ro0043_pack10Log (
    RC0022_ACTIVITY,
    RL0701_ActivityRopm,
    8,
    (signed short int) sizeof(unsigned long int),
    (char FAR *)       &hEcb,
    (signed short int) sizeof(unsigned long int),
    (char FAR *)       &hAcb,
    (signed short int) sizeof(unsigned long int),
    (char FAR *)       &ulP1,
    (signed short int) sizeof(unsigned long int),
    (char FAR *)       &ulP2,
    (signed short int) sizeof(unsigned long int),
    (char FAR *)       &ulP3,
    (signed short int) sizeof(unsigned long int),
    (char FAR *)       &ulP4,
    (signed short int) sizeof(signed long int),
    (char FAR *)       &lInEvent,
    (signed short int) sizeof(rT0730_stContext),
    (char FAR *)       pContext);

  /*------------------------------------------------*/
  /* Step 3 from PM-generic: process incoming event */
  /*------------------------------------------------*/
  /* -------- Seperate as per the Incomming PM Num. -------------------- */

  switch (lInEvent & 0xff00){
      case RC0700_RoseEvent :
         sSubAutoNum = RC073g_RopmPm;
         break;
      case RC0400_PpmEvent:
      case RC0600_AcpmEvent:
         sSubAutoNum = RC073h_RopmPrTr;
         break;
      case RC1000_RtpmEvent:
         sSubAutoNum = RC073i_RopmRtTr;
         break; 
      default:
          /* ------- !!! what error to give --------------------------- */
         break;
   } /*end of the switch */
   lOutEvent = lInEvent; 
   while (( sSubAutoNum != RC073j_End) && (rc == OK)  ){ 
      lInEvent = lOutEvent;
      /* --------- Map the inEvents to Local events If necessary --------- */
      switch(lInEvent){
         case RC073P_TRANSind:
       /*       rc = ro070y_GetApduType ( ulP1, &lInEvent);   */
                lInEvent = ulP4;
                ulP4     = LNULL;
              break;
        case RC040F_PCONTINUE:  /* do Nothing */
                goto EndWhileloop;
      }
           
      rc = ro070z_SearchAndGetIndex( pContext                        ,
                                     lInEvent                        ,
                                     sSubAutoNum                     ,
                                    (signed short int FAR *) &sIndex );
                                                                    
      if ( rc != OK ) break;  
      /* ---------- Call the Automata ------------------------------------ */
      rc = (*vRopmAutomata[sIndex].pAction) (pContext   , 
                                             (long FAR *)&lInEvent  ,
                                             (long FAR *)&lOutEvent ,
                                             (unsigned long FAR *)&ulP1, 
                                             (unsigned long FAR *)&ulP2, 
                                             (unsigned long FAR *)&ulP3, 
                                             (unsigned long FAR *)&ulP4 );  

      sSubAutoNum = pContext->sSubAutoNum;
      
   } /*end of the while loop */
  
EndWhileloop:
 
   rc = ro070x_ResolveRc (pContext             ,
                          lInEvent             ,
                          lOutEvent            ,
                          ulP1                 ,
                          ulP2                 ,
                          ulP3                 ,
                          ulP4                 ,
                          rc                   );
 
  /*----------------------------------------------------------------------*/
  /* Step 4 from PM-generic: unlock PM context                            */
  /*----------------------------------------------------------------------*/

  /*if state is Idle, the context is no longer needed */
  if ( (pContext->sStateRopm  == RC0731_STAnoPm) ||
       (pContext->sStateTrPm  == RC0735_STAnoTr1 ) )
  {
         rc = ro070t_ClearContext ( pContext );
    /* ------------- deregister and free the context -------------------- */
         if ( (rc =(short)ro0032_localunlock(hContext) ) == OK ) {
            if( ro0034_localfree(hContext) )  rc = -R_LFREEE ;
            ro0213_regPmDestr ();
         }
         else
            rc = -R_LULOKE; 
   
  }
  else
    /* ------------------- unlock the context  -------------------------  */
     if (ro0032_localunlock(hContext) != OK )   rc = -R_LULOKE; 


  /*----------------------------------------------------------------------*/
  /* Step 5 from PM-generic: synchronous return code                      */
  /*----------------------------------------------------------------------*/

  RM0003_RETURN (rs0700, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0701_cleanUp				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-06                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed   short int FAR PASCAL ro0701_cleanUp (
         signed  long int lEvent         ,
         unsigned long int hP1orhContext ,
         unsigned long int hP2           ,
         unsigned long int hP3           ,
         unsigned long int hP4           )

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
signed   short  int      rc = OK;

unsigned long   int      hAddr = LNULL;
unsigned long   int      hCldAdr = LNULL;
unsigned long   int      hAeTitle = LNULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0701)
 if (!lEvent) {/* ---------Free the Context ------------------- */
    if (ro0034_localfree   (hP1orhContext) != OK) rc = -R_LFREEE;
 }
 else {        /* ----------- Free th events ------------------ */    
    if (hP1orhContext)     rc = ro0246_clearPdvTree (&hP1orhContext);  
    if (hP3)  { 
       switch(lEvent){
            case RC0701_RO_BINDreq:
               {
                rT0701_stBindReqRopm * pSer = LNULL;
                pSer = (rT0701_stBindReqRopm *)ro0031_locallock( hP3);
                   if(pSer != LNULL){
                      hAddr = pSer->stAll.hCalledAddress;
                      hAeTitle= pSer->stAcpm.hAeTitle;
                   }
               }
               break;
            case RC0702_RO_BINDind:
                 {
                   rT0702_stBindIndRopm * pSer = LNULL;
                   pSer = (rT0702_stBindIndRopm *) ro0031_locallock (hP3);
                   if(pSer != LNULL){
                      hAddr = pSer->stAll.hCallingAddress;
                      hCldAdr = pSer->stAll.hCalledAddress;
                      hAeTitle= pSer->stAcpm.hAeTitle;
                   }

                 }
                 break;
            case RC0703_RO_BINDrsp:
                 if ((hP4  & RCro85_Redirection) == RCro85_Redirection) 
                     break;
                 else {
                   rT0703_stBindRspRopm * pSer = LNULL;
                   pSer = (rT0703_stBindRspRopm *) ro0031_locallock (hP3);
                   if(pSer != LNULL){
                      hAddr = pSer->stAll.hRespondAddress;
                      hAeTitle= pSer->stAcpm.hAeTitle;
                   }

                 }
                 break;
                              
            case RC0704_RO_BINDcnf:
                 {
                   rT0704_stBindCnfRopm * pSer = LNULL;
                   pSer = (rT0704_stBindCnfRopm *) ro0031_locallock (hP3);
                   if(pSer != LNULL){
                      hAddr = pSer->stAll.hRespondAddress;
                      hAeTitle= pSer->stAcpm.hAeTitle;
                   }
                 }
                 break;
        }/*end of switch of levent*/
        if (hAddr ) if (ro0034_localfree (hAddr) != OK ) rc = -R_LFREEE;
        if (hAeTitle ) if (ro0034_localfree (hAeTitle) != OK ) rc = -R_LFREEE;
        if (hP3) {
              if (ro0032_localunlock(hP3) != OK)            rc = -R_LULOKE;
              if (ro0034_localfree (hP3) != OK)            rc = -R_LFREEE;
        }
    }/* end if of hP3    */
  }/* end else of lEvent */    
         
                 
RM0003_RETURN (rs0701, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0702_encodRopm				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int FAR PASCAL ro0702_encodRopm (
                               
          rTro76_stDefinedBy FAR  * pDefinedBy   ,             
	  rTro01_pfvObjectId	    pTrfSynObjId ,
          unsigned long  int        hsStore      ,             
          void               NEAR * pPpmContext  ,             
          unsigned long  int FAR  * phRoseApdus  ,             
          unsigned long  int FAR  * pLength      ,       
          signed         int        iAnyNumb         )
                                                                                



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

signed   short  int      rc       = OK;

rT0715_unRoseApdus  FAR *pApdu    = LNULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0702)
 /* check on BER is removed for Bin. data by kc on 20/5    
 if (ro0003_objcmp (pTrfSynObjId,
		   (unsigned long int FAR *)vBerTrfSyn) == OK) {	*/
     pApdu = (rT0715_unRoseApdus FAR *) ro0021_globallock(*phRoseApdus);
     if (pApdu != LNULL){
         rc = ROSEapdus_cod (hsStore, -1, 0, pApdu, pPpmContext);            
         if (rc >= 0 )
             if (ro0022_globalunlock (*phRoseApdus) != OK)  rc = -R_GULOKE;
     } /*end if of successful lock */
     else rc = -R_GLOCKE;
 /*} by kc on 20/5 */ /*--- end if of BER Transfer Syn ---*/
 /* else*/ /*--- Other than BER ---*/
    /* rc = RCro6m_UnexpectedPpduParam; */
 
RM0003_RETURN (rs0702, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0703_decodeRopm          			      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0703_decodRopm (
signed   short int        sAbstrSynId,      /*  -> : see rouply0.h */     
signed   long  int        lEncodingChoice,  /*  -> : see rouply0.h */     
rTro76_stDefinedBy FAR  * pDefinedBy,       /*  -  : Not used.     */     
rTro01_pfvObjectId	  pTrfSynObjId,	    /*  -> : see rouply0.h */
unsigned long  int        hsStore,          /*  -> : see rostor0.h */     
void               NEAR * pPpmContext,      /*  -> :               */     
unsigned long  int FAR  * phRoseApdus,      /* <-> : see roropm1.h */     
unsigned long  int        ulLength,         /*  -> :               */     
signed         int        iAnyNumb )        /* --> : Any Number    */

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
signed   short int      rc        = OK;

/* rTro00_vObjectId       vBerTrfSyn = {3, 2, 1, 1};  */
rT0715_unRoseApdus FAR   *pApdu      = LNULL; 
unsigned       char    ucTagNum   = 0;  
unsigned long  int     ulLen      = NULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0703)
 /*
 if (ro0003_objcmp (pTrfSynObjId,
		   (unsigned long int FAR *)vBerTrfSyn) == OK) {
  Donot compare for all because of Bin data by kc on 17/05/93 */
 /*------------------------------------------------------------------ */
 /* Now the Ropm Data has to be decoded. If the ANY contains one      */
 /* of the Ropm Tags then goto decoding function .Otherwise skip      */
 /* Ropm decoding and go to Rose -USER                                */
 /*------------------------------------------------------------------ */
   
    ulLen = ro0062_storelength (hsStore);  
 
    ulLength = ulLength + ro0076_stwhere (hsStore);
    ucTagNum = (unsigned char) ro0077_stgetcp (hsStore);
                                            /* Reading 1 byte for Tag */
 /*--- Check for the Rose TAG num ---*/
 /* !!! to be changed constants defined in roropm3.h */
    if (  ( (ucTagNum | 32) != RC074a_AnyROIVapdu) &&
          ( (ucTagNum | 32) != RC074b_AnyRORSapdu) &&
          ( (ucTagNum | 32) != RC074c_AnyROERapdu) &&
          ( (ucTagNum | 32) != RC074d_AnyRORJapdu) &&
            ( !iAnyNumb                          )   )
     /*---- iAnyNumb will be NON-ZERO in the case of Invoke/Result/Error --*/
     /*---- iAnyNumb will be ZERO in the case of Bind/Unbind/Abort --------*/
     /*--- No Rose Apdu so decode ANY Rose-User ---*/
           rc = RTS_any_dec(hsStore, ulLength, phRoseApdus, 1, pPpmContext);
 /*--- with anyNumb as 1 it control goes to ROSE- User    ---*/

    else {  /*--- Rose Apdu  check for BER---*/
     /* here also don't use the check by kc on 20/5 
      if (ro0003_objcmp ((unsigned long int FAR *)*pTrfSynObjId,
			 (unsigned long int FAR *)vBerTrfSyn) == OK) {	*/
       /*--- Allocate PDV node & lock it ---*/

         *phRoseApdus = ro0020_globalalloc(sizeof(rT0715_unRoseApdus));
         if (*phRoseApdus != NULL ) { /*--- Successful Allocation ---*/
             pApdu = (rT0715_unRoseApdus FAR *) ro0021_globallock(*phRoseApdus);
             if (pApdu != LNULL)    { /*--- Successful locking ---*/
         
              /*--- Initialise Pdv Header ---*/
                 pApdu->stPdvHeader.sPmNumb         = RC0219_Rose; 
                 pApdu->stPdvHeader.sAbstrSynId     = sAbstrSynId;
                 pApdu->stPdvHeader.lEncodingChoice = lEncodingChoice;
                 pApdu->stPdvHeader.lFraCount       = (long) 1;
                 pApdu->stPdvHeader.vPdvHandle[0]   = *phRoseApdus;
  
               /*--- decode the Rose Apdus -----------*/
                rc = ROSEapdus_dec (hsStore ,ulLen, pApdu, pPpmContext );
                if ( rc >= OK ) 
                    if ( ro0022_globalunlock ( *phRoseApdus ) != OK )
                                                               rc = -R_GULOKE;
                
             } /*--- end if of Successful locking ---*/
             else    {
                 rc = -R_GLOCKE;
                 if (ro0024_globalfree ( *phRoseApdus ) != OK) rc =-R_GFREEE;
              }
         } /*--- end if of successful Allcation ---*/
         else                                          rc = -R_GALLCE;     
   /*   } else  rc = RCro6m_UnexpectedPpduParam; by kc on 20/5 */
    } /*--- end of else of Rose Apdu ---*/ 
RM0003_RETURN (rs0703, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0705_informRopm          			      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-06                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int FAR PASCAL ro0705_informRopm (
unsigned long  int      hContext,          
signed   short int      sInformType,      
void               FAR *pRequestedInfo )




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
signed   short int      rc = OK;

rT0730_stContext   NEAR * pContext = LNULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0705)
 if (hContext ) {
     pContext = (rT0730_stContext * ) ro0031_locallock(hContext);
     if (pContext != LNULL ) {
        switch ( (sInformType | RC0700_RoseEvent)) {
             case RC07z0_PossibilityInfo:
                   {
                     rT07z0_stPossibilityInfo FAR * pInfo = LNULL;
 
                     pInfo = (rT07z0_stPossibilityInfo FAR *) pRequestedInfo;
                     rc = ro070w_GetPossibilityInfo (pContext ,
                                                     pInfo );
                     
                   }
                   break;
        }/*--- end switch ---*/
        if(ro0032_localunlock(hContext)) rc = -R_LULOKE;;
     } /*--- end if of successful lock ---*/        
     else rc = -R_LLOCKE;
 }/*--- end if of hContext present ---*/
 else rc = -R_LLOCKE;
RM0003_RETURN (rs0705, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0704_clearNodeRopm                     	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int FAR PASCAL ro0704_clearNodeRopm( 

unsigned long int FAR * phApdu)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*     phRoseApdus            :Memory handle to PDV node (pointer to).*/
/* OUT-PARAMETERS             :                                       */
/*     phRoseApdus            :Memory handle to PDV node (pointer to).*/
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
signed   short int      rc = OK;

rT0715_unRoseApdus FAR * pApdu = LNULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0704)
 pApdu = (rT0715_unRoseApdus FAR * ) ro0021_globallock (*phApdu) ;
 if (pApdu == LNULL)
     RM0003_RETURN (rs0704, signed short int, -R_GLOCKE)  
 switch (pApdu->stPdvHeader.lType) { 
      case RC071P_ROIVapdu:
           if ( pApdu->stROIVapdu.bArgument )
               rc =ro0246_clearPdvTree(&pApdu->stROIVapdu.hArgument );
           break;
      case RC071Q_RORSapdu:
           if ( pApdu->stRORSapdu.bOperationResult )
               rc =ro0246_clearPdvTree(&pApdu->stRORSapdu.stOperationResult.
                                                       hResult );
           break;
      case RC071R_ROERapdu:
           if ( pApdu->stROERapdu.bParameter )
               rc =ro0246_clearPdvTree(&pApdu->stROERapdu.hParameter );
           break;
  } /* end of switch of lType */         
  if (rc == OK) {
       if (ro0022_globalunlock (*phApdu) != OK) rc = -R_GULOKE;        
       else if (ro0024_globalfree   (*phApdu) != OK) rc = -R_GFREEE;  
       else *phApdu = LNULL; 
  }      
RM0003_RETURN (rs0704, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0707_GiveProviderRejInd                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed  short int NEAR PASCAL ro0707_GiveProviderRejInd(
rT0730_stContext   NEAR * pContext,       
signed   long  int FAR  * plOutEvent,
signed   long  int        lReason,
unsigned long  int FAR  * phP1,           
unsigned long  int FAR  * phP2,          
unsigned long  int FAR  * phP3,         
unsigned long  int FAR  * phP4        )

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
signed   short int      rc = OK;

unsigned long  int        hHd     = LNULL;
rT070j_stPRejInd   NEAR * pRjPInd = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0707)

    hHd = ro0030_localalloc (sizeof(rT070j_stPRejInd) );
    if (hHd == LNULL ) rc = R_LALLCE;
    else {    /* ------- Successful Allocation ----------- */
         pRjPInd = (rT070j_stPRejInd * ) ro0031_locallock (hHd);
             if (pRjPInd == LNULL ) rc = R_LLOCKE;

             else { /* -------- Sucessful Alloc & Lock ------------- */
   
                 /* ---------- User Reject ind------------------------ */
                     if (pContext->bSyncFlag == TRUE){
                         pRjPInd->lInvokeID = pContext->lInvokeId;
                         pRjPInd->bInvokeID = TRUE ;
                     }
                     else 
                         pRjPInd->bInvokeID = FALSE ;

                 pRjPInd->bRejectReason = FALSE;
                 /* !!! */
                                      
                 /* ----------- Pdv Node is Over ------------------ */
                 
                 if ( (ro0032_localunlock(hHd) ) != OK )
                         rc = R_LULOKE;
                 
                /*  ------- Assign Output control prrameters ----- */
                 *phP3 = hHd;
                 *phP1 = LNULL;
                 *plOutEvent =  RC070k_RO_RJPind;
                 pContext->sSubAutoNum =  RC073j_End ;
                 rc           = RC073k_PushOutEvent;
             } /* end else of Successful Alloc & lock */
          } /*end else of Successful Alloc */
RM0003_RETURN (rs0707, signed short int, rc)                                    
}/*end*/                                                                        



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0708_ProviderAbort                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-03                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0708_ProviderAbort (
rT0730_stContext   NEAR  *pContext,       
signed   long  int        lInEvent,
signed         int        sRetCode,
unsigned long  int        hP1,           
unsigned long  int        hP2,          
unsigned long  int        hP3,         
unsigned long  int        hP4        )
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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0708)
 rc = ro0701_cleanUp (lInEvent, hP1, hP2, hP3, hP4 );
 

 if (rc == OK ){
       pContext->sStateRopm =
       pContext->sStateTrPm = NULL;
       pContext->sSubAutoNum = RC073j_End;
       rc = ro0211_pushOutEvent ( RC0221_RoseUser,
                              RC070b_RO_PABORTind,
                              0                  ,
                              0                  ,
                              0                  ,
                              0                  ); /*!!! reason*/
       if (pContext->bmappingInfo ) { 
       rc = ro0211_pushOutEvent ( RC0218_Rtse,
                              RC100h_RT_U_ABORTreq,
                              0                  ,
                              0                  ,
                              0                  ,
                              0                  ); 
       }
       else rc = ro0211_pushOutEvent ( RC0217_LcrtFct,
                              RC0609_AA_ABORTreq,
                              0                  ,
                              0                  ,
                              0                  ,
                              0                  ); 
    }  

RM0003_RETURN (rs0708, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0709_ProviderReject                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0709_ProviderReject (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4,        
signed   short int        sAbsId,
signed   short int        bInvokeId,
signed   long  int        lInvokeId)

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
signed   short int      rc = OK;
rT0715_unRoseApdus  FAR *pPdv = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0709)
 rc = ro0701_cleanUp (*plInEvent,*phP1,*phP2,*phP3,*phP4);

 if (rc == OK ) {
    *phP1 = *phP2 = *phP3 = *phP4 = LNULL;
    *phP1 = ro0020_globalalloc ( sizeof (rT0715_unRoseApdus));
     pPdv = (*phP1) ? (rT0715_unRoseApdus FAR * ) ro0021_globallock (*phP1)
                    : LNULL;
     if (pPdv != NULL){
                 pPdv->stPdvHeader.lFraCount     = (long int)1;
                 pPdv->stPdvHeader.vPdvHandle[0] = *phP1;
                 pPdv->stPdvHeader.sPmNumb       = RC0219_Rose;
                 pPdv->stPdvHeader.sAbstrSynId   = sAbsId;
                 pPdv->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
                 pPdv->stPdvHeader.lType = RC071S_RORJapdu;

                 /* -------- Fill the body -------------------------- */
                 if (bInvokeId ==FALSE )
                      pPdv->stRORJapdu.lInvokeIDChoiceNum = RC071Q_NoInvId;
                 else 
                      pPdv->stRORJapdu.lInvokeIDChoiceNum =  RC071P_InvIdPrest;
                      pPdv->stRORJapdu.lInvokeID = lInvokeId;
                      
                 pPdv->stRORJapdu.lProblemChoiceNum = RC0712_GeneralProblem;
                 pPdv->stRORJapdu.lProblem = RC0716_unrecognisedAPDU;
                
                 *plOutEvent = RC073w_TRANSreq;
                 pContext->sSubAutoNum =  (pContext->bmappingInfo )
                                         ? RC073h_RopmPrTr
                                         : RC073i_RopmRtTr;
                 if (ro0022_globalunlock (*phP1) ) rc = -R_GULOKE;
      }
       else rc = -R_GALLCE;
   }
             
RM0003_RETURN (rs0709, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro070s_GetRtseAbsId                   	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-07                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro070s_GetRtseAbsId ( 
signed short int      * psAbsId)


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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs070s)
 *psAbsId = 1;
 /* !!! to be changed later 7/4/93 */
RM0003_RETURN (rs070s, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro070t_ClearContext                     	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
static signed short int PASCAL ro070t_ClearContext (
 rT0730_stContext  NEAR * pContext)



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
signed   short int      rc = OK;

unsigned  long  int      ulP1;
unsigned  long  int      ulP2;
unsigned  long  int      ulP3;
unsigned  long  int      ulP4;
signed    long  int      lOutEvent;
/* signed    long  int      lInEvent;  removed  during dos compilation -sss */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs070t)

 while ( rc == OK ) {
     rc = ro070v_GetFromQueue ( pContext,
                                (long FAR *)&lOutEvent,
                                (unsigned  long FAR *)&ulP1,
                                (unsigned  long FAR *)&ulP2, 
                                (unsigned  long FAR *)&ulP3, 
                                (unsigned  long FAR *)&ulP4 );
     if ( rc > OK ) {
          if (lOutEvent == RC073w_TRANSreq )
             rc = ro0707_GiveProviderRejInd ( pContext,
                                             (long FAR *) &lOutEvent,
                                              0,
                                              (unsigned  long FAR *)&ulP1,
                                              (unsigned  long FAR *)&ulP2,
                                              (unsigned  long FAR *)&ulP3,
                                              (unsigned  long FAR *)&ulP4);
     }
     else break;
  }  
 
RM0003_RETURN (rs070t, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro070u_PutInQueue        			      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro070u_PutInQueue (

rT0730_stContext  NEAR  *pContext,       
signed   long  int       lEvent,
unsigned long  int       hP1,           
unsigned long  int       hP2,          
unsigned long  int       hP3,         
unsigned long  int       hP4        )
                                
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
signed   short int      rc = OK;
unsigned long  int         hQue = LNULL;
rT0731_stTransQue   NEAR * pQue = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs070u)
  if (lEvent ) {
       hQue = ro0030_localalloc ( sizeof (rT0731_stTransQue ) );
       if (hQue != LNULL ) {
           pQue = (rT0731_stTransQue *) ro0031_locallock (hQue) ;
           if (pQue != LNULL ) {
                pQue->lEvent = lEvent;
                pQue->vParams[0] = hP1;
                pQue->vParams[1] = hP2;
                pQue->vParams[2] = hP3;
                pQue->vParams[3] = hP4;
                pQue->hNext      = LNULL;
                if (ro0032_localunlock (hQue ) ) rc = -R_LULOKE;
                else {
                      pQue =(rT0731_stTransQue NEAR *)
                                   ro0031_locallock (pContext->hLast) ;
                      if ( pQue ) {
                         pQue->hNext = hQue;
                         if (ro0032_localunlock (pContext->hLast) )
                                   rc = -R_LULOKE;
                         else pContext->hLast = hQue;
                       }/*end if of lock of lsat */
                 }/*end else of unlock pQue */
             }/*end if of lock pQue */
             else  rc = -R_LLOCKE;
        }/*end if of alloc */
        else  rc = -R_LALLCE;
  }
  else rc = -R_ONEVNT;
RM0003_RETURN (rs070u, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro070v_GetFromQueue                     	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro070v_GetFromQueue (

rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4        )
                                

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
signed   short int      rc = OK;

unsigned long  int         hQue = LNULL;
rT0731_stTransQue   NEAR * pQue = LNULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs070v)
 /* --- Get the event from the firsr of the queue */
 if (pContext->hFirst)  {
     pQue = (rT0731_stTransQue *) ro0031_locallock ( pContext->hFirst );
     if (pQue ) {
         *plEvent = pQue->lEvent;
         *phP1    = pQue->vParams[0];
         *phP2    = pQue->vParams[1];
         *phP3    = pQue->vParams[2];
         *phP4    = pQue->vParams[3];
         hQue     = pQue->hNext;
         if ( ro0032_localunlock ( pContext->hFirst ) ) rc = -R_LULOKE;
         else if ( ro0034_localfree (pContext->hFirst ) ) rc = -R_LFREEE;
         else {
               pContext->hFirst = hQue;
               rc               = 1;
         }
      }
      else rc = -R_LLOCKE;
     }
RM0003_RETURN (rs070v, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro070w_GetPossibilityInfo                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
static signed  short int NEAR PASCAL ro070w_GetPossibilityInfo (
rT0730_stContext         NEAR * pContext,
rT07z0_stPossibilityInfo FAR  * pInfo)

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
signed   short int      rc = OK;
signed   short int      sIndex = NULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs070w)

 pInfo->bPossible = TRUE;
 /*--- Check 1 - State-Automata ----*/
 rc = ro070z_SearchAndGetIndex ( pContext ,
                                 pInfo->lEvent,
                                 RC073g_RopmPm,
                                 (signed short int FAR *)&sIndex );
 if ( (rc == OK) && (sIndex) ) {
 /*--- Check 2 - for particular event ---- */
   switch(pInfo->lEvent){
     case RC070c_RO_INVreq:
          
          if ( ( (pContext->bAssocInit  ==  TRUE               ) && 
                 (pContext->ulAssocClas ==  RC070m_AssocClass2 ) ) ||

               ( (pContext->bAssocInit == FALSE              ) &&
                 (pContext->ulAssocClas == RC070l_AssocClass1)   ) ||

               (pContext->bSyncFlag   ==  TRUE               )      ) 
             pInfo->bPossible = FALSE;
          break;
     case RC070e_RO_RESreq:
     case RC070g_RO_ERRreq:
          if ( ( (pContext->bAssocInit == TRUE                  ) && 
                 ( pContext->ulAssocClas ==  RC070l_AssocClass1 )  ) ||
               
               ( (pContext->bAssocInit == FALSE                 ) && 
                 ( pContext->ulAssocClas ==  RC070m_AssocClass2 )  ) ||
                
               ( (pContext->bSyncFlag == TRUE              )&&
                 (pContext->lInvokeId != pInfo->lInvokeId  )   )   ) 
             pInfo->bPossible = FALSE;
          break;

     case RC0703_RO_BINDrsp:
          if (pContext->bAssocInit)        /* **skh: 19/7/93 */
              pInfo->bPossible = FALSE;
          else
              pInfo->lEvent = RC0603_AA_ASSOCIATErsp;
          break; 

     case RC0705_RO_UNBINDreq:
          if (!pContext->bAssocInit)        /* **skh: 19/7/93 */
              pInfo->bPossible = FALSE;
          else
              pInfo->lEvent = RC0605_AA_RELEASEreq;
          break; 

     case RC0707_RO_UNBINDrsp:
          if ((!pContext->sReleaseFg) || (pContext->bAssocInit)) /***skh:19/7 */
             pInfo->bPossible = FALSE;
          else
             pInfo->lEvent = RC0607_AA_RELEASErsp;
          break;        
      
     case RC0709_RO_UABORTreq:
          pInfo->lEvent = RC0609_AA_ABORTreq;
          break; 

   }/*--- end switch -----*/
 }/*--- end if of Index OK ---*/
 else pInfo->bPossible = FALSE;
 if ( (rc == OK) && ( pInfo->bPossible == TRUE) ){
             /* check only if OK in ROSE */
    switch(pInfo->lEvent){
          case RC0603_AA_ASSOCIATErsp :
          case RC0605_AA_RELEASEreq   :
          case RC0607_AA_RELEASErsp   :
          case RC0609_AA_ABORTreq     :
                rc = ro0231_getInform (
                         pInfo->ulAcb,
                      /* ro0214_getAcbEcb ((unsigned long int FAR *)NULL), */
                         RC0216_Acse                                    ,
                         RC06z4_CheckLegal                              ,
                         pInfo  );
                break;

    } 

 }
RM0003_RETURN (rs070w, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro070y_GetApduType              		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

/*
static signed  short int NEAR PASCAL ro070y_GetApduType(

       unsigned long  int             ulP1,
       signed   long  int  NEAR *     plInEvent)
*/

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
/**********************====================
{-*entr*-
-* ****************** l o c a l    v a r i a b l e s **************** *-
-* ******************                                **************** *-
signed   short  int        rc = OK;
rT0715_unRoseApdus  FAR * pApdus;

-* ****************** S T A R T ************************************* *-
-* ******************           ************************************* *-

RM0002_ENTRY (rs070y)
   pApdus = (rT0715_unRoseApdus FAR *) ro0021_globallock (ulP1) ;
   if (pApdus == NULL) 
       rc = -R_GLOCKE;
   else {
      switch(pApdus->stPdvHeader.lType){
          case RC071P_ROIVapdu:
               *plInEvent = RC073Q_ROINV;
               break; 
          case RC071Q_RORSapdu:
               *plInEvent = RC073R_RORES;
               break; 

          case  RC071R_ROERapdu:
               *plInEvent = RC073S_ROERR;
               break; 

          case RC071S_RORJapdu:
               switch(pApdus->stRORJapdu.lProblemChoiceNum){
                   case RC0712_GeneralProblem:
                           *plInEvent = ( (pApdus->stRORJapdu.lProblem  >= 0)&&
                                          (pApdus->stRORJapdu.lProblem  <= 2) )
                                        ? RC073U_RORJP
                                        : RC073V_APDUua;
                           break;
                   case RC0713_InvokeProblem:
                           *plInEvent = ( (pApdus->stRORJapdu.lProblem  >= 0)&&
                                          (pApdus->stRORJapdu.lProblem  <= 7)&&
                                          (pApdus->stRORJapdu.lInvokeIDChoiceNum
                                           == RC071P_InvIdPrest)            )
                                        ? RC073U_RORJP
                                        : RC073V_APDUua;
                           break;
                   case RC0714_ReturnResultProblem:
                           *plInEvent = ( (pApdus->stRORJapdu.lProblem  >= 0)&&
                                          (pApdus->stRORJapdu.lProblem  <= 2)&&
                                          (pApdus->stRORJapdu.lInvokeIDChoiceNum
                                           == RC071P_InvIdPrest)            )
                                        ? RC073U_RORJP
                                        : RC073V_APDUua;
                           break;
                   case  RC0715_ReturnErrorProblem :
                           *plInEvent = ( (pApdus->stRORJapdu.lProblem  >= 0)&&
                                          (pApdus->stRORJapdu.lProblem  <= 4)&&
                                          (pApdus->stRORJapdu.lInvokeIDChoiceNum
                                           == RC071P_InvIdPrest)            )
                                        ? RC073U_RORJP
                                        : RC073V_APDUua;
                           break;
                   default:
                           *plInEvent = RC073V_APDUua ;
                           break;
               }-* end of ROJU switch *-                                            
               break; 
          default:
               *plInEvent = RC073V_APDUua;
               break; 
    }-*end of the switch *-
    if (ro0022_globalunlock (ulP1) ) rc = -R_GULOKE;
  }-*end of else *-

RM0003_RETURN (rs070y, signed short int, rc)                                    
}-*end*-                                                                        
====================**********************/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro070a_encodSniPRopm                                        */
/*                                                                            */
/* AUTHOR       : Vinita                                                      */
/* DATE         : 93-04-08                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro070a_encodSniPRopm ( 

unsigned long int          hStore,
void                NEAR  *pPpmContext,
unsigned long int   FAR   *phRoseApdus)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     pStore         :  Buffer holding encoded data string (pointer to).     */
/*     pPpmContext    :  PPM context (pointer to).                            */
/*     hRoseApdus     :  Memory handle to PDV node (pointer to).              */
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

{/*entr-ro070a_encodSniPRopm*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed   short int     sRetCode=0;
rT0715_unRoseApdus FAR *pRoseApdus;
signed   long  int     lType;
/* char           FAR *pOrigBuffer;  removed during compilation on dos   */
/* char           FAR *pSendBuffer;  removed during compilation on dos   */
/* char           FAR *pStoreAddress; removed during compilation on dos   */
/* unsigned long  int  ulLength;  removed during compilation on dos   */
unsigned long  int  PdvNode=0;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (rs070a)

pRoseApdus = (rT0715_unRoseApdus FAR *) ro0021_globallock (*phRoseApdus);

lType = pRoseApdus->stPdvHeader.lType;  

 /*pOrigBuffer=pSendBuffer; */ /* preserve original position of the buffer */

switch (pRoseApdus->stPdvHeader.lType) {

   case RC071P_ROIVapdu:
     sRetCode = ROIVSniP_type_cod(hStore,&(pRoseApdus->stROIVapdu),
                pPpmContext); 
     if (pRoseApdus->stROIVapdu.bArgument)
        PdvNode = pRoseApdus->stROIVapdu.hArgument;
  
     break;
   case RC071Q_RORSapdu:
     sRetCode = RORSSniP_type_cod(hStore,&(pRoseApdus->stRORSapdu),
                pPpmContext); 
     if (pRoseApdus->stRORSapdu.bOperationResult)
        PdvNode = pRoseApdus->stRORSapdu.stOperationResult.hResult;
     break;
   case RC071R_ROERapdu:
     sRetCode = ROERSniP_type_cod(hStore,&(pRoseApdus->stROERapdu),
                pPpmContext); 
     if (pRoseApdus->stROERapdu.bParameter)
         PdvNode = pRoseApdus->stROERapdu.hParameter;
     break;
   case RC071S_RORJapdu:
     sRetCode = RORJSniP_type_cod(hStore,&(pRoseApdus->stRORJapdu),
                pPpmContext); 
     break;

  }  /* end switch lType  */
    
     /* find length of the data encoded  so far */
/*       ulLength = (pSendBuffer - pOrigBuffer);  */

     /* get store address to copy encoded string to store */
/*      pStoreAddress = (char FAR *)ro0069_stputs (*pStore, ulLength);
      if (pStoreAddress  == NULL)      
        RM0003_RETURN (rs070a, signed short int, -R_STOERR) */

     /* copy encoded data string  to  store */
/*      ro0041_memcpy (pStoreAddress, pOrigBuffer, ulLength);  */

     /* position store pointer to new position */
/*     sRetCode = ro0066_storepos(*pStore,ulLength) ; */

     /* call despatcher to switch encoding */ 
     if(PdvNode)
     sRetCode = ro0242_encodSniP (RC0221_RoseUser, hStore, 
                pPpmContext,(unsigned long FAR *)&PdvNode); 
     else {
        sRetCode  = ro0067_stputc (hStore,0);
        sRetCode  = ro0067_stputc (hStore,0);
        sRetCode  = ro0067_stputc (hStore,0);
        sRetCode  = ro0067_stputc (hStore,0);
     }/* for no User Data */
 

    if (ro0022_globalunlock (*phRoseApdus) != OK)
    sRetCode  = -R_GLOCKE; 
    RM0003_RETURN (rs070a, signed short int, sRetCode)

}/*end-ro070a_encodSniPRopm*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro070b_decodSniPRopm                                        */
/*                                                                            */
/* AUTHOR       : Vinita                                                      */
/* DATE         : 93-04-07                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */


signed short int PASCAL ro070b_decodSniPRopm ( 
signed   short int  FAR   * psAbsId,
unsigned long int           hStore,
void                NEAR  * pPpmContext,
unsigned long int   FAR   * phRoseApdus)

{/*entr-ro070b_decodSniPRopm*/

/* *********************** l o c a l    v a r i a b l e s ******************* 
*/
/* ***********************                                ******************* */

signed   short int         sRetCode = OK;
rT0715_unRoseApdus  FAR   *pRoseApdus;
char                FAR   *pBuffActual;
/* char             FAR   *pSendBuffer;removed during compilation on dos   */
/* char             FAR   *pStoreAddress;removed during compilation on dos */
unsigned long  int         ulLength = NULL;
unsigned long  int  FAR   *PdvNode = LNULL;
signed   long  int         i=0;
/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (rs070b)

  /* **** get store address to copy encoded string to store **** */

     ulLength  = ro0076_stwhere (hStore);

     pBuffActual = ro0072_stgets (hStore, (long) (2 *sizeof(char)));      
     if (pBuffActual == NULL ) 
        RM0003_RETURN (rs070b, signed short int, -R_STOERR)

/*     pBuffActual = pStoreAddress; */

     
     *phRoseApdus  = ro0020_globalalloc ( sizeof (rT0715_unRoseApdus));
     pRoseApdus = (rT0715_unRoseApdus FAR *) ro0021_globallock (*phRoseApdus);

  /* ************* decode ROPM Mask Information ************ */

     RM0013_DECODEi (i, RC04yb_sPrivPciId, char) 
  
  /* ************* decode Mask Information ********************* */    
                                                                
     RM0013_DECODEi (pRoseApdus->stPdvHeader.lType, RC07y0_Rose_apduType,
                     char)                           
     pRoseApdus->stPdvHeader.lType -= 1;  /* by kc for compatible with this */
     sRetCode = ro0066_storepos ( hStore,(ulLength+2));
     if (sRetCode == OK ) {
       switch (pRoseApdus->stPdvHeader.lType) {

          case RC071P_ROIVapdu:
            sRetCode = ROIV_type_dec(hStore,&(pRoseApdus->stROIVapdu),
                       pPpmContext); 
            if ( pRoseApdus->stROIVapdu.bArgument )
                PdvNode = (unsigned long int FAR *)
                              &pRoseApdus->stROIVapdu.hArgument;
            break;

          case RC071Q_RORSapdu:
            sRetCode = RORS_type_dec(hStore,&(pRoseApdus->stRORSapdu),
                       pPpmContext); 
            if (pRoseApdus->stRORSapdu.bOperationResult)
                PdvNode = (unsigned long int FAR *)
                              &pRoseApdus->stRORSapdu.stOperationResult.hResult;
            break;

          case RC071R_ROERapdu:
            sRetCode = ROER_type_dec(hStore,&(pRoseApdus->stROERapdu),
                       pPpmContext); 
            if ( pRoseApdus->stROERapdu.bParameter)
                PdvNode = (unsigned long int FAR *)
                              &pRoseApdus->stROERapdu.hParameter;
            break;

          case RC071S_RORJapdu:
            sRetCode = RORJ_type_dec(hStore,&(pRoseApdus->stRORJapdu),
                       pPpmContext); 
            break;

          default: 
            sRetCode = RCro6l_UnrecognizedPpduParam;
            break;
       }  /* end switch lType  */
    

     /* call despatcher to switch decoding */ 
     if  (PdvNode )
       sRetCode = ro0243_decodSniP (RC0221_RoseUser, hStore, 
                pPpmContext,PdvNode,psAbsId); 
    }
    if (ro0022_globalunlock (*phRoseApdus) != OK)
        sRetCode  = -R_GLOCKE; 
    RM0003_RETURN (rs070b, signed short int, sRetCode)

}/*end-ro070b_decodSniPRopm*/
/* EOF*/
