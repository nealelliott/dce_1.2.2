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
# Revision 1.1.12.1  1996/10/03  18:48:48  arvind
# 	add -DOSF_DCE since it includes some kerberos files.
# 	[1996/09/24  21:14 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
#
# Revision 1.1.10.3  1996/03/11  02:07:52  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:57  marty]
# 
# Revision 1.1.10.2  1995/12/11  19:53:27  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:27:16  root]
# 
# Revision 1.1.5.2  1994/10/27  20:07:37  rrizzo
# 	expand copyright
# 	[1994/10/27  19:39:58  rrizzo]
# 
# Revision 1.1.5.1  1994/10/25  17:17:49  sekhar
# 	[ OT 12398 ]  Checking in Sean Mullan's changes for testing
# 		      security replication.
# 	[1994/10/24  20:24:03  sekhar]
# 
# 	HP revision /main/mullan_mothra_test_repl_cov/1  1994/09/12  19:44 UTC  mullan_s
# 	Add include path for ts-common.h
# 
# Revision 1.1.2.1  1994/08/04  16:16:02  mdf
# 	Initial checkin
# 	[1994/08/03  19:19:53  mdf]
# 
# $EndLog$
# 

TET_ROOT	= /test/tet/functional/security
DIR_TET_TESTS	= ../../../..
TET_SCEN_FILE	= ${DIR_TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${DIR_TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR	= /ts/client/rca/sec_rgy_attr_sch

LIBS		= -ldce -lsectsutil ${STANDARD_TETLIBS} 
PROGRAMS	= sec_rgy_attr_sch-tc 
OTHERS		= update_tet_scen_rule
TESTS		= ${PROGRAMS}


OFILES= sec_rgy_attr_sch.o

CFLAGS=-DOSF_DCE

INCFLAGS	= -I${OBJECTDIR}/security/utils \
		  -I${MAKETOP}security/h \
                  -I${MAKETOP}security/idl \
		  -I${MAKETOP}security/client/rca \
		  -I${MAKETOP}test/functional/security/common

ILIST		= ${PROGRAMS}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule: 
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
