/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spuctr1.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:39  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:17  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:37  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:43  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:49  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:09:35  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:30  keutel
 * 	creation
 * 	[1994/03/21  13:38:15  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spuctr1.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:39 $" } ;
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
/* NAME 	: spuctr1.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K                                      */
/* DATE 	: 25-11-93					      */
/*                                                                    */
/* COMPONENT    : RTROS Evaluation Program                            */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : In this module the the ROPM functions for decoding  */
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

static char *sccsid =    "@(#) spuctr1.c 1.1 93/12/20   RTS-V2.00";

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
#include <rodpat1.h>      /* Dispatcher en/de-coding interface        */

#include <tpdu.h>        /* RTS tpdu intterface                      */        
#include <roexif2.h>     /* RTS tpdu intterface                      */        
                                                                               
#include <asn1st.h>      /* RTS apma calling  interface.             */        
                                                                               
#include <routilt.h>     /* RTS apma calling  interface.             */        
                                                                               
#include <rouctr0.h>     /* ROS upper control service interface      */        
#include <rouctr2.h>     /* ROS upper control calling interface      */        
                                                                               
#include <roendeco.h>                                                          

#include <spstep0.h>	  /* RTS-STEP General definitions	      */

/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/
static rTro00_vObjectId  vBerTrfSyn = { 3, 2, 1, 1}; 
/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro1503_decodUpperCtrl                                       */
/*                                                                            */
/* AUTHOR       : Vinita                                      */
/* DATE         : 93-04-05                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro1503_decodUpperCtrl (
signed   short int        sAbstrSynId,
signed   long  int        lEncodingChoice,
rTro76_stDefinedBy FAR  * pDefinedBy,
rTro01_pfvObjectId        pTrfSynObjId,
unsigned long  int        hsStore,
void               NEAR * pPpmContext,
unsigned long  int FAR  * phAct1Apdus,
unsigned long  int        ulLength,
signed         int        iAnyNumb)

/* INPUT  PARAMETERS                                                          */
/*                                                                            */
/* OUTPUT PARAMETERS                                                          */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed short int          sRetCode;
signed long  int          eofStore;
rTro04_stUserData  FAR *pAct1Apdus;
signed long int         lTmpPos = NULL;
/* *********************** S T A R T **************************************** */
/* ***********************           **************************************** */

RM0002_ENTRY (rs1503)

sRetCode     = 0;
*phAct1Apdus = 0;

/* *********************** Check transfer syntax and allocate a ************* */
/* *********************** pdvNode.                             ************* */

*phAct1Apdus = ro0020_globalalloc (sizeof (rTro04_stUserData));

pAct1Apdus = (*phAct1Apdus)
            ? (rTro04_stUserData FAR *) ro0021_globallock (*phAct1Apdus)
            : (rTro04_stUserData FAR *) 0;

if (! pAct1Apdus) sRetCode = -R_GMEMRY;
if (sRetCode) RM0003_RETURN (rs1503, signed short int, sRetCode)

/* *********************** Save apdu header parameters ********************** */
/* ***********************                             ********************** */

pAct1Apdus->stPdvHeader.sPmNumb         = RC0221_RoseUser;
pAct1Apdus->stPdvHeader.sAbstrSynId     = sAbstrSynId;
pAct1Apdus->stPdvHeader.lEncodingChoice = lEncodingChoice;
if(iAnyNumb) /* ONLY for ROS-OPERATIONS */
    pAct1Apdus->stPdvHeader.lFraCount = (long int)1;
/* x-x-x by kc 10-11-93 */
/* by kc on 11.5 */
 pAct1Apdus->stPdvHeader.lType = RC1504_RcvData;  
/* *********************** Skip asn1 and link store to user ***************** */
/* *********************** data pdvNode                     ***************** */

ro0078_stlink (hsStore);

pAct1Apdus->stStore.hsStore  = hsStore;
pAct1Apdus->stStore.ulOffset = ro0076_stwhere     (hsStore);
eofStore                     = ro0062_storelength (hsStore);

if (ro0063_sterror (hsStore)) sRetCode = -R_ACSTOR;

if (!ro0003_objcmp (pTrfSynObjId, vBerTrfSyn)) {   /* by kc1805 */

  if (! sRetCode) {
      sRetCode = asn1st_skip (hsStore, eofStore);
  }

  if (! sRetCode) {
	pAct1Apdus->stStore.ulNumb  = ro0076_stwhere (hsStore)
				  - pAct1Apdus->stStore.ulOffset;

      if (ro0063_sterror (hsStore)) sRetCode = -R_ACSTOR;
  }
 }   /* kc1805 */
 else {     /* kc1805 */
   if (!iAnyNumb)  {  /* BIND/UNBIND/ABORT data */
     if (ulLength)  {
	pAct1Apdus->stStore.ulNumb  = ulLength ;
	sRetCode = ro0066_storepos(hsStore , ulLength+ 
					    pAct1Apdus->stStore.ulOffset);
	if (ro0063_sterror (hsStore)) sRetCode = -R_ACSTOR;
     }else 
       sRetCode = RCro6m_UnexpectedPpduParam; 
   } else {  /* OPERATION data decod the Tag & Length */
       sRetCode = asn1st_skiptype(hsStore , eofStore);     /* skip TAG */
       sRetCode = asn1st_lendec(hsStore,eofStore, 
				(long FAR *)&(pAct1Apdus->stStore.ulNumb) );
       if ( (pAct1Apdus->stStore.ulNumb != -1 ) && (!sRetCode ) ){
						       /* indef len */
		   lTmpPos  = ro0076_stwhere (hsStore);
		 
	       sRetCode = ro0066_storepos ( hsStore , 
				   pAct1Apdus->stStore.ulOffset);
	       pAct1Apdus->stStore.ulOffset = lTmpPos;
	       sRetCode = asn1st_skip (hsStore, eofStore);
	 }else 
	   sRetCode = RCro6m_UnexpectedPpduParam; 
   } /*kc 2105 */
 }   /* kc1805 */
if (ro0022_globalunlock (*phAct1Apdus)) sRetCode = -R_GULOKE;

RM0003_RETURN (rs1503, signed short int, sRetCode)
}/*end*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro1504_clearNodeUpperCtrl                                   */
/*                                                                            */
/* AUTHOR       : Vinita                                                      */
/* DATE         : 93-04-05                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */
/*                                                                            */

   signed   short int PASCAL ro1504_clearNodeUpperCtrl (
   unsigned long  int FAR   *phData)

/*                                                                            */
/* INPUT  PARAMETERS                                                          */
/*                                                                            */
/*     phData    : Memory handle to PDV node (pointer to).            */
/*                                                                            */
/* OUTPUT PARAMETERS                                                          */
/*                                                                            */
/*     phData    : Memory handle to PDV node (pointer to).            */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*       0            : OK.                                                   */
/*    != 0            : Unrecoverable error (see roerval.h).                  */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

static unsigned long  int       hEvData = LNULL;

rTro04_stUserData  FAR  *pAct1Apdus;
rT1502_stSnData    FAR  *pSnData;
rTro03_stPdvHeader FAR  *pPdvHeader;
rT1504_stEvData    FAR  *pEvData;
signed   long int       i=0;
signed   short int       rc;    
signed   short int       sErrCount;     /* Error counter. Dont return inspite */
					/* of errors, but increment sErrCount.*/
					/* iErrCount > 0 on return means      */
					/* errors on execution.               */

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (rs1504)

sErrCount = 0;
pPdvHeader = (rTro03_stPdvHeader FAR  *) ro0021_globallock(*phData);

if (!pPdvHeader)  sErrCount++;

if (pPdvHeader->lType == RC1504_RcvData) {

       ro0022_globalunlock (*phData);
       pAct1Apdus = (rTro04_stUserData FAR *) 
		    ro0021_globallock (*phData);

       if (!pAct1Apdus)  sErrCount++;

   /* ************** Delete data store ************************** */
   /* **************                   ************************** */

       if (pAct1Apdus->stStore.hsStore) {
	   if (ro0075_deletestore (pAct1Apdus->stStore.hsStore)) sErrCount++;
       }

   /* *************** Free pdvNode ****************************** */
   /* ***************              ****************************** */

       ro0022_globalunlock (*phData);

       if (ro0024_globalfree   (*phData))       sErrCount++;
       else                                    *phData = 0;

} /* end if lType is RcvData */

else {
       
       ro0022_globalunlock (*phData);
       pSnData = (rT1502_stSnData FAR *) 
		    ro0021_globallock (*phData);
       if (!pSnData)  sErrCount++;   

/*       hEvData = ro0020_globalalloc (sizeof (rT1504_stEvData));
       if (!hEvData)  sErrCount++; */
       if (hEvData) {
	   pEvData = (rT1504_stEvData FAR *) 
		    ro0031_locallock (hEvData);
	   if (!pEvData)  sErrCount++;
       } 
       /* ****** copy SnData structure in EvData so as to ****** */
       /* ****** free SnData (p1) & and give Buffer-ind up****** */

       /* **** Here lCount will not be 0 .It will be Fracount ** */
       /*  by kc on 11/5 */
     /* kc on 13-9-93 */
     if ( pSnData->lCount == 0 ) 
	pSnData->lCount = pSnData->stPdvHeader.lFraCount;

       /* in both lFraCount the +1 is added by kc on 10-11-93 */
       if (pSnData->lCount == (pSnData->stPdvHeader.lFraCount))
       { 
       /* ******* when it is a  first clear Pdv call of User *** */
       /* ******* Data from ACPM then copy the following  ****** */
 
       hEvData = ro0030_localalloc (sizeof (rT1504_stEvData));
       if (!hEvData)  sErrCount++;
       pEvData = (rT1504_stEvData FAR *) 
		    ro0031_locallock (hEvData);
       if (!pEvData)  sErrCount++;

	  pEvData->iMemType = pSnData->iMemType ;  

	  if (pSnData->iMemType == RC1500_Memory)
	    pEvData->unBufferRef.pBufRef = pSnData->unBufferRef.pBufRef;
	  else if (pSnData->iMemType == RC1501_File)
	    pEvData->unBufferRef.pTpduName = pSnData->unBufferRef.pTpduName;
	  else
	    pEvData->unBufferRef.ulhandle = pSnData->unBufferRef.ulhandle;

	  pEvData->bSpecFlg = FALSE;
	  pEvData->ulBufLen = pSnData->ulBufLen ;  
	  pEvData->ulFragCount = pSnData->stPdvHeader.lFraCount ;  

       } /* clear Pdv  call for first time */ 
 
	 pEvData->vFragArray[i-1].iAbstrSynId =(signed int)pSnData->vAbsId[i-1];
	 pEvData->vFragArray[i-1].ulLength    = pSnData->vLength[i-1]; 
       /* by kc on 11/5 */
       /* i++; */
	  pSnData->lCount--; 
	  if(hEvData) ro0032_localunlock(hEvData);
       if ( pSnData->lCount == 0)
       {
       /* **** This was the last call for  clear Pdv Tree *** */
       /* by kc for step   **** 
       rc = ro0221_pushOnInterface ( 0, RC0221_RoseUser, 
		   RC1501_RO_BUFFERind, 0, 0, hEvData, 0); */
       if(rc) sErrCount++;
       hEvData = LNULL;

       ro0022_globalunlock (*phData);

       if (ro0024_globalfree   (*phData))       sErrCount++;
       else                                    *phData = 0;
       }
       else
       ro0022_globalunlock (*phData);
} /* end if lType is SnData */

RM0003_RETURN (rs1504, signed short int, ((sErrCount) ? -R_FCLEAR : 0))
}/*end*/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro1507_decodSniPUpperCtrl                                   */
/*                                                                            */
/* AUTHOR       : Vinita                                                      */
/* DATE         : 93-04-05                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro1507_decodSniPUpperCtrl ( 
signed   short int FAR *psAbsId,
unsigned long  int      hsStore,
void                   *pPpmContext,
unsigned long  int FAR *phAct1Apdus)

/* INPUT  PARAMETERS                                                          */
/*                                                                            */
/* OUTPUT PARAMETERS                                                          */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed short int          sRetCode=0;
char               FAR  * pBuffActual = NULL;  
rTro04_stUserData  FAR  *pAct1Apdus = NULL;
signed long int          lLen = NULL;
/* *********************** S T A R T **************************************** */
/* ***********************           **************************************** */

RM0002_ENTRY (rs1507)

 *phAct1Apdus = ro0020_globalalloc (sizeof (rTro04_stUserData));

 pAct1Apdus = (*phAct1Apdus)
	      ? (rTro04_stUserData FAR *) ro0021_globallock (*phAct1Apdus)
	      : (rTro04_stUserData FAR *) 0;

 if (! pAct1Apdus) sRetCode = -R_GMEMRY;
 if (sRetCode) RM0003_RETURN (rs1507, signed short int, sRetCode)
/* *********************** Save apdu header parameters ********************** */
/* ***********************                             ********************** */

 pAct1Apdus->stPdvHeader.sPmNumb        = RC0221_RoseUser;

 pAct1Apdus->stPdvHeader.sAbstrSynId     = *psAbsId;
 pAct1Apdus->stPdvHeader.lType = RC1504_RcvData;  
 pAct1Apdus->stPdvHeader.lFraCount = (long int)1;
/* x-x-x by kc 10-11-93 */
/* *********************** Skip asn1 and link store to user ***************** */
/* *********************** data pdvNode                     ***************** */

 ro0078_stlink (hsStore);

 pAct1Apdus->stStore.hsStore  = hsStore;
 pBuffActual = ro0072_stgets (hsStore, sizeof(long)); 
 RM0013_DECODEi (pAct1Apdus->stStore.ulNumb, unsigned long int, long)
 
 if ( (lLen = ro0062_storelength(hsStore) ) < 0 ) sRetCode = -R_USTERR;
 pAct1Apdus->stStore.ulOffset = ro0076_stwhere    (hsStore);
 if (pAct1Apdus->stStore.ulNumb == 0xffffffff) { /* indef length */
     if(!sRetCode)
	 pAct1Apdus->stStore.ulNumb =lLen - pAct1Apdus->stStore.ulOffset ;
 }
  
 if (ro0063_sterror (hsStore)) sRetCode = -R_ACSTOR;
 if(!sRetCode){
  if ( lLen < (signed long int)
             (pAct1Apdus->stStore.ulOffset + pAct1Apdus->stStore.ulNumb)) 
    sRetCode = RCro6n_InvalidPpduParam;
  else    
    sRetCode = ro0066_storepos (hsStore, 
			     (pAct1Apdus->stStore.ulOffset +
			       pAct1Apdus->stStore.ulNumb) );
 }
  if (ro0022_globalunlock (*phAct1Apdus)) sRetCode = -R_GULOKE;
RM0003_RETURN (rs1507, signed short int, sRetCode)

}/*end*/

