# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/07/09  15:13:33  arvind
# 	Audit for 1.2.2
# 	[1996/03/11  02:36:14  marty  1.1.4.2]
#
# Revision 1.1.4.2  1996/03/11  02:36:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:22  marty]
# 
# Revision 1.1.4.1  1995/12/11  21:51:55  root
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
# 	HP revision /main/HPDCE02/kline_s_mothra/1  1995/06/27  02:16 UTC  kline_s
# 	Add aud_testec_rpc_manager.o build dependency on aud_testec.h.
# 
# 	HP revision /main/HPDCE02/1  1995/03/01  22:29 UTC  mob
# 	Change dependency from .idl to .h
# 	[1995/12/11  20:53:55  root]
# 
# Revision 1.1.2.3  1994/06/10  20:46:21  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:17:32  devsrc]
# 
# Revision 1.1.2.2  1994/04/21  16:49:42  zeliff
# 	ot10419: removed bad path from INCFLAGS
# 	[1994/04/21  16:47:52  zeliff]
# 
# Revision 1.1.2.1  1994/04/20  18:09:11  weisz
# 	initial submission
# 	[1994/04/20  18:04:13  weisz]
# 
# $EndLog$
# Directory in which to install the files
IDIR = /test/tet/functional/security/audit/ts/ecfile_filter/

# Files to install
ILIST = ${PROGRAMS} ${SCRIPTS}
IMODE = 755

# Programs to build

aud_testec.idl_IDLFLAGS = -keep c_source -cepv

PROGRAMS = aud_testec

SCRIPTS  = ecfile_filter addfilter.world addevent.toec delfilter.world \
           restore.ec startauditd stopauditd

INCFLAGS = -I../../../../../security/audit/h -I../../../../../security/audit/idl 

IDLFILES = aud_testec.idl

aud_testec_OFILES = aud_testec.o aud_testec_sstub.o aud_testec_cstub.o aud_testec_rpc_manager.o

aud_testec_sstub.o: aud_testec_sstub.c

aud_testec_cstub.o: aud_testec_cstub.c

aud_testec.o: aud_testec.h

aud_testec_rpc_manager.o: aud_testec.h

LIBS = -laudit -ldce  -ltetapi -ltettcm 
.include <${RULES_MK}>
