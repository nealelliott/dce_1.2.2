/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dua_free.c,v $
 * Revision 1.1.12.2  1996/02/18  19:42:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:19  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:22:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:09  root]
 * 
 * Revision 1.1.10.3  1994/06/21  14:44:01  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:25:44  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:07:09  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:26  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  18:04:28  marrek
 * 	February 1994 code drop
 * 	[1994/02/14  16:15:22  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:44:13  marrek]
 * 
 * Revision 1.1.8.1  1993/10/14  15:39:45  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  15:38:13  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  10:04:52  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:29:09  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:09:27  marrek
 * 	Introduce IA5_SYNTAX
 * 	[1993/02/01  17:48:15  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:11:31  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:08:27  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  16:26:30  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  13:15:13  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:02:56  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:49:03  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dua_free.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 19:42:31 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*
 *
 * Module:
 *	DUA
 * Description:
 *	This file contains:
 *	. primitives "dua_typename_free" to recover the memory space
 *	  generated when moving from ids to dua representation.
 */

#include <gds.h>
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL *********/

static void Eattr_free ( E_Attr * );
static void d_assertion_free ( dua_assertion * );
static void d_rdname_free ( dua_rdname * );
static void ora_free ( ORAddress * );

/***********************************************************************
 * handling non error data types;
 * from ids representation to dua representation
***********************************************************************/

static void Eattr_free(
  E_Attr * e_attr )
{
   switch (e_attr->e_type) {
	case common_name :
	case pds_name :
		dua_string_free(e_attr->e_value.P_str);
		break;
	case teletex_common_name :
	case teletex_organization_name :
		dua_Tstring_free(e_attr->e_value.T_str);
		break;
	case teletex_personal_name :
		TeletexPersonalName_free(&e_attr->e_value.pn_val);
		break;
	case teletex_organizational_unit_names :
		TeletexOrganizationUnitNames_free(&e_attr->e_value.ou_val);
		break;
	case teletex_domain_defined_attributes :
		TeletexDomainDefinedAttributes_free(&e_attr->e_value.dd_val);
		break;
	case physical_delivery_country_name :
	case postal_code :
		str_NP_choice_free(&e_attr->e_value.NP_val);
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
		PDSParameter_free(&e_attr->e_value.pds_val);
		break;
	case unformatted_postal_address :
		UnformattedPostalAddress_free(&e_attr->e_value.up_addr);
		break;
	case extended_network_address :
		ExtendedNetworkAddress_free(&e_attr->e_value.en_addr);
		break;
	case terminal_type :
		TerminalType_free(&e_attr->e_value.ter_val);
		break;
   }

}

void dua_att_value1_free (
  dua_att_value1 * tdua )
{
signed32 i;
	switch (tdua->mode)
	{
	case D2_ASN1:
		break;

	case D2_OBJ_IDENT:
		dua_oid_asn1_free
			  (&(tdua->value.oid_value.contents.oid_asn1));

		break;

	case D2_NUMERIC:
		if (tdua->value.str_value)
			dua_Nstring_free (tdua->value.str_value);
		break;

	case D2_IA5:
		if (tdua->value.str_value)
			dua_IA5string_free (tdua->value.str_value);
		break;

	case D2_CEXACT:
	case D2_CIGNORE:
	case D2_T61:
		if (tdua->value.str_value)
			dua_Tstring_free (tdua->value.str_value);
		break;

	case D2_PRINTABLE:
	case D2_TELNR_STX:
	case D2_COUNTRY_STX:
		if (tdua->value.str_value)
			dua_string_free (tdua->value.str_value);
		break;

	case D2_BOOLEAN:
		break;

	case D2_INTEGER:
		break;
	case D2_ACCL:
	case D2_PASSWD_STX:
	case D2_OCTET:
		if (tdua->value.oct_value.contents)
		     dua_octet_string_free (&tdua->value.oct_value);
		break;

	case D2_TIME:
		if (tdua->value.utc_time_value)
			dua_utc_time_free (tdua->value.utc_time_value);
		break;

	case D2_PSAP_STX:
		dua_psap_free (&(tdua->value.psap_value));
		break;

	case D2_GUIDE:
		guide_free (&(tdua->value.guide_value));
		break;

	case D2_TTXID_STX:
		teletex_id_free (&(tdua->value.ttxid_value));
		break;

	case D2_TLXNR_STX:
		tlx_number_free (&(tdua->value.tlx_value));
		break;

	case D2_FAXNR_STX:
		faxNumber_free (&(tdua->value.fax_value));
		break;

	case D2_MHS_PR_DM_STX:
	case D2_PR_DM_STX:
	case D2_INT_LIST:
		int_seq_free (&(tdua->value.int_seq_value));
		break;

	case D2_POST_AD_STX:
	case D2_T61_PR_LIST:
		t61_pr_seq_free (&(tdua->value.post_value));
		break;

	case D2_T61_LIST:
		t61_seq_free (&(tdua->value.t61_seq_value));
		break;

	case D2_PRT_LIST:
	case D2_CIGN_LIST:
		print_seq_free (&(tdua->value.pr_seq_value));
		break;

	case D2_MHS_ORADR_STX:
		if (tdua->value.or_address.e_attrs != (E_Attrs *)NULL)
		{
		   for (i=0; i < tdua->value.or_address.e_attrs->n; i++)
		   {
		      Eattr_free(tdua->value.or_address.e_attrs->v + i);
		    }
		 }
		ORAddress_free (&(tdua->value.or_address));
		break;

	case D2_MHS_ORNAME_STX:
		if (tdua->value.or_name.address.e_attrs != (E_Attrs *)NULL)
		{
		   for (i=0; i < tdua->value.or_name.address.e_attrs->n; i++)
		   {
		      Eattr_free(tdua->value.or_name.address.e_attrs->v + i);
		    }
		 }
		if (tdua->value.or_name.d_name.name != (dua_dist_name *)NULL)
		 {
			d_name_free (tdua->value.or_name.d_name.name);
			free (tdua->value.or_name.d_name.name);
		  }
		tdua->value.or_name.d_name.a_val.l = 0;
		ORName_free (&(tdua->value.or_name));
		break;

	case D2_MHS_DLSP_STX:
		if (tdua->value.dls_perm.typ != MEMBER_OF_GROUP &&
		    tdua->value.dls_perm.v.individual.address.e_attrs != (E_Attrs *)NULL)
		{
		   for (i=0; i < tdua->value.dls_perm.v.individual.address.e_attrs->n; i++)
		   {
		      Eattr_free(tdua->value.dls_perm.v.individual.address.e_attrs->v + i);
		    }
		 }
		DLSubmitPermission_free (&(tdua->value.dls_perm));
		break;

	case D2_CERTIFICATE:
		duaCertificate_free (&(tdua->value.certificate));
		break;

	case D2_CERT_PAIR:
		duaCertPair_free (&(tdua->value.cert_pair));
		break;

	case D2_CERT_LIST:
		duaCertList_free (&(tdua->value.cert_list));
		break;

	default:
		break;
	}
}

void dua_att_value_free (
  dua_att_value * tdua )
{
	switch (tdua->mode)
	{
	case D2_ASN1:
		break;

	case D2_DISTNAME:
		d_name_free (tdua->value.name_value);
		free (tdua->value.name_value);
		break;

	case D2_OBJ_IDENT:
		if (tdua->value.oid_value.contents.oid_asn1.v) 
			free (tdua->value.oid_value.contents.oid_asn1.v);

		break;

	case D2_CEXACT:
	case D2_CIGNORE:
	case D2_PRINTABLE:
	case D2_TELNR_STX:
	case D2_NUMERIC:
	case D2_COUNTRY_STX:
	case D2_T61:
	case D2_IA5:
		if (tdua->value.str_value) 
		  {
		  free (tdua->value.str_value);
		  tdua->value.str_value = (string)0;
		  }
		break;

	case D2_BOOLEAN:
		break;

	case D2_INTEGER:
		break;
	case D2_ACCL:
	case D2_PASSWD_STX:
	case D2_OCTET:
		if (tdua->value.oct_value.contents)
		     free (tdua->value.oct_value.contents);
		break;

	case D2_TIME:
		if (tdua->value.utc_time_value)
			free (tdua->value.utc_time_value);
		break;

	case D2_PSAP_STX:
		dua_psap_free (&(tdua->value.psap_value));
		break;

	case D2_GUIDE:
		if (tdua->value.guide_value)
			free (tdua->value.guide_value);
		break;
		
	case D2_TLXNR_STX:
		if (tdua->value.tlx_value)
			free (tdua->value.tlx_value);
		break;

	case D2_TTXID_STX:
		if (tdua->value.ttxid_value)
			free (tdua->value.ttxid_value);
		break;

	case D2_FAXNR_STX:
		if (tdua->value.fax_value)
			free (tdua->value.fax_value);
		break;

	case D2_MHS_PR_DM_STX:
	case D2_PR_DM_STX:
	case D2_INT_LIST:
		if (tdua->value.int_seq_value.nb_item)
			free (tdua->value.int_seq_value.int_item);
		break;


	case D2_POST_AD_STX:
	case D2_T61_PR_LIST:
		if (tdua->value.post_value)
			free (tdua->value.post_value);
		break;

	case D2_T61_LIST:
		if (tdua->value.t61_seq_value)
			free (tdua->value.t61_seq_value);
		break;

	case D2_PRT_LIST:
	case D2_CIGN_LIST:	/*? no		*/
		if (tdua->value.pr_seq_value)
			free (tdua->value.pr_seq_value);
		break;

	case D2_MHS_ORADR_STX:
		ora_free(&tdua->value.or_address);
		break;

	case D2_MHS_ORNAME_STX:
		ora_free(&tdua->value.or_name.address);
		break;

	case D2_MHS_DLSP_STX:
		switch (tdua->value.dls_perm.typ)
		{
		case INDIVIDUAL:
			ora_free(&tdua->value.dls_perm.v.individual.address);
			break;

		case MEMBER_OF_DL:
			ora_free(&tdua->value.dls_perm.v.member_of_dl.address);
			break;

		case PATTERN_MATCH:
			ora_free(&tdua->value.dls_perm.v.pattern_match.address);
			break;

		case MEMBER_OF_GROUP:
			break;

		default:
			break;
		}
		break;

	case D2_CERTIFICATE:
		d2a_260_certificate_free(&(tdua->value.certificate));
		break;

	case D2_CERT_PAIR:
		d2a_261_cert_pair_free(&(tdua->value.cert_pair));
		break;

	case D2_CERT_LIST:
		d2a_262_cert_list_free (&(tdua->value.cert_list));
		break;

	default:
		break;
	}
}

static void d_assertion_free (
  dua_assertion * tdua )
{
free (tdua->type.v);
dua_att_value_free (&(tdua->value));
}

static void d_rdname_free (
  dua_rdname * tdua )
{
signed32 i;

	if (tdua->nb_assertions && tdua->assertions)
	{
	   for (i=0; i<tdua->nb_assertions; i++)
		d_assertion_free (tdua->assertions + i);

	   free (tdua->assertions);

	}
}

void d_name_free (
  dua_name * tdua )
{
signed32 i;

	if (tdua->nb_rdnames && tdua->rdnames)
	{
		for (i=0; i<tdua->nb_rdnames; i++)
			d_rdname_free (tdua->rdnames + i);

	        free (tdua->rdnames);

	}
}

static void ora_free (
  ORAddress * ora )
{
	if (ora->s_attrs.c_name) free(ora->s_attrs.c_name);
	if (ora->s_attrs.a_d_name) free(ora->s_attrs.a_d_name);
	if (ora->s_attrs.p_d_name) free(ora->s_attrs.p_d_name);
	if (ora->s_attrs.p_name) free(ora->s_attrs.p_name);
	if (ora->s_attrs.o_u_names) free(ora->s_attrs.o_u_names);

	if (ora->d_d_attrs) free(ora->d_d_attrs);

	if (ora->e_attrs) free(ora->e_attrs);
}

void dua_name_free(
  dua_dist_name * dist_name)
{
signed32 i,j;

for (i=0; i<dist_name->nb_rdnames; i++)
  {
  for (j=0; j<(dist_name->rdnames + i)->nb_assertions; j++)
    {
    dua_att_value1_free(
	(dua_att_value1 *)&((dist_name->rdnames + i)->assertions + j)->value);
    free(((dist_name->rdnames + i)->assertions + j)->type.v);
    }
  if ((dist_name->rdnames + i)->nb_assertions > 0)
    {
    free((dist_name->rdnames + i)->assertions);
    }
  }

if (dist_name->nb_rdnames > 0)
  {
  free(dist_name->rdnames);
  }
}

