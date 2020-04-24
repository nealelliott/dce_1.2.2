#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.10.2  1996/03/09  23:31:16  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:27  marty]
#
# Revision 1.1.10.1  1995/12/08  18:08:54  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  12:00 UTC  pare
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 	[1995/12/08  17:24:13  root]
# 
# Revision 1.1.5.3  1993/09/23  17:56:55  pare
# 	DCE1_0_3b03 Merge
# 	[1993/09/23  17:56:13  pare]
# 
# Revision 1.1.6.3  1993/09/15  17:47:00  root
# 	    Loading drop DCE1_0_3b03
# 	Revision 1.1.5.2  1993/07/15  13:31:51  truitt
# 	OT8223: removed allocate_mem.c from the build.
# 	[1993/07/15  13:30:58  truitt]
# 
# Revision 1.1.8.2  1994/05/12  15:17:40  rhw
# 	continued code cleanup & merging
# 	[1994/05/09  18:46:39  rhw]
# 
# Revision 1.1.8.1  1994/04/21  22:05:21  cbrooks
# 	CR 3231, 8353
# 	[1994/04/21  19:42:50  cbrooks]
# 
# 	Checkpoint
# 	[1994/03/28  20:22:32  cbrooks]
# 
# Revision 1.1.6.2  1993/07/14  21:23:10  tom
# 	Bug 8223 - Remove allocate_mem.o.
# 	[1993/07/01  18:41:15  tom]
# 
# Revision 1.1.4.2  1992/12/30  20:40:30  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:09:00  htf]
# 
# Revision 1.1.1.10  1993/05/04  22:38:21  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.2.3  1992/04/07  19:20:50  mckeen
# 	merged up to mainline
# 	[1992/03/31  16:38:51  mckeen]
# 
# 	Changes prs to ${PRS}
# 	[1992/03/13  15:48:25  mckeen]
# 
# Revision 1.1.2.2  1992/01/22  14:54:58  devsrc
# 	Added '-f' to ${RM} line to remove dtscp.txt.
# 	[1992/01/22  14:54:31  devsrc]
# 
# Revision 1.1  1992/01/19  15:35:16  devrcs
# 	Initial revision
# 
# $EndLog$
#

# build for CONTROL 

VPATH = ../common:../common/POSIX:${TARGET_MACHINE}:POSIX

LIBRARIES = libprs_threaded.a

PROGRAMS = dtscp

OTHERS = dtscp.bpt

# export directories

EXPDIR  = /etc/
libprs_threaded.a_EXPDIR = /usr/lib/

# install directories and files

IDIR	= /etc/
ILIST = dtscp dtscp.bpt
dtscp_IDIR = /bin/

# compiler flags

CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} 

# include directories

INCFLAGS = -I. -I../common -I../common/POSIX -I${TARGET_MACHINE} -IPOSIX

# libraries

LIBS = -ldce ${${TARGET_MACHINE}_LIBS}


# Build libprs_threaded.a

libprs_threaded.a_OFILES = prscmd.o prsrea.o

# Build dtscp program

dtscp_OFILES = dtsscp.o dtsscp_aux.o parseargs.o selectnsibinding.o\
	do_import_binding.o choosebinding.o check_status.o

dtscp_LIBS = -lprs_threaded -ldtss_common ${LIBS}

.include <${RULES_MK}>

# Dependency and rules (prs program from EXPORT directory../usr/bin)

dtscp.bpt: dtsscp.cdt
	$(CP) $> dtscp.txt
	$(PRS) dtscp.txt
	$(MV) dtscp.bin $@
	$(RM) -f dtscp.txt

dtscp : libprs_threaded.a ../common/libdtss_common.a


