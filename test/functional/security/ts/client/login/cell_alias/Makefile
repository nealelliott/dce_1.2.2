#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.3  1996/03/09  20:47:30  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:21  marty]
#
# Revision 1.1.4.2  1995/12/11  19:52:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:47  root]
# 
# Revision 1.1.4.1  1995/07/11  23:25:27  marty
# 	HP warrenty patch
# 	[1995/07/11  23:18:38  marty]
# 
# $EndLog$

TET_ROOT        = /test/tet/functional/security
DIR_TET_TESTS   = ../../../..
TET_SCEN_FILE   = ${DIR_TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${DIR_TET_TESTS}/tools/update_tet_scen

COMPONENT_DIR   = /ts/client/login/cell_alias

SCRIPTS         = cell_alias-tc
OTHERS          = update_tet_scen_rule
ILIST           = ${SCRIPTS}
IDIR            = ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} cell_alias-tc
