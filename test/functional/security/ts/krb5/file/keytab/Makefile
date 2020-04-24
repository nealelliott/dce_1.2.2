# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/08/09  20:17:56  arvind
# 	Merge to DCE 1.2.2
# 	[1996/07/26  21:19 UTC  mullan_s  /main/DCE_1.2.2/1]
#
# 	Incorrect filenames for keytab files.
# 	[1996/07/10  15:29 UTC  mullan_s  /main/mullan_dce122_k5tests_1/1]
#
# 	build rules
# 	[1996/07/10  15:29 UTC  mullan_s  /main/mullan_dce122_k5tests_1/1]
#
# $EndLog$

TET_ROOT        = /test/tet/functional/security
DIR_TET_TESTS   = ../../../..
TET_SCEN_FILE   = ${DIR_TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${DIR_TET_TESTS}/tools/update_tet_scen

COMPONENT_DIR   = /ts/krb5/file/keytab

SCRIPTS         = krb5keytab-tc
KEYTAB_FILES	= krb5ktfilename.1 krb5ktfilename.2
OTHERS          = update_tet_scen_rule
ILIST           = ${SCRIPTS} ${KEYTAB_FILES}
IDIR            = ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} krb5keytab-tc
