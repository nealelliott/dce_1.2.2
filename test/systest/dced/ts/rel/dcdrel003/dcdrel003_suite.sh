#! /bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: dcdrel003_suite.sh,v $
# Revision 1.1.4.2  1996/03/11  02:38:43  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:30  marty]
#
# Revision 1.1.4.1  1995/12/11  21:56:02  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:54  root]
# 
# Revision 1.1.2.2  1994/06/27  18:26:50  baum
# 	Fixed install path and moved setup and misc.tcl files to lib directory
# 	[1994/06/27  18:26:23  baum]
# 
# Revision 1.1.2.1  1994/06/27  17:12:46  baum
# 	Changed names from 03 -> 003
# 	[1994/06/27  17:09:43  baum]
# 
# $EndLog$

# 1. Start ACL test alone.
# 2. Start ktb/hd/svrconf together
# 3. When they are all done, start ACL test again.

VTALRM=26
trap 'echo "dcdrel003_suite.sh: Caught VTALRM..."' $VTALRM
OUTFILE=SYSTEST.OUT

dcdrel003_acl.tcl /.:/hosts/`uname -n` $OUTFILE

dcdrel003_hd.tcl /.:/hosts/`uname -n` $OUTFILE & 
dcdrel003_ktb.tcl /.:/hosts/`uname -n` $OUTFILE &
dcdrel003_svrconf.tcl /.:/hosts/`uname -n` $OUTFILE &

dcdrel003_hd.tcl /.:/hosts/`uname -n` $OUTFILE &
dcdrel003_ktb.tcl /.:/hosts/`uname -n` $OUTFILE &
dcdrel003_svrconf.tcl /.:/hosts/`uname -n` $OUTFILE &

wait

dcdrel003_acl.tcl /.:/hosts/`uname -n` $OUTFILE
