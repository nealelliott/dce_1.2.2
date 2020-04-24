# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: osdep.mk,v $
# Revision 1.1.35.2  1996/03/11  02:50:24  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:58  marty]
#
# Revision 1.1.35.1  1995/12/11  23:05:22  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:37:03  root]
# 
# Revision 1.1.33.1  1994/06/10  20:51:24  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:20:09  devsrc]
# 
# Revision 1.1.31.1  1993/10/19  23:34:04  tom
# 	Bug 9201 - Use NOV1974_BUG instead of NOV72_BUG
# 	Use ${TARGET_OS}_CFLAGS instead of ${TARGET_MACHINE}_CFLAGS.
# 	[1993/10/19  23:33:26  tom]
# 
# Revision 1.1.2.2  1993/08/02  17:14:44  tom
# 	File for test OS dependancies.
# 	[1993/08/02  16:10:41  tom]
# 
# $EndLog$

#
# OSF/1 lib has problems with daylight savings in 1974
#
OSF1_CFLAGS		= -DNOV1974_BUG
