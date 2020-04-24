/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: mduppkg.h,v $
 * Revision 1.1.21.2  1996/02/18  23:37:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:03  marty]
 *
 * Revision 1.1.21.1  1995/12/08  15:35:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:12  root]
 * 
 * Revision 1.1.19.4  1994/06/10  21:20:22  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:59  devsrc]
 * 
 * Revision 1.1.19.3  1994/05/10  16:04:04  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:30  marrek]
 * 
 * Revision 1.1.19.2  1994/03/23  15:49:03  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:36:56  keutel]
 * 
 * Revision 1.1.19.1  1994/02/22  19:23:37  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:10:32  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:01:20  marrek]
 * 
 * Revision 1.1.17.1  1993/10/14  19:19:49  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:18:12  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:54:21  keutel]
 * 
 * $EndLog$
 */
#ifndef _MDUPPKG_H
#define _MDUPPKG_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char mduppkg_rcsid[] = "@(#)$RCSfile: mduppkg.h,v $ $Revision: 1.1.21.2 $ $Date: 1996/02/18 23:37:43 $";
#endif
/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/
/* @(#)mduppkg.h	10.2.1.1 91/12/19 (SNI) */

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
/*	@(#)mduppkg.h	1.8 (Bull S.A) 7/1/92	*/

/**********************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : mduppkg.h                                           */
/*                                                                    */
/* AUTHOR       : Bull S.A                                            */
/* DATE         : 1. 11. 1991                                         */
/*                                                                    */
/* COMPONENT    : XOM/XDS                                             */
/*                                                                    */
/* DOC.-REF.    : XDS/XOM Design Specification August 1990            */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : XDS memory allocation structures definitions.       */
/*                XDS Class types and alloc info structures.          */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)@(#)mduppkg.h	1.4 (Bull S.A) 12/11/91 */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       |    |         */
/*   1.0    | 90-10-01| Original                       |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/**********************************************************************/

#include	<xdsmdup.h>
#include	<mhsdef.h>

/**********************************************************************/
/*                                                                    */
/* Other package allocation management definitions                    */
/*                                                                    */
/**********************************************************************/

#undef	AL_MDUP_PACKAGE
#define	AL_MDUP_PACKAGE	\
	AL_XDS_DL_SUBMIT_PERMS,	\
	AL_XDS_OR_NAME,		\
	AL_XDS_OR_ADDRESS,	\
	AL_XDS_STR_LIST,

#undef	MDUP_PACKAGE_EQU
#define MDUP_PACKAGE_EQU	\
/* no other package equivalence */

#undef	MDUP_PACKAGE_ALLOC_INFO
/*----------------------------------------------------------------------*/
/*      Obsolete with the new memory management                         */
/*----------------------------------------------------------------------*/
/*
#define MDUP_PACKAGE_ALLOC_INFO	\
    {"xds_dl_submit_perm",	sizeof(xds_dl_submit_perm),  	  64, 0, FILL},\
    {"xds_OR_name",		sizeof(xds_OR_name),	  	  64, 0, FILL},\
    {"xds_OR_address",		sizeof(xds_OR_address),	  	  64, 0, FILL},\
    {"xds_str_list",		sizeof(D2_str_list),	  	  64, 0, FILL},
*/
/*----------------------------------------------------------------------*/
/*      Replace the old one with the new memory management              */
/*----------------------------------------------------------------------*/
#define MDUP_PACKAGE_ALLOC_INFO	\
    sizeof(xds_dl_submit_perm),	\
    sizeof(xds_OR_name),	\
    sizeof(xds_OR_address),	\
    sizeof(D2_str_list),

#undef	DEFINE_MDUP_PACKAGE_NUMBER
#define	XDS_MDUP                3       /* MHS Directory User Package */

#undef	IMPORT_MDUP_PACKAGE
#define	IMPORT_MDUP_PACKAGE		\
OMP_IMPORT(DS_MHS_DIR_USER_PKG)

#undef	DEFINE_MDUP_PACKAGE
#define	DEFINE_MDUP_PACKAGE		\
    {{OMP_LENGTH(DS_MHS_DIR_USER_PKG), OMP_X_DS_MHS_DIR_USER_PKG},OMP_BASIC},

/* declare an OM_object_identifier of MHS DIRECTORY USER PACKAGE      */
/* to check if this package has been negotiated when the user want    */
/* to create an instance of an MH Package class                       */
#ifdef	XDSINIT
OMP_IMPORT(DS_C_DL_SUBMIT_PERMS)
OM_object_identifier	mdup_class = 
    				{OMP_LENGTH(DS_C_DL_SUBMIT_PERMS),
				 OMP_X_DS_C_DL_SUBMIT_PERMS};
#else
extern OM_object_identifier	mdup_class;
#endif

/**********************************************************************/
/*                                                                    */
/* Other Package switches definitions                                 */
/*                                                                    */
/**********************************************************************/

#undef	CASE_CREATE_MDUP_PACKAGE
#define	CASE_CREATE_MDUP_PACKAGE			\
	case XDS_MDUP:					\
	switch (OMP_CLASS(class_sub_id)) {		\
	    case MHS_DLSP:				\
		break;					\
	    default:					\
		ret = (OM_NO_SUCH_CLASS);		\
		break;					\
	}						\
	break;						\
	case XMH_PACKAGE:				\
	{						\
	    OMP_object_type	pkg_class;		\
	    if ((ret = omP_get_class_sub_id(		\
			OMP_WORKSPACE(destination),	\
			mdup_class,			\
			&pkg_class)) != OM_SUCCESS)	\
		break;					\
	}						\
	switch (OMP_CLASS(class_sub_id)) {		\
	    case XMH_OR_NAME:				\
		break;					\
	    case XMH_OR_ADDRESS:			\
/*		ret = (OM_NOT_CONCRETE); */		\
		break;					\
	    default:					\
		ret = (OM_NO_SUCH_CLASS);		\
		break;					\
	}						\
	break;

#undef	CASE_CR_HANDLE_MDUP_PACKAGE
#define	CASE_CR_HANDLE_MDUP_PACKAGE			\
	case XDS_MDUP:					\
	switch (OMP_CLASS(class_sub_id)) {		\
	    case MHS_DLSP:				\
		break;					\
	    default:					\
		return(OM_NO_SUCH_CLASS);		\
		break;					\
	}						\
	break;						\
	case XMH_PACKAGE:				\
	switch (OMP_CLASS(class_sub_id)) {		\
	    case XMH_OR_NAME:				\
		break;					\
	    case XMH_OR_ADDRESS:			\
		break;					\
	    default:					\
		return (OM_NO_SUCH_CLASS);		\
		break;					\
	}						\
	break;

#undef	CASE_DELETE_MDUP_PACKAGE
#define	CASE_DELETE_MDUP_PACKAGE			\
	case XDS_MDUP:					\
	switch(OMP_CLASS(obj_type)) {			\
	    case MHS_DLSP:				\
		ret = CALL_DEL(dlsp);			\
		break;					\
	    default:					\
		ret = OM_NO_SUCH_OBJECT;		\
		break;					\
	    }						\
	    break;					\
	case XMH_PACKAGE:				\
	{						\
	    OMP_object_type	pkg_class;		\
	    if ((ret = omP_get_class_sub_id(		\
			(OMX_workspace) workspace,	\
			mdup_class,			\
			&pkg_class)) != OM_SUCCESS)	\
		break;					\
	}						\
	switch(OMP_CLASS(obj_type)) {			\
	    case XMH_OR_NAME:				\
		ret = CALL_DEL(OR_name);		\
		break;					\
							\
	    case XMH_OR_ADDRESS:			\
		ret = CALL_DEL(OR_address);		\
/*		ret = OM_NO_SUCH_OBJECT; */		\
		break;					\
	    default:					\
		ret = OM_NO_SUCH_OBJECT;		\
		break;					\
	    }						\
	    break;

#undef	CASE_GET_MDUP_PACKAGE_CLOSURE_OBJECT
#define	CASE_GET_MDUP_PACKAGE_CLOSURE_OBJECT		\
	    case XMH_OR_NAME:				\
		ret = CALL_GET(OR_name);		\
		break;					\
	    case XMH_OR_ADDRESS:			\
		ret = CALL_GET(OR_address);		\
/*		ret = OM_NO_SUCH_OBJECT; */		\
		break;					\
	    default:					\
		ret = OM_NO_SUCH_OBJECT;		\
		break;

#undef	CASE_GET_MDUP_PACKAGE
#define	CASE_GET_MDUP_PACKAGE				\
	case XDS_MDUP:					\
	switch(OMP_CLASS(obj_type)) {			\
	    case MHS_DLSP:				\
		ret = CALL_GET(dlsp);			\
		break;					\
	    default:					\
		ret = OM_NO_SUCH_OBJECT;		\
		break;					\
	    }						\
	    break;

#undef	DEFINE_INSTANCE_MDUP_PACKAGE
#define	DEFINE_INSTANCE_MDUP_PACKAGE
#define	OR_ADDRESS(t)		((t) == XMH_OR_NAME)

#undef	CASE_INSTANCE_MDUP_PACKAGE_CLOSURE_OBJECT
#define	CASE_INSTANCE_MDUP_PACKAGE_CLOSURE_OBJECT		\
		case XMH_OR_ADDRESS:				\
		    if (OR_ADDRESS(OMP_CLASS(subject_type)))	\
			*flag = OM_TRUE;			\
		break;						\
		case XMH_OR_NAME:				\
		break;

#undef	CASE_INSTANCE_MDUP_PACKAGE
#define	CASE_INSTANCE_MDUP_PACKAGE			\
	/*						\
	 * MDUP Package.				\
	 */						\
	case XDS_MDUP:					\
	    switch (OMP_CLASS(class_type))		\
	    {						\
		case MHS_DLSP:				\
		    break;				\
							\
		default:				\
		    ret = OM_NO_SUCH_CLASS;		\
			goto end;				\
	    }						\
	    break;					\

#undef	CASE_PUT_MDUP_PACKAGE
#define	CASE_PUT_MDUP_PACKAGE				\
	    case XDS_MDUP:				\
		switch (OMP_CLASS(object_type)) {	\
		    case MHS_DLSP:			\
		    	ret = CALL_PUT(dlsp);		\
			break;				\
		    default:				\
			ret = (OM_NO_SUCH_CLASS);	\
			break;				\
		}					\
		break;					\
	    case XMH_PACKAGE:				\
	    {						\
		OMP_object_type	pkg_class;		\
		if ((ret = omP_get_class_sub_id(		\
			(OMX_workspace) workspace,	\
			mdup_class,			\
			&pkg_class)) != OM_SUCCESS)	\
		    break;				\
	    }						\
		switch (OMP_CLASS(object_type)) {	\
		    case XMH_OR_NAME:			\
		    	ret = CALL_PUT(OR_name);	\
			break;				\
		    case XMH_OR_ADDRESS:		\
		    	ret = CALL_PUT(OR_address);	\
/*			ret = (OM_NO_SUCH_CLASS); */	\
			break;				\
		    default:				\
			ret = (OM_NO_SUCH_CLASS);	\
			break;				\
		}					\
		break;

#endif /* _MDUPPKG_H */
