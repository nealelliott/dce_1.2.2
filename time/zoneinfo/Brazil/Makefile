# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.2  1996/03/09  23:31:56  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:47:03  marty]
#
# 	Merge Hp_dce1_0_1 with DCE1_0_2
# 	[1993/03/18  17:39:27  truitt]
#
# Revision 1.1.5.1  1995/12/08  18:14:32  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/6  1995/01/30  22:45 UTC  sommerfeld
# 	Take advantage of new zic -o option to clearcasify the build
# 	[1995/01/27  04:07 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
# 
# 	HP revision /main/HPDCE02/5  1995/01/26  15:29 UTC  mothra
# 	Remove RM
# 
# 	HP revision /main/HPDCE02/4  1995/01/18  14:51 UTC  mothra
# 	remove target before rebuilding
# 
# 	HP revision /main/HPDCE02/3  1994/07/25  13:32 UTC  pare
# 	Add P back to ZIC line
# 
# 	HP revision /main/HPDCE02/2  1994/07/25  12:54 UTC  pare
# 	Change P to Q on ZIC line and add OTHERS
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  13:30 UTC  pare
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/1  1994/02/01  14:50  pare
# 	merge kk and hpdce01
# 	[1995/12/08  17:26:27  root]
# 
# 	    Loading drop DCE1_0_2B18
# 	[1993/03/11  17:28:44  osfrcs]
# 
# Revision 1.1.8.3  1993/09/17  15:04:12  pare
# 	DCE1_0_3b03 Merge
# 	[1993/09/17  15:03:43  pare]
# 
# Revision 1.1.9.3  1993/09/15  17:49:11  root
# 	    Loading drop DCE1_0_3b03
# 	Revision 1.1.8.2  1993/07/08  20:32:32  root
# 	Initial King Kong branch
# 	[1993/07/08  20:32:05  root]
# 
# Revision 1.1.2.4  1992/12/30  20:43:14  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:23:21  htf]
# 
# Revision 1.1.3.3  1992/11/16  18:11:17  smythe
# 	deleted -L /dev/null parameter from zic command to avoid
# 	build problems with /dev/null in a remote file system
# 	[1992/11/16  18:06:56  smythe]
# 
# Revision 1.1.2.3  1992/11/11  17:07:36  comuzzi
# 	Fix missing timezone files.
# 	[1992/11/11  15:14:03  comuzzi]
# 
# Revision 1.1.3.2  1992/11/10  22:09:57  smythe
# 	added subdirectory on install directory pathname
# 	[1992/11/10  22:09:15  smythe]
# 
# Revision 1.1.1.2  1992/11/10  21:57:55  osfrcs
# 	    Loading P Smythe new 1.0.2 files to src/time
# 
# Revision 1.1.2.2  1992/10/20  19:07:32  comuzzi
# 	Initial version
# 	[1992/10/20  18:54:53  comuzzi]
# 
# $EndLog$

# Build for Braziliam timezone files 

VPATH	= ../datfiles
OTHERS = East West Acre DeNoronha

# install directories and files
IDIR	= /etc/zoneinfo/Brazil/
ILIST = East West Acre DeNoronha

.include <${RULES_MK}>

East West Acre DeNoronha : southamerica
	${RM} ${_RMFLAGS_} ${.TARGET}
	$(ZIC) -o Brazil/${.TARGET} -d .. ${southamerica:P}

