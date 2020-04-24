# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
# Burlington, MA, USA
# All Rights Reserved
# (c) Copyright 1990, 1991, 1992
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.46.2  1996/03/09  23:30:55  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:10  marty]
#
# Revision 1.1.46.1  1995/12/07  21:56:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:03:25  root]
# 
# Revision 1.1.44.1  1994/06/09  13:36:58  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:27:04  devsrc]
# 
# Revision 1.1.42.1  1993/10/05  21:38:43  ohara
# 	SNI SVR4 ref port 8596
# 	[1993/09/15  22:18:11  ohara]
# 
# Revision 1.1.3.3  1992/12/15  22:24:20  alan
# 	Insert copyright notices
# 	[1992/12/07  18:59:44  alan]
# 
# Revision 1.1.3.2  1992/09/29  21:26:54  devsrc
# 	[OT 5373]    SNI/SVR4 merge.
# 	[1992/09/17  20:40:08  sekhar]
# 
# $EndLog$
# 
VPATH		= ${TARGET_MACHINE}:${TARGET_MACHINE}/${TARGET_OS_PLATFORM}

ILIST		+= libcma.a
INTERMED_SHLIBS	= libcma.so

libcma.a_IDIR	= /usr/lib/
libcma.so_IDIR	= /usr/lib/
libcma.so_EXPDIR = /usr/lib/
pthread.h_IDIR	= /share/include/

CFLAGS          += `genpath -I${TARGET_MACHINE}/${TARGET_OS_PLATFORM}`

.if exists(${TARGET_MACHINE}/${TARGET_OS_PLATFORM}/machdep.mk)
.include "${TARGET_MACHINE}/${TARGET_OS_PLATFORM}/machdep.mk"
.endif

