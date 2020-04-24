#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.10.2  1996/03/11  13:25:29  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:13:40  marty]
#
# Revision 1.1.10.1  1995/12/08  17:52:24  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:27:11  root]
# 
# Revision 1.1.8.1  1994/02/09  20:52:09  mori_m
# 	CR 9912  Added libiconv reference.
# 	[1994/02/09  20:51:40  mori_m]
# 
# Revision 1.1.6.2  1993/05/24  20:46:19  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:14:44  cjd]
# 
# Revision 1.1.4.2  1993/05/12  13:20:10  jd
# 	Initial 486 port.
# 	[1993/05/12  13:19:41  jd]
# 
# $EndLog$
#

# AT386 OSF1 with gld and gcc
#

SHARED_LIBRARIES        =  libdce_1.o libdce_2.o libdce_3.o libdce_4.o \
                           libdce.so

.NOTMAIN:  ${SHARED_LIBRARIES} export_libdce.so

LDFLAGS                 = 
EXPORTS                 = -export_default libdce:
LIBS                    =

.if ${USE_SHARED_LIBRARIES}
.undef LIBRARIES
.undef EXPLIB_TARGETS
.endif

EXPSHLIB_TARGETS        = SUBLIBS_BUILT libdce_1.o libdce_2.o \
                          libdce_3.o libdce_4.o  export_libdce.so
.if ${USE_SHARED_LIBRARIES}
ILIST                   = libdce.so
IDIR                    = /usr/shlib/
EXPDIR                  = /usr/shlib/
IMOD                    = 644
.endif

libdce.so_OFILES  = libdce_1.o libdce_2.o libdce_3.o libdce_4.o

PARTIAL_LDFLAG    = -r

libdce_1.o_SHLDFLAGS = ${PARTIAL_LDFLAG}
libdce_2.o_SHLDFLAGS = ${PARTIAL_LDFLAG}
libdce_3.o_SHLDFLAGS = ${PARTIAL_LDFLAG}
libdce_4.o_SHLDFLAGS = ${PARTIAL_LDFLAG}
libdce.so_LIBS       = -lc -lbsd -liconv ${_GCC_EXEC_PREFIX_}gnulib

#
# If we did not just build the libraries, then don't tell the common makefile
# to build libdce.a
#
.if ${SUBMAKE_TARGET} != "export_all_EXPSHLIB"
.undef SHARED_LIBRARIES
.endif
