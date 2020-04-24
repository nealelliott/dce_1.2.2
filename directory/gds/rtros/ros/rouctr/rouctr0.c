/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rouctr0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:33  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:06  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:26  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:37  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:43  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:09:07  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:21  keutel
 * 	creation
 * 	[1994/03/21  13:37:49  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rouctr0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:33 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1991 by                                                  *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                                 *
*  * All rights reserved                                                    *
*  *                                                                        *
*  * This software is furnished under licence and may be used only in       *
*  * accordance with the terms of that licence and with the inclusion of    *
*  * the above copyright notice. This software may not be provided or       *
*  * otherwise made available to, or used by, any other person. No title    *
*  * to or ownership of the software is hereby transferred.                 *
*                                                                           *
\***************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME 	: rouctr0.c					      */
/*                                                                    */
/* AUTHOR       : Vinita 					      */
/* DATE 	: 93-03-12					      */
/*                                                                    */
/* COMPONENT	: RTS association upper control function	      */
/*                                                                    */
/* PRD#/VERS.	: RTS-V3.00					      */
/*                                                                    */
/* DESCRIPTION	:						      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/* 3.00 K0  | 93-03-12|  original		       |vp  |	      */
/*	    |	      | 			       |    |	      */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) rouctr0.c 1.1 93/11/29   RTS-V2.00";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>	 /* RTS general definitions		     */
#include <roerval.h>	 /* RTS error values			     */
#include <rolgval.h>	 /* RTS logging type definitions	     */

#include <rouply0.h>	 /* RTS common upper layer definitons	     */
#include <rouply1.h>	 /* RTS common upper layer definitons	     */

#include <tpdu.h>        /* tpdu interface                           */
#include <roexif2.h>     /* Call back function def.                  */
#include <routil0.h>	 /* RTS utilities interface		     */
#include <routil1.h>	 /* RTS utilities interface (AL-types)	     */
#include <routilt.h>     /* TPDU interface                           */
#include <rodpat0.h>	 /* RTS dispatcher pm interface 	     */

#include <roropm0.h>	 /* ROS ropm eventing interface.	     */

#include <rouctr0.h>	 /* RTS upper control service interface      */
#include <rouctr2.h>	 /* RTS upper control calling interface      */
#include <rouctr3.h>	 /* RTS upper control logging interface      */
#include <rouctr4.h>	 /* RTS upper control ae invocation context  */
#include <rouctr5.h>	 /* RTS upper control ae invocation context  */



/**********************************************************************/
/*								      */
/*		 L O C A L  V A R I A B L E S			      */
/*								      */
/**********************************************************************/

/* ****************** Logging function name strings ***************** */
/* ******************                               ***************** */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1500_upperCtrl				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-15					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed	short int  FAR  PASCAL ro1500_upperCtrl (
unsigned long  int  hCxt)

/*                                                                    */
/* DESCRIPTION	:						      */
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

RM0001_STRG (rs1500, "ro1500_upperCtrl")
rT1530_stAsoContext      *phCxt;
unsigned long  int        hEvData=NULL;
unsigned long  int        hBuf=NULL;

signed	 short int        rc;
signed	 short int        route;
signed	 short int        mode;

signed	 long  int        event;

unsigned long  int        hEcb;          /*memory handle to ECB (for logging) */
unsigned long  int        hAcb;          /*memory handle to ACB (for logging) */

unsigned long  int        p1;
unsigned long  int        p2;
unsigned long  int        p3;
unsigned long  int        p4;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1500)

rc = 0;

/* ******** Step 1 from PM-generic: lock PM context ***************** */
/* ******************           ************************************* */

   /***** if no context is installed, create and initialize one ****/

if (hCxt == 0)
{
    if ( (hCxt = ro0030_localalloc (
                     (unsigned long int) sizeof(rT1530_stAsoContext))) == NULL )
        RM0003_RETURN (rs1500, signed short int, - R_LALLCE) 

    /************  register the new context ************************/

    rc = ro0212_regPmInvoc (hCxt);
    if (rc)
        RM0003_RETURN (rs1500, signed short int, rc)
} 

if ( (phCxt = (rT1530_stAsoContext  *) ro0031_locallock (hCxt)) == NULL)
    RM0003_RETURN (rs1500, signed short int, - R_LLOCKE) 

/* ****************** Pop incomming event *************************** */
/* ******************			  *************************** */

event = ro0210_popInEvent (&p1, &p2, &p3, &p4);

/* ****************** PM activity logging *************************** */

/* ******* compute the application id (ECB handle) and the ********** */
/* ******* association id (ACB handle) used for the PDU logging ***** */

   hAcb = ro0214_getAcbEcb (&hEcb);

   ro0043_pack10Log (
    RC0022_ACTIVITY,
    RL1501_ActivityUctr,
    8,
    (signed short int) sizeof(hEcb),
    (char FAR *)       &hEcb,
    (signed short int) sizeof(hAcb),
    (char FAR *)       &hAcb,
    (signed short int) sizeof(p1),
    (char FAR *)       &p1,
    (signed short int) sizeof(p2),
    (char FAR *)       &p2,
    (signed short int) sizeof(p3),
    (char FAR *)       &p3,
    (signed short int) sizeof(p4),
    (char FAR *)       &p4,
    (signed short int) sizeof(event),
    (char FAR *)       &event,
    (signed short int) sizeof(rT1530_stAsoContext),
    (char FAR *)       phCxt);

/* ****************** Check wheter it is a valid event or not ******* */
/* ****************** and compute actions.		      ******* */

switch (event) {

  /* **************** Change state to active and save the  ********** */
  /* **************** received buffers in context block to ********** */  
  /* **************** be used during cofirmation, if  not  ********** */  
  /* **************** received any, get pro. buffer during cnf. ***** */  

case RC0701_RO_BINDreq   : 
                         if (p2)  
                         {  
                             rc = ro1520_CreateEvData (p2,
                                             (unsigned long int FAR *)&hEvData);
                             if (rc) break;
                                 phCxt->hAsscHandle  = hEvData;
                         }
                         /* set association handle to NULL,so as to   */
                         /* provider buffer during confirmation       */
                         else
                            phCxt->hAsscHandle = 0;
                         route = RC0219_Rose; 
                         break;

case RC0705_RO_UNBINDreq : 
                         if (p3)
			 {
                             rc = ro1520_CreateEvData (p3,
                                            (unsigned long int FAR *)&hEvData);
                             if (rc) break;
                             phCxt->hAsscHandle  = hEvData;
                         }
                         else
                             phCxt->hAsscHandle = 0;
                         route = RC0219_Rose; 
                         break;

case RC0702_RO_BINDind   :
case RC0706_RO_UNBINDind :
case RC070d_RO_INVind    :
                         mode = 1; 
                         if (p1)
                         {
                            ro0216_getPrBuffer (&hBuf);
                            rc = ro1520_CreateEvData (hBuf,
                                            (unsigned long int FAR *)&hEvData);
                            if  (rc) break;
		            rc = ro1522_UserDataCp(p1,hEvData);
                            if (rc) break;                                      
                            p1 = hEvData;
                         }
			 route = RC0221_RoseUser; 
			 break;

case RC070a_RO_UABORTind  :
case RC070b_RO_PABORTind:
                         mode = -1; 
                         if (p1)
                         {
                             ro0216_getPrBuffer (&hBuf);
                             rc = ro1520_CreateEvData (hBuf,
                                            (unsigned long int FAR *)&hEvData);
                             if  (rc) break;
		             rc = ro1522_UserDataCp (p1,hEvData);
                             if (rc) break;
                             p1 = hEvData;
                         }
			 route = RC0221_RoseUser; 
			 break;

case RC0704_RO_BINDcnf   :  
                         if (p4 & 0x00fff0) mode = -1;
			 else               mode =  1;
                         if  ((!phCxt->hAsscHandle) && (p1)) {
                               ro0216_getPrBuffer (&hBuf);
                               rc = ro1520_CreateEvData (hBuf,
                                            (unsigned long int FAR *)&hEvData);
 			       if (rc) break;
     		         }
		         else {
                             hEvData = phCxt->hAsscHandle ;
                             phCxt->hAsscHandle = LNULL;
                         } 
		         rc=ro1522_UserDataCp(p1,hEvData);
                         if (rc) break;
                         p1 = hEvData;
			 route = RC0221_RoseUser; 
			 break;

case RC0708_RO_UNBINDcnf :  
		         mode = -1;
                         if  ((!phCxt->hAsscHandle)&&(p1)) {
                                ro0216_getPrBuffer (&hBuf);
                                rc = ro1520_CreateEvData (hBuf,
                                            (unsigned long int FAR *)&hEvData);
 			        if (rc) break;
     		         }
		         else {
                             hEvData=phCxt->hAsscHandle;
                             phCxt->hAsscHandle = LNULL;
                         }
		         rc=ro1522_UserDataCp(p1,hEvData);
                         if (rc) break;
                         p1 = hEvData;
			 route = RC0221_RoseUser;
			 break;

case RC070c_RO_INVreq    :
 			 if (p2)
		         {  
                             rc = ro1521_CreateIvkEle (p4, p2,
                                              (rT1530_stAsoContext FAR *)phCxt);
                             if (rc) break;
                         }
		         route = RC0219_Rose;  
                         break;               

case RC070f_RO_RESind    :
case RC070h_RO_ERRind    :
/* kc on 14-9-93 
case RC070k_RO_RJPind    : */
			 mode = 1; 
 			 rc = ro1525_SearchIvkEle (p4, phCxt,
                                            (unsigned long int FAR *)&hEvData);
                         if ( (rc != -R_UBUFER) && (rc != 0))
		                 break;	
                         else if ((rc == -R_UBUFER)&&(p1)) {
                                 ro0216_getPrBuffer (&hBuf);
                                 rc = ro1520_CreateEvData (hBuf,
                                            (unsigned long int FAR *)&hEvData);
 			         if (rc) break;
     		         }
		         rc=ro1522_UserDataCp(p1,hEvData);
                         if (rc) break;
                         p1 = hEvData;
			 route = RC0221_RoseUser; 
			 break;
                          
case RC070j_RO_RJUind    :     /* no user data */
case RC070k_RO_RJPind    : /* kc on 14-9-93 */ 
			 mode = 1; 
 			 rc = ro1525_SearchIvkEle (p4, phCxt,
                                            (unsigned long int FAR *)&hEvData);
                         if (!rc) { 
                             rc = ro0221_pushOnInterface ( 0, RC0221_RoseUser, 
                                        RC1501_RO_BUFFERind, 0, 0, hEvData, 0);
                             if (rc) break; 
                         } /* if no user data & buffer is found then return
			        buffer */
                         if ( (rc != -R_UBUFER) && (rc != 0)) break;
			 route = RC0221_RoseUser;
			 break;

case RC0703_RO_BINDrsp   : 
case RC0707_RO_UNBINDrsp :
case RC0709_RO_UABORTreq : 
case RC070e_RO_RESreq    :
case RC070g_RO_ERRreq    :
case RC070i_RO_RJUreq    : 
		         route = RC0219_Rose;
		         break;
} /* end switch event */

/* ****************** Push rtpm service event into user event ******* */
/* ****************** queue or route forward to rtpm	      ******* */

if (ro0032_localunlock (hCxt)) rc = -R_LULOKE;    

if (rc == 0  || rc  == -R_UBUFER) {
    switch (route) {
      case RC0219_Rose    :
          rc = ro0211_pushOutEvent (route, event, p1, p2, p3, p4);
          break;

      case RC0221_RoseUser:
          rc = ro0221_pushOnInterface (mode, route, event, p1, p2, p3, p4);
          break;

      default:
          break;
    }/* end switch route */

   if (rc) RM0003_RETURN (rs1500, signed short int, rc)

   if (mode == -1) {
       rc = ro1501_cleanUp (0, hCxt,0,0,0);
    /* Added by kc on 17/5/93 */
       ro0213_regPmDestr();
   }
} /* if  no rc */

RM0003_RETURN (rs1500, signed short int, rc)
}/*end*/




/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME 	: ro1501_cleanUp				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-04-01					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short	int PASCAL  ro1501_cleanUp (

       signed	long  int   event,
       unsigned long  int p1Pmcxt,
       unsigned long  int     p2c,
       unsigned long  int     p3c,
       unsigned long  int     p4c)

/*                                                                    */
/* DESCRIPTION	:						      */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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

RM0001_STRG (rs1501, "ro1501_cleanUp  ")
signed	 short int     eCount;
signed	 short int     p3mflg;
unsigned long  int *phAddress;
unsigned long  int *phAeTitle;
char	 NEAR		*ppc3;
rT1530_stAsoContext     *phCxt;
unsigned long  int       p1;
unsigned long  int       p2;
unsigned long  int       p3;
unsigned long  int       p4;
signed	 short int       mode;
signed	 short int       route;
signed	 short int       rc = OK;
rT1531_stInvokeData     *phInvokeData; 
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs1501)

/* ****************** To free context memory ************************ */
/* ******************		             ************************ */

if (! event)
{ 
unsigned long  int       hCur;
unsigned long  int       hPrev;
rT1531_stInvokeData FAR *phPrev;
rT1531_stInvokeData FAR *phCur;

   if ( (phCxt = (rT1530_stAsoContext  *) ro0031_locallock (p1Pmcxt)) == NULL)
      RM0003_RETURN (rs1501, signed short int, - R_LLOCKE) 
 
   if (phCxt->hAsscHandle)
   {
       mode = 0;
       route = RC0221_RoseUser;
       event = RC1501_RO_BUFFERind;
       p1=p2=p4=0;
       p3 = phCxt->hAsscHandle;
       rc = ro0221_pushOnInterface (mode, route, event, p1, p2, p3, p4);

       if (ro0032_localunlock (phCxt->hAsscHandle) != 0)                 
         RM0003_RETURN (rs1501, signed short int, -R_LULOKE)
   } /* end  association handle found */
 
   if (phCxt->hOperHandle)
   { 
       if ( (phInvokeData = (rT1531_stInvokeData *)
                               ro0031_locallock (phCxt->hOperHandle)) == NULL)
           RM0003_RETURN (rs1501, signed short int, - R_LLOCKE)
                                                                               
      /* ******** search for evdata handle meant for this  *********** */ 
      /* ******** particular invoke id & get a pointer for it ******** */    

       hCur   = phCxt->hOperHandle ;
       phCur  = (rT1531_stInvokeData FAR *)phInvokeData ;
       phPrev = (rT1531_stInvokeData FAR *)NULL     ;
       hPrev  = NULL     ;
     
       while (phCur->hNext > 0) {
          rc = ro0221_pushOnInterface ( 0, RC0221_RoseUser, RC1501_RO_BUFFERind,
                                                       0, 0, phCur->hEvData, 0);
          if (rc) break; 

          rc = ro1526_MovePtrsOnePos ((unsigned long int   FAR       *)&hPrev,
                                      (unsigned long int   FAR       *)&hCur,
                                      (rT1531_stInvokeData FAR * FAR *)&phPrev,
                                      (rT1531_stInvokeData FAR * FAR *)&phCur);
          if (rc) break; 

          if      (ro0032_localunlock (hPrev)) rc = -R_LULOKE;
          else if (ro0034_localfree   (hPrev)) rc = -R_LFREEE;
          if (rc) break; 
          hPrev  = LNULL;
          phPrev = LNULL;
       } 

       if (rc) RM0003_RETURN (rs1501, signed short int, rc)

       rc = ro0221_pushOnInterface ( 0, RC0221_RoseUser, RC1501_RO_BUFFERind,
                                                       0, 0, phCur->hEvData, 0);
       if      (ro0032_localunlock (hCur)) rc = -R_LULOKE;
       else if (ro0034_localfree   (hCur)) rc = -R_LFREEE;
       if (rc) RM0003_RETURN (rs1501, signed short int, rc)

   } /* if operation handle found */

   if      (ro0032_localunlock (p1Pmcxt)) rc = -R_LULOKE;
   else if (ro0034_localfree   (p1Pmcxt)) rc = -R_LFREEE;

RM0003_RETURN (rs1501, signed short int, rc)
}/* end if call for context cleaning */

/* ****************** Clean up p1 and p3 **************************** */
/* ******************			 **************************** */

eCount = 0;
p3mflg = 0;

/* ****************** Clean up pdvTree (p1) ************************* */
/* ******************			    ************************* */

switch (event)	  {
   case RC0701_RO_BINDreq	 :
   case RC0702_RO_BINDind	 :
   case RC0703_RO_BINDrsp	 :
   case RC0704_RO_BINDcnf	 :
	p3mflg = 1;
	if (ro0246_clearPdvTree (&p1Pmcxt)) eCount++;
	break;

   case RC0705_RO_UNBINDreq      :
   case RC0706_RO_UNBINDind      :
   case RC0707_RO_UNBINDrsp      :
   case RC0708_RO_UNBINDcnf      :
   case RC0709_RO_UABORTreq       :
   case RC070a_RO_UABORTind       :
   case RC070b_RO_PABORTind     :
	if (ro0246_clearPdvTree (&p1Pmcxt)) eCount++;
	break;

   case RC070c_RO_INVreq         : 
   case RC070d_RO_INVind         : 
   case RC070e_RO_RESreq         : 
   case RC070f_RO_RESind         : 
   case RC070g_RO_ERRreq         : 
   case RC070h_RO_ERRind         : 
   case RC070i_RO_RJUreq         : 
   case RC070k_RO_RJPind         : 
   case RC070j_RO_RJUind         :
        p3mflg = 2;
	if (ro0246_clearPdvTree (&p1Pmcxt)) eCount++;
	break;

}

if (! p3mflg) {
    RM0003_RETURN (rs1501, signed short int, ((eCount) ? -R_FCLEAR : 0))
}

/* ****************** Clean up p3 and p3 substructures ************** */
/* ******************				       ************** */

if (p3mflg == 1)
{
ppc3 = ro0031_locallock (p3c);

if (ppc3) {
       switch (event)	 {
	  case RC0701_RO_BINDreq     :	{
	       rT0701_stBindReqRopm  NEAR *pBindReqRopm;

	       pBindReqRopm =  (rT0701_stBindReqRopm *) ppc3;
	       phAddress    = &(pBindReqRopm->stAll.hCalledAddress);
	       phAeTitle    = &(pBindReqRopm->stAcpm.hAeTitle);
	       break;
	       }

	  case RC0702_RO_BINDind     :	{
	       rT0702_stBindIndRopm  NEAR *pBindIndRopm;

	       pBindIndRopm = (rT0702_stBindIndRopm *) ppc3;
	       phAddress     = &(pBindIndRopm->stAll.hCallingAddress);
	       phAeTitle     = &(pBindIndRopm->stAcpm.hAeTitle);
	       break;
	       }

	  case RC0703_RO_BINDrsp     :	{
	       rT0703_stBindRspRopm  NEAR *pBindRspRopm;

	       pBindRspRopm =  (rT0703_stBindRspRopm *) ppc3;
	       phAddress    = &(pBindRspRopm->stAll.hRespondAddress);
	       phAeTitle    = &(pBindRspRopm->stAcpm.hAeTitle);
	       break;
	       }

	  case RC0704_RO_BINDcnf     :	{
	       rT0704_stBindCnfRopm  NEAR *pBindCnfRopm;

	       pBindCnfRopm  =	(rT0704_stBindCnfRopm *) ppc3;
	       phAddress     = &(pBindCnfRopm->stAll.hRespondAddress);
	       phAeTitle     = &(pBindCnfRopm->stAcpm.hAeTitle);
	       break;
	       }
        } /* end switch event */
  
} else {
       eCount++;
}
} /* if p3mflag == 1 */

if (*phAddress) if (ro0034_localfree	 (*phAddress)) eCount++;
if (*phAeTitle) if (ro0009_deleteAETitle (phAeTitle )) eCount++;
if (ppc3     ) ro0032_localunlock (p3c);
if (ro0034_localfree (p3c))  eCount++;

RM0003_RETURN (rs1501, signed short int, ((eCount) ? -R_FCLEAR : 0))
}/*end*/
