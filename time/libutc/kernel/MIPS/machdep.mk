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
# Revision 1.1.6.2  1996/03/09  23:31:30  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:40  marty]
#
# Revision 1.1.6.1  1995/12/08  18:11:42  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:24:56  root]
# 
# Revision 1.1.4.2  1992/12/30  20:41:18  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:09:46  htf]
# 
# Revision 1.1.2.2  1992/05/06  21:54:55  comuzzi
# 	Fix defect 2790. Export symbols for libdce Makefile.
# 	[1992/05/06  21:34:36  comuzzi]
# 
# Revision 1.1  1992/01/19  15:34:49  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
# Machine dependent build for common/MIPs
#
# Dependency and rules 
#
SUFFIXES: .s.o

MIPS_libutc_OFILES = utc_gettime.o utc_adjtime.o

.s.o:
	as -o ${.TARGET} ${.IMPSRC} ${INCDIRS}
