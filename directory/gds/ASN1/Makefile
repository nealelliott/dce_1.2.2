#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.11.2  1996/03/09  23:12:23  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:35  marty]
#
# Revision 1.1.11.1  1995/12/08  15:05:32  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:47:00  root]
# 
# Revision 1.1.9.2  1994/06/21  14:43:03  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:21:55  marrek]
# 
# Revision 1.1.9.1  1994/02/22  15:20:03  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  08:50:20  marrek]
# 
# Revision 1.1.7.2  1993/08/11  11:14:52  marrek
# 	July 1993 code drop.
# 	[1993/07/28  16:54:40  marrek]
# 
# Revision 1.1.5.3  1992/12/31  16:33:35  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  12:50:59  bbelch]
# 
# Revision 1.1.5.2  1992/11/27  16:55:08  marrek
# 	November 1992 code drop
# 	[1992/11/17  14:40:57  marrek]
# 
# Revision 1.1.2.3  1992/06/04  20:25:32  rec
# 	Added lib_r to the EXPLIB_SUBDIRS line so that libdce.a knows
# 	how to make libasn1_r.a.  That is kept in the Makefile in lib_r.
# 	lib_r was removed in the previous gds drop.
# 	[1992/06/04  20:09:55  rec]
# 
# Revision 1.1.2.2  1992/06/01  20:13:28  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  18:12:32  zeliff]
# 
# Revision 1.1  1992/01/19  15:11:20  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.11.2 $ $Date: 1996/03/09 23:12:23 $
#

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#

libasn1_OFILES=binmov.o bitcod.o copy.o dump.o intcod.o real.o \
	intdec.o lencod.o lendec.o length.o move.o number.o \
	octmov.o octpak.o skip.o skiptype.o strbin.o \
	errmes.o error.o load.o liblw.o libasn1.o type_cod.o \
	field_cmp.o bits_cmp.o fetch.o oid.o strdec.o oidint.o \
	end.o end_d.o end_set.o utctime.o input.o copying.o load_text.o\
	allocm.o inreal.o absurd.o

libasn1.a_OFILES = ${libasn1_OFILES}

#
# The rest of the Makefile is "private" for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#
# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
#
OBJECTS =
LIBS    =

INCLUDES	= asn1.h

asn1.h_EXPDIR	= /usr/include/dce/

#
# Don't want to build any '.a' here if we are using shared libraries
#
.if ${USE_SHARED_LIBRARIES} == 0
  LIBRARIES     = libasn1.a
  EXPLIB_TARGETS        = ${LIBRARIES:S/^/export_/g}
  libasn1.a_EXPDIR  = /usr/lib/
.endif

EXPSHLIB_TARGETS        = ${libasn1_OFILES}

CFLAGS		+= -DSYS5 -DOSF_DCE ${${TARGET_MACHINE}_CFLAGS}

.include <$(RULES_MK)>

.endif

