/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: splctr0.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:27  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:13:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:35  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:46  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:56  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:58  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:50:30  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:23  keutel
 * 	creation
 * 	[1994/03/21  13:19:56  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: splctr0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:33 $" } ;
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
/* NAME 	: splctr0.c					      */
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

static char *sccsid =    "@(#) splctr0.c 1.2 93/12/20   RTS-V2.00";

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
#include <routil0.h>	  /* General RTS definitons		      */


#include <roropm0.h>	  /* Events of ROPM definitios                */
#include <rortpm0.h>	  /* Events of RTPM definitions               */
#include <roppm00.h>	  /* Events of PRPM definitions               */
#include <roacpm0.h>	  /* Events of ACPM definitions               */

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <splctr0.h>	  /* Function prototyping		      */
#include <rolctr3.h>	  /* Context block proto typing               */

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

char FAR * sp0501_getLctrEvent PROTO ((
       signed long int   ));
void       sp0502_writeState PROTO ((
       int          iOffset,
       char    FAR *pStr,
       int          iValue));
        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0500_traceLctr				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-28                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int sp0500_traceLctr (
	long int	                type,
	long int	                time,
	unsigned short int              argn,
	char                FAR * FAR * argv,
	unsigned long int         FAR * argl,
	unsigned short int              nerr)

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
signed   short int      rc     = OK;
signed long int  FAR  * p;
rT0530_stCxt     FAR  * pp;
char 		        str[25];

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */


   if (type == RL0501_ActivityLctr) {

      sp1251_recordHead (RC1200_OFFSET0, "R_ACTIVITY", "(L-CTR)",
			 type, time, nerr);
      if (argn == 8) {

	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &hEcb		       */
	 /*----------------------------------------------------*/

	 if (argl[0] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "LCTR.APPLID: ",
			    (unsigned long int FAR *)argv[0], RC1206_DEC_NL);

	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &hAcb		       */
	 /*----------------------------------------------------*/

	 if (argl[1] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "LCTR.ASSOCID: ",
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
	    ro0050_strcpy((char FAR *)str, (char FAR *)sp0501_getLctrEvent(*p));
	    sp1255_wrSgLong (RC1202_OFFSET2, "LCTR.event: ",  (signed long int FAR *)p, RC1207_HEX);
	    sp1264_write (RC1201_OFFSET1, "%s\n", str, NULL, NULL, NULL);
	 }




	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (*rT0530_stCxt) */
	 /* val: (char FAR *)	    Context	               */
	 /*----------------------------------------------------*/

	 if (argl[7] > 0) {
	    pp = (rT0530_stCxt FAR  *)argv[1];
            sp0502_writeState ( RC1202_OFFSET2, "LCTR.Context.sState:",
                            pp->sState);
            sp1273_wrSgShrt ( RC1202_OFFSET2, "LCTR.Context.sApCxtId:",
                            (signed short int FAR *) &(pp->sApCxtId),
                            RC1206_DEC_NL);
            sp1274_wrUnShrt ( RC1202_OFFSET2, "LCTR.Context.usAutNo:",
                            (unsigned short int FAR *) &(pp->usAutNo),
                            RC1206_DEC_NL);

	 }


      } else
	 rc = ERROR;

   } else
      rc = ERROR;

   if (rc == ERROR)
      sp1264_write (RC1202_OFFSET2, "ERROR IN LOGGING FILE\n\n", NULL, NULL,
		    NULL, NULL);


   return (OK);
 }/*end of if for type */
}/*end*/


 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0501_getLctrEvent	                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-28                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


char FAR * sp0501_getLctrEvent (
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


      case RC0602_AA_ASSOCIATEind: return ("RC0602_AA_ASSOCIATEind");           
      case RC0604_AA_ASSOCIATEcnf: return ("RC0604_AA_ASSOCIATEcnf");           
      case RC0606_AA_RELEASEind: return ("RC0606_AA_RELEASEind");           
      case RC0608_AA_RELEASEcnf: return ("RC0608_AA_RELEASEcnf");           
      case RC060a_AA_ABORTind: return ("RC060a_AA_ABORTind");           
      case RC060b_AA_PABORTind: return ("RC060b_AA_PABORTind");           



      case RC040d_PDATAind	 : return ("RC040d_PDATAind");

      case RC040f_PTOKEN_PLEASEind	 : return ("RC040f_PTOKEN_PLEASEind");
      case RC040h_PCONTROL_GIVEind	 : return ("RC040h_PCONTROL_GIVEind");
      case RC040j_PSYNC_MINORind	 : return ("RC040j_PSYNC_MINORind");
      case RC040l_PSYNC_MINORcnf	 : return ("RC040l_PSYNC_MINORcnf");
      case RC040q_PACT_STARTind	 : return ("RC040q_PACT_STARTind");
      case RC040s_PACT_RESUMEind	 : return ("RC040s_PACT_RESUMEind");
      case RC040u_PACT_ENDind	 : return ("RC040u_PACT_ENDind");
      case RC040w_PACT_ENDcnf	 : return ("RC040w_PACT_ENDcnf");
      case RC040y_PACT_INTERRUPTind	 : return ("RC040y_PACT_INTERRUPTind");
      case RC040A_PACT_INTERRUPTcnf	 : return ("RC040A_PACT_INTERRUPTcnf");
      case RC040C_PACT_DISCARDind	 : return ("RC040C_PACT_DISCARDind");
      case RC040E_PACT_DISCARDcnf	 : return ("RC040E_PACT_DISCARDcnf");
      case RC040F_PCONTINUE	 : return ("RC040F_PCONTINUE");
      case RC040G_PSYNCPOINT	 : return ("RC040G_PSYNCPOINT");

      case RC040n_PUEXCEPT_REPORTind	 : return ("RC040n_PUEXCEPT_REPORTind");

      case RC0601_AA_ASSOCIATEreq: return ("RC0601_AA_ASSOCIATEreq");           
      case RC0603_AA_ASSOCIATErsp: return ("RC0603_AA_ASSOCIATErsp");           
      case RC0607_AA_RELEASErsp  : return ("RC0607_AA_RELEASErsp");             
      case RC0605_AA_RELEASEreq  : return ("RC0605_AA_RELEASEreq");             
      case RC0609_AA_ABORTreq    : return ("RC0609_AA_ABORTreq");               


      case RC040c_PDATAreq	 : return ("RC040c_PDATAreq");


      case RC040e_PTOKEN_PLEASEreq	 : return ("RC040e_PTOKEN_PLEASEreq");
      case RC040g_PCONTROL_GIVEreq	 : return ("RC040g_PCONTROL_GIVEreq");
      case RC040i_PSYNC_MINORreq	 : return ("RC040i_PSYNC_MINORreq");
      case RC040k_PSYNC_MINORrsp	 : return ("RC040k_PSYNC_MINORrsp");
      case RC040p_PACT_STARTreq	 : return ("RC040p_PACT_STARTreq");
      case RC040r_PACT_RESUMEreq	 : return ("RC040r_PACT_RESUMEreq");
      case RC040t_PACT_ENDreq	 : return ("RC040t_PACT_ENDreq");
      case RC040v_PACT_ENDrsp	 : return ("RC040v_PACT_ENDrsp");
      case RC040x_PACT_INTERRUPTreq	 : return ("RC040x_PACT_INTERRUPTreq");
      case RC040z_PACT_INTERRUPTrsp	 : return ("RC040z_PACT_INTERRUPTrsp");
      case RC040B_PACT_DISCARDreq	 : return ("RC040B_PACT_DISCARDreq");
      case RC040D_PACT_DISCARDrsp	 : return ("RC040D_PACT_DISCARDrsp");

      case RC040m_PUEXCEPT_REPORTreq	 : return ("RC040m_PUEXCEPT_REPORTreq");
      default			 : return ("UNKNOWN EVENT");
   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0502_writeState                                   */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0502_writeState(
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
    case RC0530_Idle:
      sp1264_write(iOffset,"%s%s\n",pStr,"IDLE",NULL,NULL);
      break;
    case RC0531_Active:
      sp1264_write(iOffset,"%s%s\n",pStr,"ACTIVE",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,(signed int FAR *)&iValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/

/*********************	    END OF SPLCTR0.C	  *********************/
