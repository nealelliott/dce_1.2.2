# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.2  1996/10/03  18:49:15  arvind
# 	Rearrange how we copy ref_encode.out to the object tree.
# 	[1996/09/26  20:13 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
#
# Revision 1.1.2.1  1996/07/09  15:01:40  arvind
# 	OSF DCE 1.2.2 Drop 3
# 	[1996/07/09  14:54:35  arvind]
# 
# Revision /main/DCE_1.2.2/3  1996/06/25  19:57 UTC  mullan_s
# 	Merge to DCE_1.2.2
# 	[1996/06/20  18:19 UTC  mullan_s  /main/DCE_1.2.2/1]
# 
# Revision /main/mullan_dce_krb5_der_work/3  1996/06/25  19:54 UTC  mullan_s
# 	Copy reference_encode.out to temporary location to avoid cross-vob link
# 	from being installed.
# 
# Revision /main/mullan_dce_krb5_der_work/2  1996/06/21  19:26 UTC  mullan_s
# 	Link with libkrb5.a. Fix install dir.
# 
# Revision /main/mullan_dce_krb5_der_work/1  1996/06/10  19:30 UTC  mullan_s
# 	Build rules.
# 
# $EndLog$
# 

TET_ROOT        = /test/tet/functional/security
TET_TESTS       = ../../..
TET_SCEN_FILE   = ${TET_TESTS}/tet_scen
UPDATE_TET_SCEN = ${TET_TESTS}/tools/update_tet_scen
STANDARD_TETLIBS= -ltettcm -ltetapi
COMPONENT_DIR   = /ts/krb5/asn.1
LIBS            = -ldce -lkrb5 -lsectsutil ${STANDARD_TETLIBS}
PROGRAMS        = krb5_encode_test krb5_decode_test
SCRIPTS		= krb5_asn.1-tc
OTHERS          = update_tet_scen_rule ref_encode.out
TESTS           = krb5_asn.1-tc
INCFLAGS        = -I${EXPORTBASE}/usr/include/krb5
CFLAGS		= -DOSF_DCE
ILIST           = ${SCRIPTS} ${PROGRAMS} ref_encode.out
IDIR            = ${TET_ROOT}/${COMPONENT_DIR}/
IMODE           = 755

ENCOBJS 	= krb5_encode_test.o ktest.o ktest_equal.o utility.o trval.o
DECOBJS 	= krb5_decode_test.o ktest.o ktest_equal.o utility.o

krb5_encode_test_OFILES = $(ENCOBJS) $(DEPKLIB)
krb5_decode_test_OFILES = $(DECOBJS) $(DEPKLIB)

.include <${RULES_MK}>

update_tet_scen_rule:
	${UPDATE_TET_SCEN} ${TET_SCEN_FILE} ${COMPONENT_DIR} ${TESTS}

ref_encode.out: reference_encode.out
	${RM} ${_RMFLAGS_} ref_encode.out
	${CP} ${reference_encode.out:P} ref_encode.out
