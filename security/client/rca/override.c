/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: override.c,v $
 * Revision 1.1.12.2  1996/03/11  13:28:06  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:52  marty]
 *
 * Revision 1.1.12.1  1995/12/08  18:06:54  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/03  22:02 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/28  16:48 UTC  aha
 * 	CHFts14138: create namecache file with secure mode; CHFts14693: init RPC binding handles to NULL
 * 
 * 	HP revision /main/aha_mothra4/2  1995/03/27  20:08 UTC  aha
 * 	Initialize RPC handles to null in declaration
 * 
 * 	HP revision /main/aha_mothra4/1  1995/03/24  21:13 UTC  aha
 * 	CHFts14693: init RPC binding handles to NULL
 * 	[1995/12/08  16:32:13  root]
 * 
 * Revision 1.1.7.3  1994/05/11  19:02:46  ahop
 * 	Merged with changes from 1.1.7.2
 * 	[1994/05/11  19:02:30  ahop]
 * 
 * 	make binding handle changes to new override routines
 * 	[1994/05/06  21:39:12  ahop]
 * 
 * 	merge with group override submission
 * 	[1994/05/06  18:08:10  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  20:46:38  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * Revision 1.1.7.2  1994/05/06  18:06:29  desai
 * 	CR 10559 fix. The group override submission broke the RIOS build.
 * 	[1994/05/06  18:06:10  desai]
 * 
 * Revision 1.1.7.1  1994/05/05  20:44:30  hasbrouc
 * 	add group override support (RFC 55)
 * 	[1994/05/05  20:14:15  hasbrouc]
 * 
 * Revision 1.1.4.5  1993/02/10  15:35:06  sommerfeld
 * 	[OT7179] Deal with RPC_SUPPORTED_PROTSEQS being set to something..
 * 	try all bindings to sec_clientd.
 * 	[1993/02/09  19:48:30  sommerfeld]
 * 
 * Revision 1.1.4.4  1993/01/22  23:10:11  burati
 * 	CR6726 Fixing memory leaks (enclose bind code in mutex and
 * 	only need to do it once, so keep track of bind success).
 * 	[1993/01/22  20:33:15  burati]
 * 
 * Revision 1.1.4.3  1992/12/29  13:03:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:40:40  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/08  21:47:47  burati
 * 	CR6365 Wasn't closing sec_clientd binding file after using it
 * 	[1992/12/08  18:30:00  burati]
 * 
 * Revision 1.1.2.3  1992/05/14  18:57:30  burati
 * 	Add override_is_passwd_overridden
 * 	[1992/05/14  01:50:48  burati]
 * 
 * Revision 1.1.2.2  1992/03/24  18:28:51  burati
 * 	Make it work with current build.
 * 	[1992/03/23  21:19:57  burati]
 * 
 * 	CR2106 Make sure override_check_passwd always 'return's a value
 * 	[1992/03/23  21:14:19  burati]
 * 
 * Revision 1.1  1992/01/19  14:47:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  override.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
**  Override client agent
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)override.c        13 - 02/19/92";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <dce/rpc.h>
#include <dce/sec_base.h>
#include <dce/override.h>
#include <roverride.h>

#include <macros.h>
#include <scd_loc.h>


/*
 * override_get_login_info
 *      Lookup the account in the passwd_override file, by a combination
 *      of name, uid, gid, and if found, return any overridden information.
 *      Obtains a string binding handle (w/port) from a configuration file
 *      and makes the remote call with that handle.
 */
void override_get_login_info
#ifndef __STDC__
    (pw_name, pw_uid, pw_gid, pw_salt, pw_gecos, pw_dir, pw_shell,
                                                            overridden, status)
    unsigned_char_t             *pw_name;
    signed32                    *pw_uid;
    signed32                    *pw_gid;
    sec_rgy_unix_passwd_buf_t   pw_salt;
    sec_rgy_name_t              pw_gecos;
    sec_rgy_name_t              pw_dir;
    sec_rgy_name_t              pw_shell;
    sec_override_fields_t       *overridden;
    error_status_t              *status;
#else
  (
    unsigned_char_t             *pw_name,
    signed32                    *pw_uid,
    signed32                    *pw_gid,
    sec_rgy_unix_passwd_buf_t   pw_salt,
    sec_rgy_name_t              pw_gecos,
    sec_rgy_name_t              pw_dir,
    sec_rgy_name_t              pw_shell,
    sec_override_fields_t       *overridden,
    error_status_t              *status
  )
#endif
{
    signed32 dummy_uid = sec_rgy_uxid_unknown;
    signed32 dummy_gid = sec_rgy_uxid_unknown;
    rpc_binding_handle_t handle = NULL;
    error_status_t lst;

    CLEAR_STATUS(status);

    /* bind to sec_clientd, if necessary */
    sec_login_util_bind_to_sec_clientd(&handle, status);
    if (STATUS_EQUAL(status, sec_login_s_config))
        SET_STATUS(status, sec_login_s_no_override_info);

    if (GOOD_STATUS(status)) {
        if (!pw_uid)
            pw_uid = &dummy_uid;
        if (!pw_gid)
            pw_gid = &dummy_gid;
        (*roverride_v1_0_c_epv.roverride_get_login_info)(handle, pw_name, pw_uid,
            pw_gid, pw_salt, pw_gecos, pw_dir, pw_shell, overridden, status);
        if (STATUS_EQUAL(status, rpc_s_comm_failure)) {
            SET_STATUS(status, sec_login_s_override_failure);
        }
    }

    /* clean up resources associated with handle */
    if (handle)
        rpc_binding_free(&handle, &lst);
}


/*
 * override_check_passwd
 *      Check the specified password against the overridden password
 *      for the specified account name.
 */
boolean32 override_check_passwd
#ifndef __STDC__
    (pw_name, pw_uid, pw_gid, pw_passwd, status)
    unsigned_char_p_t           pw_name;
    signed32                    *pw_uid;
    signed32                    *pw_gid;
    sec_rgy_unix_passwd_buf_t   pw_passwd;
    error_status_t              *status;
#else
  (
    unsigned_char_p_t           pw_name,
    signed32                    *pw_uid,
    signed32                    *pw_gid,
    sec_rgy_unix_passwd_buf_t   pw_passwd,
    error_status_t              *status
  )
#endif
{
    boolean32 retval;
    rpc_binding_handle_t handle = NULL;
    error_status_t lst;

    CLEAR_STATUS(status);

    /* bind to sec_clientd, if necessary */
    sec_login_util_bind_to_sec_clientd(&handle, status);
    if (STATUS_EQUAL(status, sec_login_s_config))
        SET_STATUS(status, sec_login_s_no_override_info);

    if (GOOD_STATUS(status)) {
        retval = (*roverride_v1_0_c_epv.roverride_check_passwd)
                    (handle, pw_name, pw_uid, pw_gid, pw_passwd, status);
        if (STATUS_EQUAL(status, rpc_s_comm_failure)) {
            SET_STATUS(status, sec_login_s_override_failure);
            rpc_binding_free(&handle, &lst);
            return false;
        } else {
            rpc_binding_free(&handle, &lst);
            return retval;
        }
    } else {
        if (handle)
            rpc_binding_free(&handle, &lst);
        return false;
    }
}


/*
 * override_is_passwd_overridden
 *      Lookup the account in the passwd_override file, by account name
 *      only, because the uid and gid are unknown at this point.  Return
 *      True if passwd has been overridden.
 *
 *      Obtains a string binding handle (w/port) from a configuration file
 *      and makes the remote call with that handle.
 */
boolean32 override_is_passwd_overridden
#ifndef __STDC__
    (pw_name, pw_uid, pw_gid, pw_salt, status)
    unsigned_char_t             *pw_name;
    signed32                    *pw_uid;
    signed32                    *pw_gid;
    sec_rgy_unix_passwd_buf_t   pw_salt;
    error_status_t              *status;
#else
  (
    unsigned_char_t             *pw_name,
    signed32                    *pw_uid,
    signed32                    *pw_gid,
    sec_rgy_unix_passwd_buf_t   pw_salt,
    error_status_t              *status
  )
#endif
{
    boolean32   retval;
    signed32    dummy_uid = sec_rgy_uxid_unknown;
    signed32    dummy_gid = sec_rgy_uxid_unknown;
    rpc_binding_handle_t handle = NULL;
    error_status_t lst;

    CLEAR_STATUS(status);

    /* bind to sec_clientd, if necessary */
    sec_login_util_bind_to_sec_clientd(&handle, status);
    if (STATUS_EQUAL(status, sec_login_s_config))
        SET_STATUS(status, sec_login_s_no_override_info);

    if (GOOD_STATUS(status)) {
        if (!pw_uid)
            pw_uid = &dummy_uid;
        if (!pw_gid)
            pw_gid = &dummy_gid;
        retval = (*roverride_v1_0_c_epv.roverride_is_passwd_overridden)(handle,
                    pw_name, pw_uid, pw_gid, pw_salt, status);
        if (STATUS_EQUAL(status, rpc_s_comm_failure)) {
            SET_STATUS(status, sec_login_s_override_failure);
            rpc_binding_free(&handle, &lst);
            return false;
        } else {
            rpc_binding_free(&handle, &lst);
            return retval;
        }
    } else {
        if (handle)
            rpc_binding_free(&handle, &lst);
        return false;
    }
}



   /*
    *	override_get_by_unix_num
    *		For a given domain type in name_domain, search the domain for the supplied num.
    *		For the domain sec_rgy_domain_person, look up the account name corresponding to
    *		the supplied UID in the override_buffer.  Return account name if found.
    *		For the sec_rgy_domain_group domain, look up the group name corresponding 
    *		to the supplied GID in the override_group_buffer.  Return group name. 
    *		Return false if nothing found, or if the sec_rgy_domain_org domain is searched.
    *	
    * Errors:
    *      COMMUNICATIONS ERROR
    *      sec_login_s_no_override_info
    *      sec_login_s_override_failure
    *      sec_login_s_ovrd_ent_not_found
    *	
    */

boolean32 override_get_by_unix_num 
#ifndef __STDC__
    (name_domain, unix_id, name, status)
     	sec_rgy_domain_t		name_domain;
     	signed32			unix_id;
     	sec_rgy_name_t			name;
     	error_status_t			*status;
#else
    (
     	sec_rgy_domain_t		name_domain,
     	signed32			unix_id,
     	sec_rgy_name_t			name,
     	error_status_t			*status
    )
#endif

{
    boolean32 retval;
    rpc_binding_handle_t handle = NULL;
    error_status_t lst;

    CLEAR_STATUS(status);

    /* bind to sec_clientd, if necessary */
    sec_login_util_bind_to_sec_clientd(&handle, status);
    if (STATUS_EQUAL(status, sec_login_s_config))
        SET_STATUS(status, sec_login_s_no_override_info);

    if (GOOD_STATUS(status)) {
        retval = (*roverride_v1_0_c_epv.roverride_get_by_unix_num)
                    (handle, name_domain, unix_id, name, status);
        if (STATUS_EQUAL(status, rpc_s_comm_failure)) {
            SET_STATUS(status, sec_login_s_override_failure);
            rpc_binding_free(&handle, &lst);
            return false;
        } else {
            rpc_binding_free(&handle, &lst);
            return retval;
        }
    } else {
        if (handle)
            rpc_binding_free(&handle, &lst);
        return false;
    }
}


    /*
     * override_get_group_info
     *      Lookup the group in the group_override file, by a combination
     *      of name, gid, and if found, return any overridden information.
     * Errors:
     *      COMMUNICATIONS ERROR
     *      sec_login_s_no_override_info
     *      sec_login_s_override_failure
     *      sec_login_s_ovrd_ent_not_found
     */

void override_get_group_info
#ifndef __STDC__
    (gr_name, max_members, gr_gid, member_cursor, gr_passwd, member_list, number_supplied, number_members,
     overridden, status)
    unsigned_char_t		*gr_name;
    signed32			max_members;
    signed32			*gr_gid;
    sec_rgy_cursor_t		*member_cursor;
    sec_rgy_unix_passwd_buf_t	gr_passwd;
    sec_rgy_member_t		member_list[];
    signed32			*number_supplied;
    signed32			*number_members;
    sec_group_override_fields_t	*overridden;
    error_status_t		*status;
#else
    (
    unsigned_char_t		*gr_name,
    signed32			max_members,
    signed32			*gr_gid,
    sec_rgy_cursor_t		*member_cursor,
    sec_rgy_unix_passwd_buf_t	gr_passwd,
    sec_rgy_member_t		member_list[],
    signed32			*number_supplied,
    signed32			*number_members,
    sec_group_override_fields_t	*overridden,
    error_status_t		*status
    )
#endif

{
    boolean32 retval;
    rpc_binding_handle_t handle = NULL;
    error_status_t lst;

    CLEAR_STATUS(status);

    /* bind to sec_clientd, if necessary */
    sec_login_util_bind_to_sec_clientd(&handle, status);
    if (STATUS_EQUAL(status, sec_login_s_config))
        SET_STATUS(status, sec_login_s_no_override_info);

    if (GOOD_STATUS(status)) {
        (*roverride_v1_0_c_epv.roverride_get_group_info)
           (handle, gr_name, max_members, gr_gid, member_cursor, gr_passwd, 
		     member_list, number_supplied, number_members, overridden, status);
        if (STATUS_EQUAL(status, rpc_s_comm_failure)) {
            SET_STATUS(status, sec_login_s_override_failure);
        }
        rpc_binding_free(&handle, &lst);
    } else {
        if (handle)
            rpc_binding_free(&handle, &lst);
    }
}

   /* 
    *	override_check_group_passwd
    *		Check the specified passwd against the overriden passwd
    *		in the override_group_buffer.
    *	
    * Errors:
    *      COMMUNICATIONS ERROR
    *      sec_login_s_no_override_info
    *      sec_login_s_override_failure
    *      sec_login_s_ovrd_ent_not_found
    *	
    */

boolean32 override_check_group_passwd 
#ifndef __STDC__
    (gr_name, gr_gid, gr_passwd, status)
    unsigned_char_t		*gr_name;
    signed32			*gr_gid;
    sec_rgy_unix_passwd_buf_t	gr_passwd;
    error_status_t		*status;
#else
    (
    unsigned_char_t		*gr_name,
    signed32			*gr_gid,
    sec_rgy_unix_passwd_buf_t	gr_passwd,
    error_status_t		*status
    )
#endif
{
    boolean32 retval;
    rpc_binding_handle_t handle = NULL;
    error_status_t lst;

    CLEAR_STATUS(status);

    /* bind to sec_clientd, if necessary */
    sec_login_util_bind_to_sec_clientd(&handle, status);
    if (STATUS_EQUAL(status, sec_login_s_config))
        SET_STATUS(status, sec_login_s_no_override_info);

    if (GOOD_STATUS(status)) {
        retval = (*roverride_v1_0_c_epv.roverride_check_group_passwd)
                    (handle, gr_name, gr_gid, gr_passwd,status);
        if (STATUS_EQUAL(status, rpc_s_comm_failure)) {
            SET_STATUS(status, sec_login_s_override_failure);
            rpc_binding_free(&handle, &lst);
            return false;
        } else {
            rpc_binding_free(&handle, &lst);
            return retval;
        }
    } else {
        if (handle)
            rpc_binding_free(&handle, &lst);
        return false;
    }
}

   /* 
    *	override_is_grp_pwd_overridden
    *		Lookup the group name in the group_override file.
    *		Return true of passwd overridden.
    *	
    * Errors:
    *      COMMUNICATIONS ERROR
    *      sec_login_s_no_override_info
    *      sec_login_s_override_failure
    *      sec_login_s_ovrd_ent_not_found
    *	
    */

boolean32 override_is_grp_pwd_overridden
#ifndef __STDC__
    (gr_name, gr_gid, gr_salt, status)
    unsigned_char_t		*gr_name;
    signed32			*gr_gid;
    sec_rgy_unix_passwd_buf_t	gr_salt;
    error_status_t		*status;
#else
    (
    unsigned_char_t		*gr_name,
    signed32			*gr_gid,
    sec_rgy_unix_passwd_buf_t	gr_salt,
    error_status_t		*status
    )
#endif
{
    boolean32 retval;
    rpc_binding_handle_t handle = NULL;
    error_status_t lst;

    CLEAR_STATUS(status);

    /* bind to sec_clientd, if necessary */
    sec_login_util_bind_to_sec_clientd(&handle, status);
    if (STATUS_EQUAL(status, sec_login_s_config))
        SET_STATUS(status, sec_login_s_no_override_info);

    if (GOOD_STATUS(status)) {
        retval = (*roverride_v1_0_c_epv.roverride_is_grp_pwd_overridden)
                    (handle, gr_name, gr_gid, gr_salt,status);
        if (STATUS_EQUAL(status, rpc_s_comm_failure)) {
            SET_STATUS(status, sec_login_s_override_failure);
            rpc_binding_free(&handle, &lst);
            return false;
        } else {
            rpc_binding_free(&handle, &lst);
            return retval;
        }
    } else {
        if (handle)
            rpc_binding_free(&handle, &lst);
        return false;
    }
}

