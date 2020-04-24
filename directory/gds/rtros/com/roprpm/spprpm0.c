/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spprpm0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:42  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:08:52  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:31:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:17:45  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:46  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:29  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:24  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:53:07  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:33:48  keutel
 * 	creation
 * 	[1994/03/21  13:22:14  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spprpm0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:42 $" } ;
#endif

/**********************************************************************/
/*                                                                    */
/*    COPYRIGHT  (C)						      */
/*    Siemens Nixdorf Informationssysteme AG 1991		      */
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
/* NAME 	: spprpm0.c					      */
/*                                                                    */
/* AUTHOR	: Natalia Serra  , Chandrasekar.K                     */
/* DATE 	: 13-03-92					      */
/*                                                                    */
/* COMPONENT    : RTS Evaluation Program                              */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : In this module the trace record types are evaluated */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 13-03-92| original		       | ns |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spprpm0.c 1.2 93/12/20   RTS-V2.00";

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
#include <rodpat0.h>
#include <roppm04.h>
#include <roppm03.h>
#include <roppm00.h>
#include <roppm01.h>
#include <roppmz0.h>
#include <roppm12.h>
#include <rospm00.h>
#include <spspm0.h>
#include <routil0.h>
#include <rostor0.h>

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spprpm0.h>	  /* Function prototyping		      */

/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/
char FAR *sp0401_getPrpmEvent PROTO ((unsigned int)); 

void sp0402_writeState PROTO ((
int          iOffset,
char    FAR *pStr,
int          iValue));
void sp0403_getEvent PROTO ((
unsigned int FAR * pInEvent));

/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0400_tracePrpm()				      */
/*                                                                    */
/* AUTHOR	: Natalia Serra 				      */
/* DATE 	: 13-03-92					      */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS: type, time, argn, argl, argv			      */
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

signed short int sp0400_tracePrpm (
	long int	               type,
	long int	               time,
	unsigned short int             argn,
	char                FAR * FAR *argv,
	unsigned long int         FAR *argl,
	unsigned short int             nerr)
{
   signed   short int	   retVal;
   unsigned int       FAR *p;
   rT0430_stContext   FAR *pp;
   char 		   str[30];
   signed long int	   lStr[RCro01_MaxAbstrSyn];
   int		           i;

   static  signed long int lLen=0;
   static  rT0430_stContext    stCxt;
   static  rTro76_stDefinedBy  stDefinedBy;
   static  rT0441_stEntContext stEntContext;
   static  rTro00_vObjectId    vTransSynName;
   static  unsigned long int   ulOffset;
   static  unsigned      int   uInEvent;
   static  unsigned long int hStore =0;
   static  unsigned long int hNode =0;

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   retVal   = OK;
   switch(type){
      case RL0401_ActivityPpm:

      sp1251_recordHead (RC1200_OFFSET0, "R_ACTIVITY", "(P-PM)",
			 type, time, nerr);
      if (argn == 8) {

	 /*--------------------------------*/
	 /* len: (int)	      sizeof(hEcb) */
	 /* val: (char FAR *) &hEcb	   */
	 /*--------------------------------*/

	 if (argl[0] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "PPM.APPLID  : ",
			    (unsigned long int FAR *)argv[0], RC1206_DEC_NL);
	 }

	 /*--------------------------------*/
	 /* len: (int)	      sizeof(hAcb) */
	 /* val: (char FAR *) &hAcb	   */
	 /*--------------------------------*/

	 if (argl[1] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "PPM.ASSOCID  : ",
			    (unsigned long int FAR *)argv[1], RC1206_DEC_NL);
	 }

	 /*--------------------------------*/
	 /* len: (int)	      sizeof(ulP1) */
	 /* val: (char FAR *) &ulP1	   */
	 /*--------------------------------*/

	 if (argl[2] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "PPM.p1   : ",
			     (unsigned long int FAR *)argv[2], RC1206_DEC_NL);
	 }

	 /*--------------------------------*/
	 /* len: (int)	      sizeof(ulP2) */
	 /* val: (char FAR *) &ulP2	   */
	 /*--------------------------------*/

	 if (argl[3] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "PPM.p2   : ",
			     (unsigned long int FAR *)argv[3], RC1206_DEC_NL);
            ulOffset = (unsigned long int)*(unsigned long int FAR *)argv[3];
	 }

	 /*--------------------------------*/
	 /* len: (int)	      sizeof(ulP3) */
	 /* val: (char FAR *) &ulP3	   */
	 /*--------------------------------*/

	 if (argl[4] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "PPM.p3   : ",
			     (unsigned long int FAR *)argv[4], RC1206_DEC_NL);
	 }

	 /*--------------------------------*/
	 /* len: (int)	      sizeof(ulP4) */
	 /* val: (char FAR *) &ulP4	   */
	 /*--------------------------------*/

	 if (argl[5] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "PPM.p4   : ",
			     (unsigned long int FAR *)argv[5], RC1206_DEC_NL);
	 }

	 /*------------------------------------*/
	 /* len: (int)	      sizeof(uInEvent) */
	 /* val: (char FAR *) &uInEvent        */
	 /*------------------------------------*/

	 if (argl[6] > 0) {
	    p = (unsigned int FAR *)argv[6];
            uInEvent = *p;

	    ro0050_strcpy ((char FAR *)str, (char FAR *)sp0401_getPrpmEvent(*p));
	    sp1256_wrUnInt (RC1202_OFFSET2, "PPM.event: ", (unsigned int FAR *)p, RC1207_HEX);
	    sp1264_write (RC1201_OFFSET1, "%s\n", str, NULL, NULL, NULL);
            if ( *p == RC080b_SPABORTind ||
                 *p == RC080a_SUABORTind   )
               if (argl[5] > 0 )
                 sp1290_writeAbortReason((unsigned long int)
                      *(unsigned long int FAR *)argv[5]); 
	 }

	 /*--------------------------------------------*/
	 /* len: (int)	      sizeof(rT0430_stContext) */
	 /* val: (char FAR *) pContext		       */
	 /*--------------------------------------------*/

	 if (argl[7] > 0) {
	    pp = (rT0430_stContext FAR *)argv[7];
            ro0041_memcpy (( char FAR *) &stCxt ,
                           ( char FAR *) pp,
                           (unsigned long int ) sizeof(rT0430_stContext));
 
	    sp0402_writeState(RC1202_OFFSET2, "PPM.context.sState           : ",
			     pp->sState);
	    sp1280_writeBool (RC1202_OFFSET2, "PPM.context.bRelease         : ",
			     pp->bRelease);
	    sp1280_writeBool (RC1202_OFFSET2, "PPM.context.bReleaseCollision: ",
			     pp->bReleaseCollision );
	    sp1280_writeBool (RC1202_OFFSET2, "PPM.context.bNormalMode      : ",
			     pp->bNormalMode);
	    sp1273_wrSgShrt (RC1202_OFFSET2, "PPM.context.sSessProtVers    : ",
			     (signed short int FAR *)&(pp->sSessProtVers),
			     RC1206_DEC_NL);
	    sp1259_lstSgShrt (RC1202_OFFSET2, "PPM.context.vAbstrSynResult  :",
			      pp->vAbstrSynResult, RCro04_MaxPresCont + 1);
	    sp1259_lstSgShrt (RC1202_OFFSET2, "PPM.context.vAbstrSynId      :",
			      pp->vAbstrSynId, RCro04_MaxPresCont + 1);
	    for (i = 0; i < RCro01_MaxAbstrSyn; i++)
	       lStr[i] = (signed long int)pp->vTransSynId[i];
	    sp1260_lstSgLong (RC1202_OFFSET2, "PPM.context.vTransSynId      :",
			      lStr, RCro01_MaxAbstrSyn, TRUE);
	    sp1260_lstSgLong (RC1202_OFFSET2, "PPM.context.vPresContId      :",
			      pp->vPresContId, RCro01_MaxAbstrSyn, TRUE);
	    sp1273_wrSgShrt (RC1202_OFFSET2, "PPM.context.sDefContResult   : ",
			     (signed short int FAR *)&(pp->sDefContResult),
			     RC1206_DEC_NL);
	    sp1273_wrSgShrt (RC1202_OFFSET2, "PPM.context.sDefAbstrSynId   : ",
			     (signed short int FAR *)&(pp->sDefAbstrSynId),
			     RC1206_DEC_NL);
	    sp1273_wrSgShrt (RC1202_OFFSET2, "PPM.context.sDefTransSynId   : ",
			     (signed short int FAR *)&(pp->sDefTransSynId),
			     RC1206_DEC_NL);
	    sp1273_wrSgShrt (RC1202_OFFSET2, "PPM.context.sDcsNumb         : ",
			     (signed short int FAR *)&(pp->sDcsNumb), RC1206_DEC_NL);
	    sp1258_wrAddr (RC1202_OFFSET2, (char FAR *)"PPM.context.pDefinedBy       : ",
			   (char FAR *)pp->pDefinedBy);
	    sp1258_wrAddr (RC1202_OFFSET2, (char FAR *)"PPM.context.pTransSynName    : ",
			   (char FAR *)pp->pTransSynName);
	    sp1273_wrSgShrt (RC1202_OFFSET2, "PPM.context.sAbstrSynId      : ",
			     (signed short int FAR *)&(pp->sAbstrSynId),
			     RC1206_DEC_NL);
	    sp1258_wrAddr (RC1202_OFFSET2, "PPM.context.pEntContext      : ",
			   (char FAR *)pp->pEntContext);
	    sp1255_wrSgLong (RC1202_OFFSET2, "PPM.context.lProviderReason  : ",
	          (signed long int FAR *)&(pp->lProviderReason), RC1206_DEC_NL);
	    sp1255_wrSgLong (RC1202_OFFSET2, "PPM.context.lType            : ",
		            (signed long int FAR *)&(pp->lType), RC1206_DEC_NL);
	    sp1259_lstSgShrt (RC1202_OFFSET2,
			      "PPM.context.vTransSynNumbInReceivedCp:",
			      pp->vTransSynNumbInReceivedCp,
			      RCro01_MaxAbstrSyn);
	    sp1273_wrSgShrt (RC1202_OFFSET2, "PPM.context.sTransSynIdAbo   : ",
			     (signed short int FAR *)&(pp->sTransSynIdAbo),
			     RC1206_DEC_NL);
	    sp1271_wrLine();
	 }

      } else
	 retVal = ERROR;
      break;
    case RL0402_PduLog:
          if (argl[0] > 0){
              signed   short int rc   = OK;
              
              sp1255_wrSgLong(RC1202_OFFSET2,"Length of Pdu Logged: ",
                         (signed long int FAR *) &argl[0],RC1206_DEC_NL); 
              sp1263_hexDump(RC1202_OFFSET2,"PDU dump : ",
                             argv[0],argl[0]); 
              hStore = ro0060_newstore (    
                        ST_USEMAXBUF,    /* --> Max. buffer length.      */
                        ST_CLOSED,       /* --> Open mode: no file open. */
                        &rc);            /* <-- Error value.             */
              if (rc != OK) return (ERROR);
              rc= ro0068_stputm(hStore,(char FAR *)argv[0],(long)argl[0]);
              if ( (long)argl[0] < lLen ){
                  signed   long  int lLength=NULL;
                  signed   long  int lBufLen=NULL;
                                 char FAR * pDummy;

                  lLength = lLen - (long)argl[0];
                  lBufLen = ro0061_stbuflen(hStore);
                  if(lLength <= lBufLen)
                      pDummy = ro0069_stputs(hStore,lLength);  
                  else {   /* put data in packetes */
                     signed long int lToBeCopied=NULL;
         
                     lToBeCopied = lBufLen;
                     while (lToBeCopied){        
                         pDummy = ro0069_stputs(hStore,lToBeCopied);
                         if(pDummy){
                            lLength-=lToBeCopied;
                            lToBeCopied = (lLength <= lBufLen) ?
                                           lLength : lBufLen;
                         }else break;
                     }
                  }
                  ro0062_storelength (hStore); 
              }    
              if (rc != OK ) return (ERROR);
              sp0820_writeSpmPdu(hStore,&uInEvent,&ulOffset,TRUE);
              if ( (uInEvent ==  RC0801_SCONNECTreq) ||
                   (uInEvent == RC0803_SCONNECTrsp) ||
                   (uInEvent == RC0805_SRELEASEreq) ||
                   (uInEvent == RC0807_SRELEASErsp) ||
                   (uInEvent == RC0809_SUABORTreq) ||
                   (uInEvent == RC080c_SDATAreq) ){

              sp0403_getEvent(&uInEvent);
              rc = ro0431_decodPpm( uInEvent,
                                    (rT0430_stContext   NEAR *)&stCxt,
                                    (unsigned long  int FAR *)&hStore,
                                    (unsigned long  int FAR *)&hNode,
                                    ulOffset     );
              if (rc < 0 ) return(ERROR);              
              sp0450_writePdus(hNode,uInEvent);
              ro0246_clearPdvTree (&hNode); 
              }
          } 
       break;
    case RL0403_PduInfo:
         if (argn == 1) { 
              sp1255_wrSgLong(RC1202_OFFSET2,"Length of Pdu : ",
                         (signed long int FAR *) argv[0],RC1206_DEC_NL); 
              lLen = (signed long int ) * (signed long int FAR *) argv[0];
         }else retVal = ERROR; 
      break;
    case RL0404_PcxtInfo:
        if (argn == 3){
           if(argl[0] > 0 ){
                ro0041_memcpy((char FAR *) &stDefinedBy,
                              (char FAR *) argv[0],
                              (unsigned long int)sizeof(rTro76_stDefinedBy));
                stCxt.pDefinedBy = (rTro76_stDefinedBy FAR *)&stDefinedBy;
            }else               
                stCxt.pDefinedBy = NULL;
            if(argl[1] > 0){
                ro0041_memcpy((char FAR *) vTransSynName,
                              (char FAR *) argv[1],
                              (unsigned long int)argl[1]);
                stCxt.pTransSynName=(unsigned long  int FAR *)vTransSynName;
            }else 
                stCxt.pTransSynName=NULL;
            if(argl[2] > 0 ){
                ro0041_memcpy((char FAR *) &stEntContext,
                              (char FAR *) argv[2],
                              (unsigned long int)sizeof(rT0441_stEntContext));
                stCxt.pEntContext=(rT0441_stEntContext    *)&stEntContext;
            }else 
                stCxt.pEntContext=NULL;
              
        }else retVal=ERROR;
        break;
    case RL0405_EncodeInfo:
            
            if (argl[0] > 0 )
               uInEvent = *(unsigned int FAR *)argv[0];
            if (argl[1] > 0)
             ulOffset = (unsigned long int)*(unsigned long int FAR *)argv[1];
            if(argl[2] > 0)
              ro0041_memcpy (( char FAR *) &stCxt ,
                           ( char FAR *) argv[2],
                           (unsigned long int ) sizeof(rT0430_stContext));
            break;
    case RL0406_DecodeInfo:
             {
              signed   short int rc   = OK;
              sp0820_writeSpmPdu(hStore,&uInEvent,&ulOffset,FALSE);
              if ( (uInEvent ==  RC0802_SCONNECTind) ||
                   (uInEvent == RC0804_SCONNECTcnf) ||
                   (uInEvent == RC0806_SRELEASEind) ||
                   (uInEvent == RC0808_SRELEASEcnf) ||
                   (uInEvent == RC080a_SUABORTind) ||
                   (uInEvent == RC080d_SDATAind) ){
              
              sp0403_getEvent(&uInEvent);
              rc = ro0431_decodPpm( uInEvent,
                                    (rT0430_stContext    *)&stCxt,
                                    (unsigned long  int FAR *)&hStore,
                                    (unsigned long  int FAR *)&hNode,
                                    ulOffset     );
              if (rc < 0) return(ERROR);              
              sp0450_writePdus(hNode,uInEvent);
              ro0246_clearPdvTree (&hNode);
             }
           }
        break;
    case RL0407_OffsetInfo:
       ulOffset = *(unsigned long int FAR *)argv[0];
       break;
    default:
      retVal = ERROR;
      break;
  }

   if (retVal == ERROR)
      sp1264_write (RC1202_OFFSET2, "ERROR IN LOGGING FILE\n\n", NULL, NULL,
		    NULL, NULL);


   return (OK);
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0401_getPrpmEvent				      */
/*                                                                    */
/* DESCRIPTION	: Mapping from the event number to the string.	      */
/*                                                                    */
/*inoff ***************************************************************/

char FAR *sp0401_getPrpmEvent (
	unsigned int   iEvent)
{
   switch (iEvent) {

      /*-----------------*/
      /* RC0400_PpmEvent */
      /*-----------------*/
      case RC0401_PCONNECTreq	    : return ("RC0401_PCONNECTreq");
      case RC0402_PCONNECTind	    : return ("RC0402_PCONNECTind");
      case RC0403_PCONNECTrsp	    : return ("RC0403_PCONNECTrsp");
      case RC0404_PCONNECTcnf	    : return ("RC0404_PCONNECTcnf");
      case RC0405_PRELEASEreq	    : return ("RC0405_PRELEASEreq");
      case RC0406_PRELEASEind	    : return ("RC0406_PRELEASEind");
      case RC0407_PRELEASErsp	    : return ("RC0407_PRELEASErsp");
      case RC0408_PRELEASEcnf	    : return ("RC0408_PRELEASEcnf");
      case RC0409_PUABORTreq	    : return ("RC0409_PUABORTreq");
      case RC040a_PUABORTind	    : return ("RC040a_PUABORTind");
      case RC040b_PPABORTind	    : return ("RC040b_PPABORTind");
      case RC040c_PDATAreq	    : return ("RC040c_PDATAreq");
      case RC040d_PDATAind	    : return ("RC040d_PDATAind");
      case RC040e_PTOKEN_PLEASEreq  : return ("RC040e_PTOKEN_PLEASEreq");
      case RC040f_PTOKEN_PLEASEind  : return ("RC040f_PTOKEN_PLEASEind");
      case RC040g_PCONTROL_GIVEreq  : return ("RC040g_PCONTROL_GIVEreq");
      case RC040h_PCONTROL_GIVEind  : return ("RC040h_PCONTROL_GIVEind");
      case RC040i_PSYNC_MINORreq    : return ("RC040i_PSYNC_MINORreq");
      case RC040j_PSYNC_MINORind    : return ("RC040j_PSYNC_MINORind");
      case RC040k_PSYNC_MINORrsp    : return ("RC040k_PSYNC_MINORrsp");
      case RC040l_PSYNC_MINORcnf    : return ("RC040l_PSYNC_MINORcnf");
      case RC040m_PUEXCEPT_REPORTreq: return ("RC040m_PUEXCEPT_REPORTreq");
      case RC040n_PUEXCEPT_REPORTind: return ("RC040n_PUEXCEPT_REPORTind");
      case RC040o_PPEXCEPT_REPORTind: return ("RC040o_PPEXCEPT_REPORTind");
      case RC040p_PACT_STARTreq     : return ("RC040p_PACT_STARTreq");
      case RC040q_PACT_STARTind     : return ("RC040q_PACT_STARTind");
      case RC040r_PACT_RESUMEreq    : return ("RC040r_PACT_RESUMEreq");
      case RC040s_PACT_RESUMEind    : return ("RC040s_PACT_RESUMEind");
      case RC040t_PACT_ENDreq	    : return ("RC040t_PACT_ENDreq");
      case RC040u_PACT_ENDind	    : return ("RC040u_PACT_ENDind");
      case RC040v_PACT_ENDrsp	    : return ("RC040v_PACT_ENDrsp");
      case RC040w_PACT_ENDcnf	    : return ("RC040w_PACT_ENDcnf");
      case RC040x_PACT_INTERRUPTreq : return ("RC040x_PACT_INTERRUPTreq");
      case RC040y_PACT_INTERRUPTind : return ("RC040y_PACT_INTERRUPTind");
      case RC040z_PACT_INTERRUPTrsp : return ("RC040z_PACT_INTERRUPTrsp");
      case RC040A_PACT_INTERRUPTcnf : return ("RC040A_PACT_INTERRUPTcnf");
      case RC040B_PACT_DISCARDreq   : return ("RC040B_PACT_DISCARDreq");
      case RC040C_PACT_DISCARDind   : return ("RC040C_PACT_DISCARDind");
      case RC040D_PACT_DISCARDrsp   : return ("RC040D_PACT_DISCARDrsp");
      case RC040E_PACT_DISCARDcnf   : return ("RC040E_PACT_DISCARDcnf");
      case RC040F_PCONTINUE	    : return ("RC040F_PCONTINUE");
      case RC040G_PSYNCPOINT	    : return ("RC040G_PSYNCPOINT");

      /*-----------------*/
      /* RC0800_SpmEvent */
      /*-----------------*/
      case RC0801_SCONNECTreq	    : return ("RC0801_SCONNECTreq");
      case RC0802_SCONNECTind	    : return ("RC0802_SCONNECTind");
      case RC0803_SCONNECTrsp	    : return ("RC0803_SCONNECTrsp");
      case RC0804_SCONNECTcnf	    : return ("RC0804_SCONNECTcnf");
      case RC0805_SRELEASEreq	    : return ("RC0805_SRELEASEreq");
      case RC0806_SRELEASEind	    : return ("RC0806_SRELEASEind");
      case RC0807_SRELEASErsp	    : return ("RC0807_SRELEASErsp");
      case RC0808_SRELEASEcnf	    : return ("RC0808_SRELEASEcnf");
      case RC0809_SUABORTreq	    : return ("RC0809_SUABORTreq");
      case RC080a_SUABORTind	    : return ("RC080a_SUABORTind");
      case RC080b_SPABORTind	    : return ("RC080b_SPABORTind");
      case RC080c_SDATAreq	    : return ("RC080c_SDATAreq");
      case RC080d_SDATAind	    : return ("RC080d_SDATAind");
      case RC080e_STOKEN_PLEASEreq  : return ("RC080e_STOKEN_PLEASEreq");
      case RC080f_STOKEN_PLEASEind  : return ("RC080f_STOKEN_PLEASEind");
      case RC080g_SCONTROL_GIVEreq  : return ("RC080g_SCONTROL_GIVEreq");
      case RC080h_SCONTROL_GIVEind  : return ("RC080h_SCONTROL_GIVEind");
      case RC080i_SSYNC_MINORreq    : return ("RC080i_SSYNC_MINORreq");
      case RC080j_SSYNC_MINORind    : return ("RC080j_SSYNC_MINORind");
      case RC080k_SSYNC_MINORrsp    : return ("RC080k_SSYNC_MINORrsp");
      case RC080l_SSYNC_MINORcnf    : return ("RC080l_SSYNC_MINORcnf");
      case RC080m_SUEXCEPT_REPORTreq: return ("RC080m_SUEXCEPT_REPORTreq");
      case RC080n_SUEXCEPT_REPORTind: return ("RC080n_SUEXCEPT_REPORTind");
      case RC080o_SPEXCEPT_REPORTind: return ("RC080o_SPEXCEPT_REPORTind");
      case RC080p_SACT_STARTreq     : return ("RC080p_SACT_STARTreq");
      case RC080q_SACT_STARTind     : return ("RC080q_SACT_STARTind");
      case RC080r_SACT_RESUMEreq    : return ("RC080r_SACT_RESUMEreq");
      case RC080s_SACT_RESUMEind    : return ("RC080s_SACT_RESUMEind");
      case RC080t_SACT_ENDreq	    : return ("RC080t_SACT_ENDreq");
      case RC080u_SACT_ENDind	    : return ("RC080u_SACT_ENDind");
      case RC080v_SACT_ENDrsp	    : return ("RC080v_SACT_ENDrsp");
      case RC080w_SACT_ENDcnf	    : return ("RC080w_SACT_ENDcnf");
      case RC080x_SACT_INTERRUPTreq : return ("RC080x_SACT_INTERRUPTreq");
      case RC080y_SACT_INTERRUPTind : return ("RC080y_SACT_INTERRUPTind");
      case RC080z_SACT_INTERRUPTrsp : return ("RC080z_SACT_INTERRUPTrsp");
      case RC080A_SACT_INTERRUPTcnf : return ("RC080A_SACT_INTERRUPTcnf");
      case RC080B_SACT_DISCARDreq   : return ("RC080B_SACT_DISCARDreq");
      case RC080C_SACT_DISCARDind   : return ("RC080C_SACT_DISCARDind");
      case RC080D_SACT_DISCARDrsp   : return ("RC080D_SACT_DISCARDrsp");
      case RC080E_SACT_DISCARDcnf   : return ("RC080E_SACT_DISCARDcnf");
      case RC080F_SCONTINUE	    : return ("RC080F_SCONTINUE");
      case RC080G_SSYNCPOINT	    : return ("RC080G_SSYNCPOINT");
      case RC080H_STRIGGERABORT     : return ("RC080H_STRIGGERABORT");

      default			    : return ("UNKNOWN EVENT");
   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0402_writeState                                   */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0402_writeState(
	int          iOffset,
	char    FAR *pStr,
	int          iValue)
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
  switch(iValue){
    case RC0430_StaI0:
      sp1264_write(iOffset,"%s%s\n",pStr,"StaI0",NULL,NULL);
      break;
    case RC0431_StaI1:
      sp1264_write(iOffset,"%s%s\n",pStr,"StaI1",NULL,NULL);
      break;
    case RC0432_StaI2:
      sp1264_write(iOffset,"%s%s\n",pStr,"StaI2",NULL,NULL);
      break;
    case RC0433_Stat0:
      sp1264_write(iOffset,"%s%s\n",pStr,"Stat0",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,(signed int FAR *)&iValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0403_getEvent                                     */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-11-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0403_getEvent(
	unsigned int FAR * pInEvent)
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
unsigned int uInEvent;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  uInEvent = (unsigned int) *pInEvent;
  switch(uInEvent){
    case RC0801_SCONNECTreq:
      *pInEvent = RC0802_SCONNECTind;
      break;
    case RC0803_SCONNECTrsp:
      *pInEvent = RC0804_SCONNECTcnf;
      break;
    case RC0805_SRELEASEreq:
      *pInEvent = RC0806_SRELEASEind;
      break;
    case RC0807_SRELEASErsp:
      *pInEvent = RC0808_SRELEASEcnf;
      break;
    case RC0809_SUABORTreq:
      *pInEvent = RC080a_SUABORTind;
      break;
    case RC080c_SDATAreq:
      *pInEvent = RC080d_SDATAind;
      break;
  }
}/*end*/


/*********************	    END OF SPPRPM0.C	  *********************/
