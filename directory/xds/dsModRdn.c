/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsModRdn.c,v $
 * Revision 1.1.10.2  1996/02/18  18:24:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:13  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:36:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:22  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:48:32  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:35:39  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:09:34  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:19:43  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:58:00  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:00:50  marrek
 * 	added 2 IAPL errors.
 * 	[1993/08/03  09:50:59  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/27  14:12:38  marrek]
 * 
 * Revision 1.1.4.7  1993/02/02  08:59:19  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.6  1993/02/02  08:13:42  marrek
 * 	Insert <pthread.h> and modify error tables.
 * 	[1993/02/01  14:02:21  marrek]
 * 
 * Revision 1.1.4.5  1992/12/30  23:29:19  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:24  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/17  09:43:32  marrek
 * 	Add bad pointer check for session
 * 	[1992/12/16  09:00:46  marrek]
 * 
 * Revision 1.1.4.3  1992/12/04  17:35:56  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	[1992/12/04  12:12:39  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:07:52  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:44:24  marrek]
 * 
 * Revision 1.1.2.4  1992/07/07  21:23:23  melman
 * 	Remove dce/ from includes of xom.h xds.h and xomi.h
 * 	[1992/07/07  21:22:43  melman]
 * 
 * Revision 1.1.2.3  1992/06/30  21:00:27  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:41:44  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:14:21  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:38:00  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsModRdn.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:24:29 $";
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
/* NAME         : dsModRdn.c                                          */
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
/* DESCRIPTION  : ds_modify_rdn() function                           */
/*                ds_modify_rdn_err_tab[] error-table                */
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
#include <dsModRdn.h>

dsP_error_entry    ds_modify_rdn_err_tab[] = {

{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NOT_SPEC), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_AFFECTS_MULTIPLE_DSAS), XDS_UPDATE_ERROR, DS_E_AFFECTS_MULTIPLE_DSAS },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ALIAS_ERROR), XDS_NAME_ERROR, DS_E_ALIAS_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_ARGUMENT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_NAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_CONSTRAINT_VIOLATION), XDS_ATTRIBUTE_ERROR, DS_E_CONSTRAINT_VIOLATION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ALREADY_EXISTS), XDS_UPDATE_ERROR, DS_E_ENTRY_EXISTS },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ACCESS_RIGHTS_INSUFFICIENT), XDS_SECURITY_ERROR, DS_E_INSUFFICIENT_ACCESS_RIGHTS},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ATSYNTAX_ERROR), XDS_ATTRIBUTE_ERROR, DS_E_INVALID_ATTRIBUTE_SYNTAX },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_INVALID_REFERENCE), XDS_SERVICE_ERROR, DS_E_INVALID_REF },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NAMING_VIOLATION), XDS_UPDATE_ERROR, DS_E_NAMING_VIOLATION},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NFOUND), XDS_NAME_ERROR, DS_E_NO_SUCH_OBJECT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ORPHAN), XDS_UPDATE_ERROR, DS_E_NOT_ALLOWED_ON_NON_LEAF },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_OBJCLS_MOD_PROHIBITED), XDS_UPDATE_ERROR, DS_E_OBJECT_CLASS_MOD_PROHIB },
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
{XHASH(D2_PERM_ERR,D2_REM_ERR,D2_OUT_OF_SCOPE), XDS_SERVICE_ERROR, DS_E_OUT_OF_SCOPE }

};

#define MODIFY_RDN_ERR_TAB_SIZE \
                (sizeof(ds_modify_rdn_err_tab) / sizeof(ds_modify_rdn_err_tab[0]))





DS_status       
ds_modify_rdn(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_object           new_RDN,
    OM_boolean          delete_old_RDN,
    OM_sint             *invoke_id_return
)
{
    D2_pbhead             pbhead = { IAPL_VERSION };
    D21_pB                pB;
    DS_status             status = DS_SUCCESS;
    OM_private_object     pri_context;
    OM_private_object     pri_name;
    OM_private_object     pri_new_RDN;
    OM_sint               bind_id;
    OM_sint               op_num = 0;
    OM_sint               index;
    OM_sint               tmp_pri_obj = FALSE;
    OM_workspace          workspace;

    TRACE_ENTER ("ds_modify_rdn()")

    INIT_ONCE

	ABANDON_PRELUDE_NONQUERY

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
    TRACE_OBJECT(0, new_RDN)
    TRACE_BOOLEAN(delete_old_RDN)

    do {
        if (dsP_validate_session(session, &bind_id, &op_num) != OK)
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_SESSION,
                DS_E_BAD_SESSION
            )
        else
            pB.d21_Bbind_id = bind_id;


        if (context == DS_DEFAULT_CONTEXT) {
            TRACE_NOTE("default-context is requested:")
            TRACE_NOTE("ChainingProhib DontUseCopy AutoContinuation DontStore UseDSA")
#ifdef THREADSAFE
            if (dsP_validate_wkspace(workspace, &index) != OK) {
                status = DS_INVALID_WORKSPACE;
                continue;
            }
            else
#endif
                pri_context = dsP_wkspace_table[index].default_context;
        }
        else 
            if (dsP_validate_context(context) != OK)
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR,
                    status,
                    DS_E_BAD_CONTEXT,
                    DS_E_BAD_CONTEXT
                )
            else
                pri_context = context;

        switch(dsP_validate_name(name, &pri_name, &status, workspace)) {
            case DONE - NOTOK: tmp_pri_obj++;
            case NOTOK:        continue;
            case DONE:         tmp_pri_obj++;
            case OK:           break;
        }


        if (dsP_verify_instance (new_RDN, dsP_DS_C_RELATIVE_NAME) != FALSE) {

            if (! (OMP_PRIVATE(new_RDN))) {

                CHECK_OMP_CALL(
                    om_create,
                    (dsP_DS_C_DS_RDN, 0, workspace, &pri_new_RDN),
                    status,
                    DS_E_MISCELLANEOUS
                )
                tmp_pri_obj++;

                CHECK_OMP_CALL(
                    om_put,
                    (pri_new_RDN, OM_REPLACE_ALL, new_RDN,OM_NO_MORE_TYPES,0,0),
                    status,
                    DS_E_BAD_ARGUMENT
                )

                TRACE_NOTE("new_RDN converted to a private object")
            }
            else {
                TRACE_NOTE("new_RDN is a private object")
                pri_new_RDN = new_RDN;
                if (! OMP_DATA(new_RDN)) {
                    TRACE_ERROR("new_RDN has not been initialized")
                    CHECK_OMP_CREATE_ERROR(
                        dsP_DS_C_LIBRARY_ERROR,
                        status,
                        DS_E_BAD_ARGUMENT,
                        DS_E_BAD_ARGUMENT
                    )
                }
            }
        }
        else {
            TRACE_ERROR("new_RDN is not an instance of DS_C_RDN")

            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_ARGUMENT,
                DS_E_BAD_ARGUMENT
            )
        }

        pB.d21_Bpbhead      = &pbhead;
        pB.d21_Bc_a         = ((xds_context *)OMP_DATA(pri_context));
        pB.d21_Bentry       = ((xds_dist_name)OMP_DATA(pri_name));
        pB.d21_Bnew_rdn     = ((xds_dist_name)OMP_DATA(pri_new_RDN));
        pB.d21_Bdel_old_rdn = delete_old_RDN;
        pB.d21_Berr         = NULL;

        if (pB.d21_Bbind_id == -1)
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_CONTEXT,
                DS_E_BAD_CONTEXT
            )


        if (d21_B_replace_rdn(&pB) == D2_NOERROR) {
            TRACE_NOTE2("%s %s\n","successful replacement of old RDN with:",
                        (xds_dist_name)OMP_DATA(pri_new_RDN))
            *invoke_id_return = (OM_sint) 0;
        }
        else {
            TRACE_WARN("unsuccessful replacement of RDN")

            if (dsP_iapl2xds_error(ds_modify_rdn_err_tab, 
  MODIFY_RDN_ERR_TAB_SIZE,pB.d21_Bpbhead,pB.d21_Berr,&status,workspace) != OK) {

                TRACE_WARN2("cannot map IAPL error (%d) %s\n",
				pB.d21_Bpbhead->d2_errvalue, "onto an XDS error");
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

    do {
        if (tmp_pri_obj)
            CHECK_OMP_CALL(
                om_delete,
                (pri_name),
                status,
                DS_E_MISCELLANEOUS
            )
    } while (FALSE);

#ifdef THREADSAFE
    if (op_num)
        dsP_dec_op_num(session);
#endif

    if (status != DS_SUCCESS) {
        TRACE_OERR
        TRACE_OBJECT(1, status)
    }

end:
    TRACE_EXIT

	ABANDON_POSTLUDE_NONQUERY

    return(status);

} /* end ds_modify_rdn */
