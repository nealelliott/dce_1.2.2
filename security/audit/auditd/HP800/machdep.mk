# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:27:00  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:31  marty]
#
# Revision 1.1.4.1  1995/12/08  17:37:54  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:17:28  root]
# 
# Revision 1.1.2.2  1993/12/22  13:52:26  zeliff
# 	Forgot to add copyright message
# 	[1993/12/22  13:50:02  zeliff]
# 
# Revision 1.1.2.1  1993/12/22  13:39:44  zeliff
# 	Added -lM to resolve reference to 'fabs' from DTS
# 	portion of libdce.
# 	[1993/12/22  13:39:28  zeliff]
# 
# $EndLog$

LIBS	+= -lM
