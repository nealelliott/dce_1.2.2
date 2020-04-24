/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm03.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:30  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:41:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:23  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:43  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:33  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:47  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:14  keutel
 * 	creation
 * 	[1994/03/21  13:33:14  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROSPM03_H
#define _ROSPM03_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rospm03_rcsid[] = { "@(#) $RCSfile: rospm03.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:30 $" } ;
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
/* NAME         : rospm03.h                                           */ 
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-02                                            */
/*                                                                    */
/* COMPONENT    : RTROS convergence protocol machine                  */
/*                                                                    */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : This modul provides the defines for the             */
/*                convergence protocol machine.                       */
/*                Events and States defined in this file are used     */
/*                in SPM automata table.                              */
/*  ( Note :- Addition / deletion / updation of any event or state    */
/*            value must be done with proper care as it affects       */
/*            PM automata table )                                     */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#) rospm03.h 1.1 93/11/29 RTROS-V3.00              */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-04-02|  original                      |ma  |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*********************************************************************/
/*                                                                   */
/*              D E F I N E S                                        */
/*                                                                   */
/*********************************************************************/

#define RC08dz_MaxStates  13

#define RC08dy_MaxInEvents  24

/*------------------------------------------*/
/* States:                                  */
/*------------------------------------------*/

#define RC08d0_STA01    0
#define RC08d1_STA01A   1
#define RC08d2_STA01B   2
#define RC08d3_STA01C   3
#define RC08d4_STA01D   4
#define RC08d5_STA01E   5         /* not applied  */
#define RC08d6_STA02A   6
#define RC08d7_STA02B   7
#define RC08d8_STA03    8
#define RC08d9_STA08    9 
#define RC08da_STA09    10
#define RC08db_STA16    11
#define RC08dc_STA713   12

/*------------------------------------------*/
/* Events:                                  */
/*------------------------------------------*/

#define RC08e0_TCNind    0
#define RC08e1_TCNcnf    1
#define RC08e2_TDNind    2
#define RC08e3_TCONT     3
#define RC08e4_SCNreq    4
#define RC08e5_SCRpos    5
#define RC08e6_SCRneg    6
#define RC08e7_ACN       7
#define RC08e8_ACDO      8
#define RC08e9_AOA       9
#define RC08ea_AAC      10
#define RC08eb_ARF      11
#define RC08ec_SRLreq   12
#define RC08ed_SRRpos   13
#define RC08ee_SRRneg   14
#define RC08ef_AFN      15
#define RC08eg_AAA      16
#define RC08eh_ANF      17
#define RC08ei_AAB      18
#define RC08ej_ADN      19
#define RC08ek_SUAreq   20
#define RC08el_SDTreq   21
#define RC08em_ADT      22
#define RC08en_NoEvent  23
#define RC08eo_BadEvent 24

#endif	/* _ROSPM03_H */
