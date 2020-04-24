#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# All Rights Reserved
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.2  1996/11/13  20:35:07  arvind
# 	Cert-API drop 2 from DEC
# 	[1996/11/03  23:22 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
#
# 	CAPI drop from DEC
# 	[1996/10/16  20:24 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
#
# Revision 1.1.2.1  1996/10/03  20:27:10  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  19:21:40  arvind]
# 
# 	OSF DCE 1.2.2 Drop 4
# 
# Revision /main/DCE_1.2.2/1  1996/09/09  21:25 UTC  arvind
# 	#
# 	[1996/08/30  16:07 UTC  arvind  /main/arvind_pkss/1]
# 
# $EndLog$
#
#
# Copyright (c) Digital Equipment Corporation, 1996
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services
# pkc/h Makefile
#

SAMSFILES        = pkc.sams

IDIR = /usr/include/dce/

SAMIDL_INCLUDES  = dcepkcmsg.h \
		   dcepkcmac.h \
                   dcepkcsvc.h

SAMS_TARGETS     = dcepkcmsg.h \
                   dcepkcmsg.c \
                   dcepkcsvc.c \
                   dcepkcsvc.h \
                   dcepkc.cat 

# Files to export and their locations
INCLUDES =  asn.h \
            x500name.h \
            x500_att.h \
            x509.h \
            x501.h \
            rsakey.h \
	    dcepkcmsg.h \
            dcepkcmac.h \
            dcepkcsvc.h  \
            pkc_crypto_reg.h \
            xdsxx.h \
            pkc_consts.h \
	    pkc_certs.h \
	    pkc_common.h \
	    pkc_api.h \
	    pkc_plcy_hierarchy.h \
	    pkc_plcy_registry.h \
	    pkc_plcy_reg.h \
	    pkc_plcy_oid.h \
	    pkc_cross_cert.h \
            pkc_codesets.h \




EXPDIR = /usr/include/dce/

# Files to install and their locations
OTHERS  = dcepkc.cat \
          dcepkcmsg.h 

ILIST   = ${OTHERS} $(INCLUDES)
IDIR    = /share/include/dce/
dcepkc.cat_IDIR = /nls/msg/en_US.ASCII/
dcepkcmsg.h_IDIR = /share/include/dce/

VPATH = .:./${MACHINE}

.include <${RULES_MK}>

