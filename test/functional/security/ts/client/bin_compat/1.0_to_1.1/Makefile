#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.2  1996/10/03  18:41:21  arvind
# 	Add OSF copyright
# 	[1996/09/30  15:43 UTC  arvind  /main/DCE_1.2.2/2]
#
# Revision 1.1.2.1  1996/06/05  14:18:11  arvind
# 	Merge from DCE_1.2
# 	[1996/05/24  23:34 UTC  sommerfeld  /main/DCE_1.2.2/1]
# 
# $EndLog$
#
TET_ROOT        = /test/tet/functional/security
TET_TESTS       = ../../../..
TET_SCEN_FILE   = ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR   = /ts/client/bin_compat/1.0_to_1.1

VPATH           = ${MAKETOP}/functional/test/security/tet-tests/common
LIBS            = -ldce -lsectsutil ${STANDARD_TETLIBS}
PROGRAMS        = bin_compat-tc2
OTHERS          = update_tet_scen_rule bin_compat-tc2_child
TESTS           = ${PROGRAMS}

OFILES= bin_compat-tc2.o

INCFLAGS        = -I${MAKETOP}/test/functional/security/common

ILIST           = ${PROGRAMS} bin_compat-tc2_child
IDIR            = ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
