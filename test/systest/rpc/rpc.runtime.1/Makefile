# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.12.2  1996/03/11  02:46:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:12  marty]
#
# Revision 1.1.12.1  1995/12/11  22:43:14  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1994/10/11  20:56 UTC  bissen
# 	Merge Hpdce02_04 to HPDCE02 branch
# 	[1995/12/11  22:20:15  root]
# 
# Revision 1.1.10.1  1994/09/07  15:48:48  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:42  marty]
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  19:29 UTC  bissen
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/2  1994/03/30  16:08 UTC  truitt
# 
# 	HP revision /main/HPDCE01/truitt_junk/1  1994/03/30  15:58 UTC  truitt
# 	Modify to support changes to dcerpcrun
# 
# 	HP revision /main/HPDCE01/1  1994/02/01  14:18  giza
# 	merge kk & hpdce01
# 
# Revision 1.1.4.4  1993/09/02  20:30:17  giza
# 	Fixed IDIR defs.
# 	[1993/09/02  14:12:57  giza]
# 
# Revision 1.1.4.3  1993/08/27  21:03:59  giza
# 	fixed IDIRs for TET
# 	[1993/08/27  21:03:31  giza]
# 
# Revision 1.1.4.2  1993/08/24  18:40:33  giza
# 	New file dependencies added.
# 	[1993/08/24  18:39:47  giza]
# 
# 	Embedded copyright notice
# 	[1993/02/04  22:10:16  cjd]
# 
# Revision 1.1.8.1  1993/09/09  19:02:33  cmckeen
# 	HP's TET'ized version
# 	[1993/09/09  18:57:45  cmckeen]
# 
# Revision 1.1.6.2  1993/08/24  14:05:45  giza
# 	Added new file dependencies.
# 	[1993/08/24  14:05:06  giza]
# 
# Revision 1.1.1.5  1993/05/04  22:21:51  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.2.4  1993/03/26  18:09:07  eperkins
# 	Added PMAX_CFLAGS=-pedantic and ILIST and SCRIPTS
# 	[1993/03/26  18:08:38  eperkins]
# 
# Revision 1.1.2.3  1993/02/05  15:21:42  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:42:03  cjd]
# 
# Revision 1.1.2.2  1992/09/25  19:01:05  eperkins
# 	Initial version
# 	[1992/09/25  19:00:40  eperkins]
# 
# $EndLog$
#
#
dcerpcrun_IDIR            = /tet/rpc/ts/rpc.runtime.1/
dcerpcrun.data_IDIR       = /tet/rpc/ts/rpc.runtime.1/
dcerpcrun.functions_IDIR  = /tet/rpc/ts/rpc.runtime.1/
clientwait_IDIR           = /tet/rpc/ts/rpc.runtime.1/
clientwrapper_IDIR        = /tet/rpc/ts/rpc.runtime.1/
client_IDIR               = /tet/rpc/ts/rpc.runtime.1/
server_IDIR               = /tet/rpc/ts/rpc.runtime.1/
dumpDB_IDIR               = /tet/rpc/ts/rpc.runtime.1/

LIBRARIES	= librpctest.a
OFILES          = util.o
EXPDIR          = /usr/lib/
LIBFLAGS        = -L.

IDLBASE 	= pipetest
IDLFILES        = pipetest.idl

INCFLAGS 	= -I.

PMAX_CFLAGS	= -pedantic

CFLAGS          =  ${${TARGET_MACHINE}_CFLAGS}

LIBS            = -lrpctest -ldce ${${TARGET_MACHINE}_LIBS}

PROGRAMS        = client server dumpDB
SCRIPTS		= dcerpcrun dcerpcrun.data dcerpcrun.functions clientwait clientwrapper
ILIST		= ${PROGRAMS} ${SCRIPTS}

OTHER_CLIENT_FILES = client_aux.o
CSTUB           = ${IDLBASE}_cstub.o
SSTUB           = ${IDLBASE}_sstub.o
SAUX            = ${IDLBASE}_saux.o

MEPV            ?= -no_mepv
IDLKEEP         ?= c_source
IDLFLAGS        += -no_cpp -keep ${IDLKEEP} ${MEPV}  ${IDL_CC_OPT}

dumpDB_OFILES   =  ${CSTUB} dumpDB.o 
client_OFILES   =  ${CSTUB} ${OTHER_CLIENT_FILES} client.o 
server_OFILES   =  ${SSTUB} ${SAUX} manager.o server.o

SAUX = ${IDLBASE}_saux.o


.if (${USE_SHARED_LIBRARIES} && ${TARGET_MACHINE} == "PMAX")
IDL_CC_OPT      = -cc_opt "${CFLAGS} ${_CC_OL_} -pic-lib -L/usr/shlib -ldce"
.else
IDL_CC_OPT      = -cc_opt "${CFLAGS} ${_CC_OL_}"
.endif

.include <${RULES_MK}>

#.if ( ${IDLKEEP} == "both" || ${IDLKEEP} == "object" )
#${CSTUB} ${SSTUB} ${CAUX} ${SAUX}: ${IDLBASE}.h

#${IDLBASE}.h :  ${IDLBASE}.idl
#        ${IDL} ${${IDLBASE:=.idl}:P} ${_IDLFLAGS_} ${IDL_CC}
#.endif

${server_OFILES} ${client_OFILES} ${dumpDB_OFILES} : ${IDLFILES:.idl=.h}
