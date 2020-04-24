/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spprpm5.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:49  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:08:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:31:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:17:58  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:53  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:38  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:30  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:53:43  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:33:59  keutel
 * 	creation
 * 	[1994/03/21  13:23:23  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spprpm5.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:49 $" } ;
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
/* NAME 	: spprpm5.c					      */
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

static char *sccsid =    "@(#) spprpm5.c 1.1 93/12/20   ";

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
#include <rospm00.h>
#include <routil0.h>
#include <rostor0.h>

#include <spacpm0.h>
#include <spropm0.h>

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spprpm0.h>	  /* Function prototyping		      */

/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/

void sp0451_writeCpPdu PROTO ((
rT041A_stCp    FAR * pNode));
void sp0452_writeCpaPdu PROTO ((
rT041B_stCpa    FAR * pNode));
void sp0453_writeCprPdu PROTO ((
rT041C_stCpr    FAR * pNode));
void sp0454_writeAruPdu PROTO ((
rT041D_stAru    FAR * pNode));
void sp0455_writeArpPdu PROTO ((
rT041E_stArp    FAR * pNode));
void sp0456_writeOtherPdu PROTO ((
rT041F_stOther    FAR * pNode,
unsigned    int         uEvent));

void sp045a_writeResult PROTO ((
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue));
void sp045b_writeProvReason PROTO ((
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue));
void sp045c_writeCPRproviderReason PROTO ((
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue));
void sp045e_writeAbortReason PROTO ((
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue));
void sp045d_writeX410orNormal PROTO ((
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue));
void sp045f_writeEventId PROTO ((
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue));
/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0450_writePdus				      */
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
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 13-03-92| original		       | ns |	      */
/*           |         |                               |    |         */
/*inoff ***************************************************************/

void sp0450_writePdus ( 
	unsigned long int hNode,
	unsigned      int uEvent)
{/* entr */
  rT041G_unPpdu  FAR * pNode; 
  if (hNode) {
     pNode = (rT041G_unPpdu FAR *) ro0021_globallock (hNode); 
   
     switch(pNode->stPdvHeader.lType){
      case RC041A_Cp:
        sp0451_writeCpPdu ((rT041A_stCp    FAR *)pNode);
        break;
      case RC041B_Cpa:
        sp0452_writeCpaPdu ((rT041B_stCpa    FAR *)pNode);
        break;
      case RC041C_Cpr:
        sp0453_writeCprPdu ((rT041C_stCpr    FAR *)pNode);
        break;
      case RC041D_Aru:
        sp0454_writeAruPdu ((rT041D_stAru    FAR *)pNode);
        break;
      case RC041E_Arp:
        sp0455_writeArpPdu ((rT041E_stArp    FAR *)pNode);
        break;
      case RC041F_Other:
        sp0456_writeOtherPdu ((rT041F_stOther    FAR *)pNode,uEvent);
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
/* NAME 	: sp0451_writeCpPdu				      */
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
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 13-03-92| original		       | ns |	      */
/*           |         |                               |    |         */
/*inoff ***************************************************************/

void sp0451_writeCpPdu ( 
	rT041A_stCp    FAR * pNode)
{ /* entr */
 unsigned long int i,j;

 sp1281_writePduHdr("CP-PPDU","PRESENTATION");
 sp1255_wrSgLong  (RC1201_OFFSET1,"MODE SELECTOR     :",
                   (signed long int FAR *)&pNode->lModeSelector,
                   RC1206_DEC_NL);
 sp1280_writeBool((signed int)RC1201_OFFSET1,"NORMAL SEQUENCE    :", 
                  (signed int)pNode->bNormalSEQUENCE);
 if (pNode->bsProtVersion )
    sp1257_wrUnLong(RC1201_OFFSET1,"PROTOCOL-VERSION :",
                    (unsigned long int FAR *)&pNode->ulProtVersion,
                    RC1206_DEC_NL);
 else  
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PROTOCOL- VERSION    :", 
                  (signed int)pNode->bsProtVersion);
 if (pNode->bCallingSelector){
    sp1257_wrUnLong(RC1201_OFFSET1,"CALLING SELECTOR LENGTH :",
                    (unsigned long int FAR *)&pNode->ulCallingSelector,
                    RC1206_DEC_NL);
    sp1263_hexDump(RC1201_OFFSET1,"CALLING SELECTOR :",
                   (char FAR *) pNode->vCallingSelector,
                   pNode->ulCallingSelector);
    
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"CALLING SELECTOR     :", 
                  (signed int)pNode->bCallingSelector);
 if (pNode->bCalledSelector){
    sp1257_wrUnLong(RC1201_OFFSET1,"CALLED SELECTOR LENGTH :",
                    (unsigned long int FAR *)&pNode->ulCalledSelector,
                    RC1206_DEC_NL);
    sp1263_hexDump(RC1201_OFFSET1,"CALLED SELECTOR :",
                   (char FAR *) pNode->vCalledSelector,pNode->ulCalledSelector);
    
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"CALLED SELECTOR     :", 
                  (signed int)pNode->bCalledSelector);
 if (pNode->bPresContDef){
    sp1257_wrUnLong(RC1201_OFFSET1,"PRES. CONT. DEF.NUMBER :",
                    (unsigned long int FAR *)&pNode->ulPresContDef,
                    RC1206_DEC_NL);
    for (i=0; i < pNode->ulPresContDef; i++ ){
        sp1255_wrSgLong (RC1202_OFFSET2 , "PRES. CONT. ID          : ",  
                 (signed long int FAR *)&pNode->vPresContDef[i].lPresContId, 
                   RC1206_DEC_NL); 
        sp1282_WrObjId(RC1202_OFFSET2,"ABSTRACT SYNTAX NAME :",
                (char FAR *)(pNode->vPresContDef[i].oAbstrSynName+1), 
                (pNode->vPresContDef[i].oAbstrSynName[0]*
                 sizeof(unsigned long int) ));
        sp1257_wrUnLong(RC1202_OFFSET2,"NUM. OF TRANSFER SYN/ABS:",
                    (unsigned long int FAR *)
                      &pNode->vPresContDef[i].ulTransSynNumber,
                    RC1206_DEC_NL);
        for (j=0;j<pNode->vPresContDef[i].ulTransSynNumber;j++)
          sp1282_WrObjId(RC1202_OFFSET2,"TRANSFER SYNTAX NAME :",
                (char FAR *)(pNode->vPresContDef[i].vTransSynName[j]+1), 
                (pNode->vPresContDef[i].vTransSynName[j][0]*
                   sizeof(unsigned long int)));
             
    }
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PRES. CONT. DEF     :", 
                  (signed int)pNode->bPresContDef);
 if(pNode->bDefaultContext){
   sp1282_WrObjId(RC1201_OFFSET1,"DEFAULT ABSTRACT SYNTAX NAME :",
                (char FAR *)(pNode->oDefaultAbstrSyn+1), 
                (pNode->oDefaultAbstrSyn[0]*
                  sizeof(unsigned long int)));
   sp1282_WrObjId(RC1201_OFFSET1,"DEFAULT TRANSFER SYNTAX NAME :",
                (char FAR *)(pNode->oDefaultTransSyn+1), 
                (pNode->oDefaultTransSyn[0]*
                 sizeof(unsigned long int)));
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"DEFAULT CONTEXT     :", 
                  (signed int)pNode->bDefaultContext);
 if(pNode->bPresRequirements){
    sp1257_wrUnLong(RC1202_OFFSET2,"PRES. REQUIREMENTS      :",
                    (unsigned long int FAR *)
                      &pNode->ulPresRequirements,
                    RC1206_DEC_NL);
    sp1257_wrUnLong(RC1202_OFFSET2,"PRES. REQUIREMENTS      :",
                    (unsigned long int FAR *)
                      &pNode->bsPresRequirements,
                    RC1206_DEC_NL);
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PRES. REQUIREMENTS  :", 
                  (signed int)pNode->bPresRequirements);
   
 if(pNode->bSessRequirements){
    sp1257_wrUnLong(RC1202_OFFSET2,"SESS. REQUIREMENTS      :",
                    (unsigned long int FAR *)
                      &pNode->ulSessRequirements,
                    RC1206_DEC_NL);
    sp1257_wrUnLong(RC1202_OFFSET2,"SESS. REQUIREMENTS      :",
                    (unsigned long int FAR *)
                      &pNode->bsPresRequirements,
                    RC1206_DEC_NL);
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"SESS. REQUIREMENTS  :", 
                  (signed int)pNode->bSessRequirements);
 if(pNode->bUserData){
    switch(pNode->stUserData.lSimplyFullyChoice){
       case RC0417_SimplyEncoded:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "SIMPLY ENCODED", NULL, NULL);
                 sp0620_writeAcpmPdu(
            pNode->stUserData.unUserData.hSimply);
             break;                                           
       case RC0418_FullyEncoded:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "FULLY ENCODED", NULL, NULL);
            
             sp1257_wrUnLong(RC1201_OFFSET1,"NUM. OF PDV LIST        :",
                    (unsigned long int FAR *)
                      &pNode->stUserData.unUserData.stFully.ulPdvList,
                    RC1206_DEC_NL);
             for (i=0;i< pNode->stUserData.unUserData.stFully.ulPdvList;
                  i++)
                 sp0620_writeAcpmPdu(
            pNode->stUserData.unUserData.stFully.vPdvList[i]);
             break;                                           
       default:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "UNKNOWN ENCODED", NULL, NULL);
             break;                                           
    } 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"USER DATA         	 :", 
                  (signed int)pNode->bUserData);
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0452_writeCpaPdu				      */
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
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 13-03-92| original		       | ns |	      */
/*           |         |                               |    |         */
/*inoff ***************************************************************/

void sp0452_writeCpaPdu (
	rT041B_stCpa    FAR * pNode)
{ /* entr */
 unsigned long int i;

 sp1281_writePduHdr("CPA-PPDU","PRESENTATION");
 sp1255_wrSgLong  (RC1201_OFFSET1,"MODE SELECTOR     :",
                   (signed long int FAR *)&pNode->lModeSelector,
                   RC1206_DEC_NL);
 sp1280_writeBool((signed int)RC1201_OFFSET1,"NORMAL SEQUENCE    :", 
                  (signed int)pNode->bNormalSEQUENCE);
 if (pNode->bsProtVersion )
    sp1257_wrUnLong(RC1201_OFFSET1,"PROTOCOL-VERSION :",
                    (unsigned long int FAR *)&pNode->ulProtVersion,
                    RC1206_DEC_NL);
 else  
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PROTOCOL- VERSION    :", 
                  (signed int)pNode->bsProtVersion);
 if (pNode->bRespondingSelector){
    sp1257_wrUnLong(RC1201_OFFSET1,"RESPONDING SELECTOR LENGTH :",
                    (unsigned long int FAR *)&pNode->ulRespondingSelector,
                    RC1206_DEC_NL);
    sp1263_hexDump(RC1201_OFFSET1,"RESPONDING SELECTOR :",
                   (char FAR *) pNode->vRespondingSelector,
                   pNode->ulRespondingSelector);
    
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"RESPONDING SELECTOR     :", 
                  (signed int)pNode->bRespondingSelector);
 if (pNode->bPresContDefRes){
    sp1257_wrUnLong(RC1201_OFFSET1,"PRES. CONT. DEF.NUMBER :",
                    (unsigned long int FAR *)&pNode->ulPresContDefRes,
                    RC1206_DEC_NL);
    for (i=0; i < pNode->ulPresContDefRes; i++ ){
        sp045a_writeResult(RC1202_OFFSET2,"PRES.CONT.DEF.RESULT: ",
                   pNode->vPresContDefRes[i].lResult);

        sp1282_WrObjId(RC1202_OFFSET2,"TRANSFER SYNTAX NAME :",
                (char FAR *)(pNode->vPresContDefRes[i].oTransSynName+1), 
                (pNode->vPresContDefRes[i].oTransSynName[0]*
                 sizeof(unsigned long int) ));
        if(pNode->vPresContDefRes[i].bProviderReason)
           sp045b_writeProvReason(RC1202_OFFSET2,"PROVIDER REASON :",
                 pNode->vPresContDefRes[i].lProviderReason);
        else  
           sp1280_writeBool((signed int)RC1201_OFFSET1,"PROVIDER REASON    :", 
                          (signed int)pNode->vPresContDefRes[i].bProviderReason);
    }
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PRES. CONT. DEF.RES   :", 
                  (signed int)pNode->bPresContDefRes);
 if(pNode->bPresRequirements){
    sp1257_wrUnLong(RC1202_OFFSET2,"PRES. REQUIREMENTS      :",
                    (unsigned long int FAR *)
                      &pNode->ulPresRequirements,
                    RC1206_DEC_NL);
    sp1257_wrUnLong(RC1202_OFFSET2,"PRES. REQUIREMENTS      :",
                    (unsigned long int FAR *)
                      &pNode->bsPresRequirements,
                    RC1206_DEC_NL);
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PRES. REQUIREMENTS  :", 
                  (signed int)pNode->bPresRequirements);
   
 if(pNode->bSessRequirements){
    sp1257_wrUnLong(RC1202_OFFSET2,"SESS. REQUIREMENTS      :",
                    (unsigned long int FAR *)
                      &pNode->ulSessRequirements,
                    RC1206_DEC_NL);
    sp1257_wrUnLong(RC1202_OFFSET2,"SESS. REQUIREMENTS      :",
                    (unsigned long int FAR *)
                      &pNode->bsPresRequirements,
                    RC1206_DEC_NL);
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"SESS. REQUIREMENTS  :", 
                  (signed int)pNode->bSessRequirements);
 if(pNode->bUserData){
    switch(pNode->stUserData.lSimplyFullyChoice){
       case RC0417_SimplyEncoded:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "SIMPLY ENCODED", NULL, NULL);
                 sp0620_writeAcpmPdu(
            pNode->stUserData.unUserData.hSimply);
             break;                                           
       case RC0418_FullyEncoded:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "FULLY ENCODED", NULL, NULL);
            
             sp1257_wrUnLong(RC1201_OFFSET1,"NUM. OF PDV LIST        :",
                    (unsigned long int FAR *)
                      &pNode->stUserData.unUserData.stFully.ulPdvList,
                    RC1206_DEC_NL);
             for (i=0;i< pNode->stUserData.unUserData.stFully.ulPdvList;
                  i++)
                 sp0620_writeAcpmPdu(
            pNode->stUserData.unUserData.stFully.vPdvList[i]);
             break;                                           
       default:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "UNKNOWN ENCODED", NULL, NULL);
             break;                                           
    } 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"USER DATA         	 :", 
                  (signed int)pNode->bUserData);
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0453_writeCprPdu				      */
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
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 13-03-92| original		       | ns |	      */
/*           |         |                               |    |         */
/*inoff ***************************************************************/

void sp0453_writeCprPdu ( 
	rT041C_stCpr    FAR * pNode)
{ /* entr */
 unsigned long int i;

 sp1281_writePduHdr("CPR-PPDU","PRESENTATION");
 sp045d_writeX410orNormal(RC1201_OFFSET1,"CHOICE OF X410 or NORMAL:",
        pNode->lX410orNormalChoice);
 
 sp1257_wrUnLong(RC1201_OFFSET1,"HANDLE of X410 MODE        :",
                    (unsigned long int FAR *)&pNode->hX410RtpmNode,
                    RC1206_DEC_NL);
 if (pNode->bsProtVersion )
    sp1257_wrUnLong(RC1201_OFFSET1,"PROTOCOL-VERSION :",
                    (unsigned long int FAR *)&pNode->ulProtVersion,
                    RC1206_DEC_NL);
 else  
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PROTOCOL- VERSION    :", 
                  (signed int)pNode->bsProtVersion);
 if (pNode->bRespondingSelector){
    sp1257_wrUnLong(RC1201_OFFSET1,"RESPONDING SELECTOR LENGTH :",
                    (unsigned long int FAR *)&pNode->ulRespondingSelector,
                    RC1206_DEC_NL);
    sp1263_hexDump(RC1201_OFFSET1,"RESPONDING SELECTOR :",
                   (char FAR *) pNode->vRespondingSelector,
                   pNode->ulRespondingSelector);
    
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"RESPONDING SELECTOR     :", 
                  (signed int)pNode->bRespondingSelector);
 if (pNode->bPresContDefRes){
    sp1257_wrUnLong(RC1201_OFFSET1,"PRES. CONT. DEF.NUMBER :",
                    (unsigned long int FAR *)&pNode->ulPresContDefRes,
                    RC1206_DEC_NL);
    for (i=0; i < pNode->ulPresContDefRes; i++ ){
        sp045a_writeResult(RC1202_OFFSET2,"PRES.CONT.DEF.RESULT",
                   pNode->vPresContDefRes[i].lResult);

        sp1282_WrObjId(RC1202_OFFSET2,"TRANSFER SYNTAX NAME :",
                (char FAR *)(pNode->vPresContDefRes[i].oTransSynName+1), 
                (pNode->vPresContDefRes[i].oTransSynName[0]*
                 sizeof(unsigned long int) ));
        if(pNode->vPresContDefRes[i].bProviderReason)
           sp045b_writeProvReason(RC1202_OFFSET2,"PROVIDER REASON :",
                 pNode->vPresContDefRes[i].lProviderReason);
        else  
           sp1280_writeBool((signed int)RC1201_OFFSET1,"PROVIDER REASON    :", 
                          (signed int)pNode->vPresContDefRes[i].bProviderReason);
    }
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PRES. CONT. DEF.RES   :", 
                  (signed int)pNode->bPresContDefRes);
 if(pNode->bDefaultContextResult)
    sp1255_wrSgLong  (RC1201_OFFSET1,"DEFAULT CONT. RESULT   :",
                   (signed long int FAR *)&pNode->lDefaultContextResult,
                   RC1206_DEC_NL);
 else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"DEFAULT CONT. RESULT  :", 
                  (signed int)pNode->bPresContDefRes);
 
 if(pNode->bProviderReason)
    sp045c_writeCPRproviderReason(RC1201_OFFSET1,"PROVIDER REASON:",
                  pNode->lProviderReason);
 else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PROVIDER REASON  :", 
                  (signed int)pNode->bProviderReason);

 if(pNode->bUserData){
    switch(pNode->stUserData.lSimplyFullyChoice){
       case RC0417_SimplyEncoded:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "SIMPLY ENCODED", NULL, NULL);
                 sp0620_writeAcpmPdu(
            pNode->stUserData.unUserData.hSimply);
             break;                                           
       case RC0418_FullyEncoded:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "FULLY ENCODED", NULL, NULL);
            
             sp1257_wrUnLong(RC1201_OFFSET1,"NUM. OF PDV LIST        :",
                    (unsigned long int FAR *)
                      &pNode->stUserData.unUserData.stFully.ulPdvList,
                    RC1206_DEC_NL);
             for (i=0;i< pNode->stUserData.unUserData.stFully.ulPdvList;
                  i++)
                 sp0620_writeAcpmPdu(
            pNode->stUserData.unUserData.stFully.vPdvList[i]);
             break;                                           
       default:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "UNKNOWN ENCODED", NULL, NULL);
             break;                                           
    } 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"USER DATA         	 :", 
                  (signed int)pNode->bUserData);
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0454_writeAruPdu				      */
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
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 13-03-92| original		       | ns |	      */
/*           |         |                               |    |         */
/*inoff ***************************************************************/

void sp0454_writeAruPdu ( 
	rT041D_stAru    FAR * pNode)
{ /* entr */
 unsigned long int i;

 sp1281_writePduHdr("ARU-PPDU","PRESENTATION");
 sp045d_writeX410orNormal(RC1201_OFFSET1,"CHOICE OF X410 or NORMAL:",
        pNode->lX410orNormalChoice);
 
 sp1257_wrUnLong(RC1201_OFFSET1,"HANDLE of X410 MODE        :",
                    (unsigned long int FAR *)&pNode->hX410RtpmNode,
                    RC1206_DEC_NL);

 if (pNode->bPresContId){
    sp1257_wrUnLong(RC1201_OFFSET1,"PRES. CONT. ID.NUMBER :",
                    (unsigned long int FAR *)&pNode->ulPresContId,
                    RC1206_DEC_NL);
    for (i=0; i < pNode->ulPresContId; i++ ){
        sp1255_wrSgLong (RC1202_OFFSET2 , "PRES. CONT. ID          : ",  
                 (signed long int FAR *)&pNode->vPresContId[i].lPresContId, 
                   RC1206_DEC_NL); 
        sp1282_WrObjId(RC1202_OFFSET2,"TRANSFER SYNTAX NAME :",
                (char FAR *)(pNode->vPresContId[i].oTransSynName+1), 
                (pNode->vPresContId[i].oTransSynName[0]*
                 sizeof(unsigned long int) ));
             
    }
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PRES. CONT. ID     :", 
                  (signed int)pNode->bPresContId);
 if(pNode->bUserData){
    switch(pNode->stUserData.lSimplyFullyChoice){
       case RC0417_SimplyEncoded:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "SIMPLY ENCODED", NULL, NULL);
                 sp0620_writeAcpmPdu(
            pNode->stUserData.unUserData.hSimply);
             break;                                           
       case RC0418_FullyEncoded:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "FULLY ENCODED", NULL, NULL);
            
             sp1257_wrUnLong(RC1201_OFFSET1,"NUM. OF PDV LIST        :",
                    (unsigned long int FAR *)
                      &pNode->stUserData.unUserData.stFully.ulPdvList,
                    RC1206_DEC_NL);
             for (i=0;i< pNode->stUserData.unUserData.stFully.ulPdvList;
                  i++)
                 sp0620_writeAcpmPdu(
            pNode->stUserData.unUserData.stFully.vPdvList[i]);
             break;                                           
       default:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "UNKNOWN ENCODED", NULL, NULL);
             break;                                           
    } 
 }else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"USER DATA         	 :", 
                  (signed int)pNode->bUserData);
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0455_writeArpPdu				      */
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
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 13-03-92| original		       | ns |	      */
/*           |         |                               |    |         */
/*inoff ***************************************************************/

void sp0455_writeArpPdu ( 
	rT041E_stArp    FAR * pNode)
{ /* entr */


 sp1281_writePduHdr("ARP-PPDU","PRESENTATION");
 if(pNode->bAbortReason)
   sp045e_writeAbortReason(RC1201_OFFSET1,"PROVIDER ABORT REASON:",
         pNode->lAbortReason);
 else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"PROVIDER ABORT REASON:", 
                  (signed int)pNode->bAbortReason);
 if(pNode->bEventId)
   sp045f_writeEventId(RC1201_OFFSET1,"EVENT ID       :",
          pNode->lEventId);
 else 
    sp1280_writeBool((signed int)RC1201_OFFSET1,"EVENT ID             :", 
                  (signed int)pNode->bEventId);
}/* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0456_writeOtherPdu				      */
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
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 13-03-92| original		       | ns |	      */
/*           |         |                               |    |         */
/*inoff ***************************************************************/

void sp0456_writeOtherPdu ( 
	rT041F_stOther    FAR * pNode,
	unsigned    int         uEvent)
{ /* entr */
 unsigned long int i;

 sp1281_writePduHdr("OTHER-PPDU","PRESENTATION");
    switch(pNode->stUserData.lSimplyFullyChoice){
       case RC0417_SimplyEncoded:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "SIMPLY ENCODED", NULL, NULL);
             if(uEvent == RC080d_SDATAind)
                 sp0720_writeRopmPdu(
                     pNode->stUserData.unUserData.hSimply);
             else 
                 sp0620_writeAcpmPdu(
                  pNode->stUserData.unUserData.hSimply);
             break;                                           
       case RC0418_FullyEncoded:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "FULLY ENCODED", NULL, NULL);
            
             sp1257_wrUnLong(RC1201_OFFSET1,"NUM. OF PDV LIST        :",
                    (unsigned long int FAR *)
                      &pNode->stUserData.unUserData.stFully.ulPdvList,
                    RC1206_DEC_NL);
             for (i=0;i< pNode->stUserData.unUserData.stFully.ulPdvList;
                  i++)
                 if(uEvent == RC080d_SDATAind)
                 sp0720_writeRopmPdu(
                     pNode->stUserData.unUserData.stFully.vPdvList[i]);
                 else 
                 sp0620_writeAcpmPdu(
                   pNode->stUserData.unUserData.stFully.vPdvList[i]);
             break;                                           
       default:
             sp1264_write (RC1201_OFFSET1, "%s%s\n", "USER DATA CHOICE:", 
                 "UNKNOWN ENCODED", NULL, NULL);
             break;                                           
     }
}/* end */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp045a_writeResult                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp045a_writeResult (
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
    case RC0410_Acceptance:
      sp1264_write(iOffset,"%s%s\n",pStr,"ACCEPTANCE",NULL,NULL);
      break;
    case RC0411_UserRejection:
      sp1264_write(iOffset,"%s%s\n",pStr,"USER REJECTION",NULL,NULL);
      break;
    case RC0412_ProviderRejection:
      sp1264_write(iOffset,"%s%s\n",pStr,"PROVIDER REJECTION",NULL,NULL);
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
/* NAME 	: sp045b_writeProvReason                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp045b_writeProvReason (
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
    case RC0413_ReasonNotSpecified:
      sp1264_write(iOffset,"%s%s\n",pStr,"REASON NOT SPECIFIED",NULL,NULL);
      break;
    case RC0414_AbstrSynNotSupported:
      sp1264_write(iOffset,"%s%s\n",pStr,"ABS.SYN NOT SUPPORTED",NULL,NULL);
      break;
    case RC0415_TransSynNotSupported:
      sp1264_write(iOffset,"%s%s\n",pStr,"TRASN.SYN NOT SUPPORTED",NULL,NULL);
      break;
    case RC0416_LocalDcsLimitExceeded:
      sp1264_write(iOffset,"%s%s\n",pStr,"LOCAL DCS LIMIT EXCEEDED",NULL,NULL);
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
/* NAME 	: sp045c_writeCPRproviderReason                       */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp045c_writeCPRproviderReason (
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
    case RC041c_ReasonNotSpecified:
      sp1264_write(iOffset,"%s%s\n",pStr,"REASON NOT SPECIFIED",NULL,NULL);
      break;
    case RC041d_TemporaryCongestion:
      sp1264_write(iOffset,"%s%s\n",pStr,"TEMPORARY CONGESTION",NULL,NULL);
      break;
    case RC041e_LocalLimitExceeded:
      sp1264_write(iOffset,"%s%s\n",pStr,"LOCAL LIMIT EXCEEDED",NULL,NULL);
      break;
    case RC041f_CalledPaddressUnknown:
      sp1264_write(iOffset,"%s%s\n",pStr,"CALLED P-ADDRESS UNKNOWN",NULL,NULL);
      break;
    case RC041g_ProtVersionNotSuppo:
      sp1264_write(iOffset,"%s%s\n",pStr,"PROT VERSION NOT SUPPORTED",
       NULL,NULL);
      break;
    case RC041h_DefaultContNotSuppo:
      sp1264_write(iOffset,"%s%s\n",pStr,"DEFAULT CONT. NOT SUPPORTED",
       NULL,NULL);
      break;
    case RC041i_UserDataNotReadable:
      sp1264_write(iOffset,"%s%s\n",pStr,"USER DATA NOT READABLE",NULL,NULL);
      break;
    case RC041j_NoPSAPavailable:
      sp1264_write(iOffset,"%s%s\n",pStr,"NO PSAP AVAILABLE",NULL,NULL);
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
/* NAME 	: sp045d_writeX410orNormal                            */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp045d_writeX410orNormal (
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
    case RC041a_X410Mode:
      sp1264_write(iOffset,"%s%s\n",pStr,"X410MODE",NULL,NULL);
      break;
    case RC041b_NormalMode:
      sp1264_write(iOffset,"%s%s\n",pStr,"NORMALMODE",NULL,NULL);
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
/* NAME 	: sp045e_writeAbortReason                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp045e_writeAbortReason (
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
    case RC041k_ReasonNotSpecified:
      sp1264_write(iOffset,"%s%s\n",pStr,"REASON NOT SPECIFIED",NULL,NULL);
      break;
    case RC041l_UnrecognizedPpdu:
      sp1264_write(iOffset,"%s%s\n",pStr,"UNRECOGNIZED PPDU",NULL,NULL);
      break;
    case RC041m_UnexpectedPpdu:
      sp1264_write(iOffset,"%s%s\n",pStr,"UNEXPECTED PPDU",NULL,NULL);
      break;
    case RC041n_UnexpectedSessEvent:
      sp1264_write(iOffset,"%s%s\n",pStr,"UNEXPECTED SESS EVENT",NULL,NULL);
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
/* NAME 	: sp045f_writeEventId                                 */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 01-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp045f_writeEventId (
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
    case RC041A_Cp:
      sp1264_write(iOffset,"%s%s\n",pStr,"CP",NULL,NULL);
      break;
    case RC041B_Cpa:
      sp1264_write(iOffset,"%s%s\n",pStr,"CPA",NULL,NULL);
      break;
    case RC041C_Cpr:
      sp1264_write(iOffset,"%s%s\n",pStr,"CPR",NULL,NULL);
      break;
    case RC041D_Aru:
      sp1264_write(iOffset,"%s%s\n",pStr,"ARU",NULL,NULL);
      break;
    case RC041E_Arp:
      sp1264_write(iOffset,"%s%s\n",pStr,"ARP",NULL,NULL);
      break;
    case RC041F_Other:
      sp1264_write(iOffset,"%s%s\n",pStr,"OTHER",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,(signed int FAR *)&ulValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/
 

