#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: hacl_reset.sh,v $
# Revision 1.1.4.2  1996/03/11  02:29:40  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:22  marty]
#
# Revision 1.1.4.1  1995/12/11  15:18:29  root
# 	Submit
# 	[1995/12/11  14:33:10  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:51  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:30  melman]
# 
# Revision 1.1.2.1  1994/07/20  19:57:52  ganni
# 	initial version
# 	[1994/07/20  19:38:50  ganni]
# 
# $EndLog$
#
#

/opt/dcelocal/bin/acl_edit $1 -m unauthenticated:cri >/tmp/acl_k.out

