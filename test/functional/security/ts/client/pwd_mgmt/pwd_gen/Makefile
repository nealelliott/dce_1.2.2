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
# Revision 1.1.6.1  1996/08/09  12:16:03  arvind
# 	update for international scenario
# 	[1995/11/07  15:40 UTC  bartf  /main/bartf_sec_kinit.10/1]
#
# Revision 1.1.4.2  1996/03/11  02:07:29  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:49  marty]
# 
# Revision 1.1.4.1  1995/12/11  19:52:55  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:27:01  root]
# 
# Revision 1.1.2.2  1994/10/06  20:34:26  agd
# 	expand copyright
# 	[1994/10/06  14:29:36  agd]
# 
# Revision 1.1.2.1  1994/08/04  16:15:36  mdf
# 	Initial checkin
# 	[1994/08/03  19:21:07  mdf]
# 
# $EndLog$
# 

TET_ROOT        = /test/tet/functional/security
TET_TESTS       = ../../../..
TET_SCEN_FILE   = ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen
STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR   = /ts/client/pwd_mgmt/pwd_gen
LIBS            = -ldce -lsectsutil ${STANDARD_TETLIBS}
PROGRAMS        = pwd_gen-tc
OTHERS          = update_tet_scen_rule setup.ksh
TESTS           = ${PROGRAMS}
OFILES		= pwd_gen-tc.o
INCFLAGS        = -I${MAKETOP}security/h
ILIST           = ${PROGRAMS} setup.ksh
IDIR            = ${TET_ROOT}/${COMPONENT_DIR}/
IMODE           = 755

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} DOM_ONLY ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
