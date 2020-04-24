/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm04.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:22  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:48  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:18  keutel
 * 	OT11620
 * 	[1994/08/19  13:57:30  keutel]
 * 
 * 	OT 11620
 * 	[1994/08/19  13:51:45  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:06  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:26  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:34  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:08:19  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:03  keutel
 * 	creation
 * 	[1994/03/21  13:37:03  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rospm04.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:22 $" } ;
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
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : rospm04.c                                           */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-22                                            */
/*                                                                    */
/* COMPONENT    : RTROS Convergence protocol machine                  */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : Contains the functions related to the phase :       */
/*                "Connection Establishment".                         */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-03-22|  original                      |ma  |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "@(#) rospm04.c 1.2 93/12/20 RTROS-V3.00";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>    /* Include of ROS global defines              */
#include <roerval.h>    /* Includes of error values                   */
#include <routil0.h>    /* Include of ROS common utilites             */
#include <rouply0.h>    /* Common generation constants,types & values */
#include <rouply1.h>    /* PM service interface structures            */ 
#include <rodpat0.h>    /* Prototyping of Dispatcher functions        */
#include <rostor0.h>    /* Prototyping of Store functions             */

#include <rotpm00.h>    /* Include of TPM events                      */
#include <rospm00.h>    /* Include of SPM events                      */

#include <rospm01.h>    /* Defines for coding functions               */
#include <rospm03.h>    /* Defines for convergence protocol m/c       */  
#include <rospm05.h>    /* Prototyping of coding of functions         */ 
#include <rospm07.h>    /* Includes local to protocol machine         */ 
#include <rospm06.h>    /* Prototyping of convergence protocol m/c    */    

/*********************************************************************/
/*                                                                   */
/*              G L O B A L   V A R I A B L E S                      */
/*                                                                   */
/*********************************************************************/

/*********************************************************************/
/*                                                                   */
/*              G L O B A L   F U N C T I O N S                      */
/*                                                                   */
/*********************************************************************/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0840_TCONTINUE                                    */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-22                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0840_TCONTINUE (
rT0870_stSpmContext    *pContext,
unsigned long int ulP1,   
unsigned long int ulP2,
unsigned long int ulP3, 
unsigned long int ulP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   -------------                    */
/*                     actions   :   -------------                    */
/*                     next event:   T_DATA.request                   */
/*                     next state:   -------------                    */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*     ulP1      = First parameter of transaction function.           */
/*     ulP2      = Second parameter of transaction function.          */
/*     ulP3      = Third parameter of transaction function.           */
/*     ulP4      = Fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr-ro0840_TCONTINUE*/

signed short int sRetCode = OK;

  RM0001_STRG(functName,"ro0840_TCONTINUE")
  RM0002_ENTRY(functName)

/* ************************ S T A R T ******************************* */
/* ************************           ******************************* */

/*--------------------------------------------------------------------*/
/*     S_CONTINUE allows the presentation to send new SPDU.           */
/*--------------------------------------------------------------------*/
/* NOTE ::
           SCONTINUE will be send for every TCONTINUE if user connection exits..
           Hense pContext->usUserConnExists is made TRUE if the
           events are : SCONNECTreq & SCONNECTind
           pContext->usUserConnExists  is made FALSE if events are :
           SRELESErsp ,SRELEASEcnf , SUABORTind , SUABORTind , SPABORTind .. */
 
    if(pContext->usUserConnExists == TRUE)
    { 
        sRetCode = ro0211_pushOutEvent(RC0215_Presentation,
                            RC080F_SCONTINUE,
                            NULL,
                            NULL,
                            NULL, 
                            NULL);
    }

    RM0003_RETURN(functName, signed short int, sRetCode)
}/*end-ro0840_TCONTINUE*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0841_STA01xSCONreq                                */
/*                                                                    */
/* AUTHOR       : Milind Madhav Aphale                                */
/* DATE         : 93-03-22                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0841_STA01xSCONreq (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hConnReqAcpm,
unsigned long int ulP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:  -------                           */
/*                     actions   :   [2]                              */
/*                     next event:  T_CONNECTrequest                  */
/*                     next state:  RC08d2_STA01B                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */ 
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*     hUserData = Handle to store.                                   */ 
/*     ulOffset  = Offset in store from where encoding/decoding       */
/*                 should start.                                      */  
/*     hConnReqAcpm = Memory handle to hConnReqAcpm.                  */
/*     ulP4      = Fourth parameter of transaction function.          */ 
/*                                                                    */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr-ro0841_STA01xSCONreq*/

rTro7e_stAssocReqAcpm FAR *pConnReqAcpm;
signed short int           sRetCode = OK;

  RM0001_STRG(functName,"ro0841_STA01xSCONreq")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

  if (( pConnReqAcpm = 
         (rTro7e_stAssocReqAcpm FAR *)ro0031_locallock(hConnReqAcpm)) == NULL)
    RM0003_RETURN(functName, signed short int, -R_LLOCKE)
  
  if(( pConnReqAcpm->stSpm.ulRequirements & RCro53_FullDuplex ) ==
          RCro53_FullDuplex )
    pContext->usFunctionalUnits  = 
            (unsigned short int) pConnReqAcpm->stSpm.ulRequirements;
  else
    RM0003_RETURN(functName, signed short int, -R_SINTER)

  if (((pConnReqAcpm->stAll.sSessProtVers & RCro50_ProtVers1) 
         == RCro50_ProtVers1) ||
      ((pConnReqAcpm->stAll.sSessProtVers & RCro51_ProtVers2)
         == RCro51_ProtVers2) )
    pContext->usProtvers     =   pConnReqAcpm->stAll.sSessProtVers;
  else
    RM0003_RETURN(functName, signed short int, -R_SINTER)
 
  /* get the address to which the connection will be established         */
  pContext->sCalledAddrsLen =  
          ro0217_getAddress(pConnReqAcpm->stAll.hCalledAddress,
                            RCro13_Sselector,                  
                            RC0830_AddressLen,                      
                            (char FAR *)pContext->vCalledAddrs);
  if (pContext->sCalledAddrsLen < OK)
      RM0003_RETURN(functName, signed short int, pContext->sCalledAddrsLen)

  /* store the user data handle and store offset in pContext block.   */
 
  pContext->hUserData  = hUserData;
  pContext->ulOffset   = ulOffset;
  
  sRetCode = ro0211_pushOutEvent(RC0213_Transport,
                      RC0901_TCONNECTreq,
                      NULL,
                      NULL,
                      pConnReqAcpm->stAll.hCalledAddress,
                      NULL );
  if (sRetCode != OK)
      RM0003_RETURN(functName, signed short int, sRetCode)

  pContext->usState = RC08d2_STA01B;    /* Wait for TCONNECTcnf */
  pContext->usVtca  = FALSE;            /*  SPM initiated       */

  if (ro0032_localunlock(hConnReqAcpm) != NULL)
      RM0003_RETURN(functName, signed short int, -R_LLOCKE)

  if (ro0034_localfree (hConnReqAcpm)  != NULL)           /* skh: 5/7/93 */
      RM0003_RETURN(functName, signed short int, -R_LFREEE)

RM0003_RETURN(functName, signed short int, OK)
}/*end-ro0841_STA01xSCONreq*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0842_STA01xTCONind                                */
/*                                                                    */
/* AUTHOR       : Milind M.Aphale                                     */
/* DATE         : 93-03-22                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0842_STA01xTCONind (
rT0870_stSpmContext *pContext,
unsigned long int hP1,
unsigned long int hP2,
unsigned long int hCallingAddr,
unsigned long int hCalledAddr)

/*                                                                    */
/* DESCRIPTION  :      predicates:   -------                          */
/*                     actions   :   [1]                              */
/*                     next event:   T_CONNECTresponse                */
/*                     next state:   RC08d3_STA01C                    */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS       :                                              */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*     hP1          = First parameter of transaction function         */
/*     hP2          = HAndle to called address.                       */
/*     hCallingAddr = Handle to calling address.                      */
/*     hCalledAddr  = Fourth parameter of transaction function.       */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr-ro0842_STA01xTCONind*/

    RM0001_STRG(functName,"ro0842_STA01xTCONind")
    RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

   pContext->ulTidusize      = NULL;     /* TSDU size is not limmited */
/* ***** copy hCallingAddr by kc on 1/7/93 ************************** */
   pContext->hAddrs          = hCallingAddr;
   pContext->hCalledAddress  = hCalledAddr; /* by kc on 08-12-93 */
/*
   if ((pContext->hAddrs = ro0030_localalloc (RCro1e_MaxLenPSAP)) == 0)
      RM0003_RETURN(functName, signed short int, -R_LALLCE)
   {
       char FAR * p1;
       char FAR * p2;
       if ((p1 = (char FAR *)ro0031_locallock (hCallingAddr)) == NULL)
          RM0003_RETURN(functName, signed short int, -R_LLOCKE)
       if ((p2 = (char FAR *)ro0031_locallock (pContext->hAddrs)) == NULL)
          RM0003_RETURN(functName, signed short int, -R_LLOCKE)
       ro0041_memcpy ((char FAR *)p2,
                      (char FAR *)p1,
                      (long)RCro1d_MaxLenTSAP);
       if (ro0032_localunlock (pContext->hAddrs)!= NULL)
          RM0003_RETURN(functName, signed short int, -R_LULOKE)
       if (ro0032_localunlock (hCallingAddr)!= NULL)
          RM0003_RETURN(functName, signed short int, -R_LULOKE)
   }
*/
   pContext->usState = RC08d3_STA01C;   /*  ie. Idle,Transport Connected */ 
   pContext->usVtca  = TRUE;            /*  SPM received TCONind         */

   RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent (
                                                           RC0213_Transport,
                                                           RC0903_TCONNECTrsp,
                                                           NULL,
                                                           NULL,
                                                           NULL,
                                                           NULL))
      
}/*end-ro0842_STA01xTCONind*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0843_STA01AxAA                                    */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-22                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0843_STA01AxAA (
rT0870_stSpmContext *pContext,
unsigned long int hP1,
unsigned long int hP2,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   -----------                      */
/*                     actions   :   [3]                              */
/*                     next event:   -----------                      */
/*                     next state:   RC08d3_STA01C                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*     hP1       = First parameter of transaction function            */
/*     hP2       = Second parameter of transaction function           */
/*     hP3       = Third parameter of transaction function            */
/*     hP4       = Fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS       :                                      */ 
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr-STA01xAA*/
  
   RM0001_STRG(functName,"ro0843_STA01AxAA")
   RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

/*  TIMER not implemented   */

   pContext->usState = RC08d3_STA01C;   /* ie. Idle , transport connected */

RM0003_RETURN(functName, signed short int, OK)
}/*end-STA01xAA*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0844_STA01BxTCONcnf                               */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-22                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0844_STA01BxTCONcnf (
rT0870_stSpmContext *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,  
unsigned long int hRespondAddr,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   p204                             */
/*                     actions   :   -------------                    */
/*                     next event:   CN                               */
/*                     next state:   RC08d7_STA02B                    */
/*                                                                    */
/*                     predicates:   ^p204                            */
/*                     actions   :   -------------                    */
/*                     next event:   CN                               */
/*                     next state:   RC08d6_STA02A                    */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS    :                                                 */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*                 should start.                                      */
/* hRespondAddr = Memory handle to responding address.                */
/*    hP4       = Fourth parameter of transaction function.           */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr-ro0844_STA01BxTCONcnf*/

unsigned long  int         hOutData  = LNULL;  
char                FAR  * pOutData  = NULL;

rT0814_stCN                stCN;
signed   long  int         lStorelen = LNULL;
unsigned long  int         ulParam1  = LNULL;
unsigned long  int         ulParam2  = LNULL;
signed   short int         sRetCode  = OK;

   RM0001_STRG(functName,"ro0844_STA01BxTCONcnf")
   RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

  if (pContext->bAbortPend) {
     /*--------------------------------------------------------------------*/
     /* This is the case of S-U-ABORT-req between T-CONreq and T-CONcnf !  */
     /*--------------------------------------------------------------------*/
      pContext->bAbortPend = FALSE;
      if (hRespondAddr)
          if (ro0034_localfree   (hRespondAddr)   ) sRetCode = -R_LFREEE;
      if (!sRetCode && hUserData)
          if (ro0075_deletestore (hUserData) != OK) sRetCode = -R_SSTOER;
      if (!sRetCode)
          sRetCode = ro0835_Abort1 (pContext, LNULL, LNULL, LNULL, LNULL);

      RM0003_RETURN (functName, signed short int, sRetCode)
  }

  ro082B_InitStruc(&stCN , sizeof(rT0814_stCN));

  pContext->ulTidusize = NULL;       /* TSDU size is not limited */
  pContext->hAddrs     = hRespondAddr; 

/*--------------------------------------------------------------------*/
/* prepare the structure for encoding function                        */
/*--------------------------------------------------------------------*/

  stCN.usPopt              =  pContext->usPopt;
  stCN.ulTSDUmaxto         =  pContext->ulTidusize;
  stCN.ulTSDUmaxfrom       =  pContext->ulTidusize;
  stCN.usVersion           =  pContext->usProtvers;  
  stCN.usFunctionalUnits   =  pContext->usFunctionalUnits;        
  stCN.sCalledLen          =  pContext->sCalledAddrsLen; 
  ro0041_memcpy((char FAR *)stCN.vCalled,
                (char FAR *)pContext->vCalledAddrs,
                (unsigned long int)pContext->sCalledAddrsLen);

   /* fetch the own address */
  stCN.sCallingLen = ro0217_getAddress(NULL,                                   
                                       RCro13_Sselector,  
                                       RC0830_AddressLen,
                                       (char FAR *)stCN.vCalling);
  if (stCN.sCallingLen < OK)
      RM0003_RETURN(functName, signed short int, stCN.sCallingLen)

  /* ------------------------------------------- */
  /* Implementation with Store                   */
  /* ------------------------------------------- */
  
  /* In TCONNECTcnf  hUserData is NULL as lower interface is not 
     sending User Data store handle  
  */
     hOutData = pContext->hUserData;
     ulOffset = pContext->ulOffset;

     pContext->hUserData = NULL; 
     pContext->ulOffset  = NULL;

  if (hOutData == NULL)   /* User Data not available */
  {
      hOutData = ro0060_newstore(ST_USEMAXBUF,ST_CLOSED,
				 (signed short int FAR *)&sRetCode);
      if(sRetCode != OK)
        RM0003_RETURN(functName, signed short int, -R_SSTOER)
      stCN.lDatalen = NULL;

      ulOffset = RC0831_MaxParamLen;
      lStorelen = ro0062_storelength(hOutData); 
      if(ro0063_sterror(hOutData) != OK )
          RM0003_RETURN(functName, signed short int, -R_SSTOER)
  }
  else                    /* User Data Available  */
  {
    if(ro0074_storeactive(hOutData , 0L) != OK ) 
      RM0003_RETURN(functName, signed short int, -R_SSTOER )

    lStorelen = ro0062_storelength(hOutData); 
    if(ro0063_sterror(hOutData) != OK )
      RM0003_RETURN(functName, signed short int, -R_SSTOER)

    stCN.lDatalen = lStorelen - ulOffset;
  }

  if ((pOutData = ro0069_stputs(hOutData,ulOffset) ) == NULL )
        RM0003_RETURN(functName, signed short int, -R_SSTOER)
   stCN.pDataptr = pOutData + ulOffset; 


/* --------------------------------------------------------------- 
 # New Implementation : RTROS V-300 Implementation Restriction 
 # Data usOverflow is baned while sending.
 # Hence,if storelength > 10K give a provider abort.  
 --------------------------------------------------------------- */ 

  stCN.usOverflow    =  RC087d_NoOverflow; 

/*   if(lStorelen > RC0835_MaxConnectLen) by kc 29-07-94 K02 
     for the Conformance test     */
  if (stCN.lDatalen > RC0835_MaxConnectLen)
  {
  /* commented by kc on 6-8-93 for T50.E.19 
    if(ro0073_storeidle(hOutData) != OK)
      RM0003_RETURN(functName, signed short int, -R_SSTOER )
*/
    if(ro0075_deletestore(hOutData) != OK)
      RM0003_RETURN(functName, signed short int, -R_SSTOER )

    pContext->usUserConnExists = FALSE;
    pContext->usState          = RC08d0_STA01;
        
    RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent (
                                                     RC0215_Presentation,
			                             RC080b_SPABORTind,
                                                     NULL,
                                                     NULL,
                                                     NULL,
                                                     RC0313_RestrictionInPICS))


    /* ------------------------------------------------ 
     #  Cleaning up of SPM Context block is carried 
     #  out in generic function as state changing to 
     #  RC08d0_STA01. 
     ------------------------------------------------ */
  } 

/* --------------------------------------------------- */

  ro0814_EncodeCN ((rT0814_stCN FAR *)&stCN);

  ulParam1 = hOutData;
  ulParam2 = (unsigned long int)(stCN.pDataptr - pOutData);
   
  pContext->usState = RC08d6_STA02A;  /* ie. Wait for ACCEPT SPDU */

  if(ro0073_storeidle(hOutData) != OK)
      RM0003_RETURN(functName, signed short int, -R_SSTOER )

  sRetCode = ro0211_pushOutEvent (RC0213_Transport,
                                  RC0907_TDATAreq,
                                  ulParam1,  
                                  ulParam2,
                                  NULL,
                                  NULL);
  if (sRetCode != OK)
      RM0003_RETURN(functName, signed short int, sRetCode )

/**********************************************************************/
/**********************************************************************/

RM0003_RETURN(functName, signed short int, OK)
}/*end-ro0844_STA01BxTCONcnf*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0845_STA01CxCN                                    */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-22                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0845_STA01CxCN ( 
rT0870_stSpmContext *pContext,
unsigned long int hUserData,
unsigned long int ulOffset, 
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:  ^p01 &^ p76 & p204                */
/*                     actions   :   [50]                             */
/*                     next event:   OA                               */
/*                     next state:   RC08d4_STA01D                    */
/*                                                                    */
/*                     predicates:  ^p01 &^ p76 & ^p204               */
/*                     actions   :   [50]                             */
/*                     next event:   S_CONNECT.indication             */
/*                     next state:   RC08d9_STA08                     */
/*                                                                    */
/*                     predicates:  ^p01 & p76 & p02                  */
/*                     actions   :   [4]                              */
/*                     next event:   RF-nr                            */
/*                     next state:   RC08db_STA16                     */
/*                                                                    */
/*                     predicates:   p01                              */
/*                     actions   :   -----------                      */
/*                     next event:   T_DISCONNECTrequest              */
/*                     next state:   RC08d0_STA01                     */
/*                                                                    */
/* IN-PARAMETERS         :                                            */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hP3       = Third parameter of transaction function.              */
/*  hP4       = Fourth parameter of transaction function.             */
/*                                                                    */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/


/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr-ro0845_STA01CxCN*/

signed   short int           slength_address;
signed   short int           addr_cmp  = 0;
char                         vOwnAddress [ RC0830_AddressLen + 1];

unsigned long  int           hInData   = LNULL;
char  FAR                  * pInData   = NULL;

unsigned long  int           hOutData  = LNULL;
char   FAR                 * pOutData  = NULL;


unsigned long  int           hConnIndAcpm;
rTro7f_stAssocIndAcpm FAR  * pConnIndAcpm;

rT0814_stCN   stCN;
rT0819_stRF   stRF;
rT0818_stOA   stOA;
signed   short int           sRetCode  = OK;

unsigned long  int           ulParam1  = LNULL;
unsigned long  int           ulParam2  = LNULL;
unsigned long  int           ulParam3  = LNULL;
signed   long  int           lStorelen = LNULL;
signed   long  int           lBuflen   = LNULL;
   RM0001_STRG(functName,"ro0845_STA01CxCN")
   RM0002_ENTRY(functName)

/* ********************** S T A R T ************************************ */
/* **********************           ************************************ */

  ro082B_InitStruc( &stCN , sizeof (rT0814_stCN ));
  ro082B_InitStruc( &stRF , sizeof (rT0819_stRF ));
  ro082B_InitStruc( &stOA , sizeof (rT0818_stOA ));
  ro082B_InitStruc(vOwnAddress , (RC0830_AddressLen + 1) );

  hInData = hUserData;

  if (hInData) {
    if(ro0074_storeactive(hInData , 0L) != OK ) 
      RM0003_RETURN(functName, signed short int, -R_FSTERR )
  }
  
  lBuflen = ro080a_StoreOffset(0,hInData,ulOffset);
   /* fro Redir by kc on 7/7/93 */
  pContext->stRedir.ulOffset = ulOffset;

  if( ! lBuflen)
        RM0003_RETURN(functName, signed short int, -R_SSTOER)
  if ((pInData = ro0072_stgets(hInData,lBuflen)) == NULL)
        RM0003_RETURN(functName, signed short int, -R_SSTOER)

  stCN.usFunctionalUnits = RC08ca_FUValue;
  stCN.pDataptr          = pInData + ulOffset;

  lStorelen = ro0062_storelength(hInData);
  if (ro0063_sterror(hInData) != OK )
      RM0003_RETURN(functName, signed short int, -R_SSTOER)
  stCN.lDatalen = lStorelen - ulOffset; 

  if ( ro0824_DecodeCN ((rT0814_stCN FAR *)&stCN) != 0)
  {
    if(ro0073_storeidle(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_SSTOER)
    RM0003_RETURN(functName, signed short int, 
          ro0831_InvalidSpdu(pContext,hInData,RC0831_MaxParamLen,NULL,NULL)) 
  }
  if (!pContext->hAddrs ){
    if ((pContext->hAddrs = ro0030_localalloc (RCro1e_MaxLenPSAP)) == 0) 
      RM0003_RETURN(functName, signed short int, -R_LALLCE )
  }
  /* by kc on 08-12-93 for called address */
  if(!pContext->hCalledAddress){
    if ((pContext->hCalledAddress= ro0030_localalloc (RCro1e_MaxLenPSAP)) == 0) 
      RM0003_RETURN(functName, signed short int, -R_LALLCE )
  }
  /* end 08-12-93 */          
           
  sRetCode = ro0218_conAddress (&pContext->hAddrs,  /* copies the incoming    */
                                 RCro13_Sselector,  /* calling address into   */
                                 stCN.sCallingLen,  /* the address buffer     */
                                (char FAR *)stCN.vCalling);
  
  if (sRetCode < OK)
      RM0003_RETURN(functName, signed short int, sRetCode)
  /* kc on 08-12-93 */
  sRetCode = ro0218_conAddress (&pContext->hCalledAddress,  /* copies the incoming    */
                                 RCro13_Sselector,  /* calling address into   */
                                 stCN.sCalledLen,  /* the address buffer     */
                                (char FAR *)stCN.vCalled);
  
  if (sRetCode < OK)
      RM0003_RETURN(functName, signed short int, sRetCode)
  /* kc on 08-12-93 */ 
  
  /***********     
   Note :- for REDIRECTION ..  
     Incomming calling address and address length is stored in 
     pContext block. This information is required by responding 
     side when redirection is to be done..
  ***********/
  
  /* pContext->stRedir.sCallingAddrsLen = stCN.sCallingLen;
   ro0041_memcpy((char FAR *)pContext->stRedir.vCallingAddrs,
                 (char FAR *)stCN.vCalling,
                 (unsigned long int)stCN.sCallingLen);

   pContext->stRedir.usFunctionalUnits = pContext->usFunctionalUnits;
   pContext->stRedir.usPopt            = pContext->usPopt;
   pContext->stRedir.usProtvers        = pContext->usProtvers; 
 */ 
/**********************************************************************/
/**********************************************************************/

  if (!pContext->usVtca)     /* SPM initiated ie. received TCONreq */
  {
      sRetCode = ro0211_pushOutEvent (RC0213_Transport,
                                      RC0905_TDISCONNECTreq,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL);
      if (sRetCode != OK)
          RM0003_RETURN(functName, signed short int, sRetCode)

      pContext->usState = RC08d0_STA01;  /* ie. idle,no transport connection */ 

      if(ro0034_localfree (pContext->hAddrs) != NULL)
          RM0003_RETURN(functName, signed short int, -R_LFREEE)
      pContext->hAddrs = NULL;
    
      if(ro0034_localfree (pContext->hCalledAddress) != NULL)
          RM0003_RETURN(functName, signed short int, -R_LFREEE)
      pContext->hCalledAddress = NULL;
      if(ro0073_storeidle (hInData) != OK)
        RM0003_RETURN(functName, signed short int, -R_SSTOER )

      if(ro0075_deletestore (hInData) != OK)
        RM0003_RETURN(functName, signed short int, -R_SSTOER )

  }/* SPM initiated */
  else                    /*      SPM not initiated   usVtca == TRUE    */
  {

    slength_address = 0;

    slength_address = 
      ro0217_getAddress(NULL,            /* get the own address to compare  */
                      RCro13_Sselector,    /* the incoming address with the   */
                      RC0830_AddressLen, /* own address                     */
                      (char FAR *)vOwnAddress);
    if (slength_address < OK)
        RM0003_RETURN(functName, signed short int,
                                             (signed short int)slength_address )

    /* compare called address with the own address */ 

    addr_cmp = (signed short int) ro0045_memAndLenCmp (
                               (char FAR *)vOwnAddress,
                               (char FAR *)stCN.vCalled,
                               (unsigned long int)slength_address, 
                               (unsigned long int)stCN.sCalledLen);

/*--------------------------------------------------------------------*/
/* checks if incoming call can be accepted!                           */
/* The call is not be accepted if                                     */
/*   - usOverflow item is set and version is 1                        */
/*   - the session selector is not correct.                           */
/*   - the version is not 1 or 2                                      */
/*--------------------------------------------------------------------*/
    if ((addr_cmp != OK) ||   /* session selector */
        (((stCN.usVersion & RCro50_ProtVers1) == RCro50_ProtVers1) && 
          (stCN.usOverflow)) ||
        (!(((stCN.usVersion & RCro50_ProtVers1) == RCro50_ProtVers1) ||
           ((stCN.usVersion & RCro51_ProtVers2)  == RCro51_ProtVers2))))
    {

        /* --------------------------------------- #
        #       CALL IS NOT ACCEPTED               #  
        # ---------------------------------------- */
   
      hOutData = ro0060_newstore(ST_USEMAXBUF,ST_CLOSED,
                                 (signed short int FAR *)&sRetCode);
      if(sRetCode != OK)
        RM0003_RETURN(functName, signed short int, -R_SSTOER)

      lStorelen = ro0062_storelength(hInData);
      if (ro0063_sterror(hInData) != OK )
          RM0003_RETURN(functName, signed short int, -R_SSTOER)

      ulOffset = RC0831_MaxParamLen;

      if ((pOutData = ro0069_stputs(hOutData,ulOffset)) == NULL)
          RM0003_RETURN(functName, signed short int, -R_SSTOER)

      pContext->usProtvers    = stCN.usVersion;
      
      stRF.usFunctionalUnits  = RC08ca_FUValue;
      stRF.pDataptr           = pOutData + ulOffset;
      stRF.lDatalen           = NULL;

      stRF.usTdiscAreuse      = RC0871_NotReuse;
      stRF.usTdiscUserAbort   = RC0857_NoUserAbort;
      stRF.usVersion          = pContext->usProtvers;
      stRF.ulEnclosure        = RC0876_OneSPDU;

      if (addr_cmp != OK)
       stRF.usReason          = RC08c5_SessionSelectorUnknown;
      else
       stRF.usReason          = Rc08c6_RefuseNoReason;   

      ro0819_EncodeRF((rT0819_stRF FAR *)&stRF);

      ulParam1 = hOutData;  
      ulParam2 = (unsigned long int)(stRF.pDataptr - pOutData);

      if(ro0073_storeidle(hOutData) != OK) 
          RM0003_RETURN(functName, signed short int, -R_SSTOER)

      pContext->usState   =  RC08db_STA16;  /* ie. Wait for TCONind */

      sRetCode = ro0211_pushOutEvent (RC0213_Transport,
                                      RC0907_TDATAreq,
                                      ulParam1, 
                                      ulParam2,
                                      NULL,  
                                      NULL);
      if (sRetCode != OK)
          RM0003_RETURN(functName, signed short int, sRetCode)

      if (pContext->hAddrs) 
          if(ro0034_localfree(pContext->hAddrs) != NULL)
              RM0003_RETURN(functName, signed short int, -R_LFREEE)
      pContext->hAddrs = NULL;
      if(ro0034_localfree(pContext->hCalledAddress) != NULL)
              RM0003_RETURN(functName, signed short int, -R_LFREEE)
      pContext->hCalledAddress = NULL;

     if(ro0073_storeidle(hInData) != OK)
           RM0003_RETURN(functName, signed short int, -R_SSTOER )

     if(ro0075_deletestore(hInData) != OK)
           RM0003_RETURN(functName, signed short int, -R_SSTOER )

    }/* if-call is not accepted */
    else
    {
/**********************************************************************/
/**********************************************************************/

        /* --------------------------------------- #
        #         CALL IS ACCEPTED     		   #  
        # ---------------------------------------- */
/* stRedir is removed by kc on 7/7/93 */
      pContext->usPopt                    = stCN.usPopt;
      pContext->usFunctionalUnits         = stCN.usFunctionalUnits;
      pContext->usProtvers                = stCN.usVersion;
 /* pContext->stRedir parameters are added by kc  on 1/7/93 */
/************************************************************************/
/************************
BANNED ......

  if ((pContext->ulTidusize < stCN.ulTSDUmaxfrom) || (stCN.ulTSDUmaxfrom == 0))
        stOA.ulTSDUmaxfrom = pContext->ulTidusize;
  else
        pContext->ulTidusize = stCN.ulTSDUmaxfrom;
      
  if ((pContext->ulTidusize < stCN.ulTSDUmaxto)  || (stCN.ulTSDUmaxto == 0))
        stOA.ulTSDUmaxto = pContext->ulTidusize;
  else
        pContext->ulTidusize = stCN.ulTSDUmaxto;

BANNED ......
**************************/
/***********************************************************************/

      if (stCN.usOverflow == RC087c_Overflow)
      {
         /* -------------------------------------------------------- #
          # NOTE :- IMPLIMENTATION LIMITATIONS OF RTROS V-3.00       #
          #         Data Overflow is banned while sending,           #
          #         hence it is not supported in receiving also.     # 
          # -------------------------------------------------------- */

          if(ro0073_storeidle(hInData) != OK)
               RM0003_RETURN(functName, signed short int, -R_SSTOER )

          if(ro0075_deletestore(hInData) != OK)
               RM0003_RETURN(functName, signed short int, -R_SSTOER )

          pContext->usUserConnExists = FALSE;
          pContext->usState = RC08d0_STA01;

          RM0003_RETURN(functName, signed short int,
                                 ro0211_pushOutEvent (RC0215_Presentation,
			                              RC080b_SPABORTind,
                                                      NULL,
                                                      NULL,                  
                                                      NULL,
                                                      RC0313_RestrictionInPICS))

          /* ----------------------------------------------#
           #  Cleaning up of SPM Context block is carried  # 
  	   #  out in generic function                      #
           # --------------------------------------------- */
       
/*********************************************************************
  DATA OVERFLOW BANNED .........


         hOutData = ro0060_newstore(ST_USEMAXBUF,ST_CLOSED,
                                    (signed short int FAR *)&sRetCode);
         if(sRetCode != OK)
            RM0003_RETURN(functName, signed short int, -R_SSTOER)

         lStorelen = ro0062_storelength(hInData);
         if (ro0063_sterror(hInData) != OK )
             RM0003_RETURN(functName, signed short int, -R_SSTOER)

         ulOffset = RC0831_MaxParamLen;

         if ((pOutData = ro0069_stputs(hOutData,ulOffset)) == NULL)
            RM0003_RETURN(functName, signed short int, -R_SSTOER)

        stOA.pDataptr = pOutData + ulOffset;
        stOA.lDatalen = NULL;

        stOA.ulTSDUmaxto   = pContext->ulTidusize;
        stOA.ulTSDUmaxfrom = pContext->ulTidusize;
        stOA.usVersion     = RCro51_ProtVers2;      

        Encode_OA((rT0818_stOA FAR *)&stOA);

        ulParam1 = hOutData; 
        ulParam2 = (unsigned long int)(stOA.pDataptr - pOutData);

        if(ro0073_storeidle(hOutData) != OK)
           RM0003_RETURN(functName, signed short int, -R_SSTOER )

        ro0211_pushOutEvent(RC0213_Transport,
                            RC0907_TDATAreq,
                            ulParam1,
                            ulParam2, 
                            NULL,   
                            NULL);

  DATA OVERFLOW BANNED .........
******************************************/

      }/*if(stCN.usOverflow == RC087c_Overflow)*/
      
/**********************************************************************/
/**********************************************************************/

      if ( (hConnIndAcpm = ro0030_localalloc(sizeof(rTro7f_stAssocIndAcpm)) )
            == NULL)
        RM0003_RETURN(functName, signed short int, -R_LALLCE)

      if ((pConnIndAcpm =
          (rTro7f_stAssocIndAcpm FAR *) ro0031_locallock(hConnIndAcpm)) == NULL)
        RM0003_RETURN(functName, signed short int, -R_LLOCKE)
      
      pConnIndAcpm->stAll.hCallingAddress = pContext->hAddrs;
      pConnIndAcpm->stAll.hCalledAddress = pContext->hCalledAddress;
     
      pConnIndAcpm->stSpm.ulRequirements  = 
               (stCN.usFunctionalUnits & RCro53_FullDuplex);
      pConnIndAcpm->stAll.sSessProtVers   = stCN.usVersion;

      /****************************************************
       NOTE :- 
 	Since the handle pContext->hAddrs is assigned to 
        pConnIndAcpm->stAll.hCallingAddress only make it NULL
        but do not free the memory area ... 
      ****************************************************/

      pContext->hAddrs = NULL;
      pContext->hCalledAddress = NULL;
   
      if ( ro0032_localunlock(hConnIndAcpm) != NULL)
          RM0003_RETURN(functName, signed short int, - R_LULOKE)
      
      if (stCN.lDatalen != NULL)    /* User Data Available  */
      {
         ulParam1 = hInData;
         
         ulParam2 = (unsigned long int)(stCN.pDataptr - pInData);
         ulParam3 = hConnIndAcpm;

        /* Saving of store for Redirect  by kc on 7/7/93 */
         if(ro0078_stlink ( hInData) < 0 ) 
              RM0003_RETURN(functName, signed short int, - R_SSTOER)

         pContext->stRedir.hsStore    = hInData;
   
        if(ro0073_storeidle(hInData) != OK)
           RM0003_RETURN(functName, signed short int, -R_SSTOER )
      }
      else                       /* User Data not available */ 
      {
         ulParam1 = NULL;
         ulParam2 = NULL;
         ulParam3 = hConnIndAcpm; 

         pContext->stRedir.hsStore    = NULL;
         if(ro0073_storeidle(hInData) != OK)
             RM0003_RETURN(functName, signed short int, -R_SSTOER )

         if(ro0075_deletestore(hInData) != OK)
             RM0003_RETURN(functName, signed short int, -R_SSTOER )

      }
      

/*****************************   
BANNED .......

      if (stCN.usOverflow == RC087c_Overflow)
          pContext->usState = RC08d4_STA01D;   * ie. Wait for CDO SPDU *
      else
          pContext->usState = RC08d9_STA08;   *  ie. Wait for SCONrsp * 
BANNED ........
***********************/

      pContext->usState = RC08d9_STA08;     /* ie. Wait for SCONrsp  */
      pContext->usUserConnExists = TRUE;

      sRetCode = ro0211_pushOutEvent (RC0215_Presentation,
                                      RC0802_SCONNECTind,
                                      ulParam1,
                                      ulParam2,
                                      ulParam3,
                                      hP4);

  }/* Call is accepted */
 }/* SPM not initiated */
RM0003_RETURN(functName, signed short int, sRetCode)

}/*end-ro0845_STA01CxCN*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0846_STA01CxSCONreq                               */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-22                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0846_STA01CxSCONreq (
rT0870_stSpmContext *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hRespondAddr,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   p01 & p204                       */
/*                     actions   :   ------------                     */
/*                     next event:   CN                               */
/*                     next state:   RC08d7_STA02B                    */
/*                                                                    */
/*                     predicates:   p01 & ^p204                      */
/*                     actions   :   ------------                     */
/*                     next event:   CN                               */
/*                     next state:   RC08d6_STA02A                    */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS          :                                           */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hRespondAddr = Handle to the responding address.                  */
/*  hP4       = Fourth parameter of transaction function.             */
/*                                                                    */
/* FORMAL OUT-PARAMETERS  :                                           */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr-ro0846_STA01CxSCONreq*/

  RM0001_STRG(functName,"ro0846_STA01CxSCONreq")
  signed short int     sRetCode = OK;  

/* ****************** S T A R T ************************************* */

  RM0002_ENTRY(functName)

if (pContext->usVtca) /*   TRANSPORT INITIATED  ?????????   */                  
  sRetCode = -R_SINTER;                                                         
else /*   SPM received the TCONind  usVtca == FALSE ? */                        
  sRetCode=ro0844_STA01BxTCONcnf(pContext,hUserData,ulOffset,hRespondAddr,hP4); 
                                                                                
RM0003_RETURN(functName, signed short int, sRetCode)                            
}/*end-ro0846_STA01CxSCONreq*/
/*EOF*/
