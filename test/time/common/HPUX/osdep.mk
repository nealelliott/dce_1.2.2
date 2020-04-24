# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: osdep.mk,v $
# Revision 1.1.6.2  1996/03/11  02:50:22  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:56  marty]
#
# Revision 1.1.6.1  1995/12/11  23:05:18  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:37:02  root]
# 
# Revision 1.1.4.1  1994/06/10  20:51:23  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:20:07  devsrc]
# 
# Revision 1.1.2.1  1993/10/19  23:34:02  tom
# 	Bug 9201 - Define NOV1974_BUG for HPUX.
# 	[1993/10/19  23:33:20  tom]
# 
# $EndLog$

#
# lib has problems with daylight savings in 1974
#
HPUX_CFLAGS		= -DNOV1974_BUG
