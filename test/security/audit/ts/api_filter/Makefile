# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# (c) Copyright International Business Machines Corp. 1994, 1996
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/07/09  15:03:47  arvind
# 	Audit for 1.2.2
# 	[1996/06/21  20:27 UTC  arvind  /main/arvind_audit122/1]
#
# 	Audit for 1.2.2
# 	[1996/03/11  02:35:53  marty  1.1.4.2]
#
# Revision 1.1.4.2  1996/03/11  02:35:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:13  marty]
# 
# Revision 1.1.4.1  1995/12/11  21:50:52  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/4  1995/08/18  20:10 UTC  kline_s
# 	Merge fix for CHFts16115
# 
# 	HP revision /main/HPDCE02/kline_s_mothra/3  1995/08/17  21:31 UTC  kline_s
# 	CHFts16115: Remove DES_HIDDEN conditional - only using one protection
# 	level due to singular Domestic/Export release.
# 
# 	HP revision /main/HPDCE02/kline_s_mothra/2  1995/07/06  19:45 UTC  kline_s
# 	Enable tests to function properly using international (ie. non-DES) bits.
# 
# 	HP revision /main/HPDCE02/2  1995/06/27  21:20 UTC  kline_s
# 	Merge in revised audit tests.
# 
# 	HP revision /main/HPDCE02/kline_s_mothra/1  1995/06/27  01:49 UTC  kline_s
# 	Added aud_testf.h dependency for aud_testf_rpc_manager.o
# 	Removed ignored -g option from CCFLAGS.
# 
# 	HP revision /main/HPDCE02/1  1995/03/01  22:30 UTC  mob
# 	Change dependency from .idl to .h
# 	[1995/12/11  20:53:25  root]
# 
# Revision 1.1.2.2  1994/06/10  20:45:26  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:17:00  devsrc]
# 
# Revision 1.1.2.1  1994/04/28  18:32:55  weisz
# 	CR 10402: initial submission
# 	[1994/04/28  17:07:17  weisz]
# 
# $EndLog$
# Directory in which to install the files
IDIR = /test/tet/functional/security/audit/ts/api_filter/

# Files to install
ILIST = ${PROGRAMS} ${SCRIPTS}
IMODE = 755

# Programs to build
aud_testf.idl_IDLFLAGS = -keep c_source -cepv
PROGRAMS = aud_testf
SCRIPTS  = api_filter \
           addfilter.group1 addfilter.princ1 addfilter.cell delfilter.princ1 \
           delfilter.group1 delfilter.cell startauditd stopauditd \
           addfilter.world delfilter.world addfilter.worldover \
           delfilter.worldover addfilter.princ1noac addfilter.group1noac \
           addfilter.princ1succalarm addfilter.princ1denialarm

CFLAGS = ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../../../../security/audit/h -I../../../../../security/audit/idl

IDLFILES = aud_testf.idl

aud_testf_OFILES = aud_testf.o aud_testf_sstub.o aud_testf_cstub.o aud_testf_rpc_manager.o

aud_testf_sstub.o: aud_testf_sstub.c

aud_testf_cstub.o: aud_testf_cstub.c

aud_testf.o: aud_testf.h

aud_testf_rpc_manager.o: aud_testf.h

LIBS = -laudit -ldce  -ltetapi -ltettcm
.include <${RULES_MK}>
