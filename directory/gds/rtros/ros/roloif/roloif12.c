/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roloif12.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:46  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:49  root]
 * 
 * Revision 1.1.2.4  1994/08/24  08:35:32  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:35:37  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:11  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:48  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:00  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:04:46  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:33  keutel
 * 	creation
 * 	[1994/03/21  13:33:46  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roloif12.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:46 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1991                     */
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
/* NAME         : roloif1.c                                           */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-18                                            */
/*                                                                    */
/* COMPONENT	: RTROS lower interface	over CMX                      */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V03.00                                        */
/*                                                                    */
/* DESCRIPTION  : This modul contains functions which are calling     */
/*                functions of the underlying transport product       */
/*                (CMX V3.0).                                         */
/*                                                                    */
/*    Only functions of the ROLOIF?.c modules call functions of the   */
/*    transport product.                                              */
/*    This module contains the calls to all the transport functions   */
/*    which are involved in sending & receiving data and establis-    */
/*    hing & releasing connections.                                   */
/*                                                                    */
/* SYSTEM DEPENDENCE : Transport interface of CMX V3.0                */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.00 K00  | 30-10-91| original		       |sss |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/


/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/
 
static char *sccsid =    "@(#) roloif12.c 1.3 94/01/19   RTS-V2.00";


/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <cmx.h>	  /* CMX interface definitions		      */

#include <rogenrl.h>	  /* General definitions		      */
#include <roerval.h>	  /* General definitions (RTS error values)   */
#include <rolgval.h>	  /* General definitions		      */
#include <routil0.h>	  /* General utilities			      */
#include <rostor0.h>	  /* Store interface			      */
#include <rouply0.h>      /* General interface definitions            */
#include <rouply1.h>      /* General interface definitions            */
#include <rodpat0.h>	  /* Dispatcher - PM interface		      */
#include <rotpm00.h>      /* Specific LOIF - SPM interface            */

#include <roloif2.h>	  /* LOIF calling interface		      */
#include <roloif32.h>	  /* LOIF calling interface		      */

#include <roloif42.h>     /* Local include                            */

 
/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/

/* Possible values to call ro0352_processReturnValue function: par 3  */

#define  ST0   0      /* Receiving: all events except .               */
#define  ST1   2      /* Sending  : all events except Connect Request */
#define  ST2   3      /* Sending  : Connect Request event	      */

#define NETB_END   0  /* NetBIOS header byte when T_END  requested    */
#define NETB_MORE  1  /* NetBIOS header byte when T_MORE requested    */

int t_error(void);

/**********************************************************************/
/*                                                                    */
/*              L O C A L  V A R I A B L E S                          */
/*                                                                    */
/**********************************************************************/
 
int                 iTransCode;      /* ret values of the CMX functs. */
int                 iTransConId;     /* Session connection id         */
unsigned long int   hEiCxt;          /* Entity invocation context     */


/**********************************************************************/
/*                                                                    */
/*              G L O B A L  V A R I A B L E S                        */
/*                                                                    */
/**********************************************************************/
static signed short int getHeadLen PROTO ((rT0300_roloifContext *pContext,
                                           signed       int      iTransConId));



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : getHeadLen                                          */
/*                                                                    */
/* AUTHOR       : Shivkumar                                           */
/* DATE         : 93-10-29                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    Determine whether the underlying stack is a NetBIOS.      */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/*    pContext   : A pointer to the lower interface context block.    */
/*    iTransConId: Transport Connection Identifier.                   */
/*                                                                    */
/* OUT-PARAMETERS:                                                    */
/*                                                                    */
/*    None.                                                           */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*		  0 : Successful				      */
/*		< 0 : Error					      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS                :                                           */
/*                                                                    */
/*inoff ***************************************************************/

static signed short int getHeadLen (
rT0300_roloifContext    *pContext,
signed       int         iTransConId)
{

RM0001_STRG (fnctName, "getHeadLen")

signed   short int    sRetCode = OK;



#ifdef RC0010_CMXV4

struct t_opti3        stOpti3;

RM0002_ENTRY (fnctName)

/*--------------------------------------------*/
/* Call t_info() CMX function a second time : */
/* request underlying stack type (indirectly) */
/* A TSDU size of zero => NetBIOS.            */
/*--------------------------------------------*/
stOpti3.t_optnr = T_OPTI3;

/* !!!KC
    ro0043_pack10Log (RC0020_INTERFACE,
                      RL2010_EnTinfo,
                      2,
                      sizeof(int),
                      (char FAR *)&iTransConId,
                      sizeof(t_opti),
                      (char FAR *)&stOpti3);
*/

sRetCode  = t_info ((int *)&iTransConId,         /* -> */
                    (t_opti *)&stOpti3           /* -> */
                   );

/* !!!KC
    ro0043_pack10Log (RC0020_INTERFACE ,
                      RL2011_ReTinfo,
                      2,
                      sizeof(int),
                      (char FAR *)&sRetCode,
                      sizeof(t_opti),
                      (char FAR *)&stOpti3);
*/

if (sRetCode == T_OK) {
    if (stOpti3.t_tsdu) {
        pContext->bNetBIOS = FALSE;
        pContext->cHeadLen = 0;
    }
    else {
        pContext->bNetBIOS = TRUE;
        pContext->cHeadLen = RC0330_NetBIOSHeadLen;
    }
} else {
    iTransCode = sRetCode ;
    sRetCode = t_error ();

    /* !!!KC
        ro0042_writeLog (RC0020_INTERFACE,
                         RL2005_TERROR,
                         sizeof(int),
                         (char FAR *)&iAddErrInfo);
    */
}
#else

RM0002_ENTRY (fnctName)

    pContext->bNetBIOS = FALSE;
    pContext->cHeadLen = 0;
#endif

if (!sRetCode) {
    pContext->hSendData = ro0020_globalalloc ( pContext->ulTiduLength
                                             + pContext->cHeadLen);
    if (!pContext->hSendData) sRetCode = -R_GALLCE;
    if (!sRetCode) {
        pContext->pSendData =(char FAR *)ro0021_globallock(pContext->hSendData);
        if (!pContext->pSendData) sRetCode = -R_GLOCKE;
    }
}

RM0003_RETURN (fnctName, signed short int, sRetCode)

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0352_processReturnValue                           */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-18                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    Process the return value of the CMX functions calls	      */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/*    iRetCode	 : Return code to process			      */
/*                                                                    */
/* OUT-PARAMETERS:                                                    */
/*                                                                    */
/*    pContext	 : Lower interface's context                          */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*		  0 : Successful				      */
/*	       STOP : Stop trying to send			      */
/*		< 0 : Error					      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS                :                                           */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro0352_processReturnValue (

int		       iRetCode,     /* return value to process       */
rT0300_roloifContext  *pContext,     /* lower interface context       */
signed short int       sEventType)   /* type of event		      */

{  
   RM0001_STRG (fnctName, "ro0352_processReturnValue")

   signed   short int	sRetVal;
   unsigned long  int	ulAbortReason;
   int			iAddErrInfo;
   int                  iTrCode;
   struct t_optc2       stOptc2; 

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   switch (iRetCode) {

      case T_OK:
         sRetVal = OK;
         break;

      case T_DATASTOP:
	 pContext->iStopState |= STOP_TRUE;
         sRetVal = STOP;
         break;
       
      case T_ERROR:
      default:

	 iAddErrInfo  = t_error ();
         ro0042_writeLog (RC0020_INTERFACE  ,     
                          RL2005_TERROR,          
                          sizeof(int),            
                          (char FAR *)&iAddErrInfo);
	 if (sEventType == ST0) {   /* Receiving all events */
            if (!(iAddErrInfo & CMXMASK)) {
               /* by kc on 10-12-93 for T_COLLISION *****
	       sRetVal = -R_ICMXER;     ****** */
               ulAbortReason=NULL;
               switch(iAddErrInfo){
                 case T_COLLISION:
                   ulAbortReason=NULL;
                   sRetVal = OK;
                      break;
                 default:
	           ulAbortReason = RC0303_CMXError;
                   break;
               }
              
            } else {
               /* by kc on 10-12-93 ******
	       sRetVal = -R_ISYSOS;  ***** */
	       ulAbortReason = RC0304_SysError;
	    }
        /*  break; */
            if(ulAbortReason)
	    sRetVal = ro0211_pushOutEvent (RC0214_Session,
	                                   RC0906_TDISCONNECTind,
				           0L,
					   iAddErrInfo,
					   ulAbortReason,
					   0L);
           
	 } else {     /* Sending */

            if (!(iAddErrInfo & CMXMASK)) {
	       ulAbortReason = RC0303_CMXError;
            } else {
	       ulAbortReason = RC0304_SysError;
	    }

	    if (iAddErrInfo < 0) { /* when is this possible */
	       iAddErrInfo  = -iAddErrInfo;
	       ulAbortReason  |=  0x00010000;
	    }

	    sRetVal = ro0211_pushOutEvent (RC0214_Session,
	                                   RC0906_TDISCONNECTind,
				           0L,
					   iAddErrInfo,
					   ulAbortReason,
					   0L);
	    if (sEventType == ST1) {     /* Send all events except
                                                 Connect Request */
	    /*------------------------------------*/
            /* Fill up values for struct t_optc2  */
            /* and send a disconnect request.     */
	    /*------------------------------------*/
               stOptc2.t_optnr = T_OPTC2;
               stOptc2.t_udatal = 0;
               stOptc2.t_udatap = NULL;
            /*------------------------------*/ 
            /* Call t_disrq() CMX function: */
            /* Transport release request    */
            /*------------------------------*/
               ro0043_pack10Log(RC0020_INTERFACE ,
                                RL2008_EnTdisrq,
                                2,
                                sizeof(int),
                                (char FAR *)&iTransConId,
                                sizeof(struct t_optc2),
                                (char FAR *)&stOptc2);

               iTrCode = t_disrq ((int *)&iTransConId,      /* -> */
                                      (t_opt2 *)&stOptc2       /* -> */
                                    );            
               ro0042_writeLog (RC0020_INTERFACE  ,     
                                RL2009_ReTdisrq,
                                sizeof(int),
                                (char FAR *)&iTrCode);

	       switch (iTrCode) {

	          case T_OK:
		     sRetVal = DESTRUCT;
		     break;

	          case T_ERROR:
	          default:

		     iAddErrInfo  = t_error ();
                     ro0042_writeLog (RC0020_INTERFACE  ,     
                          RL2005_TERROR,          
                          sizeof(int),            
                          (char FAR *)&iAddErrInfo);
                     if (!(iAddErrInfo & CMXMASK)) {
	                sRetVal = -R_ICMXER;
                     } else {
		        sRetVal = -R_ISYSOS;
		     }
                     break;
               }                    
	   }     
           if (sRetVal == OK)
              sRetVal = DESTRUCT;
	 }
	 break;
   }

   RM0003_RETURN (fnctName, signed short int, sRetVal)
}  
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0310_SendReceive                                  */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-18                                            */
/*                                                                    */
/* DESCRIPTION	:						      */
/*								      */
/*    Process all the requests and responses for sending, calling the */
/*    function ro0350_Send, and all the indications and confirmations */
/*    for receiving, calling the function ro0351_Receive.	      */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/*    hContext	 : Memory handle of the lower interface's context     */
/*                                                                    */
/* OUT-PARAMETERS: None 					      */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*                0 : Successful                                      */
/*		< 0 : Error					      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS                :                                           */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro0310_SendReceive (
unsigned long int   hContext)

{

   RM0001_STRG (fnctName, "ro0310_SendReceive")

   rT0300_roloifContext  *pContext;        /* lower interface context */
   signed   long int	  lInEvent;	   /* incoming event	      */
   signed   long int	  lOutEvent;	   /* outgoing event	      */
   unsigned long int      ulP1;            /* event parameter P1      */
   unsigned long int      ulP2;            /* event parameter P2      */
   unsigned long int      ulP3;            /* event parameter P3      */
   unsigned long int      ulP4;            /* event parameter P4      */

   unsigned long int      hAcb;            /* ACB handle              */
   unsigned long int      hEcb;            /* ECB handle              */

   unsigned long int      hAuxSend;        /* work variable           */
   rT0301_sendQueue      *pAuxSend;        /* work variable           */
   unsigned long int      hNewSend;        /* work variable           */
   rT0301_sendQueue      *pNewSend;        /* work variable           */
   signed  short int      sReturnCode;     /* Function's return value */
   signed  short int	  sRc ;

   unsigned long int	  hOutP2;
   unsigned long int	  hOutP3;
   unsigned long int	  hOutP4;
   unsigned long int	  hRecStore;	   /* Store handle  */

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   sReturnCode = OK;
   hRecStore   = NULL;
   hOutP3      = NULL;

   /*-----------------------------------------------------*/
   /* If a Context for the Transport already exists, then   */
   /* lock it, but if it does not, then allocate memory   */
   /* for a new one, lock and register the pm invocation. */
   /*-----------------------------------------------------*/

   if (hContext) {

      if ((pContext = (rT0300_roloifContext *)ro0031_locallock (hContext)) 
          == NULL)
	 sReturnCode = -R_LLOCKE;

   } else {

      if ((hContext = ro0030_localalloc (sizeof(rT0300_roloifContext))) == 0) {
	 sReturnCode = -R_LALLCE;

      } else if ((pContext =(rT0300_roloifContext *)ro0031_locallock (hContext))
                                                                      == NULL) {
         if (ro0034_localfree (hContext) != 0)
	    sReturnCode = -R_LFREEE;
	 sReturnCode = -R_LLOCKE;

      } else {
	 pContext->hRecStore = NULL;
	 sReturnCode = ro0212_regPmInvoc (hContext);
      }
   }

   if (sReturnCode != OK) {
      RM0003_RETURN (fnctName, signed short int, sReturnCode)
   }

   /*-------------------------------------*/
   /* Get one event from the Events stack */
   /*-------------------------------------*/

   lInEvent = ro0210_popInEvent (&ulP1, &ulP2, &ulP3, &ulP4);

   /*-------------------------------------------------------*/
   /* With the obtained event value, we can know if it is a */
   /* Send or a Receive event (lInEvent != 0 => Send event, */
   /* lInEvent == 0 => Receive event).			    */
   /*-------------------------------------------------------*/

    hAcb = ro0214_getAcbEcb (&hEcb);

    ro0043_pack10Log (
      RC0022_ACTIVITY,
      RL0301_ActivityLoif,
      8,
      (signed short int) sizeof(hEcb),
      (char FAR *)       &hEcb,
      (signed short int) sizeof(hAcb),
      (char FAR *)       &hAcb,
      (signed short int) sizeof(ulP1),
      (char FAR *)       &ulP1,
      (signed short int) sizeof(ulP2),
      (char FAR *)       &ulP2,
      (signed short int) sizeof(ulP3),
      (char FAR *)       &ulP3,
      (signed short int) sizeof(ulP4),
      (char FAR *)       &ulP4,
      (signed short int) sizeof(lInEvent),
      (char FAR *)       &lInEvent,
      (signed short int) sizeof(rT0300_roloifContext),
      (char FAR *)       pContext);

   if (lInEvent == 0) {

      /*-----------------------------------------------------------------*/ 
      /* RECEIVE (Transport event and/or Transport user data)                */
      /*-----------------------------------------------------------------*/

      if (ulP1 == T_DATAGO) {

	 pContext->iStopState = NO_STOP;

         if (pContext->hFirstSendQu == NULL) {

	    /*----------------------------------------------------*/
	    /* If the receiver has sent a T_DATAGO and the Send	  */
	    /* queue is empty: push a TCONTINUE event for the SPM */
	    /*----------------------------------------------------*/

	    sReturnCode = ro0211_pushOutEvent (RC0214_Session,   
					       RC0909_TCONTINUE,
					       0L,
					       0L,
					       0L,
					       0L);

         } else {

            /*-------------------------------------------------------*/
            /* If the Send queue is not empty, the T_DATAGO ends the */
            /* Stop condition and it is possible to send again       */
            /*-------------------------------------------------------*/

	    sReturnCode = ro0350_Send (pContext);

	    if (sReturnCode == STOP)
	       sReturnCode = OK;
         }

      } else {     /* ulP1 != T_DATAGO */

         /*----------------------------*/
         /* CALL THE RECEIVE FUNCTION  */
	 /*----------------------------*/
	 sReturnCode = ro0351_Receive ((rT0300_roloifContext *)pContext,
				       (signed long int *)&lOutEvent,
				       (unsigned long int)ulP1,
				       (unsigned long int)ulP2,
				       (unsigned long int)ulP3,
				       (unsigned long int *)&hRecStore,
				       (unsigned long int *)&hOutP2,
				       (unsigned long int *)&hOutP3,
				       (unsigned long int *)&hOutP4);

	 if (sReturnCode >= 0) {
	    if (lOutEvent != 0L) {
               if ((sRc  = ro0211_pushOutEvent (RC0214_Session,
						 lOutEvent,
						 hRecStore,
						 hOutP2,
						 hOutP3,
						 hOutP4)) != OK)
	          RM0003_RETURN (fnctName, signed short int, sRc )
             }
	 } else {

	 /*---------------------------*/
	 /* Free the allocated memory */
	 /*---------------------------*/

	     if (hRecStore != NULL) { /* error during T_DATAIN */
	        if ((sRc = ro0075_deletestore (hRecStore)) != 0)
	           RM0003_RETURN (fnctName, signed short int, -R_STOERR)
		hRecStore = NULL;
             }

	     if ((hOutP3 != NULL) && (lOutEvent == RC0906_TDISCONNECTind)) { 
                          /* error during T_CONNECTind or T_REDIRECTind */
	        if (ro0034_localfree (hOutP3) != 0)
	           RM0003_RETURN (fnctName, signed short int, -R_LFREEE)
		     hOutP3 = NULL;
             }
	  }
      }
                    
   } else {   /* lInEvent != 0 */

      /*-----------------------------------------------------------------*/ 
      /* SEND (Transport event and/or Transport user data)               */
      /*-----------------------------------------------------------------*/

      /*---------------------------*/
      /* Create a new Send element */
      /*---------------------------*/

      if ((hNewSend = ro0030_localalloc (sizeof(rT0301_sendQueue))) == 0) { 
         RM0003_RETURN (fnctName, signed short int, -R_LALLCE)
      }

      if ((pNewSend = (rT0301_sendQueue *)ro0031_locallock (hNewSend)) 
          == NULL) {
         if (ro0034_localfree (hNewSend) != 0) {
            RM0003_RETURN (fnctName, signed short int, -R_LFREEE)
         }
         RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)
      }

      if (ulP1 == NULL) { 

         /*-------------------------------*/
         /* There is no user data to send */
         /*-------------------------------*/

         pNewSend->hSendStore    = NULL;
         pNewSend->ulDataLen = 0L;

      } else {    

         /*------------------------------*/
         /* Get the user data from store */
         /*------------------------------*/

	 if ((sReturnCode = ro0074_storeactive (ulP1, ulP2)) != 0) /* offset = ulP2 */
	    RM0003_RETURN (fnctName, signed short int, -R_STOERR)

         pNewSend->hSendStore    = ulP1;
         pNewSend->ulDataLen = ro0062_storelength (ulP1) - ulP2;
      }

      pNewSend->iSendEvent = (int)lInEvent;
      pNewSend->ulP2	   = ulP2;
      pNewSend->ulP3	   = ulP3;
      pNewSend->ulP4	   = ulP4;
      pNewSend->hNextElem  = NULL;

      if (ro0032_localunlock (hNewSend) != 0) {
         RM0003_RETURN (fnctName, signed short int, -R_LULOKE)
      }

      if ((pContext->iStopState != NO_STOP) &&
	  (lInEvent != RC0905_TDISCONNECTreq)) {

         /*---------------------------------------------------*/
         /* If it is a Send event, but the Stop state is set  */
         /* and it is not an Disconnect event: add the Send   */
         /* request or response to the Send queue and return  */         
         /*---------------------------------------------------*/

         if (pContext->hLastSendQu != NULL) {

            if ((pAuxSend = (rT0301_sendQueue *)ro0031_locallock (pContext->hLastSendQu)) == NULL) {
               if (ro0034_localfree (hNewSend) != 0) {
                  RM0003_RETURN (fnctName, signed short int, -R_LFREEE)
	       }
               RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)
            }

            pAuxSend->hNextElem = hNewSend;

            if (ro0032_localunlock (pContext->hLastSendQu) != 0) {
               if (ro0034_localfree (hNewSend) != 0) {
                  RM0003_RETURN (fnctName, signed short int, -R_LFREEE)
               }
               RM0003_RETURN (fnctName, signed short int, -R_LULOKE)
            }

            pContext->hLastSendQu = hNewSend;

         } else {     /* the Send queue is empty */

	    pContext->hFirstSendQu = pContext->hLastSendQu = hNewSend;

         }

      } else {
	 if (lInEvent == RC0905_TDISCONNECTreq) {

	    /*--------------------------------------------------------*/
	    /* If it is an Disconnect event, delete all the Send list */
	    /* and pass the Disconnect event to the Send function     */
	    /*--------------------------------------------------------*/
         
	    hAuxSend = pContext->hFirstSendQu;

	    while (hAuxSend != NULL) {

	       if ((pAuxSend = (rT0301_sendQueue *)ro0031_locallock (hAuxSend))
		   == NULL)
		  RM0003_RETURN (fnctName, signed short int, -R_LULOKE)

	       pContext->hFirstSendQu = pAuxSend->hNextElem;

	       if (pAuxSend->hSendStore != NULL)
		  if (ro0075_deletestore (pAuxSend->hSendStore) != 0)
		     RM0003_RETURN (fnctName, signed short int, -R_STOERR)

	       if (pAuxSend->ulP3 != NULL)
		  if (ro0034_localfree (pAuxSend->ulP3) != 0)
		     RM0003_RETURN (fnctName, signed short int, -R_LFREEE)

	       if (ro0032_localunlock (hAuxSend) != 0)
		  RM0003_RETURN (fnctName, signed short int, -R_LULOKE)

	       if (ro0034_localfree (hAuxSend) != 0)
		  RM0003_RETURN (fnctName, signed short int, -R_LFREEE)

	       hAuxSend = pContext->hFirstSendQu;
	    }
	 }

         /*-------------------------------------------*/
         /* Add the new Send element to the Send      */
         /* queue (which is made empty in this case)  */
	 /*-------------------------------------------*/

         pContext->hFirstSendQu = pContext->hLastSendQu = hNewSend;

         /*-------------------------*/
         /* CALL THE SEND FUNCTION  */
	 /*-------------------------*/
	 sReturnCode = ro0350_Send (pContext);

	 if (sReturnCode == STOP)
	    sReturnCode = OK;
      }
   }

   if (ro0032_localunlock (hContext) != 0) {
      sReturnCode = -R_LULOKE;
   }

   if (sReturnCode == DESTRUCT) {
      ro0213_regPmDestr();
      sReturnCode = ro0399_cleanUp (0L, hContext, 0L, 0L, 0L);
   }
 
   RM0003_RETURN (fnctName, signed short int, sReturnCode)
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0350_Send                                         */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-18                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    Process the Send requests and responses.			      */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/*    pContext	 : Lower interface's context                          */
/*    ulP1	 : Not used					      */
/*    ulP2	 : Event parameter P2				      */
/*    ulP3	 : Event parameter P3				      */
/*    ulP4	 : Event parameter P4				      */
/*                                                                    */
/* OUT-PARAMETERS: None 					      */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*		  0 : Successful				      */
/*	   DESTRUCT : PM must be destruct			      */
/*		< 0 : Error					      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS                :                                           */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro0350_Send (

rT0300_roloifContext  *pContext)           /* lower interface context */

{
   RM0001_STRG (fnctName, "ro0350_Send")

   char                *pTransAddr;     /* ptr containing the Transport add */
   char 	       *pAddr;	     /* Partner Transport address without PSEL                                       & SSEL   */
   char 	       *pMyAddr;     /* Self Transport address without PSEL &                                        SSEL   */
   unsigned long int    hMyAddr;     /*  Self Transport address handle  */

   rT0301_sendQueue        *pAuxSend;
   unsigned long  int       hAuxSend;
   int                      iAddErrInfo ;
   int			    iAuxInt;
   char                     vFileName[MAXFILELEN];
   long 		    lStoreLen;

   /* CMX structures  */

   struct t_optc1          stOptc1;
   struct t_opti1          stOpti1;
   struct t_optc2          stOptc2;

   /*** used in TDATAreq message ***/

   signed   short int   bSendingData;
   signed   short int   bLastSidu;
   unsigned long  int   ulDataLen;
   unsigned long  int   ulPid;
   int  		iChain; 	 /* Further TDATAreq calls or not   */

   signed   short int   sReturnCode;     /* Function's return value        */
   signed   short int	sRc ;

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   sReturnCode = OK;
   pTransAddr = NULL;
   hAuxSend    = pContext->hFirstSendQu;
   bSendingData = FALSE;

   /*----------------------------------------------------*/
   /* SEND (Transport event and/or Transport user data): */
   /* Send all the events in the Send queue until it     */
   /* is empty or a T_DATASTOP is returned from CMX      */
   /*----------------------------------------------------*/

   while ((hAuxSend != NULL) && (sReturnCode == OK)) {

      /*-----------------------*/
      /* Lock the Send element */
      /*-----------------------*/

      if ((pAuxSend = (rT0301_sendQueue *)ro0031_locallock (hAuxSend)) 
	  == NULL) {
         RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)
      }
            
      iTransConId = pContext->iTransConId;

   /* Switch starts */

      switch (pAuxSend->iSendEvent) {

         case RC0901_TCONNECTreq:

            /* Returns handle of entity invocation context */
	    if ((sReturnCode = ro0215_getEiCxt (RC0230_TransportEiCxt,
			       (unsigned long int FAR *)&hEiCxt)) < 0) {
               break;
	    }

            if ((pTransAddr = ro0031_locallock (pAuxSend->ulP3)) 
                == NULL) {
	       sReturnCode = -R_LLOCKE;
               break;
            }

	    /*---------------------------------------*/
	    /* Take out the P & S Selectors from the */
	    /* the PSAP address, if present.         */
	    /*---------------------------------------*/

	    ro0354_getAddressWithoutPandSsel (pTransAddr, &pAddr);

	    /*------------------------------------*/
            /* Fill up values for struct t_opc1   */
	    /*------------------------------------*/

            stOptc1.t_optnr = T_OPTC1;
            stOptc1.t_udatal = 0;
            stOptc1.t_udatap = NULL;
            stOptc1.t_xdata = T_NO;
            stOptc1.t_timeout = T_NO;


	 /*-----------------------------------------------------*/
	 /* Allocate memory for the Transport address of        */
         /* your own application	                        */
	 /*-----------------------------------------------------*/

	    if ((hMyAddr = ro0030_localalloc (RCro1d_MaxLenTSAP)) == 0) {
	       sReturnCode = -R_LALLCE;
	       break;
	    }

	    if ((pMyAddr = ro0031_locallock (hMyAddr)) == NULL) {
	       sReturnCode = -R_LLOCKE;
	       break;
            }

            ro0217_getAddress (NULL, RCro14_TransportAddress,
                       RCro1d_MaxLenTSAP, pMyAddr);

            /*------------------------------*/ 
            /* Call t_conrq() CMX function: */
            /* Transport connect request    */
            /*------------------------------*/

            ro0043_pack10Log(RC0020_INTERFACE ,
                             RL200a_EnTconrq,
                             3,
                             sizeof(union t_address),
                             (char FAR *)pAddr,
                             sizeof(union t_address),
                             (char FAR *)pMyAddr,
                             sizeof(t_opt13),
                             (char FAR *)&stOptc1);

            iTransCode = t_conrq ((int *)&iTransConId,              /* <- */
			          (union t_address *)pAddr,         /* -> */
				  (union t_address *)pMyAddr,       /* -> */
                                  (t_opt13 *)&stOptc1               /* -> */
                                 );
            ro0043_pack10Log(RC0020_INTERFACE ,
                             RL200b_ReTconrq,
                             3,
                             sizeof(int),
                             (char FAR *)&iTransCode,
                             sizeof(int),
                             (char FAR *)&iTransConId,
                             sizeof(t_opt13),
                             (char FAR *)&stOptc1);

            
	    sReturnCode = ro0352_processReturnValue (iTransCode, pContext, ST2);

	    if (iTransCode == T_OK) {

	       /*------------------------------------------------*/
	       /* Save the local Transport connection identifier */
	       /*------------------------------------------------*/
	       ro0229_setLoifId ((signed long int)iTransConId);

	       pContext->iTransConId = iTransConId;
	    }
	    /*-------------------------------------------------*/
	    /* Free the memory associated with the address     */
	    /* parameters.This memory is now no longer needed. */
	    /*-------------------------------------------------*/

	    if (pAuxSend->ulP3 != NULL) {
	       if (ro0032_localunlock (pAuxSend->ulP3) != 0)
	          sReturnCode = -R_LULOKE;
	       if (ro0034_localfree (pAuxSend->ulP3) != 0) {
		  sReturnCode = -R_LFREEE;
               } else {
                  pAuxSend->ulP3 = NULL;
               }
	    }
	    if (hMyAddr != NULL) {
	       if (ro0032_localunlock (hMyAddr) != 0)
	          sReturnCode = -R_LULOKE;
	       if (ro0034_localfree (hMyAddr) != 0) {
		  sReturnCode = -R_LFREEE;
               } else {
                  hMyAddr = NULL;
               }
	    }
            break;

         case RC090a_TREDIRECTreq:

            ulPid = pAuxSend->ulP4;

            if ((lStoreLen = ro0062_storelength (pAuxSend->hSendStore)) < 0) { 
               sReturnCode = -R_STOERR;                                      
               break;                                                        
            }                                                                

            if (ro0066_storepos ( pAuxSend->hSendStore,                      
                                  lStoreLen ) != OK) {        
               sReturnCode = -R_STOERR;                                      
               break;                                                        
            }                                                                

            if (ro0068_stputm (pAuxSend->hSendStore,            
                               (char FAR *)&(pAuxSend->ulP2),                   
                               (long)(sizeof(unsigned long int))) != OK) {
	       sReturnCode = -R_STOERR;
	       break;
            }
            if (ro0087_StoreToFile (pAuxSend->hSendStore,
                                    (char FAR *)vFileName) != OK) {
	       sReturnCode = -R_STOERR;
	       break;
            }
	    /*------------------------------------*/
            /* Fill up values for struct t_opc1   */
	    /*------------------------------------*/
            stOptc1.t_udatal = MAXFILELEN;

            stOptc1.t_udatap = vFileName;
          
            stOptc1.t_optnr = T_OPTC1;
            stOptc1.t_xdata = T_NO;
            stOptc1.t_timeout = T_NO;

            /*------------------------------*/ 
            /* Call t_redrq() CMX function: */
            /* Transport redirect request   */
            /*------------------------------*/

            ro0043_pack10Log(RC0020_INTERFACE ,
                             RL200c_EnTredrq,
                             3,
                             sizeof(int),
                             (char FAR *)&iTransConId,
                             sizeof(int),
                             (char FAR *)&ulPid,
                             sizeof(t_opt13),
                             (char FAR *)&stOptc1);
            iTransCode = t_redrq ((int *)&iTransConId,            /* -> */
                                  (int *)&ulPid,                  /* -> */
                                  (t_opt12 *)&stOptc1             /* -> */
                                 );

            ro0042_writeLog (RC0020_INTERFACE  ,     
                             RL200d_ReTredrq,
                             sizeof(int),
                             (char FAR *)&iTransCode);

	   /*  sReturnCode = ro0352_processReturnValue (iTransCode, pContext, ST1); *** removed on 30/06/92 - sss temporary *** */

            sReturnCode = DESTRUCT;  /* sss - temporary *** */
            break;


         case RC0903_TCONNECTrsp:

            /*---------------------------------------------*/
            /* Returns handle of entity invocation context */
            /*---------------------------------------------*/

	    if ((sReturnCode = ro0215_getEiCxt (RC0230_TransportEiCxt,
			       (unsigned long int FAR *)&hEiCxt)) != 0) {
               sReturnCode = -R_JNEICX; 
               break;
            }

	    /*------------------------------------*/
            /* Fill up values for struct t_opc1   */
	    /*------------------------------------*/

            stOptc1.t_optnr = T_OPTC1;
            stOptc1.t_udatal = 0;
            stOptc1.t_udatap = NULL;
            stOptc1.t_xdata = T_NO;
            stOptc1.t_timeout = T_NO;

            /*------------------------------*/ 
            /* Call t_conrs() CMX function: */
            /* Transport connect response   */
            /*------------------------------*/

            ro0043_pack10Log(RC0020_INTERFACE ,
                             RL200e_EnTconrs,
                             2,
                             sizeof(int),
                             (char FAR *)&iTransConId,
                             sizeof(t_opt13),
                             (char FAR *)&stOptc1);
            iTransCode = t_conrs ((int *)&iTransConId,            /* -> */
                                  (t_opt13 *)&stOptc1             /* -> */
                                 );

            ro0042_writeLog (RC0020_INTERFACE  ,     
                             RL200f_ReTconrs,
                             sizeof(int),
                             (char FAR *)&iTransCode);
	    sReturnCode = ro0352_processReturnValue (iTransCode, pContext, ST1);
          

            if (iTransCode != T_ERROR) {

               /*-----------------------------*/
               /* Call t_info() CMX function: */
               /* request Transport information */
               /*-----------------------------*/
               stOpti1.t_optnr = T_OPTI1;

               ro0043_pack10Log(RC0020_INTERFACE ,
                                RL2010_EnTinfo,
                                2,
                                sizeof(int),
                                (char FAR *)&iTransConId,
                                sizeof(t_opti),
                                (char FAR *)&stOpti1);

               iAuxInt  = t_info ((int *)&iTransConId,         /* -> */
                                  (t_opti *)&stOpti1           /* -> */
                                      );

               ro0043_pack10Log(RC0020_INTERFACE ,
                                RL2011_ReTinfo,
                                2,
                                sizeof(int),
                                (char FAR *)&iAuxInt,
                                sizeof(t_opti),
                                (char FAR *)&stOpti1);

               if (iAuxInt  == T_OK) {
                   pContext->ulTiduLength = (unsigned long int)stOpti1.t_maxl;
                   sReturnCode = getHeadLen (pContext, iTransConId);
               } else {
                   iTransCode = iAuxInt ;
                   sReturnCode = t_error ();
                   ro0042_writeLog (RC0020_INTERFACE  ,
                                    RL2005_TERROR,
                                    sizeof(int),
                                    (char FAR *)&iAddErrInfo);
               }
            }
            break;

         case RC0905_TDISCONNECTreq:
      
            pAuxSend->ulP3 = NULL;           /* not used in this call  */

	    /*------------------------------------*/
            /* Fill up values for struct t_optc2  */
	    /*------------------------------------*/

            stOptc2.t_optnr = T_OPTC2;
            stOptc2.t_udatal = 0;
            stOptc2.t_udatap = NULL;

            /*------------------------------*/ 
            /* Call t_disrq() CMX function: */
            /* Transport release request      */
            /*------------------------------*/

               ro0043_pack10Log(RC0020_INTERFACE ,
                                RL2008_EnTdisrq,
                                2,
                                sizeof(int),
                                (char FAR *)&iTransConId,
                                sizeof(struct t_optc2),
                                (char FAR *)&stOptc2);
            iTransCode = t_disrq ((int *)&iTransConId,           /* -> */
                                   (t_opt2 *)&stOptc2            /* -> */
                                 );            

               ro0042_writeLog (RC0020_INTERFACE  ,     
                                RL2009_ReTdisrq,
                                sizeof(int),
                                (char FAR *)&iTransCode);

	    switch (iTransCode) {

	       case T_OK:
		  sReturnCode = DESTRUCT;
		  break;

	       case T_ERROR:
	       default:

		  iAddErrInfo  = t_error ();
                  ro0042_writeLog (RC0020_INTERFACE  ,     
                          RL2005_TERROR,          
                          sizeof(int),            
                          (char FAR *)&iAddErrInfo);
                  if (!(iAddErrInfo & CMXMASK)) {
	             sReturnCode = -R_ICMXER;
                  } else {
		     sReturnCode = -R_ISYSOS;
		  }
                  break;
             }
             break;
        
        
         case RC0907_TDATAreq:

	    /*----------------------------------------------------*/
	    /* If a STOP condition is received, this element must */
	    /* not be deleted from the Send queue but it has to   */
	    /* be kept until a new T_DATAGO condition is received.*/
	    /*----------------------------------------------------*/

            bSendingData = TRUE;
        
            ulDataLen = pContext->ulSendLen = pAuxSend->ulDataLen;
	   /* ***  if ((iAuxInt = ro0066_storepos (pAuxSend->hSendStore, 
                                         pAuxSend->ulP2)) < 0) 
	    RM0003_RETURN (fnctName, signed short int, -R_STOERR)
                done before in send receive      ***  */

            while (ulDataLen > 0) {
        
               if ((ulDataLen+pContext->cHeadLen) >= pContext->ulTiduLength) {
		   pContext->ulSendLen   = pContext->ulTiduLength;
                   ulDataLen            -= (pContext->ulTiduLength
                                         -  pContext->cHeadLen);
               } else {
		   pContext->ulSendLen   = ulDataLen + pContext->cHeadLen;
                   ulDataLen             = 0;
               }
        
               bLastSidu = (ulDataLen == 0) ? TRUE
                                            : FALSE;

               if (pContext->bNetBIOS) {
                   /*-------------------------------------------------------*/
                   /* Note: In NetBios the concept of TSDU is not supported */
                   /*       and therefore always T_END must be sent.        */
                   /*       If T_MORE is sent, then a T_DISIN is received.  */
                   /*       Therefore LOIF has to add this feature to CMX   */
                   /*       locally.                                        */
                   /*-------------------------------------------------------*/

                   pContext->pSendData [0]    = (int)(bLastSidu)
                                              ? NETB_END : NETB_MORE;
                   iChain                     = T_END;
               }
               else {
                   iChain = (int)((bLastSidu) ? T_END
                                              : T_MORE);
               }


	       if (ro0071_stgetm (pAuxSend->hSendStore,
                                 (pContext->pSendData + pContext->cHeadLen),
                                 (pContext->ulSendLen - pContext->cHeadLen))
                            < NULL)
		   RM0003_RETURN (fnctName, signed short int, -R_STOERR)
        
               /*-------------------------------*/ 
               /* Call t_datarq() CMX function: */
               /* normal data request           */
               /*-------------------------------*/
        
               ro0043_pack10Log(RC0020_INTERFACE ,
                                RL2012_EnTdatarq,
                                4,
                                sizeof(int),
                                (char FAR *)&iTransConId,
                                sizeof(int),
                                (char FAR *)&pContext->ulSendLen,
                                sizeof(int),
                                (char FAR *)&iChain,
                                (signed short int) pContext->ulSendLen,
                                (char FAR *)pContext->pSendData);

               iTransCode = t_datarq ((int  *)&iTransConId,            /* -> */
                                      (char *)pContext->pSendData,     /* -> */
                                      (int  *)&pContext->ulSendLen,    /* -> */
                                      (int  *)&iChain);                /* -> */

               ro0042_writeLog (RC0020_INTERFACE,
                                RL2013_ReTdatarq,
                                sizeof(int),
                                (char FAR *)&iTransCode);

	       sReturnCode = ro0352_processReturnValue (iTransCode, pContext,
							ST1);

	       if (ulDataLen == 0L) {
                  /*------------------------------*/
                  /* All user data already sent   */
                  /*------------------------------*/

		  bSendingData = FALSE;
	       }
                 /*--------------------------------*/
                 /* if data is not completely sent */
                 /*  save the current data length  */
                 /*--------------------------------*/
	       if (sReturnCode != OK) {	/* iTransCode == T_DATASTOP */
		  pAuxSend->ulDataLen = ulDataLen;  /* data left	*/
                  break;
               } else if (iTransCode != T_OK) { 
		  break;   /* T_ERROR ==> TDISCONNECT sent */
               }
            }
            break;
         
         default:
         
            /*-------------------------------*/ 
            /* Unknown local Transport event */
            /*-------------------------------*/
         
            iTransCode = T_OK;
	    sReturnCode = -R_LUNKEV;
	    break;
         
      } /* Switch  ends */

      if (pAuxSend->ulP3 != NULL)
	 if (ro0032_localunlock (pAuxSend->ulP3) != 0)
	    sReturnCode = -R_LULOKE;  

      if ((sReturnCode < 0) ||
	  ((iTransCode != T_OK) && (iTransCode != T_DATASTOP))) {

	 /* It is an error or a TDISCONTINUE has been sent */

	 /* !! sRc  is only used for debugging, to be able to know the
	    return code of the called functions. The error which
	    is returned is the one comming from the CMX calls !! */

	 if (pAuxSend->hSendStore != NULL) {
	    if ((sRc  = ro0075_deletestore (pAuxSend->hSendStore)) == 0) {
	       pAuxSend->hSendStore = NULL;
            } else {
	       RM0003_RETURN (fnctName, signed short int, -R_STOERR)
            }
         }
	 if (pAuxSend->ulP3 != NULL) {
	    if((sRc=(signed short int)ro0034_localfree (pAuxSend->ulP3)) == 0) {
	    pAuxSend->ulP3 = NULL;
            } else {
	       RM0003_RETURN (fnctName, signed short int, -R_LFREEE)
            }
	 } 

	 sRc  = (signed short int)ro0032_localunlock (hAuxSend);
         if (sRc != 0)
	    sReturnCode = -R_LULOKE;  
         RM0003_RETURN (fnctName, signed short int, sReturnCode)
      }

      if ((bSendingData == TRUE) && (pContext->iStopState != NO_STOP)) {

	 if (ro0032_localunlock (hAuxSend) != 0)
	    sReturnCode = -R_LULOKE;
	 hAuxSend = NULL;

      } else {

	 pContext->hFirstSendQu = pAuxSend->hNextElem;
	 if (pAuxSend->hNextElem == NULL)
	    pContext->hLastSendQu = NULL;

	 if (pAuxSend->hSendStore != NULL) {
	    if (ro0075_deletestore (pAuxSend->hSendStore) != 0)
	       RM0003_RETURN (fnctName, signed short int, -R_STOERR)
	    pAuxSend->hSendStore = NULL;
	 }

	 if (pAuxSend->ulP3 != NULL)
	    if (ro0034_localfree (pAuxSend->ulP3) != 0)
	       sReturnCode = -R_LFREEE; 

	 if (ro0032_localunlock (hAuxSend) != 0)
	    sReturnCode = -R_LULOKE;

	 if (ro0034_localfree (hAuxSend) != 0)
	    sReturnCode = -R_LFREEE;

	 if (pContext->iStopState != NO_STOP)
            hAuxSend = NULL;
         else 
            hAuxSend = pContext->hFirstSendQu;
      }
   } /* while (hAuxSend) */

   if ((sReturnCode == OK) && 
       (hAuxSend == NULL) &&
       (pContext->iStopState == NO_STOP)) {

      /*---------------------------------------------*/
      /* If the queue is empty and no Stop condition */
      /* is set: push a TCONTINUE event for the SPM  */
      /*---------------------------------------------*/

      sReturnCode = ro0211_pushOutEvent (RC0214_Session, RC0909_TCONTINUE,
					 0L, 0L, 0L, 0L);
   }

   RM0003_RETURN (fnctName, signed short int, sReturnCode)
}        
        
         
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0351_Receive                                      */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-18                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    Process the Receive indications and confirmations.	      */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/*    pContext	 : Lower interface's context                          */
/*    ulInP1	 : CMX event (from t_event call)		      */
/*    ulInP2	 : User data length (from t_event call) 	      */
/*    ulInP3	 : Transport connection identifier (from t_event call)*/
/*    pulP1	 : Event parameter P1: store handle		      */
/*                                                                    */
/* OUT-PARAMETERS:                                                    */
/*                                                                    */
/*    plOutEvent : Outgoing event				      */
/*    pulP1	 : Event parameter P1: store handle		      */
/*    pulP2	 : Event parameter P2				      */
/*    pulP3	 : Event parameter P3				      */
/*    pulP4	 : Event parameter P4				      */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*		  0 : Successful				      */
/*	   DESTRUCT : PM must be destruct			      */
/*		< 0 : Error					      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS                :                                           */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro0351_Receive (

rT0300_roloifContext  *pContext,	   /* lower interface context */
signed	 long int     *plOutEvent,	   /* outgoing event	      */
unsigned long int      ulInP1,		   /* CMX event 	      */
unsigned long int      ulInP2,		   /* User data length	      */
unsigned long int      ulInP3,		   /* transport connection id */
unsigned long int     *pulP1,		   /* event parameter P1      */
unsigned long int     *pulP2,		   /* event parameter P2      */
unsigned long int     *pulP3,		   /* event parameter P3      */
unsigned long int     *pulP4)		   /* event parameter P4      */

{
   RM0001_STRG (fnctName, "ro0351_Receive")

   unsigned long int	    hPartAddr;	 /* PSAP address handle  */
   char     FAR            *pPartAddr;	 /* Pt. to partner Transport address */
   unsigned long int	    hMyAddr;	 /* PSAP address handle  */
   char     FAR		   *pMyAddr;	 /* Pt. to partner Transport address */
   int			    ulAbortReason;	 /* Abort reason	    */
   signed short int	    sReturnCode; /* Function's return value         */
   long 		    lBufLen;
   unsigned short int	    ulCMXEvent;
   int  		    iDataChain;
   long 		    lStoreLen;
   char                     vFileName[MAXFILELEN];
   unsigned long int       *pOffset;

   /* CMX structures  */
   struct t_optc1          stOptc1;
   struct t_opti1          stOpti1;
   struct t_optc2          stOptc2;
   struct t_optc3          stOptc3;

   unsigned long  int      ulPid;

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   sReturnCode = OK;

   *pulP2 = NULL;
   *pulP3 = NULL;
   *pulP4 = NULL;

   ulCMXEvent = (unsigned short int)ulInP1;

   if (!ulInP2) /*  && (ulCMXEvent == T_DATAIN) by sss for redr */
       *pulP1 = NULL;

   iTransConId = pContext->iTransConId;

   /*-----------------------------------------------------------------*/
   /* RECEIVE (Transport event and/or Transport user data)            */
   /*-----------------------------------------------------------------*/

   switch (ulCMXEvent) {

      case T_CONIN:

	 /*-----------------------------------------------------*/
	 /* Allocate memory for the Transport address of the    */
         /* calling application	                                */
	 /*-----------------------------------------------------*/

	 if ((hPartAddr = ro0030_localalloc ( RCro1e_MaxLenPSAP)) == 0) {
	    sReturnCode = -R_LALLCE;
	    break;
	 }

	 if ((pPartAddr = ro0031_locallock (hPartAddr)) == NULL) {
	    sReturnCode = -R_LLOCKE;
	    break;
         }
	 /*-----------------------------------------------------*/
	 /* Allocate memory for the Transport address of        */
         /* your own application	                        */
	 /*-----------------------------------------------------*/

	 if ((hMyAddr = ro0030_localalloc (RCro1d_MaxLenTSAP)) == 0) {
	    sReturnCode = -R_LALLCE;
	    break;
	 }

	 if ((pMyAddr = ro0031_locallock (hMyAddr)) == NULL) {
	    sReturnCode = -R_LLOCKE;
	    break;
         }

	 /*------------------------------------*/
         /* Fill up values for struct t_opc1   */
	 /*------------------------------------*/

         stOptc1.t_optnr = T_OPTC1;
         stOptc1.t_udatal = 0;
         stOptc1.t_udatap = NULL;
         stOptc1.t_xdata = T_NO;
         stOptc1.t_timeout = T_NO;

         /*--------------------------------------*/ 
         /* Call t_conin() CMX function:         */
         /* receive Transport connect indication */
         /*--------------------------------------*/

         ro0043_pack10Log(RC0020_INTERFACE ,
                                RL2014_EnTconin,
                                4,
                                sizeof(int),
                                (char FAR *)&ulInP3,
                                sizeof(union t_address),
                                (char FAR *)pMyAddr,
                                sizeof(union t_address),
                                (char FAR *)pPartAddr,
                                sizeof(struct t_optc1),
                                (char FAR *)&stOptc1);

	 iTransCode = t_conin ((int *)&ulInP3,	             /* -> */
	        	       (union t_address *)pMyAddr,   /* <- */
			       (union t_address *)pPartAddr, /* <- */
                               (t_opt1 *)&stOptc1            /* -> */
                              );                               

         ro0043_pack10Log(RC0020_INTERFACE ,
                                RL2015_ReTconin,
                                5,
                                sizeof(int),
                                (char FAR *)&iTransCode,
                                sizeof(int),
                                (char FAR *)&ulInP3,
                                sizeof(union t_address),
                                (char FAR *)pMyAddr,
                                sizeof(union t_address),
                                (char FAR *)pPartAddr,
                                sizeof(t_opt1),
                                (char FAR *)&stOptc1);
	 pContext->iTransConId = (int)ulInP3;

	 if ((sReturnCode = ro0352_processReturnValue (iTransCode, pContext,
	      ST0)) != OK)
	    break;

	 /*------------------------------------------------*/
	 /* Save the local Transport connection identifier */
	 /*------------------------------------------------*/

	 ro0229_setLoifId ((signed long int)ulInP3);

	 /*-------------------------------------------------*/
	 /* Free the memory associated with the My address  */
	 /* parameter .This memory is now no longer needed. */
	 /*-------------------------------------------------*/
	 if (hMyAddr != NULL) {
	    if (ro0032_localunlock (hMyAddr) != 0) {
	       sReturnCode = -R_LULOKE;
               break;
            }
          /* by kc on 08-12-93 for Called Address *****
	    if (ro0034_localfree (hMyAddr) != 0) {
	       sReturnCode = -R_LFREEE;
               break;
            } else {
               hMyAddr = NULL;
            } ****** end on 08-12-93 */
	 }


         if (ro0032_localunlock (hPartAddr) != 0) {
	    sReturnCode = -R_LULOKE;
	    break;
         }

         *pulP3 = (unsigned long int)hPartAddr;
         *pulP4 = (unsigned long int)hMyAddr; /* by kc on 08-12-93 */
	 *plOutEvent = RC0902_TCONNECTind;

         break;

      case T_REDIN:


	 /*------------------------------------*/
         /* Fill up values for struct t_opc3   */
	 /*------------------------------------*/

         stOptc3.t_optnr = T_OPTC3;
         stOptc3.t_udatal = MAXFILELEN;
         stOptc3.t_udatap = vFileName;
         stOptc3.t_xdata = T_NO;
         stOptc3.t_timeout = T_NO;
         stOptc3.t_ucepid = 0;

         /*--------------------------------------*/ 
         /* Call t_redin() CMX function:         */
         /* receive Transport connect indication */
         /*--------------------------------------*/

         ro0043_pack10Log(RC0020_INTERFACE ,
                                RL2016_EnTredin,
                                3,
                                sizeof(int),
                                (char FAR *)&ulInP3,
                                sizeof(int),
                                (char FAR *)&ulPid,
                                sizeof(t_opt23),
                                (char FAR *)&stOptc3);

	 iTransCode = t_redin ((int *)&ulInP3,	              /* -> */
                               (int *)&ulPid,                 /* -> */
                               (t_opt23 *)&stOptc3            /* -> */
                              );                               

         ro0043_pack10Log(RC0020_INTERFACE ,
                                RL2017_ReTredin,
                                4,
                                sizeof(int),
                                (char FAR *)&iTransCode,
                                sizeof(int),
                                (char FAR *)&ulInP3,
                                sizeof(int),
                                (char FAR *)&ulPid,
                                sizeof(t_opt23),
                                (char FAR *)&stOptc3);
	 pContext->iTransConId = (int)ulInP3;
         *pulP4 = ulPid;

	 if ((sReturnCode = ro0352_processReturnValue (iTransCode, pContext,
	      ST0)) != OK)
	    break;


         /*------------------------------*/
         /* Call t_info() CMX function:  */
         /* request Transport information*/
         /*------------------------------*/
         stOpti1.t_optnr = T_OPTI1;
          

            ro0043_pack10Log(RC0020_INTERFACE ,
                             RL2010_EnTinfo,
                             2,
                             sizeof(int),
                             (char FAR *)&iTransConId,
                             sizeof(t_opti),
                             (char FAR *)&stOpti1);
         iTransCode  = t_info ((int *)&ulInP3,            /* -> */
                                    (t_opti *)&stOpti1         /* -> */
                                   );
            ro0043_pack10Log(RC0020_INTERFACE ,
                             RL2011_ReTinfo,
                             2,
                             sizeof(int),
                             (char FAR *)&iTransCode,
                             sizeof(t_opti),
                             (char FAR *)&stOpti1);

         if (iTransCode  == T_OK) {
             pContext->ulTiduLength = (unsigned long int)stOpti1.t_maxl;
             sReturnCode = getHeadLen (pContext, iTransConId);
         } else {
             sReturnCode = t_error ();
             ro0042_writeLog (RC0020_INTERFACE  ,     
                              RL2005_TERROR,          
                              sizeof(int),            
                              (char FAR *)&sReturnCode);
             break;
	 }

	 /*------------------------------------------------*/
	 /* Save the local Transport connection identifier */
	 /*------------------------------------------------*/

	 ro0229_setLoifId ((signed long int)ulInP3);

	 /*------------------------------------------------*/
	 /* Convert the received file to a store, and      */
	 /* extract the offset from the store.             */
	 /*------------------------------------------------*/

	 if (*pulP1 != NULL) {
	    if (ro0075_deletestore (*pulP1) != 0) {
	       sReturnCode = -R_STOERR;
	       break;
            }
	 }

	 if ((*pulP1 = ro0082_FileToStore (vFileName, -1)) == 0) {
	    sReturnCode = -R_STOERR;
	    break;
         }  

         if ((lStoreLen = ro0062_storelength (*pulP1)) < 0) {        
            sReturnCode = -R_STOERR;                                      
            break;                                                        
         }                                                                

         if (ro0066_storepos (*pulP1,                                     
                     (long)(lStoreLen - sizeof(unsigned long int))) != OK) {        
            sReturnCode = -R_STOERR;                                      
            break;                                                        
         }                                                                

                                                                 
         if ((pOffset = (unsigned long *)ro0072_stgets (*pulP1,  
                                     (long)(sizeof(unsigned long)))) == NULL) { 
            sReturnCode = -R_STOERR;                                    
            break;                                                      
         }                                                              
         ro0041_memcpy ((char FAR *)pulP2,                        
                        (char FAR *)pOffset,
                        (long)(sizeof(unsigned long)));             

         lStoreLen -=  sizeof(unsigned long);

         if (ro0084_stStoreResize (*pulP1,                  
                                   lStoreLen) != lStoreLen) {
            sReturnCode = -R_STOERR;                        
            break;                                          
         }                                                  
                                                                 

	 *plOutEvent = RC090b_TREDIRECTind;

         break;

      case T_CONCF:

	 /*------------------------------------*/
         /* Fill up values for struct t_optc1   */
	 /*------------------------------------*/

         stOptc1.t_optnr = T_OPTC1;
         stOptc1.t_udatal = 0;
         stOptc1.t_udatap = NULL;
         stOptc1.t_xdata = T_NO;
         stOptc1.t_timeout = T_NO;

         /*------------------------------*/ 
         /* Call t_concf() CMX function: */
         /* Transport connect request    */
         /*------------------------------*/

         ro0043_pack10Log(RC0020_INTERFACE ,
                             RL2018_EnTconcf,
                             2,
                             sizeof(int),
                             (char FAR *)&iTransConId,
                             sizeof(t_opt1),
                             (char FAR *)&stOptc1);
         iTransCode = t_concf ((int *)&iTransConId,              /* -> */
                               (t_opt1 *)&stOptc1                /* -> */
                              );

         ro0043_pack10Log(RC0020_INTERFACE ,
                             RL2019_ReTconcf,
                             2,
                             sizeof(int),
                             (char FAR *)&iTransCode,
                             sizeof(t_opt1),
                             (char FAR *)&stOptc1);
	 if ((sReturnCode = ro0352_processReturnValue (iTransCode, pContext,
	      ST0)) != OK)
	    break;

         /*------------------------------*/
         /* Call t_info() CMX function:  */
         /* request Transport information*/
         /*------------------------------*/
         stOpti1.t_optnr = T_OPTI1;
          
            ro0043_pack10Log(RC0020_INTERFACE ,
                             RL2010_EnTinfo,
                             2,
                             sizeof(int),
                             (char FAR *)&iTransConId,
                             sizeof(t_opti),
                             (char FAR *)&stOpti1);
         iTransCode  = t_info ((int *)&iTransConId,       /* -> */
                                    (t_opti *)&stOpti1         /* -> */
                                   );
            ro0043_pack10Log(RC0020_INTERFACE ,
                             RL2011_ReTinfo,
                             2,
                             sizeof(int),
                             (char FAR *)&iTransCode,
                             sizeof(t_opti),
                             (char FAR *)&stOpti1);
         if (iTransCode   == T_OK) {
             pContext->ulTiduLength = (unsigned long int)stOpti1.t_maxl;
             sReturnCode = getHeadLen (pContext, iTransConId);
         } else {
             sReturnCode = t_error ();
             ro0042_writeLog (RC0020_INTERFACE  ,     
                              RL2005_TERROR,          
                              sizeof(int),            
                              (char FAR *)&sReturnCode);
             break;
	 }

/* ********************** start of changes by kc on 22/6/93 ************* */
/* **** the mem. is allocated for the responder address . Eventhougth the
        CMX don't have the Responder address the upper layers may or  maynot
        have. So default allocate it . This is freed by Session           */

	 if ((hPartAddr = ro0030_localalloc ( RCro1e_MaxLenPSAP)) == 0) {
	    sReturnCode = -R_LALLCE;
	    break;
	 }
         *pulP3 = (unsigned long int)hPartAddr;
/* *** end of changes by kc on 22/6/93 ************************************ */
	 *plOutEvent = RC0904_TCONNECTcnf;

	 break;

      case T_DISIN:

         /*------------------------------------*/
         /* Fill up values for struct t_optc2  */
  	 /*------------------------------------*/

         stOptc2.t_optnr = T_OPTC2;
         stOptc2.t_udatal = 0;
         stOptc2.t_udatap = NULL;

         /*---------------------------------*/ 
         /* Call t_disin() CMX function:    */
         /* Transport disconnect indication */
         /*---------------------------------*/

            ro0043_pack10Log(RC0020_INTERFACE ,
                             RL201a_EnTdisin,
                             2,
                             sizeof(int),
                             (char FAR *)&iTransConId,
                             sizeof(struct t_optc2),
                             (char FAR *)&stOptc2);
         iTransCode = t_disin ((int *)&iTransConId,           /* -> */
                                (int *)&ulAbortReason,        /* <- */
                                (t_opt2 *)&stOptc2            /* -> */
                               );

            ro0043_pack10Log(RC0020_INTERFACE ,
                             RL201b_ReTdisin,
                             2,
                             sizeof(int),
                             (char FAR *)&iTransCode,
                             sizeof(int),
                             (char FAR *)&ulAbortReason);

	 if ((sReturnCode = ro0352_processReturnValue (iTransCode, pContext,
	      ST0)) != OK)
	    break;
 	 *pulP3 = (RC0301_DisconnectReasons | (ulAbortReason << 16));
	 *pulP2 = ulAbortReason;

        /* switch (ulAbortReason) {

            case T_USER:            
		 *pulP3 = RC030a_C_PartnerDisconnect;
		 *pulP2 = ulAbortReason;
               break;

            case T_RTIMEOUT:       
		 *pulP3 = RC030b_C_TimeoutDisconnect;
		 *pulP2 = ulAbortReason;
	       break;

	    case T_RADMIN:
		 *pulP3 = RC030c_C_AdminDisconnect;
		 *pulP2 = ulAbortReason;
	       break;

            case T_RCCPEND:            
		 *pulP3 = RC030d_C_CCPBreakdown;
		 *pulP2 = ulAbortReason;
               break;

            case T_RUNKNOWN:       
		 *pulP3 = RC030e_P_UnspecifiedReason;
		 *pulP2 = ulAbortReason;
	       break;

	    case T_RSAPNOTATT:
		 *pulP3 = RC030f_P_TSAPNotAttached;
		 *pulP2 = ulAbortReason;
	       break;

            case T_RUNSAP:            
		 *pulP3 = RC030g_P_UnknownTSAP;
		 *pulP2 = ulAbortReason;
               break;

            case T_RSYSERR:       
		 *pulP3 = RC030h_P_NetworkError;
		 *pulP2 = ulAbortReason;
	       break;

	    case T_RCONGEST:
		 *pulP3 = RC030i_P_ResourceCongest;
		 *pulP2 = ulAbortReason;
	       break;

            case T_RPROTERR:            
		 *pulP3 = RC030j_P_ProtocolError;
		 *pulP2 = ulAbortReason;
               break;

            case T_RNOCONN:       
		 *pulP3 = RC030k_P_NetworkConnectReject;
		 *pulP2 = ulAbortReason;
	       break;

            case T_RLPROTERR:            
		 *pulP3 = RC030l_L_ProtocolError;
		 *pulP2 = ulAbortReason;
               break;

            case T_RLNORESP:       
		 *pulP3 = RC030m_L_NoResponse;
		 *pulP2 = ulAbortReason;
	       break;

	    case T_RLIDLETRAF:
		 *pulP3 = RC030n_L_TimeoutDisconnect;
		 *pulP2 = ulAbortReason;
	       break;

	    default:
		 *pulP3 = RC030o_OtherError;
		 *pulP2 = ulAbortReason;
               break;
         } */

	 *pulP1      = NULL;
	 *plOutEvent = RC0906_TDISCONNECTind;
	 sReturnCode = DESTRUCT;

         break;

      case T_DATAIN: {
          signed short  int    bTiduFirstPart = TRUE;
          signed        int    iRealDataChain = T_MORE;
          unsigned long int    hRecvData      = LNULL;
          char                *pRecvData      = LNULL;
          signed   long int    lRecvLen       = LNULL;

          /*-------------------------------*/
          /* There is some data comming:   */
          /* prepare a Store to receive it */
          /*-------------------------------*/

          if (pContext->hRecStore != NULL) { /* If data was not fully rcvd
                                                during the last t_datain call */
              *pulP1 = pContext->hRecStore;
          } else {
              *pulP1 = ro0060_newstore (ST_USEMAXBUF, 0L,
                                            (signed short int *)&sReturnCode);
              if (sReturnCode != OK)
                  RM0003_RETURN (fnctName, signed short int, -R_STOERR)
          }

          lBufLen = ro0061_stbuflen (*pulP1);

          lRecvLen = (ulInP2 > lBufLen) ? (signed)lBufLen
                                        : (signed)ulInP2;

          hRecvData = ro0020_globalalloc (lRecvLen);
          if (!hRecvData) sReturnCode = -R_GALLCE;
          if (!sReturnCode) {
              pRecvData = (char FAR *) ro0021_globallock (hRecvData);
              if (!pRecvData) sReturnCode = -R_GLOCKE;
          }
          if (sReturnCode)
              RM0003_RETURN (fnctName, signed short int, sReturnCode)



         /* *********************************************************** */
         /* Note :- Data is received in the form of TSDU's.Each TSDU is */
         /* received in the form of several TIDU's.The arrival of each  */
         /* TIDU is indicated by a separate T_DATAIN which is received  */
         /* via a t_event call.Each TIDU can be received with one or    */
         /* more t_datain calls.The receipt of the entire TIDU is ind-  */
         /* icated by the return of zero by the t_datain() function.The */
         /* receipt of the entire TSDU is indicated by the T_END in the */
         /* chain parameter of the t_datain() function.T_more is indic- */
         /* ated in the chain parameter if the current TIDU is not the  */
         /* last of the current TSDU.A T_DATAIN is pushed up only when  */ 
         /* the entire TSDU has been received successfully.             */
         /* *********************************************************** */

	 do {	  /* while the annouced TIDU is not completly received */

	    /*--------------------------------*/
	    /* Call t_datain() CMX function:  */
	    /* receive normal data indication */
	    /*--------------------------------*/

            ro0043_pack10Log(RC0020_INTERFACE ,
                             RL201c_EnTdatain,
                             3,
                             sizeof(int),
                             (char FAR *)&iTransConId,
                             sizeof(int),
                             (char FAR *)&lRecvLen,
                             sizeof(int),
                             (char FAR *)&iDataChain );
	    iTransCode = t_datain ((int *)&iTransConId,		  /* -> */
				   (char *)pRecvData,	          /* <- */
				   (int *)&lRecvLen,              /* <- */
				   (int *)&iDataChain		  /* <- */
				  );

            ro0043_pack10Log (RC0020_INTERFACE  ,     
                             RL201d_ReTdatain     ,
                             3                   ,                  
                             sizeof(int),            
                             (char FAR *)&iTransCode,
                             sizeof(int),
                             (char FAR *)&iDataChain,
                             (signed short int)lRecvLen,
                             (char FAR *)pRecvData );

            if (pContext->bNetBIOS) {
                if (bTiduFirstPart) iRealDataChain = (pRecvData [0] == NETB_END)
                                                   ? T_END : T_MORE;
            }
            else                    iRealDataChain = iDataChain;

            if (ro0068_stputm (*pulP1,
                               (pRecvData + pContext->cHeadLen),
                               (lRecvLen  - pContext->cHeadLen)
                              ) < NULL)
	        RM0003_RETURN (fnctName, signed short int, -R_STOERR)


	    if (iTransCode > 0) {

	        /*------------------------------------*/
	        /* One complete Tidu not received yet */
	        /*------------------------------------*/
	        lRecvLen = (iTransCode < lRecvLen)
                                   ? iTransCode  : lRecvLen;
                bTiduFirstPart = FALSE;
	    } else if (iTransCode == T_ERROR) {
	        sReturnCode = ro0352_processReturnValue (iTransCode, pContext,
	                                                 ST0);
                break;
	    }

	 } while (iTransCode > 0);

         iDataChain = iRealDataChain;

	 if (iDataChain == T_END) {
	     *plOutEvent = RC0908_TDATAind;
	     pContext->hRecStore = NULL;
	 } else {
	     *plOutEvent = 0L;
	     pContext->hRecStore = *pulP1;
	 }

         if (ro0022_globalunlock (hRecvData) != 0)
             RM0003_RETURN (fnctName, signed short int, -R_GULOKE)
         if (ro0024_globalfree   (hRecvData))
	     RM0003_RETURN (fnctName, signed short int, -R_GFREEE)

         break;
      }

      default:
         sReturnCode = -R_LUNKEV;
         break;

   }  /* Switch Ends */

   /*---------------------------------------------------------*/
   /* If some data was coming from the partner and all of it  */
   /* is already received then close the corresponding store  */
   /*---------------------------------------------------------*/

   if ((ulInP2 > 0) && (iDataChain == T_END)) {
      if ((ro0083_stbufresize (*pulP1) < 0) ||
	  (ro0073_storeidle (*pulP1) != 0))
	 sReturnCode = -R_STOERR;
   }

   RM0003_RETURN (fnctName, signed short int, sReturnCode)
}



/********************      END OF ROLOIF1.C       *********************/
