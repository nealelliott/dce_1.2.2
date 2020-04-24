# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.2  1996/03/11  02:07:03  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:35  marty]
#
# Revision 1.1.2.1  1995/12/11  19:52:27  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:49  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:18:12  root]
# 
# Revision 1.1.2.2  1994/10/06  20:32:21  agd
# 	expand copyright
# 	[1994/10/06  14:28:52  agd]
# 
# Revision 1.1.2.1  1994/08/04  16:15:20  mdf
# 	Initial checkin
# 	[1994/08/03  19:20:03  mdf]
# 
# $EndLog$
# 

TET_ROOT	= /test/tet/functional/security
TET_TESTS       = ../../../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR	= /ts/client/login/deleg

VPATH		= ${MAKETOP}/functional/test/security/tet-tests/common
LIBS		= -ldce -lsectsutil ${STANDARD_TETLIBS}
PROGRAMS	= sec_login_deleg-tc
OTHERS		= update_tet_scen_rule
TESTS		= ${PROGRAMS}


OFILES= sec_login_deleg.o

INCFLAGS	= -I${OBJECTDIR}/security/utils \
		  -I${MAKETOP}security/h \
		  -I${MAKETOP}security/client/login \
		  -I${MAKETOP}test/functional/security/common


ILIST		= ${PROGRAMS}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
