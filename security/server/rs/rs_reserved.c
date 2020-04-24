/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_reserved.c,v $
 * Revision 1.1.20.3  1996/10/03  15:11:07  arvind
 * 	Merge
 * 	[1996/08/14  17:33 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 *
 * 	change some ERA permset value
 * 	[1996/08/05  18:22 UTC  cuti  /main/DCE_1.2.2/2]
 *
 * 	Secd set 1.2.2' schange.
 * 	[1996/07/31  15:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_migration/1]
 *
 * 	Add pk schema
 * 	[1996/05/24  23:16 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/09/07  14:43 UTC  aha  /main/HPDCE02/5]
 *
 * 	CHFts16061: allow cell administrator to
 * 	change the password on the local host account
 * 	HP 	[1995/08/28  18:54 UTC  aha  /main/HPDCE02/aha_mothra9/1]
 *
 * 	CHFts16061: allow user:<creator> to modify user information
 * 	on the local host account
 * 	HP 	[1995/07/13  21:40 UTC  burati  /main/HPDCE02/4]
 *
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	HP 	[1995/07/13  20:48 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 *
 * 	Alias ACL work
 * 	HP 	[1995/05/15  21:09 UTC  greg  /main/HPDCE02/3]
 *
 * 	Mothra bugs.
 * 	HP 	[1995/03/03  13:03 UTC  mdf  /main/mdf_mothra_defects_0/1]
 * 	[1996/10/03  13:53:45  arvind]
 *
 * Revision 1.1.20.2  1996/08/09  12:10:31  arvind
 * 	Secd set 1.2.2' schange.
 * 	[1996/07/31  15:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_migration/1]
 * 
 * 	Add pk schema
 * 	[1996/05/24  23:16 UTC  sommerfeld  /main/DCE_1.2.2/1]
 * 
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:47 UTC  hanfei  /main/DCE_1.2/2]
 * 
 * 	multi-cell group work: add NULL as cell_uuid for rsdb_pgo_add() call.
 * 	[1996/02/01  16:36 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 * 
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:10  marty]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/09/07  14:43 UTC  aha  /main/HPDCE02/5]
 * 
 * 	CHFts16061: allow cell administrator to
 * 	change the password on the local host account
 * 	HP 	[1995/08/28  18:54 UTC  aha  /main/HPDCE02/aha_mothra9/1]
 * 
 * 	CHFts16061: allow user:<creator> to modify user information
 * 	on the local host account
 * 	HP 	[1995/07/13  21:40 UTC  burati  /main/HPDCE02/4]
 * 
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	HP 	[1995/07/13  20:48 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 * 
 * 	Alias ACL work
 * 	HP 	[1995/05/15  21:09 UTC  greg  /main/HPDCE02/3]
 * 
 * 	Mothra bugs.
 * 	HP 	[1995/03/03  13:03 UTC  mdf  /main/mdf_mothra_defects_0/1]
 * 
 * 	Defect Number CHFts14394, modification to not have the machine
 * 	identity be required, so that once a change master is preformed
 * 	the old master can be removed.
 * 	[1995/12/08  17:21:33  root]
 * 
 * Revision 1.1.20.1  1996/05/10  13:23:13  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:47 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/01  16:36 UTC  hanfei
 * 	multi-cell group work: add NULL as cell_uuid for rsdb_pgo_add() call.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:18 UTC  psn
 * 	CHFts16061: allow cell administrator to
 * 	change the password on the local host account
 * 	[1995/09/07  14:43 UTC  aha  /main/HPDCE02/5]
 * 
 * 	CHFts16061: allow user:<creator> to modify user information
 * 	on the local host account
 * 	[1995/08/28  18:54 UTC  aha  /main/HPDCE02/aha_mothra9/1]
 * 
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	[1995/07/13  21:40 UTC  burati  /main/HPDCE02/4]
 * 
 * 	Alias ACL work
 * 	[1995/07/13  20:48 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 * 
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:59 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	Resolution of CHFts14769
 * 	[1995/04/04  20:03 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_2/1]
 * 
 * 	Mothra bugs.
 * 	[1995/03/07  19:47 UTC  mdf  /main/HPDCE02/1]
 * 
 * 	Defect Number CHFts14394, modification to not have the machine
 * 	identity be required, so that once a change master is preformed
 * 	the old master can be removed.
 * 	[1995/03/03  13:03 UTC  mdf  /main/mdf_mothra_defects_0/1]
 * 
 * Revision 1.1.10.9  1994/09/06  19:01:16  mdf
 * 	Need to initialize trig_binding to NULL
 * 	[1994/09/06  19:00:49  mdf]
 * 
 * Revision 1.1.10.8  1994/09/02  17:47:30  mdf
 * 	Removed HP testing code, again (didn't I do this before).
 * 	[1994/09/01  20:18:06  mdf]
 * 
 * Revision 1.1.10.7  1994/08/29  15:51:47  hondo
 * 	OT 11919 --login activity idl change to support different address types
 * 	[1994/08/28  21:46:01  hondo]
 * 
 * Revision 1.1.10.6  1994/08/25  20:47:41  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  20:37:25  sekhar]
 * 
 * Revision 1.1.10.5  1994/08/25  16:30:19  hanfei
 * 	dce1.1 rgy database backward compatibility work
 * 	[1994/08/25  14:43:59  hanfei]
 * 
 * Revision 1.1.10.4  1994/08/04  16:14:02  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:28  mdf]
 * 
 * Revision 1.1.10.2  1994/05/11  19:31:20  ahop
 * 	hp_sec_to_osf_2 drop
 * 	 add GDS schema entries
 * 	Make multi-valued flag=true for test schema entries.
 * 	Add sch_entries for pwd_val_type, pwd_strength_binding, pwd_gen_binding.
 * 	 add pwd gen/strngth schema entries
 * 	[1994/04/29  21:46:15  ahop]
 * 
 * Revision 1.1.5.6  1993/03/31  21:01:25  cuti
 * 	Add sec_salvage_db test's special entries
 * 	[1993/03/25  21:48:46  cuti]
 * 
 * Revision 1.1.5.5  1992/12/29  16:37:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:37  zeliff]
 * 
 * Revision 1.1.5.4  1992/10/07  20:37:08  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:19:29  sekhar]
 * 
 * Revision 1.1.6.2  1992/10/06  17:27:56  burati
 * 	Second replication drop: bl5
 * 	Moved master_key operations to rs_mkey.c
 * 
 * Revision 1.1.5.3  1992/09/29  20:12:57  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:37:37  sekhar]
 * 
 * Revision 1.1.5.2  1992/08/31  18:27:01  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:11:51  sekhar]
 * 
 * Revision 1.1.2.3  1992/05/13  16:06:22  pato
 * 	 Fix CR 3432: Check for null pointers in cleanup code
 * 	[1992/05/13  14:04:48  pato]
 * 
 * Revision 1.1.2.2  1992/03/27  18:41:47  ahop
 * 	 fix CR 2406: copy keyseed into larger buffer before concatenating time
 * 	[1992/03/26  14:04:38  ahop]
 * 
 * Revision 1.1  1992/01/19  14:44:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* 
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1995
 * Unpublished work. All Rights Reserved.
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      Default account initialization.
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_reserved.c,v 1.1.20.3 1996/10/03 15:11:07 arvind Exp $";
#endif

#include <stdio.h>
#include <un_strng.h>
#include <un_time.h>
#include <bstring.h>
#ifdef SNI_SVR4
#include <unistd.h>
#endif /* SNI_SVR4 */
#include <sys/file.h>

#include <dce/dce.h>
#include <dce/rpcbase.h>
#include <dce/uuid.h>
#include <dce/keymgmt.h>
#include <dce/sec_attr_tools.h>
#include <dce/sec_daclmgr.h>

#include <rss_errors.h>
#include <rs_base.h>
#include <rs_reserved.h>
#include <rs_ver_pvt.h>
#include <rs_mkey.h>

#include <rsdb_acl.h>
#include <rsdb_acct.h>
#include <rsdb_pgo.h>
#include <rsdb_dir.h>
#include <rsdb_util.h>
#include <rsdb_name_util.h>
#include <rsdb_create.h>
#include <rsdb_misc.h>
#include <rsdb_policy.h>
#include <rsdb_attr_schema.h>
#include <sec_def_flags.h>

#include <sec_svc.h>
/*
 * ATTR_SCH_MALLOC: malloc P of length L.
 * If successful, memset buffer to zero.
 * Else, free all previously malloc-ed ptrs in S and return.
 *
 * P is a pointer to be malloc-ed
 * L is the length to be malloc-ed
 * STP is a pointer to an error_status_t
 */
#define ATTR_SCH_MALLOC(P,L,STP)	{               \
    if(!((P) = malloc(L))) {                            \
        SET_STATUS(STP, sec_rgy_cant_allocate_memory);  \
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),     \
                       sec_s_rs,                        \
                       svc_c_sev_error,                 \
                       *STP);                           \
        return;                                         \
    } else {                                            \
        memset((P), NULL, (L));                         \
    } }


#define n_nobody        "nobody"
#define n_root          "root"
#define n_daemon        "daemon"
#define n_sys           "sys"
#define n_bin           "bin"
#define n_uucp          "uucp"
#define n_who           "who"
#define n_mail          "mail"
#define n_tcb           "tcb"
#define n_dce_ptgt      SEC_PRIV_SERVER_NAME
#define n_dce_rgy       SEC_RGY_SERVER_NAME

#define n_nogroup       "nogroup"
#define n_system        "system"
#define n_kmem          "kmem"
#define n_mail          "mail"
#define n_tty           "tty"
#define n_none          "none"
#define n_tcb           "tcb"

#define UUID_STR_LEN        36
static char                nobody_uuid[UUID_STR_LEN + 1] = "";
static char                root_uuid[UUID_STR_LEN + 1] = "";
static char                daemon_uuid[UUID_STR_LEN + 1] = "";
static char                sys_uuid[UUID_STR_LEN + 1] = "";
static char                bin_uuid[UUID_STR_LEN + 1] = "";
static char                uucp_uuid[UUID_STR_LEN + 1] = "";
static char                who_uuid[UUID_STR_LEN + 1] = "";
static char                mail_uuid[UUID_STR_LEN + 1] = "";
static char                tcb_uuid[UUID_STR_LEN + 1] = "";
static char                dce_ptgt_uuid[UUID_STR_LEN + 1] = "";
static char                dce_rgy_uuid[UUID_STR_LEN + 1] = "";
static char                local_cell_uuid[UUID_STR_LEN + 1] = "";
static char                creator_uuid[UUID_STR_LEN + 1] = "";
static char                host_uuid[UUID_STR_LEN + 1] = "";

static char                local_krbtgt[sizeof(sec_rgy_name_t)];
static char                creator_name[] = "";
static char                host_name[sizeof(sec_rgy_name_t)];
static boolean32           new_creator = true;
static char                def_pwd[sec_passwd_str_t_size];
 
static boolean32			is_sec_salvage_db_test = false;


typedef struct rgy_res_pgo_t {
    sec_rgy_domain_t   domain;
    char               *name;
    signed32           num;
    char               *uuid;
    boolean32          projlist_ok;
    boolean32          is_required;
    boolean32          embedded_unix_id;
    signed32           quota;
} rgy_res_pgo_t;

typedef struct rgy_res_member_t {
    sec_rgy_domain_t   domain;         /* group or org */
    char               *go_name;       /* group or org name */
    char               *p_name;        /* person (member) name */
} rgy_res_member_t;

typedef struct rgy_res_acct_t {
    char                       *person;
    char                       *group;
    char                       *org;
    char                       *homedir;
    char                       *shell;
    char                       *passwd;
    sec_rgy_acct_admin_flags_t admin_flags;
    sec_rgy_acct_auth_flags_t  auth_flags;
    sec_rgy_acct_user_flags_t  user_flags;
} rgy_res_acct_t;

typedef struct rgy_res_attr_sch_entry_t {
    char			*attr_name;
    char			*attr_id;
    sec_attr_encoding_t		attr_encoding;
    char			*acl_mgr_type;
    sec_acl_permset_t		query_permset;
    sec_acl_permset_t		update_permset;
    sec_acl_permset_t		test_permset;
    sec_acl_permset_t		delete_permset;
    sec_attr_sch_entry_flags_t	schema_entry_flags;
    sec_attr_intercell_action_t	intercell_action;
    sec_attr_trig_type_flags_t	trig_types;
    sec_attr_bind_info_t	*trig_binding;
    char			*scope;
    char			*comment;
} rgy_res_attr_sch_entry_t;


/*QUOTA
 *
 * what should quota values for reserved accounts be?
 */
/*
 * Use NULL uuid string to create UUID on the fly.
 */
/*
 * WARNING: the principals with dynamically assigned unix ids
 * (unix id = sec_rgy_uxid_unknown), currently krbtgt and local host,
 * must appear in the rgy_res_pgo array AFTER all principals with
 * hardcoded unix ids so that a dynamically assigned unix id
 * does not conflict with a subsequent hard-coded unix id.
 */
static rgy_res_pgo_t   rgy_res_pgo[] = {
/* persons */
    { sec_rgy_domain_person,
      n_nobody,
      rgyc_uxid_nobody,
      nobody_uuid,
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_person,
      n_root,
      rgyc_uxid_root,
      root_uuid,
      true,
      false,
      true,
      sec_rgy_quota_unlimited },
    { sec_rgy_domain_person,
      n_daemon,
      rgyc_uxid_daemon,
      daemon_uuid,
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_person,
      n_sys,
      rgyc_uxid_sys,
      sys_uuid,
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_person,
      n_bin,
      rgyc_uxid_bin,
      bin_uuid,
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_person,
      n_uucp,
      rgyc_uxid_uucp,
      uucp_uuid,
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_person,
      n_who,
      rgyc_uxid_who,
      who_uuid,
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_person,
      n_mail,
      rgyc_uxid_mail,
      mail_uuid,
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_person,
      n_tcb,
      rgyc_uxid_tcb,
      tcb_uuid,
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_person,
      n_dce_ptgt,
      rgyc_uxid_dce_ptgt,
      dce_ptgt_uuid,
      true,
      true,
      true,
      0 },
    { sec_rgy_domain_person,
      n_dce_rgy,
      rgyc_uxid_dce_rgy,
      dce_rgy_uuid,
      true,
      true,
      true,
      0 },
    { sec_rgy_domain_person,
      creator_name,
      rgyc_uxid_creator,
      creator_uuid,
      true,
      false,
      true,
      sec_rgy_quota_unlimited },
    { sec_rgy_domain_person,
      local_krbtgt,
      rgyc_uxid_krbtgt,
      local_cell_uuid,
      true,
      true,
      false,
      0 },
    { sec_rgy_domain_person,
      host_name,
      rgyc_uxid_host,
      host_uuid,
      true,
      false,
      true,
      0 },
/* groups */
    { sec_rgy_domain_group,
      n_nogroup,
      rgyc_gid_nogroup,
      "",
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_group,
      n_system,
      rgyc_gid_system,
      "",
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_group,
      n_daemon,
      rgyc_gid_daemon,
      "",
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_group,
      n_uucp,
      rgyc_gid_uucp,
      "",
      true,
      false,
      true,
      0 },
   { sec_rgy_domain_group,
      n_bin,
      rgyc_gid_bin,
      "",
      true,
      false,
      true,
      0 },
   { sec_rgy_domain_group,
      n_kmem,
      rgyc_gid_kmem,
      "",
      true,
      false,
      true,
      0 },
   { sec_rgy_domain_group,
      n_mail,
      rgyc_gid_mail,
      "",
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_group,
      n_tty,
      rgyc_gid_tty,
      "",
      false,
      false,
      true,
      0 },
    { sec_rgy_domain_group,
      n_none,
      rgyc_gid_none,
      "",
      true,
      false,
      true,
      0 },
    { sec_rgy_domain_group,
      n_tcb,
      rgyc_gid_tcb,
      "",
      false,
      false,
      true,
      0 },
/* orgs */
    { sec_rgy_domain_org,
      n_none,
      rgyc_oid_none,
      "",
      true,
      false,
      true,
      0 },

/* END_OF_SEC_CREATE */
    { sec_rgy_domain_org,
      "END_OF_SEC_CREATE",
      rgyc_oid_none,
      "",
      true,
      false,
      true,
      0 }, 
/* foreign cell name */
    { sec_rgy_domain_person,
      foreign_krbtgt,
      rgyc_uxid_foreign_krbtgt,
      "",
      true,
      true,
      false,
      0 }, 
/* sec_salvage_db test person */
    { sec_rgy_domain_person,
      n_test1p,
      rgyc_uxid_test1p,
      "",
      false,
      false,
      true,
      sec_rgy_quota_unlimited },
    { sec_rgy_domain_person,
      n_test2p,
      rgyc_uxid_test2p,
      "",
      true,
      false,
      true,
      sec_rgy_quota_unlimited },
    { sec_rgy_domain_person,
      n_test3p,
      rgyc_uxid_test3p,
      "",
      false,
      true,
      true,
      sec_rgy_quota_unlimited },
    { sec_rgy_domain_person,
      n_test4p,
      rgyc_uxid_test4p,
      "",
      true,
      true,
      true,
      sec_rgy_quota_unlimited },
    { sec_rgy_domain_person,
      n_test5p,
      rgyc_uxid_test5p,
      "",
      true,
      true,
      true,
      sec_rgy_quota_unlimited },
/* sec_salvage_db test group */
   { sec_rgy_domain_group,
      n_test1g,
      rgyc_gid_test1g,
      "",
      false,
      false,
      true,
      0 }, 
   { sec_rgy_domain_group,
      n_test2g,
      rgyc_gid_test2g,
      "",
      true,
      false,
      true,
      0 },
   { sec_rgy_domain_group,
      n_test3g,
      rgyc_gid_test3g,
      "",
      false,
      true,
      true,
      0 },
   { sec_rgy_domain_group,
      n_test4g,
      rgyc_gid_test4g,
      "",
      true,
      true,
      true,
      0 },

/* sec_salvage_db test org */
    { sec_rgy_domain_org,
      n_test1o,
      rgyc_oid_test1o,
      "",
      true,
      false,
      true,
      0 }, 
    { sec_rgy_domain_org,
      n_test2o,
      rgyc_oid_test2o,
      "",
      true,
      false,
      true,
      0 }, 
    { sec_rgy_domain_org,
      n_test3o,
      rgyc_oid_test3o,
      "",
      false,
      true,
      true,
      0 }, 
    { sec_rgy_domain_org,
      n_test4o,
      rgyc_oid_test4o,
      "",
      true,
      true,
      true,
      0 } 
};


static rgy_res_member_t    rgy_res_member[] = {
/* group members */
    { sec_rgy_domain_group,    n_bin,          n_who },
    { sec_rgy_domain_group,    n_kmem,         n_root },
    { sec_rgy_domain_group,    n_tty,          n_root },
    { sec_rgy_domain_group,    n_kmem,         n_sys },
    { sec_rgy_domain_group,    n_uucp,         n_uucp },
    { sec_rgy_domain_group,    n_tcb,          n_tcb },
    { sec_rgy_domain_group,    n_mail,         n_mail },

    { sec_rgy_domain_group,    n_mail,         "END_OF_SEC_CREATE" },
    { sec_rgy_domain_group,    n_test1g,         n_test1p },
    { sec_rgy_domain_group,    n_test1g,         n_test2p },
    { sec_rgy_domain_group,    n_test1g,         n_test3p },
    { sec_rgy_domain_group,    n_test1g,         n_test4p },
    { sec_rgy_domain_group,    n_test2g,         n_test2p },
    { sec_rgy_domain_group,    n_test2g,         n_test3p },
    { sec_rgy_domain_group,    n_test2g,         n_test4p },
    { sec_rgy_domain_group,    n_test3g,         n_test3p },
    { sec_rgy_domain_group,    n_test3g,         n_test4p },
    { sec_rgy_domain_group,    n_test4g,         n_test4p }, 

    { sec_rgy_domain_org,    n_test1o,         n_test1p },
    { sec_rgy_domain_org,    n_test1o,         n_test2p },
    { sec_rgy_domain_org,    n_test1o,         n_test3p },
    { sec_rgy_domain_org,    n_test1o,         n_test4p },
    { sec_rgy_domain_org,    n_test2o,         n_test2p },
    { sec_rgy_domain_org,    n_test2o,         n_test3p },
    { sec_rgy_domain_org,    n_test2o,         n_test4p },
    { sec_rgy_domain_org,    n_test3o,         n_test3p },
    { sec_rgy_domain_org,    n_test3o,         n_test4p },
    { sec_rgy_domain_org,    n_test4o,         n_test4p }
};


/* Accounts with def_pwd get default password.
 * Accounts with "" get randomly generated password.
 */
static rgy_res_acct_t   rgy_res_acct[] = {
    { n_nobody,     n_nogroup,  n_none, "/",    "", "",         sec_rgy_acct_admin_flags_none,
                                                                sec_rgy_acct_auth_flags_none,
                                                                sec_rgy_acct_user_flags_none},
    { n_root,       n_system,   n_none, "/",    "", def_pwd,    SEC_DEF_FLGS_ACCT_ADMIN_LOGIN,
                                                                SEC_DEF_FLGS_ACCT_AUTH,
                                                                SEC_DEF_FLGS_ACCT_USER },
    { n_daemon,     n_daemon,   n_none, "/",    "", "",         SEC_DEF_FLGS_ACCT_ADMIN,
                                                                SEC_DEF_FLGS_ACCT_AUTH,
                                                                SEC_DEF_FLGS_ACCT_USER },
    { n_uucp,       n_uucp,     n_none, "/usr/spool/uucppublic",
                                                "/usr/lib/uucp/uucico",
                                                    "",         SEC_DEF_FLGS_ACCT_ADMIN,
                                                                SEC_DEF_FLGS_ACCT_AUTH,
                                                                SEC_DEF_FLGS_ACCT_USER },
    { n_bin,        n_bin,      n_none, "/bin", "", "",         SEC_DEF_FLGS_ACCT_ADMIN,
                                                                SEC_DEF_FLGS_ACCT_AUTH,
                                                                SEC_DEF_FLGS_ACCT_USER },
    { n_dce_ptgt,   n_none,     n_none, "/",    "", "",         SEC_DEF_FLGS_ACCT_ADMIN,
                                                                SEC_DEF_FLGS_ACCT_AUTH,
                                                                SEC_DEF_FLGS_ACCT_USER },
    { n_dce_rgy,    n_none,     n_none, "/",    "", "",         SEC_DEF_FLGS_ACCT_ADMIN_LOGIN,
                                                                SEC_DEF_FLGS_ACCT_AUTH,
                                                                SEC_DEF_FLGS_ACCT_USER },
    { local_krbtgt, n_none,     n_none, "/",    "", "",         SEC_DEF_FLGS_ACCT_ADMIN_CELL,
                                                                SEC_DEF_FLGS_ACCT_AUTH_CELL,
                                                                SEC_DEF_FLGS_ACCT_USER_CELL },
    { creator_name, n_none,     n_none, "/",    "", def_pwd,    SEC_DEF_FLGS_ACCT_ADMIN_LOGIN,
                                                                SEC_DEF_FLGS_ACCT_AUTH,
                                                                SEC_DEF_FLGS_ACCT_USER },
    { host_name,    n_none,     n_none, "/",    "", "",         SEC_DEF_FLGS_ACCT_ADMIN_LOGIN,
                                                                SEC_DEF_FLGS_ACCT_AUTH,
                                                                SEC_DEF_FLGS_ACCT_USER },
/* END_OF_SEC_CREATE */
	{ "END_OF_SEC_CREATE",	n_nogroup,  n_none, "/",    "", "",         sec_rgy_acct_admin_flags_none,
                                                                sec_rgy_acct_auth_flags_none,
                                                                sec_rgy_acct_user_flags_none},
    { n_test1p,    n_test1g,    n_test1o, "/users/test1p", "/bin/sh", "", SEC_DEF_FLGS_ACCT_ADMIN_CELL,
                                                                           SEC_DEF_FLGS_ACCT_AUTH_CELL,
                                                                           SEC_DEF_FLGS_ACCT_USER_CELL },
    { n_test2p,    n_test2g,	n_test2o, "/users/test2p", "/bin/ksh", "", SEC_DEF_FLGS_ACCT_ADMIN_CELL,
                                                                           SEC_DEF_FLGS_ACCT_AUTH_CELL,
                                                                       	   SEC_DEF_FLGS_ACCT_USER_CELL },
    { n_test3p,    n_test3g,	n_test3o, "/users/test3p", "/bin/csh", "", SEC_DEF_FLGS_ACCT_ADMIN_LOGIN,
                                                                           SEC_DEF_FLGS_ACCT_AUTH,
                                                                       	   SEC_DEF_FLGS_ACCT_USER },
    { n_test4p,    n_test4g,	n_test4o, "/users/test4p", "/bin/sh", "", SEC_DEF_FLGS_ACCT_ADMIN,
                                                                           SEC_DEF_FLGS_ACCT_AUTH,
                                                                       	   SEC_DEF_FLGS_ACCT_USER }

};


static rgy_res_attr_sch_entry_t rgy_res_pk_attr_sch_entry[] = {
  { "DCEPKKeyEncipherment",             "76251f8c-6230-11cf-af89-08000919ebb5",
    sec_attr_enc_bytes,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
    sec_acl_perm_read,		        sec_acl_perm_mgmt_info,
    sec_acl_perm_read,		        sec_acl_perm_mgmt_info,
    (sec_attr_sch_entry_reserved | sec_attr_sch_entry_use_defaults),
    sec_attr_intercell_act_accept,	sec_attr_trig_type_none,
    NULL, 				"",
    "The principal's public part of public key pair with usage KeyEncipherment"
  },
  { "DCEPKAuthentication",              "d44dc60e-6230-11cf-98ed-08000919ebb5",
    sec_attr_enc_bytes,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
    sec_acl_perm_read,		        sec_acl_perm_mgmt_info,
    sec_acl_perm_read,		        sec_acl_perm_mgmt_info,
    (sec_attr_sch_entry_reserved | sec_attr_sch_entry_use_defaults),
    sec_attr_intercell_act_accept,	sec_attr_trig_type_none,
    NULL,					"",
    "The principal's public part of public key pair with usage Authentication"
  },
  { "DCEPKPrivateKeyStorage",           "1f79c78e-b026-11cf-b7c8-0800090a5254",
    sec_attr_enc_uuid,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
    sec_acl_perm_mgmt_info,		sec_acl_perm_mgmt_info,
    sec_acl_perm_mgmt_info,		sec_acl_perm_mgmt_info,
    (sec_attr_sch_entry_reserved | sec_attr_sch_entry_use_defaults),
    sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
    NULL,					"",
    "Values: sec_psm_pkss_mechanism, sec_psm_file_mechansim"
  },
};


static rgy_res_attr_sch_entry_t   rgy_res_attr_sch_entry[] = {
{ "pre_auth_req",			"6c9d0ec8-dd2d-11cc-abdd-080009353559",
  sec_attr_enc_integer,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_mgmt_info,		sec_acl_perm_mgmt_info,
  sec_acl_perm_mgmt_info,		sec_acl_perm_mgmt_info,
  (sec_attr_sch_entry_reserved | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "values: {0=NONE, 1=PADATA_ENC_TIMESTAMPS, 2=PADATA_ENC_THIRD_PARTY}" },
{ "pwd_val_type",			"689843ce-dd2d-11cc-a3e1-080009353559",
  sec_attr_enc_integer,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_mgmt_info,		sec_acl_perm_mgmt_info,
  sec_acl_perm_mgmt_info,		sec_acl_perm_mgmt_info,
  (sec_attr_sch_entry_reserved | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "values: {0=NONE, 1=USER_SELECT, 2=USER_CAN_SELECT, 3=GENERATION_REQUIRED}" },
{ "pwd_mgmt_binding",   		"6a93b8f2-dd2d-11cc-9be7-080009353559",
  sec_attr_enc_binding,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_mgmt_info,		sec_acl_perm_mgmt_info,
  sec_acl_perm_mgmt_info,		sec_acl_perm_mgmt_info,
  (sec_attr_sch_entry_reserved | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "binding to server exporting the password strength and generation interfaces" },
{ "X500_DN",				"c5949eba-384a-11cd-8cba-080009353559",
  sec_attr_enc_printstring,		"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,			sec_acl_perm_mgmt_info,
  sec_acl_perm_read,			sec_acl_perm_mgmt_info,
   (sec_attr_sch_entry_unique | sec_attr_sch_entry_reserved ),
  sec_attr_intercell_act_accept,	sec_attr_trig_type_none,
  NULL,					"",
  "The principal's X500 Distinguished Name" },
{ "X500_DSA_Admin",			"c6a51456-384a-11cd-b6ef-080009353559",
  sec_attr_enc_printstring_array,	"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,			sec_acl_perm_mgmt_info,
  sec_acl_perm_read,			sec_acl_perm_mgmt_info,
  (sec_attr_sch_entry_reserved | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "list of DSAs that the principal is allowed to administer" },
{ "disable_time_interval",              "63005af0-dd2d-11cc-9be7-080009353559",
  sec_attr_enc_integer,                 "06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_mgmt_info,               sec_acl_perm_mgmt_info,
  sec_acl_perm_mgmt_info,               sec_acl_perm_mgmt_info,
   (sec_attr_sch_entry_unique | sec_attr_sch_entry_reserved ),
  sec_attr_intercell_act_reject,        sec_attr_trig_type_none,
  NULL,                                 "",
  "values: {number of seconds to disable account-86400(60*60*24) is one day}" },
{ "max_invalid_attempts",               "657eb68c-dd2d-11cc-8990-080009353559",
  sec_attr_enc_integer,                 "06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_mgmt_info,               sec_acl_perm_mgmt_info,
  sec_acl_perm_mgmt_info,               sec_acl_perm_mgmt_info,
   (sec_attr_sch_entry_unique | sec_attr_sch_entry_reserved ),
  sec_attr_intercell_act_reject,        sec_attr_trig_type_none,
  NULL,                                 "",
  "values: {number of invalid attempts allowed before account is disabled}" },
{ "passwd_override",               "bc51691e-dd2d-11cc-9866-080009353559",
  sec_attr_enc_integer,                 "06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_mgmt_info,               sec_acl_perm_mgmt_info,
  sec_acl_perm_mgmt_info,               sec_acl_perm_mgmt_info,
   (sec_attr_sch_entry_unique | sec_attr_sch_entry_reserved ),
  sec_attr_intercell_act_reject,        sec_attr_trig_type_none,
  NULL,                                 "",
  "values: {the ability to not be restricted by passwd expiration}" },
};


static sec_timeval_sec_t  creation_date;  /* for setting creation dates */


/*
 * static - Internal functions
 */


/*  r s _ p r o c e s s _ s p e c i a l _ a c l s
 *
 *  The ACL on the creator's PGO should contain max
 *  privileges, instead of the default permissions,
 *  on the user_obj entry.  (The user_obj entry takes
 *  precedence when both user_obj and user entries
 *  match.)
 *
 *  CHFts16061: Previously the ACL on the local host's
 *  PGO did not have the u-bit set on its user:creator
 *  entry.  The reason was to prevent an accidental
 *  password change that could prevent login programs
 *  from working.  In practice, it is more important to
 *  be able to change the password than to prevent
 *  accidental changes, so this safeguard has been
 *  removed.
 */
static void rs_process_special_acls
  (
    sec_id_t            *rgy_creator,
    error_status_t      *st
  )
{
    sec_acl_type_t          acl_type;
    uuid_t                  *mgr_type;
    rsdb_acl_entry_t        entry;
    boolean32               add_bits = false;
           
    CLEAR_STATUS(st);
    acl_type = sec_acl_type_object;
    mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_person]);
    entry.name_buf[0] = '\0';

    entry.entry_type  = sec_acl_e_type_user_obj;
    entry.perms       = rsdb_acl_permset_person();

    rsdb_acl_replace_entry( sec_rgy_domain_person, rgy_creator->name,
                            acl_type, mgr_type, &entry, st );

    /* CHFts16061: do not delete the user_info bit from the user:creator
     * acl entry on the local host account; it is more important to be
     * able to change the local host password when necessary than to
     * prevent an accidental change
     */
}


static void add_special_acls
  (
    uuid_t		foreign_cell_uuid,
    uuid_t              special_group_uuid,
    error_status_t	*st
  )
{
    sec_acl_type_t          acl_type;
    uuid_t                  *mgr_type;
    rsdb_acl_entry_t        entry;
    rsdb_acl_mgr_type_t     mgr;
    char	            *pgo_name;
    sec_rgy_domain_t	    domain;
    uuid_t		    foreign_user_id;
    uuid_t		    foreign_group_id;


    CLEAR_STATUS(st);
    acl_type = sec_acl_type_object;
    entry.name_buf[0] = '\0'; 

    do {
	uuid_create(&foreign_user_id, st);
    } while (BAD_STATUS(st));

    do {
	uuid_create(&foreign_group_id, st);
    } while (BAD_STATUS(st));

    for (mgr = rsdb_acl_mgr_type_person; mgr < rsdb_acl_mgr_type_replist; mgr++)
    {
    	mgr_type = &(rsdb_acl_mgr_uuids[mgr]);

        switch(mgr) {                                                                        
            case rsdb_acl_mgr_type_person: pgo_name = n_test3p;                             
		domain = sec_rgy_domain_person;
                break;
            case rsdb_acl_mgr_type_group: pgo_name = n_test3g;
		domain = sec_rgy_domain_group;
                break;
            case rsdb_acl_mgr_type_org: pgo_name = n_test3o;
		domain = sec_rgy_domain_org;
                break;
            case rsdb_acl_mgr_type_dir: pgo_name = "test1pd";
		domain = sec_rgy_domain_person;
                break;
        }

    	entry.entry_type = sec_acl_e_type_group; 
	entry.perms = sec_acl_perm_name | sec_acl_perm_fullname |
	    sec_acl_perm_mgmt_info;
	entry.entry_info.id.uuid = special_group_uuid;
	entry.entry_info.id.name_len = 0;

	rsdb_acl_replace_entry(domain, *((sec_rgy_name_t *)pgo_name),
				acl_type, mgr_type, &entry, st);

	entry.entry_type = sec_acl_e_type_foreign_other;
	entry.perms = sec_acl_perm_Delete_object;
	entry.entry_info.id.uuid = foreign_cell_uuid;
	entry.entry_info.id.name_len = 0;

        rsdb_acl_replace_entry(domain, *((sec_rgy_name_t *)pgo_name),
                                acl_type, mgr_type, &entry, st);

	entry.entry_type = sec_acl_e_type_foreign_user;
	entry.perms = 0;
	entry.entry_info.foreign_id.id.uuid = foreign_user_id;
	entry.entry_info.foreign_id.realm.uuid = foreign_cell_uuid;

        rsdb_acl_replace_entry(domain, *((sec_rgy_name_t *)pgo_name),
                                acl_type, mgr_type, &entry, st);

        entry.entry_type = sec_acl_e_type_foreign_group;
        entry.perms = 0;
        entry.entry_info.foreign_id.id.uuid = foreign_group_id;
        entry.entry_info.foreign_id.realm.uuid = foreign_cell_uuid;

        rsdb_acl_replace_entry(domain, *((sec_rgy_name_t *)pgo_name),
                                acl_type, mgr_type, &entry, st);
    }

}


static void add_special_org_policy
  (
    char		*name,
    error_status_t 	*st
  )
{
    sec_rgy_plcy_t	policy;

    policy.passwd_min_len = 10;
    policy.passwd_lifetime = 60*60*24*30; /* one month */
    policy.passwd_exp_date = creation_date + 60*60*24*60; /* two month */
    policy.acct_lifespan = 60*60*24; /* one day */
    policy.passwd_flags =
	sec_rgy_plcy_pwd_no_spaces | sec_rgy_plcy_pwd_non_alpha;
    
    rsdb_policy_set_info((unsigned_char_p_t)name,  &policy, st);

}


/*
 * a d d _ p g o _ e n t s
 */
static void add_pgo_ents
  (
    sec_id_t        *rgy_creator,
    long            rgy_creator_unix_id,
    error_status_t  *st
  )
{
    rgy_res_pgo_t           *pp, *ppe;
    sec_rgy_name_t          pgo_name;
    sec_rgy_name_t          pname;
    int                     pname_len;
    sec_rgy_pgo_item_t      pgo_rec;
    int                     i;
    sec_rgy_foreign_id_t    creator_ids;
    uuid_t                  foreign_cell_uuid;
    uuid_t                  special_group_uuid;
    boolean32               tmp;

    CLEAR_STATUS(st);
    uuid_create_nil(&(creator_ids.principal), st);

    ppe = ((rgy_res_pgo_t *)
           (((char *) &rgy_res_pgo[0]) + sizeof(rgy_res_pgo)));
    for (pp = &rgy_res_pgo[0]; pp < ppe; pp++) {
        if (!is_sec_salvage_db_test &&
            strcmp(pp->name, "END_OF_SEC_CREATE") == 0) {
            break;
        } else {
            /* do sec_salvage_db test specific */ 
            if (strcmp(pp->name, "END_OF_SEC_CREATE") == 0)
       	        continue;   
        }   

        /* Check for creator record (i.e. name is null). */
        if (pp->name[0] == '\0') {
            /* Create a new PGO only if creator name is new. */
            if (new_creator) {
                strcpy((char *) pgo_name, (char *) rgy_creator->name);
                pp->num = rgy_creator_unix_id;
            } else {
                continue;
            }
        } else {
            strcpy((char *) pgo_name, pp->name);
        }

        /* For NULL UUID string, create uuid, else convert string. */
        if (*(pp->uuid) == '\0') {
            uuid_create_nil(&(pgo_rec.id), st);
            rsdb_unix_uuid_gen_check(pp->domain, pp->embedded_unix_id,
                                     (int *)&(pp->num), &(pgo_rec.id), st);
        } else {
            uuid_from_string((unsigned char *) (pp->uuid), &(pgo_rec.id), st);
            if (BAD_STATUS(st)) {
                return;
            }
            /* Check that unix id and uuid are consistent. */
            rsdb_unix_uuid_gen_check(pp->domain, pp->embedded_unix_id,
                                     (int *)&(pp->num), &(pgo_rec.id), st);
            if (BAD_STATUS(st)) {
                return;
            }
        }

        /* foreign cell name */
	if (is_sec_salvage_db_test && strcmp(pp->name, foreign_krbtgt) == 0) { 
	    foreign_cell_uuid = pgo_rec.id;
	}

        pgo_rec.unix_num = pp->num;
        pgo_rec.fullname[0] = '\0';
        pgo_rec.quota = pp->quota;

        if (pp->projlist_ok)
            pgo_rec.flags = sec_rgy_pgo_projlist_ok;
        if (pp->is_required)
            pgo_rec.flags |= sec_rgy_pgo_is_required;

        /*
         * Try adding pgo_name.  If bad status of dir_not_found, attempt
         * to create each of the directories in the pgo_name pathname
         * before adding the pgo "leaf."
         */
        rsdb_pgo_add(pp->domain, pgo_name, &pgo_rec, NULL, &creator_ids, true, &tmp,
            st);

            /* group name for special acl */ 
        if (is_sec_salvage_db_test && strcmp(pp->name, n_test4g) == 0) {
            special_group_uuid = pgo_rec.id;
            /* add an alias */
	    pgo_rec.flags |= sec_rgy_pgo_is_an_alias;
	    u_strcpy(pgo_name, "test5g");
            rsdb_pgo_add(pp->domain, pgo_name, &pgo_rec, NULL, &creator_ids, true,
                &tmp, st);
        }

        if (STATUS_EQUAL(st, sec_rgy_dir_not_found)) {
            /*
             * Use pname, copy of pgo_name, for strip operation so we
             * don't destroy pgo_name.
             */
            rsdb_name_util_get_parent(pgo_name, pname);
            pname_len = ustrlen(pname);
            rsdb_util_build_dirs(pp->domain, pname, pname_len, st);
            if (STATUS_OK(st)) {
                rsdb_pgo_add(pp->domain, pgo_name, &pgo_rec, NULL, &creator_ids,
                    true, &tmp, st);
            }
        }
        if (BAD_STATUS(st)) {
            return;
        }
    }
    /* Augment creator's ACL by adding user_obj entry with max privileges.
     * Remove dangerous u-bit from local host's acl entries.
     */
    rs_process_special_acls(rgy_creator, st);

    if (is_sec_salvage_db_test) {
	add_special_acls(foreign_cell_uuid, special_group_uuid, st);
    }

    if (is_sec_salvage_db_test) {
        add_special_org_policy(n_test4o, st);
    }
}


/*
 * a d d _ m e m b e r s
 */
static void add_members
  (
    sec_id_t        *rgy_creator,
    error_status_t  *st
  )
{
    rgy_res_member_t *mp, *mpe;
    rgy_res_acct_t *ap, *ape;
    unsigned char  *pname;

    CLEAR_STATUS(st);

    mpe = ((rgy_res_member_t *)
           (((char *) &rgy_res_member[0]) + sizeof(rgy_res_member)));

    for (mp = &rgy_res_member[0]; mp < mpe; mp++) { 
        /* Check for the end of sec_create_db */
	if (!is_sec_salvage_db_test && strcmp(mp->p_name,
            "END_OF_SEC_CREATE") == 0) {
            break;
        } else {
            /* do sec_salvage_db test specific */ 
            if (strcmp(mp->p_name, "END_OF_SEC_CREATE") == 0)
       	       	continue;   
        }

        rsdb_pgo_add_member(mp->domain, (unsigned char *) mp->go_name,
            (unsigned char *) mp->p_name, st);
        if (BAD_STATUS(st))
            return;
    }

    ape = ((rgy_res_acct_t *)
           (((char *) &rgy_res_acct[0]) + sizeof(rgy_res_acct)));

    for (ap = &rgy_res_acct[0]; ap < ape; ap++) {
        /* Check for the end of sec_create_db */
        if (!is_sec_salvage_db_test &&
            strcmp(ap->person, "END_OF_SEC_CREATE") == 0) {
            break;
        } else {
            /* do sec_salvage_db test specific */ 
            if (strcmp(ap->person, "END_OF_SEC_CREATE") == 0)
       	        continue;   
       	}
 
        /*
         * Special case: only process the rgy_creator acct if the creator
         * did not correspond to an existing principal and acct.
         */
        if (ap->person[0] == '\0') {
            /* This is the rgy_creator account record */
            if (new_creator) {
                pname = rgy_creator->name;
            } else {
                /* Creator taken care of by another account. */
                continue;
            }
        } else {
            pname = (unsigned char *) ap->person;
        }
        rsdb_pgo_add_member(sec_rgy_domain_group,
                            (unsigned char *) ap->group, pname, st);
        if (BAD_STATUS(st))
            return;

        rsdb_pgo_add_member(sec_rgy_domain_org,
                            (unsigned char *) ap->org, pname, st);
        if (BAD_STATUS(st))
            return;
    }

}


/*
 * a d d _ a c c o u n t s
 */
static void add_accounts
  (
    sec_id_t        *rgy_creator,
    sec_id_t        *rgy_local_cell,
    error_status_t  *st
  )
{
    rgy_res_acct_t          *ap, *ape;
    sec_rgy_acct_user_t     acct_user;
    sec_rgy_acct_admin_t    acct_admin;
    sec_passwd_rec_t        passwd;
    sec_passwd_version_t    passwd_version_number;
    sec_rgy_login_name_t    login_name;
    error_status_t          tmp_status;
    sec_rgy_plcy_auth_t     auth_policy;
    sec_rgy_login_name_t    dce_ptgt_login_name;
    boolean32               gen_unix_passwd = true;

    CLEAR_STATUS(st);

    acct_user.gecos[0] = '\0';

    acct_admin.creator.principal      = rgy_creator->uuid;
    acct_admin.creator.cell           = rgy_local_cell->uuid;
    acct_admin.creation_date          = creation_date;
    acct_admin.last_changer.principal = rgy_creator->uuid;
    acct_admin.last_changer.cell      = rgy_local_cell->uuid;
    acct_admin.change_date            = creation_date;
    acct_admin.expiration_date        = 0;
    acct_admin.good_since_date        = creation_date;

    ape = ((rgy_res_acct_t *)
           (((char *) &rgy_res_acct[0]) + sizeof(rgy_res_acct)));

    passwd.pepper           = NULL;
    passwd.version_number   = 1;

    for (ap = &rgy_res_acct[0]; ap < ape; ap++) { 
        /* Check for the end of sec_create_db */
        if (!is_sec_salvage_db_test &&
            strcmp(ap->person, "END_OF_SEC_CREATE") == 0) {
            break;
	} else {
            /* do sec_salvage_db test specific */ 
            if (strcmp(ap->person, "END_OF_SEC_CREATE") == 0)
       	        continue;   
	}

        /*
         * Special case: only add the rgy_creator acct if the creator did
         * not correspond to an existing principal and acct.
         */
        if (ap->person[0] == '\0') {
            if (new_creator) {
                strcpy((char *) login_name.pname, (char *) rgy_creator->name);
            } else {
                continue;
            }
        } else {
            strcpy((char *) login_name.pname, ap->person);
        }
        strcpy((char *) login_name.gname, ap->group);
        strcpy((char *) login_name.oname, ap->org);

        strcpy((char *) acct_user.homedir, ap->homedir);
        strcpy((char *) acct_user.shell, ap->shell);

        if (ap->passwd[0] == '\0') {
            /* Null string passwd means generate a random key */
            rs_mkey_random(&passwd, st);
            if( BAD_STATUS(st) )
                return;
        } else {
            PASSWD_TYPE(&passwd)            = sec_passwd_plain;
            PLAIN_PASSWD(&passwd)           = (unsigned char *) ap->passwd;
        }
        acct_user.passwd_dtm                = creation_date;

        acct_admin.flags                    = ap->admin_flags;
        acct_admin.authentication_flags     = ap->auth_flags;
        acct_user.flags                     = ap->user_flags;

        rsdb_acct_add(&login_name, sec_rgy_acct_key_person, &acct_user,
                  &acct_admin, &passwd, gen_unix_passwd, 
                  &passwd_version_number, st);
        if (BAD_STATUS(st))
            return;

        if (!is_sec_salvage_db_test) { /* sec_create_db */
            /* Special case the local host account
             * which must be added to default keytab file
             */
            if (ustrcmp(ap->person, host_name) == 0) {
                sec_key_mgmt_set_key(rpc_c_authn_dce_secret, (void *)NULL,
                    (unsigned_char_p_t)(ap->person), passwd.version_number,
                    (void *) &passwd, st);
                if (BAD_STATUS(st)) {
                    dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                                   sec_s_rs , svc_c_sev_error, *st);
                    dce_svc_printf(SEC_RES_HOST_KEY_SET_ERR_MSG);
                    return;
                }
            }    
        }
    }
    /* the dce-ptgt account should have max_ticket_lifetime of 2 hours. */
    CLEAR_LOGIN_NAME(&dce_ptgt_login_name);
    ustrcpy(dce_ptgt_login_name.pname, n_dce_ptgt);
    auth_policy.max_ticket_lifetime = 60*60*2;      /* 2 hours */
    auth_policy.max_renewable_lifetime = RSDB_DEF_AUTH_POL_MAX_RENEW_LIFE;
    rsdb_auth_policy_set_info(&dce_ptgt_login_name, &auth_policy, st);
    if (BAD_STATUS(st))
        return;
}

/*
 * set_attr_acl_mgr
 */

PRIVATE set_resv_attr_acl_mgr(
    sec_attr_schema_entry_t	*sch_entry_p,
    error_status_t  *st_p)
{

    /* Assuming one acl_mgr for now */
    ATTR_SCH_MALLOC(SA_ACL_MGR_SET_P(sch_entry_p),
	SA_ACL_MGR_SET_SIZE(1), st_p);
    ATTR_SCH_MALLOC(SA_ACL_MGR_INFO_P(sch_entry_p, 0),
	sizeof(*(SA_ACL_MGR_INFO_P(sch_entry_p, 0))), st_p);
    SA_ACL_MGR_NUM(sch_entry_p)	= 1;


}

PRIVATE void form_one_resv_schema_entry(
    rgy_res_attr_sch_entry_t	*sp,
    sec_attr_schema_entry_t	*sch_entry_p,
    error_status_t              *st_p)
{

	sch_entry_p->attr_name = (sec_attr_component_name_t)sp->attr_name;
	uuid_from_string((unsigned char *)(sp->attr_id), 
		&(sch_entry_p->attr_id), st_p);
	sch_entry_p->attr_encoding	= sp->attr_encoding;

	uuid_from_string((unsigned char *)(sp->acl_mgr_type), 
	    &(SA_ACL_MGR_TYPE(sch_entry_p, 0)), st_p);
	SA_ACL_MGR_PERMS_QUERY(sch_entry_p, 0)	= sp->query_permset;
	SA_ACL_MGR_PERMS_UPDATE(sch_entry_p, 0)	= sp->update_permset;
	SA_ACL_MGR_PERMS_TEST(sch_entry_p, 0)	= sp->test_permset;
	SA_ACL_MGR_PERMS_DELETE(sch_entry_p, 0)	= sp->delete_permset;

	sch_entry_p->schema_entry_flags	= sp->schema_entry_flags;
	sch_entry_p->intercell_action	= sp->intercell_action;
	sch_entry_p->trig_types		= sp->trig_types;

	/* Skip trig_binding for now */
	sch_entry_p->trig_binding	= NULL;

	sch_entry_p->scope		= (unsigned_char_p_t)(sp->scope);
	sch_entry_p->comment		= (unsigned_char_p_t)(sp->comment);

}


PUBLIC void get_1resv_pk_schema(
    sec_attr_schema_entry_t	**sch_entry_p,
    unsigned32                  i,
    error_status_t              *st_p) 
{
    rgy_res_attr_sch_entry_t	*sp, *spe;


    CLEAR_STATUS(st_p);

    if(!(*sch_entry_p = 
	(sec_attr_schema_entry_t *)malloc(
		sizeof(sec_attr_schema_entry_t)))) {
	SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
	return;
    }

    set_resv_attr_acl_mgr(*sch_entry_p, st_p);

    spe = ((rgy_res_attr_sch_entry_t *)
	(((char *) rgy_res_pk_attr_sch_entry) + 
		sizeof(rgy_res_pk_attr_sch_entry)));

    sp = &rgy_res_pk_attr_sch_entry[i];

    form_one_resv_schema_entry(sp, *sch_entry_p, st_p);

}

/*
 * a d d _ a t t r _ s c h _ e n t r i e s
 */
static void add_attr_sch_entries
  (
    rgy_res_attr_sch_entry_t    *entry_name,
    unsigned32                  entry_name_size,
    error_status_t  *st_p
  )
{
    sec_attr_schema_entry_t	*sch_entry_p;
    rgy_res_attr_sch_entry_t	*sp, *spe;
    unsigned32			length;

    CLEAR_STATUS(st_p);

    if(!(sch_entry_p = 
	(sec_attr_schema_entry_t *)malloc(
		sizeof(sec_attr_schema_entry_t)))) {
	SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
	return;
    }

    set_resv_attr_acl_mgr(sch_entry_p, st_p);

    spe = ((rgy_res_attr_sch_entry_t *)
	(((char *) entry_name) + 
		entry_name_size));

    /* WARNING: this routine must be fixed to
     * free memory properly when more acl_mgr_types are
     * added to schema entries.
     */

    for(sp = entry_name; STATUS_OK(st_p) && sp < spe; 
	sp++) { 

        form_one_resv_schema_entry(sp, sch_entry_p, st_p);

        rsdb_attr_sch_add((sec_attr_component_name_t)"", sch_entry_p, st_p);
    }

    if (sch_entry_p) {
      free(sch_entry_p);
    }

}

PRIVATE void rs_add_attr_sch_entries(
    error_status_t  *st_p)
{

  add_attr_sch_entries(rgy_res_attr_sch_entry, sizeof(rgy_res_attr_sch_entry), st_p);

}



/*
 * rs_res_add_pk_attr_sch_entries
 */
PUBLIC void rs_resv_add_pk_attr_sch_entries

  (
    error_status_t  *st_p
  )
{
    add_attr_sch_entries(rgy_res_pk_attr_sch_entry, sizeof(rgy_res_pk_attr_sch_entry), 
			 st_p);
}


/*
 * d b _ c r e a t e
 */
void db_create
  (
    sec_id_t                *rgy_local_cell,
    sec_id_t                *rgy_creator,
    long                    rgy_creator_unix_id,
    unsigned_char_p_t       rgy_hostname,
    unsigned_char_p_t       rgy_password,
    error_status_t          *st
  )
{
    struct timezone     tz;
    sec_timeval_t       curr_time;
    sec_rgy_name_t      *realm_princ, *cell;
    boolean32           is_local;
    unsigned_char_p_t   cell_uuid_str;

#ifdef SNI_SVR4_POSIX
    gettimeofday((struct timeval *) &curr_time);
#else
    gettimeofday((struct timeval *) &curr_time, &tz);
#endif /* SNI_SVR4_POSIX */
    creation_date = curr_time.sec;

    /*  Convert global cell name to local realm principal.
     *  Store realm in local_krbtgt (must be initialized before
     *  add_pgo_ents is called).
     */
    cell = (sec_rgy_name_t *)rgy_local_cell->name;
    realm_princ = (sec_rgy_name_t *)local_krbtgt;
    rsdb_name_util_convert_from_global(*cell, *realm_princ, &is_local, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs , svc_c_sev_error , *st);
        dce_svc_printf(SEC_RES_PRINC_CVT_ERR_MSG);
        return;
    }

    /* Init local_cell_uuid string
     */
    uuid_to_string(&rgy_local_cell->uuid, &cell_uuid_str, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs , svc_c_sev_error , *st);
        dce_svc_printf(SEC_RES_UUID_CVT_ERR_MSG);
        return;
    }

    strncpy(local_cell_uuid, (char *)cell_uuid_str, UUID_STR_LEN);
    local_cell_uuid[UUID_STR_LEN] = '\0';

    rpc_string_free(&cell_uuid_str, st);

    /* Initialize host_name, required by add_pgo_ents */
    ustrcpy(host_name, rgy_hostname);

    /* Initialize default_password, required by add_accounts */
    ustrcpy(def_pwd, rgy_password);

    add_pgo_ents(rgy_creator, rgy_creator_unix_id, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs , svc_c_sev_error , *st);
        dce_svc_printf(SEC_RES_PGO_ADD_ERR_MSG);
        return;
    }

    add_members(rgy_creator, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs , svc_c_sev_error , *st);
        dce_svc_printf(SEC_RES_MEM_ADD_ERR_MSG);
        return;
    }

    add_accounts(rgy_creator, rgy_local_cell, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs , svc_c_sev_error , *st);
        dce_svc_printf(SEC_RES_ACCT_ADD_ERR_MSG);
        return;
    }

    rs_add_attr_sch_entries(st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs , svc_c_sev_error , *st);
        dce_svc_printf(SEC_RES_ATTR_SCH_ADD_ERR_MSG);
        return;
    }

    rs_resv_add_pk_attr_sch_entries(st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs , svc_c_sev_error , *st);
        dce_svc_printf(SEC_RES_PK_ATTR_SCH_ADD_ERR_MSG);
        return;
    }

}


/* r s _ s e t u p _ c r e a t o r
 *
 * Compare the incoming creator name with the names of all
 * reserved principals.  If there is a match, assign a UUID
 * for the principal based on its reserved unix number,
 * copy this UUID to the rgy_creator, and set new_creator to
 * false to signify that new principal and account records
 * do not have to be added for the rgy_creator.
 *
 * If the creator name does not match a reserved principal
 * name, use the reserved creator unix number to create a
 * UUID for rgy_creator and leave new_creator as true to indicate
 * that a new principal and account record should be created
 * for the rgy_creator.
 *
 *
 */
void rs_setup_creator
  (
    sec_id_t            *rgy_creator,
    long                rgy_creator_unix_id,
    error_status_t      *st
  )
{
    rgy_res_pgo_t        *pp;
    unsigned_char_t      *tmp_uuid_str;

    CLEAR_STATUS(st);

    for (pp = &rgy_res_pgo[0]; pp->domain == sec_rgy_domain_person; pp++) {
        /* Compare rgy_creator name with this pgo name.
         */
        if( strcmp((char *)rgy_creator->name, (char *)pp->name) == 0 ) {
            rsdb_uuid_create(sec_rgy_domain_person, true, pp->num,
                             &(rgy_creator->uuid), st);
            if(BAD_STATUS(st)) {
                return;
            }
            uuid_to_string( &(rgy_creator->uuid), &tmp_uuid_str, st );
            if(BAD_STATUS(st)) {
                return;
            }
            strncpy(pp->uuid, (char *)tmp_uuid_str, UUID_STR_LEN);
            pp->uuid[UUID_STR_LEN] = '\0';
            new_creator = false;
            return;
        }
    }
    /* We are here if rgy_creator->name didn't match a reserved principal.
     * Use first unix_id in the person domain unix_id space for creator.
     * Create a UUID for rgy_creator.
     */
    rsdb_uuid_create(sec_rgy_domain_person, true, rgy_creator_unix_id,
                     &(rgy_creator->uuid), st);
    if(BAD_STATUS(st)) {
        return;
    }
    uuid_to_string( &(rgy_creator->uuid), &tmp_uuid_str, st );
    if(BAD_STATUS(st)) {
        return;
    }
    strncpy(creator_uuid, (char *)tmp_uuid_str, UUID_STR_LEN);
    creator_uuid[UUID_STR_LEN] = '\0';
    return;
}               


PUBLIC void rs_set_sec_salvage_db_test
  (
    boolean32   bool
  )
{     
    is_sec_salvage_db_test = bool;
}

/*
 * r s _ r e s v _ a d d _ a t t r _ s c h _ e n t r i e s
 */
PUBLIC void rs_resv_add_attr_sch_entries
  (
    error_status_t  *st_p
  )
{
    rs_add_attr_sch_entries(st_p);
}


/*  r s _ r e s v _ g e n _ u n i n i t _ d b 
 *
 * With the advent of server migration, a server
 * is running at one of many possible software revisions.
 * When a server is initialized, the surrogate master
 * will propagate only those database elements 
 * appropriate to the current software revision.
 * For instance, as of this writing, this server
 * supports both 1.1 and 1.0.2 functionality.
 * Extended registry attributes were added at 1.1.
 * A server operating at 1.0.2 will never receive any
 * ERA propagations.  Yet there are certain reserved
 * ERA schema entries that are essential for
 * proper 1.1 functioning.  These reserved ERA
 * schema entries are created by the database
 * conversion code when a 1.1 server is brought 
 * up for the first time over a 1..0.2 database.
 * The server then goes into service as a 1.0.2
 * replica.
 *
 * When a server prepares to be initialized, it
 * wipes out the database and asks a surrogate master
 * to initialize it.  Unfortunately, wiping out the
 * database also destroys the reserved ERA schema
 * entries that were created earlier, and the
 * master, believing that the server is a 1.0.2 
 * replica, will not propagate the ERA database (in fact 
 * the master itself may be a true 1.0.2 replica, and thus
 * utterly incapable of propagating ERA updates).
 * 
 * We need to ensure that the necessary 1.1 reserved
 * entries are in the database whenever the server 
 * goes into the "uninitialized" state.  Currently this 
 * occurs in only twice; when a replica is created with 
 * sec_create_db, and when  the replica is sent the
 * rs_rep_mgr_init() propagation by the master.  
 *
 * The above discussion generalizes to reserved entries
 * required by any future version of the security
 * software.  This routine should be modified to
 * ensure that whatever reserved entries are required
 * to support the max software rev the server can support,
 * but that will not be propagated at the current
 * software rev, are reapplied to the database on
 * initialization.
 */
void rs_resv_gen_uninit_db
  (
    unsigned32      current_sw_rev,
    error_status_t  *st_p
  )
{
    error_status_t  lst;

    CLEAR_STATUS(st_p);

    if (!rs_sw_feature_test_version(rs_c_sw_feature_era,
				    current_sw_rev, &lst)) {
	rs_resv_add_attr_sch_entries(st_p);   
    }
}
