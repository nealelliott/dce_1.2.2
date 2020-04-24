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
# Revision 1.1.2.3  1996/11/14  16:17:23  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  15:54:31  arvind]
#
# Revision /main/DCE_1.2.2/5  1996/10/24  21:50 UTC  sommerfeld
# 	CHFts20477: Don't install the update_tet_scen_rule.
# 	[1996/10/24  21:23 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_4/1]
# 
# $EndLog$
# 

TET_ROOT	= /test/tet/functional/security
TET_TESTS	= ../../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ltetapi # -ltettcm
COMPONENT_DIR	= /ts/krb5/sample

LIBDIRS 	= -L${EXPORTBASE}/usr/lib/mitkrb5 -L${EXPORTBASE}/usr/lib
LIBS		= -lmitkrb5 -lsectsutil ${STANDARD_TETLIBS}
PROGRAMS	= sclient sserver
TESTS		= sclient
SCRIPTS		= sample_cleanup.ksh  sample_setup.ksh
OTHERS		= update_tet_scen_rule
DATAFILES	= ${SCRIPTS}
# DATAFILES	= sclient.M sserver.M


# Compiler flags for this directory
CFLAGS = ${${TARGET_MACHINE}_CFLAGS} -DENVIRONMENT_TET

#
# Clearcase vs ODE note:
#	The following line is needed otherwise clearcase
#	installs the wrong version!
#

VPATH = ${MAKETOP}/test/functional/security/ts/krb5/sample

sclient_OFILES	= sclient.o
sserver_OFILES	= sserver.o
sclient_LIBS	= ${LIBS} -ltettcm
sserver_LIBS	= ${LIBS} -ltettcmchild

INCFLAGS = -I${EXPORTBASE}/usr/include/mitkrb5 -I${EXPORTBASE}/usr/include/mitkrb5/krb5

ILIST		= ${PROGRAMS} ${SCRIPTS}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
