# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:50:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:47  marty]
#
# Revision 1.1.4.1  1995/12/11  22:56:36  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/21  16:43 UTC  jrr
# 	Remove DCE specific signals.
# 
# 	HP revision /main/jrr_tcl_tet/1  1995/03/21  16:35 UTC  jrr
# 	Remove DCE specific signals.
# 	[1995/12/11  22:34:27  root]
# 
# Revision 1.1.2.7  1994/06/21  16:06:05  kevins
# 	OT 10955 Add VTALRM sig into special sig list for at386
# 	[1994/06/21  13:57:16  kevins]
# 
# Revision 1.1.2.6  1994/06/10  20:51:18  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:20:03  devsrc]
# 
# Revision 1.1.2.5  1994/05/25  18:10:18  rousseau
# 	Fix signal 12 problem on HP (CR 10748).
# 	[1994/05/25  18:10:02  rousseau]
# 
# Revision 1.1.2.4  1994/05/06  14:49:23  rousseau
# 	Updated install path (CR 10558).
# 	[1994/05/06  14:49:03  rousseau]
# 
# Revision 1.1.2.3  1994/03/11  01:28:33  rousseau
# 	Made list of signals platform dependent (CR 10121).
# 	[1994/03/11  01:28:19  rousseau]
# 
# Revision 1.1.2.2  1994/02/25  22:17:45  rousseau
# 	Reordered signal list.
# 	[1994/02/25  22:15:41  rousseau]
# 
# Revision 1.1.2.1  1994/02/10  22:54:38  rousseau
# 	Initial version
# 	[1994/02/10  22:51:43  rousseau]
# 
# 	Created TCL API.
# 
# $EndLog$

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



SCRIPTS		= tetapi.tcl
OTHERS		= tcm.tcl
ILIST		= tcm.tcl tetapi.tcl
IDIR		= /test/tet/lib/tcl/
DATAFILES	= ${SCRIPTS}
IMODE		= 755

tcm.tcl : tcm.tcl.dat
	sed -e 's/STD_SIGNAL_LIST/$(STD_SIGNALS)/' \
	    -e 's/SPEC_SIGNAL_LIST/$(SPEC_SIGNALS)/' \
	    ${.ALLSRC} > ${.TARGET}

.include <${RULES_MK}>
