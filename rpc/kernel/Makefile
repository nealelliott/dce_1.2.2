# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# (c) Copyright 1991, 1992 
#     Siemens-Nixdorf Information Systems, Burlington, MA, USA
#     All Rights Reserved
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.7.2  1996/03/09  23:26:01  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:41  marty]
#
# Revision 1.1.7.1  1995/12/08  00:13:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:55:26  root]
# 
# Revision 1.1.5.1  1994/01/04  17:49:15  ohara
# 	fix include of machdep.mk for new make
# 	[1994/01/04  17:34:19  ohara]
# 
# Revision 1.1.3.4  1993/01/03  22:34:47  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:49:59  bbelch]
# 
# Revision 1.1.3.3  1992/12/23  19:35:59  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  01:15:39  zeliff]
# 
# Revision 1.1.3.2  1992/09/29  21:12:42  devsrc
# 	SNI/SVR4 merge.  OT 5373
# 	[1992/09/11  15:35:31  weisman]
# 
# $EndLog$
#
# source search path
VPATH = ../runtime:${TARGET_MACHINE}

# compiler flags
CFLAGS = ${${TARGET_MACHINE}_CFLAGS} 

# include files search path
INCFLAGS = -I${TARGET_MACHINE} -I../runtime -I../runtime/${TARGET_MACHINE}


.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
