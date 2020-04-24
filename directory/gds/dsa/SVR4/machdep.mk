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
# Revision 1.1.4.2  1996/03/09  23:13:38  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:55  marty]
#
# Revision 1.1.4.1  1995/12/08  15:41:22  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:05:45  root]
# 
# Revision 1.1.2.2  1994/07/06  15:53:44  marrek
# 	Add version string in July 1994 code drop.
# 	[1994/07/06  15:40:30  marrek]
# 
# Revision 1.1.2.1  1994/06/21  14:39:35  marrek
# 	Created in June 94 code submission.
# 	[1994/06/10  16:04:16  marrek]
# 
# $EndLog$
#
# $RCSfile: machdep.mk,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:13:38 $
#

LIBRARIES += libdsal.so

EXPLIB_TARGETS = ${LIBRARIES:S/^/export_/g}

ILIST	+= libdsal.so
libdsal.so_EXPDIR	= /usr/lib/
libdsal.so_IDIR	= /usr/lib/
libdsal.so_IMODE	= 755

libdsal.so_OFILES	= d26phon.o

libdsal.so: d26phon.o
	${_CC_} -G -o libdsal.so d26phon.o

