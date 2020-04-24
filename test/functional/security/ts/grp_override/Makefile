# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.2  1996/03/09  20:47:34  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:24  marty]
#
# Revision 1.1.2.1  1995/12/11  19:53:41  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:27:21  root]
# 
# $EndLog$

#
# Makefile for group override tests
#

TET_ROOT	= /test/tet/functional/security
TET_TESTS	= ../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR	= /ts/group_override

LIBS		=  -ldce ${STANDARD_TETLIBS}

CFLAGS		= -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

PROGRAMS	= grp_override passwd_export_grp_override
DATAFILES	= grp_override.sh group_override passwd_override
OTHERS		= update_tet_scen_rule
TESTS		= grp_override.sh

ILIST		= ${PROGRAMS} ${DATAFILES}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/


.include <${RULES_MK}>

update_tet_scen_rule: 
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}

