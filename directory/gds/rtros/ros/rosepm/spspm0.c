/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spspm0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:29  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:04  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:58  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:19  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:34  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:08:53  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:14  keutel
 * 	creation
 * 	[1994/03/21  13:37:37  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spspm0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:29 $" } ;
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
/* NAME 	: spspm00.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar K.		           	      */
/* DATE 	: 15-06-93					      */
/*                                                                    */
/* COMPONENT    : ROS Evaluation Program                              */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : In this module the trace record types are evaluated */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.0A00 K00| 15-06-93| original		       | skh|	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spspm0.c 1.2 93/12/20   RTS-V2.00";

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

#include <rospm00.h>      /* SPM events                               */
#include <rospm01.h>      /* SPM events                               */
#include <rospm03.h>      /* SPM events                               */
#include <rospm07.h>
#include <rotpm00.h>      /*TPM  events                               */
#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spspm0.h>	  /* SPM Step Prototype definitions.    */

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

char FAR * sp0801_getSpmEvent PROTO((
      signed long int   lEvent ));
char FAR * sp0802_getSpmInEvent PROTO((
      signed long int   lEvent ));
void       sp0803_writeState PROTO ((
      int          iOffset,
      char    FAR *pStr,
      int          iValue));
        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0800_traceSpm				      */
/*                                                                    */
/* AUTHOR       : Shivkumar                                           */
/* DATE         : 93-04-20                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int sp0800_traceSpm  (

long int	                type,
long int	                time,
unsigned short int              argn,
char                FAR * FAR * argv,
unsigned long  int        FAR * argl,
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
signed   short int         rc     = OK;
signed long int     FAR  * p;
rT0870_stSpmContext FAR  * pp;
char 		           str[25];

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */


   if (type == RL0801_ActivitySpm) {

      sp1251_recordHead (RC1200_OFFSET0, "R_ACTIVITY", "(SE-PM)",
			 type, time, nerr);
      if (argn == 10) {

	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &hEcb		       */
	 /*----------------------------------------------------*/

	 if (argl[0] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "SPM.APPLID: ",
			    (unsigned long int FAR *)argv[0], RC1206_DEC_NL);
         }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &hAcb		       */
	 /*----------------------------------------------------*/

	 if (argl[1] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "SPM.ASSOCID: ",
			    (unsigned long int FAR *)argv[1], RC1206_DEC_NL);
	 }

         /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (signed long int)   */
	 /* val: (char FAR *)	    &event       	       */
	 /*----------------------------------------------------*/
         if  (argl[2] > 0 ) {
	    p = (signed long int FAR *)argv[2];
	    ro0050_strcpy((char FAR *)str, (char FAR *)sp0801_getSpmEvent(*p));
	    sp1255_wrSgLong (RC1202_OFFSET2, "SPM .event: ",  (signed long int FAR *)p, RC1207_HEX);
	    sp1264_write (RC1201_OFFSET1, "%s\n", str, NULL, NULL, NULL);
            if ( (*p) == RC0906_TDISCONNECTind ||
                 (*p) == RC0809_SUABORTreq      )
              if (argl[5] > 0 ) sp1290_writeAbortReason( 
                        *(unsigned long int FAR *)argv[5]);

         } 
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP1		       */
	 /*----------------------------------------------------*/

	 if (argl[3] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P1: ",
			    (unsigned long int FAR *)argv[3], RC1206_DEC_NL);
          }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP2		       */
	 /*----------------------------------------------------*/

	 if (argl[4] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P2: ",
			    (unsigned long int FAR *)argv[4], RC1206_DEC_NL);
          }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP3		       */
	 /*----------------------------------------------------*/

	 if (argl[5] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P3: ",
			    (unsigned long int FAR *)argv[5], RC1206_DEC_NL);
          }
	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (unsigned long int) */
	 /* val: (char FAR *)	    &ulP4		       */
	 /*----------------------------------------------------*/

	 if (argl[6] > 0) {
	    sp1257_wrUnLong (RC1202_OFFSET2, "Param P4: ",
			    (unsigned long int FAR *)argv[6], RC1206_DEC_NL);
          }
	 /*--------------------------------------------------*/
	 /* len: (signed short int) sizeof (signed long int) */
	 /* val: (char FAR *)	    &lInEvent		     */
	 /*--------------------------------------------------*/

	 if (argl[7] > 0) {
	    p = (signed long int FAR *)argv[7];
	    ro0050_strcpy((char FAR *)str, (char FAR *)sp0802_getSpmInEvent(*p));
	    sp1255_wrSgLong (RC1202_OFFSET2, "SPM.internal event: ",  (signed long int FAR *)p, RC1207_HEX);
	    sp1264_write (RC1201_OFFSET1, "%s\n", str, NULL, NULL, NULL);
	 }




	 /*----------------------------------------------------*/
	 /* len: (signed short int) sizeof (*rT0730_stContext) */
	 /* val: (char FAR *)	    Context	               */
	 /*----------------------------------------------------*/

	 if (argl[8] > 0) {
	    pp = (rT0870_stSpmContext FAR  *)argv[8];

	    sp0803_writeState (RC1202_OFFSET2, "SPM.context.usState        : ",
			    pp->usState);
            sp1257_wrUnLong(RC1202_OFFSET2,  "SPM.context.ulConnectResult: ",
                            (unsigned long int FAR *)&pp->ulConectresult, RC1206_DEC_NL);
            sp1257_wrUnLong(RC1202_OFFSET2,  "SPM.context.ulTidusize     : ",
                            (unsigned long int FAR *)&pp->ulTidusize, RC1206_DEC_NL);
	    sp1273_wrSgShrt (RC1202_OFFSET2, "SPM.context.sCalledAddrsLen: ",
		   (signed short int FAR *)&pp->sCalledAddrsLen, RC1206_DEC_NL);
            sp1262_asciiOrHexDump(RC1202_OFFSET2, (char FAR *)"SPM.context.CalledAadd: ",
                                  (char FAR *)pp->vCalledAddrs , (unsigned long int) pp->sCalledAddrsLen,
                                  TRUE );
            sp1258_wrAddr   (RC1202_OFFSET2,
                             (char FAR *)"SPM.context.hAddress       : ",
                             (char FAR *)&pp->hAddrs );
            sp1258_wrAddr   (RC1202_OFFSET2,
                             (char FAR *)"SPM.context.hCalledAddress : ",
                             (char FAR *)&pp->hCalledAddress );
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.FunctionalUnits: ",
               (unsigned short int FAR *)&pp->usFunctionalUnits,RC1206_DEC_NL);
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.Protocal Options: ",
               (unsigned short int FAR *)&pp->usPopt,RC1206_DEC_NL);
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.Protocal Version: ",
               (unsigned short int FAR *)&pp->usProtvers,RC1206_DEC_NL);
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.usUabort        : ",
               (unsigned short int FAR *)&pp->usUabort,RC1206_DEC_NL);
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.usAreuse        : ",
               (unsigned short int FAR *)&pp->usAreuse,RC1206_DEC_NL);
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.usVtca          : ",
               (unsigned short int FAR *)&pp->usVtca,RC1206_DEC_NL);
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.usVcoll         : ",
               (unsigned short int FAR *)&pp->usVcoll,RC1206_DEC_NL);
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.usVact          : ",
               (unsigned short int FAR *)&pp->usVact,RC1206_DEC_NL);
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.usVtrr          : ",
               (unsigned short int FAR *)&pp->usVtrr,RC1206_DEC_NL);
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.usTexp: ",
               (unsigned short int FAR *)&pp->usTexp,RC1206_DEC_NL);
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.usVdnr: ",
               (unsigned short int FAR *)&pp->usVdnr,RC1206_DEC_NL);
            sp1258_wrAddr   (RC1202_OFFSET2,
                             (char FAR *)"SPM.context.hUserData       : ",
                             (char FAR *)&pp->hUserData );
	    sp1257_wrUnLong (RC1202_OFFSET2, "SPM.context.ulOffset       : ",
			    (unsigned long int FAR *)&pp->ulOffset, 
                            RC1206_DEC_NL);
            sp1274_wrUnShrt(RC1202_OFFSET2, "SPM.context.usUserConnExists: ",
               (unsigned short int FAR *)&pp->usUserConnExists,RC1206_DEC_NL);
	    sp1280_writeBool (RC1202_OFFSET2, "SPM.context.bAbortPend      : ",
			      pp->bAbortPend);
	    sp1271_wrLine();
	 }
	 if (argl[9] > 0) {
            
            sp1263_hexDump(RC1202_OFFSET2, "SPM.context.CalledAadd: ",
                                  argv[9], (int) argl[9]);
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
/* NAME 	: sp0801_getSpmEvent	                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-06-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


char FAR * sp0801_getSpmEvent (

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
      case RC0801_SCONNECTreq         : return ("RC0801_SCONNECTreq");
      case RC0803_SCONNECTrsp         : return ("RC0803_SCONNECTrsp");
      case RC0805_SRELEASEreq         : return ("RC0805_SRELEASEreq");
      case RC0807_SRELEASErsp         : return ("RC0807_SRELEASErsp");
      case RC0809_SUABORTreq	      : return ("RC0809_SUABORTreq");

      case RC080c_SDATAreq	 : return ("RC080c_SDATAreq");

      case RC080e_STOKEN_PLEASEreq	 : return ("RC080e_STOKEN_PLEASEreq");
      case RC080g_SCONTROL_GIVEreq	 : return ("RC080g_SCONTROL_GIVEreq");

      case RC080i_SSYNC_MINORreq	 : return ("RC080i_SSYNC_MINORreq");
      case RC080k_SSYNC_MINORrsp	 : return ("RC080k_SSYNC_MINORrsp");

      case RC080m_SUEXCEPT_REPORTreq	 : return ("RC080m_SUEXCEPT_REPORTreq");

      case RC080p_SACT_STARTreq	         : return ("RC080p_SACT_STARTreq");
      case RC080r_SACT_RESUMEreq	 : return ("RC080r_SACT_RESUMEreq");
      case RC080t_SACT_ENDreq	         : return ("RC080t_SACT_ENDreq");
      case RC080v_SACT_ENDrsp	         : return ("RC080v_SACT_ENDrsp");

      case RC080x_SACT_INTERRUPTreq	 : return ("RC080x_SACT_INTERRUPTreq");
      case RC080z_SACT_INTERRUPTrsp	 : return ("RC080z_SACT_INTERRUPTrsp");
      case RC080B_SACT_DISCARDreq	 : return ("RC080B_SACT_DISCARDreq");
      case RC080D_SACT_DISCARDrsp	 : return ("RC080D_SACT_DISCARDrsp");

      case RC080F_SCONTINUE	 : return ("RC080F_SCONTINUE");
      case RC080G_SSYNCPOINT	 : return ("RC080G_SSYNCPOINT");
      case RC080H_STRIGGERABORT	 : return ("RC080H_STRIGGERABORT");


/* Tpm events    */

      case RC0902_TCONNECTind	 : return ("RC0902_TCONNECTind");
      case RC0904_TCONNECTcnf	 : return ("RC0904_TCONNECTcnf");
      case RC0906_TDISCONNECTind : return ("RC0906_TDISCONNECTind");

      case RC0908_TDATAind	         : return ("RC0908_TDATAind");

      case RC0909_TCONTINUE	         : return ("RC0909_TCONTINUE");
      case RC090b_TREDIRECTind	         : return ("RC090b_TREDIRECTind");


      default			 : return ("UNKNOWN EVENT");
   }
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0802_getSpmInEvent	                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-06-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


char FAR * sp0802_getSpmInEvent (

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

   switch (lEvent & 0x0000ffff) {
      case RC08e0_TCNind         : return ("RC08e0_TCNind");
      case RC08e1_TCNcnf         : return ("RC08e1_TCNcnf");
      case RC08e2_TDNind         : return ("RC08e2_TDNind");
      case RC08e3_TCONT         : return ("RC08e3_TCONT");
      case RC08e4_SCNreq	      : return ("RC08e4_SCNreq");
      case RC08e5_SCRpos	 : return ("RC08e5_SCRpos");
      case RC08e6_SCRneg	 : return ("RC08e6_SCRneg");
      case RC08e7_ACN	 : return ("RC08e7_ACN");

      case RC08e8_ACDO	 : return ("RC08e8_ACDO");
      case RC08e9_AOA	 : return ("RC08e9_AOA");

      case RC08ea_AAC	 : return ("RC08ea_AAC");

      case RC08eb_ARF	         : return ("RC08eb_ARF");
      case RC08ec_SRLreq	 : return ("RC08ec_SRLreq");
      case RC08ed_SRRpos	         : return ("RC08ed_SRRpos");
      case RC08ee_SRRneg	         : return ("RC08ee_SRRneg");

      case  RC08ef_AFN	 : return ("RC08ef_AFN");
      case RC08eg_AAA	 : return ("RC08eg_AAA");
      case RC08eh_ANF	 : return ("RC08eh_ANF");
      case RC08ei_AAB	 : return ("RC08ei_AAB");

      case RC08ej_ADN	 : return ("RC08ej_ADN");
      case RC08ek_SUAreq	 : return ("RC08ek_SUAreq");
      case RC08el_SDTreq	 : return ("RC08el_SDTreq");



      case RC08em_ADT	 : return ("RC08em_ADT");
      case RC08en_NoEvent	 : return ("RC08en_NoEvent");
      case RC08eo_BadEvent : return ("RC08eo_BadEvent");



      default			 : return ("UNKNOWN EVENT");
   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0803_writeState                                   */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 02-11-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0803_writeState(

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
    case RC08d0_STA01:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA01",NULL,NULL);
      break;
    case RC08d1_STA01A:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA01A",NULL,NULL);
      break;
    case RC08d2_STA01B:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA01B",NULL,NULL);
      break;
    case RC08d3_STA01C:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA01C",NULL,NULL);
      break;
    case RC08d4_STA01D:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA01D",NULL,NULL);
      break;
    case RC08d5_STA01E:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA01E",NULL,NULL);
      break;
    case RC08d6_STA02A:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA02A",NULL,NULL);
      break;
    case RC08d7_STA02B:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA02B",NULL,NULL);
      break;
    case RC08d8_STA03:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA03",NULL,NULL);
      break;
    case RC08d9_STA08:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA08",NULL,NULL);
      break;
    case RC08da_STA09:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA09",NULL,NULL);
      break;
    case RC08db_STA16:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA16",NULL,NULL);
      break;
    case RC08dc_STA713:
      sp1264_write(iOffset,"%s%s\n",pStr,"STA713",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,NULL,(signed int FAR *)&iValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/
/*********************	    END OF SPSPM00.C	  *********************/
