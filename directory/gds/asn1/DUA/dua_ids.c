/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dua_ids.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:21  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:22:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:10  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:02  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:25:50  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:10  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:31  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:04:30  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:44:26  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:06:02  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:29:23  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:09:38  marrek
 * 	Introduce IA5_SYNTAX
 * 	[1993/02/01  17:49:00  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:11:34  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:08:32  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  16:26:50  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  13:15:29  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:03:03  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:49:11  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dua_ids.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:32 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/


/*
 * Module:
 *	DUA
 * Description:
 *	This file contains:
 *	. primitives "dua_typename_ids" to translate an item from
 *	  its "dua" representation into the equivalent "ids" representation.
 */

#include <gds.h>
#include <stdlib.h>
#include <string.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL *********/

static signed32 dua_Eattrs_ids( E_Attr * );
static signed32 dua_assertion_ids( dua_assertion * , ids_assertion * );

/************************************************
 * handling non error data types;
 * from dua representation to ids representation
 ************************************************/
signed32 dua_bit_string_ids (
  dua_bit_string * tdua,
  ids_bit_string * tids )
{
const char function_name[] = "dua_bit_string_ids";
	if (tdua->nb)
	{    if (((tdua->nb + 7) >> 3) > ((tids->nb + 7)  >> 3))
	     {    if (tids->nb == 0)
	       {    if ((tids->contents =
			D27_MALLOC (svc_c_sev_warning,(tdua->nb >> 3) +1)) == NULL)
				{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
	       }
	       else
	       {    if ((tids->contents = D27_REALLOC(svc_c_sev_warning,
				tids->contents, (tdua->nb >> 3) +1)) == NULL)
				{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
	       }
	       tids->nb = tdua->nb;
	     }
	     ASN1_MOVE (tdua->contents,
			tids->contents,
			((tdua->nb - 1) >> 3) +1);
	}
	else
	{	tids->contents  = 0;
		tids->nb 	= 0;
	}
	return(D2_NOERROR);
}

signed32 dua_utc_time_ids (
  dua_utc_time * tdua,
  ids_utc_time * tids )
{
const char function_name[] = "dua_utc_time_ids";
	if (*tdua)
	{	tids->nb = dua_utc_time_len (-1, *tdua);
		if ((tids->contents =
			D27_MALLOC (svc_c_sev_warning,tids->nb)) == NULL)
				{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
		tids->nb =
			dua_utc_time_cod (tids->contents, -1, 0, *tdua) -
			tids->contents;
	}
	else
	{	tids->nb = 0;
		tids->contents = 0;
	}
	return(D2_NOERROR);
}

static signed32 dua_Eattrs_ids(
  E_Attr * e_attr )
{
const char function_name[] = "dua_Eattrs_ids";
asn1_field   a_val;

  switch (e_attr->e_type) {
	case common_name:
	case pds_name :
		if (e_attr->e_value.P_str)
		{ if ((a_val.v =
			  D27_MALLOC (svc_c_sev_warning,dua_string_len
				   (-1, e_attr->e_value.P_str))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			a_val.l =
			dua_string_cod(a_val.v, -1, 0, e_attr->e_value.P_str)-
				a_val.v;
		}
		else
		{	a_val.v = D27_MALLOC (svc_c_sev_warning,a_val.l = 2);
			(a_val.v) [0] = 19;
			(a_val.v) [1] = 0;
		}
		break;

	case teletex_common_name:
	case teletex_organization_name:
		if (e_attr->e_value.T_str)
		{ if ((a_val.v =
			  D27_MALLOC (svc_c_sev_warning,dua_Tstring_len
				   (-1, e_attr->e_value.T_str))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			a_val.l =
			dua_Tstring_cod(a_val.v, -1, 0, e_attr->e_value.T_str)-
				a_val.v;
		}
		else
		{	a_val.v = D27_MALLOC (svc_c_sev_warning,a_val.l = 2);
			(a_val.v) [0] = 20;
			(a_val.v) [1] = 0;
		}
		break;

	case teletex_personal_name :
		if ((a_val.v =
			  D27_MALLOC (svc_c_sev_warning,TeletexPersonalName_len
				   (-1, &e_attr->e_value.pn_val))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		a_val.l =
			TeletexPersonalName_cod(a_val.v, -1, 0, &e_attr->e_value.pn_val)-
				a_val.v;
		break;

	case teletex_organizational_unit_names :
		if ((a_val.v =
			  D27_MALLOC (svc_c_sev_warning,TeletexOrganizationUnitNames_len
				   (-1, &e_attr->e_value.ou_val))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		a_val.l =
			TeletexOrganizationUnitNames_cod(a_val.v, -1, 0, &e_attr->e_value.ou_val)-
				a_val.v;
		break;

	case teletex_domain_defined_attributes :
		if ((a_val.v =
			  D27_MALLOC (svc_c_sev_warning,TeletexDomainDefinedAttributes_len
				   (-1, &e_attr->e_value.dd_val))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		a_val.l =
			TeletexDomainDefinedAttributes_cod(a_val.v, -1, 0, &e_attr->e_value.dd_val)-
				a_val.v;
		break;

	case physical_delivery_country_name :
	case postal_code :
		if ((a_val.v =
			  D27_MALLOC (svc_c_sev_warning,str_NP_choice_len
				   (-1, &e_attr->e_value.NP_val))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		a_val.l =
			str_NP_choice_cod(a_val.v, -1, 0, &e_attr->e_value.NP_val)-
				a_val.v;
		break;

	case physical_delivery_office_name :
	case physical_delivery_office_number :
	case extension_OR_address_components :
	case physical_delivery_personal_name :
	case physical_delivery_organization_name :
	case extension_physical_delivery_address_components :
	case street_address :
	case post_office_box_address :
	case poste_restante_address :
	case unique_postal_name :
	case local_postal_attributes :
		if ((a_val.v =
			  D27_MALLOC (svc_c_sev_warning,PDSParameter_len
				   (-1, &e_attr->e_value.pds_val))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		a_val.l =
			PDSParameter_cod(a_val.v, -1, 0, &e_attr->e_value.pds_val)
			- a_val.v;
		break;

	case unformatted_postal_address :
		if ((a_val.v =
			  D27_MALLOC (svc_c_sev_warning,UnformattedPostalAddress_len
				   (-1, &e_attr->e_value.up_addr))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		a_val.l =
			UnformattedPostalAddress_cod(a_val.v, -1, 0, &e_attr->e_value.up_addr)
			- a_val.v;
		if (e_attr->e_value.up_addr.p_address != (PostalAddress_Lines *)NULL)
			free(e_attr->e_value.up_addr.p_address);
		break;

	case extended_network_address :
		if ((a_val.v =
			  D27_MALLOC (svc_c_sev_warning,ExtendedNetworkAddress_len
				   (-1, &e_attr->e_value.en_addr))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		a_val.l =
			ExtendedNetworkAddress_cod(a_val.v, -1, 0, &e_attr->e_value.en_addr)
			- a_val.v;
		if (e_attr->e_value.en_addr.typ == E163_ADDRESS_TYPE)
		{
		  free (e_attr->e_value.en_addr.v.e163_4_address);
		}
		else
		{
		  if (e_attr->e_value.en_addr.v.psap_address->nb_nsap > 0)
		  	free (e_attr->e_value.en_addr.v.psap_address->nsap);
		  free (e_attr->e_value.en_addr.v.psap_address);
		}
		break;

	case terminal_type :
		if ((a_val.v =
			  D27_MALLOC (svc_c_sev_warning,TerminalType_len
				   (-1, &e_attr->e_value.ter_val))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		a_val.l =
			TerminalType_cod(a_val.v, -1, 0, &e_attr->e_value.ter_val)
			- a_val.v;
		break;

	default:
		D2_ASN1_ERR_RETURN_WITH_TRACE
	}

  e_attr->e_value.a_val = a_val;
  return(D2_NOERROR);

}

signed32 dua_att_value_ids (
  dua_att_value * tdua,
  ids_att_value * tids )
{
const char function_name[] = "dua_att_value_ids";
signed32 data_len, i;

	switch (tdua->mode)
	{
	case D2_ASN1:
                tids->l = tdua->value.asn1_value.l;
		if ((tids->v = D27_MALLOC (svc_c_sev_warning,tids->l)) == NULL)
		  {
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		memcpy(tids->v,tdua->value.asn1_value.v,tids->l);
		break;

	case D2_DISTNAME:
	{	ids_name ids_name_buf;
		dua_name_ids (tdua->value.name_value,
			      &ids_name_buf);
		if ((tids->v = D27_MALLOC (svc_c_sev_warning,ids_name_len
					      (-1, &ids_name_buf))) == NULL)
				{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
		tids->l = ids_name_cod (tids->v, -1, 0, &ids_name_buf) -
				tids->v;
		i_dist_name_free (&ids_name_buf);
		break;
	}

	case D2_OBJ_IDENT:
		  tids->v =
		      D27_MALLOC
			(svc_c_sev_warning,dua_oid_asn1_len
			   (-1, &(tdua->value.oid_value.contents.oid_asn1)));
		  tids->l =
		     dua_oid_asn1_cod
			(tids->v, -1, 0,
			 &(tdua->value.oid_value.contents.oid_asn1)) -
			tids->v;
		break;

	case D2_NUMERIC:
		if (tdua->value.str_value)
		  {
		  if ((tids->v = D27_MALLOC (svc_c_sev_warning,dua_Nstring_len
				   (-1, tdua->value.str_value))) == NULL)
				{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
		  tids->l = dua_Nstring_cod
				(tids->v,-1,0,tdua->value.str_value)-tids->v;
		  }
		else
		  {
		  tids->v = D27_MALLOC(svc_c_sev_warning,tids->l = 2);
		  (tids->v) [0] = 18;
		  (tids->v) [1] = 0;
		  }
		break;

	case D2_IA5:
		if (tdua->value.str_value)
		  {
		  if ((tids->v = D27_MALLOC (svc_c_sev_warning,dua_IA5string_len
				   (-1, tdua->value.str_value))) == NULL)
				{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
		  tids->l = dua_IA5string_cod
				(tids->v,-1,0,tdua->value.str_value)-tids->v;
		  }
		else
		  {
		  tids->v = D27_MALLOC(svc_c_sev_warning,tids->l = 2);
		  (tids->v) [0] = 22;
		  (tids->v) [1] = 0;
		  }
		break;

	case D2_CEXACT:
	case D2_CIGNORE:
	case D2_T61:
		if (tdua->value.str_value)
		  {
		  if ((tids->v = D27_MALLOC (svc_c_sev_warning,dua_Tstring_len
				   (-1, tdua->value.str_value))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		  tids->l = dua_Tstring_cod
				(tids->v,-1,0,tdua->value.str_value)-tids->v;
		  }
		else
		  {
		  tids->v = D27_MALLOC(svc_c_sev_warning,tids->l = 2);
		  (tids->v) [0] = 20;
		  (tids->v) [1] = 0;
		  }
		break;

	case D2_PRINTABLE:
	case D2_TELNR_STX:
	case D2_COUNTRY_STX:
		if (tdua->value.str_value)
		  {
		  if ((tids->v = D27_MALLOC (svc_c_sev_warning,dua_string_len
				   (-1, tdua->value.str_value))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		  tids->l = dua_string_cod
				(tids->v,-1,0,tdua->value.str_value)-tids->v;
		  }
		else
		  {
		  tids->v = D27_MALLOC(svc_c_sev_warning,tids->l = 2);
		  (tids->v) [0] = 19;
		  (tids->v) [1] = 0;
		  }
		break;

	case D2_BOOLEAN:
		tids->v =
		    D27_MALLOC (svc_c_sev_warning,dua_boolean_len (-1, tdua->value.int_value));
		tids->l =
		dua_boolean_cod (tids->v, -1, 0, tdua->value.int_value) -
			tids->v;
		break;

	case D2_INTEGER:
		tids->v =
		    D27_MALLOC (svc_c_sev_warning,dua_int_len (-1, tdua->value.int_value));
		tids->l =
		dua_int_cod (tids->v, -1, 0, tdua->value.int_value) -
			tids->v;
		break;
	case D2_ACCL:
	case D2_PASSWD_STX:
	case D2_OCTET:
		if (tdua->value.oct_value.nb &&
		    tdua->value.oct_value.contents)
		   { if ((tids->v = D27_MALLOC (svc_c_sev_warning,dua_octet_string_len
				     (-1, &(tdua->value.oct_value)))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			dua_octet_string_cod (tids->v, -1, 0,
					      &(tdua->value.oct_value)) -
				tids->v;
		}
		else
		{	tids->v = D27_MALLOC(svc_c_sev_warning,tids->l = 2);
			(tids->v) [0] = 4;
			(tids->v) [1] = 0;
		}
		break;

	case D2_TIME:
		if (tdua->value.utc_time_value)
		{
		       if ((tids->v = D27_MALLOC (svc_c_sev_warning,dua_utc_time_len
				     (-1, tdua->value.utc_time_value))) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			dua_utc_time_cod (tids->v, -1, 0,
					  tdua->value.utc_time_value) -
				tids->v;
		}
		else
		{	tids->v = D27_MALLOC(svc_c_sev_warning,tids->l = 2);
			(tids->v) [0] = 23;
			(tids->v) [1] = 0;
		}

		break;

	case D2_PSAP_STX: /* PSAP 	*/
		if (data_len = dua_psap_len (-1, &(tdua->value.psap_value)))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			dua_psap_cod (tids->v, -1, 0,
				      &(tdua->value.psap_value)) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;

	case D2_GUIDE:
		if (data_len = guide_len (-1, tdua->value.guide_value))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			guide_cod (tids->v, -1, 0,
				      tdua->value.guide_value) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;
	case D2_TLXNR_STX:
		if (data_len = tlx_number_len (-1, tdua->value.tlx_value))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			tlx_number_cod (tids->v, -1, 0,
				      tdua->value.tlx_value) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;
	case D2_TTXID_STX:
		if (data_len = teletex_id_len (-1, tdua->value.ttxid_value))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			teletex_id_cod (tids->v, -1, 0,
				      tdua->value.ttxid_value) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;
	case D2_FAXNR_STX:
		if (data_len = faxNumber_len (-1, tdua->value.fax_value))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			faxNumber_cod (tids->v, -1, 0,
				      tdua->value.fax_value) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;

	case D2_MHS_PR_DM_STX:
	case D2_PR_DM_STX:
	case D2_INT_LIST:

		if (data_len = int_seq_len (-1, &(tdua->value.int_seq_value)))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			int_seq_cod (tids->v, -1, 0,
				      &(tdua->value.int_seq_value)) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;
	case D2_T61_LIST:
		if (data_len = t61_seq_len (-1, tdua->value.t61_seq_value))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			t61_seq_cod (tids->v, -1, 0,
				      tdua->value.t61_seq_value) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;

	case D2_POST_AD_STX:
	case D2_T61_PR_LIST:
		if (data_len = t61_pr_seq_len (-1, tdua->value.post_value))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			t61_pr_seq_cod (tids->v, -1, 0,
				      tdua->value.post_value) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;

	case D2_PRT_LIST:
	case D2_CIGN_LIST:	
		if (data_len = print_seq_len (-1, tdua->value.pr_seq_value))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			print_seq_cod (tids->v, -1, 0,
				      tdua->value.pr_seq_value) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;

	case D2_MHS_DLSP_STX:
		/* In case extension attributes are present,
		   encode them first */
		if (tdua->value.dls_perm.typ != MEMBER_OF_GROUP )
		 {
		  if (tdua->value.dls_perm.v.individual.address.e_attrs != (E_Attrs *)NULL)
		 {
		   for (i=0; i < tdua->value.dls_perm.v.individual.address.e_attrs->n; i++)
		     {
			if (dua_Eattrs_ids(tdua->value.dls_perm.v.individual.address.e_attrs->v + i) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			}
		  }
		 if (tdua->value.dls_perm.v.individual.d_name.name != (dua_dist_name *)NULL)
		    { dua_att_value dua_val;
		      asn1_field a_val;
		      dua_val.mode = D2_DISTNAME;
		      dua_val.value.name_value = 
				tdua->value.dls_perm.v.individual.d_name.name;
		      dua_att_value_ids (&dua_val, &a_val);
		      free(tdua->value.dls_perm.v.individual.d_name.name);
		      tdua->value.dls_perm.v.individual.d_name.a_val = a_val;
		    }
		    else
		    {
		      tdua->value.dls_perm.v.individual.d_name.a_val.l = 0;
		    }
		 }
		 else
		  { dua_att_value dua_val;
		    asn1_field a_val;
		    dua_val.mode = D2_DISTNAME;
		    dua_val.value.name_value = 
				tdua->value.dls_perm.v.member_of_group.name;
		    if (dua_att_value_ids (&dua_val, &a_val) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		    free(tdua->value.dls_perm.v.member_of_group.name);
		    tdua->value.dls_perm.v.member_of_group.a_val = a_val;
		  }
		if (data_len = DLSubmitPermission_len(-1, &tdua->value.dls_perm))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			DLSubmitPermission_cod (tids->v, -1, 0,
				      &tdua->value.dls_perm) - tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		if (tdua->value.dls_perm.typ == MEMBER_OF_GROUP)
		 {
		    if (tdua->value.dls_perm.v.member_of_group.a_val.l != 0)
			free(tdua->value.dls_perm.v.member_of_group.a_val.v);
		  }
		else
		{
		  if (tdua->value.dls_perm.v.individual.d_name.a_val.l != 0)
		  	free(tdua->value.dls_perm.v.individual.d_name.a_val.v);

		  if (tdua->value.dls_perm.v.individual.address.e_attrs != (E_Attrs *)NULL)
		   {
		     for (i=0; i < tdua->value.dls_perm.v.individual.address.e_attrs->n; i++)
		       {
			  free(tdua->value.dls_perm.v.individual.address.e_attrs->v[i].e_value.a_val.v);
		        }
		     }
		 }
		break;

	case D2_MHS_ORADR_STX:
		/* In case extension attributes are present,
		   encode them first */
		if (tdua->value.or_address.e_attrs != (E_Attrs *)NULL)
		 {
		   for (i=0; i < tdua->value.or_address.e_attrs->n; i++)
		     {
			if (dua_Eattrs_ids(tdua->value.or_address.e_attrs->v + i) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			}
		  }
		if (data_len = ORAddress_len (-1, &tdua->value.or_address))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			ORAddress_cod (tids->v, -1, 0,
				      &tdua->value.or_address) - tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		if (tdua->value.or_address.e_attrs != (E_Attrs *)NULL)
		 {
		   for (i=0; i < tdua->value.or_address.e_attrs->n; i++)
		     {
			free(tdua->value.or_address.e_attrs->v[i].e_value.a_val.v);
		      }
		  }
		break;

	case D2_MHS_ORNAME_STX:
		/* In case extension attributes are present,
		   encode them first */
		if (tdua->value.or_name.address.e_attrs != (E_Attrs *)NULL)
		 {
		   for (i=0; i < tdua->value.or_name.address.e_attrs->n; i++)
		     {
			if (dua_Eattrs_ids(tdua->value.or_name.address.e_attrs->v + i) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			}
		  }
		if (tdua->value.or_name.d_name.name != (dua_dist_name *)NULL)
		    { dua_att_value dua_val;
		      asn1_field a_val;
		      dua_val.mode = D2_DISTNAME;
		      dua_val.value.name_value = 
				tdua->value.or_name.d_name.name;
		      dua_att_value_ids (&dua_val, &a_val);
		      tdua->value.or_name.d_name.a_val = a_val;
		     }
		else
		  {
		     tdua->value.or_name.d_name.a_val.l = 0;
		   }
		if (data_len = ORName_len (-1, &tdua->value.or_name))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			ORName_cod (tids->v, -1, 0,
				      &tdua->value.or_name) - tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		if (tdua->value.or_name.d_name.a_val.l != 0)
			free (tdua->value.or_name.d_name.a_val.v);
		if (tdua->value.or_name.address.e_attrs != (E_Attrs *)NULL)
		 {
		   for (i=0; i < tdua->value.or_name.address.e_attrs->n; i++)
		     {
			free(tdua->value.or_name.address.e_attrs->v[i].e_value.a_val.v);
		      }
		  }
		break;

	case D2_CERTIFICATE:
		if (data_len = duaCertificate_len(
					-1,&(tdua->value.certificate)))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			duaCertificate_cod (tids->v, -1, ASN1_X509,
				      &(tdua->value.certificate)) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;

	case D2_CERT_PAIR:
		if (data_len = duaCertPair_len (-1,&(tdua->value.cert_pair)))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			duaCertPair_cod (tids->v, -1, ASN1_X509,
				      &(tdua->value.cert_pair)) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;

	case D2_CERT_LIST:
		if (data_len = duaCertList_len (-1,&(tdua->value.cert_list)))
		{	if ((tids->v = D27_MALLOC (svc_c_sev_warning,data_len)) == NULL)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			tids->l =
			duaCertList_cod (tids->v, -1, ASN1_X509,
				      &(tdua->value.cert_list)) -
				tids->v;
		}
		else
		{	tids->l = 0;
			tids->v = 0;
		}
		break;

	default:
		D2_ASN1_ERR_RETURN_WITH_TRACE
	}
return(D2_NOERROR);
}

static signed32 dua_assertion_ids(
  dua_assertion * tdua,
  ids_assertion * tids )
{
const char function_name[] = "dua_assertion_ids";
 	tids->type.l = tdua->type.l;
	if (( tids->type.v = D27_MALLOC(svc_c_sev_warning,tids->type.l)) == NULL)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	strncpy((char *)tids->type.v,(char *)tdua->type.v,(size_t)tdua->type.l);

	if (dua_att_value_ids (&(tdua->value), &(tids->value))
							!= D2_NOERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }

	dua_att_value_free(&(tdua->value));

	return(D2_NOERROR);
}

signed32 dua_rdname_ids(
  dua_rdname * tdua,
  ids_rdname * tids )
{
const char function_name[] = "dua_rdname_ids";
signed32 i;
dua_assertion * dua_ptr = tdua->assertions;

tids->nb_assertions = tdua->nb_assertions;

if (tdua->nb_assertions && tdua->assertions)
  {
  if ( (tids->assertions =
    D27_MALLOC (svc_c_sev_warning,(tdua->nb_assertions) * sizeof (ids_assertion))) == NULL)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
  for (i=0; i<tdua->nb_assertions; i++)
	{
	if (dua_assertion_ids (dua_ptr + i, tids->assertions + i)
						!= D2_NOERROR )
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	}
  }
else
  {
  tids->nb_assertions = 0;
  }

return (D2_NOERROR);
}

signed32 dua_dist_name_ids (
  dua_dist_name * tdua,
  ids_dist_name * tids )
{
const char function_name[] = "dua_dist_name_ids";
signed32 i;

tids->nb_rdnames = tdua->nb_rdnames;
if (tdua->nb_rdnames && tdua->rdnames)
  {
  if ( (tids->rdnames = D27_MALLOC(svc_c_sev_warning,tids->nb_rdnames * sizeof(ids_rdname)))
						  == NULL)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }

  for (i=0; i<tdua->nb_rdnames; i++)
    {
    if (dua_rdname_ids (tdua->rdnames + i, tids->rdnames + i)
							!= D2_NOERROR)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    }
  }
else
  {
  tids->nb_rdnames = 0;
  }

return (D2_NOERROR);
}
