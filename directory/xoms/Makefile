# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  13:24:50  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:13:08  marty]
#
# Revision 1.1.4.1  1995/12/08  17:49:49  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:26:07  root]
# 
# Revision 1.1.2.5  1994/06/21  14:49:02  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:49:24  marrek]
# 
# Revision 1.1.2.4  1994/06/10  21:21:47  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:56:27  devsrc]
# 
# Revision 1.1.2.3  1994/05/10  16:04:23  marrek
# 	April 1994 code submission.
# 	[1994/04/27  13:16:19  marrek]
# 
# Revision 1.1.2.2  1994/03/23  15:50:24  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:39:20  keutel]
# 
# Revision 1.1.2.1  1994/02/22  19:00:57  marrek
# 	correction for xoiinit.o
# 	[1994/02/18  17:22:31  marrek]
# 
# 	Change rcsid string format.
# 	[1994/02/09  08:35:51  marrek]
# 
# 	Creation for dce1.1.
# 	[1994/02/02  11:01:12  marrek]
# 
# $EndLog$
# 

# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:50 $

.if !defined(GDS_NO_LIBDCE)
CFLAGS		+= -DTHREADSAFE
.endif # THREADSAFE

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#

libxoms_OFILES	= xoi.o \
		xoidebug.o \
		xoiinit.o \
		xoisort.o \
		xoiutil.o \
		xos.o \
		xosattr.o \
		xosclass.o \
		xosdebug.o \
		xosfilter.o \
		xosname.o \
		xosutil.o

libxoms.a_OFILES = ${libxoms_OFILES}

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

LIBRARIES	= libxoms.a
EXPLIB_TARGETS  = ${LIBRARIES:S/^/export_/g}

EXPSHLIB_TARGETS	= ${libxoms_OFILES}
EXPDIR			= /usr/lib/
EXPINC_SUBDIRS		= h
SUBDIRS			= msg h

INCFLAGS		= -Ih -I../xom/h -I../xds/h -I../xmh/h

DATAFILES		= xoischema
ILIST			= ${DATAFILES}
IDIR			= /share/var/adm/directory/gds/adm/

.include <${RULES_MK}>

.endif
