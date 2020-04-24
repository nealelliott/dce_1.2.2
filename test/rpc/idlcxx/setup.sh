#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: setup.sh,v $
# Revision 1.1.2.1  1996/09/09  16:30:21  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  16:20:06  bfc]
#
# $EndLog$
#
# Setup the idltest directory, and some auth stuff for the refmon
# test.  To be run from the obj/<machine>/test/rpc/idlcxx directory.
#
mkdir all	# for TET execution
rm -f dce_login_noexec
ln -s `which dce_login` dce_login_noexec
KRB5CCNAME=`dce_login_noexec cell_admin -dce-`
export KRB5CCNAME
#
cdscp create dir /.:/idltest
acl_edit /.:/idltest << EOS
m unauthenticated:rwdtcia
m any_other:rwdtcia
co
EOS
