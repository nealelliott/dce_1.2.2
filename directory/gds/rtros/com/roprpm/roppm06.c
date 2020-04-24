/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm06.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:48  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:13:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:02  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:04  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:15  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:12  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:52:07  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:45  keutel
 * 	creation
 * 	[1994/03/21  13:21:20  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roppm06.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:53 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1992                     */
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
/* TYPE         : C-MODULE                                            */
/*                                                                    */
/* NAME         : roppm06.c                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-06-29                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Presentation Protocol Automata (PPA)                */
/*                                                                    */
/*   This module contains the State-Event-Matrix according to         */
/*   ISO 8823 (kernel functional unit) and all functions which are    */
/*   called by this matrix (expect of connection establishment        */
/*   transaction functions).                                          */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-03-24|  adaptation of ROS V2.5A00 K05 |hm  |         */
/* 2.0A00 K01|92-04-24|  invert objcpy rc > 0          |hm  |         */
/* 2.0A00 K02|92-06-29|  SCCS-Id changed               |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/**********************************************************************/
/*                                                                    */
/* Note:                                                              */
/* The use of the Hungarian Notation to mark the C data types         */
/* is as follows:                                                     */
/*                                                                    */
/*   b   boolean (i.e. in the most cases signed int)                  */
/*   bs  bit string (i.e. unsigned long)                              */
/*   c   signed char                                                  */
/*   h   memory handle (i.e. unsigned long)                           */
/*   hs  Store handle (i.e. unsigned long)                            */
/*   i   signed int                                                   */
/*   l   signed long                                                  */
/*   o   object identifier                                            */
/*   p   pointer to any data type                                     */
/*   s   signed short                                                 */
/*   st  structure                                                    */
/*   u   unsigned int                                                 */
/*   uc  unsigned char                                                */
/*   ul  unsigned long                                                */
/*   un  union                                                        */
/*   us  unsigned short                                               */
/*   v   vector of any data type                                      */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/* Note:                                                              */
/* In order to mark code parts which are not clear or which are still */
/* missing, the use of some strings is as follows:                    */
/*                                                                    */
/*   S!?  open question (regarding ISO standard)                      */
/*   P!?  open question (regarding programming style)                 */
/*   !!   has still to be done                                        */
/*   !!!! has still to be done (very important)                       */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) roppm06.c 1.2 93/12/20   RTS-V2.00";


/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>      /* Machine dependent generation             */
#include <roerval.h>      /* Unrecoverable error codes                */
#include <rouply0.h>      /* Common type and value definitions        */
#include <rouply1.h>      /* PM service interface structures          */
#include <routil0.h>      /* Utility interface (common functions)     */
#include <routil1.h>      /* Utility interface (object id. functions) */
#include <rodpat0.h>      /* Dispatcher interface                     */
#include <rospm00.h>      /* SPM service interface                    */
#include <roppm00.h>      /* PPM service interface                    */
#include <roppm01.h>      /* PPM protocol interface                   */
#include <roppm04.h>      /* PPM entity invoc. context (before 03.h!) */
#include <roppm03.h>      /* PPM logging interface                    */
#include <roppm11.h>      /* PPM intern: types and constants          */
#include <roppm14.h>      /* PPM intern: types and constants          */

/**********************************************************************/
/*                                                                    */
/*        (1)   Local Sequence, Protocol Error and Provider abort     */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0460_localError                                   */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0460_localError (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/* DESCRIPTION  : Indicate a Local Sequence Flow Error                */
/*                (leads to synchronous PPM error code).              */
/*                                                                    */
/*   Called from the PPA Event-State-Matrix if a sequence flow        */
/*   error caused by the l o c a l P-user (or by the PPM) occurs.     */
/*   (If a sequence flow error is caused by the p e e r entity        */
/*   ro0461_dummyUnexpectedPpdu() or ro0462_dummyUnexpectedSessEvent()*/                              
/*   is called.)                                                      */
/*                                                                    */
/*   The function is very short, but a function is required by        */
/*   the Event-State-Matrix (furthermore, this function can be        */
/*   usefull at the debugging and for the function trace logging).    */
/*                                                                    */
/* IN-PARAMETERS  : none                                              */
/*                                                                    */
/* OUT-PARAMETERS : none                                              */
/*                                                                    */
/* RETURN VALUE   :                                                   */
/*                                                                    */
/*   - R_PLSQFL   - Presentation PM local sequence flow error         */
/*                  (see roerval.h)                                   */
/*                  Note: This returncode can be issued in            */
/*                  other functions too.                              */
/*                                                                    */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0460_localError")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY  (fnctName)
  RM0003_RETURN (fnctName, signed short int, -R_PLSQFL)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0461_dummyUnexpectedPpdu                          */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0461_dummyUnexpectedPpdu (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/* DESCRIPTION  : Indicate a protocol error                           */
/*                (unexpected PPDU).                                  */
/*                                                                    */
/*   Empty dummy function, never called. Only purpose: provide        */
/*   a valid function pointer for comparation with the function       */
/*   pointers of the State-Event-Matrix.                              */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS  : none                                              */
/*                                                                    */
/* OUT-PARAMETERS : none                                              */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0461_dummyUnexpectedPpdu")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY  (fnctName)
  RM0003_RETURN (fnctName, signed short int, RC041m_UnexpectedPpdu)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0462_dummyUnexpectedSessEvent                     */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0462_dummyUnexpectedSessEvent (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/* DESCRIPTION  : Indicate a protocol error                           */
/*                (unexpected session service primitive).             */
/*                                                                    */
/*   Empty dummy function, never called. Only purpose: provide        */
/*   a valid function pointer for comparation with the function       */
/*   pointers of the State-Event-Matrix.                              */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS  : none                                              */
/*                                                                    */
/* OUT-PARAMETERS : none                                              */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0462_dummyUnexpectedSessEvent")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY  (fnctName)
  RM0003_RETURN (fnctName, signed short int, RC041n_UnexpectedSessEvent)
}/*end*/

/*********************************************************************/
/*                                                                   */
/*        (2)   Connection Release (normal) Transaction functions    */
/*                                                                   */
/*********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0463_PRELreq                                      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0463_PRELreq (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0463_PRELreq")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)
  
  /*if pres. PDV node allocated: PS-user data exist. */
  if (pNode){
    *pUserData = (rT0417_stUserData  FAR *)&pNode->stOther.stUserData;
      /* the Boolean for the User data has to be assigned here 
         Since the PDU has the Userdata as mandatory the variable from 
         context block is used here for assigning the boolean variable */
       pContext->bUserData = TRUE;
       *pbUserData = &(pContext->bUserData);
    } 

  if (pContext->bRelease == TRUE)
      pContext->bReleaseCollision = TRUE;
  pContext->bRelease              = TRUE;

  *pOutEvent = RC043w_OEvSRELreq;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0464_SRELind                                      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0464_SRELind (
		rT0430_stContext       *pContext,
		rT0441_stEntContext    *pEntContext,
		rT04a0_unConn      FAR *pConn,
		rT041G_unPpdu      FAR *pNode,
		unsigned long  int FAR * FAR *pX410RtpmNode,
		signed   long  int FAR * FAR *pbUserData,
		rT0417_stUserData  FAR * FAR *pUserData,
		unsigned       int FAR *pOutEvent,
		unsigned long  int FAR *phNode,
		unsigned long  int FAR *pP1,
		unsigned long  int FAR *pP2,
		unsigned long  int FAR *pP3,
		unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0464_SRELind")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*if pres. PDV node allocated: PS-user data exist. */
  if (pNode){
    *pUserData = (rT0417_stUserData  FAR *)&pNode->stOther.stUserData;
      /* the Boolean for the User data has to be assigned here 
         Since the PDU has the Userdata as mandatory the variable from 
         context block is used here for assigning the boolean variable */
       pContext->bUserData = TRUE;
       *pbUserData = &(pContext->bUserData);
    } 

  if (pContext->bRelease == TRUE)
    pContext->bReleaseCollision = TRUE;
  pContext->bRelease = TRUE;

  *pOutEvent = RC043z_OEvPRELind;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0465_SRELcnfPos                                   */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0465_SRELcnfPos (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0465_SRELcnfPos")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*if pres. PDV node allocated: PS-user data exist. */
  if (pNode){
    *pUserData = (rT0417_stUserData  FAR *)&pNode->stOther.stUserData;
      /* the Boolean for the User data has to be assigned here 
         Since the PDU has the Userdata as mandatory the variable from 
         context block is used here for assigning the boolean variable */
       pContext->bUserData = TRUE;
       *pbUserData = &(pContext->bUserData);
    } 


  if (pContext->bReleaseCollision == FALSE)
    pContext->sState = RC0430_StaI0;
  else
  {
    pContext->bRelease          = FALSE;
    pContext->bReleaseCollision = FALSE;
  }

  *pOutEvent = RC043A_OEvPRELcnfPos;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0466_SRELcnfNeg                                   */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0466_SRELcnfNeg (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0466_SRELcnfNeg")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*if pres. PDV node allocated: PS-user data exist. */
  if (pNode){
    *pUserData = (rT0417_stUserData  FAR *)&pNode->stOther.stUserData;
      /* the Boolean for the User data has to be assigned here 
         Since the PDU has the Userdata as mandatory the variable from 
         context block is used here for assigning the boolean variable */
       pContext->bUserData = TRUE;
       *pbUserData = &(pContext->bUserData);
    } 

  pContext->bRelease          = FALSE;
  pContext->bReleaseCollision = FALSE;

  *pOutEvent = RC043B_OEvPRELcnfNeg;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0467_PRELrspPos                                   */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0467_PRELrspPos (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0467_PRELrspPos")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*if pres. PDV node allocated: PS-user data exist. */
  if (pNode){
    *pUserData = (rT0417_stUserData  FAR *)&pNode->stOther.stUserData;
      /* the Boolean for the User data has to be assigned here 
         Since the PDU has the Userdata as mandatory the variable from 
         context block is used here for assigning the boolean variable */
       pContext->bUserData = TRUE;
       *pbUserData = &(pContext->bUserData);
    } 


  if (pContext->bReleaseCollision == FALSE)
    pContext->sState            = RC0430_StaI0;       /*S!?*/
  else
  {
    pContext->bRelease          = FALSE;
    pContext->bReleaseCollision = FALSE;
  }

  *pOutEvent = RC043x_OEvSRELrspPos;
  
  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0468_PRELrspNeg                                   */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0468_PRELrspNeg (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0468_PRELrspNeg")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*if pres. PDV node allocated: PS-user data exist. */
  if (pNode){
    *pUserData = (rT0417_stUserData  FAR *)&pNode->stOther.stUserData;
      /* the Boolean for the User data has to be assigned here 
         Since the PDU has the Userdata as mandatory the variable from 
         context block is used here for assigning the boolean variable */
       pContext->bUserData = TRUE;
       *pbUserData = &(pContext->bUserData);
    } 

  pContext->bRelease          = FALSE;
  pContext->bReleaseCollision = FALSE;

  *pOutEvent = RC043y_OEvSRELrspNeg;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*********************************************************************/
/*                                                                   */
/*        (3)   Connection Release (Abort) Transaction Functions     */
/*                                                                   */
/*********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0469_PUABreq                                       */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-04-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0469_PUABreq (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0469_PUABreq")
rT0414_stPresContId FAR *pAux;          /*aux. pointer to a context list element */
signed         int      iAbsId;         /*abstract syntax identifier */
signed         int      iTransId;       /*transfer syntax identifier */
signed   short int      rc;             /*returncode of a called function */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*----------------*/
  /* X410-mode only */
  /*----------------*/

  if (!pContext->bNormalMode)
  {
    pNode->stAru.lX410orNormalChoice = RC041a_X410Mode; /*rouply0-value P!? */

    *pX410RtpmNode = (unsigned long int FAR *)&pNode->stAru.hX410RtpmNode;
  }


  /*------------------*/
  /* Normal-mode only */
  /*------------------*/

  else
  {
    pNode->stAru.lX410orNormalChoice = RC041b_NormalMode; /*rouply0-value P!? */

    /*----------------------------------------------------------------------*/
    /* Lose PS-user data if session-protocol-version = 1                    */
    /* - See ISO 8823 6.4.2.2 (note regarding length restriction imposed    */
    /*   by the underlying session service).                                */
    /* - OSS V2.0: allows up to 9 byte SS-user data for S-UABORTreq.        */
    /* - There is no special OSS error code "S-UABORTreq user data too      */
    /*   long", that's why the PPM has to do it and there is no other       */
    /*   solution possible.                                                 */
    /*----------------------------------------------------------------------*/

    if (pContext->sSessProtVers == RCro50_ProtVers1)
    {
      rc = ro0246_clearPdvTree (pP1);   /*sets *pP1 to 0 */
      if (rc)
        RM0003_RETURN (fnctName, signed short int, rc)
    }

    /*----------------------------------------------------------------------*/
    /* Process the presentation-context-identifier-list                     */
    /* - In this implementation version: all pres. contexts,                */
    /*   not only those used in the PS-user data                            */
    /*   (ISO 8823 6.4.2.1 is not precise S!?).                             */
    /*----------------------------------------------------------------------*/

    if (*pP1                            /*(if PS-user data exist and          */
    &&  pContext->vAbstrSynResult [0]   /* if there was a PresContDefList and */
    &&  pContext->sDcsNumb              /* if the DCS is not empty)           */
    )
    {
      pNode->stAru.bPresContId  = TRUE;
      pNode->stAru.ulPresContId = pContext->sDcsNumb;

      pAux = pNode->stAru.vPresContId;  /*pAux because better readability */

      for (iAbsId = 0; iAbsId < RCro01_MaxAbstrSyn; iAbsId++)
      {
        if ((iTransId = (signed int) pContext->vTransSynId [iAbsId])
            != RC043O_NoTransId)
        {
          pAux->lPresContId = pContext->vPresContId [iAbsId];

          rc = ro0002_objcpy (
            pAux->oTransSynName,                    /* <-- Destination. */
            pEntContext->vTransSynName [iTransId]); /* --> Source.      */
          if (rc > 0)                               /* because R_LENULL */
            rc = -rc;
          if (rc)                                   /* rc now never > 0 */
            RM0003_RETURN (fnctName, signed short int, rc)

          pAux++;                   /*increment only if not "RC043O_NoTransId" */
        }
      }

    } /*to "if (bPresContId)" */
    /*Note: No "else" because pNode->stAru.bPresContId is 0-initialized. */


    /*----------------------*/
    /* Process PS-user data */
    /*----------------------*/

    *pbUserData = (signed long int    FAR *)&pNode->stAru.bUserData;
    *pUserData  = (rT0417_stUserData  FAR *)&pNode->stAru.stUserData;

  } /*to "if (!X410-mode)" */


  /*---------------------------*/
  /* X410-mode and normal-mode */
  /*---------------------------*/

  pContext->sState  = RC0430_StaI0;

  *pOutEvent        = RC043C_OEvARU;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro046a_ARU                                          */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro046a_ARU (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro046a_ARU")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*----------------*/
  /* X410-mode only */
  /*----------------*/
/* by kc on 02-12-93 Since this bNormalMode is no longer Boolean this check
   is invalid. Gives problem in PTS. So check for the X410 mode i.e., 0 
   * if (!pContext->bNormalMode)                  */
  if(pContext->bNormalMode == RC041a_X410Mode )
  {
    if (pNode->stAru.lX410orNormalChoice != RC041a_X410Mode) 
      RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)
      /*right value S!? */

    *pX410RtpmNode = (unsigned long int FAR *)&pNode->stAru.hX410RtpmNode;
  }


  /*------------------*/
  /* Normal-mode only */
  /*------------------*/

  else
  { /* here add the PTS also by kc on 02-12-93 */
    if ((pNode->stAru.lX410orNormalChoice != RC041b_NormalMode) &&
        ( pContext->bNormalMode != -1) )
      RM0003_RETURN (fnctName, signed short int, RCro6n_InvalidPpduParam)
      /*right value S!? */

    /*----------------------------------------------------------------------*/
    /* Regarding presentation-context-identifier-list see ro0423_syntaxAru()*/
    /*----------------------------------------------------------------------*/

    /*----------------------*/
    /* Process PS-user data */
    /*----------------------*/

    /* Process transfer syntax identifier of products user data (if there  */
    /* are products user data; otherwise the identifier shall be ignored). */
    /* Really necessary only if PUABORTind before PCONNECTcnf              */
    /* but filled always.                                                  */
    /* The last transfer syntax identifier is given to the PS-user.        */
    *pP4 = pContext->sTransSynIdAbo;

    *pbUserData = (signed long int    FAR *)&pNode->stAru.bUserData;
    *pUserData  = (rT0417_stUserData  FAR *)&pNode->stAru.stUserData;

  } /*to "if (!X410-mode)" */


  /*---------------------------*/
  /* X410-mode and normal-mode */
  /*---------------------------*/

  pContext->sState = RC0430_StaI0;

  *pOutEvent       = RC043D_OEvPUABind;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro046b_ARP                                          */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro046b_ARP (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro046b_ARP")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

/* ****************** Mapping of abort reason values **************** */
/* ******************                                **************** */


  /*---------------------------*/
  /* Check protocol parameters */
  /*---------------------------*/

  /* Note: check pContext->bNormalMode necessary S!? (no ARP in X410-mode) */


  /* Note: Here is known if it is a ARP or ARU PPDU (not if e.g. tag error).*/
  /*       That's why one can evaluate the event identifier during offline- */
  /*       troubleshooting and no specific error handling is necessary      */
  /*       (such a specific handling is done e.g. in ROS V2.0, ROS-D V2.5). */

  if (    pNode->stArp.bAbortReason
      &&  (   pNode->stArp.lAbortReason < RC041k_ReasonNotSpecified
          ||  pNode->stArp.lAbortReason > RCro6n_InvalidPpduParam
          )
  ||      pNode->stArp.bEventId
      &&  (   pNode->stArp.lEventId     < 0
          ||  pNode->stArp.lEventId     > 32
          )
  )
    RM0003_RETURN (fnctName, signed short int, OK)



  /*--------------------------------------------*/
  /* Mapping of protocol values to local values */
  /* (with check of protocol parameters)        */
  /*--------------------------------------------*/

  *pP4 =
    0                               /*aborted by peer ppm */
  + RC0400_PpmEvent;                /*protocol machine event number */


  /*abort reason */
  if (pNode->stArp.bAbortReason)
    *pP4 += (unsigned long int) pNode->stArp.lAbortReason << 16;
  /*no "else" necessary because "0" means "reason not specified" */


  /*event identifier */
  if (pNode->stArp.bEventId)
    *pP4 += (unsigned long int) pNode->stArp.lEventId     << 24;
  else
    *pP4 += (unsigned long int) RC040W_NoEventIdReceived  << 24;
    /*"0" means "CP". RC040W_NoEventIdReceived necessary S!? */


  pContext->sState = RC0430_StaI0;

  *pOutEvent       = RC043E_OEvPPABind;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro046c_SPABind                                      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro046c_SPABind (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro046c_SPABind")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pContext->sState = RC0430_StaI0;

  *pOutEvent       = RC043E_OEvPPABind;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*********************************************************************/
/*                                                                   */
/*        (4)   Data Transfer Transaction functions                  */
/*                                                                   */
/*********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro046d_PDTreq                                       */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro046d_PDTreq (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro046d_PDTreq")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  if (pContext->bNormalMode)
    /*if pres. PDV node allocated: PS-user data exist (are mandatory) */
    if (pNode){
      *pUserData = (rT0417_stUserData  FAR *)&pNode->stOther.stUserData;
      /* the Boolean for the User data has to be assigned here 
         Since the PDU has the Userdata as mandatory the variable from 
         context block is used here for assigning the boolean variable */
       pContext->bUserData = TRUE;
       *pbUserData = &(pContext->bUserData);
    } 
      
    
    else
      RM0003_RETURN (fnctName, signed short int, -R_PUDATE)
      /*P1 not available in X410-mode -> no check possible P!? */
      /*checked by encodRtpmX410() P!?                         */


  *pOutEvent = RC043F_OEvTD;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro046e_TD                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro046e_TD (
	rT0430_stContext       *pContext,
	rT0441_stEntContext    *pEntContext,
	rT04a0_unConn      FAR *pConn,
	rT041G_unPpdu      FAR *pNode,
	unsigned long  int FAR * FAR *pX410RtpmNode,
	signed   long  int FAR * FAR *pbUserData,
	rT0417_stUserData  FAR * FAR *pUserData,
	unsigned       int FAR *pOutEvent,
	unsigned long  int FAR *phNode,
	unsigned long  int FAR *pP1,
	unsigned long  int FAR *pP2,
	unsigned long  int FAR *pP3,
	unsigned long  int FAR *pP4)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro046e_TD")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  if (pContext->bNormalMode)
    /*if pres. PDV node allocated: PS-user data exist (are mandatory) */
    if (pNode){                           /*better "if (*phNode)" P!? */
      *pUserData = (rT0417_stUserData  FAR *)&pNode->stOther.stUserData;
      /* the Boolean for the User data has to be assigned here 
         Since the PDU has the Userdata as mandatory the variable from 
         context block is used here for assigning the boolean variable */
       pContext->bUserData = TRUE;
       *pbUserData = &(pContext->bUserData);
    } 
    else
      RM0003_RETURN (fnctName, signed short int, RC041l_UnrecognizedPpdu)
      /*right value S!? */
      /*P1 not available in X410-mode -> no check possible P!? */
      /*checked by decodRtpmX410() P!?                         */


  *pOutEvent = RC043G_OEvPDTind;

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/


/* EOF */

