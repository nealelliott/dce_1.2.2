/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ek_site.c,v $
 * Revision 1.1.8.2  1996/03/11  13:26:57  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:53  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:59:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:13  root]
 * 
 * Revision 1.1.6.2  1993/11/03  22:39:55  rps
 * 	[OT9312] ek_site_close: invalidate cache
 * 	[1993/11/03  21:53:07  rps]
 * 
 * Revision 1.1.6.1  1993/10/15  22:10:38  rps
 * 	[OT9138] ek_site_open_by_*: don't stomp on existing context if error;
 * 	fix read-only site handling.
 * 	[1993/10/15  20:25:36  rps]
 * 
 * Revision 1.1.4.4  1993/03/09  15:34:58  burati
 * 	CR7349 Get site name from rgy handle instead of from obsolete
 * 	local cache (there's more candidates for 1.1 cleanup in here).
 * 	[1993/03/08  21:00:05  burati]
 * 
 * Revision 1.1.4.3  1993/01/21  22:18:36  burati
 * 	CR4735 Bind unauthenticated on rpc_s_auth_tkt_expired error
 * 	[1993/01/19  18:50:16  burati]
 * 
 * Revision 1.1.4.2  1992/12/29  12:40:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:35:47  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/20  19:34:38  burati
 * 	Handle missing login context error code for unauth rebinding
 * 	[1992/05/20  19:32:25  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* ek_site.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1993
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit kernel site binding routines 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)ek_site.c	20 - 11/22/91";
#endif
 
#include <un_strng.h>

#include <dce/rpcbase.h>
#include <dce/rpcsts.h>
#include <dce/rgybase.h>
#include <dce/binding.h>

#include <ek_site.h>
#include <ek_error.h>
#include <ek_util.h>

#include <un_maloc.h>
#include <macros.h>

typedef struct ek_site_cache_t {
    ek_site_name_t    site;
    ek_site_access_t  access_type;
    boolean32         valid;         /* has a site been opened? */
} ek_site_cache_t;

/* Temp for now to facilitate building */
sec_rgy_handle_t    context = sec_rgy_default_handle;

/* expands to true if the specified cache is valid, false otherwise */
#define SITE_CACHE_VALID(cache) ((cache).valid == true)

#define SITE_CACHE_INVALIDATE(cache) ((cache).valid = false)

    /* Initially the site cache is invalid, so it doesn't really matter what
     * we put in the first three fields of the cache struct. */
PRIVATE ek_site_cache_t Site_cache = { "", ek_site_query, false };


PRIVATE void get_site_cache_contents
#ifndef __STDC__
    ( cache_p, site, access_p, valid ) 
    ek_site_cache_t   *cache_p;    /* [in]  */
    ek_site_name_t    site;        /* [out] */
    ek_site_access_t  *access_p;   /* [out] */
    boolean32         *valid;
#else
  (
    ek_site_cache_t   *cache_p,    /* [in]  */
    ek_site_name_t    site,        /* [out] */
    ek_site_access_t  *access_p,   /* [out] */
    boolean32         *valid
  )
#endif
{
        strcpy((char *)site, (char *) cache_p->site);
        *access_p = cache_p->access_type;
        *valid = cache_p->valid;
}


/* update_site_cache_contents
 *
 * writes a site name and access mode to the site cache, marks the cache valid.
 */
PRIVATE void update_site_cache_contents
#ifndef __STDC__
    ( cache_p, site, access ) 
    ek_site_cache_t   *cache_p;  /* [out]  */
    ek_site_name_t    site;      /* [in] */
    ek_site_access_t  access;    /* [in] */
#else
  (
    ek_site_cache_t   *cache_p,  /* [out]  */
    ek_site_name_t    site,      /* [in] */
    ek_site_access_t  access
  )
#endif
{
        strcpy((char *) cache_p->site, (char *) site);
        cache_p->access_type = access;
        cache_p->valid = true;
}


/* ek_site_open_by_name
 * 
 * Attempts to bind to the specified site.  If successful, returns the access
 * type in the access parameter, and error_status_ok.  Unlike the registry
 * client agent, does not bind to an arbitrary site if the site parameter is ""
 * or the len is zero (unless we're bound to a passwd etc 1.0 client, which
 * doesn't have the new site binding functionality).
 */
error_status_t ek_site_open_by_name
#ifndef __STDC__
    ( site, access_type_p ) 
    ek_site_name_t    site;             /* [in] */
    ek_site_access_t  *access_type_p;   /* [out] */
#else
  (
    ek_site_name_t    site,             /* [in] */
    ek_site_access_t  *access_type_p
  )
#endif
{
    unsigned_char_p_t lsite_name;
    error_status_t    st, lst;
    sec_rgy_bind_auth_info_t auth_info;
    sec_rgy_handle_t  new_context;

    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level = rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;

    SET_STATUS(&st, error_status_ok);

    if (strlen((char *)site) == 0 || *site == '\0') {
        SET_STATUS(&st, ek_misc_object_not_found);
    } 
        
    if (GOOD_STATUS(&st)) {
        sec_rgy_site_bind(site, &auth_info, &new_context, &st);
        if (STATUS_EQUAL(&st, sec_rgy_cant_authenticate) ||
            STATUS_EQUAL(&st, sec_login_s_no_current_context) ||
            STATUS_EQUAL(&st, rpc_s_auth_tkt_expired)) {
            auth_info.info_type = sec_rgy_bind_auth_none;
            sec_rgy_site_bind(site, &auth_info, &new_context, &st);
            if (GOOD_STATUS(&st)) {
                SET_STATUS(&st, ek_misc_unauth_binding);
            }
        }
        if ((BAD_STATUS(&st)) && (!STATUS_EQUAL(&st,ek_misc_unauth_binding))) {
            if (STATUS_EQUAL(&st, sec_rgy_read_only)) {
                SET_STATUS(&st, error_status_ok);
                *access_type_p = ek_site_query;
            }
        } else 
            *access_type_p = sec_rgy_site_is_readonly(new_context) ?
			ek_site_query : ek_site_update;
    }

    if ((GOOD_STATUS(&st)) || (STATUS_EQUAL(&st, ek_misc_unauth_binding)) ) {
	if (context)
	    ek_site_close();
	context = new_context;
        /* now do a site_get so we're sure to have the full name */
        sec_rgy_site_get(context, &lsite_name, &lst);
        if (GOOD_STATUS(&lst)) {
            strncpy((char *) site, (char *)lsite_name, sec_rgy_name_max_len);
            free(lsite_name);
            update_site_cache_contents(&Site_cache, site, *access_type_p);
        }
    } else
        eku_translate_status(&st);

    return st;
}


/* ek_site_open_by_access
 * attempts to open a site with the access type specified in the access_type_p 
 * parameter.   The access_type_p parameter carries the access type of the 
 * site back out.
 *
 * returns: 
 *  ek_misc_read_only       - if an update site was requested but a query site
 *                             is bound to instead.
 *  ek_misc_no_update_site  - if the caller requested an update site, but no
 *                             update site was available.  the previous binding
 *                             (if any) is still in effect
 *  error_status_ok         - if caller requested and got an update site, or
 *                             caller requested a query site and we got any
 *                             site at all.
 */
error_status_t ek_site_open_by_access
#ifndef __STDC__
    ( site, access_type_p ) 
    ek_site_name_t    site;            /* [out] */
    ek_site_access_t  *access_type_p;  /* [in, out]  */
#else
  (
    ek_site_name_t    site,            /* [out] */
    ek_site_access_t  *access_type_p
  )
#endif
{
    error_status_t    st, lst;
    unsigned_char_p_t lsite;
    sec_rgy_bind_auth_info_t auth_info;
    sec_rgy_handle_t  new_context;

    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level= rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;

    SET_STATUS(&st, error_status_ok);

    /* we use the site_open flavor calls because they bind both queries
     * and updates to a single site (the site_get flavor calls don't).
     * Unfortunately, the _open_ calls don't return the site name when
     * selecting a random site, so we have to make a site_get call  to
     * get the site name to pass back to our caller.
     */

       /* set up site name to bind to an arbitrary site */
    strcpy((char *)site, "");

    switch (*access_type_p) {

    case ek_site_query:
        sec_rgy_site_bind_query(site, &auth_info, &new_context, &st);
        if (STATUS_EQUAL(&st, sec_rgy_cant_authenticate) ||
            STATUS_EQUAL(&st, sec_login_s_no_current_context) ||
            STATUS_EQUAL(&st, rpc_s_auth_tkt_expired)) {
            auth_info.info_type = sec_rgy_bind_auth_none;
            sec_rgy_site_bind_query(site, &auth_info, &new_context, &st);
            if (GOOD_STATUS(&st)) {
                SET_STATUS(&st, ek_misc_unauth_binding);
            }
        }
        break;
        
    case ek_site_update:
        sec_rgy_site_bind_update(site, &auth_info, &new_context, &st);
        if (STATUS_EQUAL(&st, sec_rgy_cant_authenticate) ||
            STATUS_EQUAL(&st, sec_login_s_no_current_context) ||
            STATUS_EQUAL(&st, rpc_s_auth_tkt_expired)) {
            auth_info.info_type = sec_rgy_bind_auth_none;
            sec_rgy_site_bind_update(site, &auth_info, &new_context, &st);
            if (GOOD_STATUS(&st)) {
                SET_STATUS(&st, ek_misc_unauth_binding);
            }
        }
        break;
        
    default:
        SET_STATUS(&st, ek_kernel_failure); 
        break;
    }        
        
    if (GOOD_STATUS(&st) || STATUS_EQUAL(&st, sec_rgy_read_only) ||
                            STATUS_EQUAL(&st, ek_misc_unauth_binding)) {
	if (context)
	    ek_site_close();
	context = new_context;
        sec_rgy_site_get(context, &lsite, &lst); /* what site have we opened */
        if (GOOD_STATUS(&lst)) {
            strncpy((char *) site, (char *)lsite, sec_rgy_name_max_len);
            free(lsite);

            /* we're willing to give them an update site even if they only
             * requested a query site.
             */
            if ((*access_type_p == ek_site_query) &&
                (! sec_rgy_site_is_readonly(context)))
                 *access_type_p = ek_site_update;
            update_site_cache_contents(&Site_cache, site, *access_type_p);
        }

    } else if (STATUS_EQUAL(&st, sec_rgy_server_unavailable) &&
               *access_type_p == ek_site_update)
        SET_STATUS(&st, ek_misc_no_update_site);
                
    eku_translate_status(&st);
    return st;
}
            
            
/* ek_site_get_current
 * 
 * Returns the current binding, or ek_misc_no_site if the kernel is not
 * currently bound to anything.  It doesn't ping the server, so the caller
 * need not worry about communications failures.
 */
error_status_t ek_site_get_current
#ifndef __STDC__
    ( site, access_type_p ) 
    ek_site_name_t    site;           /* [out] */
    ek_site_access_t  *access_type_p; /* [out] */
#else
  (
    ek_site_name_t    site,           /* [out] */
    ek_site_access_t  *access_type_p
  )
#endif
{    
    error_status_t     st;
    boolean32          valid;
    unsigned_char_p_t  tmp_site_name;

    SET_STATUS(&st, error_status_ok);

    if (context == NULL) {
        SET_STATUS(&st, ek_misc_no_site);
    } else {
        sec_rgy_site_get(context, &tmp_site_name, &st);
        if (GOOD_STATUS(&st)) {
            strncpy(site, (char *)tmp_site_name, (sizeof(ek_site_name_t)-1));
            free((char *) tmp_site_name);
        }
    }

    return st;
}


/* ek_site_ping
 * 
 * Returns the current site, or ek_misc_no_site if the kernel is not currently
 * bound to a site.  Also opens the current to site to determine if it's
 * reachable.
 *
 * returns
 *    ek_misc_no_site            - if the kernel has never been bound to a site
 *    ek_misc_server_unavailable - if the current site can't be reached 
 */
error_status_t ek_site_ping
#ifndef __STDC__
    ( site, access_type_p ) 
    ek_site_name_t    site;           /* [out] */
    ek_site_access_t  *access_type_p; /* [out] */
#else
  (
    ek_site_name_t    site,           /* [out] */
    ek_site_access_t  *access_type_p
  )
#endif
{    
    error_status_t          st;
    boolean32          valid;
    ek_site_access_t  current_access;

    SET_STATUS(&st, error_status_ok);
    return st;

    get_site_cache_contents(&Site_cache, site, access_type_p,
                            &valid);
    if (!valid)
        SET_STATUS(&st, ek_misc_no_site);
    else {
        sec_rgy_site_open(site, &context, &st);
        if (STATUS_EQUAL(&st, sec_rgy_read_only)) {
            SET_STATUS(&st, error_status_ok);
            current_access = ek_site_query;
        } else if (GOOD_STATUS(&st)) {
            current_access = sec_rgy_site_is_readonly(context) ?
			ek_site_query : ek_site_update;
        } else if (BAD_STATUS(&st)) 
            SET_STATUS(&st, ek_misc_server_unavailable);
         
            /* if someone's changed the access on the current site since the
             * last time we pinged it, then update the cache
             */
        if (*access_type_p != current_access) {
            *access_type_p = current_access;
            update_site_cache_contents(&Site_cache, site, *access_type_p);
        }
    }
    return st;
}


/* ek_site_close
 * 
 * Closes the current site. Returns error_status_ok even if there is no current
 * binding (why should we care if the caller attempts to close something that
 * isn't open)
 */
error_status_t ek_site_close(
#ifdef __STDC__
    void
#endif
)
{
    error_status_t  st;

    if (SITE_CACHE_VALID(Site_cache)) {
        sec_rgy_site_close(context, &st);
	SITE_CACHE_INVALIDATE(Site_cache);
    }
    else 
        SET_STATUS(&st, error_status_ok);

    eku_translate_status(&st); 
    return st;
}
