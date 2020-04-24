/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rolctr0.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:25  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:33  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:36  keutel
 * 	OT 11620
 * 	[1994/08/19  13:47:19  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:45  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:54  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:56  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:50:23  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:21  keutel
 * 	creation
 * 	[1994/03/21  13:19:52  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rolctr0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:31 $" } ;
#endif

/****************************************************************************\
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
/* NAME 	: rolcrt0.c					      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-26					      */
/*                                                                    */
/* COMPONENT	: RTS association lower control function	      */
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
/* 2.00 K0  | 92-01-25|  original		       |vp  |	      */
/*	    |	      | 			       |    |	      */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) rolctr0.c 1.3 94/01/19   RTS-V3.00";

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

#include <routil0.h>	 /* RTS utilities interface		     */
#include <routil1.h>	 /* RTS utilities interface (AL-types)	     */
#include <rodpat0.h>	 /* RTS dispatcher pm interface 	     */

#include <roacpm0.h>	 /* RTS acpm  eventing interface.	     */
#include <roppm00.h>	 /* RTS ppm  eventing interface.	     */

#include <rolctr2.h>	 /* RTS apma ape invocation context.	     */
#include <rolctr3.h>	 /* RTS apma ape invocation context.	     */
#include <rouctr4.h>	 /* RTS apma ape invocation context.	     */
#include <rolctrz0.h>	 /* RTS apma ape invocation context.	     */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0500_lowerCtrl				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-15					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed	 short int PASCAL ro0500_lowerCtrl (
unsigned long  int hCxt)

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

RM0001_STRG (rs0500, "ro0500_lowerCtrl")
signed	 short int     rc;
signed	 short int  route;
signed	 short int  route1=0;
signed	 long  int except;

signed	 long  int event;
signed	 long  int event1=0;
unsigned long  int    p1;
unsigned long  int    p2;
unsigned long  int    p3;
unsigned long  int    p4;
rTro7e_stAssocReqAcpm NEAR  *pp3Req;
rTro7f_stAssocIndAcpm NEAR  *pp3Ind;
rTro7h_stAssocCnfAcpm NEAR  *pp3Cnf;

signed   short int     sAplId;
rT0530_stCxt          *phCxt;

unsigned long  int        hEcb;          /*memory handle to ECB (for logging) */
unsigned long  int        hAcb;          /*memory handle to ACB (for logging) */

unsigned long  int         hApl; 
rT0501_AEInvocation  NEAR *phApl; 
unsigned short int        usAseConfig;
signed  short  int        bAplCxtRes;

signed short int          bAbortFlg = FALSE;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0500)

rc    = 0;
except = 0;

/* ****************** Lock & Register PM Context ******************** */
/* *********************** New  PM Context ************************** */
                                                                             
  if (hCxt == 0)                                                         
  {                                                                          
    if ( (hCxt = ro0030_localalloc (                                     
                     (unsigned long int) sizeof(rT0530_stCxt)))          
                     == NULL )                                               
      RM0003_RETURN (rs0500, signed short int, - R_LALLCE) 

    if ( (phCxt = (rT0530_stCxt *) ro0031_locallock (hCxt)) == NULL) 
    {                                                                           
      ro0034_localfree (hCxt);    
      RM0003_RETURN (rs0500, signed short int, - R_LLOCKE)
    }                                                                           
    rc = ro0212_regPmInvoc (hCxt);                                          
    if (rc)                                                                     
    {                                                                           
      if (ro0032_localunlock (hCxt) != 0)                    
       RM0003_RETURN (rs0500, signed short int, -R_LULOKE)  
      if (ro0034_localfree (hCxt) !=0)                       
       RM0003_RETURN (rs0500, signed short int, -R_LFREEE)  
      RM0003_RETURN (rs0500, signed short int, rc)           
    }                                                                           
    phCxt->sState = RC0530_Idle;           
 
/* ****************** Register new PM Context *********************** */
  }                                                                             
  else                                                                          

/* ****************** Lock & Register PM Context ******************** */
/* *******************Existing  PM Context ************************** */

    if ( (phCxt = (rT0530_stCxt *) ro0031_locallock (hCxt)) == NULL) 
      RM0003_RETURN (rs0500, signed short int, - R_LLOCKE) 


/* ****************** Pop incomming event *************************** */
/* ******************			  *************************** */

event = ro0210_popInEvent (&p1, &p2, &p3, &p4);


/* ****************** PM activity logging *************************** */

/* ******* compute the application id (ECB handle) and the ********** */
/* ******* association id (ACB handle) used for the PDU logging ***** */

   hAcb = ro0214_getAcbEcb (&hEcb);

   ro0043_pack10Log (
    RC0022_ACTIVITY,
    RL0501_ActivityLctr,
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
    (signed short int) sizeof(rT0530_stCxt),
    (char FAR *)       phCxt);

/* ****************** Check wheter it is a valid event or not ******* */
/* ****************** and compute actions.		      ******* */

switch (event) {
  /* **************** Change state to active and ******************** */
  /* **************** forward event to acpm	 ******************** */

case RC0601_AA_ASSOCIATEreq	:  phCxt->sState = RC0531_Active;

     /* *********** store Appln Context Id in Context block ****** */
     /* ************** required for comparing in confirmation **** */

     pp3Req  = (rTro7e_stAssocReqAcpm NEAR *) ro0031_locallock  (p3);               
       if (! pp3Req )              
         RM0003_RETURN (rs0500, signed short int, -R_LALLCE)

     phCxt->sApCxtId = pp3Req->stAcpm.sApplContextId ;

     if(ro0032_localunlock (p3)!=0)
         RM0003_RETURN (rs0500, signed short int, -R_LULOKE)

     /* ************** Find route and store in Context *********** */
     /* ************** block for further events to use it ******** */

         if (p4)
              phCxt->usAutNo = RC0219_Rose;
         else
              phCxt->usAutNo = RC0218_Rtse;

     p4 = 0;
     route = RC0216_Acse;
     break;

case RC0603_AA_ASSOCIATErsp  :  

     /* ************* Change state to idle in the case of reject **** */
     /* ************* and forward event to rtpm 		 **** */
     /* ************* this  would also take care of redirection  **** */
     /* ************* since there is no PDU here                 **** */

     if ((p4 & 0x00f0) != RC0620_Accepted) 
     {
      phCxt->sState = RC0530_Idle;
     }
	route = RC0216_Acse;
	break;

case RC0602_AA_ASSOCIATEind	:  
     if (! p4) 
     {
       phCxt->sState = RC0531_Active;

     /* *********** store Appln Context Id in Context block ****** */
     /* *********** required for get inform function called ****** */
     /* *********** from external  interface                ****** */

     pp3Ind  = (rTro7f_stAssocIndAcpm NEAR *) ro0031_locallock  (p3);               
       if (! pp3Ind )              
         RM0003_RETURN (rs0500, signed short int, -R_LLOCKE)

     sAplId = phCxt->sApCxtId = pp3Ind->stAcpm.sApplContextId ;

     if(ro0032_localunlock (p3)!=0)
         RM0003_RETURN (rs0500, signed short int, -R_LULOKE)

     /* ************** Find route and store in Context *********** */
     /* ************** Find route and store in Context *********** */
     /* ************** block for further events to use it ******** */
 
       rc = ro0215_getEiCxt ( RC0233_ApplicationEiCxt, &hApl );   
         if (rc) 
         {
           rc = -R_AAPLCX;
           RM0003_RETURN (rs0500, signed short int, rc )
         }                               
         else     
         {
          phApl = (rT0501_AEInvocation NEAR *) ro0031_locallock ( hApl); 
          if (! phApl)
          RM0003_RETURN (rs0500, signed short int, -R_LALLCE)

          usAseConfig = phApl->vApplCtxtInfos[sAplId].usAseConfig;
          if (usAseConfig == RC0244_SwRoseOnly)
           phCxt->usAutNo = route = RC0219_Rose;
	  else
           phCxt->usAutNo = route = RC0218_Rtse;

/* ****** Added by kc on 12/7/93 to check the modeselector *********  */
/* ** start ** */
/*           if ( phApl->vApplCtxtInfos[sAplId].lModeSelector !=
                   pp3Ind->stAll.lModeSelector )
              bAbortFlg = TRUE;  K02 to accept PTS */
          ro0032_localunlock (hApl);
          if(ro0032_localunlock (hApl)!=0)
           RM0003_RETURN (rs0500, signed short int, -R_LULOKE)
          if (bAbortFlg) goto give_abort;
	 }
     }/* if no P4 */
     else 
     {
       /* clean up event */
give_abort:
       rc = ro0501_cleanUp (event, p1, p2, p3, p4);  
       /* clean up context */
       /* cleanup context is not required. by kc on 14/05 */
/*       rc = ro0501_cleanUp (0, hCxt, 0, 0, 0); */
       p1    = p2 = p3 = p4 = 0;
       event = RC0609_AA_ABORTreq;
       route = RC0216_Acse;
      }
     break;

case RC0604_AA_ASSOCIATEcnf :  
     if (p4 & 0x00fff0) 
     {
      phCxt->sState = RC0530_Idle;
      route = phCxt->usAutNo ;                                           
     }
     else {
         pp3Cnf  = (rTro7h_stAssocCnfAcpm NEAR *) ro0031_locallock  (p3);
         if (! pp3Cnf )                                                 
          RM0003_RETURN (rs0500, signed short int, -R_LLOCKE)         
         rc = ro0215_getEiCxt ( RC0233_ApplicationEiCxt, &hApl ); 
         if (rc)                                           
         {                                                 
           rc = -R_AAPLCX;                                 
           RM0003_RETURN (rs0500, signed short int, rc )   
         }                                                                                                                         
         rc = ro000c_AplCxtCmp (phCxt->sApCxtId, pp3Cnf->stAcpm.sApplContextId,    
                                hApl, (signed short int FAR *)&bAplCxtRes); 
         if(bAplCxtRes == FALSE)
         {                                                                  
            /* clean up event */
            rc = ro0501_cleanUp (event, p1, p2, p3, p4);  
            /* clean up context */
   /*         rc = ro0501_cleanUp (0, hCxt, 0, 0, 0); */
            p1 = p2 = p3 = p4 = 0;                     
            event = RC060b_AA_PABORTind;                                    
            route = phCxt->usAutNo ;                                           
            p4    = RC0627_UnsupApplCtxt;                                   
            event1 = RC0609_AA_ABORTreq ;                                   
            route1 = RC0216_Acse;                                        
  /* by kc on 20-12-93
            if(ro0032_localunlock (p3)!=0)
             RM0003_RETURN (rs0500, signed short int, -R_LULOKE) */
            break;                                                          
         }                                                                  
         else                                                               
         {                                                                  
            route = phCxt->usAutNo;                                            
            if(ro0032_localunlock (p3)!=0)
             RM0003_RETURN (rs0500, signed short int, -R_LULOKE)
            break;                                                          
         }                                                                  
      }
      break;
case RC0605_AA_RELEASEreq	:  route = RC0216_Acse;
				   break;

  /* **************** Change state to idle if (result == ************ */
  /* **************** RCro90_Affirmative) and forward	 ************ */
  /* **************** event to acpm			 ************ */

case RC0607_AA_RELEASErsp	:  if (p4 == RCro90_Affirmative) {
				      phCxt->sState = RC0530_Idle;
				   }
				   route = RC0216_Acse;
				   break;


case RC0606_AA_RELEASEind	:  route = phCxt->usAutNo;
				   break;

  /* **************** Change state to idle if (result == ************ */
  /* **************** RCro90_Affirmative) and forward	 ************ */
  /* **************** event to rtpm			 ************ */

case RC0608_AA_RELEASEcnf	:  if (p4 == RCro90_Affirmative) {
				      phCxt->sState = RC0530_Idle;
				   }
				   route = phCxt->usAutNo;
				   break;


case RC0609_AA_ABORTreq	:  phCxt->sState = RC0530_Idle;
				   route = RC0216_Acse;
				   break;

case RC060a_AA_ABORTind	:
case RC060b_AA_PABORTind	:  phCxt->sState = RC0530_Idle;
				   route =phCxt->usAutNo;
				   break;

  /* **************** Forward event to ppm ************************** */
  /* ****************			   ************************** */

  case RC040c_PDATAreq		:
  case RC040e_PTOKEN_PLEASEreq	:
  case RC040g_PCONTROL_GIVEreq	:
  case RC040i_PSYNC_MINORreq	:
  case RC040k_PSYNC_MINORrsp	:
  case RC040p_PACT_STARTreq	:
  case RC040r_PACT_RESUMEreq	:
  case RC040t_PACT_ENDreq	:
  case RC040v_PACT_ENDrsp	:
  case RC040x_PACT_INTERRUPTreq :
  case RC040z_PACT_INTERRUPTrsp :
  case RC040B_PACT_DISCARDreq	:
  case RC040D_PACT_DISCARDrsp	: route = RC0215_Presentation;
				  break;

  /* **************** Forward event to ppm  and signal ************** */
  /* **************** exception report to acpm.        ************** */

  case RC040m_PUEXCEPT_REPORTreq: route = RC0215_Presentation;
				  except = RC060d_AA_EXTERN_2;
				  break;

  /* **************** Forward event to ropm/rtpm ******************** */
  /* ****************			         ******************** */

  case RC040d_PDATAind		:
  case RC040f_PTOKEN_PLEASEind	:
  case RC040h_PCONTROL_GIVEind	:
  case RC040j_PSYNC_MINORind	:
  case RC040l_PSYNC_MINORcnf	:
  case RC040q_PACT_STARTind	:
  case RC040s_PACT_RESUMEind	:
  case RC040u_PACT_ENDind	:
  case RC040w_PACT_ENDcnf	:
  case RC040y_PACT_INTERRUPTind :
  case RC040A_PACT_INTERRUPTcnf :
  case RC040C_PACT_DISCARDind	:
  case RC040E_PACT_DISCARDcnf	:
  case RC040F_PCONTINUE 	:
  case RC040G_PSYNCPOINT	: route = phCxt->usAutNo;
				  break;

  /* **************** Forward event to rtpm and signal ************** */
  /* **************** exception report to acpm.        ************** */

  case RC040n_PUEXCEPT_REPORTind:
  case RC040o_PPEXCEPT_REPORTind: route = phCxt->usAutNo;
				  except = RC060c_AA_EXTERN_1;
				  break;

  /* **************** Unkwon incomming event ************************ */
  /* ****************			     ************************ */

  default			: rc = -R_ACUNEV;
				  break;
}

/* ****************** Route service event forward to acpm, ppm ****** */
/* ****************** rtpm or ropm			       ****** */

if ((! rc) && (!except))  {
    rc = ro0211_pushOutEvent (route, event, p1, p2, p3, p4);
    if ( (event1) && (route1) )
    rc = ro0211_pushOutEvent (route1, event1, p1, p2, p3, p4); 
}

/* ****************** Signal execption reports to acpm ************** */
/* ******************				       ************** */

if ((! rc) && (except)) {
    rc = ro0211_pushOutEvent (RC0216_Acse, except, 0, 0, 0, 0);
}

if (phCxt->sState == RC0530_Idle)  
  {                                      
/* ****************** deregister and free the context  ************** */ 
/* ******************                                  ************** */ 

    if(ro0032_localunlock (hCxt)!=0)
         RM0003_RETURN (rs0500, signed short int, -R_LULOKE)
    if(ro0034_localfree (hCxt)!=0)          
         RM0003_RETURN (rs0500, signed short int, -R_LFREEE)
    ro0213_regPmDestr ();                
  }                                      
  else                                   
/* ****************** deregister and free the context  ************** */ 
/* ******************                                  ************** */ 

    if(ro0032_localunlock (hCxt)!=0)
         RM0003_RETURN (rs0500, signed short int, -R_LULOKE)

RM0003_RETURN (rs0500, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME 	: ro0501_cleanUp				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-30					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short	int PASCAL  ro0501_cleanUp (
       signed	long  int    event,
       unsigned long  int p1PmhCxt,
       unsigned long  int      p2c,
       unsigned long  int      p3c,
       unsigned long  int      p4c)

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

RM0001_STRG (rs0501, "ro0501_cleanUp  ")
signed	 short int     eCount;
signed	 short int     p3mflg;
unsigned long  int *phAddress;
unsigned long  int *phCldAddr;
unsigned long  int *phAeTitle;
char	 NEAR		*ppc3;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0501)

/* ****************** Local  Variables  ***************************** */
/* ******************					************* */

eCount = 0;
p3mflg = 0;

/* ****************** To free context memory ************************ */
/* ******************					************* */

if (! event) 
{
     if (ro0034_localfree (p1PmhCxt))                              
      eCount++;                      /*increase the error counter */ 

RM0003_RETURN (rs0501, signed short int, eCount)
} /*end of cleanup for  context */
else
{

/* ****************** Clean up p1 and p3 **************************** */
/* ******************			 **************************** */

/* ****************** Clean up pdvTree (p1) ************************* */
/* ******************			    ************************* */

switch (event)	  {

   case RC0601_AA_ASSOCIATEreq	 :
   case RC0602_AA_ASSOCIATEind	 :
   case RC0603_AA_ASSOCIATErsp	 :
   case RC0604_AA_ASSOCIATEcnf	 :

	p3mflg = 1;
	if (ro0246_clearPdvTree (&p1PmhCxt)) eCount++;
	break;

   case RC0605_AA_RELEASEreq	 :
   case RC0606_AA_RELEASEind	 :
   case RC0607_AA_RELEASErsp	 :
   case RC0608_AA_RELEASEcnf	 :
   case RC0609_AA_ABORTreq	 :
   case RC060a_AA_ABORTind	 :

   case RC040c_PDATAreq 	 :
   case RC040d_PDATAind 	 :
   case RC040e_PTOKEN_PLEASEreq  :
   case RC040f_PTOKEN_PLEASEind  :
   case RC040g_PCONTROL_GIVEreq  :
   case RC040h_PCONTROL_GIVEind  :
   case RC040i_PSYNC_MINORreq	 :
   case RC040j_PSYNC_MINORind	 :
   case RC040k_PSYNC_MINORrsp	 :
   case RC040l_PSYNC_MINORcnf	 :
   case RC040m_PUEXCEPT_REPORTreq:
   case RC040n_PUEXCEPT_REPORTind:
   case RC040p_PACT_STARTreq	 :
   case RC040q_PACT_STARTind	 :
   case RC040r_PACT_RESUMEreq	 :
   case RC040s_PACT_RESUMEind	 :
   case RC040t_PACT_ENDreq	 :
   case RC040u_PACT_ENDind	 :
   case RC040v_PACT_ENDrsp	 :
   case RC040w_PACT_ENDcnf	 :
   case RC040x_PACT_INTERRUPTreq :
   case RC040y_PACT_INTERRUPTind :
   case RC040z_PACT_INTERRUPTrsp :
   case RC040A_PACT_INTERRUPTcnf :
   case RC040B_PACT_DISCARDreq	 :
   case RC040C_PACT_DISCARDind	 :
   case RC040D_PACT_DISCARDrsp	 :
   case RC040E_PACT_DISCARDcnf	 :

	if (ro0246_clearPdvTree (&p1PmhCxt)) eCount++;
	break;
}

/* ****************** Clean up pdvTree or tpdu description ********** */
/* ****************** (p1)				   ********** */

switch (event)	  {
   case RC040p_PACT_STARTreq  :
   case RC040q_PACT_STARTind  :
   case RC040r_PACT_RESUMEreq :
   case RC040s_PACT_RESUMEind : if (ro0034_localfree (p3c)) eCount++;
				break;
}

if (! p3mflg) {
    RM0003_RETURN (rs0501, signed short int, ((eCount) ? -R_FCLEAR : 0))
}

/* ****************** Clean up p3 and p3 substructures ************** */
/* ******************				       ************** */

ppc3 = ro0031_locallock (p3c);

if (ppc3) {
       switch (event)	 {

	  case RC0601_AA_ASSOCIATEreq:	{
	       rTro7e_stAssocReqAcpm NEAR *pp3req;

	       pp3req	 =  (rTro7e_stAssocReqAcpm NEAR *) ppc3;
	       phAeTitle = &(pp3req->stAcpm.hAeTitle);
	       phAddress = &(pp3req->stAll.hCalledAddress);
	       break;
	       }

	  case RC0602_AA_ASSOCIATEind:	{
	       rTro7f_stAssocIndAcpm NEAR *pAssocIndAcpm;

	       pAssocIndAcpm =	(rTro7f_stAssocIndAcpm *) ppc3;
	       phAddress     = &(pAssocIndAcpm->stAll.hCallingAddress);
               phCldAddr     = &(pAssocIndAcpm->stAll.hCalledAddress);
	       phAeTitle     = &(pAssocIndAcpm->stAcpm.hAeTitle);
	       break;
	       }

	  case RC0603_AA_ASSOCIATErsp:	{
	       rTro7g_stAssocRspAcpm NEAR *pp3rsp;

	       pp3rsp	 =  (rTro7g_stAssocRspAcpm NEAR *) ppc3;
	       phAeTitle = &(pp3rsp->stAcpm.hAeTitle);
	       phAddress = &(pp3rsp->stAll.hRespondAddress);
	       break;
	       }

	  case RC0604_AA_ASSOCIATEcnf:	{
	       rTro7h_stAssocCnfAcpm NEAR *pAssocCnfAcpm;

	       pAssocCnfAcpm =	(rTro7h_stAssocCnfAcpm *) ppc3;
	       phAddress     = &(pAssocCnfAcpm->stAll.hRespondAddress);
	       phAeTitle     = &(pAssocCnfAcpm->stAcpm.hAeTitle);
	       break;
	       }
       }
} else {
       eCount++;
}

if (*phAddress) if (ro0034_localfree	 (*phAddress)) eCount++;
if (*phCldAddr) if (ro0034_localfree	 (*phCldAddr)) eCount++;
if (*phAeTitle) if (ro0009_deleteAETitle (phAeTitle )) eCount++;
if (ppc3     ) ro0032_localunlock (p3c);
if (ro0034_localfree (p3c))  eCount++;

RM0003_RETURN (rs0501, signed short int, ((eCount) ? -R_FCLEAR : 0))
}/*end of else cleanUp of events */
}/*end*/
   


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0502_informLowerCtrl                              */
/*                                                                    */
/* AUTHOR       : Vinita          				      */
/* DATE         : 93-02-23                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0502_informLowerCtrl  (
unsigned long  int      hCxt,          
signed   short int      sInformType,           
void               FAR *pRequestedInfo)

/* DESCRIPTION :  vinita                                              */
/*                                                                    */
/* Compares  received Receiving Application context Id with calling   */        
/* Application Context Id to make sure receiving abstract syntax array*/   
/* is the subset of calling  abstract syntax array                    */        /*							              */
/* IN-PARAMETERS :                                                    */
/*                                                                    */
/*     hContext              = Lower Control context block(handle)    */
/*                                                                    */
/*     sInformType           = Type of information requested.         */
/*                             Possible values are :                  */
/*                             - RC05z0_AplCxtCmp.                    */
/*								      */
/*     pRequestedInfo        = Pointer to memory containing the data  */
/*                             structure associated with sInformType  */
/*                             TO BE FILLED with the requested info.  */
/*                                                                    */
/* OUT-PARAMETERS :                                                   */
/*     pRequestedInfo        = Pointer to memory containing the data  */
/*                             structure associated with sInformType  */
/*                             FILLED with the requested info.        */
/*                                                                    */
/*                                                                    */
/* PUSHED OUT-PARAMETERS : None.                                      */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/* GLOBAL DATA (read only) : None.                                    */
/*                                                                    */
/* GLOBAL DATA (modified)  : None.                                    */
/*                                                                    */
/* REMARKS :  none                                                    */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro0502_informLowerCtrl*/

/* **************** l o c a l    v a r i a b l e s ******************* */
/* ****************                                ******************* */

RM0001_STRG (pFnctName, "ro0502_informLowerCtrl")
signed   short int          rc=0  ;
rT0530_stCxt               *phCxt ;
unsigned long int           hApl  ;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (pFnctName)

if ( (phCxt = (rT0530_stCxt *) ro0031_locallock (hCxt)) == NULL)
   RM0003_RETURN (pFnctName, signed short int, - R_LLOCKE)        

switch (sInformType) {

/*----------------------------------------------------------------------------*/
/* Information on ACSEAbsId requested.                                     */
/*----------------------------------------------------------------------------*/
case RC05z1_AplCxtCmp : {

rT05z0_stAplCxtCmp FAR * pAplCxt;
pAplCxt = (rT05z0_stAplCxtCmp FAR *)pRequestedInfo;

if ( ro0215_getEiCxt ( RC0233_ApplicationEiCxt, &hApl )) 
   rc = -R_AAPLCX;
if (!rc)
rc = ro000c_AplCxtCmp(phCxt->sApCxtId ,pAplCxt->sApCxtId,
                      hApl,&pAplCxt->bAplCxtCmpRes) ; 

        break;

}/* end case RC05z0_AplCxtCmp */

}/*switch-end*/

    if(ro0032_localunlock (hCxt)!=0) 
    rc = -R_LULOKE;
RM0003_RETURN (pFnctName, signed short int, rc)

}/*end*/


/*EOF */








