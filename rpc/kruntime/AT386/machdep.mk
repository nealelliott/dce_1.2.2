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
# Revision 1.1.8.2  1996/03/09  23:26:05  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:43  marty]
#
# Revision 1.1.8.1  1995/12/08  00:13:43  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:55:30  root]
# 
# Revision 1.1.6.1  1994/06/10  20:54:00  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:59:47  devsrc]
# 
# Revision 1.1.4.1  1993/09/01  14:29:36  delgado
# 	DFS 486 port
# 	[1993/09/01  14:23:24  delgado]
# 
# $EndLog$
#
#  OSF DCE Version 1.0 

AT386_OBJS	= dce_exc_context.o

dce_exc_context.S: dce_exc_context.s
	${RM} ${_RMFLAGS_} $@
	${CP} ${dce_exc_context.s:P} $@

dce_exc_context.o: dce_exc_context.S
	${_CC_} -c ${_CCFLAGS_}  -D__ASSEMBLER__ ${${TARGET_MACHINE}_SFLAGS} ${INCDIRS} ${@:.o=.S}

# Operating-system-specific kernel RPC modules
OSF1_OBJS	= comsoc_sys.o ipnaf_sys.o pthread_sys.o uuidsys.o \
                  krpch_conf.o krpc_helper_mach.o  
