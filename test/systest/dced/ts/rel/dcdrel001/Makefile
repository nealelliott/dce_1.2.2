# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:38:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:15  marty]
#
# Revision 1.1.4.1  1995/12/11  21:55:14  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:20  root]
# 
# Revision 1.1.2.2  1994/08/30  21:04:18  fadden
# 	Remove linking with libdceutils explicitly- is included in libdce
# 	[1994/08/05  20:28:21  fadden]
# 
# Revision 1.1.2.1  1994/07/21  21:26:58  fadden
# 	Initial version after renaming
# 	[1994/07/21  20:37:28  fadden]
# 
# $EndLog$

IDLFILES	= dcdrel001a.idl dcdrel001b.idl
IDLINCFLAGS	= -I.
IDLFLAGS	= -keep c_source -client all -server all

VPATH		= ../../include

dcdrel001a_server_OFILES = dcdrel001a_server.o dcdrel001a_smgr.o \
			   dcdrel001a_sstub.o 
dcdrel001b_server_OFILES = dcdrel001b_server.o dcdrel001b_smgr.o \
			   dcdrel001b_sstub.o 
dcdrel001_srv_shut_OFILES = dcdrel001_srv_shut.o dcdrel001a_cstub.o \
                           dcdrel001b_cstub.o
dcdrel001_client_OFILES	= dcdrel001_client.o dcdrel001a_cstub.o \
			  dcdrel001b_cstub.o 

PROGRAMS	= dcdrel001a_server dcdrel001b_server dcdrel001_client \
		  dcdrel001_srv_shut
LIBS		= -ldcetst -ldcdtst -ldce

LIBFLAGS	= -L. -L../../lib -L../../../lib
INCDIRS	        += `genpath -I../../include`

#OPT_LEVEL	= -g
 
ILIST		= dcdrel001 dcdrel001a_server dcdrel001b_server \
		  dcdrel001_client dcdrel001_srv_shut dcdrel001_secval
IDIR		= /test/tet/system/dced/ts/rel/

.include <${RULES_MK}>

dcdrel001a_server.o:  dcdrel001a.h dcd_st_utils.h
dcdrel001b_server.o:  dcdrel001b.h dcd_st_utils.h
dcdrel001_srv_shut.o : dcdrel001a.h
dcdrel001a_smgr.o:    dcdrel001a.h dcd_st_utils.h
dcdrel001b_smgr.o:    dcdrel001b.h dcd_st_utils.h
dcdrel001_client.o:  dcdrel001a.h dcdrel001b.h dcd_st_utils.h 
dcdrel001a_cstub.c : dcdrel001a.idl dcdrel001a.acf
dcdrel001a_sstub.c : dcdrel001a.idl dcdrel001a.acf
dcdrel001a.h: dcdrel001a.idl
dcdrel001b_cstub.c : dcdrel001b.idl dcdrel001b.acf
dcdrel001b_sstub.c : dcdrel001b.idl dcdrel001b.acf
dcdrel001b.h: dcdrel001b.idl

