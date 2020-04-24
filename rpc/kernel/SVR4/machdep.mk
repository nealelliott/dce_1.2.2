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
# $Log: machdep.mk,v $
# Revision 1.1.8.2  1996/03/09  23:26:02  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:42  marty]
#
# Revision 1.1.8.1  1995/12/08  00:13:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:55:28  root]
# 
# Revision 1.1.6.2  1993/06/24  20:09:37  hinman
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:38:18  hinman]
# 
# Revision 1.1.3.4  1993/01/03  22:34:50  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:50:06  bbelch]
# 
# Revision 1.1.3.3  1992/12/23  19:36:04  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:46:32  zeliff]
# 
# Revision 1.1.3.2  1992/09/29  21:12:52  devsrc
# 	SNI/SVR4 merge.  OT 5373
# 	[1992/09/11  15:35:39  weisman]
# 
# $EndLog$
# 

.if (${TARGET_OS_PLATFORM} != "PYRMIPS")
OBJECTS = kern_uniqaddr.o

kern_uniqaddr.o : kern_uniqaddr.c
.endif
