/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_cdsmod.c,v $
 * Revision 1.1.11.2  1996/02/18  18:18:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:01  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:55:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:10  root]
 * 
 * Revision 1.1.9.2  1994/05/10  15:54:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:34:07  marrek]
 * 
 * Revision 1.1.9.1  1994/02/22  18:06:48  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:38:37  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  06:51:35  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:03:04  marrek]
 * 
 * Revision 1.1.5.5  1993/02/02  15:39:33  marrek
 * 	Fixes for OT 6558
 * 	[1993/02/02  11:08:24  marrek]
 * 
 * Revision 1.1.5.4  1992/12/31  19:49:30  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:27:01  bbelch]
 * 
 * Revision 1.1.5.3  1992/11/16  08:58:43  marrek
 * 	Remove duplicated history line, empty #ifndef lint and #ifdef ANSIC,
 * 	replace SNI_SVR4 by SNI_DLOPEN,
 * 	[1992/11/06  11:39:22  marrek]
 * 
 * Revision 1.1.5.2  1992/09/29  21:10:47  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:24:44  treff]
 * 
 * Revision 1.1.2.3  1992/06/30  20:59:23  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:40:10  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:38:21  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:39:25  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_cdsmod.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 18:18:06 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/*---------------------------------------------------------------------

	Filename:		d21_cdsmod.c

	Author:			P. Dunne
	Date:			1.02.1991

	Component:		OSF DCE Naming Switch

	Functions:	
		exported:	d21_C_switch().

		static:		cancel_changes(),	add_attr_values(),
					rem_attr(),			rem_attr_values(),
					check_for_member(),	add_member_values(),
					rem_member_values().

	Version:		1.0 (initial)

	Update History:
	Version:		Date:		Comments:			 	By whom:
	--------------------------------------------------------------------
	1.0				1.02.91		original				P. Dunne


 -----------------------------------------------------------------------*/

#include <gds.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

#include <d21dua.h>
#include <dce/dnsclerk.h>
#include <d21_cdssw.h>

/*-----------------  local function prototypes  ------------------------*/

static int	cancel_changes (	
				D21_pC		*mod_par_blk,
				int			no_of_mods,	
				int			no_of_values);
 
static int	add_attr_values (
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_fname,
				int					*mods_count,
				struct dnsFlagStat	*flags,
				D2_pbhead			*mod_error,
				D2_error            **mod_error_info);

static int	add_member_values (
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_fname,
				int					*mods_count,
				struct dnsFlagStat	*flags,
				D2_pbhead			*mod_error);

static int	rem_member_values (
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_fname,
				int					*mods_count,
				struct dnsFlagStat	*flags,
				D2_pbhead			*mod_error);

static int	rem_attr (
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_fname,
				int					*mods_count,
				struct dnsFlagStat	*flags,
				D2_pbhead			*mod_error);

static int	rem_attr_values (
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_fname,
				int					*mods_count,
				struct dnsFlagStat	*flags,
				D2_pbhead			*mod_error,
				D2_error			**mod_error_info);

static int	check_for_member (	
				D2_a_info	*attr_list,
				int			*found);

/*--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------

	d21_C_switch():

	Modifies an attribute in the DNS local cell. The modify operation 
	is received at the IAPL* interface and has to be mapped to DNSPI 
	interface calls. There are four types of object in DNS: 

	    add attribute, 	add attribute value, 
	    remove attribute 	remove attribute value

	Seperate functions must be called to carry out each of these types 
	of modification.

	The result from the DNSPI modify operations are mapped to the 
	IAPL* modify operation result.

 -----------------------------------------------------------------------*/


int		d21_C_switch (
			D21_pC	*mod_par_blk)
{
	dns_full_name_t		*dns_fname;
	struct dnsFlagStat	dns_flags;
	dns_opq_fname		part_result;
	D2_a_modinf			*changes;
	D2_a_info			*attr_list;
	D2_pbhead			*mod_error;
	u_char				cds_name[D2_DNL_MAX];
	int					member_found,	mods_count,		count,		
						r_code,			error;
	const char function_name[] = "d21_C_switch";

	DUA_TRACE_IAPL_ENTRY
	r_code = D2_NOERROR;
	error = FALSE;

	/*
		the pbhead structure is malloced by the IAPL* interface
																	*/
	mod_error = mod_par_blk->d21_Cpbhead;
	if (d21_x500name2cdsname (mod_par_blk->d21_Centry, cds_name)
		== D2_ERROR) {
		DUA_TRACE_X500_TO_CDS_NAME_FAILED
		mod_error->d2_retcode = D2_CALL_ERR;
        mod_error->d2_errclass = D2_PAR_ERR;
        mod_error->d2_errvalue = D2_NOT_SPEC;
        return (D2_ERROR);
	}

    dns_fname = (dns_full_name_t *) cds_name;

	/*
		IAPL* common arguments are mapped to CDS flags structure.
																	*/
	(void) d21_cds_set_flags (&dns_flags, mod_par_blk->d21_Cc_a);
	dns_flags.fsPartialRes = &part_result;
	dns_flags.fsResLength = sizeof (part_result);

	/*
	    Check what type of modification is required. 
																	*/
	changes = mod_par_blk->d21_Cmod;
	mods_count = 0;
	while ((changes != (D2_a_modinf *) NULL) && !error) {
		
		attr_list = &changes->d2_m_attr;
		if (check_for_member (attr_list, &member_found) == D2_ERROR) {
			mod_error->d2_retcode = D2_CALL_ERR;
			mod_error->d2_errclass = D2_PAR_ERR;
			mod_error->d2_errvalue = D2_NO_BIND;
			return (D2_ERROR);
		} else  {

		  	switch (changes->d2_m_mod) {

			  case D2_ADDATT:
			  case D2_ADD_AV:
				if (member_found) {
					if ((r_code = add_member_values (attr_list, dns_fname, 
							&count, &dns_flags, mod_error)) == D2_ERROR) {
						(void) cancel_changes (mod_par_blk, mods_count, count);
						error = TRUE;
					}
				} else {
					if ((r_code = add_attr_values (attr_list, dns_fname,&count, 
						&dns_flags, mod_error, &mod_par_blk->d21_Cerr)) 
						== D2_ERROR) {

						(void) cancel_changes (mod_par_blk, mods_count, count);
						error = TRUE;
					}
				}
				break;

			  case D2_DELATT:
				if ((r_code = rem_attr (attr_list, dns_fname, &count, 
					&dns_flags, mod_error)) == D2_ERROR) {

					(void) cancel_changes (mod_par_blk, mods_count, count);
					error = TRUE;
				}
				break;

			  case D2_DEL_AV:
				if (member_found) {

					if ((r_code = rem_member_values (attr_list, dns_fname,
							&count, &dns_flags, mod_error)) == D2_ERROR) {
						(void) cancel_changes (mod_par_blk, mods_count, count);
						error = TRUE;
					}

				} else {

					if ((r_code = rem_attr_values (attr_list, dns_fname,&count, 
						&dns_flags, mod_error, &mod_par_blk->d21_Cerr)) 
						== D2_ERROR) {

						(void) cancel_changes (mod_par_blk, mods_count, count);
						error = TRUE;
					}
				}
				break;

			  default:

				(void) cancel_changes (mod_par_blk, mods_count, count);
				mod_error->d2_retcode = D2_CALL_ERR;
				mod_error->d2_errclass = D2_PAR_ERR;
				mod_error->d2_errvalue = D2_NO_BIND;
				return (D2_ERROR);
			}

			changes = changes->d2_m_next;
		}
	}

	/*
		None of the three common results are currently valid for 
		modify so the common results parameter is ignored.
																	*/

	DUA_TRACE_IAPL_EXIT(mod_par_blk->d21_Cpbhead->d2_errvalue)
	return (r_code);
}



/*-----------------------------------------------------------------------

	cancel_changes():

	If there are any errors in any part of the modify operation an
	error is returned and the entry to be modified is left in the 
	state it was before the modify operation commanced.

	This function cancells any parts of the modify operation that 
	had completed successfully before the error occurred.

 -----------------------------------------------------------------------*/

static int		cancel_changes (	
					D21_pC		*mod_par_blk,
					int			no_of_mods,	
					int			no_of_values)
{
	dns_full_name_t		*dns_fname;
	struct dnsFlagStat	dns_flags;
	dns_opq_fname		part_result;
	D2_a_modinf			*changes;
	D2_a_info			*attr_list;
	D2_pbhead			*mod_error;
	u_char				cds_name[D2_DNL_MAX];
	int	mods_count, count, r_code,	error;
	const char function_name[] = "cancel_changes";

	r_code = D2_NOERROR;
	error = FALSE;

	/*
		the pbhead structure is malloced by the IAPL* interface
																	*/
	mod_error = mod_par_blk->d21_Cpbhead;

	if (d21_x500name2cdsname (mod_par_blk->d21_Centry, cds_name)
		== D2_ERROR) {
		DUA_TRACE_X500_TO_CDS_NAME_FAILED
		mod_error->d2_retcode = D2_CALL_ERR;
		mod_error->d2_errclass = D2_PAR_ERR;
		mod_error->d2_errvalue = D2_NOT_SPEC;
		return (D2_ERROR);
	};

    dns_fname = (dns_full_name_t *) cds_name;

	/*
		IAPL* common arguments are mapped to CDS flags structure.
																	*/
	(void) d21_cds_set_flags (&dns_flags, mod_par_blk->d21_Cc_a);
	dns_flags.fsPartialRes = &part_result;
	dns_flags.fsResLength = sizeof (part_result);

	/*
	    Check what type of modification is required. 
													*/
	changes = mod_par_blk->d21_Cmod;
	mods_count = 0;

	while ((mods_count != no_of_mods) && !error) {

		attr_list = &changes->d2_m_attr;

		switch (changes->d2_m_mod) {

			case D2_DELATT:
			case D2_DEL_AV:
				/*
					Add attributes and attribute values that have 
					previously been removed.
																	*/
				if ((r_code = add_attr_values (attr_list, dns_fname, &count, 
					&dns_flags, mod_error, &mod_par_blk->d21_Cerr)) 
					== D2_ERROR) {

					error = TRUE;
				}
				break;

			case D2_ADDATT:
				/*
					Remove attributes that have previously been addded.
																		*/
				if ((r_code = rem_attr (attr_list, dns_fname, &count, 
					&dns_flags, mod_error)) == D2_ERROR) {
					error = TRUE;
				}
				break;

			case D2_ADD_AV:
				/*
					Remove all the attribute values that had been 
					previously added.
																*/
				if ((r_code = rem_attr_values (attr_list, dns_fname, &count, 
					&dns_flags, mod_error, &mod_par_blk->d21_Cerr)) 
					== D2_ERROR) {

					error = TRUE;
				}
				break;

			default:
				error = TRUE;
				r_code = D2_ERROR;
				break;
		}

		changes = changes->d2_m_next;
		mods_count ++;
	}

	return (r_code);
}



/*-----------------------------------------------------------------------

	add_attr_values():

	Adds an attribute to a CDS object.

 -----------------------------------------------------------------------*/

static int	add_attr_values (
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_fname,
				int					*mods_count,
				struct dnsFlagStat	*flags,
				D2_pbhead			*mod_error,
				D2_error            **mod_error_info)
{
	dns_attr_name_t		*dns_type;
	dns_attrvalue_t		*dns_value;
	D2_a_value			*val_struct;
	u_char          	uncompressed_oid [D2_OBJ_IDL_MAX],
                    	type_str[D2_OBJ_IDL_MAX];
	int					r_code,			error,		count;
	const char function_name[] = "add_attr_values";

	r_code = D2_NOERROR;
	error = FALSE;
	*mods_count = 0;

	/*
		Convert the GDS attribute type to CDS format.
															*/
	if (d27_301_oid_str (&attr_list->d2_a_type,(char *)type_str)
				== D2_ERROR) {

		DUA_TRACE_D27_301_FAILED
		mod_error->d2_retcode = D2_CALL_ERR;
		mod_error->d2_errclass = D2_PAR_ERR;
		mod_error->d2_errvalue = D2_NO_BIND;
		return (D2_ERROR);
    };

	d21_cds_uncompress_oid (type_str, uncompressed_oid);
    dns_type = (dns_attr_name_t *) uncompressed_oid;

	/*
		Adding an attribute or attribute value is done using 
		the dnsAddAttrvalue() function in DNS. If you try to 
		add an attribute value to an attribute that doesn't 
		exist CDS just creates the attribute first.
														*/
	val_struct = attr_list->d2_a_val;
	count = 0;
	while ((count != attr_list->d2_a_no_val) && !error) {

	    if ((dns_value = D27_MALLOC(svc_c_sev_warning,sizeof(dns_attrvalue_t)))
			== NULL)
	      {
	      mod_error->d2_retcode = D2_TEMP_ERR;
	      mod_error->d2_errclass = D2_SYS_ERR;
	      mod_error->d2_errvalue = D21_ALLOC_ERROR;
	      return (D2_ERROR);
	      }
		if (d21_x500val2cdsval (val_struct, dns_value) == D2_ERROR) {

			mod_error->d2_retcode = D2_CALL_ERR;
			mod_error->d2_errclass = D2_PAR_ERR;
			mod_error->d2_errvalue = D2_CONSTRAINT_VIOLATION;
			d21_cds_attr_error (dns_fname, &attr_list->d2_a_type,
				mod_error_info);
			error = TRUE;
			r_code = D2_ERROR;

		} else {
			/*
				Add an attribute value to a CDS object or CDS directory.
																		*/
			r_code = cdsAddAttrValue (dns_fname, dns_type, dnsSet, dnsObj,
				dns_value, NULL_DNS_CTS, flags);
			
			if (flags->fsLocStat == DNS_UNKNOWNENTRY) {
				r_code = cdsAddAttrValue (dns_fname, dns_type, dnsSet, dnsDir,
					dns_value, NULL_DNS_CTS, flags);
			}

			if (r_code == DNS_ERROR) {

				d21_cds_map_error (flags->fsLocStat, mod_error);
				error = TRUE;
				r_code = D2_ERROR;

			} else {

				r_code = D2_NOERROR;
			}

			(*mods_count)++;
			val_struct ++;
			count ++;
		}
	}

	return (r_code);
}




/*-----------------------------------------------------------------------

	rem_attr():

	Deletes an attribute from a CDS object.

 -----------------------------------------------------------------------*/

static int	rem_attr (
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_fname,
				int					*mods_count,
				struct dnsFlagStat	*flags,
				D2_pbhead			*mod_error)
{
	dns_attr_name_t		*dns_type;
	u_char          	uncompressed_oid [D2_OBJ_IDL_MAX],
                    	type_str[D2_OBJ_IDL_MAX];
	int	r_code,	error;
	const char function_name[] = "rem_attr";

	r_code = D2_NOERROR;
	error = FALSE;
	*mods_count = 0;

	/*
		Convert the attribute type OID from IAPL* format 
		into uncompressed string format.							
															*/
	if (d27_301_oid_str (&attr_list->d2_a_type,(char *)type_str)
				== D2_ERROR) {

		DUA_TRACE_D27_301_FAILED
		mod_error->d2_retcode = D2_CALL_ERR;
		mod_error->d2_errclass = D2_PAR_ERR;
		mod_error->d2_errvalue = D2_NO_BIND;
		return (D2_ERROR);
    };

	d21_cds_uncompress_oid (type_str, uncompressed_oid);
    dns_type = (dns_attr_name_t *) uncompressed_oid;

	/*
		Remove an attribute from a CDS object or CDS directory.
																*/
	r_code = cdsRmAttr (dns_fname, dns_type, dnsSet, dnsObj,NULL_DNS_CTS,flags);

	if (flags->fsLocStat == DNS_UNKNOWNENTRY) {

		r_code = cdsRmAttr (dns_fname, dns_type, dnsSet, dnsDir, NULL_DNS_CTS,
					flags);
	}


	if (r_code == DNS_SUCCESS) {

		r_code = D2_NOERROR;

	} else if (r_code == DNS_ERROR) {

		d21_cds_map_error (flags->fsLocStat, mod_error);
		r_code = D2_ERROR;

	} else {

		(*mods_count)++;
	}

	return (r_code);
}





/*-----------------------------------------------------------------------

	rem_attr_values():

	Deletes an attribute value from a CDS attribute.

 -----------------------------------------------------------------------*/

static int	rem_attr_values (
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_fname,
				int					*mods_count,
				struct dnsFlagStat	*flags,
				D2_pbhead			*mod_error,
				D2_error			**mod_error_info)
{
	dns_attr_name_t		*dns_type;
	dns_attrvalue_t		dns_value;
	D2_a_value			*val_struct;
	u_char          	uncompressed_oid [D2_OBJ_IDL_MAX],
                    	type_str[D2_OBJ_IDL_MAX];
	int					r_code,		error,	count;
	const char function_name[] = "rem_attr_values";

	r_code = D2_NOERROR;
	error = FALSE;
	*mods_count = 0;

	/*
		Convert the attribute type OID from IAPL* format 
		into uncompressed string format.							
															*/
	if (d27_301_oid_str (&attr_list->d2_a_type,(char *)type_str)
				== D2_ERROR) {

		DUA_TRACE_D27_301_FAILED
		mod_error->d2_retcode = D2_CALL_ERR;
		mod_error->d2_errclass = D2_PAR_ERR;
		mod_error->d2_errvalue = D2_NO_BIND;
		return (D2_ERROR);
    };

	d21_cds_uncompress_oid (type_str, uncompressed_oid);
    dns_type = (dns_attr_name_t *) uncompressed_oid;

	/*
		Several attribute values can be removed from a given attribute. 
																		*/
	val_struct = attr_list->d2_a_val;
	count = 0;

	while ((count != attr_list->d2_a_no_val) && !error) {

		if ((r_code = d21_x500val2cdsval(val_struct, &dns_value)) != D2_ERROR) {

			/*
				Remove an attribute value from a CDS object.  
															*/
			if (cdsRmAttrValue (dns_fname, dns_type, dnsObj, &dns_value,
				NULL_DNS_CTS, flags) == DNS_ERROR) {
				DUA_TRACE_CDS_FUNCTION_FAILED("cdsRmAttrValue")
				d21_cds_map_error (flags->fsLocStat, mod_error);
				error = TRUE;
				r_code = D2_ERROR;

			} else {

				val_struct ++;
				(*mods_count)++;
				count ++;
			}

		} else {

			mod_error->d2_retcode = D2_CALL_ERR;
			mod_error->d2_errclass = D2_PAR_ERR;
			mod_error->d2_errvalue = D2_CONSTRAINT_VIOLATION;
			d21_cds_attr_error (dns_fname, &attr_list->d2_a_type, 
				mod_error_info);
			error = TRUE;
			r_code = D2_ERROR;
		} 
	}

	return (r_code);
}



/*-----------------------------------------------------------------------

	check_for_member ():

	Given an attribute list, checks if the attribute member is in this
	list.

 -----------------------------------------------------------------------*/

static int		check_for_member (	
					D2_a_info	*attr_list,
					int			*found)
{
	u_char		type_str[D2_DNL_MAX];
	int			error,	r_code;
	const char function_name[] = "check_for_member";

	r_code = D2_NOERROR;
	error = FALSE;
	*found = FALSE;

	while ((!*found) && (!error) && (attr_list != (D2_a_info *) NULL)) {

		if (d27_301_oid_str (&attr_list->d2_a_type,(char *)type_str)
					== D2_ERROR) {

			DUA_TRACE_D27_301_FAILED
			r_code  = D2_ERROR;
			error = TRUE;

		} else {
		
			if (strcmp ((char *)type_str, DSW_MEMBER_OID) == 0) 
				*found = TRUE;
			else 
				attr_list = attr_list->d2_a_next;
		}
	}

	return (r_code);
}



/*-----------------------------------------------------------------------

	add_member_values():

	Adds members to a CDS group.

 -----------------------------------------------------------------------*/

static int	add_member_values (
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_fname,
				int					*mods_count,
				struct dnsFlagStat	*flags,
				D2_pbhead			*mod_error)
{
	dns_full_name_t		*dns_member;
	dns_attr_name_t		*dns_type;
	D2_a_value			*val_struct;
	u_char          	uncompressed_oid [D2_OBJ_IDL_MAX],
                    	type_str[D2_OBJ_IDL_MAX],
    					*attr_value,    member[D2_DNL_MAX];
	int					r_code,			error,		count;
	const char function_name[] = "add_member_values";

	r_code = D2_NOERROR;
	error = FALSE;
	*mods_count = 0;

	/*
		Convert the attribute type OID from IAPL* format 
		into uncompressed string format.							
															*/
	if (d27_301_oid_str (&attr_list->d2_a_type,(char *)type_str)
				== D2_ERROR) {

		DUA_TRACE_D27_301_FAILED
		mod_error->d2_retcode = D2_CALL_ERR;
		mod_error->d2_errclass = D2_PAR_ERR;
		mod_error->d2_errvalue = D2_NO_BIND;
		return (D2_ERROR);
    };

	d21_cds_uncompress_oid (type_str, uncompressed_oid);
    dns_type = (dns_attr_name_t *) uncompressed_oid;

	/*
		In CDS, the member attribute is the only valid 
		attribute for a groupOfNames object.
														*/
	if (strcmp ((char *)type_str, DSW_MEMBER_OID) == 0) {
		/*
			Add the members to the CDS group object.
															*/
		val_struct = attr_list->d2_a_val;
		count = 0;
		while ((count != attr_list->d2_a_no_val) && !error) {

			if (val_struct->d2_a_rep != D2_DISTNAME) {
				/*
					assign the error code, class and value
															*/
				r_code = D2_ERROR;
				error = TRUE;
			} else {
				attr_value = val_struct->d2_at_value.d2_obj_name_a_v;
				if (d21_x500name2cdsname (attr_value, member) == D2_ERROR) {
					DUA_TRACE_X500_TO_CDS_NAME_FAILED
					mod_error->d2_retcode = D2_CALL_ERR;
                    mod_error->d2_errclass = D2_PAR_ERR;
                    mod_error->d2_errvalue = D2_NOT_SPEC;
                    return (D2_ERROR);
				};

				dns_member = (dns_full_name_t *) member;

				 /*
					add a group member to the groupOfNames object
																	*/
				if (cdsAddMbr (dns_fname, dns_member, dnsPrincipal, flags)
                	== DNS_ERROR) {

					DUA_TRACE_CDS_FUNCTION_FAILED("cdsAddMbr")
					d21_cds_map_error (flags->fsLocStat, mod_error);
					error = TRUE;
					r_code = D2_ERROR;
				} else {

					(*mods_count)++;
					val_struct ++;
					count ++;
				}
			}
		}
	}

	return (r_code);
}




/*-----------------------------------------------------------------------

	rem_member_values():

	Removes members from a CDS group.

 -----------------------------------------------------------------------*/

static int	rem_member_values (
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_fname,
				int					*mods_count,
				struct dnsFlagStat	*flags,
				D2_pbhead			*mod_error)
{
	dns_full_name_t		*dns_member;
	dns_attr_name_t		*dns_type;
	D2_a_value			*val_struct;
	u_char          	uncompressed_oid [D2_OBJ_IDL_MAX],
                    	type_str[D2_OBJ_IDL_MAX],
    					*attr_value,    member[D2_DNL_MAX];
	int					r_code,			error,		count;
	const char function_name[] = "rem_member_values";

	r_code = D2_NOERROR;
	error = FALSE;
	*mods_count = 0;

	/*
		Convert the attribute type OID from IAPL* format 
		into uncompressed string format.							
															*/
	if (d27_301_oid_str (&attr_list->d2_a_type,(char *)type_str)
				== D2_ERROR) {

		DUA_TRACE_D27_301_FAILED
		mod_error->d2_retcode = D2_CALL_ERR;
		mod_error->d2_errclass = D2_PAR_ERR;
		mod_error->d2_errvalue = D2_NO_BIND;
		return (D2_ERROR);
    };

	d21_cds_uncompress_oid (type_str, uncompressed_oid);
    dns_type = (dns_attr_name_t *) uncompressed_oid;

	/*
		In CDS, the member attribute is the only valid 
		attribute for a groupOfNames object.
														*/
	if (strcmp ((char *)type_str, DSW_MEMBER_OID) == 0) {
		/*
			Add the members to the CDS group object.
															*/
		val_struct = attr_list->d2_a_val;
		count = 0;
		while ((count != attr_list->d2_a_no_val) && !error) {

			if (val_struct->d2_a_rep != D2_DISTNAME) {
				/*
					assign the error code, class and value
															*/
				r_code = D2_ERROR;
				error = TRUE;
			} else {
				attr_value = val_struct->d2_at_value.d2_obj_name_a_v;
				if (d21_x500name2cdsname (attr_value, member)
					== D2_ERROR ) {
					DUA_TRACE_X500_TO_CDS_NAME_FAILED
					mod_error->d2_retcode = D2_CALL_ERR;
                    mod_error->d2_errclass = D2_PAR_ERR;
                    mod_error->d2_errvalue = D2_NOT_SPEC;
                    return (D2_ERROR);
				};

				dns_member = (dns_full_name_t *) member;

				 /*
					add a group member to the groupOfNames object
																	*/
				if (cdsRmMbr (dns_fname, dns_member, dnsPrincipal, flags)
                	== DNS_ERROR) {
					DUA_TRACE_CDS_FUNCTION_FAILED("cdsRmMbr")
					d21_cds_map_error (flags->fsLocStat, mod_error);
					error = TRUE;
					r_code = D2_ERROR;
				} else {

					(*mods_count)++;
					val_struct ++;
					count ++;
				}
			}
		}
	}

	return (r_code);
}
