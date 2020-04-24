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
# Revision 1.1.13.2  1996/03/11  02:34:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:13  marty]
#
# Revision 1.1.13.1  1995/12/11  20:20:04  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/01/11  20:30 UTC  mdf
# 	comment
# 
# 	HP revision /main/mdf_MOTHRA_0/1  1994/12/01  15:50 UTC  mdf
# 	Added clean up routine for local_rgy.
# 	[1995/12/11  19:51:46  root]
# 
# Revision 1.1.10.1  1994/06/02  21:14:33  mdf
# 	hp_sec_to_osf_3 drop, merge up with latest.
# 	[1994/05/24  15:21:26  mdf]
# 
# 	hp_sec_to_osf_3 drop, needed HFILES list.
# 
# 	hp_sec_to_osf_3 drop
# 	[1994/05/18  13:37:33  mdf]
# 
# Revision 1.1.7.4  1993/03/31  21:10:47  cuti
# 	Add sec_salvage_db test files.
# 	[1993/03/31  20:37:07  cuti]
# 
# Revision 1.1.7.3  1992/12/31  17:27:05  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:52:40  htf]
# 
# Revision 1.1.7.2  1992/11/02  15:38:09  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:40:48  root]
# 
# Revision 1.1.3.6  1992/06/02  21:20:10  sekhar
# 	Merged Unix interface changes made by frisco into
# 	the mainline.
# 	[1992/06/02  20:10:07  sekhar]
# 
# Revision 1.1.3.5  1992/05/21  21:24:26  cuti
# 	 Add passwd_override test files to DATAFILES.
# 	[1992/05/21  21:14:27  cuti]
# 
# Revision 1.1.3.4  1992/05/20  21:34:20  cuti
# 	       Removed extra sec_acl_setup.sh and garbage data.
# 	[1992/05/20  16:44:47  cuti]
# 
# Revision 1.1.3.3  1992/05/19  20:12:54  cuti
# 	 Add rgy_edit_init.sh to DATAFILES
# 	[1992/05/19  20:02:11  cuti]
# 
# 	 Add local rgy files to DATAFILES
# 	[1992/05/19  19:28:13  cuti]
# 
# Revision 1.1.3.2  1992/05/14  20:31:11  cuti
# 	Remove trailing blanks and ILIST.
# 	[1992/05/14  14:11:20  cuti]
# 
# 	Add passwd_import testing files to DATAFILES block
# 	[1992/05/12  18:54:08  cuti]
# 
# Revision 1.1  1992/01/19  04:08:41  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# DCE Security Services 
# test/security/api/moretests Makefile
#

# Compiler flags for this directory
CFLAGS = ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../../../security/h -I../../../../security/client/login

# Flags for the IDL compiler
IDLINCFLAGS = -I.
IDLFLAGS = -keep c_source

IDLOBJ   = ${IDLFILES:.idl=.h}

IDLFILES = 	dlg_test.idl

# Programs to build
PROGRAMS = 	\
		dlg_client \
		dlg_intermediary \
		\
		pgo_test_basic   \
	 	pgo_key_transfer \
		pgo_get_next     \
		\
		acct_add_del     \
		acct_admin_repl  \
		acct_lookup      \
		acct_passwd      \
		acct_rename      \
		acct_repl_all    \
		acct_user_repl   \
		\
		unix_getpwuid   \
		unix_getpwnam   \
		unix_getgrgid   \
		unix_getgrnam   \
		\
		rgy_misc	 \
		\
		auth_policy      \
		policy	 	 \
		\
		props		 \
		\
		site_mgmt	 \
		site_bind	 \
		\
		idmap            \
		\
 		key_mgmt         \
		\
		login_test	 \
		\
		sec_acl		 \
		\
		binlogin	 \
		sec_authn
#		\
#		acl_edit_add	 \
#		acl_edit_del	 \
#		\
#		login_test_basic \
#		login_test_2     \

dlg_client_OFILES       = dlg_test_cstub.o \
			  dlg_util.o \
			  dlg_client.o
dlg_intermediary_OFILES = dlg_test_sstub.o \
			  dlg_util.o \
			  dlg_intermediary.o

pgo_test_basic_OFILES   = pgo_test_basic.o    pgo_util.o  main.o
pgo_key_transfer_OFILES = pgo_key_transfer.o  pgo_util.o  main.o
pgo_get_next_OFILES    	= pgo_get_next.o      pgo_util.o  main.o

acct_add_del_OFILES 	= acct_add_del.o     main.o
acct_admin_repl_OFILES 	= acct_admin_repl.o  main.o
acct_lookup_OFILES 	= acct_lookup.o      main.o
acct_passwd_OFILES 	= acct_passwd.o      main.o
acct_rename_OFILES 	= acct_rename.o      main.o
acct_repl_all_OFILES 	= acct_repl_all.o    main.o
acct_user_repl_OFILES 	= acct_user_repl.o   main.o

unix_getpwuid_OFILES 	= unix_getpwuid.o   main.o
unix_getpwnam_OFILES 	= unix_getpwnam.o   main.o
unix_getgrgid_OFILES 	= unix_getgrgid.o   main.o
unix_getgrnam_OFILES 	= unix_getgrnam.o   main.o
 
binlogin_OFILES		= binlogin.o
login_test_OFILES	= login_test.o       main.o
login_test_basic_OFILES = login_test_basic.o main.o
login_test_2_OFILES 	= login_test_2.o     main.o

key_mgmt_OFILES		= key_mgmt.o rgy_util.o main.o

rgy_misc_OFILES 	= rgy_misc.o rgy_util.o main.o

auth_policy_OFILES	= auth_policy.o main.o
policy_OFILES		= policy.o      main.o

props_OFILES		= props.o main.o

site_mgmt_OFILES	= site_mgmt.o main.o
site_bind_OFILES	= site_bind.o main.o

sec_acl_OFILES		= sec_acl.o pgo_util.o main.o

acl_edit_add_OFILES 	= acl_edit_add.o main.o
acl_edit_del_OFILES	= acl_edit_del.o main.o

idmap_OFILES		= idmap.o rgy_util.o main.o

HFILES= dlg_test.h

dlg_test.h: dlg_test.idl

# Libraries the programs need to link with
LIBS = -ldce ${${TARGET_MACHINE}_LIBS}

DATAFILES = \
	acl_edit.sh       \
	acl_edit_setup.sh \
	all_rgy_edit.sh   \
	key_mgmt_setup.sh \
	rgy.sh		  \
	rgy_account.sh	  \
	rgy_add.sh	  \
	rgy_change.sh	  \
	rgy_check_init.sh \
	rgy_cleanup.sh	  \
	rgy_delete.sh	  \
	rgy_edit_test.sh  \
	rgy_group.sh	  \
	rgy_member.sh	  \
	rgy_misc.sh	  \
	rgy_org.sh	  \
	rgy_person.sh	  \
	rgy_policy.sh	  \
	rgy_setup.sh      \
	sec_acl_setup.sh \
	passwd_import.sh \
	passwd_import_er.out \
	passwd_import_n.out \
	passwd_import_ny.out \
	passwd_import_i.out \
	passwd_import_ig.out \
	passwd_import_ie.out \
	passwd_import_r.out \
	kill_secd.sh \
	add_org.sh \
	del_org.sh \
	passwd \
	group \
	local_rgy.sh   \
	local_login.sh \
	local_rgy_setup.sh  \
	local_rgy_cleanup.sh  \
	rgy_setup_init.sh \
	passwd_override.sh \
	passwd_override \
	po_setup.sh \
	salvager.sh \
	salvager_sample.prt

ILIST		= ${PROGRAMS} ${DATAFILES}

.include <${RULES_MK}>

