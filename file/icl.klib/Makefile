# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.40.1  1996/10/02  17:51:53  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:40:35  damon]
#
# Revision 1.1.35.2  1994/06/09  14:11:04  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:25:01  annie]
# 
# Revision 1.1.35.1  1994/02/04  20:20:50  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:14:06  devsrc]
# 
# Revision 1.1.33.1  1993/12/07  17:26:43  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:35:51  jaffe]
# 
# Revision 1.1.2.6  1993/01/19  16:04:39  cjd
# 	embedded copyright notice
# 	[1993/01/19  14:14:45  cjd]
# 
# Revision 1.1.2.5  1993/01/13  18:08:03  shl
# 	Transarc delta: comer-ot6297-icl-user-interface 1.5
# 	  Selected comments:
# 	    Added some functioanlity and a user interface to the ICL package.  The
# 	    current command is called dfstrace; the current interface is:
# 	    ./dfstrace: Commands are:
# 	    apropos         search by help text
# 	    clear           clear logs by logname or by event set
# 	    dump            dump DFS trace logs from the kernel
# 	    help            get help on commands
# 	    lslogs          list available logs
# 	    lssets          list available event sets
# 	    setlog          set the size of a log
# 	    setset          set state of event sets
# 	    Usage: dfstrace clear  [{-set <set_name>... | -log <log_name>... | -all}]
# 	    [-help]
# 	    Usage: dfstrace dump  [{-set <set_name>... | -all | -follow <log_name>}]
# 	    [-file <output_filename>]
# 	    [-sleep <seconds_between_reads>] [-help]
# 	    To get a continuous dump of a log, for example, do:
# 	    dfstrace -follow cmfx -sleep 5
# 	    Usage: dfstrace lslogs  [{-set <set_name>... | -log <log_name>... | -all}]
# 	    [-long][-help]
# 	    Lists logs, their sizes, and whether or not they are currently
# 	    allocated.
# 	    Usage: dfstrace lssets  [{-set <set_name>... | -all}] [-help]
# 	    Lists available sets and their states.
# 	    Usage: dfstrace setlog  -log <log_name> -buffersize <4-kilobyte_units>
# 	    Set log size.
# 	    Usage: dfstrace setset  {-activate | -deactivate | -release}
# 	    [{-set <set_name>... | -all}] [-help]
# 	    Change the state of an event set.  In active state, logging
# 	    occurs to log.  In inactive state, no loggin occurs but the
# 	    reference to the log persists.  In the released state, the
# 	    reference to the log is gone, so the log may be deallocated.
# 	    Misc bug fixes.
# 	    logp->logElements wasn't getting reset on clear.
# 	    By default icl tracing is turned on.  This change makes it a little
# 	    easier to turn it on and off when compiling.  The change adds 2 flags:
# 	    ICL_DEFAULT_ENABLED and ICL_DEFAULT_DISABLED.  There is an accompanying
# 	    -D in icl and icl.klib Makefiles, currently both enabled.
# 	    Added -DICL_DEFAULT_ENABLED to CFLAGS.
# 	[1993/01/12  21:06:17  shl]
# 
# Revision 1.1.2.4  1992/11/24  17:53:22  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  18:06:52  bolinger]
# 
# Revision 1.1.2.3  1992/09/15  20:31:21  jaffe
# 	sync with Transarc version
# 	[92/09/15            jaffe]
# 
# Revision 1.1.2.2  1992/09/15  13:14:33  jaffe
# 	Transarc delta: kazar-ot4921-add-in-core-logging-package 1.5
# 	  Selected comments:
# 	    Start coding in-core logging package.  Not ready for export yet,
# 	    as mostly untested code.
# 	    makefile for icl kernel stuff
# 	    added missing file and cleaned up Makefile
# 	[1992/09/14  19:54:04  jaffe]
# 
# $EndLog$
#

VPATH			= ../icl

LIBRARIES		= libkicl.a

EXPDIR			= /usr/include/dcedfs/
libkicl.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=

CFLAGS			= ${KERNELCFLAGS} -D_NO_PROTO \
			  -DICL_DEFAULT_ENABLED ${DFS_CORE}

OFILES			= icl_event.o icl_log.o icl_set.o icl_control.o

INCFLAGS		= -I../icl

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
