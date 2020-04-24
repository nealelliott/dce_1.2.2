/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm00.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:14  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:51  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:36  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:13:01  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:18  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:29  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:07:34  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:45:53  keutel
 * 	creation
 * 	[1994/03/21  13:36:11  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rospm00.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:14 $" } ;
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
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : rospm00.c                                           */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-08                                            */
/*                                                                    */
/* COMPONENT    : RTROS convergence protocol machine                  */
/*                                                                    */
/* PRD#/VERS.   : RTROS V-3.00                                        */
/*                                                                    */
/* DESCRIPTION  : This modul provides the main function of the        */
/*                convergence protocol machine and the                */
/*                state-event tables of the PM.                       */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/* 3.00 K00 | 93-03-08|  original                      |ma  |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "@(#) rospm00.c 1.2 93/12/20 RTROS-V3.00";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>    /* Include of ROS global defines              */
#include <roerval.h>    /* Includes of error values                   */
#include <routil0.h>    /* Include of ROS common utilites             */
#include <rouply0.h>    /* Common generation constants,types & values */
#include <rouply1.h>    /* PM service interface structures            */ 
#include <rodpat0.h>    /* Prototyping of Dispatcher functions        */
#include <rostor0.h>    /* Prototyping of Store functions             */
#include <rolgval.h>    /* Defines & prototyping for logging          */

#include <rotpm00.h>    /* Include of TPM events                      */
#include <rospm00.h>    /* Include of SPM events                      */

#include <rospm01.h>    /* Defines for coding functions               */
#include <rospm02.h>    /* Defines for entry  functions               */
#include <rospm07.h>    /* Includes local to protocol machine         */ 
#include <rospm03.h>    /* Defines for convergence protocol m/c       */  
#include <rospm05.h>    /* Prototyping of coding of functions         */ 
#include <rospm06.h>    /* Prototyping of convergence protocol m/c    */    

#include <rospmz0.h>    /* Defines for Inform function                */

/*********************************************************************/
/*                                                                   */
/*              D E F I N E S                                        */
/*                                                                   */
/*********************************************************************/

/*********************************************************************/
/*                                                                   */
/*              G L O B A L   V A R I A B L E S                      */
/*                                                                   */
/*********************************************************************/

static signed short int  ( NEAR PASCAL *sematrix[RC08dz_MaxStates][RC08dy_MaxInEvents])
(
rT0870_stSpmContext *pContext,       /*  SPM context ( pointer to )   */
unsigned long int ulP1 ,        /* Event parameter P1            */
unsigned long int ulP2 ,        /* Event parameter P2            */
unsigned long int ulP3 ,        /* Event parameter P3            */
unsigned long int ulP4          /* event parameter P4            */ 
) = 
  {  /* state: RC08d0_STA01  :             idle, no transport connection */ 
    {
        /* event: T_CONNECT.indication     */  ro0842_STA01xTCONind     ,   /*  00 */
        /* event: T_CONNECT.confirmation   */  ro0830_AbortLogicalFalse ,   /*  01 */ 
        /* event: T_DISCONNECT.indication  */  ro0830_AbortLogicalFalse ,   /*  02 */
        /* event: T_CONTINUE               */  ro0830_AbortLogicalFalse ,   /*  03 */ 
        /* event: S_CONNECT.request        */  ro0841_STA01xSCONreq     ,   /*  04 */
        /* event: S_CONNECT.response pos.  */  ro0832_LocalMatter       ,   /*  05 */
        /* event: S_CONNECT.response neg.  */  ro0832_LocalMatter       ,   /*  06 */
        /* event: CN                       */  ro0830_AbortLogicalFalse ,   /*  07 */
        /* event: CDO                      */  ro0830_AbortLogicalFalse ,   /*  08 */
        /* event: OA                       */  ro0830_AbortLogicalFalse ,   /*  09 */ 
        /* event: AC                       */  ro0830_AbortLogicalFalse ,   /*  10 */ 
        /* event: RF                       */  ro0830_AbortLogicalFalse ,   /*  11 */ 
        /* event: S_RELEASE.request        */  ro0834_NoProtocolEvent   ,   /*  12 */
        /* event: S_RELEASE.response pos.  */  ro0834_NoProtocolEvent   ,   /*  13 */
        /* event: S_RELEASE.response neg.  */  ro0834_NoProtocolEvent   ,   /*  14 */
        /* event: FN                       */  ro0834_NoProtocolEvent   ,   /*  15 */
        /* event: AA                       */  ro0830_AbortLogicalFalse ,   /*  16 */ 
        /* event: NF                       */  ro0834_NoProtocolEvent   ,   /*  17 */
        /* event: AB                       */  ro0830_AbortLogicalFalse ,   /*  18 */ 
        /* event: DN                       */  ro0834_NoProtocolEvent   ,   /*  19 */
        /* event: S_USER_ABORT.request     */  ro0832_LocalMatter       ,   /*  20 */
        /* event: S_DATA.request           */  ro0834_NoProtocolEvent   ,   /*  21 */
        /* event: DT                       */  ro0834_NoProtocolEvent   ,   /*  22 */
        /* event: RC08en_NoEvent           */  ro0833_NothingHappens    ,   /*  23 */
   },
  
    /* state: RC08d1_STA01A :   wait for ABORT ACCEPT SPDU              */ 
   {
        /* event: T_CONNECT.indication     */ ro0830_AbortLogicalFalse  ,   /*  00 */
        /* event: T_CONNECT.confirmation   */ ro0830_AbortLogicalFalse  ,   /*  01 */
        /* event: T_DISCONNECT.indication  */ ro0837_Abort4             ,   /*  02 */
        /* event: T_CONTINUE               */ ro0830_AbortLogicalFalse  ,   /*  03 */
        /* event: S_CONNECT.request        */ ro0832_LocalMatter        ,   /*  04 */
        /* event: S_CONNECT.response pos.  */ ro0832_LocalMatter        ,   /*  05 */
        /* event: S_CONNECT.response neg.  */ ro0832_LocalMatter        ,   /*  06 */
        /* event: CN                       */ ro0836_Abort2             ,   /*  07 */
        /* event: CDO                      */ ro0831_InvalidSpdu        ,   /*  08 */
        /* event: OA                       */ ro0831_InvalidSpdu        ,   /*  09 */
        /* event: AC                       */ ro0833_NothingHappens     ,   /*  10 */
        /* event: RF                       */ ro0833_NothingHappens     ,   /*  11 */
        /* event: S_RELEASE.request        */ ro0832_LocalMatter        ,   /*  12 */
        /* event: S_RELEASE.response pos.  */ ro0832_LocalMatter        ,   /*  13 */
        /* event: S_RELEASE.response neg.  */ ro0832_LocalMatter        ,   /*  14 */
        /* event: FN                       */ ro0833_NothingHappens     ,   /*  15 */
        /* event: AA                       */ ro0843_STA01AxAA          ,   /*  16 */
        /* event: NF                       */ ro0833_NothingHappens     ,   /*  17 */
        /* event: AB                       */ ro0836_Abort2             ,   /*  18 */
        /* event: DN                       */ ro0833_NothingHappens     ,   /*  19 */
        /* event: S_USER_ABORT.request     */ ro0832_LocalMatter        ,   /*  20 */
        /* event: S_DATA.request           */ ro0832_LocalMatter        ,   /*  21 */
        /* event: DT                       */ ro0833_NothingHappens     ,   /*  22 */
        /* event: no_event                 */ ro0833_NothingHappens     ,   /*  23 */
   },

    /* state: RC08d2_STA01B :   wait for T_CONNECT.confirmation         */ 
   {
        /* event: T_CONNECT.indication     */  ro0830_AbortLogicalFalse ,   /*  00 */
        /* event: T_CONNECT.confirmation   */  ro0844_STA01BxTCONcnf    ,   /*  01 */
        /* event: T_DISCONNECT.indication  */  ro0839_TDISCONind        ,   /*  02 */
        /* event: T_CONTINUE               */  ro0840_TCONTINUE         ,   /*  03 */
        /* event: S_CONNECT.request        */  ro0832_LocalMatter       ,   /*  04 */
        /* event: S_CONNECT.response pos.  */  ro0832_LocalMatter       ,   /*  05 */
        /* event: S_CONNECT.response neg.  */  ro0832_LocalMatter       ,   /*  06 */
        /* event: CN                       */  ro0830_AbortLogicalFalse ,   /*  07 */
        /* event: CDO                      */  ro0830_AbortLogicalFalse ,   /*  08 */
        /* event: OA                       */  ro0830_AbortLogicalFalse ,   /*  09 */
        /* event: AC                       */  ro0830_AbortLogicalFalse ,   /*  10 */
        /* event: RF                       */  ro0830_AbortLogicalFalse ,   /*  11 */
        /* event: S_RELEASE.request        */  ro0834_NoProtocolEvent   ,   /*  12 */
        /* event: S_RELEASE.response pos.  */  ro0834_NoProtocolEvent   ,   /*  13 */
        /* event: S_RELEASE.response neg.  */  ro0834_NoProtocolEvent   ,   /*  14 */
        /* event: FN                       */  ro0834_NoProtocolEvent   ,   /*  15 */
        /* event: AA                       */  ro0830_AbortLogicalFalse ,   /*  16 */
        /* event: NF                       */  ro0834_NoProtocolEvent   ,   /*  17 */
        /* event: AB                       */  ro0830_AbortLogicalFalse ,   /*  18 */
        /* event: DN                       */  ro0834_NoProtocolEvent   ,   /*  19 */
        /* event: S_USER_ABORT.request     */  ro0878_PendAbort         ,   /*  20 */
        /* event: S_DATA.request           */  ro0834_NoProtocolEvent   ,   /*  21 */
        /* event: DT                       */  ro0834_NoProtocolEvent   ,   /*  22 */
        /* event: no_event                 */  ro0833_NothingHappens    ,   /*  23 */
   },
  
    /* state: RC08d3_STA01C :   idle , transport connected              */ 
   {
        /* event: T_CONNECT.indication     */  ro0830_AbortLogicalFalse ,   /*  00 */
        /* event: T_CONNECT.confirmation   */  ro0830_AbortLogicalFalse ,   /*  01 */
        /* event: T_DISCONNECT.indication  */  ro0838_Abort5            ,   /*  02 */
        /* event: T_CONTINUE               */  ro0840_TCONTINUE         ,   /*  03 */
        /* event: S_CONNECT.request        */  ro0846_STA01CxSCONreq    ,   /*  04 */
        /* event: S_CONNECT.response pos.  */  ro0832_LocalMatter       ,   /*  05 */
        /* event: S_CONNECT.response neg.  */  ro0832_LocalMatter       ,   /*  06 */
        /* event: CN                       */  ro0845_STA01CxCN         ,   /*  07 */
        /* event: CDO                      */  ro0835_Abort1            ,   /*  08 */
        /* event: OA                       */  ro0835_Abort1            ,   /*  09 */
        /* event: AC                       */  ro0835_Abort1            ,   /*  10 */
        /* event: RF                       */  ro0835_Abort1            ,   /*  11 */
        /* event: S_RELEASE.request        */  ro0832_LocalMatter       ,   /*  12 */
        /* event: S_RELEASE.response pos.  */  ro0832_LocalMatter       ,   /*  13 */
        /* event: S_RELEASE.response neg.  */  ro0832_LocalMatter       ,   /*  14 */
        /* event: FN                       */  ro0835_Abort1            ,   /*  15 */
        /* event: AA                       */  ro0835_Abort1            ,   /*  16 */
        /* event: NF                       */  ro0835_Abort1            ,   /*  17 */
        /* event: AB                       */  ro0835_Abort1            ,   /*  18 */
        /* event: DN                       */  ro0835_Abort1            ,   /*  19 */
        /* event: S_USER_ABORT.request     */  ro0832_LocalMatter       ,   /*  20 */
        /* event: S_DATA.request           */  ro0832_LocalMatter       ,   /*  21 */
        /* event: DT                       */  ro0835_Abort1            ,   /*  22 */
        /* event: no_event                 */  ro0833_NothingHappens    ,   /*  23 */
   },

    /* state: RC08d4_STA01D :   wait for CDO SPDU                       */ 
   {
        /* event: T_CONNECT.indication     */  ro0830_AbortLogicalFalse ,  /*  00 */
        /* event: T_CONNECT.confirmation   */  ro0830_AbortLogicalFalse ,  /*  01 */
        /* event: T_DISCONNECT.indication  */  ro0838_Abort5            ,  /*  02 */
        /* event: T_CONTINUE               */  ro0840_TCONTINUE         ,  /*  03 */
        /* event: S_CONNECT.request        */  ro0832_LocalMatter       ,  /*  04 */
        /* event: S_CONNECT.response pos.  */  ro0832_LocalMatter       ,  /*  05 */
        /* event: S_CONNECT.response neg.  */  ro0832_LocalMatter       ,  /*  06 */
        /* event: CN                       */  ro0831_InvalidSpdu       ,  /*  07 */
        /* event: CDO    (sta01d_cdo)      */  ro0831_InvalidSpdu       ,  /*  08 */
        /* event: OA                       */  ro0831_InvalidSpdu       ,  /*  09 */
        /* event: AC                       */  ro0831_InvalidSpdu       ,  /*  10 */
        /* event: RF                       */  ro0831_InvalidSpdu       ,  /*  11 */
        /* event: S_RELEASE.request        */  ro0832_LocalMatter       ,  /*  12 */
        /* event: S_RELEASE.response pos.  */  ro0832_LocalMatter       ,  /*  13 */
        /* event: S_RELEASE.response neg.  */  ro0832_LocalMatter       ,  /*  14 */
        /* event: FN                       */  ro0835_Abort1            ,  /*  15 */
        /* event: AA                       */  ro0835_Abort1            ,  /*  16 */
        /* event: NF                       */  ro0835_Abort1            ,  /*  17 */
        /* event: AB                       */  ro0835_Abort1            ,  /*  18 */
        /* event: DN                       */  ro0835_Abort1            ,  /*  19 */
        /* event: S_USER_ABORT.request     */  ro0832_LocalMatter       ,  /*  20 */
        /* event: S_DATA.request           */  ro0832_LocalMatter       ,  /*  21 */
        /* event: DT                       */  ro0835_Abort1            ,  /*  22 */
        /* event: no_event                 */  ro0833_NothingHappens    ,  /*  23 */
   },
  
    /* state: RC08d5_STA01E :   wait for T_CONTINUE, S_CONNECT_REQUEST + MORE - data */ 
    /* state: RC08d5_STA01E :   not applied since 25.3.91                            */ 
   {
        /* event: T_CONNECT.indication     */ ro0830_AbortLogicalFalse ,  /*  00 */
        /* event: T_CONNECT.confirmation   */ ro0830_AbortLogicalFalse ,  /*  01 */
        /* event: T_DISCONNECT.indication  */ ro0830_AbortLogicalFalse ,  /*  02 */
        /* event: T_CONTINUE               */ ro0830_AbortLogicalFalse ,  /*  03 */
        /* event: S_CONNECT.request        */ ro0830_AbortLogicalFalse ,  /*  04 */
        /* event: S_CONNECT.response pos.  */ ro0830_AbortLogicalFalse ,  /*  05 */
        /* event: S_CONNECT.response neg.  */ ro0830_AbortLogicalFalse ,  /*  06 */
        /* event: CN                       */ ro0830_AbortLogicalFalse ,  /*  07 */
        /* event: CDO                      */ ro0830_AbortLogicalFalse ,  /*  08 */
        /* event: OA                       */ ro0830_AbortLogicalFalse ,  /*  09 */
        /* event: AC                       */ ro0830_AbortLogicalFalse ,  /*  10 */
        /* event: RF                       */ ro0830_AbortLogicalFalse ,  /*  11 */
        /* event: S_RELEASE.request        */ ro0830_AbortLogicalFalse ,  /*  12 */
        /* event: S_RELEASE.response pos.  */ ro0830_AbortLogicalFalse ,  /*  13 */
        /* event: S_RELEASE.response neg.  */ ro0830_AbortLogicalFalse ,  /*  14 */
        /* event: FN                       */ ro0830_AbortLogicalFalse ,  /*  15 */
        /* event: AA                       */ ro0830_AbortLogicalFalse ,  /*  16 */
        /* event: NF                       */ ro0830_AbortLogicalFalse ,  /*  17 */
        /* event: AB                       */ ro0830_AbortLogicalFalse ,  /*  18 */
        /* event: DN                       */ ro0830_AbortLogicalFalse ,  /*  19 */
        /* event: S_USER_ABORT.request     */ ro0830_AbortLogicalFalse ,  /*  20 */
        /* event: S_DATA.request           */ ro0830_AbortLogicalFalse ,  /*  21 */
        /* event: DT                       */ ro0830_AbortLogicalFalse ,  /*  22 */
        /* event: no_event                 */ ro0830_AbortLogicalFalse ,  /*  23 */
   },                                                               

    /* state: RC08d6_STA02A :   wait for ACCEPT SPDU                    */ 
   {
        /* event: T_CONNECT.indication     */  ro0830_AbortLogicalFalse , /*  00 */
        /* event: T_CONNECT.confirmation   */  ro0830_AbortLogicalFalse , /*  01 */
        /* event: T_DISCONNECT.indication  */  ro0839_TDISCONind        , /*  02 */
        /* event: T_CONTINUE               */  ro0840_TCONTINUE         , /*  03 */
        /* event: S_CONNECT.request        */  ro0832_LocalMatter       , /*  04 */
        /* event: S_CONNECT.response pos.  */  ro0832_LocalMatter       , /*  05 */
        /* event: S_CONNECT.response neg.  */  ro0832_LocalMatter       , /*  06 */
        /* event: CN                       */  ro0831_InvalidSpdu       , /*  07 */
        /* event: CDO                      */  ro0831_InvalidSpdu       , /*  08 */
        /* event: OA                       */  ro0831_InvalidSpdu       , /*  09 */
        /* event: AC                       */  ro0851_STA02AxAC         , /*  10 */
        /* event: RF                       */  ro0850_STA02xRF          , /*  11 */
        /* event: S_RELEASE.request        */  ro0834_NoProtocolEvent   , /*  12 */
        /* event: S_RELEASE.response pos.  */  ro0834_NoProtocolEvent   , /*  13 */
        /* event: S_RELEASE.response neg.  */  ro0834_NoProtocolEvent   , /*  14 */
        /* event: FN                       */  ro0834_NoProtocolEvent   , /*  15 */ 
        /* event: AA                       */  ro0831_InvalidSpdu       , /*  16 */
        /* event: NF                       */  ro0834_NoProtocolEvent   , /*  17 */
        /* event: AB                       */  ro083b_Ab                , /*  18 */
        /* event: DN                       */  ro0834_NoProtocolEvent   , /*  19 */
        /* event: s_user_abort.request     */  ro083a_SUABORTreq        , /*  20 */
        /* event: S_DATA.request           */  ro0832_LocalMatter       , /*  21 */
        /* event: DT                       */  ro0831_InvalidSpdu       , /*  22 */
        /* event: no_event                 */  ro0833_NothingHappens    , /*  23 */
   },
  
    /* state: RC08d7_STA02B :   wait for OA SPDU                        */ 
   {
        /* event: T_CONNECT.indication     */  ro0830_AbortLogicalFalse ,  /*  00 */
        /* event: T_CONNECT.confirmation   */  ro0830_AbortLogicalFalse ,  /*  01 */
        /* event: T_DISCONNECT.indication  */  ro0839_TDISCONind        ,  /*  02 */
        /* event: T_CONTINUE               */  ro0840_TCONTINUE         ,  /*  03 */
        /* event: S_CONNECT.request ( sta02b_s_con_req ) */
                                               ro0832_LocalMatter       ,  /*  04 */
        /* event: S_CONNECT.response pos.  */  ro0832_LocalMatter       ,  /*  05 */
        /* event: S_CONNECT.response neg.  */  ro0832_LocalMatter       ,  /*  06 */
        /* event: CN                       */  ro0831_InvalidSpdu       ,  /*  07 */
        /* event: CDO                      */  ro0831_InvalidSpdu       ,  /*  08 */
        /* event: OA    ( sta02b_oa )      */  ro0831_InvalidSpdu       ,  /*  09 */
        /* event: AC                       */  ro0831_InvalidSpdu       ,  /*  10 */
        /* event: RF                       */  ro0850_STA02xRF          ,  /*  11 */
        /* event: S_RELEASE.request        */  ro0834_NoProtocolEvent   ,  /*  12 */
        /* event: S_RELEASE.response pos.  */  ro0834_NoProtocolEvent   ,  /*  13 */
        /* event: S_RELEASE.response neg.  */  ro0834_NoProtocolEvent   ,  /*  14 */
        /* event: FN                       */  ro0834_NoProtocolEvent   ,  /*  15 */
        /* event: AA                       */  ro0831_InvalidSpdu       ,  /*  16 */
        /* event: NF                       */  ro0834_NoProtocolEvent   ,  /*  17 */
        /* event: AB                       */  ro083b_Ab                ,  /*  18 */
        /* event: DN                       */  ro0834_NoProtocolEvent   ,  /*  19 */
        /* event: s_user_abort.request     */  ro083a_SUABORTreq        ,  /*  20 */
        /* event: S_DATA.request           */  ro0834_NoProtocolEvent   ,  /*  21 */
        /* event: DT                       */  ro0834_NoProtocolEvent   ,  /*  22 */
        /* event: no_event                 */  ro0833_NothingHappens    ,  /*  23 */
   },

    /* state: RC08d8_STA03  :   wait for DN SPDU             */   
   {
        /* event: T_CONNECT.indication     */  ro0834_NoProtocolEvent  ,  /*  00 */
        /* event: T_CONNECT.confirmation   */  ro0834_NoProtocolEvent  ,  /*  01 */
        /* event: T_DISCONNECT.indication  */  ro0839_TDISCONind       ,  /*  02 */
        /* event: T_CONTINUE               */  ro0840_TCONTINUE        ,  /*  03 */
        /* event: S_CONNECT.request        */  ro0834_NoProtocolEvent  ,  /*  04 */
        /* event: S_CONNECT.response pos.  */  ro0834_NoProtocolEvent  ,  /*  05 */
        /* event: S_CONNECT.response neg.  */  ro0834_NoProtocolEvent  ,  /*  06 */
        /* event: CN                       */  ro0834_NoProtocolEvent  ,  /*  07 */
        /* event: CDO                      */  ro0834_NoProtocolEvent  ,  /*  08 */
        /* event: OA                       */  ro0834_NoProtocolEvent  ,  /*  09 */
        /* event: AC                       */  ro0834_NoProtocolEvent  ,  /*  10 */
        /* event: RF                       */  ro0834_NoProtocolEvent  ,  /*  11 */
        /* event: S_RELEASE.request        */  ro0832_LocalMatter      ,  /*  12 */
        /* event: S_RELEASE.response pos.  */  ro0832_LocalMatter      ,  /*  13 */
        /* event: S_RELEASE.response neg.  */  ro0832_LocalMatter      ,  /*  14 */
        /* event: FN                       */  ro0860_STA03xFN         ,  /*  15 */
        /* event: AA                       */  ro0831_InvalidSpdu      ,  /*  16 */
        /* event: NF                       */  ro0835_Abort1           ,  /*  17 */
        /* event: AB                       */  ro083b_Ab               ,  /*  18 */
        /* event: DN                       */  ro0862_STA03xDN         ,  /*  19 */
        /* event: s_user_abort.request     */  ro083a_SUABORTreq       ,  /*  20 */
        /* event: S_DATA.request           */  ro0832_LocalMatter      ,  /*  21 */
        /* event: DT                       */  ro0861_STA03xDT         ,  /*  22 */
        /* event: no_event                 */  ro0833_NothingHappens   ,  /*  23 */
   },
  
    /* state: RC08d9_STA08  :   wait for S_CONNECT.response             */ 
   {
        /* event: T_CONNECT.indication     */  ro0830_AbortLogicalFalse , /*  00 */
        /* event: T_CONNECT.confirmation   */  ro0830_AbortLogicalFalse , /*  01 */
        /* event: T_DISCONNECT.indication  */  ro0839_TDISCONind        , /*  02 */
        /* event: T_CONTINUE               */  ro0840_TCONTINUE         , /*  03 */
        /* event: S_CONNECT.request        */  ro0832_LocalMatter       , /*  04 */
        /* event: S_CONNECT.response pos.  */  ro0863_STA08xSCONrspPos  , /*  05 */
        /* event: S_CONNECT.response neg.  */  ro0864_STA08xSCONrspNeg  , /*  06 */
        /* event: CN                       */  ro0831_InvalidSpdu       , /*  07 */
        /* event: CDO                      */  ro0831_InvalidSpdu       , /*  08 */
        /* event: OA                       */  ro0831_InvalidSpdu       , /*  09 */
        /* event: AC                       */  ro0831_InvalidSpdu       , /*  10 */
        /* event: RF                       */  ro0831_InvalidSpdu       , /*  11 */
        /* event: S_RELEASE.request        */  ro0834_NoProtocolEvent   , /*  12 */
        /* event: S_RELEASE.response  pos. */  ro0834_NoProtocolEvent   , /*  13 */
        /* event: S_RELEASE.response neg.  */  ro0834_NoProtocolEvent   , /*  14 */
        /* event: FN                       */  ro0834_NoProtocolEvent   , /*  15 */
        /* event: AA                       */  ro0831_InvalidSpdu       , /*  16 */
        /* event: NF                       */  ro0834_NoProtocolEvent   , /*  17 */ 
        /* event: AB                       */  ro083b_Ab                , /*  18 */
        /* event: DN                       */  ro0834_NoProtocolEvent   , /*  19 */
        /* event: s_user_abort.request     */  ro083a_SUABORTreq        , /*  20 */
        /* event: S_DATA.request           */  ro0834_NoProtocolEvent   , /*  21 */
        /* event: DT                       */  ro0834_NoProtocolEvent   , /*  22 */
        /* event: no_event                 */  ro0833_NothingHappens    , /*  23 */
   },

    /* state: RC08da_STA09  :   wait for S_RELEASE.response             */ 
   {
        /* event: T_CONNECT.indication     */  ro0834_NoProtocolEvent ,   /*  00 */
        /* event: T_CONNECT.confirmation   */  ro0834_NoProtocolEvent ,   /*  01 */ 
        /* event: T_DISCONNECT.indication  */  ro0839_TDISCONind      ,   /*  02 */
        /* event: T_CONTINUE               */  ro0840_TCONTINUE       ,   /*  03 */
        /* event: S_CONNECT.request        */  ro0834_NoProtocolEvent ,   /*  04 */
        /* event: S_CONNECT.response pos.  */  ro0834_NoProtocolEvent ,   /*  05 */
        /* event: S_CONNECT.response neg.  */  ro0834_NoProtocolEvent ,   /*  06 */
        /* event: CN                       */  ro0834_NoProtocolEvent ,   /*  07 */
        /* event: CDO                      */  ro0834_NoProtocolEvent ,   /*  08 */
        /* event: OA                       */  ro0834_NoProtocolEvent ,   /*  09 */
        /* event: AC                       */  ro0834_NoProtocolEvent ,   /*  10 */
        /* event: RF                       */  ro0834_NoProtocolEvent ,   /*  11 */
        /* event: S_RELEASE.request        */  ro0870_STA09xSRELreq   ,   /*  12 */
        /* event: S_RELEASE.response pos.  */  ro0871_STA09xSRELrspPos,   /*  13 */
        /* event: S_RELEASE.response neg.  */  ro0832_LocalMatter     ,   /*  14 */
        /* event: FN                       */  ro0831_InvalidSpdu     ,   /*  15 */
        /* event: AA                       */  ro0831_InvalidSpdu     ,   /*  16 */
        /* event: NF                       */  ro0831_InvalidSpdu     ,   /*  17 */
        /* event: AB                       */  ro083b_Ab              ,   /*  18 */
        /* event: DN                       */  ro0872_STA09xDN        ,   /*  19 */
        /* event: s_user_abort.request     */  ro083a_SUABORTreq      ,   /*  20 */
        /* event: S_DATA.request           */  ro0873_STA09xSDATAreq  ,   /*  21 */
        /* event: DT                       */  ro0831_InvalidSpdu     ,   /*  22 */
        /* event: no_event                 */  ro0833_NothingHappens  ,   /*  23 */
   },                                                               
  
    /* state: RC08db_STA16  :   wait for T_CONNECT.indication           */ 
   {
        /* event: T_CONNECT.indication     */  ro0830_AbortLogicalFalse ,  /*  00 */
        /* event: T_CONNECT.confirmation   */  ro0830_AbortLogicalFalse ,  /*  01 */
        /* event: T_DISCONNECT.indication  */  ro0837_Abort4            ,  /*  02 */
        /* event: T_CONTINUE               */  ro0840_TCONTINUE         ,  /*  03 */
        /* event: S_CONNECT.request        */  ro0832_LocalMatter       ,  /*  04 */
        /* event: S_CONNECT.response pos.  */  ro0832_LocalMatter       ,  /*  05 */
        /* event: S_CONNECT.response neg.  */  ro0832_LocalMatter       ,  /*  06 */
        /* event: CN                       */  ro0836_Abort2            ,  /*  07 */
        /* event: CDO                      */  ro0831_InvalidSpdu       ,  /*  08 */
        /* event: OA                       */  ro0831_InvalidSpdu       ,  /*  09 */
        /* event: AC                       */  ro0833_NothingHappens    ,  /*  10 */
        /* event: RF                       */  ro0833_NothingHappens    ,  /*  11 */
        /* event: S_RELEASE.request        */  ro0832_LocalMatter       ,  /*  12 */
        /* event: S_RELEASE.response  pos. */  ro0832_LocalMatter       ,  /*  13 */
        /* event: S_RELEASE.response neg.  */  ro0832_LocalMatter       ,  /*  14 */
        /* event: FN                       */  ro0833_NothingHappens    ,  /*  15 */
        /* event: AA                       */  ro0836_Abort2            ,  /*  16 */
        /* event: NF                       */  ro0833_NothingHappens    ,  /*  17 */
        /* event: AB                       */  ro0836_Abort2            ,  /*  18 */
        /* event: DN                       */  ro0833_NothingHappens    ,  /*  19 */
        /* event: s_user_abort.request     */  ro0832_LocalMatter       ,  /*  20 */
        /* event: S_DATA.request           */  ro0832_LocalMatter       ,  /*  21 */
        /* event: DT                       */  ro0833_NothingHappens    ,  /*  22 */
        /* event: no_event                 */  ro0833_NothingHappens    ,  /*  23 */
   },

    /* state: RC08dc_STA713 :   data tranfer                            */ 
   {
        /* event: T_CONNECT.indication     */  ro0834_NoProtocolEvent  ,  /*  00 */
        /* event: T_CONNECT.confirmation   */  ro0834_NoProtocolEvent  ,  /*  01 */ 
        /* event: T_DISCONNECT.indication  */  ro0839_TDISCONind       ,  /*  02 */
        /* event: T_CONTINUE               */  ro0840_TCONTINUE        ,  /*  03 */
        /* event: S_CONNECT.request        */  ro0834_NoProtocolEvent  ,  /*  04 */
        /* event: S_CONNECT.response pos.  */  ro0834_NoProtocolEvent  ,  /*  05 */
        /* event: S_CONNECT.response neg.  */  ro0834_NoProtocolEvent  ,  /*  06 */
        /* event: CN                       */  ro0834_NoProtocolEvent  ,  /*  07 */
        /* event: CDO                      */  ro0834_NoProtocolEvent  ,  /*  08 */
        /* event: OA                       */  ro0834_NoProtocolEvent  ,  /*  09 */
        /* event: AC                       */  ro0834_NoProtocolEvent  ,  /*  10 */
        /* event: RF                       */  ro0834_NoProtocolEvent  ,  /*  11 */ 
        /* event: S_RELEASE.request        */  ro0874_STA713xSRELreq   ,  /*  12 */
        /* event: S_RELEASE.response pos.  */  ro0832_LocalMatter      ,  /*  13 */
        /* event: S_RELEASE.response neg.  */  ro0832_LocalMatter      ,  /*  14 */
        /* event: FN                       */  ro0875_STA713xFN        ,  /*  15 */
        /* event: AA                       */  ro0831_InvalidSpdu      ,  /*  16 */
        /* event: NF                       */  ro0831_InvalidSpdu      ,  /*  17 */
        /* event: AB                       */  ro083b_Ab               ,  /*  18 */
        /* event: DN                       */  ro0831_InvalidSpdu      ,  /*  19 */
        /* event: s_user_abort.request     */  ro083a_SUABORTreq       ,  /*  20 */
        /* event: S_DATA.request           */  ro0876_STA713xSDATAreq  ,  /*  21 */
        /* event: DT                       */  ro0877_STA713xDT        ,  /*  22 */
        /* event: no_event                 */  ro0833_NothingHappens   ,  /*  23 */
    }
  };


/*********************************************************************/
/*                                                                   */
/*              G L O B A L   F U N C T I O N S                      */
/*                                                                   */
/*********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro080a_InitContext                                  */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-08                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

void ro080a_InitContext ( rT0870_stSpmContext *pContext )

/*                                                                    */
/* DESCRIPTION  :     At the first call of the protocol machine       */
/*                    the context buffer will be initialized          */
/*                    with default values.                            */
/*                                                                    */
/* IN-PARAMETERS           :                                          */
/*                                                                    */
/*     pContext  =   Pointer to Context Buffer of Protocol Machine    */
/*                                                                    */
/* FORMAL OUT-PARAMETERS   :                                          */
/*                                                                    */
/*     pContext  =   Pointer to Context Buffer of Protocol Machine    */
/*                                                                    */
/* RETURN VALUES           :    none                                  */
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
{/*entr-ro080a_InitContext*/

  RM0001_STRG (fnctName, "ro080a_InitContext")

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
                                                                        
  RM0002_ENTRY (fnctName)                                               
   
   pContext->usState               = NULL;
   pContext->usVtca                = NULL;   
   pContext->usVcoll               = NULL;
   pContext->usVdnr                = NULL;
   pContext->ulTidusize            = NULL;

   ro082B_InitStruc(pContext->vCalledAddrs,RC0830_AddressLen+1);
   pContext->sCalledAddrsLen       = NULL;

   pContext->hAddrs                = NULL;
   pContext->hCalledAddress        = NULL;

   pContext->usFunctionalUnits     = NULL;
   pContext->usPopt                = RC0875_NoExtendSPDU;
   pContext->usProtvers            = NULL;
   pContext->ulConectresult        = NULL;
   pContext->usUserConnExists      = FALSE;
   pContext->hUserData             = NULL;
   pContext->ulOffset              = NULL;

   pContext->bAbortPend            = FALSE;

   /* pContext->usRedirection      = FALSE; */
   ro082B_InitStruc(&(pContext->stRedir),sizeof(rT08a0_stRedirect));
   

  RM0004_RETURN_VOID (fnctName) 
}/*end-ro080a_InitContext*/ 



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro080b_TransportData                                */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-08                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int  ro080b_TransportData (
unsigned long int ulParam1 )

/*                                                                    */
/* DESCRIPTION  :   This function decodes the incomming TSDU.         */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*     ulParam1   =   Handle to store                                 */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      : none                                  */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      Returns the value returned by function                        */
/*      ro082A_WhichSpdu().                                           */
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
{/*entr-ro080b_TransportData*/

signed short int ret;
unsigned long int hInData;
char              cInData;

  RM0001_STRG(functName,"ro080b_TransportData")
  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
   
     hInData  =  ulParam1;

     if(ro0074_storeactive(hInData , 0L) != OK ) 
          RM0003_RETURN(functName, signed short int, -R_FSTERR )

     if ((cInData = (char)ro0077_stgetcp (hInData) ) < 0)
          RM0003_RETURN(functName, signed short int, -R_FSTERR)

     ret = ro082A_WhichSpdu(cInData);

     if(ro0073_storeidle(hInData) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR )

  /* data buffer will be worked immediately as all the TSDUs are */
  /* comming in one store only                                   */

RM0003_RETURN(functName, signed short int, ret)
                  
}/*end-ro080b_TransportData*/



/*exon **********************************************************************/
/*                                                                          */
/* TYPE         : C-FUNKTION                                                */
/*                                                                          */
/* NAME         : ro0801_cleanUp                                            */
/*                                                                          */
/* AUTHOR       : Milind M. Aphale                                          */
/* DATE         : 93-03-08                                                  */
/*                                                                          */
/* SYNTAX       :                                                           */

signed short int FAR PASCAL ro0801_cleanUp ( 
signed   long int   levent,
unsigned long int   hP1orhContext,
unsigned long int   hP2,
unsigned long int   hP3,
unsigned long int   hP4)

/*                                                                            */
/* DESCRIPTION :                                                              */
/*                                                                            */
/*     If event is specified :                                                */
/*         Cleans up the memory space allocated to the p1..p4 parameters.     */
/*     If event no specified :                                                */
/*         Frees the context defined in place of P1.                          */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     levent        : 0      => hP1 = Context; hP2, hP3, hP4 not referenced  */
/*                     (!= 0) => hP1 & hP3 contain memory handles if not NULL */
/*     hP1orhContext : hContext, if lEvent == 0                               */
/*                     Memory handle, if (lEvent != 0 && hP1 != 0)            */
/*     hP2           : Not used                                               */
/*     hP3           : Memory handle, if (lEvent != 0 && hP3 != 0)            */
/*                     (Exception is RT_TRANSFER).                            */
/*     hP4           : Not used                                               */
/*                                                                            */
/*                                                                            */
/* OUT-PARAMETERS : none                                                      */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*      OK                                                                    */
/*     -R_FINTER                                                              */
/*                                                                            */
/* GLOBAL DATA   ( read only ):  none                                         */
/*                                                                            */
/* GLOBAL DATA   ( modified ) :  none                                         */
/*                                                                            */
/* REMARKS                    :  none                                         */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro0801_cleanUp*/

rT0871_unConn        * pConn;   /* pointer to parameter 3 structure  */
unsigned long int      hAddress = LNULL; /* mem handle to session address     */
unsigned long int      hCldAdr  = LNULL; /* mem handle to session Cld address */
rT0870_stSpmContext  * pContext;
 
  RM0001_STRG(functName,"ro0801_cleanUp")
  RM0002_ENTRY(functName)

/* ************************ S T A R T ****************************** */
/* ************************           ****************************** */

  if (levent != 0)                       
  {
  /*----------------------------------------------------------------*/
  /*                                                                */
  /*  CASE 1 :                                                      */
  /*          							    */
  /*----------------------------------------------------------------*/

    if (hP1orhContext != NULL)              /* param1 is handle to store */
                                            /* delete the store.         */
      if (ro0075_deletestore (hP1orhContext) != OK)
          RM0003_RETURN(functName, signed short int, -R_FSTERR)

     /* ----------------------------- */
     /* Free P3 and bound resources   */
     /* ----------------------------- */
    if (hP3)
    {  
      /*  free resources bound to P3  */
      if ( (pConn = (rT0871_unConn *) ro0031_locallock(hP3)) == NULL)
          RM0003_RETURN(functName, signed short int, -R_LLOCKE)
      switch (levent)
      {    
        case RC0801_SCONNECTreq : 
             hAddress = pConn->stReq.stAll.hCalledAddress;
             break;   

        case RC0802_SCONNECTind : 
             hAddress = pConn->stInd.stAll.hCallingAddress;
             hCldAdr = pConn->stInd.stAll.hCalledAddress;
             break;   

        case RC0803_SCONNECTrsp :
             hAddress = pConn->stRsp.stAll.hRespondAddress;
             break;   

        case RC0804_SCONNECTcnf : 
             hAddress = pConn->stCnf.stAll.hRespondAddress;
             break;   

        default                 :
             hAddress = NULL;
             hCldAdr  =NULL;
             break;
       }
       if(hAddress) {
         if(ro0034_localfree (hAddress))
            RM0003_RETURN(functName, signed short int, -R_LFREEE) 
       }
       if(hCldAdr) {
         if(ro0034_localfree (hCldAdr))
            RM0003_RETURN(functName, signed short int, -R_LFREEE) 
       }
       if(ro0032_localunlock (hP3))
            RM0003_RETURN(functName, signed short int, -R_LULOKE)

      /* free P3 */
       if(ro0034_localfree(hP3))
            RM0003_RETURN(functName, signed short int, -R_LFREEE)

    } /* if (hP3) */
  }
  else   /*  levent == NULL   */
  {  
    /*------------------------------------------------------------------------*/
    /*                                                                        */
    /* CASE 2:                                                                */
    /*                                                                        */
    /*         cleanUp called by Dispatcher. Only context need be freed.      */
    /*                                                                        */
    /*------------------------------------------------------------------------*/ 
    if (hP1orhContext != NULL)         
    {                                  
        pContext = (rT0870_stSpmContext *)ro0031_locallock (hP1orhContext);
        if (pContext == NULL)
            RM0003_RETURN(functName, signed short int, -R_FSTERR)

        if (pContext->hUserData  != NULL )
        {
            if(ro0075_deletestore(pContext->hUserData) != OK) 
                  RM0003_RETURN(functName, signed short int, -R_FSTERR)
            pContext->hUserData = NULL;
            pContext->ulOffset  = NULL;
        }  

        if (pContext->hAddrs != NULL)
            if (ro0034_localfree(pContext->hAddrs)) 
                RM0003_RETURN(functName, signed short int, -R_LFREEE)

        if (pContext->hCalledAddress != NULL)
            if (ro0034_localfree(pContext->hCalledAddress)) 
                RM0003_RETURN(functName, signed short int, -R_LFREEE)

        if (pContext->stRedir.hsStore )
            if (ro0075_deletestore(pContext->stRedir.hsStore) < 0)
                RM0003_RETURN(functName, signed short int, -R_SSTOER)

        if (ro0032_localunlock(hP1orhContext))
            RM0003_RETURN(functName, signed short int, -R_LULOKE)

        if (ro0034_localfree(hP1orhContext))
            RM0003_RETURN(functName, signed short int, -R_LFREEE)
    }
  }

  RM0003_RETURN(functName, signed short int, OK )

}/*end-ro0801_cleanUp*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0800_spm                                          */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-08                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int FAR PASCAL ro0800_spm (
unsigned long int hContext)

/*                                                                    */
/* DESCRIPTION  :   This function is the main function of the         */
/*                  convergence protocol machine.                     */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*        hContext   =   Handle to SPM context block                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    unrecoverable internal error                   */
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
{/*entr-ro0800_spm*/

  RM0001_STRG(functName,"ro0800_spm")

rT0870_stSpmContext *pContext;             /* pointer to PM context  */ 
unsigned long int ulP1;               /* event parameter P1     */ 
unsigned long int ulP2;               /* event parameter P2     */ 
unsigned long int ulP3;               /* event parameter P3     */ 
unsigned long int ulP4;               /* event parameter P4     */ 

signed long int  event;
signed short int in_event;

unsigned long int hACB, hECB;
signed long int ret = LNULL;
signed short int sRetCode = 0;

  RM0002_ENTRY(functName)

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

/* ------------------------------------------------------------------ */ 
/* Step 1 from PM-generic: lock PM context                            */
/*                                                                    */
/* The context buffer will be established and announced to the        */
/* dispatcher the first time or locked the other times.               */
/*                                                                    */
/* Note:If there occurs an error in this step, try to free the memory */   
/*       which was allocated for the PM context. P!? (ask JM)         */   
/* ------------------------------------------------------------------ */

  if (hContext ==NULL)
  {
    /* ------------------------------------------------------ */
    /* Allocate and lock a new context                        */
    /* ------------------------------------------------------ */

    if ((hContext = ro0030_localalloc(sizeof(rT0870_stSpmContext))) == NULL)
        RM0003_RETURN(functName, signed short int, -R_LALLCE )

    if ((pContext = (rT0870_stSpmContext *)ro0031_locallock(hContext)) == NULL)
    {
      if(ro0034_localfree(hContext) != NULL)
        RM0003_RETURN(functName, signed short int, -R_LFREEE)
      RM0003_RETURN(functName, signed short int, -R_LLOCKE)
    }

    sRetCode = ro0212_regPmInvoc(hContext);
    if (sRetCode != OK)
      RM0003_RETURN (functName, signed short int, sRetCode)

    ro080a_InitContext(pContext);
    pContext->usState   = RC08d0_STA01;
  }
  else
  {
    if ((pContext = (rT0870_stSpmContext *)ro0031_locallock(hContext)) == NULL)
       RM0003_RETURN(functName, signed short int, -R_LLOCKE )
  }

  /*--------------------------------------------*/  
  /* Step 2 from PM-generic: pop incoming event */  
  /*--------------------------------------------*/  

  event = ro0210_popInEvent(&ulP1,&ulP2,&ulP3,&ulP4);
  
/* ------------------------------------------------------------------ */
/* Step 3 from PM-generic: process incoming event                     */
/*                                                                    */  
/* The event will be transformed to the in_event including SPDU types */
/* This in_events will be needed in the state-event-matrix.           */
/* ------------------------------------------------------------------ */

  switch (event)
  {
  case RC0902_TCONNECTind:         in_event=RC08e0_TCNind;            break;
  case RC0904_TCONNECTcnf:         in_event=RC08e1_TCNcnf;            break;
  case RC0906_TDISCONNECTind:      in_event=RC08e2_TDNind;            break;
  case RC0909_TCONTINUE:           in_event=RC08e3_TCONT;             break;
  case RC0908_TDATAind:            in_event = ro080b_TransportData(ulP1);
                         if (in_event < OK)
                           RM0003_RETURN(functName, signed short int, in_event)
                                   break;

  case RC090b_TREDIRECTind:        pContext->usState = RC08d3_STA01C;
                                   in_event = RC08e7_ACN;
           /* alternative : in_event = ro080b_TransportData(ulP1); */ 
                                   /* pContext->usRedirection = TRUE; */
                                   pContext->usVtca = TRUE;
                        if(ulP3){
                         if(ro0034_localfree(ulP3) != NULL)
                           RM0003_RETURN(functName, signed short int, -R_LFREEE)
                           }/* hCallingAddress send by transport is not used. */
/********************
   Note : 
  (1) During REDIRECTind, state and event is externally set to trigger 
      ro0845_STA01CxCN transaction function of automata table.
  (2) Since STA01xTCONind function is not triggered before invoking 
      ro0845_STA01CxCN function , all the flags of pContext block must
      be externally set correctly to correspond those in RC08d3_STA01C 
      state. 
***********************/   
                                   break;  
         
  case RC0801_SCONNECTreq:         in_event=RC08e4_SCNreq;  
                                   pContext->usUserConnExists = TRUE;
                                   break;
  case RC0803_SCONNECTrsp:         if ((ret = ulP4) == RCro80_Accept)
                                   { 
                                     in_event=RC08e5_SCRpos;  
                                     pContext->ulConectresult =ulP4;     
  				   }
                                   else        
                                   {
                                     if (ret == ERROR)
                                       in_event =ERROR;       
                                     else {
                                       in_event=RC08e6_SCRneg;       
                                       pContext->ulConectresult = ulP4;
                      			}
                                   }
                                   break;
  case RC0805_SRELEASEreq:         in_event=RC08ec_SRLreq;            break;
  case RC0807_SRELEASErsp:         if (ulP4 == RCro90_Affirmative)
                                     in_event=RC08ed_SRRpos;  
                                   else
                                     in_event=RC08ee_SRRneg;    
                                   pContext->usUserConnExists = FALSE;
                                   break;
  case RC0809_SUABORTreq:          in_event=RC08ek_SUAreq;
                                   pContext->usUserConnExists = FALSE;
                                   break;
  case RC080c_SDATAreq:            in_event=RC08el_SDTreq;            break;
  default:                         in_event=RC08eo_BadEvent;         break;
  }

/* ----------------------------------------------------------------- */
/* PROTOCOL MACHINE  ACTIVITY LOGGING                                */
/* ----------------------------------------------------------------- */
 
  hACB = ro0214_getAcbEcb(&hECB);

  ro0043_pack10Log (RC0022_ACTIVITY,
                    RL0801_ActivitySpm,
                    10,
	   (int)    sizeof (hECB),
    (char FAR *)    &hECB,

	   (int)    sizeof (hACB),
    (char FAR *)    &hACB,

	   (int)    sizeof (event),
    (char FAR *)    &event,

    (int)           sizeof (ulP1),  
    (char FAR *)    &ulP1,

    (int)    	    sizeof (ulP2),
    (char FAR *)    &ulP2, 

    (int)           sizeof (ulP3),
    (char FAR *)    &ulP3,

    (int)           sizeof (ulP4),
    (char FAR *)    &ulP4, 

    (int)           sizeof (in_event),
    (char FAR *)    &in_event,

    (int)           sizeof (rT0870_stSpmContext),
    (char FAR *)    pContext,

    (int)           sizeof (pContext->vCalledAddrs),
    (char FAR *)    pContext->vCalledAddrs );

/* ------------------------------------------------------------------ */
/* Error Discovery                                                    */
/* ------------------------------------------------------------------ */
  if ((in_event == RC08eo_BadEvent) || (in_event == ERROR)) 
  {                                     /* Type of SPDU unknown   */
                                        /* false d_state          */

     sRetCode = ro0831_InvalidSpdu(pContext,ulP1,RC0831_MaxParamLen,NULL,NULL);
     if (sRetCode != OK)
         RM0003_RETURN(functName, signed short int, sRetCode)
  }
  else
  {
    if ( (in_event > RC08dy_MaxInEvents)  || (pContext->usState > RC08dz_MaxStates) )
        RM0003_RETURN(functName, signed short int, -R_SUNKEV)
    else
    {
/* ------------------------------------------------------------------ */
/* If a correct event:                                                */
/*                                                                    */
/*               S T A T E  -  E V E N T  - M A T R I X               */
/* ------------------------------------------------------------------ */
    if (in_event != RC08en_NoEvent)
      sRetCode = (*sematrix[pContext->usState][in_event]) (
                 pContext,            /* <-> PM context ( pointer to ) */ 
                 ulP1,                /*  Event Parameter              */
                 ulP2,
                 ulP3,
                 ulP4 );
      if(sRetCode)                         /* Error in transaction function */
        RM0003_RETURN(functName, signed short int, sRetCode)
    }
  }

/* ------------------------------------------------------------------ */
/* Step 4 from PM-generic: unlock PM context                          */
/*                 						      */
/* Preparing the end of the machine:                                  */
/*       - free the context                                           */
/*       - unregister the machine at the dispatcher                   */
/* ------------------------------------------------------------------ */

  if (pContext->usState == RC08d0_STA01)  
  {
    if (pContext->hAddrs != NULL)
       if(ro0034_localfree(pContext->hAddrs) != NULL)
          RM0003_RETURN(functName, signed short int, -R_LFREEE)
  
    if (pContext->hCalledAddress != NULL)
       if(ro0034_localfree(pContext->hCalledAddress) != NULL)
          RM0003_RETURN(functName, signed short int, -R_LFREEE)
  
    if (pContext->hUserData != NULL )
    {
      if(ro0075_deletestore(pContext->hUserData) != OK)
              RM0003_RETURN(functName, signed short int, -R_FSTERR)
      pContext->hUserData  = NULL;
      pContext->ulOffset   = NULL;
    }  

    if(ro0032_localunlock(hContext) != NULL)   /* free context       */
          RM0003_RETURN(functName, signed short int, -R_LULOKE)

    if(ro0034_localfree(hContext) != NULL)
          RM0003_RETURN(functName, signed short int, -R_LFREEE)

    ro0213_regPmDestr ();                     /* unregister the machine  */
    
  }
  else                   /* normal end of the machine                     */
    if ( ro0032_localunlock(hContext) != NULL)
      RM0003_RETURN(functName, signed short int, - R_LULOKE)

  /*----------------------------------------------------------------------*/ 
  /* Step 5 from PM-generic: synchronous return code                      */ 
  /*----------------------------------------------------------------------*/ 

  RM0003_RETURN(functName, signed short int, sRetCode)
}/*end-ro0800_spm*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro0802_informSpm                                            */
/*                                                                            */
/* AUTHOR       : Milind Madhav Aphale.                                       */
/* DATE         : 93-03-08                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int FAR PASCAL ro0802_informSpm (
unsigned long  int      hContext,        
signed   short int      sInformType,           /*  -> : see rospmz0.h */
void           FAR     *pRequestedInfo)        /* <-> : see rospmz0.h */

/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Provides information on the status of SPM registered in the provided   */
/*     context block.                                                         */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     hContext              = SPM context block (handle of).                 */
/*                                                                            */
/*     sInformType           = Type of information requested.                 */
/*                             Possible values are :                          */
/*                             - RC08z1_InformStoreOffset.                    */
/*                                                                            */
/*     pRequestedInfo        = Pointer to memory containing the data          */
/*                             structure associated with sInformType that is  */
/*                             TO BE FILLED with the requested info.          */
/*                                                                            */
/* FORMAL OUT-PARAMETERS :                                                    */
/*                                                                            */
/*     pRequestedInfo        = Pointer to memory containing the data          */
/*                             structure associated with sInformType that is  */
/*                             FILLED with the requested info.                */
/*                                                                            */
/*                                                                            */
/* PUSHED OUT-PARAMETERS : None.                                              */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*      = 0  SUCES     successfully                                           */
/*      < 0 (SUCES)    unrecoverable internal error                           */
/*                                                                            */
/* GLOBAL DATA (read only) : None.                                            */
/*                                                                            */
/* GLOBAL DATA (modified)  : None.                                            */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro0802_informSpm*/ 

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed   short int          sRetCode = OK;

  RM0001_STRG(functName,"ro0802_informSpm")
  RM0002_ENTRY(functName)

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

switch (sInformType) {

/*----------------------------------------------------------------------------*/
/* Information on associations requested.                                     */
/*----------------------------------------------------------------------------*/

case RC08z1_InformStoreOffset : 
    { 
      rT08z0_ulStoreOffsetLen FAR  *pStoreOffsetLen;
      pStoreOffsetLen = (rT08z0_ulStoreOffsetLen FAR *) pRequestedInfo;
      *pStoreOffsetLen = RC0831_MaxParamLen;   
      break;
    } 

default : 
    {
      sRetCode =  - R_SINFER;
      break;
    }

}/*switch-end*/

RM0003_RETURN (functName, signed short int, sRetCode)

}/*end-ro0802_informSpm*/ 


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro080a_StoreOffset                                          */
/*                                                                            */
/* AUTHOR       : Milind Madhav Aphale.                                       */
/* DATE         : 93-06-04                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed long int FAR PASCAL ro080a_StoreOffset(
signed long int levent,
unsigned long int hStore,  
unsigned long int ulOffset)

/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Returns correct offset required for store operations.                  */
/*                                                                            */
/* IN-PARAMETERS          :                                                   */
/*                                                                            */
/*     levent          = event number.                                        */
/*     hStore          = Handle to store                                      */
/*     ulOffset        = Offset to store from where encoding/decoding starts  */
/*                                                                            */
/* FORMAL OUT-PARAMETERS :  None.                                             */
/*                                                                            */
/* PUSHED OUT-PARAMETERS :  None.                                             */
/*                                                                            */
/* RETURN VALUES         :                                                    */
/*                                                                            */
/*   >  0  SUCES     successfully                                             */
/*   <= 0            unrecoverable internal error                             */
/*                                                                            */
/*   If ulOffset != 0  returns ulOffset                                       */
/*   else returns minimum of storelen or RC0831_MaxParamLen                   */
/*   If there is any error in finding store length negative value is          */
/*   returned.                                                                */
/*                                                                            */
/******************************************************************************/
{/*entr-ro080a_StoreOffset*/

signed long int lStorelen;

  RM0001_STRG(functName,"ro080a_StoreOffset")
  RM0002_ENTRY(functName)

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */
   
/* To be introduced for future expansion in Inform function
    switch(levent)
    {
     default : break;
    }
 */

    if (!ulOffset) {
        lStorelen = ro0062_storelength(hStore);                
        ulOffset = ((lStorelen < LNULL) ? -R_FSTERR
                                    : MIN (RC0831_MaxParamLen , lStorelen));
    }
    RM0003_RETURN(functName, signed long int, (signed long int)ulOffset)

}/*end-ro080a_StoreOffset*/
/*EOF*/
