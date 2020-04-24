# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:49:57  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:34  marty]
#
# Revision 1.1.4.1  1995/12/11  22:52:32  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE01/1  1994/06/21  20:14 UTC  rmm
# 	change to LD_OPT_LEVEL for 10.0 (rodan)
# 
# 	HP revision /main/rmm_rodan_work/1  1994/06/21  20:11 UTC  rmm
# 	define LD_OPT_LEVEL to nothing because -Wl,-O makes the linker die with 'signal 139' error
# 	[1995/12/11  22:33:37  root]
# 
# Revision 1.1.2.3  1993/08/09  19:54:53  cmckeen
# 	Changed the install directory to /test/tet/ instead of /tet/.
# 	[1993/08/09  19:34:22  cmckeen]
# 
# Revision 1.1.2.2  1993/07/20  20:39:41  cmckeen
# 	Initial DCE version
# 	[1993/07/15  16:16:30  cmckeen]
# 
# $EndLog$

PROGRAMS	= tccd
ILIST		= ${PROGRAMS}
IDIR		= /test/tet/bin/
INCFLAGS        = -I../inc
LIBS		= -ldtetapi

.if ${ux_release}=="1000"
LD_OPT_LEVEL	=
.endif

# generic tccd .o files
OFILES_GN = tccd.o config.o exec.o etab.o log.o stcc.o tsfile.o fake.o

# INET-specific tccd .o files
OFILES_IN = tccd_in.o tccd_bs.o

OFILES = $(OFILES_GN) $(OFILES_IN)

.include <${RULES_MK}>
