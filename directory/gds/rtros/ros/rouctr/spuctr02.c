/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spuctr02.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:37  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:11  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:14  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:34  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:42  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:48  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:09:30  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:28  keutel
 * 	creation
 * 	[1994/03/21  13:38:10  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spuctr02.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:37 $" } ;
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
/* NAME 	: spuctr0.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K.		           	      */
/* DATE 	: 28-04-93					      */
/*                                                                    */
/* COMPONENT    : ROS Evaluation Program                              */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : In this module the trace record types are evaluated */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.0A00 K00| 28-04-93| original		       | kc |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spuctr02.c 1.2 93/12/20   RTS-V2.00";

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

#include <rouply0.h>	  /* General RTS definitons		      */
#include <rouply1.h>	  /* General RTS definitons		      */
#include <routil0.h>	  /* General utility prototyping              */

#include <roropm0.h>	  /* ROPM event definitions                   */
#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spuctr02.h>	  /* Function prototyping		      */
#include <rouctr3.h>	  /* Context block definitios                 */

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

char FAR * sp1501_getUctrEvent PROTO ((
          signed long int   lEvent ));
        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1500_traceUctr				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-28                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int sp1500_traceUctr (

long int	               type,
long int	               time,
unsigned short int             argn,
char               FAR * FAR * argv,
unsigned long int        FAR * argl,
unsigned short int             nerr)

/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS: type, time, argn, argl, argv			      */
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
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
signed   short int         rc     = OK;
signed long int     FAR  * p;
rT1530_stAsoContext FAR  * pp;
char 		           str[25];

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */


   if (type == RL1501_ActivityUctr) {

      sp1251_recordHead (RC1200_OFFSET0, "R_ACTIVITY", "(U-CTR)",
			 type, time, nerr);
      if (argn == 8) {

	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &hEcb		       */
	 /*----------------------------------------------------*/

	 if (argl[0] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "UCTR.APPLID: ",
			    (unsigned long int FAR *)argv[0], RC1206_DEC_NL);

	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &hAcb		       */
	 /*----------------------------------------------------*/

	 if (argl[1] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "UCTR.ASSOCID: ",
			    (unsigned long int FAR *)argv[1], RC1206_DEC_NL);
	 }

	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP1		       */
	 /*----------------------------------------------------*/

	 if (argl[2] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P1: ",
			    (unsigned long int FAR *)argv[2], RC1206_DEC_NL);
          }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP2		       */
	 /*----------------------------------------------------*/

	 if (argl[3] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P2: ",
			    (unsigned long int FAR *)argv[3], RC1206_DEC_NL);
          }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP3		       */
	 /*----------------------------------------------------*/

	 if (argl[4] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P3: ",
			    (unsigned long int FAR *)argv[4], RC1206_DEC_NL);
          }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP4		       */
	 /*----------------------------------------------------*/

	 if (argl[5] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P4: ",
			    (unsigned long int FAR *)argv[5], RC1206_DEC_NL);
          }

	 /*--------------------------------------------------*/
	 /* len: (signed short int) sizeof (signed long int) */
	 /* val: (char FAR *)	    &lInEvent		     */
	 /*--------------------------------------------------*/

	 if (argl[6] > 0) {
	    p = (signed long int FAR *)argv[6];
	    ro0050_strcpy((char FAR *)str, (char FAR *)sp1501_getUctrEvent(*p));
	    sp1255_wrSgLong (RC1202_OFFSET2, "UCTR.event: ",  (signed long int FAR *)p, RC1207_HEX);
	    sp1264_write (RC1201_OFFSET1, "%s\n", str, NULL, NULL, NULL);
	 }




	 /*-------------------------------------------------------*/
	 /* len: (signed short int) sizeof (*rT1530_stAsoContext) */
	 /* val: (char FAR *)	    Context	                  */
	 /*-------------------------------------------------------*/

	 if (argl[7] > 0) {
	    pp = (rT1530_stAsoContext FAR *)argv[1];
            sp1258_wrAddr ( RC1202_OFFSET2, "UCTR.Context.hAsscHandle:",
                            (char FAR *) &(pp->hAsscHandle));
            sp1258_wrAddr ( RC1202_OFFSET2, "UCTR.Context.hOperHandle:",
                            (char FAR *) &(pp->hOperHandle));

	    sp1271_wrLine();
	 }


      } else
	 rc = ERROR;

   } else
      rc = ERROR;

   if (rc == ERROR)
      sp1264_write (RC1202_OFFSET2, "ERROR IN LOGGING FILE\n\n", NULL, NULL,
		    NULL, NULL);


   return (OK);
 }/* end of if */
}/*end*/


 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1501_getUctrEvent	                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-28                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


char FAR * sp1501_getUctrEvent (

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
      case RC0701_RO_BINDreq         : return ("RC0701_RO_BINDreq");
      case RC0703_RO_BINDrsp         : return ("RC0703_RO_BINDrsp");
      case RC0705_RO_UNBINDreq       : return ("RC0705_RO_UNBINDreq");
      case RC0707_RO_UNBINDrsp       : return ("RC0707_RO_UNBINDrsp");
      case RC0709_RO_UABORTreq	     : return ("RC0709_RO_UABORTreq");

      case RC070c_RO_INVreq	 : return ("RC070c_RO_INVreq");
      case RC070e_RO_RESreq	 : return ("RC070e_RO_RESreq");
      case RC070g_RO_ERRreq	 : return ("RC070g_RO_ERRreq");
      case RC070i_RO_RJUreq	 : return ("RC070i_RO_RJUreq");


      case RC0702_RO_BINDind	 : return ("RC0702_RO_BINDind");
      case RC0704_RO_BINDcnf	 : return ("RC0704_RO_BINDcnf");
      case RC0706_RO_UNBINDind	 : return ("RC0706_RO_UNBINDind");
      case RC0708_RO_UNBINDcnf	 : return ("RC0708_RO_UNBINDcnf");
      case RC070a_RO_UABORTind	 : return ("RC070a_RO_UABORTind");
      case RC070b_RO_PABORTind	 : return ("RC070b_RO_PABORTind");


      case RC070d_RO_INVind	 : return ("RC070d_RO_INVind");
      case RC070f_RO_RESind	 : return ("RC070f_RO_RESind");
      case RC070h_RO_ERRind	 : return ("RC070h_RO_ERRind");
      case RC070j_RO_RJUind	 : return ("RC070j_RO_RJUind");
      case RC070k_RO_RJPind	 : return ("RC070k_RO_RJPind");

      default			 : return ("UNKNOWN EVENT");
   }
}/*end*/


/*********************	    END OF SPUCTR0.C	  *********************/
