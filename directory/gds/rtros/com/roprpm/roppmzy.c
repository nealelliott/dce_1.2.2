/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppmzy.c,v $
 * Revision 1.1.4.2  1996/02/18  18:23:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:14:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:10  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:08  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:20  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:16  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:52:28  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:52  keutel
 * 	creation
 * 	[1994/03/21  13:21:39  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roppmzy.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:23:00 $" } ;
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
/* NAME         : roppmzy.c                                           */
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
#include <rodpat0.h>
#include <roendeco.h>
#include <roppm01.h>   /* PPM PPDU structures                */
#include <roppm04.h>   /* PPM Entity Context                 */
#include <roppm03.h>   /* PPM Context Block                  */
#include <roppm05.h>   /* E/D Control functions Protos       */
#include <roppm13.h>   /* PPM Context Handling  Dec.  Protos */
#include <roppmzy.h>   /* PPM intern: SniPrivate definitions */
#include <roppmzw.h>   /* PPM intern: SniPrivate prototyping */

/**********************************************************************/
/*                                                                    */
/*              S T A T I C   D A T A                                 */
/*                                                                    */
/**********************************************************************/

static  unsigned char rT04y0_vModeSelector [RC04y0_PriLengthModeSelector] =
                        {0x31,         /*SET */              
          		 0x80,         /*indefinite length */
                         0xa0,         /*SET */              
                         0x03,         /*length */           
                         0x80,         /*INTEGER */          
                         0x01,         /*length */           
                         0xff};        /*mode selector -1 */ 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : CPSniP_type_cod                                     */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL CPSniP_type_cod(
	unsigned long  int        hStore,
	rT041A_stCp        FAR  * x,
	rT0430_stContext   NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "CPSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int iInx2;
signed short int iInx3;
signed short int rc=0;
char              FAR *pBuffActual;
rT0410_stPdvList       stLocalPdvList;
signed short  int      sPmNumb;
rTro01_pfvObjectId     pTransSynName;

signed long int      lLen  = LNULL;
signed long int      lIniPos = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     RM0003_RETURN (fnctName, signed short int, -R_NSUPCA)
        /* Not Supported */
     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *) ro0069_stputs(hStore   ,
                                   sizeof(rT041B_stCpa) ); 
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     /* ************* mode selector encode ************** */

     for (iInx1 = 0;                                             
          iInx1 < RC04y0_PriLengthModeSelector;                        
          iInx1++)                                               
         /* For HP porting by kc on 19-08-93 
           (unsigned char)*(pBuffActual + iInx1) = 
                       (unsigned char) rT04y0_vModeSelector [iInx1]; */
         *(pBuffActual + iInx1) = 
                       (unsigned char) rT04y0_vModeSelector [iInx1];
           pBuffActual += RC04y0_PriLengthModeSelector;
           lLen += RC04y0_PriLengthModeSelector;
	  
     /* ************* protocol version encode *********** */

     RM0012_ENCODEi (RC04y1_PriVersion1, RC04ya_sPrivVersion, char)
     lLen += sizeof(char);

     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC04y2_MskPCp , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
    
     /* ********* encode Calling Selector length ******** */

     RM0012_ENCODEi (x->ulCallingSelector,RC04yc_ulCallingSelectorLength, char)  
     lLen += sizeof(char);
     /* ********* encode Calling Selector *************** */
     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulCallingSelector;
           iInx1++)

     *(pBuffActual + iInx1) = x->vCallingSelector [iInx1];
     pBuffActual += iInx1;
     lLen += iInx1;

     /* ********* encode Called  Selector length ******** */

     RM0012_ENCODEi (x->ulCalledSelector, RC04yd_ulCalledSelectorLength, char)  
     lLen += sizeof(char);

     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulCalledSelector;
           iInx1++)
     *(pBuffActual + iInx1) = x->vCalledSelector [iInx1];
     pBuffActual += iInx1;
     lLen += iInx1;

     /* ********* encode number of Pres Cxt Definitions * */

     RM0012_ENCODEi (x->ulPresContDef, RC04ye_lPresContDef, char)  
     lLen += sizeof(char);

     /* ********* encode Presentation Context Id ******** */
     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulPresContDef;
           iInx1++)
     {
     RM0012_ENCODEi (x->vPresContDef[iInx1].lPresContId, 
                     RC04yf_vPresContId, short)
       lLen += sizeof(short);

     /* ********* encode no. of elements in AbSynName *** */

     RM0012_ENCODEi (x->vPresContDef[iInx1].oAbstrSynName[0], 
                     RC04yg_vAbstrSynName, short)
       lLen += sizeof(short);

     /* ********* encode Abstract Syntax Name *********** */
     for ( iInx2 = 1;
           iInx2 < (signed int) x->vPresContDef[iInx1].oAbstrSynName[0]+1;
           iInx2++){
    
     RM0012_ENCODEi (x->vPresContDef[iInx1].oAbstrSynName[iInx2], 
                     RC04yg_vAbstrSynName, short)
       lLen += sizeof(short);
     }

     /* ****** encode no. of transfer syntaxes ********** */
     /* ********** per  this abstract syntax ************ */

     RM0012_ENCODEi (x->vPresContDef[iInx1].ulTransSynNumber, 
                     RC04yh_vTransSynNumber, char)
     lLen += sizeof(char);

     for ( iInx2 = 0;
           iInx2 < (signed int) x->vPresContDef[iInx1].ulTransSynNumber; 
           iInx2++)
     {
     /* ****** encode no. of elements in T.Syn Name  **** */

      RM0012_ENCODEi (x->vPresContDef[iInx1].vTransSynName[iInx2] [0], 
                      RC04yi_vTransSynName, short)
       lLen += sizeof(short);

      for ( iInx3 = 1;
            iInx3 <(signed int)x->vPresContDef[iInx1].vTransSynName[iInx2][0]+1;
            iInx3++) {
      RM0012_ENCODEi (x->vPresContDef[iInx1].vTransSynName[iInx2][iInx3], 
                      RC04yi_vTransSynName, short)
       lLen += sizeof(short);
       }
      } /* for transfer syntax copy */
     } /*to "for lAbstrSynNumber" */
     if ( x->bUserData ) {
     rc = ro0411_encodePdvList((rT0410_stPdvList         FAR *)&stLocalPdvList,
			       (rTro01_pfvObjectId	 FAR *)&pTransSynName ,
                               (signed short  int        FAR *)&sPmNumb ,
	                        pContext,
                                x->stUserData.unUserData.stFully.vPdvList[0]);
     if (rc == OK) {
       RM0012_ENCODEi (stLocalPdvList.lPresContId, RC04yf_vPresContId, char)
       lLen += sizeof(char);
     }
    }
    if (!rc){  
       if (ro0084_stStoreResize(hStore, (lLen + lIniPos)) < LNULL) rc=-R_PSTERR;
    }

 RM0003_RETURN (fnctName, signed short int, rc)

}/* end */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : CPASniP_type_cod                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL CPASniP_type_cod(
	unsigned long  int        hStore,
	rT041B_stCpa       FAR  * x,
	rT0430_stContext   NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "CPASniP_type_cod")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int iInx2;
signed short int rc=0;
char              FAR *pBuffActual;
rT0410_stPdvList       stLocalPdvList;
signed short  int      sPmNumb;
rTro01_pfvObjectId     pTransSynName;

signed long int      lLen  = LNULL;
signed long int      lIniPos = LNULL;

  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)
     /* get the mem.  area from store to write */
     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *) ro0069_stputs(hStore   ,
                                   sizeof(rT041B_stCpa) ); 
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     /* ************* mode selector encode ************** */

     for (iInx1 = 0;                                             
          iInx1 < RC04y0_PriLengthModeSelector;                        
          iInx1++)                                               
         /* For HP porting by kc on  19-08-93 
           (unsigned char) *(pBuffActual + iInx1) = 
                        (unsigned char) rT04y0_vModeSelector [iInx1]; */

         *(pBuffActual + iInx1) = 
                        (unsigned char) rT04y0_vModeSelector [iInx1]; 
           pBuffActual += RC04y0_PriLengthModeSelector;
           lLen += RC04y0_PriLengthModeSelector;
	  
     /* ************* protocol version encode *********** */

     RM0012_ENCODEi (RC04y1_PriVersion1, RC04ya_sPrivVersion, char)
     lLen += sizeof(char);
     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC04y3_MskPCpa , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
    
     /* ****** encode Responding Selector length ******** */

     RM0012_ENCODEi (x->ulRespondingSelector, 
                     RC04yj_lRespondingSelectorLength, char)  
     lLen += sizeof(char);

     /* ****** encode Responding Selector *************** */

     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulRespondingSelector;
           iInx1++)
     *(pBuffActual + iInx1) = x->vRespondingSelector [iInx1];
     pBuffActual += iInx1;
     lLen += iInx1;

     /* ****** encode no. of Presentation Context ******* */
     /* ****** Definition Results  ********************** */

     RM0012_ENCODEi (x->ulPresContDefRes, RC04yk_lPresContDefRes, char)  
     lLen += sizeof(char);

     /* ****** encode Presentation Context ************** */
     /* ****** Definition Results  ********************** */

     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulPresContDefRes;
           iInx1++)
     {
       RM0012_ENCODEi (x->vPresContDefRes[iInx1].lResult, 
		       RC04yl_vResult, short)  
       lLen += sizeof(short);

     /* ****** encode no. of elements in TransSynName *** */

       RM0012_ENCODEi (x->vPresContDefRes[iInx1].oTransSynName [0], 
                       RC04yi_vTransSynName, short)
       lLen += sizeof(short);

     /* ****** encode TransSynName ********************** */

       for ( iInx2 = 1;
             iInx2 < (signed int) x->vPresContDefRes[iInx1].oTransSynName[0]+1;
             iInx2++){

            RM0012_ENCODEi (x->vPresContDefRes[iInx1].oTransSynName[iInx2], 
                            RC04yi_vTransSynName, short)
            lLen += sizeof(short);
       }

       RM0012_ENCODEi (x->vPresContDefRes[iInx1].lProviderReason ,
                       RC04ym_vProviderReason, short)  
       lLen += sizeof(short);

     }/* to  no. of Presentation context definition results */
     if (x->bUserData) {
     rc = ro0411_encodePdvList ((rT0410_stPdvList         FAR *)&stLocalPdvList,
				(rTro01_pfvObjectId	  FAR *)&pTransSynName ,
                                (signed  short int        FAR *)&sPmNumb ,
	                         pContext,
                                 x->stUserData.unUserData.stFully.vPdvList[0]);

     RM0012_ENCODEi (stLocalPdvList.lPresContId, RC04yf_vPresContId, char)
     lLen += sizeof(char);
     } 

     if (!rc)
         if (ro0084_stStoreResize(hStore, (lLen+lIniPos)) < LNULL) rc=-R_PSTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : CPRSniP_type_cod                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL CPRSniP_type_cod(
	unsigned long  int        hStore,
	rT041C_stCpr       FAR  * x,
	rT0430_stContext   NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "CPRSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int iInx2;
signed short int rc=0;
char              FAR *pBuffActual;
rT0410_stPdvList       stLocalPdvList;
signed short  int      sPmNumb;
rTro01_pfvObjectId     pTransSynName;

signed long int      lLen  = LNULL;
signed long int      lIniPos = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)
     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *) ro0069_stputs(hStore   ,
                                   sizeof(rT041C_stCpr) ); 
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     /* ************* protocol version encode *********** */

     RM0012_ENCODEi (RC04y1_PriVersion1, RC04ya_sPrivVersion, char)
     lLen += sizeof(char);

     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC04y4_MskPCpr , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
    
     /* ****** encode Responding Selector length ******** */

     RM0012_ENCODEi (x->ulRespondingSelector, 
                     RC04yj_lRespondingSelectorLength, char)  
     lLen += sizeof(char);

     /* ****** encode Responding Selector *************** */

     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulRespondingSelector;
           iInx1++)
     *(pBuffActual + iInx1) = x->vRespondingSelector [iInx1];
     pBuffActual += iInx1;
     lLen += iInx1;

     /* ****** encode no. of Presentation Context ******* */
     /* ****** Definition Results  ********************** */

     RM0012_ENCODEi (x->ulPresContDefRes, RC04yk_lPresContDefRes, char)  
     lLen += sizeof(char);

     /* ****** encode Presentation Context ************** */
     /* ****** Definition Results  ********************** */

     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulPresContDefRes;
           iInx1++)
     {
       RM0012_ENCODEi (x->vPresContDefRes[iInx1].lResult, 
		       RC04yl_vResult, short)  
       lLen += sizeof(short);

     /* ****** encode no. of elements in TransSynName *** */

       RM0012_ENCODEi (x->vPresContDefRes[iInx1].oTransSynName [0], 
                       RC04yi_vTransSynName, short)
       lLen += sizeof(short);

     /* ****** encode TransSynName ********************** */

       for ( iInx2 = 1;
             iInx2 < (signed int) x->vPresContDefRes[iInx1].oTransSynName[0]+1;
             iInx2++){

       RM0012_ENCODEi (x->vPresContDefRes[iInx1].oTransSynName[iInx2], 
                       RC04yi_vTransSynName, short)
       lLen += sizeof(short);
      }

       RM0012_ENCODEi (x->vPresContDefRes[iInx1].lProviderReason ,
                       RC04ym_vProviderReason, short)  
       lLen += sizeof(short);

     }/* to  no. of Presentation context definition results */

     RM0012_ENCODEi (x->bProviderReason, RC04ym_vProviderReason, char)  
     lLen += sizeof(char);
     if ( x->bProviderReason) {
        RM0012_ENCODEi (x->lProviderReason, RC04ym_vProviderReason, char)  
        lLen += sizeof(char);
     }
     if (x->bUserData) {
     rc = ro0411_encodePdvList  ((rT0410_stPdvList        FAR *)&stLocalPdvList,
				 (rTro01_pfvObjectId	  FAR *)&pTransSynName ,
                                 (signed short int        FAR *)&sPmNumb       ,
	                          pContext,
                                  x->stUserData.unUserData.stFully.vPdvList[0]);

     RM0012_ENCODEi (stLocalPdvList.lPresContId, RC04yf_vPresContId, char)
       lLen += sizeof(char);
     }
     if (!rc)
         if (ro0084_stStoreResize(hStore, (lLen+lIniPos)) < LNULL) rc=-R_PSTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ABUSniP_type_cod                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ABUSniP_type_cod(
	unsigned long  int        hStore,
	rT041D_stAru       FAR  * x,
	rT0430_stContext   NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "ABUSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;
rT0410_stPdvList       stLocalPdvList;
signed short  int      sPmNumb;
rTro01_pfvObjectId     pTransSynName;


signed long int      lLen  = LNULL;
signed long int      lIniPos = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *) ro0069_stputs(hStore   ,
                                   sizeof(rT041D_stAru) ); 
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     /* ************* encode Mask Information *********** */
    /* start */
     RM0012_ENCODEi (RC04y1_PriVersion1 , RC04ya_sPrivVersion, char)
     lLen += sizeof(char);    /* by kc on 06-12-93 */

     RM0012_ENCODEi (RC04y5_MskPAbo , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
     RM0012_ENCODEi (FALSE , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);

     if (x->bUserData) {
      switch(x->stUserData.lSimplyFullyChoice){
        case RC0417_SimplyEncoded:
             rc= ro0410_encodeSimply (
                       (rTro01_pfvObjectId       FAR *)&pTransSynName ,
                       (signed  short int        FAR *)&sPmNumb , 
                       pContext,
                       x->stUserData.unUserData.hSimply);           
             if (rc)
                 RM0003_RETURN (fnctName, signed short int, -R_PSTERR)
             break;
        case RC0418_FullyEncoded:
             rc = ro0411_encodePdvList ((
                                rT0410_stPdvList         FAR *)&stLocalPdvList,
				(rTro01_pfvObjectId	  FAR *)&pTransSynName ,
                                (signed  short int        FAR *)&sPmNumb ,
	                         pContext,
                                 x->stUserData.unUserData.stFully.vPdvList[0]);
             if (rc)
                 RM0003_RETURN (fnctName, signed short int, -R_PSTERR)
             RM0012_ENCODEi (
                   stLocalPdvList.lPresContId, RC04yf_vPresContId, char)
             lLen += sizeof(char);
             break;
        default:
             RM0003_RETURN (fnctName, signed short int, 1)
             break;
      }
     }
     if (!rc)
         if (ro0084_stStoreResize(hStore, (lLen+lIniPos)) < LNULL) rc=-R_PSTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : UserSniP_type_cod                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL UserSniP_type_cod(
	unsigned long  int        hStore,
	rT041F_stOther     FAR  * x,
	rT0430_stContext   NEAR * pContext,
	signed short int   FAR  * psPmNumb)

{ /* entr */
RM0001_STRG (fnctName, "UserSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;
rT0410_stPdvList       stLocalPdvList;
rTro01_pfvObjectId     pTransSynName;


signed long int      lLen  = LNULL;
signed long int      lIniPos = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *) ro0069_stputs(hStore   ,
                                   sizeof(rT041F_stOther) ); 
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     RM0012_ENCODEi (RC04y1_PriVersion1, RC04ya_sPrivVersion, char)
     lLen += sizeof(char);
     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC04y6_MskPOther , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);

     rc = ro0411_encodePdvList ((rT0410_stPdvList        FAR *)&stLocalPdvList,
				(rTro01_pfvObjectId	 FAR *)&pTransSynName ,
                                 psPmNumb ,
	                         pContext,
                                 x->stUserData.unUserData.stFully.vPdvList[0]);

     RM0012_ENCODEi (stLocalPdvList.lPresContId, RC04yf_vPresContId, char)
       lLen += sizeof(char);

     if (!rc)
         if (ro0084_stStoreResize(hStore, (lLen+lIniPos)) < LNULL) rc=-R_PSTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ABPSniP_type_cod                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ABPSniP_type_cod(
	unsigned long  int        hStore,
	rT041E_stArp       FAR  * x,
	rT0430_stContext   NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "ABPSniP_type_cod")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;

signed long int      lLen  = LNULL;
signed long int      lIniPos = LNULL;
  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *) ro0069_stputs(hStore   ,
                                   sizeof(rT041E_stArp) ); 
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     /* ************* encode Mask Information *********** */

     RM0012_ENCODEi (RC04y5_MskPAbo , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
     RM0012_ENCODEi (TRUE , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
    
  RM0012_ENCODEi (x->lAbortReason, RC04o_lAbortReason, char)
     lLen += sizeof(char);
  RM0012_ENCODEi (x->lEventId, RC04p_lEventId, char)
     lLen += sizeof(char);

  if (!rc)
     if (ro0084_stStoreResize(hStore, (lLen+lIniPos)) < LNULL) rc  = -R_PSTERR;

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */


/* ****************************************************************** */
/* ****  SNI Private Transfer Syntax  Decoding Functions ************ */
/* ****************************************************************** */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : CPSniP_type_dec                                     */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL CPSniP_type_dec(
	unsigned long int        hStore,
	rT041A_stCp       FAR  * x,
	rT0430_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "CPSniP_type_dec")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int iInx2;
signed short int iInx3;
signed short int rc=0;
char FAR *pBuffActual;
signed long int      lLen = LNULL;
signed long int      lIniPos = LNULL;
signed   long  int     l;

  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     /* ************* mode selector decode ************** */
     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT041A_stCp))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)
     
                          

     for (iInx1 = 0;                                             
          iInx1 < RC04y0_PriLengthModeSelector;                        
          iInx1++)                                               

     if ((unsigned char) *(pBuffActual + iInx1) != 
                      (unsigned char) rT04y0_vModeSelector [iInx1])
        RM0003_RETURN (fnctName, signed int, -R_PDEPTS) ;
     x->lModeSelector = RCro72_SNIprivate;
     pBuffActual += RC04y0_PriLengthModeSelector;
     lLen += RC04y0_PriLengthModeSelector;

     /* ************* protocol version decode *********** */

     RM0013_DECODEi (x->bsProtVersion, RC04ya_sPrivVersion, char)
     lLen += sizeof(char);
     /* **** shift 31 bits left to match to new ********* */
     /* **** protocol version value of 0x80000000 to **** */
     /* **** old version value of 1 ********************* */

     (x->bsProtVersion) = (x->bsProtVersion) <<31;	  

     /* ************* decode Mask Information *********** */

     RM0013_DECODEi (x->stPdvHeader.lType , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
      
     (x->stPdvHeader.lType) -= 1; /* compatibility with new version */
      pContext->lType = x->stPdvHeader.lType;
 
     /* ********* decode Calling Selector length ******** */

     RM0013_DECODEi (x->ulCallingSelector,RC04yc_lCallingSelectorLength, 
                     char)  
     lLen += sizeof(char);

     /* ********* decode Calling Selector *************** */
     if (x->ulCallingSelector > RCro1a_MaxLenPSelector)
        RM0003_RETURN (fnctName, signed short int, 0x06)  
          /* Invalid PPDU parameter */
        
     if ( x->ulCallingSelector)
          x->bCallingSelector = TRUE;
     else x->bCallingSelector = FALSE;

     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulCallingSelector;
           iInx1++)

     x->vCallingSelector [iInx1]= *(pBuffActual + iInx1) ;
     pBuffActual += iInx1;
     lLen += iInx1;

     /* ********* decode Called  Selector length ******** */

     RM0013_DECODEi (x->ulCalledSelector, RC04yd_ulCalledSelectorLength, 
		     char)  
     lLen += sizeof(char);

     if (x->ulCalledSelector > RCro1a_MaxLenPSelector)
        RM0003_RETURN (fnctName, signed short int, 0x06)  
          /* Invalid PPDU parameter */
     /* ********* decode Called  Selector *************** */
     if ( x->ulCalledSelector)
          x->bCalledSelector = TRUE;
     else x->bCalledSelector = FALSE;

     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulCalledSelector;
           iInx1++)
     x->vCalledSelector [iInx1]= *(pBuffActual + iInx1);
     pBuffActual += iInx1;
     lLen += iInx1;

     /* ********* decode number of Pres Cxt Definitions * */

     RM0013_DECODEi (x->ulPresContDef, RC04ye_lPresContDef, char)  
     lLen += sizeof(char);

     if ( x->ulPresContDef)
          x->bPresContDef = TRUE;
     else x->bPresContDef = FALSE;
     /* ********* decode Presentation Context Id ******** */
     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulPresContDef;
           iInx1++)
     {
     RM0013_DECODEi (x->vPresContDef[iInx1].lPresContId, 
                     RC04yf_vPresContId, short)
     lLen += sizeof(short);

     /* ********* decode no. of elements in AbSynName *** */

     RM0013_DECODEi (x->vPresContDef[iInx1].oAbstrSynName[0], 
                     RC04yg_AbstrSynName, short)
     lLen += sizeof(short);

     /* ********* decode Abstract Syntax Name *********** */
     for ( iInx2 = 1;
           iInx2 < (signed int) x->vPresContDef[iInx1].oAbstrSynName[0]+1;
           iInx2++){
    
     RM0013_DECODEi (x->vPresContDef[iInx1].oAbstrSynName[iInx2], 
                     RC04yg_AbstrSynName, short)
     lLen += sizeof(short);
     }
     /* ****** decode no. of transfer syntaxes ********** */
     /* ********** per  this abstract syntax ************ */

     RM0013_DECODEi (x->vPresContDef[iInx1].ulTransSynNumber, 
                     RC04yh_vTransSynNumber, char )
     lLen += sizeof(char);

     for ( iInx2 = 0;
           iInx2 < (signed int) x->vPresContDef[iInx1].ulTransSynNumber; 
           iInx2++)
     {
     /* ****** encode no. of elements in T.Syn Name  **** */

      RM0013_DECODEi (x->vPresContDef[iInx1].vTransSynName[iInx2] [0], 
                      RC04yi_vTransSynName, short)
     lLen += sizeof(short);

      for ( iInx3 = 1;
            iInx3 <(signed int)x->vPresContDef[iInx1].vTransSynName[iInx2][0]+1;
            iInx3++){
      RM0013_DECODEi (x->vPresContDef[iInx1].vTransSynName[iInx2][iInx3], 
                      RC04yi_vTransSynName, short)
       lLen += sizeof(short);

      }
      } /* for transfer syntax copy */
     } /*to "for lAbstrSynNumber" */

     RM0013_DECODEi (l, RC04yf_vPresContId, char)
     lLen += sizeof(char);


     if (ro0066_storepos(hStore, (lIniPos+lLen) ) < LNULL)   rc = -R_PSTERR;
     if (!rc)
         if ((lLen = ro0062_storelength(hStore)) < LNULL)    rc = -R_PSTERR;
     if (!rc)
         if ((lIniPos = ro0076_stwhere(hStore)) < LNULL)     rc = -R_PSTERR;
     if (!rc) {
         if ( ( lLen - lIniPos) > 1 ) {
            x->bUserData = TRUE;
            x->stUserData.lSimplyFullyChoice = RC0418_FullyEncoded;
         }
         else {
            if ((lLen = ro0070_stgetc(hStore)) < LNULL) rc  = -R_PSTERR;
         }
     }
     if (!rc) {
         x->bNormalSEQUENCE = TRUE; 
         rc = ro0420_syntaxCp (pContext,(rT041G_unPpdu FAR *) x);
         x->lModeSelector = 
         pContext->bNormalMode = RCro72_SNIprivate;
     }
 RM0003_RETURN (fnctName, signed short int, rc)
}/* end */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : CPASniP_type_dec                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL CPASniP_type_dec(
	unsigned long int        hStore,
	rT041B_stCpa      FAR  * x,
	rT0430_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "CPASniP_type_dec")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int iInx2;
signed short int rc=0;
char FAR *pBuffActual;
signed long int      lLen = LNULL;
signed long int      lIniPos = LNULL;
signed long int      l;

  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT041B_stCpa))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     /* ************* mode selector decode ************** */

     for (iInx1 = 0;                                             
          iInx1 < RC04y0_PriLengthModeSelector;                        
          iInx1++)                                               

     if ((unsigned char)*(pBuffActual + iInx1) != 
                  (unsigned char)rT04y0_vModeSelector [iInx1])
        RM0003_RETURN (fnctName, signed int, -R_PDEPTS);
     x->lModeSelector = RCro72_SNIprivate;
     pBuffActual += RC04y0_PriLengthModeSelector;
     lLen += RC04y0_PriLengthModeSelector;

     /* ************* protocol version decode *********** */

     RM0013_DECODEi (x->bsProtVersion, RC04ya_sPrivVersion, char)
     lLen += sizeof(char);

     /* **** shift 31 bits left to match to new ********* */
     /* **** protocol version value of 0x80000000 to **** */
     /* **** old version value of 1 ********************* */

     (x->bsProtVersion) = (x->bsProtVersion) <<31;	  

     /* ************* decode Mask Information *********** */

     RM0013_DECODEi (x->stPdvHeader.lType , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
     (x->stPdvHeader.lType) -= 1; /* compatibility with new version */
     pContext->lType = x->stPdvHeader.lType;
 
     /* ****** decode Responding Selector length ******** */

     RM0013_DECODEi (x->ulRespondingSelector, 
                     RC04yj_lRespondingSelectorLength, char)  
     lLen += sizeof(char);

     /* ****** decode Responding Selector *************** */

     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulRespondingSelector;
           iInx1++)
     x->vRespondingSelector [iInx1] = *(pBuffActual + iInx1) ;
     pBuffActual += iInx1;
     lLen += iInx1;

     /* ****** decode no. of Presentation Context ******* */
     /* ****** Definition Results  ********************** */

     RM0013_DECODEi (x->ulPresContDefRes, RC04yk_lPresContDefRes, 
                     char)  
     lLen += sizeof(char);

     /* ****** decode Presentation Context ************** */
     /* ****** Definition Results  ********************** */

     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulPresContDefRes;
           iInx1++)
     {
       RM0013_DECODEi (x->vPresContDefRes[iInx1].lResult, 
		       RC04yl_vResult, short)  
       lLen += sizeof(short);

     /* ****** decode no. of elements in TransSynName *** */

       RM0013_DECODEi (x->vPresContDefRes[iInx1].oTransSynName [0], 
                       RC04yi_vTransSynName, short)
       lLen += sizeof(short);

     /* ****** decode TransSynName ********************** */

       for ( iInx2 = 1;
             iInx2 < (signed int) x->vPresContDefRes[iInx1].oTransSynName[0]+1;
             iInx2++) {

       RM0013_DECODEi (x->vPresContDefRes[iInx1].oTransSynName[iInx2], 
                       RC04yi_vTransSynName, short)
       lLen += sizeof(short);
      }
       RM0013_DECODEi (x->vPresContDefRes[iInx1].lProviderReason ,
                       RC04ym_vProviderReason, short)  
       lLen += sizeof(short);

     }/* to  no. of Presentation context definition results */
     RM0013_DECODEi (l, RC04yf_vPresContId, char)
     lLen += sizeof(char);

     if (ro0066_storepos(hStore, (lIniPos+lLen) ) < LNULL) rc = -R_PSTERR;
     if (!rc)
         if ((lLen = ro0062_storelength(hStore)) < LNULL)  rc = -R_PSTERR;
     if (!rc)
         if ((lIniPos = ro0076_stwhere(hStore)) < LNULL)   rc = -R_PSTERR;
     if (!rc) {
         if ( ( lLen - lIniPos) > 1 ) {
            x->bUserData = TRUE;
            x->stUserData.lSimplyFullyChoice = RC0418_FullyEncoded;
         }
         else {
            if ((lLen = ro0070_stgetc(hStore)) < LNULL) rc =  -R_PSTERR;
         }
     }

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : CPRSniP_type_dec                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL CPRSniP_type_dec(
	unsigned long int        hStore,
	rT041C_stCpr      FAR  * x,
	rT0430_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "CPRSniP_type_dec")

  /* ****************** local variables ********************* */

signed short int iInx1;
signed short int iInx2;
signed short int rc=0;
char FAR *pBuffActual;

signed long int      lLen = LNULL;
signed long int      lIniPos = LNULL;
signed long int      l;

  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT041C_stCpr))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     /* ************* protocol version decode *********** */

     RM0013_DECODEi (x->bsProtVersion, RC04ya_sPrivVersion, char)
     lLen += sizeof(char);

     /* **** shift 31 bits left to match to new ********* */
     /* **** protocol version value of 0x80000000 to **** */
     /* **** old version value of 1 ********************* */

     (x->bsProtVersion) = (x->bsProtVersion) <<31;	  

     /* ************* decode Mask Information *********** */

     RM0013_DECODEi (x->stPdvHeader.lType , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
     (x->stPdvHeader.lType) -= 1; /* compatibility with new version */
     pContext->lType = x->stPdvHeader.lType;

     /* ****** decode Responding Selector length ******** */

     RM0013_DECODEi (x->ulRespondingSelector, 
                     RC04yj_lRespondingSelectorLength, char)  
     lLen += sizeof(char);

     /* ****** decode Responding Selector *************** */

     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulRespondingSelector;
           iInx1++)
     x->vRespondingSelector [iInx1] = *(pBuffActual + iInx1);
     pBuffActual += iInx1;
     lLen += iInx1;

     /* ****** decode no. of Presentation Context ******* */
     /* ****** Definition Results  ********************** */

     RM0013_DECODEi (x->ulPresContDefRes, RC04yk_lPresContDefRes, 
                     char)  
     lLen += sizeof(char);

     /* ****** decode Presentation Context ************** */
     /* ****** Definition Results  ********************** */

     for ( iInx1 = 0;
           iInx1 < (signed int) x->ulPresContDefRes;
           iInx1++)
     {
       RM0013_DECODEi (x->vPresContDefRes[iInx1].lResult, 
		       RC04yl_vResult, short)  
       lLen += sizeof(short);

     /* ****** decode no. of elements in TransSynName *** */

       RM0013_DECODEi (x->vPresContDefRes[iInx1].oTransSynName [0], 
                       RC04yi_vTransSynName, short)
       lLen += sizeof(short);

     /* ****** decode TransSynName ********************** */

       for ( iInx2 = 1;
             iInx2 < (signed int) x->vPresContDefRes[iInx1].oTransSynName[0]+1;
             iInx2++){

       RM0013_DECODEi (x->vPresContDefRes[iInx1].oTransSynName[iInx2], 
                       RC04yi_vTransSynName, short)
       lLen += sizeof(short);
       }
       RM0013_DECODEi (x->vPresContDefRes[iInx1].lProviderReason ,
                       RC04ym_vProviderReason, short)  
       lLen += sizeof(short);

     }/* to  no. of Presentation context definition results */

     RM0013_DECODEi (l, RC04yf_vPresContId, char)
     lLen += sizeof(char);

     if (ro0066_storepos(hStore, (lIniPos+lLen) ) < LNULL)  rc = -R_PSTERR;
     if (!rc)
         if ((lLen = ro0062_storelength(hStore)) < LNULL)   rc = -R_PSTERR;
     if (!rc)
         if ((lIniPos = ro0076_stwhere(hStore)) < LNULL)    rc = -R_PSTERR;
     if (!rc) {
         if ( ( lLen - lIniPos) > 1 ) {
            x->bUserData = TRUE;
            x->stUserData.lSimplyFullyChoice = RC0418_FullyEncoded;
         }
         else {
            if ((lLen = ro0070_stgetc(hStore)) < LNULL) rc = -R_PSTERR;
         }
     }

 RM0003_RETURN (fnctName, signed short int, rc)

} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ABUSniP_type_dec                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ABUSniP_type_dec(
	unsigned long int        hStore,
	rT041D_stAru      FAR  * x,
	rT0430_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "ABUSniP_type_dec")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;
signed   long int      i;

signed long int      lLen = LNULL;
signed long int      lIniPos = LNULL;
signed long int      l;

  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT041D_stAru))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     /* ************* decode Mask Information *********** */

     RM0013_DECODEi (x->stPdvHeader.lType , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
     (x->stPdvHeader.lType) -= 1; /* compatibility with new version */
     pContext->lType = x->stPdvHeader.lType;
    
     RM0013_DECODEi (i , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);

     RM0013_DECODEi (l, RC04yf_vPresContId, short)
     lLen += sizeof(short); /* !!! check not sure */

     if (ro0066_storepos(hStore, (lIniPos+lLen) ) < LNULL)  rc = -R_PSTERR;
     if (!rc)
         if ((lLen = ro0062_storelength(hStore)) < LNULL)   rc = -R_PSTERR;
     if (!rc)
         if ((lIniPos = ro0076_stwhere(hStore))  < LNULL)   rc = -R_PSTERR;
     if (!rc) {
         if ( ( lLen - lIniPos) > 1 ) {
            x->bUserData = TRUE;
            x->stUserData.lSimplyFullyChoice = RC0418_FullyEncoded;
         }
         else {
            if ((lLen = ro0070_stgetc(hStore)) < LNULL)    rc = -R_PSTERR;
         }
     }

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : UserSniP_type_dec                                   */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL UserSniP_type_dec(
	unsigned long int        hStore,
	rT041F_stOther    FAR  * x,
	rT0430_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "UserSniP_type_dec")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;

signed long int      lLen = LNULL;
signed long int      lIniPos = LNULL;

  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT041F_stOther))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     /* ************* decode Mask Information *********** */

     RM0013_DECODEi (x->stPdvHeader.lType , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
     (x->stPdvHeader.lType) -= 1; /* compatibility with new version */
     pContext->lType = x->stPdvHeader.lType;
    /* 
     RM0013_DECODEi (l, RC04yf_vPresContId, short)
       lLen += sizeof(short);
     **** by kc to move this to called function */

     if (ro0066_storepos(hStore, (lIniPos+lLen) ) < LNULL)  rc = -R_PSTERR;
     if (!rc)
         if ((lLen = ro0062_storelength(hStore)) < LNULL)    rc = -R_PSTERR;
     if (!rc)
         if ((lIniPos = ro0076_stwhere(hStore)) < LNULL)     rc = -R_PSTERR;
     if (!rc) {
         if ( ( lLen - lIniPos) > 1 ) {
            x->stUserData.lSimplyFullyChoice = RC0418_FullyEncoded;
         }
         else {
            if ((lLen = ro0070_stgetc(hStore)) < LNULL) rc = -R_PSTERR;
         }
     }

 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ABPSniP_type_dec                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch,Vinita                               */
/* DATE         : 91-07-24,93-04-19                                   */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ABPSniP_type_dec(
	unsigned long int        hStore,
	rT041E_stArp      FAR  * x,
	rT0430_stContext  NEAR * pContext)

{ /* entr */
RM0001_STRG (fnctName, "ABPSniP_type_dec")

  /* ****************** local variables ********************* */

signed short int rc=0;
char FAR *pBuffActual;
signed long int i=0;

signed long int      lLen = LNULL;
signed long int      lIniPos = LNULL;

  /* ****************** START   **************************** */

RM0002_ENTRY (fnctName)

     lIniPos = ro0076_stwhere (hStore);
     if (lIniPos < LNULL)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     pBuffActual = (char FAR *)ro0072_stgets ( hStore, 
                     (MIN((ro0062_storelength( hStore)-lIniPos),
                          sizeof (rT041E_stArp))));
     if (!pBuffActual)
         RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

     /* ************* decode Mask Information *********** */

     RM0013_DECODEi (x->stPdvHeader.lType , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
     (x->stPdvHeader.lType) -= 1; /* compatibility with new version */
     pContext->lType = x->stPdvHeader.lType;
    
     RM0013_DECODEi (i , RC04yb_sPrivPciId, char)
     lLen += sizeof(char);
    if (i ){ 
       RM0013_DECODEi (x->lAbortReason, RC04o_lAbortReason, char)
       lLen += sizeof(char);
       RM0013_DECODEi (x->lEventId, RC04p_lEventId, char)
       lLen += sizeof(char);
    }

    if (ro0066_storepos(hStore, (lIniPos+lLen) ) <  LNULL)  rc = -R_PSTERR;
    
 RM0003_RETURN (fnctName, signed short int, rc)
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro04y0_CheckSniPTS	                	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro04y0_CheckSniPTS ( 
	unsigned long int        hStore,
	rT0430_stContext  NEAR * pContext)

/*                                                                    */
/* DESCRIPTION  :   Checks for the Sni PTS Static Header & If matches */
/* sets the bNormalMode to -1 .Does not alter the position of Store   */
/*                                                                    */
/* IN-PARAMETERS              :Store handle , Pres. Context Ptr       */
/*                                                                    */
/* OUT-PARAMETERS             : None                                  */
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
RM0001_STRG (fnctName, "ro04y0_CheckSniPTS")
signed  short  int      rc    = OK;
char               FAR *pBuff = NULL;
unsigned short int      iInx1 = NULL;
signed long  int      lIniPos=LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (fnctName)
 lIniPos = ro0076_stwhere (hStore);
 if (lIniPos < LNULL)
     RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

 pBuff = (char FAR *) ro0072_stgets (hStore ,
                                   (long)RC04y0_PriLengthModeSelector);
 if (!pBuff)
     RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

 for (iInx1 = 0; iInx1 < RC04y0_PriLengthModeSelector; iInx1++)  { 
     if ((unsigned char )*(pBuff+ iInx1) != 
                    (unsigned char)rT04y0_vModeSelector [iInx1])
       break;
 }
 if (iInx1 == RC04y0_PriLengthModeSelector ) 
     pContext->bNormalMode = RCro72_SNIprivate;
 if (ro0066_storepos(hStore , lIniPos) < LNULL) rc = -R_PSTERR;

RM0003_RETURN (fnctName, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro04y1_routeSnixAutoNo                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-05-28                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro04y1_routeSnixAutoNo ( 
	signed short int  FAR * pRoute)

/*                                                                    */
/* DESCRIPTION  :    gets  the routing info from Sni Mask to Automata */
/*                           Number.                                  */
/*                                                                    */
/* IN-PARAMETERS              :  Sni Msk                              */
/*                                                                    */
/* OUT-PARAMETERS             :  Automata Number                      */
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
RM0001_STRG (fnctName, "ro04y1_routeSnixAutoNo")
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (fnctName)
switch ( *pRoute){
   case RC04y7_MskAcse:
       *pRoute = RC0216_Acse;
       break;
   case RC04y8_MskRoseOctet:
      /* *pRoute = RC0221_RoseUser; */
       *pRoute = RC0219_Rose;
       break;
   case RC04y9_MskRoseBER  :
       *pRoute = RC0219_Rose;
       break;
   default:
       *pRoute = RC0221_RoseUser;      
       break;
 }
RM0003_RETURN (fnctName, signed short int, rc)                                    
}/*end*/                                                                        

/* EOF */
