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
# Revision 1.1.2.2  1996/11/14  16:16:09  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:30:39  arvind]
#
# $EndLog$
# 

TET_ROOT	= /test/tet/functional/security
TET_TESTS	= ../../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ldce
COMPONENT_DIR	= /ts/globalgroup_suite/ggapi

LIBDIRS 	= -L${EXPORTBASE}/usr/lib/mitkrb5 -L${EXPORTBASE}/usr/lib
LIBS		= ${STANDARD_TETLIBS} ${${TARGET_MACHINE}_LIBS}
PROGRAMS	= do_ggapi
TESTS		= ggapi
SCRIPTS		= ggapi
OTHERS		= update_tet_scen_rule
DATAFILES	= ${SCRIPTS}


# Compiler flags for this directory
CFLAGS = ${${TARGET_MACHINE}_CFLAGS} -DENVIRONMENT_TET

#
# Clearcase vs ODE note:
#	The following line is needed otherwise clearcase
#	installs the wrong version!
#

VPATH = ${MAKETOP}/test/functional/security/ts/globalgroup_suite/ggapi

do_ggapi_OFILES	= do_ggapi.o
do_ggapi_LIBS	= ${LIBS}

INCFLAGS = 

ILIST		= ${PROGRAMS} ${SCRIPTS}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
