# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:06:37  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:24  marty]
#
# Revision 1.1.4.1  1995/12/11  19:51:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:32  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:17:56  root]
# 
# Revision 1.1.2.2  1994/06/10  20:44:22  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:16:12  devsrc]
# 
# Revision 1.1.2.1  1994/05/04  21:54:47  mori_m
# 	CR 9701: RPC runtime I18N extension functional test drop.
# 	[1994/05/04  21:16:19  mori_m]
# 
# $EndLog$

TET_ROOT	= /test/tet/functional/rpc/runtime
TET_TESTS	= ../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen

STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR	= /i18n_api/ts/ns_mgmt

VPATH		= ${MAKETOP}/test/functional/rpc/runtime/i18n_api/common
LIBS            = -ldce -lc ${STANDARD_TETLIBS}

PROGRAMS        = ns_codesets1 \
		  ns_codesets2

TESTS		= ${PROGRAMS}

COMMON_OFILES	= ts-common.o
SERVER_OFILES	= dummy_if_sstub.o dummy_rtn.o

ns_codesets1_OFILES= ${COMMON_OFILES} ${SERVER_OFILES} ns_codesets1.o 
ns_codesets2_OFILES= ${COMMON_OFILES} ${SERVER_OFILES} ns_codesets2.o 

INCDIRS		+= -I${EXPORTBASE}/usr/include/dce
INCFLAGS        = -I${MAKETOP}test/functional/rpc/runtime/i18n_api/headers

ILIST		= ${PROGRAMS}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/
IDLFLAGS	= -keep c_source
IDLFILES	= dummy_if.idl

dummy_if_sstub.o:	dummy_if.idl

.include <${RULES_MK}>
