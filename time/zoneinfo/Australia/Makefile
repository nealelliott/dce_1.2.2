# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.2  1996/03/09  23:31:55  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:47:02  marty]
#
# 	Merge Hp_dce1_0_1 with DCE1_0_2
# 	[1993/03/18  16:20:51  truitt]
#
# Revision 1.1.5.1  1995/12/08  18:14:31  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/6  1995/01/30  22:45 UTC  sommerfeld
# 	Take advantage of new zic -o option to clearcasify the build
# 	[1995/01/27  04:07 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
# 
# 	HP revision /main/HPDCE02/5  1995/01/26  15:28 UTC  mothra
# 	Remove RM
# 
# 	HP revision /main/HPDCE02/4  1995/01/18  14:50 UTC  mothra
# 	remove target before rebuilding
# 
# 	HP revision /main/HPDCE02/3  1994/07/25  13:33 UTC  pare
# 	Add P back to ZIC line
# 
# 	HP revision /main/HPDCE02/2  1994/07/25  11:58 UTC  mothra
# 	Change P to Q and add OTHERS
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  13:29 UTC  pare
# 	Merge hpdce02 (DCE1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/1  1994/02/01  14:49  pare
# 	merge kk and hpdce01
# 	[1995/12/08  17:26:26  root]
# 
# 	    Loading drop DCE1_0_2B18
# 	[1993/03/11  17:28:41  osfrcs]
# 
# Revision 1.1.8.3  1993/09/17  15:01:54  pare
# 	DCE1_0_3b03 Merge
# 	[1993/09/17  15:01:23  pare]
# 
# Revision 1.1.9.3  1993/09/15  17:49:10  root
# 	    Loading drop DCE1_0_3b03
# 	Revision 1.1.8.2  1993/07/08  20:31:20  root
# 	Initial King Kong branch
# 	[1993/07/08  20:30:53  root]
# 
# Revision 1.1.3.3  1992/11/16  18:11:15  smythe
# 	deleted -L /dev/null parameter from zic command to avoid
# 	build problems with /dev/null in a remote file system
# 	[1992/11/16  18:06:43  smythe]
# 
# Revision 1.1.2.5  1992/12/30  20:43:12  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:23:16  htf]
# 
# Revision 1.1.2.4  1992/12/07  13:21:21  zeliff
# 	Removed duplicate OSF copyright markers
# 	[1992/12/06  00:45:25  zeliff]
# 
# Revision 1.1.2.3  1992/11/11  17:07:34  comuzzi
# 	Fix missing timezone files.
# 	[1992/11/11  15:13:32  comuzzi]
# 
# Revision 1.1.3.2  1992/11/10  22:05:53  smythe
# 	added subdirectory to installation directory path
# 	[1992/11/10  22:05:16  smythe]
# 
# Revision 1.1.1.2  1992/11/10  21:57:52  osfrcs
# 	    Loading P Smythe new 1.0.2 files to src/time
# 
# Revision 1.1.2.2  1992/10/20  19:07:27  comuzzi
# 	Initial version
# 	[1992/10/20  18:54:36  comuzzi]
# 
# $EndLog$

# Build for Austrailian timezone files 

VPATH	= ../datfiles

OTHERS = Tasmania Queensland North West South Victoria NSW Yancowinna LHI

# install directories and files
IDIR	= /etc/zoneinfo/Australia/
ILIST = Tasmania Queensland North West South Victoria NSW Yancowinna LHI

.include <${RULES_MK}>

Tasmania Queensland North West South Victoria NSW Yancowinna LHI : australasia
	${RM} ${_RMFLAGS_} ${.TARGET}
	$(ZIC) -o Australia/${.TARGET} -d .. ${australasia:P}

