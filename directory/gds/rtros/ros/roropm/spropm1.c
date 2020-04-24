/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spropm1.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:08  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:26  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:49  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:12  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:24  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:06:58  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:40:08  keutel
 * 	creation
 * 	[1994/03/21  13:35:42  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spropm1.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:08 $" } ;
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
/* NAME 	: spropm1.c					      */
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

static char *sccsid =    "@(#) spropm1.c 1.1 93/12/20   RTS-V2.00";

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
#include <rostor0.h>      /* Store interface                          */ 
#include <rodpat0.h>      /* Dispatcher interface                     */
#include <rodpat1.h>      /* Dispatcher en/de-coding interface        */

#include <roendeco.h>     /* Macros defs for PTS en/dec functions     */    
                                                                            
#include <roacpm0.h>                                                        
                                                                            
#include <roppm00.h>                                                        
#include <roppmzy.h>      /* PTS constant  definitions                */    
                                                                            
#include <rortpm0.h>                                                        
                                                                            
#include <roropm0.h>      /* Interface definitions                    */    
#include <roropm1.h>      /* PDU data structures                      */    
#include <roropm2.h>      /* Interface function definitions           */    
#include <roropm3.h>      /*                                          */    
#include <roropmzy.h>     /* PTS relates defs.                        */  
#include <roropzx.h>      /* Included while compiling on dos.-skh     */  
/*#include <asn1st.h> */
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
rTro01_pfvObjectId	  pTrfSynObjId,	  /*  -> : see rouply0.h */
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

/******
rTro00_vObjectId       vBerTrfSyn = {3, 2, 1, 1}; 
******/
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
           rc = RTS_any_dec(hsStore, ulLength,
                           (unsigned long int FAR *)phRoseApdus, 1,
                           (void NEAR *)pPpmContext);
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
                rc = ROSEapdus_dec (hsStore ,ulLen, pApdu, 
                      (void NEAR *)pPpmContext );
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

/* *********************** l o c a l    v a r i a b l e s ******************* */
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
            break;
       }  /* end switch lType  */
    

     /* call despatcher to switch decoding */ 
     if  (PdvNode )
       sRetCode = ro0243_decodSniP (RC0221_RoseUser, hStore, 
                                   (void NEAR *)pPpmContext,
                                   (unsigned long  int FAR *)PdvNode,
                                   (signed   short int FAR *)psAbsId); 
    }
    if (ro0022_globalunlock (*phRoseApdus) != OK)
        sRetCode  = -R_GLOCKE; 
    RM0003_RETURN (rs070b, signed short int, sRetCode)

}/*end-ro070b_decodSniPRopm*/

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
signed short int FAR PASCAL ro0704_clearNodeRopm (

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

