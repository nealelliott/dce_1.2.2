/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rolgval.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:45  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:41  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:34:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:25  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:41  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:17  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:18  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:27  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:26  keutel
 * 	creation
 * 	[1994/03/21  13:26:46  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROLGVAL_H
#define _ROLGVAL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rolgval_rcsid[] = { "@(#) $RCSfile: rolgval.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:45 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1991 BY SIEMENS NIXDORF INFORMATIONS AG         */
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
/* NAME         : rolgval.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 91-11-10                                            */
/*                                                                    */
/* COMPONENT    : RTS logging record type number definition           */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.00                                           */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* Two functions are available to the modules for writing onto the    */
/* log file:                                                          */
/* - ro0042_writeLog  (LoggingType, LogRecordType,                    */
/*                     SizeOfRecord1, PointerToRecord1)               */
/*                                                                    */
/* - ro0043_pack10Log (LoggingType, LogRecordType, NumbOfParams,      */
/*                                  SizeOfRecord1, PointerToRecord1,  */
/*                                                 .                  */
/*                                                 .                  */
/*                                  SizeOfRecordN, PointerToRecordN)  */
/*   (1 <= N <= 10)                                                   */
/*                                                                    */
/* LoggingType : RC0020_INTERFACE (For logging of external interface) */
/*               RC0021_TRACE                                         */
/*               RC0022_ACTIVITY  (For logging of PM modules etc.)    */
/*               RC0023_CWAITTRACE                                    */
/*                                                                    */
/* LogRecordType: Has the following format :                          */
/* #define RLaabb_text (PhysicalType | ModuleIdentificator            */
/*                                   | IncrementalValue)              */
/* - aa : Module number (e.g. RTPM = 10)                              */
/* - bb : Incremental value (e.g. first value 00, second value 01).   */
/* - text : Textual description of record.                            */
/* - PhysicalType :                                                   */
/*       RLro00_SimpleRecord => to use with ro0042_writeLog ()        */
/*       RLro01_PackedRecord => to use with ro0043_pack10Log ()       */
/* - ModuleIdentificator :                                            */
/*       That defined for RLaa00_<text> for the respective module     */
/* - IncrementalValue :                                               */
/*       For differentiating various definitions for the module.      */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      : sccsid  = "@(#)rolgval.h  1.3 94/01/19"                         */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/* 02.00 K0 | 91-11-10|  original                      | jm |         */
/*          | 92-01-28|  acse values added             | mr |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** Modul independent definitions ***************** */
/* ******************                               ***************** */

#define RLro00_SimpleRecord    (0x80000000L)
#define RLro01_PackedRecord    (0x81000000L)

#define RLro10_ModulNumber     (0x00ff0000L)

#define RLro20_TypeField       (0x0000ffffL)

#define RLro20_FIELDERR         0xffff

/* ****************** RTS utility  modul **************************** */
/* ******************                    **************************** */

#define RL0000_ROUTIL          (0x00000000L)

#define RL0001_FctTraceEntry   (RLro00_SimpleRecord|RL0000_ROUTIL|0x0000L)
#define RL0002_FctTraceReturn  (RLro01_PackedRecord|RL0000_ROUTIL|0x0001L)

/* ****************** RTS external interface modul ****************** */
/* ******************                              ****************** */

#define RL0100_ROEXIF           (0x00010000L)

#define RL0101_EnEVENT          (RLro01_PackedRecord|RL0100_ROEXIF|0x0001L)
#define RL0102_ReEVENT          (RLro01_PackedRecord|RL0100_ROEXIF|0x0002L)
#define RL0103_EnATTACH         (RLro01_PackedRecord|RL0100_ROEXIF|0x0003L)
#define RL0104_ReATTACH         (RLro01_PackedRecord|RL0100_ROEXIF|0x0004L)
#define RL0105_EnDETACH         (RLro01_PackedRecord|RL0100_ROEXIF|0x0005L)
#define RL0106_ReDETACH         (RLro00_SimpleRecord|RL0100_ROEXIF|0x0006L)

#define RL0107_EnOPENreq        (RLro01_PackedRecord|RL0100_ROEXIF|0x0007L)
#define RL0108_ReOPENreq        (RLro01_PackedRecord|RL0100_ROEXIF|0x0008L)
#define RL0109_EnOPENrsp        (RLro01_PackedRecord|RL0100_ROEXIF|0x0009L)
#define RL0110_ReOPENrsp        (RLro00_SimpleRecord|RL0100_ROEXIF|0x0010L)
#define RL0111_ReOPENind        (RLro01_PackedRecord|RL0100_ROEXIF|0x0011L)
#define RL0112_ReOPENcnf        (RLro01_PackedRecord|RL0100_ROEXIF|0x0012L)

#define RL0113_EnTRANSreq       (RLro01_PackedRecord|RL0100_ROEXIF|0x0013L)
#define RL0114_ReTRANSreq       (RLro00_SimpleRecord|RL0100_ROEXIF|0x0014L)
#define RL0115_ReTRANSind       (RLro01_PackedRecord|RL0100_ROEXIF|0x0015L)
#define RL0116_ReTRANScnf       (RLro01_PackedRecord|RL0100_ROEXIF|0x0016L)

#define RL0117_EnCLOSEreq       (RLro01_PackedRecord|RL0100_ROEXIF|0x0017L)
#define RL0118_ReCLOSEreq       (RLro00_SimpleRecord|RL0100_ROEXIF|0x0018L)
#define RL0119_EnCLOSErsp       (RLro01_PackedRecord|RL0100_ROEXIF|0x0019L)
#define RL011a_ReCLOSErsp       (RLro00_SimpleRecord|RL0100_ROEXIF|0x001aL)
#define RL011b_ReCLOSEind       (RLro01_PackedRecord|RL0100_ROEXIF|0x001bL)
#define RL011c_ReCLOSEcnf       (RLro01_PackedRecord|RL0100_ROEXIF|0x001cL)

#define RL011d_EnUABORTreq      (RLro01_PackedRecord|RL0100_ROEXIF|0x001dL)
#define RL011e_ReUABORTreq      (RLro00_SimpleRecord|RL0100_ROEXIF|0x001eL)
#define RL0120_ReUABORTind      (RLro01_PackedRecord|RL0100_ROEXIF|0x0020L)
#define RL0121_RePABORTind      (RLro01_PackedRecord|RL0100_ROEXIF|0x0021L)

#define RL0122_EnPROVBUFER      (RLro01_PackedRecord|RL0100_ROEXIF|0x0022L)
#define RL0123_RePROVBUFER      (RLro00_SimpleRecord|RL0100_ROEXIF|0x0023L)

#define RL0124_EnPLEASEreq      (RLro01_PackedRecord|RL0100_ROEXIF|0x0024L)
#define RL0125_RePLEASEreq      (RLro00_SimpleRecord|RL0100_ROEXIF|0x0025L)
#define RL0126_EnGIVEreq        (RLro01_PackedRecord|RL0100_ROEXIF|0x0026L)
#define RL0127_ReGIVEreq        (RLro00_SimpleRecord|RL0100_ROEXIF|0x0027L)
#define RL0128_RePLEASEind      (RLro01_PackedRecord|RL0100_ROEXIF|0x0028L)
#define RL0129_ReGIVEind        (RLro00_SimpleRecord|RL0100_ROEXIF|0x0029L)

#define RL012a_EnSetRECOVERY    (RLro01_PackedRecord|RL0100_ROEXIF|0x002aL)
#define RL012b_EnINFORM         (RLro01_PackedRecord|RL0100_ROEXIF|0x002bL)
#define RL012c_EnRECOVERY       (RLro00_SimpleRecord|RL0100_ROEXIF|0x002cL)

#define RL0130_ReSysINFORM      (RLro01_PackedRecord|RL0100_ROEXIF|0x0030L)
#define RL0131_ReIdfINFORM      (RLro01_PackedRecord|RL0100_ROEXIF|0x0031L)
#define RL0132_ReAplINFORM      (RLro01_PackedRecord|RL0100_ROEXIF|0x0032L)
#define RL0133_ReAssINFORM      (RLro01_PackedRecord|RL0100_ROEXIF|0x0033L)

#define RL0134_ReINFORMerr      (RLro00_SimpleRecord|RL0100_ROEXIF|0x0034L)
#define RL0135_ReSetRECOVERY    (RLro00_SimpleRecord|RL0100_ROEXIF|0x0035L)
#define RL0136_ReRECOVERY       (RLro00_SimpleRecord|RL0100_ROEXIF|0x0036L)

#define RL0137_ReOPENindErr     (RLro01_PackedRecord|RL0100_ROEXIF|0x0037L)
#define RL0138_ReOPENcnfErr     (RLro01_PackedRecord|RL0100_ROEXIF|0x0038L)
#define RL0139_EnOPENrsp88      (RLro01_PackedRecord|RL0100_ROEXIF|0x0039L)


/* ****************** RTS dispatcher modul ************************** */
/* ******************                      ************************** */

#define RL0200_RODPAT           (0x00020000L)

/* ****************** RTS lower interface modul ********************* */
/* ******************                           ********************* */

#define RL0300_ROLOIF           (0x00030000L)
#define RL0301_ActivityLoif     (RLro01_PackedRecord|RL0300_ROLOIF|0x0001L)

/* ****************** RTS P-PM modul ******************************** */
/* ******************                ******************************** */

#define RL0400_ROPRPM           (0x00040000L)
#define RL0401_ActivityPpm      (RLro01_PackedRecord|RL0400_ROPRPM|0x0001L)
#define RL0402_PduLog           (RLro00_SimpleRecord|RL0400_ROPRPM|0x0002L)
#define RL0403_PduInfo          (RLro01_PackedRecord|RL0400_ROPRPM|0x0003L)
#define RL0404_PcxtInfo         (RLro01_PackedRecord|RL0400_ROPRPM|0x0004L)
#define RL0405_EncodeInfo       (RLro01_PackedRecord|RL0400_ROPRPM|0x0005L)
#define RL0406_DecodeInfo       (RLro01_PackedRecord|RL0400_ROPRPM|0x0006L)
#define RL0407_OffsetInfo       (RLro00_SimpleRecord|RL0400_ROPRPM|0x0007L)

/* ****************** RTROS Session-PM module   ********************* */
/* ******************                           ********************* */
                                                                        
#define RL0800_ROSEPM           (0x00080000L)                           
#define RL0801_ActivitySpm      (RLro01_PackedRecord|RL0800_ROSEPM|0x0001L)

/* ****************** RTROS Lower Control  **************************** */
/* ******************                    **************************** */

#define RL0500_ROLCTR           (0x00050000L)
#define RL0501_ActivityLctr    (RLro01_PackedRecord|RL0500_ROLCTR|0x0001L)
/* ****************** RTS AC-PM modul ******************************* */
/* ******************                 ******************************* */

#define RL0600_ROACPM           (0x00060000L)
#define RL0601_ActivityAcpm     (RLro01_PackedRecord|RL0600_ROACPM|0x0001L)

/* ****************** ROS RO-PM modul ******************************* */
/* ******************                 ******************************* */

#define RL0700_ROROPM           (0x00070000L)
#define RL0701_ActivityRopm     (RLro01_PackedRecord|RL0700_ROROPM|0x0001L)

/* ****************** RTS RT-PM modul ******************************* */
/* ******************                 ******************************* */

#define RL1000_RORTPM           (0x000a0000L)
#define RL1001_PmFnctValues     (RLro01_PackedRecord|RL1000_RORTPM|0x0001L)

/* ****************** RTS Timer-PM modul **************************** */
/* ******************                    **************************** */

#define RL1100_ROTIME           (0x000b0000L)
#define RL1101_ActivityTimer    (RLro01_PackedRecord|RL1100_ROTIME|0x0001L)
/* ****************** ROS external interface modul ****************** */
/* ******************                              ****************** */

#define RL1300_ROEXIF           (0x000d0000L)

#define RL1301_EnEVENT          (RLro01_PackedRecord|RL1300_ROEXIF|0x0001L)
#define RL1302_ReEVENT          (RLro01_PackedRecord|RL1300_ROEXIF|0x0002L)
#define RL1303_EnATTACH         (RLro01_PackedRecord|RL1300_ROEXIF|0x0003L)
#define RL1304_ReATTACH         (RLro01_PackedRecord|RL1300_ROEXIF|0x0004L)
#define RL1305_EnDETACH         (RLro00_SimpleRecord|RL1300_ROEXIF|0x0005L)
#define RL1306_ReDETACH         (RLro00_SimpleRecord|RL1300_ROEXIF|0x0006L)

#define RL1307_EnBINDreq        (RLro01_PackedRecord|RL1300_ROEXIF|0x0010L)
#define RL1308_ReBINDreq        (RLro01_PackedRecord|RL1300_ROEXIF|0x0011L)
#define RL1309_EnBINDrsp        (RLro01_PackedRecord|RL1300_ROEXIF|0x0012L)
#define RL1310_ReBINDrsp        (RLro00_SimpleRecord|RL1300_ROEXIF|0x0013L)
#define RL1311_ReBINDind        (RLro01_PackedRecord|RL1300_ROEXIF|0x0014L)
#define RL1312_ReBINDcnf        (RLro01_PackedRecord|RL1300_ROEXIF|0x0015L)

#define RL1313_EnINVreq         (RLro01_PackedRecord|RL1300_ROEXIF|0x0020L)
#define RL1314_ReINVind         (RLro01_PackedRecord|RL1300_ROEXIF|0x0021L)

#define RL1315_EnRESreq         (RLro01_PackedRecord|RL1300_ROEXIF|0x0022L)
#define RL1316_ReRESind         (RLro01_PackedRecord|RL1300_ROEXIF|0x0023L)

#define RL1317_EnERRreq         (RLro01_PackedRecord|RL1300_ROEXIF|0x0024L)
#define RL1318_ReERRind         (RLro01_PackedRecord|RL1300_ROEXIF|0x0025L)

#define RL1319_EnUREJreq         (RLro01_PackedRecord|RL1300_ROEXIF|0x0026L)
#define RL131a_ReUREJind         (RLro01_PackedRecord|RL1300_ROEXIF|0x0027L)

#define RL131b_RePREJind         (RLro01_PackedRecord|RL1300_ROEXIF|0x0028L)

#define RL131c_EnUBINDreq         (RLro01_PackedRecord|RL1300_ROEXIF|0x0016L)
#define RL131d_ReUBINDreq         (RLro00_SimpleRecord|RL1300_ROEXIF|0x0017L)
#define RL131e_EnUBINDrsp         (RLro01_PackedRecord|RL1300_ROEXIF|0x0018L)
#define RL131f_ReUBINDrsp         (RLro00_SimpleRecord|RL1300_ROEXIF|0x0019L)
#define RL131g_ReUBINDind         (RLro01_PackedRecord|RL1300_ROEXIF|0x001aL)
#define RL131h_ReUBINDcnf         (RLro01_PackedRecord|RL1300_ROEXIF|0x001bL)

#define RL131i_EnUABORTreq        (RLro01_PackedRecord|RL1300_ROEXIF|0x001cL)
#define RL131j_ReUABORTreq        (RLro00_SimpleRecord|RL1300_ROEXIF|0x001dL)
#define RL131k_ReUABORTind        (RLro01_PackedRecord|RL1300_ROEXIF|0x001eL)
#define RL131l_RePABORTind        (RLro01_PackedRecord|RL1300_ROEXIF|0x0029L)

#define RL131m_EnPROVBUFER        (RLro01_PackedRecord|RL1300_ROEXIF|0x0030L)
#define RL131n_RePROVBUFER        (RLro01_PackedRecord|RL1300_ROEXIF|0x0031L)


#define RL131o_EnWake             (RLro00_SimpleRecord|RL1300_ROEXIF|0x0032L)

#define RL131p_ReSysINFORM        (RLro01_PackedRecord|RL1300_ROEXIF|0x0033L)
#define RL131q_ReIdfINFORM        (RLro01_PackedRecord|RL1300_ROEXIF|0x0034L)
#define RL131r_ReAplINFORM        (RLro01_PackedRecord|RL1300_ROEXIF|0x0035L)
#define RL131s_ReAssINFORM        (RLro01_PackedRecord|RL1300_ROEXIF|0x0036L)

#define RL131t_InTraCwait         (RLro00_SimpleRecord|RL1300_ROEXIF|0x0037L)
#define RL131u_RnInform           (RLro01_PackedRecord|RL1300_ROEXIF|0x0038L)

#define RL131v_ReExit             (RLro00_SimpleRecord|RL1300_ROEXIF|0x0039L)
/* ****************** ROS   Upper Control  **************************** */
/* ******************                    **************************** */

#define RL1500_ROUCTR           (0x000f0000L)
#define RL1501_ActivityUctr    (RLro01_PackedRecord|RL1500_ROUCTR|0x0001L)

/* ****************** RTS   Upper Control  **************************** */
/* ******************                    **************************** */

#define RL1600_RTUCTR           (0x000f0000L)
#define RL1601_ActivityUctr    (RLro01_PackedRecord|RL1600_RTUCTR|0x0001L)

/* ****************** ROS Lower Interface Module********************* */
/* ****************** for Sockets Interface     ********************* */

#define RL1800_ROLOIF           (0x00130000L)
#define RL1801_ActivityLoif     (RLro01_PackedRecord|RL1800_ROLOIF|0x0001L)
#define RL1802_SockDataDump     (RLro01_PackedRecord|RL1800_ROLOIF|0x0002L)

/* ****************** ROS RFC-1006 Transport PM ********************* */
/* ******************                           ********************* */

#define RL1900_ROTRPM           (0x00140000L)
#define RL1901_ActivityTrpm     (RLro01_PackedRecord|RL1900_ROTRPM|0x0001L)

/* **************** CMX Interface ************************************** */
/* ****************               ************************************** */

#define RL2000_CMX              (0x00200000L)

#define RL2001_EnTAttach        (RLro01_PackedRecord|RL2000_CMX|0x0001L)
#define RL2002_ReTAttach        (RLro01_PackedRecord|RL2000_CMX|0x0002L)
#define RL2003_EnTDetach        (RLro00_SimpleRecord|RL2000_CMX|0x0003L)
#define RL2004_ReTDetach        (RLro00_SimpleRecord|RL2000_CMX|0x0004L)
#define RL2005_TERROR           (RLro00_SimpleRecord|RL2000_CMX|0x0005L)
#define RL2006_EnTEvent         (RLro01_PackedRecord|RL2000_CMX|0x0006L)
#define RL2007_ReTEvent         (RLro01_PackedRecord|RL2000_CMX|0x0007L)
#define RL2008_EnTdisrq         (RLro01_PackedRecord|RL2000_CMX|0x0008L)
#define RL2009_ReTdisrq         (RLro01_PackedRecord|RL2000_CMX|0x0009L)
#define RL200a_EnTconrq         (RLro01_PackedRecord|RL2000_CMX|0x000aL)
#define RL200b_ReTconrq         (RLro01_PackedRecord|RL2000_CMX|0x000bL)
#define RL200c_EnTredrq         (RLro01_PackedRecord|RL2000_CMX|0x000cL)
#define RL200d_ReTredrq         (RLro00_SimpleRecord|RL2000_CMX|0x000dL)
#define RL200e_EnTconrs         (RLro01_PackedRecord|RL2000_CMX|0x000eL)
#define RL200f_ReTconrs         (RLro00_SimpleRecord|RL2000_CMX|0x000fL)
#define RL2010_EnTinfo          (RLro01_PackedRecord|RL2000_CMX|0x0010L)
#define RL2011_ReTinfo          (RLro01_PackedRecord|RL2000_CMX|0x0011L)
#define RL2012_EnTdatarq        (RLro01_PackedRecord|RL2000_CMX|0x0012L)
#define RL2013_ReTdatarq        (RLro00_SimpleRecord|RL2000_CMX|0x0013L)
#define RL2014_EnTconin         (RLro01_PackedRecord|RL2000_CMX|0x0014L)
#define RL2015_ReTconin         (RLro01_PackedRecord|RL2000_CMX|0x0015L)
#define RL2016_EnTredin         (RLro01_PackedRecord|RL2000_CMX|0x0016L)
#define RL2017_ReTredin         (RLro01_PackedRecord|RL2000_CMX|0x0017L)
#define RL2018_EnTconcf         (RLro01_PackedRecord|RL2000_CMX|0x0018L)
#define RL2019_ReTconcf         (RLro01_PackedRecord|RL2000_CMX|0x0019L)
#define RL201a_EnTdisin         (RLro01_PackedRecord|RL2000_CMX|0x001aL)
#define RL201b_ReTdisin         (RLro01_PackedRecord|RL2000_CMX|0x001bL)
#define RL201c_EnTdatain        (RLro01_PackedRecord|RL2000_CMX|0x001cL)
#define RL201d_ReTdatain        (RLro01_PackedRecord|RL2000_CMX|0x001dL)

#endif	/* _ROLGVAL_H */
