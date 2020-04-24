# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.12.2  1996/03/11  02:48:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:09:20  marty]
#
# Revision 1.1.12.1  1995/12/11  22:46:43  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/29  21:47 UTC  psn
# 	Change name of the script test_drv.ksh to dcesergy.
# 
# 	HP revision /main/HPDCE02/1  1994/07/05  15:33 UTC  bissen
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/1  1994/01/31  14:30  bissen
# 	merge kk and hpdce01
# 	[1995/12/11  22:22:08  root]
# 
# Revision 1.1.5.3  1993/08/27  21:00:20  giza
# 	fixed IDIRs for TET
# 	[1993/08/27  21:00:00  giza]
# 
# Revision 1.1.5.2  1993/07/09  14:27:26  root
# 	Initial King Kong branch
# 	[1993/07/09  14:26:59  root]
# 
# Revision 1.1.3.2  1993/05/04  20:44:06  sekhar
# 	Added rgy_test_ACF .
# 	[1993/05/04  20:27:24  sekhar]
# 
# 	Changed INCFLAGS to use a relative pathname for security/h.
# 	Added "../../tools" to INCFLAGS so systest.h can be included.
# 	[1993/05/03  20:28:41  sekhar]
# 
# 	Added OTHERS so scripts would be installed.
# 	[1993/03/31  18:12:50  eperkins]
# 
# 	Removed concatenation of old and new makefiles
# 	[1993/03/25  18:22:54  eperkins]
# 
# 	Added -g -pedantic compile flags, and secrgy_setup and login_test programs
# 	[1993/03/10  23:58:27  eperkins]
# 
# 	Embedded copyright notice
# 	[1993/02/05  14:46:41  cjd]
# 
# 	Initial version, 1.0.2 sec.rgy.7 test
# 	[1993/01/04  18:18:52  eperkins]
# 
# Revision 1.1.1.4  1993/04/21  20:28:15  root
# 	    Loading drop DCE1_0_2B23
# 
# Revision 1.1.3.2  1993/02/04  22:17:38  cjd
# 	Embedded copyright notice
# 
# $EndLog$
PMAX_CFLAGS = -pedantic
CFLAGS  = -DST_DEBUG -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS} ${SECURITY_CFLAGS}

IDLFLAGS = -keep c_source
IDLFILES = rgy_test.idl

rgy_test_ACF =

INCFLAGS = -I../../../../security/h -I../../tools

# Programs to build
PROGRAMS = secrgy_setup secrgy_server login_test secrgy_client 
SCRIPTS = dcesergy
OTHERS = dcesergy.data

# Files to install
IDIR = /tet/security/ts/sec.rgy.7/
ILIST = ${PROGRAMS} ${SCRIPTS}

# Libraries the programs need to link with
LIBS = -ldce

secrgy_server_LIBS = ${LIBS} ${${TARGET_MACHINE}_LIBS}
secrgy_client_LIBS = ${LIBS} ${${TARGET_MACHINE}_LIBS}
login_test_LIBS = ${LIBS} ${${TARGET_MACHINE}_LIBS}
secrgy_setup_LIBS = ${LIBS} ${${TARGET_MACHINE}_LIBS}

secrgy_server_OFILES = utils.o secrgy_server.o rgy_test_sstub.o rgy_test_saux.o
secrgy_client_OFILES = utils.o secrgy_client.o rgy_test_cstub.o
secrgy_setup_OFILES = utils.o secrgy_setup.o
login_test_OFILES = login_test.o utils.o 
secrgy_server.o:  rgy_test.h
secrgy_client.o utils.o:  rgy_test.h
secrgy_setup.o utils.o:  rgy_test.h
rgy_test_saux.o: rgy_test.idl
rgy_test.h: rgy_test.idl

.include <${RULES_MK}>
