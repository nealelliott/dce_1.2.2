# @OSF_COPYRIGHT@
# Copyright (c) 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# 
# HISTORY
# $Log: dts_rgy_setup.sh,v $
# Revision 1.1.4.2  1996/03/11  02:47:58  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:57  marty]
#
# Revision 1.1.4.1  1995/12/11  22:45:11  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:21:27  root]
# 
# Revision 1.1.2.2  1994/10/18  18:51:34  agd
# 	expand copyright
# 	[1994/10/18  17:40:00  agd]
# 
# Revision 1.1.2.1  1994/09/29  21:02:53  bhaim
# 	submit dlgcfg002 test
# 	[1994/09/29  21:01:29  bhaim]
# 
# $EndLog$
rgy_edit <<EOF
do p
add dts_test
add dts_svr
do a
add dts_test -g none -o none -mp -dce- -pw dts_test
add dts_svr -g none -o none -mp -dce- -pw dts_svr
kta -p dts_svr -pw dts_svr
kta -p dts_test -pw dts_test
quit
EOF

