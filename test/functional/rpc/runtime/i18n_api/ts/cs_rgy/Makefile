# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:06:32  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:21  marty]
#
# Revision 1.1.4.1  1995/12/11  19:51:35  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:29  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:17:52  root]
# 
# Revision 1.1.2.3  1994/06/22  19:59:29  mori_m
# 	CR 10043:  Separated access_cs_rgy.c into machine dependent directory.
# 	[1994/06/22  19:55:17  mori_m]
# 
# Revision 1.1.2.2  1994/06/10  20:44:19  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:16:09  devsrc]
# 
# Revision 1.1.2.1  1994/05/04  21:54:42  mori_m
# 	CR 9701: RPC runtime I18N extension functional test drop.
# 	[1994/05/04  21:20:59  mori_m]
# 
# $EndLog$

TET_ROOT	= /test/tet/functional/rpc/runtime
TET_TESTS	= ../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen

STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR	= /i18n_api/ts/cs_rgy

VPATH		= ${MAKETOP}/test/functional/rpc/runtime/i18n_api/common:${TARGET_MACHINE}
LIBS            = -ldce -lc ${STANDARD_TETLIBS}

PROGRAMS        = access_cs_rgy \
		  get_max_bytes

TESTS		= ${PROGRAMS}

COMMON_OFILES	= ts-common.o

access_cs_rgy_OFILES= ${COMMON_OFILES} access_cs_rgy.o
get_max_bytes_OFILES= ${COMMON_OFILES} get_max_bytes.o 

INCDIRS		+= -I${EXPORTBASE}/usr/include/dce
INCFLAGS        = -I${MAKETOP}test/functional/rpc/runtime/i18n_api/headers

ILIST		= ${PROGRAMS}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

.include <${RULES_MK}>
