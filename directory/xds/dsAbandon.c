/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsAbandon.c,v $
 * Revision 1.1.10.2  1996/02/18  18:24:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:03  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:35:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:11  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:48:23  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:34:40  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:22:14  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:19:08  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:56:27  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  06:58:38  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:10:50  marrek]
 * 
 * Revision 1.1.4.4  1992/12/30  23:28:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:18:46  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  09:43:00  marrek
 * 	Add bad pointer check
 * 	[1992/12/16  08:57:26  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:06:06  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:42:42  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:13:10  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:37:08  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsAbandon.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:24:15 $";
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
/* NAME         : dsAbandon.c                                         */
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
/* DESCRIPTION  : ds_abandon() function                              */
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
#include <xdsiapl.h>
#include <xds.h>
#include <xdsp.h>

#include <xdsimport.h>
#include <xdscommon.h>
#include <xdstrace.h>
#include <dsAbandon.h>


DS_status
ds_abandon(
    OM_private_object   session,
    OM_sint             invoke_id
)
{
    DS_status             status;
    OM_sint                   index;
    OM_workspace workspace;

    TRACE_ENTER ("ds_abandon()")

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

    do {
        CHECK_OMP_CREATE_ERROR(
            dsP_DS_C_ABANDON_FAILED,
            status,
            DS_E_MISCELLANEOUS,
            DS_E_TOO_LATE
        )
    } while (FALSE);

    if (((xds_error *)OMP_DATA(status))->val == DS_E_TOO_LATE)
        TRACE_ERROR("Abandon-Failed (44) returned")

    TRACE_OERR
    TRACE_OBJECT(1, status)

end:
    TRACE_EXIT

	ABANDON_POSTLUDE_NONQUERY

    return(status);

} /* end ds_abandon */
