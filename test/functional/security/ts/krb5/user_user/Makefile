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
# Revision 1.1.2.2  1996/07/09  15:02:36  arvind
# 	Put scripts in obj dir.
# 	[1996/06/11  19:23 UTC  psn  /main/DCE_1.2.2/psn_krb5/2]
#
# 	Put scripts in object dir.
# 	[1996/06/11  19:18 UTC  psn  /main/DCE_1.2.2/psn_krb5/1]
#
# 	Put scripts in object dir.
# 	[1996/06/03  14:59 UTC  psn  /main/DCE_1.2.2/2]
#
# 	Add TESTS directive.
# 	[1996/05/30  20:49 UTC  psn  /main/DCE_1.2.2/1]
#
# 	Merge to DCE 1.2.2.
# 	[1996/05/08  19:00 UTC  psn  /main/psn_k5_test/2]
#
# 	Add '-DDEBUG' as it shows up in MIT version of Makefile.
# 	[1996/04/16  23:59 UTC  psn  /main/psn_k5_test/1]
#
# 	TETize KRB5 tests.
# 	[1996/04/16  23:59 UTC  psn  /main/psn_k5_test/1]
#
# Revision 1.1.2.1  1996/06/04  22:10:30  arvind
# 	Add TESTS directive.
# 	[1996/05/30  20:49 UTC  psn  /main/DCE_1.2.2/1]
# 
# 	Merge to DCE 1.2.2.
# 	[1996/05/08  19:00 UTC  psn  /main/psn_k5_test/2]
# 
# 	Add '-DDEBUG' as it shows up in MIT version of Makefile.
# 	[1996/04/16  23:59 UTC  psn  /main/psn_k5_test/1]
# 
# 	TETize KRB5 tests.
# 	[1996/04/16  23:59 UTC  psn  /main/psn_k5_test/1]
# 
# $EndLog$
# 

TET_ROOT	= /test/tet/functional/security
TET_TESTS	= ../../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ltetapi
COMPONENT_DIR	= /ts/krb5/user_user

LIBDIRS 	= -L${EXPORTBASE}/usr/lib/mitkrb5 -L${EXPORTBASE}/usr/lib
LIBS		= -lmitkrb5 -lsectsutil ${STANDARD_TETLIBS}
PROGRAMS	= uu-client uu-server
TESTS		= uu-client
SCRIPTS		= uu_setup.ksh uu_cleanup.ksh
DATAFILES	= ${SCRIPTS}
OTHERS		= update_tet_scen_rule

# Compiler flags for this directory
CFLAGS = -DDEBUG ${${TARGET_MACHINE}_CFLAGS} -DENVIRONMENT_TET

#
# Clearcase vs ODE note:
#	The following line is needed otherwise clearcase
#	installs the wrong version!
#

VPATH = ${MAKETOP}/test/functional/security/ts/krb5/user_user

uu-client_OFILES	= client.o
uu-server_OFILES	= server.o
uu-client_LIBS	= ${LIBS} -ltettcm
uu-server_LIBS	= ${LIBS} -ltettcmchild

INCFLAGS = -I${EXPORTBASE}/usr/include/mitkrb5 -I${EXPORTBASE}/usr/include/mitkrb5/krb5

ILIST		= ${PROGRAMS} ${SCRIPTS}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
