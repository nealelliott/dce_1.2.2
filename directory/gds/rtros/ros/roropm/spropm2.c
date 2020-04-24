/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spropm2.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:10  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:47  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:28  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:53  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:14  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:07:05  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:40:11  keutel
 * 	creation
 * 	[1994/03/21  13:35:48  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spropm2.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:10 $" } ;
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
/* NAME 	: spropm2.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K                                      */
/* DATE 	: 01-12-93					      */
/*                                                                    */
/* COMPONENT    : RTROS Evaluation Program                            */
/* PRD#/VERS.   : RTROSV3.00                                          */
/*                                                                    */
/* DESCRIPTION  : In this module the trace record types are evaluated */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.00   K00| 01-12-93| original		       | kc |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spropm2.c 1.1 93/12/20   ";

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
#include <rouply1.h>
#include <routil0.h>
#include <rostor0.h>
#include <roppm04.h>
#include <roppm03.h>
#include <roppm00.h>
#include <roppm01.h>
#include <roropm0.h>
#include <roropm1.h>
#include <spuctr0.h>
#include <spropm0.h>

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spprpm0.h>	  /* Function prototyping		      */

/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/

void sp0721_writeROIVPdu PROTO ((
rT0710_stROIVapdu    FAR * pNode));
void sp0722_writeRORSPdu PROTO ((
rT0712_stRORSapdu    FAR * pNode));
void sp0723_writeROERPdu PROTO ((
rT0713_stROERapdu    FAR * pNode));
void sp0724_writeRORJPdu PROTO ((
rT0714_stRORJapdu    FAR * pNode));

void sp072a_writeOpValue PROTO (( 
rTro76_stDefinedBy FAR * pOpVal));

void sp072b_writeProblemChoice PROTO ((
signed long int          lProblemType,
signed long int          lProblem));
/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0720_writeRopmPdu                   	      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 01-12-93					      */
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

void sp0720_writeRopmPdu ( 
unsigned long int hNode)
{/* entr */
  rT0715_unRoseApdus  FAR * pNode; 
  if (hNode) {
     pNode = (rT0715_unRoseApdus FAR *) ro0021_globallock (hNode); 
   
     switch(pNode->stPdvHeader.lType){
      case RC071P_ROIVapdu:
        sp0721_writeROIVPdu((rT0710_stROIVapdu FAR *)pNode);
        break;
      case RC071Q_RORSapdu:
        sp0722_writeRORSPdu((rT0712_stRORSapdu FAR *)pNode);
        break;
      case RC071R_ROERapdu:
        sp0723_writeROERPdu((rT0713_stROERapdu FAR *)pNode);
        break;
      case RC071S_RORJapdu:
        sp0724_writeRORJPdu((rT0714_stRORJapdu FAR *)pNode);
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
/* NAME 	: sp0721_writeROIVPdu				      */
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

void sp0721_writeROIVPdu ( 
rT0710_stROIVapdu    FAR * pNode)
{ /* entr */

 sp1281_writePduHdr("INVOKE-PDU","ROPM");

 sp1255_wrSgLong(RC1201_OFFSET1,"INVOKE ID :",
                   &(pNode->lInvokeID),RC1206_DEC_NL);
 if (pNode->bLinkedID)
    sp1255_wrSgLong(RC1201_OFFSET1,"LINKED ID :",
                   &(pNode->lLinkedID),RC1206_DEC_NL);
 else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"LINKED ID:", 
                  (signed int)pNode->bLinkedID);
  
 sp1264_write (RC1201_OFFSET1, "%s\n", "---OPERATION VALUE  ---", 
                    NULL, NULL, NULL); 
 sp072a_writeOpValue(&pNode->stOperationValue);

 if(pNode->bArgument)
   sp1530_writeUctrData(pNode->hArgument);
 else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"ARGUMENT:", 
                  (signed int)pNode->bArgument);

}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0722_writeRORSPdu				      */
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

void sp0722_writeRORSPdu ( 
rT0712_stRORSapdu    FAR * pNode)
{ /* entr */

 sp1281_writePduHdr("RESULT-PDU","ROPM");

 sp1255_wrSgLong(RC1201_OFFSET1,"INVOKE ID :",
                   &(pNode->lInvokeID),RC1206_DEC_NL);
 if(pNode->bOperationResult){
   sp1264_write (RC1201_OFFSET1, "%s\n", "---OPERATION VALUE  ---", 
                    NULL, NULL, NULL); 
   sp072a_writeOpValue(&pNode->stOperationResult.stOperationValue);
   sp1530_writeUctrData(pNode->stOperationResult.hResult);
 }
 else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"OPERATION  RESULT:", 
                  (signed int)pNode->bOperationResult);
  
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0723_writeROERPdu				      */
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

void sp0723_writeROERPdu ( 
rT0713_stROERapdu    FAR * pNode)
{ /* entr */

 sp1281_writePduHdr("ERROR-PDU","ROPM");

 sp1255_wrSgLong(RC1201_OFFSET1,"INVOKE ID :",
                   &(pNode->lInvokeID),RC1206_DEC_NL);
  
 sp1264_write (RC1201_OFFSET1, "%s\n", "---ERROR VALUE  ---", 
                    NULL, NULL, NULL); 
 sp072a_writeOpValue(&pNode->stErrorValue);

 if(pNode->bParameter)
   sp1530_writeUctrData(pNode->hParameter);
 else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PARAMETER:", 
                  (signed int)pNode->bParameter);

}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0724_writeRORJPdu				      */
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

void sp0724_writeRORJPdu ( 
rT0714_stRORJapdu    FAR * pNode)
{ /* entr */

 sp1281_writePduHdr("REJECT-PDU","ROPM");
 
 switch(pNode->lInvokeIDChoiceNum){
    case RC071P_InvIdPrest:
       sp1255_wrSgLong(RC1201_OFFSET1,"OPERATION ID :",
                   &(pNode->lInvokeID),RC1206_DEC_NL);
       break;
    case RC071Q_NoInvId:
       sp1264_write (RC1201_OFFSET1, "%s\n", "OPERATION ID NOT PRESENT ", 
                    NULL, NULL, NULL); 
       break;
    default:
     sp1264_write (RC1201_OFFSET1, "%s\n", "---UNKNOWN VALUE  ---", 
                    NULL, NULL, NULL); 
 }
 sp072b_writeProblemChoice(pNode->lProblemChoiceNum,
                     pNode->lProblem);
 
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp072a_writeOpValue				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 01-12-93					      */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp072a_writeOpValue ( 
rTro76_stDefinedBy FAR * pOpVal)
/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
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
 switch(pOpVal->lType){
   case RC047a_DefinedByInteger:
      sp1264_write (RC1201_OFFSET1, "%s\n", "---INTEGER VALUE---", 
                    NULL, NULL, NULL); 
       sp1255_wrSgLong(RC1201_OFFSET1,"OPERATION VALUE :",
                   &(pOpVal->unValue.lInteger),RC1206_DEC_NL);
      break;
   case RC047b_DefinedByObjectId:
      sp1264_write (RC1201_OFFSET1, "%s\n", "---OBJECT ID FORM    ---", 
                    NULL, NULL, NULL); 
      sp1282_WrObjId(RC1202_OFFSET2,
                    (char FAR *)"OPERATION VALUE    :",     
                    (char FAR *)(pOpVal->unValue.oObjectId+1),           
                    (pOpVal->unValue.oObjectId[0]*           
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
/* NAME 	: sp072b_writeProblemChoice                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp072b_writeProblemChoice (
signed long int          lProblemType,
signed long int          lProblem)
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
  switch(lProblemType){
    case RC0712_GeneralProblem:
      sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM CHOICE:",
                        "GENERAL PROBLEM",NULL,NULL);
      switch(lProblem){
         case RC0716_unrecognisedAPDU:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "UNRECOGNISED APDU",NULL,NULL);
            break;
         case RC0717_mistypedAPDU:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "MISTYPED APDU",NULL,NULL);
            break;
         case RC0718_badlyStructuredAPDU:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "BADLY STRUCTURED APDU",NULL,NULL);
            break;
         default:
            sp1264_write(RC1201_OFFSET1,"%s%s","PROBLEM :",
                        "UNKNOWN/",NULL,NULL);
            sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,
                (signed int FAR *)&lProblem,RC1208_HEX_NL);
      }
      break;
    case RC0713_InvokeProblem:
      sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM CHOICE:",
                        "INVOKE PROBLEM",NULL,NULL);
      switch(lProblem){
         case RC0719_duplicateInvokation:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "DUPLICATE INVOKATION",NULL,NULL);
            break;
         case RC071A_unrecognisedOperation:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "UNRECOGNISED OPERATION",NULL,NULL);
            break;
         case RC071B_mistypedArgument:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "MISTYPED ARGUMENT",NULL,NULL);
            break;
         case RC071C_resourceLimitation:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "RESOURCE LIMITATION",NULL,NULL);
            break;
         case RC071D_initiatorReleasing:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "INITIATOR RELEASING",NULL,NULL);
            break;
         case RC071E_unrecognisedLinkedID:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "UNRECOGNISED LINKED ID",NULL,NULL);
            break;
         case RC071F_linkedResponseUnexpected:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "LINKED RESPONSE UNEXPECTED",NULL,NULL);
            break;
         case RC071G_unexpectedChildOperation:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "UNEXPECTED CHILD OPERATION",NULL,NULL);
            break;
         default:
            sp1264_write(RC1201_OFFSET1,"%s%s","PROBLEM :",
                        "UNKNOWN/",NULL,NULL);
            sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,
                (signed int FAR *)&lProblemType,RC1208_HEX_NL);
      }
      break;
    case RC0714_ReturnResultProblem:
      sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM CHOICE:",
                        "RETURN RESULT PROBLEM",NULL,NULL);
      switch(lProblem){
         case RC071H_unrecognisedInvocation:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "UNRECOGNISED INVOCATION",NULL,NULL);
            break;
         case RC071I_resultResponseUnexpected:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "RESULT RESPONSE UNEXPECTED",NULL,NULL);
            break;
         case RC071J_mistypedResult:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "MISTYPED RESULT",NULL,NULL);
            break;
         default:
            sp1264_write(RC1201_OFFSET1,"%s%s","PROBLEM :",
                        "UNKNOWN/",NULL,NULL);
            sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,
                (signed int FAR *)&lProblemType,RC1208_HEX_NL);
      }
      break;
    case RC0715_ReturnErrorProblem:
      sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM CHOICE:",
                        "RETURN ERROR PROBLEM",NULL,NULL);
      switch(lProblem){
         case RC071K_unrecognisedInvocation:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "UNRECOGNISED INVOCATION",NULL,NULL);
            break;
         case RC071L_ErrorResponseUnexpected:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "ERROR RESPONSE UNEXPECTED",NULL,NULL);
            break;
         case RC071M_UnrecognisedError:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "UNRECOGNISED ERROR",NULL,NULL);
            break;
         case RC071N_UnexpectedError:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "UNEXPECTED ERROR",NULL,NULL);
            break;
         case RC071O_mistypedPrarameter:
            sp1264_write(RC1201_OFFSET1,"%s%s\n","PROBLEM :",
                        "MISTYPED PRARAMETER",NULL,NULL);
            break;
         default:
            sp1264_write(RC1201_OFFSET1,"%s%s","PROBLEM :",
                        "UNKNOWN/",NULL,NULL);
            sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,
                (signed int FAR *)&lProblemType,RC1208_HEX_NL);
      }
      break;
    default:
      sp1264_write(RC1201_OFFSET1,"%s%s","PROBLEM CHOICE:",
                        "UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,
                (signed int FAR *)&lProblemType,RC1208_HEX_NL);
      break; 
  }
}/*end*/
 
