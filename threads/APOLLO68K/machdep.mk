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
# Revision 1.1.6.2  1996/03/09  23:30:25  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:43  marty]
#
# Revision 1.1.6.1  1995/12/07  21:42:50  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  20:58:06  root]
# 
# Revision 1.1.4.1  1994/06/09  13:33:22  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:24:42  devsrc]
# 
# Revision 1.1.2.2  1992/12/15  22:19:49  alan
# 	Insert copyright notices
# 	[1992/12/07  15:58:32  alan]
# 
# Revision 1.1  1992/01/19  14:39:49  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# 

cma_assem.o :	cma_assem.asm
	asm $> -nl
	mv cma_assem.bin cma_assem.o
