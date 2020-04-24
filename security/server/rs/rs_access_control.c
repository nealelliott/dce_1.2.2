/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_access_control.c,v $
 * Revision 1.1.4.2  1996/02/18  00:17:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:10  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:34:40  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:03  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_access_control.c V=6 04/29/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - Access Control management
 * 
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rs_access_control.c	6 - 04/29/91";
#endif

#include <rs_base.h>
#include <rs_pvt.h>
#include <rsdb_pvt.h>

#include <rs_lock.h>
#include <rs_access_control.h>
#include <rsdb_policy.h> 
#include <rsdb_util.h>

/*AUTH #include <rsdb_access_control.h> */

/*AUTH #include <rca_authenticate.h> */

PUBLIC boolean32 is_registry_owner
#ifndef __STDC__
    ( h, caller )
    handle_t        h;
    sec_rgy_sid_t  *caller;
#else
  (
    handle_t        h,
    sec_rgy_sid_t  *caller
  )
#endif
{

    return true;

#ifdef UNDEF
    rs_auth_key_t          master_key;
    boolean32              retval = false;
    sec_rgy_properties_t   rgy_properties;
    error_status_t         st;

 
    rsdb_properties_get_info(&rgy_properties, &st);
    if (st.all == status_$ok)
        retval = owner_is_wildcard(&rgy_properties.owner);

    if (!retval) {
        /* Should failure to find a master key indicate a different error? */
        if (rsdb_get_master_key(caller, master_key)) {
            if (auth_authenticate(h, master_key)) {
                retval = rsdb_ac_is_registry_owner(h, true, caller);
            }
        }
    }

    return retval;
#endif
}

PUBLIC boolean32 is_domain_owner
#ifndef __STDC__
    (h, domain, caller)
    handle_t            h;
    sec_rgy_domain_t   domain;
    sec_rgy_sid_t      *caller;
#else
  (
    handle_t            h,
    sec_rgy_domain_t   domain,
    sec_rgy_sid_t      *caller
  )
#endif
{

    return true; 

#ifdef UNDEF
    rs_auth_key_t      master_key;
    boolean32          retval = false;
    sec_rgy_sid_t      owner;
    error_status_t     st;


    rsdb_properties_get_domain_owner(domain, &owner, &st);
    if (st.all == status_$ok)
        retval = owner_is_wildcard(&owner);

    if (!retval) {
        if (rsdb_get_master_key(caller, master_key)) {
            if (auth_authenticate(h, master_key)) {
                retval = rsdb_ac_is_domain_owner(h, true, domain, caller);
            }
        }
    }

    return retval;
#endif
}

PRIVATE boolean32 is_owner
#ifndef __STDC__
    (h, caller, owner, user, access_mode)
    handle_t            h;
    sec_rgy_sid_t       *caller;
    sec_rgy_sid_t       *owner;
    uuid_t              *user;
    access_mode_t       access_mode;
#else
  (
    handle_t            h,
    sec_rgy_sid_t       *caller,
    sec_rgy_sid_t       *owner,
    uuid_t              *user,
    access_mode_t       access_mode
  )
#endif
{
    return true;

#ifdef UNDEF
    boolean32       retval;


    retval = owner_is_wildcard(owner);
    if (!retval) {
        if (rsdb_get_master_key(caller, master_key)) {
            if (auth_authenticate(h, master_key)) {
                retval = rsdb_ac_is_owner(h, caller, true, owner, user,
                                                access_mode);
            }
        }
    }

    return retval;
#endif
}

PUBLIC boolean32 is_pgo_authorized
#ifndef __STDC__
    ( h, caller, domain, name, access_mode, status ) 
    handle_t             h;
    sec_rgy_sid_t       *caller;
    sec_rgy_domain_t    domain;     /* [in] */
    sec_rgy_name_t      name;       /* [in] */
    access_mode_t       access_mode;/* [in] */
    error_status_t      *(status);  /* [out] */
#else
  (
    handle_t             h,
    sec_rgy_sid_t       *caller,
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      name,       /* [in] */
    access_mode_t       access_mode,/* [in] */
    error_status_t      *(status)
  )
#endif
{ 
    SET_STATUS(status, error_status_ok);
    return true;

#ifdef UNDEF
    rsdb_pgo_item_t    pgo;


    /* Access Check */
    READ_LOCK(lock_db) {
        if (!get_pgo_by_name(domain, name, &pgo)) {
            status->all = sec_rgy_object_not_found;
        } else {
            if (access_mode == access_mode_user 
                    && (pgo.flags & rgy_pgo_no_user_mod_mask)) {
                access_mode = access_mode_owner;
            }
            if (!is_owner(h, caller, &pgo.owner, &pgo.uid, access_mode)) {
                status->all = sec_rgy_not_authorized;
            }
        }
    } END_READ_LOCK;

    if (status->all == status_$ok)
        return true;

    return false;
#endif

}

