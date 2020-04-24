/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2asap.c,v $
 * Revision 1.1.4.2  1996/02/18  19:43:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:19  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:25:19  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/mothra_work/1  1995/07/07  22:49 UTC  dtruong
 * 	Initialized variable "err_apdu"
 * 	[1995/12/08  14:53:57  root]
 * 
 * Revision 1.1.2.7  1994/08/10  08:25:25  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:13:37  marrek]
 * 
 * Revision 1.1.2.6  1994/07/06  15:06:42  marrek
 * 	July 1994 code drop.
 * 	[1994/07/05  09:10:16  marrek]
 * 
 * Revision 1.1.2.5  1994/06/21  14:44:41  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:28:46  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:44:39  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:22  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  15:50:39  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:15:54  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  15:08:43  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:17:57  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:20:52  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:10:40  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:51:56  marrek]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d2asap.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 19:43:22 $";
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG		*/
/* All Rights Reserved							*/
/*									*/
/* Project	: SAP							*/
/*									*/
/* File Name	: d2asap.c						*/
/*									*/
/* Description  : The module contains functions for conversion of SAP	*/
/*		  attributes from APDU to DUA and vice versa		*/
/*									*/
/* Date         : December 8, 1993					*/
/*									*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL FUNCTIONS ********/

static d2_ret_val d2a_203_convert_certificate_from_apdu( char ** ,
							duaCertificate * );
static d2_ret_val d2a_204_alg_id_from_apdu( char ** , duaAlgId * );
static d2_ret_val d2a_205_dist_name_from_apdu( char ** , asn1_field * );
static d2_ret_val d2a_206_utc_time_from_apdu( char ** , asn1_field * );
static d2_ret_val d2a_207_bit_str_from_apdu( char ** , asn1_field * );
static d2_ret_val d2a_208_rev_certs_from_apdu( char ** ,
						duaRevokedCertificates ** );

static d2_ret_val d2a_213_convert_certificate_to_apdu ( duaCertificate * ,
								char ** );
static void d2a_214_alg_id_to_apdu ( duaAlgId * , char **);
static d2_ret_val d2a_215_dist_name_to_apdu ( asn1_field * , char **);
static void d2a_216_utc_time_to_apdu ( asn1_field * , char **);
static void d2a_217_bit_str_to_apdu ( asn1_field * , char **);
static d2_ret_val d2a_218_rev_certs_to_apdu ( duaRevokedCertificates * ,
								char ** );

static size_t d2a_254_alg_id_msg_len( duaAlgId * );
static size_t d2a_255_dist_name_msg_len( asn1_field * );
static size_t d2a_256_utc_time_msg_len( asn1_field * );
static size_t d2a_257_bit_str_msg_len( asn1_field * );
static size_t d2a_258_rev_certs_msg_len( duaRevokedCertificates * );

static void d2a_264_alg_id_free( duaAlgId * );
static void d2a_265_dist_name_free( asn1_field * );
static void d2a_266_utc_time_free( asn1_field * );
static void d2a_267_bit_str_free( asn1_field * );
static void d2a_268_rev_certs_free( duaRevokedCertificates * );

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_200_certificate_from_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate	*/
/*		  attribute from the APDU format to the C structure	*/
/*		  used at the DUA interface				*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

d2_ret_val d2a_200_certificate_from_apdu (
  char ** from,			/* IN/OUT - pointer to APDU */
  duaCertificate * to)		/* IN/OUT - pointer to DUA */
{
d2_ret_val ret_value ;

*from += sizeof(D23_av_value);
ret_value = d2a_203_convert_certificate_from_apdu(from,to);

return(ret_value);
} /* end of d2a_200_certificate_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_203_convert_certificate_from_apdu		*/
/*									*/
/* Description	: This function is used to convert a certificate 	*/
/*		  attribute from the APDU format to the C structure	*/
/*		  used at the DUA interface				*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static d2_ret_val d2a_203_convert_certificate_from_apdu(
  char ** from,			/* IN/OUT - pointer to APDU */
  duaCertificate * cert)		/* IN/OUT - pointer to DUA */
{
const char function_name[] = "d2a_203_convert_certificate_from_apdu";
duaCertUnsigned * cert_uns;

cert_uns = &(cert->unsCert);

/* version (integer) */
cert_uns->version = * (signed32 *) *from;
*from += sizeof(signed32);

/* serial number (integer) */
cert_uns->serialNumber = * (signed32 *) *from;
*from += sizeof(signed32);

/* signature (algorithm identifier) */
if (d2a_204_alg_id_from_apdu(from,
			&(cert_uns->signature)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* issuer (distinguished name) */
if (d2a_205_dist_name_from_apdu(from,
			&(cert_uns->issuer)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* validity (two UTC time strings) */
if (d2a_206_utc_time_from_apdu(from,
			&(cert_uns->validity.notBefore)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
if (d2a_206_utc_time_from_apdu(from,
			&(cert_uns->validity.notAfter)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* subject (distinguished name) */
if (d2a_205_dist_name_from_apdu(from,
			&(cert_uns->subject)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* subject public key info (algorithm identifier and bit string) */
	/* algorithm identifier */
if (d2a_204_alg_id_from_apdu(from,
	&(cert_uns->subjectPublicKeyInfo.algorithm)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
	/* bit string */
if (d2a_207_bit_str_from_apdu(from,
	&(cert_uns->subjectPublicKeyInfo.subjectPublicKey)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* SIGNED */
	/* first part: algorithm identifier */
if (d2a_204_alg_id_from_apdu(from,
	&(cert->signature)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
	/* second part: bit string */
if (d2a_207_bit_str_from_apdu(from,
	&(cert->encrOctString)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

return(D2_NOERROR);
} /* end of d2a_203_convert_certificate_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_204_alg_id_from_apdu			*/
/*									*/
/* Description	: This function is used to convert an algorithm		*/
/*		  identifier from the APDU format to the C structure	*/
/*		  used at the DUA interface				*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static d2_ret_val d2a_204_alg_id_from_apdu(
  char ** from,			/* IN/OUT - pointer to APDU */
  duaAlgId * alg_id)		/* IN/OUT - pointer to DUA */
{
const char function_name[] = "d2a_204_alg_id_from_apdu";
D23_alg_id * d23_alg_id;

d23_alg_id = (D23_alg_id *) *from;
*from += sizeof(D23_alg_id);
alg_id->algorithm.l = d23_alg_id->d23_alglen;

if ((alg_id->algorithm.v = D27_MALLOC(svc_c_sev_warning,alg_id->algorithm.l)) == NULL)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
memcpy((char *)alg_id->algorithm.v,*from,alg_id->algorithm.l);
*from += d23_alg_id->d23_algoff;

if (d23_alg_id->d23_algpar == D23_INT_PARAMETERS)
  {
  if ((alg_id->parameters = D27_MALLOC(svc_c_sev_warning,sizeof(signed32))) == NULL)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  *(alg_id->parameters) = *((signed32 *) *from);
  *from += sizeof(signed32);
  }
else
  {
  alg_id->parameters = NULL;
  }

return(D2_NOERROR);
} /* end of d2a_204_alg_id_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_205_dist_name_from_apdu			*/
/*									*/
/* Description	: This function is used to convert a distinguished	*/
/*		  name from the APDU format to the ASN.1 encoded form	*/
/*		  used at the DUA interface for Certificate		*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static d2_ret_val d2a_205_dist_name_from_apdu(
  char ** from,			/* IN/OUT - pointer to APDU */
  asn1_field * asn1_dn)		/* IN/OUT - pointer to DUA */
{
const char function_name[] = "d2a_205_dist_name_from_apdu";
dua_dist_name dua_dn;
dua_att_value dua_val;
#ifndef __hpux_13317CHFts
unsigned char * err_apdu = NULL;
#else
unsigned char * err_apdu;
#endif
signed32 err_len;
D23_ds_name * d23_ds_name;

d23_ds_name = (D23_ds_name *) *from;

*from += sizeof(D23_ds_name);
if (d2a_016_str_to_dn((char *)*from,&dua_dn,err_apdu,&err_len) != D2_NOERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
*from += d23_ds_name->d23_dsoff;

dua_val.mode = D2_DISTNAME;
dua_val.value.name_value = &dua_dn;
if (dua_att_value_ids(&dua_val,asn1_dn) != D2_NOERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

d_name_free(&dua_dn);

return(D2_NOERROR);
} /* end of d2a_205_dist_name_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_206_utc_time_from_apdu			*/
/*									*/
/* Description	: This function is used to convert an UTC time		*/
/*		  from the APDU format to the C structure		*/
/*		  used at the DUA interface				*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static d2_ret_val d2a_206_utc_time_from_apdu(
  char ** from,			/* IN/OUT - pointer to APDU */
  asn1_field * utc_time)	/* IN/OUT - pointer to DUA */
{
const char function_name[] = "d2a_206_utc_time_from_apdu";
D23_tm_info * d23_tm_info;

d23_tm_info = (D23_tm_info *) *from;
*from += sizeof(D23_tm_info);

utc_time->l = d23_tm_info->d23_tmlen;

if ((utc_time->v = D27_MALLOC(svc_c_sev_warning,utc_time->l)) == NULL)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
memcpy((char *)utc_time->v,*from,utc_time->l);
*from += d23_tm_info->d23_tmoff;

return(D2_NOERROR);
} /* end of d2a_206_utc_time_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_207_bit_str_from_apdu			*/
/*									*/
/* Description	: This function is used to convert abit string		*/
/*		  from the APDU format to the C structure		*/
/*		  used at the DUA interface				*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static d2_ret_val d2a_207_bit_str_from_apdu(
  char ** from,			/* IN/OUT - pointer to APDU */
  asn1_field * bit_str)		/* IN/OUT - pointer to DUA */
{
const char function_name[] = "d2a_207_bit_str_from_apdu";
D23_bit_info * d23_bit_info;

d23_bit_info = (D23_bit_info *) *from;
*from += sizeof (D23_bit_info);

bit_str->l = d23_bit_info->d23_blen;

if (bit_str->l != 0)
  {
  if ((bit_str->v = D27_MALLOC(svc_c_sev_warning,d23_bit_info->d23_boff))
									== NULL)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  memcpy((char *)bit_str->v,*from,d23_bit_info->d23_boff);
  *from += d23_bit_info->d23_boff;
  }
else
  {
  bit_str->v = NULL;
  }

return(D2_NOERROR);
} /* end of d2a_207_bit_str_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_201_cert_pair_from_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate pair	*/
/*		  attribute from the APDU format to the C structure	*/
/*		  used at the DUA interface				*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

d2_ret_val  d2a_201_cert_pair_from_apdu (
  char ** from,			/* IN/OUT: pointer to APDU */
  duaCertPair * cert_pair)		/* IN/OUT: pointer to DUA */
{
const char function_name[] = "d2a_201_cert_pair_from_apdu";
D23_certificate * d23_certificate;

*from +=  sizeof(D23_av_value);
d23_certificate = (D23_certificate *) *from;

cert_pair->forward = NULL;
cert_pair->reverse = NULL;

while (d23_certificate->d23_cert_tag == D23_FWDTAG ||
       d23_certificate->d23_cert_tag == D23_REVTAG)
  {
  if (d23_certificate->d23_cert_tag == D23_FWDTAG)
    {
    if ((cert_pair->forward = D27_MALLOC(svc_c_sev_warning,sizeof(duaCertificate))) == NULL)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    *from += sizeof(D23_certificate);
    if (d2a_203_convert_certificate_from_apdu(from,cert_pair->forward)
							== D2_ERROR)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    }
  if (d23_certificate->d23_cert_tag == D23_REVTAG)
    {
    if ((cert_pair->reverse = D27_MALLOC(svc_c_sev_warning,sizeof(duaCertificate))) == NULL)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    *from += sizeof(D23_certificate);
    if (d2a_203_convert_certificate_from_apdu(from,cert_pair->reverse)
							== D2_ERROR)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    }
  d23_certificate = (D23_certificate *) *from;
  } /* end of while loop */

return(D2_NOERROR);
} /* end of d2a_201_cert_pair_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_202_cert_list_from_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate list	*/
/*		  attribute from the APDU format to the C structure	*/
/*		  used at the DUA interface				*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

d2_ret_val  d2a_202_cert_list_from_apdu (
  char ** from,			/* IN/OUT: pointer to APDU */
  duaCertList * cert_list)		/* IN/OUT: pointer to DUA */
{
const char function_name[] = "d2a_202_cert_list_from_apdu";
duaCertListUnsigned * cert_list_unsigned;

cert_list_unsigned = &(cert_list->unsCertList);

*from +=  sizeof(D23_av_value);

/* signature (algorithm identifier) */
if (d2a_204_alg_id_from_apdu(from,
	&(cert_list_unsigned->signature)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* issuer (distinguished name) */
if (d2a_205_dist_name_from_apdu(from,
			&(cert_list_unsigned->issuer)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* last update (UTC time string) */
if (d2a_206_utc_time_from_apdu(from,
			&(cert_list_unsigned->lastUpdate)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* revoked certificates */
if (d2a_208_rev_certs_from_apdu(from,
		&(cert_list_unsigned->revokedCertificates)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* SIGNED */
	/* first part: algorithm identifier */
if (d2a_204_alg_id_from_apdu(from,
	&(cert_list->signature)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
	/* second part: bit string */
if (d2a_207_bit_str_from_apdu(from,
	&(cert_list->encrOctString)) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

return(D2_NOERROR);
} /* end of d2a_202_cert_list_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_208_rev_certs_from_apdu			*/
/*									*/
/* Description	: This function is used to convert revoked		*/
/*		  certificates from the APDU format to the C structure	*/
/*		  used at the DUA interface				*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static d2_ret_val  d2a_208_rev_certs_from_apdu (
  char ** from,					/* IN/OUT: pointer to APDU */
  duaRevokedCertificates ** rev_certs)		/* IN/OUT: pointer to DUA */
{
const char function_name[] = "d2a_208_rev_certs_from_apdu";
D23_rev_cert * d23_rev_cert;
duaRCUns * rev_certs_unsigned;
duaRCUns_SEQUENCE_OF_SEQUENCE * rev_cert_body;
signed32 no_rev_certs;
signed32 i;

/* initialization of revoked certificates */
*rev_certs = NULL;

no_rev_certs = *(signed32 *)*from;
*from += sizeof(signed32);

if (no_rev_certs != D2_NOT_DEFINED)
  {
  if ((*rev_certs = D27_MALLOC(svc_c_sev_warning,sizeof(duaRevokedCertificates))) == NULL)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  rev_certs_unsigned = &((*rev_certs)->unsRev);
  rev_certs_unsigned->n = no_rev_certs;
  }

if (no_rev_certs != D2_NOT_DEFINED)
  {
  if ((rev_certs_unsigned->v = D27_MALLOC(svc_c_sev_warning,rev_certs_unsigned->n *
			sizeof(duaRCUns_SEQUENCE_OF_SEQUENCE))) == NULL)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  }

d23_rev_cert = (D23_rev_cert *) *from;

/* scan the message to fill the DUA structure completely */
for (i = 0 ; i < no_rev_certs; i++)
  {
  rev_cert_body = rev_certs_unsigned->v + i;

  /* user certificate (integer) */
  rev_cert_body->userCertificate = d23_rev_cert->d23_rc_ucert;

  *from += sizeof(D23_rev_cert);

  /* signature (algorithm identifier) */
  if (d2a_204_alg_id_from_apdu(from,
			&(rev_cert_body->signature)) == D2_ERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }

  /* issuer (distinguished name) */
  if (d2a_205_dist_name_from_apdu(from,
			&(rev_cert_body->issuer)) == D2_ERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }

  /* revocation date (UTC time string) */
  if (d2a_206_utc_time_from_apdu(from,
			&(rev_cert_body->revocationDate)) == D2_ERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }

  d23_rev_cert = (D23_rev_cert *) *from;
  }

/* SIGNED */
if (no_rev_certs != D2_NOT_DEFINED)
  {
	/* first part: algorithm identifier */
  if (d2a_204_alg_id_from_apdu(from,
	&((*rev_certs)->signature)) == D2_ERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
	/* second part: bit string */
  if (d2a_207_bit_str_from_apdu(from,
	&((*rev_certs)->encrOctString)) == D2_ERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  }

return(D2_NOERROR);
} /* end of d2a_208_rev_certs_from_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_210_certificate_to_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate	*/
/*		  attribute from the C structure used at the DUA	*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

d2_ret_val d2a_210_certificate_to_apdu (
  duaCertificate * from,		/* IN - pointer to DUA */
  char ** to)			/* IN/OUT - pointer to APDU */
{
const char function_name[] = "d2a_210_certificate_to_apdu";
D23_av_value * d23_av_value;
char * to_save;

d23_av_value = (D23_av_value *) *to;

*to += sizeof(D23_av_value);
to_save = *to;

if (d2a_213_convert_certificate_to_apdu(from,to) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* offset and length in av_value */
d23_av_value->d23_avoff = *to - to_save;
d23_av_value->d23_avlen = d23_av_value->d23_avoff;

return(D2_NOERROR);
} /* end of d2a_210_certificate_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_213_convert_certificate_to_apdu		*/
/*									*/
/* Description	: This function is used to convert a certificate	*/
/*		  attribute from the C structure used at the DUA	*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static d2_ret_val d2a_213_convert_certificate_to_apdu (
  duaCertificate * from,		/* IN - pointer to DUA */
  char ** to)			/* IN/OUT - pointer to APDU */
{
const char function_name[] = "d2a_213_convert_certificate_to_apdu";
duaCertUnsigned * cert_unsigned;

cert_unsigned = &(from->unsCert);

/* version (integer) */
*((signed32 *)*to) = cert_unsigned->version;
*to += sizeof(signed32);

/* serial number (signed32) */
*((signed32 *)*to) = cert_unsigned->serialNumber;
*to += sizeof(signed32);

/* signature (algorithm identifier) */
d2a_214_alg_id_to_apdu(&(cert_unsigned->signature),to);

/* issuer (distinguished name) */
if (d2a_215_dist_name_to_apdu(&(cert_unsigned->issuer),to) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* validity (two UTC time strings) */
d2a_216_utc_time_to_apdu(&(cert_unsigned->validity.notBefore),to);
d2a_216_utc_time_to_apdu(&(cert_unsigned->validity.notAfter),to);

/* subject (distinguished name) */
if (d2a_215_dist_name_to_apdu(&(cert_unsigned->subject),to) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* subject public key info (algorithm identifier and bit string) */
	/* algorithm identifier */
d2a_214_alg_id_to_apdu(
		&(cert_unsigned->subjectPublicKeyInfo.algorithm),to);
	/* bit string */
d2a_217_bit_str_to_apdu(
		&(cert_unsigned->subjectPublicKeyInfo.subjectPublicKey),to);

/* SIGNED */
	/* algorithm identifier */
d2a_214_alg_id_to_apdu(&(from->signature),to);

	/* bit string */
d2a_217_bit_str_to_apdu(&(from->encrOctString),to);

return(D2_NOERROR);
} /* end of d2a_213_convert_certificate_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_214_alg_id_to_apdu				*/
/*									*/
/* Description	: This function is used to convert an algorithm		*/
/*		  identifier from the C structure used at the DUA	*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d2a_214_alg_id_to_apdu (
  duaAlgId * alg_id,		/* IN - pointer to DUA */
  char ** to)			/* IN/OUT - pointer to APDU */
{
D23_alg_id * d23_alg_id;

d23_alg_id = (D23_alg_id *) *to;
d23_alg_id->d23_algtag = D23_ALGTAG;
d23_alg_id->d23_algpar = D23_NO_PARAMETERS;

*to += sizeof(D23_alg_id);

d23_alg_id->d23_alglen = alg_id->algorithm.l;
memcpy(*to,(char *)alg_id->algorithm.v,d23_alg_id->d23_alglen);
d23_alg_id->d23_algoff = D2_ALIGN(d23_alg_id->d23_alglen);
*to += d23_alg_id->d23_algoff;

if (alg_id->parameters != NULL)
  {
  d23_alg_id->d23_algpar = D23_INT_PARAMETERS;
  *((signed32 *)*to) = *(alg_id->parameters);
  *to += sizeof(signed32);
  }

} /* end of d2a_214_alg_id_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_215_dist_name_to_apdu			*/
/*									*/
/* Description	: This function is used to convert an distinguished	*/
/*		  name from the encoded form used at the DUA		*/
/*		  interface for SAP attributes to the APDU format	*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static d2_ret_val d2a_215_dist_name_to_apdu (
  asn1_field * dist_name,		/* IN - pointer to DUA */
  char ** to)			/* IN/OUT - pointer to APDU */
{
const char function_name[] = "d2a_215_dist_name_to_apdu";
dua_dist_name * dua_name;

dua_name = NULL;

if (dua_dist_name_syntax_dua(dist_name,&dua_name) != D2_NOERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

if (d2a017_dn_to_str(D23_NAMTAG,dua_name,(byte **)to,NULL,0,NULL,0)
								!= D2_NOERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

if (dua_name != NULL)
  {
  dua_name_free(dua_name);
  free (dua_name);
  }

return(D2_NOERROR);
} /* end of d2a_215_dist_name_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_216_utc_time_to_apdu			*/
/*									*/
/* Description	: This function is used to convert an UTC		*/
/*		  time from the C structure used at the DUA		*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d2a_216_utc_time_to_apdu (
  asn1_field * utc_time,		/* IN - pointer to DUA */
  char ** to)			/* IN/OUT - pointer to APDU */
{
D23_tm_info * d23_tm_info;

d23_tm_info = (D23_tm_info *) *to;
d23_tm_info->d23_tmtag = D23_TIMTAG;
d23_tm_info->d23_tmfil = 0;

*to += sizeof(D23_tm_info);

d23_tm_info->d23_tmlen = utc_time->l;
memcpy(*to,(char *)utc_time->v,d23_tm_info->d23_tmlen);
d23_tm_info->d23_tmoff = D2_ALIGN(d23_tm_info->d23_tmlen);
*to += d23_tm_info->d23_tmoff;

} /* end of d2a_216_utc_time_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_217_bit_str_to_apdu				*/
/*									*/
/* Description	: This function is used to convert an bit		*/
/*		  string from the C structure used at the DUA		*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d2a_217_bit_str_to_apdu (
  asn1_field * bit_str,		/* IN - pointer to DUA */
  char ** to)			/* IN/OUT - pointer to APDU */
{
D23_bit_info * d23_bit_info;
signed32 len;

d23_bit_info = (D23_bit_info *) *to;
d23_bit_info->d23_btag = D23_BITTAG;
d23_bit_info->d23_bfil = 0;

*to += sizeof(D23_bit_info);

d23_bit_info->d23_blen = bit_str->l;
if (bit_str->l == 0)
  {
  d23_bit_info->d23_boff = 0;
  }
else
  {
  len = ((bit_str->l - 1)/8) + 1;
  d23_bit_info->d23_boff = D2_ALIGN(len);
  memcpy(*to,(char *)bit_str->v,d23_bit_info->d23_boff);
  }

*to += d23_bit_info->d23_boff;

} /* end of d2a_217_bit_str_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_211_cert_pair_to_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate pair	*/
/*		  attribute from the C structure used at the DUA	*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : December 10, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

d2_ret_val d2a_211_cert_pair_to_apdu (
  duaCertPair * from,		/* IN: pointer to DUA */
  char ** to)			/* OUT: pointer to APDU */
{
const char function_name[] = "d2a_211_cert_pair_to_apdu";
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
  if (d2a_213_convert_certificate_to_apdu(from->forward,to)  == D2_ERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  }

d23_certificate = (D23_certificate *) *to;

if (from->reverse != NULL)
  {
  d23_certificate->d23_cert_tag = D23_REVTAG;
  d23_certificate->d23_cert_fil = 0;
  *to += sizeof(D23_certificate);
  if (d2a_213_convert_certificate_to_apdu(from->reverse,to)  == D2_ERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  }

/* offset and length in av_value */
d23_av_value->d23_avoff = *to - to_save;
d23_av_value->d23_avlen = d23_av_value->d23_avoff;

return(D2_NOERROR);
} /* end of d2a_211_cert_pair_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_212_cert_list_to_apdu			*/
/*									*/
/* Description	: This function is used to convert a certificate list	*/
/*		  attribute from the C structure used at the DUA	*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 10, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

d2_ret_val d2a_212_cert_list_to_apdu (
  duaCertList * from,		/* IN: pointer to DUA */
  char ** to)			/* OUT: pointer to APDU */
{
const char function_name[] = "d2a_212_cert_list_to_apdu";
duaCertListUnsigned * cert_list_unsigned;
D23_av_value * d23_av_value;
char * to_save;

d23_av_value = (D23_av_value *) *to;

*to += sizeof(D23_av_value);
to_save = *to;

cert_list_unsigned = &(from->unsCertList);

/* signature (algorithm identifier) */
d2a_214_alg_id_to_apdu(&(cert_list_unsigned->signature),to);

/* issuer (distinguished name) */
if (d2a_215_dist_name_to_apdu(&(cert_list_unsigned->issuer),to)
							== D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* last update (UTC time string) */
d2a_216_utc_time_to_apdu(&(cert_list_unsigned->lastUpdate),to);

/* revoked certificates */
if (d2a_218_rev_certs_to_apdu(cert_list_unsigned->revokedCertificates,to)
							== D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/* SIGNED */
	/* algorithm identifier */
d2a_214_alg_id_to_apdu(&(from->signature),to);

	/* bit string */
d2a_217_bit_str_to_apdu(&(from->encrOctString),to);

/* offset and length in av_value */
d23_av_value->d23_avoff = *to - to_save;
d23_av_value->d23_avlen = d23_av_value->d23_avoff;

return(D2_NOERROR);
} /* end of d2a_212_cert_list_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_218_rev_certs_to_apdu			*/
/*									*/
/* Description	: This function is used to convert revoked		*/
/*		  certificates from the C structure used at the DUA	*/
/*		  interface to the APDU format				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 8, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static d2_ret_val d2a_218_rev_certs_to_apdu (
  duaRevokedCertificates * from,	/* IN - pointer to DUA */
  char ** to)				/* IN/OUT - pointer to APDU */
{
const char function_name[] = "d2a_218_rev_certs_to_apdu";
signed32 i;
duaRCUns *  rev_certs_unsigned;
duaRCUns_SEQUENCE_OF_SEQUENCE * rev_cert_body;
D23_rev_cert * d23_rev_cert;

if (from == NULL)
  {
  *((signed32 *)*to) = D2_NOT_DEFINED;
  *to += sizeof(signed32);
  }
else
  {
  rev_certs_unsigned = &(from->unsRev);
  *((signed32 *)*to) = rev_certs_unsigned->n;
  *to += sizeof(signed32);

  for (i=0; i<rev_certs_unsigned->n; i++)
    {
    rev_cert_body = rev_certs_unsigned->v + i;
    d23_rev_cert = (D23_rev_cert *) *to;
    d23_rev_cert->d23_rc_tag = D23_RVCTAG;
    d23_rev_cert->d23_rc_fil = 0;
    *to += sizeof(D23_rev_cert);

    /* user certificate */
    d23_rev_cert->d23_rc_ucert = rev_cert_body->userCertificate;

    /* signature (algorithm identifier) */
    d2a_214_alg_id_to_apdu(&(rev_cert_body->signature),to);

    /* issuer (distinguished name) */
    if (d2a_215_dist_name_to_apdu(&(rev_cert_body->issuer),to) == D2_ERROR)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }

    /* revocation date (UTC time string) */
    d2a_216_utc_time_to_apdu(&(rev_cert_body->revocationDate),to);
    }

  /* SIGNED */
	/* algorithm identifier */
  d2a_214_alg_id_to_apdu(&(from->signature),to);

	/* bit string */
  d2a_217_bit_str_to_apdu(&(from->encrOctString),to);

  } /* of revokedCertificates present */

return(D2_NOERROR);
} /* end of d2a_218_rev_certs_to_apdu */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_250_certificate_msg_len			*/
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

size_t d2a_250_certificate_msg_len(
  duaCertificate * in)	/* IN - pointer to the attribute value at DUA */
{
size_t len = 0;
duaCertUnsigned * cert_unsigned;

cert_unsigned = &(in->unsCert);

/* version (integer) */
len +=  sizeof(signed32);

/* serial number (integer) */
len +=  sizeof(signed32);

/* signature (algorithm identifier) */
len += d2a_254_alg_id_msg_len(&(cert_unsigned->signature));

/* issuer (distinguished name) */
len += d2a_255_dist_name_msg_len(&(cert_unsigned->issuer));

/* validity (two UTC time strings) */
len += d2a_256_utc_time_msg_len(&(cert_unsigned->validity.notBefore));
len += d2a_256_utc_time_msg_len(&(cert_unsigned->validity.notAfter));

/* subject (distinguished name) */
len += d2a_255_dist_name_msg_len(&(cert_unsigned->subject));

/* subject public key info (algorithm identifier and bit string) */
	/* algorithm identifier */
len += d2a_254_alg_id_msg_len(
		&(cert_unsigned->subjectPublicKeyInfo.algorithm));
	/* bit string */
len += d2a_257_bit_str_msg_len(
		&(cert_unsigned->subjectPublicKeyInfo.subjectPublicKey));

/* SIGNED */
	/* algorithm identifier */
len += d2a_254_alg_id_msg_len(&(in->signature));

	/* bit string */
len += d2a_257_bit_str_msg_len(&(in->encrOctString));

return(len);
} /* end of d2a_250_certificate_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_254_alg_id_msg_len				*/
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

static size_t d2a_254_alg_id_msg_len(
  duaAlgId * in)	/* IN - pointer to the attribute value at DUA */
{
size_t len = 0;

len += sizeof(D23_alg_id);
len += D2_ALIGN(in->algorithm.l);

if (in->parameters != NULL)
  {
  len += sizeof(signed32);
  }

return(len);
} /* end of d2a_254_alg_id_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_255_dist_name_msg_len			*/
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

static size_t d2a_255_dist_name_msg_len(
  asn1_field * in)	/* IN - pointer to the attribute value at DUA */
{
size_t len = 0;

len += sizeof(D23_ds_name);
len += D2_DNL_MAX;

return(len);
} /* end of d2a_255_dist_name_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_256_utc_time_msg_len			*/
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

static size_t d2a_256_utc_time_msg_len(
  asn1_field * in)	/* IN - pointer to the attribute value at DUA */
{
size_t len = 0;

len += sizeof(D23_tm_info);
len += D2_ALIGN(in->l);

return(len);
} /* end of d2a_256_utc_time_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_257_bit_str_msg_len				*/
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

static size_t d2a_257_bit_str_msg_len(
  asn1_field * in)	/* IN - pointer to the attribute value at DUA */
{
size_t len = 0;

len += sizeof(D23_bit_info);
len += D2_ALIGN(((in->l - 1)/8) + 1);

return(len);
} /* end of d2a_257_bit_str_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_251_cert_pair_msg_len			*/
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

size_t d2a_251_cert_pair_msg_len(
  duaCertPair * in)	/* IN - pointer to the attribute value at DUA */
{
size_t len = 0;

if (in->forward != NULL)
  {
  len += sizeof(D23_certificate);
  len += d2a_250_certificate_msg_len(in->forward);
  }

if (in->reverse != NULL)
  {
  len += sizeof(D23_certificate);
  len += d2a_250_certificate_msg_len(in->reverse);
  }

return(len);
} /* end of d2a_251_cert_pair_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_252_cert_list_msg_len			*/
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

size_t d2a_252_cert_list_msg_len(
  duaCertList * in)	/* IN - pointer to the attribute value at DUA */
{
size_t len=0;
duaCertListUnsigned * cert_list_unsigned;

cert_list_unsigned = &(in->unsCertList);

/* signature (algorithm identifier) */
len += d2a_254_alg_id_msg_len(&(cert_list_unsigned->signature));

/* issuer (distinguished name) */
len += d2a_255_dist_name_msg_len(&(cert_list_unsigned->issuer));

/* last update (UTC time string) */
len += d2a_256_utc_time_msg_len(&(cert_list_unsigned->lastUpdate));

/* revoked certificates */
len += d2a_258_rev_certs_msg_len(cert_list_unsigned->revokedCertificates);

/* SIGNED */
	/* algorithm identifier */
len += d2a_254_alg_id_msg_len(&(in->signature));

	/* bit string */
len += d2a_257_bit_str_msg_len(&(in->encrOctString));

return(len);
} /* end of d2a_252_cert_list_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_258_rev_certs_msg_len			*/
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

static size_t d2a_258_rev_certs_msg_len(
  duaRevokedCertificates * in)	/* IN - pointer to the attribute value at DUA */
{
size_t len=0;
signed32 i;
duaRCUns * rev_certs_unsigned;
duaRCUns_SEQUENCE_OF_SEQUENCE * rev_cert_body;

len += sizeof(signed32);

if (in != NULL)
  {
  rev_certs_unsigned = &(in->unsRev);

  for (i=0; i<rev_certs_unsigned->n; i++)
    {
    rev_cert_body = rev_certs_unsigned->v + i;
    len += sizeof(D23_rev_cert);

    /* signature (algorithm identifier) */
    len += d2a_254_alg_id_msg_len(&(rev_cert_body->signature));

    /* issuer (distinguished name) */
    len += d2a_255_dist_name_msg_len(&(rev_cert_body->issuer));

    /* revocation date (UTC time string) */
    len += d2a_256_utc_time_msg_len(&(rev_cert_body->revocationDate));
    }

  /* SIGNED */
	/* algorithm identifier */
  len += d2a_254_alg_id_msg_len(&(in->signature));

	/* bit string */
  len += d2a_257_bit_str_msg_len(&(in->encrOctString));
  } /* of revoked certificates present */

return(len);
} /* end of d2a_258_rev_certs_msg_len */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_260_certificate_free			*/
/*									*/
/* Description	: This function is used to free memory allocated	*/
/*		  while converting a certificate attribute from		*/
/*		  APDU to the DUA interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d2a_260_certificate_free(
  duaCertificate * cert)	/* IN - pointer to the attribute value at DUA */
{
duaCertUnsigned * cert_uns;

cert_uns = &(cert->unsCert);

/* signature (algorithm identifier) */
d2a_264_alg_id_free(&(cert_uns->signature));

/* issuer (distinguished name) */
d2a_265_dist_name_free(&(cert_uns->issuer));

/* validity (two UTC time strings) */
d2a_266_utc_time_free(&(cert_uns->validity.notBefore));
d2a_266_utc_time_free(&(cert_uns->validity.notAfter));

/* subject (distinguished name) */
d2a_265_dist_name_free(&(cert_uns->subject));

/* subject public key info (algorithm identifier and bit string) */
	/* algorithm identifier */
d2a_264_alg_id_free(&(cert_uns->subjectPublicKeyInfo.algorithm));
	/* bit string */
d2a_267_bit_str_free(&(cert_uns->subjectPublicKeyInfo.subjectPublicKey));

/* SIGNED */
	/* first part: algorithm identifier */
d2a_264_alg_id_free(&(cert->signature));
	/* second part: bit string */
d2a_267_bit_str_free(&(cert->encrOctString));

} /* end of d2a_260_certificate_free */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_264_alg_id_free				*/
/*									*/
/* Description	: This function is used to free memory allocated	*/
/*		  while converting an algorithm identifier from		*/
/*		  APDU to the DUA interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d2a_264_alg_id_free(
  duaAlgId * alg_id)	/* IN - pointer to the attribute value at DUA */
{

if (alg_id->algorithm.l != 0)
  {
  free(alg_id->algorithm.v);
  }

if (alg_id->parameters != NULL)
  {
  free(alg_id->parameters);
  }

} /* end of d2a_264_alg_id_free */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_265_dist_name_free				*/
/*									*/
/* Description	: This function is used to free memory allocated	*/
/*		  while converting a distinguished name from		*/
/*		  APDU to the ASN.1 encoded form used at the DUA	*/
/*		  interface for Certificate				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d2a_265_dist_name_free(
  asn1_field * asn1_dn)	/* IN - pointer to the attribute value at DUA */
{

free(asn1_dn->v);

} /* end of d2a_265_dist_name_free */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_266_utc_time_free				*/
/*									*/
/* Description	: This function is used to free memory allocated	*/
/*		  while converting a UTC time from			*/
/*		  APDU to the DUA interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d2a_266_utc_time_free(
  asn1_field * utc_time)	/* IN - pointer to the attribute value at DUA */
{

free(utc_time->v);

} /* end of d2a_266_utc_time_free */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_267_bit_str_free				*/
/*									*/
/* Description	: This function is used to free memory allocated	*/
/*		  while converting a bit string from			*/
/*		  APDU to the DUA interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d2a_267_bit_str_free(
  asn1_field * bit_str)	/* IN - pointer to the attribute value at DUA */
{

free(bit_str->v);

} /* end of d2a_267_bit_str_free */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_261_cert_pair_free				*/
/*									*/
/* Description	: This function is used to free memory allocated	*/
/*		  while converting a certificate pair attribute from	*/
/*		  APDU to the DUA interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d2a_261_cert_pair_free(
  duaCertPair * cert_pair)	/* IN - pointer to the attribute value at DUA */
{

if (cert_pair->forward != NULL)
  {
  d2a_260_certificate_free(cert_pair->forward);
  free(cert_pair->forward);
  }
if (cert_pair->reverse != NULL)
  {
  d2a_260_certificate_free(cert_pair->reverse);
  free(cert_pair->reverse);
  }

} /* end of d2a_261_cert_pair_free */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_262_cert_list_free				*/
/*									*/
/* Description	: This function is used to free memory allocated	*/
/*		  while converting a certificate list attribute from	*/
/*		  APDU to the DUA interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d2a_262_cert_list_free(
  duaCertList * cert_list)	/* IN - pointer to the attribute value at DUA */
{
duaCertListUnsigned * cert_list_unsigned;

cert_list_unsigned = &(cert_list->unsCertList);

/* signature (algorithm identifier) */
d2a_264_alg_id_free(&(cert_list_unsigned->signature));

/* issuer (distinguished name) */
d2a_265_dist_name_free(&(cert_list_unsigned->issuer));

/* last update (UTC time string) */
d2a_266_utc_time_free(&(cert_list_unsigned->lastUpdate));

/* revoked certificates */
d2a_268_rev_certs_free(cert_list_unsigned->revokedCertificates);

/* SIGNED */
	/* first part: algorithm identifier */
d2a_264_alg_id_free(&(cert_list->signature));
	/* second part: bit string */
d2a_267_bit_str_free(&(cert_list->encrOctString));

} /* end of d2a_262_cert_list_free */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a_268_rev_certs_free				*/
/*									*/
/* Description	: This function is used to free memory allocated	*/
/*		  while converting revoked certificates from		*/
/*		  APDU to the DUA interface				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d2a_268_rev_certs_free(
  duaRevokedCertificates * rev_certs)	/* IN - pointer to DUA */
{
duaRCUns * rev_certs_unsigned;
duaRCUns_SEQUENCE_OF_SEQUENCE * rev_cert_body;
signed32 no_rev_certs;

if (rev_certs != NULL)
  {
  rev_certs_unsigned = &((rev_certs)->unsRev);
  for (no_rev_certs = 0 ; no_rev_certs < rev_certs_unsigned->n; no_rev_certs++)
    {
    rev_cert_body = rev_certs_unsigned->v + no_rev_certs;

    /* signature (algorithm identifier) */
    d2a_264_alg_id_free(&(rev_cert_body->signature));

    /* issuer (distinguished name) */
    d2a_265_dist_name_free(&(rev_cert_body->issuer));

    /* revocation date (UTC time string) */
    d2a_266_utc_time_free(&(rev_cert_body->revocationDate));
    }
  free(rev_certs_unsigned->v);

  /* SIGNED */
	/* first part: algorithm identifier */
  d2a_264_alg_id_free(&(rev_certs->signature));
	/* second part: bit string */
  d2a_267_bit_str_free(&(rev_certs->encrOctString));
  free(rev_certs);
  }

} /* end of d2a_268_rev_certs_free */
