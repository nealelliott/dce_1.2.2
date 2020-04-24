/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsDispatch.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:33:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:49  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:03  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:28  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:22:48  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:09:24  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:36:33  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:54:43  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSDISPATCH_H
#define _DSDISPATCH_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dsdispatch_rcsid[] = "@(#)$RCSfile: dsDispatch.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:16 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


DS_status ds_abandon(
    OM_private_object      session,
    OM_sint                invoke_id
);
DS_status ds_add_entry(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_object           entry,
    OM_sint             *invoke_id_return
);
DS_status ds_bind(
    OM_private_object   session,
    OM_workspace        workspace,
    OM_private_object   *bound_session_return
);
DS_status ds_compare(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_object           ava,
    OM_private_object   *result_return,
    OM_sint             *invoke_id_return
);
OM_workspace ds_initialize(
    void
);
DS_status ds_list(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_private_object   *result_return,
    OM_sint             *invoke_id_return
);
DS_status ds_modify_entry(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_object           changes,
    OM_sint             *invoke_id_return
);
DS_status ds_modify_rdn(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_object           new_RDN,
    OM_boolean          delete_old_RDN,
    OM_sint             *invoke_id_return
);
DS_status ds_read(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_object           selection,
    OM_private_object   *result_return,
    OM_sint             *invoke_id_return
);
DS_status ds_receive_result(
    OM_private_object   session,
    OM_uint             *completion_flag_return,
    DS_status           *operation_status_return,
    OM_private_object   *result_return,
    OM_sint             *invoke_id_return
);
DS_status ds_remove_entry(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_sint             *invoke_id_return
);
DS_status ds_search(
    OM_private_object   session,
    OM_private_object   context,
    OM_object           name,
    OM_sint             subset,
    OM_object           filter,
    OM_boolean          search_aliases,
    OM_object           selection,
    OM_private_object   *result_return,
    OM_sint             *invoke_id_return
);
DS_status ds_shutdown(
    OM_workspace   workspace
);
DS_status ds_unbind(
    OM_private_object   session
);
DS_status ds_version(
    DS_feature          feature_list[],
    OM_workspace        workspace
);
#endif /* ifndef _DSDISPATCH_H */
