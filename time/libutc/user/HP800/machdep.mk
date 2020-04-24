#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.10.2  1996/03/09  23:31:40  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:49  marty]
#
# Revision 1.1.10.1  1995/12/08  18:12:56  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  12:51 UTC  pare
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 	[1995/12/08  17:25:15  root]
# 
# Revision 1.1.7.3  1993/09/24  15:27:41  pare
# 	DCE1_0_3b03 Merge
# 	[1993/09/24  15:27:16  pare]
# 
# Revision 1.1.8.3  1993/09/15  17:48:00  root
# 	    Loading drop DCE1_0_3b03
# 	Revision 1.1.7.2  1993/07/08  19:56:03  root
# 	Initial King Kong branch
# 	[1993/07/08  19:55:37  root]
# 
# Revision 1.1.8.2  1993/06/10  19:31:38  sommerfeld
# 	Replace adjtime with dtstime in all cases.
# 	Remove special compile flags due to adjtime rewrite.
# 	Don't build dtstimed in libdce build.
# 	(from changes by Jim Truitt and Paul Smythe).
# 	[1993/06/08  19:31:12  sommerfeld]
# 
# Revision 1.1.4.2  1992/12/30  20:42:13  htf
# 	Revision 1.1.5.2  1993/03/18  14:26:16  truitt
# 	Merge Hp_dce1_0_1 with DCE1_0_2
# 	[1993/03/18  14:25:18  truitt]
# 
# 	Embedding copyright notice
# 	[1992/12/30  19:21:55  htf]
# 
# Revision 1.1.3.5  1993/03/11  22:33:54  osfrcs
# 	    Loading drop DCE1_0_2B18
# 
# Revision 1.1.2.5  1992/12/07  18:20:08  smythe
# 	changed name of adjtimed to dtstimed to avoid conflict
# 	[1992/12/07  18:19:42  smythe]
# 
# Revision 1.1.2.4  1992/10/30  19:48:11  smythe
# 	special compile flags removed due to adjtime rewrite
# 	[1992/10/30  19:45:55  smythe]
# 
# Revision 1.1.2.3  1992/07/09  17:26:54  smythe
# 	added protection for libdce make
# 	[1992/07/09  17:26:37  smythe]
# 
# Revision 1.1.2.2  1992/06/29  22:34:18  grober
# 	Makefile for hpux adjust time function.
# 	[1992/06/29  22:34:01  grober]
# 
# Revision 1.1.2.2  1992/06/26  20:05:37  smythe
# 	HP800 machine dependent make file for adjtime entry in
# 	libutc and adjtime daemon to emulate adjtime call on hpux
# 	[1992/06/26  20:04:50  smythe]
# 
# $EndLog$
#

HP800_libutc_OFILES   = dtstime.o

.if !defined(LIBDCE_LIBRARY_MAKEFILE)

HP800_PROGRAMS        = dtstimed
HP800_ILIST           = dtstimed
dtstimed_OFILES       = dtstimed.o
dtstimed_IDIR         = /bin/

.endif
