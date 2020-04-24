/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gstaval.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:53  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:46:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:39  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:15:58  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:09  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:03:08  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:47:39  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:22:48  marrek]
 * 
 * 	Bug fixes.
 * 	[1994/04/29  09:30:49  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:11:37  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:47:05  keutel
 * 	creation
 * 	[1994/03/21  15:55:04  keutel]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : gstaval.c                                           */
/*                                                                    */
/* Description  : This file contains the functions of the logfile     */
/*                evaluation tool gdsstep of the GDS component, which */
/*                make the APDU attribute values readable.            */
/*                                                                    */
/* Date         : 02/08/94                                            */
/*                                                                    */
/* Author       : E. Kraemer, BUBA NM12                               */
/*                                                                    */
/**********************************************************************/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gstaval.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:53 $";
#endif

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <string.h>

#include <d23apdu.h>
#include <d21spat.h>
#include <dce/cmx_addr.h>

#include <gstep.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define GDS_GST_PRINT_SUBJECT_PUBLIC_KEY_INFO gds_gst_print_signature

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/***** EXPORT  *****/

				    /* Basic representations          */
char *gst_dist_name_rep      = "DistinguishedName";
char *gst_obj_ident_rep      = "ObjectIdentifier";
char *gst_printable_rep      = "PrintableString";
char *gst_t61_rep            = "T61String";
char *gst_numeric_rep        = "NumericString";
char *gst_ia5_rep            = "IA5String";
char *gst_octet_rep          = "OctetString";
char *gst_asn1_rep           = "ASN1";
char *gst_boolean_rep        = "Boolean";
char *gst_integer_rep        = "Integer";
char *gst_utctime_rep        = "UTCTime";
char *gst_s_guide_rep        = "Guide";
char *gst_telex_rep          = "TelexNumber";
char *gst_ttxid_rep          = "TeletexTerminalIdentifier";
char *gst_fax_nr_rep         = "FacsimileTelephoneNumber";
char *gst_psap_rep           = "PresentationAddress";
char *gst_certificate        = "Certificate";
char *gst_cert_pair          = "CertificatePair";
char *gst_cert_list          = "CertificateList";
char *gst_mhs_ora_rep        = "MHSORAddress";
char *gst_mhs_orn_rep        = "MHSORName";
char *gst_mhs_dlsp_rep       = "MHSDLSubmitPermission";
char *gst_acl_rep            = "AccessControlList";
char *gst_ill_rep            = "Illegal representation";

/* STATIC */
				    /* tags                           */
static char *gst_ill_tag            = "Illegal tag";

				    /* ASN1 basic elements            */
static char *gst_seq_of             = "SEQUENCE OF";
static char *gst_choice             = "CHOICE";
static char *gst_signature          = "SIGNATURE";

				    /* criteria tags                  */
static char *gst_criteria           = "criteria";
static char *gst_ill_crit           = "Illegal criteria";

static char *gst_ill_crit_item      = "Illegal criteria item choice";

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_boolean(signed32 len, byte *value);
static void gds_gst_print_integer(signed32 len, byte *value);
static void gds_gst_print_search_guide(signed32 len, byte *value);
static void gds_gst_print_criteria(signed32 *len, byte **value);
static void gds_gst_print_crit_item(signed32 *len, byte **value);
static void gds_gst_print_t61_prtbl_list(signed32 len, byte *value);
static void gds_gst_print_telex_number(signed32 len, byte *value);
static void gds_gst_print_ttx_term_id(signed32 len, byte *value);
static void gds_gst_print_fax_number(signed32 len, byte *value);
static void gds_gst_print_integer_list(signed32 len, byte *value);
static void gds_gst_print_psap_address(signed32 len, byte *value);
static void gds_gst_print_certificate(signed32 *len, byte **value);
static void gds_gst_print_cert_pair(signed32 len, byte *value);
static void gds_gst_print_cert_list(signed32 len, byte *value);
static void gds_gst_print_alg_id(signed32 *len, byte **value);
static void gds_gst_print_signature(signed32 *len, byte **value);
static void gds_gst_print_rev_cert(signed32 *len, byte **value);
static void gds_gst_print_mhs_or_address(signed32 len, byte *value);
static void gds_gst_print_mhs_or_name(signed32 len, byte *value);
static void gds_gst_print_mhs_dl_submit_perm(signed32 len, byte *value);
static void gds_gst_print_string_list(signed32 len, byte *value);
static void gds_gst_print_acl(signed32 len, byte *value);

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_attr_value                            */
/*                                                                    */
/* Description  : The function makes an attribute value readable      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_attr_value(
			/* INOUT  - message pointer to attribute value*/
    byte **message)

{                       /* gds_gst_print_attr_value                   */

  D23_av_value *av_value;           /* attribute value structure      */
  byte *value;                      /* pointer to value               */
  signed32 len;                     /* length of attribute value      */
  signed32 off;                     /* offset of attribute value      */

  av_value = (D23_av_value *)*message;
  len      = (signed32)av_value->d23_avlen;
  off      = (signed32)av_value->d23_avoff;
  value    = *message + sizeof(D23_av_value);

  printf("attribute value, length: %d, offset: %d, ",len,off);
  gds_gst_print_representation(av_value->d23_avrep);

  switch(av_value->d23_avrep)
  {
    case D2_DISTNAME:
      gds_gst_print_dist_name(len,value);
      break;
    case D2_OBJ_IDENT:
      gds_gst_print_obj_ident(len,value);
      break;
    case D2_PRINTABLE:
      printf("%.*s",len,value);
      break;
    case D2_T61:
      printf("%.*s",len,value);
      break;
    case D2_NUMERIC:
      printf("%.*s",len,value);
      break;
    case D2_IA5:
      printf("%.*s",len,value);
      break;
    case D2_TIME:
      printf("%.*s",len,value);
      break;
    case D2_GUIDE:
      gds_gst_print_search_guide(len,value);
      break;
    case D2_T61_PR_LIST:
      gds_gst_print_t61_prtbl_list(len,value);
      break;
    case D2_ASN1:
      gds_gst_print_binary_data(len,value);
      break;
    case D2_OCTET:
      gds_gst_print_binary_data(len,value);
      break;
    case D2_BOOLEAN:
      gds_gst_print_boolean(len,value);
      break;
    case D2_INTEGER:
      gds_gst_print_integer(len,value);
      break;
    case D2_INT_LIST:
      gds_gst_print_integer_list(len,value);
      break;
    case D2_ACCL:
      gds_gst_print_acl(len,value);
      break;
    case D2_TLXNR_STX:
      gds_gst_print_telex_number(len,value);
      break;
    case D2_TTXID_STX:
      gds_gst_print_ttx_term_id(len,value);
      break;
    case D2_FAXNR_STX:
      gds_gst_print_fax_number(len,value);
      break;
    case D2_PSAP_STX:
      gds_gst_print_psap_address(len,value);
      break;
    case D2_CERTIFICATE:
      gds_gst_print_certificate(&len,&value);
      break;
    case D2_CERT_PAIR:
      gds_gst_print_cert_pair(len,value);
      break;
    case D2_CERT_LIST:
      gds_gst_print_cert_list(len,value);
      break;
    case D2_MHS_DLSP_STX:
      gds_gst_print_mhs_dl_submit_perm(len,value);
      break;
    case D2_MHS_ORADR_STX:
      gds_gst_print_mhs_or_address(len,value);
      break;
    case D2_MHS_ORNAME_STX:
      gds_gst_print_mhs_or_name(len,value);
      break;
    default:
      gds_gst_print_binary_data(len,value);
      break;
  }

  printf("\n");

  *message += sizeof(D23_av_value) + off;

}                       /* gds_gst_print_attr_value                   */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_filter_substr                         */
/*                                                                    */
/* Description  : The function makes a filter substring readable      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_filter_substr(
			/* INOUT  - message pointer to filter subvalue*/
    byte **message)

{                       /* gds_gst_print_filter_substr                */

  D23_fs1_info *f_substr;           /* filter substring structure     */
  byte *value;                      /* pointer to value               */
  signed16 position;                /* substrings representation      */
  unsigned16 rep;                   /* substrings representation      */
  signed32 len;                     /* length of attribute value      */
  signed32 off;                     /* offset of attribute value      */

  f_substr = (D23_fs1_info *)*message;
  position = f_substr->d23_fs1position;
  rep      = f_substr->d23_fs1rep;
  len      = (signed32)f_substr->d23_fs1len;
  off      = (signed32)f_substr->d23_fs1off;
  value    = *message + sizeof(D23_fs1_info);

  printf("filter substring, length: %d, offset: %d, position: ",len,off);

  switch(position)
  {
    case D2_INITIAL:
      printf("initial ");
      break;
    case D2_ANY:
      printf("any ");
      break;
    case D2_FINAL:
      printf("final ");
      break;
    default:
      printf("Illegal substrings position: %d ",position);
      break;
  }

  gds_gst_print_representation(rep);

  switch(rep)
  {
    case D2_DISTNAME:
      gds_gst_print_dist_name(len,value);
      break;
    case D2_OBJ_IDENT:
      gds_gst_print_obj_ident(len,value);
      break;
    case D2_PRINTABLE:
      printf("%.*s",len,value);
      break;
    case D2_T61:
      printf("%.*s",len,value);
      break;
    case D2_NUMERIC:
      printf("%.*s",len,value);
      break;
    case D2_IA5:
      printf("%.*s",len,value);
      break;
    case D2_TIME:
      printf("%.*s",len,value);
      break;
    case D2_GUIDE:
      gds_gst_print_search_guide(len,value);
      break;
    case D2_T61_PR_LIST:
      gds_gst_print_t61_prtbl_list(len,value);
      break;
    case D2_ASN1:
      gds_gst_print_binary_data(len,value);
      break;
    case D2_OCTET:
      gds_gst_print_binary_data(len,value);
      break;
    case D2_BOOLEAN:
      gds_gst_print_boolean(len,value);
      break;
    case D2_INTEGER:
      gds_gst_print_integer(len,value);
      break;
    case D2_INT_LIST:
      gds_gst_print_integer_list(len,value);
      break;
    case D2_ACCL:
      gds_gst_print_acl(len,value);
      break;
    case D2_TLXNR_STX:
      gds_gst_print_telex_number(len,value);
      break;
    case D2_TTXID_STX:
      gds_gst_print_ttx_term_id(len,value);
      break;
    case D2_FAXNR_STX:
      gds_gst_print_fax_number(len,value);
      break;
    case D2_PSAP_STX:
      gds_gst_print_psap_address(len,value);
      break;
    case D2_CERTIFICATE:
      gds_gst_print_certificate(&len,&value);
      break;
    case D2_CERT_PAIR:
      gds_gst_print_cert_pair(len,value);
      break;
    case D2_CERT_LIST:
      gds_gst_print_cert_list(len,value);
      break;
    case D2_MHS_DLSP_STX:
      gds_gst_print_mhs_dl_submit_perm(len,value);
      break;
    case D2_MHS_ORADR_STX:
      gds_gst_print_mhs_or_address(len,value);
      break;
    case D2_MHS_ORNAME_STX:
      gds_gst_print_mhs_or_name(len,value);
      break;
    default:
      gds_gst_print_binary_data(len,value);
      break;
  }

  printf("\n");

  *message += sizeof(D23_fs1_info) + off;

}                       /* gds_gst_print_filter_substr                */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_representation                        */
/*                                                                    */
/* Description  : The function makes an ASN.1 representation readable */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_representation(
			/* IN     - representation to print           */
    unsigned16 rep)

{                       /* gds_gst_print_representation               */

  printf("representation: ");

  switch(rep)
  {
    case D2_DISTNAME:
      printf("%s",gst_dist_name_rep);
      break;
    case D2_OBJ_IDENT:
      printf("%s",gst_obj_ident_rep);
      break;
    case D2_PRINTABLE:
      printf("%s",gst_printable_rep);
      break;
    case D2_T61:
      printf("%s",gst_t61_rep);
      break;
    case D2_NUMERIC:
      printf("%s",gst_numeric_rep);
      break;
    case D2_IA5:
      printf("%s",gst_ia5_rep);
      break;
    case D2_TIME:
      printf("%s",gst_utctime_rep);
      break;
    case D2_GUIDE:
      printf("%s",gst_s_guide_rep);
      break;
    case D2_T61_PR_LIST:
      printf("%s %s {%s, %s}",gst_seq_of,gst_choice,gst_t61_rep,
	gst_printable_rep);
      break;
    case D2_ASN1:
      printf("%s",gst_asn1_rep);
      break;
    case D2_OCTET:
      printf("%s",gst_octet_rep);
      break;
    case D2_BOOLEAN:
      printf("%s",gst_boolean_rep);
      break;
    case D2_INTEGER:
      printf("%s",gst_integer_rep);
      break;
    case D2_INT_LIST:
      printf("%s %s",gst_seq_of,gst_integer_rep);
      break;
    case D2_ACCL:
      printf("%s",gst_acl_rep);
      break;
    case D2_TLXNR_STX:
      printf("%s",gst_telex_rep);
      break;
    case D2_TTXID_STX:
      printf("%s",gst_ttxid_rep);
      break;
    case D2_FAXNR_STX:
      printf("%s",gst_fax_nr_rep);
      break;
    case D2_PSAP_STX:
      printf("%s",gst_psap_rep);
      break;
    case D2_CERTIFICATE:
      printf("%s",gst_certificate);
      break;
    case D2_CERT_PAIR:
      printf("%s",gst_cert_pair);
      break;
    case D2_CERT_LIST:
      printf("%s",gst_cert_list);
      break;
    case D2_MHS_DLSP_STX:
      printf("%s",gst_mhs_dlsp_rep);
      break;
    case D2_MHS_ORADR_STX:
      printf("%s",gst_mhs_ora_rep);
      break;
    case D2_MHS_ORNAME_STX:
      printf("%s",gst_mhs_orn_rep);
      break;
    default:
      printf("%s: %d",gst_ill_rep,rep);
      break;
  }

  printf("\n");

}                       /* gds_gst_print_representation               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_boolean                               */
/*                                                                    */
/* Description  : The function makes a boolean attribute value        */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_boolean(
    signed32 len,       /* IN    - length of boolean value            */
    byte *value)        /* IN    - boolean value                      */

{                       /* gds_gst_print_boolean                      */

  if                    /* illegal length                             */
    (len != sizeof(signed32))
  {                     /* print error indication                     */
    printf("illegal length for boolean attribute value\n");
    gds_gst_print_binary_data(len,value);
  }
  else
  {                     /* check value                                */
    switch(*(signed32 *)value)
    {
      case TRUE:
	printf(gst_true);
	break;
      case FALSE:
	printf(gst_false);
	break;
    }
  }


}                       /* gds_gst_print_boolean                      */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_integer                               */
/*                                                                    */
/* Description  : The function makes an integer attribute value       */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_integer(
    signed32 len,       /* IN    - length of integer value            */
    byte *value)        /* IN    - integer value                      */

{                       /* gds_gst_print_integer                      */

  if                    /* illegal length                             */
    (len != sizeof(signed32))
  {                     /* print error indication                     */
    printf("illegal length for integer attribute value\n");
    gds_gst_print_binary_data(len,value);
  }
  else
  {                     /* print value                                */
    printf("%d",*(signed32 *)value);
  }

}                       /* gds_gst_print_integer                      */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_search_guide                          */
/*                                                                    */
/* Description  : The function makes a search guide attribute value   */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_search_guide(
    signed32 len,       /* IN    - length of search guide value       */
    byte *value)        /* IN    - search guide value                 */

{                       /* gds_gst_print_search_guide                 */

  signed32 obid_len;    /* length of object identifier                */
  signed32 rest_len;    /* rest length of attribute value             */

  rest_len = len;
  if                    /* object class present                       */
    ((obid_len = ((d21_obj_len *)value)->d21_length) > 0)
  {                     /* print object class                         */
    printf("objectClass\n");
    gds_gst_print_obj_ident(obid_len,value + sizeof(d21_obj_len));
    value    += sizeof(d21_obj_len) + ((d21_obj_len *)value)->d21_offset;
    rest_len -= sizeof(d21_obj_len) + ((d21_obj_len *)value)->d21_offset;
  }
  else
  {                     /* print criteria                             */
    gds_gst_print_criteria(&rest_len,&value);
  }

}                       /* gds_gst_print_search_guide                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_criteria                              */
/*                                                                    */
/* Description  : The function makes a criteria component readable    */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_criteria(
    signed32 *len,      /* INOUT - rest length of search guide value  */
    byte **criteria)    /* INOUT - criteria value                     */

{                       /* gds_gst_print_criteria                     */

  signed16 count;       /* count of subcriteria                       */
  register signed16 i;  /* loop variable                              */

  printf("%s\n",gst_criteria);
  switch(((d21_crdesc *)*criteria)->d21_choice)
  {
    case D2_ITEM:
      printf("%s\n",gst_item);
      *criteria += sizeof(d21_crdesc);
      *len      -= sizeof(d21_crdesc);
      gds_gst_print_crit_item(len,criteria);
      break;
    case D2_AND:
    case D2_OR:
      printf("%s \n",((d21_crdesc *)*criteria)->d21_choice == D2_AND ?
	gst_and : gst_or);
      count = ((d21_crdesc *)*criteria)->d21_count;
      *criteria += sizeof(d21_crdesc);
      *len -= sizeof(d21_crdesc);
      for               /* all subcriteria                            */
	 (i = 0; i < count && *len > 0; i++)
      {                 /* print subcriteria                          */
	gds_gst_print_criteria(len,criteria);
      }
      break;
    case D2_NOT:
      printf("%s \n",gst_not);
      *criteria += sizeof(d21_crdesc);
      *len -= sizeof(d21_crdesc);
      gds_gst_print_criteria(len,criteria);
      break;
    default:
      printf("%s: %d\n",gst_ill_crit,((d21_crdesc *)*criteria)->d21_choice);
      *criteria += sizeof(d21_crdesc);
      *len -= sizeof(d21_crdesc);
      gds_gst_print_binary_data(*len,*criteria);
      *criteria += *len;
      *len  = 0;
      break;
  }

}                       /* gds_gst_print_criteria                     */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_crit_item                             */
/*                                                                    */
/* Description  : The function makes a criteria item readable         */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_crit_item(
    signed32 *len,      /* INOUT - rest length of search guide value  */
    byte **crit_item)   /* INOUT - criteria value                     */

{                       /* gds_gst_print_crit_item                    */

  switch(((d21_itdesc *)*crit_item)->d21_match)
  {
    case D2_EQUAL:
      printf("%s \n",gst_equal);
      break;
    case D2_SUBSTRINGS:
      printf("%s \n",gst_substrings);
      break;
    case D2_GTEQ:
      printf("%s \n",gst_greater_or_equal);
      break;
    case D2_LTEQ:
      printf("%s \n",gst_less_or_equal);
      break;
    case D2_APPROX_MATCH:
      printf("%s \n",gst_approximate_match);
      break;
    default:
      printf("%s: %d\n",gst_ill_crit_item,
	((d21_itdesc *)*crit_item)->d21_match);
      gds_gst_print_binary_data(((d21_itdesc *)*crit_item)->d21_length,
	*crit_item + sizeof(d21_itdesc));
      *crit_item += sizeof(d21_itdesc) +
	((d21_itdesc *)*crit_item)->d21_offset;
      *len -= sizeof(d21_itdesc) + ((d21_itdesc *)*crit_item)->d21_offset;
      return;
  }
  gds_gst_print_obj_ident(((d21_itdesc *)*crit_item)->d21_length,
    *crit_item + sizeof(d21_itdesc));
  *crit_item += sizeof(d21_itdesc) + ((d21_itdesc *)*crit_item)->d21_offset;
  *len -= sizeof(d21_itdesc) + ((d21_itdesc *)*crit_item)->d21_offset;

}                       /* gds_gst_print_crit_item                    */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_t61_prtbl_list                        */
/*                                                                    */
/* Description  : The function makes an sequence of T61 and printable */
/*                strings readable                                    */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_t61_prtbl_list(
    signed32 len,       /* IN    - length of stringlist               */
    byte *value)        /* IN    - sequence of strings                */

{                       /* gds_gst_print_t61_prtbl_list               */

  byte       *component;/* component of string list                   */
			/* length of the component                    */
  signed32   len_component;
  signed32   rest_len;  /* rest length of attribute value             */
  byte       rep;       /* representation of one component            */

  for                   /* all components of the sequence             */
     (rest_len = len; rest_len > 0; )
  {                     /* set the component pointer                  */
    rep   = *value;
    component = value + 1;
    rest_len--;
    value = memchr(component,D2_EOS,rest_len);
    if                  /* not last component                         */
      (value != NULL)
    {                   /* reset value and length of component        */
      len_component = value - component;
      value++;
    }
    else
    {                   /* reset value and length of component        */
      len_component = rest_len;
      value         = component + rest_len;
    }
    rest_len -= (value - component);

    switch(rep)
    {
      case D2_T61_REP:
	printf("%s %.*s\n",gst_t61_rep,len_component,component);
	break;
      case D2_PRTBL_REP:
	printf("%s %.*s\n",gst_printable_rep,len_component,component);
	break;
      default:          /* illegal representation                     */
	printf("%s: %d\n",gst_ill_rep,rep);
	break;
    }
  }

}                       /* gds_gst_print_t61_prtbl_list               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_telex_number                          */
/*                                                                    */
/* Description  : The function makes a telex number readable          */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_telex_number(
    signed32 len,       /* IN    - length of stringlist               */
    byte *value)        /* IN    - sequence of strings                */

{                       /* gds_gst_print_telex_number                 */

  byte       *component;/* component of string list                   */
			/* length of the component                    */
  signed32   len_component;
  signed32   rest_len;  /* rest length of attribute value             */

  signed16   i;         /* loop variable                              */

  for                   /* all components of the sequence             */
     (i = 0, rest_len = len; rest_len > 0; i++)
  {                     /* set the component pointer                  */
    component = value;
    value = memchr(component,D2_EOS,rest_len);
    if                  /* not last component                         */
      (value != NULL)
    {                   /* reset value and length of component        */
      len_component = value - component;
      value++;
    }
    else
    {                   /* reset value and length of component        */
      len_component = rest_len;
      value         = component + rest_len;
    }
    rest_len -= (value - component);

    switch(i)
    {
      case 0:
	printf("%s %.*s\n","telexNumber",len_component,component);
	break;
      case 1:
	printf("%s %.*s\n","countryCode",len_component,component);
	break;
      case 2:
	printf("%s %.*s\n","answerBack",len_component,component);
	break;
      default:          /* illegal component                          */
	printf("%s: %d\n","unknown component of telex number",i);
	gds_gst_print_binary_data(len_component,component);
	gds_gst_print_binary_data(rest_len,value);
	rest_len = 0;
	break;
    }
  }

}                       /* gds_gst_print_t61_prtbl_list               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_ttx_term_id                           */
/*                                                                    */
/* Description  : The function makes a teletex terminal identifier    */
/*                readable.                                           */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_ttx_term_id(
    signed32 len,       /* IN    - length of teletex terminal id      */
    byte *value)        /* IN    - teletex terminal Id                */

{                       /* gds_gst_print_ttx_term_id                  */

			/* length of the component                    */
  signed32   len_component;
  signed32   rest_len;  /* rest length of attribute value             */

  for                   /* all components of the sequence             */
     (rest_len = len; rest_len > 0; )
  {                     /* check kind of component                    */
    len_component = sizeof(d21_ttxid) + ((d21_ttxid *)value)->d2_offset;
    switch(((d21_ttxid *)value)->d2_str_type)
    {
      case D2_TERM:
	printf("teletexTerminal\n");
	printf("%.*s\n",((d21_ttxid *)value)->d2_str_len,
	  value + sizeof(d21_ttxid));
	break;
      case D2_CTRL:
	printf("ControlCharacterSets\n");
	printf("%.*s\n",((d21_ttxid *)value)->d2_str_len,
	  value + sizeof(d21_ttxid));
	break;
      case D2_GRPH:
	printf("GraphicCharacterSets\n");
	printf("%.*s\n",((d21_ttxid *)value)->d2_str_len,
	  value + sizeof(d21_ttxid));
	break;
      case D2_MISC:
	printf("MiscellaneousCapabilities\n");
	printf("%.*s\n",((d21_ttxid *)value)->d2_str_len,
	  value + sizeof(d21_ttxid));
	break;
      case D2_PAGE:
	printf("PageFormats\n");
	gds_gst_print_binary_data(((d21_ttxid *)value)->d2_str_len,
	  value + sizeof(d21_ttxid));
	break;
      case D2_PRIV:
	printf("PrivateUse\n");
	gds_gst_print_binary_data(((d21_ttxid *)value)->d2_str_len,
	  value + sizeof(d21_ttxid));
	break;
      default:          /* illegal component                          */
	printf("%s: %d\n","unknown component of ttx terminal Id",
	  ((d21_ttxid *)value)->d2_str_type);
	gds_gst_print_binary_data(rest_len,value);
	len_component = rest_len;
	break;
    }
    rest_len -= len_component;
    value    += len_component;
  }

}                       /* gds_gst_print_ttx_term_id                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_fax_number                            */
/*                                                                    */
/* Description  : The function makes a fax number readable            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_fax_number(
    signed32 len,       /* IN    - length of fax number               */
    byte *value)        /* IN    - fax number                         */

{                       /* gds_gst_print_fax_number                   */

			/* G3 Facsimile nonbasic parameters           */
  unsigned32 g3_fax_nonbasic_par;

  printf("telephoneNumber\n");
  printf("%.*s\n",((d21_faxnr *)value)->d2_str_len,
	  value + sizeof(d21_faxnr));
  if                    /* non basic parameters following             */
    (((d21_faxnr *)value)->d2_offset >= 0)
  {                     /* reset the value pointer                    */
    value += sizeof(d21_ttxid) + ((d21_faxnr *)value)->d2_offset;
    g3_fax_nonbasic_par = *(unsigned32 *)value;
    printf("G3FacsimileNonBasicParameters\n");
    if                  /* "two dimensional" bit is set               */
      (g3_fax_nonbasic_par & D2_TWO_DIMENSIONAL)
    {                   /* print bit name                             */
      printf("two-dimensional ");
    }
    if                  /* "fine resolution bit is set                */
      (g3_fax_nonbasic_par & D2_FINE_RESOLUTION)
    {                   /* print bit name                             */
      printf("fine-resolution ");
    }
    if                  /* "unlimited length" bit is set              */
      (g3_fax_nonbasic_par & D2_UNLIMITED_LENGTH)
    {                   /* print bit name                             */
      printf("unlimited-length ");
    }
    if                  /* "b4 length" bit is set                     */
      (g3_fax_nonbasic_par & D2_B4_LENGTH)
    {                   /* print bit name                             */
      printf("b4-length ");
    }
    if                  /* "a3 width" bit is set                      */
      (g3_fax_nonbasic_par & D2_A3_WIDTH)
    {                   /* print bit name                             */
      printf("a3-width ");
    }
    if                  /* "b4 width " bit is set                     */
      (g3_fax_nonbasic_par & D2_B4_WIDTH)
    {                   /* print bit name                             */
      printf("b4-width ");
    }
    if                  /* "uncompressed" bit is set                  */
      (g3_fax_nonbasic_par & D2_UNCOMPRESSED)
    {                   /* print bit name                             */
      printf("uncompressed ");
    }
    if                  /* illegal bits are set                       */
      (g3_fax_nonbasic_par &
       ~(D2_TWO_DIMENSIONAL | D2_FINE_RESOLUTION | D2_UNLIMITED_LENGTH |
	 D2_B4_LENGTH | D2_A3_WIDTH | D2_B4_WIDTH | D2_UNCOMPRESSED))
    {                   /* print error indication                     */
      printf("illegal bits detected ");
      gds_gst_print_bit_string(8 * sizeof(unsigned32),value);
    }
  }

}                       /* gds_gst_print_fax_number                   */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_integer_list                          */
/*                                                                    */
/* Description  : The function makes a sequence of integers readable  */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_integer_list(
    signed32 len,       /* IN    - length of integer list in byte     */
    byte *value)        /* IN    - integer list                       */

{                       /* gds_gst_integer_list                       */

  signed32 nb_ints;     /* number of integers in the list             */

  register signed32 i;  /* loop variable                              */

  nb_ints = len / sizeof(signed32);

  for                   /* all components of the sequence             */
     (i = 0; i < nb_ints; i++, value += sizeof(signed32))
  {                     /* print the component                        */
    printf("%d ",*(signed32 *)value);
  }

}                       /* gds_gst_print_integer_list                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_psap_address                          */
/*                                                                    */
/* Description  : The function makes a presentation address readable  */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_psap_address(
    signed32 len,       /* IN    - length of PSAP address             */
    byte *value)        /* IN    - PSAP address                       */

{                       /* gds_gst_print_psap_address                 */

			/* length of the component                    */
  signed32   len_component;
  signed32   rest_len;  /* rest length of attribute value             */

  for                   /* all components of the sequence             */
     (rest_len = len; rest_len > 0; )
  {                     /* check kind of component                    */
    len_component = ((T_adrdesc *)value)->t_nextoff;
    switch(((T_adrdesc *)value)->t_infotype)
    {
      case T_PSAPINFO:
	printf("pSelector, len: %d\n",((T_adrdesc *)value)->t_infolng);
	gds_gst_print_binary_data(((T_adrdesc *)value)->t_infolng,
	  value + sizeof(T_adrdesc));
	break;
      case T_SSAPINFO:
	printf("sSelector, len: %d\n",((T_adrdesc *)value)->t_infolng);
	gds_gst_print_binary_data(((T_adrdesc *)value)->t_infolng,
	  value + sizeof(T_adrdesc));
	break;
      case T_TSAPINFO:
	printf("tSelector, len: %d\n",((T_adrdesc *)value)->t_infolng);
	gds_gst_print_binary_data(((T_adrdesc *)value)->t_infolng,
	  value + sizeof(T_adrdesc));
	break;
      case T_NSAPINFO:
	printf("nAddress, len: %d\n",((T_adrdesc *)value)->t_infolng);
	gds_gst_print_binary_data(((T_adrdesc *)value)->t_infolng,
	  value + sizeof(T_adrdesc));
	break;
      default:          /* illegal component                          */
	printf("%s: %d\n","unknown component of presentation address",
	  ((T_adrdesc *)value)->t_infotype);
	gds_gst_print_binary_data(rest_len,value);
	len_component = rest_len;
	break;
    }
    value    += len_component;
    rest_len -= len_component;
  }

}                       /* gds_gst_print_psap_address                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_certificate                           */
/*                                                                    */
/* Description  : The function makes a certificate readable           */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_certificate(
    signed32 *len,      /* INOUT - rest length of structure containing*/
			/*         certificate                        */
    byte **value)       /* INOUT - certificate                        */

{                       /* gds_gst_print_certificate                  */

  printf("version: %ld ",*(long *)*value);
  *value += sizeof(signed32);
  *len   -= sizeof(signed32);

  printf("serialNumber: %ld ",*(long *)*value);
  *value += sizeof(signed32);
  *len   -= sizeof(signed32);

  printf("signature:\n");
  gds_gst_print_alg_id(len,value);

  if                    /* value not yet processed completely         */
    (*len > 0)
  {                     /* print issuer                               */
    printf("issuer:\n");
    gds_gst_print_dn_struct(len,value);
  }

  if                    /* value not yet processed completely         */
    (*len > 0)
  {                     /* print Validity                             */
    printf("validity:\n");
    printf("notBefore:\n");
    gds_gst_print_tm_info(len,value);
  }

  if                    /* value not yet processed completely         */
    (*len > 0)
  {                     /* print not after                            */
    printf("notAfter:\n");
    gds_gst_print_tm_info(len,value);
  }

  if                    /* value not yet processed completely         */
    (*len > 0)
  {                     /* print subject                              */
    printf("subject:\n");
    gds_gst_print_dn_struct(len,value);
  }

  if                    /* value not yet processed completely         */
    (*len > 0)
  {                     /* print subject public key info              */
    printf("subjectPublicKeyInfo:\n");
    GDS_GST_PRINT_SUBJECT_PUBLIC_KEY_INFO(len,value);
  }

  if                    /* value not yet processed completely         */
    (*len > 0)
  {                     /* print subject public key info              */
    printf("%s\n",gst_signature);
    gds_gst_print_signature(len,value);
  }

}                       /* gds_gst_print_certificate                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_cert_pair                             */
/*                                                                    */
/* Description  : The function makes a certificate pair readable      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_cert_pair(
    signed32 len,       /* IN    - length of certificate pair         */
    byte *value)        /* IN    - certificate pair                   */

{                       /* gds_gst_print_cert_pair                    */

  signed32   rest_len;  /* rest length of attribute value             */
  unsigned16 tag;       /* certificate tag                            */

  if                    /* certificate pair is empty                  */
    (len == 0)
  {                     /* print error indication                     */
    printf("empty certificate pair\n");
  }
  else
  {                     /* scan the certificate pair                  */
    for                 /* all components of the certificate pair     */
       (rest_len = len; rest_len > 0; )
    {                   /* check the tag                              */
      tag = ((D23_certificate *)value)->d23_cert_tag;
      switch(tag)
      {
	case D23_FWDTAG:
	  printf("forward\n");
	  rest_len -= sizeof(D23_certificate);
	  value    += sizeof(D23_certificate);
	  gds_gst_print_certificate(&rest_len,&value);
	  break;
	case D23_REVTAG:
	  printf("reverse\n");
	  rest_len -= sizeof(D23_certificate);
	  value    += sizeof(D23_certificate);
	  gds_gst_print_certificate(&rest_len,&value);
	  break;
	default:
	  printf("Illegal Certificate tag: %d\n",tag);
	  gds_gst_print_binary_data(rest_len,value);
	  value += rest_len;
	  rest_len = 0;
	  break;
      }
    }
  }

}                       /* gds_gst_print_cert_pair                    */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_cert_list                             */
/*                                                                    */
/* Description  : The function makes a certificate list readable      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_cert_list(
    signed32 len,       /* IN    - length of certificate list         */
    byte *value)        /* IN    - certificate list                   */

{                       /* gds_gst_print_cert_list                    */

  signed32   rest_len;  /* rest length of attribute value             */
  signed32   nr_rev_cert; /* number of revoked certificates           */

  register   signed32 i;/* loop variable                              */

  rest_len = len;
  printf("signature:\n");
  gds_gst_print_alg_id(&rest_len,&value);

  if                    /* value not yet processed completely         */
    (rest_len > 0)
  {                     /* print issuer                               */
    printf("issuer:\n");
    gds_gst_print_dn_struct(&rest_len,&value);
  }

  if                    /* value not yet processed completely         */
    (rest_len > 0)
  {                     /* print last update                          */
    printf("lastUpdate:\n");
    gds_gst_print_tm_info(&rest_len,&value);
  }

  if                    /* value not yet processed completely         */
    (rest_len > 0)
  {                     /* assign number of revoked certificates      */
    nr_rev_cert = *(signed32 *)value;
    value += sizeof(signed32);
    rest_len -= sizeof(signed32);
    for                 /* all revoked certificates                   */
       (i = 0; rest_len > 0 && i < nr_rev_cert; i++)
    {                     /* print the revoked certificate              */
      gds_gst_print_rev_cert(&rest_len,&value);
    }
    if                  /* value not yet processed completely         */
      (rest_len > 0)
    {                   /* print signature                            */
      gds_gst_print_signature(&rest_len,&value);
    }
  }
}                       /* gds_gst_print_cert_list                    */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_alg_id                                */
/*                                                                    */
/* Description  : The function makes an algorithm identifier readable */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_alg_id(
    signed32 *len,      /* INOUT - rest length of containing structure*/
    byte **value)       /* INOUT - algorithm identifier               */

{                       /* gds_gst_print_alg_id                       */

  unsigned16 algpar;    /* indicates kind of parameters               */
  signed32 obidoff;     /* offset of object identifier                */

  printf("algorithm: ");
  if                    /* algorithm identifier tag is missing        */
    (((D23_alg_id *)*value)->d23_algtag != D23_ALGTAG)
  {                     /* print illegal tag information              */
    printf("%s: %d (algorithm tag expected)\n",gst_ill_tag,
      ((D23_alg_id *)*value)->d23_algtag);
    gds_gst_print_binary_data(*len,*value);
    *value += *len;
    *len = 0;
  }
  else
  {                     /* print object identifier                    */
    algpar  = ((D23_alg_id *)*value)->d23_algpar;
    obidoff = sizeof(D23_alg_id) + ((D23_alg_id *)*value)->d23_algoff;
    gds_gst_print_obj_ident(((D23_alg_id *)*value)->d23_alglen,
      *value + sizeof(D23_alg_id));
    *len   -= obidoff;
    *value += obidoff;
    switch(algpar)
    {
      case D23_NO_PARAMETERS:
	break;
      case D23_INT_PARAMETERS:
	printf("parameters %s %ld\n",gst_integer_rep,*(signed32 *)*value);
	*value += sizeof(signed32);
	*len   -= sizeof(signed32);
	break;
      default:
	printf("Illegal kind of parameters: %d\n",algpar);
	gds_gst_print_binary_data(*len,*value);
	*value += *len;
	*len = 0;
	break;
    }
  }

}                       /* gds_gst_print_alg_id                       */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_signature                             */
/*                                                                    */
/* Description  : The function makes a signature structure readable   */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_signature(
    signed32 *len,      /* INOUT - rest length of containing structure*/
    byte **value)       /* INOUT - signature                          */

{                       /* gds_gst_print_signature                    */

  printf("signature\n");

  gds_gst_print_alg_id(len,value);
  if                    /* signature not processed completely         */
    (*len > 0)
  {                     /* print bit string                           */
    gds_gst_print_bit_struct(len,value);
  }

}                       /* gds_gst_print_signature                    */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_rev_cert                              */
/*                                                                    */
/* Description  : The function makes a revoked certificate readable   */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_rev_cert(
    signed32 *len,      /* INOUT - rest length of containing structure*/
    byte **value)       /* INOUT - revoked certificate                */

{                       /* gds_gst_print_rev_cert                     */

  signed32 ser_nr;      /* serial number of revoked certificate       */
  unsigned16 tag;       /* revoked certificate structure tag          */

  tag = ((D23_rev_cert *)*value)->d23_rc_tag;

  if                    /* tag is revoked certificate tag             */
    (tag == D23_RVCTAG)
  {                     /* assign serial number                       */
    ser_nr  = ((D23_rev_cert *)*value)->d23_rc_ucert;
    *value += sizeof(D23_rev_cert);
    *len   -= sizeof(D23_rev_cert);
    printf("revoked certificate\n");
    printf("serial number: %ld\n",(long)ser_nr);
    if                  /* revoked certificate not processed compl.   */
      (*len > 0)
    {                   /* print signature                            */
      printf("signature\n");
      gds_gst_print_alg_id(len,value);
    }
    if                  /* revoked certificate not processed compl.   */
      (*len > 0)
    {                   /* print issuer                               */
      printf("issuer\n");
      gds_gst_print_dn_struct(len,value);
    }
    if                  /* revoked certificate not processed compl.   */
      (*len > 0)
    {                   /* print revocation date                      */
      printf("revocation date\n");
      gds_gst_print_tm_info(len,value);
    }
  }
  else
  {                     /* print illegal tag information              */
    printf("%s: %d (revoked certificate tag expected)\n",gst_ill_tag,tag);
    gds_gst_print_binary_data(*len,*value);
    *value += *len;
    *len = 0;
  }

}                       /* gds_gst_print_rev_cert                     */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_mhs_or_address                        */
/*                                                                    */
/* Description  : The function makes a MHS OR-address readable        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_mhs_or_address(
    signed32 len,       /* IN    - length of OR address               */
    byte *value)        /* IN    - OR address                         */

{                       /* gds_gst_print_mhs_or_address               */

			/* length of the component                    */
  signed32   len_component;
  signed32   rest_len;  /* rest length of attribute value             */

  for                   /* all components of the sequence             */
     (rest_len = len; rest_len > 0; )
  {                     /* check kind of component                    */
    len_component = sizeof(D21_OR_address_part) +
      ((D21_OR_address_part *)value)->d21_ora_off;
    switch(((D21_OR_address_part *)value)->d21_ora_tag)
    {
      case D21_ORA_ADMD_N:
	printf("ADMD-Name = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_COM_N:
	printf("Common-Name = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_CTRY_N:
	printf("Country-Name = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_DOM_T1:
      case D21_ORA_DOM_T2:
      case D21_ORA_DOM_T3:
      case D21_ORA_DOM_T4:
	printf("DomainDefinedAttribute Type = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_DOM_V1:
      case D21_ORA_DOM_V2:
      case D21_ORA_DOM_V3:
      case D21_ORA_DOM_V4:
	printf("DomainDefinedAttribute Value = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_GEN:
	printf("Generation-qualifier = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_GIV_N:
	printf("Given-Name = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_INIT:
	printf("Initials = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_ORG_N:
	printf("OrganizationName = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_ORGU_N1:
      case D21_ORA_ORGU_N2:
      case D21_ORA_ORGU_N3:
      case D21_ORA_ORGU_N4:
	printf("OrganizationalUnitName = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_A_D:
	printf("printable-address = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_CODE:
	printf("PostalCode = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_CTRY:
	printf("PhysicalDeliveryCountryName = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_D_P_N:
	printf("PhysicalDeliveryPersonalName = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_G_D_A:
	printf("ExtensionPhysicalDeliveryAddressComponents = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_LOC:
	printf("LocalPostalAttributes = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_O_B_N:
	printf("PostOfficeBoxAddress = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_O_N:
	printf("PhysicalDeliveryOfficeName = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_O_NUM:
	printf("PhysicalDeliveryOfficeNumber = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_ORG_N:
	printf("PhysicalDeliveryOrganizationName = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_P_D:
	printf("D21_ORA_P_P_D = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_P_N:
	printf("D21_ORA_P_P_N = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_S_A:
	printf("StreetAddress = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_PRMD_N:
	printf("PrivateDomainName = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_SURNAME:
	printf("Surname = ");
	gds_gst_print_string_list(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_ISDN_N:
	printf("e163-4-address number = ");
	printf("%.*s",((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_ISDN_S:
	printf("e163-4-address sub-address = ");
	printf("%.*s",((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_NUM_ID:
	printf("NumericUserIdentifier = ");
	printf("%.*s",((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_X121_A:
	printf("X121-address = ");
	printf("%.*s",((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_A_F:
	printf("PostalAddress (%s) = ",gst_t61_rep);
	printf("%.*s",((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_D_S_N:
	printf("PhysicalDeliveryServiceName = ");
	printf("%.*s",((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_T_ID:
	printf("TerminalIdentifier = ");
	printf("%.*s",((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_P_A_L:
	printf("D21_ORA_P_A_L = ");
	printf("%.*s",((D21_OR_address_part *)value)->d21_ora_len,
	   value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_PRES_A:
	printf("%s =\n",gst_psap_rep);
	gds_gst_print_psap_address(
	  ((D21_OR_address_part *)value)->d21_ora_len,
	  value + sizeof(D21_OR_address_part));
	break;
      case D21_ORA_T_T:
	printf("%s = ",gst_integer_rep);
	gds_gst_print_integer(((D21_OR_address_part *)value)->d21_ora_len,
	  value + sizeof(D21_OR_address_part));
	break;
      default:          /* illegal component                          */
	printf("%s: %d\n","unknown component of OR-address",
	  ((D21_OR_address_part *)value)->d21_ora_tag);
	gds_gst_print_binary_data(rest_len,value);
	len_component = rest_len;
	break;
    }
    printf("\n");
    value    += len_component;
    rest_len -= len_component;
  }

}                       /* gds_gst_print_mhs_or_address               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_mhs_or_name                           */
/*                                                                    */
/* Description  : The function makes a MHS OR-name readable           */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_mhs_or_name(
    signed32 len,       /* IN    - length of OR name                  */
    byte *value)        /* IN    - OR name                            */

{                       /* gds_gst_print_mhs_or_name                  */

			/* length of the component                    */
  signed32   len_component;
  signed32   rest_len;  /* rest length of attribute value             */

  for                   /* all components of the OR name              */
     (rest_len = len; rest_len > 0; )
  {                     /* check kind of component                    */
    len_component = sizeof(D21_OR_name) +
      ((D21_OR_name *)value)->d21_orn_off;
    switch(((D21_OR_name *)value)->d21_orn_tag)
    {
      case D21_ORN_DN:
	printf("%s\n",gst_dist_name_rep);
	gds_gst_print_dist_name(((D21_OR_name *)value)->d21_orn_len,
	  value + sizeof(D21_OR_name));
	break;
      case D21_ORN_ORA:
	printf("%s\n",gst_mhs_ora_rep);
	gds_gst_print_mhs_or_address(((D21_OR_name *)value)->d21_orn_len,
	  value + sizeof(D21_OR_name));
	break;
      default:          /* illegal component                          */
	printf("%s: %d\n","unknown component of OR-Name",
	  ((D21_OR_name *)value)->d21_orn_tag);
	gds_gst_print_binary_data(rest_len,value);
	len_component = rest_len;
	break;
    }
    value    += len_component;
    rest_len -= len_component;
  }

}                       /* gds_gst_print_mhs_or_name                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_mhs_dl_submit_perm                    */
/*                                                                    */
/* Description  : The function makes a MHS DL-submit permission       */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_mhs_dl_submit_perm(
    signed32 len,       /* IN    - length of DL submit permission     */
    byte *value)        /* IN    - DL submit permission               */

{                       /* gds_gst_print_mhs_dl_submit_perm           */

  switch(((D21_DL_sub_perm *)value)->d21_dlsp_tag)
  {
    case D21_DLSP_INDIV:
    case D21_DLSP_PATT:
    case D21_DLSP_MEM_DL:
      printf("%s\n",gst_mhs_orn_rep);
      gds_gst_print_mhs_or_name(((D21_DL_sub_perm *)value)->d21_dlsp_len,
	value + sizeof(D21_DL_sub_perm));
      break;
    case D21_DLSP_MEM_G:
      printf("%s\n",gst_dist_name_rep);
      gds_gst_print_dist_name(((D21_DL_sub_perm *)value)->d21_dlsp_len,
	value + sizeof(D21_DL_sub_perm));
      break;
    default:            /* illegal component                          */
      printf("%s: %d\n","unknown component of DL-submit-permission",
	((D21_DL_sub_perm *)value)->d21_dlsp_tag);
      gds_gst_print_binary_data(len,value);
      break;
  }

}                       /* gds_gst_print_mhs_dl_submit_perm           */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_string_list                           */
/*                                                                    */
/* Description  : The function makes a string list component of an    */
/*                OR-address readable                                 */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_string_list(
    signed32 len,       /* IN    - length of stringlist               */
    byte *value)        /* IN    - sequence of strings                */

{                       /* gds_gst_print_string_list                  */

  byte       *component;/* component of string list                   */
  unsigned16 rep;       /* representation of one component            */


  for                   /* all components of string list              */
     (component = value; component - value < len; )
  {                     /* check the representation of the component  */
    rep = ((D21_str_list *)component)->d21_strl_rep;
    switch(rep)
    {
      case D2_PRINTABLE:
	printf("(%s) %.*s ",gst_printable_rep,
	  ((D21_str_list *)component)->d21_strl_len,
	  component + sizeof(D21_str_list));
	break;
      case D2_T61:
	printf("(%s) %.*s ",gst_t61_rep,
	  ((D21_str_list *)component)->d21_strl_len,
	  component + sizeof(D21_str_list));
	break;
      case D2_NUMERIC:
	printf("(%s) %.*s ",gst_numeric_rep,
	  ((D21_str_list *)component)->d21_strl_len,
	  component + sizeof(D21_str_list));
	break;
      default:            /* illegal representation                     */
	printf("%s: %d\n",gst_ill_rep,rep);
	gds_gst_print_binary_data(((D21_str_list *)component)->d21_strl_len,
	  component + sizeof(D21_str_list));
	break;
    }
    component += sizeof(D21_str_list) +
      ((D21_str_list *)component)->d21_strl_off;
  }

}                       /* gds_gst_print_string_list                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_acl                                   */
/*                                                                    */
/* Description  : The function makes an ACL readable                  */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_acl(
    signed32 len,       /* IN    - length of ACL in byte              */
    byte *value)        /* IN    - sequence of strings                */

{                       /* gds_gst_print_acl                          */

  byte *endval;         /* end of value                               */
  byte *endname;        /* end of distinguished name                  */
  register signed16 i;  /* loop variable                              */

  endval = value + len;

  for                   /* all ACL items                              */
     (i = 0; i < D2_ACLIDX_MAX; i++)
  {                     /* check the access right                     */
    if                  /* access right has changed                   */
      (i % D2_NOIDX_ACL == 0)
    {                   /* print the access right                     */
      printf("Access right: ");
      switch(i / D2_NOIDX_ACL)
      {
	case D2_MPUBLIC:
	  printf("Modify Public\n");
	  break;
	case D2_RSTANDARD:
	  printf("Read Standard\n");
	  break;
	case D2_MSTANDARD:
	  printf("Modify Standard\n");
	  break;
	case D2_RSENSITIVE:
	  printf("Read Sensitive\n");
	  break;
	case D2_MSENSITIVE:
	  printf("Modify Sensitive\n");
	  break;
	default:
				    /* can never happen               */
	  printf("Illegal access right: %d\n",i / D2_NOIDX_ACL);
	  break;
      }
    }
    switch(*value)
    {
      case D2_SOBJ:
	printf("Single object\n");
	break;
      case D2_SUBTREE:
	printf("Root of subtree\n");
	break;
      case D2_GROUP:
	printf("Name of group\n");
	break;
      case D2_IGNORE:
	printf("Ignored ACL item\n");
	break;
      default:
	printf("Illegal interpretation: %d\n",*value);
	break;
    }
    value++;

    if                  /* DN not terminated                          */
      ((endname = memchr(value,D2_EOS,endval - value)) == NULL)
    {                   /* print error indication                     */
      printf("Missing DN terminator in ACL item\n");
      gds_gst_print_binary_data(endval - value,value);
      break;
    }
    else
    {                   /* print distinguished name                   */
      gds_gst_print_dist_name(endname - value + 1,value);
      value = endname + 1;
    }
  }

}                       /* gds_gst_print_acl                          */
