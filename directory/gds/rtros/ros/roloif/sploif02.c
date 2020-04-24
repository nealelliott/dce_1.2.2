/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sploif02.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:54  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:35  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:03  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:25  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:57  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:12  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:05:26  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:43  keutel
 * 	creation
 * 	[1994/03/21  13:34:21  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: sploif02.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:54 $" } ;
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
/* NAME 	: sploif0.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K.			              */
/* DATE 	: 19-06-93					      */
/*                                                                    */
/* COMPONENT    : RTS Evaluation Program                              */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : In this module the trace record types are evaluated */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.00      | 19-06-93| original		       | kc |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) sploif02.c 1.2 93/12/20   RTS-V2.00";

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

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <sploif02.h>	  /* Function prototyping		      */
#include <rotpm00.h>      /* TPM function definitions                 */
#include <roloif32.h>     /* LOIF context block definition            */


/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/
char FAR * sp0301_getLoifEvent PROTO (( signed long int   lEvent));


/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0300_traceLoif()				      */
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

signed short int sp0300_traceLoif (

long int	     type,
long int	     time,
unsigned short int   argn,
char FAR * FAR *     argv,
unsigned long int   *argl,
unsigned short int   nerr)

{
    signed short int rc = OK;
    char             str[25];
   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   if (type == RL0301_ActivityLoif) {

      sp1251_recordHead (RC1200_OFFSET0, "R_ACTIVITY", "(LOIF)",
			 type, time, nerr);
      if (argn == 8) {
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &hEcb		       */
	 /*----------------------------------------------------*/

	 if (argl[0] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "LOIF.APPLID: ",
			    (unsigned long int *)argv[0], RC1206_DEC_NL);
          }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &hAcb		       */
	 /*----------------------------------------------------*/

	 if (argl[1] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "LOIF.ASSOCID: ",
			    (unsigned long int *)argv[1], RC1206_DEC_NL);
	 }

	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP1		       */
	 /*----------------------------------------------------*/

	 if (argl[2] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P1: ",
			    (unsigned long int *)argv[2], RC1206_DEC_NL);
          }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP2		       */
	 /*----------------------------------------------------*/

	 if (argl[3] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P2: ",
			    (unsigned long int *)argv[3], RC1206_DEC_NL);
          }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP3		       */
	 /*----------------------------------------------------*/

	 if (argl[4] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P3: ",
			    (unsigned long int *)argv[4], RC1206_DEC_NL);
          }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP4		       */
	 /*----------------------------------------------------*/

	 if (argl[5] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P4: ",
			    (unsigned long int *)argv[5], RC1206_DEC_NL);
          }

	 /*--------------------------------------------------*/
	 /* len: (signed short int) sizeof (signed long int) */
	 /* val: (char FAR *)	    &lInEvent		     */
	 /*--------------------------------------------------*/

	 if (argl[6] > 0) {
	    signed long int * p = (signed long int *)argv[6];
	    ro0050_strcpy(str, sp0301_getLoifEvent(*p));
	    sp1255_wrSgLong (RC1202_OFFSET2, "LOIF.event: ",  p, RC1207_HEX);
	    sp1264_write (RC1201_OFFSET1, "%s", str, NULL, NULL, NULL);
	    sp1271_wrLine();
	 }

	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (*rT0730_stContext) */
	 /* val: (char FAR *)	    Context	               */
	 /*----------------------------------------------------*/

	 if (argl[7] > 0) {
	    rT0300_roloifContext NEAR * pp = 
                (rT0300_roloifContext NEAR *)argv[7];
           
            sp1254_wrSgInt(RC1202_OFFSET2,"LOIF.context.TransConnId :",
                           &pp->iTransConId,RC1206_DEC_NL);
	    sp1257_wrUnLong (RC1202_OFFSET2, "LOIF.context.TiduLength :",
			    &pp->ulTiduLength,RC1206_DEC_NL);
	    sp1257_wrUnLong (RC1202_OFFSET2, "LOIF.context.hRecStore :",
			    &pp->hRecStore,RC1206_DEC_NL);
	    sp1257_wrUnLong (RC1202_OFFSET2, "LOIF.context.hFirstSendQu :",
			    &pp->hFirstSendQu,RC1206_DEC_NL);
	    sp1257_wrUnLong (RC1202_OFFSET2, "LOIF.context.hLastSendQu :",
			    &pp->hLastSendQu,RC1206_DEC_NL);
            sp1254_wrSgInt(RC1202_OFFSET2,"LOIF.context.iStopState :",
                           &pp->iStopState,RC1206_DEC_NL);
            sp1280_writeBool(RC1202_OFFSET2, "LOIF.context.bNetBIOS   : ",
                 pp->bNetBIOS);                      
	    sp1257_wrUnLong (RC1202_OFFSET2, "LOIF.context.hSendData :",
			    &pp->hSendData,RC1206_DEC_NL);
            sp1258_wrAddr (RC1202_OFFSET2, "LOIF.context.pSendData  : ",  
                (char  FAR *)(pp->pSendData) );       
	    sp1257_wrUnLong (RC1202_OFFSET2, "LOIF.context.ulSendLen :",
			    &pp->ulSendLen,RC1206_DEC_NL);
            sp1262_asciiOrHexDump(RC1202_OFFSET2,"LOIF.context.cHeadLen :",
                           (char FAR *)&pp->cHeadLen,sizeof(unsigned char),
                           FALSE);
         }
    }else  rc = ERROR;
  }else rc = ERROR;
  return(rc);  
}


 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0301_getLoifEvent	                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-06-20                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


char FAR * sp0301_getLoifEvent (

signed long int   lEvent)


/*                                                                    */
/* DESCRIPTION	: Mapping from the event number to the string.	      */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   switch (lEvent) {
      case 0:return("RECEIVE");
      case RC0901_TCONNECTreq         : return ("RC0901_TCONNECTreq");
      case RC0903_TCONNECTrsp         : return ("RC0903_TCONNECTrsp");
      case RC0905_TDISCONNECTreq       : return ("RC0905_TDISCONNECTreq");
      case RC0907_TDATAreq       : return ("RC0907_TDATAreq");
      case RC0909_TCONTINUE	     : return ("RC0909_TCONTINUE");
      case RC090a_TREDIRECTreq	     : return ("RC090a_TREDIRECTreq");

      default			 : return ("UNKNOWN EVENT");
   }
}/*end*/
/*********************	    END OF SPLOIF0.C	  *********************/
