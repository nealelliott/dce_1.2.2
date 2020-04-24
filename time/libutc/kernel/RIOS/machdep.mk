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
# Revision 1.1.9.2  1996/03/09  23:31:37  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:46  marty]
#
# Revision 1.1.9.1  1995/12/08  18:12:15  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:25:04  root]
# 
# Revision 1.1.6.1  1994/05/16  17:27:33  cbrooks
# 	CR 10651
# 	[1994/05/16  15:16:11  cbrooks]
# 
# Revision 1.1.4.2  1992/12/30  20:41:51  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:21:22  htf]
# 
# Revision 1.1.2.4  1992/05/08  16:46:10  rec
# 	Added ../../common/${TARGET_MACHINE} to INCFLAGS to get the
# 	required header file.
# 	[1992/05/08  13:36:06  rec]
# 
# Revision 1.1.2.3  1992/05/06  21:55:08  comuzzi
# 	Fix defect 2790. Export symbol for libdce Makefile.
# 	[1992/05/06  21:38:06  comuzzi]
# 
# Revision 1.1.2.2  1992/02/25  19:11:25  jim
# 	Move code to time/kernel/RIOS from here.
# 	[1992/02/25  18:31:55  jim]
# 
# Revision 1.1  1992/01/19  15:35:06  devrcs
# 	Initial revision
# 
# $EndLog$
#


CFLAGS			= ${DCEPATHS} ${${TARGET_MACHINE}_CFLAGS}

RIOS_libutc_OFILES = lclcalls.o utc_gettime_user.o utc_adjtime_user.o \
		     utc_shared_map.o utc_shared_create.o dtsload.o

CC_OPT_LEVEL = -g

INCFLAGS = -I../../common -I../../common/${TARGET_MACHINE} -I../../common/POSIX -I../user

utc_adjtime_user.o_CFLAGS	= ${CFLAGS} -Dutc_adjtime=user_utc_adjtime
utc_gettime_user.o_CFLAGS	= ${CFLAGS} -Dutc_gettime=user_utc_gettime

