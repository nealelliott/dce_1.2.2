/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsUnbind.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:37:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:36  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:48:55  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:53  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:22:30  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:22:07  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:05:31  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:08:20  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:18:18  marrek]
 * 
 * Revision 1.1.4.4  1992/12/30  23:29:43  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:56  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  09:44:18  marrek
 * 	Add bad pointer check for session
 * 	[1992/12/16  09:07:03  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:09:26  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:46:09  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:15:20  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:42:34  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsUnbind.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:13 $";
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
/* NAME         : dsUnbind.c                                          */
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
/* DESCRIPTION  : ds_unbind() function                               */
/*                ds_unbind_err_tab[] error_table                    */
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
#include <dsUnbind.h>

dsP_error_entry    ds_unbind_err_tab[] = {

{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NOT_SPEC), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NO_BIND), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_ILL_VERS,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_NO_RESPONSE), XDS_SERVICE_ERROR, DS_E_UNAVAILABLE },
{XHASH(D2_PERM_ERR,D2_SYS_ERR,D2_NOT_SPEC), XDS_SYSTEM_ERROR }

};

#define UNBIND_ERR_TAB_SIZE \
        (sizeof(ds_unbind_err_tab) / sizeof(ds_unbind_err_tab[0]))



DS_status   
ds_unbind(
    OM_private_object   session
)
{
    D2_pbhead             pbhead = { IAPL_VERSION };
    D21_p2                p2;
    DS_status             status = DS_SUCCESS;
    OM_sint               index;
    OM_sint               bind_id;
    OM_sint               op_num = 0;
    OM_sint               i;
    OM_workspace          workspace;

    TRACE_ENTER ("ds_unbind()")

    INIT_ONCE

    if ((session) && (OMP_INTERNAL(session))) {
        workspace = OMP_WORKSPACE(session);
        if (dsP_validate_wkspace(workspace, &index) != OK) {
            status = DS_NO_WORKSPACE;
			goto end;
        } /* endif */
    } /* endif */
    else {
        TRACE_ERROR("session is invalid")
		status = DS_NO_WORKSPACE;
		goto end;
    } /* endif */

    TRACE_IARGS
    TRACE_OBJECT(0, session)

    do {
        if (dsP_validate_session(session, &bind_id, &op_num) != OK)
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_SESSION,
                DS_E_BAD_SESSION
            )
        else {
#ifdef THREADSAFE
            if ((op_num-1) >0) {
                TRACE_WARN2("%d %s\n", op_num-1, "outstanding operation(s) - cannot unbind")
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_SERVICE_ERROR,
                    status,
                    DS_E_MISCELLANEOUS,
                    DS_E_BUSY
                )
            }
#endif
            p2.d21_2bind_id = bind_id;
        }

        p2.d21_2pbhead  = &pbhead;


        if (p2.d21_2bind_id == -1)
            i = D2_NOERROR;
        else
            i = d21_2_unbind(&p2);


        if (i == D2_NOERROR) {

            LOCK(dsP_bindtab_mutex)

            for (i=0; i < DSP_MAX_SESSIONS; i++)
                if (dsP_bind_table[i].bound_session == session) {
                    dsP_bind_table[i].bound_session = (OM_private_object) 0; 
                    dsP_bind_table[i].bind_id       = 0; 
                    dsP_bind_table[i].op_num        = 0; 
                    break;
                }

            UNLOCK(dsP_bindtab_mutex)

            if (i == DSP_MAX_SESSIONS) {
                TRACE_ERROR("session not found in bind-table")
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR,
                    status,
                    DS_E_BAD_SESSION,
                    DS_E_BAD_SESSION
                )
            }

            if (p2.d21_2bind_id == -1)
                TRACE_NOTE("successful unbind from DUA-cache")
            else
                TRACE_NOTE2("%s   %s\n","successful unbind from DSA:",
				((xds_session *)OMP_DATA(session))->dsa_name ?
				(char *)((xds_session *)OMP_DATA(session))->dsa_name : " ");

        }
        else {
            TRACE_WARN2("%s   %s\n","unsuccessful unbind from DSA:",
			((xds_session *)OMP_DATA(session))->dsa_name ?
            (char *)((xds_session *)OMP_DATA(session))->dsa_name : " ");

            if (dsP_iapl2xds_error(ds_unbind_err_tab, UNBIND_ERR_TAB_SIZE, 
                              p2.d21_2pbhead, NULL, &status, workspace) != OK) {

                TRACE_WARN2("cannot map IAPL error (%d) %s\n",
                              p2.d21_2pbhead->d2_errvalue, "onto an XDS error");
                if (! status)
                    CHECK_OMP_CREATE_ERROR(
                        dsP_DS_C_LIBRARY_ERROR, 
                        status, 
                        DS_E_MISCELLANEOUS, 
                        DS_E_MISCELLANEOUS
                    )
            }
        }
    } while (FALSE);

    if (status != DS_SUCCESS) {
        if (op_num)
            dsP_dec_op_num(session);
        TRACE_OERR
        TRACE_OBJECT(1, status)
    }

end:
    TRACE_EXIT
    return(status);

} /* end ds_unbind */
