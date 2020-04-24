# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/08/09  20:18:20  arvind
# 	Merge to DCE 1.2.2
# 	[1996/07/10  14:51 UTC  mullan_s  /main/mullan_dce122_k5tests_1/1]
#
# 	build rules
# 	[1996/07/10  14:51 UTC  mullan_s  /main/mullan_dce122_k5tests_1/1]
#
# $EndLog$

TET_ROOT        = /test/tet/functional/security
DIR_TET_TESTS   = ../../..
TET_SCEN_FILE   = ${DIR_TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${DIR_TET_TESTS}/tools/update_tet_scen

COMPONENT_DIR   = /ts/krb5/kinit

#
# This test depends on the MIT krb5 kinit and klist programs
# to help verify that krb5 applications are interoperable with
# a DCE KDC. So, we build kinit & klist from the mit_krb5
# directory and install them to the test directory.

INCFLAGS 	= -I${EXPORTBASE}/usr/include/mitkrb5 -I${EXPORTBASE}/usr/include/mitkrb5/krb5
LIBDIRS 	= -L${EXPORTBASE}/usr/lib/mitkrb5
LIBS     	= -lmitkrb5
VPATH		= ${MAKETOP}/mit_krb5/clients/kinit:${MAKETOP}/mit_krb5/clients/klist
SCRIPTS         = krb5kinit-tc
PROGRAMS	= kinit klist
OTHERS          = update_tet_scen_rule
ILIST           = ${SCRIPTS} ${PROGRAMS}
IDIR            = ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} krb5kinit-tc
