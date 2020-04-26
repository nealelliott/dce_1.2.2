#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/09  23:32:15  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:47:20  marty]
#
# Revision 1.1.6.1  1995/12/08  20:55:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  20:50:34  root]
# 
# Revision 1.1.4.1  1994/06/10  20:53:21  devsrc
# 	cr10872- fix copyright
# 	[1994/06/10  16:38:08  devsrc]
# 
# Revision 1.1.2.2  1993/08/16  17:41:45  rsalz
# 	Initial release.
# 	[1993/08/16  17:34:14  rsalz]
# 
# $EndLog$
#

PROGRAMS = sams
SETUP_PROGRAMS = sams
ILIST = ${PROGRAMS}
IDIR = /bin/
LIBS =

sams_OFILES	= sams.o samsgram.o samslex.o

CFLAGS  = ${${TARGET_MACHINE}_CFLAGS}
YFLAGS = ${${TARGET_MACHINE}_YFLAGS} -d

.include <${RULES_MK}>

samsgram.c samsgram.h:	samsgram.y
	${YACC} ${YFLAGS} ${.ALLSRC}
	${MV} y.tab.c samsgram.c
	${MV} y.tab.h samsgram.h

samslex.c:	samslex.l
samslex.o:	samsgram.h
${OFILES}:	sams.h
