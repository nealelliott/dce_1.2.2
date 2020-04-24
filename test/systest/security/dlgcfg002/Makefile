# @OSF_COPYRIGHT@
# Copyright (c) 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:47:51  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:55  marty]
#
# Revision 1.1.4.1  1995/12/11  22:44:57  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:21:21  root]
# 
# Revision 1.1.2.3  1994/10/18  18:45:02  agd
# 	expand copyright
# 	[1994/10/18  17:39:25  agd]
# 
# 	expand copyright
# 
# Revision 1.1.2.2  1994/09/30  18:55:21  bhaim
# 	add README to Makefile
# 	[1994/09/30  18:54:55  bhaim]
# 
# Revision 1.1.2.1  1994/09/29  21:02:42  bhaim
# 	submit dlgcfg002 test
# 	[1994/09/29  21:01:23  bhaim]
# 
# $EndLog$

#INCENV         = -I. -I/usr/include/reentrant
CFLAGS          = ${${TARGET_MACHINE}_CFLAGS}

#ANSI_FLAGS     = -Aa -D_HPUX_SOURCE
#HP_FLAGS       = -D_REENTRANT
#LIBS           = -lbb -ldce -lm -lc_r
#LIBS           = -ldce
LIBS            = -ldce




IDLINCFLAGS     = -I.
IDLFLAGS        = -keep c_source

IDLFILES        = dts_test.idl 

HFILES  = ${IDLFILES} ${IDLOBJS} dts_macros.h

IDLOBJ          = ${IDLFILES:.idl=.h}
OFILES          = ${IDLFILES:.idl=_cstub.o} ${IDLFILES:.idl=_sstub.o}

# Programs to build
#OTHERS  = README
SCRIPTS = dts_rgy_setup.sh dts_acl_setup.sh 

PROGRAMS        = dts_client dts_intermediary

ILIST = ${PROGRAMS} ${SCRIPTS} README
IDIR = /test/tet/system/security/ts/dlgcfg002/

dts_client_OFILES       = dts_test_cstub.o \
                          dlg_util.o \
                          dts_client.o
dts_intermediary_OFILES = dts_test_sstub.o \
                          dlg_util.o \
                          dts_intermediary.o

.include <${RULES_MK}>

