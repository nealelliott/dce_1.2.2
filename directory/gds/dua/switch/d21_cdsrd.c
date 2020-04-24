/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_cdsrd.c,v $
 * Revision 1.1.11.2  1996/02/18  18:18:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:03  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:55:30  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/08  21:19 UTC  smythe
 * 	fix for CHFts15346
 * 	[1995/12/08  15:11:12  root]
 * 
 * Revision 1.1.9.4  1994/08/17  13:56:47  keutel
 * 	OT 11222: adding CDS class and CDS class version via XDS
 * 	[1994/08/17  12:49:53  keutel]
 * 
 * Revision 1.1.9.3  1994/08/09  12:02:11  keutel
 * 	OT 9890: dynamic reallocation
 * 	[1994/08/04  11:14:35  keutel]
 * 
 * Revision 1.1.9.2  1994/05/10  15:54:26  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:34:19  marrek]
 * 
 * Revision 1.1.9.1  1994/02/22  18:06:50  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:38:56  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  06:53:26  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:03:28  marrek]
 * 
 * Revision 1.1.5.8  1993/03/17  19:38:03  treff
 * 	DCT @ OSF, for Helmut Volpers@sni
 * 	Fix unitialized return code, invert one
 * 	if, to get switch really working on pMax.
 * 	[1993/03/17  19:36:52  treff]
 * 
 * Revision 1.1.5.7  1993/02/04  13:28:03  marrek
 * 	Fixes for OT 6597.
 * 	[1993/02/04  13:24:32  marrek]
 * 
 * Revision 1.1.5.6  1993/02/02  15:39:55  marrek
 * 	Fixes for OT 6678, 6751, 6967
 * 	[1993/02/02  11:09:33  marrek]
 * 
 * Revision 1.1.5.5  1993/02/02  10:18:09  marrek
 * 	Fixes for OT 6554
 * 	insert <pthread.h>
 * 	[1993/02/02  09:32:25  marrek]
 * 
 * Revision 1.1.5.4  1992/12/31  19:49:34  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:27:06  bbelch]
 * 
 * Revision 1.1.5.3  1992/11/16  08:59:02  marrek
 * 	Remove empty #ifndef lint and #ifdef ANSIC,
 * 	replace #include <malloc.h> with #include <string.h>,
 * 	integrate changes for BUG 5144, proposed by Ron Monzillo.
 * 	Replace SNI_SVR4 by SNI_DLOPEN.
 * 	[1992/11/06  11:42:04  marrek]
 * 
 * Revision 1.1.5.2  1992/09/29  21:10:54  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:24:55  treff]
 * 
 * Revision 1.1.2.4  1992/06/30  20:59:29  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:40:18  melman]
 * 
 * Revision 1.1.2.3  1992/06/02  03:38:27  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:39:32  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:44:48  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:39:02  weisman]
 * 
 * Revision 1.1  1992/01/19  15:13:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_cdsrd.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 18:18:08 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/*---------------------------------------------------------------------

	Filename:		d21_cdsrd.c 

	Author:			P. Dunne
	Date:			1.02.1991

	Component:		OSF DCE Naming Switch

	Functions:	
		exported:       d21_3_switch().

		static:         read_attr_values(),    enum_attr_types(),
		                match_attr_types(),    copy_attr_types(),
		                encode_attr_types(),   map_alias(),
		                setval_attr(),         setval_member(),		
		                setsyntax_objectClass() setup_for_read(),
		                free_read_result(),    free_attr_list(),
		                free_attr_values(),    size_attr_values(),
		                size_read_result(),    make_result_contiguous(),
		                is_dotted_oid (),      make_values_contiguous().
					

	Version:		1.0 (initial)

	Update History:
	Version:    Date:    Comments:                By whom:
	--------------------------------------------------------------------
	1.0         1.02.91  original                 P. Dunne


 -----------------------------------------------------------------------*/

#include <gds.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include <d21dua.h>
#include <dce/dnsclerk.h>
#include <d21_cdssw.h>


/*--------------------  local static function prototyopes --------------*/

static int		setup_for_read (	
					D21_p3      *read_par_blk,
					D2_pbhead	*read_error);

static int		read_attr_values (	
					D2_a_info			**attr_list,
					dns_full_name_t		*dns_fname,
					struct dnsFlagStat	*flags,
					D2_pbhead			*read_error);

static int		enum_attr_types (	
					D2_a_info			*attr_list,
					dns_full_name_t		*dns_fname,
					struct dnsFlagStat	*flags,
					dns_opq_fname		*part_result,
					D2_pbhead			*read_error);

static int		match_attr_types (	
					D2_a_info	**attributes,
					D2_req_info	*attr_types,
					D2_pbhead	*read_error);

static int		copy_attr_types (	
					D2_a_info	**attributes,
					D2_req_info	*attr_types,
					D2_pbhead	*read_error);

static int		setval_attr (	
					D2_a_info			*attr_list,
					dns_full_name_t		*dns_fname,
					struct dnsFlagStat	*flags,
					D2_pbhead			*read_error);

static int		setval_member (	
					D2_a_info			*attr_list,
					dns_full_name_t		*dns_fname,
					struct dnsFlagStat	*flags,
					D2_pbhead			*read_error);

static int 		map_alias (	
					D2_a_info			*attr_list,
					dns_full_name_t		*dns_alias,
					int					eis_values,
					struct dnsFlagStat	*flags,
					D2_pbhead			*read_error);

static int		encode_attr_types (	
					D2_a_info			*attr_list,
					D2_pbhead			*read_error);

static int		decode_attr_types (	
					D2_a_info			*attr_list,
					D2_pbhead			*read_error);

static void		setsyntax_objectClass (	
					D2_a_info			*attr_list);

static int		is_dotted_oid (
					u_char	*str);

static void		free_read_result (	
					D2_ei_info	*read_result);

static void		free_attr_list (	
					D2_a_info	*attr_list);

static void		free_attr_values (	
					D2_a_info	*attr_list);

static int		size_read_result (	
					D2_ei_info	*read_result);

static int		size_attr_values (	
					D2_a_info	*attr_list);

static int		make_result_contiguous (	
					D2_ei_info	**read_result,
					D2_pbhead	*read_error);

static int		make_values_contiguous (	
					D2_a_info	*attr_list,
					D2_a_info	*contig_list,
					char		**tail_address);

/*--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------

	d21_3_switch():

	Reads an object from the local DNS cell. The read operation is
	received at the IAPL* interface and has to be mapped to a
	DNSPI interface calls. The result from the DNSPI read operation
	is then mapped to the IAPL* read operation result.

 -----------------------------------------------------------------------*/

int		d21_3_switch (	
			D21_p3		*read_par_blk)
{
	dns_full_name_t		*dns_fname,		cds_alias;
	dns_opq_fname		part_result;
	struct dnsFlagStat	flags;
	D2_a_info			**p_attr_list;
	D2_pbhead			*read_error;
	D2_c_res			*c_results;
	u_char				cds_name[D2_DNL_MAX], iapl_alias[D2_DNL_MAX];
	int	eis_all, eis_values, deref_alias, alias_found, r_code;
	const char function_name[] = "d21_3_switch";

	DUA_TRACE_IAPL_ENTRY
	r_code = D2_NOERROR;

	/*
		The D2_pbhead and D2_c_res structures are allocted by 
		the IAPL* user.
																*/
	read_error = read_par_blk->d21_3pbhead;
	c_results = read_par_blk->d21_3cres;

	if (setup_for_read (read_par_blk, read_error) == D2_ERROR) {
		return (D2_ERROR);
	}
	p_attr_list = &read_par_blk->d21_3res->d2_ei_ai;

	/*
		Sets read operation entry information selection flags
		    eis_all		=*=	select all attributes
		    eis_values	=*=	select attribute types and values
																*/
	if (read_par_blk->d21_3r_info->d2_r_no_at == D2_ALL_ATT_REQ)
		eis_all = TRUE;
	else
		eis_all = FALSE;

	if (read_par_blk->d21_3r_info->d2_r_val_at == D2_R_VAL_TYP)
		eis_values = TRUE;
	else
		eis_values = FALSE;

	if (read_par_blk->d21_3c_a->d2_serv_cntrls & D2_DREFALIAS)
		deref_alias = FALSE;
	else 
		deref_alias = TRUE;

	/*
		IAPL* common arguments are mapped to CDS flags structure.
																	*/
	(void) d21_cds_set_flags (&flags, read_par_blk->d21_3c_a);
	flags.fsPartialRes = &part_result;
	flags.fsResLength = sizeof (part_result);
	flags.fsMaybeMore = TRUE;

	if (d21_x500name2cdsname (read_par_blk->d21_3entry, cds_name) 
		== D2_ERROR) {
		DUA_TRACE_X500_TO_CDS_NAME_FAILED
		read_error->d2_retcode = D2_CALL_ERR;
        read_error->d2_errclass = D2_PAR_ERR;
        read_error->d2_errvalue = D2_NOT_SPEC;
        return (D2_ERROR);
	}

	dns_fname = (dns_full_name_t *) cds_name;

	/*
		checks to see if an alias has been traversed and 
																*/
	if (d21_cds_check_for_alias (dns_fname, &cds_alias, &alias_found,
		&flags, read_error) == D2_ERROR) {

		r_code = D2_ERROR;

	} else {

		if (alias_found && !deref_alias) {

			c_results->d2_al_deref = FALSE;
			r_code = map_alias (*p_attr_list, &cds_alias, eis_values, 
				&flags, read_error);

		} else {

			if (alias_found && deref_alias) {

				dns_fname = &cds_alias;
				c_results->d2_al_deref = TRUE;

				if (d21_cdsname2x500name ((u_char *)&cds_alias,
iapl_alias)
					== D2_ERROR) {
					DUA_TRACE_CDS_TO_X500_NAME_FAILED
					read_error->d2_retcode = D2_CALL_ERR;
					read_error->d2_errclass = D2_PAR_ERR;
					read_error->d2_errvalue = D2_NOT_SPEC;
					return (D2_ERROR);

				} else { 

					strcpy ((char *)read_par_blk->d21_3res->d2_ei_entry,(char *)iapl_alias);
				}

			} else { 

				c_results->d2_al_deref = FALSE;
			}
			/*
				IAPL* common arguments are mapped to CDS flags structure.
																		*/
			(void) d21_cds_set_flags (&flags, read_par_blk->d21_3c_a);
			flags.fsPartialRes = &part_result;
			flags.fsResLength = sizeof (part_result);
			flags.fsMaybeMore = TRUE;

			if (eis_all && !eis_values) {

				/*
					Read operation: where all attribute types are selected.
					Enumerate all attribute types of the DNS object.
																	*/
				if (enum_attr_types (*p_attr_list, dns_fname, &flags, &part_result,
					read_error) == D2_ERROR) {

					r_code = D2_ERROR;

				} else {

					if (flags.fsFromCache)
						read_par_blk->d21_3res->d2_ei_fromentry = FALSE;
					else 
						read_par_blk->d21_3res->d2_ei_fromentry = TRUE;

					if (encode_attr_types (*p_attr_list, read_error)
						== D2_ERROR) {
						
						r_code = D2_ERROR;
					}
				}

			} else if (eis_all && eis_values) {

				/*
					Read operation: where all attribute types and values
					are selected.
																			*/
				read_par_blk->d21_3res->d2_ei_fromentry = TRUE;

				if (enum_attr_types (*p_attr_list, dns_fname, &flags, &part_result,
					read_error) == D2_ERROR) {

					r_code = D2_ERROR;

				} else {

					if (flags.fsFromCache)
						read_par_blk->d21_3res->d2_ei_fromentry = FALSE;

					/*
						Read the CDS attribute values for all attributes in 
						the attribute list.						
																	*/
					if (read_attr_values (p_attr_list, dns_fname, &flags, 
					    read_error) == D2_ERROR) {

						r_code = D2_ERROR;

					} else {

						if (flags.fsFromCache)
							read_par_blk->d21_3res->d2_ei_fromentry = FALSE;
					}
				}

			} else if (!eis_all && !eis_values) {

				/*
					Read operation: selected attribute types are requested.
																		*/
				read_par_blk->d21_3res->d2_ei_fromentry = TRUE;
				
				if (enum_attr_types (*p_attr_list, dns_fname, &flags, &part_result,
					read_error) == D2_ERROR) {

					r_code = D2_ERROR;

				} else {

					if (flags.fsFromCache)
						read_par_blk->d21_3res->d2_ei_fromentry = FALSE;

					/*
						Remove all attribute types from the attribute list 
						that are not contained in the read operation 
						requested attribute list.  						
																		*/
					if (match_attr_types (p_attr_list,read_par_blk->d21_3r_info,
						read_error) == D2_ERROR) {

						r_code = D2_ERROR;

					} else {

						if (encode_attr_types (*p_attr_list, read_error)
							== D2_ERROR) {
							
							r_code = D2_ERROR;
						}
					}
				}

			} else if (!eis_all && eis_values) {

				/*
					Read operation: where selected attribute types and 
					values are requested.
																	*/
				read_par_blk->d21_3res->d2_ei_fromentry = TRUE;

				/*
					Copy the read operation requested attribute types to the
					attribute list.						
																	*/
				if (copy_attr_types (p_attr_list, read_par_blk->d21_3r_info, 
				    read_error) == D2_ERROR) {

					r_code = D2_ERROR;

				} else {

					/*
						Decode the IAPL* OIDs into uncompressed string format.
																			 */
					if (decode_attr_types (*p_attr_list, read_error)
						== D2_ERROR) {
						
						r_code = D2_ERROR;

					} else {

						/*
							Read the CDS attribute values for all attributes 
							in the attribute list.			
																			*/
						if (read_attr_values (p_attr_list, dns_fname, &flags, 
					    					read_error) == D2_ERROR) {

							r_code = D2_ERROR;

						} else {

							if (flags.fsFromCache)
								read_par_blk->d21_3res->d2_ei_fromentry = FALSE;
						}
					}
				}
			}
		}
	}

	if (r_code == D2_ERROR) {

		d21_cds_error_info (read_error, flags.fsPartialRes, 
											&read_par_blk->d21_3err);
		free_read_result (read_par_blk->d21_3res);
		read_par_blk->d21_3res = (D2_ei_info *) NULL;

	} else if (read_par_blk->d21_3res->d2_ei_ai == (D2_a_info *) NULL) {
		
		free ((char *) read_par_blk->d21_3res);
		read_par_blk->d21_3res = (D2_ei_info *) NULL;

	} else {
		
		r_code = make_result_contiguous (&read_par_blk->d21_3res, read_error);
	}

	DUA_TRACE_IAPL_EXIT(read_par_blk->d21_3pbhead->d2_errvalue)
	return (r_code);
}


/*-----------------------------------------------------------------------

	setup_for_read():

	Mallocs space for the read result structures.

 -----------------------------------------------------------------------*/

static int		setup_for_read (	
					D21_p3      *read_par_blk,
					D2_pbhead	*read_error)
{
	D2_a_info	*attr_list;
	char		*ei_space,	*list_space;
	int			r_code,		struct_size;

	r_code = D2_NOERROR;

	/*
		Malloc space for the entry information.
												*/
	struct_size = sizeof (D2_ei_info);
	if ((ei_space = D27_MALLOC(svc_c_sev_warning,struct_size + D2_DNL_MAX)) == NULL) {

		read_error->d2_retcode = D2_TEMP_ERR;
		read_error->d2_errclass = D2_SYS_ERR;
		read_error->d2_errvalue = D21_ALLOC_ERROR;
		return (D2_ERROR);
	}

	read_par_blk->d21_3res = (D2_ei_info *) ei_space;
	read_par_blk->d21_3res->d2_ei_next = (D2_ei_info *) NULL;
	read_par_blk->d21_3res->d2_ei_fromentry = TRUE;

	ei_space += struct_size;
	read_par_blk->d21_3res->d2_ei_entry = (D2_name_string) ei_space;
	strcpy ((char *)read_par_blk->d21_3res->d2_ei_entry,(char *)read_par_blk->d21_3entry);

	/*
		Malloc space for the attribute list. Always malloc space 
		for the attribute structure and the attribute type together. 
		This makes it a lot easier for freeing.
																	*/
	struct_size = sizeof (D2_a_info);
	if ((list_space = D27_MALLOC(svc_c_sev_warning,struct_size + D2_OBJ_IDL_MAX)) == NULL) {

		free ((char *) read_par_blk->d21_3res);
		read_par_blk->d21_3res = (D2_ei_info *) NULL;

		read_error->d2_retcode = D2_TEMP_ERR;
		read_error->d2_errclass = D2_SYS_ERR;
		read_error->d2_errvalue = D21_ALLOC_ERROR;
		return (D2_ERROR);
	}

	attr_list = (D2_a_info *) list_space;
	read_par_blk->d21_3res->d2_ei_ai = attr_list;

	attr_list->d2_a_next = (D2_a_info *) NULL;
	attr_list->d2_a_val = (D2_a_value *) NULL;
	attr_list->d2_a_no_val = 0;

	list_space += struct_size;
	attr_list->d2_a_type.d2_type = (Ob_id_string) list_space;
	
	return (r_code);
}


/*-----------------------------------------------------------------------

	enum_attr_types():

	Enumerates the attribute of an object given the object name.

 -----------------------------------------------------------------------*/

static int		enum_attr_types (	
					D2_a_info			*attr_list,
					dns_full_name_t		*dns_fname,
					struct dnsFlagStat	*flags,
					dns_opq_fname		*part_result,
					D2_pbhead			*read_error)
{
	dns_handle			handle;
	enum dnsAttrType	dns_attr_type;		
	enum dnsEntries		dns_ent_type;
	u_char				type_str [D2_OBJ_IDL_MAX];
	char				*list_space;
	int			struct_size,	len,		 r_code;
	const char function_name[] = "enum_attr_types";


	r_code = D2_NOERROR;

	if ((handle = cdsGetHandle()) == (dns_handle) NULL) {
		DUA_TRACE_CDS_FUNCTION_FAILED("cdsGetHandle")
		read_error->d2_retcode = D2_CALL_ERR;
		read_error->d2_errclass = D2_PAR_ERR;
		read_error->d2_errvalue = D2_NO_BIND;
		r_code = D2_ERROR;

	} else {

		/*
			Enumerate the attribute types of an entry. 
														*/
		len = D2_OBJ_IDL_MAX;
		r_code = cdsEnumAttr (dns_fname, handle, dnsObj, &dns_attr_type, 
					(void *)NULL, type_str, &len, flags);
	
		/*	
			If the entry is not a CDS object, check to see if it is
			a CDS child pointer.
																	*/
		if (flags->fsLocStat == DNS_UNKNOWNENTRY) {

			(void) cdsFreeHandle (handle);

			if ((handle = cdsGetHandle()) == (dns_handle) NULL) {
				DUA_TRACE_CDS_FUNCTION_FAILED("cdsGetHandle")
				read_error->d2_retcode = D2_CALL_ERR;
				read_error->d2_errclass = D2_PAR_ERR;
				read_error->d2_errvalue = D2_NO_BIND;
				r_code = D2_ERROR;

			} else {

				(void) memset ((char *) flags, 0, sizeof (struct dnsFlagStat));
				flags->fsPartialRes = part_result;
				flags->fsResLength = sizeof (*part_result);
				flags->fsMaybeMore = TRUE;
				flags->fsNameType = dnsStrXDS;

				len = D2_OBJ_IDL_MAX;
				r_code = cdsEnumAttr (dns_fname, handle, dnsDir, &dns_attr_type,
							(void *)NULL, type_str, &len, flags);
				dns_ent_type = dnsDir;
			}

		} else {

			dns_ent_type = dnsObj;
		}

		if (r_code != DNS_SUCCESS)
			{
			r_code = DNS_ERROR;
			}
		else {

			/*
				CDS may return either dotted string OIDS of short 
				name string OID types.
																	*/
			if (is_dotted_oid (type_str)) {

				strcpy ((char *)attr_list->d2_a_type.d2_type,(char *)type_str);
				attr_list->d2_a_type.d2_typ_len = strlen ((char *)type_str);
			
			} else {

				len = D2_OBJ_IDL_MAX;
				if (cdsCvtStrToObjID (type_str, (u_char *) NULL, 
					attr_list->d2_a_type.d2_type, &len,(dns_attr_name_t *)NULL, 
					(int *)NULL,  (u_char **) NULL) == DNS_ERROR) {
					DUA_TRACE_CDS_FUNCTION_FAILED("cdsCvtStrToObjID")
					read_error->d2_retcode = D2_CALL_ERR;
					read_error->d2_errclass = D2_PAR_ERR;
					read_error->d2_errvalue = D2_NO_BIND;
					r_code = D2_ERROR;
				}

				attr_list->d2_a_type.d2_typ_len = len - 1;
			}
		
			len = D2_OBJ_IDL_MAX;
			r_code = cdsEnumAttr (dns_fname, handle, dns_ent_type, 
				&dns_attr_type, (void *)NULL, type_str, &len, flags);
		}

		struct_size = sizeof (D2_a_info);

		while (r_code == DNS_SUCCESS) {

			/*
				malloc space for an attribute attribute type.
																*/
			if ((list_space = D27_MALLOC(svc_c_sev_warning,struct_size + D2_OBJ_IDL_MAX)) == NULL) {

				read_error->d2_retcode = D2_TEMP_ERR;
				read_error->d2_errclass = D2_SYS_ERR;
				read_error->d2_errvalue = D21_ALLOC_ERROR;
				return (D2_ERROR);
			}

			attr_list->d2_a_next = (D2_a_info *) list_space;
			attr_list = attr_list->d2_a_next;

			attr_list->d2_a_val = (D2_a_value *) NULL;
			attr_list->d2_a_next = (D2_a_info *) NULL;
			attr_list->d2_a_no_val = 0;

			list_space += struct_size;
			attr_list->d2_a_type.d2_type = (Ob_id_string) list_space;
			
			/*
				CDS may return either dotted string OIDS of short 
				name string OID types.
																	*/
			if (is_dotted_oid (type_str)) {

				strcpy ((char *)attr_list->d2_a_type.d2_type,(char *)type_str);
				attr_list->d2_a_type.d2_typ_len = strlen ((char *)type_str);
			
			} else {

				len = D2_OBJ_IDL_MAX;
				if (cdsCvtStrToObjID (type_str, (u_char *) NULL, 
					attr_list->d2_a_type.d2_type, &len, (dns_attr_name_t *)NULL,
					(int *)NULL,  (u_char **) NULL) == DNS_ERROR) {
					DUA_TRACE_CDS_FUNCTION_FAILED("cdsCvtStrToObjID")
					read_error->d2_retcode = D2_CALL_ERR;
					read_error->d2_errclass = D2_PAR_ERR;
					read_error->d2_errvalue = D2_NO_BIND;
					r_code = D2_ERROR;
				}
/* c.f previous call to cdsCvtStrToObjID */
                                attr_list->d2_a_type.d2_typ_len = len - 1;
			}

			len = D2_OBJ_IDL_MAX;
			r_code = cdsEnumAttr (dns_fname, handle, dns_ent_type, 
					&dns_attr_type, (void *)NULL, type_str, &len, flags);
		}

		if ((r_code == DNS_ERROR) || (r_code == DNS_NOROOM)) {

			d21_cds_map_error (flags->fsLocStat, read_error);
			r_code = D2_ERROR;
		}

		(void) cdsFreeHandle (handle);
	}

	return (r_code);
}


/*-----------------------------------------------------------------------

	encode_attr_types():

	Converts the attribute type OIDs into BER format.

 -----------------------------------------------------------------------*/

static int		encode_attr_types (	
					D2_a_info			*attr_list,
					D2_pbhead			*read_error)
{
	u_char		type_str[D2_OBJ_IDL_MAX];
	int			r_code,		error;
	const char function_name[] = "encode_attr_types";

	r_code = D2_NOERROR;
	error = FALSE;

	while (!error && (attr_list != (D2_a_info *) NULL)) {

		/*  
			convert the OID to IAPL HEX format    
												*/
		strcpy ((char *)type_str,(char *)attr_list->d2_a_type.d2_type);
		d21_cds_compress_oid (type_str);

		if (d27_302_str_oid ((char *)type_str, &attr_list->d2_a_type)
				== D2_ERROR) {

			DUA_TRACE_D27_302_FAILED
			read_error->d2_retcode = D2_CALL_ERR;
			read_error->d2_errclass = D2_PAR_ERR;
			read_error->d2_errvalue = D2_NOT_SPEC;
			r_code = D2_ERROR;
			error = TRUE;
		} 

		attr_list = attr_list->d2_a_next;
	}

	return (r_code);
}


/*-----------------------------------------------------------------------

	decode_attr_types():

	Converts the attribute type OIDs into uncompressed string format.

 -----------------------------------------------------------------------*/

static int		decode_attr_types (	
					D2_a_info		*attr_list,
					D2_pbhead		*read_error)
{
	u_char		type_str[D2_OBJ_IDL_MAX];
	int		r_code,	error;
	const char function_name[] = "decode_attr_types";

	r_code = D2_NOERROR;
	error = FALSE;

	while (!error && (attr_list != (D2_a_info *) NULL)) {

		/*  
			convert the OID to CDS string format    
												*/
		if (d27_301_oid_str (&attr_list->d2_a_type,(char *)type_str)
				== D2_ERROR) {

			read_error->d2_retcode = D2_CALL_ERR;
			read_error->d2_errclass = D2_PAR_ERR;
			read_error->d2_errvalue = D2_NOT_SPEC;
			DUA_TRACE_D27_301_FAILED
            		return (D2_ERROR);
        };

		d21_cds_uncompress_oid (type_str, attr_list->d2_a_type.d2_type);
		attr_list->d2_a_type.d2_typ_len = strlen ((char *)attr_list->d2_a_type.d2_type);

		attr_list = attr_list->d2_a_next;
	}

	return (r_code);
}


/*-----------------------------------------------------------------------

	read_attr_values():

	Given an object name and a list of attribute types, reads the
	attribute values for each of the attribute types.

 -----------------------------------------------------------------------*/

static int	read_attr_values (	
				D2_a_info			**p_attr_list,
				dns_full_name_t		*dns_fname,
				struct dnsFlagStat	*flags,
				D2_pbhead			*read_error)
{
	D2_a_info		*attr_list,					*prev_attr;
	u_char			type_str[D2_OBJ_IDL_MAX];
	int				r_code,	error,	count;
	const char function_name[] = "read_attr_values";

	r_code = D2_NOERROR;
	error = FALSE;

	/*
		read the attribute values for each attribute type in 
		the attribute list
															*/
	attr_list = *p_attr_list;
	prev_attr = attr_list;
	count = 0;

	while ((attr_list != (D2_a_info *) NULL) && !error) {

		/*	
			If the attribute type is a CDS member string convert 
			it to the equivalent OID.		
																	*/
		strcpy ((char *)type_str,(char *)attr_list->d2_a_type.d2_type);

		if (strcmp ((char *)type_str, DSW_CDS_MEMBER_OID) == 0) {

			if ((r_code = setval_member (attr_list, dns_fname,flags,read_error))				== D2_ERROR) {

				error = TRUE;
			} 
			
			strcpy ((char *)type_str, DSW_MEMBER_OID);

		} else {

			if ((r_code = setval_attr (attr_list, dns_fname, flags, read_error))				== D2_ERROR) {

				error = TRUE;

			}  else {

				d21_cds_compress_oid (type_str);
			}
		}

		if (!error) {
				
			/* 
				Don't return attributes with no values. Don't free the 
				attribute list if it is the first attribute list structure 
				in the read results.
																		*/
			if (attr_list->d2_a_no_val == 0) {

				if (count == 0) {

					if (attr_list->d2_a_next == (D2_a_info *) NULL) {

						*p_attr_list = (D2_a_info *) NULL;
						free ((char *) attr_list);
						attr_list = (D2_a_info *) NULL;

					} else {

						*p_attr_list = attr_list->d2_a_next;
						free ((char *) attr_list);
						attr_list = *p_attr_list;
					}

				} else {

					if (attr_list->d2_a_next == (D2_a_info *) NULL) {
						prev_attr->d2_a_next = (D2_a_info *) NULL;
					} else {
						prev_attr->d2_a_next = attr_list->d2_a_next;
					}

					free ((char *) attr_list);
					attr_list = prev_attr->d2_a_next;
				}

			} else {

				/*
					Object Class values will be returned from CDS with
					the syntax T61 string. Reset the syntax to object
					identifier syntax.
																		*/
				if (strcmp ((char *)type_str, DSW_CLASS_OID) == 0) {

					setsyntax_objectClass (attr_list);
				}

				/*	
					convert the attribute type OID to IAPL* internal format 	
																		*/
				if (d27_302_str_oid ((char *)type_str,
							&attr_list->d2_a_type) 
					== D2_ERROR) {

					DUA_TRACE_D27_302_FAILED
					read_error->d2_retcode = D2_CALL_ERR;
					read_error->d2_errclass = D2_PAR_ERR;
					read_error->d2_errvalue = D2_NOT_SPEC;
					r_code = D2_ERROR;
				}

				prev_attr = attr_list;
				attr_list = attr_list->d2_a_next;
				count ++;
			}
		}
	}

	return (r_code);
}



/*-----------------------------------------------------------------------


	match_attr_types ():

	Matches a list of attribute types against a list of attributes 
	and returns the list of matched attributes.

 -----------------------------------------------------------------------*/

static int		match_attr_types (	
					D2_a_info	**match_list,
					D2_req_info	*req_list,
					D2_pbhead	*read_error)
{
	D2_a_info	*attr_list,		*prev_attr;
	char 		* help;
	D2_a_type	*req_type;
	u_char		type_str[D2_OBJ_IDL_MAX], uncompressed_oid[D2_OBJ_IDL_MAX];
	int	found,	local_error, r_code, count, match_count;
	const char function_name[] = "match_attr_types";

	r_code = D2_NOERROR;

	match_count = 0;
	attr_list = *match_list;
	prev_attr = attr_list;

	while (attr_list != (D2_a_info *) NULL) {

		req_type = req_list->d2_r_type_at;
		found = FALSE;
		local_error = FALSE;
		count = 0;

		while (!found && !local_error && (count != req_list->d2_r_no_at)) {

			if (d27_301_oid_str (req_type,(char *)type_str)
				== D2_ERROR) {

				DUA_TRACE_D27_301_FAILED
				read_error->d2_retcode = D2_CALL_ERR;
				read_error->d2_errclass = D2_PAR_ERR;
				read_error->d2_errvalue = D2_NOT_SPEC;
				local_error = TRUE;
				r_code = D2_ERROR;

			} else {

				d21_cds_uncompress_oid (type_str, uncompressed_oid);

				if (strcmp ((char *)uncompressed_oid,(char *)attr_list->d2_a_type.d2_type) 
					== 0) {
					
					found = TRUE;

				} else {

					req_type ++;
				}
			}

			count ++;
		}

		if (!local_error) {

			if (!found) {

				if (match_count == 0) {

					*match_list = attr_list->d2_a_next;
					prev_attr = attr_list->d2_a_next;

				} else {

					prev_attr->d2_a_next = attr_list->d2_a_next;
				}

			} else {

				match_count ++;
				prev_attr = attr_list;
			}

			help = (char *) attr_list;
			attr_list = attr_list->d2_a_next;
			if (!found) {
				(void) free (help);}
		}
	}

	req_list->d2_r_no_at = match_count;

	return (r_code);
}


/*-----------------------------------------------------------------------


	copy_attr_types ():

	Copies a list of attribute types to a list of attributes.

 -----------------------------------------------------------------------*/

static int	copy_attr_types (	
				D2_a_info	**p_attr_list,
				D2_req_info	*req_list,
				D2_pbhead	*read_error)
{
	D2_a_info	*attr_list;
	D2_a_type	*req_type;
	char		*list_space;
	int	r_code,		struct_size,	oid_len,	count;

	r_code = D2_NOERROR;

	if ((req_list  == (D2_req_info *)0) || 
	    (req_list->d2_r_no_at == 0)     ||
	    (req_list->d2_r_type_at == (D2_a_type *) 0)) {

		*p_attr_list = NULL;
		return (D2_NOERROR);
	}

	req_type = req_list->d2_r_type_at;
	oid_len = req_type->d2_typ_len;
	struct_size = sizeof (D2_a_info);

#ifndef __hpux_CHFts15346
        /* the allocation excluded below has already been done by setup_for_read */
        /* no reason to abandon the setup_for_read allocation and leak its memory */
        attr_list = *p_attr_list;
#else
	/*
		Allocate space for the attribute type.
												*/
	if ((list_space = D27_MALLOC(svc_c_sev_warning,struct_size + D2_OBJ_IDL_MAX)) == NULL) {

		read_error->d2_retcode = D2_TEMP_ERR;
		read_error->d2_errclass = D2_SYS_ERR;
		read_error->d2_errvalue = D21_ALLOC_ERROR;
		return (D2_ERROR);
	}

	attr_list = (D2_a_info *) list_space;
	attr_list->d2_a_next = (D2_a_info *) NULL;
	attr_list->d2_a_val = (D2_a_value *) NULL;
	attr_list->d2_a_no_val = 0;
	*p_attr_list = attr_list;

	/*	
		Copy the requested attribute type to the attribute list.
																*/
	list_space += struct_size;
	attr_list->d2_a_type.d2_type = (Ob_id_string) list_space;
#endif

	memcpy (attr_list->d2_a_type.d2_type, req_type->d2_type, oid_len);
	attr_list->d2_a_type.d2_typ_len = oid_len;

	for (count = 1; count != req_list->d2_r_no_at; count ++) {

		/*
			Allocate space for the attribute type.	
													*/
		req_type ++;
		oid_len = req_type->d2_typ_len;

		if ((list_space = D27_MALLOC(svc_c_sev_warning,struct_size + D2_OBJ_IDL_MAX)) == NULL) {

			read_error->d2_retcode = D2_TEMP_ERR;
			read_error->d2_errclass = D2_SYS_ERR;
			read_error->d2_errvalue = D21_ALLOC_ERROR;
			return (D2_ERROR);
		}
	
		attr_list->d2_a_next = (D2_a_info *) list_space;
		attr_list = attr_list->d2_a_next;

		attr_list->d2_a_next = (D2_a_info *) NULL;
		attr_list->d2_a_val = (D2_a_value *) NULL;
		attr_list->d2_a_no_val = 0;

		/*	
			Copy the requested attribute type to the attribute list.
																	*/
		list_space += struct_size;
		attr_list->d2_a_type.d2_type = (Ob_id_string) list_space;

		memcpy (attr_list->d2_a_type.d2_type, req_type->d2_type, oid_len);
		attr_list->d2_a_type.d2_typ_len = oid_len;

	}

	return (r_code);
}



/*-----------------------------------------------------------------------

	map_alias():

	maps the resolved alias information onto the IAPL* attribute list.

 -----------------------------------------------------------------------*/

static int map_alias (	
			D2_a_info			*attr_list,
			dns_full_name_t		*dns_alias,
			int					eis_values,
			struct dnsFlagStat	*flags,
			D2_pbhead			*read_error)
{
	D2_a_info 	*list_head,	*prev_attr;
	D2_a_type	attr_type;
	static char	*aon_oid		= DSW_AON_OID;
	static char	*oc_oid			= DSW_CLASS_OID;
	static char	*alias_oc_val	= DSW_ALIAS_OID;
	D2_name_string	obj_name;
	char		*list_space;
	int			r_code,		struct_size;
	const char function_name[] = "map_alias";

	r_code = D2_NOERROR;

	/*	
		Convert the aliasedObjectName OID string into IAPL* internal format.
																		*/
	if (d27_302_str_oid (aon_oid, &attr_list->d2_a_type) == D2_ERROR) {

		DUA_TRACE_D27_302_FAILED
		read_error->d2_retcode = D2_CALL_ERR;
		read_error->d2_errclass = D2_PAR_ERR;
		read_error->d2_errvalue = D2_NOT_SPEC;
		r_code = D2_ERROR;

	} else {

		/*
			Malloc and assign the objectClass attribute type.
																*/
		struct_size = sizeof (D2_a_info);
		if ((list_space = D27_MALLOC(svc_c_sev_warning,struct_size + D2_OBJ_IDL_MAX)) == NULL) {

			read_error->d2_retcode = D2_TEMP_ERR;
			read_error->d2_errclass = D2_SYS_ERR;
			read_error->d2_errvalue = D21_ALLOC_ERROR;
			return (D2_ERROR);
		}
		
		attr_list->d2_a_next = (D2_a_info *) list_space;
		list_head = attr_list;
		prev_attr = attr_list;

		attr_list = attr_list->d2_a_next;
		attr_list->d2_a_next = (D2_a_info *) NULL;

		list_space += struct_size;
		attr_list->d2_a_type.d2_type = (Ob_id_string) list_space;

		/*	
			Convert the objectClass OID string into IAPL* internal format	
																		*/
		if (d27_302_str_oid (oc_oid, &attr_list->d2_a_type) == D2_ERROR) {

			DUA_TRACE_D27_302_FAILED
			read_error->d2_retcode = D2_CALL_ERR;
			read_error->d2_errclass = D2_PAR_ERR;
			read_error->d2_errvalue = D2_NOT_SPEC;
			r_code = D2_ERROR;

		} else if (eis_values) {

			attr_list = prev_attr;
			struct_size = sizeof (D2_a_value);

			/*
				Allocate space for the aliasedObjectName attribute value.
																		*/
			if ((list_space = D27_MALLOC(svc_c_sev_warning,struct_size + D2_DNL_MAX)) == NULL) {

				read_error->d2_retcode = D2_TEMP_ERR;
				read_error->d2_errclass = D2_SYS_ERR;
				read_error->d2_errvalue = D21_ALLOC_ERROR;
				return (D2_ERROR);
			}

			attr_list->d2_a_val = (D2_a_value *) list_space;
			list_space += struct_size;
			obj_name = (D2_name_string) list_space;

			if (d21_cdsname2x500name ((u_char *)dns_alias, obj_name)

				== D2_ERROR) {
				DUA_TRACE_CDS_TO_X500_NAME_FAILED
				read_error->d2_retcode = D2_CALL_ERR;
                read_error->d2_errclass = D2_PAR_ERR;
                read_error->d2_errvalue = D2_NOT_SPEC;
                return (D2_ERROR);
			}

			attr_list->d2_a_no_val = 1;
			attr_list->d2_a_val->d2_a_rep = D2_DISTNAME;
			attr_list->d2_a_val->d2_a_v_len = strlen ((char *)obj_name) + 1;
			attr_list->d2_a_val->d2_at_value.d2_obj_name_a_v = obj_name;
			attr_list = attr_list->d2_a_next;

			/*
				Allocate space for the objectClass attribute value.
																	*/
			if ((list_space = D27_MALLOC(svc_c_sev_warning,struct_size + D2_OBJ_IDL_MAX)) == NULL) {

				read_error->d2_retcode = D2_TEMP_ERR;
				read_error->d2_errclass = D2_SYS_ERR;
				read_error->d2_errvalue = D21_ALLOC_ERROR;
				return (D2_ERROR);
			}

			attr_list->d2_a_val = (D2_a_value *) list_space;

			list_space += struct_size;
			attr_type.d2_type = (Ob_id_string) list_space;

			/*	
				Convert the object class attribute value to IAPL* format.
																		*/
			if (d27_302_str_oid (alias_oc_val, &attr_type) == D2_ERROR) {

				DUA_TRACE_D27_302_FAILED
				read_error->d2_retcode = D2_CALL_ERR;
				read_error->d2_errclass = D2_PAR_ERR;
				read_error->d2_errvalue = D2_NOT_SPEC;
				r_code = D2_ERROR;

			}

			attr_list->d2_a_no_val = 1;
			attr_list->d2_a_val->d2_a_rep = D2_OBJ_IDENT;
			attr_list->d2_a_val->d2_at_value.d2_obid_a_v = attr_type.d2_type;
			attr_list->d2_a_val->d2_a_v_len = attr_type.d2_typ_len;
		}

		attr_list = list_head;
	}

    return (r_code);
}




/*-----------------------------------------------------------------------

	setval_attr():

	Sets the attribute values for a given attribute type.

 -----------------------------------------------------------------------*/

static int		setval_attr (	
					D2_a_info			*attr_list,
					dns_full_name_t		*dns_fname,
					struct dnsFlagStat	*flags,
					D2_pbhead			*read_error)
{
	enum dnsEntries	    dns_ent_type;
    enum dnsAttrType    dns_attr_type;
    dns_opq_fname       *partial_result;
	dns_handle		handle;
	dns_attr_name_t	*dns_type;
	dns_attrvalue_t	*dns_value;
	dns_attrvalue_t	dns_value_help;
	D2_a_value		*attr_value;
    u_char			type_str [D2_OBJ_IDL_MAX];
	char			*list_space;
	int			r_code,    len,    struct_size,    error;
	const char function_name[] = "setval_attr";
	signed32 number_allocated;

	r_code = D2_NOERROR;
	partial_result = flags->fsPartialRes;

	if ((handle = cdsGetHandle()) == (dns_handle) NULL) {
		DUA_TRACE_CDS_FUNCTION_FAILED("cdsGetHandle")
		read_error->d2_retcode = D2_CALL_ERR;
		read_error->d2_errclass = D2_PAR_ERR;
		read_error->d2_errvalue = D2_NO_BIND;
		r_code = D2_ERROR;

	} else {

		dns_type = (dns_attr_name_t *) attr_list->d2_a_type.d2_type;
		flags->fsNameType = dnsStrXDS;

		r_code = cdsReadAttrValue (dns_fname, handle, dns_type, dnsObj,
		    (void *)NULL, &dns_value, (dns_cts **)NULL,
			(void *)NULL, (int *)NULL, flags);
		
		/*	
			If the entry is not a CDS object, check to see if it is
			a CDS child pointer.
																	*/
		if (flags->fsLocStat == DNS_UNKNOWNENTRY) {

			(void) memset ((char *) flags, 0, sizeof (struct dnsFlagStat));
			flags->fsPartialRes = partial_result;
			flags->fsResLength = sizeof (dns_opq_fname);
			flags->fsMaybeMore = TRUE;
			flags->fsNameType = dnsStrXDS;

			if ((r_code = cdsReadAttrValue (dns_fname, handle, dns_type, 
				dnsDir, (void *)NULL, &dns_value, (dns_cts **)NULL,
				(void *)NULL, (int *)NULL, flags)) == DNS_ERROR) {
				DUA_TRACE_CDS_FUNCTION_FAILED("cdsReadAttrValue")
				d21_cds_map_error (flags->fsLocStat, read_error);
				r_code = D2_ERROR;
			}

			dns_ent_type = dnsDir;

		} else {

			dns_ent_type = dnsObj;
		}

		/*
			The function dnsEnumAttr() does not need to be called here 
            if dnsReadAttrValue() called above returned partial results 
			correctly.
																		*/
		(void) memset ((char *) flags, 0, sizeof (struct dnsFlagStat));
		flags->fsPartialRes = partial_result;
		flags->fsResLength = sizeof (dns_opq_fname);
		flags->fsMaybeMore = TRUE;
		flags->fsNameType = dnsStrXDS;
		len = sizeof (type_str);

		if (flags->fsLocStat == DNS_UNKNOWNENTRY) {
			cdsEnumAttr (dns_fname, handle, dnsDir, &dns_attr_type,
				(void *)NULL, type_str, &len, flags);
		}

		if (r_code == DNS_SUCCESS) {

			/*
				Allocate space for a set of attribute value structures.
																		*/
			struct_size = sizeof (D2_a_value);
			if ((list_space = D27_MALLOC(svc_c_sev_warning,DSW_AV_MAX * struct_size)) == NULL) {

				read_error->d2_retcode = D2_TEMP_ERR;
				read_error->d2_errclass = D2_SYS_ERR;
				read_error->d2_errvalue = D21_ALLOC_ERROR;
				return (D2_ERROR);
			}

			number_allocated = DSW_AV_MAX;
			attr_list->d2_a_val = (D2_a_value *) list_space;
			attr_value = attr_list->d2_a_val;
			attr_list->d2_a_no_val = 0;
			error = FALSE;

			while ((r_code == DNS_SUCCESS) && !error) {

				if (attr_list->d2_a_no_val >= number_allocated)
				  {
				  if ((list_space = D27_REALLOC(svc_c_sev_warning,list_space,(DSW_AV_MAX + number_allocated) * struct_size)) == NULL) {

					  read_error->d2_retcode = D2_TEMP_ERR;
					  read_error->d2_errclass = D2_SYS_ERR;
					  read_error->d2_errvalue = D21_ALLOC_ERROR;
					  return (D2_ERROR);
					  }

				  number_allocated += DSW_AV_MAX;
				  attr_list->d2_a_val = (D2_a_value *) list_space;
			 	  attr_value = attr_list->d2_a_val + attr_list->d2_a_no_val;
				  }

				dns_value_help = *dns_value;
				if (strcmp((char *)dns_type,DSW_CDS_CLASS_OID) == 0)
					{
					dns_value_help.val_u.val_byte.length = dns_value->val_u.val_byte.length - 2;
					dns_value_help.val_u.val_byte.byte_p = (void *)((char *)dns_value->val_u.val_byte.byte_p + 2);
					}
				if (d21_cdsval2x500val (attr_value, &dns_value_help) == D2_ERROR) {

					error = TRUE;
					r_code = D2_ERROR;

				} else {

					attr_list->d2_a_no_val ++;
					attr_value ++;

					/*
						read each attribute value for an attribute type 
																		*/
					if ((r_code = cdsReadAttrValue (dns_fname, handle, 
						dns_type, dns_ent_type, (dns_full_name_t *)NULL, 
						&dns_value, (dns_cts **)NULL, (u_char *)NULL, 
						(int *)NULL, flags)) == DNS_ERROR) {
						DUA_TRACE_CDS_FUNCTION_FAILED("cdsReadAttrValue")
						error = TRUE;
						d21_cds_map_error (flags->fsLocStat, read_error);
						r_code = D2_ERROR;
					}
				}
			}
		}

		if ((r_code == DNS_SUCCESS) || (r_code == DNS_EMPTY)) {

			r_code = D2_NOERROR;
		}

		(void) cdsFreeHandle (handle);
	}

	return (r_code);
}




/*-----------------------------------------------------------------------

	setval_member():

	Sets the attribute values for the member attribute.

 -----------------------------------------------------------------------*/

static int	setval_member (	
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_fname,
				struct dnsFlagStat	*flags,
				D2_pbhead			*read_error)
{
	dns_handle			handle;
	dns_attr_name_t		*dns_type;
	enum dnsGrpMbrType  mtype;
	D2_a_value			*attr_value;
	u_char				*gds_member,	iapl_member[D2_DNL_MAX], 
						cds_member[D2_DNL_MAX];
	char				*list_space;
	int		struct_size,	len,	r_code,		error;
	signed32 number_allocated;
	const char function_name[] = "setval_member";

	r_code = D2_NOERROR;

	if ((handle = cdsGetHandle()) == (dns_handle) NULL) {
		DUA_TRACE_CDS_FUNCTION_FAILED("cdsGetHandle")
		read_error->d2_retcode = D2_CALL_ERR;
		read_error->d2_errclass = D2_PAR_ERR;
		read_error->d2_errvalue = D2_NO_BIND;
		r_code = D2_ERROR;

	} else {

		len = D2_DNL_MAX;
		dns_type = (dns_attr_name_t *) attr_list->d2_a_type.d2_type;
		flags->fsNameType = dnsStrXDS;

		if ((r_code = cdsReadMbr (dns_fname, handle, (dns_full_name_t **)NULL, 
				&mtype, cds_member, &len, flags)) == DNS_SUCCESS) {

			d21_cds_expand_oids (cds_member, iapl_member);

			/*
				Alloctate space for a set of attribute value structures
																	*/
			struct_size = sizeof (D2_a_value);
			if ((list_space = D27_MALLOC(svc_c_sev_warning,DSW_AV_MAX * struct_size)) == NULL) {

				read_error->d2_retcode = D2_TEMP_ERR;
				read_error->d2_errclass = D2_SYS_ERR;
				read_error->d2_errvalue = D21_ALLOC_ERROR;
				return (D2_ERROR);
			}

			number_allocated = DSW_AV_MAX;
			attr_list->d2_a_val = (D2_a_value *) list_space;
			attr_value = attr_list->d2_a_val;
			attr_list->d2_a_no_val = 0;
			error = FALSE;

			while ((r_code == DNS_SUCCESS) && !error) {

				if (attr_list->d2_a_no_val >= number_allocated)
					{
					if ((list_space = D27_REALLOC(svc_c_sev_warning,list_space,(DSW_AV_MAX
 + number_allocated) * struct_size)) == NULL) {

						read_error->d2_retcode = D2_TEMP_ERR;
						read_error->d2_errclass = D2_SYS_ERR;
						read_error->d2_errvalue = D21_ALLOC_ERROR;
						return (D2_ERROR);
						}

					number_allocated += DSW_AV_MAX;
					attr_list->d2_a_val = (D2_a_value *) list_space;
					attr_value = attr_list->d2_a_val + attr_list->d2_a_no_val;
					}

				/*
					assign the CDS attribute value to the GDS 
					attribute value structure 								
																*/
				if ((gds_member = D27_MALLOC(svc_c_sev_warning,D2_DNL_MAX)) 
					== NULL) {

					read_error->d2_retcode = D2_TEMP_ERR;
					read_error->d2_errclass = D2_SYS_ERR;
					read_error->d2_errvalue = D21_ALLOC_ERROR;
					return (D2_ERROR);
				}

				if (d21_cdsname2x500name (iapl_member, gds_member) 
					== D2_ERROR) {
					DUA_TRACE_CDS_TO_X500_NAME_FAILED
					read_error->d2_retcode = D2_CALL_ERR;
					read_error->d2_errclass = D2_PAR_ERR;
					read_error->d2_errvalue = D2_NOT_SPEC;
					return (D2_ERROR);
				}

				attr_value->d2_at_value.d2_obj_name_a_v = gds_member;
				attr_value->d2_a_v_len = strlen((char *)gds_member)+1;
				attr_value->d2_a_rep = D2_DISTNAME;
				
				attr_list->d2_a_no_val ++;
				attr_value ++;
				len = D2_DNL_MAX;
				flags->fsNameType = dnsStrXDS;

				if ((r_code = cdsReadMbr (dns_fname, handle,
					(dns_full_name_t **)NULL, &mtype, cds_member, &len, flags))
					== DNS_SUCCESS) {

					d21_cds_expand_oids (cds_member, iapl_member);
				}
			}
		}

		if ((r_code == DNS_EMPTY) || (r_code == DNS_SUCCESS)) {
		
			r_code = D2_NOERROR;

		} else if (r_code == DNS_ERROR) {

			d21_cds_map_error (flags->fsLocStat, read_error);
			r_code = D2_ERROR;
			error = TRUE;
		}

		(void) cdsFreeHandle (handle);
	}

	return (r_code);
}




/*-----------------------------------------------------------------------

	setsyntax_objectClass():

	Moves through a list of attribute values changing the attribute
	value syntax to objectIdentifier.

 -----------------------------------------------------------------------*/

static void		setsyntax_objectClass (	
					D2_a_info			*attr_list)
{
	D2_a_value	*attr_value;
	u_char		*t61_string;
	int		count;

	count = 0;
	attr_value = attr_list->d2_a_val;

	while (count != attr_list->d2_a_no_val) {

		/*
			In general, attribute values received from CDS are 
			assigned the syntax T61 string.
																*/
		attr_value->d2_a_rep = D2_OBJ_IDENT;
		t61_string = attr_value->d2_at_value.d2_t61_a_v;
		attr_value->d2_at_value.d2_obid_a_v  = t61_string;

		count ++;
		attr_value ++;
	}
	
}



/*-----------------------------------------------------------------------

	is_dotted_oid ():

	Checks to see if the string is an OID string of the form X.Y.Z
    where X,Y and Z are numbers. For example, 2.5.4.3 .

 -----------------------------------------------------------------------*/

static int	is_dotted_oid (	
					u_char	*str)
{
	int		r_code,	INVALID_CHAR;

	INVALID_CHAR = FALSE;
	r_code = TRUE;

	if (str == (u_char *) NULL) {

		r_code = FALSE; 

	} else {

		while (*str != '\0' && !INVALID_CHAR) {

			if ((*str != '.') && (*str < '0' || *str > '9')) {

				INVALID_CHAR = TRUE;
				r_code = FALSE;

			} else {

				str ++;
			}
		}
	}

	return (r_code);
}


/*-----------------------------------------------------------------------

	free_read_result():

	Frees the read result linked list.

 -----------------------------------------------------------------------*/

static void		free_read_result (	
					D2_ei_info	*read_result)
{
	if (read_result != (D2_ei_info *) NULL) {

		if (read_result->d2_ei_ai != (D2_a_info *) NULL) {

			free_attr_list (read_result->d2_ei_ai);
		}
		
		free ((char *) read_result);
	}
		
}



/*-----------------------------------------------------------------------

	free_attr_list():

	Frees the attribute list linked list.

 -----------------------------------------------------------------------*/

static void		free_attr_list (	
					D2_a_info	*attr_list)
{
	if (attr_list != (D2_a_info *) NULL) {

		if (attr_list->d2_a_val != (D2_a_value *) NULL) {

			free_attr_values (attr_list);
		}
		
		free_attr_list (attr_list->d2_a_next);
		free ((char*) attr_list);
	}
		
}

/*-----------------------------------------------------------------------

	free_attr_values():

	Frees the attribute values space allocated for a given attribute.

 -----------------------------------------------------------------------*/

static void		free_attr_values (	
					D2_a_info	*attr_list)
{
	D2_a_value	*attr_value;
	int			count;

	if (attr_list->d2_a_val != (D2_a_value *) NULL) {

		attr_value = attr_list->d2_a_val;

		for (count = 0; count != attr_list->d2_a_no_val; count ++) {

			switch (attr_value->d2_a_rep) {
				
				case D2_PRINTABLE:

					free ((char *) attr_value->d2_at_value.d2_prt_a_v);
					break;

				case D2_T61:

					free ((char *) attr_value->d2_at_value.d2_t61_a_v);
					break;

				case D2_OCTET:

					free ((char *) attr_value->d2_at_value.d2_oct_a_v);
					break;

				case D2_DISTNAME:

					free ((char *) attr_value->d2_at_value.d2_obj_name_a_v);
					break;

				case D2_OBJ_IDENT:

					free ((char *) attr_value->d2_at_value.d2_obid_a_v);
					break;

				case D2_INTEGER:
					break;

				default:

					break;
			}

			attr_value ++;
		}

		free ((char *) attr_list->d2_a_val);
	}
		
}


/*-----------------------------------------------------------------------

	size_read_result():

	Determines the space used by the read result.

 -----------------------------------------------------------------------*/

static int		size_read_result (	
					D2_ei_info	*read_result)
{
	D2_a_info	*attr_list;
	int			result_size,		struct_size,	size;

	result_size = 0;

	if (read_result != (D2_ei_info *) NULL) {

		struct_size = sizeof (D2_ei_info);
		size = D2_ALIGN(strlen ((char *)read_result->d2_ei_entry) + 1);

		result_size += struct_size + size;
		attr_list = read_result->d2_ei_ai;
	
		while (attr_list != (D2_a_info *) NULL) {
		
		    result_size += D2_ALIGN(attr_list->d2_a_type.d2_typ_len+1)
				      + sizeof(D2_a_info);

			if (attr_list->d2_a_val != (D2_a_value *) NULL) {

				size = size_attr_values (attr_list);
				result_size += size;
			}

			attr_list = attr_list->d2_a_next;
		}
	}

	return (result_size);
}

/*-----------------------------------------------------------------------

	size_attr_values():

	Determines the space used by an attribute value.

 -----------------------------------------------------------------------*/

static int		size_attr_values (	
					D2_a_info	*attr_list)
{
	D2_a_value	*attr_value;
	int			size, 	struct_size,	count;

	size = 0;

	if (attr_list->d2_a_val != (D2_a_value *) NULL) {

		attr_value = attr_list->d2_a_val;

		for (count = 0; count != attr_list->d2_a_no_val; count ++) {

			switch (attr_value->d2_a_rep) {
				
				case D2_OCTET:

					size += D2_ALIGN(attr_value->d2_a_v_len + 1);
					break;

				case D2_DISTNAME:

					size += D2_ALIGN(strlen ((char *)attr_value->d2_at_value.d2_obj_name_a_v)+1);
					size ++;
					break;

				case D2_OBJ_IDENT:

					size += D2_ALIGN(attr_value->d2_a_v_len + 1);
					break;

				case D2_PRINTABLE:

					size += D2_ALIGN(attr_value->d2_a_v_len + 1);
					break;

				case D2_T61:

					size += D2_ALIGN(attr_value->d2_a_v_len + 1);
					break;

				case D2_INTEGER:

					break;

				default:

					break;
			}

			attr_value ++;
		}

		struct_size = sizeof (D2_a_value);
		size += struct_size * attr_list->d2_a_no_val;
	}
		
	return (size);
}



/*-----------------------------------------------------------------------

	make_result_contiguous():

	Copies the read result to one piece of contiguous space.

 -----------------------------------------------------------------------*/

static int		make_result_contiguous (	
					D2_ei_info	**p_read_result,
					D2_pbhead	*read_error)
{
	D2_ei_info	*read_result , *contig_result;
	D2_a_info	*attr_list , *contig_list;
	char		*ei_space;
	char		*tail_address;
	int		result_size , ei_struct_size , list_struct_size,
			size , r_code;

	r_code = D2_NOERROR;

	read_result = *p_read_result;

	if (read_result != (D2_ei_info *) NULL) {

		ei_struct_size = sizeof (D2_ei_info);
		list_struct_size = sizeof (D2_a_info);
		result_size = size_read_result (read_result);
		
		if ((ei_space = D27_MALLOC(svc_c_sev_warning,result_size)) == NULL) {
		
            read_error->d2_retcode = D2_TEMP_ERR;
            read_error->d2_errclass = D2_SYS_ERR;
            read_error->d2_errvalue = D21_ALLOC_ERROR;
            return (D2_ERROR);
		}

		tail_address = ei_space + result_size;

		contig_result = (D2_ei_info *) ei_space;
		contig_result->d2_ei_next = (D2_ei_info *) NULL;
		contig_result->d2_ei_fromentry = read_result->d2_ei_fromentry;
		*p_read_result = contig_result;
		
		ei_space += ei_struct_size;
		tail_address -= (strlen ((char *)read_result->d2_ei_entry)+1);
		contig_result->d2_ei_entry = (D2_name_string) tail_address;
		strcpy ((char *)contig_result->d2_ei_entry,
			(char *)read_result->d2_ei_entry);

		contig_list = (D2_a_info *) ei_space;
		contig_result->d2_ei_ai = contig_list;
		contig_list->d2_a_next = (D2_a_info *) NULL;
		contig_list->d2_a_no_val = 0;
		contig_list->d2_a_val = (D2_a_value *) NULL;
		
		attr_list = read_result->d2_ei_ai;

		while (attr_list != (D2_a_info *) NULL) {

			tail_address -= attr_list->d2_a_type.d2_typ_len;

			contig_list->d2_a_type.d2_type =
						(Ob_id_string) tail_address;
			memcpy (contig_list->d2_a_type.d2_type,
				attr_list->d2_a_type.d2_type,
				attr_list->d2_a_type.d2_typ_len);

			contig_list->d2_a_type.d2_typ_len =
						attr_list->d2_a_type.d2_typ_len;
			contig_list->d2_a_next = (D2_a_info *) NULL;
			contig_list->d2_a_no_val = attr_list->d2_a_no_val;
			contig_list->d2_a_val = (D2_a_value *) NULL;

			size = make_values_contiguous (attr_list, contig_list,
							&tail_address);

			attr_list = attr_list->d2_a_next;

			if (attr_list != (D2_a_info *) NULL) {

				ei_space += list_struct_size + size;

				contig_list->d2_a_next = (D2_a_info *) ei_space;
				contig_list = contig_list->d2_a_next;

				contig_list->d2_a_next = (D2_a_info *) NULL;
				contig_list->d2_a_no_val = 0;

#ifdef NOTDEFINED
				ei_space += list_struct_size;
				contig_list->d2_a_type.d2_type = (Ob_id_string) ei_space;
#endif /* NOTDEFINED */
			}
		}

		free_read_result (read_result);
	}

	return (r_code);
} /* end make_result_contiguous */




/*-----------------------------------------------------------------------

	make_values_contiguous():

	Copies the attribute values one piece of contiguous space.

 -----------------------------------------------------------------------*/

static int		make_values_contiguous (	
					D2_a_info	*attr_list,
					D2_a_info	*contig_list,
					char		**tail_address)
{
	D2_a_value	*attr_value,	*contig_value;
	char		*ei_space;
	int		values_size, count;

	values_size = 0;

	if (attr_list->d2_a_val != (D2_a_value *) NULL) {

		if (attr_list->d2_a_no_val != 0) {
		
			ei_space = (char *) contig_list;
			ei_space += sizeof (D2_a_info);

			contig_list->d2_a_val = (D2_a_value *) ei_space;
			contig_value = contig_list->d2_a_val;

			attr_value = attr_list->d2_a_val;
			values_size = sizeof(D2_a_value)*attr_list->d2_a_no_val;
		}

		for (count = 0; count != attr_list->d2_a_no_val; count ++) {

			contig_value->d2_a_v_len =  attr_value->d2_a_v_len;
			contig_value->d2_a_rep =  attr_value->d2_a_rep;
			
			switch (attr_value->d2_a_rep) {

			  case D2_OCTET:

				*tail_address -= attr_value->d2_a_v_len;
				contig_value->d2_at_value.d2_oct_a_v =
						(Octet_string) *tail_address;
				memcpy (contig_value->d2_at_value.d2_oct_a_v,
					attr_value->d2_at_value.d2_oct_a_v,
					attr_value->d2_a_v_len);

				break;

			  case D2_PRINTABLE:

				*tail_address -= attr_value->d2_a_v_len;
				contig_value->d2_at_value.d2_prt_a_v =
						(Print_string) *tail_address;
				memcpy (contig_value->d2_at_value.d2_prt_a_v,
					attr_value->d2_at_value.d2_prt_a_v,
					attr_value->d2_a_v_len);

				break;

			  case D2_T61:

				*tail_address -= attr_value->d2_a_v_len;
				contig_value->d2_at_value.d2_t61_a_v =
						(T61_string) *tail_address;
				memcpy (contig_value->d2_at_value.d2_t61_a_v,
					attr_value->d2_at_value.d2_t61_a_v,
					attr_value->d2_a_v_len);

				break;

			  case D2_DISTNAME:

				*tail_address -= attr_value->d2_a_v_len;
				contig_value->d2_at_value.d2_obj_name_a_v =
						(u_char *) *tail_address;
			       memcpy(contig_value->d2_at_value.d2_obj_name_a_v,
					attr_value->d2_at_value.d2_obj_name_a_v,
					attr_value->d2_a_v_len);

				break;

			  case D2_OBJ_IDENT:

				*tail_address -= attr_value->d2_a_v_len;
				contig_value->d2_at_value.d2_obid_a_v =
						(Ob_id_string) *tail_address;
				memcpy (contig_value->d2_at_value.d2_obid_a_v,
					attr_value->d2_at_value.d2_obid_a_v,
					attr_value->d2_a_v_len);

#ifdef NOTDEFINED
				contig_value->d2_a_v_len = attr_value->d2_a_v_len;
				ei_space += contig_value->d2_a_v_len;
				values_size += contig_value->d2_a_v_len;
#endif /* NOTDEFINED */
				break;

			  case D2_INTEGER:
				break;

			  default:
				break;
			}
			
			attr_value ++;
			contig_value ++;
		}
	}

	return (values_size);
}
