# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:13:33  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:52  marty]
#
# Revision 1.1.4.1  1995/12/08  15:41:02  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:05:40  root]
# 
# Revision 1.1.2.2  1994/07/06  15:53:42  marrek
# 	Add version string in July 1994 code drop.
# 	[1994/07/06  15:40:24  marrek]
# 
# Revision 1.1.2.1  1994/06/21  14:39:33  marrek
# 	Created in June 94 code submission.
# 	[1994/06/10  16:04:13  marrek]
# 
# $EndLog$
#
# $RCSfile: machdep.mk,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:13:33 $
#

SHARED_LIBRARIES = libdsal.sl

EXPSHLIB_TARGETS = ${SHARED_LIBRARIES:S/^/export_/g}

ILIST	+= ${SHARED_LIBRARIES}
libdsal.sl_IDIR	= /usr/lib/
libdsal.sl_EXPDIR	= /usr/lib/
libdsal.sl_IMODE	= 755

libdsal.sl_OFILES	= d26phon.o

