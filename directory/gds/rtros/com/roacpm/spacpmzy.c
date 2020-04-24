/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spacpmzy.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:43  root]
 * 
 * Revision 1.1.2.3  1994/06/09  19:01:01  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  19:00:29  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:47  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:45:45  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:40  keutel
 * 	creation
 * 	[1994/03/21  13:15:21  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spacpmzy.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:31 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1991                     */
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
/* NAME         : roacpmzy.c                                          */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24, 93-04-19                                  */
/*                                                                    */
/* COMPONENT    : ROS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : Encode/Decode  for  PPM  Private Transfer Syntax    */
/*                                                                    */
/*   This module provides functions which                             */
/*                                                                    */
/*     - encode the PPDU for private transfer syntax                  */
/*     - decode the PPDU for private transfer syntax.                 */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/*   R_G000_Windows : Compiler constant for MS-Windows                */
/*                    (and for MS-DOS if log utility)                 */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 2.50 K0  | 91-01-01|  original                      |hm  |         */
/* 3.00     | 93-04-19|                                |vp  |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <roerval.h>
#include <rolgval.h>
#include <rouply0.h>
#include <routil0.h>
#include <rostor0.h>
#include <roendeco.h>
#include <roacpm1.h>   /*  ACPM PPDU structures */
#include <roacpm3.h>   /*  ACPM PPDU structures */
#include <roppmzy.h>
#include <roacpmzy.h>


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : AARQSniP_type_cod                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL AARQSniP_type_cod (
unsigned long int        hStore,     
rT0641_aarqApdu   FAR  * x,
rT0650_acse       NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "AARQSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int rc=0;
char FAR *pBuffActual;

signed long int  lIniPos= LNULL;
signed long int  lLen   = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     pBuffActual = (char FAR *)ro0069_stputs ( hStore, 
                          sizeof (rT0641_aarqApdu));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

  /* ************* encode ACSE Mask Information ************ */

     RM0012_ENCODEi (RC04y7_MskAcse, RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
    
     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC0665_AARQapdu, RC06y0_Acse_apduType, char)
     lLen += sizeof(char);
    
     /* ******** encode Protocol Version **************** */

     RM0012_ENCODEi (x->ulProtoVersion, RC06y1_Aarq_protoVersion, char)  
     lLen += sizeof(char);

     /* ******** encode no. of elements in ************** */
     /* ******** appln context name ********************* */

     RM0012_ENCODEi (x->vApliCxtName [0], RC06y2_Aarq_appliContext, short)
     lLen += sizeof(short);

     /* ******** encode appln context name ************** */

     for ( iInx1 = 1;
           iInx1 < (signed int) x->vApliCxtName [0] + 1;
           iInx1++){

     RM0012_ENCODEi (x->vApliCxtName [iInx1], RC06y2_Aarq_appliContext, short)
     lLen += sizeof(short);
     }

     /* ******** encode no. of elements in ************** */
     /* ******** called AP Title ************************ */

     RM0012_ENCODEi (x->uCalledApTitle.vApTitleForm2 [0], 
                     RC06y3_Aarq_calledApTitl, short)
     lLen += sizeof(short);

     /* ******** encode called AP Title ***************** */

     for ( iInx1 = 1;
           iInx1 < (signed int) x->uCalledApTitle.vApTitleForm2 [0] + 1;
           iInx1++){

     RM0012_ENCODEi (x->uCalledApTitle.vApTitleForm2 [iInx1], 
                     RC06y4_Aarq_calingApTitl, short)
     lLen += sizeof(short);
     }

     /* ******** encode no. of elements in ************** */
     /* ******** calling AP Title *********************** */

     RM0012_ENCODEi (x->uCallingApTitle.vApTitleForm2 [0], 
                     RC06y4_Aarq_calingApTitl, short)
     lLen += sizeof(short);

     /* ******** encode calling AP Title **************** */

     for ( iInx1 = 1;
           iInx1 < (signed int) x->uCallingApTitle.vApTitleForm2 [0] + 1;
           iInx1++){

     RM0012_ENCODEi (x->uCallingApTitle.vApTitleForm2 [iInx1], 
                     RC06y4_Aarq_calingApTitl, short)
     lLen += sizeof(short);
     }

     if (ro0084_stStoreResize(hStore ,  (lLen + lIniPos) ) < LNULL)
          rc = -R_ASTERR;

     RM0003_RETURN (fnctName, signed short int, rc)
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : AARESniP_type_cod                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL AARESniP_type_cod (
unsigned long int        hStore,
rT0642_aareApdu   FAR  * x,
rT0650_acse       NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "AARESniP_type_cod")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int rc=0;
char FAR *pBuffActual;

signed long int  lIniPos= LNULL;
signed long int  lLen   = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)
     
     lIniPos = ro0076_stwhere (hStore);
     pBuffActual = (char FAR *)ro0069_stputs ( hStore, 
                          sizeof (rT0642_aareApdu ));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

  /* ************* encode ACSE Mask Information ************ */

     RM0012_ENCODEi (RC04y7_MskAcse, RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
    
     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC0666_AAREapdu, RC06y0_Acse_apduType, char)
     lLen += sizeof(char);
    
     /* ******** encode Protocol Version **************** */

     RM0012_ENCODEi ((x->ulProtoVersion >> 31 ), RC06y5_Aare_protoVersion, char)  
     lLen += sizeof(char);

     /* ******** encode no. of elements in ************** */
     /* ******** appln context name ********************* */

     RM0012_ENCODEi (x->vApliCxtName [0], RC06y6_Aare_appliContext, short)
     lLen += sizeof(short);

     /* ******** encode appln context name ************** */

     for ( iInx1 = 1;
           iInx1 < (signed int) x->vApliCxtName [0] + 1;
           iInx1++) {

     RM0012_ENCODEi (x->vApliCxtName [iInx1], RC06y6_Aare_appliContext, short)
     lLen += sizeof(short);
    }
     /* ******** encode association result ************** */

     RM0012_ENCODEi (x->lResult, RC06y7_Aare_associResult, char)  
     lLen += sizeof(char);

     /* ******** encode association result source ******* */

     RM0012_ENCODEi (x->lResultSourceDiag, RC06y8_Aare_resultSource, char)  
     lLen += sizeof(char);

     /* ******** encode association result diagnostic *** */
   
     if (x->lResultSourceDiag == RC0644_ResultSourceUser){
     RM0012_ENCODEi (x->uResultSourceDiag.lAcseServiceUser , 
                     RC06y8_Aare_diagnostic  , char)  
     }
     else{
     RM0012_ENCODEi (x->uResultSourceDiag.lAcseServiceProvider, 
                     RC06y8_Aare_diagnostic  , char)  
     }

     lLen += sizeof(char);
     /* ******** encode no. of elements in ************** */
     /* ******** Responding AP Title ******************** */
   
     RM0012_ENCODEi (x->uRespondApTitle.vApTitleForm2 [0], 
                     RC06ya_Aare_responApTitl, short)
     lLen += sizeof(short);
     for ( iInx1 = 1;
           iInx1 < (signed int) x->uRespondApTitle.vApTitleForm2 [0] + 1;
           iInx1++) {
    RM0012_ENCODEi ( x->uRespondApTitle.vApTitleForm2 [iInx1], 
                     RC06ya_Aare_responApTitl, short)
     lLen += sizeof(short);
   }

   if (ro0084_stStoreResize(hStore,  (lLen + lIniPos) ) < LNULL) rc = -R_ASTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : RLRQSniP_type_cod                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL RLRQSniP_type_cod (
unsigned long int        hStore,     
rT0643_rlrqApdu   FAR  * x,
rT0650_acse       NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "RLRQSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;

signed long int  lIniPos= LNULL;
signed long int  lLen   = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)
     pBuffActual = (char FAR *)ro0069_stputs ( hStore, 
                          sizeof (rT0643_rlrqApdu));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

  /* ************* encode ACSE Mask Information ************ */

     RM0012_ENCODEi (RC04y7_MskAcse, RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
    
     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC0667_RLRQapdu, RC06y0_Acse_apduType, char)
     lLen += sizeof(char);
    
     /* ******** encode Release Request Reason ********** */

     RM0012_ENCODEi (x->lReason, RC06yb_Rlrq_rlrqReason, char)
     lLen += sizeof(char);

     if (ro0084_stStoreResize(hStore, (lLen + lIniPos)) < LNULL) rc = -R_ASTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : RLRESniP_type_cod                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL RLRESniP_type_cod (
unsigned long int        hStore,     
rT0644_rlreApdu   FAR  * x,
rT0650_acse       NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "RLRESniP_type_cod")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;

signed long int  lIniPos= LNULL;
signed long int  lLen   = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)
     pBuffActual = (char FAR *)ro0069_stputs ( hStore, 
                          sizeof (rT0644_rlreApdu));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

  /* ************* encode ACSE Mask Information ************ */

     RM0012_ENCODEi (RC04y7_MskAcse, RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
    
     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC0668_RLREapdu, RC06y0_Acse_apduType, char)
     lLen += sizeof(char);
    
     /* ******** encode Release Request Reason ********** */

     RM0012_ENCODEi (x->lReason, RC06yc_Rlre_rlreReason, char)
     lLen += sizeof(char);

     if (ro0084_stStoreResize(hStore, (lLen + lIniPos)) < LNULL) rc = -R_ASTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ABRTSniP_type_cod                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ABRTSniP_type_cod (
unsigned long int        hStore,     
rT0645_abrtApdu   FAR  * x,
rT0650_acse       NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "ABRTSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int rc = OK;
char FAR *pBuffActual;

signed long int  lIniPos= LNULL;
signed long int  lLen   = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

     pBuffActual = (char FAR *)ro0069_stputs ( hStore, 
                          sizeof (rT0645_abrtApdu));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

  /* ************* encode ACSE Mask Information ************ */

     RM0012_ENCODEi (RC04y7_MskAcse, RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
    
     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC0669_ABRTapdu, RC06y0_Acse_apduType, char)
     lLen += sizeof(char);
    
     /* ******** encode Release Request Reason ********** */

     RM0012_ENCODEi (x->lAbortSource, RC06yd_Abrt_aboSource, char)
     lLen += sizeof(char);

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : AARQSniP_type_dec                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL AARQSniP_type_dec (
unsigned long int        hStore,
rT0641_aarqApdu   FAR  * x,
rT0650_acse       NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "AARQSniP_type_dec")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int rc=0;
char FAR *pBuffActual;
signed long int  lIniPos= LNULL;
signed long int  lLen   = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)
      
     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT0641_aarqApdu))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)


     /* ******** decode Protocol Version **************** */

     RM0013_DECODEi (x->ulProtoVersion, RC06y1_Aarq_protoVersion, 
                     char)
     lLen += sizeof(char);

     /* **** shift 31 bits left to match to new ********* */
     /* **** protocol version value of 0x80000000 to **** */
     /* **** old version value of 1 ********************* */

     (x->ulProtoVersion) = (x->ulProtoVersion) <<31;	  

     /* ******** decode no. of elements in ************** */
     /* ******** appln context name ********************* */

     RM0013_DECODEi (x->vApliCxtName [0], RC06y2_Aarq_appliContext, 
		     short)
     lLen += sizeof(short);

     /* ******** decode appln context name ************** */

     for ( iInx1 = 1;
           iInx1 < (signed int) x->vApliCxtName [0] + 1;
           iInx1++) {

        RM0013_DECODEi (x->vApliCxtName [iInx1], RC06y2_Aarq_appliContext, 
                     short)
        lLen += sizeof(short);
     }
     /* ******** decode no. of elements in ************** */
     /* ******** called AP Title ************************ */

     RM0013_DECODEi (x->uCalledApTitle.vApTitleForm2 [0], 
                     RC06y3_Aarq_calledApTitl, short)
     lLen += sizeof(short);
     if (x->uCalledApTitle.vApTitleForm2[0]){
         x->bCalledApTitle = TRUE;
         x->lCalledApTitle = RCro77_ObjId;
     }
     /* ******** decode called AP Title ***************** */

     for ( iInx1 = 1;
           iInx1 < (signed int) x->uCalledApTitle.vApTitleForm2 [0] + 1;
           iInx1++){

       RM0013_DECODEi (x->uCalledApTitle.vApTitleForm2 [iInx1], 
                     RC06y4_Aarq_calingApTitl, short)
       lLen += sizeof(short);
     }
     /* ******** decode no. of elements in ************** */
     /* ******** calling AP Title *********************** */

     RM0013_DECODEi (x->uCallingApTitle.vApTitleForm2 [0], 
                     RC06y4_Aarq_calingApTitl, short)
     lLen += sizeof(short);

     if (x->uCallingApTitle.vApTitleForm2[0]){
         x->bCallingApTitle = TRUE;
         x->lCallingApTitle = RCro77_ObjId;
     }
     /* ******** decode calling AP Title **************** */

     for ( iInx1 = 1;
           iInx1 < (signed int) x->uCallingApTitle.vApTitleForm2 [0] + 1;
           iInx1++){

       RM0013_DECODEi (x->uCallingApTitle.vApTitleForm2 [iInx1], 
                     RC06y4_Aarq_calingApTitl, short)
       lLen += sizeof(short);
     }

     if (ro0066_storepos(hStore, (lIniPos+lLen) ) < OK)   rc = -R_ASTERR;
     if (!rc)
         if ((lLen = ro0062_storelength(hStore)) < LNULL) rc = -R_ASTERR;
     if (!rc)
         if ((lIniPos = ro0076_stwhere( hStore)) < LNULL) rc = -R_ASTERR;
     if (!rc) {
         if ( (lLen - lIniPos) > 1 ){
            x->bUserInfo = TRUE;
            x->ulUserInfo = (long)1;
         }
         else {
            iInx1 =  ro0070_stgetc(hStore);
            if (iInx1 < OK) rc = -R_ASTERR;
         }
     }
 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : AARESniP_type_dec                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL AARESniP_type_dec (
unsigned long int        hStore,
rT0642_aareApdu   FAR  * x,
rT0650_acse       NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "AARESniP_type_dec")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int rc=0;
char FAR *pBuffActual;

signed long int  lIniPos= LNULL;
signed long int  lLen   = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT0642_aareApdu))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)
    

     /* ******** decode Protocol Version **************** */

     RM0013_DECODEi (x->ulProtoVersion, RC06y5_Aare_protoVersion, 
                     char)
     lLen += sizeof(char);

     /* **** shift 31 bits left to match to new ********* */
     /* **** protocol version value of 0x80000000 to **** */
     /* **** old version value of 1 ********************* */

     (x->ulProtoVersion) = (x->ulProtoVersion) <<31;	  

     /* ******** decode no. of elements in ************** */
     /* ******** appln context name ********************* */

     RM0013_DECODEi (x->vApliCxtName [0], RC06y6_Aare_appliContext, 
		     short)
     lLen += sizeof(short);

     /* ******** decode appln context name ************** */

     for ( iInx1 = 1;
           iInx1 < (signed int) x->vApliCxtName [0] + 1;
           iInx1++){

       RM0013_DECODEi (x->vApliCxtName [iInx1], RC06y6_Aare_appliContext, 
		     short)
       lLen += sizeof(short);
     }

     /* ******** decode association result ************** */

     RM0013_DECODEi (x->lResult, RC06y7_Aare_associResult, char)  
     lLen += sizeof(char);

     /* ******** decode association result source ******* */

     RM0013_DECODEi (x->lResultSourceDiag, RC06y8_Aare_resultSource, 
                     char)  
     lLen += sizeof(char);

     /* ******** decode association result source ******* */

     RM0013_DECODEi (x->lResultSourceDiag, RC06y8_Aare_resultSource, 
		     char)  
     lLen += sizeof(char);

     /* ******** decode association result diagnostic *** */
     if (x->lResultSourceDiag == RC0644_ResultSourceUser)
     RM0013_DECODEi (x->uResultSourceDiag.lAcseServiceUser , 
                     RC06y8_Aare_diagnostic  , short)  
     else
     RM0013_DECODEi (x->uResultSourceDiag.lAcseServiceProvider, 
                     RC06y8_Aare_diagnostic  , short )
  
     lLen += sizeof(short);
     /* ******** decode no. of elements in ************** */
     /* ******** Responding AP Title ******************** */
   
     RM0013_DECODEi (x->uRespondApTitle.vApTitleForm2 [0], 
                     RC06ya_Aare_responApTitl, short)
     lLen += sizeof(short);
     if ( x->uRespondApTitle.vApTitleForm2 [0]){
        x->bRespondApTitle = TRUE;
        x->lRespondAeQualifier = RCro77_ObjId;
     }
     for ( iInx1 = 1;
           iInx1 < (signed int) x->uRespondApTitle.vApTitleForm2 [0] + 1;
           iInx1++) {
       RM0013_DECODEi ( x->uRespondApTitle.vApTitleForm2 [iInx1], 
                     RC06ya_Aare_responApTitl, short)
       lLen += sizeof(short);
     }

     if (ro0066_storepos(hStore, (lIniPos+lLen) ) < LNULL) rc =  -R_ASTERR; 
     if (!rc)
         if ((lLen = ro0062_storelength(hStore)) < LNULL)  rc =  -R_ASTERR;
     if (!rc)
         if ((lIniPos = ro0076_stwhere( hStore)) < LNULL)  rc =  -R_ASTERR;
     if (!rc) {
         if ( (lLen - lIniPos) > 1 ) {
            x->bUserInfo = TRUE;
            x->ulUserInfo = (long)1;
         }
         else {
            if ((iInx1 =  ro0070_stgetc(hStore)) < OK) rc  = -R_ASTERR;
         }
     }

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : RLRQSniP_type_dec                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL RLRQSniP_type_dec (
unsigned long int        hStore,
rT0643_rlrqApdu   FAR  * x,
rT0650_acse       NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "RLRQSniP_type_dec")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;

signed long int  lIniPos= LNULL;
signed long int  lLen   = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos <  LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)
     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT0643_rlrqApdu))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

     /* ******** decode Release Request Reason ********** */

     RM0013_DECODEi (x->lReason, RC06yb_Rlrq_rlrqReason, char)
     lLen += sizeof(char);
     x->bReason = TRUE;

     if (ro0066_storepos(hStore, (lIniPos+lLen) ) < LNULL)   rc = -R_ASTERR; 
     if (!rc)
         if ((lLen = ro0062_storelength(hStore)) < LNULL)    rc = -R_ASTERR;
     if (!rc)
         if ((lIniPos = ro0076_stwhere( hStore)) < LNULL)    rc = -R_ASTERR;
     if (!rc)  {
         if ( (lLen - lIniPos) > 1 ) {
            x->bUserInfo = TRUE;
            x->ulUserInfo = (long)1;
         }
         else {
            lLen =  ro0070_stgetc(hStore);
            if (lLen < OK) rc  =  -R_ASTERR;
         }
     }

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : RLRESniP_type_dec                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL RLRESniP_type_dec (
unsigned long int        hStore,
rT0644_rlreApdu   FAR  * x,
rT0650_acse       NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "RLRESniP_type_dec")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;

signed long int  lIniPos= LNULL;
signed long int  lLen   = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT0644_rlreApdu))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

     /* ******** decode Release Request Reason ********** */

     RM0013_DECODEi (x->lReason, RC06yc_Rlre_rlreReason, 
                     char)
     lLen += sizeof(char);
     x->bReason = TRUE;

     if (ro0066_storepos(hStore, (lIniPos+lLen) ) < LNULL)   rc = -R_ASTERR;
     if (!rc)
         if ((lLen = ro0062_storelength(hStore)) < LNULL)    rc = -R_ASTERR;
     if (!rc)
         if ((lIniPos = ro0076_stwhere( hStore)) < LNULL)    rc = -R_ASTERR;
     if (!rc) {
         if ( (lLen - lIniPos) > 1 ){
            x->bUserInfo = TRUE;
            x->ulUserInfo = (long)1;
         }
         else {
            lLen =  ro0070_stgetc(hStore);
            if  (lLen < LNULL) rc = -R_ASTERR;
         }
     }
 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ABRTSniP_type_dec                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ABRTSniP_type_dec (
unsigned long int        hStore,
rT0645_abrtApdu   FAR  * x,
rT0650_acse       NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "ABRTSniP_type_dec")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;

signed long int  lIniPos= LNULL;
signed long int  lLen   = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT0645_abrtApdu))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_ASTERR)

     /* ******** decode Release Request Reason ********** */

     RM0013_DECODEi (x->lAbortSource, RC06yd_Abrt_aboSource, char)
     lLen += sizeof(char);

     if (ro0066_storepos(hStore, (lIniPos+lLen) ) <  LNULL)   rc = -R_ASTERR;
     if (!rc)
         if ((lLen = ro0062_storelength(hStore)) < LNULL)     rc = -R_ASTERR;
     if (!rc)
         if ((lIniPos = ro0076_stwhere( hStore)) < LNULL)     rc = -R_ASTERR;
     if (!rc) {
         if ( (lLen - lIniPos) > 1 ){
            x->bUserInfo = TRUE;
            x->ulUserInfo = (long)1;
         }
         else {
            lLen =  ro0070_stgetc(hStore);
            if (lLen <  LNULL) rc = -R_ASTERR;
         }
     }
 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */


/*  EOF */
