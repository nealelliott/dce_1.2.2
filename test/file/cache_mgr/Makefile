#
#  @OSF_COPYRIGHT@
#  COPYRIGHT NOTICE
#  Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
#  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
#  src directory for the full copyright text.
#
#  Makefile --
#	For the test/file/cache_mgr directory.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.108.1  1996/10/17  18:17:28  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:49:42  damon]
#
# Revision 1.1.103.3  1994/02/07  16:28:04  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:19  mckeen]
# 
# Revision 1.1.103.2  1994/02/04  20:39:55  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:22:16  devsrc]
# 
# Revision 1.1.101.1  1993/12/07  17:41:25  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  18:39:36  jaffe]
# 
# Revision 1.1.2.5  1993/04/09  17:15:13  gmd
# 	Added README to ILIST.
# 	Removed unnecessary IDIR.
# 	[1993/04/08  23:14:11  gmd]
# 
# 	Added README as to DATAFILES for ILIST so it is also
# 	included in test install tree.
# 	[1993/04/06  20:40:13  gmd]
# 
# Revision 1.1.2.4  1993/01/11  18:24:58  htf
# 	Embedded copyright notice
# 	[1993/01/11  16:56:09  htf]
# 
# Revision 1.1.2.3  1992/11/04  19:21:36  jaffe
# 	Transarc delta: fred-cc-fix-socket-port 1.3
# 	  Selected comments:
# 	    Additional fixes were necessary.  A couple calls to htons were added and
# 	    also normalized the optn() flags.  There values are different on AIX and
# 	    OSF/1.
# 	    Added openFlags.h file.
# 	    Forgot to include new .h file.
# 	[1992/11/04  17:33:26  jaffe]
# 
# Revision 1.1.2.2  1992/10/28  16:00:52  jaffe
# 	Selected comments:
# 	  Added Mike Kazar's cache consistency tests to the test tree.
# 	  Forgot to put a EndLog marker in the header comment.
# 	[1992/10/27  16:56:33  jaffe]
# 
# $EndLog$

PROGRAMS = hub spoke
DATAFILES = README

LIBRARIES = libcom.a

ILIST = ${PROGRAMS} ${DATAFILES}

CCTYPE = ansi
CARGS = -g

COMMONINCLS = com.h queue.h stub.h openFlags.h
HUBINCLS    = hub.h $(COMMONINCLS)
SPOKEINCLS  = spoke.h $(COMMONINCLS)

.if exists (${TARGET_OS}/machdep.mk)
.include "${TARGET_OS}/machdep.mk"
.endif

hub_OFILES = hub.o hub_subr.o hub_tests.o hub_parse.o client_stub.o    \
	     data_stub.o 
hub_LIBS   = libcom.a ${OTHER_LIBS}

spoke_OFILES = spoke.o data_stub.o server_stub.o
spoke_LIBS   = libcom.a ${OTHER_LIBS}

spoke: spoke.o libcom.a data_stub.o server_stub.o

libcom.a_OFILES = com.o client.o

com.o_HFILES         = ${COMMONINCLS}
client.o_HFILES      = ${COMMONINCLS}
spoke.o_HFILES       = ${SPOKEINCLS}
client_stub.o_HFILES = ${COMMONINCLS}
server_stub.o_HFILES = ${COMMONINCLS}
data_stub.o_HFILES   = ${COMMONINCLS}
hub_subr.o_HFILES    = ${HUBINCLS}
hub_tests.o_HFILES   = ${HUBINCLS}
hub.o_HFILES         = ${HUBINCLS}
hub_parse.o_HFILES   = ${HUBINCLS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>
