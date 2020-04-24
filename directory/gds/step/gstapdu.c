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
 * $Log: gstapdu.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:48  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:19  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:46:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:32  root]
 * 
 * Revision 1.1.2.6  1994/08/24  12:15:04  keutel
 * 	OT 11802
 * 	[1994/08/23  16:05:47  keutel]
 * 
 * Revision 1.1.2.5  1994/07/19  14:48:59  marrek
 * 	HP warnings removed.
 * 	[1994/07/19  12:13:10  marrek]
 * 
 * Revision 1.1.2.4  1994/06/21  14:48:04  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:45:17  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:15:52  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:04  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:03:06  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  15:05:58  marrek]
 * 
 * 	Bug fixes and I18N for April 1994 submission.
 * 	[1994/05/06  09:57:53  marrek]
 * 
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:47:23  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:22:30  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:11:19  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:47:01  keutel
 * 	creation
 * 	[1994/03/21  15:54:50  keutel]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : gstapdu.c                                           */
/*                                                                    */
/* Description  : This file contains the functions of the logfile     */
/*                evaluation tool gdsstep of the GDS component, which */
/*                make the APDU messages readable.                    */
/*                                                                    */
/* Date         : 12/15/93                                            */
/*                                                                    */
/* Author       : E. Kraemer, BUBA NM12                               */
/*                                                                    */
/**********************************************************************/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gstapdu.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:48 $";
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
#include <d27util.h>
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

/**** EXPORT ****/

				    /* boolean values                 */
char *gst_true  = "TRUE";
char *gst_false = "FALSE";

				    /* Filter tags                    */
char *gst_item       = "ITEM";
char *gst_and        = "AND";
char *gst_or         = "OR";
char *gst_not        = "NOT";
char *gst_ill_filter = "Illegal filter";

				    /* Matching rules                 */
char *gst_equal              = "EQUALITY";
char *gst_substrings         = "SUBSTRINGS";
char *gst_greater_or_equal   = "GREATEROREQUAL";
char *gst_less_or_equal      = "LESSOREQUAL";
char *gst_present            = "PRESENT";
char *gst_approximate_match  = "APPROXIMATEMATCH";

/**** LOCAL  ****/
				    /* tags                           */
static char *gst_ill_tag            = "Illegal tag";
static char *gst_ill_len            = "Illegal length";
static char *gst_invcred            = "Invalid credentials tag: %d\n";

				    /* ASN1 basic elements            */
static char *gst_bit_string         = "BIT STRING";

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_bind_request(byte **message, signed32 *size);
static void gds_gst_print_unbind_request(byte **message, signed32 *size);
static void gds_gst_print_read_request(byte **message, signed32 *size);
static void gds_gst_print_compare_request(byte **message, signed32 *size);
static void gds_gst_print_abandon_request(byte **message, signed32 *size);
static void gds_gst_print_list_request(byte **message, signed32 *size);
static void gds_gst_print_search_request(byte **message, signed32 *size);
static void gds_gst_print_add_request(byte **message, signed32 *size);
static void gds_gst_print_remove_request(byte **message, signed32 *size);
static void gds_gst_print_modify_request(byte **message, signed32 *size);
static void gds_gst_print_modrdn_request(byte **message, signed32 *size);

static void gds_gst_print_bind_result(byte **message, signed32 *size);
static void gds_gst_print_unbind_result(byte **message, signed32 *size);
static void gds_gst_print_read_result(byte **message, signed32 *size);
static void gds_gst_print_compare_result(byte **message, signed32 *size);
static void gds_gst_print_abandon_result(byte **message, signed32 *size);
static void gds_gst_print_list_result(byte **message, signed32 *size);
static void gds_gst_print_search_result(byte **message, signed32 *size);
static void gds_gst_print_add_result(byte **message, signed32 *size);
static void gds_gst_print_remove_result(byte **message, signed32 *size);
static void gds_gst_print_modify_result(byte **message, signed32 *size);
static void gds_gst_print_modrdn_result(byte **message, signed32 *size);

static void gds_gst_print_apdu_header(byte **message, signed32 *size);

static void gds_gst_print_simple_credentials(byte **message, signed32 *size);
static void gds_gst_print_strong_credentials(byte **message, signed32 *size);
static void gds_gst_print_extern_credentials(byte **message, signed32 *size);
static void gds_gst_print_validity(byte **message, signed32 *size);
static void gds_gst_print_password(byte **message, signed32 *size);

static void gds_gst_print_chaining_arg(byte **message, signed32 *size);
static void gds_gst_print_operation_prog(D23_op_pr_info *op_prog);
static void gds_gst_print_trace_info(byte **message, signed32 *size);
static void gds_gst_print_chaining_res(byte **message, signed32 *size);
static void gds_gst_print_cross_ref(byte **message, signed32 *size);
static void gds_gst_print_access_point(byte **message, signed32 *size);
static void gds_gst_print_part_oq(byte **message, signed32 *size);
static void gds_gst_print_referral(byte **message, signed32 *size);
static void gds_gst_print_dsa_referral(byte **message, signed32 *size);

static void gds_gst_print_common_arg(byte **message, signed32 *size);
static void gds_gst_print_serv_control(D23_serv_ctrl *serv_ctrl);
static void gds_gst_print_common_res(byte **message, signed32 *size);

static void gds_gst_print_en_info(byte **message, signed32 *size);
static void gds_gst_print_ei_select(byte **message, signed32 *size);
static void gds_gst_print_at_info(byte **message, signed32 *size);
static void gds_gst_print_mod_info(byte **message, signed32 *size);
static void gds_gst_print_atype(byte **message, signed32 *size);
static void gds_gst_print_avinfo(byte **message, signed32 *size);
static void gds_gst_print_rep_info(byte **message, signed32 *size);
static void gds_gst_print_rscomp_info(byte **message, signed32 *size);
static void gds_gst_print_so_info(byte **message, signed32 *size);

static void gds_gst_print_search_arg(byte **message, signed32 *size);
static void gds_gst_print_filter(byte **message, signed32 *size);
static void gds_gst_print_filter_item(byte **message, signed32 *size);

static void gds_gst_print_at_problem(byte **message, signed32 *size);

static void gds_gst_print_eom(byte **message, signed32 *size);

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_apdu_request                          */
/*                                                                    */
/* Description  : The function makes an APDU request readable. The    */
/*                first call prints and stores the operation ID, the  */
/*                second call prepares the APDU.                      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_apdu_request(
    const char *format, /* IN    - format string for printf           */
    signed16   parcount,/* IN    - number of parameter in list        */
    signed32   size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_apdu_request                 */

  static signed16 op_id;/* operation ID                               */

  byte       *message;  /* APDU message                               */

  if                    /* first call                                 */
    (parcount == 0)
  {                     /* store the operation ID                     */
    op_id = size == sizeof(signed16) ? *(signed16 *)data : *(signed32 *)data;
    gds_gst_print_op_id(format,size,data);
    printf("\n");
  }
  else
  {                     /* check operation ID                         */
    message = (byte *)data;
    printf("Message size: %ld, request message:\n",size);
    if                    /* administration functionality               */
      (op_id > D2_ADM_OPID)
    {                     /* decrease by D2_ADM_OPID			*/
      op_id -= D2_ADM_OPID;
    }
    switch(op_id)
    {
      case D23_BIND:
	gds_gst_print_bind_request(&message,&size);
	break;
      case D23_UNBIND:
	gds_gst_print_unbind_request(&message,&size);
	break;
      case D23_READ:
	gds_gst_print_read_request(&message,&size);
	break;
      case D23_COMPARE:
	gds_gst_print_compare_request(&message,&size);
	break;
      case D23_ABANDON:
	gds_gst_print_abandon_request(&message,&size);
	break;
      case D23_LIST:
	gds_gst_print_list_request(&message,&size);
	break;
      case D23_SEARCH:
	gds_gst_print_search_request(&message,&size);
	break;
      case D23_ADDOBJT:
	gds_gst_print_add_request(&message,&size);
	break;
      case D23_REMOBJT:
	gds_gst_print_remove_request(&message,&size);
	break;
      case D23_MODENTR:
	gds_gst_print_modify_request(&message,&size);
	break;
      case D23_REPLRDN:
	gds_gst_print_modrdn_request(&message,&size);
	break;
      default:
	gds_gst_print_binary_data(size,message);
	message += size;
	break;
    }
    if                  /* still message to prepare                   */
      (size > 0)
    {                   /* print rest of message in binary format     */
      printf("Trailing bytes in message\n");
      gds_gst_print_binary_data(size,message);
      message += size;
      size     = 0;
    }
  }

}                       /* gds_gst_print_apdu_request                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_apdu_result                           */
/*                                                                    */
/* Description  : The function makes an APDU result readable. The     */
/*                first call prints and stores the operation ID, the  */
/*                second call prepares the APDU.                      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_apdu_result(
    const char *format, /* IN    - format string for printf           */
    signed16   parcount,/* IN    - number of parameter in list        */
    signed32   size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_apdu_result                  */

  static signed16 op_id;/* operation ID                               */

  byte       *message;  /* APDU message                               */

  if                    /* first call                                 */
    (parcount == 0)
  {                     /* store the operation ID                     */
    op_id = size == sizeof(signed16) ? *(signed16 *)data : *(signed32 *)data;
    gds_gst_print_op_id(format,size,data);
    printf("\n");
  }
  else
  {                     /* check operation ID                         */
    message = (byte *)data;
    printf("Message size: %ld, result message:\n",size);
    if                    /* administration functionality               */
      (op_id > D2_ADM_OPID)
    {                     /* decrease by D2_ADM_OPID			*/
      op_id -= D2_ADM_OPID;
    }
    switch(op_id)
    {
      case D23_BIND:
	gds_gst_print_bind_result(&message,&size);
	break;
      case D23_UNBIND:
	gds_gst_print_unbind_result(&message,&size);
	break;
      case D23_READ:
	gds_gst_print_read_result(&message,&size);
	break;
      case D23_COMPARE:
	gds_gst_print_compare_result(&message,&size);
	break;
      case D23_ABANDON:
	gds_gst_print_abandon_result(&message,&size);
	break;
      case D23_LIST:
	gds_gst_print_list_result(&message,&size);
	break;
      case D23_SEARCH:
	gds_gst_print_search_result(&message,&size);
	break;
      case D23_ADDOBJT:
	gds_gst_print_add_result(&message,&size);
	break;
      case D23_REMOBJT:
	gds_gst_print_remove_result(&message,&size);
	break;
      case D23_MODENTR:
	gds_gst_print_modify_result(&message,&size);
	break;
      case D23_REPLRDN:
	gds_gst_print_modrdn_result(&message,&size);
	break;
      default:
	gds_gst_print_binary_data(size,message);
	message += size;
	size     = 0;
	break;
    }
    if                  /* still message to prepare                   */
      (size > 0)
    {                   /* print rest of message in binary format     */
      printf("Trailing bytes in message\n");
      gds_gst_print_binary_data(size,message);
      message += size;
      size     = 0;
    }
  }

}                       /* gds_gst_print_apdu_result                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_apdu_error                            */
/*                                                                    */
/* Description  : The function makes an APDU error readable.          */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_apdu_error(
    signed32   size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_apdu_error                   */

  static char *name_error      = "NameError";
  static char *update_error    = "UpdateError";
  static char *attribute_error = "AttributeError";
  static char *security_error  = "SecurityError";
  static char *service_error   = "ServiceError";
  static char *abandon_failed  = "AbandonFailed";
  static char *abandoned       = "Abandoned";
  static char *referral        = "Referral";

  byte        *message; /* APDU message                               */
  D23_rserror *rserror; /* APDU RSerror structure                     */
  enum {
	  NameError,
	  UpdateError,
	  AttributeError,
	  SecurityError,
	  ServiceError,
	  AbandonFailed,
	  Abandoned,
	  Referral,
	  NonStandard
	} error;

  message = (byte *)data;
  printf("Message size: %ld, error message:\n",size);

  if                    /* message is too short                       */
    (sizeof(D23_rserror) > size)
  {                     /* print illegal message length               */
    printf("%s: %d of ERROR structure\n",gst_ill_len,sizeof(D23_rserror));
    gds_gst_print_binary_data(size,message);
    message += size;
    size     = 0;
  }
  else
  {                     /* print error structure                      */
    rserror = (D23_rserror *)message;
    printf("Error header, Version: %d ",rserror->d23_Zversion);
    switch(rserror->d23_Zretcod)
    {
      case D2_NO_ERR:
	printf("No error ");
	break;
      case D2_TEMP_ERR:
	printf("Temporary error ");
	break;
      case D2_CALL_ERR:
	printf("Call error ");
	break;
      case D2_PERM_ERR:
	printf("Permanent error ");
	break;
      case D2_WARNING:
	printf("Warning ");
	break;
      default:
	printf("Unknown: %d ",rserror->d23_Zretcod);
	break;
    }
    printf("Error class: ");
    switch(rserror->d23_Zerrcls)
    {
      case D2_NOT_SPEC:
	printf("Not specified ");
	break;
      case D2_PAR_ERR:
	printf("Parameter error ");
	break;
      case D2_ILL_STAT:
	printf("Invalid state error ");
	break;
      case D2_ILL_VERS:
	printf("Illegal Version error ");
	break;
      case D2_SYS_ERR:
	printf("System error ");
	break;
      case D2_REM_ERR:
	printf("Remote error ");
	break;
      case D2_SEC_ERR:
	printf("Security error ");
	break;
      case D2_INT_ERR:
	printf("Internal error ");
	break;
      case D2_MESSAGE:
	printf("message ");
	break;
      default:
	printf("Unknown: %d ",rserror->d23_Zerrval);
	break;
    }
    printf("Error value: ");
    switch(rserror->d23_Zerrval)
    {
      case D2_REF_ERROR:
	error = Referral;
	printf("%s",referral);
	break;
      case D2_ATT_ERROR:
	error = AttributeError;
	printf("%s",attribute_error);
	break;
      case D2_CREDENTIALS_INVALID:
	error = SecurityError;
	printf("%s: invalidCredentials",security_error);
	break;
      case D2_INAPPR_AUTH:
	error = SecurityError;
	printf("%s: inappropriateAuthentication",security_error);
	break;
      case D2_INVALID_SIGNATURE:
	error = SecurityError;
	printf("%s: invalidSignature",security_error);
	break;
      case D2_PROTECTION_REQUIRED:
	error = SecurityError;
	printf("%s: protectionRequired",security_error);
	break;
      case D2_ACCESS_RIGHTS_INSUFFICIENT:
	error = SecurityError;
	printf("%s: insufficientAccessRights",security_error);
	break;
      case D2_NFOUND:
	error = NameError;
	printf("%s: noSuchObject",name_error);
	break;
      case D2_ON_ALIAS_NOT_ALLOWED:
	error = NameError;
	printf("%s: aliasDereferencingProblem",name_error);
	break;
      case D2_NMSYNTAX_ERROR:
	error = NameError;
	printf("%s: invalidAttributeSyntax",name_error);
	break;
      case D2_ALIAS_ERROR:
	error = NameError;
	printf("%s: aliasProblem",name_error);
	break;
      case D2_TIME_LIMIT_EXCEEDED:
	error = ServiceError;
	printf("%s: timeLimitExceeded",service_error);
	break;
      case D2_TOO_BUSY:
	error = ServiceError;
	printf("%s: Busy",service_error);
	break;
      case D2_NO_RESPONSE:
	error = ServiceError;
	printf("%s: unavailable",service_error);
	break;
      case D2_UNWILLING:
	error = ServiceError;
	printf("%s: unwillingToPerform",service_error);
	break;
      case D2_CHAINING_REQUIRED:
	error = ServiceError;
	printf("%s: chainingRequired",service_error);
	break;
      case D2_UNABLE_TO_PROCEED:
	error = ServiceError;
	printf("%s: unableToProceed",service_error);
	break;
      case D2_INVALID_REFERENCE:
	error = ServiceError;
	printf("%s: invalidReference",service_error);
	break;
      case D2_ADM_LIM_EXCEEDED:
	error = ServiceError;
	printf("%s: administrativeLimitExceeded",service_error);
	break;
      case D2_LOOP_DETECTED:
	error = ServiceError;
	printf("%s: loopDetected",service_error);
	break;
      case D2_UNAV_CRIT_EXTENSION:
	error = ServiceError;
	printf("%s: unavailableCriticalExtension",service_error);
	break;
      case D2_OUT_OF_SCOPE:
	error = ServiceError;
	printf("%s: outOfScope",service_error);
	break;
      case D2_DIT_INCONSISTENT:
	error = ServiceError;
	printf("%s: ditError",service_error);
	break;
      case D2_ORPHAN:
	error = UpdateError;
	printf("%s: notAllowedOnNonleaf",update_error);
	break;
      case D2_ALREADY_EXISTS:
	error = UpdateError;
	printf("%s: entryAlreadyExists",update_error);
	break;
      case D2_NOT_ALLOWED_ON_RDN:
	error = UpdateError;
	printf("%s: notAllowedOnRDN",update_error);
	break;
      case D2_NAMING_VIOLATION:
	error = UpdateError;
	printf("%s: namingViolation",update_error);
	break;
      case D2_OBJCLS_MOD_PROHIBITED:
	error = UpdateError;
	printf("%s: objectClassModificationProhibited",update_error);
	break;
      case D2_AFFECTS_MULTIPLE_DSAS:
	error = UpdateError;
	printf("%s: affectsMultipleDSAs",update_error);
	break;
      case D2_OBJ_CLS_VIOLATION:
	error = UpdateError;
	printf("%s: objectClassViolation",update_error);
	break;
      case D2_NO_SUCH_OPERATION:
	error = AbandonFailed;
	printf("%s: noSuchOperation",abandon_failed);
	break;
      case D2_TOO_LATE:
	error = AbandonFailed;
	printf("%s: tooLate",abandon_failed);
	break;
      case D2_CANNOT_ABANDON:
	error = AbandonFailed;
	printf("%s: cannotAbandon",abandon_failed);
	break;
      case D2_ABANDONED:
	error = Abandoned;
	printf("%s ",abandoned);
	break;
      default:
	error = NonStandard;
	printf("Unknown: %d ",rserror->d23_Zretcod);
	break;
    }
    message += sizeof(D23_rserror);
    size    -= sizeof(D23_rserror);
    printf("\n");
    switch(error)
    {
      case NameError:
	printf("matched part:\n");
	gds_gst_print_dn_struct(&size,&message);
	break;
      case UpdateError:
      case SecurityError:
      case ServiceError:
      case Abandoned:
      case NonStandard:
	break;
      case AttributeError:
	printf("object:\n");
	gds_gst_print_dn_struct(&size,&message);
	while           /* attribute problem is following             */
	     (size > 0 && ((D23_at_prob *)message)->d23_aptag == D23_APTAG)
	{               /* print the attribute problem                */
	  gds_gst_print_at_problem(&message,&size);
	}
	break;
      case AbandonFailed:
	if              /* still message to prepare                   */
	  (sizeof(signed32) <= size)
	{               /* print invoke ID                            */
	  printf("InvokeId: %ld",*(signed32 *)message);
	  message += sizeof(signed32);
	  size    -= sizeof(signed32);
	  printf("\n");
	}
	break;
      case Referral:
	if              /* DSA referral is following                  */
	  (sizeof(D23_drf_info) <= size &&
	   ((D23_drf_info *)message)->d23_drftag == D23_DRFTAG)
	{               /* print DSA referral                         */
	  gds_gst_print_dsa_referral(&message,&size);
	}
	else if         /* referral is following                      */
	       (sizeof(D23_rf_info) <= size &&
		((D23_rf_info *)message)->d23_rftag == D23_REFTAG)
	{               /* print referral                             */
	  gds_gst_print_referral(&message,&size);
	}
	else
	{               /* print message in binary format             */
	  printf("Referral missing\n");
	  gds_gst_print_binary_data(size,message);
	  message += size;
	  size     = 0;
	}
	break;
    }
    if                  /* still message to prepare                   */
      (size > 0)
    {                   /* print rest of message in binary format     */
      gds_gst_print_eom(&message,&size);
    }
    if                  /* still message to prepare                   */
      (size > 0)
    {                   /* print rest of message in binary format     */
      printf("Trailing bytes in message\n");
      gds_gst_print_binary_data(size,message);
      message += size;
      size     = 0;
    }
  }

}                       /* gds_gst_print_apdu_error                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_bind_request                          */
/*                                                                    */
/* Description  : The function makes a BIND request readable.         */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_bind_request(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_bind_request                 */

  byte       *psap;     /* pointer to PSAP address                    */
  signed16   crkind;    /* kind of credentials                        */
  unsigned16 cr_tag;    /* tag of credential structure                */

  if                    /* message is too short                       */
    (sizeof(D23_inbind) > *size)
  {                     /* print illegal message length               */
    printf("%s: %d of BIND header\n",gst_ill_len,sizeof(D23_inbind));
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }
  else
  {                     /* print header information                   */
    printf("Bind header, APDU-Version: %d, Dir-Id: %d\n",
      ((D23_inbind *)*message)->d23_Aversno,
      ((D23_inbind *)*message)->d23_Adirid);
    crkind = ((D23_inbind *)*message)->d23_Acrkind;
    *message += sizeof(D23_inbind);
    *size    -= sizeof(D23_inbind);
    switch(crkind)
    {
      case D23_NO_CRED:
	printf("No credentials\n");
	break;
      case D23_SIMPLE_CRED:
	cr_tag = ((D23_simcr_info *)*message)->d23_scrtag;
	if              /* simple credentials tag found               */
	  (cr_tag == D23_SIMTAG)
	{               /* print simple credentials                   */
	  gds_gst_print_simple_credentials(message,size);
	}
	else
	{               /* print simple credentials missing           */
	  printf(gst_invcred,cr_tag);
	  gds_gst_print_binary_data(*size,*message);
	  *message += *size;
	  *size     = 0;
	}
	break;
      case D23_STRONG_CRED:
	cr_tag = ((D23_strcr_info *)*message)->d23_strtag;
	if              /* strong credentials tag found               */
	  (cr_tag == D23_STRTAG)
	{               /* print strong credentials                   */
	  gds_gst_print_strong_credentials(message,size);
	}
	else
	{               /* print strong credentials missing           */
	  printf(gst_invcred,cr_tag);
	  gds_gst_print_binary_data(*size,*message);
	  *message += *size;
	  *size     = 0;
	}
	break;
      case D23_EXTERNAL_CRED:
	cr_tag = ((D23_extcr_info *)*message)->d23_exttag;
	if              /* extern credentials tag found               */
	  (cr_tag == D23_EXTTAG)
	{               /* print extern credentials                   */
	  gds_gst_print_extern_credentials(message,size);
	}
	else
	{               /* print simple credentials missing           */
	  printf(gst_invcred,cr_tag);
	  gds_gst_print_binary_data(*size,*message);
	  *message += *size;
	  *size     = 0;
	}
	break;
      default:
	printf("Unknown credentials type: %d\n",crkind);
	gds_gst_print_binary_data(*size,*message);
	*message += *size;
	*size     = 0;
	break;
    }

    if                  /* EOM not reached                            */
      (*size > 0 && ((D23_eom *)*message)->d23_eomtag != D23_EOMTAG)
    {                   /* print PSAP address                         */
      psap = *message;
      gds_gst_print_attr_value(message);
      *size -= (*message - psap);
    }
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_bind_request                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_unbind_request                        */
/*                                                                    */
/* Description  : The function makes an UNBIND request readable.      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_unbind_request(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_unbind_request               */

  gds_gst_print_apdu_header(message,size);
  gds_gst_print_eom(message,size);

}                       /* gds_gst_print_unbind_request               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_read_request                          */
/*                                                                    */
/* Description  : The function makes a READ request readable.         */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_read_request(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_read_request                 */

  gds_gst_print_apdu_header(message,size);
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining arguments                   */
    gds_gst_print_chaining_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common arguments                     */
    gds_gst_print_common_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print object                               */
    printf("object:\n");
    gds_gst_print_dn_struct(size,message);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print entry information selection          */
    gds_gst_print_ei_select(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print end of message structure             */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_read_request                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_compare_request                       */
/*                                                                    */
/* Description  : The function makes a COMPARE request readable.      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_compare_request(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_compare_request              */

  gds_gst_print_apdu_header(message,size);
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining arguments                   */
    gds_gst_print_chaining_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common arguments                     */
    gds_gst_print_common_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print object name                          */
    printf("object:\n");
    gds_gst_print_dn_struct(size,message);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print purported attribute                  */
    printf("purported:\n");
    gds_gst_print_at_info(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print end of message structure             */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_compare_request              */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_abandon_request                       */
/*                                                                    */
/* Description  : The function makes a ABANDON request readable.      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_abandon_request(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_abandon_request              */

  gds_gst_print_apdu_header(message,size);
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print invoke ID                            */
    printf("Invoke ID: %ld\n",*(signed32 *)*message);
    *message += sizeof(signed32);
    *size    -= sizeof(signed32);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print end of message structure             */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_abandon_request              */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_list_request                          */
/*                                                                    */
/* Description  : The function makes a LIST request readable.         */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_list_request(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_list_request                 */

  gds_gst_print_apdu_header(message,size);
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining arguments                   */
    gds_gst_print_chaining_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common arguments                     */
    gds_gst_print_common_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print object arguments                     */
    printf("object:\n");
    gds_gst_print_dn_struct(size,message);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print eond of message structure            */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_list_request                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_search_request                        */
/*                                                                    */
/* Description  : The function makes a SEARCH request readable.       */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_search_request(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_search_request               */

  gds_gst_print_apdu_header(message,size);
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining arguments                   */
    gds_gst_print_chaining_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common arguments                     */
    gds_gst_print_common_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print search arguments                     */
    gds_gst_print_search_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print base object                          */
    printf("baseObject:\n");
    gds_gst_print_dn_struct(size,message);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print entry information selection          */
    gds_gst_print_ei_select(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print filter information                   */
    gds_gst_print_filter(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print end of message structure             */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_search_request               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_add_request                           */
/*                                                                    */
/* Description  : The function makes an ADD request readable.         */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_add_request(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_add_request                  */

  gds_gst_print_apdu_header(message,size);
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining arguments                   */
    gds_gst_print_chaining_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common arguments                     */
    gds_gst_print_common_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print object                               */
    printf("object:\n");
    gds_gst_print_dn_struct(size,message);
  }
  printf("attributes:\n");
  while                 /* still attribute information following      */
       (*size > sizeof(D23_at_type) &&
	((D23_at_type *)*message)->d23_atttag == D23_ATTTAG)
  {                     /* print attribute information                */
    gds_gst_print_at_info(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print endo of message structure            */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_add_request                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_remove_request                        */
/*                                                                    */
/* Description  : The function makes a REMOVE request readable.       */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_remove_request(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_remove_request               */

  gds_gst_print_apdu_header(message,size);
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining arguments                   */
    gds_gst_print_chaining_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common arguments                     */
    gds_gst_print_common_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print object                               */
    printf("object:\n");
    gds_gst_print_dn_struct(size,message);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print endo of message structure            */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_remove_request               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_modify_request                        */
/*                                                                    */
/* Description  : The function makes a MODIFY request readable.       */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_modify_request(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_modify_request               */

  gds_gst_print_apdu_header(message,size);
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining arguments                   */
    gds_gst_print_chaining_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common arguments                     */
    gds_gst_print_common_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print object                               */
    printf("object:\n");
    gds_gst_print_dn_struct(size,message);
  }
  printf("modifications:\n");
  while                 /* still attribute information following      */
       (*size >= sizeof(D23_md_info) &&
	((D23_md_info *)*message)->d23_mdtag == D23_MODTAG)
  {                     /* print attribute information                */
    gds_gst_print_mod_info(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print endo of message structure            */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_modify_request               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_modrdn_request                        */
/*                                                                    */
/* Description  : The function makes a MODIFY RDN request readable.   */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_modrdn_request(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_modrdn_request               */

  gds_gst_print_apdu_header(message,size);
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining arguments                   */
    gds_gst_print_chaining_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common arguments                     */
    gds_gst_print_common_arg(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print object                               */
    gds_gst_print_rep_info(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print end of message structure             */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_modrdn_request               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_bind_result                           */
/*                                                                    */
/* Description  : The function makes a BIND result readable.          */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_bind_result(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_bind_result                  */

  signed16 version;     /* protocol version                           */
  signed16 crkind;      /* kind of credentials                        */
  unsigned16 cr_tag;    /* tag of credential structure                */

  if                    /* message is too short                       */
    (sizeof(D23_rsbind) > *size)
  {                     /* print illegal message length               */
    printf("%s: %d of BIND header\n",gst_ill_len,sizeof(D23_rsbind));
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }
  else
  {                     /* print header information                   */
    version = ((D23_rsbind *)*message)->d23_Bversion;
    crkind  = ((D23_rsbind *)*message)->d23_Bcrkind;
    gds_gst_print_apdu_header(message,size);
    printf("Protocol version: %d ",version);
    *message += sizeof(D23_rsbind) - sizeof(D23_header);
    *size    -= sizeof(D23_rsbind) - sizeof(D23_header);
    switch(crkind)
    {
      case D23_NO_CRED:
	printf("No credentials\n");
	break;
      case D23_SIMPLE_CRED:
	cr_tag = ((D23_simcr_info *)*message)->d23_scrtag;
	if              /* simple credentials tag found               */
	  (cr_tag == D23_SIMTAG)
	{               /* print simple credentials                   */
	  gds_gst_print_simple_credentials(message,size);
	}
	else
	{               /* print simple credentials missing           */
	  printf(gst_invcred,cr_tag);
	  gds_gst_print_binary_data(*size,*message);
	  *message += *size;
	  *size     = 0;
	}
	break;
      case D23_STRONG_CRED:
	cr_tag = ((D23_strcr_info *)*message)->d23_strtag;
	if              /* strong credentials tag found               */
	  (cr_tag == D23_STRTAG)
	{               /* print strong credentials                   */
	  gds_gst_print_strong_credentials(message,size);
	}
	else
	{               /* print strong credentials missing           */
	  printf(gst_invcred,cr_tag);
	  gds_gst_print_binary_data(*size,*message);
	  *message += *size;
	  *size     = 0;
	}
	break;
      case D23_EXTERNAL_CRED:
	cr_tag = ((D23_extcr_info *)*message)->d23_exttag;
	if              /* extern credentials tag found               */
	  (cr_tag == D23_EXTTAG)
	{               /* print extern credentials                   */
	  gds_gst_print_extern_credentials(message,size);
	}
	else
	{               /* print simple credentials missing           */
	  printf(gst_invcred,cr_tag);
	  gds_gst_print_binary_data(*size,*message);
	  *message += *size;
	  *size     = 0;
	}
	break;
      default:
	printf("Unknown credentials type: %d\n",crkind);
	gds_gst_print_binary_data(*size,*message);
	*message += *size;
	*size     = 0;
	break;
    }

    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_bind_result                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_unbind_result                         */
/*                                                                    */
/* Description  : The function makes an UNBIND result readable.       */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_unbind_result(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_unbind_result                */

  gds_gst_print_eom(message,size);

}                       /* gds_gst_print_unbind_result                */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_read_result                           */
/*                                                                    */
/* Description  : The function makes a READ result readable.          */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_read_result(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_read_result                  */

  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining results                     */
    gds_gst_print_chaining_res(message,size);
  }
  if                    /* still message to print                     */
    (*size >= sizeof(D23_en_info) &&
     ((D23_en_info *)*message)->d23_entag == D23_ENTTAG)
  {                     /* print entry information                    */
    gds_gst_print_en_info(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common results                       */
    gds_gst_print_common_res(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print endo of message structure            */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_read_result                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_compare_result                        */
/*                                                                    */
/* Description  : The function makes a COMPARE result readable.       */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_compare_result(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_compare_result               */

  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining results                     */
    gds_gst_print_chaining_res(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print compare result information           */
    gds_gst_print_rscomp_info(message,size);
  }
  if                    /* distinguished name following               */
    (sizeof(D23_ds_name) <= *size &&
     ((D23_ds_name *)*message)->d23_dstag == D23_NAMTAG)
  {                     /* print distinguished name                   */
    printf("Distinguished name:\n");
    gds_gst_print_dn_struct(size,message);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common results                       */
    gds_gst_print_common_res(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print end of message structure             */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_compare_result               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_abandon_result                        */
/*                                                                    */
/* Description  : The function makes an ABANDON result readable.      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_abandon_result(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_abandon_result               */

  gds_gst_print_eom(message,size);

}                       /* gds_gst_print_abandon_result               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_list_result                           */
/*                                                                    */
/* Description  : The function makes a LIST result readable.          */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_list_result(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_list_result                  */

  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining results                     */
    gds_gst_print_chaining_res(message,size);
  }
  if                    /* distinguished name following               */
    (sizeof(D23_ds_name) <= *size &&
     ((D23_ds_name *)*message)->d23_dstag == D23_BONTAG)
  {                     /* print distinguished name                   */
    printf("Distinguished name:\n");
    gds_gst_print_dn_struct(size,message);
  }
  while                 /* still subordinate information following    */
       (*size >= sizeof(D23_subord_info) &&
	((D23_subord_info *)*message)->d23_sotag == D23_SOITAG)
  {                     /* print subordinate information              */
    gds_gst_print_so_info(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print partial outcome qualifier            */
    gds_gst_print_part_oq(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common results                       */
    gds_gst_print_common_res(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print endo of message structure            */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_list_result                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_search_result                         */
/*                                                                    */
/* Description  : The function makes a SEARCH result readable.        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_search_result(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_search_result                */

  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining results                     */
    gds_gst_print_chaining_res(message,size);
  }
  if                    /* distinguished name following               */
    (sizeof(D23_ds_name) <= *size &&
     ((D23_ds_name *)*message)->d23_dstag == D23_BONTAG)
  {                     /* print distinguished name                   */
    printf("Distinguished name:\n");
    gds_gst_print_dn_struct(size,message);
  }
  while                 /* still subordinate information following    */
       (*size >= sizeof(D23_en_info) &&
	((D23_en_info *)*message)->d23_entag == D23_ENTTAG)
  {                     /* print entry information                    */
    gds_gst_print_en_info(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print partial outcome qualifier            */
    gds_gst_print_part_oq(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print common results                       */
    gds_gst_print_common_res(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print endo of message structure            */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_search_result                */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_add_result                            */
/*                                                                    */
/* Description  : The function makes an ADD result readable.          */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_add_result(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_add_result                   */

  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining results                     */
    gds_gst_print_chaining_res(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print endo of message structure            */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_add_result                   */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_remove_result                         */
/*                                                                    */
/* Description  : The function makes a REMOVE result readable.        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_remove_result(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_remove_result                */

  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining results                     */
    gds_gst_print_chaining_res(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print endo of message structure            */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_remove_result                */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_modify_result                         */
/*                                                                    */
/* Description  : The function makes a MODIFY result readable.        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_modify_result(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_modify_result                */

  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining results                     */
    gds_gst_print_chaining_res(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print endo of message structure            */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_modify_result                */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_modrdn_result                         */
/*                                                                    */
/* Description  : The function makes a MODIFY RDN result readable.    */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_modrdn_result(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_modrdn_result                */

  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print chaining results                     */
    gds_gst_print_chaining_res(message,size);
  }
  if                    /* still message to print                     */
    (*size > 0)
  {                     /* print end of message structure             */
    gds_gst_print_eom(message,size);
  }

}                       /* gds_gst_print_modrdn_result                */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_apdu_header                           */
/*                                                                    */
/* Description  : The function makes an APDU header readable.         */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_apdu_header(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_apdu_header                  */

  if                    /* message is too short                       */
    (sizeof(D23_header) > *size)
  {                     /* print illegal message length               */
    printf("%s: %d of APDU header\n",gst_ill_len,sizeof(D23_header));
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }
  else
  {                     /* print header information                   */
    printf("APDU header, Version: %d, Dir-Id: %d, Bind-Id: %d\n",
      ((D23_header *)*message)->d23_versno,
      ((D23_header *)*message)->d23_dirid,
      ((D23_header *)*message)->d23_bindid);
    *message += sizeof(D23_header);
    *size    -= sizeof(D23_header);
  }

}                       /* gds_gst_print_apdu_header                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_simple_credentials                    */
/*                                                                    */
/* Description  : The function makes simple credentials readable      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_simple_credentials(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_simple_credentials           */

  printf("Simple credentials\n");
  *message += sizeof(D23_simcr_info);
  *size    -= sizeof(D23_simcr_info);
  if                    /* validity structure is following            */
    (((D23_val_info *)*message)->d23_valtag == D23_VALTAG)
  {                     /* print validity structure                   */
    gds_gst_print_validity(message,size);
  }
  if                    /* Distinguished name structure is following  */
    (((D23_ds_name *)*message)->d23_dstag == D23_NAMTAG)
  {                     /* print distinguished name structure         */
    gds_gst_print_dn_struct(size,message);
  }
  if                    /* password structure is following            */
    (((D23_pw_info *)*message)->d23_pwtag == D23_PWDTAG)
  {                     /* print password structure                   */
    gds_gst_print_password(message,size);
  }

}                       /* gds_gst_print_simple_credentials           */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_strong_credentials                    */
/*                                                                    */
/* Description  : The function makes strong credentials readable      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_strong_credentials(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_strong_credentials           */

  signed32 stroff;      /* strong credentials offset                  */

  printf("Strong credentials\n");
  stroff = sizeof(D23_strcr_info) + ((D23_strcr_info *)*message)->d23_stroff;

  if                    /* message is too short                       */
    (stroff > *size)
  {                     /* print illegal message length               */
    printf("%s: %d in strong credentials info\n",gst_ill_len,stroff);
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }
  else
  {                     /* print strong credentials                   */
    printf("security token\n");
    stroff   -= sizeof(D23_strcr_info);
    *size    -= sizeof(D23_strcr_info);
    *message += sizeof(D23_strcr_info);
    gds_gst_print_binary_data(stroff,*message);
    *message += stroff;
    *size    -= stroff;
  }

}                       /* gds_gst_print_strong_credentials           */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_extern_credentials                    */
/*                                                                    */
/* Description  : The function makes extern credentials readable      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_extern_credentials(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_extern_credentials           */

  signed32 extoff;      /* extern credentials offset                  */

  printf("Extern credentials\n");
  extoff = sizeof(D23_extcr_info) + ((D23_extcr_info *)*message)->d23_extoff;

  if                    /* message is too short                       */
    (extoff > *size)
  {                     /* print illegal message length               */
    printf("%s: %d in extern credentials info\n",gst_ill_len,extoff);
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }
  else
  {                     /* check external credentials kind            */
    printf("External credentials kind: ");
    switch(((D23_extcr_info *)*message)->d23_extkind)
    {
      case D23_DCE_AUTH:
	printf("DCE\n");
	break;
      default:
      printf("Unknown\n");
	break;
    }
    printf("security token\n");
    extoff   -= sizeof(D23_extcr_info);
    *size    -= sizeof(D23_extcr_info);
    *message += sizeof(D23_extcr_info);
    gds_gst_print_binary_data(extoff,*message);
    *message += extoff;
    *size    -= extoff;
  }

}                       /* gds_gst_print_extern_credentials           */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_chaining_arg                          */
/*                                                                    */
/* Description  : The function makes chaining arguments readable      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_chaining_arg(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_chaining_arg                 */

  D23_chain_arg *chain_arg;

  chain_arg = (D23_chain_arg *)*message;
  if                    /* chaining arguments present                 */
    (sizeof(D23_chain_arg) <= *size && chain_arg->d23_chtag == D23_CHTAG)
  {                     /* print chaining arguments structure         */
    printf("Chaining arguments:\n");
    printf("Alias dereferenced: ");
    switch(chain_arg->d23_al_deref)
    {
      case TRUE:
	printf("%s ",gst_true);
	break;
      case FALSE:
	printf("%s ",gst_false);
	break;
      default:
	printf("Unknown: %d ",chain_arg->d23_al_deref);
	break;
    }
    if                  /* no aliased RDNs set                        */
      (chain_arg->d23_al_RDN == D2_NOT_DEFINED)
    {                   /* print no aliased RDNs                      */
      printf("No aliased RDNs\n");
    }
    else
    {                   /* print aliased RDNs                         */
      printf("Aliased RDNs: %d\n",chain_arg->d23_al_RDN);
    }
    printf("Entry only: ");
    switch(chain_arg->d23_entry_only)
    {
      case TRUE:
	printf("%s ",gst_true);
	break;
      case FALSE:
	printf("%s ",gst_false);
	break;
      default:
	printf("Unknown: %d ",chain_arg->d23_entry_only);
	break;
    }
    printf("Return Cross references: ");
    switch(chain_arg->d23_ret_cref)
    {
      case TRUE:
	printf("%s ",gst_true);
	break;
      case FALSE:
	printf("%s ",gst_false);
	break;
      default:
	printf("Unknown: %d ",chain_arg->d23_ret_cref);
	break;
    }
    printf("Reference type: ");
    switch(chain_arg->d23_ref_type)
    {
      case D2_SUPERIOR:
	printf("SUPERIOR\n");
	break;
      case D2_SUBORDINATE:
	printf("SUBORDINATE\n");
	break;
      case D2_CROSS:
	printf("CROSS\n");
	break;
      case D2_NON_SPEC_SUBORDINATE:
	printf("NON SPECIFIC SUBORDINATE\n");
	break;
      default:
	printf("Illegal: %d\n",chain_arg->d23_ref_type);
	break;
    }
    if                  /* no time limit set                          */
      (chain_arg->d23_chtmlt == D2_NOT_DEFINED)
    {                   /* print time limit set                       */
      printf("No time limit\n");
    }
    else
    {                   /* print the time limit                       */
      printf("Time limit: %d\n",chain_arg->d23_chtmlt);
    }
    gds_gst_print_operation_prog(&chain_arg->d23_op_prog);
    printf("Maximum number of traces: %d\n",chain_arg->d23_max_tr);
    *message += sizeof(D23_chain_arg);
    *size    -= sizeof(D23_chain_arg);
    while               /* trace information is following             */
	 (sizeof(D23_trace_info) <= *size &&
	  ((D23_trace_info *)*message)->d23_trtag == D23_TRTAG)
    {                   /* print trace information                    */
      gds_gst_print_trace_info(message,size);
    }
    if                  /* originator is following                    */
      (sizeof(D23_ds_name) <= *size &&
       ((D23_ds_name *)*message)->d23_dstag == D23_RQTAG)
    {                   /* print originator name                      */
      printf("Originator:\n");
      gds_gst_print_dn_struct(size,message);
    }
    if                  /* target object following                    */
      (sizeof(D23_ds_name) <= *size &&
       ((D23_ds_name *)*message)->d23_dstag == D23_BONTAG)
    {                   /* print target object name                   */
      printf("Target object:\n");
      gds_gst_print_dn_struct(size,message);
    }
    if                  /* domain information following               */
      (sizeof(D23_dom_info) <= *size &&
       ((D23_dom_info *)*message)->d23_dmtag == D23_DMTAG)
    {                   /* print domain information                   */
      printf("Domain information\n");
      *message += sizeof(D23_dom_info);
      *size    -= sizeof(D23_dom_info);
    }
    if                  /* security parameters following              */
      (sizeof(D23_se_parm) <= *size &&
       ((D23_se_parm *)*message)->d23_setag == D23_SECTAG)
    {                   /* print security parameters                  */
      printf("Security parameters\n");
      *message += sizeof(D23_se_parm);
      *size    -= sizeof(D23_se_parm);
    }
  }

}                       /* gds_gst_print_chaining_arg                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_chaining_res                          */
/*                                                                    */
/* Description  : The function makes chaining results readable        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_chaining_res(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_chaining_res                 */

  D23_chain_res *chain_res;

  chain_res = (D23_chain_res *)*message;
  if                    /* chaining results present                   */
    (sizeof(D23_chain_res) <= *size && chain_res->d23_chrtag == D23_CHRTAG)
  {                     /* print chaining results structure           */
    printf("Chaining results, maximum number of cross references: %d\n",
      chain_res->d23_max_cref);
    *message += sizeof(D23_chain_res);
    *size    -= sizeof(D23_chain_res);
    if                  /* domain information following               */
      (sizeof(D23_dom_info) <= *size &&
       ((D23_dom_info *)*message)->d23_dmtag == D23_DMTAG)
    {                   /* print domain information                   */
      printf("Domain information\n");
      *message += sizeof(D23_dom_info);
      *size    -= sizeof(D23_dom_info);
    }
    while               /* cross references following                 */
	 (sizeof(D23_cross_ref) <= *size &&
	  ((D23_cross_ref *)*message)->d23_creftag == D23_CREFTAG)
    {                   /* print cross references                     */
      gds_gst_print_cross_ref(message,size);
    }
  }

}                       /* gds_gst_print_chaining_res                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_cross_ref                             */
/*                                                                    */
/* Description  : The function makes cross references readable        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_cross_ref(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_cross_ref                    */

  *message += sizeof(D23_cross_ref);
  *size    -= sizeof(D23_cross_ref);
  printf("Context prefix:\n");
  gds_gst_print_dn_struct(size,message);
  if                    /* access point is present                    */
    (sizeof(D23_acc_info) <= *size &&
     ((D23_acc_info *)*message)->d23_acctag == D23_ACCTAG)
  {                     /* print access point structure               */
    gds_gst_print_access_point(message,size);
  }
  else
  {                     /* print message in binary format             */
    printf("Access point is missing\n");
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }

}                       /* gds_gst_print_cross_ref                    */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_access_point                          */
/*                                                                    */
/* Description  : The function makes an access point readable         */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_access_point(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_access_point                 */

  byte       *psap;     /* pointer to PSAP address                    */

  *message += sizeof(D23_acc_info);
  *size    -= sizeof(D23_acc_info);
  printf("Access point:\n");
  printf("DSA name:\n");
  gds_gst_print_dn_struct(size,message);
  if                    /* PSAP address is following                  */
    (*size >= sizeof(D23_av_value) &&
     (unsigned16)((D23_av_value *)*message)->d23_avlen < D23_MINTAG &&
     ((D23_av_value *)*message)->d23_avrep == D2_PSAP_STX)
  {                     /* print PSAP address                         */
    psap = *message;
    gds_gst_print_attr_value(message);
    *size -= (*message - psap);
  }
  else
  {                     /* print message in binary format             */
    printf("PSAP address is missing\n");
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }

}                       /* gds_gst_print_access_point                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_part_oq                               */
/*                                                                    */
/* Description  : The function makes a partial outcome qualifier      */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_part_oq(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_part_oq                      */

			/* pointer to partial outcome qualifier       */
  D23_part_oq *part_oq;

  part_oq = (D23_part_oq *)*message;
  if                    /* partial outcome qualifier present          */
    (part_oq->d23_oqtag == D23_OQTAG)
  {                     /* print the partial outcome qualifier        */
    printf("partial outcome qualifier, unavailable critical extension: ");
    switch(part_oq->d23_oq_uncrit)
    {
      case TRUE:
	printf(gst_true);
	break;
      case FALSE:
	printf(gst_false);
	break;
      default:
	printf("Unknown: %d",part_oq->d23_oq_uncrit);
	break;
    }
    printf("\n");
    printf("limit problem: ");
    switch(part_oq->d23_oq_limit)
    {
      case D2_TIME_LIMIT_EX:
	printf("timeLimitExceeded");
	break;
      case D2_SIZE_LIMIT_EX:
	printf("sizeLimitExceeded");
	break;
      case D2_ADM_LIMIT_EX:
	printf("administrativeLimitExceeded");
	break;
      case D2_NO_LIMIT_EX:
	printf("noLimitExceeded");
	break;
      default:
	printf("Unknown: %d",part_oq->d23_oq_limit);
	break;
    }
    printf("\n");
    *message += sizeof(D23_part_oq);
    *size    -= sizeof(D23_part_oq);
    printf("unexplored:\n");
    while                 /* referral is following                    */
	 (sizeof(D23_rf_info) <= *size &&
	  ((D23_rf_info *)*message)->d23_rftag == D23_REFTAG)
    {                   /* print referral                             */
      gds_gst_print_referral(message,size);
    }
  }

}                       /* gds_gst_print_part_oq                      */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_dsa_referral                          */
/*                                                                    */
/* Description  : The function makes a DSA referral readable          */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_dsa_referral(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_dsa_referral                 */

			/* pointer to DSA referral                    */
  D23_drf_info *drf_info;

  drf_info = (D23_drf_info *)*message;
  printf("DSA referral:\n");
  *message += sizeof(D23_drf_info);
  *size    -= sizeof(D23_drf_info);
  if                    /* referral is following                      */
    (sizeof(D23_rf_info) <= *size &&
     ((D23_rf_info *)*message)->d23_rftag == D23_REFTAG)
  {                     /* print referral                             */
    gds_gst_print_referral(message,size);
    if                  /* context prefix is following                */
      (sizeof(D23_ds_name) <= *size &&
       ((D23_ds_name *)*message)->d23_dstag == D23_NAMTAG)
    {                   /* print context prefix                       */
      printf("context prefix:\n");
      gds_gst_print_dn_struct(size,message);
    }
  }
  else
  {                     /* print message in binary format             */
    printf("referral is missing\n");
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }

}                       /* gds_gst_print_dsa_referral                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_referral                              */
/*                                                                    */
/* Description  : The function makes a referral readable              */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_referral(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_referral                     */

  D23_rf_info *referral;

  referral = (D23_rf_info *)*message;
  printf("Referral:\n");
  if                    /* no aliased RDNs set                        */
    (referral->d23_rf_alirdn == D2_NOT_DEFINED)
  {                     /* print no aliased RDNs                      */
    printf("No aliased RDNs\n");
  }
  else
  {                     /* print aliased RDNs                         */
    printf("Aliased RDNs: %d\n",referral->d23_rf_alirdn);
  }
  if                    /* no RDNs resolved set                       */
    (referral->d23_rf_rdnres == D2_NOT_DEFINED)
  {                     /* no print RDNs resolved                     */
    printf("Undefined RDNs resolved\n");
  }
  else
  {                     /* print RDNs resolved                        */
    printf("RDNs resolved: %d\n",referral->d23_rf_rdnres);
  }
  printf("Reference type: ");
  switch(referral->d23_rf_type)
  {
    case D2_SUPERIOR:
      printf("SUPERIOR\n");
      break;
    case D2_SUBORDINATE:
      printf("SUBORDINATE\n");
      break;
    case D2_CROSS:
      printf("CROSS\n");
      break;
    case D2_NON_SPEC_SUBORDINATE:
      printf("NON SPECIFIC SUBORDINATE\n");
      break;
    case D2_NOT_DEFINED:
      printf("Undefined\n");
      break;
    default:
      printf("Illegal: %d\n",referral->d23_rf_type);
      break;
  }
  gds_gst_print_operation_prog(&referral->d23_op_pr);
  printf("Entry only: ");
  switch(referral->d23_rf_entry_only)
  {
    case TRUE:
      printf(gst_true);
      break;
    case FALSE:
      printf(gst_false);
      break;
    default:
      printf("Unknown: %d",referral->d23_rf_entry_only);
      break;
  }
  printf("\n");
  printf("Number of access points: %d\n",referral->d23_nm_acc);
  *message += sizeof(D23_rf_info);
  *size    -= sizeof(D23_rf_info);
  printf("Target object:\n");
  gds_gst_print_dn_struct(size,message);
  while                 /* access point is present                    */
       (sizeof(D23_acc_info) <= *size &&
	((D23_acc_info *)*message)->d23_acctag == D23_ACCTAG)
  {                     /* print access point structure               */
    gds_gst_print_access_point(message,size);
  }

}                       /* gds_gst_print_referral                     */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_common_arg                            */
/*                                                                    */
/* Description  : The function makes common arguments readable        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_common_arg(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_common_arg                   */

  D23_com_arg *com_arg;

  com_arg = (D23_com_arg *)*message;
  if                    /* common arguments present                   */
    (sizeof(D23_com_arg) <= *size && com_arg->d23_cmtag == D23_CMTAG)
  {                     /* print common arguments structure           */
    printf("Common arguments:\n");
    if                  /* no aliased RDNs set                        */
      (com_arg->d23_cmali == D2_NOT_DEFINED)
    {                   /* print no aliased RDNs                      */
      printf("No aliased RDNs\n");
    }
    else
    {                   /* print aliased RDNs                         */
      printf("Aliased RDNs: %d\n",com_arg->d23_cmali);
    }
    gds_gst_print_serv_control(&com_arg->d23_cmctr);
    gds_gst_print_operation_prog(&com_arg->d23_cmopr);
    *message += sizeof(D23_com_arg);
    *size    -= sizeof(D23_com_arg);
    if                  /* security parameters following              */
      (sizeof(D23_se_parm) <= *size &&
       ((D23_se_parm *)*message)->d23_setag == D23_SECTAG)
    {                   /* print security parameters                  */
      printf("Security parameters\n");
      *message += sizeof(D23_se_parm);
      *size    -= sizeof(D23_se_parm);
    }
    if                  /* requestor is following                     */
      (sizeof(D23_ds_name) <= *size &&
       ((D23_ds_name *)*message)->d23_dstag == D23_RQTAG)
    {                   /* print requestor name                       */
      printf("Requestor:\n");
      gds_gst_print_dn_struct(size,message);
    }
    if                  /* extensions are present                     */
      (sizeof(D23_bit_info) <= *size &&
       ((D23_bit_info *)*message)->d23_btag == D23_BITTAG)
    {                   /* print bit string information structure     */
      printf("extensions:\n");
      gds_gst_print_bit_struct(size,message);
    }
  }
  else
  {                     /* print common arguments missing             */
    printf("Common arguments missing\n");
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }

}                       /* gds_gst_print_common_arg                   */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_common_res                            */
/*                                                                    */
/* Description  : The function makes common results readable          */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_common_res(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_common_res                   */

  D23_com_res *com_res;

  com_res = (D23_com_res *)*message;
  if                    /* common results present                     */
    (sizeof(D23_com_res) <= *size && com_res->d23_crtag == D23_CRSTAG)
  {                     /* print common results structure             */
    printf("CommonResults, aliasDereferenced: ");
    switch(com_res->d23_ali_deref)
    {
      case TRUE:
	printf("TRUE");
	break;
      case FALSE:
	printf("FALSE");
	break;
      default:
	printf("Unknown: %d",com_res->d23_ali_deref);
	break;
    }
    printf("\n");
    *message += sizeof(D23_com_res);
    *size    -= sizeof(D23_com_res);
    if                  /* performer is following                     */
      (sizeof(D23_ds_name) <= *size &&
       ((D23_ds_name *)*message)->d23_dstag == D23_NAMTAG)
    {                   /* print performer name                       */
      printf("performer:\n");
      gds_gst_print_dn_struct(size,message);
    }
    if                  /* security parameters following              */
      (sizeof(D23_se_parm) <= *size &&
       ((D23_se_parm *)*message)->d23_setag == D23_SECTAG)
    {                   /* print security parameters                  */
      printf("SecurityParameters\n");
      *message += sizeof(D23_se_parm);
      *size    -= sizeof(D23_se_parm);
    }
  }
  else
  {                     /* print common results missing               */
    printf("CommonResults missing\n");
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }

}                       /* gds_gst_print_common_res                   */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_serv_control                          */
/*                                                                    */
/* Description  : The function makes service controls readable        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_serv_control(
			/* IN    - message to be printed              */
    D23_serv_ctrl *serv_ctrl)

{                       /* gds_gst_print_serv_control                 */

  printf("Service Controls:\n");

  gds_gst_print_options_serv_control(serv_ctrl->d23_scopt);

  printf("Priority: ");
  switch(serv_ctrl->d23_scpri)
  {
    case D2_PRIO_LOW:
      printf("low");
      break;
    case D2_PRIO_MEDIUM:
      printf("medium");
      break;
    case D2_PRIO_HIGH:
      printf("high");
      break;
    default:
      printf("Unknown: %d",serv_ctrl->d23_scpri);
      break;
  }
  printf("\n");

  gds_gst_print_time_limit(serv_ctrl->d23_sctlm);
  gds_gst_print_size_limit(serv_ctrl->d23_scslm);

  printf("Scope of referrals: ");
  switch(serv_ctrl->d23_scref)
  {
    case D2_DMD_SCOPE:
      printf("dmd");
      break;
    case D2_COUNTRY_SCOPE:
      printf("country");
      break;
    case D2_WORLD_SCOPE:
      printf("world");
      break;
    default:
      printf("Unknown: %d",serv_ctrl->d23_scref);
      break;
  }
  printf("\n");

}                       /* gds_gst_print_serv_control                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_operation_prog                        */
/*                                                                    */
/* Description  : The function makes an operation progress readable   */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_operation_prog(
			/* IN    - message to be printed              */
    D23_op_pr_info *op_prog)

{                       /* gds_gst_print_operation_prog               */

  printf("OperationProgress: ");
  switch(op_prog->d23_opnm_rs)
  {
    case D2_NOT_STARTED:
      printf("notStarted");
      break;
    case D2_PROCEEDING:
      printf("proceeding, nextRDNToBeResolved: %d",
	op_prog->d23_op_nxtrdn);
      break;
    case D2_COMPLETED:
      printf("completed");
      break;
    default:
      printf("Illegal: %d",op_prog->d23_opnm_rs);
      break;
  }
  printf("\n");

}                       /* gds_gst_print_operation_prog               */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_trace_info                            */
/*                                                                    */
/* Description  : The function makes a trace information readable     */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_trace_info(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_trace_info                   */

  D23_trace_info *trace_info;

  trace_info = (D23_trace_info *)*message;
  printf("Trace information:\n");
  gds_gst_print_operation_prog(&trace_info->d23_op_prog);
  *message += sizeof(D23_trace_info);
  *size    -= sizeof(D23_trace_info);
  printf("DSA name:\n");
  gds_gst_print_dn_struct(size,message);
  if                    /* target object following                    */
    (sizeof(D23_ds_name) <= *size &&
     ((D23_ds_name *)*message)->d23_dstag == D23_NAMTAG)
  {                     /* print target object name                   */
    printf("Target object name:\n");
    gds_gst_print_dn_struct(size,message);
  }

}                       /* gds_gst_print_trace_info                   */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_en_info                               */
/*                                                                    */
/* Description  : The function makes an entry information readable    */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_en_info(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_en_info                      */

  D23_en_info *en_info; /* pointer to entry information structure     */

  en_info = (D23_en_info *)*message;
  printf("entryInformation, fromEntry: ");
  switch(en_info->d23_enmas)
  {
    case TRUE:
      printf("TRUE");
      break;
    case FALSE:
      printf("FALSE");
      break;
    default:
      printf("Unknown: %d",en_info->d23_enmas);
      break;
  }
  printf(", max number of attributes: %d\n",en_info->d23_enmaxat);
  *message += sizeof(D23_en_info);
  *size    -= sizeof(D23_en_info);
  gds_gst_print_dn_struct(size,message);
  printf("Attributes:\n");
  while                 /* still attribute information following      */
       (*size > sizeof(D23_at_type) &&
	((D23_at_type *)*message)->d23_atttag == D23_ATTTAG)
  {                     /* print attribute information                */
    gds_gst_print_at_info(message,size);
  }

}                       /* gds_gst_print_en_info                      */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_so_info                               */
/*                                                                    */
/* Description  : The function makes a subordinate information        */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_so_info(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_so_info                      */

			/* pointer to subordinate information         */
  D23_subord_info *so_info;

  so_info = (D23_subord_info *)*message;
  printf("subordinate, aliasEntry: ");
  switch(so_info->d23_soali)
  {
    case TRUE:
      printf("TRUE");
      break;
    case FALSE:
      printf("FALSE");
      break;
    default:
      printf("Unknown: %d",so_info->d23_soali);
      break;
  }
  printf(" fromEntry: ");
  switch(so_info->d23_somas)
  {
    case TRUE:
      printf("TRUE");
      break;
    case FALSE:
      printf("FALSE");
      break;
    default:
      printf("Unknown: %d",so_info->d23_somas);
      break;
  }
  *message += sizeof(D23_subord_info);
  *size    -= sizeof(D23_subord_info);
  printf("relativeDistinguishedName:\n");
  gds_gst_print_dn_struct(size,message);


}                       /* gds_gst_print_so_info                      */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_ei_select                             */
/*                                                                    */
/* Description  : The function makes an entry information selection   */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_ei_select(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_ei_select                    */

  D23_rq_info *rq_info; /* pointer to requested information structure */

  register signed16 i;  /* loop variable                              */

  rq_info = (D23_rq_info *)*message;
  if                    /* requested information selection is present */
    (sizeof(D23_rq_info) <= *size && rq_info->d23_rqtag == D23_REQTAG)
  {                     /* print requested information structure      */
    printf("selection:\n");
    printf("InfoTypes: ");
    switch(rq_info->d23_rqreq)
    {
      case D2_R_TYP:
	printf("attributeTypesOnly\n");
	break;
      case D2_R_VAL_TYP:
	printf("attributeTypesAndValues\n");
	break;
      default:
	printf("Unknown: %d\n",rq_info->d23_rqreq);
	break;
    }
    if                  /* all attributes are requested               */
      (rq_info->d23_rqnbr == D2_ALL_ATT_REQ)
    {                   /* print all attributes                       */
      printf("allAttributes");
      *message += sizeof(D23_rq_info);
      *size    -= sizeof(D23_rq_info);
    }
    else
    {                   /* print number of requested attributes       */
      printf("number of selected attributes: %d\n",rq_info->d23_rqnbr);
      *message += sizeof(D23_rq_info);
      *size    -= sizeof(D23_rq_info);
      for               /* all requested attributes                   */
	 (i = 0; i < rq_info->d23_rqnbr && *size > 0; i++)
      {                 /* print the attribute type                   */
	gds_gst_print_atype(message,size);
      }
    }
    printf("\n");
  }
  else                  /* print message in binary format             */
  {                     /* print requested attributes missing         */
    printf("requested attributes missing\n");
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }

}                       /* gds_gst_print_ei_select                    */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_at_info                               */
/*                                                                    */
/* Description  : The function makes an attribute information         */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_at_info(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_at_info                      */

  gds_gst_print_atype(message,size);
  gds_gst_print_avinfo(message,size);
  printf("\n");

}                       /* gds_gst_print_at_info                      */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_mod_info                              */
/*                                                                    */
/* Description  : The function makes an modification information      */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_mod_info(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_mod_info                     */

  D2_attr_mod mod;      /* attribute modification                     */

  mod = ((D23_md_info *)*message)->d23_mdmod;
  printf("modification: ");
  switch(mod)
  {
    case D2_ADDATT:
      printf("addAttribute");
      break;
    case D2_DELATT:
      printf("removeAttribute");
      break;
    case D2_ADD_AV:
      printf("addValues");
      break;
    case D2_DEL_AV:
      printf("removeValues");
      break;
    case D2_REP_AV:
      printf("replaceValue");
      break;
    default:
      printf("Unknown: %d",mod);
      break;
  }
  printf("\n");
  *message += sizeof(D23_md_info);
  *size    -= sizeof(D23_md_info);

  gds_gst_print_atype(message,size);
  gds_gst_print_avinfo(message,size);

}                       /* gds_gst_print_mod_info                     */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_at_problem                            */
/*                                                                    */
/* Description  : The function makes an attribute problem information */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_at_problem(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_at_problem                   */

  signed16 ap;          /* attribute problem                          */

  ap = ((D23_at_prob *)*message)->d23_apval;
  printf("attribute problem: ");
  switch(ap)
  {
    case D2_ATTRIBUTE_MISSING:
      printf("noSuchAttributeOrValue");
      break;
    case D2_CONSTRAINT_VIOLATION:
      printf("constraintViolation");
      break;
    case D2_INAPPR_MATCH:
      printf("inappropriateMatching");
      break;
    case D2_UNDEFINED_ATTR_TYPE:
      printf("undefinedAttributeType");
      break;
    case D2_ATSYNTAX_ERROR:
      printf("invalidAttributeSyntax");
      break;
    case D2_AT_VALUE_EXISTS:
      printf("attributeOrValueAlreadyExists");
      break;
    default:
      printf("Unknown: %d",ap);
      break;
  }
  printf("\n");
  *message += sizeof(D23_at_prob);
  *size    -= sizeof(D23_at_prob);

  gds_gst_print_atype(message,size);
  gds_gst_print_avinfo(message,size);

}                       /* gds_gst_print_at_problem                   */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_atype                                 */
/*                                                                    */
/* Description  : The function makes an attribute type readable       */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_atype(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_atype                        */

  D23_at_type *at_type; /* pointer to attribute type structure        */

  at_type = (D23_at_type *)*message;
  if                    /* attribute type is following                */
    (sizeof(D23_at_type) + at_type->d23_atoff <= *size &&
     at_type->d23_atttag == D23_ATTTAG)
  {                     /* print attribute type                       */
    *message += sizeof(D23_at_type);
    *size    -= sizeof(D23_at_type);
    gds_gst_print_obj_ident(at_type->d23_atlen,*message);
    *message += at_type->d23_atoff;
    *size    -= at_type->d23_atoff;
  }

}                       /* gds_gst_print_atype                        */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_avinfo                                */
/*                                                                    */
/* Description  : The function makes an attribute value information   */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_avinfo(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_avinfo                       */

  D23_av_info *av_info; /* pointer to attribute value info structure  */

  av_info = (D23_av_info *)*message;
  if                    /* attribute value information is following   */
    (sizeof(D23_av_info) <= *size && av_info->d23_avtag == D23_AVTAG)
  {                     /* print attribute value information          */
    printf("Maximum number of values: %d ",
      av_info->d23_avnum,av_info->d23_asyntx);
    gds_gst_print_representation(av_info->d23_asyntx);
    *message += sizeof(D23_av_info);
    *size    -= sizeof(D23_av_info);
    while               /* attribute values are following             */
	 ((unsigned16)((D23_av_value *)*message)->d23_avlen < D23_MINTAG)
    {                   /* print the attribute value                  */
      gds_gst_print_attr_value(message);
    }
    *size -= *message - (byte *)(av_info + 1);
  }
  printf("\n");

}                       /* gds_gst_print_avinfo                       */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_rep_info                              */
/*                                                                    */
/* Description  : The function makes a replace information readable   */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_rep_info(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_rep_info                     */

  D23_repinfo *rep_info;/* replace information                        */

  rep_info = (D23_repinfo *)*message;
  if                    /* replacement information following          */
    (*size >= sizeof(D23_repinfo) && rep_info->d23_reptag == D23_REPTAG)
  {                     /* print it                                   */
    printf("replace RDN, deleteOldRDN: ");
    switch(rep_info->d23_delordn)
    {
      case TRUE:
	printf(gst_true);
	break;
      case FALSE:
	printf(gst_false);
	break;
      default:
	printf("Unknown: %d",rep_info->d23_delordn);
	break;
    }
    printf("\n");
    *message += sizeof(D23_repinfo);
    *size    -= sizeof(D23_repinfo);
    printf("object:\n");
    gds_gst_print_dn_struct(size,message);
    printf("New RDN:\n");
    gds_gst_print_dn_struct(size,message);
  }
  else
  {                     /* print end of message binary                */
    printf("Replace information missing\n");
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }

}                       /* gds_gst_print_rep_info                     */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_rscomp_info                           */
/*                                                                    */
/* Description  : The function makes a compare result information     */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_rscomp_info(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_rscomp_info                  */

			/* pointer to compare result information      */
  D23_rscompinfo *rscomp_info;

  rscomp_info = (D23_rscompinfo *)*message;
  if                    /* compare result information is present      */
    (sizeof(D23_rscompinfo) <= *size && rscomp_info->d23_Ltag == D23_LTAG)
  {                     /* print compare result information structure */
    printf("matched: ");
    switch(rscomp_info->d23_Lmatsgn)
    {
      case TRUE:
	printf("%s ",gst_true);
	break;
      case FALSE:
	printf("%s ",gst_false);
	break;
      default:
	printf("Unknown %d ",rscomp_info->d23_Lmatsgn);
	break;
    }
    printf("fromEntry: ");
    switch(rscomp_info->d23_Lfrmen)
    {
      case TRUE:
	printf("TRUE");
	break;
      case FALSE:
	printf("FALSE");
	break;
      default:
	printf("Unknown: %d",rscomp_info->d23_Lfrmen);
	break;
    }
    printf("\n");
    *message += sizeof(D23_rscompinfo);
    *size    -= sizeof(D23_rscompinfo);
  }
  else
  {                     /* print message in binary format             */
    printf("Compare result information missing\n");
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }

}                       /* gds_gst_print_rscomp_info                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_search_arg                            */
/*                                                                    */
/* Description  : The function makes search arguments readable        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_search_arg(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_search_arg                   */

  D23_srcharg *search_arg;

  search_arg = (D23_srcharg *)*message;
  if                    /* search arguments present                   */
    (sizeof(D23_srcharg) <= *size && search_arg->d23_Itag == D23_ITAG)
  {                     /* print search arguments structure           */
    printf("Search arguments, ");
    printf("searchAliases: %s ",search_arg->d23_Iali == TRUE ?
      gst_true : gst_false);
    printf("subset: ");
    switch(search_arg->d23_Iobjsub)
    {
      case D2_BASE_OBJECT:
	printf("baseObject");
	break;
      case D2_ONE_LEVEL:
	printf("oneLevel");
	break;
      case D2_WHOLE_SUBTREE:
	printf("wholeSubtree");
	break;
      default:
	printf("Unknown: %d",search_arg->d23_Iobjsub);
	break;
    }
    *message += sizeof(D23_srcharg);
    *size    -= sizeof(D23_srcharg);
  }
  else
  {                     /* print search arguments missing             */
    printf("search arguments missing");
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }
  printf("\n");

}                       /* gds_gst_print_search_arg                   */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_filter                                */
/*                                                                    */
/* Description  : The function makes a filter structure readable      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_filter(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_filter                       */

  D23_ft1_info *f_info; /* pointer to filter information structure    */

  register signed16 i;  /* loop variable                              */

  f_info = (D23_ft1_info *)*message;
  if                    /* requested information selection is present */
    (sizeof(D23_ft1_info) <= *size && f_info->d23_fp1tag == D23_PATTAG)
  {                     /* print filter pattern structure             */
    printf("Filter: ");
    printf("operation mode: ");
    switch(f_info->d23_fp1mod)
    {
      case D2_AND:
	printf("%s, number of combined filters: %d\n",gst_and,
	  f_info->d23_fp1count);
	*message += sizeof(D23_ft1_info);
	*size    -= sizeof(D23_ft1_info);
	for             /* all combined filters                       */
	   (i = 0; i < f_info->d23_fp1count; i++)
	{               /* print the filter                           */
	  printf("%d. component\n",i);
	  gds_gst_print_filter(message,size);
	}
	break;
      case D2_OR:
	printf("%s number of combined filters: %d\n",gst_or,
	  f_info->d23_fp1count);
	*message += sizeof(D23_ft1_info);
	*size    -= sizeof(D23_ft1_info);
	for             /* all combined filters                       */
	   (i = 0; i < f_info->d23_fp1count; i++)
	{               /* print the filter                           */
	  gds_gst_print_filter(message,size);
	}
	break;
      case D2_NOT:
	printf("%s\n",gst_not);
	*message += sizeof(D23_ft1_info);
	*size    -= sizeof(D23_ft1_info);
	gds_gst_print_filter(message,size);
	break;
      case D2_ITEM:
	printf("%s\n",gst_item);
	*message += sizeof(D23_ft1_info);
	*size    -= sizeof(D23_ft1_info);
	gds_gst_print_filter_item(message,size);
	break;
      default:
	printf("Unknown: %d\n",f_info->d23_fp1mod);
	gds_gst_print_binary_data(*size,*message);
	*message += *size;
	*size     = 0;
	break;
    }
  }

}                       /* gds_gst_print_filter                       */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_filter_item                           */
/*                                                                    */
/* Description  : The function makes a filter item structure readable */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_filter_item(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_filter_item                  */

  D23_fi1_info *f_item; /* pointer to filter item structure           */
  signed16     off;     /* offset of message element                  */

  f_item = (D23_fi1_info *)*message;
  if                    /* requested information selection is present */
    (sizeof(D23_fi1_info) <= *size && f_item->d23_fi1tag == D23_FIITAG)
  {                     /* print filter item structure                */
    *message += sizeof(D23_fi1_info);
    *size    -= sizeof(D23_fi1_info);
    printf("Filter item, ");
    gds_gst_print_representation(f_item->d23_fi1syntx);
    printf("matching rule: ");
    switch(f_item->d23_fi1match)
    {
      case D2_EQUAL:
	printf("%s\nAttribute type: ",gst_equal);
	gds_gst_print_atype(message,size);
	printf("\n");
	off       = ((D23_av_value *)*message)->d23_avoff;
	gds_gst_print_attr_value(message);
	*size    -= sizeof(D23_av_value) + off;
	break;
      case D2_SUBSTRINGS:
	printf("%s\nAttribute type: ",gst_substrings);
	gds_gst_print_atype(message,size);
	printf("\n");
	while           /* filter substrings following                */
	     (*size > 0 &&
	      ((D23_fs1_info *)*message)->d23_fs1tag == D23_FISTAG)
	{               /* print filter substrings structure          */
	  off       = ((D23_fs1_info *)*message)->d23_fs1off;
	  gds_gst_print_filter_substr(message);
	  *size    -= sizeof(D23_fs1_info) + off;
	}
	break;
      case D2_GTEQ:
	printf("%s\nAttribute type: ",gst_greater_or_equal);
	gds_gst_print_atype(message,size);
	printf("\n");
	off       = ((D23_av_value *)*message)->d23_avoff;
	gds_gst_print_attr_value(message);
	*size    -= sizeof(D23_av_value) + off;
	break;
      case D2_LTEQ:
	printf("%s\nAttribute type: ",gst_less_or_equal);
	gds_gst_print_atype(message,size);
	printf("\n");
	off       = ((D23_av_value *)*message)->d23_avoff;
	gds_gst_print_attr_value(message);
	*size    -= sizeof(D23_av_value) + off;
	break;
      case D2_PRESENT:
	printf("%s\nAttribute type: ",gst_present);
	gds_gst_print_atype(message,size);
	printf("\n");
	break;
      case D2_APPROX_MATCH:
	printf("%s\nAttribute type: ",gst_approximate_match);
	gds_gst_print_atype(message,size);
	printf("\n");
	off       = ((D23_av_value *)*message)->d23_avoff;
	gds_gst_print_attr_value(message);
	*size    -= sizeof(D23_av_value) + off;
	break;
      default:
	printf("Unknown: %d\n",f_item->d23_fi1match);
	gds_gst_print_binary_data(*size,*message);
	*message += *size;
	*size     = 0;
	break;
    }
  }

}                       /* gds_gst_print_filter_item                  */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_eom                                   */
/*                                                                    */
/* Description  : The function makes an EOM structure readable.       */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_eom(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_eom                          */

  if                    /* message is too short                       */
    (sizeof(D23_eom) > *size)
  {                     /* print illegal message length               */
    printf("%s: %d of EOM structure\n",gst_ill_len,sizeof(D23_eom));
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }
  else if               /* end of message tag found                   */
	 (((D23_eom *)*message)->d23_eomtag == D23_EOMTAG)
  {                     /* print end of message                       */
    printf("End of message, Partial outcome qualifier offset: %ld\n",
      ((D23_eom *)*message)->d23_eom_oqoff);
    *message += sizeof(D23_eom);
    *size    -= sizeof(D23_eom);
  }
  else
  {                     /* print message in binary format             */
    printf("End of message tag is missing\n");
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }

}                       /* gds_gst_print_eom                          */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_dn_struct                             */
/*                                                                    */
/* Description  : The function makes a distinguished name structure   */
/*                readable                                            */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_dn_struct(
    signed32 *size,     /* INOUT - rest length of containing structure*/
    byte **message)     /* INOUT - message to print                   */

{                       /* gds_gst_print_dn_struct                    */

  unsigned16 complete;  /* indicates complete decoded DN              */
  unsigned16 tag;       /* DN structure tag                           */
  signed16 dnlen;       /* length of decoded DN                       */
  signed16 dnoff;       /* offset of decoded DN                       */

  byte *encoded_part;   /* encoded part of DN                         */

  if                    /* message is too short                       */
    (sizeof(D23_ds_name) > *size)
  {                     /* print illegal message length               */
    printf("%s: %d of DN structure\n",gst_ill_len,sizeof(D23_ds_name));
    gds_gst_print_binary_data(*size,*message);
    *message += *size;
    *size     = 0;
  }
  else
  {                     /* assign tag and complete flag               */
    tag      = ((D23_ds_name *)*message)->d23_dstag;
    complete = ((D23_ds_name *)*message)->d23_dscomplete;

    if                  /* distinguished name tag is missing          */
      (tag != D23_NAMTAG && tag != D23_BONTAG && tag != D23_RQTAG)
    {                   /* print illegal tag information              */
      printf("%s: %d (distinguished name tag expected)\n",gst_ill_tag,tag);
      gds_gst_print_binary_data(*size,*message);
      *message += *size;
      *size = 0;
    }
    else
    {                   /* print distinguished name                   */
      printf("%s\n",gst_dist_name_rep);
      dnlen = ((D23_ds_name *)*message)->d23_dslen;
      dnoff = ((D23_ds_name *)*message)->d23_dsoff;
      gds_gst_print_dist_name(dnlen,*message + sizeof(D23_ds_name));
      *message += sizeof(D23_ds_name) + dnoff;
      *size    -= sizeof(D23_ds_name) + dnoff;
			/* check completeness of decoded DN           */
      switch(complete)
      {
	case TRUE:
	  break;
	case FALSE:
	  if (dnlen != 0)
	  {
	    encoded_part = *message;
	    gds_gst_print_attr_value(message);
	    *size -= *message - encoded_part;
	  }
	  break;
	default:
	  printf("Illegal complete flag in DN: %d\n",complete);
	  gds_gst_print_binary_data(*size,*message);
	  *message += *size;
	  *size = 0;
	  break;
      }
    }
  }

}                       /* gds_gst_print_dn_struct                    */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_validity                              */
/*                                                                    */
/* Description  : The function makes a validity structure readable    */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_validity(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_validity                     */

  if                    /* time info tag is present                   */
    (*size > 0 && ((D23_tm_info *)*message)->d23_tmtag == D23_TIMTAG)
  {                     /* print time information structure           */
    gds_gst_print_tm_info(size,message);
  }
  if                    /* another time info tag is present           */
    (*size > 0 && ((D23_tm_info *)*message)->d23_tmtag == D23_TIMTAG)
  {                     /* print time information structure           */
    gds_gst_print_tm_info(size,message);
  }
  if                    /* bit string is present                      */
    (*size > 0 && ((D23_bit_info *)*message)->d23_btag == D23_BITTAG)
  {                     /* print bit string information structure     */
    gds_gst_print_bit_struct(size,message);
  }
  if                    /* another bit string is present              */
    (*size > 0 && ((D23_bit_info *)*message)->d23_btag == D23_BITTAG)
  {                     /* print bit string information structure     */
    gds_gst_print_bit_struct(size,message);
  }

}                       /* gds_gst_print_validity                     */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_password                              */
/*                                                                    */
/* Description  : The function makes a password structure readable    */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_password(
    byte     **message, /* INOUT - message to be printed              */
    signed32 *size)     /* INOUT - size of message to be printed      */

{                       /* gds_gst_print_password                     */

  signed32 pwoff;       /* length of password                         */

  printf("Password is secret\n");
  pwoff = sizeof(D23_pw_info) +
    (signed32)((D23_pw_info *)*message)->d23_pwoff;
  if                    /* message is too short                       */
    (pwoff > *size)
  {                     /* print illegal message length               */
    printf("%s: %d in password info\n",gst_ill_len,pwoff);
    gds_gst_print_binary_data(*size,*message);
    pwoff = *size;
  }
  *message += pwoff;
  *size    -= pwoff;

}                       /* gds_gst_print_password                     */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_tm_info                               */
/*                                                                    */
/* Description  : The function makes a time info structure readable   */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_tm_info(
    signed32 *len,      /* INOUT - rest length of containing structure*/
    byte **value)       /* INOUT - time information                   */

{                       /* gds_gst_print_tm_info                      */

  signed32 tmoff;       /* offset of time info structure              */

  tmoff   = sizeof(D23_tm_info) + ((D23_tm_info *)*value)->d23_tmoff;
  if                    /* time info tag is missing                   */
    (((D23_tm_info *)*value)->d23_tmtag != D23_TIMTAG)
  {                     /* print illegal tag information              */
    printf("%s: %d (time info tag expected)\n",gst_ill_tag,
      ((D23_tm_info *)*value)->d23_tmtag);
    gds_gst_print_binary_data(*len,*value);
    *value += *len;
    *len = 0;
  }
  else if               /* message is too short                       */
	 (tmoff > *len)
  {                     /* print illegal message length               */
    printf("%s: %d in time info\n",gst_ill_len,tmoff);
    gds_gst_print_binary_data(*len,*value);
    *value += *len;
    *len = 0;
  }
  else
  {                     /* assigne time offset                        */
    printf("UTCTime: %.*s\n",((D23_tm_info *)*value)->d23_tmlen,
      *value + sizeof(D23_tm_info));
    *len   -= tmoff;
    *value += tmoff;
  }

}                       /* gds_gst_print_tm_info                      */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_bit_struct                            */
/*                                                                    */
/* Description  : The function makes a bit string structure readable  */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_bit_struct(
    signed32 *len,      /* INOUT - rest length of containing structure*/
    byte **value)       /* INOUT - bit string structure               */

{                       /* gds_gst_print_bit_struct                   */

  unsigned16 tag;       /* DN structure tag                           */
  unsigned16 bitlen;    /* length of bit string in bits               */
  unsigned16 bitoff;    /* offset of bit string in byte               */

  tag      = ((D23_bit_info *)*value)->d23_btag;

  bitoff = sizeof(D23_bit_info) + ((D23_bit_info *)*value)->d23_boff;
  if                    /* bitstring structure tag is missing         */
    (tag != D23_BITTAG)
  {                     /* print illegal tag information              */
    printf("%s: %d (bit string structure tag expected)\n",gst_ill_tag,tag);
    gds_gst_print_binary_data(*len,*value);
    *value += *len;
    *len = 0;
  }
  else if               /* message is too short                       */
	 ((signed32)bitoff > *len)
  {                     /* print illegal message length               */
    printf("%s: %d in bitstring info\n",gst_ill_len,bitoff);
    gds_gst_print_binary_data(*len,*value);
    *value += *len;
    *len = 0;
  }
  else
  {                     /* print bit string                           */
    printf("%s\n",gst_bit_string);
    bitlen = ((D23_bit_info *)*value)->d23_blen;
    gds_gst_print_bit_string(bitlen,*value + sizeof(D23_bit_info));
    *value += bitoff;
    *len   -= bitoff;
  }

}                       /* gds_gst_print_bit_struct                   */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_bit_string                            */
/*                                                                    */
/* Description  : The function makes a bit string printable           */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_bit_string(
    signed32 len,       /* IN    - length of bit string in bits       */
    byte *value)        /* IN    - bit string                         */

{                       /* gds_gst_print_bit_string                   */

  unsigned32 portion;   /* portion of th bit string                   */
  signed32   port_count;/* count of portions                          */
  signed32   port_len;  /* length of the portion                      */
			/* high bit of bit string portion             */
  unsigned32 highbit = (unsigned32)1 << 31;

  register signed16 i,j;/* loop variables                             */

  if (len >0)
  {
    port_count = (len - 1) / (8 * sizeof(portion));
    for                   /* all portions of the bit string             */
       (i = 0; i < port_count + 1; i++, value += sizeof(unsigned32))
    {                     /* set the length of the portion              */
      portion  = *(unsigned32 *)value;
      port_len = i < port_count ? 8 * sizeof(portion) :
				len - port_count * 8 * sizeof(portion);
      for                 /* all bits of the portion                    */
         (j = 0; j < port_len; j++)
      {                   /* print the bit of the portion               */
        printf("%d",portion & (highbit >> j));
        if                /* end of segment reached                     */
	  (j % 4 == 3)
        {                 /* print segment separator                    */
	  printf("_");
        }
      }
    }
  } /* if len > 0 */
}                       /* gds_gst_print_bit_string                   */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_obj_ident                             */
/*                                                                    */
/* Description  : The function makes an object identifier readable    */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_obj_ident(
    signed16 len,       /* IN    - length of object identifier        */
    byte *value)        /* IN    - object identifier                  */

{                       /* gds_gst_print_obj_ident                    */

  D2_obj_id ob_id;
  char ob_id_string[D2_OBJ_IDL_MAX];

  ob_id.d2_typ_len = len;
  ob_id.d2_type    = (Ob_id_string)value;
  if                    /* object identifier cannot be converted      */
    (d27_301_oid_str(&ob_id,ob_id_string) < 0)
  {                     /* print error indication                     */
    printf("Object identifier could not be converted:\n");
    gds_gst_print_binary_data(len,value);
  }
  else
  {                     /* print object identifier                    */
    printf("%s ",ob_id_string);
  }

}                       /* gds_gst_print_obj_ident                    */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_dist_name                             */
/*                                                                    */
/* Description  : The function makes a distinguished name readable    */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_dist_name(
    signed16 len,       /* IN    - length of integer list in byte     */
    byte *value)        /* IN    - sequence of strings                */

{                       /* gds_gst_print_dist_name                    */

			/* string of type value separators            */
  static char tv_separators[] =
    {D2_T61_REP,D2_PRTBL_REP,D2_NUM_REP,D2_IA5_REP,D2_EOS};
			/* string of AVA and RDN separators           */
  static char ar_separators[] = {D2_RDN_SEP,D2_AVA_SEP,D2_EOS};

  byte *rdn;            /* relative distinguished name                */
  byte *ava;            /* attribute value assertion                  */
  byte *rep;            /* representation of one attribute value      */

  if                    /* DN has no RDNs                             */
    (len <= 1)
  {                     /* print "root"                               */
    printf("ROOT\n");
  }
  else if               /* DN is not null terminated                  */
	 (value[len - 1] != D2_EOS)
  {                     /* print error indication                     */
    printf("DN not null terminated:\n");
    gds_gst_print_binary_data(len,value);
  }
  else
  {                     /* print all RDNs                             */
    for                 /* all RDNs of the distinguished name         */
       (rdn = value; *rdn != D2_EOS; )
    {                   /* print the RDN                              */
      for               /* all AVAs of the RDN                        */
	(ava = rdn; *ava != D2_RDN_SEP && *ava != D2_EOS; )
      {                 /* check whether first AVA in RDN             */
	if              /* AVA is not the first one in RDN            */
	  (*ava == D2_AVA_SEP)
	{               /* reset AVA                                  */
	  ava++;
	}
	if              /* no type value separator found              */
	  ((rep = (byte *)strpbrk((char *)ava,tv_separators)) == NULL)
	{               /* print error indication                     */
	  printf("Missing type-value separator in AVA:\n");
	  gds_gst_print_binary_data(len - (ava - value),ava);
	}
	else
	{               /* print object identifier                    */
	  printf("%.*s = ",rep - ava,ava);
	  if            /* no AVA or RDN separator found              */
	    ((ava = (byte *)strpbrk((char *)rep + 1,ar_separators)) == NULL)
	  {             /* set ava to end of DN                       */
	    ava = value + len - 1;
	  }
	  switch(*rep)
	  {
	    case D2_T61_REP:
	      printf("(%s) %.*s ",gst_t61_rep,ava - (rep + 1),rep + 1);
	      break;
	    case D2_PRTBL_REP:
	      printf("(%s) %.*s ",gst_printable_rep,ava - (rep + 1),
		rep + 1);
	      break;
	    case D2_NUM_REP:
	      printf("(%s) %.*s ",gst_numeric_rep,ava - (rep + 1),rep + 1);
	      break;
	    case D2_IA5_REP:
	      printf("(%s) %.*s ",gst_ia5_rep,ava - (rep + 1),rep + 1);
	      break;
	    default:    /* illegal representation (cannot happen)     */
	      printf("%s: %d ",gst_ill_rep,rep);
	      gds_gst_print_binary_data(len - (rep + 1 - value),rep + 1);
	      break;
	  }
	}
      }
      rdn = *ava == D2_EOS ? ava : ava + 1;
      printf("\n");
    }
  }

}                       /* gds_gst_print_dist_name                    */


/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_options_serv_control                  */
/*                                                                    */
/* Description  : It prints the options of the service controls.      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_options_serv_control(
  signed32 options)
{
  printf("Options in service controls:\n");
  if                    /* Prefer chaining bit is set                 */
    (options & D2_PREF_CHAINING)
  {                     /* print prefer chaining                      */
    printf("preferChaining ");
  }
  if                    /* Chaining prohibited bit is set             */
    (options & D2_NOCHAINING)
  {                     /* print chaining prohibited                  */
    printf("chainingProhibited ");
  }
  if                    /* Local scope bit is set                     */
    (options & D2_LOCALSCOPE)
  {                     /* print local scope                          */
    printf("localScope ");
  }
  if                    /* No cache bit is set                        */
    (options & D2_NOCACHE)
  {                     /* print don't use copy                       */
    printf("dontUseCopy ");
  }
  if                    /* Don't dereference alias bit is set         */
    (options & D2_DREFALIAS)
  {                     /* print don't dereference alias              */
    printf("dontDereferenceAlias ");
  }
  if                    /* No referral bit is set                     */
    (options & D2_NOREFERRAL)
  {                     /* print return no referral                   */
    printf("returnNoReferral ");
  }
  if                    /* Use DUA Cache bit is set                   */
    (options & D2_DUACACHE)
  {                     /* print use DUA Cache                        */
    printf("useDUACache ");
  }
  if                    /* Use DSA bit is set                         */
    (options & D2_USEDSA)
  {                     /* print use DSA                              */
    printf("useDSA ");
  }
  if                    /* Use DUA Cache first bit is set             */
    (options & D2_DUAFIRST)
  {                     /* print DUA Cache first                      */
    printf("DUACacheFirst ");
  }
  if                    /* Resident class bit is set                  */
    (options & D2_RESIDENT_CLASS)
  {                     /* print resident class                       */
    printf("residentClass ");
  }
  if                    /* Privileged class bit is set                */
    (options & D2_PRIVILEGED_CLASS)
  {                     /* print privileged class                     */
    printf("privilegedClass ");
  }
  if                    /* Normal class bit is set                    */
    (options & D2_NORMAL_CLASS)
  {                     /* print normal class                         */
    printf("normalClass ");
  }
  if                    /* Don't store in Cache bit is set            */
    (options & D2_DONT_STORE)
  {                     /* print don't store in cache                 */
    printf("dontStoreInCache ");
  }
  if                    /* Append to file bit is set                  */
    (options & D2_FILE_MODE)
  {                     /* print append to file                       */
    printf("fileMode ");
  }
  if                    /* Prefer ADM functions bit is set            */
    (options & D2_PREF_ADM_FUN)
  {                     /* print prefer ADM functions                 */
    printf("preferAdmFunctions ");
  }
  printf("\n");
}                        /* end of gds_gst_print_options_serv_control */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_time_limit                            */
/*                                                                    */
/* Description  : It prints the time limit.			      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_time_limit(
  signed16 time_limit)
{
if (time_limit == D2_T_UNLIMITED)
  {
  printf("No time limit\n");
  }
else
  {
  printf("Time limit: %d\n",time_limit);
  }
} /* end of gds_gst_print_time_limit */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_size_limit                            */
/*                                                                    */
/* Description  : It prints the size limit.			      */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_size_limit(
  signed16 size_limit)
{
if (size_limit == D2_S_UNLIMITED)
  {
  printf("No size limit\n");
  }
else
  {
  printf("Size limit: %d\n",size_limit);
  }
} /* end of gds_gst_print_size_limit */
