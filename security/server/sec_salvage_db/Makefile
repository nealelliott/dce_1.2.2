# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.13.1  1996/11/13  18:12:10  arvind
# 	CHFts20737: fun and games with international builds
# 	[1996/11/12  21:32 UTC  sommerfeld  /main/sommerfeld_pk_kdc_6/1]
#
# Revision 1.1.11.2  1996/03/09  23:30:22  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:40  marty]
# 
# Revision 1.1.11.1  1995/12/08  18:00:52  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/01/30  22:43 UTC  sommerfeld
# 	rearrange libs to allow for DES_HIDDEN link to work.
# 	[1995/01/27  04:18 UTC  sommerfeld  /main/sommerfeld_mothmk_2/1]
# 
# Revision 1.1.8.6  1994/07/15  15:03:05  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/07/14  17:24:03  mdf]
# 
# Revision 1.1.8.4  1994/06/17  18:42:56  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:20:05  devsrc]
# 
# Revision 1.1.8.3  1994/06/03  13:39:22  mdf
# 	removed DCE_AUD_POINTS from CFLAGS.
# 
# 	  NOTE:  THIS IS ONLY A QUICK FIX, THERE'S A MEMORY LEAK IN THE
# 	         AUDIT CODE, THIS FIX DISABLES THE CODE!
# 	[1994/06/03  13:35:10  mdf]
# 
# Revision 1.1.8.2  1994/04/01  21:24:54  kline_s
# 	Merged with changes from 1.1.8.1
# 	[1994/04/01  21:24:27  kline_s]
# 
# 	[OT 10264] First audit code point drop for security.
# 	[1994/04/01  21:16:55  kline_s]
# 
# Revision 1.1.8.1  1994/03/25  18:16:21  sekhar
# 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
# 	              ( Link in dcesadmsg.o )
# 	[1994/03/25  18:15:31  sekhar]
# 
# Revision 1.1.8.1  1994/03/25  18:16:21  sekhar
# 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
# 	              ( Link in dcesadmsg.o )
# 	[1994/03/25  18:15:31  sekhar]
# 
# Revision 1.1.3.2  1993/03/05  18:31:37  frisco
# 	fix for RIOS port
# 	[1993/03/03  20:00:21  frisco]
# 
# 	initial salvager functionality
# 	[1993/03/02  21:20:25  frisco]
# 
# Revision 1.1.1.2  1992/11/25  21:10:47  frisco
# 	Initial Revision
# 
# $EndLog$

# Copyright (c) Hewlett-Packard Company 1992
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services
# /server/sec_salvage_db Makefile
#

# Additional places to search for source files
VPATH = ../rs:../../h

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS} -DDCE_AUD_POINTS

INCFLAGS = -I../../h -I../../idl -I../rs

# Programs to build
PROGRAMS = sec_salvage_db

# Files to install
ILIST = sec_salvage_db

# Directory in which to install the files
IDIR = /bin/

sec_salvage_db_OFILES = \
	sec_svg_db.o \
	sec_svg_util.o \
	sec_svg_prt.o \
	sec_svg_rec.o \
	dcesadmsg.o

# Libraries the programs need to link with
LDIRS =	-L../../krb5/kdc -L../../krb5/lib/kdb -L../rsdb -L../rrs -L../../utils -L../../audit/libaudit -L../lm
EXE_LIBS = 	-lkrb5kdc -lkdb -lrsdb -lsecutil -llm
.ifdef INTL_BUILD
LDIRS += -L../../psm
EXE_LIBS += -lpsm
.endif
.ifdef DES_HIDDEN
EXE_LIBS += ../../../libdce/bound_dce_nostrip.o
.endif
EXE_LIBS +=  -lrrs -laudit -ldce 

sec_salvage_db_LIBS = ${LDIRS} -L../rs -lrsdb -lrs ${EXE_LIBS} ${${TARGET_MACHINE}_LIBS}

.include <${RULES_MK}>
