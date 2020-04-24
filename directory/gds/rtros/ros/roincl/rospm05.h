/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm05.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:32  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:41:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:24  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:47  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:34  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:41  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:49  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:15  keutel
 * 	creation
 * 	[1994/03/21  13:33:16  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROSPM05_H
#define _ROSPM05_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rospm05_rcsid[] = { "@(#) $RCSfile: rospm05.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:32 $" } ;
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
/* NAME         : rospm05.h                                           */
/*                                                                    */
/* AUTHOR       : Milind M.Aphale                                     */
/* DATE         : 93-04-05                                            */
/*                                                                    */
/* COMPONENT    : RTROS convergence protocol machine                  */
/*                                                                    */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : This modul provides the function prototypes of      */
/*                the coding functions                                */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :  sccsid  =  @(#) rospm05.h 1.1 93/11/29 RTROS-V3.00             */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-04-05|  original                      |ma  |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*********************************************************************/
/*                                                                   */
/*              F U N C T I O N  P R O T O T Y P E S                 */
/*                                                                   */
/*********************************************************************/

signed short int ro0810_EncodeAA   PROTO ((rT0810_stAA FAR *)); 
signed short int ro0820_DecodeAA   PROTO ((rT0810_stAA FAR *));

signed short int ro0811_EncodeAB  PROTO ((rT0811_stAB FAR *));
signed short int ro0821_DecodeAB  PROTO ((rT0811_stAB FAR *));

signed short int ro0812_EncodeAC  PROTO ((rT0812_stAC FAR *));
signed short int ro0822_DecodeAC  PROTO ((rT0812_stAC FAR *));

signed short int ro0814_EncodeCN  PROTO ((rT0814_stCN FAR *));
signed short int ro0824_DecodeCN  PROTO ((rT0814_stCN FAR *));

signed short int ro0815_EncodeDN  PROTO ((rT0815_stDN FAR *));
signed short int ro0825_DecodeDN  PROTO ((rT0815_stDN FAR *));

signed short int ro0816_EncodeDT  PROTO ((rT0816_stDT FAR *));
signed short int ro0826_DecodeDT  PROTO ((rT0816_stDT FAR *));

signed short int ro0817_EncodeFN  PROTO ((rT0817_stFN FAR *));
signed short int ro0827_DecodeFN  PROTO ((rT0817_stFN FAR *));

signed short int ro0819_EncodeRF  PROTO ((rT0819_stRF FAR *));
signed short int ro0829_DecodeRF  PROTO ((rT0819_stRF FAR *));

/**********
 Not impemented in RTROS V-3.00 K00

signed short int ro0818_EncodeOA PROTO ((void));
signed short int ro0828_DecodeOA PROTO ((void));

signed short int ro0813_EncodeCDO PROTO ((void));
signed short int ro0823_DecodeCDO PROTO ((void));
**********/

#endif	/* _ROSPM05_H */
