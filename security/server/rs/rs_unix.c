/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_unix.c,v $
 * Revision 1.1.70.2  1996/02/18  00:20:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:24  marty]
 *
 * Revision 1.1.70.1  1995/12/08  17:57:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:44  root]
 * 
 * Revision 1.1.68.1  1993/10/07  13:12:21  mccann
 * 	CR8651 64bit porting changes
 * 	[1993/10/07  13:08:00  mccann]
 * 
 * Revision 1.1.5.4  1993/04/05  16:13:12  burati
 * 	Check cell_prop for hidden passwds before passing it back
 * 	[1993/04/02  23:57:13  burati]
 * 
 * Revision 1.1.5.3  1993/02/04  14:26:32  frisco
 * 	Fix for OT #6789
 * 	[1993/01/29  22:30:52  frisco]
 * 
 * Revision 1.1.5.2  1992/12/29  16:37:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:27:09  zeliff]
 * 
 * Revision 1.1.3.2  1992/06/02  21:14:58  sekhar
 * 	Merged in Unix interface changes made by frisco
 * 	[1992/06/01  21:15:19  sekhar]
 * 
 * Revision 1.1.1.2  1992/05/21  21:44:58  frisco
 * 	Initial Unix interface support
 * 
 * $EndLog$
 */

/*  rs_unix.c V=50 5/21/92 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - Unix interfaces
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rs_unix.c	50 - 5/21/92";
#endif

#include "rs_base.h"
#include "rs_unix.h"
#include "rsdb_unix.h"
#include "rs_util.h"
#include "rsdb_pgo.h"
#include "rsdb_acct.h"
#include "rs_lock.h"
#include "un_strng.h"

#ifndef NULL
#   define NULL 0L
#endif

/*
** Temporary macros to access variant part of query unions.
** Someday NIDL will support a better mechanism for specifying
** unions.
*/
#define QUERY_NAME(Q) (Q).tagged_union.name
#define QUERY_UNIX_NUM(Q) (Q).tagged_union.unix_num
#define RESULT_PWENT(R) (R).tagged_union.passwd_entry
#define RESULT_GRENT(R) (R).tagged_union.group_entry


PUBLIC void rs_unix_getpwents 
#ifndef __STDC__
    ( h, key, num, cursor, num_out, result, cache_info, st )
    handle_t                h;          /* [in] */
    rs_unix_query_key_t     *key;       /* [in] */
    unsigned32              num;        /* [in] */
    sec_rgy_cursor_t        *cursor;    /* [in, out] */
    unsigned32              *num_out;   /* [out] */
    sec_rgy_unix_passwd_t   result[];   /* [out] */
    rs_cache_data_t         *cache_info;/* [out] */
    error_status_t          *st;        /* [out] */
#else
  (
    handle_t                h,          /* [in] */
    rs_unix_query_key_t     *key,       /* [in] */
    unsigned32              num,        /* [in] */
    sec_rgy_cursor_t        *cursor,    /* [in, out] */
    unsigned32              *num_out,   /* [out] */
    sec_rgy_unix_passwd_t   result[],   /* [out] */
    rs_cache_data_t         *cache_info,/* [out] */
    error_status_t          *st
  )
#endif
{
    error_status_t          status;
    sec_rgy_pgo_item_t      person;
    sec_rgy_name_t          name;
    int                     name_len;
    sec_rgy_login_name_t    login_name;
    sec_rgy_unix_sid_t      unix_sid;
    sec_rgy_sid_t           sid;
    sec_rgy_acct_key_t      key_parts;
    sec_rgy_acct_user_t     user_part;
    sec_rgy_acct_admin_t    admin_part;
    sec_rgy_properties_t    cell_props;
    boolean32               auth;
    sec_rgy_cursor_t        local_cursor;
    boolean32               check_nil_acct;
    unsigned long           i;
    unsigned32              total_out;

    *st = error_status_ok;
    total_out = 0;

    check_nil_acct = true;

    /* disallow use of getpwent lookup, only getpwnam or getpwuid */
    if (key->query == rs_unix_query_none)
        *st = sec_rgy_not_implemented;

    if (key->query == rs_unix_query_name) {
        name_len = QUERY_NAME(*key).name_len;
        if (name_len >= sizeof(sec_rgy_name_t)) {
            *st = sec_rgy_bad_data;
            rs_util_bad_client(h, "rs_unix_getpwents", st);
        }
        else
            QUERY_NAME(*key).name[name_len] = '\0';
    }

    if (*st == error_status_ok) {
        READ_LOCK(lock_db) {
            /* At this time, st is a dummy param and is always set to 0 */
            rsdb_properties_get_info(&cell_props, st);

            if (key->query == rs_unix_query_name) {
                name_len = QUERY_NAME(*key).name_len;
                strcpy((char *) name, (char *) &(QUERY_NAME(*key).name[0]));
            } else if (key->query == rs_unix_query_unix_num) {
                cursor->valid = false;
                local_cursor.valid = false;
                rsdb_pgo_get_by_unix_num(sec_rgy_domain_person, QUERY_UNIX_NUM(*key),
                                            true, &local_cursor, name, &person, st);
                if (*st != error_status_ok) {
                    check_nil_acct = false;
                }
            } else {
                name_len = 1;
                name[0] = '\0';
                check_nil_acct = false;
            }

            for (i = 0; i <= num; i++) {
                if (*st == error_status_ok) {
                    strcpy((char *) login_name.pname, (char *) name);
                    login_name.gname[0] = '\0';
                    login_name.oname[0] = '\0';
                    rsdb_acct_lookup(&login_name, cursor, &sid, &unix_sid,
                                            &key_parts, &user_part, &admin_part,
                                            st);
                }

                if (*st == error_status_ok) {
                    rsdb_unix_gen_pwent(name, &unix_sid, &sid, key_parts,
                                            &user_part, &admin_part,
                                            &result[i], st);
                    if (GOOD_STATUS(st) &&
			FLAG_SET(cell_props.flags,sec_rgy_prop_shadow_passwd)){
                        memset(result[i].passwd, 0, sizeof(result[i].passwd));
                        strcpy((char *)result[i].passwd, "*");
                    }
                    if (key->query == rs_unix_query_none) {
                        name_len = 1;
                        name[0] = '%';
                    }
                    total_out++;
                } else if (check_nil_acct && *st != sec_rgy_bad_data) {
                    /*
                     * Do something for people who don't have accounts
                     */
                    if (key->query == rs_unix_query_name) {
                        if (name_len <= sizeof(sec_rgy_name_t)) {
                            rsdb_pgo_get_by_name(sec_rgy_domain_person, name,
                                                    &local_cursor, &person, st);
                        }
                        if (*st != error_status_ok) {
                            check_nil_acct = false;
                        }
                    }
                    if (check_nil_acct) {
                        rsdb_unix_gen_dummy_pwent(name, &person, &result[i]);
                        *st = error_status_ok;
                        total_out++;
                    }
                    /*
                     * End of query - no more data
                     */
                    break;
                } else {
                    /*
                     * End of query - no more data
                     */
                    break;
                }
            }

        } END_READ_LOCK;
    }
    if (total_out > 0)
        *num_out = total_out - 1;
    else
        *num_out = 0;

    rs_util_get_cache_data(cache_info);
}

PUBLIC void rs_unix_getmemberents 
#ifndef __STDC__
    ( h, domain, key, max_num_results, member_cursor,
      cursor, num_members, members, cache_info, result, status )
    handle_t                h;                  /* [in] */
    sec_rgy_domain_t        domain;             /* [in] */
    rs_unix_query_key_t     *key;               /* [in] */
    signed32                max_num_results;    /* [in] */
    sec_rgy_cursor_t        *member_cursor;     /* [in, out] */
    sec_rgy_cursor_t        *cursor;            /* [in, out] */
    signed32                *num_members;       /* [out] */
    sec_rgy_member_t        members[];          /* [out] */
    rs_cache_data_t         *cache_info;        /* [out] */
    sec_rgy_unix_group_t    *result;            /* [out] */
    error_status_t          *status;            /* [out] */
#else
  (
    handle_t                h,                  /* [in] */
    sec_rgy_domain_t        domain,             /* [in] */
    rs_unix_query_key_t     *key,               /* [in] */
    signed32                max_num_results,    /* [in] */
    sec_rgy_cursor_t        *member_cursor,     /* [in, out] */
    sec_rgy_cursor_t        *cursor,            /* [in, out] */
    signed32                *num_members,       /* [out] */
    sec_rgy_member_t        members[],          /* [out] */
    rs_cache_data_t         *cache_info,        /* [out] */
    sec_rgy_unix_group_t    *result,            /* [out] */
    error_status_t          *status             /* [out] */
  )
#endif
{
    sec_rgy_pgo_item_t      grp_pgo;
    signed32                actual_num_members;
    sec_rgy_cursor_t        local_cursor;
    sec_rgy_name_t          name;
    int                     name_len;
    signed32                total_members;

    *status = error_status_ok;
    total_members = 0;

    /* disallow use of getgrent lookup, only getgrnam or getgruid */
    if (key->query == rs_unix_query_none)
        *status = sec_rgy_not_implemented;

    if (key->query == rs_unix_query_name) {
        name_len = QUERY_NAME(*key).name_len;
        if (QUERY_NAME(*key).name_len >= sizeof(sec_rgy_name_t)) {
            *status = sec_rgy_bad_data;
            rs_util_bad_client(h, "rs_unix_getmemberents", status);
        }
        else
            QUERY_NAME(*key).name[name_len] = '\0';
    }

    if (*status == error_status_ok) {
        READ_LOCK(lock_db) {
            if (key->query == rs_unix_query_name) {
                rsdb_pgo_get_by_name(domain, QUERY_NAME(*key).name,
                                        cursor, &grp_pgo, status);
                strcpy((char *) name, (char *) QUERY_NAME(*key).name);
                local_cursor = *cursor;
                rsdb_pgo_get_prev_marker(domain, &local_cursor, cursor);
            } else if (key->query == rs_unix_query_unix_num) {
                rsdb_pgo_get_by_unix_num(domain, QUERY_UNIX_NUM(*key),
                                            true, cursor, name, &grp_pgo, status);
                local_cursor = *cursor;
                rsdb_pgo_get_prev_marker(domain, &local_cursor, cursor);
            } else {
                local_cursor = *cursor;
                rsdb_pgo_get_next(domain, &local_cursor, name, &grp_pgo, status);
            }

            if (*status == error_status_ok) {
                rsdb_pgo_get_members(true, domain, name, member_cursor,
                                     max_num_results, members, &total_members,
                                     &actual_num_members, status);

                if (*status == error_status_ok) {

                    rsdb_unix_gen_grent(name, &grp_pgo, members, total_members,
                                        result, status);
                    /*
                     * If we don't care about extra members (getgrent) or
                     * there aren't any more members, then set the org cursor
                     * to the next org, otherwise keep the previous cursor intact
                     */
                    if (!member_cursor->valid) {
                        *cursor = local_cursor;
                    }
                }
            }
        } END_READ_LOCK;
    }
    if (total_members > 0)
        *num_members = total_members - 1;
    else
        *num_members = 0;

    rs_util_get_cache_data(cache_info);
}
