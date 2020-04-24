/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spropmzy.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:11  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:48  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:31  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:55  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:15  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:07:11  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:40:14  keutel
 * 	creation
 * 	[1994/03/21  13:35:53  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spropmzy.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:11 $" } ;
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
/* NAME         : roropmzy.c                                          */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24, 93-04-19                                  */
/*                                                                    */
/* COMPONENT    : ROS remote operations protocol machine              */
/*                                                                    */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : Encode/Decode  for  ROPM  Private Transfer Syntax   */
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
#include <roppmzy.h>
#include <roropm1.h>   /*  ROPM PPDU structures */
#include <roropm3.h>   /*  ROPM PPDU structures */
#include <roropmzy.h>


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ROIVSniP_type_cod                                       */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ROIVSniP_type_cod(
unsigned long int        hStore,
rT0710_stROIVapdu FAR  * x,
rT0730_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "ROIVSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int rc=0;
char FAR *pBuffActual;

signed long int      lLen  = LNULL;
signed long int      lIniPos = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     pBuffActual = (char FAR *) ro0069_stputs(hStore   ,
                                   sizeof(rT0710_stROIVapdu) ); 
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

  /* ************* encode ACSE Mask Information ************ */
     /* not sure which mask to use ?? Octet or BER */

     RM0012_ENCODEi (RC04y8_MskRoseOctet, RC04yb_sPrivPciId, char)
     lLen += sizeof(char);

     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC07yA_RoivApdu, RC07y0_Rose_apduType, char)
     lLen += sizeof(char);
    
     /* ******** encode Invoke  Id ********************** */

     RM0012_ENCODEi (x->lInvokeID, RC07y1_Roiv_invokeId, short)
       lLen += sizeof(short);
                                                            
     /* ******** encode Invoke  Id ********************** */

     RM0012_ENCODEi (x->bLinkedID, RC07y2_Roiv_linkedVa, char)
     lLen += sizeof(char);
     if (x->bLinkedID ){
         RM0012_ENCODEi (x->lLinkedID, RC07y3_Roiv_linkedId, short)  
         lLen += sizeof(short);
     }

     /* ******** encode Operation  Type ***************** */

     RM0012_ENCODEi (x->stOperationValue.lType, RC07y4_Roiv_operType, char)  
     lLen += sizeof(char);

     if (x->stOperationValue.lType == RC047a_DefinedByInteger){
         RM0012_ENCODEi (x->stOperationValue.unValue.lInteger, 
        		     RC07y5_Roiv_localValue, short)
         lLen += sizeof(short);
     }
     else {
     RM0012_ENCODEi (x->stOperationValue.unValue.oObjectId[0], 
                     RC07y6_Roiv_globlValue, short)
     lLen += sizeof(short);
     for ( iInx1 = 1;
           iInx1 < (signed int) x->stOperationValue.unValue.oObjectId[0]+1;
           iInx1++) {
        RM0012_ENCODEi (x->stOperationValue.unValue.oObjectId [iInx1], 
		     RC07y6_Roiv_globlValue, short)
        lLen += sizeof(short);
     }
     }/* encode object ID Operation Value */

     if (ro0066_storepos(hStore ,  (lLen + lIniPos) ) < LNULL) rc = -R_OSTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : RORSSniP_type_cod                                       */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL RORSSniP_type_cod(
unsigned long int        hStore,
rT0712_stRORSapdu FAR  * x,
rT0730_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "RORSSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int rc=0;
char FAR *pBuffActual;

signed long int      lLen  = LNULL;
signed long int      lIniPos = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     pBuffActual = (char FAR *) ro0069_stputs(hStore   ,
                                   sizeof(rT0712_stRORSapdu) ); 
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

  /* ************* encode ROSE Mask Information ************ */
     /* not sure which mask to use ?? Octet or BER */

     RM0012_ENCODEi (RC04y8_MskRoseOctet, RC04yb_sPrivPciId, char)
     lLen += sizeof(char);

     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC07yB_RorsApdu, RC07y0_Rose_apduType, char)
     lLen += sizeof(char);
    
     /* ******** encode Invoke  Id ********************** */

     RM0012_ENCODEi (x->lInvokeID, RC07y7_Rors_invokeId, short)
     lLen += sizeof(short);
    
     /* ******** encode Operation Result **************** */

     RM0012_ENCODEi (x->stOperationResult.stOperationValue.lType , 
                     RC07y8_Rors_operType, char)  
     lLen += sizeof(char);

     if (x->stOperationResult.stOperationValue.lType 
                       == RC047a_DefinedByInteger){
        RM0012_ENCODEi (x->stOperationResult.stOperationValue.unValue.lInteger, 
		     RC07y5_Roiv_localValue, short)
       lLen += sizeof(short);
     }
     else {
     RM0012_ENCODEi (x->stOperationResult.stOperationValue.unValue.oObjectId[0],
                     RC07ya_Rors_globlValue, short)
     lLen += sizeof(short);
     for ( iInx1 = 1;
           iInx1 < (signed int) x->stOperationResult.stOperationValue.
                                   unValue.oObjectId[0]+1;
           iInx1++){
        RM0012_ENCODEi (x->stOperationResult.stOperationValue.unValue.
                     oObjectId [iInx1],RC07ya_Rors_globlValue,short) 
        lLen += sizeof(short);
     }
     } /* end object identifier Operation Result Value */

     if (ro0066_storepos(hStore ,  (lLen + lIniPos) ) < LNULL) rc  = -R_OSTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ROERSniP_type_cod                                       */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ROERSniP_type_cod(
unsigned long int        hStore,
rT0713_stROERapdu FAR  * x,
rT0730_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "ROERSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int rc=0;
char FAR *pBuffActual;

signed long int      lLen  = LNULL;
signed long int      lIniPos = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     pBuffActual = (char FAR *) ro0069_stputs(hStore   ,
                                   sizeof(rT0713_stROERapdu) ); 
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

  /* ************* encode ROSE Mask Information ************ */
     /* not sure which mask to use ?? Octet or BER */

     RM0012_ENCODEi (RC04y8_MskRoseOctet, RC04yb_sPrivPciId, char)
     lLen += sizeof(char);

     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC07yC_RoerApdu, RC07y0_Rose_apduType, char)
     lLen += sizeof(char);
    
     /* ******** encode Invoke  Id ********************** */

     RM0012_ENCODEi (x->lInvokeID, RC07yb_Roer_invokeId, short)
       lLen += sizeof(short);
    
     /* ******** encode Operation Error Value *********** */

     RM0012_ENCODEi (x->stErrorValue.lType, RC07yc_Roer_operType, char)  
     lLen += sizeof(char);

     if (x->stErrorValue.lType == RC047a_DefinedByInteger){
       RM0012_ENCODEi (x->stErrorValue.unValue.lInteger, 
		     RC07yd_Roer_localValue, short)
       lLen += sizeof(short);
     }
     else {
     RM0012_ENCODEi (x->stErrorValue.unValue.oObjectId[0], 
                     RC07ye_Roer_globlValue, short)
     lLen += sizeof(short);
     for ( iInx1 = 1;
           iInx1 < (signed int) x->stErrorValue.unValue.oObjectId[0]+1;
           iInx1++){
       RM0012_ENCODEi (x->stErrorValue.unValue.oObjectId [iInx1], 
		     RC07ye_Roer_globlValue, short)
       lLen += sizeof(short);
     }
     }/* encode object ID Operation Value */

     if (ro0066_storepos(hStore ,  (lLen + lIniPos) ) < LNULL) rc = -R_OSTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : RORJSniP_type_cod                                       */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL RORJSniP_type_cod(
unsigned long int        hStore,
rT0714_stRORJapdu FAR  * x,
rT0730_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "RORJSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;

signed long int      lLen  = LNULL;
signed long int      lIniPos = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     pBuffActual = (char FAR *) ro0069_stputs(hStore   ,
                                   sizeof(rT0714_stRORJapdu) ); 
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

  /* ************* encode ROSE Mask Information ************ */
     /* not sure which mask to use ?? Octet or BER */

     RM0012_ENCODEi (RC04y8_MskRoseOctet, RC04yb_sPrivPciId, char)
     lLen += sizeof(char);

     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC07yD_RorjApdu, RC07y0_Rose_apduType, char)
     lLen += sizeof(char);
    
     /* ******** encode Invoke  Id ********************** */

     RM0012_ENCODEi (x->lInvokeIDChoiceNum, RC07yf_Rorj_invokeVa, char)
     lLen += sizeof(char);
     if (x->lInvokeIDChoiceNum) {
       RM0012_ENCODEi (x->lInvokeID, RC07yg_Rorj_invokeId, short)  
       lLen += sizeof(short);
     }
     /* ******** encode Reject Problem Type ************* */

     RM0012_ENCODEi (x->lProblemChoiceNum, RC07yh_Rorj_probType, char)
     lLen += sizeof(char);

     /* ******** encode Reject Problem Type ************* */

     RM0012_ENCODEi (x->lProblem , RC07yi_Rorj_problem , char)  
     lLen += sizeof(char);

     if (ro0066_storepos(hStore ,  (lLen + lIniPos+sizeof(long)) ) < LNULL)
         rc  =  -R_OSTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
}/* end */

/* ****************************************************************** */
/* ************** ROPM  PTS  DECODE FUNCTION ************************ */
/* ****************************************************************** */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ROIV_type_dec                                       */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ROIV_type_dec(
unsigned long int        hStore,
rT0710_stROIVapdu FAR  * x,
rT0730_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "ROIV_type_dec")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int rc=0;
char FAR *pBuffActual =NULL;

signed long int      lLen = LNULL;
signed long int      lIniPos = LNULL;
unsigned long int      ulUserDLen = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT0710_stROIVapdu))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     /* ******** decode Invoke  Id ********************** */

     RM0013_DECODEi (x->lInvokeID, RC07y1_Roiv_invokeId, short)
     lLen += sizeof(short);                                                            
     /* ******** decode Invoke  Id ********************** */

     RM0013_DECODEi (x->bLinkedID, RC07y2_Roiv_linkedVa, char)
     lLen += sizeof(char);
     if (x->bLinkedID) {
       RM0013_DECODEi (x->lLinkedID, RC07y3_Roiv_linkedId, short)  
       lLen += sizeof(short); 
     }                                                           
     

     /* ******** decode Operation  Type ***************** */

     RM0013_DECODEi (x->stOperationValue.lType, RC07y4_Roiv_operType, 
                     char)  
     lLen += sizeof(char);                                                            

     if (x->stOperationValue.lType == RC047a_DefinedByInteger) {
     RM0013_DECODEi (x->stOperationValue.unValue.lInteger, 
		     RC07y5_Roiv_localValue, short)
     lLen += sizeof(short);                      
     }
     else {
     RM0013_DECODEi (x->stOperationValue.unValue.oObjectId[0], 
                     RC07y6_Roiv_globlValue, short)
     lLen += sizeof(short);                                                            
       for ( iInx1 = 1;
           iInx1 < (signed int) x->stOperationValue.unValue.oObjectId[0]+1;
           iInx1++) {
         RM0013_DECODEi (x->stOperationValue.unValue.oObjectId [iInx1], 
		     RC07y6_Roiv_globlValue,short)
         lLen += sizeof(short); 
       }
     }/* decode object ID Operation Value */
     RM0013_DECODEi (ulUserDLen , unsigned long int  , long )
 
     if ( ulUserDLen  ) {
        x->bArgument = TRUE;
        if (ro0066_storepos(hStore, (lIniPos+lLen) ) < LNULL) rc = -R_OSTERR;
     }else {
        x->bArgument = FALSE;
        if (ro0066_storepos(hStore, (lIniPos+lLen+sizeof(long))) < LNULL)
            rc = -R_OSTERR;
     }

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : RORS_type_dec                                       */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL RORS_type_dec(
unsigned long int        hStore,
rT0712_stRORSapdu FAR  * x,
rT0730_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "RORS_type_dec")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int rc=0;
char FAR *pBuffActual;

signed long int      lLen = LNULL;
signed long int      lIniPos = LNULL;
unsigned long int      ulUserDLen = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT0712_stRORSapdu))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     /* ******** decode Invoke  Id ********************** */

     RM0013_DECODEi (x->lInvokeID, RC07y7_Rors_invokeId, short)
     lLen += sizeof(short);
    
     /* ******** decode Operation Result **************** */
     /* !!! */

     RM0013_DECODEi (x->stOperationResult.stOperationValue.lType , 
                     RC07y8_Rors_operType, char)  
     lLen += sizeof(char);

     if (x->stOperationResult.stOperationValue.lType 
                             == RC047a_DefinedByInteger){
       RM0013_DECODEi (x->stOperationResult.stOperationValue.unValue.lInteger, 
		     RC07y5_Roiv_localValue, short )
       lLen += sizeof(short);
     }
     else {
     RM0013_DECODEi (x->stOperationResult.stOperationValue.unValue.oObjectId[0],
                     RC07ya_Rors_globlValue, short )
     lLen += sizeof(short);

     for ( iInx1 = 1;
           iInx1 < (signed int) x->stOperationResult.stOperationValue.
                                   unValue.oObjectId[0]+1;
           iInx1++){
       RM0013_DECODEi (x->stOperationResult.stOperationValue.unValue.
                     oObjectId [iInx1],RC07ya_Rors_globlValue,short) 
       lLen += sizeof(short);
      }
      
     } /* end object identifier Operation Result Value */
       RM0013_DECODEi (ulUserDLen , unsigned long int ,long) 
     if (!ulUserDLen){
            lLen += sizeof(long);
            x->bOperationResult = FALSE; 
     }else x->bOperationResult = TRUE;     
     
     if (ro0066_storepos(hStore, (lIniPos+lLen) ) < LNULL) rc =  -R_OSTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ROER_type_dec                                       */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ROER_type_dec(
unsigned long int        hStore,
rT0713_stROERapdu FAR  * x,
rT0730_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "ROER_type_dec")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int rc=0;
char FAR *pBuffActual;

signed long int      lLen = LNULL;
signed long int      lIniPos = LNULL;
unsigned long int      ulUserDLen = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT0713_stROERapdu))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     /* ******** decode Invoke  Id ********************** */

     RM0013_DECODEi (x->lInvokeID, RC07yb_Roer_invokeId, short)
     lLen += sizeof(short);
    
     /* ******** decode Operation Error Value *********** */

     RM0013_DECODEi (x->stErrorValue.lType, RC07yc_Roer_operType, 
                     char)  
     lLen += sizeof(char);

     if (x->stErrorValue.lType == RC047a_DefinedByInteger){
       RM0013_DECODEi (x->stErrorValue.unValue.lInteger, 
		     RC07yd_Roer_localValue, short)
       lLen += sizeof(short);
     }
     else {
     RM0013_DECODEi (x->stErrorValue.unValue.oObjectId[0], 
                     RC07ye_Roer_globlValue, short)
     lLen += sizeof(short);
     for ( iInx1 = 1;
           iInx1 < (signed int) x->stErrorValue.unValue.oObjectId[0]+1;
           iInx1++){
       RM0013_DECODEi (x->stErrorValue.unValue.oObjectId [iInx1], 
		     RC07ye_Roer_globlValue, short )
       lLen += sizeof(short);
     }

     }/* decode object ID Operation Value */
     RM0013_DECODEi (ulUserDLen , unsigned long int  , long )
     if ( ulUserDLen  ) {
        x->bParameter = TRUE;
        if (ro0066_storepos(hStore , (lIniPos+lLen) ) < LNULL) rc = -R_OSTERR;
     }else {
        x->bParameter = FALSE;
        if (ro0066_storepos(hStore , (lIniPos+lLen+sizeof(long)) ) < LNULL)
            rc = -R_OSTERR;
     }

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : RORJ_type_dec                                       */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL RORJ_type_dec(
unsigned long int        hStore,
rT0714_stRORJapdu FAR  * x,
rT0730_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "RORJ_type_dec")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;

signed long int      lLen = LNULL;
signed long int      lIniPos = LNULL;
unsigned long int      ulUserDLen = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT0714_stRORJapdu))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_OSTERR)

     /* ******** decode Invoke  Id ********************** */

     RM0013_DECODEi (x->lInvokeIDChoiceNum, RC07yf_Rorj_invokeVa, 
                     char  )
     lLen += sizeof(char);
     if (x->lInvokeIDChoiceNum){
       RM0013_DECODEi (x->lInvokeID, RC07yg_Rorj_invokeId, short)  
       lLen += sizeof(short);
     }

     /* ******** decode Reject Problem Type ************* */

     RM0013_DECODEi (x->lProblemChoiceNum, RC07yh_Rorj_probType, 
                     char )
     lLen += sizeof(char);

     /* ******** decode Reject Problem Type ************* */

     RM0013_DECODEi (x->lProblem , RC07yi_Rorj_problem , char)  
     lLen += sizeof(char);
     RM0013_DECODEi (ulUserDLen , unsigned long int  , long )
     lLen += sizeof(long);

     if (ro0066_storepos(hStore , (lIniPos+lLen) ) < LNULL) rc = -R_OSTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
}/* end */

/* EOF */
