# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/06/04  21:51:28  arvind
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:33 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
#
# 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
# 	[1996/05/03  20:00 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1]
#
# 	Add RCS header & OSF copyright.
# 	[1996/05/03  20:00 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1]
#
# $EndLog$
#
# Copyright (c) Hewlett-Packard Company 1996
# Unpublished work. All Rights Reserved.
#
# DCE mit_krb5/util/et Makefile

# Files to export and their locations
INCLUDES = com_err.h et_h et_c
EXPDIR   = /usr/include/mitkrb5/

.include <${RULES_MK}>

et_h: et_h.awk
	${RM} ${_RMFLAGS_} et_h
	${CP} ${et_h.awk:P} et_h

et_c: et_c.awk
	${RM} ${_RMFLAGS_} et_c
	${CP} ${et_c.awk:P} et_c
