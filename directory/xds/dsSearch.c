/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsSearch.c,v $
 * Revision 1.1.77.2  1996/02/18  18:25:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:39  marty]
 *
 * Revision 1.1.77.1  1995/12/08  15:36:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:29  root]
 * 
 * Revision 1.1.75.2  1994/03/23  15:48:52  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:38  keutel]
 * 
 * Revision 1.1.75.1  1994/02/22  19:22:22  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:21:59  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:04:53  marrek]
 * 
 * Revision 1.1.73.1  1993/10/14  19:19:39  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:17:53  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:44:56  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  07:07:26  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:17:30  marrek]
 * 
 * Revision 1.1.4.8  1993/02/02  08:59:43  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.7  1993/02/02  08:14:13  marrek
 * 	Insert <pthread.h> and modify error tables.
 * 	[1993/02/01  14:04:16  marrek]
 * 
 * Revision 1.1.4.6  1992/12/30  23:29:33  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:42  bbelch]
 * 
 * Revision 1.1.4.5  1992/12/17  09:43:55  marrek
 * 	Add bad pointer check for session
 * 	Consistent style for TRACE NOTES
 * 	[1992/12/16  09:04:00  marrek]
 * 
 * Revision 1.1.4.4  1992/12/04  17:36:13  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	[1992/12/04  12:13:22  marrek]
 * 
 * Revision 1.1.4.3  1992/11/30  08:08:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:45:20  marrek]
 * 
 * Revision 1.1.4.2  1992/10/06  18:40:00  sommerfeld
 * 	Flush #ident.
 * 	[1992/09/30  21:24:44  sommerfeld]
 * 
 * Revision 1.1.2.4  1992/07/17  19:59:22  weisman
 * 	Checked in at OSF for SNI
 * 	[1992/07/17  19:29:41  weisman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsSearch.c,v $ $Revision: 1.1.77.2 $ $Date: 1996/02/18 18:25:07 $";
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
/* NAME         : dsSearch.c                                         */
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
/* DESCRIPTION  : ds_search() function                               */
/*                ds_search_err_tab[] error-table                    */
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
#include <dsSearch.h>

dsP_error_entry    ds_search_err_tab[] = {    

{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NOT_SPEC), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ALIAS_ERROR), XDS_NAME_ERROR, DS_E_ALIAS_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_ARGUMENT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_NAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_CONSTRAINT_VIOLATION), XDS_ATTRIBUTE_ERROR, DS_E_CONSTRAINT_VIOLATION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_INAPPR_MATCH), XDS_ATTRIBUTE_ERROR, DS_E_INAPPROP_MATCHING },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ACCESS_RIGHTS_INSUFFICIENT), XDS_SECURITY_ERROR, DS_E_INSUFFICIENT_ACCESS_RIGHTS},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ATSYNTAX_ERROR), XDS_ATTRIBUTE_ERROR, DS_E_INVALID_ATTRIBUTE_SYNTAX },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_INVALID_REFERENCE), XDS_SERVICE_ERROR, DS_E_INVALID_REF },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NFOUND), XDS_NAME_ERROR, DS_E_NO_SUCH_OBJECT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_UNDEFINED_ATTR_TYPE), XDS_ATTRIBUTE_ERROR, DS_E_UNDEFINED_ATTRIBUTE_TYPE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NMSYNTAX_ERROR), XDS_NAME_ERROR, DS_E_INVALID_ATTRIBUTE_VALUE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ON_ALIAS_NOT_ALLOWED), XDS_NAME_ERROR, DS_E_ALIAS_DEREFERENCING_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_DSA_BIND), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SELECTION), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_INV_R_NO_AT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_R_NO_AT_TOO_BIG), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_INV_R_VAL_AT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_INV_SEARCH_SEL), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
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

#define SEARCH_ERR_TAB_SIZE \
                (sizeof(ds_search_err_tab) / sizeof(ds_search_err_tab[0]))





DS_status       
ds_search(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_sint             subset,
    OM_object           filter,
    OM_boolean          search_aliases,
    OM_object           selection,
    OM_private_object   *result_return,
    OM_sint             *invoke_id_return
)
{
    D2_pbhead             pbhead = { IAPL_VERSION };
    D2_ref_inf            *ref;
    D2_acc_point          *acc;
    D21_p5                p5 = D21_P5_NULL;
    DS_status             status = DS_SUCCESS;
    OM_private_object     pri_context;
    OM_private_object     pri_name;
    OM_private_object     pri_filter;
    OM_private_object     pri_selection;
    xds_search_result     *x_search_result;
    xds_part_oq           *x_part_oq = NULL;
    D2_Fi_filter          *iapl_filter;
    OM_sint               bind_id;
    OM_sint               op_num = 0;
    OM_sint               index;
    OM_sint               tmp_pri_obj = FALSE;
    OM_sint               tmp_pri_fil = FALSE;
    OM_sint               tmp_iapl_fil = FALSE;
    OM_sint               tmp_pri_sel = FALSE;
    OM_sint               miscellaneous = FALSE;
    OM_sint               bad_argument = FALSE;
    OM_workspace          workspace;

    TRACE_ENTER ("ds_search()")

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
    TRACE_INTEGER(subset)
    TRACE_OBJECT(0, filter)
    TRACE_BOOLEAN(search_aliases)
    TRACE_OBJECT(0, selection)

    do {
        p5.d21_5cres = NULL;

        if (dsP_validate_session(session, &bind_id, &op_num) != OK) {
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_SESSION,
                DS_E_BAD_SESSION)
        } else {
            p5.d21_5bind_id = bind_id;
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
				tmp_pri_obj++;
				continue;
            case NOTOK:
				continue;
            case DONE:
				tmp_pri_obj++;
				break;
            case OK:
				break;
        } /* endswitch */

        if ((subset != DS_BASE_OBJECT) && (subset != DS_ONE_LEVEL) && 
                                                 (subset != DS_WHOLE_SUBTREE)) {
            TRACE_ERROR("subset is invalid")
            bad_argument++;
            continue;
        } /* endif */

        if (filter == DS_NO_FILTER) {
            TRACE_NOTE("no-filter is requested")
#ifdef THREADSAFE
            if (dsP_validate_wkspace(workspace, &index) != OK) {
                status = DS_INVALID_WORKSPACE;
                continue;
            } else
#endif
                pri_filter = dsP_wkspace_table[index].no_filter;
        } else {
            if (dsP_verify_instance (filter, dsP_DS_C_FILTER) != FALSE) {

                if (! (OMP_PRIVATE(filter))) {

                    CHECK_OMP_CALL(
                        om_create,
                        (dsP_DS_C_FILTER, FALSE, workspace, &pri_filter),
                        status,
                        DS_E_MISCELLANEOUS)
                    tmp_pri_fil++;

                    CHECK_OMP_CALL(
                        om_put,
                        (pri_filter,OM_REPLACE_ALL,filter,OM_NO_MORE_TYPES,0,0),
                        status,
                        DS_E_BAD_ARGUMENT)

                    TRACE_NOTE("filter converted to a private object")
                } else {
                    TRACE_NOTE("filter is a private object")
                    pri_filter = filter;
                    if (! OMP_DATA(filter)) {
                        TRACE_ERROR("filter has not been initialized")
                        bad_argument++;
                        continue;
                    } /* endif */
                } /* endif */
            } else {
                TRACE_ERROR("filter is not an instance of DS_C_FILTER")
                bad_argument++;
                continue;
            } /* endif */
        } /* endif */

        if (dsP_xds2iapl_filter ( ((xds_filter *)OMP_DATA(pri_filter)),
                                                          &iapl_filter) != OK) {
            TRACE_WARN("cannot map XDS filter onto IAPL filter")
            bad_argument++;
            continue;
        } /* endif */
        tmp_iapl_fil++;


        if (selection == DS_SELECT_ALL_TYPES_AND_VALUES) {
            TRACE_NOTE("all attribute types and values selected")
#ifdef THREADSAFE
            if (dsP_validate_wkspace(workspace, &index) != OK) {
                status = DS_INVALID_WORKSPACE;
                continue;
            } else
#endif
                pri_selection = dsP_wkspace_table[index].select_all_tv;
        } else if (selection == DS_SELECT_ALL_TYPES) {
                 TRACE_NOTE("all attribute types selected")
#ifdef THREADSAFE
                if (dsP_validate_wkspace(workspace, &index) != OK) {
                    status = DS_INVALID_WORKSPACE;
                    continue;
                } else
#endif
                     pri_selection = dsP_wkspace_table[index].select_all_t;
             } else if (selection == DS_SELECT_NO_ATTRIBUTES) {
                      TRACE_NOTE("no attributes selected")
#ifdef THREADSAFE
                    if (dsP_validate_wkspace(workspace, &index) != OK) {
                        status = DS_INVALID_WORKSPACE;
                        continue;
                    } else
#endif
                          pri_selection = dsP_wkspace_table[index].select_none;
                  } else {
						switch(dsP_validate_selection(selection, &pri_selection,
                                                          &status, workspace)) {
						case DONE - NOTOK:
							tmp_pri_sel++;
							continue;
						case NOTOK:
							continue;
						case DONE:
							tmp_pri_sel++;
							break;
						case OK: 
							break;
                      } /* endswitch */
                  } /* endif */

        p5.d21_5pbhead      = &pbhead;
        p5.d21_5c_a         = ((xds_context *)OMP_DATA(pri_context));

        if (!(OMP_DATA(pri_name)) ||
            (*((xds_dist_name)OMP_DATA(pri_name)) == D2_EOS))
            p5.d21_5base_obj = NULL;
        else
            p5.d21_5base_obj = ((xds_dist_name)OMP_DATA(pri_name));

        switch (subset) {
            case DS_WHOLE_SUBTREE:
                p5.d21_5subset = D2_WHOLE_SUBTREE;
                break;
            case DS_ONE_LEVEL:
                p5.d21_5subset = D2_ONE_LEVEL;
                break;
            case DS_BASE_OBJECT:
                p5.d21_5subset = D2_BASE_OBJECT;
                break;
        } /* endswitch */
        p5.d21_5f_pattern    = iapl_filter;
        p5.d21_5r_info       = ((xds_entry_info_sel *)OMP_DATA(pri_selection));
        p5.d21_5file         = NULL;
        p5.d21_5res          = NULL;
        p5.d21_5rf           = NULL;
        p5.d21_5dist_name    = NULL;
        p5.d21_5lim_prob     = 0;
        p5.d21_5uav_crit_ext = 0;
        p5.d21_5srch_al      = search_aliases;
        p5.d21_5err          = NULL;

        if ((p5.d21_5cres = (D2_c_res *)calloc (1,sizeof(D2_c_res))) == 0) {
            miscellaneous++;
            continue;
        } /* endif */

        if (((p5.d21_5c_a)->d2_serv_cntrls & D2_DUACACHE) && 
                              (! ((p5.d21_5c_a)->d2_serv_cntrls & D2_USEDSA))) {
            p5.d21_5bind_id =((xds_session *)OMP_DATA(session))->dir_id;
            TRACE_NOTE("using DUA-cache only")
        } else {
            if (p5.d21_5bind_id == -1)
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR,
                    status,
                    DS_E_BAD_CONTEXT,
                    DS_E_BAD_CONTEXT)
		} /* endif */


        if (d21_5_search(&p5) == D2_NOERROR) {
            TRACE_NOTE2("%s %s\n","successful search of:",
			OMP_DATA(pri_name)? OMP_DATA(pri_name) : "[Root]")

            if ((p5.d21_5lim_prob != D2_NO_LIMIT_EX) ||
                (p5.d21_5rf != NULL) ||
                (p5.d21_5uav_crit_ext)) {

                CHECK_OMP_XALLOC(
                    x_part_oq,
                    (xds_part_oq *),
                    XDS_PARTIAL_OUTCOME_QUAL,
                    status)

                if (p5.d21_5lim_prob == D2_TIME_LIMIT_EX) {
                    TRACE_WARN("time-limit exceeded")
                } else {
                    if (p5.d21_5lim_prob == D2_SIZE_LIMIT_EX) {
                        TRACE_WARN("size-limit exceeded")
                    } else {
                        if (p5.d21_5lim_prob == D2_ADM_LIMIT_EX) {
                            TRACE_WARN("administrative-limit exceeded")
						} /* endif */
					} /* endif */
				} /* endif */

                ref = p5.d21_5rf;
                while (ref) {
                    acc = ref->d2_acc_point;
                    while (acc) {
                        if (acc->d2_dsa_ap)
                            TRACE_WARN2("%s %s\n","referral to:", acc->d2_dsa_ap);
                        acc = acc->d2_next_ap;
                    } /* endwhile */
                    ref = ref->d2_next_ref;
                } /* endwhile */

                if (p5.d21_5lim_prob == D2_NO_LIMIT_EX) {
                    x_part_oq->limit_prob = DS_NO_LIMIT_EXCEEDED;
                } else {
                    x_part_oq->limit_prob = p5.d21_5lim_prob;
				} /* endif */

                x_part_oq->cont_ref = p5.d21_5rf;
                x_part_oq->unav_crit_ext = p5.d21_5uav_crit_ext;
            } /* endif */

            CHECK_OMP_XALLOC(
                x_search_result,
                (xds_search_result *),
                XDS_SEARCH_RESULT,
                status)

            x_search_result->search_entries = p5.d21_5res;
            x_search_result->b_object       = p5.d21_5dist_name;
            x_search_result->s_part_oq      = x_part_oq;
            x_search_result->com_res        = p5.d21_5cres;

            CHECK_OMP_CALL(
                omP_create_handle,
                (dsP_DS_C_SEARCH_RESULT, workspace, x_search_result, result_return),
                status,
                DS_E_MISCELLANEOUS)
            *invoke_id_return       = (OM_sint) 0;
        } else {
            TRACE_WARN2("%s %s\n","unsuccessful search of:",
			OMP_DATA(pri_name) ? OMP_DATA(pri_name) : "[Root]")

            if (dsP_iapl2xds_error(ds_search_err_tab, SEARCH_ERR_TAB_SIZE,
                        p5.d21_5pbhead, p5.d21_5err,&status, workspace) != OK) {

                TRACE_WARN2("cannot map IAPL error (%d) %s\n",
				p5.d21_5pbhead->d2_errvalue, "onto an XDS error");
                if (! status) {
                    miscellaneous++;
                    continue;
                } /* endif */
            } /* endif */
        } /* endif */

    } while (FALSE);

    if (bad_argument) {
        while (bad_argument--) {
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_ARGUMENT,
                DS_E_BAD_ARGUMENT)
		} /* endwhile */
    } else {
        while (miscellaneous--) {
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR, 
                status, 
                DS_E_MISCELLANEOUS, 
                DS_E_MISCELLANEOUS)
		} /* endwhile */
	} /* endif */


    do {
        if (tmp_pri_obj) {
            CHECK_OMP_CALL(
                om_delete,
                (pri_name),
                status,
                DS_E_MISCELLANEOUS)
		} /* endif */
        if (tmp_pri_fil) {
            CHECK_OMP_CALL(
                om_delete,
                (pri_filter),
                status,
                DS_E_MISCELLANEOUS)
		} /* endif */
        if ((tmp_iapl_fil) && (iapl_filter)) {
            dsP_free_iapl_filter(iapl_filter);
		} /* endif */

        if (tmp_pri_sel) {
            CHECK_OMP_CALL(
                om_delete,
                (pri_selection),
                status,
                DS_E_MISCELLANEOUS)
		} /* endif */
    } while (FALSE);

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
	if (status != DS_SUCCESS && p5.d21_5cres != NULL) {
		free(p5.d21_5cres);
	} /* endif */

    TRACE_EXIT

	ABANDON_POSTLUDE_QUERY

    return(status);

} /* end ds_search */
