/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif20.c,v $
 * Revision 1.1.4.2  1996/03/11  13:21:59  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:45  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:33  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:07  keutel
 * 	OT 11620
 * 	[1994/08/19  13:50:18  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:09:48  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:51:58  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:12  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:01:32  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:46  keutel
 * 	creation
 * 	[1994/03/21  13:30:33  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roexif20.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:59 $" } ;
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
/* NAME 	: roexif20.c      				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-07-23					      */
/*                                                                    */
/* COMPONENT    : ROS external interface  IROS 2.50                   */
/*                                                                    */
/* PRD#/VERS.	: ROS-V3.00					      */
/*                                                                    */
/* DESCRIPTION	: This modul contains the entry points to ROS-V2.5.   */
/*	and converts into the IROS 3.00 data structures               */ 
/*                                                                    */
/* SYSTEM DEPENDENCE :	none					      */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/*  3.00 K0 | 93-07-23|  original		       | kc |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*		Module Identification				      */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <ros.h>	 /* ROS external interface    IROS V2.5.     */
#include <roerval.h>     /* ROS error values.                        */

#include <rogenrl.h>     /* ROS general definitions.                 */
#include <tpdu.h>        /* TPDU interface definitions               */
#include <roexif02.h>    /* the extern file for sys dependent funs   */
                         /* interface between roexif00.c to          */
                         /*  roexifx2.c                              */

#include <roexif2.h>     /* TPDU interface definitions		     */
#include <rodpat2.h>     /* Dispatcher interface prototypes.         */


/*********************************************************************/
/*                                                                   */
/*		M A C R O S					     */
/*                                                                   */
/*********************************************************************/

/* ****************** Macro which defines all single entry ********** */
/* ****************** functions                            ********** */

#define RM1331_ENTRY(name, pblk, fnctnb)                              \
      R_int FAR PASCAL name ( pblk FAR *r_parblk)            \
      {                                                               \
	      r_parblk->r_hdinfo.r_fnctnb = fnctnb;		      \
/*     r_parblk->r_cont		  = R_FALSE;		*/    \
      return (ro133z_roexif ((R_excopb FAR *) r_parblk));             \
      }

#define RM1332_ENTRYPROTO(name, pblk)    			     \
								     \
	R_int FAR PASCAL name PROTO ((pblk FAR *));


static R_int	NEAR PASCAL  ro133z_roexif PROTO ((R_excopb FAR	*r_parlst));

/*
       R_int    NEAR PASCAL  ro1330_roexcom (r_parlst)  
       R_excopb FAR         *r_parlst;                  
*/

R_int FAR PASCAL ro1330_roexcom PROTO ((
                                        unsigned long  int  FAR   *pMyScbHd,
                                        signed	 long  int         lTaskId,
                                        FARPROC                    tpduFct,
                                        R_excopb            FAR   *r_parlst));



/*********************************************************************/
/*                                                                   */
/*              L O C A L  V A R I A B L E S                         */
/*                                                                   */
/*********************************************************************/

/* ****************** Busy flag and system control block ************ */
/* ****************** handle.				 ************ */

static signed long  int  lMyScbHd = 0;
static signed short int  sBusyFlg = 0;

/* ****************** Current task number and tpdu call ************* */
/* ****************** back function pointer.		************* */

static signed long  int  lTaskId  = 0;
static FARPROC       	 tpduFct = 0;


#ifdef RP0000_MicV0600


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0199_tpduCallBack				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-02-23					      */
/*                                                                    */
/* SYNTAX       :                                                     */

int FAR PASCAL EXPORT ro0199_tpduCallBack (

       signed short	  int fctNo,
       rT0100_unCallBack  FAR  *pblk)

/*                                                                    */
/* DESCRIPTION	: 					              */
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

TP_short rc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

switch (fctNo)	  {
case RC0101_Open    : rc = tp2_001_open     (&(pblk->stOpen    )); break; 
case RC0102_Read    : rc = tp2_002_read     (&(pblk->stRead    )); break; 
case RC0103_Write   : rc = tp2_003_write    (&(pblk->stWrite   )); break; 
case RC0104_Position: rc = tp2_004_position (&(pblk->stPosition)); break; 
case RC0105_Close   : rc = tp2_005_close    (&(pblk->stClose   )); break; 
case RC0106_Delete  : rc = tp2_006_delete   (&(pblk->stDelete  )); break; 
default             : rc = TP_CALL_ERR;                            break; 

}

return (rc);
}/*end*/

#endif /* RP0000_MicV0600 */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro133z_roexif                  		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-03-02					      */
/*                                                                    */
/* SYNTAX       :                                                     */

static R_int	NEAR PASCAL  ro133z_roexif  (
       R_excopb FAR	    *r_parlst)

/*                                                                    */
/* DESCRIPTION	: Used as a driver between IROS v2.5 to IROS V3.00    */
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

/* ****************** Compute task/process number ******************* */
/* ******************				  ******************* */

if (! lTaskId) {
      if (! (lTaskId = ro1320_taskId ()) )	 {
	    r_parlst->r_hdinfo.r_errval = R_PROCID;
	    r_parlst->r_hdinfo.r_erclas = R_SYSERR;
	    r_parlst->r_hdinfo.r_retcod = R_PRMERR;

	    return (r_parlst->r_hdinfo.r_retcod);
      }
}
#ifndef RP0000_MicV0600
else { /* TaskId(pid) present . Now check for the Real Pid & this taskId */   
       /* for the check of CHILD PROCESS !!! by kc on 16-11-93 */             
     if ( lTaskId != ro1320_taskId() ){ /* Yes CHILD PROCESS clear the dpat */ 
      if (! (lTaskId = ro1320_taskId ()) )	 {
	    r_parlst->r_hdinfo.r_errval = R_PROCID;
	    r_parlst->r_hdinfo.r_erclas = R_SYSERR;
	    r_parlst->r_hdinfo.r_retcod = R_PRMERR;

	    return (r_parlst->r_hdinfo.r_retcod);
      }
         ro0275_ClearAll();            
    }                                       
}
#endif                                                                             

/* ****************** Compute tpdu call back function *************** */
/* ****************** pointer.			      *************** */

if ( (!				    tpduFct  ) &&
     (r_parlst->r_hdinfo.r_fnctnb == R_ATACH) ) {

/* Type cast added for GNU GCC */
      tpduFct = ro1321_ssiMpi (
			(FARPROC)ro0199_tpduCallBack, r_parlst->r_attapb.r_pid);

      if (! tpduFct) {
	    r_parlst->r_hdinfo.r_errval = R_TPBFCT;
	    r_parlst->r_hdinfo.r_erclas = R_SYSERR;
	    r_parlst->r_hdinfo.r_retcod = R_PRMERR;

	    return (r_parlst->r_hdinfo.r_retcod);
      }
}

/* ****************** Check busy flag and call  *************** */
/* ****************** execute communication function. *************** */

if ((sBusyFlg) &&
    (r_parlst->r_hdinfo.r_fnctnb != R_LOGON ) &&
    (r_parlst->r_hdinfo.r_fnctnb != R_LOGOF ) &&
    (r_parlst->r_hdinfo.r_fnctnb != R_LOGWR ) ) {
       r_parlst->r_hdinfo.r_errval = R_HRBUSY;
       r_parlst->r_hdinfo.r_erclas = R_ILSTAT;
       r_parlst->r_hdinfo.r_retcod = R_TMPERR;
} else {
       sBusyFlg = 1;
       ro1330_roexcom ((unsigned long int FAR *)&lMyScbHd, lTaskId, tpduFct,
                       r_parlst);
       sBusyFlg = 0;
}

return (r_parlst->r_hdinfo.r_retcod);


}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTIONS                                         */
/*                                                                    */
/* NAME         : ROS external interface functions                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-03-10					      */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/*      R_int   r_xxxxx (r_parblk)                                    */
/*      R_xxxxpb        *r_parblk;                                    */
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

/* ****************** R_excom () entry ****************************** */
/* ******************                  ****************************** */

R_int	  FAR PASCAL  r_excom (
R_excopb  FAR	     *r_parlst)
{
 /* -------------- Local variables --------------------------------- */

 R_int  rc;
 R_bool bCont=R_FALSE;
 r_parlst--;
 do {
    r_parlst++;

    switch(r_parlst->r_hdinfo.r_fnctnb){

     case R_ATACH : 
                    bCont = r_parlst->r_attapb.r_cont;
                    r_parlst->r_attapb.r_cont = R_FALSE;
		    break;

     case R_DTACH : 
                    bCont = r_parlst->r_detapb.r_cont;
                    r_parlst->r_detapb.r_cont = R_FALSE;
		    break;
     case R_PRBUF :
                    bCont = r_parlst->r_pvbfpb.r_cont;
                    r_parlst->r_pvbfpb.r_cont = R_FALSE;
		    break;

     case R_REDRQ : 
                    bCont = r_parlst->r_rdrqpb.r_cont;
                    r_parlst->r_rdrqpb.r_cont = R_FALSE;
                    break;

     case R_WAKE  : 
                    bCont = r_parlst->r_wakepb.r_cont;
                    r_parlst->r_wakepb.r_cont = R_FALSE;
		    break;

     case R_GTDCS : 
                    bCont = r_parlst->r_getdpb.r_cont;
                    r_parlst->r_getdpb.r_cont = R_FALSE;
		    break;

     case R_GTPBF : 
                    bCont = r_parlst->r_getdpb.r_cont;
                    r_parlst->r_getdpb.r_cont = R_FALSE;
		    break;
     /* ************* Execute binding, unbinding and abort ********** */
     /* *************					   ********** */

     case R_BINRQ : 
                    bCont = r_parlst->r_bnrqpb.r_cont;
                    r_parlst->r_bnrqpb.r_cont = R_FALSE;
                    break;
     case R_BINRS : 
                    bCont = r_parlst->r_bnrspb.r_cont;
                    r_parlst->r_bnrspb.r_cont = R_FALSE;
                    break;
     case R_UBNRQ : 
                    bCont = r_parlst->r_ubrqpb.r_cont;
                    r_parlst->r_ubrqpb.r_cont = R_FALSE;
                    break;
     case R_UBNRS : 
                    bCont = r_parlst->r_ubrspb.r_cont;
                    r_parlst->r_ubrspb.r_cont = R_FALSE;
                    break;
     case R_ABORQ : 
                    bCont = r_parlst->r_abrqpb.r_cont;
                    r_parlst->r_abrqpb.r_cont = R_FALSE;
		    break;
     /* ************* Execute remote operations ********************* */
     /* *************				********************* */
     case R_INVRQ : 
                    bCont = r_parlst->r_ivrqpb.r_cont;
                    r_parlst->r_ivrqpb.r_cont = R_FALSE;
                    break;
     case R_RESRQ : 
                    bCont = r_parlst->r_rsrqpb.r_cont;
                    r_parlst->r_rsrqpb.r_cont = R_FALSE;
                    break;
     case R_ERRRQ : 
                    bCont = r_parlst->r_errqpb.r_cont;
                    r_parlst->r_errqpb.r_cont = R_FALSE;
                    break;
     case R_RJURQ : 
                    bCont = r_parlst->r_rurqpb.r_cont;
                    r_parlst->r_rurqpb.r_cont = R_FALSE;
		    break;

     /* ************* Execute logging functions ********************* */
     /* *************				********************* */

     case R_LOGON : 
                    bCont = r_parlst->r_lgonpb.r_cont;
                    r_parlst->r_lgonpb.r_cont = R_FALSE;
                    break;
     case R_LOGOF : 
                    bCont = r_parlst->r_lgofpb.r_cont;
                    r_parlst->r_lgofpb.r_cont = R_FALSE;
                    break;
     case R_LOGWR : 
                    bCont = r_parlst->r_lgwrpb.r_cont;
                    r_parlst->r_lgwrpb.r_cont = R_FALSE;
		    break;

     /* ************* Execute not supported or ********************** */
     /* ************* unkown functions	       ********************** */

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
	     break;
     } /* end of switch */
     rc = ro133z_roexif(r_parlst);
  }while( (rc >= R_SUCES)&&
          (bCont !=R_FALSE)       );
 return (rc);
} /*end */


/* ****************** R_cwait () entry ****************************** */
/* ******************                  ****************************** */

R_int	 FAR PASCAL  r_cwait (
R_chwtpb FAR	    *r_parblk)
{
r_parblk->r_hdinfo.r_fnctnb = R_CWAIT;
return (ro133z_roexif ((R_excopb FAR *) r_parblk));
}


/*********************************************************************/
/*                                                                   */
/*		M A C R O S for function prototypes                  */
/*                                                                   */
/*********************************************************************/

/* ****************** Local functions        ************************ */

RM1332_ENTRYPROTO (r_atach, R_attapb)
RM1332_ENTRYPROTO (r_dtach, R_detapb)
RM1332_ENTRYPROTO (r_prbuf, R_pvbfpb)
RM1332_ENTRYPROTO (r_redrq, R_rdrqpb)
RM1332_ENTRYPROTO (r_wake,  R_wakepb)
RM1332_ENTRYPROTO (r_gtdcs, R_getdpb)
RM1332_ENTRYPROTO (r_gtpbf, R_getpbf)


RM1332_ENTRYPROTO (r_frasd, R_fasdpb)  /* Not supported	     */
RM1332_ENTRYPROTO (r_frarv, R_farvpb)  /* Not supported	     */

/* ****************** Association handling function entries ********* */
/* ******************                                       ********* */

RM1332_ENTRYPROTO (r_binrq, R_bnrqpb)
RM1332_ENTRYPROTO (r_binrs, R_bnrspb)
RM1332_ENTRYPROTO (r_ubnrq, R_ubrqpb)
RM1332_ENTRYPROTO (r_ubnrs, R_ubrspb)
RM1332_ENTRYPROTO (r_aborq, R_abrqpb)

RM1332_ENTRYPROTO (r_binin, R_bncnpb)  /* Not supported	     */
RM1332_ENTRYPROTO (r_bincn, R_bncnpb)  /* Not supported	     */
RM1332_ENTRYPROTO (r_ubnin, R_ubcnpb)  /* Not supported	     */
RM1332_ENTRYPROTO (r_ubncn, R_ubcnpb)  /* Not supported	     */

/* ****************** Remote operation functions entries ************ */
/* ******************                                    ************ */

RM1332_ENTRYPROTO (r_invrq, R_ivrqpb)
RM1332_ENTRYPROTO (r_resrq, R_rsrqpb)
RM1332_ENTRYPROTO (r_errrq, R_errqpb)
RM1332_ENTRYPROTO (r_rjurq, R_rurqpb)

RM1332_ENTRYPROTO (r_invin, R_ivinpb)  /* Not supported	     */
RM1332_ENTRYPROTO (r_resin, R_rsinpb)  /* Not supported	     */
RM1332_ENTRYPROTO (r_errin, R_erinpb)  /* Not supported	     */
RM1332_ENTRYPROTO (r_rjpin, R_rpinpb)  /* Not supported	     */

/* ****************** ROS logging functions ************************* */
/* ******************                       ************************* */

RM1332_ENTRYPROTO (r_logon, R_lgonpb)
RM1332_ENTRYPROTO (r_logof, R_lgofpb)
RM1332_ENTRYPROTO (r_logwr, R_lgwrpb)


/*********************************************************************/
/*                                                                   */
/*		Function definitions here !!                         */
/*                                                                   */
/*********************************************************************/


/* ****************** Local functions        ************************ */

RM1331_ENTRY (r_atach, R_attapb, R_ATACH)
RM1331_ENTRY (r_dtach, R_detapb, R_DTACH)
RM1331_ENTRY (r_prbuf, R_pvbfpb, R_PRBUF)
RM1331_ENTRY (r_redrq, R_rdrqpb, R_REDRQ)
RM1331_ENTRY (r_wake,  R_wakepb, R_WAKE )
RM1331_ENTRY (r_gtdcs, R_getdpb, R_GTDCS)
RM1331_ENTRY (r_gtpbf, R_getpbf, R_GTPBF)


RM1331_ENTRY (r_frasd, R_fasdpb, R_FRASD)  /* Not supported	     */
RM1331_ENTRY (r_frarv, R_farvpb, R_FRARV)  /* Not supported	     */

/* ****************** Association handling function entries ********* */
/* ******************                                       ********* */

RM1331_ENTRY (r_binrq, R_bnrqpb, R_BINRQ)
RM1331_ENTRY (r_binrs, R_bnrspb, R_BINRS)
RM1331_ENTRY (r_ubnrq, R_ubrqpb, R_UBNRQ)
RM1331_ENTRY (r_ubnrs, R_ubrspb, R_UBNRS)
RM1331_ENTRY (r_aborq, R_abrqpb, R_ABORQ)

RM1331_ENTRY (r_binin, R_bncnpb, R_BININ)  /* Not supported	     */
RM1331_ENTRY (r_bincn, R_bncnpb, R_BINCN)  /* Not supported	     */
RM1331_ENTRY (r_ubnin, R_ubcnpb, R_UBNIN)  /* Not supported	     */
RM1331_ENTRY (r_ubncn, R_ubcnpb, R_UBNCN)  /* Not supported	     */

/* ****************** Remote operation functions entries ************ */
/* ******************                                    ************ */

RM1331_ENTRY (r_invrq, R_ivrqpb, R_INVRQ)
RM1331_ENTRY (r_resrq, R_rsrqpb, R_RESRQ)
RM1331_ENTRY (r_errrq, R_errqpb, R_ERRRQ)
RM1331_ENTRY (r_rjurq, R_rurqpb, R_RJURQ)

RM1331_ENTRY (r_invin, R_ivinpb, R_INVIN)  /* Not supported	     */
RM1331_ENTRY (r_resin, R_rsinpb, R_RESIN)  /* Not supported	     */
RM1331_ENTRY (r_errin, R_erinpb, R_ERRIN)  /* Not supported	     */
RM1331_ENTRY (r_rjpin, R_rpinpb, R_RJPIN)  /* Not supported	     */

/* ****************** ROS logging functions ************************* */
/* ******************                       ************************* */

RM1331_ENTRY (r_logon, R_lgonpb, R_LOGON)
RM1331_ENTRY (r_logof, R_lgofpb, R_LOGOF)
RM1331_ENTRY (r_logwr, R_lgwrpb, R_LOGWR)

/* EOF */

