#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.13.2  1996/10/03  14:37:27  arvind
# 	Build libdce.a anyhow (we still have a few statically-linked things in
# 	the tree)
# 	[1996/09/17  23:26 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/2]
#
# 	Enhanced NO_GDS.
# 	[1996/09/17  00:12 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
#
# 	Merge from cuti_pk_export to DCE_1.2.2
# 	[1996/06/27  13:54 UTC  cuti  /main/cuti_pk_export/1]
#
# 	Add bsafe2 to libdce.sl_LIBS
# 	[1996/03/11  13:25:30  marty  1.1.11.2]
#
# Revision 1.1.13.1  1996/07/08  18:22:49  arvind
# 	Merge from cuti_pk_export to DCE_1.2.2
# 	[1996/06/27  13:54 UTC  cuti  /main/cuti_pk_export/1]
# 
# 	Add bsafe2 to libdce.sl_LIBS
# 	[1996/03/11  13:25:30  marty  1.1.11.2]
# 
# Revision 1.1.11.2  1996/03/11  13:25:30  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:13:41  marty]
# 
# Revision 1.1.11.1  1995/12/08  17:52:29  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:27:14  root]
# 
# Revision 1.1.9.3  1994/08/26  16:01:10  sommerfeld
# 	Don't install libdce.a, either..
# 	[1994/08/25  15:41:23  sommerfeld]
# 
# Revision 1.1.9.2  1994/08/24  20:09:56  sommerfeld
# 	Turn off build of libdce.a
# 	[1994/08/24  18:15:09  sommerfeld]
# 
# Revision 1.1.9.1  1994/06/13  15:20:31  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:57:28  devsrc]
# 
# Revision 1.1.6.2  1993/06/10  19:20:15  sommerfeld
# 	Initial revision, derived from HP product sources.
# 	[1993/04/07  15:57:20  sommerfeld]
# 
# $EndLog$
#

# HP800/HPUX

SHARED_LIBRARIES        =  libdce.sl

.NOTMAIN:  ${SHARED_LIBRARIES} export_libdce.sl

LIBS                    =

EXPSHLIB_TARGETS        = SUBLIBS_BUILT libdce_1.o libdce_2.o \
                          libdce_3.o libdce_4.o  export_libdce.sl

.if defined (USE_DES)
libdce.sl_LIBS		= -lm -lbsafe2
.else
libdce.sl_LIBS		= -lm 
.endif

.if ${USE_SHARED_LIBRARIES}
#.undef ILIST
#.undef LIBRARIES
#.undef EXPLIB_TARGETS

ILIST                   += libdce.sl
.endif

IDIR                    = /usr/lib/
EXPDIR                  = /usr/lib/
IMOD                    = 644

PARTIAL_LDFLAG    = -r

libdce.sl_OFILES  = libdce_1.o libdce_2.o libdce_4.o

.if !defined(NO_GDS)
libdce.sl_OFILES += libdce_3.o 
.endif

libdce_1.o:	${libdce_1.o_OFILES}
	${_LD_} ${PARTIAL_LDFLAG} -o ${.TARGET}.X ${libdce_1.o_OFILES}
	${MV} ${.TARGET}.X ${.TARGET}

libdce_2.o:	${libdce_2.o_OFILES}
	${_LD_} ${PARTIAL_LDFLAG} -o ${.TARGET}.X ${libdce_2.o_OFILES}
	${MV} ${.TARGET}.X ${.TARGET}

.if !defined(NO_GDS)
libdce_3.o:	${libdce_3.o_OFILES}
	${_LD_} ${PARTIAL_LDFLAG} -o ${.TARGET}.X ${libdce_3.o_OFILES}
	${MV} ${.TARGET}.X ${.TARGET}
.endif

libdce_4.o:	${libdce_4.o_OFILES}
	${_LD_} ${PARTIAL_LDFLAG} -o ${.TARGET}.X ${libdce_4.o_OFILES}
	${MV} ${.TARGET}.X ${.TARGET}

# just the .o's, ma'am.

NO_RECOMPILE=1
#
# If we did not just build the libraries, then don't tell the common makefile
# to build libdce.a
#
.if ${SUBMAKE_TARGET} != "export_all_EXPSHLIB"
.undef SHARED_LIBRARIES
.endif

