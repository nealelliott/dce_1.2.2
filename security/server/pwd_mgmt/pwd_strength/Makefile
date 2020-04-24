# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:30:01  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:21  marty]
#
# Revision 1.1.4.1  1995/12/08  17:47:22  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:19:23  root]
# 
# Revision 1.1.2.3  1994/07/15  15:00:54  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/07/14  17:18:31  mdf]
# 
# Revision 1.1.2.2  1994/06/17  18:42:41  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:14:18  devsrc]
# 
# Revision 1.1.2.1  1994/05/11  19:26:14  ahop
# 	fix LDIR for utils
# 	[1994/05/02  17:24:20  ahop]
# 
# 	hp_sec_to_osf_2 drop
# 	[1994/04/29  21:23:45  ahop]
# 
# $EndLog$
# 
# Copyright (c) Hewlett-Packard Company 1994
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services
# /server/pwd_mgmt/pwd_strengthd Makefile
#

# Additional places to search for files
VPATH = ${EXPORTBASE}/usr/include/dce

# Flags for the NIDL compiler
IDLINCFLAGS = -I${EXPORTBASE}/usr/include/dce
IDLFLAGS = -keep c_source
IDLFILES =	\
		rsec_pwd_mgmt.idl

IDLDATAFILES =	rsec_pwd_mgmt.h

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../gen

# Programs to build
PROGRAMS = pwd_strengthd

# Files to install
ILIST = pwd_strengthd

# Directory in which to install the files
IDIR = /bin/

STUBS =		\
		rsec_pwd_mgmt_sstub.o

pwd_strengthd_OFILES = pwd_strengthd.o pwd_cache.o ${STUBS}

# Libraries the programs need to link with
LDIRS = -L../gen
EXE_LIBS = -lgen -ldce

pwd_strengthd_LIBS = ${LDIRS} ${EXE_LIBS} ${${TARGET_MACHINE}_LIBS}

.include <${RULES_MK}>

${pwd_strengthd_OFILES} : ${IDLDATAFILES}
