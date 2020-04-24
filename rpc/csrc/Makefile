# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/05/10  13:08:13  arvind
# 	Drop 1 of DCE 1.2.2 to OSF
#
# 	HP revision /main/DCE_1.2/2  1996/03/14  18:48 UTC  psn
# 	Merge endgame fixes at HP.
#
# 	HP revision /main/DCE_1.2/psn_1.2_endgame/1  1996/02/28  21:45 UTC  psn
# 	Fix OT13348 as done during OSF 1.2.1 Endgame.
#
# 	HP revision /main/DCE_1.2/1  1996/01/03  19:35 UTC  psn
# 	Merge fix for libdld.sl 10.0 dependency.
#
# 	[1995/07/27  21:56 UTC  kline_s  /main/HPDCE02/7]
#
# 	Fix 10.0-specific dependency for libdld.sl.
# 	[1995/07/27  18:48 UTC  kline_s  /main/HPDCE02/kline_s_mothra_10/5]
#
# 	merge back ux_release changes
# 	[1995/07/14  19:51 UTC  rmm  /main/HPDCE02/6]
#
# 	Change 1000 cond to work for Walnut Creek
# 	[1995/06/29  22:37 UTC  rmm  /main/HPDCE02/rmm_wc_work/1]
#
# 	Add SUBDIRS tables and methods.
# 	[1995/03/30  23:32 UTC  kline_s  /main/HPDCE02/5]
#
# 	Add SUBDIRS tables and methods.
# 	[1995/03/29  19:10 UTC  kline_s  /main/HPDCE02/kline_s_mothra_10/4]
#
# 	I18N RPC changes required for 10.0
# 	[1995/02/16  22:26 UTC  kline_s  /main/HPDCE02/4]
#
# 	Merge changes from mainline into branch.
# 	[1995/02/10  21:15 UTC  kline_s  /main/HPDCE02/kline_s_mothra_10/3]
#
# 	[1995/02/10  21:14 UTC  kline_s  /main/HPDCE02/kline_s_mothra_10/3]
#
# 	Use special OS naming for 9.0 code_set_registry.txt file (instead
# 	of for the 10.0 version.
# 	[1995/02/10  21:05 UTC  kline_s  /main/HPDCE02/kline_s_mothra_10/2]
#
# 	Allow for 10.x/9.x OS differentiations of code_set_registry.txt files
# 	Defer actual creation of code_set_registry.db until install/customize
# 	phase.  Needed to avoid cross OS version build problems in using csrc.
# 	[1995/02/09  22:53 UTC  kline_s  /main/HPDCE02/kline_s_mothra_10/1]
#
# 	Remove duplicate build rules for message catalog files.
# 	Don't assume `.' is in $PATH.
# 	[1995/01/27  04:14 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
#
# 	Enable RPC I18N functionality
# 	[1994/12/23  00:26 UTC  kline_s  /main/HPDCE02/2]
#
# 	Enable I18N RPC features for Mothra.
# 	[1994/12/22  21:39 UTC  kline_s  /main/HPDCE02/kline_s_mothra_latest/1]
#
# 	Fix yacc and lex
# 	[1994/07/12  15:26 UTC  mothra  /main/HPDCE02/1]
#
# Revision 1.1.2.3  1994/06/10  20:53:27  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:59:36  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  18:36:38  mori_m
# 	CR 10579  Added dcecsr.cat_IDIR to install dcecsr.cat to /nls/msg/en_US.ASCII/
# 	[1994/05/10  18:17:56  mori_m]
# 
# Revision 1.1.2.1  1994/02/08  21:44:13  mori_m
# 	CR 9701:  Initial submission for RPC runtime I18N support
# 	[1994/02/08  21:43:50  mori_m]
# 
# $EndLog$

VPATH    =./csr/${TARGET_MACHINE}

PROGRAMS = csrc
OTHERS   = code_set_registry.txt

.if ${TARGET_MACHINE} == "HP800" && ${ux_release} < 1000
OTHERS   = code_set_registry_9.txt
.endif

# iconv converter tables/methods are not built for 9.0.x versions
.if ${TARGET_MACHINE} == "HP800" && ${ux_release} >= 1000
EXPLIB_SUBDIRS = methods
SUBDIRS = methods tables
.endif

ILIST = ${PROGRAMS} dcecsr.cat ${OTHERS} 
IDIR = /bin/
dcecsr.cat_IDIR	= /nls/msg/en_US.ASCII/

code_set_registry.txt_IDIR=/nls/
.if ${TARGET_MACHINE} == "HP800" && ${ux_release} < 1000
code_set_registry_9.txt_IDIR=/nls/
.endif
#
#
# Add /usr/lib to possible paths so that libdld.sl will be
# resolved at runtime.

.if ${TARGET_MACHINE} == "HP800" && ${ux_release} >= 1000
csrc_LDFLAGS	= -Wl,-a,default,+b,/usr/lib
.endif

INCFLAGS	= -I${TARGET_MACHINE}
EXPDIR = /usr/include/dce/
LIBS = -ldce
INCLUDES	= csrc.h dcecsrmsg.h
SAMSFILES	= csr.sams
SAMIDL_INCLUDES	= dcecsrmsg.h
SAMS_TARGETS	= dcecsrmsg.h dcecsrsvc.c dcecsrmsg.c dcecsr.cat

csrc_OFILES	= csrc.o csrcgram.o csrclex.o dcecsrmsg.o

#CFLAGS  = ${${TARGET_MACHINE}_CFLAGS} -g
YFLAGS = ${${TARGET_MACHINE}_YFLAGS} -d

.include <${RULES_MK}>

# for parallel make
.ORDER: csrcgram.h csrcgram.c y.tab.c y.tab.h
csrcgram.c csrcgram.h:	csrcgram.y 
	-@: # LC_CTYPE='${LC_CTYPE:UC:S/^POSIX$/C/}' LC_ALL='${LC_ALL:UC:S/^POSIX$/C/}' LANG='${LANG:UC:S/^POSIX$/C/}'
	${RM} ${_RMFLAGS_} y.tab.c y.tab.h csrcgram.c csrcgram.h
	${YACC} ${YFLAGS} ${_YFLAGS_} ${.ALLSRC:N/usr/lib/yaccpar}
	${MV} y.tab.c csrcgram.c
	${MV} y.tab.h csrcgram.h

csrclex.c:	csrclex.l 
	-@: # LC_CTYPE='${LC_CTYPE:UC:S/^POSIX$/C/}' LC_ALL='${LC_ALL:UC:S/^POSIX$/C/}' LANG='${LANG:UC:S/^POSIX$/C/}'
	${RM} ${_RMFLAGS_} ${.TARGET}
	$(LEX) ${_LFLAGS_} ${.ALLSRC:N/usr/lib/lex/*form}
	${MV} lex.yy.c csrclex.c
csrclex.o:	csrcgram.h 
${csrc_OFILES}:	csrc.h 

dcecsrmac.c:	csr.sams
	${SAMS} -ot ${.ALLSRC}
