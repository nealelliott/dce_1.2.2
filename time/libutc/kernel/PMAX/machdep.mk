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
# Revision 1.1.12.2  1996/03/09  23:31:34  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:44  marty]
#
# Revision 1.1.12.1  1995/12/08  18:11:58  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:25:00  root]
# 
# Revision 1.1.10.3  1992/12/30  20:41:35  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:19:54  htf]
# 
# Revision 1.1.10.2  1992/12/04  20:11:39  marty
# 	OSC1.1.1 upgrade.
# 	[1992/12/04  19:26:02  marty]
# 
# Revision 1.1.8.2  1992/11/23  21:41:00  weisman
# 	Remove -traditional from assembler compilations.  This ultimately
# 	enables correct SYSCALL expansion in asm.h
# 	[1992/11/23  21:40:28  weisman]
# 
# Revision 1.1.2.3  1992/05/06  21:55:05  comuzzi
# 	Fix defect 2790. Export symbol for libdce Makefile.
# 	[1992/05/06  21:36:25  comuzzi]
# 
# Revision 1.1.2.2  1992/04/07  19:21:00  mckeen
# 	Changed .PREFIX use to .TARGET in rules
# 	[1992/03/13  15:48:39  mckeen]
# 
# Revision 1.1  1992/01/19  15:34:58  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.12.2  1996/03/09  23:31:34  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:44  marty]
#
# Revision 1.1.13.2  1996/03/09  22:46:44  marty
# 	Update OSF copyright year
#
# Revision 1.1.12.1  1995/12/08  18:11:58  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:25:00  root]
#
# Revision 1.1.11.2  1995/12/08  17:25:00  root
# 	Submit OSF/DCE 1.2.1
#
# Revision 1.1.10.3  1992/12/30  20:41:35  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:19:54  htf]
#
# Revision 1.1.11.2  1992/12/30  19:19:54  htf
# 	Embedding copyright notice
#
# Revision 1.1.10.2  1992/12/04  20:11:39  marty
# 	OSC1.1.1 upgrade.
# 	[1992/12/04  19:26:02  marty]
#
# Revision 1.1.9.2  1992/12/04  19:26:02  marty
# 	OSC1.1.1 upgrade.
#
# Revision 1.1.8.2  1992/11/23  21:41:00  weisman
# 	Remove -traditional from assembler compilations.  This ultimately
# 	enables correct SYSCALL expansion in asm.h
# 	[1992/11/23  21:40:28  weisman]
#
# Revision 1.1.7.2  1992/11/23  21:40:28  weisman
# 	Remove -traditional from assembler compilations.  This ultimately
# 	enables correct SYSCALL expansion in asm.h
#
# Revision 1.1.2.3  1992/05/06  21:55:05  comuzzi
# 	Fix defect 2790. Export symbol for libdce Makefile.
# 	[1992/05/06  21:36:25  comuzzi]
#
# Revision 1.1.3.2  1992/05/06  21:36:25  comuzzi
# 	Fix defect 2790. Export symbol for libdce Makefile.
#
# Revision 1.1.2.2  1992/04/07  19:21:00  mckeen
# 	Changed .PREFIX use to .TARGET in rules
# 	[1992/03/13  15:48:39  mckeen]
#
# Revision 1.1.1.2  1992/03/13  15:48:39  mckeen
# 	Changed .PREFIX use to .TARGET in rules
#
# Revision 1.1  1992/01/19  15:34:58  devrcs
# Initial revision
#
# Revision 1.4.2.3  91/11/08  16:38:13  melman
# 	Rewrote for the new version of make - don't be scared. This is the
# 	version we have used since Snapshot 3...the makefile was old!!
# 	[91/11/08  16:35:20  melman]
# 
# Revision 1.4.2.2  91/11/05  14:59:58  melman
# 	Changed to an older version of machdep.mk we had here (from the
# 	OSF changes to dts4, ie dts4.ssbox).  The difference is that cpp
# 	must use files with a .S (upper-case) vs. a .s (lower-case).  The
# 	fixed machdep.mk copies the .s to a .S (upper-case) and then runs
# 	the cpp on it and then proceeds as it did.  ccp will not do
# 	anything to a file named with a .s (lower-case), even if there are
# 	preprocessor directives in the file.
# 	[91/11/05  14:59:09  melman]
# 
# Revision 1.1.2.2  91/07/17  12:29:25  melman
# 	Builds utc_adjtime.o and utc_gettime.o
# 	[91/07/17  12:24:23  melman]
# 
# Revision 1.1.10.2  91/06/10  14:39:43  jd
# 	      Made changes required for build 2.0.2.
# 
# Revision 1.1.8.2  91/06/05  12:55:10  devsrc
# 	      Merging up to CMA4
# 
# $EndLog$
# 

PMAX_libutc_OFILES= utc_adjtime.o utc_gettime.o

${PMAX_libutc_OFILES}: $${.TARGET:.o=.s}
	${CP} ${${.TARGET:.o=.s}:P} ${.TARGET:.o=.S}
	${_CC_} -E ${_CCFLAGS_} -D${TARGET_MACHINE} -DASSEMBLER ${.TARGET:.o=.S}> ${.TARGET:.o=.pp}
	sed '/^\#/d' ${.TARGET:.o=.pp} > ${.TARGET:.o=.s}
	${RM} ${_RMFLAGS_} ${.TARGET:.o=.S}
	${RM} ${_RMFLAGS_} ${.TARGET:.o=.pp}
	${_CC_} -c ${.TARGET:.o=.s}
	${RM} ${_RMFLAGS_} ${.TARGET:.o=.s}

