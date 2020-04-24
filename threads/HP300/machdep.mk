#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.6.2  1996/03/09  23:30:30  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:47  marty]
#
# Revision 1.1.6.1  1995/12/07  21:46:04  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  20:59:12  root]
# 
# Revision 1.1.4.1  1994/06/09  13:34:18  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:25:20  devsrc]
# 
# Revision 1.1.2.2  1992/12/15  22:20:56  alan
# 	Insert copyright notices
# 	[1992/12/07  15:59:19  alan]
# 
# Revision 1.1  1992/01/19  14:40:02  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
# define machine dependent compiler flags

cma_assem.o :	cma_assem.s
	m4 ${ASMFLAG} $> | as -o cma_assem.o
