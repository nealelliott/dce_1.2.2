/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif21.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:00  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:18  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/26  21:36 UTC  dtruong
 * 	Added ifdef __hpux to fix core dump
 * 	[1995/12/08  16:21:36  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:08  keutel
 * 	OT 11620
 * 	[1994/08/19  13:50:41  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:09:51  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:00  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:14  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:01:59  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:49  keutel
 * 	creation
 * 	[1994/03/21  13:30:57  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roexif21.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:00 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1993 BY SIEMENS NIXDORF INFORMATIONS AG         */
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
/* NAME         : roexif21.c                                          */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-07-26                                            */
/*                                                                    */
/* COMPONENT    : ROS external interface  IROS 2.50                   */
/*                                                                    */
/* PRD#/VERS.   : ROS-V3.00                                           */
/*                                                                    */
/* DESCRIPTION  : This modul contains the entry points to ROS-V2.5.   */
/*      and converts into the IROS 3.00 data structures               */ 
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/*  3.00 K0 | 93-07-26|  original                      | kc |         */
/*  3.00 K02| 94-07-29|  OSF change req for No AET chk | kc |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/

/* *************** System include files **************************** */
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <memory.h>      /* for memset used in this file             */

/* ***************************************************************** */


#include <ros.h>         /* ROS external interface    IROS V2.5      */

#include <ross.h>        /* ROS external interface    IROS V3.0      */

#include <rogenrl.h>     /* ROS general definitions                  */
#include <rouply0.h>     /* ROS general data structures              */
#include <roerval.h>     /* ROS error values                         */
#include <roacpz0.h>     /* ACPM infrom Data structrs                */
#include <roropz0.h>     /* ROPM inform Data structure               */
#include <roppmz0.h>     /* PPM inform Data structure               */


#include <roexif06.h>    /* This function proto types                */
#include <roexif09.h>    /* Headers for bypassing roexif00.h         */

/*********************************************************************/
/*                                                                   */
/*              M A C R O S                                          */
/*                                                                   */
/*********************************************************************/

			/* Needed, because not present in ros.h of   */
			/* ROS.D V2.5A                               */
#define R_G009_maxTransSyn           R_G002_maxAbstractSyntaxes

R_int FAR PASCAL ro1330_roexcom PROTO ((
					unsigned long  int  FAR   *pulMyScbHd,
					signed   long  int         lTaskId,
					FARPROC                    tpduFct,
					R_excopb            FAR   *r_parlst));


signed short int NEAR PASCAL  ro133o_setHeader PROTO ((
			   RO_fncthd  FAR    *pHead,
			   signed short int  sCode));

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1330_roexcom                                      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-02                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

/* Use this and remove <roexif09.h> if you DO NOT want to bypass
   roexif00.c for accessing IROS V3.0

R_int   NEAR PASCAL  ro1330_roexcom (r_parlst)
R_excopb FAR        *r_parlst;
*/

/* Use this and include <roexif09.h> if you want to bypass
   roexif00.c for accessing IROS V3.0
*/

R_int FAR PASCAL ro1330_roexcom (
unsigned long  int  FAR   *pulMyScbHd,
signed   long  int         lTaskId,
FARPROC                    tpduFct,
R_excopb            FAR   *r_parlst)


/*                                                                    */
/* DESCRIPTION  : Used as a driver between IROS v2.5 to IROS V3.00    */
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

{/*entr*/

/*********************************************************************/
/*                                                                   */
/*              L O C A L  V A R I A B L E S                         */
/*                                                                   */
/*********************************************************************/
signed short int  rc;

RO_excopb         unExcopb;      /* union of the data struct. of ross.h*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

/* ******** copy the header informations **************************** */
/* ******** check for NULL parlst *********************************** */
if(r_parlst == NULL)
   return(RO_CALERR);    

/* ******** check for VERSION 250  ********************************* */
if(r_parlst->r_hdinfo.r_versnb == R_VERSION )
    unExcopb.r_hdinfo.r_versnb = RO_VERSION;
else
    unExcopb.r_hdinfo.r_versnb = r_parlst->r_hdinfo.r_versnb;

/* -------------- Initialise extent pointer to NULL ---------------- */
unExcopb.r_hdinfo.r_extent = (void FAR * )NULL;


     switch (r_parlst->r_hdinfo.r_fnctnb)
     {
     /* ************* Execute local functions *********************** */
     /* *************                         *********************** */

     case R_ATACH : 
		    unExcopb.r_hdinfo.r_fnctnb = RO_ATACH;
		    unExcopb.r_hdinfo.r_cont = r_parlst->r_attapb.r_cont;
		    rc = ro1331_attach (pulMyScbHd, lTaskId, tpduFct,
					(R_attapb  FAR *)&r_parlst->r_attapb,
					(RO_attapb FAR *)&unExcopb.r_attapb);
		    
		    break;

     case R_DTACH : 
		    unExcopb.r_hdinfo.r_fnctnb = RO_DTACH;
		    unExcopb.r_hdinfo.r_cont = r_parlst->r_detapb.r_cont;
		    unExcopb.r_detapb.r_aplid= r_parlst->r_detapb.r_aplid;
		    rc =ro_dtach_taskId (pulMyScbHd, lTaskId, tpduFct,  &unExcopb.r_detapb);
		    break;

     case R_CWAIT :{
		    RO_evtpb  unEventpb;
		    unExcopb.r_hdinfo.r_fnctnb = RO_CWAIT;
		    unExcopb.r_hdinfo.r_cont = RO_FALSE;  
		    unExcopb.r_chwtpb.r_wtime = r_parlst->r_chwtpb.r_wtime;
		    unExcopb.r_chwtpb.r_evntrf = 
			      (RO_evtpb FAR *) &unEventpb;
		    rc = ro_cwait_taskId (pulMyScbHd, lTaskId, tpduFct, &unExcopb.r_chwtpb);                
                             /*  **skh overwritten ! Is it ok  ?  */

     /* ------------- copy the struct back to IROS V2.5 ---------------- */

		    rc = ro1337_CheckOrWait (pulMyScbHd, lTaskId, tpduFct,
			   (RO_evtpb FAR   *)&unEventpb,
			   (R_evtpb  FAR   *)r_parlst->r_chwtpb.r_evntrf);
		    if ( ( rc == R_NOEVNT ) || (rc == R_TIMOUT )  )
			  r_parlst->r_chwtpb.r_evnum =0;
		    else                                               
			  r_parlst->r_chwtpb.r_evnum =1;
		   }
		    break;
     
     case R_PRBUF :
		    unExcopb.r_hdinfo.r_fnctnb = RO_PRBUF;
		    unExcopb.r_hdinfo.r_cont = (int)r_parlst->r_pvbfpb.r_cont;
		    unExcopb.r_pvbfpb.r_aplid = 
			 (unsigned long )r_parlst->r_pvbfpb.r_aplid;
		    
		   /* -------------- copy R_buffer to RO_buffer ------------ */
		     
		    unExcopb.r_pvbfpb.r_buffer.r_mtype  = 
			   (int)r_parlst->r_pvbfpb.r_buffer.r_mtype;
		    unExcopb.r_pvbfpb.r_buffer.r_bufref  = 
			   (char FAR *)r_parlst->r_pvbfpb.r_buffer.r_bufref;
		    unExcopb.r_pvbfpb.r_buffer.r_buflen  = 
			   (unsigned long )r_parlst->r_pvbfpb.r_buffer.r_buflen;

		    /* ------ call IROS 3.00 ----------------------------- */
		    rc = ro_prbuf_taskId (pulMyScbHd, lTaskId, tpduFct, &(unExcopb.r_pvbfpb));

		    break;

     case R_REDRQ : 
		    unExcopb.r_hdinfo.r_fnctnb = RO_REDRQ;
		    rc = ro1336_redirect(pulMyScbHd, lTaskId, tpduFct,
					 (RO_rdrqpb FAR *)&(unExcopb.r_rdrqpb),
					 (R_rdrqpb FAR *)&r_parlst->r_rdrqpb);
		    break;


     case R_WAKE  : 
		    unExcopb.r_hdinfo.r_fnctnb = RO_WAKE;
	  /* --------------- copy the <cont> ----------------------- */

		    unExcopb.r_wakepb.r_hdinfo.r_cont = 
				 r_parlst->r_wakepb.r_cont;
	 /* ---------------- copy pid ------------------------------ */
		    unExcopb.r_wakepb.r_pid =
				(long)r_parlst->r_wakepb.r_pid;


	/* ----------- call IROS 3.00 function --------------------- */

		    rc =ro_wake_taskId (pulMyScbHd, lTaskId, tpduFct, & (unExcopb.r_wakepb)); 
		    break;

     case R_GTDCS : 
		    unExcopb.r_hdinfo.r_fnctnb = RO_INFRM;
		    unExcopb.r_inform.r_inftyp = RO_GTDCS;
		     
		    rc     = ro1335_Inform (pulMyScbHd, lTaskId, tpduFct,
					    r_parlst,
					    (RO_excopb FAR *)&unExcopb);
		    break;
     case R_GTPBF:
		    unExcopb.r_hdinfo.r_fnctnb = RO_INFRM;
		    unExcopb.r_inform.r_inftyp = RO_GTPBF;
		     
		    rc     = ro1335_Inform (pulMyScbHd, lTaskId, tpduFct,
					    r_parlst,
					    (RO_excopb FAR *)&unExcopb);
		    break;
		   
     /* ************* Execute binding, unbinding and abort ********** */
     /* *************                                      ********** */

     case R_BINRQ : 
     case R_BINRS : 
     case R_UBNRQ : 
     case R_UBNRS : 
     case R_ABORQ : 
		   rc = ro1332_AssocFuns (pulMyScbHd, lTaskId, tpduFct,
					  (R_excopb  FAR *)r_parlst,
					  (RO_excopb FAR *)&unExcopb);
		   break;
     /* ************* Execute remote operations ********************* */
     /* *************                           ********************* */
     case R_INVRQ : 
     case R_RESRQ : 
     case R_ERRRQ : 
     case R_RJURQ : 
		    rc = ro1333_Operations(pulMyScbHd, lTaskId, tpduFct,
					   (R_excopb  FAR *)r_parlst,
					   (RO_excopb FAR *)&unExcopb);
		    break;

     /* ************* Execute logging functions ********************* */
     /* *************                           ********************* */

     case R_LOGON : 
     case R_LOGOF : 
     case R_LOGWR : 
		    rc = ro1334_Logging(pulMyScbHd, lTaskId, tpduFct,
					(RO_excopb FAR *)&unExcopb,
					r_parlst);
		    break;

     /* ************* Execute not supported or ********************** */
     /* ************* unkown functions         ********************** */

     case R_FRASD :
     case R_FRARV :
     case R_BININ :
     case R_BINCN :
     case R_UBNIN :
     case R_UBNCN :
     case R_INVIN :
     case R_RESIN :
     case R_ERRIN :
     case R_RJPIN :
     
     default:
	     unExcopb.r_hdinfo.r_errval = R_FCTCAL;
	     unExcopb.r_hdinfo.r_erclas = RO_ILVERS;
	     unExcopb.r_hdinfo.r_retcod = RO_CALERR;
 
	     break;
     }
   r_parlst->r_hdinfo.r_errval = unExcopb.r_hdinfo.r_errval ;
   r_parlst->r_hdinfo.r_erclas = unExcopb.r_hdinfo.r_erclas ;
   r_parlst->r_hdinfo.r_retcod = unExcopb.r_hdinfo.r_retcod ;


return (r_parlst->r_hdinfo.r_retcod);

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133a_Objcpy                                       */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-03                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static    signed short int NEAR PASCAL ro133a_Objcpy (
		unsigned long int FAR  *pNObjId, 
		R_unsign          FAR  *pOObjId, 
		R_unsign                Length)
/*                                                                    */   
/* DESCRIPTION  :   This function copies an object identifier value   */   
/*                  and checks whether it is longer than allowed.     */   
/*                  It returns SUCES if no error ocuurs, a value      */   
/*                  less than SUCES if an error occurs or a value     */   
/*                  grather  than SUCES if the length of the          */   
/*                  source object identifier value is equal to NULL.  */   
/*                                                                    */   
/* IN-PARAMETERS              :                                       */   
/*                                                                    */   
/*     pTarget  : object identifier target array.                     */   
/*                                                                    */   
/*     pSource  : object identifier source array.                     */   
/*                                                                    */   
/*    ulLength  : length of the source object identifier value        */   
/*                                                                    */   
/* OUT-PARAMETERS             : none                                  */   
/*                                                                    */   
/* RETURN VALUES :                                                    */   
/*                                                                    */   
/*      = 0  SUCES     successfully                                   */   
/*      < 0 (SUCES)    error                                          */   
/*      > 0 (SUCES)    not defined (length field equal to NULL)       */   
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
unsigned long int ulLen;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
    ulLen = (unsigned long)Length;
    if (ulLen ==  0) {
	*pNObjId     =  0;
	return (R_LENULL);
    } else {
	if ((ulLen > RCro00_MaxObjIdComp) || (ulLen < 3 )){
	     *pNObjId     =  0;         
	 return (-R_MAXOBJ);
	}
	else {
	 *pNObjId++   = ulLen;
	 while ((ulLen--) >  0)  *pNObjId++ = *pOObjId++;
	}
    return (0);
    }
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1331_attach                                       */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed short int NEAR PASCAL ro1331_attach (

unsigned long  int  FAR   *pulMyScbHd,
signed   long  int         lTaskId,
FARPROC                    tpduFct,
R_attapb            FAR   *pOldAttapb,
RO_attapb           FAR   *pNewAttapb)

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
unsigned      int uCount;
unsigned      int i,j;
signed        int rc;




/* ***************** IROS 3.00 attach parameters ******************* */
rTro00_vObjectId       vApplProcessTitle;

rTro00_vObjectId       vApplCxtName[ R_G003_maxAppliContexts];
rTro00_vObjectId       vAbsSynName[R_G003_maxAppliContexts *   
				    R_G002_maxAbstractSyntaxes]; 
rTro00_vObjectId       vTrsSynName[R_G003_maxAppliContexts *    
				    R_G002_maxAbstractSyntaxes * 
				    R_G009_maxTransSyn];         


RO_apcary              vApplCxtAry[R_G003_maxAppliContexts];
RO_absary              vAbsSynAry [R_G003_maxAppliContexts *
				  R_G002_maxAbstractSyntaxes]; 
RO_trsary              vTrsSynAry [R_G003_maxAppliContexts *
				   R_G002_maxAbstractSyntaxes *
				   R_G009_maxTransSyn];

int           vTrsIdAry   [R_G002_maxAbstractSyntaxes]
				   [RCro03_MaxTransPerAbstr];

int           vAbsIdAry  [R_G003_maxAppliContexts]
				  [R_G002_maxAbstractSyntaxes];


RO_aetfm2              stAeTitl2;
RO_apcary       FAR  * pstApplCxt;
RO_absary       FAR  * pstAbsSyn;
RO_trsary       FAR  * pstTrsSyn;

R_apcary        FAR  * pstOldApplCxt;
R_absary        FAR  * pstOldAbsSyn;
R_trsary        FAR  * pstOldTrsSyn;

				  
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

/* ************ Initialise all the arrays to 0 ********************** */
ro133b_memset((char FAR *)vApplProcessTitle,
	      (int)0,
	      sizeof(rTro00_vObjectId));
ro133b_memset((char FAR *)vApplCxtName,
	      (int)0,
	      sizeof(rTro00_vObjectId)*R_G003_maxAppliContexts);
ro133b_memset((char FAR *)vAbsSynName,
	      (int)0,
	      sizeof(rTro00_vObjectId)*R_G003_maxAppliContexts *
				       R_G002_maxAbstractSyntaxes);
 
ro133b_memset((char FAR *)vTrsSynName,
	      (int)0,
	      sizeof(rTro00_vObjectId)*R_G003_maxAppliContexts *     
				       R_G002_maxAbstractSyntaxes * 
				       R_G009_maxTransSyn);         

ro133b_memset((char FAR *)vApplCxtAry,
	      (int)0,
	      sizeof(RO_apcary)*R_G003_maxAppliContexts);
ro133b_memset((char FAR *)vAbsSynAry,
	       (int)0,
	       sizeof(RO_absary)*R_G003_maxAppliContexts *
				       R_G002_maxAbstractSyntaxes);

ro133b_memset((char FAR *)vTrsSynAry,
	     (int)0, 
	     sizeof(RO_trsary)*R_G003_maxAppliContexts *   
					  R_G002_maxAbstractSyntaxes *
						    R_G009_maxTransSyn);        

ro133b_memset((char FAR *)vTrsIdAry,
	      (int)0,
	      sizeof(int)*R_G002_maxAbstractSyntaxes *     
				       RCro03_MaxTransPerAbstr);

ro133b_memset((char FAR *)vAbsIdAry,
	      (int)0,
	      sizeof(int) *R_G003_maxAppliContexts  *            
					R_G002_maxAbstractSyntaxes);

				    


/* ************ copy the RO_pradd from R_pradd ********************** */

/* ********* copy the Presentation address refernce into new struct * */
/* ********* Only the pointer to the memory area which contains the * */
/* ********* Pres. addr. is copied.                             ***** */

/* -------- which one to Use 1.R_attapb.r_pradd      ---------------- */
/* --------------------  *   2.R_attapb.r_aetitl...  ---------------- */

ro133c_PresAddrCpy( (RO_pradd FAR *)&(pNewAttapb->r_pradd),
		    (R_pradd  FAR *)&(pOldAttapb->r_aplref));

/* *********** copy over for Pres. addr. **************************** */

/* *********** The  AE title  consists of AP title & AE qualifier *** */
/* ----- These are optional if no Id then pointer points to NULL ***  */
/* ------ which one to use 1.R_attapb.r_aetitl (not supported v2.5)   */
/*                       * 2.R_attapb.r_aplref.r_aetitl(R_apname)     */

 rc =ro133a_Objcpy ((unsigned long int FAR *)vApplProcessTitle             ,
		    (R_unsign FAR *)pOldAttapb->r_aplref.r_aetitl.r_obidrf ,
		     pOldAttapb->r_aplref.r_aetitl.r_obidnb                );
  
/* ************ Copy the Application Context List ******************* */

/*  commented for testing form1 ************************* */
/* ??? testing */
 pNewAttapb->r_aetitl.r_aptype = (RO_AETF2 | RO_AETNC) ; 
    /* by kc 29-07-94 for OSF changed from Checking to No checking */
                                  /* In 2.5 always form2 */
 pNewAttapb->r_aetitl.r_aetval = (RO_aetfm2 FAR *)&stAeTitl2;
 if (rc == 0)
     stAeTitl2.r_aptitl = vApplProcessTitle;
else if (rc > 0)
      stAeTitl2.r_aptitl = (unsigned long int FAR *)NULL;   
else return(rc);                                                  
/******************************** end of comment */
/* --------- AE qualifier is OPTIONAL so NULL ----------------------- */
     stAeTitl2.r_aequal = (long FAR *)NULL; 

/* ******************************************************************* */
/* ************ for the testing of Form1 ***************************** */
/*
{
 RO_avastr   stAVA1;
 RO_rdname   stRDN;
 RO_aetfm1   stAETitl1;
 char        vstr[10] ="CHAN";

 pNewAttapb->r_aetitl.r_aptype = RO_AETF1;
 pNewAttapb->r_aetitl.r_aetval = (RO_aetfm1 FAR *)&stAETitl1;
 stAVA1.r_avatyp = (unsigned long FAR *) vApplProcessTitle;
 stAVA1.r_avanum = 4;
 stAVA1.r_avaval = (char FAR *) vstr;

 stRDN.r_rdnnum =1;
 stRDN.r_rdnval = (RO_avastr FAR *) &stAVA1;

 stAETitl1.r_aptnum=stAETitl1.r_aeqnum=1;
 stAETitl1.r_aptnam=(RO_rdname FAR *)&stRDN;
 stAETitl1.r_aeqval=(RO_avastr FAR *)&stAVA1; 

} */
/* *********************** end of testing *************************** */
/* ******** Invocation Ids **************** */
pNewAttapb->r_invcid.r_apivid = (long F *)NULL;
pNewAttapb->r_invcid.r_aeivid = (long F *)NULL;

pNewAttapb->r_apclst.r_apcnum = pOldAttapb->r_apclst.r_apcnum;
pNewAttapb->r_apclst.r_absnum = pOldAttapb->r_apclst.r_absnum;
pNewAttapb->r_apclst.r_trsnum = pOldAttapb->r_apclst.r_trsnum;

/* ------- assign the pointers of the arrays of appl. cxt array ---- */
/* --------Abstract Syn. array & Trans Syn array               ---- */
 
pNewAttapb->r_apclst.r_apcary = (RO_apcary FAR *)vApplCxtAry;
pNewAttapb->r_apclst.r_absary = (RO_absary FAR *)vAbsSynAry;
pNewAttapb->r_apclst.r_trsary = (RO_trsary FAR *)vTrsSynAry;

uCount = MIN((R_unsign)pOldAttapb->r_apclst.r_trsnum,             
	      (R_unsign)(R_G003_maxAppliContexts *                 
	       R_G002_maxAbstractSyntaxes *              
	       R_G009_maxTransSyn          ) );
 /* !!!holes  Fill the NewAttapb as per the TransID in the OldAttapb */
 
for (i=0; i < uCount; i++){

    pstOldTrsSyn = (pOldAttapb->r_apclst.r_trsary+i);
    /*--- the placement will be as per the ID ----*/
 
    pstTrsSyn = ( (pNewAttapb->r_apclst.r_trsary )+ 
		    ( pstOldTrsSyn->r_trsid)         );   
    rc=ro133a_Objcpy(
       (unsigned long int FAR *)(vTrsSynName + i)                             ,
       (R_unsign FAR *)(pOldAttapb->r_apclst.r_objary+(pstOldTrsSyn->r_objinx)),
	pstOldTrsSyn->r_objnum                                                );
    if(rc == 0)pstTrsSyn->r_trsnam = (unsigned long FAR *) (vTrsSynName+i);
    /* kc on 30-8-93 */
    else if(rc > 0)                                      
       pstTrsSyn->r_trsnam = (unsigned long FAR *) NULL; 
    else return (rc);                                    
}

uCount = MIN ( (R_unsign)pOldAttapb->r_apclst.r_absnum ,
		(R_unsign)(R_G003_maxAppliContexts *    
		 R_G002_maxAbstractSyntaxes)  );
 
for (i=0; i < uCount; i++){
    pstOldAbsSyn = (pOldAttapb->r_apclst.r_absary+i);

    pstAbsSyn =((pNewAttapb->r_apclst.r_absary) +
		 (pstOldAbsSyn->r_absid));   
    rc = ro133a_Objcpy(
       (unsigned  long int FAR *) (vAbsSynName+i)                             ,
       (R_unsign FAR *)(pOldAttapb->r_apclst.r_objary+(pstOldAbsSyn->r_objinx)),
	pstOldAbsSyn->r_objnum                                                );
    if (rc==0)pstAbsSyn->r_absnam = (unsigned long FAR *) (vAbsSynName+i);
   /* kc on 30-8-93 */
    else if (rc > 0)                                       
         pstAbsSyn->r_absnam = (unsigned long FAR *)NULL;     
    else return(rc);                                       

    pstAbsSyn->r_idtnum = (unsigned int)pstOldAbsSyn->r_idtnum;

    /* --------- assign the idtary vector --------------- */
    pstAbsSyn->r_idtary =(int FAR *) &(vTrsIdAry[i][0]) ;
    for ( j=0; j < pstAbsSyn->r_idtnum; j++)
	*(pstAbsSyn->r_idtary+j) = 
	 *( (pOldAttapb->r_apclst.r_idtary)+(pstOldAbsSyn->r_idtinx)+j);

}

uCount = MIN(pOldAttapb->r_apclst.r_apcnum,R_G003_maxAppliContexts);

for (i=0;i < uCount; i++){ /* loop 1 for Appl. Context */
    pstOldApplCxt = (pOldAttapb->r_apclst.r_apcary+i);
    pstApplCxt = ((pNewAttapb->r_apclst.r_apcary)+i);
  /* ---- No Application Context ID ---*/

    /* ------- Appl. Name is copied ------------------------ */

    rc =ro133a_Objcpy(
      (unsigned long int FAR *)(vApplCxtName+i)                                ,
      (R_unsign FAR *)(pOldAttapb->r_apclst.r_objary+(pstOldApplCxt->r_apcinx)),
       pstOldApplCxt->r_objnum                                                );

    if(rc==0)pstApplCxt->r_apcnam = (unsigned long FAR *) (vApplCxtName+i);
 /* kc on 30-8-93 */
    else if (rc > 0)                                        
       pstApplCxt->r_apcnam = (unsigned long FAR *) NULL;   
    else return(rc);                                        
    /* ------------ pmode  ----------------------------- */
    /* copy from r_pfunit */
    if ( pstOldApplCxt->r_pfunit ==  R_PKERN )
	pstApplCxt->r_pmode = RO_PNORM; /* now it is set to normal */
    else if (pstOldApplCxt->r_pfunit ==  R_PSNIP )
	pstApplCxt->r_pmode = RO_PSNIP; 
    else 
	pstApplCxt->r_pmode = -1; /* It will give error in exif */
    /* -------------- asecif -------------------------------- */

    pstApplCxt->r_asecif = (unsigned int)pstOldApplCxt ->r_asecif;       

    /* -------------- idsnum -------------------------------- */
    pstApplCxt->r_idsnum = (unsigned int)pstOldApplCxt ->r_idsnum; 

    /* --------------Abstract Syn  Id array ----------------- */
    pstApplCxt->r_idsary = (int FAR *) &(vAbsIdAry[i][0]);

    for (j=0; j < pstApplCxt->r_idsnum; j++)
	*(pstApplCxt->r_idsary +j) = 
	 *( (pOldAttapb->r_apclst.r_idsary)+(pstOldApplCxt ->r_idsinx)+j);
}
		    
    
  
/* ***** Call the IROS 3.00 attach call *************************** */
if (!rc) 
     rc  = ro_atach_taskId (pulMyScbHd, lTaskId, tpduFct,  pNewAttapb );
    
/* ********** Assign back to IROS 2.5 struct from return values ** */
    if(pNewAttapb->r_hdinfo.r_retcod == RO_NOT1ST  )
	pNewAttapb->r_hdinfo.r_retcod = R_NOT1ST;
     pOldAttapb->r_aplid = (R_idtype)pNewAttapb->r_aplid;  


     return (rc );

}/*end*/                                                                        



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro133o_setHeader				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int NEAR PASCAL  ro133o_setHeader (
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

return(  pHead->r_retcod);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1332_AssocFuns                                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-06                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed short int NEAR PASCAL ro1332_AssocFuns(
unsigned long  int  FAR   *pulMyScbHd,
signed   long  int         lTaskId,
FARPROC                    tpduFct,
R_excopb            FAR   *pOExcopb,
RO_excopb           FAR   *pNExcopb)


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
signed   short int  rc;
RO_aetfm2              stAeTitl2;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

switch(pOExcopb->r_hdinfo.r_fnctnb){
    case R_BINRQ:
		{
		R_bnrqpb    FAR *pOBindReqPb;
		RO_bnrqpb   FAR *pNBindReqPb;

	  /* --- local variables only for BINDreq ---------------- */

		rTro00_vObjectId       vApplProcessTitle;
		
		rT06z0_stInfrmApplCxtName stApplCxt;

	  /* ---- Initialising the variables --------------------- */
	
		ro133b_memset((char FAR *)vApplProcessTitle,
			      (int)0,
			       sizeof(rTro00_vObjectId));

	  /* --------- copying the data struct starts --------------- */

		pOBindReqPb = (R_bnrqpb  FAR *)& (pOExcopb->r_bnrqpb);
		pNBindReqPb = (RO_bnrqpb FAR *)& (pNExcopb->r_bnrqpb);
	    
                if (pOBindReqPb->r_otype != R_ACKASN ) 
                  return( ro133o_setHeader(&pNBindReqPb->r_hdinfo,-R_HOTYPE));
        
            
		pNBindReqPb->r_hdinfo.r_fnctnb = RO_BINRQ;
		pNBindReqPb->r_hdinfo.r_cont = pOBindReqPb->r_cont;
		pNBindReqPb->r_aplid  = (unsigned long)pOBindReqPb->r_aplid;
		pNBindReqPb->r_aclass = (int)pOBindReqPb->r_aclass;
	       
	 /* ---------- copy apcon  of Responder -------------------- */
	 /* !!! problem Objid to int  Use inform ---*/
		rc = ro133a_Objcpy (
			(unsigned long int FAR *)stApplCxt.vApplCxtName, 
			(R_unsign          FAR *)pOBindReqPb->r_apcon.r_obidrf,
						 pOBindReqPb->r_apcon.r_obidnb);
		stApplCxt.ulApplID = pOBindReqPb->r_aplid; 
		stApplCxt.ulAssId = 0;
		rc = ro1338_GetApplContext (pulMyScbHd, lTaskId, tpduFct,
				   (rT06z0_stInfrmApplCxtName FAR *)&stApplCxt,
				    RC06z2_ApplCxtID );
	     
		if (rc == RO_SUCES)
		      pNBindReqPb->r_apcon = 
				 stApplCxt.sApplCxtId;
		else 
		      pNBindReqPb->r_apcon = -1; /* !!! invalid index */

	 /* !!! what to do if failed */

	 /* ---------------- copy aetitl of Responder --------------- */
		rc = ro133a_Objcpy(
			(unsigned long int FAR *)vApplProcessTitle,
			(R_unsign FAR *)pOBindReqPb->r_ptnref.r_aetitl.r_obidrf,
			 pOBindReqPb->r_ptnref.r_aetitl.r_obidnb);
/* *************************************************** */
          pNBindReqPb->r_aetitl.r_aptype = RO_AETF2; /* In 2.5 always form2 */
          pNBindReqPb->r_aetitl.r_aetval = (RO_aetfm2 FAR *)&stAeTitl2;
		if (rc == 0)
                  stAeTitl2.r_aptitl = vApplProcessTitle; 
               else if( rc > 0)                                               
                  stAeTitl2.r_aptitl = (unsigned long FAR *)NULL; 
               else return (rc);                                            
/*********************************************************** */  
	 /* --------- AE qualifier is OPTIONAL so NULL -------------- */
    
       	stAeTitl2.r_aequal = (long FAR *)NULL; 

/* ************ for the testing of Form1 ***************************** */
/*
{
 RO_avastr   stAVA1;
 RO_rdname   stRDN;
 RO_aetfm1   stAETitl1;
 char        vstr[10] ="CHAN";

 pNBindReqPb->r_aetitl.r_aptype = RO_AETF1;
 pNBindReqPb->r_aetitl.r_aetval = (RO_aetfm1 FAR *)&stAETitl1;
 stAVA1.r_avatyp = (unsigned long FAR *) vApplProcessTitle;
 stAVA1.r_avanum = 4;
 stAVA1.r_avaval = (char FAR *) vstr;

 stRDN.r_rdnnum =1;
 stRDN.r_rdnval = (RO_avastr FAR *) &stAVA1;

 stAETitl1.r_aptnum=stAETitl1.r_aeqnum=1;
 stAETitl1.r_aptnam=(RO_rdname FAR *)&stRDN;
 stAETitl1.r_aeqval=(RO_avastr FAR *)&stAVA1; 

}
*/
/* *********************** end of testing *************************** */
         pNBindReqPb->r_invcid.r_apivid = (long F *)NULL;
         pNBindReqPb->r_invcid.r_aeivid = (long F *)NULL;
         
	 /* --------- Pres Addr of the Responder -------------------- */
		ro133c_PresAddrCpy ((RO_pradd FAR *)&(pNBindReqPb->r_pradd),
				    (R_pradd  FAR *)&(pOBindReqPb->r_ptnref));

	 /* ------- copy Send Data struct ------------------------- */
		if((rc=ro133d_SDataCpy (
                                (RO_snfrag FAR *)&(pNBindReqPb->r_sndfra),
				(R_sndata  FAR *)&(pOBindReqPb->r_sndata))))
                return( ro133o_setHeader(&pNBindReqPb->r_hdinfo,rc));

	 /* ------- copy Buffer from Evdata of Bing req ---------- */
		  ro133e_EvDataCpy ((RO_buffer FAR *)&(pNBindReqPb->r_buffer),
				    (R_evdata  FAR *)&(pOBindReqPb->r_evdata));

	   /* ***** Call the IROS 3.00 call ********************** */

		  rc = ro_binrq_taskId (pulMyScbHd, lTaskId, tpduFct, pNBindReqPb);
	   /* --------- copy the returned values ------------------ */
 
		  pOBindReqPb->r_assid = 
			 (R_idtype)pNBindReqPb->r_assid;
		  
	       }
	       break;
    case R_BINRS:
	       {
		R_bnrspb    FAR *pOBindRspPb;
		RO_bnrspb   FAR *pNBindRspPb;
	  /* --- local variables only for BINDreq ---------------- */

		rTro00_vObjectId       vApplProcessTitle;

		
		rT06z0_stInfrmApplCxtName stApplCxt;
	  /* ---- Initialising the variables --------------------- */
	
		ro133b_memset((char FAR *)vApplProcessTitle,(int)0,
			      sizeof(rTro00_vObjectId));
	  /* --------- copying the data struct starts --------------- */

		pOBindRspPb = (R_bnrspb  FAR *)& (pOExcopb->r_bnrspb);
		pNBindRspPb = (RO_bnrspb FAR *)& (pNExcopb->r_bnrspb);

                if (pOBindRspPb->r_otype != R_ACKASN ) 
                  return( ro133o_setHeader(&pNBindRspPb->r_hdinfo,-R_HOTYPE));
		pNBindRspPb->r_hdinfo.r_fnctnb = RO_BINRS;
	  /* ---------------copy cont flag --------------------------- */

		pNBindRspPb->r_hdinfo.r_cont = pOBindRspPb->r_cont;
	  /* --------- copy assoid ------------------------------ */

		pNBindRspPb->r_assid  =
		  (unsigned long)pOBindRspPb->r_assid;
	  /* ------------- copy the Assoc Class ------------------- */
	  /* ------no Aclass in IROS 2.5  so use Default 3 -------- */
		pNBindRspPb->r_aclass = RO_ACL3; 
	  /* --------- copy Result ------------------------------ */

		pNBindRspPb->r_result  = 0x00000710UL; /* reject */
		switch(pOBindRspPb->r_result &  0x0000ffff){
		   case R_ACCEPT:
		       pNBindRspPb->r_result  = 0x00000700UL; /* accept */
		       break;
		   case R_UREJCT:
		       break;
		   case R_NOREAS:
		       pNBindRspPb->r_result  |= RO_NoReason;
		       break;
		   case R_APCNAM:
		       pNBindRspPb->r_result  |= RO_UnsupApplCtxt; 
		       break;
		   case R_CAGAPT:
		       pNBindRspPb->r_result  |= RO_UnrecClgApTitle; 
		       break;
		   case R_CADART:
		       pNBindRspPb->r_result  |= RO_UnrecCldApTitle; 
		       break;
                   default:
		       pNBindRspPb->r_result  |= (RO_UnrecCldAeInvocId+1); 
		       break;
		}

		 
		
	 /* ---------- copy apcon   -------------------------------- */
	 /* !!! problem Objid to int ---*/
		rc = ro133a_Objcpy(
			(unsigned long int FAR *)stApplCxt.vApplCxtName, 
			(R_unsign          FAR *)pOBindRspPb->r_apcon.r_obidrf,
						 pOBindRspPb->r_apcon.r_obidnb);
		
		stApplCxt.ulApplID = 0;
		stApplCxt.ulAssId  = (unsigned long int)pOBindRspPb->r_assid;
		rc = ro1338_GetApplContext (pulMyScbHd, lTaskId, tpduFct,
			     (rT06z0_stInfrmApplCxtName  FAR  *)&stApplCxt,
			      RC06z2_ApplCxtID);
	     
		if(rc >= RO_SUCES)
		      pNBindRspPb->r_apcon = 
				 stApplCxt.sApplCxtId;

		else 
		      pNBindRspPb->r_apcon = -1; /* !!! invalid index */
	 /* !!! what to do if failed */


       pNBindRspPb->r_aetitl.r_aptype = RO_AETF2; /* In 2.5 always form2 */
       pNBindRspPb->r_aetitl.r_aetval = (RO_aetfm2 FAR *)&stAeTitl2;
	 /* ---------------- copy aetitl of Responding -------------- */
		rc = ro133a_Objcpy(
		       (unsigned long int FAR *)vApplProcessTitle,
		       (R_unsign FAR *)         pOBindRspPb->r_aetitl.r_obidrf,
						pOBindRspPb->r_aetitl.r_obidnb);
      /* kc on 30-8-93 */
		if (rc == 0)
                  stAeTitl2.r_aptitl = vApplProcessTitle;
		else if ( rc > 0 )
		  stAeTitl2.r_aptitl = (unsigned long FAR *)NULL;
                else return(rc);
	 /* --------- AE qualifier is OPTIONAL so NULL -------------- */
		stAeTitl2.r_aequal = (long FAR *)NULL;

         pNBindRspPb->r_invcid.r_apivid = (long F *)NULL;
         pNBindRspPb->r_invcid.r_aeivid = (long F *)NULL;
	 /* --------- Pres Addr of the Responding ------------------- */
		ro133c_PresAddrCpy ((RO_pradd FAR *)&(pNBindRspPb->r_pradd),
				    (R_pradd  FAR *)&(pOBindRspPb->r_pradd));

	 /* ---------- copy R_sndata to RO_sndfrg ------------------ */
	       if((rc=ro133d_SDataCpy(
                               (RO_snfrag FAR *)&(pNBindRspPb->r_sndfra),
			       (R_sndata  FAR *)&(pOBindRspPb->r_sndata))))
                return( ro133o_setHeader(&pNBindRspPb->r_hdinfo,rc));

	   /* ***** Call the IROS 3.00 call ********************** */
	      
	       rc = ro_binrs_taskId (pulMyScbHd, lTaskId, tpduFct, pNBindRspPb);

	   /* --------- copy the returned values ------------------ */

	       }
	       break;
    case R_UBNRQ:
	     {
		R_ubrqpb    FAR *pOUnBndReqPb;
		RO_ubrqpb   FAR *pNUnBndReqPb;

	  /* --------- copying the data struct starts --------------- */

		pOUnBndReqPb = (R_ubrqpb  FAR *)& (pOExcopb->r_ubrqpb);
		pNUnBndReqPb = (RO_ubrqpb FAR *)& (pNExcopb->r_ubrqpb);

                if (pOUnBndReqPb->r_otype != R_ACKASN ) 
                  return( ro133o_setHeader(&pNUnBndReqPb->r_hdinfo,-R_HOTYPE));
		pNUnBndReqPb->r_hdinfo.r_fnctnb =RO_UBNRQ;

	  /* ---------------copy cont flag --------------------------- */
		pNUnBndReqPb->r_hdinfo.r_cont = pOUnBndReqPb->r_cont;

	  /* --------- copy assoid ------------------------------ */
		pNUnBndReqPb->r_assid  =
		  (unsigned long)pOUnBndReqPb->r_assid;

	  /* ------------ copy R_sndata to RO_sndfrg ----------- */
	       if((rc=ro133d_SDataCpy (
                                (RO_snfrag FAR *)&(pNUnBndReqPb->r_sndfra),
				(R_sndata  FAR *)&(pOUnBndReqPb->r_sndata))))
                return( ro133o_setHeader(&pNUnBndReqPb->r_hdinfo,rc));

	  /* ------------- copy R_evdata to RO_buffer ---------- */
	       ro133e_EvDataCpy ((RO_buffer FAR *)&(pNUnBndReqPb->r_buffer),
				 (R_evdata  FAR *)&(pOUnBndReqPb->r_evdata));

	   /* ***** Call the IROS 3.00 call ********************** */
	      
	       rc = ro_ubnrq_taskId (pulMyScbHd, lTaskId, tpduFct, pNUnBndReqPb);

	   /* --------- copy the returned values ------------------ */

	     }
	     break;
    case R_UBNRS:
	     {
		R_ubrspb    FAR *pOUnBndRspPb;
		RO_ubrspb   FAR *pNUnBndRspPb;

	  /* --------- copying the data struct starts --------------- */

		pOUnBndRspPb = (R_ubrspb  FAR *)& (pOExcopb->r_ubrspb);
		pNUnBndRspPb = (RO_ubrspb FAR *)& (pNExcopb->r_ubrspb);

                if (pOUnBndRspPb->r_otype != R_ACKASN ) 
                  return( ro133o_setHeader(&pNUnBndRspPb->r_hdinfo,-R_HOTYPE));
		pNUnBndRspPb->r_hdinfo.r_fnctnb = RO_UBNRS;
	  /* ---------------copy cont flag --------------------------- */
		pNUnBndRspPb->r_hdinfo.r_cont = pOUnBndRspPb->r_cont;
	  /* --------- copy assoid ------------------------------ */

		pNUnBndRspPb->r_assid  =
		  (unsigned long)pOUnBndRspPb->r_assid;

	  /* ----------- ureason ---------------------------------------*/
                 if(pOUnBndRspPb->r_fail == R_TRUE)       
                  pNUnBndRspPb->r_ureasn = RO_NotFinished;
                 else                                     
                  pNUnBndRspPb->r_ureasn = RO_NotDefined; 
	   /*  Since not in IROS V2.5 --------------- */ 

	  /* ----------- copy R_sndata to RO_sndfrg -------------- */

	       if((rc=ro133d_SDataCpy (
                              (RO_snfrag FAR *)&(pNUnBndRspPb->r_sndfra),
				(R_sndata  FAR *)&(pOUnBndRspPb->r_sndata))))
                return( ro133o_setHeader(&pNUnBndRspPb->r_hdinfo,rc));


	   /* ***** Call the IROS 3.00 call ********************** */
	      
	       rc = ro_ubnrs_taskId (pulMyScbHd, lTaskId, tpduFct, pNUnBndRspPb);

	   /* --------- copy the returned values ------------------ */

	     }
	     break;
    case R_ABORQ:
	     {
		R_abrqpb    FAR *pOAbrReqPb;
		RO_abrqpb   FAR *pNAbrReqPb;

	  /* --------- copying the data struct starts --------------- */

		pOAbrReqPb = (R_abrqpb  FAR *)& (pOExcopb->r_abrqpb);
		pNAbrReqPb = (RO_abrqpb FAR *)& (pNExcopb->r_abrqpb);

                if (pOAbrReqPb->r_otype != R_ACKASN ) 
                  return( ro133o_setHeader(&pNAbrReqPb->r_hdinfo,-R_HOTYPE));
		pNAbrReqPb->r_hdinfo.r_fnctnb = RO_ABORQ;
	  /* ---------------copy cont flag --------------------------- */
		pNAbrReqPb->r_hdinfo.r_cont = pOAbrReqPb->r_cont;
	  /* --------- copy assoid ------------------------------ */

		pNAbrReqPb->r_assid  =
		  (unsigned long)pOAbrReqPb->r_assid;
	  /* ----------- copy R_sndat to RO_sndfra --------------- */

	       if((rc=ro133d_SDataCpy ((RO_snfrag FAR *)&(pNAbrReqPb->r_sndfra),
				(R_sndata  FAR *)&(pOAbrReqPb->r_sndata))))
                return( ro133o_setHeader(&pNAbrReqPb->r_hdinfo,rc));
                
                

	   /* ***** Call the IROS 3.00 call ********************** */
	      
	       rc = ro_aborq_taskId (pulMyScbHd, lTaskId, tpduFct, pNAbrReqPb);

	   /* --------- copy the returned values ------------------ */
	     }
	     break; 

	 } /* end of switch */
	 return (rc);

}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1333_Operations                                   */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-08                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed short int NEAR PASCAL ro1333_Operations (
unsigned long  int  FAR   *pulMyScbHd,
signed   long  int         lTaskId,
FARPROC                    tpduFct,
R_excopb            FAR   *pOExcopb,
RO_excopb           FAR   *pNExcopb)

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
signed   short int  rc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

switch(pOExcopb->r_hdinfo.r_fnctnb){
    case R_INVRQ:
		{
		R_ivrqpb    FAR *pOInvoReqPb;
		RO_ivrqpb   FAR *pNInvoReqPb;
	  /* ------- data types required for Operation Invoke ------- */
		rTro00_vObjectId       vOpGlobal;
		long                   lOpLocal=0;
		long                   lLinkId=0;

	  /* ---- Initialising the variables --------------------- */
	
		ro133b_memset((char FAR *) vOpGlobal,(int)0,
			       sizeof(rTro00_vObjectId));

	  /* --------- copying the data struct starts --------------- */

		pOInvoReqPb = (R_ivrqpb  FAR *)& (pOExcopb->r_ivrqpb);
		pNInvoReqPb = (RO_ivrqpb FAR *)& (pNExcopb->r_ivrqpb);

                if (pOInvoReqPb->r_otype != R_ACKASN ) 
                  return( ro133o_setHeader(&pNInvoReqPb->r_hdinfo,-R_HOTYPE));
		pNInvoReqPb->r_hdinfo.r_fnctnb = RO_INVRQ;
	  /* ----------- copy the <cont> flag ----------------------- */
		pNInvoReqPb->r_hdinfo.r_cont = pOInvoReqPb->r_cont;
	  /* ------------ copy assid  ------------------------------- */
		pNInvoReqPb->r_assid = (unsigned long) pOInvoReqPb->r_assid;
	  /* ------------ copy invoke Id----------------------------- */
		pNInvoReqPb->r_invid = (long) pOInvoReqPb->r_invid;
	  /* ------------ copy link id------------------------------- */
	  /* --------- copy only if it is present ------------------- */
		if (pOInvoReqPb->r_lnkid){
		   lLinkId =(long) *(pOInvoReqPb->r_lnkid);
		   pNInvoReqPb->r_lnkid = (signed long int FAR *) &lLinkId;
		}
		else pNInvoReqPb->r_lnkid =(long *)NULL;
	 

	  /* ------------ copy oclass ------------------------------- */
		pNInvoReqPb->r_oclass = pOInvoReqPb->r_oclass;

	  /* -------------- copy Operation Value -------------------- */
		pNInvoReqPb->r_opval.r_oploc= (signed long int FAR *)&lOpLocal;
		pNInvoReqPb->r_opval.r_opglb=(unsigned long int FAR *)vOpGlobal;
		ro133f_OpValuCpy ((RO_opval FAR *)&(pNInvoReqPb->r_opval),
				  (R_opval  FAR *)&(pOInvoReqPb->r_opval));

	  /* ------------ copy AbsId from Old sndata----------------- */
		if ( (pOInvoReqPb->r_sndata.r_fralen ) &&
		     (pOInvoReqPb->r_sndata.r_fraref) )
		 pNInvoReqPb->r_absid = (int)pOInvoReqPb->r_sndata.r_absid;
		else 
		 pNInvoReqPb->r_absid = ro133n_getROSEAbsID (
						  pulMyScbHd, lTaskId, tpduFct);
			       
	  /* ------------ copy Old Sndata to New Sndata ------------- */
		if ( (rc =ro133g_SnDataCpy (
                            (RO_sndata FAR *)&(pNInvoReqPb->r_sndata),
			    (R_sndata  FAR *)&(pOInvoReqPb->r_sndata))))
                return( ro133o_setHeader(&pNInvoReqPb->r_hdinfo,rc));
                   
	  /* ------ copy R_evdata to RO_buffer ----------------------- */
		ro133e_EvDataCpy ((RO_buffer FAR *)&(pNInvoReqPb->r_buffer),
				  (R_evdata  FAR *)&(pOInvoReqPb->r_evdata));

	   /* ***** Call the IROS 3.00 call ********************** */
	      
	       rc = ro_invrq_taskId (pulMyScbHd, lTaskId, tpduFct, pNInvoReqPb);

	   /* --------- copy the returned values ------------------ */
		   /* none */
 
		}
		break;
    case R_RESRQ:
		{
		R_rsrqpb    FAR * pORsltReqPb;
		RO_rsrqpb   FAR * pNRsltReqPb;
	  /* ------- data types required for Operation Invoke ------- */
		rTro00_vObjectId       vOpGlobal;
		long                   lOpLocal=0;

	  /* ---- Initialising the variables --------------------- */
	
		ro133b_memset((char FAR *)vOpGlobal,(int)0,
			      sizeof(rTro00_vObjectId));

	  /* --------- copying the data struct starts --------------- */

		pORsltReqPb = (R_rsrqpb  FAR *)& (pOExcopb->r_rsrqpb);
		pNRsltReqPb = (RO_rsrqpb FAR *)& (pNExcopb->r_rsrqpb);

                if (pORsltReqPb->r_otype != R_ACKASN ) 
                  return( ro133o_setHeader(&pNRsltReqPb->r_hdinfo,-R_HOTYPE));
		pNRsltReqPb->r_hdinfo.r_fnctnb = RO_RESRQ;
	  /* ----------- copy the <cont> flag ----------------------- */
		pNRsltReqPb->r_hdinfo.r_cont = pORsltReqPb->r_cont;
	  /* ------------ copy assid  ------------------------------- */
		pNRsltReqPb->r_assid = (unsigned long) pORsltReqPb->r_assid;
	  /* ------------ copy invoke Id----------------------------- */
		pNRsltReqPb->r_invid = (long) pORsltReqPb->r_invid;
	 
	  /* -------------- copy Operation Value -------------------- */
		pNRsltReqPb->r_opval.r_oploc = (signed long int FAR *)&lOpLocal;
		pNRsltReqPb->r_opval.r_opglb=(unsigned long int FAR *)vOpGlobal;
		ro133f_OpValuCpy ((RO_opval FAR *)&(pNRsltReqPb->r_opval),
				  (R_opval  FAR *)&(pORsltReqPb->r_opval));

	  /* ------------ copy AbsId from Old sndata----------------- */
		if ( (pORsltReqPb->r_sndata.r_fralen ) &&
		     (pORsltReqPb->r_sndata.r_fraref) )
		 pNRsltReqPb->r_absid = (int)pORsltReqPb->r_sndata.r_absid;
		else 
		 pNRsltReqPb->r_absid = ro133n_getROSEAbsID(pulMyScbHd, lTaskId, tpduFct);
		  
			       
	  /* ------------ copy Old Sndata to New Sndata ------------- */
		if((rc=ro133g_SnDataCpy(
                                 (RO_sndata FAR *)&(pNRsltReqPb->r_sndata),
				 (R_sndata  FAR *)&(pORsltReqPb->r_sndata))))
                return( ro133o_setHeader(&pNRsltReqPb->r_hdinfo,rc));

	   /* ***** Call the IROS 3.00 call ********************** */
	      
	       rc = ro_resrq_taskId (pulMyScbHd, lTaskId, tpduFct, pNRsltReqPb);

	   /* --------- copy the returned values ------------------ */
		   /* none */

		}
		break;
    case R_ERRRQ:
		{
		R_errqpb    FAR *pOErorReqPb;
		RO_errqpb   FAR *pNErorReqPb;
	  /* ------- data types required for Operation Invoke ------- */
		rTro00_vObjectId       vErGlobal;
		long                   lErLocal=0;

	  /* ---- Initialising the variables --------------------- */
	
		ro133b_memset((char FAR *)vErGlobal,(int)0,
			       sizeof(rTro00_vObjectId));

	  /* --------- copying the data struct starts --------------- */

		pOErorReqPb = (R_errqpb  FAR *)& (pOExcopb->r_errqpb);
		pNErorReqPb = (RO_errqpb FAR *)& (pNExcopb->r_errqpb);

                if (pOErorReqPb->r_otype != R_ACKASN ) 
                  return( ro133o_setHeader(&pNErorReqPb->r_hdinfo,-R_HOTYPE));
		pNErorReqPb->r_hdinfo.r_fnctnb = RO_ERRRQ;
	  /* ----------- copy the <cont> flag ----------------------- */
		pNErorReqPb->r_hdinfo.r_cont = pOErorReqPb->r_cont;
	  /* ------------ copy assid  ------------------------------- */
		pNErorReqPb->r_assid = (unsigned long) pOErorReqPb->r_assid;
	  /* ------------ copy invoke Id----------------------------- */
		pNErorReqPb->r_invid = (long) pOErorReqPb->r_invid;
	 

	  /* -------------- copy Error Value -------------------- */
		pNErorReqPb->r_erval.r_oploc = (signed long int FAR *)&lErLocal;
		pNErorReqPb->r_erval.r_opglb=(unsigned long int FAR *)vErGlobal;
		ro133f_OpValuCpy ((RO_opval FAR *)&(pNErorReqPb->r_erval),
				  (R_opval  FAR *)&(pOErorReqPb->r_erval));

	  /* ------------ copy AbsId from Old sndata----------------- */
		if ( (pOErorReqPb->r_sndata.r_fralen ) &&
		     (pOErorReqPb->r_sndata.r_fraref) )
		 pNErorReqPb->r_absid = (int)pOErorReqPb->r_sndata.r_absid;
		else 
		 pNErorReqPb->r_absid = ro133n_getROSEAbsID(pulMyScbHd, lTaskId, tpduFct);
			       
	  /* ------------ copy Old Sndata to New Sndata ------------- */
		if((rc=ro133g_SnDataCpy(
                                   (RO_sndata FAR *)&(pNErorReqPb->r_sndata),
				 (R_sndata  FAR *)&(pOErorReqPb->r_sndata))))
                return( ro133o_setHeader(&pNErorReqPb->r_hdinfo,rc));

	   /* ***** Call the IROS 3.00 call ********************** */
	      
	       rc = ro_errrq_taskId (pulMyScbHd, lTaskId, tpduFct, pNErorReqPb);

	   /* --------- copy the returned values ------------------ */
		   /* none */
 
		}
		break;
    case R_RJURQ:
		{
		
		R_rurqpb    FAR *pOURejReqPb;
		RO_rurqpb   FAR *pNURejReqPb;
		
	  /* --------- copying the data struct starts --------------- */

		pOURejReqPb = (R_rurqpb  FAR *)& (pOExcopb->r_rurqpb);
		pNURejReqPb = (RO_rurqpb FAR *)& (pNExcopb->r_rurqpb);

                if (pOURejReqPb->r_otype != R_ACKASN ) 
                  return( ro133o_setHeader(&pNURejReqPb->r_hdinfo,-R_HOTYPE));
		pNURejReqPb->r_hdinfo.r_fnctnb = RO_RJURQ;
	  /* ----------- copy the <cont> flag ----------------------- */
		pNURejReqPb->r_hdinfo.r_cont = pOURejReqPb->r_cont;
	  /* ------------ copy assid  ------------------------------- */
		pNURejReqPb->r_assid = 
			     (unsigned long) pOURejReqPb->r_assid;
	  /* ------------ copy invoke Id----------------------------- */
		pNURejReqPb->r_invid = 
			       (long) pOURejReqPb->r_invid;  
	  /* ------------ copy reason   ----------------------------- */

	       pNURejReqPb->r_reason = pOURejReqPb->r_reason;
		 
	       
	  /* ------------ copy AbsId                ----------------- */
	  
	       pNURejReqPb->r_absid = ro133n_getROSEAbsID(pulMyScbHd, lTaskId, tpduFct);
	   /* ***** Call the IROS 3.00 call ********************** */
	      
	       rc = ro_rjurq_taskId (pulMyScbHd, lTaskId, tpduFct, pNURejReqPb);

	   /* --------- copy the returned values ------------------ */
		   /* none */
 
		}
		break;
	default:
	     pNExcopb->r_hdinfo.r_errval = R_CALERR;
	     pNExcopb->r_hdinfo.r_erclas = R_ILVERS;
	     pNExcopb->r_hdinfo.r_retcod = R_CALERR;
	     rc = R_CALERR;  
	     break;
 }/*end of switch */
 return(rc);
}/*end*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1334_Logging                                      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-08                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed short int NEAR PASCAL ro1334_Logging (
unsigned long  int  FAR   *pulMyScbHd,
signed   long  int         lTaskId,
FARPROC                    tpduFct,
RO_excopb           FAR   *pNExcopb,
R_excopb            FAR   *pOExcopb)

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
signed   short int  rc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

switch(pOExcopb->r_hdinfo.r_fnctnb){
    case R_LOGON:
		{
		R_lgonpb    FAR *pOLgonPb;
		RO_lgonpb   FAR *pNLgonPb;

	  /* --------- copying the data struct starts --------------- */

		pOLgonPb = (R_lgonpb  FAR *)& (pOExcopb->r_lgonpb);
		pNLgonPb = (RO_lgonpb FAR *)& (pNExcopb->r_lgonpb);

		pNLgonPb->r_hdinfo.r_fnctnb = RO_LOGON;
	  /* ----------- copy the <cont> flag ----------------------- */
		pNLgonPb->r_hdinfo.r_cont = pOLgonPb->r_cont;
	 
	  /* ----------- copy fname --------------------------------- */
		pNLgonPb->r_fname = (char FAR *)pOLgonPb->r_fname;
	  /* ----------- copy fmode --------------------------------- */
		pNLgonPb->r_fmode = (int) pOLgonPb->r_fmode;

	  /* ----------- copy rcnum --------------------------------- */
		pNLgonPb->r_rcnum = (unsigned long) pOLgonPb->r_rcnum;

	  /* ----------- copy option--------------------------------- */
		pNLgonPb->r_option= (unsigned long ) pOLgonPb->r_option;


	   /* ***** Call the IROS 3.00 call ********************** */
	      
	       rc = ro_logon_taskId (pulMyScbHd, lTaskId, tpduFct, pNLgonPb);

	   /* --------- copy the returned values ------------------ */
		   /* none */
 
		}
		break;
    case R_LOGOF:
		{
		R_lgofpb    FAR *pOLgofPb;
		RO_lgofpb   FAR *pNLgofPb;

	  /* --------- copying the data struct starts --------------- */

		pOLgofPb = (R_lgofpb  FAR *)& (pOExcopb->r_lgofpb);
		pNLgofPb = (RO_lgofpb FAR *)& (pNExcopb->r_lgofpb);

		pNLgofPb->r_hdinfo.r_fnctnb = RO_LOGOF;
	  /* ----------- copy the <cont> flag ----------------------- */
		pNLgofPb->r_hdinfo.r_cont = pOLgofPb->r_cont;
	 

	   /* ***** Call the IROS 3.00 call ********************** */
	      
	       rc = ro_logof_taskId (pulMyScbHd, lTaskId, tpduFct, pNLgofPb);

	   /* --------- copy the returned values ------------------ */
		   /* none */
 
		}
		break;
    case R_LOGWR:
		{
		R_lgwrpb    FAR *pOLgwrPb;
		RO_lgwrpb   FAR *pNLgwrPb;

	  /* --------- copying the data struct starts --------------- */

		pOLgwrPb = (R_lgwrpb  FAR *)& (pOExcopb->r_lgwrpb);
		pNLgwrPb = (RO_lgwrpb FAR *)& (pNExcopb->r_lgwrpb);

		pNLgwrPb->r_hdinfo.r_fnctnb = RO_LOGWR;
	  /* ----------- copy the <cont> flag ----------------------- */
		pNLgwrPb->r_hdinfo.r_cont = pOLgwrPb->r_cont;
	 
	  /* ----------- copy option--------------------------------- */
		pNLgwrPb->r_option= (unsigned long ) pOLgwrPb->r_option;
	  
	  /* ----------- copy rectype--------------------------------- */
		pNLgwrPb->r_rectyp= (long ) pOLgwrPb->r_rectyp;

	  /* ----------- copy reclen--------------------------------- */
		pNLgwrPb->r_reclen= (unsigned long ) pOLgwrPb->r_reclen;

	  /* ----------- copy recptr--------------------------------- */
		pNLgwrPb->r_recptr= (char FAR *) pOLgwrPb->r_recptr;

	   /* ***** Call the IROS 3.00 call ********************** */
	      
	       rc = ro_logwr_taskId (pulMyScbHd, lTaskId, tpduFct, pNLgwrPb);

	   /* --------- copy the returned values ------------------ */
		   /* none */
 
 
		}
		break;
	default:
	     pNExcopb->r_hdinfo.r_errval = R_CALERR;
	     pNExcopb->r_hdinfo.r_erclas = R_ILVERS;
	     pNExcopb->r_hdinfo.r_retcod = R_CALERR;
	     rc = R_CALERR;  
	     break;

 }/*end of switch */
 return(rc);

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1335_Inform                                       */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


static signed short int NEAR PASCAL ro1335_Inform (
unsigned long  int  FAR   *pulMyScbHd,
signed   long  int         lTaskId,
FARPROC                    tpduFct,
R_excopb            FAR   *pOExcopb,
RO_excopb           FAR   *pNExcopb)

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
signed    short int       rc;
unsigned  short int       uCount;
unsigned  short int       i;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
/* ******************           ************************************* */
switch(pNExcopb->r_inform.r_inftyp){
     case RO_GTDCS:
	{
	 RO_gtdcif                 stGetDcs;
	 RO_gtdcif           FAR * pNGetDcs;
	 R_getdpb            FAR * pOGetDcs;

	pOGetDcs = (R_getdpb  FAR *)&(pOExcopb->r_getdpb);
	/* --------------- copy the <cont> flag ---------------------------- */
	pNExcopb->r_hdinfo.r_cont = pOGetDcs->r_cont;


	/* --------------- copy the typedef for r_infval ------------------- */
 
	pNGetDcs =  pNExcopb->r_inform.r_infval
		 =  (RO_gtdcif FAR *)&stGetDcs;

	/* --------------- copy the assid ---------------------------- */
	pNGetDcs->r_assid = pOGetDcs->r_assid;

	/* ---------------- assign array number ---------------------- */
	pNGetDcs->r_panum = RO_maxExternals;
 
	/* ***** Call the IROS 3.00 call ********************** */
	      
	rc = ro_infrm_taskId (pulMyScbHd, lTaskId, tpduFct, & (pNExcopb->r_inform ));

	/* --------- copy the returned values ------------------ */

	/* --------------- copy the pcnum ---------------------------- */
	if (rc == OK) {
	    pOGetDcs->r_pcnum = (R_unsign) pNGetDcs->r_pcnum;
 
	/* -------------- copy the PresCxt --------------------------- */
	   uCount = MIN(pOGetDcs->r_pcnum ,RO_maxExternals);
	   for ( i=0;i<uCount;i++){
	      pOGetDcs->r_prcnt[i].r_absid = pNGetDcs->r_prcxt[i].r_absid;
	      pOGetDcs->r_prcnt[i].r_trsid = pNGetDcs->r_prcxt[i].r_trsid;
    
    /* !!! what to do with pctid & more elements of the IROS2.5 */
    /* Information will be lost */

	    }
       }else 
	    pOGetDcs->r_pcnum = (R_unsign) 0;
     }
     break;
     
    case RO_GTPBF:
	 {
	 RO_gtpbif                 stPrBufC;
	 RO_gtpbif           FAR * pNPrBufC;
	 R_getpbf            FAR * pOPrBufC;

	 
	pOPrBufC = (R_getpbf  FAR *)&(pOExcopb->r_getpbf);
	/* --------------- copy the <cont> flag ---------------------------- */
	pNExcopb->r_hdinfo.r_cont = pOPrBufC->r_cont;


	/* --------------- copy the typedef for r_infval ------------------- */
 
	pNPrBufC =  pNExcopb->r_inform.r_infval
		 =  (RO_gtpbif FAR *)&stPrBufC;

	/* --------------- copy the aplid ---------------------------- */
	pNPrBufC->r_aplid = (unsigned long)pOPrBufC->r_aplid;


	/* ***** Call the IROS 3.00 call ********************** */
	      
	rc = ro_infrm_taskId (pulMyScbHd, lTaskId, tpduFct, & (pNExcopb->r_inform ));

	/* --------------- copy the pcnum ---------------------------- */
	if (rc == OK) 
	    pOPrBufC->r_number = (R_unsign) pNPrBufC->r_number;
	else 
	    pOPrBufC->r_number = (R_unsign) 0;
 

	 }
	 break;
	

 }

 return (rc); 
	   
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1336_redirect                                     */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-10                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


static signed short int NEAR PASCAL ro1336_redirect(
unsigned long  int  FAR   *pulMyScbHd,
signed   long  int         lTaskId,
FARPROC                    tpduFct,
RO_rdrqpb           FAR   *pNRedirect,
R_rdrqpb            FAR   *pORedirect)

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
signed    short int rc;


 rTro00_vObjectId       vApplProcessTitle;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

 /* ---- Initialising the variables --------------------- */
	
 ro133b_memset((char FAR *)vApplProcessTitle,(int)0,sizeof(rTro00_vObjectId));
 /* --------------- copy the <cont> flag ---------------------------- */
 pNRedirect->r_hdinfo.r_cont = pORedirect->r_cont;

 /* --------------- copy assid -------------------------------------- */

 pNRedirect->r_assid = (unsigned long) pORedirect->r_assid;
 /* --------------- copy pid   -------------------------------------- */

 pNRedirect->r_pid   = (long) pORedirect->r_pid  ;


 /* ---------------- copy aetitl of Calling Appl ref -------- */
/* rc = ro133a_Objcpy(vApplProcessTitle,
	       pORedirect->r_ptcon.r_obidrf,
	       pORedirect->r_ptcon.r_obidnb);
 if (rc == 0)
     pNRedirect->r_aetitl.r_aptitl = vApplProcessTitle; */
 /* --------- AE qualifier is OPTIONAL so NULL -------------- */
    
/* pNRedirect->r_aetitl.r_aequal = (long FAR *)NULL; */
 /* --------- Pres Addr of the Calling Appl ref ------------- */
/* ro133c_PresAddrCpy(&(pNRedirect->r_pradd),
		   &(pORedirect->r_ptnref)); */
 
 /* ----------- copy R_sndat to RO_sndfra --------------- */

/* ro133d_SDataCpy(&(pNRedirect->r_sndfra),
		 &(pORedirect->r_sndata)); */

 
 /* ***** Call the IROS 3.00 call ********************** */
	      
 rc = ro_redrq_taskId (pulMyScbHd, lTaskId, tpduFct, pNRedirect);

 /* --------- copy the returned values ------------------ */
 /* none */



 return (rc); 
	   
}/*end*/                                                                        



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133k_setParameter                                 */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-20                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static char FAR * NEAR PASCAL ro133k_setParameter (
	char                      FAR  *bassis,
	char                FAR * FAR  *unUsedBuf,
	signed   long  int        FAR  *unUsedLen,
	char                      FAR  *parbuf,
	unsigned long  int              parlen,
	signed short  int               divFactor)




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

char         FAR          *rc;
int                   offSet;
unsigned long int          i;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */


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
	    rc          = *unUsedBuf;
	    *unUsedBuf += parlen;

       for (i=0; i < parlen; i++) *(rc+i) = *(parbuf+i);
       }
}
return(rc);

}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133p_retrieveAeTitle                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-11                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static    signed short int NEAR PASCAL ro133p_retrieveAeTitle(
	    R_evtpb               FAR * pEvtPb  ,
	    char           FAR *  FAR * pUnUsedBuf,
	    signed long int       FAR * pUnUsedLen,
	    RO_apname             FAR * pNApName,
	    R_objid               FAR * pOAetlId)

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
signed short int rc = OK;
/* *************** check mode **************************************** */
   if (!pNApName->r_aetval){
     pOAetlId->r_obidrf = (R_unsign F *)NULL;
     pOAetlId->r_obidnb = 0;
   }
   switch(pNApName->r_aptype){
    case RO_AETF1:
     pOAetlId->r_obidrf = (R_unsign F *)NULL;
     pOAetlId->r_obidnb = 0;
      break;
    case RO_AETF2:
         {
#ifndef __hpux

/* Commented these line out because pAeTitl2->r_aptitl is a NULL pointer
   caused ro133l_retrieveObjID function to core dump */

          RO_aetfm2 F * pAeTitl2 = (RO_aetfm2 F *) pNApName->r_aetval;
	  rc = ro133l_retrieveObjID( 
		      pEvtPb,
		     (char FAR * FAR *)pUnUsedBuf,
		     (signed long int FAR *)pUnUsedLen,
		     (unsigned long int FAR *)pAeTitl2->r_aptitl,
		     (R_unsign FAR * FAR *)&(pOAetlId->r_obidrf),
		     (R_unsign FAR *)&(pOAetlId->r_obidnb) );
#endif
        } 
      break;
    default:
     pOAetlId->r_obidrf = (R_unsign F *)NULL;
     pOAetlId->r_obidnb = 0;
     break;
   }

return (rc);
} /* end */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1337_CheckOrWait                                  */ 
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-10                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


static signed short int NEAR PASCAL ro1337_CheckOrWait (
unsigned long  int  FAR   *pulMyScbHd,
signed   long  int         lTaskId,
FARPROC                    tpduFct,
RO_evtpb            FAR   *pNEventPb,
R_evtpb             FAR   *pOEventPb)



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
 signed   short int   rc = OK;

 char             FAR *pUnUsedBuf;
 signed long int       UnUsedLen;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

 pUnUsedBuf =  (char FAR *)pOEventPb;
 UnUsedLen  = (signed long int)sizeof(R_evtpb);

 /* ---------------- copy the Header -------------------------------- */

 pOEventPb->r_evnthd.r_subid =(R_idtype)NULL; 
 pOEventPb->r_evnthd.r_uconwd =(char *)NULL; 
 pOEventPb->r_evnthd.r_evlink =(char *)NULL; 

 /* --------------- copy as per the Ev type ------------------------ */

 switch(pNEventPb->r_evnthd.r_evtype){
      case RO_NOEVET:
		rc = R_NOEVNT;
		break; 
      case RO_TIMEUT:
		rc  = R_TIMOUT;
		break; 
      case RO_RBINEV:
	      {
		R_rbinev  FAR * pORetrnBufEv =
                                          (R_rbinev FAR *)&pOEventPb->r_rbinev;
		RO_rbinev FAR * pNRetrnBufEv =
                                          (RO_rbinev FAR *)&pNEventPb->r_rbinev;

 
		pUnUsedBuf += sizeof(R_rbinev);
		UnUsedLen  -= sizeof (R_rbinev); 
		pORetrnBufEv->r_evnthd.r_evtype = R_RBINEV;
	   /* ------------ copy the buffer types ------------- */
		pORetrnBufEv->r_buffer.r_mtype =
			    (R_mtype)pNRetrnBufEv->r_buffer.r_mtype;
 
		pORetrnBufEv->r_buffer.r_buflen =
			(R_unsign)pNRetrnBufEv->r_buffer.r_buflen; 
	  /* ----- Buffer pointer only copied since Buffer is - */
	  /* -  given by the interface only ------------------- */ 
		switch(pNRetrnBufEv->r_buffer.r_mtype){
		   case RO_MEMORY:
			pORetrnBufEv->r_buffer.r_bufref = 
			       (char FAR *) pNRetrnBufEv->r_buffer.r_bufref;
			break;
		   case RO_FILE:
			pORetrnBufEv->r_buffer.r_bufref = (char FAR *)
			  ro133k_setParameter (
			      (char FAR *)pOEventPb                         ,
			      (char FAR * FAR *)&pUnUsedBuf                 ,
			      (signed long int FAR *)&UnUsedLen             ,
			      (char FAR * )pNRetrnBufEv->r_buffer.r_bufref  ,
			      pNRetrnBufEv->r_buffer.r_buflen               ,
			      (signed short int)4                          );
			break;
#ifdef RP0000_MicV0600
		  case RO_GLOHAN:
			pORetrnBufEv->r_buffer.r_bufref = (char FAR *)
			  ro133k_setParameter( (char FAR *)pOEventPb,
			      &pUnUsedBuf,
			      &UnUsedLen,
			      (char FAR * )pNRetrnBufEv->r_buffer.r_bufref,
			      sizeof(unsigned long int)     ,  
			      (signed short int )4                          );

		       break;
#endif     


	       }                                 
				  
	      }
		break; 
      case RO_BNINEV:
	       {
		R_bninev  FAR * pOBndIndEv =
                                        (R_bninev  FAR *)&(pOEventPb->r_bninev);
		RO_bninev FAR * pNBndIndEv =
                                        (RO_bninev FAR *)&(pNEventPb->r_bninev);

		rT06z0_stInfrmApplCxtName stApplCxt;

		pUnUsedBuf += sizeof(R_bninev);
		UnUsedLen  -= sizeof (R_bninev); 
	       
	       pOBndIndEv->r_evnthd.r_evtype = R_BNINEV;
      /* ------------ copy Assid ---------------------------------- */
	       pOBndIndEv->r_assid  = 
			  (R_idtype)pNBndIndEv->r_assid;
      /* -------------- copy aplid ------------------------------- */
	       pOBndIndEv->r_aplid  = 
			  (R_idtype)pNBndIndEv->r_aplid;
      /* ------------ copy pid ---------------------------------- */
	       pOBndIndEv->r_pid  = 
			  (R_idtype)pNBndIndEv->r_pid;
      /* ------------ copy ptcon ---------------------------------- */
	       /* !!! get the Objid by inform fn. */ 
		stApplCxt.sApplCxtId = pNBndIndEv->r_ptcon; 
		stApplCxt.ulApplID = pNBndIndEv->r_aplid; 
		stApplCxt.ulAssId = pNBndIndEv->r_assid;
		rc = ro1338_GetApplContext (pulMyScbHd, lTaskId, tpduFct,
			   (rT06z0_stInfrmApplCxtName  FAR  *)&stApplCxt,
			    RC06z1_ApplCxtName);
	     
		if(rc >= RO_SUCES)
		    rc = ro133l_retrieveObjID( 
			   pOEventPb,
			  (char FAR * FAR *)&pUnUsedBuf,
			  (signed long int FAR *)&UnUsedLen,
			  (unsigned long int FAR *)stApplCxt.vApplCxtName,
			  (R_unsign FAR * FAR *)&(pOBndIndEv->r_ptcon.r_obidrf),
			  (R_unsign FAR *)&(pOBndIndEv->r_ptcon.r_obidnb));
		else pOBndIndEv->r_ptcon.r_obidnb = 0; /* !!! no Objid */
	 /* !!! what to do if failed */

      /* ------------- copy ptnref from aetitl & pradd ------------  */
	       pOBndIndEv->r_ptnref.r_paddrf = (char FAR *)
		      ro133k_setParameter (
			      (char FAR *)pOEventPb                     ,
			      (char FAR * FAR *)&pUnUsedBuf             ,
			      (signed long int FAR *)&UnUsedLen         ,
			      (char FAR * )pNBndIndEv->r_pradd.r_paddrf ,
			      pNBndIndEv->r_pradd.r_paddln              ,
			      (signed short int )4                      );

	       pOBndIndEv->r_ptnref.r_paddln = (R_unsign)
					    pNBndIndEv->r_pradd.r_paddln;
		    
	  /* --------- copy AE title -------------------------- */
	       pOBndIndEv->r_ptnref.r_aetitl.r_obidrf = 
					   (R_unsign FAR *)pUnUsedBuf;
/*	       rc = ro133l_retrieveObjID( 
		 pOEventPb,
		(char      FAR * FAR *)&pUnUsedBuf,
		(signed long int FAR *)&UnUsedLen,                
		(unsigned long int FAR *)pNBndIndEv->r_aetitl.r_aptitl,
		(R_unsign FAR * FAR *)&(pOBndIndEv->r_ptnref.r_aetitl.r_obidrf),
		(R_unsign FAR *)&(pOBndIndEv->r_ptnref.r_aetitl.r_obidnb));
 */     
	       rc = ro133p_retrieveAeTitle( 
		      pOEventPb,
		     (char FAR * FAR *)&pUnUsedBuf,
		     (signed long int FAR *)&UnUsedLen,                
		     (RO_apname FAR *)&pNBndIndEv->r_aetitl, 
		     (R_objid FAR *)&(pOBndIndEv->r_ptnref.r_aetitl) );
      /* ------------- copy evdata from evfrag -------------------- */
      /* --------- call the buf copy ------------------------------ */
	       rc = ro133i_retrieveEData(
		       pOEventPb,
		      (char FAR * FAR *)&pUnUsedBuf,
		      (signed long int FAR *)&UnUsedLen,                
		      (RO_evfrag FAR *)&(pNBndIndEv->r_evfrag), 
		      (R_evdata FAR *)&(pOBndIndEv->r_evdata));
	    }
	    break; 
      case RO_BNCNEV:
	       {
		R_bncnev  FAR * pOBndCnfEv =
                                         (R_bncnev  FAR *)&pOEventPb->r_bncnev;
		RO_bncnev FAR * pNBndCnfEv =
                                         (RO_bncnev FAR *)&pNEventPb->r_bncnev;

		rT06z0_stInfrmApplCxtName stApplCxt;

		pUnUsedBuf += sizeof(R_bncnev);
		UnUsedLen  -= sizeof (R_bncnev); 
	       
	       pOBndCnfEv->r_evnthd.r_evtype = R_BNCNEV;
      /* ------------ copy Assid ---------------------------------- */
	       pOBndCnfEv->r_assid  = 
			  (R_idtype)pNBndCnfEv->r_assid;
      /* -------------- copy result------------------------------- */
/*               pOBndCnfEv->r_result = 0;
	       if ( (pNBndCnfEv->r_result & 0x000000f0) )
		  pOBndCnfEv->r_result += 7100 ;
	       pOBndCnfEv->r_result += (R_int)
			 ( pNBndCnfEv->r_result & 0x0000000f);  */
     
	       pOBndCnfEv->r_result = 0x0000;
	       pOBndCnfEv->r_result |= 
		     ((pNBndCnfEv->r_result & 0x000000f0) << 4 )   ;
	       pOBndCnfEv->r_result |= 
		     (pNBndCnfEv->r_result & 0x0000000f)    ;

      /* ------------ copy ptcon ---------------------------------- */
	       /* !!! get the Objid by inform fn. */ 
		stApplCxt.sApplCxtId = pNBndCnfEv->r_ptcon; 
		stApplCxt.ulApplID = 0;
                if( RO_ISACCEPT(pNBndCnfEv->r_result))
          		stApplCxt.ulAssId = pNBndCnfEv->r_assid; 
                else stApplCxt.ulAssId =NULL;
		rc = ro1338_GetApplContext (pulMyScbHd, lTaskId, tpduFct,
			   (rT06z0_stInfrmApplCxtName  FAR  *)&stApplCxt,
			    RC06z1_ApplCxtName);
	     
		if(rc >= RO_SUCES)
		    rc = ro133l_retrieveObjID( 
			  pOEventPb,
			  (char FAR * FAR *)&pUnUsedBuf,
			  (signed long int FAR *)&UnUsedLen,                
			  (unsigned long int FAR *)stApplCxt.vApplCxtName,    
			  (R_unsign FAR * FAR *)&(pOBndCnfEv->r_ptcon.r_obidrf),
			  (R_unsign FAR *)&(pOBndCnfEv->r_ptcon.r_obidnb));

		else pOBndCnfEv->r_ptcon.r_obidnb = 0; /* !!! no Objid */
	 /* !!! what to do if failed */
      /* ------------- copy ptnref from aetitl & pradd ------------  */
	       
	       pOBndCnfEv->r_pradd.r_paddrf = (char FAR *)
		      ro133k_setParameter (
			    (char FAR *)pOEventPb                       ,
			    (char FAR * FAR *)&pUnUsedBuf               ,
			    (signed long int FAR *)&UnUsedLen           ,
			    (char FAR * )pNBndCnfEv->r_pradd.r_paddrf   ,
			    pNBndCnfEv->r_pradd.r_paddln                ,
			    (signed short int)4                         );
	       pOBndCnfEv->r_pradd.r_paddln = (R_unsign)
				    pNBndCnfEv->r_pradd.r_paddln;
	      
	  /* --------- copy AE title -------------------------- */
/*
	       rc = ro133l_retrieveObjID( 
		      pOEventPb,
		     (char FAR * FAR *)&pUnUsedBuf,
		     (signed long int FAR *)&UnUsedLen,                
		     (unsigned long int FAR *)pNBndCnfEv->r_aetitl.r_aptitl, 
		     (R_unsign FAR * FAR *)&(pOBndCnfEv->r_aetitl.r_obidrf),
		     (R_unsign FAR *)&(pOBndCnfEv->r_aetitl.r_obidnb) );
*/
      
	       rc = ro133p_retrieveAeTitle( 
		      pOEventPb,
		     (char FAR * FAR *)&pUnUsedBuf,
		     (signed long int FAR *)&UnUsedLen,                
		     (RO_apname FAR *)&pNBndCnfEv->r_aetitl, 
		     (R_objid FAR *)&(pOBndCnfEv->r_aetitl) );
      /* ------------- copy evdata from evfrag -------------------- */
      /* --------- call the buf copy ------------------------------ */
	       rc = ro133i_retrieveEData(
		       pOEventPb,
		      (char FAR * FAR *)&pUnUsedBuf,
		      (signed long int FAR *)&UnUsedLen,                
		      (RO_evfrag FAR *)&(pNBndCnfEv->r_evfrag), 
		      (R_evdata FAR *)&(pOBndCnfEv->r_evdata));
	    }
	    break;
      case RO_UBINEV:
	       {
		R_ubinev  FAR * pOUBndIndEv =
                                        (R_ubinev  FAR *)&pOEventPb->r_ubinev;
		RO_ubinev FAR * pNUBndIndEv =
                                        (RO_ubinev FAR *)&pNEventPb->r_ubinev;

		pUnUsedBuf += sizeof(R_ubinev);
		UnUsedLen  -= sizeof (R_ubinev); 
	       
	       pOUBndIndEv->r_evnthd.r_evtype = R_UBINEV;
      /* ------------ copy Assid ---------------------------------- */
	       pOUBndIndEv->r_assid  = 
			  (R_idtype)pNUBndIndEv->r_assid;
      
      /* ------------- copy evdata from evfrag -------------------- */
      /* --------- call the buf copy ------------------------------ */
	       rc = ro133i_retrieveEData(
		       pOEventPb,
		      (char FAR * FAR *)&pUnUsedBuf,
		      (signed long int FAR *)&UnUsedLen,                
		      (RO_evfrag FAR *)&(pNUBndIndEv->r_evfrag), 
		      (R_evdata  FAR *)&(pOUBndIndEv->r_evdata));
	    }
	    break;
      case RO_UBCNEV:
	       {
		R_ubcnev  FAR * pOUBndCnfEv =
                                         (R_ubcnev  FAR *)&pOEventPb->r_ubcnev;
		RO_ubcnev FAR * pNUBndCnfEv =
                                         (RO_ubcnev FAR *)&pNEventPb->r_ubcnev;

		pUnUsedBuf += sizeof(R_ubcnev);
		UnUsedLen  -= sizeof (R_ubcnev); 
	       
	       pOUBndCnfEv->r_evnthd.r_evtype = R_UBCNEV;
      /* ------------ copy Assid ---------------------------------- */
	       pOUBndCnfEv->r_assid  = 
			  (R_idtype)pNUBndCnfEv->r_assid;
      /* ------------ copy fail  ---------------------------------- */
                if(pNUBndCnfEv->r_ureasn == RO_NotFinished)  
                   pOUBndCnfEv->r_fail = R_TRUE;             
                else                                         
                 pOUBndCnfEv->r_fail  = R_FALSE;             

      
      /* ------------- copy evdata from evfrag -------------------- */
      /* --------- call the buf copy ------------------------------ */
	       rc = ro133i_retrieveEData(
		       pOEventPb,
		      (char FAR * FAR *)&pUnUsedBuf,
		      (signed long int FAR *)&UnUsedLen,                
		      (RO_evfrag FAR *)&(pNUBndCnfEv->r_evfrag), 
		      (R_evdata  FAR *)&(pOUBndCnfEv->r_evdata));
	    }
	    break;

      case RO_ABINEV:
	       {
		R_abinev  FAR * pOAbrIndEv =
                                         (R_abinev  FAR *)&pOEventPb->r_abinev;
		RO_abinev FAR * pNAbrIndEv =
                                         (RO_abinev FAR *)&pNEventPb->r_abinev;

		pUnUsedBuf += sizeof(R_abinev);
		UnUsedLen  -= sizeof (R_abinev); 
	       
	       pOAbrIndEv->r_evnthd.r_evtype = R_ABINEV;
      /* ------------ copy Assid ---------------------------------- */
	       pOAbrIndEv->r_assid  = 
			  (R_idtype)pNAbrIndEv->r_assid;
      /* -------------- copy result ------------------------------- */
	       pOAbrIndEv->r_result  = 
			  (R_int)pNAbrIndEv->r_result;
      /* ------------ copy adinfo ---------------------------------- */
	       pOAbrIndEv->r_adinfo  = 
			  (R_int)pNAbrIndEv->r_adinfo;
      
      /* ------------- copy evdata from evfrag -------------------- */
      /* --------- call the buf copy ------------------------------ */
	       rc = ro133i_retrieveEData(
			pOEventPb,
		       (char FAR * FAR *)&pUnUsedBuf,
		       (signed long int FAR *)&UnUsedLen,                
		       (RO_evfrag FAR *)&(pNAbrIndEv->r_evfrag), 
		       (R_evdata  FAR *)&(pOAbrIndEv->r_evdata));
	    }
	    break;

    
      case RO_IVINEV:
	       {
		R_ivinev  FAR * pOInvIndEv =
                                        (R_ivinev  FAR *)&pOEventPb->r_ivinev;
		RO_ivinev FAR * pNInvIndEv =
                                        (RO_ivinev FAR *)&pNEventPb->r_ivinev;

		pUnUsedBuf += sizeof(R_ivinev);
		UnUsedLen  -= sizeof (R_ivinev); 
	       
	       pOInvIndEv->r_evnthd.r_evtype = R_IVINEV;

      /* ------------ copy Assid ---------------------------------- */
	       pOInvIndEv->r_assid  = 
			  (R_idtype)pNInvIndEv->r_assid;

      /* -------------- copy invid ------------------------------- */
	       pOInvIndEv->r_invid  = 
			  (R_int)pNInvIndEv->r_invid;

      /* -------------- copy the Opvalue -------------------------- */
	       rc = ro133m_retrieveOpValue(
			 pOEventPb,
			(char FAR * FAR *)&pUnUsedBuf,
			(signed long int FAR *)&UnUsedLen,                
			(RO_opval FAR *)&(pNInvIndEv->r_opval),  
			(R_opval  FAR *)&(pOInvIndEv->r_opval) );

      /* ------------ copy Linked id -----------------------------  */
	       if (pNInvIndEv->r_lnkid) {
		     pOInvIndEv->r_lnkid = (R_int FAR *)
			 ro133k_setParameter (
			      (char FAR *)pOEventPb                    ,
			      (char FAR * FAR *)&pUnUsedBuf            ,
			      (signed long int FAR *)&UnUsedLen        ,
			      (char FAR * )pNInvIndEv->r_lnkid         ,
			      (unsigned long int)(sizeof (long))       ,
			      (signed short int)4                      );
			 
	       }
	       else    /* no linked id */
		     pOInvIndEv->r_lnkid = (R_int *)NULL;
		 
			
      /* ------------- copy R_evdata from RO_evdata ----------------- */
      /* --------- call the buf copy ------------------------------ */
	       rc = ro133j_retrieveEvData(
			 pOEventPb,
			(char FAR * FAR *)&pUnUsedBuf,
			(signed long int FAR *)&UnUsedLen,                
			(RO_evdata FAR *)&(pNInvIndEv->r_evdata), 
			(R_evdata  FAR *)&(pOInvIndEv->r_evdata));
	    }
	    break;
      case RO_RSINEV:
	       {
		R_rsinev  FAR * pOResIndEv =
                                         (R_rsinev  FAR *)&pOEventPb->r_rsinev;
		RO_rsinev FAR * pNResIndEv =
                                         (RO_rsinev FAR *)&pNEventPb->r_rsinev;

	       pUnUsedBuf += sizeof(R_rsinev);
	       UnUsedLen  -= sizeof (R_rsinev); 
	       
	       pOResIndEv->r_evnthd.r_evtype = R_RSINEV;
      /* ------------ copy Assid ---------------------------------- */
	       pOResIndEv->r_assid  = 
			  (R_idtype)pNResIndEv->r_assid;
      /* -------------- copy invid ------------------------------- */
	       pOResIndEv->r_invid  = 
			  (R_int)pNResIndEv->r_invid;
      /* -------------- copy the Opvalue -------------------------- */

	       rc = ro133m_retrieveOpValue(
			pOEventPb,
		       (char FAR * FAR *)&pUnUsedBuf,
		       (signed long int FAR *)&UnUsedLen,                
		       (RO_opval FAR *)&(pNResIndEv->r_opval),  
		       (R_opval  FAR *)&(pOResIndEv->r_opval) );
			
      /* ------------- copy R_evdata from RO_evdata ----------------- */
      /* --------- call the buf copy ------------------------------ */
	       rc = ro133j_retrieveEvData(
			 pOEventPb,
			(char FAR * FAR *)&pUnUsedBuf,
			(signed long int FAR *)&UnUsedLen,                
			(RO_evdata FAR *)&(pNResIndEv->r_evdata), 
			(R_evdata  FAR *)&(pOResIndEv->r_evdata));
	    }
	    break;
      case RO_ERINEV:
	       {
		R_erinev  FAR * pOErrIndEv =
                                        (R_erinev  FAR *)&pOEventPb->r_erinev;
		RO_erinev FAR * pNErrIndEv =
                                        (RO_erinev FAR *)&pNEventPb->r_erinev;

		pUnUsedBuf += sizeof(R_erinev);
		UnUsedLen  -= sizeof (R_erinev); 
	       
	       pOErrIndEv->r_evnthd.r_evtype = R_ERINEV;
      /* ------------ copy Assid ---------------------------------- */
	       pOErrIndEv->r_assid  = 
			  (R_idtype)pNErrIndEv->r_assid;
      /* -------------- copy invid ------------------------------- */
	       pOErrIndEv->r_invid  = 
			  (R_int)pNErrIndEv->r_invid;
      /* -------------- copy the Ervalue -------------------------- */

	       rc = ro133m_retrieveOpValue(
			 pOEventPb,
			(char FAR * FAR *)&pUnUsedBuf,
			(signed long int FAR *)&UnUsedLen,                
			(RO_opval FAR *)&(pNErrIndEv->r_erval),  
			(R_opval  FAR *)&(pOErrIndEv->r_erval) );
			
      /* ------------- copy R_evdata from RO_evdata ----------------- */
      /* --------- call the buf copy ------------------------------ */
	       rc = ro133j_retrieveEvData(
			 pOEventPb,
			(char FAR * FAR *)&pUnUsedBuf,
			(signed long int FAR *)&UnUsedLen,                
			(RO_evdata FAR *)&(pNErrIndEv->r_evdata), 
			(R_evdata  FAR *)&(pOErrIndEv->r_evdata));
	    }
	    break;
      case RO_RUINEV:
	       {
		R_ruinev  FAR * pORjUIndEv =
                                          (R_ruinev  FAR *)&pOEventPb->r_ruinev;
		RO_ruinev FAR * pNRjUIndEv =
                                          (RO_ruinev FAR *)&pNEventPb->r_ruinev;

	       
	       pORjUIndEv->r_evnthd.r_evtype = R_RUINEV;
      /* ------------ copy Assid ---------------------------------- */
	       pORjUIndEv->r_assid  = 
			  (R_idtype)pNRjUIndEv->r_assid;
      /* -------------- copy invid ------------------------------- */
	       pORjUIndEv->r_invid  = 
			  (R_int)pNRjUIndEv->r_invid;
      /* -------------- copy reason ------------------------------- */
/*               if ( pNRjUIndEv->r_reason == RO_NTRAN  )
		    pORjUIndEv->r_reason = R_NTRAN;
	       else {
		  unsigned long int  i = 
			   (pNRjUIndEv->r_reason & 0x000f0000) >> 16;  
		  pORjUIndEv->r_reason  = (R_reason)7000;
		  pORjUIndEv->r_reason  += (R_reason)(i  * 100) ;
		  pORjUIndEv->r_reason  += (R_reason)
				 (pNRjUIndEv->r_reason & 0x000000ff);*/
		pORjUIndEv->r_reason = pNRjUIndEv->r_reason;
 

	    }
	    break;
      case RO_RPINEV:
	       {
		R_rpinev  FAR * pORjPIndEv =
                                        (R_rpinev  FAR *)&pOEventPb->r_rpinev;
		RO_rpinev FAR * pNRjPIndEv =
                                        (RO_rpinev FAR *)&pNEventPb->r_rpinev;

	       
	       pORjPIndEv->r_evnthd.r_evtype = R_RPINEV;
      /* ------------ copy Assid ---------------------------------- */
	       pORjPIndEv->r_assid  = 
			  (R_idtype)pNRjPIndEv->r_assid;
      /* ------------ copy Reason ---------------------------------- */
/*
	       if ( pNRjPIndEv->r_reason == RO_NTRAN  )
		    pORjPIndEv->r_reason = R_NTRAN;
	       else {
		  pORjPIndEv->r_reason  = 7000;
		  pORjPIndEv->r_reason  += (R_reason)
			    ((pNRjPIndEv->r_reason & 0x000f0000) * 100 );
		  pORjPIndEv->r_reason  += (R_reason)
				       (pNRjPIndEv->r_reason & 0x000000ff);*/
		pORjPIndEv->r_reason = pNRjPIndEv->r_reason;

      /* ----------- set valid ---------------------------------- */
	       pORjPIndEv->r_valid = R_TRUE;  /* default */
      /* -------------- copy invid if present -------------------- */
	       if (pNRjPIndEv->r_invid) 
		   pORjPIndEv->r_invid  = 
			  (R_int)*pNRjPIndEv->r_invid;
	       else {
		   pORjPIndEv->r_invid  = 
			  (R_int)NULL;
	     /* reset the valid flag */
		   pORjPIndEv->r_valid = R_FALSE;
	      }
		   
      
      /* ---- ------- copy prdata --------------------------------- */
      /* !!!consult with Muller */
	    }
	    break; 

      case RO_IVRQEV:
      case RO_RSRQEV:
      case RO_ERRQEV:
      case RO_RURQEV:  /* !!!! what to do inthese cases */
	     break;
  } /*end of switch*/
  return(rc);
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1338_GetApplContext                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-11                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed short int NEAR PASCAL ro1338_GetApplContext (
unsigned long  int         FAR   *pulMyScbHd,
signed   long  int                lTaskId,
FARPROC                           tpduFct,
rT06z0_stInfrmApplCxtName  FAR   *pstApplCxt,
signed         int                sType)

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
 signed short int rc=0;
 RO_excopb        unExcopb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

 unExcopb.r_hdinfo.r_cont   = R_FALSE;
 unExcopb.r_hdinfo.r_fnctnb = RO_INFRM;
 unExcopb.r_hdinfo.r_versnb = RO_VERSION;
 unExcopb.r_inform.r_inftyp = sType;     
 unExcopb.r_inform.r_infval = (void FAR *) pstApplCxt;

		    
 /* --------- Use the IROS 3.00 call ----------------------- */
 rc = ro_infrm_taskId (pulMyScbHd, lTaskId, tpduFct, &unExcopb.r_inform);

 return(rc);    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133b_memset                                       */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-05                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static   void NEAR PASCAL ro133b_memset (
  char FAR *area,
  int byte,
  unsigned long int length)

/* DESCRIPTION  :   This function overwrites an <area> of the memory  */
/*                  with <byte>. This is done sequentially <length>   */
/*                  times.                                            */
/*                                                                    */
/* INPUT-PARAMETERS :                                                 */
/*                                                                    */
/*      area1 :  Startaddress in memory.                              */
/*       byte :  content of the area after the operation              */
/*     length :  how many bytes to overwrite.                         */
/*                                                                    */
/* OUTPUT-PARAMETERS :  none                                          */
/*                                                                    */
/* RETURN VALUES :      none                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS : none                                                     */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

/*
#ifdef unix
     memset (area,byte,length);
#else ifdef dos
    _fmemset (area, byte, (unsigned int)length);
#endif
*/

  while (length) {
      *area = (char)byte;
      length--;
      area++;
  }

}/*end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133c_PresAddrCpy                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-08                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static    signed short int NEAR PASCAL ro133c_PresAddrCpy (
	     RO_pradd            FAR  * pNPrsAddr,
	     R_pradd             FAR  * pOPrsAddr)

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
signed short int rc=0;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
/* copy the prsentation Address */
   /* ---------- start coppying ---------------------------- */
   /* ------- copy only if present -------------------------- */
    if (pOPrsAddr->r_paddln)
	   pNPrsAddr->r_paddrf =  
		    (char FAR *)pOPrsAddr->r_paddrf;
    else 
	   pNPrsAddr->r_paddrf =  
		    (char FAR *)NULL;
		
    pNPrsAddr->r_paddln = pOPrsAddr->r_paddln;
return(rc);
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133d_SDataCpy                                     */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static    signed short int NEAR PASCAL ro133d_SDataCpy(
	    RO_snfrag        FAR  * pSnFrag,
	    R_sndata         FAR  * pSnData)

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

/* ********************* Local variable **************************** */
/* ********************                 **************************** */
signed short int rc =0; 
/* *************** check mode **************************************** */
     /* ---------- start coppying ---------------------------- */
     /* ------- copy only if present -------------------------- */
     if ( (pSnData->r_fraref) &&
	 (pSnData->r_fralen)   ){ 
          if (pSnData->r_more != R_FALSE) return(-R_HMOREE);
	  switch(pSnData->r_mtype) {
		case R_MEMORY:
		     pSnFrag->r_mtype =RO_MEMORY;
		     pSnFrag->r_fraary[0].r_sublen =
			     (unsigned long)pSnData->r_fralen;
		     break;

		case R_FILE:
		     pSnFrag->r_mtype =RO_FILE;
		     pSnFrag->r_fraary[0].r_sublen =0;
              /* for testing kc on 9-10-93 start */
              /*   {
                     int i=0;
                     for (i=0;i<5;i++){
                        pSnFrag->r_fraary[i].r_absid=3;
                        pSnFrag->r_fraary[i].r_sublen=1000;
                     }}*/ /* end of testing  change fracnt also 9-10 */
		     break;

#ifdef RP0000_MicV0600
		case R_GLOHAN:
		     pSnFrag->r_mtype = RO_GLOHAN;
		     pSnFrag->r_fraary[0].r_sublen =
			     (unsigned long)pSnData->r_fralen;
		     break;
#endif

		default :
		     pSnFrag->r_mtype = (int)pSnData->r_mtype;
                     pSnFrag->r_fraary[0].r_sublen = 0;
		     break;
	  }

	  pSnFrag->r_fraref = (char     FAR * ) pSnData->r_fraref;
	  pSnFrag->r_fralen = (unsigned long int       ) pSnData->r_fralen;
	  pSnFrag->r_fraary[0].r_absid = (signed   int )   pSnData->r_absid;
	  pSnFrag->r_fracnt            = (unsigned long) 1;
     }
     else {     /* no sndata */
	  pSnFrag->r_fraref = (char FAR *) NULL;
	  pSnFrag->r_fralen = (unsigned long)NULL;

     } 
 return(rc);

} /* end */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133e_EvDataCpy                                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static    signed short int NEAR PASCAL ro133e_EvDataCpy (

	    RO_buffer        FAR  * pNBuffer,
	    R_evdata         FAR  * pOEvData)

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


/* ---------- start coppying ---------------------------- */
/* ------- copy only if present -------------------------- */
    if ( (pOEvData->r_bufref) &&
	 (pOEvData->r_buflen)   ){
	 pNBuffer->r_mtype =
		    (int)pOEvData->r_mtype;

	 pNBuffer->r_bufref =
		    (char FAR *)pOEvData->r_bufref;

	 pNBuffer->r_buflen =
		     (unsigned long)pOEvData->r_buflen;

    }
    else {   /* no evdata */ 
	     pNBuffer->r_bufref = (char FAR *) NULL;
	     pNBuffer->r_buflen = (unsigned long)NULL;

     }
    return (OK);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133f_OpValuCpy                                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static    signed short int NEAR PASCAL ro133f_OpValuCpy(
	    RO_opval       FAR    * pNOpValue,
	    R_opval        FAR    * pOOpValue)

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
/* ********************* Local variable **************************** */
/* ********************                 **************************** */
signed short int rc =0; 


/* ---------- start coppying ---------------------------- */
/* ------- copy only if present -------------------------- */
    if ( (pOOpValue->r_oploc )) {    /*  local operation type */
	     *(pNOpValue->r_oploc) = (long) *(pOOpValue->r_oploc);
	     pNOpValue->r_opglb = (unsigned long *)NULL;
    }

    else {    /* Global value (objid ) */
	 pNOpValue->r_oploc = (long *)NULL;
	 ro133a_Objcpy(
		   (unsigned long int FAR *)pNOpValue->r_opglb,
		   (R_unsign          FAR *)pOOpValue->r_opglb.r_obidrf,
					    pOOpValue->r_opglb.r_obidnb);
    }
 return(rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133g_SnDataCpy                                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static    signed short int NEAR PASCAL ro133g_SnDataCpy (
	    RO_sndata         FAR * pNSnData,
	    R_sndata          FAR * pOSnData)

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

/* ********************* Local variable **************************** */
/* ********************                 **************************** */
signed short int rc =0; 
/* *************** check mode **************************************** */

/* copy R_sndata -> RO_sndata */
     /* ---------- start coppying ---------------------------- */
     /* ------- copy only if present -------------------------- */
     if ( (pOSnData->r_fraref) &&
	 (pOSnData->r_fralen)   ){ 
          if(pOSnData->r_more != R_FALSE) return (-R_HMOREE);
	  pNSnData->r_mtype =
		   (int)pOSnData->r_mtype;
	  pNSnData->r_fraref =
		   (char FAR *)pOSnData->r_fraref;
	  pNSnData->r_fralen =
			     (unsigned long)pOSnData->r_fralen;
     }
     else {   /* no sndata */
	  pNSnData->r_fraref = (char FAR *) NULL;
	  pNSnData->r_fralen = (unsigned long)NULL;
     } 
 return(rc);

} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133i_retrieveEData                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-11                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static    signed short int NEAR PASCAL ro133i_retrieveEData(
	    R_evtpb               FAR * pEvtPb  ,
	    char           FAR *  FAR * pUnUsedBuf,
	    signed long int       FAR * pUnUsedLen,
	    RO_evfrag             FAR * pEvFrag,
	    R_evdata              FAR * pEvData)

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
signed short int rc = OK;
/* *************** check mode **************************************** */
  /* copy RO_evfrag -> R_evdata */
    pEvData->r_specfl = (R_bool)pEvFrag->r_specfl;
    pEvData->r_datlen = (R_unsign)pEvFrag->r_fraary[0].r_sublen;
    pEvData->r_absid = (R_int)pEvFrag->r_fraary[0].r_absid;
    /* kc on 14-9-93 */
    if ( pEvFrag->r_bufref ){
	pEvData->r_absid = (R_int)pEvFrag->r_fraary[0].r_absid; 
	pEvData->r_buflen = (R_unsign)pEvFrag->r_buflen;
        pEvData->r_datlen = (R_unsign)pEvFrag->r_fraary[0].r_sublen; 
	switch (pEvFrag->r_mtype){
	     case RO_MEMORY:
	       pEvData->r_mtype = R_MEMORY;
	       pEvData->r_bufref= (char FAR *) pEvFrag->r_bufref;
	       break;
	     case RO_FILE:
	       pEvData->r_mtype = R_FILE;
	       pEvData->r_bufref = (char FAR *)
		    ro133k_setParameter (
			  (char FAR *)pEvtPb                 ,
			  (char FAR * FAR *)pUnUsedBuf       ,
			  (signed long int FAR *)pUnUsedLen  ,
			  (char FAR * )pEvFrag->r_bufref     ,
			  pEvFrag->r_buflen                  ,
			  (signed short int)4                );
	       break;
	     default:
	       pEvData->r_mtype = pEvFrag->r_mtype;
	       pEvData->r_bufref= (char FAR *) 
		    ro133k_setParameter( (char FAR *)pEvtPb,
				     pUnUsedBuf,
				     pUnUsedLen,
				     (char FAR * )pEvFrag->r_bufref,
				     sizeof (unsigned long)      , 
				     4                          );
	       break;
	}
	       
   }
   else {
/*	pEvData->r_absid = (R_int)NULL; */
	pEvData->r_buflen = (R_unsign)NULL;
/*	pEvData->r_datlen = (R_unsign)NULL; */
	pEvData->r_bufref = (char FAR *)NULL;
	pEvData->r_mtype  = NULL; 
    }
  

return (rc);
} /* end */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133j_retrieveEvData                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-11                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static    signed short int NEAR PASCAL ro133j_retrieveEvData(
	    R_evtpb                 FAR * pEvtPb  ,
	    char             FAR *  FAR * pUnUsedBuf,
	    signed long int         FAR * pUnUsedLen,
	    RO_evdata               FAR * pNEvData,
	    R_evdata                FAR * pOEvData)

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
signed short int rc;
/* *************** check mode **************************************** */
 /* copy RO_evdata -> R_evdata */
    pOEvData->r_specfl = (R_bool)pNEvData->r_specfl;
    pOEvData->r_absid  = (R_int)pNEvData->r_absid;
    pOEvData->r_buflen = (R_unsign)pNEvData->r_buflen;
    pOEvData->r_datlen = (R_unsign)pNEvData->r_datlen;
    switch(pNEvData->r_mtype){
      case RO_MEMORY:
	  pOEvData->r_mtype  = R_MEMORY;
	  pOEvData->r_bufref = (char FAR *)pNEvData->r_bufref;
	  break;
      case RO_FILE:
	  pOEvData->r_mtype  = R_FILE;
	  pOEvData->r_bufref = (char FAR *)
	       ro133k_setParameter (
		     (char FAR *)pEvtPb                   ,
		     (char FAR * FAR *)pUnUsedBuf         ,
		     (signed long int FAR *)pUnUsedLen    ,
		     (char FAR * )pNEvData->r_bufref      ,
		     pNEvData->r_buflen                   ,
		     (signed short int)4                  );
	  break;
      default:
	  pOEvData->r_mtype  = pNEvData->r_mtype;
	  pOEvData->r_bufref = (char FAR *)
		    ro133k_setParameter( (char FAR *)pEvtPb,
				     pUnUsedBuf,
				     pUnUsedLen,
				     (char FAR * )pNEvData->r_bufref,
				     sizeof(unsigned long )      ,
				     4                          );
	  break;
     }
	 


return (rc);
} /* end */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133l_retrieveObjID                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


static    signed short int NEAR PASCAL ro133l_retrieveObjID(
		R_evtpb                 FAR * pEvtPb  ,
		char              FAR * FAR *pUnUsedBuf,
		signed long int         FAR *pUnUsedLen,
		unsigned long int       FAR *pNObjId, 
		R_unsign          FAR * FAR *pOObjId, 
		R_unsign                FAR *pLength)



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
/*
R_unsign                usLoc;
*/
unsigned long  int      ulLen;
R_unsign           FAR *pLoc;
unsigned long int       i;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
   /* kc on 9-9-93 */       
   if ( pNObjId == NULL ){  
    *pLength = NULL;        
    return (R_LENULL);      
   }                        
   ulLen =  (unsigned long)*pNObjId++;
   *pLength = (R_unsign)ulLen;
   if(ulLen == 0 ) return (R_LENULL);
   else {
	if ((ulLen > RCro00_MaxObjIdComp) || (ulLen < 3 )){
	     *pLength     =  0;         
	 return (-R_MAXOBJ);
	}
	else {
/* 	 usLoc = (R_unsign) *pNObjId++; kc on 14-9-93 */
       /* kc on 14-9-93 */
	 *pOObjId = (R_unsign FAR *) 
	     ro133k_setParameter (
		   (char FAR *)pEvtPb                    ,
		   (char FAR * FAR *)pUnUsedBuf          ,
		   (signed long int FAR *)pUnUsedLen     ,
		   (char FAR * )pNObjId                  ,
		   (unsigned long int)(sizeof(R_unsign) * ulLen) ,
		   (signed short int)4                   ); 
	  pLoc = *pOObjId; 
	  for (i=0;i<ulLen;i++)
	      *(pLoc+i)  = (R_unsign) *pNObjId++;
	      
	}
	return (0);
  }

}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133m_retrieveOpValue                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


static    signed short int NEAR PASCAL ro133m_retrieveOpValue(
	    R_evtpb                 FAR * pEvtPb,
	    char             FAR *  FAR * pUnUsedBuf,
	    signed long int         FAR * pUnUsedLen,
	    RO_opval                FAR * pNOpValue,
	    R_opval                 FAR * pOOpValue)



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
unsigned long  int ulOpVal=NULL; /* kc on 17-9-93 */ 
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
/* ---------- start coppying ---------------------------- */
/* ------- copy only if present -------------------------- */
    if ( (pNOpValue->r_oploc )) {    /*  local operation type */
        ulOpVal = * pNOpValue->r_oploc;
	 pOOpValue->r_oploc = (R_int FAR *)
	     ro133k_setParameter (
		   (char FAR *)pEvtPb                    ,
		   (char FAR * FAR *)pUnUsedBuf          ,
		   (signed long int FAR *)pUnUsedLen     ,
		   (char FAR * )pNOpValue->r_oploc       ,
		   (unsigned long int)(sizeof(int))      ,
		   (signed short int)4                   );
/* kc on 17-9-93 */                                        
   *pOOpValue->r_oploc = (R_int) ulOpVal;                  
     pOOpValue->r_opglb.r_obidnb = NULL;                   
     pOOpValue->r_opglb.r_obidrf = (R_unsign FAR *)NULL;   
    }
    else {    /* Global value (objid ) */
	 pOOpValue->r_oploc = (R_int *)NULL;

	 rc = ro133l_retrieveObjID(
	       pEvtPb,
	      (char FAR * FAR *)pUnUsedBuf,
	      (signed long int FAR *)pUnUsedLen,
	      (unsigned long int FAR *)pNOpValue->r_opglb,
	      (R_unsign FAR * FAR *)&(pOOpValue->r_opglb.r_obidrf),
	      (R_unsign FAR *)&(pOOpValue->r_opglb.r_obidnb));

  }

	return (0);
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro133n_getROSEAbsID                                 */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-30                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static       int  NEAR PASCAL ro133n_getROSEAbsID (
unsigned long  int  FAR   *pulMyScbHd,
signed   long  int         lTaskId,
FARPROC                    tpduFct)

/*                                                                    */
/* DESCRIPTION  :  usees ro_infrm function to get the ROSE AbsId      */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              : None                                  */
/*                                                                    */
/* OUT-PARAMETERS             : None                                  */
/*                                                                    */
/* RETURN VALUES              :   AbsID                               */
/*                                >= 0  Success                       */
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
signed   short int      rc = -1;
rT04z1_stAbsNameId      stAbsId;
RO_excopb               unExcopb;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
 unExcopb.r_hdinfo.r_cont   = R_FALSE;
 unExcopb.r_hdinfo.r_fnctnb = RO_INFRM;
 unExcopb.r_hdinfo.r_versnb = RO_VERSION;
 unExcopb.r_inform.r_inftyp = RC04zQ_EXIFAbId;     
 unExcopb.r_inform.r_infval = (void FAR *) &stAbsId;

 /* --------- Use the IROS 3.00 call ----------------------- */
 rc = ro_infrm_taskId (pulMyScbHd, lTaskId, tpduFct, &unExcopb.r_inform);
 if (!rc)   return (stAbsId.sAbsId);
 else return -1;
}/*end*/                                                                        


/* EOF */
