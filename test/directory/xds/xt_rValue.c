/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_rValue.c,v $
 * Revision 1.1.10.2  1996/02/18  00:24:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:12:05  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:14:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:15  root]
 * 
 * Revision 1.1.8.6  1994/08/10  08:26:02  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:18:16  marrek]
 * 
 * Revision 1.1.8.5  1994/07/06  15:09:46  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:28:07  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:28:59  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:49:10  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:33:23  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  15:03:48  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  16:06:44  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  18:13:45  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:16:11  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:31:53  marrek]
 * 
 * Revision 1.1.6.3  1993/08/12  12:35:29  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:29:50  marrek]
 * 
 * Revision 1.1.6.2  1993/07/02  15:18:46  marrek
 * 	Changes for port to HP800.
 * 	[1993/07/02  14:36:41  marrek]
 * 
 * Revision 1.1.4.3  1993/01/05  20:34:45  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:49:18  htf]
 * 
 * Revision 1.1.4.2  1992/12/01  15:06:37  marrek
 * 	November 1992 code drop
 * 	[1992/12/01  13:21:33  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  03:28:02  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  23:08:01  melman]
 * 
 * Revision 1.1.2.2  1992/06/11  20:02:17  melman
 * 	gds drop
 * 	[1992/06/11  19:45:37  melman]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: xt_rValue.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 00:24:46 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton *************************************************************/
/*                                                                  */
/* TYPE         : MODUL                                             */
/*                                                                  */
/* NAME         : xt_rValue.c                                       */
/*                                                                  */
/* AUTHOR       : Cahill             D AP11                         */
/* DATE         : 10.00.1990                                        */
/*                                                                  */
/* KOMPONENTE   : XDS/XOM automatic test program                    */
/*                                                                  */
/* DOK.-NR.     :                                                   */
/*                                                                  */
/* PRD#/VERS.   :                                                   */
/*                                                                  */
/* DESCRIPTION  :                                                   */
/*                                                                  */
/* SYSTEMABHAENGIGKEIT: -                                       */
/*                                                                  */
/* HISTORIE     :                                                   */
/*                                                                  */
/* Vers.Nr. |  Datum  | Aenderungen                    | KZ | CR# FM#*/
/*          |     |                  |    |             */
/*          |     |                  |    |             */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                  */
/*          Module identifikation                                   */
/*                                                                  */
/**********************************************************************/

/**********************************************************************/
/*                                                                  */
/*            I N C L U D E S                                       */
/*                                                                  */
/**********************************************************************/

#ifdef THREADSAFE
#	include <pthread.h>
#endif
#include <stdio.h>
#include <xt.h>
#ifdef THREADSAFE
#	include <xt_thread.h>
#else
#	include <xt_nonthread.h>
#endif
#include <xt_rValue.h>

extern BOOLEAN xt_do_to_string_and_back;

OM_IMPORT (DS_C_DS_DN)
OM_IMPORT (DS_C_SEARCH_INFO)
OM_IMPORT (DS_C_PRESENTATION_ADDRESS)
OM_IMPORT (DS_C_PARTIAL_OUTCOME_QUAL)
OM_IMPORT (DS_C_LIST_INFO_ITEM)
OM_IMPORT (DS_C_LIST_INFO)
OM_IMPORT (DS_C_CONTINUATION_REF)
OM_IMPORT (DS_C_ACCESS_POINT)
OM_IMPORT (DS_C_OPERATION_PROGRESS)
OM_IMPORT (DS_C_ENTRY_INFO)
OM_IMPORT (DS_C_DS_RDN)
OM_IMPORT (DS_C_AVA)
OM_IMPORT (DS_C_ATTRIBUTE)
OM_IMPORT (DS_C_ATTRIBUTE_PROBLEM)
OM_IMPORT (DS_C_LIST_RESULT)
OM_IMPORT (DS_C_SEARCH_RESULT)
OM_IMPORT (DS_C_FACSIMILE_PHONE_NBR)
OM_IMPORT (DS_C_POSTAL_ADDRESS)
OM_IMPORT (DS_C_SEARCH_CRITERION)
OM_IMPORT (DS_C_SEARCH_GUIDE)
OM_IMPORT (DS_C_TELETEX_TERM_IDENT)
OM_IMPORT (DS_C_TELEX_NBR)
OM_IMPORT (DS_C_ENTRY_MOD)
OM_IMPORT (DS_C_FILTER_ITEM)
OM_IMPORT (DS_C_ATTRIBUTE_LIST)
OM_IMPORT (DS_C_REFERRAL)
OM_IMPORT (DS_C_ERROR)
OM_IMPORT (DS_C_SYSTEM_ERROR)
OM_IMPORT (DS_C_LIBRARY_ERROR)
OM_IMPORT (DS_C_COMMUNICATIONS_ERROR)
OM_IMPORT (DS_C_ATTRIBUTE_ERROR)
OM_IMPORT (DS_C_NAME_ERROR)
OM_IMPORT (DS_C_SECURITY_ERROR)
OM_IMPORT (DS_C_SERVICE_ERROR)
OM_IMPORT (DS_C_UPDATE_ERROR)
OM_IMPORT (DS_C_ABANDON_FAILED)
OM_IMPORT (DS_C_DL_SUBMIT_PERMS)
OM_IMPORT (MH_C_OR_ADDRESS)
OM_IMPORT (MH_C_OR_NAME)
OM_IMPORT (DSX_C_GDS_ACL)
OM_IMPORT (DSX_C_GDS_ACL_ITEM)
OM_IMPORT (DS_C_ALGORITHM_IDENT)
OM_IMPORT (DS_C_CERT)
OM_IMPORT (DS_C_CERT_LIST)
OM_IMPORT (DS_C_CERT_PAIR)
OM_IMPORT (DS_C_CERT_SUBLIST)
OM_IMPORT (DS_C_SIGNATURE)



#define XT_CMP_STRING(rc, type)                           \
        ((rc.length == type.length) &&                          \
        !(memcmp(rc.elements,type.elements, type.length))) ? 1 : 0

#define XT_NUM_READ_RESULT_ATTS                 5
#define XT_NUM_NAME_ATTS                        3
#define XT_NUM_RDN_ATTS                         3
#define XT_NUM_AVA_ATTS                         4
#define XT_NUM_ENTRY_ATTS                       5
#define XT_NUM_ATTRIBUTE_ATTS                   4
#define XT_NUM_SESSION_ATTS                     8
#define XT_NUM_ADDRESS_ATTS                     6
#define XT_NUM_COMPARE_RESULT_ATTS              7
#define XT_NUM_LIST_RESULT_ATTS                 4
#define XT_NUM_LIST_INFO_ATTS                   7
#define XT_NUM_PARTIAL_OUTCOME_QUALIFIER_ATTS   5
#define XT_NUM_CONTINUATION_REFERENCE_ATTS      7
#define XT_NUM_ACCESS_POINT_ATTS                4
#define XT_NUM_OPERATION_PROGRESS_ATTS          4
#define XT_NUM_LIST_INFO_ITEM_ATTS              5
#define XT_NUM_SEARCH_RESULT_ATTS               4
#define XT_NUM_SEARCH_INFO_ATTS                 7
#define XT_NUM_ABANDON_FAILED_ATTS              3
#define XT_NUM_ATTRIBUTE_ERROR_ATTS             4
#define XT_NUM_ATTRIBUTE_PROBLEM_ATTS           5
#define XT_NUM_COMMUNICATIONS_ERROR_ATTS        3
#define XT_NUM_LIBRARY_ERROR_ATTS               3
#define XT_NUM_NAME_ERROR_ATTS                  4
#define XT_NUM_REFERRAL_ATTS                    7
#define XT_NUM_SECURITY_ERROR_ATTS              3
#define XT_NUM_SERVICE_ERROR_ATTS               3
#define XT_NUM_SYSTEM_ERROR_ATTS                3
#define XT_NUM_UPDATE_ERROR_ATTS                3
#define XT_NUM_TELEX_NUM_ATTS                   5
#define XT_NUM_TELETEX_TERM_ID_ATTS             3
#define XT_NUM_SEARCH_GUIDE_ATTS                3
#define XT_NUM_SEARCH_CRIT_ATTS                 6
#define XT_NUM_POSTAL_ADDR_ATTS                 3
#define XT_NUM_FACSIMILE_TELE_NUM_ATTS          3
#define XT_NUM_DL_SUBMIT_PERMISSIONS            7
#define XT_NUM_OR_ADDRESS                       46
#define XT_NUM_OR_NAME                          47
#define XT_NUM_GDS_ACL                          7
#define XT_NUM_GDS_ACL_ITEM                     4
#define XT_NUM_ALGORITHM_IDENT					4
#define XT_NUM_CERT								12
#define XT_NUM_CERT_LIST						7
#define XT_NUM_CERT_PAIR						4
#define XT_NUM_CERT_SUBLIST						7
#define XT_NUM_SIGNATURE						5


/****************************************************************************/
			char *xt_status_to_buffer(
									DS_status status,
									char *buffer)
/****************************************************************************/
{
	OM_boolean			is_instance;
    if ((
		om_instance(status, DS_C_ERROR, &is_instance) == OM_SUCCESS &&
		is_instance == OM_TRUE
		) || (
		om_instance(status, DS_C_ATTRIBUTE_ERROR, &is_instance) == OM_SUCCESS &&
		is_instance == OM_TRUE
		)) {

		OM_return_code om_return_code;
		OM_string string;

		/* initialize string */
		string.elements = buffer;
		string.length = XT_MAX_XOMS_STRING;
		om_return_code = omX_object_to_string(
		/* OM_object object */				status,
		/* OM_boolean local_strings */		OM_TRUE,
		/* OM_string *string */				&string);

		if (om_return_code != OM_SUCCESS) {
			xt_log_both1("\txt_status_to_buffer: omX_object_to_string failed: %s", xt_om_error_to_string(om_return_code));
		} /* endif */
    } /* endif */
	return (buffer);
} /* end xt_status_to_buffer */

/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_status()                                 */
/*                                                                          */
/*    Compares the expected status returned with the received status        */
/*    returned                                                              */
/*                                                                          */
/* received_status == DS_SUCCESS  && *expected_status == DS_SUCCESS --OK    */
/* received_status == DS_SUCCESS  && *expected_status != DS_SUCCESS --ERROR */
/* received_status != DS_SUCCESS  && *expected_status == DS_SUCCESS --ERROR */
/* received_status != DS_SUCCESS  && *expected_status != DS_SUCCESS --COMPARE*/
/*                                                                          */
/****************************************************************************/
OM_sint xt_cmp_status (
	OM_private_object  received_status,
	OM_descriptor      *expected_status,
	/*OM_descriptor      **expected_status,*/
	enum f_type        func_called)
{
	const char function_name[] = "xt_cmp_status";
    BOOLEAN error_found = FALSE;
    OM_return_code      retc;
    OM_public_object    received_status_p;
    OM_value_position   total_number;
	char    status_buffer[XT_MAX_XOMS_STRING]; /* for status_to_string */
	char *calling_function;

	switch (func_called) {
		case XT_COPY:
			calling_function = "XT_COPY";
			break;
		case XT_CPYVAL:
			calling_function = "XT_CPYVAL";
			break;
		case XT_CREATE:
			calling_function = "XT_CREATE";
			break;
		case XT_DECODE:
			calling_function = "XT_DECODE";
			break;
		case XT_DELETE:
			calling_function = "XT_DELETE";
			break;
		case XT_ENCODE:
			calling_function = "XT_ENCODE";
			break;
		case XT_GET:
			calling_function = "XT_GET";
			break;
		case XT_INSTAN:
			calling_function = "XT_INSTAN";
			break;
		case XT_PUT:
			calling_function = "XT_PUT";
			break;
		case XT_OMREAD:
			calling_function = "XT_OMREAD";
			break;
		case XT_REMOVE:
			calling_function = "XT_REMOVE";
			break;
		case XT_WRITE:
			calling_function = "XT_WRITE";
			break;
		case XT_ABANDN:
			calling_function = "XT_ABANDN";
			break;
		case XT_ADDENT:
			calling_function = "XT_ADDENT";
			break;
		case XT_BIND:
			calling_function = "XT_BIND";
			break;
		case XT_COMPAR:
			calling_function = "XT_COMPAR";
			break;
		case XT_INIT:
			calling_function = "XT_INIT";
			break;
		case XT_LIST:
			calling_function = "XT_LIST";
			break;
		case XT_MODENT:
			calling_function = "XT_MODENT";
			break;
		case XT_MODRDN:
			calling_function = "XT_MODRDN";
			break;
		case XT_DSREAD:
			calling_function = "XT_DSREAD";
			break;
		case XT_RECRES:
			calling_function = "XT_RECRES";
			break;
		case XT_RMENT:
			calling_function = "XT_RMENT";
			break;
		case XT_SEARCH:
			calling_function = "XT_SEARCH";
			break;
		case XT_SHUTDN:
			calling_function = "XT_SHUTDN";
			break;
		case XT_UNBIND:
			calling_function = "XT_UNBIND";
			break;
		case XT_VERSN:
			calling_function = "XT_VERSN";
			break;
		case XT_BLDTREE:
			calling_function = "XT_BLDTREE";
			break;
		case XT_RMTREE:
			calling_function = "XT_RMTREE";
			break;
		case XT_FILL:
			calling_function = "XT_FILL";
			break;
		case XT_FILLO:
			calling_function = "XT_FILLO";
			break;
		case XT_EXTRACT:
			calling_function = "XT_EXTRACT";
			break;
		case XT_EXTRACTA:
			calling_function = "XT_EXTRACTA";
			break;
		case XT_S_TO_O:
			calling_function = "XT_S_TO_O";
			break;
		case XT_O_TO_S:
			calling_function = "XT_O_TO_S";
			break;
		default:
			xt_log_both1("unknown function in %s\n",function_name);
			calling_function = "UNKNOWN FUNCTION";
			break;
	} /* end switch */

	if (received_status != DS_SUCCESS && received_status != DS_NO_WORKSPACE ) {
#if 0
		if (xt_do_to_string_and_back) {
#endif /* 0 */
			xt_log_both1("\txt_cmp_status: received status from %s was:",calling_function);
			xt_log_both1("\"%s\""
			,xt_status_to_buffer(received_status,status_buffer));
#if 0
		} /* endif */
#endif /* 0 */
	} /* endif */

    if ((received_status == DS_SUCCESS && expected_status == DS_SUCCESS) ||
		(received_status==DS_NO_WORKSPACE && expected_status==DS_NO_WORKSPACE))
        return XT_OK;

    if (received_status != DS_SUCCESS && expected_status == DS_SUCCESS)  {
        xt_log_both1("xt_cmp_status: DS_SUCCESS expected but not returned from %s",calling_function);
        return (XT_ERROR);
    } /* endif */
    if (received_status!=DS_NO_WORKSPACE && expected_status==DS_NO_WORKSPACE) {
        xt_log_both1("xt_cmp_status: DS_NO_WORKSPACE expected but not returned from %s",calling_function);
        return (XT_ERROR);
    } /* endif */

    if (received_status==DS_NO_WORKSPACE && expected_status!=DS_NO_WORKSPACE) {
        xt_log_both1("xt_cmp_status: DS_NO_WORKSPACE unexpectedly returned from %s", calling_function);
        return (XT_ERROR);
    } /* endif */
    if (received_status == DS_SUCCESS && expected_status != DS_SUCCESS)  {
        xt_log_both1("xt_cmp_status: DS_SUCCESS unexpectedly returned from %s",calling_function);
        return (XT_ERROR);
    } /* endif */

    /* if here then received_status and expected_status must be error objects */

    /* get a public copy of received_status  */
    if ((retc = (om_get(received_status, OM_NO_EXCLUSIONS, ((OM_type_list) 0), OM_FALSE, ((OM_value_position) 0),
                        ((OM_value_position) 0), &received_status_p,    &total_number ))) !=  OM_SUCCESS) {
        xt_log_both1 ("\t(NOTICE) total_number from om_get for received_status = %s", xt_itoa(total_number));
        xt_log_both1 ("\t(NOTICE) retc from om_get for received_status = %s", xt_itoa(retc));
        xt_log_both ("\t(ERROR) retc from om_get for received_status not OM_SUCCESS");
        XT_LOG_ERROR( "om_get for received_status failed");
        return (XT_ERROR);
    } /* endif */
    xt_log_both1 ("\t(NOTICE) total_number from om_get for received_status = %s", xt_itoa(total_number));

    /* check if system-error */
    if ((!error_found) &&  (!memcmp ((char *)received_status_p->value.string.elements, OMP_D_DS_C_SYSTEM_ERROR,  received_status_p->value.string.length))) {
        error_found = TRUE;
        if (xt_cmp_system_error(received_status_p, expected_status) !=  XT_OK) {
            XT_LOG_ERROR( "error matching system_error attributes");
            return XT_ERROR;
        } /* endif */
    } /* endif */

    /* check if library-error */
    if ((!error_found) &&  (!memcmp ((char *)received_status_p->value.string.elements, OMP_D_DS_C_LIBRARY_ERROR,  received_status_p->value.string.length))) {
        error_found = TRUE;
        if (xt_cmp_library_error(received_status_p,     expected_status) !=  XT_OK) {
            XT_LOG_ERROR("error matching library_error attributes");
            return XT_ERROR;
        } /* endif */
    } /* endif */

    /* check if communications-error */
    if ((!error_found) &&  (!memcmp ((char *)received_status_p->value.string.elements, OMP_D_DS_C_COMMUNICATIONS_ERROR,
                        received_status_p->value.string.length)))
        if ((func_called == XT_RECRES) || (func_called == XT_UNBIND) ||  (func_called == XT_VERSN)) {
            XT_LOG_ERROR("communications error returned by this function - invalid error");
            return XT_ERROR;
        } /* endif */
        else {
            error_found = TRUE;
            if (xt_cmp_communications_error(received_status_p, expected_status) !=  XT_OK) {
                XT_LOG_ERROR("error matching communications_error attributes");
                return XT_ERROR;
            } /* endif */
        } /* endif */

    /* check if attribute-error */
    if ((!error_found) &&  (!memcmp ((char *)received_status_p->value.string.elements, OMP_D_DS_C_ATTRIBUTE_ERROR,
        received_status_p->value.string.length)))
        if (!((func_called == XT_ADDENT) ||     (func_called == XT_COMPAR) ||  (func_called == XT_MODENT) ||
            (func_called ==  XT_MODRDN) ||  (func_called ==  XT_DSREAD) || (func_called ==  XT_SEARCH))) {
            XT_LOG_ERROR("attribute error returned by this function - invalid error");
            return XT_ERROR;
        } /* endif */
        else {
            error_found = TRUE;
            if (xt_cmp_attribute_error(received_status_p, expected_status) !=  XT_OK) {
                XT_LOG_ERROR("error matching attribute_error attributes");
                return XT_ERROR;
            } /* endif */
        } /* endif */

    /* check if name-error */
    if ((!error_found) &&  (!memcmp ((char *)received_status_p->value.string.elements, OMP_D_DS_C_NAME_ERROR,      received_status_p->value.string.length)))
        if ((func_called == XT_BIND)   || (func_called == XT_UNBIND) ||  (func_called == XT_VERSN)  ||
            (func_called ==  XT_ABANDN) ||  (func_called ==  XT_RECRES))  {
            XT_LOG_ERROR("name error returned by this function - invalid error");
            return XT_ERROR;
        } /* endif */
        else {
            error_found = TRUE;
            if (xt_cmp_name_error(received_status_p, expected_status) !=  XT_OK) {
                XT_LOG_ERROR("error matching name_error attributes");
                return XT_ERROR;
            } /* endif */
        } /* endif */

    /* check if referral */
    if ((!error_found) &&  (!memcmp ((char *)received_status_p->value.string.elements, OMP_D_DS_C_REFERRAL,  received_status_p->value.string.length)))
        if ((func_called == XT_BIND)   || (func_called == XT_UNBIND) ||  (func_called == XT_VERSN)  ||
            (func_called ==  XT_ABANDN) ||  (func_called ==  XT_RECRES))  {
            XT_LOG_ERROR("referral returned by this function - invalid error");
            return XT_ERROR;
        } /* endif */
        else {
            error_found = TRUE;
            if (xt_cmp_referral(received_status_p, expected_status) !=  XT_OK) {
                XT_LOG_ERROR("error matching referral attributes");
                return XT_ERROR;
            } /* endif */
        } /* endif */

    /* check if security-error */
    if ((!error_found) &&  (!memcmp ((char *)received_status_p->value.string.elements, OMP_D_DS_C_SECURITY_ERROR,      received_status_p->value.string.length)))
        if ((func_called == XT_UNBIND) || (func_called == XT_VERSN)  ||  (func_called == XT_RECRES) ||
            (func_called ==  XT_ABANDN))  {
            XT_LOG_ERROR("security error returned by this function - invalid error");
            return XT_ERROR;
        } /* endif */
        else {
            error_found = TRUE;
            if (xt_cmp_security_error(received_status_p, expected_status) !=  XT_OK) {
                XT_LOG_ERROR("error matching security-error attributes");
                return XT_ERROR;
            } /* endif */
        } /* endif */

    /* check if service-error */
    if ((!error_found) &&  (!memcmp ((char *)received_status_p->value.string.elements, OMP_D_DS_C_SERVICE_ERROR,  received_status_p->value.string.length)))
        if ((func_called == XT_UNBIND) || (func_called == XT_VERSN)  ||  (func_called == XT_RECRES) ||
            (func_called ==  XT_ABANDN))  {
            XT_LOG_ERROR("service error returned by this function - invalid error");
            return XT_ERROR;
        } /* endif */
        else {
            error_found = TRUE;
            if (xt_cmp_service_error(received_status_p,     expected_status) !=  XT_OK) {
                XT_LOG_ERROR("error matching service-error attributes");
                return XT_ERROR;
            } /* endif */
        } /* endif */

    /* check if update-error */
    if ((!error_found) &&  (!memcmp ((char *)received_status_p->value.string.elements, OMP_D_DS_C_UPDATE_ERROR,  received_status_p->value.string.length)))
        if (!((func_called == XT_ADDENT) ||     (func_called == XT_MODENT) ||  (func_called == XT_MODRDN) ||
            (func_called ==  XT_RMENT)))  {
            XT_LOG_ERROR("update error returned by this function - invalid error");
            return XT_ERROR;
        } /* endif */
        else {
            error_found = TRUE;
            if (xt_cmp_update_error(received_status_p, expected_status) !=  XT_OK) {
                XT_LOG_ERROR("error matching update-error attributes");
                return XT_ERROR;
            } /* endif */
        } /* endif */

    if ((!error_found) &&  (!memcmp ((char *)received_status_p->value.string.elements, OMP_D_DS_C_ABANDON_FAILED,      received_status_p->value.string.length)))
        if (!(func_called == XT_ABANDN)) {
            XT_LOG_ERROR("abandon failed returned by this function - invalid error");
            return XT_ERROR;
        } /* endif */
        else {
            error_found = TRUE;
            if (xt_cmp_abandon_failed(received_status_p, expected_status) !=  XT_OK) {
                XT_LOG_ERROR("error matching abandon_failed attributes");
                return XT_ERROR;
            } /* endif */
        } /* endif */

    if (!error_found) {
        XT_LOG_ERROR( "no error attribute received");
        return XT_ERROR;
    } /* endif */

    return(XT_OK);
} /* end xt_cmp_status */




/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_system_error()                           */
/*                                                                          */
/*    Compares an expected system_error with a received system_error        */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_system_error ( OM_public_object  received_system_error,
OM_public_object  expected_system_error )
{
	const char function_name[] = "xt_cmp_system_error";
    struct attribute_struct system_error_atts_tbl [XT_NUM_SYSTEM_ERROR_ATTS] =
    {
        { OM_CLASS,                 0, 0, ONE,      NULL, NULL },
        { DS_PROBLEM,               0, 0,   ONE,     NULL, NULL  },

        NULL_ATTRIBUTE
    };
    attribute            * atts_tbl = system_error_atts_tbl;
    OM_return_code      retc;

    /* check received_system_error and expected_system_error against table of attributes */
    if (xt_check_atts (received_system_error, expected_system_error, atts_tbl) == XT_ERROR)  {
        XT_LOG_ERROR("error in received system_error attributes");
        xt_log_both ("\t(ERROR) error in received system_error attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_system_error, expected_system_error, XT_NUM_SYSTEM_ERROR_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_system_error */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_update_error()                           */
/*                                                                          */
/*    Compares an expected update_error with a received update_error        */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_update_error ( OM_public_object  received_update_error,
OM_public_object  expected_update_error )
{
	const char function_name[] = "xt_cmp_update_error";
    struct attribute_struct update_error_atts_tbl [XT_NUM_UPDATE_ERROR_ATTS] =
    {
        { OM_CLASS,                   0, 0, ONE,      NULL, NULL },
        { DS_PROBLEM,                 0, 0,     ONE,      NULL, NULL },


        NULL_ATTRIBUTE                      };
    attribute            * atts_tbl = update_error_atts_tbl;
    OM_return_code      retc;

    /* check received_update_error and expected_update_error against table of attributes */
    if (xt_check_atts (received_update_error, expected_update_error, atts_tbl) == XT_ERROR)  {
        XT_LOG_ERROR("error in received update_error attributes");
        xt_log_both ("\t(ERROR) error in received update_error attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_update_error, expected_update_error, XT_NUM_UPDATE_ERROR_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_update_error */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_library_error()                          */
/*                                                                          */
/*    Compares an expected library_error with a received library_error      */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_library_error ( OM_public_object  received_library_error,
OM_public_object  expected_library_error )
{
	const char function_name[] = "xt_cmp_library_error";
    struct attribute_struct library_error_atts_tbl [XT_NUM_LIBRARY_ERROR_ATTS] =
    {
        { OM_CLASS,                       0, 0, ONE,      NULL, NULL },
        { DS_PROBLEM,                     0, 0,     ONE,      NULL, NULL },

        NULL_ATTRIBUTE                      };
    attribute            * atts_tbl = library_error_atts_tbl;
    OM_return_code      retc;

    /*  check r_library_err and e_library_err against  */
    /*  table of permitted attributes                  */
    if (xt_check_atts (received_library_error, expected_library_error, atts_tbl) ==  XT_ERROR) {
        XT_LOG_ERROR( "error in received library_error attributes");
        xt_log_both ("\terror in received library_error attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_library_error, expected_library_error, XT_NUM_LIBRARY_ERROR_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_library_error */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_communications_error()                   */
/*                                                                          */
/*    Compares an expected communications_error with a received             */
/*    communications_error                                                  */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_communications_error ( OM_public_object  received_communications_error,
OM_public_object  expected_communications_error )
{
	const char function_name[] = "xt_cmp_communications_error";
    struct attribute_struct communications_error_atts_tbl [XT_NUM_COMMUNICATIONS_ERROR_ATTS] =
    {
        { OM_CLASS,                         0, 0, ONE,      NULL, NULL },
        { DS_PROBLEM,                     0, 0,     ONE,      NULL, NULL },

        NULL_ATTRIBUTE
    };
    attribute            * atts_tbl = communications_error_atts_tbl;
    OM_return_code      retc;

    /*  check received_communications_error and expected_communications_error  */
    /*  against table of permitted attributes                    */
    if (xt_check_atts (received_communications_error, expected_communications_error, atts_tbl) == XT_ERROR)  {
        XT_LOG_ERROR("error in received communications_error attributes");
        xt_log_both ("\t(ERROR) error in received communications_error attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_communications_error, expected_communications_error, XT_NUM_COMMUNICATIONS_ERROR_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_communications_error */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_attribute_error()                        */
/*                                                                          */
/*    Compares an expected attribute_error with a received attribute_error  */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_attribute_error ( OM_public_object  received_attribute_error,
OM_public_object  expected_attribute_error )
{
	const char function_name[] = "xt_cmp_attribute_error";
    struct attribute_struct attribute_error_atts_tbl [XT_NUM_ATTRIBUTE_ERROR_ATTS] =
    {
        { OM_CLASS,                         0, 0, ONE,      NULL, NULL },
        { DS_OBJECT_NAME,                 0, 0,     ONE,      NULL, NULL },
        { DS_PROBLEMS,                    0, 0,     ONE_OR_MORE, NULL, NULL  },

        NULL_ATTRIBUTE
    };
    attribute            * atts_tbl = attribute_error_atts_tbl;
    OM_return_code      retc;

    /*  check r_attribute_err and e_attribute_err against  */
    /*  table of permitted attributes                      */
    if (xt_check_atts (received_attribute_error, expected_attribute_error, atts_tbl) ==  XT_ERROR) {
        XT_LOG_ERROR("error in received attribute_error attributes");
        xt_log_both ("\t(ERROR) error in received attribute_error attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_attribute_error,     expected_attribute_error, XT_NUM_ATTRIBUTE_ERROR_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_attribute_error */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_name_error()                             */
/*                                                                          */
/*    Compares an expected name_error with a received name_error            */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_name_error ( OM_public_object  received_name_error,
OM_public_object  expected_name_error )
{
	const char function_name[] = "xt_cmp_name_error";
    struct attribute_struct name_error_atts_tbl [XT_NUM_NAME_ERROR_ATTS] =
    {
        { OM_CLASS,                         0, 0, ONE,      NULL, NULL },
        { DS_MATCHED,                     0, 0,     ONE,      NULL, NULL },
        { DS_PROBLEM,                     0, 0,     ONE,      NULL, NULL },


        NULL_ATTRIBUTE
    };
    attribute            * atts_tbl = name_error_atts_tbl;
    OM_return_code      retc;

    /* check r_name_err and e_name_err against table of permitted attributes */
    if (xt_check_atts (received_name_error,     expected_name_error, atts_tbl) ==  XT_ERROR) {
        XT_LOG_ERROR("error in received name_error attributes");
        xt_log_both ("\t(ERROR) error in received name_error attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_name_error, expected_name_error, XT_NUM_NAME_ERROR_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_name_error */




/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_referral()                               */
/*                                                                          */
/*    Compares an expected referral with a received referral                */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_referral ( OM_public_object  received_referral,
OM_public_object  expected_referral )
{
	const char function_name[] = "xt_cmp_referral";
    struct attribute_struct referral_atts_tbl [XT_NUM_REFERRAL_ATTS] =
    {
        { OM_CLASS,                         0, 0, ONE,      NULL, NULL },
        { DS_ACCESS_POINTS,                       0, 0,     ONE_OR_MORE, NULL, NULL  },
        { DS_ALIASED_RDNS,                0, 0,     ONE,      NULL, NULL },
        { DS_OPERATION_PROGRESS,          0, 0,     ONE,      NULL, NULL },
        { DS_RDNS_RESOLVED,                       0, 0,     ZERO_OR_ONE, NULL, NULL  },
        { DS_TARGET_OBJECT,                       0, 0,     ONE,      NULL, NULL },

        NULL_ATTRIBUTE
    };
    attribute            * atts_tbl = referral_atts_tbl;
    OM_return_code      retc;

    /* check received_referral and expected_referral against table of permitted attributes */
    if (xt_check_atts (received_referral, expected_referral, atts_tbl) ==  XT_ERROR) {
        XT_LOG_ERROR("error in received referral attributes");
        xt_log_both ("\terror in received referral attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_referral, expected_referral, XT_NUM_REFERRAL_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_referral */




/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_security_error()                         */
/*                                                                          */
/*    Compares an expected security_error with a received security_error    */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_security_error ( OM_public_object  received_security_error,
OM_public_object  expected_security_error )
{
	const char function_name[] = "xt_cmp_security_error";
    struct attribute_struct security_error_atts_tbl [XT_NUM_SECURITY_ERROR_ATTS] =
    {
        { OM_CLASS,                         0, 0, ONE,      NULL,
        NULL },
        { DS_PROBLEM,                     0, 0,     ONE,      NULL,
        NULL },


        NULL_ATTRIBUTE
    };
    attribute            * atts_tbl = security_error_atts_tbl;
    OM_return_code      retc;

    /*  check r_security_err and e_security_err against  */
    /*  table of permitted attributes                    */
    if (xt_check_atts (received_security_error,     expected_security_error, atts_tbl) == XT_ERROR)  {
        XT_LOG_ERROR("error in received security_error attributes");
        xt_log_both ("\t(ERROR) error in received security_error attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_security_error, expected_security_error, XT_NUM_SECURITY_ERROR_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_security_error */




/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_service_error()                          */
/*                                                                          */
/*    Compares an expected service_error with a received service_error      */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_service_error ( OM_public_object  received_service_error,
OM_public_object  expected_service_error )
{
	const char function_name[] = "xt_cmp_service_error";
    struct attribute_struct service_error_atts_tbl [XT_NUM_SERVICE_ERROR_ATTS] =
    {
        { OM_CLASS,                         0, 0, ONE,      NULL,
        NULL },
        { DS_PROBLEM,                     0, 0,     ONE,      NULL,
        NULL },


        NULL_ATTRIBUTE
    };
    attribute            * atts_tbl = service_error_atts_tbl;
    OM_return_code      retc;

    /*  check r_service_err and e_service_err against  */
    /*  table of permitted attributes                    */
    if (xt_check_atts (received_service_error, expected_service_error, atts_tbl) ==  XT_ERROR) {
        XT_LOG_ERROR( "error in received service_error attributes");
        xt_log_both ("\t(ERROR) error in received service_error attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_service_error, expected_service_error, XT_NUM_SERVICE_ERROR_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_service_error */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_abandon_failed()                         */
/*                                                                          */
/*    Compares an expected abandon_failed with a received abandon_failed    */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_abandon_failed ( OM_public_object  received_abandon_failed,
OM_public_object  expected_abandon_failed )
{
	const char function_name[] = "xt_cmp_abandon_failed";
    struct attribute_struct abandon_failed_atts_tbl [XT_NUM_ABANDON_FAILED_ATTS] =
    {
        { OM_CLASS,                         0, 0, ONE,      NULL,
        NULL },
        { DS_PROBLEM,                     0, 0,     ONE,      NULL,
        NULL },


        NULL_ATTRIBUTE
    };
    attribute            * atts_tbl = abandon_failed_atts_tbl;
    OM_return_code      retc;

    /*  check received_abandon_failed and expected_abandon_failed against  */
    /*  table of permitted attributes                        */
    if (xt_check_atts (received_abandon_failed,     expected_abandon_failed, atts_tbl) == XT_ERROR)  {
        XT_LOG_ERROR("error in received abandon_failed attributes");
        xt_log_both ("\t(ERROR) error in received abandon_failed attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_abandon_failed, expected_abandon_failed, XT_NUM_ABANDON_FAILED_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_abandon_failed */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_session()                                */
/*                                                                          */
/* Compares the expected session, expected_session, from the testcase file, */
/* with the session returned from the interface function, received_session  */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_session ( OM_private_object received_session,
OM_descriptor    *expected_session )
{
	const char function_name[] = "xt_cmp_session";
    struct attribute_struct session_atts_tbl [XT_NUM_SESSION_ATTS] =
    {
        { OM_CLASS,                         0, 0, ONE,      NULL,
        NULL },
        { DS_DSA_ADDRESS,                 0, 0,     ZERO_OR_ONE,
        NULL, NULL },
        { DS_DSA_NAME,                    0, 0,     ZERO_OR_ONE,
        NULL, NULL },
        { DS_FILE_DESCRIPTOR,             0, 0,     ONE,      NULL,
        NULL },
        { DS_REQUESTOR,                           0, 0,     ZERO_OR_ONE,
        NULL, NULL },
        { DSX_PASSWORD,                           0, 0,     ZERO_OR_ONE,
        NULL, NULL },
        { DSX_DIR_ID,                     0, 0,     ONE,      NULL,
        NULL },


        NULL_ATTRIBUTE
    };
    OM_public_object    received_session_p;
    attribute            * atts_tbl = session_atts_tbl;
    OM_return_code      retc;
    OM_value_position   total_number;

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&received_session,     "received_session");
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* get a public copy of received_session  */
    if ((retc = (om_get (received_session, OM_NO_EXCLUSIONS, ((OM_type_list) 0), OM_FALSE, ((OM_value_position) 0),
                        ((OM_value_position) 0), &received_session_p, &total_number ))) !=  OM_SUCCESS)  {
        xt_log_both1 ("\t(NOTICE) total_number from om_get for received_session = %s", xt_itoa(total_number));
        xt_log_both1 ("\t(NOTICE) retc from om_get for received_session = %s", xt_itoa(retc));
        xt_log_both ("\t(ERROR) retc from om_get for received_session not OM_SUCCESS");
        XT_LOG_ERROR( "om_get for received_session failed");
        return (XT_ERROR);
    } /* endif */
    xt_log_both1 ("\t(NOTICE) total_number from om_get for received_session = %s", xt_itoa(total_number));

    /* check received_session and expected_session against a table of permitted attributes */
    if (xt_check_atts (received_session_p, expected_session, atts_tbl) ==  XT_ERROR) {
        XT_LOG_ERROR( "error in received session attributes");
        xt_log_both ("\t(ERROR) error in received session attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_session_p, expected_session, XT_NUM_SESSION_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_session */




/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_compare_result()                         */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_compare_result ( OM_private_object received_result,
OM_descriptor    *expected_result )
{
	const char function_name[] = "xt_cmp_compare_result";

    struct attribute_struct compare_result_atts_tbl [XT_NUM_COMPARE_RESULT_ATTS]
	= {
        { OM_CLASS,                  0, 0,	   ONE,         NULL, NULL },
        { DS_FROM_ENTRY,             0, 0,     ONE,         NULL, NULL },
        { DS_MATCHED,                0, 0,     ONE,         NULL, NULL },
        { DS_OBJECT_NAME,            0, 0,     ZERO_OR_ONE, NULL, NULL },
        { DS_ALIAS_DEREFERENCED,     0, 0,     ONE,         NULL, NULL },
        { DS_PERFORMER,              0, 0,     ZERO_OR_ONE, NULL, NULL },
        NULL_ATTRIBUTE
    };
    OM_public_object    received_result_p;
    attribute            * atts_tbl = compare_result_atts_tbl;
    OM_return_code      retc;
    OM_value_position   total_number;


    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&received_result, "received_result");
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* get a public copy of received_result */
    if ((retc = (om_get (received_result, OM_NO_EXCLUSIONS,((OM_type_list) 0), OM_FALSE, ((OM_value_position) 0), ((OM_value_position) 0), &received_result_p,    &total_number ))) !=  OM_SUCCESS)  {
        xt_log_both1 ("\t(NOTICE) total_number from om_get for received_result = %s", xt_itoa(total_number));
        xt_log_both1 ("\t(NOTICE) retc from om_get for received_result = %s", xt_itoa(retc));
        xt_log_both ("\t(ERROR) retc from om_get for received_result not OM_SUCCESS");
        XT_LOG_ERROR( "om_get for received_result failed");
        return (XT_ERROR);
    } /* endif */
    xt_log_both1 ("\t(NOTICE) total_number from om_get for received_result = %s", xt_itoa(total_number));

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&received_result_p, "received_result_p");
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* check received_result and expected_result against a table of permitted attributes */
    if (xt_check_atts (received_result_p, expected_result, atts_tbl)
	==  XT_ERROR) {
        XT_LOG_ERROR("error in received compare_result attributes");
        xt_log_both ("\t(ERROR) error in received compare_result attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_result_p,
	expected_result, XT_NUM_COMPARE_RESULT_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_compare_result */




/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_list_result()                            */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_list_result ( OM_private_object received_result,
OM_descriptor    *expected_result )
{
	const char function_name[] = "xt_cmp_list_result";
    struct attribute_struct list_result_atts_tbl [XT_NUM_LIST_RESULT_ATTS] =
    {
        { OM_CLASS,                      0, 0,     ONE,         NULL, NULL },
        { DS_LIST_INFO,                  0, 0,     ZERO_OR_ONE, NULL, NULL },
        { DS_UNCORRELATED_LIST_INFO,     0, 0,     ZERO_OR_MORE,NULL, NULL },
        NULL_ATTRIBUTE
    };
    OM_public_object    received_result_p;
    attribute            * atts_tbl = list_result_atts_tbl;
    OM_return_code      retc;
    OM_value_position   total_number;


    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&received_result, "received_result");
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* get a public copy of received_result */
    if ((retc = (om_get (received_result, OM_NO_EXCLUSIONS, ((OM_type_list) 0),
	OM_FALSE, ((OM_value_position) 0), ((OM_value_position) 0),
	&received_result_p, &total_number ))) !=  OM_SUCCESS)  {
        xt_log_both1 ("\t(NOTICE) total_number from om_get for received_result = %s",
		xt_itoa(total_number));
        xt_log_both1 ("\t(NOTICE) retc from om_get for received_result = %s",
		xt_itoa(retc));
        xt_log_both ("\t(ERROR) retc from om_get for received_result not OM_SUCCESS");
        XT_LOG_ERROR( "om_get for received_result failed");
        return (XT_ERROR);
    } /* endif */

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&received_result_p, "received_result_p");
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* check received_result and expected_result against a table of permitted attributes */
    if (xt_check_atts (received_result_p, expected_result, atts_tbl) ==  XT_ERROR) {
        XT_LOG_ERROR("error in received list_result attributes");
        xt_log_both ("\t(ERROR) error in received list_result attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_result_p, expected_result,
	XT_NUM_LIST_RESULT_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_list_result */




/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_read_result()                            */
/*                                                                          */
/*Compares the expected read_result, expected_result, from the testcase file*/
/* with the read_result returned from the interface function, received_result*/
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_read_result ( OM_private_object received_result,
OM_descriptor    *expected_result )
{
	const char function_name[] = "xt_cmp_read_result";
    struct attribute_struct read_result_atts_tbl [XT_NUM_READ_RESULT_ATTS] =
    {
        { OM_CLASS,                 0, 0, ONE,          NULL, NULL },
        { DS_ALIAS_DEREFERENCED,    0, 0, ONE,          NULL, NULL },
        { DS_PERFORMER,             0, 0, ZERO_OR_ONE,  NULL, NULL },
        { DS_ENTRY,                 0, 0, ONE,          NULL, NULL },
        NULL_ATTRIBUTE
    };
    OM_public_object    received_result_p;
    attribute            * atts_tbl = read_result_atts_tbl;
    OM_return_code      retc;
    OM_value_position   total_number;

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&received_result, "received_result");
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* get a public copy of received_result */
    if ((retc = (om_get (received_result, OM_NO_EXCLUSIONS,     ((OM_type_list) 0), OM_FALSE, ((OM_value_position) 0),
                        ((OM_value_position) 0), &received_result_p,    &total_number ))) !=  OM_SUCCESS)  {
        xt_log_both1 ("\t(NOTICE) total_number from om_get for received_result = %s", xt_itoa(total_number));
        xt_log_both1 ("\t(NOTICE) retc from om_get for received_result = %s", xt_itoa(retc));
        xt_log_both ("\t(ERROR) retc from om_get for received_result not OM_SUCCESS");
        XT_LOG_ERROR( "om_get for received_result failed");
        return (XT_ERROR);
    } /* endif */
    xt_log_both1 ("\t(NOTICE) total_number from om_get for received_result = %s", xt_itoa(total_number));

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&received_result_p, "received_result_p");
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* check received_result and expected_result against a table of permitted attributes */
    if (xt_check_atts (received_result_p, expected_result, atts_tbl) ==  XT_ERROR) {
        XT_LOG_ERROR("error in received read_result attributes");
        xt_log_both ("\t(ERROR) error in received read_result attributes");
        return (XT_ERROR);
    } /* endif */

    retc = xt_compare_atts (atts_tbl, received_result_p, expected_result, XT_NUM_READ_RESULT_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_read_result */




/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_search_result()                          */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_search_result ( OM_private_object received_result,
OM_descriptor    *expected_result )
{
	const char function_name[] = "xt_cmp_search_result";
    struct attribute_struct search_result_atts_tbl [XT_NUM_SEARCH_RESULT_ATTS] =
    {
        { OM_CLASS,                     0, 0, ONE,          NULL, NULL },
        { DS_SEARCH_INFO,               0, 0, ZERO_OR_ONE,  NULL, NULL },
        { DS_UNCORRELATED_SEARCH_INFO,  0, 0, ZERO_OR_MORE, NULL, NULL },
        NULL_ATTRIBUTE
    };
    OM_public_object    received_result_p;
    attribute            * atts_tbl = search_result_atts_tbl;
    OM_return_code      retc;
    OM_value_position   total_number;

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&received_result, "received_result");
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* get a public copy of received_result */
    if ((retc = (om_get (received_result, OM_NO_EXCLUSIONS,
	((OM_type_list) 0), OM_FALSE, ((OM_value_position) 0),
	((OM_value_position) 0), &received_result_p, &total_number )))
	!=  OM_SUCCESS)  {
        xt_log_both1 ("\t(NOTICE) total_number from om_get for received_result = %s", xt_itoa(total_number));
        xt_log_both1 ("\t(NOTICE) retc from om_get for received_result = %s", xt_itoa(retc));
        xt_log_both ("\t(ERROR) retc from om_get for received_result not OM_SUCCESS");
        XT_LOG_ERROR( "om_get for received_result failed");
        return (XT_ERROR);
    } /* endif */
    xt_log_both1 ("\t(NOTICE) total_number from om_get for received_result = %s", xt_itoa(total_number));

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&received_result_p, "received_result_p");
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* check received_result and expected_result against a table of permitted attributes */
    if (xt_check_atts (received_result_p, expected_result, atts_tbl) ==  XT_ERROR) {
        XT_LOG_ERROR("error in received search_result attributes");
        xt_log_both ("\t(ERROR) error in received search_result attributes");
        return (XT_ERROR);
    } /* endif */

    /* compare received_result and expected_result */
    retc = xt_compare_atts (atts_tbl, received_result_p, expected_result,
	XT_NUM_SEARCH_RESULT_ATTS);

    xt_free_index (atts_tbl);

    return (retc);
} /* end xt_cmp_search_result */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_obj()                                    */
/*                                                                          */
/****************************************************************************/

static OM_sint xt_cmp_obj ( OM_type        received_type,
OM_type        expected_type,
OM_descriptor  *r_obj,
OM_descriptor  *e_obj )
{
	const char function_name[] = "xt_cmp_obj";
    struct attribute_struct name_atts_tbl [XT_NUM_NAME_ATTS] = {
        { OM_CLASS,             0, 0, ONE,      NULL, NULL },
        { DS_RDNS,              0, 0, ZERO_OR_MORE,     NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct rdn_atts_tbl [XT_NUM_RDN_ATTS] = {
        { OM_CLASS,             0, 0, ONE,      NULL, NULL },
        { DS_AVAS,              0, 0, ZERO_OR_MORE,     NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct ava_atts_tbl [XT_NUM_AVA_ATTS] = {
        { OM_CLASS,             0, 0, ONE,      NULL, NULL },
        { DS_ATTRIBUTE_TYPE,    0, 0, ONE,      NULL, NULL },
        { DS_ATTRIBUTE_VALUES,  0, 0, ONE,      NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct entry_information_atts_tbl [XT_NUM_ENTRY_ATTS] =
    {
        { OM_CLASS,             0, 0, ONE,      NULL, NULL },
        { DS_FROM_ENTRY,        0, 0, ONE,      NULL, NULL },
        { DS_OBJECT_NAME,       0, 0, ONE,      NULL, NULL },
        { DS_ATTRIBUTES,        0, 0, ZERO_OR_MORE,     NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct attributes_atts_tbl [XT_NUM_ATTRIBUTE_ATTS] =
    {
        { OM_CLASS,             0, 0, ONE,      NULL, NULL },
        { DS_ATTRIBUTE_TYPE,    0, 0, ONE,      NULL, NULL },
        { DS_ATTRIBUTE_VALUES,  0, 0, ZERO_OR_MORE,     NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct address_atts_tbl [XT_NUM_ADDRESS_ATTS] =
    {
        { OM_CLASS,             0, 0, ONE,      NULL, NULL },
        { DS_N_ADDRESSES,       0, 0, ONE_OR_MORE, NULL, NULL },
        { DS_P_SELECTOR,        0, 0, ZERO_OR_ONE, NULL, NULL },
        { DS_S_SELECTOR,        0, 0, ZERO_OR_ONE, NULL, NULL },
        { DS_T_SELECTOR,        0, 0, ZERO_OR_ONE, NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct list_result_atts_tbl [XT_NUM_LIST_RESULT_ATTS] =
    {
        { OM_CLASS,                         0, 0, ONE,      NULL, NULL },
        { DS_LIST_INFO,                     0, 0, ZERO_OR_ONE, NULL, NULL },
        { DS_UNCORRELATED_LIST_INFO,        0, 0, ZERO_OR_MORE, NULL, NULL  },
        NULL_ATTRIBUTE
    };
    struct attribute_struct list_info_atts_tbl [XT_NUM_LIST_INFO_ATTS] =
    {
        { OM_CLASS,                     0, 0, ONE,      NULL, NULL },
        { DS_OBJECT_NAME,               0, 0, ZERO_OR_ONE, NULL, NULL },
        { DS_PARTIAL_OUTCOME_QUAL,      0, 0, ZERO_OR_ONE,  NULL, NULL },
        { DS_SUBORDINATES,              0, 0, ZERO_OR_MORE,     NULL, NULL },
        { DS_ALIAS_DEREFERENCED,        0, 0, ONE,      NULL, NULL },
        { DS_PERFORMER,                 0, 0, ZERO_OR_ONE, NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct partial_outcome_qualifier_atts_tbl [XT_NUM_PARTIAL_OUTCOME_QUALIFIER_ATTS] =
    {
        { OM_CLASS,             0, 0, ONE,      NULL, NULL },
        { DS_LIMIT_PROBLEM,     0, 0, ONE,      NULL, NULL },
        { DS_UNAVAILABLE_CRIT_EXT,  0, 0, ONE,      NULL, NULL },
        { DS_UNEXPLORED,        0, 0, ZERO_OR_MORE,     NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct continuation_reference_atts_tbl [XT_NUM_CONTINUATION_REFERENCE_ATTS] =
    {
        { OM_CLASS,             0, 0, ONE,      NULL, NULL },
        { DS_ACCESS_POINTS,     0, 0, ONE_OR_MORE, NULL, NULL },
        { DS_ALIASED_RDNS,      0, 0, ONE,      NULL, NULL },
        { DS_OPERATION_PROGRESS,0, 0, ONE,      NULL, NULL },
        { DS_RDNS_RESOLVED,     0, 0, ZERO_OR_ONE, NULL, NULL },
        { DS_TARGET_OBJECT,     0, 0, ONE,      NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct access_point_atts_tbl [XT_NUM_ACCESS_POINT_ATTS] =
    {
        { OM_CLASS,             0, 0, ONE,      NULL, NULL },
        { DS_ADDRESS,           0, 0, ONE,      NULL, NULL },
        { DS_AE_TITLE,          0, 0, ONE,      NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct operation_progress_atts_tbl [XT_NUM_OPERATION_PROGRESS_ATTS] =
    {
        { OM_CLASS,                   0, 0,     ONE,      NULL, NULL },
        { DS_NAME_RESOLUTION_PHASE,   0, 0,     ONE,      NULL, NULL },
        { DS_NEXT_RDN_TO_BE_RESOLVED, 0, 0,     ZERO_OR_ONE, NULL, NULL     },
        NULL_ATTRIBUTE
    };
    struct attribute_struct list_info_item_atts_tbl [XT_NUM_LIST_INFO_ITEM_ATTS] =
    {
        { OM_CLASS,             0, 0, ONE,      NULL, NULL },
        { DS_ALIAS_ENTRY,       0, 0, ONE,      NULL, NULL },
        { DS_FROM_ENTRY,        0, 0, ONE,      NULL, NULL },
        { DS_RDN,               0, 0, ONE,      NULL, NULL },

        NULL_ATTRIBUTE
    };
    struct attribute_struct search_result_atts_tbl [XT_NUM_SEARCH_RESULT_ATTS] =
    {
        { OM_CLASS,                     0, 0,     ONE,      NULL, NULL },
        { DS_SEARCH_INFO,               0, 0,     ZERO_OR_ONE, NULL, NULL },
        { DS_UNCORRELATED_SEARCH_INFO,  0, 0,     ZERO_OR_MORE, NULL, NULL },


        NULL_ATTRIBUTE
    };
    struct attribute_struct search_info_atts_tbl [XT_NUM_SEARCH_INFO_ATTS] =
    {
        { OM_CLASS,                 0, 0,     ONE,      NULL, NULL },
        { DS_ENTRIES,               0, 0,     ZERO_OR_MORE, NULL, NULL },
        { DS_OBJECT_NAME,           0, 0,     ZERO_OR_ONE, NULL, NULL },
        { DS_PARTIAL_OUTCOME_QUAL,  0, 0,     ZERO_OR_ONE , NULL, NULL },
        { DS_ALIAS_DEREFERENCED,    0, 0,     ONE,      NULL, NULL },
        { DS_PERFORMER,             0, 0,     ZERO_OR_ONE, NULL, NULL },


        NULL_ATTRIBUTE
    };
    struct attribute_struct attribute_problem_atts_tbl [XT_NUM_ATTRIBUTE_PROBLEM_ATTS] =
    {
        { OM_CLASS,                       0, 0,     ONE,      NULL, NULL },
        { DS_ATTRIBUTE_TYPE,              0, 0,     ONE,      NULL, NULL },
        { DS_ATTRIBUTE_VALUE,             0, 0,     ZERO_OR_ONE, NULL, NULL },
        { DS_PROBLEM,                     0, 0,     ONE,      NULL, NULL },


        NULL_ATTRIBUTE
    };
    struct attribute_struct facsimile_telephone_num_atts_tbl [XT_NUM_FACSIMILE_TELE_NUM_ATTS] =
    {
        { OM_CLASS,                        0, 0, ONE,      NULL, NULL },
        { DS_PHONE_NBR,                    0, 0, ONE,      NULL, NULL },


        NULL_ATTRIBUTE
    };
    struct attribute_struct postal_addr_atts_tbl [XT_NUM_POSTAL_ADDR_ATTS] =
    {
        { OM_CLASS,                       0, 0,     ONE,      NULL, NULL },
        { DS_POSTAL_ADDRESS,              0, 0,     ONE_TO_SIX, NULL,
        NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct search_criterion_atts_tbl [XT_NUM_SEARCH_CRIT_ATTS] =
    {
        { OM_CLASS,                       0, 0,     ONE,      NULL, NULL },
        { DS_ATTRIBUTE_TYPE,              0, 0,     ZERO_OR_ONE, NULL, NULL },
        { DS_CRITERIA,                    0, 0,     ZERO_OR_MORE, NULL, NULL },
        { DS_FILTER_ITEM_TYPE,            0, 0,     ZERO_OR_ONE, NULL, NULL },
        { DS_FILTER_TYPE,                 0, 0,     ONE,      NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct search_guide_atts_tbl [XT_NUM_SEARCH_GUIDE_ATTS] =
    {
        { OM_CLASS,                       0, 0,     ONE,      NULL, NULL },
        { DS_CRITERIA,                    0, 0,     ONE,      NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct teletex_term_id_atts_tbl [XT_NUM_TELETEX_TERM_ID_ATTS] =
    {
        { OM_CLASS,                   0, 0,     ONE,      NULL, NULL },
        { DS_TELETEX_TERM,            0, 0,     ONE,      NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct telex_num_atts_tbl [XT_NUM_TELEX_NUM_ATTS] =
    {
        { OM_CLASS,                       0, 0, ONE,      NULL, NULL },
        { DS_ANSWERBACK,                  0, 0, ONE,      NULL, NULL },
        { DS_COUNTRY_CODE,                0, 0, ONE,      NULL, NULL },
        { DS_TELEX_NBR,                   0, 0, ONE,      NULL, NULL },


        NULL_ATTRIBUTE
    };
    struct attribute_struct dl_subm_perm_atts_tbl [XT_NUM_DL_SUBMIT_PERMISSIONS] =
    {
        { OM_CLASS,                      0, 0,     ONE,      NULL, NULL },
        { DS_PERM_TYPE,                  0, 0,     ONE,      NULL, NULL },
        { DS_INDIVIDUAL,                 0, 0,     ZERO_OR_ONE, NULL, NULL },
        { DS_MEMBER_OF_DL,               0, 0,     ZERO_OR_ONE, NULL, NULL },
        { DS_PATTERN_MATCH,              0, 0,     ZERO_OR_ONE, NULL, NULL },
        { DS_MEMBER_OF_GROUP,            0, 0,     ZERO_OR_ONE, NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct or_address_atts_tbl [XT_NUM_OR_ADDRESS] =
    {
        { OM_CLASS,                       0, 0,     ONE,      NULL, NULL },
        { MH_T_ADMD_NAME,                 0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_COMMON_NAME,               0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_COUNTRY_NAME,              0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_DOMAIN_TYPE_1,             0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_TYPE_2,             0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_TYPE_3,             0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_TYPE_4,             0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_VALUE_1,            0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_VALUE_2,            0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_VALUE_3,            0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_VALUE_4,            0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_GENERATION,                0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_GIVEN_NAME,                0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_INITIALS,                  0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_ISDN_NUMBER,               0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_ISDN_SUBADDRESS,           0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_NUMERIC_USER_IDENTIFIER,   0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_ORGANIZATION_NAME,         0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_ORGANIZATIONAL_UNIT_NAME_1,0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_ORGANIZATIONAL_UNIT_NAME_2,0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_ORGANIZATIONAL_UNIT_NAME_3,0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_ORGANIZATIONAL_UNIT_NAME_4,0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_ADDRESS_DETAILS,    0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_ADDRESS_IN_FULL,    0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_POSTAL_ADDRESS_IN_LINES,   0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_POSTAL_CODE,               0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_POSTAL_COUNTRY_NAME,       0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_POSTAL_DELIVERY_POINT_NAME,0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_DELIV_SYSTEM_NAME,  0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_POSTAL_GENERAL_DELIV_ADDR, 0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_LOCALE,             0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_OFFICE_BOX_NUMBER,  0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_OFFICE_NAME,        0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_OFFICE_NUMBER,      0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_ORGANIZATION_NAME,  0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_PATRON_DETAILS,     0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_PATRON_NAME,        0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_STREET_ADDRESS,     0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_PRESENTATION_ADDRESS,      0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_PRMD_NAME,                 0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_SURNAME,                   0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_TERMINAL_IDENTIFIER,       0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_TERMINAL_TYPE,             0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_X121_ADDRESS,              0, 0,     ZERO_OR_ONE, NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct or_name_atts_tbl [XT_NUM_OR_NAME] =  {
        { OM_CLASS,                       0, 0, 	ONE,      NULL, NULL },
        { MH_T_DIRECTORY_NAME,            0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_ADMD_NAME,                 0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_COMMON_NAME,               0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_COUNTRY_NAME,              0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_DOMAIN_TYPE_1,             0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_TYPE_2,             0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_TYPE_3,             0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_TYPE_4,             0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_VALUE_1,            0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_VALUE_2,            0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_VALUE_3,            0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_DOMAIN_VALUE_4,            0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_GENERATION,                0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_GIVEN_NAME,                0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_INITIALS,                  0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_ISDN_NUMBER,               0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_ISDN_SUBADDRESS,           0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_NUMERIC_USER_IDENTIFIER,   0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_ORGANIZATION_NAME,         0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_ORGANIZATIONAL_UNIT_NAME_1,0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_ORGANIZATIONAL_UNIT_NAME_2,0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_ORGANIZATIONAL_UNIT_NAME_3,0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_ORGANIZATIONAL_UNIT_NAME_4,0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_ADDRESS_DETAILS,    0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_ADDRESS_IN_FULL,    0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_POSTAL_ADDRESS_IN_LINES,   0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_POSTAL_CODE,               0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_POSTAL_COUNTRY_NAME,       0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_POSTAL_DELIVERY_POINT_NAME,0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_DELIV_SYSTEM_NAME,  0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_POSTAL_GENERAL_DELIV_ADDR, 0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_LOCALE,             0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_OFFICE_BOX_NUMBER,  0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_OFFICE_NAME,        0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_OFFICE_NUMBER,      0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_ORGANIZATION_NAME,  0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_PATRON_DETAILS,     0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_PATRON_NAME,        0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_POSTAL_STREET_ADDRESS,     0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_PRESENTATION_ADDRESS,      0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_PRMD_NAME,                 0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_SURNAME,                   0, 0,     ZERO_TO_TWO, NULL, NULL },
        { MH_T_TERMINAL_IDENTIFIER,       0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_TERMINAL_TYPE,             0, 0,     ZERO_OR_ONE, NULL, NULL },
        { MH_T_X121_ADDRESS,              0, 0,     ZERO_OR_ONE, NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct gds_acl_atts_tbl [XT_NUM_GDS_ACL] =  {
        { OM_CLASS,                       0, 0, 	ONE,      NULL, NULL },
        { DSX_MODIFY_PUBLIC,              0, 0,     ZERO_OR_MORE, NULL, NULL },
        { DSX_READ_STANDARD,              0, 0,     ZERO_OR_MORE, NULL, NULL },
        { DSX_MODIFY_STANDARD,            0, 0,     ZERO_OR_MORE, NULL, NULL },
        { DSX_READ_SENSITIVE,             0, 0,     ZERO_OR_MORE, NULL, NULL },
        { DSX_MODIFY_SENSITIVE,           0, 0,     ZERO_OR_MORE, NULL, NULL },
        NULL_ATTRIBUTE
    };
    struct attribute_struct gds_acl_item_atts_tbl [XT_NUM_GDS_ACL_ITEM] =
    {
        { OM_CLASS,                       0, 0, ONE,      NULL, NULL },
        { DSX_INTERPRETATION,             0, 0, ZERO_OR_MORE, NULL, NULL },
        { DSX_USER,                       0, 0, ZERO_OR_MORE, NULL, NULL },
        NULL_ATTRIBUTE
    };

    struct attribute_struct algorithm_ident_atts_tbl [XT_NUM_ALGORITHM_IDENT] =
    {
        { OM_CLASS,                       0, 0, ONE,      NULL, NULL },
        { DS_ALGORITHM,                   0, 0, ONE,      NULL, NULL },
        { DS_ALGORITHM_PARAMETERS,        0, 0, ZERO_OR_ONE,  NULL, NULL },
        NULL_ATTRIBUTE
    };

    struct attribute_struct cert_atts_tbl [XT_NUM_CERT] =
    {
        { OM_CLASS,                       0, 0, ONE,      NULL, NULL },
        { DS_ISSUER,                      0, 0, ONE,      NULL, NULL },
        { DS_SIGNATURE,                   0, 0, ONE,      NULL, NULL },
        { DS_SIGNATURE_VALUE,             0, 0, ONE,      NULL, NULL },
        { DS_SERIAL_NBR,                  0, 0, ONE,      NULL, NULL },
        { DS_SUBJECT,                     0, 0, ONE,      NULL, NULL },
        { DS_SUBJECT_ALGORITHM,           0, 0, ONE,      NULL, NULL },
        { DS_SUBJECT_PUBLIC_KEY,          0, 0, ONE,      NULL, NULL },
        { DS_VALIDITY_NOT_AFTER,          0, 0, ONE,      NULL, NULL },
        { DS_VALIDITY_NOT_BEFORE,         0, 0, ONE,      NULL, NULL },
        { DS_VERSION,                     0, 0, ONE,      NULL, NULL },
        NULL_ATTRIBUTE
    };

    struct attribute_struct cert_list_atts_tbl [XT_NUM_CERT_LIST] =
    {
        { OM_CLASS,                       0, 0, ONE,      NULL, NULL },
        { DS_ISSUER,                      0, 0, ONE,      NULL, NULL },
        { DS_SIGNATURE,                   0, 0, ONE,      NULL, NULL },
        { DS_SIGNATURE_VALUE,             0, 0, ONE,      NULL, NULL },
        { DS_LAST_UPDATE,                 0, 0, ONE,      NULL, NULL },
        { DS_REVOKED_CERTS,               0, 0, ZERO_OR_MORE,   NULL, NULL },
        NULL_ATTRIBUTE
    };

    struct attribute_struct cert_pair_atts_tbl [XT_NUM_CERT_PAIR] =
    {
        { OM_CLASS,                       0, 0, ONE,      NULL, NULL },
        { DS_FORWARD,                     0, 0, ZERO_OR_ONE,   NULL, NULL },
        { DS_REVERSE,                     0, 0, ZERO_OR_ONE,   NULL, NULL },
		NULL_ATTRIBUTE };
		
	struct attribute_struct cert_sublist_atts_tbl [XT_NUM_CERT_SUBLIST] =
    {
        { OM_CLASS,                       0, 0, ONE,      NULL, NULL },
        { DS_ISSUER,                      0, 0, ONE,      NULL, NULL },
        { DS_SIGNATURE,                   0, 0, ONE,      NULL, NULL },
        { DS_SIGNATURE_VALUE,             0, 0, ONE,      NULL, NULL },
        { DS_REVOC_DATE,                  0, 0, ONE,      NULL, NULL },
        { DS_SERIAL_NBRS,                 0, 0, ONE,      NULL, NULL },
        NULL_ATTRIBUTE
    };
	struct attribute_struct signature_atts_tbl [XT_NUM_SIGNATURE] =
    {
        { OM_CLASS,                       0, 0, ONE,      NULL, NULL },
        { DS_ISSUER,                      0, 0, ONE,      NULL, NULL },
        { DS_SIGNATURE,                   0, 0, ONE,      NULL, NULL },
        { DS_SIGNATURE_VALUE,             0, 0, ONE,      NULL, NULL },
        NULL_ATTRIBUTE
    };




    attribute            * atts_tbl;
    OM_sint num_atts = 0;
    OM_return_code      retc;

    if (received_type != expected_type)  {
        XT_LOG_ERROR( "received and expected types differ");
        xt_log_both ("\t(ERROR) received and expected types differ");
        return (XT_ERROR);
    } /* endif */

    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_SEARCH_INFO))  {
        atts_tbl = search_info_atts_tbl;
        num_atts = XT_NUM_SEARCH_INFO_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_PRESENTATION_ADDRESS)) {
        atts_tbl = address_atts_tbl;
        num_atts = XT_NUM_ADDRESS_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_PARTIAL_OUTCOME_QUAL )) {
        atts_tbl = partial_outcome_qualifier_atts_tbl;
        num_atts = XT_NUM_PARTIAL_OUTCOME_QUALIFIER_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_LIST_INFO_ITEM ))  {
        atts_tbl = list_info_item_atts_tbl;
        num_atts = XT_NUM_LIST_INFO_ITEM_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_LIST_INFO )) {
        atts_tbl = list_info_atts_tbl;
        num_atts = XT_NUM_LIST_INFO_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_CONTINUATION_REF )) {
        atts_tbl = continuation_reference_atts_tbl;
        num_atts = XT_NUM_CONTINUATION_REFERENCE_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_ACCESS_POINT )) {
        atts_tbl = access_point_atts_tbl;
        num_atts = XT_NUM_ACCESS_POINT_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_OPERATION_PROGRESS ))  {
        atts_tbl = operation_progress_atts_tbl;
        num_atts = XT_NUM_OPERATION_PROGRESS_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_DS_DN )) {
        atts_tbl = name_atts_tbl;
        num_atts = XT_NUM_NAME_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_ENTRY_INFO ))  {
        atts_tbl = entry_information_atts_tbl;
        num_atts = XT_NUM_ENTRY_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_DS_RDN ))  {
        atts_tbl = rdn_atts_tbl;
        num_atts = XT_NUM_RDN_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_AVA )) {
        atts_tbl = ava_atts_tbl;
        num_atts = XT_NUM_AVA_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_ATTRIBUTE )) {
        atts_tbl = attributes_atts_tbl;
        num_atts = XT_NUM_ATTRIBUTE_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_ATTRIBUTE_PROBLEM )) {
        atts_tbl = attribute_problem_atts_tbl;
        num_atts = XT_NUM_ATTRIBUTE_PROBLEM_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_LIST_RESULT )) {
        atts_tbl = list_result_atts_tbl;
        num_atts = XT_NUM_LIST_RESULT_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_SEARCH_RESULT )) {
        atts_tbl = search_result_atts_tbl;
        num_atts = XT_NUM_SEARCH_RESULT_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_FACSIMILE_PHONE_NBR))  {
        atts_tbl = facsimile_telephone_num_atts_tbl;
        num_atts = XT_NUM_FACSIMILE_TELE_NUM_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_POSTAL_ADDRESS ))  {
        atts_tbl = postal_addr_atts_tbl;
        num_atts = XT_NUM_POSTAL_ADDR_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_SEARCH_CRITERION )) {
        atts_tbl = search_criterion_atts_tbl;
        num_atts = XT_NUM_SEARCH_CRIT_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_SEARCH_GUIDE )) {
        atts_tbl = search_guide_atts_tbl;
        num_atts = XT_NUM_SEARCH_GUIDE_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_TELETEX_TERM_IDENT)) {
        atts_tbl = teletex_term_id_atts_tbl;
        num_atts = XT_NUM_TELETEX_TERM_ID_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_TELEX_NBR )) {
        atts_tbl = telex_num_atts_tbl;
        num_atts = XT_NUM_TELEX_NUM_ATTS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_DL_SUBMIT_PERMS )) {
        atts_tbl = dl_subm_perm_atts_tbl ;
        num_atts = XT_NUM_DL_SUBMIT_PERMISSIONS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, MH_C_OR_NAME )) {
        atts_tbl = or_address_atts_tbl ;
        num_atts = XT_NUM_OR_ADDRESS;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, MH_C_OR_ADDRESS ))  {
        atts_tbl = or_name_atts_tbl ;
        num_atts = XT_NUM_OR_NAME;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DSX_C_GDS_ACL )) {
        atts_tbl = gds_acl_atts_tbl ;
        num_atts = XT_NUM_GDS_ACL;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DSX_C_GDS_ACL_ITEM )) {
        atts_tbl = gds_acl_item_atts_tbl ;
        num_atts = XT_NUM_GDS_ACL_ITEM;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_ALGORITHM_IDENT)) {
        atts_tbl = algorithm_ident_atts_tbl ;
        num_atts = XT_NUM_ALGORITHM_IDENT;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_CERT)) {
        atts_tbl = cert_atts_tbl ;
        num_atts = XT_NUM_CERT;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_CERT_LIST)) {
        atts_tbl = cert_list_atts_tbl ;
        num_atts = XT_NUM_CERT_LIST;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_CERT_PAIR)) {
        atts_tbl = cert_pair_atts_tbl ;
        num_atts = XT_NUM_CERT_PAIR;
    } /* endif */
    if (XT_CMP_STRING(r_obj[0].value.string, DS_C_CERT_SUBLIST)) {
        atts_tbl = cert_sublist_atts_tbl ;
        num_atts = XT_NUM_CERT_SUBLIST;
    } /* endif */
    if (num_atts == 0) {
        XT_LOG_ERROR( "attribute type not recognised");
        xt_log_both ("\t(ERROR) attribute type not recognised");
        return (XT_ERROR);
    } /* endif */


    /* check r_obj and e_obj against a table of permitted attributes */
    if (xt_check_atts (r_obj, e_obj, atts_tbl) == XT_ERROR)  {
        XT_LOG_ERROR( "error in received attributes");
        xt_log_both ("\t(ERROR) error in received attributes");
        return (XT_ERROR);
    } /* endif */

    /* compare r_obj and e_obj */
    retc = (xt_compare_atts (atts_tbl, r_obj, e_obj, num_atts));

    xt_free_index (atts_tbl);

    return (retc);

} /* end xt_cmp_obj */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_integer()                                */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_integer ( OM_sint    received_integer,
OM_sint    expected_integer )
{
    if (received_integer != expected_integer)
        return(XT_NOT_MATCHED);
    else
        return(XT_OK);
} /* end xt_cmp_integer */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_enum()                                   */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_enum ( OM_enumeration  received_enum,
OM_enumeration  expected_enum )
{
    if (expected_enum != received_enum)
        return(XT_NOT_MATCHED);
    else
        return(XT_OK);
} /* end xt_cmp_enum  */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_boolean()                                */
/*                                                                          */
/*    Compares an expected boolean value with a received boolean value      */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_boolean ( OM_boolean received_boolean,
OM_boolean expected_boolean )
{
    if (expected_boolean == OM_FALSE) {
        if (received_boolean != OM_FALSE)
            return(XT_NOT_MATCHED);
    } else {                      /* expected_boolean != OM_FALSE */
        if (received_boolean == OM_FALSE)
            return(XT_NOT_MATCHED);
    } /* endif */
    return(XT_OK);
} /* end xt_cmp_boolean */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_class()                                  */
/*                                                                          */
/*    Compares an expected class with a received class                      */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_class ( OM_object_identifier    received_class,
OM_object_identifier    expected_class )
{
    OM_sint retc = XT_NOT_MATCHED;

    if ((received_class.length == expected_class.length) &&          !(memcmp((char
    *)received_class.elements, (char *)expected_class.elements, expected_class.length)))
    retc = XT_OK;
    else {
        if (((XT_CMP_STRING(received_class, DS_C_ATTRIBUTE))
		||  (XT_CMP_STRING(received_class, DS_C_AVA))
		||  (XT_CMP_STRING(received_class, DS_C_ENTRY_MOD))
		||  (XT_CMP_STRING(received_class, DS_C_FILTER_ITEM))) 
		&&
        ((XT_CMP_STRING(expected_class, DS_C_ATTRIBUTE))
		||  (XT_CMP_STRING(expected_class, DS_C_AVA))  
		||  (XT_CMP_STRING(expected_class, DS_C_ENTRY_MOD)) 
		||  (XT_CMP_STRING(expected_class, DS_C_FILTER_ITEM))))
            retc = XT_OK;

        if (((XT_CMP_STRING(received_class, DS_C_ATTRIBUTE_LIST)) 
		||  (XT_CMP_STRING(received_class, DS_C_ENTRY_INFO))) 
		&&
        ((XT_CMP_STRING(expected_class, DS_C_ATTRIBUTE_LIST)) 
		|| (XT_CMP_STRING(expected_class, DS_C_ENTRY_INFO))))
            retc = XT_OK;

        if (((XT_CMP_STRING(received_class, DS_C_CONTINUATION_REF))  
		||  (XT_CMP_STRING(received_class, DS_C_REFERRAL))) 
		&&
		   ((XT_CMP_STRING(expected_class, DS_C_CONTINUATION_REF))  
		|| (XT_CMP_STRING(expected_class, DS_C_REFERRAL))))
            retc = XT_OK;
    } /* endif */

    return retc;
} /* end xt_cmp_class */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_string()                                 */
/*                                                                          */
/*    Compares an expected string with a received string                    */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_string ( OM_string    received_string,
OM_string    expected_string )
{
    OM_sint i, r, e;
    char    *pr;
    char    *pe;

    if (received_string.length == expected_string.length) {
        if (memcmp ((char *)received_string.elements, (char *)expected_string.elements, expected_string.length)) {

            pr = (char *)received_string.elements;
            pe = (char *)expected_string.elements;
            for (i = 0; i <     received_string.length; i++) {
                if (isalpha(pr[i]))
                    r = tolower(pr[i]);
                else
                    r = pr[i];
                if (isalpha(pe[i]))
                    e = tolower(pe[i]);
                else
                    e = pe[i];
                if (r != e)
                    return(XT_NOT_MATCHED);
            } /* endfor */
        } /* endif */
    } else {
        return(XT_NOT_MATCHED);
    } /* endif */

    return(XT_OK);
} /* end xt_cmp_string */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_return_code()                            */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_return_code ( OM_uint    received_return_code,
OM_uint    expected_return_code )
{
    if (received_return_code != expected_return_code)
        return(XT_NOT_MATCHED);
    else
        return(XT_OK);
} /* end xt_cmp_return_code */



/****************************************************************************/
/*                                                                          */
/*                      xt_cmp_object()                                 */
/*                                                                          */
/****************************************************************************/

OM_sint xt_cmp_object ( OM_descriptor    *received_object,
OM_descriptor    *expected_object )
{
    OM_sint i;
	const char function_name[] = "xt_cmp_object";

    for (i = 0; (!IS_OM_NULL_DESCRIPTOR(received_object[i])); i++) {
        if (received_object[i].type !=  expected_object[i].type) {
            return (XT_ERROR);
        } /* endif */
        if (received_object[i].syntax != expected_object[i].syntax)  {
            return (XT_ERROR);
        } /* endif */
        switch (expected_object[i].syntax & OM_S_SYNTAX) {
        case OM_S_BOOLEAN :
            if (xt_cmp_boolean (received_object[i].value.boolean, expected_object[i].value.boolean) !=  XT_OK)
                return (XT_ERROR);
            break;
        case OM_S_OBJECT :
            if (xt_cmp_object (received_object[i].value.object.object, expected_object[i].value.object.object) !=
                XT_OK)
                return (XT_ERROR);
            break;
        case OM_S_INTEGER :
            if (xt_cmp_integer (received_object[i].value.integer, expected_object[i].value.integer) !=  XT_OK)
                return (XT_ERROR);
            break;
        case OM_S_ENUMERATION :
            if (xt_cmp_enum (received_object[i].value.enumeration, expected_object[i].value.enumeration) !=
                XT_OK)
                return (XT_ERROR);
            break;
        default :                 /* are all the rest strings ??? */
            if (xt_cmp_string (received_object[i].value.string, expected_object[i].value.string) !=  XT_OK)
                return (XT_ERROR);
            break;
        } /* endif */
    } /* endfor */
    return (XT_OK);
} /* end xt_cmp_object */


static struct om_type_table_struc {
	char *string;
	int number;
}om_type_table[] = {
	{"OM_NO_MORE_TYPES",0},
	{"OM_ARBITRARY_ENCODING",1},
	{"OM_ASN1_ENCODING",2},
	{"OM_CLASS",3},
	{"OM_DATA_VALUE_DESCRIPTOR",4},
	{"OM_DIRECT_REFERENCE",5},
	{"OM_INDIRECT_REFERENCE",6},
	{"OM_OBJECT_CLASS",7},
	{"OM_OBJECT_ENCODING",8},
	{"OM_OCTET_ALIGNED_ENCODING",9},
	{"OM_PRIVATE_OBJECT",10},
	{"OM_RULES",11},
	{"DS_ACCESS_POINTS",701},
	{"DS_ADDRESS",702},
	{"DS_AE_TITLE",703},
	{"DS_ALIASED_RDNS",704},
	{"DS_ALIAS_DEREFERENCED",705},
	{"DS_ALIAS_ENTRY",706},
	{"DS_ALL_ATTRIBUTES",707},
	{"DS_ASYNCHRONOUS",708},
	{"DS_ATTRIBUTES",709},
	{"DS_ATTRIBUTES_SELECTED",710},
	{"DS_ATTRIBUTE_TYPE",711},
	{"DS_ATTRIBUTE_VALUE",712},
	{"DS_ATTRIBUTE_VALUES",713},
	{"DS_AUTOMATIC_CONTINUATION",714},
	{"DS_AVAS",715},
	{"DS_CHAINING_PROHIB",716},
	{"DS_CHANGES",717},
	{"DS_CRIT",718},
	{"DS_DONT_DEREFERENCE_ALIASES",719},
	{"DS_DONT_USE_COPY",720},
	{"DS_DSA_ADDRESS",721},
	{"DS_DSA_NAME",722},
	{"DS_ENTRIES",723},
	{"DS_ENTRY",724},
	{"DS_EXT",725},
	{"DS_FILE_DESCRIPTOR",726},
	{"DS_FILTERS",727},
	{"DS_FILTER_ITEMS",728},
	{"DS_FILTER_ITEM_TYPE",729},
	{"DS_FILTER_TYPE",730},
	{"DS_FINAL_SUBSTRING",731},
	{"DS_FROM_ENTRY",732},
	{"DS_IDENT",733},
	{"DS_INFO_TYPE",734},
	{"DS_INITIAL_SUBSTRING",735},
	{"DS_ITEM_PARAMETERS",736},
	{"DS_LIMIT_PROBLEM",737},
	{"DS_LIST_INFO",738},
	{"DS_LOCAL_SCOPE",739},
	{"DS_MATCHED",740},
	{"DS_MOD_TYPE",741},
	{"DS_NAME_RESOLUTION_PHASE",742},
	{"DS_NEXT_RDN_TO_BE_RESOLVED",743},
	{"DS_N_ADDRESSES",744},
	{"DS_OBJECT_NAME",745},
	{"DS_OPERATION_PROGRESS",746},
	{"DS_PARTIAL_OUTCOME_QUAL",747},
	{"DS_PERFORMER",748},
	{"DS_PREFER_CHAINING",749},
	{"DS_PRIORITY",750},
	{"DS_PROBLEM",751},
	{"DS_PROBLEMS",752},
	{"DS_P_SELECTOR",753},
	{"DS_RDN",754},
	{"DS_RDNS",755},
	{"DS_RDNS_RESOLVED",756},
	{"DS_REQUESTOR",757},
	{"DS_SCOPE_OF_REFERRAL",758},
	{"DS_SEARCH_INFO",759},
	{"DS_SIZE_LIMIT",760},
	{"DS_SUBORDINATES",761},
	{"DS_S_SELECTOR",762},
	{"DS_TARGET_OBJECT",763},
	{"DS_TIME_LIMIT",764},
	{"DS_T_SELECTOR",765},
	{"DS_UNAVAILABLE_CRIT_EXT",766},
	{"DS_UNCORRELATED_LIST_INFO",767},
	{"DS_UNCORRELATED_SEARCH_INFO",768},
	{"DS_UNEXPLORED",769},
	{"DS_ANSWERBACK",801},
	{"DS_COUNTRY_CODE",802},
	{"DS_CRITERIA",803},
	{"DS_OBJECT_CLASS",804},
	{"DS_PARAMETERS",805},
	{"DS_POSTAL_ADDRESS",806},
	{"DS_PHONE_NBR",807},
	{"DS_TELETEX_TERM",808},
	{"DS_TELEX_NBR",809},
	{"DSX_PASSWORD",850},
	{"DSX_DIR_ID",851},
	{"DSX_DUAFIRST",852},
	{"DSX_DONT_STORE",853},
	{"DSX_NORMAL_CLASS",854},
	{"DSX_PRIV_CLASS",855},
	{"DSX_RESIDENT_CLASS",856},
	{"DSX_USEDSA",857},
	{"DSX_DUA_CACHE",858},
	{"DSX_MODIFY_PUBLIC",859},
	{"DSX_READ_STANDARD",860},
	{"DSX_MODIFY_STANDARD",861},
	{"DSX_READ_SENSITIVE",862},
	{"DSX_MODIFY_SENSITIVE",863},
	{"DSX_INTERPRETATION",864},
	{"DSX_USER",865},
	{"DSX_PREFER_ADM_FUNCS",866},
	{"DSX_AUTH_MECHANISM",867},
	{"DSX_AUTH_INFO",868},
	{"DSX_SIGN_MECHANISM",869},
	{"DSX_PROT_REQUEST",870},
	{"DS_PERM_TYPE",901},
	{"DS_INDIVIDUAL",902},
	{"DS_MEMBER_OF_DL",903},
	{"DS_PATTERN_MATCH",904},
	{"DS_MEMBER_OF_GROUP",905},
	{"DS_ALGORITHM",821},
	{"DS_FORWARD",822},
	{"DS_ISSUER",823},
	{"DS_LAST_UPDATE",824},
	{"DS_ALGORITHM_PARAMETERS",825},
	{"DS_REVERSE",826},
	{"DS_REVOC_DATE",827},
	{"DS_REVOKED_CERTS",828},
	{"DS_SERIAL_NBR",829},
	{"DS_SERIAL_NBRS",830},
	{"DS_SIGNATURE",831},
	{"DS_SIGNATURE_VALUE",832},
	{"DS_SUBJECT",833},
	{"DS_SUBJECT_ALGORITHM",834},
	{"DS_SUBJECT_PUBLIC_KEY",835},
	{"DS_VALIDITY_NOT_AFTER",836},
	{"DS_VALIDITY_NOT_BEFORE",837},
	{"DS_VERSION",838}
};

const int om_type_table_len = sizeof(om_type_table)/sizeof(om_type_table[0]);

char *om_type_string(OM_type arg)
{
	int i;
	for ( i = 0; i < om_type_table_len; i++) {
		if (om_type_table[i].number == arg) {
			return(om_type_table[i].string);
		} /* endif */
	} /* endfor */
	return("UNKNOWN TYPE");
} /* end om_type_string */


/****************************************************************************/
/*                                                                          */
/*                      xt_check_atts()                                 */
/*                                                                          */
/*      This checks the expected and received lists of attributes against   */
/*      a table of "correct" attributes.                                    */
/*                                                                          */
/****************************************************************************/

OM_sint xt_check_atts (
	OM_descriptor        *p_rv,
	OM_descriptor        *p_ev,
	attribute            *p_atts_tbl)
{
	const char function_name[] = "xt_check_atts";
    OM_sint total_atts_found = 0;
    OM_sint r_total = 0;
    OM_sint e_total = 0;
    OM_sint i, j, k, l;
    OM_sint xt_return_code = XT_OK;
	OM_boolean warning_issued = OM_FALSE;


    for (j = 0; !IS_NULL_ATTRIBUTE(p_atts_tbl[j]); j++)  {

        p_atts_tbl[j].r_number_found = p_atts_tbl[j].e_number_found =  0;
        p_atts_tbl[j].r_index = p_atts_tbl[j].e_index = NULL;

        for (i = 0; (!IS_OM_NULL_DESCRIPTOR(p_rv[i])); i++)  {
            if (p_rv[i].type == p_atts_tbl[j].att_type)
                p_atts_tbl[j].r_number_found++;
        } /* endif */
        if (p_atts_tbl[j].r_number_found) {
            if (p_atts_tbl[j].r_index)
                free(p_atts_tbl[j].r_index);
            if ((p_atts_tbl[j].r_index = (OM_sint *)malloc(
			(p_atts_tbl[j].r_number_found) *  (sizeof(OM_sint)))) == NULL) {
                xt_log_both1("\t(ERROR) malloc of r_index for att type %s failed",
				xt_itoa(p_atts_tbl[j].att_type));
                XT_LOG_ERROR( "malloc failed");
                xt_return_code =  XT_ERROR;
				goto end;
            } /* endif */
        } /* endif */
        for (k = l = 0; (k <= i) && (l < p_atts_tbl[j].r_number_found); k++) {
            if (p_rv[k].type == p_atts_tbl[j].att_type)
                p_atts_tbl[j].r_index[l++] = k;
        } /* endif */
        if (!r_total)
            r_total = i;

        for (i = 0; (!IS_OM_NULL_DESCRIPTOR(p_ev[i])); i++)  {
            if (p_ev[i].type == p_atts_tbl[j].att_type)
                p_atts_tbl[j].e_number_found++;
        } /* endif */
        if (p_atts_tbl[j].e_number_found) {
            if (p_atts_tbl[j].e_index)
                free(p_atts_tbl[j].e_index);
            if ((p_atts_tbl[j].e_index = (OM_sint *)malloc(
			(p_atts_tbl[j].e_number_found) *  (sizeof(OM_sint)))) == NULL) {
                xt_log_both1("\t(ERROR) malloc of e_index for att type %s failed",
				xt_itoa(p_atts_tbl[j].att_type));
                XT_LOG_ERROR( "malloc failed");
                xt_return_code =  XT_ERROR;
				goto end;
            } /* endif */
        } /* endif */
        for (k = l = 0; (k <= i) && (l < p_atts_tbl[j].e_number_found); k++) {
            if (p_ev[k].type == p_atts_tbl[j].att_type)
                p_atts_tbl[j].e_index[l++] = k;
        } /* endif */
        if (!e_total)
            e_total = i;

        if ((p_atts_tbl[j].r_number_found == 0) 
		&& !((p_atts_tbl[j].number_allowed ==  ZERO_OR_ONE)  
		|| (p_atts_tbl[j].number_allowed ==  ZERO_OR_MORE) 
		||  (p_atts_tbl[j].number_allowed ==  ZERO_TO_TWO))) {
            xt_log_both2(
			"\t(ERROR) at least one attribute of type %s required, received %s",
			xt_itoa(p_atts_tbl[j].att_type),
			xt_itoa(p_atts_tbl[j].r_number_found));
            xt_return_code = XT_ERROR;
			goto end;
        } /* endif */

        if ((p_atts_tbl[j].r_number_found > 1) 
		&& !((p_atts_tbl[j].number_allowed ==  ZERO_OR_MORE) 
		|| (p_atts_tbl[j].number_allowed == ONE_OR_MORE)
		||  (p_atts_tbl[j].number_allowed ==  ZERO_TO_TWO)
		|| (p_atts_tbl[j].number_allowed ==  ONE_TO_SIX)))    {
            xt_log_both2(
			"\t(ERROR) at most one attribute of type %s allowed, received %s",
			xt_itoa(p_atts_tbl[j].att_type),
			xt_itoa(p_atts_tbl[j].r_number_found));
            xt_return_code = XT_ERROR;
			goto end;
        } /* endif */

        if (((p_atts_tbl[j].r_number_found > 2)
		&& !((p_atts_tbl[j].number_allowed ==  ZERO_OR_MORE)
		|| (p_atts_tbl[j].number_allowed == ONE_OR_MORE)
		||  (p_atts_tbl[j].number_allowed ==  ONE_TO_SIX)))
		|| ((p_atts_tbl[j].r_number_found >  6) 
		&& !((p_atts_tbl[j].number_allowed ==  ZERO_OR_MORE)
		||  (p_atts_tbl[j].number_allowed == ONE_OR_MORE)))) {
            xt_log_both2( "\t(ERROR) %s attributes of type %s received - too many",
			xt_itoa(p_atts_tbl[j].r_number_found),
			xt_itoa(p_atts_tbl[j].att_type));
            xt_return_code = XT_ERROR;
			goto end;
        } /* endif */

        if (p_atts_tbl[j].r_number_found > p_atts_tbl[j].e_number_found) {
			warning_issued = OM_TRUE;
            xt_log_both2(
			"\t(WARNING) more attributes of type %s (%s) received than expected",
			xt_itoa(p_atts_tbl[j].att_type),om_type_string(p_atts_tbl[j].att_type));
            xt_log_both1("\t received: %s", xt_itoa(p_atts_tbl[j].r_number_found));
            xt_log_both1("\t expected: %s", xt_itoa(p_atts_tbl[j].e_number_found));
		} /* endif */

        if (p_atts_tbl[j].r_number_found < p_atts_tbl[j].e_number_found) {
			warning_issued = OM_TRUE;
            xt_log_both2("\t(WARNING) fewer attributes of type %s (%s) received than expected",
			xt_itoa(p_atts_tbl[j].att_type),
			om_type_string(p_atts_tbl[j].att_type));
            xt_log_both1("\t received: %s", xt_itoa(p_atts_tbl[j].r_number_found));
            xt_log_both1("\t expected: %s", xt_itoa(p_atts_tbl[j].e_number_found));
		} /* endif */

        total_atts_found = total_atts_found  + p_atts_tbl[j].r_number_found;
    } /* endfor */

    if (r_total  > total_atts_found) {
		warning_issued = OM_TRUE;
        xt_log_both ("\t(WARNING) extra attributes in received object");
	} /* endif */

    if (e_total  > total_atts_found) {
		warning_issued = OM_TRUE;
        xt_log_both ("\t(WARNING) extra attributes in expected object");
	} /* endif */

end:
	if ( xt_verbose || warning_issued || xt_return_code != XT_OK) {
		xt_log_both("");
		xt_log_both1("in %s",function_name);
		xt_log_both("received descriptors:");
		for (i = 0; (!IS_OM_NULL_DESCRIPTOR(p_rv[i])); i++)  {
			xt_log_both2("\t%3s: %s", xt_itoa(p_rv[i].type),
			om_type_string(p_rv[i].type));
		} /* endfor */
			xt_log_both("expected descriptors:");
		for (i = 0; (!IS_OM_NULL_DESCRIPTOR(p_ev[i])); i++)  {
			xt_log_both2("\t%3s: %s", xt_itoa(p_ev[i].type),
			om_type_string(p_ev[i].type));
		} /* endfor */
			xt_log_both("allowed descriptors:");
		for (i = 0; !IS_NULL_ATTRIBUTE(p_atts_tbl[i]); i++)  {
			xt_log_both2("\t%3s: %s", xt_itoa(p_atts_tbl[i].att_type),
			om_type_string(p_atts_tbl[i].att_type));
		} /* endfor */
		xt_log_both("");
		xt_log_both("");
	} /* endif */
    return (xt_return_code);
} /* end xt_check_atts */

static OM_sint
/*******************************************************************/
						xt_cmp_bit_string(
/*******************************************************************/
	char *first,
	char *second,
	OM_uint length1,
	OM_uint length2)
{
	OM_uint *buf1, *buf2;
	OM_uint length1_bytes = ((length1 +7)/8);
	OM_uint length2_bytes = ((length2 +7)/8);
	OM_sint i, result;

	buf1 = calloc(length1_bytes,sizeof(OM_uint));
	buf2 = calloc(length2_bytes,sizeof(OM_uint));

	for ( i = 0 ; i < length1_bytes; i++) {
		buf1[i] = first[i];
	} /* endfor */

	for ( i = 0 ; i < length2_bytes; i++) {
		buf2[i] = second[i];
	} /* endfor */

	if (result = memcmp (buf1,buf2,(size_t)MIN(length1_bytes, length2_bytes))) {
		result = (length1 - length2);
	} /* endif */
	return (result);

} /* end xt_cmp_bit_string */


/****************************************************************************/
/*                                                                          */
/*                      xt_compare_atts()                               */
/*                                                                          */
/****************************************************************************/

OM_sint xt_compare_atts (
	attribute       *atts_tbl,
	OM_descriptor   *received_list,
	OM_descriptor   *expected_list,
	OM_sint no_of_atts)
{
	const char function_name[] = "xt_compare_atts";
    attribute  attrib;
    OM_sint i, j, r, e, m;
    BOOLEAN matched = FALSE;
    OM_sint retc;
    OM_sint xt_return_code = XT_OK;

    for (m = 0; m < no_of_atts; m++) {
        attrib = atts_tbl[m];
        if (IS_NULL_ATTRIBUTE (attrib) && (m != no_of_atts - 1)) {
            xt_log_both ("\t(ERROR) premature NULL attribute in attribute table");
            return(XT_ERROR);
        } /* endif */
        if ((m == no_of_atts - 1) && (!IS_NULL_ATTRIBUTE (attrib)))  {
            xt_log_both ("\t(ERROR) too many attributes in attribute table");
            return(XT_ERROR);
        } /* endif */

        for (i = 0; (i < attrib.e_number_found); i++) {
            e = attrib.e_index[i];
            matched = FALSE;

            for (j = 0; ((j  < attrib.r_number_found) &&   !matched); j++) {
                r = attrib.r_index[j];

                /* compare syntaxes of the received and expected descriptors */

                if ((expected_list[e].syntax & OM_S_SYNTAX) 
				==  (received_list[r].syntax & OM_S_SYNTAX)) {

                    /* compare values of received and expected descriptors, */
                    /* parameters to compare function depend on the syntax  */

                    switch (received_list[r].syntax  &  OM_S_SYNTAX) {
                    case OM_S_BOOLEAN :
                        retc = xt_cmp_boolean (received_list[r].value.boolean,
						expected_list[e].value.boolean);
                        break;
                    case OM_S_OBJECT :
                        retc = xt_cmp_obj (received_list[r].type,
						expected_list[e].type,
						received_list[r].value.object.object,
						expected_list[e].value.object.object);
                        break;
                    case OM_S_INTEGER :
                        retc = xt_cmp_integer (received_list[r].value.integer,
						expected_list[e].value.integer);
                        break;
                    case OM_S_ENUMERATION :
                        retc = xt_cmp_enum (received_list[r].value.enumeration,
						expected_list[e].value.enumeration);
                        break;
                    case OM_S_OBJECT_IDENTIFIER_STRING :
                        if (received_list[r].type ==  OM_CLASS) {
                            retc = xt_cmp_class (received_list[r].value.string,
							expected_list[e].value.string);
                        } else {
                            retc = xt_cmp_string (received_list[r].value.string,
							expected_list[e].value.string);
						} /* endif */
                        break;
					/* CC 29.7.95 */
					case OM_S_BIT_STRING:
						retc = xt_cmp_bit_string(received_list[r].value.string.elements,
						expected_list[e].value.string.elements,
						received_list[r].value.string.length,
						expected_list[e].value.string.length);
                        break;
                    default :
					/* are all the rest strings ??? */
                        retc = xt_cmp_string (received_list[r].value.string,
						expected_list[e].value.string);
                        break;
                    } /* endif */
                    if (retc == XT_OK)
                        matched = TRUE;
                    else if (retc == XT_ERROR)
                        return (XT_ERROR);
                    /* else retc == XT_NOT_MATCHED */
                } /* endif */
            } /* endif */
            if (!matched)
                return (XT_NOT_MATCHED);
        } /* endif */
    } /* endfor */
	if ( xt_verbose) {
		xt_log_both2("in %s matched %s attributes\n",function_name,xt_itoa(i));
	} else {
	} /* endif */
    return(XT_OK);
} /* end xt_compare_atts */



/****************************************************************************/
/*                                                                          */
/*                      xt_free_index()                                 */
/*                                                                          */
/****************************************************************************/

OM_sint xt_free_index (attribute *atts_tbl)
{
    OM_sint i;

    for (i = 0; !IS_NULL_ATTRIBUTE(atts_tbl[i]); i++) {
        if (atts_tbl[i].r_index)
            free (atts_tbl[i].r_index);
        atts_tbl[i].r_index = NULL;
        if (atts_tbl[i].e_index)
            free (atts_tbl[i].e_index);
        atts_tbl[i].e_index = NULL;
    } /* endfor */
} /* end xt_free_index */
