#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.11.2  1996/03/11  13:25:33  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:13:43  marty]
#
# Revision 1.1.11.1  1995/12/08  17:52:42  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:27:20  root]
# 
# Revision 1.1.7.5  1993/03/02  20:27:05  sommerfeld
# 	[7281] take out -x; it makes debugging possible.
# 	[1993/03/02  19:33:43  sommerfeld]
# 
# Revision 1.1.7.4  1992/12/30  17:16:53  zeliff
# 	Embedding copyright notice
# 	[1992/12/30  17:15:05  zeliff]
# 
# Revision 1.1.7.3  1992/10/20  19:27:02  rsarbo
# 	link dacl2acl and afssys using standard mechanisms
# 	[1992/10/20  19:26:06  rsarbo]
# 
# Revision 1.1.7.2  1992/10/19  14:20:09  rsarbo
# 	link dacl2acl and afssys libs
# 	[1992/10/19  14:18:15  rsarbo]
# 
# Revision 1.1.2.2  1992/02/04  22:02:44  mhickey
# 	Pulled out the PMAX specific shared lib build rules
# 
# 	Defect 2030
# 	[1992/02/04  22:02:00  mhickey]
# 
# $EndLog$
#

# PMAX OSF1 with gld and gcc
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
libdce.so_LIBS       = -lc -lbsd 

#
# If we did not just build the libraries, then don't tell the common makefile
# to build libdce.a
#
.if ${SUBMAKE_TARGET} != "export_all_EXPSHLIB"
.undef SHARED_LIBRARIES
.endif
