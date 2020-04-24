/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsAddEntry.c,v $
 * Revision 1.1.77.2  1996/02/18  18:24:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:04  marty]
 *
 * Revision 1.1.77.1  1995/12/08  15:35:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:12  root]
 * 
 * Revision 1.1.75.2  1994/03/23  15:48:24  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:34:44  keutel]
 * 
 * Revision 1.1.75.1  1994/02/22  18:09:20  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:19:12  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:56:37  marrek]
 * 
 * Revision 1.1.73.1  1993/10/14  19:19:13  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:28  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:44:32  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  06:58:52  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:11:00  marrek]
 * 
 * Revision 1.1.4.5  1992/12/30  23:28:55  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:18:51  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/17  09:43:05  marrek
 * 	Add bad pointer check for session
 * 	[1992/12/16  08:58:30  marrek]
 * 
 * Revision 1.1.4.3  1992/12/04  17:35:00  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	[1992/12/04  12:10:34  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:06:19  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:42:53  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:13:18  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:37:14  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsAddEntry.c,v $ $Revision: 1.1.77.2 $ $Date: 1996/02/18 18:24:17 $";
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
/* NAME         : dsAddEntry.c                                        */
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
/* DESCRIPTION  : ds_add_entry() function                            */
/*                ds_add_entry_err_tab[] error-table                 */
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
#include <dsAddEntry.h>

dsP_error_entry    ds_add_entry_err_tab[] = {

{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NOT_SPEC), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_AFFECTS_MULTIPLE_DSAS), XDS_UPDATE_ERROR, DS_E_AFFECTS_MULTIPLE_DSAS },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_AT_VALUE_EXISTS), XDS_ATTRIBUTE_ERROR, DS_E_ATTRIBUTE_OR_VALUE_EXISTS },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_ARGUMENT), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_BAD_NAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_CONSTRAINT_VIOLATION), XDS_ATTRIBUTE_ERROR, DS_E_CONSTRAINT_VIOLATION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ALREADY_EXISTS), XDS_UPDATE_ERROR, DS_E_ENTRY_EXISTS },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ACCESS_RIGHTS_INSUFFICIENT), XDS_SECURITY_ERROR, DS_E_INSUFFICIENT_ACCESS_RIGHTS},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ATSYNTAX_ERROR), XDS_ATTRIBUTE_ERROR, DS_E_INVALID_ATTRIBUTE_SYNTAX },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_INVALID_REFERENCE), XDS_SERVICE_ERROR, DS_E_INVALID_REF },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NAMING_VIOLATION), XDS_UPDATE_ERROR, DS_E_NAMING_VIOLATION},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NFOUND), XDS_NAME_ERROR, DS_E_NO_SUCH_OBJECT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_OBJ_CLS_VIOLATION), XDS_UPDATE_ERROR, DS_E_OBJECT_CLASS_VIOLATION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_UNDEFINED_ATTR_TYPE), XDS_ATTRIBUTE_ERROR, DS_E_UNDEFINED_ATTRIBUTE_TYPE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NMSYNTAX_ERROR), XDS_NAME_ERROR, DS_E_INVALID_ATTRIBUTE_VALUE },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_ON_ALIAS_NOT_ALLOWED), XDS_NAME_ERROR, DS_E_ALIAS_DEREFERENCING_PROBLEM },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_DSA_BIND), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SELECTION), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_SC), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_ATL_EXCEEDED), XDS_LIBRARY_ERROR, DS_E_BAD_ARGUMENT },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_RDN_MISSING), XDS_NAME_ERROR, DS_E_NAMING_VIOLATION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_RDN_TOO_LONG), XDS_NAME_ERROR, DS_E_NAMING_VIOLATION },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_REF_ERROR), XDS_REFERRAL },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_NO_ENTRYNAME), XDS_LIBRARY_ERROR, DS_E_BAD_NAME },
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D21_SERV_CTRLS_INVALID), XDS_LIBRARY_ERROR, DS_E_BAD_CONTEXT},
{XHASH(D2_CALL_ERR,D2_PAR_ERR,D2_NO_BIND), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_CALL_ERR,D2_ILL_VERS,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_BAD_SESSION },
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D2_NOT_SPEC), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS },
{XHASH(D2_TEMP_ERR,D2_SYS_ERR,D21_ALLOC_ERROR), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_TOO_BUSY), XDS_SERVICE_ERROR, DS_E_BUSY },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_NO_RESPONSE), XDS_SERVICE_ERROR, DS_E_UNAVAILABLE },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_CHAINING_REQUIRED), XDS_SERVICE_ERROR, DS_E_CHAINING_REQUIRED },
{XHASH(D2_TEMP_ERR,D2_REM_ERR,D2_UNWILLING), XDS_SERVICE_ERROR, DS_E_UNWILLING_TO_PERFORM },
{XHASH(D2_TEMP_ERR,D2_INT_ERR,D21_RESULTAPDU_INVALID), XDS_LIBRARY_ERROR, DS_E_MISCELLANEOUS},
{XHASH(D2_PERM_ERR,D2_SYS_ERR,D2_NOT_SPEC), XDS_SYSTEM_ERROR },
{XHASH(D2_PERM_ERR,D2_REM_ERR,D2_OUT_OF_SCOPE), XDS_SERVICE_ERROR, DS_E_OUT_OF_SCOPE }

};

#define ADD_ENTRY_ERR_TAB_SIZE \
        (sizeof(ds_add_entry_err_tab) / sizeof(ds_add_entry_err_tab[0]))


DS_status   
ds_add_entry(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_object           entry,
    OM_sint             *invoke_id_return
)
{
    D2_pbhead             pbhead = { IAPL_VERSION };
    D21_p7                p7;
    DS_status             status = DS_SUCCESS;
    OM_private_object     pri_context;
    OM_private_object     pri_name;
    OM_private_object     pri_attribute_list;
    OM_sint                   bind_id;
    OM_sint                   op_num = 0;
    OM_sint                   index;
    OM_sint          tmp_pri_obj = FALSE;
    OM_sint          tmp_pri_entry = FALSE;
    OM_workspace workspace;

    TRACE_ENTER ("ds_add_entry()")

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
    TRACE_OBJECT(0, entry)

    do {
        if (dsP_validate_session(session, &bind_id, &op_num) != OK)
            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_SESSION,
                DS_E_BAD_SESSION
            )
        else
            p7.d21_7bind_id = (short) bind_id;


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

        if (dsP_verify_instance (entry, dsP_DS_C_ATTRIBUTE_LIST) != FALSE) {

            if ((! (OMP_PRIVATE(entry))) || 
                ((OMP_PRIVATE(entry)) && 
                (((OMP_private_object *)OMP_INTERNAL(entry))->object_type.class 
                                                      != XDS_ATTRIBUTE_LIST))) {

                CHECK_OMP_CALL(
                    om_create,
                    (dsP_DS_C_ATTRIBUTE_LIST, 0, workspace,&pri_attribute_list),
                    status,
                    DS_E_MISCELLANEOUS
                )
                tmp_pri_entry++;

                CHECK_OMP_CALL(
                    om_put,
                    (pri_attribute_list, OM_REPLACE_ALL, entry, 0, 0, 0),
                    status,
                    DS_E_BAD_ARGUMENT
                )
                TRACE_NOTE("entry converted to a private object")
            }
            else {
                TRACE_NOTE("entry is a private object")
                pri_attribute_list = entry;
                if (! OMP_DATA(entry)) {
                    TRACE_ERROR("entry has not been initialized")
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
            TRACE_ERROR("entry is not an instance of DS_C_ATTRIBUTE_LIST")

            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                status,
                DS_E_BAD_ARGUMENT,
                DS_E_BAD_ARGUMENT
            )
        }
        
        p7.d21_7pbhead      = &pbhead;
        p7.d21_7c_a         = ((xds_context *)OMP_DATA(pri_context));
        p7.d21_7entry_name  = ((xds_dist_name)OMP_DATA(pri_name));
        p7.d21_7_at_info    = ((xds_attr_list *)OMP_DATA(pri_attribute_list));
        p7.d21_7err         = NULL;

        if (((p7.d21_7c_a)->d2_serv_cntrls & D2_DUACACHE) && 
            (! ((p7.d21_7c_a)->d2_serv_cntrls & D2_USEDSA))) {
            p7.d21_7bind_id = ((xds_session *)OMP_DATA(session))->dir_id;
            TRACE_NOTE("using DUA-cache only")
        }
        else
            if (p7.d21_7bind_id == -1)
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_LIBRARY_ERROR,
                    status,
                    DS_E_BAD_CONTEXT,
                    DS_E_BAD_CONTEXT
                )


        if (d21_7_add_object(&p7) == D2_NOERROR) {
            TRACE_NOTE2("%s %s\n", "successful addition of:",
			OMP_DATA(pri_name) ? OMP_DATA(pri_name) : "[Root]")
            *invoke_id_return   = (OM_sint) 0;
        }
        else {
            TRACE_WARN2("%s %s\n", "unsuccessful addition of:",
			OMP_DATA(pri_name) ? OMP_DATA(pri_name) : "[Root]")

            if (dsP_iapl2xds_error(
                ds_add_entry_err_tab, ADD_ENTRY_ERR_TAB_SIZE,
                p7.d21_7pbhead, p7.d21_7err, &status, workspace) != OK){


                TRACE_WARN2("cannot map IAPL error (%d) %s\n",
				p7.d21_7pbhead->d2_errvalue, "onto an XDS error");

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
        if (tmp_pri_entry)
            CHECK_OMP_CALL(
                om_delete,
                (pri_attribute_list),
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


} /* end  ds_add_entry */
