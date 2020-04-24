#   
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#  
# All Rights Reserved
#    
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/11/19  20:43:54  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/19  20:41:46  arvind]
#
# $EndLog$
#
# (c) Copyright 1996,
# Digital Equipment Corporation, Maynard, Massachusetts, USA
# All Rights Reserved
#
# Makefile for Private Key Store Server (PKSS) and friends
#


#
# Need this for the TET tests
TET_ROOT      = /test/tet/functional/security
TET_TESTS     = ../..
TET_SCEN_FILE = $(TET_TESTS)/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen


STANDARD_TETLIBS = -ltettcm -ltetapi
COMPONENT_DIR = /ts/pkss


PROGRAMS        = \
                  test_pkss1    \
                  test_pkss2    \
                  test_pkss3    \
                  test_pkss4    \
                  test_pkss5    \
                  test_pkss5a

OTHERS          = update_tet_scen_rule
TESTS = test-pkss1.sh test-pkss2.sh test-pkss3.sh test-pkss4.sh test-pkss5a.sh test-pkss5.sh


VPATH = ../../../../security/api/pkss:\
../../../../../security/pkc/h:\
../../../../tet/inc/posix_c:\
../../../../../security/pkc/pkss/server:../../../../../test/security/api/pkss:../../../../../security/pkc/pkss/server/stubs:../../../../../directory/cds/library:../../../../../security/pkc/pkss/pipe:../../../../../security/pkc/pkss/api:../../../../../security/pkc/pkss/acl:../../../../../security/psm/domestic:../../../../../directory/cds/gloria/gdb

DATAFILES = test-pkss1.sh \
            test-pkss2.sh \
            test-pkss3.sh \
            test-pkss4.sh \
            test-pkss5a.sh \
            test-pkss5.sh \

RIOS_CPP_LIBS = -L/usr/lpp/xlC/lib -L/usr/lpp/xlC -lC -lm

CPP_LIBS = ${${TARGET_MACHINE}_CPP_LIBS}


#
# install directory
#
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/
ILIST           = ${PROGRAMS} ${DATAFILES}


#
# things to install
#

#
# libs for binding
#
LIBS = ${${TARGET_MACHINE}_LIBS} -ldce ${CPP_LIBS} ${STANDARD_TETLIBS}

#LIBDIRS = -L../../../../tet/src/posix_c/api
#
# flags for compiling
#
#CFLAGS		= ${${TARGET_MACHINE}_CFLAGS} -g -DDEBUG -DSVC_DEBUG -DENABLE_ALL
CFLAGS		= ${${TARGET_MACHINE}_CFLAGS} -g -DENABLE_ALL -DMALLOCTRACE -DENVIRONMENT_TET



INCFLAGS	= -I.					\
		  -I../../../../../security/h		\
		  -I../../../../../security/pkc/pkss/server	\
		  -I../../../../../security/psm/domestic \
		  -I../../../../../directory/cds/includes	\
		  -I../../../../../directory/cds/gloria/gdb
		  

LDFLAGS = -bloadmap:load.map

#
# lists of objects for programs
#

test_pkss1.o:  test_pkss1.cxx

test_pkss1_OFILES =    test_pkss1.o

test_pkss2.o:  test_pkss2.cxx

test_pkss2_OFILES =    test_pkss2.o

test_pkss3.o:  test_pkss3.cxx

test_pkss3_OFILES =    test_pkss3.o

test_pkss4.o:  test_pkss4.cxx

test_pkss4_OFILES =    test_pkss4.o

test_pkss5.o:  test_pkss5.cxx

test_pkss5_OFILES =    test_pkss5.o

test_pkss5a.o:  test_pkss5a.cxx

test_pkss5a_OFILES =    test_pkss5a.o


.include <${RULES_MK}>

update_tet_scen_rule: 
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}

