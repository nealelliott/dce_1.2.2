# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: test_setup.tcl,v $
# Revision 1.1.2.2  1996/10/03  18:40:36  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  18:14:19  arvind]
#
# Revision 1.1.2.1  1996/08/09  21:24:44  arvind
# 	Update OSF copyright years
# 	[1996/03/10  19:27:32  marty]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:50:34  root]
# 
# 	moved admin tests to test/functional
# 	[1994/09/28  21:05:53  melman]
# 
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:51:59  devsrc]
# 
# 	Removed unneeded definitions.
# 	[1994/05/06  15:37:45  rousseau]
# 
# 	Ignore signal 26 (SIGVTALARM).
# 	[1994/03/07  23:01:10  rousseau]
# 
# 	Initial test drop.
# 	[1994/02/10  23:14:53  rousseau]
# 
# 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
# 	[1996/08/09  19:57:24  arvind]
# 
# $EndLog$

# Boiler plate setup for DCECP functional tests.

# Largest system supported signal + 1
set TET_NSIG 33

#
# KRB5/RLOGIN & RSH test related environment variables
#

# cell admin's
set env(KRB5_CELL_ADMIN) cell_admin
set env(KRB5_CELL_ADMIN_PW) -dce-

# test user name has to be less than 8 char for satisfying AIX limitations
set env(KRB5_TESTER) tester
set env(KRB5_TESTER_PW) tester

