/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif01.c,v $
 * Revision 1.1.4.2  1996/03/11  13:21:51  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:38:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:22  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:03  keutel
 * 	OT 11620
 * 	[1994/08/19  13:50:08  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:09:37  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:51:52  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:07  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:01:17  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:38  keutel
 * 	creation
 * 	[1994/03/21  13:30:20  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roexif01.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:51 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1993 BY SIEMENS NIXDORF INFORMATIONS AG	      */
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
/* NAME 	: roexif01.c					      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-02-24					      */
/*                                                                    */
/* COMPONENT    : ROS external interface                              */
/*                                                                    */
/* PRD#/VERS.	: ROS-V3.00					      */
/*                                                                    */
/* DESCRIPTION	:         					      */
/*								      */
/*                                                                    */
/* SYSTEM DEPENDENCE :	none					      */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/*  3.00 K0 | 93-02-24|  original		       | kc |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                     Module Identification                          */
/**********************************************************************/
#ifndef RP0000_MicV0600
/*
static char *sccsid =    "@(#)roexif01.c" SNI_VERSION " RTROS-V03.00";
*/
#endif
/*********************************************************************/
/*              I N C L U D E S                                      */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <ross.h>	 /* ROS external interface		     */
#include <rogenrl.h>	 /* ROS general definitions		     */
#include <roerval.h>	 /* ROS error values			     */
#include <rolgval.h>     /* ROS log values                           */
#include <routil0.h>     /* ROS common utilities                     */
#include <rouply0.h>	 /* ROS common upper layer definitions	     */
#include <rouply1.h>     /* Data structures for service parameters   */

#include <routil1.h>     /* ROS common utilities                     */
#include <rodpat0.h>     /* ROS data structures                      */
#include <rodpat2.h>	 /* ROS dispatcher external interface	     */

#include <roacpz0.h>     /* Acpm Inform data structures              */


#include  <rouctr0.h>     /*  Upper Control Buffer data structures   */
#include  <rouctr4.h>     /*                                         */

#include <rolctrz0.h>    /* Lower Control Inform data structures     */
#include <roropm0.h>     /* service parameter data declaration       */
#include <roropz0.h>     /* Infom Data structures                    */
#include <roppm00.h>     /* Inform data structures                   */
#include <roppm01.h>     /* Inform data structures                   */
#include <roppm04.h>     /* Attach data parameters Pres. Cxt         */
#include <roppmz0.h>     /* Inform data parameters & definitions     */

#include <roexif07.h>    /* Inform data structures                  */
#include <roexif03.h>    /* common definitions for rts compatible   */
#include <roexif04.h>    /* Local Function description               */
#include <roexif08.h>    /* Logging Function Proto TYping          */

/*********************************************************************/
/*              L O C A L  V A R I A B L E S                         */
/*********************************************************************/



/* ****************** Logging function name strings ***************** */
/* ******************                               ***************** */

RM0001_STRG (rs1310, "ro1310_roexcom     ")
RM0001_STRG (rs1311, "ro1311_attach      ")
RM0001_STRG (rs1312, "ro1312_detach      ")
RM0001_STRG (rs1313, "ro1313_AssocFuns   ")
RM0001_STRG (rs1314, "ro1314_Operation   ")
RM0001_STRG (rs1315, "ro1315_provBuf     ")
RM0001_STRG (rs1316, "ro1316_redirect    ")
RM0001_STRG (rs1317, "ro1317_Inform      ")
RM0001_STRG (rs1318, "ro1318_receive     ")


RM0001_STRG (rs131a, "ro131a_setHeader   ")
RM0001_STRG (rs131b, "ro131b_checkApplCxt")
RM0001_STRG (rs131c, "ro131c_storeApInv  ")
RM0001_STRG (rs131d, "ro131d_storeAddress")
RM0001_STRG (rs131e, "ro131e_copyAplTitle")
RM0001_STRG (rs131f, "ro131f_storePresInv")
RM0001_STRG (rs131g, "ro131g_storeUserDat")
RM0001_STRG (rs131h, "ro131h_GetAplCxtNameId")
RM0001_STRG (rs131i, "ro131i_freeUserData")
RM0001_STRG (rs131j, "ro131j_storeBuffer ")
RM0001_STRG (rs131k, "ro131k_CpyOpValue  ")
RM0001_STRG (rs131l, "ro131l_issueRosEven")
RM0001_STRG (rs131m, "ro131m_retrieveAddr")
RM0001_STRG (rs131n, "ro131n_retrieveEvFr")
RM0001_STRG (rs131o, "ro131o_retrieveOpVa")
RM0001_STRG (rs131p, "ro131p_retrieveAeTi")
RM0001_STRG (rs131q, "ro131q_setParameter")
RM0001_STRG (rs131r, "ro131r_GetAscCf    ")
RM0001_STRG (rs131s, "ro131s_CheckLegalOc")
RM0001_STRG (rs131t, "ro131t_GetAbsSynRsl")
RM0001_STRG (rs131u, "ro131u_findAbsId   ")
RM0001_STRG (rs131v, "ro131v_getMode   ")
RM0001_STRG (rs131w, "ro131w_ChkObjId  ")
RM0001_STRG (rs131x, "ro131x_ChkAEtitl ")
RM0001_STRG (rs131y, "ro131y_ChkInAEtitl ")
RM0001_STRG (rs131z, "ro131z_CpyRDN ")
RM0001_STRG (rs132a, "ro132a_CpyAVA ")
RM0001_STRG (rs132b, "ro132b_retrieveRDN")
RM0001_STRG (rs132c, "ro132c_retriveAVA ")
RM0001_STRG (rs132d, "ro132d_retrieveInvocId ")
RM0001_STRG (rs132e, "ro132e_ChkSetofAVAs")


signed short int FAR PASCAL roexlog PROTO ((RO_excopb      FAR * pParlst));
signed short int NEAR PASCAL ro1311_attach PROTO ((
       unsigned long  int              	hScb,
       FARPROC            	        pTpduFct,
       RO_attapb 	   FAR	       *pAttapb));
         
signed short int NEAR PASCAL ro1313_AssocFuns PROTO ((
       unsigned long  int               hScb,
       RO_excopb 	   FAR	       *pParlst));

signed short int NEAR PASCAL ro1314_Operation PROTO ((
       unsigned long  int     	        hScb,
       RO_excopb 	   FAR	       *pParlst));

signed short int NEAR PASCAL ro1315_provBuf PROTO ((
       unsigned long  int     	        hScb,
       RO_pvbfpb 	   FAR	       *pProvBuf));

signed	short int NEAR PASCAL ro1316_redirect  PROTO ((
       unsigned long  int        hScb,
       RO_rdrqpb 	  FAR *  pRdirReq));

signed	short int NEAR PASCAL ro1317_Inform  PROTO ((
       unsigned long  int        hScb,
       RO_inform 	  FAR *  pInform));

signed short int NEAR  PASCAL ro1318_receive  PROTO ((
       unsigned long int        hScb,
       RO_chwtpb 	 FAR  * pChWait));



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131a_setHeader				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL  ro131a_setHeader (
			   RO_fncthd  FAR    *pHead,
			   signed short int  sCode)
/*                                                                    */
/* DESCRIPTION  : This function assigns a value for the reutrn code   */
/*                error calss and error value to a function header.   */
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

RM0002_ENTRY (rs131a)

if  (sCode >= RO_SUCES) {
       pHead->r_retcod = sCode;
       pHead->r_erclas = 0;
       pHead->r_errval = 0;
} else {
       sCode	       = -sCode;
       pHead->r_retcod = sCode % 10;

       switch (pHead->r_retcod) {
       case  1: pHead->r_retcod = RO_CALERR; break;
       case  2: pHead->r_retcod = RO_PRMERR; break;
       case  9: pHead->r_retcod = RO_TMPERR; break;
       default: pHead->r_retcod = RO_PRMERR; break;
       }

       pHead->r_erclas = (sCode / 10) % 10;
       pHead->r_errval = sCode;
}

RM0003_RETURN (rs131a, signed short int, pHead->r_retcod)
}/*end*/



/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro131d_storeAddress                                      */
/*                                                                         */
/* AUTHOR       : Chandrasekar.K.                                          */
/* DATE         : 93-03-02                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int NEAR PASCAL ro131d_storeAddress (
		       char               FAR    *pcSourceAddr   ,
		       unsigned long int          ulSourceAddrLen,
		       unsigned long int  FAR    *phTargetAddr    )

/*                                                                         */
/* DESCRIPTION  : Assigns the memory for the <ulSourceAddrLen> and copies  */
/* from the Source to the memory .                                         */
/*                                                                         */
/* IN-PARAMETERS :  * pcSourceAddr     -Char Ptr to Address string         */
/*                    ulSourceAddrLen  -Address Length                     */
/*                                                                         */
/* OUT-PARAMETERS:  phTargetAddr       -Mem. handle                        */
/*                                                                         */
/* RETURN VALUES :  == 0  success                                          */
/*                  != 0  Error                                            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed short int      rc;
char         NEAR     *pcTargetAddr;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY (rs131d)
rc = 0;

*phTargetAddr = ro0030_localalloc (ulSourceAddrLen);
pcTargetAddr    = (*phTargetAddr) ? ro0031_locallock (*phTargetAddr) : 0;

if (pcTargetAddr) {
    ro0041_memcpy ( (char FAR *)pcTargetAddr, pcSourceAddr, ulSourceAddrLen );

   if (ro0032_localunlock (*phTargetAddr))                  rc = -R_LULOKE;
   else                                                     rc = 0;
} else                                                      rc = -R_LLOCKE;

RM0003_RETURN (rs131d, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1311_attach 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-02-25					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro1311_attach (

       unsigned long  int              	hScb,
       FARPROC            	        pTpduFct,
       RO_attapb 	   FAR	       *pAttapb)

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

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

signed	 short int	 rc;

                                   /* memory handles                  */
unsigned long  int       hMyAddr  = 0;  /* Own presentation Address        */
unsigned long  int       hPresCxt = 0; /* Presentation context            */
unsigned long  int       hApplCxt = 0; /* Application context             */

unsigned long  int       ulEcb    = 0;    /* Ecb array number                */

unsigned long  int       ulDummy  = 0; 

/* ****************** S T A R T ************************************* */ 
/* ******************       ***************************************** */   

RM0002_ENTRY (rs1312)

rc = OK;

/* -----------write interface logging------------------------------------- */   
/* ----------------------------------------------------------------------- */   
ro0043_pack10Log(RC0020_INTERFACE				,
		 RL1303_EnATTACH				,
		 2						,
		 (signed short int) sizeof (RO_attapb)		,
		 (char FAR *)	    pAttapb                     ,

                 
		 (signed short int) pAttapb->r_pradd.r_paddln	,
		 (char FAR *)	    pAttapb->r_pradd.r_paddrf	);






/* ********************* check the input parameters ***************** */
/* *********************                            ***************** */   


/* ****************** Check AE title ******************************** */   
/* ***** If the pointer is NULL then the <aptitl>,<aeequl> are ****** */
/* ***** not present . Since they are OPTIONAL                 ****** */

 rc = ro131y_ChkInAEtitl(&pAttapb->r_aetitl);

/* ***************** Presentation address length  ******************* */   
if (! rc) 
    if ( pAttapb->r_pradd.r_paddln == NULL )       rc = -R_ADDLEN;
    else if ((pAttapb->r_pradd.r_paddln > NULL ) && 
             (pAttapb->r_pradd.r_paddrf == NULL )  )   rc = -R_ADDREF;


/* ****************** Appl Context checking  ************************ */   
    /* Max appl. context */
 
if ( (!rc) && (pAttapb->r_apclst.r_apcnum > RC0570_MaxApplCtxtInfos ) )  
        rc = -R_AAPCXE;
if (! rc) rc = ro131b_checkApplCxt( (RO_apclst FAR *)&pAttapb->r_apclst );


/* ****************** store the attach parameters ******************* */   

if (! rc ) rc = ro131d_storeAddress(pAttapb->r_pradd.r_paddrf,
                                    pAttapb->r_pradd.r_paddln,
                                    (unsigned  long int FAR *)&hMyAddr);

if(! rc) rc = ro131f_storePresInv(&pAttapb->r_apclst,
                                  &hPresCxt);


if (! rc) rc = ro131c_storeApInv (&pAttapb->r_aetitl,
                                  &pAttapb->r_apclst,
                                  &pAttapb->r_invcid,
                                  &hApplCxt);
/* !!! call it aftter complete check */


/* -----------call dispatcher -------------------------------------------- */
/* ----------------------------------------------------------------------- */
/* ---- Here the recInPm,recInEvt (recovery ) are NULL ------------------- */
/* ---- !!! why ulDummy & 0 for EiCxt[2] --------------------------------- */
/* ---- !!! why size of Appl. Cxt 0      --------------------------------- */


if (! rc) {
    rc = ro0260_attach ( hScb, &ulEcb,
			 RC0241_CfRoseOnly,
			 0, 0,
			 &hMyAddr , pAttapb->r_pradd.r_paddln    ,
			 &ulDummy , 0                            ,
			 &hPresCxt, (unsigned long int)
				    sizeof ( rT0441_stEntContext),
			 &hApplCxt, (unsigned long int) 0         );

/* Note: hMyAddr, hPresInv, hApplInv are handles. After the ro0260_attach()*/
/*       call their value is ZERO. This identicates that the handle are    */
/*       working handles of the dispatcher.                                */

    switch  (rc) {
    case  0       : rc = RO_SUCES;   pAttapb->r_aplid = ulEcb; break;
    case  1       : rc = RO_NOT1ST;  pAttapb->r_aplid = ulEcb; break;
    default:				       break;
    }
}

if (rc < 0)  {
       if (hMyAddr ) {
               if (ro0032_localunlock(hMyAddr)!= OK) rc = -R_LULOKE; 
               if (ro0034_localfree (hMyAddr )!= OK) rc = -R_LFREEE;
       }
       if (hPresCxt) {
               if(ro0032_localunlock(hPresCxt)!= OK) rc = -R_LULOKE; 
               if(ro0034_localfree (hPresCxt) != OK) rc = -R_LFREEE;
       }
       if (hApplCxt){
               if (ro0032_localunlock(hApplCxt)!= OK) rc = -R_LULOKE; 
               if (ro0034_localfree (hApplCxt) != OK) rc = -R_LFREEE;
       }
}
RM0003_RETURN (rs1312, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131b_checkApplCxt                                 */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-03-01					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int  NEAR PASCAL ro131b_checkApplCxt ( 
	RO_apclst	    FAR   *pApclst)

/*                                                                    */
/* DESCRIPTION	              :                                       */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :  == 0  Success                        */
/*                               != 0  Failure                        */
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
signed	 short int	   rc;
unsigned         int         i;
unsigned         int         k;
unsigned short int         usOffset;
RO_apcary             FAR  *pApplCxt;
RO_trsary             FAR  *pTransSynAry;
RO_absary             FAR  *pAbsSynAry;
signed short int            bAcseFound=FALSE;

unsigned long             vAcse[] = { 5, 2, 2, 1, 0, 1 };
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131b)

rc = OK;
/* *********** check the Transfer syntaxes ************************** */
if ( pApclst->r_trsnum > RCro02_MaxTransSyn ) rc = -R_HCXTTS;

if (! rc)
    for (i=0,usOffset=0;i<pApclst->r_trsnum; i++,usOffset++){
    /*--- !!!holes check for the trsname to be Empty ( 0 element) ---*/
       pTransSynAry = pApclst->r_trsary + usOffset;
       if(!pTransSynAry) break;
        if ( pTransSynAry->r_trsnam == NULL ) {
             /* --- No TrsSyn so skip ---*/
             --i;continue;
        }     
          if ( ro131w_ChkObjId(pTransSynAry->r_trsnam) ) {
                 rc =  -R_NUTREL;break;
        }
    }


/* *********** check for Abstract syntaxes  ************************* */
if (! rc) if (pApclst->r_absnum > RCro01_MaxAbstrSyn ) rc = -R_PCXTNM;

if (!rc)
    if ( pApclst->r_absnum==0) rc = -R_HNOABS; 

if (! rc )
    for ( i=0,usOffset=0; i<pApclst->r_absnum; i++,usOffset++){
        pAbsSynAry = pApclst->r_absary + usOffset;
        if (!pAbsSynAry) break;
        /* --- !!!holes check for r_absnam Null to skip the empty slot ---*/
        if ( pAbsSynAry->r_absnam == NULL ){
             --i;continue;
        }
    /* *********** check for ObjId of the Abstract syntaxes ************* */
          if(ro131w_ChkObjId(pAbsSynAry->r_absnam)){
                 rc = -R_NUABEL;
                 break;
        }
	if (! ro0003_objcmp ( vAcse, pAbsSynAry->r_absnam ) )bAcseFound=TRUE; 
           
    /* **********  check for max transfer syntaxes per abstract syntax ** */
        if ( pAbsSynAry->r_idtnum > RCro03_MaxTransPerAbstr ) {
            rc = -R_MTPANU; 
            break;
        }

    /* **********   check the idtary element's validity  **************** */
    /* **********   i.e. valid index into Transfer syntax array ********* */
   
    /* !!! why int in RO_apcary & unsigned long in RO_absary    */
      
        for (k=0; (k < pAbsSynAry->r_idtnum); k++){ 
            if ( *(pAbsSynAry->r_idtary + k ) > RCro02_MaxTransSyn){
                rc = -R_TRFIDX;
                break;
            } 
           /* check for the validity of the corresponding OBJID */
            if (ro131w_ChkObjId(
               (pApclst->r_trsary+(*(pAbsSynAry->r_idtary + k)))->r_trsnam
                                )){
                 rc = -R_TRFIDX; break; 
            }
        } 

        
    } /* check for Abstract Syntax is over */
if ((!rc) && (!bAcseFound)) rc = -R_NOACSE;
/* ********** check for Appl. Context Array ************************* */
if (!rc)
    if (pApclst->r_apcnum == 0) rc = -R_APLINV;

if (! rc )
    for (i = 0 ; i < pApclst->r_apcnum; i++ ) {
        pApplCxt = pApclst->r_apcary + i;
        
 
   ro0043_pack10Log(RC0020_INTERFACE                               ,  
                    RL1303_EnATTACH                                ,  
                    4                                              ,
                    (signed short int)sizeof(RO_apcary)            ,
                    (char FAR *) pApplCxt                           ,
                    (signed short int) sizeof(unsigned short int)  ,
                    (char FAR *) &i                         ,
                    (pApplCxt->r_apcnam)                            ?
                    (signed short int)(pApplCxt->r_apcnam[0] *
                       sizeof(unsigned long int))                  :
                    0                                              ,
                    (char FAR *) (pApplCxt->r_apcnam+1)             ,
                    (pApplCxt->r_idsary)                            ?
                    (signed short int)(pApplCxt->r_idsnum *
                         sizeof(int)  )                            :
                    0                                              ,
                    (char FAR *) pApplCxt->r_idsary                 );
    /* ************ check the Appl. Context name ************************ */
        if ( ro131w_ChkObjId(pApplCxt->r_apcnam)) {rc = -R_OTITLE;break;}
    /* ************ check the pmode    ********************************** */
        if ( (pApplCxt->r_pmode != RO_PNORM) &&
             (pApplCxt->r_pmode != RO_PSNIP) ){
                 rc = -R_HWPMDE;break;
        } 
    
    /* ************  check the asecif ( configurarion ) ***************** */

        if (!( (pApplCxt->r_asecif & RO_ACSE ) &&
             (pApplCxt->r_asecif & RO_ROSE )) ) {
                 rc = -R_HWCONF;break;
        }

    /* ************   check the validity of the index into Abstract ***** */
    /* ************   Syntax array    *********************************** */
 /* !!!? */
        if ( (pApplCxt->r_idsnum > RCro04_MaxPresCont) ) {rc = -R_PAPCXE;break;}

        for (k=0; k < pApplCxt->r_idsnum; k++){
            if ( *(pApplCxt->r_idsary +k) > RCro01_MaxAbstrSyn){
                rc = -R_PAPCXE;break;
            }
           /* check for the validity of the corresponding OBJID */
            if (ro131w_ChkObjId(
               (pApclst->r_absary+(*(pApplCxt->r_idsary + k)))->r_absnam
                                )){
                 rc = -R_PAPCXE; break; 
            }
        }

    } /* check Appl. context ends */
         

RM0003_RETURN (rs131b, signed short int, rc )
}/*end*/
 



/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNCTION                                               */
/*                                                                         */
/* NAME         : ro131e_copyAplTitle                                      */
/*                                                                         */
/* AUTHOR       : Chandrasekar.K.                                          */
/* DATE         : 93-03-02                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int NEAR PASCAL ro131e_copyAplTitle ( 

       unsigned long int  FAR    * phTarget,
       RO_apname          FAR    * pstSource)
/*                                                                         */
/* DESCRIPTION  :  Copies the application title.                           */
/*                 In case of no AplTitle the handle is 0.                 */
/*                                                                         */
/* IN-PARAMETERS:    phTarget   pointer to a handle of AeTitle (intern)    */
/*                   pstSource  pointer to an AeTitle          (extern)    */
/*                                                                         */
/* OUT-PARAMETERS:   phTarget   pointer to a handle of AeTitle (intern)    */
/*                                                                         */
/*                                                                         */
/* RETURN VALUES :   0  SUCES                                              */
/*                  <0  failiure                                           */
/*                                                                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */

signed short int      rc,i;


unsigned long int  hTarget;
rTro72_unAeTitle  *pTarget;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs131e)
rc = OK;


    hTarget = 0;

    if (! pstSource) {
          /* no pointer means no AplTitle */
          *phTarget = 0;
    } else {
          /* ------- lock the target -------------------------------- */
          hTarget = ro0030_localalloc ( sizeof ( rTro72_unAeTitle));
          pTarget = (hTarget)  
                       ? (rTro72_unAeTitle NEAR *) ro0031_locallock (hTarget)
		       : 0;
          rc      = (pTarget) ? 0 : -R_LMEMRY;
          if (rc) RM0003_RETURN (rs131e, signed short int, rc)
          pTarget->lAeTitleForm = pstSource->r_aptype;
          switch ((pstSource->r_aptype & 0x00ff)){
            case RO_AETF1:
               {
                RO_aetfm1 FAR * pAeTitl1;
                pAeTitl1 =(RO_aetfm1 FAR *)pstSource->r_aetval;
               
                
                
                /* ------- Copy AP Title ----------*/
                pTarget->stAeTitleForm1.lApTitle = pAeTitl1->r_aptnum;
                for (i=0;i<pAeTitl1->r_aptnum;i++)
                    ro131z_CpyRDN((pAeTitl1->r_aptnam+i),
                                   &(pTarget->stAeTitleForm1.vApTitle[i]));
                /* ------- Copy AE Qualifier ------ */
                pTarget->stAeTitleForm1.lAeQuali = pAeTitl1->r_aeqnum;
                for (i=0;i<pAeTitl1->r_aeqnum;i++)
                    ro132a_CpyAVA((pAeTitl1->r_aeqval+i),
                                  &(pTarget->stAeTitleForm1.vAeQuali[i]));   
                
               }
               break;
            case RO_AETF2:
              {
                RO_aetfm2 FAR * pAeTitl2;
                pAeTitl2 =(RO_aetfm2 FAR *)pstSource->r_aetval;
                /* ------- copy from extern to intern presentation ---------- */
                rc = ro0002_objcpy (pTarget->stAeTitleForm2.oApTitleObjId,
    			  pAeTitl2->r_aptitl);

                /* ---rc>0 means that not defined, no objid ---- */
                if ( rc>0) rc=0;

                if (!rc) {
                    if ( pAeTitl2->r_aequal) {
              	      pTarget->stAeTitleForm2.bAeQualInt = TRUE;
              	      pTarget->stAeTitleForm2.lAeQualInt = 
                                *(pAeTitl2->r_aequal);
                     } else {
          	      pTarget->stAeTitleForm2.bAeQualInt = FALSE;
                     }
                 }
              }
              break;
           default:
              rc = -R_HWAETL;
              break;
          }/* end of switch */
              /* ------- unlock of target ---------------------------------- */
          if ( ro0032_localunlock ( hTarget)) rc = -R_LULOKE;
          else *phTarget = hTarget;
          
    } /* ------end of else (pstSource) ------*/
RM0003_RETURN (rs131e, signed short int, rc)
}/*end*/


/*exon *********************************************************************/ 
/*                                                                         */   
/* TYPE         : C-FUNKTION                                               */   
/*                                                                         */   
/* NAME         : ro131c_storeApInv                                        */   
/*                                                                         */   
/* AUTHOR       : Chandrasekar.K                                           */   
/* DATE         : 93-03-01                                                 */   
/*                                                                         */   
/* SYNTAX       :                                                          */   
                                                                               
signed short int  NEAR PASCAL ro131c_storeApInv ( 
       RO_apname         FAR   *pAplTitle,                                     
       RO_apclst         FAR   *pAplLst  ,                       
       RO_invcid         FAR   *pInvcIds ,              
       unsigned long int FAR   *phApInv  )                                    
                                                                               
                                                                               
/*                                                                         */   
/* DESCRIPTION  :  inserts the attach aplication parameter in the          */   
/*                 internal structure (rT0501_AEInvocation)                */   
/*                                                                         */   
/*                                                                         */   
/*                                                                         */   
/* IN-PARAMETERS              :                                            */   
/*                                                                         */   
/* OUT-PARAMETERS:                                                         */   
/*                                                                         */   
/* RETURN VALUES :  == 0 Success                                           */
/*                  != 0 Failure                                           */
/*                                                                         */   
/* GLOBAL DATA   ( read only ):  none                                      */   
/*                                                                         */   
/* GLOBAL DATA   ( modified ) :  none                                      */   
/*                                                                         */   
/* REMARKS                    :  none                                      */   
/*                                                                         */   
/*inoff ********************************************************************/   
                                                                                
{/*entr*/                                                                       
/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */

signed short int            rc ;
unsigned short int          i,j;
unsigned short int          usOffset;
rT0501_AEInvocation   FAR  *pApInv;
RO_apcary             FAR  *pAplCxt;


/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs131c)
rc = 0;

/* ************* allocate memory for application contexts **************** */

*phApInv = ro0030_localalloc (sizeof (rT0501_AEInvocation));
pApInv   = (*phApInv) ? (rT0501_AEInvocation NEAR *) ro0031_locallock (*phApInv)
		      : 0;
rc   = (pApInv)   ? 0 :                                         -R_LMEMRY;

if (rc) RM0003_RETURN (rs131c, signed short int, rc)

/* ****************** Copy the input parameter *************************** */
/* ******************                          *************************** */
rc = ro131e_copyAplTitle( &(pApInv->stOwnAeTitle.hHanOfSub[0]),
			    pAplTitle                         );

if (! rc)
    rc = ro000a_lengthAETitle (pApInv->stOwnAeTitle.hHanOfSub[0],
			       &(pApInv->stOwnAeTitle.uLenOfSub[0]));

if (!rc) {
    pApInv->stOwnAeTitle.uBasLength = sizeof ( rT0501_AEInvocation);
    if(pInvcIds->r_apivid){
       pApInv->stOwnInvoc.bApInvocId = (signed long int) TRUE; 
       pApInv->stOwnInvoc.lApInvocId = (signed long int) *(pInvcIds->r_apivid);
    }
    else{
       pApInv->stOwnInvoc.bApInvocId = (signed long int) FALSE; 
       pApInv->stOwnInvoc.lApInvocId = (signed long int) 0;
    }
    if(pInvcIds->r_aeivid){
       pApInv->stOwnInvoc.bAeInvocId = (signed long int) TRUE; 
       pApInv->stOwnInvoc.lAeInvocId = (signed long int) *(pInvcIds->r_aeivid);
    }
    else{
       pApInv->stOwnInvoc.bAeInvocId = (signed long int) FALSE; 
       pApInv->stOwnInvoc.lAeInvocId = (signed long int) 0;
    }

    pApInv->usOffer               = (unsigned short int) RC1332_InitRespon;/*!*/

    pApInv->sNumbOfApplCtxts      = (signed   short int) pAplLst->r_apcnum;
}

if (! rc && pAplLst)
    for ( i=0,usOffset=0; i<pAplLst->r_apcnum; i++,usOffset++){
      pAplCxt = pAplLst->r_apcary+usOffset;
      if (pAplCxt->r_apcnam  == NULL ) {
         --i;continue;
      }
 /* --------- Verify Later !!! --------------------------------------- */
/*
   ro0043_pack10Log(RC0020_INTERFACE                               ,  
                    RL1303_EnATTACH                                ,  
                    4                                              ,
                    (signed short int)sizeof(RO_apcary)            ,
                    (char FAR *) pAplCxt                           ,
                    (signed short int) sizeof(unsigned short int)  ,
                    (char FAR *) &usOffset                         ,
                    (pAplCxt->r_apcnam)                            ?
                    (signed short int)(pAplCxt->r_apcnam[0] *
                       sizeof(unsigned long int))                  :
                    0                                              ,
                    (char FAR *) (pAplCxt->r_apcnam+1)             ,
                    (pAplCxt->r_idsary)                            ?
                    (signed short int)(pAplCxt->r_idsnum *
                         sizeof(int)  )                            :
                    0                                              ,
                    (char FAR *) pAplCxt->r_idsary                 );
    */
      if ( (pAplCxt->r_asecif & RO_ACSE) &&
           (pAplCxt->r_asecif & RO_ROSE) )  /* NOT RTSE */
              pApInv->vApplCtxtInfos[usOffset].usAseConfig =  RC0241_CfRoseOnly;
      else {
          rc = -R_HWCONF;
          break;
      }         

 
      rc = ro0002_objcpy (pApInv->vApplCtxtInfos[usOffset].oApplCtxtName,
			  pAplCxt->r_apcnam           );
      if      (rc > 0) rc = 0;
      if (rc)     break;

      /* ------ Added on 16-03-93 to roapma4.h in rT0501_AEInvocation --- */
      /* ------ copy the modeSelector ----------------------------------- */
      switch (pAplCxt->r_pmode){
              case RO_PNORM:
                 pApInv->vApplCtxtInfos[usOffset].lModeSelector =
                                           RCro71_NormalMode;
                 break;
              case RO_PSNIP:
                 pApInv->vApplCtxtInfos[usOffset].lModeSelector =
                            (unsigned long int)       RCro72_SNIprivate;
                 break;
              default:
                  rc = -R_W84_88;
                  break; 
      } /*end switch */
      if (rc) break;
     
      /* -------- copy AbsId array ---------------------------------------- */
      *(pApInv->vApplCtxtInfos[usOffset].vAbstrSynId) = (signed short int) 
                                     pAplCxt->r_idsnum;

      for (j =0; j<pAplCxt->r_idsnum;j++)
              *((pApInv->vApplCtxtInfos[usOffset].vAbstrSynId)+j+1) = 
                          (signed short int) *(pAplCxt->r_idsary+j);
    }

if (!rc)
    if (ro0032_localunlock (*phApInv)) rc = -R_LULOKE;

RM0003_RETURN (rs131c, signed short int, rc)
}/*end*/


/*exon *****************************************************************/
/*                                                                     */
/* TYPE         : C-FUNCTION                                           */
/*                                                                     */
/* NAME         : ro131f_storePresInv                                  */
/*                                                                     */
/* AUTHOR       : Chandrasekar.K.                                      */
/* DATE         : 93-03-02                                             */
/*                                                                     */
/* SYNTAX       :                                                      */

signed short int NEAR PASCAL ro131f_storePresInv ( 

	 RO_apclst          FAR  * pApcLst   ,
	 unsigned long  int FAR  * phPrsCxt  )

/*                                                                     */
/* DESCRIPTION  :                                                      */
/*                                                                     */
/* IN-PARAMETERS:                                                      */
/*                                                                     */
/* OUT-PARAMETERS:                                                     */
/*                                                                     */
/* RETURN VALUES :                                                     */
/*                                                                     */
/* GLOBAL DATA   ( read only ):  none                                  */
/*                                                                     */
/* GLOBAL DATA   ( modified ) :  none                                  */
/*                                                                     */
/* REMARKS                    :  none                                  */
/*                                                                     */
/*inoff ************************************************************** */

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s ***************** */
/* ******************                                ***************** */


signed   short  int       rc;
unsigned short  int       i;
unsigned short  int       j;
signed          int       uiAseCf;
unsigned short int        usOffset;
rT0441_stEntContext NEAR *pPrsCxt;
RO_absary           FAR  *pAbsAry;

unsigned long             vAcse[] = { 5, 2, 2, 1, 0, 1 };
unsigned long             vRtse[] = { 3, 2, 3, 2       };
unsigned long             vBER[]  = { 3, 2, 1, 1       };

/* ****************** S T A R T ************************************** */
/* ******************           ************************************** */
RM0002_ENTRY (rs131f)
rc = 0;

/* ****************** Allocate     memory and Lock it       ********** */

*phPrsCxt = ro0030_localalloc (sizeof (rT0441_stEntContext));
pPrsCxt    = (*phPrsCxt)  
                   ? (rT0441_stEntContext NEAR *) ro0031_locallock (*phPrsCxt) 
                   : 0;
rc   = (pPrsCxt)    ? 0 : -R_LMEMRY;

if (rc) RM0003_RETURN (rs131f, signed short int, rc)

/* ************* Copy the Transfer Syntax   ************************** */
 /* !!!holes  check and skip */
for (i =0,usOffset=0; i < pApcLst->r_trsnum; i++,usOffset++ ) {
    if ( (pApcLst->r_trsary+usOffset)->r_trsnam == NULL ) {
        --i;continue;
    }
    rc = ro0002_objcpy( pPrsCxt->vTransSynName[usOffset],
                       (pApcLst->r_trsary+usOffset)->r_trsnam);
   ro0043_pack10Log(RC0020_INTERFACE                               ,  
                    RL1303_EnATTACH                                ,  
                    3                                              ,
                    (signed short int) (sizeof(RO_trsary))         ,
                    (char FAR *) (pApcLst->r_trsary+usOffset)     ,
                    (signed short int) (sizeof(unsigned short int)),
                    (char FAR *) &usOffset                         ,
                    ((pApcLst->r_trsary+usOffset)->r_trsnam)       ?
                    (signed short int)
                    ((pApcLst->r_trsary+usOffset)->r_trsnam[0] *
                    sizeof(unsigned long int))                     :
                    0                                              ,
                    (char FAR *) 
                     ((pApcLst->r_trsary+usOffset)->r_trsnam+1)
                    );
                 
 
}

/* --------- No transfer syntax -------------------------------------- */
if ( (i == 0)&&(rc >= 0) ) 
      rc = ro0002_objcpy( pPrsCxt->vTransSynName[0], vBER); 
        
/* ------------------ end of copy pTrf ------------------------------- */

/* ------------- initialise the    vAbstrSyn[i].vPmNumb[j] ----------- */
/* ------------- with default value RC0441_NoPmNumb ------------------ */
if (!rc)
    for ( i=0; i<RCro01_MaxAbstrSyn; i++) {
        for ( j=0; j<RC0440_MaxDecodeControl; j++)
            pPrsCxt->vAbstrSyn[i].vPmNumb[j] = RC0441_NoPmNumb;
    }

/* ------------- end of initialize the e->vAbstrSyn[i].vPmNumb[i] ---- */

/* ************* Copy Abstract Syntax ******************************** */
 /* !!!holes check empty array & skip */
if(!rc)
    for (i =0,usOffset=0; i < pApcLst->r_absnum; i++,usOffset++ ){  
                                                           /* loop 1 */
         pAbsAry = pApcLst->r_absary+usOffset;
         if ( pAbsAry->r_absnam  == NULL ){
            --i;continue;
         }

   ro0043_pack10Log(RC0020_INTERFACE                               ,  
                    RL1303_EnATTACH                                ,  
                    4                                              ,
                    (signed short int)sizeof(RO_absary)            ,
                    (char FAR *) pAbsAry                           ,
                    (signed short int) sizeof(unsigned short int)  ,
                    (char FAR *) &usOffset                         ,
                    (pAbsAry->r_absnam)                            ?
                    (signed short int)(pAbsAry->r_absnam[0] *
                       sizeof(unsigned long int))                  :
                    0                                              ,
                    (char FAR *) (pAbsAry->r_absnam+1)             ,
                    (pAbsAry->r_idtary)                            ?
                    (signed short int)(pAbsAry->r_idtnum *
                         sizeof(int)  )                            :
                    0                                              ,
                    (char FAR *) pAbsAry->r_idtary                 );
         rc = ro0002_objcpy(
            (unsigned long int FAR *)pPrsCxt->vAbstrSyn[usOffset].oAbstrSynName,
            (unsigned long int FAR *)pAbsAry->r_absnam                        );
         if ( rc < 0) break;

         /* -------- copying of Transfer syn. Ids -------------------- */
         pPrsCxt->vAbstrSyn[usOffset].vTransSynId[0] =
                     pAbsAry->r_idtnum;
        
         for (j=0; j < pAbsAry->r_idtnum;j++){ /* loop 2*/
             pPrsCxt->vAbstrSyn[usOffset].vTransSynId[j+1] =
                      (signed short int) *(pAbsAry->r_idtary+j)  ;
         } /* for loop 2 ends */
         
             /* ------- If no Transfer syntax is present ------------- */
             if(j==0) {
	   /* --- BER stands in vTransSynName[0] -------------------- */
	        pPrsCxt->vAbstrSyn[i].vTransSynId[0] = 1;
	        pPrsCxt->vAbstrSyn[i].vTransSynId[1] = 0;
             } /*!!! why BER in 1 */
        
	 /* - compare the absSyn with RTSE ,ROSE & ACSE obj. ident.--- */
	 /* - and initialise the decode control array vPmNumb[]------- */
             
	     if (! ro0003_objcmp ( vAcse, pAbsAry->r_absnam ) ) {
	        pPrsCxt->vAbstrSyn[usOffset].vPmNumb[0] = 
                                (signed short int) RC0216_Acse;
	        continue;
	     }
	     if (! ro0003_objcmp( (unsigned long int FAR *)vRtse,
                                                           pAbsAry->r_absnam ) )
             {
	       pPrsCxt->vAbstrSyn[usOffset].vPmNumb[0] = 
                               (signed short int) RC0218_Rtse;
	       continue;
	     }
            /* !!! PmNumb is not clear  */
            /* Get the <asecif> of the apcary which includes this absary */

            rc = ro131r_GetAscCf (pApcLst, pAbsAry->r_absnam,
                                  (signed int FAR *)&uiAseCf );

            if (( rc == OK) && (uiAseCf & RO_RTSE )
                            && (uiAseCf & RO_ROSE )  ){
            /* ---- Fill from 3 - 5 as per ANY number -------- */

                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[0] = 
                                  (signed short int) RC0218_Rtse;        
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[1] = 
                                  (signed short int) RC0220_RtseUser;    
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[2] = 
                                  (signed short int) NULL;               
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[3] = 
                                  (signed short int) RC0221_RoseUser;    
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[4] = 
                                  (signed short int) RC0221_RoseUser;    
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[5] = 
                                  (signed short int) RC0221_RoseUser;    
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[6] = 
                                  (signed short int) RC0221_RoseUser;    
               /* !!! here the Rose user has to be filled as per the ANY 
                  number of its user data */
                continue;  
            }else if (( rc == OK) && (uiAseCf &  RO_ROSE) ){
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[0] = 
                                  (signed short int) RC0219_Rose;        
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[1] = 
                                  (signed short int) RC0221_RoseUser;    
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[2] = 
                                  (signed short int) NULL;               
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[3] = 
                                  (signed short int) RC0221_RoseUser;    
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[4] = 
                                  (signed short int) RC0221_RoseUser;    
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[5] = 
                                  (signed short int) RC0221_RoseUser;    
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[6] = 
                                  (signed short int) RC0221_RoseUser;    
                continue;  
            }else  {
                /*   ***** kc on 14-12-93 **** for FILE Error ***
                 rc = -R_HWCONF;
                 break;  ******** */
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[0] = 
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[1] = 
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[2] = 
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[3] = 
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[4] = 
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[5] = 
                pPrsCxt->vAbstrSyn[usOffset].vPmNumb[6] = 
                                  (signed short int) RC0441_NoPmNumb;    
                rc = OK;
                continue;
            } 

    } /* for loop 1 ends */
 
if (!rc) if (ro0032_localunlock ( *phPrsCxt)) rc = -R_LULOKE;

RM0003_RETURN (rs131f, signed short int, rc)
}/*end*/

                                                                                
/*exon *********************************************************************/ 
/*                                                                         */   
/* TYPE         : C-FUNCTION                                               */   
/*                                                                         */   
/* NAME         : ro131g_storeUserData                                     */   
/*                                                                         */   
/* AUTHOR       : Chandrasekar.K                                           */   
/* DATE         : 93-03-15                                                 */   
/*                                                                         */   
/* SYNTAX       :                                                          */   
                                                                                
signed short int   NEAR  PASCAL ro131g_storeUserData (
                                                    
      RO_snfrag            FAR * pFragData,
      signed    long  int        lFragCount,
      signed    short int        sAbsId,
      unsigned  long  int  FAR * pPdvAry,
      unsigned  long  int        ulOffSet,
      unsigned  long  int        ulLength,
      unsigned  long  int  FAR * pHandle)

       
                                                                                
/*                                                                         */   
/* DESCRIPTION  :                                                          */   
/*                                                                         */   
/* IN-PARAMETERS              :                                            */   
/*                                                                         */
/* OUT-PARAMETERS:                                                         */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */

signed   short int      rc,i;

unsigned long  int ulLen=0;
rT1502_stSnData  FAR *p;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs131g)
rc = OK;

*pHandle = ro0020_globalalloc ( sizeof (rT1502_stSnData) );

if (*pHandle) {
   p = (rT1502_stSnData FAR *) ro0021_globallock (*pHandle);
   if (p) {
      p->stPdvHeader.sPmNumb         = (signed short int) RC0221_RoseUser;
      p->stPdvHeader.sAbstrSynId     = (signed short int) sAbsId;
				       /* if close then absid else -1     */
      p->stPdvHeader.lEncodingChoice = (signed long int) RCro0m_ChoiceAny;
      p->stPdvHeader.lType           = 0;

      if(lFragCount){
        p->stPdvHeader.lFraCount  = pFragData->r_fracnt;
           /* by kc for the bug of 5 Externals only 10-11-93 */
        for (i=0; i < (RCro0b_MaxExternal-1) ; i++ ){
            p->stPdvHeader.vPdvHandle[i] =  *pHandle;
            p->vLength[i]                =  (unsigned long int)
                                             pFragData->r_fraary[i].r_sublen;
            p->vOffset[i]                = ulLen;
            p->vAbsId[i]                 = pFragData->r_fraary[i].r_absid;
            ulLen                        += pFragData->r_fraary[i].r_sublen;
         } /*end for loop*/ 
         p->vLength[4]                   =(unsigned long int)
                                           pFragData->r_fraary[4].r_sublen;
         p->vOffset[4]                   = ulLen;
        
        /*end */

      }
      else {
        p->stPdvHeader.lFraCount = 1;
        if(pFragData->r_mtype == RO_FILE)
          p->vLength[0]            = 0;
        else  
          p->vLength[0]            = ulLength;
        p->vOffset[0]            = ulOffSet;
      }  

      switch(pFragData->r_mtype){
         case RO_MEMORY:
                p->iMemType =RC1500_Memory; 
                p->unBufferRef.pBufRef = pFragData->r_fraref;

                break;
         case RO_FILE:
                p->iMemType =RC1501_File; 
                p->unBufferRef.pTpduName = pFragData->r_fraref;
           /*     p->vLength[0]            = NULL; */
                break;
#ifdef RP0000_MicV0600
         case RO_GLOHAN:
                p->iMemType =RC1502_Handle; 
                p->unBufferRef.ulhandle = 
                                *((unsigned long int FAR *)pFragData->r_fraref);
                break;
#endif


      }       
      p->ulBufLen = pFragData->r_fralen;
      if (ro0022_globalunlock (*pHandle))                   rc = -R_GULOKE;
   } else                                                   rc = -R_GLOCKE;
} else                                                      rc = -R_GALLCE;

RM0003_RETURN (rs131g, signed short int, rc)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNCTION                                               */
/*                                                                         */
/* NAME         : ro131h_GetAplCxtNameId                                   */
/*                                                                         */
/* AUTHOR       : Chandrasekar.K.                                          */
/* DATE         : 92-03-16                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int NEAR PASCAL ro131h_GetAplCxtNameId (
              unsigned  long  int             hScb,
              unsigned  long  int             ulAcb,
              rT06z0_stInfrmApplCxtName FAR * pStIfApl,
              signed short  int               sMode)

/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      This function retrieves the application context name /Id from the  */
/*      application entity invocation.                                     */
/*                                                                         */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS :                                                        */
/*                                                                         */
/* RETURN VALUES  :                                                        */
/*                    < 0: error                                           */
/*                    = 0: OK                                              */
/*                    = 1: no Aplication context name/Id founded           */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed short int		rc;
rT0501_AEInvocation NEAR     *pApl;
unsigned long int   NEAR  *pAplObj;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY  (rs131h)

 if (ulAcb != NULL )
     rc = ro0264_getPmInfo(hScb                       ,
                           ulAcb                      ,
                           RC0216_Acse                ,
                           sMode                      ,
                           pStIfApl                   );
 else { /* if no Association use getEcbInfo */
    signed   short int   cnfNum;
    signed   short int   recPm;
    signed   long  int   recEvt;
    unsigned long  int   myAddr;
    unsigned long  int   Dummy;
    unsigned long  int   hPrsCxt;
    unsigned long  int   hApplCxt;


    rc =ro0265_getEcbInfo ( hScb                    ,
                            pStIfApl->ulApplID      ,
                            &cnfNum                 ,
                            &recPm                  ,
                            &recEvt                 ,
                            &myAddr                 ,
                            &Dummy                  ,
                            &hPrsCxt                ,
                            &hApplCxt               );
 
    if (! rc) {
       pApl = (rT0501_AEInvocation NEAR *) ro0031_locallock ( hApplCxt);
       if (pApl) {

       /* -------- search for the apl cxt name ------------------------ */
         rc = 1;
         switch(sMode){
             case RC06z2_ApplCxtID:
	      for (pStIfApl->sApplCxtId = 0; 
                    pStIfApl->sApplCxtId < pApl->sNumbOfApplCtxts; 
                    (pStIfApl->sApplCxtId)++) {
		  pAplObj = pApl->vApplCtxtInfos[pStIfApl->sApplCxtId].
                                                            oApplCtxtName;

		  if (! ro0003_objcmp(pStIfApl->vApplCxtName, pAplObj)) { 
                      rc = 0; break; 
                  }
	      }
              break;
            case RC06z1_ApplCxtName:
              pAplObj = 
               pApl->vApplCtxtInfos[pStIfApl->sApplCxtId].oApplCtxtName;
             if(! ro0002_objcpy(pStIfApl->vApplCxtName, pAplObj )){
               rc=0;break;
             }
               
          }/* end of switch  */
	      if(ro0032_localunlock (hApplCxt)!=OK) rc = -R_LULOKE;
       } else {
	      rc = -R_LLOCKE;
       }
    }/*end if of Successfull ret fron getEcbInfo */ 
    else {
       if (rc > 0)  rc = -R_AAPLCX;
      }
  }/*end else of No ACB */
RM0003_RETURN (rs131h, signed short int, rc )
}/*end*/

                                                                                
/*exon *********************************************************************/ 
/*                                                                         */   
/* TYPE         : C-FUNCTION                                               */   
/*                                                                         */   
/* NAME         : ro131i_freeUserData                                     */   
/*                                                                         */   
/* AUTHOR       : Chandrasekar.K                                           */   
/* DATE         : 93-03-17                                                 */   
/*                                                                         */   
/* SYNTAX       :                                                          */   
                                                                                
signed short int   NEAR  PASCAL ro131i_freeUserData ( 

      unsigned  long  int     hp1)

       
                                                                                
/*                                                                         */   
/* DESCRIPTION  :                                                          */   
/*                                                                         */   
/* IN-PARAMETERS              :                                            */   
/*                                                                         */
/* OUT-PARAMETERS:                                                         */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */

signed   short int      rc;

rT1502_stSnData  FAR *p;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs131i)
rc = OK;

if (hp1) {
   p = (rT1502_stSnData FAR *) ro0021_globallock (hp1);
   if (p) {
        /* ------- Commented by kc on 30/03/93 ------------------- */
        /* -- For the change in Pdv Header ----------------------- */
        /* ---- Start ---- */
        /*
        for(i=0;i<p->stPdvHeader.lFraCount;i++)
          if (ro0024_globalfree(p->stPdvHeader.vPdvHandle[i]))  {
                      rc = -R_GFREEE;break;}
        */ /*end*/
          if (ro0022_globalunlock (hp1))  rc = -R_GULOKE;

   }else rc = -R_GLOCKE; 

   if (!rc) if (ro0024_globalfree(hp1))   rc = -R_GFREEE;
 
 }
  


RM0003_RETURN (rs131i, signed short int, rc)
}/*end*/


/*exon *********************************************************************/ 
/*                                                                         */   
/* TYPE         : C-FUNCTION                                               */   
/*                                                                         */   
/* NAME         : ro131j_storeBuffer                                       */   
/*                                                                         */   
/* AUTHOR       : Chandrasekar.K                                           */   
/* DATE         : 93-03-16                                                 */   
/*                                                                         */   
/* SYNTAX       :                                                          */   
                                                                                
signed short int   NEAR  PASCAL ro131j_storeBuffer (

      RO_buffer           FAR * pBuffer,
      unsigned  long  int FAR * pHandle)

/*                                                                         */   
/* DESCRIPTION  :                                                          */   
/*                                                                         */   
/* IN-PARAMETERS              :                                            */   
/*                                                                         */
/* OUT-PARAMETERS:                                                         */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */

signed   short int      rc;

rT1501_stBuffer  *p;
/* kc on 9-9-98 */
signed  long int          lSize = sizeof(rT1501_stBuffer);

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs131j)
rc = OK;
*pHandle = LNULL;
if ( (pBuffer->r_buflen)  &&
     (pBuffer->r_bufref)   ) {
/* kc on 9-9-98 */
    if (pBuffer->r_mtype == RO_FILE ) lSize += pBuffer->r_buflen;   
    *pHandle = ro0030_localalloc (lSize );                              

    if (*pHandle) {
       p = (rT1501_stBuffer *) ro0031_locallock (*pHandle);
       if (p) {
             switch(pBuffer->r_mtype){
             case RO_MEMORY:
                    p->iMemType =RC1500_Memory;
                    p->unBufferRef.pBufRef = pBuffer->r_bufref; 
                    break;
             case RO_FILE:
                    p->iMemType =RC1501_File; 
                    p->unBufferRef.pTpduName =                               
                     (char FAR *)( (char FAR *)p + sizeof(rT1501_stBuffer)); 
                    ro0041_memcpy(p->unBufferRef.pTpduName ,                 
                                  pBuffer->r_bufref,                         
                                       pBuffer->r_buflen);                   
                    break;
#ifdef RP0000_MicV0600
             case RO_GLOHAN:
                    p->iMemType =RC1502_Handle; 
                    p->unBufferRef.ulhandle = 
                                  *((unsigned long int FAR *)pBuffer->r_bufref);
                    break;
#endif


          }       
          p->ulLength    = (unsigned long int) pBuffer->r_buflen;
        
          if (ro0032_localunlock (*pHandle))                   rc = -R_LULOKE;
       } else                                                   rc = -R_LLOCKE;
    } else                                                      rc = -R_LALLCE;
  }

RM0003_RETURN (rs131j, signed short int, rc)
}/*end*/

/*exon *********************************************************************/ 
/*                                                                         */   
/* TYPE         : C-FUNCTION                                               */   
/*                                                                         */   
/* NAME         : ro131k_CpyOpValue                                        */   
/*                                                                         */   
/* AUTHOR       : Chandrasekar.K                                           */   
/* DATE         : 93-03-18                                                 */   
/*                                                                         */   
/* SYNTAX       :                                                          */   
                                                                                
signed short int   NEAR  PASCAL ro131k_CpyOpValue  (

        RO_opval           FAR * pIOpValue,
        rTro76_stDefinedBy FAR * pDOpValue,
        signed short  int        bMode)


       
                                                                                
/*                                                                         */   
/* DESCRIPTION  :                                                          */   
/*                                                                         */   
/* IN-PARAMETERS              :                                            */   
/*                                                                         */
/* OUT-PARAMETERS:                                                         */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */

signed   short int      rc;


/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs131k)
rc = OK;

 if (bMode){ /*from stDefinedBy to RO_opval */
    switch( pDOpValue->lType) {
       case RC047a_DefinedByInteger:

    /* Note the pointer is already assigned the space */
          *(pIOpValue->r_oploc) = pDOpValue->unValue.lInteger;
          break;
       case RC047b_DefinedByObjectId:
    /* Note the pointer is already assigned the space */
          rc = ro0002_objcpy(pIOpValue->r_opglb,
                             pDOpValue->unValue.oObjectId );
          break;       
    }
 }
 else {  /* from RO_opval to stDefinedBy */
  if (pIOpValue->r_oploc){ /* local value */
      pDOpValue->lType = RC047a_DefinedByInteger;
      pDOpValue->unValue.lInteger= (signed   long  int) *(pIOpValue->r_oploc);
  }
  else if(pIOpValue->r_opglb){  /* Global value */
      if(ro131w_ChkObjId(pIOpValue->r_opglb))
          RM0003_RETURN (rs131k, signed short int, -R_HWOPVL)
      pDOpValue->lType = RC047b_DefinedByObjectId;
      rc = ro0002_objcpy( pDOpValue->unValue.oObjectId,
                            pIOpValue->r_opglb );
  }else rc = -R_HWOPVL;
 }

RM0003_RETURN (rs131k, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131q_setParameter                    	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-20                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

char FAR * NEAR PASCAL ro131q_setParameter (
	char		          FAR * bassis,
	char		    FAR * FAR * unUsedBuf,
	signed	 long  int        FAR * unUsedLen,
	char		          FAR * parbuf,
	unsigned long  int	        parlen,
	signed   short int	        divFactor)




/*                                                                    */
/* DESCRIPTION  : This function inserts an event parameter value into */
/*                the not fixed part of the event buffer. Therefore,  */
/* the target address is alignt and it is checked by this function    */
/* whether the event buffer is to small or not.                       */
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
/* REMARKS                    :  The alignment functionality may      */
/*                               be system depended.                  */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

char	     FAR	  *rc;
int		      offSet;
unsigned long int	   i;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1311)

rc = 0;

if  (parlen) {
       offSet  = *unUsedBuf - bassis;
       offSet %= divFactor;

       if  (offSet > 0) {
	   offSet     = divFactor - offSet;
	   *unUsedBuf += offSet;
	   *unUsedLen -= offSet;
       }
       *unUsedLen -= parlen;

       if  (*unUsedLen >= 0L) {
	    rc		= *unUsedBuf;
	    *unUsedBuf += parlen;

       for (i=0; i < parlen; i++) *(rc+i) = *(parbuf+i);
       }
}
RM0003_RETURN (rs1312, char FAR *, rc)                                    
}/*end*/                                                                        



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131n_retrieveEvFrag                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro131n_retrieveEvFrag (
RO_evtpb                   FAR  *  punEventBuf      ,     
unsigned long  int                 hSource          ,
char                 FAR * FAR  *  pUnUsedBufRef    ,     
signed  long    int        FAR  *  pUnUsedLen       ,    
void                       FAR  *  pTargetRef       ,
signed  short   int                sMode            )



/*                                                                    */
/* DESCRIPTION  :                                                     */
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
signed    short int    rc;
unsigned long int      i;
rT1504_stEvData    FAR * pServEvData;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131n)

rc = OK;

  if (hSource)  { 
     pServEvData = (rT1504_stEvData    FAR *) ro0031_locallock (hSource); 
     if (! pServEvData)            
           RM0003_RETURN (rs131n, signed short int, -R_LLOCKE) 
  } 
 if (sMode){   /* --------- Fragment Data -------------------- */
    RO_evfrag   FAR * pEvFrag = (RO_evfrag FAR *) pTargetRef;
  
    if (!hSource){
       pEvFrag->r_buflen = (unsigned long)NULL;
       pEvFrag->r_bufref = (char FAR *)NULL;
       pEvFrag->r_mtype = (int) NULL;
       pEvFrag->r_specfl = (int) NULL;
       pEvFrag->r_fracnt = (long) NULL;
       pEvFrag->r_fraary[0].r_absid = 
                     (int)NULL;
       pEvFrag->r_fraary[0].r_sublen=
                     (unsigned long)LNULL;
    }
    else {      
       pEvFrag->r_mtype = (int) pServEvData->iMemType; 
       pEvFrag->r_specfl = (int) pServEvData->bSpecFlg; 
       pEvFrag->r_buflen = (unsigned long) pServEvData->ulBufLen; 
       pEvFrag->r_fracnt = (unsigned long) pServEvData->ulFragCount; 
    
   /* --------NOTE: Allocate the pointer -------------- */
       switch(pServEvData->iMemType){
           case RC1500_Memory:
               pEvFrag->r_mtype = RO_MEMORY;
               pEvFrag->r_bufref = 
                         (char FAR *)pServEvData->unBufferRef.pBufRef;
               break;
           case RC1501_File:
               pEvFrag->r_mtype = RO_FILE;
               pEvFrag->r_bufref = (char FAR *) 
                      ro131q_setParameter( 
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     , 
                         pUnUsedLen                                        ,
                         (char FAR *) pServEvData->unBufferRef.pTpduName   ,  
                         pServEvData->ulBufLen                             ,
                         (signed short  int)4                                            );
               break;
           case RC1502_Handle:
               pEvFrag->r_mtype = RC1502_Handle;
               pEvFrag->r_bufref =(char FAR * )
                     ro131q_setParameter( 
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     , 
                         pUnUsedLen                                        ,
                         (char FAR *) &(pServEvData->unBufferRef.ulhandle ),  
                         (unsigned long int)(sizeof (unsigned long int))   ,
                         (signed short  int)4                                            );
               break;
       }/*end of switch */
               
                               
      for (i=0;i<= pServEvData->ulFragCount; i++){
          pEvFrag->r_fraary[i].r_absid = 
                     (int)pServEvData->vFragArray[i].iAbstrSynId;
          pEvFrag->r_fraary[i].r_sublen = 
                     (unsigned long)pServEvData->vFragArray[i].ulLength;
      }
      if (ro0032_localunlock (hSource) != OK ) rc = -R_LULOKE;
    } /*end of hSource */    
  }/*end of smodeif */
 else {   /* --------- copy from EvData --------------------- */
    RO_evdata   FAR * pEvFrag = (RO_evdata FAR *) pTargetRef;
    
    if (!hSource){
        pEvFrag->r_buflen = (unsigned long)NULL;
        pEvFrag->r_bufref = (char FAR *)NULL;
        pEvFrag->r_mtype = (int) NULL;
        pEvFrag->r_specfl = (int) NULL;
        pEvFrag->r_absid = (int) NULL;
        pEvFrag->r_datlen = (unsigned long) NULL;
    }
    else {
       pEvFrag->r_mtype = (int) pServEvData->iMemType; 
       pEvFrag->r_specfl = (int) pServEvData->bSpecFlg; 
       pEvFrag->r_buflen = (unsigned long) pServEvData->ulBufLen; 
    
   /* --------NOTE: Allocate the pointer -------------- */
       switch(pServEvData->iMemType){
           case RC1500_Memory:
               pEvFrag->r_mtype = RO_MEMORY;
               pEvFrag->r_bufref = 
                         (char FAR *)pServEvData->unBufferRef.pBufRef;
               break;
           case RC1501_File:
               pEvFrag->r_mtype = RO_FILE;
               pEvFrag->r_bufref = (char FAR *) 
                      ro131q_setParameter( 
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     , 
                         pUnUsedLen                                        ,
                         (char FAR *) pServEvData->unBufferRef.pTpduName   ,  
                         pServEvData->ulBufLen                             ,
                         (signed short  int)4                                            );

               break;
           case RC1502_Handle:
               pEvFrag->r_mtype = RC1502_Handle;
               pEvFrag->r_bufref =(char FAR *)
                     ro131q_setParameter( 
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     , 
                         pUnUsedLen                                        ,
                         (char FAR *) &(pServEvData->unBufferRef.ulhandle ),  
                         (sizeof (unsigned long int))                      ,
                         (signed short  int)4                                            );
               break;
       }/*end of switch */
          pEvFrag->r_absid = 
                     (int)pServEvData->vFragArray[0].iAbstrSynId;
          pEvFrag->r_datlen=
                     (unsigned long) pServEvData->vFragArray[0].ulLength;
      if (ro0032_localunlock (hSource) != OK ) rc = -R_LULOKE;
     }/*end of else of hSource != Null */                                 
 } /* end of sMode else */

RM0003_RETURN (rs131n, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131l_issueRosEvent				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-03-19					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed	short int NEAR PASCAL ro131l_issueRosEvent(
       unsigned long  int	    ulEcb,
       unsigned long  int	    ulAcb,
       signed	long  int	    lEvent,
       unsigned long  int	    hP1,
       unsigned long  int	    hP2,
       unsigned long  int	    hP3,
       unsigned long  int	    hP4,
       RO_evtpb		  FAR	*   pEventPb)

/*                                                                    */
/* DESCRIPTION	:						      */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/*    esps                                                            */
/*    evntrf                                                          */
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

char		 FAR * punUsedBuf;

signed long  int      unUsedLen;
signed short int	     rc;

unsigned long int      sEvFileLen;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131l)

rc	  = OK;
punUsedBuf = (char FAR *) pEventPb;
unUsedLen = sizeof(RO_evtpb);

switch (lEvent)	{

     /* ************* Issue return buffer event ********************* */
     /* *************				********************* */

case RC1501_RO_BUFFERind:
      { 
        RO_rbinev	            FAR    * pIntrBufInd;
        rT1504_stEvData           NEAR     * pServBufInd;

        punUsedBuf += sizeof (RO_rbinev);
        unUsedLen  -= sizeof (RO_rbinev);

        pIntrBufInd                    = (RO_rbinev FAR *)&(pEventPb->r_rbinev);
        pIntrBufInd->r_evnthd.r_evtype = RO_RBINEV;
       
        pServBufInd = (rT1504_stEvData NEAR *) ro0031_locallock (hP3);
        if (! pServBufInd) {
        	rc = -R_LLOCKE; break;
        }
        pIntrBufInd->r_buffer.r_buflen   = pServBufInd->ulBufLen;
       
        switch (pServBufInd->iMemType){
           case RC1500_Memory:
                pIntrBufInd->r_buffer.r_mtype   = RO_MEMORY;
                pIntrBufInd->r_buffer.r_bufref  = 
                                  pServBufInd->unBufferRef.pBufRef;
                break;
           case RC1501_File:
                pIntrBufInd->r_buffer.r_mtype  = RO_FILE;
                pIntrBufInd->r_buffer.r_bufref = (char FAR *)
                    ro131q_setParameter( 
                         (char       FAR *) pEventPb                        ,
                         (char FAR * FAR *)&punUsedBuf                      , 
                         (signed long int FAR *)&unUsedLen                  ,
                         (char FAR *) (pServBufInd->unBufferRef.pTpduName)  ,
                         pServBufInd->ulBufLen                              ,
                         (signed short int)4                                );
                break;

#ifdef RP0000_MicV0600
            case RC1502_Handle:
                   pIntrBufInd->r_buffer.r_mtype = RO_GLOHAN;
                   pIntrBufInd->r_buffer.r_bufref =(char FAR * )
                       ro131q_setParameter( 
                         (char       FAR *) pEventPb  ,
                         (char FAR * FAR *)&punUsedBuf                       , 
                         (unsigned long int FAR *)&unUsedLen  ,
                         (char FAR *) &(pServBufInd->unBufferRef.ulhandle ),  
                         (unsigned long int)(sizeof (unsigned long int))   ,
                         (signed short int)4                              );
               break;
#endif
            default:
               rc        = -RO_NOEVNT;
               break;
                                                    
        } /*end of switch */ 
     sEvFileLen = ( (pIntrBufInd->r_buffer.r_buflen) &&
                        (pIntrBufInd->r_buffer.r_mtype == RO_FILE) )
                ?     pIntrBufInd->r_buffer.r_buflen
                :     0                                             ;
     ro0043_pack10Log (RC0020_INTERFACE				  ,
			  RL131n_RePROVBUFER				  ,
			  2						  ,
			  (signed short int) sizeof (RO_rbinev)		  ,
			  (char FAR *)	     pIntrBufInd                  ,
			  (signed short int) sEvFileLen                     ,
			  (char FAR *)	    pIntrBufInd->r_buffer.r_bufref );
        if      (ro0032_localunlock (hP3) ) rc = -R_LULOKE;
        else if (ro0034_localfree   (hP3) ) rc = -R_LFREEE;
              

      }
      break;
     /* ************* Issue bind indication event ******************* */
     /* *************                             ******************* */


case RC0702_RO_BINDind:
     {
     RO_bninev	            FAR    * pIntrBndInd;
     rT0702_stBindIndRopm  NEAR    * pServBndInd;



     punUsedBuf += sizeof (RO_bninev);
     unUsedLen  -= sizeof (RO_bninev);

     pIntrBndInd                     = (RO_bninev FAR *)&(pEventPb->r_bninev);
     pIntrBndInd->r_evnthd.r_evtype  = RO_BNINEV;
     pIntrBndInd->r_assid            = ulAcb;
     pIntrBndInd->r_aplid            = ulEcb;
     pIntrBndInd->r_pid	             = hP2;        /* only in the case of */
     if(hP2) hP2 =0;                               /*Redir it holds the process 
                                             that redirected this ind How?*/

     pServBndInd = (rT0702_stBindIndRopm NEAR *) ro0031_locallock (hP3);
     if (! pServBndInd) {
	rc = -R_LLOCKE; break;
     }

     /* --------- Copy Appl Cxt ID <ptcon> --------------------------- */
     pIntrBndInd->r_ptcon = (int)pServBndInd->stAcpm.sApplContextId;

     /* ---------- Copy apname <aetitl> From ACPM parameters --------- */
     rc = ro131p_retrieveAeTitle (
                               pEventPb                                ,
                               pServBndInd->stAcpm.hAeTitle            ,
                               (char      FAR * FAR *)&punUsedBuf      ,
                               (signed long int FAR *)&unUsedLen       ,
                               (RO_apname FAR *)&(pIntrBndInd->r_aetitl));
     if (rc) break;
   /* by kc on 21-10-93 for Called Ae Title   */
     rc = ro131p_retrieveAeTitle (
                               pEventPb                                ,
                               pServBndInd->stAcpm.hCldAeTitle         ,
                               (char      FAR * FAR *)&punUsedBuf      ,
                               (signed long int FAR *)&unUsedLen       ,
                               (RO_apname FAR *)&(pIntrBndInd->r_cldaet));
     if(rc)break;
     rc = ro132d_reterieveInvocId (
                               (RO_invcid  FAR *)&(pIntrBndInd->r_invcid),
                               (rTro74_stInvoc FAR *)
                                  &(pServBndInd->stAcpm.stInvoc),
                               pEventPb                                ,
                               (char      FAR * FAR *)&punUsedBuf      ,
                               (signed long int FAR *)&unUsedLen       );
     if(rc)break;
     rc = ro132d_reterieveInvocId (
                               (RO_invcid  FAR *)&(pIntrBndInd->r_cldinv),
                               (rTro74_stInvoc FAR *)
                                  &(pServBndInd->stAcpm.stCldInvoc),
                               pEventPb                                ,
                               (char      FAR * FAR *)&punUsedBuf      ,
                               (signed long int FAR *)&unUsedLen       );
     if(rc)break;
     /* ---------- Copy the pradd  from stAll-------------------------- */
 
     rc = ro131m_retrieveAddress(pEventPb                                 ,
                                 pServBndInd->stAll.hCallingAddress       ,
                                 (char      FAR * FAR *)&punUsedBuf       ,
                                 (signed long int FAR *)&unUsedLen        ,
                                 (RO_pradd FAR *)&(pIntrBndInd->r_pradd ) );
     if (rc) break;

     rc = ro131m_retrieveAddress(pEventPb                                 ,
                                 pServBndInd->stAll.hCalledAddress       ,
                                 (char      FAR * FAR *)&punUsedBuf       ,
                                 (signed long int FAR *)&unUsedLen        ,
                                 (RO_pradd FAR *)&(pIntrBndInd->r_cldpad ) );
     if (rc) break;


     rc = ro131n_retrieveEvFrag(
                               pEventPb                                       ,
                               hP1                                            ,
                               (char      FAR * FAR *)&punUsedBuf             ,
                               (signed long int FAR *)&unUsedLen              ,
                               (void            FAR *)&(pIntrBndInd->r_evfrag),
                               (signed short  int)1                           );
     if (rc) break;

     sEvFileLen = ( (pIntrBndInd->r_evfrag.r_buflen) &&
                        (pIntrBndInd->r_evfrag.r_mtype == RO_FILE) )
                ?     pIntrBndInd->r_evfrag.r_buflen
                :     0                                             ;

     /* -------------- Logging ---------------------------------------- */
     /*kc on 14-9-93 */
     ro0043_pack10Log (RC0020_INTERFACE				  ,
			  RL1311_ReBINDind				  ,
			  4						  ,
			  (signed short int) sizeof (RO_bninev)		  ,
			  (char FAR *)	     pIntrBndInd                  ,
			  (signed short int) sEvFileLen                     ,
			  (char FAR *)	    pIntrBndInd->r_evfrag.r_bufref, 
			  (signed short int) pIntrBndInd->r_pradd.r_paddln,
			  (char FAR *)	     pIntrBndInd->r_pradd.r_paddrf,
			  (signed short int) pIntrBndInd->r_cldpad.r_paddln,
			  (char FAR *)	     pIntrBndInd->r_cldpad.r_paddrf);


     if (pServBndInd){
         if      (ro0032_localunlock (hP3) ) rc = -R_LULOKE;
         else if (ro0034_localfree   (hP3) ) rc = -R_LFREEE;
     }
     if (rc) break;

     if (hP2)          if (ro0034_localfree (hP2) ) rc = -R_LFREEE;
     if (!rc) if (hP1) if (ro0034_localfree (hP1) ) rc = -R_LFREEE; 

     }
     break;

     /* ************* Issue bind confirmation event ***************** */
     /* *************                               ***************** */

case RC0704_RO_BINDcnf:
     {
     RO_bncnev	            FAR    * pIntrBndCnf;
     rT0704_stBindCnfRopm  NEAR    * pServBndCnf;



     punUsedBuf += sizeof (RO_bncnev);
     unUsedLen  -= sizeof (RO_bncnev);

     pIntrBndCnf                     = (RO_bncnev FAR *)&(pEventPb->r_bncnev);
     pIntrBndCnf->r_evnthd.r_evtype  = RO_BNCNEV;
     pIntrBndCnf->r_assid            = ulAcb;

     /* ---------- Copy Result ---------------------------- */
     
     pIntrBndCnf->r_result           = 0x00000700UL;
     pIntrBndCnf->r_result          |= (unsigned long int )hP4;
     pServBndCnf = (rT0704_stBindCnfRopm NEAR *) ro0031_locallock (hP3);
     if (! pServBndCnf) {
	rc = -R_LLOCKE; break;
     }

     /* --------- Copy Appl Cxt ID <ptcon> --------------------------- */
     pIntrBndCnf->r_ptcon = (int)pServBndCnf->stAcpm.sApplContextId;

     /* ---------- Copy apname <aetitl> From ACPM parameters --------- */

     rc = ro131p_retrieveAeTitle (
                               pEventPb                                      ,
                               pServBndCnf->stAcpm.hAeTitle                  ,
                               (char      FAR * FAR *)&punUsedBuf            ,
                               (signed long int FAR *)&unUsedLen             ,
                               (RO_apname       FAR *)&(pIntrBndCnf->r_aetitl));
     if (rc) break;
    
      rc = ro132d_reterieveInvocId (
                               (RO_invcid  FAR *)&(pIntrBndCnf->r_invcid),
                               (rTro74_stInvoc FAR *)
                                  &(pServBndCnf->stAcpm.stInvoc),
                               pEventPb                                ,
                               (char      FAR * FAR *)&punUsedBuf      ,
                               (signed long int FAR *)&unUsedLen       );
     if(rc)break;

     /* ---------- Copy the pradd  from stAll-------------------------- */
 
     rc = ro131m_retrieveAddress(pEventPb                                ,
                                 pServBndCnf->stAll.hRespondAddress      ,
                                 (char FAR * FAR *)&punUsedBuf           ,
                                 (signed long int FAR *)&unUsedLen       ,
                                 (RO_pradd FAR *)&(pIntrBndCnf->r_pradd) );
     if (rc) break;


     /* ------- Assign the bufref to unUsedBuf incase handle & name -- */

     rc = ro131n_retrieveEvFrag(
                               pEventPb                                        ,
                               hP1                                             ,
                               (char      FAR * FAR *)&punUsedBuf              ,
                               (signed long int FAR *)&unUsedLen               ,
                               (void            FAR *)&(pIntrBndCnf->r_evfrag) ,
                               (signed short  int)1                           );
     if (rc) break;

     sEvFileLen = ( (pIntrBndCnf->r_evfrag.r_buflen) &&
                        (pIntrBndCnf->r_evfrag.r_mtype == RO_FILE) )  ?
                      pIntrBndCnf->r_evfrag.r_buflen                   :
                      0                                             ;
     /* -------------- Logging ---------------------------------------- */

 /* kc on 9-9-93 */
      ro0043_pack10Log(RC0020_INTERFACE		                  ,
			 RL1312_ReBINDcnf			          ,
			 3					          ,
			 (signed short int) sizeof (RO_bncnev)	          ,
			 (char FAR *)	    pIntrBndCnf                   ,
			 (signed short int) sEvFileLen                     ,
			 (char FAR *)	    pIntrBndCnf->r_evfrag.r_bufref, 
			 (signed short int) pIntrBndCnf->r_pradd.r_paddln ,
			 (char FAR *)	    pIntrBndCnf->r_pradd.r_paddrf );

     if (pServBndCnf ) {
         if      (ro0032_localunlock (hP3) ) rc = -R_LULOKE;
         else if (ro0034_localfree   (hP3) ) rc = -R_LFREEE;
     }

     if (!rc && hP2) if (ro0034_localfree (hP2) ) rc = -R_LFREEE;
     if (!rc && hP1) if (ro0034_localfree (hP1) ) rc = -R_LFREEE;
     }
     break;

     /* ************* Issue unbind indication event ***************** */
     /* *************                               ***************** */

case RC0706_RO_UNBINDind:
     {
     RO_ubinev	            FAR    * pIntrUBnInd;



     punUsedBuf += sizeof (RO_ubinev);
     unUsedLen  -= sizeof (RO_ubinev);


     pIntrUBnInd                     = (RO_ubinev  FAR *)&(pEventPb->r_ubinev);
     pIntrUBnInd->r_evnthd.r_evtype  = RO_UBINEV;
     pIntrUBnInd->r_assid            = ulAcb;


     rc = ro131n_retrieveEvFrag(
                               pEventPb                                        ,
                               hP1                                             ,
                               (char      FAR * FAR *)&punUsedBuf              ,
                               (signed long int FAR *)&unUsedLen               ,
                               (void            FAR *)&(pIntrUBnInd->r_evfrag) ,
                               (signed short  int)1                           );
     if (rc) break;

     sEvFileLen = ( (pIntrUBnInd->r_evfrag.r_buflen) &&
                        (pIntrUBnInd->r_evfrag.r_mtype == RO_FILE) )  ?
                      pIntrUBnInd->r_evfrag.r_buflen                   :
                      0                                             ;

     /* -------------- Logging ---------------------------------------- */
     ro0043_pack10Log(RC0020_INTERFACE			          ,
			  RL131g_ReUBINDind			          ,
			  2					          ,
			  (signed short int) sizeof (RO_ubinev)	          ,
			  (char FAR *)	     pIntrUBnInd                  ,
			  (signed short int) sEvFileLen                     ,
	          	  (char FAR *)	    pIntrUBnInd->r_evfrag.r_bufref);  

     if (hP1) if (ro0034_localfree (hP1) ) rc = -R_LFREEE;
     
     }
     break;


     /* ************* Issue unbind confirmation event *************** */
     /* *************                                 *************** */

case RC0708_RO_UNBINDcnf:
     {
     RO_ubcnev	            FAR    * pIntrUBnCnf;



     punUsedBuf += sizeof (RO_ubcnev);
     unUsedLen  -= sizeof (RO_ubcnev);


     pIntrUBnCnf                     = (RO_ubcnev FAR *)&(pEventPb->r_ubcnev);
     pIntrUBnCnf->r_evnthd.r_evtype  = RO_UBCNEV;
     pIntrUBnCnf->r_assid            = ulAcb;
     pIntrUBnCnf->r_ureasn           = hP2;


     rc = ro131n_retrieveEvFrag(
                               pEventPb                                        ,
                               hP1                                             ,
                               (char      FAR * FAR *)&punUsedBuf              ,
                               (signed long int FAR *)&unUsedLen               ,
                               (void            FAR *)&(pIntrUBnCnf->r_evfrag) ,
                               (signed short  int) 1                          );
     if (rc) break;

     sEvFileLen = ( (pIntrUBnCnf->r_evfrag.r_buflen) &&
                        (pIntrUBnCnf->r_evfrag.r_mtype == RO_FILE) )  ?
                      pIntrUBnCnf->r_evfrag.r_buflen                   :
                      0                                             ;

     /* -------------- Logging ---------------------------------------- */
     ro0043_pack10Log (RC0020_INTERFACE			 ,
			  RL131h_ReUBINDcnf			 ,
			  2					 ,
			  (signed short int) sizeof (RO_ubcnev)	 ,
			  (char FAR *)	     pIntrUBnCnf         ,
			    (signed short int) sEvFileLen                     ,
	          	  (char FAR *)	    pIntrUBnCnf->r_evfrag.r_bufref);  

     if (hP1) if (ro0034_localfree (hP1) ) rc = -R_LFREEE;

     }
     break;

     /* ************* Issue abort indication event ****************** */
     /* *************                              ****************** */

case RC070a_RO_UABORTind:
case RC070b_RO_PABORTind:
     {
     RO_abinev	            FAR    * pIntrAbrInd;
    long                             lType=0;

     punUsedBuf += sizeof (RO_abinev);
     unUsedLen  -= sizeof (RO_abinev);


     pIntrAbrInd                     = (RO_abinev FAR *)&(pEventPb->r_abinev);
     pIntrAbrInd->r_evnthd.r_evtype  = RO_ABINEV;
     pIntrAbrInd->r_assid            = ulAcb;
     pIntrAbrInd->r_result           =  hP4;
     pIntrAbrInd->r_adinfo	     = hP2;    



     rc = ro131n_retrieveEvFrag(
                               pEventPb                                        ,
                               hP1                                             ,
                               (char      FAR * FAR *)&punUsedBuf              ,
                               (signed long int FAR *)&unUsedLen               ,
                               (void            FAR *)&(pIntrAbrInd->r_evfrag) ,
                               (signed short  int) 1                          );
     if (rc) break;

     sEvFileLen = ( (pIntrAbrInd->r_evfrag.r_buflen) &&
                        (pIntrAbrInd->r_evfrag.r_mtype == RO_FILE) )  ?
                      pIntrAbrInd->r_evfrag.r_buflen                   :
                      0                                             ;
     lType= (lEvent== RC070a_RO_UABORTind) ? RL131k_ReUABORTind :
            RL131l_RePABORTind;
     /* -------------- Logging ---------------------------------------- */
     ro0043_pack10Log (RC0020_INTERFACE			  ,
   			    (lEvent == RC070a_RO_UABORTind)      ?
                            RL131k_ReUABORTind : 
                            RL131l_RePABORTind         ,
			   2					  ,
			   (signed short int) sizeof (RO_abinev)   ,
			   (char FAR *)       pIntrAbrInd	  ,
			    (signed short int) sEvFileLen                     ,
	          	  (char FAR *)	    pIntrAbrInd->r_evfrag.r_bufref);  



     if (hP1) if (ro0034_localfree (hP1) ) rc = -R_LFREEE;
     }
     break;

     /* ************* Issue invoke indication event ***************** */
     /* *************                               ***************** */

case RC070d_RO_INVind:
     {
     RO_ivinev	            FAR    * pIntrInvInd;
     rT070c_stInvInd       NEAR    * pServInvInd;



     punUsedBuf += sizeof (RO_ivinev);
     unUsedLen  -= sizeof (RO_ivinev);


     pIntrInvInd                     = (RO_ivinev FAR *)&(pEventPb->r_ivinev);
     pIntrInvInd->r_evnthd.r_evtype  = RO_IVINEV;
     pIntrInvInd->r_assid            = ulAcb;



     pServInvInd = (rT070c_stInvInd NEAR *) ro0031_locallock (hP3);
     if (! pServInvInd) {
	rc = -R_LLOCKE; break;
     }
 
     /* ----------- Copy Invoke ID ---------------------------------- */
     pIntrInvInd->r_invid            = (long)pServInvInd->lInvokeID;

     if (pServInvInd->bLinkedID){

          pIntrInvInd->r_lnkid = (long FAR *)
             ro131q_setParameter( 
                         (char FAR * ) pEventPb                            ,
                         &punUsedBuf                                        , 
                         &unUsedLen                                        ,
                         (char FAR *) &(pServInvInd->lLinkedID)            ,  
                         (sizeof( long))                                   ,
                         (signed short  int)4                                            );
                                             
     }
     else    /* ------ NO LinkedID ---------------------------------- */
         pIntrInvInd->r_lnkid  = (long FAR * )NULL;

     /* ----------- Copy Operation Value ------------------------------- */
     rc = ro131o_retrieveOpValue (
                   pEventPb                                                   ,
                  (rTro76_stDefinedBy FAR *)&(pServInvInd->stOperationValue)  ,
                  (char         FAR * FAR *)&punUsedBuf                       , 
                  (signed long int    FAR *)&unUsedLen                        ,
                  (RO_opval           FAR *)&(pIntrInvInd->r_opval)           );
     if (rc) break;
                             

     rc = ro131n_retrieveEvFrag(
                               pEventPb                                        ,
                               hP1                                             ,
                               (char      FAR * FAR *)&punUsedBuf              ,
                               (signed long int FAR *)&unUsedLen               ,
                               (void            FAR *)&(pIntrInvInd->r_evdata) ,
                               (signed short  int)0     ); /* 0 for evdata */
     if (rc) break;

     sEvFileLen = ( (pIntrInvInd->r_evdata.r_buflen) &&
                        (pIntrInvInd->r_evdata.r_mtype == RO_FILE) )  ?
                      pIntrInvInd->r_evdata.r_buflen                   :
                      0                                             ;

     /* -------------- Logging ---------------------------------------- */
     ro0043_pack10Log  (RC0020_INTERFACE				    ,
			    RL1314_ReINVind				    ,
			    5						    ,
			    (signed short int) sizeof (RO_ivinev)	    ,
			    (char FAR *)       pIntrInvInd		    ,
			    (signed short int) sEvFileLen                     ,
	          	    (char FAR *)   pIntrInvInd->r_evdata.r_bufref ,  
			    (pIntrInvInd->r_lnkid)			    ?
			    (signed short int)sizeof (*pIntrInvInd->r_lnkid):
			    (signed short int) 0			    ,
			    (char FAR *)       pIntrInvInd->r_lnkid	    ,
			    (	pIntrInvInd->r_opval.r_oploc)		    ?
			    (signed short int) sizeof (long)                :
			    (signed short int) 0			    ,
			    (char FAR *)       pIntrInvInd->r_opval.r_oploc ,
                            (pIntrInvInd->r_opval.r_opglb)                  ?
                            (sizeof(unsigned long)                          *
                                pIntrInvInd->r_opval.r_opglb[0])            :
                             (signed short int) 0                           ,
			    (char FAR *)pIntrInvInd->r_opval.r_opglb+1 );

     if (pServInvInd) {
         if      (ro0032_localunlock (hP3) ) rc = -R_LULOKE;
         else if (ro0034_localfree   (hP3) ) rc = -R_LFREEE;
     }

     if (!rc && hP1) 
         if (ro0034_localfree (hP1) ) rc = -R_LFREEE;

     }
     break;

     /* ************* Issue result indication event ***************** */
     /* *************                               ***************** */
case RC070f_RO_RESind:
     {
     RO_rsinev	            FAR    * pIntrResInd;
     rT070e_stResInd       NEAR    * pServResInd;



     punUsedBuf += sizeof (RO_rsinev);
     unUsedLen  -= sizeof (RO_rsinev);


     pIntrResInd                     = (RO_rsinev FAR *)&(pEventPb->r_rsinev);
     pIntrResInd->r_evnthd.r_evtype  = RO_RSINEV;
     pIntrResInd->r_assid            = ulAcb;



     pServResInd = (rT070e_stResInd NEAR *) ro0031_locallock (hP3);
     if (! pServResInd) {
	rc = -R_LLOCKE; break;
     }
 
     /* ----------- Copy Invoke ID ---------------------------------- */
     pIntrResInd->r_invid            = (long)pServResInd->lInvokeID;

     if (pServResInd->bOperationValue) {  /*  Opvalue & User Data present */

         /* ----------- Copy Operation Value ------------------------------- */
     rc = ro131o_retrieveOpValue(
                     pEventPb                                                  ,
                     (rTro76_stDefinedBy FAR *)&(pServResInd->stOperationValue),
                     (char         FAR * FAR *)&punUsedBuf                     ,
                     (signed long int    FAR *)&unUsedLen                      ,
                     (RO_opval           FAR *)&(pIntrResInd->r_opval)        );
     if (rc) break;
/* kc on 14-9-93 
     rc = ro131n_retrieveEvFrag(
                               pEventPb                                       ,
                               hP1                                            ,
                               (char      FAR * FAR *)&punUsedBuf             ,
                               (signed long int FAR *)&unUsedLen              ,
                               (void            FAR *)&(pIntrResInd->r_evdata),
                               (signed short  int)0   );*/ /* 0 for evdata */
     if (rc) break;
    } /*end of if for Opvalue & UserData */
    else  {     /* No OpValue & UserData */
   
         pIntrResInd->r_opval.r_oploc = (long FAR *)NULL;
         pIntrResInd->r_opval.r_opglb =(unsigned long FAR * )NULL; 
/*       pIntrResInd->r_evdata.r_bufref = (char FAR *)NULL; kc on 14-9-93 */
    }
 /* kc on 14-9-93 */
     rc = ro131n_retrieveEvFrag(
                               pEventPb                                       ,
                               hP1                                            ,
                               (char      FAR * FAR *)&punUsedBuf             ,
                               (signed long int FAR *)&unUsedLen              ,
                               (void            FAR *)&(pIntrResInd->r_evdata),
                               (signed short  int)0   ); /* 0 for evdata */
    sEvFileLen = ( (pIntrResInd->r_evdata.r_buflen) &&
                        (pIntrResInd->r_evdata.r_mtype == RO_FILE) )  ?
                      pIntrResInd->r_evdata.r_buflen                   :
                      0                                             ;
     /* -------------- Logging ---------------------------------------- */
    ro0043_pack10Log (RC0020_INTERFACE				 ,
			   RL1316_ReRESind				 ,
			   4						 ,
			   (signed short int) sizeof (RO_rsinev) 	 ,
			   (char FAR *)       pIntrResInd       	 ,
			    (signed short int) sEvFileLen                ,
	          	    (char FAR *)   pIntrResInd->r_evdata.r_bufref ,  
			    (	pIntrResInd->r_opval.r_oploc)		    ?
			    (signed short int) sizeof (long)                :
			    (signed short int) 0			    ,
			    (char FAR *)       pIntrResInd->r_opval.r_oploc ,
                            (pIntrResInd->r_opval.r_opglb)                  ?
                            (sizeof(unsigned long)                          *
                                pIntrResInd->r_opval.r_opglb[0] )           :
                             (signed short int) 0                           ,
			    (char FAR *)pIntrResInd->r_opval.r_opglb+1 );

    if      (ro0032_localunlock (hP3) ) rc = -R_LULOKE;
    else if (ro0034_localfree   (hP3) ) rc = -R_LFREEE;

    if (!rc && hP1) {
        if (ro0034_localfree (hP1) ) rc = -R_LFREEE;
    }


     }
     break;

     /* ************* Issue error indication event ****************** */
     /* *************                              ****************** */
case RC070h_RO_ERRind:
     { 
     RO_erinev	            FAR    * pIntrErrInd;
     rT070g_stErrInd       NEAR    * pServErrInd;



     punUsedBuf += sizeof (RO_erinev);
     unUsedLen  -= sizeof (RO_erinev);


     pIntrErrInd                     = (RO_erinev FAR *)&(pEventPb->r_erinev);
     pIntrErrInd->r_evnthd.r_evtype  = RO_ERINEV;
     pIntrErrInd->r_assid            = ulAcb;



     pServErrInd = (rT070g_stErrInd NEAR *) ro0031_locallock (hP3);
     if (! pServErrInd) {
	rc = -R_LLOCKE; break;
     }
 
     /* ----------- Copy Invoke ID ---------------------------------- */
     pIntrErrInd->r_invid            = (long)pServErrInd->lInvokeID;


     /* ----------- Copy Operation Value ------------------------------- */

     rc = ro131o_retrieveOpValue(
            pEventPb                                                ,
            (rTro76_stDefinedBy FAR *)&(pServErrInd->stErrorValue)  ,
            (char         FAR * FAR *)&punUsedBuf                   , 
            (signed long int    FAR *)&unUsedLen                    ,
            (RO_opval           FAR *)&(pIntrErrInd->r_erval)       );
     if (rc) break;

     rc = ro131n_retrieveEvFrag(
                               pEventPb                                        ,
                               hP1                                             ,
                               (char       FAR * FAR *)&punUsedBuf             ,
                               (signed long  int FAR *)&unUsedLen              ,
                               (void             FAR *)&(pIntrErrInd->r_evdata),
                               (signed short int)      0); /* 0 for evdata */
     if (rc) break;

     sEvFileLen = ( (pIntrErrInd->r_evdata.r_buflen) &&
                        (pIntrErrInd->r_evdata.r_mtype == RO_FILE) )  ?
                      pIntrErrInd->r_evdata.r_buflen                   :
                      0                                             ;


     /* -------------- Logging ---------------------------------------- */
     ro0043_pack10Log  (RC0020_INTERFACE				      ,
			    RL1318_ReERRind				      ,
			    4						      ,
			    (signed short int) sizeof (RO_erinev)	      ,
			    (char FAR *)       pIntrErrInd        	      ,
			    (signed short int) sEvFileLen                     ,
	          	    (char FAR *)   pIntrErrInd->r_evdata.r_bufref     ,
			    (	pIntrErrInd->r_erval.r_oploc)		    ?
			    (signed short int) sizeof (long)                :
			    (signed short int) 0			    ,
			    (char FAR *)       pIntrErrInd->r_erval.r_oploc ,
                            (pIntrErrInd->r_erval.r_opglb)                  ?
                            (sizeof(unsigned long)                          *
                                pIntrErrInd->r_erval.r_opglb[0])            :
                             (signed short int) 0                           ,
			    (char FAR *)pIntrErrInd->r_erval.r_opglb+1 );
     if      (ro0032_localunlock (hP3) ) rc = -R_LULOKE;
     else if (ro0034_localfree   (hP3) ) rc = -R_LFREEE;
     if (!rc && hP1) if (ro0034_localfree (hP1) ) rc  = -R_LFREEE;

     }
     break;

     /* ************* Issue reject by user event ******************** */
     /* *************                            ******************** */
case RC070j_RO_RJUind:
     {
     RO_ruinev	            FAR    * pIntrRjUInd;
     rT070i_stURejInd      NEAR    * pServRjUInd;



     punUsedBuf += sizeof (RO_ruinev);
     unUsedLen  -= sizeof (RO_ruinev);


     pIntrRjUInd                     = (RO_ruinev FAR *)&(pEventPb->r_ruinev);
     pIntrRjUInd->r_evnthd.r_evtype  = RO_RUINEV;
     pIntrRjUInd->r_assid            = ulAcb;



     pServRjUInd = (rT070i_stURejInd NEAR *) ro0031_locallock (hP3);
     if (! pServRjUInd) {
	rc = -R_LLOCKE; break;
     }
     /* ----------- Copy Invoke ID ---------------------------------- */
     pIntrRjUInd->r_invid            = (long)pServRjUInd->lInvokeID;
     /* ----------- Copy Reject  Reason ----------------------------- */
     pIntrRjUInd->r_reason        = (unsigned long)pServRjUInd->ulRejReason;

     /* -------------- Logging ---------------------------------------- */
     ro0042_writeLog (RC0020_INTERFACE	,
		      RL131a_ReUREJind,
		      sizeof (RO_ruinev) ,
		      (char FAR *) pIntrRjUInd	);

     if      (ro0032_localunlock (hP3) ) rc = -R_LULOKE;
     else if (ro0034_localfree   (hP3) ) rc = -R_LFREEE;

     }
     break;

     /* ************* Issue reject by provider event **************** */
     /* *************                                **************** */
case RC070k_RO_RJPind:
     {
     RO_rpinev	            FAR    * pIntrRjPInd;
     rT070j_stPRejInd       NEAR   * pServRjPInd;



     punUsedBuf += sizeof (RO_rpinev);
     unUsedLen  -= sizeof (RO_rpinev);


     pIntrRjPInd                     = (RO_rpinev FAR *)&(pEventPb->r_rpinev);
     pIntrRjPInd->r_evnthd.r_evtype  = RO_RPINEV;
     pIntrRjPInd->r_assid            = ulAcb;



     pServRjPInd = (rT070j_stPRejInd NEAR *) ro0031_locallock (hP3);
     if (! pServRjPInd) {
	rc = -R_LLOCKE; break;
     }
     if(pServRjPInd->bInvokeID) {  /* --- invoke Id present ---- */
         pIntrRjPInd->r_invid  =(long FAR *)punUsedBuf; 
         /* ----------- Copy Invoke ID ---------------------------------- */
         *(pIntrRjPInd->r_invid )        = (long)pServRjPInd->lInvokeID;
     }
     else   /* --------- No Invoke Id ----------------------------------- */
         pIntrRjPInd->r_invid  =(long FAR *)NULL; 


     if (pServRjPInd->bRejectReason){  /* ---- Reason Present ---------- */

          pIntrRjPInd->r_reason  = (unsigned long)pServRjPInd->lRejectReason;
     }
     else    /* ------ NO Reason ---------------------------------- */
         pIntrRjPInd->r_reason  = NULL;



     /* -------------- Logging ---------------------------------------- */
     ro0043_pack10Log (RC0020_INTERFACE 		    ,
		       RL131b_RePREJind 		    ,
		       1				    ,
		       (signed short int) sizeof (RO_rpinev) ,
		       (char FAR *)	  pIntrRjPInd	    );

     if      (ro0032_localunlock (hP3) ) rc = -R_LULOKE;
     else if (ro0034_localfree   (hP3) ) rc = -R_LFREEE;

     }
     break;

     /* ************* Event unkown at the interface ***************** */
     /* *************				    ***************** */
default:
    /*  exifPanic =  1; */
     rc        = -RO_NOEVNT;
     break;
} /*end of switch */


RM0003_RETURN (rs131l, signed short int, rc)
}/*end*/



/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro131m_retrieveAddress                                   */
/*                                                                         */
/* AUTHOR       : Michael Rippstain ,Chandrasekar.K.                       */
/* DATE         : 93-03-19                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int NEAR PASCAL ro131m_retrieveAddress (

      RO_evtpb                     FAR *  punEventBuf      ,     
      unsigned  long    int               hHandle          ,     
      char                   FAR * FAR *  pUnUsedBufRef    ,     
      signed  long      int        FAR *  pUnUsedLen       ,    
      RO_pradd                     FAR *  pPrAddr             )


/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/* IN-PARAMETERS :                                                         */
/*                                                                         */
/*                                                                         */
/*                                                                         */
/* OUT-PARAMETERS:                                                         */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */

signed   short int       rc;
char               NEAR *pSource;

unsigned long  int       ulTargetBufferLen;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs131m)
rc = OK;
pSource = 0;

/*
if (pPrAddr->r_paddln)  ulSourceDataLen = pPrAddr->r_paddln;
else ulSourceDataLen = 0;
*/
if (hHandle){
    rc = (signed short int)ro0222_getAddrLength (
                                  (unsigned long int FAR *) &ulTargetBufferLen,
          		          (unsigned long int      )  hHandle);
    if (! rc) {

      if (! ulTargetBufferLen) { /* user desires no psap */
            pPrAddr->r_paddrf = (char FAR *)NULL;
            pPrAddr->r_paddln = NULL;
      }
  /*  else if ( ulTargetBufferLen < ulSourceDataLen) {
	    user information about data 
	    pPrAddr->r_paddln = ulSourceDataLen;
	    rc = -R_ADDCMP; 
  }*/ else { 
            pSource = (char NEAR *) ro0031_locallock (hHandle);
            if (! pSource)   RM0003_RETURN (rs131m, signed short int, -R_LLOCKE)

            pPrAddr->r_paddrf = (char FAR *)
            ro131q_setParameter( 
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     , 
                         pUnUsedLen                                        ,
                         (char FAR *) pSource                              ,
                         ulTargetBufferLen                                 ,
                         (signed short  int)4                              );

	    pPrAddr->r_paddln = ulTargetBufferLen ;
      }
    }/*  if !rc */

    if ((!rc) && pSource) {
        if      (ro0032_localunlock (hHandle)) rc = -R_LULOKE;
        else if (ro0034_localfree   (hHandle)) rc = -R_LFREEE;
    }
} else {
            pPrAddr->r_paddrf = (char FAR *)NULL;
            pPrAddr->r_paddln = NULL;
}

RM0003_RETURN (rs131m, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131o_retrieveOpValue                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-20                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed  short  int  NEAR PASCAL ro131o_retrieveOpValue(

      RO_evtpb                      FAR  *  punEventBuf      ,     
      rTro76_stDefinedBy            FAR  *  pDOpValue        ,
      char                   FAR *  FAR  *  pUnUsedBufRef    ,     
      signed  long    int           FAR  *  pUnUsedLen       ,    
      RO_opval                      FAR  *  pOpValue            )
                             



/*                                                                    */
/* DESCRIPTION  :                                                     */
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
signed   short int rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131o)
    switch( pDOpValue->lType) {
       case RC047a_DefinedByInteger:
          
          pOpValue->r_oploc =  (long FAR *)
             ro131q_setParameter( 
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     , 
                         pUnUsedLen                                        ,
                         (char FAR *) &(pDOpValue->unValue.lInteger       ),
                         (sizeof(unsigned long))                           ,
                         (signed short  int)4                              );
          pOpValue->r_opglb = (unsigned long FAR *)LNULL;  /* skh : 3/7/93 */
          break;

       case RC047b_DefinedByObjectId:
             pOpValue->r_opglb = (unsigned long FAR *) 
                  ro131q_setParameter( (char FAR * ) punEventBuf           ,
                           pUnUsedBufRef                                   ,
                           pUnUsedLen                                      ,
                           (char FAR *) pDOpValue->unValue.oObjectId       ,
                           (( pDOpValue->unValue.oObjectId[0] +1) * 
                            sizeof(unsigned long) )                        ,
                            (signed short  int)4                           );
             pOpValue->r_oploc = (long FAR *) LNULL;       /* skh : 3/7/93 */
          break;       
    }
RM0003_RETURN (rs131o, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131p_retrieveAeTitle                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-20                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro131p_retrieveAeTitle(

      RO_evtpb                         FAR  *  punEventBuf      ,     
      unsigned  long  int                      hHandle          ,     
      char                       FAR * FAR  *  pUnUsedBufRef    ,     
      signed  long    int              FAR  *  pUnUsedLen       ,    
      RO_apname                        FAR  *  pApName)

                                         



/*                                                                    */
/* DESCRIPTION  :                                                     */
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
 signed   short  int  rc       = OK;
 signed   short  int  i        = NULL;
 rTro72_unAeTitle    * pAETitl = NULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131p)
   /*------------ Lock the Handle --------------------------- */
   if (!hHandle){
       /* kc on 9-9-93 */
   /*    pApName->r_aptitl = (unsigned long FAR * ) NULL; 
       pApName->r_aequal = (long FAR *) NULL;            */
       pApName->r_aetval = (void FAR *) NULL; 
       RM0003_RETURN (rs131p, signed short int, rc)
   }
   pAETitl =  (rTro72_unAeTitle NEAR *) ro0031_locallock (hHandle);
   rc      = (pAETitl) ? 0 : -R_LMEMRY;
   if (rc) RM0003_RETURN (rs131p, signed short int, rc)

    /* ------------- Copy aptitl ---------------------------- */
   /* kc on 14-9-93 */ 
   switch( pAETitl->lAeTitleForm ){
      case  RCro77_ObjId:
         {
          RO_aetfm2  stAeTitl2;
          pApName->r_aptype = RO_AETF2;
          stAeTitl2.r_aptitl =(unsigned long FAR *)
             ro131q_setParameter( (char FAR * ) punEventBuf                  ,
                           pUnUsedBufRef                                     ,
                           pUnUsedLen                                        ,
                           (char FAR *) pAETitl->stAeTitleForm2.oApTitleObjId,
                           ((pAETitl->stAeTitleForm2.oApTitleObjId[0] + 1) *
                           (sizeof(unsigned long) ))                         ,
                           (signed short  int)4                              );

         if ( pAETitl->stAeTitleForm2.bAeQualInt == TRUE) 
          stAeTitl2.r_aequal =(long FAR *)
             ro131q_setParameter (
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     ,
                         pUnUsedLen                                        ,
                         (char FAR *) &(pAETitl->stAeTitleForm2.lAeQualInt),
                         (sizeof(unsigned long))                           ,
                         (signed short  int)4                              );
         else
          stAeTitl2.r_aequal = (long FAR *) NULL;
         pApName->r_aetval = (RO_aetfm2 FAR *)
              ro131q_setParameter(
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     ,
                         pUnUsedLen                                        ,
                         (char FAR *) &stAeTitl2                           ,
                         (sizeof(RO_aetfm2))                           ,
                         (signed short  int)4                              );
                        
          ro0043_pack10Log(RC0020_INTERFACE              ,  
                    RL1303_EnATTACH                      ,  
                    3                                    ,
                    (signed short int)sizeof(RO_apname)  ,
                    (char FAR *)pApName                  ,
                    (stAeTitl2.r_aptitl)                 ?
                    (signed short int)(sizeof(unsigned long int)*
                         stAeTitl2.r_aptitl[0] )         :
                          0                              ,
                    (char FAR *)((stAeTitl2.r_aptitl)+1) ,
                    (stAeTitl2.r_aequal)                 ?
                    (signed short int) sizeof(long)      :
                      0                                  ,
                    (char FAR *)stAeTitl2.r_aequal       );
        }
       break;
     case RCro78_RelDisName:
        {
         RO_aetfm1  stAeTitl1;
         pApName->r_aptype = RO_AETF1;
         stAeTitl1.r_aptnum = pAETitl->stAeTitleForm1.lApTitle;
        /* reserve space in the EvPb union */
         stAeTitl1.r_aptnam = (RO_rdname FAR *)
              ro131q_setParameter(
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     ,
                         pUnUsedLen                                        ,
                         (char FAR *) &stAeTitl1                           ,
                         (sizeof(RO_rdname)*pAETitl->stAeTitleForm1.lApTitle),
                         (signed short  int)4                              );
         for (i=0;i<pAETitl->stAeTitleForm1.lApTitle;i++) 
           rc=ro132b_retrieveRDN(
                  (stAeTitl1.r_aptnam+i),
                  &(pAETitl->stAeTitleForm1.vApTitle[i]),
                  punEventBuf,
                  pUnUsedBufRef,
                  pUnUsedLen);  
         
         stAeTitl1.r_aeqnum = pAETitl->stAeTitleForm1.lAeQuali;
        /* reserve space in the EvPb union */
         stAeTitl1.r_aeqval = (RO_avastr FAR *)
              ro131q_setParameter(
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     ,
                         pUnUsedLen                                        ,
                         (char FAR *) &stAeTitl1                           ,
                         (sizeof(RO_avastr)*pAETitl->stAeTitleForm1.lAeQuali),
                         (signed short  int)4                              );

         for (i=0;i<pAETitl->stAeTitleForm1.lAeQuali;i++) 
           rc=ro132c_retrieveAVA(
                  (stAeTitl1.r_aeqval+i),
                  &(pAETitl->stAeTitleForm1.vAeQuali[i]),
                  punEventBuf,
                  pUnUsedBufRef,
                  pUnUsedLen);  

        
        pApName->r_aetval = (RO_aetfm1 F *)
              ro131q_setParameter(
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     ,
                         pUnUsedLen                                        ,
                         (char FAR *) &stAeTitl1                           ,
                         (sizeof(RO_aetfm1)),
                         (signed short  int)4                              );
   
        }
       break;
     default:
       rc = -R_HWAETL;
       break;
    }
   if (hHandle) {
       if      (ro0032_localunlock (hHandle)) rc = -R_LULOKE;
       else if (ro0034_localfree   (hHandle)) rc = -R_LFREEE;
   }

   RM0003_RETURN (rs131p, signed short int, rc)
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131r_GetAscCf 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro131r_GetAscCf ( 
	 RO_apclst          FAR  * pAplLst   ,
         unsigned long int  FAR  * vAbsName  ,
         signed        int  FAR  * pAseCf    )


/*                                                                    */
/* DESCRIPTION  :                                                     */
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
signed  short int      rc;
unsigned      int      i,j;
unsigned short int     usApcOffset;

RO_apcary       FAR  * pApcAry;
RO_absary       FAR  * pAbsAry; 

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131r)
 rc = ERROR;
 pAbsAry = pAplLst->r_absary;
 /* !!!? */

 for (i=0,usApcOffset=0; i < pAplLst->r_apcnum; i++,usApcOffset++) { 
                                                    /* for each Appl. Ary */
    pApcAry = pAplLst->r_apcary + usApcOffset;
    if ( pApcAry->r_apcnam == NULL) {
       --i;continue;
    }
    *pAseCf = pApcAry->r_asecif;
    for (j=0; j < pApcAry->r_idsnum; j++ ){       /* for each AbstrSyn */
	     if (! ro0003_objcmp(
                      (unsigned long int FAR *)vAbsName, 
                      (pAbsAry+pApcAry->r_idsary[j])->r_absnam ) ) 
                            RM0003_RETURN (rs131r, signed short int, 0)
    }
 }
 RM0003_RETURN (rs131r, signed short int, rc)
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131s_CheckLegalOcur                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro131s_CheckLegalOcur(
     unsigned long  int        hScb,
     unsigned long  int        ulAcb,
     signed   long  int        lEventNum,
     signed   short int  FAR * pbLegal)

/*                                                                    */
/* DESCRIPTION  :                                                     */
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
signed    short  int   rc ;

rT07z0_stPossibilityInfo stInfo;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131s)
 rc = OK;
 stInfo.lEvent = lEventNum;
 stInfo.ulAcb   = ulAcb;
 rc = ro0264_getPmInfo( hScb                                    ,
                        ulAcb                                   ,
                        RC0219_Rose                             ,
                        (signed short int)RC07z0_PossibilityInfo,
                        (rT07z0_stPossibilityInfo FAR *)&stInfo );
 if (!rc) *pbLegal = stInfo.bPossible;
 else *pbLegal = FALSE;   
RM0003_RETURN (rs131s, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131t_GetAbsSynRslt                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro131t_GetAbsSynRslt(

          unsigned  long  int          hScb,
          unsigned  long  int          ulAcb,
          rTro65_pfvAbstrSynResult     pvAbsIdRslt)



/*                                                                    */
/* DESCRIPTION  :                                                     */
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
signed  short int  rc;

rT04z0_stDCS       stDcs;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131t)

 rc = ro0264_getPmInfo (hScb               ,
                        ulAcb              ,
                        RC0215_Presentation,
                        RC04zR_getDCS      ,
                        (rT04z0_stDCS FAR *)&stDcs);
 if (!rc)
     ro0041_memcpy( (char FAR *) pvAbsIdRslt            , 
                    (char FAR *) stDcs.vAbstrSynResult  ,
                    (sizeof(stDcs.vAbstrSynResult[0])  *
                           (stDcs.vAbstrSynResult[0] + 1)       ) );

RM0003_RETURN (rs131t, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131u_findAbsId				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-06-04                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro131u_findAbsId  (
unsigned long int hScb,
signed   long int lAplId,
signed short int sAbsId)


/*                                                                    */
/* DESCRIPTION  :  calls the Inform function of Ppm.                  */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   == 0  Success                       */
/*                                != 0  Error                         */
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
signed   short int      rc = OK;
rT04z2_stAbsIdFind      stAbsIdFnd;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131u)
 stAbsIdFnd.sAbsId = sAbsId;
 stAbsIdFnd.bFound = FALSE;
 stAbsIdFnd.lAplId = lAplId;

 if (lAplId < 0 )  
    rc = ro0273_getPmInfo2 ( hScb ,
                             RC0241_CfRoseOnly ,
                             RC0215_Presentation,
                             RC04zS_findAbsId,      
                             (rT04z2_stAbsIdFind FAR *)&stAbsIdFnd);
 else  {
     unsigned long int Dummy = LNULL;
     unsigned long int hEntContext = LNULL;
     rT0441_stEntContext    *pEntContext = LNULL;
 
     rc = ro0265_getEcbInfo (hScb,
                             (unsigned long int ) lAplId,
                             (signed short int FAR *)&Dummy,
                             (signed short int FAR *)&Dummy,
                             (signed long int FAR *)&Dummy,
                             &Dummy,
                             &Dummy,
                             &hEntContext,
                             &Dummy );
    if (!rc) {
        pEntContext =  (rT0441_stEntContext NEAR *) 
                               ro0031_locallock (hEntContext);
        if (pEntContext){
          if (ro131w_ChkObjId(pEntContext->vAbstrSyn [sAbsId].oAbstrSynName))
               stAbsIdFnd.bFound = FALSE;
          else 
               stAbsIdFnd.bFound = TRUE;
          if (ro0032_localunlock(hEntContext)) rc = -R_LULOKE;
        } else rc = -R_LLOCKE;
    }
 }     
 if (!rc) {
   if ( stAbsIdFnd.bFound  != TRUE ) rc = -R_HWABSI;
 }  
RM0003_RETURN (rs131u, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131v_getMode  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-06-28                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro131v_getMode ( 
unsigned long int      hScb,
unsigned long int      hAcb,
signed        int FAR *pMode)



/*                                                                    */
/* DESCRIPTION  :                                                     */
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
signed   short int      rc = OK;
rT04z3_stMode           stMode;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131v)
  stMode.sMode = NULL;
  rc = ro0264_getPmInfo(
              hScb                               ,
              hAcb,
              RC0215_Presentation                     ,
              RC04zT_getMode                   ,
              (rT04z3_stMode FAR *)&stMode );
  if (!rc ) 
     switch (stMode.sMode){
        case 1:
           *pMode = RO_PNORM;
           break;
        case -1:
           *pMode = RO_PSNIP;
           break;
        default:
           rc = -R_W84_88;
     }
RM0003_RETURN (rs131v, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131w_ChkObjId 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-07-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro131w_ChkObjId (
unsigned long int   FAR  * pObjId)


/*                                                                    */
/* DESCRIPTION  : Checks the boundary conditions & components of      */
/*                OBJIDs .                                            */
/*                                                                    */
/* IN-PARAMETERS              :  Ptr to the OBJID                     */
/*                                                                    */
/* OUT-PARAMETERS             : None                                  */
/*                                                                    */
/* RETURN VALUES              :   == 0  Success                       */
/*                                != 0  Error                         */
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

RM0002_ENTRY (rs131w)
 if ( pObjId == NULL ) 
       RM0003_RETURN (rs131w, signed short int, ERROR) 
 if ( ( pObjId[0]   > RCro00_MaxObjIdComp ) ||
      ( pObjId[0]   < 3                   ) ||
      ( pObjId[1] > 2 || pObjId[1] < 2 && pObjId[2] > 39) )
        RM0003_RETURN (rs131w, signed short int, ERROR) 
 else                                    
        RM0003_RETURN (rs131w, signed short int, OK) 
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131x_ChkAEtitl				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-06                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro131x_ChkAEtitl  (
unsigned long int      hScb,
signed   long int      lAplId,
RO_apname         FAR *pApName)


/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   == 0  Success                       */
/*                                != 0  Error                         */
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
signed   short int      rc = OK;
signed   short int      i ;
unsigned long int Dummy = LNULL;
unsigned long int hAeInvoc = LNULL;
rT0501_AEInvocation    *pAeInvoc = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131x)
 
     if ( pApName->r_aetval == NULL )                              
          RM0003_RETURN (rs131x, signed short int, rc)
     rc = ro0265_getEcbInfo (hScb,
                             (unsigned long int ) lAplId,
                             (signed short int FAR *)&Dummy,
                             (signed short int FAR *)&Dummy,
                             (signed long int FAR *)&Dummy,
                             &Dummy,
                             &Dummy,
                             &Dummy,
                             &hAeInvoc );
    if (!rc) {
        pAeInvoc =  (rT0501_AEInvocation NEAR *) 
                               ro0031_locallock (hAeInvoc);
        if (pAeInvoc){
         rTro72_unAeTitle  NEAR *phAeTitl=NULL;
         if ( pAeInvoc->stOwnAeTitle.hHanOfSub[0]){
           phAeTitl=(rTro72_unAeTitle NEAR *) ro0031_locallock(
                     pAeInvoc->stOwnAeTitle.hHanOfSub[0]);
           if(phAeTitl){
             if (phAeTitl->lAeTitleForm & RO_AETNC ){
                if(ro0032_localunlock(pAeInvoc->stOwnAeTitle.hHanOfSub[0])) 
                                        rc =-R_LULOKE;       
                if (ro0032_localunlock(hAeInvoc)) rc = -R_LULOKE;
                RM0003_RETURN (rs131x, signed short int, rc)
           }      
           if( (pApName->r_aptype & ~RO_AETNC )    != 
               (phAeTitl->lAeTitleForm & ~RO_AETNC) )
                     rc=-R_HWAETL;
           if(!rc)
             switch (phAeTitl->lAeTitleForm & ~RO_AETNC ){
               case RCro78_RelDisName:
                 {
                   RO_aetfm1  FAR * pAeTitl1 = 
                           (RO_aetfm1 FAR *)pApName->r_aetval;
                   
                   rc=-R_HWAETL;
                   if ( pAeTitl1->r_aptnum != 
                             phAeTitl->stAeTitleForm1.lApTitle) break;
                   for (i=0;i<pAeTitl1->r_aptnum;i++)
                      if( ro132e_ChkSetofAVAs ( 
                             (pAeTitl1->r_aptnam+i)->r_rdnnum,
                             (pAeTitl1->r_aptnam+i)->r_rdnval,
                             phAeTitl->stAeTitleForm1.vApTitle[i].lRDN,
                             phAeTitl->stAeTitleForm1.vApTitle[i].vRDN)) break;
                    
                   /* cmp Ae qualifier */
                   if( ro132e_ChkSetofAVAs ( 
                             pAeTitl1->r_aeqnum,
                             pAeTitl1->r_aeqval,
                             phAeTitl->stAeTitleForm1.lAeQuali,
                             phAeTitl->stAeTitleForm1.vAeQuali)) break;
                   rc=OK;
                    
                 }
                     break;
               case  RCro77_ObjId:
                 {
                   RO_aetfm2  FAR * pAeTitl2 = 
                           (RO_aetfm2 FAR *)pApName->r_aetval;
                   
                   if (ro0003_objcmp(pAeTitl2->r_aptitl,
                            phAeTitl->stAeTitleForm2.oApTitleObjId))
                      rc =-R_HWAETL;
                   if(!rc)
                     if( phAeTitl->stAeTitleForm2.bAeQualInt)
                       if( pAeTitl2->r_aequal) {
                        if ( *pAeTitl2->r_aequal != 
                               phAeTitl->stAeTitleForm2.lAeQualInt) 
                                   rc=-R_HWAETL;
                        }else rc=-R_HWAETL;
                     else if (pAeTitl2->r_aequal)rc=-R_HWAETL;
 

                 }
                  break; 
             default: 
                 rc =-R_HWAETL;
                 break;
          }
                 
          if(ro0032_localunlock(pAeInvoc->stOwnAeTitle.hHanOfSub[0])) 
                                        rc =-R_LULOKE;       
         }else rc=-R_LLOCKE;
          if (ro0032_localunlock(hAeInvoc)) rc = -R_LULOKE;
       } else rc = -R_LLOCKE;
    }
   }      
RM0003_RETURN (rs131x, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131y_ChkInAEtitl				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-08                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro131y_ChkInAEtitl  ( 
RO_apname         FAR *pApName)


/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   == 0  Success                       */
/*                                != 0  Error                         */
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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131y)
  if(pApName->r_aetval == NULL )
     RM0003_RETURN (rs131y, signed short int, rc)
  switch((pApName->r_aptype & ~RO_AETNC)){
   case RO_AETF1:
         {
          RO_aetfm1  FAR * pAeTitl1;
          int              i,j;
          pAeTitl1 = (RO_aetfm1 FAR *)pApName->r_aetval;
       
          ro0043_pack10Log(RC0020_INTERFACE            ,
                    RL1303_EnATTACH                      ,  
                    2                                    ,
                    (signed short int) sizeof(RO_apname) ,
                    (char FAR *) pApName                 ,
                    (signed short int)sizeof(RO_aetfm1)  ,
                    (char FAR *)pAeTitl1                 );
          for (i=0;i< pAeTitl1->r_aptnum;i++){
              RO_rdname FAR * pRDName;
              
              pRDName = pAeTitl1->r_aptnam+i;
              
              ro0042_writeLog (RC0020_INTERFACE  ,
                               RL1303_EnATTACH   ,
                               (signed short int)sizeof(RO_rdname),
                               (char FAR *)pRDName  );
              if(pRDName->r_rdnnum > RCro7f_MaxRDNSequence)
                      RM0003_RETURN (rs131y, signed short int, -R_HWAETL)
  
              for (j=0;j<pRDName->r_rdnnum;j++){
                  RO_avastr FAR * pAVAstr;

                  pAVAstr = pRDName->r_rdnval+j;
                  
                    
                  ro0043_pack10Log(RC0020_INTERFACE            ,
                          RL1303_EnATTACH                      ,  
                          3                                    ,
                          (signed short int)sizeof(RO_avastr)  ,
                          (char FAR *)pAVAstr                  ,
                          (pAVAstr->r_avatyp)                  ?
                          (signed short int)(sizeof(unsigned long int)*
                           pAVAstr->r_avatyp[0])               :
                            0                                  ,
                          (char FAR *) ((pAVAstr->r_avatyp)+1) ,
                          (signed short int)pAVAstr->r_avanum  ,
                          (char FAR *)pAVAstr->r_avaval        );
                  if(ro131w_ChkObjId(pAVAstr->r_avatyp)) 
                      RM0003_RETURN (rs131y, signed short int, -R_HWAETL)
                  if(pAVAstr->r_avanum > RCro7h_MaxAttriValue)
                      RM0003_RETURN (rs131y, signed short int, -R_HWAETL)
              }
               
           }
           for (j=0;j<pAeTitl1->r_aeqnum;j++){
                  RO_avastr FAR * pAVAstr;

                  pAVAstr = pAeTitl1->r_aeqval+j;
                  
                    
                  ro0043_pack10Log(RC0020_INTERFACE            ,
                          RL1303_EnATTACH                      ,  
                          3                                    ,
                          (signed short int)sizeof(RO_avastr)  ,
                          (char FAR *)pAVAstr                  ,
                          (pAVAstr->r_avatyp)                  ?
                          (signed short int)(sizeof(unsigned long int)*
                           pAVAstr->r_avatyp[0])               :
                            0                                  ,
                          (char FAR *) ((pAVAstr->r_avatyp)+1) ,
                          (signed short int)pAVAstr->r_avanum  ,
                          (char FAR *)pAVAstr->r_avaval        );
                  if(ro131w_ChkObjId(pAVAstr->r_avatyp)) 
                      RM0003_RETURN (rs131y, signed short int, -R_HWAETL)
                  if(pAVAstr->r_avanum > RCro7h_MaxAttriValue)
                      RM0003_RETURN (rs131y, signed short int, -R_HWAETL)
              }
          
         }
         break;
   case RO_AETF2:
         {
          RO_aetfm2  FAR * pAeTitl2;
          pAeTitl2 =  (RO_aetfm2 FAR *) pApName->r_aetval;

          ro0043_pack10Log(RC0020_INTERFACE              ,  
                    RL1303_EnATTACH                      ,  
                    3                                    ,
                    (signed short int)sizeof(RO_apname)  ,
                    (char FAR *)pApName                  ,
                    (pAeTitl2->r_aptitl)                 ?
                    (signed short int)(sizeof(unsigned long int)*
                         pAeTitl2->r_aptitl[0] )         :
                          0                              ,
                    (char FAR *)((pAeTitl2->r_aptitl)+1) ,
                    (pAeTitl2->r_aequal)                 ?
                    (signed short int) sizeof(long)      :
                      0                                  ,
                    (char FAR *)pAeTitl2->r_aequal       );
          if ( pAeTitl2->r_aptitl)
               if ( ro131w_ChkObjId ( pAeTitl2->r_aptitl))
                        rc = -R_HWAETL;
         }
         break;
   default:
        rc=-R_HWAETL;
        break;
  }
RM0003_RETURN (rs131y, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro131z_CpyRDN     				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-11                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro131z_CpyRDN       ( 
RO_rdname         FAR * pRDName,
rTro69_stRDN      FAR * pstRDN)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   == 0  Success                       */
/*                                != 0  Error                         */
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
signed   short int      rc = OK;
signed   short int      i  = NULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs131z)
 pstRDN->lRDN = pRDName->r_rdnnum;
 for(i=0;i<pRDName->r_rdnnum;i++)
    if((rc=ro132a_CpyAVA((pRDName->r_rdnval+i),
                  &(pstRDN->vRDN[i])))) break;

RM0003_RETURN (rs131z, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro132a_CpyAVA     				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-11                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro132a_CpyAVA       ( 
RO_avastr         FAR * pAVAval,
rTro68_stAVA      FAR * pstAVA)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   == 0  Success                       */
/*                                != 0  Error                         */
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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs132a)
 
 rc = ro0002_objcpy (pstAVA->vAVA_Type,
    			  pAVAval->r_avatyp);
 if (!rc){
  pstAVA->lAVA_Value = pAVAval->r_avanum;
  
  ro0041_memcpy((char FAR *)pstAVA->vAVA_Value,
                (char FAR *)pAVAval->r_avaval,
                pAVAval->r_avanum); 
 }

RM0003_RETURN (rs132a, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro132b_retrieveRDN 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro132b_retrieveRDN    ( 
      RO_rdname                        FAR  *  pRDName,
      rTro69_stRDN                     FAR  *  pstRDN,
      RO_evtpb                         FAR  *  punEventBuf      ,     
      char                       FAR * FAR  *  pUnUsedBufRef    ,     
      signed  long    int              FAR  *  pUnUsedLen           )

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   == 0  Success                       */
/*                                != 0  Error                         */
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
signed   short int      rc = OK;
signed   short int      i  = NULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs132b)
    pRDName->r_rdnnum = pstRDN->lRDN;
    pRDName->r_rdnval = (RO_avastr FAR *)
              ro131q_setParameter(
                         (char FAR * ) punEventBuf                         ,
                         pUnUsedBufRef                                     ,
                         pUnUsedLen                                        ,
                         (char FAR *) &rc                                  ,
                         (sizeof(RO_avastr)*pstRDN->lRDN),
                         (signed short  int)4                              );
    for(i=0;i<pstRDN->lRDN;i++){
          rc = ro132c_retrieveAVA(
                   (pRDName->r_rdnval+i),
                   &(pstRDN->vRDN[i]),  
                   punEventBuf,
                   pUnUsedBufRef,
                   pUnUsedLen);
          ro0042_writeLog (RC0020_INTERFACE  ,
                               RL1303_EnATTACH   ,
                               (signed short int)sizeof(RO_rdname),
                               (char FAR *)pRDName  );
         }
         

RM0003_RETURN (rs132b, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro132c_retrieveAVA				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro132c_retrieveAVA  ( 
      RO_avastr                        FAR  *  pAVAval,
      rTro68_stAVA                     FAR  *  pstAVA,
      RO_evtpb                         FAR  *  punEventBuf      ,     
      char                       FAR * FAR  *  pUnUsedBufRef    ,     
      signed  long    int              FAR  *  pUnUsedLen           )

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   == 0  Success                       */
/*                                != 0  Error                         */
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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs132c)
 
 pAVAval->r_avatyp = (unsigned long F *) 
             ro131q_setParameter( (char FAR * ) punEventBuf                  ,
                           pUnUsedBufRef                                     ,
                           pUnUsedLen                                        ,
                           (char FAR *) pstAVA->vAVA_Type                 ,
                           ((pstAVA->vAVA_Type[0] + 1) *
                           (sizeof(unsigned long) ))                         ,
                           (signed short  int)4                              );
 pAVAval->r_avanum = pstAVA->lAVA_Value;
 pAVAval->r_avaval = (char F *)
             ro131q_setParameter( (char FAR * ) punEventBuf                  ,
                           pUnUsedBufRef                                     ,
                           pUnUsedLen                                        ,
                           (char FAR *) pstAVA->vAVA_Value                 ,
                           (pstAVA->lAVA_Value *
                           (sizeof(char) ))                                   ,
                           (signed short  int)4                              );
 

 ro0043_pack10Log(RC0020_INTERFACE            ,
                          RL1303_EnATTACH                      ,  
                          3                                    ,
                          (signed short int)sizeof(RO_avastr)  ,
                          (char FAR *)pAVAval                  ,
                          (pAVAval->r_avatyp)                  ?
                          (signed short int)(sizeof(unsigned long int)*
                           pAVAval->r_avatyp[0])               :
                            0                                  ,
                          (char FAR *) ((pAVAval->r_avatyp)+1) ,
                          (signed short int)pAVAval->r_avanum  ,
                          (char FAR *)pAVAval->r_avaval        );
RM0003_RETURN (rs132c, signed short int, rc)                                    
}/*end*/          

                                                              
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro132d_reterieveInvocId                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-11                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro132d_reterieveInvocId   ( 
RO_invcid         FAR * pInvocIds,
rTro74_stInvoc    FAR * pstInvoc,
RO_evtpb                         FAR  *  punEventBuf      ,     
char                       FAR * FAR  *  pUnUsedBufRef    ,     
signed  long    int              FAR  *  pUnUsedLen           )

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   == 0  Success                       */
/*                                != 0  Error                         */
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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs132d)
 if(pstInvoc->bApInvocId == TRUE)
   pInvocIds->r_apivid = (long F *)
             ro131q_setParameter( (char FAR * ) punEventBuf                  ,
                           pUnUsedBufRef                                     ,
                           pUnUsedLen                                        ,
                           (char FAR *) &(pstInvoc->lApInvocId)              ,
                           (sizeof(long) )                                  ,
                           (signed short  int)4                              );
 else pInvocIds->r_apivid = (long F *)NULL;
 if(pstInvoc->bAeInvocId == TRUE)
   pInvocIds->r_aeivid = (long F *)
             ro131q_setParameter( (char FAR * ) punEventBuf                  ,
                           pUnUsedBufRef                                     ,
                           pUnUsedLen                                        ,
                           (char FAR *) &(pstInvoc->lAeInvocId)              ,
                           (sizeof(long) )                                  ,
                           (signed short  int)4                              );
 else pInvocIds->r_aeivid = (long F *)NULL;
      
RM0003_RETURN (rs132d, signed short int, rc)                                    
}/*end*/                                                                        


/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro132e_ChkSetofAVAs                                      */
/*                                                                         */
/* AUTHOR       : Chandrasekar.K.                                          */
/* DATE         : 22-10-93                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int NEAR PASCAL
 ro132e_ChkSetofAVAs(signed long  int       lAVA1,
		     RO_avastr        FAR * vAVA1,
		     signed long  int       lAVA2,
		     rTro68_stAVA     FAR * vAVA2 )

/*                                                                         */
/* DESCRIPTION  :   This function compares a SET OF Attribute Value        */
/*                  Assertions (AVA).                                      */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/* Parameter 1       : number of SET OF elements from AVA1                 */
/* Parameter 2       : vectores of AVA1                                    */
/* Parameter 3       : number of SET OF elements from AVA2                 */
/* Parameter 4       : vectores of AVA2                                    */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*              = 0  : if ok                                               */
/*              < 0  : if an error occurs                                  */
/*              > 0  : if different AVAs, the number descibes what         */
/*                     difference:                                         */
/*                1  : different number of SET OFs (lAVA)                  */
/*                1x : the x indicates the AVA1, which isn't found         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed short int k, l, sAVA1Found, rcc, rc;
signed short int vFoundAVA2[RCro7g_MaxRDN];  /* Found array for founded AVA1
						in SET OF AVA2 */
/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs132e)

rc = 0;
ro0043_memset((char*) vFoundAVA2, (int) -1,
	      (unsigned long int) sizeof (signed short int)*RCro7g_MaxRDN);

if ( lAVA1 != lAVA2) {
   rc = 1;       /* number of AVAs are different, error */

} else {

   /* set AVA1 */
   for ( k=0; k<lAVA1; k++) {

       l        = 0;    /* set first AVA2 */
       sAVA1Found = FALSE;

       /* cmp AVA1 with AVA2 */
       while (( sAVA1Found==FALSE) && ( l<lAVA2)) {

	 rcc = 0;

	 /* check if this AVA was still found */
	 if (vFoundAVA2[l] == -1) {

		 /* cmp object identifier */
		 if ( ro0003_objcmp ((vAVA1+k)->r_avatyp,
				     (vAVA2+l)->vAVA_Type ))
		     rcc = 1;
		 /* cmp number of vAVA_Value[] elements */
	    else if ( (vAVA1+k)->r_avanum != (vAVA2+l)->lAVA_Value)
		     rcc = 2;
		 /* cmp vAVA_Value[] */
	    else if ( ro0042_memcmp ((vAVA1+k)->r_avaval,
				     (vAVA2+l)->vAVA_Value,
				     (unsigned long int)(vAVA1+k)->r_avanum ))
		     rcc = 3;


	    if ( rcc==0) {  /* AVAs are equal */

		vFoundAVA2[l] = k;     /* set the Found array  */
		sAVA1Found    = TRUE;  /* set the Found note, next AVA1  */
	    }
	 } /* (vFoundAVA2[l] == -1) { */

	 l++;    /* set new AVA2 */

       } /* end of while (( sAVA1Found==FALSE) && ( l<lAVA2)) { */

       /* check if a AVA isn't founded */
       if ( sAVA1Found==FALSE) {
	   rc = 10+k;                /* AVA1[k] isn't found       */
	   break;                    /* return, AVAs are different */
       }

   } /* end of for ( k=0; k<lAVA1; k++) { */
} /* end of else */

RM0003_RETURN (rs132e, signed short int, rc)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1312_detach   				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-06                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL ro1312_detach (
         
       unsigned long  int  	        hScb,
       RO_detapb 	   FAR	       *pDetapb)




/*                                                                    */
/* DESCRIPTION  :                                                     */
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
signed   short int rc;
unsigned long  int  ulNoCxt;
unsigned long  int  hPresCxt;
unsigned long  int  hApplCxt;
unsigned long  int  hMyAddr;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
RM0002_ENTRY (rs1312)

/* -----------write interface logging------------------------------------- */
/* ----------------------------------------------------------------------- */

ro0042_writeLog (RC0020_INTERFACE ,
		 RL1305_EnDETACH,
		 sizeof (RO_detapb),
		 (char FAR *) pDetapb   );

/* -----------call dispatcher -------------------------------------------- */
/* ----------------------------------------------------------------------- */

rc = ro0261_detach ( hScb    ,
		     (unsigned long int) pDetapb->r_aplid,
		     &hMyAddr ,
		     &ulNoCxt ,
		     &hPresCxt,
		     &hApplCxt );

if (! rc)  {
    /* this handle are given back from dispatcher to free this handle
       at the place where allocated    */
    if (hMyAddr){
        if      (ro0032_localunlock (hMyAddr)) rc = -R_LULOKE;
        else if (ro0034_localfree   (hMyAddr)) rc = -R_LFREEE;
    }
    if (!rc && hPresCxt) {
        if      (ro0032_localunlock (hPresCxt)) rc = -R_LULOKE;
        else if (ro0034_localfree   (hPresCxt)) rc = -R_LFREEE;
    }
    if (!rc && hApplCxt) {
        rT0501_AEInvocation   FAR  *pApInv =NULL;

        pApInv = (rT0501_AEInvocation NEAR *) ro0031_locallock (hApplCxt);
        if (pApInv){ 
           if (pApInv->stOwnAeTitle.hHanOfSub[0])
               ro0034_localfree(pApInv->stOwnAeTitle.hHanOfSub[0]);
        }
        else rc = -R_LLOCKE;
        if (!rc) {
            if      (ro0032_localunlock (hApplCxt)) rc = -R_LULOKE;
            else if (ro0034_localfree   (hApplCxt)) rc = -R_LFREEE;
        }
    }
}

/* --------- set Header ---------------------------------------------------*/
/* ------------------------------------------------------------------------*/
ro131a_setHeader(&pDetapb->r_hdinfo, rc);

/* -------write Header Logging-------------------------------------------- */
/* !!! to be completed */

RM0003_RETURN (rs1312, signed short int, rc)
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1313_AssocFuns				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-16                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro1313_AssocFuns (
         
       unsigned long  int               hScb,
       RO_excopb 	   FAR	       *pParlst)



/*                                                                    */
/* DESCRIPTION  :                                                     */
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
signed   short  int      rc        = OK;
signed   short  int      sCount;

unsigned long  int       i;
unsigned long  int       ulAcb     = LNULL ;
unsigned long  int       p1        = LNULL ;
unsigned long  int       p2        = LNULL ;
unsigned long  int       p3        = LNULL ;
unsigned long  int       p4        = LNULL ;
unsigned long  int       ulEcb     = LNULL ;
signed   short int       sMode     = NULL  ;
signed         int       iMode     = NULL  ;
signed   long  int       lEvent    = LNULL ;

rT06z0_stInfrmApplCxtName stAplIf;
 
RO_snfrag          FAR * pSnFragData;                                         
signed   short int       sCnfNo    = 0;    
signed   short int       sRecInPm  = 0;    
signed   long  int       lRecInEvt = 0;    
                                         
unsigned long  int       hOwnAddr  = 0;     
unsigned long  int       ulDummy   = 0;     
unsigned long  int       hPresInv  = 0;     
unsigned long  int       hApplInv  = 0;     
                                         
signed   long  int       lAplId    = -1;          
                         
unsigned long  int       sSnFileLen;
unsigned long  int       sBfFileLen;

signed   short int       bLegal;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
RM0002_ENTRY (rs1313)
 
 rc = OK;


 switch(pParlst->r_hdinfo.r_fnctnb){
    case RO_BINRQ:
        {
         RO_bnrqpb  FAR * pBndReqPb;
         


         pBndReqPb = (RO_bnrqpb FAR *)&(pParlst->r_bnrqpb);

         
         sSnFileLen = ( (pBndReqPb->r_sndfra.r_fralen) &&
                        (pBndReqPb->r_sndfra.r_mtype == RO_FILE) )  ?
                      pBndReqPb->r_sndfra.r_fralen                   :
                      0                                             ;

        sBfFileLen = ( (pBndReqPb->r_buffer.r_buflen) &&
                       (pBndReqPb->r_buffer.r_mtype == RO_FILE)  )  ?
                     pBndReqPb->r_buffer.r_buflen                   :
                     0                                              ;

                       
         /* --------write interface logging--------------------------- */   
         /* ---------------------------------------------------------- */   

        ro0043_pack10Log(RC0020_INTERFACE		         ,
				RL1307_EnBINDreq		         ,
				4				         ,
				(signed short int) sizeof (RO_bnrqpb)    ,
				(char FAR *)	   pBndReqPb             ,
				(signed short int) sSnFileLen	         ,
				(sSnFileLen)			         ?
				(char FAR *)pBndReqPb->r_sndfra.r_fraref :
				(char FAR *)	   0		         ,
				(signed short int) sBfFileLen            ,
				(sBfFileLen)			         ?
				(char FAR *)pBndReqPb->r_buffer.r_bufref :
				(char FAR *)	   0		         ,
				(signed short int) pBndReqPb->r_pradd.r_paddln,
				(char FAR *) pBndReqPb->r_pradd.r_paddrf );



         /* --------check input parameter----------------------------- */   
         /* ---------------------------------------------------------- */   
         /* ------------  Check Pres Addr ---------------------------- */   
         
         if ( pBndReqPb->r_pradd.r_paddln == 0)     rc = -R_ADDLEN;
         else if ((pBndReqPb->r_pradd.r_paddln > 0) && 
                  (pBndReqPb->r_pradd.r_paddrf == 0)  )   rc = -R_ADDREF; 

                                                                  
         /* ------------- Check Assoc Class -------------------------- */   
         if ( (pBndReqPb->r_aclass < RO_ACL1 ) ||
              (pBndReqPb->r_aclass > RO_ACL3 ) )      rc = -R_HACLAS;
       
         /* ------------- Check Apname <aetitl> Called ---------------- */   
         if(!rc) rc = ro131y_ChkInAEtitl(&pBndReqPb->r_aetitl);

         if (rc) break;

         /* ------------- Check Buffer ------------------------------- */   
         /* !!! dos unix incompatible in Global Handle */
        if ( (pBndReqPb->r_buffer.r_buflen ) &&
             (pBndReqPb->r_buffer.r_bufref )  ) {
#ifdef RP0000_MicV0600
           if  ( (pBndReqPb->r_buffer.r_mtype < RO_MEMORY ) ||
                 (pBndReqPb->r_buffer.r_mtype > RO_GLOHAN   )  ) rc = -R_HMTYPE;
#else 
           if  ( (pBndReqPb->r_buffer.r_mtype < RO_MEMORY ) ||
                 (pBndReqPb->r_buffer.r_mtype > RO_FILE   )  ) rc = -R_HMTYPE;
#endif
        }
        if (rc) break;

         /* ------------ Check Apcon --------------------------------- */   
         /* --------!!! how to check the index (use util fun )-------- */   
         stAplIf.sApplCxtId = (signed int)pBndReqPb->r_apcon;
         stAplIf.ulApplID   = 
         lAplId              =pBndReqPb->r_aplid; 
         rc = ro131h_GetAplCxtNameId (hScb,
                                     (unsigned long int)NULL,
                                     (rT06z0_stInfrmApplCxtName FAR *)& stAplIf,
                                     (signed short int)RC06z1_ApplCxtName);
        if (rc) rc = -R_APLCXT;
        /* ------------ assign the User Send data -------------------- */
        if (!rc)
            pSnFragData = (RO_snfrag FAR *)&(pBndReqPb->r_sndfra);
        }
        break;

    case RO_BINRS:
        {
         RO_bnrspb  FAR * pBndRspPb;
         
         rT05z0_stAplCxtCmp stCxtChk; 


         pBndRspPb = (RO_bnrspb FAR *)&(pParlst->r_bnrspb);

         sSnFileLen = ( (pBndRspPb->r_sndfra.r_fralen) &&
                        (pBndRspPb->r_sndfra.r_mtype == RO_FILE) )  ?
                      pBndRspPb->r_sndfra.r_fralen                   :
                      0                                             ;


         /* --------write interface logging--------------------------- */   
         /* ---------------------------------------------------------- */   

	 ro0043_pack10Log(RC0020_INTERFACE		      ,
				RL1309_EnBINDrsp		      ,
				3				      ,
				(signed short int) sizeof (RO_bnrspb) ,
				(char FAR *)	   pBndRspPb          ,
				(signed short int) sSnFileLen	         ,
				(sSnFileLen)			         ?
				(char FAR *)pBndRspPb->r_sndfra.r_fraref :
				(char FAR *)	   0		         ,
				(signed short int) pBndRspPb->r_pradd.r_paddln,
				(char FAR *) pBndRspPb->r_pradd.r_paddrf );
         
         /* --------check input parameter----------------------------- */   
         /* ---------------------------------------------------------- */   
         /* ------------ Check the Legality of the function ---------- */
         rc = ro131s_CheckLegalOcur(
                                    hScb                            ,
                                    pBndRspPb->r_assid              ,
                                    RC0703_RO_BINDrsp               ,
                                    (signed short int FAR *)&bLegal );
         if ((rc) || (!bLegal)) { rc = -R_FCTCAL; break; }

         /* ------------  Check Pres Addr ---------------------------- */   
         

         else if ((pBndRspPb->r_pradd.r_paddln > 0) && 
                  (pBndRspPb->r_pradd.r_paddrf == 0) ) { rc = -R_ADDREF; break;} 

         /* ------------- Check Assoc Class -------------------------- */   
         if ( (pBndRspPb->r_aclass < RO_ACL1 ) ||
              (pBndRspPb->r_aclass > RO_ACL3 ) )   { rc = -R_HACLAS; break; }
       
         /* ------------- Check Apname <aetitl> Called ---------------- */   
         
         if (pBndRspPb->r_aetitl.r_aetval != NULL ){
               signed long int lAplId=NULL;
               signed long int lAssId=NULL;
               rc = ro131y_ChkInAEtitl(&pBndRspPb->r_aetitl);
               if(rc) break;

               lAssId = ro0214_getAcbEcb((unsigned long int FAR *)&lAplId); 
               rc=ro131x_ChkAEtitl(hScb,lAplId,&(pBndRspPb->r_aetitl));
         }
         /* ------------ Check Apcon --------------------------------- */   
         /* --------!!! how to check the index (use util fun )-------- */   
         stCxtChk.sApCxtId = pBndRspPb->r_apcon;
 

        /* ---------check the Appl. Cxt ID with the BINDind ---------- */ 
          if (!rc)
             rc = ro0264_getPmInfo(
                           hScb                               ,
                           pBndRspPb->r_assid                 ,
                           RC0217_LcrtFct                     ,
                           RC05z1_AplCxtCmp                   ,
                          (rT05z0_stAplCxtCmp FAR *)&stCxtChk );
          if (rc) break;

          if ( stCxtChk.bAplCxtCmpRes == FALSE ) { rc = -R_INFACA; break; }

        /* --------  check the r_result ----------------------------- */   
          if ( (pBndRspPb->r_result < RO_ACCEPT ) ||
               (pBndRspPb->r_result > ( RO_UREJCT | RO_UnrecCldAeInvocId )))
                        { rc = -R_HRSTER; break; }

        /* ------------ assign the User Send data -------------------- */
        pSnFragData = (RO_snfrag FAR *)&(pBndRspPb->r_sndfra);
        ulAcb = pBndRspPb->r_assid;
        }
        break;
    case RO_UBNRQ:
        {
         RO_ubrqpb  FAR * pUBndReqPb;
         


         pUBndReqPb = (RO_ubrqpb FAR *)&(pParlst->r_ubrqpb);
         sSnFileLen = ( (pUBndReqPb->r_sndfra.r_fralen) &&
                        (pUBndReqPb->r_sndfra.r_mtype == RO_FILE) )  ?
                      pUBndReqPb->r_sndfra.r_fralen                   :
                      0                                             ;

         sBfFileLen = ( (pUBndReqPb->r_buffer.r_buflen) &&
                       (pUBndReqPb->r_buffer.r_mtype == RO_FILE)  )  ?
                     pUBndReqPb->r_buffer.r_buflen                   :
                     0                                              ;

         /* --------write interface logging--------------------------- */   
         /* ---------------------------------------------------------- */   

         ro0043_pack10Log(RC0020_INTERFACE		      ,
				RL131c_EnUBINDreq		      ,
				3				      ,
				(signed short int) sizeof (RO_ubrqpb)  ,
				(char FAR *)	   pUBndReqPb         ,
				(signed short int) sSnFileLen	         ,
				(sSnFileLen)			         ?
				(char FAR *)pUBndReqPb->r_sndfra.r_fraref :
				(char FAR *)	   0		         ,
				(signed short int) sBfFileLen            ,
				(sBfFileLen)			         ?
				(char FAR *)pUBndReqPb->r_buffer.r_bufref :
				(char FAR *)	   0		         );
 
         /* --------check input parameter----------------------------- */   
         /* ---------------------------------------------------------- */   
         /* ------------ Check the Legality of the function ---------- */
         rc = ro131s_CheckLegalOcur(
                                    hScb                            ,
                                    pUBndReqPb->r_assid             ,
                                    RC0705_RO_UNBINDreq             ,
                                    (signed short int FAR *)&bLegal );
         if ((rc) || (!bLegal)) { rc = -R_FCTCAL; break; }

         /* ------------- Check Buffer ------------------------------- */   
         /* !!! dos unix incompatible in Global Handle */
        if ( (pUBndReqPb->r_buffer.r_buflen ) &&
             (pUBndReqPb->r_buffer.r_bufref )  ){
#ifdef RP0000_MicV0600
           if  ( (pUBndReqPb->r_buffer.r_mtype < RO_MEMORY ) ||
                 (pUBndReqPb->r_buffer.r_mtype > RO_GLOHAN   )  ) 
                                         { rc = -R_HMTYPE;  break; }
#else 
           if  ( (pUBndReqPb->r_buffer.r_mtype < RO_MEMORY ) ||
                 (pUBndReqPb->r_buffer.r_mtype > RO_FILE   )  )
                                         { rc = -R_HMTYPE;  break; }
#endif
        }
        /* ------------ assign the User Send data -------------------- */
        pSnFragData = (RO_snfrag FAR *)&(pUBndReqPb->r_sndfra);
        ulAcb = pUBndReqPb->r_assid;
        }
        break;

    case RO_UBNRS:
        {
         RO_ubrspb FAR  * pUBndRspPb;
         


         pUBndRspPb = (RO_ubrspb FAR *)&(pParlst->r_ubrspb);

         sSnFileLen = ( (pUBndRspPb->r_sndfra.r_fralen) &&
                        (pUBndRspPb->r_sndfra.r_mtype == RO_FILE) )  ?
                      pUBndRspPb->r_sndfra.r_fralen                   :
                      0                                             ;
         /* --------write interface logging--------------------------- */   
         /* ---------------------------------------------------------- */   

         ro0043_pack10Log(RC0020_INTERFACE		      ,
				RL131e_EnUBINDrsp		      ,
				2				      ,
				(signed short int) sizeof (RO_ubrspb)  ,
				(char FAR *)	   pUBndRspPb         ,
				(signed short int) sSnFileLen	         ,
				(sSnFileLen)			         ?
				(char FAR *)pUBndRspPb->r_sndfra.r_fraref :
				(char FAR *)	   0		         );

         /* --------check input parameter----------------------------- */   
         /* ---------------------------------------------------------- */   
         /* ------------ Check the Legality of the function ---------- */
         rc = ro131s_CheckLegalOcur(
                                    hScb                            ,
                                    pUBndRspPb->r_assid             ,
                                    RC0707_RO_UNBINDrsp             ,
                                    (signed short int FAR *)&bLegal );
         if ((rc) || (!bLegal)) { rc = -R_FCTCAL; break; }

         /* none   */
        /* ------------ check the r_ureasn -------------------------- */
           if ( (pUBndRspPb->r_ureasn & 0x0000ffff ) != RO_NotDefined ) 
                       {  rc = -R_HRSNER; break; }

        /* ------------ assign the User Send data -------------------- */
        pSnFragData = (RO_snfrag FAR *)&(pUBndRspPb->r_sndfra);
        ulAcb = pUBndRspPb->r_assid;
        }
        break;

    case RO_ABORQ:
        {
         RO_abrqpb FAR  * pAbrReqPb;
         


         pAbrReqPb = (RO_abrqpb FAR *)&(pParlst->r_abrqpb);

         sSnFileLen = ( (pAbrReqPb->r_sndfra.r_fralen) &&
                        (pAbrReqPb->r_sndfra.r_mtype == RO_FILE) )  ?
                      pAbrReqPb->r_sndfra.r_fralen                   :
                      0                                             ;
         /* --------write interface logging--------------------------- */   
         /* ---------------------------------------------------------- */   

         ro0043_pack10Log(RC0020_INTERFACE		      ,
				RL131i_EnUABORTreq		      ,
				2				      ,
				(signed short int) sizeof (RO_abrqpb)  ,
				(char FAR *)	   pAbrReqPb          ,
				(signed short int) sSnFileLen	         ,
				(sSnFileLen)			         ?
				(char FAR *)pAbrReqPb->r_sndfra.r_fraref :
				(char FAR *)	   0		         );
         /* --------check input parameter----------------------------- */   
         /* ---------------------------------------------------------- */   
         /* none */
        
         /* ------------ assign the User Send data -------------------- */
        pSnFragData = (RO_snfrag FAR *)&(pAbrReqPb->r_sndfra);
        ulAcb = pAbrReqPb->r_assid;
        rc = ro131s_CheckLegalOcur(
                                    hScb                            ,
                                    pAbrReqPb->r_assid              ,
                                    RC0709_RO_UABORTreq             ,
                                    (signed short int FAR *)&bLegal );
         if ((rc) || (!bLegal)) { rc = -R_FCTCAL; break; }
        }

        break;
 } /*end of switch */
 
 /* Check the common elements of the BINDreq BINDrsp UNBINDreq  ----- */
 /* UNBINDrsp And ABORTreq ------------------------------------------ */
 /* ----------- check snfrag parameters ----------------------------- */
 /* !!! dos unix incompatible in Global Handle */

 /* -------- check only if present --------------------------------- */
if (rc) RM0003_RETURN(rs1313,signed short int,rc)


if ( ( pSnFragData ) &&
     ( pSnFragData->r_fraref != NULL) &&
     ( pSnFragData->r_fralen >  NULL)   ) {
       unsigned long int ulTotlen = NULL;

#ifdef RP0000_MicV0600
         if ( (pSnFragData->r_mtype < RO_MEMORY )||
              (pSnFragData->r_mtype > RO_GLOHAN   ) ) rc = -R_HMTYPE;
#else

         if ( (pSnFragData->r_mtype < RO_MEMORY )||
              (pSnFragData->r_mtype > RO_FILE   ) ) rc = -R_HMTYPE;
#endif

         if (!rc)
             if ( pSnFragData->r_fracnt > RO_maxExternals ) rc = -R_HFRCNT ; 
         if ((!rc) && ( pSnFragData->r_fralen  > MAXLONG_POS )) rc = -R_HUDTLE;
         if (!rc) 
           for (i=0; i<pSnFragData->r_fracnt;i++)  {

              rc = ro131u_findAbsId (hScb ,  lAplId ,
                                  pSnFragData->r_fraary[i].r_absid );
              if (!rc) ulTotlen += pSnFragData->r_fraary[i].r_sublen;
           }
        

         if ( (!rc) && ( ulTotlen != pSnFragData->r_fralen )
              && (pSnFragData->r_mtype == RO_MEMORY))
                                              rc =  -R_HUDTLE;
         if (( !rc) && ( pParlst->r_hdinfo.r_fnctnb != RO_BINRQ)  ) {
             rc =  ro131v_getMode ( hScb , ulAcb, (signed int FAR *)&iMode );
             if (!rc )
               if ( (iMode == RO_PSNIP ) && ( pSnFragData->r_fracnt > 1 ) )
                  rc = -R_HMTYPE;
         }
 }
     /* ------- No SnData ------------------------ */


 if (rc) RM0003_RETURN(rs1313,signed short int,rc)

/* ******************* Build Pdv tree ******************************** */

 switch(pParlst->r_hdinfo.r_fnctnb){
    case RO_BINRQ:
        {
         RO_bnrqpb            FAR * pBndReqPb;
         rT0701_stBindReqRopm     * pBindRq;
         rT0501_AEInvocation      * pAEInv;
         unsigned short int         usAssocCnfNum; 

         pBndReqPb = (RO_bnrqpb FAR *)&(pParlst->r_bnrqpb);

         /* ----------- Allocate the P3 ------------------------------ */   
         p3 = ro0030_localalloc(sizeof (rT0701_stBindReqRopm));
         if (!p3)  { rc = -R_LALLCE; break; }

         pBindRq = (rT0701_stBindReqRopm NEAR *)ro0031_locallock (p3);
         if (!pBindRq) { rc = -R_LLOCKE; break; }
   
         /* ----------- Get the Ecb info from Scb -------------------- */   
         rc = ro0265_getEcbInfo (hScb,
                                 pBndReqPb->r_aplid,   /* ecb during atach */
                                 &sCnfNo   ,  
                                 &sRecInPm ,  
                                 &lRecInEvt,  
                                 &hOwnAddr ,  
                                 &ulDummy  ,  
                                 &hPresInv ,  
                                 &hApplInv  );
         if (rc) break;

         /* ----------- lock the Appl. Invocation -------------------- */
         pAEInv = (rT0501_AEInvocation NEAR *)ro0031_locallock (hApplInv);
         if (!pAEInv) { rc = -R_LLOCKE; break; }

         /* ----------- copy all the parameters ---------------------- */   
         /* ----------- copy Ropm parameters ------------------------- */   
         pBindRq->stRopm.ulAssocClass = (unsigned long int)
                                                 pBndReqPb->r_aclass;

 
         pBindRq->stRopm.ulCheckPointSize = (unsigned long int) 
                                                RC1330_CheckPtSize;

         pBindRq->stRopm.ulWindowSize = (unsigned long int)
                                               RC1331_WindowSize;
 
          
         /* ----------- copy Acpm parameters ------------------------- */   
         pBindRq->stAcpm.sApplContextId = (signed   short int)
                              pBndReqPb->r_apcon;

         rc = ro131e_copyAplTitle(&(pBindRq->stAcpm.hAeTitle) ,
                                  &(pBndReqPb->r_aetitl) );
         if (rc) break;
         if ( pBndReqPb->r_invcid.r_apivid ) {
             pBindRq->stAcpm.stInvoc.bApInvocId = TRUE;
             pBindRq->stAcpm.stInvoc.lApInvocId = 
              (signed long int)*(pBndReqPb->r_invcid.r_apivid);
         }else{
         /* This parameter  won't be supported from the RTEXIF   */
             pBindRq->stAcpm.stInvoc.bApInvocId = (signed long int) 0;  
             pBindRq->stAcpm.stInvoc.lApInvocId = (signed long int) 0;  
         }
         if ( pBndReqPb->r_invcid.r_aeivid ) {
             pBindRq->stAcpm.stInvoc.bAeInvocId = TRUE;
             pBindRq->stAcpm.stInvoc.lAeInvocId = 
              (signed long int)*(pBndReqPb->r_invcid.r_aeivid);
         }else{
             pBindRq->stAcpm.stInvoc.bAeInvocId = (signed long int) 0;  
             pBindRq->stAcpm.stInvoc.lAeInvocId = (signed long int) 0;  
         } 
         /* ----------- copy stPpm parameters ------------------------ */   
         /* !!! from where to get the AbsIds for one Appl. */
         sCount = pAEInv->vApplCtxtInfos[(signed short int)pBndReqPb->r_apcon].
                 vAbstrSynId[0];                           

         for (i=0;(signed short int)i<=sCount;i++)
             *(pBindRq->stPpm.vAbstrSynId+i) =      
              *(pAEInv->vApplCtxtInfos[(signed short int)pBndReqPb->r_apcon].
                                          vAbstrSynId +i)    ;

         
         pBindRq->stPpm.sDefAbstrSynId = (signed short int)
                                               RCro6i_NoDefContext;

                
         pBindRq->stPpm.ulRequirements = (unsigned long int)   
                                               RCro60_Kernel;
 
          
         /* ----------- copy stAll parameters ------------------------ */   
         rc = ro131d_storeAddress (             pBndReqPb->r_pradd.r_paddrf,
                                               pBndReqPb->r_pradd.r_paddln,
                   (unsigned  long int FAR *)&(pBindRq->stAll.hCalledAddress));
         if (rc) break;

         pBindRq->stAll.sSessProtVers = (signed   short int) RCro51_ProtVers2;
 
         pBindRq->stAll.lModeSelector =  (signed long int)  
                pAEInv->vApplCtxtInfos[(signed short int)pBndReqPb->r_apcon].
                                             lModeSelector;
         usAssocCnfNum = 
                pAEInv->vApplCtxtInfos[(signed short int)pBndReqPb->r_apcon].
                                             usAseConfig;
         /* ---------------------------------------------------------- */   
       
         /* --------- Assign other parameters if any ----------------- */   
         /* ----------- Buffer in p2 --------------------------------- */
         rc = ro131j_storeBuffer((RO_buffer         FAR *)&pBndReqPb->r_buffer,
                                  (unsigned long int FAR *)&p2);
         if (rc) break;

         p4 = (sCnfNo == RC0241_CfRoseOnly )? 1 : 0 ;
         sMode = 0;    /*new assoc */
         ulEcb = pBndReqPb->r_aplid;
         lEvent = RC0701_RO_BINDreq;
 
         /* --------- unlocks the APInvoc ---------------------------- */   
         if (ro0032_localunlock(hApplInv) ) rc =-R_LULOKE;
 
        }
        break;
    case RO_BINRS:
         {
         RO_bnrspb            FAR * pBndRspPb;
         rT0703_stBindRspRopm     * pBindRs;



         pBndRspPb = (RO_bnrspb FAR *)&(pParlst->r_bnrspb);

         /* ----------- Allocate the P3 ------------------------------ */   
         p3 = ro0030_localalloc(sizeof (rT0703_stBindRspRopm));
         if (!p3) { rc = -R_LALLCE; break; }

         pBindRs = (rT0703_stBindRspRopm NEAR *)ro0031_locallock (p3);
         if (!pBindRs) { rc = -R_LLOCKE; break; }

        
         /* ----------- copy all the parameters ---------------------- */   
         /* ----------- copy Ropm parameters ------------------------- */   
         pBindRs->stRopm.ulAssocClass = (unsigned long int)
                                                 pBndRspPb->r_aclass;

 
         pBindRs->stRopm.ulCheckPointSize = (unsigned long int) 
                                                RC1330_CheckPtSize;

         pBindRs->stRopm.ulWindowSize = (unsigned long int)
                                               RC1331_WindowSize;
 
         /* ----------- copy Acpm parameters ------------------------- */   
         pBindRs->stAcpm.sApplContextId = (signed   short int)
                              pBndRspPb->r_apcon;

         rc = ro131e_copyAplTitle(&(pBindRs->stAcpm.hAeTitle) ,
                                   &(pBndRspPb->r_aetitl) );
         if (rc) break;


         if ( pBndRspPb->r_invcid.r_apivid ) {
             pBindRs->stAcpm.stInvoc.bApInvocId = TRUE;
             pBindRs->stAcpm.stInvoc.lApInvocId = 
              (signed long int)*(pBndRspPb->r_invcid.r_apivid);
         }else{
             pBindRs->stAcpm.stInvoc.bApInvocId = (signed long int) 0;  
             pBindRs->stAcpm.stInvoc.lApInvocId = (signed long int) 0;  
         }
         if ( pBndRspPb->r_invcid.r_aeivid ) {
             pBindRs->stAcpm.stInvoc.bAeInvocId = TRUE;
             pBindRs->stAcpm.stInvoc.lAeInvocId = 
              (signed long int)*(pBndRspPb->r_invcid.r_aeivid);
         }else{
             pBindRs->stAcpm.stInvoc.bAeInvocId = (signed long int) 0;  
             pBindRs->stAcpm.stInvoc.lAeInvocId = (signed long int) 0;  
         } 
         /* ----------- copy stPpm parameters ------------------------ */   
         /*!!! from where to get the AbsSyn Result */
         /* --- Get using Inform 30/03 ----------- */
 
         rc = ro131t_GetAbsSynRslt (
                      hScb                                                   ,
                      pBndRspPb->r_assid                                     ,
                     (rTro65_pfvAbstrSynResult)pBindRs->stPpm.vAbstrSynResult);
         if (rc) break;
       
         pBindRs->stPpm.sDefContResult = (signed short int)
                                               RCro6i_NoDefContext;

                
         pBindRs->stPpm.ulRequirements = (unsigned long int)   
                                               RCro60_Kernel;
 
         /* ----------- copy stAll parameters ------------------------ */   
       if ( pBndRspPb->r_pradd.r_paddln )
             rc = ro131d_storeAddress (pBndRspPb->r_pradd.r_paddrf,
                                       pBndRspPb->r_pradd.r_paddln,
            (unsigned long int FAR *)&(pBindRs->stAll.hRespondAddress));
       if (rc) break;
     
       pBindRs->stAll.sSessProtVers = (signed   short int) RCro51_ProtVers2;
 
 
       /* --------- Assign other parameters if any ----------------- */   
       p2 = 0;
       p4 = pBndRspPb->r_result; 
       sMode = (p4 & 0x000000f0) ? -1 : 1 ;     /*!!!*/
       lEvent = RC0703_RO_BINDrsp;
 
          
       }
       break;

    case RO_UBNRQ:
         {
         RO_ubrqpb        FAR * pUBndReqPb;

         pUBndReqPb = (RO_ubrqpb FAR *)&(pParlst->r_ubrqpb);

         /* ----------- Allocate the P2 ------------------------------ */   
         /* ----------- No P2 ---------------------------------------- */
          p2 = 0; 
        
         /* ----------- copy all the parameters ---------------------- */   
         /* ------------   No parameters ----------------------------- */
          p3 = 0;

         /* ---------------------------------------------------------- */   
       
         /* --------- Assign other parameters if any ----------------- */   
         /* ----------- Buffer in p2 --------------------------------- */
          rc = ro131j_storeBuffer((RO_buffer        FAR *)&pUBndReqPb->r_buffer,
                                 (unsigned long int FAR *)&p3);
          if (rc) break;

          p4 = 0;
          sMode = 1;    /*old assoc */
          lEvent = RC0705_RO_UNBINDreq;
 
 
         } 
        break;
    case RO_UBNRS:
         {
         RO_ubrspb        FAR * pUBndRspPb;

         pUBndRspPb = (RO_ubrspb FAR *)&(pParlst->r_ubrspb);

         /* ----------- Allocate the P3 ------------------------------ */   
         /* ----------- No P3 ---------------------------------------- */
          p3 = 0; 
        
         /* ----------- copy all the parameters ---------------------- */   
         /* ------------   No parameters ----------------------------- */

         /* ---------------------------------------------------------- */   
       
         /* --------- Assign other parameters if any ----------------- */   
          p2 = pUBndRspPb->r_ureasn;  /* Reason */
          p4 = RCro90_Affirmative;    /* Result */
          sMode = -1;    /*!!! assoc */
          lEvent = RC0707_RO_UNBINDrsp;
 
         } 
        break;
    case RO_ABORQ:
         {
         RO_abrqpb        FAR * pAbrtReqPb;

         pAbrtReqPb = (RO_abrqpb FAR *)&(pParlst->r_abrqpb);

         /* ----------- Allocate the P3 ------------------------------ */   
         /* ----------- No P3 ---------------------------------------- */
          p3 = 0; 
        
         /* ----------- copy all the parameters ---------------------- */   
         /* ------------   No parameters ----------------------------- */

         /* ---------------------------------------------------------- */   
       
         /* --------- Assign other parameters if any ----------------- */   
          p2 = 0;
          p4 = RO_UABORT;     /* Abort reason ( Peer User) */
          sMode = -2;   /* !!! */ 
          lEvent = RC0709_RO_UABORTreq;
 
         } 
        break;
 } /*end of switch */
 /* --------------- Now copy the User Data --------------------------- */

if (rc) RM0003_RETURN (rs1313, signed short int, rc)                                    

if ( ( pSnFragData ) &&
     ( pSnFragData->r_fraref != NULL) &&
     ( pSnFragData->r_fralen  >  NULL)   ) {


      rc = ro131g_storeUserData( pSnFragData,
                            pSnFragData->r_fracnt, 
                            pSnFragData->r_fraary[0].r_absid,
                            (unsigned long int FAR *)0, 
                            0,
                            pSnFragData->r_fralen,
                            (unsigned long int FAR *)&p1);
 
} else /* No send Data */
     p1 =0;

if (!rc && p3) {
    if (ro0032_localunlock(p3)) rc = -R_LULOKE;
}

 /* --------------- call the dispatcher function send --------------- */
if (!rc)  rc = ro0270_send( hScb, sMode,ulEcb,
                             &ulAcb,RC0221_RoseUser,
                             lEvent,
                             &p1,&p2,&p3,&p4);

if ((!rc) && (lEvent == RC0701_RO_BINDreq))
          pParlst->r_bnrqpb.r_assid = ulAcb;

/* ---------- releasee p1 & p3 if !!! */
/* only if a handle is back it is to free */                          

if ( p1 && (! rc))  rc = ro131i_freeUserData (p1);                    
if ( p3 && (! rc))  if (ro0034_localfree (p3)) rc = -R_LFREEE;    

                
RM0003_RETURN (rs1313, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1314_Operation				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-16                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro1314_Operation (
         
       unsigned long  int     	        hScb,
       RO_excopb 	   FAR	       *pParlst)



/*                                                                    */
/* DESCRIPTION  :                                                     */
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
signed   short  int   rc;

unsigned long  int        ulAcb;              
unsigned long  int        p1      = LNULL;
unsigned long  int        p2      = LNULL;
unsigned long  int        p3      = LNULL;
unsigned long  int        p4      = LNULL;
RO_sndata          FAR  * pSnData ;
signed   short int        sAbsId;
signed   long  int        lEvent;

signed   short int        sSnFileLen;
signed   short int        sBfFileLen;

signed  short int         bLegal; 

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1311)
 rc = OK;
 /*------------------- Test the parameters -------------------------- */
  
 switch(pParlst->r_hdinfo.r_fnctnb){
    case RO_INVRQ:
        {
         RO_ivrqpb    FAR * pInvReq ;

         pInvReq = (RO_ivrqpb FAR *)&(pParlst->r_ivrqpb);

         sSnFileLen = ( (pInvReq->r_sndata.r_fralen) &&
                        (pInvReq->r_sndata.r_fraref) &&
                        (pInvReq->r_sndata.r_mtype == RO_FILE) )   ?
                      (signed short int)pInvReq->r_sndata.r_fralen :
                      (signed short int)0                          ;

         sBfFileLen = ( (pInvReq->r_buffer.r_buflen) &&
                       (pInvReq->r_buffer.r_bufref) &&
                       (pInvReq->r_buffer.r_mtype == RO_FILE)  )  ?
                     (signed short int)pInvReq->r_buffer.r_buflen :
                     (signed short int)0                          ;

         /* -------------- write login ----------------------------- */
	 ro0043_pack10Log (RC0020_INTERFACE		       ,
				 RL1313_EnINVreq		       ,
				 6				       ,
				 (signed short int) sizeof (RO_ivrqpb) ,
				 (char FAR *)	    pInvReq            ,
				 (signed short int) sSnFileLen	       ,
				 (char FAR *)pInvReq->r_sndata.r_fraref,
				 (signed short int) sBfFileLen	       ,
				 (char FAR *)pInvReq->r_buffer.r_bufref,
				 (pInvReq->r_lnkid)		       ?
				 (signed short int) sizeof (long)      :
				 (signed short int) 0		       ,
                                 (char FAR *)pInvReq->r_lnkid          ,
				 (pInvReq->r_opval.r_oploc)	       ?
				 (signed short int) sizeof (long)      :
				 (signed short int) 0		       ,
				 (char FAR *) pInvReq->r_opval.r_oploc ,
				 (pInvReq->r_opval.r_opglb)            ?
                                 (signed short int)(pInvReq->r_opval.
                                    r_opglb[0]*sizeof(unsigned long int)):
				 (signed short int) 0		       ,
				 (char FAR *)((pInvReq->r_opval.r_opglb)+1));

         /* --------------check the operation type ----------------- */
         /* ------------ Check the Legality of the function ---------- */
         rc = ro131s_CheckLegalOcur(
                                    hScb                            ,
                                    pInvReq->r_assid                ,
                                    RC070c_RO_INVreq                ,
                                    (signed short int FAR *)&bLegal );
         if ((rc) || (!bLegal)) { rc = -R_FCTCAL; break; }

         if ( (pInvReq->r_oclass  < RO_OPC1 ) ||
              (pInvReq->r_oclass  > RO_OPC5 ) )  { rc = -R_HOCLAS; break;  }

         /* ------------- check the AbsSyn Id ----------------------- */
          /*
         if ( (pInvReq->r_absid < RCro20_MinAbstrSynNumb ) ||
              (pInvReq->r_absid > RCro21_MaxAbstrSynNumb ) )
                              { rc =-R_HWABSI; break; } */

         rc = ro131u_findAbsId (hScb , -1 ,pInvReq->r_absid);
         if (rc) break;
         
         /* ------------- Check Buffer ------------------------------- */   
         /* !!! dos unix incompatible in Global Handle */
        if ( (pInvReq->r_buffer.r_buflen ) &&
             (pInvReq->r_buffer.r_bufref )  )  {
 
#ifdef RP0000_MicV0600
            if  ( (pInvReq->r_buffer.r_mtype < RO_MEMORY ) ||
                  (pInvReq->r_buffer.r_mtype > RO_GLOHAN   )  ) rc = -R_HMTYPE;
#else 
            if  ( (pInvReq->r_buffer.r_mtype < RO_MEMORY ) ||
                  (pInvReq->r_buffer.r_mtype > RO_FILE   )  ) rc = -R_HMTYPE;
#endif
        }
        if (rc) break;

        /* ----------- check for OP Type - 5 & buffer Occurence ------ */

        if ( (pInvReq->r_oclass == RO_OPC5 )  &&
             (pInvReq->r_buffer.r_bufref != NULL) ) { rc = -R_HOCLAS; break; }

        /* ------------ assign the User Send data -------------------- */
        if ( (pInvReq->r_sndata.r_fralen) &&
             (pInvReq->r_sndata.r_fraref)  )
                  pSnData = (RO_sndata FAR *)&(pInvReq->r_sndata);
        else pSnData = NULL;
        }
        break;

   case RO_RESRQ:
       {
         RO_rsrqpb   FAR * pRstReq ;
   
         pRstReq = (RO_rsrqpb FAR *)&(pParlst->r_rsrqpb);

         sSnFileLen = ( (pRstReq->r_sndata.r_fralen) &&
                        (pRstReq->r_sndata.r_fraref) &&
                        (pRstReq->r_sndata.r_mtype == RO_FILE) )   ?
                      (signed short int)pRstReq->r_sndata.r_fralen :
                      (signed short int)0                          ;

         /* -------------- write login ----------------------------- */
         ro0043_pack10Log (RC0020_INTERFACE		       ,
				 RL1315_EnRESreq		       ,
				 4				       ,
				 (signed short int) sizeof (RO_rsrqpb) ,
				 (char FAR *)	    pRstReq            ,
				 (signed short int) sSnFileLen	       ,
				 (char FAR *)pRstReq->r_sndata.r_fraref,
				 (pRstReq->r_opval.r_oploc)	       ?
				 (signed short int) sizeof (long)      :
				 (signed short int) 0		       ,
				 (char FAR *) pRstReq->r_opval.r_oploc ,
				 (pRstReq->r_opval.r_opglb)            ?
                                 (signed short int)(pRstReq->r_opval.
                                    r_opglb[0]*sizeof(unsigned long int)):
				 (signed short int) 0		       ,
				 (char FAR *)((pRstReq->r_opval.r_opglb)+1));

         /* ------------ Check the Legality of the function ---------- */
          rc = ro131s_CheckLegalOcur(
                                    hScb                            ,
                                    pRstReq->r_assid                ,
                                    RC070e_RO_RESreq                ,
                                    (signed short int FAR *)&bLegal );
         if ((rc) || (!bLegal)) { rc = -R_FCTCAL;  break; }

         /* ------------- check the AbsSyn Id ----------------------- */
/*
         if ( (pRstReq->r_absid < RCro20_MinAbstrSynNumb ) ||
              (pRstReq->r_absid > RCro21_MaxAbstrSynNumb ) )
                                 { rc =-R_HWABSI; break; } */
         rc = ro131u_findAbsId (hScb , -1 ,pRstReq->r_absid );
         if (rc) break;

        /* ------------ assign the User Send data -------------------- */
        if ( (pRstReq->r_sndata.r_fralen) &&
             (pRstReq->r_sndata.r_fraref)  )
              pSnData = (RO_sndata FAR *)&(pRstReq->r_sndata);
        else pSnData = NULL;
       }
       break;

   case RO_ERRRQ:
       {
         RO_errqpb    FAR * pErrReq ;
   
         pErrReq = (RO_errqpb FAR *)&(pParlst->r_errqpb);

         sSnFileLen = ( (pErrReq->r_sndata.r_fralen) &&
                        (pErrReq->r_sndata.r_fraref) &&
                        (pErrReq->r_sndata.r_mtype == RO_FILE) )   ?
                      (signed short int)pErrReq->r_sndata.r_fralen :
                      (signed short int)0                          ;

         /* -------------- write login ----------------------------- */
         ro0043_pack10Log (RC0020_INTERFACE		       ,
				 RL1317_EnERRreq		       ,
				 4				       ,
				 (signed short int) sizeof (RO_errqpb) ,
				 (char FAR *)	    pErrReq            ,
				 (signed short int) sSnFileLen	       ,
				 (char FAR *)pErrReq->r_sndata.r_fraref,
				 (pErrReq->r_erval.r_oploc)	       ?
				 (signed short int) sizeof (long)      :
				 (signed short int) 0		       ,
				 (char FAR *) pErrReq->r_erval.r_oploc ,
				 (pErrReq->r_erval.r_opglb)            ?
                                 (signed short int)(pErrReq->r_erval.
                                   r_opglb[0]*sizeof(unsigned long int)):
				 (signed short int) 0		       ,
				 (char FAR *)((pErrReq->r_erval.r_opglb)+1)   );

         /* ------------ Check the Legality of the function ---------- */
         rc = ro131s_CheckLegalOcur(
                                    hScb                            ,
                                    pErrReq->r_assid                ,
                                    RC070g_RO_ERRreq                ,
                                    (signed short int FAR *)&bLegal );
         if ((rc) || (!bLegal)) { rc = -R_FCTCAL; break; }

         /* ------------- check the AbsSyn Id ----------------------- */
  /*
         if ( (pErrReq->r_absid < RCro20_MinAbstrSynNumb ) ||
              (pErrReq->r_absid > RCro21_MaxAbstrSynNumb ) )
                                      rc =-R_HWABSI;  */

         rc = ro131u_findAbsId (hScb , -1 ,pErrReq->r_absid );
         if (rc) break;

        /* ------------ assign the User Send data -------------------- */
        if ( (pErrReq->r_sndata.r_fralen) &&
             (pErrReq->r_sndata.r_fraref)  )
             pSnData = (RO_sndata FAR *)&(pErrReq->r_sndata);
        else pSnData = NULL;
       }
       break;

   case RO_RJURQ:
       {
         RO_rurqpb    FAR * pRjUReq ;
   
         pRjUReq = (RO_rurqpb FAR *)&(pParlst->r_rurqpb);
         /* -------------- write login ----------------------------- */
         ro0042_writeLog	(RC0020_INTERFACE		,
				 RL1319_EnUREJreq		,
				 sizeof (RO_rurqpb)		,
				 (char FAR*) pRjUReq);

         /* ------------ Check the Legality of the function ---------- */
         rc = ro131s_CheckLegalOcur(
                                    hScb                            ,
                                    pRjUReq->r_assid                ,
                                    RC070i_RO_RJUreq                ,
                                    (signed short int FAR *)&bLegal );
         if ((rc )|| (!bLegal)) { rc = -R_FCTCAL;  break; }

         /* ------------- check the AbsSyn Id ----------------------- */
/*
         if ( (pRjUReq->r_absid < RCro20_MinAbstrSynNumb ) ||
              (pRjUReq->r_absid > RCro21_MaxAbstrSynNumb ) )
                                      rc =-R_HWABSI;  */

         rc = ro131u_findAbsId (hScb , -1 ,pRjUReq->r_absid );
         if (rc) break;

        /* ------------ check the reason ----------------------------  */
         if        (pRjUReq->r_reason < RO_RINDU)     rc = -R_HRRSER;
         else if ( (pRjUReq->r_reason > RO_RINUC) &&
                   (pRjUReq->r_reason < RO_RREUI) )   rc = -R_HRRSER;
         else if ( (pRjUReq->r_reason > RO_RREMR) &&
                   (pRjUReq->r_reason < RO_RERUI) )   rc = -R_HRRSER;
         else if (  pRjUReq->r_reason > RO_RERMP)     rc = -R_HRRSER;

        /* ------------ assign the User Send data -------------------- */
        pSnData = NULL; 
       }
       break;
 }/*end of switch */

 /* Check the common elements of the INVreq  RESreq Errreq     ----- */
 /* RJUreq                 ------------------------------------------ */
 /* ----------- check sndata parameters ----------------------------- */
 /* !!! dos unix incompatible in Global Handle */

if (!rc && pSnData){
#ifdef RP0000_MicV0600
  if ( (pSnData->r_mtype < RO_MEMORY )||
       (pSnData->r_mtype > RO_GLOHAN   ) ) rc = -R_HMTYPE;
#else

  if ( (pSnData->r_mtype < RO_MEMORY )||
       (pSnData->r_mtype > RO_FILE   ) ) rc = -R_HMTYPE;
#endif
  if (!rc)
      if ( pSnData->r_fralen > (unsigned long int )MAXLONG_POS ) rc = -R_HUDTLE;
}

if (rc) RM0003_RETURN (rs1311,signed short int,rc)

switch(pParlst->r_hdinfo.r_fnctnb){
    case RO_INVRQ:
        {
         RO_ivrqpb        FAR * pInvReqPb;
         rT070b_stInvReq      * pInvReq;

         pInvReqPb = (RO_ivrqpb FAR *)&(pParlst->r_ivrqpb);
         /* ----------- Allocate the P3 ------------------------------ */   
         p3 = ro0030_localalloc(sizeof (rT070b_stInvReq));
         if (!p3) { rc = -R_LALLCE; break; }

         pInvReq = (rT070b_stInvReq NEAR *)ro0031_locallock (p3);
         if (!pInvReq) { rc = -R_LLOCKE; break; }

         /* ----------- copy all the parameters ---------------------- */   
         /* ----------- copy Ropm parameters ------------------------- */   
      
         /* ----------- copy OpValue --------------------------------- */

         rc = ro131k_CpyOpValue (
                       (RO_opval           FAR *)&(pInvReqPb->r_opval),
                       (rTro76_stDefinedBy FAR *)&(pInvReq->stOperationValue),
                        0); 
         if (rc) break;

          /* -------- Operation Class --------------------------------- */

          pInvReq->ulOperationClass = (unsigned long int)pInvReqPb->r_oclass; 
                 
          /* ---------------- Invoke Id ------------------------------- */ 
                  
          pInvReq->lInvokeID    =
          p4                    = (signed long int)pInvReqPb->r_invid; 
         
          /* ---------------- Linked Id ------------------------------- */

          if(pInvReqPb->r_lnkid){  /* if present */
              pInvReq->bLinkedID = TRUE;        
              pInvReq->lLinkedID = (signed long int) *(pInvReqPb->r_lnkid); 
          }else
              pInvReq->bLinkedID = FALSE;
     
         /* ---------- Priority --------------------------------------- */
 
          pInvReq->lPriority = 0; /* !!! check with shiv */
 
         /* ------------- Copy AbsId ----------------------------------- */
          pInvReq->sAbsId = sAbsId = pInvReqPb->r_absid;

         /* ------------------------------------------------------------ */

       
         /* --------- Assign other parameters if any ----------------- */   
          p2 =0;
          rc = ro131j_storeBuffer((RO_buffer         FAR *)&pInvReqPb->r_buffer,
                                  (unsigned long int FAR *)&p2);
          if (rc) break;

          ulAcb = pInvReqPb->r_assid;
          lEvent = RC070c_RO_INVreq;
        }
        break;
 
   case RO_RESRQ:
       {
         RO_rsrqpb        FAR * pResReqPb;
         rT070d_stResReq      * pResReq;


         pResReqPb = (RO_rsrqpb FAR *)&(pParlst->r_rsrqpb);

         /* ----------- Allocate the P3 ------------------------------ */   
         p3 = ro0030_localalloc(sizeof (rT070d_stResReq));
         if (!p3)       { rc = -R_LALLCE; break; }

         pResReq = (rT070d_stResReq NEAR *)ro0031_locallock (p3);
         if (!pResReq)  { rc = -R_LLOCKE; break; }

         /* ----------- copy all the parameters ---------------------- */   
         /* ----------- copy Ropm parameters ------------------------- */   
         if (  (!pResReqPb->r_opval.r_oploc) &&
                (!(pResReqPb->r_opval.r_opglb) &&
                 ((*(pResReqPb->r_opval.r_opglb)))) ) /* OpVal are NULL */
            if (pSnData){ /* Data present */  
              rc=-R_HWOPVL;
              break;
            }
         if ( ( (!pResReqPb->r_opval.r_oploc) &&
                (!(pResReqPb->r_opval.r_opglb) &&
                 ((*(pResReqPb->r_opval.r_opglb)))) )  ||
              (!pSnData                         )  ) /* No OpValue */
            pResReq->bOperationValue = FALSE; 
         else {  /* Copy Opvalue */
            pResReq->bOperationValue = TRUE;
            rc = ro131k_CpyOpValue (
                       (RO_opval           FAR *)&(pResReqPb->r_opval),
                       (rTro76_stDefinedBy FAR *)&(pResReq->stOperationValue),
                       0);
            if (rc) break;
         }

         /* --------------- Invoke ID -------------------------------- */
         pResReq->lInvokeID = (signed long int)pResReqPb->r_invid;     
     
         /* ---------- Priority --------------------------------------- */
         pResReq->lPriority  = 0;   /*!!! check */      

         /* ------------- Copy AbsId ----------------------------------- */
          pResReq->sAbsId = sAbsId = pResReqPb->r_absid;

         /* --------- Assign other parameters if any ----------------- */   
          p2 = 0;
          p4 = 0 ;
          ulAcb = pResReqPb->r_assid;
          lEvent = RC070e_RO_RESreq;
       }
       break;
 
   case RO_ERRRQ:
       {
         RO_errqpb        FAR * pErrReqPb;
         rT070f_stErrReq      * pErrReq;


         pErrReqPb = (RO_errqpb FAR *)&(pParlst->r_errqpb);

         /* ----------- Allocate the P3 ------------------------------ */   
         p3 = ro0030_localalloc(sizeof (rT070f_stErrReq));
         if (!p3) { rc = -R_LALLCE; break; }

         pErrReq = (rT070f_stErrReq NEAR *)ro0031_locallock (p3);
         if (!pErrReq) { rc = -R_LLOCKE; break; }

         /* ----------- copy all the parameters ---------------------- */   
         /* ----------- copy Ropm parameters ------------------------- */   

         /* ----------- Err Value ------------------------------------ */

          rc = ro131k_CpyOpValue (
                   (RO_opval           FAR *)&(pErrReqPb->r_erval),
                   (rTro76_stDefinedBy FAR *)&(pErrReq->stErrorValue),
                    0);
          if (rc) break;

         /* --------------- Invoke ID -------------------------------- */
         pErrReq->lInvokeID = (signed long int)pErrReqPb->r_invid;     
     
         /* ---------- Priority --------------------------------------- */
         pErrReq->lPriority  = 0;   /*!!! check */      


         /* ------------- Copy AbsId ----------------------------------- */
          pErrReq->sAbsId = sAbsId = pErrReqPb->r_absid;
         /* --------- Assign other parameters if any ----------------- */   
          p2 = 0;
          p4 = 0;
          ulAcb = pErrReqPb->r_assid;
          lEvent = RC070g_RO_ERRreq;
       }
       break;
 
   case RO_RJURQ:
       {
         RO_rurqpb        FAR * pRjUReqPb;
         rT070h_stURejReq     * pRjUReq;

         pRjUReqPb = (RO_rurqpb FAR *)&(pParlst->r_rurqpb);

         /* ----------- Allocate the P3 ------------------------------ */   
         p3 = ro0030_localalloc(sizeof (rT070h_stURejReq));
         if (!p3)      { rc = -R_LALLCE; break; }

         pRjUReq = (rT070h_stURejReq NEAR *)ro0031_locallock (p3);
         if (!pRjUReq) { rc = -R_LLOCKE; break; }

         /* ----------- copy all the parameters ---------------------- */   
         /* ----------- copy Ropm parameters ------------------------- */   

         /* -------------- Reject Reason ------------------------------ */
         pRjUReq->ulRejReason = (unsigned long int) pRjUReqPb->r_reason; 
         /* --------------- Invoke ID -------------------------------- */
         pRjUReq->lInvokeID = (signed long int)pRjUReqPb->r_invid;     
     
         /* ---------- Priority --------------------------------------- */
         pRjUReq->lPriority  = 0;   /*!!! check */      


         /* ------------- Copy AbsId ----------------------------------- */
          pRjUReq->sAbsId = sAbsId = pRjUReqPb->r_absid;
         /* --------- Assign other parameters if any ----------------- */   
          p2 = 0;
          p4 = 0;
          ulAcb = pRjUReqPb->r_assid;
          lEvent = RC070i_RO_RJUreq;
 
       }
       break;
 }/*end of switch */

 /* --------------- Assign p1 User Data ------------------------------- */
 
 /* --------------- Now copy the User Data --------------------------- */
 if ( (!rc) && pSnData)
    rc = ro131g_storeUserData (
                      (RO_snfrag FAR *)pSnData,
                      0,       /* fragment count  it will be count-1*/ 
                      sAbsId,
                      (unsigned long int FAR *)0, 
                      0,                 /* Offset */
                      pSnData->r_fralen,
                      (unsigned long int FAR *)&p1);
    

if (rc) 
        RM0003_RETURN (rs1312, signed short int, rc)
 /* !!! check */
if (p3)
    if (ro0032_localunlock (p3)) rc = -R_LULOKE;

  
 /* --------------- call the dispatcher function send --------------- */
if (!rc) 
    rc = ro0270_send ( hScb, 1, 0,
                       &ulAcb,RC0221_RoseUser,
                       lEvent,
                       &p1, &p2, &p3, &p4);

 /* ---------- releasee p1 & p3 if !!! */
  /* only if a handle is back it is to free */                          

if ( p1 && (! rc))  rc = ro131i_freeUserData (p1);                    
if ( p3 && (! rc))  if ( ro0034_localfree    (p3))  rc = -R_LFREEE;    


RM0003_RETURN (rs1312, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1315_provBuf  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-18                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int NEAR PASCAL ro1315_provBuf (
         
       unsigned long  int     	        hScb,
       RO_pvbfpb 	   FAR	       *pProvBuf)



/*                                                                    */
/* DESCRIPTION  :                                                     */
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

signed	 short int	 rc = OK;
unsigned long  int	ulEcb;
unsigned long  int	ulBuf;
signed  short  int      sBfFileLen;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1315)



/* ****************** Write interface logging *********************** */
/* ******************			      *********************** */
 
/* !!! log function */


sBfFileLen = ( (pProvBuf->r_buffer.r_buflen) &&
               (pProvBuf->r_buffer.r_bufref) &&
               (pProvBuf->r_buffer.r_mtype == RO_FILE)  )
           ?  (signed short int)pProvBuf->r_buffer.r_buflen
           :  (signed short int)0                          ;

ro0043_pack10Log (RC0020_INTERFACE			 ,
		  RL131m_EnPROVBUFER			 ,
		  2					 ,
		  (signed short int) sizeof (RO_pvbfpb)	 ,
		  (char FAR *)	     pProvBuf		 ,
		  (signed short int) sBfFileLen		 ,
		  (char FAR *)pProvBuf->r_buffer.r_bufref);

/* ****************** Check input parameter ************************* */
/* ******************			    ************************* */

if ( (pProvBuf->r_buffer.r_buflen ) &&
     (pProvBuf->r_buffer.r_bufref )  ) {
#ifdef RP0000_MicV0600
            if  ( (pProvBuf->r_buffer.r_mtype < RO_MEMORY ) ||
                  (pProvBuf->r_buffer.r_mtype > RO_GLOHAN   )  ) rc = -R_HMTYPE;
#else 
            if  ( (pProvBuf->r_buffer.r_mtype < RO_MEMORY ) ||
                  (pProvBuf->r_buffer.r_mtype > RO_FILE   )  ) rc = -R_HMTYPE;
#endif
        }
else if ((pProvBuf->r_buffer.r_bufref == (char FAR *) 0) ||
	 (pProvBuf->r_buffer.r_buflen ==	       0))     rc = -R_HBUFER;

else						               rc = 0;

if (! rc) {
       ulEcb = pProvBuf->r_aplid;
       rc = ro131j_storeBuffer((RO_buffer         FAR *)&pProvBuf->r_buffer,
			       (unsigned long int FAR *)&ulBuf		     );
} else {
       ulBuf = 0;
}

/* ****************** Call dispatcher provied buffer **************** */


if (! rc)
    rc = ro0262_provideBuffer (hScb, ulEcb, &ulBuf);

RM0003_RETURN (rs1315, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1316_redirect				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-03-18					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed	short int NEAR PASCAL ro1316_redirect (

       unsigned long  int        hScb,
       RO_rdrqpb 	  FAR *  pRdirReq)

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

signed	 short int	 rc = OK;
unsigned long  int       ulP1       = LNULL;
unsigned long  int       ulP2       = LNULL;
unsigned long  int       ulP3       = LNULL;
unsigned long  int       ulP4       = LNULL;
unsigned long  int       ulAcb      = LNULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1316)

 ulP2 = pRdirReq->r_pid;
 ulP4 = (RO_ACCEPT | RCro85_Redirection);
 ulAcb = pRdirReq->r_assid;

 rc = ro0270_send (hScb, -1,0,
                   &ulAcb,RC0221_RoseUser,
                   RC0703_RO_BINDrsp,
                   &ulP1,&ulP2,&ulP3,&ulP4);

RM0003_RETURN (rs1316, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1317_Inform  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-03-18					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed	short int NEAR PASCAL ro1317_Inform (

       unsigned long  int        hScb,
       RO_inform 	  FAR *  pInform)

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

signed	 short int	 rc,i,j; /* K02 j is added */
signed   short int       sLen=0;   
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1317)
 rc = OK;
 
 switch(pInform->r_inftyp){
     case RO_GTDCS:
          {
            RO_gtdcif    FAR * pStGetDcs;
            rT04z0_stDCS       stPpmDcs;
            unsigned long  int       ulEcb;
            
            sLen = sizeof(RO_gtdcif);   
            pStGetDcs  =(RO_gtdcif FAR * ) pInform->r_infval; 

            /* ---------------------------------------------------------- */
           
            /* ----- check the Assoc Id for Validity -------------------- */
            rc = ro0269_AcbGetEcb (hScb,pStGetDcs->r_assid,&ulEcb);
            if (rc == -R_WASSOC)
                rc = ro0273_getPmInfo2 ( hScb                          ,
                                         RC0241_CfRoseOnly             ,
                                         RC0215_Presentation           ,
                                         RC04zR_getDCS                 ,
                                         (rT04z0_stDCS FAR *)&stPpmDcs );
            else if (rc == OK)          /* Assoc. Exists */
                rc = ro0264_getPmInfo (hScb          ,
                                   pStGetDcs->r_assid,
                                   RC0215_Presentation,
                                   RC04zR_getDCS,
                                   (rT04z0_stDCS FAR *)&stPpmDcs);
            /* ------------ copy the prmeters back to RO_gtdcif --------- */
            if (!rc) {
                pStGetDcs->r_pcnum = (int)stPpmDcs.pcnum;
                for (j=1,i=0;j<= stPpmDcs.vAbstrSynResult[0];j++)            
                 if ( stPpmDcs.vAbstrSynResult[j] == RCro6c_Acceptance ) 
                 pStGetDcs->r_prcxt[i++].r_absid =                   
                                   (int) stPpmDcs.vAbstrSynId[j];
              
                for(i=1;i<=stPpmDcs.pcnum;i++)  {
             /*        pStGetDcs->r_prcxt[i-1].r_absid = 
                             (int) stPpmDcs.vAbstrSynId[i]; K02 */
                    pStGetDcs->r_prcxt[i-1].r_trsid = 
                (int) stPpmDcs.vTransSynId[pStGetDcs->r_prcxt[i-1].r_absid];
                    pStGetDcs->r_prcxt[i-1].r_pctid = 
                (int) stPpmDcs.vPresContId[pStGetDcs->r_prcxt[i-1].r_absid];
                
                } 
            } else 
                pStGetDcs->r_pcnum = NULL;
          }
          break;

     case RO_GTPBF:
          {
            /*!!! call dispatcher's function to get the Number */
           RO_gtpbif  FAR * pstGtPrBfNumb;
           
           sLen = sizeof(RO_gtpbif);
           pstGtPrBfNumb  = (RO_gtpbif FAR * ) pInform->r_infval;
           rc = ro0274_getPrBfCount ( hScb , pstGtPrBfNumb->r_aplid,
                                      &(pstGtPrBfNumb->r_number)     );
           if (rc)   /* make number as 0 */
               pstGtPrBfNumb->r_number= NULL;

          }
          break;
     case RO_GPRID:  /* here the Pres.Cont. Ids are taken from dispatcher */
          {
           unsigned long  int ulDummy;
           signed   short int sDummy;
           signed   long  int lDummy;
           unsigned long  int hApplInv=LNULL;
           rT0501_AEInvocation NEAR * pApplInv=LNULL;
           RO_gtprid          FAR * pGetPrId;
           signed   short int i,sCount;
           signed   long  int lContId=(signed long int)1;

           pGetPrId = (RO_gtprid FAR * ) pInform->r_infval;
           rc=ro0265_getEcbInfo(hScb,pGetPrId->r_aplid,
                                (signed short int FAR *)&sDummy,
                                (signed short int FAR *)&sDummy,
                                (signed long  int FAR *)&lDummy,
                                (unsigned long int FAR *)&ulDummy,
                                (unsigned long int FAR *)&ulDummy,
                                (unsigned long int FAR *)&ulDummy, 
                                (unsigned long int FAR *)&hApplInv);
          if(hApplInv){
           if(!(pApplInv = (rT0501_AEInvocation NEAR *)
                 ro0031_locallock (hApplInv)))
                   RM0003_RETURN (rs1317, signed short int, -R_LLOCKE)
          }else 
             RM0003_RETURN (rs1317, signed short int, rc)
          sCount = pApplInv->vApplCtxtInfos[pGetPrId->r_apcon].vAbstrSynId[0];
          sCount = MIN(sCount,RO_maxExternals);
          for(i=1;i<=sCount;i++,lContId += 2)
              if (pApplInv->vApplCtxtInfos[pGetPrId->r_apcon].vAbstrSynId[i]
                   == pGetPrId->r_absid)break;
          if(i > sCount ) pGetPrId->r_pctid = (signed long int)-1;
          else pGetPrId->r_pctid= lContId;
          if (ro0032_localunlock (hApplInv)) 
              RM0003_RETURN (rs1317, signed short int, -R_LULOKE) 
           
          }
          break;
     case RC06z1_ApplCxtName:
          {
           rT06z0_stInfrmApplCxtName FAR * pstAplIf =
                        (rT06z0_stInfrmApplCxtName FAR *) pInform->r_infval; 
           
           if (( pstAplIf->ulAssId == 0 ) && (pstAplIf->ulApplID==0)){
            pstAplIf->ulAssId = ro0214_getAcbEcb (&pstAplIf->ulApplID) ;	
            pstAplIf->ulAssId = NULL;
          }
           rc = ro131h_GetAplCxtNameId( hScb,
                                           pstAplIf->ulAssId    ,
                                           pstAplIf,
                                           (signed short int)
                                                 RC06z1_ApplCxtName);
          }
          break;

     case RC06z2_ApplCxtID:
          {
           rT06z0_stInfrmApplCxtName FAR * pstAplIf =
                        (rT06z0_stInfrmApplCxtName FAR *) pInform->r_infval; 
           if (( pstAplIf->ulAssId == 0 ) && (pstAplIf->ulApplID==0)){
            pstAplIf->ulAssId = ro0214_getAcbEcb (&pstAplIf->ulApplID) ;	
            pstAplIf->ulAssId = NULL;
          }
            rc = ro131h_GetAplCxtNameId(hScb,
                                           pstAplIf->ulAssId    ,
                                           pstAplIf,
                                           (signed short int)
                                                RC06z2_ApplCxtID);
          }
          break;

     case RC04zP_ACSEAbId:
     case RC04zQ_EXIFAbId:
          {
            rc = ro0273_getPmInfo2 ( hScb ,
                                     RC0241_CfRoseOnly ,
                                     RC0215_Presentation,
                                     pInform->r_inftyp,      
                                     (void FAR *)pInform->r_infval);
          }
          break;

     default:
          rc = -R_HNOSUP;
          break; 
 }/*end of switch */

 ro0043_pack10Log (RC0020_INTERFACE			 ,
                RL131u_RnInform			         ,
		2					 ,
                (signed short int) sizeof (RO_inform)	 ,
                (char FAR *)	pInform                  ,
                (signed short int) sLen		         ,
                (char FAR *)pInform->r_infval             );
RM0003_RETURN (rs1317, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1318_receive				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K                                      */
/* DATE 	: 93-03-19					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR  PASCAL ro1318_receive (

       unsigned long int        hScb,
       RO_chwtpb 	 FAR  * pChWait)
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

signed	 short int   rc;

unsigned long  int   ulEcb;
unsigned long  int   ulAcb;
unsigned long int    ulBlockTime;

signed	 long	int  lEvent;
unsigned long	int  ulP1;
unsigned long	int  ulP2;
unsigned long	int  ulP3;
unsigned long	int  ulP4;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1318)

/* ****************** Write interface logging *********************** */
/* ******************			      *********************** */

ro0042_writeLog (RC0023_CWAITTRACE,
		 RL131t_InTraCwait,
		 sizeof (RO_chwtpb),
		 (char FAR *) pChWait   );
/* ****************** Check input parameter ************************* */
/* ******************			    ************************* */

 if (pChWait->r_wtime > RO_INFIN) rc = -R_WTIME;
 else				  rc = OK;

/* ****************** Call dispatcher receive function ************** */
/* ****************** and issue computed event	       ************** */


if (! rc) {
      ulBlockTime = (pChWait->r_wtime < RO_INFIN) ? pChWait->r_wtime : -1;

      rc	= ro0271_receive (hScb                              ,
                                  (signed   short int)ulBlockTime   ,
                                  (unsigned long  int FAR *)&ulEcb  ,
                                  (unsigned long  int FAR *)&ulAcb  ,
				  (signed long  int FAR *)&lEvent ,
                                  (unsigned long  int FAR *)&ulP1   ,
                                  (unsigned long  int FAR *)&ulP2   ,
                                  (unsigned long  int FAR *)&ulP3   ,
                                  (unsigned long  int FAR *)&ulP4   );


      switch (rc)  {
      case  0: 
	       rc = ro131l_issueRosEvent(ulEcb	    , ulAcb	 ,
		                         lEvent	    , ulP1	 ,
				         ulP2	    , ulP3	 ,
				         ulP4	    , pChWait->r_evntrf);
	       break;

      case  2: pChWait->r_evntrf->r_evnthd.r_evtype = RO_NOEVET;
	       rc	  = RO_NOEVNT;
	       break;

      case  3: pChWait->r_evntrf->r_evnthd.r_evtype = RO_TIMEUT;
	       rc	  = RO_TIMOUT;
	       break;

      default: break;
      }
}

RM0003_RETURN (rs1318, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1310_roexcom				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-02-24					      */
/*                                                                    */
/* SYNTAX       :                                                     */

	signed short int FAR PASCAL ro1310_roexcom (
	unsigned long  int  FAR   *pScbHd,
	signed	 long  int	   lTaskId,
	FARPROC       		  tpduFct,
	RO_excopb	    FAR   *pParlst)

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

signed	 short int	   rc;

signed   short int         bExcont;


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1310)

rc = OK;

if(pParlst == NULL)
   return (RO_CALERR);
/* *********       Create Scb for attach              *************** */
/* *********  Use the no recovery mode                *************** */
if ( !(*pScbHd) ){
      if ( (pParlst->r_hdinfo.r_fnctnb == RO_ATACH) ){

    /* --------- for attach sys the mode ,lSyName,pSyName are ------ */
    /* ---- NULL . since they are used in Rdb. --------------------- */

          rc = ro0250_attachSys(pScbHd,0,lTaskId,tpduFct,0,0);
          if(rc) rc = ro131a_setHeader(&(pParlst->r_hdinfo),-rc);
      }
      /* kc on 30-8-93 */
     else  if ( ( pParlst->r_hdinfo.r_fnctnb == RO_LOGON ) ||                  
                 ( pParlst->r_hdinfo.r_fnctnb == RO_LOGOF ) ||                  
                 ( pParlst->r_hdinfo.r_fnctnb == RO_LOGWR ) ||                  
                 ( pParlst->r_hdinfo.r_fnctnb == RO_WAKE  ) ) ;                 
                                                                                
 
      else 
        rc = ro131a_setHeader(&(pParlst->r_hdinfo),-R_FCTCAL);

      if(rc < 0 )
        RM0003_RETURN (rs1310, signed short int, pParlst->r_hdinfo.r_retcod)
}


       
pParlst--;

do   {
     /* ************* Execute parameter block list ****************** */
     /* *************				   ****************** */

     pParlst++;

     /* check for external interface panic flag ********************* */
/*
     if (bExifPanic) {
         ro131a_setHeader(&(pParlst->r_hdinfo),-R_EXPANC);
	 break;

     }
*/

     /* ************* check for VERSION  **************************** */
     if (pParlst->r_hdinfo.r_versnb != RO_VERSION) {
         ro131a_setHeader(&(pParlst->r_hdinfo),-R_NSUPVS);
	 break;
     }
     if (pParlst->r_hdinfo.r_cont != RO_FALSE){           
        ro131a_setHeader(&pParlst->r_hdinfo,-R_HWCONT);   
        break;                                            
     }                                                         
     switch (pParlst->r_hdinfo.r_fnctnb)
     {
     /* ************* Execute local functions *********************** */
     /* *************			      *********************** */

     case RO_ATACH : 
                    bExcont = pParlst->r_hdinfo.r_cont;
/*                    pParlst->r_attapb.r_pid = lTaskId;  */
         /* !!! whether pid or aplid check with ross.h i/o arrow */ 
                    rc = ro1311_attach(*pScbHd,tpduFct,&(pParlst->r_attapb));
		    break;

     case RO_DTACH : 
                    bExcont = pParlst->r_hdinfo.r_cont;
                    rc = ro1312_detach(*pScbHd,&(pParlst->r_detapb));
		    break;

     case RO_CWAIT : 
		    bExcont   = FALSE;
		    rc	     = ro1318_receive (*pScbHd, &(pParlst->r_chwtpb));
		    break;

     case RO_PRBUF : 
                    bExcont = pParlst->r_hdinfo.r_cont;
                    rc = ro1315_provBuf(*pScbHd,&(pParlst->r_pvbfpb));
		    break;

     case RO_REDRQ : 
                    bExcont = pParlst->r_hdinfo.r_cont;
                    rc = ro1316_redirect(*pScbHd,&(pParlst->r_rdrqpb));
		    break;

     case RO_WAKE  : 
                    bExcont = pParlst->r_hdinfo.r_cont;
                    /* ---------- writelog !!! ---------------------- */
                                    
		    ro0042_writeLog (RC0020_INTERFACE		     ,
				     RL131o_EnWake		     ,
				     sizeof (RO_wakepb)		     ,
				     (char FAR *) &pParlst->r_wakepb);
                   /*
                    rc = ro0263_wake ( *pScbHd, pParlst->r_wakepb.r_pid); 
                     * Not supported in RTROSV3.00 ***** */
                    rc = -R_NSUPCA;
		    break;

     case RO_INFRM:
                   
                   bExcont = pParlst->r_hdinfo.r_cont;
                   rc = ro1317_Inform(*pScbHd,&(pParlst->r_inform)); 
                   break;


     /* ************* Execute binding, unbinding and abort ********** */
     /* *************					   ********** */

     case RO_BINRQ : 
     case RO_BINRS : 
     case RO_UBNRQ : 
     case RO_UBNRS : 
     case RO_ABORQ : 
                    bExcont = pParlst->r_hdinfo.r_cont;
                    rc = ro1313_AssocFuns ( *pScbHd,pParlst);
		    break;

     /* ************* Execute remote operations ********************* */
     /* *************				********************* */

     case RO_INVRQ : 
     case RO_RESRQ : 
     case RO_ERRRQ : 
     case RO_RJURQ : 

                    bExcont = pParlst->r_hdinfo.r_cont;
                    rc = ro1314_Operation(*pScbHd,pParlst);
		    break;

     /* ************* Execute logging functions ********************* */
     /* *************				********************* */

     case RO_LOGON : 
     case RO_LOGOF : 
     case RO_LOGWR : 
                    bExcont = pParlst->r_hdinfo.r_cont;
                    rc = roexlog (pParlst);
		    break;

     /* ************* Execute not supported or ********************** */
     /* ************* unkown functions	       ********************** */


     default	  :
                    rc = -R_FCTCAL;
		    break;
     }

} while ((ro131a_setHeader (&(pParlst->r_hdinfo), rc) >= RO_SUCES) &&
	 (bExcont				     != FALSE));
 if(pParlst->r_hdinfo.r_fnctnb != RO_CWAIT) {
   if (pParlst->r_hdinfo.r_fnctnb != RO_LOGWR)
      ro0042_writeLog (RC0020_INTERFACE	         ,
		 RL131v_ReExit			 ,
		 sizeof (RO_excopb)		 ,
		 (char FAR *) pParlst);
 }
 else
   ro0042_writeLog (RC0023_CWAITTRACE	         ,
		 RL131v_ReExit			 ,
		 sizeof (RO_excopb)		 ,
		 (char FAR *) pParlst);

RM0003_RETURN (rs1310, signed short int, pParlst->r_hdinfo.r_retcod)
}/*end*/



/* EOF */

