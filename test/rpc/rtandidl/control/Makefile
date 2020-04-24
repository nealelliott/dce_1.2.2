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
# Revision 1.1.10.2  1996/03/11  02:20:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:56:41  marty]
#
# Revision 1.1.10.1  1995/12/11  19:36:34  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/07/29  19:15 UTC  tatsu_s
# 	Merged Mothra upto DCE 1.1 bata.
# 
# 	HP revision /main/HPDCE01/1  1994/07/12  19:18 UTC  rmm
# 	Added rpc.fun.tet.
# 	[1995/12/11  19:19:15  root]
# 
# Revision 1.1.8.1  1993/11/02  22:02:04  tom
# 	Install scripts which run the tests.
# 	[1993/11/02  22:00:35  tom]
# 
# Revision 1.1.4.5  1993/03/20  22:38:06  weisman
# 	Fix for OT 7554: Failure to cleanup state files
# 	[1993/03/20  22:30:01  weisman]
# 
# Revision 1.1.4.4  1993/02/03  18:18:15  cuti
# 	Add ns_mgmt_exp_age.tsh and ns_mgmt_exp_age.tshrc
# 	[1993/02/01  22:29:34  cuti]
# 
# Revision 1.1.4.3  1993/01/11  21:15:27  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  14:56:01  bbelch]
# 
# Revision 1.1.4.2  1992/11/02  15:37:28  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:39:11  root]
# 
# Revision 1.1.2.2  1992/07/12  20:28:42  cuti
# 	Add missing tsh files.
# 	[1992/07/12  20:24:58  cuti]
# 
# Revision 1.1  1992/01/19  03:41:02  devrcs
# 	Initial revision
# 
# $EndLog$
#
# 
# Copyright 1989 by Apollo Computer, Inc.
# Copyright 1990 by Locus Computing Corp.
# 
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Apollo Computer, Inc not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Apollo Computer, Inc. makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
# 

SCRIPTS	= do_rpc_test run_client run_server rpc.fun.tet

DATAFILES  =  all_auth.tsh            ns_binding_unexport.tsh      \
        mgmt_inq_dflt_authn_level.tsh mgmt_set_authz_fn.tsh       \
        mgmt.tshrc                   uuid.tsh                     \
        auth.tshrc                   set_auth_info.tsh            \
        if_reg_auth_info.tsh         svr_reg_auth_info.tsh        \
        inq_auth_client.tsh          inq_auth_info.tsh            \
	all.tsh                      ns_entry_inq_if_ids.tsh      \
	all_binding.tsh              ns_entry_object_inq.tsh      \
	ns_mgmt_binding_unexport.tsh ns_mgmt_entry_create.tsh     \
	ns_mgmt_entry_delete.tsh     all_testdriver.tsh           \
	all_ns.tsh                   all_uuid.tsh                 \
	ary.tsh                      ns_profile_delete.tsh        \
	ary.tshrc                    ns_profile_elt_add.tsh       \
	binding.tsh                  ns_profile_elt_inq_begin.tsh \
	binding.tshrc                ns_profile_elt_inq_done.tsh  \
	binding_copy.tsh             ns_profile_elt_inq_next.tsh  \
	binding_free.tsh             ns_profile_elt_remove.tsh    \
	binding_from_string.tsh      object.tshrc                 \
	binding_inq_object.tsh       object_set_type.tsh          \
	binding_reset.tsh            outofline.tsh                \
	binding_set_object.tsh       outofline.tshrc              \
	binding_vector_free.tsh      pattr.tsh                    \
	pattr.tshrc                  pipe.tsh                     \
	pipe.tshrc                   protseqs.tsh                 \
	protseqs.tshrc               rattr.tsh                    \
	rattr.tshrc                  rec.tsh                      \
	rec.tshrc                    repas.tsh                    \
	client.tsh                   runclisrvr.sh                \
	client.tshrc                 cmd_line.tsh                 \
	clisrv.tsh                   runsrv.sh                    \
	cmd_line.tshrc               runsrvrif.sh                 \
	enum.tsh                     enum.tshrc                   \
	tattr.tsh                    runtsh.sh                    \
	error.tshrc                  scal.tsh                     \
	error_inq_text.tsh           scal.tshrc                   \
	group.tshrc                  server.tsh                   \
	group_mbr_add.tsh            server.tshrc                 \
	group_mbr_inq_begin.tsh      group_mbr_inq_done.tsh       \
	group_mbr_inq_next.tsh       spec.tsh                     \
	group_mbr_remove.tsh         spec.tshrc                   \
	iattr.tsh                    srvrif.tsh                   \
	iattr.tshrc                  string.tsh                   \
	ih.tsh                       string.tshrc                 \
	ih.tshrc                     string_compose.tsh           \
	interface.tsh                string_free.tsh              \
	interface.tshrc              string_parse.tsh             \
	memory.tsh                   strz.tsh                     \
	mgmt_inq_com_timeout.tsh     tshrc                        \
	network.tsh                  network.tshrc                \
	all_idl.tsh                  uuid.tshrc                   \
	nocode.tsh                   uuid_compare.tsh             \
	ns.tshrc                     uuid_create.tsh              \
	ns_binding_export.tsh        uuid_create_nil.tsh          \
	ns_binding_import_begin.tsh  uuid_equal.tsh               \
	ns_binding_import_done.tsh   uuid_from_string.tsh         \
	ns_binding_import_next.tsh   uuid_hash.tsh                \
	ns_binding_lookup_begin.tsh  uuid_is_nil.tsh              \
	ns_binding_lookup_done.tsh   uuid_to_string.tsh           \
	ns_binding_lookup_next.tsh   vrec.tsh                     \
	ns_binding_select.tsh        vrec.tshrc                   \
        cmd_line.acf                 cmd_line.idl                 \
        cmd_line.sh                  nocode.sh                    \
        nocode.acf                   nocode.idl                   \
	mgmt_set_com_timeout.tsh     mgmt_stop_server_listening.tsh \
        mgmt_inq_if_ids.tsh          mgmt_ep_elt_inq.tsh          \
        mgmt_ep_elt_inq.tshrc        all_runtime.tsh              \
        ep_elt_inq.tshrc             mgmt_inq_stats.tsh           \
		mgmt_set_server_stack.tsh  	binding_server_to_client.tsh  \
        binding_server_to_client.tshrc  object_inq_type.tsh       \
        ns_binding.tsh               ns_entry.tsh                 \
        ns_mgmt.tsh                  ns_profile.tsh               \
        all_mgmt.tsh                 run_client                   \
        run_server                                                \
        ns_mgmt_exp_age.tsh          ns_mgmt_exp_age.tshrc        \
	all_testdriver_cleanup.tsh

ILIST	= ${DATAFILES} ${SCRIPTS}
                    
.include <${RULES_MK}>
