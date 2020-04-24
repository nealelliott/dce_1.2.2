#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.7.2  1996/03/11  02:36:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:25  marty]
#
# Revision 1.1.7.1  1995/12/11  21:52:21  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/11/15  22:07 UTC  mullan_s
# 	Merge.
# 
# 	HP revision /main/HPDCE01/1  1994/01/31  18:09  mdf
# 	merged forward
# 	[1995/12/11  20:54:04  root]
# 
# Revision 1.1.4.2  1993/08/02  18:23:08  sekhar
# 	Changes for security replication system test.
# 	[1993/08/02  18:21:44  sekhar]
# 
# Revision 1.1.1.10  1993/05/04  22:17:38  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.4.3  1992/12/31  17:35:39  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:59:40  htf]
# 
# Revision 1.1.4.2  1992/11/02  15:38:20  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:41:05  root]
# 
# Revision 1.1.2.4  1992/07/16  20:10:53  sekhar
# 	OT CR 3376 : Expunged all references to sec_test which is being defuncted.
# 	[1992/07/16  13:49:26  sekhar]
# 
# Revision 1.1.2.3  1992/03/12  21:24:45  sekhar
# 	Link libsecutil (in sec_test_LIBS) to resolve undefined symbols.
# 	[1992/03/12  19:23:32  sekhar]
# 
# Revision 1.1.2.2  1992/02/07  18:31:17  eperkins
# 	Removed extraneous libraries from LIBS defintion
# 	[1992/02/07  18:30:35  eperkins]
# 
# Revision 1.1  1992/01/19  04:08:55  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=1 10/23/91 //littl/prgy/src/client/tests_dir
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# test/security/commands/rgy Makefile
#

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../../../security/h

#   The security functional tests update query and rgylogin are used
#   in the replication system test. These can be installed by running
#   the install pass .

update_IDIR	= /tet/security/ts/secrep/
query_IDIR	= /tet/security/ts/secrep/
rgylogin_IDIR	= /tet/security/ts/secrep/

# Programs to build
PROGRAMS = query update rgylogin

ILIST	= ${PROGRAMS}

update_OFILES =	update.o timing.o
query_OFILES =	query.o 
rgylogin_OFILES = rgylogin.o

# Libraries the programs need to link with
LDIRS = 
LIBS =  -ldce 
	
APOLLO68K_LIBS  ?= -lsec -lkrb5 -ldce -lsec -ldce -lkrb5 -lintl

update_LIBS = ${LDIRS} ${LIBS} ${${TARGET_MACHINE}_LIBS} 
query_LIBS = ${LDIRS} ${LIBS}  ${${TARGET_MACHINE}_LIBS} 
rgylogin_LIBS = ${LDIRS} ${LIBS}  ${${TARGET_MACHINE}_LIBS} 

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
