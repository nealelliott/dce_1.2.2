#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.62.2  1996/03/11  02:28:35  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:21  marty]
#
# Revision 1.1.62.1  1995/12/11  20:14:28  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:47:17  root]
# 
# Revision 1.1.60.2  1994/09/07  15:48:47  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:41  marty]
# 
# Revision 1.1.60.1  1993/12/28  22:11:40  pellis
# 	Updated for ODE 2.3 make
# 	[1993/12/28  21:55:05  pellis]
# 
# Revision 1.1.4.3  1993/01/13  19:54:22  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:22:06  bbelch]
# 
# Revision 1.1.4.2  1992/11/02  15:37:38  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:39:43  root]
# 
# Revision 1.1.2.3  1992/08/17  18:36:04  eperkins
# 	Added dependency line for idl headers
# 	[1992/08/17  18:35:21  eperkins]
# 
# Revision 1.1.2.2  1992/05/01  13:48:18  rsalz
# 	Added as part of rpc6 drop.
# 	[1992/04/30  23:22:20  rsalz]
# 
# $EndLog$
#
#  OSF DCE Version 1.0 

PROGRAMS 	= client server
CFLAGS		= -DDEBUG ${${TARGET_MACHINE}_CFLAGS}
IDLFLAGS 	= -no_cpp -keep c_source -no_mepv 

IDLFILES	= afs.idl smpl.idl unkif.idl

ILIST		= ${PROGRAMS}

client_OFILES		= unkif_cstub.o smpl_cstub.o afs_cstub.o \
			  smplt.o afst.o afsu.o client.o 
server_OFILES		= smpl_sstub.o afs_sstub.o afs_saux.o \
			  smplm.o afsm.o afsu.o server.o 
${server_OFILES} ${client_OFILES} : ${IDLFILES:.idl=.h}

.if exists (${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>
