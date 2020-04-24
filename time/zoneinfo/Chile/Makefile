# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.2  1996/03/09  23:31:58  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:47:05  marty]
#
# 	Merge Hp_dce1_0_1 with DCE1_0_2
# 	[1993/03/18  17:45:05  truitt]
#
# Revision 1.1.5.1  1995/12/08  18:14:35  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/6  1995/01/30  22:45 UTC  sommerfeld
# 	Take advantage of new zic -o option to clearcasify the build
# 	[1995/01/27  04:07 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
# 
# 	HP revision /main/HPDCE02/5  1995/01/26  15:30 UTC  mothra
# 	Remove RM
# 
# 	HP revision /main/HPDCE02/4  1995/01/18  14:56 UTC  mothra
# 	remove target before rebuilding
# 
# 	HP revision /main/HPDCE02/3  1994/07/25  13:31 UTC  pare
# 	Add P back to ZIC line
# 
# 	HP revision /main/HPDCE02/2  1994/07/25  12:55 UTC  pare
# 	Change P to Q on ZIC line and add OTHERS
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  13:33 UTC  pare
# 	Merge hpdce02 (DCE 1.1beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/1  1994/02/01  14:52  pare
# 	merge kk and hpdce01
# 	[1995/12/08  17:26:29  root]
# 
# 	    Loading drop DCE1_0_2B18
# 	[1993/03/11  17:28:49  osfrcs]
# 
# Revision 1.1.8.3  1993/09/17  15:08:19  pare
# 	DCE1_0_3b03 Merge
# 	[1993/09/17  15:08:02  pare]
# 
# Revision 1.1.9.3  1993/09/15  17:49:13  root
# 	    Loading drop DCE1_0_3b03
# 	Revision 1.1.8.2  1993/07/08  20:35:08  root
# 	Initial King Kong branch
# 	[1993/07/08  20:34:41  root]
# 
# Revision 1.1.2.4  1992/12/30  20:43:17  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:23:30  htf]
# 
# Revision 1.1.3.3  1992/11/16  18:11:21  smythe
# 	deleted -L /dev/null parameter from zic command to avoid
# 	build problems with /dev/null in a remote file system
# 	[1992/11/16  18:07:21  smythe]
# 
# Revision 1.1.2.3  1992/11/11  17:07:41  comuzzi
# 	Fix missing timezone files.
# 	[1992/11/11  15:18:10  comuzzi]
# 
# Revision 1.1.3.2  1992/11/10  22:17:31  smythe
# 	added subdirectory on install directory pathname
# 	[1992/11/10  22:17:04  smythe]
# 
# Revision 1.1.1.2  1992/11/10  21:57:59  osfrcs
# 	    Loading P Smythe new 1.0.2 files to src/time
# 
# Revision 1.1.2.2  1992/10/20  19:07:44  comuzzi
# 	initial version
# 	[1992/10/20  18:55:31  comuzzi]
# 
# $EndLog$

# Build for Chilean timezone files

VPATH	= ../datfiles
OTHERS = Continental EasterIsland

# install directories and files
IDIR	= /etc/zoneinfo/Chile/
ILIST = Continental EasterIsland

.include <${RULES_MK}>

Continental EasterIsland : southamerica
	${RM} ${_RMFLAGS_} ${.TARGET}
	$(ZIC) -o Chile/${.TARGET} -d .. ${southamerica:P}

