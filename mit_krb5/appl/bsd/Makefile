# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.3  1996/10/03  14:38:09  arvind
# 	Merge to DCE_1.2.2
# 	[1996/07/09  19:47 UTC  mullan_s  /main/DCE_1.2.2/2]
#
# 	Remove KRBV compiler define - just use KERBEROS instead.
# 	[1996/08/26  19:39 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/2]
#
# 	Change DES_HIDDEN to USE_DES.
# 	[1996/06/18  14:43 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
#
# 	Add HAVE_SETOWN define for AIX.
# 	[1996/05/30  21:16 UTC  mullan_s  /main/DCE_1.2.2/1]
#
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:36 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
#
# 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
# 	[1996/05/17  17:39 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
#
# 	If DES_HIDDEN defined, add -DNOENCRYPTION to CFLAGS.
# 	[1996/05/03  20:03 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/3]
#
# 	Add build rules for err.c
# 	[1996/04/19  21:21 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/2]
#
# 	Add OSF/HP copyrights.
# 	Set correct compiler flags for RIOS.
# 	[1996/04/19  21:21 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/2]
#
# Revision 1.1.2.2  1996/08/09  12:02:03  arvind
# 	Merge to DCE_1.2.2
# 	[1996/06/18  14:43 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
# 
# 	Add HAVE_SETOWN define for AIX.
# 	[1996/05/30  21:16 UTC  mullan_s  /main/DCE_1.2.2/1]
# 
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:36 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
# 
# 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
# 	[1996/05/17  17:39 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
# 
# 	If DES_HIDDEN defined, add -DNOENCRYPTION to CFLAGS.
# 	[1996/05/03  20:03 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/3]
# 
# 	Add build rules for err.c
# 	[1996/04/19  21:21 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/2]
# 
# 	Add OSF/HP copyrights.
# 	Set correct compiler flags for RIOS.
# 	[1996/04/19  21:21 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/2]
# 
# Revision 1.1.2.1  1996/06/04  21:45:53  arvind
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:36 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
# 
# 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
# 	[1996/05/17  17:39 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
# 
# 	If DES_HIDDEN defined, add -DNOENCRYPTION to CFLAGS.
# 	[1996/05/03  20:03 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/3]
# 
# 	Add build rules for err.c
# 	[1996/04/19  21:21 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/2]
# 
# 	Add OSF/HP copyrights.
# 	Set correct compiler flags for RIOS.
# 	[1996/04/19  21:21 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/2]
# 
# $EndLog$
#
#
# Copyright (c) Hewlett-Packard Company 1996
# Unpublished work. All Rights Reserved.
#
#
# MIT krb5
# mit_krb5/appl/bsd Makefile
#

# Files to install
ILIST = rsh rshd rlogin rlogind

# Directories in which to install the files
rsh_IDIR = /bin/
rshd_IDIR = /etc/
rlogin_IDIR = /bin/
rlogind_IDIR = /etc/

INCFLAGS = -I${EXPORTBASE}/usr/include/mitkrb5 -I${EXPORTBASE}/usr/include/mitkrb5/krb5

.if ${TARGET_MACHINE}=="RIOS"
RUTIL_CFLAGS = -DPOSIX_TERMIOS -DPOSIX_SIGNALS -DPOSIX_SETJMP -DHAVE_WAITPID -DWAIT_USES_INT \
	-DKERBEROS -DKRBV -DOSF_DCE -DHAVE_STDARG_H -DHAVE_SYS_SELECT_H -DHAVE_SETOWN \
	${${TARGET_MACHINE}_CFLAGS}
.else
RUTIL_CFLAGS = -DPOSIX_TERMIOS -DPOSIX_SIGNALS -DPOSIX_SETJMP -DHAVE_WAITPID -DKERBEROS \
	-DKRBV -DOSF_DCE -DHAVE_STDARG_H -DWAIT_USES_INT ${${TARGET_MACHINE}_CFLAGS}
.endif

.if defined (USE_DES)
CFLAGS = ${RUTIL_CFLAGS} ${DCEPATHS}
.else
CFLAGS = ${RUTIL_CFLAGS} -DNOENCRYPTION ${DCEPATHS}
.endif

# Program to build
PROGRAMS =  rlogin rsh rshd rlogind

.if defined (USE_DES)
rlogin_OFILES =		forward.o getenv.o kcmd.o rlogin.o setenv.o
rsh_OFILES =		err.o forward.o getenv.o kcmd.o rsh.o setenv.o
rlogind_OFILES =	forward.o getenv.o recvauth.o rlogind.o setenv.o
rshd_OFILES =		forward.o getenv.o recvauth.o rshd.o setenv.o
.else
rlogin_OFILES =		forward.o getenv.o kcmd.o rlogin.o setenv.o
rsh_OFILES =		err.o forward.o getenv.o kcmd.o rsh.o setenv.o
rlogind_OFILES =	forward.o getenv.o recvauth.o rlogind.o setenv.o
rshd_OFILES =		forward.o getenv.o recvauth.o rshd.o setenv.o
.endif

# Directories to look for libraries
LIBDIRS = -L${EXPORTBASE}/usr/lib/mitkrb5

# Libraries to link the program with
LIBS     =  -lmitkrb5
rshd_LIBS = -lmitkrb5 -ldbm -lpty
rlogind_LIBS = -lmitkrb5 -ldbm -lpty

.include <${RULES_MK}>
