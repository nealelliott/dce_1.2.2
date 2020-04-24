/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil1.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:25  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:37:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:34  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:11  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:52  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:47  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:19  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:02  keutel
 * 	creation
 * 	[1994/03/21  13:28:40  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUTIL1_H
#define _ROUTIL1_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _routil1_rcsid[] = { "@(#) $RCSfile: routil1.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:25 $" } ;
#endif

/* *********************************************************************** */
/*                                                                         */
/*  * COPYRIGHT  (C)                                                       */
/*  * Siemens Nixdorf Informationssysteme AG 1992                          */
/*  * All rights reserved                                                  */
/*  *                                                                      */
/*  * This software is furnished under licence and may be used only        */
/*  * in accordance with the terms of that licence and with the            */
/*  * inclusion of the above copyright notice.                             */
/*  * This software may not be provided or otherwise made available        */
/*  * to, or used by, any other person. No title to or ownership of        */
/*  * the software is hereby  transferred.                                 */
/*                                                                         */
/* *********************************************************************** */

/*daton ********************************************************************/
/*                                                                         */
/* TYPE         : INCLUDE                                                  */
/*                                                                         */
/* NAME         : routil1.h                                                */
/*                                                                         */
/* AUTHOR       : Johann Mueller, Michael Rippstain                        */
/* DATE         : 92-02-19                                                 */
/*                                                                         */
/* COMPONENT    : RTS common utilities  external interface                 */
/*                                                                         */
/* PRD#/VERS.   : RTS-V02.00                                               */
/*                                                                         */
/* DESCRIPTION  : RTS common utilities  external interface                 */
/*                                                                         */
/*                - Object identifier value processing facility            */
/*                - Application Handling functions                         */
/*                                                                         */
/* SYSTEM DEPENDENCE :                                                     */
/*                                                                         */
/* HISTORY      : sccsid  = "@(#)routil1.h  1.1 93/11/29"                              */
/*                                                                         */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM#      */
/* 02.00    | 92-02-19|  original                      | gj |              */
/* 02.00    | 92-04-21|  the roxxxx_xxAETitle are added| mr |              */
/* 03.00    | 93-04-09|  roxxxx_AplCxtCmpi added       | kc |              */
/* 03.00    | 93-04-09|  MoveSend and MoveReceive added| vp |              */
/*          |         |                                |    |              */
/*datoff *******************************************************************/

/* *************************************************************************/
/*            Object identifier value processing facility                  */
/*                        (routil0.c)                                      */
/* *************************************************************************/


/* ****************** Old form API *************************************** */

signed short int PASCAL ro0000_objcpy PROTO ((unsigned long int FAR *,
                                              unsigned long int FAR *,
					      unsigned long int       ));
/* Return value = 0 :  successfull                       */
/*              < 0 :  error                             */
/*              > 0 :  not defined, length equal to NULL */
/* Parameter 1      :  target                            */
/* Parameter 2      :  source                            */
/* Parameter 3      :  length                            */

signed short int PASCAL ro0001_objcmp PROTO ((unsigned long int      ,
                                              unsigned long int FAR *,
					      unsigned long int      ,
                                              unsigned long int FAR * ));
/* Return value = 0  : if equal     */
/*             <> 0  : if not equal */
/* Parameter 1       : length1      */
/* Parameter 2       : value 1      */
/* Parameter 3       : length2      */
/* Parameter 4       : value 2      */


/* ****************** New form API *************************************** */

signed short int PASCAL ro0002_objcpy PROTO ((unsigned long int FAR *,
					      unsigned long int FAR * ));
/* Return value = 0 :  successfull                       */
/*              < 0 :  error                             */
/*              > 0 :  not defined, length equal to NULL */
/* Parameter 1      :  target                            */
/* Parameter 2      :  source                            */


signed short int PASCAL ro0003_objcmp PROTO ((unsigned long int FAR *,
					      unsigned long int FAR * ));
/* Return value = 0  : if equal     */
/*              < 0 :  error        */
/*              > 0  : if not equal */
/* Parameter 1       : value 1      */
/* Parameter 2       : value 2      */


/* *************************************************************************/
/*            Application Entity Invocation (AE-Invoc-Id)                  */
/*                 processing facility (routil0.c)                         */
/* *************************************************************************/

signed short int PASCAL
ro0004_cmpApplInvocation PROTO ((rTro74_stInvoc FAR *,
				 rTro74_stInvoc FAR * ));
/* Return value = 0  : if equal                                */
/*              < 0 :  error                                   */
/*              > 0  : if not equal                            */
/* Parameter 1       : pointer to AE-Invoc-Id1 (see rouply0.h) */
/* Parameter 2       : pointer to AE-Invoc-Id2 (see rouply0.h) */

void PASCAL
ro0005_cpyApplInvocation PROTO ((rTro74_stInvoc FAR *,
				 rTro74_stInvoc FAR * ));
/* Parameter 1       : pointer to AE-Invoc-Id target (see rouply0.h) */
/* Parameter 2       : pointer to AE-Invoc-Id source (see rouply0.h) */


/* *************************************************************************/
/*            Application Entity Title (AE-Title) processing facility      */
/*                          (routil0.c)                                    */
/* *************************************************************************/

signed short int PASCAL
ro0006_cmpAETitle PROTO ((unsigned long int,
			  unsigned long int ));
/* Return value = 0  : if equal                            */
/*              > 0  : if not equal                        */
/*              < 0  : if an error occurs                  */
/* Parameter 1       : Handle of AE-Title1 (see rouply0.h) */
/* Parameter 2       : Handle of AE-Title2 (see rouply0.h) */

signed short int PASCAL
ro0007_cpyAETitle PROTO ((unsigned long int FAR *,
			  unsigned long int       ));
/* Return value = 0  : if ok                                */
/*             != 0  : if an error occurs                   */
/* Parameter 1       : pointer to Handle of AE-Title target */
/*                     (see rouply0.h)                      */
/* Parameter 2       : Handle of AE-Title source            */
/*                     (see rouply0.h)                      */

signed short int PASCAL
ro0009_deleteAETitle PROTO ((unsigned long int FAR * ));
/* Return value = 0  : if ok                                         */
/*             != 0  : if an error occurs                            */
/* Parameter         : pointer to Handle of AE-Title (see rouply0.h) */

signed short int PASCAL
ro000a_lengthAETitle PROTO ((unsigned long int      ,
			     unsigned long int FAR * ));
/* Return value = 0  : if ok                                         */
/*             != 0  : if an error occurs                            */
/* Parameter 1       : Handle of AE-Title (see rouply0.h)            */
/* Parameter 2       : pointer to length of AE-Title (see rouply0.h) */


signed short int FAR PASCAL 
ro000c_AplCxtCmp PROTO ((
           signed    short int                ,
           signed    short int                ,
           unsigned  long  int                ,
           signed    short int      FAR     * ));

/* Return value = 0  : if ok                                         */
/*             != 0  : if an error occurs                            */
/* Parameter 1       : Appl Context Id                               */
/* Parameter 2       : Appl Context Id                               */
/* Parameter 3       : Handle for AE Invocation                      */
/* Parameter 4       : Pointer to the Boolean Output parameter       */

signed short int FAR PASCAL 
ro000d_MoveSend PROTO ((
           unsigned  long  int    FAR *   ,
           unsigned  long  int    FAR *    ));

/* Return value = 0  : if ok                                         */
/*             != 0  : if an error occurs                            */
/* Parameter 1       : User Info array                               */
/* Parameter 2       : Pointer to ul UserInfo (no. of externals)     */

signed short int FAR PASCAL 
ro000e_MoveReceive PROTO ((
           unsigned  long  int    FAR *   ,
           unsigned  long  int    FAR *    ));

/* Return value = 0  : if ok                                         */
/*             != 0  : if an error occurs                            */
/* Parameter 1       : User Info array                               */
/* Parameter 2       : Pointer to ul UserInfo (no. of externals)     */

#endif	/* _ROUTIL1_H */
