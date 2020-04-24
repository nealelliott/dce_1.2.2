#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.8.3  1996/03/09  23:30:47  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:02  marty]
#
# Revision 1.1.8.2  1995/12/07  21:50:31  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/DCE1_1_WP/1  1995/04/26  16:55 UTC  jrr
# 	Changes to allow valid AIX core dumps.
# 	[1995/12/07  21:00:49  root]
# 
# Revision 1.1.4.1  1994/06/09  13:35:58  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:26:29  devsrc]
# 
# Revision 1.1.2.2  1992/12/15  22:23:41  alan
# 	Insert copyright notices
# 	[1992/12/07  16:03:11  alan]
# 
# Revision 1.1  1992/01/19  14:38:41  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# 

RIOS_DATAFILES	= cma_host.h

RIOS_OFILES	= getasyncinfo.o cma_core.o

cma_assem.o :	cma_assem.s
	m4 /usr/include/sys/asdef.s $> > file1.s
	as -o cma_assem.o file1.s
	rm file1.s
#  DEC/CMS REPLACEMENT HISTORY, Element MACHDEP.MK
#  *4    10-SEP-1991 12:21:52 BUTENHOF "IBM CMA5 reverse drop integration"
#  *3    12-JUN-1991 13:07:17 BUTENHOF "Upgrade to latest IBM versions"
#  *2    31-JAN-1991 13:00:28 SCALES "Convert to stream format for ULTRIX build"
#  *1    15-JAN-1991 22:27:38 BUTENHOF "RIOS-specific makefile"
#  DEC/CMS REPLACEMENT HISTORY, Element MACHDEP.MK
