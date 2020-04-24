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
# $Log: machdep.mk,v $
# Revision 1.1.8.2  1996/03/09  23:31:25  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:36  marty]
#
# Revision 1.1.8.1  1995/12/08  18:11:19  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:24:50  root]
# 
# Revision 1.1.6.2  1993/06/24  21:31:00  hinman
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:49:01  hinman]
# 
# Revision 1.1.3.3  1992/12/30  20:41:02  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:09:35  htf]
# 
# Revision 1.1.3.2  1992/09/29  21:27:31  devsrc
# 	[OT 5373]  SNI/SVR4 Merge.
# 	[1992/09/14  20:04:31  sekhar]
# 
# $EndLog$
# 

.if exists(${TARGET_MACHINE}/${TARGET_OS_PLATFORM}/machdep.mk)
VPATH = ${TARGET_MACHINE}:${TARGET_MACHINE}/${TARGET_OS_PLATFORM}
.include "${TARGET_MACHINE}/${TARGET_OS_PLATFORM}/machdep.mk"
.endif
