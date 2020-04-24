# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/06/04  22:06:09  arvind
# 	Merge OSF endgame fixes to HP.
# 	[1996/03/14  19:48 UTC  psn  /main/DCE_1.2/1]
#
# 	Fix OT13361 as was done during OSF 1.2.1 Endgame.
# 	[1996/02/28  23:58 UTC  psn  /main/psn_1.2_endgame/1]
#
# Revision 1.1.4.3  1996/03/11  02:06:17  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:15  marty]
# 
# Revision 1.1.4.2  1996/02/22  21:42:24  root
# 	ENDGAME: Build fix
# 	[1996/02/22  21:41:46  root]
# 
# Revision 1.1.4.1  1995/12/11  19:50:11  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:25:57  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:17:17  root]
# 
# Revision 1.1.2.5  1994/09/27  18:47:25  mori_m
# 	CR 11041:  Added new functional tests for conformant array operation.
# 	[1994/09/27  18:29:59  mori_m]
# 
# Revision 1.1.2.4  1994/09/23  19:21:58  mori_m
# 	Merged with changes from 1.1.2.3
# 	[1994/09/23  19:21:50  mori_m]
# 
# 	CR 11041/11044
# 	        Deleted the tests based on the wrong assumption (code sets i14y
# 	        cannot be done from the stubs), and added more tests for varying array.
# 	[1994/09/23  18:29:04  mori_m]
# 
# Revision 1.1.2.3  1994/09/23  18:43:37  mori_m
# 	CR 11041/11044
# 	        Deleted the tests based on the wrong assumption (code sets i14y
# 	        cannot be done from the stubs), and added more tests for varying array.
# 	[1994/09/23  18:29:04  mori_m]
# 
# Revision 1.1.2.2  1994/06/10  20:43:35  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:15:51  devsrc]
# 
# Revision 1.1.2.1  1994/05/04  21:54:14  mori_m
# 	CR 9701: RPC runtime I18N extension functional test drop.
# 	[1994/05/04  21:32:55  mori_m]
# 
# $EndLog$

TET_ROOT	= /test/tet/functional/rpc/runtime
TET_TESTS	= ../../..
TET_SCEN_FILE	= ${TET_TESTS}/tet_scen

DATAFILES	= i18n_input_data i18n_expected_result result_check.sh
STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR	= /i18n_api/ts/cs_eval/wchar

VPATH		= ${MAKETOP}/test/functional/rpc/runtime/i18n_api/common:${MAKETOP}/test/tet/src/posix_c/api
CFLAGS		+= -DIDL_PROTOTYPES

LIBS            = -ldce -lc ${STANDARD_TETLIBS}
PROGRAMS        = server \
		  method_fixed \
		  method_copy_fixed \
		  method_vary \
		  method_conf \
		  method_conf_vary \
		  tags_fixed \
		  tags_copy_fixed \
		  tags_fixed_cmir \
		  tags_fixed_smir \
		  tags_vary \
		  tags_vary_cmir \
		  tags_vary_smir \
		  tags_conf \
		  tags_conf_cmir \
		  tags_conf_smir \
		  tags_conf_vary \
		  tags_conf_vary_cmir \
		  tags_conf_vary_smir

TESTS		= ${PROGRAMS}
CFLAGS		+= -DIDL_PROTOTYPES

COMMON_OFILES	= ts-common.o
SERVER_OFILES	= cs_test_sstub.o manager.o
CLIENT_OFILES	= cs_test_cstub.o

server_OFILES		= ${SERVER_OFILES} server.o
method_fixed_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} method_fixed.o
method_copy_fixed_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} method_copy_fixed.o
method_conf_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} method_conf.o
method_vary_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} method_vary.o
method_conf_vary_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} method_conf_vary.o
tags_fixed_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_fixed.o
tags_copy_fixed_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_copy_fixed.o
tags_fixed_cmir_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_fixed_cmir.o
tags_fixed_smir_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_fixed_smir.o
tags_vary_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_vary.o
tags_vary_cmir_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_vary_cmir.o
tags_vary_smir_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_vary_smir.o
tags_conf_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_conf.o
tags_conf_cmir_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_conf_cmir.o
tags_conf_smir_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_conf_smir.o
tags_conf_vary_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_conf_vary.o
tags_conf_vary_cmir_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_conf_vary_cmir.o
tags_conf_vary_smir_OFILES	= ${COMMON_OFILES} ${CLIENT_OFILES} tags_conf_vary_smir.o

server_LIBS		= -ldce -lc
method_fixed_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
method_copy_fixed_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
method_conf_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
method_vary_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
method_conf_vary_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
tags_fixed_LIBS		= -ldce -lc ${STANDARD_TETLIBS}
tags_copy_fixed_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
tags_fixed_cmir_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
tags_fixed_smir_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
tags_vary_LIBS		= -ldce -lc ${STANDARD_TETLIBS}
tags_vary_cmir_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
tags_vary_smir_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
tags_conf_LIBS		= -ldce -lc ${STANDARD_TETLIBS}
tags_conf_cmir_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
tags_conf_smir_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
tags_conf_vary_LIBS		= -ldce -lc ${STANDARD_TETLIBS}
tags_conf_vary_cmir_LIBS	= -ldce -lc ${STANDARD_TETLIBS}
tags_conf_vary_smir_LIBS	= -ldce -lc ${STANDARD_TETLIBS}

INCDIRS		+= -I${EXPORTBASE}/usr/include/dce
INCFLAGS        = -I${MAKETOP}test/functional/rpc/runtime/i18n_api/headers

ILIST		= ${PROGRAMS} ${DATAFILES}
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/
IDLFLAGS	= -keep c_source
IDLFILES	= cs_test.idl

cs_test_sstub.o:	cs_test.idl
cs_test_cstub.o:	cs_test.idl

.include <${RULES_MK}>
