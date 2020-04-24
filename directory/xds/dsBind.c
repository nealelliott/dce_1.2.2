/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsBind.c,v $
 * Revision 1.1.10.2  1996/02/18  18:24:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:06  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:35:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:14  root]
 * 
 * Revision 1.1.8.4  1994/07/06  15:08:54  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:23:28  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  16:03:44  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:13:40  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:48:25  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:34:48  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:09:22  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:19:18  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:56:49  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  06:59:07  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:11:11  marrek]
 * 
 * Revision 1.1.4.6  1993/02/02  08:58:49  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.5  1993/02/02  08:13:01  marrek
 * 	insert <pthread.h>
 * 	introduce def_sess.
 * 	[1993/02/01  13:42:31  marrek]
 * 
 * Revision 1.1.4.4  1992/12/30  23:28:59  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:18:55  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  17:35:09  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	[1992/12/04  12:11:01  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:06:33  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:43:06  marrek]
 * 
 * Revision 1.1.2.4  1992/07/01  16:53:40  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:22:49  melman]
 * 
 * Revision 1.1.2.3  1992/06/30  21:00:17  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:41:31  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:13:27  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:37:21  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsBind.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:24:18 $";
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
/* NAME         : dsBind.c                                            */
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
/* DESCRIPTION  : ds_bind() function                                 */
/*                dsP_bind_err_tab[] error-table                      */
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
#include <xdsgds.h>

#include <xdsimport.h>
#include <xdscommon.h>
#include <xdstrace.h>
#include <dsBind.h>


dsP_error_entry    dsP_bind_err_tab[] = {

{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NOT_SPEC), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_ARGUMENT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_DSA_UNKNOWN), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_CREDENTIALS_INVALID), XDS_SECURITY_ERROR, DS_E_INVALID_CREDENTIALS },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SELECTION), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_C_MAXDIR), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_CREDENT_ERR), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_DIR_UNKNOWN), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_ILL_VERS,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS },
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D21_BINDS_TOO_MANY), XDS_LIBRARY_ERROR, DS_E_TOO_MANY_SESSIONS},
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D21_ALLOC_ERROR), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_TOO_BUSY), XDS_SERVICE_ERROR, DS_E_BUSY },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_NO_RESPONSE), XDS_SERVICE_ERROR, DS_E_UNAVAILABLE }

};

#define BIND_ERR_TAB_SIZE \
        (sizeof(dsP_bind_err_tab) / sizeof(dsP_bind_err_tab[0]))



DS_status   
ds_bind(
    OM_object           session,
    OM_workspace        workspace,
    OM_private_object   *bound_session_return)
{
    D2_pbhead           pbhead = { IAPL_VERSION };
    D2_credent          u_credent = { 0, 0 };
    D2_a_value          a_value;
    D2_str              auth_info = { 0, NULL };
    D21_p1              p1 = D21_P1_NULL;
    DS_status           status = DS_SUCCESS;
    OM_private_object   pri_session;
    xds_session         *x_session;
    OM_sint             index;
    OM_sint        		tmp_pri_obj = FALSE;
   	OM_sint        		i;
    OM_sint        		cds = 0;
    OM_sint        		cache = 0;
    OM_sint        		def_sess = 0;
    char       			*cp;

    TRACE_ENTER ("ds_bind()")

    INIT_ONCE

	ABANDON_PRELUDE_NONQUERY

    if (dsP_validate_wkspace(workspace, &index) != OK) {
        status = DS_INVALID_WORKSPACE;
    } /* endif */

    TRACE_IARGS
    TRACE_OBJECT(0, session)

    do {
        if (session == DS_DEFAULT_SESSION) {
            TRACE_NOTE("default-session is requested")
            def_sess = 1;

            if (((OMP_private_object *)
                OMP_INTERNAL(dsP_wkspace_table[index].default_session))
                                       ->object_type.class == XDS_GDS_SESSION) {
                CHECK_OMP_CALL(
                    om_create,
                    (dsP_DSX_C_GDS_SESSION, TRUE, workspace, &pri_session),
                    status,
                    DS_E_MISCELLANEOUS)
            } else {
                CHECK_OMP_CALL(
                    om_create,
                    (dsP_DS_C_SESSION, TRUE, workspace, &pri_session),
                    status,
                    DS_E_MISCELLANEOUS)
            } /* endif */
            tmp_pri_obj++;
        } else {
            if (dsP_verify_instance (session, dsP_DS_C_SESSION) != FALSE) {

                if (! (OMP_PRIVATE(session))) {

#ifdef THREADSAFE
                    if (dsP_validate_wkspace(workspace, &index) != OK) {
                        status = DS_INVALID_WORKSPACE;
                        continue;
                    } /* endif */
#endif
                    if (dsP_verify_instance(
					dsP_wkspace_table[index].default_session,
					dsP_DSX_C_GDS_SESSION)) {

                        CHECK_OMP_CALL(
                            om_create,
                            (dsP_DSX_C_GDS_SESSION, 0, workspace, &pri_session),
                            status,
                            DS_E_MISCELLANEOUS)
                    } else {
                        CHECK_OMP_CALL(
                            om_create,
                            (dsP_DS_C_SESSION, 0, workspace, &pri_session),
                            status,
                            DS_E_MISCELLANEOUS)

                    } /* endif */
                    tmp_pri_obj++;

                    CHECK_OMP_CALL(
                        om_put,
                        (pri_session, OM_REPLACE_ALL, session, 0, 0, 0),
                        status,
                        DS_E_BAD_SESSION)
                    TRACE_NOTE("session converted to a private object")
                } else {
                    TRACE_NOTE("session is a private object")
                    pri_session = session;
                    if (! OMP_DATA(session)) {
                        TRACE_ERROR("session has not been initialized")
                        CHECK_OMP_CREATE_ERROR(
                            dsP_DS_C_LIBRARY_ERROR, 
                            status, 
                            DS_E_BAD_SESSION,
                            DS_E_BAD_SESSION)
                    } /* endif */
                } /* endif */
            } else {
                TRACE_ERROR("session is not an instance of DS_C_SESSION")

                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR, 
                    status, 
                    DS_E_BAD_SESSION,
                    DS_E_BAD_SESSION)
            } /* endif */
        } /* endif */

        x_session = ((xds_session *)OMP_DATA(pri_session));

        p1.d21_1pbhead  = &pbhead;
        p1.d21_1dir_id  = x_session->dir_id;

        if ((x_session->requestor) || (x_session->password.d2_p_len)) {
            if (x_session->requestor) {
                u_credent.d2_c_name = x_session->requestor;
			} /* endif */
            if (x_session->password.d2_p_len) {
                u_credent.d2_c_passwd = &x_session->password;
			} /* endif */
            p1.d21_1u_credent = &u_credent;
        } else {
            p1.d21_1u_credent = NULL;
        } /* endif */

        p1.d21_1name_dsa    = x_session->dsa_name;

        if (x_session->dsa_address) { 
            a_value.d2_a_v_len = dsP_get_psap_length(x_session->dsa_address);
            a_value.d2_a_rep = D2_PSAP_STX;
            a_value.d2_at_value.d2_psap_a_v = x_session->dsa_address;
            p1.d21_1addr_dsa = &a_value;
        } else {
            p1.d21_1addr_dsa = NULL; 
		} /* endif */

        p1.d21_1auth_mech = x_session->auth_mech; 
        switch(x_session->auth_mech) {
            case DSX_NONE_AT_ALL:
                TRACE_NOTE("no authentication");
                break;
            case DSX_SIMPLE:
                TRACE_NOTE("simple authentication")
                break;
            case DSX_DCE_AUTH:
                TRACE_NOTE("DCE authentication")
                break;
            case DSX_DEFAULT:
                p1.d21_1auth_mech = dsP_def_auth_mech;
              TRACE_NOTE2("%s (%d)","default authentication",dsP_def_auth_mech);
                break;
            case DSX_STRONG:
                TRACE_NOTE("strong authentication")
                break;
            case DSX_SIMPLE_PROT1:
                TRACE_NOTE("simple protected 1 authentication")
                break;
            case DSX_SIMPLE_PROT2:
                TRACE_NOTE("simple protected 2 authentication")
                break;
            default:
                TRACE_ERROR("unrecognised authentication mechanism")
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR, 
                    status, 
                    DS_E_NOT_SUPPORTED,
                    DS_E_NOT_SUPPORTED)
                break;
        }

        if (x_session->auth_info.d2_size) {
            auth_info.d2_size  = x_session->auth_info.d2_size;
            auth_info.d2_value = x_session->auth_info.d2_value;
            p1.d21_1auth_info  = &auth_info;
        } else {
            p1.d21_1auth_info  = NULL;
        } /* endif */

        p1.d21_1ret_dsa = NULL; 

        if ((p1.d21_1name_dsa == NULL) 
		&& (p1.d21_1addr_dsa == NULL) 
		&& (! def_sess)) {
            cache = 1;
        } else {
            i = d21_1_bind(&p1);
		} /* endif */

        if ((i == D2_NOERROR) || (cache)) {

            if (p1.d21_1ret_dsa == NULL)
                cds++;

            if ((p1.d21_1ret_dsa) 
			&& ((x_session->dsa_name == NULL) 
			|| (strcmp((char *)x_session->dsa_name,(char *)p1.d21_1ret_dsa)))) {
               TRACE_NOTE2("%s   %s\n","attempt to bind to DSA:",
			   p1.d21_1name_dsa ? (char *)p1.d21_1name_dsa : "")
               if ((cp = OMP_XALLOC_STR(workspace, (char *)p1.d21_1ret_dsa,
			   strlen((char *)p1.d21_1ret_dsa))) == NULL) {
                    TRACE_ERROR ("OMP_XALLOC_STR( dsa_name ) failed - OM_MEMORY_INSUFFICIENT")
                    CHECK_OMP_CREATE_ERROR(
                        dsP_DS_C_LIBRARY_ERROR, 
                        status, 
                        DS_E_MISCELLANEOUS, 
                        DS_E_MISCELLANEOUS)
                } /* endif */
                if (x_session->dsa_name != NULL) {
                    OMP_XFREE_STR(workspace, x_session->dsa_name,
					strlen((char *)x_session->dsa_name));
				} /* endif */
                x_session->dsa_name = (D2_name_string) cp;
            } /* endif */

            if (p1.d21_1ret_dsa) {
                free(p1.d21_1ret_dsa);
			} /* endif */


            LOCK(dsP_bindtab_mutex)

            for (i=0; i < DSP_MAX_SESSIONS; i++) {
                if (!dsP_bind_table[i].bound_session) {
                    dsP_bind_table[i].bound_session 
					= *bound_session_return = pri_session;
                    dsP_bind_table[i].bind_id = (cache ? -1 : p1.d21_1bind_id);
                    dsP_bind_table[i].op_num  = 0;
                    tmp_pri_obj--;
                    break;
                } /* endif */
			} /* endfor */

            UNLOCK(dsP_bindtab_mutex)


            if (i == DSP_MAX_SESSIONS) {
                TRACE_ERROR("too many sessions - bind-table full")
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR, 
                    status, 
                    DS_E_TOO_MANY_SESSIONS, 
                    DS_E_TOO_MANY_SESSIONS)
            } /* endif */

            if (cache) {
                TRACE_NOTE("successful bind to DUA-cache")
            } else {
				  TRACE_NOTE2("%s   %s\n","successful bind to DSA:",
				  ((xds_session *)OMP_DATA(pri_session))->dsa_name ?
				  (char *)((xds_session *)OMP_DATA(pri_session))->dsa_name :
				  cds?"using CDS":" ")
			} /* endif */
        } else {
            TRACE_WARN2("%s   %s\n","unsuccessful bind to DSA:",
			 ((xds_session *)OMP_DATA(pri_session))->dsa_name ?
			  (char *)((xds_session *)OMP_DATA(pri_session))->dsa_name : " ")

            if (dsP_iapl2xds_error(
                dsP_bind_err_tab, BIND_ERR_TAB_SIZE, 
                p1.d21_1pbhead, NULL, &status, workspace) != OK) {

                TRACE_WARN2("cannot map IAPL error (%d) %s\n",
				p1.d21_1pbhead->d2_errvalue, "onto an XDS error");

                if (! status)
                    CHECK_OMP_CREATE_ERROR(
                        dsP_DS_C_LIBRARY_ERROR, 
                        status, 
                        DS_E_MISCELLANEOUS, 
                        DS_E_MISCELLANEOUS)
            } /* endif */
        } /* endif */
    } while (FALSE);

    while (tmp_pri_obj-- > 0) {
        CHECK_OMP_CALL(
            om_delete,
            (pri_session),
            status,
            DS_E_MISCELLANEOUS)
	} /* endwhile */

    if (status == DS_SUCCESS) {
        TRACE_OARGS
        TRACE_OBJECT(1, *bound_session_return)
    } else {
        TRACE_OERR
        TRACE_OBJECT(1, status)
    } /* endif */

    TRACE_EXIT

	ABANDON_POSTLUDE_NONQUERY

    return(status);

} /* end  ds_bind */
