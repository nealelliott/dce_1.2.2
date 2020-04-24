/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_create.c,v $
 * Revision 1.1.10.2  1996/02/18  00:21:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:09  marty]
 *
 * Revision 1.1.10.1  1995/12/08  17:59:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:13  root]
 * 
 * Revision 1.1.7.3  1994/08/04  16:14:38  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:13:35  mdf]
 * 
 * Revision 1.1.2.5  1992/12/29  16:38:30  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:09  zeliff]
 * 
 * Revision 1.1.2.4  1992/11/20  15:54:41  ahop
 * 	Move init_database to rsdb.c; remove rs_mkey_create from create_database
 * 	[1992/11/16  21:12:08  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:38:09  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:20:24  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  16:03:53  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:28:08  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:12:40  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* 
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *       Registry Server - db interlude - create a new database
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rsdb_create.c,v 1.1.10.2 1996/02/18 00:21:17 marty Exp $";
#endif

#include <dce/dce.h>
#include <dce/rgynbase.h>
#include <dce/nbase.h>

#include <rs_base.h>
#include <rs_util.h>
#include <rsdb_pvt.h>
#include <rsdb_pgo.h>
#include <rsdb_acct.h>
#include <rsdb_policy.h>
#include <rsdb_acl.h>
#include <rsdb_create.h>
#include <rsdb_auth.h>
#include <rsdb_attr_schema.h>
#include <sec_svc.h>

PRIVATE void create_rgy
#ifndef __STDC__
    (rgy_creator, rgy_local_cell, rgy_myname, default_properties)
    sec_id_t                *rgy_creator;
    sec_id_t                *rgy_local_cell;
    unsigned_char_p_t       rgy_myname;
    sec_rgy_properties_t    *default_properties;
#else
    (
    sec_id_t                *rgy_creator,
    sec_id_t                *rgy_local_cell,
    unsigned_char_p_t       rgy_myname,
    sec_rgy_properties_t    *default_properties
    )
#endif
{
    error_status_t          status;
    sec_rgy_plcy_t          default_policy;
    sec_rgy_plcy_auth_t     default_auth_policy;
    sec_rgy_login_name_t    rgy_auth_policy_key;
    sec_rgy_name_t          *rname;

    rsdb_init_rgy_info(rgy_creator, rgy_local_cell, rgy_myname);

    rsdb_properties_set_info(default_properties, &status);
    if (BAD_STATUS(&status)) {
        dce_svc_printf(SEC_RSDB_CANT_SET_PROPERTIES_MSG);
    }

   /* Set local cell */
    rname = (sec_rgy_name_t *)(rgy_local_cell->name);
    rsdb_properties_set_realm(*rname, &rgy_local_cell->uuid, &status);
    if (BAD_STATUS(&status)) {
        dce_svc_printf(SEC_RSDB_CANT_SET_REALM_MSG);
    }

    default_policy.passwd_min_len       = RSDB_DEF_POL_PASSWD_MIN_LEN;
    default_policy.passwd_lifetime      = RSDB_DEF_POL_PASSWD_LIFE;
    default_policy.passwd_exp_date      = RSDB_DEF_POL_PASSWD_EXP_DATE;
    default_policy.passwd_flags         = RSDB_DEF_POL_PASSWD_FLAGS;
    default_policy.acct_lifespan        = RSDB_DEF_POL_ACCT_LIFE;

    rsdb_policy_set_info( (unsigned_char_p_t) "", &default_policy, &status);
    if (BAD_STATUS(&status)) {
        dce_svc_printf(SEC_RSDB_CANT_SET_POLICY_MSG);
    }

    rgy_auth_policy_key.pname[0] = '\0';
    rgy_auth_policy_key.gname[0] = '\0';
    rgy_auth_policy_key.oname[0] = '\0';

    default_auth_policy.max_ticket_lifetime  = RSDB_DEF_AUTH_POL_MAX_TKT_LIFE;
    default_auth_policy.max_renewable_lifetime  = RSDB_DEF_AUTH_POL_MAX_RENEW_LIFE;


    rsdb_auth_policy_set_info(&rgy_auth_policy_key, &default_auth_policy,
				&status);
    if (BAD_STATUS(&status)) {
        dce_svc_printf(SEC_RSDB_CANT_SET_AUTH_POLICY_MSG);
    }
}

PUBLIC void rsdb_create_set_default_properties
#ifndef __STDC__ 
    (default_properties)
    sec_rgy_properties_t    *default_properties;
#else
    (
    sec_rgy_properties_t    *default_properties
    )
#endif
{
    default_properties->read_version     = RSDB_DEF_PROP_READ_VERSION;
    default_properties->write_version    = RSDB_DEF_PROP_WRITE_VERSION;
    default_properties->minimum_ticket_lifetime = RSDB_DEF_PROP_MIN_TKT_LIFE;
    default_properties->default_certificate_lifetime = RSDB_DEF_PROP_DEF_CERT_LIFE;

    /* The low_unix_ids are settable in rgy_create */
    default_properties->low_unix_id_person = RSDB_DEF_PROP_LOW_UNIX_ID_PERSON;
    default_properties->low_unix_id_group  = RSDB_DEF_PROP_LOW_UNIX_ID_GROUP;
    default_properties->low_unix_id_org    = RSDB_DEF_PROP_LOW_UNIX_ID_ORG;
    default_properties->max_unix_id        = RSDB_DEF_PROP_MAX_UNIX_ID;
    default_properties->flags              = RSDB_DEF_PROP_FLAGS;
    *default_properties->realm             = '\0';
    default_properties->unauthenticated_quota = RSDB_DEF_PROP_UNAUTH_QUOTA;
}

PUBLIC void rsdb_create_database 
#ifndef __STDC__
    (rgy_creator, rgy_local_cell, rgy_myname, default_properties )
    sec_id_t                *rgy_creator;
    sec_id_t                *rgy_local_cell;
    unsigned_char_p_t       rgy_myname;
    sec_rgy_properties_t    *default_properties;
#else
    (
    sec_id_t                *rgy_creator,
    sec_id_t                *rgy_local_cell,
    unsigned_char_p_t       rgy_myname,
    sec_rgy_properties_t    *default_properties
    )
#endif
{
    rsdb_acl_create();
    create_rgy(rgy_creator, rgy_local_cell, 
        rgy_myname, default_properties);
    rsdb_pgo_create(rgy_creator, rgy_local_cell, default_properties);
    rsdb_acct_create();
    rsdb_attr_sch_create();
    rsdb_login_create();
    rsdb_journal_create();

}

