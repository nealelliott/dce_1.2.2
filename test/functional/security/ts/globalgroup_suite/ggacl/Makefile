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
# Revision 1.1.2.2  1996/11/14  16:15:50  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:30:31  arvind]
#
# $EndLog$
# 

TET_ROOT	= /test/tet/functional/security
TET_TESTS	= ../../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

COMPONENT_DIR	= /ts/globalgroup_suite/ggacl

PROGRAMS	=
TESTS		= ggacl
SCRIPTS		= ggacl
OTHERS		= update_tet_scen_rule
DATAFILES	= ${SCRIPTS}

ILIST		= ${PROGRAMS} ${SCRIPTS}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/
IMODE		= 755

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
