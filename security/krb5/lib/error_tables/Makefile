# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.8.2  1996/03/09  23:29:46  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:07  marty]
#
# Revision 1.1.8.1  1995/12/08  17:50:58  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:58:40  root]
# 
# Revision 1.1.6.1  1994/06/02  21:29:02  mdf
# 	hp_sec_to_osf_3 drop, merge up with latest.
# 	[1994/05/24  15:16:57  mdf]
# 
# 	hp_sec_to_osf_3 drop
# 
# Revision 1.1.4.2  1993/10/08  19:57:46  sommerfeld
# 	Remove derived objects before recreating them.
# 	[1993/10/08  19:55:58  sommerfeld]
# 
# Revision 1.1.4.2  1992/12/29  14:25:30  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  21:07:11  zeliff]
# 
# Revision 1.1.2.2  1992/04/07  19:18:54  mckeen
# 	Changed compile_et to ${COMPILE_ET}. COMPILE_ET is now defined in osf.dce.mk
# 	[1992/03/13  15:46:38  mckeen]
# 
# Revision 1.1  1992/01/19  14:53:14  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=3 11/25/91 //littl/prgy/krb5/lib/error_tables
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# krb5/lib/error_tables Makefile
#

# Files to export and their locations
INCLUDES = kdb5_err.h krb5_err.h
EXPDIR   = /usr/include/krb5/error_tables/

# Files to install and their locations
OTHERS = dcekdb.cat dcekrb.cat
ILIST   = ${OTHERS}
dcekdb.cat_IDIR    = /nls/msg/en_US.ASCII/
dcekrb.cat_IDIR    = /nls/msg/en_US.ASCII/

.include <${RULES_MK}>

kdb5_err.h: kdb5_err.et
	${RM} ${_RMFLAGS_} ${.TARGET} kdb5_err.msf
	${COMPILE_ET} $>

krb5_err.h: krb5_err.et
	${RM} ${_RMFLAGS_} ${.TARGET} krb5_err.msf
	${COMPILE_ET} $>

dcekdb.cat: kdb5_err.msf
	${RM} ${_RMFLAGS_} ${.TARGET}
	${GENCAT} $@ $>

dcekrb.cat: krb5_err.msf
	${RM} ${_RMFLAGS_} ${.TARGET}
	${GENCAT} $@ $>

