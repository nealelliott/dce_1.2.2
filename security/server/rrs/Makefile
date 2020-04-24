# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.12.3  1996/03/09  23:30:02  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:22  marty]
#
# Revision 1.1.12.2  1995/12/08  17:47:46  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/29  22:53 UTC  rps
# 	machdep.mk
# 
# 	HP revision /main/HPDCE02/rps_moop/2  1995/06/29  03:49 UTC  rps
# 	include machdep
# 
# 	HP revision /main/HPDCE02/rps_moop/1  1995/06/16  00:01 UTC  rps
# 	PIC code not needed
# 
# 	HP revision /main/HPDCE02/1  1995/05/15  21:04 UTC  greg
# 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
# 	[1995/04/21  19:53 UTC  greg  /main/SEC_migrate_merge/1]
# 
# 	Merge .../greg_sec_migrate/LATEST --> .../SEC_migrate/LATEST.
# 	[1995/02/11  22:14 UTC  greg  /main/SEC_migrate/2]
# 
# 	Test my theory that we don't need to be building idl-derived
# 	objects for interfaces that are already present in libdce
# 	(rs_plcy_cstub.o, rs_replist.h, etc).
# 	[1995/02/03  13:32 UTC  greg  /main/SEC_migrate/greg_sec_migrate/1]
# 
# 	Add compatibility code for rrs_replist_read_full().
# 
# 	[1995/01/30  04:45 UTC  greg  /main/SEC_migrate/1]
# 
# Revision 1.1.8.1  1994/10/26  20:00:47  hondo
# 	changes for replication of login activity.
# 	[1994/10/24  20:05:56  hondo]
# 
# Revision 1.1.5.3  1994/09/07  15:48:37  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:34  marty]
# 
# Revision 1.1.5.2  1994/06/03  13:39:21  mdf
# 	Added dependency information for rs_repadm and rs_replist
# 	[1994/06/03  12:39:20  mdf]
# 
# Revision 1.1.5.1  1994/06/02  21:26:51  mdf
# 	hp_sec_to_osf_3 drop, merge up with latest.
# 	[1994/05/24  15:17:08  mdf]
# 
# 	hp_sec_to_osf_3 drop
# 
# 	Move -I../../client/rca to -I../../utils because of generated header
# 	file move.
# 	[1993/10/19  18:42:24  sommerfeld]
# 
# 	  Second replication code drop: bl5
# 	[1992/10/06  15:53:26  burati]
# 
# Revision 1.1.2.3  1993/09/15  15:49:15  root
# 	    Loading drop DCE1_0_3b03
# 
# Revision 1.1.2.6  1993/01/29  18:53:44  burati
# 	Find client side of rep interface in the correct place (client/rca)
# 	[1993/01/18  14:51:09  burati]
# 
# Revision 1.1.2.5  1993/01/26  23:02:03  emartin
# 	bl6d: add rs_plcy.idl related entries
# 	[1993/01/08  16:11:46  emartin]
# 
# Revision 1.1.2.4  1992/12/29  16:33:39  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  21:21:35  zeliff]
# 
# Revision 1.1.2.3  1992/10/07  20:29:59  sekhar
# 	[OT 5566]  Bl5 Replication code drop from HP.
# 	[1992/10/07  19:14:14  sekhar]
# 
# Revision 1.1.2.2  1992/08/31  18:22:38  sekhar
# 	First replication code drop.
# 	[1992/08/31  17:49:59  sekhar]
# 
# Revision 1.1  1992/01/19  14:44:26  devrcs
# 	Initial revision
# 
# $EndLog$
#


#
# Copyright (c) Hewlett-Packard Company 1991, 1992, 1993
# Unpublished work. All Rights Reserved.
#
# DCE Security Services 
# /server/rrs Makefile
#

# Additional places to search for files
VPATH = ../../idl

# Flags for the NIDL compiler
IDLINCFLAGS = -I. -I../../idl 
IDLFLAGS =  -keep c_source -cepv 
IDLFILES =      \
                rs_prop_acct.idl    \
                rs_prop_acl.idl     \
                rs_prop_attr.idl     \
                rs_prop_attr_schema.idl     \
                rs_prop_login_reset.idl     \
                rs_prop_pgo.idl     \
                rs_prop_plcy.idl    \
                rs_prop_replist.idl \
                rs_repmgr.idl

DATAFILES =

IDLDATAFILES =  \
                rs_prop_acct.h      \
                rs_prop_acl.h       \
                rs_prop_attr.h     \
                rs_prop_attr_schema.h     \
                rs_prop_login_reset.h     \
                rs_prop_pgo.h       \
                rs_prop_plcy.h      \
                rs_prop_replist.h   \
                rs_repmgr.h     

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../h -I../../idl -I../../utils

RRSLIB = \
                rrs_prop_acct.o     \
                rrs_prop_acl.o      \
                rrs_prop_attr.o     \
                rrs_prop_attr_schema.o     \
                rrs_prop_login_reset.o     \
                rrs_prop_pgo.o      \
                rrs_prop_plcy.o     \
                rrs_prop_replist.o  \
                rrs_prop_util.o     \
                rrs_rep_remote.o    \
                rrs_repmgr.o

RRSSTUBS = \
                rs_prop_acct_cstub.o    \
                rs_prop_acl_cstub.o     \
                rs_prop_attr_cstub.o     \
                rs_prop_attr_schema_cstub.o     \
                rs_prop_login_reset_cstub.o     \
                rs_prop_pgo_cstub.o     \
                rs_prop_plcy_cstub.o    \
                rs_prop_replist_cstub.o \
                rs_repmgr_cstub.o       

RRS_OBJS =  ${RRSSTUBS} ${RRSLIB}


# Libraries to build
LIBRARIES = librrs.a
EXPLIB_TARGETS = librrs.a

librrs.a_OFILES = ${RRS_OBJS}


.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
