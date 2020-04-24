# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.113.1  1996/10/17  18:24:13  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:52:18  damon]
#
# Revision 1.1.106.3  1994/02/07  16:28:12  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:26  mckeen]
# 
# Revision 1.1.106.2  1994/02/04  20:42:54  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:23:26  devsrc]
# 
# Revision 1.1.102.2  1993/12/09  21:20:42  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/09  21:14:54  jaffe]
# 
# Revision 1.1.2.8  1993/04/09  17:15:29  gmd
# 	Added ReadMe to ILIST via DATAFILES.
# 	[1993/04/08  23:35:37  gmd]
# 
# Revision 1.1.2.7  1993/01/13  21:06:43  shl
# 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
# 	  Selected comments:
# 	    To place the DFS fole exporter tests into the tree.
# 	    Add the fx sub-directory.
# 	    Add the common_data sub-directory.
# 	    Migrate the fx exerciser to the new fx interface (OT 6054)
# 	    Fix compilation problems.
# 	Transarc delta: fred-ot6239-fix-install-dirs 1.1
# 	  Selected comments:
# 	    Fix where test programs are installed.
# 	    Removed the IDIR of /bin/.
# 	[1993/01/13  13:51:10  shl]
# 
# Revision 1.1.2.6  1993/01/11  18:29:24  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:06:48  htf]
# 
# Revision 1.1.2.5  1992/11/18  21:44:41  jaffe
# 	Transarc delta: fred-itl-ofer-fixes-10-30 1.1
# 	  Selected comments:
# 	    Incorporates Ofer's changes that permit procedures to have parameters
# 	    passed to them, and also includes the vector and valList data types.  The
# 	    vector data type is needed for DFS testing.  It allows us to manipulate
# 	    binary data.
# 	    Added itl_stdtypes.h, itl_listtype.[ch] and itl_vectype.[ch].
# 	Transarc delta: jess-itl-fix-makefile 1.5
# 	  Selected comments:
# 	    This delta is not precisely named. Do't be confused that the delta
# 	    is opened for porting Sanzi Rich's sun based distributed ITL to the
# 	    rios and pmax platforms and also for some bug fixes.
# 	    To have Rich Sanzi's subdir spoke compiled.
# 	    See above reason.
# 	    Fix a parameter passing error.
# 	    There was an unavailable error when calling server_register_if.
# 	    This reopen fix the problem some way.
# 	    Chaned the definition of stat_compare function because dfs does not
# 	    care all of stat fields.
# 	[1992/11/18  14:46:27  jaffe]
# 
# Revision 1.1.2.4  1992/11/02  15:20:26  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:16:10  root]
# 
# Revision 1.1.2.3  1992/10/28  17:38:40  jaffe
# 	Transarc delta: fred-itl-add-unary-var-exists 1.2
# 	  Selected comments:
# 	    To import Ofer's fix for undefined variables.
# 	    Add the ? unary operator to check for the existence of ITL variables.
# 	    Added description of ITL_FS_TEST compile time flag.
# 	    Change the message output when a variable is missing.
# 	Transarc delta: fred-itl-dir-stat-mem-cmds 1.2
# 	  Selected comments:
# 	    Adding support for directory functions, stat function, and memory mapped
# 	    files.
# 	    Not done yet, just upgrading.
# 	    This complets this delta.
# 	    Aded the new files, itl_fileFile.c, itl_fileFile.h, itl_fileDir.c and
# 	    itl_fileDir.h
# 	Transarc delta: fred-reorg-itl-dir 1.2
# 	  Selected comments:
# 	    To re-organize the src/test/file/itl directory.  I moved all the file
# 	    specific stuff (for libitlFile.a) to the file sub-directory.
# 	    Removed all the libitlFile.a stuff.
# 	    Added file to the SUBDIRS.
# 	    Forgot to add the Makefile.
# 	Transarc delta: jdp-add-itl-stuff 1.8
# 	  Selected comments:
# 	    Add code for three new itl libraries: libitlUtils.a, libitlAggr.a, and
# 	    libitlFset.a
# 	    Build three subdirectories, one for each library named above.
# 	    Update error message in itlu_CheckReturnCode() so that the offending command
# 	    is printed.
# 	    Added documentation for libitlUtils.a, fixed some bugs, and reorganized.
# 	    Changed interface and implementation of result checking routines.
# 	    Provide routine for converting a value back to a name, assuming that
# 	    the value--name mapping was constructed using itlu_ProcessValueMap().
# 	    Checkpointing in order to import another delta.
# 	    Fix misc. compilation problems and close inorder to open another delta.
# 	    Make this stuff compile on the pmax.
# 	[1992/10/27  19:55:47  jaffe]
# 
# Revision 1.1.2.2  1992/09/25  17:13:50  jaffe
# 	Transarc delta: fred-checkin-itl 1.1
# 	  Selected comments:
# 	    Port of Interactive Test Language from Encina BDE to DCE pthreads.
# 	    Initial checkin.
# 	Transarc delta: fred-fix-itl-initialization 1.1
# 	  Selected comments:
# 	    Fix initialization code in file_test.c.  Also fixed the itl_main.c code
# 	    to reflect how other ITL based programs should initialize the ITL.
# 	    Changed the makefile to export two test scripts, that are there for
# 	    validating changes to ITL.
# 	    Added mttest1.scr and test1.scr to ILIST if building itl program.
# 	[1992/09/23  20:58:50  jaffe]
# 
# $EndLog$
#
#  Makefile for DFS ITL library.
#

PROGRAMS = itl_client

DATAFILES = ReadMe

ILIST = ${PROGRAMS} ${DATAFILES}
#
#  If ITL_BUILD_TEST is defined then we are building a generic ITL library
#  for testing new features, or to see that old stuff still works after
#  changes to ITL.
#
.ifdef ITL_BUILD_TEST
    PROGRAMS   += itl
    itl_OFILES  = itl_main.o
    itl_LIBS    = -litl ${OS_LIBS} -ldce
    ILIST      += itl mttest1.scr test1.scr testincl2.itl
.endif

LIBRARIES = libitl.a

EXPDIR = /usr/include/utils/itl/

IMODE = 0755

CCTYPE = ansi

#
#  ITL_FS_TEST -
#	Defines code specific to testing for the file systems group.
#	
CFLAGS = ${USERCFLAGS} -U_BSD -DITL_FS_TEST

#
#  Temporarily, limit the sub-directories on the SUN till all software 
#  is working there.
#
.if ${TARGET_MACHINE} == "SPARC"
    SUBDIRS = utils file spoke
.else
    SUBDIRS = utils file aggr fset spoke common_data fx bomb pioctl cm bakserver
.endif

EXPINC_SUBDIRS = ${SUBDIRS}
EXPLIB_SUBDIRS = ${SUBDIRS}

INCLUDES = itl.h itl_io.h itlio_internal.h itl_thread.h itl_exp.h       \
           itl_value.h itl_private.h itl_threadStatic.h                 \
	   cht_hash.h itl_stds.h itl_dceActions.h itl_listtype.h        \
	   itl_vectype.h itl_stdtypes.h

cht_hash.h_EXPDIR = /usr/include/utils/

libitl.a_OFILES = itl_io.o itlio_internal.o itl_thread.o itl_print.o    \
		  itl_exp.o itl_value.o itl_stdtypes.o itl_global.o     \
		  itl_server.o itl_port.o itl_command.o  itl_listtype.o \
		  itl_vectype.o
libitl.a_EXPDIR = /usr/lib/

EXPLIB_TARGETS = ${LIBRARIES:S/^/export_/g}

itl_client_OFILES = itl_client.o

itl_client_LIBS   = -ldce ${OS_LIBS}

.if ${TARGET_OS} == "SUNOS5"
itl_client_LIBS += -lsocket
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>


