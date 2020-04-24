/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spprpm4.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:48  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:08:57  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:31:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:17:56  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:52  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:35  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:29  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:53:32  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:33:58  keutel
 * 	creation
 * 	[1994/03/21  13:23:11  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spprpm4.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:48 $" } ;
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
/* NAME 	: spprpm4.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K                                      */
/* DATE 	: 25-11-93					      */
/*                                                                    */
/* COMPONENT    : RTROS Evaluation Program                            */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : In this module the the dispatcher config functions  */
/*                are copied                                          */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.00   K00| 25-11-93| original		       | kc |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spprpm4.c 1.1 93/12/20   RTS-V2.00";

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
#include <roerval.h>	  /* General RTS definitons		      */
#include <rouply0.h>
#include <rouply1.h>      /* PM service interface structures          */
#include <routil0.h>
#include <rostor0.h>      /* Store interface                          */ 
#include <rodpat0.h>      /* Dispatcher interface                     */
#include <rodpat1.h>      /* Dispatcher en/de-coding interface        */
#include <rodpat3.h>    /* Dispatcher config funs protos */ 

#include <roppm02.h>      /* PRPM function defs                       */
#include <roacpm2.h>      /* ACPM function defs                       */
#include <roropm2.h>      /* ROPM function defs                       */
#include <rouctr2.h>      /* Upper control function defs              */
#include <spstep0.h>	  /* RTS-STEP General definitions	      */

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
/* NAME 	: ro0294_cfPdvDeFct				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-11-10					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0294_cfPdvDeFct (
   signed   short int	     cnfNo,
   signed   short int	     pmNo,
   signed   short int	     absId,
   signed   long  int	     chTag,
   rTro76_stDefinedBy 	      FAR  * defBy,
   rTro01_pfvObjectId     tSyNa,
   unsigned long  int	     store,
   void               NEAR * pcxt,
   unsigned long  int FAR  * pdvN,
   unsigned long  int	     len,
   signed         int        iAnyNumb)


/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed	short int  rc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0294)

switch (cnfNo) {
   case RC0241_CfRoseOnly: goto lRosOnlyOsiStack;
   default:		 RM0003_RETURN (rs0294, signed short int, -R_NPSTCF)
}

/* ****************** RosOnlyOsiStack: PPM, ACPM, ROPM  ************* */
/* ******************       				************* */

lRosOnlyOsiStack:

switch (pmNo) {
 case RC0216_Acse    :
      rc = ro0603_decodAcpm      (absId,chTag,defBy,tSyNa,store,pcxt,pdvN,len, 
                                   iAnyNumb );
      break;

 case RC0219_Rose    :
      rc = ro0703_decodRopm      (absId,chTag,defBy,tSyNa,store,pcxt,pdvN,len,
                                   iAnyNumb);
      break;

 case RC0221_RoseUser:
      rc = ro1503_decodUpperCtrl (absId,chTag,defBy,tSyNa,store,pcxt,pdvN,len,
                                   iAnyNumb);
      break;

 default	     :
      rc = -R_NPDVDE;
      break;
}

RM0003_RETURN (rs0294, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0297_cfX410DeFct				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-02-24					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0297_cfX410DeFct (
   signed   short int	     cnfNo,
   signed   short int        pmNo,
   unsigned       int        uInEvent,
   signed   long  int        ltype,
   unsigned long  int	     pStore,
   void               NEAR * pContext,
   unsigned long  int FAR  * phNode)


/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0297)

switch (cnfNo) {
   case RC0241_CfRoseOnly: goto lRosOnlyOsiStack;
   default:		   RM0003_RETURN (rs0297, signed short int, -R_NPSTCF)
}

/* ****************** RosOnlyOsiStack: PPM, ACPM, ROPM  ************* */
/* ******************       				************* */

lRosOnlyOsiStack:

RM0003_RETURN (rs0297, signed short int, -R_D410DE)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0299_cfSniPDeFct				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-30					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0299_cfSniPDeFct (
   signed   short int	    cnfNo,
   signed   short int	    pmNo,
   unsigned long  int	    store,
   void               NEAR * pcxt,
   unsigned long  int FAR  * pdvN,
   signed   short int FAR  * psAbsId)


/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed	short int  rc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0299)

switch (cnfNo) {
   case RC0241_CfRoseOnly: goto lRosOnlyOsiStack;
   default:		 RM0003_RETURN (rs0299, signed short int, -R_NPSTCF)
}

/* ****************** RosOnlyOsiStack: PPM, ACPM, ROPM  ************* */
/* ******************       				************* */

lRosOnlyOsiStack:

switch (pmNo) {
 case RC0216_Acse    :
      rc = ro060c_decodSniPAcpm      (psAbsId,store, pcxt, pdvN);
      break;

 case RC0219_Rose    :
      rc = ro070b_decodSniPRopm      (psAbsId,store, pcxt, pdvN);
     break;

 case RC0221_RoseUser:
      rc = ro1507_decodSniPUpperCtrl (psAbsId, store, pcxt, pdvN);
      break;

 default	     :
      rc = -R_NPDVDE;
      break;
}

RM0003_RETURN (rs0299, signed short int, rc)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0295_cfPdvClFct				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-11-10					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0295_cfPdvClFct (
   signed   short int	      cnfNo,
   unsigned long  int  FAR *pdvNode)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed	short int    rc;
signed	short int  pmNo;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0295)

pmNo = ro0405_getPmNumb (*pdvNode);
if (pmNo < 0) {
    RM0003_RETURN (rs0295, signed short int, pmNo)
}

switch (cnfNo) {
   case RC0241_CfRoseOnly: goto lRosOnlyOsiStack;
   default:		 RM0003_RETURN (rs0295, signed short int, -R_NPSTCF)
}

/* ****************** RosOnltOsiStack:  PPM, ACPM, ROPM ************* */
/* ******************       				************* */

lRosOnlyOsiStack:

switch (pmNo) {
   case RC0215_Presentation : rc = ro0404_clearNodePpm	     (pdvNode); break;
   case RC0216_Acse	    : rc = ro0604_clearNodeAcpm      (pdvNode); break;
   case RC0219_Rose	    : rc = ro0704_clearNodeRopm      (pdvNode); break;
   case RC0221_RoseUser     : rc = ro1504_clearNodeUpperCtrl (pdvNode); break;
   default		    : rc = -R_NPDVCF;			        break;
}

RM0003_RETURN (rs0295, signed short int, rc)
}/*end*/

