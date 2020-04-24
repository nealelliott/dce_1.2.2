/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsVersion.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:46  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:37:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:40  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:48:54  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:48:58  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:48:58  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:02  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:22:35  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:22:10  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:05:56  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:09:01  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:18:53  marrek]
 * 
 * Revision 1.1.4.4  1992/12/30  23:29:51  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:20:06  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  17:36:58  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	[1992/12/04  12:15:21  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:10:06  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:46:46  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:15:39  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:42:47  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsVersion.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:18 $";
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
/* NAME         : dsVersion.c                                         */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    : X/Open Directory Services API Specification ver. 1.0*/
/*              : OSI Object Management API Specification ver. 2.0    */
/*                XDS/XOM Design Specification August 1990            */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : ds_version() function                              */
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
#	include <pthread.h>
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
#include <dsVersion.h>


DS_status   
ds_version(
    DS_feature   feature_list[],
    OM_workspace workspace)
{
    OM_sint          i;
    OM_sint          index;
    DS_status        status = DS_SUCCESS;

    TRACE_ENTER ("ds_version()")

    INIT_ONCE

    if (dsP_validate_wkspace(workspace, &index) != OK) {
        status = DS_INVALID_WORKSPACE;
		goto end;
    } /* endif */

    TRACE_IARGS
    TRACE_FEATURE(feature_list)
    
    do {
        CHECK_OMP_CALL(
            omP_version,
            (workspace, (OMP_feature *)feature_list),
            status,
            DS_E_MISCELLANEOUS)
    } while (FALSE);

#ifdef THREADSAFE
    if (dsP_validate_wkspace(workspace, &index) != OK)
        status = DS_INVALID_WORKSPACE;
#endif

    LOCK(dsP_wksptab_mutex)

    for (i=0; 
       ((feature_list[i].feature.length != 0) && (status == DS_SUCCESS)); i++) {

        if ((feature_list[i].activated == OM_TRUE) &&
            (feature_list[i].feature.length == dsP_DSX_GDS_PKG.length) &&
            (memcmp(feature_list[i].feature.elements,
                    dsP_DSX_GDS_PKG.elements,
                    dsP_DSX_GDS_PKG.length) == 0)) {
            TRACE_NOTE("Global Directory Service Package activated")

            if (dsP_wkspace_table[index].default_session) {
                CHECK_OMP_CALL(
                    om_delete,
                    (dsP_wkspace_table[index].default_session),
                    status,
                    DS_E_MISCELLANEOUS)
                dsP_wkspace_table[index].default_session = 0;
            } /* endif */

            CHECK_OMP_CALL(
                om_create,
                (dsP_DSX_C_GDS_SESSION, TRUE, workspace, 
                 &dsP_wkspace_table[index].default_session),
                status,
                DS_E_MISCELLANEOUS)
            TRACE_NOTE("default-session is a DSX_C_GDS_SESSION object");

            if (dsP_wkspace_table[index].default_context) {
                CHECK_OMP_CALL(
                    om_delete,
                    (dsP_wkspace_table[index].default_context),
                    status,
                    DS_E_MISCELLANEOUS)
                dsP_wkspace_table[index].default_context = 0;
            } /* endif */

            CHECK_OMP_CALL(
                om_create,
                (dsP_DSX_C_GDS_CONTEXT, TRUE, workspace,
                 &dsP_wkspace_table[index].default_context),
                status,
                DS_E_MISCELLANEOUS)
            TRACE_NOTE("default-context is a DSX_C_GDS_CONTEXT object");
            continue;
        } /* endif */

        if ((feature_list[i].activated == OM_TRUE) &&
            (feature_list[i].feature.length == 
             dsP_DS_BASIC_DIR_CONTENTS_PKG.length) &&
            (memcmp(feature_list[i].feature.elements, 
                    dsP_DS_BASIC_DIR_CONTENTS_PKG.elements,
                    dsP_DS_BASIC_DIR_CONTENTS_PKG.length) == 0))
            TRACE_NOTE("Basic Directory Contents Package activated")

        if ((feature_list[i].activated == OM_TRUE) &&
            (feature_list[i].feature.length == dsP_DS_MHS_DIR_USER_PKG.length)&&
            (memcmp(feature_list[i].feature.elements,
                    dsP_DS_MHS_DIR_USER_PKG.elements,
                    dsP_DS_MHS_DIR_USER_PKG.length) == 0))
              TRACE_NOTE("MHS Directory User Package activated")

        if ((feature_list[i].activated == OM_TRUE) &&
            (feature_list[i].feature.length == dsP_DS_STRONG_AUTHENT_PKG.length) &&
            (memcmp(feature_list[i].feature.elements,
                    dsP_DS_STRONG_AUTHENT_PKG.elements,
                    dsP_DS_STRONG_AUTHENT_PKG.length) == 0))
              TRACE_NOTE("Strong Authentication Package activated")

    } /* endfor */

    UNLOCK(dsP_wksptab_mutex)

    if (status != DS_SUCCESS) {
        TRACE_OERR
        TRACE_OBJECT(1, status)
    } /* endif */

end:
    TRACE_EXIT
    return (status);

} /* end ds_version */
