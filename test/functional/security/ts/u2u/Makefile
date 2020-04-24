# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/06/04  22:11:09  arvind
# 	Merge from mb_u2u
# 	[1996/05/09  19:34 UTC  burati  /main/DCE_1.2/1]
#
# 	Makefile for u2u test
# 	[1996/05/09  19:26 UTC  burati  /main/mb_u2u/1]
#
# $EndLog$
# 

TET_ROOT	= /test/tet/functional/security
TET_TESTS       = ../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR	= /ts/u2u

VPATH		= ${MAKETOP}/functional/test/security/tet-tests/common
LIBS		= -ldce -lsectsutil ${STANDARD_TETLIBS}
PROGRAMS	= sec_u2u-tc
OTHERS		= update_tet_scen_rule
TESTS		= ${PROGRAMS}


OFILES= sec_u2u.o

INCFLAGS	= -I${OBJECTDIR}/security/utils \
		  -I${MAKETOP}security/h \
		  -I${MAKETOP}security/client/login \
		  -I${MAKETOP}test/functional/security/common


ILIST		= ${PROGRAMS}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
