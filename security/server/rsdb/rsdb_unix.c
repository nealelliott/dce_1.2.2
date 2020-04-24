/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_unix.c,v $
 * Revision 1.1.7.2  1996/02/18  00:22:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:44  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:00:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:33  root]
 * 
 * Revision 1.1.5.2  1992/12/29  16:39:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:29:14  zeliff]
 * 
 * Revision 1.1.3.2  1992/06/02  21:17:37  sekhar
 * 	Dropped Unix interface changes made by frisco
 * 	[1992/06/01  21:18:30  sekhar]
 * 
 * Revision 1.1.1.2  1992/05/22  19:44:37  frisco
 * 	Initial revision - Unix interface backend
 * 
 * $EndLog$
 */
/*
*/

/*  rsdb_unix.c V=20 05/21/92 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - Unix interfaces backend
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rsdb_unix.c	20 - 05/21/92";
#endif

#include "rs_base.h"
#include "rsdb_unix.h"
#include "rsdb_pgo.h"
#include "un_strng.h"

PUBLIC void rsdb_unix_gen_pwent 
#ifndef __STDC__
    ( login_name, unix_sid, sid, key_parts, user_part, admin_part,
      passwd_rec, status )
    sec_rgy_name_t         login_name;
    sec_rgy_unix_sid_t     *unix_sid;
    sec_rgy_sid_t          *sid;
    sec_rgy_acct_key_t     key_parts;
    sec_rgy_acct_user_t    *user_part;
    sec_rgy_acct_admin_t   *admin_part;
    sec_rgy_unix_passwd_t  *passwd_rec;
    error_status_t         *status;
#else
  (
    sec_rgy_name_t        login_name,
    sec_rgy_unix_sid_t     *unix_sid,
    sec_rgy_sid_t          *sid,
    sec_rgy_acct_key_t     key_parts,
    sec_rgy_acct_user_t    *user_part,
    sec_rgy_acct_admin_t   *admin_part,
    sec_rgy_unix_passwd_t  *passwd_rec,
    error_status_t         *status
  )
#endif
{
    sec_rgy_pgo_item_t     pgo;
    sec_rgy_cursor_t       cursor;
    unsigned char          *p;
    sec_rgy_name_t         name;
    int                    name_len;
    int                    gecos_len;
    unsigned char          name_buf[sizeof(sec_rgy_unix_login_name_t) + 1];

    /* Construct account name */
    cursor.valid = false;
    rsdb_pgo_get_by_uid(sec_rgy_domain_person, &(sid->person), true, 
                            &cursor, name, &pgo, status);
    if (*status != error_status_ok)
        return;

    strcpy((char *) name_buf, (char *) login_name);
    p = name_buf;
    while (*p != '.')
        p++;

    if (key_parts == sec_rgy_acct_key_person) {
        *p = '\0';
    }

    else if (key_parts == sec_rgy_acct_key_group) {
        p++;
        while (*p != '.' && *p)
            p++;
        *p = '\0';
    }       

    /* Get person info */
    passwd_rec->uid = unix_sid->person;

    /* Get Group Info */
    passwd_rec->gid = unix_sid->group;
    
    /* Get Org Info */
    passwd_rec->oid = unix_sid->org;

    /* copy name */
    strcpy((char *) passwd_rec->name, (char *) name_buf);

    /* copy passwd */
    strncpy((char *) passwd_rec->passwd, (char *) user_part->passwd, 
            sec_rgy_max_unix_passwd_len);

    /* copy gecos */
    strcpy((char *) passwd_rec->gecos, (char *) pgo.fullname);
    name_len = strlen((char *) pgo.fullname);
    gecos_len = strlen((char *) user_part->gecos);

    if (name_len > 0 && gecos_len > 0) {
        passwd_rec->gecos[name_len] = ',';
        name_len++;
    }
    if (gecos_len > 0) {
        if (gecos_len + name_len >= sizeof(passwd_rec->gecos))
            gecos_len = sizeof(passwd_rec->gecos) - name_len - 1;

        strncpy((char *) &(passwd_rec->gecos[name_len]), (char *) user_part->gecos, 
                        gecos_len);
        passwd_rec->gecos[gecos_len + name_len] = '\0';
    }
    else
        passwd_rec->gecos[name_len] = '\0';

    /* copy homedir */
    name_len = strlen((char *) user_part->homedir);
    if (name_len >= sizeof(passwd_rec->homedir))
        name_len = sizeof(passwd_rec->homedir) - 1;

    strncpy((char *) passwd_rec->homedir, (char *) user_part->homedir, name_len);
    passwd_rec->homedir[name_len] = '\0';

    /* copy shell */
    name_len = strlen((char *) user_part->shell);
    if (name_len >= sizeof(passwd_rec->shell))
        name_len = sizeof(passwd_rec->shell) - 1;

    strncpy((char *) passwd_rec->shell, (char *) user_part->shell, name_len);
    passwd_rec->shell[name_len] = '\0';
}

PUBLIC void rsdb_unix_gen_dummy_pwent 
#ifndef __STDC__
    ( name, pgo, passwd_rec )
    sec_rgy_name_t         name;
    sec_rgy_pgo_item_t     *pgo;
    sec_rgy_unix_passwd_t  *passwd_rec;
#else 
  (
    sec_rgy_name_t         name,
    sec_rgy_pgo_item_t     *pgo,
    sec_rgy_unix_passwd_t  *passwd_rec
  )
#endif
{
    strcpy((char *) passwd_rec->name, (char *) name);
    passwd_rec->uid         = pgo->unix_num;
    passwd_rec->gid         = -2;       /* none */
    passwd_rec->oid         = -2;       /* none */
    strcpy((char *) passwd_rec->gecos, (char *) pgo->fullname);
    strcpy((char *) passwd_rec->passwd, "*NOACCT*");
    strcpy((char *) passwd_rec->homedir, "");
    strcpy((char *) passwd_rec->shell, "");
}

PUBLIC void rsdb_unix_gen_grent
#ifndef __STDC__
    ( name, grp_pgo, members, num_members, result, status )
    sec_rgy_name_t         name;
    sec_rgy_pgo_item_t     *grp_pgo;
    sec_rgy_member_t       members[];
    long                   num_members;
    sec_rgy_unix_group_t   *result;
    error_status_t         *status;
#else
  (
    sec_rgy_name_t         name,
    sec_rgy_pgo_item_t     *grp_pgo,
    sec_rgy_member_t       members[],
    long                   num_members,
    sec_rgy_unix_group_t   *result,
    error_status_t         *status
  )
#endif
{
    int                    i;
    sec_rgy_member_buf_t   mem_string;

    *status = error_status_ok;

    /* set group name */

    strcpy((char *) result->name, (char *) name);

    /* set group Unix ID */

    result->gid = grp_pgo->unix_num;

    /* construct member string */

    mem_string[0] = '\0';
    for (i = 0; i < num_members; i++) {

         if ( (strlen((char *) mem_string) + strlen((char *) members[i]))
             >= sizeof(sec_rgy_member_buf_t) )
                  break;

         if ( i > 0 )
             strcat((char *) mem_string, ",");

         strcat((char *) mem_string, (char *) members[i]);

    }

    strcpy((char *) result->members, (char *) mem_string);

}

