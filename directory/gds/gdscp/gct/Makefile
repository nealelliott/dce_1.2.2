# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:13:50  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:08  marty]
#
# Revision 1.1.4.1  1995/12/08  15:57:08  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:12:11  root]
# 
# Revision 1.1.2.3  1994/09/19  08:39:31  marrek
# 	Fixes for OT12176 (wrong incore tables.)
# 	[1994/09/19  08:33:49  marrek]
# 
# Revision 1.1.2.2  1994/06/09  18:45:38  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:18:32  devsrc]
# 
# Revision 1.1.2.1  1994/03/23  15:16:35  keutel
# 	creation
# 	[1994/03/21  12:57:20  keutel]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:13:50 $
#

.if !defined(GDS_NO_LIBDCE)
RIOS_CFLAGS	+= -D_THREAD_SAFE
CFLAGS		+= ${${TARGET_MACHINE}_CFLAGS} -DTHREADSAFE
.else
CFLAGS		+= ${${TARGET_MACHINE}_CFLAGS}
.endif # THREADSAFE

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#

libgct_OFILES	= gctargs.o \
		gctbind.o \
		gctcompare.o \
		gctcreate.o \
		gcterr.o \
		gctlist.o \
		gctmoddsc.o \
		gctmodify.o \
		gctmodrdn.o \
		gctmoveto.o \
		gctread.o \
		gctremove.o \
		gctsearch.o \
		gctshowabbrv.o \
		gctshowdsc.o \
		gctutil.o

libgct.a_OFILES = ${libgct_OFILES}

#
# The rest of the Makefile is "private" for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
# 
# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
#
OBJECTS=
LIBS=

LIBRARIES	= libgct.a
EXPLIB_TARGETS  = ${LIBRARIES:S/^/export_/g}

EXPDIR		= /usr/lib/
EXPINC_SUBDIRS	= h
SUBDIRS		= h

INCFLAGS	= -Ih -I../../h

.include <${RULES_MK}>

.endif
