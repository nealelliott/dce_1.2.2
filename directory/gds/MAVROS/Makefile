#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.18.1  1996/07/08  18:22:26  arvind
# 	Remove root-relative references to yaccpar and *form, which is
# 	Just Plain Wrong.
# 	Turn off YY_ERR_MSG, which causes error messages to be
# 	incomprehensible on HP-UX
# 	[1996/05/02  21:28 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/1]
#
# Revision 1.1.16.2  1996/03/09  23:12:27  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:38  marty]
# 
# Revision 1.1.16.1  1995/12/08  15:09:01  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/21  22:16 UTC  psn
# 	Fix build problems on IBM for DCE 1.2.1 drop.
# 
# 	HP revision /main/HPDCE02/2  1994/08/17  18:29 UTC  dtruong
# 	Merge from Hpdce02_01
# 	[1995/12/08  14:48:57  root]
# 
# Revision 1.1.14.2  1994/06/21  14:43:06  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:22:02  marrek]
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  00:23 UTC  dtruong
# 	Merge to DCE 1.1
# 
# 	HP revision /main/HPDCE01/3  1994/02/07  22:34  dceobj
# 	change CCOPTS to workaround BE problem
# 
# 	HP revision /main/HPDCE01/2  1994/02/01  22:23  tatsu_s
# 	Merged tatsu_s_ccase_fix_b1 to HPDCE01.
# 
# 	HP revision /main/HPDCE01/tatsu_s_ccase_fix_b1/2  1994/02/01  21:20  tatsu_s
# 	Disabled the variable evaluations on ':' command.
# 
# 	HP revision /main/HPDCE01/tatsu_s_ccase_fix_b1/1  1994/02/01  16:41  tatsu_s
# 	Made EVs not being echoed.
# 
# 	HP revision /main/HPDCE01/1  1994/01/25  14:33  tatsu_s
# 	Submitted tatsu_s_ccase_fix_b0.
# 	Added the capture of the critical environments in the translation rules.
# 
# Revision 1.1.12.2  1993/08/11  13:59:08  marrek
# 	July 1993 code drop.
# 	[1993/07/29  11:06:20  marrek]
# 
# Revision 1.1.9.4  1992/12/31  17:11:18  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  12:54:22  bbelch]
# 
# Revision 1.1.9.3  1992/12/07  13:18:51  zeliff
# 	Removed duplicate OSF copyright markers
# 	[1992/12/06  00:42:39  zeliff]
# 
# Revision 1.1.9.2  1992/12/04  19:39:45  marty
# 	OSC1.1.1 upgrade.
# 	[1992/12/04  19:24:00  marty]
# 
# Revision 1.1.5.2  1992/11/17  19:51:42  marty
# 	OSC1.1.1 upgrade.
# 	[1992/11/17  13:35:17  marty]
# 
# Revision 1.1.3.4  1992/06/01  20:21:27  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  18:33:04  zeliff]
# 
# Revision 1.1.3.3  1992/04/30  21:11:06  john
# 	Added SETUP_PROGRAMS.
# 	[1992/04/16  13:26:27  john]
# 
# Revision 1.1.3.2  1992/04/09  20:05:47  jim
# 	Changes for AIX 3.2 ansi problems.
# 	[1992/04/09  19:24:13  jim]
# 
# Revision 1.1  1992/01/19  15:11:52  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.18.1 $ $Date: 1996/07/08 18:22:26 $
#

PROGRAMS	= mavros
SETUP_PROGRAMS	= mavros
EXPBIN_TARGETS	= mavros
GARBAGE		= lex.yy.c y.output y.tab.c y.tab.h

EXPDIR		= /usr/bin/
YFLAGS		= -dv
HP800_CFLAGS	+= -D_NO_SETLOCALE -UYY_ERR_MSG
CFLAGS		+= -DYY_ERR_MSG -DSYS5 -DXOS ${${TARGET_MACHINE}_CFLAGS}

mavros_EXPDIR	= /usr/bin/
mavros_LIBS	= -ly -ll

mavros_OFILES	= \
		y.tab.o		mavrospro.o	mavrosgen.o \
		mavros.o \
		mavrosinout.o	mavroscpy.o 	mavrosbase.o \
		mavrosprt.o	mavrosincl.o	mavroslw.o 


.include <${RULES_MK}>

lex.yy.c: mavros.lex
	-@: # LC_CTYPE='${LC_CTYPE:UC:S/^POSIX$/C/}' LC_ALL='${LC_ALL:UC:S/^POSIX$/C/}' LANG='${LANG:UC:S/^POSIX$/C/}'
	${LEX} ${mavros.lex:P}

y.tab.c: mavros.gr
	-@: # LC_CTYPE='${LC_CTYPE:UC:S/^POSIX$/C/}' LC_ALL='${LC_ALL:UC:S/^POSIX$/C/}' LANG='${LANG:UC:S/^POSIX$/C/}'
	${YACC} -dv ${mavros.gr:P}

y.tab.o: y.tab.c lex.yy.c


