/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropm4.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:02  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:41  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:16  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:36  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:06  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:20  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:06:33  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:55  keutel
 * 	creation
 * 	[1994/03/21  13:35:22  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roropm4.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:02 $" } ;
#endif

/* ****************************************************************** */
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
/* NAME         : roropm4.c                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-01                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Automata Functions  of RTPM-TR table A.7.           */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropm4.c	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-04-01|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* ****************************************************************** */
/*                  I n c l u d e  F i l e s                          */
/* ****************************************************************** */

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

#include <rortpm0.h>
#include <rortpm1.h>
#include <roacpm0.h>

#include <roppm00.h>


#include <roropm0.h>      /* Interface definitions                    */
#include <roropm1.h>      /* PDU data structures                      */
#include <roropm3.h>      /* Local Events & Data structures           */
#include <roropm4.h>      /* Automata Function proto TYpe             */
/** Not needed Baiju , GNU GCC 
#include <roropm5.h>      =* Static Automata Table                    *=
**/
#include <roropm6.h>      /* Internally used  function prototypes.    */


/* ****************** Logging function name strings ***************** */
/* ******************                               ***************** */
RM0001_STRG (rs0740, "ro0740_STA00xStartE")
RM0001_STRG (rs0741, "ro0741_STA00xRT_OPE")

RM0001_STRG (rs0742, "ro0742_STA10xAA_EST")
RM0001_STRG (rs0743, "ro0743_STA10xAA_End")

RM0001_STRG (rs0744, "ro0744_STA20_22xAA_")

RM0001_STRG (rs0745, "ro0745_STA10xRT_OPE")
RM0001_STRG (rs0746, "ro0746_STA10xRT_CLO")

RM0001_STRG (rs0747, "ro0747_STA20_22xABO")
RM0001_STRG (rs0748, "ro0748_STA20xAA_REL")
RM0001_STRG (rs0749, "ro0749_STA20xTRANSr")
RM0001_STRG (rs074a, "ro074a_STA20xRT_CLO")
RM0001_STRG (rs074b, "ro074b_STA20xRT_TUR")

RM0001_STRG (rs074c, "ro074c_STA21xAA_REL")
RM0001_STRG (rs074d, "ro074d_STA21_23xAA_")
RM0001_STRG (rs074e, "ro074e_STA21xRT_TRc")
RM0001_STRG (rs074f, "ro074f_STA21xRT_TRc")
RM0001_STRG (rs074g, "ro074g_STA21xRT_TPi")    

RM0001_STRG (rs074h, "ro074h_STA22xTRANSr")    
RM0001_STRG (rs074i, "ro074i_STA22xRT_TRi")   
RM0001_STRG (rs074j, "ro074j_STA22xRT_TGi")   

RM0001_STRG (rs074k, "ro074k_STA23xRT_TRi")    
RM0001_STRG (rs074l, "ro074l_STA23xRT_TGi")   
RM0001_STRG (rs074m, "ro074m_STA21_23xTRA")
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0740_STA00xStartESTAB                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0740_STA00xStartESTAB (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        
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
signed   short int      rc       = OK;
rT0701_stBindReqRopm   *pBindRq  = LNULL;

unsigned long  int      hHdSer   = LNULL;
rT100a_stOpenReqRtpm   *pOPENreq = LNULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0740)
 /* ------ Lock the Service parameters ------------------------------- */
 pBindRq = (rT0701_stBindReqRopm NEAR *)ro0031_locallock ( *phP3);
 if (!pBindRq) rc = -R_LLOCKE;
 if (rc == OK ) {
     if ( *phP4 == 0) {   /* RTSE included !!! */
          hHdSer = ro0030_localalloc  (sizeof (rT100a_stOpenReqRtpm));
          if      (hHdSer == LNULL)   rc = -R_LALLCE;
          else {  /* Successful Allocation */
               pOPENreq = (rT100a_stOpenReqRtpm *) 
                                    ro0031_locallock (hHdSer);
               if (pOPENreq  == LNULL) {
                     rc = -R_LLOCKE;     
               }
               else { /* successful Allocl & lock */
                     /* ------ Copy the structures for All,Ppm &Acpm --- */
                     pOPENreq->stAll = pBindRq->stAll;
                     pOPENreq->stPpm = pBindRq->stPpm;
                     pOPENreq->stAcpm = pBindRq->stAcpm;
                   /* -------- Fill the RTSE Service parameters ------- */
                     pOPENreq->stRtpm.ulCheckpointSize     = 
                                     pBindRq->stRopm.ulCheckPointSize;
                     pOPENreq->stRtpm.ulWindowSize         = 
                                     pBindRq->stRopm.ulWindowSize;
                     pOPENreq->stRtpm.ulDialogueMode       = 
                                     RC1011_TwoWayAlternate;

                     pOPENreq->stRtpm.ulInitialTurn =
                              (pBindRq->stRopm.ulAssocClass  != 
                                   RC070m_AssocClass2 )
                            ? RC1015_TurnInitiator
                            : RC1016_TurnResponder  ;
                     
                                     
                  /* ------!!! ----------------------- */
                     pOPENreq->stRtpm.lApplicationProtocol = RCro73_P1ApplProt;
                     rc = ro070s_GetRtseAbsId ( 
                                     &(pOPENreq->stRtpm.sAbstrSynId) );
                    /* ------ Rtse is Over --------------------------- */
                    /* --------- unlock the Rtse Service parameter ------ */
                     if ((ro0032_localunlock(hHdSer) ) != OK) {
                         rc = -R_LULOKE;
                     }/* end of unlock error if */

               } /*end of succussful allok &lock else */
           } /*end of successful allock only */ 
     }/*end of the hP4 if- */
     if ((ro0032_localunlock(*phP3) ) == OK) {
         if(ro0034_localfree (*phP3 ) != OK ) rc = -R_LFREEE;
     }else rc = -R_LULOKE;

     if(rc==OK) {
         /* ------ Assign the OutEvent --------------------------- */
         pContext->sStateTrPm   = RC073a_STA10;
         pContext->sSubAutoNum  = RC073j_End;
         rc                     = RC073k_PushOutEvent;
         *plOutEvent            = RC1001_RT_OPENreq;
         *phP3                   = hHdSer;
         pContext->bToken      =
                   (pBindRq->stRopm.ulAssocClass  != RC070m_AssocClass2 )
                   ? TRUE
                   : FALSE  ;
         pContext->ulAssocClas  = pBindRq->stRopm.ulAssocClass;
     }
 }/*end of the successful lock of hP3 */ 
RM0003_RETURN (rs0740, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0741_STA00xRT_OPENind                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0741_STA00xRT_OPENind (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        
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
signed   short int          rc       = OK;
rT100b_stOpenIndRtpm NEAR * pOpenInd = LNULL;
unsigned long  int          hHdSer   = LNULL;
rT0702_stBindIndRopm NEAR * pBindInd = LNULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0741)
 /* ------ Lock the Service parameters ------------------------------- */
 pOpenInd = (rT100b_stOpenIndRtpm *) ro0031_locallock ( *phP3);
 if (!pOpenInd) rc = -R_LLOCKE;
 if (rc == OK ) {

         /* ------ Check the Indication ------------------------- */
         if( pOpenInd->stRtpm.ulDialogueMode != RC1011_TwoWayAlternate );
    /* !!! check has to be made for sAbsId,also */
 


          hHdSer = ro0030_localalloc  (sizeof (rT0702_stBindIndRopm));
          if      (hHdSer == LNULL)   rc = -R_LALLCE;
          else {  /* Successful Allocation */
               pBindInd = (rT0702_stBindIndRopm *) 
                                    ro0031_locallock (hHdSer);
               if (pBindInd  == LNULL) {
                     rc = -R_LLOCKE;     
               }
               else { /* successful Allocl & lock */
                     /* ------ Copy the structures for All,Ppm &Acpm --- */
                     pBindInd->stAll = pOpenInd->stAll;
                     pBindInd->stPpm = pOpenInd->stPpm;
      /* temp change for the new parameter ( called AET & AP,AE Invocs *
                     pBindInd->stAcpm = pOpenInd->stAcpm;     */
                     pBindInd->stAcpm.sApplContextId=
                                        pOpenInd->stAcpm.sApplContextId;
                     pBindInd->stAcpm.hAeTitle=
                                        pOpenInd->stAcpm.hAeTitle;
                     pBindInd->stAcpm.stInvoc=
                                        pOpenInd->stAcpm.stInvoc;
            
                     /* --------- Copy Over ---------------------------- */
                     pContext->bToken = 
                       (pOpenInd->stRtpm.ulInitialTurn == 
                                         RC1015_TurnInitiator)
                      ? FALSE
                      : TRUE  ;
 
                    /* --------- unlock the Acpm Service parameter ------ */
                     if ((ro0032_localunlock(hHdSer) ) != OK) {
                         rc = -R_LULOKE;
                     }/* end of unlock error if */

               } /*end of succussful allok &lock else */
           } /*end of successful allock only */ 
            
     if  (rc == OK) {
         pContext->sStateTrPm   = RC073a_STA10 ;
         pContext->ulAssocClas  = RC070n_AssocClass3; /* Default */ 
         pContext->bmappingInfo = FALSE;              /*  RTSE */
         pContext->bAssocInit = FALSE;              /*  Responder */
     }
     if ((ro0032_localunlock(*phP3) ) == OK) {
         if(ro0034_localfree (*phP3 ) != OK ) rc = -R_LFREEE;
     }else rc = -R_LULOKE;
     if(rc==OK) {
         /* ------ Assign the OutEvent --------------------------- */
         *plOutEvent  =RC073K_AA_StartESTAB;
         *phP3         =hHdSer;
         pContext->sSubAutoNum  = RC073g_RopmPm;
     }
 }/*end of the successful lock of hP3 */ 
RM0003_RETURN (rs0741, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0742_STA10xAA_ESTAB                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0742_STA10xAA_ESTAB (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        
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
rT0703_stBindRspRopm  *  pBindRsp  = LNULL;
unsigned long  int       hHdSer    = LNULL;
rT100c_stOpenRspRtpm  *  pOpenRsp  = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0742)

 /* *** check for the Redirection  ***** */
 if ( ( *phP4 & RCro85_Redirection) == RCro85_Redirection ) {
         *plOutEvent  =RC0603_AA_ASSOCIATErsp;
         pContext->sSubAutoNum  = RC073j_End;
         rc = RC073k_PushOutEvent;
         pContext->sStateTrPm   = RC0739_STAnoTr2;
         RM0003_RETURN (rs0742, signed short int, rc)
 }                                    
 /* ------ Lock the Service parameters ------------------------------- */
 pBindRsp = (rT0703_stBindRspRopm *)ro0031_locallock ( *phP3);
 if (!pBindRsp) rc = -R_LLOCKE;
 if (rc == OK ) {
          hHdSer = ro0030_localalloc  (sizeof (rT100c_stOpenRspRtpm));
          if      (hHdSer == LNULL)   rc = -R_LALLCE;
          else {  /* Successful Allocation */
               pOpenRsp = (rT100c_stOpenRspRtpm *) 
                                    ro0031_locallock (hHdSer);
               if (pOpenRsp  == LNULL) {
                     rc = -R_LLOCKE;     
               }
               else { /* successful Allocl & lock */
                     /* ------ Copy the structures for All,Ppm &Acpm --- */
                     pOpenRsp->stAll = pBindRsp->stAll;
                     pOpenRsp->stPpm = pBindRsp->stPpm;
                     pOpenRsp->stAcpm = pBindRsp->stAcpm;
                    /*--- Rtpm Parameters  -----------------------------*/  
                     pOpenRsp->stRtpm.ulCheckpointSize = 
                               pBindRsp->stRopm.ulCheckPointSize;
                     pOpenRsp->stRtpm.ulWindowSize     = 
                               pBindRsp->stRopm.ulWindowSize;
                    /*--- Rtpm parameters are Over --------------------- */
                    /* --------- unlock the Acpm Service parameter ------ */
                     if ((ro0032_localunlock(hHdSer) ) != OK) {
                         rc = -R_LULOKE;
                     }/* end of unlock error if */

               } /*end of succussful allok &lock else */
           } /*end of successful allock only */ 

     if  ( (rc == OK)|| ( *phP4 == RCro80_Accept  )) {
         if (  pContext->bToken  == TRUE )
                 pContext->sStateTrPm   = RC073b_STA20;
         else 
                 pContext->sStateTrPm   = RC073d_STA22;
         
     }else 
         pContext->sStateTrPm   = RC0739_STAnoTr2;

     if ((ro0032_localunlock(*phP3) ) == OK) {
         if(ro0034_localfree (*phP3 ) != OK ) rc = -R_LFREEE;
     }else rc = -R_LULOKE;
     if(rc==OK) {
         /* ------ Assign the OutEvent --------------------------- */
         *plOutEvent            = RC1003_RT_OPENrsp;
         *phP3                  = hHdSer;
         pContext->sSubAutoNum  = RC073j_End;
         rc                     = RC073k_PushOutEvent;
     }
 }/*end of the successful lock of hP3 */ 

RM0003_RETURN (rs0742, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0743_STA10xAA_EndREL                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0743_STA10xAA_EndREL (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        
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

RM0002_ENTRY (rs0743)
 if (pContext->sReleaseFg){
    pContext->sStateTrPm = RC0739_STAnoTr2;
    pContext->sReleaseFg = FALSE;
    *plOutEvent          = RC1007_RT_CLOSErsp;
    pContext->sSubAutoNum= RC073j_End ;
    rc                   =RC073k_PushOutEvent;
 } 
 else rc = -R_WREVNT;

RM0003_RETURN (rs0743, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0744_STA20-22xAA_ABreq                            */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0744_STA20_22xAA_ABreq (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        
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

RM0002_ENTRY (rs0744)
    pContext->sStateTrPm = RC0739_STAnoTr2;
    pContext->sReleaseFg = FALSE;
    *plOutEvent          = RC100h_RT_U_ABORTreq;
    pContext->sSubAutoNum= RC073j_End ;
    rc                   =RC073k_PushOutEvent;
RM0003_RETURN (rs0744, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0745_STA10xRT_OPENcnf                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0745_STA10xRT_OPENcnf (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        
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

rT0704_stBindCnfRopm  *  pBindCnf  = LNULL;
unsigned long  int       hHdSer    = LNULL;
rT100d_stOpenCnfRtpm  *  pOpenCnf  = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0745)
 /* ------ Lock the Service parameters ------------------------------- */
 pOpenCnf = (rT100d_stOpenCnfRtpm *) ro0031_locallock ( *phP3);
 if (!pOpenCnf) rc = -R_LLOCKE;
 if (rc == OK ) {
          hHdSer = ro0030_localalloc  (sizeof (rT0704_stBindCnfRopm));
          if      (hHdSer == LNULL)   rc = -R_LALLCE;
          else {  /* Successful Allocation */
               pBindCnf = (rT0704_stBindCnfRopm *) 
                                    ro0031_locallock (hHdSer);
               if (pBindCnf  == LNULL) {
                     rc = -R_LLOCKE;     
               }
               else { /* successful Allocl & lock */
                     /* ------ Copy the structures for All,Ppm &Acpm --- */
                     pBindCnf->stAll = pOpenCnf->stAll;
                     pBindCnf->stPpm = pOpenCnf->stPpm;
                     pBindCnf->stAcpm = pOpenCnf->stAcpm;
                     /* --------- Copy Over ---------------------------- */
                    /* --------- unlock the Acpm Service parameter ------ */
                     if ((ro0032_localunlock(hHdSer) ) != OK) {
                         rc = -R_LULOKE;
                     }/* end of unlock error if */

               } /*end of succussful allok &lock else */
           } /*end of successful allock only */ 
            
     if  (rc == OK) {
         
          pContext->sStateTrPm   = RC073b_STA20;
     }
     if ((ro0032_localunlock(*phP3) ) == OK) {
         if(ro0034_localfree (*phP3 ) != OK ) rc = -R_LFREEE;
     }else rc = -R_LULOKE;
     if(rc==OK) {
         /* ------ Assign the OutEvent --------------------------- */
         *plOutEvent  =RC073L_AA_ESTAB;
         *phP3         =hHdSer;
         pContext->sSubAutoNum  = RC073g_RopmPm;
     }
 }/*end of the successful lock of hP3 */ 
RM0003_RETURN (rs0745, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0746_STA10xRT_CLOSEcnf                            */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0746_STA10xRT_CLOSEcnf (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs0746)
 if (pContext->sReleaseFg){
     pContext->sStateTrPm = RC0739_STAnoTr2;
     *plOutEvent          = RC073N_AA_EndREL;
     pContext->sSubAutoNum= RC073g_RopmPm ;
 }
 else rc = -R_WREVNT;
RM0003_RETURN (rs0746, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0747_STA20-22xABORTind                            */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0747_STA20_22xABORTind (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs0747)
     pContext->sStateTrPm = RC0739_STAnoTr2;
     pContext->sSubAutoNum=  RC073g_RopmPm ;
/*
     if ( (*plInEvent == RC100i_RT_U_ABORTind ) )
        *plOutEvent          = RC070a_RO_UABORTind;
     else 
        *plOutEvent          = RC070b_RO_PABORTind; commented by kc on 5/5 */

        *plOutEvent          = RC073O_AA_ABind;
RM0003_RETURN (rs0747, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0748_STA20xAA_REL                    	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0748_STA20xAA_REL (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs0748)
 pContext->sReleaseFg = TRUE;
 pContext->sStateTrPm = RC073a_STA10;
 *plOutEvent          = RC1005_RT_CLOSEreq;
 pContext->sSubAutoNum= RC073j_End;
RM0003_RETURN (rs0748, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0749_STA20xTRANSreq                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0749_STA20xTRANSreq (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs0749)
   *plOutEvent          = RC1009_RT_TRANSFERreq;
   pContext->sSubAutoNum= RC073j_End;
   rc                   = RC073k_PushOutEvent;
   if (*phP1)  *phP3    = RC1022_UdataIsPdv;
   else        *phP3    = RC1021_UdataAbsent;

   *phP4                = RC1025_MaxTransferTime;  /*--- Transfer Time ---*/
/*   *phP3                = 0; */
RM0003_RETURN (rs0749, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074a_STA20xRT_CLOSEind                            */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074a_STA20xRT_CLOSEind (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs074a)
 *plOutEvent          = RC073M_AA_REL;
 pContext->sSubAutoNum= RC073g_RopmPm;
 pContext->sStateTrPm = RC073a_STA10;
 pContext->sReleaseFg = TRUE;
RM0003_RETURN (rs074a, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074bSTA20xRT_TURN_PLEASEind                       */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074b_STA20xRT_TURN_PLEASEind (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs074b)
 /*--- Issue RT_TURN_GIVE ---*/
 *phP2  = FALSE;         /*--- sign is (+)ve ---*/
 *phP4  = NULL;    
 pContext->sStateTrPm = RC073d_STA22;
 *plOutEvent          = RC100e_RT_TURN_GIVEreq;
 pContext->sSubAutoNum= RC073j_End;
 rc                   = RC073k_PushOutEvent ;
RM0003_RETURN (rs074b, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074c_STA21xAA_REL                    	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074c_STA21xAA_REL (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int         rc     = OK;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs074c)
 if(pContext->sReleaseFg){
    pContext->sStateTrPm = RC073a_STA10;
    /*--- Push the Close Req First then push PRjind ---*/
    rc = ro0211_pushOutEvent ( RC0218_Rtse         ,
                               RC1005_RT_CLOSEreq  , 
                               *phP1               ,
                               0                   ,
                               0                   ,
                               *phP2                );
    /*--- Issue the Provider Reject ------------------- */
    rc = ro0707_GiveProviderRejInd ( pContext, 
                                 plOutEvent,
                                 0,
                                 phP1,phP2,phP3,phP4 );


 }
 else                                                      rc = -R_WREVNT; 
RM0003_RETURN (rs074c, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074d_STA21-23xAA_ABreq                            */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074d_STA21_23xAA_ABreq (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs074d)

    pContext->sStateTrPm = RC0739_STAnoTr2;
    /*--- Push the Abort Req First then push PRjind ---*/
    rc = ro0211_pushOutEvent ( RC0218_Rtse         ,
                               RC100h_RT_U_ABORTreq  , 
                               *phP1               ,
                               0                   ,
                               0                   ,
                               *phP4                ); 
      /*!!! no reason in RTSE  */

    /*--- Issue the Provider Reject ------------------- */
    rc = ro0707_GiveProviderRejInd ( pContext, 
                                 plOutEvent,
                                 0,
                                 phP1,phP2,phP3,phP4 );


RM0003_RETURN (rs074d, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074e_STA21xRT_TRcnfPos                            */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074e_STA21xRT_TRcnfPos (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs074e)
 pContext->bSyncFlag = FALSE;
 pContext->sStateTrPm = RC073b_STA20;
 rc = ro070v_GetFromQueue ( pContext ,
                            plOutEvent,
                            phP1,phP2,phP3,phP4 );
 if (rc > OK ) {
    pContext->sSubAutoNum = RC073i_RopmRtTr; 
    rc                    = OK;
 }
RM0003_RETURN (rs074e, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074f_STA21xRT_TRcnfNeg                            */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074f_STA21xRT_TRcnfNeg (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs074f)
 pContext->bSyncFlag = FALSE;
 pContext->sStateTrPm = RC073b_STA20;

 /* ----- Issue Provider Reject ind ----*/
 rc = ro0707_GiveProviderRejInd ( pContext    ,
                              plOutEvent  ,
                              0           ,
                              phP1,phP2,phP3,phP4 );
 
RM0003_RETURN (rs074f, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074g_STA21xRT_TPind                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074g_STA21xRT_TPind (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs074g)
 pContext->sStateTrPm = RC073c_STA21;
RM0003_RETURN (rs074g, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074h_STA22xTRANSreq                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074h_STA22xTRANSreq (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs074h)
 /* ---!!! we poped in the TRANSreq so we have to        ---*/
 /* ---1. Push it out                                    ---*/
 /* ---2. Put this in the queue                          ---*/
  rc = ro070u_PutInQueue ( pContext,
                         *plInEvent,
                         *phP1,*phP2,*phP3,*phP4 );
  if (rc == OK) {
 /*--- Issue the RT-TPreq ---*/
    *phP1 = *phP3 = LNULL;
    *phP2 = LNULL;   /*--- (+)ve sign ---*/
    *phP4 = 0;       /*--- Value of the priority ---*/
    *plOutEvent = RC100c_RT_TURN_PLEASEreq;
    pContext->sSubAutoNum = RC073j_End;
    rc                    = RC073k_PushOutEvent;
    pContext->sStateTrPm  = RC073e_STA23;
 }
RM0003_RETURN (rs074h, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074i_STA22xRT_TRind                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074i_STA22xRT_TRind (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs074i)
 if (*phP2 == RC1022_UdataIsPdv ){
    *plOutEvent = RC073P_TRANSind;
    pContext->sSubAutoNum = RC073g_RopmPm;
 }
 else {      /*--- give Abort ----*/
   rc = RC073m_ProviderAbort ;
   pContext->sSubAutoNum = RC073j_End ;
 }

RM0003_RETURN (rs074i, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074j_STA22xRT_TGind                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074j_STA22xRT_TGind (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs074j)
 pContext->sStateTrPm = RC073b_STA20;
 pContext->sSubAutoNum = RC073j_End;
 /* --- No pushout ---*/
 *plOutEvent = NULL;
RM0003_RETURN (rs074j, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074k_STA23xRT_TRind             		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074k_STA23xRT_TRind (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs074k)
 if (*phP2 == RC1022_UdataIsPdv ){
    *plOutEvent = RC073P_TRANSind;
    pContext->sSubAutoNum = RC073g_RopmPm;
 }
 else {      /*--- give Abort ----*/
   rc = RC073m_ProviderAbort ;
   pContext->sSubAutoNum = RC073j_End ;
 }
RM0003_RETURN (rs074k, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074l_STA23xRT_TGind                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074l_STA23xRT_TGind (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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

RM0002_ENTRY (rs074l)
 /* !!! if any TRANSreq is waiting issue it now */
 rc = ro070v_GetFromQueue ( pContext,
                            plOutEvent,
                            phP1, phP2, phP3, phP4 );
 if (rc > OK ) {
        pContext->sStateTrPm = RC073b_STA20;
        pContext->sSubAutoNum= RC073i_RopmRtTr;
        rc                   = OK;
 }
RM0003_RETURN (rs074l, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro074m_STA21xTRANSreq                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro074m_STA21_23xTRANSreq (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

/*                                                                    */
/* DESCRIPTION  : This automata is not present in the standards.      */
/*                Included since to provide ASYN Operation            */
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

RM0002_ENTRY (rs074m)
 rc = ro070u_PutInQueue ( pContext,
                        *plInEvent,
                        *phP1,*phP2,*phP3,*phP4 );
 
RM0003_RETURN (rs074m, signed short int, rc)                                    
}/*end*/                                                                        

/* EOF */
