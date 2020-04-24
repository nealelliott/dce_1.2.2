/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xmhdef.h,v $
 * Revision 1.1.21.2  1996/02/18  23:38:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:11  marty]
 *
 * Revision 1.1.21.1  1995/12/08  15:37:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:31  root]
 * 
 * Revision 1.1.19.2  1994/06/10  21:20:37  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:05  devsrc]
 * 
 * Revision 1.1.19.1  1994/02/22  19:00:01  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:46  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:03:59  marrek]
 * 
 * Revision 1.1.17.1  1993/10/14  19:23:54  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:22:50  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:54:22  keutel]
 * 
 * $EndLog$
 */
#ifndef _XMHDEF_H
#define _XMHDEF_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xmhdef_rcsid[] = "@(#)$RCSfile: xmhdef.h,v $ $Revision: 1.1.21.2 $ $Date: 1996/02/18 23:38:06 $";
#endif
/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/
/* @(#)xmhdef.h	10.2.1.1 91/12/19 (SNI) */

/**********************************************************************/
/*               COPYRIGHT (C) BULL S.A. 1990 1991 1992               */
/*                        All Rights Reserved                         */
/**********************************************************************/
/*                                                                    */
/*   * This software is furnished under licence and may be used only  */
/*   * in accordance with the terms of that licence and with the      */
/*   * inclusion of the above copyright notice.                       */
/*   * This software may not be provided or otherwise made available  */
/*   * to, or used by, any other person. No title to or ownership of  */
/*   * the software is hereby transferred.                            */
/*                                                                    */
/**********************************************************************/
/*	@(#)xmhdef.h	1.8 (Bull S.A) 4/23/92	*/

/**********************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xmhdef.h                                            */
/*                                                                    */
/* AUTHOR       : Bull S.A.                                           */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XOM/XDS                                             */
/*                                                                    */
/* DOC.-REF.    : X/Open Directory Services API Specification ver. 1.0*/
/*              : OSI Object Management API Specification ver. 2.0    */
/*              : X.400 API Version 2.0                               */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : Definition, for OM classes of MH package used by    */
/*                XDS, of all OM attributes that may be found         */
/*                in an instance of an OM class.                      */
/*                X400 definitions (temporarily)                      */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)@(#)xmhdef.h	1.8 (Bull S.A) 4/23/92 */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       |    |         */
/*   1.0    | 90-10-01| Original                       |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/**********************************************************************/

/*************  X400 include file ***************/
#include <xmhp.h>
/*******************End X400 definitions **********************/


/*	X400 MH object types			*/
#define	XMH_G3_FAX_NBPS 	18
#define	XMH_TELETEX_NBPS 	46

#define	TX_NBP_TYPE_LIST						\
		{{OM_CLASS,1,0,0},					\
		{MH_T_CONTROL_CHARACTER_SETS,1,0,0},			\
		{MH_T_GRAPHIC_CHARACTER_SETS,1,0,0},			\
		{MH_T_MISCELLANEOUS_CAPABILITIES,1,0,0},		\
		{MH_T_PAGE_FORMATS,1,0,0},				\
		{MH_T_PRIVATE_USE,1,0,0},				\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define	G3_FAX_TYPE_LIST						\
		{{OM_CLASS,1,0,0},					\
		{MH_T_A3_WIDTH,1,0,0},					\
		{MH_T_B4_LENGTH,1,0,0},					\
		{MH_T_B4_WIDTH,1,0,0},					\
		{MH_T_FINE_RESOLUTION,1,0,0},				\
		{MH_T_TWO_DIMENSIONAL,1,0,0},				\
		{MH_T_UNCOMPRESSED,1,0,0},				\
		{MH_T_UNLIMITED_LENGTH,1,0,0},				\
		{OM_NO_MORE_TYPES,0,0,0}}		
#endif /* _XMHDEF_H */
