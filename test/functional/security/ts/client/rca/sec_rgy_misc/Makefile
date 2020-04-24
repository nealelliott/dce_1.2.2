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
# Revision 1.1.2.2  1996/03/11  02:07:55  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:58  marty]
#
# Revision 1.1.2.1  1995/12/11  19:53:32  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/27  17:52 UTC  bartf
# 	add sec_rgy_misc test
# 
# 	HP revision /main/bartf_sec_90/2  1995/06/27  17:41 UTC  bartf
# 	add passwd_override
# 
# 	HP revision /main/bartf_sec_90/1  1995/06/15  16:51 UTC  bartf
# 	initial test for sec_rgy_pgo_get_by_eff_unix_num with no passwd_override
# 	[1995/12/11  19:27:19  root]
# 
# $EndLog$
# 
TET_ROOT	= /test/tet/functional/security
TET_TESTS	= ../../../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR	= /ts/client/rca/sec_rgy_misc

LIBS		= -ldce -lsectsutil ${STANDARD_TETLIBS}
PROGRAMS	= sec_rgy_misc-tc 
OTHERS		= update_tet_scen_rule
TESTS		= ${PROGRAMS}
DATAFILES	= passwd_override


OFILES		= sec_rgy_misc.o 

INCFLAGS	= -I../../../../../../../security/utils \
		  -I${MAKETOP}security/h \
		  -I${MAKETOP}security/idl \
		  -I${MAKETOP}security/client/rca \
                  -I${MAKETOP}test/functional/security/common

ILIST		= ${PROGRAMS} ${DATAFILES}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule: 
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
