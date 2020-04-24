/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spuctr2.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:41  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:13  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:19  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:39  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:45  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:51  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:09:39  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:32  keutel
 * 	creation
 * 	[1994/03/21  13:38:18  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spuctr2.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:41 $" } ;
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
/* NAME 	: spuctr2.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K                                      */
/* DATE 	: 30-11-93					      */
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

static char *sccsid =    "@(#) spuctr2.c 1.1 93/12/20   ";

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
#include <rospm00.h>
#include <routil0.h>
#include <rostor0.h>

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
void sp1530_writeUctrData PROTO (( 
unsigned long int hNode));

        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1530_writeUctrData                  	      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K				      */
/* DATE 	: 30-11-93					      */
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

void sp1530_writeUctrData ( 
unsigned long int hNode)
{/* entr */
  rTro04_stUserData  FAR * pNode; 
  signed short int rc;
  signed long int lStoreBufferLength,lBytesRemaining,lOutRequiredDataLength;
  char FAR * pOutBufferAddress;

  if (hNode) {
     pNode = (rTro04_stUserData FAR *) ro0021_globallock (hNode); 
     if (pNode){
       sp1281_writePduHdr("USER-DATA","UPPER-CONTROL");
       sp1273_wrSgShrt(RC1201_OFFSET1,"USER-DATA ABS ID :",
                       &(pNode->stPdvHeader.sAbstrSynId),
                       RC1206_DEC_NL);
       sp1264_write(RC1200_OFFSET0,"%s\n",
"------------------------USER DATA-------------------------------------------",
NULL,NULL,NULL);     
       rc = ro0066_storepos(pNode->stStore.hsStore,  
                     pNode->stStore.ulOffset);
       if (!rc ){
        lStoreBufferLength = ro0061_stbuflen (pNode->stStore.hsStore);   
        lBytesRemaining = lOutRequiredDataLength = pNode->stStore.ulNumb;
        sp1255_wrSgLong(RC1202_OFFSET2," User Data Length = ",&lBytesRemaining,
              RC1206_DEC_NL);
        if(lOutRequiredDataLength > 1024 )
           lBytesRemaining = lOutRequiredDataLength= 1024;  
                                                                      
        do {                                                                  
    /************ No. of bytes to be read from STORE should ********/ 
    /************ not exceed STORE buffer size. ********************/ 
            lOutRequiredDataLength = MIN(lBytesRemaining, lStoreBufferLength);
    /* ********** Get the read pointer in STORE buffer *********** */ 
    /* ********** where data to be read is present. ************** */ 
            pOutBufferAddress = ro0072_stgets (pNode->stStore.hsStore,   
                                       lOutRequiredDataLength);       
            sp1263_hexDump(RC1202_OFFSET2,"", 
               pOutBufferAddress,lOutRequiredDataLength);              
            lBytesRemaining -= lOutRequiredDataLength;
        } while (lBytesRemaining > 0);
     }
   
   ro0022_globalunlock (hNode);
   }
  }

 
}/* end */

