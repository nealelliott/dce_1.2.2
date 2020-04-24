# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:48:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:59  marty]
#
# Revision 1.1.4.1  1995/12/11  22:45:15  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:21:29  root]
# 
# Revision 1.1.2.2  1994/10/06  20:39:17  agd
# 	expand copyright
# 	[1994/10/06  14:31:00  agd]
# 
# Revision 1.1.2.1  1994/09/19  18:27:22  bhaim
# 	multidelegate test
# 	[1994/09/19  18:26:58  bhaim]
# 
# $EndLog$

#
# (c) Copyright 1992, 1993, 1994 Hewlett-Packard Co.
#


#INCENV		= -I. -I/usr/include/reentrant
CFLAGS          = ${${TARGET_MACHINE}_CFLAGS} 

#ANSI_FLAGS	= -Aa -D_HPUX_SOURCE
#HP_FLAGS	= -D_REENTRANT
#LIBS		= -lbb -ldce -lm -lc_r
#LIBS		= -ldce 
LIBS            = -ldce




IDLINCFLAGS	= -I.
IDLFLAGS	= -keep c_source

IDLFILES	= string_conv.idl string_conv2.idl string_conv3.idl \
		  string_conv4.idl string_conv5.idl string_conv6.idl

HFILES	= ${IDLFILES} ${IDLOBJS}

IDLOBJ		= ${IDLFILES:.idl=.h} 
OFILES 		= ${IDLFILES:.idl=_cstub.o} ${IDLFILES:.idl=_sstub.o}

# Programs to build
OTHERS  = README
SCRIPTS = cds_entry.ksh rgy_acct.ksh acct_delete_osf.ksh 

PROGRAMS	= conv_server conv_server2 conv_server3 conv_server4 \
		  conv_server5 conv_server6 conv_client


ILIST = ${PROGRAMS} ${SCRIPTS} ${OTHERS}
IDIR = /test/tet/system/security/ts/dlgstr001/

conv_client_OFILES = string_conv_cstub.o print_attrs.o client.o

conv_server_OFILES = string_conv_sstub.o string_conv2_cstub.o \
		     string_conv2_sstub.o manager.o print_attrs.o  \
		     server.o identity.o

conv_server2_OFILES = string_conv2_sstub.o string_conv3_cstub.o \
		      string_conv3_sstub.o manager2.o print_attrs.o  \
		      server2.o identity.o

conv_server3_OFILES = string_conv3_sstub.o string_conv4_cstub.o \
		      string_conv4_sstub.o manager3.o print_attrs.o  \
		      server3.o identity.o

conv_server4_OFILES = string_conv4_sstub.o string_conv5_cstub.o \
		      string_conv5_sstub.o manager4.o print_attrs.o  \
		      server4.o identity.o

conv_server5_OFILES = string_conv5_sstub.o string_conv6_cstub.o \
		      string_conv6_sstub.o manager5.o print_attrs.o  \
		      server5.o identity.o

conv_server6_OFILES = string_conv6_sstub.o manager6.o print_attrs.o  \
		      server6.o identity.o

.include <${RULES_MK}>
