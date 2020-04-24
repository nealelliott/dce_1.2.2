/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spropm0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:07  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:45  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:24  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:44  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:11  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:23  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:06:53  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:40:05  keutel
 * 	creation
 * 	[1994/03/21  13:35:38  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spropm0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:07 $" } ;
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
/* NAME 	: spropm0.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K.		           	      */
/* DATE 	: 20-04-93					      */
/*                                                                    */
/* COMPONENT    : ROS Evaluation Program                              */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : In this module the trace record types are evaluated */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.0A00 K00| 20-04-93| original		       | kc |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spropm0.c 1.2 93/12/20   RTS-V2.00";

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

#include <roppm00.h>
#include <roropm0.h>
#include <roropm3.h>
#include <roacpm0.h>
#include <rortpm0.h>

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spropm0.h>	  /* Function prototyping		      */

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

char FAR * sp0701_getRopmEvent PROTO((
      signed long int   lEvent ));
void       sp0702_writeStateRopm PROTO ((
      int          iOffset,
      char    FAR *pStr,
      int          iValue));
void       sp0703_writeStateTrPm PROTO ((
      int          iOffset,
      char    FAR *pStr,
      int          iValue));
void       sp0704_writeTrRtRopm PROTO ((
      int          iOffset,
      char    FAR *pStr,
      int          iValue));
        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0700_traceRopm				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-20                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int sp0700_traceRopm (

long int	               type,
long int	               time,
unsigned short int             argn,
char               FAR * FAR * argv,
unsigned long  int       FAR * argl,
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
signed   short int      rc     = OK;
signed long int  FAR  * p;
rT0730_stContext FAR  * pp;
signed short int        sAux;
char 		        str[25];

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */


   if (type == RL0701_ActivityRopm) {

      sp1251_recordHead (RC1200_OFFSET0, "R_ACTIVITY", "(RO-PM)",
			 type, time, nerr);
      if (argn == 8) {

	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &hEcb		       */
	 /*----------------------------------------------------*/

	 if (argl[0] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "ROPM.APPLID: ",
			    (unsigned long int FAR *)argv[0], RC1206_DEC_NL);
          }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &hAcb		       */
	 /*----------------------------------------------------*/

	 if (argl[1] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "ROPM.ASSOCID: ",
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
	    ro0050_strcpy((char FAR *)str, (char FAR *)sp0701_getRopmEvent(*p));
	    sp1255_wrSgLong (RC1202_OFFSET2, "ROPM.event: ",  (signed long int FAR *)p, RC1207_HEX);
	    sp1264_write (RC1201_OFFSET1, "%s\n", str, NULL, NULL, NULL);
            if (*p == RC060b_AA_PABORTind )
              if (argl[5] > 0 )
                   sp1290_writeAbortReason((unsigned long int)
                        *(unsigned long int FAR *)argv[5]);
	 }




	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (*rT0730_stContext) */
	 /* val: (char FAR *)	    Context	               */
	 /*----------------------------------------------------*/

	 if (argl[7] > 0) {
	    pp = (rT0730_stContext FAR  *)argv[7];

	    sp0702_writeStateRopm(RC1202_OFFSET2, 
                      "ROPM.context.sStateRopm   : ",
			    pp->sStateRopm);
            if(pp->bmappingInfo)
	       sp0704_writeTrRtRopm (RC1202_OFFSET2, 
                      "ROPM.context.sStateTrPm   : ",
                      pp->sStateTrPm);
              
	    else sp0703_writeStateTrPm (RC1202_OFFSET2,
                          "ROPM.context.sStateTrPm   : ",
			    pp->sStateTrPm);
	    sp1280_writeBool (RC1202_OFFSET2, "ROPM.context.bReleaseFg   : ",
			    pp->sReleaseFg);
	    sAux = (signed short int)pp->sSubAutoNum;
	    sp1273_wrSgShrt (RC1202_OFFSET2, "ROPM.context.sSubAutoNum  : ",
			    (signed short int FAR *)&sAux, RC1206_DEC_NL);
	    sp1280_writeBool (RC1202_OFFSET2, "ROPM.context.bAssocInit   : ",
			    pp->bAssocInit);
	    sp1257_wrUnLong (RC1202_OFFSET2, "ROPM.context.ulAssocClas   : ",
			    (unsigned long int FAR *)&(pp->ulAssocClas), 
                            RC1206_DEC_NL);
	    sp1280_writeBool (RC1202_OFFSET2, "ROPM.context.bToken        : ",
			    pp->bToken);
	    sAux = (signed short int)pp->sAbsIdOp;
	    sp1273_wrSgShrt (RC1202_OFFSET2, "ROPM.context.sAbsIdOp      : ",
			    (signed short int FAR *)&sAux, RC1206_DEC_NL);
	    sp1280_writeBool (RC1202_OFFSET2, "ROPM.context.bSyncFlag     : ",
			    pp->bSyncFlag);
	    sp1255_wrSgLong (RC1202_OFFSET2, "ROPM.context.lInvokeId     : ",
			    (signed long int FAR *)&(pp->lInvokeId), 
                            RC1206_DEC_NL);
         
	    sAux = (signed short int)pp->sRejCount;
	    sp1273_wrSgShrt (RC1202_OFFSET2, "ROPM.context.sRejCount     : ",
			    (signed short int FAR *)&sAux, RC1206_DEC_NL);
	    sp1280_writeBool (RC1202_OFFSET2, "ROPM.context.bmappingInfo   : ",
			    pp->bmappingInfo);

            sp1258_wrAddr(RC1202_OFFSET2,
                            (char FAR *)"ROPM.context.hFirst          : ",
                            (char FAR *)&(pp->hFirst)                );
 
            sp1258_wrAddr(RC1202_OFFSET2,
                            (char FAR *)"ROPM.context.hLast           : ",
                            (char FAR *)&(pp->hLast)                );
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
}/*end*/


 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0701_getRopmEvent	                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-20                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


char FAR * sp0701_getRopmEvent (

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

      case RC0602_AA_ASSOCIATEind	 : return ("RC0602_AA_ASSOCIATEind");
      case RC0604_AA_ASSOCIATEcnf	 : return ("RC0604_AA_ASSOCIATEcnf");
      case RC0606_AA_RELEASEind	         : return ("RC0606_AA_RELEASEind");
      case RC0608_AA_RELEASEcnf	         : return ("RC0608_AA_RELEASEcnf");
      case RC060b_AA_PABORTind	         : return ("RC060b_AA_PABORTind");
      case RC073y_ABORTind	         : return ("RC073y_ABORTind");

      case RC040d_PDATAind	 : return ("RC040d_PDATAind");

      case RC1002_RT_OPENind	 : return ("RC1002_RT_OPENind");
      case RC1004_RT_OPENcnf	 : return ("RC1004_RT_OPENcnf");
      case RC1006_RT_CLOSEind	 : return ("RC1006_RT_CLOSEind");
      case RC1008_RT_CLOSEcnf	 : return ("RC1008_RT_CLOSEcnf");
      case RC100g_RT_P_ABORTind	 : return ("RC100g_RT_P_ABORTind");
      case RC100i_RT_U_ABORTind	 : return ("RC100i_RT_U_ABORTind");

      case RC100a_RT_TRANSFERind	 : return ("RC100a_RT_TRANSFERind");
      case RC100b_RT_TRANSFERcnf	 : return ("RC100b_RT_TRANSFERcnf");
      case RC100d_RT_TURN_PLEASEind	 : return ("RC100d_RT_TURN_PLEASEind");
      case RC100f_RT_TURN_GIVEind	 : return ("RC100f_RT_TURN_GIVEind");
      case RC040F_PCONTINUE	 : return ("RC040F_PCONTINUE");

      default			 : return ("UNKNOWN EVENT");
   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0702_writeStateRopm                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 02-11-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0702_writeStateRopm(

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
    case RC0731_STAnoPm:
      sp1264_write(iOffset,"%s%s\n",pStr,"STAnoPm",NULL,NULL);
      break;
    case RC0732_STA01:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA01",NULL,NULL);
      break;
    case RC0733_STA02:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA02",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,NULL,(signed int FAR *)&iValue,RC1206_DEC_NL);
      break; 
  }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0703_writeStateTrPm                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 02-11-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0703_writeStateTrPm(

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
    case RC0735_STAnoTr1:
      sp1264_write(iOffset,"%s%s\n",pStr,"STAnoTr1",NULL,NULL);
      break;
    case RC0736_STA100:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA100",NULL,NULL);
      break;
    case RC0737_STA200:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA200",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,NULL,(signed int FAR *)&iValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0704_writeTrRtRopm                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0704_writeTrRtRopm(

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
    case RC0739_STAnoTr2:
      sp1264_write(iOffset,"%s%s\n",pStr,"RC0739_STAnoTr2",NULL,NULL);
      break;
    case RC073a_STA10:
      sp1264_write(iOffset,"%s%s\n",pStr,"RC073a_STA10",NULL,NULL);
      break;
    case RC073b_STA20:
      sp1264_write(iOffset,"%s%s\n",pStr,"RC073b_STA20",NULL,NULL);
      break;
    case RC073c_STA21:
      sp1264_write(iOffset,"%s%s\n",pStr,"RC073c_STA21",NULL,NULL);
      break;
    case RC073d_STA22:
      sp1264_write(iOffset,"%s%s\n",pStr,"RC073d_STA22",NULL,NULL);
      break;
    case RC073e_STA23:
      sp1264_write(iOffset,"%s%s\n",pStr,"RC073e_STA23",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,NULL,(signed int FAR *)&iValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/
/*EOF*/
