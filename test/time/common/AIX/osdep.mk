# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: osdep.mk,v $
# Revision 1.1.8.2  1996/03/11  02:50:21  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:56  marty]
#
# Revision 1.1.8.1  1995/12/11  23:05:15  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:37:01  root]
# 
# Revision 1.1.6.1  1994/06/10  20:51:22  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:20:06  devsrc]
# 
# Revision 1.1.4.1  1993/10/19  23:34:00  tom
# 	Bug 9201 - Use NOV1974_BUG instead of NOV72_BUG
# 	Use ${TARGET_OS}_CFLAGS instead of ${TARGET_MACHINE}_CFLAGS.
# 	[1993/10/19  23:33:17  tom]
# 
# Revision 1.1.2.2  1993/08/02  17:14:35  tom
# 	File for test OS dependancies.
# 	[1993/08/02  16:18:10  tom]
# 
# $EndLog$

# In order to defer resolution of the loadable system calls
# for test_kernel-kernel we have to go over to the time directories...

VPATH = ../../../time/kernel/RIOS

test_kernel-kernel: kern_utctime.imp

test_kernel-kernel_LDFLAGS =  -bI:${kern_utctime.imp:P}

#
# AIX libc has a problem with daylight savings in 1974
#
AIX_CFLAGS		= -DNOV1974_BUG
