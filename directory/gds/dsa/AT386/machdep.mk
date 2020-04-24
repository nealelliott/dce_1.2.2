# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:13:32  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:51  marty]
#
# Revision 1.1.4.1  1995/12/08  15:40:57  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:05:39  root]
# 
# Revision 1.1.2.2  1994/07/06  15:53:41  marrek
# 	Add version string in July 1994 code drop.
# 	[1994/07/06  15:40:23  marrek]
# 
# Revision 1.1.2.1  1994/06/21  14:39:32  marrek
# 	Created in June 94 code submission.
# 	[1994/06/10  16:04:11  marrek]
# 
# $EndLog$
#
# $RCSfile: machdep.mk,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:13:32 $
#

EXPORTS	= -export_default libdsal:
SHARED_LIBRARIES = libdsal.so

EXPSHLIB_TARGETS = ${SHARED_LIBRARIES:S/^/export_/g}

ILIST	+= ${SHARED_LIBRARIES}
libdsal.so_IDIR	= /usr/shlib/
libdsal.so_EXPDIR	= /usr/shlib/
libdsal.so_IMODE	= 755

libdsal.so_OFILES	= d26phon.o

