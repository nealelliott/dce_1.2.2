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
# Revision 1.1.6.1  1996/08/09  12:16:18  arvind
# 	update for international scenario
# 	[1995/11/07  15:40 UTC  bartf  /main/bartf_sec_kinit.10/1]
#
# Revision 1.1.4.2  1996/03/11  02:07:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:51  marty]
# 
# Revision 1.1.4.1  1995/12/11  19:53:05  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:27:04  root]
# 
# Revision 1.1.2.4  1994/10/06  20:35:06  agd
# 	expand copyright
# 	[1994/10/06  14:29:48  agd]
# 
# Revision 1.1.2.3  1994/09/20  22:34:12  max
# 	CR 11702 fix.  Fix Makefile to work on machines providing their
# 	own pthreads.  This is done by adding machine specific libraries
# 	to the link stream.
# 	[1994/09/20  22:33:51  max]
# 
# Revision 1.1.2.2  1994/09/07  15:48:42  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:38  marty]
# 
# Revision 1.1.2.1  1994/08/04  16:15:45  mdf
# 	Initial checkin
# 	[1994/08/03  19:20:47  mdf]
# 
# $EndLog$
# 

VPATH = ${MAKETOP}/security/idl
IDLINCFLAGS	= -I. -I${MAKETOP}/security/idl
IDLFLAGS	= -keep c_source 
IDLFILES	= rsec_pwd_mgmt.idl

rsec_pwd_mgmt_ACF =

TET_ROOT	= /test/tet/functional/security
TET_TESTS	= ../../../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen

STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR	= /ts/client/pwd_mgmt/pwd_strength

LIBS		= -ldce -lsectsutil ${STANDARD_TETLIBS} \
		  ${${TARGET_MACHINE}_LIBS}
PROGRAMS	= pwd_strength-tc
OTHERS		= update_tet_scen_rule
TESTS		= ${PROGRAMS}
STUBS		= rsec_pwd_mgmt_cstub.o


OFILES= ${STUBS} pwd_strength-tc.o

INCFLAGS	= -I${MAKETOP}security/h

ILIST		= ${PROGRAMS}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} DOM_ONLY ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
