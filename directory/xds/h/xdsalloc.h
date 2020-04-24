/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdsalloc.h,v $
 * Revision 1.1.6.2  1996/02/18  23:37:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:17  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:35:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:16  root]
 * 
 * Revision 1.1.4.5  1994/08/10  08:25:51  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:16:20  marrek]
 * 
 * Revision 1.1.4.4  1994/06/21  14:48:57  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:49:03  marrek]
 * 
 * Revision 1.1.4.3  1994/05/10  16:04:06  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:36  marrek]
 * 
 * Revision 1.1.4.2  1994/03/23  15:49:05  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:36:59  keutel]
 * 
 * Revision 1.1.4.1  1994/02/22  19:23:46  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:10:50  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:01:51  marrek]
 * 
 * Revision 1.1.2.2  1993/08/10  07:09:43  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:19:21  marrek]
 * 
 * Revision 1.1.4.2  1992/12/30  21:17:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:05:52  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:19:51  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:46:53  melman]
 * 
 * $EndLog$
 */
#ifndef _XDSALLOC_H
#define _XDSALLOC_h

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdsalloc_rcsid[] = "@(#)$RCSfile: xdsalloc.h,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 23:37:48 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/
/* @(#)xdsalloc.h	10.2.1.1 91/12/19 (SNI) */

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
/*	@(#)xdsalloc.h	1.5 (Bull S.A) 4/23/92	*/

/**********************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xdsalloc.h                                          */
/*                                                                    */
/* AUTHOR       : Bull S.A                                            */
/* DATE         : 1. 10. 1990                                         */
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
/* HISTORY      : sccsid  =  @(#)@(#)xdsalloc.h	1.4 (Bull S.A) 11/27/91 */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       |    |         */
/*   1.0    | 90-10-01| Original                       |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/**********************************************************************/

/*
 * XDS allocation classes, for structures declared.
 * Warning : alloc_type and alloc_info are order dependent.
 */
#define	AL_XDS_BEG	AL_GEN_END	/* offset in alloc_info table */

enum 	xds_alloc_type
{
	AL_VALUE=AL_XDS_BEG,		/* Attribute-Value 	*/
	AL_TYPE,			/* Attribute-Type 	*/
	/* 
	 * SERVICE PACKAGE.
	 */
	AL_XDS_ACCESS_POINT,
	AL_XDS_ATTRIBUTE,
	AL_XDS_AVA,
	AL_XDS_COMMON_RESULTS,
	AL_XDS_COMPARE_RESULT,
	AL_XDS_CONTEXT,
	AL_XDS_CONTINUATION_REF,
	AL_XDS_ENTRY_INFO,
	AL_XDS_ENTRY_INFO_SELECTION,
	AL_XDS_ENTRY_MOD,
	AL_XDS_ERROR,
	AL_XDS_EXT,
	AL_XDS_FILTER,
	AL_XDS_FILTER_ITEM,
	AL_XDS_LIST_INFO,
	AL_XDS_LIST_INFO_ITEM,
	AL_XDS_OPERATION_PROGRESS,
	AL_XDS_PARTIAL_OUTCOME_QUAL,
	AL_XDS_READ_RESULT,
	AL_XDS_SEARCH_INFO,
	AL_XDS_SESSION,
	/* 
	 * BDCP PACKAGE.
	 */
	AL_XDS_FACSIMILE_PHONE_NBR,
	AL_XDS_POSTAL_ADDRESS,
	AL_XDS_SEARCH_CRITERION,
	AL_XDS_SEARCH_GUIDE,
	AL_XDS_TELETEX_TERM_IDENT,
	AL_XDS_TELEX_NBR,
	AL_CRIT_ITEM,		/* For XDS_SEARCH_CRITERION */
/*	AL_MDUP_PACKAGE	*/
#ifdef	_MDUP_PKG_
	AL_XDS_DL_SUBMIT_PERMS,
	AL_XDS_OR_NAME,
	AL_XDS_OR_ADDRESS,
	AL_XDS_STR_LIST,
#endif	/* _MDUP_PKG_ */
    /*
	 * SAP PACKAGE.
	 */
	AL_XDS_ALGORITHM_IDENT,
	AL_XDS_CERTIFICATE,
	AL_XDS_CERTIFICATE_LIST,
	AL_XDS_CERTIFICATE_PAIR,
	AL_XDS_CERTIFICATE_SUBLIST,
	AL_XDS_CERTIFICATE_SUBSUBLIST,
	AL_XDS_SIGNATURE,
					  
	AL_XDS_END
};

#ifndef	NULL
#define	NULL	((void *) 0)
#endif

#ifdef	XDSINIT

/*----------------------------------------------------------------------*/
/*      Obsolete with the new memory management                         */
/*----------------------------------------------------------------------*/
/*
#define XDS_STRING_UNIT	32

struct	alloc_stat
 xds_alloc_stat[AL_XDS_END] = {0};

struct	alloc_info
 xds_alloc_info[] =
{
    {"string",			XDS_STRING_UNIT,		1024, 1, FILL},
    {"descriptor",		sizeof(OM_descriptor),		1024, 1, FILL},
    {"handle",			sizeof(OMP_private_object),	1024, 0, FILL},
    {"value",			sizeof(D2_a_value),		1024, 1, FILL},
    {"type",			sizeof(D2_a_type),		1024, 1, FILL},
    {"xds_acc_point",		sizeof(xds_acc_point),		  64, 0, FILL},
    {"xds_attribute",		sizeof(xds_attribute),		  64, 0, FILL},
    {"xds_ava",			sizeof(xds_ava),		  64, 0, FILL},
    {"xds_common_res",		sizeof(xds_common_res),		  64, 0, FILL},
    {"xds_compare_result",	sizeof(xds_compare_result),	  64, 0, FILL},
    {"xds_context",		sizeof(xds_context),		  64, 0, FILL},
    {"xds_cont_ref",		sizeof(xds_cont_ref),		  64, 0, FILL},
    {"xds_entry_info",		sizeof(xds_entry_info),		  64, 0, FILL},
    {"xds_entry_info_sel",	sizeof(xds_entry_info_sel),	  64, 0, FILL},
    {"xds_entry_mod",		sizeof(xds_entry_mod),		  64, 0, FILL},
    {"xds_error",		sizeof(xds_error),		  64, 0, FILL},
    {"xds_extension",		sizeof(xds_extension),		  64, 0, FILL},
    {"xds_filter",		sizeof(xds_filter),		  64, 0, FILL},
    {"xds_filter_item",		sizeof(xds_fi_item),		  64, 0, FILL},
    {"xds_list_info",		sizeof(xds_list_info),		  64, 0, FILL},
    {"xds_list_item",		sizeof(xds_list_item),		  64, 0, FILL},
    {"xds_op_progress",		sizeof(xds_op_progress),	  64, 0, FILL},
    {"xds_part_oq",		sizeof(xds_part_oq),		  64, 0, FILL},
    {"xds_read_result",		sizeof(xds_read_result),	  64, 0, FILL},
    {"xds_search_info",		sizeof(xds_search_info),	  64, 0, FILL},
    {"xds_session",		sizeof(xds_session),		  64, 0, FILL},
    {"xds_faxnumber",		sizeof(xds_faxnumber),		  64, 0, FILL},
    {"xds_postal_address",	sizeof(xds_postal_address),	  64, 0, FILL},
    {"xds_search_criterion",	sizeof(xds_search_criterion),	  64, 1, FILL},
    {"xds_search_guide",	sizeof(xds_search_guide),	  64, 0, FILL},
    {"xds_ttxid",		sizeof(xds_ttxid),	  	  64, 0, FILL},
    {"xds_telex_number",	sizeof(xds_telex_number),	  64, 0, FILL},
    {"crit_item",		sizeof(D2_crit_item),	  	  64, 0, FILL},
    MDUP_PACKAGE_ALLOC_INFO
#ifdef	_MDUP_PKG_
    {"xds_dl_submit_perm",	sizeof(xds_dl_submit_perm),  	  64, 0, FILL},
    {"xds_OR_name",		sizeof(xds_OR_name),	  	  64, 0, FILL},
    {"xds_OR_address",		sizeof(xds_OR_address),	  	  64, 0, FILL},
    {"xds_str_list",		sizeof(D2_str_list),	  	  64, 0, FILL},
#endif
};

struct	alloc_table
 xds_alloc_table =
{
    AL_XDS_END,
    XDS_STRING_UNIT,
    xds_alloc_info,
    xds_alloc_stat
};
*/

/*----------------------------------------------------------------------*/
/*      Replace the old one with the new memory management              */
/*----------------------------------------------------------------------*/
struct	alloc_table
 xds_alloc_table[] =
{
    {sizeof(char)},
    {sizeof(OM_descriptor)},
    {sizeof(OMP_private_object)},
    {sizeof(D2_a_value)},
    {sizeof(D2_a_type)},
    {sizeof(xds_acc_point)},
    {sizeof(xds_attribute)},
    {sizeof(xds_ava)},
    {sizeof(xds_common_res)},
    {sizeof(xds_compare_result)},
    {sizeof(xds_context)},
    {sizeof(xds_cont_ref)},
    {sizeof(xds_entry_info)},
    {sizeof(xds_entry_info_sel)},
    {sizeof(xds_entry_mod)},
    {sizeof(xds_error)},
    {sizeof(xds_extension)},
    {sizeof(xds_filter)},
    {sizeof(xds_fi_item)},
    {sizeof(xds_list_info)},
    {sizeof(xds_list_item)},
    {sizeof(xds_op_progress)},
    {sizeof(xds_part_oq)},
    {sizeof(xds_read_result)},
    {sizeof(xds_search_info)},
    {sizeof(xds_session)},
    {sizeof(xds_faxnumber)},
    {sizeof(xds_postal_address)},
    {sizeof(xds_search_criterion)},
    {sizeof(xds_search_guide)},
    {sizeof(xds_ttxid)},
    {sizeof(xds_telex_number)},
    {sizeof(D2_crit_item)},
/*    MDUP_PACKAGE_ALLOC_INFO */
#ifdef	_MDUP_PKG_
    {sizeof(xds_dl_submit_perm)},
    {sizeof(xds_OR_name)},
    {sizeof(xds_OR_address)},
    {sizeof(D2_str_list)},
#endif	/* _MDUP_PKG_ */
/*  SAP_PACKAGE_ALLOC_INFO */
	{sizeof(xds_algorithm_ident)},			
	{sizeof(xds_certificate)},			
	{sizeof(xds_certificate_list)},	
	{sizeof(xds_certificate_pair)},
	{sizeof(xds_certificate_sublist)},
	{sizeof(xds_certificate_subsublist)}
};


#else
extern struct alloc_table xds_alloc_table;
#endif

/*
 * XDS SERVICE PACKAGE Equivalence allocation classes.
 */
#define	AL_XDS_ATTRIBUTE_LIST		AL_XDS_ATTRIBUTE
#define	AL_XDS_ENTRY_MOD_LIST		AL_XDS_ENTRY_MOD
#define AL_XDS_LIST_RESULT		AL_XDS_LIST_INFO
#define	AL_XDS_SEARCH_RESULT		AL_XDS_SEARCH_INFO
/*
 * XDS GDS PACKAGE Equivalence allocation classes.
 */
#define	AL_XDS_GDS_CONTEXT		AL_XDS_CONTEXT
#define	AL_XDS_GDS_SESSION		AL_XDS_SESSION
/*
 * XDS BDCP PACKAGE Equivalence allocation classes.
 */
#define	AL_STR				AL_TYPE
#define	AL_CRIT_SET			AL_TYPE
/*
 * XDS OTHER PACKAGE Equivalence allocation classes.
 */
/*	MDUP_PACKAGE_EQU	*/

#define	ALLOC_XDS_TYPE(class)	(AL_##class)

#endif /* _XDSALLOC_H */
