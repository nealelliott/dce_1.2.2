/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm05.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:24  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:50  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:10  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:28  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:08:26  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:06  keutel
 * 	creation
 * 	[1994/03/21  13:37:09  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rospm05.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:24 $" } ;
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
/* NAME         : rospm05.c                                           */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 29-03-93                                            */
/*                                                                    */
/* COMPONENT    : RTROS Convergence protocol machine                  */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Contains the functions related to the phase :       */
/*                " Connection establishment "                        */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-03-29|  original                      |ma  |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "@(#) rospm05.c 1.1 93/11/29 RTROS-V3.00";

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
/* NAME         : ro0850_STA02xRF                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-29                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0850_STA02xRF (
rT0870_stSpmContext    *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4          )

/*                                                                    */
/* DESCRIPTION  :      predicates:   ---------                        */
/*                     actions   :    [3]                             */
/*                     next event:   T_DISCONNECTreqest               */
/*                     next state:   RC08d0_STA01                     */
/*                                                                    */
/*                     predicates:   ---------                        */
/*                     actions   :   ---------                        */
/*                     next event:   S_CONNECT.indication             */
/*                     next state:   RC08d0_STA01                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS          :                                           */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hP3       = Third parameter of transaction functon.               */
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
{/*entr-ro0850_STA02xRF*/

unsigned long  int            hOutData      = LNULL;
char                    FAR * pOutData      = NULL;

/* old implementation of spm uses 
unsigned long  int            hConnCnfSpm   = LNULL;
rTro5g_stConnRspCnfSpm  FAR * pConnCnfSpm   = NULL; */

unsigned long  int            hAssocCnfAcpm = LNULL;
rTro7h_stAssocCnfAcpm   FAR * pAssocCnfAcpm = NULL;

unsigned long  int            ulRelResult   = LNULL;      

rT0819_stRF stRF;
unsigned long  int            ulParam1      = LNULL;
unsigned long  int            ulParam2      = LNULL;
unsigned long  int            ulParam3      = LNULL;
unsigned long  int            ulParam4      = LNULL;
signed   long  int            lStorelen     = LNULL;
signed   long  int            lBuflen       = LNULL;

signed   short int            sRetCode      = OK;

  RM0001_STRG(functName,"ro0850_STA02xRF")

/* ****************** S T A R T ************************************* */

  RM0002_ENTRY(functName)

/* !!!!!!!!!!!!!!!!!!   TIMER fehlt !!!!!!!!!!!!!!!!!!!!!!!! */

  lStorelen = ulParam1 = ulParam2 = ulParam3 = ulParam4 = LNULL;
  ulRelResult = LNULL;
  ro082B_InitStruc(&stRF,sizeof(rT0819_stRF));
 
  hOutData = hUserData;

/**********************************************************************/
/**********************************************************************/

  if (hOutData) {
    if(ro0074_storeactive(hOutData , 0L) != OK ) 
      RM0003_RETURN(functName, signed short int, -R_FSTERR )
  }

  lBuflen = ro080a_StoreOffset(0,hOutData,ulOffset);
  if( ! lBuflen)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
  if ((pOutData = ro0072_stgets(hOutData,lBuflen)) == NULL)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)

  stRF.usFunctionalUnits      = RC08ca_FUValue;
  stRF.pDataptr               = pOutData + ulOffset;

  lStorelen   = ro0062_storelength(hOutData); 
  if(ro0063_sterror(hOutData) != OK )
      RM0003_RETURN(functName, signed short int, -R_FSTERR)
  stRF.lDatalen = lStorelen - ulOffset;

  if ( ro0829_DecodeRF((rT0819_stRF FAR *)&stRF) != 0)
   {
     if(ro0073_storeidle(hOutData) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR)
     RM0003_RETURN(functName, signed short int,
           ro0831_InvalidSpdu(pContext,hOutData,RC0831_MaxParamLen,NULL,NULL))
   } 
/**********************************************************************/
/**********************************************************************/

  if ((hAssocCnfAcpm = ro0030_localalloc(sizeof(rTro7h_stAssocCnfAcpm)))
        == NULL)
       RM0003_RETURN(functName, signed short int, - R_LALLCE)

  if ((pAssocCnfAcpm = 
       (rTro7h_stAssocCnfAcpm FAR *)ro0031_locallock(hAssocCnfAcpm)) == NULL)
       RM0003_RETURN(functName, signed short int, - R_LLOCKE)
  
  pAssocCnfAcpm->stAll.hRespondAddress = pContext->hAddrs;
  pAssocCnfAcpm->stAll.sSessProtVers   = stRF.usVersion;
  pAssocCnfAcpm->stSpm.ulRequirements  = stRF.usFunctionalUnits;

  if ((stRF.usReason == 0) ||(stRF.usReason == 2)) 
    ulRelResult = RCro81_UserReject;
  else 
    ulRelResult = MAKELONG(stRF.usReason | 0x0500, 0x0000);
  
  if ( ro0032_localunlock(hAssocCnfAcpm) != NULL)
    RM0003_RETURN(functName, signed short int, - R_LULOKE)

  /*********************
   Note :- pcontext->hAddrs handle sshould not be freed here , as it 
           is passed to pAssocCnfAcpm->stAll.hRespondAddress.
   ********************/

  pContext->hAddrs = NULL;  /* handle was given away */

/**********************************************************************/
/**********************************************************************/
  
  if (stRF.lDatalen != NULL)   /* User Data Available */
  {
     ulParam1 = hOutData;
     ulParam2 = (unsigned long int)( stRF.pDataptr - pOutData);
     ulParam3 = hAssocCnfAcpm; 
     ulParam4 = ulRelResult; 
     if(ro0073_storeidle(hOutData) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR )
  }
  else                       /* User Data Not Available */
  {
     ulParam1 = NULL;
     ulParam2 = NULL;
     ulParam3 = hAssocCnfAcpm;
     ulParam4 = ulRelResult;

    if(ro0073_storeidle(hOutData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

    if(ro0075_deletestore(hOutData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

  }
  
   pContext->usState     = RC08d0_STA01; /* ie. Idle,no transport connection */ 
/* pContext->usUserConnExists = FALSE; */

  sRetCode = ro0211_pushOutEvent (RC0215_Presentation,
                                  RC0804_SCONNECTcnf,
                                  ulParam1,
                                  ulParam2,
                                  ulParam3,
                                  ulParam4);
  if (sRetCode != OK)
      RM0003_RETURN(functName, signed short int, sRetCode )

/**********************************************************************/
/**********************************************************************/


  sRetCode = ro0211_pushOutEvent (RC0213_Transport,
                                  RC0905_TDISCONNECTreq,
                                  NULL,
                                  NULL,
                                  2,        
                                  NULL);
      

/**********************************************************************/
/**********************************************************************/
  
RM0003_RETURN(functName, signed short int, sRetCode)
}/*ro0850_STA02xRF*/




/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0851_STA02AxAC                                    */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-29                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
signed short int NEAR PASCAL ro0851_STA02AxAC (
rT0870_stSpmContext *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  :      predicates:   ------------                     */
/*                     actions   :   [5][6]                           */
/*                     next event:   S_CONNECT.confirmation           */
/*                     next state:   RC08dc_STA713                    */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS           :                                          */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*  hUserData = Handle to store.                                      */
/*  ulOffset  = Offset in store from where encoding/decoding          */
/*              should start.                                         */
/*  hP3       = Third parameter of transaction functon.               */
/*  hP4       = Fourth parameter of transaction function.             */
/*                                                                    */
/* FORMAL OUT-PARAMETERS   :                                          */
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
{/*entr-ro0851_STA02AxAC*/

unsigned long  int            hInData      = LNULL;
char                    FAR * pInData      = NULL;

rT0812_stAC                   stAC;
signed   long  int            lStorelen    = LNULL;
unsigned long  int            ulParam1     = LNULL;
unsigned long  int            ulParam2     = LNULL;
unsigned long  int            ulParam3     = LNULL;
unsigned long  int            ulParam4     = LNULL;

unsigned long  int            hConnCnfAcpm;
rTro7h_stAssocCnfAcpm   FAR * pConnCnfAcpm;
unsigned long  int            ulConResult  = LNULL;
signed   long  int            lBuflen      = LNULL;
signed   short int            sRetCode     = OK;

  RM0001_STRG(functName,"ro0851_STA02AxAC")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

  ro082B_InitStruc(&stAC,sizeof(rT0812_stAC));

  hInData = hUserData;

/**********************************************************************/
/**********************************************************************/

  if(ro0074_storeactive(hInData , 0L) != OK ) 
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

  lBuflen = ro080a_StoreOffset(0,hInData,ulOffset);
  if( ! lBuflen)
       RM0003_RETURN(functName, signed short int, -R_FSTERR)
  if ((pInData = ro0072_stgets(hInData,lBuflen)) == NULL)
       RM0003_RETURN(functName, signed short int, -R_FSTERR)

  stAC.usFunctionalUnits   = RC08ca_FUValue;
  stAC.pDataptr            = pInData + ulOffset;

  lStorelen = ro0062_storelength(hInData); 
  if(ro0063_sterror(hInData) != OK )
      RM0003_RETURN(functName, signed short int, -R_FSTERR)
  stAC.lDatalen = lStorelen - ulOffset;

  if ( ro0822_DecodeAC ((rT0812_stAC FAR *)&stAC) != 0)
  {  
    if(ro0073_storeidle(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR)
    RM0003_RETURN(functName, signed short int, 
          ro0831_InvalidSpdu(pContext,hInData,RC0831_MaxParamLen,NULL,NULL))
  }

/**********************************************************************/
/**********************************************************************/
 
  sRetCode = ro0218_conAddress (&pContext->hAddrs,
                                 RCro13_Sselector,
                                 stAC.sRespondingLen,
                                (char FAR *)stAC.ucResponding);
  if (sRetCode < OK)
      RM0003_RETURN(functName, signed short int, sRetCode)

  if (((stAC.usVersion & RCro50_ProtVers1) == RCro50_ProtVers1) ||
     ((stAC.usVersion & RCro51_ProtVers2)  == RCro51_ProtVers2))
      pContext->usProtvers = stAC.usVersion;
  else
  {  
    if (ro0073_storeidle(hInData) != OK)
        RM0003_RETURN(functName, signed short int, 
            ro0831_InvalidSpdu(pContext,hUserData,RC0831_MaxParamLen,NULL,NULL))
  }

  if ((pContext->ulTidusize < stAC.ulTSDUmaxfrom) || (stAC.ulTSDUmaxfrom == 0))
    stAC.ulTSDUmaxfrom = pContext->ulTidusize;
  else
    pContext->ulTidusize = stAC.ulTSDUmaxfrom;
  
  if ((pContext->ulTidusize < stAC.ulTSDUmaxto)  || (stAC.ulTSDUmaxto == 0))
    stAC.ulTSDUmaxto = pContext->ulTidusize;
  else
    pContext->ulTidusize = stAC.ulTSDUmaxto;

  if ((stAC.usFunctionalUnits & RCro53_FullDuplex) == RCro53_FullDuplex)
    pContext->usFunctionalUnits       = stAC.usFunctionalUnits;
  else
  {  
     if(ro0073_storeidle(hInData) != OK)
       RM0003_RETURN(functName, signed short int,
           ro0831_InvalidSpdu(pContext,hUserData,RC0831_MaxParamLen,NULL,NULL))
  }

/**********************************************************************/
/**********************************************************************/

/***************
Banned ......
#ifdef RC_ENCLOSURE
  if ((stAC.ulEnclosure == RC0876_OneSPDU) ||
      (stAC.ulEnclosure == RC0877_StartSPDU) ||
      (stAC.ulEnclosure == NULL))
#else
  if (first_call)
#endif
    {
Banned .....
**************/
    if ((hConnCnfAcpm = 
          ro0030_localalloc(sizeof(rTro7h_stAssocCnfAcpm))) == NULL)
      RM0003_RETURN(functName, signed short int, - R_LALLCE)

    if ((pConnCnfAcpm = 
           (rTro7h_stAssocCnfAcpm FAR *)ro0031_locallock(hConnCnfAcpm)) 
                     == NULL)
      RM0003_RETURN(functName, signed short int, - R_LLOCKE)

    pConnCnfAcpm->stAll.hRespondAddress = pContext->hAddrs;
    pConnCnfAcpm->stSpm.ulRequirements  = stAC.usFunctionalUnits;
    pConnCnfAcpm->stAll.sSessProtVers   = stAC.usVersion;
    ulConResult                         = RCro80_Accept; 	

    /************************************************
     Note :- 
            Handle is passed from pContext->hAddrs to 
            pConnCnfAcpm->stAll.hRespondAddress , 
            hence only make pContext->hAddrs = NULL
            without freeing memory. 
    *************************************************/
    pContext->hAddrs                   = NULL;

    if ( ro0032_localunlock(hConnCnfAcpm) != NULL)
      RM0003_RETURN(functName, signed short int, - R_LULOKE)

    ulParam3   = hConnCnfAcpm;
    ulParam4   = ulConResult;

/**********************************************************************/
/**********************************************************************/

  if (stAC.lDatalen != NULL)      /* User Data Available   */
  {
    ulParam1 = hInData;
    ulParam2 = (unsigned long int)(stAC.pDataptr - pInData);

    if(ro0073_storeidle(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )
  }
  else                         /* User Data not Available */
  {
    ulParam1 = NULL;
    ulParam2 = NULL;

    if(ro0073_storeidle(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

    if(ro0075_deletestore(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )
  }
/**********************************************************************/
/**********************************************************************/

/*****************
Banned ......
Note :  This banned code has to be modified correctly if to be used later....

#ifdef RC_ENCLOSURE
     if ((pstruc->ulEnclosure == RC0876_OneSPDU) || (pstruc->ulEnclosure == RC0877_StartSPDU))
        param3 = hrs0802_struc;
     else
       param3 = NULL;
     param4 = pstruc->ulEnclosure;
#else
  
     if (more_flag & RC0921_End)
     {

Banned ......
****************/

        pContext->usState = RC08dc_STA713;    /* ie. Data Transfer */
        pContext->usVact  = FALSE;            /*  no activity */
        pContext->usVcoll = FALSE;            /*  no collision */
        pContext->usVdnr  = FALSE;            /*  no DN SPDU   */
        pContext->usTexp  = FALSE;        /*  no transport expedited service  */

  sRetCode = ro0211_pushOutEvent (RC0215_Presentation,
                                  RC0804_SCONNECTcnf,
                                  ulParam1,
                                  ulParam2,
                                  ulParam3,
                                  ulParam4);
/**********************************************************************/
/**********************************************************************/

RM0003_RETURN(functName, signed short int, sRetCode)
}/*end-ro0851_STA02AxAC*/
/*  EOF  */
