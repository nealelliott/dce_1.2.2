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
# Revision 1.1.9.2  1996/03/09  23:26:13  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:51  marty]
#
# Revision 1.1.9.1  1995/12/08  00:14:42  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:56:03  root]
# 
# Revision 1.1.7.5  1993/01/03  22:35:42  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:51:30  bbelch]
# 
# Revision 1.1.7.4  1992/12/23  19:37:55  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:51:59  zeliff]
# 
# Revision 1.1.7.3  1992/12/10  22:28:12  marty
# 	Upgrade DFS to OSF/1 1.1.1.  [OT defect 5236]
# 	[1992/12/10  22:18:25  marty]
# 
# Revision 1.1.5.3  1992/10/01  20:50:27  garyf
# 	change to assembly language rule for OSF/1 R1.1
# 	[1992/09/29  20:10:29  garyf]
# 
# Revision 1.1.5.2  1992/09/29  22:47:03  marty
# 	OSC1.1 upgrade
# 	[1992/09/29  22:39:06  marty]
# 
# Revision 1.1.2.8  1992/05/26  21:41:07  garyf
# 	move common options to rpc.mk
# 	[1992/05/26  20:55:19  garyf]
# 
# Revision 1.1.2.7  1992/05/20  18:31:07  garyf
# 	cleanup compilation options
# 	[1992/05/20  18:24:39  garyf]
# 
# Revision 1.1.2.6  1992/05/11  20:56:24  mhickey
# 	Change addition of explicit path to sandbox export area to
# 	INCDIRS to addition of paths to any existing export areas
# 	for kernel include directories.
# 	Defect 3136
# 	[1992/05/11  20:52:02  mhickey]
# 
# Revision 1.1.2.5  1992/05/04  21:00:45  rsalz
# 	Set OBJECT_FORMAT for OSF/1 pmax kernels
# 	[1992/05/04  20:59:17  rsalz]
# 
# Revision 1.1.2.4  1992/04/09  18:50:29  mckeen
# 	No longer set compiler values.  Only set OBJECT_FORMAT
# 	[1992/04/09  18:30:42  mckeen]
# 
# Revision 1.1.2.3  1992/01/28  19:57:25  delgado
# 	fix missing merge
# 	[1992/01/28  19:53:29  delgado]
# 
# Revision 1.1.2.2  1992/01/28  19:26:31  treff
# 	update for new ranlib
# 	[1992/01/28  19:26:06  treff]
# 
# Revision 1.1  1992/01/19  22:13:37  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#  OSF DCE Version 1.0 

PMAX_OBJS	= dce_exc_context.o

dce_exc_context.S: dce_exc_context.s
	${RM} -f $@
	${CP} ${dce_exc_context.s:P} $@

dce_exc_context.o: dce_exc_context.S
	${ANSI_CC} -c ${CFLAGS}  -D__ASSEMBLER__ ${${TARGET_MACHINE}_SFLAGS} ${INCDIRS} ${@:.o=.S}
# old options for OSF/1 R1.0.4:
#dce_exc_context.o: dce_exc_context.S
#	${ANSI_CC} -c ${CFLAGS}  -DASSEMBLER ${${TARGET_MACHINE}_SFLAGS} ${INCDIRS} ${@:.o=.S}

# Operating-system-specific kernel RPC modules
OSF1_OBJS	= comsoc_sys.o ipnaf_sys.o pthread_sys.o uuidsys.o krpc_helper_mach.o
