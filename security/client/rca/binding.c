/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: binding.c,v $
 * Revision 1.1.12.2  1996/03/11  13:28:02  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:49  marty]
 *
 * Revision 1.1.12.1  1995/12/08  18:06:19  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/2  1994/03/23  23:15 UTC  mob
 * 
 * 	HP revision /main/HPDCE01/mob_rodan/1  1994/03/23  23:13 UTC  mob
 * 	Fix CHFts10718 
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:00  mdf
 * 	merge
 * 	[1995/12/08  16:32:02  root]
 * 
 * Revision 1.1.4.2  1993/10/09  02:26:45  rps
 * 	[OT8673] sec_rgy_site_binding_get_info: fix default context dumps
 * 	[1993/10/09  02:25:05  rps]
 * 
 * Revision 1.1.2.3  1993/09/15  15:37:21  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.5.7  1993/03/09  18:33:32  rps
 * 	[OT7258] Add name parms to tower bind interfaces
 * 	[1993/03/09  17:56:54  rps]
 * 
 * Revision 1.1.5.6  1993/03/08  17:54:35  rps
 * 	[OT 7272] Really implement sec_rgy_site_is_readonly()
 * 	[1993/03/03  20:25:38  rps]
 * 
 * Revision 1.1.5.5  1993/02/21  19:39:07  burati
 * 	CR6726 Free tmp context when done in sec_rgy_site_bind_update
 * 	[1993/02/21  00:22:15  burati]
 * 
 * Revision 1.1.5.4  1993/02/09  19:38:26  rps
 * 	[OT6844] Update sec_rgy_site_open_update for replication.
 * 	[1993/02/08  23:34:48  rps]
 * 
 * Revision 1.1.5.3  1992/12/29  13:02:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:40:18  zeliff]
 * 
 * Revision 1.1.5.2  1992/12/08  18:59:29  rps
 * 	Add tower support.
 * 	Add replication update support.
 * 	[1992/12/03  01:29:00  rps]
 * 
 * Revision 1.1.2.4  1992/06/18  21:18:27  sommerfeld
 * 	Call rpc_stralloc rather than doing the malloc/strcpy ourselves.
 * 	Don't use the ..._LEN() macros to pull fields out of the rgy_handle.
 * 	[1992/06/10  22:44:26  sommerfeld]
 * 
 * Revision 1.1.2.3  1992/05/08  20:53:56  sommerfeld
 * 	Remove sec_rgy_site_close (moved to internal_binding.c).
 * 	[1992/04/29  03:09:27  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/03/25  15:13:16  burati
 * 	Oops, make that CR2363, not 2326
 * 	[1992/03/25  00:29:31  burati]
 * 
 * 	CR2326 Destroy mutex in sec_rgy_site_close before free'ing context itself.
 * 	[1992/03/25  00:13:34  burati]
 * 
 * Revision 1.1  1992/01/19  14:47:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  binding.c
**
** Copyright (c) Hewlett-Packard Company 1991,1992,1993
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      DCE Security Service - binding context management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)binding.c	25 - 11/20/91";
#endif

#include <un_maloc.h>
#include <stdlib.h>
#include <binding.h>
#include <rs_misc.h>

#include <rca_pvt.h>
#include <macros.h>
#include <u_str.h>

#include <stdio.h>
#include <dce/dce_cf.h>

 /*
  * DCE Security service binding contexts allow applications to select specific
  * instances of servers for each operation.  Some applications may want to
  * operate on a DCE security instance from another cell or may want to operate
  * on multiple instances simultaneously.  Not all (probably not most) clients
  * want or need explicit context management.  To accommodate such clients, a
  * default context is maintained by the client agent.  The default context
  * handle may be passed to any operation which accepts an [in] context handle
  * parameter.  The default context may be rebound by the client agent
  * whenever the current default site becomes unavailable, or when an update
  * is performed and the default context is bound to a readonly site. Explicit
  * contexts are never automatically rebound.
  */


/*
 * s e c _ r g y _ c e l l _ b i n d
 *
 * Bind to the registry at the specified cell.
 *
 * Error Status:
 * Unable to connect to requested site.
 */

PUBLIC void     sec_rgy_cell_bind
#ifndef __STDC__
    ( cell_name, auth_info, context, status )
    unsigned_char_p_t           cell_name;  /* [in] */
    sec_rgy_bind_auth_info_t    *auth_info; /* [in]  */
    sec_rgy_handle_t            *context;   /* [out] */
    error_status_t              *status;    /* [out] */
#else
  (
    unsigned_char_p_t           cell_name,  /* [in] */
    sec_rgy_bind_auth_info_t    *auth_info, /* [in]  */
    sec_rgy_handle_t            *context,   /* [out] */
    error_status_t              *status
  )
#endif
{
    rca_site_bind(cell_name, auth_info, rca_NSI_cell,
		    NULL, SEC_RGY_SERVER_NAME,
                    (char *) cell_name, context, status);
}


/*
 * s e c _ r g y _ s i t e _ b i n d
 *
 * Establish a connection.  site_name identifies the registry to use.  If the
 * site_name is a string of length zero, a registry is selected by the client
 * agent.
 *
 * Error Status:
 * Unable to connect to requested site.
 */

PUBLIC void     sec_rgy_site_bind
#ifndef __STDC__
    ( site_name, auth_info, context, status )
    unsigned_char_p_t           site_name;  /* [in] */
    sec_rgy_bind_auth_info_t    *auth_info; /* [in]  */
    sec_rgy_handle_t            *context;   /* [out] */
    error_status_t              *status;    /* [out] */
#else
  (
    unsigned_char_p_t           site_name,  /* [in] */
    sec_rgy_bind_auth_info_t    *auth_info, /* [in]  */
    sec_rgy_handle_t            *context,   /* [out] */
    error_status_t              *status
  )
#endif
{
    rca_site_bind(site_name, auth_info, rca_NSI_name | rca_NSI_cell,
		    NULL, SEC_RGY_SERVER_NAME, NULL,
                    context, status);
}


/*
 * S E C _ R G Y _ S I T E _ B I N D _ U P D A T E
 *
 * Bind to an update site.
 *
 * A NULL site_name specifies an update site in the local cell;
 * non-NULL specifies an update site in the cell of the named site.
 *
 * Error Status:
 * no update site available
 */
void sec_rgy_site_bind_update
#ifndef __STDC__
    ( site_name, auth_info, context, status )
    unsigned_char_t             *site_name; /* [in] */
    sec_rgy_bind_auth_info_t    *auth_info; /* [in]  */
    sec_rgy_handle_t            *context;   /* [out] */
    error_status_t              *status;    /* [out] */
#else
  (
    unsigned_char_t             *site_name, /* [in] */
    sec_rgy_bind_auth_info_t    *auth_info, /* [in]  */
    sec_rgy_handle_t            *context,   /* [out] */
    error_status_t              *status
  )
#endif
{
    error_status_t   lst;
    sec_rgy_handle_t search_context;

    sec_rgy_site_bind(site_name, auth_info, &search_context, status);
    if (BAD_STATUS(status))
	return;

    rca_site_resolve_update(auth_info, search_context, context, status);
    sec_rgy_site_close(search_context, &lst);
}


/*
 * S E C _ R G Y _ S I T E _ B I N D _ Q U E R Y
 *
 * Bind to an arbitrary query site.
 *
 */
void sec_rgy_site_bind_query
#ifndef __STDC__
    ( site_name, auth_info, context, status )
    unsigned_char_t             *site_name; /* [in] */
    sec_rgy_bind_auth_info_t    *auth_info; /* [in]  */
    sec_rgy_handle_t            *context;   /* [out] */
    error_status_t              *status;    /* [out] */
#else
  (
    unsigned_char_t             *site_name, /* [in] */
    sec_rgy_bind_auth_info_t    *auth_info, /* [in]  */
    sec_rgy_handle_t            *context,   /* [out] */
    error_status_t              *status
  )
#endif
{
    sec_rgy_site_bind(site_name, auth_info, context, status);
}


/*
 * S E C _ R G Y _ S I T E _ B I N D _ F R O M _ T O W E R V
 *
 * Establish a connection.  tower identifies the registry to use.
 *
 * Error Status:
 * Unable to connect to requested site.
 */
PUBLIC void sec_rgy_site_bind_from_towerv
#ifndef __STDC__
    ( towerv, cell_name, site_name, auth_info, context, status )
    rpc_tower_vector_p_t        towerv;     /* [in]  */
    unsigned_char_t             *cell_name; /* [in]  */
    unsigned_char_t             *site_name; /* [in]  */
    sec_rgy_bind_auth_info_t    *auth_info; /* [in]  */
    sec_rgy_handle_t            *context;   /* [out] */
    error_status_t              *status;    /* [out] */
#else
  (
    rpc_tower_vector_p_t        towerv,     /* [in]  */
    unsigned_char_t             *cell_name, /* [in]  */
    unsigned_char_t             *site_name, /* [in]  */
    sec_rgy_bind_auth_info_t    *auth_info, /* [in]  */
    sec_rgy_handle_t            *context,   /* [out] */
    error_status_t              *status     /* [out] */
  )
#endif
{
    rca_site_bind_from_tower_vec(towerv, cell_name, site_name, auth_info,
				context, status);
}


/*
 * S E C _ R G Y _ S I T E _ B I N D I N G _ G E T _ I N F O
 *
 * Return the site name and authentication info  associated with the
 * context parameter.  If the context is the default context, the
 * info for the default binding is returned.  [out] string parameter
 * storage must be freed by the caller.  Passing in a NULL value for
 * any of the [out] values (except for the status) will prevent that
 * value from being returned.
 */

PUBLIC void sec_rgy_site_binding_get_info
#ifndef __STDC__
    ( context, cell_name, server_name, string_binding, auth_info, status )
    sec_rgy_handle_t          context;
    unsigned_char_t           **cell_name;
    unsigned_char_t           **server_name;
    unsigned_char_t           **string_binding;
    sec_rgy_bind_auth_info_t  *auth_info;
    error_status_t            *status;
#else
  (
    sec_rgy_handle_t          context,
    unsigned_char_t           **cell_name,
    unsigned_char_t           **server_name,
    unsigned_char_t           **string_binding,
    sec_rgy_bind_auth_info_t  *auth_info,
    error_status_t            *status
  )
#endif
{
    CLEAR_STATUS(status);

    if (cell_name) *cell_name = NULL;
    if (server_name) *server_name = NULL;
    if (string_binding) *string_binding = NULL;
    
    /*
     * this will convert the default context a real handle, if necessary
     */
    rca_lock_handle(&context, status);

    if (cell_name && STATUS_OK(status) && CELL_NAME(context)) {
	*cell_name = rpc_stralloc(CELL_NAME(context));
	if (*cell_name == NULL) 
	{
	    SET_STATUS(status, sec_rgy_cant_allocate_memory);
            goto free_none;
	}
    }

    if (server_name && STATUS_OK(status) && SITE_NAME(context)) {
        *server_name = rpc_stralloc(SITE_NAME(context));
	
        if (*server_name == NULL) {
            SET_STATUS(status, sec_rgy_cant_allocate_memory);
            goto free_cell_name;
        }
    }

    if (string_binding && STATUS_OK(status)) {
        rpc_binding_to_string_binding(HANDLE(context), string_binding,
                                        status);
    }

    if (STATUS_OK(status)) {

        if (auth_info) {
            *auth_info = AUTH_INFO(context);
        }
        rca_unlock_handle(context);
        return;
   }

    /*
     * Error exits (and cleanup code)
     */
    if (server_name)
        if (*server_name)
            free((char *) *server_name);
free_cell_name:
    if (cell_name)
        if (*cell_name)
            free((char *) *cell_name);
free_none:
    if (context)
	rca_unlock_handle(context);

}

/*
 * s e c _ r g y _ s i t e _ o p e n
 *
 * Set up the default auth info, and call sec_rgy_site_bind to do the
 * actual work.
 *
 * Error Status:
 * Unable to connect to requested site.
 */

PUBLIC void     sec_rgy_site_open
#ifndef __STDC__
    ( site_name, context, status )
    unsigned_char_t             *site_name; /* [in] */
    sec_rgy_handle_t            *context;   /* [out] */
    error_status_t              *status;    /* [out] */
#else
  (
    unsigned_char_t             *site_name, /* [in] */
    sec_rgy_handle_t            *context,   /* [out] */
    error_status_t              *status
  )
#endif
{
    sec_rgy_bind_auth_info_t auth_info;
    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level = rpc_c_authn_level_pkt_integrity;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_private;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;

    sec_rgy_site_bind(site_name, &auth_info, context, status);
}


/*
 * s e c _ r g y _ s i t e _ o p e n _ u p d a t e
 *
 * Bind to an update site.
 *
 * Error Status:
 * no update site available
 */

PUBLIC void     sec_rgy_site_open_update
#ifndef __STDC__
    ( site_name, context, status )
    unsigned_char_t     *site_name; /* [in] */
    sec_rgy_handle_t    *context;   /* [out] */
    error_status_t      *status;    /* [out] */
#else
  (
    unsigned_char_t     *site_name, /* [in] */
    sec_rgy_handle_t    *context,   /* [out] */
    error_status_t      *status
  )
#endif
{
    sec_rgy_bind_auth_info_t auth_info;
    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level = rpc_c_authn_level_pkt_integrity;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_private;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;

    sec_rgy_site_bind_update(site_name, &auth_info, context, status);
}


/*
 * s e c _ r g y _ s i t e _ o p e n _ q u e r y
 *
 * Bind to an arbitrary query site.
 *
 */

PUBLIC void     sec_rgy_site_open_query
#ifndef __STDC__
    ( site_name, context, status )
    unsigned_char_t     *site_name;  /* [in] */
    sec_rgy_handle_t    *context;    /* [out] */
    error_status_t      *status;     /* [out] */
#else
  (
    unsigned_char_t     *site_name,  /* [in] */
    sec_rgy_handle_t    *context,    /* [out] */
    error_status_t      *status
  )
#endif
{
    sec_rgy_site_open(site_name, context, status);
}


/*
 * s e c _ r g y _ s i t e _ g e t
 *
 * Return the string representation of the site binding associated with the
 * context parameter.  If the context is the default context, the default
 * site binding is returned.
 *
 */

PUBLIC void     sec_rgy_site_get
#ifndef __STDC__
    ( context, site_name, status )
    sec_rgy_handle_t    context;    /* [in] */
    unsigned_char_t     **site_name;/* [out] */
    error_status_t      *status;    /* [out] */
#else
  (
    sec_rgy_handle_t    context,    /* [in] */
    unsigned_char_t     **site_name,/* [out] */
    error_status_t      *status
  )
#endif
{
    sec_rgy_site_binding_get_info(context, NULL, site_name, NULL, NULL, status);
}


/*
 * s e c _ r g y _ s i t e _ i s _ r e a d o n l y 
 *
 * returns true if the context's site is readonly, false otherwise 
 */

PUBLIC boolean32 sec_rgy_site_is_readonly
#ifndef __STDC__
    ( context )
    sec_rgy_handle_t    context;    /* [in] */
#else
  (
    sec_rgy_handle_t    context
  )
#endif
{
    boolean32 readonly;
    error_status_t status;

    readonly = rca_site_check_readonly(context, &status);

    if (BAD_STATUS(&status))	/* conservative */
        return TRUE;

    return readonly;
}
