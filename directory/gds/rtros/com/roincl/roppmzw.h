/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppmzw.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:08  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:06  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:36  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:35  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:59  keutel
 * 	creation
 * 	[1994/03/21  13:19:22  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPMZW_H
#define _ROPPMZW_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppmzw_rcsid[] = { "@(#) $RCSfile: roppmzw.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:56 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1991                     */
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
/* NAME         : roppmzw.h                                           */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE         : 93-06-19                                            */
/*                                                                    */
/* COMPONENT    : RTROS presentation protocol machine                 */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : Encode/Decode    Private Transfer Syntax:           */
/*                function prototypes.                                */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/*datoff **************************************************************/


signed short int PASCAL CPSniP_type_cod PROTO ((
unsigned long  int        hStore,
rT041A_stCp        FAR  * x,
rT0430_stContext   NEAR * pContext));

signed short int PASCAL CPASniP_type_cod PROTO ((
unsigned long  int        hStore,
rT041B_stCpa       FAR  * x,
rT0430_stContext   NEAR * pContext));

signed short int PASCAL CPRSniP_type_cod PROTO ((
unsigned long  int        hStore,
rT041C_stCpr       FAR  * x,
rT0430_stContext   NEAR * pContext));

signed short int PASCAL ABUSniP_type_cod PROTO ((
unsigned long  int        hStore,
rT041D_stAru       FAR  * x,
rT0430_stContext   NEAR * pContext));

signed short int PASCAL UserSniP_type_cod PROTO ((
unsigned long  int        hStore,
rT041F_stOther     FAR  * x,
rT0430_stContext   NEAR * pContext,
signed short int   FAR  * psPmNumb));

signed short int PASCAL ABPSniP_type_cod PROTO ((
unsigned long  int        hStore,
rT041E_stArp       FAR  * x,
rT0430_stContext   NEAR * pContext));

signed short int PASCAL CPSniP_type_dec PROTO ((
unsigned long int         hStore,
rT041A_stCp        FAR  * x,
rT0430_stContext   NEAR * pContext));

signed short int PASCAL CPASniP_type_dec PROTO ((
unsigned long int         hStore,
rT041B_stCpa       FAR  * x,
rT0430_stContext   NEAR * pContext));

signed short int PASCAL CPRSniP_type_dec PROTO ((
unsigned long int         hStore,
rT041C_stCpr       FAR  * x,
rT0430_stContext   NEAR * pContext));

signed short int PASCAL ABUSniP_type_dec PROTO ((
unsigned long int         hStore,
rT041D_stAru       FAR  * x,
rT0430_stContext   NEAR * pContext));

signed short int PASCAL UserSniP_type_dec PROTO ((
unsigned long int         hStore,
rT041F_stOther     FAR  * x,
rT0430_stContext   NEAR * pContext));

signed short int PASCAL ABPSniP_type_dec PROTO ((
unsigned long int         hStore,
rT041E_stArp       FAR  * x,
rT0430_stContext   NEAR * pContext));

signed short int PASCAL ro04y0_CheckSniPTS PROTO ((
unsigned long int        hStore,
rT0430_stContext  NEAR * pContext));

signed short int PASCAL ro04y1_routeSnixAutoNo PROTO ((
signed short int  FAR  * pRoute));

#endif	/* _ROPPMZW_H */
