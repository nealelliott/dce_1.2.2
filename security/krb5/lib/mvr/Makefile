# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.10.1  1996/10/03  15:04:47  arvind
# 	turn off generation of line directives in the output
# 	since they seem to make debugging problems harder instead of
# 	easier.
# 	[1996/09/16  22:19 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
#
# Revision 1.1.8.2  1996/03/09  23:29:51  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:13  marty]
# 
# Revision 1.1.8.1  1995/12/08  18:02:17  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:00:19  root]
# 
# Revision 1.1.6.1  1994/06/02  21:29:06  mdf
# 	hp_sec_to_osf_3 drop, merge up with latest.
# 	[1994/05/24  15:17:03  mdf]
# 
# 	hp_sec_to_osf_3 drop
# 
# 	Makefile cleanups .. add rm -f's
# 	[1993/10/08  19:46:04  sommerfeld]
# 
# 	Embedding copyright notice
# 	[1992/12/28  21:16:20  zeliff]
# 
# Revision 1.1.2.2  1992/04/07  19:19:01  mckeen
# 	Changed mavros and mavcod to MAVROS and MAVCOD which are now defined
# 	in osf.std.mk
# 	[1992/03/13  15:46:45  mckeen]
# 
# Revision 1.1  1992/01/19  14:49:53  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=2 08/29/91 //littl/prgy/krb5/lib/mvr
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /krb5/lib/mvr Makefile
#

# Files to export
INCLUDES = KRB5.h \
	    KRB5_mvr.h

# Directory in which to place exported files
EXPDIR = /usr/include/krb5/mvr/

.include <${RULES_MK}>

KRB5.mvr KRB5_mvr.h: krb5.mvr
	${RM} ${_RMFLAGS_} KRB5.mvr KRB5_mvr.h
	${MAVCOD} -srce ${krb5.mvr:P}

KRB5.h: KRB5_mvr.h KRB5.mvr
	${RM} ${_RMFLAGS_} KRB5.h KRB5.c KRB5.make
	${MAVROS} KRB5.mvr -nolinenum -u -m -i KRB5_mvr.h
