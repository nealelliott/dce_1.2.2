/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_audit_trail.c,v $
 * Revision 1.1.5.2  1996/02/18  00:17:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:34  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:50:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:18  root]
 * 
 * Revision 1.1.2.4  1992/12/29  16:34:45  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:12  zeliff]
 * 
 * Revision 1.1.2.3  1992/09/29  20:12:35  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:36:32  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:23:52  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:11:19  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_audit_trail.c V=11 08/22/91 //littl/prgy/src/server/rs
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
 *      Registry Server - Audit Trail
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rs_audit_trail.c	11 - 08/22/91";
#endif

#include <sys/types.h>
#include <sys/file.h>
#include <stdio.h>
#include <un_io.h>
#include <un_time.h>
#include <un_strng.h>
#include <un_maloc.h>


#include <dce/rpc.h>

#include <rs_secidmap.h>

#include <rs_audit_trail.h>
#include <rs_pvt.h>
#include <rs_base.h>

#include <rsdb_pgo.h>

/*
 * Define audit trail file location
 */
#if defined(__STDC__) && !defined(_K_R_PORT_CHECK_)
#   define AUDIT_TRAIL_FILE         DCELOCAL_PATH "/var/security/adm/audit_trail"

#else

static char *rs_audit_trail_file = NULL;
#   define AUDIT_TRAIL_FILE \
    STRING_CONCAT(rs_audit_trail_file, DCELOCAL_PATH, "/var/security/adm/audit_trail", "")

#endif

#ifndef CONST
#   ifdef __STDC__
#       define CONST const
#   else
#       define CONST
#   endif
#endif

/*
 * Local variables
 */
PRIVATE boolean32   rs_audit    = false;


/* g e t _ c a l l e r _ n a m e
 *
 * Obtain a printable name for the caller.
 */
PRIVATE get_caller_name
#ifndef __STDC__
    ( h, global_name )
    handle_t        h;
    sec_rgy_name_t  global_name;
#else
  (
    handle_t        h,
    sec_rgy_name_t  global_name
  )
#endif
{
    sec_rgy_name_t          cell_name;
    sec_rgy_name_t          princ_name;
    error_status_t          st;
    unsigned32              authz_svc;
    rpc_authz_handle_t      privs;
    sec_id_pac_t            *pac;
    static CONST char       *unknown_princ = "Unknown Principal";
    static CONST char       *unknown_cell  = "Unknown Cell";
    rsec_id_output_selector_t   selector;
    unsigned char           *uuid_string;
    int                     len;

    rpc_binding_inq_auth_client(h, &privs, NULL, NULL, NULL, &authz_svc, &st);
    if (BAD_STATUS(&st)) {
        ustrcpy(global_name, unknown_princ);
    } else {
        if (authz_svc == rpc_c_authz_name) {
            ustrcpy(global_name, privs);
        } else if (authz_svc == rpc_c_authz_dce) {
            pac = (sec_id_pac_t *) privs;
            selector = rsec_id_output_select_cname |
                    rsec_id_output_select_gname | rsec_id_output_select_pname;

            rs_sec_id_gen_name(sec_rgy_domain_person, &pac->realm.uuid,
                                &pac->principal.uuid, selector, global_name,
                                cell_name, princ_name, &st);
            if (st == sec_id_e_foreign_cell_referral) {
                uuid_to_string(&pac->principal.uuid, &uuid_string, &st);
                if (BAD_STATUS(&st)) {
                    ustrcpy(global_name, unknown_princ);
                } else {
                    ustrcpy(global_name, uuid_string);
                    rpc_string_free(&uuid_string, &st);
                }
                len = ustrlen(global_name);
                global_name[len] = '@';
                ustrcpy(&global_name[len+1], cell_name);

            } else if (BAD_STATUS(&st)) {
                uuid_to_string(&pac->principal.uuid, &uuid_string, &st);
                if (BAD_STATUS(&st)) {
                    ustrcpy(global_name, unknown_princ);
                } else {
                    ustrcpy(global_name, uuid_string);
                    rpc_string_free(&uuid_string, &st);
                }
                len = ustrlen(global_name);
                global_name[len] = '@';

                uuid_to_string(&pac->realm.uuid, &uuid_string, &st);
                if (BAD_STATUS(&st)) {
                    ustrcpy(&global_name[len+1], unknown_cell);
                } else {
                    ustrcpy(&global_name[len+1], uuid_string);
                    rpc_string_free(&uuid_string, &st);
                }
            }
            /*
             * GOOD_STATUS - name is already in global_name
             */

        } else {
            ustrcpy(global_name, unknown_princ);
        }
    }
}


PUBLIC void rs_audit_trail_enable (
#ifdef __STDC__
    void
#endif
)
{
    rs_audit = true;
}

PUBLIC void rs_audit_trail_log
#ifndef __STDC__
    ( h, info, key )
    handle_t    h;
    char        *info;
    char        *key;
#else
  (
    handle_t    h,
    char        *info,
    char        *key
  )
#endif
{
    int                     fd;
    sec_rgy_login_name_t    login_name;
    error_status_t          status;
    unsigned long           len;
    unsigned_char_p_t       binding_p;
    static unsigned_char_t  unknown[] = "Unknown caller";
    boolean32               free_binding = false;
    char                    time_buf[32];
    sec_rgy_name_t          global_name;
    char                    buf[sizeof(sec_rgy_name_t) + 512];
    char                    *name_buf = (char *) global_name;
    struct  timeval         tv;
    struct  timezone        tz;

    if (!rs_audit)
        return;

#ifdef SNI_SVR4_POSIX
    if (gettimeofday(&tv) == 0) {
#else
    if (gettimeofday(&tv, &tz) == 0) {
#endif
        strcpy(time_buf, ctime(&tv.tv_sec));
        len = strlen(time_buf);
        time_buf[len-1] = '\0';
    } else {
        strcpy(time_buf, "<Time Unavailable>\n");
    }

    fd = open(AUDIT_TRAIL_FILE, O_RDWR | O_APPEND | O_CREAT, 0666);

    CLEAR_STATUS(&status);

    if (h) {
        rpc_binding_to_string_binding(h, &binding_p, &status);
	if (BAD_STATUS(&status)) {
	    binding_p = unknown;
	} else {
	    free_binding = true;
	}
        get_caller_name(h, global_name);
     } else {
        binding_p = unknown;
        name_buf = (char *) unknown;
    }

    if (key == NULL) {
        sprintf(buf, "-----\n    %s: by %s from %s\n    %s\n",
		time_buf, name_buf, binding_p, info);
    } else {
        sprintf(buf, "-----\n    %s: by %s from %s\n    %s [Key = '%s']\n",
                    time_buf, name_buf, binding_p, info, key);
    }

    if (free_binding) {
	if (binding_p != NULL) {
	    rpc_string_free(&binding_p, &status);
	}
    }

    if (fd != -1) {
        write(fd, buf, strlen(buf));
        close(fd);
    } else if (!rs_verbose) {
	printf("%s", buf);
    }

    if (rs_verbose) {
        printf("%s", buf);
    }
}
