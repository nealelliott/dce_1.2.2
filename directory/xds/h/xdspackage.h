/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdspackage.h,v $
 * Revision 1.1.6.2  1996/02/18  23:38:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:01  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:37:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:28  root]
 * 
 * Revision 1.1.4.4  1994/06/21  14:49:00  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:49:14  marrek]
 * 
 * Revision 1.1.4.3  1994/03/23  15:49:14  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:13  keutel]
 * 
 * Revision 1.1.4.2  1994/02/24  15:03:36  marrek
 * 	Change rcs string format.
 * 	[1994/02/24  12:14:04  marrek]
 * 
 * Revision 1.1.4.1  1994/02/22  18:59:55  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:36  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:03:34  marrek]
 * 
 * Revision 1.1.2.2  1993/08/10  07:11:44  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:21:12  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:17:32  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:06:00  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:38:20  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:45:27  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:20:10  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:47:10  melman]
 * 
 * $EndLog$
 */
#ifndef _XDSPACKAGE_H
#define _XDSPACKAGE_h

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdspackage_rcsid[] = "@(#)$RCSfile: xdspackage.h,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 23:38:02 $";
#endif


/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/
/* @(#)xdspackage.h	10.2.1.1 91/12/19 (SNI) */

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
/*	@(#)xdspackage.h	1.15 (Bull S.A) 6/29/92	*/

/**********************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xdspackage.h                                        */
/*                                                                    */
/* AUTHOR       : Bull S.A.                                           */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XOM/XDS                                             */
/*                                                                    */
/* DOC.-REF.    : X/Open Directory Services API Specification ver. 1.0*/
/*              : OSI Object Management API Specification ver. 2.0    */
/*              : X.400 API Version 2.0                               */
/*              : XDS/XOM Design Specification August 1990            */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : declarations of supported packages,                 */
/*                declaration of XDS specific switch function table   */ 
/*                used for workspace initialization.                  */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)@(#)xdspackage.h	1.10 (Bull S.A) 12/3/91 */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       |    |         */
/*   1.0    | 90-10-01| Original                       |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/**********************************************************************/

#ifdef	_MDUP_PKG_
#include	<mduppkg.h>
#else

#define	DEFINE_MDUP_PACKAGE_NUMBER
/* define XDS package number */

#define	IMPORT_MDUP_PACKAGE
/* OMP_IMPORT MDUP_PACKAGE */

#define	DEFINE_MDUP_PACKAGE
/* define xds_package entry for other package */

#define	AL_MDUP_PACKAGE	\
/* add definitions for new structures */

#define MDUP_PACKAGE_EQU	\
/* add equivalences for allocation classes */

#define MDUP_PACKAGE_ALLOC_INFO	\
/* add values in alloc_info table */
/*
    {"xds_other_struct",	sizeof(xds_other_struct),	  64, 0, FILL},
*/

#define	CASE_CREATE_MDUP_PACKAGE			\
/* add a case sequence */

#define	CASE_CR_HANDLE_MDUP_PACKAGE			\
/* add a case sequence */

#define	CASE_DELETE_MDUP_PACKAGE			\
/* add a case sequence */

#define	CASE_GET_MDUP_PACKAGE_CLOSURE_OBJECT		\
/* add other closure package objects */

#define	CASE_GET_MDUP_PACKAGE				\
/* add a case sequence */

#define DEFINE_INSTANCE_MDUP_PACKAGE			\
/* add define for other package */

#define	CASE_INSTANCE_MDUP_PACKAGE_CLOSURE_OBJECT	\
/* add other closure package objects */

#define	CASE_INSTANCE_MDUP_PACKAGE
/* add a case sequence */

#define	CASE_PUT_MDUP_PACKAGE
/* add a case sequence */

#define	CASE_PUT_VALUE_MDUP_PKG

#define	CASE_GET_VALUE_MDUP_PKG

#define	CASE_DELETE_VALUE_MDUP_PKG

#endif


/*
 * The package number is the index in the om_packages array
 *
*/
#define XDS_SERVICE_PACKAGE	0	/* Directory Service Package	 */
#define XDS_GDS_PACKAGE		1	/* Gds Directory Package	 */
#define XDS_BDCP			2	/* Basic Directory Contents Package */
/* DEFINE_MDUP_PACKAGE_NUMBER */
#define XDS_MDUP			3	/* MHS Directory User Package */
#define XDS_SAP_PACKAGE     4	/* Strong Authentication Package */


/*	package number = negative (index+1) in closure_packages array	*/
#define	XMH_PACKAGE		-1

#ifdef XDSINIT
OMP_IMPORT(DS_SERVICE_PKG)
OMP_IMPORT(DSX_GDS_PKG)
OMP_IMPORT(DS_BASIC_DIR_CONTENTS_PKG)
OMP_IMPORT(DS_SAP_PKG)
	/* VR IMPORT_MDUP_PACKAGE*/
/*
OMP_IMPORT(DS_MHS_DIR_USER_PKG)
*/

OMP_package xds_packages[NBMAXPACKAGES] = 
{	
	{{OMP_LENGTH(DS_SERVICE_PKG), OMP_X_DS_SERVICE_PKG},OMP_DEFAULT},
	{{OMP_LENGTH(DSX_GDS_PKG), OMP_X_DSX_GDS_PKG},OMP_BASIC},
	{{OMP_LENGTH(DS_BASIC_DIR_CONTENTS_PKG),OMP_X_DS_BASIC_DIR_CONTENTS_PKG},OMP_BASIC},
	DEFINE_MDUP_PACKAGE
#if 0
	{{OMP_LENGTH(DS_MHS_DIR_USER_PKG), OMP_X_DS_MHS_DIR_USER_PKG},OMP_BASIC},
#endif /* 0 */
    {{OMP_LENGTH(DS_STRONG_AUTHENT_PKG), OMP_O_DS_STRONG_AUTHENT_PKG},OMP_BASIC}
};

OM_object_identifier xds_default_package = 
	{OMP_LENGTH(DS_SERVICE_PKG),OMP_X_DS_SERVICE_PKG};

/* XDS Closure Packages */
OMP_EXPORT(MH_PACKAGE)
OMP_EXPORT(MH_C_G3_FAX_NBPS)
OMP_EXPORT(MH_C_TELETEX_NBPS)
OMP_EXPORT(MH_C_OR_ADDRESS)
OMP_EXPORT(MH_C_OR_NAME)

OMP_package xds_closure_packages[NBMAX_CLOSURE_PACKAGES] = 
{	
	{{OMP_LENGTH(MH_PACKAGE),OMP_X_MH_PACKAGE},OMP_BASIC},
	{{0,0},0}
};

#else

extern	OMP_package	xds_packages[];
extern	OM_object_identifier	xds_default_package;

/* XDS Closure Packages */
OMP_IMPORT(MH_PACKAGE)
OMP_IMPORT(MH_C_G3_FAX_NBPS)
OMP_IMPORT(MH_C_TELETEX_NBPS)
OMP_IMPORT(MH_C_OR_ADDRESS)
OMP_IMPORT(MH_C_OR_NAME)
extern	OMP_package	xds_closure_packages[];

#endif

#ifdef XDSINIT
/* 	List of XDS switch functions 		*/
extern OM_return_code	dsP_create_switch(
    const OMP_object_type   class_sub_id,
    OM_private_object       destination,
    const OM_boolean        initialise);

extern OM_return_code	dsP_cr_handle_switch(
    const OMP_object_type   class_sub_id);

extern OM_return_code	dsP_decode_switch(
	const OM_private_object     original,
	const OM_object_identifier  rules,
	OM_private_object           *encoding);

extern OM_return_code	dsP_delete_switch(
	OM_object subject);

extern OM_return_code	dsP_encode_switch(
	const OM_private_object encoding,
	OM_private_object		*original);

extern OM_return_code 	dsP_get_switch(
	const OM_private_object original,
	const OM_exclusions     exclusions,
	const OM_type_list      included_types,
	const OM_boolean        local_strings,
	const OM_value_position initial_value,
	const OM_value_position limiting_value,
	OM_public_object        *copy,
	OM_value_position       *total_number);

extern OM_return_code dsP_init_switch(
    const OMX_workspace workspace,
    xds_session *default_session);

extern OM_return_code	dsP_instance_switch(
	const OM_workspace      workspace,
	const OM_object         subject,
	const OM_object_identifier  class,
	const OMP_object_type   class_type,
	const OMP_object_type   subject_type,
	OM_boolean              *flag);

extern OM_return_code	dsP_put_switch(
    const OMP_object_type   object_type,
    OM_private_object       destination,
    const OM_modification   modification,
    const OM_object         source,
    const OM_type_list      included_types,
    const OM_value_position initial_value,
    const OM_value_position limiting_value);

extern OM_return_code	dsP_delete_specific_value(
    OMX_workspace   workspace,
    OM_descriptor   *desc_ptr);

extern OM_return_code	dsP_check_specific_syntax(
	OM_syntax	syntax);

struct	OMP_switch_functions_body	xds_sw_functions =
{
	(OMP_create_switch)	dsP_create_switch,
	(OMP_cr_handle_switch)	dsP_cr_handle_switch,
	(OMP_decode_switch)	dsP_decode_switch,
	(OMP_delete_switch)	dsP_delete_switch,
	(OMP_encode_switch)	dsP_encode_switch,
	(OMP_get_switch)	dsP_get_switch,
	(OMP_instance_switch)	dsP_instance_switch,
	(OMP_put_switch)	dsP_put_switch,
	(OMP_init_switch)	dsP_init_switch,
	(OMP_delete_specific_value)	dsP_delete_specific_value,
	(OMP_check_specific_syntax)	dsP_check_specific_syntax,
};	

#else
extern	struct	OMP_switch_functions_body	xds_sw_functions;
#endif

/***********************************************************************/
/*     define macros to easily change the interface                    */
/***********************************************************************/

#define OMP_INITIALIZE(DEF_SESSION,WORKSPACE)				\
	omP_initialize(&xds_sw_functions,				\
			xds_packages,					\
			xds_closure_packages,				\
			(void *)0,					\
			&xds_alloc_table,				\
			(void *)DEF_SESSION,				\
			(OM_workspace *)WORKSPACE)

#endif /* _XDSPACKAGE_H */
