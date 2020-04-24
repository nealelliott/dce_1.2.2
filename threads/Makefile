# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.38.2  1996/06/04  22:16:10  arvind
# 	Eliminate warning from make; use default (system-wide) gencat rule.
# 	[1996/05/24  23:07 UTC  sommerfeld  /main/DCE_1.2.2/1]
#
# 	Build libcma.sl
# 	[1996/03/08  21:38 UTC  bissen  /main/DCE_1.2/2]
#
# 	Update OSF copyright year
# 	[1996/03/09  22:45:54  marty]
#
# 	Submit OSF/DCE 1.2.1
# 	HP 	[1995/11/16  21:42 UTC  jrr  /main/jrr_1.2_mothra/1]
#
# 	Add support for machine specific INCLUDES
# 	[1995/12/07  21:00:23  root]
#
# Revision 1.2.38.1  1996/05/10  13:30:48  arvind
# 	Drop 1 of DCE 1.2.2 to OSF
# 
# 	HP revision /main/DCE_1.2/2  1996/03/08  21:38 UTC  bissen
# 	Build libcma.sl
# 
# 	HP revision /main/DCE_1.2/1  1996/01/03  18:48 UTC  psn
# 	Add support for machine specific INCLUDES
# 	[1995/11/16  21:42 UTC  jrr  /main/jrr_1.2_mothra/1]
# 
# Revision 1.2.23.5  1994/09/07  20:56:11  bowe
# 	Added dcethdmsg.h and dcethdmsg.o (plus SAMS variables) [CR 11885]
# 	[1994/09/07  20:55:27  bowe]
# 
# Revision 1.2.23.4  1994/07/26  20:53:20  annie
# 	added VISTA - 11163
# 	[1994/07/26  20:53:12  annie]
# 
# Revision 1.2.23.3  1994/07/13  15:07:03  marty
# 	CR # 10017 - Set permissions on header files to 444 during install pass
# 	[1994/07/12  15:53:30  marty]
# 
# Revision 1.2.23.2  1994/06/09  13:35:16  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:25:57  devsrc]
# 
# Revision 1.2.23.1  1994/04/01  20:14:08  jd
# 	Fist drop of code cleanup
# 	[1994/03/28  19:15:22  jd]
# 
# Revision 1.2.19.3  1993/08/16  14:05:44  rhw
# 	changed gencat to ${GENCAT} in Makefile per OT CR7327
# 	[1993/08/09  19:25:25  rhw]
# 
# Revision 1.2.19.2  1993/06/10  19:28:37  sommerfeld
# 	Clean up backquotes; move `genpath ...` into INCFLAGS where it gets done
# 	automagically for you.
# 	[1993/03/18  18:06:32  sommerfe]
# 
# 	Clean up backquotes.
# 	[1993/03/18  18:04:59  sommerfe]
# 
# Revision 1.2.6.5  1993/02/06  00:57:34  jd
# 	Install pthread.h and pthread_exc.h in /usr/include.
# 	[1993/02/06  00:49:35  jd]
# 
# Revision 1.2.6.4  1992/12/15  22:22:50  alan
# 	Insert copyright notices
# 	[1992/12/07  18:56:12  alan]
# 
# Revision 1.2.6.3  1992/09/04  20:11:29  bolinger
# 	Fix OT defect 5317:  track name change of threads message
# 	catalog from dcethreads.cat to dcethd.cat (so that catalog
# 	can be found using standard dce_error_inq_text() lookup
# 	algorithm).
# 	[1992/09/04  15:04:59  bolinger]
# 
# Revision 1.2.6.2  1992/09/03  14:33:42  bolinger
# 	Replace current source file with equivalent from DEC BL10+
# 	source base, as originally integrated in the
# 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
# 	[1992/09/02  01:52:10  bolinger]
# 
# Revision 1.1.6.2  1992/09/01  15:18:46  bolinger
# 	Update CMA version for template.
# 	[1992/09/01  15:18:08  bolinger]
# 
# Revision 1.1.2.4  1992/08/14  20:20:46  bolinger
# 	Bring forward 1.0.1 revision 1.2.2.3 (fix for OT defect 2031).
# 	[1992/08/10  15:15:06  bolinger]
# 
# Revision 1.1.2.3  1992/05/10  17:40:47  bolinger
# 	Remove duplicated copyright, log.
# 	[1992/05/10  17:40:30  bolinger]
# 
# Revision 1.1.2.2  1992/05/10  16:04:56  bolinger
# 	Integrate threads/Makefile (revision 1.2.2.2) with makefile from
# 	BL10 DEC submission (in this directory [nosupport/threads_bl10]).
# 	[1992/05/10  16:04:35  bolinger]
# 
# Revision 1.2.2.3  1992/05/11  17:51:35  mhickey
# 	Removed obsolete define of NEED_PROJECT_MAKE_DEFS
# 	and include of osf.dce.mk.  Fixes defect 2031.
# 	[1992/04/22  15:23:31  mhickey]
# 
# Revision 1.2.2.2  1992/01/27  21:40:27  mhickey
# 	Removed dependency on obsolete DATAFILES functionality via a better
# 	.msg to .cat rule.
# 	[1992/01/27  21:39:47  mhickey]
# 
# Revision 1.2  1992/01/19  22:14:45  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0 
#
# 
#
#
# Library to build
#

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#
libcma_OFILES = \
	cma_alert.o		cma_assert.o 	        cma_attr.o 	\
	cma_condition.o		cma_context.o		cma_crtlx.o 	\
	cma_debugger.o		cma_deb_core.o		cma_deb_event.o	\
	cma_defer.o		cma_dispatch.o		cma_errno.o	\
	cma_errors.o		exc_handling.o		cma_handle.o	\
	cma_init.o		cma_kernel.o		cma_message.o	\
	cma_mutex.o		cma_once.o		cma_queue.o	\
	cma_pthread.o		cma_ptd_exc.o		cma_semaphore.o	\
	cma_sequence.o		cma_stack.o		cma_tcb.o	\
	cma_thread.o		cma_timer.o		cma_util.o	\
	cma_vm.o		cma_vp.o		cma_assem.o	\
	dcethdmsg.o	\
	${${TARGET_MACHINE}_libcma_OFILES}

libcma.a_OFILES = ${libcma_OFILES}

libcma.sl_OFILES = ${libcma_OFILES}

libcma.sl_SHLDFLAGS     = -b

libcma.sl_SHLDFLAGS     += -h cma__transfer_regs -h cma_singlestack_transfer_regs \
			   -h cma__restore_regs -h cma_singlestack_restore_regs

SHARED_LIBRARIES        = libcma.sl
EXPSHLIB_TARGETS        = ${libcma_OFILES} ${SHARED_LIBRARIES:S/^/export_/g}
libcma.sl_IMODE = 555
libcma.sl_EXPDIR        = /usr/lib/

#
# the rest of the makefile is "private" for the build of libcma.a
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#
# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
# We don't put OBJECTS = here since this Makefile actually has this list 
#
LIBS = 

CATNAME		= dcethd
VPATH           = ${TARGET_MACHINE}:COMMON

#
# Don't want to build any '.a' here if we are using shared libraries
#
.if ${USE_SHARED_LIBRARIES} == 0
  LIBRARIES     = 	libcma.a
  EXPLIB_TARGETS        = ${LIBRARIES:S/^/export_/g}
.endif

SAMSFILES		= thd.sams
SAMSFILES_INCLUDES	= dcethdmsg.h
SAMS_TARGETS		= dcethdmsg.c

#EXPSHLIB_TARGETS= ${libcma_OFILES}
EXPBIN_TARGETS	= dcethd.cat
IDIR		= /share/include/dce/
EXPDIR		= /usr/include/dce/

#
# For some reason, CC_OPT_LEVEL and OPT_LEVEL seem to be ignored by our
# ODE 2.0.2 unless specified manually on the build command line; so hack
# around it by using "OPTLEVEL". To make a debug build, specify "OPTLEVEL=-g"
# on the build command.
#
OPTLEVL		=
CFLAGS		= ${${TARGET_MACHINE}_CFLAGS} -D_CMA_NOWRAPPERS_
INCFLAGS	= -I${TARGET_MACHINE}
AFLAGS		= ${OPTLEVEL} -Dunix -D__LANGUAGE_ASSEMBLER__ ${${TARGET_MACHINE}_AFLAGS}

OBJECTS		= 

OTHERS		= ${CATNAME}.cat

INCLUDES = 	cma.h		cma_config.h		\
		cma_px.h        pthread.h		pthread_exc.h \
		cmalib_crtlx.h	cma_sigwait.h		cma_errno.h \
		exc_handling.h		threadmsg.h \
		cma_defs.h	cma_queue.h		cma_assert.h \
		cma_kernel.h 	cma_defer.h		dcethdmsg.h \
		${${TARGET_MACHINE}_INCLUDES}

IMODE		= 444
ILIST		= ${INCLUDES} ${OTHERS}

libcma.a_EXPDIR		= /usr/lib/
cma_stdio.c_IDIR	= /share/examples/threads/
dcethd.cat_EXPDIR	= /usr/bin/
dcethd.cat_IDIR     = /nls/msg/en_US.ASCII/
dcethd.msg_EXPDIR	= /usr/bin/
dcethd.msg_IDIR	= /share/examples/threads/
pthread.h_EXPDIR	= /usr/include/
pthread.h_IDIR		= /share/include/
pthread_exc.h_IDIR	= /share/include/

cma_init.M: cma_version.h
cma_init.o: cma_version.h

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

cma_version.h: cma_version.template
	sed "s;<<version>>;CMA BL10+;" ${cma_version.template:P} > $@
.endif


