#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# (c) Copyright 1991, 1992 
#    Siemens-Nixdorf Information Systems, Burlington, MA, USA
#     All Rights Reserved
#
# HISTORY
# $Log: rpc.mk,v $
# Revision 1.1.18.4  1996/03/11  12:21:53  marty
# 	Update OSF copyright years.
# 	[1996/03/11  12:21:41  marty]
#
# Revision 1.1.18.3  1995/12/13  21:19:07  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/13  21:18:18  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/13  21:17:51  root]
# 
# Revision 1.1.16.2  1994/07/13  22:33:17  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:17:52  devsrc]
# 
# 	Removed OSDEBUG from HP kernel flags
# 	[1994/06/27  18:59:35  mckeen]
# 
# Revision 1.1.16.1  1993/12/28  22:11:35  pellis
# 	Updated for ODE 2.3 make
# 	[1993/12/28  21:55:00  pellis]
# 
# Revision 1.1.14.4  1993/08/12  16:06:54  kissel
# 	OT 8341 - Use kernel compile flags and header files for kernel rpc tests.
# 	[1993/08/11  19:20:10  kissel]
# 
# Revision 1.1.14.3  1993/08/03  19:42:41  tom
# 	Bug 3931 - Move default definitions up to test.mk.
# 	[1993/08/03  16:58:45  tom]
# 
# Revision 1.1.14.2  1993/05/24  20:48:30  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:16:01  cjd]
# 
# Revision 1.1.12.2  1993/05/12  18:25:39  jd
# 	Initial 486 port.
# 	[1993/05/12  18:25:20  jd]
# 
# Revision 1.1.6.4  1993/01/11  21:14:53  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  14:55:34  bbelch]
# 
# Revision 1.1.6.3  1992/10/14  20:43:48  marty
# 	OSC1.1.1 upgrade. Changes are compatible with osc1.0.4.
# 	[1992/10/14  20:02:43  marty]
# 
# Revision 1.1.7.2  1992/10/08  21:08:26  garyf
# 	fixes for compilation on OSF/1 R1.1
# 	[1992/09/29  20:11:48  garyf]
# 
# Revision 1.1.6.2  1992/09/29  21:22:17  devsrc
# 	SNI/SVR4 merge.  OT 5373
# 	[1992/09/11  15:47:28  weisman]
# 
# Revision 1.1.2.7  1992/06/26  07:04:04  jim
# 	Set CCTYPE to traditional for AIX 3.2 in the rpc/rtandidl/moretests
# 	directory.  Some of these files cannot be compiled ANSI due to
# 	macro redefinitions.
# 	[1992/06/12  01:26:04  jim]
# 
# Revision 1.1.2.6  1992/05/26  21:41:18  garyf
# 	make kdriver build
# 	add PMAX compilation options
# 	[1992/05/26  20:59:42  garyf]
# 
# Revision 1.1.2.5  1992/05/20  21:27:42  rsalz
# 	Set USE_SHARED_LIBRARIES=0 for kv2test.
# 	[1992/05/15  18:08:32  rsalz]
# 
# Revision 1.1.2.4  1992/05/11  19:51:27  mhickey
# 	Submitted wrong verson of file.  See log below for changes.
# 	Defect 3332
# 	[1992/05/11  19:50:52  mhickey]
# 
# Revision 1.1.2.2  1992/05/01  13:47:53  rsalz
# 	Added as part of rpc6 drop.
# 	[1992/04/30  23:21:51  rsalz]
# 
# $EndLog$
#

.if !defined(_TESTRPC_MK_)
_TESTRPC_MK_=

#
# Get useful stuff from non-test rpc.mk.
#
.include "${MAKETOP}/rpc/rpc.mk"

#
# Add kernel specific defines, include dirs, etc. for kernel rpc tests.
#
.if ${MAKESUB} == "test/rpc/runtime/kncstest/" || ${MAKESUB} == "test/rpc/runtime/kv2test/" \
 || ${MAKESUB} == "test/rpc/runtime/kdriver/"

.if ${TARGET_MACHINE} == "PMAX"
USE_SHARED_LIBRARIES=0
KERNEL_INCDIRS := ${INCDIRS:S;${target_machine}/usr/include;&/kernel;g:S/^-I//g}
parse_ext_args = action_ADDEXISTS list_INCDIRS dirs_KERNEL_INCDIRS addhow_PREPEND prefix_-I
.include <osf.parse_ext.mk>
.endif

# The following is copied directly from ./rpc/rpc.mk for HPUX.  Maybe things should be
# rearranged to leave this in there since it is included above, but that seems like too big a
# change for now.
#
.if ${TARGET_MACHINE} == "HP800"
.if !defined(TARGET_OS_VERSION)
# default version is UX9.0 ...
TARGET_OS_VERSION = UX90
.endif

.if ${TARGET_OS_VERSION} == "UX80"
HP800_KERNEL_CFLAGS = -DGENERIC -DWSIO_PHYSIO -DEQUIVMEM -DNGRAF_WS -DSPARSE_PDIR \
		-D_WSIO -DSNAKES_IO -DWIPER -DPA89 -DGETMOUNT -DLOCAL_DISC -DFSS -DREGION -DAPPLETALK \
		-D_HPUX_SOURCE -DHFS -DFS_TUNE1 -DAUDIT -DACLS -DFSD_KI -DPSTAT -DPOSIX -DMP \
		-DLWSYSCALL -DSYMLINK -DICA_ON -DRDB -DEQUIVMEM -DSYSCALLTRACE -DNOCOMPAT -DNOPAGING \
		-DBSDJOBCTL -DIODCIO -DNIO -DQUOTA -DFPC_BUG -DNSYNC -DEISA_PDC_KLUDGE -DKERNEL_SWITCH \
		-DTEAC_FLOPPY -DAUTOCHANGER -D_PA_RISC_1_1 -DTIMEX_BUG -DTIMEX -DHP_DFS
.else
HP800_KERNEL_CFLAGS = -DGENERIC -DAPPLETALK -DFDDI_VM -DHW_VISIBLE_TBL -D_KERNEL_BUILD -D_UNSUPPORTED -DWSIO_PHYSIO \
		-DEQUIVMEM -DNGRAF_WS -DSPARSE_PDIR -D_WSIO -DSNAKES_IO -DPA89 -DGETMOUNT -DLOCAL_DISC -DFSS \
		-DREGION -D_HPUX_SOURCE -DHFS -DAUDIT \
		-DACLS -DFSD_KI -DPSTAT -DPOSIX -DMP -DLWSYSCALL -DICA_ON -DRDB -DEQUIVMEM -DSYSCALLTRACE \
		-DBSDJOBCTL -DQUOTA -DFPC_BUG -DNSYNC -DTEAC_FLOPPY -DAUTOCHANGER -D_PA_RISC_1_1 -DTIMEX_BUG -DTIMEX -DHP_DFS
.endif

USE_SHARED_LIBRARIES = 0
NO_ZFLAG=1
parse_ext_args = action_ADDEXISTS list_INCDIRS dirs_HP_KERNEL_INCDIRS addhow_PREPEND prefix_-I
.include <osf.parse_ext.mk>
.endif

.endif

.if ${MAKESUB}=="test/rpc/rtandidl/testsh/"
CCTYPE		?= ansi
.endif

.if ${MAKESUB}=="test/rpc/runtime/perf/"
RIOS_LIBS       ?= -lm ${RIOS_LIBS_COMMON}
VAX_LIBS        ?= -lm ${VAX_LIBS_COMMON}
PMAX_LIBS       ?= -lm ${PMAX_LIBS_COMMON}
AT386_LIBS      ?= -lm ${AT386_LIBS_COMMON}
MIPS_LIBS       ?= -lm ${MIPS_LIBS_COMMON}
HP_PA_LIBS      ?= -lm ${HP_PA_LIBS_COMMON}
HP800_LIBS 	?= -lm ${HP800_LIBS_COMMON}
HP300_LIBS 	?= -lm ${HP800_LIBS_COMMON}
.endif

.if ${MAKESUB}=="test/rpc/rtandidl/moretests/"
RIOS_LDFLAGS	?= -bnodelcsect
CCTYPE		?= ansi
.endif

.if !empty(MAKESUB:Mtest/rpc/idl/*)
CCTYPE		?= ansi
.endif

#
# Defaults for standard target-specific definitions (in case they weren't
# defined above).
#

RIOS_CFLAGS     ?= ${RIOS_CFLAGS_COMMON}
VAX_CFLAGS      ?= ${VAX_CFLAGS_COMMON}
HP_PA_CFLAGS    ?= ${HP_PA_CFLAGS_COMMON}
HP800_CFLAGS 	?= ${HP800_CFLAGS_COMMON}
HP300_CFLAGS	?= ${HP300_CFLAGS_COMMON}
PMAX_CFLAGS     ?= ${PMAX_CFLAGS_COMMON}
AT386_CFLAGS    ?= ${AT386_CFLAGS_COMMON}
SVR4_CFLAGS     ?= ${TARGET_FLAGS} -DSYSV

RIOS_YFLAGS     ?= ${RIOS_YFLAGS_COMMON}

RIOS_LIBS       ?= ${RIOS_LIBS_COMMON}
VAX_LIBS        ?= ${VAX_LIBS_COMMON}
PMAX_LIBS       ?= ${PMAX_LIBS_COMMON}
AT386_LIBS      ?= ${AT386_LIBS_COMMON}
MIPS_LIBS       ?= ${MIPS_LIBS_COMMON}
APOLLO68K_LIBS  ?= ${APOLLO68K_LIBS_COMMON}
HP_PA_LIBS      ?= ${HP_PA_LIBS_COMMON}
HP800_LIBS 	?= ${HP800_LIBS_COMMON}
HP300_LIBS 	?= ${HP300_LIBS_COMMON}

.endif
