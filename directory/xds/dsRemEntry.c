/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsRemEntry.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:38  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:36:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:28  root]
 * 
 * Revision 1.1.8.3  1994/07/06  15:09:12  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:08  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:48:50  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:33  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:22:20  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:21:53  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:04:40  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:07:06  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:17:13  marrek]
 * 
 * Revision 1.1.4.7  1993/02/02  08:59:33  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.6  1993/02/02  08:14:00  marrek
 * 	Insert <pthread.h> and modify error tables.
 * 	[1993/02/01  14:03:34  marrek]
 * 
 * Revision 1.1.4.5  1992/12/30  23:29:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:37  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/17  09:43:45  marrek
 * 	Add bad pointer check for session
 * 	[1992/12/16  09:03:00  marrek]
 * 
 * Revision 1.1.4.3  1992/12/04  17:36:04  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	[1992/12/04  12:12:57  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:08:27  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:45:04  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:14:46  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:38:22  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsRemEntry.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:05 $";
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
/* NAME         : dsRemEntry.c                                        */
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
/* DESCRIPTION  : ds_remove_entry() function                         */
/*                ds_remove_entry_err_tab[] error-table              */
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
#include <dsRemEntry.h>

dsP_error_entry    ds_remove_entry_err_tab[] = {

{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NOT_SPEC), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_AFFECTS_MULTIPLE_DSAS), XDS_UPDATE_ERROR, DS_E_AFFECTS_MULTIPLE_DSAS },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ALIAS_ERROR), XDS_NAME_ERROR, DS_E_ALIAS_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_ARGUMENT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_NAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ACCESS_RIGHTS_INSUFFICIENT), XDS_SECURITY_ERROR, DS_E_INSUFFICIENT_ACCESS_RIGHTS},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_INVALID_REFERENCE), XDS_SERVICE_ERROR, DS_E_INVALID_REF },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NFOUND), XDS_NAME_ERROR, DS_E_NO_SUCH_OBJECT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ORPHAN), XDS_UPDATE_ERROR, DS_E_NOT_ALLOWED_ON_NON_LEAF },
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
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D21_ALLOC_ERROR), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_TOO_BUSY), XDS_SERVICE_ERROR, DS_E_BUSY },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_CHAINING_REQUIRED), XDS_SERVICE_ERROR, DS_E_CHAINING_REQUIRED },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_NO_RESPONSE), XDS_SERVICE_ERROR, DS_E_UNAVAILABLE },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_UNWILLING), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_TEMP_ERR,D2_INT_ERR,D21_RESULTAPDU_INVALID), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS},
{XHASH(D2_PERM_ERR,D2_SYS_ERR,D2_NOT_SPEC), XDS_SYSTEM_ERROR },
{XHASH(D2_PERM_ERR,D2_REM_ERR,D2_OUT_OF_SCOPE), XDS_SERVICE_ERROR, DS_E_OUT_OF_SCOPE },

};

#define REMOVE_ENTRY_ERR_TAB_SIZE \
        (sizeof(ds_remove_entry_err_tab) / sizeof(ds_remove_entry_err_tab[0]))



DS_status   
ds_remove_entry(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_sint             *invoke_id_return
)
{
    D2_pbhead             pbhead = { IAPL_VERSION };
    D21_p8                p8;
    DS_status             status = DS_SUCCESS;
    OM_private_object     pri_context;
    OM_private_object     pri_name;
    OM_sint               bind_id;
    OM_sint               op_num = 0;
    OM_sint               index;
    OM_sint               tmp_pri_obj = FALSE;
    OM_workspace          workspace;

    TRACE_ENTER ("ds_remove_entry()")

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
    TRACE_OBJECT(0, context)
    TRACE_OBJECT(0, name)

    do {
        if (dsP_validate_session(session, &bind_id, &op_num) != OK) {
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_SESSION,
                DS_E_BAD_SESSION)
        } else {
            p8.d21_8bind_id = bind_id;
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
            case DONE - NOTOK: tmp_pri_obj++;
            case NOTOK:        continue;
            case DONE:         tmp_pri_obj++;
            case OK:           break;
        } /* endswitch */
        
        p8.d21_8pbhead  = &pbhead;
        p8.d21_8c_a     = ((xds_context *)OMP_DATA(pri_context));
        p8.d21_8entry   = ((xds_dist_name)OMP_DATA(pri_name));
        p8.d21_8err     = NULL;

        if (((p8.d21_8c_a)->d2_serv_cntrls & D2_DUACACHE) && 
            (! ((p8.d21_8c_a)->d2_serv_cntrls & D2_USEDSA))) {
            p8.d21_8bind_id = ((xds_session *)OMP_DATA(session))->dir_id;
            TRACE_NOTE("using DUA-cache only")
        } else
            if (p8.d21_8bind_id == -1)
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR,
                    status,
                    DS_E_BAD_CONTEXT,
                    DS_E_BAD_CONTEXT)


        if (d21_8_remove_object(&p8) == D2_NOERROR) {
            TRACE_NOTE2("%s %s\n","successful removal of:",
			OMP_DATA(pri_name) ? OMP_DATA(pri_name) : "[Root]")
            *invoke_id_return   = (OM_sint) 0;
        } else {
            TRACE_WARN2("%s %s\n","unsuccessful removal of:",
			OMP_DATA(pri_name) ? OMP_DATA(pri_name) : "[Root]")

            if (dsP_iapl2xds_error(ds_remove_entry_err_tab,
			REMOVE_ENTRY_ERR_TAB_SIZE,p8.d21_8pbhead,p8.d21_8err,
			&status,workspace) != OK) {

                TRACE_WARN2("cannot map IAPL error (%d) %s\n",
				p8.d21_8pbhead->d2_errvalue, "onto an XDS error");

                if (! status)
                    CHECK_OMP_CREATE_ERROR(
                        dsP_DS_C_LIBRARY_ERROR, 
                        status, 
                        DS_E_MISCELLANEOUS, 
                        DS_E_MISCELLANEOUS)
            } /* endif */
#if 0	/* temp removed - should be freed by caller */
			OM_FREE(p8.d21_8err);
#endif /* 0 */
        } /* endif */
    } while (FALSE);

    while (tmp_pri_obj--)
        if (OMP_PUBLIC(name))
            CHECK_OMP_CALL(
                om_delete,
                (pri_name),
                status,
                DS_E_MISCELLANEOUS
            )

#ifdef THREADSAFE
    if (op_num)
        dsP_dec_op_num(session);
#endif

    if (status != DS_SUCCESS) {
        TRACE_OERR
        TRACE_OBJECT(1, status)
    } /* endif */
end:
    TRACE_EXIT
    return(status);


} /* end ds_remove_entry */
