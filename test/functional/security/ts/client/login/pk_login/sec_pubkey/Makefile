#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.2  1996/10/03  18:45:07  arvind
# 	Add OSF copyright
# 	[1996/09/30  15:44 UTC  arvind  /main/DCE_1.2.2/2]
#
# Revision 1.1.2.1  1996/08/09  20:14:25  arvind
# 	Merge from DCE_1.2
# 	[1996/08/09  19:59:55  arvind]
# 
# $EndLog$
#
TET_ROOT	= /test/tet/functional/security
TET_TESTS       = ../../../../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR	= /ts/client/login/pk_login/sec_pubkey

VPATH		= ${MAKETOP}/functional/test/security/tet-tests/common
LIBS		= -ldce -lsectsutil ${STANDARD_TETLIBS}
PROGRAMS	= sec_pubkey_P sec_pubkey_N
OTHERS		= update_tet_scen_rule setup.ksh cleanup.ksh
TESTS		= ${PROGRAMS}

#OFILES= sec_pubkey_P.o sserver.o
sec_pubkey_P_OFILES      = sec_pubkey_P.o
sec_pubkey_N_OFILES      = sec_pubkey_N.o


INCFLAGS	= -I${OBJECTDIR}/security/utils \
		  -I${MAKETOP}security/h \
		  -I${MAKETOP}security/client/login \
		  -I${MAKETOP}test/functional/security/common


ILIST		= ${PROGRAMS} setup.ksh cleanup.ksh
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}



