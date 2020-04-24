# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993, 1992, 1991, 1990, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:38:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:21  marty]
#
# Revision 1.1.4.1  1995/12/11  21:55:37  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:36  root]
# 
# Revision 1.1.2.3  1994/07/27  19:26:27  fadden
# 	Add the control script
# 	[1994/07/26  23:54:20  fadden]
# 
# Revision 1.1.2.2  1994/06/27  18:26:44  baum
# 	Fixed install path and moved setup and misc.tcl files to lib directory
# 	[1994/06/27  18:26:17  baum]
# 
# Revision 1.1.2.1  1994/06/27  17:15:00  baum
# 	Initial checkin
# 	[1994/06/27  17:14:27  baum]
# 
# $EndLog$

PROGRAMS = dcdrel002_server  dcdrel002_client

IDLFILES = dcdrel002.idl

IDLDATAFILES    = ${IDLFILES:S/.idl$/.h/g}
HFILES		= ${IDLDATAFILES}

dcdrel002_server_OFILES	= dcdrel002_server.o dcdrel002_sstub.o
dcdrel002_client_OFILES	= dcdrel002_client.o dcdrel002_cstub.o

REL002_FILES	= dcdrel002.tcl dcdrel002

IDIR		= /test/tet/system/dced/ts/rel/
ILIST		= ${REL002_FILES} ${PROGRAMS}
DATAFILES	= ${REL002_FILES}
IMODE		= 755

IDLINCFLAGS	= -I.
IDLFLAGS	= -cepv -keep c_source
CFLAGS          += ${STRICT_COMPILER_WARNINGS}
INCFLAGS        = -I.
LIBS            = -ldce


.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

${dcdrel002_server_OFILES} ${dcdrel002_client_OFILES} : ${HFILES}
