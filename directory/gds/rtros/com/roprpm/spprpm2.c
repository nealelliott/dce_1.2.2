/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spprpm2.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:45  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:08:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:31:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:17:51  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:49  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:33  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:53:24  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:33:54  keutel
 * 	creation
 * 	[1994/03/21  13:22:30  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spprpm2.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:45 $" } ;
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
/* NAME 	: spprpm2.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K                                      */
/* DATE 	: 25-11-93					      */
/*                                                                    */
/* COMPONENT    : RTROS Evaluation Program                            */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : In this module the the dispatcher functions         */
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

static char *sccsid =    "@(#) spprpm2.c 1.1 93/12/20   RTS-V2.00";

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
#include <rodpat3.h>      /* Dispatcher Config funs  interface        */

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
/* NAME 	: ro0248_decodingPm				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-12-01					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0248_decodingPm (
   signed   short int	     pmNo,
   signed   short int	     absId,
   signed   long  int	     chTag,
   rTro76_stDefinedBy FAR  * defBy,
   rTro01_pfvObjectId	     tSyNa,
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

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0248)
RM0003_RETURN (rs0248, signed short int, ro0294_cfPdvDeFct (RC0241_CfRoseOnly,
							    pmNo	    ,
							    absId	    ,
							    chTag	    ,
							    defBy	    ,
							    tSyNa	    ,
							    store	    ,
							    pcxt	    ,
							    pdvN	    ,
							    len 	    ,
                                                            iAnyNumb         ))
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0243_decodSniP				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-30					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0243_decodSniP (
	signed short int          sPmNo,
	unsigned long  int 	  hsStore,
	void               NEAR * pContext,
	unsigned long  int FAR  * phNode,
	signed short int   FAR  * psAbsId)

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

RM0002_ENTRY  (rs0243)
RM0003_RETURN (rs0243, signed short int, ro0299_cfSniPDeFct (RC0241_CfRoseOnly,
				                             sPmNo           , 
							     hsStore	     ,
							     pContext	     ,
							     phNode	     ,
                                                             psAbsId         ))
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0245_decodX410				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-02-24					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL  ro0245_decodX410 (
	signed short   int        sPmNo,
	unsigned       int        uInEvent,
	signed   long  int        lType,
	unsigned long  int        hsStore,
	void 		   NEAR * pContext,
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

RM0002_ENTRY  (rs0245)
RM0003_RETURN (rs0245, signed short int, ro0297_cfX410DeFct (RC0241_CfRoseOnly,
				                             sPmNo           , 
				                             uInEvent        , 
                                	                     lType           ,	
							     hsStore	     ,
							     pContext	     ,
							     phNode	     ))
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0246_clearPdvTree				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-12-01					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0246_clearPdvTree (
   unsigned long  int FAR   *pdvNode)

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

signed short int rc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0246)

if (*pdvNode) rc = ro0295_cfPdvClFct (RC0241_CfRoseOnly, pdvNode);
else	      rc = 0;

RM0003_RETURN (rs0246, signed short int, rc)
}/*end*/



