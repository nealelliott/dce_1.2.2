/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sv_rover.c,v $
 * Revision 1.1.4.2  1996/02/18  19:16:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:49  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:28:01  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/10/17  21:33 UTC  bartf
 * 	Makefile
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/04  11:28 UTC  sekhar
 * 	Merged to mothra.
 * 
 * 	HP revision /main/HPDCE02/sekhar_mothra_fix/3  1995/05/01  12:57 UTC  sekhar
 * 	[ CHFts15111 ] Fix (incorrect) loop for group overrides too.
 * 
 * 	HP revision /main/HPDCE02/sekhar_mothra_fix/2  1995/05/01  10:52 UTC  sekhar
 * 	[ CHFts15111 ] Fix (incorrect) loop which inserts override entries into a list.
 * 	n ordered list.
 * 
 * 	HP revision /main/HPDCE02/sekhar_mothra_fix/1  1995/04/28  12:46 UTC  sekhar
 * 	[ CHFts15111 ]  Fix the order in which override fields are processed for
 * 	                passwd_override and group_override as well.
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/30  20:15 UTC  mdf
 * 	Fix defect.
 * 
 * 	HP revision /main/mdf_MOTHRA_3/1  1995/01/30  15:09 UTC  mdf
 * 	Correction for CHFts14193.  When the riverride_get_group_info call was
 * 	moved from roverride.c to here, the functional test was failing
 * 	due to the change in the logic when checking for number_members.
 * 	[1995/12/08  18:00:05  root]
 * 
 * Revision 1.1.2.9  1994/09/29  13:42:03  hasbrouc
 * 	Fix group override support that broke when this code moved
 * 	from roverride.c and was cleaned up
 * 	[1994/09/29  13:40:57  hasbrouc]
 * 
 * Revision 1.1.2.8  1994/09/22  22:00:38  bowe
 * 	SVC work [CR 11725]
 * 	[1994/09/22  21:43:13  bowe]
 * 
 * Revision 1.1.2.7  1994/07/06  20:31:34  rsalz
 * 	Rewrite parsing.
 * 	Rename almost all datatypes and internal functions for consistency.
 * 	Add locking (OT CR 11174).
 * 	[1994/07/06  19:46:10  rsalz]
 * 
 * Revision 1.1.2.6  1994/06/13  20:05:40  rsalz
 * 	Code cleanup.
 * 	Server registration done in main now.
 * 	[1994/06/13  20:05:07  rsalz]
 * 
 * Revision 1.1.2.5  1994/06/09  15:23:53  rsalz
 * 	Remove static declaration from some variables (OT CR 10912).
 * 	[1994/06/09  15:22:27  rsalz]
 * 
 * Revision 1.1.2.4  1994/06/06  19:45:00  bowe
 * 	Merge in group override code. [CR 9768]
 * 	[1994/06/06  19:43:22  bowe]
 * 
 * Revision 1.1.2.3  1994/05/06  15:19:45  bowe
 * 	Added operation stubs for group overrides [cr 10556]
 * 	[1994/05/06  15:16:35  bowe]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:11  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:56:08  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:26:01  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:31  bowe]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <dce/uuid.h>
#include <dce/rpc.h>
#include <roverride.h>
#include <dcedimpl.h>
#include <svimpl.h>

#define STRNCPY(dest, src) \
	strncpy((char *)(dest), (src), sizeof (dest)), \
	(dest)[sizeof (dest) - 1] = '\0'

    /* Sort key in order of importance. */
#define sec_weight_none		0
#define sec_weight_gid		1
#define sec_weight_uid		2
#define sec_weight_name		3

typedef struct user_entry_s_t {
    struct user_entry_s_t	*next;
    sec_override_fields_t	overridden;
    int				weight;
    sec_rgy_name_t		name;
    sec_rgy_unix_passwd_buf_t	passwd;
    signed32			uid;
    signed32			gid;
    sec_rgy_name_t		gecos;
    sec_rgy_name_t		dir;
    sec_rgy_name_t		shell;
} user_entry_t;

typedef struct group_member_s_t {
    struct group_member_s_t	*next;
    sec_rgy_name_t		member;
} group_member_t;

typedef struct group_entry_s_t {
    struct group_entry_s_t	*next;
    sec_group_override_fields_t	overridden;
    int				weight;
    sec_rgy_name_t		name;
    sec_rgy_unix_passwd_buf_t	passwd;
    signed32			gid;
    group_member_t		*members;
} group_entry_t;

static user_entry_t		*user_list;
static group_entry_t		*group_list;
static time_t			user_time;
static time_t			group_time;
static dce_lock_t		user_lock;
static dce_lock_t		group_lock;


static int
errno2status(
    int		e
)
{
    switch (e) {
    case ENOENT:
	return sec_login_s_no_override_info;
    }
    return sec_login_s_override_failure;
}


static boolean32
not_numeric(
    char	*p
)
{
    for ( ; *p; p++)
	if (!isdigit(*p))
	    return TRUE;
    return FALSE;
}


/*
**  Split a line into colon-separated fields.
*/
static int
split(
    char	*p,
    char	**fields,
    int		count
)
{
    int		i;

    for (i = 0, fields[i++] = p; *p; p++)
	if (*p == ':') {
	    *p = '\0';
	    fields[i++] = p + 1;
	    if (i > count)
		return count + 1;
	}
    return i;
}


static void
free_user_list(
    void
)
{
    user_entry_t	*next;

    for (; user_list != NULL; user_list = next) {
	next = user_list->next;
	free(user_list);
    }
}


static void
free_group_list(
    void
)
{
    group_entry_t	*next;
    group_member_t	*m;
    group_member_t	*next_m;

    for (; group_list != NULL; group_list = next) {
	if (group_list->members != NULL) {
	    for (m = group_list->members; m != NULL; m = next_m) {
		next_m = m->next;
		free(m);
	    }
	}
	next = group_list->next;
	free(group_list);
    }
}


/*
 *  Parse override buffer of the form:
 *	[principal]:[passwd]:[unix_id]:[grp_id]:[GECOS]:[home_dir]:[shell]
 *  Where each field is optional, but all 6 separators (:) must exist.
 */
static boolean32
parse_user_entry(
    char			*buffer
)
{
    static user_entry_t		niluser =
	{ NULL, sec_override_none, sec_weight_none };
    user_entry_t		*new;
    user_entry_t		*next;
    user_entry_t                *prev;
    char			*fields[7];

    if (split(buffer, fields, 7) != 7
     || not_numeric(fields[2]) || not_numeric(fields[3]))
	return FALSE;

    new = malloc(sizeof *new);
    *new = niluser;
    if (fields[0][0]) {
	STRNCPY(new->name, fields[0]);
	new->weight = sec_weight_name;
    }
    if (fields[1][0]) {
	STRNCPY(new->passwd, fields[1]);
	new->overridden |= sec_override_pw_passwd;
    }
    if (fields[2][0]) {
	new->uid = atoi(fields[2]);
	if (new->weight == sec_weight_none)
	    new->weight = sec_weight_uid;
	else
	    new->overridden |= sec_override_pw_uid;
    }
    if (fields[3][0]) {
	new->gid = atoi(fields[3]);
	if (new->weight == sec_weight_none)
	    new->weight = sec_weight_gid;
	else
	    new->overridden |= sec_override_pw_gid;
    }
    if (fields[4][0]) {
	STRNCPY(new->gecos, fields[4]);
	new->overridden |= sec_override_pw_gecos;
    }
    if (fields[5][0]) {
	STRNCPY(new->dir, fields[5]);
	new->overridden |= sec_override_pw_dir;
    }
    if (fields[6][0]) {
	STRNCPY(new->shell, fields[6]);
	new->overridden |= sec_override_pw_shell;
    }

    if (new->weight == sec_weight_none
     || new->overridden == sec_override_none) {
	 /* Don't bother */
	free(new);
	return TRUE;
    }

    /* Add to the list. */
    if (user_list == NULL)
	user_list = new;
    else {
	for (next = user_list; next != NULL; prev=next , next = next->next) {
	    /* Insert if more specific */
	    if (new->weight > next->weight)
		break;
        }

        if (next == NULL) {        /* Insert at the end of list */
            prev->next = new ; 
        }
        else {
            new->next  = next ;
            if (next == user_list) /* Insert at beginning of list */
                user_list  = new;
            else                   /* Insert in middle of list */
                prev->next = new  ;
        }
    }
    return TRUE;
}


static void
parse_user_file(
    FILE		*F
)
{
    char		buff[1024];
    int			i;

    while (fgets(buff, sizeof buff, F) != NULL) {
	if ((i = strlen(buff)) > 0 && buff[--i] == '\n')
	    buff[i] = '\0';
	if (!parse_user_entry(buff)) {
	    dce_svc_printf(DCED_S_SV_BAD_OVERRIDE_ENTRY_MSG, buff);
	}
    }
    fclose(F);
}

/*
**  Parse group override buffer of the form:
**	[group_name]:[passwd]:[group_uid]:[member [,member...]]
**  Where each field is optional, but all 3 separators (:) must exist.
*/
static boolean32
parse_group_entry(
    char		*buffer
)
{
    static group_entry_t nilgroup =
	{ NULL, sec_override_none, sec_weight_none };
    char		*p;
    char		*member;
    boolean32		done;
    group_entry_t	*new;
    group_entry_t	*next;
    group_entry_t       *prev;
    group_member_t	*new_m;
    group_member_t	*next_m;
    char		*fields[4];

    if (split(buffer, fields, 4) != 4 || (!strlen(fields[0]) && !strlen(fields[2])) || not_numeric(fields[2]))
	return FALSE;

    new = malloc(sizeof *new);
    *new = nilgroup;
    if (fields[0][0]) {
	STRNCPY(new->name, fields[0]);
	new->weight = sec_weight_name;
    }
    if (fields[1][0]) {
	STRNCPY(new->passwd, fields[1]);
	new->overridden |= sec_group_override_gr_passwd;
    }
    if (fields[2][0]) {
	new->gid = atoi(fields[2]);
	if (new->weight == sec_weight_none)
	    new->weight = sec_weight_gid;
	else
	    new->overridden |= sec_group_override_gr_gid;
    }
    p = fields[3];
    if (p[0]) {
	new->overridden |= sec_group_override_member;
	if (p[0] != '*') {
	    /* Parse comma-separated member list. */
	    for (done = FALSE; !done; ) {
		for (member = p; *p && *p != ','; p++)
		    continue;
		if (*p == '\0')
		    done = TRUE;
		if (*member) {
		    *p = '\0';
		    new_m = malloc(sizeof *new_m);
		    new_m->next = NULL;
		    STRNCPY(new_m->member, member);
		    if (new->members == NULL)
			new->members = new_m;
		    else
			next_m->next = new_m;
		    next_m = new_m;
		    p++;
		}
	    }
	}
    }

    if (new->weight == sec_weight_none
     || new->overridden == sec_group_override_none) {
	/* Don't bother */
	free(new);
	return TRUE;
    }

    /* List manipulation */
    if (group_list == NULL)
	group_list = new;
    else {
	for (next = group_list; next != NULL; prev=next , next = next->next) {
	    /* Insert if more specific */
	    if (new->weight > next->weight)
		break;
        }

        if (next == NULL)           /* Insert at the end of list */
            prev->next = new ; 
        else {
            new->next  = next ;
            if (next == group_list) /* Insert at beginning of list */
                group_list  = new;
            else                    /* Insert in middle of list */
                prev->next = new  ;
        }
    }
    return TRUE;
}


static void
parse_group_file(
    FILE		*F
)
{
    char		buff[2048];
    int			i;

    while (fgets(buff, sizeof buff, F) != NULL) {
	if ((i = strlen(buff)) > 0 && buff[--i] == '\n')
	    buff[i] = '\0';
	if (!parse_group_entry(buff)) {
	    dce_svc_printf(DCED_S_SV_BAD_OVERRIDE_ENTRY_MSG, buff);
	}
    }
    fclose(F);
}


/*
**  If user override file exists, make sure the override list is current.
*/
static boolean32
check_user_file(
    error_status_t	*st
)
{
    struct stat		Sb;
    FILE		*F;
    error_status_t	st2;

    dce_lock_justwrite(user_lock, st);

    if (stat(sv_override_file_name, &Sb) < 0) {
	*st = errno2status(errno);
	free_user_list();
	dce_lock_unlock(user_lock, &st2);
	return FALSE;
    }

    /* If update time is older than file modified time, reprocess it */
    if (user_time < Sb.st_mtime) {
	free_user_list();
	if ((F = fopen(sv_override_file_name, "r")) == NULL) {
	    *st = errno2status(errno);
	    dce_lock_unlock(user_lock, &st2);
	    return FALSE;
	}
	user_time = Sb.st_mtime;
	parse_user_file(F);
    }

    dce_lock_unlock(user_lock, st);
    return user_list != NULL;
}


/*
**  If group override file exists, make sure the override list is current.
*/
static boolean32
check_group_file(
    error_status_t	*st
)
{
    struct stat		Sb;
    FILE		*F;
    error_status_t	st2;

    dce_lock_justwrite(group_lock, st);

    if (stat(sv_group_override_file_name, &Sb) < 0) {
	*st = errno2status(errno);
	free_group_list();
	dce_lock_unlock(group_lock, &st2);
	return FALSE;
    }

    /* If update time is older than file modified time, reprocess it */
    if (group_time < Sb.st_mtime) {
	free_group_list();
	if ((F = fopen(sv_group_override_file_name, "r")) == NULL) {
	    *st = errno2status(errno);
	    dce_lock_unlock(group_lock, &st2);
	    return FALSE;
	}
	group_time = Sb.st_mtime;
	parse_group_file(F);
    }

    dce_lock_unlock(group_lock, st);
    return group_list != NULL;
}

/*
**  PUBLIC INTERFACES
*/


/*
**  Initial load of the override files.
*/
void
load_override_files(
    error_status_t	*st
)
{
    FILE		*F;
    struct stat		Sb;

    dce_lock_create(&user_lock, st);
    CHECK_STAT_RET(*st);
    if (stat(sv_override_file_name, &Sb) < 0
     || (F = fopen(sv_override_file_name, "r")) == NULL)
	*st = errno2status(errno);
    else {
	user_time = Sb.st_mtime;
	parse_user_file(F);
    }

    dce_lock_create(&group_lock, st);
    CHECK_STAT_RET(*st);
    if (stat(sv_group_override_file_name, &Sb) < 0
     || (F = fopen(sv_group_override_file_name, "r")) == NULL)
	*st = errno2status(errno);
    else {
	group_time = Sb.st_mtime;
	parse_group_file(F);
    }

    *st = error_status_ok;
}

/*
**  Does the specified account match this override entry?
*/
static boolean32
user_override_match(
    user_entry_t	*user,
    unsigned char	*pw_name,
    signed32		*pw_uid,
    signed32		*pw_gid
)
{
    switch (user->weight) {
    default:
	return FALSE;
    case sec_weight_name:
	if (strcmp((char *)pw_name, (char *)user->name) != 0)
	    return FALSE;
	break;
    case sec_weight_uid:
	if (*pw_uid != user->uid)
	    return FALSE;
	break;
    case sec_weight_gid:
	if (*pw_gid != user->gid)
	    return FALSE;
	break;
    }
    return TRUE;
}


void
roverride_get_login_info(
    handle_t			h,
    unsigned char		*pw_name,
    signed32			*pw_uid,
    signed32			*pw_gid,
    sec_rgy_unix_passwd_buf_t	pw_passwd,
    sec_rgy_name_t		pw_gecos,
    sec_rgy_name_t		pw_dir,
    sec_rgy_name_t		pw_shell,
    sec_override_fields_t	*overridden,
    error_status_t		*st
)
{
    user_entry_t		*user;

    *pw_passwd = '\0';
    *pw_gecos = '\0';
    *pw_dir = '\0';
    *pw_shell = '\0';
    *overridden = sec_override_none;

    if (!check_user_file(st))
	return;

    dce_lock_read(user_lock, st);
    for (user = user_list; user != NULL; user = user->next)
	if (user_override_match(user, pw_name, pw_uid, pw_gid))
	    break;
    if (user == NULL) {
	dce_lock_unlock(user_lock, st);
	*st = sec_login_s_ovrd_ent_not_found;
	return;
    }

    strcpy((char *)pw_passwd, (char *)user->passwd);
    strcpy((char *)pw_gecos, (char *)user->gecos);
    strcpy((char *)pw_dir, (char *)user->dir);
    strcpy((char *)pw_shell, (char *)user->shell);
    if (user->overridden & sec_override_pw_uid)
	*pw_uid = (signed32)user->uid;
    if (user->overridden & sec_override_pw_gid)
	*pw_gid = (signed32)user->gid;
    *overridden = user->overridden;
    dce_lock_unlock(user_lock, st);
}


boolean32
roverride_check_passwd(
    handle_t			h,
    unsigned char		*pw_name,
    signed32			*pw_uid,
    signed32			*pw_gid,
    sec_rgy_unix_passwd_buf_t	pw_passwd,
    error_status_t		*st
)
{
    user_entry_t		*user;

    if (!check_user_file(st))
	return FALSE;

    dce_lock_read(user_lock, st);
    for (user = user_list; user != NULL; user = user->next)
	if (user->weight == sec_weight_name
	 && (user->overridden & sec_override_pw_passwd)
	 && strcmp((char *)pw_name, (char *)user->name) == 0)
	    break;
    if (user == NULL) {
	dce_lock_unlock(user_lock, st);
	*st = sec_login_s_ovrd_ent_not_found;
	return FALSE;
    }

    /* overridden passwd matched? */
    if (strcmp((char *)pw_passwd, (char *)user->passwd) != 0) {
	dce_lock_unlock(user_lock, st);
	*st = sec_login_s_invalid_password;
	return FALSE;
    }

    dce_lock_unlock(user_lock, st);
    return TRUE;
}


boolean32
roverride_is_passwd_overridden(
    handle_t			h,
    unsigned char		*pw_name,
    signed32			*pw_uid,
    signed32			*pw_gid,
    sec_rgy_unix_passwd_buf_t	pw_salt,
    error_status_t		*st
)
{
    user_entry_t		*user;

    pw_salt[0] = '\0';

    if (!check_user_file(st))
	return FALSE;

    dce_lock_read(user_lock, st);
    for (user = user_list; user != NULL; user = user->next)
	if (user->weight == sec_weight_name
	 && (user->overridden & sec_override_pw_passwd)
	 && strcmp((char *)pw_name, (char *)user->name) == 0)
	    break;
    if (user == NULL) {
	dce_lock_unlock(user_lock, st);
	return FALSE;
    }

    /* Only need to pass back the salt */
    pw_salt[0] = user->passwd[0];
    pw_salt[1] = user->passwd[1];
    pw_salt[2] = '\0';
    dce_lock_unlock(user_lock, st);
    return TRUE;
}


/*
**  Search a name domain for a specified numeric id.  Return FALSE if
**  not found or domain is sec_rgy_domain_org.  For sec_rgy_domain_person
**  look for Unix uid and return user name.  For sec_rgy_domain_group,
**  look group Unix gid and return group name.
*/
boolean32
roverride_get_by_unix_num(
    handle_t		h,
    sec_rgy_domain_t	name_domain,
    signed32		unix_id,
    sec_rgy_name_t	name,
    error_status_t	*st
)
{
    user_entry_t	*user;
    group_entry_t	*group;
    boolean32		found;

    found = FALSE;
    switch ((int)name_domain) {
    default:
	*st = sec_login_s_internal_error;
	return FALSE;
    case sec_rgy_domain_org:
	*st = error_status_ok;
	return FALSE;
    case sec_rgy_domain_person:
	if (check_user_file(st)) {
	    dce_lock_read(user_lock, st);
	    for (user = user_list; user; user = user->next)
		if (unix_id == user->uid) {
		    if (user->weight == sec_weight_name) {
			strcpy((char *)name, (char *)user->name);
			found = TRUE;
		    }
		    break;
		}
	    dce_lock_unlock(user_lock, st);
	}
	break;
    case sec_rgy_domain_group:
	if (check_group_file(st)) {
	    dce_lock_read(group_lock, st);
	    for (group = group_list; group; group = group->next)
		if (unix_id == group->gid) {
		    if (group->weight == sec_weight_name) {
			strcpy((char *)name, (char *)group->name);
			found = TRUE;
		    }
		    break;
		}
	    dce_lock_unlock(group_lock, st);
	}
	break;
    }

    return found;
}

/*
**  Does the specified group match this override entry?
*/
static boolean32
group_override_match(
    group_entry_t	*group,
    unsigned char	*gr_name,
    signed32		*gr_gid
)
{
    switch (group->weight) {
    default:
	return FALSE;
    case sec_weight_name:
	if (strcmp((char *)gr_name, (char *)group->name) != 0)
	    return FALSE;
	break;
    case sec_weight_gid:
	if (*gr_gid != group->gid)
	    return FALSE;
	break;
    }
    return TRUE;
}


/*
**  Lookup the group in the group override list.
*/
void
roverride_get_group_info(
    handle_t			h,
    unsigned_char_t		*gr_name,
    signed32			size,
    signed32			*gr_gid,
    sec_rgy_cursor_t		*cursor,
    sec_rgy_unix_passwd_buf_t	gr_passwd,
    sec_rgy_member_t		member_list[],
    signed32			*number_supplied,
    signed32			*number_members,
    sec_group_override_fields_t	*overridden,
    error_status_t		*st
)
{
    group_entry_t		*group;
    group_member_t		*m;
    int				i;
    int				count;

    *gr_passwd = '\0';
    *overridden = sec_override_none;
    *number_members = 0;
    *number_supplied = 0;

    if (!check_group_file(st))
	return;

    dce_lock_read(group_lock, st);
    for (group = group_list; group != NULL; group = group->next)
	if (group_override_match(group, gr_name, gr_gid)) {
	    strcpy((char *)gr_passwd, (char *)group->passwd);
	    if (group->overridden & sec_group_override_gr_gid)
		*gr_gid = (signed32)group->gid;
	    *overridden = group->overridden;
	    break;
	}
    if (group == NULL) {
	dce_lock_unlock(group_lock, st);
	*st = sec_login_s_ovrd_ent_not_found;
	return;
    }

    /* Parse the members. */
    for (m = group->members; m != NULL; m = m->next) {
	(*number_members)++;
    }
    if (*number_members) {
	if (cursor->valid) {
	    if (cursor->handle >= *number_members) {
		dce_lock_unlock(group_lock, st);
		*st = sec_rgy_no_more_entries;
		return;
	    }
	    count = cursor->handle;
	}
	else {
	    cursor->handle = 0;
	    count = 0;
	}
	for (m = group->members, i = 0; i != count && m != NULL; i++)
	    m = m->next;
	for (i = 0; i < size && m != NULL; m = m->next, cursor->handle++) {
	    strcpy((char *)member_list[i++], (char *)m->member);
	}
	if (i != size && cursor->handle == *number_members)
	    cursor->valid = FALSE;
	else
	    cursor->valid = TRUE;
	*number_supplied = i;
    }

    dce_lock_unlock(group_lock, st);
}

/*
**  Check the specified passwd against the group override list.
*/
boolean32
roverride_check_group_passwd(
    handle_t			h,
    unsigned_char_t		*gr_name,
    signed32			*gr_gid,
    sec_rgy_unix_passwd_buf_t	gr_passwd,
    error_status_t		*st
)
{
    group_entry_t		*group;

    if (!check_group_file(st))
	return FALSE;

    dce_lock_read(group_lock, st);
    for (group = group_list; group != NULL; group = group->next)
	if (group->weight == sec_weight_name
	 && (group->overridden & sec_group_override_gr_passwd)
	 && strcmp((char *)gr_name, (char *)group->name) == 0)
	    break;
    if (group == NULL) {
	dce_lock_unlock(group_lock, st);
	*st = sec_login_s_ovrd_ent_not_found;
	return FALSE;
    }

    if (strcmp((char *)gr_passwd, (char *)group->passwd) != 0) {
	*st = sec_login_s_invalid_password;
	dce_lock_unlock(group_lock, st);
	return FALSE;
    }

    dce_lock_unlock(group_lock, st);
    return TRUE;
}


/*
**  Does the named group have an overridden password?
*/
boolean32
roverride_is_grp_pwd_overridden(
    handle_t			h,
    unsigned char		*gr_name,
    signed32			*gr_gid,
    sec_rgy_unix_passwd_buf_t	gr_salt,
    error_status_t		*st
)
{
    group_entry_t		*group;

    gr_salt[0] = '\0';
    if (!check_group_file(st))
	return FALSE;

    dce_lock_read(group_lock, st);
    for (group = group_list; group != NULL; group = group->next)
	if (group->weight == sec_weight_name
	 && (group->overridden & sec_group_override_gr_passwd)
	 && strcmp((char *)gr_name, (char *)group->name) == 0)
	    break;
    if (group == NULL) {
	dce_lock_unlock(group_lock, st);
	*st = sec_login_s_ovrd_ent_not_found;
	return FALSE;
    }

    /* Only need to pass back the salt */
    gr_salt[0] = group->passwd[0];
    gr_salt[1] = group->passwd[1];
    gr_salt[2] = '\0';
    *gr_gid = (group->overridden & sec_group_override_gr_gid) != 0
	    ? group->gid : 0;
    dce_lock_unlock(group_lock, st);
    return TRUE;
}
