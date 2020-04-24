/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: policy.c,v $
 * Revision 1.1.8.1  1996/10/03  14:46:52  arvind
 * 	Mark as volatile variables which are live across setjmp
 * 	[1996/09/16  20:30 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.6.2  1996/03/11  13:28:09  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:55  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  18:07:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:32:16  root]
 * 
 * Revision 1.1.2.3  1993/02/05  22:44:51  sommerfeld
 * 	[OT5061] Deal with change to CHECK_CACHE macro.
 * 	[1993/02/04  22:32:06  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/12/29  13:03:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:38  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:47:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  policy.c V=17 04/24/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      Registry Client Agent Interface - Properties and Policy management
** 
** Copyright Hewlett-Packard Company 1989, 1990
** Copyright Apollo Computer Inc. 1987, 1988, 1989
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)policy.c	17 - 04/24/91";
#endif

#include <stdio.h>

#include <rca_pvt.h>
#include <rgymacro.h>

#include <rs_plcy.h>
#include <policy.h>

#include <sysdep.h>

#include <un_strng.h>

#define Crs_properties_get_info(CH,b,CI,c)\
    (*rs_policy_v1_0_c_epv.rs_properties_get_info) (HANDLE(CH),b,CI,c)

#define Crs_properties_set_info(CH,b,CI,c)\
    (*rs_policy_v1_0_c_epv.rs_properties_set_info) (HANDLE(CH),b,CI,c)

#define Crs_policy_get_info(CH,b,c,CI,d)\
    (*rs_policy_v1_0_c_epv.rs_policy_get_info) (HANDLE(CH),b,c,CI,d)

#define Crs_policy_set_info(CH,b,c,CI,d)\
    (*rs_policy_v1_0_c_epv.rs_policy_set_info) (HANDLE(CH),b,c,CI,d)

#define Crs_policy_get_effective(CH,b,c,CI,d)\
    (*rs_policy_v1_0_c_epv.rs_policy_get_effective) (HANDLE(CH),b,c,CI,d)

#define Crs_auth_policy_get_info(CH,b,c,CI,d)\
    (*rs_policy_v1_0_c_epv.rs_auth_policy_get_info) (HANDLE(CH),b,c,CI,d)

#define Crs_auth_policy_set_info(CH,b,c,CI,d)\
    (*rs_policy_v1_0_c_epv.rs_auth_policy_set_info) (HANDLE(CH),b,c,CI,d)

#define Crs_auth_policy_get_effective(CH,b,c,CI,d)\
    (*rs_policy_v1_0_c_epv.rs_auth_policy_get_effective) (HANDLE(CH),b,c,CI,d)

#define Crs_policy_get_override_info(CH,b,c,CI,d)\
    (*rs_policy_v1_0_c_epv.rs_policy_get_override_info) (HANDLE(CH),b,c,CI,d)

#define Crs_policy_set_override_info(CH,b,c,CI,d)\
    (*rs_policy_v1_0_c_epv.rs_policy_set_override_info) (HANDLE(CH),b,c,CI,d)

#ifdef GLOBAL_LIBRARY
#   include <set_sect_pvt.c>
#endif

PUBLIC void sec_rgy_properties_get_info
#ifndef __STDC__
    ( context, properties, status ) 
    sec_rgy_handle_t        context;        /* [in] */
    sec_rgy_properties_t    *properties;    /* [out] */
    error_status_t          *status;        /* [out] */
#else
  (
    sec_rgy_handle_t        context,        /* [in] */
    sec_rgy_properties_t    *properties,    /* [out] */
    error_status_t          *status
  )
#endif
{
    rs_cache_data_t        cache_info;

    SETUP_RETRY(rca_op_read, status) {
        Crs_properties_get_info(context, properties, &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_properties_set_info
#ifndef __STDC__
    ( context, properties, status ) 
    sec_rgy_handle_t        context;        /* [in] */
    sec_rgy_properties_t    *properties;    /* [in] */
    error_status_t          *status;        /* [out] */
#else
  (
    sec_rgy_handle_t        context,        /* [in] */
    sec_rgy_properties_t    *properties,    /* [in] */
    error_status_t          *status
  )
#endif
{
    rs_cache_data_t        cache_info;

    SETUP_RETRY(rca_op_write, status) {
        Crs_properties_set_info(context, properties, &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}


PUBLIC void sec_rgy_plcy_get_info
#ifndef __STDC__
    ( context, organization, policy_data, status) 
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_name_t      organization;   /* [in] */
    sec_rgy_plcy_t      *policy_data;   /* [out] */
    error_status_t      *status;        /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_name_t      organization,   /* [in] */
    sec_rgy_plcy_t      *policy_data,   /* [out] */
    error_status_t      *status
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!CHECK_PGO_NAME(organization)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_read, status) {
        Crs_policy_get_info(context, organization, policy_data, &cache_info,
                            status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

#ifdef DEBUG_POLICY
PRIVATE rca_print_policy
#ifndef __STDC__
    ( org, policy, status ) 
    char              *org;
    sec_rgy_plcy_t    *policy;
    error_status_t    *status;
#else
  (
    char              *org,
    sec_rgy_plcy_t    *policy,
    error_status_t    *status
  )
#endif
{
    printf("    %:\n\
        passwd min len  = %d\n\
        lifetime        = %d\n\
        expiration date = %d\n\
        lifespan        = %d\n\
        flags           = [%s%s] %#x\n", org, policy->passwd_min_len, 
        policy->passwd_lifetime, policy->passwd_exp_date, 
        policy->acct_lifespan, 
        policy->passwd_flags & sec_rgy_plcy_pwd_no_spaces ? " no spaces ": "",
        policy->passwd_flags & sec_rgy_plcy_pwd_non_alpha ? " non alpha ": "",
        policy->passwd_flags);
}
#endif /* DEBUG_POLICY */

PUBLIC void sec_rgy_plcy_set_info
#ifndef __STDC__
    (context, organization, policy_data, status ) 
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_name_t      organization;   /* [in] */
    sec_rgy_plcy_t      *(policy_data); /* [in] */
    error_status_t      *(status);      /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_name_t      organization,   /* [in] */
    sec_rgy_plcy_t      *(policy_data), /* [in] */
    error_status_t      *(status)
  )
#endif
{
    rs_cache_data_t        cache_info;
#ifdef DEBUG_POLICY
    rca_global_info_t   *info;
    long                debug_flags;
#endif /* DEBUG_POLICY */

    if (!CHECK_PGO_NAME(organization)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

#ifdef DEBUG_POLICY
    info = rca_access_global_info(false);
    debug_flags = info->debug_flags;
    rca_release_global_info(info);

    if (debug_flags & DEBUG_POLICY_FLAG) {
        rca_print_policy((char *) organization, policy_data, status);
    }
#endif

    SETUP_RETRY(rca_op_write, status) {
        Crs_policy_set_info(context, organization, policy_data, &cache_info,
                            status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_plcy_get_effective
#ifndef __STDC__
    ( context, organization, policy_data, status )
    sec_rgy_handle_t    context;         /* [in] */
    sec_rgy_name_t      organization;    /* [in] */
    sec_rgy_plcy_t      *(policy_data);  /* [out] */
    error_status_t      *(status);       /* [out] */
#else
  (
    sec_rgy_handle_t    context,         /* [in] */
    sec_rgy_name_t      organization,    /* [in] */
    sec_rgy_plcy_t      *(policy_data),  /* [out] */
    error_status_t      *(status)
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!CHECK_PGO_NAME(organization)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_read, status) {
        Crs_policy_get_effective(context, organization, policy_data,
                                    &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_plcy_get_override_info
#ifndef __STDC__
    ( context, policy_category, x_passwd, root_passwd, other_passwd,
                                                custom_data, status )
    sec_rgy_handle_t    context;            /* [in] */
    sec_rgy_name_t      policy_category;    /* [in ref ] */
    boolean32           *x_passwd;          /* [out] */         
    boolean32           *root_passwd;       /* [out] */         
    boolean32           *other_passwd;      /* [out] */         
    boolean32           *custom_data;       /* [out] */         
    error_status_t      *status;            /* [out] */
#else
  (
    sec_rgy_handle_t    context,            /* [in] */
    sec_rgy_name_t      policy_category,    /* [in ref ] */
    boolean32           *x_passwd,          /* [out] */         
    boolean32           *root_passwd,       /* [out] */         
    boolean32           *other_passwd,      /* [out] */         
    boolean32           *custom_data,       /* [out] */         
    error_status_t      *status
  )
#endif
{
    rs_policy_override_flags_t  policy_data;   
    rs_cache_data_t             cache_info;

    if (!CHECK_PGO_NAME(policy_category)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_read, status) {
        Crs_policy_get_override_info(context, policy_category, &policy_data,
                                        &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;

    /* If we get an error other not_implemented then turn all overrides off.
     * If the servers are unavailable, then we'll eventually get data from the
     * local passwd file which should (may) have correctly overridden entries.
     */
    /*OVERRIDE is the above still true? */
    /*LOCAL */
    if (STATUS_OK(status) && STATUS(status) != sec_rgy_not_implemented) {
        *x_passwd = *root_passwd = *other_passwd = *custom_data = false;
        return;
    }

    /* transform bitset entries into booleans */
    if (policy_data & rs_override_exclude_passwd) {
        *x_passwd = true;
    } else {
        *x_passwd = false;
    }
    if (policy_data & rs_override_root_passwd) {
        *root_passwd = true;
    } else {
        *root_passwd = false;
    }
    
    if (policy_data & rs_override_other_passwd) {
        *other_passwd = true;
    } else {
        *other_passwd = false;
    }
    if (policy_data & rs_override_custom_data) {
        *custom_data = true;
    } else {
        *custom_data = false;
    }

    return;
}

PUBLIC void sec_rgy_plcy_set_override_info
#ifndef __STDC__
    ( context,  policy_category, x_passwd, root_passwd, other_passwd,
                                                custom_data, status )
    sec_rgy_handle_t    context;            /* [in] */
    sec_rgy_name_t      policy_category;    /* [in ref ] */
    boolean32           x_passwd;           /* [in] */         
    boolean32           root_passwd;        /* [in] */         
    boolean32           other_passwd;       /* [in] */         
    boolean32           custom_data;        /* [in] */         
    error_status_t      *status;            /* [out] */
#else
  (
    sec_rgy_handle_t    context,            /* [in] */
    sec_rgy_name_t      policy_category,    /* [in ref ] */
    boolean32           x_passwd,           /* [in] */         
    boolean32           root_passwd,        /* [in] */         
    boolean32           other_passwd,       /* [in] */         
    boolean32           custom_data,        /* [in] */         
    error_status_t      *status
  )
#endif
{
    volatile rs_policy_override_flags_t  policy_data;   
    rs_cache_data_t             cache_info;

    if (!CHECK_PGO_NAME(policy_category)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;                
    }

    /* Transform input booleans into bit set. */
    policy_data = 0;

    if (x_passwd) {
        policy_data |= rs_override_exclude_passwd;
    }

    if (root_passwd) {
        policy_data |= rs_override_root_passwd;
    }

    if (other_passwd) {
        policy_data |= rs_override_other_passwd;
    }
    if (custom_data) {
        policy_data |= rs_override_custom_data;
    }

    SETUP_RETRY(rca_op_write, status) {
        Crs_policy_set_override_info(context, policy_category, policy_data,
                                        &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;

    return;
}
   
PUBLIC void  sec_rgy_auth_plcy_get_info
#ifndef __STDC__
    ( context, account, auth_policy, status ) 
    sec_rgy_handle_t        context;        /* [in] */
    sec_rgy_login_name_t    *account;       /* [in ref] */
    sec_rgy_plcy_auth_t     *auth_policy;   /* [out] */
    error_status_t          *status;        /* [out] */
#else
  (
    sec_rgy_handle_t        context,        /* [in] */
    sec_rgy_login_name_t    *account,       /* [in ref] */
    sec_rgy_plcy_auth_t     *auth_policy,   /* [out] */
    error_status_t          *status
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!CHECK_LOGIN_NAME(account)) { 
        SET_STATUS(status, sec_rgy_bad_data);
	return;
    }

    SETUP_RETRY(rca_op_read, status) {
        Crs_auth_policy_get_info(context, account, auth_policy, &cache_info,
                                    status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;

    return;
}

PUBLIC void  sec_rgy_auth_plcy_set_info
#ifndef __STDC__
    ( context, account, auth_policy, status ) 
    sec_rgy_handle_t        context;        /* [in] */
    sec_rgy_login_name_t    *account;       /* [in ref] */
    sec_rgy_plcy_auth_t     *auth_policy;   /* [in] */
    error_status_t          *status;        /* [out] */
#else
  (
    sec_rgy_handle_t        context,        /* [in] */
    sec_rgy_login_name_t    *account,       /* [in ref] */
    sec_rgy_plcy_auth_t     *auth_policy,   /* [in] */
    error_status_t          *status
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!CHECK_LOGIN_NAME(account)) { 
        SET_STATUS(status, sec_rgy_bad_data);
	return;
    }

    SETUP_RETRY(rca_op_write, status) {
        Crs_auth_policy_set_info(context, account, auth_policy, &cache_info,
                                    status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;

    return;
}

PUBLIC void  sec_rgy_auth_plcy_get_effective
#ifndef __STDC__
    ( context, account, auth_policy, status )
    sec_rgy_handle_t        context;        /* [in] */
    sec_rgy_login_name_t    *account;       /* [in ref] */
    sec_rgy_plcy_auth_t     *auth_policy;   /* [out] */
    error_status_t          *status;        /* [out] */
#else
  (
    sec_rgy_handle_t        context,        /* [in] */
    sec_rgy_login_name_t    *account,       /* [in ref] */
    sec_rgy_plcy_auth_t     *auth_policy,   /* [out] */
    error_status_t          *status
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!CHECK_LOGIN_NAME(account)) { 
        SET_STATUS(status, sec_rgy_bad_data);
	return;
    }

    SETUP_RETRY(rca_op_read, status) {
        Crs_auth_policy_get_effective(context, account, auth_policy,
                                        &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;

    return;
}
