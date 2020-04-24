/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsModEntry.c,v $
 * Revision 1.1.10.2  1996/02/18  18:24:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:12  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:36:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:21  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:48:31  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:35:36  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:09:32  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:19:37  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:57:50  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:00:36  marrek
 * 	added DS_E_OBJECT_CLASS_MOD_PROHIB to errors.
 * 	[1993/08/03  09:52:03  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/27  14:12:26  marrek]
 * 
 * Revision 1.1.4.7  1993/02/02  08:59:11  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.6  1993/02/02  08:13:24  marrek
 * 	Insert <pthread.h> and modify error tables.
 * 	[1993/02/01  14:01:41  marrek]
 * 
 * Revision 1.1.4.5  1992/12/30  23:29:16  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:20  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/17  09:43:26  marrek
 * 	Add bad pointer check for session
 * 	[1992/12/16  09:00:14  marrek]
 * 
 * Revision 1.1.4.3  1992/12/04  17:35:49  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	[1992/12/04  12:12:20  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:07:38  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:44:12  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:14:13  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:37:52  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsModEntry.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:24:27 $";
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
/* NAME         : dsModEntry.c                                        */
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
/* DESCRIPTION  : ds_modify_entry() function                         */
/*                ds_modify_entry_err_tab[] error-table              */
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
#include <dsModEntry.h>

dsP_error_entry    ds_modify_entry_err_tab[] = {

{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NOT_SPEC), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_AFFECTS_MULTIPLE_DSAS), XDS_UPDATE_ERROR, DS_E_AFFECTS_MULTIPLE_DSAS },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ALIAS_ERROR), XDS_NAME_ERROR, DS_E_ALIAS_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_AT_VALUE_EXISTS), XDS_ATTRIBUTE_ERROR, DS_E_ATTRIBUTE_OR_VALUE_EXISTS },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_ARGUMENT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_NAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_CONSTRAINT_VIOLATION), XDS_ATTRIBUTE_ERROR, DS_E_CONSTRAINT_VIOLATION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ACCESS_RIGHTS_INSUFFICIENT), XDS_SECURITY_ERROR, DS_E_INSUFFICIENT_ACCESS_RIGHTS},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ATSYNTAX_ERROR), XDS_ATTRIBUTE_ERROR, DS_E_INVALID_ATTRIBUTE_SYNTAX },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_INVALID_REFERENCE), XDS_SERVICE_ERROR, DS_E_INVALID_REF },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ATTRIBUTE_MISSING), XDS_ATTRIBUTE_ERROR, DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NFOUND), XDS_NAME_ERROR, DS_E_NO_SUCH_OBJECT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NOT_ALLOWED_ON_RDN), XDS_UPDATE_ERROR, DS_E_NOT_ALLOWED_ON_RDN },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_OBJCLS_MOD_PROHIBITED), XDS_UPDATE_ERROR, DS_E_OBJECT_CLASS_MOD_PROHIB },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_OBJ_CLS_VIOLATION), XDS_UPDATE_ERROR, DS_E_OBJECT_CLASS_VIOLATION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_UNDEFINED_ATTR_TYPE), XDS_ATTRIBUTE_ERROR, DS_E_UNDEFINED_ATTRIBUTE_TYPE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NMSYNTAX_ERROR), XDS_NAME_ERROR, DS_E_INVALID_ATTRIBUTE_VALUE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ON_ALIAS_NOT_ALLOWED), XDS_NAME_ERROR, DS_E_ALIAS_DEREFERENCING_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_DSA_BIND), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SELECTION), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SC), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_ATL_EXCEEDED), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
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

#define MODIFY_ENTRY_ERR_TAB_SIZE \
                (sizeof(ds_modify_entry_err_tab) / sizeof(ds_modify_entry_err_tab[0]))




DS_status       
ds_modify_entry(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_object           changes,
    OM_sint             *invoke_id_return
)
{
    D2_pbhead             pbhead = { IAPL_VERSION };
    D21_pC                pC;
    DS_status             status = DS_SUCCESS;
    OM_private_object     pri_context;
    OM_private_object     pri_name;
    OM_private_object     pri_changes;
    OM_sint               bind_id;
    OM_sint               op_num = 0;
    OM_sint               index;
    OM_sint          		tmp_pri_obj = FALSE;
    OM_sint          		tmp_pri_chg = FALSE;
    OM_workspace 			workspace;

    TRACE_ENTER ("ds_modify_entry()")

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
    TRACE_OBJECT(0, changes)

    do {
        if (dsP_validate_session(session, &bind_id, &op_num) != OK)
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_SESSION,
                DS_E_BAD_SESSION
        )
        else
            pC.d21_Cbind_id = bind_id;


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

        if (dsP_verify_instance (changes, dsP_DS_C_ENTRY_MOD_LIST) != FALSE) {

            if (! (OMP_PRIVATE(changes))) {

                CHECK_OMP_CALL(
                    om_create,
                    (dsP_DS_C_ENTRY_MOD_LIST, 0, workspace, &pri_changes),
                    status,
                    DS_E_MISCELLANEOUS
                )
                tmp_pri_chg++;

                CHECK_OMP_CALL(
                    om_put,
                    (pri_changes, OM_REPLACE_ALL, changes,OM_NO_MORE_TYPES,0,0),
                    status,
                    DS_E_BAD_ARGUMENT
                )

                TRACE_NOTE("changes converted to a private object")
            }
            else {
                TRACE_NOTE("changes is a private object")
                pri_changes = changes;
                if (! OMP_DATA(changes)) {
                    TRACE_ERROR("changes has not been initialized")
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
            TRACE_ERROR("changes is not an instance of DS_C_ENTRY_MOD_LIST")

            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_ARGUMENT,
                DS_E_BAD_ARGUMENT
            )
        }
                
        pC.d21_Cpbhead  = &pbhead;
        pC.d21_Cc_a     = ((xds_context *)OMP_DATA(pri_context));
        pC.d21_Centry   = ((xds_dist_name)OMP_DATA(pri_name));
        pC.d21_Cmod     = ((xds_entry_mod_list *)OMP_DATA(pri_changes));
        pC.d21_Cerr     = NULL;

        if (pC.d21_Cbind_id == -1)
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_CONTEXT,
                DS_E_BAD_CONTEXT
            )


        if (d21_C_modify_attribute(&pC) == D2_NOERROR) {
            TRACE_NOTE2("%s %s\n","successful modification of:",
			OMP_DATA(pri_name) ? OMP_DATA(pri_name):"[Root]")
            *invoke_id_return = (OM_sint) 0;
        }
        else {
            TRACE_WARN2("%s %s\n","unsuccessful modification of:",
			OMP_DATA(pri_name) ? OMP_DATA(pri_name) : "[Root]")

            if (dsP_iapl2xds_error(ds_modify_entry_err_tab, 
MODIFY_ENTRY_ERR_TAB_SIZE,pC.d21_Cpbhead,pC.d21_Cerr,&status,workspace) != OK) {


                TRACE_WARN2("cannot map IAPL error (%d) %s\n",
				pC.d21_Cpbhead->d2_errvalue, "onto an XDS error");
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
        if (tmp_pri_chg)
            CHECK_OMP_CALL(
                om_delete,
                (pri_changes),
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

} /* end ds_modify_entry */
