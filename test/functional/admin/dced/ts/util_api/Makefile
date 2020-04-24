# @OSF_COPYRIGHT@
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.3  1996/03/11  02:29:52  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:27  marty]
#
# Revision 1.1.6.2  1995/12/11  15:19:59  root
# 	Submit
# 	[1995/12/11  14:33:35  root]
# 
# Revision 1.1.2.2  1994/10/18  22:51:39  max
# 	Eliminated spurious object files
# 	[1994/10/18  22:51:13  max]
# 
# Revision 1.1.2.1  1994/10/18  20:55:39  max
# 	Initial submission of FVTs for dced utility API
# 	[1994/10/18  20:47:54  max]
# 
# $EndLog$
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
# Revision 1.1.6.3  1996/03/11  02:29:52  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:27  marty]
#
# Revision 1.1.7.2  1996/03/10  19:37:27  marty
# 	Update OSF copyright years
#
# Revision 1.1.6.2  1995/12/11  15:19:59  root
# 	Submit
# 	[1995/12/11  14:33:35  root]
#
# Revision 1.1.7.2  1995/12/11  14:33:35  root
# 	Submit
#
# Revision 1.1.2.2  1994/10/18  22:51:39  max
# 	Eliminated spurious object files
# 	[1994/10/18  22:51:13  max]
#
# Revision 1.1.3.2  1994/10/18  22:51:13  max
# 	Eliminated spurious object files
#
# Revision 1.1.2.1  1994/10/18  20:55:39  max
# 	Initial submission of FVTs for dced utility API
# 	[1994/10/18  20:47:54  max]
#
# Revision 1.1.1.2  1994/10/18  20:47:54  max
# 	Initial submission of FVTs for dced utility API
#
# $EndLog$
#
#
#


# Compiler flags for this directory
.ifdef _NO_TET
CFLAGS = ${${TARGET_MACHINE}_CFLAGS} -DDEBUG -D_NO_TET
STANDARD_TETLIBS= 
NON_TET_MAIN=ntet_main.o
.else
CFLAGS = ${${TARGET_MACHINE}_CFLAGS} -DDEBUG
STANDARD_TETLIBS= -ltettcm -ltetapi
NON_TET_MAIN=
.endif

INCFLAGS =

# Programs to build
PROGRAMS = 	                     \
		dce_util_test_server \
		inq_uuids            \
		inq_attr             \
		inq_server           \
		sec_begin            \
		sec_done             \
		register             \
		unregister           \
		disable_service      \
		enable_service       \
		use_protseq



DATAFILES	= \
		tutil.tcl

dce_util_test_server_OFILES =               \
		dce_util_test_if_sstub.o    \
		dce_util_test_server.o      \
		inq_uuids_s.o               \
		inq_attr_s.o                \
		inq_server_s.o              \
		sec_begin_s.o               \
		sec_done_s.o                \
		register_s.o                \
		unregister_s.o              \
		disable_service_s.o         \
		enable_service_s.o          \
		use_protseq_s.o





inq_uuids_OFILES	=   inq_uuids_t.o common.o ${NON_TET_MAIN}
inq_attr_OFILES		=   inq_attr_t.o common.o ${NON_TET_MAIN}
inq_server_OFILES	=   inq_server_t.o common.o ${NON_TET_MAIN}
sec_begin_OFILES	=   sec_begin_t.o common.o ${NON_TET_MAIN}
sec_done_OFILES		=   sec_done_t.o common.o ${NON_TET_MAIN}
register_OFILES		=   register_t.o common.o ${NON_TET_MAIN}
unregister_OFILES	=   unregister_t.o common.o ${NON_TET_MAIN}
disable_service_OFILES	=   disable_service_t.o common.o ${NON_TET_MAIN}
enable_service_OFILES	=   enable_service_t.o common.o ${NON_TET_MAIN}
use_protseq_OFILES	=   use_protseq_t.o common.o ${NON_TET_MAIN}




# Libraries the programs need to link with
LIBS = -ldce ${STANDARD_TETLIBS}   ${${TARGET_MACHINE}_LIBS}
IDIR            = /test/tet/functional/admin/dced/ts/util_api/
ILIST		= ${PROGRAMS} ${DATAFILES}
IDLFLAGS        = -keep c_source
IDLFILES        = dce_util_test_if.idl

.include <${RULES_MK}>

# other dependencies
${dce_util_test_server_OFILES} : ${IDLFILES:.idl=.h}
#${server_start_OFILES} ${server_stop_OFILES} : ${IDLFILES}
#${server_enable_if_OFILES} ${server_disable_if_OFILES} : ${IDLFILES}







