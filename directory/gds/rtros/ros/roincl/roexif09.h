/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif09.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:18  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:03  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:13  keutel
 * 	OT 11620
 * 	[1994/08/19  13:51:19  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:10:24  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:19  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:29  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:10  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:53  keutel
 * 	creation
 * 	[1994/03/21  13:32:38  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROEXIF09_H
#define _ROEXIF09_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roexif09_rcsid[] = { "@(#) $RCSfile: roexif09.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:18 $" } ;
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
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : roexif09.h                                          */
/*                                                                    */
/* AUTHOR       : Shivkumar, H.                                       */
/* DATE         : 93-07-29                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Jugglery for driver directly accessing the          */
/*                roexcom fuction of IROS v3.0. (bypass roexif00.c) !!*/
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roexif09.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-07-29|  original                      |skh |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/*********************************************************************/
/*                                                                   */
/*		M A C R O S for redefining the  ro_xxxx  functions   */
/*                                                                   */
/*********************************************************************/

/* ****************** Local function entries ************************ */
/* ******************                        ************************ */

/*
#define ro_atach(pblk)   ro_atach1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_dtach(pblk)   ro_dtach1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_prbuf(pblk)   ro_prbuf1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_redrq(pblk)   ro_redrq1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_wake(pblk)    ro_wake1  (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_infrm(pblk)   ro_infrm1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_cwait(pblk)   ro_cwait1 (pulMyScbHd, lTaskId, tpduFct, pblk)
*/

/* ****************** Association handling function entries ********* */
/* ******************                                       ********* */

/*
#define ro_binrq(pblk)   ro_binrq1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_binrs(pblk)   ro_binrs1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_ubnrq(pblk)   ro_ubnrq1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_ubnrs(pblk)   ro_ubnrs1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_aborq(pblk)   ro_aborq1 (pulMyScbHd, lTaskId, tpduFct, pblk)
*/

/* ****************** Remote operation functions entries ************ */
/* ******************                                    ************ */

/*
#define ro_invrq(pblk)   ro_invrq1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_resrq(pblk)   ro_resrq1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_errrq(pblk)   ro_errrq1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_rjurq(pblk)   ro_rjurq1 (pulMyScbHd, lTaskId, tpduFct, pblk)
*/

/* ****************** ROS logging functions ************************* */
/* ******************                       ************************* */

/*
#define ro_logon(pblk)   ro_logon1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_logof(pblk)   ro_logof1 (pulMyScbHd, lTaskId, tpduFct, pblk)
#define ro_logwr(pblk)   ro_logwr1 (pulMyScbHd, lTaskId, tpduFct, pblk)
*/

signed short int FAR PASCAL ro1310_roexcom PROTO ((
                                unsigned  long  int  FAR   *pScbHd,
                                signed	  long  int         lTaskId,
                                FARPROC       	            tpduFct,
                                RO_excopb            FAR   *pParlst));

/*********************************************************************/
/*                                                                   */
/*		M A C R O S for defining two general Macros          */
/*                                                                   */
/*********************************************************************/

/* ****************** Macro which defines all single entry ********** */
/* ****************** functions                            ********** */

#define RM1301_ENTRY(name, pblk, fnctnb)			       \
								       \
        int FAR PASCAL name (pulMyScbHd, lTaskId, tpduFct, ro_parblk)  \
	      unsigned long  int  FAR   *pulMyScbHd;                   \
	      signed   long  int         lTaskId;                      \
	      FARPROC                    tpduFct;                      \
              pblk                FAR   *ro_parblk;	               \
	      { 						       \
	      ro_parblk->r_hdinfo.r_fnctnb = fnctnb;		       \
	      ro_parblk->r_hdinfo.r_cont   = FALSE;		       \
	      return (ro1310_roexcom (pulMyScbHd, lTaskId, tpduFct,    \
                                      (RO_excopb FAR *) ro_parblk));   \
	      }

#define RM1302_ENTRYPROTO(name, pblk)    			       \
								       \
	int FAR PASCAL name PROTO ((unsigned long  int FAR  *,         \
                                    signed   long  int       ,         \
                                    FARPROC                  ,         \
                                    pblk               FAR  *));

/*********************************************************************/
/*                                                                   */
/*		Actual function prototypes and definitions here !!   */
/*                                                                   */
/*********************************************************************/

/* ****************** Local function protos  ************************ */
/* ******************                        ************************ */

RM1302_ENTRYPROTO (ro_atach1_taskId, RO_attapb)
RM1302_ENTRYPROTO (ro_atach_taskId, RO_attapb)
RM1302_ENTRYPROTO (ro_dtach_taskId, RO_detapb)
RM1302_ENTRYPROTO (ro_prbuf_taskId, RO_pvbfpb)
RM1302_ENTRYPROTO (ro_redrq_taskId, RO_rdrqpb)
RM1302_ENTRYPROTO (ro_wake_taskId,  RO_wakepb)
RM1302_ENTRYPROTO (ro_infrm_taskId, RO_inform)
RM1302_ENTRYPROTO (ro_cwait_taskId, RO_chwtpb)


/* ****************** Association handling function protos  ********* */
/* ******************                                       ********* */

RM1302_ENTRYPROTO (ro_binrq_taskId, RO_bnrqpb)
RM1302_ENTRYPROTO (ro_binrs_taskId, RO_bnrspb)
RM1302_ENTRYPROTO (ro_ubnrq_taskId, RO_ubrqpb)
RM1302_ENTRYPROTO (ro_ubnrs_taskId, RO_ubrspb)
RM1302_ENTRYPROTO (ro_aborq_taskId, RO_abrqpb)


/* ****************** Remote operation functions protos  ************ */
/* ******************                                    ************ */

RM1302_ENTRYPROTO (ro_invrq_taskId, RO_ivrqpb)
RM1302_ENTRYPROTO (ro_resrq_taskId, RO_rsrqpb)
RM1302_ENTRYPROTO (ro_errrq_taskId, RO_errqpb)
RM1302_ENTRYPROTO (ro_rjurq_taskId, RO_rurqpb)


/* ****************** ROS logging functions protos ****************** */
/* ******************                              ****************** */

RM1302_ENTRYPROTO (ro_logon_taskId, RO_lgonpb)
RM1302_ENTRYPROTO (ro_logof_taskId, RO_lgofpb)
RM1302_ENTRYPROTO (ro_logwr_taskId, RO_lgwrpb)


/* ****************** RO_cwait () entry ****************************** */
/* ******************                  ****************************** */

int      FAR PASCAL  ro_cwait_taskId (pulMyScbHd, lTaskId, tpduFct, ro_parblk)
unsigned long  int  FAR   *pulMyScbHd;
signed   long  int         lTaskId;
FARPROC                    tpduFct;
RO_chwtpb           FAR	  *ro_parblk;
{
ro_parblk->r_hdinfo.r_fnctnb = RO_CWAIT;
return (ro1310_roexcom (pulMyScbHd, lTaskId, tpduFct,
                        (RO_excopb FAR *) ro_parblk));
}

/* ****************** Local function entries ************************ */
/* ******************                        ************************ */

RM1301_ENTRY (ro_atach1_taskId, RO_attapb, -RO_ATACH)
RM1301_ENTRY (ro_atach_taskId, RO_attapb, RO_ATACH)
RM1301_ENTRY (ro_dtach_taskId, RO_detapb, RO_DTACH)
RM1301_ENTRY (ro_prbuf_taskId, RO_pvbfpb, RO_PRBUF)
RM1301_ENTRY (ro_redrq_taskId, RO_rdrqpb, RO_REDRQ)
RM1301_ENTRY (ro_wake_taskId,  RO_wakepb, RO_WAKE )
RM1301_ENTRY (ro_infrm_taskId, RO_inform, RO_INFRM)


/* ****************** Association handling function entries ********* */
/* ******************                                       ********* */

RM1301_ENTRY (ro_binrq_taskId, RO_bnrqpb, RO_BINRQ)
RM1301_ENTRY (ro_binrs_taskId, RO_bnrspb, RO_BINRS)
RM1301_ENTRY (ro_ubnrq_taskId, RO_ubrqpb, RO_UBNRQ)
RM1301_ENTRY (ro_ubnrs_taskId, RO_ubrspb, RO_UBNRS)
RM1301_ENTRY (ro_aborq_taskId, RO_abrqpb, RO_ABORQ)


/* ****************** Remote operation functions entries ************ */
/* ******************                                    ************ */

RM1301_ENTRY (ro_invrq_taskId, RO_ivrqpb, RO_INVRQ)
RM1301_ENTRY (ro_resrq_taskId, RO_rsrqpb, RO_RESRQ)
RM1301_ENTRY (ro_errrq_taskId, RO_errqpb, RO_ERRRQ)
RM1301_ENTRY (ro_rjurq_taskId, RO_rurqpb, RO_RJURQ)


/* ****************** ROS logging functions ************************* */
/* ******************                       ************************* */

RM1301_ENTRY (ro_logon_taskId, RO_lgonpb, RO_LOGON)
RM1301_ENTRY (ro_logof_taskId, RO_lgofpb, RO_LOGOF)
RM1301_ENTRY (ro_logwr_taskId, RO_lgwrpb, RO_LOGWR)

#endif	/* _ROEXIF09_H */
