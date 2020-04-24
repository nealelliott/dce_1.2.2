/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsList.c,v $
 * Revision 1.1.81.2  1996/02/18  18:24:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:11  marty]
 *
 * Revision 1.1.81.1  1995/12/08  15:36:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:20  root]
 * 
 * Revision 1.1.79.2  1994/03/23  15:48:30  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:35:32  keutel]
 * 
 * Revision 1.1.79.1  1994/02/22  18:09:31  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:19:32  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:57:40  marrek]
 * 
 * Revision 1.1.77.1  1993/10/14  19:19:21  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:41  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:44:47  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  07:00:21  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:12:13  marrek]
 * 
 * Revision 1.1.4.7  1993/02/02  08:59:04  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.6  1993/02/02  08:13:17  marrek
 * 	Insert <pthread.h> and modify error tables.
 * 	[1993/02/01  14:00:30  marrek]
 * 
 * Revision 1.1.4.5  1992/12/30  23:29:12  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:14  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/17  09:43:19  marrek
 * 	Add bad pointer check for session
 * 	[1992/12/16  08:59:37  marrek]
 * 
 * Revision 1.1.4.3  1992/12/04  17:35:41  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	[1992/12/04  12:12:01  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:07:24  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:43:59  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:53:43  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:23:28  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:14:03  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:37:46  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsList.c,v $ $Revision: 1.1.81.2 $ $Date: 1996/02/18 18:24:26 $";
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
/* NAME         : dsList.c                                            */
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
/* DESCRIPTION  : ds_list() function                                 */
/*                ds_list_err_tab[] error-table                      */
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
#	include <pthread.h>
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
#include <dsList.h>

dsP_error_entry    ds_list_err_tab[] = {  

{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NOT_SPEC), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ALIAS_ERROR), XDS_NAME_ERROR, DS_E_ALIAS_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_ARGUMENT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_NAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ACCESS_RIGHTS_INSUFFICIENT), XDS_SECURITY_ERROR, DS_E_INSUFFICIENT_ACCESS_RIGHTS},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_INVALID_REFERENCE), XDS_SERVICE_ERROR, DS_E_INVALID_REF },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NFOUND), XDS_NAME_ERROR, DS_E_NO_SUCH_OBJECT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NMSYNTAX_ERROR), XDS_NAME_ERROR, DS_E_INVALID_ATTRIBUTE_VALUE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ON_ALIAS_NOT_ALLOWED), XDS_NAME_ERROR, DS_E_ALIAS_DEREFERENCING_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_DSA_BIND), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SELECTION), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SC), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_REF_ERROR), XDS_REFERRAL },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_ENTRYNAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_SERV_CTRLS_INVALID), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NO_BIND), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_ILL_VERS,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS },
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D21_REFERRALS_TOO_MANY), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
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

#define LIST_ERR_TAB_SIZE \
        (sizeof(ds_list_err_tab) / sizeof(ds_list_err_tab[0]))



DS_status   
ds_list(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_private_object   *result_return,
    OM_sint             *invoke_id_return
)
{
    D2_pbhead             pbhead = { IAPL_VERSION };
    D2_ref_inf            *ref;
    D2_acc_point          *acc;
    D21_p4                p4 = D21_P4_NULL;
    DS_status             status = DS_SUCCESS;
    OM_private_object     pri_context;
    OM_private_object     pri_name;
    xds_list_info         *x_list_info;
    xds_part_oq           *x_part_oq = NULL;
    OM_sint               bind_id;
    OM_sint               op_num = 0;
    OM_sint               index;
    OM_sint               tmp_pri_obj = 0;
    OM_boolean            miscellaneous = FALSE;
    OM_workspace          workspace;

    TRACE_ENTER ("ds_list()")

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

    do {
        p4.d21_4cres = NULL;

        if (dsP_validate_session(session, &bind_id, &op_num) != OK) {
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_SESSION,
                DS_E_BAD_SESSION)
        } else {
            p4.d21_4bind_id = bind_id;
		} /* endif */

        if (context == DS_DEFAULT_CONTEXT) {
            TRACE_NOTE("default-context is requested:")
            TRACE_NOTE("ChainingProhib DontUseCopy AutoContinuation DontStore UseDSA")
#ifdef THREADSAFE
            if (dsP_validate_wkspace(workspace, &index) != OK) {
                status = DS_INVALID_WORKSPACE;
                continue;
            } else 
#endif
                pri_context = dsP_wkspace_table[index].default_context;
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
            case DONE - NOTOK:
				tmp_pri_obj = TRUE;
				continue;
            case NOTOK:
				continue;
            case DONE:
				tmp_pri_obj = TRUE;
				break;
            case OK: 
				break;
        } /* endswitch */

        p4.d21_4pbhead  = &pbhead;
        p4.d21_4c_a     = ((xds_context *)OMP_DATA(pri_context));

        if (!(OMP_DATA(pri_name)) || 
            (*((xds_dist_name)OMP_DATA(pri_name)) == D2_EOS)) {
            p4.d21_4parent  = NULL;
        } else {
            p4.d21_4parent  = ((xds_dist_name)OMP_DATA(pri_name));
		} /* endif */

        p4.d21_4file         = NULL;
        p4.d21_4res          = NULL;
        p4.d21_4rf           = NULL;
        p4.d21_4dist_name    = NULL;
        p4.d21_4lim_prob     = 0;
        p4.d21_4uav_crit_ext = 0;
        p4.d21_4err          = NULL;

        if ((p4.d21_4cres = (D2_c_res *)calloc (1,sizeof(D2_c_res))) == 0) {
            miscellaneous++;
            continue;
		} /* endif */

        if (((p4.d21_4c_a)->d2_serv_cntrls & D2_DUACACHE) && 
                              (! ((p4.d21_4c_a)->d2_serv_cntrls & D2_USEDSA))) {
            p4.d21_4bind_id = ((xds_session *)OMP_DATA(session))->dir_id;
            TRACE_NOTE("using DUA-cache only")
        } else {
            if (p4.d21_4bind_id == -1) {
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR,
                    status,
                    DS_E_BAD_CONTEXT,
                    DS_E_BAD_CONTEXT)
			} /* endif */
		} /* endif */


        if (d21_4_list(&p4) == D2_NOERROR) {
            TRACE_NOTE2("%s %s\n","successful list of:",
			OMP_DATA(pri_name)? (char *)OMP_DATA(pri_name):"[Root]")

            if ((p4.d21_4lim_prob != D2_NO_LIMIT_EX) ||
                (p4.d21_4rf != NULL) ||
                (p4.d21_4uav_crit_ext)) {

                CHECK_OMP_XALLOC(
                    x_part_oq,
                    (xds_part_oq *),
                    XDS_PARTIAL_OUTCOME_QUAL,
                    status)

                if (p4.d21_4lim_prob == D2_TIME_LIMIT_EX) {
                    TRACE_WARN("time-limit exceeded")
                } else {
                    if (p4.d21_4lim_prob == D2_SIZE_LIMIT_EX) {
                        TRACE_WARN("size-limit exceeded")
                    } else {
                        if (p4.d21_4lim_prob == D2_ADM_LIMIT_EX) {
                            TRACE_WARN("administrative-limit exceeded")
						} /* endif */
					} /* endif */
				} /* endif */

                ref = p4.d21_4rf;
                while (ref) {
                    acc = ref->d2_acc_point;
                    while (acc) {
                        if (acc->d2_dsa_ap) {
                            TRACE_WARN2("%s %s\n","referral to:", acc->d2_dsa_ap);
						} /* endif */
                        acc = acc->d2_next_ap;

                    } /* endwhile */
                    ref = ref->d2_next_ref;
                } /* endwhile */

                if (p4.d21_4lim_prob == D2_NO_LIMIT_EX) {
                    x_part_oq->limit_prob = DS_NO_LIMIT_EXCEEDED;
                } else {
                    x_part_oq->limit_prob = p4.d21_4lim_prob;
				} /* endif */

                x_part_oq->cont_ref = p4.d21_4rf;
                x_part_oq->unav_crit_ext = p4.d21_4uav_crit_ext;
			} /* endif */

            CHECK_OMP_XALLOC(
                x_list_info,
                (xds_list_info *),
                XDS_LIST_INFO,
                status)

            x_list_info->t_object     = p4.d21_4dist_name;
            x_list_info->l_part_oq    = x_part_oq;
            x_list_info->subordinates = p4.d21_4res;
            x_list_info->com_res      = p4.d21_4cres;

            CHECK_OMP_CALL(
                omP_create_handle,
                (dsP_DS_C_LIST_RESULT, workspace, x_list_info, result_return),
                status,
                DS_E_MISCELLANEOUS)
            *invoke_id_return   = (OM_sint) 0;
        } else {
			TRACE_WARN2("%s %s\n","unsuccessful list of:",
			OMP_DATA(pri_name) ? OMP_DATA(pri_name) : "[Root]")
			if (dsP_iapl2xds_error( ds_list_err_tab, LIST_ERR_TAB_SIZE,
			p4.d21_4pbhead, p4.d21_4err, &status, workspace) != OK) {

				TRACE_WARN2("cannot map IAPL error (%d) %s\n",
				p4.d21_4pbhead->d2_errvalue, "onto an XDS error");
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

    while (tmp_pri_obj--) {
        if (OMP_PUBLIC(name)) {
            CHECK_OMP_CALL(
                om_delete,
                (pri_name),
                status,
                DS_E_MISCELLANEOUS)
		} /* endif */
	} /* endwhile */

#ifdef THREADSAFE
    if (op_num) {
        dsP_dec_op_num(session);
	} /* endif */
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
	if (status != DS_SUCCESS && p4.d21_4cres != NULL) {
		free(p4.d21_4cres);
	} /* endif */

    TRACE_EXIT

	ABANDON_POSTLUDE_QUERY

    return(status);

} /* end ds_list */
