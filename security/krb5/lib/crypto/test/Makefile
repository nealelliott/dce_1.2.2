# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# @HP_COPYRIGHT@
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/10/03  15:01:40  arvind
# 	Make it build again..
# 	[1996/09/17  23:32 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
#
# Revision 1.1.2.2  1996/03/09  20:45:35  marty
# 	Add OSF copyright
# 	[1996/03/09  20:25:03  marty]
# 
# Revision 1.1.2.1  1995/12/08  17:50:23  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/08/21  17:36 UTC  sommerfeld
# 	crypto regression test makefile.
# 	[1995/08/17  21:50 UTC  sommerfeld  /main/sommerfeld_mothpatch/1]
# 
# $EndLog$

DESDIR=../../krb5/

DESLIB=${DESDIR}ecb_encrypt.o ${DESDIR}make_sched.o \
	${DESDIR}des_tables.o ${DESDIR}cbc_encrypt.o
MD5LIB=	${DESDIR}md5.o

authcert_OFILES=authcert.o 
authcert_LIBS=${DESLIB}
desspeed_OFILES=desspeed.o
desspeed_LIBS=${DESLIB} ${MD5LIB}
md5cert_OFILES=md5cert.o 
md5cert_LIBS=${MD5LIB}
dump_sched_OFILES=dump_sched.o
dump_sched_LIBS=${DESLIB}

CFLAGS=-DOSF_DCE

PROGRAMS=authcert desspeed md5cert dump_sched
OTHERS=tests
DATAFILES=certdata

certdata_IMODE=0444

IDIR=/tests/
ILIST=${PROGRAMS} ${DATAFILES} ${SCRIPTS}
IOWNER=${USER}
SCRIPTS=runtest.sh

.include <${RULES_MK}>


