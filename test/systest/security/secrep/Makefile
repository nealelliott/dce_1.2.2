# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.2  1996/03/09  20:51:56  marty
# 	Add OSF copyright
# 	[1996/03/09  20:29:37  marty]
#
# Revision 1.1.2.1  1995/12/11  22:47:06  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/12/05  22:09 UTC  psn
# 	Prepare test for DCE 1.2.1 drop to OSF.
# 
# 	HP revision /main/HPDCE02/1  1994/08/03  20:05 UTC  bissen
# 	merge from HPDCE01
# 
# 	HP revision /main/HPDCE01/1  1994/05/03  15:39 UTC  gordon_l
# 	Fixes for change_master/become_master
# 
# 	HP revision /main/gordon_branch/1  1994/05/03  15:30 UTC  gordon_l
# 	Fixes for change_master/become_master
# 	[1995/12/11  22:22:20  root]
# 
# Revision 1.1.2.2  1993/08/02  17:39:54  sekhar
# 	Replication system test. Initial version
# 	[1993/08/02  17:28:37  sekhar]
# 
# $EndLog$
#
#   sekhar Initial Version
# 
#   This is the makefile will build/install the files required
#   for replication system test. Currently all of the files
#   are shell scripts and hence only the install pass needs to 
#   to be run. 
#
#   The replication system also uses the security functional tests
#   update, query and rgylogin ( in directory test/security/commands/rgy ) .
#   The update, query and rgylogin  must be built and the install pass
#   pass run in the directory test/security/commands/rgy . The install
#   installs the executables in /tet/security/ts/secrep  ).
#   
#   N.B. The install pass must be run as root ( because install tries
#   to chown the files and chown command required root access. )

profile_IDIR		= /tet/security/ts/secrep/
replog_IDIR		= /tet/security/ts/secrep/
rgy_IDIR		= /tet/security/ts/secrep/
secd_IDIR		= /tet/security/ts/secrep/
secd_kill_remote_IDIR	= /tet/security/ts/secrep/
repdb_IDIR		= /tet/security/ts/secrep/
sa_IDIR			= /tet/security/ts/secrep/
replist_IDIR		= /tet/security/ts/secrep/
slv_IDIR		= /tet/security/ts/secrep/
xconfig_IDIR		= /tet/security/ts/secrep/
config_slv_IDIR		= /tet/security/ts/secrep/
sft_IDIR		= /tet/security/ts/secrep/
tc_common_IDIR		= /tet/security/ts/secrep/
tc_slv_del_IDIR		= /tet/security/ts/secrep/
tc_slv_delfrc_IDIR	= /tet/security/ts/secrep/
tc_slv_destroy_IDIR	= /tet/security/ts/secrep/
tc_slv_initrep_IDIR	= /tet/security/ts/secrep/
tc_slv_login_IDIR 	= /tet/security/ts/secrep/
tc_slv_chgkey_IDIR 	= /tet/security/ts/secrep/
tc_concupd_IDIR		= /tet/security/ts/secrep/
tc_bakrest_IDIR		= /tet/security/ts/secrep/
tc_bm_IDIR 		= /tet/security/ts/secrep/
tc_bm_restart_IDIR 	= /tet/security/ts/secrep/
tc_cm_basic_IDIR 	= /tet/security/ts/secrep/
tc_cm_c_update_IDIR 	= /tet/security/ts/secrep/
tc_cm_login_query 	= /tet/security/ts/secrep/
tc_cm_q_update_IDIR 	= /tet/security/ts/secrep/
tc_cm_restart_IDIR 	= /tet/security/ts/secrep/
tc_cm_slvdel		= /tet/security/ts/secrep/
tc_cm_slvinit_IDIR 	= /tet/security/ts/secrep/
tc_secrep_cleanup_IDIR 	= /tet/security/ts/secrep/

ILIST   		= ${SCRIPTS}

SCRIPTS			= profile replog rgy secd secd_kill_remote repdb sa \
			  replist slv xconfig config_slv \
                          sft tc_common \
                          tc_slv_del         tc_slv_delfrc   tc_slv_chgkey    \
			  tc_slv_destroy     tc_slv_initrep  tc_slv_login     \
			  tc_concupd         tc_bakrest      tc_bm            \
                          tc_bm_restart      tc_cm_basic     tc_cm_c_update   \
                          tc_cm_login_query  tc_cm_q_update  tc_cm_restart    \
                          tc_cm_slvdel       tc_cm_slvinit   tc_secrep_cleanup

.include <${RULES_MK}>
