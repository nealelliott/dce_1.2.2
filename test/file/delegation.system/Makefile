#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# Transarc com COPYRIGHT 1994

EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}
EXPDIR                  = /usr/include/

INCLUDES                = delegate.h check_status.h

PROGRAMS = delegate delegated dfs_exe_file
DATAFILES = run_server.sh wait_for_server.sh stopem.sh \
	setup.dcp do_dfs_dir.dcp do_dfs_file.dcp README \
	template.tcl cellconfig.data template.sh runas.sh \
	run_simple_cmd.sh do_deleg_chain.dcp driver.sh \
	cellconfig.sh

CFLAGS = -Dunix ${USERCFLAGS}
LIBS	= -ldce ${OS_LIBS}

IDLFILES     = delegate.idl  
ACFFILES     = delegate.acf
IDLFLAGS     = -keep c_source -cepv
IDLDATAFILES = delegate.h

delegated_OFILES = delegate_sstub.o delegated.o delegate_funcs.o \
	delegate_cstub.o delegate_util.o check_access.o

delegate_OFILES  = delegate.o delegate_cstub.o delegate_util.o check_access.o

dfs_exe_file_OFILES = dfs_exe_file.o

ILIST	= ${PROGRAMS} ${DATAFILES} 

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
