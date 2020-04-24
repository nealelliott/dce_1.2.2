#
# @OSF_FREE_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993, 1992, 1991, 1990, 1996 Open Software Foundation, Inc.
# Permission is hereby granted to use, copy, modify and freely distribute
# the software in this file and its documentation for any purpose without
# fee, provided that the above copyright notice appears in all copies and
# that both the copyright notice and this permission notice appear in
# supporting documentation.  Further, provided that the name of Open
# Software Foundation, Inc. ("OSF") not be used in advertising or
# publicity pertaining to distribution of the software without prior
# written permission from OSF.  OSF makes no representations about the
# suitability of this software for any purpose.  It is provided "as is"
# without express or implied warranty.
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
# All Rights Reserved
#
#	SVR4 makefile
#	@(#)svr4.mk	1.10
# 
# HISTORY
# $Log: svr4.mk,v $
# Revision 1.1.14.2  1996/03/09  23:25:32  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:08  marty]
#
# Revision 1.1.14.1  1995/12/07  22:11:17  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:09:06  root]
# 
# Revision 1.1.12.3  1994/04/19  20:07:03  ohara
# 	use export tree for crtn.o crtio.o
# 	[1994/04/19  20:03:28  ohara]
# 
# Revision 1.1.12.2  1994/02/22  18:38:52  ohara
# 	set the opt level
# 	[1994/02/18  16:02:31  ohara]
# 
# Revision 1.1.12.1  1994/01/04  19:41:39  ohara
# 	don't add "dce" to the end of the INCDIRS
# 	[1994/01/04  18:01:25  ohara]
# 
# Revision 1.1.10.1  1993/10/05  21:38:37  ohara
# 	SNI SVR4 ref port 8596
# 	[1993/09/15  22:03:48  ohara]
# 
# Revision 1.1.8.2  1993/06/24  20:08:26  hinman
# 	[hinman] - Save merged-in SNI version (these files have been freed of nasty code)
# 	[1993/06/17  14:22:43  hinman]
# 
# Revision 1.1.5.5  1993/02/01  20:27:10  hinman
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  15:58:59  hinman]
# 
# Revision 1.1.5.4  1992/12/31  16:48:13  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:52:28  zeliff]
# 	Revision 1.1.6.2  1993/01/11  16:10:11  hinman
# 	[hinman] - Check in merged SNI version
# 
# Revision 1.1.5.3  1992/12/23  17:35:14  john
# 	Fixed the copyright marker to be OSF_FREE_COPYRIGHT.
# 	[1992/12/23  17:15:02  john]
# 
# Revision 1.1.5.2  1992/09/29  20:02:45  devsrc
# 	New version for SVR4 - OSF merge
# 
# 	Submitted by  Steve Gilbane
# 	[1992/09/11  15:26:51  rec]
# 
# Revision 9.5.2.2  92/08/05  13:20:54  steveg
# 	New version for OSF Merge
# 
# $EndLog$
# 

.if !defined(_SVR4_DCE_MK_)
_SVR4_DCE_MK_=

#
# To add things to svr4.mk:
#	Put HARDWIRED variable assignments BEFORE <osf.rules.mk>
#	Put variable assignments that depend on other variables AFTER <osf.rules.mk>
#	Put new rules AFTER including osf.rules.mk 
#	The printout facility has to be at the end after all assignments are made
#

# Compilation environment
CCTYPE		=	host
HOST_CC		?=	${EXPORTBASE}/usr/bin/cc
CC_LD		?=	${EXPORTBASE}/usr/bin/ld
TRADITIONAL_CC	?=	${HOST_CC}
CPUTYPE		=	486
OBJECT_FORMAT	=	ELF

# OPT_LEVEL cannot be defined when using ld to link, since 
# the OPT_LEVEL flags are passed to both compiling and linking lines,
# so set these below separately.  If CC == LD, use OPT_LEVEL to set both.
# Note CC_OPT_LEVEL has to be set here to override -O in osf.std.mk.
CC_OPT_LEVEL 	?= -O
LD_OPT_LEVEL 	?=

# Utilities and tools
HOST_RANLIB	=	echo Ranlib not needed for
TRADITIONAL_RANLIB	= echo Ranlib not needed for
COMPILE_ET	?=	compile_et
DEF_ARFLAGS	?=	cr
PRS		?= 	prs
MSG		?=	msg
MAVROS		?=	mavros
MAVCOD		?=	mavcod
UNIFDEF		?=	/usr/ucb/unifdef
TAR_INFLAGS	?=	-cf
TAR_OUTFLAGS	?=	-xof
UUDECODE    	?=	uudecode
UNCOMPRESS  	?=	uncompress
UNCOMPRESS_FLAGS ?= -f

# for ODE
SVR4_VA_ARGV	=	VA_ARGV_IS_RECAST

# Because mandatory file locking has problems interoperating between
# SVR4 and other hosts, best to build GDS schema in /tmp (which probably
# isn't NFS-mounted)
SNI_TMP_DSA	= SNI_TMP_DSA

# This flag is used in the source code construct:
#	#if (__STDC__ || defined(__STDC_ZERO_OK__))
# to handle ANSI compilers (like SVR4's) that define __STDC__ to be 0
POSIX_FLAGS	?=	-DSNI_SVR4_POSIX
STDC_FLAG	?=	-D__STDC_ZERO_OK__

# The += operator on TARGET_FLAGS allows OS-dependant flags to be set above as well.
TARGET_FLAGS	+=	-D SNI_SVR4 \
			-D SNI_SVR4_TAINTED \
			-D SNI_PROPRIETARY \
			 -Kpic \
	  		 ${POSIX_FLAGS} \
	  		 ${STDC_FLAG}
# Make sure we set a default for INTERPRETER_DIR. It's set by the
# sandbox environment, but customers won't necessarily be using that
INTERPRETER_DIR	?=	/opt/dcelocal/lib
LD_RUN_PATH	?=	/opt/dcelocal/lib

INCDIRS		?=	-I${EXPORTBASE}/usr/include 
LIBDIRS		?=	-L${EXPORTBASE}/usr/lib

# In the setup phase we cannot use libdce or the re-entrant libraries.
#	Also, we set the linker to be cc (not ld), and clear flags normally
#	passed to ld
.ifdef MAKEFILE_PASS
.if ${MAKEFILE_PASS} == "SETUP"
TARGET_LIBS     	= -lc
TARGET_LDFLAGS		=
CC_LD			= ${EXPORTBASE}/usr/bin/cc
.endif
.endif

#Note, the following are set with '?' so that they will not
# possibly override settings made earlier.

TARGET_LIBS     	?= -I ${INTERPRETER_DIR}/libc_r.so.1  -ldce  -lm_r \
			   -lnsl_r_i -lmalloc_r -lresolv_r -lucb_r -lc_r \
			   -Qy ${EXPORTBASE}/usr/lib/crtn.o
TARGET_LDFLAGS		?= ${EXPORTBASE}/usr/lib/rcrt1.o \
	          	   ${EXPORTBASE}/usr/lib/crti.o /usr/ccs/lib/values-Xt.o \
		  	   -L${EXPORTBASE}/usr/lib -lsocket_r

# set this for other misc cflags
_host_CFLAGS_	=

.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#
# Installed shared libraries.  These may be defined in component makefiles as:
#	INTERMED_SHLIBS	= foo.so
#	foo.so_IDIR	= /usr/lib/
# where it is assumed there is an associated foo.a already defined.  The intent
# is to declare a dynamic version of a library that will be installed on a DCE system.
# The rules below adds the .so target(s) to the lists of targets to be
# installed, exported, and cleaned.  The build rule depends on the building
# of the associated .a and extracts the component .o's from the .a.
# Do not build while building libdce, since the LIBRARIES go away during that pass.
#

# We have to include this because INTERMED_SHLIBS are set in <component>.mk
.include <osf.dce.mk>

.if defined(INTERMED_SHLIBS)
EXPLIB_TARGETS	+=	${INTERMED_SHLIBS:S/^/export_/g}
ILIST		+=	${INTERMED_SHLIBS}
GARBAGE		+=	${INTERMED_SHLIBS}
# Flags for cc when building shared libraries
SHCCFLAGS		= -G -K PIC
${INTERMED_SHLIBS}: ${INTERMED_SHLIBS:.so=.a}
	${_AR_} x ${.TARGET:.so=.a}
	${_CC_} ${SHCCFLAGS} -o ${.TARGET} ${OFILES:U${${.TARGET:.so=.a}_OFILES:U*.o}}  ${SHDEPLIBS}
	${RM} ${_RMFLAGS_} ${OFILES:U${${.TARGET:.so=.a}_OFILES:U*.o}}
.endif
.endif
EXPORT_USING_SYMLINKS	= 1

# END OF BUILD-WIDE VARIABLE ASSIGNMENTS

.include <osf.rules.mk>

# Because we want to build ode from the same tree, include the ode-dependent
# stuff here
#
.if !empty(MAKESUB:Mode*)
.include <${MAKETOP}/ode/ode.mk>
.endif

# osf.std.mk gives _ansi_{INC,LIB}DIRS but not host (?)
_host_INCDIRS_	=	${_ansi_INCDIRS_}
_host_LIBDIRS_	=	${_ansi_LIBDIRS_}
_host_GENINC_	=	${x:L:!${GENPATH} -I.!}

# osf.obj.mk uses _PP_CCFLAGS_ for running cpp but no one defines it (?)
_PP_CCFLAGS_ = ${_CCFLAGS_}

# Debugging: must be last rules in the file
#
.include <osf.debugvar.mk>
#
# General debugging printout
#
printout:
	@echo 
	@echo "Make variables and some component variables"
	@echo "	in ${MAKESUB}."
	@echo "------- Compiler --------"
	@echo "_CC_		= ${_CC_} "
	@echo "CCTYPE		= ${CCTYPE}"
	@echo "_CCTYPE_	= ${_CCTYPE_}"
	@echo "_${_CCTYPE_}_CC_	= ${_${_CCTYPE_}_CC_}"
	@echo "HOST_CC		= ${HOST_CC}"
	@echo "------- cc Flags --------"
	@echo "_CCFLAGS_	= ${_CCFLAGS_}"
	@echo "_CC_CFLAGS_	= ${_CC_CFLAGS_}"
	@echo "_CC_OL_		= ${_CC_OL_}"
	@echo "OPT_LEVEL	= ${OPT_LEVEL}"
	@echo "CC_OPT_LEVEL	= ${CC_OPT_LEVEL}"
	@echo "CENV		= ${CENV}"
	@echo "CFLAGS		= ${CFLAGS}"
	@echo "TARGET_FLAGS	= ${TARGET_FLAGS}"
	@echo "CARGS		= ${CARGS}"
	@echo "_CC_NOSTDINC_	= ${_CC_NOSTDINC_}"
	@echo "_GENINC_		= ${_GENINC_}"
	@echo "_CC_GENINC_	= ${_CC_GENINC_}"
	@echo "_INCFLAGS_	= ${_INCFLAGS_}"
	@echo "INCARGS		= ${INCARGS}"
	@echo "INCFLAGS	= ${INCFLAGS}"
	@echo "INCENV		= ${INCENV}"
	@echo "_CC_INCDIRS_	= ${_CC_INCDIRS_}"
	@echo "INCDIRS		= ${INCDIRS}"
	@echo "------- ld Flags --------"
	@echo "_LIBFLAGS_	= ${_LIBFLAGS_}"
	@echo "LIBARGS		= ${LIBARGS}"
	@echo "LIBFLAGS	= ${LIBFLAGS}"
	@echo "LIBENV		= ${LIBENV}"
	@echo 
	@echo "_LDFLAGS_	= ${_LDFLAGS_}"
	@echo "_CC_LDFLAGS_	= ${_CC_LDFLAGS_}"
	@echo "_LD_OL_		= ${_LD_OL_}"
	@echo "LD_OPT_LEVEL	= ${LD_OPT_LEVEL}"
	@echo "LDENV		= ${LDENV}"
	@echo "LDFLAGS		= ${LDFLAGS}"
	@echo "LDARGS		= ${LDARGS}"
	@echo "_GENLIB_		= ${_GENLIB_}"
	@echo "_CC_LIBDIRS_	= ${_CC_LIBDIRS_}"
	@echo
	@echo "_LIBS_		= ${_LIBS_}"
	@echo "LIBSENV		= ${LIBSENV}"
	@echo "LIBS		= ${LIBS}"
	@echo "TARGET_LIBS	= ${TARGET_LIBS}"
	@echo "LIBRARIES	= ${LIBRARIES} "
	@echo "------- Utilties --------"
	@echo "_LD_		= ${_LD_}"
	@echo "_RANLIB_		= ${_RANLIB_}"
	@echo "_AR_		= ${_AR_}"
	@echo "MAKE		= ${MAKE}"
.endif
