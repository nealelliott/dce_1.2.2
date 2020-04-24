/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: unix.c,v $
 * Revision 1.1.14.1  1996/10/03  14:49:11  arvind
 * 	Silence warning about uninitialized variables.
 * 	[1996/09/16  20:31 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.12.2  1996/02/18  00:07:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:56  marty]
 * 
 * Revision 1.1.12.1  1995/12/13  16:24:05  root
 * 	Submit
 * 	[1995/12/11  15:13:56  root]
 * 
 * Revision 1.1.8.1  1993/10/05  22:29:54  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:03:59  mccann]
 * 
 * Revision 1.1.6.5  1993/02/05  22:45:55  sommerfeld
 * 	[OT5061] Deal with change to CHECK_CACHE macro.
 * 	[1993/02/04  22:32:50  sommerfeld]
 * 
 * Revision 1.1.6.3  1992/12/29  13:04:46  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:42:18  zeliff]
 * 
 * Revision 1.1.6.2  1992/09/29  21:17:00  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:20:41  sekhar]
 * 
 * Revision 1.1.3.2  1992/06/02  20:54:15  sekhar
 * 	Drop Unix interface changes made by frisco
 * 	[1992/06/01  21:04:43  sekhar]
 * 
 * Revision 1.1.1.2  1992/05/22  18:34:46  frisco
 * 	Initial revision - Unix client interface routines
 * 
 * $EndLog$
 */

/*  unix.c V=23 05/21/92 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/* 
**      Registry Client Unix Interface
**
** Copyright Hewlett-Packard Company 1989, 1990
** Copyright Apollo Computer Inc. 1987, 1988
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)unix.c	23 - 05/21/92";
#endif

#include "rs_base.h"
#include "rca_pvt.h"
#include "rgymacro.h"

#include "unix.h"
#include "rs_unix.h"

#include "override.h"

#include "un_stat.h"
#include "un_strng.h"
#include "un_io.h"
#include <sys/file.h>
#if defined(SYS5) || defined(SNI_SVR4_POSIX)
#    include <fcntl.h>
#endif

#define Crs_unix_getpwents(RH,b,c,d,e,f,CI,g)\
        (*rs_unix_v1_0_c_epv.rs_unix_getpwents) (HANDLE(RH),b,c,d,e,f,CI,g)

#define Crs_unix_getmemberents(RH,b,c,d,e,f,g,h,CI,i,j)\
        (*rs_unix_v1_0_c_epv.rs_unix_getmemberents) (HANDLE(RH),b,c,d,e,f,g,h,CI,i,j)

/*
** Temporary macros to access variant part of query unions.
** Someday NIDL will support a better mechanism for specifying
** unions.
*/
#define QUERY_NAME(Q) (Q).tagged_union.name
#define QUERY_UNIX_NUM(Q) (Q).tagged_union.unix_num
#define RESULT_PWENT(R) (R).tagged_union.passwd_entry
#define RESULT_GRENT(R) (R).tagged_union.group_entry

#ifdef GLOBAL_LIBRARY
#   include <set_sect_pvt.c>
#endif

/*
 * Internal Function Prototypes
 */

PRIVATE void getpwents ( 
#ifdef __STDC__
    sec_rgy_handle_t       context,
    rs_unix_query_key_t    *key,
    sec_rgy_cursor_t       *marker,
    sec_rgy_unix_passwd_t  result[],
    unsigned32             max_num,
    unsigned32             *num_out,
    error_status_t         *status
#endif
);

PRIVATE void getgrent ( 
#ifdef __STDC__
    sec_rgy_domain_t        domain,
    sec_rgy_handle_t        context,
    rs_unix_query_key_t     *key,
    signed32                max_num_members,
    sec_rgy_cursor_t        *member_marker,
    sec_rgy_cursor_t        *marker,
    signed32                *num_members,
    sec_rgy_member_t        members[],
    sec_rgy_unix_group_t    *result,
    error_status_t          *status
#endif
);

PRIVATE void rgy_override_entry (
#ifdef __STDC__
    sec_rgy_unix_passwd_t   *pw,
    sec_override_fields_t   *overridden,
    error_status_t          *st
#endif
);

/*
 *  End of Function Prototypes
 */

PRIVATE void getpwents
#ifndef __STDC__
    (context, key, marker, result, max_num, num_out, status)
    sec_rgy_handle_t       context;
    rs_unix_query_key_t    *key;
    sec_rgy_cursor_t       *marker;
    sec_rgy_unix_passwd_t  result[];
    unsigned32             max_num;
    unsigned32             *num_out;
    error_status_t         *status;
#else
  (
    sec_rgy_handle_t       context,
    rs_unix_query_key_t    *key,
    sec_rgy_cursor_t       *marker,
    sec_rgy_unix_passwd_t  result[],
    unsigned32             max_num,
    unsigned32             *num_out,
    error_status_t         *status
  )
#endif
{
    rs_cache_data_t     cache_info;

    SETUP_RETRY(rca_op_read, status) {
        Crs_unix_getpwents(context, key, max_num, marker, num_out,
            result, &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_unix_getpwent 
#ifndef __STDC__
    ( context, marker, passwd_entry, status )
    sec_rgy_handle_t       context;
    sec_rgy_cursor_t       *marker;
    sec_rgy_unix_passwd_t  *passwd_entry;
    error_status_t         *status;
#else
  (
    sec_rgy_handle_t       context,
    sec_rgy_cursor_t       *marker,
    sec_rgy_unix_passwd_t  *passwd_entry,
    error_status_t         *status
  )
#endif
{
    rs_unix_query_key_t    key;
    unsigned32             max_num = 0;
    sec_override_fields_t  overridden;
    unsigned32             num_out;

    key.query = rs_unix_query_none;

    getpwents(context, &key, marker, passwd_entry, max_num, &num_out, status);

    if (*status == error_status_ok) {
        rgy_override_entry(passwd_entry, &overridden, status);
    }
}

PUBLIC void sec_rgy_unix_getpwuid 
#ifndef __STDC__
    ( context, uid, marker, passwd_entry, status )
    sec_rgy_handle_t       context;
    signed32               uid;
    sec_rgy_cursor_t       *marker;
    sec_rgy_unix_passwd_t  *passwd_entry;
    error_status_t         *status;
#else
  (
    sec_rgy_handle_t       context,
    signed32               uid,
    sec_rgy_cursor_t       *marker,
    sec_rgy_unix_passwd_t  *passwd_entry,
    error_status_t         *status
  )
#endif
{
    rs_unix_query_key_t    key;
    unsigned32             max_num = 0;
    sec_override_fields_t  overridden;
    unsigned32             num_out;

    key.query = rs_unix_query_unix_num;
    QUERY_UNIX_NUM(key) = uid;

    getpwents(context, &key, marker, passwd_entry, max_num, &num_out, status);

    if (*status == error_status_ok) {
        rgy_override_entry(passwd_entry, &overridden, status);
    }
}

PUBLIC void sec_rgy_unix_getpwnam 
#ifndef __STDC__
    ( context, name, name_len, marker, passwd_entry, status )
    sec_rgy_handle_t       context;
    sec_rgy_name_t         name;
    unsigned32             name_len;
    sec_rgy_cursor_t       *marker;
    sec_rgy_unix_passwd_t  *passwd_entry;
    error_status_t         *status;
#else
  (
    sec_rgy_handle_t       context,
    sec_rgy_name_t         name,
    unsigned32             name_len,
    sec_rgy_cursor_t       *marker,
    sec_rgy_unix_passwd_t  *passwd_entry,
    error_status_t         *status
  )
#endif
{
    rs_unix_query_key_t    key;
    unsigned32             max_num = 0;
    sec_override_fields_t  overridden;
    unsigned32             num_out;

    if (name_len > sizeof(sec_rgy_name_t)) {
        *status = sec_rgy_bad_data;
        return;
    }

    key.query = rs_unix_query_name;
    QUERY_NAME(key).name_len = name_len;
    strncpy((char *) QUERY_NAME(key).name, (char *) name, name_len);

    getpwents(context, &key, marker, passwd_entry, max_num, &num_out, status);

    if (*status == error_status_ok) {
        rgy_override_entry(passwd_entry, &overridden, status);
    }
}

PRIVATE void getgrent 
#ifndef __STDC__
    ( domain, context, key, max_num_members, member_marker, 
               marker, num_members, members, result, status )
    sec_rgy_domain_t        domain;
    sec_rgy_handle_t        context;
    rs_unix_query_key_t     *key;
    signed32                max_num_members;
    sec_rgy_cursor_t        *member_marker;
    sec_rgy_cursor_t        *marker;
    signed32                *num_members;
    sec_rgy_member_t        members[];
    sec_rgy_unix_group_t    *result;
    error_status_t          *status;
#else
  (
    sec_rgy_domain_t        domain,
    sec_rgy_handle_t        context,
    rs_unix_query_key_t     *key,
    signed32                max_num_members,
    sec_rgy_cursor_t        *member_marker,
    sec_rgy_cursor_t        *marker,
    signed32                *num_members,
    sec_rgy_member_t        members[],
    sec_rgy_unix_group_t    *result,
    error_status_t          *status
  )
#endif
{
    rs_cache_data_t     cache_info;

    SETUP_RETRY(rca_op_read, status) {
        Crs_unix_getmemberents(context, sec_rgy_domain_group, key, max_num_members, 
              member_marker, marker, num_members, members, &cache_info, result, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        } 
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_unix_getgrent 
#ifndef __STDC__
    ( context, max_num_members, marker, group_entry,
                        num_members, members, status )
    sec_rgy_handle_t        context;
    signed32                max_num_members;
    sec_rgy_cursor_t        *marker;
    sec_rgy_unix_group_t    *group_entry;
    signed32                *num_members;
    sec_rgy_member_t        members[];
    error_status_t          *status;
#else
  (
    sec_rgy_handle_t        context,
    signed32                max_num_members,
    sec_rgy_cursor_t        *marker,
    sec_rgy_unix_group_t    *group_entry,
    signed32                *num_members,
    sec_rgy_member_t        members[],
    error_status_t          *status
  )
#endif
{
    rs_unix_query_key_t    key;
    sec_rgy_cursor_t       member_marker;
    sec_rgy_domain_t       domain;
    signed32               max_members = 0;
    signed32               ret_members;

    member_marker.valid = false;

    if (max_num_members > 0)
        max_members = max_num_members - 1;

    key.query = rs_unix_query_none;
    domain = sec_rgy_domain_group;

    getgrent(domain, context, &key, max_members, &member_marker, marker, 
             &ret_members, members, group_entry, status);

    if (GOOD_STATUS(status))
        *num_members = ret_members + 1;
}

PUBLIC void sec_rgy_unix_getgrgid 
#ifndef __STDC__
    ( context, gid, max_num_members, marker, group_entry, 
                   num_members, members, status )
    sec_rgy_handle_t        context;
    signed32                gid;
    signed32                max_num_members;
    sec_rgy_cursor_t        *marker;
    sec_rgy_unix_group_t    *group_entry;
    signed32                *num_members;
    sec_rgy_member_t        members[];
    error_status_t          *status;
#else
  (
    sec_rgy_handle_t        context,
    signed32                gid,
    signed32                max_num_members,
    sec_rgy_cursor_t        *marker,
    sec_rgy_unix_group_t    *group_entry,
    signed32                *num_members,
    sec_rgy_member_t        members[],
    error_status_t          *status
  )
#endif
{
    rs_unix_query_key_t    key;
    sec_rgy_cursor_t       member_marker;
    sec_rgy_domain_t       domain;
    signed32               max_members = 0;
    signed32               ret_members;

    member_marker.valid = false;

    if (max_num_members > 0)
        max_members = max_num_members - 1;

    key.query = rs_unix_query_unix_num;
    QUERY_UNIX_NUM(key) = gid;
    domain = sec_rgy_domain_group;

    getgrent(domain, context, &key, max_members, &member_marker, marker, 
             &ret_members, members, group_entry, status);
    
    if (GOOD_STATUS(status))
        *num_members = ret_members + 1;
}

PUBLIC void sec_rgy_unix_getgrnam 
#ifndef __STDC__
    ( context, name, name_len, max_num_members, marker, 
          group_entry, num_members, members, status )
    sec_rgy_handle_t        context;
    sec_rgy_name_t          name;
    signed32                name_len;
    signed32                max_num_members;
    sec_rgy_cursor_t        *marker;
    sec_rgy_unix_group_t    *group_entry;
    signed32                *num_members;
    sec_rgy_member_t        members[];
    error_status_t          *status;
#else
  (
    sec_rgy_handle_t        context,
    sec_rgy_name_t          name,
    signed32                name_len,
    signed32                max_num_members,
    sec_rgy_cursor_t        *marker,
    sec_rgy_unix_group_t    *group_entry,
    signed32                *num_members,
    sec_rgy_member_t        members[],
    error_status_t          *status
  )
#endif
{
    rs_unix_query_key_t    key;
    sec_rgy_cursor_t       member_marker;
    sec_rgy_domain_t       domain;
    signed32               max_members = 0;
    signed32               ret_members;

    member_marker.valid = false;

    if (max_num_members > 0)
        max_members = max_num_members - 1;

    if (name_len > sizeof(sec_rgy_name_t)) {
        *status = sec_rgy_bad_data;
        return;
    }

    key.query = rs_unix_query_name;
    QUERY_NAME(key).name_len = name_len;
    strncpy((char *) QUERY_NAME(key).name, (char *) name, name_len);
    domain = sec_rgy_domain_group;

    getgrent(domain, context, &key, max_members, &member_marker, marker, 
             &ret_members, members, group_entry, status);
    
    if (GOOD_STATUS(status))
        *num_members = ret_members + 1;
}

PRIVATE void rgy_override_entry
#ifndef __STDC__
    (pw, overridden, st)
    sec_rgy_unix_passwd_t   *pw;
    sec_override_fields_t   *overridden;
    error_status_t          *st;
#else
  (
    sec_rgy_unix_passwd_t   *pw,
    sec_override_fields_t   *overridden,
    error_status_t          *st
  )
#endif
{
    sec_rgy_unix_passwd_buf_t       passwd;
    sec_rgy_name_t         gecos;
    sec_rgy_name_t         dir;
    sec_rgy_name_t         shell;
    unsigned32             gecos_size;
    unsigned32             dir_size;
    unsigned32             shell_size;
    
    override_get_login_info(pw->name, &pw->uid, &pw->gid, passwd, 
         gecos, dir, shell, overridden, st);

    if (*st == sec_login_s_no_override_info || *st == sec_login_s_ovrd_ent_not_found)
         *st = error_status_ok;
    else {
        if (*overridden == sec_override_pw_passwd) {
             strncpy((char *) pw->passwd, (char *) passwd, sec_rgy_max_unix_passwd_len);
        }
    
        if (*overridden == sec_override_pw_gecos) {
             gecos_size = sizeof(sec_rgy_unix_gecos_t);
             if (strlen((char *) gecos) >= gecos_size) {
                 strncpy((char *) pw->gecos, (char *) gecos, gecos_size - 1);
                 pw->gecos[gecos_size - 1] = '\0';
             }
             else
                 strcpy((char *) pw->gecos, (char *) gecos);
        }
    
        if (*overridden == sec_override_pw_dir) {
             dir_size = sizeof(sec_rgy_pname_t);
             if (strlen((char *) dir) >= dir_size) {
                 strncpy((char *) pw->homedir, (char *) dir, dir_size - 1);
                 pw->homedir[dir_size - 1] = '\0';
             }
             else
                 strcpy((char *) pw->homedir, (char *) dir);
        }
    
        if (*overridden == sec_override_pw_shell) {
             shell_size = sizeof(sec_rgy_pname_t);
             if (strlen((char *) shell) >= shell_size) {
                 strncpy((char *) pw->shell, (char *) shell, shell_size - 1);
                 pw->shell[shell_size - 1] = '\0';
             }
             else
                 strcpy((char *) pw->shell, (char *) dir);
        }
    }
}
