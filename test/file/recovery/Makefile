#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.8.1  1996/10/17  18:33:46  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:56:46  damon]
#
# Revision 1.1.3.1  1994/07/13  22:31:29  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:14:56  devsrc]
# 
# 	moving files from src/file/episode to test/file
# 	[1994/03/22  20:40:08  delgado]
# 
# Revision 1.1.14.1  1994/02/04  20:14:22  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:55  devsrc]
# 
# Revision 1.1.12.2  1994/01/20  18:43:00  annie
# 	added copyright header
# 	[1994/01/20  18:39:15  annie]
# 
# Revision 1.1.12.1  1993/12/07  17:19:57  jaffe
# 	New File from Transarc 1.0.3a
# 	[1993/12/02  20:32:31  jaffe]
# 
# $EndLog$
#

# Copyright (C) 1993, 1992, 1991 Transarc Corporation - All rights reserved

PROGRAMS 	= copyaggr checksum

LIBS		= -lafssys -ldce ${OS_LIBS}

SCRIPTS 	= rcx runrcx \
	          all_tests all_simple_tests stress_tests 8k_1ktests

DATAFILES	= README \
		  basictest.rs bigsimple.rs \
                  simple01.rs simple02.rs simple03.rs simple04.rs \
		  simple05.rs simple06.rs simple07.rs simple08.rs \
	          simple09.rs simple10.rs simple11.rs simple12.rs \
	          simple14.rs simple15.rs simple16.rs

ILIST   = ${PROGRAMS} ${DATAFILES} ${SCRIPTS}

copyaggr_OFILES	= copyaggr.o crc.o 

checksum_OFILES	= checksum.o crc.o

checksum_LIBS   = ${OS_LIBS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

.SUFFIXES: .pl

.pl:
	${SED} -e '1s;^#!;&;' -e t\
	       -e 's;^#\(.*\)\@SOURCEWARNING\@;\1${NASF};' -e t\
	       ${${.TARGET}_SED_OPTIONS:U${SED_OPTIONS}}\
        ${.IMPSRC} > ${.TARGET}.X
	${CHMOD} +x ${.TARGET}.X
	${MV} -f ${.TARGET}.X ${.TARGET}
