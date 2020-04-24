/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsRead.c,v $
 * Revision 1.1.76.2  1996/02/18  18:25:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:36  marty]
 *
 * Revision 1.1.76.1  1995/12/08  15:36:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:24  root]
 * 
 * Revision 1.1.74.2  1994/03/23  15:48:49  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:27  keutel]
 * 
 * Revision 1.1.74.1  1994/02/22  19:22:16  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:21:44  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:04:21  marrek]
 * 
 * Revision 1.1.72.1  1993/10/14  19:19:37  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:17:46  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:44:50  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  07:06:42  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:16:51  marrek]
 * 
 * Revision 1.1.4.6  1993/02/02  08:59:26  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.5  1993/02/02  08:13:50  marrek
 * 	Insert <pthread.h> and modify error tables.
 * 	[1993/02/01  14:02:56  marrek]
 * 
 * Revision 1.1.4.4  1992/12/30  23:29:22  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:29  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  09:43:39  marrek
 * 	Add bad pointer check for session
 * 	consistent style for TRACE NOTES
 * 	[1992/12/16  09:02:26  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:08:05  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:44:36  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:14:30  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:38:09  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsRead.c,v $ $Revision: 1.1.76.2 $ $Date: 1996/02/18 18:25:02 $";
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
/* NAME         : dsRead.c                                            */
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
/* DESCRIPTION  : ds_read() function                                 */
/*                ds_read_err_tab[] error-table                      */
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
#include <dsRead.h>

dsP_error_entry    ds_read_err_tab[] = {

{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NOT_SPEC), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ADM_LIM_EXCEEDED), XDS_SERVICE_ERROR, DS_E_ADMIN_LIMIT_EXCEEDED },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ALIAS_ERROR), XDS_NAME_ERROR, DS_E_ALIAS_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_ARGUMENT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_DSA_UNKNOWN), XDS_SECURITY_ERROR, DS_E_NO_INFO},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_NAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_DIT_INCONSISTENT), XDS_SERVICE_ERROR, DS_E_DIT_ERROR},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ACCESS_RIGHTS_INSUFFICIENT), XDS_SECURITY_ERROR, DS_E_INSUFFICIENT_ACCESS_RIGHTS},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_INVALID_REFERENCE), XDS_SERVICE_ERROR, DS_E_INVALID_REF },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ATTRIBUTE_MISSING), XDS_ATTRIBUTE_ERROR, DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NFOUND), XDS_NAME_ERROR, DS_E_NO_SUCH_OBJECT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NMSYNTAX_ERROR), XDS_NAME_ERROR, DS_E_INVALID_ATTRIBUTE_VALUE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ON_ALIAS_NOT_ALLOWED), XDS_NAME_ERROR, DS_E_ALIAS_DEREFERENCING_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_DSA_BIND), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SELECTION), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_INV_R_NO_AT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_R_NO_AT_TOO_BIG), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_INV_R_VAL_AT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SC), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_REF_ERROR), XDS_REFERRAL },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_ENTRYNAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_SERV_CTRLS_INVALID), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NO_BIND), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS },
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D21_ALLOC_ERROR), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_TOO_BUSY), XDS_SERVICE_ERROR, DS_E_BUSY },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_LOOP_DETECTED), XDS_SERVICE_ERROR, DS_E_LOOP_DETECTED },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_TIME_LIMIT_EXCEEDED), XDS_SERVICE_ERROR, DS_E_TIME_LIMIT_EXCEEDED },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_NO_RESPONSE), XDS_SERVICE_ERROR, DS_E_UNAVAILABLE },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_UNWILLING), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_TEMP_ERR,D2_INT_ERR,D21_RESULTAPDU_INVALID), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS},
{XHASH(D2_PERM_ERR,D2_SYS_ERR,D2_NOT_SPEC), XDS_SYSTEM_ERROR },
{XHASH(D2_PERM_ERR,D2_REM_ERR,D2_OUT_OF_SCOPE), XDS_SERVICE_ERROR, DS_E_OUT_OF_SCOPE },
{XHASH(D2_WARNING,D2_ILL_VERS,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION }

};

#define READ_ERR_TAB_SIZE \
        (sizeof(ds_read_err_tab) / sizeof(ds_read_err_tab[0]))



DS_status   
ds_read(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_object           selection,
    OM_private_object   *result_return,
    OM_sint             *invoke_id_return
)
{
    D2_pbhead             pbhead = { IAPL_VERSION };
    D21_p3                p3 = D21_P3_NULL;
    DS_status             status = DS_SUCCESS;
    OM_private_object     pri_context;
    OM_private_object     pri_name;
    OM_private_object     pri_selection;
    xds_read_result       *x_read_result;
    OM_sint               bind_id;
    OM_sint               op_num = 0;
    OM_sint               index;
    OM_boolean            tmp_pri_obj = FALSE;
    OM_boolean            tmp_pri_sel = FALSE;
    OM_boolean            miscellaneous = FALSE;
    OM_workspace          workspace;

    TRACE_ENTER ("ds_read()")

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
    TRACE_OBJECT(0, selection)

    do {
        p3.d21_3cres = NULL;

        if (dsP_validate_session(session, &bind_id, &op_num) != OK) {
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_SESSION,
                DS_E_BAD_SESSION
            )
        } else {
            p3.d21_3bind_id = bind_id;
		} /* endif */


        if (context == DS_DEFAULT_CONTEXT) {
            TRACE_NOTE("default-context is requested:")
            TRACE_NOTE("ChainingProhib DontUseCopy AutoContinuation DontStore UseDSA")
#ifdef THREADSAFE
            if (dsP_validate_wkspace(workspace, &index) != OK) {
                status = DS_INVALID_WORKSPACE;
                continue;
            } else {
#endif /* THREADSAFE */
                pri_context = dsP_wkspace_table[index].default_context;
#ifdef THREADSAFE
			} /* endif */
#endif /* THREADSAFE */
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
            case DONE - NOTOK: tmp_pri_obj = TRUE;
            case NOTOK:        continue;
            case DONE:         tmp_pri_obj = TRUE;
            case OK:           break;
        } /* endswitch */

		switch ((int)selection) {
		case (int)DS_SELECT_ALL_TYPES_AND_VALUES:
            TRACE_NOTE("all attribute types and values selected")
            pri_selection = dsP_wkspace_table[index].select_all_tv;
			break;
		case (int)DS_SELECT_ALL_TYPES:
			TRACE_NOTE("all attribute types selected")
			pri_selection = dsP_wkspace_table[index].select_all_t;
			break;
		case (int)DS_SELECT_NO_ATTRIBUTES:
			TRACE_NOTE("no attributes selected")
			pri_selection = dsP_wkspace_table[index].select_none;
			break;
		default:
			switch(dsP_validate_selection(selection, &pri_selection,
									  &status, workspace)) {
			case DONE - NOTOK:
				tmp_pri_sel = TRUE;
				continue;
			case NOTOK:
				continue;
			case DONE:
				tmp_pri_sel = TRUE;
				break;
			case OK:
				break;
			} /* endswitch */
			break;
		} /* endswitch */

        p3.d21_3pbhead = &pbhead;
        p3.d21_3c_a    = ((xds_context *)OMP_DATA(pri_context));
        p3.d21_3entry  = ((xds_dist_name)OMP_DATA(pri_name));
        p3.d21_3r_info = ((xds_entry_info_sel *)OMP_DATA(pri_selection));

        p3.d21_3res    = NULL;
        p3.d21_3err    = NULL;

        if ((p3.d21_3cres = (D2_c_res *)calloc (1,sizeof(D2_c_res))) == 0){
            miscellaneous++;
            continue;
        } /* endif */


        if (((p3.d21_3c_a)->d2_serv_cntrls & D2_DUACACHE) && 
            (! ((p3.d21_3c_a)->d2_serv_cntrls & D2_USEDSA))) {
            p3.d21_3bind_id = ((xds_session *)OMP_DATA(session))->dir_id;
            TRACE_NOTE("using DUA-cache only")
        } else if (p3.d21_3bind_id == -1) {
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR,
                    status,
                    DS_E_BAD_CONTEXT,
                    DS_E_BAD_CONTEXT)
		} /* endif */

        if (d21_3_read(&p3) == D2_NOERROR) {
            TRACE_NOTE2("%s %s\n", "successful read of:",
			OMP_DATA(pri_name)? OMP_DATA(pri_name) : "[Root]")

			CHECK_OMP_XALLOC(
				x_read_result,
				(xds_read_result *),
				XDS_READ_RESULT,
				status)

			x_read_result->read_info = p3.d21_3res;
			x_read_result->com_res   = p3.d21_3cres;

			CHECK_OMP_CALL(
				omP_create_handle,
				(dsP_DS_C_READ_RESULT, workspace, x_read_result, result_return),
				status,
				DS_E_MISCELLANEOUS)
			*invoke_id_return   = (OM_sint) 0;
        } else {
            TRACE_WARN2("%s %s\n", "unsuccessful read of:",
			OMP_DATA(pri_name) ? OMP_DATA(pri_name) : "[Root]")

            if (dsP_iapl2xds_error (ds_read_err_tab, READ_ERR_TAB_SIZE,
                         p3.d21_3pbhead, p3.d21_3err,&status,workspace) != OK) {

                TRACE_WARN2("cannot map IAPL error (%d) %s\n",
				p3.d21_3pbhead->d2_errvalue, "onto an XDS error");
                if (! status) {
                    miscellaneous++;
                    continue;
				} /* endif */
			} /* endif */
		} /* endif */

    } while (FALSE);


	while (miscellaneous--) {
		CHECK_OMP_CREATE_ERROR(
			dsP_DS_C_LIBRARY_ERROR, 
			status, 
			DS_E_MISCELLANEOUS, 
			DS_E_MISCELLANEOUS)
	} /* endwhile */

    do {
        if (tmp_pri_obj)
            CHECK_OMP_CALL(
                om_delete,
                (pri_name),
                status,
                DS_E_MISCELLANEOUS
            )
        if (tmp_pri_sel)
            CHECK_OMP_CALL(
                om_delete,
                (pri_selection),
                status,
                DS_E_MISCELLANEOUS
            )
    } while (FALSE);

    if (op_num) {
        dsP_dec_op_num(session);
	} /* endif */

    if (status == DS_SUCCESS) {
        TRACE_OARGS
        TRACE_OBJECT(1, *result_return)
    } else {
        TRACE_OERR
        TRACE_OBJECT(1, status)
	} /* endif */

	ABANDON_INTERLUDE_QUERY
end:
	if (status != DS_SUCCESS && p3.d21_3cres != NULL) {
		free(p3.d21_3cres);
	} /* endif */

    TRACE_EXIT

	ABANDON_POSTLUDE_QUERY
	
    return(status);

} /* end ds_read */
