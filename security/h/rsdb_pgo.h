/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_pgo.h,v $
 * Revision 1.1.46.1  1996/05/10  13:17:27  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:46 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/3  1996/02/21  15:39 UTC  hanfei
 * 	rename  rsdb_pgo_get_members_foreign_groupsets and its args
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/08  19:19 UTC  hanfei
 * 	take out parameter domain from rsdb_pgo_get_members_foreign_groupsets().
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/02  19:27 UTC  hanfei
 * 	multi-cell group work: add additional parameter cell_uuid to
 * 	rsdb_pgo_add() and add function header
 * 	for rsdb_pgo_get_members_foreign_groupsets().
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:11 UTC  psn
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	[1995/07/13  20:09 UTC  burati  /main/HPDCE02/1]
 *
 * 	Alias ACL bug work
 * 	[1995/07/13  20:07 UTC  burati  /main/mb_mothra5/1]
 *
 * Revision 1.1.39.1  1993/10/05  22:30:23  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:10:07  mccann]
 * 
 * Revision 1.1.2.4  1993/02/26  23:48:41  emartin
 * 	em_bl6f: decl rsdb_pgo_clear
 * 	[1993/02/22  22:50:16  emartin]
 * 
 * Revision 1.1.2.3  1992/12/29  13:26:22  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:37  zeliff]
 * 
 * Revision 1.1.2.2  1992/08/31  18:20:09  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:58:13  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  rsdb_pgo.h
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - Person, Group, Org Naming management - function defs
 */

#ifndef rsdb_pgo_h__included
#define rsdb_pgo_h__included

/*
 *Include base datatype definitions
 */

#include <dce/nbase.h>
#include <dce/rgynbase.h>

#include <rsdb_pvt.h>

/*
 * Prototypes - public routines
 */

void rsdb_pgo_create(
    sec_id_t                *rgy_creator,
    sec_id_t                *rgy_local_cell,
    sec_rgy_properties_t    *properties
);

void rsdb_pgo_init(
    void
);

void rsdb_pgo_clear(
    void
);

void export_pgo (
    sec_rgy_domain_t       domain,
    sec_rgy_name_t         name,
    sec_rgy_pgo_item_t     *item,
    rsdb_pgo_item_t        *pgo
);

void rsdb_pgo_add (
    sec_rgy_domain_t        domain,     /* [in] */
    sec_rgy_name_t          name,       /* [in] */
    sec_rgy_pgo_item_t      *(item),    /* [in] */
    uuid_t                  *cell_id,   /* [in] */
    sec_rgy_foreign_id_t    *client_ids,/* [in] */
    boolean32               alias_fix,  /* [in] */
    boolean32               *prop_acl,  /* [out] */
    error_status_t          *(status)
);

void rsdb_pgo_delete (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      name,       /* [in] */
    error_status_t      *(status)
);

void rsdb_pgo_replace_internal (
    sec_rgy_domain_t    domain,     /* [in] */
    rsdb_named_item_t   *pgo        /* [in] */
);

void rsdb_pgo_replace (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      name,       /* [in] */
    sec_rgy_pgo_item_t  *(item),    /* [in] */
    error_status_t      *(status)
);

void rsdb_pgo_rename (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      old_name,   /* [in] */
    sec_rgy_name_t      new_name,   /* [in] */
    error_status_t      *(status)
);

void rsdb_pgo_move (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      old_name,   /* [in] */
    sec_rgy_name_t      new_name,   /* [in] */
    error_status_t      *(status)
);

void rsdb_pgo_uid_of_named_item (
    sec_rgy_domain_t    domain,     /* [in] */
    rsdb_named_item_t   *named,     /* [in] */
    uuid_t              *(uid)      /* [out] */
);

void rsdb_pgo_get_by_name (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      name,       /* [in] */
    sec_rgy_cursor_t    *(cursor),  /* [in, out] */
    sec_rgy_pgo_item_t  *(item),    /* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_get_by_uid (
    sec_rgy_domain_t    domain,        /* [in] */
    uuid_t              *(uid),        /* [in] */
    boolean32           allow_aliases, /* [in] */
    sec_rgy_cursor_t    *(cursor),     /* [in, out] */
    sec_rgy_name_t      name,          /* [out] */
    sec_rgy_pgo_item_t  *(item),       /* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_get_by_unix_num (
    sec_rgy_domain_t    domain,        /* [in] */
    long                unix_num,      /* [in] */
    boolean32           allow_aliases, /* [in] */
    sec_rgy_cursor_t    *(cursor),     /* [in, out] */
    sec_rgy_name_t      name,          /* [out] */
    sec_rgy_pgo_item_t  *(item),       /* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_get_next_id (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_cursor_t    *(cursor),  /* [in, out] */
    sec_rgy_name_t      name,       /* [out] */
    sec_rgy_pgo_item_t  *(item),    /* [out] */
    rsdb_pvt_id_t       *id,        /* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_get_next (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_cursor_t    *(cursor),  /* [in, out] */
    sec_rgy_name_t      name,       /* [out] */
    sec_rgy_pgo_item_t  *(item),    /* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_get_prev_marker (
    sec_rgy_domain_t   domain,         /* [in] */
    sec_rgy_cursor_t   *(cursor),      /* [in] */
    sec_rgy_cursor_t   *(prev_cursor)
);

void rsdb_pgo_name_to_uid (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      name,       /* [in] */
    uuid_t              *(uid),     /* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_name_to_unix_num (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      name,       /* [in] */
    long                *(unix_num),/* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_uid_to_name (
    sec_rgy_domain_t    domain,     /* [in] */
    uuid_t              *(uid),     /* [in] */
    sec_rgy_name_t      name,       /* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_unix_num_to_name (
    sec_rgy_domain_t    domain,     /* [in] */
    long                unix_num,   /* [in] */
    sec_rgy_name_t      name,       /* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_uid_to_unix_num (
    sec_rgy_domain_t    domain,     /* [in] */
    uuid_t              *(uid),     /* [in] */
    long                *(unix_num),/* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_unix_num_to_uid (
    sec_rgy_domain_t    domain,     /* [in] */
    long                unix_num,   /* [in] */
    uuid_t              *(uid),     /* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_add_member (
    sec_rgy_domain_t    domain,         /* [in] */
    sec_rgy_name_t      go_name,        /* [in] */
    sec_rgy_name_t      person_name,    /* [in] */
    error_status_t      *(status)
);

void rsdb_pgo_delete_member (
    sec_rgy_domain_t    domain,         /* [in] */
    sec_rgy_name_t      go_name,        /* [in] */
    sec_rgy_name_t      person_name,    /* [in] */
    error_status_t      *(status)
);

void rsdb_pgo_get_members (
    boolean32           unix_semantics,      /* [in] */
    sec_rgy_domain_t    domain,              /* [in] */
    sec_rgy_name_t      go_name,             /* [in] */
    sec_rgy_cursor_t    *(member_cursor),    /* [in, out] */
    signed32            max_members,         /* [in] */
    sec_rgy_member_t    member_list[],       /* [out] */
    signed32            *(number_supplied),  /* [out] */
    signed32            *(number_members),   /* [out] */
    error_status_t      *(status)
);

boolean32 rsdb_pgo_is_member (
    sec_rgy_domain_t    domain,         /* [in] */
    sec_rgy_name_t      go_name,        /* [in] */
    sec_rgy_name_t      person_name,    /* [in] */
    error_status_t      *(status)       /* [out] */
);

void  rsdb_pgo_get_foreign_groupsets (
    boolean32           unix_semantics,      /* [in] */
    uuid_t              *cell_uuid,          /* [in] */
    uuid_t              *princ_uuid,         /* [in] */
    sec_rgy_cursor_t    *(group_cursor),     /* [in, out] */
    signed32            max_groups,          /* [in] */
    sec_rgy_member_t    group_list[],        /* [out] */
    signed32            *(number_supplied),  /* [out] */
    signed32            *(number_groups),    /* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_policy_get_info (
    sec_rgy_name_t      organization,   /* [in] */
    sec_rgy_plcy_t      *(policy_data), /* [out] */
    error_status_t      *(status)
);

void rsdb_pgo_policy_set_info (
    sec_rgy_name_t      organization,   /* [in] */
    sec_rgy_plcy_t      *(policy_data), /* [in] */
    error_status_t      *(status)
);

#endif /* rsdb_pgo_h__included */
