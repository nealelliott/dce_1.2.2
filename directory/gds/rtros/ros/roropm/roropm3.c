/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropm3.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:00  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:40  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:13  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:34  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:04  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:18  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:06:17  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:52  keutel
 * 	creation
 * 	[1994/03/21  13:35:08  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roropm3.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:00 $" } ;
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
/* NAME         : roropm3.c                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-02                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Automata Functions for RO-Tr Presentation           */
/*                Table A.8.                                          */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropm3.c	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-04-02|  original                      |kc  |         */
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

#include  <rortpm0.h>
#include <roacpm0.h>

#include <roppm00.h>


#include <roropm0.h>      /* Interface definitions                    */
#include <roropm1.h>      /* PDU data structures                      */
#include <roropm3.h>      /* Local Events & Data structures           */
#include <roropm4.h>      /* Automata Function proto TYpe             */
/** Not needed Baiju , GNU GCC 
#include <roropm5.h>      =* Static Automata Table                    *=
**/


/* ****************** Logging function name strings ***************** */
/* ******************                               ***************** */
RM0001_STRG (rs0730, "ro0730_STA000xstart")
RM0001_STRG (rs0731, "ro0731_STA000xASSOC")
RM0001_STRG (rs0732, "ro0732_STA100xAA_ES")
RM0001_STRG (rs0733, "ro0733_STA000xAA_En")
RM0001_STRG (rs0734, "ro0734_STAXXXxAA_AB")
RM0001_STRG (rs0735, "ro0735_STA100xAA_AS")
RM0001_STRG (rs0736, "ro0736_STA100xAA_RE")
RM0001_STRG (rs0737, "ro0737_STAXXXxABORT")
RM0001_STRG (rs0738, "ro0738_STA200xAA_RE")
RM0001_STRG (rs0739, "ro0739_STA200xTRANS")
RM0001_STRG (rs073a, "ro073a_STA200xP_DAT")
RM0001_STRG (rs073b, "ro073b_STA200xAA_RE")
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0730_STA000xstartESTAB                            */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0730_STA000xstartESTAB (
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
rT0701_stBindReqRopm   *pBindRq;
unsigned long  int      hHdSer;
rTro7e_stAssocReqAcpm  *pAssocReq;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0730)
 /* ------ Lock the Service parameters ------------------------------- */
 pBindRq = (rT0701_stBindReqRopm *)ro0031_locallock ( *phP3);
 if (!pBindRq) rc = -R_LLOCKE;
 if (rc == OK ) {
          hHdSer = ro0030_localalloc  (sizeof (rTro7e_stAssocReqAcpm));
          if      (hHdSer == LNULL)   rc = -R_LALLCE;
          else {  /* Successful Allocation */
               pAssocReq = (rTro7e_stAssocReqAcpm *) 
                                    ro0031_locallock (hHdSer);
               if (pAssocReq  == LNULL) {
                     rc = -R_LLOCKE;     
               }
               else { /* successful Allocl & lock */
                     /* ------ Copy the structures for All,Ppm &Acpm --- */
                     pAssocReq->stAll = pBindRq->stAll;
                     pAssocReq->stPpm = pBindRq->stPpm;
                     pAssocReq->stAcpm = pBindRq->stAcpm;
                   /* ------- For Session in the case of NO RTSE Fill -- */
                     pAssocReq->stSpm.ulRequirements =
                                        RCro53_FullDuplex;
                     pAssocReq->stSpm.lSyncPoint     =
                                        RCro5h_NoInitialSpsn;
                     pAssocReq->stSpm.ulToken        = 
                                        ( RCro57_DataToken   |
                                          RCro5a_ReleaseToken );
                     pAssocReq->stSpm.ulQuality            = LNULL;
                      
              /* !!! No ConnId OPtional by Shiv on 1/4/93 ------ */
                    /* ------ Session is Over --------------------------- */
                     
                    /* --------- unlock the Acpm Service parameter ------ */
                     if ((ro0032_localunlock(hHdSer) ) != OK) {
                         rc = -R_LULOKE;
                     }/* end of unlock error if */

               } /*end of succussful allok &lock else */
           } /*end of successful allock only */ 

     if  (rc == OK) {
         pContext->sStateTrPm   = RC0736_STA100;
         pContext->ulAssocClas  =  pBindRq->stRopm.ulAssocClass;
     }
     if((ro0032_localunlock(*phP3)) == OK ){ 
         if((ro0034_localfree (*phP3 )) != OK ) rc = -R_LFREEE;
     }
     else  rc = -R_LLOCKE;
     if (rc == OK){
         /* ------ Assign the OutEvent --------------------------- */
         *plOutEvent  =RC0601_AA_ASSOCIATEreq;
         *phP3         =hHdSer;
         pContext->sSubAutoNum  = RC073j_End;
         rc = RC073k_PushOutEvent;
     }
 }/*end of the successful lock of hP3 */ 
RM0003_RETURN (rs0730, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0731_STA000xASSOCind                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0731_STA000xASSOCind (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        
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
signed   short int         rc        = OK;

rTro7f_stAssocIndAcpm    * pAssocInd = LNULL;
unsigned long  int         hHdSer    = LNULL;
rT0702_stBindIndRopm     * pBindInd  = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0731)
 /* ------ Lock the Service parameters ------------------------------- */
 pAssocInd = (rTro7f_stAssocIndAcpm *) ro0031_locallock ( *phP3);
 if (!pAssocInd) rc = -R_LLOCKE;
 if (rc == OK ) {
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
                     pBindInd->stAll = pAssocInd->stAll;
                     pBindInd->stPpm = pAssocInd->stPpm;
                     pBindInd->stAcpm = pAssocInd->stAcpm;
                     /* --------- Copy Over ---------------------------- */
                    /* --------- unlock the Acpm Service parameter ------ */
                     if ((ro0032_localunlock(hHdSer) ) != OK) {
                         rc = -R_LULOKE;
                     }/* end of unlock error if */

               } /*end of succussful allok &lock else */
           } /*end of successful allock only */ 
            
     if  (rc == OK) {
         pContext->sStateTrPm   = RC0736_STA100;
         pContext->ulAssocClas  = RC070n_AssocClass3; /* Default */ 
         pContext->bmappingInfo = TRUE;              /*  No RTSE */
         pContext->bAssocInit   = FALSE;              /* RESPONDER */
     }
     if (ro0032_localunlock(*phP3)==OK ){
         if(ro0034_localfree (*phP3 ) != OK ) rc = -R_LFREEE;
     }
     else rc = -R_LULOKE;
     if(rc == OK){
         /* ------ Assign the OutEvent --------------------------- */
         *plOutEvent  =RC073K_AA_StartESTAB;
         *phP3         =hHdSer;
         pContext->sSubAutoNum  = RC073g_RopmPm;
     }
 }/*end of the successful lock of hP3 */ 
RM0003_RETURN (rs0731, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0732_STA100xAA_ESTAB                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0732_STA100xAA_ESTAB (
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
signed   short int      rc         = OK;

rT0703_stBindRspRopm  *  pBindRsp  = LNULL;
unsigned long  int       hHdSer    = LNULL;
rTro7g_stAssocRspAcpm  * pAssocRsp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0732)
 /* *** check for the Redirection  ***** */
 if ( ( *phP4 & RCro85_Redirection) == RCro85_Redirection ) {
         *plOutEvent  =RC0603_AA_ASSOCIATErsp;
         pContext->sSubAutoNum  = RC073j_End;
         rc = RC073k_PushOutEvent;
         pContext->sStateTrPm   = RC0735_STAnoTr1;
         RM0003_RETURN (rs0732, signed short int, rc)
 }                                    
 /* ------ Lock the Service parameters ------------------------------- */
 pBindRsp = (rT0703_stBindRspRopm *)ro0031_locallock ( *phP3);
 if (!pBindRsp) rc = -R_LLOCKE;
 if (rc == OK ) {
          hHdSer = ro0030_localalloc  (sizeof (rTro7g_stAssocRspAcpm));
          if      (hHdSer == LNULL)   rc = -R_LALLCE;
          else {  /* Successful Allocation */
               pAssocRsp = (rTro7g_stAssocRspAcpm *) 
                                    ro0031_locallock (hHdSer);
               if (pAssocRsp  == LNULL) {
                     rc = -R_LLOCKE;     
               }
               else { /* successful Allocl & lock */
                     /* ------ Copy the structures for All,Ppm &Acpm --- */
                     pAssocRsp->stAll = pBindRsp->stAll;
                     pAssocRsp->stPpm = pBindRsp->stPpm;
                     pAssocRsp->stAcpm = pBindRsp->stAcpm;
                   /* ------- For Session in the case of NO RTSE Fill -- */
                     pAssocRsp->stSpm.ulRequirements =
                                        RCro53_FullDuplex;
                     pAssocRsp->stSpm.lSyncPoint     =
                                        RCro5h_NoInitialSpsn;
                     pAssocRsp->stSpm.ulToken        =       LNULL;
                     pAssocRsp->stSpm.ulQuality            = LNULL;
                      
              /* !!! No ConnId OPtional by Shiv on 1/4/93 ------ */
                    /* ------ Session is Over --------------------------- */
                     
                    /* --------- unlock the Acpm Service parameter ------ */
                     if ((ro0032_localunlock(hHdSer) ) != OK) {
                         rc = -R_LULOKE;
                     }/* end of unlock error if */

               } /*end of succussful allok &lock else */
           } /*end of successful allock only */ 

     if  ( (rc == OK ) || ( *phP4  ==  RCro80_Accept) )
         pContext->sStateTrPm   = RC0737_STA200;
     else 
         pContext->sStateTrPm   = RC0735_STAnoTr1;
 
     if (ro0032_localunlock(*phP3)==OK ){
         if(ro0034_localfree (*phP3 ) != OK ) rc = -R_LFREEE;
     }
     else rc = -R_LULOKE;
     if(rc == OK){
         /* ------ Assign the OutEvent --------------------------- */
         *plOutEvent  =RC0603_AA_ASSOCIATErsp;
         *phP3         =hHdSer;
         pContext->sSubAutoNum  = RC073j_End;
         rc = RC073k_PushOutEvent;
     }
 }/*end of the successful lock of hP3 */ 
RM0003_RETURN (rs0732, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0733_STA100xAA_EndREL                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0733_STA000xAA_EndREL (
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

RM0002_ENTRY (rs0733)
 if (pContext->sReleaseFg){
    pContext->sStateTrPm = RC0735_STAnoTr1;
    pContext->sReleaseFg = FALSE;
    *plOutEvent          = RC0607_AA_RELEASErsp;
    pContext->sSubAutoNum= RC073j_End ;
    rc                   =RC073k_PushOutEvent;
 } 
 else rc = -R_WREVNT;

RM0003_RETURN (rs0733, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0734_STAXXXxAA_ABreq                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0734_STAXXXxAA_ABreq (
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

RM0002_ENTRY (rs0734)

    pContext->sStateTrPm = RC0735_STAnoTr1;
    pContext->sReleaseFg = FALSE;
    *plOutEvent          = RC0609_AA_ABORTreq;
    pContext->sSubAutoNum= RC073j_End ;
    rc                   =RC073k_PushOutEvent;

RM0003_RETURN (rs0734, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0735_STA100xAA_ASSOCcnf                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0735_STA100xAA_ASSOCcnf (
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
rTro7h_stAssocCnfAcpm  * pAssocCnf = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0735)
 /* ------ Lock the Service parameters ------------------------------- */
 pAssocCnf = (rTro7h_stAssocCnfAcpm *) ro0031_locallock ( *phP3);
 if (!pAssocCnf) rc = -R_LLOCKE;
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
                     pBindCnf->stAll = pAssocCnf->stAll;
                     pBindCnf->stPpm = pAssocCnf->stPpm;
                     pBindCnf->stAcpm = pAssocCnf->stAcpm;
                     /* --------- Copy Over ---------------------------- */
                    /* --------- unlock the Acpm Service parameter ------ */
                     if ((ro0032_localunlock(hHdSer) ) != OK) {
                         rc = -R_LULOKE;
                     }/* end of unlock error if */

               } /*end of succussful allok &lock else */
           } /*end of successful allock only */ 
            
     if  ( (rc == OK)&& ( *phP4 == RCro80_Accept  ))  {
         pContext->sStateTrPm   = RC0737_STA200;
     }else 
         pContext->sStateTrPm   = RC0735_STAnoTr1;
     if (ro0032_localunlock(*phP3)==OK ){
         if(ro0034_localfree (*phP3 ) != OK ) rc = -R_LFREEE;
     }
     else rc = -R_LULOKE;
     if(rc == OK){
         /* ------ Assign the OutEvent --------------------------- */
         *plOutEvent  =RC073L_AA_ESTAB;
         *phP3         =hHdSer;
         pContext->sSubAutoNum  = RC073g_RopmPm;
     }
 }/*end of the successful lock of hP3 */ 
RM0003_RETURN (rs0735, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0736_STA100xAA_RELEASEcnf                         */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0736_STA100xAA_RELEASEcnf (
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

RM0002_ENTRY (rs0736)
 if (pContext->sReleaseFg){
     pContext->sStateTrPm = RC0735_STAnoTr1;
     *plOutEvent          = RC073N_AA_EndREL;
     pContext->sSubAutoNum=  RC073g_RopmPm ;
 }
 else rc = -R_WREVNT;
     
RM0003_RETURN (rs0736, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0737_STAXXXxABORTind                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0737_STAXXXxABORTind (
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

RM0002_ENTRY (rs0737)
     pContext->sStateTrPm = RC0735_STAnoTr1;
     pContext->sSubAutoNum=  RC073g_RopmPm ;
   
  /*   if ( (*plInEvent == RC060a_AA_ABORTind ) ||
          (*phP4     == RC0610_AA_UserABORT) )
        *plOutEvent          = RC070a_RO_UABORTind;
     else 
        *plOutEvent          = RC070b_RO_PABORTind; comented by kc on 5/5 */
     *plOutEvent    = RC073O_AA_ABind;
     
RM0003_RETURN (rs0737, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0738_STA200xAA_REL                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0738_STA200xAA_REL (
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

RM0002_ENTRY (rs0738)
 pContext->sReleaseFg = TRUE;
 pContext->sStateTrPm = RC0736_STA100;
 *plOutEvent          = RC0605_AA_RELEASEreq;
 pContext->sSubAutoNum= RC073j_End;
 rc                   = RC073k_PushOutEvent;
RM0003_RETURN (rs0738, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0739_STA200xTRANSreq                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0739_STA200xTRANSreq (
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

RM0002_ENTRY (rs0739)
 *plOutEvent          = RC040c_PDATAreq;
 pContext->sSubAutoNum= RC073j_End;
 rc                   = RC073k_PushOutEvent;
 *phP2 = *phP3 = *phP4= LNULL;
RM0003_RETURN (rs0739, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro073a_STA200xP_DATAind                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro073a_STA200xP_DATAind (
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
signed   short int        rc     = OK;

rT0715_unRoseApdus  FAR * pApdus;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs073a)
   pApdus = (rT0715_unRoseApdus FAR *) ro0021_globallock (*phP1) ;
   if (pApdus == NULL) 
       rc = -R_LLOCKE;
   else {
      switch(pApdus->stPdvHeader.lType){
          case RC071P_ROIVapdu:
               *phP4 = RC073Q_ROINV;
               break; 
          case RC071Q_RORSapdu:
               *phP4 = RC073R_RORES;
               break; 

          case  RC071R_ROERapdu:
               *phP4 = RC073S_ROERR;
               break; 

          case RC071S_RORJapdu:
               switch(pApdus->stRORJapdu.lProblemChoiceNum){
                   case RC0712_GeneralProblem:
                           *phP4 = ( (pApdus->stRORJapdu.lProblem  >= 0)&&
                                          (pApdus->stRORJapdu.lProblem  <= 2) )
                                        ? RC073U_RORJP
                                        : RC073V_APDUua;
                           break;
                   case RC0713_InvokeProblem:
                           *phP4 = ( (pApdus->stRORJapdu.lProblem  >= 0)&&
                                          (pApdus->stRORJapdu.lProblem  <= 7)&&
                                          (pApdus->stRORJapdu.lInvokeIDChoiceNum
                                           == RC071P_InvIdPrest)            )
                                        ? RC073T_RORJU
                                        : RC073V_APDUua;
                           break;
                   case RC0714_ReturnResultProblem:
                           *phP4 = ( (pApdus->stRORJapdu.lProblem  >= 0)&&
                                          (pApdus->stRORJapdu.lProblem  <= 2)&&
                                          (pApdus->stRORJapdu.lInvokeIDChoiceNum
                                           == RC071P_InvIdPrest)            )
                                        ? RC073T_RORJU
                                        : RC073V_APDUua;
                           break;
                   case  RC0715_ReturnErrorProblem :
                           *phP4 = ( (pApdus->stRORJapdu.lProblem  >= 0)&&
                                          (pApdus->stRORJapdu.lProblem  <= 4)&&
                                          (pApdus->stRORJapdu.lInvokeIDChoiceNum
                                           == RC071P_InvIdPrest)            )
                                        ? RC073T_RORJU
                                        : RC073V_APDUua;
                           break;
                   default:
                           *phP4 = RC073V_APDUua ;
                           break;
               }/* end of ROJU switch */                                            
               break; 
          default:
               *phP4 = RC073V_APDUua;
               break; 
    }/*end of the switch */
    if (ro0022_globalunlock (*phP1) ) rc = -R_GULOKE;
  }/*end of else */
  *plOutEvent          = RC073P_TRANSind;
  pContext->sSubAutoNum= RC073g_RopmPm;
RM0003_RETURN (rs073a, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro073b_STA200xAA_RELEASEind                         */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro073b_STA200xAA_RELEASEind (
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

RM0002_ENTRY (rs073b)
 *plOutEvent          = RC073M_AA_REL;
 pContext->sSubAutoNum= RC073g_RopmPm;
 pContext->sStateTrPm = RC0736_STA100;
 pContext->sReleaseFg = TRUE;
RM0003_RETURN (rs073b, signed short int, rc)                                    
}/*end*/                                                                        


