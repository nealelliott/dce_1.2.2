# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/06/04  21:49:12  arvind
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:34 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
#
# 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
# 	[1996/05/03  20:01 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1]
#
# 	Add RCS header & OSF copyright.
# 	[1996/05/03  20:01 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1]
#
# $EndLog$
#
# Copyright (c) Hewlett-Packard Company 1996
# Unpublished work. All Rights Reserved.
#
#
# MIT krb5
# mit_krb5/clients/kinit Makefile
#

INCFLAGS = -I${EXPORTBASE}/usr/include/mitkrb5 -I${EXPORTBASE}/usr/include/mitkrb5/krb5

CFLAGS = -DHAVE_PWD_H=1

# Program to build
PROGRAMS =  kinit

# Directories to look for libraries
LIBDIRS = -L${EXPORTBASE}/usr/lib/mitkrb5

# Libraries to link the program with
LIBS     =  -lmitkrb5

.include <${RULES_MK}>
