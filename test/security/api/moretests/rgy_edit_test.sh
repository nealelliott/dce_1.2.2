#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: rgy_edit_test.sh,v $
# Revision 1.1.6.2  1996/03/11  02:35:21  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:57  marty]
#
# Revision 1.1.6.1  1995/12/11  21:49:47  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:48  root]
# 
# Revision 1.1.4.2  1992/12/31  17:32:06  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:57:25  htf]
# 
# Revision 1.1.2.2  1992/03/27  20:58:26  bmw
# 	Added tests rgy_add.sh, rgy_change.sh, rgy_delete.sh, and
# 	rgy_policy.sh which were missing.
# 	[1992/03/27  20:51:26  bmw]
# 
# Revision 1.1  1992/01/19  04:07:11  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# 
# 
# Copyright 1991 by Apollo Computer, Inc.
# 
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Apollo Computer, Inc not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Apollo Computer, Inc. makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
# 
#
# src/test/security/api/moretests/rgy_edit_test.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 16:40:58
#	from delta on	8/5/91 14:44:36
#

/bin/sh rgy_add.sh $*
/bin/sh rgy_policy.sh $*
/bin/sh rgy_change.sh $*
/bin/sh rgy_delete.sh $*
/bin/sh rgy_person.sh $*
/bin/sh rgy_group.sh $*
/bin/sh rgy_org.sh $*
/bin/sh rgy_account.sh $*
/bin/sh rgy_member.sh $*
/bin/sh rgy_misc.sh $*

