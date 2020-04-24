/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsUtils.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:54  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:35:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:09  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:20:15  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:55  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:49:01  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:24  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:31  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:10:24  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:39:30  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:58:05  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSUTILS_H
#define _DSUTILS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dsutils_rcsid[] = "@(#)$RCSfile: dsUtils.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:40 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


#include <xom.h>
#include <xdsp.h>

OM_sint
dsP_verify_instance(
    OM_object               object,
    OM_object_identifier    class
);

DS_status
dsP_handle_omP_error(
    const OM_return_code  rc,
    const char *           fn,
    const OM_sint         ec,
    const OM_workspace    workspace
);

OM_sint
dsP_iapl2xds_error(
	dsP_error_entry    *err_tab,
	OM_sint            entry_num,
    D2_pbhead          *pb,
    D2_error           *err,
    DS_status          *status,
    OM_workspace       workspace
);

OM_sint
dsP_find_ete_entry(
    dsP_error_entry    *et,
    OM_sint            entry_num,
    unsigned long      ul,
    dsP_error_entry    **entry
);

OM_sint
dsP_validate_wkspace(
    OM_workspace   workspace,
    OM_sint        *index
);

OM_sint
dsP_validate_session(
    OM_private_object  session,
    OM_sint            *bind_id,
    OM_sint            *op_num
);

OM_sint
dsP_dec_op_num(
    OM_private_object  session
);

OM_sint
dsP_validate_context(
    OM_private_object   context
);

OM_sint
dsP_validate_name(
    OM_object           name,
    OM_private_object   *pri_name,
    DS_status           *status,
    OM_workspace        workspace
);

OM_sint
dsP_validate_selection(
    OM_object           selection,
    OM_private_object   *pri_selection,
    DS_status           *status,
    OM_workspace        workspace
);

OM_sint
dsP_xds2iapl_filter(
    xds_filter    *xds_filt,
    D2_Fi_filter **iapl_filt
);

OM_sint
dsP_count_filters(
    xds_filter    *xds_filt,
    OM_sint           *count
);

OM_sint
dsP_fill_filters(
    xds_filter    *x_f,
    D2_Fi_filter  *i_f,
    OM_sint            i_f_cur,
    OM_sint           *i_f_nxt
);

OM_sint
dsP_fill_filter_items(
    xds_fi_item  *x_fi,
    D2_Fi_filter *i_f,
    OM_sint          *i_f_nxt
);

OM_sint
dsP_free_iapl_filter(
        D2_Fi_filter    *iapl_filt
);

void
dsP_init_once(
    void
);

#ifdef THREADSAFE
OM_sint
dsP_thread_id(
    void
);

#endif /* THREADSAFE */
#endif /* ifndef _DSUTILS_H */
