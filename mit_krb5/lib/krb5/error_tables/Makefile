# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/06/04  21:51:03  arvind
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:32 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
#
# 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
# 	[1996/05/03  20:00 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1]
#
# 	Add RCS header & OSF copyright.
# 	[1996/05/03  20:00 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1]
#
# $EndLog$
#
#
# Copyright (c) Hewlett-Packard Company 1996
# Unpublished work. All Rights Reserved.
#
#
# MIT krb5
# mit_krb5/lib/krb5/error_tables Makefile
#

# Files to export and their locations
INCLUDES = kdb5_err.h krb5_err.h adm_err.h asn1_err.h kv5m_err.h \
           kdb5_err.c krb5_err.c adm_err.c asn1_err.c kv5m_err.c

EXPDIR = /usr/include/mitkrb5/

.include <${RULES_MK}>

kdb5_err.h: kdb5_err.et
	${AWK} -f ./../../../util/et/et_h outfile=./kdb5_err.h ${kdb5_err.et:P}

krb5_err.h: krb5_err.et
	${AWK} -f ./../../../util/et/et_h outfile=./krb5_err.h ${krb5_err.et:P}

adm_err.h: adm_err.et
	${AWK} -f ./../../../util/et/et_h outfile=./adm_err.h ${adm_err.et:P}

asn1_err.h: asn1_err.et
	${AWK} -f ./../../../util/et/et_h outfile=./asn1_err.h ${asn1_err.et:P}

kv5m_err.h: kv5m_err.et
	${AWK} -f ./../../../util/et/et_h outfile=./kv5m_err.h ${kv5m_err.et:P}

asn1_err.c: asn1_err.et
	${AWK} -f ./../../../util/et/et_c outfile=./asn1_err.c ${asn1_err.et:P}

isode_err.c: isode_err.et
	${AWK} -f ./../../../util/et/et_c outfile=./isode_err.c ${isode_err.et:P}

kdb5_err.c: kdb5_err.et
	${AWK} -f ./../../../util/et/et_c outfile=./kdb5_err.c ${kdb5_err.et:P}

krb5_err.c: krb5_err.et
	${AWK} -f ./../../../util/et/et_c outfile=./krb5_err.c ${krb5_err.et:P}

adm_err.c: adm_err.et
	${AWK} -f ./../../../util/et/et_c outfile=./adm_err.c ${adm_err.et:P}

kv5m_err.c: kv5m_err.et
	${AWK} -f ./../../../util/et/et_c outfile=./kv5m_err.c ${kv5m_err.et:P}
