/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: aclimpl.h,v $
 * Revision 1.1.4.2  1996/02/18  23:32:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:07:15  root]
 * 
 * Revision 1.1.2.1  1994/04/26  19:11:30  bowe
 * 	Initial checkin (was part of dceaclimpl.c, now defunct)
 * 	[1994/04/26  19:09:08  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <dce/dce.h>
#include <dce/uuid.h>
#include <dce/dce_msg.h>
#include <dce/dbif.h>
#include <dce/aclif.h>
#include <dce/binding.h>

#define MAX_PERMISSIONS			(~(sec_acl_permset_t)0)

/* Private ACL management structure */
typedef struct {
    uuid_t			mgr_type;
    dce_db_handle_t		db;			/* from dce_db_open() */
    unsigned32			printstring_size;
    sec_acl_printstring_t	*printstring;
    sec_acl_printstring_t	*mgr_info;
    dce_acl_resolve_func_t	resolver;		/* caller's function */
    void			*resolver_arg;		/* function's  arg */
    sec_acl_permset_t		control_perm;		/* perm for control */
    sec_acl_permset_t		test_perm;		/* perm for test */
    sec_acl_permset_t		max_perm;		/* max perm bits */
    boolean			orphans_ok;		/* C bit required? */
    boolean			has_owner;		/* owner? */
    boolean			has_groups;		/* groups? */
} acl_mgt_t;


#define STAT_CHECK_RET(st) { if (st != error_status_ok) return; }
#define STAT_CHECK_RET_FALSE(st) { if (st != error_status_ok) return FALSE; }

/* ACL lib private function prototypes */
acl_mgt_t *dce_acl__get_info_from_type(uuid_t *, error_status_t *);

