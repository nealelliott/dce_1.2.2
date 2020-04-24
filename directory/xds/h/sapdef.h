/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sapdef.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:35:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:14  root]
 * 
 * Revision 1.1.2.6  1994/08/10  08:25:49  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:16:17  marrek]
 * 
 * Revision 1.1.2.5  1994/07/06  15:09:17  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:25  marrek]
 * 
 * Revision 1.1.2.4  1994/06/21  14:48:56  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:49:00  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:20:26  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:01  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:05  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:33  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:41  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:10:39  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:39:53  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:58:29  marrek]
 * 
 * $EndLog$
 */
#ifndef _SAPDEF_H
#define _SAPDEF_H

#include <d2dir.h>

#if !defined(lint) && defined(GDS_RCS_ID)
static char sapdef_rcsid[] = "@(#)$RCSfile: sapdef.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:45 $";
#endif
/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


/**********************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : sapdef.h                                            */
/*                                                                    */
/* AUTHOR       : 			                                          */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XOM/XDS                                             */
/*                                                                    */
/* DOC.-REF.    : X/Open Directory Services API Specification ver. 1.0*/
/*              : OSI Object Management API Specification ver. 2.0    */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : Definition, for OM classes of SAP package used by   */
/*                XDS, of all OM attributes that may be found         */
/*                in an instance of an OM class.                      */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :													  */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/**********************************************************************/

/* the following _TYPE_LIST defines are of type OMP_type_list	*/
/* defined in xomdef.h, which has the following structure: */

/*			typedef struct  {									*/
/*				OM_type     type;								*/
/*				OM_sint16   max_val;							*/
/*				OM_sint16   nb_val;								*/
/*				OM_value_length max_length;						*/
/*				void        *desc;								*/
/*			} OMP_type_list;									*/


#define ALGORITHM_IDENT_TYPE_LIST							\
	{{OM_CLASS,1,0,0},          							\
	{DS_ALGORITHM,1,0,0},									\
	{DS_ALGORITHM_PARAMETERS,1,0,0},						\
															\
	{OM_NO_MORE_TYPES,0,0,0}}


#define CERTIFICATE_TYPE_LIST								\
	{{OM_CLASS,1,0,0},          							\
/* from Signature: */										\
	{DS_ISSUER,1,0,0},										\
	{DS_SIGNATURE,1,0,0},									\
	{DS_SIGNATURE_VALUE,1,0,0},								\
/* from this class */										\
	{DS_SERIAL_NBR,1,0,0},									\
	{DS_SUBJECT,1,0,0},										\
	{DS_SUBJECT_ALGORITHM,1,0,0},							\
	{DS_SUBJECT_PUBLIC_KEY,1,0,0},							\
	{DS_VALIDITY_NOT_AFTER,1,0,DS_VL_VALIDITY_NOT_AFTER},	\
	{DS_VALIDITY_NOT_BEFORE,1,0,DS_VL_VALIDITY_NOT_BEFORE},	\
	{DS_VERSION,1,0,0},										\
															\
	{OM_NO_MORE_TYPES,0,0,0}}


#define CERTIFICATE_LIST_TYPE_LIST							\
	{{OM_CLASS,1,0,0},          							\
/* from Signature: */										\
	{DS_ISSUER,1,0,0},										\
	{DS_SIGNATURE,1,0,0},									\
	{DS_SIGNATURE_VALUE,1,0,0},								\
/* from this class */										\
	{DS_LAST_UPDATE,1,0,0},									\
	{DS_REVOKED_CERTS,MAX_VALUES,0,0},								\
															\
	{OM_NO_MORE_TYPES,0,0,0}}


#define CERTIFICATE_PAIR_TYPE_LIST							\
	{{OM_CLASS,1,0,0},          							\
/* from this class */										\
	{DS_FORWARD,MAX_VALUES,0,0},          							\
	{DS_REVERSE,MAX_VALUES,0,0},          							\
															\
	{OM_NO_MORE_TYPES,0,0,0}}


#define CERTIFICATE_SUBLIST_TYPE_LIST						\
	{{OM_CLASS,1,0,0},          							\
/* from Signature: */										\
	{DS_ISSUER,1,0,0},										\
	{DS_SIGNATURE,1,0,0},									\
	{DS_SIGNATURE_VALUE,1,0,0},								\
/* from this class */										\
	{DS_REVOC_DATE,MAX_VALUES,0,0},									\
	{DS_SERIAL_NBRS,MAX_VALUES,0,0},									\
															\
	{OM_NO_MORE_TYPES,0,0,0}}

#define SIGNATURE_TYPE_LIST									\
	{{OM_CLASS,1,0,0},          							\
/* from this class */										\
	{DS_ISSUER,1,0,0},										\
	{DS_SIGNATURE,1,0,0},									\
	{DS_SIGNATURE_VALUE,1,0,0},								\
             												\
	{OM_NO_MORE_TYPES,0,0,0}}

#define IAPL_V1988		0

#endif /* _SAPDEF_H */
