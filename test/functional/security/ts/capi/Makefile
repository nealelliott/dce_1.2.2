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
# Revision 1.1.2.1  1996/11/14  16:10:59  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  15:22:19  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/10/18  00:44 UTC  arvind
# 	Fix build errors
# 	[1996/10/17  20:30 UTC  arvind  /main/arvind_capi/4]
# 
# 	Fix build errors
# 	[1996/10/17  20:10 UTC  arvind  /main/arvind_capi/3]
# 
# 	Fix build error
# 	[1996/10/17  19:44 UTC  arvind  /main/arvind_capi/2]
# 
# 	CAPI drop from DEC
# 	[1996/10/16  20:36 UTC  arvind  /main/arvind_capi/1]
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
OTHERS          = update_tet_scen_rule
TESTS	= test-registry test-hierarchy


STANDARD_TETLIBS = -ltettcm -ltetapi
COMPONENT_DIR = /ts/capi

DATA_SUBDIRS = certs keys testcases

# VPATH = ${MAKETOP}/test/security/api/capi:\
#        ${MAKETOP}/security/pkc/h
VPATH = ../../../../security/api/capi:${MAKETOP}/security/pkc/h


RIOS_CPP_LIBS = -L/usr/lpp/xlC/lib -L/usr/lpp/xlC -lC -lm
CPP_LIBS = ${${TARGET_MACHINE}_CPP_LIBS}


#
# install directory
#
IDIR		= ${TET_ROOT}/${COMPONENT_DIR}/

# ILIST =	test_pkss5a

#
# things to install
#

#
# libs for binding
#
# LIBS = ${${TARGET_MACHINE}_LIBS} -lpk -lcdsutil -lgdb -lpsm -ldce -lbsafe2 ${CPP_LIBS} ${STANDARD_TETLIBS}


LIBFLAGS = -L../../../../../security/pkc -L../../../../../security/pkc/capi/trust \
            -L../../../../../security/pkc/capi/plcy  -L../../../../../security/pkc/capi/crypto \
            -L../../../../../security/pkc/xds

LIBS = ${${TARGET_MACHINE}_LIBS} -lpkplcy -lpktrust -lpk -lpkcrypto -lpkxds -ldce \
       ${CPP_LIBS} ${STANDARD_TETLIBS} -bloadmap:error 

CC_LD = xlC



# LIBDIRS = -L../../../../../security/psm -L../../../../../security/pkc -L../../../../../directory/cds/gloria/gdb -L../../../../../directory/cds/library -L../../../../tet/src/posix_c/api
#
# flags for compiling
#
#CFLAGS		= ${${TARGET_MACHINE}_CFLAGS} -g -DDEBUG -DSVC_DEBUG -DENABLE_ALL
# CFLAGS		= ${${TARGET_MACHINE}_CFLAGS} -g -DENABLE_ALL -DMALLOCTRACE -DENVIRONMENT_TET

CFLAGS		= ${${TARGET_MACHINE}_CFLAGS}  -DENVIRONMENT_TET 

INCFLAGS = -I. -I../../../../../security/pkc/h -I../../../../../security/h \
               -I../../../../../directory/cds/includes \
               -I${BSAFE_ROOT}/usr/include

SCRIPTS = test-registry test-hierarchy
DATAFILES = create_foo delete_foo create_era


test_registry_tet_OFILES = test_registry.o trycase_helper.o
trycase_a_tet_OFILES = trycase_a.o cds_help_a.o trycase_helper.o
PROGRAMS =  trycase_a_tet  test_registry_tet


.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}
