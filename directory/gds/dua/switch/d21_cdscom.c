/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_cdscom.c,v $
 * Revision 1.1.11.2  1996/02/18  18:17:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:57  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:55:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:03  root]
 * 
 * Revision 1.1.9.2  1994/05/10  15:54:21  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:33:47  marrek]
 * 
 * Revision 1.1.9.1  1994/02/22  18:45:37  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:37:51  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  06:47:30  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:02:09  marrek]
 * 
 * Revision 1.1.5.4  1992/12/31  19:49:17  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:46  bbelch]
 * 
 * Revision 1.1.5.3  1992/11/16  08:58:00  marrek
 * 	Remove empty #ifndef lint and #ifdef ANSIC.
 * 	[1992/11/06  11:34:29  marrek]
 * 
 * Revision 1.1.5.2  1992/09/29  21:10:29  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:24:11  treff]
 * 
 * Revision 1.1.2.4  1992/06/30  20:59:05  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:39:43  melman]
 * 
 * Revision 1.1.2.3  1992/06/02  03:37:57  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:39:04  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:41:57  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:38:24  weisman]
 * 
 * Revision 1.1  1992/01/19  15:13:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_cdscom.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 18:17:58 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*---------------------------------------------------------------------

	Filename:		d21_cdscom.c 

	Author:			P. Dunne
	Date:			1.02.1991

	Component:		OSF DCE Naming Switch

	Functions:	
		exported:	d21_6_switch().

		static:		comp_aon_attr();
	Version:		1.0 (initial)

	Update History:
	Version:		Date:		Comments:			 	By whom:
	--------------------------------------------------------------------
	1.0				1.02.91		original				P. Dunne


 -----------------------------------------------------------------------*/

#include <gds.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include <d21dua.h>
#include <dce/dnsclerk.h>
#include <d21_cdssw.h>

/*---------------  local static function prototypes  ---------------------*/

static int		comp_aon_attr (
			D2_ava			*aon_attr,
			dns_full_name_t	*cds_alias,
			int				*matched,
			D2_pbhead		*comp_error);



/*--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------

	d21_6_switch():

	Carries out the compare operation on an object in the local DNS 
	cell. The compare operation is received at the IAPL* interface 
	and has to be mapped to a DNSPI interface calls. The result from 
	the DNSPI compare operation is then mapped to the IAPL* compare
	operation result.

 -----------------------------------------------------------------------*/

int		d21_6_switch (
			D21_p6	*comp_par_blk)
{
	dns_full_name_t		*dns_fname,	cds_alias,	*dns_member;
	dns_attr_name_t 	*dns_type;
	dns_opq_fname		part_result;
	dns_attrvalue_t		dns_value;
	dnsFlagStat_t		flags;
	D2_ava				*gds_attr;
	D2_pbhead			*comp_error;
	D2_c_res			*c_results;
	const char function_name[] = "d21_6_switch";
	u_char				cds_name[D2_DNL_MAX],	member[D2_DNL_MAX],	
						*attr_value,	type_str[D2_OBJ_IDL_MAX],
						uncompressed_oid [D2_OBJ_IDL_MAX];
	int					matched,	deref_alias,	alias_found,
						r_code;

	DUA_TRACE_IAPL_ENTRY
	r_code = D2_NOERROR;

	/*
		the pbhead and 6cres structures are malloced by the user
																*/
	comp_error = comp_par_blk->d21_6pbhead;
	c_results = comp_par_blk->d21_6cres;

	if (d21_x500name2cdsname (comp_par_blk->d21_6entry, cds_name) 
		== D2_ERROR) {
		DUA_TRACE_X500_TO_CDS_NAME_FAILED
		comp_error->d2_retcode = D2_CALL_ERR;
        comp_error->d2_errclass = D2_PAR_ERR;
        comp_error->d2_errvalue = D2_NOT_SPEC;
        return (D2_ERROR);
	}

	dns_fname = (dns_full_name_t *) cds_name;

	/*
		IAPL* common arguments are mapped to DNS flags structure.
																	*/
	(void) d21_cds_set_flags (&flags, comp_par_blk->d21_6c_a);
	flags.fsPartialRes = &part_result;
	flags.fsResLength = sizeof (part_result);
	flags.fsMaybeMore = TRUE;

	/*
		checks to see if an alias has been traversed.
															*/
	if (d21_cds_check_for_alias (dns_fname, &cds_alias, &alias_found,
		&flags, comp_error) == D2_ERROR) {

        d21_cds_error_info (comp_error, dns_fname, &comp_par_blk->d21_6err);
		r_code = D2_ERROR;

	} else {

		if (comp_par_blk->d21_6c_a->d2_serv_cntrls & D2_DREFALIAS) {
			deref_alias = FALSE;
		} else {
			deref_alias = TRUE;
		}

		gds_attr = comp_par_blk->d21_6comp;

		/*	set default values	*/
		comp_par_blk->d21_6match = FALSE;
		comp_par_blk->d21_6from_entry = TRUE;
		c_results->d2_al_deref = FALSE;

		if (alias_found && !deref_alias) {

			/*
				Examine the alias entry itself.
												*/
			if ((r_code = comp_aon_attr (gds_attr, &cds_alias, &matched, 
				comp_error))  != D2_ERROR) {

				if (matched) {
					comp_par_blk->d21_6match = TRUE;
				} 

				if (flags.fsFromCache) {
					comp_par_blk->d21_6from_entry = FALSE;
				}
			}

		} else {

			if (alias_found && deref_alias) {

				strcpy ((char *) dns_fname, (char *) &cds_alias);
				c_results->d2_al_deref = TRUE;
			}
				
			/*
				Assign the attribute type and value.		
														*/
			if (d27_301_oid_str (&gds_attr->d2_avatype,
					(char *)type_str) == D2_ERROR) {

				comp_error->d2_retcode = D2_CALL_ERR;
				comp_error->d2_errclass = D2_PAR_ERR;
				comp_error->d2_errvalue = D2_NO_BIND;
				DUA_TRACE_D27_301_FAILED
				return (D2_ERROR);
			};

			d21_cds_uncompress_oid (type_str, uncompressed_oid);
        	dns_type = (dns_attr_name_t *) uncompressed_oid;

			if (d21_x500val2cdsval (&gds_attr->d2_avaval, &dns_value) 
				== D2_ERROR) {

				comp_error->d2_retcode = D2_CALL_ERR;
                comp_error->d2_errclass = D2_PAR_ERR;
                comp_error->d2_errvalue = D2_CONSTRAINT_VIOLATION;
                d21_cds_attr_error (dns_fname, &gds_attr->d2_avatype, 
					&comp_par_blk->d21_6err);
				r_code = D2_ERROR;

			} else {

				/*
					Compare an attribute of a CDS object.
															*/
				if (strcmp ((char *) type_str, DSW_MEMBER_OID) == 0) {
					
					attr_value= gds_attr->d2_avaval.d2_at_value.d2_obj_name_a_v;

					if (d21_x500name2cdsname (attr_value, member) == D2_ERROR) {

						DUA_TRACE_X500_TO_CDS_NAME_FAILED
						comp_error->d2_retcode = D2_CALL_ERR;
						comp_error->d2_errclass = D2_PAR_ERR;
						comp_error->d2_errvalue = D2_NOT_SPEC;
						return (D2_ERROR);
            		}

					dns_member = (dns_full_name_t *) member;

					if ((r_code = cdsTestMbr (dns_fname, dns_member, 
						(int *) NULL, (void *) NULL, (void *) NULL, &flags))
						== DNS_ERROR) {
						DUA_TRACE_CDS_FUNCTION_FAILED("cdsTestMbr")
						d21_cds_map_error (flags.fsLocStat, comp_error);
						d21_cds_error_info (comp_error, dns_fname, 
													&comp_par_blk->d21_6err);
					}

				} else {

					if ((r_code = cdsTestAttrValue (dns_fname, dns_type, dnsObj,
						&dns_value, &flags)) == DNS_ERROR) {
						DUA_TRACE_CDS_FUNCTION_FAILED("cdsTestAttrValue")
						d21_cds_map_error (flags.fsLocStat, comp_error);
						d21_cds_error_info (comp_error, dns_fname, 
													&comp_par_blk->d21_6err);
						r_code = D2_ERROR;
					}
				}

				if (r_code == DNS_SUCCESS) {

					comp_par_blk->d21_6match = TRUE;
					r_code = D2_NOERROR;

				} else if (r_code == DNS_NOTFND) {

					comp_par_blk->d21_6match = FALSE;
					r_code = D2_NOERROR;
				}

				if (flags.fsFromCache) {
					comp_par_blk->d21_6from_entry = FALSE;
				}

			} 
		} 
	}

	DUA_TRACE_IAPL_EXIT(comp_par_blk->d21_6pbhead->d2_errvalue)
    return (r_code);
}




/*-----------------------------------------------------------------------

	comp_aon_attr():

	Compares a GDS aliasedObjectName attribute and a CDS alias. 

 -----------------------------------------------------------------------*/

static int	comp_aon_attr (
		D2_ava			*aon_attr,
		dns_full_name_t	*cds_alias,
		int				*matched,
		D2_pbhead		*comp_error)
{

	D2_a_type	*attr_type;
	D2_a_value	*attr_value;
	u_char		*aon_value;
	int			r_code;

	r_code = D2_NOERROR;
	attr_type = &aon_attr->d2_avatype;
	attr_value = &aon_attr->d2_avaval;

	if (strcmp ((char *) attr_type->d2_type, DSW_AON_OID) == 0 ) {

		if (attr_value->d2_a_rep != D2_DISTNAME) {
			comp_error->d2_retcode = D2_CALL_ERR;
			comp_error->d2_errclass = D2_PAR_ERR;
			comp_error->d2_errvalue = D2_NO_BIND;
			r_code = D2_ERROR;
		} else {
			aon_value = attr_value->d2_at_value.d2_obj_name_a_v;
			if (strcmp ((char *) cds_alias, (char *) aon_value) == 0) {
				*matched = TRUE;
			} else {
				*matched = FALSE;
			}
		}
	}
	
	return (r_code);
}
						
