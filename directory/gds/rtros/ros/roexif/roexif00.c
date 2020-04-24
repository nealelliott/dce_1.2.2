/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif00.c,v $
 * Revision 1.1.4.2  1996/03/11  13:21:49  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:38:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:17  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:02  keutel
 * 	OT 11620
 * 	[1994/08/19  13:49:22  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:49  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:51:48  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:06  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:00:27  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:35  keutel
 * 	creation
 * 	[1994/03/21  13:29:33  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roexif00.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:49 $" } ;
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
/* NAME 	: roexif00.c					      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-02-23					      */
/*                                                                    */
/* COMPONENT    : ROS external interface                              */
/*                                                                    */
/* PRD#/VERS.	: ROS-V3.00					      */
/*                                                                    */
/* DESCRIPTION	: This modul contains the entry points to ROS-V3.0.   */
/*		  for IROS-3.00                                       */
/*                                                                    */
/* SYSTEM DEPENDENCE :	none					      */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/*  3.00 K0 | 93-02-23|  original		       | kc |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/





/**********************************************************************/
/*                                                                    */
/*		Module Identification				      */
/*                                                                    */
/**********************************************************************/

static char *sccsid = " ";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <ross.h>	 /* ROS external interface		     */
#include <roerval.h>	 /* ROS error value file		     */

#include <rogenrl.h>	 /* ROS general definitions		     */

#include <tpdu.h>        /* TPDU interface definitions               */
#include <roexif02.h>    /* the extern file for sys dependent funs   */
                         /* interface between roexif00.c to          */
                         /*  roexifx2.c                              */

#include <roexif2.h>     /* TPDU interface definitions		     */

#include <rodpat2.h>     /* for the PROTO typeing                    */

signed short int FAR PASCAL ro1310_roexcom PROTO ((
unsigned long  int  FAR   *pScbHd,
signed   long  int         lTaskId,
FARPROC                    tpduFct,
RO_excopb           FAR   *pParlst));

static int      NEAR PASCAL  ro1300_roexif PROTO ((
RO_excopb           FAR   *ro_parlst));

/*********************************************************************/
/*                                                                   */
/*		M A C R O S					     */
/*                                                                   */
/*********************************************************************/

/* ****************** Macro which defines all single entry ********** */
/* ****************** functions                            ********** */

#define RM1301_ENTRY(name, pblk, fnctnb)			     \
								     \
	int FAR PASCAL name (                                        \
              pblk FAR *ro_parblk)	                             \
	      { 						     \
	      ro_parblk->r_hdinfo.r_fnctnb = fnctnb;		     \
	      return (ro1300_roexif ((RO_excopb FAR *) ro_parblk));  \
	      }

#define RM1302_ENTRYPROTO(name, pblk)    			     \
								     \
	int FAR PASCAL name PROTO ((pblk FAR *));


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


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1300_roexif 				      */
/*                                                                    */
/* AUTHOR       : chandrasekar.K.                                     */
/* DATE 	: 93-02-23					      */
/*                                                                    */
/* SYNTAX       :                                                     */

static int	NEAR PASCAL  ro1300_roexif (
RO_excopb FAR	            *ro_parlst)

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

{/*entr*/
/* ****************** Compute task/process number ******************* */
/* ******************				  ******************* */
if (! lTaskId) {
      if (! (lTaskId = ro1320_taskId ()) )	 {
	    ro_parlst->r_hdinfo.r_errval = R_PROCID;
	    ro_parlst->r_hdinfo.r_erclas = RO_SYSERR;
	    ro_parlst->r_hdinfo.r_retcod = RO_PRMERR;

	    return (ro_parlst->r_hdinfo.r_retcod);
      }
}
#ifndef RP0000_MicV0600
else { /* TaskId(pid) present . Now check for the Real Pid & this taskId */
        /* for the check of CHILD PROCESS !!! by kc on 16-11-93 */
      if ( lTaskId != ro1320_taskId() ) {
                     /* Yes CHILD PROCESS clear the dpat */
      if (! (lTaskId = ro1320_taskId ()) )	 {
	    ro_parlst->r_hdinfo.r_errval = R_PROCID;
	    ro_parlst->r_hdinfo.r_erclas = RO_SYSERR;
	    ro_parlst->r_hdinfo.r_retcod = RO_PRMERR;

	    return (ro_parlst->r_hdinfo.r_retcod);
      }
          ro0275_ClearAll();
     }
 }
#endif

/* ****************** Compute tpdu call back function *************** */
/* ****************** pointer.			      *************** */

if ( (!				    tpduFct  ) &&
     (ro_parlst->r_hdinfo.r_fnctnb == RO_ATACH) ) {

      tpduFct = ro1321_ssiMpi (
		    (FARPROC)ro0199_tpduCallBack, ro_parlst->r_attapb.r_pid);

      if (! tpduFct) {
	    ro_parlst->r_hdinfo.r_errval = R_TPBFCT;
	    ro_parlst->r_hdinfo.r_erclas = RO_SYSERR;
	    ro_parlst->r_hdinfo.r_retcod = RO_PRMERR;

	    return (ro_parlst->r_hdinfo.r_retcod);
      }
}

/* ****************** Check busy flag and call  *************** */
/* ****************** execute communication function. *************** */

if ((sBusyFlg) &&
    (ro_parlst->r_hdinfo.r_fnctnb != RO_LOGON ) &&
    (ro_parlst->r_hdinfo.r_fnctnb != RO_LOGOF ) &&
    (ro_parlst->r_hdinfo.r_fnctnb != RO_LOGWR ) ) {
       ro_parlst->r_hdinfo.r_errval = R_HRBUSY;
       ro_parlst->r_hdinfo.r_erclas = RO_ILSTAT;
       ro_parlst->r_hdinfo.r_retcod = RO_TMPERR;
} else {
       sBusyFlg = 1;
       ro1310_roexcom ((unsigned long  int  FAR   *)&lMyScbHd,
                        lTaskId, tpduFct, ro_parlst);
       sBusyFlg = 0;
}

return (ro_parlst->r_hdinfo.r_retcod);
}/*end*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTIONS                                         */
/*                                                                    */
/* NAME         : ROS external interface functions                    */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 90-12-31					      */
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


/* ****************** RO_excom () entry ****************************** */
/* ******************                  ****************************** */

int	  FAR PASCAL  ro_excom (
RO_excopb  FAR	     *ro_parlst)
{
return (ro1300_roexif (ro_parlst));
}

/* ****************** RO_cwait () entry ****************************** */
/* ******************                  ****************************** */

int	 FAR PASCAL  ro_cwait (
RO_chwtpb FAR	    *ro_parblk)
{
ro_parblk->r_hdinfo.r_fnctnb = RO_CWAIT;
return (ro1300_roexif ((RO_excopb FAR *) ro_parblk));
}

/* ****************** Local function entries ************************ */
/* ******************                        ************************ */

RM1301_ENTRY (ro_atach, RO_attapb, RO_ATACH)
RM1301_ENTRY (ro_dtach, RO_detapb, RO_DTACH)
RM1301_ENTRY (ro_prbuf, RO_pvbfpb, RO_PRBUF)
RM1301_ENTRY (ro_redrq, RO_rdrqpb, RO_REDRQ)
RM1301_ENTRY (ro_wake,  RO_wakepb, RO_WAKE )
RM1301_ENTRY (ro_infrm, RO_inform, RO_INFRM)


/* ****************** Association handling function entries ********* */
/* ******************                                       ********* */

RM1301_ENTRY (ro_binrq, RO_bnrqpb, RO_BINRQ)
RM1301_ENTRY (ro_binrs, RO_bnrspb, RO_BINRS)
RM1301_ENTRY (ro_ubnrq, RO_ubrqpb, RO_UBNRQ)
RM1301_ENTRY (ro_ubnrs, RO_ubrspb, RO_UBNRS)
RM1301_ENTRY (ro_aborq, RO_abrqpb, RO_ABORQ)


/* ****************** Remote operation functions entries ************ */
/* ******************                                    ************ */

RM1301_ENTRY (ro_invrq, RO_ivrqpb, RO_INVRQ)
RM1301_ENTRY (ro_resrq, RO_rsrqpb, RO_RESRQ)
RM1301_ENTRY (ro_errrq, RO_errqpb, RO_ERRRQ)
RM1301_ENTRY (ro_rjurq, RO_rurqpb, RO_RJURQ)


/* ****************** ROS logging functions ************************* */
/* ******************                       ************************* */

RM1301_ENTRY (ro_logon, RO_lgonpb, RO_LOGON)
RM1301_ENTRY (ro_logof, RO_lgofpb, RO_LOGOF)
RM1301_ENTRY (ro_logwr, RO_lgwrpb, RO_LOGWR)

/*EOF */

