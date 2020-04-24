/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsInit.c,v $
 * Revision 1.1.80.2  1996/02/18  18:24:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:09  marty]
 *
 * Revision 1.1.80.1  1995/12/08  15:36:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:18  root]
 * 
 * Revision 1.1.78.3  1994/05/10  16:03:45  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:13:45  marrek]
 * 
 * Revision 1.1.78.2  1994/03/23  15:48:29  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:35:29  keutel]
 * 
 * Revision 1.1.78.1  1994/02/22  18:09:29  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:19:28  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:57:30  marrek]
 * 
 * Revision 1.1.76.1  1993/10/14  19:19:19  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:38  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:44:42  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  07:00:05  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:12:02  marrek]
 * 
 * Revision 1.1.4.4  1992/12/30  23:29:09  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:09  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  17:35:31  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	[1992/12/04  12:11:41  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:07:11  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:43:45  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:13:54  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:37:40  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsInit.c,v $ $Revision: 1.1.80.2 $ $Date: 1996/02/18 18:24:24 $";
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
/* NAME         : dsInit.c                                            */
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
/* DESCRIPTION  : ds_initialize() function                           */
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
#include <xdsimport.h>
#include <alloc.h>
#include <xdsalloc.h>
#include <xdspackage.h>

#include <d2dir.h>
#include <d27util.h>
#include <d21iapl.h>
#include <xdsiapl.h>
#include <xds.h>
#include <xdsp.h>

#include <xdscommon.h>
#include <xdstrace.h>
#include <dsInit.h>


static OM_descriptor select_all_t_mod[] = {
    OMP_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    { DS_INFO_TYPE, OM_S_ENUMERATION, { DS_TYPES_ONLY, 0 }},
    OM_NULL_DESCRIPTOR,
};

static OM_descriptor select_none_mod[] = {
    OMP_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    { DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, { OM_FALSE, 0 }},
    OM_NULL_DESCRIPTOR,
};



OM_workspace    
ds_initialize(
    void
)
{

    D2_pbhead            pbhead = { IAPL_VERSION };
    D2_c_arg             c_a;
    D2_c_res             c_res = { 0, (D2_name_string)0 };
    D21_p3               p3 = D21_P3_NULL;
    D2_ei_info           *ei_info = 0;
    D2_a_type            a_type;
    D2_req_info          r_info;
    xds_session          x_session;
    OM_sint         	 index;
    OM_private_object    default_session;
    OM_private_object    default_context;
    OM_private_object    no_filter;
    OM_private_object    select_all_tv;
    OM_private_object    select_all_t;
    OM_private_object    select_none;
    OM_workspace         workspace = NULL;

    TRACE_ENTER("ds_initialize()")

    INIT_ONCE

    x_session.dsa_name          = NULL;
    x_session.dsa_address       = NULL;
    x_session.requestor         = NULL;
    x_session.password.d2_p_len = 0;
    x_session.password.d2_p_pw  = NULL;
    x_session.dir_id            = 1;
    x_session.auth_mech         = 0;
    x_session.auth_info.d2_size = 0;
    x_session.auth_info.d2_value= NULL;

    do {
        c_a.d2_extension                = NULL;
        c_a.d2_op_progress.d2_nr_phase  = D2_NOT_STARTED;
        c_a.d2_op_progress.d2_next_rdn  = 0;
        c_a.d2_al_rdn                   = 0;
        c_a.d2_serv_cntrls              = D2_DUACACHE;
        c_a.d2_priority                 = D2_PRIO_MEDIUM;
        c_a.d2_time_limit               = D2_T_UNLIMITED;
        c_a.d2_size_limit               = D2_S_UNLIMITED;
        c_a.d2_scope_ref                = D2_DMD_SCOPE;

        p3.d21_3pbhead      = &pbhead;
        p3.d21_3c_a         = &c_a;
        p3.d21_3bind_id     = D21DIR_ID;

        p3.d21_3entry       = (byte *)D2_N_DEF_DSA;

        a_type.d2_typ_len   = sizeof(D2_SEE_ALSO) -1;
        a_type.d2_type      = (byte *)D2_SEE_ALSO;

        r_info.d2_r_val_at  = D2_R_VAL_TYP;
        r_info.d2_r_no_at   = 1;
        r_info.d2_r_type_at = &a_type;

        p3.d21_3r_info      = &r_info;
        p3.d21_3cres        = &c_res;
        p3.d21_3res         = 0;

        if (d21_3_read(&p3) == D2_NOERROR) {
            D2_a_info   *ai = p3.d21_3res->d2_ei_ai;

            if ((ai->d2_a_no_val) &&
                (! D2_ATTCMP(&(ai->d2_a_type),D2_SEE_ALSO))) {

                p3.d21_3entry      =
                x_session.dsa_name = (D2_name_string)
				    ai->d2_a_val->d2_at_value.d2_obj_name_a_v;

                TRACE_NOTE2("%s   %s\n","default DSA name:", x_session.dsa_name); 

                ei_info = p3.d21_3res;
                p3.d21_3res = 0;

                a_type.d2_typ_len  = sizeof(D2_PSAP_ADDRESS) -1;
                a_type.d2_type     = (byte *)D2_PSAP_ADDRESS;
                c_res.d2_al_deref  = 0;
                c_res.d2_performer = (D2_name_string)0;

                if (d21_3_read(&p3) == D2_NOERROR) {
                    ai = p3.d21_3res->d2_ei_ai;
                    if ((ai->d2_a_no_val) &&
                              (! D2_ATTCMP(&(ai->d2_a_type),D2_PSAP_ADDRESS))) {
                         OM_sint  len;
                        Psap_string ps = ai->d2_a_val->d2_at_value.d2_psap_a_v;

                        if ((len = dsP_get_psap_length(ps)) == 0) {
                            TRACE_ERROR("invalid psap address encountered")
                            continue;
                        }
                        x_session.dsa_address = ps;
                    }
                    else
                        TRACE_NOTE("DSA address not found") 
                }
                else
                    TRACE_WARN("cannot read DSA address from DUA cache")
            } 
            else
                TRACE_NOTE("default DSA name not found") 
        }
        else
            TRACE_WARN("cannot read default DSA name from DUA cache")

    } while (FALSE);


    if (OMP_INITIALIZE(&x_session, &workspace) != OM_SUCCESS) {
        TRACE_ERROR ("initialization failure - cannot initialize XOM")
		workspace = NULL;
		goto end;
    }

    if (ei_info)
        free (ei_info);
    if (p3.d21_3res)
        free (p3.d21_3res);


    if (om_create(dsP_DS_C_ENTRY_INFO_SELECTION, TRUE, workspace, 
                                                &select_all_tv) != OM_SUCCESS) {
        TRACE_ERROR ("cannot create Entry-Info-Selection #1")
		workspace = NULL;
		goto end;
    }


    if (om_create(dsP_DS_C_ENTRY_INFO_SELECTION, TRUE, workspace,
                                                 &select_all_t) != OM_SUCCESS) {
        TRACE_ERROR ("cannot create Entry-Info-Selection #2")
		workspace = NULL;
		goto end;
    }
    if (om_put(select_all_t, OM_REPLACE_ALL, select_all_t_mod,OM_NO_MORE_TYPES,
                                                          0, 0) != OM_SUCCESS) {
        TRACE_ERROR ("cannot modify Entry-Info-Selection #2")
		workspace = NULL;
		goto end;
    }


    if (om_create(dsP_DS_C_ENTRY_INFO_SELECTION, TRUE, workspace, &select_none)
                                                                != OM_SUCCESS) {
        TRACE_ERROR ("cannot create Entry-Info-Selection #3")
		workspace = NULL;
		goto end;
    }
    if (om_put(select_none, OM_REPLACE_ALL, select_none_mod, OM_NO_MORE_TYPES,
                                                          0, 0) != OM_SUCCESS) {
        TRACE_ERROR ("cannot modify Entry-Info-Selection #3")
		workspace = NULL;
		goto end;
    }


    if (om_create(dsP_DS_C_FILTER, TRUE, workspace, &no_filter) != OM_SUCCESS){
        TRACE_ERROR ("cannot create No-Filter")
		workspace = NULL;
		goto end;
    }

    if (om_create(dsP_DS_C_SESSION, TRUE, workspace, &default_session) 
                                                                != OM_SUCCESS) {
        TRACE_ERROR ("cannot create default Session")
		workspace = NULL;
		goto end;
    }

    if (om_create(dsP_DS_C_CONTEXT, TRUE, workspace, &default_context)
                                                                != OM_SUCCESS) {
        TRACE_ERROR ("cannot create default Context")
		workspace = NULL;
		goto end;
    }


    LOCK(dsP_wksptab_mutex)

    for (index=0; index < DSP_MAX_WKSPACES; index++)
        if (dsP_wkspace_table[index].default_session == 0)
            break;

    if (index != DSP_MAX_WKSPACES) {
        dsP_wkspace_table[index].select_all_tv   = select_all_tv;
        dsP_wkspace_table[index].select_all_t    = select_all_t;
        dsP_wkspace_table[index].select_none     = select_none;
        dsP_wkspace_table[index].no_filter       = no_filter;
        dsP_wkspace_table[index].default_session = default_session;
        dsP_wkspace_table[index].default_context = default_context;
    }

    UNLOCK(dsP_wksptab_mutex)

    if (index == DSP_MAX_WKSPACES) {
        TRACE_ERROR("too many workspaces - workspace table full")
        omP_shutdown(workspace);
		workspace = NULL;
		goto end;
    }

    TRACE_NOTE2 ("workspace %d %s\n", index, "has been initialized")
end:
    TRACE_EXIT

    return (workspace);

} /* end  ds_initialize */
