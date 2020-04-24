#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: acct_delete_osf.ksh,v $
# Revision 1.1.4.2  1996/03/11  02:48:04  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:09:00  marty]
#
# Revision 1.1.4.1  1995/12/11  22:45:20  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:21:30  root]
# 
# Revision 1.1.2.2  1994/10/06  20:39:30  agd
# 	expand copyright
# 	[1994/10/06  14:31:05  agd]
# 
# Revision 1.1.2.1  1994/09/19  18:21:00  bhaim
# 	multidelegate test
# 	[1994/09/19  17:52:15  bhaim]
# 
# $EndLog$
set -x
rgy_edit -update<<EOF
do a
ktd -p string-conv -v 1 
ktd -p string-conv2 -v 1 
ktd -p string-conv3 -v 1 
ktd -p string-conv4 -v 1 
ktd -p string-conv5 -v 1 
ktd -p string-conv6 -v 1 
ktd -p odss-delegation-princ -v 1 
do p
del string-conv
del string-conv2 
del string-conv3
del string-conv4
del string-conv5
del string-conv6
del odss-delegation-princ
EOF
