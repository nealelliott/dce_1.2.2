/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rouctr1.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:34  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:09  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:23  keutel
 * 	OT11620
 * 	[1994/08/19  13:58:52  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:28  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:39  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:44  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:09:16  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:23  keutel
 * 	creation
 * 	[1994/03/21  13:37:57  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rouctr1.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:34 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1991 by                                                  *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                                 *
*  * All rights reserved                                                    *
*  *                                                                        *
*  * This software is furnished under licence and may be used only in       *
*  * accordance with the terms of that licence and with the inclusion of    *
*  * the above copyright notice. This software may not be provided or       *
*  * otherwise made available to, or used by, any other person. No title    *
*  * to or ownership of the software is hereby transferred.                 *
*                                                                           *
\***************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : rouctr1.c                                           */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE         : 93-03-04                                            */
/*                                                                    */
/* COMPONENT    : RTS association control function                    */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.00                                         */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/* 2.00 K0  | 92-01-25|  original                      |jm  |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                     Module Identification                          */
/**********************************************************************/

static char *sccsid =    "@(#) rouctr1.c 1.2 93/12/20   RORTS-V3.00";

/*********************************************************************/
/*              I N C L U D E S                                      */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>     /* RTS general definitions                  */
#include <roerval.h>     /* RTS error values                         */
#include <rolgval.h>     /* RTS logging type definitions             */

#include <rouply0.h>     /* RTS common upper layer definitons        */
#include <rouply1.h>     /* RTS common upper layer definitons        */

#include <routil0.h>     /* RTS utilities interface                  */
#include <routil1.h>     /* RTS utilities interface for upper layer  */

#include <rodpat0.h>     /* RTS dispatcher pm interface              */

#include <roppm01.h>     /* PPM data structures which are common     */   
#include <roppm05.h>     /* PPM common functions proto type          */   

#include <rostor0.h>     /* RTS utility store interface              */
#include <tpdu.h>        /* RTS tpdu intterface                      */
#include <roexif2.h>     /* RTS tpdu intterface                      */

#include <asn1st.h>      /* RTS apma calling  interface.             */

#include <routilt.h>     /* RTS apma calling  interface.             */

#include <rouctr0.h>     /* ROS upper control service interface      */ 
#include <rouctr2.h>     /* ROS upper control calling interface      */ 

#include <roendeco.h>


/**********************************************************************/
/*               L O C A L  V A R I A B L E S                         */
/**********************************************************************/

/* ****************** Bassic encoding Rules object ****************** */
/* ****************** identifier value             ****************** */

static rTro00_vObjectId  vBerTrfSyn = { 3, 2, 1, 1};

/* ****************** Logging function name strings ***************** */
/* ******************                               ***************** */

RM0001_STRG (rs1502, "ro1502_encodUpperCtrl")
RM0001_STRG (rs1503, "ro1503_decodUpperCtrl")
RM0001_STRG (rs1504, "ro1504_clearNodeUpperCtrl")
RM0001_STRG (rs1506, "ro1506_encodSniPUpperCtrl")
RM0001_STRG (rs1507, "ro1507_decodSniPUpperCtrl")


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro1502_encodUpperCtrl                                       */
/*                                                                            */
/* AUTHOR       : Vinita                                                      */
/* DATE         : 93-01-04                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro1502_encodUpperCtrl (
rTro76_stDefinedBy   FAR  * pDefinedBy,
rTro01_pfvObjectId          pTrfSynObjId,
unsigned long int           hsStore,
void                 NEAR * pPpmContext,
unsigned long int    FAR  * phSnData,
unsigned long int    FAR  * pLength,
signed         int          iAnyNumb)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
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

signed   short int       rc;
signed   long  int       i;
signed   long  int       PdvHandleCount;
rT1502_stSnData     FAR *pSnData;


char                FAR *pBufferAddress;

signed long int       lIniPos = LNULL;
rT00t1_stTpOpen         stTpOpen; 
rT00t5_stTpClose        stTpClose;
rT00t2_stTpRead         stTpRead;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (rs1502)

pSnData = (rT1502_stSnData FAR *) ro0021_globallock (*phSnData);
rc   = (pSnData) ? 0 : -R_GLOCKE;

if (rc)
    RM0003_RETURN (rs1502, signed short int, rc)

/* *********************** Move user data into store where the  ************* */
/* *********************** encoded string is saved.             ************* */

  /* *** assign the Null to Length by kc on 25/5 for Octet/Bit *** */

*pLength = NULL; 
i = pSnData->lCount; 
  /* *****************************
  * Binary data compatible with ROS25a . by Adding TAG & LEN in the case of
     Invoke/Result/Error  Only         
  * This difference is indicated by iAnyNumb.
      == 0  -> Bind/Unbind/abort User data 
      != 0  -> Invoke/Result/Error User data   
   ********************************************************************* */
 
   
if(  (iAnyNumb ) /* if Any Numb is present Encpsulate the UserData with T&L */
	&&(ro0003_objcmp (pTrfSynObjId, vBerTrfSyn)) /* Only for Bin. data */
     ) {
     rc = ro0067_stputc (hsStore, 0xe1); /* Private TrSyn */
       /* ***** Length is coded as defined *********** */

     lIniPos = ro0076_stwhere(hsStore);
     rc = asn1st_deflenpos(hsStore);
     if (rc){ 
	if (ro0022_globalunlock (*phSnData)) rc = -R_GULOKE;
	RM0003_RETURN (rs1502, signed short int, rc)
     }
}
  
if (pSnData->iMemType == RC1500_Memory) {
   pBufferAddress = pSnData->unBufferRef.pBufRef + pSnData->vOffset[i] ;
   rc = ro0068_stputm (hsStore, pBufferAddress, 
				 (unsigned long) pSnData->vLength[i]);
} /* end memtype is memory */

else if (pSnData->iMemType == RC1502_Handle) {
    if ( (pBufferAddress = (char FAR *)            
    ro0021_globallock (pSnData->unBufferRef.ulhandle)) == NULL)      
    RM0003_RETURN (rs1502, signed short int, - R_LLOCKE)

    pBufferAddress += pSnData->vOffset[i] ;  
    rc = ro0068_stputm (hsStore, pBufferAddress, pSnData->vLength[i]); 

    if (ro0022_globalunlock (pSnData->unBufferRef.ulhandle) != 0) 
    RM0003_RETURN (rs1502, signed short int, -R_LULOKE)  

} /* end memtype is handle */

else  /* iMemType  == RC1501_File */
{
    if (!i)
    {
     stTpOpen.bIgnore              = FALSE;
     stTpOpen.pDataDescription     = pSnData->unBufferRef.pTpduName;
     stTpOpen.usLenDataDescription = (unsigned short int)pSnData->ulBufLen;
     stTpOpen.iDir                 = TPIN;
								
     rc = ro00t1_tpOpen ((rT00t1_stTpOpen FAR *)&stTpOpen);                             
     if (rc){                                       
      if (ro0022_globalunlock (*phSnData)) rc = -R_GULOKE;
      RM0003_RETURN (rs1502, signed short int, rc)
    } 
     pSnData->sOpenId  = stTpOpen.sOpenId;
     
}/* end if it is the first count */
    /* common code assuming  file is already opened  */

   stTpRead.bIgnore             =FALSE;                      
   stTpRead.sOpenId              =pSnData->sOpenId;
   if ((i == 0) && (pSnData->vLength[i]== 0))
       stTpRead.lInRequiredDataLength=stTpOpen.lFileLength;
   else 
      stTpRead.lInRequiredDataLength=pSnData->vLength[i];
 
   rc = ro00t7_tpduToStore ((rT00t2_stTpRead FAR *)&stTpRead, hsStore);
   if (rc){                                        
      if (ro0022_globalunlock (*phSnData)) rc = -R_GULOKE;
      RM0003_RETURN (rs1502, signed short int, rc)
   }
     PdvHandleCount = (pSnData->stPdvHeader.lFraCount) -1;
/*   PdvHandleCount = (pSnData->stPdvHeader.lFraCount) ;*/
  
   /***** if this was the last external called tthen the  file should  ****/
   /***** be closed  ********************/
 
   if (i == PdvHandleCount) 
   {
      stTpClose.bIgnore = FALSE;                    
      stTpClose.sOpenId = pSnData->sOpenId;       
						
      rc = ro00t5_tpClose ((rT00t5_stTpClose FAR *) &stTpClose);               
      if (rc){                                        
	if (ro0022_globalunlock (*phSnData)) rc = -R_GULOKE;
	RM0003_RETURN (rs1502, signed short int, rc)
      }
   } /* end to i=PdvHandleCount */
 
  } /* end memtype is File */
 
    /* fill the Length parameter */
   if ( lIniPos) {  /* if AnyNumb & NonBER then this will be atleast 1 */
	 rc = asn1st_deflencod (hsStore ,lIniPos);
       if (rc){        
	  if (ro0022_globalunlock (*phSnData)) rc = -R_GULOKE;
	  RM0003_RETURN (rs1502, signed short int, rc)
	} 
   }
   pSnData->lCount++;
   if (ro0022_globalunlock (*phSnData)) rc = -R_GULOKE;

RM0003_RETURN (rs1502, signed short int, rc)
}/*end*/


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
signed long int         lIndefcheck = NULL; 
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
 pAct1Apdus->stPdvHeader.lType = RC1504_RcvData;  
/* *********************** Skip asn1 and link store to user ***************** */
/* *********************** data pdvNode                     ***************** */

ro0078_stlink (hsStore);

pAct1Apdus->stStore.hsStore  = hsStore;
pAct1Apdus->stStore.ulOffset = ro0076_stwhere     (hsStore);
eofStore                     = ro0062_storelength (hsStore);

if (ro0063_sterror (hsStore)) sRetCode = -R_ACSTOR;

/* ASN1 User data decoding */                                             
if  (!ro0003_objcmp (pTrfSynObjId, vBerTrfSyn))  {                        
  if (!iAnyNumb ) { /*  BIND/UNBIND/ABORT data */                         
   if ( (ulLength) && (!sRetCode) )  { /* ASN1 data with def length  */   
      pAct1Apdus->stStore.ulNumb  = ulLength ;                            
      sRetCode = ro0066_storepos(hsStore , ulLength+                      
                                            pAct1Apdus->stStore.ulOffset);
      if (ro0063_sterror (hsStore)) sRetCode = -R_ACSTOR;                 
   } else if(!sRetCode)  {/* ASN1 data with indef length  */              
      sRetCode = asn1st_skip (hsStore, eofStore);
     if (! sRetCode) {
	pAct1Apdus->stStore.ulNumb  = ro0076_stwhere (hsStore)
				  - pAct1Apdus->stStore.ulOffset;

      if (ro0063_sterror (hsStore)) sRetCode = -R_ACSTOR;
      } 
   } /* end of ASN1 data with indef length encoding */             
 } /* end of ASN1 BIND/UNBIND/ABORT data */                        
 else {   /* OPERATION ASN1 data */                   /* **skh */
   if (( (ulLength+pAct1Apdus->stStore.ulOffset) == (unsigned long int)eofStore)
      && ( ro042b_indefDecget(pPpmContext))) {                                  
        /* skip from back !!! */                                                
          pAct1Apdus->stStore.ulNumb =                                          
                                 eofStore - ro0076_stwhere(hsStore) -           
                                        2 * ro042b_indefDecget(pPpmContext) ;   
          pAct1Apdus->stStore.ulOffset = ro0076_stwhere(hsStore) ;              
          sRetCode = ro0066_storepos ( hsStore ,                                
                                   pAct1Apdus->stStore.ulNumb +                 
                                        pAct1Apdus->stStore.ulOffset );         
   } /*end of OPERATION ASN1 data skip from back */                                else { /* Use the Length to skip */                                       
      pAct1Apdus->stStore.ulNumb  = ulLength ;                               
      sRetCode = ro0066_storepos(hsStore , ulLength+                         
                                            pAct1Apdus->stStore.ulOffset);   
      if (ro0063_sterror (hsStore)) sRetCode = -R_ACSTOR;                    
                                                                             
   }/* end of using the Length */                                            
 } /* end of OPERATION ASN1 data */                                          
}   /* end of ASN1 User Data decoding  */                                    
else {   /* BINARY data decoding */ 
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
       lTmpPos  = ro0076_stwhere (hsStore);                    
       sRetCode = asn1st_length(hsStore,eofStore,&lIndefcheck);
/*
       sRetCode = asn1st_lendec(hsStore,eofStore,  
				(long FAR *)&(pAct1Apdus->stStore.ulNumb) ); */
           /* Def length bin data Operation type decoding */
       if  ( (lIndefcheck != 0) && (!sRetCode) ) {                          
                               /* i.e. it is not indefinite length */       
           sRetCode = ro0066_storepos ( hsStore , lTmpPos );                
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
        }
        else if (!sRetCode){ /* indef len  Added by Baiju / Amit              
                                        Tue Apr  5 14:31:11 MDT 1994 */       
             ro042c_indefDecinc(pPpmContext);                                 
             pAct1Apdus->stStore.ulNumb =                                     
                                 eofStore - ro0076_stwhere(hsStore) -         
                                        2 * ro042b_indefDecget(pPpmContext) ; 
             pAct1Apdus->stStore.ulOffset = ro0076_stwhere(hsStore) ;         
             sRetCode = ro0066_storepos ( hsStore ,                           
                                   pAct1Apdus->stStore.ulNumb +               
                                        pAct1Apdus->stStore.ulOffset + 2);    
        } /* end of BIN indef length decding */                               
   } /* end of BIN data OPERATION decod  */ 
 }   /* end of BINARY data decoding  */     
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
    
       rc = ro0221_pushOnInterface ( 0, RC0221_RoseUser, 
		   RC1501_RO_BUFFERind, 0, 0, hEvData, 0);
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
/* NAME         : ro1506_encodSniPUpperCtrl                                   */
/*                                                                            */
/* AUTHOR       : Vinita                                                      */
/* DATE         : 93-01-04                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro1506_encodSniPUpperCtrl ( 
unsigned long int           hsStore,
void                 NEAR * pPpmContext,
unsigned long int    FAR  * pNode)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
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

signed   short int       rc=0;

rT1502_stSnData    FAR  *pSnData = NULL;
char               FAR  *pStoreAddress =NULL;
char               FAR  *pBufferAddress = NULL ;
char               FAR  *pBuffActual =NULL;
unsigned  long int       ulLen = NULL;
signed  long int       lIniPos = NULL;

rT00t1_stTpOpen         stTpOpen; 
rT00t5_stTpClose        stTpClose;
rT00t2_stTpRead         stTpRead;
/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (rs1506)
if (*pNode){
  pSnData = (rT1502_stSnData FAR *) ro0021_globallock (*pNode);
  if (pSnData) {
     lIniPos = ro0076_stwhere (hsStore);
     pStoreAddress = ro0069_stputs (hsStore,sizeof(long)); /* length */
     if (!pStoreAddress )
	 RM0003_RETURN (rs1506, signed short int, -R_USTERR )


     switch  (pSnData->iMemType){
       case  RC1500_Memory:
     /*       pStoreAddress = ro0069_stputs (hsStore,
					   pSnData->vLength[pSnData->lCount]);
      *** kc */
	    pBufferAddress = 
		     pSnData->unBufferRef.pBufRef + 
				pSnData->vOffset[pSnData->lCount] ;
	    if (pBufferAddress)
		rc = ro0068_stputm (hsStore, 
			     pBufferAddress, 
	     (unsigned long) pSnData->vLength[pSnData->lCount]);
	    /* ro0041_memcpy (pStoreAddress, 
			   pBufferAddress, 
			   pSnData->vLength[pSnData->lCount]);  */

	    ulLen = pSnData->vLength[pSnData->lCount];
	    break;
       case RC1502_Handle:
	    if ( (pBufferAddress = (char  *)            
	      ro0031_locallock (pSnData->unBufferRef.ulhandle)) != NULL){      
	      /*     pStoreAddress = ro0069_stputs (hsStore,
					  pSnData->vLength[pSnData->lCount]);
	       *****  kc */

		   pBufferAddress += pSnData->vOffset[pSnData->lCount] ;  

		   rc = ro0068_stputm (hsStore, 
			  pBufferAddress, 
		   (unsigned long) pSnData->vLength[pSnData->lCount]);
/* 
		   ro0041_memcpy (pStoreAddress, 
				  pBufferAddress,  
				   pSnData->vLength[pSnData->lCount]); 
  * ***** kc */
		   ulLen = pSnData->vLength[pSnData->lCount];
		   if (ro0032_localunlock (pSnData->unBufferRef.ulhandle) != 0) 
			 rc = -R_LULOKE;
	    }else rc = -R_LLOCKE;     

	    break;
       case RC1501_File:
	    if (pSnData->lCount == NULL) {
	      stTpOpen.bIgnore             =FALSE;                      
      /* **** by kc on 25-11-93 for Windows problem of movable mem. area ** */
      /* start */
              pSnData->unBufferRef.pTpduName = (char FAR *)
                                               ((char FAR *)pSnData +
                                               sizeof(rT1502_stSnData));
	      stTpOpen.pDataDescription    =pSnData->unBufferRef.pTpduName; 
                 
	      stTpOpen.usLenDataDescription=
					  (unsigned short int)pSnData->ulBufLen;
	      stTpOpen.iDir                =TPIN;                       

	      rc = ro00t1_tpOpen ((rT00t1_stTpOpen FAR *)&stTpOpen);                             
              if (rc) { /* break;  by kc on 4-5-94 for unlock error */   
                if (ro0022_globalunlock (*pNode)) rc = -R_GULOKE;        
                RM0003_RETURN (rs1502, signed short int, rc)             
             }                                                           
	      pSnData->sOpenId  = stTpOpen.sOpenId;
	    }/* end if it is the first count */
	     /* common code assuming  file is already opened  */

	   stTpRead.bIgnore             =FALSE;                      
	   stTpRead.sOpenId              =pSnData->sOpenId;
	   if((pSnData->lCount == 0) && (pSnData->vLength[pSnData->lCount]== 0))
	       stTpRead.lInRequiredDataLength=stTpOpen.lFileLength;
	   else 
	      stTpRead.lInRequiredDataLength=pSnData->vLength[pSnData->lCount];
	   ulLen =stTpRead.lInRequiredDataLength;
	   rc = ro00t7_tpduToStore ((rT00t2_stTpRead FAR *)&stTpRead, hsStore);
	   if (rc) break;                                       
  
   /***** if this was the last external called tthen the  file should  ****/
   /***** be closed  ********************/
	  if (pSnData->lCount == (pSnData->stPdvHeader.lFraCount-1) ) {
              /* kc 10-11-93 -1 in lFraCount is removed */
	     stTpClose.bIgnore = FALSE;                    
	     stTpClose.sOpenId = pSnData->sOpenId;       

	     rc = ro00t5_tpClose ((rT00t5_stTpClose FAR *)&stTpClose);
	  } /* end to i=PdvHandleCount */
          /* update the length as per the read in from user area *        
            * by kc on 4-5-94 */                                          
           ulLen =  ro0076_stwhere (hsStore) - lIniPos - sizeof(long);    
            break;                                                        
     }                                                                    

   if (rc != OK  )
     RM0003_RETURN (rs1506, signed short int, rc )
   pSnData->lCount++;
   rc = ro0066_storepos(hsStore, lIniPos);
   if (rc == OK) { 
     pBuffActual = ro0069_stputs(hsStore, sizeof (long));
     if (pBuffActual) {
	RM0012_ENCODEi (ulLen, unsigned long int, long) 
	rc = ro0066_storepos (hsStore , ( lIniPos+ulLen+sizeof(long)));
     }
     else rc = -R_USTERR;
   }
   if (ro0022_globalunlock (*pNode)) rc = -R_GULOKE;
  }else rc  = -R_GLOCKE;
}
RM0003_RETURN (rs1506, signed short int, rc )

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
signed   short int FAR  * psAbsId,
unsigned long  int        hsStore,
void               NEAR * pPpmContext,
unsigned long  int FAR  * phAct1Apdus)

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

/* EOF */
