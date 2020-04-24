/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spacpm1.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:11  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:37  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:47:31  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:49  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:44  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:45:18  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:33  keutel
 * 	creation
 * 	[1994/03/21  13:14:57  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spacpm1.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:25 $" } ;
#endif

/**********************************************************************/
/*                                                                    */
/*    COPYRIGHT  (C)						      */
/*    Siemens Nixdorf Informationssysteme AG 1993		      */
/*    All rights reserved					      */
/*								      */
/*    This software is furnished under licence and may be used only   */
/*    in accordance with the terms of that licence and with the       */
/*    inclusion of the above copyright notice.			      */
/*    This software may not be provided or otherwise made available   */
/*    to, or used by, any other person. No title to or ownership of   */
/*    the software is hereby transferred.			      */
/*                                                                    */
/**********************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME 	: spacpm1.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K                                      */
/* DATE 	: 25-11-93					      */
/*                                                                    */
/* COMPONENT    : RTROS Evaluation Program                            */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : In this module the the ACPM functions for decoding  */
/*                are copied                                          */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.00   K00| 25-11-93| original		       | kc |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spacpm1.c 1.1 93/12/20   RTS-V2.00";

/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>	  /* General RTS definitons		      */
#include <rolgval.h>	  /* General RTS definitons		      */
#include <roerval.h>	  /* General RTS definitons		      */
#include <rouply0.h>
#include <rouply1.h>      /* PM service interface structures          */
#include <routil0.h>
#include <routil1.h>
#include <rostor0.h>      /* Store interface                          */ 
#include <rodpat0.h>      /* Dispatcher interface                     */
#include <asn1st.h>      /* ASN1 MAVROS defs                         */

#include <rodpat1.h>      /* Dispatcher en/de-coding interface        */

#include <roacpm2.h>     /* ac-pm function call interface                  */

#include <roendeco.h>                                                        
#include <roppm00.h>     /* presentation eventing interface                */
#include <roppm01.h>     /* presentation ANS1  structures                  */
#include <roppm02.h>     /* presentation function call interf.             */
#include <roppm05.h>     /* presentation En/decode function protos         */
#include <roppmz0.h>     /* presentation eventing interface                */
#include <roppmzy.h>     /* presentation eventing interface                */

#include <roacpm0.h>     /* ac-pm eventing interface                       */  
#include <roacpm1.h>     /* ac-pm protocol interface                       */  
#include <roacpm3.h>     /* ac-pm internals                                */  
                                                                               
                                                                               
#include <roacpmzx.h>    /*     ac-pm prototypes for Mavros En/Decode fct  */  
#include <roacpz0.h>    /* acpm inform function structures                */   
#include <roacpmzy.h>    /* acpm inform function structures                */  
                                                                               
#include <roacpm01.h>    /*     ac-pm prototypes for help functions        */  

#include <spstep0.h>	  /* RTS-STEP General definitions	      */

/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro0603_decodAcpm                                            */
/*                                                                            */
/* AUTHOR       : Michael Rippstain                                           */
/* DATE         : 92-01-27                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */


signed short int PASCAL ro0603_decodAcpm (
	signed    short int       sAbstrSynId,
	signed   long  int        lEncodingChoice,
	rTro76_stDefinedBy FAR  * pDefinedBy,
	rTro01_pfvObjectId	  pTrfSynObjId,
	unsigned long  int        hsStore,
	void               NEAR * pPpmContext,
	unsigned long  int FAR  * phAcseApdus,
	unsigned long  int        ulLength,
	signed         int        iAnyNumb)
/* INPUT  PARAMETERS                                                          */
/*                                                                            */
/*     sAbstrSynId    :  Abstract syntax identifier (describes the named      */
/*                       abstract syntax the node belongs to).                */
/*                       Has to be written into the PDV header                */
/*                       (is not necessary for the decoding function itself). */
/*                                                                            */
/*     lEncodingChoice:  Encoding choice value of the PDV node:               */
/*                        - ANY                                               */
/*                        - OCTET STRING                                      */
/*                        - BIT STRING.                                       */
/*                       Has to be written into the PDV header                */
/*                       (is not necessary for the decoding function itself). */
/*                       For values and further comments see rouply0.h.       */
/*                                                                            */
/*     pDefinedBy     :  with ACSE Not used for RTPM (NULL pointer). Supposed to contain*/
/*                       ANY DEFINED BY decoding information (pointer to).    */
/*                                                                            */
/*     pTrfSynObjId   :  Transfer syntax object identifier (pointer to).      */
/*                                                                            */
/*     hsStore        :  Store handle (decoded byte string).                  */
/*                                                                            */
/*     pPpmContext    :  PPM context (pointer to).                            */
/*                                                                            */
/*     ulLength       : Total length of all embedded PDV decodings.           */
/*                       - ANY, OCTET STRING : scale "byte".                  */
/*                       - BIT STRING        : scale "bit".                   */
/*                      Note: ANY length only if within PDV-list or EXTERNAL  */
/*                            and if definite length decoding (otherwise 0).  */
/*                                                                            */
/* OUTPUT PARAMETERS                                                          */
/*                                                                            */
/*     phAcseApdus    : Memory handle to PDV node (pointer to), allocated     */
/*                      within the function.                                  */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*       0          : OK.                                                     */
/*    != 0          : Error                                                   */
/*                    (caller returns immediately with the same value,        */
/*                     mapping of the value within the PPM decoding           */
/*                     function).                                             */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro0603_decodAcpm*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed   short int     sRetCode;
rT0646_acseApdus  FAR *pAcseApdus;
rTro00_vObjectId       vBerTrfSyn = {3, 2, 1, 1};


/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (rs0603)

sRetCode = OK;

/*----------------------------------------------------------------------------*/
/* Currently only BER transfer syntax supported. Future additions possible.   */
/* Unsupported transfer syntax is a peer RTPM error i.e. a non-serious error. */
/*                                                                           */
/* Note: Before the whole decoding process, the PPM does a storeactive (on   */
/*       the store that was storeidled by LOIF). The whole decoding process  */
/*       then proceeds and finally the store is deleted.                     */
/*       Note: The store associated with the RTTR apdu must not be deleted.  */
/*             Therefore, the MAVROS decoder does a stlink on the store.     */
/*                                                                           */
/* **SKH: pTrfSynObjId is a pointer to vector. Change in interface ?          */
/*----------------------------------------------------------------------------*/
if (ro0003_objcmp (pTrfSynObjId,
		  (rTro01_pfvObjectId)vBerTrfSyn) != OK)
    RM0003_RETURN (rs0603, signed short int, RCro6m_UnexpectedPpduParam)

/*----------------------------------------------------------------------------*/
/* Allocate and lock the PDV node.                                            */
/*----------------------------------------------------------------------------*/
*phAcseApdus = ro0020_globalalloc (sizeof (rT0646_acseApdus));
if (*phAcseApdus == NULL)
    RM0003_RETURN (rs0603, signed short int, -R_GALLCE)

pAcseApdus = (rT0646_acseApdus FAR *) (void FAR *) 
                                   ro0021_globallock (*phAcseApdus);
if (pAcseApdus == (rT0646_acseApdus FAR *)NULL)
    RM0003_RETURN (rs0603, signed short int, -R_GLOCKE)

/*----------------------------------------------------------------------------*/
/* Initialize the parameters for the decoding function.                       */
/* Note: pAcseApdus->stPdvHeader.lType would be initialized in the decoding   */
/*       function of MAVROS; so need not be done here !                       */
/*----------------------------------------------------------------------------*/
pAcseApdus->stPdvHeader.sPmNumb         = RC0216_Acse;
pAcseApdus->stPdvHeader.sAbstrSynId     = sAbstrSynId;
pAcseApdus->stPdvHeader.lEncodingChoice = lEncodingChoice;

/*----------------------------------------------------------------------------*/
/* The actual decoding in this function here.                                 */
/* **SKH: store error detecteds in RTSE_apdus_dec.                            */
/*----------------------------------------------------------------------------*/
if (ulLength == 0)
    ulLength = ro0062_storelength (hsStore);
else
    ulLength = ulLength + ro0076_stwhere(hsStore);

sRetCode = (signed short int) ACSE_apdu_dec (hsStore, ulLength, 
              (rT0646_acseApdus FAR *)pAcseApdus, 
              (void NEAR *)pPpmContext);
if (sRetCode < 0)
    RM0003_RETURN (rs0603, signed short int, sRetCode)

/*----------------------------------------------------------------------------*/
/* Positive decoder return codes are non-serious errors. They should be       */
/* properly handled and not immediately returned.                             */
/*----------------------------------------------------------------------------*/
if (ro0022_globalunlock (*phAcseApdus) != OK)
    RM0003_RETURN (rs0603, signed short int, -R_GULOKE)

RM0003_RETURN (rs0603, signed short int, sRetCode)
}/*end-ro0603_decodAcpm*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNCTION                                               */
/*                                                                         */
/* NAME         : ro0604_clearNodeAcpm                                     */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-01-27                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */
/*                                                                         */
signed short int PASCAL ro0604_clearNodeAcpm (
	unsigned long  int FAR  *phAcseApdus)

/*                                                                         */
/* INPUT  PARAMETERS                                                       */
/*                                                                         */
/*     phAcseApdus    : Memory handle to PDV node (pointer to).            */
/*                                                                         */
/* OUTPUT PARAMETERS                                                       */
/*                                                                         */
/*     phAcseApdus    : Memory handle to PDV node (pointer to).            */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/*       0            : OK.                                                */
/*    != 0            : Unrecoverable error (see roerval.h).               */
/*                                                                         */
/* GLOBAL DATA (read only) : None                                          */
/*                                                                         */
/* GLOBAL DATA (modified)  :                                               */
/*                                                                         */
/* REMARKS :  none                                                         */
/*                                                                         */
/*inoff ********************************************************************/

{/*entr-ro0604_clearNodeAcpm*/

/* *********************** l o c a l    v a r i a b l e s **************** */
/* ***********************                                **************** */

signed   short int     sRetCode;
unsigned short int     i;
rT0646_acseApdus  FAR *pAcseApdus;
signed   short int     sErrCount;    /* Error counter. Dont return inspite */
				     /* of errors, but increment sErrCount.*/
				     /* iErrCount > 0 on return means      */
				     /* errors on execution.               */

/* **************************** S T A R T ******************************** */
/* ****************************           ******************************** */

RM0002_ENTRY (rs0604)

/*-------------------------------------------------------------------------*/
/* Note1: The function is called only if a PDV node exists (checked by     */
/*        ro0246_clearPdvTree).                                            */
/*                                                                         */
/* Note2: No attempt at localunlock (PDVnode). It must be already unlocked */
/*        prior to a call to this function.                                */
/*                                                                         */
/* Note3: The function continues in the case of an error.                  */
/*        Is this behaviour OK                                             */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* Initialization.                                                         */
/*-------------------------------------------------------------------------*/

sRetCode = OK;
sErrCount = 0;

if (phAcseApdus) {
    pAcseApdus = (rT0646_acseApdus FAR *) (void FAR *) 
                                 ro0021_globallock (*phAcseApdus);
}

if ((!phAcseApdus) || (pAcseApdus == (rT0646_acseApdus FAR *)NULL)) {
    sErrCount++;
}
else {
    /*---------------------------------------------------------------------*/
    /* Select PDU type and clear user data, if any.                        */
    /*---------------------------------------------------------------------*/

    switch (pAcseApdus->stPdvHeader.lType) {

     case RC0665_AARQapdu :
	if (pAcseApdus->aarqApdu.bUserInfo != 0 ) {
	  for ( i=0; i<pAcseApdus->aarqApdu.ulUserInfo; i++)
	      if (ro0246_clearPdvTree (
		       &(pAcseApdus->aarqApdu.vUserInfo[i]) ) != OK)
		  sErrCount++;
	}
	break;

     case RC0666_AAREapdu :
	if (pAcseApdus->aareApdu.bUserInfo != 0 ) {
	  for ( i=0; i<pAcseApdus->aareApdu.ulUserInfo; i++)
	      if (ro0246_clearPdvTree (
		       &(pAcseApdus->aareApdu.vUserInfo[i]) ) != OK)
		  sErrCount++;
	}
	break;


     case RC0667_RLRQapdu :
	if (pAcseApdus->rlrqApdu.bUserInfo != 0 ) {
	  for ( i=0; i<pAcseApdus->rlrqApdu.ulUserInfo; i++)
	      if (ro0246_clearPdvTree (
		       &(pAcseApdus->rlrqApdu.vUserInfo[i]) ) != OK)
		  sErrCount++;
	}
	break;

     case RC0668_RLREapdu :
	if (pAcseApdus->rlreApdu.bUserInfo != 0 ) {
	  for ( i=0; i<pAcseApdus->rlreApdu.ulUserInfo; i++)
	      if (ro0246_clearPdvTree (
		       &(pAcseApdus->rlreApdu.vUserInfo[i]) ) != OK)
		  sErrCount++;
	}
	break;

     case RC0669_ABRTapdu :
	if (pAcseApdus->abrtApdu.bUserInfo != 0 ) {
	  for ( i=0; i<pAcseApdus->abrtApdu.ulUserInfo; i++)
	      if (ro0246_clearPdvTree (
		       &(pAcseApdus->abrtApdu.vUserInfo[i]) ) != OK)
		  sErrCount++;
	}
	break;

    default : sErrCount++; break;
    }/*switch-end*/
}/*else-end*/

/*-------------------------------------------------------------------------*/
/* Return back formalities here.                                           */
/*-------------------------------------------------------------------------*/

if (ro0022_globalunlock (*phAcseApdus) != OK)
    sErrCount++;

if (ro0024_globalfree (*phAcseApdus) != OK)
    sErrCount++;
else
    *phAcseApdus = 0;

if (sErrCount != 0) sRetCode = -R_FCLEAR;
else                sRetCode = OK;

RM0003_RETURN (rs0604, signed short int, sRetCode)

}/*end-ro0604_clearNodeAcpm*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro060c_decodSniPAcpm                                        */
/*                                                                            */
/* AUTHOR       : Vinita                                                      */
/* DATE         : 93-04-07                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */


signed short int PASCAL ro060c_decodSniPAcpm ( 
signed   short int   FAR  * psAbsId,
unsigned long int           pStore,
void                NEAR  * pPpmContext,
unsigned long int    FAR  * hAcseApdus)

{/*entr-ro060c_decodSniPAcpm*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed   short int     sRetCode = OK;
rT0646_acseApdus   FAR *pAcseApdus;
unsigned long  int FAR *PdvNode = LNULL;
signed   long  int  i=0;
/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (rs060c)

/* **** get store address to copy encoded string to store **** */
*hAcseApdus = ro0020_globalalloc (sizeof (rT0646_acseApdus));
if (*hAcseApdus)  {
    pAcseApdus = (rT0646_acseApdus FAR *) ro0021_globallock (*hAcseApdus);
    if (pAcseApdus == (rT0646_acseApdus FAR *)NULL)
        sRetCode  = -R_GLOCKE;
    else {
    /* ************* decode ACSE Mask Information **************** */ 
                                                                
        i = ro0070_stgetc( pStore);

        /* Whether check is required for 0x 0a  as Acse Msk !!!*/
        pAcseApdus->stPdvHeader.sPmNumb = RC0216_Acse;  

    /* ************* decode Mask Information ********************* */    
        pAcseApdus->stPdvHeader.lType = (signed long int)
                                            ro0070_stgetc (pStore);
                         

        switch (pAcseApdus->stPdvHeader.lType) {

           case RC0665_AARQapdu:
                sRetCode = AARQSniP_type_dec(pStore,
                      &(pAcseApdus->aarqApdu),pPpmContext); 
                if (pAcseApdus->aarqApdu.bUserInfo)  {
                  PdvNode =
                    (unsigned long int FAR *)&pAcseApdus->aarqApdu.vUserInfo[0];
                }
           break;

           case RC0666_AAREapdu:
                sRetCode = AARESniP_type_dec(pStore,
                           &(pAcseApdus->aareApdu),pPpmContext); 
                if (pAcseApdus->aareApdu.bUserInfo) {
                  PdvNode =
                    (unsigned long int FAR *)&pAcseApdus->aareApdu.vUserInfo[0];
                }
           break;

           case RC0667_RLRQapdu:
                sRetCode = RLRQSniP_type_dec(pStore,
                           &(pAcseApdus->rlrqApdu),pPpmContext); 
                if (pAcseApdus->rlrqApdu.bUserInfo) {
                  PdvNode =
                    (unsigned long int FAR *)&pAcseApdus->rlrqApdu.vUserInfo[0];
                }
           break;

           case RC0668_RLREapdu:
                sRetCode = RLRESniP_type_dec(pStore,
                           &(pAcseApdus->rlreApdu),pPpmContext); 
                if (pAcseApdus->rlreApdu.bUserInfo) {
                  PdvNode =
                    (unsigned long int FAR *)&pAcseApdus->rlreApdu.vUserInfo[0];
                }
           break;

           case RC0669_ABRTapdu:
                sRetCode = ABRTSniP_type_dec(pStore,
                           &(pAcseApdus->abrtApdu),pPpmContext); 
                if (pAcseApdus->abrtApdu.bUserInfo) {
                  PdvNode =
                    (unsigned long int FAR *)&pAcseApdus->abrtApdu.vUserInfo[0];
                }
           break; 

        }  /* end switch lType  */
    

        /* call despatcher to switch encoding */ 
        if (PdvNode) {
             rTro00_vObjectId  TransSynName;  
             signed short int  sPmNumb;       
             rT0410_stPdvList  stEncodingInfo;
 
            i = ro0070_stgetc (pStore); /* to get the ConId */  
            stEncodingInfo.lPresContId=(signed long int)i;   
            stEncodingInfo.oTransSynName [0] = 0;                     
            sRetCode = ro0416_decodePdvList (                               
      (rTro01_pfvObjectId       FAR *) &TransSynName,     
      (signed short int         FAR *) psAbsId,           
      (signed short int         FAR *) &sPmNumb,          
      (rT0410_stPdvList         FAR *) &stEncodingInfo,   
      (void                   NEAR  *) pPpmContext );        
           if(!sRetCode)                                                
            sRetCode = ro0243_decodSniP (RC0221_RoseUser, pStore, 
                                         (void NEAR *)pPpmContext,     
                                         (unsigned long  int FAR *)PdvNode,
                                         (signed   short int FAR *)psAbsId); 
        }
  
        if (ro0022_globalunlock (*hAcseApdus) != OK)
            sRetCode  = -R_GLOCKE; 

    }/*else-end: pAcseApdus == NULL */

} else sRetCode = -R_GALLCE;

RM0003_RETURN (rs060c, signed short int, sRetCode)

}/*end-ro060c_decodSniPAcpm*/

