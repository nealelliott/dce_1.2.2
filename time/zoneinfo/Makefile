# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/09  23:31:59  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:47:06  marty]
#
# Revision 1.1.6.1  1995/12/08  18:14:36  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/01/30  22:45 UTC  sommerfeld
# 	Take advantage of new zic -o option to clearcasify the build
# 	[1995/01/27  04:07 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
# 
# 	HP revision /main/HPDCE02/2  1994/07/25  13:24 UTC  mothra
# 	add OTHERS
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  13:34 UTC  pare
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/1  1994/02/01  14:54  pare
# 	merge kk and hpdce01
# 	[1995/12/08  17:26:30  root]
# 
# Revision 1.1.8.3  1993/09/17  14:59:20  pare
# 	DCE1_0_3b03 Merge
# 	[1993/09/17  14:58:38  pare]
# 
# Revision 1.1.9.3  1993/09/15  17:49:14  root
# 	    Loading drop DCE1_0_3b03
# 	Revision 1.1.8.2  1993/07/08  20:36:24  root
# 	Initial King Kong branch
# 	[1993/07/08  20:35:54  root]
# 
# Revision 1.1.6.2  1993/03/18  16:17:27  truitt
# 	Merge Hp_dce1_0_1 with DCE1_0_2
# 	[1993/03/18  16:16:18  truitt]
# 
# Revision 1.1.4.2  1993/03/11  17:28:51  osfrcs
# 	    Loading drop DCE1_0_2B18
# 
# Revision 1.1.3.3  1992/11/16  18:11:13  smythe
# 	deleted -L /dev/null parameter from zic command to avoid
# 	build problems with /dev/null in a remote file system
# 	[1992/11/16  18:07:36  smythe]
# 
# Revision 1.1.2.7  1993/01/21  16:28:52  tom
# 	Bug 6946 - install Navajo time zone (same as US/Mountain).
# 	[1993/01/20  21:54:45  tom]
# 
# Revision 1.1.2.6  1992/12/30  20:43:20  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:23:35  htf]
# 
# Revision 1.1.2.5  1992/11/11  17:07:44  comuzzi
# 	Install missing timezone files.
# 	[1992/11/09  19:29:45  comuzzi]
# 
# Revision 1.1.3.2  1992/11/10  22:29:59  smythe
# 	added missing country subdirectories to the subdir list
# 	[1992/11/10  22:29:14  smythe]
# 
# Revision 1.1.2.4  1992/10/23  12:55:29  comuzzi
# 	Build and install timezone files
# 	[1992/10/22  17:28:35  comuzzi]
# 
# Revision 1.1.2.3  1992/10/21  19:23:39  john
# 	Added SETUP_SUBDIRS to build zic in the zic subdir.
# 	[1992/10/21  19:22:12  john]
# 
# 	initial version
# 	[1992/10/20  18:56:00  comuzzi]
# 
# Revision 1.1.1.2  1992/11/10  22:09:33  osfrcs
# 	    Loading P Smythe new 1.0.2 files to src/time
# 
# $EndLog$

# Build for zoneinfo 

VPATH	= datfiles

SUBDIRS = zic Australia Brazil Canada Chile Mexico SystemV US
SETUP_SUBDIRS = zic

# export directories

OTHERS = Cuba Jamaica Navajo Egypt Libya PRC ROC Hongkong Iran Israel \
        Japan ROK Singapore NZ GB-Eire WET Iceland MET Poland EET \
        Turkey W-SU CET GMT GMT-12 GMT-11 GMT-10 GMT-9 GMT-8 GMT-7 \
        GMT-6 GMT-5 GMT-4 GMT-3 GMT-2 GMT-1 GMT-0 GMT+0 GMT+1 GMT+2 \
        GMT+3 GMT+4 GMT+5 GMT+6 GMT+7 GMT+8 GMT+9 GMT+10 GMT+11 \
        GMT+12 GMT+13 GMT0 GMT1 GMT2 GMT3 GMT4 GMT5 GMT6 GMT7 GMT8 \
        GMT9 GMT10 GMT11 GMT12 GMT13 UTC UCT Universal Greenwich Zulu \
        posixrules

# install directories and files
IDIR  = /etc/zoneinfo/
ILIST = Cuba Jamaica Navajo Egypt Libya PRC ROC Hongkong Iran Israel \
        Japan ROK Singapore NZ GB-Eire WET Iceland MET Poland EET \
        Turkey W-SU CET GMT GMT-12 GMT-11 GMT-10 GMT-9 GMT-8 GMT-7 \
        GMT-6 GMT-5 GMT-4 GMT-3 GMT-2 GMT-1 GMT-0 GMT+0 GMT+1 GMT+2 \
        GMT+3 GMT+4 GMT+5 GMT+6 GMT+7 GMT+8 GMT+9 GMT+10 GMT+11 \
        GMT+12 GMT+13 GMT0 GMT1 GMT2 GMT3 GMT4 GMT5 GMT6 GMT7 GMT8 \
        GMT9 GMT10 GMT11 GMT12 GMT13 UTC UCT Universal Greenwich Zulu \
        posixrules

.include <${RULES_MK}>

Cuba Jamaica Navajo : northamerica
	${RM} ${_RMFLAGS_} ${.TARGET}
	$(ZIC) -o ${.TARGET} -d . ${northamerica:P}

posixrules : northamerica
	${RM} ${_RMFLAGS_} ${.TARGET}
	$(CP) US/Eastern ${.TARGET}

Egypt Libya : africa
	${RM} ${_RMFLAGS_} ${.TARGET}
	$(ZIC) -o ${.TARGET} -d . ${africa:P}

PRC ROC Hongkong Iran Israel Japan ROK Singapore : asia
	${RM} ${_RMFLAGS_} ${.TARGET}
	$(ZIC) -o ${.TARGET} -d . ${asia:P}

NZ : australasia
	${RM} ${_RMFLAGS_} ${.TARGET}
	$(ZIC) -o ${.TARGET} -d . ${australasia:P}

GB-Eire WET Iceland MET Poland EET Turkey W-SU CET : europe
	${RM} ${_RMFLAGS_} ${.TARGET}
	$(ZIC) -o ${.TARGET} -d . ${europe:P}

GMT GMT-12 GMT-11 GMT-10 GMT-9 GMT-8 GMT-7 GMT-6 GMT-5 GMT-4 GMT-3 \
GMT-2 GMT-1 GMT-0 GMT+0 GMT+1 GMT+2 GMT+3 GMT+4 GMT+5 GMT+6 GMT+7 \
GMT+8 GMT+9 GMT+10 GMT+11 GMT+12 GMT+13 GMT0 GMT1 GMT2 GMT3 GMT4 \
GMT5 GMT6 GMT7 GMT8 GMT9 GMT10 GMT11 GMT12 GMT13 UTC UCT Universal \
Greenwich Zulu : etcetera
	${RM} ${_RMFLAGS_} ${.TARGET}
	$(ZIC) -o ${.TARGET} -d . ${etcetera:P}
