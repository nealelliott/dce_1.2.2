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
# Revision 1.2.20.2  1996/03/11  02:26:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:59:21  marty]
#
# Revision 1.2.20.1  1995/12/11  20:02:31  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:42:39  root]
# 
# Revision 1.2.18.1  1994/09/07  15:48:43  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:39  marty]
# 
# Revision 1.2.16.1  1993/09/27  18:18:03  tom
# 	Bug 8524 - Remove EXPDIR and IDIR, let it default to:
# 	 dcetest/dce1.0/rpc/rtandidl/moretests.
# 	[1993/09/27  18:17:30  tom]
# 
# Revision 1.2.8.8  1993/02/03  18:19:54  cuti
# 	Change IDIR contents and add ns_mgmt_exp_age entry
# 	[1993/02/01  22:31:59  cuti]
# 
# Revision 1.2.8.7  1993/02/01  21:33:30  hinman
# 	[hinman] - SVR4 changes
# 	[1993/02/01  21:04:19  hinman]
# 
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  16:29:56  hinman]
# 
# 	Revision 1.2.14.2  1993/01/11  16:14:56  hinman
# 	[hinman] - Check in merged SNI version
# 
# Revision 1.2.8.6  1993/01/11  22:10:46  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:03:14  bbelch]
# 
# Revision 9.5.1.4  92/11/24  10:18:30  greg
# 	Don't expand ${CFLAGS} into ${IDLFLAGS} on SVR4 platforms.
# 
# Revision 9.5.2.2  92/11/24  10:17:31  greg
# 	Don't expand ${CFLAGS} into ${IDLFLAGS} on SVR4 platforms.
# 
# Revision 9.5.1.3  92/11/06  09:21:31  hinman
# 	Auto checkin of OSF rev 1.2.8.5
# 
# Revision 1.2.8.5  1992/11/02  15:20:43  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:39:22  root]
# 
# Revision 1.2.8.4  1992/10/16  13:48:08  cuti
# 	ot5663: Add my_mgmt.h to dependency list of mgmt_inq_stats.o
# 	[1992/10/16  13:45:37  cuti]
# 
# Revision 1.2.8.3  1992/10/12  16:11:27  cuti
# 	Add my_mgmt_cstub.o to mgmt_inq_stats_OFILES
# 	[1992/10/12  14:34:52  cuti]
# 
# Revision 1.2.8.2  1992/10/02  18:04:40  markar
# 	  OT CR 4938 fix: removed extraneous CFLAG declarations.
# 	[1992/09/22  18:15:10  markar]
# 
# Revision 1.2.2.7  1992/07/21  20:30:32  dceguest
# 	Added dependency files mgmt_copy.h and mgmts.h for ns_binding_import.c,
# 	ns_binding_export.c, ns_binding_lookup.c, ns_binding_unexport.c
# 	[1992/07/21  15:13:15  dceguest]
# 
# Revision 1.2.2.6  1992/07/14  21:35:59  cuti
# 	Change to -keep c_source in IDLFLAGS to avoid using default compiler in stub files.
# 	[1992/07/14  21:18:11  cuti]
# 
# Revision 1.2.2.5  1992/07/09  21:32:22  smith
# 	Added dependencies for pgo_util.o and sofserv.o
# 	[1992/07/09  19:32:56  smith]
# 
# Revision 1.2.2.4  1992/06/26  20:05:59  rsalz
# 	Fix previous change; remove built-in IDL rules.
# 	[1992/06/26  17:36:52  rsalz]
# 
# Revision 1.2.2.3  1992/06/24  21:59:56  rsalz
# 	Remove hard-coded IDL rules.
# 	[1992/06/23  21:18:54  rsalz]
# 
# Revision 1.2.2.2  1992/04/07  19:20:22  mckeen
# 	Added comtest_client_xfr.o dependencies
# 	[1992/03/13  15:47:55  mckeen]
# 
# Revision 1.2  1992/01/19  22:14:01  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  Copyright (c) 1990 by
#      Hewlett-Packard Company, Palo Alto, Ca. &
#      Digital Equipment Corporation, Maynard, Mass. &
# 	Locus Computing Corporation, Burlington, Mass.
#
#
#
#  Makefile for the tsh test programs
#
# 
#  TSH RUNTIME 
#	Extracted	11/25/90 13:33:26
#	from delta on	11/25/90 13:33:05
#

LDFLAGS                 = ${${TARGET_MACHINE}_LDFLAGS}

CFLAGS                  = ${${TARGET_MACHINE}_CFLAGS} 
group_mbr_inquiry.o_CFLAGS = -DDNS_CDS ${${TARGET_MACHINE}_CFLAGS} 

LIBS                    = -ldce ${${TARGET_MACHINE}_LIBS}


PROGRAMS    =  if_reg_auth_info             sofserv                    \
               svr_reg_auth_info            binding_server_to_client   \
	       rpc_server                   mgmt_inq_stats             \
	       mgmt_set_server_stack	    object_inq_type            \
               mgmt_inq_dflt_authn_level    mgmt_set_authz_fn          \
               set_inq_auth_info            sofservclient              \
               enumc                        scalc                      \
               recc                         strzc                      \
               specc                        iattrc                     \
               pattrc                       aryc                       \
               ihc                          tattrc                     \
               rattrc                       vrecc                      \
               pipec                        outoflinec                 \
               repasc                       stringc                    \
               object_set_type              uuid_compare               \
               uuid_create                  uuid_create_nil            \
               uuid_equal                   uuid_from_string           \
               uuid_hash                    uuid_is_nil                \
               uuid_to_string                                          \
               string_binding_compose       string_binding_parse       \
               binding_from_string          string_free                \
               binding_copy                 binding_free               \
               binding_inq_object           binding_reset              \
               binding_set_object           binding_vector_free        \
               error_inq_text               \
               mgmt_inq_com_timeout         network_inq_protseqs       \
               network_inq_protseqs_valid   group_mbr_add_remove       \
               group_mbr_inquiry            ns_profile_elt_add         \
               ns_profile_elt_inq           ns_profile_delete          \
               ns_mgmt_entry_create         ns_mgmt_entry_delete       \
               ns_mgmt_binding_unexport     ns_binding_import          \
               ns_binding_export            ns_binding_lookup          \
               ns_binding_select            ns_binding_unexport        \
               ns_entry_object_inq          ns_entry_inq_if_ids        \
               mgmt_ep_elt_inq              mgmt_inq_if_ids            \
               mgmt_server                  mgmt_set_com_timeout       \
               mgmt_stop_server_listening   \
               tstdriver                    tstsrvr                    \
               server                       ns_mgmt_exp_age                           

ILIST		= ${PROGRAMS}

server_OFILES = enumm.o						       \
               enum_sstub.o                 ihm.o                      \
               ih_sstub.o                   scalm.o                    \
               scal_sstub.o                 recm.o                     \
               rec_sstub.o                  strzm.o                    \
               strz_sstub.o                 specm.o                    \
               spec_sstub.o                 arym.o                     \
               ary_sstub.o                  rattrm.o                   \
               rattr_sstub.o                tattrm.o                   \
               tattr_sstub.o                vrecm.o                    \
               vrec_sstub.o                 pipem.o                    \
               pipe_sstub.o                 pipe_saux.o                \
               outofline_sstub.o            outofline_saux.o           \
               outoflinem.o                 pattrm.o                   \
               pattr_sstub.o                repasm.o                   \
               repas_sstub.o                stringm.o                  \
	       string_sstub.o               server.o
tstdriver_OFILES = tstdriver.o              str_binding_tsts.o         \
               bind_handle_tsts.o           interface_obj_tsts.o       \
               network_obj_tsts.o           tsttab.o                   \
               uuid_vers_tsts.o             comtest_client_xfr.o       \
               comtest_server_if_cstub.o  \
               comtest_interface_x_cstub.o  comtest_interface_y_cstub.o 

tstsrvr_OFILES = tstdriver.o                server_obj_tsts.o          \
               comtest_interface_x.o        comtest_interface_y.o      \
               comtest_interface_x_sstub.o  comtest_interface_y_sstub.o \
               comtest_server_if_sstub.o    srvrtab.o			\
	       comtest_interface_x_cstub.o  comtest_interface_y_cstub.o 
               
object_set_type_OFILES        = object_set_type.o        main.o 
uuid_compare_OFILES           = uuid_compare.o           main.o
uuid_create_OFILES            = uuid_create.o            main.o
uuid_create_nil_OFILES        = uuid_create_nil.o        main.o
uuid_equal_OFILES             = uuid_equal.o             main.o
uuid_from_string_OFILES       = uuid_from_string.o       main.o
uuid_hash_OFILES              = uuid_hash.o              main.o
uuid_is_nil_OFILES            = uuid_is_nil.o            main.o
uuid_to_string_OFILES         = uuid_to_string.o         main.o
string_binding_compose_OFILES = string_binding_compose.o main.o
string_binding_parse_OFILES   = string_binding_parse.o   main.o
binding_from_string_OFILES    = binding_from_string.o    main.o
string_free_OFILES            = string_free.o            main.o
binding_copy_OFILES           = binding_copy.o           main.o
binding_free_OFILES           = binding_free.o           main.o
binding_inq_object_OFILES     = binding_inq_object.o     main.o
binding_set_object_OFILES     = binding_set_object.o     main.o
binding_reset_OFILES          = binding_reset.o          main.o
binding_vector_free_OFILES    = binding_vector_free.o    main.o  \
				fake_cstub.o		 fake_sstub.o
error_inq_text_OFILES         = error_inq_text.o         main.o
mgmt_inq_com_timeout_OFILES   = mgmt_inq_com_timeout.o   main.o
network_inq_protseqs_OFILES   = network_inq_protseqs.o   main.o
network_inq_protseqs_valid_OFILES = network_inq_protseqs_valid.o  main.o 
group_mbr_add_remove_OFILES   = group_mbr_add_remove.o   main.o
group_mbr_inquiry_OFILES      = group_mbr_inquiry.o      main.o
ns_profile_elt_add_OFILES     = ns_profile_elt_add.o     main.o   ns_utility.o
ns_profile_elt_inq_OFILES     = ns_profile_elt_inq.o     main.o   ns_utility.o
ns_profile_delete_OFILES      = ns_profile_delete.o      main.o   ns_utility.o
ns_mgmt_entry_create_OFILES   = ns_mgmt_entry_create.o   main.o   ns_utility.o
ns_mgmt_entry_delete_OFILES   = ns_mgmt_entry_delete.o   main.o   ns_utility.o
ns_mgmt_binding_unexport_OFILES = ns_mgmt_binding_unexport.o main.o ns_utility.o
ns_mgmt_exp_age_OFILES			= ns_mgmt_exp_age.o main.o
ns_binding_import_OFILES      = ns_binding_import.o      mgmts_sstub.o mgmt_copy_sstub.o main.o ns_utility.o
ns_binding_export_OFILES      = ns_binding_export.o      mgmts_sstub.o mgmt_copy_sstub.o main.o   ns_utility.o
ns_binding_lookup_OFILES      = ns_binding_lookup.o      mgmts_sstub.o mgmt_copy_sstub.o main.o   ns_utility.o
ns_binding_select_OFILES      = ns_binding_select.o      main.o   ns_utility.o
ns_binding_unexport_OFILES    = ns_binding_unexport.o    mgmts_sstub.o mgmt_copy_sstub.o main.o   ns_utility.o
ns_entry_object_inq_OFILES    = ns_entry_object_inq.o    main.o   ns_utility.o
ns_entry_inq_if_ids_OFILES    = ns_entry_inq_if_ids.o    main.o   ns_utility.o
sofserv_OFILES                = test_rpc_auth_sstub.o    sofserv.o pgo_util.o
sofservclient_OFILES          = test_rpc_auth_cstub.o    sofservclient.o \
				main.o                   pgo_util.o
aryc_OFILES                   = ary_cstub.o    util.o    main.o   aryc.o
enumc_OFILES                  = enum_cstub.o             main.o   enumc.o
iattrc_OFILES                 = iattr_cstub.o            main.o   iattrc.o
ihc_OFILES                    = ih_cstub.o               main.o   ihc.o
outoflinec_OFILES             = outofline_cstub.o        main.o   outoflinec.o \
                                outofline_caux.o
pattrc_OFILES                 = pattr_cstub.o            main.o   pattrc.o
pipec_OFILES                  = pipe_cstub.o             main.o   pipec.o 
rattrc_OFILES                 = rattr_cstub.o            main.o   rattrc.o
recc_OFILES                   = rec_cstub.o              main.o   recc.o
scalc_OFILES                  = scal_cstub.o   util.o    main.o   scalc.o 
specc_OFILES                  = spec_cstub.o             main.o   specc.o
strzc_OFILES                  = strz_cstub.o             main.o   strzc.o
tattrc_OFILES                 = tattr_cstub.o            main.o   tattrc.o
vrecc_OFILES                  = vrec_cstub.o   util.o    main.o   vrecc.o  
repasc_OFILES                 = repas_cstub.o            main.o   repasc.o
stringc_OFILES                = string_cstub.o           main.o   stringc.o
if_reg_auth_info_OFILES          = if_reg_auth_info.o           main.o   \
                                    rpc_auth_stubs.o            pgo_util.o
mgmt_inq_dflt_authn_level_OFILES = mgmt_inq_dflt_authn_level.o  main.o
mgmt_set_authz_fn_OFILES         = mgmt_set_authz_fn.o          main.o   \
                                    rpc_auth_stubs.o
rpc_auth_stubs_OFILES            = rpc_auth_stubs.o             main.o
set_inq_auth_info_OFILES         = set_inq_auth_info.o          main.o  pgo_util.o
svr_reg_auth_info_OFILES         = svr_reg_auth_info.o          main.o  pgo_util.o
mgmt_ep_elt_inq_OFILES		 = mgmt_ep_elt_inq.o mgmts_sstub.o mgmt_copy_sstub.o main.o
mgmt_inq_if_ids_OFILES		 = mgmt_inq_if_ids.o my_mgmt_cstub.o main.o
mgmt_server_OFILES    		 = mgmt_server.o my_mgmt_sstub.o 
mgmt_set_com_timeout_OFILES	 = mgmt_set_com_timeout.o       main.o
mgmt_stop_server_listening_OFILES = mgmt_stop_server_listening.o my_mgmt_cstub.o main.o
binding_server_to_client_OFILES   = binding_server_to_client.o  main.o \
				    binding_server_cstub.o
rpc_server_OFILES                 = rpc_server.o  \
				    binding_server_sstub.o
mgmt_set_server_stack_OFILES      = mgmt_set_server_stack.o     main.o
mgmt_inq_stats_OFILES             = mgmt_inq_stats.o   my_mgmt_cstub.o  main.o
object_inq_type_OFILES            = object_inq_type.o           main.o

IDLFILES                = ary.idl binding_server.idl \
	comtest_interface_x.idl comtest_interface_y.idl \
	comtest_server_if.idl enum.idl fake.idl iattr.idl ih.idl \
	mgmt_copy.idl mgmts.idl my_mgmt.idl outofline.idl pattr.idl \
	pipe.idl rattr.idl rec.idl repas.idl scal.idl spec.idl string.idl \
	strz.idl tattr.idl test_rpc_auth.idl vrec.idl

ary_sstub.o	ary_cstub.o   ary.h:	ary.idl
enum_sstub.o  enum_cstub.o  enum.h:	enum.idl
iattr_cstub.o iattr_sstub.o iattr.h:	iattr.idl
ih_cstub.o    ih_sstub.o    ih.h:	ih.idl
outofline_cstub.o outofline_caux.o outofline_sstub.o outofline.h:	outofline.idl
pattr_cstub.o pattr_sstub.o pattr.h:	pattr.idl
pipe_cstub.o  pipe_sstub.o  pipe.h:	pipe.idl
rattr_cstub.o rattr_sstub.o rattr.h:	rattr.idl
rec_cstub.o   rec_sstub.o   rec.h:	rec.idl
scal_cstub.o  scal_sstub.o  scal.h:	scal.idl
spec_cstub.o  spec_sstub.o  spec.h:	spec.idl
strz_cstub.o  strz_sstub.o  strz.h:	strz.idl
tattr_cstub.o tattr_sstub.o tattr.h:	tattr.idl
vrec_cstub.o  vrec_sstub.o  vrec.h:	vrec.idl
repas_cstub.o repas_sstub.o repas.h:	repas.idl
test_rpc_auth_cstub.o test_rpc_auth_sstub.o test_rpc_auth.h: test_rpc_auth.idl
string_cstub.o string_sstub.o string.h:	string.idl
comtest_server_if_cstub.o comtest_server_if_sstub.o    comtest_server_if.h:	comtest_server_if.idl
comtest_interface_x_cstub.o comtest_interface_x_sstub.o comtest_interface_x.h:	comtest_interface_x.idl
comtest_interface_y_cstub.o comtest_interface_y_sstub comtest_interface_y.h:	comtest_interface_y.idl
comtest_ep_cstub.o comtest_ep_sstub.o comtest_ep.h: comtest_ep.idl
my_mgmt_cstub.o   my_mgmt_sstub.o   my_mgmt.h:   my_mgmt.idl
mgmt_copy_cstub.o mgmt_copy_sstub.o mgmt_copy.h: mgmt_copy.idl
mgmts_cstub.o     mgmts_sstub.o     mgmts.h:     mgmts.idl
fake_cstub.o      fake_sstub.o      fake.h:      fake.idl
binding_server_cstub.o binding_server_sstub.o   binding_server.h: binding_server.idl

aryc.o:			ary.h
aryc.o:			scal.h
arym.o:			ary.h
binding_copy.o:		tsh_string.h
binding_free.o:		tsh_string.h
binding_from_string.o:	tsh_string.h
binding_inq_object.o:	tsh_string.h
binding_reset.o:	tsh_string.h
binding_server_to_client.o: binding_server.h
binding_set_object.o:	tsh_string.h
binding_vector_free.o:	fake.h
comtest_interface_x.o:	comtest_interface_x.h
comtest_interface_y.o:	comtest_interface_y.h
enumc.o:		enum.h
enumm.o:		enum.h
iattrc.o:		iattr.h
ihc.o:			ih.h
ihm.o:			ih.h
mgmt_ep_elt_inq.o:	mgmt_copy.h
mgmt_ep_elt_inq.o:	mgmts.h
mgmt_inq_if_ids.o:	my_mgmt.h
mgmt_inq_stats.o:	my_mgmt.h
mgmt_server.o:		my_mgmt.h
mgmt_stop_server_listening.o: my_mgmt.h
ns_binding_export.o:    mgmt_copy.h
ns_binding_export.o:    mgmts.h
ns_binding_import.o:    mgmt_copy.h
ns_binding_import.o:    mgmts.h
ns_binding_lookup.o:    mgmt_copy.h
ns_binding_lookup.o:    mgmts.h
ns_binding_unexport.o:  mgmt_copy.h
ns_binding_unexport.o:  mgmts.h
outoflinec.o:		outofline.h
outoflinem.o:		outofline.h
pattrc.o:		pattr.h
pattrm.o:		pattr.h
pipec.o:		pipe.h
pipem.o:		pipe.h
rattrc.o:		rattr.h
rattrm.o:		rattr.h
recc.o:			rec.h
recm.o:			rec.h
repasc.o:		repas.h
repasm.o:		repas.h
rpc_server.o:		binding_server.h
scalc.o:		scal.h
scalm.o:		scal.h
server.o:		enum.h
server.o:		ih.h
server.o:		scal.h
server.o:		rec.h
server.o:		strz.h
server.o:		spec.h
server.o:		rattr.h
server.o:		ary.h
server.o:		tattr.h
server.o:		vrec.h
server.o:		pipe.h
server.o:		outofline.h
server.o:		pattr.h
server.o:		repas.h
server.o:		string.h
sofserv.o:		test_rpc_auth.h
sofservclient.o:	test_rpc_auth.h
specc.o:		spec.h
specm.o:		spec.h
str_binding_tsts.o:	string.h
string_binding_compose.o: tsh_string.h
string_binding_parse.o:	tsh_string.h
string_free.o:		tsh_string.h
stringc.o:		string.h
stringm.o:		scal.h
stringm.o:		string.h
strzc.o:		strz.h
strzm.o:		scal.h
strzm.o:		strz.h
tattrc.o:		tattr.h
tattrm.o:		tattr.h
vrecc.o:		vrec.h
vrecm.o:		vrec.h
# These are in comtest_client_xfr.h:
comtest_client_xfr.o:	comtest_server_if.h comtest_interface_x.h \
			comtest_interface_y.h
# These are in server_obj_tsts.h
server_obj_tsts.o:	comtest_server_if.h comtest_interface_x.h \
			comtest_interface_y.h
.include <${RULES_MK}>
.if (${USE_SHARED_LIBRARIES} && defined (__OSF__) )
IDL_CC_OPT      = -cc_opt "${CFLAGS} -pic-lib -L/usr/shlib -ldce"
.endif
#IDLFLAGS                = ${CFLAGS} ${INCDIRS} -keep both ${IDL_CC_OPT}
#IDLFLAGS                = -keep both ${CFLAGS}
.if ${TARGET_MACHINE}=="SVR4"
IDLFLAGS                = -keep c_source  
.else
IDLFLAGS                = -keep c_source ${CFLAGS}
.endif












