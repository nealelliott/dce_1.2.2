/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spacpm0.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:35  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:47:30  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:48  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:43  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:45:11  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:31  keutel
 * 	creation
 * 	[1994/03/21  13:14:51  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spacpm0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:23 $" } ;
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
/* NAME 	: spacpm0.c					      */
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

static char *sccsid =    "@(#) spacpm0.c 1.2 93/12/20   RTS-V2.00";

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
#include <roppm00.h>
#include <roacpm0.h>
#include <roacpm3.h>
#include <routil0.h>

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spacpm0.h>	  /* Function prototyping		      */

/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/
char FAR *sp0601_getAcpmEvent PROTO ((signed long int));
void sp0602_writeState PROTO ((
int          iOffset,
char    FAR *pStr,
int          iValue));
void sp0603_writeAssocFlag PROTO ((
int          iOffset,
char    FAR *pStr,
int          iValue));



/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0600_traceAcpm()				      */
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

signed short int sp0600_traceAcpm (
long int	                 type,
long int	                 time,
unsigned short int               argn,
char               FAR * FAR *   argv,
unsigned long int        FAR *   argl,
unsigned short int               nerr)

{
   signed   short int	   retVal;
   signed long int    FAR *p;
   rT0650_acse        FAR *pp;
   signed int		   iAux;
   char 		   str[25];

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   retVal   = OK;

   if (type == RL0601_ActivityAcpm) {

      sp1251_recordHead (RC1200_OFFSET0, "R_ACTIVITY", "(AC-PM)",
			 type, time, nerr);
      if (argn == 8) {

	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &acb		       */
	 /*----------------------------------------------------*/

	 if (argl[3] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "ACPM.APPLID: ",
			    (unsigned long int FAR *)argv[3], RC1206_DEC_NL);
	 }
	 if (argl[0] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "ACPM.ASSOCID: ",
			    (unsigned long int FAR *)argv[0], RC1206_DEC_NL);
	 }

	 /*------------------------------------------*/
	 /* len: (signed short int) sizeof (*cxtPtr) */
	 /* val: (char FAR *)	    cxtPtr	     */
	 /*------------------------------------------*/
         
	 if (argl[4] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "ACPM.ParmP1: ",
			    (unsigned long int FAR *)argv[4], RC1206_DEC_NL);
	 }
	 if (argl[5] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "ACPM.ParamP2: ",
			    (unsigned long int FAR *)argv[5], RC1206_DEC_NL);
	 }
	 if (argl[6] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "ACPM.ParamP3: ",
			    (unsigned long int FAR *)argv[6], RC1206_DEC_NL);
	 }
	 if (argl[7] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "ACPM.ParamP4: ",
			    (unsigned long int FAR *)argv[7], RC1206_DEC_NL);
	 }

	 if (argl[1] > 0) {
	    pp = (rT0650_acse FAR *)argv[1];
	    sp0602_writeState (RC1202_OFFSET2, "ACPM.context.sState        : ",
			    pp->sState);
	    sp0603_writeAssocFlag(RC1202_OFFSET2, 
                            "ACPM.context.sAssocInitFlag: ",
			    pp->sAssocInitFlag);
	    iAux = (signed int)pp->sSpmProtoVers;
	    sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"ACPM.context.sSpmProtoVers : ",
			    (signed int FAR *)&iAux, RC1206_DEC_NL);
	    iAux = (signed int)pp->sAcseAbsId;
	    sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"ACPM.context.sAcseAbsId    : ",
			    (signed int FAR *)&iAux, RC1206_DEC_NL);
	    sp1255_wrSgLong (RC1202_OFFSET2, "ACPM.context.lMode         : ",
			     (signed long int FAR *)&(pp->lMode), RC1206_DEC_NL);
	    sp1271_wrLine();
	 }

	 /*--------------------------------------------------*/
	 /* len: (signed short int) sizeof (signed long int) */
	 /* val: (char FAR *)	    &pmevnt		     */
	 /*--------------------------------------------------*/

	 if (argl[2] > 0) {
	    p = (signed long int FAR *)argv[2];
	    ro0050_strcpy ((char FAR *)str,(char FAR *)sp0601_getAcpmEvent(*p));
	    sp1255_wrSgLong (RC1202_OFFSET2, "ACPM.event: ",  (signed long int FAR *)p, RC1207_HEX);
	    sp1264_write (RC1201_OFFSET1, "%s", str, NULL, NULL, NULL);
	    sp1271_wrLine();
            if ( (*p) == RC040b_PPABORTind ||
                 (*p) == RC040a_PUABORTind    )
                 if (argl[7] > 0) sp1290_writeAbortReason(
                     (unsigned long int ) *(unsigned long int FAR *)argv[7]);
	 }

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
/* NAME 	: sp0601_getAcpmEvent				      */
/*                                                                    */
/* DESCRIPTION	: Mapping from the event number to the string.	      */
/*                                                                    */
/*inoff ***************************************************************/

char FAR *sp0601_getAcpmEvent (
signed long int   lEvent)

{
   switch (lEvent) {
      case RC0601_AA_ASSOCIATEreq: return ("RC0601_AA_ASSOCIATEreq");
      case RC0603_AA_ASSOCIATErsp: return ("RC0603_AA_ASSOCIATErsp");
      case RC0607_AA_RELEASErsp  : return ("RC0607_AA_RELEASErsp");
      case RC0605_AA_RELEASEreq  : return ("RC0605_AA_RELEASEreq");
      case RC0609_AA_ABORTreq	 : return ("RC0609_AA_ABORTreq");
      case RC0402_PCONNECTind	 : return ("RC0402_PCONNECTind");
      case RC0404_PCONNECTcnf	 : return ("RC0404_PCONNECTcnf");
      case RC0408_PRELEASEcnf	 : return ("RC0408_PRELEASEcnf");
      case RC0406_PRELEASEind	 : return ("RC0406_PRELEASEind");
      case RC040a_PUABORTind	 : return ("RC040a_PUABORTind");
      case RC040b_PPABORTind	 : return ("RC040b_PPABORTind");
      case RC060c_AA_EXTERN_1	 : return ("RC060c_AA_EXTERN_1");
      case RC060d_AA_EXTERN_2	 : return ("RC060d_AA_EXTERN_2");
      default			 : return ("UNKNOWN EVENT");
   }
}/* end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0602_writeState                                   */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 02-11-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0602_writeState(
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
    case RC0651_STA0:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA0",NULL,NULL);
      break;
    case RC0652_STA1:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA1",NULL,NULL);
      break;
    case RC0653_STA2:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA2",NULL,NULL);
      break;
    case RC0654_STA3:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA3",NULL,NULL);
      break;
    case RC0655_STA4:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA4",NULL,NULL);
      break;
    case RC0656_STA5:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA5",NULL,NULL);
      break;
    case RC0657_STA6:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA6",NULL,NULL);
      break;
    case RC0658_STA7:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA7",NULL,NULL);
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
/* NAME 	: sp0603_writeAssocFlag                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0603_writeAssocFlag(
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
    case 0:
      sp1264_write(iOffset,"%s%s\n",pStr,"INITIATOR",NULL,NULL);
      break;
    case 1:
      sp1264_write(iOffset,"%s%s\n",pStr,"RESPONDER",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)NULL,(signed int FAR *)&iValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/


/*********************	    END OF SPACPM0.C	  *********************/
