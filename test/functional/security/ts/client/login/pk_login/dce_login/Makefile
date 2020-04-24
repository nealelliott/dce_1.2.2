#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/11/14  16:13:19  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:28:41  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/10/21  18:19 UTC  bartf
# 	add dce_login tests
# 
# Revision /main/bartf_pk.3/2  1996/10/21  17:36 UTC  bartf
# 	add pk_ files
# 
# Revision /main/bartf_pk.3/1  1996/10/21  17:27 UTC  bartf
# 	dce_login tests
# 
# $EndLog$
#

TET_ROOT        = /test/tet/functional/security
DIR_TET_TESTS   = ../../../../..
TET_SCEN_FILE   = ${DIR_TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${DIR_TET_TESTS}/tools/update_tet_scen

COMPONENT_DIR   = /ts/client/login/pk_login/dce_login

SCRIPTS         = dce_login-tc_P dce_login-tc_N
OTHERS          = update_tet_scen_rule
PK_FILES        = pk_pubkeyfile pk_pvtkeyfile
ILIST           = ${SCRIPTS} ${PK_FILES} 
IDIR            = ${TET_ROOT}/${COMPONENT_DIR}/
DATAFILES       = ${PK_FILES}

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} DOM_ONLY ${TET_SCEN_FILE} ${COMPONENT_DIR} ${SCRIPTS}
