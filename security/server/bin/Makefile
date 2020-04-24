# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.15.1  1996/08/09  12:08:01  arvind
# 	Merge Registry support for KDC private key storage
# 	[1996/07/13  20:49 UTC  aha  /main/aha_pk6/2]
#
# 	Merge-out and bugfixes and Registry work
# 	[1996/06/07  19:06 UTC  aha  /main/aha_pk6/1]
#
# 	Changes for Public Key Login; pick up bsafe library (TEMPORARY!!!).
# 	[1996/03/09  23:29:57  marty  1.2.13.2]
#
# Revision 1.2.13.2  1996/03/09  23:29:57  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:18  marty]
# 
# Revision 1.2.13.1  1995/12/08  17:46:33  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/01/30  22:43 UTC  sommerfeld
# 	fix DES_HIDDEN linking..
# 	[1995/01/27  04:18 UTC  sommerfeld  /main/sommerfeld_mothmk_2/1]
# 
# Revision 1.2.8.5  1994/08/26  15:28:53  mdf
# 	Fixed link problem with secd
# 	[1994/08/26  15:23:41  mdf]
# 
# Revision 1.2.8.4  1994/07/15  15:00:35  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/07/14  17:18:04  mdf]
# 
# Revision 1.2.8.2  1994/06/03  13:39:19  mdf
# 	removed DCE_AUD_POINTS from CFLAGS.
# 
# 	  NOTE:  THIS IS ONLY A QUICK FIX, THERE'S A MEMORY LEAK IN THE
# 	         AUDIT CODE, THIS FIX DISABLES THE CODE!
# 	[1994/06/03  13:34:48  mdf]
# 
# Revision 1.2.8.1  1994/04/01  21:37:22  kline_s
# 	[OT 10264] First audit code point drop for security.
# 	[1994/04/01  21:26:13  kline_s]
# 
# Revision 1.2.2.5  1993/01/26  23:00:59  emartin
# 	CFLAGS -DRS_REP_NAUTH
# 	[1993/01/13  16:26:32  emartin]
# 
# Revision 1.2.2.4  1992/12/29  16:33:31  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  21:21:26  zeliff]
# 
# Revision 1.2.2.3  1992/11/04  22:26:41  burati
# 	Removed refs to sec_admin and rgy_admin.c, since it has moved
# 	to security/client/admin/sec_admin/*
# 	[1992/11/04  19:43:56  burati]
# 
# Revision 1.2.2.2  1992/08/31  18:22:19  sekhar
# 	First security replication code drop
# 	[1992/08/31  15:10:39  sekhar]
# 
# Revision 1.2  1992/01/19  22:13:51  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0

#   Makefile.ODE V=9 10/28/91 //littl/prgy/src/server/bin
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#

#
# DCE Security Services
# /server/bin Makefile
#

# Additional places to search for source files
VPATH = ../rs

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD -DRS_REP_NAUTH ${${TARGET_MACHINE}_CFLAGS} -DDCE_AUD_POINTS

INCFLAGS = -I../../h -I../../idl -I../rs

# Programs to build
PROGRAMS = secd sec_create_db

# Files to install
ILIST = secd sec_create_db

# Directory in which to install the files
IDIR = /bin/

secd_OFILES = rs_main.o
sec_create_db_OFILES = rgy_create.o

LDIRS = -L../../krb5/kdc -L../../krb5/lib/kdb -L../rsdb \
	  -L../rrs -L../../utils -L../../audit/libaudit -L../lm -L../rs \
          -L../../psm

EXE_LIBS = -lrs -lkrb5kdc -lkdb -lrsdb -lsecutil -laudit -lpsm

.ifdef DES_HIDDEN
EXE_LIBS += ../../../libdce/bound_dce_nostrip.o
.endif

EXE_LIBS += -lrrs 

secd_LIBS = ${LDIRS} ${EXE_LIBS} -lrss -llm -ldce ${${TARGET_MACHINE}_LIBS}

sec_create_db_LIBS =${LDIRS}  ${EXE_LIBS} -llm -ldce ${${TARGET_MACHINE}_LIBS}

.include <${RULES_MK}>
