/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dua_dua.c,v $
 * Revision 1.1.12.2  1996/02/18  19:42:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:17  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:22:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:07  root]
 * 
 * Revision 1.1.10.3  1994/06/21  14:44:00  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:25:41  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:07:08  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:23  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  18:04:23  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:43:54  marrek]
 * 
 * Revision 1.1.8.1  1993/10/14  15:39:42  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  15:38:10  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  10:02:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:28:45  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  10:09:17  marrek
 * 	Introduce IA5_SYNTAX
 * 	[1993/02/01  17:47:28  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:11:26  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:08:20  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:01:56  marrek
 * 	November 1992 code drop
 * 	Cast to (dua_att_value1 *) in dua_att_value_dua ()
 * 	[1992/12/04  10:39:14  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  16:26:06  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  09:46:36  marrek]
 * 
 * Revision 1.1.2.5  1992/08/11  20:52:54  mhickey
 * 	Drop from Germany of a file that fixes conversion problems going to the
 * 	network.
 * 
 * 	Defect 4963.
 * 	[1992/08/11  20:52:33  mhickey]
 * 
 * Revision 1.1.2.4  1992/07/01  20:31:42  melman
 * 	Fixes CR 3975 client to client/server configs
 * 	[1992/07/01  20:27:23  melman]
 * 
 * Revision 1.1.2.3  1992/07/01  16:52:01  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:20:27  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  20:02:46  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:48:51  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dua_dua.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 19:42:29 $";
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
 *	. primitives "dua_typename_dua" to translate an item from
 *	  its "ids" representation into the equivalent "dua" representation.
 */

#include <gds.h>
#include <stdlib.h>
#include <string.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*       D E C L A R A T I O N of data				      */
/*                                                                    */
/**********************************************************************/

/******** LOCAL *********/

static signed32 dua_Eattrs_dua ( E_Attr * );
static signed32 dua_assertion_dua (ids_assertion * , dua_assertion * );
static signed32 d2a777_try_to_dec_as_basic_type(ids_att_value *,dua_att_value1 *);

/***********************************************************************
 * handling non error data types;
 * from ids representation to dua representation
***********************************************************************/
signed32 dua_bit_string_dua (
  ids_bit_string * tids,
  dua_bit_string * tdua )
{
const char function_name[] = "dua_bit_string_dua";
	if (tids->nb && tids->contents)
	{    if (((tids->nb + 7) >> 3) > ((tdua->nb + 7) >> 3))
	     {    if (tdua->nb == 0)
	       {    if ((tdua->contents =
			D27_MALLOC (svc_c_sev_warning,(tids->nb >> 3) +1)) == NULL)
			{ 
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
	       }
	       else
	       {    if ((tdua->contents = D27_REALLOC(svc_c_sev_warning,
				tdua->contents, (tids->nb >> 3) +1)) == NULL)
			{ 
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
	       }
	       tdua->nb = tids->nb;
	     }
	     ASN1_MOVE (tids->contents,
			tdua->contents,
			((tids->nb - 1) >> 3) +1);
	}
	else
	{	tdua->nb = 0;
		tdua->contents = 0;
	}
	return(D2_NOERROR);
}

signed32 dua_utc_time_dua (
  ids_utc_time * tids,
  dua_utc_time * tdua )
{
const char function_name[] = "dua_utc_time_dua";
	if (tids->nb == 0) 
	   { *tdua = 0; }
	else
	 {
	  if (!dua_utc_time_match(tids->contents))
		{
		D2_ASN1_TRACE_TAG_MISMATCH("UTC-Time")
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  if (dua_utc_time_dec (tids->contents, tids->contents+tids->nb,
				   tdua) == 0)
		{
		D2_ASN1_TRACE_DEC_ERROR("UTC-Time")
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  }
	return(D2_NOERROR);
}

static signed32 dua_Eattrs_dua (
  E_Attr * e_attr )
{
asn1_field a_val;
const char function_name[] = "dua_Eattrs_dua";

    a_val = e_attr->e_value.a_val;

    switch (e_attr->e_type) {
	case common_name :
	case pds_name :
		if (a_val.l)
		{
	  	  if (!dua_string_match(a_val.v))
			{
			D2_ASN1_TRACE_TAG_MISMATCH("Printable string")
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
		   if (dua_string_dec (a_val.v, a_val.v+a_val.l,
					&(e_attr->e_value.P_str)) == 0)
			{
			D2_ASN1_TRACE_DEC_ERROR("Printable string")
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
		}
		else
		 { e_attr->e_value.P_str = 0; }
		break;

	case teletex_common_name :
	case teletex_organization_name :
		if (a_val.l)
		{
	  	   if (!dua_Tstring_match(a_val.v))
			{
			D2_ASN1_TRACE_TAG_MISMATCH("T61 string")
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
		   if (dua_Tstring_dec (a_val.v, a_val.v+a_val.l,
					&(e_attr->e_value.T_str)) == 0)
			{
			D2_ASN1_TRACE_DEC_ERROR("T61 string")
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
		}
		else
		 { e_attr->e_value.T_str = 0; }
		break;

	case teletex_personal_name :
		if (!TeletexPersonalName_match(a_val.v))
		  {
		  D2_ASN1_TRACE_TAG_MISMATCH("TeletexPersonalName")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (TeletexPersonalName_dec (a_val.v, a_val.v+a_val.l,
					&(e_attr->e_value.pn_val)) == 0)
		  {
		  D2_ASN1_TRACE_DEC_ERROR("TeletexPersonalName")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case teletex_organizational_unit_names :
		if (!TeletexOrganizationUnitNames_match(a_val.v))
		  {
		  D2_ASN1_TRACE_TAG_MISMATCH("TeletexOrganizationUnitNames")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (TeletexOrganizationUnitNames_dec (a_val.v, a_val.v+a_val.l,
					&(e_attr->e_value.ou_val)) == 0)
		  {
		  D2_ASN1_TRACE_DEC_ERROR("TeletexOrganizationUnitNames")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case teletex_domain_defined_attributes :
		if (!TeletexDomainDefinedAttributes_match(a_val.v))
		  {
		  D2_ASN1_TRACE_TAG_MISMATCH("TeletexDomainDefinedAttributes")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (TeletexDomainDefinedAttributes_dec(a_val.v,a_val.v+a_val.l,
					&(e_attr->e_value.dd_val)) == 0)
		  {
		  D2_ASN1_TRACE_DEC_ERROR("TeletexDomainDefinedAttributes")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case physical_delivery_country_name :
	case postal_code :
		if (!str_NP_choice_match(a_val.v))
		  {
		  D2_ASN1_TRACE_TAG_MISMATCH("str_NP_choice")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (str_NP_choice_dec (a_val.v, a_val.v+a_val.l,
					&(e_attr->e_value.NP_val)) == 0)
		  {
		  D2_ASN1_TRACE_DEC_ERROR("str_NP_choice")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
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
		if (!PDSParameter_match(a_val.v))
		  {
		  D2_ASN1_TRACE_TAG_MISMATCH("PDSParameter")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (PDSParameter_dec (a_val.v, a_val.v+a_val.l,
					&(e_attr->e_value.pds_val)) == 0)
		  {
		  D2_ASN1_TRACE_DEC_ERROR("PDSParameter")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case unformatted_postal_address :
		if (!UnformattedPostalAddress_match(a_val.v))
		  {
		  D2_ASN1_TRACE_TAG_MISMATCH("UnformattedPostalAddress")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (UnformattedPostalAddress_dec (a_val.v, a_val.v+a_val.l,
					&(e_attr->e_value.up_addr)) == 0)
		  {
		  D2_ASN1_TRACE_DEC_ERROR("UnformattedPostalAddress")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case extended_network_address :
		if (!ExtendedNetworkAddress_match(a_val.v))
		  {
		  D2_ASN1_TRACE_TAG_MISMATCH("ExtendedNetworkAddress")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (ExtendedNetworkAddress_dec (a_val.v, a_val.v+a_val.l,
					&(e_attr->e_value.en_addr)) == 0)
		  {
		  D2_ASN1_TRACE_DEC_ERROR("ExtendedNetworkAddress")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case terminal_type :
		if (!TerminalType_match(a_val.v))
		  {
		  D2_ASN1_TRACE_TAG_MISMATCH("TerminalType")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (TerminalType_dec (a_val.v, a_val.v+a_val.l,
					&(e_attr->e_value.ter_val)) == 0)
		  {
		  D2_ASN1_TRACE_DEC_ERROR("TerminalType")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }

	}	

   return(D2_NOERROR);
 }

signed32 dua_dist_name_syntax_dua (
  ids_att_value * tids,
  dua_dist_name ** tdua )
{
ids_dist_name ids_name_buf;
const char function_name[] = "dua_dist_name_syntax_dua";

if (!ids_name_match(tids->v))
  {
  D2_ASN1_TRACE_TAG_MISMATCH("ids_name")
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
if (ids_name_dec (tids->v, tids->v+tids->l, &ids_name_buf) == 0)
  {
  D2_ASN1_TRACE_DEC_ERROR("ids_name")
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

if ((*tdua = D27_MALLOC (svc_c_sev_warning,sizeof (dua_dist_name))) == NULL)
		  { 
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }



if (dua_dist_name_dua (&ids_name_buf, *tdua) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

ids_dist_name_free (&ids_name_buf);

return(D2_NOERROR);
}

signed32 dua_att_value_dua (
  ids_att_value  * tids,
  dua_att_value1 * tdua,
  signed32 att_syntax )
{
signed32 i;
const char function_name[] = "dua_att_value_dua";

	switch (tdua->mode = att_syntax)
	{
	case D2_ERROR:
		if (d2a777_try_to_dec_as_basic_type(tids,tdua) != D2_NOERROR)
		    {
		    D2_ASN1_ERR_RETURN_WITH_TRACE
		    }
		break;
	case D2_OBJ_IDENT:
		if (!dua_oid_asn1_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("dua_oid_asn1")
		  if (d2a777_try_to_dec_as_basic_type(tids,
			tdua) != D2_NOERROR)
		    {
		    D2_ASN1_ERR_RETURN_WITH_TRACE
		    }
		  } /* of not match */
		else 
		  {
		  if (dua_oid_asn1_dec (tids->v, tids->v+tids->l,
		       &tdua->value.oid_value.contents.oid_asn1) == 0)
		    {
  		    D2_ASN1_TRACE_DEC_ERROR("dua_oid_asn1")
		    D2_ASN1_ERR_RETURN_WITH_TRACE
		    }
		  tdua->value.oid_value.is_oid_ints = FALSE;
		  } /* of match */

		break;

	case D2_NUMERIC:
		if (tids->l)
		   {
		   if (!dua_Nstring_match(tids->v))
		     {
  		     D2_ASN1_TRACE_TAG_MISMATCH("Numeric string")
		     if (d2a777_try_to_dec_as_basic_type(tids,
				 tdua) != D2_NOERROR)
		       {
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		     } /* of not match */
		   else
		     {
		     if (dua_Nstring_dec (tids->v, tids->v+tids->l,
					&(tdua->value.str_value)) == 0)
		       {
  		       D2_ASN1_TRACE_DEC_ERROR("Numeric string")
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		     } /* of match */
		   } /* of tids->l > 0 */
		else
		 { tdua->value.str_value = 0; }
		break;

	case D2_IA5:
		if (tids->l)
		   {
		   if (!dua_IA5string_match(tids->v))
		     {
  		     D2_ASN1_TRACE_TAG_MISMATCH("IA5 string")
		     if (d2a777_try_to_dec_as_basic_type(tids,
				 tdua) != D2_NOERROR)
		       {
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		     } /* of not match */
		   else
		     {
		     if (dua_IA5string_dec (tids->v, tids->v+tids->l,
					&(tdua->value.str_value)) == 0)
		       {
  		       D2_ASN1_TRACE_DEC_ERROR("IA5 string")
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		     } /* of match */
		   } /* of tids->l > 0 */
		else
		 { tdua->value.str_value = 0; }
		break;

	case D2_CEXACT:
	case D2_CIGNORE:
	case D2_T61:
		tdua->mode = D2_T61;
		if (tids->l)
		   {
		   if (!dua_Tstring_match(tids->v))
		     {
  		     D2_ASN1_TRACE_TAG_MISMATCH("T61 string")
		     if (d2a777_try_to_dec_as_basic_type(tids,
				tdua) != D2_NOERROR)
		       {
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		     } /* of not match */
		   else
		     {
		     if (dua_Tstring_dec (tids->v, tids->v+tids->l,
					&(tdua->value.str_value)) == 0)
		       {
  		       D2_ASN1_TRACE_DEC_ERROR("T61 string")
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		     } /* of match */
		   } /* of tids->l > 0 */
		else
		 { tdua->value.str_value = 0; }
		break;

	case D2_T61_PR:
		if (tids->l == 0)
		  {
		  tdua->mode = D2_T61;
		  tdua->value.str_value = 0;
		  }
		else
		  {
		  if (dua_string_match(tids->v))
		    {
		    tdua->mode = D2_PRINTABLE;
		    if (dua_string_dec (tids->v, tids->v+tids->l,
					&(tdua->value.str_value)) == 0)
		       {
  		       D2_ASN1_TRACE_DEC_ERROR("Printable string")
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		    break;
		    }

		  if (dua_Tstring_match(tids->v))
		    {
		    tdua->mode = D2_T61;
		    if (dua_Tstring_dec (tids->v, tids->v+tids->l,
					&(tdua->value.str_value)) == 0)
		       {
  		       D2_ASN1_TRACE_DEC_ERROR("T61 string")
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		    break;
		    }

  		  D2_ASN1_TRACE_TAG_MISMATCH("T61/Printable string")
		  if (d2a777_try_to_dec_as_basic_type(tids,
				tdua) != D2_NOERROR)
		       {
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		  
		  } /* of tids->l > 0 */
		break;

	case D2_PRINTABLE:
	case D2_TELNR_STX:
	case D2_COUNTRY_STX:
		tdua->mode = D2_PRINTABLE;
		if (tids->l)
		   {
		   if (!dua_string_match(tids->v))
		     {
  		     D2_ASN1_TRACE_TAG_MISMATCH("Printable string")
		     if (d2a777_try_to_dec_as_basic_type(tids,
				tdua) != D2_NOERROR)
		       {
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		     } /* of not match */
		   else
		     {
		     if (dua_string_dec (tids->v, tids->v+tids->l,
					&(tdua->value.str_value)) == 0)
		       {
  		       D2_ASN1_TRACE_TAG_MISMATCH("Printable string")
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		     } /* of match */
		   } /* of tids->l > 0 */
		else
		 { tdua->value.str_value = 0; }
		break;

	case D2_BOOLEAN:
		if (!dua_boolean_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("boolean")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (dua_boolean_dec (tids->v, tids->v+tids->l,
				     &(tdua->value.int_value)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("boolean")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case D2_INTEGER:
		if (!dua_int_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("integer")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (dua_int_dec (tids->v, tids->v+tids->l,
			     &(tdua->value.int_value)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("integer")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

        case D2_ACCL:
	case D2_PASSWD_STX:
	case D2_OCTET:
		if (tdua->mode == D2_PASSWD_STX)
				tdua->mode = D2_OCTET;
		if (tids->l)
		 {
		    if (!dua_octet_string_match(tids->v))
		      {
  		      D2_ASN1_TRACE_TAG_MISMATCH("octet string")
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		      }
		    if (dua_octet_string_dec (tids->v, tids->v+tids->l,
					      &(tdua->value.oct_value)) == 0)
		  	{
  			D2_ASN1_TRACE_DEC_ERROR("octet string")
		  	D2_ASN1_ERR_RETURN_WITH_TRACE
		  	}
		  }
		else
		{	tdua->value.oct_value.nb = 0;
			tdua->value.oct_value.contents = 0;
		}
		break;

	case D2_TIME:
		if (tids->l)
		  {
		     if (!dua_utc_time_match(tids->v))
		       {
  		       D2_ASN1_TRACE_TAG_MISMATCH("UTC time")
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		     if (dua_utc_time_dec(tids->v, tids->v+tids->l,
				 &(tdua->value.utc_time_value)) == 0)
		  	{
  			D2_ASN1_TRACE_DEC_ERROR("UTC time")
		  	D2_ASN1_ERR_RETURN_WITH_TRACE
		  	}
		   }
		else 
		   { tdua->value.utc_time_value = 0; }
		break;

	case D2_PSAP_STX:
		if (!dua_psap_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("PSAP")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (dua_psap_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.psap_value)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("PSAP")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case D2_GUIDE:
		if (!guide_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("Guide")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (guide_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.guide_value)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("Guide")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;
	case D2_TLXNR_STX:
		if (!tlx_number_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("Telex number")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (tlx_number_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.tlx_value)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("Telex number")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;
	case D2_TTXID_STX:
		if (!teletex_id_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("TTXID")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (teletex_id_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.ttxid_value)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("TTXID")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;
	case D2_FAXNR_STX:
		if (!faxNumber_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("FAXNR")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (faxNumber_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.fax_value)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("FAXNR")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case D2_MHS_PR_DM_STX:
	case D2_PR_DM_STX:
	case D2_INT_LIST:
		tdua->mode = D2_INT_LIST;
		if (!int_seq_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("integer list")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (int_seq_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.int_seq_value)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("integer list")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case D2_T61_LIST:
		tdua->mode = D2_T61_LIST;
		if (!t61_seq_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("T61 sequence")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (t61_seq_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.t61_seq_value)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("T61 sequence")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case D2_POST_AD_STX:
	case D2_T61_PR_LIST:
		tdua->mode = D2_T61_PR_LIST;
		if (!t61_pr_seq_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("T61/Prinatble sequence")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (t61_pr_seq_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.post_value)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("T61/Prinatble sequence")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case D2_PRT_LIST:
	case D2_CIGN_LIST:	/*? no		*/
		tdua->mode = D2_PRT_LIST;
		if (!print_seq_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("Printable sequence")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (print_seq_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.pr_seq_value)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("Printable sequence")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case D2_MHS_ORADR_STX:
		if (!ORAddress_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("ORAddress")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (ORAddress_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.or_address)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("ORAddress")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		/* In case extension attributes are present,
		   decode them first */
		if (tdua->value.or_address.e_attrs != (E_Attrs *)NULL)
		 {
		   for (i=0; i < tdua->value.or_address.e_attrs->n; i++)
		     {
			if (dua_Eattrs_dua(tdua->value.or_address.e_attrs->v + i) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			}
		  }
		break;

	case D2_MHS_ORNAME_STX:
		if (!ORName_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("ORName")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (ORName_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.or_name)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("ORName")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (tdua->value.or_name.address.e_attrs != (E_Attrs *)NULL)
		 {
		   for (i=0; i < tdua->value.or_name.address.e_attrs->n; i++)
		     {
			if (dua_Eattrs_dua(tdua->value.or_name.address.e_attrs->v + i) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			}
		  }
		if (tdua->value.or_name.d_name.a_val.l == 0)
		 {
		  tdua->value.or_name.d_name.name = (dua_dist_name *)NULL;
		  }
		 else
		 {
		  asn1_field a_val ;
		  a_val = tdua->value.or_name.d_name.a_val;
		  dua_dist_name_syntax_dua(&a_val,
					&tdua->value.or_name.d_name.name);
		  }
		break;

	case D2_MHS_DLSP_STX:
		if (!DLSubmitPermission_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("DLSubmitPermission")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (DLSubmitPermission_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.dls_perm)) == 0)
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("DLSubmitPermission")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (tdua->value.dls_perm.typ != MEMBER_OF_GROUP )
		 {
		  if (tdua->value.dls_perm.v.individual.address.e_attrs != (E_Attrs *)NULL)
		  {
		   for (i=0; i < tdua->value.dls_perm.v.individual.address.e_attrs->n; i++)
		     {
			if (dua_Eattrs_dua(tdua->value.dls_perm.v.individual.address.e_attrs->v + i) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
		      }
		   }
		   if (tdua->value.dls_perm.v.individual.d_name.a_val.l != 0)
		     {
		     asn1_field a_val;
		     a_val = tdua->value.dls_perm.v.individual.d_name.a_val;
		     dua_dist_name_syntax_dua(&a_val,
		&tdua->value.dls_perm.v.individual.d_name.name);
		     }
		   else
		     {
	  tdua->value.dls_perm.v.individual.d_name.name = (dua_dist_name *)NULL;
		     }
		  }
		 else
		  {
		   /* length:
		        tdua->value.dls_perm.v.member_of_group.a_val.l == 0
			can not be 0.
		    */
		   asn1_field a_val ;
		   a_val = tdua->value.dls_perm.v.member_of_group.a_val;
		   dua_dist_name_syntax_dua(&a_val,
				&tdua->value.dls_perm.v.member_of_group.name);
		   }
		  break;

	case D2_ASN1:
		/* In the case the att_syntax is different from any of the
		 * above specified cases, the value field of the decoded
		 * representation will convey the ASN1 representation as
		 * received from the IDS representation.
		 */
		  if (tdua->value.asn1_value.l = tids->l)
		    {
		    tdua->value.asn1_value  = *tids;
		    }
		  else 
		    {
		    tdua->value.asn1_value.v = 0;
		    }
		  break;

	case D2_CERTIFICATE:
		if (!duaCertificate_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("Certificate")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (duaCertificate_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.certificate)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("Certificate")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case D2_CERT_PAIR:
		if (!duaCertPair_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("Certificate pair")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (duaCertPair_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.cert_pair)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("Certificate pair")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	case D2_CERT_LIST:
		if (!duaCertList_match(tids->v))
		  {
  		  D2_ASN1_TRACE_TAG_MISMATCH("Certificate list")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		if (duaCertList_dec (tids->v, tids->v+tids->l,
			      &(tdua->value.cert_list)) == 0)
		  {
  		  D2_ASN1_TRACE_DEC_ERROR("Certificate list")
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		break;

	default:
	      D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    return(D2_NOERROR);
}

static signed32 dua_assertion_dua (
  ids_assertion * tids,
  dua_assertion * tdua )
{
const char function_name[] = "dua_assertion_dua";
signed32 att_syntax;

tdua->type.l = tids->type.l;
if ((tdua->type.v = D27_MALLOC(svc_c_sev_warning,tids->type.l)) == NULL)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
strncpy((char *)tdua->type.v,(char *)tids->type.v,(size_t)tids->type.l);
att_syntax = recover_att_syntax (&tdua->type);
if (dua_att_value_dua (&(tids->value), (dua_att_value1 *)&(tdua->value),
					att_syntax) 	== D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
if ( (tdua->value.mode != D2_T61) && (tdua->value.mode != D2_PRINTABLE) &&
	   (tdua->value.mode != D2_NUMERIC) && (tdua->value.mode != D2_IA5) )
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
return(D2_NOERROR);
}

signed32 dua_rdname_dua (
  ids_rdname * tids,
  dua_rdname * tdua )
{
const char function_name[] = "dua_rdname_dua";
signed32 i;
	ids_assertion * ids_ptr = tids->assertions;

	tdua->nb_assertions = tids->nb_assertions;
	if (tdua->nb_assertions > 0)
	  {
	  if ((tdua->assertions =
	    D27_MALLOC(svc_c_sev_warning,tids->nb_assertions * sizeof(dua_assertion))) == NULL)
		{ 
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  }
	if (tids->nb_assertions && ids_ptr)
	{  for (i=0; i<tids->nb_assertions; i++)
	   {
		if (dua_assertion_dua (ids_ptr + i,
				   tdua->assertions + i) == D2_ERROR)
			{ 
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
	   }
	}
	else
	{    /*   tdua->assertions = 0; */
		tdua->nb_assertions = 0;
	}
     return(D2_NOERROR);
}

signed32 dua_dist_name_dua (
  ids_dist_name * tids,
  dua_dist_name * tdua )
{
const char function_name[] = "dua_dist_name_dua";
signed32 i;

	tdua->nb_rdnames = tids->nb_rdnames;
	if ((tdua->rdnames =
	  D27_MALLOC(svc_c_sev_warning,(tids->nb_rdnames+1) * sizeof(dua_rdname))) == NULL)
		{ 
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	if (tids->nb_rdnames && tids->rdnames)
	{      for (i=0; i<tids->nb_rdnames; i++)
		{
			if (dua_rdname_dua (tids->rdnames + i,
					tdua->rdnames + i) == D2_ERROR)
				{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
		}
	}
	else
	{       tdua->nb_rdnames = 0;
		tdua->rdnames = 0;
	}
     return(D2_NOERROR);
}


static signed32 d2a777_try_to_dec_as_basic_type(
  ids_att_value  * tids,
  dua_att_value1 * tdua )
{
const char function_name[] = "d2a777_try_to_dec_as_basic_type";

if (dua_boolean_match(tids->v))
  {
  if (dua_boolean_dec (tids->v, tids->v+tids->l,
			     &(tdua->value.int_value)) == 0)
	  {
  	  D2_ASN1_TRACE_DEC_ERROR("boolean")
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
  tdua->mode = D2_BOOLEAN;
  return (D2_NOERROR);
  }

if (dua_int_match(tids->v))
  {
  if (dua_int_dec (tids->v, tids->v+tids->l,
			     &(tdua->value.int_value)) == 0)
	  {
  	  D2_ASN1_TRACE_DEC_ERROR("integer")
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
  tdua->mode = D2_INTEGER;
  return (D2_NOERROR);
  }

if (dua_Nstring_match(tids->v))
  {
  if (dua_Nstring_dec (tids->v, tids->v+tids->l,
				&(tdua->value.str_value)) == 0)
	  {
  	  D2_ASN1_TRACE_DEC_ERROR("Numeric string")
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
  tdua->mode = D2_NUMERIC;
  return (D2_NOERROR);
  }

if (dua_IA5string_match(tids->v))
  {
  if (dua_IA5string_dec (tids->v, tids->v+tids->l,
				&(tdua->value.str_value)) == 0)
	  {
  	  D2_ASN1_TRACE_DEC_ERROR("IA5 string")
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
  tdua->mode = D2_IA5;
  return (D2_NOERROR);
  }

if (dua_Tstring_match(tids->v))
  {
  if (dua_Tstring_dec (tids->v, tids->v+tids->l,
				&(tdua->value.str_value)) == 0)
	  {
  	  D2_ASN1_TRACE_DEC_ERROR("T61 string")
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
  tdua->mode = D2_T61;
  return (D2_NOERROR);
  }

if (dua_string_match(tids->v))
  {
  if (dua_string_dec (tids->v, tids->v+tids->l,
				&(tdua->value.str_value)) == 0)
	  {
  	  D2_ASN1_TRACE_DEC_ERROR("Printable string")
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
  tdua->mode = D2_PRINTABLE;
  return (D2_NOERROR);
  }

if (dua_octet_string_match(tids->v))
  {
  if (dua_octet_string_dec (tids->v, tids->v+tids->l,
				&(tdua->value.oct_value)) == 0)
	  {
  	  D2_ASN1_TRACE_DEC_ERROR("octet string")
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
  tdua->mode = D2_OCTET;
  return (D2_NOERROR);
  }

if (dua_oid_asn1_match(tids->v))
  {
  if (dua_oid_asn1_dec (tids->v, tids->v+tids->l,
		       &tdua->value.oid_value.contents.oid_asn1) == 0)
	{
  	D2_ASN1_TRACE_DEC_ERROR("object identifier")
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
  tdua->value.oid_value.is_oid_ints = FALSE;
  tdua->mode = D2_OBJ_IDENT;
  return (D2_NOERROR);
  }

D2_ASN1_ERR_RETURN_WITH_TRACE
}
