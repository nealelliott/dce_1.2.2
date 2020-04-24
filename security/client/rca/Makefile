# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.13.2  1996/03/11  13:28:01  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:15:48  marty]
#
# Revision 1.1.13.1  1995/12/08  18:06:08  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:31:59  root]
# 
# Revision 1.1.8.1  1994/09/07  15:48:36  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:33  marty]
# 
# Revision 1.1.6.4  1993/01/29  18:49:54  burati
# 	replication changes
# 	[1993/01/14  23:14:49  burati]
# 
# Revision 1.1.6.3  1992/12/29  13:02:25  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:40:14  zeliff]
# 
# Revision 1.1.6.2  1992/12/08  18:59:04  rps
# 	Add rs_bind
# 	[1992/12/03  01:28:40  rps]
# 
# Revision 1.1.2.3  1992/06/02  20:48:22  sekhar
# 	Drop the Unix interface changes made by frisco.
# 	[1992/06/01  21:01:48  sekhar]
# 
# Revision 1.1.3.2  1992/05/22  18:16:45  frisco
# 	Add support for Unix interfaces.
# 
# Revision 1.1.2.2  1992/04/07  19:18:21  mckeen
# 	Removed DATAFILES definition
# 	[1992/03/13  15:41:49  mckeen]
# 
# Revision 1.1  1992/01/19  14:47:36  devrcs
# 	Initial revision
# 
# $EndLog$
#

#
# Copyright (c) Hewlett-Packard Company 1991,1992,1993
# Unpublished work. All Rights Reserved.
#
# DCE Security Services 
# /client/rca Makefile
#

# Additional places to look for files
VPATH = ../../idl

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

# Flags for the IDL compiler
IDLFLAGS = -keep c_source -cepv 
IDLINCFLAGS = -I. -I../../idl
IDLFILES = 	\
		rs_acct.idl \
		rs_bind.idl \
		rs_misc.idl \
		rs_pgo.idl  \
		rs_plcy.idl \
		rs_unix.idl \
		rs_repadm.idl \
		rs_replist.idl \
		rpriv.idl   \
		rsecidmap.idl \
                rsec_cert.idl \
                roverride.idl \
                krb5rpc.idl
rs_acct_ACF =
rs_bind_ACF =
rs_misc_ACF =
rs_pgo_ACF =
rs_plcy_ACF =
rs_unix_ACF =
rs_repadm_ACF =
rs_replist_ACF =
rsecidmap_ACF =
rsec_cert_ACF =
roverride_ACF =
krb5rpc_ACF =

IDLDATAFILES =  \
		rs_acct.h   \
		rs_bind.h   \
		rs_misc.h   \
		rs_pgo.h    \
		rs_plcy.h   \
		rs_unix.h   \
		rs_repadm.h \
		rs_replist.h\
		rpriv.h     \
		rsecidmap.h \
                rsec_cert.h \
                roverride.h \
                krb5rpc.h

.include <${RULES_MK}>
