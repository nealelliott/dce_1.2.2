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
# Revision 1.1.15.1  1996/10/02  17:45:28  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:36:38  damon]
#
# Revision 1.1.9.1  1994/06/09  14:06:28  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:21:53  annie]
# 
# Revision 1.1.7.2  1993/06/01  21:33:31  jaffe
# 	Ot 8082: Fix AIX compiler problems
# 	[1993/06/01  21:31:11  jaffe]
# 
# Revision 1.1.5.3  1993/01/21  19:32:34  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:46:21  zeliff]
# 
# Revision 1.1.5.2  1992/11/24  17:01:13  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  17:55:58  bolinger]
# 
# Revision 1.1.2.2  1992/05/22  20:49:01  garyf
# 	add -I to pick up include files
# 	[1992/05/22  03:07:44  garyf]
# 
# Revision 1.1  1992/01/19  02:41:05  devrcs
# 	Initial revision
# 
# $EndLog$
#

VPATH 			= ${TARGET_MACHINE}

INCLUDES		= export.h 

OTHERS			= export.ext
LIBRARIES		= libaixexport.a
PROGRAMS		= cfgexport

EXPDIR			= /usr/lib/
export.h_EXPDIR		= /usr/include/dcedfs/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

CFLAGS			= ${KERNELCFLAGS}
cfgexport.o_CFLAGS	= ${USERCFLAGS}
INCFLAGS		= -I${TARGET_MACHINE}
libaixexport.a_OFILES	= misc.o

export.ext_OFILES	= export.o symtab.o
export.ext_LIBS		= -lcsys
export.ext_LDFLAGS	= -H8 -eexport /lib/kernex.exp /lib/syscalls.exp \
			  -bexport:export.exp

cfgexport_LIBS		= -lafssys

ILIST			= export.ext ${PROGRAMS}
IDIR			= /ext/

GARBAGE			= export.exp export.ext.ldmap

export.ext: ${export.ext_OFILES} export.exp
	-${CP} ${export.exp:P} .
	${_LD_} -o $@ ${$@_LDFLAGS} ${$@_OFILES} ${$@_LIBS} \
		-bloadmap:export.ext.ldmap
	${RM} -f export.exp

misc.o: misc.s
	-${CP} ${misc.s:P} .
	${AS} -o misc.o misc.s
	${RM} -f misc.s
