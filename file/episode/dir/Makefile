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
# Revision 1.1.900.1  1996/10/02  17:20:19  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:29:36  damon]
#
# Revision 1.1.894.3  1994/06/09  14:00:00  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:35:17  annie]
# 
# Revision 1.1.894.2  1994/02/04  20:12:46  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:00  devsrc]
# 
# Revision 1.1.89.1  1993/12/07  17:18:24  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:53:31  jaffe]
# 
# Revision 1.1.5.4  1993/01/18  22:10:19  cjd
# 	embedded copyright notice
# 	[1993/01/18  21:51:19  cjd]
# 
# Revision 1.1.5.3  1992/11/24  16:24:01  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  16:21:51  bolinger]
# 
# Revision 1.1.5.2  1992/08/31  19:20:24  jaffe
# 	Transarc delta: jdp-ot4179-lfs-encapsulate-initialization 1.3
# 	  Selected comments:
# 	    This delta simplifies Episode user-space initialization.  There are two parts
# 	    to this change.  The first simplifies the process of calling the *_Init
# 	    routines.  Previously, you typically had to call dirb_Init(), N anode init
# 	    routines, and logbuf_Init().  With this change, you need only specify the
# 	    initialization parameters to the necessary layers in one structure and call
# 	    the top-most init routine.  Since each initialization routine is now
# 	    responsible for calling the lower layers' initialization routines, only
# 	    one call is necessary.  The second change made in this delta was the
# 	    encapsulation of the user-space device initialization code in one routine,
# 	    us_InitDevice().  This routine is declared in tools/us.h and defined in
# 	    tools/us_io.c.
# 	    Added "-ldce" to LIBS line.
# 	    Fix a couple of remaining compilation problems.
# 	    Fix compilation problems on OSF/1.
# 	[1992/08/30  02:15:41  jaffe]
# 
# Revision 1.1.2.2  1992/05/05  19:03:43  jdp
# 	delta bwl-build-lwp 1.4
# 	delta jdp-epimerge-use-ode-makefiles 1.5
# 	[1992/05/04  21:18:14  jdp]
# 
# Revision 1.1  1992/01/19  02:40:48  devrcs
# 	Initial revision
# 
# $EndLog$
#

INCLUDES                = dir.h dir_buffer.h
LIBRARIES		= libdir.a
PROGRAMS		= dtest damager

EXPDIR			= /usr/include/dcedfs/
libdir.a_EXPDIR		= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

CFLAGS			= ${USERCFLAGS}
libdir.a_OFILES		= dir_buffer.o dir_main.o dir_salvage.o

dtest_LIBS		= -ldir -lanode -llogbuf -lasync \
			  -lbomb -lafsutil -lafssys -ltools -llwp -licl -losi \
			  -lcom_err -lncompat -ldce ${OS_LIBS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
