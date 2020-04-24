# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
# Burlington, MA, USA
# All Rights Reserved
# (c) Copyright 1990, 1991, 1992
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.8.2  1996/03/09  23:30:49  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:05  marty]
#
# Revision 1.1.8.1  1995/12/07  21:53:02  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:02:10  root]
# 
# Revision 1.1.6.1  1994/06/09  13:36:13  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:26:36  devsrc]
# 
# Revision 1.1.3.3  1992/12/15  22:24:03  alan
# 	Insert copyright notices
# 	[1992/12/07  16:08:17  alan]
# 
# Revision 1.1.3.2  1992/09/29  21:26:09  devsrc
# 	[OT 5373]    SNI/SVR4 merge.
# 	[1992/09/17  20:39:35  sekhar]
# 
# $EndLog$
# 
# 

SVR4_INCLUDES	= cma_host.h dce_reg.h
SVR4_OFILES	= cma_vmprot.o cma_pyrmips.o 


cma_assem.o: cma_assem.s
	${_CC_}  -c  ${_CCFLAGS_} -D${TARGET_MACHINE} -DASSEMBLER $>
