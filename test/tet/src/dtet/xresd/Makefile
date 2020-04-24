# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:50:00  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:36  marty]
#
# Revision 1.1.4.1  1995/12/11  22:54:22  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:33:56  root]
# 
# Revision 1.1.2.3  1993/08/09  19:54:55  cmckeen
# 	Change the install directory to /test/tet/ instead of /tet/.
# 	[1993/08/09  19:34:56  cmckeen]
# 
# Revision 1.1.2.2  1993/07/20  20:43:09  cmckeen
# 	Initial DCE version
# 	[1993/07/15  16:25:22  cmckeen]
# 
# $EndLog$

PROGRAMS	= tetxresd
ILIST		= ${PROGRAMS}
IDIR		= /test/tet/bin/
INCFLAGS	= -I../inc/
LIBS		= -ldtetapi

OFILES_GN 	= xresd.o cfname.o codesfile.o tfproc.o xresproc.o xtab.o \
		  fake.o rescode.o

# INET xresd .o files
OFILES_IN 	= xresd_in.o xresd_bs.o

OFILES = $(OFILES_GN) $(OFILES_IN)

.include <${RULES_MK}>
