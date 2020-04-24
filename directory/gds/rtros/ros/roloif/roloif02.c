/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roloif02.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:44  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:46  root]
 * 
 * Revision 1.1.2.5  1994/08/24  08:35:31  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:35:18  marrek]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:14  keutel
 * 	OT 11620
 * 	[1994/08/19  13:51:26  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:08  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:46  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:57  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:04:30  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:31  keutel
 * 	creation
 * 	[1994/03/21  13:33:32  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roloif02.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:44 $" } ;
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
/* NAME         : roloif0.c                                           */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* COMPONENT	: RTROS lower interface				      */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V03.00                                        */
/*                                                                    */
/* DESCRIPTION  : This module contains functions which are calling    */
/*                functions of the underlying transport product       */
/*                (CMX V3.0).                                         */
/*                                                                    */
/*    Only functions of the ROLOIF?.c modules call functions of the   */
/*    transport product.                                              */
/*    This module contains the calls to t_attach, t_detach and        */
/*    t_event (t_error as well).                                      */
/*                                                                    */
/* SYSTEM DEPENDENCE : MAKRO constant BS2000 defined    : for BS2000  */
/*                     MAKRO constant BS2000 not defined: for SINIX   */
/*                     This module also depends on the transport      */
/*                     interface of CMX-V3.0                          */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.0A00 K00| 93-03-09| original                      |sss |         */
/*           |         |                               |    |         */
/*datoff **************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/
 
static char *sccsid =    "@(#) roloif02.c 1.1 93/11/29   RTS-V2.00";


/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <cmx.h>          /* interface to the SINIX transport system  */
#include <signal.h>       /* SIGCLD signal is used in wake call.      */

#include <rogenrl.h>      /* General definitions                      */
#include <roerval.h>      /* General definitions (RTS error values)   */
#include <rolgval.h>      /* General definitions                      */
#include <routil0.h>      /* General utilities                        */
#include <rostor0.h>      /* Store interface                          */
#include <rouply0.h>      /* General interface definitions            */
#include <rouply1.h>      /* General interface definitions            */
#include <rodpat0.h>      /* Dispatcher - PM interface                */
#include <rotpm00.h>      /* Specific LOIF - SPM interface            */

#include <roloif2.h>      /* LOIF calling interface                   */
#include <roloif32.h>      /* LOIF calling interface                   */
#include <roloif42.h>
 
/*********************************************************************/
/*              D E F I N E S                                        */
/*********************************************************************/

#define   RC0300_CHECK   0
#define   RC0301_WAIT   -1
 
int t_error(void);

/*********************************************************************/
/*              G L O B A L  V A R I A B L E S                       */
/*********************************************************************/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0300_attach                                       */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    It calls the attach function of the CMX-V3.0 transport          */
/*    interface  and returns if an error occurs when the transport    */
/*    functon is called.                                              */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/*    hECB       : Handle of ECB                                      */
/*    pAppId     : Pointer to the application identifier of loif      */
/*    ulSysId    : System identifier                                  */
/*    hAddress   : Handle of local address                            */
/*                                                                    */
/* OUT-PARAMETERS:                                                    */
/*                                                                    */
/*    pAppId     : Pointer to the app id of loif as returned by the   */
/*                 t_attach() function.                               */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*                0 : Successful                                      */
/*                1 : Not first                                       */
/*              < 0 : Error                                           */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS                :                                           */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro0300_attach (

unsigned long int       hECB,        /* --> handle of ECB             */
unsigned long int FAR  *pAppId,      /* <-- pointer to app id of loif */
unsigned long int       ulSysId,     /* --> system id                 */
unsigned long int       hAddress,    /* --> handle of local address   */
signed  short int FAR  *pTpmFlowCtrl)/* <-- Tpmflow control DOS       */
 
{

   RM0001_STRG (fnctName, "ro0300_attach")
 
   signed short int   sReturnCode;  /* function's return code              */
   int		      iAddErrInf;
   char 	     *pAddress;
   char 	     *pAddr;
   struct t_opta2    stOpta2;
   int               iRc;

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   if ((pAddress = ro0031_locallock (hAddress)) == NULL) {
      RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)
   }
      
   ro0354_getAddressWithoutPandSsel (pAddress, &pAddr);
    

   /*---------------------------------------------*/
   /* Assign values to elements of struct t_opta2 */
   /*---------------------------------------------*/

   stOpta2.t_optnr  = T_OPTA2;
   stOpta2.t_apmode = (T_ACTIVE | T_PASSIVE | T_REDIRECT);
   stOpta2.t_conlim = T_NOLIMIT;
   stOpta2.t_uattid = (int)hECB;
   
   /*--------------------------------*/ 
   /* Call transport attach function */
   /*--------------------------------*/
   ro0043_pack10Log(RC0020_INTERFACE ,
                    RL2001_EnTAttach,
                    2               ,
                    sizeof (struct t_myname),
                    (char FAR *) pAddr,
                    sizeof(struct t_opta2),
                    (char FAR *)&stOpta2);

   iRc =  t_attach ((struct t_myname *)pAddr ,(t_opta *)&stOpta2) ;
   ro0043_pack10Log(RC0020_INTERFACE , 
                    RL2002_ReTAttach,
                    2,
                    sizeof(int),
                    (char FAR *)&iRc,
                    sizeof(struct t_opta2),
                    (char FAR *)&stOpta2);
       
   switch (iRc){

      /*---------------------*/ 
      /* Process return code */
      /*---------------------*/
   
      case T_OK:        
         /* *******  by kc on 25/6/93 since CMX returns 0x0 for CCP-LAN1 */
         if (!stOpta2.t_attid ) stOpta2.t_attid = 1; 
         *pAppId = stOpta2.t_attid;
         sReturnCode = OK;
         break;
 
      case T_NOTFIRST:  
         *pAppId = stOpta2.t_attid;
         sReturnCode = 1;
         break;

      default:          
	 iAddErrInf = t_error ();
         ro0042_writeLog (RC0020_INTERFACE  ,
                          RL2005_TERROR,
                          sizeof(int),
                          (char FAR *)&iAddErrInf);
         if (!(iAddErrInf & CMXMASK)) {
	    sReturnCode = -R_ICMXER;
         } else {
	   sReturnCode = -R_ISYSOS;
	 }
         break;
      /* switch (iAddErrInf) {
            case T_ENOENT:
	       sReturnCode = -R_INOENT;
               break;
 
            case T_EFAULT:
	       sReturnCode = -R_IFAULT;
               break;

            case T_WPARAMETER:
	       sReturnCode = -R_IPARAM;
               break;
 
            case T_WAPPLICATION:
	       sReturnCode = -R_IAPPLI;
               break;

            case T_WAPP_LIMIT:
	       sReturnCode = -R_IAPLMT;
               break;
 
            case T_WPROC_LIMIT:
	       sReturnCode = -R_IPRLMT;
               break;

            case T_NOCCP:
	       sReturnCode = -R_INOCCP;
               break;

            case T_WLIBVERSION:
	       sReturnCode = -R_ILIBVN;
               break;

            default :
	       sReturnCode = -R_ISYSOS;
               break;
         } */
   }

   if (ro0032_localunlock (hAddress) != 0) {
      sReturnCode = -R_LULOKE;
   }
 
   RM0003_RETURN (fnctName, signed short int, sReturnCode)
}

 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0301_detach                                       */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    It calls the detach function of the CMX-V3.0 transport          */
/*    interface and returns if an error occurs when the transport     */
/*    function is called.                                             */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/*    hECB       : Handle of ECB                                      */
/*    pAppId     : Pointer to the application identifier of loif      */
/*                                                                    */
/* OUT-PARAMETERS: None                                               */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*                0 : Successful                                      */
/*              < 0 : Error                                           */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS                :                                           */
/*                                                                    */
/*inoff ***************************************************************/
 
signed short int PASCAL ro0301_detach (

unsigned long int       hECB,        /* --> handle of ECB             */
unsigned long int FAR  *pAppId)      /* --> pointer to app id of loif */
 
{

   RM0001_STRG (fnctName, "ro0301_detach")
 
   signed short int   sReturnCode;  /* function's return code       */
   int		      iAddErrInf;
   char FAR          *pMyAddr;
   unsigned long int  hMyAddr;
   int                iRc; 
   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)
 
   /*-----------------------------------------------------*/
   /* Allocate memory for the Transport address of        */
   /* your own application	                          */
   /*-----------------------------------------------------*/

   if ((hMyAddr = ro0030_localalloc (RCro1d_MaxLenTSAP)) == 0) {
      sReturnCode = -R_LALLCE;
      RM0003_RETURN (fnctName, signed short int, sReturnCode);
   }

   if ((pMyAddr = ro0031_locallock (hMyAddr)) == NULL) {
      sReturnCode = -R_LLOCKE;
      RM0003_RETURN (fnctName, signed short int, sReturnCode);
   }

   if ((sReturnCode = ro0232_getAddress2 (hECB, RCro14_TransportAddress,
                       RCro1d_MaxLenTSAP, pMyAddr)) < 0) 
      RM0003_RETURN (fnctName, signed short int, sReturnCode);
      
   

   /*--------------------------------*/ 
   /* Call transport detach function */
   /*--------------------------------*/
   ro0042_writeLog (RC0020_INTERFACE  ,
                    RL2003_EnTDetach,
                    sizeof(struct t_myname),
                    (char FAR *)pMyAddr);
   iRc = t_detach ((struct t_myname*)pMyAddr); 
   
   ro0042_writeLog (RC0020_INTERFACE  ,
                    RL2004_ReTDetach,
                    sizeof(int),
                    (char FAR *)iRc);
   switch (iRc) {

      /*---------------------*/
      /* Process return code */
      /*---------------------*/
   
      case T_OK: 
         sReturnCode = OK;
         break;
 
      default:   
	 iAddErrInf = t_error ();
         ro0042_writeLog (RC0020_INTERFACE  ,
                          RL2005_TERROR,
                          sizeof(int),
                          (char FAR *)&iAddErrInf);
            if (!(iAddErrInf & CMXMASK)) {
	        sReturnCode = -R_ICMXER;
            } else {
	      sReturnCode = -R_ISYSOS;
	    }
         break;
      /* switch (iAddErrInfo) {
            case T_EFAULT: 
	       sReturnCode = -R_IFAULT;
               break;

            case T_WPARAMETER:
	       sReturnCode = -R_IPARAM;
               break;
 
            case T_WAPPLICATION:
	       sReturnCode = -R_IAPPLI;
               break;

            case T_WSEQUENCE: 
	       sReturnCode = -R_ISEQUE;
               break;

            default: 
	       sReturnCode = -R_ISYSOS;
               break;
         } */
   }
 
   /*-------------------------------------------------*/
   /* Free the memory associated with the address     */
   /* parameters.This memory is now no longer needed. */
   /*-------------------------------------------------*/
   if (hMyAddr != NULL) {
      if (ro0032_localunlock (hMyAddr) != 0)
         sReturnCode = -R_LULOKE;
      if (ro0034_localfree (hMyAddr) != 0) {
         sReturnCode = -R_LFREEE;
      } else {
         hMyAddr = NULL;
      }
   }
   RM0003_RETURN (fnctName, signed short int, sReturnCode)
}

 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0305_wake                                         */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    It uses signals to wake up a process.                           */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/*    ulSysId    : System id of the process which has to be waked     */
/*                                                                    */
/* OUT-PARAMETERS: None                                               */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*                0 : Successful                                      */
/*              < 0 : Error:                                          */
/*			 -R_WAKEER				      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS                :                                           */
/*                                                                    */
/*inoff ***************************************************************/
 
signed short int PASCAL ro0305_wake (

unsigned long int   ulSysId)         /* --> system identifier         */
 
{

   RM0001_STRG (fnctName, "ro0305_wake")
 
   signed short int   sReturnCode;  /* function's return code          */
 
   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)
 
   /*---------------------------*/ 
   /* Make a  kill system call  */
   /*---------------------------*/
   /* 
   iAuxInt = kill ((long)ulSysId, SIGCLD); 
   switch (iAuxInt) { */
      /*---------------------*/ 
      /* Process return code */
      /*---------------------*/
 /*      case OK: 
         sReturnCode = OK;
         break;
 
      default: 
	 sReturnCode = -R_WAKEER;
         break;
   }
 */ 
   RM0003_RETURN (fnctName, signed short int, sReturnCode)
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0311_CheckOrWait                                  */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    It calls the transport event function to receive transport      */
/*    events.                                                         */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/*    sBlockTime : Blocking time:                                     */
/*                    ==  0 : Check only                              */
/*                    == -1 : Infinitive waiting                      */
/*                    other values : waiting time in seconds          */
/*                                                                    */
/*    ulSysId    : System identifier                                  */
/*                                                                    */
/* OUT-PARAMETERS:                                                    */
/*                                                                    */
/*    *phECB     : Pointer to handle of ECB                           */
/*    *phLoifId  : Pointer to handle of loif assoc. identifier	      */
/*    *pEvent    : Pointer to event                                   */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*                 0 : Success, event                                 */
/*                 2 : No event                                       */
/*                 3 : Timeout, no event                              */
/*               < 0 : Error:                                         */
/*                        -R_ICMXER                                   */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS                :                                           */
/*                                                                    */
/*inoff ***************************************************************/
 
signed short int PASCAL ro0311_CheckOrWait (

unsigned long  int FAR  *phECB,         /* <-- pt to handle of ECB    */
signed	 long  int FAR	*phLoifId,	/* <-- pt to hdl of Loif id   */
signed  short  int FAR  *pPmNo,         /* <-- pt to pm number        */
signed   long  int FAR  *pEvent,        /* <-- pt to event            */
unsigned long  int FAR	*pulP1, 	/* <-- pointer to p1	      */
unsigned long  int FAR	*pulP2, 	/* <-- pointer to p2	      */
unsigned long  int FAR	*pulP3, 	/* <-- pointer to p3	      */
unsigned long  int FAR	*pulP4, 	/* <-- pointer to p4	      */
signed   short int       sBlockTime,    /* --> blocking time          */
unsigned long  int       ulSysId)       /* --> system identifier      */
 
{
 
   RM0001_STRG (fnctName, "ro0311_CheckOrWait")

   signed short int   sReturnCode;  /* function's return code            */
   int		      iRetEvent;
   int                iCmode;
   int		      iAddErrInf;
   struct t_opte1     stOpte1;
 
   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   *pulP1 = *pulP2 = *pulP3 = *pulP4 = 0L;

   *pPmNo = RC0213_Transport;

   /*-----------------------------------------*/ 
   /* Control of cmode parameter's value      */
   /*-----------------------------------------*/

   if (sBlockTime < RC0301_WAIT) {
      RM0003_RETURN (fnctName, signed short int, ERROR)
   }

   /*-------------------------------------------------*/ 
   /* If there is a blocking time, set the time limit */
   /* and cmode parameter valve to T_WAIT             */
   /*-------------------------------------------------*/

   if ((sBlockTime > RC0300_CHECK) || (sBlockTime ==RC0301_WAIT)){
      iCmode = T_WAIT;
      stOpte1.t_timeout = sBlockTime;
   } else {
   /*---------------------------------------------*/ 
   /* else set cmode parameter value to  T_CHECK  */
   /*---------------------------------------------*/ 
      iCmode = T_CHECK;    
   }
   /*---------------------------------------------------------*/ 
   /*  Assign values to the elements of the structure t_opte1 */
   /*---------------------------------------------------------*/ 

    stOpte1.t_optnr = T_OPTE1;
    
   /*-----------------------------------------------------------------*/ 
   /* Call transport event funcnction and wait for an event if there  */
   /* is a specific blocking time .                                   */
   /*-----------------------------------------------------------------*/

   ro0043_pack10Log(RC0023_CWAITTRACE , 
                    RL2006_EnTEvent,
                    2              ,
                    sizeof(int),
                    (char FAR *)iCmode,
                    sizeof(struct t_opte1),
                    (char FAR *)&stOpte1);
   iRetEvent = t_event ((int *)pulP3,		/* <- */
		       iCmode,		        /* -> */
		       (t_opte *)&stOpte1);     /* -> */
   
   ro0043_pack10Log(RC0023_CWAITTRACE , 
                    RL2007_ReTEvent,
                    3              ,
                    sizeof(unsigned long int),
                    (char FAR *)pulP3,
                    sizeof(int),
                    (char FAR *)&iRetEvent,
                    sizeof(struct t_opte1),
                    (char FAR *)&stOpte1);
 
   *pulP1 = (unsigned long int)iRetEvent;
   *pulP2 = (unsigned long int)stOpte1.t_evdat;

   /*-----------------------------------------------*/ 
   /* Process return code of session event function */
   /*-----------------------------------------------*/

   switch (iRetEvent) {
      
      case T_ERROR: 
	 iAddErrInf = t_error ();
         ro0042_writeLog (RC0020_INTERFACE  ,
                          RL2005_TERROR,
                          sizeof(int),
                          (char FAR *)&iAddErrInf);
         if (!(iAddErrInf & CMXMASK)) {
	    sReturnCode = -R_ICMXER;
         } else {
	    sReturnCode = -R_ISYSOS;
	 }
     /*  switch (iAddErrInf) {

            case T_EFAULT: 
	       sReturnCode = -R_IFAULT;
               break;

            case T_WPARAMETER:
	       sReturnCode = -R_IPARAM;
               break;
 
            case T_WSEQUENCE: 
	       sReturnCode = -R_ISEQUE;
               break;

            default: 
	       sReturnCode = -R_ISYSOS;
               break;
         }  */
         break;
 
      case T_NOEVENT: 
         if (iCmode == T_CHECK) {
            sReturnCode = 2;
         } else {
           sReturnCode = 3;
         }
         break;
 
      case T_CONIN:
      case T_REDIN:
	 *phECB    = (unsigned long int)stOpte1.t_uattid;
	 *phLoifId = 0L;
	 *pEvent   = 0;
         sReturnCode = OK;
         break;

      default:
	 *phECB    = 0L;
	 *phLoifId = *pulP3;
	 *pEvent   = 0;
         sReturnCode = OK;
         break;
   }
   RM0003_RETURN (fnctName, signed short int, sReturnCode)
}

 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0399_cleanUp                                      */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    This function frees only the second parameter (it ignores the   */
/*    other ones), and it asumes that it is a memory handle of the    */
/*    lower interface's context.                                      */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/*    lEvent	 : Not used					      */
/*    ulP1	 : Memory handle of the lower interface's context     */
/*    ulP2	 : Not used					      */
/*    ulP3	 : Not used					      */
/*    ulP4	 : Not used					      */
/*                                                                    */
/* OUT-PARAMETERS: None 					      */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*                0 : Successful                                      */
/*              < 0 : Error:                                          */
/*			 -R_LLOCKE				      */
/*			 -R_LULOKE				      */
/*			 -R_LFREEE				      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS		  :					      */
/*                                                                    */
/*inoff ***************************************************************/
 
signed short int PASCAL ro0399_cleanUp (

signed   long int   lEvent,           /* --> event                    */
unsigned long int   ulP1,             /* --> Parameter 1              */
unsigned long int   ulP2,             /* --> Parameter 2              */
unsigned long int   ulP3,             /* --> Parameter 3              */
unsigned long int   ulP4)             /* --> Parameter 4              */
 
{

   RM0001_STRG (fnctName, "ro0399_cleanUp")

   signed  short int      sReturnCode;     /* function's return code  */
   rT0300_roloifContext  *pContext;        /* lower interface context */
   unsigned long int      hAuxSend;        /* work variable           */
   rT0301_sendQueue      *pAuxSend;        /* work variable           */
 
   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   sReturnCode = OK;

   /*--------------------------------------*/
   /* Free the Context structure (ulP1):   */
   /* If the Send list is not empty, first */
   /* free it and after, free the context  */
   /*--------------------------------------*/

   if (ulP1 == NULL)
      RM0003_RETURN (fnctName, signed short int, sReturnCode)

   if ((pContext = (rT0300_roloifContext *)ro0031_locallock (ulP1)) == NULL)
      RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)

   if (pContext->hRecStore != NULL)
      if (ro0075_deletestore (pContext->hRecStore) != 0)
	 RM0003_RETURN (fnctName, signed short int, -R_STOERR)

   if (pContext->hSendData) {
       if (ro0022_globalunlock (pContext->hSendData) != 0)
            RM0003_RETURN (fnctName, signed short int, -R_GULOKE)

       if (ro0024_globalfree (pContext->hSendData))
	    RM0003_RETURN (fnctName, signed short int, -R_GFREEE)
   }
   hAuxSend = pContext->hFirstSendQu;

   while (hAuxSend != NULL) {

      if ((pAuxSend = (rT0301_sendQueue *)ro0031_locallock (hAuxSend)) 
	  == NULL)
         RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)

      pContext->hFirstSendQu = pAuxSend->hNextElem;

      if (pAuxSend->hSendStore != NULL)
	 if (ro0075_deletestore (pAuxSend->hSendStore) != 0)
	    RM0003_RETURN (fnctName, signed short int, -R_STOERR)

      if (pAuxSend->ulP3 != NULL)
	 if (ro0034_localfree (pAuxSend->ulP3) != 0)
	    RM0003_RETURN (fnctName, signed short int, -R_LFREEE)

      if (ro0032_localunlock(hAuxSend) != 0)
         RM0003_RETURN (fnctName, signed short int, -R_LULOKE)

      if (ro0034_localfree (hAuxSend) != 0)
         RM0003_RETURN (fnctName, signed short int, -R_LFREEE)

      hAuxSend = pContext->hFirstSendQu;
   }

   /*----------------------------------------*/
   /* After the Send queue has been deleted, */
   /* free the main Context structure	     */
   /*----------------------------------------*/

   if (ro0032_localunlock(ulP1) != 0)
      sReturnCode = -R_LULOKE;
   else if (ro0034_localfree (ulP1) != 0)
      sReturnCode = -R_LFREEE;
      
   RM0003_RETURN (fnctName, signed short int, sReturnCode)
}


/********************       END OF ROLOIF0.C       ********************/
