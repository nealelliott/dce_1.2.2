#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.20.1  1996/07/08  18:22:16  arvind
# 	Remove root-relative references to yaccpar and *form, which is
# 	Just Plain Wrong.
# 	Turn off YY_ERR_MSG, which causes error messages to be
# 	incomprehensible on HP-UX
# 	[1996/05/02  21:25 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/1]
#
# Revision 1.1.18.2  1996/03/09  23:12:24  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:35  marty]
# 
# Revision 1.1.18.1  1995/12/08  15:08:16  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/21  22:22 UTC  psn
# 	Fix to build on IBM.
# 
# 	HP revision /main/HPDCE02/2  1994/08/17  18:22 UTC  dtruong
# 	Merge from Hpdce02_01
# 	[1995/12/08  14:48:38  root]
# 
# Revision 1.1.16.2  1994/06/21  14:43:05  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:22:00  marrek]
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  00:20 UTC  dtruong
# 	Merge to DCE 1.1
# 
# 	HP revision /main/HPDCE01/3  1994/02/07  22:34  dceobj
# 	link in libdld.sl to get around BE problem
# 
# 	HP revision /main/HPDCE01/2  1994/02/01  22:21  tatsu_s
# 	Merged tatsu_s_ccase_fix_b1 to HPDCE01.
# 
# 	HP revision /main/HPDCE01/tatsu_s_ccase_fix_b1/2  1994/02/01  21:20  tatsu_s
# 	Disabled the variable evaluations on ':' command.
# 
# 	HP revision /main/HPDCE01/tatsu_s_ccase_fix_b1/1  1994/02/01  16:38  tatsu_s
# 	Made EVs not being echoed.
# 
# 	HP revision /main/HPDCE01/1  1994/01/25  14:33  tatsu_s
# 	Submitted tatsu_s_ccase_fix_b0.
# 	Added the capture of the critical environments in the translation rules.
# 
# Revision 1.1.14.2  1993/08/11  13:47:38  marrek
# 	July 1993 code drop.
# 	[1993/07/29  11:04:31  marrek]
# 
# Revision 1.1.8.4  1992/12/31  17:10:42  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  12:53:41  bbelch]
# 
# Revision 1.1.8.3  1992/12/04  19:59:25  marty
# 	OSC1.1.1 upgrade.
# 	[1992/12/04  19:58:59  marty]
# 
# Revision 1.1.8.2  1992/11/27  17:53:37  marrek
# 	November 1992 code drop
# 	[1992/11/23  13:01:07  marrek]
# 
# Revision 1.1.3.4  1992/06/01  20:20:29  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  18:21:45  zeliff]
# 
# Revision 1.1.3.3  1992/04/30  21:07:31  john
# 	Added SETUP_PROGRAMS.
# 	[1992/04/16  13:25:53  john]
# 
# Revision 1.1.3.2  1992/04/09  20:05:25  jim
# 	removed -Drindex=strchr from CFLAGS.
# 	[1992/04/09  18:55:55  jim]
# 
# Revision 1.1  1992/01/19  15:11:37  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.20.1 $ $Date: 1996/07/08 18:22:16 $
#

PROGRAMS	= mavcod
SETUP_PROGRAMS	= mavcod
EXPBIN_TARGETS	= mavcod
GARBAGE		= y.tab.h y.tab.c y.output lex.yy.c

EXPDIR		= /usr/bin/
YFLAGS		= -dv
HP800_CFLAGS	+= -D_NO_SETLOCALE -UYY_ERR_MSG
CFLAGS		+= -DYY_ERR_MSG -DSYS5 -DXOS ${${TARGET_MACHINE}_CFLAGS}
LFLAGS		= -I../ASN1 -DDEBUG

mavcod_EXPDIR	= /usr/bin/
mavcod_LIBS	= -ly -ll

mavcod_OFILES	= mavcod.o y.tab.o loading.o print.o analyse.o header.o mvr.o

.include <${RULES_MK}>

y.tab.c: mavcod.gr
	-@: # LC_CTYPE='${LC_CTYPE:UC:S/^POSIX$/C/}' LC_ALL='${LC_ALL:UC:S/^POSIX$/C/}' LANG='${LANG:UC:S/^POSIX$/C/}'
	${YACC} -dv ${mavcod.gr:P}

lex.yy.c: mavcod.lex
	-@: # LC_CTYPE='${LC_CTYPE:UC:S/^POSIX$/C/}' LC_ALL='${LC_ALL:UC:S/^POSIX$/C/}' LANG='${LANG:UC:S/^POSIX$/C/}'
	${LEX} ${mavcod.lex:P}

y.tab.o: y.tab.c lex.yy.c
