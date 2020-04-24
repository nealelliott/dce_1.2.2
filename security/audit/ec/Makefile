# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.2  1996/03/09  23:27:02  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:33  marty]
#
# Revision 1.1.5.1  1995/12/08  17:38:40  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:17:39  root]
# 
# Revision 1.1.2.4  1994/09/19  21:46:58  weisz
# 	CR 12223: make client permissions checking consistent.
# 	[1994/09/19  21:45:25  weisz]
# 
# Revision 1.1.2.3  1994/06/17  18:41:17  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:08:02  devsrc]
# 
# Revision 1.1.2.2  1994/04/01  21:52:16  kline_s
# 	[OT 10264] First audit code point drop for security.
# 	Add SEC event class files.
# 	[1994/04/01  21:42:00  kline_s]
# 
# Revision 1.1.2.1  1994/03/15  22:43:11  zeliff
# 	OT10142: add DTS and AUD event class files
# 	[1994/03/15  22:42:47  zeliff]
# 
# $EndLog$

AUDIT_FILES = dce_audit_filter_modify dce_audit_filter_query \
              dce_audit_admin_modify dce_audit_admin_query

DTS_FILES = dce_dts_mgt_query dce_dts_mgt_modify dce_dts_synch \
		dce_dts_time_provider

SEC_FILES = dce_sec_authent dce_sec_control dce_sec_modify dce_sec_query \
		dce_sec_server

ILIST = ${AUDIT_FILES} ${DTS_FILES} ${SEC_FILES}
IDIR = /etc/audit/ec/

.include <${RULES_MK}>
