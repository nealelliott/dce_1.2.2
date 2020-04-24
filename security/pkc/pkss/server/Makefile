#   
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#  
# All Rights Reserved
#    
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/10/03  20:30:16  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  19:26:01  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/09/09  21:28 UTC  arvind
# 	PKSS drop (2) from DEC (DCE1.2.2)
# 	[1996/09/04  14:24 UTC  arvind  /main/arvind_pkss/2]
# 
# $EndLog$
#
# (c) Copyright 1996,
# Digital Equipment Corporation, Maynard, Massachusetts, USA
# All Rights Reserved
#
#

RIOS_CPP_LIBS = -L/usr/lpp/xlC/lib -L/usr/lpp/xlC -lC -lm

CPP_LIBS = ${${TARGET_MACHINE}_CPP_LIBS}

# This directory can be built in two distinct ways, depending on whether
# NO_PIPE is defined.  This symbol affects whether the client-side of the PKSS
# is built as a single library, or whether it is built as a library plus
# a helper process, connected by a pipe.  The client-side objects are divided
# into three classes:
#
# i) Common objects, included in both the helper and client library
# ii) library-only objects
# iii) Helper-only objects
#
# When building with NO_PIPE, all three classes are inserted into libpkssapi;
# when building without NO_PIPE, the helper process is built from classes
# i and iii, while the library is built from classes i and ii.
# Also, when building with the pipe, the modules pkss_pipe_client and
# pkss_pipe_format are inserted into the client library, and pkss_pipe_server
# and pkss_pipe_format are linked with the helper process.

PKSS_CLT_COMMON_OBJECTS =       sec_pkss_clt_util.o     \
                                sec_pkss_util.o         \
                                sec_pkss_dump_key.o     \
                                pkss_config_common.o    \
                                dnscfg.o                \
                                dcepkssvc.o             \
                                dcepksmsg.o             \

PKSS_CLT_LIBRARY_OBJECTS =      sec_pvtkey_pkss.o               \
                                sec_pvtkey_pkss_capabilities.o  \
                                sec_pkss_mgmt_api.o             \
                                sec_pkss_mgmt_if_cstub.o        \

PKSS_CLT_HELPER_OBJECTS =       sec_pkss_clt_api.o      \
                                sec_pkss_clt_if_cstub.o \

PKSS_COMMON_DEPS = sec_pkss_asn.h       \
                   sec_pkss_util.h      \
                   asn.h

PKSS_API_DEPS = $(PKSS_COMMON_DEPS)     \
                sec_pkss_clt_util.h

PKSS_MGR_DEPS = $(PKSS_COMMON_DEPS)     \
                gdb44_methods.h         \
                sec_pkss_svr_util.h     \
                sec_pkss_dbkey_class.h  \
#               malloc_trace.h

PKSS_MGMT_DEPS =        sec_pkss_mgmt_api.h     \
                        sec_pkss_mgmt_if.h

PKSS_CLT_DEPS =         sec_pkss_clt_api.h      \
                        sec_pkss_clt_if.h
#NO_PIPE=1

###########################################################
# define the following "globally" for the libdce.so build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#
# List of objects

.if defined(NO_PIPE)
libpkssapi_OFILES =     ${PKSS_CLT_LIBRARY_OBJECTS}     \
                        ${PKSS_CLT_COMMON_OBJECTS}      \
                        ${PKSS_CLT_HELPER_OBJECTS}

.else
libpkssapi_OFILES =     ${PKSS_CLT_LIBRARY_OBJECTS}     \
                        ${PKSS_CLT_COMMON_OBJECTS}      \
                        pkss_pipe_client.o              \
                        pkss_pipe_format.o

.endif

libpkssapi.a_OFILES = ${libpkssapi_OFILES}


#
# The rest of the Makefile is "private" for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)

pkss_helper_OFILES =	pkss_helper.o

pkss_helper2_OFILES =   pkss_pipe_server.o      \
                        pkss_pipe_format.o      \
                        $(PKSS_CLT_HELPER_OBJECTS)      \
                        ${PKSS_CLT_COMMON_OBJECTS}      \


pkssd_OFILES = sec_pkss_main.o          \
               sec_pkss_clt_mgr.o       \
               sec_pkss_mgmt_mgr.o      \
               sec_pkss_svr_util.o      \
               sec_pkss_util.o          \
               sec_pkss_dbkey_class.o   \
               gdb44_methods.o          \
               pkss_config_common.o     \
               pkss_config_server.o     \
               dcepkssvc.o              \
               dcepksmsg.o              \
               pkss_acl_mgr.o           \
               dnscfg.o                 \
               sec_pkss_clt_if_sstub.o  \
               sec_pkss_mgmt_if_sstub.o \
               rdaclif_sstub.o

#
# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
#
OBJECTS =       ${PKSS_CLT_LIBRARY_OBJECTS}     \
                ${PKSS_CLT_HELPER_OBJECTS}      \
                ${PKSS_CLT_COMMON_OBJECTS}      \
                pkss_pipe_client.o              \
                pkss_pipe_server.o              \
                pkss_pipe_format.o              \
                ${pkssd_OFILES}

#
# libs for binding
#
LIBS            = ${${TARGET_MACHINE}_LIBS}     \
                  -lpk -lcdsutil -lgdb -lpsm -ldce -lbsafe2 ${CPP_LIBS}

LIBDIRS += -L../../../psm       \
           -L..                 \
           -L../..              \
           -L../../../../directory/cds/gloria/gdb

# dirs to search for source
VPATH = ../../../../directory/cds/library:../pipe:../acl:../../h:../pipe/${TARGET_MACHINE}

#
# Don't want to build any '.a' here if we are using shared libraries
#
.if ${USE_SHARED_LIBRARIES} == 0
  # libraries to build
  LIBRARIES	= libpkssapi.a
  # things to install, utils is private
#  ILIST = libpkssapi.a
.else
  LIBRARIES	=
.endif

EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}

EXPSHLIB_TARGETS        = ${libpkssapi_OFILES}

# install dir for libs & export dir for libs
EXPDIR		= /usr/include/dce/
IDIR		= /bin/
ILIST = pkssd pkss_helper2 pkss_helper

# compiler flags
CFLAGS	        = ${${TARGET_MACHINE}_CFLAGS}  -g -DENABLE_ALL

INCFLAGS        = -I.                                   \
                  -I../../../psm/h                      \
                  -I../../../psm/domestic               \
                  -I../../../h                          \
                  -I../../../../directory/cds/gloria/gdb \
                  -I../../../../directory/cds/includes  \
                  -I../acl				\
                  -I../pipe/$(TARGET_MACHNE)

IDLFLAGS = -keep c_source

LDFLAGS		= ${${TARGET_MACHINE}_LDFLAGS}

IDLFILES        = sec_pkss_types_if.idl         \
                  sec_pkss_clt_if.idl           \
                  sec_pkss_mgmt_if.idl          \
                  rdaclif.idl

IDLINCFLAGS = -I.

IDLOBJ = ${IDLFILES:.idl=.h}

SAMSFILES        = pks.sams

SAMIDL_INCLUDES  = dcepksmsg.h \
                   dcepksmac.h \
                   dcepkssvc.h

SAMS_TARGETS     = dcepksmsg.h \
                   dcepksmsg.c \
                   dcepkssvc.c \
                   dcepkssvc.h \
                   dcepks.cat

#
# programs to build
#
PROGRAMS        = pkssd

#
# pkss_helper is linked statically to avoid a security exposure.  The
# exact load flags to produce a statically linked executable will vary
# from platform to platform.
#
.if !defined(NO_PIPE)
PROGRAMS        += pkss_helper pkss_helper2
pkss_helper_LIBS=
pkss_helper_LDFLAGS = -bnso -bI:/lib/syscalls.exp
.endif

#
# Files to export and their locations
#
INCLUDES = dcepksmsg.h \
           dcepksmac.h \
           dcepkssvc.h \
           ${IDLOBJ}


.include <${RULES_MK}>

.endif
# 'endif' for libdce library makefile




