#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.39.2  1996/03/09  23:11:59  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:16  marty]
#
# Revision 1.1.39.1  1995/12/08  15:27:59  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:47:59  root]
# 
# Revision 1.1.36.9  1994/08/19  21:10:38  zee
# 	      Add server_badclock.o.
# 	[1994/08/19  18:08:43  zee]
# 
# Revision 1.1.36.8  1994/08/16  18:27:59  jd
# 	drop from janet
# 	[1994/08/13  16:06:50  jd]
# 
# Revision 1.1.36.7  1994/08/03  19:03:22  mccann
# 	final sams cleanup drop
# 	[1994/08/01  20:01:46  mccann]
# 
# Revision 1.1.36.6  1994/07/25  15:14:53  proulx
# 	Use common ACL manager for subcomponent mgmt ACLs
# 	[1994/07/25  13:56:13  proulx]
# 
# Revision 1.1.36.5  1994/06/09  18:42:40  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:14:54  devsrc]
# 
# Revision 1.1.36.4  1994/05/06  16:04:13  zee
# 	HCell BL4 support - add modules back_hcell_connections.o, server_hcell_maint.o
# 
# 	HCell BL4 support:
# 	new modules back_hcell_connections.o, server_hcell_maint.o
# 	Revision 1.1.36.3  1994/03/22  20:00:15  griffin
# 	HCell BL3 support: cprpc_hcell interface.
# 	[1994/03/22  18:18:25  griffin]
# 
# 	HCell BL3 support: New modules (server_hcell_support), etc.
# 	[1994/05/05  21:18:28  zee]
# 
# Revision 1.1.36.2  1994/03/12  22:09:29  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  14:36:45  peckham]
# 
# Revision 1.1.36.1  1994/01/28  23:09:02  burati
# 	Delegation/EPAC changes - use rdaclifv0 until new one integrated (dlg_bl1)
# 	[1994/01/18  21:27:11  burati]
# 
# Revision 1.1.34.1  1993/10/15  21:19:03  ohara
# 	expand the VPATH to use context, needed for ODE2.3 on SVR4
# 	[1993/10/15  16:12:57  ohara]
# 
# Revision 1.1.2.2  1992/12/30  14:18:13  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  23:18:25  zeliff]
# 
# Revision 1.1  1992/01/19  15:26:22  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
#
# THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
# ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
# INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
# COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
# OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
# TRANSFERRED.
#
# THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
# AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
# CORPORATION.
#
# DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
# SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
#
# Makefile for local naming daemon 
#

VPATH		= ../stubs:../../../../export/${target_machine}/usr/include/dce

# programs to build
PROGRAMS	= cdsd cdsd_diag cds_dbdump 
#PROGRAMS	= cdsd 

# install directory
IDIR		= /bin/

# things to install
ILIST		= cdsd

# libs for binding
LIBS		= ${${TARGET_MACHINE}_LIBS}

# flags for compiling
CFLAGS		= ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS	= -I../stubs -I../includes

LDFLAGS		= ${${TARGET_MACHINE}_LDFLAGS}

# lists of objects for programs

rdaclif_sstub.c_IDLFLAGS = -no_mepv
rdaclifv0_sstub.c_IDLFLAGS = -no_mepv

IDLFILES	= cds_clerkserver.idl cds_types.idl cprpc_server.idl \
	          rdaclifv0.idl rdaclif.idl cprpc_hcell.idl

cds_clerkserver_ACF =
cds_types_ACF =
cprpc_server_ACF =
cprpc_hcell_ACF =

cdsd_diag_OFILES = sdi_ui.o

cds_dbdump_OFILES = dump_files.o dump_navigator.o dump_type_interpreters.o 

cdsd_OFILES = \
	back_collect.o \
	back_combine.o \
	back_copy_updates.o \
	back_do_updates.o \
	back_gather_updates.o \
	back_ground.o \
	back_hcell_connections.o \
	back_parentpointer.o \
	back_propagate.o \
	back_replicas.o \
	back_skulk.o \
	back_spread.o \
	back_upgrade.o \
	db_btree.o \
	db_checkpoint.o \
	db_cleanup.o \
	db_common.o \
	db_compression.o \
	db_diags.o \
	db_directory.o \
	db_entry.o \
	db_hash.o \
	db_pseudo.o \
	db_tlog.o \
	db_unix.o \
	dns_service_ncl.o \
	dns_service_rpc.o \
	dump_navigator.o \
	dump_type_interpreters.o \
	event_lib.o \
	find_lib.o \
	mgmt_file.o \
	mgmt_init.o \
	names_lib.o \
	replica_lib.o \
	security_lib.o \
        server_badclock.o \
	server_diags.o \
	server_globals.o \
	server_hcell_maint.o \
	server_hcell_support.o \
	server_main.o \
	sets_lib.o \
	ta_adver.o \
	ta_cle.o \
	ta_create.o \
	ta_delete.o \
	ta_enumerate.o \
	ta_epoch.o \
	ta_link.o \
	ta_lookup.o \
	ta_mgmt.o \
	ta_modify.o \
	ta_read.o \
	ta_replica.o \
	ta_resolve.o \
	ta_skulk.o \
	ta_subs.o \
	ta_test.o \
	ta_testgroup.o \
	uid_lib.o \
	unix_lib.o \
	unix_cds_net.o \
	unix_stubs.o \
	unix_towerset_build.o \
	update_lib.o \
	rpc_dispatch.o \
	cds_clerkserver_sstub.o \
	cds_types_saux.o \
	cprpc_server_sstub.o \
	cprpc_hcell_sstub.o \
        server_svc.o 

GARBAGE = cds_clerkserver_sstub.c cds_types_saux.c cprpc_server_sstub.c \
	  rdaclifv0_sstub.c rdaclif_sstub.c cprpc_hcell_sstub.c


.include <${RULES_MK}>

.ifdef BLD_SEC
cdsd_OFILES += cds_dacl.o \
	rdaclifv0_sstub.o rdaclif_sstub.o 
.endif


