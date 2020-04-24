# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:27:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:21  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:10  root
# 	Submit
# 	[1995/12/11  14:32:20  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:19  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:17  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:23  melman]
# 
# Revision 1.1.2.5  1994/09/27  19:40:44  rousseau
# 	Add DATAFILES line
# 	[1994/09/27  19:40:31  rousseau]
# 
# Revision 1.1.2.4  1994/09/20  18:22:24  pwang
# 	Merged with changes from 1.1.2.3
# 	[1994/09/20  18:22:02  pwang]
# 
# 	Added README [OT#10262]
# 	[1994/09/20  18:07:41  pwang]
# 
# Revision 1.1.2.3  1994/06/17  18:44:05  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:29:48  devsrc]
# 
# Revision 1.1.2.2  1994/06/11  21:50:41  pwang
# 	Added HFILES dependency
# 	[1994/06/11  21:49:43  pwang]
# 
# Revision 1.1.2.1  1994/06/10  21:40:29  pwang
# 	Initial Implementation
# 	[1994/06/10  21:23:25  pwang]
# 
# $EndLog$

PROGRAMS	= tserver tclient
IDLFILES	= tserver.idl
IDLDATAFILES    = ${IDLFILES:S/.idl$/.h/g}

SERVER_FILES	= server_catalog_N.tcl server_catalog_P.tcl	\
		  server_create_N.tcl server_create_P.tcl	\
		  server_delete_N.tcl server_delete_P.tcl	\
		  server_disable_N.tcl server_disable_P.tcl	\
		  server_enable_N.tcl server_enable_P.tcl	\
		  server_modify_N.tcl server_modify_P.tcl	\
		  server_ping_N.tcl server_ping_P.tcl		\
		  server_show_N.tcl server_show_P.tcl		\
		  server_start_N.tcl server_start_P.tcl		\
		  server_stop_N.tcl server_stop_P.tcl		\
		  server_setup.tcl server_cleanup.tcl

SERVER_DATA	= server_setup.sh  server_cleanup.sh

IDIR		= /test/tet/functional/admin/dcecp/ts/server/
ILIST		= ${PROGRAMS} ${SERVER_FILES} ${SERVER_DATA}
DATAFILES	= ${SERVER_FILES} ${SERVER_DATA}
IMODE		= 755

tserver_OFILES	= tserver.o tserver_sstub.o
tclient_OFILES	= tclient.o tserver_cstub.o

HFILES		= ${IDLDATAFILES}

IDLINCFLAGS	= -I.
IDLFLAGS	= -cepv -keep c_source
CFLAGS          += ${STRICT_COMPILER_WARNINGS}
INCFLAGS        = -I.
LIBS            = -ldce

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

${tserver_OFILES} ${tclient_OFILES} : ${HFILES}
