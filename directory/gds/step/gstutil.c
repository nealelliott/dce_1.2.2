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
 * $Log: gstutil.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:58  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:46:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:48  root]
 * 
 * Revision 1.1.2.6  1994/09/06  20:41:30  bowe
 * 	Rework args to gds_gst_print_data() [CR 12055]
 * 	[1994/09/06  20:40:54  bowe]
 * 
 * Revision 1.1.2.5  1994/09/06  16:30:20  keutel
 * 	OT 12055: dce_svc_log_get_arg will become obsolete
 * 	[1994/09/06  16:10:46  keutel]
 * 
 * Revision 1.1.2.4  1994/06/21  14:48:08  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:45:38  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:16:10  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:16  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:03:13  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:48:01  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:23:12  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:47:14  keutel
 * 	creation
 * 	[1994/03/21  15:55:18  keutel]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : gstutil.c                                           */
/*                                                                    */
/* Description  : This file contains utility functions of the logfile */
/*                evaluation tool gdsstep of the GDS component, which */
/*                make the APDU messages readable. They prepare       */
/*                data, that may not be assigned to a particular      */
/*                message type.                                       */
/*                                                                    */
/* Date         : 12/15/93                                            */
/*                                                                    */
/* Author       : E. Kraemer, BUBA NM12                               */
/*                                                                    */
/**********************************************************************/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gstutil.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:58 $";
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

#include <d26dsa.h>

#include <gstep.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_op_id                                 */
/*                                                                    */
/* Description  : The function prints the operation ID readable       */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_op_id(
    const char *format, /* IN    - format string for printf           */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_op_id                        */

  char *form_elm;       /* pointer to format element                  */
  signed16 op_id;       /* operation ID                               */

  op_id     = size == sizeof(signed16) ? *(signed16 *)data : *(signed32 *)data;
  form_elm  = strchr(format,'%');
  *form_elm = '\0';
  printf(format);
  *form_elm = '%';

  if                    /* administration functionality               */
    (op_id > D2_ADM_OPID)
  {                     /* print adm prefix                           */
    printf(" ADM_");
    op_id -= D2_ADM_OPID;
  }

  switch(op_id)
  {
    case D23_READ:
      printf("READ");
      break;
    case D23_COMPARE:
      printf("COMPARE");
      break;
    case D23_ABANDON:
      printf("ABANDON");
      break;
    case D23_LIST:
      printf("LIST");
      break;
    case D23_SEARCH:
      printf("SEARCH");
      break;
    case D23_ADDOBJT:
      printf("ADD_ENTRY");
      break;
    case D23_REMOBJT:
      printf("REMOVE_ENTRY");
      break;
    case D23_MODENTR:
      printf("MODIFY_ENTRY");
      break;
    case D23_REPLRDN:
      printf("MODIFY_RDN");
      break;
    case D23_BIND:
      printf("BIND");
      break;
    case D23_UNBIND:
      printf("UNBIND");
      break;
    default:
      printf("illegal operation ID: %d",op_id);
      break;
  }

}                       /* gds_gst_print_op_id                        */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_entry                                 */
/*                                                                    */
/* Description  : The function prints an entry name in readable form  */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_entry(
    const char *format, /* IN    - format string for printf           */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_entry                        */

  char *form_elm;       /* pointer to format element                  */

  form_elm  = strchr(format,'%');
  *form_elm = '\0';
  printf(format);
  *form_elm = '%';

  printf("\n");
  gds_gst_print_dist_name((signed16)size,(byte *)data);

}                       /* gds_gst_print_entry                        */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_bo_filter                             */
/*                                                                    */
/* Description  : The function prints a filter attribute type, that   */
/*                is derived from the base object.                    */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_bo_filter(
    const char *format, /* IN    - format string for printf           */
    signed16   call_nr, /* IN    - number of repeated function call   */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_bo_filter                    */

  char *form_elm;       /* pointer to format element                  */

  if                    /* first call                                 */
    (call_nr == 0)
  {                     /* print the count of nameparts               */
    printf(format,size == sizeof(signed16) ? *(signed16 *)data :
      *(signed32 *)data);
  }
  else
  {                     /* print the object identifier                */
    form_elm  = strchr(format,'%');
    *form_elm = '\0';
    printf(format);
    *form_elm = '%';
    gds_gst_print_obj_ident((signed16)size,(byte *)data);
    printf("\n");
  }

}                       /* gds_gst_print_bo_filter                    */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_filter_type                           */
/*                                                                    */
/* Description  : The function prints a filter attribute type.        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_filter_type(
    const char *format, /* IN    - format string for printf           */
    signed16   call_nr, /* IN    - number of repeated function call   */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_filter                       */

  char *form_elm;       /* pointer to format element                  */

  if                    /* first call                                 */
    (call_nr == 0)
  {                     /* print the C-ISAM filename                  */
    printf(format,(char *)data);
  }
  else
  {                     /* print the object identifier                */
    form_elm  = strchr(format,'%');
    *form_elm = '\0';
    printf(format);
    *form_elm = '%';
    gds_gst_print_obj_ident((signed16)size,(byte *)data);
    printf("\n");
  }

}                       /* gds_gst_print_filter_type                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_name_filter                           */
/*                                                                    */
/* Description  : The function prints a filter attribute type of a    */
/*                naming attribute                                    */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_name_filter(
    const char *format, /* IN    - format string for printf           */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_name_filter                  */

  char *form_elm;       /* pointer to format element                  */

  form_elm  = strchr(format,'%');
  *form_elm = '\0';
  printf(format);
  *form_elm = '%';
  gds_gst_print_obj_ident((signed16)size,(byte *)data);
  printf("\n");

}                       /* gds_gst_print_name_filter                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_rv_intsearch                          */
/*                                                                    */
/* Description  : The function prints the return value of internal    */
/*                search readable                                     */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_rv_int_search(
    const char *format, /* IN    - format string for printf           */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_rv_int_search                */

  char *form_elm;       /* pointer to format element                  */
  signed16 ret_value;   /* return value of internal search            */

  ret_value = size == sizeof(signed16) ?
    *(signed16 *)data : *(signed32 *)data;
  form_elm  = strchr(format,'%');
  *form_elm = '\0';
  printf(format);
  *form_elm = '%';

  switch(ret_value)
  {
    case D2_NO_ERR:
      printf("NoError");
      break;
    case D2_NFOUND:
      printf("noSuchObject");
      break;
    case D2_NMSYNTAX_ERROR:
      printf("nameSyntaxError");
      break;
    case D2_ALIAS_ERROR:
      printf("aliasProblem");
      break;
    case D2_ON_ALIAS_NOT_ALLOWED:
      printf("aliasDereferencingProblem");
      break;
    case D2_ATT_ERROR:
      printf("noRequestedAttributeFound");
      break;
    case D2_UNDEFINED_ATTR_TYPE:
      printf("filterUndefined");
      break;
    case D2_ATTRIBUTE_MISSING:
      printf("noSuchAttribute");
      break;
    case D2_OBJ_CLS_VIOLATION:
      printf("filterObjectClassViolation");
      break;
    case D2_ACCESS_RIGHTS_INSUFFICIENT:
      printf("filterInsufficientAccessRights");
      break;
    case D26_NOFILTERMATCH:
      printf("noFilterMatch");
      break;
    case D2_REF_ERROR:
      printf("referral");
      break;
    case D2_DIT_INCONSISTENT:
      printf("ditError");
      break;
    case D2_UNABLE_TO_PROCEED:
      printf("unableToProceed");
      break;
    case D2_INVALID_REFERENCE:
      printf("invalidReference");
      break;
    case D2_TIME_LIMIT_EXCEEDED:
      printf("timeLimitExceeded");
      break;
    case D2_ADM_LIM_EXCEEDED:
      printf("administrativeLimitExceeded");
      break;
    case D2_LOOP_DETECTED:
      printf("loopDetected");
      break;
    case D2_UNAV_CRIT_EXTENSION:
      printf("unavailableCriticalExtension");
      break;
    case D2_OUT_OF_SCOPE:
      printf("outOfScope");
      break;
    case D2_ABANDONED:
      printf("abandoned");
      break;
    case D26_ERROR:
      printf("systemError");
      break;
    default:
      printf("illegal return value: %d",ret_value);
      break;
  }

}                       /* gds_gst_print_rv_int_search                */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_rv_fa_check                           */
/*                                                                    */
/* Description  : The function prints the return value of the filter  */
/*                attribute check readable                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_rv_fa_check(
    const char *format, /* IN    - format string for printf           */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_rv_fa_check                  */

  char *form_elm;       /* pointer to format element                  */
  signed16 ret_value;   /* return value of filter attribute check     */

  ret_value = size == sizeof(signed16) ?
    *(signed16 *)data : *(signed32 *)data;
  form_elm  = strchr(format,'%');
  *form_elm = '\0';
  printf(format);
  *form_elm = '%';

  switch(ret_value)
  {
    case D2_NO_ERR:
      printf("NoError");
      break;
    case D2_UNDEFINED_ATTR_TYPE:
      printf("filterUndefined");
      break;
    case D2_ATTRIBUTE_MISSING:
      printf("noSuchAttribute");
      break;
    case D2_ACCESS_RIGHTS_INSUFFICIENT:
      printf("filterInsufficientAccessRights");
      break;
    case D26_NOFILTERMATCH:
      printf("noFilterMatch");
      break;
    case D2_AT_VALUE_EXISTS:
      printf("attributeOrValueAlreadyExists");
      break;
    default:
      printf("illegal return value: %d",ret_value);
      break;
  }

}                       /* gds_gst_print_rv_fa_check                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_rv_acl_check                          */
/*                                                                    */
/* Description  : The function prints the return value of the ACL     */
/*                check readable                                      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_rv_acl_check(
    const char *format, /* IN    - format string for printf           */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_rv_acl_check                 */

  char *form_elm;       /* pointer to format element                  */
  signed16 ret_value;   /* return value of filter attribute check     */

  ret_value = size == sizeof(signed16) ?
    *(signed16 *)data : *(signed32 *)data;
  form_elm  = strchr(format,'%');
  *form_elm = '\0';
  printf(format);
  *form_elm = '%';

  switch(ret_value)
  {
    case D2_NO_ERR:
      printf("NoError");
      break;
    case D2_ACCESS_RIGHTS_INSUFFICIENT:
      printf("InsufficientAccessRights");
      break;
    default:
      printf("illegal return value: %d",ret_value);
      break;
  }

}                       /* gds_gst_print_rv_acl_check                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_type_and_rep                          */
/*                                                                    */
/* Description  : The function prints a filter attribute type and an  */
/*                representation.                                     */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_type_and_rep(
    const char *format, /* IN    - format string for printf           */
    signed16   call_nr, /* IN    - number of repeated function call   */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_type_and_rep                 */

  char *form_elm;       /* pointer to format element                  */
  unsigned16 rep;       /* representation                             */

  if                    /* first call                                 */
    (call_nr == 0)
  {                     /* print the object identifier                */
    form_elm  = strchr(format,'%');
    *form_elm = '\0';
    printf(format);
    *form_elm = '%';
    gds_gst_print_obj_ident((signed16)size,(byte *)data);
    printf(" ");
  }
  else
  {                     /* print the representation                   */
    rep = (unsigned16)(size == sizeof(signed16) ?
      *(signed16 *)data : *(signed32 *)data);
    gds_gst_print_representation(rep);
  }

}                       /* gds_gst_print_type_and_rep                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_type_and_pos                          */
/*                                                                    */
/* Description  : The function prints a filter attribute type and a   */
/*                substrings position.                                */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_type_and_pos(
    const char *format, /* IN    - format string for printf           */
    signed16   call_nr, /* IN    - number of repeated function call   */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_type_and_pos                 */

  char *form_elm;       /* pointer to format element                  */
  signed16 position;    /* position                                   */

  form_elm  = strchr(format,'%');
  *form_elm = '\0';
  printf(format);
  *form_elm = '%';
  if                    /* first call                                 */
    (call_nr == 0)
  {                     /* print the object identifier                */
    gds_gst_print_obj_ident((signed16)size,(byte *)data);
  }
  else
  {                     /* print the position                         */
    printf(format);
    position = (unsigned16)(size == sizeof(signed16) ?
      *(signed16 *)data : *(signed32 *)data);
    switch(position)
    {
      case D2_INITIAL:
	printf("initial");
	break;
      case D2_ANY:
	printf("any");
	break;
      case D2_FINAL:
	printf("final");
	break;
      default:
	printf("Illegal substrings position: %d",position);
	break;
    }
  }

}                       /* gds_gst_print_type_and_pos                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_int_and_name                          */
/*                                                                    */
/* Description  : The function prints an integer and a distinguished  */
/*                name.                                               */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_int_and_name(
    const char *format, /* IN    - format string for printf           */
    signed16   call_nr, /* IN    - number of repeated function call   */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_int_and_name                 */

  signed16 int_data;    /* integer data                               */

  if                    /* first call                                 */
    (call_nr == 0)
  {                     /* print the integer data                     */
    int_data = (int)(size == sizeof(signed16) ?
      *(signed16 *)data : *(signed32 *)data);
    printf(format,int_data);
  }
  else
  {                     /* print the entry                            */
    gds_gst_print_entry(format,size,data);
  }

}                       /* gds_gst_print_int_and_name                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_int_and_syntax                        */
/*                                                                    */
/* Description  : The function prints an integer and an attribute     */
/*                syntax.                                             */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_int_and_syntax(
    const char *format, /* IN    - format string for printf           */
    signed16   call_nr, /* IN    - number of repeated function call   */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_int_and_syntax               */

  char *form_elm;       /* pointer to format element                  */
  signed16 int_data;    /* integer data                               */

  int_data = (int)(size == sizeof(signed16) ?
    *(signed16 *)data : *(signed32 *)data);
  if                    /* first call                                 */
    (call_nr == 0)
  {                     /* print the integer data                     */
    printf(format,int_data);
  }
  else
  {                     /* print the syntax                           */
    form_elm  = strchr(format,'%');
    *form_elm = '\0';
    printf(format);
    *form_elm = '%';
    switch(int_data)
    {
      case D2_DISTNAME:
	printf(gst_dist_name_rep);
	break;
      case D2_OBJ_IDENT:
	printf("%s",gst_obj_ident_rep);
	break;
      case D2_PRINTABLE:
	printf("%s",gst_printable_rep);
	break;
      case D2_CEXACT:
	printf("caseExact");
	break;
      case D2_CIGNORE:
	printf("caseIgnore");
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
      case D2_POST_AD_STX:
	printf("postalAddress");
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
      case D2_PR_DM_STX:
	printf("preferredDeliveryMethod");
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
      case D2_MHS_PR_DM_STX:
	printf("MHSpreferredDeliveryMethod");
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
	printf("Unknown attribute syntax: %d",int_data);
	break;
    }
  }

}                       /* gds_gst_print_int_and_syntax               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_rep_len_value                         */
/*                                                                    */
/* Description  : The function prints the representation, the length  */
/*                and the value of an attribute                       */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_rep_len_value(
    const char *format, /* IN    - format string for printf           */
    signed16   call_nr, /* IN    - number of repeated function call   */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_rep_len_value                */

  char *form_elm;       /* pointer to format element                  */
  int  int_data;        /* integer data                               */

  switch(call_nr)
  {
    case 0:             /* representation                             */
      int_data = (int)(size == sizeof(signed16) ?
	*(signed16 *)data : *(signed32 *)data);
      form_elm  = strchr(format,'%');
      *form_elm = '\0';
      printf(format);
      *form_elm = '%';
      gds_gst_print_representation((unsigned16)int_data);
      break;
    case 1:             /* length of attribute value                  */
      int_data = (int)(size == sizeof(signed16) ?
	*(signed16 *)data : *(signed32 *)data);
      printf(format,int_data);
      break;
    case 2:
      form_elm  = strchr(format,'%');
      *form_elm = '\0';
      printf(format);
      *form_elm = '%';
      gds_gst_print_binary_data(size,data);
      break;
  }

}                       /* gds_gst_print_rep_len_value                */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_dsa_configuration                     */
/*                                                                    */
/* Description  : The function prints the DSA configuration           */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_dsa_configuration(
    const char *format, /* IN    - format string for printf           */
    signed16   call_nr, /* IN    - number of repeated function call   */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_dsa_configuration            */

  char *form_elm;       /* pointer to format element                  */
  signed16 int_data;    /* integer data                               */

  int_data = (int)(size == sizeof(signed16) ?
      *(signed16 *)data : *(signed32 *)data);
  switch(call_nr)
  {
    case 0:             /* chaining policy                            */
      form_elm  = strchr(format,'%');
      *form_elm = '\0';
      printf(format);
      *form_elm = '%';
      switch(int_data)
      {
	case D26_SINGLE_HOP:
	  printf("SingleHop");
	  break;
	case D26_MULTI_HOP:
	  printf("MultiHop");
	  break;
	default:
	  printf("Illegal chaining policy: %d",int_data);
	  break;
      }
      break;
    case 1:             /* routing capability                         */
      form_elm  = strchr(format,'%');
      *form_elm = '\0';
      printf(format);
      *form_elm = '%';
      switch(int_data)
      {
	case TRUE:
	  printf(gst_true);
	  break;
	case FALSE:
	  printf(gst_false);
	  break;
	default:
	  printf("Illegal routing capability: %d",int_data);
	  break;
      }
    case 2:             /* knowledge priority                         */
    case 3:             /* routing priority                           */
      printf(format,int_data);
      break;
    case 4:             /* DSP unbind delay                           */
      printf(format,(long)int_data);
      break;
  }
  printf(" ");

}                       /* gds_gst_print_dsa_configuration            */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gds_dce_svc_log_get_arg		    	   */
/* Description   : This function is used to return the argtypes    */
/*		   from the prolog structure.			   */
/* Return Value  : NONE. The successful completion or otherwise    */
/*		   is indicated in the output parameter "status"   */
/* Author        : B T Naik, SNI BU BA NM123.	   		   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gds_dce_svc_log_get_arg(
    dce_svc_log_prolog_t   prolog,     /* I/O - prolog                   */
    unsigned32 		  *arg_index,  /* I/O - arg index                */
    pointer_t  		  *data,       /* OUT - The argtype data         */
    unsigned32 		  *size,       /* OUT - The size of the argtypes */
    error_status_t        *status)     /* OUT - Status of return code    */
{
svc_bin_arg_t  *argtype;

    if (*arg_index >= prolog->n_args) {
	*size = 0;
	*data = NULL;
	*status = svc_s_at_end;
	return;
    }
    argtype = &(prolog->args[*arg_index]);

    switch (argtype->bin_encoding) 
    {
	case (svc_bin_type_int):
	    *size = sizeof (int);
	    *data = &(argtype->tagged_union.i);
	    break;
	case (svc_bin_type_long):
	    *size = sizeof (long);
	    *data = &(argtype->tagged_union.l);
	    break;
	case (svc_bin_type_uint):
	    *size = sizeof (unsigned int);
	    *data = &(argtype->tagged_union.i);
	    break;
	case (svc_bin_type_ulong):
	    *size = sizeof (unsigned long);
	    *data = &(argtype->tagged_union.ul);
	    break;
	case (svc_bin_type_string):
	    *size = strlen((char *)argtype->tagged_union.s) + 1;
	    *data = argtype->tagged_union.s;
	    break;
	case (svc_bin_type_ptr):
	    *size = sizeof (void *);
	    *data = &(argtype->tagged_union.v);
	    break;
	case (svc_bin_type_double):
	    *size = sizeof (double);
	    *data = &(argtype->tagged_union.d);
	    break;
	case (svc_bin_type_char):
	    *size = sizeof (char);
	    *data = &(argtype->tagged_union.c);
	    break;
	case (svc_bin_type_bytes):
	    *size = argtype->tagged_union.bytes.size;
	    *data = argtype->tagged_union.bytes.data;
	    break;
    }

    ++(*arg_index);
    *status = svc_s_ok;
}
