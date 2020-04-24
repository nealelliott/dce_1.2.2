# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.16.3  1996/10/15  20:56:10  arvind
# 	OSF DCE 1.2.2 Drop 4.5
# 	[1996/10/15  20:21:28  arvind]
#
# Revision 1.1.16.2  1996/10/04  16:47:27  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/04  16:47:14  arvind]
# 
# Revision /main/DCE_1.2.2/2  1996/10/02  18:28 UTC  cuti
# 	Merge of cuti_pk_fix
# 
# Revision /main/DCE_1.2.2/cuti_pk_fix/1  1996/09/26  16:19 UTC  cuti
# 	Make international works
# 
# Revision /main/DCE_1.2.2/1  1996/09/16  20:29 UTC  aha
# 	CHFts19896: public key version support
# 
# Revision /main/aha_pk9_3/1  1996/09/13  23:43 UTC  aha
# 	Set INCFLAGS to include psm/domestic/ for use by rsdb_acct.c
# 
# Revision 1.1.12.3  1996/03/09  23:30:21  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:38  marty]
# 
# Revision 1.1.12.2  1995/12/08  17:57:51  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/29  22:52 UTC  rps
# 	machdep.mk
# 
# 	HP revision /main/HPDCE02/rps_moop/2  1995/06/29  03:50 UTC  rps
# 	+O2
# 
# 	HP revision /main/HPDCE02/rps_moop/1  1995/06/16  00:01 UTC  rps
# 	PIC code not needed
# 
# 	HP revision /main/HPDCE02/1  1995/05/15  21:10 UTC  greg
# 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
# 	[1995/04/21  12:57 UTC  greg  /main/SEC_migrate_merge/1]
# 
# 	Merge greg_sec_migrate_acls branch.
# 	[1995/04/18  18:20 UTC  greg  /main/SEC_migrate/1]
# 
# 	Add ../../idl to lINCDIRS to pick up sec_daclmgr.h
# 	[1995/04/13  17:10 UTC  greg  /main/greg_sec_migrate_acls/1]
# 
# Revision 1.1.8.2  1994/07/15  15:02:28  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/07/14  17:22:24  mdf]
# 
# Revision 1.1.8.1  1994/03/02  17:55:53  hanfei
# 	Initial security drop
# 	[1994/02/18  20:52:16  hanfei]
# 
# 	HP revision /main/ODESSA_ERA2/1  1994/01/28  20:10  mdf
# 	Merge forward.
# 
# Revision 1.1.6.3  1993/02/19  17:14:12  ahop
# 	dbcp:add rsdb_compat.o
# 	[1993/02/19  08:43:35  ahop]
# 
# Revision 1.1.6.2  1992/12/29  16:38:01  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  21:27:30  zeliff]
# 
# Revision 1.1.3.2  1992/06/02  21:16:37  sekhar
# 	Dropped Unix interface changes made by frisco
# 	[1992/06/01  21:17:00  sekhar]
# 
# Revision 1.1.1.2  1992/05/22  19:36:34  frisco
# 	Add Unix interface support
# 
# Revision 1.1  1992/01/19  14:44:56  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=3 05/21/92 //littl/prgy/src/server/rsdb
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /server/rsdb Makefile
#

# Compiler flags to use in this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../h -I../../idl

# Libraries to export
LIBRARIES = librsdb.a

RSDB_OBJS = 	balanced_trees.o   \
		kdb_rsdb.o         \
		rsdb.o             \
		rsdb_acct.o        \
		rsdb_acl.o         \
		rsdb_auth.o        \
		rsdb_attr.o \
		rsdb_attr_schema.o \
		rsdb_compat.o      \
		rsdb_create.o      \
		rsdb_credentials.o \
		rsdb_daclmgr.o     \
		rsdb_dir.o         \
		rsdb_dir_util.o    \
		rsdb_file.o        \
		rsdb_journal.o     \
		rsdb_log.o         \
		rsdb_login.o       \
		rsdb_misc.o        \
		rsdb_name_util.o   \
		rsdb_pgo.o         \
		rsdb_pgo_util.o    \
		rsdb_policy.o      \
		rsdb_replica.o     \
		rsdb_sysdep.o      \
		rsdb_unix.o        \
		rsdb_util.o 

#rsdb_acct.o_INCFLAGS= ${INCFLAGS} -I../../psm/domestic
.if !defined (USE_DES)
rsdb_acct.o_CFLAGS = ${CFLAGS} -DNOENCRYPTION
.endif

librsdb.a_OFILES = ${RSDB_OBJS}
EXPLIB_TARGETS   = librsdb.a


.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
