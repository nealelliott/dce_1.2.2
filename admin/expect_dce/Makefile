#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.3  1996/10/03  17:54:26  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  17:22:36  arvind]
#
# Revision 1.1.2.2  1996/08/21  17:08:12  arvind
# 	DCE 1.2.2 - fix build errors in expect
# 	[1996/08/21  17:07:49  arvind]
# 
# $EndLog$
#

# Generated automatically from Makefile.in by configure.
#
# Makefile for Expect
#

EXPECT_VERSION_LONG	  = \"5.19.0\"
EXPECT_VERSION		  = 5.19
TCL_VERSION		  = 7.3
ADMIN_DIR                 = ..
EXPECT_ORIG_DIR		  = ${ADMIN_DIR}/expect-${EXPECT_VERSION}
EXPECT_DIR		  = ${ADMIN_DIR}/expect_dce
EXPECT_PATH		  = ${EXPECT_DIR}/
EXPECT_LIB_DIR		  = ${EXPECT_ORIG_DIR}/library

VPATH = ${EXPECT_ORIG_DIR}:${EXPECT_LIB_DIR}:../tcl_dce:../tcl${TCL_VERSION}

# Requires at least Tcl 7.0
# Known to work with up to Tcl 7.4

# While Tk is optional, if you do use Tk, it must be at least Tk 3.3
# Known to work with up to Tk 4.0

######################################################################
# The following lines are things you may want to change
######################################################################

PROGRAMS      = expect
expect_OFILES = exp_main_exp.o

# Tcl include files.  (If you haven't installed Tcl yet, read the README file).
# This must point to the directory that contains ALL of Tcl's include
# files, not just the public ones.
TCLHDIR = -I../tcl_dce -I../tcl${TCL_VERSION}

# Tcl library.  Should be something like -ltcl or ../tcl/libtcl.a
TCLLIB = -ltcl

# flags to pass to both cc and ld
# You should be able to leave this just the way it is.  However, here are some
# note if you run into problems:
#
# Avoid -O (optimize) unless you are convinced your optimizer is flawless
# (hint: not a chance).  I have heard many reports of -O causing Expect to
# misbehave.
# I encourage you to use -g (debugging).  While it is unlikely you will
# encounter an internal error in Expect, should this happen, I may just need
# the -g information and then you will need to recompile Expect.  As an aside,
# Expect is not a space or time pig, so this won't affect the performance of
# your Expect scripts.
# Note: On Linux systems which only have dynamic X libraries, the -g prevents
# the linker from using them.  So do not use -g on such systems.
#CFLAGS += -O

# One of the following: select, poll, or simple
EVENT_TYPE = select
EVENT = exp_$(EVENT_TYPE)
 
# You will need to change EVENT_TYPE to poll if your system is:
# NCR SVR4 (1.03.01)			where select is broken on ttys
# StarServer (SVR3 and SVR4.0)		where select is broken on ttys
#
# You will need to change EVENT_TYPE to select if your system is:
# Pyramid OSx in the att universe	where poll is broken (see LIBS below)
#
# You will need to change EVENT_TYPE to simple if your system is:
# old SCO				where select is broken on ptys
# 3b2 SVR3				where poll is broken on ptys
#    If you do use "simple":
# 1) you must also edit expect_cf.h and change
#    "#undef SIMPLE_EVENT" to "#define SIMPLE_EVENT",
# 2) you cannot use Expect with Tk since Tk's event manager insists on
#    reading all events through select, and
# 3) you cannot expect or interact with two or more processes simultaneously

# Define default parameters for ptys.  This is used when 1) running in the
# background, 2) user has not defined the variable STTY_INIT to initialize
# ptys, and 3) the pty-driver's defaults suck.
#
# If your system doesn't understand "sane", try "cooked".  Apollo systems
# need nothing at all and should delete this line.  Systems using 8-bit
# character sets may need to disable parity.
# Systems that define sane to use @ as line kill and # as erase should
# use something like "sane kill  erase ".
STTY = -DDFLT_STTY=\""sane"\"

SHELL = /bin/sh
PTY_TYPE = termios
PTY = pty_$(PTY_TYPE)
MISC_OBJECTS = exp_command.o expect.o $(PTY).o exp_inter.o \
	exp_regexp.o exp_tty.o \
	exp_log.o exp_main_sub.o exp_pty.o exp_printify.o exp_trap.o \
	exp_console.o exp_strf.o exp_glob.o exp_win.o Dbg.o exp_clib.o \
	exp_closetcl.o exp_memmove.o exp_tty_comm.o util_signal.o

LIBEXPECT = libexpect.a
libexpect.a_OFILES = ${MISC_OBJECTS} ${EVENT}.o

LIBRARIES  = ${LIBEXPECT}
libexpect.a_EXPDIR = /usr/lib/

# expect must be setuid on crays in order to open ptys (and accordingly,
# you must run this Makefile as root).
# See the FAQ for more info on why this is necessary on Crays.
SETUID = :
# SETUID = chmod u+s

# allow us to handle null list gracefully, "end_of_list" should not exist
# SCRIPT_LIST = $(SCRIPTS) end_of_list
# SCRIPT_MANPAGE_LIST = $(SCRIPTS_MANPAGES) end_of_list

# Both TKHDIR and TCLHDIR include "-I"

# because STTY can include whitespace and quotes, pass STTY separately
INCFLAGS = -I. -I${TARGET_MACHINE} -I${EXPECT_ORIG_DIR}	$(TCLHDIR)

CFLAGS +=	-DEXP_VERSION=${EXPECT_VERSION_LONG} \
		-DSCRIPTDIR=\"${SCRIPTDIR}\" \
		-DEXECSCRIPTDIR=\"${EXECSCRIPTDIR}\" \
		-DTCL_DEBUGGER ${STTY}

util_signal.o_CFLAGS = ${CFLAGS} -DHAVE_SIGACTION

#
# Flags to pass to cc (i.e. add to the end of the CLFLAGS line below).
# Note that setting one will not set others automatically.  Set all that
# are relevant.
#
# NOTE THAT THESE FLAGS ARE NO LONGER SUPPORTED.  THE FUNCTIONALLY IS REPLACED
# BY THE AUTOMATIC CONFIGURATION CODE.  ONLY MESS WITH THE FOLLOWING DEFS IF
# YOU ARE POSITIVE THE AUTO CONFIG CODE IS FAILING.
# 
# -DSYSV3 if you are running SVR3 or later.
# -DSYSV4 if you are running SVR4.  This option does not preclude -DSYSV3.
# -DAUX2 if you are running Mac A/UX 2.
# -DMIPS_BSD if you are on a Mips machine using the BSD universe.
# -D_BSD_SIGNALS if you are on a Silicon Graphics AND want BSD semantics when
#	using the expect library.  Otherwise, you are better off just sticking
#	with rearming signals.

# Flags to pass to ld (i.e., add to the end of the LIBS line below).
#
# -lc -lBSD     If you are using the BSD compatibility library on an HP/UX,
#		force libc.a to be loaded first.
# -lsocket	For SCO UNIX 3.2.2 (this should now be done automatically)
#  /usr/ucblib/libucb.a is needed for solaris 2.0 after -lm
# -lX11		For Pyramid OSx, poll is broken, so use select from X lib
RIOS_LIBS = -lm
HP800_LIBS =

LIBS = $(LFLAGS) -lexpect ${TCLLIB} -ldce ${${TARGET_MACHINE}_LIBS}

## dependencies will be put after this line... ##
Dbg.o_HEADERS = Dbg.h 
exp_command.o_HEADERS = expect_cf.h exp_tty.h \
	exp_rename.h expect.h exp_command.h \
	exp_log.h exp_printify.h exp_event.h exp_pty.h 
exp_inter.o_HEADERS = expect_cf.h \
	exp_tty_in.h exp_tty.h exp_rename.h expect.h exp_command.h \
	exp_log.h exp_printify.h exp_regexp.h exp_tstamp.h
exp_log.o_HEADERS = expect_cf.h expect.h \
	exp_rename.h exp_log.h exp_printify.h 
exp_main_exp.o_HEADERS = expect_cf.h \
	expect.h exp_rename.h exp_command.h exp_log.h exp_printify.h
exp_main_sub.o_HEADERS = expect_cf.h \
	exp_rename.h \
	expect.h exp_command.h exp_tty_in.h exp_tty.h exp_log.h \
	exp_printify.h exp_event.h 
exp_main_tk.o_HEADERS = expect_cf.h Dbg.h 
exp_poll.o_HEADERS = expect_cf.h expect.h \
	exp_command.h exp_event.h 
exp_printify.o_HEADERS = expect_cf.h 
exp_pty.o_HEADERS = expect_cf.h exp_rename.h exp_pty.h 
exp_regexp.o_HEADERS = expect_cf.h \
	expect.h exp_regexp.h 
exp_select.o_HEADERS = expect_cf.h \
	expect.h exp_command.h exp_event.h 
exp_simple.o_HEADERS = expect_cf.h \
	expect.h exp_command.h exp_event.h 
exp_tk.o_HEADERS = expect_cf.h expect.h exp_command.h exp_event.h 
exp_trap.o_HEADERS = expect_cf.h expect.h \
	exp_command.h exp_log.h exp_printify.h 
exp_tty.o_HEADERS = expect_cf.h \
	expect.h exp_rename.h exp_tty_in.h exp_tty.h exp_log.h \
	exp_printify.h exp_command.h 
exp_win.o_HEADERS = exp_win.h 
expect.o_HEADERS = expect_cf.h \
	exp_rename.h expect.h exp_command.h \
	exp_log.h exp_printify.h exp_event.h exp_tty.h exp_tstamp.h
lib_exp.o_HEADERS = expect_cf.h exp_rename.h expect.h \
	exp_printify.h
pty_sgttyb.o_HEADERS = expect_cf.h exp_rename.h exp_tty_in.h \
	exp_tty.h exp_pty.h 
pty_termios.o_HEADERS = expect_cf.h exp_win.h \
	exp_tty_in.h exp_tty.h exp_rename.h exp_pty.h 
pty_unicos.o_HEADERS = expect_cf.h exp_rename.h 
util_signal.o_HEADERS = util_signal.h

# Global rules
.include <${RULES_MK}>
