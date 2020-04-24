#
# @OSF_COPYRIGHT@
# Copyright (c) 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.1  1996/11/21  14:13:33  arvind
# 	OSF DCE 1.2.2 Drop5
# 	[1996/11/21  14:12:21  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/11/20  16:23 UTC  arvind
# 	Fix from OSF
# 	[1996/11/19  21:45 UTC  arvind  /main/arvind_work/1]
# 
# Revision 1.1.2.2  1996/03/09  23:07:06  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:25:51  marty]
# 
# Revision 1.1.2.1  1995/12/08  20:53:02  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/2  1995/11/16  21:39 UTC  dat
# 	Create ILIST for libdcedpvt.a when building non-shared libraries
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/2  1995/11/16  21:38 UTC  jrr
# 	Create ILIST for libdcedpvt.a when building non-shared libraries
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/10  21:44 UTC  psn
# 	Fix for DCE 1.2.1 build on AIX
# 
# 	HP revision /main/HPDCE02/2  1995/10/19  14:07 UTC  jss
# 	Remove -g option.
# 
# 	HP revision /main/HPDCE02/1  1995/09/29  22:39 UTC  tatsu_s
# 	Submitted the fix for CHFts16512.
# 
# 	HP revision /main/tatsu_s.dced_local.b0/4  1995/09/29  17:56 UTC  tatsu_s
# 	Added libdcedpvt.1_OFILES/LIBS.
# 
# 	HP revision /main/tatsu_s.dced_local.b0/3  1995/09/26  21:39 UTC  tatsu_s
# 	Added IDIR.
# 
# 	HP revision /main/tatsu_s.dced_local.b0/2  1995/09/23  00:43 UTC  tatsu_s
# 	Added dcedpvt_server.h.
# 
# 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  15:23 UTC  tatsu_s
# 	Initial revision.
# 	[1995/12/08  20:48:46  root]
# 
# $EndLog$
#

COMPILED_IDL	= dcedhdsvc.o dcedhdmsg.o \
	dced_convert_cstub.o dced0_convert_cstub.o

COMPILED_OTHER	= init.o acls.o lock.o copy.o misc.o hdmisc.o \
		asops.o hdops.o kmops.o scops.o sxops.o \
		globals.o
COMPILED_HEADERS= hostdata.h rkeytab.h rs_attr_schema.h srvrconf.h srvrexec.h

CFLAGS		+= ${STRICT_COMPILER_WARNINGS}
INCFLAGS	= -I. -I../idl \
		-I../../../rpc/runtime \
		-I../../../rpc/runtime/${TARGET_MACHINE} \
		-I../../../security/h -I../../../security/idl

INCLUDES	= dcedpvt.h dcedpvt_server.h dce_lock.h
EXPDIR		= /usr/include/dce/

libdcedpvt_OFILES	= $(COMPILED_IDL) $(COMPILED_OTHER)
libdcedpvt.1_OFILES	= ${libdcedpvt_OFILES}
libdcedpvt.1_LIBS	= -ldce
libdcedpvt.sl_OFILES	= ${libdcedpvt_OFILES}
libdcedpvt.sl_LIBS	= -ldce
libdcedpvt.a_OFILES	= ${libdcedpvt_OFILES}
libdcedpvt.a_LIBS	= -ldce

.if ${USE_SHARED_LIBRARIES} != 0

.if ${ux_release} >= 1000
  libdcedpvt.sl_SHLDFLAGS= -b +h libdcedpvt.1 +b /usr/lib \
			-c ${export_symbols:P}  ${LIBDIRS}
  libdcedpvt.1_SHLDFLAGS	= -b +h libdcedpvt.1 +b /usr/lib \
			-c ${export_symbols:P}  ${LIBDIRS}
  SHARED_LIBRARIES	= libdcedpvt.sl libdcedpvt.1
  ILIST			= libdcedpvt.sl libdcedpvt.1
.else
  libdcedpvt.sl_SHLDFLAGS= -b -c ${export_symbols:P} ${LIBDIRS}
  SHARED_LIBRARIES	= libdcedpvt.sl
  ILIST			= libdcedpvt.sl
.endif

EXPSHLIB_TARGETS	= ${libdcedpvt_OFILES} \
			${SHARED_LIBRARIES:S/^/export_/g}
libdcedpvt.sl_EXPDIR	= /usr/lib/
libdcedpvt.1_EXPDIR	= /usr/lib/

libdcedpvt.sl_IDIR	= /usr/lib/
libdcedpvt.1_IDIR	= /usr/lib/

.else
  LIBRARIES		= libdcedpvt.a
  EXPLIB_TARGETS	= ${libdcedpvt_OFILES} \
			${LIBRARIES:S/^/export_/g}
  ILIST			= libdcedpvt.a
  libdcedpvt.a_IDIR	= /usr/lib/
  libdcedpvt.a_EXPDIR	= /usr/lib/
.endif

dced_convert_ACF	=
dced0_convert_ACF	=
IDLFLAGS		= -keep c_source
hostdata.idl_IDLFLAGS	= -keep c_source -no_mepv
rkeytab.idl_IDLFLAGS	= -keep c_source -no_mepv
rs_attr_schema.idl_IDLFLAGS	= -keep c_source -no_mepv
srvrconf.idl_IDLFLAGS	= -keep c_source -no_mepv
srvrexec.idl_IDLFLAGS	= -keep c_source -no_mepv
rdaclifv0.idl_IDLFLAGS	= -keep c_source -no_mepv
rdaclif.idl_IDLFLAGS	= -keep c_source -no_mepv
rs_attr_schema.idl_IDLINCFLAGS	= -I../../../security/idl
dced_convert.idl_IDLINCFLAGS	= -I.
dced_convert.idl_IDLFLAGS	= -no_mepv -keep c_source 
dced0_convert.idl_IDLINCFLAGS	= -I.
dced0_convert.idl_IDLFLAGS	= -no_mepv -keep c_source

IDLFILES	= hostdata.idl rkeytab.idl \
		srvrconf.idl srvrexec.idl rs_attr_schema.idl \
		dced_convert.idl dced0_convert.idl rdaclif.idl rdaclifv0.idl
VPATH		= ../idl:../../../security/idl:../../../rpc/sys_idl

.include <${RULES_MK}>

idlcompile:		$(COMPILED_IDL)

libdcedpvt.sl:		export_symbols
libdcedpvt.1:		export_symbols

${COMPILED_OTHER}:	dced_convert.h dced0_convert.h acldefs.h $(COMPILED_HEADERS)

dced_convert.h:		dced_convert.idl
dced_convert_cstub.o:	dced_convert_cstub.c
dced0_convert.h:	dced0_convert.idl
dced0_convert_cstub.o:	dced0_convert_cstub.c

hostdata.h:		hostdata.idl
rkeytab.h:		rkeytab.idl
rs_attr_schema.h:	rs_attr_schema.idl
srvrconf.h:		srvrconf.idl
srvrexec.h:		srvrexec.idl
