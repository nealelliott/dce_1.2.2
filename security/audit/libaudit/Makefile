# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# (c) Copyright International Business Machines Corp. 1994, 1996
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/07/08  18:30:37  arvind
# 	Audit for 1.2.2
# 	[1996/06/21  20:26 UTC  arvind  /main/arvind_audit122/1]
#
# 	Audit for 1.2.2
# 	[1996/03/09  23:27:16  marty  1.1.4.2]
#
# Revision 1.1.4.2  1996/03/09  23:27:16  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:46  marty]
# 
# Revision 1.1.4.1  1995/12/08  17:41:11  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/08/18  19:53 UTC  kline_s
# 	Merge fix for CHFts16115
# 
# 	HP revision /main/HPDCE02/2  1995/08/18  19:52 UTC  kline_s
# 
# 	HP revision /main/kline_s_mothra/3  1995/08/18  17:19 UTC  kline_s
# 	Merge fix for CHFts16115.
# 
# 	HP revision /main/kline_s_mothra/2  1995/08/17  21:28 UTC  kline_s
# 	CHFts16115: Remove DES_HIDDEN conditional - only using one protection
# 	level due to singular Domestic/Export release.
# 
# 	HP revision /main/kline_s_mothra/1  1995/07/06  19:51 UTC  kline_s
# 	Distinguish between domestic and international (ie. no encryption) builds.
# 	[1995/12/08  17:18:05  root]
# 
# Revision 1.1.2.6  1994/09/26  20:24:29  blazin
# 	Merged with changes from 1.1.2.5
# 	[1994/09/26  20:24:23  blazin]
# 
# 	For CR 12300, added support to copy libaudit.a into the install area.
# 	[1994/09/26  20:23:02  blazin]
# 
# Revision 1.1.2.5  1994/09/23  21:26:13  weisz
# 	CR 11123: remove hardcoded -g from CFLAGS.
# 	[1994/09/23  21:25:44  weisz]
# 
# Revision 1.1.2.4  1994/06/29  12:16:48  zeliff
# 	ot10155: change CFLAGS assignment operator
# 	[1994/06/29  12:11:44  zeliff]
# 
# Revision 1.1.2.3  1994/03/15  15:48:31  zeliff
# 	OT10048: remove libaudit.a from ILIST
# 	[1994/03/15  15:48:21  zeliff]
# 
# Revision 1.1.2.2  1994/03/11  22:48:33  luan
# 	CR 9775 9776: Moved sams stuff to build in libaudit.
# 	[1994/03/11  22:08:13  luan]
# 
# Revision 1.1.2.1  1993/12/22  12:39:21  zeliff
# 	First version of code
# 	[1993/12/21  15:01:34  zeliff]
# 
# $EndLog$
#

VPATH = ../idl

LIBRARIES = 		libaudit.a
libaudit.a_EXPDIR =	/usr/lib/

# SAMS stuff
SAMSFILES = aud.sams
SAMIDL_INCLUDES = dceaudmsg.h dceaudmac.h dceaudsvc.h
SAMS_TARGETS = dceaudmsg.h dceaudmsg.c dceaudsvc.c dceaudmac.h \
        dceaud.cat
INCLUDES = dceaudmsg.h dceaudmac.h dceaudsvc.h
EXPDIR = /usr/include/dce/

audit_pickle.idl_IDLFLAGS = -server none -keep c_source
audit_update.idl_IDLFLAGS = -client none -keep c_source
audit_log.idl_IDLFLAGS = -server none -keep c_source -cepv 

IDLFILES = audit_update.idl audit_log.idl audit_pickle.idl
IDLOBJ = audit_update_sstub.o audit_pickle_cstub.o audit_log_cstub.o

EVTFILES = evt_classes.o evt_write.o evt_read.o evt_filter.o evt_utils.o 
OBJFILES = ${EVTFILES} \
	   esl_in_core.o esl_file_read.o esl_sync.o \
	   trl_ops.o \
	   svc_utils.o 

CFLAGS		+= ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../h -I../auditd -I../../h

libaudit.a_OFILES = ${IDLOBJ} ${OBJFILES} dceaudmsg.o dceaudsvc.o

${EVTFILES}: audit_log.h
${OBJFILES}: dceaudmsg.h dceaudmac.h dceaudsvc.c dceaudmsg.c

ILIST = dceaud.cat dceaudmsg.h libaudit.a
dceaud.cat_IDIR = /nls/msg/en_US.ASCII/
dceaudmsg.h_IDIR = /share/include/dce/
libaudit.a_IDIR = /usr/lib/

.include <${RULES_MK}>
audit_update_sstub.o: audit_update.idl
audit_pickle_cstub.o: audit_pickle.idl
audit_log_cstub.o: audit_log.idl

