# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/06/04  21:51:59  arvind
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:38 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
#
# 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
# 	[1996/04/19  18:57 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/3]
#
# 	Add OSF/HP copyrights
# 	[1996/04/19  18:57 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/3]
#
# $EndLog$
#
#
# Copyright (c) Hewlett-Packard Company 1996
# Unpublished work. All Rights Reserved.
#
#
# MIT krb5
# mit_krb5/utils/pty Makefile
#

# Files to export and their locations
INCLUDES = pty_err.h libpty.h
EXPDIR = /usr/include/mitkrb5/

.if ${TARGET_MACHINE}=="RIOS"
CFLAGS = -DHAVE_VHANGUP -DSTDC_HEADERS -DHAVE_GETUTENT \
	 -DHAVE_SETSID -DHAVE_TTYNAME -DHAVE_UNISTD_H  \
	 -DHAVE_STDLIB_H -DHAVE_STRING_H -DHAVE_UTMP_H \
	 -DHAVE_SYS_TTY_H -DNEED_SYS_ERRLIST -Dkrb5_sigtype=void \
	 -DPOSIX_SIGTYPE -DPOSIX_SIGNALS -DPOSIX_SETJMP \
	 -DUSE_DIRENT_H -DPOSIX_TERMIOS -DHAVE_SETUTENT \
	 -DSETPGRP_TWOARG -Dconst= -DKERBEROS -DOSF_DCE 
.else
CFLAGS = -DHAVE_VHANGUP -DSTDC_HEADERS -DHAVE_GETUTENT \
	 -DHAVE_SETSID -DHAVE_TTYNAME -DHAVE_PTSNAME   \
	 -DHAVE_UNISTD_H -DHAVE_STDLIB_H -DHAVE_STRING_H \
	 -DHAVE_UTMP_H -DHAVE_SYS_TTY_H -DNEED_SYS_ERRLIST \
 	 -Dkrb5_sigtype=void -DPOSIX_SIGTYPE -DPOSIX_SIGNALS \
	 -DPOSIX_SETJMP -DUSE_DIRENT_H -DPOSIX_TERMIOS \
	 -DHAVE_SETUTENT -DSETPGRP_TWOARG -Dconst= -DKERBEROS -DOSF_DCE
.endif

libpty.a_OFILES = cleanup.o getpty.o init.o init_slave.o logwtmp.o open_ctty.o \
		  open_slave.o update_utmp.o update_wtmp.o vhangup.o void_assoc.o \
		  bsd44lite_pty.o bsd44lite_login_tty.o bsd44lite_logout.o pty_err.o

INCFLAGS = -I${EXPORTBASE}/usr/include/mitkrb5 -I${EXPORTBASE}/usr/include/mitkrb5/krb5

LIBRARIES = libpty.a
libpty.a_EXPDIR = /usr/lib/mitkrb5/
EXPLIB_TARGETS  = export_libpty.a

.include <${RULES_MK}>

pty_err.h: pty_err.et
	${AWK} -f ./../et/et_h outfile=./pty_err.h ${pty_err.et:P}

pty_err.c: pty_err.et
	${AWK} -f ./../et/et_c outfile=./pty_err.c ${pty_err.et:P}
