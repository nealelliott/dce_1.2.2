/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsShutdown.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:40  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:37:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:31  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:48:53  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:40  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:22:25  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:22:03  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:05:03  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:07:40  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:17:44  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  23:29:36  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:47  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:08:50  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:45:37  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:15:04  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:41:05  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsShutdown.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:08 $";
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
/* NAME         : dsShutdown.c                                        */
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
/* DESCRIPTION  : ds_shutdown() function                             */
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
#include <dsShutdown.h>



DS_status
ds_shutdown(
    OM_workspace workspace
)
{
    OM_sint          index;
    DS_status        status = DS_SUCCESS;

    TRACE_ENTER ("ds_shutdown()")

    INIT_ONCE

    if (dsP_validate_wkspace(workspace, &index) != OK) {
        status = DS_INVALID_WORKSPACE;
		goto end;
    }

    do {

#ifdef THREADSAFE
       {
            OM_sint  i;
            OM_sint  j;

            LOCK(dsP_bindtab_mutex)

            for (i=0, j=0; i < DSP_MAX_SESSIONS; i++)
                if (dsP_bind_table[i].bound_session)
                    if (OMP_WORKSPACE(dsP_bind_table[i].bound_session) 
                                                                   == workspace)
                        j++;

            UNLOCK(dsP_bindtab_mutex)

            if (j) {
                TRACE_WARN2("%d %s\n", j, "bound session(s) - cannot shutdown")
                CHECK_OMP_CREATE_ERROR(
                    dsP_DS_C_SERVICE_ERROR,
                    status,
                    DS_E_MISCELLANEOUS,
                    DS_E_BUSY
                )
            }
        }
#endif
        omP_shutdown(workspace);

        LOCK(dsP_wksptab_mutex)

        dsP_wkspace_table[index].default_session = 0;
        dsP_wkspace_table[index].default_context = 0;
        dsP_wkspace_table[index].no_filter = 0;
        dsP_wkspace_table[index].select_all_tv = 0;
        dsP_wkspace_table[index].select_all_t = 0;
        dsP_wkspace_table[index].select_none = 0;

        UNLOCK(dsP_wksptab_mutex)

        TRACE_NOTE2 ("workspace %d %s\n", index, "has been shutdown")

    } while (FALSE);

end:
    TRACE_EXIT
    return(status);

} /* end ds_shutdown */
