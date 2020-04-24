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
# $Log: all_rgy_edit.sh,v $
# Revision 1.1.6.2  1996/03/11  02:34:20  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:20  marty]
#
# Revision 1.1.6.1  1995/12/11  21:48:05  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:07  root]
# 
# Revision 1.1.4.2  1992/12/31  17:28:23  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:53:51  htf]
# 
# Revision 1.1.2.2  1992/03/27  20:58:08  bmw
# 	      Added missing tests and reordered tests. This now corresponds
# 	to rgy_edit_test.sh.
# 	[1992/03/27  20:50:40  bmw]
# 
# Revision 1.1  1992/01/19  04:08:07  devrcs
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
# src/test/security/api/moretests/all_rgy_edit.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 16:58:24
#	from delta on	11/21/91 21:42:30
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

