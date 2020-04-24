# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:49:54  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:32  marty]
#
# Revision 1.1.4.1  1995/12/11  22:50:11  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:33:09  root]
# 
# Revision 1.1.2.3  1993/08/09  19:54:47  cmckeen
# 	Changed the install directory to /test/tet/ instead of /tet/
# 	[1993/08/09  19:33:15  cmckeen]
# 
# Revision 1.1.2.2  1993/07/20  20:35:13  cmckeen
# 	Initial DCE version
# 	[1993/07/15  16:03:58  cmckeen]
# 
# $EndLog$

PROGRAMS 	= tetsyncd
ILIST 		= ${PROGRAMS}
IDIR 		= /test/tet/bin/
INCFLAGS	= -I../inc
LIBS		= -ldtetapi

# generic syncd .o files
OFILES_GN 	= syncd.o synproc.o stab.o fake.o
# INET syncd .o files
OFILES_IN 	= syncd_in.o syncd_bs.o
OFILES 		= $(OFILES_GN) $(OFILES_IN)

.include <${RULES_MK}>

