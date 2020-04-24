#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.73.1  1996/10/02  17:06:32  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:05:36  damon]
#
# $EndLog$
#
# $Header: /u0/rcs_trees/dce/rcs/file/cm/Makefile,v 1.1.73.1 1996/10/02 17:06:32 damon Exp $
#
# Copyright (c) Transarc Corporation 1996.  All rights reserved.

# To override the default values for the initial/minimum protection levels,
#   add definitions for one or more of the symbols below to CFLAGS .
#
# Initial protection level for communication in the same cell:
#   CM_DEFAULT_LCLINIT_PROTECTION   (defaults to rpc_c_protect_level_pkt_integ)
#
# Lower bound on protection level for communication in the same cell:
#   CM_DEFAULT_LCLMIN_PROTECTION   (defaults to rpc_c_protect_level_none)
#
# Initial protection level for communication in other cells:
#   CM_DEFAULT_RMTINIT_PROTECTION   (defaults to rpc_c_protect_level_pkt_integ)
#
# Lower bound on protection level for communication in other cells:
#   CM_DEFAULT_RMTMIN_PROTECTION   (defaults to rpc_c_protect_level_pkt)
#
# The SAME definitions should be added in the src/file/afsd directory as well.

SUBDIRS			= test
INCLUDES		= cm.h cm_sysname.h cm_chunk.h cm_trace.h cm_stats.h \
			  cm_serverpref.h
LIBRARIES		= libcm.a
OTHERS			= dfszcm.cat

EXPDIR			= /usr/include/dcedfs/
libcm.a_EXPDIR		= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

dfszcm.cat_IDIR		= /nls/msg/en_US.ASCII/

ILIST			= ${INCLUDES} ${OTHERS}
IDIR			= /share/include/dcedfs/

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS} ${cm_KERNELCFLAGS}
OFILES			= cm_aclent.o cm_bkg.o cm_cell.o cm_conn.o \
		  	  cm_daemons.o cm_dcache.o \
		  	  cm_init.o cm_lockf.o \
		  	  cm_pioctl.o cm_rrequest.o cm_scache.o cm_server.o \
		  	  cm_shutdown.o cm_subr.o cm_tknimp.o cm_tokens.o \
		  	  cm_vdirent.o cm_vfsops.o cm_vnodeops.o cm_volume.o \
		  	  cm_xsyscalls.o cm_dnamehash.o \
			  cm_pcache.o cm_vcache.o cm_async.o cm_serverpref.o \
			  cm_site.o

.if defined(EXPRESS_OPS)
OFILES += cm_expressops.o cm_express.o
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.if exists (${TARGET_OS}/Makefile)
.include "${TARGET_OS}/Makefile"
.endif

.include <${RULES_MK}>

cm_trace.h cm_trace.msf: cm_trace.et
	${COMPILE_ET} ${cm_trace.et:P} -h cm_trace

dfszcm.cat: cm_trace.msf
	${GENCAT} $@ $>
