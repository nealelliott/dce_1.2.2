/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sptime0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:00  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:32:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:16  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:07:47  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:50  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:54:25  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:22  keutel
 * 	creation
 * 	[1994/03/21  13:24:08  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: sptime0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:00 $" } ;
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
/* NAME 	: sptime0.c					      */
/*                                                                    */
/* AUTHOR	: Natalia Serra 				      */
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

static char *sccsid =    "@(#) sptime0.c 1.2 93/12/20   RTS-V2.00";

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
#include <rotime0.h>

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <sptime0.h>	  /* Function prototyping		      */
#include <routil0.h>	  /* Function prototyping		      */

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
/* NAME 	: sp1100_traceTime()				      */
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

signed short int sp1100_traceTime (
	long int	                type,
	long int	                time,
	unsigned short int              argn,
	char                FAR * FAR * argv,
	unsigned long int         FAR * argl,
	unsigned short int              nerr)

{
   signed   short int	   retVal;
   unsigned int       FAR *p;
   char 		   str[35];

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   retVal   = OK;

   if (type == RL1101_ActivityTimer) {

      sp1251_recordHead (RC1200_OFFSET0, "R_ACTIVITY", "(TIMER)",
			 type, time, nerr);
      if (argn == 6) {

	 /*------------------------------------------*/
	 /* len: (signed short int) sizeof (hECBAux) */
	 /* val: (char FAR	 *) &hECBAux	     */
	 /*------------------------------------------*/

	 if (argl[0] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "TIMER.ecb  : ",
			    (unsigned long int FAR *)argv[0], RC1206_DEC_NL);
	 }

	 /*------------------------------------------*/
	 /* len: (signed short int) sizeof (hACBAux) */
	 /* val: (char FAR	 *) &hACBAux	     */
	 /*------------------------------------------*/

	 if (argl[1] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "TIMER.acb  : ",
			    (unsigned long int FAR *)argv[1], RC1206_DEC_NL);
	    sp1271_wrLine();
	 }

	 /*-------------------------------------------*/
	 /* len: (signed short int) sizeof (iInEvent) */
	 /* val: (char FAR	 *) &iInEvent	      */
	 /*-------------------------------------------*/

	 if (argl[2] > 0) {
	    p = (unsigned int FAR *)argv[2];
	    sp1256_wrUnInt (RC1202_OFFSET2, "TIMER.event: ", (unsigned int FAR *)p, RC1207_HEX);
	    ro0050_strcpy((char FAR *)str, (char FAR *)sp1101_getTimeEvent(*p));
	    sp1264_write (RC1201_OFFSET1, "%s", str, NULL, NULL, NULL);
	    sp1271_wrLine();
	 }

	 /*---------------------------------------*/
	 /* len: (signed short int) sizeof (ulP1) */
	 /* val: (char FAR	 *) &ulP1	  */
	 /*---------------------------------------*/

	 if (argl[3] > 0) {
	    switch (*p) {
	       case RC1101_SetTimerReq:
		  sp1257_wrUnLong (RC1202_OFFSET2, "TIMER.delay_of_time: ",
				   (unsigned long int FAR *)argv[3], RC1206_DEC_NL);
		  break;
	       case RC1102_ResetTimerReq:
		  sp1257_wrUnLong (RC1202_OFFSET2, "TIMER.reset_option : ",
				   (unsigned long int FAR *)argv[3], RC1205_DEC);
		  ro0050_strcpy((char FAR *)str, (char FAR *)sp1102_getResetOption(*argv[3]));
		  sp1264_write (RC1201_OFFSET1, "%s", str, NULL, NULL, NULL);
		  break;
	    }
	 }

	 /*---------------------------------------*/
	 /* len: (signed short int) sizeof (ulP3) */
	 /* val: (char FAR	 *) &ulP3	  */
	 /*---------------------------------------*/

	 if (argl[4] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "TIMER.pm_number    : ",
			     (unsigned long int FAR *)argv[4], RC1206_DEC_NL);
	 }

	 /*---------------------------------------*/
	 /* len: (signed short int) sizeof (ulP4) */
	 /* val: (char FAR	 *) &ulP4	  */
	 /*---------------------------------------*/

	 if (argl[5] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "TIMER.call_id      : ",
			     (unsigned long int FAR *)argv[5], RC1206_DEC_NL);
	 }
	 sp1271_wrLine();

      } else
	 retVal = ERROR;

   } else
      retVal = ERROR;

   if (retVal == ERROR)
      sp1264_write (RC1202_OFFSET2, "ERROR IN LOGGING FILE\n\n", NULL, NULL,
		    NULL, NULL);


   return (OK);
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1101_getTimeEvent() 			      */
/*		  sp1102_getResetOption()			      */
/*                                                                    */
/* DESCRIPTION	: Mapping from the parameter value to a string.       */
/*                                                                    */
/*inoff ***************************************************************/

char FAR *sp1101_getTimeEvent (
	signed int   iEvent)

{
   switch (iEvent) {
      case RC1101_SetTimerReq  : return ("RC1101_SetTimerReq");
      case RC1102_ResetTimerReq: return ("RC1102_ResetTimerReq");
      case RC1103_GetEventReq  : return ("RC1103_GetEventReq");
      default		       : return ("UNKNOWN EVENT");
   }
}


char FAR *sp1102_getResetOption (
	unsigned long int   ulVal)

{
   if (ulVal == 0) {
      return ("Reset only the first");
   } else {
      return ("Reset all for the specific PM");
   }
}

/*********************	    END OF SPTIME0.C	  *********************/
