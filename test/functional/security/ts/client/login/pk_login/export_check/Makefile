#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/10/03  20:36:00  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  19:43:53  arvind]
#
# 	OSF DCE 1.2.2 Drop 4
#
# Revision /main/DCE_1.2.2/2  1996/09/30  15:44 UTC  arvind
# 	Add OSF copyright
# 
# $EndLog$
#
TET_ROOT        = /test/tet/functional/security
DIR_TET_TESTS   = ../../../../..
TET_SCEN_FILE   = ${DIR_TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${DIR_TET_TESTS}/tools/update_tet_scen

COMPONENT_DIR   = /ts/client/login/pk_login/export_check

SCRIPTS         = export_check-tc
OTHERS          = update_tet_scen_rule
ILIST           = ${SCRIPTS}
IDIR            = ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} INT_ONLY ${TET_SCEN_FILE} ${COMPONENT_DIR} export_check-tc
