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
# Revision 1.1.11.2  1996/03/09  23:31:26  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:37  marty]
#
# Revision 1.1.11.1  1995/12/08  18:11:23  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/14  22:43 UTC  psn
# 	Visit both kernel and user mode as it is needed to build tests on DCE 1.2.1 AIX
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  12:45 UTC  pare
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/2  1994/02/02  16:14  pare
# 	merge kk and hpdce01
# 
# 	HP revision /main/HPDCE01/2  1994/02/02  15:49  pare
# 	merge kk and hpdce01
# 
# 	HP revision /main/HPDCE01/2  1994/02/02  15:47  pare
# 	merge kk and hpdce01
# 	[1995/12/08  17:24:51  root]
# 
# Revision 1.1.5.3  1993/09/24  15:23:09  pare
# 	DCE1_0_3b03 Merge
# 	[1993/09/24  15:19:24  pare]
# 
# 	Remove data subdirs.
# 	[1993/05/12  17:47:37  jd]
# 
# Revision 1.1.6.3  1993/09/15  17:47:33  root
# 	    Loading drop DCE1_0_3b03
# 	Revision 1.1.5.2  1993/07/08  19:50:46  root
# 	Initial King Kong branch
# 	[1993/07/08  19:50:04  root]
# 
# Revision 1.1.9.2  1993/05/24  20:51:31  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:19:47  cjd]
# 
# Revision 1.1.3.2  1993/03/24  18:26:01  truitt
# 	Corrected SUBDIRS assignment to reflect DTS_LIBUTC_MODE, instead
# 	of using 'kernel user' automatically.
# 	[1993/03/24  18:25:18  truitt]
# 
# Revision 1.1.1.8  1993/03/11  22:33:17  osfrcs
# 	    Loading drop DCE1_0_2B18
# 
# Revision 1.1.5.5  1993/02/01  19:52:06  tom
# 	Always go in to both user and kernel subdirs.  This is so the
# 	test will have both versions to link against.
# 	[1993/02/01  18:31:02  tom]
# 
# Revision 1.1.5.2  1992/12/30  20:41:09  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:09:41  htf]
# 
# Revision 1.1  1992/01/19  15:35:07  devrcs
# 	Initial revision
# 
# $EndLog$
#

SUBDIRS = kernel user

DATA_SUBDIRS = ${SUBDIRS}
EXPINC_SUBDIRS = ${SUBDIRS}
EXPLIB_SUBDIRS = ${SUBDIRS}

.include <${RULES_MK}>

