# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  01:43:12  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:14:24  marty]
#
# Revision 1.1.4.1  1995/12/08  22:06:37  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:42:35  root]
# 
# Revision 1.1.2.6  1994/09/02  18:34:29  pwang
# 	Removed extra linked libraries [OT#11402]
# 	[1994/09/02  18:13:17  pwang]
# 
# Revision 1.1.2.5  1994/06/10  20:18:24  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:57:41  devsrc]
# 
# Revision 1.1.2.4  1994/05/26  00:29:55  baum
# 	Now getting rdaclifv0.idl
# 	[1994/05/26  00:29:22  baum]
# 
# Revision 1.1.2.3  1994/05/25  15:50:51  baum
# 	Changes to allow comp[ilation on HP
# 	[1994/05/25  15:50:29  baum]
# 
# Revision 1.1.2.2  1994/05/19  14:34:46  baum
# 	Resync tests with ACL & DB API changes
# 	[1994/05/19  14:34:10  baum]
# 
# Revision 1.1.2.1  1994/04/22  06:21:36  baum
# 	Initial Checkin
# 	[1994/04/22  06:21:24  baum]
# 
# $EndLog$
CFLAGS = ${${TARGET_MACHINE}_CFLAGS} -D _USE_TET
#CFLAGS = ${${TARGET_MACHINE}_CFLAGS}
STANDARD_TETLIBS= -ltettcm -ltetapi

rdaclifv0.idl_IDLFLAGS	= -keep c_source -no_mepv
IDLFILES	= acl_test_db.idl acl_test.idl rdaclifv0.idl
IDLINCFLAGS	= -I.
IDLFLAGS	= -keep c_source

acl_test_server_OFILES	= acl_test_sstub.o acl_test_server.o \
			  acl_test_db_cstub.o svrUtils.o rdaclifv0_sstub.o \
			  rdaclifv0_cstub.o

acl_setup_OFILES = acl_setup.o

rdacl_svr_opers_OFILES = acl_test_cstub.o rdacl_svr_opers.o child_ctrl.o \
                         test_svr_ctrl.o

dce_acl_conv_fcns_OFILES = dce_acl_conv_fcns.o

dce_acl_perm_fcns_OFILES = acl_test_cstub.o dce_acl_perm_fcns.o child_ctrl.o \
                           test_svr_ctrl.o

PROGRAMS	= dce_acl_perm_fcns dce_acl_conv_fcns rdacl_svr_opers \
		  acl_test_server acl_setup 

DATAFILES	= add_principals.tcl

SVR_LIBS	= -ldcetst -ldce

LIBS		= ${SVR_LIBS} ${STANDARD_TETLIBS}

acl_test_server_LIBS = ${SVR_LIBS}

OPT_LEVEL	= -g

ILIST		= ${PROGRAMS} ${DATAFILES}
IDIR		= /test/tet/functional/dce/utils/acldb/ts/acl/
IMODE		= 755
#acl_test_server_IOWNER = root
#acl_test_server_IMODE = 4755

acl_test_db_ACF =
rdaclifv0.idl_ACF =

VPATH = ../../../../../../security/idl

.include <${RULES_MK}>

rdaclifv0_sstub.o : rdaclifv0_sstub.c
acl_test_db_cstub.c: acl_test_db.idl
acl_test_db.h: acl_test_db.idl
acl_test_cstub.c: acl_test.idl
acl_test_sstub.c: acl_test.idl
acl_test.h: acl_test.idl
dce_acl_conv_fcns.o : acl_test_utils.h acl_test.h acl_test_db.h
dce_acl_perm_fcns.o : acl_test_utils.h acl_test.h acl_test_db.h
test_svr_ctrl.o : acl_test_utils.h acl_test.h acl_test_db.h
child_ctrl.o : acl_test_utils.h acl_test.h acl_test_db.h
rdacl_svr_opers.o: acl_test_utils.h acl_test.h acl_test_db.h
acl_test_server.o: acl_test.h acl_test_db.h acl_test_utils.h
acl_setup.o: acl_test.h acl_test_db.h acl_test_utils.h
svrUtils.o: acl_test.h 
