#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.112.1  1996/10/17  18:25:37  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:52:53  damon]
#
# Revision 1.1.107.3  1994/02/07  16:28:17  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:30  mckeen]
# 
# Revision 1.1.107.2  1994/02/04  20:43:38  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:23:44  devsrc]
# 
# Revision 1.1.105.1  1993/12/07  17:43:32  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:10:23  jaffe]
# 
# Revision 1.1.2.3  1993/01/11  18:29:38  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:07:17  htf]
# 
# Revision 1.1.2.2  1992/10/28  17:39:13  jaffe
# 	Transarc delta: fred-itl-add-fcntl-flock 1.3
# 	  Selected comments:
# 	    To add fcntl and flock to the ITL file stuff.
# 	    Also will fix some minor bugs, and convert to use Jeff Prem's utilities.
# 	    This is a checkpoint.  I need to import a delta from Jeff.
# 	    Complete the integration of Jeff's utility stuff and finish up work on
# 	    fcntl and flock.
# 	    Added the files, itl_fileFlock.c and itl_fileFlock.h
# 	    Forgot to add the files that implement the file locking stuff. What
# 	    a dufus.
# 	Transarc delta: fred-itl-make-open-like-c 1.3
# 	  Selected comments:
# 	    Fix ITL system calls to have more C like syntax.  Previously only a
# 	    single open or opendir could be issued against a file resource, since the
# 	    filename/directory name being opened was hashed.  Now the names are no longer
# 	    hashed and the resulting file descriptor (or DIR *) is placed in a
# 	    user supplied variable.  This changes the syntax of the open, creat,
# 	    opendir and mmap commands.  They now all require a variable name as the
# 	    last argument to the command.
# 	    Also a number of commands that previously took a filename or directory
# 	    name, now take the actual file descriptor or DIR *, as was placed in a
# 	    variable by the open and opendir comands.
# 	    Removed itl_fileFile.[ch], since filenames are no longer hashed.
# 	    Make the necessary changes to the fset ("ftutil") tests to match the
# 	    described above.
# 	    Cleanup debugging code.
# 	Transarc delta: fred-itl-move-hashing-to-utils 1.1
# 	  Selected comments:
# 	    Moved the name hashing stuff from the itl/file sub-dir to itl/utils
# 	    sub-dir so it is accessible to anyone writing ITL C code.
# 	    Removed itlFile_Hash.o and itlFile_Hash.h.
# 	Transarc delta: fred-reorg-itl-dir 1.2
# 	  Selected comments:
# 	    To re-organize the src/test/file/itl directory.  I moved all the file
# 	    specific stuff (for libitlFile.a) to the file sub-directory.
# 	    Forgot to add the Makefile.
# 	    Has all the libitlFile.a stuff.
# 	Transarc delta: jess-itl-add-statfs 1.1
# 	  Selected comments:
# 	    Add statfs function call to the itl file interface.
# 	    Change it so that itl_fileStatfs.[ch] can be compiled and linked to itlFileLib.a.
# 	[1992/10/27  19:58:25  jaffe]
# 
# $EndLog$
#
#  Makefile for DFS ITL file library.
#

LIBRARIES = libitlFile.a

EXPDIR = /usr/include/utils/itl/

CCTYPE = ansi

CFLAGS = ${USERCFLAGS} -U_BSD -DITL_FS_TEST

INCLUDES = itl_file.h itl_fileUtils.h                                   \
	   itl_fileDir.h itl_fileStat.h itl_fileFlock.h                 \
           itl_fileStatfs.h

libitlFile.a_OFILES = itl_file.o itl_fileUtils.o                        \
                      itl_fileDir.o itl_fileStat.o itl_fileFlock.o      \
		      itl_fileStatfs.o
libitlFile.a_EXPDIR = /usr/lib/

EXPLIB_TARGETS = ${LIBRARIES:S/^/export_/g}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>


