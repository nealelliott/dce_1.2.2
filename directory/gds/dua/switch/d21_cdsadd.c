/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_cdsadd.c,v $
 * Revision 1.1.11.2  1996/02/18  18:17:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:56  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:55:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:57  root]
 * 
 * Revision 1.1.9.3  1994/08/17  13:56:44  keutel
 * 	OT 11222: adding CDS class and CDS class version via XDS
 * 	[1994/08/17  12:49:35  keutel]
 * 
 * Revision 1.1.9.2  1994/05/10  15:54:19  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:33:43  marrek]
 * 
 * Revision 1.1.9.1  1994/02/22  18:45:25  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:37:41  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  06:46:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:01:55  marrek]
 * 
 * Revision 1.1.5.4  1992/12/31  19:49:13  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:42  bbelch]
 * 
 * Revision 1.1.5.3  1992/11/16  08:57:51  marrek
 * 	Delete duplicate history line, eremove empty #ifndef lint and #ifdef ANSIC,
 * 	add TRACE_ERROR for d21_fulnk("cdsAddMbr").
 * 	remove/add some tabs.
 * 	[1992/11/06  11:33:24  marrek]
 * 
 * Revision 1.1.5.2  1992/09/29  21:10:23  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:23:57  treff]
 * 
 * Revision 1.1.2.3  1992/06/30  20:59:02  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:39:34  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:37:53  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:38:58  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_cdsadd.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 18:17:56 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/*---------------------------------------------------------------------

	Filename:		d21_cdsadd.c 

	Author:			P. Dunne
	Date:			1.02.1991

	Component:		OSF DCE Naming Switch

	Functions:	
		exported:	d21_7_switch()			

		static: 	add_obj_attrs(), 		add_grp_attrs(),		
					getval_objectClass(),	getval_aliasedObjectName(),
					addval_member().	

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


/*------------------- local static function prototypes ----------------*/

static int	add_obj_attrs (	
				D2_a_info			*attr_list,	
				dns_full_name_t		*dns_name,
				struct dnsFlagStat	*flags,
				D2_pbhead			*add_error,
				D2_error			**add_error_info);


static int	add_grp_attrs (	
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_name,
				dnsFlagStat_t		*flags,
				D2_pbhead			*add_error,
				D2_error			**add_error_info);

static int	addval_member (	
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_name,
				struct dnsFlagStat	*flags,
				D2_pbhead			*add_error);

static int getval_class_and_version (
				dns_simple_name_t ** dns_class,
				dnsVersion_t	* dns_version,
				boolean		* dns_version_found,
				D2_a_info	*attr_list,
				D2_pbhead	*add_error);

static int	getval_objectClass (	
				u_char		*obj_class, 
				D2_a_info	*attr_list,
				D2_pbhead	*add_error);


static int	getval_aliasedObjectName (	
				u_char		**alias,	
				D2_a_info	*attr_list,
				D2_pbhead	*add_error);

/*--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------

	d21_7_switch():

	Adds an object to the local DNS cell. The add operation is 
	received at the IAPL* interface and has to be mapped to 
	DNSPI interface calls. The result from the DNSPI add operation
	is then mapped to an IAPL* add operation result.

 -----------------------------------------------------------------------*/

int		d21_7_switch (D21_p7	*add_par_blk)
{

	dns_full_name_t		*dns_name,	*dns_alias;
	dns_opq_fname		part_result;
	dns_simple_name_t	* dns_class;
	dnsVersion_t		dns_version;
	boolean dns_version_found;
	struct dnsFlagStat	dns_flags;
	D2_a_info			*attr_list;
	D2_pbhead			*add_error;
	u_char              cds_name[D2_DNL_MAX],	cds_alias[D2_DNL_MAX],	
						obj_class [D2_OBJ_IDL_MAX];
	u_char				*alias;
	int					r_code;
	const char function_name[] = "d21_7_switch";

	DUA_TRACE_IAPL_ENTRY

	r_code = D2_NOERROR;
	alias = (u_char *) NULL;
	/*
		IAPL* handles the malloc for the D2_pbhead structure.
																	*/
	add_error = add_par_blk->d21_7pbhead;

	if (d21_x500name2cdsname (add_par_blk->d21_7entry_name, cds_name) 
		== D2_ERROR) {

		DUA_TRACE_X500_TO_CDS_NAME_FAILED
		add_error->d2_retcode = D2_CALL_ERR;
		add_error->d2_errclass = D2_PAR_ERR;
		add_error->d2_errvalue = D2_NOT_SPEC;
		return (D2_ERROR);
	}
	
	dns_name = (dns_full_name_t *) cds_name;

	/*
		IAPL* common arguments are mapped to DNS flags structure.
																	*/
	(void) d21_cds_set_flags (&dns_flags, add_par_blk->d21_7c_a);
	dns_flags.fsPartialRes = &part_result;
	dns_flags.fsResLength = sizeof (part_result);

	/*
		Retrieve the object class of the object to be created.
																*/
	attr_list = add_par_blk->d21_7_at_info;
	if ((r_code = getval_class_and_version (&dns_class, &dns_version,
		&dns_version_found, attr_list, add_error)) == D2_ERROR)
	  {
	  return (D2_ERROR);
	  }
	if ((r_code = getval_objectClass (obj_class, attr_list, add_error))
	    != D2_ERROR) {

		/*
			In DNS, different functions are used to create objects 
			groups and aliases.
		
			Check to see if the object to be created is 
				*	an alias
				*	a group
		    	*	not a group or an alias .
																*/
		if (strcmp ((char *) obj_class, DSW_ALIAS_OID) == (int) NULL) {
			/*
				Retrieve the value of the aliasedObjectName attribute
																	*/
			if (getval_aliasedObjectName (&alias,attr_list,add_error) 
			    	== D2_ERROR) {
				r_code = D2_ERROR;
			} else {
				/*
					create an alias object
												*/
				if (d21_x500name2cdsname (alias, cds_alias) == D2_ERROR) {
					DUA_TRACE_X500_TO_CDS_NAME_FAILED
					add_error->d2_retcode = D2_CALL_ERR;
					add_error->d2_errclass = D2_PAR_ERR;
					add_error->d2_errvalue = D2_NOT_SPEC;
					return (D2_ERROR);
				}

				dns_alias = (dns_full_name_t *) cds_alias;

				if (cdsCreateSLink(dns_name, dns_alias, NULL_DNS_TIMEOUT,
					NULL_DNS_CTS, &dns_flags) == DNS_ERROR) {
					DUA_TRACE_CDS_FUNCTION_FAILED("cdsCreateSLink")
					d21_cds_map_error (dns_flags.fsLocStat,
add_error);
					r_code = D2_ERROR;
				}
			}

		} else if (strcmp ((char *) obj_class, DSW_GROUP_OID) == (int) NULL) {

			/*
				create a groupOfNames object	
											*/
			if (cdsCreateGrp (dns_name, &dns_flags) == DNS_ERROR) {

				DUA_TRACE_CDS_FUNCTION_FAILED("cdsCreateGrp")
				d21_cds_map_error (dns_flags.fsLocStat, add_error);
				r_code = D2_ERROR;

			} else {

				/*
					add each member in the members attribute to the 
					groupOfNames object.				
																*/
				if ((r_code = add_grp_attrs (attr_list, dns_name, &dns_flags, 
				    add_error, &add_par_blk->d21_7err)) == D2_ERROR) {

					/*
						Since the add naming operation is autonomous remove 
						the groupOfNames object if there is an error adding
						members to the group.
																		*/
					(void) cdsDelGrp (dns_name, &dns_flags);
				}
			}
		} else {

			/*
				create an object.
										*/
			dns_flags.fsConf = dnsConfHi;
			/*	
				CDS won't accept an object class that contains dots in it.
																		*/
			d21_cds_dots2hypens (obj_class);

			if (obj_class[0] == '\0') {
				strcpy ((char *) obj_class, "object-class");
			}

			if (cdsCreateObj (dns_name, dns_class,
			dns_version_found == TRUE? &dns_version:(void *) NULL, 
			(void *) NULL, (void *) NULL, &dns_flags) == DNS_ERROR) {

				DUA_TRACE_CDS_FUNCTION_FAILED("cdsCreateObj")
				d21_cds_map_error (dns_flags.fsLocStat, add_error);
				r_code = D2_ERROR;

			} else {
				/*
					add the attributes to the created object
																*/
				if ((r_code = add_obj_attrs (attr_list, dns_name, &dns_flags, 
				    add_error, &add_par_blk->d21_7err)) == D2_ERROR) {
					/*
						Since the add naming operation is autonomous remove 
						the groupOfNames object if there is an error adding
						members to the group.
																		*/
					(void) cdsDelObj (dns_name, &dns_flags);
				}
			}
		}
	}

	if (r_code == D2_ERROR) {

		d21_cds_error_info (add_error, dns_flags.fsPartialRes, 
											&add_par_blk->d21_7err);
	}

	DUA_TRACE_IAPL_EXIT(add_par_blk->d21_7pbhead->d2_errvalue)
	return (r_code);
}



/*-----------------------------------------------------------------------

	getval_objectClass ():

	Retrieves the value of the attribute objectClass from a list of 
	attributes. The objectClass attribute is then removed from the 
	attribute list.

 -----------------------------------------------------------------------*/

static int	getval_objectClass (	
				u_char		*obj_class, 
				D2_a_info	*attr_list,
				D2_pbhead	*add_error)
{
	D2_obj_id	class_oid;
	u_char		type_str[D2_DNL_MAX];
	int			found,	r_code;
	const char function_name[] = "getval_objectClass";

	r_code = D2_NOERROR;
	found = FALSE;

	while ((!found) && (attr_list != (D2_a_info *) NULL)) {

		if (d27_301_oid_str (&attr_list->d2_a_type, (char *)type_str) == D2_ERROR) {

			add_error->d2_retcode = D2_CALL_ERR;
			add_error->d2_errclass = D2_PAR_ERR;
			add_error->d2_errvalue = D2_NO_BIND;
			DUA_TRACE_D27_301_FAILED
			return (D2_ERROR);
		};
		
		if (strcmp ((char *) type_str, DSW_CLASS_OID) == (int) NULL) {
			found = TRUE;
		} else {
			attr_list = attr_list->d2_a_next;
		}
	}

	if (!found) {

		*obj_class = '\0';

	} else {

		if (attr_list->d2_a_val->d2_a_rep != D2_OBJ_IDENT) {

			/*
				assign the error code, class and value
														*/
			add_error->d2_retcode = D2_CALL_ERR;
			add_error->d2_errclass = D2_PAR_ERR;
			add_error->d2_errvalue = D2_NO_BIND;
			r_code = D2_ERROR;

		} else {

			class_oid.d2_typ_len = attr_list->d2_a_val->d2_a_v_len;
			class_oid.d2_type = attr_list->d2_a_val->d2_at_value.d2_obid_a_v;
			if (d27_301_oid_str (&class_oid, (char *)obj_class) == D2_ERROR) {

				DUA_TRACE_D27_301_FAILED
				add_error->d2_retcode = D2_CALL_ERR;
				add_error->d2_errclass = D2_PAR_ERR;
				add_error->d2_errvalue = D2_NO_BIND;
				r_code = D2_ERROR;
			} 

		}

	}

	return (r_code);
}



/*-----------------------------------------------------------------------

	getval_aliasedObjectName ():

	Retrieves the value of the aliasedObjectName attribute from a 
	from a list of attributes.

 -----------------------------------------------------------------------*/

static int	getval_aliasedObjectName (	
				u_char		**alias,	
				D2_a_info	*attr_list,
				D2_pbhead	*add_error)
{
	u_char	type_str[D2_DNL_MAX];
	int		found,	r_code;
	const char function_name[] = "getval_aliasedObjectName";

	found = FALSE;
	r_code = D2_NOERROR;

	while ((!found) && (attr_list != (D2_a_info *) NULL)) {

		if (d27_301_oid_str (&attr_list->d2_a_type, (char *)type_str) == D2_ERROR) {

			DUA_TRACE_D27_301_FAILED
			add_error->d2_retcode = D2_CALL_ERR;
			add_error->d2_errclass = D2_PAR_ERR;
			add_error->d2_errvalue = D2_NO_BIND;
			return (D2_ERROR);
		};

		if ((strcmp ((char *) type_str, DSW_AON_OID)) == (int) NULL)
			found = TRUE;
		else
			attr_list = attr_list->d2_a_next;
	}

	if (!found) {
		/*
				assign the error code, class and value
														*/
		add_error->d2_retcode = D2_CALL_ERR;
		add_error->d2_errclass = D2_PAR_ERR;
		add_error->d2_errvalue = D2_NO_BIND;
		r_code = D2_ERROR;
	} else {
		if (attr_list->d2_a_val->d2_a_rep != D2_DISTNAME) {
			/*
				assign the error code, class and value
														*/
			add_error->d2_retcode = D2_CALL_ERR;
			add_error->d2_errclass = D2_PAR_ERR;
			add_error->d2_errvalue = D2_NO_BIND;
			r_code = D2_ERROR;
		} else {

			*alias = attr_list->d2_a_val->d2_at_value.d2_obj_name_a_v;
		}
	}

	return (r_code);
}


/*-----------------------------------------------------------------------

	add_obj_attrs ():

	Given an object name an a list of attributes adds each attribute
	in the attribute list to the object.

 -----------------------------------------------------------------------*/

static int	add_obj_attrs (	
				D2_a_info			*attr_list,	
				dns_full_name_t		*dns_name,
				struct dnsFlagStat	*flags,
				D2_pbhead			*add_error,
				D2_error			**add_error_info)
{
	dns_attr_name_t	*dns_type;
	dns_attrvalue_t	dns_value;
	D2_a_value		*val_struct;
    u_char      	uncompressed_oid [D2_OBJ_IDL_MAX],
					type_str[D2_OBJ_IDL_MAX];
	int				r_code,		count;
	const char function_name[] = "add_obj_attrs";

	r_code = D2_NOERROR;

	while (attr_list != (D2_a_info *) NULL) {

		if (d27_301_oid_str (&attr_list->d2_a_type, (char *)type_str) == D2_ERROR) {

	    DUA_TRACE_D27_301_FAILED
			add_error->d2_retcode = D2_CALL_ERR;
			add_error->d2_errclass = D2_PAR_ERR;
			add_error->d2_errvalue = D2_NO_BIND;
            return (D2_ERROR);
        };

		d21_cds_uncompress_oid (type_str, uncompressed_oid);
		dns_type = (dns_attr_name_t *) uncompressed_oid;
		val_struct = attr_list->d2_a_val;

		if (strcmp((char *)uncompressed_oid,DSW_CDS_CLASS_OID) != 0 &&
		    strcmp((char *)uncompressed_oid,DSW_CDS_CLASS_VERSION_OID) != 0)
		{

		for (count = 0; count != attr_list->d2_a_no_val; count ++) {

			if (d21_x500val2cdsval (val_struct, &dns_value) == D2_ERROR) {
				
				add_error->d2_retcode = D2_CALL_ERR;
				add_error->d2_errclass = D2_PAR_ERR;
				add_error->d2_errvalue = D2_CONSTRAINT_VIOLATION;
				d21_cds_attr_error (dns_name, &attr_list->d2_a_type,
					add_error_info);
				return (D2_ERROR);

			} else {

				/*
					add the attribute value to the DNS object
																*/
				if (cdsAddAttrValue (dns_name, dns_type, dnsSet, dnsObj, 
					&dns_value, NULL_DNS_CTS, flags) == DNS_ERROR) {
					DUA_TRACE_CDS_FUNCTION_FAILED("cdsAddAttrValue")
					d21_cds_map_error (flags->fsLocStat, add_error);

					if (flags->fsLocStat == DNS_INVALIDUPDATE) {
						
						d21_cds_attr_error (dns_name, &attr_list->d2_a_type,
							add_error_info);
					} 

					return (D2_ERROR);
				}
			}

			val_struct ++; 
		}
		} /* end of: if not CDS_Class and CDS_Class_Version */

		attr_list = attr_list->d2_a_next;
	}

	return (r_code);
}



/*-----------------------------------------------------------------------

	add_grp_attrs ():

	Given an groupOfNames object and a list of attributes, adds the 
	member attribute to groupOfnames object.

 -----------------------------------------------------------------------*/

static int	add_grp_attrs (	
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_name,
				struct dnsFlagStat	*flags,
				D2_pbhead			*add_error,
				D2_error			**add_error_info)
{
	dns_attr_name_t *dns_type;
	dns_attrvalue_t dns_value;
	D2_a_value		*val_struct;
	u_char          type_str[D2_OBJ_IDL_MAX],	
					uncompressed_oid [D2_OBJ_IDL_MAX];
	int				local_error,	r_code,		count;
	const char function_name[] = "add_grp_attrs";

	r_code = D2_NOERROR;
	local_error = FALSE;

	/*
		look for the member attribute in the attribute list
																*/
	while (!local_error && (attr_list != (D2_a_info *) NULL)) {

		/*
			Convert the OID from IAPL* internal compressed format 
			uncompressed string format.
																*/
		if (d27_301_oid_str (&attr_list->d2_a_type, (char *)type_str) == D2_ERROR) {

	   		DUA_TRACE_D27_301_FAILED
			add_error->d2_retcode = D2_CALL_ERR;
			add_error->d2_errclass = D2_PAR_ERR;
			add_error->d2_errvalue = D2_NO_BIND;
			local_error = TRUE;
			r_code = D2_ERROR;

        } else {

			d21_cds_uncompress_oid (type_str, uncompressed_oid);

			if (strcmp ((char *) type_str, DSW_MEMBER_OID) == (int) NULL) {

				if (addval_member (attr_list, dns_name, flags, add_error)
					== D2_ERROR) {
					r_code =  D2_ERROR;
					local_error = TRUE;
				}

			} else { 

				dns_type = (dns_attr_name_t *) uncompressed_oid;
				val_struct = attr_list->d2_a_val;
				count = 0;
				local_error = FALSE;

				while (!local_error && (count != attr_list->d2_a_no_val)) {

					if (d21_x500val2cdsval (val_struct, &dns_value) 
						== D2_ERROR) {

						add_error->d2_retcode = D2_CALL_ERR;
						add_error->d2_errclass = D2_PAR_ERR;
						add_error->d2_errvalue = D2_CONSTRAINT_VIOLATION;
						d21_cds_attr_error (dns_name, &attr_list->d2_a_type,
							add_error_info);
						r_code = D2_ERROR;
						local_error = TRUE;

					} else {

						/*
							add the attribute value to the CDS group
																		*/
						if (cdsAddAttrValue (dns_name, dns_type, dnsSet, dnsObj,
							&dns_value, NULL_DNS_CTS, flags) == DNS_ERROR) {
							DUA_TRACE_CDS_FUNCTION_FAILED("cdsAddAttrValue")
							d21_cds_map_error (flags->fsLocStat, add_error);
							if (flags->fsLocStat == DNS_INVALIDUPDATE) {

								d21_cds_attr_error (dns_name, 
										&attr_list->d2_a_type, add_error_info);
							}

							r_code = D2_ERROR;
							local_error = TRUE;

						} else {

							val_struct ++; 
							count ++;
						}
					}
				}
			}
		}

		attr_list = attr_list->d2_a_next;
	}

	return (r_code);
}



/*-----------------------------------------------------------------------

	addval_member ():

	Given a member attribute, adds each member to the cds group.

 -----------------------------------------------------------------------*/

static int	addval_member (	
				D2_a_info			*attr_list,
				dns_full_name_t		*dns_name,
				struct dnsFlagStat	*flags,
				D2_pbhead			*add_error)
{
	dns_full_name_t	*dns_member;
	D2_a_value		*val_struct;
	u_char			*attr_value,	member[D2_DNL_MAX];
	int				r_code,		count,	error;
	const char function_name[] = "getval_objectClass";

	r_code = D2_NOERROR;
	error = FALSE;

	/*	
		Add all the members from the member attribute to the
		groupOfNames object.	
															*/
	count = 0;
	while ((count != attr_list->d2_a_no_val) && (!error)) {

		/*
			Check the attribute value syntax.
												*/
		val_struct = attr_list->d2_a_val;
		if (val_struct->d2_a_rep != D2_DISTNAME) {
			/*
				assign the error code, class and value
														*/
			add_error->d2_retcode = D2_CALL_ERR;
			add_error->d2_errclass = D2_PAR_ERR;
			add_error->d2_errvalue = D2_NO_BIND;
			r_code = D2_ERROR; 
			error = TRUE;
		} else { 

			attr_value = val_struct->d2_at_value.d2_obj_name_a_v;

			if (d21_x500name2cdsname (attr_value, member) == D2_ERROR) {

					DUA_TRACE_X500_TO_CDS_NAME_FAILED
					add_error->d2_retcode = D2_CALL_ERR;
					add_error->d2_errclass = D2_PAR_ERR;
					add_error->d2_errvalue = D2_NOT_SPEC;
					return (D2_ERROR);
			}

			dns_member = (dns_full_name_t *) member;

			/*
				add a group member to the groupOfNames object
																*/
			if (cdsAddMbr (dns_name, dns_member, dnsPrincipal, flags)
				== DNS_ERROR) {

				DUA_TRACE_CDS_FUNCTION_FAILED("cdsAddMbr")
				d21_cds_map_error (flags->fsLocStat, add_error);
				error = TRUE;
				r_code = D2_ERROR;
			} else {
				val_struct ++;
				count ++;
			}
		}
	}

	return (r_code);
}

static int getval_class_and_version (
                                dns_simple_name_t ** dns_class,
                                dnsVersion_t    * dns_version,
				boolean         * dns_version_found,
                                D2_a_info       *attr_list,
                                D2_pbhead       *add_error)
{
u_char type_str[D2_DNL_MAX];
u_char uncompressed_type_str[D2_DNL_MAX];
int found;
const char function_name[] = "getval_class_and_version";

found = 0;
*dns_class = NULL;
*dns_version_found = FALSE;

while ((found < 2) && (attr_list != (D2_a_info *) NULL))
  {
  if (d27_301_oid_str (&attr_list->d2_a_type, (char *)type_str) == D2_ERROR)
    {
    add_error->d2_retcode = D2_CALL_ERR;
    add_error->d2_errclass = D2_PAR_ERR;
    add_error->d2_errvalue = D2_NO_BIND;
    DUA_TRACE_D27_301_FAILED
    return (D2_ERROR);
    }

  d21_cds_uncompress_oid (type_str, uncompressed_type_str);

  if (strcmp ((char *) uncompressed_type_str, DSW_CDS_CLASS_OID) == 0)
    {
    *dns_class = (dns_simple_name_t *) 
				attr_list->d2_a_val->d2_at_value.d2_oct_a_v;
    found++;
    }
  else
    {
    if (strcmp ((char *) uncompressed_type_str, DSW_CDS_CLASS_VERSION_OID) == 0)
      {
      dns_version->cdsMajVer = 
			(attr_list->d2_a_val->d2_at_value.d2_prt_a_v)[0] - '0';
      dns_version->cdsMinVer = 
			(attr_list->d2_a_val->d2_at_value.d2_prt_a_v)[2] - '0';
      *dns_version_found = TRUE;
      found++;
      }
    }

  attr_list = attr_list->d2_a_next;
  } /* end of while */

return (D2_NOERROR);
}
