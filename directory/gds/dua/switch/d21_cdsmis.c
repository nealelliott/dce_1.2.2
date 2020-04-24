/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_cdsmis.c,v $
 * Revision 1.1.847.2  1996/02/18  18:18:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:00  marty]
 *
 * Revision 1.1.847.1  1995/12/08  15:55:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:07  root]
 * 
 * Revision 1.1.845.4  1994/08/17  13:56:45  keutel
 * 	OT 11222: adding CDS class and CDS class version via XDS
 * 	[1994/08/17  12:49:42  keutel]
 * 
 * Revision 1.1.845.3  1994/06/21  14:46:47  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:39:30  marrek]
 * 
 * Revision 1.1.845.2  1994/05/10  15:54:24  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:34:00  marrek]
 * 
 * Revision 1.1.845.1  1994/02/22  18:06:47  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:38:23  marrek]
 * 
 * Revision 1.1.843.2  1993/10/14  17:27:34  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:26:01  keutel]
 * 
 * Revision 1.1.843.1  1993/10/13  17:32:13  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:34:52  keutel]
 * 
 * Revision 1.1.7.3  1993/08/19  11:22:40  marrek
 * 	+1 and -1 removed in length calculation when
 * 	x500 val to cdsval and vice-versa.
 * 	[1993/08/19  11:21:40  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  06:50:08  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:02:43  marrek]
 * 
 * Revision 1.1.5.7  1993/02/04  13:27:42  marrek
 * 	Fixes for 6597.
 * 	[1993/02/04  13:23:49  marrek]
 * 
 * Revision 1.1.5.6  1993/02/02  15:39:15  marrek
 * 	Fixes for OT 6597
 * 	[1993/02/02  11:07:29  marrek]
 * 
 * Revision 1.1.5.5  1993/02/02  10:17:47  marrek
 * 	Fixes for OT 6554
 * 	insert <pthread.h> and <D00_switch_protect.h>
 * 	[1993/02/02  09:31:15  marrek]
 * 
 * Revision 1.1.5.4  1992/12/31  19:49:25  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:56  bbelch]
 * 
 * Revision 1.1.5.3  1992/11/16  08:58:28  marrek
 * 	Remove empty #ifndef lint and #ifdef ANSIC.
 * 	Remove macro STRINGCAST(x) and use explicite cast to (char *).
 * 	[1992/11/06  11:37:57  marrek]
 * 
 * Revision 1.1.5.2  1992/09/29  21:10:41  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:24:32  treff]
 * 
 * Revision 1.1.2.8  1992/07/06  19:40:01  melman
 * 	Fixes for switch failures CR 4610, 4582
 * 	[1992/07/06  19:38:33  melman]
 * 
 * Revision 1.1.2.7  1992/07/01  16:53:18  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:22:15  melman]
 * 
 * Revision 1.1.2.6  1992/06/30  20:59:18  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:39:57  melman]
 * 
 * Revision 1.1.2.5  1992/06/29  22:23:44  melman
 * 	Bug 4344: Added #include <string.h>
 * 	Bug 4344: In case dns_Timestamp in d21_cdsval2x500val() change args to
 * 	strlen as follows:
 * 	    Was:	len = strlen (dns_value->val_u.val_timestamp);
 * 	    Changed to:	len = strlen (dns_value->val_u.val_timestamp.dns_cts);
 * 	Bug 4344: Also in case dns_Timestamp in d21_cdsval2x500val() change args to
 * 	strcpy as follows:
 * 	    Was:	strcpy (oct_value, dns_value->val_u.val_timestamp);
 * 	    Change to:	strcpy (oct_value, dns_value->val_u.val_timestamp.dns_cts);
 * 	Bug 4344:  In both changes above changed the ".dns_cts" (which did not
 * 	compile with the error "structure has no member named `cds_cts_t'") to
 * 	".uc" (which did compile).
 * 	[1992/06/24  17:22:03  melman]
 * 
 * Revision 1.1.2.4  1992/06/02  03:38:09  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:39:18  zeliff]
 * 
 * Revision 1.1.2.3  1992/03/22  22:43:35  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:38:49  weisman]
 * 
 * Revision 1.1.2.2  1992/01/22  23:03:18  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:19:01  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_cdsmis.c,v $ $Revision: 1.1.847.2 $ $Date: 1996/02/18 18:18:04 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/


/*---------------------------------------------------------------------

	Filename:		d21_cdsmis.c

	Author:			P. Dunne
	Date:			1.02.1991

	Component:		OSF DCE Naming Switch

	Description:	Switch functions that are of general use in 
					handling naming operations.
	Functions:		
		exported:	d21_cds_set_flags(),	d21_cdsval2x500val(),
					d21_x500val2cdsval(),	d21_cds_unwilling().
					d21_cds_compress_oid(),	d21_cds_uncompress_oid(),
					d21_cds_dots2hypens(),	d21_cds_hypens2dots(),
					d21_cds_map_error(),	d21_cds_error_info().
					d21_cds_attr_error(),	d21_cds_check_for_alias(),
					d21_x500name2cdsname(). d21_cdsname2x500name().
		            d21_cds_expand_oids (	

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
#include <dce/sec_login.h>
#include <d21_cdssw.h>

/*--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------

	d21_cds_set_flags ():

	Maps the common arguments parameters in IAPL* to the DNS flags
	structure.

 -----------------------------------------------------------------------*/

int	d21_cds_set_flags (	
		struct dnsFlagStat	*flags,
		D2_c_arg			*common_args)
{
	long			services;
	const char function_name[] = "d21_cds_set_flags";
	
        static int unauth = -1;
        error_status_t sec_status;
        sec_login_handle_t login_context_h;
        int return_value = D2_NOERROR;

	(void) memset ((char *) flags, 0, sizeof (struct dnsFlagStat));

	flags->fsConf = dnsConfLow;
	flags->fsNameType = dnsStrDCE;

	services = common_args->d2_serv_cntrls;

	if (services & D2_NOCACHE)
		flags->fsConf = dnsConfHi;

BEGIN_unauth_LOCK
        if (unauth < 0) {
	/* Get current context and save it */
	     sec_login_get_current_context(&login_context_h, &sec_status);
	       if (return_value != D2_ERROR) {
		    unauth = (sec_status != error_status_ok);
		} /* end if */
        } /* end if */
    if (return_value != D2_ERROR) {
        flags->fsUnAuth = flags->fsTrustAll = unauth ;
    } /* end if */
 
END_unauth_LOCK

    return (return_value);

} /* end d21_cds_set_flags */
 



/*-----------------------------------------------------------------------

	d21_cdsval2x500val():

	Assigns a value from a descriminating union of cns attribute values
	to a descriminating union of gns naming attribute values.

 -----------------------------------------------------------------------*/

int		d21_cdsval2x500val (	
			D2_a_value			*value_union,
			dns_attrvalue_t		*dns_value)
{
	char	*str_value;
	u_char	*oct_value;
	int		len,	r_code;
	char * version_val;

	r_code = D2_NOERROR;

	if (dns_value->val_syntax == dns_char) {

		len = dns_value->val_u.val_char.length;
		if ((str_value = D27_MALLOC(svc_c_sev_warning,len)) == NULL) {
			return (D2_ERROR);
		}

    } else if (dns_value->val_syntax == dns_byte) {

		len = dns_value->val_u.val_byte.length;
		if ((oct_value = D27_MALLOC(svc_c_sev_warning,len)) == NULL) {
			return (D2_ERROR);
		}
    }

	if (dns_value->val_syntax == dns_Version)
	  {
	  len = 4;
	  if ((version_val = D27_MALLOC(svc_c_sev_warning,len)) == NULL)
	    {
	    return (D2_ERROR);
	    }
	  }

	switch (dns_value->val_syntax) {
		case dns_uuid:
			value_union->d2_a_rep = D2_OCTET;
			len = strlen ((char *)dns_value->val_u.val_uuid.node);
			if ((str_value = D27_MALLOC(svc_c_sev_warning,len))
							== NULL) {
				return (D2_ERROR);
			}
			strcpy (str_value,(char *)dns_value->val_u.val_uuid.node);
			value_union->d2_at_value.d2_oct_a_v =  (u_char *) str_value;
			value_union->d2_a_v_len = len;
			break;

	  	case dns_Timestamp:
			value_union->d2_a_rep = D2_OCTET;
			len = CDS_CTS_LEN_MAX;
			if ((oct_value = D27_MALLOC(svc_c_sev_warning,len))
							== NULL) {
                        	return (D2_ERROR);
			}
			value_union->d2_a_v_len = len;
			if (len != 0) {
				memcpy ((char *)oct_value,(char *)dns_value->val_u.val_timestamp.uc,len);
				value_union->d2_at_value.d2_oct_a_v = oct_value;
			}
			break;

		case dns_byte:

			value_union->d2_a_rep = D2_OCTET;
			value_union->d2_a_v_len = len;
			strncpy ((char *)oct_value, dns_value->val_u.val_byte.byte_p, len);
			value_union->d2_at_value.d2_oct_a_v = oct_value;
			break;
		
		case dns_char:

			value_union->d2_a_rep = D2_T61;
			len = dns_value->val_u.val_char.length;
			value_union->d2_a_v_len = len; 
			strncpy (str_value, dns_value->val_u.val_char.char_p, len);
			value_union->d2_at_value.d2_t61_a_v = (u_char *) str_value;
			break;

		case dns_Version:

			value_union->d2_a_rep = D2_PRINTABLE;
			version_val[0] = dns_value->val_u.val_version.dnsMajVer + '0';
			version_val[1] = '.';
			version_val[2] = dns_value->val_u.val_version.dnsMinVer + '0';
			version_val[3] = '\0';
			value_union->d2_at_value.d2_prt_a_v = version_val;
			value_union->d2_a_v_len = len;
			break;

		case dns_long:

			value_union->d2_a_rep = D2_INTEGER;
			value_union->d2_at_value.d2_int_a_v = dns_value->val_u.val_long;
			break;
		default:

			r_code = D2_ERROR;
			break;

	}

	return (r_code);
}



/*-----------------------------------------------------------------------

	d21_x500val2cdsval():

	Assigns a value from a descriminating union of x.500 attribute values
	to a descriminating union of cns naming attribute values.

  -----------------------------------------------------------------------*/

int		d21_x500val2cdsval (	
			D2_a_value			*attr_value,
			dns_attrvalue_t		*dns_value)
{
	u_char	cds_dn [D2_DNL_MAX],	*oct_value;
	char	*str_value;
	int		int_value,	r_code;
	const char function_name[] = "d21_x500val2cdsval";

	r_code = D2_NOERROR;

	switch (attr_value->d2_a_rep) {

	    case D2_T61:
			/*
				assign the x.500 attribute value to the dns attribute value
																			*/
			dns_value->val_syntax = dns_char;
			dns_value->val_u.val_char.length = attr_value->d2_a_v_len;
	     	str_value = (char *) attr_value->d2_at_value.d2_t61_a_v;
			dns_value->val_u.val_char.char_p =  str_value;
	     	break;

	    case D2_OBJ_IDENT: 
			/*
				assign the x.500 attribute value to the dns attribute value
																			*/
			dns_value->val_syntax = dns_byte;
			dns_value->val_u.val_byte.length = attr_value->d2_a_v_len;
			oct_value = attr_value->d2_at_value.d2_obid_a_v;
			dns_value->val_u.val_byte.byte_p = (void *) oct_value;
			break;

	    case D2_OCTET:
			/*
				assign the x.500 attribute value to the dns attribute value
																			*/
			dns_value->val_syntax = dns_byte;
			dns_value->val_u.val_byte.length = attr_value->d2_a_v_len;
	     	oct_value = attr_value->d2_at_value.d2_oct_a_v;
			dns_value->val_u.val_byte.byte_p = oct_value;
			break;

	    case D2_PRINTABLE:
			/*
				assign the x.500 attribute value to the dns attribute value
																			*/
			dns_value->val_syntax = dns_char;
			dns_value->val_u.val_char.length = attr_value->d2_a_v_len;
			str_value = attr_value->d2_at_value.d2_prt_a_v;
			dns_value->val_u.val_char.char_p =  str_value;
			break;

	    case D2_DISTNAME:
			/*
				assign the x.500 attribute value to the dns attribute value
																			*/
			dns_value->val_syntax = dns_char;
			str_value = (char *) attr_value->d2_at_value.d2_obj_name_a_v;

			if (d21_x500name2cdsname ((u_char *) str_value,  cds_dn) 
				== D2_ERROR) {
				DUA_TRACE_X500_TO_CDS_NAME_FAILED
				r_code = D2_ERROR;
			}

			dns_value->val_u.val_char.char_p =  (char *) cds_dn;
			dns_value->val_u.val_char.length = strlen ((char *)cds_dn);
			break;

	    case D2_NUMERIC:
			/*
				assign the x.500 attribute value to the dns attribute value
																			*/
			dns_value->val_syntax = dns_char;
			dns_value->val_u.val_char.length = attr_value->d2_a_v_len;
			str_value = attr_value->d2_at_value.d2_num_a_v;
			dns_value->val_u.val_char.char_p =  str_value;
			break;

	    case D2_BOOLEAN:
			/*
				assign the x.500 attribute value to the dns attribute value
																			*/
			dns_value->val_syntax = dns_long;
	     	int_value = (int) attr_value->d2_at_value.d2_bool_a_v;
			dns_value->val_u.val_long = (long) int_value;
			break;

	    case D2_INTEGER:
			/*
				assign the x.500 attribute value to the dns attribute value
																			*/
			dns_value->val_syntax = dns_long;
	     	int_value = attr_value->d2_at_value.d2_int_a_v;
			dns_value->val_u.val_long = (long) int_value;
			break;

	    case D2_TIME:
			/*
				assign the x.500 attribute value to the dns attribute value
																			*/
			dns_value->val_syntax = dns_char;
			dns_value->val_u.val_char.length = attr_value->d2_a_v_len;
	     	str_value = attr_value->d2_at_value.d2_utc_a_v;
			dns_value->val_u.val_char.char_p =  str_value;
			break;

	    case D2_PSAP_STX:
			/*
				assign the x.500 attribute value to the dns attribute value
																			*/
			dns_value->val_syntax = dns_byte;
			dns_value->val_u.val_byte.length = attr_value->d2_a_v_len;
	     	oct_value = attr_value->d2_at_value.d2_oct_a_v;
			dns_value->val_u.val_byte.byte_p = oct_value;
			break;

	    case D2_ASN1:
			/*
				assign the x.500 attribute value to the dns attribute value
																			*/
			dns_value->val_syntax = dns_byte;
			dns_value->val_u.val_byte.length = attr_value->d2_a_v_len;
	     	oct_value = attr_value->d2_at_value.d2_oct_a_v;
			dns_value->val_u.val_byte.byte_p = oct_value;
			break;

	    default:
			r_code = D2_ERROR;
			break;
	}

	return (r_code);
}


/*-----------------------------------------------------------------------

	d21_cds_unwilling ():

	Returns the X.500 service error unwillingToPerform.

  -----------------------------------------------------------------------*/

void	d21_cds_unwilling (
			D2_pbhead	*naming_error)
{
	/* assume that naming_error->d2_version is already set */
	naming_error->d2_retcode = D2_TEMP_ERR;
	naming_error->d2_errclass = D2_REM_ERR;
	naming_error->d2_errvalue = D2_UNWILLING;;
}


/*-----------------------------------------------------------------------

	d21_x500name2cdsname ():

	Converts a name in IAPL* format to CDS format. In IAPL*, attribute 
	names and types are seperated by the <CTRL C> character,
        attribute value asserions are seperated by the <CTRL B>
	character, and the relative distinguished names are seperated by
	the <CTRL A> character.
	 In CDS, names are not typed and are seperated by the "/" character,
	attribute value asserions are seperated by ',' .

 -----------------------------------------------------------------------*/

int	d21_x500name2cdsname (	
				u_char	*iapl_name,	
				u_char	*cds_name)
{
	int		oid_size,	first,	second,	new;
	Bool	rdn_loop;

	oid_size = strlen (DSW_TYPELESS_OID);
	strcpy ((char *)cds_name, DSW_CDS_GN_PREFIX);
	cds_name += strlen (DSW_CDS_GN_PREFIX);

	while (*iapl_name != D2_EOS) {

	    *cds_name = '/';
	    cds_name ++;
	    if (*iapl_name != D2_RDN_SEP) rdn_loop = TRUE;

	    while (rdn_loop) {

		/*	
			Map the type part of the RDN to CDS format.
														*/
		if (strncmp ((char *)iapl_name, DSW_TYPELESS_OID, oid_size) == 0) {

			iapl_name += oid_size + 1;

		} else {

			/*  uncompress the first part of the OID as defined
				by the ASN.1 Basic Encoding Rules                   */
			first = *iapl_name - '0';
			iapl_name ++;
			second  = *iapl_name - '0';
			iapl_name ++;
			new = (first * 10) + second;
			second = new % 40;
			first = (new - second) / 40;


			/*  pdunne:     27/03/91
				using a basic algorithm here - it may have to be upgraded  */
			*cds_name = '0' + first;
			cds_name ++;
			*cds_name = '.';
			cds_name ++;
			*cds_name = '0' + second;
			cds_name ++;

			while ( (*iapl_name != D2_T61_REP) &&
				(*iapl_name != D2_PRTBL_REP) &&
				(*iapl_name != D2_IA5_REP) &&
				(*iapl_name != D2_NUM_REP) ) {

				*cds_name = *iapl_name;
				cds_name ++;
				iapl_name ++;
			}

			*cds_name = '=';
			cds_name ++;
			iapl_name ++;
		}

		/*
			Map the value part of the RDN to CDS format.
														*/
		while (*iapl_name != D2_RDN_SEP && *iapl_name != D2_AVA_SEP && *iapl_name != D2_EOS) {

			*cds_name = *iapl_name;
			cds_name ++;
			iapl_name ++;
		}

		if (*iapl_name == D2_AVA_SEP) {
		    *cds_name = ',';
		    cds_name++;
		    iapl_name ++;
		}
		else {
		     rdn_loop = FALSE;
		     if (*iapl_name == D2_EOS) {

			*cds_name = '\0';
		
		     } else {

			iapl_name ++;
		     }
		}
	     } /* end of rdn loop */
	}

	return (D2_NOERROR);
}


/*-----------------------------------------------------------------------

	d21_cdsname2x500name ():

	Converts a name in IAPL* format to CDS format. In IAPL*, attribute 
	names and types are seperated by the <CTRL C> character,
        attribute value asserions are seperated by the <CTRL B>
	character, and the relative distinguished names are seperated by
	the <CTRL A> character.
	 In CDS, names are not typed and are seperated by the "/" character,
	attribute value asserions are seperated by ',' .

 -----------------------------------------------------------------------*/

int	d21_cdsname2x500name (	
			u_char	*cds_name,
			u_char	*iapl_name)
{
	u_char	*dummy;
	int		TL_OID_FOUND,	oid_size,    first,    second,	
			new,			r_code;
	Bool rdn_loop;
	char * oid;

	TL_OID_FOUND = FALSE;
	r_code = D2_NOERROR;
	oid_size = strlen (DSW_TYPELESS_OID);
	cds_name += strlen (DSW_CDS_GN_PREFIX);
	cds_name ++;

	while (*cds_name != '\0') {

	    if (*cds_name != D2_RDN_SEP) rdn_loop = TRUE;
	    while (rdn_loop) {

		/*
			Check for a CDS untyped RDN.
										*/ 
		dummy = cds_name;

		while (*dummy != '/' && *dummy != '=' && *dummy != ',' && *dummy != '\0') {

			dummy ++;
		}

		if (*dummy != '=') {

			TL_OID_FOUND = TRUE;
		}

		/*
			Map the type part of the RDN.
											*/
		if (TL_OID_FOUND) {

			strncpy ((char *)iapl_name, DSW_TYPELESS_OID, oid_size);
			iapl_name += oid_size;
			*iapl_name = D2_T61_REP;
			iapl_name ++;

		} else {

 			/*	compress the first two parts of the OID	as defined 
 				by the ASN.1 Basic Encoding Rules					*/
 			first = *cds_name - '0';
 			cds_name += 2;
 			second  = *cds_name - '0';
 			new = (first * 40) + second;
 			cds_name ++;
 
 			/*	pdunne:		27/03/91
 				using a very basic algorithm here - it may have to 
 				be upgraded											*/
			oid = (char *) iapl_name;
 			*iapl_name = '0' + (new / 10);
 			iapl_name ++;
 			*iapl_name = '0' + (new % 10);
 			iapl_name ++;
 			
			while (*cds_name != '=') {
				*iapl_name = *cds_name;
				iapl_name ++;
				cds_name ++;
			}

			if (strncmp (oid, DSW_COUNTRY_OID,
				sizeof(DSW_COUNTRY_OID)-1 ) == 0)
			  {
			  *iapl_name =  D2_PRTBL_REP;
			  }
			else
			  {
			  *iapl_name =  D2_T61_REP;
			  }
			iapl_name ++;
			cds_name ++;
		}

		/* 
			Map the value part of the RDN.
											*/
		while (*cds_name != '/' && *cds_name != ',' && *cds_name != '\0') {

			*iapl_name = *cds_name;
			iapl_name ++;
			cds_name ++;
		}

		if (*cds_name ==  ',') {
		    *iapl_name = D2_AVA_SEP;
		    iapl_name++;
		    cds_name++;
		}
		else {
		    rdn_loop = FALSE;
		    if (*cds_name ==  '\0') {

			*iapl_name = '\0';
		
		    } else {

			*iapl_name = D2_RDN_SEP;
			iapl_name ++;
			cds_name ++;
		    }
		}
	    } /* end of rdn_loop */
	}
	
	return (r_code);
}



/*-----------------------------------------------------------------------

	d21_cds_compress_oid ():

	Takes an OID string and compresses the first two OID parts into one
	OID part. For example, 2.5.4.3 is compressed to 85.4.3 as defined 
	in the ASN.1 Basic Encoding Rules.

 -----------------------------------------------------------------------*/

void	d21_cds_compress_oid (	
					u_char	*oid_str)
{
	char	*oid_in,	*oid_out;
	int		first,		second,		new;

	oid_in = (char *) oid_str;
	oid_out = (char *)oid_str;

	/*	compress the first two parts of the OID	as defined 
		by the ASN.1 Basic Encoding Rules					*/
	first = *oid_in - '0';
	oid_in += 2;
	second  = *oid_in - '0';
	new = (first * 40) + second;

	/*	pdunne:		27/03/91
		using a very basic algorithm here - it may have to be upgraded	*/
	*oid_out = '0' + (new / 10);
	oid_out ++;
	*oid_out = '0' + (new % 10);
	
	/* shift the rest of the OID string up	*/
	oid_out ++;
	oid_in ++;
	while (*oid_in != '\0') {
		*oid_out = *oid_in;
		oid_out ++;
		oid_in ++;
	}
	*oid_out = '\0';

}



/*-----------------------------------------------------------------------

	d21_cds_uncompress_oid ():

	Takes an OID string and uncompresses the first part into two OID 
	parts into one. For example, 85.4.3 is uncompressed to 2.5.4.3 
	as defined in the ASN.1 Basic Encoding Rules.

 -----------------------------------------------------------------------*/

void	d21_cds_uncompress_oid (	
						u_char	*oid_in,	
						u_char	*oid_out)
{
	int		first,		second,		new;

	/*	uncompress the first part of the OID as defined 
		by the ASN.1 Basic Encoding Rules					*/
	first = *oid_in - '0';
	oid_in ++;
	second  = *oid_in - '0';
	new = (first * 10) + second;
	second = new % 40;
	first = (new - second) / 40;


	/*	pdunne:		27/03/91
		using a very basic algorithm here - it may have to be upgraded	*/
	*oid_out = '0' + first;
	oid_out ++;
	*oid_out = '.';
	oid_out ++;
	*oid_out = '0' + second;
	
	/* shift the rest of the OID string up	*/
	oid_out ++;
	oid_in ++;
	while (*oid_in != '\0') {
		*oid_out = *oid_in;
		oid_out ++;
		oid_in ++;
	}
	*oid_out = '\0';

}


/*-----------------------------------------------------------------------

	d21_cds_dots2hypens ():

	Changes all '.' characters in a string to '-'characters

 -----------------------------------------------------------------------*/

void	d21_cds_dots2hypens (	
					u_char	*oid)	
{
	while (*oid != '\0') {

		if (*oid == '.') {
			*oid = '-';
		}
		oid ++;
	}

}


/*-----------------------------------------------------------------------

	d21_cds_hypens2dots ():

	Changes all '-' characters in a string to '.'characters

 -----------------------------------------------------------------------*/

void	d21_cds_hypens2dots (	
					u_char	*oid)	
{
	while (*oid != '\0') {

		if (*oid == '-') {
			*oid = '.';
		}
		oid ++;
	}

}



/*-----------------------------------------------------------------------

	d21_cds_map_error ():

	Maps an error code received from CDS onto the gds error structure.

 -----------------------------------------------------------------------*/

void	d21_cds_map_error (	
			int			cds_error,
			D2_pbhead	*gds_error)
{
	DUA_TRACE_PROBLEM_IN_CDS(cds_error)

	switch (cds_error) {

		case DNS_POSSIBLECYCLE:
			gds_error->d2_retcode = D2_TEMP_ERR;
			gds_error->d2_errclass = D2_REM_ERR;
			gds_error->d2_errvalue = D2_LOOP_DETECTED;
			break;

		case DNS_ENTRYEXISTS:
			gds_error->d2_retcode = D2_CALL_ERR;
			gds_error->d2_errclass = D2_PAR_ERR;
			gds_error->d2_errvalue = D2_ALREADY_EXISTS;
			break;

		case DNS_UNKNOWNENTRY:
		case DNS_DANGLINGLINK:
			gds_error->d2_retcode = D2_CALL_ERR;
			gds_error->d2_errclass = D2_PAR_ERR;
			gds_error->d2_errvalue = D2_NFOUND;
			break;

		case DNS_INVALIDUPDATE:
			gds_error->d2_retcode = D2_CALL_ERR;
			gds_error->d2_errclass = D2_PAR_ERR;
			gds_error->d2_errvalue = D2_CONSTRAINT_VIOLATION;
			break;

		case DNS_ACCESSVIOLATION:
			gds_error->d2_retcode = D2_CALL_ERR;
			gds_error->d2_errclass = D2_PAR_ERR;
			gds_error->d2_errvalue = D2_ACCESS_RIGHTS_INSUFFICIENT;
			break;

		case DNS_SOCKET:
			gds_error->d2_retcode = D2_TEMP_ERR;
			gds_error->d2_errclass = D2_REM_ERR;
			gds_error->d2_errvalue = D2_NO_RESPONSE;
			break;

		default:
			gds_error->d2_retcode = D2_CALL_ERR;
			gds_error->d2_errclass = D2_PAR_ERR;
			gds_error->d2_errvalue = D2_NOT_SPEC;
			break;
	}
	
}



/*-----------------------------------------------------------------------

	d21_cds_error_info ():

	For some errors the structure D2_error is used in addition to the
	D2_pbhead structure to specify the error information. This function
	assigns the information to the D2_error structure.

 -----------------------------------------------------------------------*/

int	d21_cds_error_info (	
			D2_pbhead		*cds_error,
			dns_full_name_t	*resolved,
			D2_error		**p_error_info)
{
	D2_error	*error_info;
	D2_n_error	*name_error;
	u_char		*gds_resolved,	cds_resolved [D2_DNL_MAX];
	char		*error_space;
	int	error_size,	r_code;
	const char function_name[] = "d21_cds_error_info";

	r_code = D2_NOERROR;
	
	if (cds_error->d2_errvalue == D2_NFOUND) {

		error_size = sizeof (D2_error) + sizeof (D2_n_error) + D2_DNL_MAX;

		if ((error_space = D27_MALLOC(svc_c_sev_warning,error_size)) == NULL) {

			return (D2_ERROR);
		}
		
		error_info = (D2_error *) error_space;
		*p_error_info =  error_info;

		error_space += sizeof (D2_error);
		name_error = (D2_n_error *) error_space;
		error_info->d2_e_problem.d2_e_nmproblem = name_error;

		d21_cds_expand_oids ((u_char *)resolved, cds_resolved);
		error_space += sizeof (D2_n_error);
		gds_resolved = (D2_name_string) error_space;

		if (d21_cdsname2x500name (cds_resolved, gds_resolved) == D2_ERROR) {
			DUA_TRACE_CDS_TO_X500_NAME_FAILED
			r_code = D2_ERROR;
		}

		error_info->d2_e_type = D2_NM_PROBLEM;
		name_error->d2_ne_match = gds_resolved;
		name_error->d2_ne_prob = D2_NFOUND;
	}

	return (r_code);
}


/*-----------------------------------------------------------------------

	d21_cds_attr_error ():

	Assign an attribute error to the D2_error structure.

 -----------------------------------------------------------------------*/

int	d21_cds_attr_error (	
				dns_full_name_t *dns_fname,
				D2_a_type		*attr_type,
				D2_error		**p_gds_error)
{
	D2_error		*gds_error;
	D2_a_error		*attr_error;
	D2_a_problem	*attr_problem;
    u_char      	*gds_dn,		*error_type;
	char			*error_space;
	int	error_size,		len,		r_code;
	const char function_name[] = "d21_cds_attr_error";

	r_code = D2_NOERROR;

	error_size = sizeof (D2_error) +
				 sizeof (D2_a_error) + D2_DNL_MAX +
				 sizeof (D2_a_problem) + D2_OBJ_IDL_MAX;

	if ((error_space = D27_MALLOC(svc_c_sev_warning,error_size)) == NULL) {

        return (D2_ERROR);
    }

	gds_error = (D2_error *) error_space;
	*p_gds_error = gds_error;

	error_space += sizeof (D2_error);
	attr_error = (D2_a_error *) error_space;

	error_space += sizeof (D2_a_error);
	gds_dn = (D2_name_string) error_space;

	error_space += D2_DNL_MAX;
	attr_problem = (D2_a_problem *) error_space;
	
	error_space += sizeof (D2_a_problem);
	error_type =  (Ob_id_string) error_space;

	gds_error->d2_e_type = D2_AT_PROBLEM;
	gds_error->d2_e_problem.d2_e_atproblem = attr_error;

    if (d21_cdsname2x500name ((u_char *)dns_fname, gds_dn) == D2_ERROR) {
		DUA_TRACE_CDS_TO_X500_NAME_FAILED
		return (D2_ERROR);
	}

	attr_error->d2_ae_name = gds_dn;
	attr_error->d2_ae_prob = attr_problem;
	attr_problem->d2_ap_next = (D2_a_problem *) NULL;

	attr_problem->d2_ap_prob = D2_CONSTRAINT_VIOLATION;
	attr_problem->d2_ap_info.d2_a_next = (D2_a_info *) NULL;
	attr_problem->d2_ap_info.d2_a_no_val = 0;
	attr_problem->d2_ap_info.d2_a_val = (D2_a_value *) NULL;

	attr_problem->d2_ap_info.d2_a_type.d2_type = error_type;
	len = attr_type->d2_typ_len;
	memcpy (error_type, attr_type->d2_type,len);
	attr_problem->d2_ap_info.d2_a_type.d2_typ_len = len;

	return (r_code);
}



/*-----------------------------------------------------------------------

	d21_cds_check_for_alias():

	Checks to see if an object is an alias. If an alias is discovered then
	the aliased name is resolved.

 -----------------------------------------------------------------------*/

int	d21_cds_check_for_alias (	
		dns_full_name_t		*dns_fname,
		dns_full_name_t		*cds_alias,
		int					*alias_found,					
		struct dnsFlagStat	*flags,
		D2_pbhead			*op_error)
{
    dns_opq_fname       part_result,	resolved;
	dns_handle			handle;
	u_char				dns_attr_str [D2_DNL_MAX];
	enum dnsAttrType	dns_attrtype;		
	int					len,	r_code;
	const char function_name[] = "d21_cds_check_for_alias";

	r_code = D2_NOERROR;
	*alias_found = FALSE;

	if ((handle = cdsGetHandle()) == (dns_handle) NULL) {
		DUA_TRACE_CDS_FUNCTION_FAILED("cdsGetHandle")
		op_error->d2_retcode = D2_CALL_ERR;
		op_error->d2_errclass = D2_PAR_ERR;
		op_error->d2_errvalue = D2_NO_BIND;
		r_code = D2_ERROR;

	} else {

		len = sizeof (dns_attr_str);
		if (cdsEnumAttr (dns_fname, handle, dnsSLink, &dns_attrtype, 
			(void *)NULL, dns_attr_str, &len, flags) == DNS_SUCCESS) {

			*alias_found = TRUE;
			memset ((char *) flags, 0, sizeof (struct dnsFlagStat));
			flags->fsPartialRes = &part_result;
			flags->fsResLength = sizeof (part_result);
			flags->fsNameType = dnsStrXDS;
			len = sizeof (dns_full_name_t);

			if ((r_code = cdsResolveName (dns_fname, &resolved, &len, flags)) 
				== DNS_ERROR) {
				DUA_TRACE_CDS_FUNCTION_FAILED("cdsResolveName")
				d21_cds_map_error (flags->fsLocStat, op_error);
				r_code = D2_ERROR;
			}
			d21_cds_expand_oids((u_char *)&resolved, (u_char *)cds_alias);

		} else {
			*alias_found = FALSE;
			}



		(void) cdsFreeHandle (handle);
	}

    return (r_code);
}

/*-----------------------------------------------------------------------

	d21_cds_expand_oids ():

    Moves through a name expanding any OIDS it finds.

 -----------------------------------------------------------------------*/

void d21_cds_expand_oids (	
			u_char	*cds_name,
			u_char	*expanded_name)
{
	u_char	*dummy;
	int		TYPE_FOUND,	first,	second,    new;

	TYPE_FOUND = FALSE;

	/* 
		copy the global name prefix to expanded_name 
													*/
	strcpy ((char *)expanded_name, DSW_CDS_GN_PREFIX);
	cds_name += strlen (DSW_CDS_GN_PREFIX);
	expanded_name += strlen (DSW_CDS_GN_PREFIX);
	*expanded_name = *cds_name;
	cds_name ++;
	expanded_name ++;

	while (*cds_name != '\0') {

		/*
			Check for a CDS untyped RDN.
										*/ 
		dummy = cds_name;
		TYPE_FOUND = FALSE;

		while (*dummy != '/' && *dummy != '=' && *dummy != '\0') {

			dummy ++;
		}

		if (*dummy == '=') {

			TYPE_FOUND = TRUE;
		}

		/*
			Map the type part of the RDN.
											*/
		if (TYPE_FOUND) {

			/*  uncompress the first part of the OID as defined
				by the ASN.1 Basic Encoding Rules                   */
			first = *cds_name - '0';
			cds_name ++;
			second  = *cds_name - '0';
			cds_name ++;
			new = (first * 10) + second;
			second = new % 40;
			first = (new - second) / 40;


			/*  pdunne:     27/03/91
				using a basic algorithm here - it may have to be upgraded  */
			*expanded_name = '0' + first;
			expanded_name ++;
			*expanded_name = '.';
			expanded_name ++;
			*expanded_name = '0' + second;
			expanded_name ++;
 			
			while (*cds_name != '=') {
				*expanded_name = *cds_name;
				expanded_name ++;
				cds_name ++;
			}

			*expanded_name =  '=';
			expanded_name ++;
			cds_name ++;
		}

		/* 
			Map the value part of the RDN.
											*/
		while (*cds_name != '/' && *cds_name != '\0') {

			*expanded_name = *cds_name;
			expanded_name ++;
			cds_name ++;
		}

		if (*cds_name ==  '\0') {

			*expanded_name = '\0';
		
		} else {

			*expanded_name = '/';
			expanded_name ++;
			cds_name ++;
		}
	}
	
}
