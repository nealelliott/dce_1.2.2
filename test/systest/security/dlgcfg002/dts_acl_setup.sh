# @OSF_COPYRIGHT@
# Copyright (c) 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# 
# HISTORY
# $Log: dts_acl_setup.sh,v $
# Revision 1.1.4.2  1996/03/11  02:47:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:56  marty]
#
# Revision 1.1.4.1  1995/12/11  22:45:02  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:21:23  root]
# 
# Revision 1.1.2.2  1994/10/18  18:51:29  agd
# 	expand copyright
# 	[1994/10/18  17:39:37  agd]
# 
# Revision 1.1.2.1  1994/09/29  21:02:48  bhaim
# 	submit dlgcfg002 test
# 	[1994/09/29  21:01:26  bhaim]
# 
# $EndLog$
acl_edit /.:/hosts/clam/dts-entity -m user:dts_test:rwc -m user_delegate:dts_svr:rwc
