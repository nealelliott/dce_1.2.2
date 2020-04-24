#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.3  1996/11/14  16:13:05  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:28:32  arvind]
#
# Revision /main/DCE_1.2.2/5  1996/10/21  18:19 UTC  bartf
# 	add dce_login tests
# 
# Revision /main/DCE_1.2.2/bartf_pk.3/1  1996/10/18  17:57 UTC  bartf
# 	add dce_login
# 
# Revision /main/DCE_1.2.2/4  1996/09/30  15:44 UTC  arvind
# 	Add OSF copyright
# 
# $EndLog$
#
.if defined (USE_DES)
SUBDIRS = sec_pubkey sec_pvtkey sec_psm rsec_pk sec_rgy export_check dce_login
.else
SUBDIRS = sec_pubkey sec_psm rsec_pk sec_rgy export_check
.endif

.include <${RULES_MK}>
