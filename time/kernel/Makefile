#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
#(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
# All Rights Reserved
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.2  1996/03/09  23:31:18  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:30  marty]
#
# Revision 1.1.9.1  1995/12/08  18:09:56  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:24:28  root]
# 
# Revision 1.1.7.1  1993/12/07  22:35:54  devobj
# 	Changed < and > to " (except RULES_MK) in order to use ODE2.3 make
# 	[1993/12/07  22:35:12  devobj]
# 
# Revision 1.1.5.3  1992/12/30  20:40:35  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:09:09  htf]
# 
# Revision 1.1.5.2  1992/09/29  21:27:18  devsrc
# 	[OT 5373]  SNI/SVR4 Merge.
# 	[1992/09/14  20:04:15  sekhar]
# 
# Revision 1.1.2.2  1992/05/06  21:54:42  comuzzi
# 	Fix defects 2295 and 3143. Added architecture dependent dirs in common to INCFLAGS.
# 	[1992/05/06  21:26:57  comuzzi]
# 
# Revision 1.1  1992/01/19  15:33:57  devrcs
# 	Initial revision
# 
# $EndLog$
#

# source search path
VPATH = ../common:${TARGET_MACHINE}

# compiler flags
CFLAGS = ${${TARGET_MACHINE}_CFLAGS} -DKERNEL -D_KERNEL 

# include files search path
INCFLAGS = -I${TARGET_MACHINE} -I../common/${TARGET_MACHINE}/${TARGET_OS_PLATFORM} -I../common

# install directory
IDIR = /ext/

# move to obj tree 
DATAFILES = ${${TARGET_MACHINE}_DATAFILES}

# export
INCLUDES = ${${TARGET_MACHINE}_INCLUDES}

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
