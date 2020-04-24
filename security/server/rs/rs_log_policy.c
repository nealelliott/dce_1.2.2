/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_policy.c,v $
 * Revision 1.1.5.2  1996/02/18  00:18:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:58  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:52:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:37  root]
 * 
 * Revision 1.1.3.4  1993/03/11  15:13:30  emartin
 * 	7469 partial fix.  rs_log_policy_prop_policy - null-terminate org_name
 * 	string.
 * 	[1993/03/11  15:11:21  emartin]
 * 
 * Revision 1.1.3.3  1992/12/29  16:35:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:24:02  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:33:40  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:16:56  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  17:00:32  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1  1992/01/19  14:43:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - Policy Log operations
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_log_policy.c,v 1.1.5.2 1996/02/18 00:18:11 marty Exp $";
#endif
         
#include <rs_base.h>
#include <rs_log.h>

#include <rrs_prop_plcy.h>
#include <rs_log_policy.h>
#include <rs_log_util.h>
#include <rs_lock.h>
#include <rsdb_policy.h>

#include <bstring.h>
#include <un_strng.h>

/*
* Local types
*/

typedef struct rs_log_policy_props_t {
    rs_log_hdr_t                upd_hdr;
    signed32                    read_version;
    signed32                    write_version;
    sec_timeval_period_t        min_lifetime;
    sec_timeval_period_t        default_lifetime;
    unsigned32                  low_id_person;
    unsigned32                  low_id_group;
    unsigned32                  low_id_org;
    unsigned32                  max_id;
    signed32                    unauth_quota;
    sec_rgy_properties_flags_t  flags;
    uuid_t                      realm_uuid;
    unsigned32                  realm_name_len;
    char                        buf[sec_rgy_name_t_size];
} rs_log_policy_props_t;

typedef struct rs_log_policy_policy_t {
    rs_log_hdr_t         upd_hdr;
    sec_rgy_plcy_t       policy;
    unsigned32           name_len;
    sec_rgy_name_t       name;
} rs_log_policy_policy_t;

typedef struct rs_log_policy_auth_policy_t {
    rs_log_hdr_t         upd_hdr;
    sec_rgy_plcy_auth_t  auth_policy;
    unsigned32           person_name_len;
    unsigned32           group_name_len;
    unsigned32           org_name_len;
    char                 buf[3 * sizeof(sec_rgy_name_t)];
} rs_log_policy_auth_policy_t;

typedef struct rs_log_policy_override_t {
    rs_log_hdr_t                  upd_hdr;
    rs_policy_override_flags_t    flags;
    unsigned32                    name_len;
    sec_rgy_name_t                name;
} rs_log_policy_override_t;


/*
* PRIVATE local routines
*/
PRIVATE PROP_FUNCTION(rs_log_policy_prop_props);
PRIVATE PROP_FUNCTION(rs_log_policy_prop_policy);
PRIVATE PROP_FUNCTION(rs_log_policy_prop_override);
PRIVATE PROP_FUNCTION(rs_log_policy_prop_auth_policy);

PRIVATE REPLAY_FUNCTION(rs_log_policy_replay_props);
PRIVATE REPLAY_FUNCTION(rs_log_policy_replay_policy);
PRIVATE REPLAY_FUNCTION(rs_log_policy_replay_override);
PRIVATE REPLAY_FUNCTION(rs_log_policy_replay_auth_policy);


PRIVATE void export_auth_policy_item (
#ifdef __STDC__
    rs_log_policy_auth_policy_t  *policy_item,
    sec_rgy_login_name_t         *account_name,
    sec_rgy_plcy_auth_t          *auth_policy
#endif
);


/*
* END OF PRIVATE ROUTINES
*/


/*
* Define Module opcodes and EPV
*/
#define rs_log_policy_props_op       0
#define rs_log_policy_policy_op      1
#define rs_log_policy_override_op    2
#define rs_log_policy_auth_policy_op 3

#define rs_log_policy_last         4 /* Must be equal to the number
                    of operations */

static rs_log_ops_t rs_log_policy_epv[rs_log_policy_last] = {
    { GLOBAL_UPDATE, rs_log_policy_prop_props,
      rs_log_policy_replay_props, "set properties" } ,
    { GLOBAL_UPDATE, rs_log_policy_prop_policy,
      rs_log_policy_replay_policy, "set policy" } ,
    { GLOBAL_UPDATE, rs_log_policy_prop_override,
      rs_log_policy_replay_override, "set override" } ,
    { GLOBAL_UPDATE, rs_log_policy_prop_auth_policy,
      rs_log_policy_replay_auth_policy, "set auth policy" } ,
};



PUBLIC void rs_log_policy_init
#ifndef __STDC__
    ( st )
    error_status_t  *st;
#else
  (
    error_status_t  *st
  )
#endif
{
    int         num_ops;

    num_ops = rs_log_policy_last;
    rs_log_init_module(RS_LOG_MODULE_PROPS, num_ops, rs_log_policy_epv, st);
}


PRIVATE void import_properties
#ifndef __STDC__
    ( properties, log_props, size )
    sec_rgy_properties_t   *(properties);   /* [in] */
    rs_log_policy_props_t  *log_props;      /* [out] */
    long                   *size;           /* [out] */
#else
  (
    sec_rgy_properties_t   *(properties),   /* [in] */
    rs_log_policy_props_t  *log_props,      /* [out] */
    long                   *size
  )
#endif
{
    /* fixed poortion */
    log_props->read_version     = properties->read_version;
    log_props->write_version    = properties->write_version;
    log_props->min_lifetime     = properties->minimum_ticket_lifetime;
    log_props->default_lifetime = properties->default_certificate_lifetime;
    log_props->flags            = properties->flags;
    log_props->realm_uuid       = properties->realm_uuid;
    log_props->low_id_person    = properties->low_unix_id_person;
    log_props->low_id_group     = properties->low_unix_id_group;
    log_props->low_id_org       = properties->low_unix_id_org;
    log_props->max_id           = properties->max_unix_id;
    log_props->unauth_quota     = properties->unauthenticated_quota;

    /* variable portion */
    log_props->realm_name_len = strlen( (char *) properties->realm);
    strncpy(log_props->buf, (char *) properties->realm,
        log_props->realm_name_len);

    *size = sizeof(*log_props) - sizeof(log_props->buf)
    + log_props->realm_name_len;
}

PRIVATE void export_properties
#ifndef __STDC__
    ( log_props, properties )
    rs_log_policy_props_t  *log_props;      /* [in] */
    sec_rgy_properties_t   *(properties);   /* [out] */
#else
  (
    rs_log_policy_props_t  *log_props,      /* [in] */
    sec_rgy_properties_t   *(properties)
  )
#endif
{
    /* fixed poortion */
    properties->read_version                = log_props->read_version;
    properties->write_version               = log_props->write_version;
    properties->minimum_ticket_lifetime     = log_props->min_lifetime;
    properties->default_certificate_lifetime = log_props->default_lifetime;
    properties->flags                       = log_props->flags;
    properties->realm_uuid                  = log_props->realm_uuid;
    properties->low_unix_id_person          = log_props->low_id_person;
    properties->low_unix_id_group           = log_props->low_id_group;
    properties->low_unix_id_org             = log_props->low_id_org;
    properties->max_unix_id                 = log_props->max_id;
    properties->unauthenticated_quota       = log_props->unauth_quota;

    /* variable portion */
    strncpy( (char *) properties->realm, log_props->buf,
        log_props->realm_name_len);
    properties->realm[log_props->realm_name_len] = '\0';

}


PUBLIC void rs_log_policy_props
#ifndef __STDC__
    ( properties, master_info )
    sec_rgy_properties_t       *(properties);
    rs_replica_master_info_t   *master_info;
#else
  (
    sec_rgy_properties_t       *(properties),
    rs_replica_master_info_t   *master_info
  )
#endif
{
    long                    size;
    rs_log_policy_props_t   info;


    import_properties(properties, &info, &size);
    rs_log_setup_log_header(&info.upd_hdr, size, RS_LOG_MODULE_PROPS,
                rs_log_policy_props_op);

    if (master_info != NULL) {
        rs_log_write(&info.upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&info.upd_hdr, NULL, NULL);
    }
}


PRIVATE void rs_log_policy_prop_props
#ifndef __STDC__
    ( handle, info, master_info, propq_only, st )
    handle_t                    handle;
    rs_log_rec_t                *info;
    rs_replica_master_info_t    *master_info;
    boolean32                   propq_only;
    error_status_t              *st;
#else
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
#endif
{
    rs_log_policy_props_t       *log_info = (rs_log_policy_props_t *) info;
    sec_rgy_properties_t        properties;

    export_properties(log_info, &properties);
    rrs_prop_properties_set_info(handle, &properties, 
                                 master_info, propq_only, st);
}

PRIVATE void rs_log_policy_replay_props
#ifndef __STDC__
    ( info, st )
    rs_log_rec_t    *info;
    error_status_t  *st;
#else
  (
    rs_log_rec_t    *info,
    error_status_t  *st
  )
#endif
{
    rs_log_policy_props_t   *log_info = (rs_log_policy_props_t *) info;
    sec_rgy_properties_t    properties;

    CLEAR_STATUS(st);
    export_properties(log_info, &properties);
    rsdb_properties_set_info(&properties, st);
}

PUBLIC void rs_log_policy_policy
#ifndef __STDC__
    ( org_name, policy, master_info )
    sec_rgy_name_t              org_name;
    sec_rgy_plcy_t              *policy;
    rs_replica_master_info_t    *master_info;
#else
  (
    sec_rgy_name_t              org_name,
    sec_rgy_plcy_t              *policy,
    rs_replica_master_info_t    *master_info
  )
#endif
{
    long                    size;
    rs_log_policy_policy_t  info;

    info.policy     = *policy;
    info.name_len   =  strlen( (char *) org_name);
    if (info.name_len > 0) {
        strncpy( (char *)info.name, (char *) org_name, info.name_len);
    }
    size = sizeof(info) + info.name_len - sizeof(info.name);
    rs_log_setup_log_header(&info.upd_hdr, size, RS_LOG_MODULE_PROPS,
                rs_log_policy_policy_op);

    if (master_info != NULL) {
        rs_log_write(&info.upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&info.upd_hdr, NULL, NULL);
    }
}

PRIVATE void rs_log_policy_prop_policy
#ifndef __STDC__
    ( handle, info, master_info, propq_only, st )
    handle_t                    handle;
    rs_log_rec_t                *info;
    rs_replica_master_info_t    *master_info;
    boolean32                   propq_only;
    error_status_t              *st;
#else
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
#endif
{
    rs_log_policy_policy_t      *log_info = (rs_log_policy_policy_t *) info;
    sec_rgy_plcy_t              policy;
    sec_rgy_name_t              org_name;

    bcopy( (Pointer) &log_info->policy, (Pointer) &policy, sizeof(policy));

    if (log_info->name_len > 0) {
        strncpy( (char *)org_name, (char *) log_info->name, log_info->name_len);
    } 
    org_name[log_info->name_len] = '\0';

    rrs_prop_plcy_set_info(handle, org_name, &policy, 
                           master_info, propq_only, st);
}

PRIVATE void rs_log_policy_replay_policy
#ifndef __STDC__
    ( info, st )
    rs_log_rec_t    *info;
    error_status_t  *st;
#else
  (
    rs_log_rec_t    *info,
    error_status_t  *st
  )
#endif
{
    rs_log_policy_policy_t  *log_info = (rs_log_policy_policy_t *) info;
    sec_rgy_plcy_t          policy;
    sec_rgy_name_t          org_name;

    CLEAR_STATUS(st);

    bcopy( (Pointer) &log_info->policy, (Pointer) &policy, sizeof(policy));
    strncpy( (char *)org_name, (char *) log_info->name, log_info->name_len);
    org_name[log_info->name_len] = '\0';

    rsdb_policy_set_info(org_name, &policy, st);
}


PRIVATE void export_auth_policy_item
#ifndef __STDC__
    ( policy_item, account_name, auth_policy )
    rs_log_policy_auth_policy_t *policy_item;
    sec_rgy_login_name_t        *account_name;
    sec_rgy_plcy_auth_t         *auth_policy;
#else
  (
    rs_log_policy_auth_policy_t *policy_item,
    sec_rgy_login_name_t        *account_name,
    sec_rgy_plcy_auth_t         *auth_policy
  )
#endif
{
    *auth_policy  = policy_item->auth_policy;

    rs_log_export_acct_name(account_name,
             policy_item->person_name_len,
             policy_item->group_name_len,
             policy_item->org_name_len,
             policy_item->buf);
}

PUBLIC void rs_log_policy_auth_policy
#ifndef __STDC__
    ( account, auth_policy, master_info )
    sec_rgy_login_name_t        *account;
    sec_rgy_plcy_auth_t         *auth_policy;
    rs_replica_master_info_t    *master_info;
#else
  (
    sec_rgy_login_name_t        *account,
    sec_rgy_plcy_auth_t         *auth_policy,
    rs_replica_master_info_t    *master_info
  )
#endif
{
    long                        size = 0;
    rs_log_policy_auth_policy_t info;

    info.auth_policy = *auth_policy;

    rs_log_import_acct_name(account, &(info.person_name_len),
             &(info.group_name_len), &(info.org_name_len),
             info.buf, &size);

    size = size + sizeof(info) - sizeof(info.buf);
    rs_log_setup_log_header(&info.upd_hdr, size, RS_LOG_MODULE_PROPS,
                rs_log_policy_auth_policy_op);

    if (master_info != NULL) {
        rs_log_write(&info.upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&info.upd_hdr, NULL, NULL);
    }
}

PRIVATE void rs_log_policy_prop_auth_policy
#ifndef __STDC__
    ( handle, info, master_info, propq_only, st )
    handle_t                    handle;
    rs_log_rec_t                *info;
    rs_replica_master_info_t    *master_info;
    boolean32                   propq_only;
    error_status_t              *st;
#else
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
#endif
{
    sec_rgy_login_name_t        account;
    sec_rgy_plcy_auth_t         auth_policy;
    rs_log_policy_auth_policy_t *log_info
    = (rs_log_policy_auth_policy_t *) info;

    export_auth_policy_item(log_info, &account, &auth_policy);

    rrs_prop_auth_plcy_set_info(handle, &account, &auth_policy, 
                                master_info, propq_only, st);
}

PRIVATE void rs_log_policy_replay_auth_policy
#ifndef __STDC__
    ( info, st )
    rs_log_rec_t    *info;
    error_status_t  *st;
#else
  (
    rs_log_rec_t    *info,
    error_status_t  *st
  )
#endif
{
    sec_rgy_login_name_t         account;
    sec_rgy_plcy_auth_t          auth_policy;
    rs_log_policy_auth_policy_t  *log_info
    = (rs_log_policy_auth_policy_t *) info;

    export_auth_policy_item(log_info, &account, &auth_policy);

    CLEAR_STATUS(st);
    rsdb_auth_policy_set_info(&account, &auth_policy, st);
}


/* 
 * The override info logging routines are obsolete, so we 
 * no-op them to serve as benign placeholders in the 
 * log epv
 */
PUBLIC  void  rs_log_policy_override
#ifndef __STDC__
    ( name, p_d, master_info )
    sec_rgy_name_t              name;
    rs_policy_override_flags_t  p_d;
    rs_replica_master_info_t    *master_info;
#else
  (
    sec_rgy_name_t              name,
    rs_policy_override_flags_t  p_d,
    rs_replica_master_info_t    *master_info
  )
#endif
{
}

PRIVATE void rs_log_policy_prop_override
#ifndef __STDC__
    ( handle, info, master_info, propq_only, st )
    handle_t                    handle;
    rs_log_rec_t                *info;
    rs_replica_master_info_t    *master_info;
    boolean32                   propq_only;
    error_status_t              *st;
#else
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
#endif
{
   CLEAR_STATUS(st);
}

PRIVATE void rs_log_policy_replay_override
#ifndef __STDC__
    ( info, st )
    rs_log_rec_t    *info;
    error_status_t  *st;
#else
  (
    rs_log_rec_t    *info,
    error_status_t  *st
  )
#endif
{
    CLEAR_STATUS(st);
}
