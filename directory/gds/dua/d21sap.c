/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21sap.c,v $
 * Revision 1.1.4.2  1996/02/18  18:17:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:54:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:35  root]
 * 
 * Revision 1.1.2.4  1994/08/10  08:25:31  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:14:17  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:04  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:51  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:10  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  09:21:46  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:33:03  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:04  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:11:15  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:52:27  marrek]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21sap.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:17:36 $";
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG		*/
/* All Rights Reserved							*/
/*									*/
/* Project	: SAP							*/
/*									*/
/* File Name	: d21sap.c						*/
/*									*/
/* Description  : The module contains functions for conversion of SAP	*/
/*		  attributes from APDU to IAPL and vice versa		*/
/*									*/
/* Date         : November 25, 1993					*/
/*									*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <d21dua.h>

/******** LOCAL FUNCTIONS ********/

static void d21_503_convert_certificate_from_apdu (char ** , char ** );
static void d21_504_alg_id_from_apdu (Alg_id * , char ** , char ** );
static void d21_508_rev_certs_from_apdu (Rev_certs ** , char ** , char ** );

static void d21_513_convert_certificate_to_apdu( Certificate * , char ** );
static void d21_514_alg_id_to_apdu( Alg_id * , char ** );
static void d21_515_dist_name_to_apdu( D2_name_string , char ** );
static void d21_516_utc_time_to_apdu( UTC_time_string ,	char ** );
static void d21_517_bit_str_to_apdu( D2_bit_str * ,	char ** );
static void d21_518_rev_certs_to_apdu( Rev_certs * , char ** );

static signed32 d21_543_pure_certificate_struct_len( char ** );
static signed32 d21_544_alg_id_struct_len( char ** );
static signed32 d21_548_rev_certs_struct_len( char ** );

static signed32 d21_554_alg_id_msg_len( Alg_id * );
static signed32 d21_555_dist_name_msg_len( D2_name_string );
static signed32 d21_556_utc_time_msg_len( UTC_time_string );
static signed32 d21_557_bit_str_msg_len( D2_bit_str * );
static signed32 d21_558_rev_certs_msg_len( Rev_certs * );

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_500_certificate_from_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate	*/
/*		  attribute from the APDU format to the C structure	*/
/*		  used at the IAPL interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_500_certificate_from_apdu (
  char ** from,		/* IN/OUT - pointer to APDU */
  char ** to)		/* OUT - pointer to linearizised IAPL */
{
*from +=  sizeof(D23_av_value);
d21_503_convert_certificate_from_apdu(from,to);
} /* end of d21_500_certificate_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_503_convert_certificate_from_apdu		*/
/*									*/
/* Description	: This function is used to convert a certificate	*/
/*		  attribute from the APDU format to the C structure	*/
/*		  used at the IAPL interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_503_convert_certificate_from_apdu (
  char ** from,		/* IN/OUT - pointer to APDU */
  char ** to)		/* OUT - pointer to linearizised IAPL */
{
D23_ds_name * d23_ds_name;
D23_tm_info * d23_tm_info;
D23_bit_info * d23_bit_info;

Certificate *cert = (Certificate *) *to;
Cert_unsigned *cert_uns = &(cert->to_be_signed);

char * utc_time_end;

*to += sizeof(Certificate);

/* version (integer) */
cert_uns->version = * (signed32 *) *from;
*from += sizeof(signed32);

/* serial number (integer) */
cert_uns->serial_number = * (signed32 *) *from;
*from += sizeof(signed32);

/* signature (algorithm identifier) */
d21_504_alg_id_from_apdu(&(cert_uns->signature),from,to);

/* issuer (distinguished name) */
d23_ds_name = (D23_ds_name *) *from;
*from += sizeof(D23_ds_name);
cert_uns->issuer = (D2_name_string) *to;
D21_MEMCPY(cert_uns->issuer,*from,d23_ds_name->d23_dslen);
*to += d23_ds_name->d23_dsoff;
*from += d23_ds_name->d23_dsoff;

/* validity (two UTC time strings) */
d23_tm_info = (D23_tm_info *) *from;
*from += sizeof(D23_tm_info);
cert_uns->validity.not_before = (UTC_time_string) *to;
D21_MEMCPY(cert_uns->validity.not_before,*from,d23_tm_info->d23_tmlen);
utc_time_end = *to + d23_tm_info->d23_tmlen;
*utc_time_end = '\0';
*to += D2_ALIGN(d23_tm_info->d23_tmlen + 1);
*from += d23_tm_info->d23_tmoff;

d23_tm_info = (D23_tm_info *) *from;
*from += sizeof(D23_tm_info);
cert_uns->validity.not_after = (UTC_time_string) *to;
D21_MEMCPY(cert_uns->validity.not_after,*from,d23_tm_info->d23_tmlen);
utc_time_end = *to + d23_tm_info->d23_tmlen;
*utc_time_end = '\0';
*to += D2_ALIGN(d23_tm_info->d23_tmlen + 1);
*from += d23_tm_info->d23_tmoff;

/* subject (distinguished name) */
d23_ds_name = (D23_ds_name *) *from;
*from += sizeof(D23_ds_name);
cert_uns->subject = (D2_name_string) *to;
D21_MEMCPY(cert_uns->subject,*from,d23_ds_name->d23_dslen);
*to += d23_ds_name->d23_dsoff;
*from += d23_ds_name->d23_dsoff;

/* subject public key info (algorithm identifier and bit string) */
	/* algorithm identifier */
d21_504_alg_id_from_apdu(&(cert_uns->subj_public_key_info.algorithm),from,to);
	/* bit string */
d23_bit_info = (D23_bit_info *) *from;
*from += sizeof (D23_bit_info);
cert_uns->subj_public_key_info.subject_key.d2_size = d23_bit_info->d23_blen;
cert_uns->subj_public_key_info.subject_key.d2_value = (unsigned char *) *to;
D21_MEMCPY(cert_uns->subj_public_key_info.subject_key.d2_value,*from,
						d23_bit_info->d23_boff);
*to += d23_bit_info->d23_boff;
*from += d23_bit_info->d23_boff;

/* SIGNED */
	/* first part: algorithm identifier */
d21_504_alg_id_from_apdu(&(cert->alg),from,to);
	/* second part: bit string */
d23_bit_info = (D23_bit_info *) *from;
*from += sizeof (D23_bit_info);
cert->encr_str.d2_size = d23_bit_info->d23_blen;
cert->encr_str.d2_value = (unsigned char *) *to;
D21_MEMCPY(cert->encr_str.d2_value,*from, d23_bit_info->d23_boff);
*to += d23_bit_info->d23_boff;
*from += d23_bit_info->d23_boff;
} /* end of d21_503_convert_certificate_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_501_cert_pair_from_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate pair	*/
/*		  attribute from the APDU format to the C structure	*/
/*		  used at the IAPL interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_501_cert_pair_from_apdu (
  char ** from,		/* IN/OUT: pointer to APDU */
  char ** to)		/* OUT: pointer to linearizised IAPL */
{
D23_certificate * d23_certificate;
Cert_pair * cert_pair = (Cert_pair *) *to;

*from +=  sizeof(D23_av_value);
d23_certificate = (D23_certificate *) *from;
*to += sizeof(Cert_pair);
cert_pair->forward = NULL;
cert_pair->reverse = NULL;

while (d23_certificate->d23_cert_tag == D23_FWDTAG ||
       d23_certificate->d23_cert_tag == D23_REVTAG)
  {
  if (d23_certificate->d23_cert_tag == D23_FWDTAG)
    {
    cert_pair->forward = (Certificate *) *to;
    }
  else
    {
    cert_pair->reverse = (Certificate *) *to;
    }
  *from += sizeof(D23_certificate);

/* convert the certificate */
  d21_503_convert_certificate_from_apdu(from,to);
  d23_certificate = (D23_certificate *) *from;
  }
} /* end of d21_501_cert_pair_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_502_cert_list_from_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate list	*/
/*		  attribute from the APDU format to the C structure	*/
/*		  used at the IAPL interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_502_cert_list_from_apdu (
  char ** from,		/* IN/OUT: pointer to APDU */
  char ** to)		/* OUT: pointer to linearizised IAPL */
{
D23_ds_name * d23_ds_name;
D23_tm_info * d23_tm_info;
D23_bit_info * d23_bit_info;

Cert_list * cert_list = (Cert_list *) *to;
Cert_list_unsigned * cert_list_unsigned = &(cert_list->to_be_signed);

char * utc_time_end;

*from +=  sizeof(D23_av_value);

*to += sizeof(Cert_list);

/* signature (algorithm identifier) */
d21_504_alg_id_from_apdu(&(cert_list_unsigned->signature),from,to);

/* issuer (distinguished name) */
d23_ds_name = (D23_ds_name *) *from;
*from += sizeof(D23_ds_name);
cert_list_unsigned->issuer = (D2_name_string) *to;
D21_MEMCPY(cert_list_unsigned->issuer,*from,d23_ds_name->d23_dslen);
*to += d23_ds_name->d23_dsoff;
*from += d23_ds_name->d23_dsoff;

/* last update (UTC time string) */
d23_tm_info = (D23_tm_info *) *from;
*from += sizeof(D23_tm_info);
cert_list_unsigned->last_update = (UTC_time_string) *to;
D21_MEMCPY(cert_list_unsigned->last_update,*from,d23_tm_info->d23_tmlen);
utc_time_end = *to + d23_tm_info->d23_tmlen;
*utc_time_end = '\0';
*to += D2_ALIGN(d23_tm_info->d23_tmlen + 1);
*from += d23_tm_info->d23_tmoff;

/* revoked certificates */
d21_508_rev_certs_from_apdu(&(cert_list_unsigned->rev_certs),from,to);

/* SIGNED */
	/* first part: algorithm identifier */
d21_504_alg_id_from_apdu(&(cert_list->alg),from,to);
	/* second part: bit string */
d23_bit_info = (D23_bit_info *) *from;
*from += sizeof (D23_bit_info);
cert_list->encr_str.d2_size = d23_bit_info->d23_blen;
cert_list->encr_str.d2_value = (unsigned char *) *to;
D21_MEMCPY(cert_list->encr_str.d2_value,*from,d23_bit_info->d23_boff);
*to += d23_bit_info->d23_boff;
*from += d23_bit_info->d23_boff;
} /* end of d21_502_cert_list_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_504_alg_id_from_apdu			*/
/*									*/
/* Description	: This function is used to convert an algorithm		*/
/*		  identifier from the APDU format to the C structure	*/
/*		  used at the IAPL interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_504_alg_id_from_apdu (
  Alg_id * alg_id,	/* IN - pointer to the Alg_id structure to be filled */
  char ** from,		/* IN/OUT - pointer to APDU */
  char ** to)		/* OUT - pointer to linearizised IAPL */
{
D23_alg_id * d23_alg_id;

d23_alg_id = (D23_alg_id *) *from;
*from += sizeof(D23_alg_id);
alg_id->algorithm.d2_typ_len = d23_alg_id->d23_alglen;
alg_id->algorithm.d2_type = (Ob_id_string) *to;
D21_MEMCPY(alg_id->algorithm.d2_type,*from,d23_alg_id->d23_alglen);
*to += d23_alg_id->d23_algoff;
*from += d23_alg_id->d23_algoff;

if (d23_alg_id->d23_algpar == D23_INT_PARAMETERS)
  {
  alg_id->parameters = (signed32 *) *to;
  *(alg_id->parameters) = *((signed32 *) *from);
  *from += sizeof(signed32);
  *to += sizeof(signed32);
  }
else
  {
  alg_id->parameters = NULL;
  }
} /* end of d21_504_alg_id_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_508_rev_certs_from_apdu			*/
/*									*/
/* Description	: This function is used to convert revoked		*/
/*		  certificates from the APDU format to the C structure	*/
/*		  used at the IAPL interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_508_rev_certs_from_apdu(
  Rev_certs ** rev_certs, /* IN/OUT - pointer to Rev_certs */
  char ** from,		/* IN/OUT - pointer to APDU */
  char ** to)		/* OUT - pointer to linearizised IAPL */
{
D23_rev_cert * d23_rev_cert;
D23_ds_name * d23_ds_name;
D23_tm_info * d23_tm_info;
D23_bit_info * d23_bit_info;

Rev_certs_unsigned * rev_certs_unsigned;
Rev_cert_body * rev_cert_body;

signed32 no_rev_certs;
signed32 i;

char * utc_time_end;

/* initialization of revoked certificates */
*rev_certs = NULL;

no_rev_certs = *(signed32 *)*from;
*from += sizeof(signed32);

if (no_rev_certs != D2_NOT_DEFINED)
  {
  *rev_certs = (Rev_certs *)*to;
  rev_certs_unsigned = &((*rev_certs)->to_be_signed);
  rev_certs_unsigned->no_rev_certs = no_rev_certs;
  *to += sizeof(Rev_certs);
  rev_certs_unsigned->rev_cert = (Rev_cert_body *)*to;
  *to += no_rev_certs * sizeof(Rev_cert_body);
  }

d23_rev_cert = (D23_rev_cert *) *from;

/* scan the message to fill the IAPL structure completely */
for (i = 0 ; i < no_rev_certs; i++)
  {
  rev_cert_body =  rev_certs_unsigned->rev_cert + i;

  /* user certificate (integer) */
  rev_cert_body->user_cert = d23_rev_cert->d23_rc_ucert;

  *from += sizeof(D23_rev_cert);

  /* signature (algorithm identifier) */
  d21_504_alg_id_from_apdu(&(rev_cert_body->signature),from,to);

  /* issuer (distinguished name) */
  d23_ds_name = (D23_ds_name *) *from;
  *from += sizeof(D23_ds_name);
  rev_cert_body->issuer = (D2_name_string) *to;
  D21_MEMCPY(rev_cert_body->issuer,*from,d23_ds_name->d23_dslen);
  *to += d23_ds_name->d23_dsoff;
  *from += d23_ds_name->d23_dsoff;

  /* revocation date (UTC time string) */
  d23_tm_info = (D23_tm_info *) *from;
  *from += sizeof(D23_tm_info);
  rev_cert_body->rev_date = (UTC_time_string) *to;
  D21_MEMCPY(rev_cert_body->rev_date,*from, d23_tm_info->d23_tmlen);
  utc_time_end = *to + d23_tm_info->d23_tmlen;
  *utc_time_end = '\0';
  *to += D2_ALIGN(d23_tm_info->d23_tmlen + 1);
  *from += d23_tm_info->d23_tmoff;

  d23_rev_cert = (D23_rev_cert *) *from;
  }

/* SIGNED */
if (no_rev_certs != D2_NOT_DEFINED)
  {
	/* first part: algorithm identifier */
  d21_504_alg_id_from_apdu(&((*rev_certs)->alg),from,to);
	/* second part: bit string */
  d23_bit_info = (D23_bit_info *) *from;
  *from += sizeof (D23_bit_info);
  (*rev_certs)->encr_str.d2_size = d23_bit_info->d23_blen;
  (*rev_certs)->encr_str.d2_value = (unsigned char *) *to;
  D21_MEMCPY((*rev_certs)->encr_str.d2_value,*from,d23_bit_info->d23_boff);
  *to += d23_bit_info->d23_boff;
  *from += d23_bit_info->d23_boff;
  }
} /* end of d21_508_rev_certs_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_510_certificate_to_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate	*/
/*		  attribute from the C structure used at the IAPL	*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_510_certificate_to_apdu (
  Certificate * from,		/* IN - pointer to IAPL */
  char ** to)			/* IN/OUT - pointer to APDU */
{
D23_av_value * d23_av_value;
char * to_save;

d23_av_value = (D23_av_value *) *to;

*to += sizeof(D23_av_value);
to_save = *to;

d21_513_convert_certificate_to_apdu(from,to);

/* offset and length in av_value */
d23_av_value->d23_avoff = *to - to_save;
d23_av_value->d23_avlen = d23_av_value->d23_avoff;
} /* end of d21_510_certificate_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_513_convert_certificate_to_apdu		*/
/*									*/
/* Description	: This function is used to convert a certificate	*/
/*		  attribute from the C structure used at the IAPL	*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_513_convert_certificate_to_apdu(
  Certificate * from,		/* IN - pointer to IAPL */
  char ** to)			/* IN/OUT - pointer to APDU */
{
Cert_unsigned * cert_unsigned;

cert_unsigned = &(from->to_be_signed);

/* version (integer) */
*((signed32 *)*to) = cert_unsigned->version;
*to += sizeof(signed32);

/* serial number (integer) */
*((signed32 *)*to) = cert_unsigned->serial_number;
*to += sizeof(signed32);

/* signature (algorithm identifier) */
d21_514_alg_id_to_apdu(&(cert_unsigned->signature),to);

/* issuer (distinguished name) */
d21_515_dist_name_to_apdu(cert_unsigned->issuer,to);

/* validity (two UTC time strings) */
d21_516_utc_time_to_apdu(cert_unsigned->validity.not_before,to);
d21_516_utc_time_to_apdu(cert_unsigned->validity.not_after,to);

/* subject (distinguished name) */
d21_515_dist_name_to_apdu(cert_unsigned->subject,to);

/* subject public key info (algorithm identifier and bit string) */
	/* algorithm identifier */
d21_514_alg_id_to_apdu(&(cert_unsigned->subj_public_key_info.algorithm),to);
	/* bit string */
d21_517_bit_str_to_apdu(&(cert_unsigned->subj_public_key_info.subject_key),to);

/* SIGNED */
	/* algorithm identifier */
d21_514_alg_id_to_apdu(&(from->alg),to);

	/* bit string */
d21_517_bit_str_to_apdu(&(from->encr_str),to);
} /* end of d21_513_convert_certificate_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_514_alg_id_to_apdu				*/
/*									*/
/* Description	: This function is used to convert an algorithm		*/
/*		  identifier from the C structure used at the IAPL	*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_514_alg_id_to_apdu(
  Alg_id * alg_id,	/* IN - pointer to algorithm identifier */
  char ** to)		/* IN/OUT - pointer to APDU */
{
D23_alg_id * d23_alg_id;

d23_alg_id = (D23_alg_id *) *to;
d23_alg_id->d23_algtag = D23_ALGTAG;
d23_alg_id->d23_algpar = D23_NO_PARAMETERS;

*to += sizeof(D23_alg_id);

d23_alg_id->d23_alglen = alg_id->algorithm.d2_typ_len;
D21_MEMCPY(*to,alg_id->algorithm.d2_type,d23_alg_id->d23_alglen);
d23_alg_id->d23_algoff = D2_ALIGN(d23_alg_id->d23_alglen);
*to += d23_alg_id->d23_algoff;

if (alg_id->parameters != NULL)
  {
  d23_alg_id->d23_algpar = D23_INT_PARAMETERS;
  *((signed32 *)*to) = *(alg_id->parameters);
  *to += sizeof(signed32);
  }
} /* end of d21_514_alg_id_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_515_dist_name_to_apdu			*/
/*									*/
/* Description	: This function is used to convert an distinguished	*/
/*		  name from the C structure used at the IAPL		*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_515_dist_name_to_apdu(
  D2_name_string name,	/* IN - pointer to distinguished name */
  char ** to)			/* IN/OUT - pinter to APDU */
{
D23_ds_name * d23_ds_name;

d23_ds_name = (D23_ds_name *) *to;
d23_ds_name->d23_dstag = D23_NAMTAG;
d23_ds_name->d23_dscomplete = TRUE;

*to += sizeof(D23_ds_name);

d23_ds_name->d23_dslen = strlen((char *)name) + 1;
D21_MEMCPY(*to,name,d23_ds_name->d23_dslen);
d23_ds_name->d23_dsoff = D2_ALIGN(d23_ds_name->d23_dslen);
*to += d23_ds_name->d23_dsoff;
} /* end of d21_515_dist_name_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_516_utc_time_to_apdu			*/
/*									*/
/* Description	: This function is used to convert an UTC time		*/
/*		  string from the C structure used at the IAPL		*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_516_utc_time_to_apdu(
  UTC_time_string time_str,	/* IN - pointer to UTC time string */
  char ** to)			/* IN/OUT - pinter to APDU */
{
D23_tm_info * d23_tm_info;

d23_tm_info = (D23_tm_info *) *to;
d23_tm_info->d23_tmtag = D23_TIMTAG;
d23_tm_info->d23_tmfil = 0;

*to += sizeof(D23_tm_info);

d23_tm_info->d23_tmlen = strlen((char *)time_str);
D21_MEMCPY(*to,time_str,d23_tm_info->d23_tmlen);
d23_tm_info->d23_tmoff = D2_ALIGN(d23_tm_info->d23_tmlen);
*to += d23_tm_info->d23_tmoff;
} /* end of d21_516_utc_time_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_517_bit_str_to_apdu				*/
/*									*/
/* Description	: This function is used to convert a bit		*/
/*		  string from the C structure used at the IAPL		*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_517_bit_str_to_apdu(
  D2_bit_str * bit_str,	/* IN - pointer to bit string */
  char ** to)			/* IN/OUT - pinter to APDU */
{
D23_bit_info * d23_bit_info;
signed32 len;

d23_bit_info = (D23_bit_info *) *to;
d23_bit_info->d23_btag = D23_BITTAG;
d23_bit_info->d23_bfil = 0;

*to += sizeof(D23_bit_info);

d23_bit_info->d23_blen = bit_str->d2_size;
if (bit_str->d2_size != 0)
  {
  len = ((d23_bit_info->d23_blen - 1)/8) + 1;
  D21_MEMCPY(*to,bit_str->d2_value,len);
  d23_bit_info->d23_boff = D2_ALIGN(len);
  *to += d23_bit_info->d23_boff;
  }
else
  {
  d23_bit_info->d23_boff = 0;
  }
} /* end of d21_517_bit_str_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_511_cert_pair_to_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate pair	*/
/*		  attribute from the C structure used at the IAPL	*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_511_cert_pair_to_apdu (
  Cert_pair * from,		/* IN: pointer to IAPL */
  char ** to)			/* OUT: pointer to APDU */
{
D23_certificate * d23_certificate;
D23_av_value * d23_av_value;
char * to_save;

d23_av_value = (D23_av_value *) *to;

*to += sizeof(D23_av_value);
to_save = *to;

d23_certificate = (D23_certificate *) *to;

if (from->forward != NULL)
  {
  d23_certificate->d23_cert_tag = D23_FWDTAG;
  d23_certificate->d23_cert_fil = 0;
  *to += sizeof(D23_certificate);
  d21_513_convert_certificate_to_apdu(from->forward,to);
  }

d23_certificate = (D23_certificate *) *to;
  
if (from->reverse != NULL)
  {
  d23_certificate->d23_cert_tag = D23_REVTAG;
  d23_certificate->d23_cert_fil = 0;
  *to += sizeof(D23_certificate);
  d21_513_convert_certificate_to_apdu(from->reverse,to);
  }
  
/* offset and length in av_value */
d23_av_value->d23_avoff = *to - to_save;
d23_av_value->d23_avlen = d23_av_value->d23_avoff;
} /* end of d21_511_cert_pair_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_512_cert_list_to_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate list	*/
/*		  attribute from the C structure used at the IAPL	*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_512_cert_list_to_apdu (
  Cert_list * from,		/* IN: pointer to IAPL */
  char ** to)			/* OUT: pointer to APDU */
{
Cert_list_unsigned * cert_list_unsigned;
D23_av_value * d23_av_value;
char * to_save;

d23_av_value = (D23_av_value *) *to;

*to += sizeof(D23_av_value);
to_save = *to;

cert_list_unsigned = &(from->to_be_signed);

/* signature (algorithm identifier) */
d21_514_alg_id_to_apdu(&(cert_list_unsigned->signature),to);

/* issuer (distinguished name) */
d21_515_dist_name_to_apdu(cert_list_unsigned->issuer,to);

/* last update (UTC time string) */
d21_516_utc_time_to_apdu(cert_list_unsigned->last_update,to);

/* revoked certificates */
d21_518_rev_certs_to_apdu(cert_list_unsigned->rev_certs,to);

/* SIGNED */
	/* algorithm identifier */
d21_514_alg_id_to_apdu(&(from->alg),to);

	/* bit string */
d21_517_bit_str_to_apdu(&(from->encr_str),to);

/* offset and length in av_value */
d23_av_value->d23_avoff = *to - to_save;
d23_av_value->d23_avlen = d23_av_value->d23_avoff;
} /* end of d21_512_cert_list_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_518_rev_certs_to_apdu			*/
/*									*/
/* Description	: This function is used to convert the revoked		*/
/*		  certificates from the IAPL				*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : November 25, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_518_rev_certs_to_apdu(
  Rev_certs * from,		/* IN: pointer to IAPL */
  char ** to)			/* OUT: pointer to APDU */
{
signed32 i;
Rev_certs_unsigned * rev_certs_unsigned;
Rev_cert_body * rev_cert_body;
D23_rev_cert * d23_rev_cert;

if (from == NULL)
  {
  *((signed32 *)*to) = D2_NOT_DEFINED;
  *to += sizeof(signed32);
  }
else
  {
  rev_certs_unsigned = &(from->to_be_signed);
  *((signed32 *)*to) = rev_certs_unsigned->no_rev_certs;
  *to += sizeof(signed32);

  for (i=0; i<rev_certs_unsigned->no_rev_certs; i++)
    {
    rev_cert_body = rev_certs_unsigned->rev_cert + i;
    d23_rev_cert = (D23_rev_cert *) *to;
    d23_rev_cert->d23_rc_tag = D23_RVCTAG;
    d23_rev_cert->d23_rc_fil = 0;
    *to += sizeof(D23_rev_cert);

    /* user certificate */
    d23_rev_cert->d23_rc_ucert = rev_cert_body->user_cert;

    /* signature (algorithm identifier) */
    d21_514_alg_id_to_apdu(&(rev_cert_body->signature),to);

    /* issuer (distinguished name) */
    d21_515_dist_name_to_apdu(rev_cert_body->issuer,to);

    /* revocation date (UTC time string) */
    d21_516_utc_time_to_apdu(rev_cert_body->rev_date,to);
    }

  /* SIGNED */
	/* algorithm identifier */
  d21_514_alg_id_to_apdu(&(from->alg),to);

	/* bit string */
  d21_517_bit_str_to_apdu(&(from->encr_str),to);
  } /* if from != NULL */

} /* end of d21_518_rev_certs_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_540_certificate_struct_len			*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  a certificate attribute at IAPL			*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

signed32 d21_540_certificate_struct_len(
  char * in)		/* IN - pointer to APDU message */
{
in += sizeof(D23_av_value);
return(d21_543_pure_certificate_struct_len(&in));
} /* end of d21_540_certificate_struct_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_543_pure_certificate_struct_len		*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  a certificate attribute at IAPL			*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static signed32 d21_543_pure_certificate_struct_len(
  char ** in)		/* IN - pointer to APDU message */
{
signed32 len = 0;

len += sizeof(Certificate);

					/* components of the unsigned part */
*in += sizeof(signed32);				/* version */
*in += sizeof(signed32);				/* serial number */

len += d21_544_alg_id_struct_len(in);		/* algorithm identifier */

len += ((D23_ds_name *)*in)->d23_dsoff;		/* issuer */
*in += sizeof(D23_ds_name) + ((D23_ds_name *)*in)->d23_dsoff;

len += D2_ALIGN(((D23_tm_info *)*in)->d23_tmlen); /* validity (not before) */
*in += sizeof(D23_tm_info) + ((D23_tm_info *)*in)->d23_tmoff;

len += D2_ALIGN(((D23_tm_info *)*in)->d23_tmlen); /* validity (not after) */
*in += sizeof(D23_tm_info) + ((D23_tm_info *)*in)->d23_tmoff;

len += ((D23_ds_name *)*in)->d23_dsoff;		/* subject */
*in += sizeof(D23_ds_name) + ((D23_ds_name *)*in)->d23_dsoff;

						/* subject public key info */
len += d21_544_alg_id_struct_len(in);		/* algorithm identifier */
len += ((D23_bit_info *)*in)->d23_boff;		/* bit string */
*in += sizeof(D23_bit_info) + ((D23_bit_info *)*in)->d23_boff;

					/* SIGNED */
len += d21_544_alg_id_struct_len(in);		/* algorithm identifier */
len += ((D23_bit_info *)*in)->d23_boff;		/* bit string */
*in += sizeof(D23_bit_info) + ((D23_bit_info *)*in)->d23_boff;

return(len);
} /* end of d21_543_pure_certificate_struct_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_544_alg_id_struct_len			*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  an algorithm identifier at IAPL			*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static signed32 d21_544_alg_id_struct_len(
  char ** in)		/* IN/OUT - pointer to APDU message */
{
signed32 len=0;
D23_alg_id * d23_alg_id;

d23_alg_id = (D23_alg_id *)*in;

len += d23_alg_id->d23_algoff;			/* algorithm */
*in += sizeof(D23_alg_id) + d23_alg_id->d23_algoff;

if (d23_alg_id->d23_algpar == D23_INT_PARAMETERS)
  {
  *in += sizeof(signed32);				/* parameters */
  len += sizeof(signed32);
  }

return(len);
} /* end of d21_544_alg_id_struct_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_541_cert_pair_struct_len			*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  a certificate pair attribute at IAPL			*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

signed32 d21_541_cert_pair_struct_len(
  char * in)		/* IN - pointer to APDU message */
{
signed32 len = 0;

in += sizeof(D23_av_value);

len += sizeof(Cert_pair);

while (((D23_certificate *)in)->d23_cert_tag == D23_FWDTAG ||
       ((D23_certificate *)in)->d23_cert_tag == D23_REVTAG)
  {
  in += sizeof(D23_certificate);
  len += d21_543_pure_certificate_struct_len(&in);
  }

return(len);
} /* end of d21_541_cert_pair_struct_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_542_cert_list_struct_len			*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  a certificate list attribute at IAPL			*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

signed32 d21_542_cert_list_struct_len(
  char * in)		/* IN - pointer to APDU message */
{
signed32 len = 0;

in += sizeof(D23_av_value);

len += sizeof(Cert_list);

len += d21_544_alg_id_struct_len(&in);		/* algorithm identifier */

len += ((D23_ds_name *)in)->d23_dsoff;		/* issuer */
in += sizeof(D23_ds_name) + ((D23_ds_name *)in)->d23_dsoff;

len += D2_ALIGN(((D23_tm_info *)in)->d23_tmlen);	/* last update */
in += sizeof(D23_tm_info) + ((D23_tm_info *)in)->d23_tmoff;

len += d21_548_rev_certs_struct_len(&in);	/* revoked certificates */

					/* SIGNED */
len += d21_544_alg_id_struct_len(&in);		/* algorithm identifier */
len += ((D23_bit_info *)in)->d23_boff;		/* bit string */
in += sizeof(D23_bit_info) + ((D23_bit_info *)in)->d23_boff;

return(len);
} /* end of d21_542_cert_list_struct_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_548_rev_certs_struct_len			*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  arevoked certificate at IAPL				*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static signed32 d21_548_rev_certs_struct_len(
  char ** in)		/* IN/OUT - pointer to APDU message */
{
signed32 len=0;
D23_rev_cert * d23_rev_cert;
signed32 no_rev_certs;
signed32 i;

no_rev_certs = *(signed32 *)*in;
*in += sizeof(signed32);

d23_rev_cert = (D23_rev_cert *) *in;

if (no_rev_certs != D2_NOT_DEFINED)
  {
  len += sizeof(Rev_certs);
  }
/* scan the message */
for (i=0; i < no_rev_certs; i++)
  {
  len += sizeof(Rev_cert_body);

  *in += sizeof(D23_rev_cert);

  len += d21_544_alg_id_struct_len(in);			/* signature */

  len += ((D23_ds_name *)*in)->d23_dsoff;		/* issuer */
  *in += sizeof(D23_ds_name) + ((D23_ds_name *)*in)->d23_dsoff;

  len += D2_ALIGN(((D23_tm_info *)*in)->d23_tmlen);	/* revocation date */
  *in += sizeof(D23_tm_info) + ((D23_tm_info *)*in)->d23_tmoff;

  d23_rev_cert = (D23_rev_cert *) *in;
  }

					/* SIGNED */
if (no_rev_certs != D2_NOT_DEFINED)
  {
  len += d21_544_alg_id_struct_len(in);		/* algorithm identifier */
  len += ((D23_bit_info *)*in)->d23_boff;		/* bit string */
  *in += sizeof(D23_bit_info) + ((D23_bit_info *)*in)->d23_boff;
  }

return(len);
} /* end of d21_548_rev_certs_struct_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_550_certificate_msg_len			*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  a certificate attribute in the APDU			*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

signed32 d21_550_certificate_msg_len(
  Certificate * in)	/* IN - pointer to the attribute value at IAPL */
{
signed32 len=0;
Cert_unsigned * cert_unsigned;

cert_unsigned = &(in->to_be_signed);

/* version (integer) */
len +=  sizeof(signed32);

/* serial number (integer) */
len +=  sizeof(signed32);

/* signature (algorithm identifier) */
len += d21_554_alg_id_msg_len(&(cert_unsigned->signature));

/* issuer (distinguished name) */
len += d21_555_dist_name_msg_len(cert_unsigned->issuer);

/* validity (two UTC time strings) */
len += d21_556_utc_time_msg_len(cert_unsigned->validity.not_before);
len += d21_556_utc_time_msg_len(cert_unsigned->validity.not_after);

/* subject (distinguished name) */
len += d21_555_dist_name_msg_len(cert_unsigned->subject);

/* subject public key info (algorithm identifier and bit string) */
	/* algorithm identifier */
len += d21_554_alg_id_msg_len(
			&(cert_unsigned->subj_public_key_info.algorithm));
	/* bit string */
len += d21_557_bit_str_msg_len(
			&(cert_unsigned->subj_public_key_info.subject_key));

/* SIGNED */

/* algorithm identifier */
len += d21_554_alg_id_msg_len(&(in->alg));

/* bit string */
len += d21_557_bit_str_msg_len(&(in->encr_str));

return(len);
} /* end of d21_550_certificate_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_554_alg_id_msg_len				*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  an algorithm identifier in the APDU			*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static signed32 d21_554_alg_id_msg_len(
  Alg_id * in)	/* IN - pointer to the attribute value at IAPL */
{
signed32 len=0;

len +=  sizeof(D23_alg_id);

len += D2_ALIGN(in->algorithm.d2_typ_len);

if (in->parameters != NULL)
  {
  len += sizeof(signed32);
  }

return(len);
} /* end of d21_554_alg_id_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_555_dist_name_msg_len			*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  a distinguished name in the APDU			*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static signed32 d21_555_dist_name_msg_len(
  D2_name_string in)	/* IN - pointer to the attribute value at IAPL */
{
signed32 len=0;

len += sizeof(D23_ds_name);
len += D2_ALIGN(strlen((char *)in));

return(len);
} /* end of d21_555_dist_name_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_556_utc_time_msg_len			*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  a UTC time in the APDU				*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static signed32 d21_556_utc_time_msg_len(
  UTC_time_string in)	/* IN - pointer to the attribute value at IAPL */
{
signed32 len=0;

len += sizeof(D23_tm_info);
len += D2_ALIGN(strlen((char *)in));

return(len);
} /* end of d21_556_utc_time_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_557_bit_str_msg_len				*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  a bit string in the APDU				*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static signed32 d21_557_bit_str_msg_len(
  D2_bit_str * in)	/* IN - pointer to the attribute value at IAPL */
{
signed32 len=0;

len += sizeof(D23_bit_info);
len += D2_ALIGN(((in->d2_size - 1)/8) + 1);

return(len);
} /* end of d21_557_bit_str_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_551_cert_pair_msg_len			*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  a certificate pair attribute in the APDU		*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

signed32 d21_551_cert_pair_msg_len(
  Cert_pair * in)	/* IN - pointer to the attribute value at IAPL */
{
signed32 len=0;

if (in->forward != NULL)
  {
  len += sizeof(D23_certificate);
  len += d21_550_certificate_msg_len(in->forward);
  }

if (in->reverse != NULL)
  {
  len += sizeof(D23_certificate);
  len += d21_550_certificate_msg_len(in->reverse);
  }

return(len);
} /* end of d21_551_cert_pair_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_552_cert_list_msg_len			*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  a certificate list attribute in the APDU		*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

signed32 d21_552_cert_list_msg_len(
  Cert_list * in)	/* IN - pointer to the attribute value at IAPL */
{
signed32 len=0;
Cert_list_unsigned * cert_list_unsigned;

cert_list_unsigned = &(in->to_be_signed);

/* signature (algorithm identifier) */
len += d21_554_alg_id_msg_len(&(cert_list_unsigned->signature));

/* issuer (distinguished name) */
len += d21_555_dist_name_msg_len(cert_list_unsigned->issuer);

/* last update (UTC time string) */
len += d21_556_utc_time_msg_len(cert_list_unsigned->last_update);

/* revoked certificates */
len += d21_558_rev_certs_msg_len(cert_list_unsigned->rev_certs);

/* SIGNED */

/* algorithm identifier */
len += d21_554_alg_id_msg_len(&(in->alg));

/* bit string */
len += d21_557_bit_str_msg_len(&(in->encr_str));

return(len);
} /* end of d21_552_cert_list_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_558_rev_certs_msg_len			*/
/*									*/
/* Description	: This function is used to compute the length of	*/
/*		  revoked certificates in the APDU			*/
/*									*/
/* Return Value	: length						*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static signed32 d21_558_rev_certs_msg_len(
  Rev_certs * in)	/* IN - pointer to the attribute value at IAPL */
{
signed32 len=0;

signed32 i;
Rev_certs_unsigned * rev_certs_unsigned;
Rev_cert_body * rev_cert_body;

len += sizeof(signed32);

if (in != NULL)
  {
  rev_certs_unsigned = &(in->to_be_signed);

  for (i=0; i<rev_certs_unsigned->no_rev_certs; i++)
    {
    rev_cert_body = rev_certs_unsigned->rev_cert + i;
    len += sizeof(D23_rev_cert);

    /* signature (algorithm identifier) */
    len += d21_554_alg_id_msg_len(&(rev_cert_body->signature));

    /* issuer (distinguished name) */
    len += d21_555_dist_name_msg_len(rev_cert_body->issuer);

    /* revocation date (UTC time string) */
    len += d21_556_utc_time_msg_len(rev_cert_body->rev_date);
    }

  /* SIGNED */

  /* algorithm identifier */
  len += d21_554_alg_id_msg_len(&(in->alg));

  /* bit string */
  len += d21_557_bit_str_msg_len(&(in->encr_str));
  } /* of in != NULL */

return(len);
} /* end of d21_558_rev_certs_msg_len */
