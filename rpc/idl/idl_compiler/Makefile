# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.26.1  1996/10/03  14:40:13  arvind
# 	Remove spurious dependancies.
# 	[1996/09/17  23:25 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
#
# Revision 1.1.24.2  1996/03/09  23:25:44  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:25  marty]
# 
# Revision 1.1.24.1  1995/12/07  22:11:57  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/2  1995/11/21  21:49 UTC  jrr
# 	Fix SETUP lines.
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:22 UTC  psn
# 	Expand "*form" on dependency line for OSF DCE 1.2.1 drop to be built on IBM
# 
# 	HP revision /main/HPDCE02/3  1995/09/06  18:46 UTC  gaz
# 	disable generation of DMS code into stubs for Mothra-patch and Walnut Creek
# 
# 	HP revision /main/HPDCE02/gaz_mothra_fcompat/1  1995/09/06  14:16 UTC  gaz
# 	Mothra/WC DMS fcompat
# 
# 	HP revision /main/HPDCE02/2  1995/03/05  14:00 UTC  gaz
# 	Merge DMS changes to mainline
# 
# 	HP revision /main/HPDCE02/lmm_mothra_instr/2  1995/02/01  23:56 UTC  lmm
# 	change internal #ifdef macro to _HP_DCE_INSTR
# 
# 	HP revision /main/HPDCE02/lmm_mothra_instr/1  1994/11/01  23:51 UTC  lmm
# 	adding DMS support
# 
# 	HP revision /main/HPDCE02/1  1994/08/03  16:31 UTC  tatsu_s
# 	Merged mothra up to dce 1.1.
# 
# 	HP revision /main/HPDCE01/4  1994/02/03  15:57  lmm
# 	Merge changes back to HPDCE01
# 
# 	HP revision /main/HPDCE01/lmm_fix_idl_Makefile/1  1994/02/03  00:05  lmm
# 	added .elif line to avoid setting INCDIRS
# 
# 	HP revision /main/HPDCE01/3  1994/02/01  22:30  tatsu_s
# 	Merged tatsu_s_ccase_fix_b1 to HPDCE01.
# 
# 	HP revision /main/HPDCE01/tatsu_s_ccase_fix_b1/2  1994/02/01  21:20  tatsu_s
# 	Disabled the variable evaluations on ':' command.
# 
# 	HP revision /main/HPDCE01/tatsu_s_ccase_fix_b1/1  1994/02/01  16:47  tatsu_s
# 	Made EVs not being echoed.
# 
# 	HP revision /main/HPDCE01/2  1994/01/24  22:21  tatsu_s
# 	Submitted tatsu_s_ccase_fix_b0.
# 	Added the capture of the critical environments in the translation rules.
# 
# 	HP revision /main/HPDCE01/1  1994/01/24  19:40  lmm
# 	merge kk and hpdce01
# 	[1995/12/07  21:09:22  root]
# 
# Revision 1.1.8.6  1993/11/18  19:06:54  sommerfeld
# 	Move setup pass build to separate directory.
# 	Don't link this against libc_r (requires parallel change to osf.prog.mk).
# 	[1993/11/18  19:03:12  sommerfeld]
# 
# Revision 1.1.8.5  1993/10/19  22:12:21  sommerfeld
# 	Add a bunch of missing rm -f's.
# 	[1993/10/19  21:52:16  sommerfeld]
# 
# Revision 1.1.18.4  1993/07/07  19:53:32  ganni
# 	reduced stub idl sources
# 	[1993/07/07  19:24:44  ganni]
# 
# Revision 1.1.18.3  1993/06/24  20:09:01  hinman
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:37:49  hinman]
# 
# Revision 1.1.18.2  1993/05/24  20:46:52  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:14:53  cjd]
# 
# Revision 1.1.16.2  1993/05/12  02:09:19  jd
# 	Initial 486 port.
# 	[1993/05/12  02:08:57  jd]
# 
# Revision 1.1.8.3  1993/09/02  17:56:19  sommerfeld
# 	clean up genpath/INCDIRS macros slightly.
# 
# 	[1993/08/31  14:07:04  sommerfeld]
# 
# 	clean up genpath stuff.
# 	[1993/07/29  20:29:51  sommerfe]
# 
# Revision 1.1.6.2  1993/05/28  13:04:58  tatsu_s
# 	Added INCDIRS for hpdce_version.h
# 	[1993/05/28  02:59:59  tatsu_s]
# 
# Revision 1.1.6.5  1993/01/03  21:37:22  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  14:31:37  bbelch]
# 
# Revision 1.1.6.4  1992/12/23  18:42:02  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  00:58:36  zeliff]
# 
# Revision 1.1.6.3  1992/12/04  17:09:38  marty
# 	OSC1.1.1 upgrade.
# 	[1992/12/04  17:00:53  marty]
# 
# Revision 1.1.9.2  1992/11/24  22:17:49  marty
# 	Allow for using osc1.0.4 lex or native lex.
# 	[1992/11/24  22:14:34  marty]
# 
# Revision 1.1.6.2  1992/11/06  21:26:08  john
# 	Moved the NO_Y_H_RULE before .include <osf.rules.mk>.
# 	[1992/11/03  15:46:58  john]
# 
# 	Added NO_Y_H_RULE to turn off the default .y.h: rule.
# 	[1992/11/03  15:28:57  john]
# 
# Revision 1.1.3.3  1992/04/30  21:15:05  john
# 	Added SETUP_PROGRAMS and SETUP_OTHERS.
# 	[1992/04/10  14:40:50  john]
# 
# Revision 1.1.3.2  1992/04/07  19:17:30  mckeen
# 	Added OFILES dependency on y_tab.h
# 	[1992/03/31  16:37:19  mckeen]
# 
# Revision 1.1  1992/01/19  03:00:32  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
# src/rpc/idl/idl_compiler/Makefile for IDL Compiler
#

# Don't link this against -lc_r; it's not necessary.
NO_LIBC_R = 1
.if (${TARGET_MACHINE} == "SVR4")

.if (${TARGET_OS_PLATFORM} == "PYRMIPS")
INCDIRS = ${XENV_BINCDIRS}
.else	# !TARGET_OS_PLATFORM=PYRMIPS
INCDIRS	=
.endif	# TARGET_OS_PLATFORM=PYRMIPS

.elif (${SITE} == "HP" && ${TARGET_OS} == "HPUX")

.else	# !TARGET_MACHINE=SVR4
INCDIRS =
.endif	# TARGET_MACHINE=SVR4

# Program to build and install
PROGRAMS= idl

# Program to be built by the setup pass
SETUP_PROGRAMS  =       idl
SETUP_OTHERS    =       idl.cat

# Additional file to build and install
OTHERS = idl.cat

# Files to install and their locations
ILIST	= ${PROGRAMS} ${OTHERS}
IDIR	= /bin/
idl.cat_IDIR	= /nls/msg/en_US.ASCII/

OFILES = acf_y_tab.o lex_yy.o acf_lex_yy.o astp_com.o \
	astp_cpx.o astp_gbl.o astp_sim.o astp_dmp.o checker.o\
	chkichar.o command.o driver.o errors.o files.o frontend.o       \
	getflags.o keywds.o main.o message.o nametbl.o       \
	namdump.o propagat.o sysdep.o idlparse.o     \
	 mtsbacke.o  ddbe.o ddspell.o cspeldcl.o  cspell.o  cstubmts.o \
         hdgen.o  clihamts.o  mtspipes.o irepgen.o irepscp.o \
         ifspemts.o  sstubmts.o  comstmts.o user_exc.o icharsup.o \
	${${TARGET_MACHINE}_idl_OFILES}

CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DMIA #-DBSD -D_BSD 
YFLAGS	= ${${TARGET_MACHINE}_YFLAGS} -d


.if defined (USE_OSC1_0_4_LEX) 
LEX = env LEXER=${DEFTOOLBASE}/oscncform ${DEFTOOLBASE}/osc104lex
LIBS=
INCFLAGS= -I.
.else
# set the NO_Y_H_RULE to ignore the default rule for .y.h:
NO_Y_H_RULE=ON
.endif

#
#standard rules
#
.include <${RULES_MK}>


#
#dependencies and rules
#
${OFILES} : y_tab.h

acf_lex_yy.o: acf_lex_yy.c acf_lex_yy_i.c acf_y_tab.h

acf_lex_yy_i.c:  acf.l
	-@: # LC_CTYPE='${LC_CTYPE:UC:S/^POSIX$/C/}' LC_ALL='${LC_ALL:UC:S/^POSIX$/C/}' LANG='${LANG:UC:S/^POSIX$/C/}'
	${RM} ${_RMFLAGS_} ${.TARGET}	
	${LEX} ${_LFLAGS_} -t ${.ALLSRC} > ${.TARGET}

# for parallel make
.ORDER:	 acf_y_tab.h acf_y_tab.c y_tab.h y_tab.c
acf_y_tab.c acf_y_tab.h: acf.y
	-@: # LC_CTYPE='${LC_CTYPE:UC:S/^POSIX$/C/}' LC_ALL='${LC_ALL:UC:S/^POSIX$/C/}' LANG='${LANG:UC:S/^POSIX$/C/}'
	${RM} ${_RMFLAGS_} acf_y_tab.c acf_y_tab.h
	${YACC} ${_YFLAGS_} ${.ALLSRC}
	${MV} y.tab.c acf_y_tab.c
	${MV} y.tab.h acf_y_tab.h

lex_yy.c: nidl.l
	-@: # LC_CTYPE='${LC_CTYPE:UC:S/^POSIX$/C/}' LC_ALL='${LC_ALL:UC:S/^POSIX$/C/}' LANG='${LANG:UC:S/^POSIX$/C/}'
	${RM} ${_RMFLAGS_} ${.TARGET}	
	${LEX} ${_LFLAGS_} -t ${.ALLSRC} > ${.TARGET}

y_tab.c y_tab.h: nidl.y
	-@: # LC_CTYPE='${LC_CTYPE:UC:S/^POSIX$/C/}' LC_ALL='${LC_ALL:UC:S/^POSIX$/C/}' LANG='${LANG:UC:S/^POSIX$/C/}'
	${RM} ${_RMFLAGS_} y_tab.c y_tab.h
	${YACC} ${_YFLAGS_} ${.ALLSRC}
	${MV} y.tab.c y_tab.c
	${MV} y.tab.h y_tab.h

idlparse.o:	y_tab.c y_tab.h

lex_yy.o: y_tab.h

# Add explicit dependencies for files depending on IDL_VERSION_TEXT,
# which is defined in sysdep.h.
command.o driver.o:	sysdep.h

# This rule creates an include file containing the text of messages in
# the message file, so that IDL can print default messages.
# The following sed command (note that $ must be doubled in the Makefile):
#	1. Deletes lines beginning with $.
#	2. Deletes blank lines.
#	3. Changes lines of the form:
#		<number> <message test>
#	    to
#		"message text",		
default_msg.h: nidlmsg.m
	${RM} ${_RMFLAGS_} ${.TARGET}
	${SED}  -e '/^\$$/d;/^$$/d;s/^[^ ]* /"/;s/$$/",/;' \
		${.ALLSRC} > ${.TARGET}

message.o:	default_msg.h

# Generate the message catalog
idl.cat: nidlmsg.m
	${RM} ${_RMFLAGS_} ${.TARGET}
	${GENCAT} ${.TARGET} ${.ALLSRC}

