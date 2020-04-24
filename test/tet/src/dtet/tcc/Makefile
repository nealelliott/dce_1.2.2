# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:49:55  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:33  marty]
#
# Revision 1.1.4.1  1995/12/11  22:50:47  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:33:17  root]
# 
# Revision 1.1.2.3  1993/08/09  19:54:50  cmckeen
# 	Changed the install directory to /test/tet/ instead of /tet/.
# 	[1993/08/09  19:33:42  cmckeen]
# 
# Revision 1.1.2.2  1993/07/20  20:36:22  cmckeen
# 	Initial DCE version
# 	[1993/07/15  16:05:38  cmckeen]
# 
# $EndLog$

PROGRAMS 	= dtcc
ILIST		= dtcc
IDIR		= /test/tet/bin/
INCFLAGS	= -I../inc
LIBS		= -ldtetapi

# generic tcc .o files
# these get compiled with TET_CFLAGS
T_OFILES_GN = again.o bld_cln.o config.o dconfig.o dist.o error.o exec.o \
        generic.o group.o journal.o lock.o misc.o modes.o remote.o rescode.o \
        resfile.o save.o scenario.o spawn.o store.o syntax.o tcc.o \
        testcase.o util.o

# these get compiled with DTET_CFLAGS
DT_OFILES_GN = d_fake.o d_tccfuncs.o 

OFILES_GN = $(T_OFILES_GN) $(DT_OFILES_GN)

# INET-specific tcc .o files; these get compiled with DTET_CFLAGS in dtet.mk
OFILES_IN = d_tcc_in.o d_tcc_bs.o d_fake_in.o

OFILES = $(OFILES_GN) $(OFILES_IN)

DTET_CFLAGS 		=
TET_CFLAGS 		= -D_POSIX_SOURCE -DNSIG=32
CFLAGS 			= ${TET_CFLAGS}
d_fake.o_CFLAGS 	= ${DTET_CFLAGS}
d_tccfuncs.o_CFLAGS 	= ${DTET_CFLAGS}
d_tcc_in.o_CFLAGS 	= ${DTET_CFLAGS}
d_tcc_bs.o_CFLAGS 	= ${DTET_CFLAGS}
d_fake_in.o_CFLAGS 	= ${DTET_CFLAGS}

.include <${RULES_MK}>
