/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spspm2.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:30  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:05  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:02  root]
 * 
 * Revision 1.1.2.5  1994/08/24  08:35:34  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:35:47  marrek]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:22  keutel
 * 	OT11620
 * 	[1994/08/19  13:57:58  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:23  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:35  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:41  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:08:58  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:16  keutel
 * 	creation
 * 	[1994/03/21  13:37:39  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spspm2.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:30 $" } ;
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
/* NAME 	: spspm2.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K                                      */
/* DATE 	: 02-12-93					      */
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

static char *sccsid =    "@(#) spspm2.c 1.1 93/12/20   ";

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
#include <rospm00.h>
#include <rospm01.h>
#include <spspm0.h>

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spprpm0.h>	  /* Function prototyping		      */

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
/* NAME 	: sp0820_writeSpmPdu                      	      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 02-12-93					      */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0820_writeSpmPdu ( 
unsigned long  int       hStore,
unsigned       int FAR * puInEvent,
unsigned long  int FAR * pulOffset,
signed   short int       bPduDump)
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
{/* entr */
/* ********************* LOCAL VARIABLES **************************** */
signed   short int         sData=NULL;
signed   short int         bOutEvent=FALSE;
signed   long  int         lOutEvent;
signed   long  int         lIniPos=NULL;
signed   long  int         lSize=NULL;
               char FAR *  pBuf=NULL;
signed   short int         bWrite=FALSE;

  if (hStore){
    if(ro0074_storeactive(hStore , 0L) == OK ){
      if ((sData = ro0070_stgetc (hStore) ) == 0){
        if (bPduDump)bWrite=TRUE;
        bOutEvent=TRUE;
        while(!(sData=ro0070_stgetc(hStore)));
        if(sData != RC0083_EOF)
          lIniPos = (ro0076_stwhere(hStore)-1);
      }else if(!bPduDump)bWrite=TRUE;
      if(lIniPos >= 0)
        ro0066_storepos(hStore,lIniPos);
      if(bWrite) { 
       switch(sData){
         case 1:
             sp1281_writePduHdr("DATA TRANSFER(DT)-SPDU","SESSION");
             lOutEvent=RC080c_SDATAreq;
             break;
         case 9:
             sp1281_writePduHdr("FINISH(FN)-SPDU","SESSION");
             lOutEvent=RC0805_SRELEASEreq;
             break;
         case 10:
             sp1281_writePduHdr("DISCONNECT(DN)-SPDU","SESSION");
             lOutEvent=RC0807_SRELEASErsp;
             break;
         case 12:
             sp1281_writePduHdr("REFUSE(RF)-SPDU","SESSION");
             lOutEvent=RC0803_SCONNECTrsp;
             break;
         case 13:
             sp1281_writePduHdr("CONNECT(CN)-SPDU","SESSION");
             lOutEvent=RC0801_SCONNECTreq;
             break;
         case 14:
             sp1281_writePduHdr("ACCEPT(AC)-SSPDU","SESSION");
             lOutEvent=RC0803_SCONNECTrsp;
             break;
         case 25:
             sp1281_writePduHdr("ABORT(AB)-SPDU","SESSION");
             lOutEvent=RC0809_SUABORTreq;
             break;
         case 26:
             sp1281_writePduHdr("ABORT ACCEPT(AA)-SPDU","SESSION");
             break;
         default:
             sp1281_writePduHdr("UNKNOWN-SPDU","SESSION");
             break;
       }
       if(bOutEvent ) {
           lSize = RC0831_MaxParamLen-lIniPos;
          *pulOffset = RC0831_MaxParamLen;
          *puInEvent=(unsigned int)lOutEvent;
       } else lSize = *pulOffset;
       if(lSize > 0) pBuf=ro0072_stgets(hStore,lSize);
       if(pBuf)sp1263_hexDump(RC1201_OFFSET1,"SESSION DATA :",pBuf,lSize);
      }
      ro0073_storeidle(hStore);                            
  }
 }
}/* end */

