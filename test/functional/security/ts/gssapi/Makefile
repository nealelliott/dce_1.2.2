# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.2  1996/03/09  20:47:36  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:26  marty]
#
# Revision 1.1.2.1  1995/12/11  19:53:59  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:27:26  root]
# 
# $EndLog$

TET_ROOT	= /test/tet/functional/security
TET_TESTS	= ../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR	= /ts/gssapi

LIBS		= -ldce -lsectsutil ${STANDARD_TETLIBS}
PROGRAMS	= test-gssapi
OTHERS		= update_tet_scen_rule
TESTS		= test-gss1.sh


# Compiler flags for this directory
CFLAGS = ${${TARGET_MACHINE}_CFLAGS} -DENVIRONMENT_TET

#
# Clearcase vs ODE note:
#	The following line is needed otherwise clearcase
#	installs the wrong version!
#

VPATH = ${MAKETOP}/test/functional/security/ts/gssapi:${MAKETOP}/test/security/api/gssapi

DATAFILES = gss-test-setup.sh \
            gss-test-cleanup.sh \
            test-gss1.sh \

OFILES      = test-gssapi.o

INCFLAGS = -I${MAKETOP}/security/h 

ILIST		= ${PROGRAMS} ${DATAFILES}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule: 
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}

