/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spacpm2.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:39  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:47:32  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:51  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:45  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:45:29  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:35  keutel
 * 	creation
 * 	[1994/03/21  13:15:07  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spacpm2.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:27 $" } ;
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
/* NAME 	: spacpm2.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K                                      */
/* DATE 	: 29-11-93					      */
/*                                                                    */
/* COMPONENT    : RTROS Evaluation Program                            */
/* PRD#/VERS.   : RTROSV3.00                                          */
/*                                                                    */
/* DESCRIPTION  : In this module the trace record types are evaluated */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.00   K00| 29-11-93| original		       | kc |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spacpm2.c 1.1 93/12/20   ";

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
#include <rouply0.h>
#include <roppm04.h>
#include <roppm03.h>
#include <roppm00.h>
#include <roppm01.h>
#include <roacpm0.h>
#include <roacpm1.h>
#include <rospm00.h>
#include <routil0.h>
#include <rostor0.h>
#include <spuctr0.h>
#include <spacpm0.h>

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spprpm0.h>	  /* Function prototyping		      */

/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/

void sp0621_writeAARQPdu PROTO ((
rT0641_aarqApdu    FAR * pNode));
void sp0622_writeAAREPdu PROTO ((
rT0642_aareApdu    FAR * pNode));
void sp0623_writeRLRQPdu PROTO ((
rT0643_rlrqApdu    FAR * pNode));
void sp0624_writeRLREPdu PROTO ((
rT0644_rlreApdu    FAR * pNode));
void sp0625_writeABRTPdu PROTO ((
rT0645_abrtApdu    FAR * pNode));

void sp062a_writeApTitle PROTO ((
signed long int       lType,
rT0635_uApTitle FAR * pUnAeTitle));
void sp062b_writeAeQuali PROTO (( 
signed long int       lType,
rT0637_uAeQualifier FAR * pUnAeTitle));
void sp062c_writeResult PROTO ((
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue));
void sp062d_writePReason PROTO ((
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue));
void sp062e_writeAbortSource PROTO ((
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue));
void sp062f_writeReason PROTO ((
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue));
void sp062g_writeRDN    PROTO ((
rT0631_stRDN  FAR * pRDN));

void sp062h_writeAVA    PROTO ((
rT0630_stAttriValueAssertion FAR * pAVA));
/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0620_writeAcpmPdu                   	      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 29-11-93					      */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS: Handle to the Pdu           			      */
/*                                                                    */
/* OUT-PARAMETERS:   none                                             */
/*                                                                    */
/* RETURN VALUES :   none                                             */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/*inoff ***************************************************************/

void sp0620_writeAcpmPdu (
	unsigned long int hNode)
{/* entr */
  rT0646_acseApdus  FAR * pNode; 
  if (hNode) {
     pNode = (rT0646_acseApdus FAR *) ro0021_globallock (hNode); 
   
     switch(pNode->stPdvHeader.lType){
      case RC0665_AARQapdu:
        sp0621_writeAARQPdu((rT0641_aarqApdu    FAR *)pNode);
        break;
      case RC0666_AAREapdu:
        sp0622_writeAAREPdu((rT0642_aareApdu    FAR *)pNode);
        break;
      case RC0667_RLRQapdu:
        sp0623_writeRLRQPdu((rT0643_rlrqApdu    FAR *)pNode);
        break;
      case RC0668_RLREapdu:
        sp0624_writeRLREPdu((rT0644_rlreApdu    FAR *)pNode);
        break;
      case RC0669_ABRTapdu:
        sp0625_writeABRTPdu((rT0645_abrtApdu    FAR *)pNode);
        break;
      default:
        break;
    }
 ro0022_globalunlock (hNode);
 }
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0621_writeAARQPdu				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 29-11-93					      */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS: Pointer to the Pdu           			      */
/*                                                                    */
/* OUT-PARAMETERS:   none                                             */
/*                                                                    */
/* RETURN VALUES :   none                                             */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/*inoff ***************************************************************/

void sp0621_writeAARQPdu ( 
	rT0641_aarqApdu    FAR * pNode)
{ /* entr */
 unsigned long int i;

 sp1281_writePduHdr("AARQ-PDU","ACPM");
 sp1257_wrUnLong(RC1201_OFFSET1,"PROTOCOL-VERSION LENGTH:",
                    (unsigned long int FAR *)&pNode->ulProtoVersionLength,
                    RC1206_DEC_NL);
 sp1257_wrUnLong(RC1201_OFFSET1,"PROTOCOL-VERSION :",
                    (unsigned long int FAR *)&pNode->ulProtoVersion,
                    RC1208_HEX_NL);
 
 sp1282_WrObjId (RC1201_OFFSET1,
                (char FAR *)"APPL. CXT. NAME      :",
                (char FAR *)(pNode->vApliCxtName+1), 
                (pNode->vApliCxtName[0]*
                 sizeof(unsigned long int) ));
 if(pNode->bCalledApTitle){
    sp1264_write(RC1201_OFFSET1,"%s\n","CALLED APPL. TITLE :",NULL,NULL,NULL); 
    sp062a_writeApTitle(pNode->lCalledApTitle,&pNode->uCalledApTitle);
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"CALLED AP-TITLE    :", 
                  (signed int)pNode->bCalledApTitle);
 if(pNode->bCalledAeQualifier){
    sp1264_write(RC1201_OFFSET1,"%s\n","CALLED AE QUALIFIER :",NULL,NULL,NULL); 
    sp062b_writeAeQuali(pNode->lCalledAeQualifier,&pNode->uCalledAeQualifier); 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"CALLED AE QUALIFIER    :", 
                  (signed int)pNode->bCalledAeQualifier);
 if(pNode->bCalledApInvocId)
   sp1255_wrSgLong(RC1201_OFFSET1,"CALLED AP INVOCATION ID :",
                   &(pNode->lCalledApInvocId),RC1206_DEC_NL);
 else  
    sp1280_writeBool((signed int)RC1201_OFFSET1,"CALLED AP INVOCATION ID :", 
                  (signed int)pNode->bCalledApInvocId);
 if(pNode->bCalledAeInvocId)
   sp1255_wrSgLong(RC1201_OFFSET1,"CALLED AE INVOCATION ID :",
                   &(pNode->lCalledAeInvocId),RC1206_DEC_NL);
 else  
    sp1280_writeBool((signed int)RC1201_OFFSET1,"CALLED AE INVOCATION ID :", 
                  (signed int)pNode->bCalledAeInvocId);

 if(pNode->bCallingApTitle){
    sp1264_write(RC1201_OFFSET1,"%s\n","CALLING APPL. TITLE :",NULL,NULL,NULL); 
    sp062a_writeApTitle(pNode->lCallingApTitle,&pNode->uCallingApTitle);
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"CALLING AP-TITLE    :", 
                  (signed int)pNode->bCallingApTitle);
 if(pNode->bCallingAeQualifier){
    sp1264_write(RC1201_OFFSET1,"%s\n","CALLING AE QUALIFIER :",NULL,NULL,NULL); 
    sp062b_writeAeQuali(pNode->lCallingAeQualifier,&pNode->uCallingAeQualifier); 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"CALLING AE QUALIFIER    :", 
                  (signed int)pNode->bCallingAeQualifier);
 if(pNode->bCallingApInvocId)
   sp1255_wrSgLong(RC1201_OFFSET1,"CALLING AP INVOCATION ID :",
                   &(pNode->lCallingApInvocId),RC1206_DEC_NL);
 else  
    sp1280_writeBool((signed int)RC1201_OFFSET1,"CALLING AP INVOCATION ID :", 
                  (signed int)pNode->bCallingApInvocId);
 if(pNode->bCallingAeInvocId)
   sp1255_wrSgLong(RC1201_OFFSET1,"CALLING AE INVOCATION ID :",
                   &(pNode->lCallingAeInvocId),RC1206_DEC_NL);
 else  
    sp1280_writeBool((signed int)RC1201_OFFSET1,"CALLING AE INVOCATION ID :", 
                  (signed int)pNode->bCallingAeInvocId);
 sp1280_writeBool((signed int)RC1201_OFFSET1,"IMPLEMENTATION INFO      :", 
                  (signed int)pNode->bImplInfo);
 if(pNode->bUserInfo){
   sp1257_wrUnLong(RC1201_OFFSET1,"NUMB. OF EXTERNALS:",
                    (unsigned long int FAR *)&pNode->ulUserInfo,
                    RC1206_DEC_NL);
   for (i=0;i<pNode->ulUserInfo;i++)
      sp1530_writeUctrData(pNode->vUserInfo[i]); 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"USER INFO                :", 
                  (signed int)pNode->bUserInfo);
  
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0622_writeAAREPdu				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 01-12-93					      */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS: Pointer to the Pdu           			      */
/*                                                                    */
/* OUT-PARAMETERS:   none                                             */
/*                                                                    */
/* RETURN VALUES :   none                                             */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/*inoff ***************************************************************/

void sp0622_writeAAREPdu (
	rT0642_aareApdu    FAR * pNode)
{ /* entr */
 unsigned long int i;

 sp1281_writePduHdr("AARE-PDU","ACPM");
 sp1257_wrUnLong(RC1201_OFFSET1,"PROTOCOL-VERSION LENGTH:",
                    (unsigned long int FAR *)&pNode->ulProtoVersionLength,
                    RC1206_DEC_NL);
 sp1257_wrUnLong(RC1201_OFFSET1,"PROTOCOL-VERSION :",
                    (unsigned long int FAR *)&pNode->ulProtoVersion,
                    RC1208_HEX_NL);
 
 sp1282_WrObjId (RC1201_OFFSET1,
                (char FAR *)"APPL. CXT. NAME      :",
                (char FAR *)(pNode->vApliCxtName+1), 
                (pNode->vApliCxtName[0]*
                 sizeof(unsigned long int) ));
 sp062c_writeResult(RC1201_OFFSET1," RESULT :",pNode->lResult);
 switch(pNode->lResultSourceDiag){
   case RC0644_ResultSourceUser:
      sp1264_write (RC1201_OFFSET1, "%s%s\n", "RESULT SOURCE DIAG:", 
                    "SOURCE USER", NULL, NULL); 
      sp1255_wrSgLong(RC1201_OFFSET1,"USER SOURCE   :",
                   &(pNode->uResultSourceDiag.lAcseServiceUser),RC1208_HEX_NL);
      
      break; 
   case RC0645_ResultSourceProvid:
      sp1264_write (RC1201_OFFSET1, "%s%s\n", "RESULT SOURCE DIAG:", 
                    "SOURCE PROVIDER", NULL, NULL); 
      sp1255_wrSgLong(RC1201_OFFSET1,"PROVIDER SOURCE    :",
                   &(pNode->uResultSourceDiag.lAcseServiceProvider),
                   RC1208_HEX_NL);
      break;
   default:
      sp1264_write (RC1201_OFFSET1, "%s\n", "---UNKNOWN      ---", 
                    NULL, NULL, NULL); 
      break;
 }
 if(pNode->bRespondApTitle){
    sp1264_write(RC1201_OFFSET1,"%s\n","RESPOND APPL. TITLE :",NULL,NULL,NULL); 
    sp062a_writeApTitle(pNode->lRespondApTitle,&pNode->uRespondApTitle);
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"RESPOND AP-TITLE    :", 
                  (signed int)pNode->bRespondApTitle);
 if(pNode->bRespondAeQualifier){
    sp1264_write(RC1201_OFFSET1,"%s\n","RESPOND AE QUALIFIER :",NULL,NULL,
                  NULL); 
    sp062b_writeAeQuali(pNode->lRespondAeQualifier,
                     &pNode->uRespondAeQualifier); 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"RESPOND AE QUALIFIER    :", 
                  (signed int)pNode->bRespondAeQualifier);
 if(pNode->bRespondApInvocId)
   sp1255_wrSgLong(RC1201_OFFSET1,"RESPOND AP INVOCATION ID :",
                   &(pNode->lRespondApInvocId),RC1206_DEC_NL);
 else  
    sp1280_writeBool((signed int)RC1201_OFFSET1,"RESPOND AP INVOCATION ID :", 
                  (signed int)pNode->bRespondApInvocId);
 if(pNode->bRespondAeInvocId)
   sp1255_wrSgLong(RC1201_OFFSET1,"RESPOND AE INVOCATION ID :",
                   &(pNode->lRespondAeInvocId),RC1206_DEC_NL);
 else  
    sp1280_writeBool((signed int)RC1201_OFFSET1,"RESPOND AE INVOCATION ID :", 
                  (signed int)pNode->bRespondAeInvocId);

 sp1280_writeBool((signed int)RC1201_OFFSET1,"IMPLEMENTATION INFO      :", 
                  (signed int)pNode->bImplInfo);
 if(pNode->bUserInfo){
   sp1257_wrUnLong(RC1201_OFFSET1,"NUMB. OF EXTERNALS:",
                    (unsigned long int FAR *)&pNode->ulUserInfo,
                    RC1206_DEC_NL);
   for (i=0;i<pNode->ulUserInfo;i++)
      sp1530_writeUctrData(pNode->vUserInfo[i]); 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"USER INFO                :", 
                  (signed int)pNode->bUserInfo);
  
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0623_writeRLRQPdu				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 01-12-93					      */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS: Pointer to the Pdu           			      */
/*                                                                    */
/* OUT-PARAMETERS:   none                                             */
/*                                                                    */
/* RETURN VALUES :   none                                             */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/*inoff ***************************************************************/

void sp0623_writeRLRQPdu (
	rT0643_rlrqApdu    FAR * pNode)
{ /* entr */
 unsigned long int i;

 sp1281_writePduHdr("RLRQ-PDU","ACPM");
 if(pNode->bReason)
   sp062f_writeReason(RC1201_OFFSET1,"REASON     :",pNode->lReason);
 else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"REASON             :", 
                  (signed int)pNode->bReason);
 if(pNode->bUserInfo){
   sp1257_wrUnLong(RC1201_OFFSET1,"NUMB. OF EXTERNALS:",
                    (unsigned long int FAR *)&pNode->ulUserInfo,
                    RC1206_DEC_NL);
   for (i=0;i<pNode->ulUserInfo;i++)
      sp1530_writeUctrData(pNode->vUserInfo[i]); 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"USER INFO                :", 
                  (signed int)pNode->bUserInfo);
  
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0624_writeRLREPdu				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 01-12-93					      */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS: Pointer to the Pdu           			      */
/*                                                                    */
/* OUT-PARAMETERS:   none                                             */
/*                                                                    */
/* RETURN VALUES :   none                                             */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/*inoff ***************************************************************/

void sp0624_writeRLREPdu (
	rT0644_rlreApdu    FAR * pNode)
{ /* entr */
 unsigned long int i;

 sp1281_writePduHdr("RLRE-PDU","ACPM");
 if(pNode->bReason)
   sp062d_writePReason(RC1201_OFFSET1,"REASON     :",pNode->lReason);
 else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"REASON             :", 
                  (signed int)pNode->bReason);
 if(pNode->bUserInfo){
   sp1257_wrUnLong(RC1201_OFFSET1,"NUMB. OF EXTERNALS:",
                    (unsigned long int FAR *)&pNode->ulUserInfo,
                    RC1206_DEC_NL);
   for (i=0;i<pNode->ulUserInfo;i++)
      sp1530_writeUctrData(pNode->vUserInfo[i]); 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"USER INFO                :", 
                  (signed int)pNode->bUserInfo);
  
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0625_writeABRTPdu				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 01-12-93					      */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS: Pointer to the Pdu           			      */
/*                                                                    */
/* OUT-PARAMETERS:   none                                             */
/*                                                                    */
/* RETURN VALUES :   none                                             */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/*inoff ***************************************************************/

void sp0625_writeABRTPdu (
	rT0645_abrtApdu    FAR * pNode)
{ /* entr */
 unsigned long int i;

 sp1281_writePduHdr("ABRT-PDU","ACPM");
 sp062e_writeAbortSource(RC1201_OFFSET1,"ABORT SOURCE   :",pNode->lAbortSource);
 if(pNode->bUserInfo){
   sp1257_wrUnLong(RC1201_OFFSET1,"NUMB. OF EXTERNALS:",
                    (unsigned long int FAR *)&pNode->ulUserInfo,
                    RC1206_DEC_NL);
   for (i=0;i<pNode->ulUserInfo;i++)
      sp1530_writeUctrData(pNode->vUserInfo[i]); 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,
                     (char FAR *)"USER INFO                :", 
                     (signed int)pNode->bUserInfo);
  
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp062a_writeApTitle				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 29-11-93					      */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp062a_writeApTitle ( 
signed long int       lType,
rT0635_uApTitle FAR * pUnAeTitle)
/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS: Ae title choice type , Pointer to Union of Ae title */
/*                                                                    */
/* OUT-PARAMETERS:   none                                             */
/*                                                                    */
/* RETURN VALUES :   none                                             */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/*inoff ***************************************************************/
{ /*entr*/
 switch(lType){
   case RCro78_RelDisName:
      {
      signed long int i;
      unsigned long int j;
      sp1264_write (RC1201_OFFSET1, "%s\n", "---RELATIVE DIS. NAME---", 
                    NULL, NULL, NULL); 
       
      sp1255_wrSgLong(RC1201_OFFSET1,"NUMB. OF RDN SEQUENCES   :",
                   &(pUnAeTitle->stApTitleForm1.lName),RC1206_DEC_NL);
      for (i=0;i<pUnAeTitle->stApTitleForm1.lName;i++){
        
        sp1257_wrUnLong(RC1201_OFFSET1,"NUMB. OF RDNs:",
                    (unsigned long int FAR *)
                 &pUnAeTitle->stApTitleForm1.uName.stRDNSequence.lRDNSequence,
                    RC1206_DEC_NL);
        for(j=0;j<pUnAeTitle->stApTitleForm1.uName.stRDNSequence.lRDNSequence;
           j++) 
        sp062g_writeRDN(&
         (pUnAeTitle->stApTitleForm1.uName.stRDNSequence.stRDNSequence[j]));
      }
      } 
      break;
   case RCro77_ObjId:
      sp1264_write (RC1201_OFFSET1, "%s\n", "---OBJECT ID FORM    ---", 
                    NULL, NULL, NULL); 
      sp1282_WrObjId(RC1202_OFFSET2,
                    (char FAR *)"AE TITLE NAME        :",     
                    (char FAR *)(pUnAeTitle->vApTitleForm2+1),           
                    (pUnAeTitle->vApTitleForm2[0]*           
                     sizeof(unsigned long int) ));                       
      break;
   default:
      sp1264_write (RC1201_OFFSET1, "%s\n", "---UNKNOWN FORM     ---", 
                    NULL, NULL, NULL); 
      break;
 }
}/*end*/ 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp062b_writeAeQuali				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 29-11-93					      */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp062b_writeAeQuali ( 
signed long int       lType,
rT0637_uAeQualifier FAR * pUnAeQuali)
/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS: type of Ae Quali,Pointer to Union     */
/*                                                                    */
/* OUT-PARAMETERS:   none                                             */
/*                                                                    */
/* RETURN VALUES :   none                                             */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/*inoff ***************************************************************/
{/*entr*/
 switch(lType){
   case RCro78_RelDisName:
      sp1264_write (RC1201_OFFSET1, "%s\n", "---RELATIVE DIS. NAME---", 
                    NULL, NULL, NULL); 
  
      break;
   case RCro77_ObjId:
      sp1264_write (RC1201_OFFSET1, "%s\n", "---INTRGER   FORM    ---", 
                    NULL, NULL, NULL); 
      sp1255_wrSgLong(RC1201_OFFSET1,"AE QUALIFIER             :",
                   &(pUnAeQuali->lAeQualifierForm2),RC1206_DEC_NL);
      
      break;
   default:
      sp1264_write (RC1201_OFFSET1, "%s\n", "---UNKNOWN FORM     ---", 
                    NULL, NULL, NULL); 
      break;
 }
  
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp062c_writeResult                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp062c_writeResult (
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(ulValue){
    case RC0652_RLRQreasonNormal:
      sp1264_write(iOffset,"%s%s\n",pStr,"RLRQ REASON NORMAL",NULL,NULL);
      break;
    case RC0653_RLRQreasonUrgent:
      sp1264_write(iOffset,"%s%s\n",pStr,"RLRQ REASON URGENT",NULL,NULL);
      break;
    case RC0654_RLRQreasonUsrDef:
      sp1264_write(iOffset,"%s%s\n",pStr,"RLRQ REASON USRDEF",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,(signed int FAR *)&ulValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp062d_writePReason                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp062d_writePReason (
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(ulValue){
    case RC0657_RLREreasonNormal:
      sp1264_write(iOffset,"%s%s\n",pStr,"RLRE REASON NORMAL",NULL,NULL);
      break;
    case RC0658_RLREreasonNotFin:
      sp1264_write(iOffset,"%s%s\n",pStr,"RLRE REASON NOT FINISHED",NULL,NULL);
      break;
    case RC0659_RLREreasonUsrDef:
      sp1264_write(iOffset,"%s%s\n",pStr,"RLRE REASON USR DEFINED",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,(signed int FAR *)&ulValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp062e_writeAbortSource                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp062e_writeAbortSource (
	int                        iOffset,
	char                  FAR *pStr,
	unsigned long int          ulValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(ulValue){
    case RC0660_AboSourceUser:
      sp1264_write(iOffset,"%s%s\n",pStr,"ABORT SOURCE USER",NULL,NULL);
      break;
    case RC0661_AboSourceProvider:
      sp1264_write(iOffset,"%s%s\n",pStr,"ABORT SOURCE PROVIDER",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,(signed int FAR *)&ulValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp062f_writeReason                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp062f_writeReason (
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(ulValue){
    case RC0652_RLRQreasonNormal:
      sp1264_write(iOffset,"%s%s\n",pStr,"RLRQ REASON NORMAL",NULL,NULL);
      break;
    case RC0653_RLRQreasonUrgent:
      sp1264_write(iOffset,"%s%s\n",pStr,"RLRQ REASON URGENT",NULL,NULL);
      break;
    case RC0654_RLRQreasonUsrDef:
      sp1264_write(iOffset,"%s%s\n",pStr,"RLRQ REASON USRDEF",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,(signed int FAR *)&ulValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp062g_writeRDN                                     */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp062g_writeRDN    (
rT0631_stRDN  FAR * pRDN)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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
 unsigned long int i;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
 
 sp1255_wrSgLong(RC1201_OFFSET1,"NUMBER OF RDNs         :",
                   (signed long int FAR *)&(pRDN->lRDN),RC1206_DEC_NL);
 for (i=0;i<pRDN->lRDN;i++)
  sp062h_writeAVA(&pRDN->stRDN[i]);
}/*end*/
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp062h_writeAVA                                     */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp062h_writeAVA    (
rT0630_stAttriValueAssertion FAR * pAVA)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

 sp1282_WrObjId(RC1202_OFFSET2,(char FAR *)"AVA Type      :",     
        (char FAR *)(pAVA->vAttributeType+1),           
        (pAVA->vAttributeType[0]*           
         sizeof(unsigned long int) ));                       
 sp1257_wrUnLong(RC1202_OFFSET2,"AVA Value Type :",
                    (unsigned long int FAR *)&pAVA->lAttributeType,
                    RC1206_DEC_NL);
 sp1263_hexDump (RC1202_OFFSET2, "AVA VALUE   : ",  
                       pAVA->vAttributeValue, pAVA->lAttributeType);            
 
}/*end*/
 
