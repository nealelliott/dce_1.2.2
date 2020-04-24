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
# Revision 1.1.4.2  1996/03/11  02:50:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:38  marty]
#
# Revision 1.1.4.1  1995/12/11  22:55:10  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/07/10  15:45 UTC  jrr
# 	Conditionalize signal list depending on machine type.
# 	[1995/07/10  15:43 UTC  jrr  /main/jrr_dcecp_scripts/1]
# 
# Revision 1.1.2.3  1993/08/09  19:54:57  cmckeen
# 	Changed the install directory to /test/tet/ instead of /tet/.
# 	[1993/08/09  19:35:18  cmckeen]
# 
# Revision 1.1.2.2  1993/07/20  20:44:33  cmckeen
# 	Initial DCE version
# 	[1993/07/15  16:29:16  cmckeen]
# 
# $EndLog$
#

# standard signal numbers - change to correct numbers for your system
# SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGABRT, SIGFPE, SIGPIPE, SIGALRM,
# SIGTERM, SIGUSR1, SIGUSR2, SIGTSTP, SIGCONT, SIGTTIN, SIGTTOU
.if ${TARGET_MACHINE} == "HP800"
STD_SIGNALS = 1 2 3 4 6 8 13 14 15 16 17 25 26 27 28
.elseif ${TARGET_MACHINE} == "AT386" || ${TARGET_MACHINE} == "RIOS" || ${TARGET_MACHINE} == "SVR4"
STD_SIGNALS = 1 2 3 4 6 8 13 14 15 19 21 22 30
.endif

# signals that are always unhandled - change for your system
# May need to include SIGSEGV and others if the shell can't trap them
# SIGKILL, SIGCHLD, SIGSTOP, (SIGSEGV, ...).
.if ${TARGET_MACHINE} == "HP800"
SPEC_SIGNALS = 9 18 11 24 31 32
.elseif ${TARGET_MACHINE} == "AT386" || ${TARGET_MACHINE} == "RIOS" || ${TARGET_MACHINE} == "SVR4"
SPEC_SIGNALS = 9 20 17 11
.endif


OTHERS		= tcm.ksh

ILIST		= tcm.ksh tetapi.ksh
IDIR		= /test/tet/lib/ksh/
IMODE		= 755

tcm.ksh: tcm.ksh.dat
	sed -e 's/STD_SIGNAL_LIST/$(STD_SIGNALS)/' \
	    -e 's/SPEC_SIGNAL_LIST/$(SPEC_SIGNALS)/' \
	    ${.ALLSRC} > ${.TARGET}

.include <${RULES_MK}>
