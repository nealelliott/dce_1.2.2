# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/06/04  21:51:40  arvind
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:33 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
#
# 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
# 	[1996/05/03  20:01 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1]
#
# 	Add RCS header & OSF copyright.
# 	[1996/05/03  20:01 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1]
#
# $EndLog$
#
# Copyright (c) Hewlett-Packard Company 1996
# Unpublished work. All Rights Reserved.
#
# DCE mit_krb5/util/profile Makefile

# Header files, source files to build
INCLUDES = prof_err.h profile.h prof_err.c

# Export location to put include files
EXPDIR                  = /usr/include/mitkrb5/

.include <${RULES_MK}>

prof_err.h: prof_err.et
	${AWK} -f ./../../util/et/et_h outfile=prof_err.h ${prof_err.et:P}

profile.h: profile.hin prof_err.h
	cat ${profile.hin:P} prof_err.h > ./profile.h

prof_err.c: prof_err.et
	${AWK} -f ./../../util/et/et_c outfile=prof_err.c ${prof_err.et:P}
