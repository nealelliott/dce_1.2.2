/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsCompare.c,v $
 * Revision 1.1.78.2  1996/02/18  18:24:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:07  marty]
 *
 * Revision 1.1.78.1  1995/12/08  15:36:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:15  root]
 * 
 * Revision 1.1.76.2  1994/03/23  15:48:27  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:35:23  keutel]
 * 
 * Revision 1.1.76.1  1994/02/22  18:09:24  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:19:23  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:56:59  marrek]
 * 
 * Revision 1.1.74.1  1993/10/14  19:19:15  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:31  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:44:38  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  06:59:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:11:23  marrek]
 * 
 * Revision 1.1.4.7  1993/02/02  08:58:57  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.6  1993/02/02  08:13:09  marrek
 * 	Insert <pthread.h> and modify error tables.
 * 	[1993/02/01  13:59:39  marrek]
 * 
 * Revision 1.1.4.5  1992/12/30  23:29:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:00  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/17  09:43:12  marrek
 * 	Add bad pointer check for session
 * 	[1992/12/16  08:59:03  marrek]
 * 
 * Revision 1.1.4.3  1992/12/04  17:35:21  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	[1992/12/04  12:11:21  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:06:46  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:43:19  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:13:37  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:37:27  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsCompare.c,v $ $Revision: 1.1.78.2 $ $Date: 1996/02/18 18:24:20 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : MODUL                                               */
/*                                                                    */
/* NAME         : dsCompare.c                                         */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    : X/Open CAE Spec. API to Directory Services (XDS)    */
/*                X/Open CAE Spec. OSI-Abstract-Data Manip. API (XOM) */
/*                SNI XDS/XOM Design Specification November 1991      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : ds_compare() function                              */
/*                ds_compare_err_tab[] error-table                   */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 90-10-01| Original                       | VR |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <alloc.h>
#include <xdsalloc.h>

#include <d2dir.h>
#include <d21iapl.h>
#include <d21.h>
#include <xdsiapl.h>
#include <xds.h>
#include <xdsp.h>

#include <xdsimport.h>
#include <xdscommon.h>
#include <xdstrace.h>
#include <dsCompare.h>

dsP_error_entry    ds_compare_err_tab[] = {   

{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NOT_SPEC), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ALIAS_ERROR), XDS_NAME_ERROR, DS_E_ALIAS_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_ARGUMENT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_NAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_CONSTRAINT_VIOLATION), XDS_ATTRIBUTE_ERROR, DS_E_CONSTRAINT_VIOLATION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ACCESS_RIGHTS_INSUFFICIENT), XDS_SECURITY_ERROR, DS_E_INSUFFICIENT_ACCESS_RIGHTS},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ATSYNTAX_ERROR), XDS_ATTRIBUTE_ERROR, DS_E_INVALID_ATTRIBUTE_SYNTAX },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_INVALID_REFERENCE), XDS_SERVICE_ERROR, DS_E_INVALID_REF },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ATTRIBUTE_MISSING), XDS_ATTRIBUTE_ERROR, DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NFOUND), XDS_NAME_ERROR, DS_E_NO_SUCH_OBJECT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NMSYNTAX_ERROR), XDS_NAME_ERROR, DS_E_INVALID_ATTRIBUTE_VALUE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ON_ALIAS_NOT_ALLOWED), XDS_NAME_ERROR, DS_E_ALIAS_DEREFERENCING_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_DSA_BIND), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SELECTION), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SC), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_REF_ERROR), XDS_REFERRAL },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_ENTRYNAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_SERV_CTRLS_INVALID), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NO_BIND), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_ILL_VERS,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS },
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D21_ALLOC_ERROR), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_TOO_BUSY), XDS_SERVICE_ERROR, DS_E_BUSY },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_LOOP_DETECTED), XDS_SERVICE_ERROR, DS_E_LOOP_DETECTED },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_TIME_LIMIT_EXCEEDED), XDS_SERVICE_ERROR, DS_E_TIME_LIMIT_EXCEEDED },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_NO_RESPONSE), XDS_SERVICE_ERROR, DS_E_UNAVAILABLE },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_UNWILLING), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_TEMP_ERR,D2_INT_ERR,D21_RESULTAPDU_INVALID), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS},
{XHASH(D2_PERM_ERR,D2_SYS_ERR,D2_NOT_SPEC), XDS_SYSTEM_ERROR },
{XHASH(D2_PERM_ERR,D2_REM_ERR,D2_OUT_OF_SCOPE), XDS_SERVICE_ERROR, DS_E_OUT_OF_SCOPE }

};

#define COMPARE_ERR_TAB_SIZE \
                (sizeof(ds_compare_err_tab) / sizeof(ds_compare_err_tab[0]))





DS_status       
ds_compare(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_object           ava,
    OM_private_object   *result_return,
    OM_sint             *invoke_id_return
)
{
    D2_pbhead             pbhead = { IAPL_VERSION };
    D21_p6                p6 = D21_P6_NULL;
    DS_status             status = DS_SUCCESS;
    OM_private_object     pri_context;
    OM_private_object     pri_name;
    OM_private_object     pri_ava;
    xds_compare_result    *x_compare_result;
    OM_sint                   bind_id;
    OM_sint                   op_num = 0;
    OM_sint                   index;
    OM_sint          tmp_pri_obj = FALSE;
    OM_sint          tmp_pri_ava = FALSE;
    OM_sint          miscellaneous = FALSE;
    OM_workspace workspace;

    TRACE_ENTER ("ds_compare()")

    INIT_ONCE

	ABANDON_PRELUDE_QUERY

    if ((session) && (OMP_INTERNAL(session))) {
        workspace = OMP_WORKSPACE(session);
        if (dsP_validate_wkspace(workspace, &index) != OK) {
            status = DS_NO_WORKSPACE;
			goto end;
        } /* endif */
    } else {
        TRACE_ERROR("session is invalid")
		status = DS_NO_WORKSPACE;
		goto end;
    } /* endif */

    TRACE_IARGS
    TRACE_OBJECT(0, session)
    TRACE_OBJECT(0, context)
    TRACE_OBJECT(0, name)
    TRACE_OBJECT(0, ava)

    do {
        p6.d21_6cres = NULL;

        if (dsP_validate_session(session, &bind_id, &op_num) != OK) {
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_SESSION,
                DS_E_BAD_SESSION)
        } else {
            p6.d21_6bind_id = bind_id;
		} /* endif */


        if (context == DS_DEFAULT_CONTEXT) {
            TRACE_NOTE("default-context is requested:")
            TRACE_NOTE("ChainingProhib DontUseCopy AutoContinuation DontStore UseDSA")
#ifdef THREADSAFE
            if (dsP_validate_wkspace(workspace, &index) != OK) {
                status = DS_INVALID_WORKSPACE;
                continue;
            } else {
#endif
                pri_context = dsP_wkspace_table[index].default_context;
#ifdef THREADSAFE
			} /* endif */
#endif
        } else {
            if (dsP_validate_context(context) != OK) {
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR,
                    status,
                    DS_E_BAD_CONTEXT,
                    DS_E_BAD_CONTEXT)
            } else {
                pri_context = context;
			} /* endif */
		} /* endif */

        switch(dsP_validate_name(name, &pri_name, &status, workspace)) {
            case DONE - NOTOK: tmp_pri_obj++;
            case NOTOK:        continue;
            case DONE:         tmp_pri_obj++;
            case OK:           break;
        } /* endswitch */

        if (dsP_verify_instance (ava, dsP_DS_C_AVA) != FALSE) {

            if (! (OMP_PRIVATE(ava))) {
                CHECK_OMP_CALL(
                    om_create,
                    (dsP_DS_C_AVA, 0, workspace, &pri_ava),
                    status,
                    DS_E_MISCELLANEOUS)
                tmp_pri_ava++;

                CHECK_OMP_CALL(
                    om_put,
                    (pri_ava, OM_REPLACE_ALL, ava, OM_NO_MORE_TYPES, 0, 0),
                    status,
                    DS_E_BAD_ARGUMENT)

                TRACE_NOTE("ava converted to a private object")
            } else {
                TRACE_NOTE("ava is a private object")
                pri_ava = ava;
                if (! OMP_DATA(ava)) {
                    TRACE_ERROR("ava has not been initialized")
                    CHECK_OMP_CREATE_ERROR(
                        dsP_DS_C_LIBRARY_ERROR,
                        status,
                        DS_E_BAD_ARGUMENT,
                        DS_E_BAD_ARGUMENT)
                } /* endif */
            } /* endif */
        } else {
            TRACE_ERROR("ava is not an instance of DS_C_AVA")
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_ARGUMENT,
                DS_E_BAD_ARGUMENT)
        } /* endif */
                
        p6.d21_6pbhead = &pbhead;
        p6.d21_6c_a    = ((xds_context *)OMP_DATA(pri_context));
        p6.d21_6entry  = ((xds_dist_name)OMP_DATA(pri_name));
        p6.d21_6comp   = ((xds_ava *)OMP_DATA(pri_ava));

        p6.d21_6dist_name  = NULL;
        p6.d21_6match      = FALSE;
        p6.d21_6from_entry = FALSE;
        p6.d21_6err        = NULL;

        if ((p6.d21_6cres = (D2_c_res *)calloc (1,sizeof(D2_c_res))) == 0) {
            miscellaneous++;
            continue;
        } /* endif */

        if (((p6.d21_6c_a)->d2_serv_cntrls & D2_DUACACHE) && 
            (! ((p6.d21_6c_a)->d2_serv_cntrls & D2_USEDSA))) {
            p6.d21_6bind_id = ((xds_session *)OMP_DATA(session))->dir_id;
            TRACE_NOTE("using DUA-cache only")
        } else {
            if (p6.d21_6bind_id == -1)
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR,
                    status,
                    DS_E_BAD_CONTEXT,
                    DS_E_BAD_CONTEXT)
		} /* endif */

        if (d21_6_compare(&p6) == D2_NOERROR) {
            TRACE_NOTE2("%s %s\n","successful completion of comparison of:",
			(OMP_DATA(pri_name) ? OMP_DATA(pri_name) : "[Root]"))

            CHECK_OMP_XALLOC(
                x_compare_result,
                (xds_compare_result *),
                XDS_COMPARE_RESULT,
                status)

            x_compare_result->dist_name  = p6.d21_6dist_name;
            x_compare_result->match      = p6.d21_6match;
            x_compare_result->from_entry = p6.d21_6from_entry;
            x_compare_result->com_res    = p6.d21_6cres;

            CHECK_OMP_CALL(
                omP_create_handle,
                (dsP_DS_C_COMPARE_RESULT, workspace, x_compare_result, result_return),
                status,
                DS_E_MISCELLANEOUS)
            *invoke_id_return = (OM_sint) 0;
        } else {
            TRACE_WARN2("%s %s\n","unsuccessful completion of comparison of:",
			OMP_DATA(pri_name) ? OMP_DATA(pri_name) : "[Root]")

            if (dsP_iapl2xds_error(
                ds_compare_err_tab, COMPARE_ERR_TAB_SIZE,
                p6.d21_6pbhead, p6.d21_6err,&status, workspace) != OK){

                TRACE_WARN2("cannot map IAPL error (%d) %s\n",
			  	p6.d21_6pbhead->d2_errvalue, "onto an XDS error");
                if (! status) {
                    miscellaneous++;
                    continue;
                } /* endif */
            } /* endif */
        } /* endif */

    } while (FALSE);

    do {
        if (miscellaneous)
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR, 
                status, 
                DS_E_MISCELLANEOUS, 
                DS_E_MISCELLANEOUS)
    } while (FALSE);

    do {
        if (tmp_pri_obj)
            CHECK_OMP_CALL(
                om_delete,
                (pri_name),
                status,
                DS_E_MISCELLANEOUS)
        if (tmp_pri_ava)
            CHECK_OMP_CALL(
                om_delete,
                (pri_ava),
                status,
                DS_E_MISCELLANEOUS)
    } while (FALSE);

#ifdef THREADSAFE
    if (op_num)
        dsP_dec_op_num(session);
#endif

    if (status == DS_SUCCESS) {
        TRACE_OARGS
        TRACE_OBJECT(1, *result_return)
	} else {
        TRACE_OERR
        TRACE_OBJECT(1, status)
    } /* endif */

	ABANDON_INTERLUDE_QUERY
end:
	if (status != DS_SUCCESS && p6.d21_6cres != NULL) {
		free(p6.d21_6cres);
	} /* endif */

    TRACE_EXIT

	ABANDON_POSTLUDE_QUERY
    return(status);

} /* end  ds_compare */
