#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.2  1996/11/13  20:29:26  arvind
# 	CAPI drop from DEC
# 	[1996/10/16  20:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
#
# Revision 1.1.2.1  1996/10/03  20:25:45  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  19:20:27  arvind]
# 
# Revision /main/DCE_1.2.2/1  1996/09/09  21:24 UTC  arvind
# 	PKSS drop from DEC (DCE1.2.2)
# 	[1996/08/30  16:12 UTC  arvind  /main/arvind_pkss/1]
# 
# $EndLog$
#
# (c) Copyright 1996,
# Digital Equipment Corporation, Maynard, Massachusetts, USA
# All Rights Reserved
#
#



OBJFILES = asn.o \
           x500name.o \
           x500_att.o \
           xom_consts.o \
           x509.o \
	   pkc_common.o \
	   dcepkcsvc.o \
	   dcepkcmsg.o \





libpk_OFILES = ${OBJFILES} 
libpk.a_OFILES = ${OBJFILES}


${OBJFILES}: h/asn.h \
             h/x500name.h \


.if !defined(LIBDCE_LIBRARY_MAKEFILE)

#VPATH = h:asn:ca:utils:crypto:common:../h:../psm:${BSAFE_ROOT}/usr/lib
VPATH = h:asn:utils:crypto:common:../h:../psm:${BSAFE_ROOT}/usr/lib

HOST_LD= xlC

INCFLAGS = -Ih -I../h -I../psm/h -I${BSAFE_ROOT}/usr/include

LIBS = -L${BSAFE_ROOT}/usr/lib -L../psm -L. -lpk -ldce -lpsm -lbsafe2

IDLFILES = 
IDLOBJ = 


CFLAGS		+= ${${TARGET_MACHINE}_CFLAGS}

CFLAGS		+= -DASN_DEBUG

CFLAGS		+= -DXDS_DEBUG

#CFLAGS += -DFORCE_THREADS

#.if ${NOENCRYPTION} == 1
#CFLAGS          += -DNOENCRYPTION  
#.endif

SAMIDL_SUBDIRS = h pkss capi

EXPINC_SUBDIRS = h pkss capi

EXPLIB_SUBDIRS = pkss capi xds

SUBDIRS = pkss capi



LIBRARIES = 		libpk.a

OBJECTS = ${OBJFILES}

#PROGRAMS = asnutil dhparamgen 
PROGRAMS = dhparamgen 
libpk.a_EXPDIR =	/usr/lib/

EXPSHLIB_TARGETS = ${libpk_OFILES}


EXPDIR = /usr/lib

asn.o : asn.cxx asn.h

x500name.o : x500name.cxx asn.h

x509.o : x509.cxx asn.h

x500_att.o : x500_att.cxx asn.h

xom_consts.o : xom_consts.c

#ca : ca.cxx
#
#ca_LIBS = -L${BSAFE_ROOT}/usr/lib -L../psm -ldce -lpk -lpsm -lbsafe2 

asnutil : asnutil.cxx

dhparamgen : dhparamgen.c

xds-cp.o : xds-cp.cxx xdsxx.h asn.h

xds-cp : xds-cp.o
	${HOST_LD} -o xds-cp ${CFLAGS} ${LDFLAGS} ${LIBDIRS} \
	${LIBS} xds-cp.o

pkc_common.o : pkc_common.cxx

.include <${RULES_MK}>

.endif



