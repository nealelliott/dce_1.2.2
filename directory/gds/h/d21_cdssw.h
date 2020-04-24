/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_cdssw.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:27  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:59:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:26  root]
 * 
 * Revision 1.1.2.3  1994/08/17  13:56:48  keutel
 * 	OT 11222: adding CDS class and CDS class version via XDS
 * 	[1994/08/17  12:49:56  keutel]
 * 
 * Revision 1.1.2.2  1994/06/09  18:46:10  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:13  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:10  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:12:34  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:53:59  marrek]
 * 
 * $EndLog$
 */
#ifndef _D21_CDSSW_H
#define _D21_CDSSW_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d21_cdssw_rcsid[] = "@(#)$RCSfile: d21_cdssw.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:29 $";
#endif

/* ****************************************************************** */
/*                                                                    */
/*   * COPYRIGHT   (C)  1989 BY SIEMENS AG                            */
/*   * All rights reserved                                            */
/*   *                                                                */
/*   * This software is furnished under licence and may be used only  */
/*   * in accordance with the terms of that licence and with the      */
/*   * inclusion of the above copyright notice.                       */
/*   * This software may not be provided or otherwise made available  */
/*   * to, or used by, any other person. No title to or ownership of  */
/*   * the software is hereby transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*---------------------------------------------------------------------

	Filename:		d21_cdssw.h.h 

	Author:			P. Dunne
	Date:			1.02.1991

	Component:		OSF DCE Naming Switch

	Description  : 	Switch software that submits naming operations 
                	and obtains naming results from both the global
                	and cell naming servics.                      

	Version:		1.0 (initial)

	Update History:
	Version:		Date:		Comments:			 	By whom:
	--------------------------------------------------------------------
	1.0				1.02.91		original				P. Dunne


 -----------------------------------------------------------------------*/

#include <dce/dnsclerk.h>
 
#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#define DSW_REFERRED		1			/*	referral to the CDS name space	*/
#define DSW_NSPACE			1			/*	switch to the CDS name space	*/
#define DSW_AV_MAX			1000		    /*	max of 1000 values per attribute	*/
#define DSW_CDS_GN_PREFIX	"/..."		/* global name prefix for CDS		*/

/*		object class OIDs used by the switch		*/
#define DSW_ALIAS_OID	"85.6.1"
#define DSW_GROUP_OID	"85.6.9"		/*	groupOfNames object	*/

/*		attribute OIDs used by the switch		*/
#define DSW_CLASS_OID		"85.4.0"
#define DSW_CN_OID			"85.4.3"
#define DSW_MEMBER_OID		"85.4.31"			
#define	DSW_AON_OID			"85.4.1"	/*  aliasedObjectName attribute	*/
#define DSW_COUNTRY_OID		"85.4.6"
#define DSW_TYPELESS_OID	"43.22.1.2.2"

/*		CDS OIDs used by the switch				*/
#define DSW_CDS_MEMBER_OID		"1.3.22.1.3.10"
#define DSW_CDS_CLASS_OID		"1.3.22.1.3.15"
#define DSW_CDS_CLASS_VERSION_OID	"1.3.22.1.3.16"

/*-------  prototype function definitions for the switch  --------------*/


int		d21_cds_init (void);

int		d21_cds_switch (
			u_char	*obj_name);

int		d21_cds_referral (
			D2_name_string	gds_name,
			D2_error		*naming_error);

int		d21_7_switch (
			D21_p7	*add_par_blk);

int		d21_6_switch (
			D21_p6	*comp_par_blk);

int		d21_4_switch (
			D21_p4	*list_par_blk);

int		d21_3_switch (
			D21_p3	*read_par_blk);

int		d21_C_switch (
			D21_pC	*mod_par_blk);

int		d21_8_switch (
			D21_p8	*rem_par_blk);

int		d21_cds_get_syntax (
			u_char	*a_type);	

int		d21_cds_set_flags (
			struct dnsFlagStat	*flags,
			D2_c_arg			*common_args);
		
int		d21_x500name2cdsname (	
			u_char	*iapl_name,	
			u_char	*cds_name);

int		d21_cdsname2x500name (	
			u_char	*cds_name,
			u_char	*iapl_name);	

int		d21_x500val2cdsval (	
			D2_a_value	*attr_value,
			dns_attrvalue_t		*dns_value);

int		d21_cdsval2x500val (	
			D2_a_value			*value_union,
			dns_attrvalue_t		*dns_value);

void	d21_cds_unwilling (
			D2_pbhead	*naming_error);

int		d21_cds_check_for_alias (	
			dns_full_name_t		*dns_fname,
			dns_full_name_t		*cds_alias,
			int					*alias_found,
			struct dnsFlagStat	*flags,
			D2_pbhead			*op_error);

void	d21_cds_compress_oid (	
					u_char	*oid_str);

void	d21_cds_uncompress_oid (	
					u_char	*oid_in,	
					u_char	*oid_out); 

void	d21_cds_dots2hypens (	
					u_char	*oid);

void	d21_cds_hypens2dots (	
					u_char	*oid);

void	d21_cds_map_error (
			int			cds_error,
			D2_pbhead	*gds_error);

int		d21_cds_error_info (	
			D2_pbhead		*cds_error,
			dns_full_name_t	*resolved,
			D2_error		**error_info);

int  	d21_cds_attr_error (
                dns_full_name_t *dns_fname,
                D2_a_type       *attr_type,
                D2_error        **p_gds_error);

void d21_cds_expand_oids (	
			u_char	*cds_name,
			u_char	*expanded_name);

/*------------------------------------------------------------------*/

#endif /* _D21_CDSSW_H */
