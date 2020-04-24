/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm03.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:20  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:45  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:03  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:24  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:33  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:08:07  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:00  keutel
 * 	creation
 * 	[1994/03/21  13:36:39  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rospm03.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:20 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1993                     */
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
/* NAME         : rospm03.c                                           */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* COMPONENT    : RTROS Convergence protocol machine                  */
/*                                                                    */
/* PRD#/VERS.   : RTROS V-3.00                                        */
/*                                                                    */
/* DESCRIPTION  : provides the abort functions for the                */
/*                convergence protocol machine.                       */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-04-19|  original                      |ma  |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "@(#) rospm03.c 1.2 93/12/20 RTROS-V3.00";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>    /* Include of ROS global defines              */
#include <roerval.h>    /* Includes of error values                   */
#include <routil0.h>    /* Include of ROS common utilites             */
#include <rouply0.h>    /* Common generation constants,types & values */
#include <rouply1.h>    /* PM service interface structures            */ 
#include <rodpat0.h>    /* Prototyping of Dispatcher functions        */
#include <rostor0.h>    /* Prototyping of Store functions             */

#include <rotpm00.h>    /* Include of TPM events                      */
#include <rospm00.h>    /* Include of SPM events                      */

#include <rospm01.h>    /* Defines for coding functions               */
#include <rospm03.h>    /* Defines for convergence protocol m/c       */  
#include <rospm05.h>    /* Prototyping of coding of functions         */ 
#include <rospm07.h>    /* Includes local to protocol machine         */ 
#include <rospm06.h>    /* Prototyping of convergence protocol m/c    */    

/*********************************************************************/
/*                                                                   */
/*              G L O B A L   V A R I A B L E S                      */
/*                                                                   */
/*********************************************************************/

/*********************************************************************/
/*                                                                   */
/*              G L O B A L   F U N C T I O N S                      */
/*                                                                   */
/*********************************************************************/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0830_AbortLogicalFalse 			      */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0830_AbortLogicalFalse (
rT0870_stSpmContext *pContext,
unsigned long int hP1,
unsigned long int hP2,
unsigned long int hP3,
unsigned long int hP4)

/* DESCRIPTION  :      predicates:   -------                          */
/*                     actions   :   -------                          */
/*                     next event:   -------                          */
/*                     next state:   -------                          */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*      hP1      = First parameter of transaction function.           */
/*      hP2      = Second parameter of transaction function.          */
/*      hP3      = Third parameter of transaction function.           */
/*      hP4      = Fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT PARAMETERS      :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro0830_AbortLogicalFalse*/

   RM0001_STRG(functName,"ro0830_AbortLogicalFalse")
   RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */
  /*   negativ synchron error  */

RM0003_RETURN(functName, signed short int, -R_SUNKEV)
}/*end-ro0830_AbortLogicalFalse*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0831_InvalidSpdu                                  */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0831_InvalidSpdu (
rT0870_stSpmContext *pContext,
unsigned long int hUserData,
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)

/* DESCRIPTION  :      predicates:   -------                          */
/*                     actions   :   AB                               */
/*                     next event:   T_DATAreq + S_P_ABORT            */
/*                     next state:   RC08db_STA16                     */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*     hUserData = Handle to store.                                   */
/*     ulOffset  = Offset of store from where encoding /decoding      */
/*                 starts.                                            */
/*     hP3       = Third parameter of transaction function.           */
/*     hP4       = Fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro0831_InvalidSpdu*/

unsigned long  int        hInData;  

unsigned long  int        hOutData;
char               FAR  * pOutData;

rT0811_stAB               stAB;
unsigned long  int        ulParam1 = LNULL;
unsigned long  int        ulParam2 = LNULL;
signed   short int        sRetCode = OK;

  RM0001_STRG(functName,"ro0831_InvalidSpdu")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

  hInData = hUserData;
 
  if(ro0075_deletestore(hInData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

  if(pContext->usState != RC08d8_STA03)
   sRetCode = ro0211_pushOutEvent(RC0215_Presentation,
                        RC080b_SPABORTind,
                        NULL,
                        NULL,                  
                        NULL,                       /* privat abort reason  */
                        MAKELONG(0x0501,0x0085) );  /* no reason            */
   if (sRetCode != OK)
       RM0003_RETURN(functName, signed short int, sRetCode )

    pContext->usUserConnExists = FALSE;

/*--------------------------------------------------------------------*/
/* encoding of AB SPDU                                                */ 
/*--------------------------------------------------------------------*/
   
    stAB.usFunctionalUnits    =  RC08ca_FUValue;
    stAB.ulEnclosure          =  RC0876_OneSPDU;  /* PROBLEM */
    stAB.usTdiscAreuse        =  RC0871_NotReuse;                      
    stAB.usTdiscUserAbort     =  RC0858_ProtAbort;                      
    stAB.ucReflParam[0]       =  NULL; 
    stAB.sReflParamLen        =  1;

    hOutData = ro0060_newstore(ST_USEMAXBUF,ST_CLOSED,
                     (signed short int FAR *)&sRetCode);
    if(sRetCode != OK)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)

    /********  
     Since new store is created here , and ulOffset is not comming 
     from PPM , assign ulOffset == max. offset required by any PDU 
     in SPM.
    ********/
    ulOffset = RC0831_MaxParamLen;  

    if ((pOutData = ro0069_stputs(hOutData,ulOffset)) == NULL)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)

    stAB.pDataptr = pOutData + ulOffset; 
    stAB.lDatalen = NULL;
    
    ro0811_EncodeAB(( rT0811_stAB FAR *)&stAB);
 
    ulParam1 = hOutData;
    ulParam2 = (unsigned long int)(stAB.pDataptr - pOutData);

    if(ro0073_storeidle(hOutData) != OK)
        RM0003_RETURN(functName, signed short int, -R_FSTERR )

    pContext->usState = RC08db_STA16;

    sRetCode = ro0211_pushOutEvent(RC0213_Transport,
                        RC0907_TDATAreq,
                        ulParam1,
                        ulParam2, 
                        NULL,
                        NULL);
    
    RM0003_RETURN(functName, signed short int, sRetCode)
}/*end-ro0831_InvalidSpdu*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0832_LocalMatter                                  */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0832_LocalMatter (
rT0870_stSpmContext *pContext,
unsigned long int hP1,
unsigned long int hP2,
unsigned long int hP3,
unsigned long int hP4)

/* DESCRIPTION  :      predicates:   -------                          */
/*                     actions   :   -------                          */
/*                     next event:   -------                          */
/*                     next state:   -------                          */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*      hP1      = First parameter of transaction function.           */
/*      hP2      = Second parameter of transaction function.          */
/*      hP3      = Third parameter of transaction function.           */
/*      hP4      = Fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS       :                                      */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro0832_LocalMatter*/

  RM0001_STRG(functName,"ro0832_LocalMatter")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

  /*   negativ synchron error  */

RM0003_RETURN(functName, signed short int, -R_SLSQFL)
}/*end-ro0832_LocalMatter*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0833_NothingHappens                               */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0833_NothingHappens (
rT0870_stSpmContext *pContext,
unsigned long int hP1,
unsigned long int hP2,
unsigned long int hP3,
unsigned long int hP4)

/* DESCRIPTION  :      predicates:   -------                          */
/*                     actions   :   -------                          */
/*                     next event:   -------                          */
/*                     next state:   -------                          */
/*                                                                    */
/*                                                                    */
/*           nothing happens                                          */
/*           state(before) = state(after)                             */
/*           RC08en_NoEvent                                           */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*      hP1      = First parameter of transaction function.           */
/*      hP2      = Second parameter of transaction function.          */
/*      hP3      = Third parameter of transaction function.           */
/*      hP4      = Fourth parameter of transaction function.          */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro0833_NothingHappens*/

  RM0001_STRG(functName,"ro0833_NothingHappens")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */
;
      /*   nothing happens                */
      /*   state(before) = state(after)   */
      /*   RC08en_NoEvent                       */


RM0003_RETURN(functName, signed short int, OK)
}/* end-ro0833_NothingHappens*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : NoProtocolEvent                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0834_NoProtocolEvent (
rT0870_stSpmContext *pContext,
unsigned long int hP1,
unsigned long int hP2,
unsigned long int hP3,
unsigned long int hP4)

/* DESCRIPTION  :      predicates:   -------                          */
/*                     actions   :   -------                          */
/*                     next event:   -------                          */
/*                     next state:   -------                          */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*      hP1      = First parameter of transaction function.           */
/*      hP2      = Second parameter of transaction function.          */
/*      hP3      = Third parameter of transaction function.           */
/*      hP4      = Fourth parameter of transaction function.          */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro0834_NoProtocolEvent*/

  RM0001_STRG(functName,"ro0834_NoProtocolEvent")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

  /*   negativ synchron error  */

RM0003_RETURN(functName, signed short int, -R_SUNKEV)
}/*end-ro0834_NoProtocolEvent*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0835_Abort1                                       */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0835_Abort1 (
rT0870_stSpmContext *pContext,
unsigned long int hP1,
unsigned long int hP2,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   ------------                     */
/*                     actions   :   ------------                     */
/*                     next event:   T_DISCONNECTrequest              */
/*                     next state:   RC08d0_STA01                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*      hP1      = First parameter of transaction function.           */
/*      hP2      = Second parameter of transaction function.          */
/*      hP3      = Third parameter of transaction function.           */
/*      hP4      = Fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro0835_Abort1*/

  RM0001_STRG(functName,"ro0835_Abort1")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

  pContext->usState = RC08d0_STA01;

RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent(RC0213_Transport,
                                                          RC0905_TDISCONNECTreq,
                                                          NULL,
                                                          NULL, 
                                                          NULL, 
                                                          NULL))
}/*end-ro0835_Abort1*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0836_Abort2                                       */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0836_Abort2 (
rT0870_stSpmContext *pContext,
unsigned long int hP1,
unsigned long int hP2,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   ---------                        */
/*                     actions   :    [3]                             */
/*                     next event:   T_DISCONNECTreqest               */
/*                     next state:   RC08d0_STA01                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*      hP1      = First parameter of transaction function.           */
/*      hP2      = Second parameter of transaction function.          */
/*      hP3      = Third parameter of transaction function.           */
/*      hP4      = Fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro0836_Abort2*/

  RM0001_STRG(functName,"ro0836_Abort2")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

/* !!!!!!!!!!!!!!!!!!   TIMER fehlt !!!!!!!!!!!!!!!!!!!!!!!! */

  ro080a_InitContext(pContext);
  pContext->usState = RC08d0_STA01;
  
RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent(RC0213_Transport,
                                                          RC0905_TDISCONNECTreq,
                                                          NULL,
                                                          NULL,
                                                          NULL,
                                                          NULL))
      
}/*end-ro0836_Abort2*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0837_Abort4                                       */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0837_Abort4 (
rT0870_stSpmContext *pContext,
unsigned long int hP1, 
unsigned long int hP2,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   ---------                        */
/*                     actions   :    [3]                             */
/*                     next event:   ---------                        */
/*                     next state:   RC08d0_STA01                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*      hP1      = First parameter of transaction function.           */
/*      hP2      = Second parameter of transaction function.          */
/*      hP3      = Third parameter of transaction function.           */
/*      hP4      = Fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro0837_Abort4*/
  
  RM0001_STRG(functName,"ro0837_Abort4")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

/* !!!!!!!!!!!!!!!!!!   TIMER fehlt !!!!!!!!!!!!!!!!!!!!!!!! */

  ro080a_InitContext(pContext);
  pContext->usState = RC08d0_STA01;
  
RM0003_RETURN(functName, signed short int, OK)
}/*end-ro0837_Abort4*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0838_Abort5                                       */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0838_Abort5 (
rT0870_stSpmContext *pContext,
unsigned long int hP1, 
unsigned long int hP2,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   ----------------                 */
/*                     actions   :   ----------------                 */
/*                     next event:   ----------------                 */
/*                     next state:   STA01                            */
/*                                                                    */
/*                     call after this events:                        */
/*                                                                    */
/*                     T_DIS_IND      in state RC08d3_STA01C          */
/*                     T_DIS_IND      in state RC08d4_STA01D          */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*      hP1      = First parameter of transaction function.           */
/*      hP2      = Second parameter of transaction function.          */
/*      hP3      = Third parameter of transaction function.           */
/*      hP4      = Fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro0838_Abort5*/

  RM0001_STRG(functName,"ro0838_Abort5")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

  ro080a_InitContext(pContext);
  pContext->usState = RC08d0_STA01;
  
RM0003_RETURN(functName, signed short int, OK)
}/*end-ro0838_Abort5*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0839_TDISCONind                                   */
/*                                                                    */
/* AUTHOR       :                                                     */
/* DATE         :                                                     */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0839_TDISCONind (
rT0870_stSpmContext *pContext,
unsigned long int hP1, 
unsigned long int ulAddAboInfo, 
unsigned long int ulAboReason,
unsigned long int hP4)

/* DESCRIPTION  :      predicates:   -------                          */
/*                     actions   :   -------                          */
/*                     next event:   SPABind                          */
/*                     next state:   RC08d0_STA01                     */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*      hP1      = First parameter of transaction function.           */
/*  ulAddAboInfo = Additional info about abort reason.                */
/*  ulAboReason  = Abort reaason                                      */
/*      hP4      = fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro0839_TDISCONind*/

  RM0001_STRG(functName,"ro0839_TDISCONind")
  signed short int    sRetCode = OK;

  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */

  if (pContext->usUserConnExists) {
      /*---------------------------------------------------------------------*/
      /* Note: User connection might not exist if a S-U-ABORT-req was issued */
      /*       between a T-CONreq and T-CONcnf.                              */
      /*---------------------------------------------------------------------*/
      sRetCode = ro0211_pushOutEvent (RC0215_Presentation, RC080b_SPABORTind,
                                      NULL, ulAddAboInfo, NULL, ulAboReason);
  }

  pContext->usState          = RC08d0_STA01;  
  pContext->usUserConnExists = FALSE;

  RM0003_RETURN (functName, signed short int, sRetCode)

}/*end-ro0839_TDISCONind*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro083a_SUABORTreq                                   */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro083a_SUABORTreq (
rT0870_stSpmContext *pContext,
unsigned long int hUserData, 
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)

/* DESCRIPTION  :      predicates:   ----                             */
/*                     actions   :   [4]                              */
/*                     next event:   AB-nr                            */
/*                     next state:   RC08db_STA16                     */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*     hUserData = Handle to store.                                   */
/*     ulOffset  = Offset of store from where encoding /decoding      */
/*                 starts.                                            */
/*     hP3       = Third parameter of transaction function.           */
/*     hP4       = Fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro083a_SUABORTreq*/

unsigned long  int        hOutData;
char               FAR  * pOutData;

rT0811_stAB               stAB;
unsigned long  int        ulParam1  = LNULL;
unsigned long  int        ulParam2  = LNULL;
signed   long  int        lStorelen = LNULL;
signed   short int        sRetCode  = OK;

  RM0001_STRG(functName,"ro083a_SUABORTreq")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */
  lStorelen = 0;
  ulParam1 = ulParam2 = 0;
  hOutData = hUserData;

  stAB.usFunctionalUnits = RC08ca_FUValue;

  if (hOutData == NULL)         /* No user data  */
  {
    hOutData = ro0060_newstore(ST_USEMAXBUF,ST_CLOSED,
                              (signed short int FAR *)&sRetCode);
    if(sRetCode != OK)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)

    lStorelen = ro0062_storelength(hOutData); 
    if (ro0063_sterror(hOutData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)

    ulOffset = RC0831_MaxParamLen; 
    stAB.lDatalen = NULL;
  }
  else
  {
    if(ro0074_storeactive(hOutData , 0L) != OK ) 
         RM0003_RETURN(functName, signed short int, -R_FSTERR )

    lStorelen = ro0062_storelength(hOutData); 
    if(ro0063_sterror(hOutData) != OK )
         RM0003_RETURN(functName, signed short int, -R_FSTERR)
    stAB.lDatalen = lStorelen - ulOffset;
    /* by kc on 16-11-93 */
    if (stAB.lDatalen > RC0835_MaxConnectLen){
     RM0003_RETURN(functName, signed short int,                              
       ro0831_InvalidSpdu(pContext,hOutData,RC0831_MaxParamLen,NULL,NULL))  
    } 
  }

  if ((pOutData = ro0069_stputs(hOutData,ulOffset)) == NULL)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
  stAB.pDataptr = pOutData + ulOffset;

  stAB.usTdiscUserAbort  = RC0856_UserAbort;
  stAB.usTdiscAreuse     = RC0871_NotReuse;
  stAB.ulEnclosure       =  RC0876_OneSPDU; /* PROBLEM */ 
  stAB.ucReflParam[0]    = 1;               /* PROBLEM */ 
  stAB.sReflParamLen     = 1;
 
  ro0811_EncodeAB(( rT0811_stAB FAR *)&stAB);
  
/*--------------------------------------------------------------------*/
/* Send the buffer at once                                            */
/*--------------------------------------------------------------------*/
  ulParam1 = hOutData;
  ulParam2 = (unsigned long int)(stAB.pDataptr - pOutData);
  
  pContext->usState = RC08db_STA16;     /* ie. Wait for TCONind  */
  
  if(ro0073_storeidle(hOutData) != OK)
      RM0003_RETURN(functName, signed short int, -R_FSTERR )

    /*  start TIMER not implemented  */

RM0003_RETURN(functName, signed short int, ro0211_pushOutEvent(RC0213_Transport,
                                                               RC0907_TDATAreq,
                                                               ulParam1,
                                                               ulParam2, 
                                                               NULL,
                                                               NULL))

}/*end-ro083a_SUABORTreq*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro083b_Ab                                           */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro083b_Ab (
rT0870_stSpmContext *pContext,
unsigned long int hUserData, 
unsigned long int ulOffset,
unsigned long int hP3,
unsigned long int hP4)

/* DESCRIPTION  :      predicates:   p02                              */
/*                     actions   :   -------                          */
/*                     next event:   SxABind                          */
/*                     next event:   AA                               */
/*                     next state:   RC08db_STA16                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*     hUserData = Handle to store.                                   */
/*     ulOffset  = Offset of store from where encoding /decoding      */
/*                 starts.                                            */
/*     hP3       = Third parameter of transaction function.           */
/*     hP4       = Fourth parameter of transaction function.          */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro083b_Ab*/

unsigned long  int         hInData   = LNULL;
char                FAR  * pInData   = NULL;

rT0811_stAB stAB;
unsigned long  int         ulParam1  = LNULL;
unsigned long  int         ulParam2  = LNULL;
unsigned long  int         ulParam3  = LNULL;
unsigned long  int         ulParam4  = LNULL;
signed   long  int         lStorelen = LNULL;
signed   long  int         lBuflen   = LNULL;
signed   long  int         event     = LNULL;
signed   short int         sRetCode  = OK;


  RM0001_STRG(functName,"ro083b_Ab")

/* ****************** S T A R T ************************************* */

  RM0002_ENTRY(functName)

  hInData = hUserData;

  if (!(pContext->usTexp))        /*  p02  => ^TEXP  */
  {

    if(ro0074_storeactive(hInData , 0L) != OK ) 
        RM0003_RETURN(functName, signed short int, -R_FSTERR )

    lBuflen = ro080a_StoreOffset(0,hInData,ulOffset);
    if( ! lBuflen)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
    if ((pInData = ro0072_stgets(hInData,lBuflen )) == NULL)
        RM0003_RETURN(functName, signed short int, -R_FSTERR)

    stAB.usFunctionalUnits   = RC08ca_FUValue; 
    stAB.pDataptr            = pInData + ulOffset; 

    lStorelen = ro0062_storelength(hInData); 
    if(ro0063_sterror(hInData) != OK )
        RM0003_RETURN(functName, signed short int, -R_FSTERR)
    stAB.lDatalen  = lStorelen - ulOffset;
   
    if ( ro0821_DecodeAB(( rT0811_stAB FAR *)&stAB) != 0)
     { 
       if(ro0073_storeidle(hInData) != OK)
           RM0003_RETURN(functName, signed short int, -R_FSTERR )
       RM0003_RETURN(functName, signed short int, 
       ro0831_InvalidSpdu(pContext,hInData,RC0831_MaxParamLen,NULL,NULL))
     }
              
    if (stAB.usTdiscUserAbort == RC0856_UserAbort)
    {
      if (stAB.lDatalen != NULL)    /* User Data Available  */
      {
        ulParam1 = hInData;
        ulParam2 = (unsigned long int)(stAB.pDataptr - pInData);

        if(ro0073_storeidle(hInData) != OK)
           RM0003_RETURN(functName, signed short int, -R_FSTERR )
      }
      else                      /* User Data not available */
      {
        ulParam1 = NULL;
        ulParam2 = NULL;

        if(ro0073_storeidle(hInData) != OK)
            RM0003_RETURN(functName, signed short int, -R_FSTERR )

        if(ro0075_deletestore(hInData) != OK)
            RM0003_RETURN(functName, signed short int, -R_FSTERR )
      }
      event  = RC080a_SUABORTind;
      ulParam3 = NULL;
      ulParam4 = NULL;
    }
    else  /* of if (stAB.usTdiscUserAbort != RC0856_UserAbort) */
    {
      if (stAB.usTdiscProtAbort == RC0858_ProtAbort)
         ulParam4 = MAKELONG(0x0501, stAB.ucReflParam[0]);
      else
        if (stAB.usTdiscNoReason == RC0855_NoReason)
          ulParam4 = NULL;
        else                             
          ulParam4 = (unsigned long int)-1;   /* implementation restrictions  */

      event    = RC080b_SPABORTind;
      ulParam1 = NULL;
      ulParam2 = NULL;            /* additional information if local reason */
      ulParam3 = NULL;
    }
        
    sRetCode = ro0211_pushOutEvent(RC0215_Presentation,
                        event,
                        ulParam1,
                        ulParam2,
                        ulParam3,
                        ulParam4);
    if (sRetCode != OK)
        RM0003_RETURN(functName, signed short int, sRetCode)

    pContext->usUserConnExists = FALSE;

    sRetCode = ro0211_pushOutEvent(RC0213_Transport,
                        RC0905_TDISCONNECTreq,
                        NULL,
                        NULL, 
                        NULL, 
                        NULL);
    if (sRetCode != OK)
        RM0003_RETURN(functName, signed short int, sRetCode)
        
    pContext->usState = RC08d0_STA01;
  }
  else
    sRetCode = -R_SINTER;

RM0003_RETURN(functName, signed short int, sRetCode)
}/*end-ro083b_Ab*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0878_PendAbort                                    */
/*                                                                    */
/* AUTHOR       : Shivkumar                                           */
/* DATE         : 93-11-30                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro0878_PendAbort (
rT0870_stSpmContext *pContext,
unsigned long int hP1,
unsigned long int hP2,
unsigned long int hP3,
unsigned long int hP4)

/*                                                                    */
/* DESCRIPTION  :      predicates:   ------------                     */
/*                     actions   :   ------------                     */
/*                     next event:   Nothing                          */
/*                     next state:   Same as current state            */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*      hP1      = First parameter of transaction function.           */
/*      hP2      = Second parameter of transaction function.          */
/*      hP3      = Third parameter of transaction function.           */
/*      hP4      = Fourth parameter of transaction function.          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
/*                                                                    */
/*  pContext  = pointer to SPM context block. ( rT0870_stSpmContext ) */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
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
{/*entr-ro0878_PendAbort*/

  RM0001_STRG  (functName, "ro0878_PendAbort")
  signed short int sRetCode = OK;

  RM0002_ENTRY (functName)

/* ****************** S T A R T ************************************* */
  pContext->bAbortPend = TRUE;

  if (hP1) {
      if (ro0075_deletestore (hP1) != OK) sRetCode = -R_FSTERR;
      else                                pContext->usUserConnExists = FALSE;
  }

  RM0003_RETURN (functName, signed short int, sRetCode)

}/*end-ro0878_PendAbort*/


/*EOF*/
