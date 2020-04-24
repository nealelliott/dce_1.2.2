/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsRecvRes.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:37  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:36:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:26  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:48:50  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:30  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:22:18  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:21:48  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:04:30  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:06:54  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:17:02  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  23:29:26  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:33  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:08:15  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:44:49  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:14:38  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:38:15  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsRecvRes.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:03 $";
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
/* NAME         : dsRecvRes.c                                         */
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
/* DESCRIPTION  : ds_receive_result() function                       */
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
#include <dsRecvRes.h>



DS_status
ds_receive_result(
    OM_private_object   session,
    OM_uint             *completion_flag_return,
    DS_status           *operation_status_return,
    OM_private_object   *result_return,
    OM_sint             *invoke_id_return
)
{
    OM_sint          bind_id;
    OM_sint          op_num = 0;
    OM_sint          index;
    OM_workspace     workspace;
    DS_status        status = DS_SUCCESS;

    TRACE_ENTER ("ds_receive_result()")

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

#ifdef THREADSAFE
    if ((op_num-1) >0) {
        *completion_flag_return = DS_OUTSTANDING_OPERATIONS;
        TRACE_WARN2("%d %s\n", op_num-1, "outstanding operation(s)")
    }
    else 
#endif
    {
        *completion_flag_return = DS_NO_OUTSTANDING_OPERATION;
        TRACE_WARN("no outstanding operation")
    }
        operation_status_return = (DS_status *)NULL;
        result_return = (OM_private_object *)NULL;
        invoke_id_return = (OM_sint *)NULL;

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
    return(status);
} /* end ds_receive_result */
