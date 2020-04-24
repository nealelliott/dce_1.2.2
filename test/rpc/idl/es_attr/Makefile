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
# Revision 1.1.4.2  1996/03/11  02:13:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:53:02  marty]
#
# Revision 1.1.4.1  1995/12/11  20:04:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:31:03  root]
# 
# Revision 1.1.2.1  1994/05/10  18:03:27  tom
# 	Test for ecoding services get/set attribute.
# 	[1994/05/10  16:02:46  tom]
# 
# $EndLog$
#

VPATH		= ..
INCFLAGS	= -I..
IDLINCFLAGS	= -I. ${INCFLAGS}
LIBFLAGS	= -L..

# Programs to build
PROGRAMS	= client server
DATAFILES	= RUN_ARGS

client_OFILES	= client.o es_attr_encode_cstub.o es_attr_decode_cstub.o
server_OFILES	= server.o

IDLFILES	= es_attr_encode.idl es_attr_decode.idl
IDLFLAGS	= -keep c_source -no_mepv

HFILES		= es_attr_encode.h es_attr_decode.h
LIBS            = -lidltest -ldce ${${TARGET_MACHINE}_LIBS}

ILIST		= ${PROGRAMS} ${DATAFILES}

.include <${RULES_MK}>
