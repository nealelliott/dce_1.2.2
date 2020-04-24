# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.532.1  1996/10/02  21:01:05  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:48:03  damon]
#
# Revision 1.1.527.2  1994/06/09  14:20:22  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:31:53  annie]
# 
# Revision 1.1.527.1  1994/02/04  20:30:09  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:18:04  devsrc]
# 
# Revision 1.1.525.1  1993/12/07  17:33:45  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:58:22  jaffe]
# 
# Revision 1.1.4.3  1993/01/21  15:20:50  cjd
# 	embedded copyright notice
# 	[1993/01/20  16:00:01  cjd]
# 
# Revision 1.1.4.2  1992/08/31  21:14:56  jaffe
# 	Transarc delta: bab-ot2978-tkm-multi-thread-tkm_test 1.16
# 	  Selected comments:
# 	    This version of this delta has the real per-host threads and work queues
# 	    in it.  This is another check-point before restoring the use of
# 	    the hostops vectors in the testing code.
# 	    Hostops vectors now used in user-space test.  Added more support
# 	    for tkset/tkc testing.
# 	    Checkpoint before importing the real tkc/tkset code.
# 	    Pick up changes to tkm_test.y that hadn't been saved.
# 	    Buildable check-point that includes tkm-host host auditing.
# 	    Build tkc in user-space for debugging & testing.
# 	    last check-point before pulling tkm_test out of tkm directory.
# 	    This one seems to be passing all the basic tkm/tkc/tkset tests.
# 	    Moved testing program into token_test directory.
# 	    Fix bug in tkc host handling.
# 	    Add support for host yield operation.
# 	    Support for printing strings during execution and attempt to get
# 	    thread numbers.
# 	    Compensate for error in build 11 version of pthread.h
# 	    tkc change to get desired interleaving in user-space tests.
# 	    Locate the fix for pthread_getunique_np in a place where more
# 	    of the test program can use it, and protected with ifdefs for RIOS.
# 	    Correct these libraries' misconception that they are to be compiled
# 	    by the kernel compiler on OSF/1.
# 	    Removed "KERNEL_CC =" line.
# 	    tk_test did not link with new configuration.
# 	[1992/08/30  12:24:32  jaffe]
# 
# Revision 1.1.2.2  1992/05/20  20:56:59  mason
# 	Transarc delta: bab-ot2978-tkm-multi-thread-tkm_test 1.14
# 	  Files modified:
# 	    OSF1: Makefile; libafs/RIOS: Makefile; tkc: tkc.c, tkc.h
# 	    tkc.ulib: Makefile
# 	    tkm: Makefile, tkm_blockingOps.c, tkm_fidHash.c
# 	    tkm: tkm_getToken.c, tkm_hostData.h, tkm_hostList.h
# 	    tkm: tkm_hostStubs.c, tkm_hostStubs.h, tkm_test.h
# 	    tkm: tkm_test.l, tkm_test.y, tkm_testops.c, tkm_tkcHost.c
# 	    tkm: tkm_tkcsetHost.c, tkm_tkmHost.c, tkm_tksetHost.c
# 	    tkm: tkm_tokens.c, tkm_tokens_private.h
# 	    tkset: Makefile, tkset.h; tkset.ulib: Makefile
# 	    token_test: Makefile, tktest_hostStubs.c, tktest_hostStubs.h
# 	    token_test: tktest_test.h, tktest_test.l, tktest_test.y
# 	    token_test: tktest_testops.c, tktest_tkcsetHost.c
# 	    token_test: tktest_tkmHost.c
# 	  Selected comments:
# 	    This delta implements the use of the thread-per-host model in tkm_test.
# 	    This first version is a check-point that just implements the use of
# 	    host operation structures (to be used as work queue elements).
# 	    This version of this delta has the real per-host threads and work queues
# 	    in it.  This is another check-point before restoring the use of
# 	    the hostops vectors in the testing code.
# 	    Hostops vectors now used in user-space test.  Added more support
# 	    for tkset/tkc testing.
# 	    Checkpoint before importing the real tkc/tkset code.
# 	    Pick up changes to tkm_test.y that hadn't been saved.
# 	    Buildable check-point that includes tkm-host host auditing.
# 	    Build tkc in user-space for debugging & testing.
# 	    last check-point before pulling tkm_test out of tkm directory.
# 	    This one seems to be passing all the basic tkm/tkc/tkset tests.
# 	    Moved testing program into token_test directory.
# 	    Fix bug in tkc host handling.
# 	    Add support for host yield operation.
# 	    Support for printing strings during execution and attempt to get
# 	    thread numbers.
# 	    Compensate for error in build 11 version of pthread.h
# 	    tkc change to get desired interleaving in user-space tests.
# 	    Locate the fix for pthread_getunique_np in a place where more
# 	    of the test program can use it, and protected with ifdefs for RIOS.
# 
# 	# Transarc delta: bab-ot2978-tkm-multi-thread-tkm_test 1.14
# 	#   Files modified:
# 	#     OSF1: Makefile; libafs/RIOS: Makefile; tkc: tkc.c, tkc.h
# 	#     tkc.ulib: Makefile
# 	#     tkm: Makefile, tkm_blockingOps.c, tkm_fidHash.c
# 	#     tkm: tkm_getToken.c, tkm_hostData.h, tkm_hostList.h
# 	#     tkm: tkm_hostStubs.c, tkm_hostStubs.h, tkm_test.h
# 	#     tkm: tkm_test.l, tkm_test.y, tkm_testops.c, tkm_tkcHost.c
# 	#     tkm: tkm_tkcsetHost.c, tkm_tkmHost.c, tkm_tksetHost.c
# 	#     tkm: tkm_tokens.c, tkm_tokens_private.h
# 	#     tkset: Makefile, tkset.h; tkset.ulib: Makefile
# 	#     token_test: Makefile, tktest_hostStubs.c, tktest_hostStubs.h
# 	#     token_test: tktest_test.h, tktest_test.l, tktest_test.y
# 	#     token_test: tktest_testops.c, tktest_tkcsetHost.c
# 	#     token_test: tktest_tkmHost.c
# 	#   Selected comments:
# 	#     This delta implements the use of the thread-per-host model in tkm_test.
# 	#     This first version is a check-point that just implements the use of
# 	#     host operation structures (to be used as work queue elements).
# 	#     This version of this delta has the real per-host threads and work queues
# 	#     in it.  This is another check-point before restoring the use of
# 	#     the hostops vectors in the testing code.
# 	#     Hostops vectors now used in user-space test.  Added more support
# 	#     for tkset/tkc testing.
# 	#     Checkpoint before importing the real tkc/tkset code.
# 	#     Pick up changes to tkm_test.y that hadn't been saved.
# 	#     Buildable check-point that includes tkm-host host auditing.
# 	#     Build tkc in user-space for debugging & testing.
# 	#     last check-point before pulling tkm_test out of tkm directory.
# 	#     This one seems to be passing all the basic tkm/tkc/tkset tests.
# 	#     Moved testing program into token_test directory.
# 	#     Fix bug in tkc host handling.
# 	#     Add support for host yield operation.
# 	#     Support for printing strings during execution and attempt to get
# 
# $EndLog$
#
# (C) Copyright 1992 Transarc Corporation.  All Rights Reserved.
#
VPATH			= ../tkc

LIBRARIES		= libtkc.a

EXPDIR			= /usr/lib/

EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

CFLAGS			= ${USERCFLAGS}

libtkc.a_OFILES		= tkc.o tkc_alloc.o tkc_cache.o tkc_hostops.o \
			  tkc_locks.o tkc_revoke.o tkc_tokens.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
