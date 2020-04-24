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
# Revision 1.1.4.2  1996/03/11  02:07:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:36  marty]
#
# Revision 1.1.4.1  1995/12/11  19:52:31  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:51  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:18:14  root]
# 
# Revision 1.1.2.2  1994/10/06  20:32:34  agd
# 	expand copyright
# 	[1994/10/06  14:28:56  agd]
# 
# Revision 1.1.2.1  1994/08/04  16:15:21  mdf
# 	Initial checkin
# 	[1994/08/03  19:20:11  mdf]
# 
# $EndLog$
# 

TET_ROOT	= /test/tet/functional/security
DIR_TET_TESTS	= ../../../..
TET_SCEN_FILE	= ${DIR_TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${DIR_TET_TESTS}/tools/update_tet_scen

COMPONENT_DIR	= /ts/client/login/invalid_attempts

SCRIPTS		= inv_test1 good_test1a
OTHERS		= update_tet_scen_rule
DATA_FILES	= inv_data
ILIST		= ${SCRIPTS} ${DATA_FILES}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/


.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} inv_test1

