#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY 
# $Log: Makefile,v $
# Revision 1.1.7.2  1996/03/11  02:29:55  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:01:05  marty]
#
# Revision 1.1.7.1  1995/12/11  20:15:55  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:48:59  root]
# 
# Revision 1.1.5.5  1993/03/31  21:10:13  cuti
# 	Add sec_salvage_db test files
# 	[1993/03/31  20:36:21  cuti]
# 
# Revision 1.1.5.4  1993/01/29  21:07:03  cuti
# 	Add new tests.
# 	[1993/01/29  19:10:44  cuti]
# 
# Revision 1.1.5.3  1992/12/31  17:25:18  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:50:16  htf]
# 
# Revision 1.1.5.2  1992/11/02  15:38:03  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:40:38  root]
# 
# Revision 1.1.2.6  1992/06/08  20:43:07  cuti
# 	Add passwd_import test.
# 	[1992/06/08  20:11:09  cuti]
# 
# Revision 1.1.2.5  1992/06/02  21:29:10  sekhar
# 	Merged Unix interface changes made by frisco
# 	to the mainline.
# 	[1992/06/02  20:27:46  sekhar]
# 
# Revision 1.1.2.4  1992/05/28  20:26:13  cuti
# 	Add local_rgy's test.
# 	[1992/05/28  18:26:12  cuti]
# 
# Revision 1.1.2.3  1992/05/21  21:49:31  cuti
# 	 Add passwd_override test files.
# 	[1992/05/21  19:50:05  cuti]
# 
# Revision 1.1.2.2  1992/03/18  19:06:28  burati
# 	Add login_certify_identity.tsh to DATAFILES
# 	[1992/03/18  15:59:56  burati]
# 
# Revision 1.1  1992/01/19  04:06:59  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# DCE Security Services 
# test/security/api/control Makefile
#

DATAFILES = \
	acct.tshrc                      \
	acct_add.tsh                    \
	acct_admin_repl.tsh             \
	acct_del.tsh                    \
	acct_lookup.tsh                 \
	acct_passwd.tsh                 \
	acct_rename.tsh                 \
	acct_repl_all.tsh               \
	acct_user_repl.tsh              \
	acl_edit.tsh                    \
	acl_edit.tshrc                  \
	all_acct.tsh                    \
	all_auth_pol.tsh                \
	all_key_mgmt.tsh		\
	all_login.tsh			\
	all_misc.tsh                    \
	all_misc_test.tsh		\
	all_pgo.tsh                     \
	all_policy.tsh                  \
	all_props.tsh                   \
	auth_pol.tshrc                  \
	auth_pol_get.tsh                \
	auth_pol_get_eff.tsh            \
	auth_pol_set.tsh                \
	common.tshrc                    \
	id_map.tsh			\
	idmap.tshrc		 	\
	key_mgmt.tshrc			\
	key_mgmt_change_key.tsh		\
	key_mgmt_manage_key.tsh		\
	key_mgmt_get_next_key.tsh	\
	key_mgmt_cursor.tsh			\
	key_mgmt_delete_key.tsh		\
	key_mgmt_delete_key_type.tsh		\
	key_mgmt_free_key.tsh		\
	key_mgmt_garbage_collect.tsh	\
	key_mgmt_gen_rand_key.tsh	\
	key_mgmt_get_key.tsh		\
	key_mgmt_get_nth_key.tsh	\
	key_mgmt_set_key.tsh		\
	krb5.tshrc                      \
	login.tsh                       \
	login.tshrc                     \
	login_certify_identity.tsh      \
	login_check_password.tsh        \
	login_export_import_context.tsh \
	login_get_current_context.tsh   \
	login_get_expiration.tsh        \
	login_get_groups.tsh            \
	login_get_info.tsh              \
	login_get_effective.tsh			\
	login_get_pwent.tsh             \
	login_newgroups.tsh             \
	login_purge_context.tsh         \
	login_refresh_identity.tsh      \
	login_release_context.tsh       \
	login_set_context.tsh           \
	login_setup_identity.tsh        \
	login_test_all.tsh              \
	login_test_basic.tsh            \
	login_validate_identity.tsh     \
	login_valid_and_cert_ident.tsh  \
	misc.tsh                        \
	misc.tshrc                      \
	misc_test_all.tsh               \
	most_sec.tsh			\
	pgo.tshrc                       \
	pgo_delete.tsh                  \
	pgo_get_next.tsh                \
	pgo_group.tsh                   \
	pgo_key_transfer.tsh            \
	pgo_member.tsh                  \
	pgo_org.tsh                     \
	pgo_person.tsh                  \
	pgo_test_basic.tsh              \
	policy.tshrc                    \
	policy_get.tsh                  \
	policy_get_eff.tsh              \
	policy_set.tsh                  \
	props.tshrc                     \
	props_get.tsh                   \
	props_set.tsh                   \
	rgy_reset_cursor.tsh            \
	rgy_set_cache_handler.tsh       \
	rgy_set_resolve_mode.tsh        \
	rgy_wait_until_consistent.tsh   \
	sec_acl.tsh                     \
	sec_acl.tshrc                   \
	site.tshrc                      \
	site_bind.tsh                   \
	site_mgmt.tsh                   \
	passwd_override.tsh				\
	passwd_over.tshrc				\
	local_rgy.tsh					\
	local_rgy.tshrc					\
	unix.tsh                   		\
	passwd_import.tsh				\
	passwd_import.tshrc				\
	sec_salvage_db.tsh		\
	sec_salvage_db.tshrc		\
	tshrc                           

ILIST	= ${DATAFILES}

.include <${RULES_MK}>    

