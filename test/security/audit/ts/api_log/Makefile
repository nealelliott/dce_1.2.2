# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# (c) Copyright International Business Machines Corp. 1994, 1996
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/07/09  15:08:07  arvind
# 	Audit for 1.2.2
# 	[1996/06/21  20:28 UTC  arvind  /main/arvind_audit122/1]
#
# 	Audit for 1.2.2
# 	[1996/03/11  02:35:58  marty  1.1.4.2]
#
# Revision 1.1.4.2  1996/03/11  02:35:58  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:15  marty]
# 
# Revision 1.1.4.1  1995/12/11  21:51:24  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/4  1995/08/18  20:10 UTC  kline_s
# 	Merge fix for CHFts15645.
# 
# 	HP revision /main/HPDCE02/kline_s_mothra/3  1995/08/17  21:30 UTC  kline_s
# 	CHFts16115: Remove DES_HIDDEN conditional - only using one protection
# 	level due to singular Domestic/Export release.
# 
# 	HP revision /main/HPDCE02/kline_s_mothra/2  1995/07/06  19:45 UTC  kline_s
# 	Enable tests to function properly using international (ie. non-DES) bits.
# 
# 	HP revision /main/HPDCE02/kline_s_mothra/1  1995/06/27  02:04 UTC  kline_s
# 	Added aud_test.h dependency for aud_test_rpc_manager.o.
# 	Removed ignored -g option from list of CCFLAGS options.
# 
# 	HP revision /main/HPDCE02/1  1995/03/01  22:29 UTC  mob
# 	Change dependency from .idl to .h
# 	[1995/12/11  20:53:38  root]
# 
# Revision 1.1.2.3  1994/06/10  20:45:47  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:17:11  devsrc]
# 
# Revision 1.1.2.2  1994/04/21  16:49:38  zeliff
# 	ot10419: removed bad path from INCFLAGS
# 	[1994/04/21  16:47:42  zeliff]
# 
# Revision 1.1.2.1  1994/04/20  13:27:50  weisz
# 	initial submission
# 	[1994/04/20  13:25:51  weisz]
# 
# $EndLog$
# Directory in which to install the files
IDIR = /test/tet/functional/security/audit/ts/api_log/

# Files to install
ILIST = ${PROGRAMS} ${SCRIPTS} ${OTHERS}
IMODE = 755

# Programs to build
aud_test.idl_IDLFLAGS = -keep c_source -cepv
PROGRAMS = aud_test
SCRIPTS  = api_log
OTHERS   =

CFLAGS = ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../../../../security/audit/h -I../../../../../security/audit/idl

IDLFILES = aud_test.idl

aud_test_OFILES = aud_test.o aud_test_sstub.o aud_test_cstub.o aud_test_rpc_manager.o

aud_test_sstub.o: aud_test_sstub.c

aud_test_cstub.o: aud_test_cstub.c

aud_test.o: aud_test.h

aud_test_rpc_manager.o: aud_test.h

aud_test_LIBS = -ltettcm -ltetapi -laudit -ldce 

.include <${RULES_MK}>
