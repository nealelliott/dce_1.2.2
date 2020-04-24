#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: add_root_as_admin.ksh,v $
# Revision 1.1.6.2  1996/03/11  02:43:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:07:13  marty]
#
# Revision 1.1.6.1  1995/12/11  21:59:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:58:36  root]
# 
# Revision 1.1.4.1  1994/06/10  20:48:49  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:02  devsrc]
# 
# Revision 1.1.2.1  1993/10/01  21:04:51  gmd
# 	CR 8891 - to increase ease of use, removed instructions
# 	from README.RWALL and created this script from them. This
# 	script (run as cell_admin) will increase the dce principal
# 	root's privileges to "admin" level w/r/to the registry and
# 	dfs. This is useful when dfs write access to a non-LFS fileset
# 	is needed.
# 	[1993/10/01  20:54:17  gmd]
# 
# 	CR 8891 Porting changes plus cleanup:
# 	        Removed UNIX user setup code
# 	        Added variables for HP-UX commands/syntax
# 	        Added more verification routines
# 	        Changed exitting code
# 	[1993/10/01  20:42:16  gmd]
# 
# $EndLog$
###################################################################
#
# Function: add_root_as_admin
# Add root to admin groups so root can
# do dfs account and fileset setup.
#
###################################################################

RGY_EDIT=${rgy_edit:-"/opt/dcelocal/bin/rgy_edit"}

add_root_as_admin()
{
${RGY_EDIT} -update << ==END==
domain group
member acct-admin -a root
member subsys/dce/sec-admin -a root
member subsys/dce/dfs-admin -a root
quit
==END==
}

add_root_as_admin
exit
