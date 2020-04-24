#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: setup.ksh,v $
# Revision 1.1.8.3  1996/03/11  02:07:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:50  marty]
#
# Revision 1.1.8.2  1995/12/11  19:53:01  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/01/11  14:34 UTC  mothra
# 	rgy_edit needs to be -update not -u
# 
# 	HP revision /main/HPDCE02/2  1995/01/10  20:29 UTC  mgm
# 	change rgy_edit to rgy_edit -u so that tests work with replica
# 
# 	HP revision /main/HPDCE02/1  1994/12/05  18:59 UTC  mullan_s
# 
# 	HP revision /main/mullan_mothra_schema_fixes/1  1994/12/05  18:57 UTC  mullan_s
# 	Change CDS location of pwd strength server.
# 	[1995/12/11  19:27:03  root]
# 
# Revision 1.1.4.1  1994/10/19  20:27:31  mob
# 	OT 11533 - Change from attr_update to dcecp syntax in setup.
# 	[1994/10/19  19:25:40  mob]
# 
# Revision 1.1.2.2  1994/10/06  20:34:54  agd
# 	expand copyright
# 	[1994/10/06  14:29:44  agd]
# 
# Revision 1.1.2.1  1994/08/04  16:15:43  mdf
# 	Initial checkin
# 	[1994/08/03  19:21:19  mdf]
# 
# $EndLog$
#

# Create pwd_gen account

rgy_edit -update << !
do p
add "pwd_gen" 
do a
add "pwd_gen" -g "none" -o "none" -mp "-dce-" -pw "pwd_gen"
!

if [[ $? != 0 ]]
then
    exit 1
fi


# Attach pwd_mgmt_binding ERA
dcecp -c "principal modify pwd_gen -add {pwd_mgmt_binding {{dce pwd_strength pktprivacy secret name} {/.:/subsys/dce/pwd_mgmt/pwd_strength}}}"

if [[ $? != 0 ]]
then
    exit 1
fi
