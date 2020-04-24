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
# Revision 1.1.200.1  1996/10/02  20:58:30  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:47:04  damon]
#
# Revision 1.1.195.6  1994/08/23  18:59:14  rsarbo
# 	delegation
# 	[1994/08/23  16:17:20  rsarbo]
# 
# Revision 1.1.195.5  1994/07/13  22:26:52  devsrc
# 	merged with bl-10
# 	[1994/06/28  21:16:30  devsrc]
# 
# 	delegation ACL support
# 	[1994/06/07  14:05:21  delgado]
# 
# Revision 1.1.195.4  1994/06/09  14:19:10  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:31:09  annie]
# 
# Revision 1.1.195.3  1994/05/20  22:03:25  sommerfeld
# 	Clean up build of testacl so as to not cause ../dacl/testacl.o to
# 	get clobbered in ClearCase
# 	[1994/05/17  20:23:11  sommerfeld]
# 
# Revision 1.1.195.2  1994/02/04  20:29:18  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:17:42  devsrc]
# 
# Revision 1.1.194.1  1993/12/07  17:33:06  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:51:41  jaffe]
# 
# Revision 1.1.8.3  1993/01/21  15:18:24  cjd
# 	embedded copyright notice
# 	[1993/01/20  15:55:50  cjd]
# 
# Revision 1.1.8.2  1992/08/31  21:12:36  jaffe
# 	Transarc delta: jaffe-fix-dfs-daily-1.35 1.1
# 	  Selected comments:
# 	    Minor fixes to get the build to complete.
# 	    Removed -lkrb5 again.
# 	[1992/08/30  12:22:20  jaffe]
# 
# Revision 1.1.2.7  1992/06/08  22:46:14  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  21:50:46  mason]
# 
# Revision 1.1.2.6  1992/05/05  19:00:56  rec
# 	Removed dependence on libkrb5.a by allowing libdce.a
# 	to export required symbol (krb5_cksumarray).
# 	[1992/05/05  18:56:30  rec]
# 
# Revision 1.1.2.5  1992/04/21  14:36:48  mason
# 	Transarc delta: bab-ot2379-dacl-build-testacl 1.2
# 	  Files modified:
# 	    security/dacl: Makefile, testacl.c
# 	    security/dacl_lfs: Makefile
# 	  Selected comments:
# 	    Get the user-space testing programs built for the dacl package.
# 	    Get testacl.o built here (w/ the Episode flags) for testacl.epi, and build
# 	    testacl and testacl.epi in this directory (since it is built second of
# 	    the two by the security Makefile, and both test programs require both
# 	    libraries).
# 	    ot 2379
# 	    Error table initialization needed to be removed from testacl.c
# 	[1992/04/20  23:02:46  mason]
# 
# Revision 1.1.2.4  1992/04/15  17:12:29  rec
# 	Changed instance of -lkrb5 to ${RIOS_LIBS} on COMMONLIBS line.
# 	Including krb5 is dependent on building on a RIOS where the
# 	krb5 symbols are not exported in libdce.
# 	[1992/04/15  16:50:26  rec]
# 
# Revision 1.1.2.3  1992/04/14  03:35:23  mason
# 	Transarc delta: bab-ot2288-dacl-source-id-size 1.4
# 	  Files modified:
# 	    security/dacl: epi_id.h; security/dacl.klib: Makefile
# 	    security/dacl2acl: Makefile; security/dacl_lfs: Makefile
# 	    security/dacl_lfs.klib: Makefile
# 	  Selected comments:
# 	    Specification of source principal id size needs to be separated from
# 	    size of stored principal ids, since they aren't always the same.
# 	    Also, should only compare as many bits in a principal id comparison
# 	    as are passed from the external source.
# 	    Make sure that the affected Makefiles set the appropriate flag to indicate
# 	    the size of the source ids.
# 	    Add -DEPI_SOURCE_IDS_ARE_32BITS to list of CFLAGS.
# 	    Missed one of the Makefiles that need this flag set on the last pass.
# 	    NB: if you import this delta, you MUST also import bab-ot2370-dacl-use-all-bits
# 	    if you want the dacl package to work in non-Episode user-space (i.e., in any
# 	    of our servers)!
# 	Transarc delta: bab-ot2379-dacl-build-testacl 1.1
# 	  Files modified:
# 	    security/dacl: Makefile; security/dacl_lfs: Makefile
# 	  Selected comments:
# 	    Get the user-space testing programs built for the dacl package.
# 	    Get testacl.o built here (w/ the Episode flags) for testacl.epi, and build
# 	    testacl and testacl.epi in this directory (since it is built second of
# 	    the two by the security Makefile, and both test programs require both
# 	    libraries).
# 	    ot 2379
# 	[1992/04/13  17:16:03  mason]
# 
# Revision 1.1.2.2  1992/01/31  16:15:33  delgado
# 	Add modifications to build libdcedfs.a
# 	[1992/01/31  16:15:04  delgado]
# 
# Revision 1.1  1992/01/19  02:53:24  devrcs
# 	Initial revision
# 
# $EndLog$
#

libdacllfs.a_OFILES     = dacl_access.o dacl_pac.o epi_id.o


.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
#
#  Information to build components when we are not build libdcedfs.a
#

VPATH			= ..:../dacl

LIBRARIES		= libdacllfs.a

# The programs are build here, since they require both libraries.
PROGRAMS		= testacl testacl.epi

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

CFLAGS 			= ${USERCFLAGS} -DEPI_USE_FULL_ID -DDACL_EPISODE \
                          -DDACL_COMPILE_FOR_EPI -DEPI_SOURCE_IDS_ARE_32BITS

INCFLAGS		= -I../dacl

OFILES			= dacl_access.o dacl_pac.o epi_id.o

# these two versions of these .o files are compiled with different flags from the one
# compiled in ../dacl
OTHERS			= testacl.o dacl_print.o

COMMONLIBS		= -lcmd -lcmdAux -lcom_err -licl -losi -lncompat -ldce ${OS_LIBS}

# for the main file, we have to go to the other directory, since it had the right
# flags for building that version
testacl_OFILES		= testacl.nonlfs.o dfssec_errorStrings.o ../dacl/libdacl.a libdacllfs.a

testacl_LIBS		= ${COMMONLIBS}

# dacl_print.o must be before ../libdacl.a, since it is redefining some things
testacl.epi_OFILES	= testacl.o dacl_print.o dfssec_errorStrings.o ../dacl/libdacl.a libdacllfs.a

testacl.epi_LIBS	= -lafssys ${COMMONLIBS}


.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
.endif

# do *not* put dependancy on ../dacl/testacl.o here; it confuses
# clearcase.

testacl.nonlfs.o:
	cp ../dacl/testacl.o testacl.nonlfs.o


