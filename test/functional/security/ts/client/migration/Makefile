#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1995, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.3  1996/03/11  02:07:21  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:41  marty]
#
# Revision 1.1.4.2  1995/12/11  19:52:45  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:57  root]
# 
# Revision 1.1.4.1  1995/07/11  23:25:30  marty
# 	      HP warrenty patch
# 	      [1995/07/11  23:23:42  marty]
# 
# $EndLog$
#
# Copyright (c) Hewlett-Packard Company 1995
# Unpublished work. All Rights Reserved.
#
#

TET_ROOT        = /test/tet/functional/security
TET_TESTS       = ../../..
TET_SCEN_FILE   = ${TET_TESTS}/tet_scen
#UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen
STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR   = /ts/client/migration
LIBS            = -ldce -lsectsutil ${STANDARD_TETLIBS}
PROGRAMS        = migration-tc
#OTHERS          = update_tet_scen_rule
TESTS           = ${PROGRAMS}
OFILES          = migration-tc.o
INCFLAGS        = -I${MAKETOP}/test/functional/security/common
ILIST           = ${PROGRAMS}
IDIR            = ${TET_ROOT}/${COMPONENT_DIR}/
IMODE           = 755

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
