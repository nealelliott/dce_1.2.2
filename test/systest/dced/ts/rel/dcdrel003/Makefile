# @OSF_COPYRIGHT@
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:38:33  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:24  marty]
#
# Revision 1.1.4.1  1995/12/11  21:55:45  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:42  root]
# 
# Revision 1.1.2.3  1994/07/26  21:41:47  fadden
# 	Modify to incorporate dcdrel003 control script
# 	[1994/07/26  19:50:55  fadden]
# 
# Revision 1.1.2.2  1994/06/27  18:26:46  baum
# 	Fixed install path and moved setup and misc.tcl files to lib directory
# 	[1994/06/27  18:26:19  baum]
# 
# Revision 1.1.2.1  1994/06/27  17:12:32  baum
# 	Changed names from 03 -> 003
# 	[1994/06/27  17:09:33  baum]
# 
# $EndLog$
# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993, 1992, 1991, 1990, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 

PROGRAMS = dcdrel003_server  dcdrel003_client

IDLFILES = dcdrel003.idl

IDLDATAFILES    = ${IDLFILES:S/.idl$/.h/g}
HFILES		= ${IDLDATAFILES}

dcdrel003_server_OFILES	= dcdrel003_server.o dcdrel003_sstub.o
dcdrel003_client_OFILES	= dcdrel003_client.o dcdrel003_cstub.o

REL003_FILES	= dcdrel003_acl.tcl dcdrel003_hd.tcl dcdrel003_ktb.tcl \
		  dcdrel003_svrconf.tcl dcdrel003_suite.sh \
		  dcdrel003

IDIR		= /test/tet/system/dced/ts/rel/

ILIST		= ${REL003_FILES} ${PROGRAMS}
DATAFILES	= ${REL003_FILES}
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

${dcdrel003_server_OFILES} ${dcdrel003_client_OFILES} : ${HFILES}
