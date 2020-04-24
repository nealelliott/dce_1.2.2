#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: gss-test-setup.sh,v $
# Revision 1.1.4.1  1996/10/03  18:49:01  arvind
# 	DCE 1.2.2 branch
# 	[1996/08/08  20:51 UTC  mullan_s  /main/mullan_dce122_bugs_1/1]
#
# 	Remove TET calls from this script as it is no longer a TET testcase.
# 	[1996/03/11  02:08:02  marty  1.1.2.2]
#
# Revision 1.1.2.2  1996/03/11  02:08:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:51:00  marty]
# 
# Revision 1.1.2.1  1995/12/11  19:54:06  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/01/27  14:58 UTC  mdf
# 	Functional test changes
# 
# 	HP revision /main/HPDCE02/mdf_MOTHRA_2/3  1995/01/27  14:24 UTC  mdf
# 	More changes to make output of tests look reasonable.
# 
# 	HP revision /main/HPDCE02/mdf_MOTHRA_2/2  1995/01/25  19:45 UTC  mdf
# 	More fixes.
# 
# 	HP revision /main/HPDCE02/mdf_MOTHRA_2/1  1995/01/25  19:37 UTC  mdf
# 	Fixes.
# 
# 	HP revision /main/HPDCE02/1  1995/01/20  13:42 UTC  mdf
# 	Correct the location of GSSAPI functional tests
# 
# 	HP revision /main/mdf_MOTHRA_1/1  1995/01/19  18:39 UTC  mdf
# 	[1995/12/11  19:27:29  root]
# 
# Revision 1.1.2.2  1994/08/10  19:23:10  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:27  annie]
# 
# Revision 1.1.2.1  1994/07/20  14:55:21  wray
# 	Make tet-compliant
# 	[1994/07/20  14:54:34  wray]
# 
# $EndLog$
#
#
cellpw=-dce-

test_setup()
{
    #echo "Adding accounts for GSSAPI test: gss-test-1 & gss-test-2"
    #
    if [ ${cellpw:="NULL"} = "NULL" ]; then
       echo "Please supply the cell_admin password:"
       stty -echo 2>/dev/null
       read cellpw
       stty echo 2>/dev/null
       echo "\n"
    fi
    #
    #echo "Adding principals..."
    rgy_edit -update > /dev/null <<- EOF1
	domain principal
	add gss-prin-1
	add gss-prin-2
	domain account
	add gss-prin-1 -g none -o none -pw gss-pw-1 -mp $cellpw
	add gss-prin-2 -g none -o none -pw gss-pw-2 -mp $cellpw
	ktadd -p gss-prin-2 -pw gss-pw-2 -f /krb5/gss-keytab
	quit
EOF1
    #
    #echo "Setup phase completed"
    #
}
    
test_setup
