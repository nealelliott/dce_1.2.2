# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.2  1996/03/09  23:29:38  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:44:59  marty]
#
# Revision 1.1.9.1  1995/12/08  17:41:06  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:56:51  root]
# 
# Revision 1.1.7.1  1994/05/11  19:15:02  ahop
# 	hp_sec_to_osf_2 drop
# 	[1994/04/29  21:11:13  ahop]
# 
# Revision 1.1.5.2  1992/12/29  13:57:39  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:51:37  zeliff]
# 
# Revision 1.1.3.2  1992/06/01  20:43:04  sekhar
# 	Drop the changes made by Joe Pato for des code
# 	restructuring.
# 	[1992/06/01  19:53:50  sekhar]
# 
# Revision 1.1.1.2  1992/05/14  17:16:01  pato
# 	Add des_int.h to exported set (OT#2330)
# 
# Revision 1.1  1992/01/19  14:49:09  devrcs
# 	Initial revision
# 
# $EndLog$
#

# Copyright (c) Hewlett-Packard Company 1991, 1994
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /krb5/include/krb5 Makefile
#

EXPINC_SUBDIRS = mvr

# Files to export
INCLUDES = dbm.h       func-proto.h   los-proto.h    rcache.h   \
	encryption.h   hostaddr.h     libos.h        sysincl.h  \
	asn1.h         error_def.h    macros.h       wordsize.h \
	base-defs.h    errors.h       kdb.h          widen.h    \
	ccache.h       ext-proto.h    kdb_dbm.h      mit-des.h  \
	config.h       fieldbits.h    keytab.h       osconf.h   \
	copyright.h    krb5.h         osf_dce.h      narrow.h   \
	crc-32.h       free.h         proto.h        des_int.h  \
	safepriv.h     rsa-md4.h      rsa-md5.h      preauth.h

# Directory in which to place exported files
EXPDIR = /usr/include/krb5/

.include <${RULES_MK}>
