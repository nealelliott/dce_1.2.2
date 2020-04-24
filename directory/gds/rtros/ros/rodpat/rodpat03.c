/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rodpat03.c,v $
 * Revision 1.1.4.2  1996/03/11  13:21:45  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:38:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:08  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:39  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:51:41  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:02  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:00:10  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:28  keutel
 * 	creation
 * 	[1994/03/21  13:29:19  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rodpat03.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:45 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1991 BY SIEMENS NIXDORF INFORMATIONS AG	      */
/*  * All rights reserved                                             */
/*  *                                                                 */
/*  * This software is furnished under licence and may be used only   */
/*  * in accordance with the terms of that licence and with the       */
/*  * inclusion of the above copyright notice.                        */
/*  * This software may not be provided or otherwise made available   */
/*  * to, or used by, any other person. No title to or ownership of   */
/*  * the software is hereby  transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME 	: rodpat02.c              			      */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Vinita Paunikar                     */
/* DATE 	: 93-03-29					      */
/*                                                                    */
/* COMPONENT	: ROS dispatcher				      */
/*                                                                    */
/* PRD#/VERS.	: RTROS-V03.00			      		      */
/*                                                                    */
/* DESCRIPTION	: ROS dispatcher configuration module     	      */
/*                for stack : SPM, PPM, ACPM, ROPM       	      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/* V03.00 K0| 93-03-29|  original		       |jm  |	      */
/* V03.00 K0| 93-04-09|  Separate ROS specific fnclity |vp  |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "@(#)rodpat03.c  1.2  93/12/20";

#ifdef RC0043_LuRosSpm

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>	 /* RTROS general definitions		     */
#include <roerval.h>	 /* RTROS error values			     */
#include <routil0.h>	 /* RTROS utilities interface		     */

#include <rouply0.h>	 /* RTROS common upper layer definitons	     */

#include <rodpat0.h>	 /* RTROS dispatcher PM interface 	     */
#include <rodpat3.h>	 /* RTROS dispatcher config. interface	     */

#include <rotime2.h>	 /* RTROS timer calling interface 	     */
#include <roloif2.h>	 /* RTROS loif  calling interface 	     */
#include <rospm02.h>	 /* RTROS ppm   calling interface 	     */
#include <roppm02.h>	 /* RTROS ppm   calling interface 	     */
#include <roacpm2.h>	 /* RTROS acpm  calling interface 	     */
#include <rolctr2.h>	 /* RTROS act1  calling interface 	     */
#include <roropm2.h>	 /* RTROS rtpm  calling interface 	     */
#include <rouctr2.h>	 /* RTROS act1  calling interface 	     */

/*********************************************************************/
/*                                                                   */
/*              L O C A L  V A R I A B L E S                         */
/*                                                                   */
/*********************************************************************/

/* ****************** Logging function name strings ***************** */
/* ******************                               ***************** */

RM0001_STRG (rs0290, "ro0290_cfPmFct")
RM0001_STRG (rs0291, "ro0291_cfClFct")
RM0001_STRG (rs0292, "ro0292_cfIfFct")
RM0001_STRG (rs0293, "ro0293_cfPdvEn")
RM0001_STRG (rs0294, "ro0294_cfPdvDe")
RM0001_STRG (rs0295, "ro0295_cfPdvCl")
RM0001_STRG (rs0296, "ro0296_cfX410E")
RM0001_STRG (rs0297, "ro0297_cfX410D")
RM0001_STRG (rs0298, "ro0298_cfSniPE")
RM0001_STRG (rs0299, "ro0299_cfSniPD")



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0290_cfPmFct				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-11-10					      */
/*                                                                    */
/* SYNTAX       :                                                     */

   signed   short int PASCAL ro0290_cfPmFct (

   signed   short int cnfNo,
   signed   short int pmNo,
   unsigned long  int pmcxt)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:	      : 				      */
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

signed short int rc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0290)

switch (cnfNo) {
   case RC0241_CfRoseOnly: goto lRosOnlyOsiStack;
   default:		   RM0003_RETURN (rs0290, signed short int, -R_NPSTCF);
}

/* ****************** RosOnlyOsiStack: SPM, PPM, ACPM,  ************* */
/* ******************       	       ROPM.		************* */

lRosOnlyOsiStack:

switch (pmNo) {
   case RC0201_Timer	    : rc = ro1100_timer       (pmcxt); break;
   case RC0202_Lower	    : rc = ro0310_SendReceive (pmcxt); break;
   case RC0213_Transport    : rc = ro0310_SendReceive (pmcxt); break;
   case RC0214_Session	    : rc = ro0800_spm         (pmcxt); break;
   case RC0215_Presentation : rc = ro0400_ppm	      (pmcxt); break;
   case RC0216_Acse	    : rc = ro0600_acpm	      (pmcxt); break;
   case RC0217_LcrtFct	    : rc = ro0500_lowerCtrl   (pmcxt); break;
   case RC0219_Rose	    : rc = ro0700_ropm	      (pmcxt); break;
   case RC0221_RoseUser     : rc = ro1500_upperCtrl   (pmcxt); break;
   default		    : rc = -R_NPMFCT;		       break;
}

RM0003_RETURN (rs0290, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0291_cfClFct				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-11-10					      */
/*                                                                    */
/* SYNTAX       :                                                     */

   signed   short int PASCAL ro0291_cfClFct (

   signed   short int	cnfNo,
   signed   short int	 pmNo,
   signed   long  int	event,
   unsigned long  int p1Pmcxt,
   unsigned long  int	   p2,
   unsigned long  int	   p3,
   unsigned long  int	   p4)

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

RM0002_ENTRY (rs0291)

switch (cnfNo) {
   case  RC0241_CfRoseOnly: goto lRosOnlyOsiStack;
   default:		 RM0003_RETURN (rs0291, signed short int, -R_NPSTCF)
}

/* ****************** RosOnlyOsiStack: SPM, PPM, ACPM,  ************* */
/* ******************       	       ROPM.		************* */

lRosOnlyOsiStack:

switch (pmNo) {
   case RC0201_Timer	    : rc = ro1101_cleanUp (event, p1Pmcxt, p2, p3, p4);
			      break;
   case RC0202_Lower	    : rc = ro0399_cleanUp (event, p1Pmcxt, p2, p3, p4);
			      break;
   case RC0213_Transport    : rc = ro0399_cleanUp (event, p1Pmcxt, p2, p3, p4);
			      break;
   case RC0214_Session	    : rc = ro0801_cleanUp (event, p1Pmcxt, p2, p3, p4);
			      break;
   case RC0215_Presentation : rc = ro0401_cleanUp (event, p1Pmcxt, p2, p3, p4);
			      break;
   case RC0216_Acse	    : rc = ro0601_cleanUp (event, p1Pmcxt, p2, p3, p4);
			      break;
   case RC0217_LcrtFct	    : rc = ro0501_cleanUp (event, p1Pmcxt, p2, p3, p4);
			      break;
   case RC0219_Rose	    : rc = ro0701_cleanUp (event, p1Pmcxt, p2, p3, p4);
			      break;
   case RC0221_RoseUser     : rc = ro1501_cleanUp (event, p1Pmcxt, p2, p3, p4);
			      break;
   default		    : rc = -R_NCLFCT;
			      break;
}

RM0003_RETURN (rs0291, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0292_cfIfFct				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-11-10					      */
/*                                                                    */
/* SYNTAX       :                                                     */

   signed   short int PASCAL ro0292_cfIfFct (

   signed   short int cnfNo,
   signed   short int  pmNo,
   unsigned long  int pmCxt,
   signed   short int  inNo,
   void     FAR       *info)

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

RM0002_ENTRY (rs0292)

switch (cnfNo) {
   case RC0241_CfRoseOnly: goto lRosOnlyOsiStack;
   default:		 RM0003_RETURN (rs0292, signed short int, -R_NPSTCF)
}

/* ****************** RosOnlyOsiStack: SPM, PPM, ACPM,  ************* */
/* ******************       	       ROPM.		************* */

lRosOnlyOsiStack:

switch (pmNo) {
   case RC0201_Timer	    : rc = -R_NIFFCT;				 break;
   case RC0202_Lower	    : rc = -R_NIFFCT;                            break;
   case RC0213_Transport    : rc = -R_NIFFCT;				 break;
   case RC0214_Session	    : rc = ro0802_informSpm (pmCxt, inNo, info); break;
   case RC0215_Presentation : rc = ro0406_informPpm (pmCxt, inNo, info); break;
   case RC0216_Acse	    : rc = ro060a_informAcpm(pmCxt, inNo, info); break;
   case RC0217_LcrtFct	    : rc = ro0502_informLowerCtrl (pmCxt, inNo, info); 
									 break;
   case RC0219_Rose	    : rc = ro0705_informRopm(pmCxt, inNo, info); break;
   case RC0221_RoseUser     : rc = -R_NIFFCT;				 break;
   default		    : rc = -R_NIFFCT;				 break;
}

RM0003_RETURN (rs0292, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0293_cfPdvEnFct				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-11-10					      */
/*                                                                    */
/* SYNTAX       :                                                     */

   signed   short int PASCAL ro0293_cfPdvEnFct (
   signed   short int	     cnfNo,
   signed   short int	     pmNo,
   rTro76_stDefinedBy 	      FAR  * defBy,
   rTro01_pfvObjectId 	      tSyNa,
   unsigned long  int	     store,
   void               NEAR * pcxt,
   unsigned long  int FAR  * pdvN,
   unsigned long  int FAR  * len,
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

RM0002_ENTRY (rs0293)

switch (cnfNo) {
   case RC0241_CfRoseOnly: goto lRosOnlyOsiStack;
   default:		 RM0003_RETURN (rs0293, signed short int, -R_NPSTCF)
}

/* ****************** RosOnlyOsiStack: SPM, PPM, ACPM,  ************* */
/* ******************       	       ROPM.		************* */

lRosOnlyOsiStack:

switch (pmNo) {
 case RC0216_Acse    :
      rc = ro0602_encodAcpm (defBy, tSyNa, store, pcxt, pdvN, len ,iAnyNumb);
      break;

 case RC0219_Rose    :
      rc = ro0702_encodRopm (defBy, tSyNa, store, pcxt, pdvN, len, iAnyNumb);
      break;

 case RC0221_RoseUser:
      rc = ro1502_encodUpperCtrl(defBy,tSyNa,store,pcxt, pdvN, len, iAnyNumb);
      break;

 default	     :
      rc = -R_NPDVEN;
      break;
}

RM0003_RETURN (rs0293, signed short int, rc)
}/*end*/


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
   rTro76_stDefinedBy 	      FAR *  defBy,
   rTro01_pfvObjectId 	        tSyNa,
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

/* ****************** RosOnlyOsiStack: SPM, PPM, ACPM,  ************* */
/* ******************       	       ROPM.		************* */

lRosOnlyOsiStack:

switch (pmNo) {
 case RC0216_Acse    :
      rc = ro0603_decodAcpm      (absId,chTag,defBy,tSyNa,store,pcxt,pdvN,len, 
                                   iAnyNumb  );
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

/* ****************** RosOnltOsiStack:  SPM, PPM, ACPM, ************* */
/* ******************       		ROPM.   	************* */

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

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0296_cfX410EnFct				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-02-24					      */
/*                                                                    */
/* SYNTAX       :                                                     */

   signed   short int PASCAL ro0296_cfX410EnFct (
   signed   short int	     cnfNo,
   signed   short int        pmNo,
   unsigned long  int FAR  * pStore,
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

RM0002_ENTRY (rs0296)

switch (cnfNo) {
   case RC0241_CfRoseOnly: goto lRosOnlyOsiStack;
   default:		  RM0003_RETURN (rs0296, signed short int, -R_NPSTCF)
}

/* ****************** RosOnlyOsiStack: SPM, PPM, ACPM,  ************* */
/* ******************       	       ROPM.		************* */

lRosOnlyOsiStack:

RM0003_RETURN (rs0296, signed short int, -R_D410EN)
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

/* ****************** RosOnlyOsiStack: SPM, PPM, ACPM,  ************* */
/* ******************       	       ROPM.		************* */

lRosOnlyOsiStack:

RM0003_RETURN (rs0297, signed short int, -R_D410DE)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0298_cfSniPEnFct				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-30					      */
/*                                                                    */
/* SYNTAX       :                                                     */

   signed   short int PASCAL ro0298_cfSniPEnFct (
   signed   short int	     cnfNo,
   signed   short int	     pmNo,
   unsigned long  int	     store,
   void               NEAR * pcxt,
   unsigned long  int FAR  * pdvN)


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

RM0002_ENTRY (rs0298)

switch (cnfNo) {
   case RC0241_CfRoseOnly: goto lRosOnlyOsiStack;
   default:		 RM0003_RETURN (rs0293, signed short int, -R_NPSTCF)
}

/* ****************** RosOnlyOsiStack: SPM, PPM, ACPM,  ************* */
/* ******************       	       ROPM.		************* */

lRosOnlyOsiStack:

switch (pmNo) {
 case RC0216_Acse    :
      rc = ro060b_encodSniPAcpm      (store, pcxt, pdvN );
      break;

 case RC0219_Rose    : 
      rc = ro070a_encodSniPRopm      (store, pcxt, pdvN);
      break;

 case RC0221_RoseUser:
      rc = ro1506_encodSniPUpperCtrl (store, pcxt, pdvN);
      break;

 default	     :
      rc = -R_NPDVEN;
      break;
}

RM0003_RETURN (rs0298, signed short int, rc)
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
   signed   short int	     cnfNo,
   signed   short int	     pmNo,
   unsigned long  int	     store,
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

/* ****************** RosOnlyOsiStack: SPM, PPM, ACPM,  ************* */
/* ******************       	       ROPM.		************* */

lRosOnlyOsiStack:

switch (pmNo) {
 case RC0216_Acse    :
      rc = ro060c_decodSniPAcpm      (psAbsId, store, pcxt, pdvN);
      break;

 case RC0219_Rose    :
      rc = ro070b_decodSniPRopm      (psAbsId, store, pcxt, pdvN);
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

#endif  /* RC0043_LuRosSpm */
