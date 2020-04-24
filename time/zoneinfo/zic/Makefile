# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.7.2  1996/03/09  23:32:13  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:47:18  marty]
#
# 	DCE1_0_3b03 Merge
# 	[1993/09/17  16:39:40  pare]
# 	Revision 1.1.4.1  1994/05/12  15:17:53  rhw
# 	continued code cleanup & merging
# 	[1994/05/09  18:46:51  rhw]
#
# Revision 1.1.7.1  1995/12/09  20:58:02  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  13:54 UTC  pare
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/1  1994/01/31  19:47  pare
# 	merge kk and hpdce01
# 	[1995/12/08  17:26:44  root]
# 
# 	Merge Hp_dce1_0_1 with DCE1_0_2
# 	[1993/03/18  15:47:07  truitt]
# 
# Revision 1.1.9.3  1993/09/15  17:49:31  root
# 	    Loading drop DCE1_0_3b03
# 	Revision 1.1.8.2  1993/07/08  20:52:52  root
# 	Initial King Kong branch
# 	[1993/07/08  20:52:26  root]
# 
# Revision 1.1.4.2  1993/03/11  17:29:28  osfrcs
# 	    Loading drop DCE1_0_2B18
# 
# Revision 1.1.2.4  1992/12/30  20:43:45  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:24:48  htf]
# 
# Revision 1.1.3.3  1992/11/12  21:07:55  smythe
# 	deleted zic ILIST and IDIR since we will not ship this
# 	[1992/11/12  21:07:22  smythe]
# 
# Revision 1.1.3.2  1992/11/11  15:25:13  smythe
# 	adding new pure OSF DCE1.0.2 version to HP source base
# 	[1992/11/11  15:20:59  smythe]
# 
# Revision 1.1.1.2  1992/11/10  21:58:31  osfrcs
# 	    Loading P Smythe new 1.0.2 files to src/time
# 
# Revision 1.1.2.3  1992/10/21  19:23:43  john
# 	Added SETUP_PROGRMS to build zic in the setup pass
# 	[1992/10/21  19:22:43  john]
# 
# Revision 1.1.2.2  1992/10/20  19:09:18  comuzzi
# 	initial version
# 	[1992/10/20  19:03:23  comuzzi]
# 
# $EndLog$

# Build for SERVICE 

PROGRAMS = zic
SETUP_PROGRAMS = zic

# install directories and files

# compiler flags
CFLAGS	= ${${TARGET_MACHINE}_CFLAGS}

# include directories
INCFLAGS = -I. -I../../common -I../../common/POSIX

# libraries
#
LIBS = ${${TARGET_MACHINE}_LIBS}

INCLUDES = 

# Build for zone information compiler

zic_OFILES = zic.o scheck.o ialloc.o

.include <${RULES_MK}>
