/*
 *  @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: server_svc.c,v $
 * Revision 1.1.4.2  1996/02/18  19:36:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:22  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:32:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:53  root]
 * 
 * Revision 1.1.2.3  1994/08/03  19:04:39  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:28:09  mccann]
 * 
 * Revision 1.1.2.2  1994/06/09  18:43:52  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:25  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/12  22:12:30  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  20:03:39  peckham]
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1993-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 * FACILITY:    Cell Directory Service Daemon (cdsd)
 *
 * ABSTRACT:    Servicibility action routines for the exported RPC 
 *              servicibility interface of the server sub-component.
 *
 *
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/dce_error.h>
#include <dce/svcremote.h>
#include <dce/dcesvcmsg.h>
#include <dce/aclbase.h>
#include <pthread.h>
#include <threads_lib.h>
#include <dpeacl.h>
#include <ta.h>
#include <security.h>


/*
 * Description:
 *
 *      RPC remote action routine to set/change the DCE servicibility routing 
 *      level(s) and output location.
 *
 * Inputs:
 *
 *      h        - RPC binding handle for checking authorisation
 *      where    - String indicating the servicibility level(s) and output 
 *                 location(s) to set/change.
 *
 * Implicit Inputs:
 *
 *      The current servicibility table
 *
 * Outputs:
 *
 *      st       - returned status indicating operations success or failure.
 *
 * Implicit Outputs:
 *
 *      The servicibility routing table is altered to reflect the new 
 *      level(s) and output location(s) via a call to dce_svc_routing().
 * 
 * Return Value :
 *
 *      none 
 *
 */
void
dce_svc_set_route(
    handle_t            h,
    idl_byte            where[],
    error_status_t      *st)
{
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >dce_svc_set_route :\n\thandle %lx\n\twhere %s\n",
        h,
        where));

#ifdef DCE_SEC
    /*
    ** Check if user has write access for this operation then 
    ** attempt to change routing.
    */
    if (!TestServerUserAccess(h, sec_acl_perm_write))
    {
        *st = svc_s_no_perm;
        /*
        ** If we fail to obtain security print debug warning
        */
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_set_route : \n\tno authority to change routing\n\terror (%lx)\n",
            *st));
        return;
    }
#endif /* #ifdef DCE_SEC */

    dce_svc_routing(where, st);
    if (*st != svc_s_ok)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_set_route :\n\tunable to change svc routing\n\terror %lx\n",
            *st));
        return;
    }
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dce_svc_set_route : Successfull svc routing operation\n"));
    
    return;
} /* End dce_svc_set_route */


/*
 * Description:
 *
 *      RPC remote action routine to set/change the DCE servicibility 
 *      DEBUG routing level(s) and output location(s).
 *
 * Inputs:
 *
 *      h        - RPC binding handle for checking authorisation
 *      where    - String indicating the servicibility DEBUG level(s) and
 *                 output location(s) to set/change.
 *
 * Implicit Inputs:
 *
 *      The current servicibility table 
 *
 * Outputs:
 *
 *      st       - returned status indicating operations success or failure.
 *
 * Implicit Outputs:
 *
 *      The servicibility routing table is altered to reflect the new 
 *      level(s) and output location(s) for DEBUG via a call to 
 *      dce_svc_debug_routing().
 * 
 * Return Value :
 *
 *      none 
 *
 */
void
dce_svc_set_dbg_route(
    handle_t            h,
    idl_byte            where[],
    error_status_t      *st)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >dce_svc_set_dbg_routing :\n\thandle %lx\n\twhere %s\n",
        h,
        where));

#ifdef DCE_SEC
    /*
    ** Check if user has write access for this operation then 
    ** attempt to change routing.
    */
    if (!TestServerUserAccess(h, sec_acl_perm_write))
    {
        *st = svc_s_no_perm;
        /*
        ** If we fail to obtain security print debug warning
        */
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_set_dbg_routing :\n\tno authority to change svc debug routing\n\terror %lx\n",
            *st));
        return;
    }
#endif /* #ifdef DCE_SEC */

    dce_svc_debug_routing(where, st);
    if (*st != svc_s_ok)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_set_dbg_routing :\n\tunable to change svc debug routing\n\terror %lx\n",
            *st));
        return;
    }
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dce_svc_set_dbg_routing : Successfull svc routing operation \n"));
    return;
} /* End dce_svc_set_dbg_routing */


/*
 * Description:
 *
 *      RPC remote action routine to set/change the DCE servicibility DEBUG
 *      level(s).
 *
 * Inputs:
 *
 *      h        - RPC binding handle for checking authorisation
 *      flags    - String indicating the DEBUG level(s) to set/change.
 *
 * Implicit Inputs:
 *
 *      The current servicibility table.
 *
 * Outputs:
 *
 *      st       - returned status indicating operations success or failure.
 *
 * Implicit Outputs:
 *
 *      The servicibility routing table is altered to reflect the new 
 *      DEBUG level(s) via a call to dce_svc_debug_set_levels().
 * 
 * Return Value :
 *
 *      none 
 *
 */
void
dce_svc_set_dbg_levels(
    handle_t            h,
    idl_byte            flags[],
    error_status_t      *st)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >dce_svc_set_dbg_levels :\n\thandle %lx\n\tflags %s\n",
        h,
        flags));

#ifdef DCE_SEC
    /*
    ** Check if user has write access for this operation then 
    ** attempt to change routing.
    */
    if (!TestServerUserAccess(h, sec_acl_perm_write))
    {
        *st = svc_s_no_perm;
        /*
        ** If we fail to obtain security print debug warning
        */
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_set_dbg_levels :\n\tno authority to change svc debug levels\n\terror %lx\n",
            *st));
        return;
    }
#endif /* #ifdef DCE_SEC */

    dce_svc_debug_set_levels(flags, st);
    if (*st != svc_s_ok)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_set_dbg_levels :\n\tunable to change svc debug levels\n\terror %lx\n",
            *st));
        return;
    }
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dce_svc_set_dbg_levels : Successfull svc routing operation\n"));
    return;
} /* End dce_svc_set_dbg_levels */


/*
 * Description:
 *
 *      RPC remote action routine to retrieve a listing of all DCE components
 *      in this process currently registered with the servicibility component.
 *
 * Inputs:
 *
 *      h        - RPC binding handle for checking authorisation
 *
 * Implicit Inputs:
 *
 *      The current servicibility table
 *
 * Outputs:
 *
 *      table    - An array of strings listing all components currently
 *                 registered with the servicibility component.
 *      st       - returned status indicating operations success or failure.
 *
 * Implicit Outputs:
 *
 *      none
 * 
 * Return Value :
 *
 *      none 
 *
 */
void
dce_svc_inq_components(
    handle_t                    h,
    dce_svc_stringarray_t       *table,
    error_status_t              *st)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >dce_svc_inq_components :\n\thandle %lx\n\ttable %lx\n",
        h,
        table));

#ifdef DCE_SEC
    /*
    ** Check if user has write access for this operation then 
    ** attempt to change routing.
    */
    if (!TestServerUserAccess(h, sec_acl_perm_read))
    {
        *st = svc_s_no_perm;
        /*
        ** If we fail to obtain security print debug warning
        */
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_inq_table :\n\tno authority to retreive registered svc components table\n\terror %lx\n",
            *st));
        return;
    }
#endif /* #ifdef DCE_SEC */

    dce_svc_components(table, st);
    if (*st != svc_s_ok)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_inq_components :\n\tunable to retreive registered svc components table\n\terror %lx\n",
            *st));
        return;
    }
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dce_svc_inq_components : Successfull svc routing operation\n"));
    return;
} /* End dce_svc_inq_components */


/*
 * Description:
 *
 *      RPC remote action routine to retrieve the servicibility table for 
 *      a named DCE component and return a copy of its contents
 *
 * Inputs:
 *
 *      h         - RPC binding handle for checking authorisation
 *      component - String identifying the requested component.
 *
 * Implicit Inputs:
 *
 *      none
 *
 * Outputs:
 *
 *      table    - An array of structures which contain the servicibility 
 *                 settings for each sub-component of the component 
 *                 requested.
 *      st       - returned status indicating operations success or failure.
 *
 * Implicit Outputs:
 *
 *      none
 * 
 * Return Value :
 *
 *      none 
 *
 */
void
dce_svc_inq_table(
    handle_t                    h,
    dce_svc_string_t            component,
    dce_svc_subcomparray_t      *table,
    error_status_t              *st)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >dce_svc_inq_table :\n\thandle %lx\n\tcomponent %s\n",
        h,
        component));

#ifdef DCE_SEC
    /*
    ** Check if user has write access for this operation then 
    ** attempt to change routing.
    */
    if (!TestServerUserAccess(h, sec_acl_perm_read))
    {
        *st = svc_s_no_perm;
        /*
        ** If we fail to obtain security print debug warning
        */
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_inq_table :\n\tno authority to retreive svc component table\n\terror %lx\n",
            *st));
        return;
    }
#endif /* #ifdef DCE_SEC */

    dce_svc_table(component, table, st);
    if (*st != svc_s_ok)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_inq_table :\n\tunable to retreive svc component table\n\terror %lx\n",
            *st));
        return;
    }
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dce_svc_inq_table : Successfull svc routing operation\n"));
    return;
} /* End dce_svc_inq_table */

/*
 * Description:
 *
 *      RPC remote action routine to retrieve all currently specified
 *      routings for all components.
 *
 * Inputs:
 *
 *      h         - RPC binding handle for checking authorisation
 *
 * Implicit Inputs:
 *
 *      none
 *
 * Outputs:
 *
 *      table    - An array of structures which contain the servicibility 
 *                 settings for all components registered. 
 *      st       - returned status indicating operations success or failure.
 *
 * Implicit Outputs:
 *
 *      none
 * 
 * Return Value :
 *
 *      none 
 *
 */
void
dce_svc_inq_routings(
    handle_t                    h,
    boolean32                   debug_values,
    dce_svc_stringarray_t       *table,
    error_status_t              *st)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >dce_svc_inq_routings :\n\thandle %lx\n\tdebug requested %d\n",
        h,
        debug_values));

#ifdef DCE_SEC
    /*
    ** Check if user has write access for this operation then 
    ** attempt to change routing.
    */
    if (!TestServerUserAccess(h, sec_acl_perm_read))
    {
        *st = svc_s_no_perm;
        /*
        ** If we fail to obtain security print debug warning
        */
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_inq_routings :\n\tno authority to retreive svc routing specifications \n\terror %lx\n",
            *st));
        return;
    }
#endif /* #ifdef DCE_SEC */

    dce_svc_routings(table, debug_values, st);
    if (*st != svc_s_ok)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_inq_routings :\n\tunable to retreive svc routings\n\terror %lx\n",
            *st));
        return;
    }
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dce_svc_inq_routings : Successfull svc routing operation\n"));
    return;
} /* End dce_svc_inq_routings */

/*
 * Description:
 *
 *      RPC remote action routine to set/change the DCE servicibility 
 *      filtering that is occuring for this process. 
 *
 * Inputs:
 *
 *      h         - RPC binding handle for checking authorisation
 *      component - String identifying the requested component.
 *      arg_size  - Number of extra arguements (component specific).
 *      arguement - extra arguements (component specific)
 *
 * Implicit Inputs:
 *
 *      none
 *
 * Outputs:
 *
 *      st       - returned status indicating operations success or failure.
 *
 * Implicit Outputs:
 *
 *      The servicibility filtering done for the requested component msy be
 *      altered.
 * 
 * Return Value :
 *
 *      none 
 *
 */
void
dce_svc_filter_control(
    handle_t                    h,
    dce_svc_string_t            component,
    idl_long_int                arg_size,
    idl_byte                    *argument,
    error_status_t              *st)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >dce_svc_filter_control :\n\thandle %lx\n\tcomponent %s\n",
        h,
        component));

#ifdef DCE_SEC
    /*
    ** Check if user has write access for this operation then 
    ** attempt to change routing.
    */
    if (!TestServerUserAccess(h, sec_acl_perm_write))
    {
        *st = svc_s_no_perm;
        /*
        ** If we fail to obtain security print debug warning
        */
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_filter_control :\n\tno authority to change svc filter settings\n\terror %lx\n",
            *st));
        return;
    }
#endif /* #ifdef DCE_SEC */

    dce_svc_filter(component, arg_size, argument, st);
    if (*st != svc_s_ok)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            " <dce_svc_filter_control :\n\tunable to change svc filter settings\n\terror %lx\n",
            *st));
        return;
    }
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dce_svc_filter_control : Successfull svc routing operation\n"));
    return;
} /* End dce_svc_filter_control */


/*
 * Description:
 *
 *      RPC remote action routine to retreive and return servicibility 
 *      statistics.  
 *
 * Inputs:
 *
 *      h        - RPC binding handle for checking authorisation
 *
 * Implicit Inputs:
 *
 *      none
 *
 * Outputs:
 *      
 *      stats    - returned servicibility statistics.
 *      st       - returned status indicating operations success or failure.
 *
 * Implicit Outputs:
 *
 *      none
 * 
 * Return Value :
 *
 *      none 
 *
 */
void
dce_svc_inq_stats(
    handle_t            h,
    dce_svc_stats_t     *stats,
    error_status_t      *st)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >dce_svc_inq_stats :\n\thandle %lx\n",
        h));
    *st = svc_s_no_stats;
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dce_svc_inq_stats : Successfull svc routing opertaion\n"));
    return;
}

serviceability_v1_0_epv_t dce_svc_ep = {
    dce_svc_set_route,
    dce_svc_set_dbg_route,
    dce_svc_set_dbg_levels,
    dce_svc_inq_components,
    dce_svc_inq_table,
    dce_svc_inq_routings,
    dce_svc_filter_control,
    dce_svc_inq_stats
};
