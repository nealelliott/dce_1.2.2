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
# Makefile for Cross-development environment of RISCOS tools building SVR4 targets
# 
# HISTORY
# $Log: mipstosinix.mk,v $
# Revision 1.1.6.2  1996/03/09  23:25:13  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:40:52  marty]
#
# Revision 1.1.6.1  1995/12/07  22:09:25  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:08:25  root]
# 
# Revision 1.1.3.4  1992/12/31  16:47:01  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:50:50  zeliff]
# 
# Revision 1.1.3.3  1992/12/23  17:34:59  john
# 	Fixed the copyright marker to be OSF_FREE_COPYRIGHT.
# 	[1992/12/23  17:14:13  john]
# 
# Revision 1.1.3.2  1992/09/29  20:02:12  devsrc
# 	New file for SVR4 OSF merge
# 
# 	Submitted by STeve Gilbane
# 	[1992/09/11  15:32:27  rec]
# 
# $EndLog$
# 

.if !defined(_XSVR4_DCE_MK_)
_XSVR4_DCE_MK_=

#
# Here is where we set the tools we need
# these MUST precede the .include line
#
XBINDIR		=	/usr/dsg/bin

SNI_CROSS_BUILD	=	1
CCTYPE		= 	host
HOST_CC		?=	${XBINDIR}/Xcc.noucb
TRADITIONAL_CC	?=	${XBINDIR}/Xcc.noucb
AS		?=	/usr/cessna4/CrossCompilers/ccs/bin/as
LEX		?=	${XBINDIR}/Xlex
YACC		?=	${XBINDIR}/Xyacc
HOST_AR		?=	${XBINDIR}/Xar
HOST_LD		?=	${XBINDIR}/Xld
HOST_RANLIB	=	echo ranlib not needed of 
HOST_AS		?=	/usr/cessna4/CrossCompilers/ccs/bin/as
TRADITIONAL_AR	?=	${XBINDIR}/Xar
TRADITIONAL_LD	?=	${XBINDIR}/Xld
TRADITIONAL_RANLIB	=	echo ranlib not needed of 
TRADITIONAL_AS	?=      /usr/cessna4/CrossCompilers/ccs/bin/as
COMPILE_ET	?=	compile_et
PRS		?=	prs
MSG		?=	msg
MAVROS		?=	mavros
MAVCOD		?=	mavcod
RELEASE		=	release.pl
PARSER_AID	?=	parser_aid
UNIFDEF		?=	echo unifdef not supported
AWK		?=	/usr/bsd43/bin/awk

SNILIBS		?= 

#These AR defines are needed after the inclusion of osf.rules.mk
HOST_AR		?=	${XBINDIR}/Xar
TRADITIONAL_AR	?=	${XBINDIR}/Xar
ANSI_AR 	?= 	${XBINDIR}/Xar

# default to debugging 
OPT_LEVEL 	?= 

# To use these, undef OPT_LEVEL and set these
CC_OPT_LEVEL 	= 
LD_OPT_LEVEL 	=

OBJECT_FORMAT	= ELF

# This flag is used in the source code construct:
#	#if (__STDC__ || defined(__STDC_ZERO_OK__))
# to handle ANSI compilers (like ours) that define __STDC__ to be 0
STDC_FLAG	?= -D __STDC_ZERO_OK__

# UCB or POSIX
UCB_FLAG	=	-DSNI_SVR4_POSIX -DSNI_POSIX_CHECK= 

# Defines for platform
TARGET_FLAGS	?= -D SNI_SINIX \
	  -D SNI_MX300I \
	  -D M_I86 \
	  -D SNI_SVR4 \
  	  ${UCB_FLAG} \
  	  ${STDC_FLAG} \
	  -D SVR4_UDSOCKET_NOCHMOD \
	  -D SNI_SVR4_TAINTED \
	  -D SNI_PROPRIETARY

# set this for other misc cflags
_host_CFLAGS_	=

TARGET_LIBS     ?= -Wl,-I${INTERPRETER_DIR}/libc_r.so.1 -ldce -lm_r ${SNILIBS} \
		-lsocket_r -lnsl_r_i -lmalloc_r -lresolv_r -lucb_r -lc_r 

# SVR4 uses -cf for flags instead of cf
TAR_INFLAGS	= -cf
TAR_OUTFLAGS	= -xof

#
# Installed shared libraries.  These may be defined in component makefiles as:
#	INTERMED_SHLIBS	= foo.so
#	foo.so_IDIR	= /usr/lib/
# where it is assumed there is an associated foo.a already defined.  The intent
# is to declare a dynamic version of a library that will be installed on a DCE system.
# The rules below adds the .so target(s) to the lists of targets to be
# installed, exported, and cleaned.  The build rule depends on the building
# of the associated .a and extracts the component .o's from the .a.
#

# Have to include this to pull in <component.mk> where INTERMED_SHLIBS gets defined
.include <osf.dce.mk>

.if defined(INTERMED_SHLIBS)

# Flags for cc when building shared libraries
SHCCFLAGS		= -G -K PIC

EXPLIB_TARGETS	+=	${INTERMED_SHLIBS:S/^/export_/g}
ILIST		+=	${INTERMED_SHLIBS}
GARBAGE		+=	${INTERMED_SHLIBS}

${INTERMED_SHLIBS}: ${INTERMED_SHLIBS:.so=.a}
	${_AR_} x ${.TARGET:.so=.a}
	${_CC_} ${SHCCFLAGS} -o ${.TARGET} ${OFILES:U${${.TARGET:.so=.a}_OFILES:U*.o}}
	${RM} ${_RMFLAGS_} ${OFILES:U${${.TARGET:.so=.a}_OFILES:U*.o}}
.endif

EXPORT_USING_SYMLINKS	= 1

.include <osf.rules.mk>

# osf.std.mk gives ansi {INC,LIB}DIRS but not host (?)
_host_INCDIRS_	=	${_ansi_INCDIRS_}
_host_LIBDIRS_	=	${_ansi_LIBDIRS_}
_host_GENINC_	=	${x:L:!${GENPATH} -I.!}

# The following avoid the use of environment variables
# If non-ODE environment, these should be set in the environment
ODEINCDIRS	!= 	sbinfo incdirs
INCDIRS		+=  	${ODEINCDIRS}
ODELIBDIRS	!=	sbinfo libdirs
LIBDIRS		+=  	${ODELIBDIRS}

#
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
