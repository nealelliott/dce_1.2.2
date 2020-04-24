/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm07.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:27  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:02  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:55  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:21  keutel
 * 	OT11620
 * 	[1994/08/19  13:57:55  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:16  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:32  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:08:46  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:11  keutel
 * 	creation
 * 	[1994/03/21  13:37:30  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rospm07.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:27 $" } ;
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
/* NAME         : rospm07.c                                           */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-12                                            */
/*                                                                    */
/* COMPONENT    : RTROS  Convergence protocol machine.                */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : This module provides functions which are            */
/*                triggered in states                                 */
/*                STA09 : Wait for SRELEASE responce                  */
/*               STA713 : Data transfer                               */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-04-12|  original                      |ma  |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "@(#) rospm07.c 1.1 93/11/29 RTROS-V3.00";

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
/* NAME         : ro0870_STA09xSRELreq                                */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
signed short int NEAR PASCAL ro0870_STA09xSRELreq (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)
/*                                                                    */
/* DESCRIPTION  :      predicates:   -------                          */
/*                     actions   :   [8][18]                          */
/*                     next event:   FN-nr                            */
/*                     next state:   RC08da_STA09                     */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hP3       = Third parameter of transaction functon.               */
/*  hP4       = Fourth parameter of transaction function.             */
/*                                                                    */
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
{/*entr-ro0870_STA09xSRELreq*/

unsigned long  int         hOutData  = LNULL;  
char                FAR  * pOutData  = NULL;
rT0817_stFN                stFN;
unsigned long  int         ulParam1  = LNULL;
unsigned long  int         ulParam2  = LNULL;
signed   long  int         lStorelen = LNULL;

signed   short int         sRetCode  = OK;

   RM0001_STRG(functName,"ro0870_STA09xSRELreq")
   RM0002_ENTRY(functName)

/* ************************ S T A R T ******************************* */
/* ************************           ******************************* */

  ro082B_InitStruc((rT0817_stFN FAR *)&stFN , sizeof(rT0817_stFN));

  hOutData = hUserData;

  stFN.usFunctionalUnits = RC08ca_FUValue;

  if (hOutData == NULL)        /* No User Data   */
  {
    hOutData = ro0060_newstore(ST_USEMAXBUF,ST_CLOSED,
				(signed short int FAR *)&sRetCode);
    if(sRetCode != OK)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
 
    ulOffset = RC0831_MaxParamLen;
    
    lStorelen = ro0062_storelength(hOutData); 
    if (ro0063_sterror(hOutData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)

    stFN.lDatalen = NULL; 
  }
  else                        /* User Data Available  */
  {
    if (ro0074_storeactive(hOutData , 0L) != OK ) 
         RM0003_RETURN(functName, signed short int, -R_FSTERR )
  
    lStorelen = ro0062_storelength(hOutData); 
    if (ro0063_sterror(hOutData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)
    stFN.lDatalen = lStorelen - ulOffset;
/* by kc on 12-10-93 */    
/*   if(lStorelen > RC0835_MaxConnectLen) by kc 29-07-94 K02    
     for the Conformance test     */                            
  if (stFN.lDatalen > RC0835_MaxConnectLen)                                                                        
  {                                                                           
    RM0003_RETURN(functName, signed short int,                                
        ro0831_InvalidSpdu(pContext,hOutData,RC0831_MaxParamLen,NULL,NULL))   
                                                                              
  }                                                                           
  }
     
  if ((pOutData = ro0069_stputs(hOutData,(long)ulOffset)) == NULL)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)

  stFN.pDataptr          = pOutData + ulOffset; 
  stFN.usTdiscAreuse     = RC0871_NotReuse;
  stFN.usTdiscUserAbort  = RC0856_UserAbort;
  stFN.ulEnclosure       = RC0876_OneSPDU;   

  ro0817_EncodeFN ((rT0817_stFN FAR *)&stFN);
  
   ulParam1 = hOutData;
   ulParam2 = (unsigned long int)(stFN.pDataptr - pOutData);

   if(ro0073_storeidle(hOutData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

   pContext->usVtrr  = FALSE;             /* NOT_REUSE            */
   pContext->usVcoll = TRUE;              /* COLL_FN              */
   pContext->usState = RC08da_STA09;      /* ie. Wait for SRELrsp */
      
RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent(RC0213_Transport,
                                                               RC0907_TDATAreq,
                                                               ulParam1,
                                                               ulParam2,
                                                               NULL,
                                                               NULL))

}/*end-ro0870_STA09xSRELreq*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0871_STA09xSRELrspPos                             */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
signed short int NEAR PASCAL ro0871_STA09xSRELrspPos (  
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)
/*                                                                    */
/* DESCRIPTION  :      predicates:   ^p66 &p75                        */
/*                     actions   :   [4]                              */
/*                     next event:   DN                               */
/*                     next state:   RC08db_STA16                     */
/*                                                                    */
/*                     predicates:    p66                             */
/*                     actions   :   ---------                        */
/*                     next event:   DN                               */
/*                     next state:   RC08d3_STA01C                    */
/*                                                                    */
/*                     predicates:   p69 & p01                        */
/*                     actions   :   ---------                        */
/*                     next event:   DN                               */
/*                     next state:   RC08d8_STA03                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hP3       = Third parameter of transaction functon.               */
/*  hP4       = Fourth parameter of transaction function.             */
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
{/*entr-ro0871_STA09xSRELrspPos*/

unsigned long  int         hOutData  = LNULL;  
char                FAR  * pOutData  = NULL;

rT0815_stDN                stDN;
signed   long  int         lStorelen = LNULL;
unsigned long  int         ulParam1  = LNULL;
unsigned long  int         ulParam2  = LNULL;
signed   short int         sRetCode  = OK;

  RM0001_STRG(functName,"ro0871_STA09xSRELrspPos")
  RM0002_ENTRY(functName)

/* ************************* S T A R T ****************************** */
/* *************************           ****************************** */
  
  ro082B_InitStruc((rT0815_stDN FAR *)&stDN , sizeof(rT0815_stDN));

  hOutData = hUserData;
  
  stDN.usFunctionalUnits = RC08ca_FUValue;
 
  if (hOutData == NULL)      /* User Data not available  */
  {
    hOutData = ro0060_newstore(ST_USEMAXBUF,ST_CLOSED,
				(signed short int FAR *)&sRetCode);
    if(sRetCode != OK)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)

    ulOffset = RC0831_MaxParamLen;
    stDN.lDatalen = NULL; 
  }
  else
  {
    if (ro0074_storeactive(hOutData , 0L) != OK ) 
         RM0003_RETURN(functName, signed short int, -R_FSTERR )

    lStorelen   = ro0062_storelength(hOutData); 
    if (ro0063_sterror(hOutData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)
    stDN.lDatalen = lStorelen - ulOffset;

/* by kc on 12-10-93 */                                                       
/*   if(lStorelen > RC0835_MaxConnectLen) by kc 29-07-94 K02    
     for the Conformance test     */                            
  if (stDN.lDatalen > RC0835_MaxConnectLen)                     
  {                                                                           
    pContext->usState = RC08d8_STA03;          /* ie. Wait for DN SPDU     */
    RM0003_RETURN(functName, signed short int,                                
        ro0831_InvalidSpdu(pContext,hOutData,RC0831_MaxParamLen,NULL,NULL))   
                                                                              
  }                                                                           
  }
  if ((pOutData = ro0069_stputs(hOutData,(long)ulOffset)) == NULL)
         RM0003_RETURN(functName, signed short int, -R_FSTERR)

  stDN.pDataptr     = pOutData + ulOffset;
  stDN.ulEnclosure  = RC0876_OneSPDU;   

  ro0815_EncodeDN ((rT0815_stDN FAR *)&stDN);
  
  ulParam1 = hOutData;
  ulParam2 = (unsigned long int)(stDN.pDataptr - pOutData);

  if(ro0073_storeidle(hOutData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )
      
  if((pContext->usVcoll) && (pContext->usVtca))  /*  SCONREQ && NO_coolision */
      pContext->usState = RC08d8_STA03;          /* ie. Wait for DN SPDU     */
  else
  {
    if( (!pContext->usVtrr) &&
        ( ((pContext->usVcoll) && (pContext->usVdnr)) || (!pContext->usVcoll) )
      )       /*  COLLISION && DISCON_SPDU) ||  not REUSE  */
     {
       /* Start timer not implemented  */
        pContext->usState = RC08db_STA16; /* ie. Wait for TCONind */
      }
      else
        RM0003_RETURN(functName, signed short int, -R_SINTER)
        /* PROBLEM : of freeing the p1 to p4 */
  }
  
RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent(RC0213_Transport,
                                                               RC0907_TDATAreq,
                                                               ulParam1,
                                                               ulParam2,
                                                               NULL,
                                                               NULL))

}/*end-ro0871_STA09xSRELrspPos*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0872_STA09xDN                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
signed short int NEAR PASCAL ro0872_STA09xDN (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   p69 & 1p01                       */
/*                     actions   :   [32]                             */
/*                     next event:   SRELcnf                          */
/*                     next state:   RC08da_STA09                     */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hP3       = Third parameter of transaction functon.               */
/*  hP4       = Fourth parameter of transaction function.             */
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
{/*entr-ro0872_STA09xDN*/

unsigned long  int         hInData   = LNULL;  
char                FAR  * pInData   = NULL;

rT0815_stDN stDN;
signed   long  int         lStorelen = LNULL;
signed   long  int         lBuflen   = LNULL;
unsigned long  int         ulParam1  = LNULL;
unsigned long  int         ulParam2  = LNULL;
unsigned long  int         ulParam4  = LNULL;

  RM0001_STRG(functName,"ro0872_STA09xDN")
  RM0002_ENTRY(functName)

/* *********************** S T A R T ******************************** */
/* ***********************           ******************************** */

  ro082B_InitStruc((rT0815_stDN FAR *)&stDN , sizeof(rT0815_stDN));

  if ((pContext->usVcoll) && (pContext->usVtca))
  {
    hInData = hUserData;

    if (ro0074_storeactive(hInData , 0L) != OK ) 
         RM0003_RETURN(functName, signed short int, -R_FSTERR )

    lBuflen = ro080a_StoreOffset(0L,hInData,ulOffset);
    if( ! lBuflen)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
    if ((pInData = ro0072_stgets(hInData,lBuflen)) == NULL)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)

    stDN.pDataptr               = pInData + ulOffset;
    stDN.usFunctionalUnits      = RC08ca_FUValue; 

    lStorelen   = ro0062_storelength(hInData); 
    if (ro0063_sterror(hInData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)
    stDN.lDatalen = lStorelen - ulOffset;
  
    if ( ro0825_DecodeDN((rT0815_stDN FAR *)&stDN) != 0)
    {
      if(ro0073_storeidle(hInData) != OK)
            RM0003_RETURN(functName, signed short int, -R_FSTERR)
      RM0003_RETURN(functName, signed short int,
           ro0831_InvalidSpdu(pContext,hInData,RC0831_MaxParamLen,NULL,NULL))
    }
  
    if (stDN.lDatalen == NULL)     /* User Data not Available  */
    {
      ulParam1 = NULL;
      ulParam2 = NULL;

      if(ro0073_storeidle(hInData) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR )

      if(ro0075_deletestore(hInData) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR )
    }
    else                       /* User Data Available     */
    {
      ulParam1 = hInData;
      ulParam2 = (unsigned long int)(stDN.pDataptr - pInData);

      if(ro0073_storeidle(hInData) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR )
    }

    ulParam4 = 1;                   

    pContext->usState = RC08d3_STA01C;  /* ie. Idle, Transport connected */
    pContext->usVdnr  = TRUE;           /* DISCON_SPDU                   */
  
    RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent (
                                                           RC0215_Presentation,
                                                           RC0808_SRELEASEcnf,
                                                           ulParam1,
                                                           ulParam2,
                                                           NULL,
                                                           ulParam4))
  }
  else
    RM0003_RETURN(functName, signed short int, -R_SINTER)

}/*end-ro0872_STA09xDN*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0873_STA09xSDATAreq                               */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */

signed short int NEAR PASCAL ro0873_STA09xSDATAreq ( 
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   p04                              */
/*                     actions   :   -------                          */
/*                     next event:   DT                               */
/*                     next state:   RC08da_STA09                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hP3       = Third parameter of transaction functon.               */
/*  hP4       = Fourth parameter of transaction function.             */
/*                                                                    */
/* FORMAL OUT-PARAMETERS       :                                      */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
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
{/*entr-ro0873_STA09xSDATAreq*/

unsigned long  int         hInData   = LNULL;  
char                FAR  * pInData   = NULL;

rT0816_stDT                stDT;
unsigned long  int         ulParam1  = LNULL;
unsigned long  int         ulParam2  = LNULL;
signed   long  int         lStorelen = LNULL;

  RM0001_STRG(functName,"ro0873_STA09xSDATAreq")
  RM0002_ENTRY(functName)

/* *********************** S T A R T ******************************** */
/* ***********************           ******************************** */

   ro082B_InitStruc((rT0816_stDT FAR *)&stDT,sizeof(rT0816_stDT));

   /* FULL DUPLEX && NO_COLISION */
  if ((pContext->usFunctionalUnits & RCro53_FullDuplex) &&
      (pContext->usVcoll == FALSE))
  {
      hInData = hUserData;

      if (ro0074_storeactive(hInData , 0L) != OK ) 
            RM0003_RETURN(functName, signed short int, -R_FSTERR )

      if ((pInData = ro0069_stputs(hInData,(long)ulOffset)) == NULL)
           RM0003_RETURN(functName, signed short int, -R_FSTERR)
      stDT.pDataptr = pInData + ulOffset;

      lStorelen   = ro0062_storelength(hInData); 
      if (ro0063_sterror(hInData) != OK)
            RM0003_RETURN(functName, signed short int, -R_FSTERR)
      stDT.lDatalen = (long int)(lStorelen - ulOffset); 

      ro0816_EncodeDT((rT0816_stDT FAR *)&stDT);
  
      ulParam1 = hInData;
      ulParam2 = (unsigned long int)(stDT.pDataptr - pInData);

      if(ro0073_storeidle(hInData) != OK)
           RM0003_RETURN(functName, signed short int, -R_FSTERR )
      
      RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent (
                                                             RC0213_Transport,
                                                             RC0907_TDATAreq,
                                                             ulParam1,
                                                             ulParam2,
                                                             NULL,
                                                             NULL))
  }
  else
      RM0003_RETURN(functName, signed short int, -R_SINTER)

}/*end-ro0873_STA09xSDATAreq*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0874_STA713xSRELreq                               */
/*								      */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */

signed short int NEAR PASCAL ro0874_STA713xSRELreq (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   ^p64                             */
/*                     actions   :   [8]                              */
/*                     next event:   FN-nr                            */
/*                     next state:   RC08d8_STA03                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hP3       = Third parameter of transaction functon.               */
/*  hP4       = Fourth parameter of transaction function.             */
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
{/*entr-ro0874_STA713xSRELreq*/

unsigned long  int         hOutData  = LNULL;  
char                FAR  * pOutData  = NULL;

rT0817_stFN                stFN;
signed   long  int         lStorelen = LNULL;
unsigned long  int         ulParam1  = LNULL;
unsigned long  int         ulParam2  = LNULL;
signed   short int         sRetCode  = OK;

  RM0001_STRG(functName,"ro0874_STA713xSRELreq")  
  RM0002_ENTRY(functName)

/* ********************** S T A R T ********************************* */
/* **********************           ********************************* */

  ro082B_InitStruc((rT0817_stFN FAR *)&stFN , sizeof(rT0817_stFN));

  if ((pContext->usTexp == FALSE) && (pContext->usVtca == FALSE))
  {   /*  Texpedited Service    &&     SCONREQ */

    stFN.usFunctionalUnits = RC08ca_FUValue; 
    hOutData = hUserData;
  
    if (hOutData == NULL)   /* User Data Not Available    */
    {
      hOutData = ro0060_newstore(ST_USEMAXBUF,ST_CLOSED,
				 (signed short int FAR *)&sRetCode);
      if(sRetCode != OK)
           RM0003_RETURN(functName, signed short int, -R_FSTERR)

      ulOffset = RC0831_MaxParamLen;  
      stFN.lDatalen = NULL; 
    }
    else                    /* User Data Available        */
    {
      if (ro0074_storeactive(hOutData , 0L) != OK ) 
          RM0003_RETURN(functName, signed short int, -R_FSTERR )

      lStorelen   = ro0062_storelength(hOutData); 
      if (ro0063_sterror(hOutData) != OK )
          RM0003_RETURN(functName, signed short int, -R_FSTERR)
      stFN.lDatalen = lStorelen - ulOffset; 
/*   if(lStorelen > RC0835_MaxConnectLen) by kc 29-07-94 K02    
     for the Conformance test     */                            
  if (stFN.lDatalen > RC0835_MaxConnectLen)                     
  {                                                                           
    RM0003_RETURN(functName, signed short int,                                
        ro0831_InvalidSpdu(pContext,hOutData,RC0831_MaxParamLen,NULL,NULL))   
                                                                              
  }                                                                           
    }

    if ((pOutData = ro0069_stputs(hOutData,(long)ulOffset)) == NULL)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
  
    stFN.pDataptr          = pOutData + ulOffset;
    stFN.usTdiscAreuse     = RC0871_NotReuse;
    stFN.usTdiscUserAbort  = RC0856_UserAbort;
    stFN.ulEnclosure       = RC0876_OneSPDU;

    ro0817_EncodeFN((rT0817_stFN FAR *)&stFN);
    
    ulParam1 = hOutData;
    ulParam2 = (unsigned long int)(stFN.pDataptr - pOutData);

    if(ro0073_storeidle(hOutData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

    pContext->usVtrr  = FALSE;
    pContext->usState = RC08d8_STA03;         /* ie. Wait for DN SPDU */
        
    RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent (
                                                             RC0213_Transport,
                                                             RC0907_TDATAreq,
                                                             ulParam1,
                                                             ulParam2,
                                                             NULL,  
                                                             NULL))
  }
  else
    RM0003_RETURN(functName, signed short int, -R_SINTER)

}/*end-ro0874_STA713xSRELreq*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0875_STA713xFN                                    */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */

signed short int NEAR PASCAL ro0875_STA713xFN (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   p68                              */
/*                     actions   :   [8]                              */
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
/*  hP3       = Third parameter of transaction functon.               */
/*  hP4       = Fourth parameter of transaction function.             */
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
{/*entr-ro0875_STA713xFN*/

unsigned long  int         hInData   = LNULL;  
char                FAR  * pInData   = NULL;

rT0817_stFN                stFN;
signed   long  int         lStorelen = LNULL;
signed   long  int         lBuflen   = LNULL;

unsigned long int          ulParam1  = LNULL;
unsigned long int          ulParam2  = LNULL;

  RM0001_STRG(functName,"ro0875_STA713xFN")
  RM0002_ENTRY(functName)

/* ************************ S T A R T ****************************** */
/* ************************           ****************************** */

  ro082B_InitStruc((rT0817_stFN FAR *)&stFN , sizeof(rT0817_stFN));

  hInData = hUserData;

  if (ro0074_storeactive(hInData , 0L) != OK ) 
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

  lBuflen = ro080a_StoreOffset(0L,hInData,ulOffset);
  if( ! lBuflen)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
  if ((pInData = ro0072_stgets(hInData,lBuflen)) == NULL)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)

  stFN.usFunctionalUnits    = RC08ca_FUValue;
  stFN.pDataptr             = pInData + ulOffset;

  lStorelen   = ro0062_storelength(hInData); 
  if (ro0063_sterror(hInData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)
  stFN.lDatalen = lStorelen - ulOffset; 

  if ( ro0827_DecodeFN((rT0817_stFN FAR *)&stFN) != 0)
  { 
    if(ro0073_storeidle(hInData) != OK)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
    RM0003_RETURN(functName, signed short int, 
       ro0831_InvalidSpdu(pContext,hInData,RC0831_MaxParamLen,NULL,NULL))
  }
 
  pContext->usAreuse = stFN.usTdiscAreuse;
  pContext->usUabort = stFN.usTdiscUserAbort;

  if (stFN.lDatalen == 0)     /*  User Data Not Available */ 
  {
    ulParam1 = NULL;
    ulParam2 = NULL;

    if(ro0073_storeidle(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

    if(ro0075_deletestore(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

  }
  else        
  {
    ulParam1 = hInData;
    ulParam2 = (unsigned long int)(stFN.pDataptr - pInData );

    if(ro0073_storeidle(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )
  }

  pContext->usState = RC08da_STA09;          /* Wait for SRELrsp   */  
  pContext->usVtrr  = FALSE;                 /* NOT_REUSE          */

RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent (
                                                            RC0215_Presentation,
                                                            RC0806_SRELEASEind,
                                                            ulParam1,
                                                            ulParam2,
                                                            NULL,
                                                            NULL))
}/*end-ro0875_STA713xFN*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0876_STA713xSDATAreq                              */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */

signed short int NEAR PASCAL ro0876_STA713xSDATAreq (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   ^TREADY                          */
/*                     actions   :   data save                        */
/*                     next event:   -------                          */
/*                     next state:   -------                          */
/*                                                                    */
/*                     predicates:   TREADY                           */
/*                     actions   :   ^TReADY                          */
/*                     next event:   dt                               */
/*                     next state:   -------                          */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hP3       = Third parameter of transaction functon.               */
/*  hP4       = Fourth parameter of transaction function.             */
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
{/*entr-ro0876_STA713xSDATAreq*/

unsigned long  int         hInData   = LNULL;  
char                FAR  * pInData   = NULL;

rT0816_stDT                stDT;
unsigned long  int         ulParam1  = LNULL;
unsigned long  int         ulParam2  = LNULL;
signed   long  int         lStorelen = LNULL;

  RM0001_STRG(functName,"ro0876_STA713xSDATAreq")
  RM0002_ENTRY(functName)

/* ************************ S T A R T ****************************** */
/* ************************           ****************************** */

    ro082B_InitStruc((rT0816_stDT FAR *)&stDT , sizeof(rT0816_stDT));

    hInData = hUserData;

    stDT.usFunctionalUnits  = RC08ca_FUValue;

    if (ro0074_storeactive(hInData , 0L) != OK ) 
        RM0003_RETURN(functName, signed short int, -R_FSTERR )

    if ((pInData = ro0069_stputs(hInData,(long)ulOffset)) == NULL)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
      
    stDT.pDataptr = pInData + ulOffset;
    
    lStorelen   = ro0062_storelength(hInData); 
    if (ro0063_sterror(hInData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)
    stDT.lDatalen = lStorelen - ulOffset;

    ro0816_EncodeDT((rT0816_stDT FAR *)&stDT);
  
    ulParam1 = hInData;
    ulParam2 =(unsigned long int)(stDT.pDataptr - pInData);

    if(ro0073_storeidle(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )
    
RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent(RC0213_Transport,
                                                               RC0907_TDATAreq,
                                                               ulParam1,
                                                               ulParam2,
                                                               NULL,
                                                               NULL))
}/*end-ro0876_STA713xSDATAreq*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0877_STA713xDT                                    */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */

signed short int NEAR PASCAL ro0877_STA713xDT (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   p05                              */
/*                     actions   :   -------                          */
/*                     next event:   SDATind                          */
/*                     next state:   RC08dc_STA713                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hP3       = Third parameter of transaction functon.               */
/*  hP4       = Fourth parameter of transaction function.             */
/*                                                                    */
/* FORMAL OUT-PARAMETERS       :                                      */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
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
{/*entr-ro0877_STA713xDT*/

unsigned long  int         hInData   = LNULL;
char                FAR  * pInData   = NULL;

rT0816_stDT                stDT;
unsigned long  int         ulParam1  = LNULL;
unsigned long  int         ulParam2  = LNULL;
signed   long  int         lStorelen = LNULL;
signed   long  int         lBuflen   = LNULL;

  RM0001_STRG(functName,"ro0877_STA713xDT")
  RM0002_ENTRY(functName)

/* *********************** ro0877_STA713xDT() ***************************** */
/* ***********************             ***************************** */

  ro082B_InitStruc((rT0816_stDT FAR *)&stDT , sizeof(rT0816_stDT));

  hInData = hUserData;

  if (ro0074_storeactive(hInData , 0L) != OK ) 
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

  lBuflen = ro080a_StoreOffset(0L,hInData,ulOffset);
  if( ! lBuflen)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
  if ((pInData = ro0072_stgets(hInData,lBuflen)) == NULL)
       RM0003_RETURN(functName, signed short int, -R_FSTERR)

  stDT.usFunctionalUnits = RC08ca_FUValue;

  stDT.pDataptr = pInData + ulOffset;
  lStorelen = ro0062_storelength(hInData); 
  if (ro0063_sterror(hInData) != OK )
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
}/*end-ro0877_STA713xDT*/

/*EOF*/
