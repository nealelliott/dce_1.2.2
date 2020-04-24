/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm06.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:25  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:01  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:52  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:19  keutel
 * 	OT11620
 * 	[1994/08/19  13:57:40  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:13  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:29  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:08:35  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:09  keutel
 * 	creation
 * 	[1994/03/21  13:37:18  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rospm06.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:25 $" } ;
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
/* NAME         : rospm06.c                                           */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-05                                            */
/*                                                                    */
/* COMPONENT    : RTROS  Convergence protocol machine                 */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : This module provides functions which are            */
/*                triggered in states                                 */ 
/*                STA03 : Wait for DN SPDU                            */
/*                STA08 : Wait for SCONNECT responce                  */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-04-05|  original                      |ma  |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "@(#) rospm06.c 1.1 93/11/29 RTROS-V3.00";

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
#include <rospm07.h>    /* Includes local to protocol machine         */ 
#include <rospm03.h>    /* Defines for convergence protocol m/c       */  
#include <rospm05.h>    /* Prototyping of coding of functions         */ 
#include <rospm06.h>    /* Prototyping of convergence protocol m/c    */    

/*********************************************************************/
/*                                                                   */
/*              D E F I N E S                                        */
/*                                                                   */
/*********************************************************************/

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
/* NAME         : ro0860_STA03xFN                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-05                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
signed short int NEAR PASCAL ro0860_STA03xFN (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int ulP3,
unsigned long int ulP4)
/*                                                                    */
/* DESCRIPTION  :      predicates:   ^p65                             */
/*                     actions   :   [8][18]                          */
/*                     next event:   SRELind                          */
/*                     next state:   RC08da_STA09                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  ulP3      = Third parameter of transaction functon.               */
/*  ulP4      = Fourth parameter of transaction function.             */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
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
{/*entr-ro0860_STA03xFN*/

unsigned long  int    hInData   = LNULL;  
char FAR            * pInData;

rT0817_stFN           stFN;
unsigned long  int    ulParam1  = LNULL;
unsigned long  int    ulParam2  = LNULL;
signed   long  int    lStorelen = LNULL;
signed   long  int    lBuflen   = LNULL;

  RM0001_STRG(functName,"ro0860_STA03xFN")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

  ro082B_InitStruc(&stFN,sizeof(rT0817_stFN));

  hInData = hUserData;
  
  if(ro0074_storeactive(hInData , 0L) != OK ) 
       RM0003_RETURN(functName, signed short int, -R_FSTERR )

  lBuflen = ro080a_StoreOffset(0,hInData,ulOffset);
  if( ! lBuflen)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
  if ((pInData = ro0072_stgets(hInData,lBuflen)) == NULL)
      RM0003_RETURN(functName, signed short int, -R_FSTERR)
 
  stFN.usFunctionalUnits     = RC08ca_FUValue;  
  stFN.pDataptr              = pInData + ulOffset;

  lStorelen      = ro0062_storelength(hInData); 
  if(ro0063_sterror(hInData) != OK )
      RM0003_RETURN(functName, signed short int, -R_FSTERR)
  stFN.lDatalen    = lStorelen - ulOffset;

  if ( ro0827_DecodeFN((rT0817_stFN FAR *)&stFN) != 0)
  {
     if(ro0073_storeidle(hInData) != OK)
           RM0003_RETURN(functName, signed short int, -R_FSTERR)
     RM0003_RETURN(functName, signed short int, 
           ro0831_InvalidSpdu(pContext,hInData,RC0831_MaxParamLen,NULL,NULL))
  } 
  pContext->usAreuse = stFN.usTdiscAreuse;
  pContext->usUabort = stFN.usTdiscUserAbort;

  if (stFN.lDatalen == 0)    /* No User Data  */
  {
    ulParam1 = NULL;
    ulParam2 = NULL;

    if(ro0073_storeidle(hInData) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR )

    if(ro0075_deletestore(hInData) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR )
  }
  else                      /* User Data Available */
  {
    ulParam1 = hInData;   
    /* ulParam2 = (unsigned long int)(pInData - stFN.pDataptr); */
     ulParam2 = (unsigned long int)(stFN.pDataptr - pInData); 

    if(ro0073_storeidle(hInData) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR )
  }

  pContext->usState = RC08da_STA09;     /* ie. wait for SRELrsp */
  pContext->usVtrr  = FALSE;            /*  NOT_REUSE           */
  pContext->usVcoll = TRUE;             /*  FN collision        */

RM0003_RETURN (functName, signed short int, ro0211_pushOutEvent (
                                                            RC0215_Presentation,
                                                            RC0806_SRELEASEind,
                                                            ulParam1,
                                                            ulParam2,
                                                            NULL,
                                                            NULL))

}/*end-ro0860_STA03xFN*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0861_STA03xDT                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-05                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0861_STA03xDT ( 
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int ulP3,
unsigned long int ulP4)

/* DESCRIPTION  :      predicates:   p10                              */
/*                     actions   :   -------                          */
/*                     next event:   SDATind                          */
/*                     next state:   RC08d8_STA03                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  ulP3      = Third parameter of transaction functon.               */
/*  ulP4      = Fourth parameter of transaction function.             */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
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
{/*entr-ro0861_STA03xDT*/

unsigned long  int         hInData;
char                 FAR * pInData;

rT0816_stDT                stDT;
signed   long  int         lStorelen = LNULL;
signed   long  int         lBuflen   = LNULL;
unsigned long  int         ulParam1  = LNULL;
unsigned long  int         ulParam2  = LNULL;

  RM0001_STRG(functName,"ro0861_STA03xDT")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

  ro082B_InitStruc(&stDT,sizeof(rT0816_stDT));

  hInData = hUserData;

  if (!pContext->usVcoll)                 /* usVcoll == NO_COLLISION) */
  {
    if(ro0074_storeactive(hInData , 0L) != OK ) 
         RM0003_RETURN(functName, signed short int, -R_FSTERR )

    lBuflen = ro080a_StoreOffset(0,hInData,ulOffset);
    if( ! lBuflen)
         RM0003_RETURN(functName, signed short int, -R_FSTERR)
    if ((pInData = ro0072_stgets(hInData,lBuflen)) == NULL )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)

    stDT.usFunctionalUnits = RC08ca_FUValue;
    
    stDT.pDataptr = pInData + ulOffset;

    lStorelen = ro0062_storelength(hInData); 
    if(ro0063_sterror(hInData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)
    stDT.lDatalen = lStorelen - ulOffset;
  
    if ( ro0826_DecodeDT((rT0816_stDT FAR *)&stDT) != 0)
    {
      if(ro0073_storeidle(hInData) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR)
      RM0003_RETURN(functName, signed short int, 
            ro0831_InvalidSpdu(pContext,hInData,RC0831_MaxParamLen,NULL,NULL))
    }
 
    ulParam1 = hInData;
    ulParam2 = (unsigned long int)(stDT.pDataptr - pInData);

    if(ro0073_storeidle(hInData) != OK)
       RM0003_RETURN(functName, signed short int, -R_FSTERR )
  
    RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent (
                                                            RC0215_Presentation,
                                                            RC080d_SDATAind, 
                                                            ulParam1,
                                                            ulParam2,
                                                            NULL,
                                                            NULL))
  }
  else
      RM0003_RETURN(functName, signed short int, -R_SINTER)

}/*end-ro0861_STA03xDT*/ 


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-05                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0862_STA03xDN (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int ulP3,
unsigned long int ulP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   ^p66                             */
/*                     actions   :   -------                          */
/*                     next event:   SRELcnf+                         */
/*                     next event:   TDISreq                          */
/*                     next state:   RC08d0_STA01                     */
/*                                                                    */
/*                     predicates:    p66                             */
/*                     actions   :   -------                          */
/*                     next event:   SRELcnf+                         */
/*                     next state:   RC08d3_STA01C                    */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  ulP3      = Third parameter of transaction functon.               */
/*  ulP4      = Fourth parameter of transaction function.             */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
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
{/*entr-ro0862_STA03xDN*/
unsigned long  int         hInData   = LNULL;  
char                 FAR * pInData   = NULL;

rT0815_stDN                stDN;
unsigned long  int         ulParam1  = LNULL;
unsigned long  int         ulParam2  = LNULL;
unsigned long  int         ulParam3  = LNULL;
unsigned long  int         ulParam4  = LNULL;
signed   long  int         lStorelen = LNULL;
signed   long  int         lBuflen   = LNULL;
signed   short int         sRetCode  = OK;

  RM0001_STRG(functName,"ro0862_STA03xDN")
  RM0002_ENTRY(functName)

/* *********************** S T A R T ********************************** */
/* ***********************           ********************************** */

  ro082B_InitStruc(&stDN,sizeof(rT0815_stDN));

  hInData = hUserData;

   if(ro0074_storeactive(hInData , 0L) != OK ) 
         RM0003_RETURN(functName, signed short int, -R_FSTERR )

   lBuflen = ro080a_StoreOffset(0,hInData,ulOffset);
   if( ! lBuflen)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
   if ((pInData = ro0072_stgets(hInData,lBuflen)) == NULL)
         RM0003_RETURN(functName, signed short int, -R_FSTERR)

  stDN.usFunctionalUnits  = RC08ca_FUValue;
  stDN.pDataptr           = pInData + ulOffset;
    
  lStorelen   = ro0062_storelength(hInData); 
  if(ro0063_sterror(hInData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)
  stDN.lDatalen = lStorelen - ulOffset;

  if ( ro0825_DecodeDN((rT0815_stDN FAR *)&stDN) != 0)
  { 
    if(ro0073_storeidle(hInData) != OK)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
    RM0003_RETURN(functName, signed short int, 
        ro0831_InvalidSpdu(pContext,hInData,RC0831_MaxParamLen,NULL,NULL))
  }

  if (stDN.lDatalen == 0)  /*  No User Data */ 
  {
    ulParam1 = NULL;
    ulParam2 =NULL;

    if(ro0073_storeidle(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

    if(ro0075_deletestore(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )
  }
  else
  {
    ulParam1 = hInData;
    ulParam2 = (unsigned long int)(stDN.pDataptr - pInData);

    if(ro0073_storeidle(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )
  }
  ulParam3 = NULL;
  ulParam4 = RCro90_Affirmative;

  sRetCode = ro0211_pushOutEvent (RC0215_Presentation, 
                                  RC0808_SRELEASEcnf,
                                  ulParam1,
                                  ulParam2,
                                  ulParam3,
                                  ulParam4);
  if (sRetCode != OK)
      RM0003_RETURN(functName, signed short int, sRetCode )

  pContext->usUserConnExists = FALSE;

  if (!pContext->usVtrr)
  {
      sRetCode = ro0211_pushOutEvent (RC0213_Transport,
                                      RC0905_TDISCONNECTreq,
                                      NULL,
                                      NULL,
                                      2,         
                                      NULL);
      
      pContext->usState = RC08d0_STA01;  /*ie. Idle , no transport connection */
  }
  else
  {
      pContext->usState = RC08d3_STA01C; /* ie. Transport connected           */
  }

RM0003_RETURN(functName, signed short int, sRetCode)
}/*end-ro0862_STA03xDN*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0863_STA08xSCONrspPos                             */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-05                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0863_STA08xSCONrspPos (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hConnRspAcpm,
unsigned long int ulConResult) 
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  :      predicates:   ------------                     */
/*                     actions   :   [5]                              */
/*                     next event:   AC                               */
/*                     next state:   RC08dc_STA713                    */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hConnRspAcpm = Memory handle to connect responce service paaram.  */
/*  ulConResult  = Connection result.                                 */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
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
{/*entr-ro0863_STA08xSCONrspPos*/

signed   short int          sRetCode  = OK;
unsigned long  int          hOutData  = LNULL;  
char                  FAR * pOutData  = NULL;

rTro7g_stAssocRspAcpm FAR * pConnRspAcpm;

rT0812_stAC stAC;
unsigned long  int          ulParam1  = LNULL;
unsigned long  int          ulParam2  = LNULL;
signed   long  int          lStorelen = LNULL;

 RM0001_STRG(functName,"ro0863_STA08xSCONrspPos")
 RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */
 /* by kc on 7/7/93  */
 /* Since RspPos has come no meaning to hold the Redir Store          */
 if ( pContext->stRedir.hsStore ) {
   if ( ro0075_deletestore ( pContext->stRedir.hsStore) < 0  )
      RM0003_RETURN(functName, signed short int, -R_SSTOER)
   pContext->stRedir.hsStore = NULL;
   pContext->stRedir.ulOffset = NULL;
 }
    
 ro082B_InitStruc(&stAC,sizeof(rT0812_stAC));

 hOutData = hUserData; 
  
 if ((pConnRspAcpm =
     (rTro7g_stAssocRspAcpm FAR *)ro0031_locallock(hConnRspAcpm)) == NULL)
         RM0003_RETURN(functName, signed short int, -R_LLOCKE)
  
  stAC.sRespondingLen = 
        ro0217_getAddress(pConnRspAcpm->stAll.hRespondAddress,
                          RCro13_Sselector,
                          RC0830_AddressLen,
                          (char FAR *)stAC.ucResponding);
  if (stAC.sRespondingLen < OK)
      RM0003_RETURN(functName, signed short int, stAC.sRespondingLen)
   
 if ( (pConnRspAcpm->stSpm.ulRequirements & RCro53_FullDuplex) 
           == RCro53_FullDuplex ) 
       pContext->usFunctionalUnits =
                 (unsigned short int) pConnRspAcpm->stSpm.ulRequirements; 
 else
      RM0003_RETURN(functName, signed short int, -R_SINTER)

 if ( ((pConnRspAcpm->stAll.sSessProtVers & RCro50_ProtVers1) 
          == RCro50_ProtVers1) ||
      ((pConnRspAcpm->stAll.sSessProtVers & RCro51_ProtVers2)
          == RCro51_ProtVers2)  )
    pContext->usProtvers  =   pConnRspAcpm->stAll.sSessProtVers; 
 else
    RM0003_RETURN(functName, signed short int, -R_SINTER)

 pContext->ulConectresult  =  ulConResult;
 
 if (pConnRspAcpm->stAll.hRespondAddress) {
     if (ro0034_localfree(pConnRspAcpm->stAll.hRespondAddress) != OK)
         sRetCode = -R_LFREEE;
 }
 if (sRetCode == OK) {
     if      (ro0032_localunlock (hConnRspAcpm) != OK) sRetCode = -R_LULOKE;
     else if (ro0034_localfree   (hConnRspAcpm) != OK) sRetCode = -R_LFREEE; 
 }
 if (sRetCode != OK)
     RM0003_RETURN(functName, signed short int, sRetCode)

 stAC.sCallingLen = ro0217_getAddress( NULL,
                                        RCro13_Sselector,
                                        RC0830_AddressLen,
                                        (char FAR *)stAC.vCalling );
 if (stAC.sCallingLen < OK)
     RM0003_RETURN(functName, signed short int, stAC.sCallingLen)

  stAC.usPopt             =  pContext->usPopt;
  stAC.ulTSDUmaxto        =  pContext->ulTidusize;
  stAC.ulTSDUmaxfrom      =  pContext->ulTidusize;
  stAC.usVersion          =  pContext->usProtvers; 
  stAC.usFunctionalUnits  =  pContext->usFunctionalUnits;
  stAC.ulEnclosure        =  RC0876_OneSPDU;    
  
  if (hOutData == NULL)    /* No User Data */
  {
    hOutData = ro0060_newstore(ST_USEMAXBUF,ST_CLOSED,
			       (signed short int FAR *)&sRetCode);
    if(sRetCode != OK)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)

    ulOffset = RC0831_MaxParamLen; 

    lStorelen = ro0062_storelength(hOutData); 
    if (ro0063_sterror(hOutData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)

    if ((pOutData = ro0069_stputs(hOutData,(long)ulOffset)) == NULL)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)

    stAC.pDataptr = pOutData + ulOffset; 
    stAC.lDatalen = NULL;
  }
  else  /* User Data Available */
  {

    if(ro0074_storeactive(hOutData , 0L) != OK ) 
         RM0003_RETURN(functName, signed short int, -R_FSTERR )

    if ((pOutData = ro0069_stputs(hOutData,(long)ulOffset)) == NULL)
         RM0003_RETURN(functName, signed short int, -R_FSTERR)
    stAC.pDataptr = pOutData + ulOffset;

    lStorelen = ro0062_storelength(hOutData); 
    if (ro0063_sterror(hOutData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)
    stAC.lDatalen = lStorelen - ulOffset;
  }
/* by kc on 12-10-93 */
/*   if(lStorelen > RC0835_MaxConnectLen) by kc 29-07-94 K02    
     for the Conformance test     */                            
  if (stAC.lDatalen > RC0835_MaxConnectLen)                     
  {
    RM0003_RETURN(functName, signed short int, 
        ro0831_InvalidSpdu(pContext,hOutData,RC0831_MaxParamLen,NULL,NULL))
     
  } 
   ro0812_EncodeAC ((rT0812_stAC FAR *)&stAC);                          

   ulParam1 = hOutData;          
   ulParam2 = (unsigned long int)(stAC.pDataptr - pOutData);

   pContext->usVact  = FALSE;             /*    NO_Activity;         */
   pContext->usVcoll = FALSE;             /*    NO_COLLISION;        */
   pContext->usVdnr  = FALSE;             /*    DISCON_SPDU;         */
   pContext->usTexp  = FALSE;             /*    no expedited service */ 

   pContext->usState = RC08dc_STA713;

   if(ro0073_storeidle(hOutData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )
  
RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent(RC0213_Transport,
                                                               RC0907_TDATAreq,
                                                               ulParam1,
                                                               ulParam2,
                                                               NULL,  
                                                               NULL))
  
}/*end-ro0863_STA08xSCONrspPos*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0864_STA08xSCONrspNeg                             */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-05                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
signed short int NEAR PASCAL ro0864_STA08xSCONrspNeg (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffsetorPID,
unsigned long int hConnRspAcpm,
unsigned long int ulConResult)

/*                                                                    */
/* DESCRIPTION  :      predicates:   ^p02                             */
/*                     actions   :   [4]                              */
/*                     next event:   RF-nr                            */
/*                     next state:   RC08db_STA16                     */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffsetorPID  = Offset in store from where encoding/decoding     */
/*              should start.                                         */
/*              PID if Redirection indicated by ulConResult.          */
/*  hConnRspAcpm = Memory handle to connection responce               */
/*              service parameter OR                                  */
/*  ulConResult       = Connection Result.                            */ 
/*          if ulConResult & RCro85_Redirection == RCro85_Redirection */
/*          Connection Responce negative means Redirection request    */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
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
{/*entr-ro0864_STA08xSCONrspNeg*/

unsigned long  int          hOutData  = LNULL;  
char                  FAR * pOutData  = LNULL;

rTro7g_stAssocRspAcpm FAR * pConnRspAcpm;
rT0819_stRF                 stRF;

signed   long  int          lStorelen = LNULL;
unsigned long  int          ulParam1  = LNULL;
unsigned long  int          ulParam2  = LNULL;
signed   short int          sRetCode  = OK;

  RM0001_STRG(functName,"ro0864_STA08xSCONrspNeg")
  RM0002_ENTRY(functName)

/* **************************** S T A R T ******************************* */
/* ****************************           ******************************* */

 /*  Timer not implemented   */


  if((ulConResult & RCro85_Redirection) ==  RCro85_Redirection)
  {
   /* ***************** REDIRECT ************************ */
   /* *****************          ************************ */
/* Commented by kc on 7/7/93 
    rT0814_stCN                 stCN;

    ro082B_InitStruc((rT0814_stCN FAR *)&stCN , sizeof(rT0814_stCN)); 

    hOutData     = hUserData;

    stCN.usPopt              =  pContext->stRedir.usPopt;            
    stCN.ulTSDUmaxto         =  NULL; *  Unlimited TSDU size    *       
    stCN.ulTSDUmaxfrom       =  NULL; *  pContext->ulTidusize;  *      
    stCN.usVersion           =  pContext->stRedir.usProtvers;        
    stCN.usFunctionalUnits   =  pContext->stRedir.usFunctionalUnits; 
    stCN.sCallingLen         =  pContext->stRedir.sCallingAddrsLen;
    ro0041_memcpy((char FAR *)stCN.vCalling ,
                  (char FAR *)pContext->stRedir.vCallingAddrs,
                  (unsigned long int)stCN.sCallingLen);

    stCN.usOverflow          =  RC087d_NoOverflow; 

    * In redirect called address remains same.    *

    stCN.sCalledLen          =  pContext->sCalledAddrsLen;  
    ro0041_memcpy((char FAR *)stCN.vCalled ,
                  (char FAR *)pContext->vCalledAddrs ,
                  (unsigned long int)pContext->sCalledAddrsLen );

    if(ro0074_storeactive(hOutData , 0L) != OK )             
      RM0003_RETURN(functName, signed short int, -R_FSTERR ) 
                                                             
    lStorelen = ro0062_storelength(hOutData);                
    if(ro0063_sterror(hOutData) != OK )                      
      RM0003_RETURN(functName, signed short int, -R_FSTERR)  
                                                             
    stCN.lDatalen = lStorelen - ulOffsetorPID;                     

    if ((pOutData = ro0069_stputs(hOutData,(long)ulOffsetorPID) ) == NULL )  
              RM0003_RETURN(functName, signed short int, -R_FSTERR)   
    stCN.pDataptr = pOutData + ulOffsetorPID;                         

    ro0814_EncodeCN((rT0814_stCN FAR *)&stCN);                                         

    ulParam1 = hOutData;                                              
    ulParam2 = (unsigned long int)(stCN.pDataptr - pOutData);         

    pContext->usUserConnExists = FALSE;
    pContext->usState =  RC08d0_STA01;
                                                                    
    if(ro0073_storeidle(hOutData) != OK)                              
           RM0003_RETURN(functName, signed short int, -R_FSTERR )        
                                                                    
*********** end of comment by kc on 7/7/93 */
   ulParam1 =  pContext->stRedir.hsStore;
   ulParam2 =  pContext->stRedir.ulOffset;
   pContext->stRedir.hsStore  = 
   pContext->stRedir.ulOffset = LNULL;
   sRetCode = ro0211_pushOutEvent (RC0213_Transport,
                                   RC090a_TREDIRECTreq,
                                   ulParam1,
                                   ulParam2,
                                   NULL,
                                   ulOffsetorPID);
  }
  else
  {   
 /******************************************************************
  # Normal behaviour of Connect Resp. Negative .......
 ******************************************************************/
  
    ro082B_InitStruc(&stRF,sizeof(rT0819_stRF));

    hOutData     = hUserData;

    if ((pConnRspAcpm = 
       (rTro7g_stAssocRspAcpm FAR *)ro0031_locallock (hConnRspAcpm))
         == NULL)
       RM0003_RETURN(functName, signed short int, - R_LLOCKE)
  
    pContext->usFunctionalUnits =  
          (unsigned short int)pConnRspAcpm->stSpm.ulRequirements;
    pContext->usProtvers        =  pConnRspAcpm->stAll.sSessProtVers;
    pContext->ulConectresult    =  ulConResult;

    /********************************************************
    Note :- pConnRspAcpm->stAll.hRespondAddress if (! NULL) 
            free the memory handle.
    *********************************************************/ 

    if (pConnRspAcpm->stAll.hRespondAddress)
    {  if(ro0034_localfree( pConnRspAcpm->stAll.hRespondAddress ) != NULL)
           RM0003_RETURN(functName, signed short int, -R_LFREEE)
    } 
    if(ro0032_localunlock(hConnRspAcpm) != NULL)
           RM0003_RETURN(functName, signed short int, -R_LULOKE)
    if(ro0034_localfree(hConnRspAcpm) != NULL)
           RM0003_RETURN(functName, signed short int, -R_LFREEE)

    stRF.usFunctionalUnits = RC08ca_FUValue;   
    stRF.usTdiscAreuse     = RC0871_NotReuse;
    stRF.usVersion         = pContext->usProtvers;
    stRF.ulEnclosure       = RC0876_OneSPDU;                         
    if( pContext->ulConectresult == RCro81_UserReject)
        stRF.usReason      = RC08c0_CalledSSUserRej;
    else 
        stRF.usReason      = NULL;
   
    if (hOutData == NULL)    /* No User Data        */  
    {
       hOutData = ro0060_newstore(ST_USEMAXBUF,ST_CLOSED,
				  (signed short int FAR *)&sRetCode);
       if(sRetCode != OK)
            RM0003_RETURN(functName, signed short int, -R_FSTERR)

       ulOffsetorPID = RC0831_MaxParamLen; 

       lStorelen = ro0062_storelength(hOutData); 
       if (ro0063_sterror(hOutData) != OK )
            RM0003_RETURN(functName, signed short int, -R_FSTERR)

       stRF.lDatalen = NULL;
     }
     else                      /* User Data Available  */
     {
       if (ro0074_storeactive(hOutData , 0L) != OK ) 
         RM0003_RETURN(functName, signed short int, -R_FSTERR )

       lStorelen = ro0062_storelength(hOutData); 
       if (ro0063_sterror(hOutData) != OK )
           RM0003_RETURN(functName, signed short int, -R_FSTERR)
       stRF.lDatalen = lStorelen - ulOffsetorPID; 
/* by kc on 12-10-93 */
/*   if(lStorelen > RC0835_MaxConnectLen) by kc 29-07-94 K02    
     for the Conformance test     */                            
  if (stRF.lDatalen > RC0835_MaxConnectLen)                     
  {
    RM0003_RETURN(functName, signed short int, 
        ro0831_InvalidSpdu(pContext,hOutData,RC0831_MaxParamLen,NULL,NULL))
     
  } 
    }

    if ((pOutData = ro0069_stputs(hOutData,(long)ulOffsetorPID)) == NULL)
           RM0003_RETURN(functName, signed short int, -R_FSTERR)
    stRF.pDataptr = pOutData + ulOffsetorPID;

    ro0819_EncodeRF((rT0819_stRF FAR *)&stRF);
    
    ulParam1 = hOutData;
    ulParam2 = (unsigned long int )(stRF.pDataptr - pOutData );
     
    pContext->usState   = RC08db_STA16;   /* ie. Wait for TCONind */

    if(ro0073_storeidle(hOutData) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR )
 
    sRetCode = ro0211_pushOutEvent (RC0213_Transport,
                                    RC0907_TDATAreq,
                                    ulParam1,
                                    ulParam2,
                                    NULL,
                                    NULL);
  
    /* NOTE :- After sending RF SPDU , make pContext->usUserConnExists = TRUE 
               to avoid TCONTINUE comming from Transport.
    */
    pContext->usUserConnExists = FALSE;  
  }/* not redirect */
   
 RM0003_RETURN(functName, signed short int, sRetCode)
}/*end-ro0864_STA08xSCONrspNeg*/
/* EOF  */
