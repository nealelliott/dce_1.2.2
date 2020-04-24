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
# Revision 1.1.4.2  1996/03/11  02:50:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:11:03  marty]
#
# Revision 1.1.4.1  1995/12/11  23:06:11  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/08/28  20:40 UTC  pare
# 	Merge service directory files
# 
# 	HP revision /main/HPDCE02/pare_MOTHRA/1  1995/08/28  13:19 UTC  pare
# 	Add test_tp_examples.ksh to DATAFILES
# 
# 	HP revision /main/HPDCE02/2  1995/06/07  18:31 UTC  pare
# 	Merge directory
# 
# 	HP revision /main/HPDCE02/pare_mothra/1  1995/06/07  17:26 UTC  pare
# 	Add test_dtsdate.ksh to DATAFILES
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  18:55 UTC  pare
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/1  1994/03/30  18:33 UTC  pare
# 
# 	HP revision /main/pare_rodan/1  1994/03/30  18:29 UTC  pare
# 	add .ksh datafiles
# 	[1995/12/11  22:37:18  root]
# 
# Revision 1.1.2.3  1992/12/31  20:35:48  htf
# 	Embedded copyright notice
# 	[1992/12/31  20:28:59  htf]
# 
# Revision 1.1.2.2  1992/11/02  15:39:13  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:42:22  root]
# 
# Revision 1.1  1992/01/19  14:38:21  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

# Build for rantest_api test program 
PROGRAMS = dtss-graph 

# compiler flags
CFLAGS	= $($(TARGET_MACHINE)_CFLAGS)
DATAFILES = test_dtsd.ksh test_dts_provider.ksh test_dtsdate.ksh test_tp_examples.ksh
ILIST	= ${PROGRAMS} ${DATAFILES}

# include directories
INCFLAGS = -I../../../time/common

# build graph program
dtss-graph_OFILES = dtss-graph.o
dtss-graph_LIBS = -ldtss_common -ldce -lm $($(TARGET_MACHINE)_LIBS)

.include <${RULES_MK}>

